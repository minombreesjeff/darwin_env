/*  tree.c
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
* File Name:  tree.c
*
* Author:  Vladimir Soussov
*   
* File Description:  tree manager functions
*
*
* $Log: tree.c,v $
* Revision 1.11  2000/08/15 14:34:33  soussov
* fixes bug in tree_getAncestor
*
* Revision 1.10  2000/08/14 16:05:30  soussov
* improvements in tree_getAncestor
*
* Revision 1.9  2000/08/07 14:32:04  soussov
* fixes bug in tree_getAncestor function
*
* Revision 1.8  1999/05/18 21:13:47  soussov
* TREE_SHUTDOWN event added
*
* Revision 1.7  1998/08/24 18:04:15  kans
* added continue statements to empty for loops to suppress Mac warnings
*
* Revision 1.6  1998/08/24 17:42:04  kans
* fixed old style function definition warnings
*
* Revision 1.5  1998/07/02 18:24:29  vakatov
* Cleaned the code & made it pass through the C++ compilation
*
* Revision 1.4  1998/06/23 16:33:49  vakatov
* [WIN32,MSVC++]  Made the tree API functions DLL-exportable
*
* Revision 1.3  1998/04/02 22:30:26  soussov
* Some prototype added to make Denis happy
*
* Revision 1.2  1998/04/01 17:48:06  soussov
* changed tp include <>
*
* Revision 1.1  1998/03/31 21:22:25  sirotkin
* With Vladimir - moved from distrib/internal/taxonomy/tree
*
* Revision 6.0  1997/08/25 18:29:47  madden
* Revision changed to 6.0
*
* Revision 1.1  1997/05/30 16:16:16  soussov
* Set of files for ncbitree library
*
* Revision 1.1  1997/05/29 20:44:26  soussov
* Initial version of tree library
*
*
*/

#include <treemgr.h>

static void notify(TreePtr tree, TreeEvent tevent, _NodeId id1, _NodeId id2, VoidPtr data, Int2 d_size)
{
    int i;

    for(i= 0; i < TREE_NOF_SPIES; i++) {
	if(tree->spy[i] != NULL) {
	    (*(tree->spy[i]))(tree, i, tevent, id1, id2, data, d_size);
	}
    }
}

static void tree_Message(CharPtr str1, CharPtr str2, Int2 msg_type)
{
    fprintf(stderr, "Function %s :\t%s\n", str1, str2);
}

NLM_EXTERN Boolean tree_toNode(TreeCursorPtr cursor, _NodeId id)
{
    _BagStore bstore;
    _NodeBagPtr bag;

    if(id.idi == 0) return FALSE; /* empty id */
    if(cursor->node_id.idi == id.idi) {
	return TRUE; /* same node */
    }

    if((cursor->node_id.idi != 0) && 
       (cursor->node_id.id4.bag_store == id.id4.bag_store) &&
       (cursor->node_id.id4.bag == id.id4.bag)) {
	/* we are looking for node from the same bag */

	bag= cursor->cbag;
	if(((bag->node[id.id4.node].flags & TREE_FREE_ROOM) != 0) ||
	   (bag->node[id.id4.node].vers != id.id4.vers)) {
	    return FALSE;
	}

    }
    else {

	if(((bstore= cursor->tree->bag_store[id.id4.bag_store]) == NULL) || 
	   ((bag= bstore[id.id4.bag]) == NULL) ||
	   ((bag->node[id.id4.node].flags & TREE_FREE_ROOM) != 0) ||
	   (bag->node[id.id4.node].vers != id.id4.vers)) {

	    /* node does not exist */
	    return FALSE;
	}
    }

    if(bag->node[id.id4.node].flags & TREE_MERGED_NODE) {
	/* this node has been merged */
	return tree_toNode(cursor, bag->node[id.id4.node].sibling);
    }

    cursor->prev_id= cursor->node_id;
    cursor->node_id= id;
    cursor->node= bag->node + id.id4.node;
    cursor->cbag= bag;
    if(cursor->callBack != NULL) {
	(*(cursor->callBack))(cursor, TREE_CURSOR_MOVED, id);
    }
    return TRUE;
}

NLM_EXTERN Boolean tree_root(TreeCursorPtr cursor)
{
    return tree_toNode(cursor, cursor->tree->rootNodeId);
}

NLM_EXTERN Boolean tree_parent(TreeCursorPtr cursor)
{

    if(cursor->node_id.idi == cursor->tree->rootNodeId.idi) return FALSE;

    return tree_toNode(cursor, cursor->node->parent);
}

NLM_EXTERN Boolean tree_sibling(TreeCursorPtr cursor)
{
    return tree_toNode(cursor, cursor->node->sibling);
}

NLM_EXTERN Boolean tree_child(TreeCursorPtr cursor)
{
    return tree_toNode(cursor, cursor->node->child);
}


/**********************************************
 * Open cursor and set it on a root node.
 * You can provide an optional callback function
 * Tree manager calls this callBack function in following cases:
 * - cursor has been moved
 * - node under cursor is going to be deleted, updated, moved or merged using another cursor
 *
 * callBack function should be defined as
 * void callBackFunc(TreeCursorPtr cursor, TreeEvent event, TreeNodeId id //optional);
 */

