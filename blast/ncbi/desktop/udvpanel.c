/*   udvpanel.c
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
* File Name:  udvpanel.c
*
* Author:  Patrick Durand
*
* Version Creation Date:   5/3/99
*
* $Revision: 6.53 $
*
* File Description: 
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: udvpanel.c,v $
* Revision 6.53  2002/03/14 16:38:48  kans
* SeqMgrIndexFeaturesEx takes new parameter
*
* Revision 6.52  2000/06/27 20:46:38  hurwitz
* fixed bugs with select rectangle, added select row option
*
* Revision 6.51  2000/06/16 17:30:40  kans
* UDVResetProc sends TRUE to UDV_FreeVDPstruct
*
* Revision 6.50  2000/05/19 13:48:31  hurwitz
* made a version of DDE that doesn't allow aligned gaps, changed wording for adding new rows
*
* Revision 6.49  2000/05/18 13:42:16  lewisg
* get rid of features list for all but standalone udv
*
* Revision 6.48  2000/05/17 21:32:41  lewisg
* disable feature search except for standalone udv
*
* Revision 6.47  2000/05/15 23:39:34  lewisg
* shred cblast, add menu items for gapped/ungapped, fix pdbheaders
*
* Revision 6.46  2000/05/09 17:00:46  kans
* remove monitor when creating udv panel
*
* Revision 6.45  2000/05/01 21:21:13  lewisg
* make features dialogs modal
*
* Revision 6.44  2000/04/27 22:21:58  lewisg
* misc bugs/features
*
* Revision 6.43  2000/04/27 19:50:59  kans
* implemented udv reset and select (PD+JK)
*
* Revision 6.42  2000/04/27 15:38:02  hurwitz
* changed wording on menu
*
* Revision 6.41  2000/04/20 23:27:43  lewisg
* misc bug fixes
*
* Revision 6.40  2000/04/17 21:46:56  lewisg
* do not do double index on viewmgr update, rename menus
*
* Revision 6.39  2000/04/13 13:58:02  durand
* allowed udv to display reverse complement sequence
*
* Revision 6.38  2000/04/11 13:19:26  durand
* added UDV_LoadSpecificEditor for Sequin
*
* Revision 6.37  2000/04/10 21:41:27  lewisg
* move alignment menus into ddv, udv from cn3d
*
* Revision 6.36  2000/04/05 21:54:17  kans
* resize udvpanel, get vdp directly from slate
*
* Revision 6.35  2000/04/05 21:13:20  kans
* more changes to allow udv panel to be added to Sequin viewer
*
* Revision 6.34  2000/03/31 13:51:22  durand
* added UDV_InitForSequin()
*
* Revision 6.33  2000/03/30 19:54:09  kans
* initial work to get UDV and DDV into Sequin viewer
*
* Revision 6.32  2000/03/22 20:37:29  durand
* added system menus to slave UDV
*
* Revision 6.31  2000/03/15 19:32:20  lewisg
* launch only single udv window
*
* Revision 6.30  2000/03/07 17:34:14  durand
* avoid udv sending an OM_MSG_FLUSH message when it runs as a slave
*
* Revision 6.29  2000/03/06 14:00:47  durand
* first release of the Summary viewer done
*
* Revision 6.28  2000/03/02 15:47:52  durand
* use MovableModalWindow for dialog boxes
*
* Revision 6.27  2000/02/19 01:23:59  lewisg
* use ibm, add row tracking code, various bug fixes
*
* Revision 6.26  2000/02/16 22:38:29  durand
* fixed some wierd behaviours of features selections
*
* Revision 6.25  2000/02/15 22:40:58  lewisg
* add ability to launch udv so that it colors by row, fixes to colormgr, track rows from viewmgr, fix visual c projects
*
* Revision 6.24  2000/02/11 15:40:29  durand
* replaced ObjMgrSendMsg by ObjMgrSelect calls for features selection
*
* Revision 6.23  2000/02/07 20:17:37  lewisg
* minor bug fixes, use gui font for win32
*
* Revision 6.22  2000/01/24 16:15:22  durand
* add loader function for the genome viewer
*
* Revision 6.21  2000/01/18 22:49:16  lewisg
* send OM_MSG_FLUSH to ddv/udv, tweak CPK coloration, misc bugs
*
* Revision 6.20  2000/01/12 15:06:52  durand
* .
*
* Revision 6.19  2000/01/11 15:03:18  durand
* remove network stuff
*
* Revision 6.18  2000/01/10 15:29:26  durand
* add include to netcnfg include file
*
* Revision 6.17  2000/01/10 15:18:41  durand
* use Entrez instead of ID1
*
* Revision 6.16  2000/01/08 00:47:54  lewisg
* fixes to selection, update, color
*
* Revision 6.15  2000/01/05 21:06:37  durand
* update mouse click actions and DrawSequence function for a better use from ddv and cn3d
*
* Revision 6.14  2000/01/03 21:06:38  durand
* update the way the udv main window is created
*
* Revision 6.13  1999/12/28 23:06:34  lewisg
* udv/cn3d communication
*
* Revision 6.12  1999/12/15 23:17:47  lewisg
* make cn3d launch udv, fix launching of non-autonomous udv, add mouseup message
*
* Revision 6.11  1999/11/29 15:17:55  durand
* designed a new GUI; fixed problems under Win95 and Linux
*
* Revision 6.10  1999/11/19 15:01:47  durand
* speed up mouse selection ; avoid sequence flashing during selection ; update menu functionalities
*
* Revision 6.9  1999/11/09 21:06:58  durand
* add sequence selection manager
*
* Revision 6.8  1999/10/02 15:11:16  durand
* update the code to be used by wwwudv
*
* Revision 6.7  1999/09/07 19:38:45  durand
* don't display special features
*
* Revision 6.6  1999/07/30 20:08:57  durand
* updates for the new Entrez graphical viewer
*
* Revision 6.5  1999/06/08 13:52:36  durand
* update UDV data structures for the MSA editor
*
* Revision 6.4  1999/06/07 15:39:44  durand
* add LOG line to keep track of the history
*
*
*
* ==========================================================================
*/

#include <udviewer.h>
#include <samutil.h>
#include <viewmgr.h>
#ifdef UDV_GENOME
#include <udvgv.h>
#endif

/*local text*/
static Char szAppName[]="OneD-Viewer";

/*local text*/
static Char szAbout[]="OneD-Viewer : A sequence viewer for GenBank\n\
Version 1.0.2\n\nInformation Engineering Branch\n\
NCBI - NLM - NIH, Bldg 38A\n\
8600 Rockville Pike\n\
Bethesda, MD 20894 - USA\n\n\
info@ncbi.nlm.nih.gov";

/*struture passed to the FEAT List dialog box; one struct by entry*/
#define COMMENT_SIZE 100
typedef  struct  fldata {
	Int4 num;	/*order num*/		
	Int4 from;	/*position of the feature on the sequence*/
	Int4 to;
	Int2 segments;	/*feature constituted by several seg. (ex: RNA)*/
	Char szType[COMMENT_SIZE];	/*names of the feature*/
	Char szComment[COMMENT_SIZE];
	Uint2 eID;	/*entity ID of this feature*/
	Uint2 iID;	/*item ID of this feature*/
	Uint1 strand;	/*minus, plus, ...*/
	} FLData, PNTR FLDataPtr;

typedef  struct  fksdata {
	WindoW 	hWinMain;
	TexT	keyField;
	ButtoN  bOk;
	}FKSData, PNTR FKSDataPtr;

/*use to initialize the Feature List Dialog box*/
static char *szSeqFeatClassName[]={
				"All",
				"Gene",
				"Organism",
				"CD region",
				"Protein",
				"RNA",
				"Publication",
				"SEQ",
				"IMP",
				"Region",
				"Comment",
				"Bond",
				"Site",
				"RSITE",
				"USER",
				"TXINIT",
				"Numbering",
				"Secondary structure",
				"Non standard residue",
				"Heteroatom",
				"Biosource",
				};

/*local functions*/
static ValNodePtr UDV_GetParaGNodeGivenPos(ValNodePtr ParaG_head,Int4 bsp_pos);
static void  UnDViewerVScrlUpdatePage(Int4Ptr PageUpDn,Int2 cyClient,
				Int2 LineHeight);
static void  PanelOrgChange(PaneL p,ViewerDialogDataPtr vdp);
static Boolean  CreateUDVpanel(WindoW w,ViewerMainPtr PNTR vmp,
                               SAM_ViewGlobal *vgp);
static void UDV_ReLocateRcParaGList(RecT rcP,Boolean ShowTop,Boolean ShowTick,
			Boolean ShowSequence, Boolean ShowFeatures,
			Boolean ShowBlank,Int4Ptr nTotL,ValNodePtr ParaG_head);
static void ShowFeaturesListDlg(IteM i);
static void UDV_SearchFeatForKey(IteM i);
static void UDV_ShowReverseComplement(IteM i);
static void  UDV_analyse_buffer(UnDViewerGraphDataPtr GrData,
		ValNodePtr ParaG_list,BspInfoPtr bsp_i,Uint2 ActionType,Boolean bReverse);
#ifdef UDV_GENOME
static void UDV_GenViewProc(IteM I);
#endif
/*******************************************************************************

  Function : UDV_TimerProc()
  
  Purpose :  function called by the timer. Right now, this function is used
     to highlighte the GoTo letter.
  				    
  Return value : 

*******************************************************************************/
static void UDV_TimerProc (WindoW w)
{
ViewerDialogDataPtr vdp;
ViewerMainPtr		vmp;
ValNodePtr          vnp;
ParaGPtr            pgp;
Int4                bsp_pos;

	vmp=(ViewerMainPtr)GetObjectExtra(w);
	if (vmp==NULL || vmp->vdp==NULL) return;
	vdp = vmp->vdp;
	if (vdp==NULL) return;
	if (vdp->ParaG==NULL) return;
		
	if (vdp->udt.status==UDV_SET_TIMER){
		vdp->udt.delay=0;
		if (vdp->udt.action==UDV_INVAL_REGION){
			vnp=UDV_GetParaGNodeGivenPos(vdp->ParaG,vdp->udt.pos);
			if (!vnp) return;
			pgp=(ParaGPtr)vnp->data.ptrvalue;
			UDV_InvalRegion(vdp->UnDViewer,&(vdp->udv_graph),
				pgp,_max_(0,vdp->udt.pos-1),vdp->udt.pos+1,FALSE);
		}
		vdp->udt.status=UDV_TEST_TIMER;
	}
	if (vdp->udt.status==UDV_TEST_TIMER){
		if (vdp->udt.delay>10){
			if (vdp->udt.action==UDV_INVAL_REGION){
				bsp_pos=vdp->udt.pos;
				vdp->udt.pos=0;
				vnp=UDV_GetParaGNodeGivenPos(vdp->ParaG,bsp_pos);
				if (!vnp) return;
				pgp=(ParaGPtr)vnp->data.ptrvalue;
				vdp->udv_graph.GotoLetter=0;
				UDV_InvalRegion(vdp->UnDViewer,&(vdp->udv_graph),
					pgp,_max_(0,bsp_pos-1),bsp_pos+1,FALSE);
				vdp->udt.action=UDV_NOTHING;
			}
		}
	}
	vdp->udt.delay++;
}


/*******************************************************************************

  Function : UDV_GetSelectedRegions()
  
  Purpose :  get the selected region(s) of one bioseq.
  
  Parameters :	om_ssp;list of selected regions (usually this field points to
                  ObjMgr data)
				bsp_eID,bsp_iID; bioseq identifiers.
				    
  Return value : list of selected regions on the bioseq bsp_eID,bsp_iID

*******************************************************************************/
NLM_EXTERN ValNodePtr UDV_GetSelectedRegions(SelStructPtr om_ssp, Uint2 bsp_eID,
	Uint2 bsp_iID)
{
SelStructPtr ssp;
SeqLocPtr    slp,slp2;
ValNodePtr   bsp_vnp=NULL,vnp;

	if (om_ssp==NULL || bsp_eID==0 || bsp_iID==0) return(NULL);

	ssp=om_ssp;
	
	while(ssp){
		if (ssp->entityID==bsp_eID && ssp->itemID==bsp_iID && 
			ssp->itemtype==OBJ_BIOSEQ && ssp->regiontype==OM_REGION_SEQLOC){
			slp=(SeqLocPtr)ssp->region;
			while(slp){
				slp2=slp->next;
				slp->next=NULL;
				if (!bsp_vnp){
					vnp=ValNodeAddPointer(NULL,0,(Pointer)slp);
					if (!vnp) return(NULL);
					bsp_vnp=vnp;
				}
				else{
					vnp=ValNodeAddPointer(&vnp,0,(Pointer)slp);
					if (!vnp){
						if (bsp_vnp) ValNodeFree(bsp_vnp);
						return(NULL);
					}
				}
				slp->next=slp2;
				slp=slp->next;
			}
		}
		ssp=ssp->next;
	}
	return(bsp_vnp);
}

/*******************************************************************************

  Function : UDV_IsLetterSelected()
  
  Purpose : check if a bsp_pos is selected (vnp_bsp is usually built with 
      DDV_GetSelectedRegions() function)
  
  Return value : TRUE if bsp_pos is selected

*******************************************************************************/
NLM_EXTERN Boolean UDV_IsLetterSelected(ValNodePtr vnp_bsp, Int4 bsp_pos,
		Int4 bspLength,Boolean bDisplayRevComp)
{
Boolean    bSelected=FALSE;
ValNodePtr vnp;
SeqLocPtr  slp;
Int4 bsp_start,bsp_stop;
	if (vnp_bsp==NULL || bsp_pos==(Int4)-1) return(FALSE);

	vnp=vnp_bsp;

	while(vnp){
		slp=(SeqLocPtr)vnp->data.ptrvalue;
		bsp_start=SeqLocStart(slp);
		bsp_stop=SeqLocStop(slp);
		if (bDisplayRevComp){
			bsp_start=UDV_RevertBioSeqCoord(bsp_start,bspLength);
			bsp_stop=UDV_RevertBioSeqCoord(bsp_stop,bspLength);
			if (bsp_stop<bsp_start)
				swap(&bsp_start,&bsp_stop);
		}
		if (bsp_pos>=bsp_start && bsp_pos<=bsp_stop){
			bSelected=TRUE;
			break;
		}
		vnp=vnp->next;
	}
	
	return(bSelected);
}

