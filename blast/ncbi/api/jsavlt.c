#include <jsavlt.h>

/*****************************************************************************
 
**************** START OF STATIC FUNCTIONS (Vixie & Wirth) ******************
  
*****************************************************************************/

/* as_tree - tree library for as
 * vix 14dec85 [written]
 * vix 02feb86 [added tree balancing from wirth "a+ds=p" p. 220-221]
 * vix 06feb86 [added tree_mung()]
 * vix 20jun86 [added tree_delete per wirth a+ds (mod2 v.) p. 224]
 * vix 23jun86 [added delete del to add for replaced nodes]
 * vix 22jan93 [revisited; uses RCS, ANSI, POSIX; has bug fixes]

 gish dec93 [renamed functions to begin with avl_ prefix]
 gish dec93 [modified Avl_SearchNode () so as to be non-recursive (faster)]
 gish dec93 [added memory allocation function pointers]

 spouge mar94 [coretoolized]
 */

/* This program text was created by Paul Vixie using examples from the book:
 * "Algorithms & Data Structures," Niklaus Wirth, Prentice-Hall, 1986, ISBN
 * 0-13-022005-1.  This code and associated documentation is hereby placed
 * in the public domain.
 */

#ifdef MSG
#undef MSG
#endif
#define MSG(X)

static void balanceL (
Avl_NodePtr PNTR ppr_p,
BoolPtr pi_balance)
{
	Avl_NodePtr p1;
	Avl_NodePtr p2;
	Int1 b1;
	Int1 b2;
	MSG("left branch has shrunk")
	switch ((*ppr_p)->avl_b) {
	case -1: MSG("was imbalanced, fixed implicitly")
		(*ppr_p)->avl_b = 0;
	break;
	case 0:	MSG("was okay, is now one off")
		(*ppr_p)->avl_b = 1;
		*pi_balance = FALSE;
	break;
	case 1:	MSG("was already off, this is too much")
		p1 = (*ppr_p)->avl_r;
		b1 = p1->avl_b;
		if (b1 >= 0) {
			MSG("single RR")
			(*ppr_p)->avl_r = p1->avl_l;
			p1->avl_l = *ppr_p;
			if (b1 == 0) {
				MSG("b1 == 0")
				(*ppr_p)->avl_b = 1;
				p1->avl_b = -1;
				*pi_balance = FALSE;
			} else {
				MSG("b1 != 0")
				(*ppr_p)->avl_b = 0;
				p1->avl_b = 0;
			}
			*ppr_p = p1;
		} else {
			MSG("FloatHi RL")
			p2 = p1->avl_l;
			b2 = p2->avl_b;
			p1->avl_l = p2->avl_r;
			p2->avl_r = p1;
			(*ppr_p)->avl_r = p2->avl_l;
			p2->avl_l = *ppr_p;
			if (b2 == 1) {
				(*ppr_p)->avl_b = -1;
			} else {
				(*ppr_p)->avl_b = 0;
			}
			if (b2 == -1) {
				p1->avl_b = 1;
			} else {
				p1->avl_b = 0;
			}
			*ppr_p = p2;
			p2->avl_b = 0;
		}
	break;
	default:	
(void) Message (MSG_FATAL, "balanceL : Impossible alternative");
	break;
	}
}

static void balanceR (
Avl_NodePtr PNTR ppr_p,
BoolPtr pi_balance)
{
	Avl_NodePtr p1;
	Avl_NodePtr p2;
	Int1 b1;
	Int1 b2;
	MSG("right branch has shrunk")
	switch ((*ppr_p)->avl_b) {
	case 1:	MSG("was imbalanced, fixed implicitly")
		(*ppr_p)->avl_b = 0;
	break;
	case 0:	MSG("was okay, is now one off")
		(*ppr_p)->avl_b = -1;
		*pi_balance = FALSE;
	break;
	case -1: MSG("was already off, this is too much")
		p1 = (*ppr_p)->avl_l;
		b1 = p1->avl_b;
		if (b1 <= 0) {
			MSG("single LL")
			(*ppr_p)->avl_l = p1->avl_r;
			p1->avl_r = *ppr_p;
			if (b1 == 0) {
				MSG("b1 == 0")
				(*ppr_p)->avl_b = -1;
				p1->avl_b = 1;
				*pi_balance = FALSE;
			} else {
				MSG("b1 != 0")
				(*ppr_p)->avl_b = 0;
				p1->avl_b = 0;
			}
			*ppr_p = p1;
		} else {
			MSG("FloatHi LR")
			p2 = p1->avl_r;
			b2 = p2->avl_b;
			p1->avl_r = p2->avl_l;
			p2->avl_l = p1;
			(*ppr_p)->avl_l = p2->avl_r;
			p2->avl_r = *ppr_p;
			if (b2 == -1) {
				(*ppr_p)->avl_b = 1;
			} else {
				(*ppr_p)->avl_b = 0;
			}
			if (b2 == 1) {
				p1->avl_b = -1;
			} else {
				p1->avl_b = 0;
			}
			*ppr_p = p2;
			p2->avl_b = 0;
		}
	break;
	default:	
(void) Message (MSG_FATAL, "balanceR : Impossible alternative");
	break;
	}
}

