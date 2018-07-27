/*   dvncode.c
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
* File Name:  dvncode.c
*
* Author: Hogue 
*
* Version Creation Date:   25 JULY 95
*
* $Revision: 6.2 $
*
* File Description: 
*   	Doubly-linked list functions like ValNode ones.
*	(with my apologies to the original authors)
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
* $Log: dvncode.c,v $
* Revision 6.2  1999/11/10 23:19:39  lewisg
* rewrite of selection code for ddv
*
* Revision 6.1  1999/04/26 20:49:59  lewisg
* changed arguments named list to fix visual c++ bug
*
* Revision 6.0  1997/08/25 18:10:35  madden
* Revision changed to 6.0
*
* Revision 5.0  1996/05/28 14:02:09  ostell
* Set to revision 5.0
*
 * Revision 1.3  1996/01/31  21:24:16  hogue
 * Minor change to add if (list) tests
 *
 * Revision 1.2  1995/08/03  21:44:33  kans
 * numerous changes
 *
 * Revision 1.1  1995/08/02  17:07:15  kans
 * Initial revision
 *
*
* ==========================================================================
*/

#include <ncbi.h>
#include <dvncode.h>


/*****************************************************************************
*
*   DValNodeNew(dvp)
*      adds after last node in list if dvp not NULL
*      returns a pointer to the node.
*****************************************************************************/
DValNodePtr LIBCALL DValNodeNew (DValNodePtr dvp)

{
	DValNodePtr newnode;

	newnode = (DValNodePtr)Nlm_MemNew((size_t)(sizeof(DValNode)));
	if (dvp)
    	{
	    while (dvp->next)
		dvp = dvp->next;
	    dvp->next = newnode;
	    newnode->last = dvp;
    	}
	else
	{
	    newnode->last = NULL;
	    newnode->next = NULL;
	}
	
	return newnode;
}

/*****************************************************************************
*
*   DValNodeAdd(head)
*      adds after last node in list if *head not NULL
*      If *head is NULL, sets it to the new DValNode
*      returns pointer to the NEW node added
*
*****************************************************************************/
DValNodePtr LIBCALL DValNodeAdd (DValNodePtr PNTR head)

{
	DValNodePtr newnode;

	if (head)
	{
		newnode = DValNodeNew(*head);
		if (*head == NULL)
			*head = newnode;
	}
	else
		newnode = DValNodeNew(NULL);

	return newnode;
}

/*****************************************************************************
*
*   DValNodeLink(head, newnode)
*      adds newnode at end of chain
*      if (*head == NULL) *head = newnode
*      ALWAYS returns pointer to START of chain
*  see also DValNodeHeadLink - adds at head of chain....
*****************************************************************************/
DValNodePtr LIBCALL DValNodeLink (DValNodePtr PNTR head, DValNodePtr newnode)
{
   DValNodePtr dvp;

   if (head == NULL)
	 return newnode;

   dvp = *head;
   
   if (dvp)
   {
        while (dvp->next)
            dvp = dvp->next;
        dvp->next = newnode;
	newnode->last = dvp;
    }
    else
		*head = newnode;

   return *head;
}

/*****************************************************************************
*
*   DValNodeAddStr (head, choice, str)
*      adds like DValNodeAdd()
*      sets newnode->choice = choice (if choice does not matter, use 0)
*      sets newnode->data.ptrvalue = str
*         does NOT copy str
*      if str == NULL, does not add a DValNode
*	returns the new node
*****************************************************************************/
DValNodePtr LIBCALL DValNodeAddStr (DValNodePtr PNTR head, 
		Nlm_Int2 choice, Nlm_CharPtr str)
{
	DValNodePtr newnode;

	if (str == NULL) return NULL;

	newnode = DValNodeAdd(head);
	if (newnode)
	{
		newnode->choice = choice;
		newnode->data.ptrvalue = (Nlm_VoidPtr)str;
	}

	return newnode;
}

