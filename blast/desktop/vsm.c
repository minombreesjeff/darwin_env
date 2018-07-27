/*   vsm.c
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
* File Name:  vsm.c
*
* Author:  Jim Ostell
*
* Version Creation Date:   11-29-94
*
* $Revision: 6.13 $
*
* File Description: 
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* ==========================================================================
*/

#include <vsmpriv.h>
#include <objsub.h>
#include <objfdef.h>
#include <dlogutil.h>
#include <bspview.h>

static VSMWinPtr NEAR VSMWinNew PROTO((VSeqMgrPtr vsmp));

static void NEAR VSMWinShow PROTO((VSMWinPtr vsmwp));

static void NEAR VSMWinDelete PROTO((VSMWinPtr vsmwp, Uint2 entityID, Uint2 itemID,
                                          Uint2 itemtype));

static void NEAR VSMWinRefreshData PROTO((VSMWinPtr vsmwp, Uint2 entityID,
                                     Uint2 itemID, Uint2 itemtype));

static void NEAR VSMPictSelect PROTO((VSMPictPtr vsmpp,
               Uint2 entityID, Uint2 itemID, Uint2 itemtype, Boolean select));

static VSMWinPtr NEAR VSeqMgrGetWinForW PROTO((WindoW w));

static Boolean NEAR VSeqMgrCreateMainWindow PROTO((VSeqMgrPtr vsmp, Boolean program_window));

static void NEAR VSeqMgrPopulateMain PROTO((VSMWinPtr vsmwp));

#define VSM_PICT_UP_BUTTON  65001
#define VSM_PICT_DOWN_BUTTON 65002
SegmenT VSMEntityDraw PROTO((ObjMgrDataPtr omdp, VSMPictPtr vsmpp, VSeqMgrPtr vsmp));

static OMUserDataPtr NEAR VSMAddPictureToEntity PROTO((VSMWinPtr vsmwp, Uint2 entityID, Int2 expand));

/*****************************************************************************
*
*   VSeqMgrGenFunc()
*   	General function to call from ObjMgr
*
*****************************************************************************/
Int2 LIBCALLBACK VSeqMgrGenFunc PROTO((Pointer ompcp));

/*****************************************************************************
*
*   VSeqMgrMsgFunc()
*   	function to call on each DeskTop window by the ObjMgr
*
*****************************************************************************/
Int2 LIBCALLBACK VSeqMgrMsgFunc PROTO((OMMsgStructPtr ommsp));


Int2 LIBCALLBACK VSMPictMsgFunc PROTO((OMMsgStructPtr ommsp));

/*****************************************************************************
*
*   Public VSeqMgr Functions
*
*****************************************************************************/

/*****************************************************************************
*
*   VSeqMgrInit(show)
*   	if (show) displays the window.
*   	OtherWise, just initializes.
*
*****************************************************************************/
Boolean LIBCALL VSeqMgrInit (Boolean show)
{
	VSeqMgrPtr vsmp;

	if (show)
	{
		return VSeqMgrShow();
	}

	vsmp = VSeqMgrGet();
	if (vsmp == NULL) return FALSE;

	return TRUE;
}

/*****************************************************************************
*
*   VSeqMgrShow()
*   	display the VSeqMgr
*   	initializes if not already done
*
*****************************************************************************/
Boolean LIBCALL VSeqMgrShow(void)
{
	VSeqMgrPtr vsmp;
	VSMWinPtr tmp, next=NULL;

        /***
	Message(MSG_OK, "In VSeqMgrShow");
	***/

	vsmp = VSeqMgrGet();
	for (tmp = vsmp->wins; tmp != NULL; tmp = tmp->next)
	{
		tmp->in_process = TRUE;
		VSMWinRefreshData(tmp, 0,0,0);
		VSMWinShow(tmp);
	}

	return TRUE;
}

/*****************************************************************************
*
*   VSeqMgrDelete()
*      removes the VSeqMgr from the screen
*      keeps settings for the main window
*
*****************************************************************************/
Boolean LIBCALL VSeqMgrDelete(void)
{
	VSeqMgrPtr vsmp;
	VSMWinPtr tmp, next=NULL;
	
	vsmp = VSeqMgrGet();
	for (tmp = vsmp->wins; tmp != NULL; tmp = next)
	{
		next = tmp->next;
		VSMWinDelete(tmp, 0,0,0);
	}
	return TRUE;
}

static char * VSeqMgrStr = "NCBIDT";

/*****************************************************************************
*
*   Return the current VSeqMgr
*   	Initialize if not done already
*
*****************************************************************************/
VSeqMgrPtr LIBCALL VSeqMgrGet (void)
{
	VSeqMgrPtr vsmp=NULL;

	vsmp = (VSeqMgrPtr) GetAppProperty(VSeqMgrStr);
	if (vsmp == NULL)
	{	                             /*** have to initialize it **/
		vsmp = (VSeqMgrPtr) MemNew (sizeof(VSeqMgr));
	                             /*** initialize the functions **/
		vsmp->omp = ObjMgrGet();
		vsmp->appname = StringSave(VSeqMgrStr);   /* used for the app file as well */
		SetAppProperty (VSeqMgrStr, (void *) vsmp);
		
		VSeqMgrCreateMainWindow(vsmp, FALSE);
	}
	
	return vsmp;
}

/*****************************************************************************
*
*   Run VSeqMgr as the main program
*   	Initialize if not done already
*
*****************************************************************************/
void LIBCALL VSeqMgrRun (CharPtr title, CharPtr aboutinfo)
{
	VSeqMgrPtr vsmp=NULL;

	vsmp = (VSeqMgrPtr) GetAppProperty(VSeqMgrStr);
	if (vsmp == NULL)
	{	                             /*** have to initialize it **/
		vsmp = (VSeqMgrPtr) MemNew (sizeof(VSeqMgr));
		vsmp->aboutinfo = StringSave(aboutinfo);
		vsmp->title = StringSave(title);
	                             /*** initialize the functions **/
		vsmp->omp = ObjMgrGet();
		vsmp->appname = StringSave(VSeqMgrStr);   /* used for the app file as well */
		SetAppProperty (VSeqMgrStr, (void *) vsmp);
		
		if (! VSeqMgrCreateMainWindow(vsmp, TRUE))
			return;
	}

	VSeqMgrShow();

	ProcessEvents();

	return;
}

static void QuitVSeqMgr (WindoW w)
{
	VSMWinPtr vsmwp;

 	vsmwp = VSeqMgrGetWinForW(w);
	if (vsmwp->is_main_VSM)
		VSeqMgrDelete();
	else
		VSMWinDelete(vsmwp,0,0,0);
	return;
}

static void ResizeVSeqMgr (WindoW w)
{
	VSMWinPtr vsmwp;

	vsmwp = VSeqMgrGetWinForW(w);

	/***
	Message(MSG_OK, "In Resize");
	***/

	if (vsmwp != NULL)
	{
		if (vsmwp->shown && (! vsmwp->in_process))
		{
			vsmwp->in_process = TRUE;
			VSMWinRefreshData(vsmwp, 0,0,0);
			vsmwp->in_process = FALSE;
			/**
			VSMWinShow(vsmwp);
			**/
		}
	}
	return;
}

static void NEAR VSMWinShow (VSMWinPtr vsmwp)
{
	if (vsmwp == NULL) return;

	/***
	Message(MSG_OK, "In VSMWinShow");
	***/
	
	if (! vsmwp->populated)
	{
		vsmwp->in_process = TRUE;
		VSMWinRefreshData(vsmwp, 0,0,0);
	}
    
    vsmwp->in_process = FALSE;
	if (! vsmwp->shown)
	{
		Show(vsmwp->w);
		vsmwp->shown = TRUE;
		Select(vsmwp->w);
	}
	return;
}

static void NEAR VSMWinRefreshData (VSMWinPtr vsmwp, Uint2 entityID,
                                     Uint2 itemID, Uint2 itemtype)
{
	if (vsmwp == NULL) return;
	
	/**
	Message(MSG_OK, "In refresh data");
	**/
	                   /* not on screen and not being prepared for screen */
	if (! (vsmwp->shown || vsmwp->in_process))
		return;

	if (vsmwp->entityID == 0)
		VSeqMgrPopulateMain(vsmwp);
	else
		Message(MSG_ERROR, "Got a non-main window in refresh");
	return;
}

static void VSeqMgrOpenProc (ChoicE c)
{
	Int2 value;
	Uint2 procid;
	VSeqMgrPtr vsmp;
	Int2 retval;
	ObjMgrProcPtr ompp;
	OMProcControl ompc;

	MemSet(&ompc, 0, sizeof(OMProcControl));
	vsmp = VSeqMgrGet();	
	value = GetValue(c);
	SetValue(c, 0);
	procid = vsmp->procmenu[OMPROC_OPEN][value];
	
	if (procid)
	{
		ompp = ObjMgrProcFind(vsmp->omp, procid, NULL, 0);
		if (ompp != NULL)
		{
			ompc.proc = ompp;
			retval = (*(ompp->func)) (&ompc);
		
			if (retval == OM_MSG_RET_ERROR)
				ErrShow();
		}
	}
	return;
}

static void VSeqMgrSaveProc (ChoicE c)
{
	Int2 value;
	Uint2 procid;
	VSeqMgrPtr vsmp;
	Int2 retval;
	ObjMgrProcPtr ompp;
	OMProcControl ompc;
	
	vsmp = VSeqMgrGet();	
	value = GetValue(c);
	SetValue(c, 0);
	
	MemSet(&ompc, 0, sizeof(OMProcControl));
	ompc.do_not_reload_from_cache = TRUE;
	if (! GatherDataForProc(&ompc, TRUE))  /* anything selected? */
	{
		ErrShow();
		return;
	}

	procid = vsmp->procmenu[OMPROC_SAVE][value];
	if (procid)
	{
		ompp = ObjMgrProcFind(vsmp->omp, procid, NULL, 0);
		if (ompp != NULL)
		{
			ompc.proc = ompp;
			retval = (*(ompp->func)) (&ompc);
		
			if (retval == OM_MSG_RET_ERROR)
				ErrShow();
		}
	}
	return;
}

/*
static Boolean ObjMgrProcMatch (ObjMgrProcPtr ompp, OMProcControlPtr ompcp, Boolean showerror)
{
	ObjMgrTypePtr omtp1, omtp2;
	ObjMgrPtr omp;
	Boolean ok = FALSE;
	Uint2 subtype=0;
	Pointer ptr=NULL;

	if ((ompp == NULL) || (ompcp == NULL))
		return FALSE;

	if (ompp->inputtype == 0)
		return TRUE;

	if (ompp->inputtype == ompcp->input_itemtype)
	{
		ok = TRUE;
		ptr = ompcp->input_data;
	}
	else if (ompp->inputtype == ompcp->input_choicetype)
	{
		ok = TRUE;
		ptr = ompcp->input_choice;
	}

	if (ok)
	{
		if (ompp->subinputtype != 0)
		{
			omp = ObjMgrGet();
			omtp1 = ObjMgrTypeFind(omp, ompp->inputtype, NULL, NULL);
			if (omtp1 != NULL)
				subtype = (*(omtp1->subtypefunc)) (ptr);
			if (subtype != ompp->subinputtype)
			{
				if (showerror)
					ErrPostEx(SEV_ERROR,0,0,"Trying to put subtype %d into function taking subtype %d",
						(int)subtype, (int)(ompp->subinputtype));
				return FALSE;
			}
		}
	}

	if (! ok)
	{
		if (showerror)
		{
			omp = ObjMgrGet();
			omtp1 = ObjMgrTypeFind(omp, ompp->inputtype, NULL, NULL);
			if (omtp1 == NULL)
			{
				ErrPostEx(SEV_ERROR,0,0,"Input type for function not registered");
				return FALSE;
			}
			omtp2 = ObjMgrTypeFind(omp, ompcp->input_itemtype, NULL, NULL);
			if (omtp2 == NULL)
			{
				ErrPostEx(SEV_ERROR,0,0,"Selected data type not registered");
				return FALSE;
			}
			ErrPostEx(SEV_ERROR,0,0,"Trying to put a %s into function taking a %s",
				omtp2->label, omtp1->label);
		}
		return FALSE;
	}
	return TRUE;
}
*/

