/*   $Id: binary.c,v 1.6 2000/08/28 18:47:40 vakatov Exp $
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
* File Name:  $Id: binary.c,v 1.6 2000/08/28 18:47:40 vakatov Exp $
*
* Author:  Lewis Geer
*
* Version Creation Date:   8/24/99
*
* $Revision: 1.6 $
*
* File Description: Various binary search algorithms 
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: binary.c,v $
* Revision 1.6  2000/08/28 18:47:40  vakatov
* Added missing type cast to pass C++ compilation
*
* Revision 1.5  1999/11/23 22:02:29  vakatov
* Fixed for the MSVC DLL compilation
*
* Revision 1.4  1999/10/14 22:00:33  vakatov
* More changes in the included headers
*
* Revision 1.3  1999/10/14 19:08:09  kans
* header changes for Mac
*
* Revision 1.2  1999/10/05 23:18:25  lewisg
* add ddv and udv to cn3d with memory management
*
* Revision 1.1  1999/09/21 18:09:13  lewisg
* binary search added to color manager, various bug fixes, etc.
* ==========================================================================
*/

#include <binary.h>


/*****************************************************************************
*
*   B_NewGlobal() returns a new binary search global.  Takes a key comparison
*   function and the size to realloc the binary search list.  If you pass 0
*   for AllocSize it will use the default value B_ALLOCSIZE * sizeof(B_List).  
*   Returns NULL on failure.
*
*   B_DeleteGlobal() deletes the global.  Return 1 on success, 0 on failure. 
*
*****************************************************************************/

NLM_EXTERN B_Global * B_NewGlobal(B_CompareFunc CompareFunc, Int4 AllocSize)
{
    B_Global * pGlobal;

    if (CompareFunc == NULL) return NULL;
    pGlobal = (B_Global *) MemNew(sizeof(B_Global));
    if(pGlobal == NULL) return NULL;

/*    pGlobal->RW = NlmRWinit(); */
    if(AllocSize == 0) pGlobal->AllocSize = B_ALLOCSIZE;
    else pGlobal->AllocSize = AllocSize;
    pGlobal->CompareFunc = CompareFunc;
    pGlobal->List = NULL;
    pGlobal->Size = 0;
    pGlobal->TrueSize = 0;
    pGlobal->Cursor = -1;

    return pGlobal;
}

NLM_EXTERN Int4 B_DeleteGlobal(B_Global *pGlobal)
{
    if(pGlobal == NULL) return 0;

    MemFree(pGlobal->List);
/*    NlmRWdestroy(pGlobal->RW); */
    MemFree(pGlobal);

    return 1;
}


/*****************************************************************************
*
*   Inserts a new entry into the List.  Returns the position.  The position
*   is not guaranteed between deletions and insertions
*
*   Automatically reallocates memory for List as List grows
*
*****************************************************************************/

