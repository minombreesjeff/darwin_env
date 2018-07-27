
#include <layout.h>
#include <fstyle.h>
#define MAX_INS	40	/*maximum levels for insertions*/
#define MAX_STACK	5	/*maximum allowed level of stacks*/
#include <vibrant.h>

/**********************************************************************
*
*	LayoutNodeFlat(head, maxScale, top, seq_pos, downward, label_pos)
*	layout the FeatNode as a stacked layout
*	head: the list of FeatNode. Will be resorted
*	maxScale: the maximum scale in the picture
*	top: current available position at y-axis
*	seq_pos: store the position for drawing the sequence after the 
*	stacking is over
*	downward: stack the genes below the sequence line
*	label_pos:  if(TRUE), label the map position for each cluster
*
**********************************************************************/

/***********************************************************************
*
*	ModLabelInterval(left, right, maxScale, label)
*	Modify left, right position to take the StringWidth of label into
*	account.
*
************************************************************************/

static Boolean ModLabelInterval(Int4Ptr left, Int4Ptr right, Int4 maxScale, Int4 label_len, Uint1 label_align)
{

	Int4 center;
	Int4 p_left, p_right;
	
		
	if(maxScale <=0 || label_len == 0)
		return FALSE;
	switch (label_align)
	{
		case MSM_LABEL_TOP:
		case MSM_LABEL_BOTTOM:
			p_left = (*left)/maxScale;
			p_right = (*right)/maxScale;
			if(label_len > (p_right - p_left))
			{
				center = (*left + (*right))/2;
				*left = center - (label_len/2) * maxScale;
				*right = center + (label_len/2) * maxScale;
			}
			break;

		case MSM_LABEL_LEFT:
			*left -= (label_len +1)*maxScale;
			break;
		case MSM_LABEL_RIGHT:
			*right += (label_len +1)*maxScale;
			break;
		default:
			return FALSE;
	}
	return TRUE;
}


Int2 get_current_class(FeatNodePtr fnp)
{
	Int2 c_class;
	Boolean has_gb, has_medline;

	c_class = (fnp->feattype == 0) ? MSM_SEQUENCE: (Int2)(fnp->feattype);
	has_gb = (Boolean)((fnp->extra_data) & EXTRA_GENBANK);
	has_medline = (Boolean)((fnp->extra_data) & EXTRA_MEDLINE);
	if(has_gb && has_medline)
		return MSM_EXTRA_BOTH;
	if(has_gb)
		return MSM_EXTRA_GENBANK;
	if(has_medline)
		return MSM_EXTRA_MEDLINE;
	return c_class;
}


static void modify_label_for_supress(FeatNodePtr c_fnp)
{
	Char label[101];

	if(c_fnp->supress_node != NULL)
	{
		if(c_fnp->label != NULL)
		{
			sprintf(label, "[%s]", c_fnp->label);
			MemFree(c_fnp->label);
			c_fnp->label = StringSave(label);
		}
	}
}

static Boolean is_Dseg_label(CharPtr label)
{
	CharPtr str;
	Boolean has_number;

	if(label == NULL)
		return FALSE;
	if(*label != 'D')
		return FALSE;
	has_number = FALSE;
	for(str = label+1; str != NULL && *str != '\0'; ++str)
	{
		if(has_number)
		{
			if(IS_ALPHA(*str))
				return (*str == 'S');
		}
		else
		{
			if(!IS_DIGIT(*str))
				return FALSE;
			has_number = TRUE;
		}
	}

	return FALSE;
}
				
		

static Boolean load_supress_node(ValNodePtr head, ValNodePtr s_node)
{
	FeatNodePtr fnp;
	FeatNodePtr s_fnp;
	Int4 s_pos, pos;
	ValNodePtr vnp;
	Int4 i;
	Boolean has_gb;
	Boolean has_med;
	Uint1 merge_code;	/*1=merge_to_exiting 2=exiting_merge_to_me */

	s_fnp = s_node->data.ptrvalue;
	if(!is_Dseg_label(s_fnp->label))
		return FALSE;
	has_gb = (Boolean)((s_fnp->extra_data) & EXTRA_GENBANK);
	has_med = (Boolean)((s_fnp->extra_data) & EXTRA_MEDLINE);

	s_pos = s_fnp->extremes.left;
	vnp = head;
	i = 0;
	merge_code = 0;
	while(vnp)
	{
		fnp = vnp->data.ptrvalue;
		pos = fnp->extremes.left;
		if(pos == s_pos)
		{
			if(is_Dseg_label(fnp->label))
			/* if(StringNCmp(fnp->label, s_fnp->label, 3) == 0) */
			{
				++i;
				if(!has_gb && !has_med)
					merge_code = 1;
				else
				{
					 if(((fnp->extra_data) & EXTRA_GENBANK) == 0)
					{
						if(has_gb)
							merge_code = 2;
						else
						{
							if(((fnp->extra_data) & EXTRA_MEDLINE) == 0)
								merge_code = 2;
							else
								merge_code = 1;
						}
					}
					else if(!has_gb)
						merge_code = 1;
				}
				/*deal with the landmark genes*/
				if(s_fnp->landmark)	/*it is a landmark gene*/
				{
					if(fnp->landmark)
						merge_code = 0;
					else if(merge_code == 1)
						merge_code = 0;
				}
				else if(fnp->landmark && merge_code == 2)
					merge_code = 0;
		
					
				if(merge_code == 1)
				{
					ValNodeLink(&(fnp->supress_node), s_node);
					return TRUE;
				}
				if(merge_code == 2)
				{
					s_fnp->supress_node = fnp->supress_node;
					fnp->supress_node = NULL;
					vnp->data.ptrvalue = s_fnp;
					s_node->data.ptrvalue = fnp;

					ValNodeLink(&(s_fnp->supress_node), s_node);
					return TRUE;
				}
			}
		}
		vnp = vnp->next;
	}

	if(i < MAX_STACK)
		return FALSE;
	vnp = head;
	while(vnp)
	{
		fnp = vnp->data.ptrvalue;
		pos = fnp->extremes.left;
		if(pos == s_pos)
		{
			/* if(StringNCmp(fnp->label, s_fnp->label, 3) == 0) */
			if(is_Dseg_label(fnp->label))
			{
				if(!fnp->landmark)
				{
					if(s_fnp->landmark)
						merge_code = 2;
					else
						merge_code = 1;
					if(merge_code == 1)
					{
						ValNodeLink(&(fnp->supress_node), s_node);
						return TRUE;
					}
					if(merge_code == 2)
					{
						s_fnp->supress_node = fnp->supress_node;
						fnp->supress_node = NULL;
						vnp->data.ptrvalue = s_fnp;
						s_node->data.ptrvalue = fnp;
						return TRUE;
					}
				}
			}
		}
		vnp = vnp->next;
	}

	return FALSE;

}


static Int2 find_flat_line(Int4 left, Int4 right, ValNodePtr PNTR lines, Int4 space, Int4 num, ValNodePtr curr)
{
	Int4 i;
	ValNodePtr vnp, next, prev;
	FeatNodePtr fnp, n_fnp;
	Int4 space_l, space_r;

	for(i = 0; i<num; ++i)
	{
		vnp = lines[i];
		if(vnp == NULL)
		{	
			lines[i] = curr;
			return (Int2)i;
		}
		else
		{
			prev = NULL;
			while(vnp)
			{
				next = vnp->next;
				fnp = vnp->data.ptrvalue;
				if(prev == NULL)
				{
					if(right+space <= fnp->ef_left)
					{
						lines[i] = curr;
						curr->next = vnp;
						return (Int2)i;
					}
				}
				
				if(next == NULL)
				{
					if(fnp->ef_right + space <= left)
					{
						vnp->next = curr;
						return (Int2)i;
					}
				}
				else
				{
					n_fnp = next->data.ptrvalue;
					space_l = fnp->ef_right + space;
					space_r = n_fnp->ef_left - space;
					if(left >=space_l && right <= space_r)
					{
						vnp->next = curr;
						curr->next = next;
						return (Int2)i;
					}
				}
				prev = vnp;
				vnp = next;
			}
		}
	}

	return -1;
}

