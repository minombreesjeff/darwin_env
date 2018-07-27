/*  treevint.h
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
* File Name:  treevint.h
*
* Author:  Vladimir Soussov
*   
* File Description:  Data types for treeviewer
*
*
* $Log: treevint.h,v $
* Revision 1.6  1999/08/12 18:31:10  soussov
* fixed bug in allocNewSegm
*
* Revision 1.5  1998/04/01 21:52:50  soussov
* Prototype fixed
*
* Revision 1.4  1998/04/01 21:16:47  soussov
* Some prototypes added
*
* Revision 1.3  1998/04/01 20:09:26  soussov
* Some prototypes added
*
* Revision 1.2  1998/04/01 17:43:56  soussov
* changed tp include <>
*
* Revision 1.1  1998/03/31 21:22:29  sirotkin
* With Vladimir - moved from distrib/internal/taxonomy/tree
*
* Revision 6.0  1997/08/25 18:29:58  madden
* Revision changed to 6.0
*
* Revision 1.1  1997/05/30 16:16:24  soussov
* Set of files for ncbitree library
*
* Revision 1.1  1997/05/29 20:44:37  soussov
* Initial version of tree library
*
*
*/

#ifndef TREEVINT_H_DONE
#define TREEVINT_H_DONE

#include <ncbi.h>
#include <vibrant.h>
#include <imagelst.h>
#include <treemgr.h>

typedef struct t_box {
    Int4 yt, yb, xl, xr;
} _box, PNTR _boxPtr;

typedef struct t_point {
    Int4 x, y;
} _point, PNTR _pointPtr;

#define RESERVED_SEGM 0x1
#define TREE_FIRST_NODE 0x2
#define LINK_TO_NEXT_BUFF 0x4

typedef struct t_tnSegm {
    struct t_tnSegm* next;
    Int4             w, h;
    TreeNodeId       work;
    Uint1            viewer_id;
    Uint1            flags;
} _tnSegm, PNTR _tnSegmPtr;

typedef struct t_drawContext {
    WindoW tree_window;
    PaneL  tree_panel;
    Int2   panel_origin_X, panel_origin_Y;
    _point port_origin;
    _box   clip_box;
    Uint4   bg_color, line_color, text_color, sbg_color, sfg_color, plus_color;
    Uint1   bg_color_set, line_color_set, text_color_set, sbg_color_set, sfg_color_set, plus_color_set; 
    Int1   pen_w;
    Int1   font_id;
} _drawContext, PNTR _drawContextPtr;

typedef enum {
    TVIEW_ON_SELECT = 0,
    TVIEW_ON_OPEN,
    TVIEW_ON_CLOSE
} TViewEvent;
    
typedef Int2 (LIBCALLBACK * TreeNodeCmpFunc) PROTO((TreeCursorPtr, TreeCursorPtr));

typedef void (LIBCALLBACK * TreeViewEventFunc) PROTO((TreeCursorPtr, TViewEvent));

typedef struct t_TreeView TreeView, PNTR TreeViewPtr;

typedef void (LIBCALLBACK * TreeViewClickFunc) PROTO((TreeViewPtr, TreeNodeId, Boolean));

struct t_TreeView {
    _drawContext  draw_port;
    TreePtr       tree;
    _tnSegmPtr    segm_buff;
    TreeCursorPtr cr1, cr2, cr3, cr4, cr5;
    _ImageListPtr i_list;
    Int4          sel_x, sel_y;
    TreeNodeCmpFunc   cmpSiblings;
    TreeViewEventFunc evntFunc;
    TreeViewClickFunc dblClickFunc;
    TreeViewClickFunc shiftClickFunc;
    TreeViewClickFunc ctrlClickFunc;
    TreeNodeId    my_root;
    Int2          row_height;
    Int2          row_width;
    Int2          length_scale;
    Uint1         my_spy_id;
};


#endif