/*****************************************************************************
*
*   DValNodeCopyStr (head, choice, str)
*      adds like DValNodeAdd()
*      sets newnode->choice = choice (if choice does not matter, use 0)
*      sets newnode->data.ptrvalue = str
*         makes a COPY of str which must be FREED
*	   with DValNodeFreeData(node, NULL);
*      if str == NULL, does not add a DValNode
*    returns the new node
*****************************************************************************/
DValNodePtr LIBCALL DValNodeCopyStr (DValNodePtr PNTR head, Nlm_Int2 choice, Nlm_CharPtr str)
{
	DValNodePtr newnode;

	if (str == NULL) return NULL;

	newnode = DValNodeAdd(head);
	if (newnode)
	{
		newnode->choice = choice;
		newnode->data.ptrvalue = StringSave(str);
	}

	return newnode;
}

/*****************************************************************************
*
*   DValNodeAddInt (head, choice, value)
*      adds like DValNodeAdd()
*      sets newnode->choice = choice (if choice does not matter, use 0)
*      sets newnode->data.intvalue = value
*	 returns the new node
*****************************************************************************/
DValNodePtr LIBCALL DValNodeAddInt (DValNodePtr PNTR head, Nlm_Int2 choice, Nlm_Int4 value)
{
	DValNodePtr newnode;

	newnode = DValNodeAdd(head);
	if (newnode)
	{
		newnode->choice = choice;
		newnode->data.intvalue = value;
	}

	return newnode;
}

/*****************************************************************************
*
*   DValNodeAddBoolean (head, choice, value)
*      adds like DValNodeAdd()
*      sets newnode->choice = choice (if choice does not matter, use 0)
*      sets newnode->data.boolvalue = value
*	 returns the new node
*****************************************************************************/
DValNodePtr LIBCALL DValNodeAddBoolean (DValNodePtr PNTR head, Nlm_Int2 choice, Nlm_Boolean value)
{
	DValNodePtr newnode;

	newnode = DValNodeAdd(head);
	if (newnode)
	{
		newnode->choice = choice;
		newnode->data.boolvalue = value;
	}

	return newnode;
}

/*****************************************************************************
*
*   DValNodeAddFloat (head, choice, value)
*      adds like DValNodeAdd()
*      sets newnode->choice = choice (if choice does not matter, use 0)
*      sets newnode->data.realvalue = value
*	 returns the new node
*****************************************************************************/
DValNodePtr LIBCALL DValNodeAddFloat (DValNodePtr PNTR head, Nlm_Int2 choice, Nlm_FloatHi value)
{
	DValNodePtr newnode;

	newnode = DValNodeAdd(head);
	if (newnode)
	{
		newnode->choice = choice;
		newnode->data.realvalue = value;
	}

	return newnode;
}

/*****************************************************************************
*
*   DValNodeAddPointer (head, choice, value)
*      adds like DValNodeAdd()
*      sets newnode->choice = choice (if choice does not matter, use 0)
*      sets newnode->data.ptrvalue = value
*	 returns the new node
*****************************************************************************/
DValNodePtr LIBCALL DValNodeAddPointer (DValNodePtr PNTR head, Nlm_Int2 choice, Nlm_VoidPtr value)
{
	DValNodePtr newnode;

	newnode = DValNodeAdd(head);
	if (newnode)
	{
		newnode->choice = choice;
		newnode->data.ptrvalue = value;
	}

	return newnode;
}

/*****************************************************************************
*
*   DValNodeAddFunction (head, choice, value)
*      adds like DValNodeAdd()
*      sets newnode->choice = choice (if choice does not matter, use 0)
*      sets newnode->data.funcvalue = value
*      returns the new node
*****************************************************************************/
DValNodePtr LIBCALL DValNodeAddFunction (DValNodePtr PNTR head, Nlm_Int2 choice, Nlm_FnPtr value)
{
	DValNodePtr newnode;

	newnode = DValNodeAdd(head);
	if (newnode)
	{
		newnode->choice = choice;
		newnode->data.funcvalue = value;
	}

	return newnode;
}


/*****************************************************************************
*
*   DValNodeFree(dvp)
*   	frees whole chain of DValNodes
*       Does NOT free associated data pointers
*           see DValNodeFreeData()
*   returns NULL
*****************************************************************************/
DValNodePtr LIBCALL DValNodeFree (DValNodePtr dvp)
{
	DValNodePtr next;
/* unlinks the rest of the chain if there is one */
    if (dvp == NULL) return NULL;
	if (dvp->last)
		  dvp->last->next = NULL;
	while (dvp)
	{
		next = dvp->next;
		Nlm_MemFree(dvp);
		dvp = next;
		
	}
	return NULL;
}

