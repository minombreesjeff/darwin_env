/*   odlbox.c
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
* File Name:  odlbox.c
*
* Author:  Patrick Durand
*
* Version Creation Date:   5/3/99
*
* $Revision: 6.3 $
*
* File Description: 
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: odlbox.c,v $
* Revision 6.3  1999/06/07 15:41:31  durand
* add LOG line to keep track of the history
*
*
*
* ==========================================================================
*/

#include <ncbi.h>
#include <vibrant.h>

#include <odlbox.h>

typedef  struct  rect4 {/*a new rect type for very big listbox*/
	Int4  left;
	Int4  top;
	Int4  right;
	Int4  bottom;
	} RecT4, PNTR Rect4Ptr;

typedef struct lbvscroll {/*vertical Scroll Bar data*/
	Int4 ScrollMax;		/*Size of the vertical Scroll Bar*/
						/*also equal number of data lines displayed within
						the listbox*/
	Int4 ScrollPos;		/*Current position within the vertical Scroll Bar*/
	Int4 ScrollPage;	/*Page decal of the vertical Scroll Bar*/
	Int4 nTotLines;		/*total number of line*/
	Int2 cxClient;		/*size of the client lbox window*/
	Int2 cyClient;
	RecT rcP;
	} LBVScroll, PNTR LBVScrollPtr;

typedef struct lbentrydata {/*data attached to each list box entry*/
	Pointer		pUserData;	/*a 'stuff' provided by the user*/
	RecT4		rc;			/*the position of the entry*/
	Boolean 	Selected;	/*TRUE: item is selected*/
	} LBEntryData, PNTR LBEntryDataPtr;

typedef struct mainlbdata {	/*list box main data*/
	WindoW 				Parent;	/*parent of the list box*/
	Int4				nEntry;	/*number of entry in this list box*/
	Int4				CurEntry;/*current selected entry (zero-based)*/
	LBEntryDataPtr		CurEntryData;
	ValNodePtr			LBData;	/*private list box data ; 
								LBData->data.ptrvalue is of LBEntryDataPtr type*/
	LBVScroll			vscroll;/*Vertical scrollbar data*/
	OwnerDrawLboxFunc	MsgFunc;/*Message Loop provided by the user*/
	FonT 				f;		/*user Font*/
	Int2				cxChar;
	Int2				cyChar;
		/*internal use only*/
	ValNodePtr			LBDataTmp;
	Int4				decal;
	} MainLBData, PNTR MainLBDataPtr;

/*******************************************************************************

  Function : LBCleanupDataProc()
  
  Purpose : List Box personnal CleanupExtraProc Callback 
  
  Parameters : g; handle of the dialog box
  				data; to erase
  
  Return value : none 

*******************************************************************************/
static void LBCleanupDataProc (GraphiC g, VoidPtr data)
{
MainLBDataPtr 	lbdata;
ValNodePtr		vnp;

	lbdata=(MainLBDataPtr)data;
	if (lbdata==NULL) return;
	
	/*delete list of user data*/
	if (lbdata->nEntry>0){
		for(vnp=lbdata->LBData ; vnp!=NULL ; vnp=vnp->next){
			if (vnp->data.ptrvalue) MemFree(vnp->data.ptrvalue);
		}
		ValNodeFree(lbdata->LBData);		
	}

	MemFree (lbdata);
}

/*******************************************************************************

  Function : LB_calc_RCdraw()
  
  Purpose : compute the RC of an entry to be drawn  
  
  Parameters :	rc_entry; absolute position of an entry
  				decal; y decal to place rc_entry within the listbox panel 
  
  Return value : position of the entry within the listbox panel

*******************************************************************************/
static RecT LB_calc_RCdraw(RecT4 rc_entry,Int4 decal)
{
RecT rc;

	rc.top=(Int2)(rc_entry.top-decal);
	rc.bottom=(Int2)(rc_entry.bottom-decal);
	rc.left=(Int2)rc_entry.left;
	rc.right=(Int2)rc_entry.right;

	return(rc);
}

