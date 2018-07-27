static char const rcsid[] = "$Id: db_slist.c,v 6.2 2003/05/30 17:25:36 coulouri Exp $";

/*  db_slist.c
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
* File Name:  db_slist.c
*
* Author:  Kun-Mao Chao and Jinghui Zhang
*
* Version Creation Date: 5/24/95
*
*
* File Description:  
* Example of Skip List source code for C:

* Skip Lists are a probabilistic alternative to balanced trees, as
* described in the June 1990 issue of CACM and were invented by 
* William Pugh in 1987. 

* This file contains source code to implement a dictionary using 
* skip lists and a test driver to test the routines.

* A couple of comments about this implementation:
* The routine randomLevel has been hard-coded to generate random
* levels using p=0.25. It can be easily changed.
*  
* The insertion routine has been implemented so as to use the
* dirty hack described in the CACM paper: if a random level is
* generated that is more than the current maximum level, the
* current maximum level plus one is used instead.

* Levels start at zero and go up to MaxLevel (which is equal to
* 	(MaxNumberOfLevels-1).

* The compile flag allowDuplicates determines whether or not duplicates
* are allowed. If defined, duplicates are allowed and act in a FIFO manner.
* If not defined, an insertion of a value already in the file updates the
* previously existing binding.
* 
* BitsInRandom is defined to be the number of bits returned by a call to
* random(). For most all machines with 32-bit integers, this is 31 bits
* as currently set. 

*
* Modifications:
* --------------------------------------------------------------------------
* Date     Name        Description of modification
*
* $Log: db_slist.c,v $
* Revision 6.2  2003/05/30 17:25:36  coulouri
* add rcsid
*
* Revision 6.1  1998/08/24 21:19:37  kans
* fixed -v -fd warnings
*
* Revision 6.0  1997/08/25 18:53:01  madden
* Revision changed to 6.0
*
* Revision 5.0  1996/05/28 13:43:15  ostell
* Set to revision 5.0
*
 * Revision 4.0  1995/07/26  13:50:15  ostell
 * force revision to 4.0
 *
 * Revision 1.2  1995/05/25  20:15:32  zjing
 * falign.c
 *
*
*
* ==========================================================================
*/


#ifndef _LIST_
#include <list.h>
#endif

#ifndef _DB_LIST_
#include <db_list.h>
#endif

db_node NIL;

extern Int4 randomsLeft;
extern Int4 randomBits;

/* db_init - defines NIL and initializes the random bit source */ 
void db_init(void) { 
  
  NIL = db_newNodeOfLevel(0);
  NIL->key = 0x7fffffff;
  NIL->value = NULL;
  NIL->forward[0] = NIL;
  randomBits = rand();
  randomsLeft = BitsInRandom/2;
}

/* dblist_NIL_free - free NIL 1/27/94 */
void dblist_NIL_free(void)
{
   MemFree(NIL);
}

/* db_newList - create a new list */
db_list db_newList(void) {
  db_list l;
  Int4 i;

  l = (db_list)ckalloc(sizeof(struct db_listStructure));
  l->level = 0;
  l->header = db_newNodeOfLevel(MaxNumberOfLevels);
  l->header->key = -1;
  l->header->value = NULL;
  l->header->back = NULL;
  for(i=0;i<MaxNumberOfLevels;i++) l->header->forward[i] = NIL;
  return(l);
  }

/* idb_newList - create a new list */
idb_list idb_newList(void) {
  idb_list l;
  Int4 i;

  l = (idb_list)ckalloc(sizeof(struct idb_listStructure));
  l->level = 0;
  l->header = db_newNodeOfLevel(IMaxNumberOfLevels);
  l->header->key = -1;
  l->header->value = NULL;
  l->header->back = NULL;
  for(i=0;i<IMaxNumberOfLevels;i++) l->header->forward[i] = NIL;
  return(l);
  }

/* db_reset_pos - reset cur_p */
void db_reset_pos(db_list l)
{
	register Int4 k;
	register db_node p;

	p = l->header;
	for (k = 0; k <= l->level; ++k)   l->update[k] = p;
}

