/*  $Id: ddvgraph.c,v 1.41 2003/04/21 15:30:21 kans Exp $
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
* File Name:  ddvgraph.c
*
* Author:  Patrick Durand
*
* Version Creation Date:   06/19/99
*
* $Revision: 1.41 $
*
* File Description: graphic engine of DeuxD-Viewer (DDV)
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: ddvgraph.c,v $
* Revision 1.41  2003/04/21 15:30:21  kans
* use consistent newline character
*
* Revision 1.40  2001/04/27 20:12:46  juran
* Heed warnings:  Make functions static.
Block out 'dead code' (functions that aren't called anywhere in ncbi).
*
* Revision 1.39  2000/07/24 22:00:08  hurwitz
* fixed bug that 1/2 characters were displayed sometimes
*
* Revision 1.38  2000/07/19 19:04:01  hurwitz
* fixed bug that was causing overwrite of numbering over tick marks in DDE
*
* Revision 1.37  2000/07/07 22:43:55  lewisg
* interface tweaks
*
* Revision 1.36  2000/07/07 22:31:14  lewisg
* interface tweaks
*
* Revision 1.35  2000/05/16 19:43:00  hurwitz
* grey out create block, delete block, undo, and redo as needed
*
* Revision 1.34  2000/05/09 22:24:06  hurwitz
* extend ruler to cover tails even when there's only 1 aligned block in the alignment
*
* Revision 1.33  2000/05/09 19:53:17  hurwitz
* extended ruler to cover tails
*
* Revision 1.32  2000/04/26 21:54:27  hurwitz
* added save function to tell AlnMgr about edits made in DDE
*
* Revision 1.31  2000/04/05 20:52:35  hurwitz
* added GUI control for shifting left and right alignment boundaries
*
* Revision 1.30  2000/03/29 23:57:04  hurwitz
* temporary fix for DDE use before it's initialized
*
* Revision 1.29  2000/03/29 20:02:48  hurwitz
* keep track of master during move row operations
*
* Revision 1.28  2000/03/24 20:34:59  lewisg
* add blast from file, bug fixes, get rid of ant code, etc.
*
* Revision 1.27  2000/02/23 19:49:33  durand
* use row number instead of SeqId for coloring
*
* Revision 1.26  2000/02/15 15:31:46  lewisg
* move DDVRulerDescr to pgppop
*
* Revision 1.25  2000/02/08 17:18:40  hurwitz
* don't use char* szEditSeq
*
* Revision 1.24  2000/02/07 14:03:35  durand
* replace BioseqUnlockById by BioseqUnlock
*
* Revision 1.23  2000/02/04 16:05:40  durand
* add click action to select a row
*
* Revision 1.22  2000/02/03 15:56:47  hurwitz
* added constructor and destructor for DDVRulerDescrPtr
*
* Revision 1.21  2000/02/02 14:44:31  durand
* added function to create data structure for block editor, fixed some bugs
*
* Revision 1.20  2000/01/26 13:38:54  durand
* update the GUI for the editor. Add functions to create the data to be used by the editor
*
* Revision 1.19  2000/01/24 16:11:13  lewisg
* speed up seqid comparison in color manager, make fast windows version of SetColor()
*
* Revision 1.18  2000/01/10 15:09:46  durand
* Use Entrez instead of ID1
*
* Revision 1.17  2000/01/05 21:11:14  durand
* update mouse click actions and DrawSequence function for a better use from ddv and cn3d
*
* Revision 1.16  1999/12/29 22:55:03  lewisg
* get rid of seqalign id
*
* Revision 1.15  1999/12/07 21:40:14  durand
* add mouse modes menu and caret facility for the editor
*
* Revision 1.14  1999/11/29 15:26:25  durand
* designed a new GUI to fix problems under MacOS, Linux and SGI
*
* Revision 1.13  1999/11/23 17:34:33  lewisg
* SavePort added to redraw
*
* Revision 1.12  1999/11/18 14:37:14  durand
* avoid flashing sequence during selection
*
* Revision 1.11  1999/11/09 17:09:00  durand
* transfer some functions from ddvgraph to ddvcreate, so that ddvcreate remains Vibrant free and can be compiled with BLAST
*
* Revision 1.10  1999/11/03 21:29:49  durand
* add CTRL and SHFT keys for mouse selection. redesign the loader functions of DDV to properly register the message callbacks
*
* Revision 1.9  1999/10/29 14:15:38  durand
* add simple mouse selection functions
*
* Revision 1.8  1999/10/21 13:12:12  durand
* fix a problem in panel draw callback
*
* Revision 1.7  1999/10/20 13:17:18  durand
* add display for disc. SeqAlign tails
*
* Revision 1.5  1999/10/18 17:20:56  durand
* fix a bug in the bsp coord mapping for minus strand
*
* Revision 1.4  1999/10/15 21:57:36  durand
* add a UI for display options
*
* Revision 1.3  1999/10/12 15:01:29  lewisg
* resolve confict with internal/ddv
*
* Revision 1.2  1999/10/02 15:11:21  durand
* update the code to reuse more functions from UDV
*
* Revision 1.1  1999/09/30 14:10:26  durand
* add ddv to toolkit
*
* Revision 1.16  1999/09/22 20:40:20  durand
* update the drawing procedure to deal with discontinuous seqalign
*
* Revision 1.15  1999/09/22 13:17:15  wheelan
* Changed DDV_InitColour_When_Start to use new AlnMgrGetNextNthSeqRange function (better for discontinuous)
*
* Revision 1.14  1999/09/20 13:39:04  durand
* replace totsaps by alnsaps (new AlignMgr stuff)
*
* Revision 1.13  1999/09/16 13:07:07  durand
* DDV is able to display bioseq coords along with seqalign coords
*
* Revision 1.11  1999/09/09 21:54:23  durand
* create a display for disconitnuous SeqAlign
*
* Revision 1.10  1999/09/01 23:02:59  lewisg
* binary search in color functions
*
* Revision 1.9  1999/08/06 14:32:18  wheelan
* changes to support new alignmgr function names
*
* Revision 1.8  1999/08/04 18:01:47  wheelan
* changes to support new seqalign indexing
*
* Revision 1.7  1999/07/22 15:25:27  durand
* improve DDV_DrawSequence function
*
* Revision 1.6  1999/07/20 14:58:00  durand
* use the Color Manager to display colored MSA
*
* Revision 1.5  1999/07/06 19:02:21  durand
* update DDV_GetSequenceFromParaG
*
* Revision 1.4  1999/06/30 14:57:23  durand
* update DDV loader functions
*
* Revision 1.3  1999/06/28 22:07:18  durand
* add loader functions and clean the code with Lint and Purify
*
* Revision 1.2  1999/06/19 18:36:44  durand
* update display procedure
*
* Revision 1.1  1999/06/19 17:21:05  durand
* add Vibrant DDV code
*
*
*
* ==========================================================================
*/