static void ShowVSMProc (IteM i)
{
	VSeqMgrShow();
	return;
}

/*****************************************************************************
*
*   VSMAddToMenu(MenU m, Int2 menutype)
*      Adds a VSM style submenu to Menu
*      All included procs must have already been registered with the
*        objmgr.
*
*****************************************************************************/
Boolean LIBCALL VSMAddToMenu (MenU m, Int2 menutype)
{
	MenU s;
	ChoicE c;
	ObjMgrProcPtr ompp;
	ObjMgrPtr omp;
	VSeqMgrPtr vsmp;
	Int2 ctr, proctype;
	static Int2 proctypes [3] = {
		0,
		OMPROC_OPEN,
		OMPROC_SAVE};
	static CharPtr menuname [3] = {
		"NCBI DeskTop",
		"Open",
		"Save As"};
	ChsActnProc ap;

	if (m == NULL) return FALSE;
	switch (menutype)
	{
		case VSM_DESKTOP:
			CommandItem(m, menuname[menutype], ShowVSMProc);
			return TRUE;
		case VSM_OPEN_MENU:
			ap = VSeqMgrOpenProc;
			break;
		case VSM_SAVE_MENU:
			ap = VSeqMgrSaveProc;
			break;
		default:
			return FALSE;
	}

	proctype = proctypes[menutype];
	vsmp = VSeqMgrGet();
	omp = vsmp->omp;

	s = SubMenu (m, menuname[menutype]);
	c = ChoiceGroup(s, ap);
	ompp = NULL;
	ctr = 0;
	while ((ompp = ObjMgrProcFindNext(omp, proctype, 0,0, ompp)) != NULL)
	{
		ChoiceItem(c, ompp->proclabel);
		ctr++;
		vsmp->procmenu[proctype][ctr] = ompp->procid;
	}
	return TRUE;
}

static void VSeqMgrStdMenuProc (IteM i)
{
	Int2 retval;
	ObjMgrProcPtr ompp;
	OMProcControl ompc;

	ompp = (ObjMgrProcPtr) GetObjectExtra(i);
	if (ompp == NULL || ompp->func == NULL)
		return;
	
	MemSet(&ompc, 0, sizeof(OMProcControl));
	ompc.do_not_reload_from_cache = TRUE;
	GatherDataForProc(&ompc, TRUE);
	ompc.proc = ompp;
	retval = (*(ompp->func)) (&ompc);
	if (retval == OM_MSG_RET_ERROR)
		ErrShow();
	/*
	if (! GatherDataForProc(&ompc, TRUE))
	{
		ErrShow();
		return;
	}

	if (ObjMgrProcMatch(ompp, &ompc, TRUE))
	{
		ompc.proc = ompp;
		retval = (*(ompp->func)) (&ompc);
	
		if (retval == OM_MSG_RET_ERROR)
			ErrShow();
	}
	else
		ErrShow();
	*/
	return;
}

/*****************************************************************************
*
*   VSMAddMenu(WindoW w, Int2 menutype)
*      Adds a VSM style menu to Window
*      All included procs must have already been registered with the
*        objmgr.
*      If no procs of this type are registered, does not add to window
*
*****************************************************************************/
typedef struct sbstruc {
  CharPtr    name;
  MenU       menu;
} Sbstruc, PNTR SbstrucPtr;

Boolean LIBCALL VSMAddMenu (WindoW w, Int2 menutype)
{
	MenU m, sub, sub2, x;
	ObjMgrProcPtr ompp;
	ObjMgrPtr omp;
	ObjMgrTypePtr omtp;
	VSeqMgrPtr vsmp;
	Int2 ctr, proctype;
	FeatDispGroupPtr fdgp;
	FeatDefPtr fdp;
	Uint1 key;
	CharPtr label;
	static Int2 proctypes [5] = {
		0,
		OMPROC_FILTER ,
		OMPROC_ANALYZE ,
		OMPROC_VIEW ,
		OMPROC_EDIT } ;
	static CharPtr menuname [5] = {
		NULL,
		"Filter",
		"Analysis",
		"View",
		"Editor" };
	IteM i;
	ValNodePtr submenulist = NULL, vnp;
	SbstrucPtr sbp;
	Uint2 subtype;

#ifndef WIN_MAC
	if (w == NULL) return FALSE;
#endif
	switch (menutype)
	{
		case VSM_FILTER_MENU:
		case VSM_ANALYZE_MENU:
		case VSM_VIEW_MENU:
			proctype = proctypes[menutype];
			vsmp = VSeqMgrGet();
			omp = vsmp->omp;

			ompp = ObjMgrProcFindNext(omp, proctype, 0,0, NULL);
			if (ompp == NULL)   /* none registered */
				return FALSE;

			m = PulldownMenu(w, menuname[menutype]);
			ctr = 0;
			do
			{
				x = NULL;
				if (! StringHasNoText (ompp->submenu)) {
					vnp = submenulist;
					while (vnp != NULL && x == NULL) {
						sbp = (SbstrucPtr) vnp->data.ptrvalue;
						if (sbp != NULL) {
							if (StringICmp (ompp->submenu, sbp->name) == 0) {
								x = sbp->menu;
							}
						}
						vnp = vnp->next;
					}
					if (x == NULL) {
						sbp = (SbstrucPtr) MemNew (sizeof (Sbstruc));
						if (sbp != NULL) {
							sbp->name = ompp->submenu;
							sbp->menu = SubMenu (m, sbp->name);
							x = sbp->menu;
							ValNodeAddPointer (&submenulist, 0, (VoidPtr) sbp);
						}
					}
				}
				if (x == NULL) {
					x = m;
				}
				i = CommandItem(x, ompp->proclabel, VSeqMgrStdMenuProc);
				SetObjectExtra(i, (VoidPtr)ompp, NULL);
				ctr++;
			}
			while ((ompp = ObjMgrProcFindNext(omp, proctype, 0,0, ompp)) != NULL);
			ValNodeFree (submenulist);
			return TRUE;
		case VSM_EDIT_MENU:
			proctype = proctypes[menutype];
			vsmp = VSeqMgrGet();
			omp = vsmp->omp;

			ompp = ObjMgrProcFindNext(omp, proctype, 0,0, NULL);
			if (ompp == NULL)   /* none registered */
				return FALSE;

			m = PulldownMenu(w, menuname[menutype]);
			/* code is below this switch statement */
			break;
		default:
			return FALSE;
	}
	omtp = NULL;
	ctr = 0;
	while ((omtp = ObjMgrTypeFindNext(omp, omtp)) != NULL) {
		ompp = ObjMgrProcFindNext(omp, proctype, omtp->datatype, 0, NULL);
		if (ompp != NULL) {
			if (omtp->datatype == OBJ_SEQFEAT) {
				sub = SubMenu (m, omtp->label);
				fdgp = NULL;
				while ((fdgp = DispGroupFindNext (fdgp, &key, &label)) != NULL) {
					if (fdgp->groupkey != 0) {
						sub2 = SubMenu (sub, fdgp->groupname);
						fdp = NULL;
						label = NULL;
						while ((fdp = FeatDefFindNext (fdp, &key, &label, fdgp->groupkey, FALSE)) != NULL) {
							if (key != FEATDEF_BAD) {
								ompp = NULL;
								while ((ompp = ObjMgrProcFindNext(omp, proctype,
								   	    omtp->datatype, 0, ompp)) != NULL) {
									if (ompp->subinputtype == fdp->featdef_key) {
										i = CommandItem(sub2, ompp->proclabel, VSeqMgrStdMenuProc);
										SetObjectExtra(i, (VoidPtr)ompp, NULL);
										ctr++;
									}
								}
							}
						}
					}
				}
                sub2 = SubMenu (sub, "Remaining Features");
                fdp = NULL;
                while ((fdp = FeatDefFindNext (fdp, &key, &label, 0, FALSE)) != NULL) {
                  if (key != FEATDEF_BAD) {
                    ompp = NULL;
                    while ((ompp = ObjMgrProcFindNext (omp, proctype,
				            omtp->datatype, 0, ompp)) != NULL) {
				      subtype = ompp->subinputtype;
                      if (subtype == fdp->featdef_key &&
                          subtype != FEATDEF_PUB &&
                          subtype != FEATDEF_Imp_CDS &&
                          subtype != FEATDEF_misc_RNA &&
                          subtype != FEATDEF_precursor_RNA &&
                          subtype != FEATDEF_mat_peptide &&
                          subtype != FEATDEF_sig_peptide &&
                          subtype != FEATDEF_transit_peptide &&
                          subtype != FEATDEF_source &&
                          subtype != FEATDEF_virion &&
                          subtype != FEATDEF_mutation &&
                          subtype != FEATDEF_allele &&
                          subtype != FEATDEF_site_ref) {
                        i = CommandItem (sub2, ompp->proclabel, VSeqMgrStdMenuProc);
                        SetObjectExtra(i, (VoidPtr)ompp, NULL);
                        ctr++;
                      }
                    }
                  }
                }
			} else {
				sub = SubMenu (m, omtp->label);
				do
				{
					i = CommandItem(sub, ompp->proclabel, VSeqMgrStdMenuProc);
					SetObjectExtra(i, (VoidPtr)ompp, NULL);
					ctr++;
				}
				while ((ompp = ObjMgrProcFindNext(omp, proctype,
				       omtp->datatype, 0, ompp)) != NULL);
			}
		}
	}
	return TRUE;
}

static void VSeqMgrQuitProc (IteM i)
{
	VSeqMgrDelete();
	return;
}

static void VSeqMgrEditProc (IteM i)
{
	Message(MSG_OK, "Edit");
	return;
}

static void VSeqMgrCutProc (IteM i)
{
	OMProcControl ompc;
	
	MemSet(&ompc, 0, sizeof(OMProcControl));
	ompc.do_not_reload_from_cache = TRUE;
	if (! DetachDataForProc(&ompc, TRUE))  /* anything selected? */
	{
		ErrShow();
		return;
	}

	if (! ompc.whole_entity)   /* just a part gone, so need an update */
		ObjMgrSendMsg(OM_MSG_DEL, ompc.input_entityID, ompc.input_itemID, ompc.input_itemtype);

	if (! ObjMgrAddToClipBoard (0, ompc.input_data))
		ErrShow();

	return;
}

static void VSeqMgrCopyProc (IteM i)
{
	OMProcControl ompc;
	
	MemSet(&ompc, 0, sizeof(OMProcControl));
	ompc.do_not_reload_from_cache = TRUE;
	if (! CopyDataForProc(&ompc, TRUE))  /* anything selected? */
	{
		ErrShow();
		return;
	}

	if (ompc.output_data == NULL)
	{
		ErrShow();
		return;
	}

	if (! ObjMgrAddToClipBoard (0, ompc.output_data))
		ErrShow();

	return;
}

