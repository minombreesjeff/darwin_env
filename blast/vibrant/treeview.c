/*  treeview.c
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
* File Name:  treeview.c
*
* Author:  Vladimir Soussov
*   
* File Description:  treeview functions
*
*
* $Log: treeview.c,v $
* Revision 1.13  2001/03/28 14:05:47  beloslyu
* fix the comments to C style
*
* Revision 1.12  2001/03/28 01:37:40  juran
* *** empty log message ***
*
* Revision 1.11  2000/11/06 21:51:15  soussov
* adds code to prevent infinite loop in getNodeCoordinates
*
* Revision 1.10  1999/08/12 18:31:10  soussov
* fixed bug in allocNewSegm
*
* Revision 1.9  1999/08/11 14:19:42  soussov
* fixed bug with Int4 -> Int2 conversion
*
* Revision 1.8  1998/07/02 18:24:33  vakatov
* Cleaned the code & made it pass through the C++ compilation
*
* Revision 1.7  1998/04/08 16:51:49  soussov
* typo in function name fixed
*
* Revision 1.6  1998/04/02 23:03:51  vakatov
* PC-specific fixes
*
* Revision 1.5  1998/04/01 21:52:47  soussov
* Prototype fixed
*
* Revision 1.4  1998/04/01 20:14:41  soussov
* vibincld.h added
*
* Revision 1.3  1998/04/01 20:09:22  soussov
* Some prototypes added
*
* Revision 1.2  1998/04/01 17:43:59  soussov
* changed tp include <>
*
* Revision 1.1  1998/03/31 21:22:27  sirotkin
* With Vladimir - moved from distrib/internal/taxonomy/tree
*
* Revision 6.1  1998/01/13 19:18:51  soussov
* qsort for large sibling list added
*
* Revision 6.0  1997/08/25 18:29:53  madden
* Revision changed to 6.0
*
* Revision 1.1  1997/05/30 16:16:21  soussov
* Set of files for ncbitree library
*
* Revision 1.1  1997/05/29 20:44:31  soussov
* Initial version of tree library
*
*
*/

#include <treemgr.h>
#include <treeview.h>

#define TREE_X0 10
#define TREE_Y0 10

#define getTree4Cursor tree_getUserData

#define port_MoveTo(p, a, b) sMoveTo((a) + (p)->panel_origin_X, (b) + (p)->panel_origin_Y)
#define port_LineTo(p, a, b) sLineTo((a) + (p)->panel_origin_X, (b) + (p)->panel_origin_Y)

static void closeAllSubtrees(TreeCursorPtr cursor);
extern void Nlm_GetRect PROTO((Nlm_GraphiC a, Nlm_RectPtr r));

static _tnSegmPtr getNewSegmBuff(void)
{
    _tnSegmPtr ts= (_tnSegmPtr)MemNew(4096*sizeof(_tnSegm));

    if(ts) {
	memset(ts, 0, 4096*sizeof(_tnSegm));
	ts[4095].flags= LINK_TO_NEXT_BUFF;
    }
    return ts;
}

static void sMoveTo(Int4 x, Int4 y)
{
    if(x > 32000) x= 32000;
    else if(x < -32000) x= -32000;
    if(y > 32000) y= 32000;
    else if(y < -32000) y= -32000;

    MoveTo((Int2)x, (Int2)y);
}

static void sLineTo(Int4 x, Int4 y)
{
    if(x > 32000) x= 32000;
    else if(x < -32000) x= -32000;
    if(y > 32000) y= 32000;
    else if(y < -32000) y= -32000;

    LineTo((Int2)x, (Int2)y);
}

static void port_PaintRect(_drawContextPtr port, RectPtr r)
{
    r->left+= port->panel_origin_X;
    r->right+= port->panel_origin_X;
    r->top+= port->panel_origin_Y;
    r->bottom+= port->panel_origin_Y;

    PaintRect(r);
}

static void Rect2Box(_drawContextPtr p, RectPtr r, _boxPtr b)
{
    b->xl= (r->left - p->panel_origin_X) + p->port_origin.x;
    b->xr= (r->right - p->panel_origin_X) + p->port_origin.x;
    b->yt= (r->top - p->panel_origin_Y) + p->port_origin.y;
    b->yb= (r->bottom - p->panel_origin_Y) + p->port_origin.y;
}

static void Box2Rect(_drawContextPtr p, _boxPtr b, RectPtr r)
{
    r->left= p->panel_origin_X + (b->xl - p->port_origin.x);
    r->right= p->panel_origin_X + (b->xr - p->port_origin.x);
    r->top= p->panel_origin_Y + (b->yt - p->port_origin.y);
    r->bottom= p->panel_origin_Y + (b->yb - p->port_origin.y);
}

static CharPtr getNodeLabel(TreeCursorPtr cursor)
{
    Uint2 size;
    CharPtr label;

    label= (CharPtr) tree_getNode(cursor, TREE_NODE_FMT_LABEL, &size);
    if(label != NULL) label[size]= '\0';
    return label;
}

static Int4 getNodeLength(TreeCursorPtr cursor)
{
    Uint2 size;
    Int4 length;

    length= (Int4)tree_getNode(cursor, TREE_NODE_FMT_DISTANCE, &size);
    return (length <= 0)? 1 : length;
}

static Int2 getIconId(TreeCursorPtr cursor)
{
    Uint2 size;
    Int4 a;
    Int2 id;

    a= (Int4) tree_getNode(cursor, TREE_NODE_FMT_ICON, &size);
    id= (Int2)a;

    return id;
}
    
#ifdef XXX_UNUSED_STATIC_FUNC
static void addBox2Region(_boxPtr region, _boxPtr box)
{
    if(box->xl < region->xl) region->xl= box->xl;
    if(box->xr > region->xr) region->xr= box->xr;
    if(box->yt < region->yt) region->yt= box->yt;
    if(box->yb > region->yb) region->yb= box->yb;
}
#endif /* XXX_UNUSED_STATIC_FUNC */

static Boolean BoxXRegion(_boxPtr region, _boxPtr box, _boxPtr res)
{
    if((box->yt >= region->yb) || (box->yb <= region->yt)) return FALSE;
    if((box->xl >= region->xr) || (box->xr <= region->xl)) return FALSE;

    if(res != NULL) {
	res->xl= (box->xl > region->xl)? box->xl : region->xl;
	res->xr= (box->xr < region->xr)? box->xr : region->xr;
	res->yt= (box->yt > region->yt)? box->yt : region->yt;
	res->yb= (box->yb < region->yb)? box->yb : region->yb;
    }

    return TRUE;
}

#ifdef XXX_UNUSED_STATIC_FUNC
static void moveBox(_boxPtr box, Int4 dx, Int4 dy)
{
    
    if(dx != 0) {
	box->xl+= dx;
	box->xr+= dx;
    }
    if(dy != 0) {
	box->yt+= dy;
	box->yb+= dy;
    }
}
#endif /* XXX_UNUSED_STATIC_FUNC */

#ifdef XXX_UNUSED_STATIC_FUNC
static void zoomBox(_boxPtr box, Int4 dx, Int4 dy)
{
    if(dx != 0) {
	box->xl-= dx;
	box->xr+= dx;
    }

    if(dy != 0) {
	box->yt-= dy;
	box->yb+= dy;
    }
}
#endif /* XXX_UNUSED_STATIC_FUNC */

static Boolean ptInBox(_boxPtr box, Int4 x, Int4 y)
{
    return ((y >= box->yt) && (y <= box->yb) && (x >= box->xl) && (x <= box->xr));
}

static void freeNodeSegm(TreeCursorPtr cursor)
{
    _tnSegmPtr ts1;
    _tnSegmPtr ts2;
    TreeViewPtr tv;

    tv= (TreeViewPtr) getTree4Cursor(cursor);
    ts1= (_tnSegmPtr)(cursor->node->sys_data);

    if(ts1 == NULL) return;

    if(ts1->viewer_id == tv->my_spy_id) {
	cursor->node->sys_data= ts1->next;
	ts2= ts1;
    }
    else {
	for(ts2= ts1->next; ts2 != NULL; ts2= ts2->next) {
	    if(ts2->viewer_id == tv->my_spy_id) {
		ts1->next= ts2->next;
		break;
	    }
	    else {
		ts1= ts2;
	    }
	}
    }

    if(ts2 != NULL) {
	ts2->flags= 0;
    }
}

static _tnSegmPtr allocNewSegm(TreeCursorPtr cursor)
{
    _tnSegmPtr ts;
    _tnSegmPtr tsn;
    TreeViewPtr tv;
    /* Int4 i, j; */

    /*find free room */
    tv= (TreeViewPtr) getTree4Cursor(cursor);
    ts= tv->segm_buff;

    while(ts) {
	if(ts->flags == 0) {
	    ts->flags= RESERVED_SEGM;
	    ts->next= (_tnSegmPtr)cursor->node->sys_data;
	    cursor->node->sys_data= ts;
	    ts->viewer_id= tv->my_spy_id;
	    return ts;
	}
	if((ts->flags & LINK_TO_NEXT_BUFF) != 0) {
	    if(ts->next) {
		ts= ts->next;
		continue;
	    }
	    else break;
	}
	++ts;
    }

    if(ts == NULL) return NULL;
    /* try to allocate more memory */
    if((tsn= getNewSegmBuff()) == NULL) {
	return NULL;
    }
    
    tsn->flags= RESERVED_SEGM;
    tsn->viewer_id= tv->my_spy_id;
    ts->next= tsn;
    cursor->node->sys_data= tsn;
    
    return tsn;
}
    
    

static Int4 getIconWidth(_ImageListPtr ilp, Uint2 i_id)
{
    Uint2 w= 0, h;

    if(ilp != NULL) {
	ilst_size(ilp, i_id, &w, &h);
    }
    return (Int4)w;
}
    

static Int4 calcNodeWidth(TreeViewPtr tv, Int2 icon_id, Int4 length, CharPtr label)
{
    return ((tv->length_scale >= 0)? (tv->length_scale * length) : (length/(-tv->length_scale))) +
	(((tv->i_list != NULL) && (icon_id >= 0))? getIconWidth(tv->i_list, icon_id) : 0) +
	(((label != NULL) && (*label != '\0'))? (StringWidth(label) + 4) : 0);
}

static _tnSegmPtr getNodeViewerData(_tnSegmPtr tn, Uint1 v_id)
{
    while(tn != NULL) {
	if(tn->viewer_id == v_id) return tn;
	tn= tn->next;
    }
    return NULL;
}

static Boolean getBox4Cursor(TreeCursorPtr cursor, _boxPtr box)
{
    _tnSegmPtr node_data;
    TreeViewPtr tv;

    tv= (TreeViewPtr) getTree4Cursor(cursor);
    if((node_data= getNodeViewerData((_tnSegmPtr)cursor->node->sys_data, tv->my_spy_id)) != NULL) {
	box->xr+= node_data->w;
	box->yb+= node_data->h;
	return TRUE;
    }

    return FALSE;
}

