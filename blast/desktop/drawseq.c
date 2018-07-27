

#include <vibrant.h>
#include <drawseq.h>
#include <edutil.h>

Uint1 custom_color[3];

#define SCALE_SPACE 20
static Boolean  AddFlatNodeSymbol PROTO((SegmenT pseg, ValNodePtr fnp_node, Int4 scale));

/*######################################################################
#
#	functions for setting up the color for different object
#
######################################################################*/


#define RGB_B(x) (Uint1)((x)&255);
#define RGB_G(x) (Uint1)(((x)>>8)&255);
#define RGB_R(x) (Uint1)(((x)>>16)&255);
static Boolean convert_color(Int4 val, Uint1Ptr color)
{

	if(val<0 || color == NULL)
		return FALSE;
	color[0] = RGB_R(val);
	color[1] = RGB_G(val);
	color[2] = RGB_B(val);
	return TRUE;
}


Uint1 MUSK_COLOR[3];
/************************************************************************
*
*	get_seg_color(order)
*	set up the color for a segmented sequence
*
*************************************************************************/
Uint1Ptr get_seg_color(Int2 order)
{
	Int2 c_num;
	Int4 color_val;
	
	c_num = (Int2)GetMuskCParam(MSM_CCOLOR, MSM_NOSUBCLASS, MSM_NUM);
	if(c_num != 0)
	{
		order = (Int2)(order)%c_num +1;
		color_val = GetMuskCParam(MSM_CCOLOR, order, MSM_COLOR);
		if(convert_color(color_val, MUSK_COLOR))
			return MUSK_COLOR;
	}
	return NULL;

}


/**********************************************************************
*
*	get the color for different enzymes
*	used in drawing the restriction map
*
**********************************************************************/
Uint1Ptr get_enz_color(Int2 enzID)
{
	return get_seg_color(enzID);
}


/**********************************************************************
*
*
*
***********************************************************************/
static void DrawSquiggle(SegmenT seg, Int4 x, Int4 y, Int2 primID, Boolean is_left)
{
	Int4 space;

	space = GetMuskCParam(MSM_TOPSTYLE, MSM_SPACE, MSM_HEIGHT);
	AddLine(seg, x, y, x, y-space, FALSE, 0);
	if(is_left)
		AddSymbol(seg, x+2, y - space, LEFT_TRIANGLE_SYMBOL, TRUE, LOWER_LEFT, primID);
	else
		AddSymbol(seg, x, y - space, RIGHT_TRIANGLE_SYMBOL, TRUE, LOWER_RIGHT, primID);

}


static Int2 get_arrow_for_strand(Uint1 strand)
{
	switch(strand)
	{
		case Seq_strand_minus:
			return LEFT_ARROW;
		case Seq_strand_plus:
			return RIGHT_ARROW;
		default:
			return NO_ARROW;
	}
}

void add_attribute_pen(SegmenT seg, Int2 p_class, Int2 sub_class)
{
	Uint1 color[3];
	Uint1 flags = COLOR_ATT|STYLE_ATT|SHADING_ATT|WIDTH_ATT;
	Int4  c_val;
	Uint1 linestyle;
	Uint1 penwidth;
	Uint1 shading;

	c_val = GetMuskCParam(p_class, sub_class, MSM_COLOR);
	convert_color(c_val, color);

	linestyle = (Uint1)GetMuskCParam(p_class, sub_class, MSM_LTYPE);
	penwidth = (Uint1)GetMuskCParam(p_class, sub_class, MSM_PENWIDTH);
	shading = (Uint1)GetMuskCParam(p_class, sub_class, MSM_SHADING);

	AddAttribute(seg, flags, &color[0], linestyle, shading, penwidth, COPY_MODE);

}

/*************************************************************************
*
*	DrawFeatNode(vnp, type, rec_width, scale, seg, label_type, fill)
*	Draw a list of featnode, return TRUE for success
*	vnp: a list of featnode
*	rec_width: width for drawing a rectangle. if <2, draw a line
*	scale: max scale
* 	seg: drawing segment
*	label_type: Add label on the segment
*	fill: fill the rectangle?
*
*************************************************************************/

static void AddLabelWithAlign(SegmenT seg, Uint1 label_align, Int4 left, Int4 right, Int4 top, Int4 bottom, CharPtr label, FonT font, Uint2 itemID)
{
	Int4 center, middle;
	

	switch(label_align)
	{
		case MSM_LABEL_TOP:
			center = (left + right)/2;
			AddTextLabel(seg, center, top, label, font, 0, UPPER_CENTER, itemID);
			break;
		case MSM_LABEL_BOTTOM:
			center = (left + right)/2;
			AddTextLabel(seg, center, bottom, label, font, 0, LOWER_CENTER, itemID);
			break;
		case MSM_LABEL_LEFT:
			middle = (top + bottom)/2;
			AddTextLabel(seg, left, middle, label, font, 0, MIDDLE_LEFT, itemID);
			break;
		case MSM_LABEL_RIGHT:
			middle = (top + bottom)/2;
			AddTextLabel(seg, right, middle, label, font, 0, MIDDLE_RIGHT, itemID);
			break;
		default:
			break;
	}
}

static Boolean load_gene_data(GeneDataPtr gdp, FeatNodePtr fnp, Uint2 itemType, Int4 x, Int4 y)
{
	while(gdp)
	{
		if(gdp->entityID == fnp->entityID)
		{
			if(gdp->itemID == fnp->itemID)
			{
				if(gdp->itemType == itemType)
				{
					gdp->x = x;
					gdp->y = y;
					return TRUE;
				}
			}
		}
		gdp = gdp->next;
	}
	return FALSE;
}

static Boolean AddBoxWithAlign(SegmenT seg, Uint1 label_align, FeatNodePtr fnp, Int4 scale, GeneDataPtr gdp, Uint2 itemType)
{
	Int4 center, middle;
	Int4 width;
	Int4 left, right, top, bottom;
	CharPtr label;
	Int4 labelHeight;
	Uint2 itemID;
	Char buf[10];

	if(fnp == NULL || fnp->label == NULL)
		return FALSE;
	left = fnp->extremes.left;
	right = fnp->extremes.right;
	top = fnp->top;
	bottom = fnp->bottom;
	labelHeight = fnp->labelHeight;
	itemID = fnp->itemID;
	label = fnp->label;
	if(labelHeight == 0)	/*the label will NOT be displayed*/
	{
		StringCpy(buf, "XXXX");
		label = buf;
		/* font = (FonT)GetMuskCParam(fnp->feattype, MSM_FLABEL, MSM_FONT);
		SelectFont(font); */
		labelHeight = FontHeight();
		label_align = MSM_LABEL_TOP;
		top -= labelHeight;
			
	}
	
	width = scale * StringWidth(label);
	AddAttribute (seg, COLOR_ATT, YELLOW_COLOR, 0, 0, 0, 0);
	switch(label_align)
	{
		case MSM_LABEL_TOP:
			center = (left + right)/2;
			left = center - width/2;
			right = center + width/2;
			bottom = top;
			top += labelHeight;
			break;
		case MSM_LABEL_BOTTOM:
			center = (left + right)/2;
			left = center - width/2;
			right = center + width/2;
			top = bottom;
			bottom -= labelHeight;
			break;
		case MSM_LABEL_LEFT:
			middle = (top + bottom)/2;
			top = middle - labelHeight/2;
			bottom = middle + labelHeight/2;
			right = left;
			left = left - width;
			
			break;
		case MSM_LABEL_RIGHT:
			middle = (top + bottom)/2;
			top = middle - labelHeight/2;
			bottom = middle + labelHeight/2;
			left  = right;
			right += width;
			break;
		default:
			return FALSE;
	}
	AddRectangle (seg, left, top+5, right, bottom+5, FALSE, TRUE, itemID);
	load_gene_data(gdp, fnp, itemType, left, top);

	/*show the partial label*/
	if(fnp->labelHeight == 0 && fnp->label != NULL)
	{
		center = (fnp->extremes.left + fnp->extremes.right)/2;
		middle = (fnp->top + fnp->bottom)/2;
		AddAttribute (seg, COLOR_ATT, RED_COLOR, 0, 0, 0, 0);
		AddSymbol(seg, center, middle, DIAMOND_SYMBOL, TRUE, MIDDLE_CENTER, fnp->itemID); 
	}
	return TRUE;
}



static void AddFeatNodeLabel(SegmenT seg, FeatNodePtr fnp, Uint1 label_align, FonT font)
{
	if(fnp->labelHeight > 0)
		AddLabelWithAlign(seg, label_align, fnp->extremes.left, fnp->extremes.right, fnp->top, fnp->bottom, fnp->label, font, fnp->itemID);
}

static void AddFeatNodeTickMark(SegmenT seg, FeatNodePtr fnp, Uint1 label_align)
{
	Int4 center, middle;
	Int4 y_pos, x_pos;
	Int4 tick_len;
	

	tick_len = TICK_LEN;
	switch(label_align)
	{
		case MSM_LABEL_TOP:
			center = (fnp->extremes.left + fnp->extremes.right)/2;
			y_pos = fnp->bottom;
			AddLine(seg, center, y_pos, center, y_pos-tick_len, FALSE, fnp->itemID);
			break;
		case MSM_LABEL_BOTTOM:
			center = (fnp->extremes.left + fnp->extremes.right)/2;
			y_pos = fnp->top;
			AddLine(seg, center, y_pos, center, y_pos+tick_len, FALSE, fnp->itemID);
			break;
		case MSM_LABEL_LEFT:
			middle = (fnp->top + fnp->bottom)/2;
			x_pos = fnp->extremes.left;
			AddLine(seg, x_pos, middle, x_pos-tick_len, middle, FALSE, fnp->itemID);
			break;
		case MSM_LABEL_RIGHT:
			middle = (fnp->top + fnp->bottom)/2;
			x_pos = fnp->extremes.right;
			AddLine(seg, x_pos, middle, x_pos+tick_len, middle, FALSE, fnp->itemID);
			break;
		default:
			break;
	}
}


/*gdata is added to record the possible selected marker*/
static Boolean  AddFeatNodeListLabel(SegmenT p_seg, ValNodePtr fnp_node, Uint1 label_align, Int4 scale, GeneDataPtr gdata)
{
	FeatNodePtr fnp;
	Boolean has_prev = FALSE;
	Int2 ptype = 0, _class;
	FonT font = NULL;
	Int4 color_val;
	Uint1 color[3];

	SegmenT seg = NULL;
	Uint2 p_entityID = 0;
	
/*	if(label_align > MSM_LABEL_RIGHT || label_align < MSM_LABEL_TOP)*/
	if(label_align > 3 /* || label_align < 0 */)
		return FALSE;
	while(fnp_node)
	{
		fnp = fnp_node->data.ptrvalue;
		if(!has_prev || fnp->entityID != p_entityID)
		{
			seg = CreateSegment(p_seg, fnp->entityID, 0);
			p_entityID = fnp->entityID;
			has_prev = FALSE;
		}
		if(fnp && fnp->label)
		{
			_class = get_current_class(fnp);
				
			if(!has_prev || ptype != _class)
			{
				/*if(has_prev)
					DeleteFont(font);*/
				font = (FonT)GetMuskCParam(_class, MSM_FLABEL, MSM_FONT);
				SelectFont(font);
				color_val = GetMuskCParam(_class, MSM_FLABEL, MSM_COLOR);
				convert_color(color_val, color);
				AddAttribute(seg, COLOR_ATT, &(color[0]), 0, 0, 0, 0);
			
				has_prev = TRUE;
				ptype = _class;
			}
			if(fnp->landmark)
			{
				AddBoxWithAlign(seg, label_align, fnp, scale, gdata, (Uint2)(fnp_node->choice));
				AddAttribute (seg, COLOR_ATT, color, 0, 0, 0, 0);
			}
			AddFeatNodeLabel(seg, fnp, label_align, font);
		}
		if(fnp->draw_tick)
			AddFeatNodeTickMark(seg, fnp, label_align);
			
		fnp_node = fnp_node->next;
	}
	
	/*if(has_prev)
		DeleteFont(font);*/
	return TRUE;
}

static void DrawGapInterval(SegmenT seg, Int4 left, Int4 right, Int4 top, Int4 bottom, Int4 gap_style, Uint2 itemID)
{
	Int4 center, middle;
	
	switch(gap_style)
	{
		case MSM_GAP_LINE:
			middle = (top + bottom)/2;
			AddLine(seg, left, middle, right, middle, FALSE, itemID);
			break;
		case MSM_GAP_ANGLE:
			center = (left + right)/2;
			middle = (top + bottom)/2;
			AddLine(seg, left, middle, center, top, FALSE, itemID);
			AddLine(seg, center, top, right, middle, FALSE, itemID);
			break;
		default:
			break;
	}
}


static void AddGapForOneFeatNode(FeatNodePtr fnp, Int4Ptr pstyle, Int2Ptr p_class, BoolPtr has_prev, SegmenT seg)
{
	Int4 prev_pos, i_left, i_right;
	Int4 start, stop;
	Int4 style, segstyle;
	ValNodePtr ivals;
	IvalNodePtr inp = NULL;
	Int4 top, bottom;
	Int2 _class;
	Boolean show_gap;

	ivals = fnp->interval;
		
	_class = get_current_class(fnp);
	if(!(*has_prev) || (*p_class) != _class)
	{
		style = GetMuskCParam(_class, MSM_SEGMENT, MSM_STYLE);
		segstyle = (style & MSM_SEG_FORM);
		show_gap = ((segstyle == MSM_SEG_BOX) || (segstyle == MSM_SEG_LINE));
		if(!show_gap)
			return;
			
		add_attribute_pen(seg, _class, MSM_FGAP);
		style = GetMuskCParam(_class, MSM_FGAP, MSM_STYLE);
		*has_prev = TRUE;
		*p_class = _class;
		*pstyle = segstyle;
	}
	else
	{
		segstyle = *pstyle;
		show_gap =(segstyle == MSM_SEG_BOX || segstyle == MSM_SEG_LINE);
		if(!show_gap)
			return;
		else
			style = GetMuskCParam(_class, MSM_FGAP, MSM_STYLE);
			
	}
			

	top = fnp->top;
	bottom = fnp->bottom;
	if(ivals == NULL)
	{
		start = fnp->extremes.left;
		stop = fnp->extremes.right;
		DrawGapInterval(seg, start, stop, top, bottom, style, fnp->itemID);
		return;
	}

	if(fnp->extremes.strand == Seq_strand_minus)
		prev_pos = fnp->extremes.right;
	else
		prev_pos = fnp->extremes.left;
	while(ivals)
	{
		inp = ivals->data.ptrvalue;
		i_left = inp->gr.left;
		i_right = inp->gr.right;
		if(inp->gr.strand == Seq_strand_minus)
		{
			stop = prev_pos;
			start = i_right;
			prev_pos = i_left;
		}
		else
		{
			start = prev_pos;
			stop = i_left;
			prev_pos  = i_right;
		}
		if(stop > start)
			DrawGapInterval(seg, start, stop, top, bottom, style, fnp->itemID);				
		ivals = ivals->next;
	}

	/*check the last piece*/
	if(inp !=NULL)
	{
		if(inp->gr.strand == Seq_strand_minus)
		{
			stop = prev_pos;
			start = fnp->extremes.left;
		}
		else
		{
			start = prev_pos;
			stop = fnp->extremes.right;
		}
		if(stop > start)
			DrawGapInterval(seg, start, stop, top, bottom, style, fnp->itemID);
	}
}


static void AddFeatNodeListGaps(SegmenT p_seg, ValNodePtr fnp_node)
{
	FeatNodePtr fnp;
	Boolean has_prev = FALSE;
	Int2 pclass = -1;
	Int4 style;
	SegmenT seg = NULL;
	Uint2 p_entityID = 0;
	
	while(fnp_node)
	{
		fnp = fnp_node->data.ptrvalue;
		if(!has_prev || fnp->entityID != p_entityID)
		{
			seg = CreateSegment(p_seg, fnp->entityID, 0);
			p_entityID = fnp->entityID;
			has_prev = FALSE;	/*Reset has_prev to reset the style of the current seg*/
		}
		AddGapForOneFeatNode(fnp, &style, &pclass, &has_prev, seg);
		fnp_node = fnp_node->next;
	}
}

static Boolean DrawFeatNodeSegment(FeatNodePtr fnp, Int4 segstyle, Int2 subclass, SegmenT seg, Boolean show_arrow, Boolean show_interval)
{
	Int4 e_left, e_right, end_pos;
	Int4 i_left, i_right;
	Int4 top, bottom;
	Uint1 strand;
	Boolean fill;
	Int2 arrow = NO_ARROW;
	ValNodePtr ivals;
	IvalNodePtr inp;

	if(fnp == NULL)
		return FALSE;	
	if(subclass != MSM_SEGMENT && subclass != MSM_SEG_BORD)
		return FALSE;
	if(segstyle != MSM_SEG_LINE && segstyle != MSM_SEG_BOX)
		return FALSE;
	if(segstyle == MSM_SEG_LINE && subclass == MSM_SEG_BORD)
		return FALSE;
	fill = (subclass == MSM_SEGMENT);
		
	e_left = fnp->extremes.left;
	e_right = fnp->extremes.right;
	top = fnp->top;
	bottom = fnp->bottom;
	strand = fnp->extremes.strand;
	if(fnp->interval == NULL)
	{
		if(show_interval)
			return TRUE;
		if(show_arrow)
			arrow = get_arrow_for_strand(strand);
		if(segstyle == MSM_SEG_BOX)
			AddRectangle(seg, e_left, top, e_right, bottom, arrow, fill, fnp->itemID);
		else
			AddLine(seg, e_left, bottom, e_right, bottom, FALSE, fnp->itemID);
	} 
	if(strand == Seq_strand_minus)
		end_pos = e_left;
	else
		end_pos = e_right;
				
	for(ivals = fnp->interval; ivals !=NULL; ivals = ivals->next)
	{
		arrow = NO_ARROW;
		inp = ivals->data.ptrvalue;
		i_left = inp->gr.left;
		i_right = inp->gr.right;
		if(show_arrow)
		{
			if(strand == Seq_strand_minus && i_left == end_pos)
				arrow = LEFT_ARROW;
			if(strand == Seq_strand_plus && i_right == end_pos)
				arrow = RIGHT_ARROW;
		}
		if(segstyle == MSM_SEG_BOX)
			AddRectangle (seg, i_left, top, i_right, bottom, arrow, fill, fnp->itemID);
		else
			AddLine(seg, i_left, bottom, i_right, bottom, FALSE, fnp->itemID);
	}
	return TRUE;
}

	
static void AddFeatNodeListBorder(SegmenT p_seg, ValNodePtr fnp_node, Boolean show_arrow, Boolean show_interval)
{
	FeatNodePtr fnp;
	Boolean has_prev = FALSE;
	Int2 ptype = 0, _class;
	Boolean show_border = FALSE;
	Int4 style, segstyle = 0;
	Boolean arrow = FALSE;
	SegmenT seg = NULL;
	Uint2 p_entityID = 0;

	
	
	while(fnp_node)
	{
		fnp = fnp_node->data.ptrvalue;
		_class = get_current_class(fnp);
		if(!has_prev || fnp->entityID != p_entityID)
		{
			seg = CreateSegment(p_seg, fnp->entityID, 0);
			p_entityID = fnp->entityID;
			has_prev = FALSE;	/*force reset of the current style*/
		}
		if(!has_prev || ptype != _class)
		{
			/*the border is drawn only when the segment is drawn as a box */
			style = GetMuskCParam(_class, MSM_SEG_BORD, MSM_TRUEFALSE);
			if(style != 0)
			{
				style = GetMuskCParam(_class, MSM_SEGMENT, MSM_STYLE);
				segstyle = (style & MSM_SEG_FORM);
				show_border = (segstyle == MSM_SEG_BOX);
				if(show_border)
				{
					add_attribute_pen(seg, _class, MSM_SEG_BORD);
					if(show_arrow)
						arrow = (Boolean)(style & (Int4)MSM_SEG_SHOWORIENT);
					else
						arrow = FALSE;
				}
			}
			else
				show_border = FALSE;
			has_prev = TRUE;
			ptype = _class;
		}
		if(show_border)
			DrawFeatNodeSegment(fnp, segstyle, MSM_SEG_BORD, seg, arrow, show_interval);
		fnp_node = fnp_node->next;
	}
}

