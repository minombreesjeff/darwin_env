/*   undviewer.h
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
* File Name:  undviewer.h
*
* Author:  Patrick Durand
*
* Version Creation Date:   5/3/99
*
* $Revision: 6.57 $
*
* File Description: 
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: udviewer.h,v $
* Revision 6.57  2000/07/12 15:38:08  hurwitz
* made rectangle select much faster.  it's almost working.
*
* Revision 6.56  2000/07/08 20:44:01  vakatov
* Get all "#include" out of the 'extern "C" { }' scope;  other cleanup...
*
* Revision 6.55  2000/06/27 20:46:38  hurwitz
* fixed bugs with select rectangle, added select row option
*
* Revision 6.54  2000/06/26 19:32:17  hurwitz
* can select a rectangle instead of a single row, auto horizontal scroll works but not vertical scroll yet
*
* Revision 6.53  2000/06/16 14:57:02  lewisg
* move entrez calls out of desktop
*
* Revision 6.52  2000/05/19 13:48:31  hurwitz
* made a version of DDE that doesn't allow aligned gaps, changed wording for adding new rows
*
* Revision 6.51  2000/05/15 23:39:34  lewisg
* shred cblast, add menu items for gapped/ungapped, fix pdbheaders
*
* Revision 6.50  2000/04/28 13:51:20  kans
* db_list is a PopuP, not a scrolling LisT
*
* Revision 6.49  2000/04/27 22:21:58  lewisg
* misc bugs/features
*
* Revision 6.48  2000/04/27 19:47:31  kans
* implemented udv reset and select (PD+JK)
*
* Revision 6.47  2000/04/13 13:58:03  durand
* allowed udv to display reverse complement sequence
*
* Revision 6.46  2000/04/11 13:19:26  durand
* added UDV_LoadSpecificEditor for Sequin
*
* Revision 6.45  2000/04/10 21:41:26  lewisg
* move alignment menus into ddv, udv from cn3d
*
* Revision 6.44  2000/04/10 20:33:40  lewisg
* fix show/hide for blast multiple, make blast multiple API generic
*
* Revision 6.43  2000/04/05 21:13:20  kans
* more changes to allow udv panel to be added to Sequin viewer
*
* Revision 6.42  2000/04/05 20:52:36  hurwitz
* added GUI control for shifting left and right alignment boundaries
*
* Revision 6.41  2000/04/03 22:26:32  hurwitz
* can now shift a row with click and drag
*
* Revision 6.40  2000/03/31 13:51:21  durand
* added UDV_InitForSequin()
*
* Revision 6.39  2000/03/30 19:54:09  kans
* initial work to get UDV and DDV into Sequin viewer
*
* Revision 6.38  2000/03/28 21:03:14  hurwitz
* added gui control to re-order rows
*
* Revision 6.37  2000/03/15 19:32:20  lewisg
* launch only single udv window
*
* Revision 6.36  2000/03/06 14:00:47  durand
* first release of the Summary viewer done
*
* Revision 6.35  2000/03/02 15:41:46  lewisg
* move StartNetworkProc typedef to samutil.h
*
* Revision 6.34  2000/03/01 22:49:41  lewisg
* import bioseq, neatlyindex, get rid of dead code
*
* Revision 6.33  2000/02/16 22:38:30  durand
* fixed some wierd behaviours of features selections
*
* Revision 6.32  2000/02/16 16:00:33  durand
* added UDVprocid and GVprocid to vmp data structure
*
* Revision 6.31  2000/02/15 22:40:58  lewisg
* add ability to launch udv so that it colors by row, fixes to colormgr, track rows from viewmgr, fix visual c projects
*
* Revision 6.30  2000/01/11 15:03:18  durand
* remove network stuff
*
* Revision 6.29  2000/01/10 15:18:40  durand
* use Entrez instead of ID1
*
* Revision 6.28  2000/01/05 21:06:38  durand
* update mouse click actions and DrawSequence function for a better use from ddv and cn3d
*
* Revision 6.27  2000/01/03 21:06:38  durand
* update the way the udv main window is created
*
* Revision 6.26  1999/12/03 23:17:23  lewisg
* Patrick's new global update msg, argument passing when launching ddv, experimental editing
*
* Revision 6.25  1999/11/29 15:17:55  durand
* designed a new GUI; fixed problems under Win95 and Linux
*
* Revision 6.24  1999/11/19 15:01:47  durand
* speed up mouse selection ; avoid sequence flashing during selection ; update menu functionalities
*
* Revision 6.23  1999/11/18 14:54:23  durand
* add a new function to create an image map given a ParaG structure - Entrez sequence viewer only
*
* Revision 6.22  1999/11/09 21:06:58  durand
* add sequence selection manager
*
* Revision 6.21  1999/10/21 13:00:33  durand
* add new fields in UDV_mouse_select struct
*
* Revision 6.20  1999/10/20 20:07:44  durand
* add first_row and first_col to UDV_mouse_select
*
* Revision 6.19  1999/10/02 15:11:16  durand
* update the code to be used by wwwudv
*
* Revision 6.18  1999/09/29 20:09:51  durand
* modify some parts of the code to be used by the cgi-bin release of UDV
*
* Revision 6.17  1999/09/22 14:26:50  durand
* add fields for DDV mouse management
*
* Revision 6.16  1999/09/20 21:57:41  durand
* switch UDV_draw_double_cursor from static to NLM_EXTERN
*
* Revision 6.15  1999/09/08 15:36:58  beloslyu
* Typo fixed
*
* Revision 6.14  1999/09/08 14:09:32  durand
* update UDV_Draw_scale to deal with discontinuous SEqAlign
*
* Revision 6.13  1999/07/30 20:08:56  durand
* updates for the new Entrez graphical viewer
*
* Revision 6.12  1999/07/19 20:35:36  durand
* switch ScalePositionfrom from Boolean to Uint1 in UDV_Draw_scale
*
* Revision 6.11  1999/07/09 13:56:56  durand
* removed all the defines to udvdef.h
*
* Revision 6.10  1999/07/06 17:02:59  durand
* add UDVPanelRegion structure for DDV
*
* Revision 6.9  1999/06/16 22:11:24  durand
* update UDV functions to be used by DDV
*
* Revision 6.8  1999/06/16 13:07:00  durand
* update UDV functions to be used by DDV
*
* Revision 6.7  1999/06/15 15:25:53  durand
* remove DDV_win in UDVPanelData structure
*
* Revision 6.6  1999/06/08 13:52:36  durand
* update UDV data structures for the MSA editor
* ==========================================================================
*/

