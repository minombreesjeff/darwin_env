#ifdef NOWAY_ABOUT_TO_BE_ZOMBIED
/*      
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
* File Name:    ni_list.c
*
* Author:       Beatty, Gish
*
* Version Creation Date:        1/1/92
*
* $Revision: 6.0 $
*
* File Description: 
*   List and ring management functions.
*
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
* 4/23/92  Epstein     Added extensive in-line commentary, and removed all tabs
* 5/11/92  Epstein     Changed ListSwapAdj() to provide more rigorous testing
*                      that its two arguments are adjacent in the list.
* 5/14/92  Epstein     Added ListStrCopy() and ListStrDel()
* 7/06/92  Epstein     Fixed bug in ListStrCopy(), where the newly created
*                      list was not being returned to the caller ... whoops.
*
* ==========================================================================
*
*
* RCS Modification History:
* $Log: ni_list.c,v $
* Revision 6.0  1997/08/25 18:38:51  madden
* Revision changed to 6.0
*
* Revision 5.1  1997/01/28 21:23:42  epstein
* prepare to be zombied; functionality has been moved to ncbimisc.[ch]
*
 * Revision 5.0  1996/05/28  14:11:55  ostell
 * Set to revision 5.0
 *
 * Revision 4.0  1995/07/26  13:56:32  ostell
 * force revision to 4.0
 *
 * Revision 1.2  1995/05/17  17:52:27  epstein
 * add RCS log revision history
 *
*/

#include "ni_list.h"



/*
 * Purpose:     Insert an item as the next element in a doubly linked list(ring)
 *
 * Parameters:
 *   elem           Next element to be inserted; this is data only,not a NodePtr
 *   ap             Insertion point
 *
 * Returns:
 *                The newly allocated NodePtr, containing forward and backward
 *                pointers and a pointer to elem
 *
 *
 * Description:
 *              Allocate the necessary memory for a "Node", attach the
 *              caller's data to that Node, and insert the Node after the
 *              specified node in the list, maintaining the integrity of
 *              a doubly-linked ring. If there are no other items in the
 *              ring, create a "minimal" ring which consists of the single
 *              Node pointing to itself in both directions.
 *
 * Note:
 *              Most "list" data is actually stored in a doubly-linked ring, as
 *              shown below. Furthermore, note that each node only contains a
 *              pointer to the actual data in the list, rather than the actual
 *              data itself.
 *
 *          +------------------------------------------------------------------+
 *          ^                                                                  |
 *          |       +-------------------------------------------------------+  |
 *          |       |                                                       ^  |
 *          |       V                                                       |  |
 *          |   +-------+       +-------+                       +-------+   |  |
 *          |   | next  |------>| next  |------> ...    ------->| next  |-->+  |
 *          |   +-------+       +-------+                       +-------+      |
 *          +<--| last  |<------| last  |<------ ...    <-------| last  |<-----+
 *              +-------+       +-------+                       +-------+
 *              | elem  |       | elem  |                       | elem  |
 *              +-------+       +-------+                       +-------+
 *                  |               |                               |
 *                  |               |                               |
 *                  V               V                               V
 *              +-------+       +-------+                       +-------+
 *              | actual|       | actual|                       | actual|
 *              | data  |       | data  |                       | data  |
 *              +-------+       +-------+                       +-------+
 */

NodePtr 
ListInsert(VoidPtr elem, NodePtr ap)                     /* ptr to node to insert after */
{
    NodePtr             np;
    
    if (elem == NULL)
        return NULL;
    
    np = (NodePtr) MemNew(sizeof(Node));
    np->elem = elem;
    
    if (ap == NULL) {           /* no nodes in list */
        np->last = np;
        np->next = np;
        return np;
    }
    else {                              /* 1 or more nodes in list */
        np->next = ap->next;
        ap->next = np;
        np->next->last = np;
        np->last = ap;
        return np;
    }
} /* ListInsert */