static void AddFeatNodeListSegments(SegmenT p_seg, ValNodePtr fnp_node, Boolean show_arrow, Boolean show_interval)
{
	Int4 style, segstyle = 0;
	FeatNodePtr fnp;
	Boolean has_prev = FALSE;
	Int2 ptype = 0, _class;
	Boolean show_segments = FALSE;
	Boolean arrow = FALSE;
	SegmenT seg = NULL;
	Uint2 p_entityID = 0;
		
	while(fnp_node)
	{
		fnp = fnp_node->data.ptrvalue;
		if(!has_prev || fnp->entityID != p_entityID)
		{
			seg = CreateSegment(p_seg, fnp->entityID, 0);
			p_entityID = fnp->entityID;
			has_prev = FALSE;	/*force to reset the style*/
		}
		_class = get_current_class(fnp);
		if(!has_prev || ptype != _class)
		{

			style = GetMuskCParam(_class, MSM_SEGMENT, MSM_STYLE);
			segstyle = (style & MSM_SEG_FORM);
			if(segstyle == MSM_SEG_BOX || segstyle == MSM_SEG_LINE)
			{
				add_attribute_pen(seg, _class, MSM_SEGMENT);
				has_prev = TRUE;
				ptype = _class;
				show_segments = TRUE;
				if(show_arrow)
					arrow = (Boolean)(style & (Int4)MSM_SEG_SHOWORIENT);
				else
					arrow = FALSE;
			}
		}
		if(show_segments)
			DrawFeatNodeSegment(fnp, segstyle, MSM_SEGMENT, seg, arrow, show_interval);
		fnp_node = fnp_node->next;
	}
}


static ValNodePtr create_flat_node (ValNodePtr fnp_node)
{
	ValNodePtr flat_list = NULL, prev = NULL, curr;
	FeatNodePtr fnp;
	Int4 segstyle, style;
	Int2 _class;

	while(fnp_node)
	{
		if(fnp_node->choice == OBJ_SEQFEAT)
		{
			fnp = fnp_node->data.ptrvalue;
			_class = get_current_class(fnp);
			style = GetMuskCParam(_class, MSM_SEGMENT, MSM_STYLE);
			segstyle = (style & MSM_SEG_FORM);
			if(segstyle != MSM_SEG_BOX && segstyle != MSM_SEG_LINE)
			{
				curr = ValNodeNew(NULL);
				curr->choice = fnp_node->choice;
				curr->data.ptrvalue = fnp;
				if(prev == NULL)
					flat_list = curr;
				else
					prev->next = curr;
				prev = curr;
			}
		}
		fnp_node = fnp_node->next;
	}

	return flat_list;
}

Boolean DrawFeatNode (ValNodePtr fnp_node, SegmenT seg, Uint1 label_type, Boolean show_arrow, Int4 scale, GeneDataPtr gdata)
{
	Uint1 label_align;
	Boolean show_interval = TRUE;
	ValNodePtr flat_list;

	label_align = (Uint1)GetMuskCParam(MSM_TOPSTYLE, MSM_LABEL, MSM_STYLE);
	flat_list = create_flat_node (fnp_node);
	if(flat_list != NULL)
	{
		if(label_type != NO_LABEL)
			AddFeatNodeListLabel(seg, flat_list, label_align, scale, gdata);
		AddFlatNodeSymbol(seg, flat_list, scale);
		ValNodeFree(flat_list);
	}

	if(scale > GetMuskCParam(MSM_TOPSTYLE, MSM_ENDS, MSM_SCALE))
		show_interval = FALSE;
	label_align = (Uint1)GetMuskCParam(MSM_TOPSTYLE, MSM_LABEL, MSM_STYLE);

	if(fnp_node == NULL || fnp_node->choice !=OBJ_SEQFEAT)
		return FALSE;
	if(label_type != NO_LABEL)
		AddFeatNodeListLabel(seg, fnp_node, label_align, scale, gdata);
	if(show_interval)
		AddFeatNodeListGaps(seg, fnp_node);
	AddFeatNodeListSegments(seg, fnp_node, show_arrow, show_interval);
	AddFeatNodeListBorder(seg, fnp_node, show_arrow, show_interval);

		/**if(draw_trunc)
		{			
			if(fnp->extremes.l_trunc)
				DrawSquiggle(seg, e_left, fnp->line, fnp->itemID, TRUE);
			if(fnp->extremes.r_trunc)
				DrawSquiggle(seg, e_right, fnp->line, fnp->itemID, FALSE);
		}**/

	return TRUE;
}

static Uint1 get_bin_symbol(Uint2 bin_order)
{
	switch(bin_order%4)
	{
		case 0:
			return UP_TRIANGLE_SYMBOL;
		case 1:
			return LEFT_TRIANGLE_SYMBOL;
		case 2:
			return DOWN_TRIANGLE_SYMBOL;
		default:
			return RIGHT_TRIANGLE_SYMBOL;
	}
}

Uint1 get_map_type(Uint4 extra)
{
	if(extra & EXTRA_FRAME_WORK)
		return FRAME_WORK;

	if(extra & EXTRA_RECMIN)
		return RECMIN;

	if(extra & EXTRA_LIKELY)
		return LIKELY;

	if(extra & EXTRA_MDUP)
		return MDUP;

	if(extra & EXTRA_DUP)
		return DUP;

	if(extra & EXTRA_CONTIG_STS)
		return CONTIG_STS;

	return 0;
}

static Uint1Ptr get_symbol_color(Uint4 extra)
{
	if(extra & EXTRA_YAC_END)
		return GREEN_COLOR;
	if(extra & EXTRA_RANDOM)
	{	/*this is a gold color */
		custom_color[0] = 255;
		custom_color[1] = 200;
		/* custom_color[1] = 215; */
		custom_color[2] = 0;
		return custom_color;
	}
	if(extra & EXTRA_GENETIC)
		return MAGENTA_COLOR;
	if(extra & EXTRA_GENE)
		return RED_COLOR;
	if(extra & EXTRA_EST)
		return BLUE_COLOR;
	if(extra & EXTRA_MISC) 
		return CYAN_COLOR;
	return BLACK_COLOR;
}

static Boolean  AddFlatNodeSymbol(SegmenT pseg, ValNodePtr fnp_node, Int4 scale)
{
	FeatNodePtr fnp;
	Boolean has_prev = FALSE;
	SegmenT seg = NULL;
	Int2 ptype, _class;
	Int4 symbol_height = 8;
	Int4 middle, center;
	/*Int4 color_val;*/
	Int4 segstyle;
	/*Uint1 color[3];*/
	Uint1Ptr this_color;
	Uint1 symbol_type = 0, symbol;
	/*Uint2 bin_order;*/
	Boolean fill;
	Uint1 map_mark_type;
	Uint2 p_entityID;

	if(fnp_node == NULL)
		return FALSE;
	ptype = 0;
	p_entityID = 0;
	while(fnp_node)
	{
		fnp = fnp_node->data.ptrvalue;
		_class = get_current_class(fnp);
		fnp = fnp_node->data.ptrvalue;
		if(!has_prev || fnp->entityID != p_entityID)
		{
			seg = CreateSegment(pseg, fnp->entityID, 0);
			p_entityID = fnp->entityID;
			has_prev = FALSE;
		}
		if(!has_prev || ptype != _class)
		{
			/*color_val = GetMuskCParam(_class, MSM_SEGMENT, MSM_COLOR);
			convert_color(color_val, color);
			AddAttribute(seg, COLOR_ATT, &(color[0]), 0, 0, 0, 0);*/
			segstyle = GetMuskCParam(_class, MSM_SEGMENT, MSM_STYLE);
			switch(segstyle & MSM_SEG_FORM)
			{
				case MSM_SEG_SYM_RECT:
					symbol_type = RECT_SYMBOL;
					break;
				case MSM_SEG_SYM_DIAMOND:
					symbol_type = DIAMOND_SYMBOL;
					break;
				case MSM_SEG_SYM_OVAL:
					symbol_type = OVAL_SYMBOL;
					break;
				case MSM_SEG_SYM_TRIANGLE:
					symbol_type = UP_TRIANGLE_SYMBOL;
					break;
				default:
					symbol_type = DIAMOND_SYMBOL;
					break;
			}
			
			ptype = _class;
		}
		this_color = get_symbol_color(fnp->extra_data);
		AddAttribute(seg, COLOR_ATT, this_color, 0, 0, 0, 0);
		map_mark_type = get_map_type(fnp->extra_data);
		if(fnp->bin_order != 0)
			symbol = get_bin_symbol(fnp->bin_order);
		else
			symbol = symbol_type;
		fill = TRUE;
		if(map_mark_type == LIKELY || map_mark_type == MDUP || map_mark_type == DUP || map_mark_type == CONTIG_STS)
			fill = FALSE;
		center = (fnp->extremes.left + fnp->extremes.right)/2;
		middle = (fnp->top + fnp->bottom)/2;
		AddSymbol(seg, center, middle, symbol, fill, MIDDLE_CENTER, fnp->itemID);
		if(map_mark_type == FRAME_WORK)
		{
			AddSymbol(seg, center-8*scale, middle, symbol, TRUE, MIDDLE_CENTER, fnp->itemID);
			AddSymbol(seg, center+8*scale, middle, symbol, TRUE, MIDDLE_CENTER, fnp->itemID);
		}
		p_entityID = fnp->entityID;
		fnp_node = fnp_node->next;
	}
	return TRUE;
}

Boolean DrawFlatNode(ValNodePtr vnp, SegmenT seg, Uint1 label_type, Int4 scale, GeneDataPtr gdata)
{
	Uint1 label_align;

	if(label_type == UPPER_LABEL)
		label_align = MSM_LABEL_TOP;
	else
		label_align = MSM_LABEL_BOTTOM;
	AddFeatNodeListLabel(seg, vnp, label_align, scale, gdata);
	AddFlatNodeSymbol(seg, vnp, scale);
	return TRUE;
}


static void AddAlignmentLabel(ValNodePtr anp_list, SegmenT seg)
{
	FonT font;
	Int4 color_val;
	Uint1 color[3];
	AlignNodePtr anp;
	Uint1 label_align;
	Uint2 itemID;
	
	
	font = (FonT)GetMuskCParam(MSM_SEQUENCE, MSM_FLABEL, MSM_FONT);
	SelectFont(font);
	color_val = GetMuskCParam(MSM_SEQUENCE, MSM_FLABEL, MSM_COLOR);
	convert_color(color_val, color);
	AddAttribute(seg, COLOR_ATT|SHADING_ATT, &(color[0]), 0, SOLID_SHADING, 0, COPY_MODE);

	label_align = (Uint1)GetMuskCParam(MSM_TOPSTYLE, MSM_LABEL, MSM_STYLE);
	while(anp_list)
	{
		anp = anp_list->data.ptrvalue;
		if(anp->use_seq_ids)
			itemID = anp->bsp_itemID;
		else
			itemID = anp->itemID;
		AddLabelWithAlign(seg, label_align, anp->extremes.left, anp->extremes.right, anp->top, anp->bottom, anp->label, font, itemID);
		anp_list = anp_list->next;
	}
	/*DeleteFont(font);*/
}


static Uint1Ptr get_color_for_block (AlignNodePtr anp, Uint2 val)
{
	switch(anp->index)
	{
		case 0:
			return get_seg_color((Int2)val);
		case 1:	/*it is a map index */
			if(val == MAP_STATUS_Mapping)
				return MAGENTA_COLOR;
			if(val == MAP_STATUS_Mapped)
				return RED_COLOR;
			if(val == MAP_STATUS_Paused)
				return BLUE_COLOR;
			return BLACK_COLOR;
		case 2:	/*it is a sequence index*/
			if(val == SEQ_STATUS_Plan)
				return CYAN_COLOR;
			if(val == SEQ_STATUS_Sequencing)
				return MAGENTA_COLOR;
			if(val == SEQ_STATUS_Sequenced)
				return RED_COLOR;
			if(val == SEQ_STATUS_Paused)
				return BLUE_COLOR;
			return BLACK_COLOR;
		default:
			return BLACK_COLOR;
	}
}

			
static void AddAlignBlocks(ValNodePtr anp_list, SegmenT seg)
{
	AlignNodePtr anp;
	Uint1Ptr color;
	AlignBlockPtr abp;
	Int2 arrow;
	Int2 c_num;
	Uint2 itemID;

	
	c_num = (Int2)GetMuskCParam(MSM_CCOLOR, MSM_NOSUBCLASS, MSM_NUM);
	while(anp_list)
	{
		anp = anp_list->data.ptrvalue;
		if(anp->use_seq_ids)
			itemID = anp->bsp_itemID;
		else
			itemID = anp->itemID;
		for(abp = anp->blocks; abp != NULL; abp = abp->next)
		{
			arrow = get_arrow_for_strand(abp->gr.strand);
			color = get_color_for_block (anp,(Int2)(abp->order)); 

			/* pseg = CreateSegment(seg, OBJ_BIOSEQ_SEG, 0);
			sseg = CreateSegment(pseg, anp->entityID, 0);
			AddAttribute(sseg, COLOR_ATT, color, 0, 0, 0, 0);
			AddRectangle(sseg, abp->gr.left, anp->top, abp->gr.right, anp->bottom, arrow, TRUE, abp->order); */
			AddAttribute(seg, COLOR_ATT, color, 0, 0, 0, 0);
			AddRectangle(seg, abp->gr.left, anp->top, abp->gr.right, anp->bottom, arrow, TRUE, itemID);
		}
		anp_list = anp_list->next;
	}
}

static void AddSeqWithAlignmentSymbol(SegmenT seg, ValNodePtr anp_list, Int4 scale)
{
	Int4 length, width;
	AlignNodePtr anp;
	Int4 left, right, center;
	Uint2 itemID;

	
	length = scale *4;
	width = GetMuskCParam(MSM_TOPSTYLE, MSM_SPACE, MSM_HEIGHT);
	if(width > 3)
		width = 3;

	while(anp_list)
	{
		anp = anp_list->data.ptrvalue;
		if(anp && anp->seq_has_align == TRUE)
		{
			center = (anp->extremes.left + anp->extremes.right)/2;
			left = center - length/2;
			right = center + length/2;
			if(anp->use_seq_ids)
				itemID = anp->bsp_itemID;
			else
				itemID = anp->itemID;
			AddRectangle(seg, left, anp->top + width, right, anp->top, FALSE, TRUE, itemID);
		}

		anp_list = anp_list->next;
	}

}
/*#######################################################################
#
#	functions related to DrawAlignNode
#
#######################################################################*/


static void AddRepeatsToAlignment (ValNodePtr anp_list, SegmenT seg)
{
	AlignNodePtr anp;
	AlignSegPtr asp;
	Int4 left, right;
	ValNodePtr f_node;
	FeatNodePtr fnp;
	Uint2 itemID;
	Uint1 arrow;

	AddAttribute(seg, COLOR_ATT|SHADING_ATT, BLACK_COLOR, 0, LIGHT_SHADING, 0, 0);
	while(anp_list)
	{
		if(anp_list->choice != OBJ_SEQANNOT)
		{
			anp = anp_list->data.ptrvalue;
			if(anp != NULL &&  anp->segs != NULL)
			{
				if(anp->use_seq_ids)
					itemID = anp->bsp_itemID;
				else
					itemID = anp->itemID;
				for(asp = anp->segs; asp != NULL; asp = asp->next)
				{
					f_node = asp->cnp;
					while(f_node)
					{
						fnp = f_node->data.ptrvalue;
						arrow = NO_ARROW;
						left = fnp->extremes.right;
						right =fnp->extremes.left;
						if(anp->extremes.strand == Seq_strand_minus)
						{
							if(fnp->extremes.left == anp->extremes.left)
								arrow = LEFT_ARROW;
						}
						else
						{
							if(fnp->extremes.right == anp->extremes.right)
								arrow = RIGHT_ARROW;
						}
						
						if(fnp->feattype == FEATDEF_repeat_region || fnp->feattype == FEATDEF_repeat_unit)
							AddRectangle(seg, left, asp->top-1, right, asp->bottom+1, arrow, TRUE, itemID);
						f_node = f_node->next;
					}
				}
			}
		}
		anp_list = anp_list->next;
	}
	AddAttribute(seg, COLOR_ATT|SHADING_ATT, BLACK_COLOR, 0, SOLID_SHADING, 0, COPY_MODE);
}


static void DrawAlignSegs(SegmenT seg, AlignNodePtr anp)
{
	Int4 i_left, i_right, e_left, e_right;
	Int4 p_left = 0, p_right = 0, p_top = 0, p_bottom = 0;
	AlignSegPtr asp;
	Int2 c_arrow, p_arrow = 0;
	Uint1 strand;
	Boolean has_prev = FALSE;
	Boolean load;
	Int4 seglen;
	Uint2 itemID;

	

	/*add each segment in alignment*/
	strand = anp->extremes.strand;
	e_left = anp->extremes.left;
	e_right = anp->extremes.right;
	if(anp->use_seq_ids)
		itemID = anp->bsp_itemID;
	else
		itemID = anp->itemID;
	
	
	if(anp->extremes.l_trunc == TRUE)
		DrawSquiggle(seg, e_left, anp->bottom, itemID, TRUE);
	if(anp->extremes.r_trunc == TRUE)
		DrawSquiggle(seg, e_right, anp->bottom, itemID, FALSE);

	if(anp->segs == NULL)	/*no intervals*/
	{
		c_arrow = get_arrow_for_strand(strand);
		if(ABS(anp->bottom - anp->top) <= 3)
			AddLine(seg, e_left, anp->top, e_right, anp->top, FALSE, itemID);
		else
			AddRectangle(seg, e_left, anp->top, e_right, anp->bottom, c_arrow, FALSE, itemID);
		return;
	}
	


	for(asp = anp->segs; asp !=NULL; asp = asp->next)
	{
		switch(asp->type)
		{
			case INS_SEG:
				seglen = asp->gr.right;
				AddRectangle(seg, asp->gr.left, asp->top, (asp->gr.left+seglen-1), asp->bottom, NO_ARROW, FALSE, itemID);
				AddLine(seg, asp->ins_pos, anp->bottom, asp->ins_pos, asp->top, FALSE, itemID);
				break;
			case GAP_SEG:
				AddLine(seg, asp->gr.left, asp->top, asp->gr.right, asp->top, FALSE, itemID);
				break;
			default:
				i_left = asp->gr.left;
				i_right = asp->gr.right;
				c_arrow = NO_ARROW;
				if(i_left == e_left && strand == Seq_strand_minus)
					c_arrow = LEFT_ARROW;
				if(i_right == e_right && strand == Seq_strand_plus)
					c_arrow = RIGHT_ARROW;
				load = FALSE;
					
				if(has_prev)
				{
					if(p_right+1 == i_left)
					{
						p_right = i_right;
						if(strand != Seq_strand_minus)
							p_arrow = c_arrow;
						load = TRUE;
					}
					else
						AddRectangle(seg, p_left, asp->top, p_right, asp->bottom, p_arrow, FALSE, itemID);
				}
				if(!load)
				{
					p_left = i_left;
					p_right = i_right;
					p_top = asp->top;
					p_bottom = asp->bottom;
					p_arrow = c_arrow;
				}
				has_prev = TRUE;
				break;
		}
	}
	if(has_prev)
		AddRectangle(seg, p_left, p_top, p_right, p_bottom, p_arrow, FALSE, itemID);


}

static void AddAlignmentSegs(ValNodePtr anp_list, SegmenT seg)
{
	AlignNodePtr anp;
	Int2 num;	/*counting the number of the follower groups*/
	Boolean p_follower = FALSE;

	add_attribute_pen(seg, MSM_SEQUENCE, MSM_SEG_BORD);
	num = 0;
	while(anp_list)
	{
		anp = anp_list->data.ptrvalue;
		if(anp->num_follower != 0 )
		{
			++num;
			switch(num%3)
			{
				case 1:
					AddAttribute (seg, COLOR_ATT, BLUE_COLOR, 
						NO_LINE_STYLE, NO_SHADING, 1, NO_MODE);
					break;
				case 2:
					AddAttribute (seg, COLOR_ATT, CYAN_COLOR, 
						NO_LINE_STYLE, NO_SHADING, 1, NO_MODE);
					break;
				default:
					AddAttribute (seg, COLOR_ATT, GREEN_COLOR, 
						NO_LINE_STYLE, NO_SHADING, 1, NO_MODE);
					break;
			}
		}
		else if(num > 0 && anp->follower == FALSE && p_follower == TRUE)
			AddAttribute (seg, COLOR_ATT, BLACK_COLOR, NO_LINE_STYLE, NO_SHADING, 1, NO_MODE);
		p_follower = anp->follower;
		DrawAlignSegs(seg, anp);
		anp_list = anp_list->next;
	}
}



typedef struct alignfeature{
	Uint2 itemID;
	Int4 e_left;
	Int4 e_right;
}AlignFeature, PNTR AlignFeaturePtr;

static void ck_load_itemID (Uint2 itemID, Int4 e_left, Int4 e_right, ValNodePtr PNTR item_list)
{
	ValNodePtr curr;
	AlignFeaturePtr afp;
	
		
	for(curr = *item_list; curr !=NULL; curr = curr->next)
	{
		afp = curr->data.ptrvalue;
		if(afp->itemID == itemID)
		{
			afp->e_left = MIN(afp->e_left, e_left);
			afp->e_right = MAX(afp->e_right, e_right);
			return;
		}
	}

	afp = MemNew(sizeof(AlignFeature));
	afp->e_left = e_left;
	afp->e_right = e_right;
	afp->itemID = itemID;

	ValNodeAddPointer(item_list, 0, (Pointer)afp);
}
	

