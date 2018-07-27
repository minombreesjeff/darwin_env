/*  $Id: ddvopen.c,v 1.96 2001/04/26 16:41:20 juran Exp $
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
* File Name:  ddvopen.c
*
* Author:  Patrick Durand
*
* Version Creation Date:   06/19/99
*
* $Revision: 1.96 $
*
* File Description: code to open a SeqAlign (file & Net) and code of the
* message callback for DeuxD-Viewer (DDV).
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: ddvopen.c,v $
* Revision 1.96  2001/04/26 16:41:20  juran
* Squelch warnings.
*
* Revision 1.95  2000/07/17 17:46:48  hurwitz
* made it so show/hide dialog only updates view when ok is clicked
*
* Revision 1.94  2000/07/14 22:24:56  lewisg
* fix scroll, click, visual c++ build bugs.  add command line arg parse to ddv
*
* Revision 1.92  2000/07/05 19:23:13  lewisg
* add two panes to ddv, update msvc project files
*
* Revision 1.91  2000/06/16 14:57:03  lewisg
* move entrez calls out of desktop
*
* Revision 1.90  2000/06/15 17:33:18  hurwitz
* used view seqAlignPtr to get original (bug fix), and started working on left/right/center justify for DDE
*
* Revision 1.89  2000/06/15 16:01:07  hurwitz
* rewrote section that calls ViewMgr_Attach when SeqAlign is not editable but IBMable
*
* Revision 1.88  2000/06/13 18:23:55  hurwitz
* made ViewMgr_MakeMultiple routine, call this on each launch of DDE rather than launch of DDV
*
* Revision 1.87  2000/06/12 23:02:40  hurwitz
* enable launch of DDE from Cn3D, swith from DDV_ComputeRuler to DDE_ReMakeRuler, get rid of styles option for DDE
*
* Revision 1.86  2000/06/08 20:04:38  hurwitz
* made warning about converting to true multiple alignment into a Message window, and other small fixes
*
* Revision 1.85  2000/06/07 19:09:36  hurwitz
* made DDE_ReMakeRuler work with linked list of ParaGs
*
* Revision 1.84  2000/06/06 14:44:16  hurwitz
* fixed bug that SetAppProperty(dde_nogaps)
*
* Revision 1.83  2000/06/02 19:57:03  hurwitz
* added a query dialog when converting from pairwise multiple to true multiple, bug fix of double freeing of a seqAlignPtr
*
* Revision 1.82  2000/06/01 14:40:08  hurwitz
* temp fix: when DDV is launched as slave, don't allow aligned gaps
*
* Revision 1.81  2000/05/31 23:07:26  hurwitz
* made NoGaps a runtime parameter, fixed bug with vertical scroll of show/hide list, save edits query is not performed if nothing to save
*
* Revision 1.80  2000/05/24 21:43:00  hurwitz
* getting hide/show rows to work with DDV and DDE together
*
* Revision 1.79  2000/05/23 22:00:14  hurwitz
* working on launch of DDE from DDV
*
* Revision 1.78  2000/05/15 23:39:34  lewisg
* shred cblast, add menu items for gapped/ungapped, fix pdbheaders
*
* Revision 1.77  2000/05/15 22:00:36  hurwitz
* put save-edits query into DDV_SlaveQuit where it belongs
*
* Revision 1.76  2000/05/12 21:18:13  hurwitz
* added window asking if user wants to save unsaved edits for dde
*
* Revision 1.75  2000/05/12 16:15:25  hurwitz
* reverted to not doing IntersectOnMaster for DDE, now determined by call to ViewMgr_Attach
*
* Revision 1.74  2000/05/11 16:28:44  wheelan
* made DDV_ShredAln NLM_EXTERN
*
* Revision 1.73  2000/05/10 22:03:29  thiessen
* temporary fix for CN3D_[TRIM,SHRED] not defined
*
* Revision 1.72  2000/05/10 16:47:25  wheelan
* added CN3D_SHRED and CN3D_TRIM options; added ddv_shred_aln
*
* Revision 1.71  2000/05/09 14:21:54  wheelan
* added ddv_truncate_overlaps
*
* Revision 1.70  2000/05/08 16:29:27  wheelan
* change to ungapped BLAST
*
* Revision 1.69  2000/05/04 22:43:38  hurwitz
* don't launch DDE on top of DDV, change some wording, redraw DDE after save to AlnMgr
*
* Revision 1.68  2000/05/03 17:48:30  hurwitz
* keep DDV in same column when DDE does a save
*
* Revision 1.67  2000/05/03 16:16:51  hurwitz
* fixed bug fix from yesterday
*
* Revision 1.66  2000/05/02 19:50:38  hurwitz
* fixed some bugs with launching DDE from DDV, added new alnMgr fn for positioning DDE on proper column
*
* Revision 1.65  2000/04/28 14:43:37  hurwitz
* send message to DDV after edits in DDE are accepted by AlnMgr
*
* Revision 1.64  2000/04/27 22:21:58  lewisg
* misc bugs/features
*
* Revision 1.63  2000/04/27 19:57:59  hurwitz
* scroll to aligned block on launch of DDE
*
* Revision 1.62  2000/04/27 13:33:41  durand
* SpacerSize min value fixed to 1; update view when SpacerSize is changed
*
* Revision 1.61  2000/04/21 23:00:50  hurwitz
* can launch DDE from DDV
*
* Revision 1.60  2000/04/20 23:27:43  lewisg
* misc bug fixes
*
* Revision 1.59  2000/04/19 15:45:50  hurwitz
* can create display for a block
*
* Revision 1.58  2000/04/17 13:30:43  durand
* removed g_hParent and unused functions DDV_LaunchAlignViewer and DDV_LaunchAlignEditor
*
* Revision 1.57  2000/04/11 14:08:33  durand
* removed a debug line
*
* Revision 1.56  2000/04/10 20:33:40  lewisg
* fix show/hide for blast multiple, make blast multiple API generic
*
* Revision 1.55  2000/04/10 19:53:14  durand
* allowed ddv to display NULL SeqAlign
*
* Revision 1.54  2000/04/08 00:37:32  lewisg
* multiple seqentries, NEWSEQ message, etc.
*
* Revision 1.53  2000/04/07 15:03:08  durand
* added use of UPDATE_TYPE_NEWSEQ; DDV now rebuilt EntitiesTable after an update msg
*
* Revision 1.52  2000/04/04 17:51:54  lewisg
* fix various seq import bugs
*
* Revision 1.51  2000/03/29 23:38:06  lewisg
* hide/show, fixes to saving and opening
*
* Revision 1.50  2000/03/29 19:26:10  lewisg
* use blast2seqs instead of bandalign
*
* Revision 1.49  2000/03/27 22:15:06  lewisg
* add show/hide row dialog
*
* Revision 1.48  2000/03/27 18:07:48  hurwitz
* page up and page down working, give panel focus on launch
*
* Revision 1.47  2000/03/24 20:34:58  lewisg
* add blast from file, bug fixes, get rid of redundant code, etc.
*
* Revision 1.46  2000/03/22 20:31:34  durand
* added system menus for slave DDV
*
* Revision 1.45  2000/03/21 14:21:46  durand
* fixed a problem with menus setup
*
* Revision 1.44  2000/03/20 19:26:53  kans
* need to create menus before other window contents - Mac menu bar for windows is actually in content area
*
* Revision 1.43  2000/03/16 14:11:59  durand
* set corretly mouse mode menu
*
* Revision 1.42  2000/03/02 21:11:06  lewisg
* use bandalign for import sequence, make standalone ddv use viewmgr, make dialogs modal, send color update
*
* Revision 1.41  2000/03/02 15:43:10  durand
* use MovableModalWindow for dialog boxes
*
* Revision 1.40  2000/03/01 22:49:41  lewisg
* import bioseq, neatlyindex, get rid of dead code
*
* Revision 1.39  2000/02/28 20:15:32  durand
* if DDV is started from Cn3D, DDV doesn't set up anymore letters layout
*
* Revision 1.38  2000/02/15 22:40:57  lewisg
* add ability to launch udv so that it colors by row, fixes to colormgr, track rows from viewmgr, fix visual c projects
*
* Revision 1.37  2000/02/07 21:14:08  durand
* added a generic answer to Update messages
*
* Revision 1.36  2000/02/07 14:03:35  durand
* replace BioseqUnlockById by BioseqUnlock
*
* Revision 1.35  2000/02/05 01:32:22  lewisg
* add viewmgr, move place freeing is done in ddv, modify visual c++ projects
*
* Revision 1.34  2000/02/04 16:05:40  durand
* add click action to select a row
*
* Revision 1.33  2000/02/02 14:44:32  durand
* added function to create data structure for block editor, fixed some bugs
*
* Revision 1.32  2000/01/26 13:38:53  durand
* update the GUI for the editor. Add functions to create the data to be used by the editor
*
* Revision 1.31  2000/01/18 22:49:16  lewisg
* send OM_MSG_FLUSH to ddv/udv, tweak CPK coloration, misc bugs
*
* Revision 1.30  2000/01/12 21:52:16  durand
* add import function; update menus when DDV is loaded from Cn3D
*
* Revision 1.29  2000/01/11 15:05:23  durand
* remove network stuff
*
* Revision 1.28  2000/01/10 15:09:46  durand
* Use Entrez instead of ID1
*
* Revision 1.27  2000/01/05 21:11:14  durand
* update mouse click actions and DrawSequence function for a better use from ddv and cn3d
*
* Revision 1.26  1999/12/30 21:08:45  lewisg
* bioseq import dialog
*
* Revision 1.25  1999/12/30 13:45:50  beloslyu
* fix comment
*
* Revision 1.24  1999/12/29 22:55:03  lewisg
* get rid of seqalign id
*
* Revision 1.23  1999/12/23 19:22:06  durand
* modify default options for DDV when loaded from Cn3D
*
* Revision 1.22  1999/12/21 15:27:24  durand
* avoid to quit Cn3D when closing DDV
*
* Revision 1.21  1999/12/20 20:20:41  lewisg
* allow cn3d to do color and ddv to do case when both are running
*
* Revision 1.20  1999/12/07 21:40:13  durand
* add mouse modes menu and caret facility for the editor
*
* Revision 1.19  1999/12/06 16:19:19  durand
* add GoTo facility to DDV
*
* Revision 1.18  1999/12/03 23:17:23  lewisg
* Patrick's new global update msg, argument passing when launching ddv, experimental editing
*
* Revision 1.17  1999/11/30 18:19:47  durand
* fix a problem with function declaration DDV_CloseData
*
* Revision 1.16  1999/11/29 15:26:26  durand
* designed a new GUI to fix problems under MacOS, Linux and SGI
*
* Revision 1.15  1999/11/17 22:43:58  durand
* speed up the selection manager for large SeqAlign
*
* Revision 1.14  1999/11/09 17:09:00  durand
* transfer some functions from ddvgraph to ddvcreate, so that ddvcreate remains Vibrant free and can be compiled with BLAST
*
* Revision 1.13  1999/11/04 22:11:38  durand
* add the Desktop to DDV. Add a better set of cleanup functions when closing DDV. Before creating color tables, try to get them from the SeqAlign
*
* Revision 1.12  1999/11/03 21:29:47  durand
* add CTRL and SHFT keys for mouse selection. redesign the loader functions of DDV to properly register the message callbacks
*
* Revision 1.11  1999/10/29 19:04:21  durand
* move DDVUpdateMSG in objmgr.h
*
* Revision 1.10  1999/10/29 14:15:39  durand
* add simple mouse selection functions
*
* Revision 1.9  1999/10/23 21:20:45  lewisg
* move g_hParent to ddvopen.c
*
* Revision 1.8  1999/10/23 14:54:34  durand
* resolve external symbol g_hParent
*
* Revision 1.7  1999/10/22 14:19:43  durand
* update the code for the startup functions of DDV drawing panel
*
* Revision 1.6  1999/10/20 18:37:31  lewisg
* add messagefunc for slave mode
*
* Revision 1.5  1999/10/20 13:17:18  durand
* add display for disc. SeqAlign tails
*
* Revision 1.4  1999/10/16 15:02:25  durand
* fixes due to toolkit build failed
*
* Revision 1.3  1999/10/15 21:57:36  durand
* add a UI for display options
*
* Revision 1.2  1999/10/12 15:01:29  lewisg
* resolve confict with internal/ddv
*
* Revision 1.1  1999/09/30 14:10:28  durand
* add ddv to toolkit
*
* Revision 1.14  1999/09/30 13:38:10  durand
* DDV_CreateDisplayFromIndex takes ParaG_Size as an argument
*
* Revision 1.13  1999/09/16 13:07:52  durand
* add File|Close and File|Open|Network commands
*
* Revision 1.12  1999/09/09 21:55:06  durand
* instantiate the Fle|Close command of DDV
*
* Revision 1.11  1999/09/02 17:36:07  durand
* reconcile ddvopen.c
*
* Revision 1.10  1999/08/19 17:15:27  wheelan
* added messages to indicate timing of index building vs color tables
*
* Revision 1.9  1999/08/04 18:02:12  wheelan
* changes to support new seqalign indexing
*
* Revision 1.8  1999/07/29 12:43:07  durand
* update DDV_GetAndCheckSeqAlign
*
* Revision 1.7  1999/07/20 17:18:23  durand
* update DDV_GetAndCheckSeqAlign for PopSet Viewer
*
* Revision 1.6  1999/07/20 14:58:01  durand
* use the Color Manager to display colored MSA
*
* Revision 1.5  1999/07/01 15:28:29  durand
* validate function loaders of DDV
*
* Revision 1.2  1999/06/28 22:07:19  durand
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
#include <vibrant.h>
#include <salfiles.h>
#include <pgppop.h>
#include <udviewer.h>
#include <ddvopen.h>
#include <ddvpanel.h>
#include <ddvmain.h>
#include <ddvgraph.h>
#include <ddvcreate.h>
#include <objmgr.h>
#include <gather.h>
#include <ddvcolor.h>
#include <samutil.h>
#include <blast.h>
#include <viewmgr.h>
#include <actutils.h>

/*extern WindoW g_hParent = NULL;*/
static void DDV_RegisterMsgFuncOnBsp(Uint2 bsp_eID,DdvMainPtr dmp);

static void DDV_Convert(ButtoN g);
static void DDV_NoConvert(ButtoN g);

/*local struct used only by the Download sequence dialog box*/
	typedef struct ddvnetopen {
		WindoW 			hWndMain;	/*main window*/
		TexT			Entry;		/*Entrez entry*/
		PopuP			AccessType;	/*database type*/
		ButtoN  		ok;			/*ok button*/
		UDVMainMenuPtr 	mmp;		/*main menu*/
	} DDVNetOpen, PNTR DDVNetOpenPtr;


/*******************************************************************************

  Function : DDV_GetSelectedRegions()
  
  Purpose :  get the selected region(s) of one bioseq.
  
  Parameters :	om_ssp;list of selected regions (usually this field points to
                  ObjMgr data)
				bsp_eID,bsp_iID; bioseq identifiers.
				    
  Return value : list of selected regions on the bioseq bsp_eID,bsp_iID

*******************************************************************************/
extern ValNodePtr DDV_GetSelectedRegions(SelStructPtr om_ssp, Uint2 bsp_eID,
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

  Function : DDV_IsLetterSelected()
  
  Purpose : check if a bsp_pos is selected (vnp_bsp is usually built with 
      DDV_GetSelectedRegions() function)
  
  Return value : TRUE if bsp_pos is selected

*******************************************************************************/
extern Boolean DDV_IsLetterSelected(ValNodePtr vnp_bsp, Int4 bsp_pos)
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
		if (bsp_pos>=bsp_start && bsp_pos<=bsp_stop){
			bSelected=TRUE;
			break;
		}
		vnp=vnp->next;
	}
	
	return(bSelected);
}

