/*  $Id: ddvpanel.c,v 1.108 2004/04/01 13:43:06 lavr Exp $
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
* File Name:  ddvpanel.c
*
* Author:  Patrick Durand
*
* Version Creation Date:   06/19/99
*
* $Revision: 1.108 $
*
* File Description: window management module of DeuxD-Viewer (DDV)
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: ddvpanel.c,v $
* Revision 1.108  2004/04/01 13:43:06  lavr
* Spell "occurred", "occurrence", and "occurring"
*
* Revision 1.107  2000/08/30 14:31:28  hurwitz
* quick fix so build does not crash
*
* Revision 1.106  2000/07/25 20:30:23  hurwitz
* bug fixes: panel update when file is closed, double-click launches UDV from DDE, seqName agrees in status line and left col
*
* Revision 1.105  2000/07/24 22:00:09  hurwitz
* fixed bug that 1/2 characters were displayed sometimes
*
* Revision 1.104  2000/07/20 22:25:57  hurwitz
* bug fix.  mouse-mode query working again
*
* Revision 1.103  2000/07/19 19:04:02  hurwitz
* fixed bug that was causing overwrite of numbering over tick marks in DDE
*
* Revision 1.102  2000/07/17 13:32:33  lewisg
* move DDV_args out of the library
*
* Revision 1.101  2000/07/17 12:38:53  kans
* DDV_myargs is extern in header, instantiated in ddvpanel.c, since it is accessed from that library file
*
* Revision 1.100  2000/07/14 22:24:56  lewisg
* fix scroll, click, visual c++ build bugs.  add command line arg parse to ddv
*
* Revision 1.99  2000/07/12 22:22:41  hurwitz
* added delete block to DDV
*
* Revision 1.98  2000/07/10 14:38:08  lewisg
* move seqalign and sequentry data from window to panels
*
* Revision 1.97  2000/07/07 22:31:14  lewisg
* interface tweaks
*
* Revision 1.96  2000/07/05 19:23:13  lewisg
* add two panes to ddv, update msvc project files
*
* Revision 1.95  2000/07/05 18:42:16  hurwitz
* added split block function to DDV
*
* Revision 1.94  2000/06/30 22:31:51  hurwitz
* added merge block function for DDV
*
* Revision 1.93  2000/06/29 23:15:13  hurwitz
* leave single space between aligned blocks with no unaligned sequence between them, no auto-merge of adjacent aligned blocks
*
* Revision 1.92  2000/06/27 20:46:38  hurwitz
* fixed bugs with select rectangle, added select row option
*
* Revision 1.91  2000/06/22 20:56:51  hurwitz
* assorted bug fixes
*
* Revision 1.90  2000/06/20 19:35:11  hurwitz
* use indexed seqAlign when necessary, make multiple when redrawing
*
* Revision 1.89  2000/06/15 20:26:05  hurwitz
* added left/right/center justify for DDE
*
* Revision 1.88  2000/06/15 17:33:18  hurwitz
* used view seqAlignPtr to get original (bug fix), and started working on left/right/center justify for DDE
*
* Revision 1.87  2000/06/15 16:01:07  hurwitz
* rewrote section that calls ViewMgr_Attach when SeqAlign is not editable but IBMable
*
* Revision 1.86  2000/06/12 23:02:40  hurwitz
* enable launch of DDE from Cn3D, swith from DDV_ComputeRuler to DDE_ReMakeRuler, get rid of styles option for DDE
*
* Revision 1.85  2000/06/08 20:04:39  hurwitz
* made warning about converting to true multiple alignment into a Message window, and other small fixes
*
* Revision 1.84  2000/06/02 19:57:03  hurwitz
* added a query dialog when converting from pairwise multiple to true multiple, bug fix of double freeing of a seqAlignPtr
*
* Revision 1.83  2000/05/31 23:07:26  hurwitz
* made NoGaps a runtime parameter, fixed bug with vertical scroll of show/hide list, save edits query is not performed if nothing to save
*
* Revision 1.82  2000/05/25 21:40:43  hurwitz
* rows hidden in DDV are hidden in DDE, can save edits when rows are hidden in DDE
*
* Revision 1.81  2000/05/24 21:43:00  hurwitz
* getting hide/show rows to work with DDV and DDE together
*
* Revision 1.80  2000/05/23 22:00:15  hurwitz
* working on launch of DDE from DDV
*
* Revision 1.79  2000/05/19 13:48:31  hurwitz
* made a version of DDE that doesn't allow aligned gaps, changed wording for adding new rows
*
* Revision 1.78  2000/05/16 19:43:01  hurwitz
* grey out create block, delete block, undo, and redo as needed
*
* Revision 1.77  2000/05/15 23:39:34  lewisg
* shred cblast, add menu items for gapped/ungapped, fix pdbheaders
*
* Revision 1.76  2000/05/15 22:00:36  hurwitz
* put save-edits query into DDV_SlaveQuit where it belongs
*
* Revision 1.75  2000/05/12 21:18:13  hurwitz
* added window asking if user wants to save unsaved edits for dde
*
* Revision 1.74  2000/05/10 21:54:54  hurwitz
* free memory when DDE is closed
*
* Revision 1.73  2000/05/10 16:17:00  hurwitz
* can show tails for just 1 aligned block
*
* Revision 1.72  2000/05/09 17:56:49  hurwitz
* bug fix of vertical scrolling problem
*
* Revision 1.71  2000/05/08 22:48:33  hurwitz
* on launch of DDE, merge nodes that are same MsaTxtDisp alignment and gap
*
* Revision 1.70  2000/05/05 20:24:13  hurwitz
* some bug fixes, also redraw proper block in DDE after a save operation that causes a merge of 2 blocks
*
* Revision 1.69  2000/05/04 22:43:38  hurwitz
* don't launch DDE on top of DDV, change some wording, redraw DDE after save to AlnMgr
*
* Revision 1.68  2000/04/28 14:43:37  hurwitz
* send message to DDV after edits in DDE are accepted by AlnMgr
*
* Revision 1.67  2000/04/27 22:24:36  lewisg
* misc bugs
*
* Revision 1.66  2000/04/27 20:05:54  kans
* added stub functions for Sequin
*
* Revision 1.65  2000/04/27 19:57:59  hurwitz
* scroll to aligned block on launch of DDE
*
* Revision 1.64  2000/04/27 15:38:02  hurwitz
* changed wording on menu
*
* Revision 1.63  2000/04/27 13:33:42  durand
* SpacerSize min value fixed to 1; update view when SpacerSize is changed
*
* Revision 1.62  2000/04/26 21:54:27  hurwitz
* added save function to tell AlnMgr about edits made in DDE
*
* Revision 1.61  2000/04/21 23:00:50  hurwitz
* can launch DDE from DDV
*
* Revision 1.60  2000/04/18 19:50:26  lewisg
* add deselect menu item
*
* Revision 1.59  2000/04/17 21:46:56  lewisg
* do not do double index on viewmgr update, rename menus
*
* Revision 1.58  2000/04/10 21:41:26  lewisg
* move alignment menus into ddv, udv from cn3d
*
* Revision 1.57  2000/04/10 20:58:42  hurwitz
* added GUI controls for DeleteBlock in DDE
*
* Revision 1.56  2000/04/07 16:21:08  hurwitz
* made delete block faster, added delete block to edit menu
*
* Revision 1.55  2000/03/31 19:33:50  durand
* added DDV_ShowHideUnAlignGap for import seq. debugging purpose
*
* Revision 1.54  2000/03/27 22:15:08  lewisg
* add show/hide row dialog
*
* Revision 1.53  2000/03/25 00:22:09  hurwitz
* put DDE_StackPtr in DDV_Main, add to stack inside DDE api's, added insert char, delete char, home and end keyboard control
*
* Revision 1.52  2000/03/23 00:00:00  hurwitz
* DDE api's are called with stack now
*
* Revision 1.51  2000/03/21 14:29:49  durand
* fixed a problem with menus setup
*
* Revision 1.49  2000/03/20 22:22:48  hurwitz
* added more checks to verify subroutine, 1 bug fix
*
* Revision 1.48  2000/03/17 16:15:30  durand
* added ASN.1 SeqAlign and SeqAnoot export
*
* Revision 1.47  2000/03/16 20:53:48  thiessen
* removed C++ style comment
*
* Revision 1.46  2000/03/16 15:51:32  hurwitz
* added function to create an aligned block
*
* Revision 1.45  2000/03/16 14:11:59  durand
* set corretly mouse mode menu
*
* Revision 1.44  2000/03/14 22:08:21  hurwitz
* undo and redo working properly, restore-original function added
*
* Revision 1.43  2000/03/10 23:01:43  hurwitz
* added undo and redo functions, first pass
*
* Revision 1.42  2000/03/09 22:28:40  hurwitz
* added shift block and delete block, a bug fix too
*
* Revision 1.41  2000/03/08 22:02:07  hurwitz
* added verify function, debugging, handle align_start != 0
*
* Revision 1.40  2000/03/06 22:45:58  hurwitz
* can shift right boundary of an aligned block left and right, DDVRuler updates added
*
* Revision 1.39  2000/03/02 15:43:11  durand
* use MovableModalWindow for dialog boxes
*
* Revision 1.38  2000/03/01 22:49:41  lewisg
* import bioseq, neatlyindex, get rid of dead code
*
* Revision 1.37  2000/02/29 21:13:06  hurwitz
* added low level functions for shifting left and right the left alignment boundary
*
* Revision 1.36  2000/02/28 16:28:39  hurwitz
* added functions for deleting an aligned gap
*
* Revision 1.35  2000/02/24 23:37:00  hurwitz
* added ability to insert gaps
*
* Revision 1.34  2000/02/18 16:06:22  hurwitz
* for editing multiple sequence alignments: shift row right now working
*
* Revision 1.33  2000/02/14 20:58:57  hurwitz
* added functions for editing multiple sequence alignments: hide/show row, move row, shift row left
*
* Revision 1.32  2000/02/08 14:10:42  durand
* made extern the functions DDV_UpdateHScrollVal and
*
* Revision 1.31  2000/02/07 14:03:35  durand
* replace BioseqUnlockById by BioseqUnlock
*
* Revision 1.30  2000/02/05 01:32:22  lewisg
* add viewmgr, move place freeing is done in ddv, modify visual c++ projects
*
* Revision 1.29  2000/02/04 16:05:41  durand
* add click action to select a row
*
* Revision 1.28  2000/01/26 13:38:55  durand
* update the GUI for the editor. Add functions to create the data to be used by the editor
*
* Revision 1.27  2000/01/18 22:49:16  lewisg
* send OM_MSG_FLUSH to ddv/udv, tweak CPK coloration, misc bugs
*
* Revision 1.26  2000/01/12 21:52:17  durand
* add import function; update menus when DDV is loaded from Cn3D
*
* Revision 1.25  2000/01/12 15:49:42  durand
* add About dlg box and fix a bug in selection
*
* Revision 1.24  2000/01/11 15:05:23  durand
* remove network stuff
*
* Revision 1.23  2000/01/10 15:09:45  durand
* Use Entrez instead of ID1
*
* Revision 1.22  1999/12/23 19:22:07  durand
* modify default options for DDV when loaded from Cn3D
*
* Revision 1.21  1999/12/21 15:27:24  durand
* avoid to quit Cn3D when closing DDV
*
* Revision 1.20  1999/12/20 20:20:41  lewisg
* allow cn3d to do color and ddv to do case when both are running
*
* Revision 1.19  1999/12/07 21:40:14  durand
* add mouse modes menu and caret facility for the editor
*
* Revision 1.18  1999/12/06 16:19:19  durand
* add GoTo facility to DDV
*
* Revision 1.17  1999/12/03 23:17:23  lewisg
* Patrick's new global update msg, argument passing when launching ddv, experimental editing
*
* Revision 1.16  1999/11/30 18:19:48  durand
* fix a problem with function declaration DDV_CloseData
*
* Revision 1.15  1999/11/30 17:28:59  durand
* fix a problem of redeclaration of the function DDV_CloseData
*
* Revision 1.14  1999/11/30 14:17:31  durand
* fix a bug occurring when deleting a SeqAlign
*
* Revision 1.13  1999/11/29 15:26:26  durand
* designed a new GUI to fix problems under MacOS, Linux and SGI
*
* Revision 1.12  1999/11/17 22:43:59  durand
* speed up the selection manager for large SeqAlign
*
* Revision 1.11  1999/11/04 22:11:37  durand
* add the Desktop to DDV. Add a better set of cleanup functions when closing DDV. Before creating color tables, try to get them from the SeqAlign
*
* Revision 1.10  1999/11/03 21:29:48  durand
* add CTRL and SHFT keys for mouse selection. redesign the loader functions of DDV to properly register the message callbacks
*
* Revision 1.9  1999/10/29 19:04:59  durand
* move DDVUpdateMSG in objmgr.h
*
* Revision 1.8  1999/10/29 14:15:38  durand
* add simple mouse selection functions
*
* Revision 1.7  1999/10/22 20:12:47  durand
* add Export command (text, HTML and Phylip formats)
*
* Revision 1.6  1999/10/20 15:01:16  durand
* bug fixed when closing Display Styles DlgBox
*
* Revision 1.5  1999/10/20 13:17:19  durand
* add display for disc. SeqAlign tails
*
* Revision 1.4  1999/10/16 15:02:25  durand
* fixes due to toolkit build failed
*
* Revision 1.3  1999/10/15 21:57:36  durand
* add a UI for display options
*
* Revision 1.2  1999/10/02 15:11:20  durand
* update the code to reuse more functions from UDV
*
* Revision 1.1  1999/09/30 14:10:29  durand
* add ddv to toolkit
*
* Revision 1.9  1999/09/21 14:19:08  durand
* add mouse click management layout
*
* Revision 1.8  1999/09/16 13:07:53  durand
* add File|Close and File|Open|Network commands
*
* Revision 1.7  1999/09/09 21:55:06  durand
* instantiate the Fle|Close command of DDV
*
* Revision 1.6  1999/07/20 14:58:02  durand
* use the Color Manager to display colored MSA
*
* Revision 1.5  1999/07/01 15:28:29  durand
* validate function loaders of DDV
*
* Revision 1.2  1999/06/28 22:07:19  durand
* add loader functions and clean the code with Lint and Purify
*
* Revision 1.1  1999/06/19 17:21:06  durand
* add Vibrant DDV code
*
*
*
* ==========================================================================
*/

#include <ncbi.h>
#include <vibrant.h>
#include <udviewer.h>
#include <ddvpanel.h>
#include <ddvopen.h>
#include <ddvmain.h>
#include <ddvgraph.h>
#include <ddvclick.h>
#include <ddvcreate.h>
#include <alignmgr.h>
#include <objmgr.h>
#include <pgppop.h>
#include <vsm.h>
#include <salfiles.h>
#include <viewmgr.h>

Char szAppName[]="DDV";
Char szAppName2[]="DDE";

/*local text*/
static Char szAbout[]="DeuxD-Viewer : A sequence alignment viewer for GenBank\n\
Version 1.0\n\nInformation Engineering Branch\n\
NCBI - NLM - NIH, Bldg 38A\n\
8600 Rockville Pike\n\
Bethesda, MD 20894 - USA\n\n\
info@ncbi.nlm.nih.gov";

#ifdef DEBUG_DDV
static void DDV_ShowHideUnAlignGap(IteM i);
#endif
static void DDV_ExportTexte(IteM i);
static void UDV_GotoBtnProc(ButtoN g);
static void DDV_DeleteRow(IteM i);
static void DDV_MoveRowLeft(IteM i);
static void DDV_MoveRowRight(IteM i);
static void DDV_MoveLeftBoundaryLeft(IteM i);
static void DDV_MoveLeftBoundaryRight(IteM i);
static void DDV_MoveRightBoundaryLeft(IteM i);
static void DDV_MoveRightBoundaryRight(IteM i);
static void DDV_DeleteBlock(IteM i);
static void DDV_CreateBlock(IteM i);
static void DDV_Prev(IteM i);
static void DDV_Next(IteM i);
static void DDV_Orig(IteM i);
static void DDV_DoEditFunction(IteM i, Int4 WhichOne);
static void DDV_LaunchEditor(IteM i);
static void DDV_MergeBlocks(IteM i);
static void DDV_SplitBlock(IteM i);
static void DDV_DeleteBlock2(IteM i);
static void DDV_CallSaveEdits(IteM i);

/*******************************************************************************

  Function : DDV_AboutProc()
  
  Purpose : about dialog box
  
  Parameters : -
  
  Return value : none 

*******************************************************************************/
static void  DDV_AboutProc(IteM i)
{
  MsgAlert(KEY_OK, SEV_INFO, "About DeuxD-Viewer",szAbout);
}

