/*   udvgraph.c
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
* File Name:  udvgraph.c
*
* Author:  Patrick Durand
*
* Version Creation Date:   5/3/99
*
* $Revision: 6.49 $
*
* File Description: mouse management, graphic engine of the sequence viewer
*                   part of this code is also used for the WWW Entrez viewer
*                   (WWW_UDV define)
* Modifications:
* --------------------------------------------------------------------------
* $Log: udvgraph.c,v $
* Revision 6.49  2002/08/23 18:47:52  kans
* protect against dereferencing NULL prot
*
* Revision 6.48  2000/06/27 20:46:38  hurwitz
* fixed bugs with select rectangle, added select row option
*
* Revision 6.47  2000/04/13 13:58:02  durand
* allowed udv to display reverse complement sequence
*
* Revision 6.46  2000/04/11 11:52:10  durand
* added code to load editor after a double-click
*
* Revision 6.45  2000/04/05 20:52:35  hurwitz
* added GUI control for shifting left and right alignment boundaries
*
* Revision 6.44  2000/04/05 12:02:55  durand
* minor changes in the drawing of graphic elements
*
* Revision 6.43  2000/04/03 22:26:31  hurwitz
* can now shift a row with click and drag
*
* Revision 6.42  2000/03/28 21:03:14  hurwitz
* added gui control to re-order rows
*
* Revision 6.41  2000/03/06 14:56:38  durand
* fixed problems with mouse selection between udv and cn3d
*
* Revision 6.40  2000/03/06 14:00:48  durand
* first release of the Summary viewer done
*
* Revision 6.39  2000/03/01 20:22:57  durand
* update deselect/select code to allow UDV sending correct message to Cn3D
*
* Revision 6.38  2000/02/28 19:17:24  lewisg
* eliminate mouseup message for feature selection
*
* Revision 6.37  2000/02/16 22:38:30  durand
* fixed some wierd behaviours of features selections
*
* Revision 6.36  2000/02/15 22:40:58  lewisg
* add ability to launch udv so that it colors by row, fixes to colormgr, track rows from viewmgr, fix visual c projects
*
* Revision 6.35  2000/02/11 15:40:28  durand
* replaced ObjMgrSendMsg by ObjMgrSelect calls for features selection
*
* Revision 6.34  2000/02/08 20:41:19  durand
* create the image map for a sequence without feature
*
* Revision 6.33  2000/01/08 00:47:54  lewisg
* fixes to selection, update, color
*
* Revision 6.32  2000/01/05 21:06:36  durand
* update mouse click actions and DrawSequence function for a better use from ddv and cn3d
*
* Revision 6.31  1999/12/29 22:55:03  lewisg
* get rid of seqalign id
*
* Revision 6.30  1999/12/15 23:17:47  lewisg
* make cn3d launch udv, fix launching of non-autonomous udv, add mouseup message
*
* Revision 6.29  1999/12/02 13:48:16  durand
* fix a bug for Entrez sequence viewer
*
* Revision 6.28  1999/11/29 15:17:54  durand
* designed a new GUI; fixed problems under Win95 and Linux
*
* Revision 6.27  1999/11/19 15:01:48  durand
* speed up mouse selection ; avoid sequence flashing during selection ; update menu functionalities
*
* Revision 6.26  1999/11/18 14:54:22  durand
* add a new function to create an image map given a ParaG structure - Entrez sequence viewer only
*
* Revision 6.25  1999/11/09 23:26:25  kans
* include jzmisc.h for swap prototype
*
* Revision 6.24  1999/11/09 21:06:58  durand
* add sequence selection manager
*
* Revision 6.23  1999/11/05 14:54:27  durand
* update logo position
*
* Revision 6.22  1999/11/02 13:38:18  durand
* update selection code for Entrez sequence viewer
*
* Revision 6.21  1999/10/14 16:14:25  kans
* added include for asn2ff6.h for GetProductSeqId and GetGINumFromSip
*
* Revision 6.20  1999/10/14 13:34:33  durand
* use BioseqFind to get product (CDS display)
*
* Revision 6.19  1999/10/13 17:08:17  durand
* use smaller font for WWW release of the viewer
*
* Revision 6.18  1999/10/02 15:11:15  durand
* update the code to be used by wwwudv
*
* Revision 6.17  1999/09/29 20:09:51  durand
* modify some parts of the code to be used by the cgi-bin release of UDV
*
* Revision 6.16  1999/09/27 14:10:31  durand
* switch to ddvcolor to allow UDV working from Cn3D (LG and PD)
*
* Revision 6.15  1999/09/20 21:57:41  durand
* switch UDV_draw_double_cursor from static to NLM_EXTERN
*
* Revision 6.14  1999/09/13 20:37:17  durand
* update UDV_Draw_scale for DDV
*
* Revision 6.13  1999/09/07 23:03:10  durand
* update UDV_Draw_scale to deal with discontinuous SEqAlign
*
* Revision 6.12  1999/07/30 20:08:57  durand
* updates for the new Entrez graphical viewer
*
* Revision 6.11  1999/07/19 20:35:35  durand
* switch ScalePositionfrom from Boolean to Uint1 in UDV_Draw_scale
*
* Revision 6.10  1999/06/29 14:59:52  durand
* update udv_draw_scale for DDV
*
* Revision 6.9  1999/06/16 13:07:00  durand
* update UDV functions to be used by DDV
*
* Revision 6.8  1999/06/08 13:52:35  durand
* update UDV data structures for the MSA editor
*
* Revision 6.7  1999/06/07 15:39:43  durand
* add LOG line to keep track of the history
*
*
*
* ==========================================================================
*/

#include <udviewer.h>
#include <udvdef.h>
#include <ddvcolor.h>
#include <asn2ff6.h>
#include <jzmisc.h>
#include <viewmgr.h>

static RecT  UDV_calc_RCdraw(Int4 StartLine,Int4 nLines, RecT rcP,
		Int2 decal_gauche,Int4 decal_haut,Int2 LineHeight);
#ifndef WWW_UDV
static void UDV_draw_panel(PaneL p,ViewerDialogDataPtr vdp,RecT PNTR MyUpdateRect,
	Boolean bSelect);	
#endif /*WWW_UDV*/

/*****************************************************************************

Function: LogoFontCreate()

Purpose: create a simple set of fonts for the logo 

Return value: none

*****************************************************************************/
#ifndef WWW_UDV
NLM_EXTERN void LogoFontCreate(FonT PNTR f1,FonT PNTR f2,FonT PNTR f3)
{

#ifdef WIN_MAC
		  *f1 = ParseFont ("Geneva,10");
		  *f2 = ParseFont ("Times,14,b");
		  *f3 = ParseFont ("Times,14,b,i");
#endif

#ifdef WIN_MSWIN
		  *f1 = ParseFont ("Arial,16");
		  *f2 = ParseFont ("Times New Roman,20,b");
		  *f3 = ParseFont ("Times New Roman,20,b,i");
#endif

#ifdef WIN_MOTIF
		  *f1 = ParseFont ("Helvetica,20,b");
		  *f2 = ParseFont ("Times,24,b");
		  *f3 = ParseFont ("Times,24,b,i");
#endif

}
#endif /*WWW_UDV*/

/*****************************************************************************

Function: UDV_FontCreate()

Purpose: create a simple set of fonts for the viewer

Return value: handle to the font 

*****************************************************************************/
NLM_EXTERN FonT UDV_InitFont(UDVFontDataPtr udvfp)
{

#ifdef WIN_MAC
		udvfp->hFnt = ParseFont ("Monaco, 9");
#endif

#ifdef WIN_MSWIN
		udvfp->hFnt = ParseFont ("Courier, 7");
#endif

#ifdef WIN_MOTIF
		udvfp->hFnt = ParseFont ("fixed, 12");
#endif

#ifdef WWW_UDV
		udvfp->hFnt = ParseFont ("fixed, 10");
#endif

	return(udvfp->hFnt);
}

/*****************************************************************************

Function: UDV_FontDim()

Purpose: computes cxChar and cyChar  

Return value: none

*****************************************************************************/
NLM_EXTERN void  UDV_FontDim(Int2Ptr cxChar,Int2Ptr cyChar)
{
	*cxChar=MaxCharWidth();
	*cyChar=LineHeight();
}


/*****************************************************************************

Function: UDV_ComputeLineHeight()

Purpose: compute the height of a single data line  

Return value: the height

*****************************************************************************/
NLM_EXTERN Int2 UDV_ComputeLineHeight(Int2 cyChar)
{
	/*return(3*cyChar/2);*/
	return(cyChar);
}

/*****************************************************************************

Function: UDV_ComputePanelSize()

Purpose: computes cxClient and cyClient  
         rc is the UDV panel
		 
Return value: none

*****************************************************************************/
NLM_EXTERN void  UDV_ComputePanelSize(RecT rc,Int2Ptr cxClient,
			Int2Ptr cyClient)
{
 
	InsetRect (&rc, (Int2)VIEWER_HORZ_MARGIN, (Int2)VIEWER_VERT_MARGIN);
	*cxClient=rc.right-rc.left+1;
	*cyClient=rc.bottom-rc.top+1;

}

/*****************************************************************************

Function: UDV_ComputeBlockByLine()

Purpose: computes the number of Ten-letter blocks as well as the number of 
		letters displayed on one line of the viewer  

Return value: none (results are in nCharByLine & nBlockByLine)

*****************************************************************************/
NLM_EXTERN void  UDV_ComputeBlockByLine(Int2 cxClient,Int2 cxName,
			Int2 cxLeftScale,Int2 cxChar,Int2Ptr nCharByLine,
			Int2Ptr nBlockByLine)
{
Int2 cx, cxWin,i,LB;

	LB=LETTER_BLOCK_WIDTH+1;/*+1: inter-block space*/
	cxWin=cxClient-cxName-cxLeftScale-2*VIEWER_HORZ_MARGIN;
	i=1;
	while(TRUE){/*count number of blocks within panel*/
		cx=i*LB*cxChar;
		if (cx>cxWin){
			i--;
			break;
		}
		i++;
	}
	
	*nCharByLine=i*LETTER_BLOCK_WIDTH;
	*nBlockByLine=i;
}

/*****************************************************************************

Function: UDV_Init_GraphData()

Purpose: Init Graphical values for the UnDViewer. Called one times at the
		start of the soft.

Return value: none (results are in GrDataPtr)

*****************************************************************************/
NLM_EXTERN void  UDV_Init_ScaleData(UnDViewerGraphDataPtr GrDataPtr)
{
	/*display options*/
	GrDataPtr->udv_panel.ShowFeatures=TRUE;
	GrDataPtr->udv_panel.ShowScale=TRUE;
	GrDataPtr->DisplayOptions=DDV_DISP_VERT;

	/*scale option*/
	GrDataPtr->udv_scale.ScalePosition=(Int2)SCALE_POS_BOTH;
	GrDataPtr->udv_scale.ShowMajorTick=TRUE;
	GrDataPtr->udv_scale.ShowMMinorTick=TRUE;
	GrDataPtr->udv_scale.MajorTickEvery=(Int2)SCALE_MAJOR_TICK_EVERY;
	GrDataPtr->udv_scale.MinorTickEvery=(Int2)SCALE_MINOR_TICK_EVERY;
/*	GrDataPtr->udv_scale.ScaleColor=(Uint4)SCALE_LETTER_COLOR;
	GrDataPtr->udv_scale.TickMajColor=(Uint4)SCALE_MAJTICK_COLOR;
	GrDataPtr->udv_scale.TickMinColor=(Uint4)SCALE_MINTICK_COLOR;*/
	GrDataPtr->udv_scale.cxLeftScale=(Int2)SCALE_WIDTH_IF_LEFT*
					GrDataPtr->udv_font.cxChar;


}

/*****************************************************************************

Function: UDV_Init_GraphData()

Purpose: Init Graphical values for the UnDViewer. Called one times at the
		start of the soft.

Return value: none (results are in GrDataPtr)

*****************************************************************************/
#ifndef WWW_UDV
NLM_EXTERN void  UDV_Init_GraphData(PaneL p,UnDViewerGraphDataPtr GrDataPtr)
{
RecT rc;	
	
	UDV_Init_ScaleData(GrDataPtr);
	/*Panel Size & Lines to display*/
	if (p!=NULL){/*Vibrant viewer*/
		ObjectRect(p,&rc);
		UDV_ComputePanelSize(rc,&GrDataPtr->udv_panel.cxClient,
			&GrDataPtr->udv_panel.cyClient);
		GrDataPtr->udv_panel.cxName=PANEL_NAME_WIDTH*GrDataPtr->udv_font.cxChar;
		UDV_ComputeBlockByLine(GrDataPtr->udv_panel.cxClient,
			GrDataPtr->udv_panel.cxName,
			GrDataPtr->udv_scale.cxLeftScale,GrDataPtr->udv_font.cxChar,
			&GrDataPtr->udv_panel.nCharByLine,
			&GrDataPtr->udv_panel.nBlockByLine);
	}

}
#endif
/*****************************************************************************

Function: UDV_BuildFeatColorTable()

Purpose: build a simple colour table to draw Nuc/Prot features

Return value: a pointer to a colout table (each colour is encoded as an Uint4)

*****************************************************************************/
NLM_EXTERN Uint4Ptr  UDV_BuildFeatColorTable(void)
{
Uint4Ptr pClr;
Uint1 i;

	pClr=(Uint4Ptr)MemNew(FEATDEF_MAX*sizeof(Uint4));
	if (!pClr) return(NULL);
	for (i=0;i<FEATDEF_MAX;i++)	
		pClr[i]=GetColorRGB(0,0,0);

	/*Warning: values are one-based*/
	pClr[FEATDEF_GENE]=GetColorRGB(128,128,128);			/*gray*/
	
	pClr[FEATDEF_precursor_RNA]=GetColorRGB(0,0,255);/*blue*/
	pClr[FEATDEF_misc_RNA]=GetColorRGB(0,0,255);
	pClr[FEATDEF_preRNA]=GetColorRGB(0,0,255);
	pClr[FEATDEF_mRNA]=GetColorRGB(0,0,255);
	pClr[FEATDEF_tRNA]=GetColorRGB(0,0,255);
	pClr[FEATDEF_rRNA]=GetColorRGB(0,0,255);
	pClr[FEATDEF_snRNA]=GetColorRGB(0,0,255);
	pClr[FEATDEF_scRNA]=GetColorRGB(0,0,255);
	pClr[FEATDEF_otherRNA]=GetColorRGB(0,0,255);
	pClr[FEATDEF_prim_transcript]=GetColorRGB(0,0,255);

	pClr[FEATDEF_CDS]=GetColorRGB(255,150,255);	/*pink*/
	pClr[FEATDEF_exon]=GetColorRGB(255,150,255);
	pClr[FEATDEF_intron]=GetColorRGB(255,150,255);

	pClr[FEATDEF_PROT]=GetColorRGB(0,128,0);		/*dk green*/
	pClr[FEATDEF_mat_peptide]=GetColorRGB(0,128,0);
	pClr[FEATDEF_sig_peptide]=GetColorRGB(0,128,0);
	pClr[FEATDEF_transit_peptide]=GetColorRGB(0,128,0);
	pClr[FEATDEF_preprotein]=GetColorRGB(0,128,0);
	pClr[FEATDEF_mat_peptide_aa]=GetColorRGB(0,128,0);
	pClr[FEATDEF_sig_peptide_aa]=GetColorRGB(0,128,0);
	pClr[FEATDEF_transit_peptide_aa]=GetColorRGB(0,128,0);


	pClr[FEATDEF_SITE]=GetColorRGB(255,0,0);		/*red*/

	pClr[FEATDEF_REGION]=GetColorRGB(210,154,14);		/*orange*/
	pClr[FEATDEF_mutation]=GetColorRGB(210,154,14);
	pClr[FEATDEF_variation]=GetColorRGB(210,154,14);

	pClr[FEATDEF_PSEC_STR]=GetColorRGB(104,201,220); /*cyan*/

	pClr[FEATDEF_HET]=GetColorRGB(128,128,0);	/*yellow*/

	pClr[FEATDEF_BOND]=GetColorRGB(255,92,255);	/*pink*/

	return(pClr);
}

/*****************************************************************************

Function: UDV_Build_Other_Colors()

Purpose: create the colors for font, rulers,...

Return value: none (results stored in GrData)

*****************************************************************************/
NLM_EXTERN void UDV_Build_Other_Colors(UnDViewerGraphDataPtr GrDataPtr)
{
	/*font*/
	GrDataPtr->udv_font.UseDefaultColorLetter=TRUE;
	GrDataPtr->udv_font.LetterColor=(Uint4)FONT_DEF_COLOR;
	/*ruler*/
	GrDataPtr->udv_scale.ScaleColor=(Uint4)SCALE_LETTER_COLOR;
	GrDataPtr->udv_scale.TickMajColor=(Uint4)SCALE_MAJTICK_COLOR;
	GrDataPtr->udv_scale.TickMinColor=(Uint4)SCALE_MINTICK_COLOR;
}

/*****************************************************************************

Function: UDV_Build_NA_LayoutPalette()

Purpose: build a simple layout table to draw colour Nuc sequence

Return value: none (results store in GrData)

*****************************************************************************/
NLM_EXTERN void  UDV_Build_NA_LayoutPalette(UnDViewerGraphDataPtr GrData)
{

	GrData->NA_LayoutPal[NA_A_LAYOUT].LetClr=GetColorRGB(0,192,0);
	GrData->NA_LayoutPal[NA_A_LAYOUT].bkClr=0;
	GrData->NA_LayoutPal[NA_A_LAYOUT].AspLet=LAYOUT_LOWER_CASE;

	GrData->NA_LayoutPal[NA_G_LAYOUT].LetClr=GetColorRGB(192,192,0);
	GrData->NA_LayoutPal[NA_G_LAYOUT].bkClr=0;
	GrData->NA_LayoutPal[NA_G_LAYOUT].AspLet=LAYOUT_LOWER_CASE;

	GrData->NA_LayoutPal[NA_C_LAYOUT].LetClr=GetColorRGB(0,0,255);
	GrData->NA_LayoutPal[NA_C_LAYOUT].bkClr=0;
	GrData->NA_LayoutPal[NA_C_LAYOUT].AspLet=LAYOUT_LOWER_CASE;

	GrData->NA_LayoutPal[NA_T_LAYOUT].LetClr=GetColorRGB(255,0,0);
	GrData->NA_LayoutPal[NA_T_LAYOUT].bkClr=0;
	GrData->NA_LayoutPal[NA_T_LAYOUT].AspLet=LAYOUT_LOWER_CASE;

	GrData->NA_LayoutPal[NA_U_LAYOUT].LetClr=GetColorRGB(192,0,192);
	GrData->NA_LayoutPal[NA_U_LAYOUT].bkClr=0;
	GrData->NA_LayoutPal[NA_U_LAYOUT].AspLet=LAYOUT_LOWER_CASE;
}