/*****************************************************************************
*
*   DValNodeFreeData(dvp, freefn)
*   	frees whole chain of DValNodes
*	calls your function "freefn" with arg (dvp->data.ptrvalue)
*	so you can free more complex structures yourself.
*       frees associated data pointers - BEWARE of this if these are not
*           allocated single memory block structures.
*
*  say a function FreeDataInNode(PTR mydata) 
*  call it with DValNodeFreeDataCall(dvp, FreeDataInNode);
*  returns NULL
*****************************************************************************/
DValNodePtr LIBCALL DValNodeFreeData (DValNodePtr dvp,  pFreeFunc freefn)
{
	DValNodePtr next;
	
	/* unlink the start of the chain if there is one */
	if (dvp->last)
	    dvp->last->next = NULL;
	while (dvp)
	{   
		if (freefn)
		   (*freefn)(dvp->data.ptrvalue);
		else
		    Nlm_MemFree(dvp->data.ptrvalue);   
		next = dvp->next;
		Nlm_MemFree(dvp);
		dvp = next;
	}
	return NULL;
}

/*****************************************************************************
*
*   DValNodePtr DValNodeExtract(headptr, choice)
*       removes first node in chain where ->choice == choice
*       rejoins chain after removing the node
*       sets node->next and node->last to NULL
*      returns the node extracted
*****************************************************************************/
DValNodePtr LIBCALL DValNodeExtract (DValNodePtr PNTR headptr, Nlm_Int2 choice)
{
   DValNodePtr dvp = * headptr;

    while (dvp)
    {
        if (dvp->choice ==  choice)
        {
            if (dvp->last == NULL)    /* first one */
	    {
		if (dvp->next) /* not the only one */
		    dvp->next->last = NULL;
                * headptr = dvp->next;
	    }
            else
	    {
                dvp->last->next = dvp->next;
		if (dvp->next) /* not the end */
		    dvp->next->last = dvp->last;
	    }
            dvp->next = NULL;
	    dvp->last = NULL;
            return dvp;
        }
        else
	    dvp = dvp->next;
    }

    return NULL;    /* not found */
}

/*****************************************************************************
*
*   DValNodePtr DValNodeExtractList(headptr, choice)
*       removes ALL nodes in chain where ->choice == choice
*       rejoins chain after removing the nodes
*       returns independent chain of extracted nodes
*
*****************************************************************************/
DValNodePtr LIBCALL DValNodeExtractList (DValNodePtr PNTR headptr, Nlm_Int2 choice)
{
    DValNodePtr first = NULL, dvp;

	dvp = DValNodeExtract(headptr, choice);
    while (dvp != NULL)
    {
		if (first == NULL)
		{
    		    first = dvp;
		}
		else
		  DValNodeLink(&first,  dvp);
	}

    return first;
}

/*****************************************************************************
*
*   DValNodeFindNext (head, curr, choice)
*   	Finds next DValNode with dvp->choice == choice after curr
*       If curr == NULL, starts at head of list
*       If choice < 0 , returns all DValNodes
*       Returns NULL, when no more found
*
*****************************************************************************/
DValNodePtr LIBCALL DValNodeFindNext (DValNodePtr head, DValNodePtr curr, Nlm_Int2 choice)
{
	if (head == NULL) return NULL;

	if (curr == NULL)
		curr = head;
	else
		curr = curr->next;

	while (curr)
	{
		if ((choice < 0) || (curr->choice == choice))
			return curr;
		curr = curr->next;
	}

	return curr;
}

/*
 *  DVNodeListLen(list) - returns the length of the list.
 */

Nlm_Int2 LIBCALL DVNodeListLen (DValNodePtr head)
{
  Nlm_Int2  item;

  item = 0;
  if (head) {
    while (head->next) {
      head = head->next;
      item++;
    }
  }
  return item;
}

/*
 * DValNodeRead(dvp,  ptr, size) reads the contents of 
 * dvp->data.ptrvalue from 0-size and copies to an
 * allocated memory block of size pointed to by ptr.
 * returns true or false
 */