Int4 LayoutNodeFlat(ValNodePtr PNTR head, Int4 maxScale, Int4 top, Uint1 label_type, Boolean supress)
{
	Uint1 labelstyle;
	Int4Ptr  y_height;
	Int4 num, k, i, j;
	Boolean has_prev = FALSE;
	FonT font;
	Int2 p_class, ptype = 0;
	Int4 font_height = 0;
	Int4 currentHeight;
	Int4 y_max;
	Int4 left, right, center;
	ValNodePtr vnp, next, prev;
	FeatNodePtr fnp;
	Int4 symbol_width, symbol_height;
	Int4 label_len;
	Uint1 label_align;
	Int4 next_y;
	ValNodePtr maptype_list[MARK_TYPE_NUM];
	ValNodePtr PNTR lines;
	Uint1 map_mark_type;
	Uint1 display_order[MARK_TYPE_NUM];


	if(label_type != UPPER_LABEL && label_type != LOWER_LABEL)
		return top;
	if(label_type == UPPER_LABEL)
		label_align = MSM_LABEL_TOP;
	if(label_type == LOWER_LABEL)
		label_align = MSM_LABEL_BOTTOM;
		
	/*pre-treat the supress node*/
	if(supress)
	{
		vnp = *head;
		*head = NULL;
		while(vnp)
		{
			next = vnp->next;
			vnp->next = NULL;
			if(!load_supress_node(*head, vnp))
				ValNodeLink(head, vnp);
			vnp = next;
		}
		for(vnp = *head; vnp != NULL; vnp = vnp->next)
		{
			fnp = vnp->data.ptrvalue;
			modify_label_for_supress(fnp);
		}
			
	}
	for(num = 0; num<MARK_TYPE_NUM; ++num)
		display_order[num] = (Uint1)num;
	display_order[1] = NO_TYPE;	/*reverse the order of FRAME_WORK*/
	display_order[0] = FRAME_WORK;

	for(num = 0; num<MARK_TYPE_NUM; ++num)
		maptype_list[num] = NULL;

	num = 0;
	vnp = *head;
	while(vnp)
	{
		next = vnp->next;
		fnp = vnp->data.ptrvalue;
		center = (fnp->extremes.left + fnp->extremes.right)/2;
		fnp->extremes.left = center;
		fnp->extremes.right = center;
		map_mark_type = get_map_type(fnp->extra_data);
		vnp->next = NULL;
		ValNodeLink(&(maptype_list[map_mark_type]), vnp);
		++num;
		vnp = next;
	}
	if(num == 0)
		return top;
			
	lines = MemNew((size_t)(num) * sizeof(ValNodePtr));
	y_height = MemNew((size_t)(2*num) * sizeof (Int4));
	symbol_width = 8;	/*temporary*/
	symbol_height = 8;	/*temporary*/

	k =0; i =0;
	labelstyle = NO_LABEL;
	for(j = 0; j<MARK_TYPE_NUM; ++j)
	{
		
		vnp = maptype_list[display_order[j]];
		prev = NULL;
		while(vnp)
		{
			next = vnp->next;
			vnp->next = NULL;
			fnp = vnp->data.ptrvalue;

			/*get the height of the box and the font of label*/
			p_class = get_current_class(fnp);
			if(!has_prev || (p_class != ptype))
			{
				has_prev = TRUE;
				labelstyle = (Uint1)GetMuskCParam(p_class, MSM_FLABEL, MSM_STYLE);
				if(labelstyle != NO_LABEL)
				{
					font = (FonT)GetMuskCParam(p_class, MSM_FLABEL, MSM_FONT);
					SelectFont(font);
					font_height = FontHeight();
				}
				ptype = p_class;
			}
			left = fnp->extremes.left;
			right = fnp->extremes.right;
			label_len = symbol_width * 3;	
			if(fnp->label == NULL)
				fnp->labelHeight = 0;
			else
			{
				fnp->labelHeight = font_height;
				fnp->label_len = StringWidth(fnp->label);
				label_len = MAX(label_len, fnp->label_len);
			}
			ModLabelInterval(&left, &right, maxScale, label_len, label_align);

			fnp->ef_left = left;
			fnp->ef_right = right;
			i = find_flat_line(left, right, lines, 2L*maxScale, num, vnp);
			fnp->line = i;
			currentHeight = fnp->labelHeight + symbol_width;
			y_height[i] = MAX(y_height[i], currentHeight);
			k = MAX(k, i);
			vnp = next;
		}
	}

	*head = NULL;
	for(i = 0; i<=k; ++i)
	{
		ValNodeLink(head, lines[i]);
	}
	MemFree(lines);


	y_max = TICK_LEN;		
	for(i =0; i<= k; ++i)
		y_max += y_height[i];
	
	next_y = top - y_max;
	if(label_align == MSM_LABEL_TOP)
		top -= y_max;


	for(i =0; i<= k; ++i)
	{
		for(vnp = (*head); vnp !=NULL; vnp = vnp->next)
		{
			fnp = vnp->data.ptrvalue;
			if(fnp)
			{
				if(fnp->line == i)
				{
					switch(label_align)
					{
						case MSM_LABEL_TOP:
							fnp->top = top + symbol_height;
							break;

						case MSM_LABEL_BOTTOM:
							fnp->top = top;
							break;
						default:
							fnp->top = top;
							break;
					}
					fnp->bottom = fnp->top - symbol_width;
				}
			}
		}
		if(label_align == MSM_LABEL_TOP)
			top += y_height[i];
		else
			top -= y_height[i];
	}

	MemFree(y_height);
	return next_y;
}

/*for each alignnode, only use the symbol to indicate the center of its left/right 
* border. No label
*/
Int4 LayoutAlignFlat(ValNodePtr head, Int4 maxScale, Int4 top)
{
	Int2 num_line, line;
	Int4 next_line;
	Int4Ptr y_pos;
	AlignNodePtr anp;
	Int4 symbol_height;
	Int4 left, right;

	num_line = get_vnp_num(head);
	if(num_line <= 0)
		return top;
	next_line = top;
	y_pos = MemNew((size_t)2*num_line * sizeof (Int4));
	symbol_height = 8 + 2;	/*8 is the height of symbol, 2 is for separation space*/
	while(head)
	{
		if(head->choice != OBJ_SEQANNOT)
		{
			anp = head->data.ptrvalue;
			left = anp->extremes.left;
			right = left + symbol_height * maxScale;
			line = find_f_pos(left, right, y_pos, 0, (Int2)num_line);
			anp->line = top - (line + 1) * symbol_height;
			next_line = MIN(anp->line, next_line);
		}
		head = head->next;
	}
	MemFree(y_pos);
	return next_line;

}

static Int4 count_one_interval(GatherRange gr_1, GatherRange gr_2)
{
	Int4 count = 0;

	count += ABS(gr_1.left - gr_2.left);
	count += ABS(gr_1.right - gr_2.right);
	return count;
}

static Int4 count_mismatch(FeatNodePtr fnp_1, FeatNodePtr fnp_2, BoolPtr same_num)
{
	Int2 num_1, num_2;
	ValNodePtr vnp_1, vnp_2;
	Int4 count = 0;
	IvalNodePtr inp_1, inp_2;
	

	*same_num = FALSE;
	if(fnp_1->interval != NULL && fnp_2->interval != NULL)
	{
		num_1 = get_vnp_num(fnp_1->interval);
		num_2 = get_vnp_num(fnp_2->interval);
		if(num_1 == num_2)
		{
			vnp_1 = fnp_1->interval;
			vnp_2 = fnp_2->interval;
			while(vnp_1 && vnp_2)
			{
				inp_1 = vnp_1->data.ptrvalue;
				inp_2 = vnp_2->data.ptrvalue;
				count += count_one_interval(inp_1->gr, inp_2->gr);
				vnp_1 = vnp_1->next;
				vnp_2 = vnp_2->next;
				*same_num = TRUE;
			}
			return count;
		}
	}

	return count_one_interval(fnp_1->extremes, fnp_2->extremes);
}