static ValNodePtr dup_interval_node(ValNodePtr list)
{
	ValNodePtr new = NULL;
	IvalNodePtr inp, n_inp;

	while(list)
	{
		inp = list->data.ptrvalue;
		n_inp = MemNew(sizeof(IvalNode));
		MemCopy((Pointer)n_inp, (Pointer)inp, sizeof(IvalNode));
		ValNodeAddPointer(&new, 0, n_inp);

		list = list->next;
	}

	return new;
}

static ValNodePtr get_last_node(ValNodePtr list)
{
	if(list == NULL)
		return NULL;

	while(list->next != NULL)
		list = list->next;
	return list;
}

static Boolean merge_feature_interval_node(ValNodePtr PNTR i_node, ValNodePtr list, GatherRangePtr grp_i, GatherRangePtr grp_l)
{
	ValNodePtr i_last, l_last, new;
	IvalNodePtr i_inp, l_inp;
	
	
	if(grp_i->right +1 == grp_l->left)
	{
		if(list != NULL)
		{
			if(*i_node == NULL)
				*i_node = dup_interval_node(list);
			else
			{
				i_last = get_last_node(*i_node);
				i_inp = i_last->data.ptrvalue;
				l_inp = list->data.ptrvalue;
				if(i_inp->gr.right +1 ==  l_inp->gr.left)
				{
					i_inp->gr.right = l_inp->gr.right;
					i_last->next = dup_interval_node(list->next);
				}
				else
					i_last->next = dup_interval_node(list);
			}
		}
		grp_i->right = grp_l->right;
		return TRUE;
	}

	if(grp_i->left == grp_l->right +1)
	{
		if(list != NULL)
		{
			if(*i_node == NULL)
				*i_node = dup_interval_node(list);
			else
			{
				new = dup_interval_node(list);
				l_last = get_last_node(list);
				l_inp = l_last->data.ptrvalue;
				i_inp = (*i_node)->data.ptrvalue;
				if(l_inp->gr.right +1 == i_inp->gr.left)
				{
					l_last = get_last_node(new);
					l_inp = l_last->data.ptrvalue;
					l_inp->gr.right = i_inp->gr.right;
					l_last->next = (*i_node)->next;
					(*i_node)->next = NULL;
					ValNodeFreeData(*i_node);
				}
				else
				{
					ValNodeLink(&new, *i_node);
					*i_node = new;
				}
			}

			grp_i->left = grp_l->left;
		}

		return TRUE;
	}

	return FALSE;
}

		

static Boolean is_repeat_feature (Uint1 feattype)
{
	return (feattype == FEATDEF_repeat_region ||feattype == FEATDEF_repeat_unit);
}

static void AddAlignFeature(AlignNodePtr anp, SegmenT a_seg, Boolean show_arrow)
{
	ValNodePtr item_list = NULL, c_list;
	ValNodePtr i_node, p_node;
	Uint2 itemID;
	AlignSegPtr asp;
	FeatNodePtr fnp, c_fnp;
	GatherRange extremes;
	ValNode vn;
	ValNodePtr f_list;
	Uint1 ptype = 0;
	ValNodePtr repeat_node;
	
	SegmenT seg;
	Int4 offset;
	Uint1 strand;
	Boolean arrow;
	AlignFeaturePtr afp;
	

	/*collect a list of feature itemIDs for REG_SEGs because some
	  may need to be merged later */
	/*features in INS_SEG+DIAG_SEG are drawn because they don't need 
	  to be merged */	
	for(asp = anp->segs; asp !=NULL; asp = asp->next)
	{
		if(asp->type == INS_SEG || asp->type == DIAG_SEG)
		{
			if(asp->cnp !=NULL)
			{
				repeat_node = strip_repeat_feature(&(asp->cnp));
				seg = CreateSegment(a_seg, OBJ_SEQFEAT, 0);
				/*not showing any arrow here*/
				DrawFeatNode (asp->cnp, seg, NO_LABEL, FALSE, 1, NULL);
				if(repeat_node != NULL)
					ValNodeLink(&(asp->cnp), repeat_node);
				/*resume the previous layout for FeatNode*/
				if(asp->type == INS_SEG)
				{
					offset = asp->gr.left - asp->ins_pos;
					if(offset != 0)
						modify_insert_fnode(asp->cnp, (-offset));
				}
			}
		}
		if(asp->type == REG_SEG)	/*need some merging process*/
		{
			for(f_list = asp->cnp; f_list !=NULL; f_list = f_list->next)
			{
				fnp = f_list->data.ptrvalue;
				if(!is_repeat_feature (fnp->feattype))
					ck_load_itemID (fnp->itemID, fnp->extremes.left, fnp->extremes.right, &item_list);
			}
		}
	}
	
   if(item_list != NULL)
   {
		
	/*merge the features in REG_SEG according to their itemID*/		
	for(c_list = item_list; c_list != NULL; c_list = c_list->next)
	{
		seg = CreateSegment(a_seg, OBJ_SEQFEAT, 0);
		afp = c_list->data.ptrvalue;
		itemID = afp->itemID;
		c_fnp = NULL;
		i_node = NULL;
		for(asp = anp->segs; asp !=NULL; asp = asp->next)
		{	if(asp->type == REG_SEG)
			{
				for(f_list = asp->cnp; f_list !=NULL; f_list = f_list->next)
				{
					fnp = f_list->data.ptrvalue;
					if(fnp->itemID == itemID && !is_repeat_feature(fnp->feattype))	/*find the match*/
					{
						if(c_fnp == NULL)
						{
							c_fnp = fnp;
							if(fnp->interval != NULL)
								i_node = dup_interval_node(fnp->interval);
							MemCopy(&extremes, &(fnp->extremes), sizeof(GatherRange));
							
						}
						else if(!merge_feature_interval_node(&i_node, fnp->interval, &(c_fnp->extremes), &(fnp->extremes)))
						{
							if(show_arrow)
							{
								strand = c_fnp->extremes.strand;
								if(strand == Seq_strand_minus)
									arrow = (c_fnp->extremes.left == afp->e_left);
								else
									arrow = (c_fnp->extremes.right == afp->e_right);
							}
							else
								arrow = FALSE;
							p_node = c_fnp->interval;
							c_fnp->interval = i_node;
							vn.choice = OBJ_SEQFEAT;
							vn.data.ptrvalue = c_fnp;
							vn.next = NULL;
							DrawFeatNode (&vn, seg, NO_LABEL, arrow, 1, NULL);
							c_fnp->interval = p_node;
							MemCopy((Pointer)&(c_fnp->extremes), (Pointer)&extremes, sizeof(GatherRange));
							if(i_node != NULL)
								i_node = ValNodeFreeData(i_node);
							c_fnp = fnp;
							if(fnp->interval != NULL)
								i_node = dup_interval_node(fnp->interval);
							MemCopy((Pointer)&extremes, (Pointer)&(fnp->extremes), sizeof(GatherRange));
						}
					}
				
				}
			}
		}
			
		if(c_fnp != NULL)
		{
			if(show_arrow)
			{
				strand = c_fnp->extremes.strand;
				if(strand == Seq_strand_minus)
					arrow = (c_fnp->extremes.left == afp->e_left);
				else
					arrow = (c_fnp->extremes.right == afp->e_right);
			}
			else
				arrow = FALSE;
			p_node = c_fnp->interval;
			c_fnp->interval = i_node;
			vn.choice = OBJ_SEQFEAT;
			vn.data.ptrvalue = c_fnp;
			vn.next = NULL;
			DrawFeatNode (&vn, seg, NO_LABEL, arrow, 1, NULL);
			c_fnp->interval = p_node;
			MemCopy((Pointer)&(c_fnp->extremes), (Pointer)&extremes, sizeof(GatherRange));
		}
		if(i_node != NULL)
			i_node = ValNodeFreeData(i_node);
	}

	ValNodeFreeData(item_list);
   }

}

static void AddAlignmentFeature(ValNodePtr anp_node, SegmenT seg, Int4 scale)
{
	AlignNodePtr anp;
	Boolean show_arrow;

	show_arrow = (GetMuskCParam(MSM_TOPSTYLE, MSM_ENDS, MSM_SCALE) >= scale);
	
	while(anp_node)
	{
		anp = anp_node->data.ptrvalue;
		if(anp->segs !=NULL)
			AddAlignFeature(anp, seg, show_arrow);
		anp_node = anp_node->next;
	}
}	

static void AddAlignmentMismatch(ValNodePtr anp_node, Int4 scale, SegmenT seg)
{
	Int4 i_right;
	Int4 top, bottom;
	Int4 p_i, j;
	ValNodePtr curr, list;
	AlignNodePtr anp;
	AlignSegPtr asp;
	Int4 symbol_width = 8;
	Int2 p_type = -1, type;
	Uint2 itemID;

	AddAttribute (seg, WIDTH_ATT | COLOR_ATT, RED_COLOR, NO_LINE_STYLE, NO_SHADING, 1, NO_MODE);
	
	for(list = anp_node; list !=NULL; list = list->next)
	{
		anp = list->data.ptrvalue;
		p_i = -1;
		if(anp->use_seq_ids)
			itemID = anp->bsp_itemID;
		else
			itemID = anp->itemID;
		for(asp = anp->segs; asp !=NULL; asp = asp->next)
		{
			i_right = asp->gr.right;
			top = asp->top;
			bottom = asp->bottom;
			for(curr = asp->mismatch; curr!=NULL; curr = curr->next)
			{
				j = curr->data.intvalue;
				type = curr->choice;
				if(type == 0)
					symbol_width = 1;
				else
					symbol_width = 8;
				if(p_i == -1 || (j-p_i) > scale * symbol_width || p_type != type)
				{
					switch (type)
					{
						case MISMATCH_OPEN:
							AddSymbol(seg, j, (top+bottom)/2, OVAL_SYMBOL, FALSE, MIDDLE_CENTER, itemID);
							break;
						case MISMATCH_CLOSE:
							AddSymbol(seg, j, (top+bottom)/2, OVAL_SYMBOL, TRUE, MIDDLE_CENTER, itemID);
							break;
						case MISMATCH_SQUARE:
							AddSymbol(seg, j, (top+bottom)/2, RECT_SYMBOL, TRUE, MIDDLE_CENTER, itemID);
							break;
						default:
							AddLine(seg, j, asp->top, j, (asp->bottom), FALSE, itemID);
							break;
					}
					p_i = j;
					p_type = type;
				}
			}
		}
	}
}

static ValNodePtr extra_align_node(ValNodePtr anp_node, Boolean use_seq_ids)
{
	ValNodePtr head = NULL;
	AlignNodePtr anp;

	while(anp_node)
	{
		anp = anp_node->data.ptrvalue;
		if(anp->use_seq_ids == use_seq_ids)
			ValNodeAddPointer(&head, anp_node->choice, anp);
		anp_node = anp_node->next;
	}

	return head;
}

/**********************************************************************
*
*	DrawAlignNode(vnp, scale, seg)
*	Draw a list of AlignNode. 
*	1) It always labels the sequences
*	2) the label is always on top of the sequence
*	3) right now, it does NOT show the truncation
*
**********************************************************************/
Boolean DrawAlignNode(ValNodePtr vnp, Int4 scale, SegmenT seg)
{
	ValNodePtr anp_node, curr, next;
	AlignNodePtr anp;
	SegmenT t_seg, p_seg;

	if(vnp == NULL)
		return FALSE;

	anp_node = extra_align_node(vnp, TRUE);
	if(anp_node != NULL)
	{
		p_seg = CreateSegment(seg, OBJ_BIOSEQ, 0);
		curr = anp_node;
		while(curr)
		{
			next = curr->next;
			curr->next = NULL;
			anp = curr->data.ptrvalue;
			t_seg = CreateSegment(p_seg, anp->seq_entityID, 0);
			AddAlignmentSegs(curr, t_seg);
			AddSeqWithAlignmentSymbol(t_seg, curr, scale);
			AddAlignmentMismatch(curr, scale, t_seg);
			AddAlignmentFeature(curr, t_seg, scale);
			AddAlignBlocks(curr, t_seg);	/*add the blocks that are used in segmented seq*/
			AddAlignmentLabel(curr, t_seg);
			AddRepeatsToAlignment (curr, t_seg); 
			curr->next = next;
			curr = next;
		}
		ValNodeFree(anp_node);
	}

		
	anp_node = extra_align_node(vnp, FALSE);
	if(anp_node != NULL)
	{
		AddAlignmentSegs(anp_node, seg);
		AddSeqWithAlignmentSymbol(seg, anp_node, scale);
		AddAlignmentMismatch(anp_node, scale, seg);
		AddAlignmentFeature(anp_node, seg, scale);
		AddAlignBlocks(anp_node, seg);	/*add the blocks that are used in segmented seq*/
		AddAlignmentLabel(anp_node, seg);
		AddRepeatsToAlignment(anp_node, seg); 
		ValNodeFree(anp_node);
	}

	return TRUE;
}
	

Boolean DrawFlatAlign(SegmenT seg, ValNodePtr anp_list)
{
	Uint2 itemID;
	AlignNodePtr anp;
	Int4 pos;

	if(anp_list == NULL)
		return FALSE;
	AddAttribute (seg, WIDTH_ATT | COLOR_ATT, BLACK_COLOR, NO_LINE_STYLE, NO_SHADING, 1, NO_MODE);
	while(anp_list)
	{
		if(anp_list->choice != OBJ_SEQANNOT)
		{
			anp = anp_list->data.ptrvalue;
			itemID = anp->itemID;
			pos = anp->extremes.left;
			
			AddSymbol(seg, pos, anp->line, DIAMOND_SYMBOL, TRUE, UPPER_CENTER, itemID);
	
		}
		anp_list = anp_list->next;
	}

	return TRUE;
}

	

/*************************************************************************
*
*	DrawGeneticMap(features, mpp, pic, scale)
*	draw the genetic map from featnode
*	features: the list of FeatNode, will be resorted
*	mpp: the current map position. The drawing will recalculate
*	mpp->seq_top and mpp->bottom
*	pic: the drawing picture
*	scale: the maximum scale in drawing. Will be used in the layout
*	gdata: to include the possible selected marker
*
*************************************************************************/
Boolean DrawGeneticMap(ValNodePtr PNTR features, MapPosPtr mpp, SegmenT pic, Int4 scale, GeneDataPtr gdata, ValNodePtr PNTR image_list)
{
	SegmenT seg;
	Int4 top;
	ValNodePtr g_node;
	Boolean retval = FALSE;

	seg = CreateSegment(pic, OBJ_BIOSEQ_MAPFEAT, 0);
	g_node = extract_node_list(features, OBJ_BIOSEQ_MAPFEAT, 0, 0, ALL_LABEL);
	top = mpp->seq_top;
	AddAttribute (seg, WIDTH_ATT | COLOR_ATT, BLACK_COLOR, NO_LINE_STYLE, NO_SHADING, 1, NO_MODE);
	if(g_node)
	{
		top = LayoutNodeFlat(&g_node, scale, top, UPPER_LABEL, FALSE);
		if(image_list != NULL)
			load_align_label_rectangle(g_node, image_list, scale, TRUE);
		DrawFlatNode(g_node, seg, UPPER_LABEL, scale, gdata);
		FreeFeatureList(g_node);
		mpp->seq_bottom = mpp->seq_top = top;
		retval = TRUE;
	}
	AddAttribute (seg, WIDTH_ATT | COLOR_ATT, BLACK_COLOR, NO_LINE_STYLE, NO_SHADING, 1, NO_MODE);
	AddLine(seg, mpp->left, mpp->seq_top, mpp->right, mpp->seq_top, FALSE, 0);
	DrawSeqScale(seg, mpp->slp, mpp->left, mpp->seq_top, scale, FALSE);
	mpp->bottom = mpp->seq_bottom - SCALE_SPACE;
	return retval;
}



/*************************************************************************
*
*	DrawPhysicalMap(features, mpp, pic, scale)
*	draw the physical map from featnode
*	features: the list of FeatNode, will be resorted
*	mpp: the current map position. The drawing will recalculate
*	mpp->seq_top and mpp->bottom
*	pic: the drawing picture
*	scale: the maximum scale in drawing. Will be used in the layout
*
*************************************************************************/
Boolean DrawPhysicalMap(ValNodePtr PNTR features, MapPosPtr mpp, SegmenT pic, Int4 scale, ValNodePtr PNTR image_list)
{
	SegmenT pseg, seg;
	Int4 top;
	ValNodePtr g_node;
	Boolean retval = FALSE;

	pseg = CreateSegment(pic, OBJ_BIOSEQ_MAPFEAT, 0);
	seg = CreateSegment(pseg, mpp->entityID, 0);
	g_node = extract_node_list(features, OBJ_SEQFEAT, 0, 0, NUM_LABEL);
	top = mpp->seq_top;
	AddAttribute (seg, WIDTH_ATT | COLOR_ATT, BLACK_COLOR, NO_LINE_STYLE, NO_SHADING, 1, NO_MODE);
	AddLine(seg, mpp->left, top, mpp->right, top, FALSE, 0); 
	if(g_node)
	{
		top = LayoutNodeFlat(&g_node, scale, top, LOWER_LABEL, FALSE);
		if(image_list != NULL)
			load_align_label_rectangle(g_node, image_list, scale, TRUE);
		DrawFlatNode(g_node, pseg, LOWER_LABEL, scale, NULL);
		top -= (TICK_LEN+LABEL_SPACE);
		FreeFeatureList(g_node);
		mpp->seq_bottom = mpp->seq_top;
		retval = TRUE;
	}
	else
		DrawSeqScale(pic, mpp->slp, mpp->left, mpp->bottom, scale, FALSE);

	top -= (LABEL_SPACE+TICK_LEN);
	mpp->bottom = top;
	return retval;
}


static ValNodePtr sort_same_itemID(ValNodePtr head)
{
	FeatNodePtr fnp;
	ValNodePtr list = NULL, new;

	while(head)
	{
		fnp = head->data.ptrvalue;
		new = merge_same_itemID(&head, fnp->itemID);
		ValNodeLink(&list, new);
	}

	return list;
}
	

			
/***********************************************************************
*
*	DrawRestrictionMap(features, mpp, pic, rsite_flat, strand)
*	draw the restriction map
*	features: the FeatNode contains the info for restriction map
*	mpp: the map position
*	pic: picture
*	rsite_flat: if(TRUE), all the enzymes are shown in one line. 
*	strand: the orientation of the map
*
***********************************************************************/
Boolean DrawRestrictionMap(ValNodePtr PNTR features, MapPosPtr mpp, SegmenT pic, Boolean rsite_flat, Uint1 strand, Int4 scale)
{

	Int4 x1, x2, center;
	Int2 arrow;
	ValNodePtr seq_node, g_node, curr, c_head;
	FeatNodePtr fnp;
	IvalNodePtr inp;
	ValNodePtr ivals;
	Int4 top;
	Int2 i;
	Uint1Ptr color;
	Boolean retval = FALSE;
	SegmenT pseg, seg;
	Int4 seq_width;
	FonT font;

	font = (FonT)GetMuskCParam(MSM_SEQUENCE, MSM_SLABEL, MSM_FONT);
	SelectFont(font);
	seq_width = FontHeight();
	/* seq_width = GetMuskCParam(MSM_SEQUENCE, MSM_SEGMENT, MSM_HEIGHT); */
	arrow = get_arrow_for_strand(strand);
	x1 = mpp->left;
	x2 = mpp->right;
	center = (x1+x2)/2;
	top = mpp->seq_top;

	pseg = CreateSegment(pic, OBJ_BIOSEQ_MAPFEAT, 0);
	seg = CreateSegment(pseg, mpp->entityID, 0);
	if(rsite_flat)
	{
		seq_node = extract_node_list(features, OBJ_BIOSEQ, 0, 0, ALL_LABEL);
		fnp = seq_node->data.ptrvalue;
		AddAttribute (seg, WIDTH_ATT | COLOR_ATT, BLACK_COLOR, NO_LINE_STYLE, NO_SHADING, 1, NO_MODE);
		AddRectangle (seg, x1, top, x2, top-seq_width, arrow, FALSE, fnp->itemID);
		AddLabel(seg, x1, top, fnp->label, SMALL_TEXT, 0, LOWER_CENTER, fnp->itemID);
		mpp->seq_top = top;
		mpp->seq_bottom = top - seq_width;
		top -= LABEL_SPACE;
		FreeFeatureList(seq_node);
		mpp->bottom = top;
	}

	g_node = extract_node_list(features, OBJ_BIOSEQ_MAPFEAT, 0, 0, ALL_LABEL);
	if(g_node == NULL)
		return FALSE;
	i = 0;
	while(g_node)
	{
		fnp = (FeatNodePtr)(g_node->data.ptrvalue);
		c_head  = merge_same_itemID(&g_node, fnp->itemID);
		if(c_head)
		{
			if(!rsite_flat)
			{
				(mpp->seq_bottom) -=seq_width;
				top = mpp->seq_bottom + seq_width;
				AddAttribute (seg, WIDTH_ATT | COLOR_ATT,BLACK_COLOR, NO_LINE_STYLE, NO_SHADING, 1, COPY_MODE);
				AddLabel(seg, mpp->left, top+seq_width/2, fnp->label, SMALL_TEXT, 5, LOWER_LEFT, fnp->itemID);
				AddLine(seg, mpp->left, top - seq_width, mpp->right, top-seq_width, FALSE, 0);
				if(i == 0)
					AddLine(seg, mpp->left, top, mpp->right, top, FALSE, 0);
			}

		   for(curr = c_head; curr !=NULL; curr = curr->next)
		   {
			fnp = (FeatNodePtr)(curr->data.ptrvalue);
			color = get_enz_color(i);
			AddAttribute (seg, WIDTH_ATT | COLOR_ATT,color, NO_LINE_STYLE, NO_SHADING, 1, COPY_MODE);
			ivals = fnp->interval;
			while(ivals)
			{
				inp = ivals->data.ptrvalue;
				AddLine(seg, inp->gr.left, (mpp->seq_bottom+seq_width), inp->gr.left, mpp->seq_bottom, FALSE, fnp->itemID);
				ivals = ivals->next;
			}
		   }
		   if(!rsite_flat)
			top = mpp->seq_bottom;
		   ++i;
		   FreeFeatureList(c_head);
		}
	}
	DrawSeqScale(seg, mpp->slp, mpp->left, mpp->seq_bottom, scale, FALSE);
	mpp->bottom = mpp->seq_bottom - SCALE_SPACE;
	return TRUE;
}
		
