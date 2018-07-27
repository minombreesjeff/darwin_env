/*   $Id: binary.h,v 1.5 1999/11/23 22:02:30 vakatov Exp $
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
* File Name:  $Id: binary.h,v 1.5 1999/11/23 22:02:30 vakatov Exp $
*
* Author:  Lewis Geer
*
* Version Creation Date:   8/24/99
*
* $Revision: 1.5 $
*
* File Description: Various binary search algorithms
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: binary.h,v $
* Revision 1.5  1999/11/23 22:02:30  vakatov
* Fixed for the MSVC DLL compilation
*
* Revision 1.4  1999/11/05 21:39:59  vakatov
* Get rid of a nested comment in "struct _B_Global"
*
* Revision 1.3  1999/10/14 22:00:33  vakatov
* More changes in the included headers
*
* Revision 1.2  1999/10/14 19:08:09  kans
* header changes for Mac
*
* Revision 1.1  1999/09/21 18:09:14  lewisg
* binary search added to color manager, various bug fixes, etc.
* ==========================================================================
*/


#ifndef BINARY_H
#define BINARY_H

#include <ncbi.h>


#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* 
typedef for the key comparison function.    
< 0 if Key1 < Key2, == 0 if Key1 == Key2, > 0 if Key1 > Key2   
*/
typedef Int4 (*B_CompareFunc)(void *Key1, void *Key2);

/* 
B_List used in B_Global to hold a List of keys and bags
*/
typedef struct _B_List {
    void * Key;
    void * Bag;
} B_List;

/* 
typedef for the global for binary searching
*/
typedef struct _B_Global {
    B_CompareFunc CompareFunc;  /* the function used to compare keys */
    Int4 AllocSize;  /* the size of memory block to allocate/deallocate */
    B_List *List;  /* the list of keys and bags */
    Int4 Size; /* the length of the list (NOT in bytes) */
    Int4 TrueSize;  /* The true size of the list (NOT in bytes */
    Int4 Cursor;  /* keeps track of the last search result */
#if 0
    TNlmRWlock RW;  /* reader/writer lock */
#endif
} B_Global;

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

#define B_ALLOCSIZE 64

NLM_EXTERN B_Global * B_NewGlobal(B_CompareFunc CompareFunc, Int4 AllocSize);

NLM_EXTERN Int4 B_DeleteGlobal(B_Global *pGlobal);


/*****************************************************************************
*
*   Inserts a new entry into the List.  Returns the position.  The position
*   is not guaranteed between deletions and insertions
*
*   Automatically reallocates memory for List as List grows
*
*****************************************************************************/

NLM_EXTERN Int4 B_Insert(B_Global *pGlobal, void *Key, void *Bag);


/*****************************************************************************
*
*   B_Get() Returns the position in the List associated with the key.
*   Returns INT4_MIN if no match.
*
*   B_GetFirst() does the same, but returns the first item in the List that
*   matches the Key.
*
*   B_GetBag() gets bag associated with the key.
*
*****************************************************************************/

NLM_EXTERN Int4 B_Get(B_Global *pGlobal, void *Key, Boolean *NotFound);

NLM_EXTERN Int4 B_GetFirst(B_Global *pGlobal, void *Key, Boolean *NotFound);

NLM_EXTERN void * B_GetBag(B_Global *pGlobal, void *Key);


/*****************************************************************************
*
*   B_Get() is a macro that returns the number of entries in the list
*   pGlobal is a pointer to the B_Global.
*
*****************************************************************************/

#define B_GetSize(pGlobal) ((pGlobal)->Size)

/*****************************************************************************
*
*   B_GetList() is a macro that returns entry at a list position
*   pGlobal is a pointer to the B_Global and 
*
*****************************************************************************/

#define B_GetList(pGlobal, Position) ((pGlobal)->List[(Position)])


#ifdef __cplusplus
}
#endif

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

#endif /* BINARY_H */