static FloatHi calculate_overlap(Int4 left_1, Int4 right_1, Int4 left_2, Int4 right_2)
{
   Int4 temp1, temp2;
	
   temp1 = MAX(left_1, left_2);
   temp2 = MIN(right_1, right_2);
   if ( right_1 == left_1 ) return 1.0;
   return (FloatHi)(temp2 - temp1 +1)/(FloatHi)(right_1 - left_1 +1);
	
}
				
			
static void link_to_group(ValNodePtr this_group, ValNodePtr new)
{
	FeatNodePtr n_fnp;
	FeatNodePtr fnp;
	ValNodePtr my_node = NULL, curr, prev, next;
	Int4 count, p_count;
	Boolean same, p_same;
	Boolean found;

	if(this_group == NULL || new == NULL)
		return;
	n_fnp = new->data.ptrvalue;

	n_fnp->follower= TRUE;
	curr = this_group;
	while(curr)
	{
		fnp = curr->data.ptrvalue;
		count = count_mismatch(fnp, n_fnp, &same);
		if(curr != this_group && !(fnp->follower))
			break;
		found = FALSE;
		if(fnp->feattype != n_fnp->feattype)
		{
			if(my_node == NULL)
				found = TRUE;
			else
			{
				if(same != p_same)
					found = same;
				else
					found = (p_count >= count);
			}
		}
		if(found)
		{
			my_node = curr;
			p_count = count;
			p_same = same;
		}
		prev = curr;
		curr = curr->next;
	}
	if(my_node == NULL)
	{
		next = prev->next;
		prev->next = new;
		new->next = next;
	}
	else
	{
		curr = this_group;
		while(curr)
		{
			next =curr->next;
			if(curr == my_node)
			{
				curr->next = new;
				new->next = next;
				break;
			}
			curr = curr->next;
		}
	}
}

static void ordered_link(ValNodePtr PNTR head, ValNodePtr new)
{
	ValNodePtr group, prev = NULL;
	FeatNodePtr g_fnp, fnp;
	Int4 left;

	if(*head == NULL)
		*head = new;
	else
	{
		group = *head;
		fnp = new->data.ptrvalue;
		left = fnp->extremes.left;
		while(group!= NULL)
		{
			g_fnp = group->data.ptrvalue;
			if(!(g_fnp->follower))
				if(g_fnp->extremes.left > left)
					break;
			prev = group;
			group = group->next;
		}
		if(prev == NULL)
		{
			new->next = *head;
			*head = new;
		}
		else
		{
			new->next = prev->next;
			prev->next = new;
		}
	}
}

static void make_prev_group(ValNodePtr PNTR h_group, ValNodePtr new)
{
	ValNodePtr next, group;
	FeatNodePtr fnp, n_fnp;
	Int4 e_left, e_right, left, right;
	ValNodePtr this_group;
	FloatHi g_overlap = 0.0, t_overlap = 0.0;
	FloatHi overlap_1, overlap_2;
	Boolean found;
	ValNodePtr head_group;

	head_group = *h_group;
	if(new == NULL || head_group == NULL)
		return;
	while(new)
	{
		next = new->next;
		n_fnp = new->data.ptrvalue;
		e_left = n_fnp->extremes.left;
		e_right = n_fnp->extremes.right;
		this_group = NULL;
		g_overlap = 0.0;
		t_overlap = 0.0;
		for(group = head_group; group != NULL; group = group->next)
		{
			fnp = group->data.ptrvalue;
			if(!(fnp->follower) &&  (fnp->extremes.strand == n_fnp->extremes.strand))	/*only checks the leaders*/
			{
				left = fnp->extremes.left;
				right = fnp->extremes.right;
				if(!(left > e_right || right < e_left))	/*overlaps*/
				{

					overlap_1 = calculate_overlap(left, right, e_left, e_right);
					overlap_2 = calculate_overlap(e_left, e_right, left, right);
					if(overlap_1 < 0.50 && overlap_2 < 0.50) 
						found = FALSE;
					else
					{
						if(fnp->feattype == n_fnp->feattype)
							/*found = (overlap_1 == 1.00 && overlap_2 == 1.00);*/
							found = FALSE;
						else
							found = TRUE;
					}
					if(found)
					{
						if(this_group != NULL)
						{
							if(overlap_1 > t_overlap)
								found = TRUE;
							else
							{
								if(overlap_1 == t_overlap)
									found = (overlap_2 > g_overlap);
							}
						}
					}
					if(found)
					{
						this_group = group;
						g_overlap = overlap_2;
						t_overlap = overlap_1;
					}
				}
			}
		}
	
		new->next = NULL;
		if(this_group != NULL)
			link_to_group(this_group, new);
		else	/*link to the end*/
		{
			n_fnp->follower= FALSE;
			ordered_link(h_group, new);
			/*ValNodeLink(&head_group, new);*/
		}
		
		new = next;
	}
}

/*************************************************************************
*
*	ReSetFeatNode(fnp_node): set all the fnp->line = 0. Before the layout, 
*	fnp->line == 0 indicate the start of a new group.
*
**************************************************************************/			
void ReSetFeatNode(ValNodePtr fnp_node)
{
	ValNodePtr curr;
	FeatNodePtr fnp;
	
	for(curr = fnp_node; curr != NULL; curr = curr->next)
	{
		fnp = curr->data.ptrvalue;
		fnp->follower = FALSE;
	}
}			


/*************************************************************************
*
*	GroupFeatNode(head, new_group)
*	Add a new list of FeatNode (from the same feattype) to the head 
*	of its group. The new group will be sorted 
*
**************************************************************************/			
ValNodePtr GroupFeatNode(ValNodePtr PNTR head)
{

	ValNodePtr next;
	ValNodePtr new_group;

	if(head == NULL || *head == NULL)
		return NULL;
	new_group = *head;
	next = new_group->next;
	new_group->next = NULL;
	make_prev_group(head, next);
	return (*head);
}

static Boolean is_inside_interval(Int4 a, Int4 x, Int4 y)
{
	return (a >=x && a<=y);
}

static Boolean is_mRNA_CDS_compatible(FeatNodePtr cds_node, FeatNodePtr mRNA_node, Int4Ptr offset, Int2Ptr interval_missed)
{
	Int2 cds_num, mRNA_num;
	ValNodePtr cds_interval, mRNA_interval;
	IvalNodePtr cds_inp, mRNA_inp;
	Boolean found;

	if(cds_node->interval == NULL || mRNA_node->interval == NULL)
		return FALSE;
	*offset = 0;
	*interval_missed = 0;

	cds_num = get_vnp_num(cds_node->interval);
	mRNA_num = get_vnp_num(mRNA_node->interval);

	if(mRNA_num < cds_num)
		return FALSE;

	if(cds_node->extremes.strand == Seq_strand_minus)
	{
		if(mRNA_node->extremes.strand != Seq_strand_minus)
			return FALSE;
	}
	if(mRNA_node->extremes.strand == Seq_strand_minus)
	{
		if(cds_node->extremes.strand != Seq_strand_minus)
			return FALSE;
	}


	found = FALSE;
	cds_interval = cds_node->interval; 
	cds_inp = cds_interval->data.ptrvalue;
	mRNA_interval = mRNA_node->interval; 
	while(mRNA_interval != NULL)
	{
		mRNA_inp = mRNA_interval->data.ptrvalue;
		if(is_inside_interval(cds_inp->gr.left, mRNA_inp->gr.left, mRNA_inp->gr.right))
		{
			if(is_inside_interval(cds_inp->gr.right, mRNA_inp->gr.left, mRNA_inp->gr.right))
			{
				found = TRUE;
				if(cds_interval->next != NULL)	/*there is more than one exon in CDS*/
				{
					if(cds_inp->gr.strand == Seq_strand_minus)
						found = (cds_inp->gr.left == mRNA_inp->gr.left);
					else
						found = (cds_inp->gr.right == mRNA_inp->gr.right);
				}
				if(found)
				{
					*offset += (cds_inp->gr.left - mRNA_inp->gr.left);
					*offset += (mRNA_inp->gr.right - cds_inp->gr.right);
				}
			}
		}
		if(found)
			break;
		else
		{
			++(*interval_missed);
			mRNA_interval = mRNA_interval->next;
		}
	}

	if(!found || mRNA_interval == NULL)
		return FALSE;

	cds_num = get_vnp_num(cds_interval->next);
	mRNA_num = get_vnp_num(mRNA_interval->next);
	if(cds_num > mRNA_num)
		return FALSE;

	if(cds_num == 0)
		return TRUE;

	cds_interval = cds_interval->next;
	mRNA_interval = mRNA_interval->next;
	while(cds_interval && mRNA_interval)
	{
		cds_inp = cds_interval->data.ptrvalue;
		mRNA_inp = mRNA_interval->data.ptrvalue;
		if(cds_interval->next == NULL)	/*the last CDS*/
		{
			if(cds_inp->gr.strand == Seq_strand_minus)
			{
				if(mRNA_inp->gr.right!= cds_inp->gr.right)
					return FALSE;
				if(cds_inp->gr.left< mRNA_inp->gr.left)
					return FALSE;
			}
			else
			{
				if(mRNA_inp->gr.left != cds_inp->gr.left)
					return FALSE;
				if(cds_inp->gr.right > mRNA_inp->gr.right)
					return FALSE;
			}
			*offset += (mRNA_inp->gr.right - cds_inp->gr.right);
		}
		else
		{
			if(cds_inp->gr.right != mRNA_inp->gr.right)
				return FALSE;
			if(cds_inp->gr.left != mRNA_inp->gr.left)
				return FALSE;
		}

		cds_interval = cds_interval->next;
		mRNA_interval = mRNA_interval->next;
	}

	return TRUE;
}