static TreeNodeId getNextSibling(TreeCursorPtr cursor, TreeNodeId nxt_id)
{
    Boolean r;
    _tnSegmPtr node_data;
    TreeViewPtr tv;

    tv= (TreeViewPtr)getTree4Cursor(cursor);

    if(nxt_id.idi == cursor->node_id.idi) {
	nxt_id.idi= 0;
	return nxt_id;
    }

    if(nxt_id.idi != 0) {
	tree_toNode(cursor, nxt_id);
	if((node_data= getNodeViewerData((_tnSegmPtr)cursor->node->sys_data, tv->my_spy_id)) != NULL) {
	    return node_data->work;
	}
	else {
	    nxt_id.idi= 0;
	    return nxt_id;
	}
    }

    /* find the first one */

    if(tree_parent(cursor)) {

	for(r= tree_child(cursor); r; r= tree_sibling(cursor)) {
	    if((node_data= getNodeViewerData((_tnSegmPtr)cursor->node->sys_data, tv->my_spy_id)) != NULL) {
		if((node_data->flags & TREE_FIRST_NODE) != 0) return node_data->work;
	    }
	}
    }

    nxt_id.idi= 0;
    return nxt_id;
}
	
static void draw_line(_drawContextPtr port, Int4 x1, Int4 y1, Int4 x2, Int4 y2)
{
    if(y1 == y2) {
	/* horisontal line */
	if(((y1 + port->pen_w) <= port->clip_box.yt) ||
	   ((y1 - port->pen_w) >= port->clip_box.yb)) return;

	if(x1 > x2) {
	    y2= x1;
	    x1= x2;
	    x2= y2;
	    y2= y1;
	}

	if(((x2 + port->pen_w) <= port->clip_box.xl) ||
	   ((x1 - port->pen_w) >= port->clip_box.xr)) return;
    }
    else if(x1 == x2) {
	/* vertical line */
	if(((x1 + port->pen_w) <= port->clip_box.xl) ||
	   ((x1 - port->pen_w) >= port->clip_box.xr)) return;

	if(y1 > y2) {
	    x2= y1;
	    y1= y2;
	    y2= x2;
	    x2= x1;
	}

	if(((y1 - port->pen_w) >= port->clip_box.yb) ||
	   ((y2 + port->pen_w) <= port->clip_box.yt)) return;
    }

    /* adjust coordinates */
    x1-= port->port_origin.x;
    x2-= port->port_origin.x;
    y1-= port->port_origin.y;
    y2-= port->port_origin.y;

    port_MoveTo(port, x1, y1);
    /* TODO add here the pen settings */
    SetColor(port->line_color);
    port_LineTo(port, x2, y2);
}

#if 0	
static void erase_line(_drawContextPtr port, Int4 x1, Int4 y1, Int4 x2, Int4 y2)
{
    if(y1 == y2) {
	/* horisontal line */
	if(((y1 + port->pen_w) <= port->clip_box.yt) ||
	   ((y1 - port->pen_w) >= port->clip_box.yb)) return;

	if(x1 > x2) {
	    y2= x1;
	    x1= x2;
	    x2= y2;
	    y2= y1;
	}

	if(((x2 + port->pen_w) <= port->clip_box.xl) ||
	   ((x1 - port->pen_w) >= port->clip_box.xr)) return;
    }
    else if(x1 == x2) {
	/* vertical line */
	if(((x1 + port->pen_w) <= port->clip_box.xl) ||
	   ((x1 - port->pen_w) >= port->clip_box.xr)) return;

	if(y1 > y2) {
	    x2= y1;
	    y1= y2;
	    y2= x2;
	    x2= x1;
	}

	if(((y1 - port->pen_w) >= port->clip_box.yb) ||
	   ((y2 + port->pen_w) <= port->clip_box.yt)) return;
    }

    /* adjust coordinates */
    x1-= port->port_origin.x;
    x2-= port->port_origin.x;
    y1-= port->port_origin.y;
    y2-= port->port_origin.y;

    port_MoveTo(port, x1, y1);
    /* TODO add here the pen settings */
    Black();
    EraseMode();
    port_LineTo(port, x2, y2);
    CopyMode();
}
#endif

static void draw_plus(_drawContextPtr port, Int4 x, Int4 y)
{
    RecT rect;
    Int2 radius;

    radius= port->pen_w + 3;

    if(((y + radius) < port->clip_box.yt) ||
       ((y - radius) > port->clip_box.yb) ||
       ((x - radius) > port->clip_box.xr) ||
       ((x + radius) < port->clip_box.xl)) return;

    x-= port->port_origin.x;
    y-= port->port_origin.y;

    rect.right=  (Int2)(x + radius);
    rect.left=   (Int2)(x - radius);
    rect.top=    (Int2)(y - radius);
    rect.bottom= (Int2)(y + radius);

    SetColor(port->plus_color);
    Solid();
    port_PaintRect(port, &rect);
    SetColor(port->bg_color);
    port_MoveTo(port, x, y - radius + 1);
    port_LineTo(port, x, y + radius - 1);
    port_MoveTo(port, x - radius + 1, y);
    port_LineTo(port, x + radius - 1, y);
}
    
static void draw_minus(_drawContextPtr port, Int4 x, Int4 y)
{
    RecT rect;
    Int2 radius;

    radius= port->pen_w + 3;

    if(((y + radius) < port->clip_box.yt) ||
       ((y - radius) > port->clip_box.yb) ||
       ((x - radius) > port->clip_box.xr) ||
       ((x + radius) < port->clip_box.xl)) return;

    x-= port->port_origin.x;
    y-= port->port_origin.y;

    rect.right=  (Int2)(x + radius);
    rect.left=   (Int2)(x - radius);
    rect.top=    (Int2)(y - radius);
    rect.bottom= (Int2)(y + radius);

    SetColor(port->plus_color);
    Solid();
    port_PaintRect(port, &rect);
    SetColor(port->bg_color);
    port_MoveTo(port, x - radius + 1, y);
    port_LineTo(port, x + radius - 1, y);
}

static void draw_label(_drawContextPtr port, CharPtr label, Int4 x, Int4 y)
{
    
    if(x >= port->clip_box.xr) return;

    /* TODO insert here pen and font settings */
    SetColor(port->text_color);

    y+= FontHeight()/2 - Descent();
    x-= port->port_origin.x;
    y-= port->port_origin.y;
    port_MoveTo(port, x, y);
    PaintChar(' ');
    PaintString(label);
}
    
static void draw_icon(_drawContextPtr port, _ImageListPtr il, Uint2 i_ind, Int4 x, Int4 y)
{
    Uint2 w, h;

    if(!ilst_size(il, i_ind, &w, &h)) {
	w= h= 0;
    }

    x-= w/2 + 1;
    y-= h/2 + 1;

    if((y > port->clip_box.yb) ||
       ((y + h) < port->clip_box.yt) ||
       (x > port->clip_box.xr) ||
       ((x + w) < port->clip_box.xl)) return;

    x-= port->port_origin.x;
    y-= port->port_origin.y;
    ilst_draw(il, i_ind, x + port->panel_origin_X, y + port->panel_origin_Y);
}

static void highLightBox(_drawContextPtr port, _boxPtr node_box, CharPtr label)
{
    RecT r;
    Int4 x, y;

    r.left= node_box->xl - port->port_origin.x;
    r.right= node_box->xr - port->port_origin.x;
    r.top= node_box->yt - port->port_origin.y;
    r.bottom= node_box->yb - port->port_origin.y;

    InsetRect(&r, 1, 1);
    r.right+= 1;
    SetColor(port->sbg_color);
    port_PaintRect(port, &r);
    SetColor(port->sfg_color);

    x= node_box->xl;
    y= ((node_box->yt + node_box->yb)/2) + FontHeight()/2 - Descent();
    x-= port->port_origin.x;
    y-= port->port_origin.y;
    port_MoveTo(port, x, y);
    PaintChar(' ');
    if(label != NULL) PaintString(label);
    /*InvertRect(&r);*/    
}

static void unHighLightBox(_drawContextPtr port, _boxPtr node_box, CharPtr label)
{
    RecT r;
    Int4 x, y;

    r.left= node_box->xl - port->port_origin.x;
    r.right= node_box->xr - port->port_origin.x;
    r.top= node_box->yt - port->port_origin.y;
    r.bottom= node_box->yb - port->port_origin.y;

    InsetRect(&r, 1, 1);
    r.right+= 1;
    SetColor(port->bg_color);
    port_PaintRect(port, &r);
    SetColor(port->text_color);

    x= node_box->xl;
    y= ((node_box->yt + node_box->yb)/2) + FontHeight()/2 - Descent();
    x-= port->port_origin.x;
    y-= port->port_origin.y;
    port_MoveTo(port, x, y);
    PaintChar(' ');
    if(label != NULL) PaintString(label);
    /*InvertRect(&r);*/    
}

/*******************************************
 * set clip box on whole panel
 */
static void setPort2Panel(_drawContextPtr my_port)
{
    RecT r;

    ObjectRect(my_port->tree_panel, &r);

    /* convert it to panel coordinates */
    Rect2Box(my_port, &r, &my_port->clip_box);
}

static void drawVertical(TreeCursorPtr cursor, Int4 x0, Int4 y0)
{
    if(tree_child(cursor)) {
	TreeViewPtr tv= (TreeViewPtr) getTree4Cursor(cursor);
	Int4 y1= y0;
	Int4 dy= tv->row_height;
	_tnSegmPtr tns;

	if(tv->cmpSiblings == NULL) {
	    /* we don't want to sort siblings */
	    do {
		y1+= dy;
		if((tns= getNodeViewerData((_tnSegmPtr)cursor->node->sys_data, tv->my_spy_id)) != NULL) {
		    dy= tns->h;
		}
		else dy= 0;
	    }
	    while (tree_sibling(cursor));
	}
	else {/* siblings have to be sorted */
	    TreeNodeId id;

	    for(id.idi= 0; (id= getNextSibling(cursor, id)).idi != 0;) {
		y1+= dy;
		if((tns= getNodeViewerData((_tnSegmPtr)cursor->node->sys_data, tv->my_spy_id)) != NULL) {
		    dy= tns->h;
		}
		else dy= 0;
	    }
	}

	tree_parent(cursor);
	draw_line(&tv->draw_port, x0, y0, x0, y1);
    }
}
	