/*****************************************************************************

Function: UDV_Build_AA_LayoutPalette()

Purpose: build a simple layout table to draw colour Prot sequence

Return value: none (results store in GrData)

*****************************************************************************/
NLM_EXTERN void  UDV_Build_AA_LayoutPalette(UnDViewerGraphDataPtr GrData)
{
RGBColoR LetClr[26]={{0,0,0},	/* A */
					{0,0,0},	/* B */
					{0,0,0},	/* C */
					{255,0,0},	/* D */
					{255,0,0},	/* E */
					{0,192,0},	/* F */
					{0,0,0},	/* G */
					{0,0,0},	/* H */
					{0,192,0},	/* I */
					{0,0,0},	/* J */
					{0,0,255},	/* K */
					{0,192,0},	/* L */
					{0,192,0},	/* M */
					{255,0,0},	/* N */
					{0,0,0},	/* O */
					{255,0,0},	/* P */
					{255,0,0},	/* Q */
					{0,0,255},	/* R */
					{0,0,0},	/* S */
					{0,0,0},	/* T */
					{0,0,0},	/* U */
					{0,192,0},	/* V */
					{0,192,0},	/* W */
					{0,0,0},	/* X */
					{0,192,0},	/* Y */
					{0,0,0},	/* Z */
					};

RGBColoR BkClr[26]={{255,255,255},	/* A */
					{255,255,255},	/* B */
					{255,255,255},	/* C */
					{255,255,255},	/* D */
					{255,255,255},	/* E */
					{255,255,255},	/* F */
					{255,255,255},	/* G */
					{255,255,255},	/* H */
					{255,255,255},	/* I */
					{255,255,255},	/* J */
					{255,255,255},	/* K */
					{255,255,255},	/* L */
					{255,255,255},	/* M */
					{255,255,255},	/* N */
					{255,255,255},	/* O */
					{255,255,255},	/* P */
					{255,255,255},	/* Q */
					{255,255,255},	/* R */
					{255,255,255},	/* S */
					{255,255,255},	/* T */
					{255,255,255},	/* U */
					{255,255,255},	/* V */
					{255,255,255},	/* W */
					{255,255,255},	/* X */
					{255,255,255},	/* Y */
					{255,255,255},	/* Z */
					};
Uint1 i;

	for(i=0;i<26;i++){
		GrData->AA_LayoutPal[i].LetClr=GetColorRGB(LetClr[i].red,
													LetClr[i].green,
													LetClr[i].blue);
		GrData->AA_LayoutPal[i].bkClr=GetColorRGB(BkClr[i].red,
													BkClr[i].green,
													BkClr[i].blue);
		GrData->AA_LayoutPal[i].AspLet=LAYOUT_UPPER_CASE;
	}	
}

/*******************************************************************************

  Function : UDV_GetCurrentDispRange()
  
  Purpose : get first,last position of the sequence on the screen
  
  Return value : -

*******************************************************************************/
#ifndef WWW_UDV
NLM_EXTERN void	UDV_GetCurrentDispRange(ViewerDialogDataPtr vdp,
		Int4Ptr from_bsp,Int4Ptr to_bsp,Int4Ptr from_line,Int4Ptr to_line)
{
ValNodePtr vnp,vnp2;
ParaGPtr   pgp;
Int4       from_row,to_row;

	*from_bsp=0;
	*to_bsp=0;
	if (vdp==NULL) return;
	
	/*rows displayed in the screen*/
	from_row=vdp->udv_graph.udv_vscrl.ScrollPos;
	if (vdp->udv_graph.udv_vscrl.ScrollPage==0){
		to_row=vdp->udv_graph.udv_panel.nTotLines;
	}
	else{
		to_row=from_row+vdp->udv_graph.udv_vscrl.ScrollPage;
		if (to_row>vdp->udv_graph.udv_panel.nTotLines) 
			to_row=vdp->udv_graph.udv_panel.nTotLines;
	}
	/*find the ParaG where from_row is Located*/
	vnp=vdp->ParaG;
	while(vnp){
		if (vnp->data.ptrvalue){
			pgp=(ParaGPtr)vnp->data.ptrvalue;
			if ((pgp->StartLine<=from_row)&&((pgp->StartLine+pgp->nLines)>=from_row)){
					break;
			}
		}
		vnp=vnp->next;
	}
	*from_bsp=pgp->StartLetter;
	/*find the ParaG where to_row is Located*/
	vnp2=vnp;
	while(vnp2){
		if (vnp2->data.ptrvalue){
			pgp=(ParaGPtr)vnp2->data.ptrvalue;
			if ((pgp->StartLine<=to_row)&&((pgp->StartLine+pgp->nLines)>=to_row)){
					break;
			}
		}
		vnp2=vnp2->next;
	}	
	*to_bsp=pgp->StopLetter;
	
	if (from_line) *from_line=from_row;
	if (to_line) *to_line=to_row;
}
#endif

/*******************************************************************************

  Function : UDV_InvalRegion()
  
  Purpose : invalidate a specific region of a sequence within a ParaG
  
  Return value : -

*******************************************************************************/
#ifndef WWW_UDV
NLM_EXTERN void UDV_InvalRegion(PaneL UnDViewer,UnDViewerGraphDataPtr GrData,
		ParaGPtr pgp,Int4 start_inval,Int4 stop_inval,Boolean IsSelect)
{
Int4 				decal_haut,StartLine;
ValNodePtr 			vnp,vnp2;
RecT 				rc,rcP;
Int2				decal_gauche,left_pos,right_pos;
ViewerDialogDataPtr vdp;
WindoW              w,temport;
ViewerMainPtr       vmp;

	/*get some usefull data...*/
	vdp = (ViewerDialogDataPtr) GetObjectExtra (UnDViewer);

	if (vdp == NULL) return;

    temport=SavePort(ParentWindow(UnDViewer));
	Select(UnDViewer);
	ObjectRect(UnDViewer,&rcP);
	
	StartLine=pgp->StartLine;
	if (GrData->udv_scale.ShowMajorTick) StartLine++;
	if (GrData->udv_scale.ScalePosition==SCALE_POS_TOP || 
			GrData->udv_scale.ScalePosition==SCALE_POS_BOTH) StartLine++;

	decal_haut=GrData->udv_vscrl.ScrollPos*
			GrData->udv_font.LineHeight-VIEWER_VERT_MARGIN;	
	decal_gauche=GrData->udv_panel.cxName+rcP.left+
			GrData->udv_scale.cxLeftScale;
	
	left_pos=(Int2)(start_inval-pgp->StartLetter);
	left_pos+=(left_pos/LETTER_BLOCK_WIDTH);
	right_pos=(Int2)(stop_inval-pgp->StartLetter);
	right_pos+=(right_pos/LETTER_BLOCK_WIDTH);
	rc.top=(Int2)(StartLine*GrData->udv_font.LineHeight+rcP.top-decal_haut);
	rc.bottom=(Int2)(rc.top+GrData->udv_font.LineHeight)+GrData->udv_font.LineHeight/3;
	rc.left=decal_gauche+left_pos*GrData->udv_font.cxChar;
	rc.right=decal_gauche+right_pos*GrData->udv_font.cxChar+2*GrData->udv_font.cxChar;
	if (!IsSelect){
		InvalRect(&rc);
	}
	else{
		UDV_draw_panel(UnDViewer,vdp,&rc,IsSelect);
	}
    RestorePort(temport);
}
#endif


/*****************************************************************************

Function: UDV_calc_pos()

Purpose: get the bsp_coord given some data (current ParaG,...)

Parameters:	GrData; graphical data object
			pt; mouse position
			rcClip; region where pgp is included
			pgp; ParaG data
			szPos; used to put pos as a string
			
Return value: pos (1-based value)

*****************************************************************************/
static Int4  UDV_calc_pos(UnDViewerGraphDataPtr GrData,PoinT * pt,
		RecT * rcClip,ParaGPtr pgp,CharPtr szPos)
{
Int2 xMargin,i,max;
Int4 pos,start,stop;


	xMargin=rcClip->left+GrData->udv_font.cxChar;

	pos=(Int4)((pt->x-xMargin+GrData->udv_font.cxChar/2)/
		GrData->udv_font.cxChar);
	max=GrData->udv_panel.nBlockByLine+1;
	for (i=0;i<max;i++){
		start=i*LETTER_BLOCK_WIDTH+i;
		stop=start+LETTER_BLOCK_WIDTH;
		if (pos>=start && pos<stop) {pos=pos-i; break;}
		if (pos==stop) {pos=pos-i-1; break;}
	}
	pos+=pgp->StartLetter;
	if (pos<pgp->StartLetter) pos=pgp->StartLetter;
	if (pos>pgp->StopLetter) pos=pgp->StopLetter;
	pos++;
	
	if (szPos!=NULL) sprintf(szPos,"%d",pos);

	return(pos);
}

/*****************************************************************************

Function: UDV_find_item()

Purpose: find if a feature is selected (generally in reponse to ObjMgr)

Parameters: ParaG; list to check
			bsp; identifier of the sequence
			entityID, 
			itemID; identifiers of the feature
			szFeatureName; name of the feature, if found (return value)
			pgpFeat; ParaG where the feature is located
			nLineDecal; line number within ParaG where the feature is located
			isp; identifier of the feature (eID, iID, iType)
			
Return value: TRUE if feature has been found in the ParaG list

*****************************************************************************/
#ifndef WWW_UDV
static Boolean  UDV_find_item(ValNodePtr ParaG,BioseqPtr bsp,
		Uint2 entityID,Uint2 itemID,Uint2 itemType,CharPtr szFeatureName,
		ParaGPtr PNTR pgpFeat,Uint2Ptr nLineDecal,UDV_Item_SelectPtr isp)
{
ParaGPtr pgp;				/*data to check*/
Uint2 j;					/*little counter*/
ValNodePtr vnp,vnp2;		/*use to scan features*/
Uint2 iID,idx;				/*used to retrieve a desired Feature*/
SeqMgrFeatContext context;	/*used to retrieve feature data*/
Char szBuf[255]={""};		/*feature name */
Char szTmp[255]={""};		/*feature name */
Boolean bFound=FALSE;		/*TRUE if feature found*/
	
	if (ParaG){
		for(vnp=ParaG ; vnp!=NULL ; vnp=vnp->next){
			if (vnp->data.ptrvalue){
				pgp=(ParaGPtr)vnp->data.ptrvalue;
				/*look through features*/
				for(j=0,vnp2=pgp->pFeatList;j<pgp->nFeat;j++,vnp2=vnp2->next){
					if (vnp2 == NULL) break;

					UDV_BigDecodeIdxFeat ((Uint8)vnp2->data.bigintvalue, &iID,&idx,
							NULL,NULL);
					if (iID==itemID){
						if (!SeqMgrGetDesiredFeature(entityID,
							bsp,iID,idx,NULL,&context)) {
							break;
						}
						FeatDefLabel (context.sfp, szBuf, sizeof (szBuf) - 1, 
							OM_LABEL_BOTH);
						if (context.numivals>1){
							sprintf(szTmp,"%s [from %d to %d in %d segments]. ",
								szBuf,context.left+1,context.right+1,
								context.numivals);
						}
						else{
							sprintf(szTmp,"%s [from %d to %d]. ",
								szBuf,context.left+1,context.right+1);
						}
						StringCpy(szBuf,szTmp);
						sprintf(szTmp,"Length = %d. ",
							context.right-context.left+1);
						StringCat(szBuf,szTmp);

						if (context.sfp->comment) {
							sprintf(szTmp,"Note: %s. ",context.sfp->comment);
							StringCat(szBuf,szTmp);
						}
						/*store szBuf in szFeatureName*/
						if (szFeatureName) 
							StringCpy(szFeatureName,szBuf);
						*pgpFeat=pgp;
						*nLineDecal+=j;
						isp->eIDsel=entityID;
						isp->iIDsel=itemID;
						isp->iTypeSel=itemType;
						bFound=TRUE;
						break;
					}
				}
			}
			if (bFound) break;
		}		
	}
	return(bFound);
}
#endif /*WWW_UDV*/

/*****************************************************************************

Function: UDV_click_item()

Purpose: find whether the user has clicked on a feature or not (retrieve
	values which will be sent to ObjMgr)

Parameters: GrData; graphical data object
			pt; mouse position
			rc; ParaG region
			pgp: ParaG data
			bsp_i; bsp data
			entityID, 
			itemID,
			itemType; identifiers of the features
			index; index as defined by the Feature Index for a specific feature

Return value: TRUE if  the user has clicked within a feat

*****************************************************************************/
#ifndef WWW_UDV
static Boolean  UDV_click_item(UnDViewerGraphDataPtr GrData,PoinT pt,
		RecT rc,ParaGPtr pgp,BspInfo bsp_i,Uint2Ptr entityID,
		Uint2Ptr itemID,Uint2Ptr itemType,Uint2Ptr index)
{
Boolean InFeat=FALSE;		/*TRUE if the user has clicked within a feat*/
ValNodePtr vnp;				/*use to scan features*/
Int2 i,j,yDecal=0/*1*/;			/*use to find a feat region*/
Int4 pos,OccupyTo;			/*position (# of letters) of the mouse*/
Uint2 iID,idx;				/*used to retrieve a desired Feature*/
SeqMgrFeatContext context;	/*used to retrieve feature data*/
Boolean IsTransNeeded=FALSE;

	if (pgp->pFeatList==NULL) return(InFeat);

	/*go at the bottom of the sequence*/
	if (GrData->udv_scale.ScalePosition==SCALE_POS_TOP || 
					GrData->udv_scale.ScalePosition==SCALE_POS_BOTH) yDecal++;
	if (GrData->udv_scale.ShowMajorTick) yDecal++;
	
	yDecal=rc.top+yDecal*GrData->udv_font.LineHeight;
	OccupyTo=pgp->StopLetter;
	
	for(j=0,vnp=pgp->pFeatList;j<pgp->nFeat;j++,vnp=vnp->next){
		if (vnp == NULL) break;
		i=1;
		/*get desired feature given iID and idx*/
		UDV_BigDecodeIdxFeat ((Uint8)vnp->data.bigintvalue, &iID,&idx,NULL,NULL);
		if (!SeqMgrGetDesiredFeature(bsp_i.bsp_entityID,bsp_i.bsp,
            iID,idx,NULL,&context)) {
			break;
		}
		else{
			/*CDS occupies 1 or 2 lines*/
			if (context.sfp->data.choice==SEQFEAT_CDREGION){
				if (UDV_IsTranslationNeeded(&context,pgp)) i=2;
				else i=1;
			}
			else i=1;

			if (context.left<=OccupyTo)
				yDecal+=GrData->udv_font.LineHeight;

			if (pt.y>=yDecal && pt.y<=yDecal+i*GrData->udv_font.LineHeight){
				/*pos is a 1-based value*/
				if(pt.x<GrData->udv_panel.cxName-2)
					pos=context.left;
				else pos=UDV_calc_pos(GrData,&pt,&rc,pgp,NULL)-1;
				/*HET feature correction for the ends*/
				if(context.featdeftype== FEATDEF_HET){
					context.right=context.ivals[2*context.numivals-1];
				}

				if (pos>=context.left && pos<=context.right) {
					*entityID=bsp_i.bsp_entityID;
					*itemID=iID;
					*itemType=OBJ_SEQFEAT;
					*index=context.index;
					InFeat=TRUE;
					break;
				}
			}
			OccupyTo=context.right;
		}
	}
	return(InFeat);
}
#endif /*WWW_UDV*/


/*****************************************************************************

Function: UDV_draw_vert_cursor()

Purpose: draw a vertical cursor to track position of a feature

Parameters:	rcClip; draw only when the mouse is within rcClip
			pos; new position to draw

Return value: none

*****************************************************************************/
static void UDV_draw_vert_cursor(RecT rcClip,PoinT pos)
{
	if (PtInRect(pos,&rcClip)){
		Dotted();
		MoveTo(pos.x,rcClip.top);
		LineTo(pos.x,rcClip.bottom);
		Solid();
	}
}

/*****************************************************************************

Function: UDV_draw_horz_cursor()

Purpose: draw a horizontal cursor to track position of a feature

Parameters:	GrData; graphical data object
			rcClip; draw only when the mouse is within rcClip
			pos; new position to draw

Return value: none

*****************************************************************************/
static void 	UDV_draw_horz_cursor(UnDViewerGraphDataPtr GrData,
			RecT rcClip,PoinT pos)
{
	
	if (PtInRect(pos,&rcClip)){
		PoinT arrow[3];
		Dotted();
		MoveTo((Int2)(rcClip.left-GrData->udv_scale.cxLeftScale),pos.y);
		LineTo(pos.x,pos.y);
		Solid();
		arrow[0].x=rcClip.left-GrData->udv_scale.cxLeftScale;
		arrow[0].y=pos.y;
		arrow[1].x=arrow[0].x+GrData->udv_font.cxChar/2;
		arrow[1].y=pos.y-GrData->udv_font.cxChar/2;
		arrow[2].x=arrow[1].x;
		arrow[2].y=pos.y+GrData->udv_font.cxChar/2;
		PaintPoly(3,arrow);
	}
}

/*****************************************************************************

Function: UDV_draw_double_cursor()

Purpose: draw a double vertical cursor to modify the cxName value (i.e. the
		width of the region where names are drawn)

Parameters:	rcClip; draw only when the mouse is within rcClip
			pos; new position to draw

Return value: none

*****************************************************************************/
NLM_EXTERN void UDV_draw_double_cursor(RecT rcClip,PoinT pos)
{
	if (PtInRect(pos,&rcClip)){
		Dotted();
		MoveTo(pos.x,rcClip.top);
		LineTo(pos.x,rcClip.bottom);
		MoveTo((Int2)(pos.x+3),rcClip.top);
		LineTo((Int2)(pos.x+3),rcClip.bottom);
		Solid();
	}
}

NLM_EXTERN void UDV_draw_horizontal_line(RecT rcClip, Int4 VPos) {
/*****************************************************************************
*  draw a line from the left side of the rectangle to the right, at VPos.
*****************************************************************************/
  MoveTo(rcClip.left, VPos);
  LineTo(rcClip.right, VPos);
}

NLM_EXTERN void UDV_draw_rectangle(RecT rcClip, Boolean DotIt) {
/*****************************************************************************
*  draw a rectangle around the perimeter of rcClip.
*  no pixel is drawn twice.
*  if DotIt is TRUE, make the rectangle dotted.
*****************************************************************************/
  if (DotIt) Dotted();
  MoveTo(rcClip.left,    rcClip.top);
  LineTo(rcClip.right,   rcClip.top);
  MoveTo(rcClip.right,   rcClip.top+1);
  LineTo(rcClip.right,   rcClip.bottom);
  MoveTo(rcClip.right-1, rcClip.bottom);
  LineTo(rcClip.left,    rcClip.bottom);
  MoveTo(rcClip.left,    rcClip.bottom-1);
  LineTo(rcClip.left,    rcClip.top+1);
  if (DotIt) Solid();
}

NLM_EXTERN void UDV_draw_vertical_bar(RecT rcClip, Int4 HPos, Boolean DotIt) {
/*****************************************************************************
*  draw a thick vertical bar.
*  if DotIt is true, just draw a dotted outline.
*****************************************************************************/
  if (DotIt) Dotted();
  if (!DotIt) LtGray();
  MoveTo(HPos-2, rcClip.top);
  LineTo(HPos-2, rcClip.bottom);
  if (!DotIt) {
    MoveTo(HPos,   rcClip.top);
    LineTo(HPos,   rcClip.bottom);
    MoveTo(HPos+1, rcClip.top);
    LineTo(HPos+1, rcClip.bottom);
    Black();
  }
  MoveTo(HPos+2, rcClip.top);
  LineTo(HPos+2, rcClip.bottom);
  if (DotIt) Solid();
}

NLM_EXTERN void UDV_draw_horizontal_bar(Int4 VPos, Int4 LeftHPos, Int4 RightHPos) {
/*****************************************************************************
*  draw a thick horizontal bar.
*****************************************************************************/
  LtGray();
  MoveTo(LeftHPos-2,  VPos);
  LineTo(RightHPos+2, VPos);
  White();
  MoveTo(LeftHPos-1,  VPos+1);
  LineTo(RightHPos+1, VPos+1);
  LtGray();
  MoveTo(LeftHPos,    VPos+2);
  LineTo(RightHPos,   VPos+2);
  MoveTo(LeftHPos+1,  VPos+3);
  LineTo(RightHPos-1, VPos+3);
  Black();
  MoveTo(LeftHPos+2,  VPos+4);
  LineTo(RightHPos-2, VPos+4);
}