static void VSeqMgrPasteProc (IteM i)
{
	OMProcControl ompc;
	ObjMgrDataPtr omdp;
	ObjMgrTypePtr omtp;
	SelStructPtr ssp;
	ObjMgrPtr omp;
	Uint2 type;
	Pointer ptr;

	omdp = ObjMgrGetClipBoard();
	if (omdp == NULL)
	{
		Message(MSG_ERROR, "No data in clipboard");
		return;
	}

	MemSet(&ompc, 0, sizeof(OMProcControl));
	ompc.input_data = omdp->dataptr;
	ompc.input_entityID = omdp->EntityID;
	ompc.input_choice = omdp->choice;
	ompc.input_itemtype = omdp->datatype;
	ompc.input_itemID = 1;
	ompc.input_choicetype = omdp->choicetype;
	ompc.whole_entity = TRUE;
	ompc.do_not_reload_from_cache = TRUE;

	if (! CopyDataForProc(&ompc, FALSE))  /* just do the copy */
	{
		ErrShow();
		return;
	}

	if (ompc.output_data == NULL)
	{
		ErrShow();
		return;
	}

	ompc.input_data = NULL;
	ompc.input_entityID = 0;
	ompc.input_choice = NULL;
	ompc.input_itemtype = 0;
	ompc.input_choicetype = 0;
	ompc.whole_entity = FALSE;
	ompc.do_not_reload_from_cache = TRUE;

	ssp = ObjMgrGetSelected();
	if (ssp == NULL)    /* nothing selected, paste into main window */
	{
		ObjMgrRegister(ompc.output_itemtype, ompc.output_data);
		return;
	}

	if (! AttachDataForProc(&ompc, TRUE))
	{
		ErrShow();
		if (ompc.output_choice != NULL)
		{
			ptr = ompc.output_choice;
			type = ompc.output_choicetype;
		}
		else
		{
			ptr = ompc.output_data;
			type = ompc.output_itemtype;
		}
		omp = ObjMgrGet();
		omtp = ObjMgrTypeFind(omp, type,NULL,NULL);
		if (omtp != NULL)
			(*(omtp->freefunc))(ptr);
	}
	else
		ObjMgrSendMsg(OM_MSG_UPDATE, ompc.input_entityID, ompc.input_itemID, ompc.input_itemtype);



	return;
}

static void VSeqMgrMoveProc (IteM i)
{
	OMProcControl ompc;
	ObjMgrDataPtr omdp;
	ObjMgrTypePtr omtp;
	SelStructPtr ssp;
	ObjMgrPtr omp;
	Uint2 type;
	Pointer ptr;

	omdp = ObjMgrGetClipBoard();
	if (omdp == NULL)
	{
		Message(MSG_ERROR, "No data in clipboard");
		return;
	}

	MemSet(&ompc, 0, sizeof(OMProcControl));
	ompc.output_data = omdp->dataptr;
	ompc.output_entityID = omdp->EntityID;
	ompc.output_choice = omdp->choice;
	ompc.output_itemtype = omdp->datatype;
	ompc.output_choicetype = omdp->choicetype;
	ompc.whole_entity = TRUE;
	ompc.do_not_reload_from_cache = TRUE;

	ssp = ObjMgrGetSelected();
	if (ssp == NULL)    /* nothing selected, paste into main window */
	{
		ObjMgrRegister(ompc.output_itemtype, ompc.output_data);
		return;
	}

	if (! AttachDataForProc(&ompc, TRUE))
	{
		ErrShow();
		if (ompc.output_choice != NULL)
		{
			ptr = ompc.output_choice;
			type = ompc.output_choicetype;
		}
		else
		{
			ptr = ompc.output_data;
			type = ompc.output_itemtype;
		}
		omp = ObjMgrGet();
		omtp = ObjMgrTypeFind(omp, type,NULL,NULL);
		if (omtp != NULL)
			(*(omtp->freefunc))(ptr);
	}
	else
		ObjMgrSendMsg(OM_MSG_UPDATE, ompc.input_entityID, ompc.input_itemID, ompc.input_itemtype);


	return;
}

static void VSeqMgrDeleteProc (IteM i)
{
	OMProcControl ompc;
	ObjMgrTypePtr omtp;
	ObjMgrPtr omp;
	Uint2 type;
	Pointer ptr;
	
	MemSet(&ompc, 0, sizeof(OMProcControl));
	ompc.do_not_reload_from_cache = TRUE;
	if (! DetachDataForProc(&ompc, TRUE))  /* anything selected? */
	{
		ErrShow();
		return;
	}

	if (ompc.input_choicetype)
	{
		type = ompc.input_choicetype;
		ptr = ompc.input_choice;
	}
	else
	{
		type = ompc.input_itemtype;
		ptr = ompc.input_data;
	}

	omp = ObjMgrGet();
	omtp = ObjMgrTypeFind(omp, type, NULL, NULL);
	if (omtp == NULL)
	{
		Message(MSG_ERROR, "Delete: type [%d] not registered", (int)type);
		return;
	}

	(*(omtp->freefunc))(ptr);

	if (! ompc.whole_entity)   /* just a part gone, so need an update */
		ObjMgrSendMsg(OM_MSG_DEL, ompc.input_entityID, ompc.input_itemID, ompc.input_itemtype);

	return;
}

static void VSMDragAndDrop(VSMWinPtr vsmwp, Uint2 entityID, Uint2 itemID, Uint2 itemtype)
{
	OMProcControl ompc;
	
	MemSet(&ompc, 0, sizeof(OMProcControl));
	ompc.input_entityID = vsmwp->entityID1;
	ompc.input_itemID = vsmwp->itemID1;
	ompc.input_itemtype = vsmwp->itemtype1;
 	ompc.do_not_reload_from_cache = TRUE;

	if (! DetachDataForProc(&ompc, FALSE))
	{
		ErrShow();
		return;
	}

	if (! ompc.whole_entity)   /* just a part gone, so need an update */
		ObjMgrSendMsg(OM_MSG_DEL, ompc.input_entityID, ompc.input_itemID, ompc.input_itemtype);

	if (entityID == 0)   /* going into desktop */
	{
		ObjMgrSendMsg(OM_MSG_UPDATE, ompc.input_entityID, ompc.input_itemID, ompc.input_itemtype);
		ObjMgrRegister(ompc.input_itemtype, ompc.input_data);
		return;
	}
		
	ompc.output_entityID = ompc.input_entityID;
	ompc.output_itemID = ompc.input_itemID;
	ompc.output_itemtype = ompc.input_itemtype;
	ompc.output_choice = ompc.input_choice;
	ompc.output_data = ompc.input_data;
	ompc.output_choicetype = ompc.input_choicetype;
	ompc.whole_entity = FALSE;
	ompc.input_entityID = entityID;
	ompc.input_itemID = itemID;
	ompc.input_itemtype = itemtype;
	ompc.input_choicetype = 0;
	ompc.input_choice = NULL;
	ompc.input_data = NULL;
 	ompc.do_not_reload_from_cache = TRUE;

	if (! AttachDataForProc(&ompc, FALSE))
	{
		ErrShow();      /* put in desktop */
		ObjMgrRegister(ompc.output_itemtype, ompc.output_data);
	}
	else
		ObjMgrSendMsg(OM_MSG_UPDATE, ompc.input_entityID, ompc.input_itemID, ompc.input_itemtype);

	return;

}

static void VSeqMgrFontProc (IteM i)
{
	VSeqMgrPtr vsmp;
	Nlm_FontSpec font;
	FonT f;
	Char fontbuf[80];

	vsmp = VSeqMgrGet();
	GetFontSpec(vsmp->font, &font);
	if (ChooseFont(&font, CFF_READ_FSP, NULL))
	{
		/***
		f = GetPermanentFont(&font);
                ***/
                f = CreateFont(&font);
		vsmp->font = f;
		SelectFont(f);
		vsmp->lineheight = LineHeight();
		vsmp->leading = Leading();
		vsmp->charw = MaxCharWidth();
		vsmp->update_all = TRUE;
		VSeqMgrShow();
		vsmp->update_all = FALSE;
		FontSpecToStr(&font, fontbuf, 80);
		SetAppParam(vsmp->appname, "DeskTop", "FONT", fontbuf);
	}
	return;
}

static void VSeqMgrShowCacheProc (IteM i)
{
	VSeqMgrPtr vsmp;

	vsmp = VSeqMgrGet();
	if (vsmp->show_cached)
		vsmp->show_cached = FALSE;
	else
		vsmp->show_cached = TRUE;
	SetStatus(i, vsmp->show_cached);
	vsmp->update_all = TRUE;
	VSeqMgrShow();
	vsmp->update_all = FALSE;
	return;
}

static void VSeqMgrShowClipboardProc (IteM i)
{
	VSeqMgrPtr vsmp;

	vsmp = VSeqMgrGet();
	if (vsmp->show_clipboard)
		vsmp->show_clipboard = FALSE;
	else
		vsmp->show_clipboard = TRUE;
	SetStatus(i, vsmp->show_clipboard);
	vsmp->update_all = TRUE;
	VSeqMgrShow();
	vsmp->update_all = FALSE;
	return;
}

static void VSMMarquee( VSMWinPtr vsmwp )
{
	RecT dr;
	
	WidePen(4);
	InvertMode();
	Dotted();
	SectRect(&(vsmwp->r), &(vsmwp->rv), &dr);
	FrameRect(&dr);
	return;
}

static void VSeqMgrClickProc (VieweR v, SegmenT s, PoinT p)
{
	VSMWinPtr vsmwp;
	SegmenT sp, top, prev, next;
	Uint2 segID, primID, primCt;
	BoxInfo box;
	PntInfo pi;
	PoinT tl, br;

	vsmwp = (VSMWinPtr)GetViewerData(v);
	if (vsmwp == NULL)
	{
		Message(MSG_ERROR, "vsmwp was NULL in ClickProc");
		return;
	}
	
	vsmwp->dblclick = dblClick;
	vsmwp->shftkey = shftKey;
	vsmwp->dragged = FALSE;
	if (dblClick)
	{
		if (vsmwp->marquee)
		{
			VSMMarquee(vsmwp);
			vsmwp->marquee = FALSE;
		}
		return;
	}
	
	sp = FindSegment(v, p, &segID, &primID, &primCt);
	if (sp == NULL)
	{
		if (vsmwp->marquee)
		{
			VSMMarquee(vsmwp);
			vsmwp->marquee = FALSE;
		}
		return;
	}

	SegmentBox(sp, &box);
	pi.x = box.left;
	pi.y = box.top;
	MapWorldToViewer(v, pi, &tl);
	pi.x = box.right;
	pi.y = box.bottom;
	MapWorldToViewer(v, pi, &br);
	LoadRect(&(vsmwp->r), tl.x, tl.y, br.x, br.y);
	LoadPt (&(vsmwp->lastpnt), p.x, p.y);
	vsmwp->marquee = TRUE;
	VSMMarquee(vsmwp);
	
	vsmwp->itemtype1 = segID;
	vsmwp->itemID1 = primID;
	top = sp;
	prev = sp;
	while ((next = ParentSegment(top)) != NULL)
	{
		prev = top;
		top = next;
	}
	vsmwp->entityID1 = (Uint2)SegmentID(prev);
			
	return;
}

