/*   dvncode.h
* ===========================================================================
*
*                            PUBLIC DOMAIN NOTICE                          
*               National Center for Biotechnology Information
*                                                                          
*  This software/database is a "United States Government Work" under the
*  terms of the United States Copyright Act.  It was written as part of
*  the author's official duties as a United States Government employee and 
*  thus cannot be copyrighted.  This software/database is freely available 
*  to the public for use. The National Library of Medicine and the U.S.
*  Government have not placed any restriction on its use or reproduction.
*
*  Although all reasonable efforts have been taken to ensure the accuracy
*  and reliability of the software and data, the NLM and the U.S.
*  Government do not and cannot warrant the performance or results that
*  may be obtained by using this software or data. The NLM and the U.S.
*  Government disclaim all warranties, express or implied, including
*  warranties of performance, merchantability or fitness for any particular
*  purpose.
*
*  Please cite the author in any work or product based on this material.
*
* ===========================================================================
*
* File Name:  dvncode.h
*
* Author:  Hogue
*
* Version Creation Date:  25 JULY 95
*
* $Revision: 6.1 $
*
* File Description: 
*   	Doubly-linked list functions like ValNode.  	
*
*
*
* Modifications:
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
* $Log: dvncode.h,v $
* Revision 6.1  1999/04/26 20:49:59  lewisg
* changed arguments named list to fix visual c++ bug
*
* Revision 6.0  1997/08/25 18:10:37  madden
* Revision changed to 6.0
*
* Revision 5.0  1996/05/28 14:02:09  ostell
* Set to revision 5.0
*
 * Revision 1.2  1995/08/03  21:44:33  kans
 * numerous changes
 *
*
* ==========================================================================
*/


#ifndef _DVNCODE_
#define _DVNCODE_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct dvalnode {
	Nlm_Int2 choice;      /* to pick a choice or numbering */
	DataVal data;	     /* attached data */
	struct dvalnode PNTR next;  /* next in linked list */
	struct dvalnode PNTR last;  /* last in linked list */
} DValNode, PNTR DValNodePtr;

typedef void (LIBCALLBACK *pFreeFunc)(Pointer pThis);


/* function prototypes here */
DValNodePtr LIBCALL DValNodeNew (DValNodePtr dvp);
DValNodePtr LIBCALL DValNodeAdd (DValNodePtr PNTR head);
DValNodePtr LIBCALL DValNodeLink (DValNodePtr PNTR head, DValNodePtr newnode);
DValNodePtr LIBCALL DValNodeAddStr (DValNodePtr PNTR head, 
		Nlm_Int2 choice, Nlm_CharPtr str);
DValNodePtr LIBCALL DValNodeCopyStr (DValNodePtr PNTR head, Nlm_Int2 choice, Nlm_CharPtr str);
DValNodePtr LIBCALL DValNodeAddInt (DValNodePtr PNTR head, Nlm_Int2 choice, Nlm_Int4 value);
DValNodePtr LIBCALL DValNodeAddBoolean (DValNodePtr PNTR head, Nlm_Int2 choice, Nlm_Boolean value);
DValNodePtr LIBCALL DValNodeAddFloat (DValNodePtr PNTR head, Nlm_Int2 choice, Nlm_FloatHi value);
DValNodePtr LIBCALL DValNodeAddPointer (DValNodePtr PNTR head, Nlm_Int2 choice, Nlm_VoidPtr value);
DValNodePtr LIBCALL DValNodeAddFunction (DValNodePtr PNTR head, Nlm_Int2 choice, Nlm_FnPtr value);
DValNodePtr LIBCALL DValNodeFree (DValNodePtr dvp);
DValNodePtr LIBCALL DValNodeFreeData (DValNodePtr dvp,  pFreeFunc freefn);
DValNodePtr LIBCALL DValNodeExtract (DValNodePtr PNTR headptr, Nlm_Int2 choice);
DValNodePtr LIBCALL DValNodeExtractList (DValNodePtr PNTR headptr, Nlm_Int2 choice);
DValNodePtr LIBCALL DValNodeFindNext (DValNodePtr head, DValNodePtr curr, Nlm_Int2 choice);
Nlm_Int2 LIBCALL DVNodeListLen (DValNodePtr head);
Nlm_Boolean LIBCALL DValNodeRead (DValNodePtr dvp, Nlm_VoidPtr ptr, size_t size);
Nlm_Boolean LIBCALL DValNodeWrite (DValNodePtr dvp, Nlm_VoidPtr ptr, size_t size,  pFreeFunc freefn);
Nlm_Boolean LIBCALL DVNodeListAppend (DValNodePtr head, Nlm_VoidPtr ptr, size_t size);
Nlm_Boolean LIBCALL DValNodeInsert (DValNodePtr where,  DValNodePtr what);
DValNodePtr LIBCALL DValNodeHeadLink (DValNodePtr pdnList,  DValNodePtr dvp);
DValNodePtr LIBCALL DValNodeUnlink (DValNodePtr pdnList, DValNodePtr dvp);
DValNodePtr LIBCALL DValNodeListDelNode (DValNodePtr pdnList,  DValNodePtr dvp,  pFreeFunc freefn);
Nlm_Boolean LIBCALL DValNodeListInsert(DValNodePtr wherelist,  DValNodePtr whatlist);
Nlm_Boolean LIBCALL DValNodeListCat(DValNodePtr dest,  DValNodePtr source);
Nlm_Boolean LIBCALL ValNodeListCat(ValNodePtr dest, ValNodePtr source);
DValNodePtr LIBCALL DValNodeListCut(DValNodePtr what,  DValNodePtr where);

#ifdef __cplusplus
}
#endif


#endif