static Boolean insert_CDS_to_best_mRNA(ValNodePtr mRNA_head, ValNodePtr cds)
{
	ValNodePtr curr, next;
	FeatNodePtr fnp, c_fnp;
	ValNodePtr best_mRNA = NULL;
	Int4 offset, m_offset = -1;
	Int2 missed, m_missed;	/*number of exons missed at both end*/

	if(mRNA_head == NULL || cds == NULL)
		return FALSE;
	else
	{
		c_fnp = cds->data.ptrvalue;
		curr = mRNA_head;
		while(curr)
		{
			fnp = curr->data.ptrvalue;
			if(fnp->feattype == FEATDEF_mRNA)
			{
				if(is_mRNA_CDS_compatible(c_fnp, fnp, &offset, &missed))
				{
					if(best_mRNA == NULL)
					{
						best_mRNA = curr;
						m_offset = offset;
						m_missed = missed;
					}
					else
					{
						if(missed < m_missed)
						{
							best_mRNA = curr;
							m_offset = offset;
							m_missed = missed;
						}
						else if(missed == m_missed && m_offset > offset)
						{
							best_mRNA = curr;
							m_offset = offset;
							m_missed = missed;
						}

					}
				}
			}

			curr = curr->next;
		}
	}

	if(best_mRNA != NULL)
	{
		curr = mRNA_head;
		c_fnp->follower = TRUE;
		while(curr)
		{
			if(curr == best_mRNA)
			{
				next = curr->next;
				curr->next = cds;
				cds->next = next;
				return TRUE;
			}

			curr = curr->next;
		}
	}

	return FALSE;
}

static ValNodePtr find_best_gene(ValNodePtr curr, Int4 cds_left, Int4 cds_right)
{
	FeatNodePtr fnp;
	ValNodePtr best_gene = NULL;
	Int4 offset, m_offset = -1;
	Int4 left, right;

	if(curr == NULL)
		return NULL;
	else
	{
		while(curr)
		{
			fnp = curr->data.ptrvalue;
			if(fnp->feattype == FEATDEF_GENE)
			{
				if(!(fnp->extremes.left > cds_right|| fnp->extremes.right < cds_left))
				{
					offset = 0;
					left = fnp->extremes.left;
					right = fnp->extremes.right;
					offset += ABS(left - cds_left);
					offset += ABS(cds_right - right);

					if(best_gene == NULL)
					{
						best_gene = curr;
						m_offset = offset;
					}
					else if(offset < m_offset)
					{
						best_gene = curr;
						m_offset = offset;
					}
				}
			}

			curr = curr->next;
		}
	}

	return best_gene;
}


static ValNodePtr find_next_group(ValNodePtr head, Int4Ptr left, Int4Ptr right, ValNodePtr PNTR prev)
{
	ValNodePtr list;
	FeatNodePtr fnp;

	*left = -1;
	*right = -1;

	list = head;
	*prev = NULL;
	while(list)
	{
		fnp = list->data.ptrvalue;
		if(!fnp->follower && list != head)
			return list;
		else
		{
			if(*left == -1)
				*left = fnp->extremes.left;
			else
				*left = MIN(*left, fnp->extremes.left);
			if(*right == -1)
				*right = fnp->extremes.right;
			else
				*right = MAX(*right, fnp->extremes.right);
		}

		*prev = list;
		list = list->next;
	}

	return NULL;
}

static void make_group_follower(ValNodePtr first, ValNodePtr last)
{
	FeatNodePtr fnp;

	while(first)
	{
		fnp = first->data.ptrvalue;
		fnp->follower = TRUE;
		if(first == last)
			break;
		else
			first = first->next;
	}

}

static void insert_cds_mRNA_to_gene(ValNodePtr PNTR g_head, ValNodePtr PNTR cds_head)
{
	ValNodePtr cds_list, prev_list = NULL, prev;
	ValNodePtr best_gene, curr;
	ValNodePtr next, g_next;
	Int4 left, right;

	if(*g_head == NULL)
	{
		*g_head = *cds_head;
		return;
	}

	cds_list = *cds_head;
	while(cds_list)
	{
		next = find_next_group(cds_list, &left, &right, &prev);
		best_gene = NULL;
		if(left != -1 && right != -1)
		{
			best_gene = find_best_gene(*g_head, left, right);
			if(best_gene != NULL)
			{
				make_group_follower(cds_list, prev);
				if(prev_list == NULL)
					*cds_head = prev->next;
				else
					prev_list->next = prev->next;
				prev->next = NULL;
				curr = *g_head;
				while(curr)
				{
					if(curr == best_gene)
					{
						g_next = curr->next;
						curr->next = cds_list;
						prev->next = g_next;
						break;
					}
					else
						curr = curr->next;
				}
			}
		}
		if(best_gene == NULL)
			prev_list = prev;
		cds_list = next;
	}

	if(*cds_head != NULL) /*there is some leftovers*/
	{
		ValNodeLink(g_head, *cds_head);
		*cds_head = NULL;
	}
}

						

ValNodePtr OrderCdsmRNA(ValNodePtr PNTR head)
{
	ValNodePtr g_head;
	ValNodePtr cds_head, curr, next, prev;
	ValNodePtr mRNA_head;

	if(head == NULL || *head == NULL)
		return NULL;

	ReSetFeatNode(*head);
	*head = SortFeatNode(*head, NULL, NULL);

	mRNA_head = NULL;
	g_head = NULL;
	cds_head = NULL;

	g_head = extract_node_list(head, OBJ_SEQFEAT, 0, FEATDEF_GENE, ALL_LABEL);
	mRNA_head = extract_node_list(head, OBJ_SEQFEAT, 0, FEATDEF_mRNA, ALL_LABEL);
	cds_head = extract_node_list(head, OBJ_SEQFEAT, 0, FEATDEF_CDS, ALL_LABEL);

	if(cds_head != NULL)	/*try to group the CDS feature to the mRNA*/
	{
		curr = cds_head;
		prev = NULL;
		while(curr)
		{
			next = curr->next;
			curr->next = NULL;
			if(insert_CDS_to_best_mRNA(mRNA_head, curr))
			{
				if(prev == NULL)
					cds_head = next;
				else
					prev->next = next;
			}
			else
			{
				curr->next = next;
				prev = curr;
			}

			curr = next;
		}
	}

	if(cds_head != NULL)
		ValNodeLink(&mRNA_head, cds_head);
			
	insert_cds_mRNA_to_gene(&g_head, &mRNA_head);
	if(*head != NULL)
		ValNodeLink(&g_head, *head);

	*head = g_head;
	return g_head;
}
		


static Int4 get_interval_height(Int4 p_class)
{
	Int4 segstyle, style;

	style = GetMuskCParam((Int2)p_class, MSM_SEGMENT, MSM_STYLE);
	segstyle = (style & MSM_SEG_FORM);
	switch (segstyle)
	{
		case MSM_SEG_LINE:
			return GetMuskCParam(MSM_SEQUENCE, (Int2)segstyle, MSM_PENWIDTH);
		case MSM_SEG_BOX:
			return GetMuskCParam((Int2)p_class, MSM_SEGMENT, MSM_HEIGHT);
		default:
			return 8;	/*8 is the value for the symbols*/
	}
}


