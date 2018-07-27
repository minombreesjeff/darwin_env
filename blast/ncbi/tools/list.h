/*  list.h
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
* File Name:  list.h
*
* Author:  Jinghui Zhang and Kun-Mao Chao
*
* Version Creation Date: 5/25/95
*
* $Revision: 6.1 $
*
* File Description:  functions used in the sim2 algorithm
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: list.h,v $
* Revision 6.1  2000/07/08 20:44:11  vakatov
* Get all "#include" out of the 'extern "C" { }' scope;  other cleanup...
*
* Revision 6.0  1997/08/25 18:53:22  madden
* Revision changed to 6.0
*
* Revision 5.0  1996/05/28 13:43:15  ostell
* Set to revision 5.0
*
* Revision 4.1  1996/05/21  13:43:46  epstein
* change function-name 'delete' because it conflicts with VMS
* ==========================================================================
*/

#ifndef _LIST_
#define _LIST_

#include <ncbi.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct fragment {
	Int4 i, j, k;
	Int4 ref;
	Int4 score;
	struct fragment PNTR bgf;
	} fragment;

#define false 0
#define true 1
typedef Char boolean;
#define BitsInRandom 31

#define allowDuplicates

#define MaxNumberOfLevels 16
#define UMaxNumberOfLevels 2
#define MaxLevel (MaxNumberOfLevels-1)
#define UMaxLevel (UMaxNumberOfLevels-1)
#define newNodeOfLevel(l) (node)ckalloc(sizeof(struct nodeStructure)+(l)*sizeof(node *))

typedef Int4 keyType;
typedef fragment PNTR valueType;
typedef struct nodeStructure PNTR node;
typedef struct nodeStructure {
	keyType key;
	valueType value;
	node forward[1];
} nodeStructure;
typedef struct listStructure {
	Int4 level;
	node update[MaxNumberOfLevels];
	node header;
} listStructure;
typedef listStructure PNTR list;
typedef struct unodeStructure PNTR unode;
typedef struct unodeStructure {
	Int4 key;
	unode link;
} unodeStructure;
typedef struct ulistStructure {
	unode header;
} ulistStructure;
typedef ulistStructure PNTR ulist;


extern void init(void);
extern void list_NIL_free(void);
extern list newList(void);
extern ulist unewList(void);
extern void reset_pos(list);
extern void sreset_pos(list);
extern void freeList(list); 
extern void ufreeList(ulist); 
extern Int4 randomLevel(Int4);
extern void uinsert(ulist, keyType); 
extern Int4 delete_by_key(list, keyType); 
extern Boolean usearch(ulist, keyType); 
extern valueType rsearch(list, keyType);
extern valueType rsearch_next(list, keyType);
extern keyType cur_key(list);
extern void update_node(list, valueType);
extern keyType next_key(list);
extern valueType next_val(list);
extern void next(list);
extern valueType snext(list);
extern void delete_next(list);
extern void insert_next(list, keyType, valueType);

extern void active_print(list,Int4, Int4, Int4, Int4);
extern void fatal(CharPtr);
extern void fatalf(CharPtr, CharPtr);
extern FILE *ckopen(CharPtr, CharPtr);
extern CharPtr ckalloc(Int4);


#ifdef __cplusplus
}
#endif

#endif