/*******************************************************************************

  Function : DDV_CheckBSPEntitiesInPgp()
  
  Purpose : check if entityID,itemID Object is in the current paragraph
  
  Return value : TRUE if success

*******************************************************************************/
/*static Boolean DDV_CheckBSPEntities(Uint4Ptr entitiesTbl,Int4 nRowBsp,
	Int4 nBsp,Uint2 eID,Uint2 iID)
{
Uint2     bsp_eID,bsp_iID;
Int4      i;
Boolean   bRet=FALSE;

	for (i=0;i<nBsp;i++){
		UDV_DecodeIdxFeat (entitiesTbl[i], &bsp_eID,&bsp_iID);
	}
	bsp=BioseqLockById(pgp->sip);
	if (bsp){
		bsp_eID=ObjMgrGetEntityIDForPointer((Pointer)bsp);
		bsp_iID = GetItemIDGivenPointer (bsp_eID, 
				OBJ_BIOSEQ, (Pointer) bsp);	
	
		if (bsp_eID==eID && bsp_iID==iID) bRet=TRUE;
		BioseqUnlock(bsp);
	}
	return(bRet);
}
*/
/*******************************************************************************

  Function : DDV_CheckBSPEntitiesInDisplay()
  
  Purpose : check if entityID,itemID Object is in the current DDV display
  
  Return value : TRUE if object found

*******************************************************************************/
static Boolean DDV_CheckBSPEntitiesInDisplay(MsaParaGPopListPtr mpplp,
		Uint2 entityID,Uint2 itemID)
{
Uint2     bsp_eID,bsp_iID;
Int4      i;
Boolean   bRet=FALSE;

	/*scan the ParaG list to find the Bioseq*/
	for (i=0;i<mpplp->nBsp;i++){
		UDV_DecodeIdxFeat (mpplp->entitiesTbl[i], &bsp_eID,&bsp_iID);
		if (bsp_eID==entityID && bsp_iID==itemID) {
			bRet=TRUE;
			break;
		}
	}
	return(bRet);
}

/*******************************************************************************

  Function : DDV_GetBspListGivenIDs()
  
  Purpose : analyse the ParaG struct and get a list of row(s) containing the
         BSP identified by eID and iID
  
  Return value : list of row number (one-based values)

*******************************************************************************/
static Int4Ptr DDV_GetBspListGivenIDs(MsaParaGPopListPtr mpplp,
		Uint2 entityID,Uint2 itemID,Int4Ptr nFound)
{
Int4       i;
Int4Ptr    pList=NULL;
Uint2     bsp_eID,bsp_iID;

	*nFound=0;
	
	/*scan the ParaG list to find all the ref. of the Bioseq
	(entityID,itemID) in the ParaG list*/
	for (i=0;i<mpplp->nBsp;i++){
		UDV_DecodeIdxFeat (mpplp->entitiesTbl[i], &bsp_eID,&bsp_iID);
		if (bsp_eID==entityID && bsp_iID==itemID) {
			if (!pList){
				pList=(Int4Ptr)MemNew(sizeof(Int4));
			}
			else{
				pList=(Int4Ptr)MemExtend((void *)pList, 
					((*nFound)+1)*sizeof(Int4), (*nFound)*sizeof(Int4));
			}
			if (!pList) {*nFound=0;return(NULL);}


			pList[*nFound]=i+1;
			(*nFound)++;
		}

	}
	return(pList);
}

/*******************************************************************************

  Function : DDV_MSG_SELECT()
  
  Purpose : manage OM_MSG_SELECT & OM_MSG_DESELECT message
  
  Return value : OM_MSG_RET_OK if success

*******************************************************************************/
static Int2 DDV_MSG_SELECT(OMMsgStructPtr ommsp,Boolean IsSelectMsg)
{
DdvMainPtr    dmp;  /*DDV panel data*/
OMUserDataPtr omudp;/*user data set when registering DDV panel*/
Int4Ptr       bsp_List=NULL;
ParaGPtr      pgp;

Boolean       bRet;
SeqLocPtr     slp;
Int4          from_col,to_col,from_row,to_row,bsp_start,bsp_stop,nBspInstances=0,i,
              row_num;

	omudp = (OMUserDataPtr)(ommsp->omuserdata);
	if (omudp == NULL) return(OM_MSG_RET_ERROR);

	dmp=(DdvMainPtr)omudp->userdata.ptrvalue;
	if (dmp == NULL) return(OM_MSG_RET_ERROR);

	if (ommsp->itemtype != OBJ_BIOSEQ) return(OM_MSG_RET_OK);
	if (ommsp->regiontype!=OM_REGION_SEQLOC) return(OM_MSG_RET_OK);

	/*Am I concern by that Bioseq ?*/
	bRet=DDV_CheckBSPEntitiesInDisplay(&(dmp->MSA_d.pgp_l),ommsp->entityID,
		ommsp->itemID);
	
	if (bRet==FALSE) return(OM_MSG_RET_OK);  /* some other viewer */
	
	/*get the disp coord range*/
	DDV_GetCurrentDispRange(dmp->hWndDDV,&(dmp->GrData),dmp->MSA_d.pgp_l.LengthAli,
			&from_col,&to_col,&from_row,&to_row);

	/*get the occurence(s) (row(s) #) for that bioseq*/
	bsp_List=DDV_GetBspListGivenIDs(&(dmp->MSA_d.pgp_l),ommsp->entityID,
		ommsp->itemID,&nBspInstances);

	if (bsp_List==NULL || nBspInstances==0) return(OM_MSG_RET_ERROR);

	/*scan the SeqLoc list to figure out if it's currently on the screen*/
	slp=(SeqLocPtr)ommsp->region;
	while(slp){/*for each SeqLoc, I try to see what region(s) has (have) to be
		uptdated*/
		for(i=0;i<nBspInstances;i++){
			row_num=bsp_List[i]-1;
			bsp_start=DDV_GetDispCoordGivenBspCoord(dmp->MSA_d.pgp_l.TableHead[row_num],
				SeqLocStart(slp));
			bsp_stop=DDV_GetDispCoordGivenBspCoord(dmp->MSA_d.pgp_l.TableHead[row_num],
				SeqLocStop(slp));
			if (bsp_start!=(Int4)-1 && bsp_stop!=(Int4)-1){
				pgp=(ParaGPtr)(dmp->MSA_d.pgp_l.TableHead[row_num]->data.ptrvalue);

				row_num=pgp->StartLine+

						(pgp->ScaleStyle==SCALE_POS_NONE ? (Int4)0 : (Int4)1);

				DDV_InvalRegion(dmp->hWndDDV,&(dmp->GrData),
					_max_(bsp_start,from_col),_min_(bsp_stop,to_col),
					row_num,IsSelectMsg);

			}
		}
		slp=slp->next;
	}
	return(OM_MSG_RET_OK);
}

/*******************************************************************************

  Function : DDV_MSG_UPDATE_CaretPos()
  
  Purpose : update the position of the caret (edit mode only)
  
  Return value : OM_MSG_RET_OK if success

*******************************************************************************/
static Int2 DDV_MSG_UPDATE_CaretPos(OMMsgStructPtr ommsp)
{
DdvMainPtr    dmp;  /*DDV panel data*/
OMUserDataPtr omudp;/*user data set when registering DDV panel*/
Boolean       bRet;
Int4          from_col,to_col,from_row,to_row;

	omudp = (OMUserDataPtr)(ommsp->omuserdata);
	if (omudp == NULL) return(OM_MSG_RET_ERROR);

	dmp=(DdvMainPtr)omudp->userdata.ptrvalue;
	if (dmp == NULL) return(OM_MSG_RET_ERROR);
	
	/*Am I concern by that Bioseq ?*/
	bRet=DDV_CheckBSPEntitiesInDisplay(&(dmp->MSA_d.pgp_l),ommsp->entityID,
		ommsp->itemID);
	
	if (bRet==FALSE) return(OM_MSG_RET_ERROR);
	
	/*get the disp coord range*/
	DDV_GetCurrentDispRange(dmp->hWndDDV,&(dmp->GrData),dmp->MSA_d.pgp_l.LengthAli,
			&from_col,&to_col,&from_row,&to_row);
	
	/*switch to one-bsed value because dmp->dci values are one-based values*/
	from_row++;
	to_row++;
	/*hide the caret from old coordinates*/
	if (dmp->dci.old_col>=from_col && dmp->dci.old_col<=to_col &&
		dmp->dci.old_row>=from_row && dmp->dci.old_row<=to_row){
		DDV_InvalRegion(dmp->hWndDDV,&(dmp->GrData),
			dmp->dci.old_col,dmp->dci.old_col+1,
			dmp->dci.old_row,FALSE);
	}
	/*show caret on new coordinates*/
	if (dmp->dci.new_col>=from_col && dmp->dci.new_col<=to_col &&
		dmp->dci.new_row>=from_row && dmp->dci.new_row<=to_row){
		DDV_InvalRegion(dmp->hWndDDV,&(dmp->GrData),
			dmp->dci.new_col,dmp->dci.new_col+1,
			dmp->dci.new_row,FALSE);
	}
    return(OM_MSG_RET_OK);
}

/*******************************************************************************

  Function : DDV_MSG_UPDATE_DelBSP()
  
  Purpose : rebuilt a display after deletion of one BSP
  
  Return value : OM_MSG_RET_OK if success

*******************************************************************************/
static Int2 DDV_MSG_UPDATE_DelBSP(OMMsgStructPtr ommsp)
{
DdvMainPtr    dmp;  /*DDV panel data*/
OMUserDataPtr omudp;/*user data set when registering DDV panel*/

	omudp = (OMUserDataPtr)(ommsp->omuserdata);
	if (omudp == NULL) return(OM_MSG_RET_ERROR);

	dmp=(DdvMainPtr)omudp->userdata.ptrvalue;
	if (dmp == NULL) return(OM_MSG_RET_ERROR);
	
    /*delete the current display*/
	if (dmp->MSA_d.pgp_l.TableHead) 
		DDV_DeleteDisplayList(&dmp->MSA_d.pgp_l);
	if (dmp->MSA_d.pgp_l.RulerDescr) 
		ValNodeFreeData(dmp->MSA_d.pgp_l.RulerDescr);
	if (dmp->MSA_d.pgp_l.entitiesTbl) 
		MemFree(dmp->MSA_d.pgp_l.entitiesTbl);
	dmp->MSA_d.pgp_l.TableHead=NULL;
	dmp->MSA_d.pgp_l.RulerDescr=NULL;
	/*rebuild a new one*/
	if (!DDV_CreateDisplayFromIndex(dmp->MSA_d.pgp_l.sap,
		&(dmp->MSA_d.pgp_l),ParaG_Size,&(dmp->ddo))) 
		return(OM_MSG_RET_ERROR);

	dmp->MSA_d.pgp_l.entitiesTbl=
		DDV_BuildBspEntitiesTbl(dmp->MSA_d.pgp_l.TableHead,
			dmp->MSA_d.pgp_l.nBsp);
	if (!dmp->MSA_d.pgp_l.entitiesTbl)
		return(OM_MSG_RET_ERROR);	
	/*build the Master Ruler descriptor*/
/*	dmp->MSA_d.pgp_l.RulerDescr=DDV_ComputeRuler(dmp->MSA_d.pgp_l.sap,&(dmp->ddo)); */
  dmp->MSA_d.pgp_l.RulerDescr = DDE_ReMakeRuler(&(dmp->MSA_d.pgp_l), FALSE, 0);

	/*delete old tables*/
    if(dmp->MasterViewer != SAMVIEWCN3D) {
        DDV_ClearColor(dmp->Globals.colorp);
        /*build new tables*/
        if (!DDV_InitColour_When_Start(dmp->MSA_d.pgp_l.sap,
            &(dmp->MSA_d.pgp_l),&(dmp->Globals.colorp), FALSE)){
            dmp->ddo.bUseColors=FALSE;
        }
    }
    
    DDV_SortPGPLineNum(dmp->MSA_d.pgp_l.TableHead,dmp->MSA_d.pgp_l.nBsp);
	DDV_WhatSize(dmp);
	/* DDV_SetupWin(dmp->hWndDDV,TRUE,NULL); */
    DDV_Resize_DDV(dmp->hWndDDV,TRUE); 
	Update();

    return(OM_MSG_RET_OK);
}
/*******************************************************************************

  Function : DDV_MSG_UPDATE_ViewMgr()
  
  Purpose : rebuild a display after update message from the ViewMgr
  
  Return value : OM_MSG_RET_OK if success

*******************************************************************************/
static Int2 DDV_MSG_UPDATE_ViewMgr(OMMsgStructPtr ommsp)
{
DdvMainPtr    dmp;  /*DDV panel data*/
OMUserDataPtr omudp;/*user data set when registering DDV panel*/
BaR           hsb;
Int4          HPos;

	omudp = (OMUserDataPtr)(ommsp->omuserdata);
	if (omudp == NULL) return(OM_MSG_RET_ERROR);

	dmp=(DdvMainPtr)omudp->userdata.ptrvalue;
	if (dmp == NULL) return(OM_MSG_RET_ERROR);

  if (dmp->bEditor) return(OM_MSG_RET_OK);
	
  /* get the current column */
  hsb = GetSlateHScrollBar((SlatE) dmp->hWndDDV);
  HPos = GetBarValue(hsb);
  
  /*delete the current display*/
	if (dmp->MSA_d.pgp_l.TableHead) 
		DDV_DeleteDisplayList(&dmp->MSA_d.pgp_l);
	if (dmp->MSA_d.pgp_l.RulerDescr) 
		ValNodeFreeData(dmp->MSA_d.pgp_l.RulerDescr);
	if (dmp->MSA_d.pgp_l.entitiesTbl) 
		MemFree(dmp->MSA_d.pgp_l.entitiesTbl);
	dmp->MSA_d.pgp_l.TableHead=NULL;
	dmp->MSA_d.pgp_l.RulerDescr=NULL;
	/*rebuild a new one*/
	if (!DDV_CreateDisplayFromIndex(dmp->MSA_d.pgp_l.sap,
		&(dmp->MSA_d.pgp_l),ParaG_Size,&(dmp->ddo))) 
		return(OM_MSG_RET_ERROR);

	dmp->MSA_d.pgp_l.entitiesTbl=
		DDV_BuildBspEntitiesTbl(dmp->MSA_d.pgp_l.TableHead,
			dmp->MSA_d.pgp_l.nBsp);
	if (!dmp->MSA_d.pgp_l.entitiesTbl)
		return(OM_MSG_RET_ERROR);

	/* build the Master Ruler descriptor */
  /* Patrick's ComputeRuler isn't working right, so I substituted mine -- Dave H. */
  /* dmp->MSA_d.pgp_l.RulerDescr=DDV_ComputeRuler(dmp->MSA_d.pgp_l.sap,&(dmp->ddo)); */
  dmp->MSA_d.pgp_l.RulerDescr = DDE_ReMakeRuler(&(dmp->MSA_d.pgp_l), FALSE, 0);

	/*delete old tables*/
    if(dmp->MasterViewer != SAMVIEWCN3D) {
        DDV_ClearColor(dmp->Globals.colorp);
        /*build new tables*/
        if (!DDV_InitColour_When_Start(dmp->MSA_d.pgp_l.sap,
            &(dmp->MSA_d.pgp_l),&(dmp->Globals.colorp), FALSE)){
            dmp->ddo.bUseColors=FALSE;
        }
    }
    
  DDV_SortPGPLineNum(dmp->MSA_d.pgp_l.TableHead,dmp->MSA_d.pgp_l.nBsp);
	DDV_WhatSize(dmp);
	DDV_Resize_DDV(dmp->hWndDDV,TRUE); /* DDV_SetupWin(dmp->hWndDDV,TRUE,NULL); */

  /* put col back to where it was */
  SetValue(hsb, HPos);

	Update();

    return(OM_MSG_RET_OK);
}

