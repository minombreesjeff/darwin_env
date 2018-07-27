/*  $Id: ddvmain.h,v 1.44 2000/07/17 13:32:33 lewisg Exp $
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
* File Name:  ddvmain.h
*
* Author:  Patrick Durand
*
* Version Creation Date:   06/19/99
*
* $Revision: 1.44 $
*
* File Description: 
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: ddvmain.h,v $
* Revision 1.44  2000/07/17 13:32:33  lewisg
* move DDV_args out of the library
*
* Revision 1.43  2000/07/17 12:38:53  kans
* DDV_myargs is extern in header, instantiated in ddvpanel.c, since it is accessed from that library file
*
* Revision 1.42  2000/07/14 22:24:56  lewisg
* fix scroll, click, visual c++ build bugs.  add command line arg parse to ddv
*
* Revision 1.41  2000/07/12 22:22:42  hurwitz
* added delete block to DDV
*
* Revision 1.40  2000/07/10 14:38:07  lewisg
* move seqalign and sequentry data from window to panels
*
* Revision 1.39  2000/07/08 20:43:58  vakatov
* Get all "#include" out of the 'extern "C" { }' scope;  other cleanup...
*
* Revision 1.38  2000/07/05 19:23:13  lewisg
* add two panes to ddv, update msvc project files
*
* Revision 1.37  2000/07/05 18:42:16  hurwitz
* added split block function to DDV
*
* Revision 1.36  2000/06/30 22:31:51  hurwitz
* added merge block function for DDV
*
* Revision 1.35  2000/06/27 20:46:38  hurwitz
* fixed bugs with select rectangle, added select row option
*
* Revision 1.34  2000/06/15 17:33:18  hurwitz
* used view seqAlignPtr to get original (bug fix), and started working on left/right/center justify for DDE
*
* Revision 1.33  2000/05/19 13:48:31  hurwitz
* made a version of DDE that doesn't allow aligned gaps, changed wording for adding new rows
*
* Revision 1.32  2000/05/15 23:39:34  lewisg
* shred cblast, add menu items for gapped/ungapped, fix pdbheaders
*
* Revision 1.31  2000/04/26 21:54:27  hurwitz
* added save function to tell AlnMgr about edits made in DDE
*
* Revision 1.30  2000/04/21 23:00:50  hurwitz
* can launch DDE from DDV
*
* Revision 1.29  2000/04/18 19:50:24  lewisg
* add deselect menu item
*
* Revision 1.28  2000/04/17 13:30:43  durand
* removed g_hParent and unused functions DDV_LaunchAlignViewer and DDV_LaunchAlignEditor
*
* Revision 1.27  2000/04/10 21:41:26  lewisg
* move alignment menus into ddv, udv from cn3d
*
* Revision 1.26  2000/04/10 20:58:42  hurwitz
* added GUI controls for DeleteBlock in DDE
*
* Revision 1.25  2000/04/07 16:21:08  hurwitz
* made delete block faster, added delete block to edit menu
*
* Revision 1.24  2000/03/27 22:15:05  lewisg
* add show/hide row dialog
*
* Revision 1.23  2000/03/25 00:22:09  hurwitz
* put DDE_StackPtr in DDV_Main, add to stack inside DDE api's, added insert char, delete char, home and end keyboard control
*
* Revision 1.22  2000/03/14 22:08:21  hurwitz
* undo and redo working properly, restore-original function added
*
* Revision 1.21  2000/03/10 23:01:43  hurwitz
* added undo and redo functions, first pass
*
* Revision 1.20  2000/03/06 22:45:58  hurwitz
* can shift right boundary of an aligned block left and right, DDVRuler updates added
*
* Revision 1.19  2000/03/01 22:49:41  lewisg
* import bioseq, neatlyindex, get rid of dead code
*
* Revision 1.18  2000/02/04 16:05:41  durand
* add click action to select a row
*
* Revision 1.17  2000/01/26 13:38:55  durand
* update the GUI for the editor. Add functions to create the data to be used by the editor
*
* Revision 1.16  2000/01/12 21:52:17  durand
* add import function; update menus when DDV is loaded from Cn3D
*
* Revision 1.15  2000/01/11 15:05:23  durand
* remove network stuff
*
* Revision 1.14  2000/01/10 15:09:45  durand
* Use Entrez instead of ID1
*
* Revision 1.13  1999/12/20 20:20:41  lewisg
* allow cn3d to do color and ddv to do case when both are running
*
* Revision 1.12  1999/12/07 21:40:14  durand
* add mouse modes menu and caret facility for the editor
*
* Revision 1.11  1999/12/03 23:17:23  lewisg
* Patrick's new global update msg, argument passing when launching ddv, experimental editing
*
* Revision 1.10  1999/11/29 15:26:25  durand
* designed a new GUI to fix problems under MacOS, Linux and SGI
*
* Revision 1.9  1999/11/09 17:09:00  durand
* transfer some functions from ddvgraph to ddvcreate, so that ddvcreate remains Vibrant free and can be compiled with BLAST
*
* Revision 1.8  1999/11/03 21:29:48  durand
* add CTRL and SHFT keys for mouse selection. redesign the loader functions of DDV to properly register the message callbacks
*
* Revision 1.7  1999/10/29 14:15:40  durand
* add simple mouse selection functions
*
* Revision 1.6  1999/10/23 14:54:34  durand
* resolve external symbol g_hParent
*
* Revision 1.5  1999/10/22 20:12:47  durand
* add Export command (text, HTML and Phylip formats)
*
* Revision 1.4  1999/10/22 14:19:43  durand
* update the code for the startup functions of DDV drawing panel
*
* Revision 1.3  1999/10/20 13:17:19  durand
* add display for disc. SeqAlign tails
*
* Revision 1.2  1999/10/15 21:57:36  durand
* add a UI for display options
*
* Revision 1.1  1999/09/30 14:10:27  durand
* add ddv to toolkit
*
* Revision 1.7  1999/09/23 19:06:50  lewisg
* increase maxtemp number of cached sequences
*
* Revision 1.6  1999/09/21 14:19:08  durand
* add mouse click management layout
*
* Revision 1.5  1999/09/09 21:55:07  durand
* instantiate the Fle|Close command of DDV
*
* Revision 1.4  1999/07/20 14:58:01  durand
* use the Color Manager to display colored MSA
*
* Revision 1.3  1999/06/30 14:57:21  durand
* update DDV loader functions
*
* Revision 1.2  1999/06/28 22:07:21  durand
* add loader functions and clean the code with Lint and Purify
*
* Revision 1.1  1999/06/19 17:21:07  durand
* add Vibrant DDV code
*
*
*
* ==========================================================================
*/