static Int4 draw_node(TreeCursorPtr cursor, Int4 x0, Int4 y0)
{
    TreeViewPtr tv;
    _box my_box;
    _box node_box;
    Int4 ym, xm, xt, yd;
    Int2 icon_id;
    Int4 length;
    CharPtr label;
    Boolean need_select= FALSE;

    my_box.xl= my_box.xr= x0;
    my_box.yt= my_box.yb= y0;
    if(!getBox4Cursor(cursor, &my_box)) return x0;
    tv= (TreeViewPtr) getTree4Cursor(cursor);
    icon_id= (tv->i_list == NULL)? -1 : getIconId(cursor);
    length= getNodeLength(cursor);
    label= getNodeLabel(cursor);

    
    node_box.xl= x0;
    node_box.yt= y0;
    node_box.yb= y0 + tv->row_height;
    node_box.xr= x0 + calcNodeWidth(tv, icon_id, length, label);
		
    /* calculate the mediane */
    ym= y0 + tv->row_height/2;

    xm= x0 + 
	((tv->length_scale >= 0)? 
	 (tv->length_scale * length) : (length/(-tv->length_scale)));

    yd= my_box.yb - tv->row_height/2;

    if(BoxXRegion(&tv->draw_port.clip_box, &node_box, NULL)) {
	/* we have visible part */

	/* check for selected node */
	if(cursor->node_id.idi == tv->cr2->node_id.idi) {
	    need_select= TRUE;
	    tv->sel_x= x0;
	    tv->sel_y= y0;
	}
	
	/* draw horisontal line */
	draw_line(&tv->draw_port, x0, ym, xm, ym);

	/* draw vertical line */
	if((yd - ym) > tv->row_height/2) {
	    drawVertical(cursor, xm, ym);
	    draw_minus(&tv->draw_port, x0, ym);
	}
	else if(tree_child(cursor)) {
	    draw_plus(&tv->draw_port, x0, ym);
	    tree_parent(cursor);
	}
	    
	if(icon_id >= 0) {
	    /* draw icon */
	    xt= xm + getIconWidth(tv->i_list, icon_id)/2;
	    draw_icon(&tv->draw_port, tv->i_list, icon_id, xm, ym);
	}
	else xt= xm;

	if(label != NULL) {
	    /* draw label */
	    if(need_select) {
		node_box.xl= xt;
		highLightBox(&tv->draw_port, &node_box, label);
	    }
	    else draw_label(&tv->draw_port, label, xt, ym);
	}
    }
    else {
	/* node itself invisible, but we can see it's children */
	
	/* draw vertical line */
	drawVertical(cursor, xm, ym);
	/*
	if((yd - ym) > tv->row_height/2) {
	    draw_line(&tv->draw_port, xm, ym, xm, yd);
	}
	*/
    }
    return xm;
}
	

static Boolean drawTree(TreeCursorPtr cursor, Int4 x0, Int4 y0)
{
    _box my_box;
    TreeViewPtr tv;

    my_box.xl= my_box.xr= x0;
    my_box.yt= my_box.yb= y0;

    if(!getBox4Cursor(cursor, &my_box)) return FALSE; /* this node is invisible */
    tv= (TreeViewPtr) getTree4Cursor(cursor);

    if(BoxXRegion(&tv->draw_port.clip_box, &my_box, NULL)) {
	/* we have visible part */
	x0= draw_node(cursor, x0, y0);
	y0+= tv->row_height;
	if(tree_child(cursor)) {

	    if(tv->cmpSiblings == NULL) {
		/* we don't want to sort siblings */
		do {
		    if(drawTree(cursor, x0, y0)) {
			my_box.xl= my_box.xr= x0;
			my_box.yt= my_box.yb= y0;
			getBox4Cursor(cursor, &my_box);
			y0= my_box.yb;
			if(y0 > tv->draw_port.clip_box.yb) break;
		    }
		}
		while (tree_sibling(cursor));
	    }
	    else {/* siblings have to be sorted */
		TreeNodeId id;

		for(id.idi= 0; (id= getNextSibling(cursor, id)).idi != 0;) {
		    if(drawTree(cursor, x0, y0)) {
			my_box.xl= my_box.xr= x0;
			my_box.yt= my_box.yb= y0;
			getBox4Cursor(cursor, &my_box);
			y0= my_box.yb;
			if(y0 > tv->draw_port.clip_box.yb) break;
		    }
		}
	    }

	    tree_parent(cursor);
	}
    }
    return TRUE;
}

/*------------------------------------------------------
 * Redraw tree fragment in current port
 */

static void redrawInPort(TreeViewPtr tv)
{
    RecT r;

    Box2Rect(&tv->draw_port, &tv->draw_port.clip_box, &r);

    InsetRect(&r, 1, 1);
    InvalRect(&r);
    /*FrameRect(&r);*/
    Update();
}

static Boolean makeNodeVisible(TreeCursorPtr cursor)
{
    TreeViewPtr tv;
    _box my_box;
    _tnSegmPtr tsp;

    tv= (TreeViewPtr)getTree4Cursor(cursor);
    if(getNodeViewerData((_tnSegmPtr)cursor->node->sys_data, tv->my_spy_id) != NULL) return TRUE;

    /* we need to open node */
    tsp= allocNewSegm(cursor);
    if(tsp == NULL) {
	return FALSE;
    }
    else {
	Int2 icon_id;
	Int4 length, xm, ym;
	CharPtr label;
	
	icon_id= (tv->i_list == NULL)? -1 : getIconId(cursor);
	length= getNodeLength(cursor);
	label= getNodeLabel(cursor);

	tsp->h= tv->row_height;

	tsp->w= calcNodeWidth(tv, icon_id, length, label);


	
	if(tree_child(cursor)) {
	    
	    /* adjust with children's boxes if visable */
	    xm= ((tv->length_scale >= 0)? 
		 (tv->length_scale * length) : (length/(-tv->length_scale)));
    
	    ym= tv->row_height;
	    /* we don't need to sort siblings  here */
	    do {
		my_box.xl= my_box.xr= xm;
		my_box.yt= my_box.yb= ym;

		if(getBox4Cursor(cursor, &my_box)) {
		    ym= my_box.yb;
		    if(tsp->w < my_box.xr) tsp->w= my_box.xr;
		}
	    }
	    while (tree_sibling(cursor));
	    
	    tsp->h= ym;
	    tree_parent(cursor);
	}
    }
    return TRUE;
}	

static void adjust_parentSize(TreeCursorPtr cursor)
{
    TreeViewPtr tv;
    _box my_box;
    _tnSegmPtr tsp;
    TreeNodeId nid= tree_getId(cursor);

    if(tree_parent(cursor)) {
	tv= (TreeViewPtr) getTree4Cursor(cursor);
	if((tsp= getNodeViewerData((_tnSegmPtr)cursor->node->sys_data, tv->my_spy_id)) != NULL) {
	    Int2 icon_id;
	    Int4 length, xm, ym;
	    CharPtr label;
	
	    icon_id= (tv->i_list == NULL)? -1 : getIconId(cursor);
	    length= getNodeLength(cursor);
	    label= getNodeLabel(cursor);

	    tsp->h= tv->row_height;

	    tsp->w= calcNodeWidth(tv, icon_id, length, label);


	
	    if(tree_child(cursor)) {
	    
		/* adjust with children's boxes if visable */
		xm= ((tv->length_scale >= 0)? 
		     (tv->length_scale * length) : (length/(-tv->length_scale)));
    
		ym= tv->row_height;
		/* we don't need to sort siblings  here */
		do {
		    my_box.xl= my_box.xr= xm;
		    my_box.yt= my_box.yb= ym;

		    if(getBox4Cursor(cursor, &my_box)) {
			ym= my_box.yb;
			if(tsp->w < my_box.xr) tsp->w= my_box.xr;
		    }
		}
		while (tree_sibling(cursor));
	    
		tsp->h= ym;
		tree_parent(cursor);
	    }
	    adjust_parentSize(cursor);
	}
	tree_toNode(cursor, nid);
    }
}

static TreeCursorPtr sortCursor;

typedef int (*QsortCmpFunc)(const void *, const void *);

static int my_cmpSibl(TreeNodeId* a, TreeNodeId* b)
{
    TreeViewPtr tv= (TreeViewPtr) getTree4Cursor(sortCursor);

    tree_toNode(sortCursor, *b);
    tree_toNode(tv->cr3, *a);
    return (*(tv->cmpSiblings))(tv->cr3, sortCursor);
}

/* sort siblings */
static void sortSiblings(TreeCursorPtr cursor, Int2 n)
{
    TreeViewPtr tv= (TreeViewPtr)getTree4Cursor(cursor);
    _tnSegmPtr tns;
    int i;
    TreeNodeId nid, save_id= cursor->node_id;
    TreeNodeId* aref;

    if(!tree_child(cursor)) return;

    /* create the array of pointers to viewer structure */
    if(n < 2) {
	/* we have just one child - nothing to sort */
	if((tns= getNodeViewerData((_tnSegmPtr)cursor->node->sys_data, tv->my_spy_id)) != NULL) {
	    tns->work= cursor->node_id;
	    tns->flags|= TREE_FIRST_NODE;
	}
	tree_parent(cursor);
	return;
    }

    for(i= 0; i < n;) {
	if((tns= getNodeViewerData((_tnSegmPtr)cursor->node->sys_data, tv->my_spy_id)) != NULL) {
	    tns->work.idi= 0;
	    tns->flags&= ~TREE_FIRST_NODE;
	}
	if(!tree_sibling(cursor)) break;
    }

    tree_parent(cursor);
    tree_child(cursor);
    if(tv->cr3 == NULL) {
	tv->cr3= tree_openCursor(tv->tree, tv, NULL);
    }

    /* sort it */
#if 1
    if((n > 132) && ((aref= (TreeNodeId*)malloc(sizeof(TreeNodeId)*n)) != NULL)) {
	
	sortCursor= cursor;

	i= 0;
	do {
	    if(i < n) aref[i++]= tree_getId(cursor);
	}
	while(tree_sibling(cursor));
	qsort((void*)aref, i, sizeof(TreeNodeId), (QsortCmpFunc)my_cmpSibl);

	tree_toNode(cursor, aref[0]);
	if((tns= getNodeViewerData((_tnSegmPtr)cursor->node->sys_data, tv->my_spy_id)) != NULL) {
	    tns->flags|= TREE_FIRST_NODE;
	}

	for(n= 1; n < i; n++) {
	    tree_toNode(cursor, aref[n-1]);
	    if((tns= getNodeViewerData((_tnSegmPtr)cursor->node->sys_data, tv->my_spy_id)) != NULL) {
		tns->work= aref[n];
	    }
	}

	tree_toNode(cursor, aref[i-1]);
	if((tns= getNodeViewerData((_tnSegmPtr)cursor->node->sys_data, tv->my_spy_id)) != NULL) {
	    tns->work= aref[i-1];
	}
	
	free(aref);
	tree_toNode(cursor, save_id);
	return;
    }

#endif
    nid.idi= 0;
    for(i= 0; i < n; i++) {
	/* find the minimum */
	do {
	    if(((tns= getNodeViewerData((_tnSegmPtr)cursor->node->sys_data, tv->my_spy_id)) != NULL) && 
	       (tns->work.idi == 0)) break;
	}
	while(tree_sibling(cursor));

	tree_toNode(tv->cr3, tree_getId(cursor));
	while(tree_sibling(tv->cr3)) {
	    if(((tns= getNodeViewerData((_tnSegmPtr)tv->cr3->node->sys_data, tv->my_spy_id)) != NULL) && 
	       (tns->work.idi == 0) && ((*(tv->cmpSiblings))(tv->cr3, cursor) < 0)) {

		tree_toNode(cursor, tree_getId(tv->cr3));
	    }
	}


	if(((tns= getNodeViewerData((_tnSegmPtr)cursor->node->sys_data, tv->my_spy_id)) != NULL) && 
	   (tns->work.idi == 0)) {

	    tns->work= tree_getId(cursor);
	    if(nid.idi == 0) {
		tns->flags|= TREE_FIRST_NODE;
	    }
	    else {
		tree_toNode(tv->cr3, nid);
		tns= getNodeViewerData((_tnSegmPtr)tv->cr3->node->sys_data, tv->my_spy_id);
		tns->work= tree_getId(cursor);
	    }
	    nid= tree_getId(cursor);
	}

	tree_parent(cursor);
	tree_child(cursor);
    }
    tree_toNode(cursor, save_id);
}    
    