static void delQNode (
Avl_NodePtr PNTR ppr_r,
Avl_NodePtr PNTR ppr_q,
Pointer PNTR p,
BoolPtr pi_balance,
BoolPtr pi_del_called)
{
	if ((*ppr_r)->avl_r != NULL) {
		delQNode (&(*ppr_r)->avl_r, ppr_q, p, pi_balance, pi_del_called);
		if (*pi_balance) {
			balanceR (ppr_r, pi_balance);
		}
	} else {
		*p = (*ppr_q)->avl_p;
		*pi_del_called = TRUE;
		(*ppr_q)->avl_p = (*ppr_r)->avl_p;
		*ppr_q = *ppr_r;
		*ppr_r = (*ppr_r)->avl_l;
		*pi_balance = TRUE;
	}
}

static Boolean findQNode (
Avl_NodePtr PNTR ppr_p,
Pointer p_user,
Pointer PNTR p,
BoolPtr pi_balance,
BoolPtr pi_del_called,
Int4 (*compare) PROTO ((Pointer a, Pointer b)))
{
	Avl_NodePtr pr_q;
	Boolean i_ret;
	Int4 i_comp;
	if (*ppr_p == NULL) {
		MSG("key not in tree")
return FALSE;
	}
	i_comp = (*compare) (p_user, (*ppr_p)->avl_p);
	if (i_comp < 0) {
		MSG("too high - scan left")
		i_ret = findQNode (&(*ppr_p)->avl_l, p_user, p, 
			pi_balance, pi_del_called, compare);
		if (*pi_balance) {
			balanceL (ppr_p, pi_balance);
		}
	} else if (i_comp > 0) {
		MSG("too low - scan right")
		i_ret = findQNode (&(*ppr_p)->avl_r, p_user, p, 
			pi_balance, pi_del_called, compare);
		if (*pi_balance) {
			balanceR (ppr_p, pi_balance);
		}
	} else {
		MSG("equal")
		pr_q = *ppr_p;
		if (pr_q->avl_r == NULL) {
			MSG("right subtree null")
			*ppr_p = pr_q->avl_l;
			*pi_balance = TRUE;
		} else if (pr_q->avl_l == NULL) {
			MSG("right subtree non-null, left subtree null")
			*ppr_p = pr_q->avl_r;
			*pi_balance = TRUE;
		} else {
			MSG("neither subtree null")
			delQNode (&pr_q->avl_l, &pr_q, p, pi_balance, pi_del_called);
			if (*pi_balance) {
				balanceL (ppr_p, pi_balance);
			}
		}
		if (!*pi_del_called) {
			*p = pr_q->avl_p;
		}
		pr_q = (Avl_NodePtr) MemFree ((Pointer) pr_q);
		i_ret = TRUE;
	}
return i_ret;
}

static Boolean Avl_DeleteNode (
Avl_NodePtr PNTR ppr_p,
Pointer p_user,
Pointer PNTR p,
Int4 (*compare) PROTO ((Pointer a, Pointer b)))
{
	Boolean i_balance = FALSE;
	Boolean i_del_called = FALSE;
return findQNode (ppr_p, p_user, p, 
	&i_balance, &i_del_called, compare);
}

static Pointer Avl_SearchNode ( 
Avl_NodePtr p_current,
Int4 (*compare) PROTO ((Pointer a, Pointer b)),
Pointer p_user)
{
	Avl_NodePtr current = p_current;
	Pointer cur_avl_p;
	Int4 i_comp;
	while (current) {
		cur_avl_p = current->avl_p;
		i_comp = (*compare) (p_user, current->avl_p);
		if (i_comp > 0) {
			current = current->avl_r;
	continue;
		} else if (i_comp == 0) {
return cur_avl_p; /* not higher, not lower... this must be the one  */
		}
		current = current->avl_l;
	}
return NULL; /* grounded. NOT found.  */
}