/*******************************************************************************

  Function : DDV_EnableGotoTBItems()
  
  Purpose : enable/disable the controls for "GoTo col,row" toolbar 
  
  Return value : none 

*******************************************************************************/
typedef void (*MyEnaDisable) (Handle a);
extern void DDV_EnableGotoTBItems(WindoW hParent,Boolean bEnable)
{
DdvMainWinPtr      dmwp;
MyEnaDisable       my_func;

	dmwp=(DdvMainWinPtr)GetObjectExtra(hParent);
	if (!dmwp) return;
	
	if (bEnable)	
		my_func=Enable;
	else
		my_func=Disable;
	
	my_func(dmwp->gotoBtn);
	my_func(dmwp->gotoValCol);
	my_func(dmwp->gotoValRow);
}


/*******************************************************************************

  Function : DDV_WhatSize()
  
  Purpose : compute nTotLines and nTotCol depending on the display type 
  
  Parameters : -
  
  Return value : -

*******************************************************************************/
extern void DDV_WhatSize(DdvMainPtr dmp)
{
Int4 i;
ParaGPtr pgp;
	
	if (dmp==NULL) return;
	
	dmp->GrData.udv_panel.nTotLines=0;
	
	if (dmp->MSA_d.pgp_l.DisplayType==0) return;
	if(dmp->MSA_d.pgp_l.DisplayType==DDV_DISP_HORZ){
		for(i=0;i<dmp->MSA_d.pgp_l.nBsp;i++){
			if (dmp->MSA_d.pgp_l.TableHead[i]->data.ptrvalue){
				pgp=(ParaGPtr)(dmp->MSA_d.pgp_l.TableHead[i]->data.ptrvalue);
				dmp->GrData.udv_panel.nTotLines+=pgp->nLines;
			}
		}	
		dmp->GrData.udv_panel.nTotCol=dmp->MSA_d.pgp_l.LengthAli;
	}
	else{/*full vertical display*/
		/*not yet implemented*/
		return;/*add to avoid a 'lint' warning*/
	}
}

/*******************************************************************************

  Function : DDV_CalcVHScrollPgUD()
  
  Purpose : compute PageUp & Down (V & H Scrolls) 
  
  Parameters : cxyClient; either width of height of DDV panel
			LineHeight; either width of a col. or height of one data line
  
  Return value : PageUpDn

*******************************************************************************/
static Int4 DDV_CalcVHScrollPgUD(Int2 cxy_Client,Int2 LineColSize)
{
	return((Int4)(cxy_Client/LineColSize));
}

/*******************************************************************************

  Function : DDV_UpdateVScrollVal()
  
  Purpose : update the DDV VScroll bar 
  
  Parameters : p; handel of the DDV panel
  				bInit; TRUE only when the DDV is created 
  				CurPos ; new position
  
  Return value : none 

*******************************************************************************/
extern void DDV_UpdateVScrollVal(PaneL p,Boolean bInit,Int4 CurPos)
{
BaR vsb;
DdvMainPtr 	dmp;
	
	dmp = (DdvMainPtr) GetObjectExtra((Handle)p);
	/*GetAppProperty("DDV_MAIN_DATA");*/
	if (dmp==NULL) return;

	/*current scroll status*/
	vsb = GetSlateVScrollBar ((SlatE) p);
	
	if (bInit) CurPos=0;
	
	/*given curpos (lineNumber) retrieve letter number*/
	
	/*compute new values*/
	dmp->GrData.udv_vscrl.ScrollPage=
		DDV_CalcVHScrollPgUD((Int2)(dmp->GrData.udv_panel.cyClient-
			3*dmp->GrData.udv_panel.cyScale/2-5),
			dmp->GrData.udv_font.LineHeight);
	
	if (dmp->GrData.udv_panel.nTotLines>dmp->GrData.udv_vscrl.ScrollPage){
		dmp->GrData.udv_vscrl.ScrollMax=dmp->GrData.udv_panel.nTotLines-
							dmp->GrData.udv_vscrl.ScrollPage+3/**/;
	}
	else{
		dmp->GrData.udv_vscrl.ScrollMax=0;
		dmp->GrData.udv_vscrl.ScrollPos=0;
		dmp->GrData.udv_vscrl.ScrollPage=0;
	}
	
	if (CurPos<0) CurPos=0;

	if (CurPos>=dmp->GrData.udv_vscrl.ScrollMax)
		dmp->GrData.udv_vscrl.ScrollPos=dmp->GrData.udv_vscrl.ScrollMax;
	else dmp->GrData.udv_vscrl.ScrollPos=CurPos;
				
	/*update scroll*/
	CorrectBarMax(vsb,dmp->GrData.udv_vscrl.ScrollMax);
	CorrectBarValue(vsb,dmp->GrData.udv_vscrl.ScrollPos);
	CorrectBarPage(vsb,dmp->GrData.udv_vscrl.ScrollPage,
			dmp->GrData.udv_vscrl.ScrollPage);
	
}

/*******************************************************************************

  Function : DDV_UpdateHScrollVal()
  
  Purpose : update the DDV HScroll bar 
  
  Parameters : p; handel of the DDV panel
  				bInit; TRUE only when the DDV is created 
  				CurPos ; new position
  
  Return value : none 

*******************************************************************************/
extern void DDV_UpdateHScrollVal(PaneL p,Boolean bInit,Int4 CurPos)
{
BaR hsb;
DdvMainPtr 	dmp;
	
	dmp = (DdvMainPtr) GetObjectExtra((Handle)p);
	if (dmp==NULL) return;

	/*current scroll status*/
	hsb = GetSlateHScrollBar ((SlatE) p);
	if (!hsb) return;/*not always present*/
	
	if (bInit) CurPos=0;
	
	/*given curpos (lineNumber) retrieve letter number*/
	
	/*compute new values*/
	dmp->GrData.udv_hscrl.ScrollPage=
		DDV_CalcVHScrollPgUD((Int2)(dmp->GrData.udv_panel.cxClient-
			dmp->GrData.udv_panel.cxName),
			dmp->GrData.udv_font.ColWidth);
	
	if (dmp->GrData.udv_panel.nTotCol>dmp->GrData.udv_hscrl.ScrollPage){
		dmp->GrData.udv_hscrl.ScrollMax=dmp->GrData.udv_panel.nTotCol-
							dmp->GrData.udv_hscrl.ScrollPage+5/**/;
	}
	else{
		dmp->GrData.udv_hscrl.ScrollMax=0;
		dmp->GrData.udv_hscrl.ScrollPos=0;
		dmp->GrData.udv_hscrl.ScrollPage=0;
	}
	
	if (CurPos<0) CurPos=0;

	if (CurPos>=dmp->GrData.udv_hscrl.ScrollMax)
		dmp->GrData.udv_hscrl.ScrollPos=dmp->GrData.udv_hscrl.ScrollMax;
	else dmp->GrData.udv_hscrl.ScrollPos=CurPos;
				
	/*update scroll*/
	CorrectBarMax(hsb,dmp->GrData.udv_hscrl.ScrollMax);
	CorrectBarValue(hsb,dmp->GrData.udv_hscrl.ScrollPos);
	CorrectBarPage(hsb,dmp->GrData.udv_hscrl.ScrollPage,
			dmp->GrData.udv_hscrl.ScrollPage);
	
}


/*****************************************************************************

  Function : DDV_VHScrl()
  
  Purpose : DD Viewer Horizontal/Vertical Scroll Bar function 
  
  Parameters : p; viewer
  			vdp; viewer data structure
			newval; new value of the thumb
			oldval; old value of the thumb
			IsVscroll; TRUE means Vertical scrollbar
			
  Note : this function can be used to scroll the panel for any reasons.
  
  Return value : none 

*****************************************************************************/
extern void DDV_VHScrl(PaneL p,UnDViewerGraphDataPtr gdp, Int4 newval, Int4 oldval,
	Boolean IsVscroll)
{
  RecT 	      rcP;
  WindoW 	    temport;
  Int4 	      n,limit;
  Int2	      dx,dy;
  DdvMainPtr  dmp;

	if (!gdp || !p) return;

  dmp = (DdvMainPtr) GetObjectExtra(p);
	if (dmp==NULL) return;

	/*compute scroll offset*/
	if (oldval>newval)n=oldval-newval;
	else n=newval-oldval;
	
	temport=SavePort(ParentWindow((WindoW)p));
	Select(p);
	ObjectRect(p,&rcP);
	InsetRect(&rcP,4,4);
	/*drawing region:*/
  DDV_AdjustDrawingRect(&rcP, &(gdp->udv_font), dmp);

	if (IsVscroll){/*Vertical scroll*/
		gdp->udv_vscrl.ScrollPos=newval;
		limit=(Int4)gdp->udv_vscrl.ScrollPage;
		dx=0;
		dy=(Int2)((oldval-newval)*gdp->udv_font.LineHeight);
		rcP.top+=3*gdp->udv_panel.cyScale/2;
	}
	else{/*horizontal scroll*/
		gdp->udv_hscrl.ScrollPos=newval;
		limit=(Int4)gdp->udv_hscrl.ScrollPage;
		dx=(Int2)((oldval-newval)*gdp->udv_font.ColWidth);
		dy=0;
		rcP.left+=(gdp->udv_panel.cxName+gdp->udv_scale.cxLeftScale);
	}
	
	ClipRect(&rcP);

	if (n<limit){/*Line UP/Down or Column left/right*/
		ScrollRect (&rcP, dx, dy);
	}
	else{/*Page Up/Down or Page left/right*/
		InvalRect(&rcP);
	}
	ResetClip();
	RestorePort(temport);
	Update ();
}

/*****************************************************************************

  Function : DDV_VScrlProc()
  
  Purpose : DD Viewer Vertical Scroll Bar Callback 
  
  Parameters : see vibrant library
  
  Return value : none 

*****************************************************************************/
extern void DDV_VScrlProc (BaR sb, SlatE s, Int4 newval, Int4 oldval)
{

DdvMainPtr 	dmp;

	/*get the application data*/
	dmp = (DdvMainPtr) GetObjectExtra((Handle)s);
	if (dmp==NULL) return;

	DDV_VHScrl((PaneL)s,&(dmp->GrData),newval,oldval,TRUE);
}

/*****************************************************************************

  Function : DDV_HScrlProc()
  
  Purpose : DD Viewer Horizontal Scroll Bar Callback 
  
  Parameters : see vibrant library
  
  Return value : none 

*****************************************************************************/
extern void DDV_HScrlProc (BaR sb, SlatE s, Int4 newval, Int4 oldval)
{

DdvMainPtr 	dmp;

	/*get the application data*/
	dmp = (DdvMainPtr) GetObjectExtra((Handle)s);
	if (dmp==NULL) return;

	DDV_VHScrl((PaneL)s,&(dmp->GrData),newval,oldval,FALSE);
}

/*****************************************************************************

Function: DDV_InitPanelData()

Purpose: Init numerical values for DDViewer. Called one times at the
		start of the soft.

Return value: none (results are in sdp)

*****************************************************************************/
extern void  DDV_InitPanelData(UDVPanelDataPtr pdp)
{
	pdp->cxClient=0;
	pdp->cyClient=0;
	pdp->nTotLines=0;
	pdp->nTotCol=0;
	pdp->nCharByLine=DDV_DEFAULT_PARAG_SIZE;
	pdp->nBlockByLine=0;
	pdp->ShowFeatures=FALSE;
	pdp->ShowScale=TRUE;
	pdp->ShowByBlock=FALSE;
}

/*****************************************************************************

  Function : DDV_SetupWin()
  
  Purpose : resize DD viewer panel
  
  Parameters :  w; handle of the DD viewer window
  				bInit; is the window new?
  
*****************************************************************************/
extern void DDV_SetupWin (WindoW w,Boolean bInit)
{
RecT rWin, rButton;
RecT rStatus1, rStatus2, rPanel1, rPanel2;
Int2 height,diff,gap,width;
DdvMainWinPtr dmwp;

	dmwp=(DdvMainWinPtr)GetObjectExtra(w);
	
	ObjectRect(w,&rWin);
	width= rWin.right-rWin.left;

	GetPosition(dmwp->StatusGroup,&rStatus1);
	diff=rStatus1.bottom-rStatus1.top;
	gap= 4;

    ObjectRect(dmwp->gotoBtn, &rButton);
	height= rWin.bottom - rWin.top - rButton.bottom - gap;
    
    if(GetStatus(dmwp->MainMenu.ShowLowerPanel)) {
        Show(dmwp->LowerInfoPanel);
        Show(dmwp->StatusGroupLower);
        Show(dmwp->hLowerPanel);

        rPanel1.right = width;
        rPanel1.left = 0;
        rPanel1.top = rButton.bottom + gap;
        rPanel1.bottom = rPanel1.top + 2*height/3 - diff;
        
        rStatus1.right = rPanel1.right;
        rStatus1.left = rPanel1.left;
        rStatus1.top = rPanel1.bottom;
        rStatus1.bottom = rStatus1.top + diff;
        
        rPanel2.right = rPanel1.right;
        rPanel2.left = rPanel1.left;
        rPanel2.top = rStatus1.bottom;
        rPanel2.bottom = rPanel2.top + height/3 - diff;
        
        rStatus2.right = rPanel1.right;
        rStatus2.left = rPanel1.left;
        rStatus2.top = rPanel2.bottom;
        rStatus2.bottom = rStatus2.top + diff;
        
        
        SetPosition (dmwp->StatusGroup, &rStatus1);
        SetPosition (dmwp->UpperInfoPanel, &rStatus1);
        SetPosition (dmwp->StatusGroupLower, &rStatus2);
        SetPosition (dmwp->LowerInfoPanel, &rStatus2);
        SetPosition (dmwp->hUpperPanel, &rPanel1);
        SetPosition (dmwp->hLowerPanel, &rPanel2);
    } else {
        Hide(dmwp->LowerInfoPanel);
        Hide(dmwp->StatusGroupLower);
        Hide(dmwp->hLowerPanel);

        /* set focus */
        dmwp->hWndDDV = dmwp->hUpperPanel;
        DDV_SetMenuFocus(w);
        dmwp->InfoPanel = dmwp->UpperInfoPanel;
        SetTitle(dmwp->InfoPanel,"Ready !");
        SetTitle(dmwp->LowerInfoPanel,"");
        
        rPanel1.right = width;
        rPanel1.left = 0;
        rPanel1.top = rButton.bottom + gap;
        rPanel1.bottom = rPanel1.top + height - diff;
        
        rStatus1.right = rPanel1.right;
        rStatus1.left = rPanel1.left;
        rStatus1.top = rPanel1.bottom;
        rStatus1.bottom = rStatus1.top + diff;
        
        SetPosition (dmwp->StatusGroup, &rStatus1);
        SetPosition (dmwp->UpperInfoPanel, &rStatus1);
        SetPosition (dmwp->hUpperPanel, &rPanel1);
    }
    AdjustPrnt (dmwp->hUpperPanel, &rPanel1, FALSE);

/*	Update();*/
}

/*****************************************************************************

  Function : DDV_ResizeDDV
  
  Purpose : update DDV panel (size and scrolls) 
  
  Parameters : p; DDV panel
  			   bInit; TRUE only when DDV panel is created
	
  Return value : none 

*****************************************************************************/
extern void DDV_Resize_DDV (PaneL p, Boolean bInit)
{
DdvMainPtr 	dmp;
RecT		rcP;
Int4        vCurPos,hCurPos;
BaR	        vsb,hsb;
WindoW      temport;

	if (!p) return;
	dmp = (DdvMainPtr) GetObjectExtra((Handle)p);
	if (dmp==NULL) return;

    GetPosition(p, &rcP);
	/*panels of DDV; */

	UDV_ComputePanelSize(rcP,&(dmp->GrData.udv_panel.cxClient),
		&(dmp->GrData.udv_panel.cyClient));

	temport=SavePort(ParentWindow(p));
	Select(p);

	/*current scroll status*/
	vsb = GetSlateVScrollBar ((SlatE) p);
	vCurPos=GetBarValue(vsb);
	hsb = GetSlateHScrollBar ((SlatE) p);
	hCurPos=GetBarValue(hsb);
			
	/*update scrolls*/
	DDV_UpdateVScrollVal(p,bInit,vCurPos);
	DDV_UpdateHScrollVal(p,bInit,hCurPos);

	InvalRect(&rcP);
	RestorePort(temport);
}

/*****************************************************************************

  Function : DDV_WinMainResize()
  
  Purpose : Main Window Resize function 
  
  Parameters : w; handle of the parent window
  
  Return value : none 

*****************************************************************************/
extern void DDV_WinMainResize (WindoW w)
{		
    DdvMainWinPtr  mWin_d;
    
    mWin_d = (DdvMainWinPtr) GetObjectExtra(w);
    if(mWin_d == NULL) return;
    
    DDV_SetupWin (w,FALSE);
    DDV_Resize_DDV (mWin_d->hUpperPanel, FALSE);
    DDV_Resize_DDV (mWin_d->hLowerPanel, FALSE);
	Update();
}