/*****************************************************************************

Function: LB_ChangSel()

Purpose: manage ChangSelection

Parameters:	p; panel handle (currently unused)
			lbdata; list box data block
			CurEntry,CurEntryData; new item selected

Return value: none

*****************************************************************************/
static void LB_ChangSel(WindoW p,MainLBDataPtr lbdata,Int4 CurEntry,
		LBEntryDataPtr CurEntryData,Boolean SendSelectMessage)
{
MIData	mid;
Int4	decal=lbdata->vscroll.ScrollPos*stdLineHeight;
WindoW	temport;

	temport=SavePort(p);
	Select(p);
	mid.rcP=lbdata->vscroll.rcP;
	mid.f=lbdata->f;
	mid.cxChar=lbdata->cxChar;
	mid.cyChar=lbdata->cyChar;
	mid.Parent=lbdata->Parent;
	InsetRect(&mid.rcP,2,4);
	ClipRect(&mid.rcP);
	/*deselect previous entry, if needed*/
	if (lbdata->CurEntry!=(Int4)-1){
		/*mid.rcP=lbdata->vscroll.rcP;*/
		mid.Data=lbdata->CurEntryData->pUserData;
		mid.rcItem=LB_calc_RCdraw(lbdata->CurEntryData->rc,decal);
		mid.Selected=FALSE;
		lbdata->CurEntryData->Selected=FALSE;
		/*send message to draw an entry*/			
		(*lbdata->MsgFunc)((WindoW)p,ODLB_DrawItem,(Pointer)&mid);
	}
	/*select the new entry*/
	lbdata->CurEntry=CurEntry;
	lbdata->CurEntryData=CurEntryData;
	/*mid.rcP=lbdata->vscroll.rcP;*/
	mid.Data=CurEntryData->pUserData;
	mid.rcItem=LB_calc_RCdraw(CurEntryData->rc,decal);
	mid.Selected=TRUE;
	CurEntryData->Selected=TRUE;
	/*send message to draw an entry; then send a SelectItem msg*/			
	(*lbdata->MsgFunc)((WindoW)p,ODLB_DrawItem,(Pointer)&mid);
	if (SendSelectMessage){
		(*lbdata->MsgFunc)((WindoW)p,ODLB_SelectItem,(Pointer)&mid);
	}
	ResetClip();
	RestorePort(temport);
}

/*****************************************************************************

Function: LB_OnClick()

Purpose: manage Mouse-Click

Parameters:	p; list-box handle 
			pt; new mouse position

Return value: none

*****************************************************************************/
static void LB_OnClick(PaneL p, PoinT pt)
{
Int4 			limit,decal=0,i=1;
ValNodePtr 		vnp;
LBEntryDataPtr 	lbdp;
RecT 			rc;
MainLBDataPtr	lbdata;
Boolean 		bFind=FALSE;
	
	/*get some usefull data...*/
	lbdata = (MainLBDataPtr) GetObjectExtra (p);
	if (lbdata==NULL) return;
	
	decal=lbdata->vscroll.ScrollPos*stdLineHeight;
	limit=(Int4)lbdata->vscroll.rcP.top+decal;
	/*find the first entry located at the top of the listbox*/
	for(vnp=lbdata->LBData ; vnp != NULL ; vnp=vnp->next){
		if (vnp->data.ptrvalue){
			lbdp=(LBEntryDataPtr)vnp->data.ptrvalue;
			if (lbdp->rc.bottom>limit){
				rc=LB_calc_RCdraw(lbdp->rc,decal);
				if (PtInRect(pt,&rc)){
					bFind=TRUE;
					break;
				}
			}
			i++;		
		}
	}	
	
	/*if item found in the listbox*/	
	if (bFind){
		LB_ChangSel((WindoW)p, lbdata,i,lbdp,TRUE);
	}
}

/*****************************************************************************

Function: UDV_ReleaseProc()

Purpose: manage Mouse-Release

Parameters:	p; panel handle (currently unused)
			pt; new position

Return value: none

*****************************************************************************/
static void LB_OnRelease(PaneL p, PoinT pt)
{
}

/*******************************************************************************

  Function : LB_DrawEmpty()
  
  Purpose : draw a emplty window
  
  Parameters :	rectangle to fill 
  
  Return value : none

*******************************************************************************/
static void  LB_DrawEmpty(RecT rc)
{
	White();
	PaintRect(&rc);
	Black();
}

