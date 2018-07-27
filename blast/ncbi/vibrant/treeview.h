/*  treeview.h
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
* File Name:  treeview.h
*
* Author:  Vladimir Soussov
*   
* File Description:  Data types for treeview
*
*
* $Log: treeview.h,v $
* Revision 1.6  2001/03/28 16:06:10  kans
* added prototype for tview_setPlusColor
*
* Revision 1.5  1998/04/08 16:51:52  soussov
* typo in function name fixed
*
* Revision 1.4  1998/04/01 21:16:46  soussov
* Some prototypes added
*
* Revision 1.3  1998/04/01 20:09:25  soussov
* Some prototypes added
*
* Revision 1.2  1998/04/01 17:44:02  soussov
* changed tp include <>
*
* Revision 1.1  1998/03/31 21:22:28  sirotkin
* With Vladimir - moved from distrib/internal/taxonomy/tree
*
* Revision 6.0  1997/08/25 18:29:56  madden
* Revision changed to 6.0
*
* Revision 1.1  1997/05/30 16:16:23  soussov
* Set of files for ncbitree library
*
* Revision 1.1  1997/05/29 20:44:35  soussov
* Initial version of tree library
*
*
*/

#ifndef TREEVIEW_H_DONE
#define TREEVIEW_H_DONE

#include <ncbi.h>
#include <vibrant.h>
#include <treemgr.h>
#include <treevint.h>

#if 0
typedef enum {
    TVIEW_ON_SELECT = 0,
    TVIEW_ON_OPEN,
    TVIEW_ON_CLOSE
} TViewEvent;
#endif

/***************************************************************
 * Create tree view
 * parent - parent group or window. If parent == NULL then tree view will be
 *          created in a separate resizable window.
 * width, height - size of tree view in pixels
 * tree - tree which has to be shown
 * icon_list - list of icons (if NULL - no icons will be shown
 * scale - scale for distance
 * NodeCmp - node comparison function (NULL if you don't want nodes ordering)
 *
 * If you'd like to display nodes in some particular order,
 * then you have to provide NodeCmp(TreeCursorPtr, TreeCursorPtr)
 * function. For two given nodes, this function should return 
 * negative value if first node should be first, and positive if
 * second node should be first
 *            
 *
 * NOTE!
 * To display tree in tree view you have to provide the getNode function
 * (see tree.h header) and register it using tree_setGetNodeFunc()
 * Tree viewer will run your function to get label, distance and icon id.
 * So it should return CharPtr if format == TREE_NODE_FMT_LABEL
 *                     Int4    if format == TREE_NODE_FMT_ICON || format == TREE_NODE_FMT_DISTANCE
 * If you dont want to paint icon for some node then your function has to return -1 for these nodes
 */
TreeViewPtr tview_create(GrouP parent, Int2 width, Int2 height, 
			 TreePtr tree, _ImageListPtr icon_list, Int2 scale, 
			 TreeNodeCmpFunc NodeCmp);

/***************************************************************
 * The following function works almost like tview_create(NULL, ...),
 * but provide the ability to specify the window position on the screen,
 * window title and add menu to tree viewer's window.
 *
 * Example:
 *
 * static void crtMenu(WindoW m_window)
 * {
 *   MenU m1, m2;
 *
 *   m1= PulldownMenu(m_window, "File");
 *   .....
 *   CommandItem(m1, "Exit", exit_proc);
 *   .....
 *   m2= PulldownMenu(m_window, "Edit");
 *   .....
 * }
 * .....
 * TreeViewPtr tv= tview_createCustom(10, 10, 200, 100, "My favorite tree", crtMenu, 
 *                                    tree, NULL, 30, tview_labelCmp);
 *
 */
TreeViewPtr tview_createCustom(Int2 left, Int2 top, Int2 width, Int2 height,
			       CharPtr title, void (*menuCrtFunc)(WindoW),
			 TreePtr tree, _ImageListPtr il, Int2 l_scale, 
			 TreeNodeCmpFunc NodeCmp);

/***************************************************************
 * Delete tree view
 */
void tview_delete(TreeViewPtr tv, Boolean do_hide);

/***************************************************************
 * Set tree view colors
 * Function returns TRUE on success
 */
Boolean tview_setTextColor(TreeViewPtr tv, Uint4 color);
Boolean tview_setLineColor(TreeViewPtr tv, Uint4 color);
Boolean tview_setPlusColor(TreeViewPtr tv, Uint4 color);
Boolean tview_setSelBGColor(TreeViewPtr tv, Uint4 color);
Boolean tview_setSelFGColor(TreeViewPtr tv, Uint4 color);