/***********************************************************************
*
*	DrawCytoMap(features, mpp, scale, pic)
*	draw the cytogenetic map
*	features: the FeatNode for cytogenetic map
*	scale: maximum scale, used in layout
*	pic: the drawing segment
*
***********************************************************************/
static Boolean label_cyto_band(Int4 x1, Int4 x2, Int4Ptr prev, CharPtr label, Int4 scale)
{
	Int4 center;
	Int4 half_len;

	half_len = StringWidth(label)/2;
	center = (x1+x2)/(2 * scale);
	if((*prev == -1) || ((center - half_len) > (*prev)))
	{
		*prev = center + half_len;
		return TRUE;
	}
	else
		return FALSE;
}

Boolean DrawCytoMap(ValNodePtr PNTR features, MapPosPtr mpp, Int4 scale, SegmenT pic)
{
	Int4 x1, x2, center;
	SegmenT seg, pseg;
	Int4 top;
	Int4 prev_pos = -1;
	ValNodePtr g_node, curr;
	FeatNodePtr fnp;
	Boolean retval = FALSE;
	Uint1Ptr color = NULL;
	Uint1 shading = 0;
	CharPtr subdiv = NULL;
	Int2 numdiv =0;
	Int4 prev_right = -1;
	Int4 seq_width;


	top = mpp->seq_top;
	seq_width = GetMuskCParam(MSM_SEQUENCE, MSM_SEGMENT, MSM_HEIGHT);

	pseg = CreateSegment(pic, OBJ_BIOSEQ_MAPFEAT, 0);
	seg = CreateSegment(pseg, mpp->entityID, 0);

	g_node = extract_node_list(features, OBJ_BIOSEQ_MAPFEAT, 0, 0, ALL_LABEL);
	if(g_node)
	{
		for(curr = g_node; curr!=NULL; curr = curr->next)
		{
			fnp = curr->data.ptrvalue;
			switch(fnp->band)
			{
				case BND:
					color = BLACK_COLOR;
					break;
				case HET :
					color = BLUE_COLOR;
					break;
				case TEL:
				case CEN:
					color = RED_COLOR;
					shading = SOLID_SHADING;
					break;
				case BAND_POINT:
					color = RED_COLOR;
					shading = SOLID_SHADING;
					break;
				case GIEMSA_POS:
					color = BLACK_COLOR;
					shading = SOLID_SHADING;
					break;
				case GIEMSA_NEG:
					color = WHITE_COLOR;
					shading = SOLID_SHADING;
					break;
				case ACRO_CENTRIC:
					color = BLACK_COLOR;
					shading = THICK_NWSE_SHADING;
					break;
				case VARIABLE_REG:
					color = BLACK_COLOR;
					shading = THIN_NESW_SHADING;
					break;

				case MISC_BND:
					color = BLACK_COLOR;
					shading = SOLID_SHADING;
					break;
				
				default:
					Message(MSG_ERROR, "Illegal type");
					exit(1);
			}

			if(fnp->band == BND || fnp->band == HET)
			{
				if(numdiv == 0)
				{
					++numdiv;
					subdiv = fnp->pos_label;
				}
				else
				{
					if(StringCmp(subdiv, fnp->pos_label))
					{
						++numdiv;
						subdiv = fnp->pos_label;
					}
				}
				if(numdiv%2 == 0)
					shading = THIN_NESW_SHADING;
				else
					shading = THIN_NWSE_SHADING;
			}
			
			x1= fnp->extremes.left;
			x2= fnp->extremes.right;
			if(prev_right !=-1 && (x1 - prev_right) > scale)
			{
				AddAttribute (seg, SHADING_ATT | COLOR_ATT, BLACK_COLOR, NO_LINE_STYLE, SOLID_SHADING, 1, COPY_MODE);
				AddLine(seg, prev_right, top, x1, top, FALSE, 0);
				AddLine(seg, prev_right, top-seq_width, x1, top-seq_width, FALSE, 0);
			}
			prev_right = x2;

			if(fnp->band == MISC_BND)
			{
				AddAttribute (seg, COLOR_ATT|SHADING_ATT, BLACK_COLOR, NO_LINE_STYLE, SOLID_SHADING, 1, COPY_MODE);
				AddRectangle(seg, x1, top, x2, top-seq_width, NO_ARROW, FALSE, fnp->itemID);
			}
			else
			{
				AddAttribute (seg, SHADING_ATT | COLOR_ATT, color, NO_LINE_STYLE, shading, 1, COPY_MODE);
				AddRectangle(seg, x1, top, x2, top-seq_width, NO_ARROW, TRUE, fnp->itemID);
				if(fnp->band == GIEMSA_NEG)
					color = BLACK_COLOR;
				AddAttribute (seg, SHADING_ATT | COLOR_ATT, color, NO_LINE_STYLE, SOLID_SHADING, 1, COPY_MODE);
				AddRectangle(seg, x1, top, x2, top-seq_width, NO_ARROW, FALSE, fnp->itemID);
			}
			if(fnp->label)
			{
				if(label_cyto_band(x1, x2, &prev_pos, fnp->label, scale))
				{
   					center = (x1+ x2)/2;
	    	 			AddLabel(seg, center, top-seq_width-5, fnp->label, SMALL_TEXT, 0, LOWER_CENTER, fnp->itemID);
				}
			}
		}
		mpp->seq_top = top;
		mpp->seq_bottom = top - seq_width;

		top -= ( seq_width + LABEL_SPACE+5);
		mpp->bottom = top;
		FreeFeatureList(g_node);
		retval = TRUE;
	}

	return retval;
}


	
static void draw_sequence_fnp(FeatNodePtr fnp, Boolean fill, Uint1 label_align, SegmenT seg, Uint1Ptr color, Int4 segstyle, Boolean show_border)
{
	Int2 arrow;
	FonT font;
	Int4 c_val;
	Uint1 label_color[3];
	Uint1 flags = COLOR_ATT|STYLE_ATT|SHADING_ATT|WIDTH_ATT;
	Uint1 linestyle, penwidth, shading;


	if(fnp->label != NULL)
	{

		font = (FonT)GetMuskCParam(MSM_SEQUENCE, MSM_SLABEL, MSM_FONT);
		SelectFont(font);
		fnp->labelHeight = FontHeight();
	
		c_val = GetMuskCParam(MSM_SEQUENCE, MSM_SLABEL, MSM_COLOR);
		convert_color(c_val, label_color);
		AddAttribute (seg, COLOR_ATT, &label_color[0], 0, 0, 0, 0);
		AddFeatNodeLabel(seg, fnp, label_align, font);
	}

	arrow = get_arrow_for_strand(fnp->extremes.strand);

	if(color == NULL)
	{
		if(segstyle == MSM_SEG_BOX)
			add_attribute_pen(seg, MSM_SEQUENCE, MSM_SEGMENT);
		else
			add_attribute_pen(seg, MSM_SEQUENCE, (Int2)segstyle);
	}
	else	/*the one's that uses cycle color*/
	{
		linestyle = (Uint1)GetMuskCParam(MSM_SEQUENCE, (Int2)segstyle, MSM_LTYPE);
		penwidth = (Uint1)GetMuskCParam(MSM_SEQUENCE, (Int2)segstyle, MSM_PENWIDTH);
		shading = (Uint1)GetMuskCParam(MSM_SEQUENCE, (Int2)segstyle, MSM_SHADING);
		AddAttribute(seg, flags, color, linestyle, shading, penwidth, COPY_MODE);
	}

	if(fill)
	{

		if(segstyle == MSM_SEG_LINE)
			AddLine(seg, fnp->extremes.left, fnp->top, fnp->extremes.right, fnp->top, FALSE, fnp->itemID);
		else
			AddRectangle (seg, fnp->extremes.left, fnp->top, fnp->extremes.right, fnp->bottom, arrow, fill, fnp->itemID);
	}

	if(show_border || !fill)
	{
		add_attribute_pen(seg, MSM_SEQUENCE, MSM_SEG_BORD);
		AddRectangle (seg, fnp->extremes.left, fnp->top, fnp->extremes.right, fnp->bottom, arrow, FALSE, fnp->itemID);
	}

}


/*********************************************************************
*
*	DrawSeqMap(features, mpp, pic, flat, scale, is_raw_seq)
*	Draw the real sequence map
*	features: FeatNode from which the Bioseqs can be extracted
*	mpp: the MapPos that contains the current map position
*	pic: the picture
*	flat: if TRUE, draw the map with features stacked on top
*	scale: maximum scale of the sequence
*	is_raw_seq: if TRUE, it is a raw DNA sequence. (This is 
*	used to distinguish raw sequence from virtual sequence
*	if(show_segment and image_list), record the positions of the 
*	segments
*
*********************************************************************/
Boolean DrawSeqMap(ValNodePtr PNTR features, MapPosPtr mpp, SegmenT pic, Int4 scale, Boolean is_raw_seq, Boolean show_segment, ValNodePtr PNTR image_list)
{
	ValNodePtr seg_node = NULL, g_node, c_node;
	ValNodePtr prev, next, gap_node = NULL;
	Int4 middle;
	FeatNodePtr fnp, c_fnp;
	SegmenT seg, pseg;
	Uint1Ptr color;
	Boolean fill;
	Int4 top;
	Int4 e_left, e_right;
	Uint2 entityID;
	Int2 arrow;
	Uint1 label_align;
	Uint1 seq_label_align;
	Int4 style, segstyle;
	Boolean show_border;
	Int2 obj_bioseq_seg_or_delta;



	entityID = (Uint2)mpp->entityID;
	label_align = (Uint1)GetMuskCParam(MSM_TOPSTYLE, MSM_LABEL, MSM_STYLE);

	c_node = extract_node_list(features, OBJ_BIOSEQ, entityID, 0, ALL_LABEL);
	c_node = sort_same_itemID(c_node);
	if(c_node == NULL)
		return FALSE;
	c_fnp = c_node->data.ptrvalue;
	c_fnp->line = 0;

	style = GetMuskCParam(MSM_SEQUENCE, MSM_SEGMENT, MSM_STYLE);
	segstyle = (style & MSM_SEG_FORM);
	if(segstyle != MSM_SEG_BOX && segstyle != MSM_SEG_LINE)
		segstyle = MSM_SEG_BOX;
	if(segstyle ==MSM_SEG_BOX)
		show_border = (Boolean)GetMuskCParam(MSM_SEQUENCE, MSM_SEG_BORD, MSM_TRUEFALSE);
	else
		show_border = FALSE;

	top = LayoutFeatNode(c_node, mpp->seq_top, scale, 0, TRUE, FALSE);
	obj_bioseq_seg_or_delta = OBJ_BIOSEQ_SEG;
	seg_node = extract_node_list(features, OBJ_BIOSEQ_SEG, entityID, 0, ALL_LABEL);
	if(seg_node == NULL) {
		obj_bioseq_seg_or_delta = OBJ_BIOSEQ_DELTA;
		seg_node = extract_node_list(features, OBJ_BIOSEQ_DELTA, entityID, 0, ALL_LABEL);
	}
	fill = (is_raw_seq && seg_node == NULL);

	pseg = CreateSegment(pic, OBJ_BIOSEQ, 0);
	seg = CreateSegment(pseg, entityID, 0);
	if(label_align != MSM_LABEL_RIGHT)
		seq_label_align = MSM_LABEL_LEFT;
	else
		seq_label_align = MSM_LABEL_RIGHT;
	
	draw_sequence_fnp(c_fnp, fill, seq_label_align, seg, NULL, segstyle, show_border);
	/* if(image_list != NULL)
	{
		vn.choice = OBJ_BIOSEQ;
		vn.data.ptrvalue = c_fnp;
		vn.next = NULL;
		load_align_label_rectangle(&vn, image_list, scale, FALSE);
	} */
	mpp->seq_top = c_fnp->top;
	mpp->seq_bottom = c_fnp->bottom;


	/*add the segments to the sequence picture. Also get rid of the 
	FeatNode that only contains the gaps. if(fnp->follower = TRUE), it 
	is a gap */

	g_node = seg_node;
	prev = NULL;
	gap_node = NULL;
	while(g_node)
	{
		next = g_node->next;
		arrow = NO_ARROW;
		fnp = g_node->data.ptrvalue;
		e_left = fnp->extremes.left;
		e_right = fnp->extremes.right;
		if(fnp->follower)	/*it is a gap between the segment*/
		{
			if(prev == NULL)
				seg_node = g_node->next;
			else
				prev->next = g_node->next;
			g_node->next = NULL;
			ValNodeLink(&gap_node, g_node);
		}
		else
		{
			if(e_left == c_fnp->extremes.left && c_fnp->extremes.strand == Seq_strand_minus)
				arrow = LEFT_ARROW;
			if(e_right == c_fnp->extremes.right && c_fnp->extremes.strand == Seq_strand_plus)
				arrow = RIGHT_ARROW;
			color = get_seg_color(fnp->itemID);
			AddAttribute (seg, COLOR_ATT, color, 0, 0, 0, COPY_MODE);
			if(segstyle == MSM_SEG_BOX)
				AddRectangle (seg, e_left, c_fnp->top, e_right, c_fnp->bottom, arrow, TRUE, c_fnp->itemID);
			else
				AddLine(seg, e_left, c_fnp->top, e_right, c_fnp->top, FALSE, c_fnp->itemID);
			prev = g_node;
		}
		g_node = next;
	}


	if(gap_node != NULL)
	{
		AddAttribute (seg, WIDTH_ATT | COLOR_ATT, BLACK_COLOR, NO_LINE_STYLE, NO_SHADING, 2, NO_MODE);
		middle = (c_fnp->top + c_fnp->bottom)/2;
		for(g_node = gap_node; g_node != NULL; g_node = g_node->next)
		{
			fnp = g_node->data.ptrvalue;
			e_left = fnp->extremes.left;
			AddSymbol(seg, e_left, middle, DIAMOND_SYMBOL, FALSE, MIDDLE_CENTER, fnp->itemID);
		}
		FreeFeatureList(gap_node);
	}

	DrawSeqScale(seg, mpp->slp, mpp->left, mpp->seq_bottom, scale, FALSE);
	top -= SCALE_SPACE;	/*space for drawing the scale */
	if(seg_node !=NULL && show_segment)
	{
		top -= GetMuskCParam(MSM_TOPSTYLE, MSM_SPACE, MSM_HEIGHT);
		pseg = CreateSegment(pic, obj_bioseq_seg_or_delta, 0);
		seg = CreateSegment(pseg, entityID, 0);
		
		ReSetFeatNode(seg_node);
		top = LayoutFeatNode(seg_node, top, scale, 0, TRUE, FALSE);
		
		for(g_node = seg_node; g_node !=NULL; g_node = g_node->next)
		{
			fnp = g_node->data.ptrvalue;
			e_left = fnp->extremes.left;
			e_right = fnp->extremes.right;
			color = get_seg_color(fnp->itemID);
			draw_sequence_fnp(fnp, TRUE, label_align, seg, color, segstyle, show_border);
		}
		if(image_list != NULL)
			load_align_label_rectangle(seg_node, image_list, scale, FALSE); 
	}


	mpp->bottom = top;

	FreeFeatureList(c_node);
	FreeFeatureList(seg_node);
	return TRUE;
}



static Int4 DrawOneFeatNode(ValNodePtr g_node, Int4 top, SegmenT seg,  Int4 scale, Int2 group_num, GeneDataPtr gdata, ValNodePtr PNTR image_rect)
{
	Boolean show_arrow;
	Boolean compress;


	if(g_node != NULL)
	{

		show_arrow = (GetMuskCParam(MSM_TOPSTYLE, MSM_ENDS, MSM_SCALE) >= scale);
 		compress = (GetMuskCParam(MSM_GROUPS, group_num, MSM_STYLE) == MSM_COMPRESS);
 		
		top -= GetMuskCParam(MSM_TOPSTYLE, MSM_SPACE, MSM_HEIGHT);

		top = LayoutFeatNode(g_node,top, scale, scale, TRUE, compress);

		if(image_rect)
			load_align_label_rectangle(g_node, image_rect, scale, FALSE);
		DrawFeatNode (g_node, seg, UPPER_LABEL, show_arrow, scale, gdata);
	}

	return top;
}

/******************************************************************
*
*	ck_ordered_group(): check if the internal order is important
*	to a group. Currently, only set up for the Gene-CDS-mRNA feature
*
*******************************************************************/
static Boolean ck_ordered_group(Uint1 group_num, Uint1Ptr groupOrder)
{
	Uint2 i;

	if(groupOrder == NULL)
		return FALSE;

	for(i =0; i<FEATDEF_ANY; ++i)
	{
		if(groupOrder[i] == group_num)
		{
			if(i == FEATDEF_GENE || i == FEATDEF_mRNA || i == FEATDEF_CDS)
				return TRUE;
		}
	}
	return FALSE;
}


static Boolean fnp_from_same_annot(FeatNodePtr fnp, CharPtr annot_db)
{
	if(annot_db[0] == '\0')
		return (fnp->annotDB[0] == '\0');
	else
		return (StringCmp(fnp->annotDB, annot_db) ==0);
}

static ValNodePtr group_feature_by_annot_label(ValNodePtr PNTR g_node, CharPtr annot_db)
{
	ValNodePtr list = NULL;
	FeatNodePtr fnp;
	ValNodePtr curr, next, prev = NULL;

	if(g_node == NULL || *g_node == NULL)
		return NULL;

	curr = *g_node;
	fnp = curr->data.ptrvalue;
	if(fnp == NULL)
		return NULL;
	if(fnp->annotDB[0] == '\0')
		annot_db[0] = '\0';
	else
		StringCpy(annot_db, fnp->annotDB);

	while(curr)
	{
		next = curr->next;
		fnp = curr->data.ptrvalue;
		if(fnp_from_same_annot(fnp, annot_db))
		{
			if(prev == NULL)
				*g_node = next;
			else
				prev->next = next;
			curr->next = NULL;
			ValNodeLink(&list, curr);
		}
		else
			prev = curr;
		curr = next;
	}

	return list;
}
			
static void AddAnnotGroup(Int4 left, Int4 top, Int4 right, Int4 bottom, Int2 order, CharPtr annot_db, Uint2 itemID, SegmenT seg)
{
	Uint1Ptr color;
	Int4 center;

	color = get_seg_color(order);
	AddAttribute (seg, COLOR_ATT, color, 0, 0, 0, 0);

	/*need to draw the rectangle line by line. Otherwise, the segments 
	will be incorrect*/
	AddLine(seg, left, top, right, top, FALSE, itemID);
	AddLine(seg, left, bottom, right, bottom, FALSE, itemID);
	AddLine(seg, left, top, left, bottom, FALSE, itemID);
	AddLine(seg, right, top, right, bottom, FALSE, itemID);

	if(annot_db != NULL && annot_db[0] != '\0')
	{
		AddAttribute (seg, COLOR_ATT, BLACK_COLOR, 0, 0, 0, 0);
		center = (left + right)/2;
		AddTextLabel(seg, center, top, annot_db, programFont, 0, MIDDLE_CENTER, itemID);
	}
}
	


