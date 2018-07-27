/*   udvopen.c
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
* File Name:  udvopen.c
*
* Author:  Patrick Durand
*
* Version Creation Date:   5/3/99
*
* $Revision: 6.20 $
*
* File Description: open/close/choose sequence/file from disk/network
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: udvopen.c,v $
* Revision 6.20  2000/07/26 17:26:26  lewisg
* fix code for c++ inclusion
*
* Revision 6.19  2000/06/16 14:57:02  lewisg
* move entrez calls out of desktop
*
* Revision 6.18  2000/05/01 21:21:13  lewisg
* make features dialogs modal
*
* Revision 6.17  2000/04/27 22:21:58  lewisg
* misc bugs/features
*
* Revision 6.16  2000/04/13 13:58:03  durand
* allowed udv to display reverse complement sequence
*
* Revision 6.15  2000/04/10 20:33:40  lewisg
* fix show/hide for blast multiple, make blast multiple API generic
*
* Revision 6.14  2000/03/15 19:32:20  lewisg
* launch only single udv window
*
* Revision 6.13  2000/03/06 19:54:40  durand
* updated the function AccessionToGi_Entrez
*
* Revision 6.12  2000/03/02 15:47:51  durand
* use MovableModalWindow for dialog boxes
*
* Revision 6.11  2000/02/16 14:49:39  durand
* replace GatherProcLaunch by GatherSpecificProcLaunch
*
* Revision 6.10  2000/01/11 15:03:19  durand
* remove network stuff
*
* Revision 6.9  2000/01/10 15:18:40  durand
* use Entrez instead of ID1
*
* Revision 6.8  1999/11/29 15:17:55  durand
* designed a new GUI; fixed problems under Win95 and Linux
*
* Revision 6.7  1999/11/19 15:01:48  durand
* speed up mouse selection ; avoid sequence flashing during selection ; update menu functionalities
*
* Revision 6.6  1999/11/09 21:06:59  durand
* add sequence selection manager
*
* Revision 6.5  1999/10/02 15:11:16  durand
* update the code to be used by wwwudv
*
* Revision 6.4  1999/09/07 15:32:39  durand
* add BBS and RefSeq to databases list
*
* Revision 6.3  1999/06/07 15:39:44  durand
* add LOG line to keep track of the history
*
*
*
* ==========================================================================
*/

#include <udviewer.h>
#if 0
#include <blast.h>
#include <netblap3.h>
#endif


	typedef struct dlgfileopendata {/*use to manage the FileOpen dialog box*/
		WindoW 		parent;			/*main window of the application*/
		TexT		FNameEditCtrl;	/*handle of the file text control*/
		ButtoN		Ok;				/*handle of the Ok button*/
		GrouP		ReadMode;		/*handle of the file type control*/
	} DlgFileOpenData, PNTR DlgFileOpenDataPtr;


/*local struct used only by the Download sequence dialog box*/
	typedef struct udvnetopen {
		WindoW 			hWndMain;	/*main window*/
		TexT			Entry;		/*Entrez entry*/
		PopuP			AccessType;	/*database type*/
		ButtoN  		ok;			/*ok button*/
		UDVMainMenuPtr 	mmp;		/*main menu*/
	} UDVNetOpen, PNTR UDVNetOpenPtr;

/*local struct used only by the Choose sequence dialog box*/
	typedef struct udvchooseseq {
		WindoW 	hWndMain;		/*main window*/
		LisT	bsp_list;		/*listbox of bsps*/
	} UDVChooseSeq, PNTR UDVChooseSeqPtr;

	static Char szAppName[]="OneD-Viewer";

	static Uint1 UDV_DataBaseID[]={
				SEQID_GENBANK,
				SEQID_EMBL,
				SEQID_PIR,
				SEQID_SWISSPROT,
				SEQID_GI,
				SEQID_DDBJ,
				SEQID_PRF,
				SEQID_PDB,
				SEQID_OTHER,/*RefSeq*/
				SEQID_GIBBSQ};


/*******************************************************************************

  Function : UDV_Init_vdp_struct()
  
  Purpose : free memory of a bsp
  
  Parameters : vdp; contains data to delete
  				EraseParaG; TRUE means delete vdp->ParaG
				EraseMainTitle; TRUE means restore default viewer window name
				EraseInfoPanel; TRUE means delete content of InfoPanel
  
  Return value : none 

*******************************************************************************/
NLM_EXTERN void  UDV_Init_vdp_struct(PaneL p,ViewerDialogDataPtr vdp, 
	Boolean EraseParaG,Boolean EraseMainTitle,Boolean EraseInfoPanel)
{
BaR 	vsb;
WindoW 	w;

	if (!vdp) return;
	
	if (EraseParaG) UDV_FreeListParaG(&vdp->ParaG);

	MemSet(&vdp->bsp_i,0,sizeof(BspInfo));

	/*invalid Item selection identifier*/
	vdp->Item_select.eIDsel=(Uint2)-1;
	vdp->Item_select.iIDsel=(Uint2)-1;
	vdp->Item_select.iTypeSel=(Uint2)-1;
	vdp->Old_Item_select=vdp->Item_select;

	/*reinit scrollbar*/
	vsb = GetSlateVScrollBar ((SlatE)p);
	CorrectBarMax(vsb,0);
	CorrectBarValue(vsb,0);
	CorrectBarPage(vsb,0,0);
	
	/*reset winmain title*/
	if (EraseMainTitle){
		if (vdp->Parent) 
			SetTitle(vdp->Parent,szAppName);
	}
	
	/*reset infopanel title*/
	if (EraseInfoPanel && vdp->InfoPanel) 
		SetTitle(vdp->InfoPanel,"Ready !");
}