/*******************************************************************************

  Function : DDV_MSG_UPDATE_Layout()
  
  Purpose : manage the OM_MSG_UPDATE message
  
  Return value : OM_MSG_RET_OK if success

*******************************************************************************/
static Int2 DDV_MSG_UPDATE_Layout(PaneL hWndDDV, DDVUpdateLayoutDataPtr dumdp)
{
DdvMainPtr  dmp;
Int4        j;
Boolean     bResetScrolls=FALSE,
			bRebuildDisplay=FALSE,
			bSwitchColors=FALSE,
			bUpdateColors=FALSE;
Uint1       oldSpacerSize;

	if (!hWndDDV || !dumdp)
		return(OM_MSG_RET_OK);
	
	dmp = (DdvMainPtr) GetObjectExtra(hWndDDV);
	
	if (!dmp)
		return(OM_MSG_RET_OK);

	if (!Visible(hWndDDV))
		return(OM_MSG_RET_OK);
	
	oldSpacerSize=dmp->ddo.SpacerSize;
	dmp->ddo.SpacerSize=dumdp->SpacerSize;

	/*rebuild the display, if needed*/	
	if ((dmp->ddo.DispDiscStyle!=dumdp->DispDiscStyle) ||
		(oldSpacerSize!=dumdp->SpacerSize) ||
		(dmp->ddo.DiscJustification!=dumdp->DiscJustification) ||
		(dmp->ddo.ShowLeftTail!=dumdp->ShowLeftTail)||
		(dmp->ddo.ShowRightTail!=dumdp->ShowRightTail)){
		
		if((dmp->ddo.ShowLeftTail!=dumdp->ShowLeftTail)||
			(dmp->ddo.ShowRightTail!=dumdp->ShowRightTail)||
			(dmp->ddo.DispDiscStyle!=dumdp->DispDiscStyle)){
			bUpdateColors=TRUE;
		}
		/*update the styles*/
		dmp->ddo.DispDiscStyle=dumdp->DispDiscStyle;
		dmp->ddo.DiscJustification=dumdp->DiscJustification;
		dmp->ddo.ShowLeftTail=dumdp->ShowLeftTail;
		dmp->ddo.ShowRightTail=dumdp->ShowRightTail;
		
		/*delete the current display*/
		if (dmp->MSA_d.pgp_l.TableHead) 
			DDV_DeleteDisplayList(&dmp->MSA_d.pgp_l);
		if (dmp->MSA_d.pgp_l.RulerDescr) 
			ValNodeFreeData(dmp->MSA_d.pgp_l.RulerDescr);
		if (dmp->MSA_d.pgp_l.entitiesTbl) 
			MemFree(dmp->MSA_d.pgp_l.entitiesTbl);

		dmp->MSA_d.pgp_l.TableHead=NULL;
		dmp->MSA_d.pgp_l.RulerDescr=NULL;
		/*rebuild a new one*/
		if (!DDV_CreateDisplayFromIndex(dmp->MSA_d.pgp_l.sap,
			&(dmp->MSA_d.pgp_l),ParaG_Size,&(dmp->ddo))) 
			return(OM_MSG_RET_ERROR);

		dmp->MSA_d.pgp_l.entitiesTbl=
			DDV_BuildBspEntitiesTbl(dmp->MSA_d.pgp_l.TableHead,
				dmp->MSA_d.pgp_l.nBsp);
		if (!dmp->MSA_d.pgp_l.entitiesTbl)
			return(OM_MSG_RET_ERROR);	
		/*build the Master Ruler descriptor*/
/*		dmp->MSA_d.pgp_l.RulerDescr=DDV_ComputeRuler(dmp->MSA_d.pgp_l.sap,&(dmp->ddo));  */
    dmp->MSA_d.pgp_l.RulerDescr = DDE_ReMakeRuler(&(dmp->MSA_d.pgp_l), FALSE, 0);

		/*init the colours*/
		if(bUpdateColors){
            if(dmp->MasterViewer != SAMVIEWCN3D ) {
                /*delete old tables*/
                DDV_ClearColor(dmp->Globals.colorp);
                /*build new tables*/
                if (!DDV_InitColour_When_Start(dmp->MSA_d.pgp_l.sap,
                    &(dmp->MSA_d.pgp_l),&(dmp->Globals.colorp),FALSE)){
                    dmp->ddo.bUseColors=FALSE;
                }
            }
        }
        bResetScrolls=TRUE;
		bRebuildDisplay=TRUE;
	}

	/*set the ruler styles*/
	if(dumdp->nSeq && dumdp->SeqList){
		for(j=0;j<dumdp->nSeq;j++){
			switch(dumdp->RulerStyle){
				case SCALE_POS_TOP:
					DDV_SetRulerAttribInPGP(dmp->MSA_d.pgp_l.TableHead[dumdp->SeqList[j]-1], 
							SCALE_POS_TOP);
					break;
				case SCALE_POS_NONE:
					DDV_SetRulerAttribInPGP(dmp->MSA_d.pgp_l.TableHead[dumdp->SeqList[j]-1], 
							SCALE_POS_NONE);
					break;
				default:
					DDV_SetRulerAttribInPGP(dmp->MSA_d.pgp_l.TableHead[dumdp->SeqList[j]-1], 
							SCALE_POS_NONE);
					break;
			}			
		}
		bRebuildDisplay=TRUE;
	}

	/*colors*/
	if(dmp->ddo.bUseColors!=dumdp->bUseColors){
		dmp->ddo.bUseColors=dumdp->bUseColors;
		bSwitchColors=TRUE;
	}

	/*redraw if needed*/
	if (bRebuildDisplay){
	    DDV_SortPGPLineNum(dmp->MSA_d.pgp_l.TableHead,dmp->MSA_d.pgp_l.nBsp);
	    DDV_WhatSize(dmp);
	    DDV_Resize_DDV(hWndDDV,bResetScrolls); /* DDV_SetupWin(hWndDDV,bResetScrolls,NULL);*/
	    Update();
	}
	else if (bSwitchColors){
		RecT   rcP;
		WindoW temport;
		
		temport=SavePort(ParentWindow(hWndDDV));
		Select(hWndDDV);
		
		ObjectRect(hWndDDV,&rcP);
		InvalRect(&rcP);
		RestorePort(temport);
		/*Update();*/
	}


	return(OM_MSG_RET_OK);
}

/*******************************************************************************

  Function : DDV_MSG_FLUSH()
  
  Purpose : kill the viewer in response to a OM_MSG_FLUSH message
  
  Return value : OM_MSG_RET_OK if success

*******************************************************************************/
static Int2 DDV_MSG_FLUSH(OMMsgStructPtr ommsp)
{
DdvMainPtr    dmp;  /*DDV panel data*/
OMUserDataPtr omudp;/*user data set when registering DDV panel*/
/*ParaGPtr pgp;
BioseqPtr bsp;
Int4 i;
Uint2 bsp_eID;*/


	omudp = (OMUserDataPtr)(ommsp->omuserdata);
	if (omudp == NULL) return(OM_MSG_RET_ERROR);

	dmp=(DdvMainPtr)omudp->userdata.ptrvalue;
	if (dmp == NULL) return(OM_MSG_RET_ERROR);
	
    if (dmp->MSA_d.entityID==ommsp->entityID &&
        ((dmp->MSA_d.itemID==ommsp->itemID && ommsp->itemtype==OBJ_SEQALIGN)
        ||(ommsp->itemID == 0 && ommsp->itemtype==0))){
        Remove(dmp->hParent);
        DDV_CleanupDDVPdata_g(dmp);
       /* for(i=0;i<dmp->MSA_d.pgp_l.nBsp;i++){
            pgp=(ParaGPtr)(dmp->MSA_d.pgp_l.TableHead[i]->data.ptrvalue);
            bsp=BioseqLockById(pgp->sip);
            if (bsp != NULL){
                bsp_eID=ObjMgrGetEntityIDForPointer((Pointer)bsp);
                BioseqUnlock(bsp);
                if (bsp_eID>0 && bsp_eID!=dmp->MSA_d.entityID){
                    ObjMgrFreeUserData(bsp_eID,dmp->procid,dmp->proctype,dmp->userkey);
                }
            }
        }
        
        ObjMgrFreeUserData(dmp->MSA_d.entityID,dmp->procid,dmp->proctype,dmp->userkey);*/
    }
	return(OM_MSG_RET_OK);
}

/*******************************************************************************

  Function : DDV_SimpleUpdateAnswer()
  
  Purpose : simple answer to an Update Msg; just redraw the window
  
  Parameters : see Toolkit
  
  Return value : see Toolkit 

*******************************************************************************/
static void DDV_SimpleUpdateAnswer(OMMsgStructPtr ommsp)
{		            
RecT   rcP;
WindoW temport;
DdvMainPtr    dmp;  /*DDV panel data*/
OMUserDataPtr omudp;/*user data set when registering DDV panel*/

	omudp = (OMUserDataPtr)(ommsp->omuserdata);
	if (omudp == NULL) return;

	dmp=(DdvMainPtr)omudp->userdata.ptrvalue;
	if (dmp == NULL) return;
	
	temport=SavePort(ParentWindow(dmp->hWndDDV));
	Select(dmp->hWndDDV);
	
	ObjectRect(dmp->hWndDDV,&rcP);
	InvalRect(&rcP);
	RestorePort(temport);
}

/*******************************************************************************

  Function : DDV_UpdateRegMsgFunc()
  
  Purpose : register a MsgCallback in response to an OM_MSG_UPDATE, after 
          SeqImport
  
*******************************************************************************/
static void DDV_UpdateRegMsgFunc(OMMsgStructPtr ommsp,Uint2 bsp_eID)
{
DdvMainPtr    dmp;  /*DDV panel data*/
OMUserDataPtr omudp;/*user data set when registering DDV panel*/

	omudp = (OMUserDataPtr)(ommsp->omuserdata);
	if (omudp == NULL) return;

	dmp=(DdvMainPtr)omudp->userdata.ptrvalue;
	if (dmp == NULL) return;

	DDV_RegisterMsgFuncOnBsp(bsp_eID,dmp);		
}

/*******************************************************************************

  Function : DDV_OM_MsgFunc()
  
  Purpose : ObjMgr message loop of the DDV viewer/editor
  
  Parameters : see Toolkit
  
  Return value : see Toolkit 

*******************************************************************************/
static Int2 LIBCALLBACK DDV_OM_MsgFunc (OMMsgStructPtr ommsp)
{
Int2 nRet=OM_MSG_RET_OK;
  
	switch (ommsp->message)
	{
		case OM_MSG_DEL:
			break;
		case OM_MSG_CREATE:
			break;
		case OM_MSG_UPDATE:{
			DDVUpdateMSGPtr        dump;
			
			dump = (DDVUpdateMSGPtr)(ommsp->procmsgdata);
			
			/*generic UPDATE msg*/
			if (!dump){
				DDV_SimpleUpdateAnswer(ommsp);
				break;
			}

			/*specific UPDATE msg*/
            switch(dump->type){
                case UPDATE_TYPE_LAYOUT:{
                    DDVUpdateLayoutDataPtr dumdp;
                
                    dumdp=(DDVUpdateLayoutDataPtr)dump->data;
                
                    if (!dumdp)
                        break;
                
                    nRet=DDV_MSG_UPDATE_Layout(dumdp->ddv_panel,dumdp);
                    break;
                }
                case UPDATE_TYPE_EDIT_DELBSP:
                    nRet=DDV_MSG_UPDATE_DelBSP(ommsp);
                    break;
                case UPDATE_TYPE_VIEWMGR:
                    nRet=DDV_MSG_UPDATE_ViewMgr(ommsp);
                    break;
				case UPDATE_TYPE_CARETPOS:
					nRet=DDV_MSG_UPDATE_CaretPos(ommsp);
					break;
                case UPDATE_TYPE_NEWSEQ:
					if (dump->data)
						DDV_UpdateRegMsgFunc(ommsp,*((Uint2 *)dump->data));
					break;
				default :
					DDV_SimpleUpdateAnswer(ommsp);
            }
			
			break;
		}
		case OM_MSG_SELECT:
			nRet=DDV_MSG_SELECT(ommsp,TRUE);
			break;
		case OM_MSG_DESELECT:
			nRet=DDV_MSG_SELECT(ommsp,FALSE);
			break;
		case OM_MSG_CACHED:
			break;
		case OM_MSG_UNCACHED:
			break;
		case OM_MSG_TO_CLIPBOARD:
			break;
		case OM_MSG_SETCOLOR:
			break;
		case OM_MSG_FLUSH:
			DDV_MSG_FLUSH(ommsp);
			break;
		default:
			break;
	}

	return (nRet);
}

/*******************************************************************************

  Function : DDV_RegisterMsgFuncOnBsp()
  
  Purpose : register a MsgFunc for a bsp. 
  
*******************************************************************************/
static void DDV_RegisterMsgFuncOnBsp(Uint2 bsp_eID,DdvMainPtr dmp)
{
OMUserDataPtr omudp;

	if (bsp_eID==0 || dmp==NULL) return;
	
    omudp = ObjMgrAddUserData (bsp_eID,dmp->procid,dmp->proctype,dmp->userkey);
    if (omudp != NULL) {
    	omudp->messagefunc = DDV_OM_MsgFunc;
		omudp->userdata.ptrvalue = (Pointer)dmp;
	}
}

/*******************************************************************************

  Function : DDV_RegMsgFuncForBsp()
  
  Purpose : given the display data structure (nBsp & TableHead), register a
            MsgFunc callback for each bioseq of a SeqAlign.
  
  Parameters : TableHead; entry point of the display data structure (ParaG list)
			   nBsp; number of sequences in the SeqAlign
			   master_eID; entityID of the object containing the SeqAlign
			   procID; identifier of the viewer (usually DDV)
			   userkey; userkey of the viewer (usually DDV)
			   
  Return value : -

*******************************************************************************/
static void DDV_RegMsgFuncForBsp(ValNodePtr PNTR TableHead,DdvMainPtr dmp,Int4 nBsp, 
	Uint2 master_eID, Uint2 procID, Uint2 procType, Uint2 userkey)
{
OMUserDataPtr omudp,omudp_tmp;
ParaGPtr      pgp;
BioseqPtr     bsp;
Int4          i;
Boolean       bFound;
Uint2         bsp_eID;

	if (!TableHead || nBsp==0 || master_eID==0 || procID==0 
		|| procType==0 ||userkey==0) return;
		
	for(i=0;i<nBsp;i++){
		pgp=(ParaGPtr)(dmp->MSA_d.pgp_l.TableHead[i]->data.ptrvalue);
		bsp=BioseqLockById(pgp->sip);
		if (bsp){
			bsp_eID=ObjMgrGetEntityIDForPointer((Pointer)bsp);
			if (bsp_eID>0 && bsp_eID!=master_eID){
				omudp=ObjMgrGetUserData(bsp_eID,procID,procType,userkey);
				if (omudp){
					/*scan the user data associated with that bioseq and try to
					find if a Msg Func is already attached for DDV*/
					omudp_tmp=omudp;
					bFound=FALSE;
					while(omudp_tmp){
						if (omudp_tmp->procid==procID && 
							omudp_tmp->proctype==procType && 
							omudp_tmp->userkey==userkey && 
							omudp_tmp->messagefunc==DDV_OM_MsgFunc){
							
							bFound=TRUE;
							break;
						}
						omudp_tmp=omudp_tmp->next;
					}
				}
				else{/*add the Msg Callaback and DDV main data block to the bioseq*/
					bFound=FALSE;
				}
				if (bFound==FALSE){
					DDV_RegisterMsgFuncOnBsp(bsp_eID,dmp);
				}
			}
			BioseqUnlock(bsp);
		}
	}
}