static Boolean is_featnode_lod_score (ValNodePtr fnp_list)
{
	FeatNodePtr fnp;

	while(fnp_list)
	{
		if(fnp_list->choice == OBJ_SEQFEAT || fnp_list->choice == OBJ_BIOSEQ_MAPFEAT)
		{
			fnp = fnp_list->data.ptrvalue;
			if(fnp != NULL)
			{
				if((fnp->extra_data) & EXTRA_LOD_SCORE)
					return TRUE;
				else
					return FALSE;
			}
		}

		fnp_list = fnp_list->next;
	}

	return FALSE;
}


static Uint1Ptr GetLodScoreBitColor(Uint1 bit_val)
{
	switch(bit_val)
	{
		case 1:
			return BLUE_COLOR;
		case 2:
			return CYAN_COLOR;
		case 3:
			return GREEN_COLOR;
		case 4:
			return YELLOW_COLOR;
		case 5:
			return MAGENTA_COLOR;
		case 6:
			return RED_COLOR;
		default:
			return BLACK_COLOR;
	}
}

static Int4 DrawLODScore(ValNodePtr t_node, SegmenT seg, Int4 scale, Int4 top)
{
	Int4 height;
	FeatNodePtr fnp;
	Uint1Ptr color;
	Int4 space;

	height = GetMuskCParam(MSM_SEQUENCE, MSM_SEGMENT, MSM_HEIGHT);
	space = GetMuskCParam(MSM_TOPSTYLE, MSM_SPACE, MSM_HEIGHT);
	top -= space;

	while(t_node)
	{
		fnp = t_node->data.ptrvalue;
		if(fnp != NULL)
		{
			color = GetLodScoreBitColor((Uint1)(fnp->bin_order));
			AddAttribute (seg, COLOR_ATT, color, 0, 0, 0, 0);
			AddRectangle (seg, fnp->extremes.left, top, 
				fnp->extremes.right, top-height, FALSE, TRUE, 0);
		}

		t_node = t_node->next;
	}

	return (top - space - height);
}
		

static Int4 DrawFeatureWithAnnot(ValNodePtr PNTR t_head, Int4 top, Int4 scale, Boolean flat, Int4 group_num, Boolean order_group, Int4 left, Int4 right, GeneDataPtr gdata, SegmenT seg, ValNodePtr PNTR image_list)
{
	Char annot_db[100];
	Int4 g_top = 0, g_bottom;
	Int4 group_font_height;
	Boolean draw_annot_group;
	Int2 annot_order;
	ValNodePtr t_node;

	SelectFont(programFont);
	group_font_height = FontHeight();

	annot_order = 0;
	while((t_node = group_feature_by_annot_label(t_head, annot_db)) != NULL)
	{
		if(*t_head  == NULL && annot_order == 0 && annot_db[0] == '\0')
			draw_annot_group = FALSE;
		else
			draw_annot_group = TRUE;
		if(draw_annot_group )
		{
			if(annot_db[0] == '\0')
				g_top = top;
			else
			{
				top -= group_font_height;
				g_top = top + group_font_height/2;
			}
		}
		if(is_featnode_lod_score (t_node))
			top = DrawLODScore(t_node, seg, scale, top);
		else
		{
			if(flat)
			{
				top = LayoutNodeFlat(&t_node, scale, top, UPPER_LABEL, TRUE);
				if(image_list != NULL)
					load_align_label_rectangle(t_node, image_list, scale, TRUE);
				DrawFlatNode(t_node, seg, UPPER_LABEL, scale, gdata);
			}
			else
			{
				if(order_group)
					/* GroupFeatNode(&t_node); */
					t_node = OrderCdsmRNA(&t_node);
				else
					t_node= SortFeatNode(t_node, NULL, NULL);
				top = DrawOneFeatNode(t_node, top, seg, scale, (Int2)group_num, gdata, image_list);
			}
		}
		t_node= FreeFeatureList(t_node);
		g_bottom = top;

		if(draw_annot_group)
		{
			g_bottom -= group_font_height/2;
			AddAnnotGroup(left, g_top, right, g_bottom, annot_order, annot_db, 0, seg);
			top = g_bottom;
		}
		++annot_order;
	}

	return top;
}

/***********************************************************************
*
*	DrawFeatures(features, mpp, pic, flat, simple, compact, 
*	scale)
*
*	Draw the features in the a sequence
*	features: the FeatNdoe contains the features
*	mpp: the graphic position of the current sequence
*	pic: the drawing segment
*	flat: if TRUE, stack features on top
*	simple: if TRUE, draw the feature intervals as line
*	compact: if TRUE, does not distinguish subtype of different features
*	scale: the maximum scale for drawing
*       image_list: the list of AlignRect to store the image information
*
*********************************************************************/
Boolean DrawFeatures(ValNodePtr g_node, MapPosPtr mpp, SegmenT pic, Boolean flat, Uint1Ptr featureOrder, Uint1Ptr groupOrder, Int4 scale, GeneDataPtr gdata, ValNodePtr PNTR image_list)
{
	ValNodePtr t_node, t_head;
	Int4 top;
	Int4 left, right;
	Int2 entityID;
	SegmenT seg;
	Uint1 i, j, k, maxGroup;
	Boolean is_end = FALSE;
	Boolean order_group;	/*need to show the order of the group*/

	if(g_node == NULL)
		return FALSE;
	left = mpp->left;
	right = mpp->right;
	top = mpp->bottom;
	entityID = mpp->entityID;
	seg = CreateSegment(pic, OBJ_SEQFEAT, 0);

	if(flat)	/*show the features first*/
	{
		mpp->bottom = DrawFeatureWithAnnot(&g_node, top, scale, TRUE, 0, FALSE, left, right, gdata, seg, image_list);
	}
	else
	{
		maxGroup= 0;
		for(k = 0; k<FEATDEF_ANY; ++k)
			maxGroup = MAX(maxGroup, groupOrder[k]);
			
		is_end = FALSE;
		order_group = FALSE;

		for(k =1; k<=maxGroup && !is_end; ++k)
		{
			order_group = ck_ordered_group(k, groupOrder);
			t_head = NULL;
			for(i =1; i<FEATDEF_ANY && !is_end; ++i)	/*this is the order of features*/
			{
				for(j = 1; j<FEATDEF_ANY && !is_end; ++j)
				{
					if(groupOrder[j] == k && featureOrder[j] == i)
					{
						t_node = extract_node_list(&g_node, OBJ_SEQFEAT, 0, j, ALL_LABEL);
						if(t_node != NULL)
						{
							ValNodeLink(&t_head, t_node);
							is_end = (g_node == NULL);
						}
					}
				}
			}

			top = DrawFeatureWithAnnot(&t_head, top, scale, FALSE, k,order_group, left, right, gdata, seg, image_list);
		}
		mpp->bottom = top;
		if(g_node != NULL)
			FreeFeatureList(g_node);
	}
				
	return TRUE;
}



Boolean DrawHistory(ValNodePtr aligns, MapPosPtr mpp, Int4 seq_label_len, SegmenT pic, Int4 scale, ValNodePtr PNTR image_list)
{
	
	Boolean retval;
	Uint1 label_align;
	Int4 left, right;
	Int4 offset;
 
	if(aligns)
	{
		(mpp->bottom) -= 20;

		offset = scale * (MAX(seq_label_len, 8));
		label_align = (Uint1)GetMuskCParam(MSM_TOPSTYLE, MSM_LABEL, MSM_STYLE);
		
		left = mpp->left;
		right = mpp->right;
		switch(label_align)
		{
			case MSM_LABEL_RIGHT:
				left -= (8 * scale);
				right += offset;
				break;

			case MSM_LABEL_TOP:
			case MSM_LABEL_BOTTOM:
				left -= (8*8*scale);
				right += (8*8*scale);
				break;

			case MSM_LABEL_LEFT:
				left -=offset;
				right += (8 * scale);	/*8 is the size of a symbol*/
				break;

			default:
				break;
		}
		retval = DrawMPAlignment(aligns, left, right, NULL, mpp->entityID, scale, &(mpp->bottom), MSM_SEQHIST, FALSE, pic);
		if(image_list != NULL)
			load_align_label_rectangle (aligns, image_list, scale, FALSE);
		FreeAlignNode(aligns);
		return retval;
	}
	else
		return FALSE;
}


static MapLayoutPtr get_next_ypos(MapLayoutPtr mlp, Int4Ptr top)
{
	MapPosPtr mpp;

	if(mlp == NULL)
	{
		*top = -20;
		return NULL;
	}
	else
	{
		while(mlp->next != NULL)
			mlp = mlp->next;
		mpp = mlp->data.ptrvalue;
		*top = mpp->bottom -MAX(10, GetMuskCParam(MSM_TOPSTYLE, MSM_SPACE, MSM_HEIGHT));
		return mlp;
	}
}



static Boolean set_seq_collect_option(CollectSeqOptionPtr csop, Int4 scale)
{
	Int4 topstyle;
	Int4 ends;
	Int2 i;

	if(csop == NULL || scale <= 0)
		return FALSE;
		
	topstyle = GetMuskCParam(MSM_TOPSTYLE, 0, MSM_STYLE);

	/*collect the features, maps, and features*/
	csop->seglevels = (topstyle & MSM_SEGMENTS) ? 1:0;
	ends = GetMuskCParam(MSM_TOPSTYLE, MSM_ENDS, MSM_SCALE);
	csop->nointerval = (ends < scale);
	csop->slabel_format = (Uint1)GetMuskCParam(MSM_SEQUENCE, MSM_SLABEL, MSM_STYLE);
	for(i =0; i<FEATDEF_ANY; ++i)
		csop->flabel_format[i] = (Uint1)GetMuskCParam(i, MSM_FLABEL, MSM_STYLE);
	csop->label_size = (Int2)GetMuskCParam(MSM_TOPSTYLE, MSM_LABEL, MSM_NUM);
	csop->filter_level = 0;
		
	for( i =0; i<FEATDEF_ANY; ++i)	/*for checking the features to load*/
		csop->features[i] = (GetMuskCParam(i, MSM_FORDER, MSM_NUM) !=0);
		
	return TRUE;
}


/*add the label to draw the sequence*/
static Int4 add_seq_label_for_map(ValNodePtr PNTR features, MapPosPtr mpp, SegmenT pic)
{
	ValNodePtr c_node;
	FeatNodePtr fnp;
	FonT font;
	Int4 c_val;
	Uint1 label_color[3];
	SegmenT seg, pseg;
	Uint1 label_align;
	Int4 label_len;
	
	label_len = 0;
	c_node = extract_node_list(features, OBJ_BIOSEQ, mpp->entityID, 0, ALL_LABEL);
	if(c_node == NULL)
		return 0;
	fnp = c_node->data.ptrvalue;
	
	if(fnp->label != NULL)
	{
	
		font = (FonT)GetMuskCParam(MSM_SEQUENCE, MSM_FLABEL, MSM_FONT);
			SelectFont(font);
	
		label_len = StringWidth(fnp->label);
		label_align = (Uint1)GetMuskCParam(MSM_TOPSTYLE, MSM_LABEL, MSM_STYLE);
		if(label_align != MSM_LABEL_RIGHT)
			label_align = MSM_LABEL_LEFT;
		c_val = GetMuskCParam(MSM_SEQUENCE, MSM_FLABEL, MSM_COLOR);
		convert_color(c_val, label_color);
		pseg = CreateSegment(pic, OBJ_BIOSEQ, 0);
		seg = CreateSegment(pseg, mpp->entityID, 0);
		AddAttribute (seg, COLOR_ATT, &label_color[0], 0, 0, 0, 0);

		AddLabelWithAlign(seg, label_align, mpp->left, mpp->left, mpp->seq_top, mpp->seq_bottom, fnp->label, font, fnp->itemID);
	}
	FreeFeatureList(c_node);
	return label_len;
}


static void set_lolli_pop_feature(Uint1Ptr l_feature)
{
	Int2 i;
	Int4 segstyle;

	for(i =0; i<FEATDEF_ANY; ++i)
	{
		segstyle = GetMuskCParam(i, MSM_SEGMENT, MSM_STYLE);
		switch (segstyle & MSM_SEG_FORM)
		{
			case MSM_SEG_SYM_RECT:
			case MSM_SEG_SYM_DIAMOND:
			case MSM_SEG_SYM_OVAL:
			case MSM_SEG_SYM_TRIANGLE:
				l_feature[i] = TRUE;
				break;
			default:
				l_feature[i]= FALSE;
		}
	}
}

static void load_feature_class(ValNodePtr PNTR type_list, Int2 _class)
{
	ValNodePtr curr;

	for(curr = *type_list; curr != NULL; curr = curr->next)
	{
		if(curr->data.intvalue == (Int4)_class)
			return;
	}
	ValNodeAddInt(type_list, 0, (Int4)_class);
}

/*******************************************************************
*
*	from the list of FeatNode, load the feature types contained 
*	for this seq-loc. It will be used to draw the legend
*
*******************************************************************/
static void load_feature_type (ValNodePtr fnp_list, ValNodePtr PNTR type_list)
{
	FeatNodePtr fnp;
	Int2 _class;

	if(type_list == NULL)
		return;
	while(fnp_list)
	{
		if(fnp_list->choice == OBJ_SEQFEAT)
		{
			fnp = fnp_list->data.ptrvalue;
			if(fnp != NULL)
			{
				_class = get_current_class(fnp);
				load_feature_class(type_list, _class);
			}
		}

		fnp_list = fnp_list->next;
	}
}
		
	

/*********************************************************************
*
*	from the list of AlignNode, extract all the alignments 
*	that were considered as the bins of the FISH maps
*	since the alignment of the FISH map is displayed at the 
*	top, those were extracted at the first
*
*********************************************************************/
static ValNodePtr extract_fish_align(ValNodePtr PNTR anp_list)
{
	ValNodePtr curr, prev, next;
	ValNodePtr fish_list;
	AnnotInfoPtr info;
	AlignNodePtr anp;
	Boolean is_fish;

	if(*anp_list == NULL)
		return NULL;
	fish_list = NULL;
	prev = NULL;
	curr = *anp_list;

	is_fish = FALSE;
	while(curr)
	{
		next = curr->next;
		if(curr->choice == OBJ_SEQANNOT)
		{
			info = (AnnotInfoPtr)(curr->data.ptrvalue);
			is_fish = info->is_fish_align;
		}
		else
		{
			anp = curr->data.ptrvalue;
			if(is_fish)
				anp->label = MemFree(anp->label);
		}
		if(is_fish)
		{
			if(prev == NULL)
				*anp_list = next;
			else
				prev->next = next;
			curr->next = NULL;
			if(curr->choice == OBJ_SEQANNOT)
				FreeAlignNode(curr);
			else
				ValNodeLink(&fish_list, curr);
		}
		else
			prev = curr;
		curr = next;
	}

	return fish_list;
}
				

static Int4 add_fish_align (Int4 top, Uint2 entityID, ValNodePtr fish_align, 
	Int4 scale, SegmenT pic)
{
	Int4 bottom;
	SegmenT pseg, seg;
	AlignNodePtr anp;
	Int4 middle;
	ValNodePtr curr;
	Int4 space = 4;
	Int4Ptr y_pos;
	Int2 num;
	Int4 line;

	num = get_vnp_num(fish_align);
	y_pos = MemNew((size_t)(2*num) * sizeof(Int4));
	for(curr = fish_align; curr != NULL; curr = curr->next)
	{
		anp = curr->data.ptrvalue;
		if(anp->extremes.right - anp->extremes.left >= 5* scale)
			anp->line = find_f_pos(anp->extremes.left, 
				anp->extremes.right, y_pos, scale, num);
	}
	MemFree(y_pos);

							/* 1 is the pen-width */
	pseg = CreateSegment(pic, OBJ_SEQALIGN, 0);
	seg = CreateSegment(pseg, entityID, 0);

	AddAttribute (seg, COLOR_ATT, BLACK_COLOR, NO_LINE_STYLE, NO_SHADING, 1, NO_MODE);
	top -= 8;
	bottom = top;
	while(fish_align)
	{
		anp = fish_align->data.ptrvalue;
		if(anp->extremes.right - anp->extremes.left >= 5* scale)
		{
			line = top - space * (anp->line +1);
			bottom = MIN(bottom, line);
			AddLine(seg, anp->extremes.left, line, anp->extremes.right, 
				line, FALSE, anp->itemID);
			middle = (anp->extremes.left + anp->extremes.right)/2;
			AddLine(seg, middle, line, middle, top, FALSE, anp->itemID);
			AddSymbol(seg, middle, top, UP_TRIANGLE_SYMBOL, TRUE, UPPER_CENTER, anp->itemID);
		}
		fish_align = fish_align->next;
	}
	
	return bottom-4;
}
	
static void extract_sts_search_node(ValNodePtr PNTR features, ValNodePtr PNTR g_node)
{
	ValNodePtr curr, prev, next;
	FeatNodePtr fnp;
	ValNodePtr g_prev;
	Boolean found;

	if(features == NULL || *features == NULL)
		return;
	g_prev = NULL;
	if(*g_node != NULL)
	{
		g_prev = *g_node;
		while(g_prev->next != NULL)
			g_prev = g_prev->next;
	}

	curr = *features;
	prev = NULL;
	while(curr)
	{
		next = curr->next;
		fnp = curr->data.ptrvalue;
		found = FALSE;
		if(fnp->annotDB[0] != '\0')
		{
			if(StringCmp(fnp->annotDB, "STS Search") == 0)
			{
				curr->next = NULL;
				if(g_prev == NULL)
					*g_node = curr;
				else
					g_prev->next = curr;
				g_prev = curr;

				if(prev == NULL)
					*features = next;
				else
					prev->next = next;
				found = TRUE;
			}
		}
		if(!found)
			prev = curr;
		curr = next;
	}
}
	