/*******************************************************************************

  Function : LB_calc_decalRC()
  
  Purpose : compute the y decal for LB entry RecT correct positionning on
  		the listbox panel
  
  Parameters :	lbdata; list of entries
  				ScrollPos; actual position of the Vscroll bar
				vnp; first entry to draw in the panel
				rcP; rc de la listbox				
				
  Return value : y decal value (pixel unit)

*******************************************************************************/
static Int4  LB_calc_decalRC(ValNodePtr lbdata,Int4 ScrollPos,
		ValNodePtr PNTR vnp,RecT rcP)
{
Int4 			decal,limit;
LBEntryDataPtr 	lbdp;
		
	decal=ScrollPos*stdLineHeight;
	limit=(Int4)rcP.top+decal;
	/*find the first entry located at the top of the listbox*/
	for(*vnp=lbdata ; *vnp != NULL ; *vnp=(*vnp)->next){
		if ((*vnp)->data.ptrvalue){
			lbdp=(LBEntryDataPtr)(*vnp)->data.ptrvalue;
			if (lbdp->rc.bottom>limit){
					break;
			}
		}		
	}	

	return(decal);
}


/*******************************************************************************

  Function : LB_onDraw()
  
  Purpose :  draw the panel of the listbox ; if fact, call the User OnDraw
  		function
  
  Parameters :	handle of the panel
  
  Return value : none

*******************************************************************************/
static void LB_onDraw(PaneL p)
{
Int4 			decal=0;
ValNodePtr 		vnp,vnp2;
LBEntryDataPtr 	lbdp;
RecT 			rc,rcP;
MainLBDataPtr	lbdata;
MIData			mid;
WindoW temport;
	
	/*get some usefull data...*/
	lbdata = (MainLBDataPtr) GetObjectExtra (p);
	if (lbdata==NULL) return;

	temport=SavePort(ParentWindow(p));

	Select (p);
	
	/*restrict panel drawing area: 'add' little margins*/
	ObjectRect(p,&rcP);
	InsetRect(&rcP,2,4);
	ClipRect(&rcP);


	if (lbdata->LBData==NULL) {
		LB_DrawEmpty(rcP);
		ResetClip();	
		return;
	}


	decal=LB_calc_decalRC(lbdata->LBData,lbdata->vscroll.ScrollPos,&vnp,rcP);
	
	if (vnp==NULL){
		LB_DrawEmpty(rcP);
		ResetClip();	
		return;
	}

	mid.rcP=lbdata->vscroll.rcP;
	mid.f=lbdata->f;
	mid.cxChar=lbdata->cxChar;
	mid.cyChar=lbdata->cyChar;
	mid.Parent=lbdata->Parent;

	for(vnp2=vnp ; vnp2 != NULL ; vnp2=vnp2->next){
		if (vnp2->data.ptrvalue){
			lbdp=(LBEntryDataPtr)vnp2->data.ptrvalue;
			
			rc=LB_calc_RCdraw(lbdp->rc,decal);
			
			/*send message to draw an entry*/			
			mid.rcItem=rc;
			mid.Data=lbdp->pUserData;
			mid.Selected=lbdp->Selected;
			(*lbdata->MsgFunc)((WindoW)p,ODLB_DrawItem,(Pointer)&mid);
			/*end ?*/
			if (rc.bottom >= rcP.bottom) break;
		}		
	}	

	ResetClip();	
	RestorePort(temport);
}