static void VSeqMgrReleaseProc (VieweR v, SegmenT s, PoinT p)
{
	VSMWinPtr vsmwp;
	SegmenT sp, top, next, prev;
	Uint2 segID, primID, primCt, entityID=0, itemtype = 0, itemID = 0;
	Uint1 highlight = PLAIN_SEGMENT;
	Int2 expand = 0;
	Boolean do_drag = FALSE;
	SeqViewProcsPtr  svpp;
	Int2  handled;

	vsmwp = (VSMWinPtr)GetViewerData(v);
	if (vsmwp == NULL)
	{
		Message(MSG_ERROR, "vsmwp was NULL in ReleaseProc");
		return;
	}
	
	if (vsmwp->marquee)
	{
		VSMMarquee(vsmwp);
		vsmwp->marquee = FALSE;
	}
		
	sp = FindSegment(v, p, &segID, &primID, &primCt);
	if (sp == NULL)
	{
		if (! vsmwp->dragged)
			ObjMgrDeSelectAll();
		else
			do_drag = TRUE;
	}
	else
	{
		itemtype = segID;
		itemID = primID;
		top = sp;
		prev = sp;
		while ((next = ParentSegment(top)) != NULL)
		{
			prev = top;
			top = next;
		}
		entityID = (Uint2)SegmentID(prev);
       /***
		Message(MSG_OK, "Calling ObjMgrSelect(%ld, %ld, %ld)",
			(long)entityID, (long)itemID, (long)itemtype);
		***/
        
        if (vsmwp->dblclick) {
          svpp = (SeqViewProcsPtr) GetAppProperty ("SeqDisplayForm");
          if (svpp != NULL) {
            if (svpp->launchEditors) {
              WatchCursor ();
              Update ();
              handled = GatherProcLaunch (OMPROC_EDIT, FALSE, entityID, itemID,
                                          itemtype, 0, 0, itemtype, 0);
              ArrowCursor ();
              Update ();
              if (handled == OM_MSG_RET_DONE || handled == OM_MSG_RET_NOPROC) {
                return;
              }
            }
          }
          return;
        } else if ((! vsmwp->dragged) ||
        	((entityID == vsmwp->entityID1) &&
        	 (itemID == vsmwp->itemID1) &&
        	 (itemtype == vsmwp->itemtype1)))
        {
        	if (itemID == VSM_PICT_UP_BUTTON)
        		expand = 1;
        	else if (itemID == VSM_PICT_DOWN_BUTTON)
        		expand = -1;
        		
        	if (! expand) {
        		if (vsmwp->shftkey) {
					ObjMgrAlsoSelect(entityID, itemID, itemtype,0,NULL);
        		} else {
					ObjMgrSelect(entityID, itemID, itemtype,0,NULL);
				}
			}
			else
			{
				WatchCursor();
				VSMAddPictureToEntity(vsmwp, entityID, expand);
				VSMWinRefreshData(vsmwp, entityID, 0, 0);
				ArrowCursor();
			}
			return;
		}
		else
			do_drag = TRUE;
	}

	if (do_drag)
		VSMDragAndDrop(vsmwp, entityID, itemID, itemtype);

	return;
}

static void VSeqMgrDragProc (VieweR v, SegmenT s, PoinT p)
{
	VSMWinPtr vsmwp;

	vsmwp = (VSMWinPtr)GetViewerData(v);
	if (vsmwp == NULL)
	{
		Message(MSG_ERROR, "vsmwp was NULL in DragProc");
		return;
	}
	
	if (vsmwp->marquee)
	{
		vsmwp->dragged = TRUE;
		VSMMarquee(vsmwp);
		OffsetRect(&(vsmwp->r), p.x - vsmwp->lastpnt.x, p.y - vsmwp->lastpnt.y);
		VSMMarquee(vsmwp);
		LoadPt(&(vsmwp->lastpnt), p.x, p.y);
	}
			
	return;
}

static void VSMCenterLine (Nlm_RectPtr rptr, Nlm_CharPtr text, Nlm_FonT fnt)
{
  if (fnt != NULL) {
    Nlm_SelectFont (fnt);
  }
  rptr->bottom = rptr->top + Nlm_LineHeight ();
  Nlm_DrawString (rptr, text, 'c', FALSE);
  rptr->top = rptr->bottom;
}

static void VSMDrawAbout (Nlm_PaneL p)

{
  Nlm_RecT  r;
	VSeqMgrPtr vsmp;
	CharPtr ptr, tmp;
	Char last = 'x';
	FonT f1, f2;
	Int2 bottom;

#ifdef WIN_MAC
	f1 = ParseFont("Monaco,12");
	f2 = ParseFont("Monaco,9");
#else
#ifdef WIN_MOTIF
	f1 = ParseFont("Times,12");
	f2 = ParseFont("Times,9");
#else
	f1 = ParseFont("Times New Roman,12");
	f2 = ParseFont("Times New Roman,9");
#endif
#endif
	vsmp = VSeqMgrGet();
  Nlm_ObjectRect (p, &r);
  Nlm_InsetRect (&r, 4, 4);
  bottom = r.bottom;
  Nlm_Blue ();
  VSMCenterLine (&r, "The NCBI DeskTop v1.0", f1);
  VSMCenterLine (&r, "by James M. Ostell", f2);
  VSMCenterLine (&r, "National Center for Biotechnology Information", f2);
  VSMCenterLine (&r, "National Institutes of Health, Bethesda, MD USA", f2);
  VSMCenterLine (&r, "(301) 496-2475  info@ncbi.nlm.nih.gov", f2);

  Nlm_Red();
 
  SelectFont(f2);
  r.top += LineHeight();
    
    if (vsmp->title != NULL)
    	VSMCenterLine(&r, vsmp->title, f1);
	if (vsmp->aboutinfo != NULL)
	{
		ptr = vsmp->aboutinfo;
		tmp = ptr;
		while ((*ptr != '\0') && (r.top <= bottom))
		{
			while ((*tmp != '\0') && (*tmp != '~'))
				tmp++;
			last = *tmp;
			*tmp = '\0';
			VSMCenterLine(&r, ptr, f2);
			*tmp = last;
			tmp++;
			ptr = tmp;
		}
	}
	return;
}

static void QuitVSMAbout (WindoW w)
{
	Remove(w);
	return;
}

static void VSMAboutProc (IteM i)
{
	WindoW w;
	PaneL p;

	w = FixedWindow (-50, -33, -10, -10, "About NCBI DeskTop", QuitVSMAbout);
	p = SimplePanel (w, 28 * stdCharWidth, 12 * stdLineHeight, VSMDrawAbout);
	Show (w);

	return;
}

static void QuitVSM (WindoW w)
{
	QuitProgram();
	return;
}

static void VSMQuitProc (IteM i)
{
	QuitProgram();
	return;
}

static void VSMSetUpMenus (WindoW w, Boolean progwindow)
{
	MenU m;

	if (progwindow)
	{
#ifdef WIN_MAC
		m = AppleMenu (NULL);
		CommandItem (m, "About...", VSMAboutProc);
		SeparatorItem (m);
		DeskAccGroup (m);
#endif
		m = PulldownMenu (w, "File");
#ifndef WIN_MAC
		CommandItem (m, "About...", VSMAboutProc);
		SeparatorItem (m);
#endif
	}
	else
	{
		m = PulldownMenu (w, "File");
	}

	VSMAddToMenu(m, VSM_OPEN_MENU);
	VSMAddToMenu(m, VSM_SAVE_MENU);

	if (progwindow)
		CommandItem(m, "Quit/Q", VSMQuitProc);
	else
		CommandItem(m, "Quit/Q", VSeqMgrQuitProc);

	m = PulldownMenu (w, "Edit");
	CommandItem(m, "Edit/E", VSeqMgrEditProc);
	CommandItem(m, "Cut/T", VSeqMgrCutProc);
	CommandItem(m, "Copy/C", VSeqMgrCopyProc);
	CommandItem(m, "Paste/P", VSeqMgrPasteProc);
	CommandItem(m, "Move/M", VSeqMgrMoveProc);
	CommandItem(m, "Delete/D", VSeqMgrDeleteProc);
	VSMAddMenu(w, VSM_FILTER_MENU);
	VSMAddMenu(w, VSM_VIEW_MENU);
	VSMAddMenu(w, VSM_ANALYZE_MENU);
	VSMAddMenu(w, VSM_EDIT_MENU);
	m = PulldownMenu(w, "Options");
	CommandItem(m, "Select Font", VSeqMgrFontProc);
	StatusItem(m, "Show cached", VSeqMgrShowCacheProc);
	StatusItem(m, "Show clipboard", VSeqMgrShowClipboardProc);

	return;
}

/*****************************************************************************
*
*   VSeqMgrCreateMainWindow()
*
*****************************************************************************/
typedef struct vsmform {
  FORM_MESSAGE_BLOCK
} VSMForm, PNTR VSMFormPtr;

static void VSMFormMessage (ForM f, Int2 mssg)

{
  VSMFormPtr  vfp;

  vfp = (VSMFormPtr) GetObjectExtra (f);
  if (vfp != NULL) {
    switch (mssg) {
      case VIB_MSG_CLOSE :
        QuitVSeqMgr ((WindoW) f);
        break;
      default :
        if (vfp->appmessage != NULL) {
          vfp->appmessage (f, mssg);
        }
        break;
    }
  }
}

static Boolean NEAR VSeqMgrCreateMainWindow(VSeqMgrPtr vsmp, Boolean progwindow)
{
	VSMWinPtr vsmwp;
	OMUserDataPtr omudp;
	WindoW w;
	VieweR v;
	RecT rw, rv;
	FonT f;
	Char fontbuf[80];
	ObjMgrPtr omp;
	static CharPtr dttitle = "NCBI DeskTop";
	CharPtr title;
	VSMFormPtr vfp;
	Int2 pixheight, pixwidth;
	StdEditorProcsPtr sepp;
	TextViewProcsPtr tvpp;
 
	ProcessUpdatesFirst(FALSE);

	WatchCursor();
	if (vsmp->title != NULL)
		title = vsmp->title;
	else
		title = dttitle;
	w = NULL;
	if (progwindow)
	{
		WatchCursor ();
		ErrSetFatalLevel (SEV_MAX);

		if (! SeqEntryLoad())
		{
			Message(MSG_ERROR, "SeqEntryLoadFailed");
			ErrShow();
			ArrowCursor();
			return FALSE;
		}
		if (! SubmitAsnLoad())
		{
			Message(MSG_ERROR, "SubmitAsnLoadLoadFailed");
			ErrShow();
			ArrowCursor();
			return FALSE;
		}

			    /* initialize File I/O procs for VSM (vsmfile.h) */
                /* could also add to your menu with VSMAddToMenu() (vsm.h) */
		VSMFileInit();
#ifdef WIN_MAC
			VSMSetUpMenus (NULL, progwindow);
#endif
		w = DocumentWindow (-50, -33, -10, -10, title, QuitVSM, ResizeVSeqMgr);
#ifndef WIN_MAC
			VSMSetUpMenus (w, progwindow);
#endif
	}
	else
	{
		w = DocumentWindow (-50, -33, -10, -10, title, QuitVSeqMgr, ResizeVSeqMgr);
		VSMSetUpMenus (w, progwindow);
	}
	pixwidth = 300;
	pixheight = 300;
	if (w != NULL) {
		vfp = (VSMFormPtr) MemNew (sizeof (VSMForm));
		if (vfp != NULL) {
			SetObjectExtra (w, vfp, StdCleanupFormProc);
			vfp->form = (ForM) w;
			vfp->formmessage = VSMFormMessage;
			sepp = (StdEditorProcsPtr) GetAppProperty ("StdEditorForm");
			if (sepp != NULL) {
				SetActivate (w, sepp->activateForm);
				vfp->appmessage = sepp->handleMessages;
			}
		}
	}

	omp = vsmp->omp;

	tvpp = (TextViewProcsPtr) GetAppProperty ("TextDisplayForm");
	if (tvpp != NULL) {
		pixwidth = MAX (pixwidth, tvpp->minPixelWidth);
		pixheight = MAX (pixheight, tvpp->minPixelHeight);
	}

    v = CreateViewer(w, pixwidth, pixheight, TRUE, TRUE);
    RealizeWindow(w);
    ObjectRect(w, &rw);   /* calculate realation between window and viewer */
    ObjectRect(v, &rv);
    vsmp->xoff = rv.left;
    vsmp->yoff = rv.top;
    vsmp->x = (rw.right - rw.left + 1) - (rv.right - rv.left + 1) - rv.left
    	 - vScrollBarWidth;
    vsmp->y = (rw.bottom - rw.top + 1) - (rv.bottom - rv.top + 1) - rv.top
    	 - hScrollBarHeight;
	GetAppParam(vsmp->appname, "DeskTop", "FONT", "Times,10", fontbuf, 80);
	f = ParseFont(fontbuf);
	SetWindowExtra(w, (Pointer)v, NULL);
	vsmp->font = f;
	SelectFont(f);
	vsmp->lineheight = LineHeight();
	vsmp->leading = Leading();
	vsmp->charw = MaxCharWidth();
	vsmp->set_atp = AsnFind("Bioseq-set.class");
	vsmp->procid = ObjMgrProcLoad(OMPROC_EDIT, "NCBI DeskTop", "NCBI DeskTop",
		0, 0, 0,0,(Pointer)vsmp, VSeqMgrGenFunc, PROC_PRIORITY_HIGHEST);
	vsmp->proctype = OMPROC_EDIT;

	vsmwp = VSMWinNew(vsmp);
	vsmwp->w = w;
	InsetRect(&rv, 2,2);  /* allow space when clipping marquee */
	LoadRect(&(vsmwp->rv), rv.left, rv.top, rv.right, rv.bottom);
	
	omudp = ObjMgrAddUserData(0, vsmp->procid, OMPROC_EDIT, vsmwp->userkey);
	omudp->userdata.ptrvalue = (Pointer)vsmwp;
	omudp->messagefunc = VSeqMgrMsgFunc;
	vsmwp->msgfunc = VSeqMgrMsgFunc;

	ArrowCursor();
	return TRUE;
}