NLM_EXTERN TreeCursorPtr tree_openCursor(TreePtr tree, 
                                         VoidPtr userDataPointer,
                                         TreeCursorCBFunc callBack)
{
    TreeCursorPtr cursor;

    if((cursor= (TreeCursorPtr)MemNew(sizeof(TreeCursor))) == NULL) {
	tree_Message("tree_openCursor", "Not enaugh memory", NO_MEM_MSG);
	return NULL;
    }
    
    cursor->tree= tree;
    cursor->next_cursor= tree->cursor;
    tree->cursor= cursor;
    cursor->node_id.idi= 0;
    cursor->callBack= NULL;
    tree_toNode(cursor, tree->rootNodeId);
    cursor->prev_id= cursor->node_id;
    cursor->callBack= callBack;
    cursor->user_data= userDataPointer;
    return cursor;
}

/**********************************************
 * Close cursor
 */
NLM_EXTERN void tree_closeCursor(TreeCursorPtr cursor)
{
    TreeCursorPtr crs;

    if(cursor == NULL) return;

    if(cursor->tree->cursor == cursor) {
	cursor->tree->cursor= cursor->next_cursor;
	MemFree(cursor);
    }
    else {
	for(crs= cursor->tree->cursor; crs != NULL; crs= crs->next_cursor) {
	    if(crs->next_cursor == cursor) {
		crs->next_cursor= cursor->next_cursor;
		MemFree(cursor);
		return;
	    }
	}
    }
}

/***********************************************
 * Get user data associated with a cursor
 */
NLM_EXTERN VoidPtr tree_getUserData(TreeCursorPtr cursor)
{
    return (cursor != NULL)? cursor->user_data : NULL;
}

/***********************************************
 * Get node id
 */
NLM_EXTERN TreeNodeId tree_getId(TreeCursorPtr cursor)
{
    if(cursor != NULL) return cursor->node_id;
    else {
	_NodeId id;

	id.idi= 0;
	return id;
    }
}
    

NLM_EXTERN _NodeId tree_getAncestor(TreeCursorPtr cursor1,
                                    TreeCursorPtr cursor2)
{
    _NodeId aId;

    aId.idi= 0;

    if(cursor1->tree == cursor2->tree) {
	TreeCursorPtr t_cursor= tree_openCursor(cursor1->tree, NULL, NULL);
	_NodeId *lin1;
	_NodeId *lin2;
	int l1, l2, s1= 64, s2= 64;
	
	/* build lineage for cursor1 */
	lin1= (_NodeId *)MemNew(sizeof(_NodeId)*s1);
	if(lin1 == NULL) {
	    tree_Message("tree_getAncestor", "Not enaugh memory", NO_MEM_MSG);
	    return aId;
	}

	tree_toNode(t_cursor, cursor1->node_id);

	for(l1= 0; tree_parent(t_cursor); l1++) {
	    if(l1 >= s1) {
		s1+= s1/2;
		lin1= (_NodeId *)MemMore(lin1, sizeof(_NodeId)*s1);
		if(lin1 == NULL) {
		    tree_Message("tree_getAncestor", "Not enaugh memory", NO_MEM_MSG);
		    return aId;
		}
	    }
	    lin1[l1]= t_cursor->node_id;
	}
	s1= l1;

	/* check lin1 against cursor2 */
	for(l1= 0; l1 < s1; l1++) {
	    if(lin1[l1].idi == cursor2->node_id.idi) {
		MemFree(lin1);
		tree_closeCursor(t_cursor);
		return cursor2->node_id;
	    }
	}

	/* build lineage for cursor2 */
	lin2= (_NodeId *)MemNew(sizeof(_NodeId)*s2);
	if(lin2 == NULL) {
	    tree_Message("tree_getAncestor", "Not enaugh memory", NO_MEM_MSG);
	    return aId;
	}

	tree_toNode(t_cursor, cursor2->node_id);

	for(l2= 0; tree_parent(t_cursor); l2++) {
	    if(l2 >= s2) {
		s2+= s2/2;
		lin2= (_NodeId *)MemMore(lin2, sizeof(_NodeId)*s2);
		if(lin2 == NULL) {
		    tree_Message("tree_getAncestor", "Not enaugh memory", NO_MEM_MSG);
		    return aId;
		}
	    }
	    lin2[l2]= t_cursor->node_id;
	}
	s2= l2;
	
	/* check lin2 against cursor1 */
	for(l2= 0; l2 < s2; l2++) {
	    if(lin2[l2].idi == cursor1->node_id.idi) {
		MemFree(lin1);
		MemFree(lin2);
		tree_closeCursor(t_cursor);
		return cursor1->node_id;
	    }
	}
	
	/* check lineages */
	for(l1= s1, l2= s2; (--l1 > 0) && (--l2 > 0);)  {
	    if(lin1[l1-1].idi != lin2[l2-1].idi) {
		aId= lin1[l1];
		break;
	    }
	}
	if(aId.idi == 0) aId= lin1[l1];
	MemFree(lin1);
	MemFree(lin2);
	tree_closeCursor(t_cursor);
    }

    return aId;
	
}