/*****************************************************************************

Function: UDV_deselect_feature()

Purpose: deselect a feature. 

Return value: none

*****************************************************************************/
#ifndef WWW_UDV
NLM_EXTERN void UDV_deselect_feature(ViewerDialogDataPtr vdp)
{
RecT rcP;
WindoW temport;

	vdp->Old_Item_select.eIDsel=(Uint2)-1;
	vdp->Old_Item_select.iIDsel=(Uint2)-1;
	vdp->Old_Item_select.iTypeSel=(Uint2)-1;
	/*redraw the panel*/
	vdp->Item_select.eIDsel=(Uint2)-1;
	vdp->Item_select.iIDsel=(Uint2)-1;
	vdp->Item_select.iTypeSel=(Uint2)-1;
	temport=SavePort((WindoW)ParentWindow(vdp->UnDViewer));
	Select(vdp->UnDViewer);
	ObjectRect(vdp->UnDViewer,&rcP);
	InvalRect(&rcP);
	RestorePort(temport);
}
#endif /*WWW_UDV*/

/*****************************************************************************

Function: UDV_select_feature()

Purpose: select a feature. This function is generally called in reponse to a
	message from ObjMgr.

Parameters:	p; UDV panel
            vdp; main data struct
			entityID, itemID; the feature
			bRepos; move the undviewer panel if true. When the user clicks on
			this panel, this value is false. When the user clicks on the Feature
			Dialog Box, this value is TRUE in order to show to the nice user
			the start of the clicked feature.

Return value: none

*****************************************************************************/
#ifndef WWW_UDV
NLM_EXTERN void UDV_select_feature(PaneL p,ViewerDialogDataPtr vdp,
			Uint2 entityID,Uint2 itemID,Boolean bRepos)
{
ParaGPtr pgp_select;	/*selected pgp*/
Char 	 szFeatName[500]; /*used to modify the InfoPanel Text*/
WindoW 	 temport;	/*to allow a safe draw*/
Uint2 	 nLineDecal=0;
Int4 	 decal;
Boolean	 ShowTop=FALSE,
		 ShowTick=FALSE;
RecT     rcP;

	if (vdp == NULL) return;

	vdp->Old_Item_select=vdp->Item_select;
	/*compute the position (number of lines) of the first feature within the ParaG
	by taking into account the scale*/
	if (vdp->udv_graph.udv_panel.ShowScale){
		if (vdp->udv_graph.udv_scale.ScalePosition==SCALE_POS_LEFT)
			ShowTop=FALSE;
		else ShowTop=TRUE;

		if (vdp->udv_graph.udv_scale.ShowMajorTick)
			ShowTick=TRUE;
		else ShowTick=FALSE;
	}
	else{
		ShowTop=FALSE;
		ShowTick=FALSE;
	}
	nLineDecal=1;
	if (ShowTop) nLineDecal++;
	if (ShowTick) nLineDecal++;

	if (UDV_find_item(vdp->ParaG,vdp->bsp_i.bsp,
		entityID,itemID,OBJ_SEQFEAT,szFeatName,&pgp_select,&nLineDecal,
		&vdp->Item_select)){
			decal=pgp_select->StartLine+(Int4)nLineDecal;
			if ((decal<vdp->udv_graph.udv_vscrl.ScrollPos ||
				decal>(vdp->udv_graph.udv_vscrl.ScrollPage+
				vdp->udv_graph.udv_vscrl.ScrollPos)) && bRepos){
				Int4 CurPos;
				
				CurPos=decal;
				UnDViewerVScrlUpdate(p,FALSE,CurPos);
				/*create new buffer*/
				UDV_create_buffer(&vdp->udv_graph,vdp->ParaG,&vdp->bsp_i,NULL,
					vdp->bDisplayRevComp);
				/*redraw the panel*/
				temport=SavePort((WindoW)ParentWindow(p));
				Select(p);
				ObjectRect(p,&rcP);
				InvalRect(&rcP);
				RestorePort(temport);
			}
			else {
				/*redraw the panel*/
				temport=SavePort((WindoW)ParentWindow(p));
				Select(p);
				ObjectRect(p,&rcP);
				InvalRect(&rcP);
				RestorePort(temport);
			}

			vdp->Old_Item_select.eIDsel=(Uint2)-1;
			vdp->Old_Item_select.iIDsel=(Uint2)-1;
			vdp->Old_Item_select.iTypeSel=(Uint2)-1;
	}
	if (vdp->InfoPanel) {/*InfoPanel doesn't exist for slave viewer*/
		/*use only for the Autonomous version of the viewer*/
		SetTitle(vdp->InfoPanel,szFeatName);
	}
}
#endif /*WWW_UDV*/

/*****************************************************************************

Function: UDV_SendBSPSelectMsg()

Purpose: send a Msg to select a piece of sequence

Parameters:	

Note: this function MUST be called by external software using this viewer


Return value: none

*****************************************************************************/
#ifndef WWW_UDV
static void UDV_SendBSPSelectMsg(ViewerDialogDataPtr vdp,Int4 bsp_coord,
		ParaGPtr cur_pgp,PoinT * pt)
{
Int4        first_bsp_coord, bsp_coord_old,old_pos;
Uint1       direction;/*use to tell ObjMgr the direction of the mouse (left,right)*/
SeqLocPtr   slp;/*to send an AlsoSelectMsg*/
Uint2       bsp_eID,bsp_iID;
Boolean     bDeselectAll=TRUE;

	if (ctrlKey || shftKey)
		bDeselectAll=FALSE;

	/*is the mouse going to the left or to the right ?*/
	/*don't forget to check the strand...  ;-) */
	bsp_coord_old=vdp->UDV_ms.old_col;
	first_bsp_coord=vdp->UDV_ms.first_col;

	if (vdp->UDV_ms.oldPos.x<pt->x)
		direction=Seq_strand_plus;
	else
		direction=Seq_strand_minus;
	
	/*save the new position*/
	vdp->UDV_ms.oldPos=vdp->UDV_ms.newPos=*pt;
	vdp->UDV_ms.old_col=bsp_coord;
	vdp->UDV_ms.old_pgp=cur_pgp;

	/*when udv displays a reverse/complement sequence, bsp_coords are
	actually display coords. So, I have to convert them.*/
	if (vdp->bDisplayRevComp){
		bsp_coord=UDV_RevertBioSeqCoord(bsp_coord,vdp->bsp_i.bspLength);
		first_bsp_coord=UDV_RevertBioSeqCoord(first_bsp_coord,vdp->bsp_i.bspLength);
	}
	/*'from' (first_bsp_coord) always less than 'to' (bsp_coord)*/
	if (first_bsp_coord>bsp_coord)
		swap(&first_bsp_coord,&bsp_coord);

	/*now, we can send the Select message*/
	/* first_pgp, old_pgp & cur_pgp have the same sip... 
	they are on a same row; I can use one of them*/
	bsp_eID=ObjMgrGetEntityIDForPointer((Pointer)vdp->bsp_i.bsp);
	bsp_iID = GetItemIDGivenPointer (bsp_eID, 
			OBJ_BIOSEQ, (Pointer) vdp->bsp_i.bsp);	
	if (bsp_eID!=0 && bsp_iID!=0){
		if (dblClick){
			UDV_LoadSpecificEditor(vdp->bvp,bsp_eID,bsp_iID,OBJ_BIOSEQ);
			return;
		}
		
		if (shftKey){
			slp=UDV_GetClosetSeqLocGivenBspPos(vdp->bsp_i.bsp->id,bsp_eID, 
				bsp_iID, bsp_coord, &old_pos, TRUE);
		}
		else{
			slp = SeqLocIntNew (first_bsp_coord, bsp_coord, direction, 
				vdp->bsp_i.bsp->id);
		}
		if (slp){
			ObjMgrAlsoSelect (bsp_eID, bsp_iID, 
					OBJ_BIOSEQ,OM_REGION_SEQLOC, slp);
		}
	}
}
#endif /*WWW_UDV*/

/*****************************************************************************

Function: UDV_AutoSelectAllSeq()

Purpose: send a Msg to select/deselect the full sequnce

Return value: none

*****************************************************************************/
#ifndef WWW_UDV
static void UDV_AutoSelectAllSeq(ViewerDialogDataPtr vdp,Boolean bSelect)
{
SeqLocPtr   slp;
Uint2       bsp_eID,bsp_iID;

	slp = SeqLocIntNew (0, vdp->bsp_i.bspLength-1, Seq_strand_plus, 
			vdp->bsp_i.bsp->id);
	bsp_eID=ObjMgrGetEntityIDForPointer((Pointer)vdp->bsp_i.bsp);
	bsp_iID = GetItemIDGivenPointer (bsp_eID, 
			OBJ_BIOSEQ, (Pointer) vdp->bsp_i.bsp);	
	if (bsp_eID!=0 && bsp_iID!=0 && slp){
		if (bSelect)
			ObjMgrSelect (bsp_eID, bsp_iID, 
				OBJ_BIOSEQ,OM_REGION_SEQLOC, slp);
		else
			ObjMgrDeSelect (bsp_eID, bsp_iID, 
				OBJ_BIOSEQ,OM_REGION_SEQLOC, slp);
	}
	else{
		if (slp) SeqLocFree(slp);
	}
}
#endif /*WWW_UDV*/

#ifndef WWW_UDV
NLM_EXTERN void UDV_SelectFeatInFeatDlg(ViewerMainPtr vmp, Uint2 entityID, Uint2 itemID)
{
FLMDataPtr	pflm;

	if (!vmp->hFeatDlg) return;

	pflm=(FLMDataPtr)GetObjectExtra(vmp->hFeatDlg);
	if (pflm){
		Int1 Choice;
		ScanFeatForSelect 	sffs;
		Boolean SeqFeatListAvail[SEQFEAT_MAX];

		Choice=pflm->SeqFeatClass[GetValue(pflm->pop)-1];
		sffs.eID=entityID;		
		sffs.iID=itemID;		
		sffs.index=0;
		sffs.compteur=0;
		MemSet(sffs.SeqFeatListAvail,0,
				sizeof(sffs.SeqFeatListAvail));
		if (Choice==0){/*0 means All*/
			MemSet(SeqFeatListAvail,1,
					sizeof(SeqFeatListAvail));
		}
		else{/*otherwise search only a particular feat.*/
			MemSet(SeqFeatListAvail,0,
					sizeof(SeqFeatListAvail));
			SeqFeatListAvail[Choice]=TRUE;
		}	
		SeqMgrExploreFeatures (vmp->vdp->bsp_i.bsp, 
			(Pointer) &sffs,UDV_FeaturesListBoxFind, 
			NULL, SeqFeatListAvail, NULL);
		if (sffs.index>0)
			OwnerDrawLbox_SelectItem(pflm->lbox,sffs.index);
	}
}
#endif /*WWW_UDV*/



/*****************************************************************************

Function: UDV_ClickProc()

Purpose: manage Mouse-Click; this function is designed for the autonomous viewer
		to manage the InfoPanel and the Features List Dialog Box
		DON'T USE FOR EXTERNAL SOFTWARE...
		
Parameters:	p; panel handle (currently unused)
			pt; new mouse position

Return value: none

*****************************************************************************/
#ifndef WWW_UDV
NLM_EXTERN void UDV_ClickProc(PaneL p, PoinT pt)
{
WindoW 				w;
ViewerDialogDataPtr vdp;
ValNodePtr 			vnp;
ViewerMainPtr		vmp;
SeqLocPtr           slp;
ParaGPtr 			pgp=NULL;
RecT 				rc,rcP;
Int4 				decal_top,pos,old_pos,decal,limit,dec,dec2;
Boolean 			bFind=FALSE;
Int2				decal_left;
Uint2               bsp_eID,bsp_iID;

	/*get some usefull data...*/
	vdp = (ViewerDialogDataPtr) GetObjectExtra (p);
	if (vdp == NULL) return;
	
	/*Select(p);*/
	dec=vdp->udv_graph.udv_panel.cxName+
			vdp->udv_graph.udv_scale.cxLeftScale+
			vdp->udv_graph.udv_font.cxChar;
	dec2=dec+(vdp->udv_graph.udv_panel.nCharByLine+
		vdp->udv_graph.udv_panel.nBlockByLine)*
		vdp->udv_graph.udv_font.cxChar;
	/*is the mouse located in the ParaG or Name region ?*/
	if (pt.x>=dec && pt.x<=dec2){
		ObjectRect(p,&rcP);
		decal_top=vdp->udv_graph.udv_vscrl.ScrollPos*
				vdp->udv_graph.udv_font.LineHeight-VIEWER_VERT_MARGIN;
		decal_left=vdp->udv_graph.udv_panel.cxName+rcP.left+
				vdp->udv_graph.udv_scale.cxLeftScale;
		if (vdp->udv_graph.udv_vscrl.ScrollPage>0)
			limit=decal_top+vdp->udv_graph.udv_vscrl.ScrollPage*
				vdp->udv_graph.udv_font.LineHeight+rcP.top;
		else
			limit=decal_top+vdp->udv_graph.udv_panel.nTotLines*
				vdp->udv_graph.udv_font.LineHeight+rcP.top;
		/*look for the ParaG*/
		if (vdp->ParaG){
			for(vnp=vdp->ParaG ; vnp!=NULL ; vnp=vnp->next){
				if (vnp->data.ptrvalue){
					pgp=(ParaGPtr)vnp->data.ptrvalue;

					/*Compute rect*/
					decal=pgp->StartLine*
							vdp->udv_graph.udv_font.LineHeight+rcP.top;
					if (decal>limit) break;
					if (decal>=decal_top){
						rc=UDV_calc_RCdraw(pgp->StartLine,pgp->nLines,rcP,
							decal_left,decal_top,
							vdp->udv_graph.udv_font.LineHeight);
						if (PtInRect(pt,&rc)){
							vdp->UDV_ms.rcClip=rc;
							bFind=TRUE;
							break;
						}
					}
				}
			}		
		}
		if (bFind){
			Boolean ClickFeat=FALSE;
			Uint2 entityID;
			Uint2 itemID;
			Uint2 itemType;
			Uint2 index;
			/*the user clicked on a feature ?*/
			if (vdp->udv_graph.udv_panel.ShowFeatures &&
				vdp->UDV_ms.Action_type==MS_ACTION_FEAT_NOTHING){
				ClickFeat=UDV_click_item(&vdp->udv_graph,pt,rc,pgp,vdp->bsp_i,
					&entityID,&itemID,&itemType,&index);
				if (ClickFeat) {
					if (dblClick){
						UDV_LoadSpecificEditor(vdp->bvp,entityID,itemID,itemType);
					}
					else{
						ObjMgrDeSelectAll();
						ObjMgrSelect(entityID,itemID,OBJ_SEQFEAT,0,NULL);
						/*this line if for Cn3D only*/
						ObjMgrSendMsg(OM_MSG_MOUSEUP, entityID, itemID, OBJ_BIOSEQ);
						if (vdp->Parent) 
							vmp=(ViewerMainPtr)GetObjectExtra(vdp->Parent);
						if (vmp && vmp->hFeatDlg){/*update Features List Dlg if needed;
							to do : convert this Feature ListBox to a real Viewer able
							to play with ObjMgr*/
							UDV_SelectFeatInFeatDlg(vmp,entityID,itemID);
						}
					}
				}
			}
			/*click outside a feature*/
			if (!ClickFeat){/*disable old feat select, then select letters*/
				/*prepare the deselection*/
				vdp->Old_Item_select=vdp->Item_select;
				vdp->Item_select.eIDsel=(Uint2)-1;
				vdp->Item_select.iIDsel=(Uint2)-1;
				vdp->Item_select.iTypeSel=(Uint2)-1;
				/*draw viewer: do the deselection*/
				UDV_draw_viewer(p);
				/*feat select is now invalidated*/
				vdp->Item_select=vdp->Old_Item_select;
				
				/*deselect letters ?*/
				if (!(ctrlKey || shftKey))
					ObjMgrDeSelectAll ();

				pos=UDV_calc_pos(&vdp->udv_graph,&pt,&vdp->UDV_ms.rcClip,
					pgp,NULL)-1;
				vdp->UDV_ms.newPos=pt;
				vdp->UDV_ms.Action_type=MS_ACTION_SELECT_SEQ;
				vdp->UDV_ms.first_col=pos;
				vdp->UDV_ms.first_pgp=pgp;
				UDV_SendBSPSelectMsg(vdp, pos, pgp, &pt);
			}
		}
	}else
	/*is the mouse located on the 3D line (between Name list region and ParaG) ?*/
	if (pt.x>=vdp->udv_graph.udv_panel.cxName && 
		pt.x<=vdp->udv_graph.udv_panel.cxName+3){
			ObjectRect(p,&rc);
			rc.left=5*vdp->udv_graph.udv_font.cxChar; /*min value*/
			rc.right=2*PANEL_NAME_WIDTH*vdp->udv_graph.udv_font.cxChar;/*max val*/
			pt.x=vdp->udv_graph.udv_panel.cxName;
			vdp->UDV_ms.oldPos=pt;
			vdp->UDV_ms.newPos=pt;
			vdp->UDV_ms.rcClip=rc;
			vdp->UDV_ms.Action_type=MS_ACTION_RESIZE_WIN;
			InvertMode();	
			UDV_draw_double_cursor(vdp->UDV_ms.rcClip,
				vdp->UDV_ms.oldPos);
			CrossCursor();
	}
	else{
		WindoW temport;
		RecT rcI;
		CharPtr szFeatName;
		Uint2 bsp_eID,bsp_iID;
			/*deselect the sequence*/
			ObjMgrDeSelectAll ();
			/*deselection of a the feature*/
			memset(&vdp->Item_select,(Uint2)-1,sizeof(vdp->Item_select));
			memset(&vdp->Old_Item_select,(Uint2)-1,sizeof(vdp->Old_Item_select));
			temport=SavePort((WindoW)p);
			Select(p);
			ObjectRect(p,&rc);
			InvalRect(&rc);
			RestorePort(temport);
			if (vdp->InfoPanel){
				SetTitle(vdp->InfoPanel,"Ready !");
			}
			/*this line if for Cn3D only*/
            bsp_eID=ObjMgrGetEntityIDForPointer((Pointer)vdp->bsp_i.bsp);
	        bsp_iID = GetItemIDGivenPointer (bsp_eID, 
			                        OBJ_BIOSEQ, (Pointer) vdp->bsp_i.bsp);	
			ObjMgrSendMsg(OM_MSG_MOUSEUP, bsp_eID, bsp_iID, OBJ_BIOSEQ);
	}
}
#endif /*WWW_UDV*/