static void DDV_Convert(ButtoN g) {
/*----------------------------------------------------------------------------
*  just need to quit the pop-up window
*---------------------------------------------------------------------------*/
  WindoW  hDialog;

	hDialog = (WindoW)ParentWindow(g);
	if (!hDialog) return;
	Remove(hDialog);
  return;
}


static void DDV_NoConvert(ButtoN g) {
/*----------------------------------------------------------------------------
*  quit the pop-up window and close the file
*---------------------------------------------------------------------------*/
  WindoW         hDialog;
  DdvMainPtr     dmp;

	hDialog = (WindoW)ParentWindow(g);
	if (!hDialog) return;
	Remove(hDialog);

  dmp = (DdvMainPtr) GetObjectExtra(hDialog);
  DDV_FileCloseIt(dmp->hParent);
  return;
}


/*******************************************************************************

  Function : DDV_StartPanel_Slave()
  
  Purpose : start DDV main window as a slave module: init 'dmp' data structure
  			(if needed), create the DDV panel, prepare a display from 'sap'.
  
  Parameters : 	sap; pointer to a SeqAlign
  				input_entityID,input_itemID; identity of the SeqAlign
  				dmp; main data of DDV. May be NULL
				hParent; parent window for DDV. Must not be NULL
  				bSetAppProp; if TRUE, use SetAppProperty().
				bInitGraph; if TRUE, initialize the graphical data for DDV
				bEditor; if TRUE, start the editor mode. Otherwise, a simple
						viewer will ba started
                rcPp; the RecT that defines the size of the panel.  can be NULL.
						
  Note :  This function assumes that DDV will be placed inside an existing 
     parent window provided by the host application.
	 
  Return value : handle to the DDV panel

*******************************************************************************/
static Boolean DDV_StartPanel_Slave(DdvMainPtr dmp, SeqAlignPtr sap,
		Uint2* pInput_entityID, Uint2* pInput_itemID, Boolean bEditor)
{
DDV_ColorGlobal * dcgp;
MsaParaGPopListPtr  mpplp;
BaR  hsb;
Int4  HPos, Col, IsEditable;
DdvMainWinPtr  mWin_d;
Boolean*   pNoGaps;
Boolean    NoGaps;
char       Str[1024];
MsgAnswer  Answer;
Boolean    Attached = FALSE;
  
  WatchCursor();

  if (!dmp->bEditor) {
    mWin_d = (DdvMainWinPtr) GetObjectExtra(dmp->hParent);
    Enable(mWin_d->MainMenu.LaunchEditor);
    IsEditable = AlnMgrIsEditable(sap);
    if (!IsEditable) {
      if (AlnMgrIsIBMable(sap)) {
        Str[0] = 0;
        StringCat(Str, "This file is a multiple pairwise alignment.\n");
        StringCat(Str, "To edit this file, it needs to be converted\n");
        StringCat(Str, "to a true multiple alignment.\n");
        StringCat(Str, "Should it be converted?");
        mWin_d->Show_logo=TRUE;
        Answer = Message(MSG_YN, "%s", Str);
        mWin_d->Show_logo=FALSE;
        if (Answer == ANS_YES) {
          if (ViewMgr_Attach(sap, TRUE, TRUE, *pInput_entityID, *pInput_itemID) < 1)
            return (FALSE);
          Attached = TRUE;
        }
        else {
          Disable(mWin_d->MainMenu.LaunchEditor);
        }
      }
    }
    else if (IsEditable == AM_EDITGAPS) {
      pNoGaps = (Boolean*) GetAppProperty("dde_nogaps");
      if (pNoGaps == NULL) NoGaps = TRUE;
      else NoGaps = *pNoGaps;
      if (NoGaps) {
        Str[0] = 0;
        StringCat(Str, "This alignment can't be edited\n");
        StringCat(Str, "because it has aligned gaps\n");
        StringCat(Str, "and DDV is running in NoGaps mode");
        Message(MSG_OK, "%s", Str);
        Disable(mWin_d->MainMenu.LaunchEditor);
      }
    }
  }

  if (!Attached) {
    if (ViewMgr_Attach(sap, FALSE, FALSE, *pInput_entityID, *pInput_itemID) < 1)
      return (FALSE);
  }
	
	/*if we have a NULL SeqAlign, switch the display style to show
	the sequences, not a spacer*/
	if (AlnMgrIsSAPNULL(sap)){
		dmp->ddo.DispDiscStyle=MSA_TXT_STYLE_2;
		dmp->ddo.SpacerSize=SPACER_TXT_BLANK;
    	dmp->ddo.DiscJustification=DISP_JUST_LEFT;
	}

  if (bEditor) {
    dmp->dsp = (DDE_StackPtr) GetAppProperty("ddeinterndata");
    if (dmp->dsp == NULL) {return(FALSE);}
    mpplp = dmp->dsp->pEdit->pPopList;
    /* copy paragraphs from DDE_Stack into the ones that are displayed */
    MemCopy(&(dmp->MSA_d.pgp_l),mpplp,sizeof(MsaParaGPopList));
  }
  else {
	  if (!DDV_CreateDisplayFromIndex(sap,&(dmp->MSA_d.pgp_l),ParaG_Size,
		  &(dmp->ddo))) return(FALSE);
	  dmp->MSA_d.pgp_l.entitiesTbl=
		  DDV_BuildBspEntitiesTbl(dmp->MSA_d.pgp_l.TableHead,
		  dmp->MSA_d.pgp_l.nBsp);
	  if (!dmp->MSA_d.pgp_l.entitiesTbl) return(FALSE);
	  /*build the Master Ruler descriptor*/
/*	  dmp->MSA_d.pgp_l.RulerDescr=DDV_ComputeRuler(sap,&(dmp->ddo));  */
    dmp->MSA_d.pgp_l.RulerDescr = DDE_ReMakeRuler(&(dmp->MSA_d.pgp_l), FALSE, 0);
  }

	dmp->MSA_d.entityID=*pInput_entityID;
	dmp->MSA_d.itemID=*pInput_itemID;

/*
#if defined(_DEBUG_DDE)
{
  MsaParaGPopListPtr  mpplp;
  mpplp = DDV_CreateDataForEditor(&(dmp->MSA_d.pgp_l), 0, dmp->MSA_d.pgp_l.LengthAli-1);
	MemCopy(&(dmp->MSA_d.pgp_l),mpplp,sizeof(MsaParaGPopList));
  dmp->dsp = DDE_NewStack(mpplp);
}
#endif
*/

	/*get the color stuff from the sap object. Usually I can do that
	when DDV is loaded from Cn3D*/
	dcgp=DDV_GetColorGlobal((Pointer) sap);

	/*init the colours*/
	if (dcgp==NULL){
		if (!DDV_InitColour_When_Start(dmp->MSA_d.pgp_l.sap,&(dmp->MSA_d.pgp_l),
			&(dmp->Globals.colorp),FALSE)){
			dmp->ddo.bUseColors=FALSE;
		}
	}
    else dmp->Globals.colorp = dcgp;        

	/*set the initial position of the caret (edit mode)*/
	dmp->dci.old_row=1;
	dmp->dci.new_row=1;
	dmp->dci.old_col=0;
	dmp->dci.new_col=0;
	

	/*relation between align size and display type; compute nTotLines for example*/
	DDV_WhatSize(dmp);

	ArrowCursor();
	DDV_Resize_DDV(dmp->hWndDDV,TRUE); /*DDV_SetupWin (dmp->hWndDDV,TRUE,NULL); */
	/*Update();*/

  CaptureSlateFocus((SlatE)dmp->hWndDDV);

  /* scroll display horizontally so aligned region is shown at position 5 */
  if (DDE_GetNumBlocks2(&(dmp->MSA_d.pgp_l))) {
    hsb = GetSlateHScrollBar((SlatE) dmp->hWndDDV);
    HPos = GetBarValue(hsb);
    if (bEditor) {
      Col = DDE_GetAlignStart(dmp->dsp->pEdit, 0);
    }
    else {
      Col = 0;
      if (dmp->ddo.ShowLeftTail) {
        Col = AlnMgrGetMaxTailLength(sap, LEFT_TAIL);
      }
    }
    SetValue(hsb, HPos+Col-5);
  }

  /* only enable left/right/center justify for an unaligned region in the editor */
  if (bEditor) {
    mWin_d = (DdvMainWinPtr) GetObjectExtra(dmp->hParent);
    if (dmp->dsp->IsUnAligned) {
      Enable(mWin_d->MainMenu.Justify);
    }
    else {
      Disable(mWin_d->MainMenu.Justify);
    }
  }

	return(TRUE);
}


/*******************************************************************************

  Function : DDV_ObjRegMasterDDV()
  
  Purpose : call by ObjMgr to start DDV panel
  
  Parameters : see Toolkit
  
  Note : this callback is only used by the standalone DDV. Never use this
         callback for other purpose.
  
  Return value : see Toolkit 

*******************************************************************************/
static Int2 DDV_ObjRegMasterDDV (Pointer data,Boolean bEditor)
{
OMProcControlPtr    ompcp;
SeqAlignPtr			sap=NULL;
OMUserDataPtr       omudp;
DdvMainPtr          dmp;

	/*retrieve data*/
	ompcp = (OMProcControlPtr) data;

	if (ompcp == NULL || ompcp->proc == NULL) {
		ErrPostEx (SEV_ERROR, 0, 0, "Data NULL [1]");
		return OM_MSG_RET_ERROR;
	}
	/*only accept a SeqALign*/
	switch (ompcp->input_itemtype) {
		case OBJ_SEQALIGN :
			sap = (SeqAlignPtr) ompcp->input_data;
			break;
		default :
			return OM_MSG_RET_ERROR;
	}
	/*oups... nothing to deal with*/
	if (sap == NULL) {
		ErrPostEx (SEV_ERROR, 0, 0, "Data NULL [2]");
		return OM_MSG_RET_ERROR;
	}
	
	dmp = (DdvMainPtr) GetAppProperty("ddvinterndata");
	if (!dmp) goto error;
	if (!DDV_StartPanel_Slave(dmp, sap, &ompcp->input_entityID,
		&ompcp->input_itemID, bEditor)) goto error;
	
	
	/*attach a Msg Func on the current seqalign*/
	dmp->userkey=OMGetNextUserKey();
	dmp->procid=ompcp->proc->procid;
	dmp->proctype=ompcp->proc->proctype;
    omudp = ObjMgrAddUserData (ompcp->input_entityID, ompcp->proc->procid, 
		(bEditor==TRUE ? OMPROC_EDIT : OMPROC_VIEW),dmp->userkey);
    if (omudp == NULL) 
		goto error;
    omudp->messagefunc =  DDV_OM_MsgFunc;
	omudp->userdata.ptrvalue = (Pointer)dmp;

	/*attach a Msg Func on each Bioseq of the current seqalign*/
	DDV_RegMsgFuncForBsp(dmp->MSA_d.pgp_l.TableHead,dmp,dmp->MSA_d.pgp_l.nBsp, 
		ompcp->input_entityID, dmp->procid,dmp->proctype,dmp->userkey);

	DDV_EnableGotoTBItems(dmp->hParent,TRUE);
	return(OM_MSG_RET_DONE);

error:
	/*show the logo window*/
	/*mWin_d->Show_logo=TRUE;*/
	Update ();
	ArrowCursor();
	return(OM_MSG_RET_ERROR);
}

/*******************************************************************************

  Function : DDV_ObjRegMasterEditDDV()
  
  Purpose : call by ObjMgr to start DDV as a standalone editor
  
  Parameters : see Toolkit
  
  Return value : see Toolkit 

*******************************************************************************/
extern Int2 LIBCALLBACK DDV_ObjRegMasterEditDDV (Pointer data)
{
	return(DDV_ObjRegMasterDDV(data,TRUE));
}

/*******************************************************************************

  Function : DDV_ObjRegMasterViewDDV()
  
  Purpose : call by ObjMgr to start DDV as a standalone viewer
  
  Parameters : see Toolkit
  
  Return value : see Toolkit 

*******************************************************************************/
extern Int2 LIBCALLBACK DDV_ObjRegMasterViewDDV (Pointer data)
{
	return(DDV_ObjRegMasterDDV(data,FALSE));
}


/*******************************************************************************

  Function : DDV_ObjRegSlaveDDV()
  
  Purpose : call by DDV_ObjRegSlaveEditDDV() & DDV_ObjRegSlaveViewDDV(), below
  
*******************************************************************************/
static Int2 DDV_ObjRegSlaveDDV(Pointer data,Boolean bEditor)
{
OMProcControlPtr    ompcp;
SeqAlignPtr			sap=NULL;
WindoW              hParent=NULL;
DdvMainWinPtr 		mWin_d;
OMUserDataPtr       omudp = NULL;
DdvMainPtr          dmp;
SAM_ViewGlobal      *vgp;


	/*retrieve data*/
	ompcp = (OMProcControlPtr) data;
    if (ompcp == NULL || ompcp->proc == NULL) {
		ErrPostEx (SEV_ERROR, 0, 0, "Data NULL [1]");
		return OM_MSG_RET_ERROR;
	}

	/*only accept a SeqALign*/
	switch (ompcp->input_itemtype) {
		case OBJ_SEQALIGN :
			sap = (SeqAlignPtr) ompcp->input_data;
			break;
		default :
			return OM_MSG_RET_ERROR;
	}

	/*oups... nothing to deal with*/
	if (sap == NULL) {
		ErrPostEx (SEV_ERROR, 0, 0, "Data NULL [2]");
		return OM_MSG_RET_ERROR;
	}
        
	/*start the slave module*/
    /* get parameters from launching program */
    vgp = GetAppProperty(SAM_ViewString);
    if(vgp == NULL) hParent=DDV_StartMainWin_Slave(NULL,bEditor);
    else hParent=DDV_StartMainWin_Slave(vgp,bEditor);

	if (hParent){
		RealizeWindow(hParent);
        Show(hParent);
        mWin_d=(DdvMainWinPtr)GetObjectExtra(hParent);
        dmp = (DdvMainPtr) GetObjectExtra(mWin_d->hWndDDV);
        if(vgp != NULL) dmp->MasterViewer = vgp->MasterViewer;

        if(dmp->MasterViewer == SAMVIEWCN3D) {
            mWin_d->UseNetwork = TRUE;
            mWin_d->NetStartProc = vgp->NetStartProc;
        }
        
        if (!DDV_StartPanel_Slave(dmp, sap, &ompcp->input_entityID,
            &ompcp->input_itemID, bEditor)) 
            goto error;
        
		/*when the user will close DDV, DDV won't delete any data excepts
		its internal data structure*/
		dmp->dod.choice=DDV_OPENTYPE_NOTRESP;
		
		/*attach a Msg Func on the current seqalign*/
		dmp->userkey=OMGetNextUserKey();
		dmp->procid=ompcp->proc->procid;
		dmp->proctype=ompcp->proc->proctype;
    	omudp = ObjMgrAddUserData (ompcp->input_entityID, ompcp->proc->procid, 
			(bEditor==TRUE ? OMPROC_EDIT : OMPROC_VIEW),dmp->userkey);
    	if (omudp == NULL) 
			goto error;
    	omudp->messagefunc =  DDV_OM_MsgFunc;
		omudp->userdata.ptrvalue = (Pointer)dmp;
		/*attach a Msg Func on each Bioseq of the current seqalign*/
		DDV_RegMsgFuncForBsp(dmp->MSA_d.pgp_l.TableHead,dmp,dmp->MSA_d.pgp_l.nBsp, 
			ompcp->input_entityID, dmp->procid,dmp->proctype,dmp->userkey);
		DDV_EnableGotoTBItems(hParent,TRUE);
	} 
	else {
		goto error;
	}
	
	return(OM_MSG_RET_DONE);

error:
	if (hParent) Remove(hParent);
	return(OM_MSG_RET_ERROR);
}