/**********************************************************************
*
*	DrawOneSeqLoc(slp, left, scale, pic, mlp, gdata, priority)
*	draw the current Seq-loc picture
*
*   slp:   the location on a Sequence, can be a list of Seq-loc for a 
*          circular molecule
*   left:  the leftmost position on the graph
*   scale: the scale for the current picture
*   pic:   the current picture
*   mlp:   to obtain the layout information
*   gdata: the selected gene symbols for display
*   priority: the priority of search for the current sequence
*   ftype_list: return a list of type of features for the current 
*   Seq-loc. This is going to be used for drawing the legend
*   image_list: the list to store the image mapping information
*   align_num: the top number of alignment for display
*
***********************************************************************/   		
static Boolean DrawOneSeqLoc(SeqLocPtr slp, Int4 left, Int4 scale, SegmenT pic, MapLayoutPtr PNTR mlp,
GeneDataPtr gdata, Uint2 priority, ValNodePtr PNTR ftype_list, ValNodePtr PNTR image_list,
Boolean forceSeglevelsTo1, ValNodePtr extraEntityList)
{
	ValNodePtr aligns = NULL, features = NULL;
	ValNodePtr g_node;
	Uint2 entityID;
	Boolean ends;
	CollectSeqOption cs_option;
	CollectAlignOption ca_option;
	SeqIdPtr sip;
	BioseqPtr bsp;
	Uint1 bsptype;
	Char buf[41];
	Int2 i;
	MapPosPtr mpp;
	Int4 top;
	Int4 topstyle;
	Boolean show_seq_label = TRUE;	/*show the label of a sequence*/
	Boolean is_raw_seq;	/*is it a raw sequence*/
	Boolean show_segment = TRUE;	/*draw each segments in a segmented sequence?*/
	Boolean is_aa;
	SeqIdPtr maybe_mapid;	/*a possible map-id*/
	Boolean is_map;

	Uint1 featureOrder[FEATDEF_ANY];
	Uint1 groupOrder[FEATDEF_ANY];
	Boolean lolli_feature[FEATDEF_ANY]; /*identify the lollipop features*/
	Int4 groupNum;
	Int4 top_ends;
	Int4 seq_label_len;
	SeqIntPtr sint;
	Boolean has_align;
	ValNodePtr fish_align;
	Int4 fish_top;
	ValNodePtr cyto_node;
	ValNodePtr vnp1, vnp2;
	DeltaSeqPtr dsp;
	SeqLocPtr loc;


	if(slp == NULL || pic == NULL || scale <=0)
		return FALSE;

	/*set up the option for collecting sequences and features*/		
	if(!set_seq_collect_option(&cs_option, scale))
		return FALSE;
	ends = cs_option.nointerval;	/*only show the two-ends of a feature*/

	/*fetch the current Bioseq*/
	sip = SeqLocId(slp);
	bsp = BioseqLockById(sip);
	if(bsp == NULL)
	{
		SeqIdWrite(sip, buf, PRINTID_TEXTID_ACCESSION, 40);
		Message(MSG_ERROR, "fail to get the sequence %s", buf);
		return FALSE;
	}
	/* adjust the interval */
	if(slp->choice == SEQLOC_INT)
	{
		sint = slp->data.ptrvalue;
		if(sint->from < 0)
			sint->from = 0;
		if(sint->to <0 || sint->to > bsp->length-1)
			sint->to = bsp->length-1;
	}
	entityID = ObjMgrGetEntityIDForPointer((Pointer)bsp);
	if(entityID == 0)
	{
		BioseqUnlock(bsp);
		Message(MSG_ERROR, "Fail to find the entityID");
		return FALSE;
	}
	if(bsp->repr == Seq_repr_seg)
		maybe_mapid = figure_map_seqid((SeqLocPtr)(bsp->seq_ext));
	else if (bsp->repr == Seq_repr_delta && bsp->seq_ext_type == 4)
	{
		dsp = (DeltaSeqPtr) bsp->seq_ext;
		loc = DeltaSeqsToSeqLocs (dsp);
		maybe_mapid = figure_map_seqid(loc);
		SeqLocFree (loc);
	}
	else
		maybe_mapid = NULL;

	if(bsp->repr == Seq_repr_seg || bsp->repr == Seq_repr_delta)
	{
		/* cs_option.seglevels = 0; */
		/* cs_option.seglevels = 1; */
		if (forceSeglevelsTo1) {
			cs_option.seglevels = 1;
		} else
			if(BioseqHasFeature(bsp))
		{ 
			if(maybe_mapid !=NULL)
				cs_option.seglevels = 0;
			if(!BioseqHasLandMark(bsp))
				cs_option.seglevels = 0;
			if(cs_option.seglevels == 1)
				cs_option.filter_level = 1;
		}
		else
			cs_option.seglevels = 1; 
	}
	else
		cs_option.seglevels = 0;


	
	/*collecting the features + sequence segments*/
	bsptype = get_Bioseq_type(bsp);
	cs_option.bsp_type = bsptype;
	is_aa = (bsp->mol == Seq_mol_aa);
	features  = CollectItemForSeqLocEx(slp, entityID, left, is_aa, maybe_mapid, &cs_option, gdata, priority, forceSeglevelsTo1);
	for (vnp1 = extraEntityList; vnp1 != NULL; vnp1 = vnp1->next) {
	  if ((Uint2) (vnp1->data.intvalue) != entityID) {
	    vnp2  = CollectItemForSeqLocEx(slp, (Uint2) (vnp1->data.intvalue), left, is_aa, maybe_mapid, &cs_option, gdata, priority, forceSeglevelsTo1);
	    ValNodeLink (&features, vnp2);
	  }
	}
	if(features == NULL)
	{
		return FALSE;
	}
	/*load the type of the features to draw the legend for the features*/
	load_feature_type (features, ftype_list);

	/*collecting the history*/		
	topstyle = GetMuskCParam(MSM_TOPSTYLE, 0, MSM_STYLE);
	has_align = FALSE;
	if(topstyle & MSM_HISTORY)
	{
		set_option_for_collect_align(&ca_option, cs_option.label_size, COLLECT_HISTORY);
		ca_option.only_history = FALSE;
		ca_option.map_insert = TRUE;
		ca_option.slabel_format = cs_option.slabel_format;
		ca_option.nointerval = cs_option.nointerval;
		ca_option.no_sort = FALSE;
		ca_option.flat_insert = FALSE;
		aligns = collect_anpnode_with_option(&ca_option, slp, entityID, COLLECT_HISTORY, OBJ_SEQALIGN, NULL, NULL, FALSE);
	    for (vnp1 = extraEntityList; vnp1 != NULL; vnp1 = vnp1->next) {
	      if ((Uint2) (vnp1->data.intvalue) != entityID) {
		    vnp2 = collect_anpnode_with_option(&ca_option, slp, (Uint2) (vnp1->data.intvalue), COLLECT_HISTORY, OBJ_SEQALIGN, NULL, NULL, FALSE);
	        ValNodeLink (&aligns, vnp2);
	      }
	    }
		has_align = alignode_has_alignments(aligns);
		if(has_align)
		{
			show_segment = FALSE;
			/*for the alignment of STSs in SegMap*/
			CollectSegMapSTSAlign(entityID, aligns, slp, left);
		}
	}
	else
		show_segment = FALSE;


	
	/*set the layout for the current Seq-loc*/
	get_next_ypos(*mlp, &top);
	mpp = MemNew(sizeof(MapPos));
	mpp->slp = slp;
	mpp->left = left;
	mpp->right = left + slp_list_len(slp);
	mpp->top  = top;
	mpp->bottom = top;
	mpp->seq_top = top;
	mpp->seq_bottom = top;
	mpp->entityID = entityID;
	

	/*display the alignment to the FISH map first */
	fish_top = 99999;
	if(get_FISH_align(bsp) != NULL && has_align)
	{	
		fish_align = extract_fish_align(&aligns);
		if(fish_align != NULL)
		{
			fish_top = top;
			mpp->top = add_fish_align (top, entityID, fish_align, scale, pic);
			mpp->bottom = mpp->top;
			mpp->seq_top = mpp->top;
			mpp->seq_bottom = mpp->top;
			mpp->bottom = mpp->top;
			FreeAlignNode(fish_align);
		}
	}

	/*map the feature order and group order*/
	for( i =0; i<FEATDEF_ANY; ++i)	/*for checking the features to load*/
	{
		featureOrder[i] = (Uint1)GetMuskCParam(i, MSM_FORDER, MSM_NUM);
		groupNum = (Uint1)GetMuskCParam(i, MSM_FGROUP, MSM_NUM);
		groupOrder[i] = (Uint1)GetMuskCParam(MSM_GROUPS, (Int2)groupNum, MSM_NUM);
	}



	g_node = NULL;
	if(topstyle & MSM_STACKGENE)	/*draw the features on top of the sequence*/
		g_node = extract_node_list(&features, OBJ_SEQFEAT, 0, 0, ALL_LABEL);
	else
	{

		if((bsptype==SEG_SEQ && maybe_mapid == NULL)|| bsptype == RAW_SEQ)
		{
			set_lolli_pop_feature(lolli_feature);
			g_node = extract_lollipop_feature(&features, 0, lolli_feature);
		}
	
		else
		{
			top_ends = GetMuskCParam(MSM_TOPSTYLE, MSM_ENDS, MSM_SCALE);
 			if(scale > top_ends)
 				g_node = extract_node_list(&features, OBJ_SEQFEAT, 0, 0, ALL_LABEL);
 			else
 				g_node = extract_lollipop_feature(&features, scale*10, NULL);

			/*g_node = extract_lollipop_feature(&features, scale, NULL);*/
		}
	}
			
	extract_sts_search_node(&features, &g_node);
	if(g_node)
		if(DrawFeatures(g_node, mpp, pic, TRUE, featureOrder, groupOrder, scale, gdata, image_list))
			show_seq_label = FALSE;
	mpp->seq_top = mpp->bottom;
	mpp->seq_bottom = mpp->bottom;
	is_map = TRUE;	/*map does not extract bioseqs*/
	switch(bsptype)
	{
	   case GENETIC_MAP:
		DrawGeneticMap(&features, mpp, pic, scale, gdata, image_list);
		break;

	   case PHYSICAL_MAP:
		DrawPhysicalMap(&features, mpp, pic, scale, image_list);
		break;

	   case RESTRICTION_MAP:
		DrawRestrictionMap(&features, mpp, pic, FALSE, SeqLocStrand(slp), scale);
		break;

	   case CYTO_MAP:
		DrawCytoMap(&features, mpp, scale, pic);
		break;

	   default:
		is_raw_seq = (bsptype == RAW_SEQ);
		is_map = FALSE;
		DrawSeqMap(&features, mpp, pic, scale, is_raw_seq, show_segment, image_list);
		break;
	}
	
	if(is_map && bsptype != RESTRICTION_MAP)  /*need to extract from the Bioseq*/
		seq_label_len = add_seq_label_for_map(&features, mpp, pic);
	else
		seq_label_len = 0;
	
	/*add the label of the sequence*/

	
	DrawHistory(aligns, mpp, seq_label_len, pic, scale, image_list);

	if(bsptype == CYTO_MAP && gdata != NULL)
	{
		cyto_node = convert_gdata_to_featnode(gdata, mpp->slp, mpp->left);
		if(cyto_node != NULL)
			ValNodeLink(&features, cyto_node);
	}
		

	g_node = extract_node_list(&features, OBJ_SEQFEAT, 0, 0, ALL_LABEL);
	DrawFeatures(g_node, mpp, pic, FALSE, featureOrder, groupOrder, scale, gdata, image_list);
	FreeFeatureList(features);


	if(fish_top != 99999)
	{	/*there is a FISH mapping data */
		mpp->seq_bottom = fish_top - (mpp->seq_top - mpp->seq_bottom);
		mpp->seq_top = fish_top;
	}
	ValNodeAddPointer(mlp, 0, mpp);
	BioseqUnlock(bsp);
	return TRUE;

}

	
/*
*	MakeCompressAlignList: Build the AlignNode for  a compact alignment 
*   view associated 
*	with a sequence
*	query_bsp is the query sequence
*	annot: the alignment data. If NULL, the alignment data will be extracted 
*			from sequence
*	return the list of AlignNode
*/
ValNodePtr MakeCompressAlignList(BioseqPtr query_bsp, SeqAnnotPtr annot, Uint2Ptr p_entityID)
{
	SeqAnnotPtr sap;
	SeqLocPtr slp;
	CollectAlignOption ca_option;
	CollectSeqOption cs_option;
	Uint2 entityID, t_entityID;

	ValNodePtr aligns = NULL, h_aligns, t_aligns;


	if(query_bsp == NULL)
		return NULL;

	entityID = ObjMgrGetEntityIDForPointer((Pointer)query_bsp);
	if(entityID == 0)
	{
		Message(MSG_ERROR, "Fail to find the entityID for the Bioseq");
		return NULL;
	}

	if(!set_seq_collect_option(&cs_option, 200))
		return NULL;


	slp = SeqLocIntNew(0, query_bsp->length-1, 0, query_bsp->id);
	set_option_for_collect_align(&ca_option, 0, COLLECT_HISTORY);
	ca_option.only_history = FALSE;
	ca_option.map_insert = TRUE;
	ca_option.slabel_format = cs_option.slabel_format;
	ca_option.nointerval = TRUE;
	ca_option.no_sort = FALSE;
	ca_option.flat_insert = FALSE;
	ca_option.align_num = -1;
	ca_option.csop = &cs_option;
	aligns = collect_anpnode_with_option(&ca_option, slp, entityID, COLLECT_HISTORY, OBJ_SEQALIGN, NULL, NULL, FALSE);
	if(!alignode_has_alignments(aligns))
	{
		FreeAlignNode(aligns);
		aligns = NULL;
	}

	/*no alignment from the Bioseq, trying the Seq-annot*/
	if(aligns == NULL)
	{
		for(sap = annot; sap != NULL; sap = sap->next)
		{
			if(sap->type == 2)
			{
				t_entityID = ObjMgrRegister(OBJ_SEQANNOT, (Pointer)sap);
				if(t_entityID == 0)
				{
					Message(MSG_ERROR, "Fail to register the Seq-annot");
				}
				else
				{
					t_aligns = collect_anpnode_with_option(&ca_option, slp, t_entityID, COLLECT_HISTORY, OBJ_SEQALIGN, NULL, NULL, TRUE);
					ValNodeLink(&aligns, t_aligns);
				}
				*p_entityID = t_entityID;
				break;
			}
		}
	}
	else
		*p_entityID = entityID;
	if(aligns == NULL)
	{
		ErrPostEx(SEV_WARNING, 0, 0, "No alignment data");
		SeqLocFree(slp);
		return NULL;
	}

	/*collecting the alignment for the query sequence*/
	cs_option.features[FEATDEF_repeat_region] = TRUE;
	cs_option.features[FEATDEF_repeat_unit] = TRUE;
	ca_option.show_feature = TRUE;
	ca_option.nointerval = FALSE;
	ca_option.label_size = cs_option.label_size;
	h_aligns = collect_master_align_node(&ca_option, slp, OBJ_SEQALIGN, entityID);
	SeqLocFree(slp);

	ValNodeLink(&h_aligns, aligns);

	return h_aligns;
}


SegmenT DrawCompressAlignment(ValNodePtr anp_list, SeqLocPtr m_loc, Int4 width, Int4Ptr p_scale, Int4Ptr p_maxwidth, Uint2 entityID, ValNodePtr PNTR arect_list)
{
	Int2 styleNum;
	CharPtr old_style;
	Int4 scale;
	SegmenT pic;
	ValNode vn;
	Int4 max_width;
	Int4 y_pos;
	

	if(anp_list == NULL)
		return NULL;
	if(arect_list != NULL)
		*arect_list = NULL;
	vn.data.ptrvalue = m_loc;
	vn.next = NULL;
	max_width = CountMaxSeqLabel(&vn);
	scale = FigureMaxScale(&vn, (Int2)width, max_width);
	if(scale <=0)
		return NULL;
	pic = CreatePicture();

	/*set up the sepcial style */
	styleNum = GetMuskCurrentSt();
	old_style = GetMuskStyleName(styleNum);
	SetMuskCurrentSt("StyleX");

	y_pos = 0;
	DrawMPAlignment(anp_list, 0, SeqLocLen(m_loc)-1, m_loc, entityID, scale, &y_pos, MSM_SEQHIST, TRUE, pic);
	if(arect_list != NULL)
    		load_align_label_rectangle(anp_list, arect_list, scale, FALSE);

	*p_scale = scale;
	*p_maxwidth = max_width;
	SetMuskCurrentSt(old_style);
	return pic;
}


/*************************************************************************
*
*	Label_GData(): Label the gene_data node specified by the user
*
*************************************************************************/
Boolean Label_GData(GeneDataPtr gdata, SeqLocPtr slp, Int4 left, Int4 top, SegmenT seg)
{
	SeqLocPtr c_slp;
	Int4 c_left;
	Boolean success = FALSE;
	Int4 m_start, m_stop, t_start, t_stop;
	Int4 g_pos;
	

	if(slp == NULL ||gdata == NULL)
		return FALSE;
	while(gdata)
	{
		t_start = SeqLocStart(gdata->location);
		t_stop = SeqLocStop(gdata->location);
		for(c_slp = slp, c_left = left; slp!=NULL; c_slp=c_slp->next)
		{
			m_start = SeqLocStart(c_slp);
			m_stop = SeqLocStop(c_slp);
			
			if(!(t_start > m_stop || t_stop < m_start))
			{
				g_pos = MAX(0, (t_start - m_start)) + c_left;
				AddSymbol(seg, g_pos, top, DIAMOND_SYMBOL, TRUE, LOWER_CENTER, 0);
				success = TRUE;
			}
			c_left += SeqLocLen(c_slp);
		}
		gdata = gdata->next;
	}
	
	return TRUE;
}
	






/*************************************************************************
*
*	DrawVerticalAlign (align, pic, mlp)
*	draw alignment among the sequences. (connecting aligned seg by line)
*	align: the Seq-align which contains the alignment among sequence
*	It is assumed to be a Std-seg
*	pic: the drawing segment
*	mlp: the list of the layout of the sequences
*
*************************************************************************/

void draw_one_align(AlignPos ap, Int2 num, Int2 order, SegmenT seg)
{
	Int4 x1, x2;
	Int2 i;
	
	for(i = 0; i<num-1; ++i)
	{
		x1 = (ABS(ap.left[i]) + ABS(ap.right[i]))/2;
		x2 = (ABS(ap.left[i+1]) + ABS(ap.right[i+1]))/2;
		AddLine(seg, x1, ap.top[i], x2, ap.bottom[i+1], FALSE, order);
		if(ap.left[i] < 0)
		{
			AddAttribute (seg, WIDTH_ATT | COLOR_ATT, BLUE_COLOR, NO_LINE_STYLE, NO_SHADING, 1, COPY_MODE);
			AddSymbol(seg, x1, ap.bottom[i], UP_TRIANGLE_SYMBOL, TRUE, MIDDLE_CENTER, 0);
			AddAttribute (seg, WIDTH_ATT | COLOR_ATT, BLACK_COLOR, NO_LINE_STYLE, NO_SHADING, 1, COPY_MODE);
		}
		if(ap.left[i+1] < 0)
		{
			AddAttribute (seg, WIDTH_ATT | COLOR_ATT, BLUE_COLOR, NO_LINE_STYLE, NO_SHADING, 1, COPY_MODE);
			AddSymbol(seg, x2, ap.bottom[i], DOWN_TRIANGLE_SYMBOL, TRUE, MIDDLE_CENTER, 0);
			AddAttribute (seg, WIDTH_ATT | COLOR_ATT, BLACK_COLOR, NO_LINE_STYLE, NO_SHADING, 1, COPY_MODE);
		}
	}
}

static MapPosPtr get_map_pos(MapLayoutPtr mlp, SeqIdPtr sip)
{
	MapPosPtr mpp;

	while(mlp)
	{
		mpp = mlp->data.ptrvalue;
		if(SeqIdMatch(sip, SeqLocId(mpp->slp)))
			return mpp;
		mlp = mlp->next;
	}
	return NULL;
}

static AlignRangePtr get_ith_arp(AlignRangePtr arp, Int2 i)
{
	Int2 k = 0;

	while(arp)
	{
		if(k == i)
			return arp;
		arp = arp->next;
		++k;
	}
	return NULL;
}

Boolean DrawVerticalAlign (SeqAlignPtr align, SegmenT pic, MapLayoutPtr mlp)
{
	StdSegPtr ssp;
	SeqLocPtr locs;
	MapLayoutPtr curr;
	MapPosPtr mpp;
	Int2 j;
	GatherRange gr;
	AlignPos ap;
	SeqLoc sl_1;

		

	sl_1.choice = SEQLOC_MIX;
	sl_1.next = NULL;
	j = 0;
	if(align->segtype != 3)
		return FALSE;
	ssp= align->segs;
	for(curr = mlp; curr != NULL; curr = curr->next)
	{
		mpp = (MapPosPtr)(curr->data.ptrvalue);
		sl_1.data.ptrvalue = mpp->slp;
		for(locs = ssp->loc; locs != NULL; locs = locs->next)
		{
			if(SeqLocOffset(&sl_1, locs, &gr, mpp->left))
			{
		
				ap.left[j] = gr.left;
				ap.right[j] = gr.right;
				ap.top[j] = mpp->seq_top;
				ap.bottom[j] = mpp->seq_bottom;
				++j;
			}
		}
	}

	if(j >1)
	{
		SortAlignPosition(&ap, j);
		draw_one_align(ap, j, 0, pic);
		return TRUE;
	}
	else
		return FALSE;
}




static void DrawEquivAlign (ValNodePtr e_align_list, SegmenT pic, MapLayoutPtr mlp)
{
	SeqAlignPtr align;
	SeqAnnotPtr annot;
	Int2 type;
	Uint1 my_gray_color[]={63, 63, 63};

	/* AddAttribute (pic, WIDTH_ATT | COLOR_ATT, GREEN_COLOR, NO_LINE_STYLE, NO_SHADING, 1, NO_MODE); */
	while(e_align_list)
	{
		annot = (SeqAnnotPtr)(e_align_list->data.ptrvalue);
		type = GetEquivAlignType (annot);
		add_attribute_pen(pic, MSM_ALIGNMENT, MSM_ALINE);
		switch(type)
		{
			case 2:  /*it is inconsistent markers*/
				AddAttribute(pic, COLOR_ATT|STYLE_ATT, RED_COLOR, DOTTED_LINE, 0, 0, 0);
				break;
			case 3:	/*it is unknown type*/
			case 0:
				AddAttribute(pic, COLOR_ATT|STYLE_ATT, CYAN_COLOR, SOLID_LINE, 0, 0, 0);
				break;
			case 4:  /*it is the alignment to the FISH map*/
				AddAttribute(pic, COLOR_ATT|STYLE_ATT, BLACK_COLOR, SOLID_LINE, 0, 0, 0);
				break;
			default:
				AddAttribute(pic, STYLE_ATT, 0, SOLID_LINE, 0, 0, 0);
				break;
		}
		align = (SeqAlignPtr)(annot->data);
		while(align)
		{
			DrawVerticalAlign (align, pic, mlp);
			align = align->next;
		}
		e_align_list = e_align_list->next;
	}
}
		






