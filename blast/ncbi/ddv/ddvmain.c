/*  $Id: ddvmain.c,v 1.26 2000/07/17 13:32:33 lewisg Exp $
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
* File Name:  ddvmain.c
*
* Author:  Patrick Durand
*
* Version Creation Date:   06/19/99
*
* $Revision: 1.26 $
*
* File Description: starter module of DeuxD-Viewer (DDV). Onlu use to
* start DDV as a standalone software.
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: ddvmain.c,v $
* Revision 1.26  2000/07/17 13:32:33  lewisg
* move DDV_args out of the library
*
* Revision 1.25  2000/07/17 12:38:53  kans
* DDV_myargs is extern in header, instantiated in ddvpanel.c, since it is accessed from that library file
*
* Revision 1.24  2000/07/14 22:24:55  lewisg
* fix scroll, click, visual c++ build bugs.  add command line arg parse to ddv
*
* Revision 1.23  2000/07/05 19:23:13  lewisg
* add two panes to ddv, update msvc project files
*
* Revision 1.22  2000/06/06 14:44:16  hurwitz
* fixed bug that SetAppProperty(dde_nogaps)
*
* Revision 1.21  2000/05/31 23:07:26  hurwitz
* made NoGaps a runtime parameter, fixed bug with vertical scroll of show/hide list, save edits query is not performed if nothing to save
*
* Revision 1.20  2000/04/21 23:00:50  hurwitz
* can launch DDE from DDV
*
* Revision 1.19  2000/04/17 13:30:43  durand
* removed g_hParent and unused functions DDV_LaunchAlignViewer and DDV_LaunchAlignEditor
*
* Revision 1.18  2000/03/30 21:58:06  durand
* made standalone DDV a viewer, not the editor
*
* Revision 1.17  2000/03/21 14:22:42  durand
* fixed a problem with menus setup
*
* Revision 1.16  2000/03/16 18:38:38  durand
* fixed the starter function for the GUI
*
* Revision 1.15  2000/02/04 16:05:41  durand
* add click action to select a row
*
* Revision 1.14  2000/02/02 14:44:32  durand
* added function to create data structure for block editor, fixed some bugs
*
* Revision 1.13  2000/01/26 13:38:55  durand
* update the GUI for the editor. Add functions to create the data to be used by the editor
*
* Revision 1.12  2000/01/11 15:29:48  durand
* add a cast to use EntrezSeqEntryGet instead of ID1SeqEntryGet
*
* Revision 1.11  2000/01/11 15:05:23  durand
* remove network stuff
*
* Revision 1.10  2000/01/10 15:09:46  durand
* Use Entrez instead of ID1
*
* Revision 1.9  2000/01/05 21:11:14  durand
* update mouse click actions and DrawSequence function for a better use from ddv and cn3d
*
* Revision 1.8  1999/12/03 23:17:23  lewisg
* Patrick's new global update msg, argument passing when launching ddv, experimental editing
*
* Revision 1.7  1999/11/29 15:26:25  durand
* designed a new GUI to fix problems under MacOS, Linux and SGI
*
* Revision 1.6  1999/11/04 22:11:38  durand
* add the Desktop to DDV. Add a better set of cleanup functions when closing DDV. Before creating color tables, try to get them from the SeqAlign
*
* Revision 1.5  1999/10/29 14:15:40  durand
* add simple mouse selection functions
*
* Revision 1.4  1999/10/23 21:20:45  lewisg
* move g_hParent to ddvopen.c
*
* Revision 1.3  1999/10/23 14:54:33  durand
* resolve external symbol g_hParent
*
* Revision 1.2  1999/10/22 14:19:43  durand
* update the code for the startup functions of DDV drawing panel
*
* Revision 1.1  1999/09/30 14:10:27  durand
* add ddv to toolkit
*
* Revision 1.7  1999/09/23 19:06:50  lewisg
* increase maxtemp number of cached sequences
*
* Revision 1.6  1999/09/23 17:44:32  lewisg
* increase maxtemp number of cached sequences
*
* Revision 1.5  1999/07/01 15:28:28  durand
* validate function loaders of DDV
*
* Revision 1.2  1999/06/28 22:07:18  durand
* add loader functions and clean the code with Lint and Purify
*
* Revision 1.1  1999/06/19 17:21:05  durand
* add Vibrant DDV code
*
*
*
* ==========================================================================
*/

#include <ncbi.h>
#include <accentr.h>
#include <udviewer.h>
#include <ddvopen.h>
#include <ddvmain.h>
#include <ddvpanel.h>
#include <vibrant.h>
#include <netcnfg.h>