#include <ncbi.h>
#include <vibrant.h>
#include <udviewer.h>
#include <udvseq.h>
#include <ddvcolor.h>
#include <ddvmain.h>
#include <ddvgraph.h>
#include <ddvclick.h>
#include <pgppop.h>
#include <alignmgr.h>
#include <ddvcreate.h>

#undef WANTDEADCODE

static void DDV_DrawAlignmentBoundaries(PaneL p, DdvMainPtr dmp);

/*******************************************************************************

  Function : DDV_ReorderParaG()
  
  Purpose : reorder ParaG positions after style change(s)
    
  Return value : -

*******************************************************************************/
#ifdef WANTDEADCODE
static void DDV_ReorderParaG(ValNodePtr PNTR TableHead, Int4 nRow)
{
Int4       Startline=0,i,nLines;
ParaGPtr   pgp;
ValNodePtr vnp;

	for(i=0;i<nRow;i++){
		vnp=TableHead[i];
		pgp=(ParaGPtr)vnp->data.ptrvalue;
		if (pgp->ScaleStyle==SCALE_POS_TOP)
			nLines++;
		nLines=1;
		while(vnp){
			pgp=(ParaGPtr)vnp->data.ptrvalue;
			pgp->StartLine=Startline;
			pgp->nLines=nLines;
			vnp=vnp->next;
		}
		Startline+=nLines;
	}
}
#endif

/*******************************************************************************

  Function : DDV_ChangeRulerStyleForSeq()
  
  Purpose : change the ruler style for a row in the alignment; ie for a Seq.
    
  Return value : -

*******************************************************************************/
#ifdef WANTDEADCODE
static void DDV_ChangeRulerStyleForSeq(ValNodePtr PNTR TableHead,Int4 nRow,
		Uint1 RulerStyle)
{
ParaGPtr   pgp;
ValNodePtr vnp;

	vnp=TableHead[nRow];
	while(vnp){
		pgp=(ParaGPtr)vnp->data.ptrvalue;
		pgp->ScaleStyle=RulerStyle;
		vnp=vnp->next;
	}
}
#endif

/*******************************************************************************

  Function : DDV_InvalRegion()
  
  Purpose : invalidate a specific region of DDV panel

  Note : disp_from,disp_to,disp_row are display coordinates. 
         disp_from,disp_to are zero-based. 
         disp_row is one-based  
		 
  Return value : the sequence 

*******************************************************************************/
extern void DDV_InvalRegion(PaneL hWndDDV,UnDViewerGraphDataPtr GrData,
		Int4 disp_from,Int4 disp_to,Int4 disp_row,Boolean IsSelect)
{
DdvMainPtr dmp;
RecT       rcP,rc;
Int2       from_x,to_x,from_y,to_y;
WindoW     temport;

	dmp = (DdvMainPtr) GetObjectExtra(hWndDDV);
	if (dmp==NULL) return;

	/*convert disp coord to pixel positions*/
		/*left of the first letter in the range*/
	from_x=(Int2)((disp_from-GrData->udv_hscrl.ScrollPos)*GrData->udv_font.ColWidth);
		/*right of the last letter in the range*/
	to_x=(Int2)((disp_to-GrData->udv_hscrl.ScrollPos+1)*GrData->udv_font.ColWidth);
		/*just above the row*/
	from_y=(Int2)((disp_row-GrData->udv_vscrl.ScrollPos-1)*GrData->udv_font.LineHeight);
		/*just below the row*/
	to_y=(Int2)(from_y+2*GrData->udv_font.LineHeight);

	/*adjust the values according to the PaneL RecT position*/
		/*panel size*/
    temport=SavePort(ParentWindow(hWndDDV));
    Select(hWndDDV);
	ObjectRect(hWndDDV,&rcP);
	InsetRect(&rcP,4,4);
		/*drawing region:*/
	DDV_AdjustDrawingRect(&rcP,&(GrData->udv_font),dmp);
	
	rcP.left+=GrData->udv_panel.cxName+GrData->udv_scale.cxLeftScale;
	rcP.top+=3*GrData->udv_panel.cyScale/2;
	from_x+=rcP.left-1;
	to_x+=rcP.left;
	from_y+=rcP.top+1;
	to_y+=rcP.top;

	rc.left=from_x-2;
	rc.top=from_y-2;
	rc.right=to_x+2;
	rc.bottom=to_y+2;

	/*InvalRect(&rc);*/
	/*DDV_DrawPanelContent_H (hWndDDV,dmp,&rc,IsSelect);*/

	if (!IsSelect){
		/*printf("invalRect\n");*/
		InvalRect(&rc);
	}
	else{
		DDV_DrawPanelContent_H (hWndDDV,dmp,&rc,IsSelect);
	}
/*	Update();*/
    RestorePort(temport);
}

/*******************************************************************************

  Function : DDV_GetBspRangeInSeg()
  
  Purpose : scan a segment and get the region of a Bsp in the SeqAlign
    
  Return value : the sequence 

*******************************************************************************/

#ifdef WANTDEADCODE
static void DDV_GetBspRangeInSeg(SegmentPtr segp,Int4Ptr start,Int4Ptr stop)
{
SegmentPtr segp2;
Int4       from,to;

	if (!segp) return;
	from=INT4_MAX;
	to=INT4_MIN;
	segp2=segp;
	while(segp2){
		if (segp2->gap==0) {
			from=MIN(from,segp2->from);
			to=MAX(to,segp2->to);
		}
		segp2=segp2->bsp_next;
	}
	*start=from;
	*stop=to;
}
#endif


