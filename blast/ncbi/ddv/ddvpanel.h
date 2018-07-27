/*  $Id: ddvpanel.h,v 1.30 2000/07/12 22:22:42 hurwitz Exp $
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
* File Name:  ddvpanel.h
*
* Author:  Patrick Durand
*
* Version Creation Date:   06/19/99
*
* $Revision: 1.30 $
*
* File Description: 
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: ddvpanel.h,v $
* Revision 1.30  2000/07/12 22:22:42  hurwitz
* added delete block to DDV
*
* Revision 1.29  2000/07/08 20:43:58  vakatov
* Get all "#include" out of the 'extern "C" { }' scope;  other cleanup...
*
* Revision 1.28  2000/07/05 19:23:14  lewisg
* add two panes to ddv, update msvc project files
*
* Revision 1.27  2000/06/30 22:31:51  hurwitz
* added merge block function for DDV
*
* Revision 1.26  2000/06/02 19:57:04  hurwitz
* added a query dialog when converting from pairwise multiple to true multiple, bug fix of double freeing of a seqAlignPtr
*
* Revision 1.25  2000/05/15 22:00:36  hurwitz
* put save-edits query into DDV_SlaveQuit where it belongs
*
* Revision 1.24  2000/04/29 21:49:24  kans
* removed bad characters on Mac
*
* Revision 1.23  2000/04/27 20:05:55  kans
* added stub functions for Sequin
*
* Revision 1.22  2000/04/27 19:57:59  hurwitz
* scroll to aligned block on launch of DDE
*
* Revision 1.21  2000/04/21 23:00:50  hurwitz
* can launch DDE from DDV
*
* Revision 1.20  2000/04/10 21:41:26  lewisg
* move alignment menus into ddv, udv from cn3d
*
* Revision 1.19  2000/04/10 20:58:42  hurwitz
* added GUI controls for DeleteBlock in DDE
*
* Revision 1.18  2000/04/07 16:21:08  hurwitz
* made delete block faster, added delete block to edit menu
*
* Revision 1.17  2000/03/21 14:21:47  durand
* fixed a problem with menus setup
*
* Revision 1.16  2000/03/14 22:08:21  hurwitz
* undo and redo working properly, restore-original function added
*
* Revision 1.15  2000/03/10 23:01:43  hurwitz
* added undo and redo functions, first pass
*
* Revision 1.14  2000/03/06 22:45:58  hurwitz
* can shift right boundary of an aligned block left and right, DDVRuler updates added
*
* Revision 1.13  2000/02/08 14:10:43  durand
* made extern the functions DDV_UpdateHScrollVal and
*
* Revision 1.12  2000/02/04 16:05:41  durand
* add click action to select a row
*
* Revision 1.11  2000/01/26 13:38:55  durand
* update the GUI for the editor. Add functions to create the data to be used by the editor
*
* Revision 1.10  2000/01/10 15:09:45  durand
* Use Entrez instead of ID1
*
* Revision 1.9  1999/12/07 21:40:14  durand
* add mouse modes menu and caret facility for the editor
*
* Revision 1.8  1999/12/06 16:19:20  durand
* add GoTo facility to DDV
*
* Revision 1.7  1999/12/03 23:17:23  lewisg
* Patrick's new global update msg, argument passing when launching ddv, experimental editing
*
* Revision 1.6  1999/11/30 17:29:00  durand
* fix a problem of redeclaration of the function DDV_CloseData
*
* Revision 1.5  1999/11/29 15:26:26  durand
* designed a new GUI to fix problems under MacOS, Linux and SGI
*
* Revision 1.4  1999/10/22 20:12:47  durand
* add Export command (text, HTML and Phylip formats)
*
* Revision 1.3  1999/10/20 13:17:19  durand
* add display for disc. SeqAlign tails
*
* Revision 1.2  1999/10/15 21:57:37  durand
* add a UI for display options
*
* Revision 1.1  1999/09/30 14:10:29  durand
* add ddv to toolkit
*
* Revision 1.5  1999/09/09 21:55:07  durand
* instantiate the Fle|Close command of DDV
*
* Revision 1.4  1999/07/01 14:08:08  durand
* the loader functions of DDV or now in ddvopen.c
*
* Revision 1.2  1999/06/28 22:07:22  durand
* add loader functions and clean the code with Lint and Purify
*
* Revision 1.1  1999/06/19 17:21:08  durand
* add Vibrant DDV code
*
*
*
* ==========================================================================
*/

#ifndef _DDVPANEL_
#define _DDVPANEL_

#include <udviewer.h>
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

	Data structures