/*******************************************************************************

  Function : SearchBioseq()
  
  Purpose : store bsps of a SeqEntry 
  
  Parameters : see Toolbox doc !
  
  Return value : none 

*******************************************************************************/
NLM_EXTERN Boolean LIBCALLBACK SearchBioseq (BioseqPtr bsp, 
			SeqMgrBioseqContextPtr context)
{
ValNodePtr PNTR	BspTable;
	
	BspTable=(ValNodePtr PNTR)context->userdata;

	if (bsp) {
		ValNodeAddInt(BspTable,1,
				UDV_EncodeIdxFeat(context->entityID,context->itemID));
	}

	return(TRUE);
}


/*******************************************************************************

  Function : UDV_analyze_SEP_for_open()
  
  Purpose : analyse a SEP coming form a local SeqEntry file or from Entrez,
  			and find the BSPs. 
  
  Parameters : the_set; SeqEntry to analyze
  				vmp; general data software
				w; window which has called this function
				mon; monitor
  
  Return value : If error, return FALSE.  

*******************************************************************************/
NLM_EXTERN Boolean  UDV_analyze_SEP_for_open(FILE *fp,SeqEntryPtr the_set,
	ViewerMainPtr vmp,WindoW w)
{
BioseqPtr 	bsp;
Boolean 	bRet=TRUE,bReadOk=FALSE;
Pointer     dataptr;
Uint2       datatype,entityID,eID,iID;
MonitorPtr  mon;
RecT        rcP;
WindoW      temport;
	
	if (vmp->hFeatDlg){/*Features List Dlg Box*/
		SetStatus(vmp->MainMenu.ShowFeatureList,FALSE);
		Remove(vmp->hFeatDlg);
		vmp->hFeatDlg=NULL;
	}
	/*free data, remove viewer*/
	if (vmp->vdp){
		UDV_FreeVDPstruct(vmp->vdp,FALSE);

		SetValue(vmp->MainMenu.ScalePosChoice,3);
		SetStatus(vmp->MainMenu.ShowFeature,TRUE);
		UDV_set_MainMenus(&vmp->MainMenu,FALSE);
		UDV_set_MainControls(vmp,FALSE);
	}
	if (vmp->BspTable){
		ValNodeFree(vmp->BspTable);
		vmp->BspTable=NULL;
		vmp->BspChoice=0;
	}
	if (vmp->dataptr && vmp->datatype){
		ObjMgrFree(vmp->datatype,vmp->dataptr);
		vmp->datatype=0;
		vmp->dataptr=NULL;
	}

	mon = MonitorStrNewEx (szAppName, 30, FALSE);
	WatchCursor();
	
	if (fp!=NULL){/*open a file*/
		/*Read the file*/
		MonitorStrValue (mon, "Reading ASN.1 file");
		Update ();
		dataptr = ReadAsnFastaOrFlatFile (fp, &datatype, NULL, 
			FALSE, FALSE, FALSE, FALSE);
		/*register for ObjMgr*/
		entityID = ObjMgrRegister (datatype, dataptr);
		if(datatype == OBJ_SEQSUB || datatype == OBJ_SEQENTRY ||
        	datatype == OBJ_BIOSEQ || datatype == OBJ_BIOSEQSET){
			bReadOk=TRUE;
		}
	}
	else if (the_set!=NULL){/*retrieve a Network Entry*/
		/*a complete verification of sep is done elsewhere; here
		sep can be only Bioseq or BioseqSet*/
		if (IS_Bioseq (the_set)) datatype = OBJ_BIOSEQ;
		else datatype = OBJ_BIOSEQSET;

		dataptr=(Pointer)the_set->data.ptrvalue;
		/*register for ObjMgr*/
		entityID = ObjMgrRegister (datatype, dataptr);
		bReadOk=TRUE;
		
	}
	
	if (bReadOk){
		Boolean bReverse;
		/*compute the global Feature Index*/
		MonitorStrValue (mon, "Building Feature index...");
		Update ();
		if (vmp->vdp)
			bReverse=vmp->vdp->bDisplayRevComp;
		else
			bReverse=FALSE;
		UDV_CreateOneFeatureIndex(entityID,NULL,bReverse);						
		/*scan the BSPs*/
		SeqMgrExploreBioseqs (entityID, NULL, (Pointer) &vmp->BspTable, 
			SearchBioseq, TRUE, TRUE, TRUE);
	}

	if (vmp->BspTable==NULL){
		MonitorFree (mon);
		ArrowCursor();
		Message (MSG_ERROR, "No sequence found in your entry.");
		bRet=FALSE;
		goto error;
	}

	vmp->entityID=entityID;
	vmp->datatype=datatype;
	vmp->dataptr=dataptr;
	
	/*choose the first sequence by default*/
	UDV_DecodeIdxFeat ((Uint4)vmp->BspTable->data.intvalue,&eID,&iID);
	bsp=GetBioseqGivenIDs (eID, iID, OBJ_BIOSEQ);
	if (bsp){
		vmp->BspChoice=1;
		ArrowCursor();
		MonitorFree (mon);
		vmp->Show_logo=FALSE;	
		if (vmp->UDVprocid!=0){
			GatherSpecificProcLaunch(vmp->UDVprocid, "OneD-Viewer", 
				OMPROC_VIEW, FALSE, eID, iID, OBJ_BIOSEQ);
		}
		else{
			GatherProcLaunch(OMPROC_VIEW, FALSE, eID, iID, OBJ_BIOSEQ, 
				OBJ_BIOSEQ, 0, OBJ_BIOSEQ, 0);
		}

		temport=SavePort(vmp->hWndMain);
		Select(vmp->vdp->UnDViewer);
		ObjectRect(vmp->vdp->UnDViewer,&rcP);
		InvalRect(&rcP);
		Update();
		RestorePort(temport);
	}
	else {
		MonitorFree (mon);
		ArrowCursor();
		Message (MSG_ERROR, "No sequence found in your file.");
		bRet=FALSE;
	}

error:
	if (!bRet){/*show the logo in case of error*/
		vmp->Show_logo=TRUE;	
		UDV_set_MainMenus(&vmp->MainMenu,FALSE);
		UDV_set_MainControls(vmp,FALSE);
	}
	return (bRet);
}