/*******************************************************************************

  Function : DDV_GetCurrentDispRange()
  
  Purpose : compute the size (row/col) of the DDV panel
  
  Return value : see arguments (zero-based values)

*******************************************************************************/
extern void DDV_GetCurrentDispRange(PaneL hWndDDV,UnDViewerGraphDataPtr GrData,
		Int4 LengthAli,Int4Ptr from_col,Int4Ptr to_col,Int4Ptr from_row,
		Int4Ptr to_row)
{

  RecT        rcP;
  DdvMainPtr  dmp;

  dmp = (DdvMainPtr) GetObjectExtra(hWndDDV);
	if (dmp==NULL) return;

	/*panel size*/
	ObjectRect(hWndDDV,&rcP);
	InsetRect(&rcP,4,4);

	/*drawing region:*/
	DDV_AdjustDrawingRect(&rcP,&(GrData->udv_font),dmp);
	
	rcP.left+=GrData->udv_panel.cxName+GrData->udv_scale.cxLeftScale;
	rcP.top+=GrData->udv_panel.cyScale;
	
	*from_col=0;
	*to_col=(rcP.right-rcP.left)/GrData->udv_font.ColWidth-1;
	*from_col+=GrData->udv_hscrl.ScrollPos;
	*to_col+=GrData->udv_hscrl.ScrollPos;
	if (*to_col>LengthAli) *to_col=LengthAli-1;
	if (*from_col>LengthAli) *from_col=LengthAli-1;
	*from_row=0;
	*to_row=(rcP.bottom-rcP.top)/GrData->udv_font.LineHeight-1;
	*from_row+=GrData->udv_vscrl.ScrollPos;
	*to_row+=GrData->udv_vscrl.ScrollPos;
	if (*from_row>(GrData->udv_panel.nTotLines-1)) *from_row=GrData->udv_panel.nTotLines-1;
	if (*to_row>(GrData->udv_panel.nTotLines-1)) *to_row=GrData->udv_panel.nTotLines-1;

}

/*******************************************************************************

  Function : DDV_GetRulerForEditor()
  
  Purpose : create the ruler for the editor. This new descriptor is created
      using the descriptor of the viewer.

  Return value : an allocated data block ready for use by the editor. Null if
  failure

*******************************************************************************/
NLM_EXTERN ValNodePtr DDV_GetRulerForEditor(ValNodePtr descr_head,Int4 from_disp,
		Int4 to_disp)
{
DDVRulerDescrPtr drdp,prev_drdp,new_drdp;
ValNodePtr  vnp2,vnp3,vnpDesc;
Int4     cumulDecr,drdp_size,beginCopy,endCopy,drdp_start_ali;
Boolean  bPop;

	cumulDecr=0;
	vnpDesc=NULL;
	prev_drdp=NULL;
	
	for(vnp2=descr_head;vnp2!=NULL;vnp2=vnp2->next){
		drdp=(DDVRulerDescrPtr)vnp2->data.ptrvalue;
		drdp_size=drdp->disp_stop-drdp->disp_start+1;

		/*not yet in the region of interest ?*/
		if (drdp->disp_stop<from_disp) {
			cumulDecr+=drdp_size;
			continue;
		}
		
		beginCopy=_max_(from_disp,drdp->disp_start);
		endCopy=_min_(drdp->disp_stop,to_disp);

		if (drdp->bUnAligned==FALSE)
			drdp_start_ali=drdp->align_start+(beginCopy-drdp->disp_start);
		else
			drdp_start_ali=(Int4)-1;
		/*in order to merge nodes of same type, i keep track of
		the previous populated drdp node. Same style==yes, then
		just extend to the right the previous node*/
		if (prev_drdp){
			if (prev_drdp->bUnAligned==drdp->bUnAligned){
				prev_drdp->disp_stop=endCopy;
				bPop=TRUE;
			}
			else{
				bPop=FALSE;
			}
		}
		else{
			bPop=FALSE;
		}
		if (!bPop){
			new_drdp=(DDVRulerDescrPtr)MemNew(sizeof(DDVRulerDescr));
			if (!new_drdp) goto erreur;
			
			new_drdp->disp_start=beginCopy;
			new_drdp->disp_stop=endCopy;
			new_drdp->align_start=drdp_start_ali;
			new_drdp->bUnAligned=drdp->bUnAligned;
			if (!vnpDesc){
				vnp3=ValNodeAddPointer(&vnpDesc,0,(Pointer)new_drdp);
			}
			else{
				vnp3=ValNodeAddPointer(&vnp3,0,(Pointer)new_drdp);
			}
			prev_drdp=new_drdp;
		}
		if (drdp->disp_stop>=to_disp) break;
		cumulDecr+=drdp_size;
	}

	return(vnpDesc);
erreur:
	if (vnpDesc)
		ValNodeFreeData(vnpDesc);
	return(NULL);
}


/*******************************************************************************

  Function : DDV_ComputeRuler()
  
  Purpose : compute the Ruler descriptor (usefull for discontinuous align)
  
  Parameters : 
  
  Return value : a list of RUler descriptor

*******************************************************************************/
extern ValNodePtr DDV_ComputeRuler(SeqAlignPtr sap,DDV_Disp_OptPtr ddop)
{

DDVRulerDescrPtr drdp;
ValNodePtr       vnp=NULL,vnp_head=NULL;
Int4             disp_start=0,length,TotAliLength=0,r=0;
Boolean          bUnAligned;

  /* make ruler for LEFT_TAIL */
  if ((ddop->DispDiscStyle == MSA_TXT_STYLE_2) && (ddop->ShowLeftTail)) {
    length = AlnMgrGetMaxTailLength(sap, LEFT_TAIL);
    if (length) {
	    drdp=(DDVRulerDescrPtr)MemNew(sizeof(DDVRulerDescr));
	    drdp->disp_start=disp_start;
	    drdp->disp_stop=disp_start+length-1;
	    drdp->bUnAligned=TRUE;
      drdp->align_start = -1;
      disp_start += length;
	    vnp_head=ValNodeAddPointer(NULL,0,(Pointer)drdp);
	    vnp=vnp_head;
    }
  }

  if (AlnMgrIsSAPDiscAli(sap)){
    /* make ruler for multiple blocks */
		while(AlnMgrGetNextLengthBit(sap,&length,&r)){
			if (length<0){
				bUnAligned=TRUE;
				switch(ddop->DispDiscStyle){/*user's display choice*/
					case MSA_TXT_STYLE_1:
						length=ddop->SpacerSize;
						break;
					case MSA_TXT_STYLE_2:
						length=ABS(length);
						break;
				}
			}
			else{
				bUnAligned=FALSE;
			}

			drdp=(DDVRulerDescrPtr)MemNew(sizeof(DDVRulerDescr));
			drdp->disp_start=disp_start;
			drdp->disp_stop=disp_start+length-1;
			drdp->bUnAligned=bUnAligned;
			
			if (bUnAligned==FALSE)
				drdp->align_start=TotAliLength;/*SeqAlign Coord*/
			else
				drdp->align_start=-1;/*drdp->disp_start;*//*Disp Coord*/

			if (!vnp_head){
				vnp_head=ValNodeAddPointer(NULL,0,(Pointer)drdp);
				vnp=vnp_head;
			}
			else{
				vnp=ValNodeAddPointer(&vnp,0,(Pointer)drdp);
			}
			disp_start+=length;
			if (bUnAligned==FALSE) 
				TotAliLength+=length;
		}
	}

  else if (sap->type == SAT_MASTERSLAVE){
    /* make ruler for a single block */
		drdp=(DDVRulerDescrPtr)MemNew(sizeof(DDVRulerDescr));
		drdp->disp_start=disp_start;
		length=AlnMgrGetAlnLength(sap,FALSE);
		drdp->disp_stop=disp_start+length-1;
    drdp->bUnAligned = FALSE;
		drdp->align_start=0;
		if (!vnp_head){
			vnp_head=ValNodeAddPointer(NULL,0,(Pointer)drdp);
			vnp=vnp_head;
		}
		else{
			vnp=ValNodeAddPointer(&vnp,0,(Pointer)drdp);
		}
	}

  /* make ruler for RIGHT_TAIL */
  if ((ddop->DispDiscStyle == MSA_TXT_STYLE_2) && (ddop->ShowRightTail)) {
    length = AlnMgrGetMaxTailLength(sap, RIGHT_TAIL);
    if (length) {
	    drdp=(DDVRulerDescrPtr)MemNew(sizeof(DDVRulerDescr));
	    drdp->disp_start=disp_start;
	    drdp->disp_stop=disp_start+length-1;
	    drdp->bUnAligned=TRUE;
      drdp->align_start = -1;
      disp_start += length;
	    vnp = ValNodeAddPointer(&vnp,0,(Pointer)drdp);
    }
  }

	return(vnp_head);
}