/*********************************************************************
*
*	DrawGenomeMap(slp_list, e_align_list, scale, mlp)
*
*	draw all the SeqLocs in the list and show their alignment (if any)
*	slp_list: the list of Seq-locs
*	e_align_list: the list of alignment among the Seq-locs
*	scale: the scale for the picture
*	mlp: store the layout of the current picture
*	return the picture of the genome map
*	ftype_list: return a list of type of features for the current
*	Seq-loc. This is going to be used for drawing the legend
*	image_list: the list to store the image mapping information
*	align_num: number of the top alignment to show. -1 = show everything
*
*********************************************************************/
SegmenT DrawGenomeMapEx (ValNodePtr slp_list, ValNodePtr e_align_list, Int4 scale, GeneDataPtr gdata,
MapLayoutPtr PNTR mlp, ValNodePtr PNTR ftype_list, ValNodePtr PNTR image_list,
Boolean forceSeglevelsTo1, ValNodePtr extraEntityList)
{
	Boolean success = FALSE;
	ValNodePtr curr;
	SeqLocPtr slp;
	MapLayoutPtr mlp_head= NULL;
	SegmenT pic;
 	/* ValNodePtr t_image = NULL;  */
	
	if(slp_list == NULL || scale <=0)
		return NULL;
	
	scale = MIN(scale, MAX_SCALE);
	pic = CreatePicture();
	if(ftype_list != NULL)
		*ftype_list = NULL;
	if(image_list != NULL)
		*image_list = NULL;
	
	for(curr = slp_list; curr !=NULL; curr = curr->next)
	{
		slp = curr->data.ptrvalue;
		/* if(DrawOneSeqLoc(slp, 0, scale, pic, &mlp_head, gdata, (Uint2)(curr->choice), ftype_list, &t_image)) */
	 	if(DrawOneSeqLoc(slp, 0, scale, pic, &mlp_head, gdata, (Uint2)(curr->choice), ftype_list,
	 	                 image_list, forceSeglevelsTo1, extraEntityList)) 
			success = TRUE;
	}
	/* ValNodeFreeData(t_image);  */
	if(!success)
	{
		DeletePicture(pic);
		return NULL;
	}

	DrawEquivAlign (e_align_list, pic, mlp_head);
	if(mlp !=NULL)
		*mlp = mlp_head;
	else
		MapLayoutFree(mlp_head);
	return pic;
}

SegmenT DrawGenomeMap (ValNodePtr slp_list, ValNodePtr e_align_list, Int4 scale, GeneDataPtr gdata, MapLayoutPtr PNTR mlp, ValNodePtr PNTR ftype_list, ValNodePtr PNTR image_list)
{
	return DrawGenomeMapEx (slp_list, e_align_list, scale, gdata, mlp, ftype_list, image_list, FALSE, NULL);
}

/**************************************************************************
*
*	DrawSequinMap(slp_list, sep, scale)
*	return a picture with the interface defined by Jonathan K. 
*	slp_list: a ValNode list of Seq-loc
*	sep: the Seq-entry pointer
*	scale: the scale of the picture
*
**************************************************************************/
SegmenT DrawSequinMapEx (ValNodePtr slp_list, SeqEntryPtr sep, Int4 scale, GeneDataPtr gdata, ValNodePtr PNTR ftype_list, Boolean forceSeglevelsTo1, ValNodePtr extraEntityList)
{
	ValNodePtr e_align_list = NULL;
	SegmenT pic;
	
	if(slp_list == NULL || sep == NULL || scale <=0)
		return NULL;
		
	e_align_list = get_equiv_align(sep);
	/*gdata = make_gene_data(g_list);*/
	
	pic = DrawGenomeMapEx(slp_list, e_align_list, scale, gdata, NULL, ftype_list, NULL, forceSeglevelsTo1, extraEntityList);
	/*GeneDataFree(gdata);*/
	ValNodeFree(e_align_list);
	return pic;
}

SegmenT DrawSequinMap (ValNodePtr slp_list, SeqEntryPtr sep, Int4 scale, GeneDataPtr gdata, ValNodePtr PNTR ftype_list)
{
	return DrawSequinMapEx (slp_list, sep, scale, gdata, ftype_list, FALSE, NULL);
}

Boolean load_align_option_for_graphic(CollectAlignOptionPtr caop, CollectSeqOptionPtr csop, Int4 style, Boolean flat_insert)
{
	Int2 i;

	if(style < MSM_SEQHIST || style > MSM_MDIM)
		return FALSE;
	MemSet((Pointer)caop, 0, sizeof(CollectAlignOption));
	caop->graphic_offset = 0;
	caop->only_history = FALSE;
	caop->show_mismatch = (style == MSM_MPAIR);	/*can not show mismatch for MDIM for now*/
	caop->label_size= (Int2)GetMuskCParam(MSM_TOPSTYLE, MSM_LABEL, MSM_NUM);
	caop->nointerval = FALSE;
	caop->map_insert = (style== MSM_MPAIR || style == MSM_SEQHIST);
	caop->flat_insert = (style == MSM_MPAIR && flat_insert);
	caop->map_graphic = TRUE;
	caop->show_feature = TRUE;
	caop->slabel_format = (Uint1)GetMuskCParam(MSM_SEQUENCE, MSM_SLABEL, MSM_STYLE);
	caop->align_num = DEFAULT_ALIGN_NUM; 
	/* caop->align_num = (Int4)GetMuskCParam(MSM_ALIGNMENT, MSM_NOSUBCLASS, MSM_SCALE); */

	csop->seglevels = 0;
	csop->nointerval = FALSE;
	csop->slabel_format = (Uint1)GetMuskCParam(MSM_SEQUENCE, MSM_SLABEL, MSM_STYLE);
	for(i =0; i<FEATDEF_ANY; ++i)
		csop->flabel_format[i] = (Uint1)GetMuskCParam(i, MSM_FLABEL, MSM_STYLE);
	csop->label_size = caop->label_size;
	csop->filter_level = 0;
	return TRUE;
}



/************************************************************
*
* collect_alignnode_from_alp(m_loc)
* collect all the alignment stored as the history of the 
* master sequence
* m_loc: the selected location of the master sequence
* return a list of AlignNode
*
************************************************************/
ValNodePtr collect_alignnode_from_slp(SeqLocPtr m_loc, Uint2Ptr t_entityID, Boolean flat_insert)
{
	ValNodePtr anp_node;
	CollectAlignOption ca_option;
	CollectSeqOption cs_option;
	BioseqPtr bsp;
	Uint2 entityID;

	if(m_loc == NULL)
		return NULL;
	bsp = BioseqLockById(SeqLocId(m_loc));
	if(bsp == NULL)
		return NULL;
	entityID = ObjMgrGetEntityIDForPointer((Pointer)bsp);
	if(entityID == 0)
		return NULL;
	if(t_entityID != NULL)
		*t_entityID = entityID;
	load_align_option_for_graphic(&ca_option, &cs_option, MSM_MPAIR, flat_insert);
	ca_option.csop = &cs_option;
	if(!BioseqHasFeature(bsp))
		cs_option.seglevels = 1;
	anp_node= collect_anpnode_with_option(&ca_option, m_loc, entityID, COLLECT_MP, OBJ_SEQHIST_ALIGN, NULL, NULL, FALSE);

	BioseqUnlock(bsp);
	return anp_node;
}




/************************************************************************
*
*
************************************************************************/
SegmenT DrawSeqHistoryAlignment(SeqLocPtr m_loc, Int4 scale, ValNodePtr PNTR anp_list, Uint2Ptr seq_entityID, Boolean flat_insert)
{
	ValNodePtr anp_node;
	SegmenT pic;
	Int4 y_pos = 0;
	Int4 len;
	Uint2 entityID;

	anp_node = collect_alignnode_from_slp(m_loc, &entityID, flat_insert);
	if(anp_node == NULL)
		return NULL;
	pic = CreatePicture();
	len = slp_list_len(m_loc);
	DrawMPAlignment(anp_node, 0, len-1, m_loc, entityID, scale, &y_pos, MSM_MPAIR, FALSE, pic);

	if(seq_entityID != NULL)
		*seq_entityID = entityID;
	if(anp_list != NULL)
		*anp_list = anp_node;
	else
		FreeAlignNode(anp_node);
	return pic;
}

/*draw something that looks like a folder for the un-aligned contigs in 
  Eric Green's map
*/

static Boolean draw_unknown_folder(SegmenT seg, Int4 left, Int4 right, Int4 y_pos, AnnotInfoPtr annot_info)
{
	Char label[101];
	Int4 center, middle;
	Int4 font_height;

	if(annot_info->annotDB[0] == '\0')
		return FALSE;
	font_height = FontHeight();
	middle = y_pos - font_height/2;
	SelectFont(programFont);
	sprintf(label, "[%s]", annot_info->annotDB);
	center = (left + right)/2;

	AddAttribute (seg, COLOR_ATT, YELLOW_COLOR, NO_LINE_STYLE, NO_SHADING, 1, NO_MODE);
	AddRectangle (seg, left, y_pos, right, y_pos-font_height, FALSE, TRUE, annot_info->itemID);
	AddAttribute (seg, COLOR_ATT, BLACK_COLOR, NO_LINE_STYLE, NO_SHADING, 1, NO_MODE);
	AddTextLabel(seg, center, middle, label, programFont, 0, MIDDLE_CENTER, annot_info->itemID);
	return TRUE;
}

	
static Boolean get_master_alignnode_extremes (ValNodePtr anp_node, Int4Ptr m_left, Int4Ptr m_right)
{
	AlignNodePtr anp;

	while(anp_node != NULL)
	{
		if(anp_node->choice != OBJ_SEQANNOT)
		{
			anp = anp_node->data.ptrvalue;
			if(anp->is_master)
			{
				*m_left = anp->extremes.left;
				*m_right = anp->extremes.right;
				return TRUE;
			}
		}

		anp_node = anp_node->next;
	}

	return FALSE;
}


Boolean DrawMPAlignment(ValNodePtr anp_node, Int4 left, Int4 right, SeqLocPtr m_loc, Uint2 entityID, Int4 scale, Int4Ptr cur_pos, Uint1 style, Boolean compress, SegmenT pic)
{
	Int4 y_pos;
	SegmenT pseg, seg;
	Boolean ends = FALSE;
	Int4 height, ends_scale;

	Int4 g_top, g_bottom;
	Int4 group_font_height;

	ValNodePtr list, curr, prev, last, next;
	ValNodePtr head;
	AnnotInfoPtr annot_info;
	Int4 space;
	Int4 m_left = -1, m_right = -1;
	Int4 offset;
	SeqLocPtr last_loc;
	SeqIntPtr sint;
	Int4 len;
	Boolean use_symbol_for_align;	/*for each alignment, only use 
					the diamond symbol to indicate 
					the alignment location*/


	if(anp_node == NULL || pic == NULL || cur_pos == NULL)
		return FALSE;
	if(style > MSM_FIXED || style < MSM_SEQHIST)
		return FALSE;
	if(style == MSM_SEQHIST )
	{
		ends_scale = GetMuskCParam(MSM_TOPSTYLE, MSM_ENDS, MSM_SCALE);
		if(ends_scale < scale)
			ends = TRUE;
	}
	if(compress)
		ends = TRUE;
	if(ends)
		height = GetMuskCParam(MSM_SEQUENCE, MSM_SEG_BORD, MSM_PENWIDTH);
	else
		height = GetMuskCParam(MSM_SEQUENCE, MSM_SEGMENT, MSM_HEIGHT);

	y_pos = *cur_pos;
	if(m_loc != NULL)
		y_pos -= 40;	/*leaves enough space for drawing the ruler*/
	
	pseg = CreateSegment(pic, anp_node->choice, 0);
	seg = CreateSegment(pseg, entityID, 0);

	if(!get_master_alignnode_extremes(anp_node, &m_left, &m_right))
	{
		m_left = left;
		m_right = right;
	}
	if(m_loc != NULL)
	{
		len = SeqLocLen(m_loc);
		last_loc = m_loc;
		offset = 0;
		sint = NULL;
		while(last_loc->next != NULL)
		{
			len += SeqLocLen(last_loc->next);
			last_loc = last_loc->next;
		}
		if(m_left != -1 && m_right != -1 && last_loc->choice == SEQLOC_INT)
		{
			offset = m_right - m_left + 1 - len;
			if(offset > 0)
			{
				sint = last_loc->data.ptrvalue;
				if(sint->strand == Seq_strand_minus)
					sint->from -= offset;
				else
					sint->to += offset;
			}
		}
		DrawSeqScale(seg, m_loc, 0, 0, scale, TRUE);
		if(offset > 0 && sint != NULL)
		{
			if(sint->strand == Seq_strand_minus)
				sint->from += offset;
			else
				sint->to -= offset;
		}
	}

	SelectFont(programFont);
	group_font_height = FontHeight();
	space = GetMuskCParam(MSM_TOPSTYLE, MSM_SPACE, MSM_HEIGHT);


	if(anp_node == NULL)
		return FALSE;
	list = anp_node;
	prev = NULL;
	head = NULL;
	use_symbol_for_align = FALSE;
	while(list != NULL)
	{
		annot_info = NULL;
		/* if(prev != NULL)
			prev->next = list; */
		while(list && list->choice == OBJ_SEQANNOT)
		{
			annot_info = (AnnotInfoPtr)(list->data.ptrvalue);
			if(annot_info != NULL && annot_info->consistent == ALIGN_UNKNOWN)
			{	
				/*it is the folder like un-aligned thing*/
				pseg = CreateSegment(pic, OBJ_SEQANNOT, 0);
				seg = CreateSegment(pseg, annot_info->entityID, 0);
				if(draw_unknown_folder(seg, m_left, m_right, y_pos, annot_info))
				{
					y_pos -= (group_font_height + space);
					*cur_pos = y_pos;
				}
			}
			if(head == NULL)
				head = list;
			prev = list;
			list = list->next;
		}
		if(list != NULL)
		{
			use_symbol_for_align = FALSE;
			curr = list;
			last = NULL;
			while(curr && curr->choice != OBJ_SEQANNOT)
			{
				last = curr;
				curr = curr->next;
			}
			next = last->next;
			last->next = NULL;
			g_top = y_pos;
			if(annot_info != NULL)	/*there is a Seq-annot wrapper*/
			{
				y_pos  -= group_font_height;
				g_top -= group_font_height/2;
				if(annot_info->annotDB[0] != '\0')
				{
					if(StringCmp(annot_info->annotDB, "UniGene") == 0)
						use_symbol_for_align = TRUE;
				}
			}
			if(use_symbol_for_align)
				*cur_pos = LayoutAlignFlat(list, scale, y_pos);
			else
				*cur_pos = LayoutAlignNode(list, y_pos, scale, height);
			g_bottom = *cur_pos; 
			y_pos = *cur_pos;

			pseg = CreateSegment(pic, list->choice, 0);
			seg = CreateSegment(pseg, entityID, 0);
			if(use_symbol_for_align)
				DrawFlatAlign(seg, list);
			else
				DrawAlignNode(list, scale, seg);

			if(annot_info != NULL)
			{
				pseg = CreateSegment(pic, OBJ_SEQANNOT, 0);
				seg = CreateSegment(pseg, annot_info->entityID, 0);
				AddAnnotGroup(m_left, g_top, m_right, g_bottom, (Int2)(annot_info->displayOrder), annot_info->annotDB, annot_info->itemID, seg); 
				y_pos -= group_font_height;
			}
			/* last->next = next; */
			if(prev != NULL)
				prev->next = list;
			last = list;
			while(last->next != NULL)
				last = last->next;
			last->next = next;
			prev = last;
			list = next;
		}
	}
	return TRUE;
}
	
	
	



/************************************************************************
*
*	DrawSeqScale(seg, slp, left, ypos, scaleX)
*	draw a scale for the Seq-loc. It can be a list of Seq-loc
*	seg: the drawing segment
*	slp: the Seq-loc
*	left: the left offset
*	ypos: the top position
*	scaleX: the current scale of the picture
*
************************************************************************/
void DrawSeqScale(SegmenT seg, SeqLocPtr slp, Int4 left, Int4 ypos, Int4 scaleX, Boolean add_x_line)
{

  Int4 ruler;
  Int4 r_start, r_stop, curpos;
  Int4 xpos;
  Char str[20];
  Int2 i;
  NumberingPtr np;
  Boolean use_kb;
  Int2 label_width = 0;
  Int2 c_width;
  Int4 pre_pos = -1;
  BioseqPtr bsp;



	 ruler = calculate_ruler(scaleX);
	 AddAttribute (seg, WIDTH_ATT | COLOR_ATT, BLACK_COLOR, NO_LINE_STYLE, NO_SHADING, 1, NO_MODE);
	ypos -=2;
  	bsp = BioseqFind(SeqLocId(slp));
	if(bsp == NULL)
		np = NULL;
	else
		np = getBioseqNumbering(bsp);
	use_kb = (scaleX >= 1000);
	while(slp)
	{
		r_start = SeqLocStart(slp);
		r_stop = SeqLocStop(slp);
		if(add_x_line)
			AddLine(seg, left, ypos, left+SeqLocLen(slp), ypos, FALSE, 0);
		for(curpos = r_start; curpos <=r_stop; curpos +=ruler)
		{
			map_unit_label(curpos, np, str, use_kb);
			xpos = left + curpos - r_start;
			AddLine(seg, xpos, ypos, xpos, ypos-8, FALSE, 0);
			c_width = StringWidth(str);
			if(start_new_stack(pre_pos, xpos, scaleX, &label_width, c_width))
			{
				pre_pos = xpos;
				AddLabel(seg, xpos, ypos-4, str, SMALL_TEXT, 5, LOWER_CENTER, 0);
			}
			for(i =1; i<10; ++i)
			{
				if(curpos + ruler/10*i <=r_stop)
				{
					xpos = left + curpos + ruler/10*i - r_start;
					if(i ==5)
						AddLine(seg, xpos, ypos, xpos, ypos-5, FALSE, 0);
					else
						AddLine(seg, xpos, ypos, xpos, ypos-3, FALSE, 0);
				}
			}
		}
		left += SeqLocLen(slp);
		slp = slp->next;
	}
}
	
			

Int4 CountMaxSeqLabel(ValNodePtr slp_list)
{
	FonT font;
	SeqLocPtr slp;
	BioseqPtr bsp;
	Uint1 format;
	Char label[100];
	Int4 max_width = 0;


	font = (FonT)GetMuskCParam(MSM_SEQUENCE, MSM_SLABEL, MSM_FONT);
	format = (Uint1)GetMuskCParam(MSM_SEQUENCE, MSM_SLABEL, MSM_STYLE);

	SelectFont(font);

	while(slp_list)
	{
		slp = slp_list->data.ptrvalue;
		if(slp != NULL)
		{
			bsp = BioseqFind(SeqLocId(slp));
			if(bsp != NULL)
				MuskSeqIdWrite(bsp->id, label, 99, format, TRUE, FALSE);
			else
				MuskSeqIdWrite(SeqLocId(slp), label, 99, format, FALSE, FALSE);
			max_width = MAX(StringWidth(label), max_width);
		}
		slp_list = slp_list->next;
	}
	return max_width;
}
			

Int4 FigureMaxScale(ValNodePtr slp_list, Int2 view_width, Int4 max_label)
{
	Int4 width;
	Int4 scale;
	
	Int4 seq_len;
	SeqLocPtr slp;
	ValNodePtr curr;
	Int4 len;
	
	seq_len = 0;
	for(curr = slp_list; curr !=NULL; curr = curr->next)
	{
		slp = (SeqLocPtr)(curr->data.ptrvalue);
		len = slp_list_len(slp);
		seq_len = MAX(len, seq_len);
	}
	if(seq_len == 0)
		return -1;
	width = (Int4)view_width- max_label;
	if(width <=0)
		width = view_width/2;
	scale = seq_len/width + (((Int4)width%(Int4)view_width)>0);
	return MIN(scale, MAX_SCALE); 
	/* return scale; */
}

Int4 FigureMinScale(ValNodePtr slp_list, Int4 max_label)
{
	Int4 seq_len;
	SeqLocPtr slp;
	ValNodePtr curr;
	
	seq_len = 0;
	for(curr = slp_list; curr !=NULL; curr = curr->next)
	{
		slp = (SeqLocPtr)(slp_list->data.ptrvalue);
		seq_len = MAX(slp_list_len(slp), seq_len);
	}
	if(seq_len == 0)
		return 1;
	return MAX(1, (seq_len + max_label)/320000L);
}


static Uint1 find_hot_link_gi(SeqIdPtr h_sip, Int4Ptr gi)
{
	DbtagPtr db_tag;
	ObjectIdPtr oip;
	SeqIdPtr sip;

	*gi = 0;
	sip = h_sip;
	while(sip)
	{
		if(sip->choice == SEQID_GI)
		{
			*gi = sip->data.intvalue;
			return ENTREZ_DB;
		}
		else if(sip->choice == SEQID_GENERAL)
		{
			db_tag = sip->data.ptrvalue;
			if(db_tag && db_tag->db && StringCmp(db_tag->db, "THC") == 0)
			{
				oip = db_tag->tag;
				if(oip && oip->id != 0)
				{
					*gi = oip->id;
					return THC_DB;
				}
			}
		}

		sip = sip->next;
	}

	for(sip = h_sip; sip != NULL; sip = sip->next)
	{
		if(sip->choice == SEQID_GENERAL)
		{
			*gi = GetUniGeneIDForSeqId (sip);
			if(*gi != 0)
				return ENTREZ_KLUDGE_ID;
		}
	}


	return 0;
}