NLM_EXTERN Int4 B_Insert(B_Global *pGlobal, void *Key, void *Bag)
{
    Int4 InsertPos;
    Boolean NotFound, InsertBefore = TRUE;

    if(pGlobal == NULL || Key == NULL) return 0;

    if(pGlobal->Size !=0) {
        InsertPos = B_GetFirst(pGlobal, Key, &NotFound);
        if(InsertPos == INT4_MIN) return 0;
        /* scan for insertion point */
        if(InsertPos == B_GetSize(pGlobal)) InsertBefore = FALSE;
        else if((*pGlobal->CompareFunc)(Key, pGlobal->List[InsertPos].Key) > 0)
            InsertBefore = FALSE;
    }
    else InsertPos = 0;

    /* make the List bigger if necessary */
    if(pGlobal->TrueSize == 0) {
        pGlobal->TrueSize += pGlobal->AllocSize;
        pGlobal->List = (B_List *)Nlm_MemNew((size_t)pGlobal->TrueSize * sizeof(B_List));
        if(pGlobal->List == NULL) return -1;
    }
    else if (pGlobal->Size == pGlobal->TrueSize) {
        pGlobal->TrueSize += pGlobal->AllocSize;
        pGlobal->List = (B_List *)Nlm_MemMore((void *)pGlobal->List,
            (size_t) pGlobal->TrueSize * sizeof(B_List));
        if(pGlobal->List == NULL) return -1;
    }
    
    if(InsertBefore) {
        Nlm_MemMove(&pGlobal->List[InsertPos + 1], &pGlobal->List[InsertPos],
            (size_t)(pGlobal->Size - InsertPos) * sizeof(B_List));
        
        pGlobal->List[InsertPos].Key = Key;
        pGlobal->List[InsertPos].Bag = Bag;
    }
    else if(InsertPos == B_GetSize(pGlobal)) {
        pGlobal->List[InsertPos].Key = Key;
        pGlobal->List[InsertPos].Bag = Bag;
    }
    else {
        Nlm_MemMove(&pGlobal->List[InsertPos + 1], &pGlobal->List[InsertPos],
            (size_t)(pGlobal->Size - InsertPos) * sizeof(B_List));
        
        pGlobal->List[InsertPos + 1].Key = Key;
        pGlobal->List[InsertPos + 1].Bag = Bag;
    }

    pGlobal->Size++;

    return InsertPos;
}

/*****************************************************************************
*
*   B_Get() Returns the position in the List associated with the key.
*   Returns INT4_MIN if no match or the list is empty.
*
*   B_GetFirst() does the same, but returns the first item in the List that
*   matches the Key.
*
*   B_GetBag() gets bag associated with the key.
*
*****************************************************************************/


NLM_EXTERN Int4 B_Get(B_Global *pGlobal, void *Key, Boolean *NotFound)
{
    Int4 x, l, r;
    Int4 CompareValue;

    if(pGlobal == NULL || Key == NULL) {
        *NotFound = TRUE;
        return INT4_MIN;
    }

    *NotFound = FALSE;

    if(pGlobal->Cursor >= 0)
        if ((*pGlobal->CompareFunc)(Key, pGlobal->List[pGlobal->Cursor].Key) == 0)
            return pGlobal->Cursor;

    if(pGlobal->Size == 0) {
        *NotFound = TRUE;
        return 0;
    }
 
    l = 0;
    r = B_GetSize(pGlobal) - 1;

    while (TRUE) {
        x = (l + r)/2;
        CompareValue = (*pGlobal->CompareFunc)(Key, pGlobal->List[x].Key);
        if (CompareValue == 0) {
            pGlobal->Cursor = x;
            return x;
        }
        if (r <= l) break;
        if (CompareValue < 0) {
            if (x > 0) r = x - 1;
            else r = 0;
        }
        else {
            if (x < B_GetSize(pGlobal) - 1) l = x + 1;
            else l = B_GetSize(pGlobal) - 1;
        }
    }
    *NotFound = TRUE;  /* not found */
    return x;
}


NLM_EXTERN Int4 B_GetFirst(B_Global *pGlobal, void *Key, Boolean *NotFound)
{
    Int4 i, j;

    i = B_Get(pGlobal, Key, NotFound);
    if(B_GetSize(pGlobal) < 2) return i;

    if(*NotFound) {    
        if( i == INT4_MIN) return i;
        for(j = -1; j < 2; j++) {
            if(j + i < 0 ) continue;
            if(j + i >= B_GetSize(pGlobal)) break;
            if((*pGlobal->CompareFunc)(Key, pGlobal->List[j+i].Key) < 0) break;
        }
        return j + i;
    }
    
    if(i == 0) return i;
    while((*pGlobal->CompareFunc)(pGlobal->List[i].Key, pGlobal->List[i-1].Key)
        == 0 && i > 1) i--;

    return i;
}

NLM_EXTERN void * B_GetBag(B_Global *pGlobal, void *Key)
{
    Int4 i;
    Boolean NotFound;

    i = B_Get(pGlobal, Key, &NotFound);
    if(NotFound) return NULL;
    else return pGlobal->List[i].Bag;
}