static void detach_subtree(TreeCursorPtr cursor)
{
    TreeCursorPtr t_cursor= tree_openCursor(cursor->tree, NULL, NULL);

    tree_toNode(t_cursor, cursor->node->parent);
    if(t_cursor->node->child.idi == cursor->node_id.idi) {
	/* this is a first child */
	t_cursor->node->child= cursor->node->sibling;
    }
    else {
	Boolean cont= TRUE;

	for(tree_child(t_cursor); 
	    cont && (t_cursor->node->sibling.idi != cursor->node_id.idi); 
	    cont= tree_sibling(t_cursor)) continue;
	
	if(cont) {
	    t_cursor->node->sibling= cursor->node->sibling;
	}
	else {
	    fprintf(stderr, "detach_subtree: Tree structure corrupted\n");
	}
    }
    
    tree_closeCursor(t_cursor);
}
	    

    
NLM_EXTERN Boolean tree_moveNode(TreeCursorPtr cursor1, TreeCursorPtr cursor2)
{
    _NodeId aId= tree_getAncestor(cursor1, cursor2);
    _NodePtr cnode;
    _NodePtr pnode;

    if(aId.idi == cursor2->node_id.idi) return FALSE; /* to prevent loop */

    if(cursor2->tree->nof_spies > 0) {
	notify(cursor2->tree, TREE_NODE_MOVE, cursor1->node_id, cursor2->node_id, NULL, 0);
    }

    detach_subtree(cursor2);

    cnode= cursor2->node;
    pnode= cursor1->node;

    cnode->sibling= pnode->child;
    pnode->child= cursor2->node_id;
    cnode->parent= cursor1->node_id;

    if(cursor2->tree->nof_spies > 0) {
	notify(cursor2->tree, TREE_NODE_MV_DONE, cursor1->node_id, cursor2->node_id, NULL, 0);
    }

    return TRUE;
}

NLM_EXTERN Boolean tree_moveChildren(TreeCursorPtr cursor1,
                                     TreeCursorPtr cursor2)
{
    _NodeId aId= tree_getAncestor(cursor1, cursor2);
    TreeCursorPtr t_cursor;
    Boolean cnt, flg= FALSE;
    
    
    if(aId.idi == cursor2->node_id.idi) return FALSE; /* to prevent loop */

    if(cursor2->node->child.idi == 0) return TRUE; /* no children */
    
    if(cursor2->tree->nof_spies > 0) {
	notify(cursor2->tree, TREE_CHILDREN_MOVE, cursor1->node_id, cursor2->node_id, NULL, 0);
    }

    t_cursor= tree_openCursor(cursor2->tree, NULL, NULL);

    for(cnt= tree_toNode(t_cursor, cursor2->node->child); cnt; cnt= tree_sibling(t_cursor)) {
	t_cursor->node->parent= cursor1->node_id;
	flg= TRUE;
    }

    if(flg) {
	t_cursor->node->sibling= cursor1->node->child;
	cursor1->node->child= cursor2->node->child;
	cursor2->node->child.idi= 0;
    }

    if(cursor2->tree->nof_spies > 0) {
	notify(cursor2->tree, TREE_CHILDREN_MV_DONE, cursor1->node_id, cursor2->node_id, NULL, 0);
    }

    return TRUE;
}

static _NodeId add_node(TreeCursorPtr cursor, _NodeBagPtr bag, _NodeId new_id, 
		     VoidPtr node_data, Uint2 data_size)
{
    _NodePtr nnode= bag->node + new_id.id4.node;

    nnode->parent= cursor->node_id;
    nnode->child.idi= 0;
    nnode->sibling= cursor->node->child;
    nnode->flags^= TREE_FREE_ROOM;
    nnode->sys_data= NULL;
    new_id.id4.vers= ++nnode->vers;
    cursor->node->child= new_id;
    bag->nof_nodes++;
    if(data_size <= sizeof(_NodeData)) {
	memcpy(&nnode->data, node_data, data_size);
    }
    else {
	nnode->data= MemNew(data_size);
	memcpy(nnode->data, node_data, data_size);
    }
    nnode->size= data_size;

    if(cursor->tree->nof_spies > 0) {
	notify(cursor->tree, TREE_NODE_ADDED, cursor->node_id, new_id, node_data, data_size);
    }

    return new_id;
}
    