static Boolean Avl_TraverseNode (
Avl_NodePtr current,
Boolean (*boolfunc) PROTO ((Pointer a)))
{
	Avl_NodePtr p = current;
	if (!p) {
return TRUE;
	} else if (!Avl_TraverseNode (p->avl_l, boolfunc)) {
return FALSE;
	} else if (!(*boolfunc) (p->avl_p)) {
return FALSE;
	} else if (!Avl_TraverseNode (p->avl_r, boolfunc)) {
return FALSE;
	}
return TRUE;
}

static void Avl_ClearNode (
Avl_NodePtr PNTR ppr_tree)
{
	if (*ppr_tree) {
		Avl_ClearNode (&(**ppr_tree).avl_l);
		Avl_ClearNode (&(**ppr_tree).avl_r);
		*ppr_tree = (Avl_NodePtr) MemFree ((Pointer) *ppr_tree);
	}
}

static Boolean Avl_Sprout (
Avl_NodePtr PNTR ppr,
Pointer p_data,
BoolPtr pi_balance,
BoolPtr pi_insert,
Int4Ptr pi_unique,
Int4 (*compare) PROTO ((Pointer a, Pointer b)),
Int4 (*unique) PROTO ((Pointer a, Pointer b)))
{
	Avl_NodePtr p1;
	Avl_NodePtr p2;
	Int4 cmp;
	/* are we grounded?  if so, add the node "here" and set the rebalance
	 * flag, then exit.
	 */
	if (!*ppr) {
		MSG("grounded. adding new node, setting h=true")
		*ppr = (Avl_NodePtr) MemNew (sizeof(Avl_Node));
		if (*ppr == NULL) {
return FALSE;
		}
		(*ppr)->avl_l = NULL;
		(*ppr)->avl_r = NULL;
		(*ppr)->avl_b = 0;
		(*ppr)->avl_p = p_data;
		*pi_balance = TRUE;
return TRUE;
	}
	/* compare the data using routine passed by caller.
	 */
	cmp = (*compare) (p_data, (*ppr)->avl_p);
	/* if LESS, prepare to move to the left.
	 */
	if (cmp < 0) {
		MSG("LESS. sprouting left.")
		if (! Avl_Sprout (&(*ppr)->avl_l, p_data, 
			pi_balance, pi_insert, pi_unique, 
			compare, unique)) {
return FALSE;
		}
		if (*pi_balance) {	/* left branch has grown longer */
			MSG("LESS: left branch has grown")
			switch ((*ppr)->avl_b) {
			case 1:	/* right branch WAS longer; balance is ok now */
				MSG("LESS: case 1.. balance restored implicitly")
				(*ppr)->avl_b = 0;
				*pi_balance = FALSE;
			break;
			case 0:	/* balance WAS okay; now left branch longer */
				MSG("LESS: case 0.. balnce bad but still ok")
				(*ppr)->avl_b = -1;
			break;
			case -1:
				/* left branch was already too long. rebalnce */
				MSG("LESS: case -1: rebalancing")
				p1 = (*ppr)->avl_l;
				if (p1->avl_b == -1) {	/* LL */
					MSG("LESS: single LL")
					(*ppr)->avl_l = p1->avl_r;
					p1->avl_r = *ppr;
					(*ppr)->avl_b = 0;
					*ppr = p1;
				} else {			/* FloatHi LR */
					MSG("LESS: FloatHi LR")
					p2 = p1->avl_r;
					p1->avl_r = p2->avl_l;
					p2->avl_l = p1;
					(*ppr)->avl_l = p2->avl_r;
					p2->avl_r = *ppr;
					if (p2->avl_b == -1) {
						(*ppr)->avl_b = 1;
					} else {
						(*ppr)->avl_b = 0;
					}
					if (p2->avl_b == 1) {
						p1->avl_b = -1;
					} else {
						p1->avl_b = 0;
					}
					*ppr = p2;
				} /*else*/
				(*ppr)->avl_b = 0;
				*pi_balance = FALSE;
			break;
			default:
(void) Message (MSG_FATAL, "Avl_Sprout : Impossible alternative");
			break;
			} /*switch*/
		} /*if*/
return TRUE;
	} /*if*/
	/* if MORE, prepare to move to the right.
	 */
	if (cmp > 0) {
		MSG("MORE: sprouting to the right")
		if (! Avl_Sprout (&(*ppr)->avl_r, p_data, 
			pi_balance, pi_insert, pi_unique, 
			compare, unique)) {
return FALSE;
		}
		if (*pi_balance) {	/* right branch has grown longer */
			MSG("MORE: right branch has grown")
			switch ((*ppr)->avl_b)
			{
			case -1:MSG("MORE: balance was off, fixed implicitly")
				(*ppr)->avl_b = 0;
				*pi_balance = FALSE;
				break;
			case 0:	MSG("MORE: balance was okay, now off but ok")
				(*ppr)->avl_b = 1;
				break;
			case 1:	MSG("MORE: balance was off, need to rebalance")
				p1 = (*ppr)->avl_r;
				if (p1->avl_b == 1) {	/* RR */
					MSG("MORE: single RR")
					(*ppr)->avl_r = p1->avl_l;
					p1->avl_l = *ppr;
					(*ppr)->avl_b = 0;
					*ppr = p1;
				} else {			/* FloatHi RL */
					MSG("MORE: FloatHi RL")
					p2 = p1->avl_l;
					p1->avl_l = p2->avl_r;
					p2->avl_r = p1;
					(*ppr)->avl_r = p2->avl_l;
					p2->avl_l = *ppr;
					if (p2->avl_b == 1) {
						(*ppr)->avl_b = -1;
					} else {
						(*ppr)->avl_b = 0;
					}
					if (p2->avl_b == -1) {
						p1->avl_b = 1;
					} else {
						p1->avl_b = 0;
					}
					*ppr = p2;
				} /*else*/
				(*ppr)->avl_b = 0;
				*pi_balance = FALSE;
			} /*switch*/
		} /*if*/
return TRUE;
	} /*if*/
	/* not less, not more: this is the same key!  replace...
	 */
	MSG("I found it!  Replacing data value")
	*pi_balance = FALSE;
	*pi_insert = FALSE;
	if (unique) {
		*pi_unique = (*unique) (p_data, (*ppr)->avl_p);
	}
return TRUE;
}