/*****************************************************************************

Function: UDV_ManageDragHoldActions()

Purpose: manage Mouse-Drag and Mouse-Hold for sequence letters selection ONLY

Parameters:	p; panel handle (currently unused)
			vdp; viewer data struct
			pt; new position

Return value: none

*****************************************************************************/
#ifndef WWW_UDV
void UDV_ManageDragHoldActions(PaneL p, ViewerDialogDataPtr vdp,PoinT pt,
	Boolean bHold)
{			
ValNodePtr 			vnp;
ParaGPtr 			pgp=NULL;
RecT 				rc,rcP;
Int4 				decal_top,pos,nLines,decal,limit;
Boolean 			bFind=FALSE;
Int2				decal_left;

	/*does the mouse located in the ParaG or Name region ?*/
	if ((pt.x>(vdp->udv_graph.udv_panel.cxName+
			vdp->udv_graph.udv_scale.cxLeftScale+
			vdp->udv_graph.udv_font.cxChar))){

		ObjectRect(p,&rcP);
		decal_top=vdp->udv_graph.udv_vscrl.ScrollPos*
				vdp->udv_graph.udv_font.LineHeight-VIEWER_VERT_MARGIN;
		decal_left=vdp->udv_graph.udv_panel.cxName+rcP.left+
				vdp->udv_graph.udv_scale.cxLeftScale;
		if (vdp->udv_graph.udv_vscrl.ScrollPage>0)
			limit=decal_top+vdp->udv_graph.udv_vscrl.ScrollPage*
				vdp->udv_graph.udv_font.LineHeight+rcP.top;
		else
			limit=decal_top+vdp->udv_graph.udv_panel.nTotLines*
				vdp->udv_graph.udv_font.LineHeight+rcP.top;
		/*look for the ParaG*/
		if (vdp->ParaG){
			nLines=0;
			if (vdp->udv_graph.udv_scale.ShowMajorTick) nLines++;
			if (vdp->udv_graph.udv_scale.ScalePosition==SCALE_POS_TOP || 
					vdp->udv_graph.udv_scale.ScalePosition==SCALE_POS_BOTH) nLines++;
			for(vnp=vdp->ParaG ; vnp!=NULL ; vnp=vnp->next){
				if (vnp->data.ptrvalue){
					pgp=(ParaGPtr)vnp->data.ptrvalue;
					/*Compute rect; because drag is allowed only for selection
					of letters, restrict the RecT on the sequence*/
					decal=pgp->StartLine*
							vdp->udv_graph.udv_font.LineHeight+rcP.top;
					if (decal>limit) break;
					if (decal>=decal_top){
						rc=UDV_calc_RCdraw(pgp->StartLine+nLines,1,rcP,
							decal_left,decal_top,
							vdp->udv_graph.udv_font.LineHeight);
						if (PtInRect(pt,&rc)){
							vdp->UDV_ms.rcClip=rc;
							bFind=TRUE;
							break;
						}
					}
				}
			}		
		}
		if (bHold){/*vertical auto-scroll, if needed (only on Hold mouse action)*/
			BaR vsb;
			Int4 old_pos;	

				vsb = GetSlateVScrollBar ((SlatE) vdp->UnDViewer);
				old_pos=GetBarValue(vsb);
				if (pt.y>=rcP.bottom-15){
					SetValue(vsb,old_pos+2);
				}
				else if (pt.y<=rcP.top+15){
					SetValue(vsb,old_pos-2);
				}
		}
		if (bFind){
			pos=UDV_calc_pos(&vdp->udv_graph,&pt,&vdp->UDV_ms.rcClip,
				pgp,NULL)-1;
			if (vdp->UDV_ms.old_col==pos)
				return;
			UDV_SendBSPSelectMsg(vdp, pos, pgp, &pt);
		}
	}
}
#endif /*WWW_UDV*/

/*****************************************************************************

Function: UDV_DragMouse()

Purpose: manage Mouse-Drag

Parameters:	p; panel handle (currently unused)
			vdp; viewer data struct
			pt; new position

Note: this function MUST be called by external software using this viewer

Return value: none

*****************************************************************************/
#ifndef WWW_UDV
NLM_EXTERN void UDV_DragMouse(PaneL p, ViewerDialogDataPtr vdp,PoinT pt)
{

	if (vdp == NULL) return;

	switch(vdp->UDV_ms.Action_type){
		case MS_ACTION_FEAT_NOTHING:
			break;
		case MS_ACTION_SELECT_SEQ:
			UDV_ManageDragHoldActions(p,vdp,pt,FALSE);
			break;
		case MS_ACTION_RESIZE_WIN:
			InvertMode();	
			UDV_draw_double_cursor(vdp->UDV_ms.rcClip,
				vdp->UDV_ms.oldPos);
			vdp->UDV_ms.newPos=pt;
			UDV_draw_double_cursor(vdp->UDV_ms.rcClip,
				vdp->UDV_ms.newPos);
			vdp->UDV_ms.oldPos=vdp->UDV_ms.newPos;
			Update();
			break;
	}
}
#endif /*WWW_UDV*/

/*****************************************************************************

Function: UDV_DragProc()

Purpose: manage Mouse-Drag

Parameters:	p; panel handle (currently unused)
			pt; new position

Return value: none

*****************************************************************************/
#ifndef WWW_UDV
NLM_EXTERN void UDV_DragProc(PaneL p, PoinT pt)
{
WindoW 				w;
ViewerDialogDataPtr vdp;
ViewerMainPtr		vmp;

	/*get some usefull data...*/
	vdp = (ViewerDialogDataPtr) GetObjectExtra (p);
	
	UDV_DragMouse(p,vdp,pt);
}
#endif /*WWW_UDV*/

/*****************************************************************************

Function: UDV_ReleaseMouse()

Purpose: manage Mouse-Release

Parameters:	p; panel handle (currently unused)
			vdp; viewer data struct
			pt; new position

Note: this function MUST be called by external software using this viewer

Return value: none

*****************************************************************************/
#ifndef WWW_UDV
NLM_EXTERN void UDV_ReleaseMouse(PaneL p,ViewerDialogDataPtr vdp, PoinT pt)
{
Uint2       bsp_eID,bsp_iID;

	if (vdp == NULL) return;
	
	switch(vdp->UDV_ms.Action_type){
		case MS_ACTION_FEAT_NOTHING:
            bsp_eID=ObjMgrGetEntityIDForPointer((Pointer)vdp->bsp_i.bsp);
	        bsp_iID = GetItemIDGivenPointer (bsp_eID, 
			                        OBJ_BIOSEQ, (Pointer) vdp->bsp_i.bsp);	
			break;
		case MS_ACTION_SELECT_SEQ:
            bsp_eID=ObjMgrGetEntityIDForPointer((Pointer)vdp->bsp_i.bsp);
	        bsp_iID = GetItemIDGivenPointer (bsp_eID, 
			                        OBJ_BIOSEQ, (Pointer) vdp->bsp_i.bsp);	
            ObjMgrSendMsg(OM_MSG_MOUSEUP, bsp_eID, bsp_iID, OBJ_BIOSEQ);
			break;
		case MS_ACTION_RESIZE_WIN:
			InvertMode();	
			UDV_draw_double_cursor(vdp->UDV_ms.rcClip,
				vdp->UDV_ms.oldPos);
			/*redraw panel with new 'cxName' value*/
			if (PtInRect(vdp->UDV_ms.newPos,&vdp->UDV_ms.rcClip)){
				RecT rc;

				ObjectRect(p,&rc);
				rc.left=0;/*for obscure reasons, not == 0*/
				rc.top=0;
				vdp->udv_graph.udv_panel.cxName=vdp->UDV_ms.newPos.x;
				UDV_resize_viewer( p, vdp);
				InvalRect(&rc);
			}
			break;
	}
	
  ClearUDV_mouse_select(&(vdp->UDV_ms));
	vdp->UDV_ms.Action_type=MS_ACTION_FEAT_NOTHING;
	ArrowCursor();
}
#endif /*WWW_UDV*/

/*****************************************************************************

Function: UDV_ReleaseProc()

Purpose: manage Mouse-Release

Parameters:	p; panel handle (currently unused)
			pt; new position

Return value: none

*****************************************************************************/
#ifndef WWW_UDV
NLM_EXTERN void UDV_ReleaseProc(PaneL p, PoinT pt)
{
WindoW 				w;
ViewerDialogDataPtr vdp;
ViewerMainPtr		vmp;

	/*get some usefull data...*/
	vdp = (ViewerDialogDataPtr) GetObjectExtra (p);

	UDV_ReleaseMouse(p,vdp,pt);
}
#endif /*WWW_UDV*/


/*****************************************************************************

Function: UDV_HoldProc()

Purpose: manage Mouse-Hold

Parameters:	p; panel handle (currently unused)
			pt; new position

Return value: none

*****************************************************************************/
#ifndef WWW_UDV
NLM_EXTERN void UDV_HoldProc(PaneL p, PoinT pt)
{
WindoW 				w;
ViewerDialogDataPtr vdp;
ViewerMainPtr		vmp;

	/*get some usefull data...*/
	vdp = (ViewerDialogDataPtr) GetObjectExtra (p);

	if (vdp==NULL) return;
		
	switch(vdp->UDV_ms.Action_type){
		case MS_ACTION_FEAT_NOTHING:
			break;
		case MS_ACTION_SELECT_SEQ:
			UDV_ManageDragHoldActions(p,vdp,pt,TRUE);
			break;
		case MS_ACTION_RESIZE_WIN:
			break;
		default:
			break;
	}
}
#endif

/*******************************************************************************

  Function : UDV_draw_helixDNA()
  
  Purpose : draw a little peace of a DNA helix for the NCBI logo
  
  Parameters :	x,y; start position of the helix
  				cxChar,cyChar; size of the current font
				bEnd; if True; draw half an helix

  Return value : none

*******************************************************************************/
static void UDV_draw_helixDNA(Int2 x,Int2 y,Int2 cxChar,Int2 cyChar,
		Boolean bEnd)
{

Int2 cxChar_2,cyChar_2;
PoinT box[4];

	cxChar_2=cxChar/2;
	cyChar_2=cyChar/2;
	SetColor(GetColorRGB(0,0,255));
	/*1*/
	box[0].x=x+cxChar_2;
	box[0].y=y+2*cyChar;
	box[1].x=x;
	box[1].y=box[0].y-cyChar;
	box[2].x=x+cxChar;
	box[2].y=box[1].y;
	box[3].x=box[2].x+cxChar_2;
	box[3].y=box[0].y;
	
	PaintPoly(4,box);

	SetColor(GetColorRGB(0,0,128));
	/*2*/
	box[0].x=box[3].x;
	box[0].y=box[3].y;
	box[1].x=box[2].x;
	box[1].y=box[2].y;
	box[2].x=box[1].x+2*cxChar+cxChar_2;
	box[2].y=box[0].y-5*cyChar;
	box[3].x=box[2].x+cxChar_2;
	box[3].y=box[2].y+cyChar;
	PaintPoly(4,box);
	
	if (!bEnd){
		SetColor(GetColorRGB(0,0,255));
		/*3*/
		box[0].x=box[3].x;
		box[0].y=box[3].y;
		box[1].x=box[2].x;
		box[1].y=box[2].y;
		box[2].x=box[1].x+cxChar;
		box[2].y=box[1].y;
		box[3].x=box[2].x+cxChar_2;
		box[3].y=box[0].y;
		PaintPoly(4,box);
	}

	SetColor(GetColorRGB(128,0,128));
	/*4*/
	box[0].x=x;
	box[0].y=y-2*cyChar;
	box[1].x=x+cxChar_2;
	box[1].y=box[0].y-cyChar;
	box[2].x=box[0].x+cxChar;
	box[2].y=box[0].y;
	PaintPoly(3,box);

	if (!bEnd){
		SetColor(GetColorRGB(192,92,192));
		/*5*/
		box[0].x=box[1].x;
		box[0].y=box[1].y;
		box[1].x=box[0].x+cxChar;
		box[1].y=box[0].y;
		box[2].x=box[1].x+3*cxChar;
		box[2].y=box[0].y+5*cyChar;
		box[3].x=box[2].x-cxChar;
		box[3].y=box[2].y;
		PaintPoly(4,box);

		SetColor(GetColorRGB(128,0,128));
		/*7*/
		box[0].x=box[2].x;
		box[0].y=box[2].y;
		box[1].x=box[0].x-cxChar_2;
		box[1].y=box[0].y-cyChar;
		box[2].x=box[1].x+2*cxChar;
		box[2].y=box[1].y-3*cyChar;
		box[3].x=box[2].x+cxChar;
		box[3].y=box[2].y;
		PaintPoly(4,box);

		SetColor(GetColorRGB(0,0,255));
		/*8*/
		box[0].x=x+4*cxChar;
		box[0].y=y-2*cyChar;
		box[1].x=box[0].x+cxChar;
		box[1].y=box[0].y;
		box[2].x=box[0].x+2*cxChar+cxChar_2+cxChar_2;
		box[2].y=box[0].y+3*cyChar;
		box[3].x=box[2].x-cxChar;
		box[3].y=box[2].y;
		PaintPoly(4,box);
	}
	Black();
}

/*******************************************************************************

  Function : draw_logo()
  
  Purpose : draw the NCBI logo for the main window
  
  Parameters :	rcP; where to put the logo
  				f1,f2,f3; the fonts used to display the logo text

  Return value : none

*******************************************************************************/
#ifndef WWW_UDV
static void  draw_logo(RecT rcP,FonT f1,FonT f2,FonT f3,CharPtr szTxt0,
	CharPtr szTxt4)
{
Char szTxt1[]="01101011";
Char szTxt2[]="National Center for";
Char szTxt3[]="Biotechnology Information";
Int2 pos,cxChar=7,cyChar=13,cxGlobal,cyGlobal;
Int2 w1,w2,w3,y,x;
RecT rcL;

	/*some computation for positionning*/
	SelectFont(f1);
	w1=StringWidth(szTxt1);
	SelectFont(f2);
	w2=StringWidth(szTxt2);
	w3=StringWidth(szTxt3);
	cxGlobal=22*cxChar+w1+w2;
	cyGlobal=10*cyChar;
	x=(rcP.right-rcP.left)/2;
	y=(rcP.bottom-rcP.top)/2;
	rcL.left=rcP.left+x-(cxGlobal/2);
	rcL.top=rcP.top+y-(6*cyChar)/2;
	rcL.right=rcL.left+cxGlobal;
	rcL.bottom=rcL.top+6*cyChar;

	/*double DNA helix*/
	UDV_draw_helixDNA(rcL.left,rcL.bottom,cxChar,cyChar,FALSE);
	UDV_draw_helixDNA((Int2)(rcL.left+6*cxChar),rcL.bottom,cxChar,cyChar,FALSE);
	UDV_draw_helixDNA((Int2)(rcL.left+12*cxChar),rcL.bottom,cxChar,cyChar,TRUE);

	/*Undviewer*/
	SelectFont(f3);
	Blue();
	MoveTo((Int2)(rcL.left+2*cxChar),(Int2)(rcL.bottom-6*cyChar));
	PaintString(szTxt0);

	Black();
	MoveTo((Int2)(rcL.left+StringWidth(szTxt0)+3*cxChar),(Int2)(rcL.bottom-6*cyChar));
	SelectFont(f1);
	PaintString(szTxt4);

	/*01101011*/
	pos=rcL.left+13*cxChar;
	SelectFont(f1);
	Black();
	MoveTo(pos,rcL.bottom);
	PaintString(szTxt1);
	pos+=w1;

	/*NCBI*/
	SelectFont(f2);
	pos+=(w3/2+2*cxChar);

	MoveTo((Int2)(pos-w2/2),(Int2)(rcL.bottom-cyChar));
	PaintString(szTxt2);
	MoveTo((Int2)(pos-w3/2),(Int2)(rcL.bottom+cyChar));
	PaintString(szTxt3);
}
#endif /*WWW_UDV*/

/*******************************************************************************

  Function : UDV_draw_select()
  
  Purpose : draw a rect around a selected item
  
  Parameters :	rc; rect coordinates

  Return value : none

*******************************************************************************/
static void  UDV_draw_select(RecT rc)
{
	Dotted();
	FrameRect(&rc);
	Solid();
}

/*******************************************************************************

  Function : UDV_draw_ss_cont()
  
  Purpose : complete the end(s) of secondary structure to highlight the case
  			of a truncation at ParaG ends
  
  Parameters :	start; draw the arrow from...
				stop;... to.
				StartLetter; beginning of a ParaG (zero-based value)
				cxChar; width of a letter
				xMargin; draw in the ParaG on the right a this value
				y2; vertical position to draw the sequence
				ContinueRight; draw on the right only if TRUE
				ContinueLeft; draw on the left only if TRUE

  Return value : none

*******************************************************************************/
static void  UDV_draw_ss_cont(Int4 start,Int4 stop,Int4 StartLetter,
		Int2 cxChar,Int2 xMargin,Int2 y2,Boolean ContinueRight,
		Boolean ContinueLeft)
{
Int2 x;

	if (ContinueRight || ContinueLeft){
		Dotted();
		Red();
	}
	if (ContinueLeft){
		x=(((start-StartLetter)+(start-StartLetter)/
			LETTER_BLOCK_WIDTH)*cxChar)+xMargin-cxChar/2;
		
		MoveTo(x,y2);
		LineTo((Int2)(x-2*cxChar),y2);
	}
	if (ContinueRight){
		Int4 stop2;
		stop2=stop+((stop-start)/LETTER_BLOCK_WIDTH);
		x=(((stop2-StartLetter)+(stop2-StartLetter)/
			LETTER_BLOCK_WIDTH)*cxChar)+xMargin;
		
		MoveTo(x,y2);
		LineTo((Int2)(x+2*cxChar),y2);
	}
	
	if (ContinueRight || ContinueLeft){
		Solid();
		Black();
	}
}
/*******************************************************************************

  Function : UDV_draw_struc_strand()
  
  Purpose : draw the strand (prot struct only) as an arrow
  
  Parameters :	start; draw the arrow from...
				stop;... to.
				StartLetter; beginning of a ParaG (zero-based value)
				y; vertical position to draw the sequence
				cxChar; width of a letter
				cyChar; height of a letter
				LineHeight; height of a feature line
				xMargin; draw in the ParaG on the right a this value
				clr; color
				ContinueRight; draw on the right only if TRUE
				ContinueLeft; draw on the left only if TRUE

  Return value : none

*******************************************************************************/
static void  UDV_draw_struc_strand(Int4 start,Int4 stop,Int4 StartLetter,
			Int2 y,Int2 cxChar,Int2 cyChar,Int2 LineHeight,Int2 xMargin,
			Uint4 clr,Boolean ContinueLeft,Boolean ContinueRight)
{
PoinT box[7];
Int2 y2,x,cxChar_2,cyChar_2,cyChar_4;

	y2=y-LineHeight/2;
	cxChar_2=cxChar/2;
	cyChar_4=cyChar/4;
	cyChar_2=cyChar/2;
	
	/*ContinueRight || ContinueLef: draw thin line to highlight continuous
	secondary structure at ParaG ends*/
	if (ContinueLeft || ContinueRight)
		UDV_draw_ss_cont(start,stop,StartLetter,cxChar,xMargin,y2,ContinueRight,
		ContinueLeft);
	if (start!=stop){
		x=(((start-StartLetter)+(start-StartLetter)/
			LETTER_BLOCK_WIDTH)*cxChar)+xMargin+cxChar_2;

		box[0].x=x-cxChar_2;
		box[0].y=y2-cyChar_4;
		box[6].x=box[0].x;
		box[6].y=y2+cyChar_4;

		/*3d effect*/
		DkGray();
		MoveTo((Int2)(box[0].x-1),(Int2)(box[0].y+1));
		LineTo((Int2)(box[6].x-1),(Int2)(box[6].y+1));
		MoveTo((Int2)(box[0].x-2),(Int2)(box[0].y+2));
		LineTo((Int2)(box[6].x-2),(Int2)(box[6].y+2));

		/*stop+=((stop-start)/LETTER_BLOCK_WIDTH);*/
		x=(((stop-StartLetter)+(stop-StartLetter)/
			LETTER_BLOCK_WIDTH)*cxChar)+xMargin;
		x-=cxChar_2;

		box[1].x=x-cxChar_2;
		box[1].y=box[0].y;
		box[2].x=box[1].x;
		box[2].y=y2-cyChar_2;
		box[3].x=x+cxChar_2;
		box[3].y=y2;
		box[4].x=box[1].x;
		box[4].y=y2+cyChar_2;
		box[5].x=box[1].x;
		box[5].y=box[6].y;

		/*3d effect*/
		MoveTo((Int2)(box[6].x-1),(Int2)(box[6].y+1));
		LineTo((Int2)(box[5].x),(Int2)(box[5].y+1));
		LineTo((Int2)(box[4].x),(Int2)(box[4].y+1));
		MoveTo((Int2)(box[6].x-2),(Int2)(box[6].y+2));
		LineTo((Int2)(box[5].x-1),(Int2)(box[5].y+2));
		LineTo((Int2)(box[4].x-1),(Int2)(box[4].y+2));

		MoveTo(box[4].x,(Int2)(box[4].y+2));
		LineTo(box[3].x,(Int2)(box[3].y+2));

		if (clr!=(Uint4)-1) SetColor(clr);
		else Black();
		/*draw*/
		PaintPoly(7,box);
	}
	else{/*if start==stop, draw only the end of the arrow*/
		stop+=((stop-start)/LETTER_BLOCK_WIDTH);
		x=(((stop-StartLetter)+(stop-StartLetter)/
			LETTER_BLOCK_WIDTH)*cxChar)+xMargin/*-cxChar_2*/;

		box[0].x=x-cxChar_2;
		box[0].y=y2-cyChar_2;
		box[1].x=x+cxChar_2;
		box[1].y=y2;
		box[2].x=box[0].x;
		box[2].y=y2+cyChar_2;

		/*3d effect*/
		DkGray();
		MoveTo((Int2)(box[0].x-1),(Int2)(box[0].y+1));
		LineTo((Int2)(box[2].x-1),(Int2)(box[2].y+1));
		MoveTo((Int2)(box[0].x-2),(Int2)(box[0].y+2));
		LineTo((Int2)(box[2].x-2),(Int2)(box[2].y+2));
		MoveTo((Int2)(box[2].x),(Int2)(box[2].y+2));
		LineTo((Int2)(box[1].x),(Int2)(box[1].y+2));

		if (clr!=(Uint4)-1) SetColor(clr);
		else Black();
		/*draw*/
		PaintPoly(3,box);
	}
	Black();	
}


