static char const rcsid[] = "$Id: slist.c,v 6.2 2003/05/30 17:25:38 coulouri Exp $";

/*  slist.c
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
* File Name:  slist.c
*
* Author:  Kun-Mao Chao and Jinghui Zhang
*
* Version Creation Date: 5/24/95
*
*
* File Description:  
*
* Example of Skip List source code for C:

* Skip Lists are a probabilistic alternative to balanced trees, as
* described in the June 1990 issue of CACM and were invented by 
* William Pugh in 1987. 
* 
* This file contains source code to implement a dictionary using 
* skip lists and a test driver to test the routines.
* 
* A couple of comments about this implementation:
* The routine randomLevel has been hard-coded to generate random
* levels using p=0.25. It can be easily changed.
  
* The insertion routine has been implemented so as to use the
* dirty hack described in the CACM paper: if a random level is
* generated that is more than the current maximum level, the
* current maximum level plus one is used instead.
* 
* Levels start at zero and go up to MaxLevel (which is equal to
* 	(MaxNumberOfLevels-1).
* 
* The compile flag allowDuplicates determines whether or not duplicates
* are allowed. If defined, duplicates are allowed and act in a FIFO manner.
* If not defined, an insertion of a value already in the file updates the
* previously existing binding.
* 
* BitsInRandom is defined to be the number of bits returned by a call to
* random(). For most all machines with 32-bit integers, this is 31 bits
* as currently set. 


* Modifications:
* --------------------------------------------------------------------------
* Date     Name        Description of modification
*
* $Log: slist.c,v $
* Revision 6.2  2003/05/30 17:25:38  coulouri
* add rcsid
*
* Revision 6.1  1998/08/24 21:19:39  kans
* fixed -v -fd warnings
*
* Revision 6.0  1997/08/25 18:54:42  madden
* Revision changed to 6.0
*
* Revision 5.0  1996/05/28 13:43:15  ostell
* Set to revision 5.0
*
 * Revision 4.1  1996/05/21  13:43:39  epstein
 * change function-name 'delete' because it conflicts with VMS
 *
 * Revision 4.0  1995/07/26  13:50:15  ostell
 * force revision to 4.0
 *
 * Revision 1.2  1995/05/25  20:22:28  zjing
 * add header
 *
*
*
* ==========================================================================
*/


#ifndef _LIST_
#include <list.h>
#endif

node NIL1;

Int4 randomsLeft;
Int4 randomBits;

/* init - defines NIL1 and initializes the random bit source */ 
void init(void) { 

  NIL1 = newNodeOfLevel(0);
  NIL1->key = 0x7fffffff;
  NIL1->value = NULL;
  NIL1->forward[0] = NIL1;
  randomBits = rand();
  randomsLeft = BitsInRandom/2;
}

/* list_NIL_free - free NIL1 1/27/94 */
void list_NIL_free(void) {
   MemFree(NIL1);
}

/* newList - create a new list */
list newList(void) {
  list l;
  Int4 i;

  l = (list)ckalloc(sizeof(struct listStructure));
  l->level = 0;
  l->header = newNodeOfLevel(MaxNumberOfLevels);
  l->header->key = -1;
  l->header->value = NULL;
  for(i=0;i<MaxNumberOfLevels;i++) l->header->forward[i] = NIL1;
  return(l);
  } 

/* unewList - create a new list */
ulist unewList(void) {
  ulist l;

  l = (ulist)ckalloc(sizeof(struct ulistStructure));
  l->header = (unode)ckalloc(sizeof(struct unodeStructure)); 
  l->header->key = -1;
  l->header->link = NULL;
  return(l);
  } 

/* reset_pos - reset cur_p */
void reset_pos(list l)
{
	register Int4 k;
	register node p;

	p = l->header;
	for (k = 0; k <= l->level; ++k)   l->update[k] = p;
}

/* sreset_pos - reset l->update[0] */
void sreset_pos(list l)
{
	l->update[0] = l->header;
}

/* freeList - free the space of the list */
void freeList(list l) 
  {
  register node p,q;
  register Int4 i=0;
  void decre(fragment PNTR);

  p = l->header;
  do {
    ++i;
    q = p->forward[0];
    decre(p->value);
    MemFree(p);
    p = q; }
    while (p!=NIL1);
  MemFree(l);
  }

/* Feb. 1994*/
/* ufreeList - free the space of the ulist */
void ufreeList(ulist l) 
  {
  register unode p,q;
  register Int4 i=0;

  p = l->header;
  do {
    ++i;
    q = p->link;
    MemFree(p);
    p = q; }
    while (p!=NULL);
  MemFree(l);
  }