NLM_EXTERN _NodeId tree_addChild(TreeCursorPtr cursor, VoidPtr node_data, Uint2 node_data_size)
{
    int i;
    _NodeBagPtr bag= cursor->cbag;
    _NodeId new_id;
    _BagStore bstore;
    int j, k;

    if(bag->nof_nodes < NODE_BAG_SIZE) {
	/* there is a room in current bag */
	for(i= 0; i != NODE_BAG_SIZE; i++) {
	    if((bag->node[i].flags & TREE_FREE_ROOM) != 0) break; /* free room */
	}
	if(i < NODE_BAG_SIZE) {
	    /* free room was found */
	    new_id= cursor->node_id;
	    new_id.id4.node= i;
	    return add_node(cursor, bag, new_id, node_data, node_data_size);
	}
	else {
	    bag->nof_nodes= NODE_BAG_SIZE;
	}
    }

    /* there is no room in current bag */

    /* try the current store first */
    bstore= bag->my_bag_store;

    for(i= 0; i != NODE_BAG_SIZE; i++) {
	if((bstore[i] != NULL) && (bstore[i]->nof_nodes < NODE_BAG_SIZE)) {
	    bag= bstore[i];
	    for(j= 0; j != NODE_BAG_SIZE; j++) {
		if((bag->node[j].flags & TREE_FREE_ROOM) != 0) break; /* free room */
	    }
	    if(j < NODE_BAG_SIZE) {
		/* free room was found */
		new_id= cursor->node_id;
		new_id.id4.bag= i;
		new_id.id4.node= j;
		return add_node(cursor, bag, new_id, node_data, node_data_size);
	    }
	    else {
		bag->nof_nodes= NODE_BAG_SIZE;
	    }
	}
    }
    
    /* there is no room in current bag store */
    /* try all non-empty bag stores */
    for(k= 0; k != NOF_BAGS; k++) {
	if((bstore= cursor->tree->bag_store[k]) != NULL) {
	    for(i= 0; i != NODE_BAG_SIZE; i++) {
		if((bstore[i] != NULL) && (bstore[i]->nof_nodes < NODE_BAG_SIZE)) {
		    bag= bstore[i];
		    for(j= 0; j != NODE_BAG_SIZE; j++) {
			if((bag->node[j].flags & TREE_FREE_ROOM) != 0) break; /* free room */
		    }
		    if(j < NODE_BAG_SIZE) {
			/* free room was found */
			new_id.id4.bag_store= k;
			new_id.id4.bag= i;
			new_id.id4.node= j;
			return add_node(cursor, bag, new_id, node_data, node_data_size);
		    }
		    else {
			bag->nof_nodes= NODE_BAG_SIZE;
		    }
		}
	    }
	}
    }

    /* we need the new bag */
    k= cursor->tree->id4NewNode.id4.bag_store;
    j= cursor->tree->id4NewNode.id4.bag;

    if(++j >= NODE_BAG_SIZE) {
	/* all bags in last bag store are used, we need new bag store */
	if(++k >= NOF_BAGS) {
	    /* there are no more bag store available */
	    new_id.idi= 0;
	    return new_id;
	}
	/* allocate the new bag_store */
	if((bstore= (_NodeBagPtr*)MemNew(NODE_BAG_SIZE*sizeof(_NodeBagPtr))) == NULL) {
	    /* no memory */
	    new_id.idi= 0;
	    return new_id;
	}
	cursor->tree->bag_store[k]= bstore;
	cursor->tree->id4NewNode.id4.bag_store= k;
	
	for(i= 0; i != NODE_BAG_SIZE; bstore[i++]= NULL) continue;
	j= 0;
	cursor->tree->id4NewNode.id4.bag= 0;
    }
    else {
	bstore= cursor->tree->bag_store[k];
    }

    if((bag= bstore[j]= (_NodeBagPtr)MemNew(sizeof(_NodeBag))) == NULL) {
	/* no memory */
	new_id.idi= 0;
	return new_id;
    }
    
    
    bag->nof_nodes= 0;
    bag->my_bag_store= bstore;
    
    for(i= 0; i != NODE_BAG_SIZE; i++) {
	bag->node[i].flags= TREE_FREE_ROOM;
	bag->node[i].vers= 0;
    }

    new_id.idi= cursor->tree->id4NewNode.idi;
    new_id.id4.node= 0;
    new_id.id4.bag= cursor->tree->id4NewNode.id4.bag= j;
    return add_node(cursor, bag, new_id, node_data, node_data_size);
}


NLM_EXTERN Boolean tree_delNode(TreeCursorPtr cursor)
{
    TreeCursorPtr t_cursor;
    _NodeId pid;

    if(cursor->node_id.idi == cursor->tree->rootNodeId.idi) return FALSE; /* can not remove root */

    pid= cursor->node->parent;


    for(t_cursor= cursor->tree->cursor; t_cursor != NULL; t_cursor= t_cursor->next_cursor) {
	if((t_cursor != cursor) && (t_cursor->node == cursor->node)) {
	    if(t_cursor->callBack != NULL) {
		(*(t_cursor->callBack))(t_cursor, TREE_NODE_DELETE, cursor->node_id);

		if(t_cursor->node == cursor->node) {
		    tree_toNode(t_cursor, pid);
		}
	    }
	    else {		
		tree_toNode(t_cursor, pid);
	    }
	}
    }
	

    t_cursor= tree_openCursor(cursor->tree, NULL, NULL);
    tree_toNode(t_cursor, pid);

    tree_moveChildren(t_cursor, cursor);

    tree_closeCursor(t_cursor);

    if(cursor->tree->nof_spies > 0) {
	notify(cursor->tree, TREE_NODE_DELETE, cursor->node_id, pid, NULL, 0);
    }

    detach_subtree(cursor);

    if(cursor->node->size > sizeof(_NodeData)) {
	/* free node data if allocated separatelly */
	MemFree(cursor->node->data);
    }

    if(cursor->node->vers < 255) {
	/* we can reuse this room in the future */
	cursor->node->flags|= TREE_FREE_ROOM;
	cursor->cbag->nof_nodes--;
    }
    else { /* we can not reuse this room */
	int i;

	cursor->node->vers= 0;
	for(i= 0; i != NODE_BAG_SIZE; i++) {
	    if((cursor->cbag->node[i].vers != 0) || 
	       ((cursor->cbag->node[i].flags & TREE_FREE_ROOM) != 0)) break;
	}

	if(i == NODE_BAG_SIZE) { /* all rooms in this bag are empty */
	    _BagStore bs= cursor->cbag->my_bag_store;
	    
	    i= cursor->node_id.id4.bag;
	    MemFree(bs[i]);
	    bs[i]= NULL;
	}
    }

    tree_toNode(cursor, pid);

    if(cursor->tree->nof_spies > 0) {
	notify(cursor->tree, TREE_NODE_DEL_DONE, cursor->node_id, pid, NULL, 0);
    }

    return TRUE;
}