/*******************************************************************************

  Function : LB_onVScroll()
  
  Purpose : Owner Draw list box Vertical Scroll Bar Callback 
  
  Parameters : sb; handle of the scroll
  				s; pane;
				newval; new value of the thumb
				oldval; old value of the thumb
  
  Return value : none 

*******************************************************************************/
static void LB_onVScroll (BaR sb, SlatE s, Int4 newval, 
			Int4 oldval)
{
RecT 				rcP;
WindoW 				temport;
Int4 				n;
Boolean 			IsGoUp=FALSE;
MainLBDataPtr		lbdata;

	/*get some usefull data...*/
	lbdata = (MainLBDataPtr) GetObjectExtra (s);
	if (lbdata==NULL) return;
	
	lbdata->vscroll.ScrollPos=newval;
	
	temport=SavePort((WindoW)s);
	Select(s);
	ObjectRect((PaneL)s,&rcP);
	InsetRect(&rcP,2,4);
	ClipRect(&rcP);

	if (oldval>newval){
		n=oldval-newval;
		IsGoUp=TRUE;
	}
	else {
		n=newval-oldval;
		IsGoUp=FALSE;
	}
	
	if (n<lbdata->vscroll.ScrollPage){/*Line UP/Down*/
		/*move and redraw*/
		ScrollRect (&rcP, 0, (Int2)((oldval-newval)*stdLineHeight));
	}
	else{/*Page Up/Down*/
		/*redraw*/
		/*rcP.left=0;
		rcP.top=0;*/
		InvalRect(&rcP);
	}
	ResetClip();
	RestorePort(temport);
	/*Update ();*/
}

/*******************************************************************************

  Function : OwnerDrawLbox_VScrlUpdatePage()
  
  Purpose : compute PageUp & Down (VScroll) 
  
  Parameters : PageUpDn; initialize here
  				cyClient; height of UnDviewer panel
  
  Return value : none 

*******************************************************************************/
static void  OwnerDrawLbox_VScrlUpdatePage(Int4Ptr PageUpDn,
				Int2 cyClient)
{
	*PageUpDn=(Int4)(cyClient/stdLineHeight);
}

/*******************************************************************************

  Function : OwnerDrawLbox_VScrlUpdate()
  
  Purpose : update the UnDviewer VScroll bar 
  
  Parameters : p; handel of the UnDviewer panel
  				bInit; TRUE only when the UnDviewer is created (soft. start up)
  				CurPos ; new position
				
  Return value : none 

*******************************************************************************/
static void  OwnerDrawLbox_VScrlUpdate(PaneL p,
		Boolean bInit,Int4 CurPos)
{
MainLBDataPtr	lbdata;
BaR 			vsb;

	if (p==NULL) return;

	/*get some usefull data...*/
	lbdata = (MainLBDataPtr) GetObjectExtra (p);
	if (lbdata==NULL) return;
	
	/*current scroll status*/
	vsb = GetSlateVScrollBar ((SlatE) p);
	
	if (bInit) CurPos=0;
	
	/*given curpos (lineNumber) retrieve letter number*/
	/*compute new values*/	
	OwnerDrawLbox_VScrlUpdatePage(&(lbdata->vscroll.ScrollPage),
				lbdata->vscroll.cyClient);
				
	if (lbdata->vscroll.nTotLines>lbdata->vscroll.ScrollPage){
		lbdata->vscroll.ScrollMax=lbdata->vscroll.nTotLines-
							lbdata->vscroll.ScrollPage;
	}
	else{
		lbdata->vscroll.ScrollMax=0;
		lbdata->vscroll.ScrollPos=0;
		lbdata->vscroll.ScrollPage=0;
	}

	if (CurPos<0) CurPos=0;

	if (CurPos>=lbdata->vscroll.ScrollMax)
		lbdata->vscroll.ScrollPos=lbdata->vscroll.ScrollMax;
	else lbdata->vscroll.ScrollPos=CurPos;
				
	/*update scroll*/
	CorrectBarMax(vsb,lbdata->vscroll.ScrollMax);
	CorrectBarValue(vsb,lbdata->vscroll.ScrollPos);
	CorrectBarPage(vsb,lbdata->vscroll.ScrollPage,
			lbdata->vscroll.ScrollPage);	
}