#ifndef _UNDVIEWER_
#define _UNDVIEWER_


/*******************************************************************************

  INCLUDE SECTION 

*******************************************************************************/
#include <explore.h>
#include <gather.h>
#include <ncbi.h>
#include <ncbistr.h>
#include <objfdef.h>
#include <objseq.h>
#include <objsub.h>
#include <seqmgr.h>
#include <seqport.h>
#include <sequtil.h>
#include <sqnutils.h>
#include <vibrant.h>
#include <bspview.h>

#include <odlbox.h>
#include <udvseq.h>
#include <udvdef.h>

#include <ddvcolor.h>
#include <samutil.h>


#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************

  The major elements of the single sequence viewer : window and paragraph 

*******************************************************************************/

	/*****************************************************************************

	Structure of the UnDViewer area

	-Panel--------------------------------------------------    -
	|         |             |                              |    |
	| Name    | Scale area  |                              |    |
	| area    | (if left    |    Sequence area             |    |
	|         |  scale)     |    (paragraph)               |    cyClient
	|         |             |                              |    |
	|         |             |                              |    |
	--------------------------------------------------------    -
	<-cxName->
        	  <-cxLeftScale->
	<-------------------------cxClient--------------------->

	*****************************************************************************/

	/*****************************************************************************

	Structure of a ParaG : see module udvseq.h

	*****************************************************************************/