/*****************************************************************************

Function: DDV_CleanupDDVPdata() & DDV_FreeDDVPdata()

Purpose: delete the data attached to the DDV panel 

Return value: none

*****************************************************************************/
static void DDV_FreeDDVPdata(DdvMSADataPtr dmdp,Uint2 userkey,Uint2 procid,
	Uint2 proctype)
{
ParaGPtr  pgp;
BioseqPtr bsp;
Int4      i;
Uint2     bsp_eID;

	if (!dmdp) return;
	/*before deleting the list of ParaG, I need to delete the user object
	containing the Msg Func*/
	if (userkey>0 && procid>0 && proctype>0) {
		for(i=0;i<dmdp->pgp_l.nBsp;i++){
			pgp=(ParaGPtr)(dmdp->pgp_l.TableHead[i]->data.ptrvalue);
			bsp=BioseqLockById(pgp->sip);
			if (bsp != NULL){
				bsp_eID=ObjMgrGetEntityIDForPointer((Pointer)bsp);
				BioseqUnlock(bsp);
				if (bsp_eID>0 && bsp_eID!=dmdp->entityID){
					ObjMgrFreeUserData(bsp_eID,procid,proctype,userkey);
				}
			}
		}
	}
	
	if (dmdp->pgp_l.TableHead) {
		DDV_DeleteDisplayList(&dmdp->pgp_l);
		dmdp->pgp_l.TableHead=NULL;
	}
	if (dmdp->pgp_l.DisplayVert) {
		UDV_FreeListParaG(&dmdp->pgp_l.DisplayVert);
		dmdp->pgp_l.DisplayVert=NULL;
	}
	if (dmdp->pgp_l.sabp) {
		CleanupBlocks(dmdp->pgp_l.sabp);
		dmdp->pgp_l.sabp=NULL;
	}
	if (dmdp->pgp_l.bspp) {
		dmdp->pgp_l.bspp=DDV_BspInfoDeleteList(dmdp->pgp_l.bspp);
		dmdp->pgp_l.bspp=NULL;
	}
	if (dmdp->pgp_l.RulerDescr) {
		ValNodeFreeData(dmdp->pgp_l.RulerDescr);
		dmdp->pgp_l.RulerDescr=NULL;
	}
	if (dmdp->pgp_l.entitiesTbl) {
		MemFree(dmdp->pgp_l.entitiesTbl);
		dmdp->pgp_l.entitiesTbl=NULL;
	}

  /* no rows, no cols */
  dmdp->pgp_l.LengthAli = 0;
  dmdp->pgp_l.nBsp = 0;
	
    if(dmdp->entityID != 0)
	    ObjMgrFreeUserData(dmdp->entityID,procid,proctype,userkey);
	dmdp->entityID=0;
	dmdp->itemID=0;
}

extern void DDV_CleanupDDVPdata_g (DdvMainPtr dmp)
{
ValNodePtr      vnp;

	/*SeqAlign Idx & ParaG List*/
	DDV_FreeDDVPdata(&(dmp->MSA_d),dmp->userkey,dmp->procid,dmp->proctype);
	/*graphical data*/
	DDV_InitDefSAPdispStyles(&(dmp->ddo));
  ClearUDV_mouse_select(&(dmp->ms));
	dmp->ms.Action_type=MS_ACTION_FEAT_NOTHING;
	dmp->procid=0;
	dmp->proctype=0;
	dmp->userkey=0;

    switch(dmp->dod.choice){
    case DDV_OPENTYPE_NOTRESP:
        break;
    case DDV_OPENTYPE_FILE:
        vnp=dmp->dod.vnp;
        while(vnp){
            ObjMgrFree(vnp->choice, (Pointer)vnp->data.ptrvalue);
            vnp=vnp->next;
        }
        ValNodeFree(dmp->dod.vnp);
        break;
    case DDV_OPENTYPE_SEP:
    case DDV_OPENTYPE_GI:
        if (dmp->dod.sep) SeqEntryFree(dmp->dod.sep);
        break;
    }
    memset(&(dmp->dod),0,sizeof(DdvOpenData));

	/*delete list of SeqAligns*/
	if (dmp->vnp_ali){
		dmp->vnp_ali=ValNodeFree(dmp->vnp_ali);
	}

	memset(&dmp->GrData.udv_vscrl,0,sizeof(UDVScrollData));
	memset(&dmp->GrData.udv_hscrl,0,sizeof(UDVScrollData));
	dmp->GrData.udv_panel.nTotLines=0;
	dmp->GrData.udv_panel.nTotCol=0;

	/*colors; deleted only by the autonomous DDV*/
/*	if (bRemoveBioseq && dmp->Globals.colorp){ 
		DDV_DeleteColorGlobal(dmp->Globals.colorp);*/
		dmp->Globals.colorp=NULL;
/*	}*/

}

/*WindoW callback*/
static void DDV_CleanupDDVPdata (GraphiC g, VoidPtr data)
{
DdvMainPtr dmp;

	if (data){
		dmp=(DdvMainPtr)data;
    if (dmp->bEditor) {
      DDE_FreeStack(dmp->dsp);
    }
		DDV_CleanupDDVPdata_g (dmp);
	}
}


static PaneL DDV_PanelInit(GrouP g, WindoW w,DdvMainWinPtr dmwp,
                           SAM_ViewGlobal *vgp, Boolean bEditor,
                           Nlm_PnlClckProc ClickProc, Int4 Width)
{
    PaneL  p;
    DdvMainPtr dmp;
    Int2 Margins;

    Margins=20*stdCharWidth;

    	/*init data blocks*/
	dmp=(DdvMainPtr)MemNew(sizeof(DdvMain));
	if (!dmp) return(FALSE);

	/*init default SeqAlign display options for standalon DDV and for
	DDV started from Cn3D*/
	if (dmwp->AutonomeViewer)
		dmp->MasterViewer=SAMVIEWDDV;
    if(vgp == NULL) {
		DDV_InitDefSAPdispStyles(&(dmp->ddo));
	}
    else if(vgp->MasterViewer == SAMVIEWCN3D) {
		DDV_InitCn3DSAPdispStyles(&(dmp->ddo));
		dmp->MouseMode=DDV_MOUSEMODE_SELECT;
	}

	dmp->ms.Action_type=MS_ACTION_FEAT_NOTHING;

    if(vgp == NULL)
        p = AutonomousPanel4 (g, 
            (Int2)2*((screenRect.right-screenRect.left)-Margins)/3, 
            (Int2)2*((screenRect.bottom-screenRect.top)-Margins)/Width, 
            DDV_DrawViewer,DDV_VScrlProc,DDV_HScrlProc,
            sizeof(DdvMainPtr),NULL,NULL);
    else
        p = AutonomousPanel4 (g, 
            (Int2)vgp->Rect.right - vgp->Rect.left, 
            (Int2)vgp->Rect.bottom - vgp->Rect.top, 
            DDV_DrawViewer,DDV_VScrlProc,DDV_HScrlProc,
            sizeof(DdvMainPtr),NULL,NULL);

    if(!p) return NULL;

    Nlm_SetSlateBorder ((SlatE) p, FALSE);
	SetPanelClick(p,ClickProc,
		DDV_DragProc,DDV_HoldProc,DDV_ReleaseProc);
	SetSlateChar ((SlatE) p, DDV_KeyboardProc);

	SetObjectExtra (p, (Pointer) dmp, (FreeProc)DDV_CleanupDDVPdata);

	dmp->hWndDDV=p;
	dmp->hParent=w;
	dmp->bEditor=bEditor;
	if (!dmp->bEditor){
		dmp->deri.curEditRow=(Int4)-1;
		dmp->deri.curMasterRow=(Int4)0;
	}
	UDV_InitFont(&(dmp->GrData.udv_font));
	SelectFont(dmp->GrData.udv_font.hFnt);
	UDV_FontDim(&(dmp->GrData.udv_font.cxChar),&(dmp->GrData.udv_font.cyChar));

	/*init the graphical elements of DDV*/
	DDV_InitGraphGlobal(dmp);
    return p;
}

/*****************************************************************************

Function: DDV_CreateViewerPanel()

Purpose: create the Viewer main panel

Return value: none

*****************************************************************************/
extern Boolean DDV_CreateViewerPanel(WindoW w,DdvMainWinPtr dmwp,
                                     SAM_ViewGlobal *vgp, Boolean bEditor)
{
PaneL  p, p2;
GrouP  g,g2,StatusGroup,StatusGroupLower;
ButtoN gotoBtn;
TexT   gotoRowTxt,gotoColTxt;
PrompT info, infoLower;
FonT   hFnt;
Int2   cxChar,cyChar;


#ifdef WIN_MAC
		hFnt = ParseFont ("Monaco, 9");
#endif

#ifdef WIN_MSWIN
		hFnt = ParseFont ("Courier, 7");
#endif

#ifdef WIN_MOTIF
		hFnt = ParseFont ("fixed, 12");
#endif

	SelectFont (hFnt);
	cxChar=MaxCharWidth();
	cyChar=LineHeight();
	g = HiddenGroup (w, 5, 0, NULL);                  
	gotoBtn = PushButton (g, "Go to:",UDV_GotoBtnProc);
	StaticPrompt (g, "row:", 0, 3*cyChar/2, hFnt, 'l');
	gotoRowTxt = DialogText (g, "0", (Int2)4, NULL);
	StaticPrompt (g, "col:", 0, 3*cyChar/2, hFnt, 'l');
	gotoColTxt = DialogText (g, "0", (Int2)4, NULL);
	
	g = HiddenGroup (w, 1, 2, NULL);
    g2 = HiddenGroup(g,1,1,NULL);
    DDV_SetupMenus(w,dmwp->UseNetwork,bEditor, vgp);
    p=DDV_PanelInit(g2, w, dmwp, vgp, bEditor, DDV_ClickProcUpper, 4);
    if(!p) return FALSE;
    dmwp->hUpperPanel = p; /*handle of the upper DDV panel (the multiple)*/
	dmwp->hWndDDV=p; /* default focus */

	StatusGroup = HiddenGroup (g, 1, 1, NULL);
	info = StaticPrompt (StatusGroup, "Ready !", 0, 3*cyChar/2, hFnt, 'l');

    g2 = HiddenGroup (g, 1, 1, NULL);
    p2=DDV_PanelInit(g2, w, dmwp, vgp, bEditor, DDV_ClickProcLower, 6);
    if(!p) return FALSE;
    dmwp->hLowerPanel = p2; /*handle of the DDV panel (the pairwise) */

	StatusGroupLower = HiddenGroup (g, 1, 0, NULL);
    infoLower = StaticPrompt (StatusGroupLower, "", 0, 3*cyChar/2, hFnt, 'l');
    
    AlignObjects(ALIGN_RIGHT, (HANDLE) p,(HANDLE) StatusGroup,(HANDLE) p2,
        (HANDLE) StatusGroupLower, NULL);

	if (gotoBtn==NULL || gotoRowTxt==NULL || 
		gotoColTxt==NULL || info==NULL || infoLower == NULL) 
		return(FALSE);

	SetWindowTimer (w, DDV_TimerProc);

	dmwp->gotoBtn=gotoBtn;
	dmwp->gotoValRow=gotoRowTxt;
	dmwp->gotoValCol=gotoColTxt;
	dmwp->StatusGroup=StatusGroup;
    dmwp->StatusGroupLower=StatusGroupLower;
	dmwp->InfoPanel=info;
    dmwp->UpperInfoPanel = info;
    dmwp->LowerInfoPanel = infoLower;

    DDV_SetMenuFocus(w);
	Disable(dmwp->gotoBtn);
	Disable(dmwp->gotoValRow);
	Disable(dmwp->gotoValCol);
	/*init graph objects: call UDV functions ...*/
    

	return(TRUE);
}

/*******************************************************************************

  Function : DDV_WinMainProgQuit()
  
  Purpose : end of prog 
  
  Parameters : w; main window handle
  
  Return value : none 

*******************************************************************************/
extern void DDV_WinMainProgQuit(WindoW w)
{
	QuitProgram();
}

/*******************************************************************************

  Function : QuitProc()
  
  Purpose : bye-bye procedure ! 
  
  Parameters : i; menu item which has called this function
  
  Return value : none 

*******************************************************************************/
static void QuitProc(IteM i)
{
WindoW  hWinMain;

  hWinMain=(WindoW)ParentWindow(i);
  if (!hWinMain) return;
  DDV_SlaveQuit(hWinMain);
}

NLM_EXTERN void DDV_Save(ButtoN g) {
/*----------------------------------------------------------------------------
*  save edits, quit pop-up, quit DDE
*---------------------------------------------------------------------------*/
  WindoW         hDialog;
  WindoW         hWinMain;
  DdvMainWinPtr  mWin_d;
  DdvMainPtr     dmp;

  /* quit pop-up */
	hDialog = (WindoW)ParentWindow(g);
	if (!hDialog) return;
	Remove(hDialog);

  /* save the edits */
  hWinMain = (WindoW) GetObjectExtra(hDialog);
  mWin_d = (DdvMainWinPtr) GetObjectExtra(hWinMain);
	dmp = (DdvMainPtr) GetObjectExtra(mWin_d->hWndDDV);
  DDV_SaveEdits(dmp, TRUE);

  /* quit DDE */
  if (mWin_d->AutonomeViewer)
    QuitProgram(); /*standalone DDV*/
  else
    Remove(hWinMain);/*slave viewer : just kill the main window*/
}


NLM_EXTERN void DDV_NoSave(ButtoN g) {
/*----------------------------------------------------------------------------
*  quit pop-up, quit DDE
*---------------------------------------------------------------------------*/
  WindoW         hDialog;
  WindoW         hWinMain;
  DdvMainWinPtr  mWin_d;

  /* quit the pop-up */
	hDialog = (WindoW)ParentWindow(g);
	if (!hDialog) return;
	Remove(hDialog);

  /* quit DDE */
  hWinMain = (WindoW) GetObjectExtra(hDialog);
  mWin_d = (DdvMainWinPtr) GetObjectExtra(hWinMain);
  if (mWin_d->AutonomeViewer)
    QuitProgram(); /*standalone DDV*/
  else
    Remove(hWinMain);/*slave viewer : just kill the main window*/
}


NLM_EXTERN void DDV_Cancel(ButtoN g) {
/*----------------------------------------------------------------------------
*  quit pop-up
*---------------------------------------------------------------------------*/
  WindoW  hDialog;

  /* quit the pop-up */
	hDialog = (WindoW)ParentWindow(g);
	if (!hDialog) return;
	Remove(hDialog);
}


/*******************************************************************************

  Function : DDV_CloseData()
  
  Purpose : bye-bye data, i.e. use that function to delete all the data
            loaded within DDV.
  
  Parameters : mWin_d; main data block
  
  Return value : none 

*******************************************************************************/
extern void DDV_CloseData(DdvMainWinPtr mWin_d,Boolean bFinalExit)
{

	WatchCursor();
	/*dmp=(DdvMainPtr)GetObjectExtra(mWin_d->hWndDDV);*/
	
	/*DDV_CleanupDDVPdata_g(dmp);*/
	if (!bFinalExit)
		DDV_SetupWin (mWin_d->hWndMain,TRUE);
	ArrowCursor();
}


NLM_EXTERN void DDV_FileCloseIt(WindoW hWinMain) {
/*----------------------------------------------------------------------------
*  part of DDV_FileClose
*---------------------------------------------------------------------------*/
  DdvMainPtr    dmp;
  DdvMainWinPtr mWin_d;
  RecT          rc;

	mWin_d = (DdvMainWinPtr) GetObjectExtra((Handle)hWinMain);
	if (mWin_d==NULL) return;

	/* delete data */
	dmp=(DdvMainPtr)GetObjectExtra(mWin_d->hWndDDV);
	DDV_CleanupDDVPdata_g(dmp);
	DDV_CloseData(mWin_d,FALSE);
	DDV_EnableGotoTBItems(hWinMain,FALSE);

	/* blank the panel that's closed */
	mWin_d->Show_logo=FALSE;
  ObjectRect(mWin_d->hUpperPanel, &rc);
  InvalRect(&rc);
  ObjectRect(mWin_d->hLowerPanel, &rc);
  InvalRect(&rc);

  Update();
}


/*******************************************************************************

  Function : DDV_FileClose()
  
  Purpose : close a SeqAlign 
  
  Parameters : i; menu item which has called this function
  
  Return value : none 

*******************************************************************************/
static void DDV_FileClose(IteM i)
{
  WindoW  hWinMain;

	hWinMain=(WindoW)ParentWindow(i);
	if (!hWinMain) return;

  DDV_FileCloseIt(hWinMain);
  return;
}