/*******************************************************************************

  Function : OwnerDrawLbox_ResetLBContent()
  
  Purpose : delete actual data of the list box
  
  Parameters : 	lbox; handle of the list box
  
  Return value : none 

*******************************************************************************/
NLM_EXTERN void  OwnerDrawLbox_ResetLBContent(WindoW lbox)
{
MainLBDataPtr 	lbdata;
ValNodePtr		vnp;
WindoW			temport;
BaR				vsb;

	lbdata=(MainLBDataPtr)GetObjectExtra (lbox);
	if (lbdata==NULL) return;
	
	/*delete list of user data*/
	if (lbdata->nEntry>0){
		for(vnp=lbdata->LBData ; vnp!=NULL ; vnp=vnp->next){
			if (vnp->data.ptrvalue) MemFree(vnp->data.ptrvalue);
		}
		ValNodeFree(lbdata->LBData);
		lbdata->LBData=NULL;		
	}
	/*init lbox internal data*/
	lbdata->nEntry=0;
	lbdata->CurEntry=-1;
	lbdata->CurEntryData=NULL;
	lbdata->vscroll.ScrollMax=0;
	lbdata->vscroll.ScrollPos=0;
	lbdata->vscroll.ScrollPage=0;
	lbdata->vscroll.nTotLines=0;
	lbdata->LBDataTmp=NULL;
	lbdata->decal=0;
	
	/*reinit scrollbar and redraw an empty listbox*/
	vsb = GetSlateVScrollBar ((SlatE)lbox);
	CorrectBarMax(vsb,0);
	CorrectBarValue(vsb,0);
	CorrectBarPage(vsb,0,0);
	temport=SavePort(lbox);
	Select(lbox);
	InvalRect(&lbdata->vscroll.rcP);
	RestorePort(temport);
}

/*******************************************************************************

  Function : OwnerDrawLbox_GetCurData()
  
  Purpose : retrieve the current data of the selected item in an owner 
  			draw listbox
  
  Parameters : 	lbox; handle of the list box
  
  Return value : current item data 

*******************************************************************************/
NLM_EXTERN Pointer  OwnerDrawLbox_GetCurData(WindoW lbox)
{
MainLBDataPtr	lbdata;
	
	/*retrieve lbox data*/
	lbdata = (MainLBDataPtr) GetObjectExtra (lbox);
	if (lbdata == NULL) return(NULL);/*oups*/

	return(lbdata->CurEntryData->pUserData);	
}

/*******************************************************************************

  Function : OwnerDrawLbox_GetData()
  
  Purpose : retrieve the current data of the selected item in an owner 
  			draw listbox
  
  Parameters : 	lbox; handle of the list box
  				item; data to retrieve (one based value)
				
  Return value : current item data 

*******************************************************************************/
NLM_EXTERN Pointer  OwnerDrawLbox_GetData(WindoW lbox,Int4 item)
{
MainLBDataPtr	lbdata;
ValNodePtr		vnp;
Int4			i=1;
Boolean bFound=FALSE;
	
	/*retrieve lbox data*/
	lbdata = (MainLBDataPtr) GetObjectExtra (lbox);
	if (lbdata == NULL) return(NULL);/*oups*/

	/*scan list of user data*/
	if (lbdata->nEntry>0){
		for(vnp=lbdata->LBData ; vnp!=NULL ; vnp=vnp->next){
			if (i==item) {
				bFound=TRUE;
				break;
			}
			i++;
		}
	}
	
	if (!bFound) return(NULL);
	
	return((Pointer)vnp->data.ptrvalue);	
}

/*******************************************************************************

  Function : OwnerDrawLbox_GetNentry()
  
  Purpose : retrieve the number of entries of aa owner draw listbox
  
  Parameters : 	lbox; handle of the list box
  
  Return value : current item data 

*******************************************************************************/
NLM_EXTERN Int4  OwnerDrawLbox_GetNentry(WindoW lbox)
{
MainLBDataPtr	lbdata;
	
	/*retrieve lbox data*/
	lbdata = (MainLBDataPtr) GetObjectExtra (lbox);
	if (lbdata == NULL) return(0);/*oups*/

	return(lbdata->nEntry);	
}

/*******************************************************************************

  Function : OwnerDrawLbox_GetCurSel()
  
  Purpose : retrieve the current selected item in an owner draw listbox
  
  Parameters : 	lbox; handle of the list box
  
  Return value : current item selected in the list box (Zero-based value)

*******************************************************************************/
NLM_EXTERN Int4  OwnerDrawLbox_GetCurSel(WindoW lbox)
{
MainLBDataPtr	lbdata;
	
	/*retrieve lbox data*/
	lbdata = (MainLBDataPtr) GetObjectExtra (lbox);
	if (lbdata == NULL) return((Int4)-1);/*oups*/

	return(lbdata->CurEntry);	
}