/*******************************************************************************

  Function : AccessionToGi_Entrez()
  
  Purpose : given s string, retrieve the UID 
  
  Parameters : string, either a name or an accession number	
  				type; entry choosen by the nice user in the dataBase type
					popup list
  
  Return value : none 

*******************************************************************************/
static Int4 AccessionToGi_Entrez (CharPtr string,Int2 type)
{
SeqIdPtr 	sip;
Int4 		uid=0;
Uint1 		Choice;

	sip=(SeqIdPtr)ValNodeNew(NULL);
	if (!sip) return(0);

	Choice=UDV_DataBaseID[type-1];

	switch(Choice){
		case SEQID_GENBANK:
		case SEQID_EMBL:
		case SEQID_PIR:
		case SEQID_DDBJ:
		case SEQID_PRF:
		case SEQID_SWISSPROT:
		case SEQID_OTHER:{
			TextSeqIdPtr tsip;
			
			tsip=TextSeqIdNew();
			if (!tsip) break;
			sip->data.ptrvalue=(VoidPtr)tsip;
			sip->choice = Choice;
			/*try to retrieve an uid; to avoid the user to know whether string
			is an accession or a name, this function tests both*/
			tsip->name=StringSave(string);
			uid=GetGIForSeqId(sip);
			if (uid==0){
				*tsip->name='\0';
				tsip->accession=StringSave(string);
			}
			TextSeqIdFree(tsip);
			break;
		}
		case SEQID_GIBBSQ:
			sip->data.intvalue=(Int4)atoi(string);
			sip->choice = Choice;
			uid=GetGIForSeqId(sip);
			break;
		case SEQID_GI:
			if (!StrToLong(string,&uid)) uid=0;
			break;
		case SEQID_PDB:{
			PDBSeqIdPtr psip;
			
			psip=PDBSeqIdNew();
			if (!psip) break;
			sip->data.ptrvalue=(VoidPtr)psip;
			/*type is one of the SEQID_... reported in objloc.h*/
			sip->choice = Choice;
			/*try to retrieve an uid; to avoid the user to know whether string
			is an accession or a name, this function tests both*/
			psip->mol=StringSave(string);
			psip->chain=0;        /* 0 = no chain set.  default = 32 */
        	psip->rel=NULL;
			uid=GetGIForSeqId(sip);
			PDBSeqIdFree(psip);
			break;
		}
	}

	ValNodeFree(sip);
	return(uid);
}


/*******************************************************************************

  Function : UDV_NetOpen_okProc()
  
  Purpose : manage ok button of the Network open dialog box 
  
  Parameters : g; button
  
  Return value : none 

*******************************************************************************/
static void UDV_NetOpen_okProc(ButtoN g)
{
WindoW			hOpenDlg;
UDVNetOpenPtr 	unop;
Char 			szAccess[50]={""};
Int2			AccessType;
Int4			uid=0;
ViewerMainPtr 	vmp;
MonitorPtr  	mon;
SeqEntryPtr		sep=NULL;
Boolean			bReadok=FALSE;
UdvGlobalsPtr   ugp=NULL;

	hOpenDlg=(WindoW)ParentWindow(g);

	if (!hOpenDlg) return;
	
	unop = (UDVNetOpenPtr) GetObjectExtra (hOpenDlg);	

	if (unop==NULL) return;

	vmp = (ViewerMainPtr) GetObjectExtra (unop->hWndMain);
	if (vmp==NULL) return;

	Hide(hOpenDlg);
	
	/*retrieve and analyze Accession or Gi*/
	GetTitle(unop->Entry, szAccess, sizeof(szAccess));

	if (StringHasNoText (szAccess)) {
		Message (MSG_OK, "Please enter an accession number or an entry name");
		Show (hOpenDlg);
		Select (hOpenDlg);
		Select (unop->Entry);
		return;
	}

	AccessType=GetValue(unop->AccessType);
	WatchCursor();
	uid=AccessionToGi_Entrez(szAccess,AccessType);	
	/*Connect ID1 with a valid UID*/
	if (uid==0){
		ArrowCursor();
		Message (MSG_OK, "Unable to find your record in the database.");
		Show (hOpenDlg);
		Select (hOpenDlg);
		Select (unop->Entry);
		return;
	}
	else{
		mon = MonitorStrNewEx (szAppName, 30, FALSE);
		MonitorStrValue (mon, "Retrieving your record...");
		Update ();
	
		ugp=(UdvGlobalsPtr)GetAppProperty("UdvGlobals");
		if (ugp && ugp->fetchSepProc) {
				sep=ugp->fetchSepProc(uid, 0);
		}
				
		ArrowCursor();
		MonitorFree (mon);
		if (sep){
			if (IS_Bioseq (sep) || IS_Bioseq_set (sep))	bReadok=TRUE;
			else bReadok=FALSE;
		}
		else bReadok=FALSE;
		
		if (bReadok){
			UDV_analyze_SEP_for_open(NULL,sep,vmp,unop->hWndMain);
		}
		else{
			Message (MSG_OK, "Unable to retrieve your record in the database.");
		}
	}

	/*enable main menus and delete Download dlg box*/
	UDV_set_PullMenus(unop->mmp,TRUE);
	Remove(hOpenDlg);
}