#ifdef DEBUG_DDV
static Uint1 DDV_GetCurrentUAGAPStyle(MsaParaGPopListPtr mpplp)
{
Int4          j;
ValNodePtr    vnp,vnp2;
ParaGPtr      pgp;
MsaTxtDispPtr mtdp;

	if (mpplp->TableHead){
		for(j=0;j<mpplp->nBsp;j++){
			vnp=mpplp->TableHead[j];
			while(vnp){
				pgp=(ParaGPtr)vnp->data.ptrvalue;
				if (pgp){
					vnp2=pgp->ptxtList;
					while(vnp2){
						mtdp=(MsaTxtDispPtr)vnp2->data.ptrvalue;
						if (mtdp->IsUnAligned && mtdp->IsGap){
							return(mtdp->TextStyle);
						}
						vnp2=vnp2->next;
					}
				}
				vnp=vnp->next;
			}
		}
	}
	return(0);
}

static void DDV_SetCurrentUAGAPStyle(MsaParaGPopListPtr mpplp,Uint1 UATextStyle)
{
Int4          j;
ValNodePtr    vnp,vnp2;
ParaGPtr      pgp;
MsaTxtDispPtr mtdp;

	if (mpplp->TableHead){
		for(j=0;j<mpplp->nBsp;j++){
			vnp=mpplp->TableHead[j];
			while(vnp){
				pgp=(ParaGPtr)vnp->data.ptrvalue;
				if (pgp){
					vnp2=pgp->ptxtList;
					while(vnp2){
						mtdp=(MsaTxtDispPtr)vnp2->data.ptrvalue;
						if (mtdp->IsUnAligned && mtdp->IsGap){
							mtdp->TextStyle=UATextStyle;
						}
						vnp2=vnp2->next;
					}
				}
				vnp=vnp->next;
			}
		}
	}
}


/*******************************************************************************

  Function : DDV_ShowHideUnAlignGap()
  
  Purpose : use or not the ~ char to display an unalign gap
  
  Return value : none 

*******************************************************************************/
static void DDV_ShowHideUnAlignGap(IteM i)
{
static Boolean bFirst=TRUE;
static Uint1 UA_TextStyle;
DdvMainWinPtr mWin_d;
DdvMainPtr 	  dmp;
WindoW		  hWinMain,hDispDlg;

	/*get some data*/
	hWinMain=(WindoW)ParentWindow(i);
	if (hWinMain==NULL) return;
	mWin_d = (DdvMainWinPtr) GetObjectExtra (hWinMain);
	if (mWin_d==NULL) return;
	if (mWin_d->hWndDDV==NULL) return;
	dmp = (DdvMainPtr) GetObjectExtra(mWin_d->hWndDDV);
	if (dmp==NULL) return;

	if (bFirst){
		UA_TextStyle=DDV_GetCurrentUAGAPStyle(&(dmp->MSA_d.pgp_l));
		bFirst=FALSE;
	}
	
	if (UA_TextStyle==MSA_TXT_STYLE_UAGAP)
		UA_TextStyle=MSA_TXT_STYLE_GAP;
	else
		UA_TextStyle=MSA_TXT_STYLE_UAGAP;
		
	DDV_SetCurrentUAGAPStyle(&(dmp->MSA_d.pgp_l),UA_TextStyle);
}
#endif

/*******************************************************************************

  Function : DDV_DispStyles_OkProc()
  
  Purpose : manage ok button of the  Display Styles dialog box 
  
  Parameters : g; button
  
  Return value : none 

*******************************************************************************/
static void DDV_DispStyles_OkProc(ButtoN g)
{
WindoW              hDispDlg;
DdvMainWinPtr       mWin_d;
DdvDispStylesMSGPtr dsmp;
DdvMainPtr          dmp;
DDVUpdateLayoutDataPtr dumdp;
DDVUpdateMSGPtr     dump;
Char                szBuf[15]={""};
Int2                value;
Char                title[10]={""};
Int4                j;

	hDispDlg=(WindoW)ParentWindow(g);
	Hide(hDispDlg);
	if (!hDispDlg) return;
	
	dsmp = (DdvDispStylesMSGPtr) GetObjectExtra (hDispDlg);

	if (dsmp==NULL) return;
	
	mWin_d = (DdvMainWinPtr) GetObjectExtra (dsmp->hWinMain);
	dmp = (DdvMainPtr) GetObjectExtra(mWin_d->hWndDDV);

	dumdp=(DDVUpdateLayoutDataPtr)MemNew(sizeof(DDVUpdateLayoutData));
	if (!dumdp){
		MemFree(dsmp);
		Remove(hDispDlg);
		/*todo : add user message to inform for error*/
		return;
	}

	dump=(DDVUpdateMSGPtr)MemNew(sizeof(DDVUpdateMSG));
	if (!dump){
		MemFree(dsmp);
		MemFree(dumdp);
		Remove(hDispDlg);
		/*todo : add user message to inform for error*/
		return;
	}

	/*check box for color display*/
	dumdp->bUseColors=GetStatus((Handle)dsmp->chk2);
	/*show left tail check box*/
	dumdp->ShowLeftTail=GetStatus((Handle)dsmp->chk3);
	/*show right tail check box*/
	dumdp->ShowRightTail=GetStatus((Handle)dsmp->chk4);
	
	if (Enabled((Handle)dsmp->g1)){
		/*size of the spacer*/
		GetTitle(dsmp->edit1, title, 9);
		value=(Int2)atoi(title);
		if (value<1)
			dumdp->SpacerSize=1;
		else if (value>20){
			dumdp->SpacerSize=20;
		}
		else dumdp->SpacerSize=(Uint1)value;
		/*get the values back from the user*/
		value=GetValue((Handle)dsmp->g2);
		if (value==1){/*use spacer ?*/
			dumdp->DispDiscStyle=MSA_TXT_STYLE_1;
		}
		else{/*use justification*/
			dumdp->DispDiscStyle=MSA_TXT_STYLE_2;
			value=GetValue((Handle)dsmp->g5);
			switch(value){
				case 1:
					dumdp->DiscJustification=DISP_JUST_LEFT;
					break;
				case 2:
					dumdp->DiscJustification=DISP_JUST_RIGHT;
					break;
				case 3:
					dumdp->DiscJustification=DISP_JUST_CENTER;
					break;
				case 4:
					dumdp->DiscJustification=DISP_JUST_SPLIT;
					break;
			}
		}
	}
	
	/*sequence list... something selected?*/
	/*set the ruler styles*/
	for(j=0;j<dmp->MSA_d.pgp_l.nBsp;j++){
		if (GetItemStatus (dsmp->BspNames, (Int2)(j+1))){/*selected ?*/
			if(!dumdp->SeqList){
				dumdp->SeqList=(Int4Ptr)MemNew(sizeof(Int4));
				if (!dumdp->SeqList) break;
				dumdp->SeqList[0]=j+1;
				dumdp->nSeq++;
			}
			else{
				dumdp->SeqList=(Int4Ptr)MemExtend(dumdp->SeqList,
						(dumdp->nSeq+1)*sizeof(Int4),dumdp->nSeq*sizeof(Int4));
				if (dumdp->SeqList==NULL){
					dumdp->nSeq=0;
					break;
				}
				dumdp->SeqList[dumdp->nSeq]=j+1;
				dumdp->nSeq++;
			}
		}
	}
	if (dumdp->nSeq>0){
		value=GetValue((Handle)dsmp->g12);
		switch(value){
			case 1:
				dumdp->RulerStyle=SCALE_POS_TOP;
				break;
			case 2:
				dumdp->RulerStyle=SCALE_POS_NONE;
				break;
		}			
	}
	
	dumdp->ddv_panel=mWin_d->hWndDDV;

	/*send a OM_MSG_UPDATE message*/
	dump->type=UPDATE_TYPE_LAYOUT;
	dump->data=(Pointer)dumdp;
	ObjMgrSendProcMsg(OM_MSG_UPDATE, dmp->MSA_d.entityID,dmp->MSA_d.itemID,
			OBJ_SEQALIGN,0,0,(Pointer)dump);
	
	/*close the dlg box*/
	Remove(hDispDlg);
}

/*******************************************************************************

  Function : DDV_DispStyles_cancelProc()
  
  Purpose : manage cancel button of the Display Styles dialog box 
  
  Parameters : g; button
  
  Return value : none 

*******************************************************************************/
static void DDV_DispStyles_cancelProc(ButtoN g)
{
WindoW			    hDispDlg;
DdvDispStylesMSGPtr dsmp;

	hDispDlg=(WindoW)ParentWindow(g);

	if (!hDispDlg) return;

	dsmp = (DdvDispStylesMSGPtr) GetObjectExtra (hDispDlg);

	if (dsmp==NULL) return;

	Remove(hDispDlg);
}


/*******************************************************************************

  Function : DDV_DispStyles_SelAllProc()
  
  Purpose : manage SelAll BSP button of the Display Styles dialog box 
  
  Parameters : g; button
  
  Return value : none 

*******************************************************************************/
static void DDV_DispStyles_SelAllProc(ButtoN g)
{
WindoW			hDispDlg;
DdvMainWinPtr       mWin_d;
DdvDispStylesMSGPtr dsmp;
DdvMainPtr          dmp;
Int4                j;
	
	hDispDlg=(WindoW)ParentWindow(g);
	if (!hDispDlg) return;
	dsmp = (DdvDispStylesMSGPtr) GetObjectExtra (hDispDlg);
	if (dsmp==NULL) return;
	mWin_d = (DdvMainWinPtr) GetObjectExtra (dsmp->hWinMain);
	dmp = (DdvMainPtr) GetObjectExtra(mWin_d->hWndDDV);
	
	for(j=0;j<dmp->MSA_d.pgp_l.nBsp;j++){
		SetItemStatus (dsmp->BspNames, (Int2)(j+1), TRUE);	
	}
}

/*******************************************************************************

  Function : DDV_DispStyles_SelNoneProc()
  
  Purpose : manage Sel None BSP button of the Display Styles dialog box 
  
  Parameters : g; button
  
  Return value : none 

*******************************************************************************/
static void DDV_DispStyles_SelNoneProc(ButtoN g)
{
WindoW			hDispDlg;
DdvMainWinPtr       mWin_d;
DdvDispStylesMSGPtr dsmp;
DdvMainPtr          dmp;
Int4                j;
	
	hDispDlg=(WindoW)ParentWindow(g);
	if (!hDispDlg) return;
	dsmp = (DdvDispStylesMSGPtr) GetObjectExtra (hDispDlg);
	if (dsmp==NULL) return;
	mWin_d = (DdvMainWinPtr) GetObjectExtra (dsmp->hWinMain);
	dmp = (DdvMainPtr) GetObjectExtra(mWin_d->hWndDDV);
	
	for(j=0;j<dmp->MSA_d.pgp_l.nBsp;j++){
		SetItemStatus (dsmp->BspNames, (Int2)(j+1), FALSE);	
	}
}


/*******************************************************************************

  Function : DDV_DispStyles_DiscSetCtrlVal()
  
  Purpose : set the values for the ctrl in the group "disc. align options" 
  
  Return value : none 

*******************************************************************************/
static void DDV_DispStyles_DiscSetCtrlVal(DdvDispStylesMSGPtr dsmp,
	DDV_Disp_OptPtr ddop)
{
Char szBuf[15]={""};


	Enable(dsmp->g1);

	if (ddop->DispDiscStyle==MSA_TXT_STYLE_1)
		SetValue(dsmp->g2,1);
	else
		SetValue(dsmp->g2,2);

	sprintf(szBuf,"%i",ddop->SpacerSize);
	SetTitle(dsmp->edit1,szBuf);/*spacer size*/

	if (ddop->DiscJustification>0)
		SetValue(dsmp->g5,(Int2)(ddop->DiscJustification-DISP_JUST_LEFT+1));
	else
		SetValue(dsmp->g5,1);
}

/*******************************************************************************

  Function : DDV_DeselectItem()
  
  Purpose : Deselects everything 
  
  Return value : none 

*******************************************************************************/
static void  DDV_DeselectItem(IteM i)
{
    DdvMainWinPtr mWin_d;
    DdvMainPtr 	  dmp;
    WindoW		  hWinMain;

	hWinMain=(WindoW)ParentWindow(i);
	if (hWinMain==NULL) return;
	mWin_d = (DdvMainWinPtr) GetObjectExtra (hWinMain);
	if (mWin_d==NULL) return;
	if (mWin_d->hWndDDV==NULL) return;
	dmp = (DdvMainPtr) GetObjectExtra(mWin_d->hWndDDV);
	if (dmp==NULL) return;

    ObjMgrDeSelectAll();
    ObjMgrSendProcMsg(OM_MSG_UPDATE, dmp->MSA_d.entityID,dmp->MSA_d.itemID,
        OBJ_SEQALIGN,0,0,NULL);
    ObjMgrSendMsg(OM_MSG_MOUSEUP, dmp->MSA_d.entityID, dmp->MSA_d.itemID,
        OBJ_SEQALIGN);
}

/*******************************************************************************

  Function : DDV_DisplayStyles()
  
  Purpose : create the Display Styles dialog box 
  
  Return value : none 

*******************************************************************************/
static void DDV_DisplayStyles(IteM i)
{
DdvMainWinPtr mWin_d;
DdvMainPtr 	  dmp;
WindoW		  hWinMain,hDispDlg;
GrouP         g0,g1,g2,g3,g4,g5,g6,g7,g8,g9,g10,g11,g12,g13;
LisT          BspNames;
ButtoN        chk2,chk3,chk4;
TexT          edit1;
DdvDispStylesMSGPtr  dsmp;
SeqIdPtr      sip;
Char          szAccess[21];
Char          szBuf[10];
Int4          j;
ValNodePtr    vnp;
ParaGPtr      pgp;

	/*get some data*/
	hWinMain=(WindoW)ParentWindow(i);
	if (hWinMain==NULL) return;
	mWin_d = (DdvMainWinPtr) GetObjectExtra (hWinMain);
	if (mWin_d==NULL) return;
	if (mWin_d->hWndDDV==NULL) return;
	dmp = (DdvMainPtr) GetObjectExtra(mWin_d->hWndDDV);
	if (dmp==NULL) return;

	/*init MsgStruct; to be used when user clicks on [OK]*/
	dsmp=(DdvDispStylesMSGPtr)MemNew(sizeof(DdvDispStylesMSG));	
	if (!dsmp) return;
	
	/*DlgBox window*/
	hDispDlg=MovableModalWindow(-30, -20,  -10,  -10, 
				"DDV - display options",  NULL);
	if (!hDispDlg){
		MemFree(dsmp);
		return;
	}

	SetObjectExtra (hDispDlg, (Pointer) dsmp, StdCleanupExtraProc);
/*
    the dlg controls

    g6                 main group for all the dlg; put all the controls there
     |->chk2           check for color display 
     |
     |->g0             main group for "Disc. SeqAlign display options"
     |   |->g1 
     |   |   |->g2     radio group 1 (spacer/seq.)
     |   |   |->g3
     |   |       |->g4 TexT ctrl, spacer size
     |   |       |->g5 radio group 2 (justification)
     |   |->g13        check btns for tails
     |        
     |->g8             main group for "Bioseq ruler"
     |   |->g9
     |       |->BspNames BSP names listbox
     |       |->g10     the controls (radio & buttons)
     |            |->g11
     |            |->g12
     |->g7            (ok/cancel buttons)
*/
	/*top-level group*/
	g6=HiddenGroup(hDispDlg, -1, 0,NULL);
	
	chk2=CheckBox(g6,"Use a color display",NULL);

	/*Disc. SeqAlign display options group - start*/
	g0=NormalGroup(g6, -1, 0, "Disc. SeqAlign display options",NULL, NULL);
	g1=HiddenGroup(g0, 2, 0,NULL);
	g2=HiddenGroup(g1,0,2,NULL);
	RadioButton(g2,"spacer");
	RadioButton(g2,"sequence");
	g3=HiddenGroup(g1,0,2,NULL);
	g4=HiddenGroup(g3,2,0,NULL);
	edit1=DialogText(g4,NULL,6,NULL);
	g5=HiddenGroup(g3,0,4,NULL);
	RadioButton(g5,"Left");
	RadioButton(g5,"Right");
	RadioButton(g5,"Center");
	RadioButton(g5,"Split");
	g13=HiddenGroup(g0,0,2,NULL);
	chk3=CheckBox(g13,"Show left tail",NULL);
	chk4=CheckBox(g13,"Show right tail",NULL);
	/*Disc. SeqAlign display options group - end*/

	/*Bioseq list - start*/
	g8=NormalGroup(g6, -1, 0, "Bioseq ruler",NULL, NULL);
	g9=HiddenGroup(g8,0,2,NULL);
	BspNames=MultiList(g9,6,6,NULL);
	if (dmp->MSA_d.pgp_l.TableHead){
		for(j=0;j<dmp->MSA_d.pgp_l.nBsp;j++){
			vnp=dmp->MSA_d.pgp_l.TableHead[j];
			pgp=(ParaGPtr)vnp->data.ptrvalue;
			if (pgp->sip){
				sip = SeqIdFindBest(pgp->sip, SEQID_GENBANK);
				if (!sip)
					sip = SeqIdFindBest(pgp->sip, 0);
				SeqIdWrite(sip, szAccess,PRINTID_TEXTID_ACCESSION, 20);   
				ListItem(BspNames,szAccess);
			}
		}
	}
	g10=HiddenGroup(g9,2,0,NULL);
	g11=NormalGroup(g10, 2, 0, "Auto select",NULL, NULL);
	PushButton (g11, "All", DDV_DispStyles_SelAllProc);
	PushButton (g11, "None", DDV_DispStyles_SelNoneProc);
	g12=NormalGroup(g10, 3, 0, "Ruler Style",NULL, NULL);
	RadioButton(g12,"Top");
	RadioButton(g12,"None");
	SetValue(g12,2);
	/*Bioseq list - end*/

	/*ok - cancel group - start*/
	g7=HiddenGroup(g6, 2, 0,NULL);
	PushButton (g7, "Ok", DDV_DispStyles_OkProc);
	PushButton (g7, "Cancel", DDV_DispStyles_cancelProc);
	/*ok - cancel group - end*/
	
	/*copy data*/
	dsmp->hWinMain=hWinMain;
	dsmp->chk2=chk2;
	dsmp->chk3=chk3;
	dsmp->chk4=chk4;
	dsmp->g1=g1;
	dsmp->g2=g2;
	dsmp->g5=g5;
	dsmp->g12=g12;
	dsmp->edit1=edit1;
	dsmp->BspNames=BspNames;

	/*build the dlgbox*/
	RealizeWindow(hDispDlg);
	Show(hDispDlg);

	/*color display check box*/
	SetStatus(chk2,dmp->ddo.bUseColors);
	/*show left tail check box*/
	SetStatus(chk3,dmp->ddo.ShowLeftTail);
	/*show right tail check box*/
	SetStatus(chk4,dmp->ddo.ShowRightTail);
	/*spacer size*/
	sprintf(szBuf,"%i",dmp->ddo.SpacerSize);
	SetTitle(dsmp->edit1, szBuf);

	/*depending on the SAP type, init some controls*/
  DDV_DispStyles_DiscSetCtrlVal(dsmp,&(dmp->ddo));
/*  
	if (AlnMgrIsSAPDiscAli(dmp->MSA_d.pgp_l.sap)){
		DDV_DispStyles_DiscSetCtrlVal(dsmp,&(dmp->ddo));
	}
	else{
		Disable(g1);
	}
*/
}