/* open subtree */    
static Boolean openSubtree(TreeCursorPtr cursor, Boolean do_callBack)
{
    TreeViewPtr tv;
    _tnSegmPtr node_data;
    _box node_box;
    Int4 length, xm, ym;
    Int2 n= 0;
    TreeNodeId save_id= cursor->node_id;


    tv= (TreeViewPtr)getTree4Cursor(cursor);
    if((node_data= getNodeViewerData((_tnSegmPtr)cursor->node->sys_data, tv->my_spy_id)) == NULL) {
	return FALSE;
    }

    if(do_callBack && (tv->evntFunc != NULL)) {
	(*(tv->evntFunc))(cursor, TVIEW_ON_OPEN);
	Select(tv->draw_port.tree_panel);
	tree_toNode(cursor, save_id);
    }

    length= getNodeLength(cursor);

    node_data->h= tv->row_height;

    node_data->w= calcNodeWidth(tv, (tv->i_list == NULL)? -1 : getIconId(cursor), 
				length, getNodeLabel(cursor));

    xm= ((tv->length_scale >= 0)? 
	 (tv->length_scale * length) : (length/(-tv->length_scale)));
    
    ym= tv->row_height;

    if(!tree_child(cursor)) return FALSE;

    /* we don't need to sort siblings */
    do {
	if(makeNodeVisible(cursor)) {
	    node_box.xl= node_box.xr= xm;
	    node_box.yt= node_box.yb= ym;
	    if(getBox4Cursor(cursor, &node_box)) {
		ym= node_box.yb;
		if(node_data->w < node_box.xr) node_data->w= node_box.xr;
	    }
	    ++n;
	}
    }
    while (tree_sibling(cursor));


    node_data->h= ym;

    tree_parent(cursor);

    if(tv->cmpSiblings != NULL) {
	if(n > 100) WatchCursor();
	sortSiblings(cursor, n);
	if(n > 100) ArrowCursor();
    }

    adjust_parentSize(cursor);

    return TRUE;
}

static Boolean closeSubtree(TreeCursorPtr cursor, Boolean do_callBack)
{
    TreeViewPtr tv;
    _tnSegmPtr tsp;
    Int2 icon_id;
    Int4 length;
    CharPtr label;
    TreeNodeId nid= cursor->node_id;


    tv= (TreeViewPtr) getTree4Cursor(cursor);
    if((tsp= getNodeViewerData((_tnSegmPtr)cursor->node->sys_data, tv->my_spy_id)) == NULL) {
	return FALSE;
    }

    if(do_callBack && (tv->evntFunc != NULL)) {
	(*(tv->evntFunc))(cursor, TVIEW_ON_CLOSE);
	Select(tv->draw_port.tree_panel);
	tree_toNode(cursor, nid);
    }

    /* check if selected node belongs to subtree to close */
    if(cursor->node_id.idi != tv->cr2->node_id.idi) {
	if(tree_isDescendant(cursor, tv->cr2)) {
	    /* it is, so move select to cursor */
	    tree_toNode(tv->cr2, cursor->node_id);
	    tv->sel_y= -1024;
	}
    }

    icon_id= (tv->i_list == NULL)? -1 : getIconId(cursor);
    length= getNodeLength(cursor);
    label= getNodeLabel(cursor);

    tsp->h= tv->row_height;

    tsp->w= calcNodeWidth(tv, icon_id, length, label);

    if(tree_child(cursor)) {
	do {
	    freeNodeSegm(cursor);
	}
	while (tree_sibling(cursor));
	tree_parent(cursor);
    }

    adjust_parentSize(cursor);


    return TRUE;
}

/*****************************************************************************
*
*   TreeVScrollProc (sb, viewer, newval, oldval)
*
*****************************************************************************/

static void TreeVScrollProc (BaR sb, SlatE viewer, Int2 newval, Int2 oldval)

{
    TreeViewPtr  tv;
    Int2         height;
    Int4         offsetY;
    Int2         pixels;
    RecT         r;
    Int2         visLines;

    if (viewer != NULL && oldval != newval) {
	if (Visible(viewer) && AllParentsVisible (viewer)) {
	    GetPanelExtra((PaneL) viewer, &tv);
	    ObjectRect(viewer, &r);
	    InsetRect (&r, 1, 1);
	    height = r.bottom - r.top;
	    
	    visLines = (height) / tv->row_height;

	    Select (viewer);
	    if((newval > oldval && newval - visLines + 1 <= oldval) ||
	       (newval < oldval && newval + visLines - 1 >= oldval)) {
		pixels = (newval - oldval) * tv->row_height;
		if (ABS(pixels) < height) {
		    ScrollRect (&r, 0, -pixels);
		    if (pixels > 0) {
			r.top = r.bottom - pixels;
		    } 
		    else {
			r.bottom = r.top - pixels;
		    }
		    InsetRect (&r, -1, -1);
		    InvalRect (&r);
		} 
		else {
		    InsetRect (&r, -1, -1);
		    InvalRect (&r);
		}
	    } 
	    else {
		InsetRect (&r, -1, -1);
		InvalRect (&r);
	    }

	    offsetY= tv->row_height * (Int4) (newval - oldval);
	    tv->draw_port.port_origin.y+= offsetY;

	    /* calculate clip box for given rect  */
	    Rect2Box(&tv->draw_port, &r, &tv->draw_port.clip_box);

	    Update ();

	    setPort2Panel(&tv->draw_port); /* resize clip box to whole panel */
	}
    }
}

/*****************************************************************************
*
*   TreeHScrollProc (sb, viewer, newval, oldval)
*
*****************************************************************************/

static void TreeHScrollProc (BaR sb, SlatE viewer, Int2 newval, Int2 oldval)

{
    TreeViewPtr  tv;
    Int4       lineWidth;
    Int4       offsetX;
    Int2       pixels;
    RecT       r;
    Int2       visLines;
    Int2       width;

    if(viewer != NULL && oldval != newval) {
	if(Visible (viewer) && AllParentsVisible (viewer)) {
	    GetPanelExtra ((PaneL)viewer, &tv);
	    ObjectRect (viewer, &r);
	    InsetRect (&r, 1, 1);
	    width = r.right - r.left;
	    lineWidth= tv->row_width;
	    visLines = (width) / lineWidth;

	    Select (viewer);
	    if((newval > oldval && newval - visLines + 1 <= oldval) ||
	       (newval < oldval && newval + visLines - 1 >= oldval)) {
		pixels= (newval - oldval) * lineWidth;
		if (ABS (pixels) < width) {
		    ScrollRect (&r, -pixels, 0);
		    if (pixels > 0) {
			r.left = r.right - pixels;
		    } 
		    else {
			r.right = r.left - pixels;
		    }
		    InsetRect (&r, -1, -1);
		    InvalRect (&r);
		} 
		else {
		    InsetRect (&r, -1, -1);
		    InvalRect (&r);
		}
	    }
	    else {
		InsetRect (&r, -1, -1);
		InvalRect (&r);
	    }

	    offsetX = lineWidth * (Int4) (newval - oldval);
	    tv->draw_port.port_origin.x+= offsetX;

	    /* calculate clip box for given rect  */
	    Rect2Box(&tv->draw_port, &r, &tv->draw_port.clip_box);

	    Update ();

	    setPort2Panel(&tv->draw_port); /* resize clip box to whole panel */

	}
    }
}
    
static void CorrectScrollbar(BaR sb, Int4 unitSize, Int4 tree_size, Int4 port_min,  Int4 port_max)
{

    if (sb != NULL) {
	Int4 numUnits= ((tree_size < port_max)? port_max : tree_size)/unitSize;
	Int4 visUnits= (port_max - port_min + 1)/unitSize;
	Int2 max_val= (Int2)MAX((numUnits - visUnits), 0);
	Int2 val = (Int2) (port_min / unitSize);

	CorrectBarMax  (sb, max_val);
	CorrectBarValue(sb, val);
	CorrectBarPage (sb, (Int2)(visUnits - 1), (Int2)(visUnits - 1));
    }
}
    
static void DrawTreeProc(PaneL tree_panel)
{
    TreeViewPtr  tv;

    if(tree_panel != NULL) {
	GetPanelExtra (tree_panel, &tv);
	if(tv->draw_port.bg_color_set == 0) {
	    Uint4 def_col= GetColor();
	    RecT r;

	    GetPosition(tree_panel, &r);
	    tv->draw_port.panel_origin_X= r.left;
	    tv->draw_port.panel_origin_Y= r.top;
	    if(tv->draw_port.line_color_set == 0) tv->draw_port.line_color= def_col;
	    if(tv->draw_port.text_color_set == 0) tv->draw_port.text_color= def_col;
	    if(tv->draw_port.plus_color_set == 0) tv->draw_port.plus_color= tv->draw_port.line_color;
    	    if(tv->draw_port.sbg_color_set  == 0) tv->draw_port.sbg_color= def_col;

	    InvertColors();
	    def_col= GetColor();
	    tv->draw_port.bg_color_set= 1;
	    tv->draw_port.bg_color= def_col;
    	    if(tv->draw_port.sfg_color_set  == 0) tv->draw_port.sfg_color= def_col;
	    InvertColors();

	    if(tv->draw_port.line_color == tv->draw_port.bg_color) {
		Black();
		tv->draw_port.line_color= tv->draw_port.text_color= tv->draw_port.sbg_color= GetColor();
		White();
		tv->draw_port.bg_color= tv->draw_port.sfg_color= GetColor();
	    }
	}
	tree_toNode(tv->cr1, tv->my_root); /*tree_root(tv->cr1);*/
	drawTree(tv->cr1, TREE_X0, TREE_Y0);
    }
}

/*----------------------------------------------------------
 * find node with coordinate x, y
 */