static Int4 layout_uncompressed_group(ValNodePtr prev, ValNodePtr next, Int4Ptr y_pos, Int4Ptr group_value, Int4 next_sp, Int2 num_line, Int4Ptr com_height)
{
	/*this will allow the layout among the features of the same type 
	  within the group */
	ValNodePtr curr;
	ValNodePtr t_prev, t_curr;
	FeatNodePtr c_fnp, t_fnp;
	Uint1 p_feattype;
	Int2 group_size;
	Boolean has_prev = FALSE;
	Int2 group_start, group_stop;
	Int2 i;
	Int4 y_offset;
	Int4 maxline;
	Boolean is_end;
	Int4 left, right;
	Boolean process;

	if(prev == NULL)
		return 0;
	p_feattype = 0;
	group_size = 0;
	group_start = 0;
	group_stop = 0;
	y_offset = 0;	/*after the first line, everything should be layed down*/
	maxline = 0;
	t_prev = NULL;
	is_end = FALSE;
	curr = prev;
	while(!is_end)
	{
		is_end = (curr == next || curr == NULL);
		process = TRUE;
		if(!is_end)
		{
			c_fnp = curr->data.ptrvalue;
			if(p_feattype != c_fnp->feattype || t_prev == NULL)
			{	/*need to find everything in the group*/
				++group_size;
				process = FALSE;	
			}
		}
		if(!process)	/*linking it to the current size*/
		{
			if(t_prev == NULL)
				t_prev = curr;
		}
		else if(t_prev != NULL)
		{
		 	/*either the running to the end or mapped to 
			  the same type of Seq-feat*/
	
			if(group_size == 1)	/*there is only one sequence*/
			{
				left = group_value[2*group_start];
				right = group_value[2*group_start +1];
				i = find_f_pos(left, right, y_pos+2*y_offset, next_sp, (Int2)(num_line - y_offset));
			}
			else
				i = find_group_pos(y_pos+2*y_offset, next_sp, (Int2)(num_line -y_offset), group_size, group_value + group_start*2);

				
			if(!has_prev)
			{
				y_offset = i+group_size -1;
				has_prev = TRUE;
			}
			else
			{
				i+= (Int2)(y_offset);
			}
			maxline = MAX(i+group_size-1, maxline);
			for(t_curr = t_prev; t_curr != curr; t_curr = t_curr->next)
			{
				t_fnp = t_curr->data.ptrvalue;
				t_fnp->line = i+1;	
				if(i != -1)
				{
					com_height[i] = MAX(com_height[i],  t_fnp->bottom);
					++i;
				}
			}
			t_prev = curr;
			group_start += group_size;
			group_size = 1;
		}

		p_feattype = c_fnp->feattype;
		if(!is_end)
			curr = curr->next;
	}

	return maxline;
}

/***********************************************************************
*
*	Int4 LayoutFeatNode(head, top, next_sp, space, maxScale, check_interval)
*	Layout the fnp->line, inp->line according to Maximun packing, return 
*	the next availabel space for drawing	 
*	head: the FeatNode. will be sorted
*	top: the current y position
*	next_sp: min. space for separating two neibouring objects
*	space: the vertical space consumed by each line
*	maxScale: max scale for the picture
*	check_interval: layout each interval?	
*	
*
************************************************************************/
Int4 LayoutFeatNode(ValNodePtr head, Int4 top, Int4 maxScale, Int4 next_sp, Boolean show_label, Boolean compress)
{
	Int4Ptr y_pos, com_height;	/*com_height: storing height for compressed layout*/

	Int4 maxHeight = 0;
	Int4 height;
	Int2 maxline = 0, group_size;
	Int2 i, k;
	Int4 left, right, p_left, p_right;
	Int4 line_scale;	/*scale for showing only the two-ends of interval*/
	Boolean drawline;	/*draw a line between the two ends of the interval*/
	
	Int2 num_line = 0;
	ValNodePtr vnp, curr, prev;
	FeatNodePtr fnp, c_fnp;
	Int2 ptype = 0;		/*previous feature type. to save some time*/
	Int2 p_class;
	FonT font = NULL;
	Int4 font_height = 0;
	Int4 f_space;		/*space separate features at different level*/
	Boolean show_trunc = FALSE;
	Int4 segstyle, labelstyle;
	Int4 cur_line, y_next;
	Boolean has_prev = FALSE;
	Int4 label_len;
	Uint1 label_align;	/*the alignment of a labe: top, left, bottom, right*/
	Int4Ptr group_value;

	if(head == NULL)
		 return top;

	line_scale = GetMuskCParam(MSM_TOPSTYLE, MSM_ENDS, MSM_SCALE);
	drawline = (maxScale >= line_scale);
	f_space = GetMuskCParam(MSM_TOPSTYLE, MSM_SPACE, MSM_HEIGHT);
	/*the alignment of a label in relation to the object*/
	label_align = (Uint1)GetMuskCParam(MSM_TOPSTYLE, MSM_LABEL, MSM_STYLE);

	num_line = (Int2)get_vnp_num(head);
	
	/*for layout each level*/
	y_pos = MemNew((size_t)2*num_line * sizeof (Int4));
	com_height = MemNew((size_t)num_line* sizeof (Int4));
	group_value = MemNew((size_t)2*num_line* sizeof (Int4));

	k = 0;
	labelstyle = NO_LABEL;
	vnp = head;
	prev = NULL;
	label_len = 0;
	group_size = 0;
	while(vnp)
	{
		fnp = vnp->data.ptrvalue;

		/*get the height of the box and the font of label*/
		p_class = get_current_class(fnp);
		if(p_class == MSM_SEQUENCE)	/*assume it is a sequence*/
			drawline = FALSE;
		if(!has_prev || (p_class != ptype))	/*get the styles*/
		{
			has_prev = TRUE;
			if(drawline && fnp->landmark == FALSE)
				height = GetMuskCParam(p_class, MSM_SEG_BORD, MSM_PENWIDTH);
			else
			{
				height = get_interval_height(p_class);
				segstyle = GetMuskCParam(p_class, MSM_SEGMENT, MSM_STYLE);
				labelstyle = GetMuskCParam(p_class, MSM_FLABEL, MSM_STYLE);
				show_trunc = (Boolean)(segstyle & (Int4)MSM_SEG_SHOWTRUNC);
				if(show_label)
				{
					font = (FonT)GetMuskCParam(p_class, MSM_FLABEL, MSM_FONT);
					SelectFont(font);
					font_height = FontHeight();
				}
			}
			ptype = p_class;
		}

		/*convert label to type, content, both, summary format*/
		left = fnp->extremes.left;
		right = fnp->extremes.right;
		/*if(fnp->extremes.l_trunc && show_trunc)
			left -= (6*maxScale);
		if(fnp->extremes.r_trunc && show_trunc)
			right += (6*maxScale);*/
		fnp->labelHeight = font_height;
		if(compress && fnp->follower)
		{
			fnp->labelHeight = 0;
			if(label_align == MSM_LABEL_LEFT)
			{
				if(left < p_left)
				{
					fnp->labelHeight = font_height;
					if(prev != NULL)
					{
						c_fnp = prev->data.ptrvalue;
						c_fnp->labelHeight = 0;
					}
				}
			}
			if(label_align == MSM_LABEL_RIGHT)
			{
				if(right > p_right)
				{
					fnp->labelHeight = font_height;
					if(prev != NULL)
					{
						c_fnp = prev->data.ptrvalue;
						c_fnp->labelHeight = 0;
					}
				}
			}
				
		}

		/*temporary store the value*/
		fnp->bottom = height;
		if(fnp->labelHeight!= 0)
		{
			label_len = StringWidth(fnp->label);
			fnp->label_len = label_len;
			ModLabelInterval(&left, &right, maxScale, label_len, label_align);
		}
		
		/*load the data for each level*/
		if(!(fnp->follower))	/*start of a new group*/
		{
			if( prev != NULL)	/*process the previous group*/
			{
				if(compress)
				{
					i = find_f_pos(p_left, p_right, y_pos, next_sp, (Int2)num_line);
					com_height[i] = MAX(com_height[i], maxHeight);
					maxline = MAX(i, maxline);
					for(curr = prev; curr != vnp; curr = curr->next)
					{
						c_fnp = curr->data.ptrvalue;
						c_fnp->line = i+1;	
					}
				}
				else
				{
					i = (Int2)layout_uncompressed_group(prev, vnp, y_pos, group_value, next_sp, num_line, com_height);
					maxline = MAX(i, maxline);
				}

			}
			p_left = left;
			p_right = right;
			prev = vnp;
			group_size = 1;
			group_value[2*(group_size-1)] = left;
			group_value[2*(group_size-1) +1] = right;
			maxHeight = height;
		}
		else
		{
			p_left = MIN(left, p_left);
			p_right = MAX(right, p_right);
			maxHeight = MAX(height, maxHeight);
			++ group_size;
			group_value[2*(group_size-1)] = left;
			group_value[2*(group_size-1) +1] = right;
		}
		vnp = vnp->next;
	}
	
	if(prev != NULL)	/*process the last one*/
	{
		if(compress)
		{
			i = find_f_pos(p_left, p_right, y_pos, next_sp, (Int2)num_line);
			com_height[i] = MAX(com_height[i], maxHeight);
			maxline = MAX(i, maxline);
			for(curr = prev; curr != vnp; curr = curr->next)
			{
				c_fnp = curr->data.ptrvalue;
				c_fnp->line = i+1;	
			}
		}
		else
		{
			i = (Int2)layout_uncompressed_group(prev, vnp, y_pos, group_value, next_sp, num_line, com_height);
			maxline = MAX(i, maxline);
		}
	}

	MemFree(y_pos);
	MemFree(group_value);

	
	cur_line = top;
	y_next = top;
	for(i = 0; i<= maxline; ++i)	/*Layout each line in each level*/
	{
		maxHeight = 0;
		for(vnp = head; vnp != NULL; vnp = vnp->next)
		{
			fnp = vnp->data.ptrvalue;
			if(fnp->line == i+1)
			{
				if(!(fnp->follower))
					font_height = fnp->labelHeight;
				maxHeight = MAX(maxHeight, font_height);
				top = cur_line;
				top -= (com_height[i] - fnp->bottom)/2;
				switch(label_align)
				{
					case MSM_LABEL_TOP:
						top -= font_height;
						break;
					case MSM_LABEL_LEFT:
					case MSM_LABEL_RIGHT:
						if(font_height > com_height[i])
							top -= (font_height - com_height[i])/2;
						break;
					default:
						break;
				}
				fnp->top = top;
				fnp->bottom = top - fnp->bottom; 
			}
		}
		switch(label_align)
		{
			case MSM_LABEL_TOP:
			case MSM_LABEL_BOTTOM:
				cur_line -= (maxHeight + com_height[i]);
				break;
			case MSM_LABEL_LEFT:
			case MSM_LABEL_RIGHT:
				cur_line -= MAX(maxHeight, com_height[i]);
				break;
			default:
				break;
		}
		cur_line -= f_space;
	}
						
	MemFree(com_height);

	return cur_line;
}

	