/*******************************************************************************

  Function : DDV_ObjRegSlaveEditDDV()
  
  Purpose : call by ObjMgr to start DDV as a slave, editor mode on a SeqAlign
  
  Parameters : see Toolkit
  
  Return value : see Toolkit 

*******************************************************************************/
extern Int2 LIBCALLBACK DDV_ObjRegSlaveEditDDV (Pointer data)
{
	return(DDV_ObjRegSlaveDDV(data,TRUE));
}

/*******************************************************************************

  Function : DDV_ObjRegSlaveViewDDV()
  
  Purpose : call by ObjMgr to start DDV as a slave, editor mode on a SeqAlign
  
  Parameters : see Toolkit
  
  Return value : see Toolkit 

*******************************************************************************/
extern Int2 LIBCALLBACK DDV_ObjRegSlaveViewDDV (Pointer data)
{
	return(DDV_ObjRegSlaveDDV(data,FALSE));
}

/*******************************************************************************

  Function : DDV_GetAndCheckSeqAlign()
  
  Purpose : given a file OR a GI OR a SeqEntry , retrieve SeqAlign(s) 
  
  Parameters :  fp; a handle of an open file
  				gi; a GI numger
				sep2; a SeqEntry
				fetchSepProc; fetch procedure of the user (only if GI!=0)
				
  Return value : a list of nodes. For each node, data.intvalue is an entityID
           of a SeqAlign 

*******************************************************************************/
extern ValNodePtr DDV_GetAndCheckSeqAlign(FILE *fp,Int4 gi,SeqEntryPtr sep2,
	UdvFetchSeqEntryProc fetchSepProc,DdvOpenDataPtr dodp,Uint2Ptr entityID)
{
SeqEntryPtr sep=NULL;
ValNodePtr  head = NULL,vnp_ali=NULL;
Pointer     dataptr=NULL;
Uint2       datatype=0;
Uint2		nRet=DVV_MSG_M_OK;

	*entityID=0;
	if (fp!=NULL){/*open a file*/
		/*Read the file (it can contain several objects)*/
		while ((dataptr = ReadAsnFastaOrFlatFile (fp, &datatype, NULL, FALSE, 
			FALSE, TRUE, FALSE)) != NULL) {
				ValNodeAddPointer (&head, datatype, dataptr);
		}
		/*scan the first node of the head list*/
		datatype = head->choice;
		dataptr = head->data.ptrvalue;
        *entityID=ObjMgrRegister(datatype,dataptr);
		AssignIDsInEntity (*entityID,0,NULL);
		switch(datatype){
			case OBJ_SEQENTRY:
			case OBJ_SEQALIGN:
			case OBJ_SEQANNOT:
			case OBJ_BIOSEQ:
			case OBJ_BIOSEQSET:
				DDV_GetSeqAlign(dataptr,datatype,&vnp_ali);
				break;
			default:
				break;
		}
		if (dodp){
			dodp->choice=DDV_OPENTYPE_FILE;
			dodp->vnp=head;
		}
	} else if (gi!=0){/*retrieve a Network Entry given a GI*/
		if (fetchSepProc) {/*fetch the SeqEntry*/
			sep=fetchSepProc(gi, 0);
			if (!sep) {
				nRet=DVV_MSG_O_E_READGI;
				goto error;
			}
			*entityID=ObjMgrGetEntityIDForPointer((Pointer)sep);
			AssignIDsInEntity (*entityID,0,NULL);
			SeqEntryExplore(sep,(Pointer)&vnp_ali,DDV_SearchAli);
			if (dodp){
				dodp->choice=DDV_OPENTYPE_GI;
				dodp->sep=sep;
			}
		} else {
			nRet=DVV_MSG_O_E_NOFETCHFUNC;
			goto error;
		}
	} else if (sep2!=NULL){/*analyse a SeqEntry*/
		*entityID=ObjMgrGetEntityIDForPointer((Pointer)sep2);
		AssignIDsInEntity (*entityID,0,NULL);
		SeqEntryExplore(sep2,(Pointer)&vnp_ali,DDV_SearchAli);
		if (dodp){
			dodp->choice=DDV_OPENTYPE_SEP;
			dodp->sep=sep2;
		}
	} else {
		nRet=DVV_MSG_O_E_NOTHINGTODO;
		goto error;
	}
	
	/*register for ObjMgr*/
	if (!vnp_ali) {
		nRet=DVV_MSG_O_E_BADTYPE;
		goto error;
	}

	
	return(vnp_ali);

error:
	/*to add : Msg Handler with nRet*/
	return(NULL);
}

/*******************************************************************************

  Function : DDV_FOpenTextProc()
  
  Purpose : manage File name edit control of the FileOpen dialog box 
  
  Parameters : t; edit control
  
  Return value : none 

*******************************************************************************/
static void DDV_FOpenTextProc(TexT t)
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

  Function : DDV_FOpenBrowseProc()
  
  Purpose : manage browse button of the FileOpen dialog box 
  
  Parameters : g; button
  
  Return value : none 

*******************************************************************************/
static void DDV_FOpenBrowseProc(ButtoN g)
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
		DDV_FOpenTextProc(dfodp->FNameEditCtrl);
	}

	return;   
}

/*******************************************************************************

  Function : DDV_FOpenCancelProc()
  
  Purpose : manage cancel button of the FileOpen dialog box 
  
  Parameters : g; button
  
  Return value : none 

*******************************************************************************/
static void DDV_FOpenCancelProc(ButtoN g)
{
WindoW				hOpenDlg;
DlgFileOpenDataPtr  dfodp;
DdvMainWinPtr 		mWin_d;

	hOpenDlg=(WindoW)ParentWindow(g);

	if (!hOpenDlg) return;
	dfodp = (DlgFileOpenDataPtr) GetObjectExtra (hOpenDlg);
	
	if (dfodp){
		mWin_d = (DdvMainWinPtr) GetObjectExtra (dfodp->parent);
		if (mWin_d) Enable(mWin_d->MainMenu.FileOpen);
	}
	
	Remove(hOpenDlg);
}

/*******************************************************************************

  Function : DDV_FOpenAcceptProc()
  
  Purpose : manage ok button of the FileOpen dialog box 
  
  Parameters : g; button
  
  Return value : none 

*******************************************************************************/
static void DDV_FOpenAcceptProc(ButtoN g)
{
WindoW				hOpenDlg;
DlgFileOpenDataPtr  dfodp;
DdvMainPtr          dmp;
DdvMainWinPtr 		mWin_d;
Char 				szFName[PATH_MAX]={""};
Boolean				isBinary;
FILE				*fp;
ValNodePtr 			vnp_ali=NULL;
Uint2 				nRet=DVV_MSG_M_OK,The_entityID;
Int2				procval;

	hOpenDlg=(WindoW)ParentWindow(g);

	if (!hOpenDlg) return;
	dfodp = (DlgFileOpenDataPtr) GetObjectExtra (hOpenDlg);
	
	if (dfodp){

		mWin_d = (DdvMainWinPtr) GetObjectExtra (dfodp->parent);
		if (mWin_d!=NULL){
			/*try to connect Entrez; if failure, still ok : the user can
			use DDV with a local file. Otherwise he/she will be in trouble ;-)*/
			if (mWin_d->AutonomeViewer && mWin_d->NetStartProc){
				mWin_d->NetStartProc(mWin_d->UseNetwork);
			}

			Enable(mWin_d->MainMenu.FileOpen);
			/*file name*/
			GetTitle(dfodp->FNameEditCtrl, szFName, sizeof(szFName));

			/*file reading mode*/
			if (GetValue(dfodp->ReadMode)==2) isBinary=TRUE;
			else isBinary=FALSE;
	
			Remove(hOpenDlg);

			/* open the i/o files in the correct mode */
			if ((fp = FileOpen (szFName, isBinary?"rb":"r")) == NULL){
				nRet=DVV_MSG_O_E_OPENFILEFAIL;
				goto error;
			}
			/*restrieve a list of registered seqalign(s)*/
			WatchCursor();
			/*delete old data, if needed*/
			dmp=(DdvMainPtr)GetObjectExtra(mWin_d->hWndDDV);
			DDV_CleanupDDVPdata_g(dmp);
			DDV_CloseData(mWin_d,FALSE);
			
			vnp_ali=DDV_GetAndCheckSeqAlign(fp,0,NULL,mWin_d->fetchSepProc,&(dmp->dod),
				&The_entityID);
			ArrowCursor();
			if (vnp_ali && The_entityID!=0){
				Uint2 entityID,itemID;
				SeqAlignPtr sap;
								
				/*get the first SeqAlign in the list and load the viewer panel*/
				dmp->vnp_ali=vnp_ali;
				
				if(mWin_d->Show_logo){
					mWin_d->Show_logo=FALSE;
				}
				sap=(SeqAlignPtr)vnp_ali->data.ptrvalue;
				entityID=sap->idx.entityID;
				itemID=sap->idx.itemID;
				SetAppProperty("ddvinterndata",(void *)dmp);
				procval=GatherProcLaunch ((dmp->bEditor==TRUE? OMPROC_EDIT : OMPROC_VIEW), FALSE, 
					entityID, itemID, OBJ_SEQALIGN, 0, 0, OBJ_SEQALIGN, 0);
				if ((procval = OM_MSG_RET_ERROR))
					mWin_d->Show_logo=TRUE;
				RemoveAppProperty("ddvinterndata");
			}
			FileClose(fp);
		}
	}
	
error:	
	return;
}

/*******************************************************************************

  Function : DDV_OpenFile()
  
  Purpose : callback of the File|Open command 
  
  Parameters : i; command item
  
  Return value : none 

*******************************************************************************/
extern void DDV_OpenFile(IteM i)
{
DlgFileOpenDataPtr  dfodp;
DdvMainWinPtr 		mWin_d;
WindoW				hWinMain,hOpenDlg;
GrouP				g,g4;
ButtoN				b1;
TexT				t1;    

	hWinMain=(WindoW)ParentWindow(i);

	if (!hWinMain) return;
	
	mWin_d = (DdvMainWinPtr) GetObjectExtra (hWinMain);

	if (mWin_d==NULL) return;
		
	dfodp=(DlgFileOpenDataPtr)MemNew(sizeof(DlgFileOpenData));
	if (!dfodp) return;
	MemSet(dfodp,0,sizeof(DlgFileOpenData));

    hOpenDlg = MovableModalWindow(-30, -20,  -10,  -10, 
				"DDV - Open a local file",  NULL);
    g = NormalGroup(hOpenDlg, 2, 1, "File name:",  systemFont, NULL);
    SetGroupMargins(g, 10, 10);
    SetGroupSpacing(g, 10, 20);  
    t1 = DialogText(g,"",25, DDV_FOpenTextProc);
    PushButton(g, " browse...", DDV_FOpenBrowseProc);
   
    g = HiddenGroup(hOpenDlg, 3, 1, NULL);
    SetGroupMargins(g, 10, 10);
    SetGroupSpacing(g, 30, 30);
 
    g4 = NormalGroup(g, 2, 1, "File type", systemFont,  NULL);
    SetGroupMargins(g4, 10, 10);
    RadioButton(g4, "Ascii");
    RadioButton(g4, "Binary");
    SetValue(g4, 1);

    b1 = DefaultButton(g, "OK",  DDV_FOpenAcceptProc);
    PushButton(g, "Cancel",  DDV_FOpenCancelProc);
   
    Disable(b1);

	dfodp->parent=hWinMain;	
	dfodp->FNameEditCtrl=t1;
	dfodp->Ok=b1;
	dfodp->ReadMode=g4;

	SetObjectExtra (hOpenDlg, (Pointer) dfodp, StdCleanupExtraProc);
	
    Select(hOpenDlg);
    Show(hOpenDlg);

	/*allow only one FOpen dlg at a time*/
    /*UDV_set_PullMenus(&vmp->MainMenu,FALSE);*/
}

/*******************************************************************************

  Function : DDV_NetOpen_cancelProc()
  
  Purpose : manage ok button of the Network open dialog box 
  
  Parameters : g; button
  
  Return value : none 

*******************************************************************************/
static void DDV_NetOpen_cancelProc(ButtoN g)
{
WindoW			hOpenDlg;
DDVNetOpenPtr 	dnop;

	hOpenDlg=(WindoW)ParentWindow(g);

	if (!hOpenDlg) return;
	
	dnop = (DDVNetOpenPtr) GetObjectExtra (hOpenDlg);

	if (dnop==NULL) return;

	Remove(hOpenDlg);
}

/*******************************************************************************

  Function : DDV_IsInt()
  
  Purpose : test if an entry access code is a GI number, i.e. an Int
  
  Parameters : t; edit control
  
  Return value : none 

*******************************************************************************/
static Boolean DDV_IsInt(CharPtr szAccess)
{
Int2 i=0;

	while(szAccess[i]){
		if (!IS_DIGIT(szAccess[i])){
			Message (MSG_OK, "Not a GI number.");
			return(FALSE);
		}
		i++;
	}
	return(TRUE);
}