/*******************************************************************************

  DATA STRUCTURE SECTION 

*******************************************************************************/
	typedef struct udvscaledata {
		Uint4 ScaleColor;	/*letter color*/
		Uint4 TickMajColor;	/*tick color*/
		Uint4 TickMinColor;	/*tick color*/
		Boolean ShowMajorTick;	/*display tick marks if TRUE*/
		Boolean ShowMMinorTick;	/*display tick marks if TRUE*/
		Int2 ScalePosition;	/*position of the scale, see #define above*/
		Int2 cxLeftScale;	/*width of the 'SCALE_POS_LEFT' area, pixel unit
							NULL if ScalePosition== _TOP or if ShowScale==FALSE*/
		Int2 MajorTickEvery;	/*put a major tick every*/
		Int2 MinorTickEvery;	/*put a minor tick every*/
		} UDVScaleData, PNTR UDVScaleDataPtr;

	typedef struct udvletterlayout {
		Uint4 LetClr;		/*color of a letter*/
		Uint4 bkClr;		/*bk color of a letter*/
		Uint1 AspLet;		/*aspect (uppercase,...)*/
		} UDVLetterLayout, PNTR UDVLetterLayoutPtr;

	typedef struct udvfontdata {
		FonT hFnt;			/*Current Font*/
		Boolean UseDefaultColorLetter; /*if true, use the following color for 
							letter (NA or AA); draft mode*/
		Uint4 LetterColor;
		Int2 cxChar;		/*width of a char, pixel unit*/
		Int2 cyChar;		/*height of a char, pixel unit*/
		Int2 LineHeight;	/*height of a single data line*/
		Int2 ColWidth;/*width of a single data column-MSA only*/
		} UDVFontData, PNTR UDVFontDataPtr;

	/*the following structure was added for DDV. It's here (and not in the
	code of DDV), because that struct is part of UDVPanelData*/
	typedef struct udvpanelregion{/*descriptor of panel regions : ruler,
									names list, ParaG region, etc.*/
		RecT  region;/*size of the region*/
		Uint1 type;  /*type of the region; see above*/
		} UDVPanelRegion, PNTR UDVPanelRegionPtr;

	typedef struct udvpaneldata {
		ValNodePtr region;/*descriptor of the panel content; each 
						region->data.ptrvalue contains a pointer to UDVPanelRegion*/
		Int4 nTotLines;		/*total number of line to display*/
		Int4 nTotCol;/*total number of column to display-MSA only*/
		/*Display options*/
		Boolean ShowFeatures; /*TRUE: show features*/
		Boolean ShowScale;	/*TRUE: show num. scale*/
		Boolean ShowByBlock;/*TRUE: use nBlockByLine -MSA only*/
		/*panel size*/
		Int2 cxClient;		/*width of the client panel area, pixel unit*/
		Int2 cyClient;		/*height of the client panel area, pixel unit*/
		Int2 cxName;		/*width of the 'Name' area, pixel unit*/
		Int2 cyScale;		/*height of the 'Sacle' area, pixel unit (MSA only)*/

		/*Letter lines*/
		Int2 nCharByLine;	/*number of letter displayed on one line of the viewer*/
		Int2 nBlockByLine;	/*number Ten-letter blocks displayed on one line of 
							the viewer*/
		} UDVPanelData, PNTR UDVPanelDataPtr;

	typedef struct udvscrolldata {
		/*Scroll Bar*/
		Int4 ScrollMax;		/*Size of the vertical Scroll Bar*/
							/*also equal number of data lines displayed within
							UnDViewer*/
		Int4 ScrollPos;		/*Current position within the vertical Scroll Bar*/
		Int4 ScrollPage;	/*Page decal of the vertical Scroll Bar*/
		} UDVScrollData, PNTR UDVScrollDataPtr;

	typedef struct undviewergraphdata {
		UDVFontData 	udv_font;	/*Font*/
		UDVPanelData 	udv_panel;	/*Panel*/		
		UDVScaleData	udv_scale;	/*Numerical scale*/
		UDVScrollData	udv_vscrl;	/*vertical Scroll Bar*/
		UDVScrollData	udv_hscrl;	/*horizontal Scroll Bar-MSA only*/
		Uint4Ptr 		pClr;		/*Color table for Features*/
		Uint4			DisplayOptions;/*bits vector to encode display options*/
		UDVLetterLayout NA_LayoutPal[5];
		UDVLetterLayout AA_LayoutPal[26];
		Int4            GotoLetter;/*letter to highlight after a GoTo action; one-based value*/
		} UnDViewerGraphData, PNTR UnDViewerGraphDataPtr;

	typedef struct udv_item_select {
		Uint2		eIDsel;			/*-|               */
		Uint2		iIDsel;			/* |--selected item*/
		Uint2		iTypeSel;		/*-|               */
		} UDV_Item_Select, PNTR UDV_Item_SelectPtr;
		
  typedef struct udv_mouse_select {
    Int1      Action_type;      /*action with the mouse*/
    RecT      rcClip;           /*limit mvt of the mouse to this rc*/
    PoinT     oldPos;           /*old pos of the mouse*/
    PoinT     newPos;           /*new pos of the mouse*/
    ParaGPtr	pgp;              /*current ParaG data*/
    Char      szPos[20];        /*current position within the bsp*/
    Int4      old_row;          /*used by DDV only (display coords); 
							                    set when 'on_click' and every 'on_drag'*/
    Int4      old_col;          /*
                                  set when 'on_click' and every 'on_drag'*/
    ParaGPtr    old_pgp;        /*pgp where old_col was located*/
    Int4        first_row;      /*set only when 'on_click'*/
    Int4        first_col;      /*set only when 'on_click'*/
    Int4        last_row;       /*save corner of rectangle*/
    Int4        last_col;       /*save corner of rectangle*/
    Int4*       first_cols;     /*array for multiple rows*/
    ParaGPtr    first_pgp;      /*pgp where first_col was located*/
    ParaGPtr*   first_pgps;     /*array for multiple rows*/
    PoinT       first_point;    /*point where first clicked*/
    Int4        first_HScroll;  /*horizontal scroll position when first clicked*/
    Int4        first_VScroll;  /*vertical scroll position when first clicked*/
    Int4        old_HScroll;    /*horizontal scroll when select rectangle last drawn */
    Int4        old_VScroll;    /*vertical scroll when select rectangle last drawn */
    RecT        outline;        /*rectangle when making selection*/
  } UDV_mouse_select,PNTR UDV_mouse_selectPtr;

	typedef  struct  scanfeat {/*use to scan feature when the user clicks
						on a feature*/
		Uint2 eID;		/*identity of the feature*/
		Uint2 iID;		
		Int4 index;		/*position within the Feature List Box*/
		} ScanFeat, PNTR ScanFeatPtr;

	typedef struct udvtimerdata {/*used with the UDV timer*/
		Int4 pos;/*one-based; bsp coord*/
		Int2 delay;
		Uint1 status;
		Uint1 action;
	}UDVTimerData, PNTR UDVTimerDataPtr;

	typedef struct viewerdialogdata {
		Uint2 				procid;		/*identification of ObjMgr User Data*/
		Uint2 				proctype;	/* idem */
		Uint2 				userkey;	/* idem */
		BspInfo				bsp_i;		/*bsp information*/
		UnDViewerGraphData 	udv_graph;	/*UnDviewer graphical panel info*/
		ValNodePtr 			ParaG;		/*ParaG data of the entire display*/
		PaneL				UnDViewer;	/*handle of the seq viewer panel*/
		PrompT				InfoPanel;	/*handle of the info panel*/
		WindoW              Parent;  /*only valid for Autonomous viewer*/
		UDV_mouse_select	UDV_ms;		/*used for mouse manipulation*/
		UDV_Item_Select		Item_select;/*selected Item*/
		UDV_Item_Select		Old_Item_select;/*old selected Item*/
		Boolean				ClickFeatFromDlg;/*click from Feat List Dlg==TRUE*/
		UDVTimerData		udt;
        SAM_ViewGlobal     *vgp;  /*added for data exchange with Cn3D*/
        BioseqViewPtr       bvp;  /*added for data exchange with Sequin*/
		Boolean             bDisplayRevComp;/*TRUE : diasplay the reverse
		    complement of a sequence. Nuc. only.*/
		} ViewerDialogData, PNTR ViewerDialogDataPtr;

	typedef struct udvmainmenu {/*handles of the main menu commands*/
		MenU File;
		MenU Options;
		MenU Help;
        MenU Align;
		IteM FileOpen;		/*open file command*/
		IteM EntrezOpen;	/*open from Entrez command*/
		IteM FileClose;		/*close file command*/
		IteM ListSequence;	/*list of available sequence*/
		IteM QuitViewer;	/*close the viewer*/
		IteM ShowFeature;	/*show feature on/off command*/
		IteM ShowFeatureList;	/*show feature on/off command*/
		IteM SearchForFeature;/*search features by keyword*/
		IteM ShowRevComp;	/*show reverse complement (nuc. seq. only)*/
		MenU ScalePos;		/*scale position command*/
		ChoicE ScalePosChoice; /*scale pos value (sub-menu)*/
		IteM RefreshScreen;	/*redraw the entire screen*/
    MenU AddRow;
    MenU FromFile;
    MenU FromNet;
    IteM BlastMany;
		IteM ConfigNet;/*Entrez Network configuration*/
		IteM HelpAbout;	/*what's that ?*/
	} UDVMainMenu, PNTR UDVMainMenuPtr;

	typedef struct udvlogodata {
		FonT	f1;
		FonT	f2;
		FonT	f3;
		Char	szTitle[25];
		Char	szDesc[80];
	} UDVLogoData, PNTR UDVLogoDataPtr;
		
	typedef struct viewermain {
		Boolean             UseNetwork;/*true if UDV is connected to Entrez*/
		Uint2               UDVprocid;
		Uint2               GVprocid;
			/*main win menu*/
		UDVMainMenu			MainMenu;		/*menu command list*/
			/*Features List DlgBox*/
		WindoW				hFeatDlg;
			/*Panel component*/
		ButtoN              gotoBtn;
		TexT                gotoVal;
		PaneL				Logo_Panel;
		Boolean				Show_logo;
		GrouP               StatusGroup;
			/*viewer data used only when AutonomeViewer is TRUE */
		Boolean				AutonomeViewer;
		Pointer				dataptr;		/*identification of the object*/
		Uint2				datatype;		/*idem */
		Uint2				entityID;		/*idem */
		ValNodePtr			BspTable;		/*list of bsp*/
		Uint2				BspChoice;		/*selection in menu (ONE-based)*/
		WindoW				hWndMain;
			/*BSP displayed in the viewer*/
		ViewerDialogDataPtr vdp;		
		} ViewerMain, PNTR ViewerMainPtr;