/*******************************************************************************

  Function : UDV_NetOpen_cancelProc()
  
  Purpose : manage ok button of the Network open dialog box 
  
  Parameters : g; button
  
  Return value : none 

*******************************************************************************/
static void UDV_NetOpen_cancelProc(ButtoN g)
{
WindoW			hOpenDlg;
UDVNetOpenPtr 	unop;

	hOpenDlg=(WindoW)ParentWindow(g);

	if (!hOpenDlg) return;
	
	unop = (UDVNetOpenPtr) GetObjectExtra (hOpenDlg);

	if (unop==NULL) return;

	/*enable main menus and delete Download dlg box*/
	UDV_set_PullMenus(unop->mmp,TRUE);
	Remove(hOpenDlg);
}

/*******************************************************************************

  Function : UDV_NetOpen_access()
  
  Purpose : manage Accession edit control of the Download dialog box 
  
  Parameters : t; edit control
  
  Return value : none 

*******************************************************************************/
static void UDV_NetOpen_access(TexT t)
{
Char 			szAccess[50]={""};
WindoW			hOpenDlg;
UDVNetOpenPtr 	unop;

	hOpenDlg=(WindoW)ParentWindow(t);

	if (!hOpenDlg) return;
	
	unop = (UDVNetOpenPtr) GetObjectExtra (hOpenDlg);

	if (unop==NULL) return;
	
	GetTitle(t, szAccess, sizeof(szAccess));

	if (StringLen(szAccess) == 0)
		Disable(unop->ok);
	else Enable(unop->ok);

	return;
}

/*******************************************************************************

  Function : UDV_NetOpen()
  
  Purpose : retrieve a file from Download dialog box 
  
  Parameters : g; button
  
  Return value : none 

*******************************************************************************/
NLM_EXTERN void UDV_NetOpen (IteM i)
{
GrouP			c,g;
WindoW			w,hWinMain;
UDVNetOpenPtr 	unop;
ViewerMainPtr 	vmp;
UdvGlobalsPtr ugp;

	hWinMain=(WindoW)ParentWindow(i);

	if (!hWinMain) return;
	
	vmp = (ViewerMainPtr) GetObjectExtra (hWinMain);
	if (vmp==NULL) return;

	/*init the Network, if needed*/
	ugp=(UdvGlobalsPtr)GetAppProperty("UdvGlobals");
	if (ugp && ugp->NetStartProc){
		if (!ugp->NetStartProc(vmp->UseNetwork)) return;
	}

	w = MovableModalWindow (-50, -33, -10, -10, "Download From NCBI", NULL);

	if (w==NULL) return;

	unop=(UDVNetOpenPtr)MemNew(sizeof(UDVNetOpen));
	if (unop==NULL) {
		Remove(w);
		return;
	}
	unop->hWndMain=hWinMain;
	
	SetGroupSpacing (w, 10, 10);

	/*accesion*/
	g = NormalGroup (w, 3, 0, "Entry", systemFont,NULL);

#ifdef WIN_MAC
	unop->AccessType = PopupList (g, TRUE, NULL);
#endif

#ifndef WIN_MAC
	unop->AccessType = PopupList (g, FALSE, NULL);
#endif

	PopupItem(unop->AccessType,"GENBANK");
	PopupItem(unop->AccessType,"EMBL");
	PopupItem(unop->AccessType,"PIR");
	PopupItem(unop->AccessType,"SWISSPROT");
	PopupItem(unop->AccessType,"GI number");
	PopupItem(unop->AccessType,"DDBJ");
	PopupItem(unop->AccessType,"PRF");
	PopupItem(unop->AccessType,"PDB");
	PopupItem(unop->AccessType,"RefSeq");
	PopupItem(unop->AccessType,"BBS");
	
	SetValue (unop->AccessType, 6);

	unop->Entry = DialogText (g, "", 10, UDV_NetOpen_access);

	/*retrieve - cancel*/
	c = HiddenGroup (w, 4, 0, NULL);
	SetGroupSpacing (c, 10, 2);
	unop->ok=DefaultButton (c, "Retrieve", UDV_NetOpen_okProc);
	Disable (unop->ok);
	PushButton (c, "Cancel", UDV_NetOpen_cancelProc);
	
	SetObjectExtra (w, (Pointer) unop, StdCleanupExtraProc);

	/*display*/
	AlignObjects (ALIGN_CENTER, (HANDLE) g, (HANDLE) c, NULL);
	RealizeWindow (w);
	Show (w);
	Select (w);
	Update ();
	
	/*disable all menus*/
	unop->mmp=&vmp->MainMenu;
	UDV_set_PullMenus(unop->mmp,FALSE);
}

/*******************************************************************************

  Function : UDV_FOpenTextProc()
  
  Purpose : manage File name edit control of the FileOpen dialog box 
  
  Parameters : t; edit control
  
  Return value : none 

*******************************************************************************/
static void UDV_FOpenTextProc(TexT t)
{
Char 				szFName[PATH_MAX]={""};
WindoW				hOpenDlg;
DlgFileOpenDataPtr  dfodp;

	hOpenDlg=(WindoW)ParentWindow(t);

	if (!hOpenDlg) return;
	
	dfodp = (DlgFileOpenDataPtr) GetObjectExtra (hOpenDlg);

	if (dfodp==NULL) return;
	
	GetTitle(t, szFName, sizeof(szFName)-1);

	if (StringLen(szFName) == 0)
		Disable(dfodp->Ok);
	else Enable(dfodp->Ok);

	return;
}

/*******************************************************************************

  Function : UDV_FOpenBrowseProc()
  
  Purpose : manage browse button of the FileOpen dialog box 
  
  Parameters : g; button
  
  Return value : none 

*******************************************************************************/
static void UDV_FOpenBrowseProc(ButtoN g)
{
DlgFileOpenDataPtr  dfodp;
WindoW				hOpenDlg;
Char 				path[PATH_MAX]={""};

	hOpenDlg=(WindoW)ParentWindow(g);

	if (!hOpenDlg) return;

	dfodp = (DlgFileOpenDataPtr) GetObjectExtra (hOpenDlg);

	if (dfodp==NULL) return;
	if (!dfodp->FNameEditCtrl) return;

	if (GetInputFileName (path, sizeof(path), NULL, NULL)){ 
		SetTitle(dfodp->FNameEditCtrl, path);
		UDV_FOpenTextProc(dfodp->FNameEditCtrl);
	}

	return;   
}