/*****************************************************************************

Function: UDV_GetClosetSeqLocGivenBspPos()

Purpose: given a position in the Bioseq, this function return the closest
         selected region on that bioseq.
		
Parameters:	sip, eID, iID; bsioseq identifiers
            bsp_pos; current position on that bioseq
			old_pos; returns old_pos of a selection (used only if bModify is TRUE)
			bModify; if TRUE, create a modified slp

Note : sip can be null ONLY if bModify if FALSE.

Return value: see Purpose.

*****************************************************************************/
NLM_EXTERN SeqLocPtr UDV_GetClosetSeqLocGivenBspPos(SeqIdPtr sip, Uint2 eID, 
		Uint2 iID, Int4 bsp_pos, Int4Ptr old_pos, Boolean bModify)
{
SelStructPtr ssp;
SeqLocPtr    slp=NULL;
ValNodePtr   vnp_bsp,vnp;
Int4         diff,old_diff,diff_l,diff_r,bsp_start,bsp_stop;
Uint1        strand;

	ssp=ObjMgrGetSelected();
	if (ssp==NULL) return(NULL);
	
	vnp_bsp=UDV_GetSelectedRegions(ssp,eID,iID);
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

/*******************************************************************************

  Function : UDV_GetParaGNodeGivenPos()
  
  Purpose : given a bsp_pos, get back the node in the ParaG list
  
  Return value : -

*******************************************************************************/
static ValNodePtr UDV_GetParaGNodeGivenPos(ValNodePtr ParaG_head,Int4 bsp_pos)
{
ValNodePtr vnp,vnp_find;
ParaGPtr   pgp;
	
	vnp_find=NULL;
	vnp=ParaG_head;
	while(vnp){
		if (vnp->data.ptrvalue){
			pgp=(ParaGPtr)vnp->data.ptrvalue;
			if (bsp_pos>=pgp->StartLetter && bsp_pos<=pgp->StopLetter){
				vnp_find=vnp;
				break;
			}
		}
		vnp=vnp->next;
	}
	return(vnp_find);
}

/*******************************************************************************

  Function : UDV_GetLineNumGivenPos()
  
  Purpose : given a bsp_pos, get back the line number
  
  Return value : -

*******************************************************************************/
static ValNodePtr UDV_GetLineNumGivenPos(ValNodePtr ParaG_head,Int4 bsp_pos,
	Int4Ptr line_num)
{
ValNodePtr vnp;
ParaGPtr   pgp;

	vnp=UDV_GetParaGNodeGivenPos(ParaG_head,bsp_pos);
	
	if (!vnp) {
		*line_num=(Int4)-1;
		return(NULL);
	}
	
	pgp=(ParaGPtr)vnp->data.ptrvalue;
	*line_num=pgp->StartLine;
	
	return(vnp);
}

/*******************************************************************************

  Function : UDV_MSG_SELECT()
  
  Purpose : manage select/deselect messages
  
  Return value : -

*******************************************************************************/
static Int2 UDV_MSG_SELECT(ViewerDialogDataPtr vdp,OMMsgStructPtr ommsp,
		Boolean IsSelect)
{
SeqLocPtr     slp;
ParaGPtr      pgp;
ValNodePtr    vnp_start,vnp;
Int4          from_bsp,to_bsp,bsp_start,bsp_stop,tot,pop,decal,start_inval,stop_inval;
Uint2         bsp_eID,bsp_iID;

	/*some usefull tests...*/
	if (vdp == NULL) return(OM_MSG_RET_ERROR);
	if (ommsp->itemtype != OBJ_BIOSEQ) return(OM_MSG_RET_OK);
	if (ommsp->regiontype!=OM_REGION_SEQLOC) return(OM_MSG_RET_OK);

	/*Am I concern by that Bioseq ?*/
	bsp_eID=ObjMgrGetEntityIDForPointer((Pointer)vdp->bsp_i.bsp);
	bsp_iID = GetItemIDGivenPointer (bsp_eID, 
			OBJ_BIOSEQ, (Pointer) vdp->bsp_i.bsp);	
	if (bsp_eID!=ommsp->entityID || bsp_iID!=ommsp->itemID) return(OM_MSG_RET_ERROR);

	/*get the current display range*/
	UDV_GetCurrentDispRange(vdp,&from_bsp,&to_bsp,NULL,NULL);
	if (from_bsp==0 && to_bsp==0) return(OM_MSG_RET_ERROR);
	
	/*scan the SeqLoc list to figure out if it's currently on the screen*/
	slp=(SeqLocPtr)ommsp->region;
	while(slp){/*for each SeqLoc, I try to see what region(s) has (have) to be
		uptdated*/
		bsp_start=SeqLocStart(slp);
		bsp_stop=SeqLocStop(slp);
		/*when udv displays a reverse/complement sequence, bsp_coords are
		actually display coords. */
		if (vdp->bDisplayRevComp){
			bsp_start=UDV_RevertBioSeqCoord(bsp_start,vdp->bsp_i.bspLength);
			bsp_stop=UDV_RevertBioSeqCoord(bsp_stop,vdp->bsp_i.bspLength);
			if (bsp_stop<bsp_start)
				swap(&bsp_start,&bsp_stop);
		}
		if (bsp_start<=to_bsp && bsp_stop>=from_bsp){
			vnp_start=UDV_GetParaGNodeGivenPos(vdp->ParaG,bsp_start);
			vnp=vnp_start;
			tot=bsp_stop-bsp_start+1;
			pop=decal=0;
			while(vnp){
				pgp=(ParaGPtr)vnp->data.ptrvalue;
				start_inval=bsp_start+decal;
				stop_inval=_min_(bsp_stop,pgp->StopLetter);
				UDV_InvalRegion(vdp->UnDViewer,&(vdp->udv_graph),pgp,
					start_inval,stop_inval,IsSelect);
				pop+=(stop_inval-start_inval+1);
				if (pop>=tot)
					break;
				decal=pop;
				vnp=vnp->next;
			}
		}
		slp=slp->next;
	}
	return(OM_MSG_RET_OK);
	
}

/*******************************************************************************

  Function : UDV_MSG_FLUSH()
  
  Purpose : kill the viewer in response to a OM_MSG_FLUSH message
  
  Return value : OM_MSG_RET_OK if success

*******************************************************************************/
static Int2 UDV_MSG_FLUSH(ViewerDialogDataPtr vdp,OMMsgStructPtr ommsp)
{
Uint2  bsp_eID,bsp_iID;
ViewerMainPtr vmp;

	/*standalone UDV : doesn't answer this message*/
	if (vdp->Parent){
		vmp = (ViewerMainPtr)GetObjectExtra(vdp->Parent);
		if (vmp && vmp->AutonomeViewer==TRUE) 
			return(OM_MSG_RET_OK);
	}	

	/*Am I concern by that Bioseq ?*/
	bsp_eID=ObjMgrGetEntityIDForPointer((Pointer)vdp->bsp_i.bsp);
	bsp_iID = GetItemIDGivenPointer (bsp_eID, 
			OBJ_BIOSEQ, (Pointer) vdp->bsp_i.bsp);	

	if (bsp_eID==ommsp->entityID &&
		((bsp_iID==ommsp->itemID && ommsp->itemtype==OBJ_BIOSEQ)
        || (ommsp->itemID == 0 && ommsp->itemtype==0))){
		Remove(ParentWindow(vdp->UnDViewer));
	}
	return(OM_MSG_RET_OK);
}

/*******************************************************************************

  Function : UDV_OM_MsgFunc()
  
  Purpose : ObjMgr message loop of the viewer
  
  Parameters : see Toolkit
  
  Return value : see Toolkit 

*******************************************************************************/
static Int2 LIBCALLBACK UDV_OM_MsgFunc (OMMsgStructPtr ommsp)
{
OMUserDataPtr omudp;
/*BioseqPtr bsp;*/
ViewerDialogDataPtr vdp;
ViewerMainPtr vmp = NULL;
   
	omudp = (OMUserDataPtr)(ommsp->omuserdata);
	vdp = (ViewerDialogDataPtr)(omudp->userdata.ptrvalue);

	if (!vdp) return(OM_MSG_RET_OK);
		
	switch (ommsp->message)
	{
		case OM_MSG_DEL:
			break;
		case OM_MSG_CREATE:
			break;
		case OM_MSG_UPDATE:

			if (vdp->UnDViewer){
				RecT rc;
                WindoW temport;
                temport=SavePort((WindoW)vdp->UnDViewer);
	            Select (vdp->UnDViewer);
	            ObjectRect(vdp->UnDViewer,&rc);
	            InvalRect(&rc);
	            RestorePort(temport);
			}
			break;
		case OM_MSG_SELECT:
			/*Click Feat ?*/
			if (ommsp->itemtype==OBJ_SEQFEAT) {
				UDV_select_feature(vdp->UnDViewer,vdp,ommsp->entityID,
					ommsp->itemID,TRUE);
					/*(Boolean)(vdp->ClickFeatFromDlg ? TRUE : FALSE));*/
				if (!vdp->ClickFeatFromDlg){
					if (vdp->Parent) 
						vmp=(ViewerMainPtr)GetObjectExtra(vdp->Parent);
					if (vmp && vmp->hFeatDlg){/*update Features List Dlg if needed;
						to do : convert this Feature ListBox to a real Viewer able
						to play with ObjMgr*/
						UDV_SelectFeatInFeatDlg(vmp,ommsp->entityID,ommsp->itemID);
					}
				}
				vdp->ClickFeatFromDlg=FALSE;
			}
			else if (ommsp->itemtype==OBJ_BIOSEQ){
				UDV_MSG_SELECT(vdp,ommsp,TRUE);
			}
			break;
		case OM_MSG_DESELECT: 
			if (ommsp->itemtype==OBJ_SEQFEAT) {
				UDV_deselect_feature(vdp);
			}
			else if (ommsp->itemtype==OBJ_BIOSEQ){
				UDV_MSG_SELECT(vdp,ommsp,FALSE);
			}
			break;
		case OM_MSG_CACHED:
			break;
		case OM_MSG_UNCACHED:
			break;
		case OM_MSG_TO_CLIPBOARD: 
			break;
		case OM_MSG_SETCOLOR:
			if (vdp->UnDViewer){
				RecT rc;
				ObjectRect(vdp->UnDViewer,&rc);
				InvalRect(&rc);
				Update();
			}
			break;
		case OM_MSG_FLUSH:
			UDV_MSG_FLUSH(vdp,ommsp);
			break;
		default:
			break;
	}

	return OM_MSG_RET_OK;
}

/*******************************************************************************

  Function : UDV_ObjRegAutonomous()
  
  Purpose : call by ObjMgr to start the viewer; Autonomous version
  
  Parameters : see Toolkit
  
  Note: only for the Autonomous viewer purpose
  
  Return value : see Toolkit 

*******************************************************************************/
NLM_EXTERN Int2 LIBCALLBACK UDV_ObjRegAutonomous (Pointer data)
{
WindoW              w=NULL;
OMProcControlPtr    ompcp;
OMUserDataPtr       omudp;
BioseqPtr			bsp=NULL;
ViewerMainPtr 		vmp=NULL;
ViewerDialogDataPtr vdp=NULL;
Char 				szBuf[255]={""};
UdvGlobalsPtr       ugp=NULL;
RecT                rcP;
WindoW              temport;
SAM_ViewGlobal      *vgp=NULL;
Uint2               eID, iID;

    vgp = GetAppProperty(SAM_ViewString);

	/*retrieve data*/
	ompcp = (OMProcControlPtr) data;
	
	if (ompcp == NULL || ompcp->proc == NULL) {
		ErrPostEx (SEV_ERROR, 0, 0, "Data NULL [1]");
		return OM_MSG_RET_ERROR;
	}

	/*initialize the viewer windows (viewer and info panels)*/
	/*autonomous viewer ?*/
	ugp=(UdvGlobalsPtr)GetAppProperty("UdvGlobals");
	if (ugp) vmp=ugp->vmp;

    	/*if w and/or vmp are NULL, they'll be initialized in this function*/
	if (vmp || ((vmp = (ViewerMainPtr)GetAppProperty("nonautonomous")) != NULL)) {/*reuse the viewer if already there*/
		w=vmp->hWndMain;
		if (!CreateUDVpanel(w,&vmp, vgp)) 
			return OM_MSG_RET_ERROR;
	}
    else {/*for each call, create a new viewer window*/
		/*non-autonomous viewer*/
		if (!CreateUDVpanel(w,&vmp, vgp)) return OM_MSG_RET_ERROR;
	}

	/*init specific data for the ObjMgr msg loop*/	
	vdp=vmp->vdp;
	if (!vdp) return OM_MSG_RET_ERROR;

	switch (ompcp->input_itemtype) {
		case OBJ_BIOSEQ :
			bsp = (BioseqPtr) ompcp->input_data;
            UDV_CreateOneFeatureIndex(0, bsp, vdp->bDisplayRevComp);
			break;
        case OBJ_BIOSEQSET:
            UDV_CreateOneFeatureIndex(ompcp->input_entityID, NULL, vdp->bDisplayRevComp);
            vmp->BspTable = NULL;
            SeqMgrExploreBioseqs (ompcp->input_entityID, NULL, (Pointer) &vmp->BspTable, 
			SearchBioseq, TRUE, TRUE, TRUE);
            if(vmp->BspTable == NULL) return OM_MSG_RET_ERROR;
            UDV_DecodeIdxFeat ((Uint4)vmp->BspTable->data.intvalue,&eID,&iID);
	        bsp=GetBioseqGivenIDs (eID, iID, OBJ_BIOSEQ);
            if(bsp == NULL) return OM_MSG_RET_ERROR;
			break;
		case 0 :
			return OM_MSG_RET_ERROR;
		default :
			return OM_MSG_RET_ERROR;
	}
	
	if (bsp == NULL) {
		ErrPostEx (SEV_ERROR, 0, 0, "Data NULL [2]");
		return OM_MSG_RET_ERROR;
	}

	vdp->procid=ompcp->proc->procid;
	vdp->proctype=ompcp->proc->proctype;
	vdp->userkey=OMGetNextUserKey ();
    if( vgp != NULL ) {
        if (vgp->MasterViewer == SAMVIEWDDV) {
            vdp->vgp = MemNew(sizeof(SAM_ViewGlobal));
            MemCpy(vdp->vgp, vgp, sizeof(SAM_ViewGlobal));
            ViewMgr_SetRow(vgp->pCGlobal, vdp->userkey, vgp->Row);
        }
        else vdp->vgp = NULL;
    }
    else vdp->vgp = NULL;
	
	if (UDV_init_bsp_forViewer(vmp->vdp->UnDViewer,bsp,ompcp->input_entityID,
		ompcp->input_itemID,ompcp->input_itemtype,vdp)){
		UDV_set_MainMenus(&vmp->MainMenu,TRUE);
		UDV_set_MainControls(vmp,TRUE);
	}
	else{
		UDV_set_MainMenus(&vmp->MainMenu,FALSE);
		UDV_set_MainControls(vmp,FALSE);
		return OM_MSG_RET_ERROR;
	}

	/*Update dialog box title*/
	sprintf(szBuf,"%s - [%s - %d letters]",szAppName,
				vdp->bsp_i.bspAccNum,
				vdp->bsp_i.bspLength);
	SetTitle(w,szBuf);

	temport=SavePort((WindoW)vdp->UnDViewer);
	Select (vdp->UnDViewer);
	ObjectRect(vdp->UnDViewer,&rcP);
	InvalRect(&rcP);
	RestorePort(temport);

	omudp = ObjMgrAddUserData (ompcp->input_entityID, ompcp->proc->procid, 
			OMPROC_VIEW, vdp->userkey);
	if (omudp != NULL) {
		omudp->userdata.ptrvalue = (Pointer) vdp;
		omudp->messagefunc = UDV_OM_MsgFunc;
	}

	return(OM_MSG_RET_DONE);
}


/*******************************************************************************

  Function : UDV_WinMainProgQuit()
  
  Purpose : end of prog 
  
  Parameters : w; main window handle
  
  Return value : none 

*******************************************************************************/
NLM_EXTERN void UDV_WinMainProgQuit(WindoW w)
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
ViewerMainPtr 		vmp;
WindoW				hWinMain;

	hWinMain=(WindoW)ParentWindow(i);

	if (!hWinMain) return;
	
	vmp = (ViewerMainPtr) GetObjectExtra (hWinMain);

	if (vmp==NULL) return;
	
	Remove(hWinMain);
	if (vmp->AutonomeViewer) QuitProgram();
	
}