extern Args DDV_myargs[NUMARGS] = {
 /*0*/ { "Gaps allowed",  "T", NULL, NULL, TRUE, 'g', ARG_BOOLEAN, 0.0,0,NULL},
 /*1*/ { "Allow Edits", "T", NULL, NULL, TRUE , 'e', ARG_BOOLEAN, 0.0,0,NULL},
};

/*******************************************************************************

  Function : DDV_ConfigAccepted()
  
  Purpose : Entrez Network COnfiguration Dialog Box; accept a new config
  
  Return value : none 

*******************************************************************************/
static void DDV_ConfigAccepted(void)
{
    SetAppParam("DDV", "SETTINGS", "NETWORKAVAILABLE", "TRUE");
    Message(MSG_OK, "Setting will take affect when you restart DDV");
}

/*******************************************************************************

  Function : DDV_ConfigAccepted()
  
  Purpose : Entrez Network COnfiguration Dialog Box; close dlg without modif.
  
  Return value : none 

*******************************************************************************/
static void DDV_ConfigCancelled(void)
{
}

/*******************************************************************************

  Function : DDV_ConfigTurnedOff()
  
  Purpose : Entrez Network COnfiguration Dialog Box; cancel Entrez connection
  
  Return value : none 

*******************************************************************************/
static void DDV_ConfigTurnedOff(void)
{
    SetAppParam("DDV", "SETTINGS", "NETWORKAVAILABLE", "FALSE");
    Message(MSG_OK, "Setting will take affect when you restart DDV");
}

/*******************************************************************************

  Function : DDV_ConfigNetwork()
  
  Purpose : call the Entrez Network COnfiguration Dialog Box
  
  Return value : none 

*******************************************************************************/
static void DDV_ConfigNetwork(IteM i)
{
DdvMainWinPtr      dmwp;
WindoW             hParent;

	/*get main data block*/
	hParent=(WindoW)ParentWindow(i);
	if (!hParent) return;
	dmwp=(DdvMainWinPtr)GetObjectExtra(hParent);

    if (dmwp->UseNetwork) ShowNetConfigForm(NULL,
        NULL, DDV_ConfigAccepted, DDV_ConfigCancelled, DDV_ConfigTurnedOff, TRUE);
    else ShowNetConfigForm(NULL, NULL, DDV_ConfigAccepted, DDV_ConfigCancelled,
                      DDV_ConfigTurnedOff, FALSE);
}

/*******************************************************************************

  Function : DDV_StartEntrez()
  
  Purpose :  start connection to Entrez Server.
  				    
  Return value : -

*******************************************************************************/
static Boolean DDV_StartEntrez(Boolean UseNetwork)
{
Boolean bRet;

	bRet=FALSE;
	
    if (!UseNetwork) {
		bRet=FALSE; 
	}
	else{
    	if (!EntrezIsInited()) {
        	if(EntrezBioseqFetchEnable(szAppName, FALSE)) {
            	if (EntrezInit(szAppName, TRUE, NULL)) 
					bRet=TRUE;
				else
					bRet=FALSE;
        	}
			else {
				bRet=FALSE;
			}
    	}
		else{
			bRet=TRUE;
		}
	}
	return(bRet);
}


/*****************************************************************************

Function: DDV_UseNetwork()

Purpose:  Determines if DDV should use the network
  
Returns:  TRUE if yes

*****************************************************************************/
static Boolean DDV_UseNetwork(void)
{
Char str[64];

    if (GetAppParam
        ("DDV", "SETTINGS", "NETWORKAVAILABLE", NULL, str, sizeof(str))) {
        if (StringICmp(str, "TRUE") == 0) return TRUE;
    }
    return FALSE;
}