static void SelectJustify(ChoicE i) {
/*----------------------------------------------------------------------------
*  handle the Justify left/right/center selection
*---------------------------------------------------------------------------*/
  DdvMainWinPtr  mWin_d;
  DdvMainPtr     dmp;
  WindoW         hWinMain;
  Int2           Value;

	hWinMain=(WindoW)ParentWindow(i);
	if (!hWinMain) return;
	mWin_d = (DdvMainWinPtr) GetObjectExtra(hWinMain);
	if (mWin_d==NULL) return;
	dmp=(DdvMainPtr)GetObjectExtra(mWin_d->hWndDDV);
	if (dmp==NULL) return;
	if (!dmp->MSA_d.pgp_l.TableHead) return;

  Value = GetValue(i);
  switch(Value) {
    case 1:
      DDE_LeftJustify(dmp->dsp, TRUE);
      break;
    case 2:
      DDE_RightJustify(dmp->dsp, TRUE);
      break;
    case 3:
      DDE_CenterJustify(dmp->dsp, TRUE);
      break;
    default:
      break;
  }

  /* shut off the selection */
  SetValue(mWin_d->MainMenu.Justify, 0);

  DDV_ReDraw(dmp);
  return;
}


/*******************************************************************************

  Function : SelectMouseMode()
  
  Purpose : modification of the mouse mode
  
  Parameters :  i; menu item
  
  Return value : none 

*******************************************************************************/
static void SelectMouseMode(ChoicE i)
{
Int2 			value;
DdvMainWinPtr 	mWin_d;/*program data*/
DdvMainPtr      dmp;
WindoW			hWinMain;
Int4            from_col,to_col,from_row,to_row,bsp_pos;
Char            szAccess[21];


	hWinMain=(WindoW)ParentWindow(i);
	if (!hWinMain) return;
	mWin_d = (DdvMainWinPtr) GetObjectExtra (hWinMain);
	if (mWin_d==NULL) return;
	dmp=(DdvMainPtr)GetObjectExtra(mWin_d->hWndDDV);
	if (dmp==NULL) return;
	if (!dmp->MSA_d.pgp_l.TableHead) return;
	
	/*get the disp coord range*/
	DDV_GetCurrentDispRange(dmp->hWndDDV,&(dmp->GrData),
			dmp->MSA_d.pgp_l.LengthAli,&from_col,&to_col,
			&from_row,&to_row);
	/*switch to one-bsed value because dmp->dci values 
	  are one-based values*/
	from_row++;
	to_row++;
	
	value=GetValue(i);
	switch(value){
		case 1:/*query mode*/
      dmp->MouseMode=DDV_MOUSEMODE_QUERY;
      goto Continue;
		case 2:/*select mode*/
      dmp->MouseMode=DDV_MOUSEMODE_SELECT;
      goto Continue;
    case 3:
      dmp->MouseMode=DDV_MOUSEMODE_SELECT_ONE_ROW;

Continue:
      dmp->dci.old_row=dmp->dci.new_row;
			dmp->dci.old_col=dmp->dci.new_col;
			dmp->dci.new_row=(Int4)-1;
			dmp->dci.new_col=(Int4)-1;
			/*hide the caret from old coordinates*/
			if (dmp->dci.old_col>=from_col && dmp->dci.old_col<=to_col &&
				dmp->dci.old_row>=from_row && dmp->dci.old_row<=to_row){
				DDV_InvalRegion(dmp->hWndDDV,&(dmp->GrData),
					_max_(0,dmp->dci.old_col-1),dmp->dci.old_col+1,
					dmp->dci.old_row,FALSE);
			}
			SetTitle(mWin_d->InfoPanel,"Ready !");
			break;
		case 4:/*edit mode*/
			dmp->MouseMode=DDV_MOUSEMODE_EDIT;
			dmp->dci.new_row=dmp->dci.old_row;
			dmp->dci.new_col=dmp->dci.old_col;
			/*show the caret from new coordinates*/
			if (dmp->dci.new_col>=from_col && dmp->dci.new_col<=to_col &&
				dmp->dci.new_row>=from_row && dmp->dci.new_row<=to_row){
				DDV_InvalRegion(dmp->hWndDDV,&(dmp->GrData),
					_max_(0,dmp->dci.new_col-1),dmp->dci.new_col+1,
					dmp->dci.new_row,FALSE);
			}
			DDV_GetSeqNameGivenRow(dmp->MSA_d.pgp_l.TableHead, 
				dmp->dci.new_row,szAccess);
			bsp_pos=DDV_GetBspCoordGivenPgpList(
				dmp->MSA_d.pgp_l.TableHead[dmp->dci.new_row-1],
				dmp->dci.new_col);
			if (bsp_pos!=(Int4)-1){
				DDV_DispPositionInStatus(mWin_d->InfoPanel, 
					bsp_pos+1,dmp->dci.new_row,szAccess);
						/*+1 : switch to one-based value*/
			}
			break;
	}
}

static void DDV_ShowLowerPanel(IteM i)
{
    DdvMainWinPtr 	mWin_d;/*program data*/

    mWin_d = (DdvMainWinPtr) GetObjectExtra((Handle)i);
    if(!mWin_d) return;
    DDV_SetupWin (mWin_d->hWndMain,FALSE);
}

/*******************************************************************************

  Function : DDV_SetMenuFocus()
  
  Purpose : Set menu item values depending on which panel has focus 
  
  Parameters : 	w; handle of the dialog box
				
  Return value : none 

*******************************************************************************/

NLM_EXTERN void DDV_SetMenuFocus(WindoW w)
{
DdvMainWinPtr 	mWin_d;/*program data*/
DdvMainPtr      dmp;

	mWin_d = (DdvMainWinPtr) GetObjectExtra((Handle)w);

	if (!mWin_d) return;
    dmp = (DdvMainPtr) GetObjectExtra(mWin_d->hWndDDV);

    switch(dmp->MouseMode){
		case DDV_MOUSEMODE_QUERY:
			SetValue(mWin_d->MainMenu.MouseMode,1);
			break;
		case DDV_MOUSEMODE_SELECT:
            SetValue(mWin_d->MainMenu.MouseMode,2);
            break;
        case DDV_MOUSEMODE_SELECT_ONE_ROW:
            SetValue(mWin_d->MainMenu.MouseMode,3);
            break;
        case DDV_MOUSEMODE_EDIT:
            SetValue(mWin_d->MainMenu.MouseMode,4);
            break;
    }

}

/*******************************************************************************

  Function : DDV_SetupMenus()
  
  Purpose : create the menu of DD-Viewer 
  
  Parameters : 	w; handle of the dialog box
  				isID1Ok;TRUE if ID1 is enabled
  				bEditor; true = show editor's commands
				
  Return value : none 

*******************************************************************************/
extern void  DDV_SetupMenus(WindoW w,Boolean isID1Ok,Boolean bEditor,
                            SAM_ViewGlobal *vgp)
{
MenU			m,s,s2;/*temp variable*/
DdvMainWinPtr 	mWin_d;/*program data*/
Boolean* pNoGaps;
Boolean  NoGaps;

	mWin_d = (DdvMainWinPtr) GetObjectExtra((Handle)w);

	if (!mWin_d) return;

	/*File menu*/
	m=PulldownMenu(w,"File");
	mWin_d->MainMenu.File=m;

	if (mWin_d->AutonomeViewer){/*available only for the Auntonomous viewer*/
		s=SubMenu(m,"Open from ");
		mWin_d->MainMenu.FileOpen=CommandItem(s,"a local file...",
				DDV_OpenFile);
		mWin_d->MainMenu.EntrezOpen=CommandItem(s,"the network...",
				DDV_OpenNetwork);
		if (isID1Ok==FALSE) Disable(mWin_d->MainMenu.EntrezOpen);

		mWin_d->MainMenu.FileClose=CommandItem(m,"Close",DDV_FileClose);
		SeparatorItem(m);
		mWin_d->MainMenu.FileExport=CommandItem(m,"Export...",DDV_ExportTexte);
		s=SubMenu(m,"Import ");
		mWin_d->MainMenu.ImportSeq=CommandItem(s,"a single sequence",DDV_ImportBioseq);
		mWin_d->MainMenu.ImportNucSeqAlign=CommandItem(s,"a nuc. sequence alignment",DDV_ImportNucSeqAlign);
		mWin_d->MainMenu.ImportProtSeqAlign=CommandItem(s,"a prot. sequence alignment",DDV_ImportProtSeqAlign);
		SeparatorItem(m);
		VSMAddToMenu(m, VSM_DESKTOP);
		SeparatorItem(m);
	}
	else{
        mWin_d->MainMenu.FileExport=CommandItem(m,"Export...",DDV_ExportTexte);
        /* for editor allow save of edits */
        if (bEditor) {
            mWin_d->MainMenu.SaveEdits=CommandItem(m,"Save",DDV_CallSaveEdits);
        }
        SeparatorItem(m);
    }
	mWin_d->MainMenu.QuitViewer=CommandItem(m,"Quit/Q",QuitProc);

	/*Edit Menu*/	
	if (bEditor){
		m=PulldownMenu(w,"Edit");
		mWin_d->MainMenu.Edit=m;
/*
		mWin_d->MainMenu.DeleteRow=CommandItem(m,"Delete row", DDV_DeleteRow);
		mWin_d->MainMenu.MoveRowLeft=CommandItem(m,"Move row to the left", DDV_MoveRowLeft);
		mWin_d->MainMenu.MoveRowRight=CommandItem(m,"Move row to the right", DDV_MoveRowRight);
    mWin_d->MainMenu.LeftBoundaryLeft=CommandItem(m,"Move left boundary left", DDV_MoveLeftBoundaryLeft);
    mWin_d->MainMenu.LeftBoundaryRight=CommandItem(m,"Move left boundary right", DDV_MoveLeftBoundaryRight);
    mWin_d->MainMenu.RightBoundaryLeft=CommandItem(m,"Move right boundary left", DDV_MoveRightBoundaryLeft);
    mWin_d->MainMenu.RightBoundaryRight=CommandItem(m,"Move right boundary right", DDV_MoveRightBoundaryRight);
*/
    mWin_d->MainMenu.CreateBlock=CommandItem(m,"Create block...", DDV_CreateBlock);
    mWin_d->MainMenu.DeleteBlock=CommandItem(m,"Delete block", DDV_DeleteBlock);
    mWin_d->MainMenu.Prev=CommandItem(m,"Undo  (Ctrl + z)", DDV_Prev);
    mWin_d->MainMenu.Next=CommandItem(m,"Redo  (Ctrl + y)", DDV_Next);
    mWin_d->MainMenu.Orig=CommandItem(m,"Restore original", DDV_Orig);
    /* to start, can't undo or redo anything */
    Disable(mWin_d->MainMenu.Prev);
    Disable(mWin_d->MainMenu.Next);
	}

	if (mWin_d->AutonomeViewer){
        mWin_d->MainMenu.Align = PulldownMenu(w, "Alignment/A");
        mWin_d->MainMenu.Hide=CommandItem(mWin_d->MainMenu.Align,
            "Hide or Show Rows...",DDV_HideDlgItem);
    }
	
    /* Align menu */
    if(vgp != NULL) {
        if(vgp->MasterViewer == SAMVIEWCN3D) {
            mWin_d->MainMenu.Align = PulldownMenu(w, "Alignment/A");
            mWin_d->MainMenu.Hide=CommandItem(mWin_d->MainMenu.Align,
                "Hide or Show Rows...",DDV_HideDlgItem);
            mWin_d->MainMenu.Deselect=CommandItem(mWin_d->MainMenu.Align,
                "Deselect All",DDV_DeselectItem);           
            SeparatorItem(mWin_d->MainMenu.Align);
            mWin_d->MainMenu.AddRow = SubMenu(mWin_d->MainMenu.Align, "Add New Row");
            mWin_d->MainMenu.FromFile = SubMenu(mWin_d->MainMenu.AddRow, "From FASTA file");
                CommandItem(mWin_d->MainMenu.FromFile,
                    "By gapped BLAST...", (Nlm_ItmActnProc)vgp->BlastFileGap);
                CommandItem(mWin_d->MainMenu.FromFile,
                    "By ungapped BLAST...", (Nlm_ItmActnProc)vgp->BlastFile);
            if (vgp->NetStartProc) {
                mWin_d->MainMenu.FromNet = SubMenu(mWin_d->MainMenu.AddRow, "Download from Entrez");
                CommandItem(mWin_d->MainMenu.FromNet,
                    "By gapped BLAST...", (Nlm_ItmActnProc)vgp->BlastNetGap);
                CommandItem(mWin_d->MainMenu.FromNet,
                    "By ungapped BLAST...", (Nlm_ItmActnProc)vgp->BlastNet);
            } 
        }
    }

	/*Options menu*/
	m=PulldownMenu(w,"Options");
	mWin_d->MainMenu.Options=m;
  if (!bEditor) {
    mWin_d->MainMenu.DispStyles=CommandItem(m,"Styles",DDV_DisplayStyles);
  }

#ifdef DEBUG_DDV
	CommandItem(m,"Switch UA Gap",DDV_ShowHideUnAlignGap);
#endif
	s=SubMenu(m,"Mouse modes");
	mWin_d->MainMenu.MouseMode=ChoiceGroup(s,SelectMouseMode);
	ChoiceItem(mWin_d->MainMenu.MouseMode,"Query");
	ChoiceItem(mWin_d->MainMenu.MouseMode,"Select rectangle");
	ChoiceItem(mWin_d->MainMenu.MouseMode,"Select one row");
  /* only allow "Edit" option for DDE, when not running in NoGaps mode */
  pNoGaps = (Boolean*) GetAppProperty("dde_nogaps");
  if (pNoGaps == NULL) NoGaps = TRUE;
  else NoGaps = *pNoGaps;
  if (!NoGaps) {
    if (bEditor)
      ChoiceItem(mWin_d->MainMenu.MouseMode,"Edit");
  }

  if (bEditor) {
    s2=SubMenu(m,"Justify");
    mWin_d->MainMenu.Justify=ChoiceGroup(s2,SelectJustify);
    ChoiceItem(mWin_d->MainMenu.Justify,"Left");
    ChoiceItem(mWin_d->MainMenu.Justify,"Right");
    ChoiceItem(mWin_d->MainMenu.Justify,"Center");
  } 

  /* only launch the editor from the viewer */
  if (!bEditor && mWin_d->EditAllowed) {
    mWin_d->MainMenu.ShowLowerPanel = StatusItem(m,
         "Show Lower Panel/L", DDV_ShowLowerPanel);
    SetObjectExtra (mWin_d->MainMenu.ShowLowerPanel, (Pointer)mWin_d, NULL);
    mWin_d->MainMenu.LaunchEditor=CommandItem(m,"Launch editor...",DDV_LaunchEditor);
    mWin_d->MainMenu.MergeBlocks=CommandItem(m,"Merge 2 blocks...",DDV_MergeBlocks);
    mWin_d->MainMenu.SplitBlock=CommandItem(m,"Split block in 2...",DDV_SplitBlock);
    mWin_d->MainMenu.DeleteDDVBlock=CommandItem(m, "Delete block...",DDV_DeleteBlock2);
  }

	if (mWin_d->AutonomeViewer && mWin_d->NetCfgMenuProc){
		mWin_d->MainMenu.ConfigNet=CommandItem(m,"Network...",
				mWin_d->NetCfgMenuProc);
	}
	/*Help menu*/
	m=PulldownMenu(w,"Help");
	CommandItem(m,"About...",DDV_AboutProc);
}