/*******************************************************************************

  Function : UDV_FreeVDPstruct()
  
  Purpose : free VDP struct before leaving viewer
  
  Parameters : vdp; data to delete
  
  Return value : NULL

*******************************************************************************/
NLM_EXTERN void * UDV_FreeVDPstruct(ViewerDialogDataPtr vdp,Boolean final)
{
ViewerMainPtr vmp;

	if (vdp){
		/*a call to close all the internal UDV viewers (Genome Viewer for example)*/
		/*standalone UDV : doesn't answer this message*/
		if (vdp->Parent){
			vmp = (ViewerMainPtr)GetObjectExtra(vdp->Parent);
			if (vmp && vmp->AutonomeViewer==TRUE) 
				ObjMgrSendMsg(OM_MSG_FLUSH, vdp->bsp_i.bsp_entityID, 
					vdp->bsp_i.bsp_itemID, OBJ_BIOSEQ);
		}	
	
		/*free some user data of Obj Mgr*/
		if (vdp->userkey>0) {
			if (vdp->bsp_i.bsp_entityID>0){
				ObjMgrFreeUserData(vdp->bsp_i.bsp_entityID,
						vdp->procid,vdp->proctype,vdp->userkey);
			}
		}

		UDV_FreeListParaG(&vdp->ParaG);
		vdp->procid=0;
		vdp->proctype=0;
		vdp->userkey=0;
		if (vdp->bsp_i.SeqBuf)
			MemFree(vdp->bsp_i.SeqBuf);
		memset(&vdp->bsp_i,0,sizeof(BspInfo));
		memset(&vdp->udv_graph.udv_vscrl,0,sizeof(UDVScrollData));
		vdp->udv_graph.udv_panel.nTotLines=0;
		if (vdp->udv_graph.udv_panel.region)
			ValNodeFreeData(vdp->udv_graph.udv_panel.region);
    ClearUDV_mouse_select(&(vdp->UDV_ms));
		vdp->UDV_ms.Action_type=MS_ACTION_FEAT_NOTHING;
		memset(&vdp->Item_select,(Uint2)-1,sizeof(UDV_Item_Select));
		memset(&vdp->Old_Item_select,(Uint2)-1,sizeof(UDV_Item_Select));
		if (!final) UnDViewerVScrlUpdate(vdp->UnDViewer,TRUE,0);
		vdp->ClickFeatFromDlg=FALSE;
		vdp->bDisplayRevComp=FALSE;
	}

	return(NULL);
}

/*******************************************************************************

  Function : UDV_WinMainCleanupExtraProc()
  
  Purpose : free memory before leaving viewer
  
  Parameters : vmp; main data
  
  Return value : none 

*******************************************************************************/
NLM_EXTERN void UDV_WinMainCleanupExtraProc (GraphiC g, VoidPtr data)
{
ViewerMainPtr vmp=(ViewerMainPtr)data;

	if (!vmp) return;
	/*if (vmp->the_set) SeqEntryFree(vmp->the_set);	*/
	if (vmp->BspTable) ValNodeFree(vmp->BspTable);
 	
	UDV_FreeVDPstruct(vmp->vdp,TRUE);

	/*done only for Autonomous viewer*/
	if (vmp->dataptr) ObjMgrFree(vmp->datatype,vmp->dataptr);

	if (vmp->vdp->udv_graph.pClr) MemFree(vmp->vdp->udv_graph.pClr);
    MemFree(vmp->vdp->vgp);
	MemFree(vmp->vdp);
	MemFree(vmp);
}


/*******************************************************************************

  Function : UDV_AboutProc()
  
  Purpose : about dialog box
  
  Parameters : -
  
  Return value : none 

*******************************************************************************/
static void  UDV_AboutProc(IteM i)
{
  MsgAlert(KEY_OK, SEV_INFO, "About OneD-Viewer",szAbout);

}

/*******************************************************************************

  Function : UDV_set_PullMenus()
  
  Purpose : manage main window pulldown menus 
  
  Parameters : 	mmp; menu data
  				enable; if TRUE, enable the command menus listed in mmp
  
  Return value : none 

*******************************************************************************/
NLM_EXTERN void  UDV_set_PullMenus(UDVMainMenuPtr mmp,Boolean enable)
{
	if (enable){
		Enable(mmp->FileOpen);
		Enable(mmp->EntrezOpen);
		Enable(mmp->FileClose);
		Enable(mmp->ListSequence);
		Enable(mmp->QuitViewer);
		Enable(mmp->ShowFeature);
		Enable(mmp->ShowFeatureList);
		Enable(mmp->SearchForFeature);
		Enable(mmp->ShowRevComp);
		Enable(mmp->RefreshScreen);
		Enable(mmp->HelpAbout);
	}
	else{
		Disable(mmp->FileOpen);
		Disable(mmp->EntrezOpen);
		Disable(mmp->FileClose);
		Disable(mmp->ListSequence);
		Disable(mmp->QuitViewer);
		Disable(mmp->ShowFeature);
		Disable(mmp->ShowFeatureList);
		Disable(mmp->SearchForFeature);
		Disable(mmp->ShowRevComp);
		Disable(mmp->RefreshScreen);
		Disable(mmp->HelpAbout);
	}
}


/*******************************************************************************

  Function : UDV_set_MainMenus()
  
  Purpose : manage main window command menus 
  
  Parameters : 	mmp; menu data
  				enable; if TRUE, enable the command menus listed in mmp
  
  Return value : none 

*******************************************************************************/
NLM_EXTERN void  UDV_set_MainMenus(UDVMainMenuPtr mmp,Boolean enable)
{

	SetStatus(mmp->ShowRevComp,FALSE);	
	if (enable){
		if (mmp->FileClose) Enable(mmp->FileClose);
		Enable(mmp->ShowFeature);
		Enable(mmp->ShowFeatureList);
		Enable(mmp->SearchForFeature);
		Enable(mmp->ScalePos);
		Enable(mmp->RefreshScreen);
		Enable(mmp->ShowRevComp);
		if (mmp->ListSequence) Enable(mmp->ListSequence);
	}
	else{
		if (mmp->FileClose) Disable(mmp->FileClose);
		Disable(mmp->ShowFeature);
		Disable(mmp->ShowFeatureList);
		Disable(mmp->SearchForFeature);
		Disable(mmp->ScalePos);
		Disable(mmp->RefreshScreen);
		Disable(mmp->ShowRevComp);
		if (mmp->ListSequence) Disable(mmp->ListSequence);
	}
}

/*******************************************************************************

  Function : UDV_set_MainControls()
  
  Purpose : manage main window controls (GoTO btn, etc)
  
  Return value : none 

*******************************************************************************/
NLM_EXTERN void UDV_set_MainControls(ViewerMainPtr vmp,Boolean enable)
{
	
	if (enable){
		Enable(vmp->gotoBtn);
		Enable(vmp->gotoVal);
	}
	else{
		Disable(vmp->gotoBtn);
		Disable(vmp->gotoVal);
	}
}

/*******************************************************************************

  Function : RefreshScreenProc()
  
  Purpose : redraw the screen 
  
  Parameters :  i; menu item
  
  Return value : none 

*******************************************************************************/
static void RefreshScreenProc(IteM i)
{
ViewerMainPtr 		vmp;
WindoW				hWinMain,temport;
ViewerDialogDataPtr vdp;
RecT				rcP;

	hWinMain=(WindoW)ParentWindow(i);

	if (!hWinMain) return;
	
	vmp = (ViewerMainPtr) GetObjectExtra (hWinMain);

	if (vmp==NULL) return;

	vdp = vmp->vdp;
	if (vdp == NULL) return;

	if (vdp->UnDViewer==NULL) return;	

	temport=SavePort((WindoW)vdp->UnDViewer);
	Select (vdp->UnDViewer);
	ObjectRect(vdp->UnDViewer,&rcP);
	InvalRect(&rcP);
	RestorePort(temport);
}

/*******************************************************************************

  Function : ScalePosChoiceProc()
  
  Purpose : modification of the scale position 
  
  Parameters :  i; menu item
  
  Return value : none 

*******************************************************************************/
static void ScalePosChoiceProc(ChoicE i)
{
Int2 				value;
ViewerMainPtr 		vmp;
WindoW				hWinMain;

ViewerDialogDataPtr vdp;

	hWinMain=(WindoW)ParentWindow(i);

	if (!hWinMain) return;
	
	vmp = (ViewerMainPtr) GetObjectExtra (hWinMain);

	if (vmp==NULL) return;

	vdp = vmp->vdp;
	if (vdp == NULL) return;
	
	value=GetValue(i);
	switch(value){
		case 1:/*Left only*/
			vdp->udv_graph.udv_scale.ScalePosition=SCALE_POS_LEFT;
			vdp->udv_graph.udv_scale.ShowMajorTick=FALSE;
					break;
		case 2:/*Top only*/
			vdp->udv_graph.udv_scale.ScalePosition=SCALE_POS_TOP;
			vdp->udv_graph.udv_scale.ShowMajorTick=TRUE;
			break;
		case 3:/*Top and left*/
			vdp->udv_graph.udv_scale.ScalePosition=SCALE_POS_BOTH;
			vdp->udv_graph.udv_scale.ShowMajorTick=TRUE;
			break;
	}

	PanelOrgChange(vdp->UnDViewer,vdp);
}

/*******************************************************************************

  Function : ShowFeatProc()
  
  Purpose : show/hide features 
  
  Parameters : i; menu item
  
  Return value : none 

*******************************************************************************/
static void ShowFeatProc(IteM i)
{
ViewerMainPtr 		vmp;
WindoW				hWinMain;
Boolean 			ShowFeatures;
ViewerDialogDataPtr vdp;

	hWinMain=(WindoW)ParentWindow(i);

	if (!hWinMain) return;
	
	vmp = (ViewerMainPtr) GetObjectExtra (hWinMain);

	if (vmp==NULL) return;

	vdp = vmp->vdp;
	if (vdp == NULL) return;

	ShowFeatures=GetStatus(i);	
	vdp->udv_graph.udv_panel.ShowFeatures=ShowFeatures;

	if (ShowFeatures==FALSE && vmp->hFeatDlg){
		Remove(vmp->hFeatDlg);
		vmp->hFeatDlg=NULL;
		SetStatus(vmp->MainMenu.ShowFeatureList,FALSE);
	}
	if (ShowFeatures==FALSE){
		Disable(vmp->MainMenu.ShowFeatureList);
		Disable(vmp->MainMenu.SearchForFeature);
	}
	else{
		Enable(vmp->MainMenu.ShowFeatureList);	
		Enable(vmp->MainMenu.SearchForFeature);	
	}
	
	PanelOrgChange(vdp->UnDViewer,vdp);
}

/*******************************************************************************

  Function : PanelOrgChange()
  
  Purpose : redraw window after some graph changes (show features, scale,...) 
  
  Parameters : w; handle of the viewer window
  
  Return value : none 

*******************************************************************************/
static void  PanelOrgChange(PaneL p,ViewerDialogDataPtr vdp)
{
RecT 				rcP;
BaR 				vsb;
ValNodePtr 			vnp;
ParaGPtr 			pgp;
Int4 				nLettre=-1;
Int4 				CurPos=0;
Boolean				ShowTop=FALSE,
					ShowTick=FALSE;
WindoW				temport;

	if (vdp->ParaG == NULL) return;
	
	temport=SavePort((WindoW)p);

	Select (p);
	ObjectRect(p,&rcP);

	/*current scroll status*/
	vsb = GetSlateVScrollBar ((SlatE) p);
	CurPos=GetBarValue(vsb);
	if (vdp->ParaG){
		for(vnp=vdp->ParaG ; vnp!=NULL ; vnp=vnp->next){
			if (vnp->data.ptrvalue){
				pgp=(ParaGPtr)vnp->data.ptrvalue;
				if (CurPos>=pgp->StartLine && CurPos<=(pgp->StartLine+
					pgp->nLines)){
					nLettre=pgp->StartLetter;
					break;
				}
			}
		}		
	}

	/*repos. ParaG - compute TotalLines*/
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

		
	UDV_ReLocateRcParaGList(/*vdp->udv_graph,*/rcP,
		ShowTop,ShowTick,TRUE,vdp->udv_graph.udv_panel.ShowFeatures,FALSE,
		&vdp->udv_graph.udv_panel.nTotLines,vdp->ParaG);

	/*compute new Vscroll pos */
	if (vdp->ParaG){
		for(vnp=vdp->ParaG ; vnp!=NULL ; vnp=vnp->next){
			if (vnp->data.ptrvalue){
				pgp=(ParaGPtr)vnp->data.ptrvalue;
				if (nLettre>=pgp->StartLetter && nLettre<=pgp->StopLetter){
					CurPos=pgp->StartLine;
					break;
				}
			}
		}		
	}

	/*update the Vscroll Bar*/
	UnDViewerVScrlUpdatePage(&(vdp->udv_graph.udv_vscrl.ScrollPage),
		vdp->udv_graph.udv_panel.cyClient,
		vdp->udv_graph.udv_font.LineHeight);
	UnDViewerVScrlUpdate(p,FALSE,CurPos);
	
	/*create new buffer*/
	UDV_create_buffer(&vdp->udv_graph,vdp->ParaG,&vdp->bsp_i,NULL,vdp->bDisplayRevComp);

	/*Redraw*/
	InvalRect(&rcP);
	RestorePort(temport);
}