/*****************************************************************************
*
*   VSeqMgrPopulateMain(vsmwp)
*
*****************************************************************************/
static void get_viewer_position (VieweR viewer, Int4Ptr x, Int4Ptr y)

{
  Int4 scale_x, scale_y;
  BoxInfo port, world;
  RecT v_iew;

  ViewerBox (viewer, &world, &port, &v_iew, &scale_x, &scale_y);
  *x = (port.left + port.right) / 2;
  *y = (port.top + port.bottom) / 2;
}

static void NEAR VSeqMgrPopulateMain(VSMWinPtr vsmwp)
{
	VieweR v = NULL;
	RecT rw, rv;
	BoxInfo box;
	SegmenT picture, seg;
	Int2 i, currnum, lineheight,right_frame;
	Int4 x, y, top,width, bigtop, diffx, diffy;
	ObjMgrDataPtr omdp, PNTR omdpp;
	ObjMgrPtr omp;
	AsnTypePtr atp;
	WindoW w;
	VSeqMgrPtr vsmp;
	PrimitivE p;
	OMUserDataPtr omudp;
	VSMPictPtr vsmpp;
	Boolean doit;
	Int4 vwr_x, vwr_y, vwr_align;

    /***
	Message(MSG_OK, "In VSeqMgrPopulateMain");
    ***/
	
	
	if (vsmwp == NULL) return;
	vsmp = (VSeqMgrPtr)(vsmwp->vsmp);
	w = vsmwp->w;

	if (vsmp->set_atp == NULL)
		vsmp->set_atp = AsnFind("Bioseq-set.class");
    atp = vsmp->set_atp;
    
    ObjectRect(w, &rw);

    LoadRect(&rv, vsmp->xoff, vsmp->yoff, rw.right - rw.left - vsmp->x,
    	rw.bottom - rw.top - vsmp->y);
    	
    v = (VieweR) GetWindowExtra(w);
    vwr_x = INT4_MIN;
    vwr_y = INT4_MAX;
    vwr_align = UPPER_LEFT;
    if (v != NULL && vsmwp->picture != NULL) {
      get_viewer_position (v, &(vwr_x), &(vwr_y));
      vwr_align = MIDDLE_CENTER;
    }
    ResetViewer(v);
    SetPosition(v, &rv);
	LoadRect(&(vsmwp->rv), rv.left, rv.top, rv.right, rv.bottom);
	InsetRect(&(vsmwp->rv), 2,2);  /* allow space when clipping marquee */
	    
	right_frame = rv.right - rv.left + 1;

	if (vsmwp->picture == NULL)
		vsmwp->picture = CreatePicture();
	else
	{					
		while ((p = GetPrimitive(vsmwp->picture, 1)) != NULL)
		{
			UnlinkSegment((SegmenT)p);
		}
	}
	
	picture = vsmwp->picture;
	lineheight = vsmp->lineheight;

	omp = vsmp->omp;	
	currnum = omp->currobj;
	omdpp = omp->datalist;
	x = 0;
	y = 0;
	bigtop = 0;
	for (i = 0; i < currnum; i++, omdpp++)
	{
		omdp = *omdpp;
		if ((omdp->parentptr == NULL) && (omdp->EntityID != 0))
		{
			doit = TRUE;
			if ((! vsmp->show_cached) && (omdp->tempload == TL_CACHED))
				doit = FALSE;
			if ((! vsmp->show_clipboard) && (omdp->clipboard))
				doit = FALSE;

			if (doit)
			{
				omudp = ObjMgrGetUserData (omdp->EntityID, vsmwp->procid,
									vsmwp->proctype, vsmwp->userkey);
				if ((omudp == NULL) || (vsmp->update_all))  /* don't have one, or doing all */
					omudp = VSMAddPictureToEntity(vsmwp, omdp->EntityID, 0);
				vsmpp = (VSMPictPtr)(omudp->userdata.ptrvalue);
				seg = vsmpp->s;
				if ((seg != NULL) && (! vsmpp->deleted))
				{
					SegmentBox(seg, &box);
					width = box.right - box.left + 1;
					if ((x + width) > right_frame)
					{
						x = 0;
						y = bigtop - lineheight;
					}

					diffx = x - box.left;
					diffy = y - box.top;
					
					OffsetSegment(seg, diffx, diffy);
				   
					top = y - (box.top - box.bottom + 1) - lineheight;
					if (top < bigtop)
						bigtop = top;

					LinkSegment(picture, seg);

					x += (width + lineheight);
                }
			}
		}
	}

	vsmwp->populated = TRUE;
	AttachPicture (v, picture, vwr_x, vwr_y, vwr_align, 1, 1, NULL);
	SetViewerProcs(v, VSeqMgrClickProc, VSeqMgrDragProc, VSeqMgrReleaseProc, NULL);
	SetViewerData(v, (Pointer)vsmwp, NULL);
	if (Visible(w))
		vsmwp->shown = TRUE;   /* attach picture will show it */
	else
		vsmwp->shown = FALSE;
    return;

}

/*****************************************************************************
*
*   VSMWinNew(vsmp)
*   	adds a VSMWin to the chain
*   	if first one, labels as is_main_VSM
*
*****************************************************************************/
static VSMWinPtr NEAR VSMWinNew (VSeqMgrPtr vsmp)
{
	VSMWinPtr vsmwp=NULL, tmp;

	vsmwp = MemNew(sizeof(VSMWin));
	vsmwp->vsmp = (Pointer)vsmp;

	if (vsmp->wins == NULL)
	{
		vsmp->wins = vsmwp;
		vsmwp->is_main_VSM = TRUE;
	}
	else
	{
		for (tmp = vsmp->wins; tmp->next != NULL; tmp = tmp->next)
			continue;
		tmp->next = vsmwp;
	}
	vsmwp->userkey = (++(vsmp->highest_userkey));
	vsmwp->procid = vsmp->procid;
	vsmwp->proctype = vsmp->proctype;
	return vsmwp;
}

/*****************************************************************************
*
*   VSMWinDelete(vsmwp, entityID, itemID, itemtype)
*   	deletes vsmwp from chain
*
*****************************************************************************/
static void NEAR VSMWinDelete(VSMWinPtr vsmwp, Uint2 entityID, Uint2 itemID, Uint2 itemtype)
{
	VSeqMgrPtr vsmp;
	VSMWinPtr tmp, prev=NULL, next=NULL;
	VieweR v;
	ObjMgrDataPtr omdp, PNTR omdpp;
	ObjMgrPtr omp;
	Int2 i, currnum;

	if (vsmwp == NULL) return;

	vsmp = VSeqMgrGet();
	omp = vsmp->omp;
	omdpp = omp->datalist;
	currnum = omp->currobj;

	if (vsmwp->entityID == 0)
	{
		for (i = 0; i < currnum; i++, omdpp++)
		{
			omdp = *omdpp;
			if (omdp->EntityID)
				ObjMgrFreeUserData(omdp->EntityID, vsmwp->procid, vsmwp->proctype, vsmwp->userkey);
		}
	}
	
	ObjMgrFreeUserData(entityID, vsmwp->procid, vsmwp->proctype, vsmwp->userkey);

	if (vsmwp->is_main_VSM)   /* don't really delete */
	{                         /* if entityID != 0, could do a better refresh */
		v = (VieweR)GetWindowExtra(vsmwp->w);
		ResetViewer(v);
		Hide(vsmwp->w);
		vsmwp->picture = DeletePicture(vsmwp->picture);

		vsmwp->in_process = FALSE;
		vsmwp->populated = FALSE;
		vsmwp->shown = FALSE;
		return;
	}

	                          /* really delete */
	Remove(vsmwp->w);

	for (tmp = vsmp->wins; tmp != NULL; tmp = next)
	{
		next = tmp->next;
		if (tmp == vsmwp)
		{
			if (prev == NULL)
				vsmp->wins = next;
			else
				prev->next = next;
			MemFree(tmp);
			return;
		}
	}

	return;
}

/*****************************************************************************
*
*   VSeqMgrGetWinForW(w)
*   	finds the VSMWinPtr for w
*   	if w==NULL, finds the main_VSM VSMWinPtr
*
*****************************************************************************/
static VSMWinPtr NEAR VSeqMgrGetWinForW(WindoW w)
{
	VSeqMgrPtr vsmp;
	VSMWinPtr tmp;

	vsmp = VSeqMgrGet();
	for (tmp = vsmp->wins; tmp != NULL; tmp = tmp->next)
	{
		if (w == NULL)
		{
			if (tmp->is_main_VSM)
				return tmp;
		}
		else
		{
			if (tmp->w == w)
				return tmp;
		}
	}

	return tmp;
}

/*****************************************************************************
*
*   VSeqMgrGenFunc()
*   	General function to call from ObjMgr
*
*****************************************************************************/
Int2 LIBCALLBACK VSeqMgrGenFunc (Pointer ptr)
{
	VSeqMgrPtr vsmp;
	OMProcControlPtr ompcp;
	VSMWinPtr vsmwp;

	ompcp = (OMProcControlPtr)ptr;
	vsmp = (VSeqMgrPtr)(ompcp->proc->procdata);
	vsmwp = VSMWinNew(vsmp);
	vsmwp->entityID = ompcp->input_entityID;
	vsmwp->itemID = ompcp->input_itemID;
	vsmwp->itemtype = ompcp->input_itemtype;

	        /* make the picture and the window */

	return OM_MSG_RET_DONE;
}