/*******************************************************************************

  Function : DDV_DrawSequenceName()
  
  Purpose : draw the name of the sequence (left column of the DDV panel)
  
  Parameters : GrData; graphical data (font size, etc)
				ScaleStyle;style of the ParaG scale
				top, left; coord to start the draw
  
  Return value : none

*******************************************************************************/
static void  DDV_DrawSequenceName(UnDViewerGraphDataPtr GrData,ParaGPtr pgp,
	Int2 top,Int2 left,Int4 cur_row,Int4 CurEditRow,Int4 CurMasterRow)
{
SeqIdPtr  sip = NULL;
RecT      rc;
Int2      x,y,decal=1,size;/*text position/size*/
Char      szAccess[21];
BioseqPtr bsp;

	/*get a name*/	
    bsp = BioseqLockById(pgp->sip);
    if(bsp) {
        sip = SeqIdFindBestAccession(bsp->id);
        BioseqUnlock(bsp);
    }
	if (!sip)
		sip = SeqIdFindBest(pgp->sip, 0);
	SeqIdWrite(sip, szAccess,PRINTID_TEXTID_ACCESSION, 20);   

	/*compute position*/
	if (pgp->ScaleStyle==SCALE_POS_TOP) decal++;
	
	/*draw name*/
	size=StringWidth(szAccess);
	x=left/*-GrData->udv_scale.cxLeftScale*/-size;
	y=top+decal*GrData->udv_font.LineHeight;
	MoveTo(x,y);
	if (cur_row==CurEditRow){
		Magenta();
	}
	PaintString (szAccess);
	if (cur_row==CurMasterRow){
		Blue();
		MoveTo(x,y);
		LineTo(x+size,y);
	}
	/*draw a little box (for selection a full sequence)*/
	left+=GrData->udv_font.cxChar;
	top+=GrData->udv_font.cxChar/2;
	LoadRect(&rc,left,top,left+GrData->udv_font.cxChar,
		top+GrData->udv_font.cxChar);	
	Blue();
	PaintOval(&rc);
	Black();
}


/*****************************************************************************

Function: DDV_DrawSequence()

Purpose: draw a sequence within a ParaG 

Return value: -

*****************************************************************************/
static void DDV_DrawSequence(UnDViewerGraphDataPtr GrData,ParaGPtr pgp,
		DDV_ColorGlobal *pColorGlobal,SeqIdPtr sip,RectPtr rc_pgp,
		CharPtr szSeq,Int4 from,Int4 to,Uint1 ScaleStyle,
		Int2 leftDecal,Uint1 strand,Boolean bUseColors,ValNodePtr vnp_bsp,
		Int4 row,Boolean bSelect)
{
DDV_ColorCell *dclrp,*highlighClr;
Int4            dp;
Uint4           old_colour,new_colour,hlClr;
Int2            decal=1,i,x,y,pos=0;
Boolean         isAlpha,bSelected,bGoTo;
RecT            rcSel;
	
	if (!szSeq) return;	
	
	/*scale or not... to be or not to be ?*/
	if (ScaleStyle==SCALE_POS_TOP) decal++;
	
	/*compute the first letter's coordinate*/
	x=rc_pgp->left+leftDecal-GrData->udv_font.cxChar/2;
	y=rc_pgp->top+decal*GrData->udv_font.LineHeight;

	i=from;/*if from!=0, means we don't draw the entire ParaG*/
	to++;

	/*get the selection(s)*/
	
	if (pColorGlobal){/*use color*/
		old_colour=new_colour=GetColorRGB(0,0,0);
	    highlighClr =
    	    DDV_SearchColorCellbyName(pColorGlobal->pvnSpecialColors, "Highlight");
		if (highlighClr)
			hlClr = GetColorRGB (highlighClr->rgb[0], highlighClr->rgb[1],highlighClr->rgb[2]);
		else 
			hlClr = GetColorRGB (255,255,0);
		while(szSeq[i] && i<to){
			/*if a letter, get the color*/
			if (szSeq[i]!=' '){
				if (isalpha(szSeq[i])){
					isAlpha=TRUE;
					dp=DDV_GetBspCoordGivenDispCoord(pgp,pgp->StartLetter+i);
					bSelected=DDV_IsLetterSelected(vnp_bsp,dp);
					if (dp==(Int4)-1)
						dclrp=NULL;
					else
						dclrp=DDV_GetColor(pColorGlobal,NULL, row+1, dp);

					if(bUseColors){
						if (dclrp) 
							new_colour=GetColorRGB(dclrp->rgb[0],dclrp->rgb[1],dclrp->rgb[2]);
						else
							new_colour=GetColorRGB(0,0,0);
					}
				}
				else {
					bSelected=FALSE;
					isAlpha=FALSE;
					new_colour=GetColorRGB(0,0,0);
				}
				if (GrData->GotoLetter==pgp->StartLetter+i){
					bGoTo=TRUE;bSelected=TRUE;
				}
				else{
					bGoTo=FALSE;
				}
				if (bSelected){
					if (bGoTo==FALSE){
						if (bSelect) 
#ifdef WIN_MSWIN
                            Nlm_SetColorEx(hlClr);
#else
                            Nlm_SetColor(hlClr);
#endif
						else White();
					}
					else{
						Red();
					}
					rcSel.left=x-2;
					rcSel.top=y-GrData->udv_font.LineHeight;
					rcSel.bottom=y;
					rcSel.right=rcSel.left+GrData->udv_font.ColWidth;
					PaintRect(&rcSel);
					/*FrameRect(&rcSel);*/
				}
				if ((bUseColors && new_colour!=old_colour) || bSelected) {
#ifdef WIN_MSWIN
                    Nlm_SetColorEx(new_colour);
#else
                    Nlm_SetColor(new_colour);
#endif
					old_colour=new_colour;
				}
				MoveTo(x,y);
				if (isAlpha && dclrp)
					PaintChar((dclrp->LowerCase ? TO_LOWER(szSeq[i]) : szSeq[i]));
				else
					PaintChar(szSeq[i]);
			}
			i++;
			x+=GrData->udv_font.ColWidth;
		}
	}
	else{/*B&W display mode*/
		Black();
		while(szSeq[i] && i<to){
			/*if a letter, get the color*/
			if (szSeq[i]!=' '){
				MoveTo(x,y);
				PaintChar(szSeq[i]);
			}
			i++;
			x+=GrData->udv_font.ColWidth;
		}
	}
	
	Black();
}

