/*   udvmain.c
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
* File Name:  udvmain.c
*
* Author:  Patrick Durand
*
* Version Creation Date:   5/3/99
*
* $Revision: 6.15 $
*
* File Description: 
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: udvmain.c,v $
* Revision 6.15  2000/04/10 21:41:26  lewisg
* move alignment menus into ddv, udv from cn3d
*
* Revision 6.14  2000/03/06 14:01:27  durand
* first release of the Summary viewer done
*
* Revision 6.13  2000/03/01 16:53:17  durand
* add the Genome Viewer init functions
*
* Revision 6.12  2000/01/12 14:01:17  durand
* fix a cast to avoid compiling error on NT
*
* Revision 6.11  2000/01/11 15:04:02  durand
* add Entrez network stuff
*
* Revision 6.10  2000/01/10 15:19:07  durand
* use Entrez instead of ID1
*
* Revision 6.9  2000/01/03 21:05:58  durand
* update the way the udv main window is created
*
* Revision 6.8  1999/11/29 15:18:48  durand
* designed a new GUI; fixed problems under Win95 and Linux
*
* Revision 6.7  1999/11/09 21:09:17  durand
* reduce the size of the main window at startup
*
* Revision 6.6  1999/09/29 20:08:59  durand
* set UdvGlobals before using getArgs
*
* Revision 6.5  1999/07/30 20:10:15  durand
* updates for the new Entrez graphical viewer
*
* Revision 6.4  1999/06/07 15:40:30  durand
* add LOG line to keep track of the history
*
*
*
* ==========================================================================
*/

#include <udviewer.h>
#include <accentr.h>
#include <netcnfg.h>
#ifdef UDV_GENOME
#include <udvgv.h>
#endif

/*local text*/
static Char szAppName[]="OneD-Viewer";

/*******************************************************************************

  Function : UDV_ConfigAccepted()
  
  Purpose : Entrez Network COnfiguration Dialog Box; accept a new config
  
  Return value : none 

*******************************************************************************/
static void UDV_ConfigAccepted(void)
{
    SetAppParam("UDV", "SETTINGS", "NETWORKAVAILABLE", "TRUE");
    Message(MSG_OK, "Setting will take affect when you restart OneD-Viewer");
}

/*******************************************************************************

  Function : UDV_ConfigCancelled()
  
  Purpose : Entrez Network COnfiguration Dialog Box; close dlg without modif.
  
  Return value : none 

*******************************************************************************/
static void UDV_ConfigCancelled(void)
{
}

/*******************************************************************************

  Function : UDV_ConfigTurnedOff()
  
  Purpose : Entrez Network COnfiguration Dialog Box; cancel Entrez connection
  
  Return value : none 

*******************************************************************************/
static void UDV_ConfigTurnedOff(void)
{
    SetAppParam("UDV", "SETTINGS", "NETWORKAVAILABLE", "FALSE");
    Message(MSG_OK, "Setting will take affect when you restart OneD-Viewer");
}

/*******************************************************************************

  Function : UDV_ConfigNetwork()
  
  Purpose : call the Entrez Network COnfiguration Dialog Box
  
  Return value : none 

*******************************************************************************/
static void UDV_ConfigNetwork(IteM i)
{
ViewerMainPtr vmp;
WindoW        hParent;

	/*get main data block*/
	hParent=(WindoW)ParentWindow(i);
	if (!hParent) return;
	vmp = (ViewerMainPtr) GetObjectExtra (hParent);

    if (vmp->UseNetwork) ShowNetConfigForm(NULL,NULL, UDV_ConfigAccepted, 
		UDV_ConfigCancelled, UDV_ConfigTurnedOff, TRUE);
    else ShowNetConfigForm(NULL, NULL, UDV_ConfigAccepted, UDV_ConfigCancelled,
                      UDV_ConfigTurnedOff, FALSE);
}

/*******************************************************************************

  Function : UDV_StartEntrez()
  
  Purpose :  start connection to Entrez Server.
  				    
  Return value : -

*******************************************************************************/
static Boolean UDV_StartEntrez(Boolean UseNetwork)
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

Function: UDV_UseNetwork()

Purpose:  Determines if UDV should use the network
  
Returns:  TRUE if yes