/*******************************************************************************

  Function : UDV_SetupMenus()
  
  Purpose : create the menu of the main dialog box 
  
  Parameters : w; handle of the dialog box
  
  Return value : none 

*******************************************************************************/
NLM_EXTERN void  UDV_SetupMenus(WindoW w,Boolean isEntrezOk,
                                SAM_ViewGlobal *vgp)
{
MenU			m,s;	/*temp variable*/
ChoicE			c;	/* " */
ViewerMainPtr 	vmp;/*program data*/
UdvGlobalsPtr   ugp;

	vmp = (ViewerMainPtr) GetObjectExtra (w);
	ugp=(UdvGlobalsPtr)GetAppProperty("UdvGlobals");

	/*File menu*/
	m=PulldownMenu(w,"File");
	vmp->MainMenu.File=m;

	if (vmp->AutonomeViewer){/*available only for the Auntonomous viewer*/
		s=SubMenu(m,"Open from ");
		vmp->MainMenu.FileOpen=CommandItem(s,"a local file...",
				UDV_FileOpen);
		vmp->MainMenu.EntrezOpen=CommandItem(s,"the network...",
				UDV_NetOpen);
		if (isEntrezOk==FALSE) Disable(vmp->MainMenu.EntrezOpen);

		vmp->MainMenu.FileClose=CommandItem(m,"Close",UDV_FileClose);
		SeparatorItem(m);
	}
	vmp->MainMenu.ListSequence=
			CommandItem(m,"Sequence list...",UDV_CreateListBioseqDlg);
    SeparatorItem(m);
	vmp->MainMenu.QuitViewer=CommandItem(m,"Quit/Q",QuitProc);

    if(vgp != NULL) {
        if(vgp->MasterViewer == SAMVIEWCN3D) {
            vmp->MainMenu.Align = PulldownMenu(w, "Align/A");
            vmp->MainMenu.AddRow = SubMenu(vmp->MainMenu.Align, "Add New Row");
                vmp->MainMenu.FromFile = SubMenu(vmp->MainMenu.AddRow, "From FASTA file");
                CommandItem(vmp->MainMenu.FromFile,
                    "By gapped BLAST...", (Nlm_ItmActnProc)vgp->BlastFileGap);
                CommandItem(vmp->MainMenu.FromFile,
                    "By ungapped BLAST...", (Nlm_ItmActnProc)vgp->BlastFile);
            if (vgp->NetStartProc) {
                vmp->MainMenu.FromNet = SubMenu(vmp->MainMenu.AddRow, "Download from Entrez");
                CommandItem(vmp->MainMenu.FromNet,
                    "By gapped BLAST...",  (Nlm_ItmActnProc)vgp->BlastNetGap);
                CommandItem(vmp->MainMenu.FromNet,
                    "By ungapped BLAST...",  (Nlm_ItmActnProc)vgp->BlastNet);
/*                vmp->MainMenu.BlastMany = CommandItem(vmp->MainMenu.Align, 
                    "Blast...", (Nlm_ItmActnProc)vgp->BlastMany); */
            }
        }
    }


	/*Options menu*/
	m=PulldownMenu(w,"Options");
	vmp->MainMenu.Options=m;
	vmp->MainMenu.ShowFeature=StatusItem(m,"Show features",
			ShowFeatProc);
	SetStatus(vmp->MainMenu.ShowFeature,TRUE);
    if(vgp == NULL) {
        vmp->MainMenu.ShowFeatureList=StatusItem(m,"Features List...",
            ShowFeaturesListDlg);
        SetStatus(vmp->MainMenu.ShowFeatureList,FALSE);
        vmp->MainMenu.SearchForFeature=CommandItem(m,"Search for features...",
            UDV_SearchFeatForKey);
        SeparatorItem(m);
    }
    
    if(vgp == NULL) {
		vmp->MainMenu.ShowRevComp=StatusItem(m,"Show reverse complement",
			UDV_ShowReverseComplement);
		SeparatorItem(m);
	}
	vmp->MainMenu.ScalePos=SubMenu(m,"Scale position");

	c=ChoiceGroup(vmp->MainMenu.ScalePos,ScalePosChoiceProc);
	ChoiceItem(c,"Left only");
	ChoiceItem(c,"Top only");
	ChoiceItem(c,"Top and left");
	SetValue(c,3);
	vmp->MainMenu.ScalePosChoice=c;
	SeparatorItem(m);
	vmp->MainMenu.RefreshScreen=CommandItem(m,"Refresh screen",
			RefreshScreenProc);
	if (vmp->AutonomeViewer && ugp!=NULL && ugp->NetCfgMenuProc!=NULL){/*available only for the Auntonomous viewer*/
		vmp->MainMenu.ConfigNet=CommandItem(m,"Network...",ugp->NetCfgMenuProc);
	}	
#ifdef UDV_GENOME
	CommandItem(m,"Overview...",UDV_GenViewProc);
#endif
	/*Help menu*/
	m=PulldownMenu(w,"Help");
	vmp->MainMenu.Help=m;
	vmp->MainMenu.HelpAbout=CommandItem(m,"About...",UDV_AboutProc);
}

/*******************************************************************************

  Function : WinMainMsgProc()
  
  Purpose : Main dialog box answering loop 
  
  Parameters : d; handle of the dialog box
  				mssg; message
  
  Return value : none 

*******************************************************************************/
/*static void WinMainMsgProc (DialoG d, Int2 mssg)

{
ViewerDialogDataPtr vdp;
ViewerMainPtr 		vmp;

	vmp = (ViewerMainPtr) GetObjectExtra (d);
	if (vmp==NULL) return;
	
	vdp=vmp->vdp;

	if (vdp != NULL) {
		switch (mssg) {
			case VIB_MSG_INIT :
				break;
			case VIB_MSG_CLOSE :
				Remove (d);
				break;
			default :
				break;
		}
	}
}
*/
/*******************************************************************************

  Function : UDV_Resize_Logo_Panel()
  
  Purpose : resize logo panel 
  
  Parameters : p; handle of the main window
  				rcP; new rc of the logo panel
  
  Return value : none (see rcP)

*******************************************************************************/
NLM_EXTERN void  UDV_Resize_Logo_Panel (WindoW Parent,RecT PNTR rcL)
{
RecT rcDlg;

	/*size of the parent Window of the Panel*/
	ObjectRect(Parent,&rcDlg);

	/*new size of the Logo Panel*/
	LoadRect (rcL, (Int2)4*VIEWER_HORZ_MARGIN, (Int2)4*VIEWER_VERT_MARGIN, 
					(Int2)(rcDlg.right - rcDlg.left - 4*VIEWER_HORZ_MARGIN), 
					(Int2)(rcDlg.bottom - rcDlg.top - 4*VIEWER_VERT_MARGIN));
}

/*******************************************************************************

  Function : UDV_VSscroll()
  
  Purpose : UnD Viewer Vertical Scroll Bar Callback 
  
  Parameters : p; viewer
  				vdp; viewer data structure
				newval; new value of the thumb
				oldval; old value of the thumb
  
  Note : this function MUST to be used by external software using the viewer.
  
  Return value : none 

*******************************************************************************/
static void UDV_VScroll(PaneL p,ViewerDialogDataPtr vdp, Int4 newval, 
			Int4 oldval)
{
RecT 				rcP;
WindoW 				temport;
Int4 				n;
Boolean 			IsGoUp=FALSE;

	if (!vdp) return;
	
	vdp->udv_graph.udv_vscrl.ScrollPos=newval;
	
	temport=SavePort(ParentWindow((WindoW)p));
	Select(p);
	ObjectRect(p,&rcP);
	InsetRect(&rcP,4,4);
	ClipRect(&rcP);

	if (oldval>newval){
		n=oldval-newval;
		IsGoUp=TRUE;
	}
	else {
		n=newval-oldval;
		IsGoUp=FALSE;
	}
	
	if (n<vdp->udv_graph.udv_vscrl.ScrollPage){/*Line UP/Down*/
		/*Create a new Buffer ?*/
		UDV_analyse_buffer(&vdp->udv_graph,vdp->ParaG,&vdp->bsp_i,
			(Uint2)(IsGoUp==TRUE ? BUFFER_REPOP_VCRL_LUP : 
				BUFFER_REPOP_VCRL_LDN),vdp->bDisplayRevComp);	
		
		/*move and redraw*/
		ScrollRect (&rcP, 0, (Int2)((oldval-newval)*
				vdp->udv_graph.udv_font.LineHeight));
	}
	else{/*Page Up/Down*/
		/*Create a new Buffer ?*/
		UDV_analyse_buffer(&vdp->udv_graph,vdp->ParaG,&vdp->bsp_i,
			(Uint2)(IsGoUp==TRUE ? BUFFER_REPOP_VCRL_PUP : 
				BUFFER_REPOP_VCRL_PDN),vdp->bDisplayRevComp);	

		/*redraw*/
		/*rcP.left=0;
		rcP.top=0;*/
		InvalRect(&rcP);
	}
	ResetClip();
	RestorePort(temport);
	Update ();
}

/*******************************************************************************

  Function : UnDViewerVScrlProc()
  
  Purpose : UnD Viewer Vertical Scroll Bar Callback 
  
  Parameters : sb; handle of the scroll
  				s; pane;
				newval; new value of the thumb
				oldval; old value of the thumb
  
  Return value : none 

*******************************************************************************/
NLM_EXTERN void UnDViewerVScrlProc (BaR sb, SlatE s, Int4 newval, 
			Int4 oldval)
{
WindoW 				w;
ViewerDialogDataPtr vdp;
ViewerMainPtr 		vmp;	

	/*get the parent Window of the Panel*/
	/*
	w=ParentWindow(s);

	if (w==NULL) return;	
	*/
	
	/*get some usefull data...*/
	/*
	vmp = (ViewerMainPtr) GetObjectExtra (w);
	if (vmp==NULL) return;
	
	vdp=vmp->vdp;
	*/
	vdp = (ViewerDialogDataPtr) GetObjectExtra (s);

	UDV_VScroll((PaneL)s,vdp,newval,oldval);
}

/*******************************************************************************

  Function : UnDViewerVScrlUpdatePage()
  
  Purpose : compute PageUp & Down (VScroll) 
  
  Parameters : PageUpDn; initialize here
  				cyClient; height of UnDviewer panel
				LineHeight; height of one data line
  
  Return value : none 

*******************************************************************************/
static void  UnDViewerVScrlUpdatePage(Int4Ptr PageUpDn,Int2 cyClient,
				Int2 LineHeight)
{
	*PageUpDn=(Int4)(cyClient/LineHeight);
}

/*******************************************************************************

  Function : UnDViewerVScrlUpdate()
  
  Purpose : update the UnDviewer VScroll bar 
  
  Parameters : p; handel of the UnDviewer panel
  				bInit; TRUE only when the UnDviewer is created (soft. start up)
  				CurPos ; new position
  
  Return value : none 

*******************************************************************************/
NLM_EXTERN void  UnDViewerVScrlUpdate(PaneL p,Boolean bInit,Int4 CurPos)
{
WindoW 				hWnd;
BaR 				vsb;
ViewerDialogDataPtr vdp;
ViewerMainPtr 		vmp;

	/*get some usefull data...*/
	vdp = (ViewerDialogDataPtr) GetObjectExtra (p);
	if (vdp == NULL) return;

	/*current scroll status*/
	vsb = GetSlateVScrollBar ((SlatE) p);
	
	if (bInit) CurPos=0;
	
	/*given curpos (lineNumber) retrieve letter number*/
	
	/*compute new values*/	
	UnDViewerVScrlUpdatePage(&(vdp->udv_graph.udv_vscrl.ScrollPage),
				vdp->udv_graph.udv_panel.cyClient,
				vdp->udv_graph.udv_font.LineHeight);
	if (vdp->udv_graph.udv_panel.nTotLines>vdp->udv_graph.udv_vscrl.ScrollPage){
		vdp->udv_graph.udv_vscrl.ScrollMax=vdp->udv_graph.udv_panel.nTotLines-
							vdp->udv_graph.udv_vscrl.ScrollPage;
	}
	else{
		vdp->udv_graph.udv_vscrl.ScrollMax=0;
		vdp->udv_graph.udv_vscrl.ScrollPos=0;
		vdp->udv_graph.udv_vscrl.ScrollPage=0;
	}
	
	if (CurPos<0) CurPos=0;

	if (CurPos>=vdp->udv_graph.udv_vscrl.ScrollMax)
		vdp->udv_graph.udv_vscrl.ScrollPos=vdp->udv_graph.udv_vscrl.ScrollMax;
	else vdp->udv_graph.udv_vscrl.ScrollPos=CurPos;
				
	/*update scroll*/
	CorrectBarMax(vsb,vdp->udv_graph.udv_vscrl.ScrollMax);
	CorrectBarValue(vsb,vdp->udv_graph.udv_vscrl.ScrollPos);
	CorrectBarPage(vsb,vdp->udv_graph.udv_vscrl.ScrollPage,
			vdp->udv_graph.udv_vscrl.ScrollPage);
	
}

/*******************************************************************************

  Function : UDV_resize_viewer()
  
  Purpose : viewer Resize function 
  
  Parameters : p; handle of the viewer panel
  				vdp; viewer data structure
  
  Return value : none 

*******************************************************************************/
NLM_EXTERN void	UDV_resize_viewer(PaneL p,ViewerDialogDataPtr vdp)
{
BaR					vsb;
ValNodePtr			vnp;
ParaGPtr			pgp;
Int4				nLettre=-1;
Int4				CurPos=0;
Boolean				ShowTop=TRUE;
Boolean				ShowTick=TRUE;
RecT 				rcP;
WindoW				temport;

	if (vdp->ParaG == NULL) return;
	
	ObjectRect(p,&rcP);
	temport=SavePort(ParentWindow(p));
	Select(p);
	
	/*Update UnDviewer's panel size values*/
	UDV_ComputePanelSize(rcP,&(vdp->udv_graph.udv_panel.cxClient),
			&(vdp->udv_graph.udv_panel.cyClient));
	
	/*block lines*/
	UDV_ComputeBlockByLine(vdp->udv_graph.udv_panel.cxClient,
			vdp->udv_graph.udv_panel.cxName,
			vdp->udv_graph.udv_scale.cxLeftScale,
			vdp->udv_graph.udv_font.cxChar,
			&vdp->udv_graph.udv_panel.nCharByLine,
			&vdp->udv_graph.udv_panel.nBlockByLine);	

	/*current scroll status*/
	vsb = GetSlateVScrollBar ((SlatE) p);
	CurPos=GetBarValue(vsb);
	if (vdp->ParaG){
		for(vnp=vdp->ParaG ; vnp!=NULL ; vnp=vnp->next){
			if (vnp->data.ptrvalue){
				pgp=(ParaGPtr)vnp->data.ptrvalue;
				if (CurPos>=pgp->StartLine && CurPos<=(pgp->StartLine+
					pgp->nLines)){
					nLettre=pgp->StartLetter;
					break;
				}
			}
		}		
	}	
	
	/*securite*/
	if (vdp->udv_graph.udv_panel.nBlockByLine<1){
		/*WindoW temport = SavePort(ParentWindow(vdp->UnDViewer));
		Select (vdp->UnDViewer);*/
		InvalRect(&rcP);
		RestorePort (temport);
		Update();
		return;
	}

	/*repos. ParaG - compute TotalLines*/
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

	/*total lines in the viewer*/
	vdp->ParaG=UDV_CreateParaGList(vdp->udv_graph.udv_panel.nCharByLine,
				vdp->bsp_i.bspLength,0,vdp->bsp_i.bspLength-1,
				ShowTop,ShowTick,TRUE,FALSE,&vdp->udv_graph.udv_panel.nTotLines,
				vdp->ParaG);

	if (vdp->ParaG) UDV_PopulateParaGFeatures(/*vdp->udv_graph,*/vdp->bsp_i.bsp,
			vdp->ParaG,vdp->udv_graph.udv_panel.ShowFeatures,
			/*rcP,*/&vdp->udv_graph.udv_panel.nTotLines,
			vdp->udv_graph.DisplayOptions,NULL);

	/*compute new Vscroll pos */
	if (vdp->ParaG){
		for(vnp=vdp->ParaG ; vnp!=NULL ; vnp=vnp->next){
			if (vnp->data.ptrvalue){
				pgp=(ParaGPtr)vnp->data.ptrvalue;
				if (nLettre>=pgp->StartLetter && nLettre<=pgp->StopLetter){
					CurPos=pgp->StartLine;
					break;
				}
			}
		}		
	}

	/*update the Vscroll Bar*/
	UnDViewerVScrlUpdate(p,FALSE,CurPos);
	
	/*create new buffer*/
	UDV_create_buffer(&vdp->udv_graph,vdp->ParaG,&vdp->bsp_i,NULL,vdp->bDisplayRevComp);
	InvalRect(&rcP);
	RestorePort(temport);
	Update();
}