/* randomLevel - return a random level */
Int4 randomLevel(Int4 maxl)
  {register Int4 level = 0;
   register Int4 b;
   do {
    b = randomBits&3;
    if (!b) level++;
    randomBits>>=2;
    if (--randomsLeft == 0) {
        randomBits = rand();
        randomsLeft = BitsInRandom/2;
        };
    } while (!b);
    return(level>maxl ? maxl : level);
    }

/* uinsert - insert a new node to the ulist */
void uinsert(ulist l, keyType key) 
  {
  register unode p,q;

	for (p=l->header,q=p->link; q && q->key<key; p=q, q=p->link) continue;
	q = (unode)ckalloc(sizeof(struct unodeStructure));
	q->key = key;
	q->link = p->link;
	p->link = q;
    }

/* delete_by_key - delete a node from the list by key */
Int4 delete_by_key(list l, keyType key) 
  {
  register Int4 k,m;
  register node p,q;

  p = l->header;
  k = m = l->level;
  do {
	while (q = p->forward[k], q->key < key) p = q;
	l->update[k] = p;
	} while(--k>=0);

  if (q->key == key) {
	for(k=0; k<=m && (p=l->update[k])->forward[k] == q; k++) 
	  p->forward[k] = q->forward[k];
	MemFree(q);
        while( l->header->forward[m] == NIL1 && m > 0 )
	     m--;
	l->level = m;
	return(true);
	}
    else return(false);
    }

/* usearch - search a node in the ulist by key */
Boolean usearch(ulist l, keyType key) 
  {
  register unode p;

  if (!l) return(false);
  for (p=l->header; p && p->key < key; p=p->link) continue;
  if (p && p->key == key) return(true);
  else return(false);
    }

/* rsearch - search a node in the list by key */
/*	     find p s.t. p->key < key <= (p->forward[0])->key */
valueType rsearch(list l, keyType key)
  {
  register Int4 k;
  register node p,q;
  p = l->header;
  k = l->level;
  do {
	while (q = p->forward[k], q->key < key) p = q;
	l->update[k] = p;
	} while(--k>=0);
  return(p->value);
  }

/* rsearch_next - search from cur_p */
/*	     find p s.t. p->key < key <= (p->forward[0])->key */
valueType rsearch_next(list l, keyType key)
  {
  register Int4 k;
  register node p,q;
  k = l->level;
  p = l->update[k];
  do {
	while (q = p->forward[k] ,  q->key < key) p = q;
	l->update[k] = p;
	} while(--k>=0);
  return(p->value);
  }

/* cur_key - return the key of the node pointed by the pointer cur_p */
keyType cur_key(list l)
{
	return((l->update[0])->key);
}

/* update_node - update the value of the node pointed by cur_p */
void update_node(list l, valueType value)
{
	(l->update[0])->value = value;
}

/* next_key - return the key of the node next to the node pointed by cur_p */
keyType next_key(list l)
{
	node p;

	p = (l->update[0])->forward[0];
	if (p != NIL1) return(p->key);
	else return(-1);
}

/* next_val - return the value of the node next to the node pointed by cur_p */
valueType next_val(list l)
{
	node p;

	p = (l->update[0])->forward[0];
	if (p != NIL1) return(p->value);
	else return(NULL);
}

/* next - move l->cur_p right */
void next(list l)
{
	node p, q;
	Int4 k;

	p = l->update[0];
	q = p->forward[0];
	if (q == NIL1) return;
	else {
		for (k = 0; k <= l->level && (((l->update[k])->forward[k])
			== q); k++)   l->update[k] = q;
	}
}

/* snext - move l->update[0] right */
valueType snext(list l)
{
	node p, q;

	p = l->update[0];
	q = p->forward[0];
	l->update[0] = q;
	return(q->value);
}

/*delete_next - delete the node next to the node pointed by cur_p */
void delete_next(list l)
{
	register Int4 k,m;
	register node p,q;

	m = l->level;
	q = (l->update[0])->forward[0];
	if (q == NIL1) return;
	for (k=0; k<=m && (p=l->update[k])->forward[k] == q; k++)
		p->forward[k] = q->forward[k];
	MemFree(q);
	while (l->header->forward[m] == NIL1 && m > 0)  m--;
	l->level = m;
}

/* insert_next - insert a new node next to the node pointed by cur_p */
void insert_next(list l, keyType key, valueType value)
{
	register Int4 k;
	register node p,q;

	k = randomLevel(MaxLevel);
	if (k>l->level) {
		k = ++l->level;
		l->update[k] = l->header;
	}
	q = newNodeOfLevel(k);
	q->key = key;
	q->value = value;
	do {
		p = l->update[k];
		q->forward[k] = p->forward[k];
		p->forward[k] = q;
		l->update[k] = q;
	} while (--k >= 0);
}