/*******************************************************************************

  Function : UDV_draw_struc_helix()
  
  Purpose : draw the helix (prot struct only) as an helix
  
  Parameters :	start; draw the arrow from...
				stop;... to.
				StartLetter; beginning of a ParaG (zero-based value)
				y; vertical position to draw the sequence
				cxChar; width of a letter
				cyChar; height of a letter
				LineHeight; height of a feature line
				xMargin; draw in the ParaG on the right a this value
				clr; color
				ContinueRight; draw on the right only if TRUE
				ContinueLeft; draw on the left only if TRUE
	   
  Return value : none

*******************************************************************************/
static void  UDV_draw_struc_helix(Int4 start,Int4 stop,Int4 StartLetter,
			Int2 y,Int2 cxChar,Int2 cyChar,Int2 LineHeight,Int2 xMargin,
			Uint4 clr,Boolean ContinueLeft,Boolean ContinueRight)
{
PoinT box[4],pt0,pt2;
Int4 i;
Int2 y2,x,xDecal,cxChar_2,cyChar_2,cxChar_4;
Int1 status=DRAW_HELIX_DOWN;
Boolean bContPrev=FALSE;

	cxChar_2=cxChar/2;
	cxChar_4=cxChar/4;
	cyChar_2=cyChar/2;

	y2=y-LineHeight/2;

	/*ContinueRight || ContinueLef: draw thin line to highlight continuous
	secondary structure at ParaG ends*/
	if (ContinueLeft || ContinueRight)
		UDV_draw_ss_cont(start,stop,StartLetter,cxChar,xMargin,y2,ContinueRight,
		ContinueLeft);

	/*stop+=((stop-start)/LETTER_BLOCK_WIDTH);*/
	stop+=((stop-StartLetter)/LETTER_BLOCK_WIDTH)-
		((start-StartLetter)/LETTER_BLOCK_WIDTH);

	xDecal=((start-StartLetter)+((start-StartLetter)/LETTER_BLOCK_WIDTH))
			*cxChar+xMargin;
	for(i=start;i<=stop;i++){
		x=(i-start)*cxChar+xDecal;
		switch(status){
			case DRAW_HELIX_DOWN:
				/*compute polygon*/
				box[0].x=x;
				box[0].y=y2+cyChar_2;
				box[1].x=x-cxChar_2;
				box[1].y=y2;
				box[2].x=x+cxChar_4;
				box[2].y=y2;
				box[3].x=x+cxChar_2+cxChar_4;
				box[3].y=box[0].y;
				/*set color*/
				if (clr!=(Uint4)-1) SetColor(clr);
				else LtGray();
				/*draw*/
				PaintPoly(4,box);
				if (bContPrev){
					box[1]=pt0;
					box[3]=box[2];
					box[2]=pt2;
					bContPrev=FALSE;
					PaintPoly(4,box);
				}
				/*after DRAW_HELIX_DOWN will trace a DRAW_HELIX_MIDDLE*/
				status=DRAW_HELIX_MIDDLE;				
				break;
			case DRAW_HELIX_MIDDLE:
				/*compute polygon*/
				box[0].x=x-cxChar_4;
				box[0].y=y2+cyChar_2;
				box[1].x=x-cxChar+cxChar_4;
				box[1].y=y2;
				box[2].x=x+cxChar_2-cxChar_4;
				box[2].y=y2-cyChar_2;
				box[3].x=x+cxChar-cxChar_4;
				box[3].y=y2;
				/*set color: always dark gray*/
				DkGray();
				/*draw*/
				PaintPoly(4,box);
				/*after DRAW_HELIX_MIDDLE will trace a DRAW_HELIX_UP*/
				status=DRAW_HELIX_UP;				
				break;
			case DRAW_HELIX_UP:
				/*compute polygon*/
				box[0].x=x-cxChar_4;
				box[0].y=y2;
				box[1].x=x-cxChar_2-cxChar_4;
				box[1].y=y2-cyChar_2;
				box[2].x=x;
				box[2].y=box[1].y;
				box[3].x=x+cxChar_2;
				box[3].y=y2;
				/*set color: always dark gray*/
				if (clr!=(Uint4)-1) SetColor(clr);
				else LtGray();
				/*draw*/
				PaintPoly(4,box);
				/*after DRAW_HELIX_UP will trace a DRAW_HELIX_DOWN*/
				status=DRAW_HELIX_DOWN;
				bContPrev=TRUE;
				pt0=box[0];
				pt2=box[2];
				break;
		}
	}
	Black();
}

/*******************************************************************************

  Function : UDV_draw_CDS_minus()
  
  Purpose : draw the translation of a CDS located on a MINUS strand
  
  Parameters : context; feature data (see explore.h for a def of the structure)
  				start; draw the sequence from...
				stop;... to.
				StartLetter; beginning of a ParaG (zero-based value)
				GrData; graphical data
				y; vertical position to draw the sequence
				i; order number of the exon (if CDS is exon-encoded)
				xMargin; draw in the ParaG on the right a this value
				UseDefClr; TRUE: draw the sequence using the DefClr value
					Otherwise use GrData->AA_LayoutPal[] values
  
  Return value : none

*******************************************************************************/
static void  UDV_draw_CDS_minus(SeqMgrFeatContextPtr context,Int4 start,
			Int4 stop,Int4 StartLetter,UnDViewerGraphDataPtr GrData,Int2 y,
			Int2 i,Int2 xMargin,Boolean UseDefClr,Uint4 DefClr)
{						
CharPtr      	str=NULL;
Int4			il=0,
				pos=0,
				n=0,
				stop_prot=0;
Int2 			nCompt=0,
				decal=0,
				x_prot=0,
				y_prot=0,
				numivals2=0,
				cxChar_2;
ByteStorePtr	bs=NULL;
SeqIdPtr sip=NULL;
Int4 gi=0;
Boolean bGiForProductOk=FALSE;
BioseqPtr prot;

	/*retrieve the protein sequence; need to be optimized in future release*/
	if (context->sfp->product){
		prot=BioseqFind(SeqLocId(context->sfp->product));
		sip=(SeqIdPtr)GetProductSeqId(context->sfp->product);
		if (sip && prot){
			CharPtr str2;
			gi = GetGINumFromSip(sip);
			if (gi>0) bGiForProductOk=TRUE;
			str2=UDV_Read_Sequence (sip,0,prot->length-1, 
				TRUE,prot->length-1);
			if (!str2) return;
			str=str2;
		}
	}
	else{
		bs = ProteinFromCdRegion (context->sfp, TRUE);
		if (bs){
			str = (CharPtr) BSMerge (bs, NULL);
			BSFree (bs);
		}
	}

	if (!str) return;	

	/*count the lenght of the intron(s)*/
	numivals2=context->numivals*2;
	if (i>0){
		for(nCompt=i;nCompt>0;nCompt-=2){
			il+=(context->ivals[nCompt-2]-
				context->ivals[nCompt+1]-1);
		}
	}

	pos=context->ivals[1]-il-stop;
	/*place the current position within the correct translation frame*/
	if (pos % 3){
		if (!((pos-1) % 3)){
			pos=(pos-1)/3;
			decal=-1;
		}
		else if (!((pos+1) % 3)){
			pos=(pos+1)/3;
			decal=1;
		}						
	}else {
		pos=pos/3;
		decal=0;
	}

	n=pos;
	stop_prot=stop-decal-1; /*-1 == middle of the codon*/
	cxChar_2=GrData->udv_font.cxChar/2;
	
	/*vertical pos: just below the CDS colour box (pos: y)*/
	y_prot=y+GrData->udv_font.LineHeight-GrData->udv_font.cyChar/2;
	/*NOTICE : I draw from right to left; whereas normally I draw always 
		from left to right*/			
	if (UseDefClr) SetColor(DefClr);
	
	while(stop_prot>=start){
		x_prot=((stop_prot-StartLetter)+
				((stop_prot-StartLetter)/LETTER_BLOCK_WIDTH))*
				GrData->udv_font.cxChar+xMargin-cxChar_2;
		if (!UseDefClr){
			if (str[n]>='A' && str[n]<='Z'){
				SetColor(GrData->AA_LayoutPal[str[n]-'A'].LetClr);
			}
			else{
				Black();
			}
		}
		MoveTo(x_prot,y_prot);
		PaintChar(str[n]);
		stop_prot-=3;
		n++;
	}

	if (str){
		MemFree (str);
	}
	Black();
}


/*******************************************************************************

  Function : UDV_draw_CDS_plus()
  
  Purpose : draw the translation of a CDS located on a PLUS strand
  
  Parameters : context; feature data (see explore.h for a def of the structure)
  				start; draw the sequence from...
				stop;... to.
				StartLetter; beginning of a ParaG (zero-based value)
				GrData; graphical data
				y; vertical position to draw the sequence
				i; order number of the exon (if CDS is exon-encoded)
				xMargin; draw in the ParaG on the right a this value
				UseDefClr; TRUE: draw the sequence using the DefClr value
					Otherwise use GrData->AA_LayoutPal[] values
  
  Return value : none

*******************************************************************************/
static void  UDV_draw_CDS_plus(SeqMgrFeatContextPtr context,Int4 start,
			Int4 stop,Int4 StartLetter,UnDViewerGraphDataPtr GrData,Int2 y,
			Int2 i,Int2 xMargin,Boolean UseDefClr,Uint4 DefClr)
{						
CharPtr      	str=NULL;
Int4			il=0,
				pos=0,
				n=0,
				start_prot=0;
Int2 			n2=0,
				nCompt=0,
				decal=0,
				x_prot=0,
				y_prot=0,
				numivals2=0;
ByteStorePtr	bs=NULL;
Char szBuf[SZBUF_SIZE]={""};
Boolean bGiForProductOk=FALSE;
SeqIdPtr sip=NULL;
Int4 gi=0;
BioseqPtr prot;

	/*retrieve the protein sequence; need to be optimized in future release*/
	if (context->sfp->product){
		prot=BioseqFind(SeqLocId(context->sfp->product));
		sip=(SeqIdPtr)GetProductSeqId(context->sfp->product);
		if (sip && prot){
			CharPtr str2;
			gi = GetGINumFromSip(sip);
			if (gi>0) bGiForProductOk=TRUE;
			str2=UDV_Read_Sequence (sip,0,prot->length-1, 
				TRUE,prot->length-1);
			if (!str2) return;
			str=str2;
		}
	}
	else{
		bs = ProteinFromCdRegion (context->sfp, TRUE);
		if (bs){
			str = (CharPtr) BSMerge (bs, NULL);
			BSFree (bs);
		}
	}

	if (!str) return;	
	MemSet(szBuf,' ',SZBUF_SIZE-1);	
	
	/*count the lenght of the intron(s)*/
	numivals2=context->numivals*2;
	if (i>0 && i<numivals2){
		for(nCompt=2;nCompt<i+2;nCompt+=2){
			il+=(context->ivals[nCompt]-
				context->ivals[nCompt-1]-1);
		}
	}

	pos=start-il-context->ivals[0];
						
	/*place the current position within the correct translation frame*/
	if (pos % 3){
		if (!((pos-1) % 3)){
			pos=(pos-1)/3;
			decal=-1;
		}
		else if (!((pos+1) % 3)){
			pos=(pos+1)/3;
			decal=1;
		}						
	}else {
		pos=pos/3;
		decal=0;
	}

	n=pos;
	start_prot=start+decal+1;

	/*vertical pos: just below the CDS colour box (pos: y)*/
	y_prot=y+GrData->udv_font.LineHeight-GrData->udv_font.cyChar/2;
	x_prot=((start_prot-StartLetter)+
			((start_prot-StartLetter)/LETTER_BLOCK_WIDTH))*
			GrData->udv_font.cxChar+xMargin-GrData->udv_font.cxChar/2;

	if (UseDefClr) SetColor(DefClr);
	nCompt=start_prot-StartLetter+(start_prot-StartLetter)/LETTER_BLOCK_WIDTH;	
	while(start_prot<=stop){
		n2=start_prot-StartLetter+(start_prot-StartLetter)/LETTER_BLOCK_WIDTH
			-nCompt;
		if (n2<SZBUF_SIZE-1){
			szBuf[n2]=str[n];
		}
		start_prot+=3;
		n++;
	}

	MoveTo(x_prot,y_prot);
	PaintString(szBuf);
	if (str){
		MemFree (str);
	}
	Black();
}


/*******************************************************************************

  Function : UDV_draw_big_arrow_feat()
  
  Purpose : draw a single feature as an arrow
  
  Parameters :  start_x; draw from...
  				stop_x; ...to
				y: vertical position
				cxChar; width of a character
				cyChar; height of a character
				LineHeight; height of a line in a ParaG
				start_nat; start is a box, arrow or nothing ?
 				stop_nat; stop is a box, arrow or nothing ?
				clr; colour 

  Return value : none

*******************************************************************************/
static void  UDV_draw_big_arrow_feat(Int2 start_x,Int2 y,
			Int2 cxChar,Int2 cyChar,Uint4 clr)
{
PoinT arrow[3];

	if (clr!=(Uint4)-1) SetColor(clr);
	else Black();
	y-=3;
	arrow[0].x=start_x-cxChar/2;
	arrow[0].y=y;
	arrow[1].x=start_x+cxChar/2;
	arrow[1].y=y;
	arrow[2].x=start_x;
	arrow[2].y=y-(2*cyChar)/3;

	PaintPoly(3,arrow);
	Black();
}

/*******************************************************************************

  Function : UDV_draw_big_arrow_bond()
  
  Purpose : draw a bond feature as an arrow with a connected line
  
  Parameters :  start_x; draw from...
  				stop_x; ...to
				y: vertical position
				cxChar; width of a character
				cyChar; height of a character
				LineHeight; height of a line in a ParaG
				start_nat; start is a box, arrow or nothing ?
 				stop_nat; stop is a box, arrow or nothing ?
				clr; colour 

  Return value : none

*******************************************************************************/
static void  UDV_draw_big_arrow_bond(Int2 start_x,Int2 y,
			Int2 cxChar,Int2 cyChar,Uint4 clr,Int1 What)
{
PoinT arrow[3];

	if (clr!=(Uint4)-1) SetColor(clr);
	else Black();
	/*y-=3;*/
	
	arrow[0].x=start_x;
	arrow[0].y=y;
	if (What==BOND_LEFT) arrow[1].x=start_x+cxChar/2;
	else arrow[1].x=start_x-cxChar/2;
	arrow[1].y=y-cyChar/3;
	arrow[2].x=start_x;
	arrow[2].y=y-(2*cyChar)/3;
	PaintPoly(3,arrow);
	Black();
}


/*******************************************************************************

  Function : UDV_draw_big_line_feat()
  
  Purpose : draw a single feature as a box
  
  Parameters :  start_x; draw from...
  				stop_x; ...to
				y: vertical position
				cxChar; width of a character
				cyChar; height of a character
				LineHeight; height of a line in a ParaG
				start_nat; start is a box, arrow or nothing ?
 				stop_nat; stop is a box, arrow or nothing ?
				clr; colour 
 
  Return value : none

*******************************************************************************/
static void  UDV_draw_big_line_feat(Int2 start_x,Int2 stop_x,Int2 y,
			Int2 cxChar,Int2 LineHeight,Int2 start_nat,Int2 stop_nat,
			Uint4 clr)
{
RecT rcLine,rcBox;
Int2 y2,x2,l2,l3,l6;
PoinT arrow[3];

	y2=y-LineHeight/2;
	l2=LineHeight/2;
	l3=LineHeight/3;
	l6=LineHeight/6;
	
	if (clr!=(Uint4)-1) SetColor(clr);
	else Black();
	
	/*start end : box, arrow or nothing*/
	if (start_nat==FEATURE_START_BOX){
		x2=start_x;
		rcBox.left=x2-2;	
  		rcBox.top=y2-l3;	
		rcBox.right=x2+2;	
		rcBox.bottom=y2+l3;
		start_x=rcBox.right;	
		PaintRect(&rcBox);
	}
	else if (start_nat==FEATURE_START_ARROW){
		arrow[0].x=start_x;
		arrow[0].y=y2;
		arrow[1].x=start_x+cxChar;
		arrow[1].y=y2-l2;
		arrow[2].x=arrow[1].x;
		arrow[2].y=y2+l2;
		start_x=arrow[2].x;
		PaintPoly(3,arrow);
	}
	else if (start_nat==FEATURE_START_ARROW_END){
		arrow[0].x=start_x;
		arrow[0].y=y2;
		arrow[1].x=start_x+cxChar;
		arrow[1].y=y2-l2;
		arrow[2].x=arrow[1].x;
		arrow[2].y=y2+l2;
		start_x=arrow[2].x;
		PaintPoly(3,arrow);
		MoveTo(arrow[0].x,arrow[1].y);
		LineTo(arrow[0].x,arrow[2].y);
	}
	
	/*stop end : box, arrow or nothing*/
	if (stop_nat==FEATURE_START_BOX){
		x2=stop_x;
		rcBox.left=stop_x-2;	
		rcBox.top=y2-l3;	
		rcBox.right=x2+2;	
		rcBox.bottom=y2+l3;
		stop_x=rcBox.left;	
		PaintRect(&rcBox);
	}
	else if (stop_nat==FEATURE_START_ARROW){
		arrow[0].x=stop_x;
		arrow[0].y=y2;
		arrow[1].x=stop_x-cxChar;
		arrow[1].y=y2-l2;
		arrow[2].x=arrow[1].x;
		arrow[2].y=y2+l2;
		stop_x=arrow[2].x;
		PaintPoly(3,arrow);
	}
	else if (stop_nat==FEATURE_START_ARROW_END){
		arrow[0].x=stop_x;
		arrow[0].y=y2;
		arrow[1].x=stop_x-cxChar;
		arrow[1].y=y2-l2;
		arrow[2].x=arrow[1].x;
		arrow[2].y=y2+l2;
		stop_x=arrow[2].x;
		PaintPoly(3,arrow);
		MoveTo(arrow[0].x,arrow[1].y);
		LineTo(arrow[0].x,arrow[2].y);
	}

	if (start_x>=stop_x) return;
	rcLine.left=start_x;
	rcLine.top=y2-_max_(2,l6);
	rcLine.right=stop_x;
	rcLine.bottom=y2+_max_(2,l6);
	PaintRect(&rcLine);
	Black();	
}