/*
 * Purpose:     Insert an item as the previous element in a doubly linked
 *              list(ring)
 *
 * Parameters:
 *   elem           Next element to be inserted; this is data only,not a NodePtr
 *   ap             Insertion point
 *
 * Returns:
 *                The newly allocated NodePtr, containing forward and backward
 *                pointers and a pointer to elem
 *
 *
 * Description:
 *              Insert the specified item into the ring, before the specified
 *              insertion point. In the case where the specified insertion
 *              point was NULL, this is equivalent to ListInsert().
 */

NodePtr 
ListInsertPrev(VoidPtr elem, NodePtr ap)                     /* ptr to node to insert before */
{
    NodePtr             np;
    
    np = ap;
    if (ap != NULL)
        ap = ap->last;          /* previous node */
    
    ap = ListInsert(elem, ap);
    return (np == NULL) ? ap : np;
} /* ListInsertPrev */



/*
 * Purpose:     Delete a single node from a list or ring
 *
 * Parameters:
 *   np             Node to be deleted
 *
 * Returns:
 *                A pointer to the "next" node in the list/ring, after the
 *                deleted node.
 *
 *
 * Description:
 *              Delete the specified node from a list or ring. It is the
 *              responsibility of the caller to free the memory associated
 *              with the "elem" (data), if appropriate.
 */

NodePtr 
ListDelete(NodePtr np)
{
    NodePtr             nextnode, lastnode;
    
    if (np == NULL)
        return NULL;
    
    nextnode = np->next;
    lastnode = np->last;
    
    if (nextnode == NULL && lastnode == NULL)   /* only node in a list */
        ;
    else if (nextnode == NULL) {                /* last in a list */
        np->last->next = NULL;
        nextnode = np->last;
    }
    else if (lastnode == NULL) {                /* first in a list */
        np->next->last = NULL;
        nextnode = np->next;
    }
    else if (np == nextnode)                    /* last in a ring */
        nextnode = NULL;
    else {                                      /* node with both neighbors */
        np->last->next = nextnode;
        np->next->last = np->last;
    }
    
    MemFree(np);                        /* assumes element memory has been freed */
    return nextnode;
} /* ListDelete */



/*
 * Purpose:     Get the next element from a list or ring (non-destructively)
 *
 * Parameters:
 *   np             Node before the node to be selected
 *
 * Returns:
 *                A pointer to the "next" node in the list/ring (or NULL
 *                if the list/ring was NULL). Note that for a list, the
 *                returned value can also be NULL.
 *
 *
 * Description:
 *              Return the "next" node in the list or rin.g
 */

NodePtr 
ListGetNext(NodePtr np)
{
    if (np == NULL)
        return NULL;
    return np->next;
} /* ListGetNext */



/*
 * Purpose:     Swap two adjacent nodes in a list or ring
 *
 * Parameters:
 *   np1            "Prior" node
 *   np2            "Next" node
 *
 *
 * Description:
 *              Swap the two specified elements, provided that they are
 *              adjacent, and np1 precedes np2.
 */

void 
ListSwapAdj(NodePtr np1, NodePtr np2)       /* priornode, nextnode */
{
    if (np1 == NULL || np2 == NULL || np1->next->last != np1) /* must be sane */
        return;

    if (np1->next != np2 || np2->last != np1)             /* must be in order */
        return;
    
    if (np1->last != NULL)
        np1->last->next = np2;
    
    if (np2->next != NULL)
        np2->next->last = np1;
    
    np1->next = np2->next;
    np2->last = np1->last;
    
    np1->last = np2;
    np2->next = np1;
} /* ListSwapAdj */



/*
 * Purpose:     Sort the specified ring/list
 *
 * Parameters:
 *   head           Head of the list to be sorted
 *   cmpfunc        Comparison function (return values are like memcmp())
 *   order          ASCEND or DESCEND
 *
 * Returns:
 *              A pointer to the first element of the sorted ring or list
 *
 *
 * Description:
 *              Sort the specified list, in place, using bubble sort, and
 *              the specified comparison function. Determine prior to sorting
 *              whether this is a list or a ring. If it's a ring, break the
 *              ring prior to sorting, and restore it to a ring topology
 *              after sorting has been completed.
 */