/*******************************************************************************

  Function : UDV_WinMainResize()
  
  Purpose : Main Dialog Box Resize function 
  
  Parameters : w; handle of the dialog box
  
  Return value : none 

*******************************************************************************/
NLM_EXTERN void  UDV_WinMainResize (WindoW w)
{
ViewerDialogDataPtr vdp;
ViewerMainPtr 		vmp;
RecT				rcP,rcI,rcW;
Int2                height,diff,gap,width,delta;
	
	vmp = (ViewerMainPtr) GetObjectExtra (w);
	if (vmp==NULL) return;

	vdp=vmp->vdp;

	if (vdp == NULL) return;

	/*viewer not initialized; typically at the startup of the soft
	 just draw the logo*/
	if (vdp->UnDViewer == NULL) return;

	/*move UDV_Panel & InfoPanel*/
	ObjectRect(w,&rcW);
	width= rcW.right-rcW.left;
	height= rcW.bottom-rcW.top;

	GetPosition(vmp->StatusGroup,&rcI);
	GetPosition(vdp->UnDViewer,&rcP);
	diff=rcI.bottom-rcI.top;
	gap=rcI.top-rcP.bottom;
    rcI.bottom = height - rcP.left;
    rcI.top = rcI.bottom - diff;
    rcI.right = width - rcI.left;
    rcP.right = width - rcP.left;

    rcP.bottom = rcI.top - gap;
    SetPosition (vmp->StatusGroup, &rcI);
    SetPosition (vdp->InfoPanel, &rcI);
    SetPosition (vdp->UnDViewer, &rcP);
    AdjustPrnt (vdp->UnDViewer, &rcP, FALSE);

	Update();
	
	if (vdp->ParaG == NULL) return;

	UDV_resize_viewer(vdp->UnDViewer,vdp);
}

/*******************************************************************************

  Function : UDV_init_bsp_forViewer()
  
  Purpose : initialize a bsp to display it in the viewer 
  
  Parameters : 	w ; dialog box (parent window of the viewer)
  				bsp; the bsp
				eID, iID, itype; identification of the BSP
				vdp; general data to store ParaG
  
  Return value : none 

*******************************************************************************/
NLM_EXTERN Boolean  UDV_init_bsp_forViewer(PaneL p,BioseqPtr bsp,Uint2 eID,
	Uint2 iID,Uint2 itype,ViewerDialogDataPtr vdp)
{
Boolean		ShowTop=TRUE;
Boolean		ShowTick=TRUE;
Boolean		nRet=FALSE;
RecT 		rcP;
WindoW		temport;

	/*size of UDV_VIEWER*/
	temport=SavePort(ParentWindow(p));
	Select(p);
	ObjectRect(p,&rcP);

	/*store BSP info*/
	vdp->bsp_i.bsp=bsp;
	UDV_ReadBspDataForViewer(&vdp->bsp_i);
	vdp->bsp_i.bsp_entityID=eID;
	vdp->bsp_i.bsp_itemID=iID;
	vdp->bsp_i.bsp_itemType=itype;

	WatchCursor();
	/*
	mon = MonitorStrNewEx (szAppName, 30, FALSE);
	MonitorStrValue (mon, "Preparing display...");
	*/
	Update ();

	/*Populate ParaG - compute TotalLines*/
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

	vdp->ParaG=UDV_CreateParaGList(vdp->udv_graph.udv_panel.nCharByLine,
		vdp->bsp_i.bspLength,0,vdp->bsp_i.bspLength-1,
		ShowTop,ShowTick,TRUE,FALSE,&vdp->udv_graph.udv_panel.nTotLines,
		vdp->ParaG);

	if (vdp->ParaG) {
		UDV_PopulateParaGFeatures(vdp->bsp_i.bsp,vdp->ParaG,
			vdp->udv_graph.udv_panel.ShowFeatures,
			&vdp->udv_graph.udv_panel.nTotLines,vdp->udv_graph.DisplayOptions,
			NULL);
	}
	else {
		UDV_Init_vdp_struct(p,vdp,FALSE,TRUE,TRUE);
		nRet=FALSE;
		goto fin;
	}

	/*update scroll*/
	UnDViewerVScrlUpdatePage(&(vdp->udv_graph.udv_vscrl.ScrollPage),
		vdp->udv_graph.udv_panel.cyClient,
		vdp->udv_graph.udv_font.LineHeight);
	UnDViewerVScrlUpdate(p,TRUE,0);

	/*create sequence buffer*/
	UDV_create_buffer(&vdp->udv_graph,vdp->ParaG,&vdp->bsp_i,NULL,vdp->bDisplayRevComp);
	
	/*redraw the viewer*/
	rcP.top=0;rcP.left=0;
	InvalRect(&rcP);
	Update();
	nRet=TRUE;
	
fin:
	RestorePort(temport);
	ArrowCursor();
	/*
	MonitorFree (mon);
	*/
	return(nRet);
}

/*******************************************************************************

  Function : UDV_init_NonAutonomous()
  
  Purpose : viewer panel creation for external software using the viewer
  
  Parameters : 	vdp; viewer data block - allocated & initialized here
				p; viewer panel - MUST be not NULL
				f; font - MUST be not NULL
				
  Note : 1. the external software MUST initialize p and f before entering this
	  function
		 2. the PaneL p MUST have a correct size : UDV_Init_GraphData() computes
	  cxClient and cyClient used by the viewer to populate ParaG data blocks.
		 		  
  Return value : FALSE if failure 

*******************************************************************************/
NLM_EXTERN Boolean UDV_Init_NonAutonomous(PaneL p,ViewerDialogDataPtr PNTR vdp,
					FonT f)
{
	/*init data structure*/
	(*vdp)=(ViewerDialogDataPtr)MemNew(sizeof(ViewerDialogData));
	if ((*vdp)){
		MemSet((*vdp),0,sizeof(ViewerDialogData));
	}
	else{
		Message (MSG_ERROR, "Viewer creation failed.");
		return(FALSE);
	}

	(*vdp)->udv_graph.udv_font.hFnt=f;

	/*init font size*/
	Select(p);
	SelectFont((*vdp)->udv_graph.udv_font.hFnt);
	UDV_FontDim(&((*vdp)->udv_graph.udv_font.cxChar),
			&((*vdp)->udv_graph.udv_font.cyChar));

	/*compute Feature Color Palette*/
	(*vdp)->udv_graph.pClr=UDV_BuildFeatColorTable();

	/*compute layout letters for NA and AA*/
	UDV_Build_NA_LayoutPalette(&(*vdp)->udv_graph);
	UDV_Build_AA_LayoutPalette(&(*vdp)->udv_graph);			

	(*vdp)->udv_graph.udv_font.LineHeight=
				UDV_ComputeLineHeight((*vdp)->udv_graph.udv_font.cyChar);
	
	/*general graph data init; scale pos, etc.*/
	UDV_Init_GraphData((*vdp)->UnDViewer,&(*vdp)->udv_graph);
		
	return(TRUE);
}

/*******************************************************************************

  Function : UDV_GotoBtnProc()
  
  Purpose : manage the "Go To" button
  
*******************************************************************************/
static void UDV_GotoBtnProc(ButtoN g)
{
WindoW        w;
ViewerMainPtr vmp;
CharPtr       szTmp;
ParaGPtr      pgp;
ValNodePtr    vnp,vnp_old;
Int4          iValue,old_ivalue,line_num,old_line_num,from_bsp,to_bsp,
			  from_line,to_line;
BaR           vsb;
Char          szValue[20];
Char          szRange[100];

	w=ParentWindow(g);
	
	vmp=(ViewerMainPtr)GetObjectExtra(w);
	
	if (vmp->vdp->ParaG==NULL)
		return;

	GetTitle(vmp->gotoVal, szValue, 19);
	if (StringHasNoText (szValue)) {
		Message (MSG_OK, "Please enter a position");
		Select (vmp->gotoVal);
		return;
	}
	
	szTmp=szValue;
	while(*szTmp){
		if (!IS_DIGIT(*szTmp)){
			Message (MSG_OK, "The value you entered is wrong.");
			Select (vmp->gotoVal);
			return;
		}
		szTmp++;
	}

	iValue=atoi(szValue);

	if (iValue>0 && iValue<=vmp->vdp->bsp_i.bspLength){
		vnp=UDV_GetLineNumGivenPos(vmp->vdp->ParaG,iValue-1,&line_num);
		if (line_num==(Int4)-1) return;
		UDV_GetCurrentDispRange(vmp->vdp,&from_bsp,&to_bsp,&from_line,&to_line);
		old_ivalue=vmp->vdp->udv_graph.GotoLetter;
		vmp->vdp->udv_graph.GotoLetter=iValue;
		if (line_num<from_line || line_num>to_line){
			vsb = GetSlateVScrollBar ((SlatE) vmp->vdp->UnDViewer);
			SetValue(vsb,line_num);
		}
		vmp->vdp->udt.action=UDV_INVAL_REGION;
		vmp->vdp->udt.status=UDV_SET_TIMER;
		vmp->vdp->udt.pos=iValue-1;

	}
	else{
		sprintf(szRange,"Please enter a value in the range\n[1..%i].",
			vmp->vdp->bsp_i.bspLength);
		Select (vmp->gotoVal);
		Message (MSG_OK, szRange);
	}
}

NLM_EXTERN void UDVResetProc (PaneL p)

{
  ViewerDialogDataPtr  vdp;

  vdp = (ViewerDialogDataPtr) GetObjectExtra (p);
  if (vdp == NULL) return;
  UDV_FreeVDPstruct (vdp, TRUE);
}

/*******************************************************************************

  Function : CreateMainControls()
  
  Purpose : create the content of the main window
  
  Parameters : w; parent main window
				vmp; main data structure
				  
  Return value : FALSE if failure 

*******************************************************************************/
NLM_EXTERN Boolean CreateMainControls(WindoW w,ViewerMainPtr vmp,
	SAM_ViewGlobal *vgp)
{
GrouP  g,StatusGroup;
ButtoN gotoBtn;
TexT   gotoTxt;
PrompT info;
FonT   hFnt;
Int2   cxChar,cyChar,Margins;
PaneL  pnl;

	vmp->vdp=(ViewerDialogDataPtr)MemNew(sizeof(ViewerDialogData));
	if (vmp->vdp==NULL) 
		return(FALSE);

	/*FonT & FonT size*/
	if (!UDV_InitFont(&(vmp->vdp->udv_graph.udv_font))) 
		return(FALSE);

	SelectFont(vmp->vdp->udv_graph.udv_font.hFnt);
	UDV_FontDim(&(vmp->vdp->udv_graph.udv_font.cxChar),
			&(vmp->vdp->udv_graph.udv_font.cyChar));

	/*compute Feature Color Palette*/
	vmp->vdp->udv_graph.pClr=UDV_BuildFeatColorTable();
	UDV_Build_Other_Colors(&(vmp->vdp->udv_graph));
	
	/*compute layout letters for NA and AA*/
	UDV_Build_NA_LayoutPalette(&(vmp->vdp->udv_graph));
	UDV_Build_AA_LayoutPalette(&(vmp->vdp->udv_graph));			

	vmp->vdp->udv_graph.udv_font.LineHeight=
				UDV_ComputeLineHeight(vmp->vdp->udv_graph.udv_font.cyChar);

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
	g = HiddenGroup (w, 3, 0, NULL);                  
	gotoBtn = PushButton (g, "Go to:", UDV_GotoBtnProc);
	gotoTxt = DialogText (g, "0", (Int2)6, NULL);

	g = HiddenGroup (w, 1, 0, NULL);
	
	Margins=20*stdCharWidth;
    
    if(vgp != NULL) {
        if(vgp->MasterViewer == SAMVIEWCN3D) {
			pnl = AutonomousPanel4 (g, 
				vgp->Rect.right-vgp->Rect.left, 
				vgp->Rect.bottom-vgp->Rect.top, 
				 UDV_draw_viewer,UnDViewerVScrlProc,NULL,
				 sizeof(ViewerDialogDataPtr),NULL,NULL);
		}
        else {
            pnl = AutonomousPanel4 (g, 
                (Int2)2*((screenRect.right-screenRect.left)-Margins)/3, 
                (Int2)2*((screenRect.bottom-screenRect.top)-Margins)/3, 
                UDV_draw_viewer,UnDViewerVScrlProc,NULL,
                sizeof(ViewerDialogDataPtr),NULL,NULL);
        }
    }
    else{
		pnl = AutonomousPanel4 (g, 
			(Int2)2*((screenRect.right-screenRect.left)-Margins)/3, 
			(Int2)2*((screenRect.bottom-screenRect.top)-Margins)/3, 
			 UDV_draw_viewer,UnDViewerVScrlProc,NULL,
			 sizeof(ViewerDialogDataPtr),NULL,NULL);
	}
	SetObjectExtra (pnl, (Pointer) vmp->vdp, NULL);
	StatusGroup = HiddenGroup (w, 1, 0, NULL);
	info = StaticPrompt (StatusGroup, "Ready !", 0, 3*cyChar/2, hFnt, 'l');

	if (pnl==NULL || gotoBtn==NULL || gotoTxt==NULL || info==NULL) 
		return(FALSE);

	SetPanelClick(pnl,UDV_ClickProc,
		UDV_DragProc,UDV_HoldProc,UDV_ReleaseProc);
	vmp->vdp->UDV_ms.Action_type=MS_ACTION_FEAT_NOTHING;

	SetWindowTimer(w,UDV_TimerProc);

	AlignObjects(ALIGN_RIGHT, (HANDLE) pnl, (HANDLE) StatusGroup, NULL);

	vmp->gotoBtn=gotoBtn;
	vmp->gotoVal=gotoTxt;
	vmp->StatusGroup=StatusGroup;
	vmp->vdp->InfoPanel=info;
	vmp->vdp->UnDViewer=pnl;
	vmp->vdp->Parent=w;

	return(TRUE);
}