static void link_new_align_label(ValNodePtr PNTR head, ValNodePtr PNTR prev, Uint1 choice, AlignLabelRectPtr alrp)
{
	ValNodePtr curr;

	curr = ValNodeNew(NULL);
	curr->choice = choice;
	curr->data.ptrvalue = alrp;

	if(*prev == NULL)
		*head = curr;
	else
		(*prev)->next = curr;
	*prev = curr;
}


/*
*
*	Add the rectangle for the label of the sequence
*	width is the width of the label
*
*/
static Boolean add_label_rectangle (AlignLabelRectPtr alrp, Int4 width, Int4 label_height, 
		Uint1 label_align, Int4 e_left, Int4 e_top, Int4 e_right, Int4 e_bottom)
{
	Int4 center, middle;
	
	if(alrp == NULL || width == 0)
		return FALSE;
	
	switch(label_align)
	{
		case MSM_LABEL_TOP:
			center = (e_left + e_right)/2;
			alrp->left = center - width/2;
			alrp->right = center + width/2;
			alrp->bottom = e_top;
			alrp->top = e_top + label_height;
			break;
		
		case MSM_LABEL_BOTTOM:
			center = (e_left + e_right)/2;
			alrp->left = center - width/2;
			alrp->right = center + width/2;
			alrp->top = e_bottom;
			alrp->bottom = e_bottom - label_height;
			break;
				
		case MSM_LABEL_LEFT:
			middle = (e_top + e_bottom)/2;
			alrp->right= e_left;
			alrp->left=  alrp->right - width;
			alrp->bottom = middle - label_height/2;
			alrp->top = middle + label_height/2;
			break;

		case MSM_LABEL_RIGHT:
			middle = (e_top + e_bottom)/2;
			alrp->left=e_right;
			alrp->right =  alrp->left + width;
			alrp->bottom = middle - label_height/2;
			alrp->top = middle + label_height/2;
			break;
		default:
			break;
	}
	return TRUE;
}

/************************************************************************
*
*	load the information for image mapping
*
************************************************************************/
Boolean load_align_label_rectangle(ValNodePtr data_list, ValNodePtr PNTR arect_list, Int4 scale, Boolean flat)
{
	AlignNodePtr anp;
	FeatNodePtr fnp;
	BioseqPtr bsp;
	Uint1 label_align, t_label_align;
	AlignLabelRectPtr alrp;
	Int4 width;
	Int4 e_left = 0, e_right = 0;
	Int4 e_top = 0, e_bottom = 0;
	Int4 font_height;
	FonT font;
	Uint1 link_db;
	Int4 gi;
	Int4 label_height = stdLineHeight;
	ValNodePtr prev, curr;
	Uint1 choice;
	Boolean found_it;

	if(data_list == NULL || arect_list == NULL)
		return FALSE;
	prev = NULL;
	curr = *arect_list;
	if(curr != NULL)
	{
		while(curr->next != NULL)
		{
			prev = curr;
			curr = curr->next;
		}
	}

	font = (FonT)GetMuskCParam(MSM_SEQUENCE, MSM_FLABEL, MSM_FONT);
	SelectFont(font);
	font_height = FontHeight();
	label_align = (Uint1)GetMuskCParam(MSM_TOPSTYLE, MSM_LABEL, MSM_STYLE);

	while(data_list)
	{
		found_it = FALSE;
		alrp = NULL;
		/* t_label_align = label_align; */
		if(flat)
			t_label_align = MSM_LABEL_TOP;
		else
			t_label_align = label_align;
		width = 0;
		choice = 0;
		/*process the intervals first */
		switch(data_list->choice)
		{
			case OBJ_SEQALIGN:
			case OBJ_SEQHIST_ALIGN:
				anp = data_list->data.ptrvalue;
				e_left = anp->extremes.left;
				e_right = anp->extremes.right;
				e_top = anp->top;
				e_bottom = anp->bottom;
				label_height = font_height;
				if(anp->label != NULL)
					width = StringWidth(anp->label) * scale;

				bsp = BioseqFindCore(anp->sip);
				gi = 0;
				link_db = 0;
				choice = 0;
				if(bsp != NULL)
				{
					link_db = find_hot_link_gi(bsp->id, &gi);
					choice = bsp->mol;
				}
				else
				{
					link_db = find_hot_link_gi(anp->sip, &gi);
				}
				if((e_right - e_left) /scale > 1)
				{
					alrp = MemNew(sizeof(AlignLabelRect));
					alrp->itemID = anp->itemID;
					alrp->itemType = data_list->choice;
					if(link_db != UNKNOWN_DB && gi != 0)
					{
						alrp->link_db = link_db;
						alrp->gi = gi;
					}
					alrp->left = e_left;
					alrp->right = e_right;
					alrp->top = e_top;
					alrp->bottom = e_bottom;
					link_new_align_label(arect_list, &prev, choice, alrp);
				}
				/*create the new one for the label*/
				if(width > 0)
				{
					alrp = MemNew(sizeof(AlignLabelRect));
					alrp->itemID = anp->itemID;
					alrp->itemType = data_list->choice;
					alrp->link_db = link_db;
					alrp->gi = gi;
				}

				break;

			case OBJ_SEQFEAT:
			case OBJ_BIOSEQ:
			case OBJ_BIOSEQ_SEG:
				fnp = data_list->data.ptrvalue;
				if((fnp->extremes.right - fnp->extremes.left)/scale > 1)
				{
					alrp = MemNew(sizeof(AlignLabelRect));
					alrp->itemID = fnp->itemID;
					alrp->itemType = data_list->choice;
					alrp->right= fnp->extremes.right;
					alrp->left=  fnp->extremes.left;
					alrp->bottom = fnp->bottom;
					alrp->top = fnp->top;
					link_new_align_label(arect_list, &prev, choice, alrp);
				}

				if(fnp->label != NULL && fnp->labelHeight > 0 && fnp->label_len > 0)
				{
					e_left = fnp->extremes.left;
					e_right = fnp->extremes.right;
					e_top = fnp->top;
					e_bottom = fnp->bottom;
					label_height = fnp->labelHeight;
					width = scale * (fnp->label_len);
					alrp = MemNew(sizeof(AlignLabelRect));
					alrp->itemID = fnp->itemID;
					alrp->itemType = data_list->choice;
				}
				if(data_list->choice != OBJ_SEQFEAT)
				{	/*it is a Bioseq or a segment of a Bioseq*/
					if(label_align != MSM_LABEL_RIGHT)
						t_label_align = MSM_LABEL_LEFT;
					else
						t_label_align = MSM_LABEL_RIGHT;
				}
				break;

			case OBJ_BIOSEQ_MAPFEAT:
				fnp = data_list->data.ptrvalue;
				if(fnp->label != NULL && fnp->labelHeight > 0 && fnp->label_len > 0)
				{
					e_left = fnp->extremes.left;
					e_right = fnp->extremes.right;
					e_top = fnp->top;
					e_bottom = fnp->bottom;
					label_height = fnp->labelHeight;
					width = scale * (fnp->label_len);
					t_label_align = MSM_LABEL_TOP;
					alrp = MemNew(sizeof(AlignLabelRect));
					alrp->itemID = fnp->itemID;
					alrp->itemType = data_list->choice;

				}
				break;
			default:
				break;
		}

		/*process the labels*/
		if(width > 0)
		{

			add_label_rectangle (alrp, width, label_height, t_label_align, 
				e_left, e_top, e_right, e_bottom);
			link_new_align_label(arect_list, &prev, choice, alrp);
		}

		data_list = data_list->next;
	}

	return TRUE;
}

/*
*	for the contig maps, the real contig length may not correspond
*	with the mapped interval on the graphics. Need a way to find the 
*	store the interval mapped in proportion to what is in the graphic
*	the intervals are mapped by the function: FindContigList
*/
void AddIntervalForImage(ValNodePtr contig_list, ValNodePtr image_list)
{
	SeqLocPtr contig_slp;
	SeqIdPtr sip;
	ObjectIdPtr oip;
	DbtagPtr db_tag;
	ValNodePtr curr, vnp;
	AlignLabelRectPtr alrp;
	Uint1 db_type;


	for(vnp = contig_list; vnp != NULL; vnp = vnp->next)
	{
		contig_slp = vnp->data.ptrvalue;
		sip = SeqLocId(contig_slp);
		if(sip != NULL && (db_type = FindContigDB(sip)) != 0)
		{
			db_tag = sip->data.ptrvalue;
			oip = db_tag->tag;
			for(curr = image_list; curr != NULL; curr = curr->next)
			{
				alrp = curr->data.ptrvalue;
				if(alrp->link_db == db_type && 
					StringCmp(alrp->label, oip->str) == 0)
				{
					alrp->from = SeqLocStart(contig_slp);
					alrp->to = SeqLocStop(contig_slp);
				}
			}
		}
	}
}




SegmenT PicForAlignNode(ValNodePtr anp_list, SeqLocPtr m_loc, Int4 width, Int4Ptr p_scale, Int4Ptr p_maxwidth, ValNodePtr PNTR arect_list)
{
	Int4 scale;
	SegmenT pic;
	ValNode vn;
	Int4 max_width, min_scale;
	Int4 y_pos;
	Uint2 entityID;
	BioseqPtr bsp;
	

	if(anp_list == NULL)
		return NULL;
	if(arect_list != NULL)
		*arect_list = NULL;
	vn.data.ptrvalue = m_loc;
	vn.next = NULL;
	max_width = CountMaxSeqLabel(&vn);
	scale = FigureMaxScale(&vn, (Int2)width, max_width);
	min_scale = FigureMinScale (&vn, max_width);
	if(scale <=0)
		return NULL;
	pic = CreatePicture();
	y_pos = 0;
	bsp = BioseqFind(SeqLocId(m_loc));
	entityID = ObjMgrGetEntityIDForPointer((Pointer)bsp);
	DrawMPAlignment(anp_list, 0, SeqLocLen(m_loc)-1, m_loc, entityID, scale, &y_pos, MSM_MPAIR, FALSE, pic);
        load_align_label_rectangle(anp_list, arect_list, scale, FALSE);
        /* ObjMgrClearHold(); */
        SeqMgrFreeCache();

	*p_scale = scale;
	*p_maxwidth = max_width;
	return pic;
}


static void setup_option_for_cytomap(CollectSeqOptionPtr csop)
{
	Int2 i;

	csop->nointerval = TRUE;
	csop->slabel_format = PRINTID_TEXTID_ACCESSION;
	csop->seglevels = 0;
	csop->label_size = 20;
	csop->filter_level = 0;
	csop->bsp_type = CYTO_MAP;

	for(i = 0; i<FEATDEF_ANY; ++i)
	{
		csop->flabel_format[i] = OM_LABEL_CONTENT;
		csop->features[i] = FALSE;
	}
}

static void CleanUnanchoredContig(ValNodePtr PNTR anp_list)
{
	ValNodePtr vnp, prev, next;
	AnnotInfoPtr info;
	Boolean clean = FALSE;

	vnp = *anp_list;
	prev = NULL;
	while(vnp != NULL)
	{
		next = vnp->next;
		if(vnp->choice == OBJ_SEQANNOT)
		{
			info = vnp->data.ptrvalue;
			if(info && info->consistent == ALIGN_UNKNOWN)
				clean = TRUE;
			else
				clean = FALSE;
		}
		if(clean)
		{
			if(prev == NULL)
				*anp_list = next;
			else
				prev->next = next;
			vnp->next = NULL;
			FreeAlignNode(vnp);
		}
		else
			prev = vnp;
		vnp = next;
	}
}

static void AddAlignNodeOffset(ValNodePtr anp_list, Int4 offset)
{
	AlignNodePtr anp;
	AlignSegPtr asp;

	while(anp_list)
	{
		if(anp_list->choice != OBJ_SEQANNOT)
		{
			anp = anp_list->data.ptrvalue;
			anp->extremes.left += offset;
			anp->extremes.right += offset;

			for(asp = anp->segs; asp != NULL; asp = asp->next)
			{
				asp->gr.left += offset;
				asp->gr.right += offset;
			}
		}

		anp_list = anp_list->next;
	}
}

static void AddYACAnnotInfo(ValNodePtr PNTR anp_list, Uint1 type)
{
	AnnotInfoPtr info;
	ValNodePtr vnp;

	vnp = *anp_list;
	if(vnp->choice == OBJ_SEQANNOT)
	{
		info = vnp->data.ptrvalue;
		if(type == YAC_MIT)
			StringCpy(info->annotDB, "Contigs from WI/MIT");
		else
			StringCpy(info->annotDB, "Contigs from NHGRI");
		return;
	}

	info = MemNew(sizeof(AnnotInfo));
	if(type == YAC_MIT)
		StringCpy(info->annotDB, "Contigs from WI/MIT");
	else
		StringCpy(info->annotDB, "Contigs from NHGRI");
	vnp = ValNodeNew(NULL);
	vnp->choice = OBJ_SEQANNOT;
	vnp->data.ptrvalue = info;
	vnp->next = *anp_list;
	*anp_list = vnp;
}


static AlignNodePtr find_anp_for_image(AlignLabelRectPtr alrp, ValNodePtr anp_list)
{
	AlignNodePtr anp;

	while(anp_list)
	{
		if(anp_list->choice != OBJ_SEQANNOT)
		{
			anp = anp_list->data.ptrvalue;
			if(anp->itemID == alrp->itemID && anp_list->choice == alrp->itemType)
				return anp;
		}
		anp_list = anp_list->next;
	}

	return NULL;
}


static void add_label_to_image(ValNodePtr PNTR image_list, ValNodePtr anp_list)
{
	AlignLabelRectPtr alrp;
	AlignNodePtr anp;
	ValNodePtr prev, next, curr;
	Uint1 db_type;
	DbtagPtr db_tag;
	ObjectIdPtr oip;

	curr = *image_list;
	prev = NULL;
	while(curr)
	{
		next = curr->next;
		alrp = curr->data.ptrvalue;
		anp = find_anp_for_image(alrp, anp_list);
		if(anp == NULL || anp->sip == NULL || 
			(db_type = FindContigDB(anp->sip)) == 0)
		{
			if(prev == NULL)
				*image_list = next;
			else
				prev->next = next;
			curr->next = NULL;
			ValNodeFreeData(curr);
		}
		else
		{
			prev = curr;
			alrp->link_db = db_type;
			db_tag = anp->sip->data.ptrvalue;
			oip = db_tag->tag;
			alrp->label = StringSave(oip->str);
		}

		curr = next;
	}
}
			


/*produce a picture which shows the contigs that align to the 
* cytogenetic band. It will expand left to the half band size and 
*	right to the half band size. pic_width is the width of the picture
* in the image map, use link_db to do MIT_YAC or NHGRI_YAC and use
*	label to load oip->str. that should give enough information to 
*	re-produce the seq-id
*/
SegmenT DrawCytoContigMap (ValNodePtr slp_list, BioseqPtr cyto_bsp, 
			Int4 pic_width, Int4Ptr pscale, ValNodePtr PNTR image_list)
{
	ValNodePtr curr;
	SeqLocPtr cyto_slp;
	SeqLocPtr slp, t_slp;
	Int4 off_start = 0, off_stop = 0;
	SeqIntPtr sint;
	Int4 len;

	SegmenT pic;
	CollectSeqOption cs_option;
	CollectAlignOption ca_option;

	ValNodePtr features;
	Uint2 entityID;
	Int4 max_label;

	MapPosPtr mpp;
	Int4 scale;
	Int4 left;

	BioseqPtr bsp;
	ValNodePtr aligns;
	ValNodePtr anp_list;
	Uint1 db_type;

	ValNodePtr contig_slp_list, list;	/*stores the mapped loc for each contig*/


	if(slp_list == NULL || cyto_bsp == NULL || pic_width < 100)
		return NULL;
	entityID = ObjMgrGetEntityIDForPointer((Pointer)cyto_bsp);
	if(entityID == 0)
		return NULL;
	
	if(image_list != NULL)
		*image_list = NULL;
	cyto_slp = NULL;
	for(curr = slp_list; curr != NULL; curr = curr->next)
	{
		slp = curr->data.ptrvalue;
		if(BioseqMatch(cyto_bsp, SeqLocId(slp)))
		{
			cyto_slp = SeqLocIntNew(SeqLocStart(slp), SeqLocStop(slp), 
				Seq_strand_plus, SeqLocId(slp));
			len = SeqLocLen(cyto_slp);
			sint = cyto_slp->data.ptrvalue;
			sint->from = MAX(0, sint->from - len/2);
			sint->to = MIN(cyto_bsp->length-1, sint->to + len/2);
			off_start = SeqLocStart(slp) - sint->from;
			off_stop = sint->to - SeqLocStop(slp);
			break;
		}
	}

	if(cyto_slp == NULL)
		return NULL;

	/*calculate the scale for the picture*/
	SelectFont(systemFont);	
	max_label = StringWidth("XXXXXXXXXX");
	pic_width -= max_label;
	if(pic_width < 50)
	{
		SeqLocFree(cyto_slp);
		return NULL;
	}
	len = SeqLocLen(cyto_slp);
	scale = len/pic_width;
	if(len%pic_width > 0)
		scale += 1;

	/*collect information about the cytogenetic band*/
	setup_option_for_cytomap(&cs_option);
	features = CollectItemForSeqLoc(cyto_slp, entityID, 0, FALSE, NULL, 
		&cs_option, NULL, 0);
	if(features == NULL)
	{
		SeqLocFree(cyto_slp);
		return NULL;
	}

	pic = CreatePicture();
	/*draw the cytogenetic map*/
	mpp = MemNew(sizeof(MapPos));
	mpp->slp = cyto_slp;
	mpp->left = 0;
	mpp->right = len;
	mpp->top  = 0;
	mpp->bottom = 0;
	mpp->seq_top = 0;
	mpp->seq_bottom = 0;
	mpp->entityID = entityID;
	if(!DrawCytoMap(&features, mpp, scale, pic))
	{
		FreeFeatureList(features);
		MemFree(mpp);
		SeqLocFree(cyto_slp);
		DeletePicture(pic);
		return NULL;
	}
	add_seq_label_for_map(&features, mpp, pic);
	FreeFeatureList(features);
	SeqLocFree(cyto_slp);

	/*draw the contigs */
	*pscale = scale;
	set_option_for_collect_align(&ca_option, cs_option.label_size, COLLECT_HISTORY);
	ca_option.only_history = FALSE;
	ca_option.map_insert = TRUE;
	ca_option.slabel_format = cs_option.slabel_format;
	ca_option.nointerval = cs_option.nointerval;
	ca_option.no_sort = FALSE;
	ca_option.flat_insert = FALSE;

	anp_list = NULL;
	contig_slp_list = NULL;
	for(curr = slp_list; curr != NULL; curr = curr->next)
	{
		slp = curr->data.ptrvalue;
		/*check if it is the WI map or the Eric Green map*/
		if((db_type = FindContigDB(SeqLocId(slp))) != 0)
		{
			sint = slp->data.ptrvalue;
			left = 0;
			bsp = BioseqFind(sint->id);
			if(bsp != NULL)
			{
				t_slp = SeqLocIntNew(SeqLocStart(slp), SeqLocStop(slp), 
					Seq_strand_plus, SeqLocId(slp));
				sint = t_slp->data.ptrvalue;
				if(off_start > 0)
				{
					sint->from -= off_start;
					if(sint->from < 0)
					{
						left = ABS(sint->from);
						sint->from = 0;
					}
				}
				if(off_stop > 0)
				{
					sint->to += off_stop;
					if(sint->to > bsp->length -1 )
						sint->to = bsp->length;
				}

				aligns = collect_anpnode_with_option(&ca_option, t_slp, entityID, 
					COLLECT_HISTORY, OBJ_SEQALIGN, NULL, NULL, FALSE);
				if(aligns != NULL)
					CleanUnanchoredContig(&aligns);
				if(aligns != NULL)
				{
					list = FindContigList(t_slp);
					ValNodeLink(&contig_slp_list, list);
					if(left > 0)
						AddAlignNodeOffset(aligns, left);
					AddYACAnnotInfo(&aligns, db_type);
					ValNodeLink(&anp_list, aligns);
				}
				SeqLocFree(t_slp);
			}
		}
	}

	if(anp_list != NULL)
	{
		len = StringWidth("XXXXXX");
		DrawMPAlignment(anp_list, mpp->left-len*scale, mpp->right + 8*scale, NULL, mpp->entityID, scale, &(mpp->bottom), MSM_SEQHIST, FALSE, pic);
		if(image_list != NULL)
		{
			load_align_label_rectangle(anp_list, image_list, scale, FALSE);
			/*add the label so that a Seq-id can be made from link_db+label*/
			add_label_to_image(image_list,anp_list);
			/*add the real interval in a contig sequence*/
			AddIntervalForImage(contig_slp_list, *image_list);
		}
		FreeAlignNode(anp_list);
	}
	MemFree(mpp);
	free_slp_list(contig_slp_list);

	return pic;
}


		