/*******************************************************************************

  Function : DDV_WinMainCleanup()
  
  Purpose : free memory before leaving viewer
  
  Parameters : mWin_d ; main window data
  
  Return value : none 

*******************************************************************************/
extern void DDV_WinMainCleanup(GraphiC g, VoidPtr data)
{
DdvMainWinPtr mWin_d;

	if (!data) return;
	mWin_d=(DdvMainWinPtr)data;

	/*main window data - Logo*/
/*	if (mWin_d->f1) DeleteFont(mWin_d->f1);
	if (mWin_d->f2) DeleteFont(mWin_d->f2);
	if (mWin_d->f3) DeleteFont(mWin_d->f3);
*/
	/*delete Align data*/
	DDV_CloseData(mWin_d,TRUE);

	MemFree(mWin_d);
}

/*******************************************************************************
  
  Purpose : init the graphical data structure of DDV. Use only when you start DDV. 

  Function : DDV_InitGraphGlobal()
  
  Parameters : dmp; main DDV data block
  
  Return value : -

*******************************************************************************/
extern void DDV_InitGraphGlobal(DdvMainPtr dmp)
{
Int2 decal=0;

	dmp->GrData.udv_font.LineHeight=
		UDV_ComputeLineHeight(dmp->GrData.udv_font.cyChar);
	dmp->GrData.udv_font.ColWidth=
		DDV_ComputeColWidth(dmp->GrData.udv_font.cxChar);
	UDV_Init_ScaleData(&(dmp->GrData));
	UDV_Build_Other_Colors(&(dmp->GrData));
	/*... then adjust values for DDV*/
	dmp->GrData.udv_scale.ScalePosition=(Int2)SCALE_POS_TOP;
	dmp->GrData.udv_panel.ShowFeatures=FALSE;
	dmp->GrData.udv_panel.ShowByBlock=FALSE;
	dmp->GrData.udv_scale.cxLeftScale=dmp->GrData.udv_font.cxChar*2;
	dmp->GrData.udv_panel.cxName=PANEL_NAME_WIDTH*dmp->GrData.udv_font.cxChar;
	if (dmp->GrData.udv_scale.ShowMajorTick) decal++;
	if (dmp->GrData.udv_scale.ScalePosition==SCALE_POS_TOP) decal++;
	dmp->GrData.udv_panel.cyScale=decal*dmp->GrData.udv_font.LineHeight;
	dmp->MSA_d.pgp_l.DisplayType=DDV_DISP_HORZ;
}	

/*******************************************************************************

  Function : DDV_SetRulerAttribInPGP()
  
  Purpose : modify the BSP ruler attribute for a row in the MSA
  
  Return value : -

*******************************************************************************/
extern void DDV_SetRulerAttribInPGP(ValNodePtr ParaG_Head, Uint1 RulerStyle)
{
ValNodePtr vnp;
ParaGPtr   pgp;

	vnp=ParaG_Head;
	while(vnp){
		pgp=(ParaGPtr)vnp->data.ptrvalue;
		if (pgp){
			pgp->ScaleStyle=RulerStyle;
			switch (RulerStyle){
				case SCALE_POS_NONE:
					pgp->nLines=1;
					break;
				case SCALE_POS_TOP:
					pgp->nLines=2;
					break;
				default:
					pgp->nLines=1;
					break;
			}
		}
		vnp=vnp->next;
	}
}

/*******************************************************************************

  Function : DDV_SetRulerAttribInPGP()
  
  Purpose : modify the BSP ruler attribute for a row in the MSA
  
  Return value : -

*******************************************************************************/
extern void DDV_SortPGPLineNum(ValNodePtr PNTR Head, Int4 nBsp)
{
ValNodePtr vnp;
ParaGPtr   pgp;
Int4       n=0,j;

	for(j=0;j<nBsp;j++){
		vnp=Head[j];
		while(vnp){
			pgp=(ParaGPtr)vnp->data.ptrvalue;
			pgp->StartLine=n;
			vnp=vnp->next;
		}
		n+=pgp->nLines;
	}	
}

/*******************************************************************************

  Function : DDV_ExportSeqAlign()
  
  Purpose : export a SeqAlign
  
  Return value : none 

*******************************************************************************/
static void DDV_ExportSeqAlign(SeqAlignPtr sap,CharPtr szFName)
{
SeqAlignPtr  sap_to_save=NULL;
AsnIoPtr     aip;
AsnTypePtr   atp;
AsnModulePtr amp;

	sap_to_save=AlnMgrGetSubAlign(sap, NULL, 0, -1);
	if (sap_to_save==NULL) goto error;

	amp = AsnAllModPtr ();
	atp = AsnTypeFind (amp,"Seq-align");

	if ((aip = AsnIoOpen (szFName, "w")) == NULL) goto error;
	SeqAlignAsnWrite( sap, aip, atp );
	AsnIoReset(aip);
	return;
error:
	if (sap_to_save) SeqAlignFree(sap_to_save);
	Message(MSG_OK, "Sorry, ASN.1 SeqAlign cannot be exported.");
	return;	
}

/*******************************************************************************

  Function : DDV_ExportSeqAlign()
  
  Purpose : export a SeqAlign
  
  Return value : none 

*******************************************************************************/
static void DDV_ExportSeqAnnot(SeqAlignPtr sap,CharPtr szFName)
{
SeqAlignPtr  sap_to_save=NULL;

	sap_to_save=AlnMgrGetSubAlign(sap, NULL, 0, -1);
	if (sap_to_save==NULL){
		Message(MSG_OK, "Sorry, ASN.1 SeqAlign cannot be exported.");
		return;
	}
	/*Salsa export function*/
	seqalign_write (sap_to_save, szFName);
	if (sap_to_save) SeqAlignFree(sap_to_save);
}

/*******************************************************************************

  Function : DDVExportTexteChooseFormat()
  
  Purpose : manage popup list of SeqAlign Format (export) 
  
  Return value : none 

*******************************************************************************/
static void DDVExportTexteChooseFormat (PopuP pop)
{
DdvExportTexteMSGPtr  detmp;
WindoW				  hExportDlg;
Int2                  value;

	hExportDlg=(WindoW)ParentWindow(pop);

	if (!hExportDlg) return;
	
	detmp = (DdvExportTexteMSGPtr) GetObjectExtra (hExportDlg);

	if (detmp==NULL) return;
	
	value=GetValue(pop);
	
	if(value>2){
		Disable(detmp->chk2);
		Disable(detmp->chk3);
		Disable(detmp->chk4);
		Disable(detmp->chk5);
		Disable(detmp->chk6);
	}
	else{
		Enable(detmp->chk2);
		Enable(detmp->chk3);
		Enable(detmp->chk4);
		Enable(detmp->chk5);
		Enable(detmp->chk6);
	}
	return;
}

/*******************************************************************************

  Function : DDVExportTexteOkProc()
  
  Purpose : manage ok button of the Export dialog box 
    
  Return value : none 

*******************************************************************************/
static void DDVExportTexteOkProc(ButtoN g)
{
DdvExportTexteMSGPtr  detmp;
DdvMainWinPtr         mWin_d;
DdvMainPtr            dmp;
DDVOptionsBlock       dob;
WindoW				  hExportDlg;
Uint4                 disp_options=0;
FILE                  *hFile;
Int2                  value;
Boolean               bRulerNum,bRulerTick,bShowBlock,bShowStrand,bShowBSPcoord,
                      bPrintGap;
Char 				  szFName[PATH_MAX]={""};

	hExportDlg=(WindoW)ParentWindow(g);

	if (!hExportDlg) return;
	
	detmp = (DdvExportTexteMSGPtr) GetObjectExtra (hExportDlg);

	if (detmp==NULL) {
		Remove(hExportDlg);
		return;
	}
	
	mWin_d = (DdvMainWinPtr) GetObjectExtra (detmp->hWinMain);
	dmp = (DdvMainPtr) GetObjectExtra(mWin_d->hWndDDV);
	
	/*get the user's choices*/
	value=GetValue(detmp->pop);
	
	bRulerNum=bRulerTick=bShowBlock=bShowStrand=bShowBSPcoord=FALSE;
	disp_options = VIEW_FULLSEQ;
	
	if(value<3){/*text and HTML only*/
		bRulerNum=GetStatus(detmp->chk2);
		if (bRulerNum){
			bRulerTick=GetStatus(detmp->chk3);
		}
		else{
			bRulerTick=FALSE;
		}
		bShowBlock=GetStatus(detmp->chk4);
		bShowStrand=GetStatus(detmp->chk5);
		bShowBSPcoord=GetStatus(detmp->chk6);
		if (bRulerNum) disp_options |= RULER_TOP;
		if (bRulerTick) disp_options|=RULER_TICK;
		if (bShowBlock) disp_options|=DISPE_SHOWBLOCK;
		if (bShowStrand) disp_options|=DISP_STRAND;
		if (bShowBSPcoord) disp_options|=DISP_BSP_COORD;
	}

	GetTitle(detmp->edit1, szFName, PATH_MAX-1);

	switch(value){
		case 1:/*text*/
			disp_options|=DISP_FULL_TXT;
			break;
		case 2:/*html*/
			disp_options|=DISP_FULL_HTML;
			break;
		case 3:/*philou*/
			disp_options|=DISP_PHYLIP_TXT;
			break;
		case 4:/*fasta + gaps*/
			bPrintGap=TRUE;
			break;			
		case 5:/*fasta without gaps*/
			bPrintGap=FALSE;
			break;			
		case 6:/*seqalign*/
			DDV_ExportSeqAlign(dmp->MSA_d.pgp_l.sap,szFName);
			Remove(hExportDlg);
			return;
		case 7:/*seqannot*/
			DDV_ExportSeqAnnot(dmp->MSA_d.pgp_l.sap,szFName);
			Remove(hExportDlg);
			return;
	}

	MemFill(&dob,0,sizeof(DDVOptionsBlock));
	dob.LineSize=ParaG_Size;
	hFile=fopen(szFName,"w");
	if (!hFile){
		Message(MSG_OK, "File creation failed !");
	}
	else{/*a last... save the SeqAlign*/
		switch(value){
			case 1:
			case 2:
			case 3:/*text,html,philou only*/
				DDV_DumpSAPInAFile(&(dmp->MSA_d.pgp_l),&dob, hFile,disp_options,
					dmp->Globals.colorp);
				break;
			case 4:
			case 5:/*Fasta*/
				DDV_DumpSAPInFastaFile(&(dmp->MSA_d.pgp_l),&dob, hFile,bPrintGap);
				break;
		}
		fclose(hFile);
	}
	Remove(hExportDlg);
}

/*******************************************************************************

  Function : DDVExportTextecancelProc()
  
  Purpose : manage cancel button of the Export dialog box 
    
  Return value : none 

*******************************************************************************/
static void DDVExportTextecancelProc(ButtoN g)
{
WindoW				hExportDlg;
DdvExportTexteMSGPtr  detmp;

	hExportDlg=(WindoW)ParentWindow(g);

	if (!hExportDlg) return;
	
	detmp = (DdvExportTexteMSGPtr) GetObjectExtra (hExportDlg);

	if (detmp==NULL) {
		Remove(hExportDlg);
		return;
	}

	Remove(hExportDlg);
}

/*******************************************************************************

  Function : DDVExportTexteShowNumber()
  
  Purpose : manage ShoWNumber checkbox of the Export dialog box 
    
  Return value : none 

*******************************************************************************/
static void DDVExportTexteShowNumber(ButtoN g)
{
WindoW				hExportDlg;
DdvExportTexteMSGPtr  detmp;

	hExportDlg=(WindoW)ParentWindow(g);

	if (!hExportDlg) return;
	
	detmp = (DdvExportTexteMSGPtr) GetObjectExtra (hExportDlg);

	if (detmp==NULL) return;
	
	if (GetStatus(detmp->chk2)==FALSE){
		Disable(detmp->chk3);
	}
	else{
		Enable(detmp->chk3);
	}

	return;	
}

/*******************************************************************************

  Function : DDVExportTexteFNameProc()
  
  Purpose : manage File name edit control of the Export dialog box 
  
  Parameters : t; edit control
  
  Return value : none 

*******************************************************************************/
static void DDVExportTexteFNameProc(TexT t)
{
Char 				szFName[PATH_MAX]={""};
WindoW				hExportDlg;
DdvExportTexteMSGPtr  detmp;

	hExportDlg=(WindoW)ParentWindow(t);

	if (!hExportDlg) return;
	
	detmp = (DdvExportTexteMSGPtr) GetObjectExtra (hExportDlg);

	if (detmp==NULL) return;
	
	GetTitle(t, szFName, PATH_MAX-1);

	if (StringLen(szFName) == 0)
		Disable(detmp->ok);
	else Enable(detmp->ok);

	return;
}

/*******************************************************************************

  Function : DDVExportTexteBrowseFile()
  
  Purpose : manage browse button of the Export dialog box 
  
  Parameters : g; button
  
  Return value : none 

*******************************************************************************/
static void DDVExportTexteBrowseFile(ButtoN g)
{
WindoW				hExportDlg;
DdvExportTexteMSGPtr  detmp;
Char 				path[PATH_MAX]={""};

	hExportDlg=(WindoW)ParentWindow(g);

	if (!hExportDlg) return;

	detmp = (DdvExportTexteMSGPtr) GetObjectExtra (hExportDlg);

	if (detmp==NULL) return;
	if (!detmp->edit1) return;

	if (GetOutputFileName (path, PATH_MAX-1, NULL)){ 
		SetTitle(detmp->edit1, path);
		DDVExportTexteFNameProc(detmp->edit1);
	}

	return;   
}