static Int2 find_node4xy(TreeCursorPtr cursor, Int4 my_x, Int4 my_y, Int4Ptr px, Int4Ptr py)
{
    _box my_box;
    Int4 length;
    TreeViewPtr tv;
    Int2 res;

    my_box.xl= my_box.xr= my_x;
    my_box.yt= my_box.yb= my_y;

    if(!getBox4Cursor(cursor, &my_box)) return 0;

    my_box.xl-= 4;
    my_box.xr+= 4;

    if(!ptInBox(&my_box, *px, *py)) return 0;

    tv= (TreeViewPtr) getTree4Cursor(cursor);

    length= getNodeLength(cursor);
    my_x+= ((tv->length_scale >= 0)? 
	    (tv->length_scale * length) : (length/(-tv->length_scale)));

    if(*py <= (my_y+= tv->row_height)) {
	/* we are on node itself */
	Int2 res= (*px < my_x)? 1 : 2;
	*px= my_box.xl + 4;
	*py= my_box.yt;
	return res;
    }

    if(*px < (my_x - 4)) return 0;

    /* we are on nodes children */
    if(tree_child(cursor)) {

	if(tv->cmpSiblings == NULL) {
	    /* we don't want to sort siblings */
	    do {
		if((res= find_node4xy(cursor, my_x, my_y, px, py)) != 0) {
		    return res;
		}
		my_box.xl= my_box.xr= my_x;
		my_box.yt= my_box.yb= my_y;
		getBox4Cursor(cursor, &my_box);
		my_y= my_box.yb;
	    }
	    while (tree_sibling(cursor));
	}
	else {/* siblings have to be sorted */
	    TreeNodeId id;

	    for(id.idi= 0; (id= getNextSibling(cursor, id)).idi != 0;) {
		if((res= find_node4xy(cursor, my_x, my_y, px, py)) != 0) {
		    return res;
		}
		my_box.xl= my_box.xr= my_x;
		my_box.yt= my_box.yb= my_y;
		getBox4Cursor(cursor, &my_box);
		my_y= my_box.yb;
	    }
	}

	tree_parent(cursor);
    }

    return 0;
}
	

/*----------------------------------------------------------
 * this function set cursor (cr1) on selected node and return
 * the type of element selected
 */
static Int2 find_Node4Point(TreeViewPtr tv, Int4Ptr x, Int4Ptr y)
{
    tree_toNode(tv->cr1, tv->my_root); /*tree_root(tv->cr1);*/
    return find_node4xy(tv->cr1, TREE_X0, TREE_Y0, x, y);
}

static void node_open_close(TreeCursorPtr cursor, Int4 x, Int4 y)
{
    TreeViewPtr tv= (TreeViewPtr) getTree4Cursor(cursor);
    _tnSegmPtr tns;
    Boolean need_resize;
    
    tns= getNodeViewerData((_tnSegmPtr)cursor->node->sys_data, tv->my_spy_id);
    if(tns == NULL) return;

    need_resize= (tns->h > tv->row_height)? closeSubtree(tv->cr1, TRUE) : openSubtree(tv->cr1, TRUE);

    if(need_resize) {
	tree_toNode(cursor, tv->my_root); /*tree_root(cursor);*/
	tns= getNodeViewerData((_tnSegmPtr)cursor->node->sys_data, tv->my_spy_id);

	setPort2Panel(&tv->draw_port);
	
	CorrectScrollbar(GetSlateHScrollBar((SlatE)(tv->draw_port.tree_panel)),
			 tv->row_width, tns->w + 16, tv->draw_port.clip_box.xl, tv->draw_port.clip_box.xr);

	CorrectScrollbar(GetSlateVScrollBar((SlatE)(tv->draw_port.tree_panel)),
			 tv->row_height, tns->h + 24, tv->draw_port.clip_box.yt, tv->draw_port.clip_box.yb);

	if(tv->sel_y > y) {
	    /* selected node has been moved */
	    tv->sel_y= -1024;
	}

	/* redraw all below y */
	tv->draw_port.clip_box.yt= y;
	redrawInPort(tv);
	setPort2Panel(&tv->draw_port);
    }
}	

static CharPtr calcBox4Select(TreeCursorPtr cursor, Int4 x0, Int4 y0, _boxPtr sel_box)
{
    TreeViewPtr tv= (TreeViewPtr) getTree4Cursor(cursor);
    Int4 length= getNodeLength(cursor);
    Int2 icon_id= getIconId(cursor);
    CharPtr label= getNodeLabel(cursor);

    sel_box->xl= x0 + 
	((tv->length_scale >= 0)? (tv->length_scale * length) : (length/(-tv->length_scale))) +
	(((tv->i_list != NULL) && (icon_id >= 0))? (getIconWidth(tv->i_list, icon_id)/2) : 0);
    sel_box->xr= x0 + calcNodeWidth(tv, icon_id, length, label);
    sel_box->yt= y0;
    sel_box->yb= y0 + tv->row_height;
    return label;
}
	
static void select_treeNode(TreeCursorPtr cursor, Int4 x, Int4 y)
{
    TreeViewPtr tv= (TreeViewPtr) getTree4Cursor(cursor);
    _box my_box;
    CharPtr label;
    
    setPort2Panel(&tv->draw_port);
    Select(tv->draw_port.tree_panel);

#if 0
    if(tv->evntFunc != NULL) {
	(*(tv->evntFunc))(cursor, TVIEW_ON_SELECT);
	Select(tv->draw_port.tree_panel);
    }
#endif


    if(tv->sel_y >= 0) {
	/* we have selected node */
	
	/* check if selected node is in window */
	if((tv->sel_y < tv->draw_port.clip_box.yb) && 
	   ((tv->sel_y + tv->row_height) > tv->draw_port.clip_box.yt)) {
	    /* yes it is, we have to unselect it */
	    label= calcBox4Select(tv->cr2, tv->sel_x, tv->sel_y, &my_box);
	    unHighLightBox(&tv->draw_port, &my_box, label);
	}
    }

    /* select the new one */
    label= calcBox4Select(cursor, x, y, &my_box);
    highLightBox(&tv->draw_port, &my_box, label);
    tv->sel_x= x;
    tv->sel_y= y;
    tree_toNode(tv->cr2, tree_getId(cursor));
}	

/*****************************************************************************
*
*   ViewerClickProc (viewer, pt)
*
*****************************************************************************/

static void ViewerClickProc (PaneL viewer, PoinT pt)

{
    TreeViewPtr tv;
    Int4 x, y;
    TreeNodeId id;

    GetPanelExtra (viewer, &tv);

    x= (pt.x - tv->draw_port.panel_origin_X) + tv->draw_port.port_origin.x;
    y= (pt.y - tv->draw_port.panel_origin_Y) + tv->draw_port.port_origin.y;
        
    switch (find_Node4Point(tv, &x, &y)) {
    case 0: /* nothing selected */
	id.idi= 0;
	if(dblClick && (tv->dblClickFunc != NULL)) (*tv->dblClickFunc)(tv, id, FALSE);
	else if(shftKey && (tv->shiftClickFunc != NULL))
	    (*tv->shiftClickFunc)(tv, id, FALSE);
	else if(ctrlKey && (tv->ctrlClickFunc != NULL))
	    (*tv->ctrlClickFunc)(tv, id, FALSE);
	return;
    case 1:
	/* open/close subtree for this node */
	if(dblClick && (tv->dblClickFunc != NULL)) (*tv->dblClickFunc)(tv, tv->cr1->node_id, FALSE);
	else if(shftKey && (tv->shiftClickFunc != NULL))
	    (*tv->shiftClickFunc)(tv, tv->cr1->node_id, FALSE);
	else if(ctrlKey && (tv->ctrlClickFunc != NULL))
	    (*tv->ctrlClickFunc)(tv, tv->cr1->node_id, FALSE);
	else node_open_close(tv->cr1, x, y);
	return;
	    
    case 2:
	/* select new node */
	if(dblClick && (tv->dblClickFunc != NULL)) (*tv->dblClickFunc)(tv, tv->cr1->node_id, TRUE);
	else if(shftKey && (tv->shiftClickFunc != NULL))
	    (*tv->shiftClickFunc)(tv, tv->cr1->node_id, TRUE);
	else if(ctrlKey && (tv->ctrlClickFunc != NULL))
	    (*tv->ctrlClickFunc)(tv, tv->cr1->node_id, TRUE);
	else select_treeNode(tv->cr1, x, y);
	return;
    }
}

#if 0
static void ViewerHoldProc (PaneL viewer, PoinT pt)

{
    TreeViewPtr tv;
    Int4 x, y;


    GetPanelExtra (viewer, &tv);

    if(tv->holdFunc == NULL) return;

    x= (pt.x - tv->draw_port.panel_origin_X) + tv->draw_port.port_origin.x;
    y= (pt.y - tv->draw_port.panel_origin_Y) + tv->draw_port.port_origin.y;
        
    switch (find_Node4Point(tv, &x, &y)) {
    case 0: /* nothing selected */
	(*tv->holdFunc)(tv, tv->cr2->node_id, FALSE);
	return;
    case 1:
	/* open/close subtree for this node */
	(*tv->holdFunc)(tv, tv->cr1->node_id, FALSE);
	return;
	    
    case 2:
	/* select new node */
	(*tv->holdFunc)(tv, tv->cr1->node_id, TRUE);
	return;
    }
}
#endif

