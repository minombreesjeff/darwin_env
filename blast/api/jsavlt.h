#ifndef _JSAVLT_
#define _JSAVLT_

#include <ncbi.h>

/*****************************************************************************
 
***************** AVL BALANCED BINARY TREES (Vixie & Wirth) ******************
  
*****************************************************************************/

typedef struct	Avl_Node {
	struct Avl_Node PNTR avl_l;
	struct Avl_Node PNTR avl_r;
	Int1 avl_b;
	Pointer avl_p;
} Avl_Node, PNTR Avl_NodePtr;

typedef struct	{
	Avl_NodePtr root; 
	Int4 (*compare)
		PROTO ((Pointer newitem, Pointer listitem));
	Int4 (*unique)
		PROTO ((Pointer newitem, Pointer listitem));
	size_t totalNodes;
} Avl_Tree, PNTR Avl_TreePtr;

void Avl_Initialize                     /* initializes tree */
	PROTO((Avl_TreePtr tree, 
	Int4 (*compare) PROTO ((Pointer newitem, Pointer treeitem)),
	Int4 (*unique) PROTO ((Pointer newitem, Pointer treeitem))));
void Avl_Clear                          /* reinitializes tree */
	PROTO((Avl_TreePtr tree, 
	Int4 (*compare) PROTO ((Pointer newitem, Pointer treeitem)),
	Int4 (*unique) PROTO ((Pointer newitem, Pointer treeitem))));
void Avl_SetUnique                      /* action when equal data inserted */
	PROTO ((Avl_TreePtr tree,
	Int4 (*unique) PROTO ((Pointer newitem, Pointer treeitem))));
size_t Avl_TotalNodes                   /* total number of items */
	PROTO ((Avl_TreePtr tree));
Boolean Avl_Insert                      /* inserts data */
	PROTO ((Avl_TreePtr tree, Pointer p_user, Int4Ptr unique));
Pointer Avl_Search                      /* if (! (any item == data)) NULL */
	PROTO ((Avl_TreePtr tree, Pointer p_user));
Pointer Avl_Delete                      /* deletes *item == *p_user */
	PROTO ((Avl_TreePtr tree, Pointer p_user)); /* returns TRUE if found */
Boolean Avl_Traverse                    /* traverses tree ; stops if FALSE */
	PROTO ((Avl_TreePtr tree, Boolean (*boolfunc) PROTO ((Pointer treeitem))));

#endif /* !_JSAVLT_ */
