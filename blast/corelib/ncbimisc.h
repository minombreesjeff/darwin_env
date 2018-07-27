/*   ncbimisc.h
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
* File Name:  ncbimisc.h
*
* Author:  Gish, Kans, Ostell, Schuler
*
* Version Creation Date:   10/23/91
*
* $Revision: 6.13 $
*
* File Description:
*   	prototypes of miscellaneous functions
*
* Modifications:
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
* 04-15-93 Schuler     Changed _cdecl to LIBCALL
* 06-15-93 Schuler     SGML functions moved to ncbisgml.c
* 06-15-93 Schuler     Added macros for Gestalt functins.
*
* $Log: ncbimisc.h,v $
* Revision 6.13  1999/09/22 23:08:26  kans
* added extended flag to valnode for SeqDesc, maybe later SeqId and SeqLoc
*
* Revision 6.12  1999/07/29 15:58:48  kans
* added bigintvalue, ValNodeAddBigInt (PD)
*
* Revision 6.11  1999/04/23 16:25:36  shavirin
* Added definition of the function Nlm_GetChecksum()
*
* Revision 6.10  1999/04/14 19:37:23  madden
* Add Nlm_ prefix to Int8
*
* Revision 6.9  1999/04/14 15:31:07  madden
* add Nlm_Int8tostr function
*
* Revision 6.8  1999/03/11 16:21:51  kans
* ValNodeSort is more visible copy of jzmisc SortValNode
*
* Revision 6.7  1999/01/27 01:59:52  kans
* cleaned up comment problem
*
* Revision 6.6  1999/01/21 20:08:37  ostell
* added SwitchUint2 and 4, added integer bytestores
*
* Revision 6.5  1999/01/07 20:18:51  victorov
* MD5 functions declarations fixed
*
* Revision 6.4  1999/01/07 15:20:58  victorov
* added Nlm_ prefix to MD5 functions to avoid conflicts
*
* Revision 6.3  1999/01/06 22:49:27  victorov
* added MD5 hash calculator
*
* Revision 6.2  1998/10/13 20:49:40  vakatov
* + Nlm_PlatformName()
*
* Revision 6.1  1998/04/08 16:49:59  kans
* ValNodeLen returns Int4
*
* Revision 6.0  1997/08/25 18:16:49  madden
* Revision changed to 6.0
*
* Revision 5.3  1997/01/28 21:26:51  epstein
* move doubly-linked list handling functions from ni_list.[ch]
*
 * Revision 5.2  1996/12/10  16:09:16  kans
 * added ValNodeLen function
 *
 * Revision 5.1  1996/12/03  21:48:33  vakatov
 * Adopted for 32-bit MS-Windows DLLs
 *
 * Revision 5.0  1996/05/28  13:18:57  ostell
 * Set to revision 5.0
 *
 * Revision 4.2  1996/03/27  15:45:53  epstein
 * make byte-swapping macros safer on big-endian systems
 *
 * Revision 4.1  1996/03/26  16:31:00  epstein
 * migrate byte-swapping functions to ncbimisc.[ch]
 *
 * Revision 4.0  1995/07/26  13:46:50  ostell
 * force revision to 4.0
 *
 * Revision 2.14  1995/05/15  18:45:58  ostell
 * added Log line
 *
*
*
* ==========================================================================
*/

#ifndef _NCBIMISC_
#define _NCBIMISC_

#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif


#ifdef __cplusplus
extern "C" {
#endif

/* Obtain length of ASCII representation of signed and unsigned long integers */
/* opts&1 ==> use commas before thousands, millions, etc. when |value| >=10000*/
/* opts&2 ==> use commas before thousands, millions, etc. when |value| >=1000 */
/* opts&4 ==> prepend + sign to any positive values */
NLM_EXTERN int LIBCALL Nlm_Lwidth PROTO((long x, int opts));
NLM_EXTERN int LIBCALL Nlm_Ulwidth PROTO((unsigned long x, int opts));

/* convert integers to ASCII in static storage */
/* Same 1,2,4 opts as Nlm_Lwidth and Nlm_Ulwidth */
NLM_EXTERN char * LIBCALL Nlm_Ltostr PROTO((long x, int opts));
NLM_EXTERN char * LIBCALL Nlm_Ultostr PROTO((unsigned long x, int opts));
/* Nlm_Int8tostr -- convert a signed long integer to ASCII */
NLM_EXTERN CharPtr LIBCALL Nlm_Int8tostr PROTO((Nlm_Int8 value, int opts));


/* Sorting */
NLM_EXTERN void LIBCALL Nlm_HeapSort PROTO((VoidPtr base, size_t nel, size_t width, int (LIBCALLBACK *cmp) (VoidPtr, VoidPtr) ));

/* Platform name */
NLM_EXTERN const Nlm_Char* Nlm_PlatformName(void);


/*****************************************************************************
*
*   DataVal = a universal data type
*   ValNode = a linked list of DataVal
*
*****************************************************************************/


typedef union dataval {
	Nlm_VoidPtr ptrvalue;
	Nlm_Int4 intvalue;
	Nlm_FloatHi realvalue;
	Nlm_Boolean boolvalue;
	Nlm_FnPtr	funcvalue;
	Nlm_Int8    bigintvalue;
}	DataVal, PNTR DataValPtr;

typedef struct valnode {
	Nlm_Uint1 choice;          /* to pick a choice */
	Nlm_Uint1 extended;        /* extra fields reserved to NCBI allocated in structure */
	DataVal data;              /* attached data */
	struct valnode PNTR next;  /* next in linked list */
} ValNode, PNTR ValNodePtr;

/*****************************************************************************
*
*   ValNodeNew(vnp)
*      adds after last node in list if vnp not NULL
*
*   ValNodeLen(vnp)
*      returns the number of nodes in the linked list
*
*   ValNodeAdd(head)
*      adds after last node in list if *head not NULL
*      If *head is NULL, sets it to the new ValNode
*      returns pointer to the NEW node added
*
*   ValNodeLink(head, newnode)
*      adds newnode at end of chain
*      if (*head == NULL) *head = newnode
*      ALWAYS returns pointer to START of chain
*
*   ValNodeAddStr (head, choice, str)
*      adds like ValNodeAdd()
*      sets newnode->choice = choice (if choice does not matter, use 0)
*      sets newnode->data.ptrvalue = str
*         does NOT copy str
*      if str == NULL, does NOT add a ValNode
*
*   ValNodeCopyStr (head, choice, str)
*      adds like ValNodeAdd()
*      sets newnode->choice = choice (if choice does not matter, use 0)
*      sets newnode->data.ptrvalue = str
*         makes a COPY of str
*      if str == NULL, does NOT add a ValNode
*
*   ValNodeAddInt (head, choice, value)
*      adds like ValNodeAdd()
*      sets newnode->choice = choice (if choice does not matter, use 0)
*      sets newnode->data.intvalue = value
*
*   ValNodeAddBigInt (head, choice, value)
*      adds like ValNodeAdd()
*      sets newnode->choice = choice (if choice does not matter, use 0)
*      sets newnode->data.bigintvalue = value
*
*   ValNodeAddBoolean (head, choice, value)
*      adds like ValNodeAdd()
*      sets newnode->choice = choice (if choice does not matter, use 0)
*      sets newnode->data.boolvalue = value
*
*   ValNodeAddFloat (head, choice, value)
*      adds like ValNodeAdd()
*      sets newnode->choice = choice (if choice does not matter, use 0)
*      sets newnode->data.realvalue = value
*
*   ValNodeAddPointer (head, choice, value)
*      adds like ValNodeAdd()
*      sets newnode->choice = choice (if choice does not matter, use 0)
*      sets newnode->data.ptrvalue = value
*
*   ValNodeAddFunction (head, choice, value)
*      adds like ValNodeAdd()
*      sets newnode->choice = choice (if choice does not matter, use 0)
*      sets newnode->data.funcvalue = value
*
*   ValNodeFree(vnp)
*   	frees whole chain of ValNodes
*       Does NOT free associated data pointers
*
*   ValNodeFreeData(vnp)
*   	frees whole chain of ValNodes
*       frees associated data pointers - BEWARE of this if these are not
*           allocated single memory block structures.
*
*   ValNodePtr ValNodeExtract(headptr, choice)
*       removes first node in chain where ->choice == choice
*       rejoins chain after removing the node
*       sets node->next to NULL
*
*   ValNodePtr ValNodeExtractList(headptr, choice)
*       removes ALL nodes in chain where ->choice == choice
*       rejoins chain after removing the nodes
*       returns independent chain of extracted nodes
*
*   ValNodeFindNext (head, curr, choice)
*   	Finds next ValNode with vnp->choice == choice after curr
*       If curr == NULL, starts at head of list
*       If choice < 0 , returns all ValNodes
*       Returns NULL, when no more found
*
*   ValNodeSort (list, compar)
*   	Copied from SortValNode in jzcoll, renamed, for more general access
*   	Makes array from ValNode list, calls HeapSort, reconnects ValNode list
*
*****************************************************************************/
NLM_EXTERN ValNodePtr LIBCALL ValNodeNew PROTO((ValNodePtr vnp));
NLM_EXTERN Nlm_Int4   LIBCALL ValNodeLen PROTO((ValNodePtr vnp));
NLM_EXTERN ValNodePtr LIBCALL ValNodeAdd PROTO((ValNodePtr PNTR head));
NLM_EXTERN ValNodePtr LIBCALL ValNodeLink PROTO((ValNodePtr PNTR head, ValNodePtr newnode));
NLM_EXTERN ValNodePtr LIBCALL ValNodeAddStr PROTO((ValNodePtr PNTR head, Nlm_Int2 choice, Nlm_CharPtr str));
NLM_EXTERN ValNodePtr LIBCALL ValNodeCopyStr PROTO((ValNodePtr PNTR head, Nlm_Int2 choice, Nlm_CharPtr str));
NLM_EXTERN ValNodePtr LIBCALL ValNodeAddInt PROTO((ValNodePtr PNTR head, Nlm_Int2 choice, Nlm_Int4 value));
NLM_EXTERN ValNodePtr LIBCALL ValNodeAddBigInt (ValNodePtr PNTR head, Nlm_Int2 choice, Nlm_Int8 value);
NLM_EXTERN ValNodePtr LIBCALL ValNodeAddBoolean PROTO((ValNodePtr PNTR head, Nlm_Int2 choice, Nlm_Boolean value));
NLM_EXTERN ValNodePtr LIBCALL ValNodeAddFloat PROTO((ValNodePtr PNTR head, Nlm_Int2 choice, Nlm_FloatHi value));
NLM_EXTERN ValNodePtr LIBCALL ValNodeAddPointer PROTO((ValNodePtr PNTR head, Nlm_Int2 choice, Nlm_VoidPtr value));
NLM_EXTERN ValNodePtr LIBCALL ValNodeAddFunction PROTO((ValNodePtr PNTR head, Nlm_Int2 choice, Nlm_FnPtr value));
NLM_EXTERN ValNodePtr LIBCALL ValNodeFree PROTO((ValNodePtr vnp));
NLM_EXTERN ValNodePtr LIBCALL ValNodeFreeData PROTO((ValNodePtr vnp));
NLM_EXTERN ValNodePtr LIBCALL ValNodeExtract PROTO((ValNodePtr PNTR headptr, Nlm_Int2 choice));
NLM_EXTERN ValNodePtr LIBCALL ValNodeExtractList PROTO((ValNodePtr PNTR headptr, Nlm_Int2 choice));
NLM_EXTERN ValNodePtr LIBCALL ValNodeFindNext PROTO((ValNodePtr head, ValNodePtr curr, Nlm_Int2 choice));
NLM_EXTERN ValNodePtr LIBCALL ValNodeSort PROTO((ValNodePtr list, int (LIBCALLBACK *compar) (VoidPtr, VoidPtr)));

/*** old prototypes ******
NLM_EXTERN ValNodePtr LIBCALL ValNodeLink PROTO((ValNodePtr vnp, ValNodePtr newnode));
NLM_EXTERN ValNodePtr LIBCALL ValNodeExtract PROTO((ValNodePtr PNTR headptr, Nlm_Uint1 choice));
**************************/

NLM_EXTERN ValNodePtr  LIBCALL NodeListNew PROTO((void));
NLM_EXTERN ValNodePtr  LIBCALL NodeListFree PROTO((ValNodePtr head));
NLM_EXTERN Nlm_Int2    LIBCALL NodeListLen PROTO((ValNodePtr head));
NLM_EXTERN ValNodePtr  LIBCALL NodeListFind PROTO((ValNodePtr head, Nlm_Int2 item, Nlm_Boolean extend));
NLM_EXTERN Nlm_Boolean LIBCALL NodeListRead PROTO((ValNodePtr head, Nlm_Int2 item, Nlm_VoidPtr ptr, size_t size));
NLM_EXTERN Nlm_Boolean LIBCALL NodeListWrite PROTO((ValNodePtr head, Nlm_Int2 item, Nlm_VoidPtr ptr, size_t size));
NLM_EXTERN Nlm_Boolean LIBCALL NodeListAppend PROTO((ValNodePtr head, Nlm_VoidPtr ptr, size_t size));
NLM_EXTERN Nlm_Boolean LIBCALL NodeListInsert PROTO((ValNodePtr head, Nlm_Int2 item, Nlm_VoidPtr ptr, size_t size));
NLM_EXTERN Nlm_Boolean LIBCALL NodeListReplace PROTO((ValNodePtr head, Nlm_Int2 item, Nlm_VoidPtr ptr, size_t size));
NLM_EXTERN Nlm_Boolean LIBCALL NodeListDelete PROTO((ValNodePtr head, Nlm_Int2 item));

/* doubly-linked lists */
typedef struct Node PNTR NodePtr;
typedef struct Node {
        VoidPtr         elem;           /* pointer to the element */
        NodePtr         last;           /* previous element */
        NodePtr         next;           /* next element */
} Node;

#define ASCEND  0       /* order for ListSort */
#define DESCEND 1

/* functions for doubly-linked lists */

NLM_EXTERN NodePtr LIBCALL          ListInsert PROTO((VoidPtr elem, NodePtr after));

NLM_EXTERN NodePtr LIBCALL          ListInsertPrev PROTO((VoidPtr elem, NodePtr before));

NLM_EXTERN NodePtr LIBCALL          ListDelete PROTO((NodePtr node));

NLM_EXTERN NodePtr LIBCALL          ListGetNext PROTO((NodePtr after));

NLM_EXTERN void LIBCALL             ListSwapAdj PROTO((NodePtr priornode, NodePtr nextnode));

NLM_EXTERN NodePtr LIBCALL          ListSort PROTO((NodePtr sl, int (*cmpfunc)(NodePtr, NodePtr), int order));

NLM_EXTERN void LIBCALL             ListBreakRing PROTO((NodePtr np));

NLM_EXTERN void LIBCALL             ListConnectRing PROTO((NodePtr np));
NLM_EXTERN NodePtr LIBCALL          ListStrCopy PROTO((NodePtr strlist));
NLM_EXTERN void LIBCALL             ListStrDel PROTO((NodePtr np));


    /****** Choice is a compact variant of ValNode **********/

typedef union _IntPnt_ {
    Int4 intvalue;
    Pointer ptrvalue;
} IntPnt;

typedef struct _Choice_ {
    Uint1 choice;
    IntPnt value;
} Choice, PNTR ChoicePtr;

#define Lwidth	Nlm_Lwidth
#define Ulwidth	Nlm_Ulwidth
#define Ltostr	Nlm_Ltostr
#define Ultostr	Nlm_Ultostr
#define HeapSort	Nlm_HeapSort

#ifdef OS_MAC
NLM_EXTERN void Nlm_CtoPstr PROTO((Nlm_CharPtr str));
NLM_EXTERN void Nlm_PtoCstr PROTO((Nlm_CharPtr str));
#endif

/* these functions reverse byte order in integers 
   calling the same function a second time switches it back
   the native ENDIAN nature of the machine is not considered
*/
NLM_EXTERN Uint2 Nlm_SwitchUint2 PROTO ((Uint2 value));
NLM_EXTERN void Nlm_SwitchUint2Buff PROTO ((Uint2 *buff, int count));
NLM_EXTERN unsigned long  Nlm_SwitchLong PROTO ((unsigned long value));
NLM_EXTERN void Nlm_SwitchLongBuff PROTO ((unsigned long *buff, int count));
NLM_EXTERN Uint4  Nlm_SwitchUint4 PROTO ((Uint4 value));
NLM_EXTERN void Nlm_SwitchUint4Buff PROTO ((Uint4 *buff, int count));

#define SwitchUint2 Nlm_SwitchUint2
#define SwitchUint2Buff Nlm_SwitchUint2Buff
#define SwitchLong Nlm_SwitchLong
#define SwitchLongBuff Nlm_SwitchLongBuff
#define SwitchUint4 Nlm_SwitchUint4
#define SwitchUint4Buff Nlm_SwitchUint4Buff

/** The following defines ALWAYS assume the value to switched is
    BIG_ENDIAN. This is used to allow portable use of binary integers
	in some NCBI applications such as BLAST and some indexes ****/

#ifdef IS_LITTLE_ENDIAN
#define Nlm_SwapUint2(value)		Nlm_SwitchUint2(value)
#define Nlm_SwapUint2Buff(buff, count) Nlm_SwitchUint2Buff(buff, count)
#define Nlm_SwapLong(value)		Nlm_SwitchLong(value)
#define Nlm_SwapLongBuff(buff, count) Nlm_SwitchLongBuff(buff, count)
#define Nlm_SwapUint4(value)		Nlm_SwitchUint4(value)
#define Nlm_SwapUint4Buff(buff, count) Nlm_SwitchUint4Buff(buff, count)
#else
#define Nlm_SwapUint2(value)		(value)
#define Nlm_SwapUint2Buff(buff, count)
#define Nlm_SwapLong(value)		(value)
#define Nlm_SwapLongBuff(buff, count)
#define Nlm_SwapUint4(value)		(value)
#define Nlm_SwapUint4Buff(buff, count)
#endif

#define SwapUint2 Nlm_SwapUint2
#define SwapUint2Buff Nlm_SwapUint2Buff
#define SwapLong Nlm_SwapLong
#define SwapLongBuff Nlm_SwapLongBuff
#define SwapUint4 Nlm_SwapUint4
#define SwapUint4Buff Nlm_SwapUint4Buff


/**
 * MD5 stuff
 */
typedef struct md5context_ {
	Nlm_Uint4 buf[4];
	Nlm_Uint4 bits[2];
	Nlm_Uchar in[64];
} Nlm_MD5Context, PNTR Nlm_MD5ContextPtr;

NLM_EXTERN void LIBCALL Nlm_MD5Init PROTO((Nlm_MD5ContextPtr context));
NLM_EXTERN void LIBCALL Nlm_MD5Update PROTO((Nlm_MD5ContextPtr context, Nlm_UcharPtr buf, Nlm_Uint4 len));
NLM_EXTERN void LIBCALL Nlm_MD5Final PROTO((Nlm_MD5ContextPtr context, Nlm_Uchar digest[16]));
NLM_EXTERN void LIBCALL Nlm_MD5Transform PROTO((Nlm_Uint4 buf[4], Nlm_Uint4 in[16]));

#define MD5Context Nlm_MD5Context
#define MD5ContextPtr Nlm_MD5ContextPtr
#define MD5Init Nlm_MD5Init
#define MD5Update Nlm_MD5Update
#define MD5Final Nlm_MD5Final
#define MD5Transform Nlm_MD5Transform

/* Error codes for the CTX_NCBIMISC context */

Uint4 Nlm_GetChecksum(CharPtr p);

#ifdef __cplusplus
}
#endif


#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif


#endif /* !_NCBIMISC_ */