void modify_insert_fnode(ValNodePtr fnode, Int4 offset)
{
	FeatNodePtr fnp;
	ValNodePtr ival;
	IvalNodePtr inp;

	while(fnode)
	{
		fnp = fnode->data.ptrvalue;
		(fnp->extremes.left) += offset;
		(fnp->extremes.right) += offset;

		for(ival = fnp->interval; ival!=NULL; ival = ival->next)
		{
			inp = ival->data.ptrvalue;
			(inp->gr.left) += offset;
			(inp->gr.right) += offset;
		}

		fnode = fnode->next;
	}
}

static Int4 LayoutInsertions(AlignNodePtr anp, Int4 maxScale, Int4 cur_line, Int4 height, Int4 space, Int4 l_bound, Int4 r_bound)
{
	AlignSegPtr asp;
	Int4 insert_ypos[MAX_INS];
	Int2 i,level;
	Int4 left, seglen, ins;
	Int4 next_line, feat_line;
	Boolean has_insertion = FALSE;
	Int4 offset;	/*offset between the left pos and the insert pos*/
	ValNodePtr repeat_node;

	
	if(anp == NULL || anp->segs== NULL)
		return cur_line;

	MemSet((Pointer)(insert_ypos), 0, (size_t)MAX_INS * sizeof(Int4));
	level = 0;
	for(asp = anp->segs; asp !=NULL; asp = asp->next)
	{
		if(asp->type == INS_SEG)
		{
			has_insertion = TRUE;
			ins = asp->ins_pos;
			seglen = asp->gr.right;
		
			asp->line = find_insert_ypos(&left, seglen, ins, l_bound, r_bound, insert_ypos, 0, MAX_INS);
			/*asp->line = find_insert_ypos(&left, seglen, ins, l_bound, r_bound, insert_ypos, 3*maxScale, MAX_INS);*/

			/*resume the previous layout for FeatNode*/
			/*offset = asp->gr.left - asp->ins_pos;
			if(offset != 0)
				modify_insert_fnode(asp->cnp, (-offset));*/
			asp->gr.left = left;
			level = MAX((Int2)(asp->line), level);
		}
	}
	
	if(!has_insertion)
		return cur_line;

	for(i = 0; i<(level+1); ++i)
	{
		next_line = cur_line - space - height - space;
		for(asp = anp->segs; asp !=NULL; asp = asp->next)
		{
			if(asp->type == INS_SEG && asp->line == i)
			{
				asp->top = cur_line;	
				asp->bottom = asp->top - height;
				if(asp->cnp != NULL)
				{
					/*offset = asp->gr.left - (asp->ins_pos - asp->gr.right +1);*/
					offset = asp->gr.left - asp->ins_pos;
					modify_insert_fnode(asp->cnp, offset);
					ReSetFeatNode(asp->cnp);
					repeat_node = strip_repeat_feature(&(asp->cnp));
					feat_line = LayoutFeatNode(asp->cnp, (asp->bottom - space), maxScale, 0, FALSE, FALSE);
					if(repeat_node != NULL)
						ValNodeLink(&(asp->cnp), repeat_node);
					next_line = MIN(feat_line, next_line);
				}
			}
		}
		cur_line = next_line ;
	}
	
	return cur_line;
}
	

			
/**********************************************************************
*
*	merge_same_itemID(head, itemID)
*	search in the list of FeatNode to link all the FeatNode that has
*	the same itemID. 
*	head: the list of FeatNode
*	itemID: the itemID in search
*	return the list of FeatNode with the same itemID
*
**********************************************************************/
ValNodePtr merge_same_itemID(ValNodePtr PNTR head, Int2 itemID)
{
	FeatNodePtr fnp, l_fnp;
	ValNodePtr curr, prev = NULL, next;
	ValNodePtr list = NULL;

	curr = *head;
	while(curr)
	{
		fnp = curr->data.ptrvalue;
		next = curr->next;
		if(fnp->itemID == (Uint2)itemID)
		{
			if(prev == NULL)
				*head = curr->next;
			else
				prev->next = curr->next;
			curr->next = NULL;
			if(list == NULL)
				list = curr;
			else
			{
				l_fnp = list->data.ptrvalue;
				l_fnp->extremes.right = fnp->extremes.right;
				l_fnp->extremes.r_trunc = fnp->extremes.r_trunc;
				if(fnp->interval != NULL)
				{
					ValNodeLink(&(l_fnp->interval), (fnp->interval));
					fnp->interval = NULL;
				}
				FreeFeatureList(curr);
			}
				
			curr = next;
		}
		else
		{
			prev = curr;
			curr = curr->next;
		}
	}

	return list;
}

ValNodePtr strip_repeat_feature(ValNodePtr PNTR f_node)
{
	ValNodePtr list = NULL, curr;
	ValNodePtr next;
	ValNodePtr prev;
	FeatNodePtr fnp;

	prev = NULL;
	curr = *f_node;
	if(curr == NULL)
		return NULL;
	while(curr)
	{
		next = curr->next;
		fnp = curr->data.ptrvalue;
		if(fnp->feattype == FEATDEF_repeat_region || fnp->feattype == FEATDEF_repeat_unit)
		{
			curr->next = NULL;
			ValNodeLink(&list, curr);
			if(prev == NULL)
				*f_node = next;
			else
				prev->next = next;
		}
		else
			prev = curr;
		curr = next;
	}

	return list;
}

/******************************************************************
*
*	check to see if the master line has any follower. If does, 
*	add the length of the followers
*
*******************************************************************/
static Boolean modify_master_line(ValNodePtr vnp, Int4Ptr m_left, Int4Ptr m_right, Int4 maxScale, Uint1 label_align)
{
	AlignNodePtr anp;
	Int4 left, right;
	Int4 label_len;
	Boolean retval = FALSE;

	while(vnp)
	{
		anp = vnp->data.ptrvalue;
		if(anp->follower == FALSE)
			return retval;
		else
			retval = TRUE;
		anp = vnp->data.ptrvalue;
		left = anp->extremes.left;
		right = anp->extremes.right;
		if(anp->label != NULL)
		{
			label_len = StringWidth(anp->label);
			ModLabelInterval(&left, &right, maxScale, label_len, label_align);
		}
		if(anp->extremes.l_trunc == TRUE)
			left -= (6*maxScale);
		if(anp->extremes.r_trunc == TRUE)
			right += (6*maxScale);
		*m_left = MIN(*m_left, left);
		*m_right = MAX(*m_right, right);

		vnp = vnp->next;
	}
	return retval;
}

typedef struct align_feature{
	Uint2 itemID;
	Int4 pos;
	Int4 e_left, e_right;
	Int4 p_left, p_right;
	FeatNodePtr fnp;
}AlignFeature, PNTR AlignFeaturePtr;