/*****************************************************************************

Function: DDV_DrawParaG()

Purpose: draw a ParaG given from, to (seqalign coord) and a position (rc_pgp) 

Return value: -

*****************************************************************************/
static void DDV_DrawParaG(RectPtr rc_pgp,ParaGPtr pgp,Int4 from,Int4 to,
			UnDViewerGraphDataPtr GrData,DDV_ColorGlobal *pColorGlobal,
			DDV_Disp_OptPtr ddop,ValNodePtr vnp_bsp,Int4 row,Boolean bSelect)
{
CharPtr   szSeq;
BioseqPtr bsp;
SeqIdPtr  sip;
Int4      bspLength,bsp_start,bsp_stop,from_pgp,to_pgp;
Int2 decal,diff;
Boolean   IsAA;
Uint1     strand;
MsaTxtDispPtr    mtdp;
ValNodePtr vnp;

	/*get some bsp info*/	
	/*sip=SeqIdFindBest(pgp->sip,0);
	if (sip==NULL)*/
	sip=pgp->sip;
	bsp=BioseqLockById(sip);
	if (!bsp) return;
	bspLength=BioseqGetLen(bsp);
	IsAA=ISA_aa(bsp->mol);
	BioseqUnlock(bsp);

	szSeq=(CharPtr)MemNew((pgp->StopLetter-pgp->StartLetter+2)*sizeof(Char));
	/*get a sequence to draw*/
	DDV_GetSequenceFromParaG(pgp,&szSeq,bspLength,IsAA,&strand,&bsp_start,&bsp_stop);
	
	if (pgp->ScaleStyle==SCALE_POS_TOP){
		vnp=pgp->ptxtList;
		decal=0;
		while(vnp){
			mtdp=(MsaTxtDispPtr)vnp->data.ptrvalue;
			diff=(Int2)(mtdp->to-mtdp->from+1);
			if (mtdp->TextStyle == MSA_TXT_STYLE_SEQ){
				UDV_Draw_scale(
					GrData,
					FALSE,
					FALSE,
					pgp->ScaleStyle,
					(Int4)decal,
					(Int4)decal+diff,
					rc_pgp,
					(Int2)(GrData->udv_font.ColWidth/2)+
					(Int2)(decal*GrData->udv_font.ColWidth),
					bspLength,
					mtdp->from,
					FALSE,
					GrData->udv_font.ColWidth,(Uint4)DDV_DISP_HORZ);
			}
			decal+=diff;
			vnp=vnp->next;
		}
	}
	
	if (szSeq){/*draw the sequence*/
		from_pgp=_max_(pgp->StartLetter,from);
		to_pgp=_min_(pgp->StopLetter,to);
		DDV_DrawSequence(GrData,
			pgp,
			pColorGlobal,
			sip,
			rc_pgp,
			szSeq,
			from_pgp-pgp->StartLetter,
			to_pgp-pgp->StartLetter,
			pgp->ScaleStyle,
			(Int2)(GrData->udv_font.ColWidth/2+
			(Int2)(from_pgp-pgp->StartLetter)*GrData->udv_font.ColWidth),
			strand,
			ddop->bUseColors,
			vnp_bsp,
			row,
			bSelect);
		if (pgp->szEditSeq==NULL)/*viewer deletes allocated char array*/
			MemFree(szSeq);
	}
}

/*****************************************************************************

Function: DDV_ComputeColWidth()

Purpose: compute the width of a single column 

Return value: the height

*****************************************************************************/
extern Int2 DDV_ComputeColWidth(Int2 cxChar)
{
	return(cxChar+2);
}

/*******************************************************************************

  Function : DDV_AdjustDrawingRect()
  
  Purpose :  adjust the Panel Rect using the size of a letter ; in that way,
             I get a RecT where it's possible to avoid the display of half letters
			 (on the borders of the RecT).
  
  Parameters :	rcP;panel RecT
  				udv_font; font information
    
  Return value : none

*******************************************************************************/
extern void	DDV_AdjustDrawingRect(RecT * rcP, UDVFontDataPtr udv_font, DdvMainPtr dmp)
{	
  Int4  temp;

	rcP->left=(rcP->left/udv_font->ColWidth)*udv_font->ColWidth + udv_font->ColWidth;

/*	rcP->right=(rcP->right/udv_font->ColWidth)*udv_font->ColWidth-2; */
  /* bug fix, DIH, 7/24/00 */
  temp = rcP->right - (dmp->GrData.udv_panel.cxName + dmp->GrData.udv_scale.cxLeftScale);
  temp = (temp/udv_font->ColWidth)*udv_font->ColWidth - 2;
  rcP->right = temp + (dmp->GrData.udv_panel.cxName + dmp->GrData.udv_scale.cxLeftScale);

	rcP->top=(rcP->top/udv_font->LineHeight)*udv_font->LineHeight + udv_font->LineHeight;
	rcP->bottom=(rcP->bottom/udv_font->LineHeight)*udv_font->LineHeight;
}