/* recursion function to delete nodes from subtree */	    
static void del_subtreeNodes(TreeCursorPtr cursor)
{
    _NodeId nid, sid;

    nid= cursor->node_id;

    if(tree_child(cursor)) {
	do {
	    sid= cursor->node->sibling;
	    del_subtreeNodes(cursor);
	}
	while(tree_toNode(cursor, sid));
    }

    tree_toNode(cursor, nid);

    if(cursor->node->size > sizeof(_NodeData)) {
	/* free node data if allocated separatelly */
	MemFree(cursor->node->data);
    }

    if(cursor->node->vers < 255) {
	/* we can reuse this room in the future */
	cursor->node->flags|= TREE_FREE_ROOM;
	cursor->cbag->nof_nodes--;
    }
    else { /* we can not reuse this room */
	int i;

	cursor->node->vers= 0;
	for(i= 0; i != NODE_BAG_SIZE; i++) {
	    if((cursor->cbag->node[i].vers != 0) || 
	       ((cursor->cbag->node[i].flags & TREE_FREE_ROOM) != 0)) break;
	}

	if(i == NODE_BAG_SIZE) { /* all rooms in this bag are empty */
	    _BagStore bs= cursor->cbag->my_bag_store;
	    
	    i= cursor->node_id.id4.bag;
	    MemFree(bs[i]);
	    bs[i]= NULL;
	}
    }
}
    
    
    

NLM_EXTERN Boolean tree_delSubtree(TreeCursorPtr cursor)
{
    TreeCursorPtr t_cursor;
    TreeCursorPtr crs;
    _NodeId pid;
    Boolean res;

    if(cursor->node_id.idi == cursor->tree->rootNodeId.idi) return FALSE; /* can not remove root */

    pid= cursor->node->parent;

    if(cursor->tree->nof_spies > 0) {
	notify(cursor->tree, TREE_SUBTREE_DELETE, cursor->node_id, pid, NULL, 0);
    }

    crs= tree_openCursor(cursor->tree, NULL, NULL);
    
    /* shovl cursors from deleting subtree */
    for(t_cursor= cursor->tree->cursor; t_cursor != NULL; t_cursor= t_cursor->next_cursor) {
	if((t_cursor != cursor) && (t_cursor != crs)) {
	    for(res= tree_toNode(crs, t_cursor->node_id); res; res= tree_parent(crs)) {
		if(crs->node_id.idi == cursor->node_id.idi) break;
	    }

	    if(res) {
		if(t_cursor->callBack != NULL) {
		    (*(t_cursor->callBack))(t_cursor, TREE_NODE_DELETE, cursor->node_id);

		    for(res= tree_toNode(crs, t_cursor->node_id); res; res= tree_parent(crs)) {
			if(crs->node_id.idi == cursor->node_id.idi) break;
		    }

		    if(res) {
			tree_toNode(t_cursor, pid);
		    }
		}
		else {
		    tree_toNode(t_cursor, pid);
		}
	    }
	}
    }
	
    /* remove nodes */

    detach_subtree(cursor);

    tree_toNode(crs, cursor->node_id);
    del_subtreeNodes(crs);

    tree_closeCursor(crs);


    if(cursor->tree->nof_spies > 0) {
	notify(cursor->tree, TREE_SUBTREE_DEL_DONE, cursor->node_id, pid, NULL, 0);
    }

    return tree_toNode(cursor, cursor->node->parent);
}

NLM_EXTERN Boolean tree_merge(TreeCursorPtr cursor1, TreeCursorPtr cursor2)
{
    _NodeId aId= tree_getAncestor(cursor1, cursor2);
    TreeCursorPtr p_crs;

    if(aId.idi == cursor2->node_id.idi) return FALSE; /* to prevent loop */

    if(cursor2->tree->nof_spies > 0) {
	notify(cursor2->tree, TREE_NODE_MERGE, cursor1->node_id, cursor2->node_id, NULL, 0);
    }

    /* Notify and move out all cursors which point to cursor2->node_id */
    for(p_crs= cursor2->tree->cursor; p_crs != NULL; p_crs= p_crs->next_cursor) {
	if((p_crs->node == cursor2->node) && (p_crs != cursor2)) {
	    if(p_crs->callBack != NULL) {
		(*(p_crs->callBack))(p_crs, TREE_NODE_MERGE, cursor1->node_id);
		if(p_crs->node == cursor2->node) {
		    tree_toNode(p_crs, cursor1->node_id);
		}
	    }
	    else {
		tree_toNode(p_crs, cursor1->node_id);
	    }
	}
    }

    tree_moveChildren(cursor1, cursor2);
    detach_subtree(cursor2);
    cursor2->node->flags|= TREE_MERGED_NODE;
    cursor2->node->sibling= cursor1->node_id;

    if(cursor2->tree->nof_spies > 0) {
	notify(cursor2->tree, TREE_NODE_MERGE_DONE, cursor1->node_id, cursor2->node->parent, NULL, 0);
    }

    return tree_toNode(cursor2, cursor1->node_id);
}


/**********************************************
 * Create new tree and add root node to it
 * 
 */