/*******************************************************************************

  Function : OwnerDrawLbox_SelectItem()
  
  Purpose : select an item in an owner draw listbox
  
  Parameters : 	lbox; handle of the list box
  				item; item to select (One-based value)
  
  Return value : none

*******************************************************************************/
NLM_EXTERN void  OwnerDrawLbox_SelectItem(WindoW lbox,Int4 item)
{
MainLBDataPtr	lbdata;
ValNodePtr		vnp;
Int4			i=1,decal,NewPos;
Boolean			bFound=FALSE;
LBEntryDataPtr  lbdp;
RecT			rc;

	/*retrieve lbox data*/
	lbdata = (MainLBDataPtr) GetObjectExtra (lbox);
	if (lbdata == NULL) return;/*oups*/

	/*scan list of user data*/
	if (lbdata->nEntry>0){
		for(vnp=lbdata->LBData ; vnp!=NULL ; vnp=vnp->next){
			if (i==item) {
				bFound=TRUE;
				break;
			}
			i++;
		}
	}

	if (!bFound) return;
	lbdp=(LBEntryDataPtr)vnp->data.ptrvalue;
	/*scroll*/
	decal=lbdata->vscroll.ScrollPos*stdLineHeight;
	rc=LB_calc_RCdraw(lbdp->rc,decal);
	if (!RectInRect(&rc,&lbdata->vscroll.rcP)){
		WindoW temport;
		NewPos=(lbdp->rc.top-lbdata->vscroll.rcP.top)/stdLineHeight;
		OwnerDrawLbox_VScrlUpdate((PaneL)lbox,FALSE,NewPos);
		temport=SavePort(lbox);
		Select(lbox);
		InvalRect(&lbdata->vscroll.rcP);
		RestorePort(temport);
	}
	LB_ChangSel(lbox,lbdata,item,lbdp,FALSE);
}

/*******************************************************************************

  Function : OwnerDrawLbox_AddItem()
  
  Purpose : add a user item to an owner draw listbox
  
  Parameters : 	lbox; handle of the list box
  				item; data to add to the list
  
  Return value : TRUE if success, FALSE if any errors

*******************************************************************************/
NLM_EXTERN Boolean  OwnerDrawLbox_AddItem(WindoW lbox,Pointer item)
{
MainLBDataPtr	lbdata;
LBEntryDataPtr	lbdp;
MIData			mid;

	/*retrieve lbox data*/
	lbdata = (MainLBDataPtr) GetObjectExtra (lbox);
	if (lbdata == NULL) return(FALSE);/*oups*/

	/*ask the user for the item size*/
	MemSet(&mid.rcItem,0,sizeof(RecT));
	mid.Data=item;
	mid.rcP=lbdata->vscroll.rcP;
	mid.Selected=FALSE;
	mid.f=lbdata->f;
	mid.cxChar=lbdata->cxChar;
	mid.cyChar=lbdata->cyChar;
	mid.Parent=lbdata->Parent;
	if (!(*lbdata->MsgFunc)(lbox,ODLB_MeasureItem,(Pointer)&mid)) {
		/*if message isn't managed by the user, compute a default size*/
		mid.rcItem.left=lbdata->vscroll.rcP.left;
		mid.rcItem.top=0;
		mid.rcItem.right=lbdata->vscroll.rcP.right;
		mid.rcItem.bottom=stdLineHeight;
	}

	/*create new data block for the user data*/
	lbdp=(LBEntryDataPtr)MemNew(sizeof(LBEntryData));
	if (lbdp==NULL) return(FALSE);/*oups II, the return*/
	MemSet(lbdp,0,sizeof(LBEntryData));
	lbdp->pUserData=item;
		
	/*add the item to the ValNodeList*/
	if (lbdata->nEntry==0){
		lbdata->LBData=ValNodeAddPointer(NULL,0,(VoidPtr)lbdp);
		if (lbdata->LBData==NULL) {
			MemFree(lbdp);
			return(FALSE);/*oups III, the ultimate adventure*/
		}
		lbdata->LBDataTmp=lbdata->LBData;
		/*position of the new element*/
		lbdp->rc.left=(Int4)mid.rcItem.left;
		lbdp->rc.right=(Int4)mid.rcItem.right;
		lbdp->rc.top=(Int4)(mid.rcItem.top+lbdata->vscroll.rcP.top);
		lbdp->rc.bottom=(Int4)(lbdp->rc.top+mid.rcItem.bottom);
	}
	else{
		lbdata->LBDataTmp=ValNodeAddPointer(&lbdata->LBDataTmp,0,(VoidPtr)lbdp);
		if (!lbdata->LBDataTmp){
			MemFree(lbdp);
			return(FALSE); /*oups IV, the resurrection*/
		}
		/*compute correct pos for the new item*/
		lbdp->rc.left=(Int4)mid.rcItem.left;
		lbdp->rc.right=(Int4)mid.rcItem.right;
		lbdp->rc.top=(Int4)(mid.rcItem.top+lbdata->decal);
		lbdp->rc.bottom=(Int4)(lbdp->rc.top+mid.rcItem.bottom);
	}
	/*decal in the list box*/	
	lbdata->decal=lbdp->rc.bottom+1;

	/*ok*/
	lbdata->nEntry++;

	/*update the scroll, but do not move it; stay on pos 0*/
	lbdata->vscroll.nTotLines=lbdata->decal/stdLineHeight+1;
	OwnerDrawLbox_VScrlUpdate((PaneL)lbox,TRUE,0);	

	/*ask the user to draw, only if needed*/
/*	if (lbdp->rc.bottom<=(Int4)lbdata->vscroll.rcP.bottom){
		mid.rcItem.top=(Int2)lbdp->rc.top;
		mid.rcItem.bottom=(Int2)lbdp->rc.bottom;
		mid.Data=item;
		mid.rcP=lbdata->vscroll.rcP;
		mid.f=lbdata->f;
		mid.cxChar=lbdata->cxChar;
		mid.cyChar=lbdata->cyChar;
		(*lbdata->MsgFunc)(lbox,ODLB_DrawItem,(Pointer)&mid);
	}*/
	
	return(TRUE);
}