/*******************************************************************************

  Function : DDV_ExportTexte()
  
  Purpose : create the Export dialog box 
  
  Return value : none 

*******************************************************************************/
static void DDV_ExportTexte(IteM i)
{
DdvExportTexteMSGPtr  detmp;
DdvMainWinPtr mWin_d;
DdvMainPtr 	  dmp;
WindoW		  hWinMain,hExportDlg;
GrouP         g0,g1,g4,g7,g8,g10;
ButtoN        chk2,chk3,chk4,chk5,chk6,browse,ok;
TexT          edit1;
PopuP pop;
PrompT txt;

	/*get some data*/
	hWinMain=(WindoW)ParentWindow(i);
	if (hWinMain==NULL) return;
	mWin_d = (DdvMainWinPtr) GetObjectExtra (hWinMain);
	if (mWin_d==NULL) return;
	if (mWin_d->hWndDDV==NULL) return;
	dmp = (DdvMainPtr) GetObjectExtra(mWin_d->hWndDDV);
	if (dmp==NULL) return;

	/*init MsgStruct; to be used when user clicks on [OK]*/
	detmp=(DdvExportTexteMSGPtr)MemNew(sizeof(DdvExportTexteMSG));	
	if (!detmp) return;
	
	/*DlgBox window*/
	hExportDlg=MovableModalWindow(-30, -20,  -10,  -10, 
				"DDV - export a SeqAlign",  NULL);

	if (!hExportDlg){
		MemFree(detmp);
		return;
	}

	SetObjectExtra (hExportDlg, (Pointer) detmp, StdCleanupExtraProc);

	/*top-level groups*/
	g0=HiddenGroup(hExportDlg, -1, 0,NULL);
		
	g1=NormalGroup(g0,2,0,"SeqAlign Ruler",NULL,NULL);
/*	g2=HiddenGroup(g1,2,0);*/
	chk2=CheckBox(g1,"Show number",DDVExportTexteShowNumber);
	chk3=CheckBox(g1,"Show ticks",NULL);
	
	g4=NormalGroup(g0,0,4,"SeqAlign format",NULL,NULL);
	chk4=CheckBox(g4,"Use block of 10 letters",NULL);
	chk5=CheckBox(g4,"Display strand orientation",NULL);
	chk6=CheckBox(g4,"Display BioSeq coordinates",NULL);
	g7=HiddenGroup(g4,2,0,NULL);
	txt=StaticPrompt(g7,"Format :",0,0,systemFont,'l');
#ifdef WIN_MAC
	pop=PopupList(g7,TRUE,DDVExportTexteChooseFormat);
#endif

#ifndef WIN_MAC
	pop=PopupList(g7,FALSE,DDVExportTexteChooseFormat);
#endif
	AlignObjects(ALIGN_MIDDLE,(HANDLE) txt, (HANDLE) pop, NULL);
	PopupItem(pop,"Text");
	PopupItem(pop,"HTML");
	PopupItem(pop,"Phylip");
	PopupItem(pop,"Fasta with gaps");
	PopupItem(pop,"Fasta without gaps");
	PopupItem(pop,"ASN.1 SeqAlign");
	PopupItem(pop,"ASN.1 SeqAnnot");

	g8=NormalGroup(g0,3,0,"File name",NULL,NULL);
	edit1=DialogText(g8,"",20,DDVExportTexteFNameProc);
	browse=PushButton (g8, "Browse...", DDVExportTexteBrowseFile);

	/*ok - cancel group - start*/
	g10=HiddenGroup(g0, 2, 0,NULL);
	ok=PushButton (g10, "Ok", DDVExportTexteOkProc);
	PushButton (g10, "Cancel", DDVExportTextecancelProc);
	/*build the dlgbox*/
	RealizeWindow(hExportDlg);
	Show(hExportDlg);
	/*copy data*/
	detmp->hWinMain=hWinMain;
	detmp->chk2=chk2;
	detmp->chk3=chk3;
	detmp->chk4=chk4;
	detmp->chk5=chk5;
	detmp->chk6=chk6;
	detmp->pop=pop;
	detmp->edit1=edit1;
	detmp->ok=ok;
	/*color display check box*/
	SetStatus(chk2,TRUE);
	SetStatus(chk3,TRUE);
	SetStatus(chk4,TRUE);
	SetStatus(chk5,TRUE);
	SetStatus(chk6,TRUE);
	SetValue(pop,1);
	Disable(detmp->ok);
}

/*******************************************************************************

  Function : UDV_GotoBtnProc()
  
  Purpose : create the Export dialog box 
  
  Return value : none 

*******************************************************************************/
static void UDV_GotoBtnProc(ButtoN g)
{
DdvMainWinPtr      dmwp;
DdvMainPtr         dmp;
MsaParaGPopList    mppl;
WindoW             hParent;
Char               szCol[20],szRow[20],szInfo[255];
Int4               col,row,from_col,to_col,from_row,to_row,old_pos;
Boolean            bHorzScroll,bVertScroll;
BaR                hsb,vsb;

	/*get main data block*/
	hParent=(WindoW)ParentWindow(g);
	if (!hParent) return;
	dmwp=(DdvMainWinPtr)GetObjectExtra(hParent);
	if (!dmwp) return;
	dmp=(DdvMainPtr)GetObjectExtra(dmwp->hWndDDV);
	if (!dmp) return;	
	
	/*get user values*/
	GetTitle(dmwp->gotoValCol,szCol,19);
	GetTitle(dmwp->gotoValRow,szRow,19);
	col=atoi(szCol);
	row=atoi(szRow);
	
	/*check their validity*/
	mppl=dmp->MSA_d.pgp_l;
	if (row<1 || row>mppl.nBsp){
		sprintf(szInfo,"Please enter a 'row' value in\nthe range [1..%i]",
				mppl.nBsp);
		Message(MSG_OK,szInfo);
		Select(dmwp->gotoValRow);
		return;
	}
	if (col<1 || col>mppl.LengthAli){
		sprintf(szInfo,"Please enter a 'column' value in\nthe range [1..%i]",
				mppl.LengthAli);
		Message(MSG_OK,szInfo);
		Select(dmwp->gotoValCol);
		return;
	}
	
	/*do I need to scroll the panel content*/
	DDV_GetCurrentDispRange(dmwp->hWndDDV,&dmp->GrData,mppl.LengthAli,
		&from_col,&to_col,&from_row,&to_row);
	if (col>=from_col && col<=to_col)
		bHorzScroll=FALSE;
	else
		bHorzScroll=TRUE;
	if (row>=from_row && row<=to_row)
		bVertScroll=FALSE;
	else
		bVertScroll=TRUE;

	dmp->dtd.col=col;
	dmp->dtd.row=row;
	if (bHorzScroll){
		hsb = GetSlateHScrollBar ((SlatE) dmp->hWndDDV);
		col--;/*switch to zero-based values*/
		old_pos=GetBarValue(hsb);
		SetValue(hsb,old_pos+(col-old_pos)-5);
	}
	if (bVertScroll){
		vsb = GetSlateVScrollBar ((SlatE) dmp->hWndDDV);
		row--;/*switch to zero-based values*/
		old_pos=GetBarValue(vsb);
		SetValue(vsb,old_pos+(row-old_pos));
	}
	dmp->dtd.action=DDV_INVAL_REGION;
	dmp->dtd.status=DDV_SET_TIMER;
}

/*******************************************************************************

  Function : DDV_TimerProc()
  
  Purpose : timer callback of DDV 
  
  Return value : none 

*******************************************************************************/
extern void DDV_TimerProc (WindoW w)
{
DdvMainWinPtr dmwp;
DdvMainPtr    dmp;
Int4          col,row;

	/*get main data block*/
	dmwp=(DdvMainWinPtr)GetObjectExtra(w);
	if (!dmwp) return;
	dmp=(DdvMainPtr)GetObjectExtra(dmwp->hWndDDV);
	if (!dmp) return;	
	if (dmp->dtd.status==DDV_SET_TIMER){
		dmp->dtd.delay=0;
		if (dmp->dtd.action==DDV_INVAL_REGION){
			DDV_InvalRegion(dmp->hWndDDV,&(dmp->GrData),
				_max_(0,dmp->dtd.col-1),dmp->dtd.col+1,
				dmp->dtd.row,FALSE);
		}
		dmp->dtd.status=DDV_TEST_TIMER;
	}
	if (dmp->dtd.status==DDV_TEST_TIMER){
		if (dmp->dtd.delay>10){
			if (dmp->dtd.action==DDV_INVAL_REGION){
				col=dmp->dtd.col;
				row=dmp->dtd.row;
				dmp->dtd.col=0;
				dmp->dtd.row=0;
				DDV_InvalRegion(dmp->hWndDDV,&(dmp->GrData),
					_max_(0,col-1),col+1,row,FALSE);
				dmp->dtd.action=DDV_NOTHING;
			}
		}
	}
	dmp->dtd.delay++;
}

/*******************************************************************************

  Function : DDV_DeleteRow()
  
  Purpose : UI to delete a row 
  
  Return value : none 

*******************************************************************************/
static void DDV_DeleteRow(IteM i)
{
DdvMainWinPtr       mWin_d;
DdvMainPtr 	        dmp;
WindoW		          hWinMain,temport;
DDE_StackPtr        ddesp;
RecT 	              rcP;
static Int4         PassCount = 0;
BaR                 hsb;

	/*get some data*/
	hWinMain=(WindoW)ParentWindow(i);
	if (hWinMain==NULL) return;
	mWin_d = (DdvMainWinPtr) GetObjectExtra (hWinMain);
	if (mWin_d==NULL) return;
	if (mWin_d->hWndDDV==NULL) return;
	dmp = (DdvMainPtr) GetObjectExtra(mWin_d->hWndDDV);
	if (dmp==NULL) return;

  ddesp = dmp->dsp;
  
  if (PassCount == 0) {
    DDE_ShiftBlock(ddesp, 0, 5, TRUE);
  }

  if (PassCount == 1) {
    DDE_ShiftBlock(ddesp, 0, -5, TRUE);
    PassCount = 4;
  }
  
  if (PassCount == 5) {
    DDE_InsertGap(ddesp, 7, 1649, TRUE);
    DDE_InsertGap(ddesp, 7, 1649, TRUE);
    DDE_InsertGap(ddesp, 7, 9, TRUE);
    DDE_InsertGap(ddesp, 7, 9, TRUE);
    DDE_InsertGap(ddesp, 7, 9, TRUE);
    DDE_InsertGap(ddesp, 7, 11, TRUE);
    DDE_InsertGap(ddesp, 7, 3, TRUE);
    DDE_InsertGap(ddesp, 7, 1, TRUE);
    DDE_InsertGap(ddesp, 7, 0, TRUE);
    DDE_InsertGap(ddesp, 7, 0, TRUE);
    DDE_InsertGap(ddesp, 7, 27, TRUE);
    DDE_InsertGap(ddesp, 7, 23, TRUE);
    DDE_InsertGap(ddesp, 7, 22, TRUE);
    DDE_InsertGap(ddesp, 7, 1645, TRUE);
    DDE_InsertGap(ddesp, 7, 1655, TRUE);
    DDE_InsertGap(ddesp, 7, 1649, TRUE);
    DDE_InsertGap(ddesp, 7, 1649, TRUE);
    DDE_InsertGap(ddesp, 7, 1650, TRUE);
    DDE_InsertGap(ddesp, 7, 1648, TRUE);
    DDE_InsertGap(ddesp, 7, 1648, TRUE);
    PassCount = 6;
  }

  if (PassCount == 7) {
    DDE_RemoveGap(ddesp, 7, 1649, TRUE);
    DDE_RemoveGap(ddesp, 7, 1645, TRUE);
    DDE_RemoveGap(ddesp, 7, 15, TRUE);
    DDE_RemoveGap(ddesp, 7, 15, TRUE);
    DDE_RemoveGap(ddesp, 7, 1, TRUE);
    DDE_RemoveGap(ddesp, 7, 1, TRUE);
    DDE_RemoveGap(ddesp, 7, 25, TRUE);
    DDE_RemoveGap(ddesp, 7, 20, TRUE);
    DDE_RemoveGap(ddesp, 7, 18, TRUE);
    DDE_RemoveGap(ddesp, 7, 12, TRUE);
    DDE_RemoveGap(ddesp, 7, 11, TRUE);
    DDE_RemoveGap(ddesp, 7, 4, TRUE);
    DDE_RemoveGap(ddesp, 7, 0, TRUE);
    DDE_RemoveGap(ddesp, 7, 0, TRUE);
    DDE_InsertGap(ddesp, 7, 0, TRUE);
    DDE_RemoveGap(ddesp, 7, 1636, TRUE);
    DDE_InsertGap(ddesp, 7, 1645, TRUE);
    DDE_InsertGap(ddesp, 7, 1647, TRUE);
    DDE_RemoveGap(ddesp, 7, 1645, TRUE);
    DDE_RemoveGap(ddesp, 7, 1646, TRUE);
    DDE_InsertGap(ddesp, 7, 1645, TRUE);
    DDE_InsertGap(ddesp, 7, 1647, TRUE);
    DDE_RemoveGap(ddesp, 7, 1647, TRUE);
    DDE_RemoveGap(ddesp, 7, 1645, TRUE);
    DDE_RemoveGap(ddesp, 7, 0, TRUE);
    DDE_InsertGap(ddesp, 7, 0, TRUE);
    DDE_InsertGap(ddesp, 7, 0, TRUE);
    DDE_InsertGap(ddesp, 7, 1, TRUE);
    DDE_InsertGap(ddesp, 7, 3, TRUE);
    DDE_InsertGap(ddesp, 7, 5, TRUE);
    DDE_RemoveGap(ddesp, 7, 1, TRUE);
    DDE_RemoveGap(ddesp, 7, 2, TRUE);
    DDE_RemoveGap(ddesp, 7, 0, TRUE);
    DDE_RemoveGap(ddesp, 7, 0, TRUE);
    DDE_RemoveGap(ddesp, 7, 1, TRUE);
    DDE_InsertGap(ddesp, 7, 1649, TRUE);
    DDE_InsertGap(ddesp, 7, 1648, TRUE);
    DDE_InsertGap(ddesp, 7, 1647, TRUE);
    DDE_InsertGap(ddesp, 7, 1647, TRUE);
    DDE_RemoveGap(ddesp, 7, 1647, TRUE);
    DDE_InsertGap(ddesp, 7, 0, TRUE);
    DDE_RemoveGap(ddesp, 7, 1648, TRUE);
    DDE_InsertGap(ddesp, 7, 1649, TRUE);
    DDE_RemoveGap(ddesp, 7, 1649, TRUE);
    PassCount = 14;
  }

  if (PassCount == 15) {
    DDE_HideRow(ddesp, 3, TRUE);
    DDE_HideRow(ddesp, 3, TRUE);
    DDE_HideRow(ddesp, 4, TRUE);
    DDE_HideRow(ddesp, 0, TRUE);
    DDE_HideRow(ddesp, 7, TRUE);
  }

  if (PassCount ==  16) {
    DDE_ShowRow(ddesp, 2, TRUE);
    DDE_ShowRow(ddesp, 3, TRUE);
    DDE_ShowRow(ddesp, 0, TRUE);
    DDE_ShowRow(ddesp, 4, TRUE);
    DDE_ShowRow(ddesp, 7, TRUE);
  }

  if (PassCount == 17) {
    DDE_MoveRow(ddesp, 5, 2, TRUE);
  }

  if (PassCount == 18) {
    DDE_HideRow(ddesp, 3, TRUE);
    DDE_HideRow(ddesp, 4, TRUE);
    DDE_HideRow(ddesp, 0, TRUE);
    DDE_HideRow(ddesp, 7, TRUE);
    DDE_HideRow(ddesp, 2, TRUE);
    DDE_HideRow(ddesp, 1, TRUE);
    DDE_HideRow(ddesp, 6, TRUE);
    DDE_HideRow(ddesp, 5, TRUE);
    DDE_HideRow(ddesp, 4, TRUE);
    PassCount = 19;
  }

  if (PassCount == 20) {
    DDE_ShowRow(ddesp, 7, TRUE);
    DDE_ShowRow(ddesp, 0, TRUE);
    DDE_ShowRow(ddesp, 4, TRUE);
    DDE_ShowRow(ddesp, 3, TRUE);
    DDE_ShowRow(ddesp, 1, TRUE);
    DDE_ShowRow(ddesp, 5, TRUE);
    DDE_ShowRow(ddesp, 6, TRUE);
    DDE_ShowRow(ddesp, 2, TRUE);
    PassCount = 27;
  }

  if (PassCount == 28) {
    DDE_MoveRow(ddesp, 3, 3, TRUE);
    DDE_MoveRow(ddesp, 2, 5, TRUE);
    DDE_MoveRow(ddesp, 0, 6, TRUE);
    DDE_MoveRow(ddesp, 7, 1, TRUE);
    DDE_MoveRow(ddesp, 7, 0, TRUE);
    DDE_MoveRow(ddesp, 0, 7, TRUE);
    PassCount = 33;
  }

  if (PassCount == 34) {
    DDE_HideRow(ddesp, 6, TRUE);
    DDE_HideRow(ddesp, 1, TRUE);
    DDE_HideRow(ddesp, 0, TRUE);
    DDE_HideRow(ddesp, 7, TRUE);
    PassCount = 37;
  }

  if (PassCount == 38) {
    DDE_MoveRow(ddesp, 3, 1, TRUE);
    DDE_MoveRow(ddesp, 0, 2, TRUE);
    DDE_MoveRow(ddesp, 0, 3, TRUE);
    DDE_MoveRow(ddesp, 3, 0, TRUE);
    PassCount = 41;
  }

  if (PassCount == 42) {
    DDE_ShowRow(ddesp, 1, TRUE);
    DDE_ShowRow(ddesp, 7, TRUE);
    DDE_ShowRow(ddesp, 6, TRUE);
    DDE_ShowRow(ddesp, 0, TRUE);
    PassCount = 45;
  }

  if (PassCount == 46) {
    DDE_HideNewRow(ddesp, 5, TRUE);
    DDE_HideNewRow(ddesp, 2, TRUE);
    DDE_HideNewRow(ddesp, 4, TRUE);
    DDE_HideNewRow(ddesp, 3, TRUE);
    PassCount = 49;
  }

  if (PassCount == 50) {
    DDE_ShowNewRow(ddesp, 2, TRUE);
    DDE_ShowNewRow(ddesp, 3, TRUE);
    DDE_ShowNewRow(ddesp, 5, TRUE);
    DDE_ShowNewRow(ddesp, 4, TRUE);
    PassCount = 53;
  }

  if (PassCount == 54) {
    DDE_HideRow(ddesp, 4, TRUE);
    DDE_HideRow(ddesp, 7, TRUE);
    DDE_HideRow(ddesp, 0, TRUE);
    DDE_RestoreRowOrder(ddesp, TRUE);
    DDE_ShowRow(ddesp, 7, TRUE);
    DDE_ShowRow(ddesp, 0, TRUE);
    DDE_ShowRow(ddesp, 4, TRUE);
  }

  if (PassCount == 55) {
    DDE_ShiftRightBoundary(ddesp, 0, -1640, TRUE);
    DDE_DeleteBlock(ddesp, 0, TRUE);
    PassCount = 64;
  }

  if (PassCount == 65) {
    DDE_CreateBlock(ddesp, 20, 30, TRUE);
    DDE_CreateBlock(ddesp, 15, 35, TRUE);
    DDE_CreateBlock(ddesp, 25, 40, TRUE);
    DDE_CreateBlock(ddesp, 15, 40, TRUE);
  }

  MemCopy(&(dmp->MSA_d.pgp_l), ddesp->pEdit->pPopList, sizeof(MsaParaGPopList));

  DDV_InitColour_When_Start(dmp->MSA_d.pgp_l.sap,
    &(dmp->MSA_d.pgp_l),&(dmp->Globals.colorp), FALSE);

  /* recalculate window size */
  DDV_WhatSize(dmp);
  /* adjust horizontal scroll bar */
  hsb = GetSlateHScrollBar((SlatE) dmp->hWndDDV);
  DDV_UpdateHScrollVal(dmp->hWndDDV, FALSE, GetBarValue(hsb));

  PassCount++;

  temport=SavePort(mWin_d->hWndDDV);
  Select(mWin_d->hWndDDV);
  ObjectRect(mWin_d->hWndDDV, &rcP);
  InvalRect(&rcP);
  Update();
  RestorePort(temport);
 	return;
}