NLM_EXTERN TreePtr tree_new(void)
{
    TreePtr ntree;
    _BagStore bs;
    int i;

    if((ntree= (TreePtr)MemNew(sizeof(Tree))) == NULL) {
	tree_Message("tree_new", "Not enaugh memory", NO_MEM_MSG);
	return NULL;
    }

    ntree->cursor= NULL;
    ntree->get_func= NULL;
    ntree->update_func= NULL;
    ntree->save_func= NULL;

    for(i= 0; i < TREE_NOF_SPIES; ntree->spy[i++]= NULL) continue;

    ntree->nof_spies= 0;

    for(i= 1; i != NOF_BAGS; ntree->bag_store[i++]= NULL) continue;

    if((ntree->bag_store[0]= bs= (_BagStore)MemNew(sizeof(_NodeBagPtr)*NODE_BAG_SIZE)) == NULL) {
	tree_Message("tree_new", "Not enaugh memory", NO_MEM_MSG);
	return NULL;
    }
	
    for(i= 1; i != NODE_BAG_SIZE; bs[i++]= NULL) continue;

    if((bs[0]= (_NodeBagPtr)MemNew(sizeof(_NodeBag))) == NULL) {
	tree_Message("tree_new", "Not enaugh memory", NO_MEM_MSG);
	return NULL;
    }
	
    bs[0]->nof_nodes= 1;
    bs[0]->my_bag_store= bs;
    
    for(i= 1; i != NODE_BAG_SIZE; i++) {
	bs[0]->node[i].vers= 0;
	bs[0]->node[i].flags= TREE_FREE_ROOM;
    }

    /* setup root node */

    bs[0]->node[0].parent.idi= bs[0]->node[0].child.idi= bs[0]->node[0].sibling.idi= 0;
    bs[0]->node[0].size= 0;
    bs[0]->node[0].vers= 1;
    bs[0]->node[0].flags= 0;
    
    ntree->rootNodeId.id4.bag_store= 0;
    ntree->rootNodeId.id4.bag= 0;
    ntree->rootNodeId.id4.node= 0;
    ntree->rootNodeId.id4.vers= 1;
    
    ntree->id4NewNode= ntree->rootNodeId;

    return ntree;
}

/**********************************************
 * Delete tree
 */
static void freeNodeData(TreeCursorPtr cursor)
{
    if(cursor->node->size > sizeof(_NodeData)) MemFree(cursor->node->data);
    if(tree_child(cursor)) {
	do {
	    freeNodeData(cursor);
	}
	while(tree_sibling(cursor));
	tree_parent(cursor);
    }
}


NLM_EXTERN Boolean tree_delete(TreePtr tree)
{
    TreeCursorPtr cursor;
    int i, j;
    _BagStore bs;
    _NodeId nid;

    nid.idi= 0;

    notify(tree, TREE_SHUTDOWN, nid, nid, NULL, 0);
    /* close all existing cursors first */
    while((cursor= tree->cursor) != NULL) {
	tree_closeCursor(cursor);
    }

    /* delete all data allocated for nodes */
    cursor= tree_openCursor(tree, NULL, NULL);

    freeNodeData(cursor);

    tree_closeCursor(cursor);

    for(i= 0; i != NOF_BAGS; i++) {
	if((bs= tree->bag_store[i]) != NULL) {
	    for(j= 0; j != NODE_BAG_SIZE; j++)
		if(bs[j] != NULL) MemFree(bs[j]);
	    MemFree(bs);
	}
    }

    MemFree(tree);
    return TRUE;
}


/***********************************************
 * Save tree in a file
 */
static Boolean saveNode(TreeCursorPtr cursor /* cursor->user_data is a file pointer */)
{
    if(cursor->tree->save_func != NULL) cursor->tree->save_func(cursor);

    if(cursor->node->size > sizeof(_NodeData)) {
	if(FileWrite(cursor->node->data, cursor->node->size, 1, (FILE*)(cursor->user_data)) < 1) 
	    return FALSE;
    }

    if(tree_child(cursor)) {
	do {
	    if(!saveNode(cursor)) return FALSE;
	}
	while(tree_sibling(cursor));
	
	tree_parent(cursor);
    }

    return TRUE;
}


NLM_EXTERN Boolean tree_save(TreePtr tree, CharPtr filename)
{
    FILE* f;
    Int2 i, j;
    _BagStore bs;
    TreeCursorPtr cursor;
    Boolean res;

    f= FileOpen(filename, "wb");
    if(f == NULL) return FALSE;
    
    /* write ids from tree */
    if(FileWrite(&tree->id4NewNode, sizeof(_NodeId), 1, f) < 1) return FALSE;
    if(FileWrite(&tree->rootNodeId, sizeof(_NodeId), 1, f) < 1) return FALSE;
    
    i= (tree->save_func == NULL)? 1 : 0;

    if(FileWrite(&i, sizeof(Int2), 1, f) < 1) return FALSE;

    /* write tree structure itself */
    if(FileWrite(tree->bag_store, sizeof(_BagStore), NOF_BAGS, f) < NOF_BAGS) return FALSE;

    for(i= 0; i < NOF_BAGS; i++) {
	if((bs= tree->bag_store[i]) != NULL) {
	    if(FileWrite(bs, sizeof(_NodeBagPtr), NODE_BAG_SIZE, f) < NODE_BAG_SIZE) return FALSE;
	    
	    for(j= 0; j < NODE_BAG_SIZE; j++) {
		if(bs[j] != NULL) {
		    if(FileWrite(bs[j], sizeof(_NodeBag), 1, f) < 1) return FALSE;
		}
	    }
	}
    }
	    
    /* save node's data */
    cursor= tree_openCursor(tree, f, NULL);

    res= saveNode(cursor);

    FileClose(f);

    tree_closeCursor(cursor);

    return res;
}
    

/***********************************************
 * Restore tree from the file
 */