/*******************************************************************************

  Function : DDV_NetOpen_okProc()
  
  Purpose : manage ok button of the Network open dialog box 
  
  Parameters : g; button
  
  Return value : none 

*******************************************************************************/
static void DDV_NetOpen_okProc(ButtoN g)
{
WindoW			hOpenDlg;
DDVNetOpenPtr 	dnop;
Char 			szAccess[50]={""};
Int2			AccessType,procval;
Int4			uid=0;
DdvMainWinPtr 	mWin_d;
ValNodePtr      vnp_ali;
MonitorPtr      mon;
DdvMainPtr      dmp;
Uint2           The_entityID;

	hOpenDlg=(WindoW)ParentWindow(g);

	if (!hOpenDlg) return;
	
	dnop = (DDVNetOpenPtr) GetObjectExtra (hOpenDlg);	

	if (dnop==NULL) return;

	mWin_d = (DdvMainWinPtr) GetObjectExtra (dnop->hWndMain);
	if (mWin_d==NULL) return;

	dmp=(DdvMainPtr)GetObjectExtra(mWin_d->hWndDDV);	
	
	Hide(hOpenDlg);
	
	/*retrieve a Gi*/
	GetTitle(dnop->Entry, szAccess, sizeof(szAccess));

	if (StringHasNoText (szAccess)) {
		Message (MSG_OK, "Please enter a GI number");
		Show (hOpenDlg);
		Select (hOpenDlg);
		Select (dnop->Entry);
		return;
	}

	AccessType=GetValue(dnop->AccessType);
	if (!StrToLong(szAccess,&uid)) 
		uid=0;
	/*Connect ID1 with a valid UID*/
	if (uid==0){
		ArrowCursor();
		Message (MSG_OK, "Unable to find your record in the database.");
		Show (hOpenDlg);
		Select (hOpenDlg);
		Select (dnop->Entry);
		return;
	}
	else{
		/*delete old data, if needed*/
		DDV_CleanupDDVPdata_g(dmp);
		DDV_CloseData(mWin_d,FALSE);
		
		WatchCursor();

		mon = MonitorStrNewEx (szAppName, 30, FALSE);
		MonitorStrValue (mon, "Retrieving your record...");
		Update ();

		vnp_ali=DDV_GetAndCheckSeqAlign(NULL,uid,NULL,mWin_d->fetchSepProc,
			&(dmp->dod),&The_entityID);

		ArrowCursor();

		MonitorFree (mon);

		if (vnp_ali && The_entityID!=0){
			Uint2 entityID,itemID;
			SeqAlignPtr sap;
			
			/*get the first SeqAlign in the list and load the viewer panel*/
			dmp->vnp_ali=vnp_ali;

			if(mWin_d->Show_logo){
				/*Hide(mWin_d->Logo_Panel);*/
				mWin_d->Show_logo=FALSE;
			}
			sap=(SeqAlignPtr)vnp_ali->data.ptrvalue;
			entityID=sap->idx.entityID;
			itemID=sap->idx.itemID;
			SetAppProperty("ddvinterndata",(void *)dmp);
			procval=GatherProcLaunch ((dmp->bEditor==TRUE? OMPROC_EDIT : OMPROC_VIEW), FALSE, 
				entityID, itemID, OBJ_SEQALIGN, 0, 0, OBJ_SEQALIGN, 0);
			if ((procval = OM_MSG_RET_ERROR))
				mWin_d->Show_logo=TRUE;
			RemoveAppProperty("ddvinterndata");
		}
	}

	Remove(hOpenDlg);
}

/*******************************************************************************

  Function : DDV_NetOpen_access()
  
  Purpose : manage Accession edit control of the Download dialog box 
  
  Parameters : t; edit control
  
  Return value : none 

*******************************************************************************/
static void DDV_NetOpen_access(TexT t)
{
Char 			szAccess[50]={""};
WindoW			hOpenDlg;
DDVNetOpenPtr 	dnop;

	hOpenDlg=(WindoW)ParentWindow(t);

	if (!hOpenDlg) return;
	
	dnop = (DDVNetOpenPtr) GetObjectExtra (hOpenDlg);

	if (dnop==NULL) return;
	
	GetTitle(t, szAccess, sizeof(szAccess));

	if (StringLen(szAccess) == 0)
		Disable(dnop->ok);
	else Enable(dnop->ok);

	return;
}

/*******************************************************************************

  Function : DDV_OpenNetwork()
  
  Purpose : callback of the File|Network command 
  
  Parameters : i; command item
  
  Return value : none 

*******************************************************************************/
extern void DDV_OpenNetwork(IteM i)
{
DdvMainWinPtr 		mWin_d;
WindoW				hWinMain,hOpenDlg;
GrouP				g,c;
DDVNetOpenPtr       dnopp;

	hWinMain=(WindoW)ParentWindow(i);

	if (!hWinMain) return;
	
	mWin_d = (DdvMainWinPtr) GetObjectExtra (hWinMain);

	if (mWin_d==NULL) return;
		
	dnopp=(DDVNetOpenPtr)MemNew(sizeof(DDVNetOpen));
	if (!dnopp) return;

	/*init the Network, if needed*/
	if (mWin_d->AutonomeViewer && mWin_d->NetStartProc){
		if (!mWin_d->NetStartProc(mWin_d->UseNetwork)) return;
	}

    hOpenDlg = MovableModalWindow(-30, -20,  -10,  -10, 
				"DDV - Fetch a network entry",  NULL);
	SetGroupSpacing (hOpenDlg, 10, 10);
	
	dnopp->hWndMain=hWinMain;
	
	/*accesion*/
	g = NormalGroup (hOpenDlg, 3, 0, "Entry", systemFont,NULL);

#ifdef WIN_MAC
	dnopp->AccessType = PopupList (g, TRUE, NULL);
#endif

#ifndef WIN_MAC
	dnopp->AccessType = PopupList (g, FALSE, NULL);
#endif

	PopupItem(dnopp->AccessType,"PopSet");
	
	SetValue (dnopp->AccessType, 1);

	dnopp->Entry = DialogText (g, "", 10, DDV_NetOpen_access);

	c = HiddenGroup (hOpenDlg, 4, 0, NULL);
	SetGroupSpacing (c, 10, 2);
	dnopp->ok=DefaultButton (c, "Retrieve", DDV_NetOpen_okProc);
	Disable (dnopp->ok);
	PushButton (c, "Cancel", DDV_NetOpen_cancelProc);
	
	SetObjectExtra (hOpenDlg, (Pointer) dnopp, StdCleanupExtraProc);

	/*display*/
	AlignObjects (ALIGN_CENTER, (HANDLE) g, (HANDLE) c, NULL);
	RealizeWindow (hOpenDlg);
	Show (hOpenDlg);
	Select (hOpenDlg);
	Update ();
}

NLM_EXTERN void DDV_SlaveQuit(WindoW hWinMain)
{
  DdvMainWinPtr  mWin_d;
  DdvMainPtr     dmp;
  WindoW				 hDialog;
  GrouP          g1, g2;
  Boolean        QuitElseWhere = FALSE;

	mWin_d = (DdvMainWinPtr) GetObjectExtra((Handle)hWinMain);
	if (mWin_d==NULL) return;

  /* if there are unsaved edits, ask the user if they'd like to save */
  dmp = (DdvMainPtr)GetObjectExtra(mWin_d->hWndDDV);
  if (dmp->bEditor) {
    if (dmp->dsp->SomethingToSave) {
      QuitElseWhere = TRUE;
      hDialog = MovableModalWindow(-30, -20,  -10,  -10, "DDE - Save Query",  NULL);
      g1 = HiddenGroup(hDialog, 1, 1, NULL);
      StaticPrompt(g1, "Save Changes?", 0, stdLineHeight, systemFont, 'c');
      g2 = HiddenGroup(hDialog, 3, 1, NULL);
      PushButton(g2, "Yes", DDV_Save);
      PushButton(g2, "No",  DDV_NoSave);
      PushButton(g2, "Cancel", DDV_Cancel);
      SetObjectExtra (hDialog, (Pointer)hWinMain, NULL);
      Select(hDialog);
      Show(hDialog);
    }
  }
  if (!QuitElseWhere) {
    if (mWin_d->AutonomeViewer)
      QuitProgram(); /*standalone DDV*/
    else
      Remove(hWinMain);/*slave viewer : just kill the main window*/
  }
}

/*******************************************************************************

  Function : DDV_StartMainWin_Slave()
  
  Purpose : start DDV main window as a slave module: init 'dmp' data structure,
  			set 'DDV_MAIN_DATA' property, start a dlg (the parent window of DDV
			panel).
  
  Parameters : vgp; sets defaults for non-autonomous viewers
							 
  Return value : handle to the DDV main window

*******************************************************************************/
extern WindoW DDV_StartMainWin_Slave(SAM_ViewGlobal *vgp,Boolean bEditor)
{
Int2	Margins;
DdvMainWinPtr mWin_d;
WindoW	w; 
Boolean bRet;
Char AppName[40];

  /* use "DDE" for editor, "DDV" for viewer, for title */
  if (bEditor) {StringCpy(AppName, szAppName2);}
  else         {StringCpy(AppName, szAppName);}
	
	/*init data blocks*/
	mWin_d=(DdvMainWinPtr)MemNew(sizeof(DdvMainWin));
	if (!mWin_d) return(NULL);
	
	/*main window*/
	Margins=15*stdCharWidth;
    if(vgp == NULL)
        w=DocumentWindow(Margins,Margins ,-10, -10,
           AppName, 
           DDV_SlaveQuit,
           DDV_WinMainResize);
    else
        w=DocumentWindow(vgp->Rect.left,vgp->Rect.top ,-10, -10,
            AppName, 
            DDV_SlaveQuit,
            DDV_WinMainResize);

	if (!w) return(NULL);

	SetObjectExtra (w, (Pointer) mWin_d, (FreeProc)DDV_WinMainCleanup);
	mWin_d->hWndMain=w;
	/*this is a slave editor/viewer*/
	mWin_d->AutonomeViewer=FALSE;

	/*build GUI*/
	bRet=DDV_CreateViewerPanel(w,mWin_d, vgp, bEditor);

	if (!bRet) return(NULL);

	mWin_d->Show_logo=FALSE;

	return(w);
}


static Boolean DDV_DigitsOnly(Char* str)
{
    while(*str) {
        if (!isdigit(*str)) 
            return FALSE;
        str++;
    }
    return TRUE;
}

/*******************************************************************************

  Function : DDV_ImportEnableProc
  
  Purpose : this function does the actual bioseq import.  note that it doesn't
            require a vibrant dialog to operate, only that idp is filled out
  
  Return value : none 

*******************************************************************************/

static void DDV_ImportEnableProc(TexT t)
{
    Char str[32];
    WindoW hImportDlg;
    DDV_ImportDialog *idp;

	hImportDlg= (WindoW)ParentWindow(t);
	if(hImportDlg == NULL) return;	
	idp = (DDV_ImportDialog *) GetObjectExtra(hImportDlg);
	if(idp == NULL) return;

    GetTitle(idp->DDV_tAccession, str, sizeof(str));
    if (StringLen(str) == 0) {
        Disable(idp->DDV_bImportAccept);
    } else {
        Enable(idp->DDV_bImportAccept);
        if(DDV_DigitsOnly(str)) SetValue(idp->DDV_gAccType, 2);
        else SetValue(idp->DDV_gAccType, 1);
    }
    return;
}

static SeqAlignPtr ddv_truncate_overlaps (SeqAlignPtr sap)
{
   AMAlignIndexPtr  amaip;
   DenseDiagPtr     ddp;
   DenseDiagPtr     ddp_head;
   DenseDiagPtr     ddp_prev;
   DenseSegPtr      dsp1;
   DenseSegPtr      dsp2;
   Int4             i;
   SeqAlignPtr      salp;
   Int4             start1;
   Int4             start2;
   Int4             stop1;
   Int4             stop2;
   Int4             tmp;

   if (sap == NULL) return(NULL);
   if (sap->saip == NULL) return(NULL);
   if (sap->saip->indextype != INDEX_PARENT) return(NULL);

   amaip = (AMAlignIndexPtr)(sap->saip);
   for (i=0; i<amaip->numsaps-1; i++)
   {
      AlnMgrGetNthSeqRangeInSA(amaip->saps[i], 1, &start1, &stop1);
      AlnMgrGetNthSeqRangeInSA(amaip->saps[i+1], 1, &start2, &stop2);
      if (start2 <= stop1)
      {
         dsp1 = (DenseSegPtr)(amaip->saps[i]->segs);
         dsp2 = (DenseSegPtr)(amaip->saps[i+1]->segs);
         dsp1->lens[0] = dsp1->lens[0] - (stop1-start2)/2;
         tmp = dsp1->starts[0]+dsp1->lens[0];
         tmp = tmp - dsp2->starts[0];
         dsp2->starts[1] = tmp + dsp2->starts[1];
         dsp2->starts[0] = dsp1->starts[0]+dsp1->lens[0];
         dsp2->lens[0] = stop2 - dsp2->starts[0] + 1;
         SAIndexFree(amaip->saps[i]->saip);
         SAIndexFree(amaip->saps[i+1]->saip);
         amaip->saps[i]->saip = amaip->saps[i+1]->saip = NULL;
         AlnMgrIndexSingleChildSeqAlign(amaip->saps[i]);
         AlnMgrIndexSingleChildSeqAlign(amaip->saps[i+1]);
      }
      AlnMgrGetNthSeqRangeInSA(amaip->saps[i], 2, &start1, &stop1);
      AlnMgrGetNthSeqRangeInSA(amaip->saps[i+1], 2, &start2, &stop2);
      if (start2 <= stop1)
      {
         dsp1 = (DenseSegPtr)(amaip->saps[i]->segs);
         dsp2 = (DenseSegPtr)(amaip->saps[i+1]->segs);
         dsp1->lens[0] = dsp1->lens[0] - (stop1-start2)/2;
         tmp = dsp1->starts[1]+dsp1->lens[0];
         tmp = tmp - dsp2->starts[1];
         dsp2->starts[0] = tmp + dsp2->starts[0];
         dsp2->starts[1] = dsp1->starts[1]+dsp1->lens[0];
         dsp2->lens[0] = stop2 - dsp2->starts[1] + 1;
         SAIndexFree(amaip->saps[i]->saip);
         SAIndexFree(amaip->saps[i+1]->saip);
         amaip->saps[i]->saip = amaip->saps[i+1]->saip = NULL;
         AlnMgrIndexSingleChildSeqAlign(amaip->saps[i]);
         AlnMgrIndexSingleChildSeqAlign(amaip->saps[i+1]);
      }
   }
   ddp_head = ddp_prev = NULL;
   for (i=0; i<amaip->numsaps; i++)
   {
      ddp = DenseDiagNew();
      AlnMgrGetNthSeqRangeInSA(amaip->saps[i], 1, &start1, &stop1);
      AlnMgrGetNthSeqRangeInSA(amaip->saps[i], 2, &start2, &stop2);
      dsp1 = (DenseSegPtr)(amaip->saps[i]->segs);
      ddp->starts = (Int4Ptr)MemNew(2*sizeof(Int4));
      ddp->dim = 2;
      ddp->id = dsp1->ids;
      dsp1->ids = NULL;
      ddp->starts[0] = start1;
      ddp->starts[1] = start2;
      ddp->len = stop1 - start1 + 1;
      if (ddp_head != NULL)
      {
         ddp_prev->next = ddp;
         ddp_prev = ddp;
      } else
         ddp_head = ddp_prev = ddp;
   }
   salp = SeqAlignNew();
   salp->dim = 2;
   salp->segtype = SAS_DENDIAG;
   salp->type = SAT_DIAGS;
   salp->segs = (Pointer)(ddp_head);
   SeqAlignFree(sap);
   return salp;
}

NLM_EXTERN SeqAlignPtr DDV_ShredAln(SeqAlignPtr sap)
{
   AMAlignIndexPtr  amaip;
   AlnMsgPtr        amp1;
   AlnMsgPtr        amp2;
   DenseDiagPtr     ddp;
   DenseDiagPtr     ddp_head;
   DenseDiagPtr     ddp_prev;
   Int4             i;
   Boolean          more1;
   Boolean          more2;
   SeqAlignPtr      salp;

   if (sap == NULL || sap->saip == NULL || sap->saip->indextype != INDEX_PARENT)
      return NULL;
   amaip = (AMAlignIndexPtr)(sap->saip);
   if (amaip->numrows > 2)
      return NULL;
   amp1 = AlnMsgNew();
   amp2 = AlnMsgNew();
   ddp_head = ddp_prev = NULL;
   for (i=0; i<amaip->numsaps; i++)
   {
      amp1 = AlnMsgReNew(amp1);
      amp2 = AlnMsgReNew(amp2);
      amp1->from_m = amp2->from_m = 0;
      amp1->to_m = amp2->to_m = -1;
      amp1->row_num = 1;
      amp2->row_num = 2;
      while ((more1 = AlnMgrGetNextAlnBit(amaip->saps[i], amp1)) && (more2 = AlnMgrGetNextAlnBit(amaip->saps[i], amp2)))
      {
         if (amp1->gap == 0 && amp2->gap == 0)
         {
            ddp = DenseDiagNew();
            ddp->dim = 2;
            ddp->starts = (Int4Ptr)MemNew(2*sizeof(Int4));
            ddp->strands = (Uint1Ptr)MemNew(2*sizeof(Uint1));
            ddp->id = AlnMgrGetNthSeqIdPtr(sap, 1);
            ddp->id->next = AlnMgrGetNthSeqIdPtr(sap, 2);
            ddp->starts[0] = amp1->from_b;
            ddp->starts[1] = amp2->from_b;
            ddp->len = amp1->to_b - amp1->from_b + 1;
            ddp->strands[0] = AlnMgrGetNthStrand(sap, 1);
            ddp->strands[1] = AlnMgrGetNthStrand(sap, 2);
            if (ddp_head != NULL)
            {
               ddp_prev->next = ddp;
               ddp_prev = ddp;
            } else 
               ddp_head = ddp_prev = ddp;
         }
      }
   }
   salp = SeqAlignNew();
   salp->type = SAT_DIAGS;
   salp->segtype = SAS_DENDIAG;
   salp->dim = 2;
   salp->segs = (Pointer)(ddp_head);
   SeqAlignFree(sap);
   AlnMsgFree(amp1);
   AlnMsgFree(amp2);
   return salp;
}