static void DDV_MoveRowLeft(IteM i)
{
  DDV_DoEditFunction(i, MOVE_ROW_LEFT);
  return;
}

static void DDV_MoveRowRight(IteM i)
{
  DDV_DoEditFunction(i, MOVE_ROW_RIGHT);
  return;
}

static void DDV_MoveLeftBoundaryLeft(IteM i)
{
  DDV_DoEditFunction(i, MOVE_LEFT_BOUNDARY_LEFT);
  return;
}

static void DDV_MoveLeftBoundaryRight(IteM i)
{
  DDV_DoEditFunction(i, MOVE_LEFT_BOUNDARY_RIGHT);
  return;
}

static void DDV_MoveRightBoundaryLeft(IteM i)
{
  DDV_DoEditFunction(i, MOVE_RIGHT_BOUNDARY_LEFT);
  return;
}

static void DDV_MoveRightBoundaryRight(IteM i)
{
  DDV_DoEditFunction(i, MOVE_RIGHT_BOUNDARY_RIGHT);
  return;
}

static void DDV_DeleteBlock(IteM i)
{
  DDV_DoEditFunction(i, DDE_DELETE_BLOCK);
  return;
}

static void DDV_CreateBlock(IteM i)
{
  DDV_DoEditFunction(i, DDE_CREATE_BLOCK);
  return;
}

static void DDV_Prev(IteM i)
{
  DDV_DoEditFunction(i, DDE_PREV);
  return;
}

static void DDV_Next(IteM i)
{
  DDV_DoEditFunction(i, DDE_NEXT);
  return;
}

static void DDV_Orig(IteM i)
{
  DDV_DoEditFunction(i, DDE_ORIG);
  return;
}

static void DDV_LaunchEditor(IteM i)
{
  DDV_DoEditFunction(i, DDE_LAUNCH_EDITOR);
  return;
}

static void DDV_MergeBlocks(IteM i)
{
  DDV_DoEditFunction(i, DDE_MERGE_BLOCKS);
  return;
}

static void DDV_SplitBlock(IteM i)
{
  DDV_DoEditFunction(i, DDE_SPLIT_BLOCK);
  return;
}

static void DDV_DeleteBlock2(IteM i)
{
  DDV_DoEditFunction(i, DDV_DELETE_BLOCK);
  return;
}


static void DDV_CallSaveEdits(IteM i) {
/*----------------------------------------------------------------------------
*  get DDVMainPtr and call DDV_SaveEdits
*---------------------------------------------------------------------------*/
  WindoW		     hWinMain;
  DdvMainWinPtr  mWin_d;
  DdvMainPtr 	   dmp;

	hWinMain=(WindoW)ParentWindow(i);
	if (hWinMain==NULL) return;
	mWin_d = (DdvMainWinPtr) GetObjectExtra (hWinMain);
	if (mWin_d==NULL) return;
	if (mWin_d->hWndDDV==NULL) return;
	dmp = (DdvMainPtr) GetObjectExtra(mWin_d->hWndDDV);
	if (dmp==NULL) return;

  DDV_SaveEdits(dmp, TRUE);
}


static void write_annot(SeqAlignPtr align)
{
        SeqAnnotPtr annot;
        AsnIoPtr aip;

        annot = SeqAnnotNew();
        annot->type = 2;
        annot->data = align;

        aip = AsnIoOpen("temp2.sat", "w");
        SeqAnnotAsnWrite(annot, aip, NULL);
        AsnIoClose(aip);

        annot->data = NULL;
        SeqAnnotFree(annot);
}         


NLM_EXTERN void DDV_SaveEdits(DdvMainPtr dmp, Boolean UpdateDDV) {
/*----------------------------------------------------------------------------
*  create a DenseSeg for the block that has been created or edited.
*  pass this info to the AlnMgr and tell it to add or replace the block.
*---------------------------------------------------------------------------*/
  Int4*          pStarts;
  Int4*          pLens;
  Uint1*         pStrands;
  Int4           ii, jj, NumSegs, NumRows, NumBlocksAtLastSave, NumBlocksNow;
  DenseSegPtr    dsp;
  SeqAlignPtr    sap, indexed_sap;
  SeqId          head;
  SeqIdPtr       sip_prev, sip;
  DDVUpdateMSG   dum;
  MsaParaGPopListPtr  mpplp;
  Boolean        ReDraw;
  BaR            hsb;
  Int4           HPos, Col, i;

  /* note:       dsp is DenseSegPtr */
  /*        dmp->dsp is DDE_StackPtr */

  NumRows = dmp->dsp->pEdit->TotalNumRows;
  NumSegs = DDE_GetNumSegmentsInBlock(dmp->dsp->pEdit, 0);
  sap = ViewMgr_GetBegin(dmp->MSA_d.pgp_l.viewed_sap);
  indexed_sap = ViewMgr_GetBeginIndexed(dmp->MSA_d.pgp_l.viewed_sap);

  /* if there's a block that gets added or replaced, create a DenseSeg */
  if (NumSegs > 0) {
    /* create the empty dense seg, do NOT free it in this routine */
    dsp = DenseSegNew();
    dsp->dim = dmp->dsp->pEdit->TotalNumRows;
    dsp->numseg = NumSegs;
    /* create the starts, lens, and strands arrays */
    pStarts =  MemNew(NumRows * NumSegs * sizeof(Int4));
    pLens =    MemNew(NumSegs * sizeof(Int4));
    pStrands = MemNew(NumRows * NumSegs * sizeof(Uint1));
    DDE_CreateArraysForDenseSeg(dmp->dsp->pEdit, 0, pStarts, pLens); /* fill starts and lens */
    for (ii=0; ii<NumSegs; ii++) {
      for (jj=0; jj<NumRows; jj++) {
        pStrands[ii*NumRows + jj] = AlnMgrGetNthStrand(sap, jj+1); /* fill strands */
      }
    }
    dsp->starts = pStarts;
    dsp->lens = pLens;
    dsp->strands = pStrands;
    /* create the linked-list of SeqIdPtr's */
    sip_prev = &head;
    for (ii=0; ii<NumRows; ii++) {
      sip = AlnMgrGetNthSeqIdPtr(indexed_sap, ii+1);
      sip_prev->next = sip;
      sip_prev = sip_prev->next;
    }
    dsp->ids = head.next;
  }

  if (NumSegs > 0) {NumBlocksNow = 1;}
  else             {NumBlocksNow = 0;}

  NumBlocksAtLastSave = dmp->dsp->NumBlocks;

  ReDraw = FALSE;
  if ((NumBlocksAtLastSave==1) && (NumBlocksNow==0)) {
    /* delete block */
    AlnMgrReplaceBlock(indexed_sap, NULL, dmp->dsp->LaunchBlock+1);
  }
  if ((NumBlocksAtLastSave==1) && (NumBlocksNow==1)) {
    /* replace block */
    AlnMgrReplaceBlock(indexed_sap, dsp, dmp->dsp->LaunchBlock+1);
    ReDraw = TRUE;
  }
  if ((NumBlocksAtLastSave==0) && (NumBlocksNow==1)) {
    /* add block */
    AlnMgrAddBlock(indexed_sap, dsp);
    dmp->dsp->IsUnAligned = FALSE;
    ReDraw = TRUE;
  }

  SeqAlignListFree(sap);
  sap = AlnMgrGetSubAlign(indexed_sap, NULL, 0, -1);

  ViewMgr_SetBegin(dmp->MSA_d.pgp_l.viewed_sap, sap, FALSE, FALSE);
  /* indicate there are no edits to save when quitting DDE */
  dmp->dsp->SomethingToSave = FALSE;

  dmp->dsp->NumBlocks = NumBlocksNow;

  /* redraw the display when 2 aligned blocks are merged */
  /* if merging with preceeding block, launch block is decremented */
  /* (there's no need to redraw when the first block is left aligned */
  /* or when the last block is right aligned.  however, since I have */
  /* no way of knowing how many blocks there are, a right aligned last */
  /* block always causes a redraw) */

  /* update:  we decided not to do auto merge.  so, I'm setting */
  /* ReDraw to FALSE here.  no redraw. no modifying block index. */
  /* if we go back to auto-merge, just get rid of this one line. */
  ReDraw = FALSE;

  if (ReDraw) {
    if (DDE_FirstColumnIsAligned(dmp->dsp->pEdit)) {
      if (dmp->dsp->LaunchBlock == 0) {
        ReDraw = FALSE;
      }
      else {
        dmp->dsp->LaunchBlock--;
      }
    }
    else if (!DDE_LastColumnIsAligned(dmp->dsp->pEdit)) {
      ReDraw = FALSE;
    }
  }

  if (ReDraw) {
    mpplp = DDE_CreateDisplay(indexed_sap, dmp->dsp->LaunchBlock,
                              dmp->dsp->IsUnAligned, &dmp->dsp->NumBlocks);
    mpplp->viewed_sap = dmp->dsp->pEdit->pPopList->viewed_sap;
    dmp->dsp->pEdit = DDE_New(mpplp, dmp->dsp->pEdit->TotalNumRows);
    DDE_MergeNodesLists(dmp->dsp->pEdit);
    /* hide the rows in DDE that are hidden in DDV */
    NumRows = dmp->dsp->pEdit->TotalNumRows;
    for (i=0; i<NumRows; i++) {
      if (ViewMgr_TRow2VRow(dmp->MSA_d.pgp_l.viewed_sap, i+1) == -1) {
        DDE_HideRow(dmp->dsp, i, FALSE);
      }
    }
    DDE_Add(dmp->dsp);
    dmp->dsp->SomethingToSave = FALSE;
    /* position block at 5th col */
    hsb = GetSlateHScrollBar((SlatE) dmp->hWndDDV);
    HPos = GetBarValue(hsb);
    Col = DDE_GetAlignStart(dmp->dsp->pEdit, 0);
    DDV_ReDrawAtCol(dmp, HPos+Col-5, TRUE);
  }

  /* tell DDV to rebuild its display */
  if (UpdateDDV) {
	  dum.type = UPDATE_TYPE_VIEWMGR;
	  dum.data = NULL;
    ViewMgr_Update(dmp->MSA_d.pgp_l.viewed_sap);
	  ObjMgrSendProcMsg(OM_MSG_UPDATE, dmp->MSA_d.entityID, dmp->MSA_d.itemID,
      OBJ_SEQALIGN, 0, 0, (Pointer)&dum);
  }

  return;
}

/*******************************************************************************

  Function : DDV_DoEditFunction(Item i, Int4 WhichOne)
  
  Purpose : UI to do one of the edit functions
  
  Return value : none 

*******************************************************************************/
static void DDV_DoEditFunction(IteM i, Int4 WhichOne)
{
DdvMainWinPtr       mWin_d;
DdvMainPtr 	        dmp;
WindoW		          hWinMain;
DDE_StackPtr        ddesp;

	/*get some data*/
	hWinMain=(WindoW)ParentWindow(i);
	if (hWinMain==NULL) return;
	mWin_d = (DdvMainWinPtr) GetObjectExtra (hWinMain);
	if (mWin_d==NULL) return;
	if (mWin_d->hWndDDV==NULL) return;
	dmp = (DdvMainPtr) GetObjectExtra(mWin_d->hWndDDV);
	if (dmp==NULL) return;

  ddesp = dmp->dsp;
  
  switch (WhichOne) {
    case MOVE_ROW_LEFT:
      DDE_ShiftRow(ddesp, 4, -1, TRUE, TRUE);
      DDV_ReDraw(dmp);
      break;
    case MOVE_ROW_RIGHT:
      DDE_ShiftRow(ddesp, 4,  1, TRUE, TRUE);
      DDV_ReDraw(dmp);
      break;
    case MOVE_LEFT_BOUNDARY_LEFT:
      DDE_ShiftLeftBoundary(ddesp, 0, -1, TRUE);
      DDV_ReDraw(dmp);
      break;
    case MOVE_LEFT_BOUNDARY_RIGHT:
      DDE_ShiftLeftBoundary(ddesp, 0,  1, TRUE);
      DDV_ReDraw(dmp);
      break;
    case MOVE_RIGHT_BOUNDARY_LEFT:  
      DDE_ShiftRightBoundary(ddesp, 0, -1, TRUE);
      DDV_ReDraw(dmp);
      break;
    case MOVE_RIGHT_BOUNDARY_RIGHT:
      DDE_ShiftRightBoundary(ddesp, 0,  1, TRUE);
      DDV_ReDraw(dmp);
      break;
    case DDE_DELETE_BLOCK:
      if (DDE_DeleteBlock(ddesp, 0, TRUE)) {
        DDV_ReDraw(dmp);
      }
      break;
    case DDE_CREATE_BLOCK:
      /* switch to the mode that allows block creation */
      dmp->SavedMouseMode = dmp->MouseMode;
      dmp->MouseMode = DDV_MOUSEMODE_CREATEBLOCK;
      CrossCursor();
      break;
    case DDE_PREV:
      if (DDE_Prev(ddesp)) {
        DDV_ReDraw(dmp);
      }
      break;
    case DDE_NEXT:
      if (DDE_Next(ddesp)){
        DDV_ReDraw(dmp);
      }
      break;
    case DDE_ORIG:
      DDE_GetOriginal(ddesp, TRUE);
      DDV_ReDraw(dmp);
      break;
    case DDE_LAUNCH_EDITOR:
      dmp->SavedMouseMode = dmp->MouseMode;
      dmp->MouseMode = DDV_MOUSEMODE_LAUNCHEDITOR;
      CrossCursor();
      break;
    case DDE_MERGE_BLOCKS:
      dmp->SavedMouseMode = dmp->MouseMode;
      dmp->MouseMode = DDV_MOUSEMODE_MERGEBLOCKS1;
      CrossCursor();
      break;
    case DDE_SPLIT_BLOCK:
      dmp->SavedMouseMode = dmp->MouseMode;
      dmp->MouseMode = DDV_MOUSEMODE_SPLITBLOCK;
      CrossCursor();
      break;
    case DDV_DELETE_BLOCK:
      dmp->SavedMouseMode = dmp->MouseMode;
      dmp->MouseMode = DDV_MOUSEMODE_DELETEBLOCK;
      CrossCursor();
      break;
  }
  if (dmp->bEditor) {
    DDV_GreyOut(mWin_d, DDE_AtStartOfStack(dmp->dsp), DDE_AtEndOfStack(dmp->dsp));
  }
 	return;
}


NLM_EXTERN void DDVResetProc (PaneL p)

{
}

static void PopulateDDV (BioseqViewPtr bvp)

{
}

static void ShowDDV (BioseqViewPtr bvp, Boolean show)

{
}

static void SelectDDV (BioseqViewPtr bvp, Uint2 selentityID, Uint2 selitemID,
                       Uint2 selitemtype, SeqLocPtr region,
                       Boolean select, Boolean scrollto)

{
}

static void ResizeDDV (BioseqViewPtr bvp)

{
}

BioseqPageData ddvPageData = {
  "Multiple", TRUE, TRUE, TRUE, FALSE, -1,
  PopulateDDV, ShowDDV, SelectDDV,
  NULL, NULL,
  NULL, NULL, ResizeDDV, NULL
};