/*****************************************************************************
*
*   VSeqMgrMsgFunc()
*   	function to call on each DeskTop window by the ObjMgr
*
*****************************************************************************/
Int2 LIBCALLBACK VSeqMgrMsgFunc (OMMsgStructPtr ommsp)
{
	OMUserDataPtr omudp;
	VSMWinPtr vsmwp;
   
    /***
	Message(MSG_OK, "VSeqMgrMsgFunc [%d]", (int)message);
	***/

	
	omudp = (OMUserDataPtr)(ommsp->omuserdata);
	vsmwp = (VSMWinPtr)(omudp->userdata.ptrvalue);

	switch (ommsp->message)
	{
		case OM_MSG_DEL:
			if (! ommsp->entityID)  /* deleting desktop */
				VSMWinDelete(vsmwp, ommsp->entityID, ommsp->itemID, ommsp->itemtype);
			break;
		case OM_MSG_CREATE:
			VSMWinRefreshData(vsmwp, ommsp->entityID, ommsp->itemID, ommsp->itemtype);
			break;
		case OM_MSG_UPDATE:
			break;
		case OM_MSG_SELECT:          /* add highlight code */
			break;
		case OM_MSG_DESELECT:        /* add deselect code */
			break;
		case OM_MSG_CACHED:
			break;
		case OM_MSG_UNCACHED:
			break;
		case OM_MSG_TO_CLIPBOARD:  /* this is just because no clipboard now */
			Beep();
			VSMWinRefreshData(vsmwp, ommsp->entityID, ommsp->itemID, ommsp->itemtype);
			break;
		default:
			break;
	}

	return OM_MSG_RET_OK;
}

/*****************************************************************************
*
*   VSMPictMsgFunc()
*   	function to call on each entity window by the ObjMgr
*
*****************************************************************************/
Int2 LIBCALLBACK VSMPictMsgFunc (OMMsgStructPtr ommsp)
{
	OMUserDataPtr omudp;
	VSMWinPtr vsmwp;
	VSMPictPtr vsmpp=NULL;
	Boolean do_add = FALSE, do_refresh = FALSE;
   
    /***
	Message(MSG_OK, "VSMPictMsgFunc [%d]", (int)message);
	***/

	
	omudp = (OMUserDataPtr)(ommsp->omuserdata);
	vsmpp = (VSMPictPtr)(omudp->userdata.ptrvalue);
	vsmwp = vsmpp->vsmwin;
	

	switch (ommsp->message)
	{
		case OM_MSG_DEL:
			if ((vsmwp->entityID == ommsp->entityID) &&
					(vsmwp->itemID == ommsp->itemID) &&
					(vsmwp->itemtype == ommsp->itemtype))
				VSMWinDelete(vsmwp, ommsp->entityID, ommsp->itemID, ommsp->itemtype);
			else if ((! ommsp->itemID) && (! ommsp->itemtype))
			{
				vsmpp->deleted = TRUE;
				do_refresh = TRUE;
			}
			else
				do_add = TRUE;
			break;
		case OM_MSG_CREATE:  /* this is called because desktop function added picture */
			break;
		case OM_MSG_UPDATE:
			do_add = TRUE;
			break;
		case OM_MSG_SELECT:          /* add highlight code */
			VSMPictSelect(vsmpp, ommsp->entityID, ommsp->itemID, ommsp->itemtype, TRUE);
			break;
		case OM_MSG_DESELECT:        /* add deselect code */
			VSMPictSelect(vsmpp, ommsp->entityID, ommsp->itemID, ommsp->itemtype, FALSE);
			break;
		case OM_MSG_CACHED:
			do_add = TRUE;
			break;
		case OM_MSG_UNCACHED:
			do_add = TRUE;
			break;
		case OM_MSG_TO_CLIPBOARD:
			if (vsmwp->is_clipboard)
				do_refresh = TRUE;
			break;
		default:
			break;
	}
	if (do_add)
	{
		VSMAddPictureToEntity(vsmwp, ommsp->entityID, 0);
		do_refresh = TRUE;
	}

	if (do_refresh)
	{
		VSMWinRefreshData(vsmwp, ommsp->entityID, ommsp->itemID, ommsp->itemtype);
	}

	return OM_MSG_RET_OK;
}

typedef struct vsmhlp {
	Uint2 h_item, h_type;
	VieweR h_v;
	Int1 highlight;
} VSMHlp, PNTR VSMHlpPtr;

static Boolean VSMHighlightProc (SegmenT seg, PrimitivE prim, Uint2 segid, Uint2 primID, Uint2 primct, Pointer data)
{
	VSMHlpPtr vhp;

	vhp = (VSMHlpPtr)data;
	
	if (segid == vhp->h_type)
	{
		if (primID == vhp->h_item)
		{
			HighlightSegment(vhp->h_v, seg, vhp->highlight);
			return FALSE;
		}
	}
	return TRUE;
}

/*****************************************************************************
*
*	VSMPictSelect(vspp, entityID, itemID, itemtype, select)
*
*****************************************************************************/
static void NEAR VSMPictSelect(VSMPictPtr vsmpp, Uint2 entityID, Uint2 itemID, Uint2 itemtype, Boolean select)
{
	
	VieweR v;
	VSMHlp vh;
	VSMWinPtr vsmwp;


	if (vsmpp == NULL) return;
	if (vsmpp->s == NULL) return;
	if (vsmpp->vsmwin == NULL) return;
    vsmwp = (VSMWinPtr)(vsmpp->vsmwin);
	v = GetWindowExtra(vsmwp->w);

	vh.h_v = v;
	vh.h_item = itemID;
	vh.h_type = itemtype;
	if (select)
		vh.highlight = OUTLINE_SEGMENT;
	else
		vh.highlight = PLAIN_SEGMENT;


	if (! itemID)
		HighlightSegment(v, vsmpp->s, vh.highlight);
	else
		ExploreSegment(vsmpp->s, (Pointer)(&vh), VSMHighlightProc);

 	return;
}

static void add_frame (SegmenT seg, Int4 left, Int4 top, Int4 right,
                                                     Int4 bottom, Uint2 primID)
{
  AddInvFrame ( seg, left, top, right, bottom, primID );
}

typedef struct vsmgp {
	SegmenT segs[20];
	VSeqMgrPtr vsmp;
	Int4 currline;
	Uint1 level[OBJ_MAX];
	SegmenT annotseg, descrseg;
} VSMGatherProcST, PNTR VSMGatherProcSTPtr;

