/*  treemgr.h
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
* File Name:  treemgr.h
*
* Author:  Vladimir Soussov
*   
* File Description:  Manager for tree structures
*
*
* $Log: treemgr.h,v $
* Revision 1.6  1999/05/18 21:13:55  soussov
* TREE_SHUTDOWN event added
*
* Revision 1.5  1998/08/24 17:42:05  kans
* fixed old style function definition warnings
*
* Revision 1.4  1998/06/23 16:33:50  vakatov
* [WIN32,MSVC++]  Made the tree API functions DLL-exportable
*
* Revision 1.3  1998/04/02 22:30:28  soussov
* Some prototype added to make Denis happy
*
* Revision 1.2  1998/04/01 17:48:05  soussov
* changed tp include <>
*
* Revision 1.1  1998/03/31 21:22:26  sirotkin
* With Vladimir - moved from distrib/internal/taxonomy/tree
*
* Revision 6.0  1997/08/25 18:29:50  madden
* Revision changed to 6.0
*
* Revision 1.1  1997/05/30 16:16:19  soussov
* Set of files for ncbitree library
*
* Revision 1.1  1997/05/29 20:44:29  soussov
* Initial version of tree library
*
*
*/

#ifndef TREEMGR_H_DONE
#define TREEMGR_H_DONE

#include <ncbi.h>
#include <tree.h>

#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif

#ifdef __cplusplus
extern "C" {
#endif


#if 0
/* the actual enum is moved to tree.h */
typedef enum {
    TREE_CURSOR_MOVED = 0,
    TREE_NODE_UPDATE,
    TREE_NODE_UPD_DONE,
    TREE_NODEDATA_UPDATE,
    TREE_NODEDATA_UPD_DONE,
    TREE_NODE_DELETE,
    TREE_NODE_DEL_DONE,
    TREE_NODE_MOVE,
    TREE_NODE_MV_DONE,
    TREE_NODE_MERGE,
    TREE_NODE_MERGE_DONE,
    TREE_CHILDREN_MOVE,
    TREE_CHILDREN_MV_DONE,
    TREE_NODE_ADDED,
    TREE_SUBTREE_DELETE,
    TREE_SUBTREE_DEL_DONE,
    TREE_SHUTDOWN
} TreeEvent;
#endif

#define TreeNodeId _NodeId

#define NO_MEM_MSG 2001

#define TREE_NODE_FMT_DEFAULT 0
#define TREE_NODE_FMT_LABEL 1
#define TREE_NODE_FMT_REPORT 2
#define TREE_NODE_FMT_ICON 3
#define TREE_NODE_FMT_DISTANCE 4

/*=============================================
Some definition:

Tree is a set of nodes.

There is one special node in tree (root node) which has no parent. All
other nodes have one and only one parent.

Each node has integer node_id (assigned automatically) and node_data. 
Node_id is unique and can not be changed. Node_data is just array of
bytes from tree manager point of view.
*/

/**********************************************
 * Create new tree and add root node to it
 * 
 */
NLM_EXTERN TreePtr tree_new(void);

/**********************************************
 * Delete tree
 */
NLM_EXTERN Boolean tree_delete(TreePtr tree);

/***********************************************
 * Save tree in a file
 */
NLM_EXTERN Boolean tree_save(TreePtr tree, CharPtr filename);

/***********************************************
 * Restore tree from the file
 */
NLM_EXTERN TreePtr tree_restore(CharPtr filename);


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
NLM_EXTERN TreeCursorPtr tree_openCursor(TreePtr tree, VoidPtr userDataPointer, TreeCursorCBFunc callBack);

/**********************************************
 * Close cursor
 */
NLM_EXTERN void tree_closeCursor(TreeCursorPtr cursor);

/***********************************************
 * Get user data associated with a cursor
 */
NLM_EXTERN VoidPtr tree_getUserData(TreeCursorPtr cursor);


/**********************************************
 * Go to root node
 */
NLM_EXTERN Boolean tree_root(TreeCursorPtr cursor);

/**********************************************
 * Go to parent node
 */
NLM_EXTERN Boolean tree_parent(TreeCursorPtr cursor);

/***********************************************
 * Go to child node
 */
NLM_EXTERN Boolean tree_child(TreeCursorPtr cursor);

/***********************************************
 * Go to sibling
 */
NLM_EXTERN Boolean tree_sibling(TreeCursorPtr cursor);

/***********************************************
 * Add child
 * Returns: node id (integer > 0)
 * Cursor will stay on it place
 */
NLM_EXTERN TreeNodeId tree_addChild(TreeCursorPtr cursor, VoidPtr node_data, Uint2 node_data_size);

/***********************************************
 * Get node id
 */
NLM_EXTERN TreeNodeId tree_getId(TreeCursorPtr cursor);

/***********************************************
 * Get node data (for callback function)
 */
NLM_EXTERN VoidPtr tree_getNodeData(TreeCursorPtr cursor, Uint2Ptr data_size);

/***********************************************
 * Get node data
 */
NLM_EXTERN VoidPtr tree_getNode(TreeCursorPtr cursor, Uint4 format, Uint2Ptr data_size);

/***********************************************
 * Go to node
 * If node with id == node_id does not exist cursor will stay on its old place
 * If node with id == node_id has been merged cursor will point to node which
 * is result of this merging.
 */
NLM_EXTERN Boolean tree_toNode(TreeCursorPtr cursor, TreeNodeId node_id);

/***********************************************
 * Update node
 */
NLM_EXTERN Boolean tree_updateNodeData(TreeCursorPtr cursor, VoidPtr node_data, Uint2 node_data_size);

/***********************************************
 * Update node
 */
NLM_EXTERN Boolean tree_updateNode(TreeCursorPtr cursor, Uint4 format, VoidPtr node_data, Uint2 node_data_size);

/***********************************************
 * Delete node
 * All cursors which point to deleted node will be moved to parent node
 */
NLM_EXTERN Boolean tree_delNode(TreeCursorPtr cursor);

/***********************************************
 * Delete subtree
 * All cursors which point to nodes from deleted subtree will be moved to parent of 
 * this subtree
 */
NLM_EXTERN Boolean tree_delSubtree(TreeCursorPtr cursor);

/***********************************************
 * Merge nodes
 * Node with id2  == tree_getId(cursor2) will be merged with node (id1 == tree_getId(cursor1))
 * All cursors which point to id2 will point to id1. id2 will be secondary id for id1
 */
NLM_EXTERN Boolean tree_merge(TreeCursorPtr cursor1, TreeCursorPtr cursor2);

/***********************************************
 * Move node
 * Node with id2 == tree_getId(cursor2) will be child of node with id1 == tree_getId(cursor1)
 */
NLM_EXTERN Boolean tree_moveNode(TreeCursorPtr cursor1, TreeCursorPtr cursor2);

/***********************************************
 * Move children
 * All children of node with id2 == tree_getId(cursor2) will be moved to new parent
 * which id == tree_getId(cursor1)
 */
NLM_EXTERN Boolean tree_moveChildren(TreeCursorPtr cursor1, TreeCursorPtr cursor2);

/***********************************************
 * Get id of the nearest common ancestor for two nodes
 */
NLM_EXTERN TreeNodeId tree_getAncestor(TreeCursorPtr cursor1, TreeCursorPtr cursor2);

/***********************************************
 * Get level number for node.
 * Root is on level 0
 * Children of root are on level 1 
 * etc.
 */
NLM_EXTERN Int4 tree_getLevel(TreeCursorPtr cursor);

/***********************************************
 * Check that node pointed by cursor2 is descendant
 * of node pointed by cursor1
 */
NLM_EXTERN Boolean tree_isDescendant(TreeCursorPtr cursor1, TreeCursorPtr cursor2);

/***********************************************
 * Register a GetNodeFunction
 * GetNodeFunction is called by tree_getNode function. It
 * should be described as
 * VoidPtr GetNodeFunction(TreeCursorPtr cursor, Uint4 format, Uint2Ptr data_size);
 * Function should return pointer to node data.
 * format is arbitrary Uint4, but there are two predefined values for format
 * TREE_NODE_FMT_LABEL - return node label
 * TREE_NODE_FMT_REPORT - return human-readable report
 */
NLM_EXTERN TreeGetFunc tree_setGetNodeFunc(TreePtr tree, TreeGetFunc GetNodeFunction);

/***********************************************
 * Register a UpdateNodeFunction
 * UpdateNodeFunction is called by tree_updateNode function. It
 * should be described as
 * Boolean UpdateNodeFunction(TreeCursorPtr cursor, Uint4 format, VoidPtr node_data, Uint2Ptr data_size);
 * format is arbitrary Uint4, but there are two predefined values for format
 * TREE_NODE_FMT_LABEL - return node label
 * TREE_NODE_FMT_REPORT - return human-readable report
 */
NLM_EXTERN TreeUpdateFunc tree_setUpdateNodeFunc(TreePtr tree, TreeUpdateFunc UpdateNodeFunction);

/***********************************************
 * Register a SaveNodeFunction
 * SaveNodeFunction is called by tree_save function. It
 * should be described as
 * void SaveNodeFunction(TreeCursorPtr cursor);
 */
NLM_EXTERN TreeSaveFunc tree_setSaveNodeFunc(TreePtr tree, TreeSaveFunc SaveNodeFunction);

/***********************************************
 * Register a "Spy" function
 * "Spy" function is called each time when tree structure is modified
 * This function has to be defined as
 * void spy(TreePtr tree, Int2 spy_id, TreeEvent event, 
            TreeNodeId id1, TreeNodeId id2, VoidPtr node_data, Int2 node_data_size);
 * You can register up to 32 spy function for one tree
 * Return: spy_id on success
 *         -1     if 32 spies have been registred already
 */

NLM_EXTERN Int2 tree_addSpy(TreePtr tree, TreeSpyFunc spy, VoidPtr spy_data);

/***********************************************
 * Get spy's data
 */
NLM_EXTERN VoidPtr tree_getSpyData(TreePtr tree, Int2 spy_id);

/***********************************************
 * Delete "Spy" function
 */

NLM_EXTERN Boolean tree_delSpy(TreePtr tree, Int2 spy_id);

/*--------------------------------------------------------
 * Simple function which goes through tree
 *
 * static void traverse_tree_internal(TreeCursorPtr cursor);
 * {
 *    if(tree_child(cursor)) {
 *	do {
 *	    do_something_with_node(tree_getNode(cursor));
 *	    traverse_tree_internal(cursor);
 *	}
 *	while(tree_sibling(cursor));
 *
 *	tree_parent(cursor);
 *    }
 * }
 *
 * void traverse_tree(TreePtr tree)
 * {
 *    TreeCursorPtr cursor;
 *
 *    cursor= tree_openCursor(tree, NULL, NULL);
 *    
 *    do_something_with_root(tree_getNode(cursor));
 *
 *    traverse_tree_internal(cursor);
 *
 *    tree_closeCursor(cursor);
 * }
 */
 
#ifdef __cplusplus
}
#endif

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

#endif /* TREEMGR_H_DONE */