#ifndef _DDVMAIN_
#define _DDVMAIN_

#include <ncbi.h>
#include <vibrant.h>
#include <ddvopen.h>
#include <pgppop.h>
#include <ddvcolor.h>
#include <ddvcreate.h>


#ifdef __cplusplus
extern "C" {
#endif


/******************************************************************************

	ERROR / Information messages from DDV_OPEN module 

******************************************************************************/
#define DVV_MSG_M_OK 1

/******************************************************************************

	defines

******************************************************************************/

/* maximum number of sequences to hold in memory */
#define DDV_MAXTEMP 500 
/*caret style */
#define DDV_CARET_BAR 1
/*mouse mode*/
#define DDV_MOUSEMODE_QUERY           0
#define DDV_MOUSEMODE_SELECT          1
#define DDV_MOUSEMODE_EDIT            2
#define DDV_MOUSEMODE_CREATEBLOCK     3
#define DDV_MOUSEMODE_LAUNCHEDITOR    4
#define DDV_MOUSEMODE_SELECT_ONE_ROW  5
#define DDV_MOUSEMODE_MERGEBLOCKS1    6
#define DDV_MOUSEMODE_MERGEBLOCKS2    7
#define DDV_MOUSEMODE_SPLITBLOCK      8
#define DDV_MOUSEMODE_DELETEBLOCK     9

/*timer control*/
#define DDV_SET_TIMER 1
#define DDV_TEST_TIMER 2
/*action associated with the timer*/
#define DDV_INVAL_REGION 1
#define DDV_NOTHING 2

/******************************************************************************

	Data structures

******************************************************************************/
/*used to display the caret (editor mode only)*/
typedef struct ddvcaretinfo{
	Int4  old_row;/*zero-based values; display coordinates*/
	Int4  old_col;
	Int4  new_row;/*zero-based values; display coordinates*/
	Int4  new_col;
	Uint1 style; /*see DDV_CARET_* defines */
} DDVCaretInfo, PNTR DDVCaretInfoPtr;

/*used to remember the selected row (editor mode only)*/
typedef struct ddveditrowinfo{
	Int4  curEditRow;/*zero-based values; row for applying delete, move*/
	Int4  curMasterRow;/*zero-based values; this row is the master*/
} DDVEditRowInfo, PNTR DDVEditRowInfoPtr;

typedef struct ddv_global{
	DDV_ColorGlobal * colorp;
} DDV_Global, PNTR DDV_GlobalPtr;

typedef struct ddvmenu {
	/*file menu*/
	MenU File;
	IteM FileOpen;/*open file command*/
	IteM EntrezOpen;/*open from ID1 command*/
	IteM FileClose;/*close file command*/
  IteM SaveEdits;/* save changes made in editor*/
	IteM FileExport;/*export a seqalign*/
    IteM ImportSeq;/* import a sequence */
    IteM ImportNucSeqAlign;/* import a nuc SeqAlign */
    IteM ImportProtSeqAlign;/* import a prot SeqAlign */
    IteM Hide;/* show/hide dialog */
	IteM QuitViewer;/*close the viewer*/
	/*Edit menu*/
	MenU Edit;
	IteM DeleteRow;
	IteM MoveRowLeft;
	IteM MoveRowRight;
  IteM LeftBoundaryLeft;
  IteM LeftBoundaryRight;
  IteM RightBoundaryLeft;
  IteM RightBoundaryRight;
  IteM DeleteBlock;
  IteM CreateBlock;
  IteM Prev;
  IteM Next;
  IteM Orig;
    /*Align menu */
    MenU Align;
    MenU AddRow;
    MenU FromFile;
    MenU FromNet;
    IteM Deselect;
    IteM BlastFile;
    IteM BlastNet;
	/*Options menu*/
	MenU Options;
	IteM DispStyles;/*display styles*/
	ChoicE MouseMode;/*mouse mode (query, selection, edit)*/
    ChoicE Justify;/*justify (left, right, center)*/
    IteM ShowLowerPanel;
	IteM ConfigNet;/*Entrez Network Conf. dlg*/
  IteM LaunchEditor;
  IteM MergeBlocks;
  IteM SplitBlock;
  IteM DeleteDDVBlock;
	} DdvMenu, PNTR DdvMenuPtr;

typedef struct ddvmsadata {
	MsaParaGPopList pgp_l;/*ParaG list*/
	Uint2			entityID;/*currently displayed SeqAlign*/
	Uint2			itemID;/*  */
	} DdvMSAData, PNTR DdvMSADataPtr;

typedef struct ddvmainwin {
	Boolean UseNetwork;
		/*main win menu*/
	DdvMenu	MainMenu;/*menu command list*/
	ButtoN  gotoBtn;
	TexT    gotoValRow;
	TexT    gotoValCol;
	GrouP   StatusGroup;
	GrouP   StatusGroupLower;
	Boolean	Show_logo;
	FonT	f1;	/*tree fonts used to display the software logo*/
	FonT	f2;
	FonT	f3;
		/*viewer data used only when AutonomeViewer is TRUE */
	Boolean	AutonomeViewer;/*viewer is standalone ?*/
	WindoW	hWndMain;/*handle to the main window*/
	PaneL   hWndDDV;/*handle of the current DDV panel*/
    PaneL   hUpperPanel;/*handle of the upper DDV panel (the multiple)*/
    PaneL   hLowerPanel;/*handle of the DDV panel (the pairwise) */
	PrompT   InfoPanel;
	PrompT   UpperInfoPanel;
	PrompT   LowerInfoPanel;
		/*use to open a file*/
	UdvFetchSeqEntryProc  fetchSepProc;/*function to get a gi over the Network*/
	Nlm_ItmActnProc   NetCfgMenuProc;
	StartNetworkProc  NetStartProc;
    Boolean EditAllowed;  /* editing is allowed */
	}DdvMainWin,PNTR DdvMainWinPtr;

typedef struct ddvtimerdata {
	Int4 col;/*one-based; disp coord*/
	Int4 row;/*one-based; disp coord*/
	Int2 delay;
	Uint1 status;
	Uint1 action;
}DdvTimerData, PNTR DdvTimerDataPtr;

typedef struct ddvmain {
    WindoW             hParent;
    PaneL              hWndDDV;/*panel's handle*/
    UnDViewerGraphData GrData;/*graphical data*/
    DdvMSAData         MSA_d;/*data to display*/
    DDV_Global         Globals;/*shared color data, among others*/
    UDV_mouse_select   ms;/*mouse selection info*/
    DDV_Disp_Opt       ddo;
    DDVCaretInfo       dci;/*caret position and style (editor only)*/
    DDVEditRowInfo     deri;/*some rows info to apply edit commands*/
    DdvTimerData       dtd;/*last goto position*/
    Uint1              MouseMode;
    Uint1              SavedMouseMode;
    Uint2              userkey;
    Uint2              procid;
    Uint2              proctype;
    Int4               MasterViewer;/*the viewer launching ddv*/
    Boolean	           bEditor;/*true if use DDV with editor functions*/
    DDE_StackPtr       dsp;/*data for editor*/
    Int4               BlockIndex;/*the first block of merge 2 blocks*/
    Int4               SaveCol;/*col where split line is drawn*/
    		/*SAP list when open a File or fetch an DB entry*/
	ValNodePtr		vnp_ali;/*SeqAlign List*/
	DdvOpenData        dod;/*what is open in DDV*/	
	} DdvMain, PNTR DdvMainPtr;

#define NUMARGS 2
extern Args DDV_myargs[NUMARGS];

#ifdef __cplusplus
}
#endif

#endif /* ndef _DDVMAIN_ */