#define REGISTER_UDV_AUTONOMOUS ObjMgrProcLoad(OMPROC_VIEW, \
		"OneD-Viewer", "SingleSeqViewer", OBJ_BIOSEQ, 0, OBJ_BIOSEQ, 0, \
		NULL, UDV_ObjRegAutonomous, 0)	
#define REGISTER_UDV_SEQENTRY ObjMgrProcLoad(OMPROC_VIEW, \
		"OneD-Viewer SE", "SingleSeqViewer SE", OBJ_BIOSEQSET, 0, OBJ_BIOSEQSET, 0, \
		NULL, UDV_ObjRegAutonomous, 0)	

	/*struture passed to the FEAT List dialog box*/
	typedef  struct  flmdata {
		WindoW 		hWndMain;	/*UnDviewer*/
		WindoW 		lbox;		/*list of features (OwnerDraw Lbox)*/
		GrouP		gFeatClass;	/*Hidden group of the Feat Class popup*/
		PopuP		pop;	/*available class list*/
		Int1Ptr		SeqFeatClass;/*classes reported for a sequence*/
		} FLMData, PNTR FLMDataPtr;

	/*use to initialize the Feature List Dialog box*/
	typedef  struct  scanfeatforselect {
		Uint2 	eID;	/*if !=-1, UnDviewer slected feature*/		
		Uint2 	iID;		
		Int4 	index;	/*return the index of the UnDviewer slected feature*/
		Int4 	compteur;/*count the number of entries in Feature List DlgBox*/
		WindoW 	lbox;	/*the list itself*/
		Boolean SeqFeatListAvail[SEQFEAT_MAX+1];/*use to retrieve the feature
				classes*/
		Boolean SearchFeat;
		Char    szKeyword[50];
		} ScanFeatForSelect, PNTR ScanFeatForSelectPtr;

	/*
	*  The UdvGlobalsPtr may be registered with a call to SetAppProperty
	*  e.g., SetAppProperty ("UdvGlobals", &udvglobals), where udvglobals
	*  is a persistent structure filled with Vibrant objects or callback
	*  function pointers specific for a given application.
	*/

	typedef SeqEntryPtr (*UdvFetchSeqEntryProc) (Int4 uid, Int2 retcode);

	typedef  struct  udvglobals {
		ViewerMainPtr         vmp;
		UdvFetchSeqEntryProc  fetchSepProc;
		Nlm_ItmActnProc   NetCfgMenuProc;
		StartNetworkProc  NetStartProc;
		} UdvGlobals, PNTR UdvGlobalsPtr;

    /*
     * structs used only by the Blast sequence dialog box
     */