/*******************************************************************************

  Function : UDV_draw_thin_line()
  
  Purpose : draw a thin black line (generally used to delineate the introns)
  
  Parameters : 	start_x; draw from...
  				stop_x; ...to
				y: vertical position
				LineHeight; height of a line in a ParaG
  
  Return value : none

*******************************************************************************/
static void  UDV_draw_thin_line(Int2 start_x,Int2 stop_x,Int2 y,
					Int2 LineHeight,Uint4 clr)
{
Int2 y2;

	y2=y-LineHeight/2;
	
	if (clr!=(Uint4)-1) SetColor(clr);
	else LtGray();
	
	MoveTo(start_x,y2);
	LineTo(stop_x,y2);

	Black();
}

/*******************************************************************************

  Function : UDV_Draw_features()
  
  Purpose : I thing the name of the function is meaningfull...
  
  Parameters : 	GrData; graphical data (font size, etc)
  				bsp_i ; general data of the Bioseq
				UseDefClr; TRUE: draw the sequence using the DefClr value
					Otherwise use GrData->AA_LayoutPal[] values
				pgp; data of the ParaG
				rc; rectangle containing this ParaG
  				pClr; colour table [FEATDEF_MAX size]
				is; item to select, if needed
				old_is; old item to deselect, if needed

  Note : don't try to understand this function alone... if you want to keep
         a good health !				

  Return value : none

*******************************************************************************/
NLM_EXTERN void  UDV_Draw_features(UnDViewerGraphDataPtr GrData,
					BspInfoPtr bsp_i,Boolean UseDefClr,Uint4 DefClr,
					ParaGPtr pgp,RecT PNTR rc,Uint4Ptr pClr,
					UDV_Item_Select * is,UDV_Item_Select * old_is,
					Uint4 DisplayType)
{
Int2 y,ybase;				/*text position*/
Int2 xMargin;				/*initial margins*/
Int2 nTicks=0,nLet=0;		/*y decal*/
ValNodePtr vnp;				/*Features list of itemID and index values*/
Uint2 iID,idx,lineID;		/*used to retrieve a desired Feature*/
/*Uint8 index_g;			merged value containing itemID, index and lineID*/
SeqMgrFeatContextPtr context;	/*used to retrieve feature data*/
SeqMgrFeatContext context2;	/*used to retrieve feature data*/
Int4 start,stop/*,OccupyTo*/;	/*limit of the feature to draw*/
Int2 start_x,stop_x,stop_x2,		/*id. but in pixels*/
	start_nat,stop_nat;		/*ends of the feature : box, arrow*/
Int2 i,numivals2,i_decal,j;	/*counters*/
Uint4	clr;				/*color used to draw feature*/		
Boolean b_draw_line,bCDS,b_connect_left,b_connect_right,
		b_end_left,b_end_right,bDraw;
Int2 idx1,idx2,idx3,idx4,idx5,idx6,nLines=0;
BioseqPtr parent;
SeqMgrSegmentContext contextPart;

	if (!pgp->pFeatList) return;
	
	/*compute position*/
	if (GrData->udv_scale.ShowMajorTick) nTicks++;
	if (GrData->udv_scale.ScalePosition==SCALE_POS_TOP || 
					GrData->udv_scale.ScalePosition==SCALE_POS_BOTH) nLet++;
	
	xMargin=rc->left+GrData->udv_font.cxChar;
	ybase=rc->top+(nTicks+nLet+1/*2*/)*GrData->udv_font.LineHeight;
	/*OccupyTo=pgp->StopLetter;*/

	/*the current bsp is just a segment ?*/
	parent=SeqMgrGetParentOfPart(bsp_i->bsp,&contextPart);
	context=NULL;

	/*draw : loop on all features in a ParaG*/
	for(j=0,vnp=pgp->pFeatList;j<pgp->nFeat;j++,vnp=vnp->next){
		if (vnp == NULL) break;
		/*index_g=(Uint8)vnp->data.bigintvalue;*/
		UDV_BigDecodeIdxFeat ((Uint8)vnp->data.bigintvalue, &iID,&idx,&lineID,NULL);
		/*get desired feature given iID and idx*/
		if (!SeqMgrGetDesiredFeature(bsp_i->bsp_entityID,
				(parent!=NULL ? parent : bsp_i->bsp),
                iID,idx,NULL,&context2)) continue;


		if (context) {
			MemFree(context->ivals);
			context=(SeqMgrFeatContextPtr)MemFree(context);
		}
		context=UDV_ConvertFeatContext(&context2,contextPart.cumOffset,
			bsp_i->bsp->length);
		if (!context) continue;
		/*depending on the strand, various things are possible :*/
			/*PLUS/MINUS : if region (start!=stop)-> draw big box, with arrow*/

			/*UNKNOWN : if region (start!=stop)-> draw big box, without arrow*/

			/*PLUS/MINUS/UNKNOWN : if single letter (start==stop)-> draw 
			vertical arrow*/
		
		/*HET feature correction for the ends*/
		if(context->featdeftype== FEATDEF_HET){
			context->right=context->ivals[2*context->numivals-1];
		}

		/*temporary situation; will be modified in the future*/
		if (context->strand>Seq_strand_minus ||
			context->strand==Seq_strand_unknown) context->strand=Seq_strand_plus;

		/*strand PLUS*/
		if (context->strand==Seq_strand_plus){
			numivals2=context->numivals*2;
			i=0;
			i_decal=2;
		}
		
		/*strand MINUS*/
		if (context->strand==Seq_strand_minus){
			numivals2=2*context->numivals-2;
			i=numivals2;
			i_decal=-2;
		}		

		bCDS=FALSE;
		/*if (_max_(context->ivals[i],pgp->StartLetter)<=OccupyTo){
			nLines++;
		}*/
		nLines=lineID;
		bDraw=FALSE;
		while (TRUE){
			b_connect_left=FALSE;
			b_connect_right=FALSE;
			b_end_left=FALSE;
			b_end_right=FALSE;
			b_draw_line=FALSE;

			/*colour*/
			if (pClr){
				clr=pClr[context->featdeftype];
			}
			else clr=(Uint4)-1;

			/*if ivals.stop > end ParaG -> end of drawing*/
			if (context->ivals[i]>pgp->StopLetter) break;
			/*if ivals.stop<= start ParaG : not yet in the current ParaG*/
			
			if (context->ivals[i+1]<pgp->StartLetter) {
				if (context->strand==Seq_strand_plus || 
						context->strand==Seq_strand_unknown){
					if (numivals2>2 && i+2<numivals2){
					/*stop ParaG < start next ivals -> inter-region: fill 
					the ParaG with a thin line; this is the case
					for coding region: draw thin line to delineate the introns*/		
						if (context->ivals[i+2]>pgp->StopLetter){
							b_draw_line=TRUE;
						}
					}
				}
				if (context->strand==Seq_strand_minus){
					if (numivals2>2 && i-2>-1){
					/*stop ParaG < start next ivals -> inter-region: fill 
					the ParaG with a thin line; this is the case
					for coding region: draw thin line to delineate the introns*/		
						if (context->ivals[i-2]>pgp->StopLetter){
							b_draw_line=TRUE;
						}
					}
				}
				if (b_draw_line){
					start=0;
					/*nLines++;*/
					stop=pgp->StopLetter-pgp->StartLetter;
					start_x=start*GrData->udv_font.cxChar+xMargin;
					stop_x=(stop+(stop/LETTER_BLOCK_WIDTH))*
						GrData->udv_font.cxChar+xMargin;
					y=ybase+nLines*GrData->udv_font.LineHeight;
					UDV_draw_thin_line(start_x,stop_x,y,
							GrData->udv_font.LineHeight,clr);
					/*OccupyTo=pgp->StopLetter;*/
					bDraw=TRUE;
				}
				if (context->strand==Seq_strand_plus || 
						context->strand==Seq_strand_unknown){
					i=i+i_decal;
					if (i>numivals2-2) break;
					else continue;
				}					
				if (context->strand==Seq_strand_minus){
					i=i+i_decal;
					if (i<0) break;
					else continue;
				}
			}
						
			/*compute the limits of the feature within ParaG*/
			start=_max_(context->ivals[i],pgp->StartLetter);
			stop=_min_(context->ivals[i+1],pgp->StopLetter);
			bDraw=TRUE;
			/*are there connections; exons/introns for example*/
				/*on the left*/
			if (context->strand==Seq_strand_plus || 
						context->strand==Seq_strand_unknown){
				if (numivals2>2 && i>0){
					if (start>pgp->StartLetter){
						idx1=i-1;
						idx2=1;
						idx3=-2;
						b_connect_left=TRUE;
						b_end_left=TRUE;
					}
					else if(start==pgp->StartLetter){
						b_end_left=TRUE;
					}
				}
			}
			if (context->strand==Seq_strand_minus){
				if (numivals2>2 && i<numivals2-1){
					if (start>pgp->StartLetter){
						idx1=i+3;
						idx2=3;
						idx3=0;
						b_connect_left=TRUE;
						b_end_left=TRUE;
					}
					else if(start==pgp->StartLetter){
						b_end_left=TRUE;
					}
				}
			}
				/*on the right*/
			if (context->strand==Seq_strand_plus || 
						context->strand==Seq_strand_unknown){
				if (numivals2>2 && i+2<numivals2){
					if (stop<pgp->StopLetter){
						idx4=i+2;
						idx5=1;
						idx6=-2;
						b_connect_right=TRUE;
						b_end_right=TRUE;
					}
					else if(stop==pgp->StopLetter){
						b_end_right=TRUE;
					}
				}
			}
			if (context->strand==Seq_strand_minus){
				if (numivals2>2 && i>0){
					if (stop<pgp->StopLetter){
						idx4=i-2;
						idx5=3;
						idx6=0;
						b_connect_right=TRUE;
						b_end_right=TRUE;
					}
					else if(stop==pgp->StopLetter){
						b_end_right=TRUE;
					}
				}
			}
			/*compute the 'nature' of start & stop: box, arrow or nothing*/
			/*I use only Seq_strand_minus or Seq_strand_plus !!!!*/
			if (context->strand==Seq_strand_plus || 
				context->strand==Seq_strand_minus){
				if (context->ivals[i]==start){
					if (context->strand==Seq_strand_plus){ 
						if (b_end_left) start_nat=FEATURE_START_NOTHING;
						else start_nat=FEATURE_START_BOX;
					}
					else {
						if (b_end_left) start_nat=FEATURE_START_ARROW;
						else start_nat=FEATURE_START_ARROW_END;
					}
				}
				else{
					if (context->strand==Seq_strand_plus)
						start_nat=FEATURE_START_NOTHING;
					else
						start_nat=FEATURE_START_ARROW;
				}

				if (context->ivals[i+1]==stop){
					if (context->strand==Seq_strand_minus){ 
						if (b_end_right) stop_nat=FEATURE_START_NOTHING;
						else stop_nat=FEATURE_START_BOX;
					}
					else{
						if (b_end_right) stop_nat=FEATURE_START_ARROW;
						else stop_nat=FEATURE_START_ARROW_END;
					}
				}
				else{
					if (context->strand==Seq_strand_plus)
						stop_nat=FEATURE_START_ARROW;
					else
						stop_nat=FEATURE_START_NOTHING;
				}
			}
			else{
				start_nat=FEATURE_START_NOTHING;
				stop_nat=FEATURE_START_NOTHING;
			}

			/*compute limits of the drawing... zero based values from the left 
				of ParaG rc*/
			start_x=((start-pgp->StartLetter)+
				((start-pgp->StartLetter)/LETTER_BLOCK_WIDTH))*
				GrData->udv_font.cxChar+xMargin;
			stop_x=((stop-pgp->StartLetter)+((stop-pgp->StartLetter)/
				LETTER_BLOCK_WIDTH))*GrData->udv_font.cxChar+xMargin;
			stop_x2=stop_x;/*use below to display a label; wwwudv only*/
			
			y=ybase+nLines*GrData->udv_font.LineHeight;
			/*draw feature*/
			if (start!=stop || (start==stop && context->left!=context->right) 
				/*|| context->strand!=Seq_strand_unknown*/){
				switch(context->featdeftype){
					case FEATDEF_HET:
						if (clr!=(Uint4)-1) SetColor(clr);
						MoveTo(start_x-GrData->udv_font.cxChar/2,y);
						PaintChar('H');
						b_connect_left=FALSE;
						b_connect_right=FALSE;
						break;
					case FEATDEF_BOND:
						UDV_draw_thin_line(start_x,stop_x,y,
							GrData->udv_font.LineHeight,clr);
						if (pgp->StartLetter<=context->left && 
							pgp->StopLetter>=context->left){
							UDV_draw_big_arrow_bond(start_x,y,
								GrData->udv_font.cxChar,
								GrData->udv_font.cyChar,
								clr,BOND_LEFT);
						}
						if (pgp->StartLetter<=context->right && 
							pgp->StopLetter>=context->right){
							UDV_draw_big_arrow_bond(stop_x,y,
								GrData->udv_font.cxChar,
								GrData->udv_font.cyChar,
								clr,BOND_RIGHT);
						}
						break;
					case FEATDEF_PSEC_STR:
						if (context->sfp){
							Boolean ContinueLeft=FALSE;
							Boolean ContinueRight=FALSE;
							
							if (context->sfp->data.value.intvalue==1){/*helix*/
								if (start!=context->left) ContinueLeft=TRUE;
								if (stop!=context->right)ContinueRight=TRUE;
								UDV_draw_struc_helix( start, stop,
									pgp->StartLetter,
									y,GrData->udv_font.cxChar,
									GrData->udv_font.cyChar,
									GrData->udv_font.LineHeight,xMargin,clr,
									ContinueLeft,ContinueRight);
							}
							if (context->sfp->data.value.intvalue==2){/*sheet*/
								if (start!=context->left) ContinueLeft=TRUE;
								if (stop!=context->right)ContinueRight=TRUE;
								UDV_draw_struc_strand( start, stop,
									pgp->StartLetter,
									y,GrData->udv_font.cxChar,
									GrData->udv_font.cyChar,
									GrData->udv_font.LineHeight,xMargin,clr,
									ContinueLeft,ContinueRight);
							}
						}
						break;
					case FEATDEF_CDS:
						bCDS=TRUE;
						if (context->strand==Seq_strand_plus)
							UDV_draw_CDS_plus(context,start,stop,
								pgp->StartLetter,
								GrData,y,i,xMargin, UseDefClr, DefClr);						
						if (context->strand==Seq_strand_minus)
							UDV_draw_CDS_minus(context,start,stop,
								pgp->StartLetter,
								GrData,y,i,xMargin, UseDefClr, DefClr);
						/*nLines++;*/
						/*WARNING: do not add anything between FEATDEF_CDS 
						and the following default case because after the 
						translation, I must draw a box*/						
					default:
						UDV_draw_big_line_feat(start_x,stop_x,y,
							GrData->udv_font.cxChar,
							GrData->udv_font.LineHeight,
							start_nat,stop_nat,clr);
						break;
				}
			}
			else{/*feature is one letter long*/
				UDV_draw_big_arrow_feat(start_x,y,GrData->udv_font.cxChar,
					GrData->udv_font.cyChar,clr);
			}
			/*select feature, if needed*/
			if (old_is && old_is->eIDsel!=(Uint2)-1){
				if (old_is->iIDsel==iID){
					RecT rcSel;

					rcSel.left=start_x-5;
					rcSel.top=y-GrData->udv_font.LineHeight+1;
					rcSel.right=stop_x+5;
					if (!bCDS) rcSel.bottom=y;
					else rcSel.bottom=y+GrData->udv_font.LineHeight;
					White();
					UDV_draw_select(rcSel);
				}
			}
			if (is && is->eIDsel!=(Uint2)-1){
				if (is->iIDsel==iID){
					RecT rcSel;

					rcSel.left=start_x-5;
					rcSel.top=y-GrData->udv_font.LineHeight+1;
					rcSel.right=stop_x+5;
					if (!bCDS) rcSel.bottom=y;
					else rcSel.bottom=y+GrData->udv_font.LineHeight-2;
					Red();
					UDV_draw_select(rcSel);
				}
			}
			Black();

			/*draw thin line if needed; used to connect exons/introns for 
			example*/
			if (b_connect_left){
				Int4 start2,stop2;
				stop2=start;
				start2=_max_(pgp->StartLetter,context->ivals[idx1]);
				start_x=(start2-pgp->StartLetter+
					((start2-pgp->StartLetter)/LETTER_BLOCK_WIDTH))*
					GrData->udv_font.cxChar+xMargin;
				stop_x=(stop2-pgp->StartLetter+
					((stop2-pgp->StartLetter)/
					LETTER_BLOCK_WIDTH))*GrData->udv_font.cxChar+xMargin;
				UDV_draw_thin_line((Int2)(start_x+idx2),(Int2)(stop_x+idx3),y,
						GrData->udv_font.LineHeight,clr);
			}

			if (b_connect_right){
				Int4 start2,stop2;
				start2=stop;
				stop2=_min_(pgp->StopLetter,context->ivals[idx4]);
				start_x=(start2-pgp->StartLetter+
					((start2-pgp->StartLetter)/LETTER_BLOCK_WIDTH))*
					GrData->udv_font.cxChar+xMargin;
				stop_x=(stop2-pgp->StartLetter+
					((stop2-pgp->StartLetter)/
					LETTER_BLOCK_WIDTH))*GrData->udv_font.cxChar+xMargin;
				UDV_draw_thin_line((Int2)(start_x+idx5),(Int2)(stop_x+idx6),y,
					GrData->udv_font.LineHeight,clr);
				/*OccupyTo=stop2;*/
			}

			/*if (context->sfp && context->sfp->data.choice==SEQFEAT_CDREGION)
					y+=GrData->udv_font.LineHeight;		*/

			if (context->strand==Seq_strand_plus || 
						context->strand==Seq_strand_unknown){
				i=i+i_decal;
				if (i>numivals2-2) break;
			}
			if (context->strand==Seq_strand_minus){
				i=i+i_decal;
				if (i<0) break;
			}
		}
		if (bDraw && (DisplayType&DDV_DISP_LABEL)){
			if (pClr){
				SetColor(pClr[context->featdeftype]);
			}
			else Magenta();
			MoveTo(_max_(stop_x2,stop_x)+GrData->udv_font.cxChar,y);
			PaintString(context->label);		
			Black();
		}
	}
	Black();	
}