/*
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
    TREE_SUBTREE_DEL_DONE
} TreeEvent;
*/
static void spy_proc(TreePtr tree, Int2 spy_id, TreeEvent event, 
                     TreeNodeId id1, TreeNodeId id2, VoidPtr node_data,
                     Int2 node_data_size)
{
    static int tran_level= 0;
    static TreeEvent tran_stack[8];
    TreeViewPtr tv= (TreeViewPtr) tree_getSpyData(tree, spy_id);
    _tnSegmPtr tns;
    Boolean need_redraw= FALSE;

    switch(event) {
    case TREE_NODE_UPDATE: /* node will be updated */
        tree_toNode(tv->cr4, id1);
        if(getNodeViewerData((_tnSegmPtr)tv->cr4->node->sys_data, tv->my_spy_id) != NULL) {
            freeNodeSegm(tv->cr4);
            tran_stack[tran_level++]= TREE_NODE_UPDATE;
        }
        else {
            tran_stack[tran_level++]= TREE_CURSOR_MOVED;
        }
        break;
    case TREE_NODE_UPD_DONE: /* node was updated */
    case TREE_NODEDATA_UPD_DONE:
        if(tran_stack[--tran_level] == TREE_NODE_UPDATE) {
            if(tree_parent(tv->cr4)) {
                openSubtree(tv->cr4, FALSE);
            }
            else {
                makeNodeVisible(tv->cr4);
            }
            need_redraw= TRUE;
        }
        break;

    case TREE_NODEDATA_UPDATE: /* node will be updated */
        if(tran_stack[tran_level - 1] == TREE_NODE_UPDATE) {
            tran_stack[tran_level++]= TREE_CURSOR_MOVED;
        }
        else {
            tree_toNode(tv->cr4, id1);
            if(getNodeViewerData((_tnSegmPtr)tv->cr4->node->sys_data, tv->my_spy_id) != NULL) {
                freeNodeSegm(tv->cr4);
                tran_stack[tran_level++]= TREE_NODE_UPDATE;
            }
            else {
                tran_stack[tran_level++]= TREE_CURSOR_MOVED;
            }
        }
        break;

    case TREE_NODE_DELETE: /* node will be deleted */
        tree_toNode(tv->cr4, id1);
        if(getNodeViewerData((_tnSegmPtr)tv->cr4->node->sys_data, tv->my_spy_id) != NULL) {
            freeNodeSegm(tv->cr4);
            tran_stack[tran_level++]= TREE_NODE_DELETE;
            tree_toNode(tv->cr4, id2);
            closeSubtree(tv->cr4, FALSE);
        }
        else {
            tran_stack[tran_level++]= TREE_CURSOR_MOVED;
        }
        tree_parent(tv->cr4);
        break;

    case TREE_SUBTREE_DEL_DONE:
    case TREE_NODE_DEL_DONE: /* node was deleted */
        tree_toNode(tv->cr4, id1);
        if(tran_stack[--tran_level] == TREE_NODE_DELETE) {
            openSubtree(tv->cr4, FALSE);
            need_redraw= TRUE;
        }
        break;

    case TREE_NODE_MOVE: /* node will be moved */
        tree_toNode(tv->cr4, id2);
        if(getNodeViewerData((_tnSegmPtr)tv->cr4->node->sys_data, tv->my_spy_id) != NULL) {
            /* node is visible */
            tree_parent(tv->cr4);
        }
        else {
            tree_toNode(tv->cr4, id1);
        }
        break;

    case TREE_NODE_MV_DONE: /* node has been moved */
        if(tv->cr4->node_id.idi != id1.idi) {
            need_redraw= openSubtree(tv->cr4, FALSE);
            tree_toNode(tv->cr4, id1);
        }
        need_redraw|= openSubtree(tv->cr4, FALSE);
        break;

    case TREE_CHILDREN_MOVE: /* children will be moved */
        tree_toNode(tv->cr4, id2);
        if(((tns= getNodeViewerData((_tnSegmPtr)tv->cr4->node->sys_data, tv->my_spy_id)) != NULL) &&
           (tns->h > tv->row_height)) {
            need_redraw= closeSubtree(tv->cr4, TRUE);
        }
        break;

    case TREE_CHILDREN_MV_DONE: /* children have been moved */
        tree_toNode(tv->cr4, id1);
        if((tns= getNodeViewerData((_tnSegmPtr)tv->cr4->node->sys_data, tv->my_spy_id)) == NULL) {
            /* this node is invisible */
            for(need_redraw= tree_child(tv->cr4); need_redraw; need_redraw= tree_sibling(tv->cr4)) {
                if(getNodeViewerData((_tnSegmPtr)tv->cr4->node->sys_data, tv->my_spy_id) != NULL) {
                    freeNodeSegm(tv->cr4);
                }
            }
        }
        else {
            closeSubtree(tv->cr4, FALSE);
            need_redraw= openSubtree(tv->cr4, TRUE);
        }
        break;
		
    case TREE_SUBTREE_DELETE: /* subtree will be deleted */
        tree_toNode(tv->cr4, id1);
        if(getNodeViewerData((_tnSegmPtr)tv->cr4->node->sys_data, tv->my_spy_id) != NULL) {
            /* this node is visible */
	    
            tran_stack[tran_level++]= TREE_NODE_DELETE;
        }
        else {
            tran_stack[tran_level++]= TREE_CURSOR_MOVED;
        }
        closeAllSubtrees(tv->cr4);
        tree_parent(tv->cr4);
        break;
	
    case TREE_NODE_MERGE: /* nodes will be merged */
        tree_toNode(tv->cr4, id2);
        if(getNodeViewerData((_tnSegmPtr)tv->cr4->node->sys_data, tv->my_spy_id) != NULL) {
            /* this node is visible */
            freeNodeSegm(tv->cr4);
        }
        break;

    case TREE_NODE_MERGE_DONE: /* nodes was merged */
        tree_toNode(tv->cr4, id2);
        openSubtree(tv->cr4, TRUE);
        tree_toNode(tv->cr4, id1);
        openSubtree(tv->cr4, TRUE);
        need_redraw= TRUE;
        break;

    case TREE_NODE_ADDED: /* new node was added */
        tree_toNode(tv->cr4, id1);
        need_redraw= openSubtree(tv->cr4, TRUE);
        break;
	    
    case TREE_CURSOR_MOVED:
        break;  /* do nothing */
    }
    


    if(need_redraw) {
        tree_toNode(tv->cr4, tv->my_root); /*tree_root(tv->cr4);*/
        tns= getNodeViewerData((_tnSegmPtr)tv->cr4->node->sys_data, tv->my_spy_id);

        setPort2Panel(&tv->draw_port);
	
        CorrectScrollbar(GetSlateHScrollBar((SlatE)(tv->draw_port.tree_panel)),
                         tv->row_width, tns->w + 16, tv->draw_port.clip_box.xl, tv->draw_port.clip_box.xr);

        CorrectScrollbar(GetSlateVScrollBar((SlatE)(tv->draw_port.tree_panel)),
                         tv->row_height, tns->h + 24, tv->draw_port.clip_box.yt, tv->draw_port.clip_box.yb);
        tv->sel_y= -1024;
        tview_redraw(tv);
    }
}

static void close_proc(WindoW w)
{
    TreeViewPtr tv= (TreeViewPtr) GetWindowExtra(w);
    
    tview_delete(tv, 0);
}

static void resize_proc(WindoW w)
{
    TreeViewPtr tv= (TreeViewPtr) GetWindowExtra(w);
    RecT r;
    _tnSegmPtr tns;

    setPort2Panel(&tv->draw_port);
    Nlm_GetRect((GraphiC)w, &r);
    r.right-= r.left + 10;
    r.left= 10;
    r.bottom-= r.top + 10;
    r.top= 10;
    SetPosition(tv->draw_port.tree_panel, &r);

    tv->draw_port.panel_origin_X= r.left;
    tv->draw_port.panel_origin_Y= r.top;

    InsetRect(&r, 1, 1);
    tv->draw_port.clip_box.xr= tv->draw_port.clip_box.xl + (r.right - r.left - Nlm_vScrollBarWidth);
    tv->draw_port.clip_box.yb= tv->draw_port.clip_box.yt + (r.bottom - r.top - Nlm_hScrollBarHeight);

    /* we need to correct scroll bars here */
    tree_toNode(tv->cr1, tv->my_root); /*tree_root(tv->cr1);*/
    tns= getNodeViewerData((_tnSegmPtr)tv->cr1->node->sys_data, tv->my_spy_id);

    CorrectScrollbar(GetSlateHScrollBar((SlatE)(tv->draw_port.tree_panel)),
		     tv->row_width, tns->w + 16, tv->draw_port.clip_box.xl, tv->draw_port.clip_box.xr);

    CorrectScrollbar(GetSlateVScrollBar((SlatE)(tv->draw_port.tree_panel)),
		     tv->row_height, tns->h + 24, tv->draw_port.clip_box.yt, tv->draw_port.clip_box.yb);

    Select(tv->draw_port.tree_panel);
    InvalRect (&r);
    /*    Black();
    FrameRect(&r);*/
    Update();
}

void tview_resize(TreeViewPtr tv, RectPtr r)
{
    _tnSegmPtr tns;

    SetPosition(tv->draw_port.tree_panel, r);

    tv->draw_port.panel_origin_X= r->left;
    tv->draw_port.panel_origin_Y= r->top;

    InsetRect(r, 1, 1);
    tv->draw_port.clip_box.xr= tv->draw_port.clip_box.xl + (r->right - r->left - Nlm_vScrollBarWidth);
    tv->draw_port.clip_box.yb= tv->draw_port.clip_box.yt + (r->bottom - r->top - Nlm_hScrollBarHeight);

    /* we need to correct scroll bars here */
    tree_toNode(tv->cr1, tv->my_root); /*tree_root(tv->cr1);*/
    tns= getNodeViewerData((_tnSegmPtr)tv->cr1->node->sys_data, tv->my_spy_id);

    CorrectScrollbar(GetSlateHScrollBar((SlatE)(tv->draw_port.tree_panel)),
		     tv->row_width, tns->w + 16, tv->draw_port.clip_box.xl, tv->draw_port.clip_box.xr);

    CorrectScrollbar(GetSlateVScrollBar((SlatE)(tv->draw_port.tree_panel)),
		     tv->row_height, tns->h + 24, tv->draw_port.clip_box.yt, tv->draw_port.clip_box.yb);
    Select(tv->draw_port.tree_panel);
    InvalRect (r);
    Update();
}

static void tv_cr2_callback(TreeCursorPtr cursor, TreeEvent event, TreeNodeId id)
{
    if(event == TREE_CURSOR_MOVED) {
	TreeViewPtr tv= (TreeViewPtr) getTree4Cursor(cursor);

	if(tv->evntFunc != NULL) {
	    (*(tv->evntFunc))(cursor, TVIEW_ON_SELECT);
	    Select(tv->draw_port.tree_panel);
	}
    }
}

static void tv_cr5_callback(TreeCursorPtr cursor, TreeEvent event, TreeNodeId id)
{
    if(event == TREE_CURSOR_MOVED) {
	TreeViewPtr tv= (TreeViewPtr) getTree4Cursor(cursor);

	tv->my_root= id;
	if(getNodeViewerData((_tnSegmPtr)cursor->node->sys_data, tv->my_spy_id) == NULL) {
	    tree_parent(cursor);
	}
    }
}


TreeViewPtr tview_create(GrouP parent, Int2 width, Int2 height, 
			 TreePtr tree, _ImageListPtr il, Int2 l_scale, 
			 TreeNodeCmpFunc NodeCmp)
{
    TreeViewPtr tv;
    /* int i; */
    _tnSegmPtr tns;

    if((tv= (TreeViewPtr) MemNew(sizeof(TreeView))) == NULL) return NULL;

    if((tv->segm_buff= getNewSegmBuff()) == NULL) {
	MemFree(tv);
	return NULL;
    }


    if(parent == NULL) {
	/* we need to create window for tree view */
	tv->draw_port.tree_window= DocumentWindow(-50, -33, -10, -10, "Tree View", close_proc, resize_proc);
	parent= tv->draw_port.tree_window;
	SetWindowExtra(tv->draw_port.tree_window, tv, NULL);
    }
    else tv->draw_port.tree_window= NULL;

    /* create panel */
    tv->draw_port.tree_panel= AutonomousPanel(parent, width, height, DrawTreeProc,
					      TreeVScrollProc, TreeHScrollProc,
					      sizeof(TreeViewPtr), NULL, NULL);

    if(tv->draw_port.tree_panel == NULL) {
	MemFree(tv->segm_buff);
	MemFree(tv);
	return NULL;
    }

    SetPanelClick (tv->draw_port.tree_panel, ViewerClickProc, NULL, NULL, NULL);

    SetPanelExtra(tv->draw_port.tree_panel, &tv);

    tv->i_list= il;

    /* populate tv structure */
    tv->tree= tree;

    tv->cr1= tree_openCursor(tree, tv, NULL);
    tv->cr2= tree_openCursor(tree, tv, tv_cr2_callback);
    tv->cr3= NULL;
    tv->cr4= tree_openCursor(tree, tv, NULL);
    
    tv->my_spy_id= tree_addSpy(tree, spy_proc, tv);

    /* draw port setup */
    tv->draw_port.panel_origin_X= tv->draw_port.panel_origin_Y= 0;
    tv->draw_port.port_origin.x= 0;
    tv->draw_port.port_origin.y= 0;
     
    /*Black();*/
    tv->draw_port.clip_box.xl= 0;
    tv->draw_port.clip_box.yt= 0;
    tv->draw_port.clip_box.xr= width;
    tv->draw_port.clip_box.yb= height;
    tv->draw_port.line_color= tv->draw_port.text_color= tv->draw_port.sbg_color= 0;
    tv->draw_port.line_color_set= tv->draw_port.text_color_set= tv->draw_port.sbg_color_set= 0;
    /* InvertColors(); */
    tv->draw_port.bg_color= tv->draw_port.sfg_color= 0;
    tv->draw_port.bg_color_set= tv->draw_port.sfg_color_set= 0;
    tv->draw_port.plus_color= 0;
    tv->draw_port.plus_color_set= 0;
    /*InvertColors();*/

    tv->draw_port.pen_w= 1;

    tv->row_height= FontHeight() + 2;
    /*tv->row_height+= tv->row_height/4;*/
    tv->row_width= CharWidth('w');
    if(tv->row_width < 4) tv->row_width= 4;

    tv->length_scale= (l_scale == 0)? 1 : l_scale;

    tv->sel_y= -1024;

    /* prepare root node for drawing */
    tns= allocNewSegm(tv->cr1);
    tns->w= calcNodeWidth(tv, getIconId(tv->cr1), getNodeLength(tv->cr1), getNodeLabel(tv->cr1));
    tns->h= tv->row_height;

    tv->cmpSiblings= NodeCmp;
    tv->dblClickFunc= NULL;
    tv->shiftClickFunc= NULL;
    tv->ctrlClickFunc= NULL;

    tv->cr5= tree_openCursor(tree, tv, tv_cr5_callback);
    
    tv->my_root= tv->cr5->node_id;

    openSubtree(tv->cr1, TRUE);

    if(tv->draw_port.tree_window != NULL) {
	RealizeWindow(tv->draw_port.tree_window);
	Show(tv->draw_port.tree_window);
    }
    return tv;
}