/*create a summarry of the features in alignment. Get the extremities. So, 
  it only needed to be layed out once */
static void load_align_feature_list(ValNodePtr PNTR list, ValNodePtr fnp_list)
{
	ValNodePtr curr;
	FeatNodePtr fnp;
	ValNodePtr prev = NULL;
	AlignFeaturePtr afp;

	curr = *list;
	fnp = fnp_list->data.ptrvalue;
	if(fnp->feattype == FEATDEF_repeat_region || fnp->feattype == FEATDEF_repeat_unit)
		return;
	while(curr)
	{
		if(curr->choice == fnp_list->choice)
		{
			afp = curr->data.ptrvalue;

			if(afp->itemID == fnp->itemID)
			{
				afp->e_left = MIN(afp->e_left, fnp->extremes.left);
				afp->e_right = MAX(afp->e_right, fnp->extremes.right);
				return;
			}
		}

		prev = curr;
		curr = curr->next;
	}

	curr = ValNodeNew(NULL);
	curr->choice = fnp_list->choice;
	afp = MemNew(sizeof(AlignFeature));
	afp->itemID = fnp->itemID;
	afp->e_left = fnp->extremes.left;
	afp->e_right = fnp->extremes.right;
	afp->fnp = fnp;
	curr->data.ptrvalue = afp;

	if(prev == NULL)
		*list = curr;
	else
		prev->next = curr;
}


	
static Int4 LayoutAlignmentFeature (AlignSegPtr h_asp, Int4 top, Int4 space, Int4 maxScale)
{
	ValNodePtr list, curr;
	ValNodePtr fnp_list;
	FeatNodePtr fnp;
	ValNodePtr layout_list;
	AlignFeaturePtr afp;
	Int4 n_y_pos;
	AlignSegPtr asp;

	list = NULL;
	n_y_pos = top;
	asp = h_asp;
	while(asp)
	{
		if(asp->type != INS_SEG)
		{
			fnp_list = asp->cnp;
			while(fnp_list)
			{
				load_align_feature_list(&list, fnp_list);
				fnp_list = fnp_list->next;
			}
		}
		asp = asp->next;
	}

	if(list == NULL)
		return top;

	top -= space;

	layout_list = NULL;
	for(curr = list; curr != NULL; curr = curr->next)
	{
		afp = curr->data.ptrvalue;
		afp->p_left = afp->fnp->extremes.left;
		afp->p_right = afp->fnp->extremes.right;
		afp->fnp->extremes.left = afp->e_left;
		afp->fnp->extremes.right = afp->e_right;

		ValNodeAddPointer(&layout_list, curr->choice, afp->fnp);
	}

	if(layout_list != NULL)
	{
		n_y_pos  = LayoutFeatNode (layout_list, top, maxScale, 0, FALSE, FALSE);
		for(curr = list; curr != NULL; curr = curr->next)
		{
			afp = curr->data.ptrvalue;
			afp->fnp->extremes.left = afp->p_left;
			afp->fnp->extremes.right = afp->p_right;

			for(asp = h_asp; asp != NULL; asp = asp->next)
			{
				if(asp->type != INS_SEG)
				{
					for(fnp_list = asp->cnp; fnp_list != NULL; fnp_list = 
						fnp_list->next)
					{
						fnp = fnp_list->data.ptrvalue;
						if(fnp != afp->fnp && fnp->itemID == afp->fnp->itemID 
							&& fnp_list->choice == curr->choice)
						{
							fnp->top = afp->fnp->top;
							fnp->bottom = afp->fnp->bottom;
						}
					}
				}
			}
		}
		ValNodeFree(layout_list);
	}

	if(list != NULL)
		ValNodeFreeData(list);
	return n_y_pos;
}

static void load_status_flag(ValNodePtr prev, ValNodePtr curr, Int4 left, Int4 right)
{
	ValNodePtr vnp;
	BoolPtr status;

	for(vnp = prev; vnp != NULL; vnp = vnp->next)
	{
		status = (BoolPtr)(vnp->data.ptrvalue);
		MemSet(status+left, 1, (size_t)(right - left + 1) *sizeof(Boolean));
		if(vnp == curr)
			break;
	}
}

static ValNodePtr init_new_status_list(Int4 level, Int4 size)
{
	ValNodePtr list;
	Int4 i;
	BoolPtr status;

	list = NULL;
	for(i = 0; i<level; ++i)
	{
		status = MemNew((size_t)(size+1) * sizeof(Boolean));
		ValNodeAddPointer(&list, 0, status);
	}

	return list;
}

static Int4 figure_alignment_level(ValNodePtr PNTR line_status_list, Int4 left, Int4 right, Int4 num_seq, Int4 size)
{
	Int4 i;
	Int4 c_num_seq;
	Int4 max_level = 0;
	BoolPtr line_status;
	ValNodePtr list;
	Boolean found;
	ValNodePtr p_node, n_node;

	c_num_seq = 0;
	p_node = NULL;
	max_level = 0;
	for(list = *line_status_list; list != NULL; list = list->next)
	{
		line_status = list->data.ptrvalue;
		found = TRUE;
		for(i = left; i<= right; ++i)
		{
			if(line_status[i])
			{
				found = FALSE;
				break;
			}
		}
		if(!found)
		{
			c_num_seq = 0;
			p_node = NULL;
		}
		else
		{
			++c_num_seq;
			if(p_node == NULL)
				p_node = list;
			if(c_num_seq == num_seq)
			{
				load_status_flag(p_node, list, left, right);
				return (max_level + 1 - (c_num_seq -1));
			}
		}
		++max_level;
	}

	n_node = init_new_status_list(num_seq-c_num_seq, size);
	if(p_node == NULL)
		p_node = n_node;
	ValNodeLink(line_status_list, n_node);
	load_status_flag(p_node, NULL, left, right);
	return (max_level - c_num_seq + 1);
	

}


static void mod_left_right_by_scale(Int4Ptr left, Int4Ptr right, Int4 maxScale)
{
	Int4 t_left, t_right;

	t_left = *left;
	t_right = *right;

	if(maxScale > 1)
	{
		*left = t_left/maxScale;
		if(t_left%maxScale > 0)
			*left -=1;
		*right = t_right/maxScale;
		if(t_right%maxScale > 0)
			*right +=1;

	}
}

static void figure_alignment_boundary(AlignNodePtr anp, Int4Ptr pleft, 
	Int4Ptr pright, Int4 max_len, Uint1 label_align, Int4 maxScale)
{
	Int4 left, right;
	Int4 label_len;

	left = anp->extremes.left;
	right = anp->extremes.right;
	mod_left_right_by_scale(&left, &right, maxScale);


	if(anp->label != NULL)
	{
		label_len = StringWidth(anp->label);
		ModLabelInterval(&left, &right, 1, label_len, label_align);
	}
	if(anp->extremes.l_trunc == TRUE)
		left -= 14;

	if(anp->extremes.r_trunc == TRUE)
		right += 14;
	else
		right += 2;
	*pleft = MAX(left, 0);
	*pright = MIN(right, max_len);
}


static Boolean is_level_overlap(ValNodePtr PNTR pleft_list, 
		ValNodePtr PNTR pright_list, Int4 left, Int4 right, AlignNodePtr anp)
{
	Int4 l_left, l_right;
	Int4 level = 0;
	ValNodePtr left_list, right_list;

	left_list = *pleft_list;
	right_list = *pright_list;
	anp->line = 0;
	while(left_list && right_list)
	{
		l_left = left_list->data.intvalue;
		l_right = right_list->data.intvalue;

		if(left >= l_right || right <= l_left)
		{
			left_list->data.intvalue = MIN(l_left, left);
			right_list->data.intvalue = MAX(l_right, right);
			anp->line = level;
			return FALSE;
		}

		left_list = left_list->next;
		right_list = right_list->next;
		++level;
	}

	ValNodeAddInt(pleft_list, 0, left);
	ValNodeAddInt(pright_list, 0, right);
	anp->line = level;
	return TRUE;
}