/* db_sreset_pos - reset l->update[0] */
void db_sreset_pos(db_list l)
{
	l->update[0] = l->header;
}

/* idb_reset_pos - reset cur_p */
void idb_reset_pos(idb_list l)
{
	l->update = l->header;
}

/* db_freeList - free the space of the list */
void db_freeList(db_list l) 
  {
  register db_node p,q;
  void decre(fragment PNTR);

  p = l->header;
  do {
    q = p->forward[0];
    decre(p->value);
    MemFree(p);
    p = q; }
    while (p!=NIL);
  MemFree(l);
  }

/* idb_freeList - free the space of the list */
void idb_freeList(idb_list l) 
  {
  register db_node p,q;
  p = l->header;
    while (p != NIL) {
    q = p->forward[0];
    MemFree(p);
    p = q; }
  MemFree(l);
  }

/* db_randomLevel - return a random level */
Int4 db_randomLevel(Int4 maxl)
  {register Int4 level = 0;
   register Int4 b;
   do {
    b = randomBits&3;
    if (!b) level++;
    randomBits>>=2;
    if (--randomsLeft == 0) {
        randomBits = rand();
        randomsLeft = BitsInRandom/2;
        }
    } while (!b);
    return(level>maxl ? maxl : level);
    }

/* db_insert - insert a new node to the list */
void db_insert(idb_list l, keyType key, valueType value) 
  {
  register Int4 k;
  register db_node p,q;
  db_node update[IMaxNumberOfLevels];

  if (!l) return;
  p = l->header;
  k = l->level;
  do {
	while (q = p->forward[k], q->key < key) p = q;
	update[k] = p;
	} while(--k>=0);


    k = db_randomLevel(IMaxLevel);
    if (k>l->level) {
	k = ++l->level;
	update[k] = l->header;
	}
    q = db_newNodeOfLevel(k);
    q->key = key;
    q->value = value;
    do {
	p = update[k];
	q->forward[k] = p->forward[k];
	p->forward[k] = q;
	} while(--k>=0);
    q->back = update[0];
    (q->forward[0])->back = q;
    return;
    }

/* db_delete - delete a node from the list by key */
void db_delete(idb_list l, keyType key) 
  {
  register Int4 k,m;
  register db_node p,q;
  db_node update[IMaxNumberOfLevels];

  if (!l) return;
  p = l->header;
  k = m = l->level;
  do {
	while (q = p->forward[k], q->key < key) p = q;
	update[k] = p;
	} while(--k>=0);

  for(k=0; k<=m && (p=update[k])->forward[k] == q; k++) 
    p->forward[k] = q->forward[k];
  (q->forward[0])->back = p;
  MemFree(q);
  while( l->header->forward[m] == NIL && m > 0 )
       m--;
  l->level = m;
  }

/* db_rsearch - search a node in the list by key */
/*	     find p s.t. p->key < key <= (p->forward[0])->key */
valueType db_rsearch(db_list l, keyType key)
  {
  register Int4 k;
  register db_node p,q;
  p = l->header;
  k = l->level;
  do {
	while (q = p->forward[k], q->key < key) p = q;
	l->update[k] = p;
	} while(--k>=0);
  return(p->value);
  }

/* db_rsearch_next - search from cur_p */
/*	     find p s.t. p->key < key <= (p->forward[0])->key */
valueType db_rsearch_next(db_list l, keyType key)
  {
  register Int4 k;
  register db_node p,q;
  k = l->level;
  p = l->update[k];
  do {
	while (q = p->forward[k] ,  q->key < key) p = q;
	l->update[k] = p;
	} while(--k>=0);
  return(p->value);
  }

/* db_lsearch - search a node in the list by key */
/*	     find p s.t. p->key <= key < (p->forward[0])->key */
valueType db_lsearch(db_list l, keyType key)
  {
  register Int4 k;
  register db_node p,q;
  p = l->header;
  k = l->level;
  do {
	while (q = p->forward[k], q->key <= key) p = q;
	l->update[k] = p;
	} while(--k>=0);
  return(p->value);
  }