******************************************************************************/
typedef struct ddvdispstylesmsg {/*used by the Display Styles Dlg box*/
	WindoW hWinMain; /*DDV main window handle*/
	ButtoN  chk2;/*use color display chk box*/
	ButtoN  chk3;/*show left tail chk box*/
	ButtoN  chk4;/*show right tail chk box*/
	GrouP  g1;   /*main group for Highlight unaligned regions"*/
	GrouP  g2;   /*"Spacer" group for Highlight unaligned regions"*/
	GrouP  g5;   /*"seq. justification" group for Highlight unaligned regions"*/
	GrouP  g12;  /*BSP ruler style group*/
	TexT   edit1; /*size TexT */
	LisT   BspNames;/*list of BSP names*/
	} DdvDispStylesMSG, PNTR DdvDispStylesMSGPtr;

typedef struct ddvexporttextemsg {/*used by the Display Styles Dlg box*/
	WindoW hWinMain; /*DDV main window handle*/
	ButtoN  chk2;/*show number*/
	ButtoN  chk3;/*show ticks*/
	ButtoN  chk4;/*Use block of 10 letters*/
	ButtoN  chk5;/*Display strand orientation*/
	ButtoN  chk6;/*Display BioSeq coordinates*/
	ButtoN  ok;
	PopuP   pop; /*list of available formats*/
	TexT    edit1; /*filename */
	} DdvExportTexteMSG, PNTR DdvExportTexteMSGPtr;


/******************************************************************************

	Defines

******************************************************************************/

#define DDV_DEFAULT_PARAG_SIZE 70

#define MOVE_ROW_LEFT              0
#define MOVE_ROW_RIGHT             1
#define MOVE_LEFT_BOUNDARY_LEFT    2
#define MOVE_LEFT_BOUNDARY_RIGHT   3
#define MOVE_RIGHT_BOUNDARY_LEFT   4
#define MOVE_RIGHT_BOUNDARY_RIGHT  5
#define DDE_PREV                   6
#define DDE_NEXT                   7
#define DDE_ORIG                   8
#define DDE_DELETE_BLOCK           9  /* for DDE */
#define DDE_CREATE_BLOCK          10
#define DDE_LAUNCH_EDITOR         11
#define DDE_MERGE_BLOCKS          12
#define DDE_SPLIT_BLOCK           13
#define DDV_DELETE_BLOCK          14  /* for DDV */


/******************************************************************************

	Global varaibles

******************************************************************************/
extern Char szAppName[];
extern Char szAppName2[];


/******************************************************************************

	Exported functions

******************************************************************************/
extern void DDV_EnableGotoTBItems(WindoW hParent,Boolean bEnable);
extern void DDV_WhatSize(DdvMainPtr dmp);
extern void DDV_InitPanelData(UDVPanelDataPtr pdp);
extern void DDV_SetupWin (WindoW w,Boolean bInit);
extern void DDV_Resize_DDV (PaneL p, Boolean bInit);
extern void DDV_WinMainResize (WindoW w);
extern void DDV_UpdateVScrollVal(PaneL p,Boolean bInit,Int4 CurPos);
extern void DDV_UpdateHScrollVal(PaneL p,Boolean bInit,Int4 CurPos);
extern void DDV_VHScrl(PaneL p,UnDViewerGraphDataPtr gdp, Int4 newval, Int4 oldval,
	Boolean IsVscroll);
extern void DDV_VScrlProc (BaR sb, SlatE s, Int4 newval, Int4 oldval);
extern void DDV_HScrlProc (BaR sb, SlatE s, Int4 newval, Int4 oldval);
extern Boolean DDV_CreateViewerPanel(WindoW w,DdvMainWinPtr dmwp,
	SAM_ViewGlobal *vgp,Boolean bEditor);
NLM_EXTERN void DDV_SetMenuFocus(WindoW w);
extern void DDV_SetupMenus(WindoW w,Boolean isID1Ok,Boolean bEditor,
                           SAM_ViewGlobal *vgp);
extern void DDV_WinMainResize (WindoW w);
extern void DDV_WinMainCleanup (GraphiC g, VoidPtr data);
extern void DDV_WinMainProgQuit(WindoW w);
extern void DDV_InitGraphGlobal(DdvMainPtr dmp);
extern void DDV_CloseData(DdvMainWinPtr mWin_d,Boolean bFinalExit);
extern void DDV_SetRulerAttribInPGP(ValNodePtr ParaG_Head, Uint1 RulerStyle);
extern void DDV_SortPGPLineNum(ValNodePtr PNTR Head, Int4 nBsp);
extern void DDV_CleanupDDVPdata_g (DdvMainPtr dmp);
extern void DDV_TimerProc (WindoW w);
NLM_EXTERN void DDVResetProc (PaneL p);

NLM_EXTERN void DDV_Save(ButtoN g);
NLM_EXTERN void DDV_NoSave(ButtoN g);
NLM_EXTERN void DDV_Cancel(ButtoN g);
NLM_EXTERN void DDV_FileCloseIt(WindoW hWinMain);
NLM_EXTERN void DDV_SaveEdits(DdvMainPtr dmp, Boolean UpdateDDV);

#ifdef __cplusplus
}
#endif

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

#endif /* ndef _DDVPANEL_ */