Nlm_Boolean LIBCALL DValNodeRead (DValNodePtr dvp, Nlm_VoidPtr ptr, size_t size)
{
  Nlm_Boolean  copied;
  Nlm_BytePtr  dst;
  Nlm_BytePtr  src;
  
  copied = FALSE;
  if (dvp && ptr) {
    if (dvp->data.ptrvalue) {
      dst = (Nlm_BytePtr) ptr;
      src = (Nlm_BytePtr) (dvp->data.ptrvalue);
      while (size > 0) {
        *dst = *src;
        dst++;
        src++;
        size--;
      }
      copied = TRUE;
    } else {
      Nlm_MemFill (ptr, 0, size);
    }
  }
  return copied;
}

/*
 * DValNodeWrite(dvp, ptr, size,  freefn)  copies the contents of
 * a 0-size block pointed to by ptr into a previously 
 * allocated dvp.  If there is data in dvp->data.ptrvlaue it is
 * freed with either MemFree (freefn=NULL) or a user function 
 * (* freefn)
 * 
 * returns true or false
 */

Nlm_Boolean LIBCALL DValNodeWrite (DValNodePtr dvp, Nlm_VoidPtr ptr, size_t size,  pFreeFunc freefn)
{
  Nlm_Boolean  copied;
  Nlm_BytePtr  dst;
  Nlm_BytePtr  src;

  copied = FALSE;
  if (dvp) {
    if (freefn)   
	(* freefn)(dvp->data.ptrvalue);
     else  
	 MemFree (dvp->data.ptrvalue);
    if (ptr) {
      dvp->data.ptrvalue = MemNew (size);
      if (dvp->data.ptrvalue) {
        dst = (Nlm_BytePtr) (dvp->data.ptrvalue);
        src = (Nlm_BytePtr) ptr;
        while (size > 0) {
          *dst = *src;
          dst++;
          src++;
          size--;
        }
        copied = TRUE;
      }
    }
  }
  return copied;
}


/* 
 * DVNodeListAppend(list, ptr, size) appends a new node onto list
 * and copies the contents of ptr (of size) to the new node.
 * returns true or false
 */

Nlm_Boolean LIBCALL DVNodeListAppend (DValNodePtr head, Nlm_VoidPtr ptr, size_t size)
{
  Nlm_Boolean  copied;
  DValNodePtr   dvp;

  copied = FALSE;
  if (head && ptr) {
    dvp = DValNodeNew (head);
    copied = DValNodeWrite (dvp, ptr, size,  NULL);
  }
  return copied;
}


/*
 * 
 * DValNodeInsert(where,  dvn)
 * this inserts a free node dvn after item where in any list
 * returns True or False
 */ 
 
Nlm_Boolean LIBCALL DValNodeInsert (DValNodePtr where,  DValNodePtr what)
{
    if (where && what)
    {
	if ((!what->next) && (!what->last))
	{ /* must be a free node */
	    if (where->next)
		where->next->last = what;
	    what->last = where;
	    what->next = where->next;
	    where->next = what;
	    return TRUE; 
	}
    }
    return FALSE;
}


/*
 *
 * DValNodeHeadLink(list, dvp) links 
 * dvp onto head of a list,  returns
 * the list
 */
 
DValNodePtr LIBCALL DValNodeHeadLink (DValNodePtr pdnList,  DValNodePtr dvp)
{
    if (dvp)
    {
	if ((!dvp->next) && (!dvp->last))
	{   /* really is a free node? */
	    dvp->next = pdnList;
	    if (pdnList)
	      pdnList->last = dvp;
	    return dvp;  
	}
    }
    return pdnList;
}

/*
 * DValNodeUnlink(list, dvp)
 *
 * DValNodeUnlink returns a new list after removing a node from it 
 * call list = DValNodeUnlink(list, node to unlink)  
 * then you can free the data, and free the DValnode manually   */