/*******************************************************************************

  Function : UDV_FOpenCancelProc()
  
  Purpose : manage cancel button of the FileOpen dialog box 
  
  Parameters : g; button
  
  Return value : none 

*******************************************************************************/
static void UDV_FOpenCancelProc(ButtoN g)
{
WindoW				hOpenDlg;
DlgFileOpenDataPtr  dfodp;
ViewerMainPtr 		vmp;

	hOpenDlg=(WindoW)ParentWindow(g);

	if (!hOpenDlg) return;
	dfodp = (DlgFileOpenDataPtr) GetObjectExtra (hOpenDlg);
	
	if (dfodp){
		vmp = (ViewerMainPtr) GetObjectExtra (dfodp->parent);
		Enable(vmp->MainMenu.FileOpen);
	}
	
	Remove(hOpenDlg);
    UDV_set_PullMenus(&vmp->MainMenu,TRUE);
}


/*******************************************************************************

  Function : UDV_FOpenAcceptProc()
  
  Purpose : manage ok button of the FileOpen dialog box 
  
  Parameters : g; button
  
  Return value : none 

*******************************************************************************/
static void UDV_FOpenAcceptProc(ButtoN g)
{
WindoW				hOpenDlg,w;
DlgFileOpenDataPtr  dfodp;
ViewerMainPtr 		vmp;
Char 				szFName[PATH_MAX]={""};
Boolean				isBinary;
FILE				*fp;
UdvGlobalsPtr       ugp;

	hOpenDlg=(WindoW)ParentWindow(g);

	if (!hOpenDlg) return;
	dfodp = (DlgFileOpenDataPtr) GetObjectExtra (hOpenDlg);
	
	if (dfodp){
		w=dfodp->parent;
		vmp = (ViewerMainPtr) GetObjectExtra (w);
		if (vmp!=NULL){
			/*try to connect Entrez; if failure, still ok : the user can
			use DDV with a local file. Otherwise he/she will be in trouble ;-)*/
			ugp=(UdvGlobalsPtr)GetAppProperty("UdvGlobals");
			if (ugp && ugp->NetStartProc){
				if (!ugp->NetStartProc(vmp->UseNetwork)) return;
			}

			Enable(vmp->MainMenu.FileOpen);
			/*file name*/
			GetTitle(dfodp->FNameEditCtrl, szFName, sizeof(szFName));

			/*file reading mode*/
			if (GetValue(dfodp->ReadMode)==2) isBinary=TRUE;
			else isBinary=FALSE;
	
			Remove(hOpenDlg);

			/* open the i/o files in the correct mode */
			if ((fp = FileOpen (szFName, isBinary?"rb":"r")) == NULL){
				Message (MSG_ERROR, "Unable to open your file.");
				goto error;
			}
			
			UDV_analyze_SEP_for_open(fp,NULL,vmp,w);
			FileClose(fp);
		}
	}
	
error:	
    UDV_set_PullMenus(&vmp->MainMenu,TRUE);
}

/*******************************************************************************

  Function : UDV_FileOpen()
  
  Purpose : callback of the File|Open command 
  
  Parameters : i; command item
  
  Return value : none 

*******************************************************************************/
NLM_EXTERN void UDV_FileOpen(IteM i)
{
DlgFileOpenDataPtr  dfodp;
ViewerMainPtr 		vmp;
WindoW				hWinMain,hOpenDlg;
GrouP				g,g4;
ButtoN				b,b1,b2;
TexT				t1;    

	hWinMain=(WindoW)ParentWindow(i);

	if (!hWinMain) return;
	
	vmp = (ViewerMainPtr) GetObjectExtra (hWinMain);

	if (vmp==NULL) return;
		
	dfodp=(DlgFileOpenDataPtr)MemNew(sizeof(DlgFileOpenData));
	if (!dfodp) return;
	MemSet(dfodp,0,sizeof(DlgFileOpenData));

    hOpenDlg = MovableModalWindow(-30, -20,  -10,  -10, 
				"OneD-Viewer - Open a local file",  NULL);
    g = NormalGroup(hOpenDlg, 2, 1, "File name:",  systemFont, NULL);
    SetGroupMargins(g, 10, 10);
    SetGroupSpacing(g, 10, 20);  
    t1 = DialogText(g,"",25, UDV_FOpenTextProc);
    b = PushButton(g, " browse...", UDV_FOpenBrowseProc);
   
    g = HiddenGroup(hOpenDlg, 3, 1, NULL);
    SetGroupMargins(g, 10, 10);
    SetGroupSpacing(g, 30, 30);
 
    g4 = NormalGroup(g, 2, 1, "File type", systemFont,  NULL);
    SetGroupMargins(g4, 10, 10);
    RadioButton(g4, "Ascii");
    RadioButton(g4, "Binary");
    SetValue(g4, 1);

    b1 = DefaultButton(g, "OK",  UDV_FOpenAcceptProc);
    b2 = PushButton(g, "Cancel",  UDV_FOpenCancelProc);
   
    Disable(b1);

	dfodp->parent=hWinMain;	
	dfodp->FNameEditCtrl=t1;
	dfodp->Ok=b1;
	dfodp->ReadMode=g4;

	SetObjectExtra (hOpenDlg, (Pointer) dfodp, StdCleanupExtraProc);
	
    Select(hOpenDlg);
    Show(hOpenDlg);

	/*allow only one FOpen dlg at a time*/
    UDV_set_PullMenus(&vmp->MainMenu,FALSE);
}