*****************************************************************************/
static Boolean UDV_UseNetwork(void)
{
Char str[64];

    if (GetAppParam
        ("UDV", "SETTINGS", "NETWORKAVAILABLE", NULL, str, sizeof(str))) {
        if (StringICmp(str, "TRUE") == 0) return TRUE;
    }
    return FALSE;
}

/*******************************************************************************

  Function : Main()
  
  Purpose : Entry point of the software 
  
  Parameters : 
  
  Return value : 

*******************************************************************************/
Int2 Main(void)
{
ViewerMainPtr 	vmp;
WindoW			w; 
Int2			Margins;
Boolean 		UseNetwork;
RecT			rcL;
UdvGlobals      ug;
UDVLogoData		ldp;
	
	ErrSetMessageLevel(SEV_WARNING);
	ErrSetOptFlags(EO_SHOW_CODES);
	ErrSetOptFlags(EO_XLATE_CODES);
	MemSet(&ug,0,sizeof(UdvGlobals));
		
	/*init some important stuffs*/
	UseLocalAsnloadDataAndErrMsg();

/*	isID1Ok=UDV_ID1Init();
	if (!isID1Ok){
		Message (MSG_ERROR, "Unable to connect Network.");
	}
*/
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
	vmp=(ViewerMainPtr)MemNew(sizeof(ViewerMain));
	if (vmp){
		MemSet(vmp,0,sizeof(ViewerMain));
	}
	else{
		Message (MSG_ERROR, "Viewer creation failed.");
		return(1);
	}

	/*OBjMgr Callback Function declaration*/
	vmp->UDVprocid=REGISTER_UDV_AUTONOMOUS;

#ifdef UDV_GENOME
	vmp->GVprocid=REGISTER_GV_MODULE;
#endif
	
	/*main window*/
	Margins=20*stdCharWidth;
	w=DocumentWindow(Margins,Margins , -10 , -10 ,
			szAppName, 
			UDV_WinMainProgQuit,
			UDV_WinMainResize);

	if (w==NULL){
		Message (MSG_ERROR, "Viewer creation failed.");
		return(1);
	}
	vmp->hWndMain=w;
	SetObjectExtra (w, (Pointer) vmp, (FreeProc)UDV_WinMainCleanupExtraProc);

	/*this is an autonomous viewer*/
	vmp->AutonomeViewer=TRUE;

	/*Should use a connection to Entrez ?*/
	vmp->UseNetwork=UDV_UseNetwork();
	UseNetwork=vmp->UseNetwork;
	/*init menu*/
	UDV_SetupMenus(w,UseNetwork, NULL);
	UDV_set_MainMenus(&vmp->MainMenu,FALSE);
	/*create the windows*/
	CreateMainControls(w,vmp,NULL);
	UDV_set_MainControls(vmp,FALSE);

	/*init logo data*/
	LogoFontCreate(&ldp.f1,&ldp.f2,&ldp.f3);
	StringCpy(ldp.szTitle,szAppName);
	StringCpy(ldp.szDesc,", a sequence viewer for GenBank");
	SetAppProperty("UDVLogoData",(Pointer)&ldp);	
	vmp->Show_logo=TRUE;
	SetAppProperty("AutonomousUDVViewer",(Pointer)vmp);	
	
	ProcessUpdatesFirst(FALSE);
	
	RealizeWindow(w);
	UDV_WinMainResize(w);
	Show(w);

	ug.fetchSepProc=(UdvFetchSeqEntryProc)EntrezSeqEntryGet;
	ug.NetCfgMenuProc=UDV_ConfigNetwork;
	ug.NetStartProc=UDV_StartEntrez;	
	ug.vmp=vmp;
	SetAppProperty("UdvGlobals",(Pointer)&ug);
	
	/*is there a file to open on the command line ?*/
	if (GetArgc()>1){
		/*is GetArgv()[1] a file name ?*/
		FILE *f;
		
		f=FileOpen(GetArgv()[1],"r");
		if (f){
			UDV_analyze_SEP_for_open(f,NULL,vmp,w);
			FileClose(f);
		}
	}

	ProcessEvents();

	/*ID1BioseqFetchDisable();*/
	/*close network connection, if needed*/
	if (UseNetwork && EntrezIsInited()) EntrezBioseqFetchDisable();

	RemoveAppProperty("AutonomousUDVViewer");	
	RemoveAppProperty("UDVLogoData");	
	RemoveAppProperty("UdvGlobals");

	return(0);
}