/*******************************************************************************

  Function : UDV_Draw_features_MAP()
  
  Purpose : create an image map
  
  Parameters : 	GrData; graphical data (font size, etc)
  				bsp_i ; general data of the Bioseq
				pgp; data of the ParaG
				rc; rectangle containing this ParaG
				vnpp_map; to put the image map info

  Note : this is a light version of UDV_Draw_features()				

  Return value : TRUE if success

*******************************************************************************/
NLM_EXTERN Boolean UDV_Draw_features_MAP(UnDViewerGraphDataPtr GrData,
					BspInfoPtr bsp_i,ParaGPtr pgp,RecT PNTR rc,Uint2 entityID,
					ValNodePtr PNTR vnpp_map)
{
BioseqPtr parent;
UDVMapInfoPtr umip;
SeqMgrSegmentContext contextPart;
SeqMgrFeatContextPtr context;	/*used to retrieve feature data*/
SeqMgrFeatContext context2;	/*used to retrieve feature data*/
ValNodePtr vnp,vnp_map;		/*Features list of itemID and index values*/
Int4 start,stop;			/*limit of the feature to draw*/
Int2 y,ybase;				/*text position*/
Int2 xMargin;				/*initial margins*/
Int2 nTicks=0,nLet=0;		/*y decal*/
Uint2 iID,idx,lineID;		/*used to retrieve a desired Feature*/
Int2 start_x,stop_x;		/*ends of the feature : box, arrow*/
Int2 i,numivals2,i_decal,j;	/*counters*/

	vnp_map=NULL;
	
	/*compute position*/
	if (GrData->udv_scale.ShowMajorTick) nTicks++;
	if (GrData->udv_scale.ScalePosition==SCALE_POS_TOP || 
					GrData->udv_scale.ScalePosition==SCALE_POS_BOTH) nLet++;
	
	xMargin=rc->left+GrData->udv_font.cxChar;
	ybase=rc->top+(nTicks+nLet+1)*GrData->udv_font.LineHeight;

	/*the current bsp is just a segment ?*/
	parent=SeqMgrGetParentOfPart(bsp_i->bsp,&contextPart);
	context=NULL;
	
	/*descriptor node for the sequence itself*/
	/*map rect*/
	umip=(UDVMapInfoPtr)MemNew(sizeof(UDVMapInfo));
	if (umip==NULL){
		if (vnp_map) ValNodeFreeData(vnp_map);
		return(FALSE);
	}
	umip->left=xMargin-5;
	umip->bottom=ybase;
	umip->top=umip->bottom-GrData->udv_font.LineHeight+1;
	umip->right=((pgp->StopLetter-pgp->StartLetter)+((pgp->StopLetter-pgp->StartLetter)/
				LETTER_BLOCK_WIDTH))*GrData->udv_font.cxChar+xMargin+5;
	umip->Type=OBJ_BIOSEQ;
	umip->data.uintvalue=(Uint4)entityID;
	ValNodeAddPointer(vnpp_map,0,(Pointer)umip);

	if (!pgp->pFeatList) return(TRUE);

	/*draw : loop on all features in a ParaG*/
	for(j=0,vnp=pgp->pFeatList;j<pgp->nFeat;j++,vnp=vnp->next){
		if (vnp == NULL) break;
		/*index_g=(Uint8)vnp->data.bigintvalue;*/
		UDV_BigDecodeIdxFeat ((Uint8)vnp->data.bigintvalue, &iID,&idx,&lineID,NULL);
		/*get desired feature given iID and idx*/
		if (!SeqMgrGetDesiredFeature(bsp_i->bsp_entityID,
				(parent!=NULL ? parent : bsp_i->bsp),
                iID,idx,NULL,&context2)) continue;


		if (context) {
			MemFree(context->ivals);
			context=(SeqMgrFeatContextPtr)MemFree(context);
		}
		context=UDV_ConvertFeatContext(&context2,contextPart.cumOffset,
			bsp_i->bsp->length);
		if (!context) continue;
		/*depending on the strand, various things are possible :*/
			/*PLUS/MINUS : if region (start!=stop)-> draw big box, with arrow*/

			/*UNKNOWN : if region (start!=stop)-> draw big box, without arrow*/

			/*PLUS/MINUS/UNKNOWN : if single letter (start==stop)-> draw 
			vertical arrow*/
		
		/*HET feature correction for the ends*/
		if(context->featdeftype== FEATDEF_HET){
			context->right=context->ivals[2*context->numivals-1];
		}

		/*temporary situation; will be modified in the future*/
		if (context->strand>Seq_strand_minus ||
			context->strand==Seq_strand_unknown) context->strand=Seq_strand_plus;

		/*strand PLUS*/
		if (context->strand==Seq_strand_plus){
			numivals2=context->numivals*2;
			i=0;
			i_decal=2;
		}
		
		/*strand MINUS*/
		if (context->strand==Seq_strand_minus){
			numivals2=2*context->numivals-2;
			i=numivals2;
			i_decal=-2;
		}		

		while (TRUE){
			/*if ivals.stop > end ParaG -> end */
			if (context->ivals[i]>pgp->StopLetter) break;
			/*skip if not yet in the ParaG*/
			if (context->ivals[i+1]<pgp->StartLetter) {
				if (context->strand==Seq_strand_plus || 
						context->strand==Seq_strand_unknown){
					i=i+i_decal;
					if (i>numivals2-2) break;
					else continue;
				}					
				if (context->strand==Seq_strand_minus){
					i=i+i_decal;
					if (i<0) break;
					else continue;
				}
			}									
			/*compute the limits of the feature within ParaG*/
			start=_max_(context->ivals[i],pgp->StartLetter);
			stop=_min_(context->ivals[i+1],pgp->StopLetter);

			/*compute limits of the drawing... zero based values from the left 
				of ParaG rc*/
			start_x=((start-pgp->StartLetter)+
				((start-pgp->StartLetter)/LETTER_BLOCK_WIDTH))*
				GrData->udv_font.cxChar+xMargin;
			stop_x=((stop-pgp->StartLetter)+((stop-pgp->StartLetter)/
				LETTER_BLOCK_WIDTH))*GrData->udv_font.cxChar+xMargin;
			
			y=ybase+lineID*GrData->udv_font.LineHeight;

			/*map rect*/
			umip=(UDVMapInfoPtr)MemNew(sizeof(UDVMapInfo));
			if (umip==NULL){
				if (vnp_map) ValNodeFreeData(vnp_map);
				return(FALSE);
			}
			umip->left=start_x-5;
			umip->top=y-GrData->udv_font.LineHeight+1;
			umip->right=stop_x+5;
			if (context->featdeftype==FEATDEF_CDS) 
				umip->bottom=y+GrData->udv_font.LineHeight;
			else 
				umip->bottom=y;
			
			/*map nodes list*/
			umip->Type=OBJ_SEQFEAT;
			umip->data.uintvalue=UDV_EncodeIdxFeat(iID,idx);
			ValNodeAddPointer(vnpp_map,0,(Pointer)umip);

			if (context->strand==Seq_strand_plus || 
						context->strand==Seq_strand_unknown){
				i=i+i_decal;
				if (i>numivals2-2) break;
			}
			if (context->strand==Seq_strand_minus){
				i=i+i_decal;
				if (i<0) break;
			}
		}
	}
	return(TRUE);
}

/*******************************************************************************

  Function : UDV_Draw_features_label()
  
  Purpose : draw the labels of each feature located in a ParaG
  
  Parameters :	GrData; graphical data (font size, etc)
  				bsp_i ; general data of the Bioseq
				pgp; data of the ParaG
				rc; rectangle containing this ParaG
  				pClr; colour table [FEATDEF_MAX size]
				is; item to select, if needed
				old_is; old item to select, if needed

  Return value : none

*******************************************************************************/
#ifndef WWW_UDV
static void  UDV_Draw_features_label(UnDViewerGraphDataPtr GrData,
					BspInfoPtr bsp_i,ParaGPtr pgp,RecT PNTR rc,Uint4Ptr pClr,
					UDV_Item_Select is,UDV_Item_Select old_is)
{
Int2 x,y;					/*text position*/
Int2 xMargin;				/*initial margins*/
Int2 nTicks=0,nLet=0;		/*y decal*/
Char szBuf[51]={""};		/*name to draw*/
ValNodePtr vnp;				/*Features list of itemID and index values*/
Uint2 iID,idx;				/*used to retrieve a desired Feature*/
/*Uint4 index_g;			merged value containing itemID and index*/
SeqMgrFeatContext context;	/*used to retrieve feature data*/
Uint2 j;					/*counter*/

	if (!pgp->pFeatList) return;
	
	/*compute position*/
	if (GrData->udv_scale.ShowMajorTick) nTicks++;
	if (GrData->udv_scale.ScalePosition==SCALE_POS_TOP || 
					GrData->udv_scale.ScalePosition==SCALE_POS_BOTH) nLet++;
	
	xMargin=rc->left-2*GrData->udv_font.cxChar-GrData->udv_scale.cxLeftScale;
	y=rc->top+(nTicks+nLet+2)*GrData->udv_font.LineHeight;
	
	/*draw : loop on all features in a ParaG*/
	for(j=0,vnp=pgp->pFeatList;j<pgp->nFeat;j++,vnp=vnp->next){
		if (vnp == NULL) break;
		
		UDV_BigDecodeIdxFeat ((Uint8)vnp->data.bigintvalue, &iID,&idx,NULL,NULL);

		/*get desired feature given iID and idx*/
		if (!SeqMgrGetDesiredFeature(bsp_i->bsp_entityID,bsp_i->bsp,
                                iID,idx,NULL,&context)) continue;

		if (context.sfp){
			/*copy name*/
			if (FeatDefLabel (context.sfp, szBuf, sizeof (szBuf) - 1, 
						OM_LABEL_BOTH)){

				/*draw name*/
				x=xMargin-StringWidth(szBuf);
				MoveTo(x,y);
				if (pClr){
					SetColor(pClr[context.featdeftype]);
				}
				PaintString (szBuf);
				/*select if needed*/
				if (old_is.eIDsel!=(Uint2)-1){
					if (old_is.iIDsel==iID){
						RecT rcSel;
						
						rcSel.left=x-3;
						rcSel.top=y-GrData->udv_font.cyChar+1;
						rcSel.right=rcSel.left+StringWidth(szBuf)+4;
						rcSel.bottom=y+2;
						White();
						UDV_draw_select(rcSel);
					}
				}
				if (is.eIDsel!=(Uint2)-1){
					if (is.iIDsel==iID){
						RecT rcSel;
						
						rcSel.left=x-3;
						rcSel.top=y-GrData->udv_font.cyChar+1;
						rcSel.right=rcSel.left+StringWidth(szBuf)+4;
						rcSel.bottom=y+2;
						Red();
						UDV_draw_select(rcSel);
						
					}
				}
				Black();
				y+=GrData->udv_font.LineHeight;
				
			}
			if (context.sfp->data.choice==SEQFEAT_CDREGION){
				if (UDV_IsTranslationNeeded(&context,pgp)) 
					y+=GrData->udv_font.LineHeight;
			}
		}
	}
	Black();	
}
#endif /*WWW_UDV*/


/*******************************************************************************

  Function : UDV_Draw_sequence_name()
  
  Purpose : draw the name of the sequence (on the left of it) of a ParaG
  
  Parameters : GrData; graphical data (font size, etc)
  				bsp_i ; general data of the Bioseq
				rc; rectangle containing this ParaG
  
  Return value : none

*******************************************************************************/
#ifndef WWW_UDV
static void  UDV_Draw_sequence_name(UnDViewerGraphDataPtr GrData,
					BspInfoPtr bsp_i,RecT PNTR rc)
{
Int2 x,y,hdecal;			/*text position*/
Int2 nTicks=0,nLet=0;	/*y decal*/

	/*compute position*/
	if (GrData->udv_scale.ShowMajorTick) nTicks++;
	if (GrData->udv_scale.ScalePosition==SCALE_POS_TOP || 
					GrData->udv_scale.ScalePosition==SCALE_POS_BOTH) nLet++;
	
	hdecal=nTicks+nLet+1;
	
	/*draw name*/
	x=rc->left-2*GrData->udv_font.cxChar-GrData->udv_scale.cxLeftScale-
				StringWidth(bsp_i->bspAccNum);
	y=rc->top+hdecal*GrData->udv_font.LineHeight;
	MoveTo(x,y);
	PaintString (bsp_i->bspAccNum);
}
#endif /*WWW_UDV*/


/*******************************************************************************

  Function : UDV_Draw_sequence()
  
  Purpose : draw the Bioseq' sequence of a ParaG
  
  Parameters : 	GrData; graphical data (font size, etc)
				vnp_color; colours table
  				bsp_i ; general data of the Bioseq
				IsNuc;True if nuc sequence
				UseDefClr; TRUE: draw the sequence using the DefClr value
					Otherwise use GrData->AA_LayoutPal[] values
				pgp; data of the ParaG
				rc; rectangle containing this ParaG
			    start_decal; buffer start letter (0-based)
				StartLetter; ParaG start letter (0-based)
				szSequence; sequence buffer
                Row; the row to draw if we are drawing the row of a seqalign
				
  Return value : none

*******************************************************************************/
NLM_EXTERN void  UDV_Draw_sequence(UnDViewerGraphDataPtr GrData,
                    DDV_ColorGlobal *svpp ,Boolean UseDefClr,Uint4 DefClr,
					ParaGPtr pgp,RecT PNTR rc,Int4 start_decal,Int4 StartLetter,
					CharPtr szSequence, SeqId *sip,ValNodePtr vnp_bsp,
					Boolean bSelect, Int4 Row,Int4 bspLength,Boolean bDisplayRevComp)
{
Int4 pos,taille,stop,bsp_pos,	/*scale start at...(used to draw text)*/
	 bsp_pos_goto;
Int2 x,y,ldecal,hdecal;	/*text position*/
Int2 xMargin;			/*initial margins*/
Int2 nTicks=0,nLet=0;	/*y decal*/
Uint1 residue;			/*letter and sequence length to retrieve*/
Uint2 nCompt=0,nCompt2=0;
CharPtr szBuf;
DDV_ColorCell  *rgb,*highlighClr;
Uint4 blackColor = GetColorRGB(0,0,0),newColor,curColor,hlClr;
RecT  rcSel;

    if(Row >= 0) sip = NULL;  /* draw by row instead of by bioseq */

	/*alloc a buffer*/
	taille=pgp->StopLetter-pgp->StartLetter+
		(pgp->StopLetter-pgp->StartLetter)/LETTER_BLOCK_WIDTH+5;
	szBuf=(CharPtr)MemNew(taille*sizeof(Char));
	if (!szBuf) return;
	MemSet(szBuf,0,taille*sizeof(Char));
	
	/*scale or not... to be or not to be ?*/
	if (GrData->udv_scale.ShowMajorTick) nTicks++;
	if (GrData->udv_scale.ScalePosition==SCALE_POS_TOP || 
					GrData->udv_scale.ScalePosition==SCALE_POS_BOTH) nLet++;
	
	pos=pgp->StartLetter+1;	/*remember : StartLetter is zero based*/
	xMargin=rc->left+GrData->udv_font.cxChar;
	y=rc->top;
	/*xDecal=xMargin;*/
	ldecal=GrData->udv_font.cxChar/2;
	hdecal=nTicks+nLet+1;

	if (UseDefClr) SetColor(DefClr);
	newColor=(Uint4)-1;
	stop=pgp->StopLetter+2;
	y=y+hdecal*GrData->udv_font.LineHeight;
	x=xMargin-ldecal;

		
	bsp_pos_goto=GrData->GotoLetter-1;
	bsp_pos=start_decal+(StartLetter-start_decal);

#ifndef WWW_UDV
	/* -A- Draw the selection; this code is not used by Entrez cgi-bin viewer*/
	if (bSelect){
		if (svpp==NULL){
			Yellow();
		}
		else{
	    	highlighClr =
    	    	DDV_SearchColorCellbyName(svpp->pvnSpecialColors, "Highlight");
			if (highlighClr)
				hlClr = GetColorRGB (highlighClr->rgb[0], highlighClr->rgb[1],highlighClr->rgb[2]);
			else 
				hlClr = GetColorRGB (255,255,0);
			SetColor(hlClr);
		}
		while(pos<stop){
			if(UDV_IsLetterSelected(vnp_bsp,bsp_pos,bspLength,bDisplayRevComp)&& 
					bsp_pos!=bsp_pos_goto ){
				rcSel.left=x+((bsp_pos-pgp->StartLetter)+
					(bsp_pos-pgp->StartLetter)/LETTER_BLOCK_WIDTH)*
					GrData->udv_font.cxChar-1;
				rcSel.top=y-GrData->udv_font.LineHeight;
				rcSel.bottom=y;
				rcSel.right=rcSel.left+GrData->udv_font.cxChar;
				PaintRect(&rcSel);
			}
			bsp_pos++;
			pos++;
		}
	} 
	
	
#endif

	pos=pgp->StartLetter+1;	
	/* -B- Draw the highlight  GoTo letter*/
	if (bsp_pos_goto>=pgp->StartLetter && bsp_pos_goto<=pgp->StopLetter){
		rcSel.left=x+((bsp_pos_goto-pgp->StartLetter)+
			(bsp_pos_goto-pgp->StartLetter)/LETTER_BLOCK_WIDTH)*
			GrData->udv_font.cxChar-1;
		rcSel.top=y-GrData->udv_font.LineHeight;
		rcSel.bottom=y;
		rcSel.right=rcSel.left+GrData->udv_font.cxChar;
		Red();
		PaintRect(&rcSel);
	}

	/* -C- Draw the letters*/
	if (svpp==NULL){/*means we don't use the Color Manager*/
		while(pos<stop){
			residue=(Uint1)szSequence[StartLetter-start_decal+(nCompt++)];
			szBuf[nCompt2++]=residue;

			/*each LETTER_BLOCK_WIDTH, add a blank column*/
			if (!(pos % LETTER_BLOCK_WIDTH)) {
				szBuf[nCompt2]=' ';
				nCompt2++;
			}
			pos++;		
		}
		Black();
		MoveTo(x,y);
		PaintString(szBuf);
	}
	else{ /*use The Color Manager*/
		while(pos<stop){
			/*retrieve colour*/
			bsp_pos=start_decal+(StartLetter-start_decal)+nCompt;
			rgb=DDV_GetColor(svpp, sip, Row, bsp_pos);
			if(rgb != NULL ){
			   curColor = GetColorRGB (rgb->rgb[0], rgb->rgb[1],rgb->rgb[2]);
			}
			else curColor=blackColor;
			/*new colour?*/
			if (curColor!=newColor){
				if (szBuf[0]!='\0'){/*something to draw ?*/
					szBuf[nCompt2]='\0';/*CLOSE THE STRING*/
					MoveTo(x,y);
					SetColor(newColor);
					PaintString(szBuf);
				}
				newColor=curColor;
				x+=(nCompt2*GrData->udv_font.cxChar);
				nCompt2=0;
				szBuf[nCompt2]='\0';
			}
			residue=(Uint1)szSequence[StartLetter-start_decal+(nCompt++)];
			szBuf[nCompt2++]=residue;
			
			/*each LETTER_BLOCK_WIDTH, add a blank column*/
			if (!(pos % LETTER_BLOCK_WIDTH)) {
				szBuf[nCompt2]=' ';
				nCompt2++;
			}
			pos++;		
		}
		if (szBuf[0]!='\0'){/*something to draw ?*/
			szBuf[nCompt2]='\0';/*CLOSE THE STRING*/
			MoveTo(x,y);
			SetColor(newColor);
			PaintString(szBuf);
		}
	}
	Black();
	MemFree(szBuf);
}