/*******************************************************************************

  Function : UDV_FileClose()
  
  Purpose : callback of the File|Close command 
  
  Parameters : i; command item
  
  Return value : none 

*******************************************************************************/
NLM_EXTERN void UDV_FileClose(IteM i)
{
ViewerMainPtr 		vmp;
WindoW				hWinMain;
RecT				rcL,rcP;
PaneL				p1,p2;

	hWinMain=(WindoW)ParentWindow(i);

	if (!hWinMain) return;
	
	vmp = (ViewerMainPtr) GetObjectExtra (hWinMain);

	if (vmp==NULL) return;
	
	if (vmp->vdp==NULL) return;
		
	if (vmp->hFeatDlg){/*Features List Dlg Box*/
		SetStatus(vmp->MainMenu.ShowFeatureList,FALSE);
		Remove(vmp->hFeatDlg);
		vmp->hFeatDlg=NULL;
	}

	UDV_FreeVDPstruct(vmp->vdp,FALSE);

	if (vmp->BspTable){
		ValNodeFree(vmp->BspTable);
		vmp->BspTable=NULL;
		vmp->BspChoice=0;
	}
	if (vmp->dataptr && vmp->datatype){
		ObjMgrFree(vmp->datatype,vmp->dataptr);
		vmp->datatype=0;
		vmp->dataptr=NULL;
		vmp->entityID=0;
	}

	/*Update main window title and menus*/
	SetValue(vmp->MainMenu.ScalePosChoice,3);
	SetStatus(vmp->MainMenu.ShowFeature,TRUE);
	UDV_set_MainMenus(&vmp->MainMenu,FALSE);
	UDV_set_MainControls(vmp,FALSE);
	SetTitle(hWinMain,szAppName);	
	SetTitle(vmp->vdp->InfoPanel,"Ready !");
	
	vmp->Show_logo=TRUE;	
	Select(vmp->vdp->UnDViewer);
	Select(vmp->vdp->UnDViewer);
	ObjectRect(vmp->vdp->UnDViewer,&rcP);
	InvalRect(&rcP);
	Update();
}

/*******************************************************************************

  Function : UDV_CSeq_okProc()
  
  Purpose : manage ok button of the Choose sequence dialog box 
  
  Parameters : g; button
  
  Return value : none 

*******************************************************************************/
static void UDV_CSeq_okProc(ButtoN g)
{
ViewerDialogDataPtr vdp;
UDVChooseSeqPtr 	ucsp;
ViewerMainPtr 		vmp;
Int2 				value;
WindoW				hOpenDlg,temport;
ValNodePtr			vnp;
Uint2				nCompt=1,eID,iID;
BioseqPtr			bsp=NULL;
RecT                rcP;
Char 				szBuf[255]={""};

	hOpenDlg=(WindoW)ParentWindow(g);
	if (!hOpenDlg) return;
	
	ucsp=(UDVChooseSeqPtr)GetObjectExtra (hOpenDlg);
	if (ucsp==NULL) goto error;
	
	vmp = (ViewerMainPtr) GetObjectExtra (ucsp->hWndMain);
	if (vmp==NULL) goto error;
	
	UDV_set_PullMenus(&vmp->MainMenu,TRUE);
	
	vdp = vmp->vdp;
	if (vdp == NULL) return;
	
	value=GetValue(ucsp->bsp_list);/*one-base value*/
	if (vmp->BspChoice==value) goto error;

	/*look for a new bsp*/
	for (vnp=vmp->BspTable ; vnp!=NULL ;vnp=vnp->next){
		if (nCompt==value){
			UDV_DecodeIdxFeat ((Uint4)vnp->data.intvalue,&eID,&iID);
			bsp=GetBioseqGivenIDs (eID, iID, OBJ_BIOSEQ);
			vmp->BspChoice=value;
			break;
		}
		nCompt++;
	}
	
	if (bsp){
		if (vmp->hFeatDlg){/*Features List Dlg Box*/
			SetStatus(vmp->MainMenu.ShowFeatureList,FALSE);
			Remove(vmp->hFeatDlg);
			vmp->hFeatDlg=NULL;
		}
		/*free data, remove viewer*/
		UDV_FreeVDPstruct(vmp->vdp,FALSE);

		SetValue(vmp->MainMenu.ScalePosChoice,3);
		SetStatus(vmp->MainMenu.ShowFeature,TRUE);
		UDV_set_MainMenus(&vmp->MainMenu,FALSE);
		UDV_set_MainControls(vmp,FALSE);
		
		if (vmp->UDVprocid!=0){
			GatherSpecificProcLaunch(vmp->UDVprocid, "OneD-Viewer", 
				OMPROC_VIEW, FALSE, eID, iID, OBJ_BIOSEQ);
		}
        else if (vmp->GVprocid==0){
            SetAppProperty("nonautonomous",(void *)vmp);
			GatherSpecificProcLaunch(vmp->UDVprocid, "OneD-Viewer", 
				OMPROC_VIEW, FALSE, eID, iID, OBJ_BIOSEQ);
            RemoveAppProperty("nonautonomous");
		}
		else{
			GatherProcLaunch(OMPROC_VIEW, FALSE, eID, iID, OBJ_BIOSEQ, 
				OBJ_BIOSEQ, 0, OBJ_BIOSEQ, 0);
		}
	
		temport=SavePort(ucsp->hWndMain);
		Select(vdp->UnDViewer);
		ObjectRect(vdp->UnDViewer,&rcP);
		sprintf(szBuf,"%s - [%s - %d letters]",szAppName,
					vdp->bsp_i.bspAccNum,
					vdp->bsp_i.bspLength);
		SetTitle(ucsp->hWndMain,szBuf);
		InvalRect(&rcP);
		Update();
		RestorePort(temport);
	}
error:

	Remove(hOpenDlg);
}