/*******************************************************************************

  Function : UDV_InitForSequin()
  
  Purpose : initialize udv data structure to start UDV from Sequin
  
  Note : before entering this function, udvPanel must have a correct size
      (this is because this function calls UDV_Init_GraphData() ).
	  
  Return value : FALSE if failure 

*******************************************************************************/
NLM_EXTERN Boolean UDV_InitForSequin(PaneL udvPanel, BioseqViewPtr bvp)
{
ViewerDialogDataPtr vdp;
Uint2 eID, iID;

	vdp=(ViewerDialogDataPtr)MemNew(sizeof(ViewerDialogData));
	if (vdp==NULL) 
		return(FALSE);

	vdp->bvp = bvp;

	/*FonT & FonT size*/
	if (bvp != NULL && bvp->displayFont!=NULL){
		vdp->udv_graph.udv_font.hFnt = bvp->displayFont;
	}
	else{
		if (!UDV_InitFont(&(vdp->udv_graph.udv_font))) 
			return(FALSE);
	}

	SelectFont(vdp->udv_graph.udv_font.hFnt);
	UDV_FontDim(&(vdp->udv_graph.udv_font.cxChar),
			&(vdp->udv_graph.udv_font.cyChar));

	/*compute Feature Color Palette*/
	vdp->udv_graph.pClr=UDV_BuildFeatColorTable();
	UDV_Build_Other_Colors(&(vdp->udv_graph));
	
	/*compute layout letters for NA and AA*/
	UDV_Build_NA_LayoutPalette(&(vdp->udv_graph));
	UDV_Build_AA_LayoutPalette(&(vdp->udv_graph));			

	vdp->udv_graph.udv_font.LineHeight=
				UDV_ComputeLineHeight(vdp->udv_graph.udv_font.cyChar);

	/*attach data to udvPanel*/
	SetObjectExtra (udvPanel, (Pointer) vdp, NULL);

	/*set up the mouse callbacks*/
	SetPanelClick(udvPanel,UDV_ClickProc,
		UDV_DragProc,UDV_HoldProc,UDV_ReleaseProc);

	/*save udvPanel handle*/
	vdp->UnDViewer=udvPanel;

	/*general graph data init; scale pos, etc.*/
	UDV_Init_GraphData(vdp->UnDViewer,&vdp->udv_graph);

	/*build the display*/
	eID = ObjMgrGetEntityIDForPointer (bvp->bsp);
	iID = GetItemIDGivenPointer (eID, OBJ_BIOSEQ, bvp->bsp);
	UDV_init_bsp_forViewer (bvp->udv, bvp->bsp, eID, iID, OBJ_BIOSEQ, vdp);

	return(TRUE);
}

static void InitSelectUDV (ViewerDialogDataPtr vdp)

{
  Uint2         entityID;
  SelStructPtr  sel;

  if (vdp == NULL) return;
  entityID = vdp->bsp_i.bsp_entityID;
  for (sel = ObjMgrGetSelected (); sel != NULL; sel = sel->next) {
    if (entityID == sel->entityID && sel->itemtype == OBJ_SEQFEAT) {
      UDV_select_feature (vdp->UnDViewer, vdp, entityID,
					      sel->itemID, TRUE);
    }
  }
}

static void PopulateUDV (BioseqViewPtr bvp)

{
  ViewerDialogDataPtr  vdp;

  if (bvp == NULL) return;
  UDV_InitForSequin (bvp->udv, bvp);
  vdp = (ViewerDialogDataPtr) GetObjectExtra (bvp->udv);
  InitSelectUDV (vdp);
}

static void ShowUDV (BioseqViewPtr bvp, Boolean show)

{
  if (bvp == NULL) return;
  if (show) {
    SafeShow (bvp->udv);
    SafeShow (bvp->udvParentGrp);
    EnableDisableLegendItem (bvp, FALSE);
    SafeHide (bvp->findGeneGrp);
    SafeShow (bvp->clickMe);
  } else {
    SafeHide (bvp->udvParentGrp);
    SafeHide (bvp->udv);
    Reset (bvp->udv);
    SafeHide (bvp->styleControlGrp);
    SafeHide (bvp->scaleControlGrp);
    EnableDisableLegendItem (bvp, FALSE);
    SafeHide (bvp->findGeneGrp);
    SafeHide (bvp->docTxtControlGrp);
    SafeHide (bvp->modeControlGrp);
    SafeHide (bvp->clickMe);
  }
}

static void SelectUDV (BioseqViewPtr bvp, Uint2 selentityID, Uint2 selitemID,
                       Uint2 selitemtype, SeqLocPtr region,
                       Boolean select, Boolean scrollto)

{
  OMMsgStruct     omms;
  OMMsgStructPtr  ommsp;
  OMUserData      omud;
  ViewerDialogDataPtr  vdp;

  MemSet ((Pointer) &omms, 0, sizeof (OMMsgStruct));
  MemSet ((Pointer) &omud, 0, sizeof (OMUserData));
  ommsp = &omms;
  if (select) {
    ommsp->message = OM_MSG_SELECT;
  } else {
    ommsp->message = OM_MSG_DESELECT;
  }
  ommsp->entityID = selentityID;
  ommsp->itemID = selitemID;
  ommsp->itemtype = selitemtype;
  ommsp->region = (Pointer) region;
  ommsp->omuserdata = &omud;
  vdp = (ViewerDialogDataPtr) GetObjectExtra (bvp->udv);
  omud.userdata.ptrvalue = (Pointer) vdp;
  UDV_OM_MsgFunc (ommsp);
}

static void ResizeUDV (BioseqViewPtr bvp)

{
  ViewerDialogDataPtr  vdp;

  if (bvp == NULL) return;
  vdp = (ViewerDialogDataPtr) GetObjectExtra (bvp->udv);
  UDV_resize_viewer (bvp->udv, vdp);
}

BioseqPageData udvPageData = {
  "Sequence", TRUE, TRUE, TRUE, FALSE, -1,
  PopulateUDV, ShowUDV, SelectUDV,
  NULL, NULL,
  NULL, NULL, ResizeUDV, NULL
};

static void UDV_SlaveQuit(WindoW w)
{
	Remove(w);
}

/*******************************************************************************

  Function : CreateUDVpanel()
  
  Purpose : viewer panel creation ; only graphic strctures and panel itself
  
  Parameters : w; parent main window
				vmp; main data structure
				  
  Return value : FALSE if failure 

*******************************************************************************/
static Boolean  CreateUDVpanel(WindoW w,ViewerMainPtr PNTR vmp,
                               SAM_ViewGlobal *vgp)
{
ViewerDialogDataPtr vdp;
WindoW				hParent;

	/*w==NULL; we have to build a child viewer*/
	/*w!=NULL; we deal with an autonomous viewer*/
	if (w==NULL){
		Int2 Margins;
		
		*vmp=(ViewerMainPtr)MemNew(sizeof(ViewerMain));
		if (*vmp){
			MemSet(*vmp,0,sizeof(ViewerMain));
		}
		else{
			Message (MSG_ERROR, "Viewer creation failed.");
			return(FALSE);
		}

		Margins=20*stdCharWidth;
        hParent = NULL;
        if(vgp != NULL) {
            if(vgp->MasterViewer == SAMVIEWCN3D) {
                hParent=DocumentWindow(vgp->Rect.left,vgp->Rect.top,
                    (Int2)(-10),
                    (Int2)(-10),
                    szAppName, 
                    UDV_SlaveQuit,
                    UDV_WinMainResize);
            }
            else {
                hParent=DocumentWindow(Margins,Margins,
                    (Int2)(-10),
                    (Int2)(-10),
                    szAppName, 
                    UDV_SlaveQuit,
                    UDV_WinMainResize);
            }
        }
        else {
			hParent=DocumentWindow(Margins,Margins,
                (Int2)(-10),
                (Int2)(-10),
				szAppName, 
				UDV_SlaveQuit,
				UDV_WinMainResize);
        }

		if (hParent==NULL){
			Message (MSG_ERROR, "Viewer creation failed.");
			MemFree((*vmp));
			return(FALSE);
		}
        w = hParent;
		(*vmp)->hWndMain=hParent;
		SetObjectExtra (hParent, (Pointer) *vmp, (FreeProc)UDV_WinMainCleanupExtraProc);

		(*vmp)->AutonomeViewer=FALSE;

		/*menus*/
		UDV_SetupMenus(hParent,FALSE, vgp);
		UDV_set_MainMenus(&((*vmp)->MainMenu),FALSE);
		UDV_set_MainControls(*vmp,FALSE);
		/*create the windows*/
		CreateMainControls(w,*vmp,vgp);

		RealizeWindow(hParent);
		Show(hParent);
	}
	else {
		hParent=w;
		/*hide the logo panel; only fot the autonomous viewer variant*/
		if ((*vmp)->AutonomeViewer) (*vmp)->Show_logo=FALSE;
	}
	
	vdp=(*vmp)->vdp;
	
	/*general graph data init; scale pos, etc.*/
	UDV_Init_GraphData(vdp->UnDViewer,&vdp->udv_graph);
	return(TRUE);
}



/*******************************************************************************

  Function : UDV_create_buffer()
  
  Purpose : create a sequence buffer for a specific bioseq 
  
  Parameters : vdp; general data
  
  Return value : none 

*******************************************************************************/
NLM_EXTERN void  UDV_create_buffer(UnDViewerGraphDataPtr GrData,
		ValNodePtr ParaG_list,BspInfoPtr bsp_i,ValNodePtr StartScan,
		Boolean bReverse)
{
ParaGPtr 	pgp=NULL;
ValNodePtr 	vnp=NULL,vnp2=NULL;
Int4 		start_seq=0,stop_seq=0,stop=0,nLength=0,StartBuf=0,StopBuf=0;
SeqIdPtr 	sip;
	
	if (StartScan==NULL) vnp=ParaG_list;
	else vnp=StartScan;
	
	/*find the ParaG where ScrollPos is Located*/
	/*Find the end of the display; last PGP currently displayed*/
	if (GrData->udv_vscrl.ScrollMax){/*Vscroll ?*/
		for( ; vnp != NULL ; vnp=vnp->next){
			if (vnp->data.ptrvalue){
				pgp=(ParaGPtr)vnp->data.ptrvalue;
				if ((GrData->udv_vscrl.ScrollPos>=pgp->StartLine) &&
					(GrData->udv_vscrl.ScrollPos<=(pgp->StartLine+pgp->nLines))){
						start_seq=pgp->StartLetter;
						break;
				}
			}		
		}
	}	
	else{/*no Vscroll: buffer starts at the beginning of the Bioseq*/
		start_seq=0;
	}

	/*Find the end of the display; last PGP currently displayed*/
	if (GrData->udv_vscrl.ScrollMax){/*Vscroll ?*/
		stop=MIN(GrData->udv_panel.nTotLines,
			GrData->udv_vscrl.ScrollPos+GrData->udv_vscrl.ScrollPage);
		for(vnp2=vnp ; vnp2 != NULL ; vnp2=vnp2->next){
			pgp=(ParaGPtr)vnp2->data.ptrvalue;
			if (pgp->StartLine+pgp->nLines>=stop){
				stop_seq=pgp->StopLetter;
				break;
			}
		}
	}
	else{/*no Vscroll: buffer ends at the end of the Bioseq*/
		stop_seq=bsp_i->bspLength;
	}

	/*nb of char within a panel*/
	nLength=stop_seq-start_seq+1;

	/*read sequence; buffer size: three panels*/
	StartBuf=MAX(0,start_seq-nLength);
	StopBuf=MIN(bsp_i->bspLength-1,StartBuf+3*nLength);
	
	/*if same buffer, return*/
	if (bsp_i->StartBuf==StartBuf && bsp_i->StopBuf==StopBuf) return;

	bsp_i->StartBuf=StartBuf;
	bsp_i->StopBuf=StopBuf;
	bsp_i->LengthBuf=bsp_i->StopBuf-bsp_i->StartBuf+1;

	if (bsp_i->SeqBuf) MemFree(bsp_i->SeqBuf);

	sip=SeqIdFindBest(bsp_i->bsp->id,0);
	if (sip==NULL) sip=bsp_i->bsp->id;

	if (bReverse){
		bsp_i->SeqBuf=UDV_Read_SequenceEx (sip,
			bsp_i->bspLength-1-bsp_i->StopBuf,bsp_i->bspLength-1-bsp_i->StartBuf, 
			(Boolean)(!bsp_i->bspMolNuc),bsp_i->LengthBuf,Seq_strand_minus);
	}
	else{
 		bsp_i->SeqBuf=UDV_Read_Sequence (sip,bsp_i->StartBuf,bsp_i->StopBuf, 
			(Boolean)(!bsp_i->bspMolNuc),bsp_i->LengthBuf);
	}
	
	/*find the ParaG where bsp_i->StartBuf is Located*/
	if (StartScan==NULL) vnp=ParaG_list;
	else vnp=StartScan;
	for( ; vnp != NULL ; vnp=vnp->next){
		if (vnp->data.ptrvalue){
			pgp=(ParaGPtr)vnp->data.ptrvalue;
			if (pgp->StartLetter==bsp_i->StartBuf){
					bsp_i->PgpStartBuf=vnp;
					break;
			}
		}		
	}
}


/*******************************************************************************

  Function : UDV_analyse_buffer()
  
  Purpose : adjust a buffer in response to VScroll action 
  
  Parameters : vdp; general data
  
  Return value : none 

*******************************************************************************/
static void  UDV_analyse_buffer(UnDViewerGraphDataPtr GrData,
		ValNodePtr ParaG_list,BspInfoPtr bsp_i,Uint2 ActionType,
		Boolean bReverse)
{
ParaGPtr pgp=NULL;
ValNodePtr vnp=NULL,vnp2=NULL;
Int4 start_seq=0,stop_seq=0,stop=0;

	if (ActionType==BUFFER_REPOP_VCRL_LUP ||
		ActionType==BUFFER_REPOP_VCRL_PUP){/*Vscroll  Up*/
	
		if (bsp_i->PgpStartBuf==NULL) vnp=ParaG_list;
		else vnp=bsp_i->PgpStartBuf;

		/*find the ParaG where ScrollPos is Located*/
		/*Find the end of the display; last PGP currently displayed*/
		for( ; vnp != NULL ; vnp=vnp->next){
			if (vnp->data.ptrvalue){
				pgp=(ParaGPtr)vnp->data.ptrvalue;
				if ((GrData->udv_vscrl.ScrollPos>=pgp->StartLine) &&
					(GrData->udv_vscrl.ScrollPos<=(pgp->StartLine+pgp->nLines))){
						start_seq=pgp->StartLetter;
						break;
				}
			}		
		}
		
		if (start_seq<=bsp_i->StartBuf){
			UDV_create_buffer(GrData,ParaG_list,bsp_i,NULL,bReverse);
			return;
		}
	}

	if (ActionType==BUFFER_REPOP_VCRL_LDN ||
		ActionType==BUFFER_REPOP_VCRL_PDN){/*Vscroll  Down*/
	
		if (bsp_i->PgpStartBuf==NULL) vnp=ParaG_list;
		else vnp=bsp_i->PgpStartBuf;

		/*find the last ParaG located in the panel*/
		
		stop=MIN(GrData->udv_panel.nTotLines,
			GrData->udv_vscrl.ScrollPos+GrData->udv_vscrl.ScrollPage);
		for(vnp2=vnp; vnp2 != NULL ; vnp2=vnp2->next){
			pgp=(ParaGPtr)vnp2->data.ptrvalue;
			if (pgp->StartLine+pgp->nLines>=stop){
				stop_seq=pgp->StopLetter;
				break;
			}
		}
		if (stop_seq>=bsp_i->StopBuf-GrData->udv_panel.nCharByLine){
			UDV_create_buffer(GrData,ParaG_list,bsp_i,vnp,bReverse);
		}
	}
}