/*******************************************************************************

  Function : DDV_DrawPanelContent_H()
  
  Purpose :  draw the panel of DDV viewer (case 1: full horz display)
  
  Parameters :	p;handle of the panel
  				dmp; main data block
    
  Return value : none

*******************************************************************************/
extern void  DDV_DrawPanelContent_H (PaneL p,DdvMainPtr dmp,RecT PNTR MyUpdateRect,
	Boolean bSelect)
{
ParaGPtr     pgp,/*ParaG data*/
             first_pgp;/*used to get info from the first pgp of a row*/
ValNodePtr   vnp,/*used to scan ParaGand Ruler Descriptor*/
             vnp_pgp,/*used to get info from the first pgp of a row*/
			 vnp_bsp=NULL;/*selected region(s) of one bioseq*/
SelStructPtr ssp;/*used to get the selected region(s)*/
BioseqPtr    bsp;
Int4         i,/*just for a loop*/
             from_row,/*first row to draw*/
	         to_row,/*the last row to draw*/
	         top_offset=0,/*offset from top (pixel)*/
	         left_offset,/*offset from right (pixel)*/
	         from_col,/*draw column from_col...*/
	         to_col,/*...to column to_col*/
	         from_pgp,/*within a ParaG, draw 'from'... */
	         to_pgp,/*... 'to'*/
	         nLinesDraw,/*number of lines drawn*/
	         nColsDraw,/*number of col. drawn*/
	         nTotRow,/*total number of row to draw*/
	         start_i=0,/*first ParaG to draw*/
           start_ruler,/*beginning of the display ruler*/
           curMasterRow;
RecT         rcP;/*size of the DDV panel*/
RecT         rcD;/*size of the drawing panel*/
RecT         rcI;/*size of the invalidated rect*/
RecT         rc_pgp;/*location of a ParaG on the Panel*/
Int2         length_pgp,/* i.e. 'length' letters to draw*/
             x,y;/*caret position (edit mode only)*/
Uint2        bsp_eID,/*...*/
			 bsp_iID;/*... identifiers of a bioseq in the SeqAlign*/
Boolean      bNotDrawCol;
DdvMainWinPtr   mWin_d;
DDVRulerDescrPtr drdp;	
	
	if (dmp->hParent){
		mWin_d=(DdvMainWinPtr)GetObjectExtra(dmp->hParent);
        if (mWin_d->Show_logo){
            UDV_Logo_onDraw(p);
            return;
        }
	}

	/*some checks before a core dump...*/
	if (!dmp->MSA_d.pgp_l.TableHead) return;
	
	/*restrict panel drawing area: 'add' little margins*/
  ObjectRect((Handle) p, &rcP);
  InsetRect(&rcP,4,4);

	/*3D border to resize the cxName field*/
	LtGray();
	
	MoveTo((Int2)(dmp->GrData.udv_panel.cxName-1),rcP.top);
	LineTo((Int2)(dmp->GrData.udv_panel.cxName-1),rcP.bottom);

	LtGray();
	MoveTo((Int2)(dmp->GrData.udv_panel.cxName+1),rcP.top);
	LineTo((Int2)(dmp->GrData.udv_panel.cxName+1),rcP.bottom);
	LtGray();
	MoveTo((Int2)(dmp->GrData.udv_panel.cxName+2),rcP.top);
	LineTo((Int2)(dmp->GrData.udv_panel.cxName+2),rcP.bottom);

	Black();
	MoveTo((Int2)(dmp->GrData.udv_panel.cxName+3),rcP.top);
	LineTo((Int2)(dmp->GrData.udv_panel.cxName+3),rcP.bottom);

	/*drawing region:*/
	DDV_AdjustDrawingRect(&rcP,&(dmp->GrData.udv_font),dmp);

  /* debugging */
/*  UDV_draw_rectangle(rcP, TRUE); */
	
	rcD=rcI=rcP;
	rcP.left+=dmp->GrData.udv_panel.cxName+dmp->GrData.udv_scale.cxLeftScale;
	top_offset=dmp->GrData.udv_vscrl.ScrollPos*dmp->GrData.udv_font.LineHeight;
	left_offset=dmp->GrData.udv_hscrl.ScrollPos*dmp->GrData.udv_font.ColWidth;

	/*set the font*/
	SelectFont(dmp->GrData.udv_font.hFnt);
	
	rcD=rcP;
	ClipRect(&rcD);
	/*display the master numerical ruler*/
	from_col=dmp->GrData.udv_hscrl.ScrollPos;
	to_col=dmp->GrData.udv_hscrl.ScrollPos+dmp->GrData.udv_hscrl.ScrollPage;
	if (to_col==0) to_col=dmp->MSA_d.pgp_l.LengthAli;
	if (to_col>dmp->MSA_d.pgp_l.LengthAli) to_col=dmp->MSA_d.pgp_l.LengthAli;
	rcD.top+=dmp->GrData.udv_panel.cyScale/2;

	/*get the first node of the Ruler descriptor, then get the start value*/
	drdp=(DDVRulerDescrPtr)dmp->MSA_d.pgp_l.RulerDescr->data.ptrvalue;
	start_ruler=drdp->disp_start;
	UDV_Draw_scale(
		&dmp->GrData,
		dmp->GrData.udv_scale.ShowMajorTick,
		dmp->GrData.udv_scale.ShowMMinorTick,
		(Uint1)dmp->GrData.udv_scale.ScalePosition,
		from_col+start_ruler,
		to_col+start_ruler,
		&rcD,
		(Int2)(dmp->GrData.udv_font.ColWidth/2)+
		(Int2)((from_col-dmp->GrData.udv_hscrl.ScrollPos)*
		dmp->GrData.udv_font.ColWidth),
		dmp->MSA_d.pgp_l.LengthAli+start_ruler-1,
		from_col+start_ruler,
		FALSE,
		dmp->GrData.udv_font.ColWidth,(Uint4)DDV_DISP_HORZ);

	/*go at the bottom of the scale*/
	ResetClip();

	rcP.top+=3*dmp->GrData.udv_panel.cyScale/2;
	rcD=rcP;
	rcD.left=rcI.left;
	
	/*compute the region to draw (invalidate rect only)*/
	from_col=(MyUpdateRect->left-rcP.left)/dmp->GrData.udv_font.ColWidth-2;
	to_col=(MyUpdateRect->right-rcP.left)/dmp->GrData.udv_font.ColWidth+2;
	if (from_col<0 && to_col<0) bNotDrawCol=TRUE;
	else bNotDrawCol=FALSE;
	if (from_col<0) from_col=0;
	if (to_col<0) to_col=0;
	from_col+=dmp->GrData.udv_hscrl.ScrollPos;
	to_col+=dmp->GrData.udv_hscrl.ScrollPos;
	if (to_col>dmp->MSA_d.pgp_l.LengthAli) to_col=dmp->MSA_d.pgp_l.LengthAli;
	if (from_col>dmp->MSA_d.pgp_l.LengthAli) from_col=dmp->MSA_d.pgp_l.LengthAli;
	from_row=(MyUpdateRect->top-rcP.top)/dmp->GrData.udv_font.LineHeight-1;
	to_row=(MyUpdateRect->bottom-rcP.top)/dmp->GrData.udv_font.LineHeight+1;
	if (from_row<0 && to_row<0) return;		
	if (from_row<0) from_row=0;
	if (to_row<0) to_row=0;
	from_row+=dmp->GrData.udv_vscrl.ScrollPos;
	to_row+=dmp->GrData.udv_vscrl.ScrollPos;
	if (from_row>dmp->GrData.udv_panel.nTotLines) from_row=dmp->GrData.udv_panel.nTotLines;
	if (to_row>dmp->GrData.udv_panel.nTotLines) to_row=dmp->GrData.udv_panel.nTotLines;
	nTotRow=to_row-from_row;

	/*find the first ParaG to draw*/
	for(i=0;i<dmp->MSA_d.pgp_l.nBsp;i++){
		if (!(pgp=(ParaGPtr)(dmp->MSA_d.pgp_l.TableHead[i]->data.ptrvalue))) continue;
		if ((pgp->StartLine<=to_row)&&((pgp->StartLine+pgp->nLines)>=from_row)){
			start_i=i;break;
		}
	}

	ClipRect(&rcD);

  /* get the row of the master */
  if (dmp->dsp == NULL) {
    curMasterRow = 0;
  }
  else {
    curMasterRow = DDE_GetIndexOfMaster(dmp->dsp);
  }

	/*draw the names*/
  nLinesDraw=0;
	for(i=start_i;i<dmp->MSA_d.pgp_l.nBsp;i++){
		if (!(pgp=(ParaGPtr)(dmp->MSA_d.pgp_l.TableHead[i]->data.ptrvalue))) continue;
		if ((pgp->StartLine<=to_row)&&(pgp->StartLine>=from_row)){
			nLinesDraw+=pgp->nLines;
			DDV_DrawSequenceName(&dmp->GrData,pgp,
				(Int2)(pgp->StartLine*dmp->GrData.udv_font.LineHeight-
				top_offset+rcP.top),
				(Int2)(dmp->GrData.udv_panel.cxName-3),
				i, dmp->deri.curEditRow, curMasterRow);
		}
		else nLinesDraw+=pgp->nLines;
		if (nLinesDraw>to_row) break;
	}
	ResetClip();

	if (bNotDrawCol) return;

	ClipRect(&rcP);
	nLinesDraw=0;
	/*get the selection(s)*/
	
	ssp=ObjMgrGetSelected();
	
	/*draw ParaG*/
	for(i=start_i;i<dmp->MSA_d.pgp_l.nBsp;i++){/*draw line by line...*/
		if (!(pgp=(ParaGPtr)(dmp->MSA_d.pgp_l.TableHead[i]->data.ptrvalue))) continue;
		nColsDraw=0;
		if (vnp_bsp){
			vnp_bsp=ValNodeFree(vnp_bsp);
		}
		/*for each line, get the selected region(s)*/
		if (ssp!=NULL){
			vnp_pgp=dmp->MSA_d.pgp_l.TableHead[i];
			first_pgp=(ParaGPtr)(vnp_pgp->data.ptrvalue);
			if (first_pgp){
				bsp=BioseqLockById(first_pgp->sip);
				if (bsp){
					bsp_eID=ObjMgrGetEntityIDForPointer((Pointer)bsp);
					bsp_iID = GetItemIDGivenPointer (bsp_eID, 
							OBJ_BIOSEQ, (Pointer) bsp);	
					if (bsp_eID!=0 && bsp_iID!=0){
						vnp_bsp=DDV_GetSelectedRegions(ssp,bsp_eID,bsp_iID);
					}
					BioseqUnlock(bsp);
				}
			}
		}
		/*...col. by col.*/
		for(vnp=dmp->MSA_d.pgp_l.TableHead[i];vnp!=NULL;vnp=vnp->next){
			if (!(pgp=(ParaGPtr)(vnp->data.ptrvalue))) continue;
			if ((pgp->StartLetter>=from_col && pgp->StartLetter<=to_col)||
					(pgp->StopLetter>=from_col && pgp->StopLetter<=to_col)||
					(pgp->StartLetter<=from_col && pgp->StopLetter>=to_col)){
				
				from_pgp=pgp->StartLetter;
				to_pgp=pgp->StopLetter;
				
				length_pgp=to_pgp-from_pgp+1;/*+1 to surround the last letter*/
				rc_pgp.top=(Int2)(pgp->StartLine*dmp->GrData.udv_font.LineHeight-
						top_offset+rcP.top);
				rc_pgp.bottom=(Int2)(rc_pgp.top+pgp->nLines*
						dmp->GrData.udv_font.LineHeight);
				rc_pgp.left=(Int2)(from_pgp*dmp->GrData.udv_font.ColWidth-
						left_offset+rcP.left);
				rc_pgp.right=(Int2)(rc_pgp.left+length_pgp*
						dmp->GrData.udv_font.ColWidth);
				/*highlight a goto position ?*/
				if (dmp->dtd.row==i+1 && dmp->dtd.col!=0){
					dmp->GrData.GotoLetter=dmp->dtd.col-1;
				}
				else{
					dmp->GrData.GotoLetter=(Int4)-1;
				}
				DDV_DrawParaG(&rc_pgp,pgp,from_col,to_col,&dmp->GrData,
					dmp->Globals.colorp,&(dmp->ddo),vnp_bsp,i,bSelect);
			}
			else{
				nColsDraw+=(pgp->StopLetter-pgp->StartLetter);
			}
			if(nColsDraw>to_col)break;
		}
		nLinesDraw+=pgp->nLines;
		if (nLinesDraw>nTotRow) break;
	}
	/*display the caret; edit mode only*/
	if (dmp->MouseMode==DDV_MOUSEMODE_EDIT){
		x=(Int2)(dmp->dci.new_col*dmp->GrData.udv_font.ColWidth-
						left_offset+rcP.left);
		y=(Int2)(dmp->dci.new_row*dmp->GrData.udv_font.LineHeight-
						top_offset+rcP.top)-1;
		Red();
		MoveTo(x+dmp->GrData.udv_font.ColWidth,y);
		LineTo(x+dmp->GrData.udv_font.ColWidth,
			y-dmp->GrData.udv_font.LineHeight/2);
		MoveTo(x+dmp->GrData.udv_font.ColWidth-1,y);
		LineTo(x+dmp->GrData.udv_font.ColWidth-1,
			y-dmp->GrData.udv_font.LineHeight/2);
		MoveTo(x+dmp->GrData.udv_font.ColWidth/2,y+1);
		LineTo(x+3*dmp->GrData.udv_font.ColWidth/2,y+1);
		MoveTo(x+dmp->GrData.udv_font.ColWidth/2,y+2);
		LineTo(x+3*dmp->GrData.udv_font.ColWidth/2,y+2);
		Black();
	}
	ResetClip();

  if (dmp->bEditor) {
    /* this is for allowing edits of the alignment boundaries */
    DDV_DrawAlignmentBoundaries(p, dmp);
  }

	return;
}