/*******************************************************************************

  Function : DDV_StartMainWin_Master()
  
  Purpose : start DDV main window as a standalone application. Never use this 
  	function for other purpose. If you want to start DDV as a slave, take a
	look at the functions below.
  
  Parameters : 	mWin_d; main program data
  				ldp; logo data

  Return value : handle to the DDV main window

*******************************************************************************/
static WindoW DDV_StartMainWin_Master(DdvMainWinPtr mWin_d,UDVLogoDataPtr ldp)
{
Int2	Margins;
WindoW	w; 
Boolean bRet;
	
	/*main window*/
	Margins=10*stdCharWidth;

	w=DocumentWindow(Margins,Margins ,-10,-10, 
			szAppName, 
			DDV_WinMainProgQuit,
			DDV_WinMainResize);

	if (w==NULL){
		Message (MSG_ERROR, "Viewer creation failed.");
		return(NULL);
	}
	
	SetObjectExtra (w, (Pointer)mWin_d, (FreeProc)DDV_WinMainCleanup);
	SetAppProperty("UDVLogoData",(Pointer)ldp);	

	mWin_d->hWndMain=w;
	/*this is an autonomous viewer*/
	mWin_d->AutonomeViewer=TRUE;

	/*use the Network ?*/
	mWin_d->UseNetwork=TRUE;/*DDV_UseNetwork();*/

	bRet=DDV_CreateViewerPanel(w,mWin_d,NULL,FALSE); /*FALSE for viewer */
	if (!bRet) return(NULL);

	/*init logo_panel*/
	LogoFontCreate(&ldp->f1,&ldp->f2,&ldp->f3);
	StringCpy(ldp->szTitle,"DDV");
	StringCpy(ldp->szDesc,", a MSA viewer/editor");
	mWin_d->Show_logo=TRUE;

	/*set the function to get a gi over the network*/
	mWin_d->fetchSepProc=(UdvFetchSeqEntryProc)EntrezSeqEntryGet;
	mWin_d->NetCfgMenuProc=DDV_ConfigNetwork;
	mWin_d->NetStartProc=DDV_StartEntrez;
		
	return(w);
}

/*******************************************************************************

  Function : Main()
  
  Purpose : Entry point of the software 
  
  Parameters : 
  
  Return value : 

*******************************************************************************/

Int2 Main(void)
{
UDVLogoDataPtr	ldp;
Boolean 		UseNetwork;
WindoW			w; 
DdvMainWinPtr   mWin_d;/*main window data*/
ObjMgrPtr omp = NULL;
Boolean  NoGaps;

	if ( !Nlm_GetArgsSilent("DDV 1.0", NUMARGS, DDV_myargs) ) return 1;
	
	ErrSetMessageLevel(SEV_WARNING);
	ErrSetOptFlags(EO_SHOW_CODES);
	ErrSetOptFlags(EO_XLATE_CODES);
		
	/*init some important stuffs*/
	UseLocalAsnloadDataAndErrMsg();

	if (! AllObjLoad()){
		Message (MSG_ERROR, "AsnObjLoad() failed.");
		return(1);
	}

	if (! SubmitAsnLoad()){
		Message (MSG_ERROR, "SeqSubmitLoad() failed.");
		return(1);
	}

	if (!SeqCodeSetLoad ()){
		Message (MSG_ERROR, "SeqCodeSetLoad () failed.");
		return(1);
	}

	if (!GeneticCodeTableLoad()){
		Message (MSG_ERROR, "GeneticCodeTableLoad() failed.");
		return(1);
	}

	if (!FeatDefSetLoad()){
		Message (MSG_ERROR, "FeatDefSeqLoad() failed.");
		return(1);
	}

	/*init data blocks*/
	mWin_d=(DdvMainWinPtr)MemNew(sizeof(DdvMainWin));
	if (!mWin_d){
		Message (MSG_ERROR, "Viewer creation failed.");
		return(1);
	}

  /* look for "nogaps" runtime parameter */
    NoGaps = (Boolean)DDV_myargs[0].intvalue;
    SetAppProperty("dde_nogaps",(void*)&NoGaps);

    mWin_d->EditAllowed = (Boolean)DDV_myargs[1].intvalue;
	
	/*init data blocks*/
	ldp=(UDVLogoDataPtr)MemNew(sizeof(UDVLogoData));
	if (ldp){
		MemSet(ldp,0,sizeof(UDVLogoData));
	}
	else{
		Message (MSG_ERROR, "Viewer creation failed.");
		return(1);
	}

	/*OBjMgr Callback Function declaration*/
/*	REG_DDV_AUTO_EDIT; */
	REG_DDV_AUTO_VIEW;
	REGISTER_UDV_AUTONOMOUS;
    REG_DDV_SLA_EDIT;
	
    /* increment maximum # of objects held in memory */
    omp = ObjMgrWriteLock();
    if(omp != NULL ) omp->maxtemp = DDV_MAXTEMP;
    ObjMgrUnlock();

	w=DDV_StartMainWin_Master(mWin_d,ldp);
	if (w==NULL) goto fin;
	
	ProcessUpdatesFirst(FALSE);
	UseNetwork=mWin_d->UseNetwork;

	/*display main window*/
	RealizeWindow(w);
	Show(w);

	/*enter loop events*/
	ProcessEvents();
fin:
	/*close network connection, if needed*/
	if (UseNetwork && EntrezIsInited()) EntrezBioseqFetchDisable();
	/*clean up*/
	if (ldp) MemFree(ldp);
	RemoveAppProperty("UDVLogoData");	
	return(0);
}