/*******************************************************************************

  Function : DDV_DoAlign
  
  Purpose : this function does the actual bioseq import.  note that it doesn't
            require a vibrant dialog to operate, only that idp is filled out
  
  Return value : none 

*******************************************************************************/

NLM_EXTERN void DDV_DoAlign(DDV_ImportDialog *idp)
{
    Char str[32];
    Bioseq *bsp1 = NULL, *bsp2 = NULL;
    SeqId si;
    SeqAlign *sap, *salp;
    Int4 value, i;
    ValNode *pvn;
    ErrSev sev;
    
    if(idp->mode & DDVIMP2SE) {
        value = GetValue(idp->DDV_lsip); /*one-base value*/
        for(pvn = idp->pvnSips, i = 1; i != value && pvn != NULL;
            i++, pvn = pvn->next) continue;
        if(pvn != NULL)
            bsp1 = BioseqLockById((SeqId *)pvn->data.ptrvalue);
    } else bsp1 = BioseqLockById(idp->sip);
    if (bsp1 == NULL) {
        Message (MSG_ERROR, "Unable to load sequence");
        goto out2;
    }
    
    if(idp->mode & DDVIMPNET) {
        si.choice = SEQID_GI;
        GetTitle(idp->DDV_tAccession, str, sizeof(str));
        if(GetValue(idp->DDV_gAccType) == 1 && idp->AccessionCB)
            si.data.intvalue = idp->AccessionCB(str, ISA_aa(bsp1->mol));
        else si.data.intvalue = atoi(str);
        if (si.data.intvalue <= 0) {
            Message (MSG_ERROR, "Unable to understand accession");
            goto out2;
        }
        bsp2 = BioseqLockById(&si);
    } else bsp2 = BioseqLockById((SeqId *)idp->sipslave->data.ptrvalue);
    if(bsp2 == NULL) {
        Message (MSG_ERROR, "Unable to load sequence");
        BioseqUnlock(bsp1);
        goto out2;
    }
    idp->bsp = bsp2;
    
    if(ISA_aa(bsp1->mol) != ISA_aa(bsp2->mol) ||
        ISA_na(bsp1->mol) != ISA_na(bsp1->mol)) {
        Message (MSG_ERROR, "Sequences to be aligned not of same type");
        BioseqUnlock(bsp1);
        BioseqUnlock(bsp2);
        goto out2;
    }

    sev = ErrSetMessageLevel(SEV_FATAL);
    
/*    sap = ACT_GlobalAlignSimple(bsp1, bsp2, TRUE); */
    if(ISA_aa(bsp1->mol))
        sap = DDV_Blast2Seqs(bsp1, bsp2, idp->Gap, "blastp");
    else 
        sap = DDV_Blast2Seqs(bsp1, bsp2, idp->Gap, "blastn");

    ErrSetMessageLevel(sev);
    BioseqUnlock(bsp1);
    BioseqUnlock(bsp2);
    
    if(sap == NULL) {
        Message (MSG_OK, "Not able to align the sequence");
        goto out2;
    }
    
    SAM_ReplaceGI(sap);
    AlnMgrIndexSeqAlign(sap);

    if(idp->Gap) {
        AlnMgrMakeMultipleByScore(sap);
        AlnMgrDeleteHidden(sap, FALSE);
        salp = DDV_ShredAln(sap);
    } else {
        AlnMgrMakeMultipleByScoreEx(sap, 10);
        AlnMgrDeleteHidden(sap, FALSE);
        salp = ddv_truncate_overlaps(sap);
    }

    if(idp->callback) idp->callback(idp, idp->sap, salp);
    /*SeqAlignFree(sap);*/
    
out2:
    MemFree(idp->userdata);
    ValNodeFree(idp->pvnSips); /* don't free the data */
    ValNodeFree(idp->sipslave);
}

/*******************************************************************************

  Function : DDV_ImportAcceptProc
  
  Purpose : fired by clicking OK in the bioseq import dialog
  
  Return value : none 

*******************************************************************************/

static void DDV_ImportAcceptProc(ButtoN b)
{
    WindoW hImportDlg;
    DDV_ImportDialog *idp;

	hImportDlg= (WindoW)ParentWindow(b);
	if(hImportDlg == NULL) goto out;	
	idp = (DDV_ImportDialog *) GetObjectExtra(hImportDlg);
	if(idp == NULL) goto out;

    WatchCursor();
    DDV_DoAlign(idp);
    ArrowCursor();

out:
    Remove(hImportDlg);
    return;
}

/*******************************************************************************

  Function : DDV_ImportCB
  
  Purpose : callback for import bioseq dialog.  used to clean up after the
            import
  
  Return value : none 

*******************************************************************************/

NLM_EXTERN void DDV_ImportCB(DDV_ImportDialog *idp, SeqAlign *salpdest,
                             SeqAlign *salp)
{
    if (salpdest == NULL || salp == NULL) return;
    SeqAlignSetFree(salp->next);
    salp->next = NULL;
    ViewMgr_Add(salpdest, salp);
}

/*******************************************************************************

  Function : DDV_ImportCancelProc
  
  Purpose : cancel function for the import bioseq dialog
  
  Return value : none 

*******************************************************************************/

static void DDV_ImportCancelProc(ButtoN b)
{
    WindoW hImportDlg;
    DDV_ImportDialog *idp;

	hImportDlg= (WindoW)ParentWindow(b);
	if(hImportDlg == NULL) return;	
	idp = (DDV_ImportDialog *) GetObjectExtra(hImportDlg);
	if(idp == NULL) return;
    MemFree(idp->userdata);

    Remove(idp->DDV_wImport);
    return;
}

/*******************************************************************************

  Function : DDV_ImportBioseqDlg
  
  Purpose : dialog to import a bioseq into a seqalign.  Note that it has
            several modes listed in ddvopen.h
  
  Return value : none 

*******************************************************************************/

NLM_EXTERN void DDV_ImportBioseqDlg(DDV_ImportDialog *idp)
{
    GrouP g, hg;
    ButtoN b;
    DDVUpdateMSG  *dump;
    ValNode *pvn;
    Char szName[21]={""};

    if(idp == NULL) return;
    
    /* check if we have the right data for each mode */
    if(idp->mode == DDVIMPSE2SA 
        && (idp->sip == NULL || idp->sap == NULL || idp->sipslave == NULL)) return;
    else if (idp->mode == DDVIMPSE2SE 
        && (idp->pvnSips == NULL || idp->sipslave == NULL)) return;
    else if (idp->mode == DDVIMPNET2SA 
        && (idp->sap == NULL || idp->sip == NULL)) return;
    else if (idp->mode == DDVIMPNET2SE  
        && idp->pvnSips == NULL) return;

    idp->DDV_wImport = NULL;
    idp->DDV_gAccType = NULL;
    idp->DDV_bImportAccept = NULL;
    idp->DDV_tAccession = NULL;
    dump = MemNew(sizeof(DDVUpdateMSG));
    if(dump == NULL) {
        MemFree(idp);
        return;
    }
    idp->userdata = dump;
    dump->data = NULL;
	dump->type=UPDATE_TYPE_EDIT_DELBSP;

    idp->DDV_wImport = MovableModalWindow(-30, -20, -10, -10,
        "Import sequence using BLAST", NULL);
    Nlm_SetObjectExtra (idp->DDV_wImport, (Nlm_VoidPtr)idp, StdCleanupExtraProc);

    if(idp->mode & DDVIMP2SE) {
        hg = HiddenGroup(idp->DDV_wImport, 1, 4, NULL);
        g = NormalGroup(hg, 1, 2, "Choose sequence to align to:", systemFont, NULL);
        idp->DDV_lsip = SingleList(g,10,6,NULL);

        for(pvn = idp->pvnSips; pvn != NULL; pvn = pvn->next) {
            SeqIdWrite((SeqId *)pvn->data.ptrvalue,szName,
                PRINTID_TEXTID_ACCESSION, 10);
            ListItem(idp->DDV_lsip, szName);
        }
        SetValue(idp->DDV_lsip, 1);
    } else hg = HiddenGroup(idp->DDV_wImport, 1, 3, NULL);

    if(idp->mode & DDVIMPNET) {
        g = NormalGroup(hg, 1, 2, " Enter sequence identifier:", systemFont, NULL);
        idp->DDV_tAccession = DialogText(g, "", 10, (TxtActnProc) DDV_ImportEnableProc);
        idp->DDV_gAccType =
            NormalGroup(hg, 1, 2, " accession type", systemFont, NULL);
        RadioButton(idp->DDV_gAccType, "Accession");
        RadioButton(idp->DDV_gAccType, "gi");
    }

    g = HiddenGroup(hg, 2, 1, NULL);
    SetGroupSpacing(g, 15, 15);
    idp->DDV_bImportAccept =
        DefaultButton(g, "OK", (BtnActnProc) DDV_ImportAcceptProc);
    b = PushButton(g, "Cancel", (BtnActnProc) DDV_ImportCancelProc);

    if(idp->mode & DDVIMPNET) {
        SetValue(idp->DDV_gAccType, 1);
        Disable(idp->DDV_bImportAccept);
        Select(idp->DDV_tAccession);
    }
    Show(idp->DDV_wImport);
    return;
}

/*******************************************************************************

  Function : DDV_ImportBioseq
  
  Purpose : sets up dialog to import a bioseq into a seqalign
  
  Return value : none 

*******************************************************************************/

NLM_EXTERN void DDV_ImportBioseq(IteM i)
{
    DDV_ImportDialog *idp;
    DdvMainPtr    dmp;  /*DDV panel data*/
    DdvMainWinPtr mWin_d;
    WindoW		  hWinMain;

    idp = MemNew(sizeof(DDV_ImportDialog));
    if(idp == NULL) return;

	hWinMain=(WindoW)ParentWindow(i);
	if (hWinMain==NULL) return;
	mWin_d = (DdvMainWinPtr) GetObjectExtra (hWinMain);
	if (mWin_d==NULL) return;

    if (mWin_d->NetStartProc) mWin_d->NetStartProc(mWin_d->UseNetwork);

	if (mWin_d->hWndDDV==NULL) return;
	dmp = (DdvMainPtr) GetObjectExtra(mWin_d->hWndDDV);
	if (dmp==NULL) return;

    if(dmp->MSA_d.pgp_l.sap == NULL) return;

    idp->sap = dmp->MSA_d.pgp_l.sap;
    idp->sip = AlnMgrFindMaster(idp->sap);
    if(idp->sip == NULL) {
        Message (MSG_ERROR, "Alignment must have master sequence");
        MemFree(idp);
        return;
    }

    idp->callback = DDV_ImportCB;
    idp->mode = DDVIMPNET2SA;

    DDV_ImportBioseqDlg(idp);
}


NLM_EXTERN SeqAlign *DDV_Blast2Seqs(Bioseq *bsp1, Bioseq *bsp2, Boolean gapped,
                         Char *progname)
{
    BLAST_OptionsBlkPtr options;
    SeqAlign  *salp;
    
    if(bsp1 == NULL || bsp2 == NULL || progname == NULL) return NULL;
    options = BLASTOptionNew(progname, gapped);
    if(options == NULL) return NULL;
/*    options->discontinuous = FALSE; */
    salp = BlastTwoSequences(bsp1, bsp2, progname, options);
    BLASTOptionDelete(options);
    return salp;
}


/*******************************************************************************

  Function : DDV_ImportSeqAlign(), DDV_ImportNucSeqAlign() and DDV_ImportProtSeqAlign()
  
  Purpose : import a sequence alignment of text type
  
  Return value : none 

*******************************************************************************/
static void DDV_ImportSeqAlign(Boolean is_prot,IteM i)
{
WindoW        hWinMain;
DdvMainPtr    dmp;
DdvMainWinPtr mWin_d;
SeqEntryPtr   sep;
ValNodePtr    vnp_ali;
SeqAlignPtr   sap;
Uint2 		  The_entityID,entityID,itemID;
Int2		  procval;

	hWinMain=(WindoW)ParentWindow(i);
	
	mWin_d = (DdvMainWinPtr) GetObjectExtra (hWinMain);

	sep=ReadAnyAlignment(is_prot,NULL);

	if (sep!=NULL && mWin_d!=NULL){
		/*try to connect Entrez; if failure, still ok : the user can
		use DDV with a local file. Otherwise he/she will be in trouble ;-)*/
		if (mWin_d->AutonomeViewer && mWin_d->NetStartProc){
			mWin_d->NetStartProc(mWin_d->UseNetwork);
		}
		/*delete old data, if needed*/
		dmp=(DdvMainPtr)GetObjectExtra(mWin_d->hWndDDV);
		DDV_CleanupDDVPdata_g(dmp);
		DDV_CloseData(mWin_d,FALSE);

		vnp_ali=DDV_GetAndCheckSeqAlign(NULL,0,sep,mWin_d->fetchSepProc,&(dmp->dod),
			&The_entityID);
		if (vnp_ali && The_entityID!=0){

			/*get the first SeqAlign in the list and load the viewer panel*/
			dmp->vnp_ali=vnp_ali;

			if(mWin_d->Show_logo){
				mWin_d->Show_logo=FALSE;
			}
			sap=(SeqAlignPtr)vnp_ali->data.ptrvalue;
			entityID=sap->idx.entityID;
			itemID=sap->idx.itemID;
			SetAppProperty("ddvinterndata",(void *)dmp);
			procval=GatherProcLaunch ((dmp->bEditor==TRUE? OMPROC_EDIT : OMPROC_VIEW), FALSE, entityID, itemID, 
				OBJ_SEQALIGN, 0, 0, OBJ_SEQALIGN, 0);
			if ((procval = OM_MSG_RET_ERROR))
				mWin_d->Show_logo=TRUE;
			RemoveAppProperty("ddvinterndata");
		}
	}
	else{
		ErrPostEx (SEV_ERROR, 0, 0, "We do not support this format yet");
	}
}

extern void DDV_ImportNucSeqAlign(IteM i)
{
	DDV_ImportSeqAlign(FALSE,i);
}

extern void DDV_ImportProtSeqAlign(IteM i)
{
	DDV_ImportSeqAlign(TRUE,i);
}