/*******************************************************************************

  Function : UDV_ReLocateRcParaGList()
  
  Purpose : relocate RecT (position) of each ParaG in the list. This function
  			is generally called in response to a Scale change (top, left, etc)
			or to Show Features change (on/off)
  
  Parameters : 	GrData; graphical data
				rcP; size and position of the UnD viewer's panel
				ShowTop; show scale on top if TRUE
				ShowTick; show scale's ticks if TRUE
				ShowSequence; show sequence if TRUE
				ShowFeature; show features if TRUE
				ShowBlank; add blank line at the bottom of the ParaG
				nTotL; total number of single lines for the whole ParaG list
				ParaG_head; head of the ParaG list

  Note : This function
  		is generally called in response to a Scale change (top, left, etc)
		or to Show Features change (on/off).		

  Return value : none (see nTotL)

*******************************************************************************/
static void UDV_ReLocateRcParaGList(/*UnDViewerGraphData GrData,*/
			RecT rcP,
			Boolean ShowTop,Boolean ShowTick,Boolean ShowSequence, 
			Boolean ShowFeatures,
			Boolean ShowBlank,Int4Ptr nTotL,ValNodePtr ParaG_head)
{
ValNodePtr 	vnp;					/*just for ParaG list scanning*/
Int1		minLineByParaG=0;		/*min height of a paraG, nb. of lines*/			
Int4		nTotLines=0;			/*Total lines in viewer*/
ParaGPtr 	pgp=NULL;				/*ParaG data*/

	if (ParaG_head == NULL) {*nTotL=0;return;}

	/*Minimum ParaG height*/
	if (ShowTop) minLineByParaG++;
	if (ShowTick) minLineByParaG++;
	if (ShowSequence) minLineByParaG++;
	if (ShowBlank) minLineByParaG++;

	/*modify the RecT of each ParaG*/
	for(vnp=ParaG_head ; vnp!=NULL ; vnp=vnp->next){
		pgp=(ParaGPtr)vnp->data.ptrvalue;
		/*modify in each ParaG just the needed*/
		pgp->StartLine=nTotLines;
		pgp->nLines=minLineByParaG;

		if (ShowFeatures) {
			/*nLines=pgp->nFeat+pgp->nTrans;*/
			pgp->nLines+=pgp->nFeatLines;/*nLines;*/
		}

		/*modify values*/
		if (ShowFeatures) nTotLines+=pgp->nLines;
		else nTotLines+=minLineByParaG;
	}

	*nTotL=nTotLines;
}

/*******************************************************************************

Function: FeaturesListBoxScan()

Purpose: scan features to retrieve the index of a seleted feature. This callback
		is used when the user clicks a feature on the UnDviewer panel while the
		Feature List Dialog Box is opened.

Parameters: see explore.h

*******************************************************************************/
static Boolean LIBCALLBACK FeaturesListBoxScan (SeqFeatPtr sfp, 
			SeqMgrFeatContextPtr context)
{
ScanFeatPtr sf=(ScanFeatPtr)context->userdata;

	if(sf){
		sf->index++;
		if (sf->eID==context->entityID && sf->iID==context->itemID){
			return(FALSE);
		}
	}
	return(TRUE);
}



/*******************************************************************************

Function: FLB_OnDraw()

Purpose: Owner Draw Features List box drawing procedure.

Parameter: mid; entry data to draw.

Return value: -

*******************************************************************************/
static void FLB_OnDraw(MIDataPtr mid)
{
RecT 		rc;
Char 		szTmp[255]={""};
FLDataPtr 	fldp=(FLDataPtr)mid->Data;

	if (fldp==NULL) return;
	
	rc=mid->rcItem;
	InsetRect(&rc,3,1);
	rc.left+=mid->cxChar/2;
	/*mid->rcItem.right=rc.right;*/
	/*selection ?*/	
	if (mid->Selected){
		SetColor(GetColorRGB(0,0,128));
		PaintRect(&rc);
		White();
	}
	else{
		White();
		PaintRect(&rc);
		Black();
	}
	mid->rcItem.bottom-=(mid->cyChar/3);
	/*draw features elements*/
	SelectFont(mid->f);
	MoveTo(mid->rcItem.left,(Int2)(mid->rcItem.bottom-mid->cyChar));
	sprintf(szTmp,"%5d.",fldp->num);
	PaintString(szTmp);
	if (mid->Selected) White();
	else Blue();
	MoveTo((Int2)(mid->rcItem.left+7*mid->cxChar),
			(Int2)(mid->rcItem.bottom-mid->cyChar));
	PaintString("Name :");
	if (!mid->Selected) Black();
	sprintf(szTmp,"%s",fldp->szComment);
	MoveTo((Int2)(mid->rcItem.left+14*mid->cxChar),
			(Int2)(mid->rcItem.bottom-mid->cyChar));
	PaintString(szTmp);

	if (fldp->segments>1){
		sprintf(szTmp,"%s [from %d to %d in %d segments]. Length: %d.",
			fldp->szType,fldp->from+1,fldp->to+1,
			fldp->segments,fldp->to-fldp->from+1);
	}
	else{
		sprintf(szTmp,"%s [from %d to %d]. Length: %d.",
			fldp->szType,fldp->from+1,fldp->to+1,fldp->to-fldp->from+1);
	}
	if (!mid->Selected) SetColor(GetColorRGB(0,128,0));
	MoveTo((Int2)(mid->rcItem.left+8*mid->cxChar),mid->rcItem.bottom);
	PaintString("Info :");
	if (!mid->Selected) Black();
	
	MoveTo((Int2)(mid->rcItem.left+15*mid->cxChar),mid->rcItem.bottom);
	PaintString(szTmp);
	LtGray();
	mid->rcItem.bottom+=(mid->cyChar/3);
	MoveTo(rc.left,mid->rcItem.bottom);	
	LineTo(rc.right,mid->rcItem.bottom);
	Black();
}

/*******************************************************************************

Function: FeatListlBoxProc()

Purpose: Owner Draw Features List box callback function.

Parameter: mid; entry data to draw.

Return value: -

*******************************************************************************/
static Boolean FeatListlBoxProc(WindoW lbox,Int2 msg,Pointer data)
{
MIDataPtr mid=(MIDataPtr)data;

	if (mid==NULL) return(TRUE);
	
	switch(msg){
		case ODLB_DrawItem:{
			WindoW temport;
			temport=SavePort(ParentWindow(lbox));
			Select(lbox);
			/*ClipRect(&mid->rcP);*/
			FLB_OnDraw(mid);
			/*ResetClip();*/
			RestorePort(temport);
			break;
		}
		case ODLB_MeasureItem:{
			
			mid->rcItem.left=mid->rcP.left;
			mid->rcItem.right=mid->rcP.right;
			mid->rcItem.top=0;
			mid->rcItem.bottom=5*mid->cyChar/2;
			
			break;
		}
		case ODLB_SelectItem:{
			ViewerMainPtr 	vmp;
			FLMDataPtr		pflm;
			FLDataPtr 		fldp=(FLDataPtr)mid->Data;

			pflm=(FLMDataPtr)GetObjectExtra(mid->Parent);
			if (pflm==NULL) break;
			vmp=(ViewerMainPtr)GetObjectExtra(pflm->hWndMain);
			if (vmp==NULL) break;
			if (fldp==NULL) break;
			vmp->vdp->ClickFeatFromDlg=TRUE;
			ObjMgrSelect(fldp->eID,fldp->iID,OBJ_SEQFEAT,0,NULL);
			break;
		}
	}
	return(TRUE);
}

/*******************************************************************************

  Function : FeatListDlgQuit()
  
  Purpose : end of prog Features List Dialog Box
  
  Parameters : w; Dialog Box window handle
  
  Return value : none 

*******************************************************************************/

static void FeatListDlgQuit(WindoW w)
{
FLMDataPtr		pflm;
ViewerMainPtr 	vmp;
	
	pflm = (FLMDataPtr) GetObjectExtra (w);
	if (!pflm) return;

	vmp = (ViewerMainPtr) GetObjectExtra (pflm->hWndMain);

	if (vmp==NULL) return;
	
	SetStatus(vmp->MainMenu.ShowFeatureList,FALSE);
	Remove(w);
	vmp->hFeatDlg=NULL;
}


/*******************************************************************************

  Function : FeatListDlgProc()
  
  Purpose : Features List Dialog Box Resize function 
  
  Parameters : w; handle of the dialog box
  
  Return value : none 

*******************************************************************************/
static void FeatListDlgProc (WindoW w)
{
RecT			rcDlg,rc;
FLMDataPtr		pflm;

	Select(w);
	ObjectRect(w,&rcDlg);
	
	pflm = (FLMDataPtr) GetObjectExtra (w);
	if (!pflm) return;
	if (pflm->gFeatClass){
		ObjectRect(pflm->gFeatClass,&rc);
		rc.bottom+=3;
	}
	else rc.bottom=10;
	
	OwnerDrawLbox_MoveWindow(pflm->lbox,10,rc.bottom,
		(Int2)(rcDlg.right-rcDlg.left-20),
		(Int2)(rcDlg.bottom-rcDlg.top-10-rc.bottom));
}

/*******************************************************************************

Function: UDV_FeaturesListBoxFind()

Purpose: feature selection

Parameters: see explore.h

*******************************************************************************/
NLM_EXTERN Boolean LIBCALLBACK UDV_FeaturesListBoxFind (SeqFeatPtr sfp, 
			SeqMgrFeatContextPtr context)
{
ScanFeatForSelectPtr sffs;


	sffs=(ScanFeatForSelectPtr)context->userdata;
	
	sffs->compteur++;

	if (sffs->eID==context->entityID && sffs->iID==context->itemID){
		sffs->index=sffs->compteur;
		return(FALSE);
	}
		
	return(TRUE);
}

/*******************************************************************************

Function: FeaturesListBoxInit()

Purpose: retrieve features for Features List Box

Parameters: see explore.h

*******************************************************************************/
static Boolean LIBCALLBACK FeaturesListBoxInit (SeqFeatPtr sfp, 
			SeqMgrFeatContextPtr context)
{
FLDataPtr fldp;
ScanFeatForSelectPtr sffs;

/* for test only
if (context->seqfeattype==SEQFEAT_HET){
 Char  str [256];
 int i;

  if (FeatDefLabel (sfp, str, sizeof (str) - 1, OM_LABEL_BOTH)) {
    printf ( "   Feature item %d index %d (%d) (%d - %d) (%d) %s\n",
             (int) context->itemID, (int) context->index, 
                         (int) context->numivals,(int) context->left,
                         (int) context->right,(int) context->strand,str);
        if (context->numivals>1){
                for (i=0;i<context->numivals*2;i+=2){
                        printf ( "   Ivals: (%d - %d)\n", context->ivals[i],
                                                context->ivals[i+1]);
                }
        }
  }
	
}
*/
	sffs=(ScanFeatForSelectPtr)context->userdata;
	
	/*keyword search only*/
	if (sffs->SearchFeat==TRUE){
		Char szType[COMMENT_SIZE];
		
		FeatDefLabel(sfp, szType, sizeof (szType) - 1, OM_LABEL_BOTH);
		StringUpper(szType);
		if (!StringStr(szType,sffs->szKeyword)){
			if (context->sfp->comment){
				MemCopy(szType,context->sfp->comment,
					MIN(COMMENT_SIZE-1,StringLen(context->sfp->comment)));
				StringUpper(szType);
				if (!StringStr(szType,sffs->szKeyword)) return(TRUE);
			}
			else return(TRUE);
		}
	}

	sffs->compteur++;
	fldp=(FLDataPtr)MemNew(sizeof(FLData));
	if (fldp==NULL) return(TRUE);
	fldp->num=sffs->compteur;/*context->index;*/
	fldp->from=context->left;
	fldp->to=context->right;
	fldp->segments=context->numivals;
	fldp->eID=context->entityID;
	fldp->iID=context->itemID;
	fldp->strand=context->strand;
	FeatDefLabel(sfp, fldp->szType, sizeof (fldp->szType) - 1, OM_LABEL_BOTH);
	if (context->sfp->comment){
		MemCopy(fldp->szComment,context->sfp->comment,
		MIN(COMMENT_SIZE-1,StringLen(context->sfp->comment)));
	}
	OwnerDrawLbox_AddItem(sffs->lbox,(Pointer) fldp);

	if (sffs->eID==context->entityID && sffs->iID==context->itemID){
		sffs->index=sffs->compteur;
	}
	
	sffs->SeqFeatListAvail[context->seqfeattype]=TRUE;
	
	return(TRUE);
}

/*******************************************************************************

  Function : FeatClassChooseProc()
  
  Purpose : repopulate the Feature list box when the nice user choose various
  			feature classes
  
  Parameters : pop; popup list of feat classes
  
  Return value : none 

*******************************************************************************/
static void FeatClassChooseProc (PopuP pop)
{
WindoW 				w,temport;
Int1 				Choice;
ViewerMainPtr 		vmp;
ScanFeatForSelect 	sffs;
Boolean 			SeqFeatListAvail[SEQFEAT_MAX];
FLMDataPtr			pflm;
RecT				rcP;

	w=(WindoW)ParentWindow(pop);

	if (w==NULL) return;
	pflm = (FLMDataPtr) GetObjectExtra (w);
	if (!pflm) return;
	vmp = (ViewerMainPtr) GetObjectExtra (pflm->hWndMain);
	if (!vmp) return;
	if (!pflm->SeqFeatClass) return;
	Choice=pflm->SeqFeatClass[GetValue(pflm->pop)-1];
	/*reset lbox Content*/
	OwnerDrawLbox_ResetLBContent(pflm->lbox);
	
	/*repopulate lbox with the user Choice*/
	sffs.eID=vmp->vdp->Item_select.eIDsel;		
	sffs.iID=vmp->vdp->Item_select.iIDsel;		
	sffs.index=0;
	sffs.compteur=0;
	sffs.lbox=pflm->lbox;
	sffs.SearchFeat=FALSE;
	sffs.szKeyword[0]='\0';
	MemSet(sffs.SeqFeatListAvail,0,sizeof(sffs.SeqFeatListAvail));
	if (Choice==0){
		MemSet(SeqFeatListAvail,1,sizeof(SeqFeatListAvail));
	}
	else{
		MemSet(SeqFeatListAvail,0,sizeof(SeqFeatListAvail));
		SeqFeatListAvail[Choice]=TRUE;
	}	
	SeqMgrExploreFeatures (vmp->vdp->bsp_i.bsp, (Pointer) &sffs, 
				FeaturesListBoxInit, NULL, SeqFeatListAvail, NULL);
	if (sffs.index>0)
		OwnerDrawLbox_SelectItem(pflm->lbox,sffs.index);
	temport=SavePort(w);
	Select(pflm->lbox);
	ObjectRect(pflm->lbox,&rcP);
	InvalRect(&rcP);
	RestorePort(temport);
}

/*******************************************************************************

  Function : FeatListDlgCleanupProc()
  
  Purpose : clean data structure of Features DlgBox
  
  Parameters : data; something to delete
  
  Return value : none 

*******************************************************************************/
static void FeatListDlgCleanupProc (GraphiC g, VoidPtr data)
{
FLMDataPtr		pflm=(FLMDataPtr)data;

	if (pflm){
		if (pflm->SeqFeatClass) Free(pflm->SeqFeatClass);
		Free(pflm);
	}
}