/* db_lsearch_next - search from cur_p */
/*	     find p s.t. p->key <= key < (p->forward[0])->key */
valueType db_lsearch_next(db_list l, keyType key)
  {
  register Int4 k;
  register db_node p,q;
  k = l->level;
  p = l->update[k];
  do {
	while (q = p->forward[k] ,  q->key <= key) p = q;
	l->update[k] = p;
	} while(--k>=0);
  return(p->value);
  }

/* db_cur_key - return the key of the node pointed by the pointer cur_p */
keyType db_cur_key(db_list l)
{
	return((l->update[0])->key);
}

/* db_update_node - update the value of the node pointed by cur_p */
void db_update_node(db_list l, valueType value)
{
	(l->update[0])->value = value;
}

/* db_next_key - return the key of the node next to the node pointed by cur_p */
keyType db_next_key(db_list l)
{
	db_node p;

	p = (l->update[0])->forward[0];
	if (p != NIL) return(p->key);
	else return(-1);
}

/* db_next_val - return the value of the node next to the node pointed by cur_p */
valueType db_next_val(db_list l)
{
	db_node p;

	p = (l->update[0])->forward[0];
	if (p != NIL) return(p->value);
	else return(NULL);
}

/* db_prev_key - return the key of the node before the node pointed by cur_p */
keyType db_prev_key(db_list l)
{
	db_node p;

	p = (l->update[0])->back;
	if (p) return(p->key);
	else return(-1);
}

/* db_prev_val - return the value of the node before the node pointed by cur_p */
valueType db_prev_val(db_list l)
{
	db_node p;

	p = (l->update[0])->back;
	if (p) return(p->value);
	else return(NULL);
}

/* db_key_next - move l->cur_p right */
keyType db_key_next(db_list l)
{
	db_node p, q;
	Int4 k;

	p = l->update[0];
	q = p->forward[0];
	if (q == NIL) return (-1);
	else {
		for (k = 0; k <= l->level && (((l->update[k])->forward[k])
			== q); k++)   l->update[k] = q;
		return(q->key);
	}
}

/* idb_key_next - move l->cur_p right */
keyType idb_key_next(idb_list l)
{
	db_node p, q;

	p = l->update;
	q = p->forward[0];
	if (q == NIL) return (-1);
	else {
		l->update = q;
		return(q->key);
	}
}

/* db_next - move l->cur_p right */
valueType db_next(db_list l)
{
	db_node p, q;
	Int4 k;

	p = l->update[0];
	q = p->forward[0];
	if (q == NIL) return (NULL);
	else {
		for (k = 0; k <= l->level && (((l->update[k])->forward[k])
			== q); k++)   l->update[k] = q;
		return(q->value);
	}
}

/* db_snext - search sequencially */
valueType db_snext(db_list l)
{
	db_node p, q;

	p = l->update[0];
	q = p->forward[0];
	l->update[0]=q;
	return(q->value);
}

/*db_delete_next - delete the node next to the node pointed by cur_p */
void db_delete_next(db_list l)
{
	register Int4 k,m;
	register db_node p,q;

	m = l->level;
	q = (l->update[0])->forward[0];
	if (q == NIL) return;
	for (k=0; k<=m && (p=l->update[k])->forward[k] == q; k++)
		p->forward[k] = q->forward[k];
	(q->forward[0])->back = l->update[0];
	MemFree(q);
	while (l->header->forward[m] == NIL && m > 0)  m--;
	l->level = m;
}

/* db_insert_next - insert a new node next to the node pointed by cur_p */
void db_insert_next(db_list l, keyType key, valueType value)
{
	register Int4 k;
	register db_node p,q;

	k = db_randomLevel(MaxLevel);
	if (k>l->level) {
		k = ++l->level;
		l->update[k] = l->header;
	}
	q = db_newNodeOfLevel(k);
	q->key = key;
	q->value = value;
	do {
		p = l->update[k];
		q->forward[k] = p->forward[k];
		p->forward[k] = q;
		l->update[k] = q;
	} while (--k >= 0);
	q->back = p;
	(q->forward[0])->back = q;
}