/*******************************************************************************

  Function : UDV_CSeq_cancelProc()
  
  Purpose : manage cancel button of the Choose sequence dialog box 
  
  Parameters : g; button
  
  Return value : none 

*******************************************************************************/
static void UDV_CSeq_cancelProc(ButtoN g)
{	
WindoW				hOpenDlg;
UDVChooseSeqPtr 	ucsp;
ViewerMainPtr 		vmp;

	hOpenDlg=(WindoW)ParentWindow(g);
	if (!hOpenDlg) return;
	
	ucsp=(UDVChooseSeqPtr)GetObjectExtra (hOpenDlg);
	if (ucsp==NULL) goto error;
	
	vmp = (ViewerMainPtr) GetObjectExtra (ucsp->hWndMain);
	if (vmp==NULL) goto error;
	
	UDV_set_PullMenus(&vmp->MainMenu,TRUE);

error:

	Remove(hOpenDlg);
}

/*******************************************************************************

  Function : UDV_CreateListBioseqDlg()
  
  Purpose : let the user choose between various bsp 
  
  Parameters : i; menu
  
  Return value : none 

*******************************************************************************/
NLM_EXTERN void UDV_CreateListBioseqDlg(IteM i)
{
GrouP 			g1,h,h1;
LisT 			lBox;
WindoW 			d,hWinMain;
ValNodePtr 		vnp;
BioseqPtr 		bsp;
Char 			szName[21]={""};
Char 			szBuf[50]={""};
ViewerMainPtr 	vmp;
UDVChooseSeqPtr ucsp;
Uint2			eID,iID;

	ucsp=(UDVChooseSeqPtr)MemNew(sizeof(UDVChooseSeq));
	if (ucsp==NULL) return;
	
	hWinMain=(WindoW)ParentWindow(i);

	if (!hWinMain) return;
	
	vmp = (ViewerMainPtr) GetObjectExtra (hWinMain);

	if (vmp==NULL) return;

	if (vmp->BspTable==NULL) return;
	
	d=/*DocumentWindow*/Nlm_MovableModalWindow(-50, -33 ,-10, -10, szAppName, NULL/*,NULL*/);
	if (d!=NULL){
		UDV_set_PullMenus(&vmp->MainMenu,FALSE);
		/*create some controls*/
		h=HiddenGroup(d, 3, 0,  NULL);
		h1=HiddenGroup(h, 0, 2,  NULL);
		StaticPrompt(h1,"Choose a sequence :",0,0,systemFont,'l');

		lBox=SingleList(h1,20,6,NULL);

		g1=HiddenGroup(h, 0, 2, NULL);
		PushButton(g1, "Ok",UDV_CSeq_okProc);
		PushButton(g1, "Cancel",UDV_CSeq_cancelProc);
		/*fill in the list box*/
		for (vnp=vmp->BspTable ; vnp!=NULL ;vnp=vnp->next){
			UDV_DecodeIdxFeat ((Uint4)vnp->data.intvalue,&eID,&iID);
			bsp=GetBioseqGivenIDs (eID, iID, OBJ_BIOSEQ);
			if (bsp){
				SeqIdWrite(bsp->id,szName,
						PRINTID_TEXTID_ACCESSION,20);
				if (szName){
					sprintf(szBuf,"%s (%s)",
						szName,
						(ISA_na(bsp->mol) ? "nucleic seq.":"protein"));
				}else{
					sprintf(szBuf,"Unknown name (%s)",
						(ISA_na(bsp->mol) ? "nucleic seq.":"protein"));
				}
				ListItem(lBox,szBuf);
			}
		}
		SetValue(lBox,vmp->BspChoice);

		ucsp->bsp_list=lBox;
		ucsp->hWndMain=hWinMain;
		SetObjectExtra (d, (Pointer) ucsp, StdCleanupExtraProc);
		RealizeWindow(d);
		Show(d);
	}
}


#if 0 

static CharPtr udvprotdbs [] = {
  "swissprot", "nr", "pdb", "month", "kabat", "yeast", NULL
};


static MonitorPtr blastmon = NULL; /* ugh.  used below */

/*****************************************************************************

Function: UDV_BlastCallback

Purpose: creates, updates and deletes blast status dialog

*****************************************************************************/
static Boolean LIBCALLBACK UDV_BlastCallback (BlastResponse *brp,
                                               Boolean *cancel)
{
    BlastProgressPtr bpp;
    Int4 completed = 0;
    Boolean retval;
    
    *cancel=FALSE;
    switch (brp->choice) {
    case BlastResponse_start:
        if (blastmon != NULL) {
            MonitorFree (blastmon);
            blastmon = NULL;
        }
        bpp = (BlastProgressPtr) brp->data.ptrvalue;
        if (bpp != NULL) {
            completed = bpp->completed;
        }
        if (completed == 0) {
            completed = 100;
        }
        blastmon = MonitorIntNew ("BLAST", 0, completed);
        return TRUE;
    case BlastResponse_progress:
        if (blastmon != NULL) {
            bpp = (BlastProgressPtr) brp->data.ptrvalue;
            if (bpp != NULL) {
                completed = bpp->completed;
            }
            retval = MonitorIntValue (blastmon, completed);
            if (retval == FALSE) {
                *cancel = TRUE;
                MonitorFree (blastmon);
                blastmon = NULL;
                return FALSE;
            }
        }
        return TRUE;
    case BlastResponse_queued:
        return TRUE;
    case BlastResponse_done:
        if (blastmon != NULL) {
            MonitorFree (blastmon);
            blastmon = NULL;
        }
        return TRUE;
    default:
        break;
    }
    return FALSE;
}

