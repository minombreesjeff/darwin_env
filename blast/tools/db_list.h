
/*  db_list.h
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
* File Name:  db_list.h
*
* Author:  Jinghui Zhang and Kun-Mao Chao
*
* Version Creation Date: 5/25/95
*
* $Revision: 6.0 $
*
* File Description:  functions used in the sim2 algorithm
*
* Modifications:
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* $Log: db_list.h,v $
* Revision 6.0  1997/08/25 18:53:00  madden
* Revision changed to 6.0
*
* Revision 5.0  1996/05/28 13:43:15  ostell
* Set to revision 5.0
*
 * Revision 4.0  1995/07/26  13:50:15  ostell
 * force revision to 4.0
 *
 * Revision 1.2  1995/05/25  20:16:00  zjing
 * list.h
 *
*
*
* ==========================================================================
*/

#ifndef _DB_LIST_
#define _DB_LIST_

#include <ncbi.h>

#ifdef __cplusplus
extern "C" {
#endif



#define false 0
#define true 1
#define BitsInRandom 31

#define allowDuplicates

#define MaxNumberOfLevels 16
#define IMaxNumberOfLevels 4
#define MaxLevel (MaxNumberOfLevels-1)
#define IMaxLevel (IMaxNumberOfLevels-1)
#define db_newNodeOfLevel(l) (db_node)ckalloc(sizeof(struct db_nodeStructure)+(l)*sizeof(db_node *))

typedef struct db_nodeStructure PNTR db_node;
typedef struct db_nodeStructure {
	keyType key;
	valueType value;
	db_node back;
	db_node forward[1];
} db_nodeStructure;
typedef struct db_listStructure {
	Int4 level;
	db_node update[MaxNumberOfLevels];
	db_node header;
} db_listStructure;
typedef db_listStructure PNTR db_list;
typedef struct idb_listStructure {
	Int4 level;
	db_node update;
	db_node header;
} idb_listStructure;
typedef idb_listStructure PNTR idb_list;

extern void db_init(void);
extern void dblist_NIL_free(void);
extern db_list db_newList(void);
extern idb_list idb_newList(void);
extern void db_reset_pos(db_list);
extern void db_sreset_pos(db_list);
extern void idb_reset_pos(idb_list);
extern void db_freeList(db_list); 
extern void idb_freeList(idb_list); 
extern Int4 db_randomLevel(Int4);
extern void db_insert(idb_list, keyType, valueType); 
extern void db_delete(idb_list, keyType); 
extern valueType db_rsearch(db_list, keyType);
extern valueType db_rsearch_next(db_list, keyType);
extern valueType db_lsearch(db_list, keyType);
extern valueType db_lsearch_next(db_list, keyType);
extern keyType db_cur_key(db_list);
extern void db_update_node(db_list, valueType);
extern keyType db_next_key(db_list);
extern valueType db_next_val(db_list);
extern keyType db_prev_key(db_list);
extern valueType db_prev_val(db_list);
extern keyType db_key_next(db_list);
extern keyType idb_key_next(idb_list);
extern valueType db_next(db_list);
extern valueType db_snext(db_list);
extern void db_delete_next(db_list);
extern void db_insert_next(db_list, keyType, valueType);
extern void db_print(db_list);


#ifdef __cplusplus
}
#endif

#endif