TreeViewPtr tview_createCustom(Int2 left, Int2 top, Int2 width, Int2 height,
			       CharPtr title, void (*menuCrtFunc)(WindoW),
			 TreePtr tree, _ImageListPtr il, Int2 l_scale, 
			 TreeNodeCmpFunc NodeCmp)
{
    TreeViewPtr tv;
    /* int i; */
    _tnSegmPtr tns;

    if((tv= (TreeViewPtr) MemNew(sizeof(TreeView))) == NULL) return NULL;

    if((tv->segm_buff= getNewSegmBuff()) == NULL) {
	MemFree(tv);
	return NULL;
    }


    /* we need to create window for tree view */
    tv->draw_port.tree_window= DocumentWindow(left, top, -10, -10, title, close_proc, resize_proc);
    SetWindowExtra(tv->draw_port.tree_window, tv, NULL);
    
    if(menuCrtFunc != NULL) (*menuCrtFunc)(tv->draw_port.tree_window);

    /* create panel */
    tv->draw_port.tree_panel= AutonomousPanel(tv->draw_port.tree_window, width, height, DrawTreeProc,
					      TreeVScrollProc, TreeHScrollProc,
					      sizeof(TreeViewPtr), NULL, NULL);

    if(tv->draw_port.tree_panel == NULL) {
	MemFree(tv->segm_buff);
	MemFree(tv);
	return NULL;
    }

    SetPanelClick (tv->draw_port.tree_panel, ViewerClickProc, NULL, NULL, NULL);

    SetPanelExtra(tv->draw_port.tree_panel, &tv);

    tv->i_list= il;

    /* populate tv structure */
    tv->tree= tree;

    tv->cr1= tree_openCursor(tree, tv, NULL);
    tv->cr2= tree_openCursor(tree, tv, tv_cr2_callback);
    tv->cr3= NULL;
    tv->cr4= tree_openCursor(tree, tv, NULL);

    tv->my_spy_id= tree_addSpy(tree, spy_proc, tv);

    /* draw port setup */
    tv->draw_port.panel_origin_X= tv->draw_port.panel_origin_Y= 0;
    tv->draw_port.port_origin.x= 0;
    tv->draw_port.port_origin.y= 0;
     
    /*Black();*/
    tv->draw_port.clip_box.xl= 0;
    tv->draw_port.clip_box.yt= 0;
    tv->draw_port.clip_box.xr= width;
    tv->draw_port.clip_box.yb= height;
    tv->draw_port.line_color= tv->draw_port.text_color= tv->draw_port.sbg_color= 0;
    tv->draw_port.line_color_set= tv->draw_port.text_color_set= tv->draw_port.sbg_color_set= 0;
    /* InvertColors(); */
    tv->draw_port.bg_color= tv->draw_port.sfg_color= 0;
    tv->draw_port.bg_color_set= tv->draw_port.sfg_color_set= 0;
    tv->draw_port.plus_color= 0;
    tv->draw_port.plus_color_set= 0;
    /*InvertColors();*/

    tv->draw_port.pen_w= 1;

    tv->row_height= FontHeight() + 2;
    /*tv->row_height+= tv->row_height/4;*/
    tv->row_width= CharWidth('w');
    if(tv->row_width < 4) tv->row_width= 4;

    tv->length_scale= (l_scale == 0)? 1 : l_scale;

    tv->sel_y= -1024;

    /* prepare root node for drawing */
    tns= allocNewSegm(tv->cr1);
    tns->w= calcNodeWidth(tv, getIconId(tv->cr1), getNodeLength(tv->cr1), getNodeLabel(tv->cr1));
    tns->h= tv->row_height;

    tv->cmpSiblings= NodeCmp;
    tv->dblClickFunc= NULL;
    tv->shiftClickFunc= NULL;
    tv->ctrlClickFunc= NULL;

    tv->cr5= tree_openCursor(tree, tv, tv_cr5_callback);
    
    tv->my_root= tv->cr5->node_id;

    openSubtree(tv->cr1, TRUE);

    if(tv->draw_port.tree_window != NULL) {
	RealizeWindow(tv->draw_port.tree_window);
	Show(tv->draw_port.tree_window);
    }
    return tv;
}
    
Boolean tview_setTextColor(TreeViewPtr tv, Uint4 color)
{
    if((color != tv->draw_port.bg_color) || (tv->draw_port.bg_color_set == 0)){
	tv->draw_port.text_color= color;
	tv->draw_port.text_color_set= 1;
	return TRUE;
    }
    
    return FALSE;
}

Boolean tview_setLineColor(TreeViewPtr tv, Uint4 color)
{
    if((color != tv->draw_port.bg_color) || (tv->draw_port.bg_color_set == 0)) {
	tv->draw_port.line_color= color;
	tv->draw_port.line_color_set= 1;
	return TRUE;
    }
    
    return FALSE;
}

Boolean tview_setPlusColor(TreeViewPtr tv, Uint4 color)
{
    if((color != tv->draw_port.bg_color) || (tv->draw_port.bg_color_set == 0)) {
	tv->draw_port.plus_color= color;
	tv->draw_port.plus_color_set= 1;
	return TRUE;
    }
    
    return FALSE;
}

Boolean tview_setSelBGColor(TreeViewPtr tv, Uint4 color)
{
    tv->draw_port.sbg_color= color;
    tv->draw_port.sbg_color_set= 1;
    return TRUE;
}

Boolean tview_setSelFGColor(TreeViewPtr tv, Uint4 color)
{
    tv->draw_port.sfg_color= color;
    tv->draw_port.sfg_color_set= 1;
    return TRUE;
}

Int2 tview_labelCmp(TreeCursorPtr c1, TreeCursorPtr c2)
{
    static char l1[256], l2[256];
    Int2 res;

    StringNCpy(l1, getNodeLabel(c1), 255);
    StringNCpy(l2, getNodeLabel(c2), 255);
    res= StringCmp(l1, l2);
    return res;
}

Int2 tview_distanceCmp(TreeCursorPtr c1, TreeCursorPtr c2)
{
    return (Int2)(getNodeLength(c1) - getNodeLength(c2));
}

static void resortNodes(TreeCursorPtr cursor)
{
    TreeViewPtr tv= (TreeViewPtr) getTree4Cursor(cursor);
    _tnSegmPtr tns= getNodeViewerData((_tnSegmPtr)cursor->node->sys_data, tv->my_spy_id);
    TreeNodeId nid= tree_getId(cursor);
    Boolean res;

    if((tns == NULL) || (tns->h <= tv->row_height)) return;

    closeSubtree(cursor, FALSE);
    
    tree_toNode(cursor, nid);

    openSubtree(cursor, FALSE);

    tree_toNode(cursor, nid);

    for(res= tree_child(cursor); res; res= tree_sibling(cursor)) {
	resortNodes(cursor);
    }

    tree_toNode(cursor, nid);
}

TreeNodeCmpFunc tview_setNodeCmpFunc(TreeViewPtr tv, TreeNodeCmpFunc NodeCmp)
{
    TreeNodeCmpFunc old;
    
    old= tv->cmpSiblings;
    tv->cmpSiblings= NodeCmp;

    if(NodeCmp != NULL) {
	TreeNodeId sid= tv->cr2->node_id;

	WatchCursor();
	tree_toNode(tv->cr1, tv->my_root); /*tree_root(tv->cr1);*/
	resortNodes(tv->cr1);
	tree_toNode(tv->cr2, sid);
	tview_showSelected(tv);
	ArrowCursor();
    }

    return old;
}

TreeViewEventFunc tview_setEventHandler(TreeViewPtr tv, TreeViewEventFunc HandlerFunc)
{
    TreeViewEventFunc old;
    
    old= tv->evntFunc;
    tv->evntFunc= HandlerFunc;

    return old;
}


TreeViewClickFunc tview_setDblClickFunc(TreeViewPtr tv, TreeViewClickFunc dblClickProc)
{
    TreeViewClickFunc old;
    
    old= tv->dblClickFunc;
    tv->dblClickFunc= dblClickProc;

    return old;
}

TreeViewClickFunc tview_setShiftClickFunc(TreeViewPtr tv, TreeViewClickFunc shiftClickProc)
{
    TreeViewClickFunc old;
    
    old= tv->shiftClickFunc;
    tv->shiftClickFunc= shiftClickProc;

    return old;
}

TreeViewClickFunc tview_setCtrlClickFunc(TreeViewPtr tv, TreeViewClickFunc ctrlClickProc)
{
    TreeViewClickFunc old;
    
    old= tv->ctrlClickFunc;
    tv->ctrlClickFunc= ctrlClickProc;

    return old;
}
		