/*******************************************************************************

  Function : OwnerDrawLbox_MoveWindow()
  
  Purpose : move/resize an owner draw listbox
  
  Parameters : 	lbox ; do you think it's what ?
  				x ; pos on x
				y ; pos on y
				cx ; width of the ListBox
				cy ; height of the ListBox
  
  Return value : -

*******************************************************************************/
NLM_EXTERN void  OwnerDrawLbox_MoveWindow(WindoW lbox,
	Int2 x,Int2 y, Int2 cx,Int2 cy)
{
MainLBDataPtr	lbdata;
LBEntryDataPtr	lbdp;
MIData			mid;
ValNodePtr		vnp;
RecT			rcP;

Int4			oldDecal,oldCurPos;
Boolean			bFirst=TRUE,bFirst2=FALSE;

	/*retrieve lbox data*/
	lbdata = (MainLBDataPtr) GetObjectExtra (lbox);
	if (lbdata == NULL) return;

	/*compute new listbox size*/
	Select(lbox);
	rcP.left=x;
	rcP.top=y;
	rcP.right=x+cx;
	rcP.bottom=y+cy;
	lbdata->vscroll.rcP=rcP;
	lbdata->vscroll.rcP.right-=3*lbdata->cxChar;
	lbdata->vscroll.rcP.top+=1;
	lbdata->vscroll.cxClient=cx;
	lbdata->vscroll.cyClient=cy;
	mid.rcP=lbdata->vscroll.rcP;
	mid.f=lbdata->f;
	mid.cxChar=lbdata->cxChar;
	mid.cyChar=lbdata->cyChar;
	mid.Parent=lbdata->Parent;
	oldDecal=lbdata->vscroll.ScrollPos*stdLineHeight;
	lbdata->decal=lbdata->vscroll.rcP.top;
	/*resize all the items*/
	for(vnp=lbdata->LBData ; vnp!=NULL ; vnp=vnp->next){
		lbdp=(LBEntryDataPtr)vnp->data.ptrvalue;
		if (lbdp){
			if (bFirst){/*retrieve the first displayed item of the list*/
				if ((lbdp->rc.top-lbdata->vscroll.rcP.top)>=oldDecal){
					bFirst2=TRUE;
					bFirst=FALSE;
				}
			}
			mid.Data=lbdp->pUserData;
			if (!(*lbdata->MsgFunc)(lbox,ODLB_MeasureItem,(Pointer)&mid)) {
				/*if message isn't managed by the user, compute a default size*/
				mid.rcItem.left=lbdata->vscroll.rcP.left;
				mid.rcItem.top=0;
				mid.rcItem.right=lbdata->vscroll.rcP.right;
				mid.rcItem.bottom=stdLineHeight;
			}
			/*compute correct pos for the new item*/
			lbdp->rc.left=(Int4)mid.rcItem.left;
			lbdp->rc.right=(Int4)mid.rcItem.right;
			lbdp->rc.top=(Int4)(mid.rcItem.top+lbdata->decal);
			lbdp->rc.bottom=(Int4)(lbdp->rc.top+mid.rcItem.bottom);
			if (bFirst2){/*compute oldCurPos*/
				oldCurPos=(lbdp->rc.top-lbdata->vscroll.rcP.top)/stdLineHeight;
				bFirst2=FALSE;
			}
			/*decal in the list box*/	
			lbdata->decal=lbdp->rc.bottom+1;
		}
	}

	/*update the scroll*/
	lbdata->vscroll.nTotLines=lbdata->decal/stdLineHeight+1;
	OwnerDrawLbox_VScrlUpdate((PaneL)lbox,FALSE,oldCurPos);	

	/*move/resize and redraw window*/
	SetPosition(lbox, &rcP);
	AdjustPrnt(lbox, &rcP, FALSE);
}