static void DDV_DrawAlignmentBoundaries(PaneL p, DdvMainPtr dmp) {
/*******************************************************************************
  draw vertical bars at the top of the panel showing the alignment
  boundaries, and horizontal lines connecting them.
*******************************************************************************/
  Int4  LeftHPos, RightHPos;
  Int4  TopVPos, BotVPos;
  Int4  i, NumBlocks;
  RecT  rcP, rcBanner;
  DdvMainWinPtr  mWin_d;

  ObjectRect(p, &rcP);
  DDV_GetVPixelPosOfEmptySpace(dmp, rcP, &TopVPos, &BotVPos);
  rcBanner = rcP;
  rcBanner.left += dmp->GrData.udv_panel.cxName + dmp->GrData.udv_scale.cxLeftScale;
  rcBanner.top =    TopVPos;
  rcBanner.bottom = BotVPos;
  rcBanner.left -= 4;
  ClipRect(&rcBanner);
  EraseRect(&rcBanner);

  /* for each aligned block, draw a semi-rectangle on top of it */
  NumBlocks = DDE_GetNumBlocks(dmp->dsp->pEdit);
  for (i=0; i<NumBlocks; i++) {
    LeftHPos = DDV_GetHPixelPosGivenColNumber(dmp, rcP, DDE_GetAlignStart(dmp->dsp->pEdit, i));
    LeftHPos -= dmp->GrData.udv_font.ColWidth;
    RightHPos = DDV_GetHPixelPosGivenColNumber(dmp, rcP, DDE_GetAlignStop(dmp->dsp->pEdit, i));
    UDV_draw_vertical_bar(rcBanner, LeftHPos, FALSE);
    UDV_draw_vertical_bar(rcBanner, RightHPos, FALSE);
    UDV_draw_horizontal_bar(TopVPos, LeftHPos, RightHPos);
  }

  ResetClip();

  /* grey out either "create block" or "delete block" option */
  if (dmp->hParent){
	  mWin_d = (DdvMainWinPtr) GetObjectExtra(dmp->hParent);
    if (NumBlocks == 0) {
      Disable(mWin_d->MainMenu.DeleteBlock);
       Enable(mWin_d->MainMenu.CreateBlock);
    }
    else {
       Enable(mWin_d->MainMenu.DeleteBlock);
      Disable(mWin_d->MainMenu.CreateBlock);
    }
  }

}