#if 0 

    struct _UDV_BlastDlgData;
    typedef void (*UDV_BlastProcCB)(struct _UDV_BlastDlgData *bddp);

    typedef struct _UDV_BlastDlgData {
        ValNode *pvnSips;		/* list of sips in structure */
        LisT bsp_list;		/* listbox of bsps */
        PopuP db_list;		/* listbox of databases */
        ButtoN bGap;   /* should the alignment be gapped? */
        TexT tMax;
        TexT tExpect;
        UDV_BlastProcCB callback;  /* called at end of alignment */
        Boolean IBM; /* should IBM be applied? */
        SeqAlign *salp;  /* resulting seqalign */
    } UDV_BlastDlgData;

#endif /* 0 */

/*******************************************************************************

  EXTERNAL FUNCTIONS SECTION (see the .c files of UnDViewer for a complete
	  description of each function)

*******************************************************************************/
	/*drawing and UDV graphical management*/
	NLM_EXTERN ValNodePtr UDV_GetSelectedRegions(SelStructPtr om_ssp, Uint2 bsp_eID,
		Uint2 bsp_iID);
	NLM_EXTERN Boolean UDV_IsLetterSelected(ValNodePtr vnp_bsp, Int4 bsp_pos,
		Int4 bspLength,Boolean bDisplayRevComp);
	NLM_EXTERN SeqLocPtr UDV_GetClosetSeqLocGivenBspPos(SeqIdPtr sip, Uint2 eID, 
		Uint2 iID, Int4 bsp_pos, Int4Ptr old_pos, Boolean bModify);
	NLM_EXTERN void UDV_InvalRegion(PaneL UnDViewer,UnDViewerGraphDataPtr GrData,
		ParaGPtr pgp,Int4 start_inval,Int4 stop_inval,Boolean IsSelect);
	NLM_EXTERN void	UDV_GetCurrentDispRange(ViewerDialogDataPtr vdp,
		Int4Ptr from_bsp,Int4Ptr to_bsp,Int4Ptr from_line,Int4Ptr to_line);
	NLM_EXTERN void LogoFontCreate(FonT PNTR f1,FonT PNTR f2,FonT PNTR f3);
	NLM_EXTERN FonT UDV_InitFont(UDVFontDataPtr udvfp);
	NLM_EXTERN Int2 UDV_ComputeLineHeight(Int2 cyChar);
	NLM_EXTERN void UDV_FontDim(Int2Ptr cxChar,Int2Ptr cyChar);
	NLM_EXTERN void UDV_ComputePanelSize(RecT rc,Int2Ptr cxClient,
				Int2Ptr cyClient);
	NLM_EXTERN void UDV_ComputeBlockByLine(Int2 cxClient,Int2 cxName,
				Int2 cxLeftScale,Int2 cxChar,Int2Ptr nCharByLine,
				Int2Ptr nBlockByLine);
	NLM_EXTERN void UDV_Build_Other_Colors(UnDViewerGraphDataPtr GrDataPtr);
	NLM_EXTERN void  UDV_Init_ScaleData(UnDViewerGraphDataPtr GrDataPtr);
	NLM_EXTERN void UDV_Init_GraphData(PaneL p, 
											UnDViewerGraphDataPtr GrDataPtr);
	NLM_EXTERN Uint4Ptr UDV_BuildFeatColorTable(void);
	NLM_EXTERN void UDV_Build_NA_LayoutPalette(UnDViewerGraphDataPtr GrData);
	NLM_EXTERN void UDV_Build_AA_LayoutPalette(UnDViewerGraphDataPtr GrData);
	NLM_EXTERN void UDV_deselect_feature(ViewerDialogDataPtr vdp);
	NLM_EXTERN void UDV_SelectFeatInFeatDlg(ViewerMainPtr vmp, 
			Uint2 entityID, Uint2 itemID);
	NLM_EXTERN void UDV_select_feature(PaneL p,ViewerDialogDataPtr vdp,
			Uint2 entityID,Uint2 itemID,Boolean bRepos);
	NLM_EXTERN void UDV_draw_double_cursor(RecT rcClip,PoinT pos);
  NLM_EXTERN void UDV_draw_horizontal_line(RecT rcClip, Int4 VPos);
  NLM_EXTERN void UDV_draw_rectangle(RecT rcClip, Boolean DotIt);
  NLM_EXTERN void UDV_draw_vertical_bar(RecT rcClip, Int4 HPos, Boolean DotIt);
  NLM_EXTERN void UDV_draw_horizontal_bar(Int4 VPos, Int4 LeftHPos, Int4 RightHPos);
	NLM_EXTERN void UDV_ClickProc(PaneL p, PoinT pt);
	NLM_EXTERN void UDV_DragMouse(PaneL p,ViewerDialogDataPtr vdp, PoinT pt);
	NLM_EXTERN void UDV_DragProc(PaneL p, PoinT pt);
	NLM_EXTERN void UDV_ReleaseMouse(PaneL p,ViewerDialogDataPtr vdp, PoinT pt);
	NLM_EXTERN void UDV_ReleaseProc(PaneL p, PoinT pt);
	NLM_EXTERN void UDV_HoldProc(PaneL p, PoinT pt);
	NLM_EXTERN void UDV_Draw_features(UnDViewerGraphDataPtr GrData,
		BspInfoPtr bsp_i,Boolean UseDefClr,Uint4 DefClr,
		ParaGPtr pgp,RecT PNTR rc,Uint4Ptr pClr,
		UDV_Item_Select * is,UDV_Item_Select * old_is,Uint4 DisplayType);
	NLM_EXTERN Boolean UDV_Draw_features_MAP(UnDViewerGraphDataPtr GrData,
					BspInfoPtr bsp_i,ParaGPtr pgp,RecT PNTR rc,Uint2 entityID,
					ValNodePtr PNTR vnpp_map);
	NLM_EXTERN void UDV_Draw_sequence(UnDViewerGraphDataPtr GrData,
        DDV_ColorGlobal *svpp ,Boolean UseDefClr,Uint4 DefClr,
		ParaGPtr pgp,RecT PNTR rc,Int4 start_decal,Int4 StartLetter,
		CharPtr szSequence, SeqId *sip,ValNodePtr vnp_bsp,Boolean bSelect,
        Int4 Row,Int4 bspLength,Boolean bDisplayRevComp);
	NLM_EXTERN void UDV_Draw_scale(UnDViewerGraphDataPtr GrData,
		Boolean ShowMajorTick,Boolean ShowMMinorTick,Uint1 ScalePosition,
		Int4 StartLetter,Int4 StopLetter,RecT PNTR rc,Int2 LeftDecal,
		Int4 ScaleMaxVal,Int4 AlignPos,Boolean UseBlockDisp,Int2 ColWidth,
		Uint4 DisplayType);
	NLM_EXTERN void UDV_draw_viewer (PaneL p);
	NLM_EXTERN void UDV_Logo_onDraw (PaneL p);

	/*Features List Dialog Box*/
	NLM_EXTERN Boolean LIBCALLBACK UDV_FeaturesListBoxFind (SeqFeatPtr sfp, 
			SeqMgrFeatContextPtr context);

	/*UDV main window management*/
	NLM_EXTERN Int2 LIBCALLBACK UDV_ObjRegAutonomous (Pointer data);
	NLM_EXTERN void UDV_WinMainCleanupExtraProc (GraphiC g, VoidPtr data);
	NLM_EXTERN void UDV_Resize_Logo_Panel (WindoW Parent,RecT PNTR rcL);
	NLM_EXTERN void * UDV_FreeVDPstruct(ViewerDialogDataPtr vdp,Boolean final);
	NLM_EXTERN void UDV_set_PullMenus(UDVMainMenuPtr mmp,Boolean enable);
	NLM_EXTERN void UDV_set_MainMenus(UDVMainMenuPtr mmp,Boolean enable);
	NLM_EXTERN void UDV_set_MainControls(ViewerMainPtr vmp,Boolean enable);
	NLM_EXTERN void UDV_SetupMenus(WindoW w,Boolean isEntrezOk,SAM_ViewGlobal *vgp);
	NLM_EXTERN void	UDV_resize_viewer(PaneL p,ViewerDialogDataPtr vdp);
	NLM_EXTERN void UnDViewerVScrlUpdate(PaneL p,Boolean bInit,Int4 CurPos);
	NLM_EXTERN void UDV_WinMainResize(WindoW w);
	NLM_EXTERN Boolean  UDV_init_bsp_forViewer(PaneL p,BioseqPtr bsp,Uint2 eID,
			Uint2 iID,Uint2 itype,ViewerDialogDataPtr vdp);
	NLM_EXTERN Boolean UDV_Init_NonAutonomous(PaneL p,
				ViewerDialogDataPtr PNTR vdp,FonT f);
	NLM_EXTERN Boolean CreateMainControls(WindoW w,ViewerMainPtr vmp,SAM_ViewGlobal *vgp);
	NLM_EXTERN void UnDViewerVScrlProc (BaR sb, SlatE s, Int4 newval,  Int4 oldval);
	NLM_EXTERN void UDVResetProc (PaneL p);

	/*sequence buffer management*/
	NLM_EXTERN CharPtr UDV_Read_Sequence (SeqIdPtr sip, Int4 from, Int4 to, 
		Boolean IsProt,Int2 len);
	NLM_EXTERN void UDV_create_buffer(UnDViewerGraphDataPtr GrData,
		ValNodePtr ParaG_list,BspInfoPtr bsp_i,ValNodePtr StartScan,
		Boolean bReverse);
		
	/*open a SeqEntry management*/
    NLM_EXTERN Boolean LIBCALLBACK SearchBioseq (BioseqPtr bsp, 
			SeqMgrBioseqContextPtr context);
	NLM_EXTERN Boolean  UDV_analyze_SEP_for_open(FILE *fp,SeqEntryPtr the_set,
		ViewerMainPtr vmp,WindoW w);
	NLM_EXTERN void UDV_Init_vdp_struct(PaneL p,ViewerDialogDataPtr vdp, 
		Boolean EraseParaG,Boolean EraseMainTitle,Boolean EraseInfoPanel);
	NLM_EXTERN void UDV_NetOpen (IteM i);
	NLM_EXTERN void UDV_FileOpen(IteM i);
	NLM_EXTERN void UDV_FileClose(IteM i);
	NLM_EXTERN void UDV_CreateListBioseqDlg(IteM i);

	/*clean quit*/
	NLM_EXTERN void UDV_WinMainProgQuit(WindoW w);

	/*Sequin stuffs*/
	NLM_EXTERN Boolean UDV_InitForSequin(PaneL udvPanel, BioseqViewPtr bvp);
	NLM_EXTERN void UDV_LoadSpecificEditor(BioseqViewPtr bvp, Uint2 entityID, 
		Uint2 itemID, Uint2 itemtype);

  /*zero UDV_mouse_select structure*/
  NLM_EXTERN void ClearUDV_mouse_select(UDV_mouse_selectPtr msp);

#if 0
    /* blast dialog */
    NLM_EXTERN void UDV_BlastDlg(UDV_BlastDlgData *bddp);
#endif


#ifdef __cplusplus
}
#endif

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

#endif /* ndef _UNDVIEWER_ */