static Int4 DDV_GetLastRow(DDV_HideDialog *hdp)
{
  int i, max=0;

  for (i = 1; i <= hdp->numrows; i++) {
    if (ViewMgr_TRow2VRow(hdp->salp, i) > 0) {
      max = i;
    }
  }
  return(max);
}

static void DDV_SetHideList(DDV_HideDialog *hdp)
{
    int i;
    
    for(i = 1; i <= hdp->numrows; i++) {
        if(ViewMgr_TRow2VRow(hdp->salp, i) > 0) 
            SetItemStatus(hdp->DDV_lsip, i, TRUE);
        else SetItemStatus(hdp->DDV_lsip, i, FALSE);
    }
    SetItemStatus(hdp->DDV_lsip, hdp->amaip->master, TRUE);
}

static void DDV_HideList(Nlm_LisT l)
/*******************************************************************************
*  this function is called each time an item from the list is clicked.
*  it sets the status for each row, making sure not to allow the
*  master to be hidden, nor the number of rows to be less than 2.
*******************************************************************************/
{
    WindoW hHideDlg;
    DDV_HideDialog *hdp;
    Int4 i, LastRow, count = 0;

    hHideDlg = (WindoW)ParentWindow(l);
    if(hHideDlg == NULL) return;	
    hdp = (DDV_HideDialog *) GetObjectExtra(hHideDlg);
    if(hdp == NULL) return;

    /* get row count, and last displayed row */
    for(i = 1; i <= hdp->numrows; i++) {
      if (GetItemStatus(hdp->DDV_lsip, i)) {
        count++;
        LastRow = i;
      }
    }

    /* don't let number of rows fall below 2 */
    if (count < 2) {
      if (hdp->LastValidRow == -1) {
        SetItemStatus(hdp->DDV_lsip, DDV_GetLastRow(hdp), TRUE);
      }
      else {
        SetItemStatus(hdp->DDV_lsip, hdp->LastValidRow, TRUE);
      }
    }
    else {
      /* each time a row is clicked, update the last saved row */
      hdp->LastValidRow = LastRow;
    }

    /* don't let master be hidden */
    if (!GetItemStatus(hdp->DDV_lsip, 1)) {
      SetItemStatus(hdp->DDV_lsip, 1, TRUE);
    }
}


static void DDV_HideAcceptProc(ButtoN b)
{
    WindoW hHideDlg;
    DDV_HideDialog *hdp;
    Int4  i;
    Boolean  status;

    hHideDlg = (WindoW)ParentWindow(b);
    if(hHideDlg == NULL) return;	
    hdp = (DDV_HideDialog *) GetObjectExtra(hHideDlg);
    if(hdp == NULL) return;

    for(i = 1; i <= hdp->numrows; i++) {
      status = GetItemStatus(hdp->DDV_lsip, i);
      ViewMgr_SetHidden(hdp->salp, !status, i);
    }
    ViewMgr_Update(hdp->salp);
    DDV_SetHideList(hdp);

    MemFree(hdp->userdata);
    Remove(hdp->DDV_wHide);
}


/*******************************************************************************

  Function : DDV_HideDlg
  
  Purpose : dialog to import a bioseq into a seqalign.  Note that it has
            several modes listed in ddvopen.h
  
  Return value : none 

*******************************************************************************/

NLM_EXTERN void DDV_HideDlg(DDV_HideDialog *hdp)
{
    GrouP g, hg;
    ButtoN b;
    DDVUpdateMSG  *dump;
    Char szName[181], szName2[161];
    Int4 i;
    SeqId *sip;
    Bioseq *bsp;

    if(hdp == NULL) return;
    hdp->DDV_wHide = NULL;
    
    hdp->target = ViewMgr_GetBeginIndexed(hdp->salp);

    if(hdp->target == NULL) return;
    if( AlnMgrCheckAlignForParent(hdp->target) != AM_PARENT) return;
    hdp->amaip = (AMAlignIndexPtr)(hdp->target->saip);
    if (!hdp->amaip) return;
    
    dump = MemNew(sizeof(DDVUpdateMSG));
    if(dump == NULL) {
        MemFree(hdp);
        return;
    }
    hdp->userdata = dump;
    dump->data = NULL;
    dump->type=UPDATE_TYPE_EDIT_DELBSP;

    hdp->DDV_wHide = MovableModalWindow(-30, -20, -10, -10,
        "Hide/Show Sequences", NULL);
    /* need to do something more than std cleanup.  also for blast dialog */
    Nlm_SetObjectExtra (hdp->DDV_wHide, (void *)hdp, StdCleanupExtraProc);
    
    hg = HiddenGroup(hdp->DDV_wHide, 1, 2, NULL);
    g = NormalGroup(hg, 1, 2, "Choose sequences to show:", systemFont, NULL);
    hdp->DDV_lsip = MultiList(g,40, 6, DDV_HideList);

    hdp->numrows = AlnMgrGetNumRows(hdp->target);

    for(i = 1; i <= hdp->numrows; i++) {
        sip = AlnMgrGetNthSeqIdPtr(hdp->target, i);
        if(sip == NULL) continue;
        bsp=BioseqLockById(sip);
        if(bsp == NULL) continue;
        sip = SeqIdFindBestAccession(bsp->id);
        SeqIdWrite(sip, szName, PRINTID_TEXTID_ACCESSION, 10);
        CreateDefLine(NULL,bsp,szName2,157,0,NULL,NULL);
        BioseqUnlock(bsp);
        StrNCat(szName, " - ", 180);
        StrNCat(szName, szName2, 180);
        ListItem(hdp->DDV_lsip, szName);
    }   
    DDV_SetHideList(hdp);

    g = HiddenGroup(hg, 2, 1, NULL);
    SetGroupSpacing(g, 15, 15);
    b = DefaultButton(g, "OK", (BtnActnProc) DDV_HideAcceptProc);

    Show(hdp->DDV_wHide);

    /* set scroll to upper left */
    SetOffset(hdp->DDV_lsip, 0, 0);

    return;
}

NLM_EXTERN void DDV_HideDlgItem(IteM i)
{
    DDV_HideDialog *hdp;
    DdvMainPtr    dmp;  /*DDV panel data*/
    DdvMainWinPtr mWin_d;
    WindoW		  hWinMain;

    hdp = MemNew(sizeof(DDV_HideDialog));
    if(hdp == NULL) return;

    /* init LastValidRow to something illegal */
    hdp->LastValidRow = -1;

    hWinMain=(WindoW)ParentWindow(i);
    if (hWinMain==NULL) return;
    mWin_d = (DdvMainWinPtr) GetObjectExtra (hWinMain);
    if (mWin_d==NULL) return;

    if (mWin_d->hWndDDV==NULL) return;
    dmp = (DdvMainPtr) GetObjectExtra(mWin_d->hWndDDV);
    if (dmp==NULL) return;

    if(dmp->MSA_d.pgp_l.sap == NULL) return;

    hdp->salp = dmp->MSA_d.pgp_l.sap;

    DDV_HideDlg(hdp);
}


/*******************************************************************************

  Function : DDV_GetCharBufferForEditor()
  
  Purpose : get a char buffer from a row of a SeqAlign, given display coords

  Return value : an allocated char buffer ready for use by the editor. Null if
  failure

*******************************************************************************/
NLM_EXTERN void DDV_GetCharBufferForEditor(ParaGPtr pgp,CharPtr szEditSeq)
{
BioseqPtr  bsp;
Int4       bspLength;
Boolean    IsAA;

	/*get sequence size and type*/
	bsp=BioseqLockById(pgp->sip);
	if (!bsp) return;
	bspLength=BioseqGetLen(bsp);
	IsAA=ISA_aa(bsp->mol);
	BioseqUnlock(bsp);

	/*get the sequence*/
	DDV_GetSequenceFromParaG(pgp,&szEditSeq,bspLength,IsAA,NULL,NULL,NULL);
	
}

/*******************************************************************************

  Function : DDV_GetMtdpListForEditor()
  
  Purpose : get the descriptor for a row of a SeqAlign, given display coords

  Note : in plain english, this function creates the pTxtList ValNode for
     a ParaG designed to be used in the editor.
	 
  Return value : a valnode ready for use by the editor. Null if
  failure. 

*******************************************************************************/
NLM_EXTERN ValNodePtr DDV_GetMtdpListForEditor(ValNodePtr row,Int4 from_disp,
		Int4 to_disp)
{
MsaTxtDispPtr mtdp,new_mtdp,prev_mtdp;
ValNodePtr vnp,vnp2,vnp3,vnpTxt;
ParaGPtr   pgp;
Int4       cumulMtdp,beginCopy,endCopy,mtdp_size,mtdp_from_disp,
           mtdp_to_disp,mtdp_from,mtdp_to;
Boolean    bPop;


	
	prev_mtdp=NULL;
	vnpTxt=NULL;
	for (vnp=row;vnp!=NULL;vnp=vnp->next){
		pgp=(ParaGPtr)vnp->data.ptrvalue;
		/*not yet in the first ParaG ?*/
		if (pgp->StopLetter<from_disp) {
			continue;
		}
		/*get the mtdp nodes*/
		cumulMtdp=0;
		for(vnp2=pgp->ptxtList;vnp2!=NULL;vnp2=vnp2->next){
			mtdp=(MsaTxtDispPtr)vnp2->data.ptrvalue;
			mtdp_size=mtdp->to-mtdp->from+1;
			/*convert mtdp coordinates (can Bioseq coord, gap coord, etc.)
			to display coordinates*/
			mtdp_from_disp=pgp->StartLetter+cumulMtdp;
			mtdp_to_disp=mtdp_from_disp+mtdp_size-1;
			if (mtdp_to_disp<from_disp) {
				cumulMtdp+=mtdp_size;
				continue;
			}
			beginCopy=_max_(from_disp,mtdp_from_disp);
			endCopy=_min_(mtdp_to_disp,to_disp);

			/*convert display coordinates back to mtdp coordinates*/
			mtdp_from=mtdp->from+(beginCopy-mtdp_from_disp);
			mtdp_to=mtdp->from+(endCopy-mtdp_from_disp);

			/*in order to merge nodes of same type, i keep track of
			the previous populated mtdp node. Same style==yes, then
			just extend to the right the previous node*/
			if (prev_mtdp){
				if (prev_mtdp->TextStyle==mtdp->TextStyle){
					prev_mtdp->to=mtdp_to;
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
				new_mtdp=(MsaTxtDispPtr)MemNew(sizeof(MsaTxtDisp));
				if (!new_mtdp) goto erreur;
				MemCopy(new_mtdp,mtdp,sizeof(MsaTxtDisp));
				new_mtdp->from=mtdp_from;
				new_mtdp->to=mtdp_to;
				if (!vnpTxt){
					vnp3=ValNodeAddPointer(&vnpTxt,0,(Pointer)new_mtdp);
				}
				else{
					vnp3=ValNodeAddPointer(&vnp3,0,(Pointer)new_mtdp);
				}
				prev_mtdp=new_mtdp;
			}
			if (mtdp_to_disp>=to_disp) break;
			cumulMtdp+=mtdp_size;
		}
		/*done ?*/
		if (pgp->StopLetter>=to_disp) break;
	}
	return(vnpTxt);

erreur:
	if (vnpTxt){
		DDV_DeleteTxtList(&vnpTxt);
	}
	return(NULL);
}

/*******************************************************************************

  Function : DDV_CreateDataForEditor()
  
  Purpose : create a MsaParaGPopListPtr intended to be used by the editor

  Return value : an allocated data block ready for use by the editor. Null if
  failure

*******************************************************************************/
NLM_EXTERN MsaParaGPopListPtr DDV_CreateDataForEditor(MsaParaGPopListPtr mpplp, 
	Int4 from_disp,Int4 to_disp)
{
MsaParaGPopListPtr new_mpplp;

ParaGPtr           pgp,old_pgp;
Int4               i,iNewAliSize,diff;

	if (from_disp<0 || from_disp>=mpplp->LengthAli || from_disp>to_disp ||
		to_disp<0 || to_disp>=mpplp->LengthAli){
		return(NULL);
	}
	
	diff=to_disp-from_disp;
	iNewAliSize = diff+50;

	if (iNewAliSize<1 || iNewAliSize>LIMIT_EDITOR_SIZE)
		return(NULL);

	new_mpplp=(MsaParaGPopListPtr)MemNew(sizeof(MsaParaGPopList));
	if (!new_mpplp) return(NULL);
	
	/*table of ParaG list (each cell of that table corresponds to one row
	of the seqalign to display*/
	new_mpplp->TableHead=(ValNodePtr *)MemNew(mpplp->nBsp*sizeof(ValNodePtr));
	if (!new_mpplp->TableHead) {
		MemFree(new_mpplp);
		return(NULL);
	}
	/*allocate the new ParaG list; for the editor there is a single ParaG for
	each row (for the viewer there are usually n ParaGs for each row. For the
	editor, the ParaG size = to_disp-from_disp+50 (whereas in the viewer a
	ParaG size = ParaG_Size (usually 70; see pgppop.h)*/
	for (i=0;i<mpplp->nBsp;i++){
		/*allocations*/
		pgp=(ParaGPtr)MemNew(sizeof(ParaG));
		if (!pgp) goto erreur;
		pgp->szEditSeq=(CharPtr)MemNew(iNewAliSize*sizeof(Char));
		if (pgp->szEditSeq==NULL) goto erreur;
		/*get the ParaG line descriptor */
		pgp->ptxtList=DDV_GetMtdpListForEditor(mpplp->TableHead[i],from_disp,
				to_disp);
		if (!pgp->ptxtList) goto erreur;
		/*get the char buffer*/
                old_pgp=(ParaGPtr)mpplp->TableHead[i]->data.ptrvalue;
                pgp->sip = old_pgp->sip;
		DDV_GetCharBufferForEditor(pgp,pgp->szEditSeq);
		if (*(pgp->szEditSeq)=='\0') goto erreur;
		/*copy/init values*/
		pgp->StartLetter = 0;
		pgp->StopLetter = diff;
		pgp->NumOrder = i;
		pgp->StartLine = i;
		pgp->nLines = 1;
		pgp->ScaleStyle=SCALE_POS_NONE;
		/*store pgp*/
                new_mpplp->TableHead[i]=(ValNodePtr)MemNew(sizeof(ValNode));
                if (!new_mpplp->TableHead[i]) goto erreur;
		new_mpplp->TableHead[i]->data.ptrvalue=(Pointer)pgp;
	}

	/*seqali size and type*/
	new_mpplp->LengthAli=diff+1;
	new_mpplp->nBsp=mpplp->nBsp;
	new_mpplp->sap=mpplp->sap;
	new_mpplp->DisplayType=mpplp->DisplayType;
	
	/*entities table*/
	new_mpplp->entitiesTbl=(Uint4Ptr)MemNew(new_mpplp->nBsp*sizeof(Uint4));
	if (new_mpplp->entitiesTbl==NULL) goto erreur;
	MemCopy(new_mpplp->entitiesTbl,mpplp->entitiesTbl,
		new_mpplp->nBsp*sizeof(Uint4));
	/*ruler descriptor*/
	new_mpplp->RulerDescr=DDV_GetRulerForEditor(mpplp->RulerDescr,from_disp,
			to_disp);
	if (new_mpplp->RulerDescr==NULL) goto erreur;
	return(new_mpplp);	
	
erreur : /*serious problem*/	
	if (new_mpplp->entitiesTbl) 
		MemFree(new_mpplp->entitiesTbl);
	if (new_mpplp->RulerDescr)
		ValNodeFreeData(new_mpplp->RulerDescr);
	DDV_DeleteDisplayList(new_mpplp);
	MemFree(new_mpplp);
	return(NULL);
}