static Boolean restoreNode(TreeCursorPtr cursor /* cursor->user_data is a file pointer */)
{

    if(cursor->node->size > sizeof(_NodeData)) {
	if(((cursor->node->data= MemNew(cursor->node->size)) == NULL) ||
	   (FileRead(cursor->node->data, cursor->node->size, 1, (FILE*)(cursor->user_data)) < 1)) {
		return FALSE;
	}
    }

    if(tree_child(cursor)) {
	do {
	    if(!restoreNode(cursor)) return FALSE;
	}
	while(tree_sibling(cursor));
	
	tree_parent(cursor);
    }

    cursor->node->sys_data= NULL;

    return TRUE;
}

NLM_EXTERN TreePtr tree_restore(CharPtr filename)
{
    TreePtr ntree;
    _BagStore bs;
    Int2 nodeDataSaved;
    int i, j;
    FILE* f;

    f= FileOpen(filename, "rb");
    if(f == NULL) return NULL;
    
    if((ntree= (TreePtr)MemNew(sizeof(Tree))) == NULL) {
	tree_Message("tree_restore", "Not enaugh memory", NO_MEM_MSG);
	return NULL;
    }

    ntree->cursor= NULL;
    ntree->get_func= NULL;
    ntree->update_func= NULL;
    ntree->save_func= NULL;

    for(i= 0; i < TREE_NOF_SPIES; ntree->spy[i++]= NULL) continue;

    ntree->nof_spies= 0;

    if((FileRead(&ntree->id4NewNode, sizeof(_NodeId), 1, f) < 1) ||
       (FileRead(&ntree->rootNodeId, sizeof(_NodeId), 1, f) < 1) ||
       (FileRead(&nodeDataSaved, sizeof(Int2), 1, f) < 1)) {

	MemFree(ntree);
	FileClose(f);
	return NULL;
    }

    if((nodeDataSaved != 0) && (nodeDataSaved != 1)) {
	/* this is wrong file */
	MemFree(ntree);
	FileClose(f);
	return NULL;
    }

    nodeDataSaved= 1;

    /* read tree structure itself */
    if(FileRead(ntree->bag_store, sizeof(_BagStore), NOF_BAGS, f) < NOF_BAGS) {

	MemFree(ntree);
	FileClose(f);
	return NULL;
    }

    for(i= 0; i != NOF_BAGS; i++) {
	if(ntree->bag_store[i] != NULL) {
	    ntree->bag_store[i]= bs= (_BagStore)MemNew(sizeof(_NodeBagPtr)*NODE_BAG_SIZE);
	    if(bs == NULL) {
		tree_Message("tree_restore", "Not enaugh memory", NO_MEM_MSG);
		FileClose(f);
		MemFree(ntree);
		return NULL;
	    }

	    if(FileRead(bs, sizeof(_NodeBagPtr), NODE_BAG_SIZE, f) < NODE_BAG_SIZE) {
		FileClose(f);
		MemFree(bs);
		MemFree(ntree);
		return NULL;
	    }
	    
	    for(j= 0; j < NODE_BAG_SIZE; j++) {
		if(bs[j] != NULL) {
		    if((bs[j]= (_NodeBagPtr)MemNew(sizeof(_NodeBag))) == NULL) {
			tree_Message("tree_restore", "Not enaugh memory", NO_MEM_MSG);
			FileClose(f);
			MemFree(bs);
			MemFree(ntree);
			return NULL;
		    }

		    if(FileRead(bs[j], sizeof(_NodeBag), 1, f) < 1) {
			MemFree(bs[j]);
			FileClose(f);
			MemFree(bs);
			MemFree(ntree);
			return NULL;
		    }			
		}
	    }
	}
    }
	    

    if(nodeDataSaved) {
	/* we need to restore node's data */
	TreeCursorPtr cursor= tree_openCursor(ntree, f, NULL);
	
	if(!restoreNode(cursor)) {
	    FileClose(f);
	    tree_closeCursor(cursor);
	    MemFree(ntree);
	    return NULL;
	}

	tree_closeCursor(cursor);
    }

    FileClose(f);
    return ntree;
}


/***************************************************************************
 * Get node data pointer !!! This function has to be used for callback functions only !!!
 * in any case you should not update the data using returned pointer
 */
NLM_EXTERN VoidPtr tree_getNodeData(TreeCursorPtr cursor, Uint2Ptr data_size)
{
    *data_size= cursor->node->size;
    if(*data_size > sizeof(_NodeData)) return cursor->node->data;
    else return &cursor->node->data;
}

/***************************************************************************
 * Update node data !!! This function has to be used for callback functions only !!!
 * if you used tree_getNodeData for this node, pointer to node data will incorrect
 */
NLM_EXTERN Boolean tree_updateNodeData(TreeCursorPtr cursor, VoidPtr node_data, Uint2 node_data_size)
{
    if(cursor->tree->nof_spies > 0) {
	notify(cursor->tree, TREE_NODEDATA_UPDATE, cursor->node_id,
	       cursor->node_id, node_data, node_data_size);
    }

    if(cursor->node->size == node_data_size) {
	if(node_data_size <= sizeof(_NodeData)) {
	    memcpy(&cursor->node->data, node_data, node_data_size);
	}
	else {
	    memcpy(cursor->node->data, node_data, node_data_size);
	}
    }
    else if(node_data_size <= sizeof(_NodeData)) {
	if(cursor->node->size > sizeof(_NodeData)) MemFree(cursor->node->data);
	memcpy(&cursor->node->data, node_data, node_data_size);
	cursor->node->size= node_data_size;
    }
    else if(node_data_size < cursor->node->size) {
	memcpy(cursor->node->data, node_data, node_data_size);
	cursor->node->size= node_data_size;
    }
    else {

	if(cursor->node->size > sizeof(_NodeData)) MemFree(cursor->node->data);
    
	if((cursor->node->data= MemNew(node_data_size)) == NULL) return FALSE;
	memcpy(cursor->node->data, node_data, node_data_size);
	cursor->node->size= node_data_size;
    }

    if(cursor->tree->nof_spies > 0) {
	notify(cursor->tree, TREE_NODEDATA_UPD_DONE, cursor->node_id,
	       cursor->node_id, node_data, node_data_size);
    }
    
    return TRUE;
}
    