DValNodePtr LIBCALL DValNodeUnlink (DValNodePtr pdnList, DValNodePtr dvp)
{
    if (!pdnList) return NULL;
    if (dvp)
    {
	if((!dvp->next) && (!dvp->last))
	{
    	    /* not linked */
	    /* if the list by definition will have no elements */
	    if (dvp == pdnList)
	       return NULL;
	    else /* some other list! */
	       return pdnList;   
	}
	if (!dvp->last)
	{   /* first case; assumes dvp=list */
	    pdnList = pdnList->next;
	    if (pdnList) pdnList->last = NULL;
	    dvp->next = NULL;
	    return pdnList;
	}
	if (!dvp->next)
	{   /* last case */
	    dvp->last->next = NULL;
	    dvp->last = NULL;
	    return pdnList;
	}
	else
	{   /* middle case */
	    dvp->next->last = dvp->last;
	    dvp->last->next = dvp->next;
	    dvp->next = NULL;
	    dvp->last = NULL;
	    return pdnList;
	}
    }
    return pdnList;
}

/*
 * DValNodeListDelNode(list, dvp, freefn)
 * USE THIS TO REMOVE NODES FROM A LIST!  First unlinks,  then
 * frees the data with either (*freefn)(dvp->data.ptrvalue) or
 * if freefn = NULL with MemFree(dvp->data.ptrvalue)
 * returns the list
 */


DValNodePtr LIBCALL DValNodeListDelNode (DValNodePtr pdnList,  DValNodePtr dvp,  pFreeFunc freefn)
{
    pdnList = DValNodeUnlink(pdnList,  dvp);
    dvp = DValNodeFreeData(dvp,  freefn);
  return pdnList;
}

/*
 * DValNodeListInsert(where, list) inserts the doubly-
 * linked "list" into where in some other doubly linked list
 * returns true or false
 */
Nlm_Boolean LIBCALL DValNodeListInsert(DValNodePtr wherelist,  DValNodePtr whatlist)
{
    DValNodePtr insert,  where = wherelist;
    if (whatlist)
    if (wherelist)
    {
	do
	{
	    insert = whatlist;
	    whatlist = DValNodeUnlink(insert, whatlist); 
	    if(DValNodeInsert(where, insert))
		where = insert;
	}while (whatlist);
    return TRUE;
    }
    return FALSE;	
}

/*
 * DValNodeListCat(dest, source) concatenates two doubly
 * -linked lists,  returns True or False.
 */

Nlm_Boolean LIBCALL DValNodeListCat(DValNodePtr dest,  DValNodePtr source)
{
    DValNodePtr place = dest;
    if (dest)
    if (source)
    {
	while(place->next)
	    place = place->next;
	place->next = source;
	source->last = place;
	return TRUE;	  
    }
    return FALSE;
}

/*
 *  ValNodeListCat(dest, source) concatenates two singly
 * -linked lists,  returns True or False.
 */

Nlm_Boolean LIBCALL ValNodeListCat(ValNodePtr dest, ValNodePtr source)
{
   ValNodePtr place = dest;
   if (dest)
   if (source)
   {
     while(place->next)
         place = place->next;
     place->next = source;
     return TRUE;
   }
   return FALSE;
}

/*
 * DValNodeListCut(what, where)  cuts list what at where,  return
 * the new list head where
 */
DValNodePtr LIBCALL DValNodeListCut(DValNodePtr what,  DValNodePtr where)
{
    if (what)
    if (where)
    {
	if (where->last)
	    where->last->next = NULL;
	where->last = NULL;
	return where;    
    }
    return NULL;
}