/*******************************************************************************

  Function : OwnerDrawLbox_Create()
  
  Purpose : Create an owner draw listbox
  
  Parameters : 	Parent ; handle of the parent window for the listbox
  				x ; pos on x
				y ; pos on y
				cx ; width of the ListBox
				cy ; height of the ListBox
				MsgFunc ; Message loop of the ListBox
  
  Return value : WindoW if success, NULL if any errors

*******************************************************************************/
NLM_EXTERN WindoW  OwnerDrawLbox_Create(WindoW Parent,
	Int2 x,Int2 y, Int2 cx,Int2 cy, OwnerDrawLboxFunc MsgFunc,FonT f,
	Int2 cxChar,Int2 cyChar)
{
MainLBDataPtr	lbdata;
PaneL			p;
RecT			rcP;

	/*create the listbox as an AutonomousPanel4*/
	p=AutonomousPanel4(Parent,10,10,LB_onDraw,
			LB_onVScroll,NULL,0,NULL,NULL);
	/*create mouse action for the listbox*/
	SetPanelClick(p,LB_OnClick,NULL,NULL,LB_OnRelease);

	if (p==NULL) return(NULL);/*isn't it !*/

	/*use x,y,cx,cy provided by the user*/
	Select(p);
	rcP.left=x;
	rcP.top=y;
	rcP.right=x+cx;
	rcP.bottom=y+cy;
	SetPosition(p, &rcP);
	AdjustPrnt(p, &rcP, FALSE);
	
	/*Allocate the data structure and attach it to the list box*/
	lbdata=(MainLBDataPtr)MemNew(sizeof(MainLBData));	
	if (lbdata==NULL) {
		Remove(p);
		return(NULL);
	}
	MemSet(lbdata,0,sizeof(MainLBData));
	lbdata->CurEntry=-1;
	lbdata->Parent=Parent;
	lbdata->MsgFunc=(OwnerDrawLboxFunc)MsgFunc;
	lbdata->vscroll.cxClient=cx;
	lbdata->vscroll.cyClient=cy;
	rcP.top+=1;
	rcP.right-=3*cxChar;
	lbdata->vscroll.rcP=rcP;
	lbdata->f=f;
	lbdata->cxChar=cxChar;
	lbdata->cyChar=cyChar;
	SetObjectExtra (p, (Pointer) lbdata, LBCleanupDataProc);
	
	/*yes !*/
	return((WindoW)p);
}