/*****************************************************************************

Function: UDV_BlastDlgOK

Purpose: OK button for UDV_BlastDlg

*****************************************************************************/
static void UDV_BlastDlgOK(ButtoN g)
{
    UDV_BlastDlgData *bddp;
    Int2 value;
    WindoW hOpenDlg;
    BLAST_OptionsBlkPtr options;
    Int4 i;
    ValNode *pvn;
    Bioseq* bsp;
    BlastNet3Hptr bl3hp;
    BlastResponsePtr bl3rp;
    Char str[32], *program;

    hOpenDlg=(WindoW)ParentWindow(g);
    if (!hOpenDlg) return;
    
    bddp=(UDV_BlastDlgData *)GetObjectExtra (hOpenDlg);
    if (bddp == NULL) {
        ValNodeFree(bddp->pvnSips);
        Remove(hOpenDlg);
        return;
    }
    

    BlastInit("udv 1.0", &bl3hp, &bl3rp);
    value = GetValue(bddp->bsp_list); /*one-base value*/

    for(pvn = bddp->pvnSips, i = 1; i != value && pvn != NULL;
        i++, pvn = pvn->next);

    if(pvn == NULL) {
        ValNodeFree(bddp->pvnSips);
        Remove(hOpenDlg);
        return;
    }

    bsp = BioseqLockById((SeqId *)pvn->data.ptrvalue);

    if(bsp == NULL) {
        ValNodeFree(bddp->pvnSips);
        Remove(hOpenDlg);
        return;
    }

    if(ISA_aa(bsp->mol)) program = "blastp";
    else program = "blastn";

    options = BLASTOptionNew (program, GetStatus(bddp->bGap));
    GetTitle(bddp->tMax, str, sizeof(str));
    options->hitlist_size = atoi(str);
    if (options->hitlist_size == 0) options->hitlist_size = 20;
/*    GetTitle(bddp->tExpect, str, sizeof(str));
    options->expect_value = atof(str);
    if (options->expect_value == 0.0) options->expect_value = 0.001;*/
    
    Remove(hOpenDlg);

    value = GetValue(bddp->db_list); /*one-base value*/

    bddp->salp = BlastBioseqNet(bl3hp, bsp, program, udvprotdbs[value-1],
        options, NULL, NULL, UDV_BlastCallback);
    BLASTOptionDelete(options);
    BioseqUnlock(bsp);
    ValNodeFree(bddp->pvnSips);
    if(bddp->salp == NULL) return;

    if(bddp->callback) bddp->callback(bddp);   
}

/*****************************************************************************

Function: UDV_BlastDlgCancel

Purpose: cancel button for UDV_BlastDlg
  
*****************************************************************************/
static void UDV_BlastDlgCancel(ButtoN g)
{	
    WindoW hOpenDlg;
    UDV_BlastDlgData *bddp;
    
    hOpenDlg=(WindoW)ParentWindow(g);
    if (!hOpenDlg) return;
    
    bddp = (UDV_BlastDlgData *)GetObjectExtra (hOpenDlg);
    if (bddp != NULL) ValNodeFree(bddp->pvnSips);
    
    Remove(hOpenDlg);
}

/*****************************************************************************

Function: UDV_BlastDlg

Purpose: Creates blast many dialog
  
*****************************************************************************/
NLM_EXTERN void UDV_BlastDlg(UDV_BlastDlgData *bddp)
{
    GrouP h2,h,h1, h3, h4, h5;
    WindoW d;
    Char szName[21]={""};
    ValNode *pvn;
    Int4 i;

    if(bddp == NULL) return;

    d = MovableModalWindow(-50, -33 ,-10, -10, "Blast", NULL);
    if (d != NULL){
        /*create some controls*/
        h = HiddenGroup(d, 1, 5,  NULL);
        h1 = HiddenGroup(h, 1, 2,  NULL);
        StaticPrompt(h1,"Choose a sequence :",0,0,systemFont,'l');
        
        bddp->bsp_list = SingleList(h1,20,6,NULL);

        h3 = HiddenGroup(h, 1, 1, NULL);
        bddp->bGap = CheckBox(h3, "Gapped", NULL);
        SetStatus(bddp->bGap, TRUE);

        h4 = HiddenGroup(h, 2, 1, NULL);
/*        StaticPrompt(h4,"Expectation value:",0,0,systemFont,'l');
        bddp->tExpect = DialogText(h4, "0.01", 8, (TxtActnProc) NULL); */
        StaticPrompt(h4,"Maximum # of hits:",0,0,systemFont,'l');
        bddp->tMax = DialogText(h4, "20", 5, (TxtActnProc) NULL);

        h4 = HiddenGroup (h, 2, 1, NULL);
        StaticPrompt (h4, "Choose Database:", 0, 0, programFont, 'c');
        
#ifdef WIN_MAC
        bddp->db_list = PopupList(h4, TRUE, NULL);
#else 
        bddp->db_list = PopupList(h4, FALSE, NULL);
#endif
        for (i = 0; udvprotdbs [i] != NULL; i++)
            PopupItem(bddp->db_list, udvprotdbs [i]);
        SetValue(bddp->db_list,1);

        h2 = HiddenGroup(h, 2, 1, NULL);
        PushButton(h2, "Ok", UDV_BlastDlgOK);
        PushButton(h2, "Cancel", UDV_BlastDlgCancel);

        h5 = HiddenGroup(h, 1, 1, NULL);
        StaticPrompt(h5,"Warning: may take several minutes!",0,0,systemFont,'l');

        /*fill in the list box*/
        for(pvn = bddp->pvnSips; pvn != NULL; pvn = pvn->next) {
            SeqIdWrite((SeqId *)pvn->data.ptrvalue,szName,
                PRINTID_TEXTID_ACCESSION,20);
            ListItem(bddp->bsp_list, szName);
        }
        SetValue(bddp->bsp_list,1);
        
        SetObjectExtra (d, (Pointer) bddp, StdCleanupExtraProc);
        RealizeWindow(d);
        Show(d);
    }
}

#endif /* 0 */