/*******************************************************************************

  Function : UDV_Draw_scale()
  
  Purpose : draw the numerical scale of a ParaG
  
  Parameters : 	GrData; graphical data (font size, etc)
				ShowMajorTick;TRUE = show major ticks ( | )
				ShowMMinorTick;TRUE = show minor ticks ( . )
				ScalePosition; left, top, ...
				StartLetter; start scale at...
				StopLetter; ...and stop at
				rc; rectangle containing this ParaG
				LeftDecal;adjust position on the left, if needed
				ScaleMaxVal;  scale length
				UseBlockDisp; use the 10 by 10 letters block display
				
  Return value : none

*******************************************************************************/
NLM_EXTERN void UDV_Draw_scale(UnDViewerGraphDataPtr GrData,
		Boolean ShowMajorTick,Boolean ShowMMinorTick,Uint1 ScalePosition,
		Int4 StartLetter,Int4 StopLetter,RecT PNTR rc,Int2 LeftDecal,
		Int4 ScaleMaxVal,Int4 AlignPos,Boolean UseBlockDisp,Int2 ColWidth,
		Uint4 DisplayType)
{
Int4 pos,AliPos,stop;				/*scale start at...(used to draw text)*/
Int2 x,y,y2,y3,y4,xDecal;	/*text position*/
Int2 xMargin,yMargin;	/*initial margins*/
Char szBuf[15]={""};	/*scale value*/
Int2 nTicks=0,nLet=0;	/*y decal*/
Int2 cxChar_2,cyChar_4;

	if (ShowMajorTick) nTicks++;
	if (ScalePosition==SCALE_POS_TOP || 
					ScalePosition==SCALE_POS_BOTH) nLet++;
	
	pos=StartLetter+1;	/*remember : StartLetter is zero based*/
	xMargin=rc->left+LeftDecal;
	yMargin=rc->top;
	y=yMargin;
	xDecal=xMargin;
	cxChar_2=GrData->udv_font.cxChar/2;
	cyChar_4=GrData->udv_font.cyChar/4;
	y2=(Int2)(y+GrData->udv_font.LineHeight);
	AliPos=AlignPos+1;/*switch to 1-based value*/
	ScaleMaxVal++;

	if (DisplayType&DDV_DISP_VERT)
		stop=StopLetter+2;
	else
		stop=StopLetter+1;
		
	if ((ScalePosition==SCALE_POS_TOP || 
					ScalePosition==SCALE_POS_BOTH)){
		while(pos<stop){
			if (/*(pos==StartLetter+1) ||*/ !(AliPos % LETTER_BLOCK_WIDTH)
					&& AliPos!=ScaleMaxVal){
				if (DisplayType&DDV_DISP_REVERTCOORD)
					sprintf(szBuf,"%d",UDV_RevertBioSeqCoord(AliPos,ScaleMaxVal));
				else
					sprintf(szBuf,"%d",AliPos);

				SetColor(GrData->udv_scale.ScaleColor);
				/*scale on top or both*/
				if(ShowMajorTick){/*center text on the tick*/
					x=xDecal-(StringWidth(szBuf))/2;
				}
				else{/*align text right on pos*/
					x=xDecal-StringWidth(szBuf)+cxChar_2;
				}
				if (ScalePosition==SCALE_POS_TOP || 
							pos!=StartLetter+1){
					MoveTo(x,y2);
					PaintString (szBuf);
				}
			}

			if (AliPos==ScaleMaxVal){/*end sequence only*/
				if (ScalePosition==SCALE_POS_TOP || 
						ScalePosition==SCALE_POS_BOTH){
					/*scale on top or both*/
					if(ShowMajorTick){
						/*center text on the tick*/
						if (DisplayType&DDV_DISP_REVERTCOORD)
							sprintf(szBuf,"%d",UDV_RevertBioSeqCoord(AliPos,ScaleMaxVal));
						else
							sprintf(szBuf,"%d",AliPos);
						x=xDecal-(StringWidth(szBuf))/2;
						MoveTo(x,(Int2)(y2+GrData->udv_font.cyChar));
						SetColor(GrData->udv_scale.ScaleColor);
						PaintString (szBuf);
					}
					/*else {
						x=xDecal-StringWidth(szBuf)+GrData->udv_font.cxChar;
						MoveTo(x,y2);
					}*/
				}
			}

			xDecal+=ColWidth;

			/*each LETTER_BLOCK_WIDTH, add a blank column*/
			if (!(AliPos % LETTER_BLOCK_WIDTH)&&UseBlockDisp) xDecal+=ColWidth;
			pos++;AliPos++;
		}
	}

	/*scale on left only*/
	pos=StartLetter+1;	/*remember : StartLetter is zero based*/
	y=yMargin;
	xDecal=xMargin;
	y2=(Int2)(y+(nTicks+nLet+1)*GrData->udv_font.LineHeight);
	AliPos=AlignPos+1;/*switch to 1-based value*/
	if ((ScalePosition==SCALE_POS_LEFT || 
		ScalePosition==SCALE_POS_BOTH)){
		/*scale on the left; must be located on the BioSeq line*/
		SetColor(GrData->udv_scale.ScaleColor);
		if (DisplayType&DDV_DISP_REVERTCOORD)
			sprintf(szBuf,"%d",UDV_RevertBioSeqCoord(AliPos,ScaleMaxVal));
		else
			sprintf(szBuf,"%d",AliPos);
		x=rc->left-GrData->udv_font.cxChar-StringWidth(szBuf);
		MoveTo(x,y2);
		PaintString (szBuf);
	}

	/*Major ticks*/
	pos=StartLetter+1;	/*remember : StartLetter is zero based*/
	y=yMargin;
	xDecal=xMargin;
	y2=(Int2)(y+((nLet+1)*GrData->udv_font.LineHeight)-cyChar_4);
	y3=(Int2)(y+(nLet*GrData->udv_font.LineHeight)+cyChar_4);
	y4=(Int2)(y+(nLet*GrData->udv_font.LineHeight)+GrData->udv_font.cyChar);
	if ((ScalePosition==SCALE_POS_TOP || 
		ScalePosition==SCALE_POS_BOTH) && 
		ShowMajorTick){
		
		while(pos<StopLetter+1){
			if (AliPos!=ScaleMaxVal && 
				!(AliPos % LETTER_BLOCK_WIDTH)){
				SetColor(GrData->udv_scale.TickMajColor);
				x=xDecal;
				MoveTo(x,y3);
				LineTo(x,y2);
			}
			/*Minor ticks; only shown if Major present*/
			if (ShowMMinorTick && 
				!(AliPos % (LETTER_BLOCK_WIDTH/2)) && 
					(AliPos % LETTER_BLOCK_WIDTH)){
				SetColor(GrData->udv_scale.TickMinColor);
				x=xDecal;
				MoveTo(x,y4-2);
				LineTo(x,y2-2);
			}

			xDecal+=ColWidth;

			/*each LETTER_BLOCK_WIDTH, add a blank column*/
			if (!(AliPos % LETTER_BLOCK_WIDTH)&&UseBlockDisp) xDecal+=ColWidth;
			AliPos++;pos++;
		}
	}
	Black();
}

/*******************************************************************************

  Function : UDV_calc_decalRC()
  
  Purpose : compute the y decal for ParaG RecT correct positionning on
  		the viewer panel
  
  Parameters :	ParaG; list of pgp
  				ScrollPos; actual position of the Vscroll bar
				SrollMax; max value of the Vscroll bar
				ScrollPage; size of a scroll page by page
				nTotLines; total lines for all the pgp
				LineHeight; height of a single line (pixel unit) 
				vnp; first ParaG to draw in the panel
				stop; value to be used to stop the draw
				
  Return value : y decal value (pixel unit)

*******************************************************************************/
static Int4  UDV_calc_decalRC(ValNodePtr ParaG,Int4 ScrollPos,
		Int4 ScrollMax,Int4 ScrollPage,Int4 nTotLines,Int2 LineHeight,
		ValNodePtr PNTR vnp,Int4Ptr stop)
{
ValNodePtr vnp2;
Int4 decal=0;
ParaGPtr pgp;
		
	/*find the ParaG where ScrollPos is Located*/
	for(*vnp=ParaG ; *vnp != NULL ; *vnp=(*vnp)->next){
		if ((*vnp)->data.ptrvalue){
			pgp=(ParaGPtr)(*vnp)->data.ptrvalue;
			if ((ScrollPos>=pgp->StartLine) &&
				(ScrollPos<=(pgp->StartLine+pgp->nLines))){
					break;
			}
		}		
	}	

	if (ScrollMax){
		*stop=MIN(nTotLines,ScrollPos+ScrollPage);
		for(vnp2=(*vnp) ; vnp2 != NULL ; vnp2=vnp2->next){
			pgp=(ParaGPtr)vnp2->data.ptrvalue;
			if (pgp->StartLine+pgp->nLines>=(*stop)){
				break;
			}
		}
	}
	else{
		*stop=nTotLines;
	}

	decal=ScrollPos*LineHeight-VIEWER_VERT_MARGIN;

	return(decal);
}

/*******************************************************************************

  Function : UDV_calc_RCdraw()
  
  Purpose : compute the RC of a ParaG to be drawn  
  
  Parameters :	rc_pgp; absolute position of a ParaG
  				decal; y decal to place rc_pgp within the viewer panel 
					(this value is calculated by UDV_calc_decalRC())
  
  Return value : position of the ParaG within the viewer panel

*******************************************************************************/
static RecT  UDV_calc_RCdraw(Int4 StartLine,Int4 nLines, RecT rcP,
		Int2 decal_gauche,Int4 decal_haut,Int2 LineHeight)
{
RecT rc;

	rc.top=(Int2)(StartLine*LineHeight+rcP.top-decal_haut);
	rc.bottom=(Int2)(rc.top+nLines*LineHeight);
	rc.left=decal_gauche;
	rc.right=rcP.right-VIEWER_HORZ_MARGIN;

	return(rc);
}

/*******************************************************************************

  Function : UDV_draw_empty_panel()
  
  Purpose : draw a emplty window
  
  Parameters :	rectangle to fill 
  				GrDate; graphical data
  
  Return value : none

*******************************************************************************/
#ifndef WWW_UDV
static void  UDV_draw_empty_panel(RecT rc,UnDViewerGraphDataPtr GrData,
	Boolean ShowText)
{
	White();
	PaintRect(&rc);

	if (!ShowText) return;
	Black();
	SelectFont(GrData->udv_font.hFnt);

	MoveTo((Int2)(rc.left+5),(Int2)(rc.top+GrData->udv_font.cyChar));
	PaintString("Window");
	MoveTo((Int2)(rc.left+5),(Int2)(rc.top+2*GrData->udv_font.cyChar));
	PaintString("too small");
	MoveTo((Int2)(rc.left+5),(Int2)(rc.top+3*GrData->udv_font.cyChar));
	PaintString("to draw.");
}
#endif /*WWW_UDV*/

/*******************************************************************************

  Function : UDV_draw_panel()
  
  Purpose :  draw the panel of UnD viewer
  
  Parameters :	handle of the panel
  
  Note : this function can be used as a callback by external software using
  		UnD-Viewer
  
  Return value : none

*******************************************************************************/
#ifndef WWW_UDV
static void UDV_draw_panel(PaneL p,ViewerDialogDataPtr vdp,RecT PNTR MyUpdateRect,
	Boolean bSelect)	
{
Uint4               DisplayStyle;
Int4 				stop,decal_haut,from_row,to_row,nTotRow,nLinesDraw;
ValNodePtr 			vnp,vnp2,vnp_bsp;
ParaGPtr 			pgp;
RecT 				rc,rcP,rcP_old;
Int2				decal_gauche;
DDV_ColorGlobal     *svpp=NULL;
ValNodePtr          vnp_seqinfo=NULL;
SelStructPtr        ssp;/*used to get the selected region(s)*/
Uint2               bsp_eID,bsp_iID;
ViewerMainPtr 	    vmp;
Int4                Row = -1;

	/*restrict panel drawing area: 'add' little margins*/
	ObjectRect(p,&rcP);
	rcP_old=rcP;
	InsetRect(&rcP,4,4);
	ClipRect(&rcP);


	if (vdp == NULL) {
		ResetClip();	
		return;
	}

	if (vdp->Parent){
		vmp=(ViewerMainPtr)GetObjectExtra(vdp->Parent);
		if (vmp->Show_logo){
			UDV_Logo_onDraw(p);
			return;
		}
	}
	
	if (vdp->ParaG == NULL) {
		UDV_draw_empty_panel(rcP,&vdp->udv_graph,FALSE);
		ResetClip();	
		return;
	}

	if (vdp->udv_graph.udv_panel.nBlockByLine<1) {
		UDV_draw_empty_panel(rcP,&vdp->udv_graph,TRUE);
		ResetClip();	
		return;
	}
	
	decal_haut=vdp->udv_graph.udv_vscrl.ScrollPos*vdp->udv_graph.udv_font.LineHeight-
			VIEWER_VERT_MARGIN;	

	from_row=(MyUpdateRect->top-rcP.top)/vdp->udv_graph.udv_font.LineHeight-1;
	to_row=(MyUpdateRect->bottom-rcP.top)/vdp->udv_graph.udv_font.LineHeight+1;
	if (from_row<0 && to_row<0) return;		
	if (from_row<0) from_row=0;
	if (to_row<0) to_row=0;
	from_row+=vdp->udv_graph.udv_vscrl.ScrollPos;
	to_row+=vdp->udv_graph.udv_vscrl.ScrollPos;
	if (from_row>vdp->udv_graph.udv_panel.nTotLines) 
		from_row=vdp->udv_graph.udv_panel.nTotLines;
	if (to_row>vdp->udv_graph.udv_panel.nTotLines) 
		to_row=vdp->udv_graph.udv_panel.nTotLines;
	nTotRow=to_row-from_row;

	/*find the ParaG where ScrollPos is Located*/
	vnp=vdp->ParaG;
	while(vnp){
		if (vnp->data.ptrvalue){
			pgp=(ParaGPtr)vnp->data.ptrvalue;
			if ((pgp->StartLine<=to_row)&&((pgp->StartLine+pgp->nLines)>=from_row)){
					break;
			}
		}
		vnp=vnp->next;
	}	

	/*retrieve the colours table for the bsp*/
#ifndef WWW_UDV
    if(vdp->vgp != NULL) {
        if(vdp->vgp->MasterViewer == SAMVIEWDDV)
            svpp = vdp->vgp->pCGlobal;
            Row = ViewMgr_GetRow(svpp, vdp->userkey);
    }
    if(svpp == NULL)
	    svpp = (DDV_ColorGlobal *)DDV_GetColorGlobalEx((void *)vdp->bsp_i.bsp);
#endif

#ifdef WWW_UDV
	svpp = NULL;
#endif

	/*draw ParaG until out of panel*/
	SelectFont(vdp->udv_graph.udv_font.hFnt);

	/*3D border to resize the cxName field*/
	LtGray();
	
	MoveTo((Int2)(vdp->udv_graph.udv_panel.cxName-1),rcP.top);
	LineTo((Int2)(vdp->udv_graph.udv_panel.cxName-1),rcP.bottom);

	LtGray();
	MoveTo((Int2)(vdp->udv_graph.udv_panel.cxName+1),rcP.top);
	LineTo((Int2)(vdp->udv_graph.udv_panel.cxName+1),rcP.bottom);
	LtGray();
	MoveTo((Int2)(vdp->udv_graph.udv_panel.cxName+2),rcP.top);
	LineTo((Int2)(vdp->udv_graph.udv_panel.cxName+2),rcP.bottom);

	Black();
	MoveTo((Int2)(vdp->udv_graph.udv_panel.cxName+3),rcP.top);
	LineTo((Int2)(vdp->udv_graph.udv_panel.cxName+3),rcP.bottom);

	decal_gauche=vdp->udv_graph.udv_panel.cxName+rcP_old.left+
			vdp->udv_graph.udv_scale.cxLeftScale;
	nLinesDraw=0;		
	
	/*get selected regions*/
	ssp=ObjMgrGetSelected();
	vnp_bsp=NULL;
	bsp_eID=ObjMgrGetEntityIDForPointer((Pointer)vdp->bsp_i.bsp);
	bsp_iID = GetItemIDGivenPointer (bsp_eID, 
			OBJ_BIOSEQ, (Pointer) vdp->bsp_i.bsp);	
	if (bsp_eID!=0 && bsp_iID!=0){
		vnp_bsp=UDV_GetSelectedRegions(ssp,bsp_eID,bsp_iID);
	}

	if (vdp->bDisplayRevComp)
		DisplayStyle=DDV_DISP_VERT|DDV_DISP_REVERTCOORD;
	else
		DisplayStyle=DDV_DISP_VERT;
	
	for(vnp2=vnp ; vnp2 != NULL ; vnp2=vnp2->next){
		if (vnp2->data.ptrvalue){
			pgp=(ParaGPtr)vnp2->data.ptrvalue;
			
			rc=UDV_calc_RCdraw(pgp->StartLine,pgp->nLines,rcP_old,
					decal_gauche,decal_haut,vdp->udv_graph.udv_font.LineHeight);

		/*ruler*/
			if (vdp->udv_graph.udv_panel.ShowScale)
				UDV_Draw_scale(&vdp->udv_graph,
					vdp->udv_graph.udv_scale.ShowMajorTick,
					vdp->udv_graph.udv_scale.ShowMMinorTick,
					vdp->udv_graph.udv_scale.ScalePosition,
					pgp->StartLetter,pgp->StopLetter,&rc,
					vdp->udv_graph.udv_font.cxChar,
					vdp->bsp_i.bspLength,pgp->StartLetter,
					TRUE,
					vdp->udv_graph.udv_font.cxChar,DisplayStyle);
		/*Sequence*/
			if (vdp->bsp_i.SeqBuf)
				UDV_Draw_sequence(&vdp->udv_graph,svpp,
					vdp->udv_graph.udv_font.UseDefaultColorLetter,
					vdp->udv_graph.udv_font.LetterColor,
					pgp,&rc,vdp->bsp_i.StartBuf,pgp->StartLetter,
					vdp->bsp_i.SeqBuf,vdp->bsp_i.bsp->id,vnp_bsp,bSelect,Row,
					vdp->bsp_i.bspLength,vdp->bDisplayRevComp);
		/*Sequence name*/
			UDV_Draw_sequence_name(&vdp->udv_graph,&vdp->bsp_i,&rc);
		/*Features*/
			if (vdp->udv_graph.udv_panel.ShowFeatures){
				UDV_Draw_features(&vdp->udv_graph,&vdp->bsp_i,
					vdp->udv_graph.udv_font.UseDefaultColorLetter,
					vdp->udv_graph.udv_font.LetterColor,pgp,&rc,
					vdp->udv_graph.pClr,&(vdp->Item_select),&(vdp->Old_Item_select),
					DDV_DISP_VERT);
				/*Feature names
					UDV_Draw_features_label(&vdp->udv_graph,&vdp->bsp_i,pgp,
						&rc,
						vdp->udv_graph.pClr,vdp->Item_select,
						vdp->Old_Item_select);*/
			}
			/*nLinesDraw+=pgp->nLines;
			if (nLinesDraw>nTotRow) break;*/
			if ((pgp->StartLine+pgp->nLines)>to_row) break;
			/*if (pgp->StartLine > stop) break;*/
		}		
	}	

	ResetClip();	
}
#endif /*WWW_UDV*/

/*******************************************************************************

  Function : UDV_draw_viewer()
  
  Purpose :  draw the panel of UnD viewer
  
  Parameters :	handle of the panel
  
  Note : this function is called when clickfeat; this is not a callback
  
  Return value : none

*******************************************************************************/
#ifndef WWW_UDV
NLM_EXTERN void  UDV_draw_viewer (PaneL p)
{
WindoW 	w;
ViewerDialogDataPtr vdp;

	/*get some usefull data...*/
	vdp = (ViewerDialogDataPtr) GetObjectExtra (p);

	if (vdp==NULL) return;

	UDV_draw_panel(p,vdp,&updateRect,TRUE);
}
#endif /*WWW_UDV*/


/*******************************************************************************

  Function : UDV_Logo_onDraw()
  
  Purpose : draw the software logo
  
  Parameters :	handle of the panel
  
  Return value : none

*******************************************************************************/
#ifndef WWW_UDV
NLM_EXTERN void UDV_Logo_onDraw (PaneL p)
{
WindoW 				w;
RecT 				rcP;
UDVLogoDataPtr		ldp;

	/*get some usefull data...*/
	ldp = (UDVLogoDataPtr) GetAppProperty ("UDVLogoData");

	if (ldp==NULL) return;
	
	Select (p);
	/*restrict panel drawing area: 'add' little margins*/
	ObjectRect(p,&rcP);
	InsetRect(&rcP,4,4);
	ClipRect(&rcP);

	White();
	PaintRect(&rcP);
	if (ldp->f1 && ldp->f2 && ldp->f3) draw_logo(rcP,ldp->f1,ldp->f2,ldp->f3,
			ldp->szTitle,ldp->szDesc);
	ResetClip();	

	/*3D border*/
}
#endif /*WWW_UDV*/