/*******************************************************************************

  Function : InitFeaturesListDlg()
  
  Purpose : initialize Features DlgBox
  
  Parameters : vmp; main data 
  				hWndMain; handle of the viewer main window
				Search; TRUE is keyword search
				szKeyword; used only for keyword search
  
  Return value : none 

*******************************************************************************/
static void	InitFeaturesListDlg(ViewerMainPtr vmp,WindoW hWinMain,
	Boolean Search,CharPtr szKeyword)
{	
WindoW			w,lbox; 
Int2			Margins,nFeatClass,nClass=SEQFEAT_MAX+1;
Int1			k;
RecT			rcDlg,rc;
FLMDataPtr		pflm;
ScanFeatForSelect sffs;
GrouP g2;
PopuP pop;
PrompT txt;

	Margins=4*stdCharWidth;
	w=/*DocumentWindow*/Nlm_MovableModalWindow(Margins,Margins ,
			(Int2)((screenRect.right-screenRect.left)/2-2*Margins), 
			(Int2)((screenRect.bottom-screenRect.top)/3-2*Margins), 
			"Features List", 
			FeatListDlgQuit/*,
			FeatListDlgProc*/);
    Nlm_SetResize(w, FeatListDlgProc);

	if (w==NULL){
		Message (MSG_ERROR, "Dialog creation failed.");
		return;
	}
	
	Select(w);
	ObjectRect(w,&rcDlg);
	
	if (!Search){
		g2=HiddenGroup(w,2,0,NULL);
		txt=StaticPrompt(g2,"Feature classes :",0,0,NULL,'l');

#ifdef WIN_MAC
		pop=PopupList(g2,TRUE,FeatClassChooseProc);
#endif

#ifndef WIN_MAC
		pop=PopupList(g2,FALSE,FeatClassChooseProc);
#endif
		AlignObjects(ALIGN_MIDDLE,(HANDLE) txt, (HANDLE) pop, NULL);

		ObjectRect(g2,&rc);
		rc.bottom+=3;
	}
	else rc.bottom=10;

	lbox=OwnerDrawLbox_Create(w,10,rc.bottom,
		(Int2)(rcDlg.right-rcDlg.left-20),
		(Int2)(rcDlg.bottom-rcDlg.top-10-rc.bottom), 
		FeatListlBoxProc,
		vmp->vdp->udv_graph.udv_font.hFnt,
		vmp->vdp->udv_graph.udv_font.cxChar,
		vmp->vdp->udv_graph.udv_font.cyChar);
		
	if (lbox==NULL) {
		Remove(w);
		return;
	}

	pflm=(FLMDataPtr)MemNew(sizeof(FLMData));
	if (pflm==NULL){
		Remove(w);
		return;
	}
	MemSet(pflm,0,sizeof(FLMData));
	pflm->hWndMain=hWinMain;
	pflm->lbox=lbox;
	if (!Search){
		pflm->gFeatClass=g2;
		pflm->pop=pop;
	}
	else{
		pflm->gFeatClass=NULL;
		pflm->pop=NULL;
	}
	/*add features to the listbox*/
	WatchCursor();
	sffs.eID=vmp->vdp->Item_select.eIDsel;		
	sffs.iID=vmp->vdp->Item_select.iIDsel;		
	sffs.index=0;
	sffs.compteur=0;
	sffs.lbox=lbox;
	sffs.SearchFeat=Search;
	StringCpy(sffs.szKeyword,szKeyword);
	MemSet(sffs.SeqFeatListAvail,0,sizeof(sffs.SeqFeatListAvail));

	SeqMgrExploreFeatures (vmp->vdp->bsp_i.bsp, (Pointer) &sffs, 
				FeaturesListBoxInit, NULL, NULL, NULL);

	
	/*no features found*/
	if (sffs.compteur==0){
		RealizeWindow(w);
		goto error;
	}
	/*Select the currently selected feature of UNDViewer*/
	if (sffs.index>0)
		OwnerDrawLbox_SelectItem(pflm->lbox,sffs.index);

	/*Fill the popup list with Feature classes*/
	if (!Search){
		nFeatClass=0;
		for(k=0;k<nClass;k++){
			if (sffs.SeqFeatListAvail[k]) nFeatClass++;
		}
		PopupItem(pop,szSeqFeatClassName[0]);/*ALL*/
		if (nFeatClass>0){
			nFeatClass++;
			pflm->SeqFeatClass=(Int1Ptr)MemNew(nFeatClass*sizeof(Int1));
			if (pflm->SeqFeatClass){
				Int1 j=1;
				pflm->SeqFeatClass[0]=0;
				for(k=1;k<nClass;k++){
					if (sffs.SeqFeatListAvail[k]) {
						pflm->SeqFeatClass[j++]=k;
						PopupItem(pop,szSeqFeatClassName[k]);
					}
				}
			}
		}
		SetValue(pop,1);
	}
	/*show the dialog box*/
	ArrowCursor();
	SetObjectExtra (w, (Pointer) pflm, FeatListDlgCleanupProc);
	RealizeWindow(w);
	Show(w);
	Update();
	vmp->hFeatDlg=w;
	/*if true means this function has been called for a string search*/
	if (Search) SetStatus(vmp->MainMenu.ShowFeatureList,TRUE);
	return;

error:
	ArrowCursor();
	/*delete unused data*/
	if (pflm) Free(pflm);
	/*delete the dlg box*/
	Remove(w);
	/*update menu*/
	SetStatus(vmp->MainMenu.ShowFeatureList,FALSE);
	MsgAlert(KEY_OK, SEV_INFO, "OneD-Viewer","Nothing found.");
}

/*******************************************************************************

  Function : ShowFeaturesListDlg()
  
  Purpose : show/hide Features DlgBox
  
  Parameters : i; menu item
  
  Return value : none 

*******************************************************************************/
static void ShowFeaturesListDlg(IteM i)
{
ViewerMainPtr 	vmp;
WindoW hWinMain;

	hWinMain=(WindoW)ParentWindow(i);

	if (!hWinMain) return;
	
	vmp = (ViewerMainPtr) GetObjectExtra (hWinMain);

	if (vmp==NULL) return;

	if (GetStatus(i)==FALSE){
		Remove(vmp->hFeatDlg);
		vmp->hFeatDlg=NULL;
		return;
	}
	InitFeaturesListDlg(vmp, hWinMain, FALSE, NULL);
}

/*******************************************************************************

  Function : UDV_FeatKeySearchCancelProc()
  
  Purpose : manage cancel button of the Feature keyword search dialog box 
  
  Parameters : g; button
  
  Return value : none 

*******************************************************************************/
static void UDV_FeatKeySearchCancelProc(ButtoN g)
{
WindoW				w;
FKSDataPtr			fksd;
ViewerMainPtr 		vmp;

	w=(WindoW)ParentWindow(g);

	if (!w) return;
	fksd = (FKSDataPtr) GetObjectExtra (w);
	
	if (fksd){
		vmp = (ViewerMainPtr) GetObjectExtra (fksd->hWinMain);
	    UDV_set_PullMenus(&vmp->MainMenu,TRUE);
	}
	
	Remove(w);
}

/*******************************************************************************

  Function : UDV_FeatKeySearchAcceptProc()
  
  Purpose : manage search button of the Feature keyword search dialog box 
  
  Parameters : g; button
  
  Return value : none 

*******************************************************************************/
static void UDV_FeatKeySearchAcceptProc(ButtoN g)
{
Char 				szFName[PATH_MAX]={""};
WindoW				w;
FKSDataPtr			fksd;
ViewerMainPtr 		vmp;

	w=(WindoW)ParentWindow(g);

	if (!w) return;
	fksd = (FKSDataPtr) GetObjectExtra (w);
	
	if (fksd){
		vmp = (ViewerMainPtr) GetObjectExtra (fksd->hWinMain);
		/*retrieve the keyword*/
		GetTitle(fksd->keyField, szFName, sizeof(szFName)-1);
		/*find it; if yes: Features List will be created*/
		StringUpper(szFName);
		InitFeaturesListDlg(vmp, fksd->hWinMain, TRUE, szFName);
	    UDV_set_PullMenus(&vmp->MainMenu,TRUE);
	}
	
	Remove(w);
}

/*******************************************************************************

  Function : UDV_FeatKeySearchTextProc()
  
  Purpose : manage keyword edit control of the  Feature keyword search  
  			dialog box 
  
  Parameters : t; edit control
  
  Return value : none 

*******************************************************************************/
static void UDV_FeatKeySearchTextProc(TexT t)
{
Char 				szFName[PATH_MAX]={""};
WindoW				w;
FKSDataPtr			fksd;

	w=(WindoW)ParentWindow(t);

	if (!w) return;
	fksd = (FKSDataPtr) GetObjectExtra (w);

	if (fksd==NULL) return;
	
	GetTitle(t, szFName, sizeof(szFName)-1);

	if (StringLen(szFName) == 0)
		Disable(fksd->bOk);
	else Enable(fksd->bOk);

	return;
}

/*******************************************************************************

  Function : UDV_SearchFeatForKey()
  
  Purpose : callback of the Options|Search for features command 
  
  Parameters : i; command item
  
  Return value : none 

*******************************************************************************/
static void UDV_SearchFeatForKey(IteM i)
{
ViewerMainPtr 		vmp;
WindoW				hWinMain,w;
GrouP				g1,g2,g3;
ButtoN				b1,b2;
PrompT				t1;
TexT				t2;    
FKSDataPtr			fksd;
MsgAnswer			nRet;

	hWinMain=(WindoW)ParentWindow(i);

	if (!hWinMain) return;
	
	vmp = (ViewerMainPtr) GetObjectExtra (hWinMain);

	if (vmp==NULL) return;
	
	/*if hFeatDlg opens, ask to close it*/
	if (vmp->hFeatDlg){
		nRet=MsgAlert(KEY_OKC, SEV_INFO, "OneD-Viewer",
			"Features List is already opened.\n Would you like to close it?");
		if (nRet==ANS_OK){
			Remove(vmp->hFeatDlg);
			vmp->hFeatDlg=NULL;
			SetStatus(vmp->MainMenu.ShowFeatureList,FALSE);
		}
		else return;
	}

	fksd=(FKSDataPtr)MemNew(sizeof(FKSData));
	if (!fksd) return;
	MemSet(fksd,0,sizeof(FKSData));		

    w = MovableModalWindow(-30, -20,  -10,  -10, 
				"OneD-Viewer - Find Features",  NULL);
	g1=HiddenGroup(w,0,2,NULL);

	SetGroupSpacing(g1,10,10);
	g2=HiddenGroup(g1,2,0,NULL);
	t1=StaticPrompt(g2,"Keyword :",0,0,systemFont,'l');
    t2=DialogText(g2,"",20, UDV_FeatKeySearchTextProc);

	AlignObjects(ALIGN_MIDDLE,(HANDLE) t1, (HANDLE) t2, NULL);

	g3=HiddenGroup(g1,2,0,NULL);

    b1 = DefaultButton(g3, "Search",  UDV_FeatKeySearchAcceptProc);
    b2 = PushButton(g3, "Cancel",  UDV_FeatKeySearchCancelProc);
   
	SetObjectExtra (w, (Pointer) fksd, StdCleanupExtraProc);
	fksd->hWinMain=hWinMain;
	fksd->keyField=t2;
	fksd->bOk=b1;
	RealizeWindow(w);
    Show(w);
    Select(w);
	Select(t2);

	/*disable all menus*/
    UDV_set_PullMenus(&vmp->MainMenu,FALSE);
}

/*******************************************************************************

  Function : UDV_GenViewProc()
  
  Purpose : Genome Viewer menu Proc

  Return value : TRUE if ok 

*******************************************************************************/
#ifdef UDV_GENOME
static void UDV_GenViewProc(IteM i)
{
WindoW        w;
ViewerMainPtr vmp;

	w=ParentWindow(i);
	vmp = (ViewerMainPtr) GetObjectExtra (w);
	if (vmp==NULL) return;
	if (vmp->vdp==NULL) return;
	
	GatherSpecificProcLaunch(vmp->GVprocid, "GV_MOD", OMPROC_VIEW, FALSE, 
		vmp->vdp->bsp_i.bsp_entityID,vmp->vdp->bsp_i.bsp_itemID,OBJ_BIOSEQ);

}
#endif

/*******************************************************************************

  Function : UDV_LoadSpecificEditor()
  
  Purpose : load an editor given entityID, itemID, itemtype

*******************************************************************************/
NLM_EXTERN void UDV_LoadSpecificEditor(BioseqViewPtr bvp, Uint2 entityID, 
		Uint2 itemID, Uint2 itemtype)
{
SeqEntryPtr sep;

	if (bvp==NULL) return;
	
	sep = GetTopSeqEntryForEntityID (entityID);
	
	if (bvp->launchSubviewers) {
		WatchCursor ();
		Update ();
		LaunchNewBioseqViewer (bvp->bsp, entityID, itemID, itemtype);
		ArrowCursor ();
		Update ();
	} else if (LaunchViewerNotEditor (bvp, sep, entityID, itemID, itemtype)) {
		WatchCursor ();
		Update ();
		LaunchNewBioseqViewer (bvp->bsp, entityID, itemID, itemtype);
		ArrowCursor ();
		Update ();
	} else if (bvp->launchEditors) {
		WatchCursor ();
		Update ();
		GatherProcLaunch (OMPROC_EDIT, FALSE, entityID, itemID,
	    	  itemtype, 0, 0, itemtype, 0);
		ArrowCursor ();
		Update ();
	}
}

/*******************************************************************************

  Function : UDV_ShowReverseComplement()
  
  Purpose : callback of the Options|ShowRevComp 
  
  Parameters : i; command item
  
  Return value : none 

*******************************************************************************/
static void UDV_ShowReverseComplement(IteM i)
{
ViewerMainPtr 		vmp;
ViewerDialogDataPtr vdp;
WindoW				hWinMain;

	hWinMain=(WindoW)ParentWindow(i);
	if (!hWinMain) return;
	vmp = (ViewerMainPtr) GetObjectExtra (hWinMain);
	if (vmp==NULL) return;
	vdp = vmp->vdp;
	if (vdp == NULL) return;
	if (!vdp->bsp_i.bspMolNuc) return;
	
	/*switch the seq. direction*/
	WatchCursor();
	if (GetStatus(i))
		vdp->bDisplayRevComp = TRUE;
	else
		vdp->bDisplayRevComp = FALSE;
	/*rebuilt the Feature Index*/
	SeqMgrClearFeatureIndexes(0,vdp->bsp_i.bsp);
	SeqMgrIndexFeaturesEx (0, vdp->bsp_i.bsp, vdp->bDisplayRevComp, FALSE);
	
	/*rebuilt the display*/
	UDV_resize_viewer(vdp->UnDViewer,vdp);	
	ArrowCursor ();
}


NLM_EXTERN void ClearUDV_mouse_select(UDV_mouse_selectPtr msp) {
/*******************************************************************************
  zero the UDV_mouse_select structure.
*******************************************************************************/
  if (msp->first_cols != NULL) MemFree(msp->first_cols);
  if (msp->first_pgps != NULL) MemFree(msp->first_pgps);
	MemSet(msp, 0, sizeof(UDV_mouse_select));
}