static Boolean Avl_InsertNode (
Avl_NodePtr PNTR ppr_tree,
Pointer p_user,
BoolPtr pi_insert,
Int4Ptr pi_unique,
Int4 (*compare) PROTO ((Pointer a, Pointer b)),
Int4 (*unique) PROTO ((Pointer a, Pointer b)))
{
	Boolean i_balance = FALSE;
return Avl_Sprout (ppr_tree, p_user, 
		&i_balance, pi_insert, pi_unique, 
		compare, unique);
}

/*****************************************************************************
 
***************** END OF STATIC FUNCTIONS (Vixie & Wirth) *******************
  
*****************************************************************************/

void Avl_Initialize (
Avl_TreePtr tree, 
Int4 (*compare) PROTO ((Pointer newitem, Pointer treeitem)),
Int4 (*unique) PROTO ((Pointer newitem, Pointer treeitem)))
{
	tree->root = NULL;
	Avl_Clear (tree, compare, unique);
}

void Avl_Clear (
Avl_TreePtr tree, 
Int4 (*compare) PROTO ((Pointer newitem, Pointer treeitem)),
Int4 (*unique) PROTO ((Pointer newitem, Pointer treeitem)))
{
	Avl_ClearNode (&tree->root);
	tree->compare = compare;
	tree->unique = unique;  /* action upon finding item already in tree */
	tree->totalNodes = 0;
}

void Avl_SetUnique (
Avl_TreePtr tree, 
Int4 (*unique) PROTO ((Pointer newitem, Pointer treeitem)))
{
	tree->unique = unique;  /* action upon finding item already in tree */
}

size_t Avl_TotalNodes (
Avl_TreePtr tree)
{
return tree->totalNodes;
}

Boolean Avl_Insert (       /* return TRUE if p_user inserted without clash */
Avl_TreePtr tree,          /*          or if MemNew fails, */
Pointer p_user,            /*                    giving *unique = INT4_MAX */
Int4Ptr unique)
{
	Boolean i_insert = TRUE;
	if (! Avl_InsertNode (&tree->root, p_user, 
		&i_insert, unique, 
		tree->compare, tree->unique)) {
		*unique = INT4_MAX;
return TRUE;
	}
	if (i_insert) {
		tree->totalNodes++;
	}
return i_insert;
}

Pointer Avl_Search ( 
Avl_TreePtr tree,
Pointer p_user)
{
return Avl_SearchNode (tree->root, tree->compare, p_user);
}

Pointer Avl_Delete (
Avl_TreePtr tree, 
Pointer p_user)
{
	Pointer p = NULL;
	if (Avl_DeleteNode (&tree->root, p_user, 
		&p, tree->compare)) {
		tree->totalNodes--;
	}
return p;
}

Boolean Avl_Traverse (
Avl_TreePtr tree,
Boolean (*boolfunc) PROTO ((Pointer treeitem)))
{
return Avl_TraverseNode (tree->root, boolfunc);
}