static Boolean VSMGatherPictProc (GatherContextPtr gcp)
{
	VSMGatherProcSTPtr vsmgp;
	VSeqMgrPtr vsmp;
	Char buf[81];
	CharPtr tmp;
	Int2 i, k, buflen;
	Int4 top, lineheight, left, bottom, width, maxwidth;
	SegmenT seg;
	Uint1 shading, setcolor[3];
	Uint1Ptr tcolor;
	BioseqPtr bsp;
	BioseqSetPtr bssp;
	ObjMgrTypePtr omtp;
	ValNode vn;
	SeqFeatPtr sfp;
	SeqAnnotPtr annot;
	SeqGraphPtr sgp;
	ValNodePtr desc;
	ObjectIdPtr oip;
	SeqIdPtr sip;
	UserFieldPtr ufp;
	UserObjectPtr uop;
	SeqEntryPtr sep;
	SeqEntryPtr oldsep;

	vsmgp = (VSMGatherProcSTPtr)(gcp->userdata);
	vsmp = vsmgp->vsmp;
	lineheight = vsmp->lineheight;
	top = vsmgp->currline * (lineheight + 2);
	left = vsmp->charw * gcp->indent;

	i = gcp->indent + 1;   /* level 0 is the entity segment */

	SelectFont(vsmp->font);
	omtp = ObjMgrTypeFind(vsmp->omp, gcp->thistype, NULL, NULL);

	sep = GetTopSeqEntryForEntityID (gcp->entityID);
	oldsep = SeqEntrySetScope (sep);

	switch (gcp->thistype)
	{
		case OBJ_SEQSUB:
		case OBJ_SUBMIT_BLOCK:
			if (vsmgp->level[gcp->thistype] < 2)
				buflen = 40;
			else
				buflen = 80;
			seg = CreateSegment(vsmgp->segs[gcp->indent], (Uint2)(gcp->thistype), 0);
			vsmgp->segs[i] = seg;
			bottom = top - lineheight;

			AddAttribute(seg, COLOR_ATT, WHITE_COLOR,0, 0, 0, 0);
			AddSegRect(seg, TRUE, (Uint2)(gcp->itemID));
			AddAttribute(seg, COLOR_ATT, BLACK_COLOR,0, 0, 0, 0);
			AddSegRect(seg, FALSE, (Uint2)(gcp->itemID));
			if (gcp->thistype == OBJ_SEQSUB)
				StringMove(buf, "Data Submission");
			else
				(*(omtp->labelfunc))(gcp->thisitem, buf, buflen, OM_LABEL_BOTH);
			
			width = StringWidth(buf) + (2 * vsmp->charw);
				               /* this just forces the segment size */
			add_frame(seg, left, top-2, (left+width), (top-lineheight+2), (Uint2)(gcp->itemID));
			AddTextLabel(seg, left+(vsmp->charw), top, buf, vsmp->font,0,LOWER_RIGHT,(Uint2)(gcp->itemID));
			vsmgp->currline--;
			break;
		case OBJ_SEQSUB_CONTACT:
		case OBJ_SEQSUB_CIT:
			if (vsmgp->level[gcp->thistype] < 2)
				buflen = 40;
			else
				buflen = 80;
			seg = CreateSegment(vsmgp->segs[gcp->indent], (Uint2)(gcp->thistype), 0);
			vsmgp->segs[i] = seg;
			bottom = top - lineheight;

			if (gcp->thistype == OBJ_SEQSUB_CIT)
			{
				vn.choice = PUB_Sub;
				vn.data.ptrvalue = gcp->thisitem;
				vn.next = NULL;
				omtp = ObjMgrTypeFind(vsmp->omp, OBJ_PUB, NULL, NULL);
				tmp = StringMove(buf, "Cit: ");
				(*(omtp->labelfunc))((Pointer)(&vn), tmp, (buflen - 5), OM_LABEL_CONTENT);
			}
			else
				(*(omtp->labelfunc))(gcp->thisitem, buf, buflen, OM_LABEL_BOTH);
			width = StringWidth(buf) + (2 * vsmp->charw);
				               /* this just forces the segment size */
			add_frame(seg, left, top-2, (left+width), (top-lineheight+2), (Uint2)(gcp->itemID));
			AddAttribute(seg, COLOR_ATT, BLACK_COLOR,0, 0, 0, 0);
			AddTextLabel(seg, left+(vsmp->charw), top, buf, vsmp->font,0,LOWER_RIGHT,(Uint2)(gcp->itemID));
			vsmgp->currline--;
			break;
		case OBJ_BIOSEQSET:
			seg = CreateSegment(vsmgp->segs[gcp->indent], (Uint2)(gcp->thistype), 0);
			vsmgp->segs[i] = seg;
			bssp = (BioseqSetPtr)(gcp->thisitem);
			bottom = top - lineheight;

			if (gcp->indent > 3)
				k = 3;
              else
				k = gcp->indent;
			shading = (Uint1)(140 + (k * 20));
			
			for (k = 0; k < 3; k++)
				setcolor[k] = shading;

			AddAttribute(seg, COLOR_ATT, setcolor,0, 0, 0, 0);
			AddSegRect(seg, TRUE, (Uint2)(gcp->itemID));
			(*(omtp->labelfunc))(gcp->thisitem, buf, 40, OM_LABEL_TYPE);
			width = StringWidth(buf) + (2 * vsmp->charw);
				               /* this just forces the segment size */
			add_frame(seg, left, top-2, (left+width), (top-lineheight+2), (Uint2)(gcp->itemID));
			AddAttribute(seg, COLOR_ATT, BLUE_COLOR, 0, 0, 0,0);
			AddTextLabel(seg, left+(vsmp->charw), top, buf, vsmp->font,0,LOWER_RIGHT,(Uint2)(gcp->itemID));
			vsmgp->currline--;
			break;
		case OBJ_BIOSEQ:
			seg = CreateSegment(vsmgp->segs[gcp->indent], (Uint2)(gcp->thistype), 0);
			vsmgp->segs[i] = seg;
			(*(omtp->labelfunc))(gcp->thisitem, buf, 40, OM_LABEL_CONTENT);
			bsp = (BioseqPtr)(gcp->thisitem);
			width = StringWidth(buf) + (2 * vsmp->charw);
			maxwidth = width;

			if (ISA_aa(bsp->mol))
				tcolor = MAGENTA_COLOR;
			else
				tcolor = RED_COLOR;
												/* just to force segment size */
			AddAttribute(seg, COLOR_ATT , WHITE_COLOR, 0, 0,0,0);
			
			if (vsmgp->level[OBJ_BIOSEQ] > 1)
				k = 2;
			else
				k = 1;
			AddSegRect(seg, TRUE, (Uint2)(gcp->itemID));
			AddAttribute(seg, COLOR_ATT , tcolor, 0, 0,0,0);
			AddSegRect(seg, FALSE, (Uint2)(gcp->itemID));
			AddTextLabel(seg, left + vsmp->charw, top, buf, vsmp->font,0, LOWER_RIGHT,(Uint2)(gcp->itemID));
			vsmgp->currline--;
			if (k == 2)
			{
				(*(omtp->labelfunc))(gcp->thisitem, buf, 40, OM_LABEL_TYPE);
				SelectFont(vsmp->font);
				width = StringWidth(buf) + (3 * vsmp->charw);
				if (width > maxwidth)
					maxwidth = width;
				AddTextLabel(seg, left + (2 * vsmp->charw), (top - lineheight), buf, vsmp->font,0, LOWER_RIGHT,(Uint2)(gcp->itemID));
				vsmgp->currline--;
			}
			if (vsmgp->level[OBJ_BIOSEQ] == 3)
			{
				if (bsp->repr == Seq_repr_seg)
				{
					k = 3;
					vn.choice = SEQLOC_MIX;
					vn.data.ptrvalue = bsp->seq_ext;
					vn.next = NULL;
					SeqLocLabel(&vn, buf, 80, OM_LABEL_CONTENT);
					SelectFont(vsmp->font);
					width = StringWidth(buf) + (3 * vsmp->charw);
					if (width > maxwidth)
						maxwidth = width;
					AddTextLabel(seg, left + (2 * vsmp->charw), (top - (2*lineheight)), buf, vsmp->font,0, LOWER_RIGHT,(Uint2)(gcp->itemID));
					vsmgp->currline--;
				}
			}
			add_frame(seg, left, top-2, (left+maxwidth), (top-(k * lineheight)+2), (Uint2)(gcp->itemID));
			break;
		case OBJ_SEQANNOT:
			(*(omtp->labelfunc))(gcp->thisitem, buf, 40, OM_LABEL_BOTH);
			if (vsmgp->level[OBJ_SEQANNOT] > 1) {
			  annot = (SeqAnnotPtr) gcp->thisitem;
			  if (annot != NULL) {
			    for (desc = annot->desc; desc != NULL; desc = desc->next) {
			      if (desc->choice == Annot_descr_name) {
			        StringCat (buf, " (");
			        StringNCat (buf, desc->data.ptrvalue, 35);
			        StringCat (buf, ")");
			      }
			    }
			  }
			}
			width = StringWidth(buf);
			seg = CreateSegment(vsmgp->segs[gcp->indent], (Uint2)(gcp->thistype), 0);
			vsmgp->segs[i] = seg;
			AddAttribute(seg, COLOR_ATT, BLUE_COLOR, 0,0,0,0);
			AddSegRect(seg, FALSE, (Uint2)(gcp->itemID));
			AddTextLabel(seg, left, top, buf, vsmp->font,0, LOWER_RIGHT,(Uint2)(gcp->itemID));
			add_frame(seg, left, top, (left+width), (top-lineheight), (Uint2)(gcp->itemID));
			vsmgp->currline--;
			break;
		case OBJ_SEQALIGN:
			if (vsmgp->level[OBJ_SEQALIGN] < 2)
				buflen = 40;
			else
				buflen = 80;
			seg = CreateSegment(vsmgp->segs[gcp->indent], (Uint2)(gcp->thistype), 0);
			vsmgp->segs[i] = seg;
			(*(omtp->labelfunc))(gcp->thisitem, buf, buflen, OM_LABEL_BOTH);
			width = StringWidth(buf) + (2 * vsmp->charw);
			AddAttribute(seg, COLOR_ATT , BLUE_COLOR, 0, 0,0,0);
			AddTextLabel(seg, left + vsmp->charw, top, buf, vsmp->font,0, LOWER_RIGHT,(Uint2)(gcp->itemID));
			add_frame(seg, left+vsmp->charw, top, (left+width), (top-lineheight), (Uint2)(gcp->itemID));
			vsmgp->currline--;
			break;
		case OBJ_SEQGRAPH:
			if (vsmgp->level[OBJ_SEQGRAPH] < 2)
				buflen = 40;
			else
				buflen = 80;
			seg = CreateSegment(vsmgp->segs[gcp->indent], (Uint2)(gcp->thistype), 0);
			vsmgp->segs[i] = seg;
			(*(omtp->labelfunc))(gcp->thisitem, buf, buflen, OM_LABEL_BOTH);
			width = StringWidth(buf) + (2 * vsmp->charw);
			AddAttribute(seg, COLOR_ATT , BLUE_COLOR, 0, 0,0,0);
			AddTextLabel(seg, left + vsmp->charw, top, buf, vsmp->font,0, LOWER_RIGHT,(Uint2)(gcp->itemID));
			vsmgp->currline--;
			maxwidth = width;
			k = 1;
			sgp = (SeqGraphPtr) gcp->thisitem;
			if (vsmgp->level[OBJ_SEQGRAPH] > 1 && sgp != NULL && sgp->loc != NULL) {
				k++;
				SeqLocLabel(sgp->loc, buf, buflen, OM_LABEL_CONTENT);
				SelectFont(vsmp->font);
				width = StringWidth(buf) + (3 * vsmp->charw);
				if (width > maxwidth)
					maxwidth = width;
				AddTextLabel(seg, left + (2*vsmp->charw), (top-lineheight), buf, vsmp->font,0, LOWER_RIGHT,(Uint2)(gcp->itemID));
				vsmgp->currline--;
			}
			add_frame(seg, left, top-2, (left+width), (top-(k*lineheight)+2), (Uint2)(gcp->itemID));
			/* add_frame(seg, left+vsmp->charw, top, (left+width), (top-lineheight), (Uint2)(gcp->itemID)); */
			break;
		case OBJ_SEQFEAT:
			if (vsmgp->level[OBJ_SEQFEAT] < 2)
				buflen = 40;
			else
				buflen = 80;
			seg = CreateSegment(vsmgp->segs[gcp->indent], (Uint2)(gcp->thistype), 0);
			vsmgp->segs[i] = seg;
			(*(omtp->labelfunc))(gcp->thisitem, buf, buflen, OM_LABEL_BOTH);
			width = StringWidth(buf) + (2 * vsmp->charw);
			AddAttribute(seg, COLOR_ATT , BLUE_COLOR, 0, 0,0,0);
			AddTextLabel(seg, left + vsmp->charw, top, buf, vsmp->font,0, LOWER_RIGHT,(Uint2)(gcp->itemID));
			vsmgp->currline--;
			maxwidth = width;
			k = 1;
			if (vsmgp->level[OBJ_SEQFEAT] == 2)
			{
				k++;
				sfp = (SeqFeatPtr)(gcp->thisitem);
				SeqLocLabel(sfp->location, buf, buflen, OM_LABEL_CONTENT);
				SelectFont(vsmp->font);
				width = StringWidth(buf) + (3 * vsmp->charw);
				if (width > maxwidth)
					maxwidth = width;
				AddTextLabel(seg, left + (2*vsmp->charw), (top-lineheight), buf, vsmp->font,0, LOWER_RIGHT,(Uint2)(gcp->itemID));
				vsmgp->currline--;
				if (vsmp != NULL && vsmp->extraLevel && sfp->product != NULL) {
					k++;
					StringCpy (buf, "product ");
					SeqLocLabel(sfp->product, buf + 8, buflen - 8, OM_LABEL_CONTENT);
					SelectFont(vsmp->font);
					width = StringWidth(buf) + (3 * vsmp->charw);
					if (width > maxwidth)
						maxwidth = width;
					AddTextLabel(seg, left + (2*vsmp->charw), (top-lineheight*2), buf, vsmp->font,0, LOWER_RIGHT,(Uint2)(gcp->itemID));
					vsmgp->currline--;
				}
				if (vsmp != NULL && vsmp->extraLevel && sfp->data.choice == SEQFEAT_RNA && sfp->ext != NULL) {
					uop = sfp->ext;
					if (uop->type != NULL && StringICmp (uop->type->str, "MrnaProteinLink") == 0) {
						ufp = uop->data;
						if (ufp != NULL && ufp->choice == 1) {
							oip = ufp->label;
							if (oip != NULL && oip->str != NULL && StringICmp (oip->str, "protein seqID") == 0) {
								tmp = (CharPtr) ufp->data.ptrvalue;
								if (tmp != NULL) {
									sip = MakeSeqID (tmp);
									if (sip != NULL) {
										vn.choice = SEQLOC_WHOLE;
										vn.data.ptrvalue = (Pointer) sip;
										k++;
										StringCpy (buf, "protein ");
										SeqLocLabel(&vn, buf + 8, buflen - 8, OM_LABEL_CONTENT);
										SelectFont(vsmp->font);
										width = StringWidth(buf) + (3 * vsmp->charw);
										if (width > maxwidth)
											maxwidth = width;
										AddTextLabel(seg, left + (2*vsmp->charw), (top-lineheight*(k-1)), buf, vsmp->font,0, LOWER_RIGHT,(Uint2)(gcp->itemID));
										vsmgp->currline--;
										SeqIdFree (sip);
									}
								}
							}
						}
					}
				}
			}
			add_frame(seg, left, top-2, (left+width), (top-(k*lineheight)+2), (Uint2)(gcp->itemID));
			break;
		case OBJ_SEQDESC:
			if (vsmgp->level[OBJ_SEQDESC] < 2)
				buflen = 40;
			else
				buflen = 80;
			seg = CreateSegment(vsmgp->segs[gcp->indent], (Uint2)(gcp->thistype), 0);
			vsmgp->segs[i] = seg;
			(*(omtp->labelfunc))(gcp->thisitem, buf, buflen, OM_LABEL_BOTH);
			width = StringWidth(buf) + (2 * vsmp->charw);
			add_frame(seg, left, top-2, (left+width), (top-lineheight+2), (Uint2)(gcp->itemID));
			AddAttribute(seg, COLOR_ATT , BLACK_COLOR, 0, 0,0,0);
			AddTextLabel(seg, left + vsmp->charw, top, buf, vsmp->font,0, LOWER_RIGHT,(Uint2)(gcp->itemID));
			vsmgp->currline--;
			break;
		default:
			break;

	}

	SeqEntrySetScope (oldsep);

	return TRUE;
}