/* test code follows */
/*
#define NUMARGS 1
Args myargs[NUMARGS] = {
     {"Dummy Arg","stdout", NULL,NULL, TRUE,'f',ARG_FILE_OUT,0.0,0,NULL}
      };

void WalkList(DValNodePtr pdvnThis)
{
    DValNodePtr pdvnTemp;
    pdvnTemp = pdvnThis;
    printf("Walking Up the list\n");
    if(!pdvnThis)
	return;
    while (pdvnTemp->next)
    {
	printf("%s\n", (CharPtr) (pdvnTemp->data.ptrvalue));
	pdvnTemp=pdvnTemp->next;
    }
    printf("%s\n", (CharPtr) (pdvnTemp->data.ptrvalue));
    printf("Walking Down the list\n");
    while (pdvnTemp->last)
    {
	printf("%s\n", (CharPtr) (pdvnTemp->data.ptrvalue));
	pdvnTemp=pdvnTemp->last;
    }
    printf("%s\n", (CharPtr) (pdvnTemp->data.ptrvalue));
}
	

	
Int2 Main(void)
{
    Int2 i;
    CharPtr mys;
    Char buf[500];
    DValNodePtr pdvnBig, pdvnSmall, pdvnThis; 
 

   	
    if (! GetArgs("DVNcode 1.0",NUMARGS, myargs))
	 	return 1;
    
    printf("starting small\n");     
    pdvnSmall = DValNodeCopyStr(NULL, 1, "Small list Head");
      printf("%s\n", (CharPtr)(pdvnSmall->data.ptrvalue));
    pdvnSmall = DValNodeFreeData(pdvnSmall, NULL);
    printf("small DValNode done starting big\n ");  
    pdvnSmall = DValNodeCopyStr(NULL, 1, "Small list Head");
    pdvnBig = DValNodeCopyStr(NULL,1, "Big list head node 1");
    for (i=2;i<=100;i+=2)
    {
	buf[0]= NULL;
	sprintf(buf, "even List node %d", i); 
	pdvnThis = DValNodeCopyStr(&pdvnBig, 2, buf);
	buf[0] = NULL;
	sprintf(buf, "odd List node %d", i+1);
	pdvnThis = DValNodeCopyStr(&pdvnBig,  1, buf);
    }
    
    
    WalkList(pdvnBig); 

    pdvnThis = DValNodeUnlink(pdvnSmall, pdvnSmall);
    if (pdvnThis == NULL) printf("Unlink returns Null\n");
    pdvnThis = pdvnSmall;    
    pdvnBig = DValNodeLink(&pdvnBig, pdvnThis);
 WalkList(pdvnBig); 
 
  pdvnBig = DValNodeListDelNode(pdvnBig,  pdvnThis,  NULL);
    pdvnSmall = NULL;
  

  WalkList(pdvnBig); 
    pdvnSmall = DValNodeExtractList(&pdvnBig, 1);
 WalkList(pdvnBig); 
 WalkList(pdvnSmall);
    printf("length of Big %d\n", DVNodeListLen(pdvnBig));
    printf("length of Small %d\n", DVNodeListLen(pdvnSmall));
    DValNodeListCat(pdvnBig, pdvnSmall);
  WalkList(pdvnBig); 
    printf("length of Big %d\n", DVNodeListLen(pdvnBig));
    pdvnThis = pdvnSmall;
    pdvnSmall = DValNodeListCut(pdvnBig, pdvnThis);
    printf("length of Big %d\n", DVNodeListLen(pdvnBig));
    printf("length of Small %d\n", DVNodeListLen(pdvnSmall));
  WalkList(pdvnBig); 
  WalkList(pdvnSmall);
    pdvnSmall = DValNodeListDelNode(pdvnSmall,  pdvnSmall->next->next,  NULL);  
    printf("length of Small %d\n", DVNodeListLen(pdvnSmall));
  WalkList(pdvnSmall);
 WalkList(pdvnBig); 
    pdvnThis = DValNodeCopyStr(NULL,  1,  "added node");
  WalkList(pdvnSmall);
    printf("pdvnThis is  %s\n", (CharPtr) (pdvnThis->data.ptrvalue));
    pdvnSmall = DValNodeHeadLink(pdvnSmall,  pdvnThis);
    printf("length of Small %d\n", DVNodeListLen(pdvnSmall));
  WalkList(pdvnSmall); 
    pdvnThis = pdvnBig->next->next->next->next;
    DValNodeListInsert(pdvnThis, pdvnSmall);
    WalkList(pdvnBig);
    pdvnThis = NULL;
    pdvnThis = DValNodeFindNext(pdvnBig, NULL, 1);
    if (pdvnThis)
      printf("pdvnThis is  %s\n", (CharPtr) (pdvnThis->data.ptrvalue));
    pdvnBig = DValNodeFreeData(pdvnBig, NULL);
    printf("done\n");
    mys = (CharPtr) MemNew((size_t)(100*sizeof(Char)));
    strcpy(mys, "adios amiogs, MemNew still functions..\n");
    printf(mys);
    MemFree(mys);
    printf("and MemFree is OK\n");
    return 0;
}	    
	
*/
