/*  tree.h
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
* File Name:  tree.h
*
* Author:  Vladimir Soussov
*   
* File Description:  Data types for tree manager
*
*
* $Log: tree.h,v $
* Revision 1.5  1999/05/18 21:13:47  soussov
* TREE_SHUTDOWN event added
*
* Revision 1.4  1998/04/02 23:03:31  vakatov
* PC-specific fixes
*
* Revision 1.3  1998/04/02 22:30:25  soussov
* Some prototype added to make Denis happy
*
* Revision 1.2  1998/03/31 23:12:56  kans
* minor changes needed for code warrior
*
* Revision 1.1  1998/03/31 21:22:26  sirotkin
* With Vladimir - moved from distrib/internal/taxonomy/tree
*
* Revision 6.0  1997/08/25 18:29:49  madden
* Revision changed to 6.0
*
* Revision 1.1  1997/05/30 16:16:18  soussov
* Set of files for ncbitree library
*
* Revision 1.1  1997/05/29 20:44:27  soussov
* Initial version of tree library
*
*
*/

#ifndef TREE_H_DONE
#define TREE_H_DONE

#include <ncbi.h>

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

typedef struct t_nodeId0 {
    Uint1 bag_store;
    Uint1 bag;
    Uint1 node;
    Uint1 vers;
} _nodeId0;

typedef union t_nodeIdU {
    _nodeId0 id4;
    Uint4     idi;
} _NodeId, PNTR _NodeIdPtr;


typedef void* _NodeData;


#define NODE_BAG_SIZE 256
#define NOF_BAGS 256

#define TREE_FREE_ROOM 0x1
#define TREE_MERGED_NODE 0x2

typedef struct t_Node {
    _NodeId      parent;      /* parent node id */
    _NodeId      child;       /* child node id  */
    _NodeId      sibling;     /* sibling node id */
    _NodeData    data;        /* user data */
    VoidPtr      sys_data;    /* system data (can not be saved) */
    Uint2        size;
    Uint1        vers;
    Uint1        flags;
} _Node, PNTR _NodePtr;

typedef struct T_NodeBag {
    Int4               nof_nodes;    /* number of live nodes in bag */
    struct T_NodeBag** my_bag_store;
    _Node              node[NODE_BAG_SIZE];
} _NodeBag, PNTR       _NodeBagPtr;

typedef _NodeBagPtr PNTR _BagStore;

typedef struct t_TreeCursor TreeCursor, PNTR TreeCursorPtr;

typedef void (*TreeCursorCBFunc)(TreeCursorPtr, TreeEvent, _NodeId);

struct t_TreeCursor {
    struct t_Tree PNTR   tree;
    struct t_TreeCursor* next_cursor;
    _NodeId              node_id;
    _NodeId              prev_id;
    _NodePtr             node;
    TreeCursorCBFunc     callBack;
    VoidPtr              user_data;
    _NodeBagPtr          cbag;
};

typedef struct t_Tree Tree, PNTR TreePtr;

typedef void (*TreeSpyFunc)(TreePtr, Int2, TreeEvent, _NodeId, _NodeId, VoidPtr, Int2);

typedef void (*TreeSaveFunc)(TreeCursorPtr);

typedef Boolean (*TreeUpdateFunc)(TreeCursorPtr, Uint4, VoidPtr, Uint2);

typedef VoidPtr (*TreeGetFunc)(TreeCursorPtr, Uint4, Uint2Ptr);


#define TREE_NOF_SPIES 32

struct t_Tree {
    TreeCursorPtr        cursor;
    _NodeId              id4NewNode;
    _NodeId              rootNodeId;
    _BagStore            bag_store[NOF_BAGS];
    TreeGetFunc          get_func;
    TreeUpdateFunc       update_func;
    TreeSaveFunc         save_func;
    TreeSpyFunc          spy[TREE_NOF_SPIES];
    VoidPtr              spy_data[TREE_NOF_SPIES];
    Int4                 nof_spies;
};


#endif