/*****************************************************************************
*
*   VSMEntityDraw(omdp)
*
*****************************************************************************/
SegmenT VSMEntityDraw (ObjMgrDataPtr omdp, VSMPictPtr vsmpp, VSeqMgrPtr vsmp)
{
	VSMGatherProcST vsg;
	GatherScope gs;
	SegmenT top = NULL, seg;
	CharPtr tmp=NULL;
	Char buf[41];
	ObjMgrTypePtr omtp;
	Int2 width, expansion, i, width2;
	Boolean maxexpansion = FALSE;
	Int4 bottom, maxwidth;
	Uint1 tcolor[3];
	Boolean extraLevel = FALSE;

	MemSet(&vsg, 0, sizeof(VSMGatherProcST));
	MemSet(&gs, 0, sizeof(GatherScope));

	MemSet(gs.ignore, (int)(TRUE), (size_t)OBJ_MAX);
	gs.nointervals = TRUE;
	gs.do_not_reload_from_cache = TRUE;

	if (vsmpp->expansion < 0)
		vsmpp->expansion = 0;

	expansion = vsmpp->expansion;
		switch (omdp->datatype)
		{
			case OBJ_SEQSUB:
				if (expansion)
				{
					vsg.level[OBJ_SEQSUB] = 1;
					vsg.level[OBJ_SUBMIT_BLOCK] = 1;
				}
				if (expansion > 1)
				{
					vsg.level[OBJ_SEQSUB_CONTACT] = 1;
					vsg.level[OBJ_SEQSUB_CIT] = 1;
				}
				if (expansion > 2)
				{
					vsg.level[OBJ_SUBMIT_BLOCK] = 2;
					vsg.level[OBJ_SEQSUB_CONTACT] = 2;
					vsg.level[OBJ_SEQSUB_CIT] = 2;
				}
			case OBJ_BIOSEQ:
			case OBJ_BIOSEQSET:
				if (expansion)
				{
					vsg.level[OBJ_BIOSEQ] = 1;
					vsg.level[OBJ_BIOSEQSET] = 1;
				}
				if (vsmp != NULL && vsmp->extraLevel) {
					extraLevel = TRUE;
				}
				if (expansion > 1 && (! extraLevel)) {
					expansion++;
				}
				if (expansion > 1)
				{
					vsg.level[OBJ_BIOSEQ] = 2;
					vsg.level[OBJ_SEQDESC] = 1;
					vsg.level[OBJ_SEQANNOT] = 1;
				}
				if (expansion > 2)
				{
					vsg.level[OBJ_BIOSEQ] = 2;
					vsg.level[OBJ_SEQDESC] = 1;
					vsg.level[OBJ_SEQANNOT] = 1;
					vsg.level[OBJ_SEQFEAT] = 1;
					vsg.level[OBJ_SEQGRAPH] = 1;
					vsg.level[OBJ_SEQALIGN] = 1;
				}
				if (expansion > 3)
				{
					vsg.level[OBJ_BIOSEQ] = 3;
					vsg.level[OBJ_SEQDESC] = 2;
					vsg.level[OBJ_SEQANNOT] = 2;
					vsg.level[OBJ_SEQFEAT] = 2;
					vsg.level[OBJ_SEQGRAPH] = 2;
					vsg.level[OBJ_SEQALIGN] = 2;
					maxexpansion = TRUE;
				}
				if (expansion > 4)
					vsmpp->expansion = 4;
				break;
			case OBJ_SEQANNOT:
				if (expansion)
				{
					vsg.level[OBJ_SEQANNOT] = 1;
				}
				if (expansion > 1)
				{
					vsg.level[OBJ_SEQANNOT] = 1;
					vsg.level[OBJ_SEQFEAT] = 1;
					vsg.level[OBJ_SEQGRAPH] = 1;
					vsg.level[OBJ_SEQALIGN] = 1;
				}
				if (expansion > 2)
				{
					vsg.level[OBJ_SEQFEAT] = 2;
					vsg.level[OBJ_SEQGRAPH] = 2;
					vsg.level[OBJ_SEQALIGN] = 2;
					maxexpansion = TRUE;
				}
				if (expansion > 3)
					vsmpp->expansion = 3;
				break;
			case OBJ_SEQFEAT:
				if (expansion)
				{
					vsg.level[OBJ_SEQFEAT] = 2;
					maxexpansion = TRUE;
				}
				if (expansion > 1)
					vsmpp->expansion = 1;
				break;
			case OBJ_SEQALIGN:
				if (expansion)
				{
					vsg.level[OBJ_SEQALIGN] = 2;
					maxexpansion = TRUE;
				}
				if (expansion > 1)
					vsmpp->expansion = 1;
				break;
			case OBJ_SEQGRAPH:
				if (expansion)
				{
					vsg.level[OBJ_SEQGRAPH] = 2;
					maxexpansion = TRUE;
				}
				if (expansion > 1)
					vsmpp->expansion = 1;
				break;
			default:
				vsmpp->expansion = 0;
				maxexpansion = TRUE;
				break;
		}

	for (i = 0; i < OBJ_MAX; i++)
	{
		if (vsg.level[i])
			gs.ignore[i] = FALSE;
	}

	vsg.vsmp = vsmp;
	                                      /* default label */

	top = CreateSegment(NULL, omdp->EntityID, 0);
	omtp = ObjMgrTypeFind(vsmp->omp, omdp->datatype, NULL, NULL);
	
	seg = CreateSegment(top, omdp->datatype, 0);  /* 2nd level necessary if not gather */
	vsg.segs[0] = seg;
	if (omtp->label != NULL)
		tmp = omtp->label;
	else if (omtp->name != NULL)
		tmp = omtp->name;
	else
		tmp = omtp->asnname;
	width = LabelCopyExtra(buf, tmp, 40, NULL, ": ");
	tmp = buf + width;
	width = 40 - width;
	(*(omtp->labelfunc))(omdp->dataptr, tmp, width, OM_LABEL_TYPE);
	SelectFont(vsmp->font);
	width = StringWidth(buf) + (3 * vsmp->charw) + 20;
	maxwidth = width;

	AddAttribute(seg, COLOR_ATT, WHITE_COLOR, 0, 0, 0,0);
	AddSegRect(seg, TRUE, 0);

	if (omdp->tempload == TL_CACHED)
	{
		tcolor[0] = 0;
		tcolor[1] = 128;
		tcolor[2] = 128;
		AddAttribute(seg, COLOR_ATT, tcolor, 0, 0, 0,0);
	}
	else if (omdp->tempload == TL_LOADED)
	{
		tcolor[0] = 0;
		tcolor[1] = 128;
		tcolor[2] = 64;
		AddAttribute(seg, COLOR_ATT, tcolor, 0, 0, 0,0);
	}
	else if (omdp->clipboard)
		AddAttribute(seg, COLOR_ATT, RED_COLOR, 0, 0, 0,0);
	else
		AddAttribute(seg, COLOR_ATT, BLUE_COLOR, 0, 0, 0,0);
	AddSegRect(seg, FALSE, 0);

	bottom = (Int4)(vsmp->lineheight + (vsmp->lineheight/2));

	AddTextLabel(seg, vsmp->charw, bottom, buf, vsmp->font,0,UPPER_RIGHT,0);

	bottom -= vsmp->lineheight;

	if ((*(omtp->labelfunc))(omdp->dataptr, buf, 40, OM_LABEL_CONTENT))
	{
		SelectFont(vsmp->font);
		width2 = StringWidth(buf) + (2 * vsmp->charw);
		if (width2 > maxwidth)
			maxwidth = width2;
		AddTextLabel(seg, vsmp->charw,bottom,buf, vsmp->font,0,UPPER_RIGHT,0);

	}
	
	add_frame(seg, 0, (3 * vsmp->lineheight), maxwidth, 0, 0);
    
    bottom += vsmp->lineheight;
    maxwidth -= vsmp->charw;
	if (! maxexpansion)
	{
		AddAttribute(seg, COLOR_ATT, RED_COLOR, 0, 0, 0,0);
		AddRectangle(seg, (Int4)(maxwidth-20), (Int4)(bottom + vsmp->lineheight), (Int4)(maxwidth - 12),
						(Int4)(bottom+4), UP_ARROW, TRUE, (Uint2)VSM_PICT_UP_BUTTON);
						/**
		AddSymbol(seg, (Int4)(width-15), bottom, UP_TRIANGLE_SYMBOL, TRUE,
					UPPER_CENTER, (Uint2) VSM_PICT_UP_BUTTON);
					    **/
	}
	if (vsmpp->expansion)
	{
		AddAttribute(seg, COLOR_ATT, GREEN_COLOR, 0, 0, 0,0);
		AddRectangle(seg, (Int4)(maxwidth-8), (Int4)(bottom + vsmp->lineheight), (Int4)(maxwidth),
						(Int4)(bottom+4), DOWN_ARROW, TRUE, (Uint2)VSM_PICT_DOWN_BUTTON);
	}

	GatherEntity(omdp->EntityID, &vsg, VSMGatherPictProc, &gs);
	
	return top;
}

static Pointer LIBCALLBACK VSMFreePictureForEntity (Pointer ptr)
{
	VSMPictPtr vsmpp;

	if (ptr == NULL) return NULL;

	vsmpp = (VSMPictPtr)ptr;
	if (vsmpp->s != NULL)
	{
		UnlinkSegment(vsmpp->s);
		DeleteSegment(vsmpp->s);
	}
	return MemFree(vsmpp);
}

/*****************************************************************************
*
*   VSMAddPictureToEntity(vsmwp, entityID, expand)
*   	if segment with picture already present, just refreshes it
*   	else adds the new one
*		expand increments or decrements the expansion of the picture by its value
*
*****************************************************************************/
static OMUserDataPtr NEAR VSMAddPictureToEntity (VSMWinPtr vsmwp, Uint2 entityID, Int2 expand)
{
	OMUserDataPtr omudp;
	VSMPictPtr vsmpp;
	ObjMgrDataPtr omdp;
	SegmenT seg=NULL;
	ValNodePtr vnp;
	Pointer ptr;
	SelStructPtr ssp;
	VSeqMgrPtr vsmp;

	if (vsmwp == NULL) return NULL;
											  /* already have it? */
	omudp = ObjMgrGetUserData (entityID, vsmwp->procid, vsmwp->proctype, vsmwp->userkey);
	if (omudp != NULL)   /* have it already */
	{
		vsmpp = (VSMPictPtr)(omudp->userdata.ptrvalue);
		UnlinkSegment(vsmpp->s);
		DeleteSegment(vsmpp->s);
		vsmpp->s = NULL;
	}
	else
	{
		omudp = ObjMgrAddUserData(entityID, vsmwp->procid, vsmwp->proctype,
					vsmwp->userkey);
		vsmpp = MemNew(sizeof(VSMPict));
		vsmpp->vsmwin = vsmwp;
		vsmpp->entityID = entityID;
		omudp->userdata.ptrvalue = (Pointer)vsmpp;
		omudp->freefunc = VSMFreePictureForEntity;
		omudp->messagefunc = VSMPictMsgFunc;
	}

	omdp = ObjMgrGetData(entityID);
	vnp = omdp->choice;
	ptr = omdp->dataptr;
	
	vsmpp->expansion += expand;

	vsmp = vsmwp->vsmp;
	if (vsmp != NULL) {
		vsmp->extraLevel = TRUE;
	}

	seg = VSMEntityDraw(omdp, vsmpp, vsmwp->vsmp);

	if (seg == NULL)
		Message(MSG_ERROR, "VSMAddSegmentToPicture: can't handle [%d] [%d]",
			(int)(omdp->choicetype), (int)(omdp->datatype));
	else
	{
		vsmpp->s = seg;
		ssp = ObjMgrGetSelected();
		while (ssp != NULL)
		{
			if (ssp->entityID == entityID)
			{
				VSMPictSelect(vsmpp, entityID, ssp->itemID, ssp->itemtype, TRUE);
			}
			ssp = ssp->next;
		}
	}		

	return omudp;
}