void tview_redraw(TreeViewPtr tv)
{

    PaneL viewer= tv->draw_port.tree_panel;
    RecT r;
    _tnSegmPtr tns;

    if (Visible(viewer) && AllParentsVisible (viewer)) {

	Select(viewer);
	ObjectRect(viewer, &r);
	/*InsetRect(&r, 1, 1);*/
	setPort2Panel(&tv->draw_port);
	
	tree_toNode(tv->cr1, tv->my_root); /*tree_root(cursor);*/
	tns= getNodeViewerData((_tnSegmPtr)tv->cr1->node->sys_data, tv->my_spy_id);

	setPort2Panel(&tv->draw_port);
	
	CorrectScrollbar(GetSlateHScrollBar((SlatE)(tv->draw_port.tree_panel)),
		     tv->row_width, tns->w + 16, tv->draw_port.clip_box.xl, tv->draw_port.clip_box.xr);

	CorrectScrollbar(GetSlateVScrollBar((SlatE)(tv->draw_port.tree_panel)),
		     tv->row_height, tns->h + 24, tv->draw_port.clip_box.yt, tv->draw_port.clip_box.yb);
	Select(viewer);
	InvalRect(&r);
	Update();
    }
}

static Boolean ancestor(TreeCursorPtr cursor, TreeNodeId nid)
{
    TreeNodeId pid= cursor->node_id;
    TreeNodeId gpid= cursor->node->parent;

    tree_toNode(cursor, nid);
    while(cursor->node_id.idi != gpid.idi) {
	if(cursor->node_id.idi == pid.idi) return TRUE;
	if(!tree_parent(cursor)) break;
    }

    tree_toNode(cursor, pid);
    return FALSE;
}
    
/*-------------------------------------------
 * function makes node tv->cr1 visible
 */
static void getNodeCoordinates(TreeViewPtr tv, Int4Ptr node_x, Int4Ptr node_y)
{
    _tnSegmPtr tns;
    Int4 x, y;
    _box my_box;
    /*Boolean need_cont;*/
    TreeNodeId nid= tv->cr1->node_id;
    TreeNodeId id;
    Int4 length;

    /* make node visible */
    tree_toNode(tv->cr1, tv->my_root); /*tree_root(tv->cr1);*/
    x= TREE_X0;
    y= TREE_Y0;

    if(tv->cr3 == NULL) {
	tv->cr3= tree_openCursor(tv->tree, tv, NULL);
    }

    while(tv->cr1->node_id.idi != nid.idi) {

	tns= getNodeViewerData((_tnSegmPtr)tv->cr1->node->sys_data, tv->my_spy_id);
	if(tns == NULL) break;
	if(tns->h <= tv->row_height) {
	    openSubtree(tv->cr1, TRUE);
	}
	y+= tv->row_height;
	length= getNodeLength(tv->cr1);
	x+= ((tv->length_scale >= 0)? 
	     (tv->length_scale * length) : (length/(-tv->length_scale)));
	
	if(tree_child(tv->cr1)) {

	    if(tv->cmpSiblings == NULL) {
		/* we don't want to sort siblings */
		do {
		    if(ancestor(tv->cr1, nid)) break;
		    
		    my_box.xl= my_box.xr= x;
		    my_box.yt= my_box.yb= y;
		    getBox4Cursor(tv->cr1, &my_box);
		    y= my_box.yb;
		}
		while (tree_sibling(tv->cr1));
	    }
	    else {/* siblings have to be sorted */

		for(id.idi= 0; (id= getNextSibling(tv->cr1, id)).idi != 0;) {

		    if(ancestor(tv->cr1, nid)) break;

		    my_box.xl= my_box.xr= x;
		    my_box.yt= my_box.yb= y;
		    getBox4Cursor(tv->cr1, &my_box);
		    y= my_box.yb;
		}
	    }
	}
	else {
	    break;
	}
    }

    *node_x= x;
    *node_y= y;
    tree_toNode(tv->cr1, nid);
}

	
/*-------------------------------------------
 * Adjust scrolling to show selected node
 * SetBarValue(Nlm_BaR b, Nlm_Int4 val)
 * extern Nlm_Int4 Nlm_GetBarMax (Nlm_BaR b)
 */
void tview_showSelected(TreeViewPtr tv)
{
    Int4 bar_val= -1, s;
    BaR b;
    _box my_box;
    

    if(tv->draw_port.tree_window != NULL) Select(tv->draw_port.tree_window);

    setPort2Panel(&tv->draw_port);
    if(tv->sel_y < 0) {

	/* we have to calculate coordinates for selected node */
	tree_toNode(tv->cr1, tree_getId(tv->cr2));
	getNodeCoordinates(tv, &tv->sel_x, &tv->sel_y);
    }

    /* adjust vertical bar */

    b= GetSlateVScrollBar((SlatE)(tv->draw_port.tree_panel));

    if(tv->sel_y < tv->draw_port.clip_box.yt) {
	/* the node is above the screen */
	if(tv->sel_y == 0) {
	    /* this is a root */
	    bar_val= 0;
	}
	else {
	    bar_val= tv->sel_y/tv->row_height - 1;
	}
    }
    else if(tv->sel_y >= tv->draw_port.clip_box.yb) {
	/* the node is below the screen */
	s= (tv->draw_port.clip_box.yb - tv->draw_port.clip_box.yt)/tv->row_height;
	bar_val= tv->sel_y/tv->row_height - s + 2;
	if(bar_val < 0) bar_val= 1;
	s= GetBarMax(b);
	if(bar_val > s) bar_val= s;
    }

    if(bar_val >= 0) {
	SetBarValue(b, bar_val);
    }
    
    /* adjust horisontal bar */

    b= GetSlateHScrollBar((SlatE)(tv->draw_port.tree_panel));
    bar_val= -1;
    s= GetBarMax(b);

    if(s == 0) return;
    calcBox4Select(tv->cr2, tv->sel_x, tv->sel_y, &my_box);
    if((my_box.xl >= tv->draw_port.clip_box.xl) && (my_box.xr < tv->draw_port.clip_box.xr)) {
	/* perfectly visible */
	return;
    }
    
    bar_val= tv->sel_x/tv->row_width - 1;
    if(bar_val < 0) bar_val= 0;
    if(bar_val > s) bar_val= s;
    
    if(bar_val >= 0) {
	SetBarValue(b, bar_val);
    }    

}    
	
Boolean tview_selectNode(TreeViewPtr tv, TreeNodeId id)
{
    if(tree_toNode(tv->cr1, id)) {
	Int4 x, y;

	if(!tree_isDescendant(tv->cr5, tv->cr1)) {
	    tview_moveRoot(tv, tree_getAncestor(tv->cr5, tv->cr1));
	    tree_toNode(tv->cr1, id);
	}
	getNodeCoordinates(tv, &x, &y);
	select_treeNode(tv->cr1, x, y);
	tview_showSelected(tv);
	return TRUE;
    }
    return FALSE;
}
	
Boolean tview_openSubtree(TreeViewPtr tv, TreeNodeId id)
{
    if(tree_toNode(tv->cr1, id)) {
	Int4 x, y;

	getNodeCoordinates(tv, &x, &y);
	openSubtree(tv->cr1, TRUE);
	return TRUE;
    }
    return FALSE;
}

Boolean tview_closeSubtree(TreeViewPtr tv, TreeNodeId id)
{
    if(tree_toNode(tv->cr1, id)) {

	closeSubtree(tv->cr1, TRUE);
	return TRUE;
    }
    return FALSE;
}

void tview_hide(TreeViewPtr tv)
{
    if(tv->draw_port.tree_window != NULL) Hide(tv->draw_port.tree_window);
    else Hide(tv->draw_port.tree_panel);
}

void tview_show(TreeViewPtr tv)
{
    if(tv->draw_port.tree_window != NULL) Show(tv->draw_port.tree_window);
    else Show(tv->draw_port.tree_panel);
}


void tview_disable(TreeViewPtr tv)
{
    Disable(tv->draw_port.tree_panel);
}

void tview_enable(TreeViewPtr tv)
{
    Enable(tv->draw_port.tree_panel);
}

static void closeAllSubtrees(TreeCursorPtr cursor)
{
    freeNodeSegm(cursor);
    if(tree_child(cursor)) {
	do {
	    closeAllSubtrees(cursor);
	}
	while(tree_sibling(cursor));
	tree_parent(cursor);
    }
}
	
	
void tview_delete(TreeViewPtr tv, Boolean do_hide)
{
    _tnSegmPtr ts;
    _tnSegmPtr tsn;
    /* close all subtrees */
    tree_root(tv->cr1);
    closeAllSubtrees(tv->cr1);

    tree_closeCursor(tv->cr1);
    tree_closeCursor(tv->cr2);
    tree_closeCursor(tv->cr3);
    tree_closeCursor(tv->cr4);
    tree_closeCursor(tv->cr5);
    tree_delSpy(tv->tree, tv->my_spy_id);
    if(tv->draw_port.tree_panel != NULL) {
	if(do_hide) Hide(tv->draw_port.tree_panel);
	Remove(tv->draw_port.tree_panel);
    }
    if(tv->draw_port.tree_window != NULL) {
	if(do_hide) Hide(tv->draw_port.tree_window);
	Remove(tv->draw_port.tree_window);
    }

    for(ts= tv->segm_buff; ts != NULL; ts= tsn) {
	tsn= ts[4095].next;
	MemFree(ts);
    }
    MemFree(tv);
}
	
Boolean tview_moveRoot(TreeViewPtr tv, TreeNodeId id)
{
    _tnSegmPtr tns;
    BaR b;

    if(tree_toNode(tv->cr1, id)) {
	tns= getNodeViewerData((_tnSegmPtr)tv->cr1->node->sys_data, tv->my_spy_id);
	if(tns == NULL) {
	    
	    tns= allocNewSegm(tv->cr1);
	    tns->w= calcNodeWidth(tv, getIconId(tv->cr1), getNodeLength(tv->cr1), getNodeLabel(tv->cr1));
	    tns->h= tv->row_height;
	}
	tree_toNode(tv->cr5, id);
	if((tv->cr2->node_id.idi != id.idi) && (!tree_isDescendant(tv->cr5, tv->cr2))) {
	    tree_toNode(tv->cr2, id);
	}
	openSubtree(tv->cr1, TRUE);

	tns= getNodeViewerData((_tnSegmPtr)tv->cr5->node->sys_data, tv->my_spy_id);

	setPort2Panel(&tv->draw_port);
	
	CorrectScrollbar(GetSlateHScrollBar((SlatE)(tv->draw_port.tree_panel)),
			 tv->row_width, tns->w + 16, tv->draw_port.clip_box.xl, tv->draw_port.clip_box.xr);

	CorrectScrollbar(GetSlateVScrollBar((SlatE)(tv->draw_port.tree_panel)),
			 tv->row_height, tns->h + 24, tv->draw_port.clip_box.yt, tv->draw_port.clip_box.yb);

	b= GetSlateVScrollBar((SlatE)(tv->draw_port.tree_panel));
	SetBarValue(b, 0);
	
	b= GetSlateHScrollBar((SlatE)(tv->draw_port.tree_panel));
	SetBarValue(b, 0);

	tv->sel_y= -1024;
	
	tview_redraw(tv);
	return TRUE;
    }
    return FALSE;
}

TreeNodeId tview_getSelected(TreeViewPtr tv)
{
    return tv->cr2->node_id;
}

TreePtr tview_getTreePtr(TreeViewPtr tv)
{
    return tv->tree;
}

	