extern void DDV_GreyOut(DdvMainWinPtr mWin_d, Boolean Start, Boolean End) {
/*******************************************************************************
  grey out "undo" if Start is TRUE.
  grey out "redo" if End is TRUE.
*******************************************************************************/
  if (Start) { Disable(mWin_d->MainMenu.Prev); }
  else       {  Enable(mWin_d->MainMenu.Prev); }
  if (End)   { Disable(mWin_d->MainMenu.Next); }
  else       {  Enable(mWin_d->MainMenu.Next); }
}


/*******************************************************************************

  Function : DDV_DrawPanelContent_V()
  
  Purpose :  draw the panel of DDV viewer (case 2: full vert display)
  
  Parameters :	p;handle of the panel
  				dmp; main data block
    
  Return value : none

*******************************************************************************/
static void  DDV_DrawPanelContent_V (PaneL p,DdvMainPtr dmp)
{/*not yet implemented*/
	return;
}

/*******************************************************************************

  Function : DDV_DrawPanelContent()
  
  Purpose :  draw the panel of DDV viewer
  
  Parameters :	p;handle of the panel
  				dmp; main data block
    
  Return value : none

*******************************************************************************/
extern void  DDV_DrawPanelContent (PaneL p,DdvMainPtr dmp)
{
	if (dmp->MSA_d.pgp_l.DisplayType==DDV_DISP_HORZ) 
		DDV_DrawPanelContent_H (p,dmp,&updateRect,TRUE);
	else 
		DDV_DrawPanelContent_V (p,dmp);
	return;
}

/*******************************************************************************

  Function : DDV_DrawViewer()
  
  Purpose :  callback for the viewer panel
  
  Parameters :	handle of the panel
    
  Return value : none

*******************************************************************************/
extern void  DDV_DrawViewer (PaneL p)
{

DdvMainPtr 	dmp;

	/*get the application data*/
	dmp = (DdvMainPtr) GetObjectExtra(p);
	if (dmp==NULL) return;

	if (dmp->MSA_d.pgp_l.DisplayType==DDV_DISP_HORZ) 
		DDV_DrawPanelContent_H (p,dmp,&updateRect,TRUE);
	else 
		DDV_DrawPanelContent_V (p,dmp);
}