static Int4 find_align_line_number(AlignNodePtr anp, 
	ValNodePtr t_next_list, Uint1 label_align, ValNodePtr PNTR pnext, 
	ValNodePtr PNTR line_status_list, Int4 size, Int4 maxScale )
{
	Int4 m_left, m_right;
	Int4 left, right;
	AlignNodePtr n_anp;
	ValNodePtr left_list= NULL, right_list = NULL;
	Int4 level = 0;
	Int4 line;
	ValNodePtr next_list;

	if(anp == NULL)
		return -1;

	*pnext = t_next_list;

	figure_alignment_boundary(anp, &left, &right, size, label_align, maxScale);
	ValNodeAddInt(&left_list, 1, left);
	ValNodeAddInt(&right_list, 1, right);
	m_left = left;
	m_right = right;
	level = 1;
	anp->line = 0;
	next_list = t_next_list;

	while(next_list)
	{
		n_anp = next_list->data.ptrvalue;
		if(n_anp->follower == TRUE)
		{
			n_anp->line = 0;
			figure_alignment_boundary(n_anp, &left, &right, size, label_align, maxScale);
			if(is_level_overlap(&left_list, &right_list, left, right, n_anp))
				++level;
			m_left = MIN(m_left, left);
			m_right = MAX(m_right, right);
			*pnext = next_list->next;
			next_list = next_list->next;
		}
		else
			break;
	}

	line = figure_alignment_level(line_status_list, 
			m_left, m_right, level, size);
	line -= 1;
	anp->line += line;
	for(next_list = t_next_list; next_list != NULL && next_list != *pnext; next_list = next_list->next)
	{
		n_anp = next_list->data.ptrvalue;
		n_anp->line += line;
	}
	ValNodeFree(left_list);
	ValNodeFree(right_list);
	return (line + level -1);
}

static Boolean get_anp_extremes(ValNodePtr anp_list, Int4Ptr m_left, Int4Ptr m_right)
{
	AlignNodePtr anp;

	*m_left = -1;
	*m_right = -1;
	while(anp_list)
	{
		anp = anp_list->data.ptrvalue;
		if(*m_left == -1 || *m_left > anp->extremes.left)
			*m_left = anp->extremes.left;

		if(*m_right == -1 || *m_right < anp->extremes.right)
			*m_right = anp->extremes.right;

		anp_list = anp_list->next;
	}

	return (*m_left != -1);
}


	


/**********************************************************************
*
*	LayoutAlignNode(head, top, maxScale)
*	head: the list of AlignNode
*	top: the current y position
*	maxScale: the maximum scale
*	the labels on the features of a sequence is not counted in the layout
*
***********************************************************************/
Int4 LayoutAlignNode (ValNodePtr head, Int4 top, Int4 maxScale, Int4 height)
{
	Int4 num = 0;
	Int4 i, j, k;
	Int4 left, right, m_left, m_right;

	ValNodePtr vnp, curr, next;
	AlignNodePtr anp;
	AlignSegPtr asp;
	Int4 cur_line, next_line;
	Int4 ins_line;
	Int4 font_height = 0;	/*height of the Font*/
	Int4 h_font_height, h_height;
	Int4 master_height;
	Int4 space;	/*the space separate the two object*/ 
	FonT font;
	Uint1 label_align;
	Int4 font_offset = 0, h_font_offset;
	Boolean is_first;
	ValNodePtr line_status_list = NULL;

	

	curr = head;
	if(curr == NULL)
		return top;
	SortSameSeqInAlignNode(head);

	space = GetMuskCParam(MSM_TOPSTYLE, MSM_SPACE, MSM_HEIGHT);
	label_align = (Uint1)GetMuskCParam(MSM_TOPSTYLE, MSM_LABEL, MSM_STYLE);
	font = (FonT)GetMuskCParam(MSM_SEQUENCE, MSM_SLABEL, MSM_FONT);
	SelectFont(font);
	font_height = FontHeight();
	master_height = GetMuskCParam(MSM_SEQUENCE, MSM_SEGMENT, MSM_HEIGHT);

	get_anp_extremes(head, &m_left, &m_right);
	mod_left_right_by_scale(&m_left, &m_right, maxScale);
	

	vnp = head;
	k = -1;
	line_status_list = NULL;
	while(vnp)
	{
		anp = vnp->data.ptrvalue;
		i = find_align_line_number(anp, vnp->next, 
			label_align, &next, &line_status_list, m_right, maxScale);
		if(i > k)
			k = i;
		vnp = next;
	}

	if(line_status_list != NULL)
		ValNodeFreeData(line_status_list);


	font_offset = 0;
	switch(label_align)
	{
		case MSM_LABEL_TOP:
		case MSM_LABEL_BOTTOM:
			font_offset = font_height;
			break;
		case MSM_LABEL_LEFT:
		case MSM_LABEL_RIGHT:
			if(font_height > height)
				font_offset = (font_height - height);
			break;
		default:
			break;
	}
		
	cur_line = top;		/*layout the actural y positions*/
	h_font_height = font_height;
	h_font_offset = font_offset;
	h_height = height;
	for(j=0; j<=k; ++j)
	{
		is_first = TRUE;
		for(vnp=(head); vnp!=NULL; vnp=vnp->next)
		{
			anp = vnp->data.ptrvalue;
			if(anp->line == j)
			{
				font_height = (anp->label) ? h_font_height : 0;
				font_offset= (anp->label) ? h_font_offset : 0;
				if(anp->is_master)
					height = master_height;
				else
					height = h_height;
				if(is_first)
				{
					next_line = cur_line - height - font_offset - space;
					is_first = FALSE;
				}
				switch(label_align)
				{
					case MSM_LABEL_TOP:
						anp->top = cur_line - font_height;
						break;
					case MSM_LABEL_BOTTOM:
						anp->top = cur_line;
						break;
					case MSM_LABEL_LEFT:
					case MSM_LABEL_RIGHT:
						anp->top = cur_line - font_offset/2;
						break;
					default:
						break;
				}
						
				anp->bottom = anp->top- height;

				left = anp->extremes.left;
				right = anp->extremes.right;
				ins_line = anp->bottom - space;
				
				/*Layout the Non-insertions*/
				/*collect all the features in the alignment */
				for(asp = anp->segs; asp !=NULL; asp = asp->next)
				{
					if(asp->type != INS_SEG)
					{
						if(asp->type == GAP_SEG)
						{
							asp->bottom = anp->bottom + height/2;
							asp->top = asp->bottom;
						}
						else
						{
							asp->bottom = anp->bottom;
							asp->top = asp->bottom + height;
						}
					}
				}
				ins_line = LayoutAlignmentFeature (anp->segs, anp->bottom, space, maxScale);
				ins_line = LayoutInsertions(anp, maxScale, ins_line, height, space, left, right);
				if(anp->extremes.l_trunc  == TRUE|| anp->extremes.r_trunc == TRUE)
					ins_line = MIN(ins_line, anp->bottom - space - 8);
				next_line = MIN(next_line, ins_line);
				
			}
		}
		cur_line = next_line;
	}

	return next_line;
}


/****************************************************************
*
*	extract a list of AlignNode that matches the sip
*	or things with the same clone id
*
*****************************************************************/
static ValNodePtr extract_match_seqid_list(ValNodePtr PNTR n_curr, SeqIdPtr sip, CharPtr clone_id, Int2Ptr num_follower)
{
	ValNodePtr curr, prev, next;
	ValNodePtr list;
	AlignNodePtr anp;

	list = NULL;
	prev = NULL;
	curr = *n_curr;
	*num_follower = 0;
	while(curr)
	{
		next = curr->next;
		anp = curr->data.ptrvalue;
		if(SeqIdMatch(anp->sip, sip) || 
			(clone_id != NULL && StringCmp(anp->clone_id, clone_id) == 0))
		{
			anp->follower = TRUE;
			if(prev == NULL)
				*n_curr = curr->next;
			else
				prev->next = curr->next;
			curr->next = NULL;
			ValNodeLink(&list, curr);
			++(*num_follower);
		}
		else
			prev = curr;
		curr= next;
	}

	return list;
}
/******************************************************************
*
*	SortSameSeqInAlignNode(anp_list)
*	if the same sequence appears multiple times in the anp_list, 
*	it will be moved to the sequence that are the head of this 
*	list. The field anp->follower is set as the order of the 
*	repeats in this list
*
******************************************************************/
void SortSameSeqInAlignNode(ValNodePtr anp_list)
{
	ValNodePtr curr, n_curr;
	AlignNodePtr anp;
	ValNodePtr same_list;
	Int2 num_follower;

	curr = anp_list;
	while(curr)
	{
		anp = curr->data.ptrvalue;
		n_curr = curr->next;
		if(n_curr != NULL)
		{
			same_list = extract_match_seqid_list(&n_curr, anp->sip, anp->clone_id, &num_follower);
			if(same_list != NULL)
			{
				anp->num_follower = num_follower;
				ValNodeLink(&same_list, n_curr);
				curr->next = same_list;
			}
		}
		curr = n_curr;
	}
}
		
