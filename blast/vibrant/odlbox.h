/*   odlbox.h
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
* File Name:  odlbox.h
*
* Author:  Patrick Durand
*
* Version Creation Date:   5/3/99
*
* $Revision: 6.2 $
*
* File Description: 
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: odlbox.h,v $
* Revision 6.2  1999/06/07 15:41:31  durand
* add LOG line to keep track of the history
*
*
*
* ==========================================================================
*/

#ifndef _ODLBOX_
#define _ODLBOX_

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************

	DEFINES

*******************************************************************************/
/*messages*/
#define ODLB_DrawItem		1
#define ODLB_MeasureItem	2
#define ODLB_SelectItem		3

/*******************************************************************************

	callback for the user

*******************************************************************************/
	/*WindoW: lbox handle ; Int2: message ; Pointer : data*/
	/*Msg managed by the user, he must return TRUE, otherwise FALSE*/
typedef Boolean  (*OwnerDrawLboxFunc) (WindoW,Int2,Pointer);

/*******************************************************************************

	STRUCTURES

*******************************************************************************/
/*struture passed through the callback*/
typedef  struct  midata {/*MeasureItem data struct*/
	WindoW	Parent;		/*Parent of the listbox*/
	RecT 	rcP;		/*size of the listbox*/
	RecT 	rcItem;		/*size of the item*/
	Pointer Data;		/*user data*/
	Boolean Selected;	/*TRUE: item is selected*/
	FonT	f;			/*user font*/
	Int2	cxChar;
	Int2	cyChar;
	} MIData, PNTR MIDataPtr;

/*******************************************************************************

	FUNCTIONS DECLARATION
	(see .c module for a complete description)

*******************************************************************************/
NLM_EXTERN WindoW OwnerDrawLbox_Create(WindoW Parent,
	Int2 x,Int2 y, Int2 cx,Int2 cy, OwnerDrawLboxFunc MsgFunc,FonT f,
	Int2 cxChar,Int2 cyChar);
NLM_EXTERN void OwnerDrawLbox_MoveWindow(WindoW lbox,
	Int2 x,Int2 y, Int2 cx,Int2 cy);
NLM_EXTERN Boolean OwnerDrawLbox_AddItem(WindoW lbox,Pointer item);
NLM_EXTERN void OwnerDrawLbox_SelectItem(WindoW lbox,Int4 item);
NLM_EXTERN Int4 OwnerDrawLbox_GetCurSel(WindoW lbox);
NLM_EXTERN Int4 OwnerDrawLbox_GetNentry(WindoW lbox);
NLM_EXTERN Pointer OwnerDrawLbox_GetCurData(WindoW lbox);
NLM_EXTERN Pointer OwnerDrawLbox_GetData(WindoW lbox,Int4 item);
NLM_EXTERN void OwnerDrawLbox_ResetLBContent(WindoW lbox);


#ifdef __cplusplus
}
#endif

#endif /* ndef _ODLBOX_ */