NodePtr 
ListSort(NodePtr head, int (*cmpfunc )PROTO ((NodePtr, NodePtr )), int order)                                  /* 0 if equal, LT 0 if 1st element > 2nd element */
{
    NodePtr             np;
    Boolean             sorted = FALSE, ring;
    int         result;
    
    if (head == NULL)
        return NULL;
    if (head->last == NULL)
        ring = FALSE;
    else
        ring = TRUE;
    if (ring)
        ListBreakRing(head);
    
    /* just bubble sort for now */
    
    while (! sorted) {
        np = head;
        sorted = TRUE;
        
        while (np->next != NULL) {
            result = (*cmpfunc)(np, np->next);
            if ((result > 0 && order == ASCEND) || (result < 0 && order == DESCEND)) {
                sorted = FALSE;
                if (np == head)
                    head = np->next;    /* keep head pointing at 1st element */
                ListSwapAdj(np, np->next);
            }
            else
                np = np->next;
        }
    }
    
    if (ring)
        ListConnectRing(head);
    return head;        /* ptr to first element */
} /* ListSort */



/*
 * Purpose:     Break the specified ring into a non-circular (linear) list
 *
 * Parameters:
 *   np             Head of the ring to be broken
 *
 *
 * Description:
 *              Break the specified ring between its head and tail.
 *
 * Note:
 *              This function may be called safely (without effect) if the
 *              passed parameter is already a list, rather than a ring.
 */

void 
ListBreakRing(NodePtr np)
{
    if (np == NULL)
        return;
    if (np->last == NULL)
        return;
    
    np->last->next = NULL;
    np->last = NULL;
} /* ListBreakRing */



/*
 * Purpose:     Convert a list into a ring.
 *
 * Parameters:
 *   head           Head of the list to be connected
 *
 *
 * Description:
 *              Connect the specified list between its head and tail, producing
 *              a ring.
 *
 * Note:
 *              This function may be called safely (without effect) if the
 *              passed parameter is already a ring, rather than a list.
 */

void 
ListConnectRing(NodePtr head)
{
    NodePtr     np;
    
    if (head == NULL)
        return;
    
    np = head;
    
    while (np->next != NULL) {
        np = np->next;
        if (np == head)
            return;
    }
    
    np->next = head;
    head->last = np;
} /* ListConnectRing */


/*
 * Purpose:     Copy a list where the list elements are character strings
 *
 * Parameters:
 *   strlist        List to be copied
 *
 * Returns:
 *              A copy of the original list (which may be NULL)
 *
 *
 * Description:
 *              Create a list which is a copy of the original list, and
 *              also make copies of the strings.
 *
 * Note:
 *              There is no obvious way to make a generic list copying
 *              routine, because, in general, the length of each list
 *              element is unknown. This is a simple case where it is
 *              easy to copy a list.
 */

NodePtr
ListStrCopy (NodePtr strlist)
{
    NodePtr newlist = NULL;
    NodePtr np = strlist;
    CharPtr stringtext;

    if (strlist == NULL)
        return NULL;

    do {
        stringtext = StringSave((CharPtr) np->elem);
        newlist = ListInsert(stringtext, newlist);
        np = ListGetNext(np);
    } while (np != NULL && np != strlist);

    return newlist->next; /* points to 1st element in new list */
}


/*
 * Purpose:     Delete a list where the list elements are character strings
 *
 * Parameters:
 *   np             List to be deleted
 *
 *
 * Description:
 *              Delete the list nodes and the character string data associated
 *              with each node.
 *
 * Note:
 *              This routine will work for any list element which is a single
 *              block of memory. However, it will not work in the more general
 *              case where a list element in turn references other memory
 *              which must also be freed.
 */

void
ListStrDel (NodePtr np)
{
    while (np != NULL)
    {
        MemFree (np->elem);
        np = ListDelete(np);
    }
}
#endif /* NOWAY_ABOUT_TO_BE_ZOMBIED */