/***************************************************************
 * Replace NodeCmp function
 * If you'd like to display nodes in some particular order,
 * then you have to provide NodeCmp(TreeCursorPtr, TreeCursorPtr)
 * function. For two given nodes, this function should return 
 * negative value if first node should be first, and positive if
 * second node should be first
 */

TreeNodeCmpFunc tview_setNodeCmpFunc(TreeViewPtr tv, TreeNodeCmpFunc NodeCmp);

/****************************************************************
 * You can use the following two functions as a NodeCmp functions
 * The first one compares the labels
 * the second one compares the distances
 * To do this just call
 * tview_setNodeCmpFunc(tv, tview_labelCmp);
 * or
 * tview_setNodeCmpFunc(tv, tview_distanceCmp);
 */
Int2 tview_labelCmp(TreeCursorPtr c1, TreeCursorPtr c2);
Int2 tview_distanceCmp(TreeCursorPtr c1, TreeCursorPtr c2);

/*****************************************************************
 * Set event handler function for tree viewer.
 * void HandlerFunc(TreeCursorPtr cursor, TViewEvent evnt)
 * event handler will be called just before any actions
 * regarding this event will be done
 */
TreeViewEventFunc tview_setEventHandler(TreeViewPtr tv, TreeViewEventFunc HandlerFunc);

/*****************************************************************
 * Set double click function.
 * void dblClickFunc(TreeViewPtr tv, TreeNodeId id, Boolean isOverNode)
 * Your function will be called on user double click inside tree viewer panel
 * If user click over tree node, then isOverNode argument will be TRUE
 * and id will be  the id of node under cursor.
 * If there is no nodes under cursor, your function will be called with isOverNode == FALSE
 */
TreeViewClickFunc tview_setDblClickFunc(TreeViewPtr tv, TreeViewClickFunc dblClickFunc);

/*****************************************************************
 * Set shift click function.
 * void shiftClickFunc(TreeViewPtr tv, TreeNodeId id, Boolean isOverNode)
 * Your function will be called on user does shift+click inside tree viewer panel
 * If user click over tree node, then isOverNode argument will be TRUE
 * and id will be  the id of node under cursor.
 * If there is no nodes under cursor, your function will be called with isOverNode == FALSE
 */
TreeViewClickFunc tview_setShiftClickFunc(TreeViewPtr tv, TreeViewClickFunc shiftClickFunc);

/*****************************************************************
 * Set control click function.
 * void ctrlClickFunc(TreeViewPtr tv, TreeNodeId id, Boolean isOverNode)
 * Your function will be called on user does control+click inside tree viewer panel
 * If user click over tree node, then isOverNode argument will be TRUE
 * and id will be  the id of node under cursor.
 * If there is no nodes under cursor, your function will be called with isOverNode == FALSE
 */
TreeViewClickFunc tview_setCtrlClickFunc(TreeViewPtr tv, TreeViewClickFunc ctrlClickFunc);

/*****************************************************************
 * Redraw the tree
 */
void tview_redraw(TreeViewPtr tv);

/*****************************************************************
 * show selected node on the screen
 */
void tview_showSelected(TreeViewPtr tv);

/*****************************************************************
 * select node and show it on the screen
 */
Boolean tview_selectNode(TreeViewPtr tv, TreeNodeId id);

/*****************************************************************
 * open subtree and make it visible
 */
Boolean tview_openSubtree(TreeViewPtr tv, TreeNodeId id);

/*****************************************************************
 * close subtree
 */
Boolean tview_closeSubtree(TreeViewPtr tv, TreeNodeId id);

/*****************************************************************
 * Hide tree view
 */
void tview_hide(TreeViewPtr tv);

/*****************************************************************
 * Show tree view
 */
void tview_show(TreeViewPtr tv);

/*****************************************************************
 * disable tree view
 */
void tview_disable(TreeViewPtr tv);

/*****************************************************************
 * enable tree view
 */
void tview_enable(TreeViewPtr tv);

/*****************************************************************
 * Resize the tree viewer panel
 * You can use this function if you have created tree viewer
 * not as a separate window, but inside some window. 
 * r is a rectangle which describes tree panel coordinates inside
 * window
 */
void tview_resize(TreeViewPtr tv, RectPtr r);

/*****************************************************************
 * Move root of visible tree to node with node_id == id
 * You can use this function if you want to display just
 * subtree from your tree
 */
Boolean tview_moveRoot(TreeViewPtr tv, TreeNodeId id);

/*****************************************************************
 * Get selected node id
 */
TreeNodeId tview_getSelected(TreeViewPtr tv);

/*****************************************************************
 * Get TreePtr 
 */
TreePtr tview_getTreePtr(TreeViewPtr tv);

#endif