/********************************************************************
 * Get node data. This function calls load_func callback if provided
 */
NLM_EXTERN VoidPtr tree_getNode(TreeCursorPtr cursor, Uint4 format, Uint2Ptr data_size)
{
    if(cursor->tree->get_func != NULL) {
	return (*(cursor->tree->get_func))(cursor, format, data_size);
    }
    else {
	VoidPtr n_data;

	*data_size= 0;

	if((n_data= MemNew(cursor->node->size)) == NULL) return NULL;
	
	if(cursor->node->size > sizeof(_NodeData)) {
	    memcpy(n_data, cursor->node->data, cursor->node->size);
	}
	else {
	    memcpy(n_data, &cursor->node->data, cursor->node->size);
	}
	*data_size= cursor->node->size;
	return n_data;
    }
}

/*********************************************************************
 * Update node data
 */
NLM_EXTERN Boolean tree_updateNode(TreeCursorPtr cursor, Uint4 format, VoidPtr node_data, Uint2 node_data_size)
{
    if(cursor->tree->update_func != NULL) {
	Boolean res;

	if(cursor->tree->nof_spies > 0) {
	    notify(cursor->tree, TREE_NODE_UPDATE, cursor->node_id,
		   cursor->node_id, node_data, node_data_size);
	}
	res= (*(cursor->tree->update_func))(cursor, format, node_data, node_data_size);

	if(cursor->tree->nof_spies > 0) {
	    notify(cursor->tree, TREE_NODE_UPD_DONE, cursor->node_id,
		   cursor->node_id, node_data, node_data_size);
	}
	return res;
    }
    else {
	return tree_updateNodeData(cursor, node_data, node_data_size);
    }
}

/***********************************************
 * Get level number for node.
 * Root is on level 0
 * Children of root are on level 1 
 * etc.
 */
NLM_EXTERN Int4 tree_getLevel(TreeCursorPtr cursor)
{
    TreeCursorPtr t_cursor;
    Int4 level;

    t_cursor= tree_openCursor(cursor->tree, NULL, NULL);
    tree_toNode(t_cursor, cursor->node_id);

    for(level= 0; tree_parent(t_cursor); level++) continue;

    tree_closeCursor(t_cursor);

    return level;
}

/***********************************************
 * Check that node pointed by cursor2 is descendant
 * of node pointed by cursor1
 */
NLM_EXTERN Boolean tree_isDescendant(TreeCursorPtr cursor1, TreeCursorPtr cursor2)
{
    TreeCursorPtr t_cursor;
    Boolean res= FALSE;

    t_cursor= tree_openCursor(cursor2->tree, NULL, NULL);
    tree_toNode(t_cursor, cursor2->node_id);

    while(tree_parent(t_cursor)) {
	if(t_cursor->node_id.idi == cursor1->node_id.idi) {
	    res= TRUE;
	    break;
	}
    }
    tree_closeCursor(t_cursor);

    return res;
}

NLM_EXTERN TreeGetFunc tree_setGetNodeFunc(TreePtr tree, TreeGetFunc GetNodeFunction)
{
    TreeGetFunc old;

    old= tree->get_func;
    tree->get_func= GetNodeFunction;
    return old;
}

NLM_EXTERN TreeUpdateFunc tree_setUpdateNodeFunc(TreePtr tree, TreeUpdateFunc UpdateNodeFunction)
{
    TreeUpdateFunc old;

    old= tree->update_func;
    tree->update_func= UpdateNodeFunction;
    return old;
}
 
NLM_EXTERN TreeSaveFunc tree_setSaveNodeFunc(TreePtr tree, TreeSaveFunc SaveNodeFunction)
{
    TreeSaveFunc old;

    old= tree->save_func;
    tree->save_func= SaveNodeFunction;
    return old;
}

NLM_EXTERN Int2 tree_addSpy(TreePtr tree, TreeSpyFunc Spy,	 VoidPtr spy_data)
{
    Int2 i;

    if(tree->nof_spies >= TREE_NOF_SPIES) return -1;

    for(i= 0; i < TREE_NOF_SPIES; i++) {
	if(tree->spy[i] == NULL) {
	    tree->nof_spies++;
	    tree->spy[i]= Spy;
	    tree->spy_data[i]= spy_data;
	    return i;
	}
    }

    tree->nof_spies= TREE_NOF_SPIES;
    return -1;
}

NLM_EXTERN VoidPtr tree_getSpyData(TreePtr tree, Int2 spy_id)
{
    return ((spy_id >= 0) && (spy_id < TREE_NOF_SPIES) && (tree->spy[spy_id] != NULL))?
	tree->spy_data[spy_id] : NULL;
}

NLM_EXTERN Boolean tree_delSpy(TreePtr tree, Int2 spy_id)
{
    if((spy_id >= 0) && (spy_id < TREE_NOF_SPIES) && (tree->spy[spy_id] != NULL)) {
	tree->nof_spies--;
	tree->spy[spy_id]= NULL;
	return TRUE;
    }

    return FALSE;
}

