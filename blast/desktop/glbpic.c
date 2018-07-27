/*   panview.c
*
* File Name:  testseq.c
*
* Author: Jinghui Zhang
*
* Version Creation Date:   2/10/93
*
* File Description:
*
* Modifications:
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*11/6/93		used as a main() for testing the functions written
*			for the sequence displayer
*			SetFeatureProc can be reused universally
*
* ==========================================================================
*/

#include <vibrant.h>
#include <glbpic.h>
#include <fstyle.h>
#include <lsqfetch.h>
#include <drawseq.h>

#define MIN_WIDTH 20

/***************************************************************************
*
*	functions related to freeing the GlobalDrawPtr
*
****************************************************************************/
static ValNodePtr GlobalDataFree(ValNodePtr gbp_list)
{
	GlobalBspPtr gbp;
	ValNodePtr curr, next;
	AlignRegionPtr arp;

	while(gbp_list)
	{
		gbp = gbp_list->data.ptrvalue;
		next = gbp_list->next;
		gbp_list->next = NULL;
		
		if(gbp->l_marks != NULL)
			ValNodeFreeData(gbp->l_marks);
		if(gbp->cyto_marks != NULL)
			ValNodeFreeData(gbp->cyto_marks);
		if(gbp->g_marks != NULL)
			ValNodeFreeData(gbp->g_marks);

		/*freeing the global display of repeat features and alignment*/
		if(gbp->rrp_list)
			ValNodeFreeData(gbp->rrp_list);
		if(gbp->arp_list)
		{
			for(curr = gbp->arp_list; curr != NULL; curr = curr->next)
			{
				arp = curr->data.ptrvalue;
				ValNodeFreeData(arp->intervals);
			}
			ValNodeFreeData(gbp->arp_list);
		}

		if(gbp->label)
			MemFree(gbp->label);
		ValNodeFreeData(gbp_list);
		gbp_list = next;
	}

	return NULL;
}

static void separate_vnp_list(ValNodePtr PNTR head, ValNodePtr list)
{
	ValNodePtr prev = NULL, curr;

	if(list == NULL)
		return;
			
	curr = *head;
	while(curr)
	{
		if(curr == list)
		{
			if(prev == NULL)
				*head = NULL;
			else
				prev->next = NULL;
			return;
		}
		prev = curr;
		curr = curr->next;
	}
}

/**************************************************************************
*
*	free_gkibak_draw(gdraw_p)
*	free all the data related to the global draw
*
***************************************************************************/
GlobalDrawPtr free_global_draw(GlobalDrawPtr gdraw_p)
{
	GlobalDataFree(gdraw_p->gbp_list);
	
	separate_vnp_list(&(gdraw_p->equiv_align_list), gdraw_p->ext_align);
	ValNodeFree(gdraw_p->equiv_align_list);
	
	GeneDataFree(gdraw_p->g_data);
	
	if(gdraw_p->free_data)
	{
		FreeEquivAlign(gdraw_p->ext_align);
		/* ValNodeFreeData(gdraw_p->g_list); */
		FreeMuskSep(gdraw_p->msp_list);
	}
	else
		ValNodeFree(gdraw_p->msp_list);
		

	/*there are selected slp_list*/
	if(gdraw_p->slp_list)
		free_slp_list(gdraw_p->slp_list);
	
	MemFree(gdraw_p);
	return NULL;
}

/***************************************************************************
*
*	functions related to creating the GlobalDrawPtr
*
****************************************************************************/		
/***************************************************************************
*
*	create the structure of GeneDataPtr from a list of gene symbols
*
****************************************************************************/		
GeneDataPtr create_gene_data (ValNodePtr msp_list, ValNodePtr mark_list)
{
	ValNodePtr curr;
	MuskSepPtr msp;
	GeneDataPtr gdp = NULL;
	Int2 seglevels;
	SeqEntryPtr sep;
	BioseqPtr bsp;
	SeqLoc sl;
	SeqLocPtr slp;
	
	if(msp_list == NULL)
		return NULL;

		
	for(curr = msp_list; curr != NULL; curr = curr->next)
	{
		msp = (MuskSepPtr)curr->data.ptrvalue;
		if(msp && msp->sep)
		{
			sep = msp->sep;
			bsp = find_big_bioseq(sep);
			if(bsp != NULL)
			{
				seglevels = get_seglevels(bsp);
				if(seglevels == 1)
				{
					sl.choice = SEQLOC_WHOLE;
					sl.data.ptrvalue = bsp->id;
					sl.next = NULL;
					slp = &sl;
				}
				else
					slp = NULL;

				load_gdata_marks(slp, mark_list, seglevels, msp->sep, &gdp);
			}
		}
	}
	
	return gdp;
}



/***************************************************************************
*
*	load the alignment data for landmark genes
*
****************************************************************************/		

static Boolean load_inconsistent_align(StdSegPtr ssp, Int4 min_inconsist_sp)
{
	Int4 min_val, max_val;
	SeqLocPtr slp;

	min_val = -1;
	max_val = -1;
	for(slp = ssp->loc; slp != NULL; slp = slp->next)
	{
		if(min_val == -1)
			min_val = SeqLocStart(slp);
		else
			min_val = MIN(min_val, SeqLocStart(slp));
		if(max_val == -1)
			max_val = SeqLocStop(slp);
		else
			max_val = MAX(max_val, SeqLocStop(slp));
	}

	return ((max_val - min_val + 1 ) > min_inconsist_sp);
}

static Boolean load_std_seg(StdSegPtr ssp, SeqLocPtr location, ValNodePtr PNTR head, Uint1 type, Int4 min_inconsist_sp)
{
	SeqLocPtr locs;
	StdSegPtr t_ssp;

	for(locs = ssp->loc; locs != NULL; locs = locs->next)
	{
		if(SeqLocCompare(locs, location) == SLC_A_EQ_B)
		{
			if(type == 2)	/*inconsistent markers*/
			{
				if(min_inconsist_sp >0 && 
					!load_inconsistent_align(ssp, min_inconsist_sp))
					return TRUE;
			}
			if(*head == NULL)
				ValNodeAddPointer(head, type, ssp);
			else
			{
				t_ssp = (*head)->data.ptrvalue;
				if(type == 4 || t_ssp->dim < ssp->dim)
				{
					(*head)->data.ptrvalue = ssp;
					(*head)->choice = type;	/*the type of markers. consistent/inconsistent*/
				}
			}
			return TRUE;
		}
	}
	return FALSE;
}

static Boolean check_std_align(SeqLocPtr location, SeqAlignPtr align, ValNodePtr PNTR head, Uint1 type, Int4 min_inconsist_sp)
{
	StdSegPtr ssp;
	if(align->type == 3)
	{
		ssp = align->segs;
		while(ssp)
		{
			if(load_std_seg(ssp, location, head, type, min_inconsist_sp))
				return TRUE;
			ssp = ssp->next;
		}
	}
	return FALSE;
}

static void collect_equiv_align_for_landmark(GeneDataPtr gdp, ValNodePtr ealign_list, Int4 min_inconsist_sp)
{
	SeqAnnotPtr annot;
	SeqAlignPtr align;
	GeneDataPtr curr;
	Int2 type;
	StdSegPtr ssp;
	GeneDataPtr prev;

	if(gdp == NULL)
		return;
	prev = gdp;
	while(prev->next != NULL)
		prev = prev->next;

	while(ealign_list)
	{
	  annot = (SeqAnnotPtr)(ealign_list->data.ptrvalue);
	  type  = GetEquivAlignType(annot);
	  if(type == -1)
		type = 1;
	  if(type  != 0 && type != 3) /*excluding alignment with type 3*/
	  {
		align = (SeqAlignPtr)(annot->data);
		if(type == 4)
		{
			while(align)
			{
				ssp = align->segs;
				while(ssp)
				{
					curr = MemNew(sizeof(GeneData));
					ValNodeAddPointer(&(curr->align_seg), (Uint1)type, ssp);
					prev->next = curr;
					prev = curr;
					ssp = ssp->next;
				}
				align = align->next;
			}
		}
		else
		{
			while(align)
			{
				for(curr = gdp; curr!=NULL; curr = curr->next)
				{
					/* if(curr->landmark) */
						check_std_align(curr->location, align, &(curr->align_seg), (Uint1)type, min_inconsist_sp);
				}
						
				align = align->next;
			}
		}
	  }
	  ealign_list = ealign_list->next;
	}
}





static Boolean add_genedata_to_GlobalBsp(GlobalBspPtr g_bsp, GeneDataPtr gdp)
{
	MapMarkPtr mmp;
	
	if(g_bsp == NULL || gdp == NULL)
		return FALSE;
		
	while(gdp)
	{
		if(BioseqMatch(g_bsp->bsp, SeqLocId(gdp->location)))
		{
			mmp = MemNew(sizeof(MapMark));
			/* StringCpy(mmp->label, gdp->symbol); */
			LabelCopy(mmp->label, gdp->symbol, 19);
			mmp->pos = (SeqLocStart(gdp->location) + SeqLocStop(gdp->location))/2;
			/*mmp->inward = IS_NUM_GENE(gdp->symbol);*/
			if(gdp->landmark)
				ValNodeAddPointer(&(g_bsp->l_marks), 0, mmp);
			else
				ValNodeAddPointer(&(g_bsp->g_marks), 0, mmp);
		}
		gdp = gdp->next;
	}
	return TRUE;
}



static int LIBCALLBACK MapMarkCompProc (VoidPtr ptr1, VoidPtr ptr2)
{
	MapMarkPtr mmp_1, mmp_2;
	ValNodePtr vnp1, vnp2;

	if(ptr1!=NULL && ptr2 != NULL)
	{
		vnp1 = *((ValNodePtr PNTR) ptr1);
		vnp2 = *((ValNodePtr PNTR) ptr2);
		if(vnp1 !=NULL && vnp2 != NULL)
		{
			mmp_1 = vnp1->data.ptrvalue;
			mmp_2 = vnp2->data.ptrvalue;
			if(mmp_1->pos > mmp_2->pos)
				return 1;
			if(mmp_1->pos < mmp_2->pos)
				return -1;
		}
	}

	return 0;
}


static ValNodePtr sort_map_marker(ValNodePtr marks)
{
	return SortValNode(marks, MapMarkCompProc);
}

static ValNodePtr LoadCytoNode(BioseqPtr bsp, ValNodePtr PNTR head)
{
	SeqFeatPtr sfp;
	MapMarkPtr mmp;
	UserObjectPtr uop;
	Int4 pos;
	SeqLocPtr slp;
	CharPtr band_name;

	if(bsp->repr !=Seq_repr_map)
		return (*head);
	sfp = bsp->seq_ext;
	if(sfp->data.choice != 14)
		return (*head);
	
	while(sfp)
	{
		slp = sfp->location;
		pos = (SeqLocStart(slp) + SeqLocStop(slp))/2;
		uop = sfp->data.value.ptrvalue;
		band_name = get_band_name(uop);
		if(band_name !=NULL)
		{
			mmp = MemNew(sizeof(MapMark));
			/* StringCpy(mmp->label, band_name); */
			LabelCopy(mmp->label, band_name, 19);
			mmp->pos = pos;
			mmp->inward = TRUE;
			ValNodeAddPointer(head, 1, mmp);
		}
		sfp = sfp->next;
	}

	return (*head);
}

/***************************************************************************
*
*	GlobalDataNew(): Create a ValNode that contains the information for 
*	bsp: the map Bioseq
*	map_type: the map type, such as GENETIC, PHYSICAL, etc
*	show_map_unit:	label the map unit in drawing
*	check_cyto:	checking if it is a cytogenetic map
*	head:		the pre-existing GlobalData.
*	gdp:	the GeneDataPtr which will map some into the individual 
*	The new GlobalData will be linked to the end of the head, and the 
*	return TRUE for a circular molecule. FALSE for a linear
*
****************************************************************************/
static Boolean GlobalDataNew(BioseqPtr bsp, Uint1 map_type, Boolean show_map_unit, Boolean check_cyto, GeneDataPtr gdp, ValNodePtr PNTR head, Uint2 priority)
{
	GlobalBspPtr g_bsp;
	Uint1 val;

	if(bsp == NULL)
		return FALSE;

	g_bsp = MemNew(sizeof(GlobalBsp));
	
	g_bsp->hide = FALSE;	
	g_bsp->bsp = bsp;
	g_bsp->map_type = map_type;
	g_bsp->show_map_unit = show_map_unit;
	g_bsp->check_cyto = check_cyto;
	g_bsp->l_marks = NULL;
	g_bsp->g_marks = NULL;
	g_bsp->cyto_marks = NULL;
	g_bsp->priority = priority;
	g_bsp->num_lod_score = GetLODScoreNumber (bsp);
	if(priority == SEQINDEX_VAL)
		g_bsp->is_seqindex_map = TRUE;
	if(check_cyto)
		LoadCytoNode(bsp, &(g_bsp->cyto_marks));
		
	add_genedata_to_GlobalBsp(g_bsp, gdp);
	g_bsp->l_marks = sort_map_marker(g_bsp->l_marks);

	if(bsp->topology == 2)
		val = CIRCLE_BSP;
	else
		val = LINEAR_BSP;
	ValNodeAddPointer(head, val, g_bsp);
	return (val == CIRCLE_BSP);
}

	
static Int4 map_linear_xpos (Int4 pos, Int4 seq_len, Int4 left, Int4 right)
{
	FloatHi temp;

	if(seq_len == 0)
		return -1;
	temp = (FloatHi)pos * (FloatHi)(right - left)/(FloatHi)seq_len ;
	
	return (left + (Int4)temp);
}


static Int4 find_global_alignment_position (ValNodePtr PNTR list, Int4 left, Int4 right, Int4 width, Int4 offset)
{
	BoolPtr line_data;
	ValNodePtr curr;
	ValNodePtr vnp, prev = NULL;
	Int4 line = 0;
	Int4 j;
	Boolean found;
	Int4 pix_space = 5;

	left -= offset;
	right -= offset;

	found = FALSE;
	curr = *list;
	prev = NULL;
	while(curr)
	{
		line_data = curr->data.ptrvalue;
		if(line_data[left] == 0 && line_data[right] == 0)
		{
			found = TRUE;
			for(j = left; j<right; ++j)
			{
				if(line_data[j] != FALSE)
				{
					found = FALSE;
					break;
				}
			}
		}
		if(!found)
		{
			prev = curr;
			++line;
		}
		else
			break;
		curr = curr->next;
	}

	if(!found)
	{
		line_data = MemNew((size_t)width * sizeof(Boolean));
		vnp = ValNodeNew(NULL);
		vnp->data.ptrvalue = line_data;
		if(prev == NULL)
			*list = vnp;
		else
			prev->next = vnp;	
	}

	left = MAX(0, left - pix_space);
	right = MIN(width -1, right + pix_space);
	MemSet(line_data+left, 1, (size_t)(right - left +1) * sizeof(Boolean));
	return line;
}
			

			

static ValNodePtr layout_one_alignment_view(ValNodePtr arp_list, 
	Int4 bsp_len, Int4 left, Int4 right, Boolean align_has_status)
{
	Int4 num;
	ValNodePtr curr;
	Int4Ptr y_pos;
	AlignRegionPtr arp;
	Int4 t_left, t_right;
	ValNodePtr line_data = NULL;

	if(arp_list == NULL)
		return NULL;

	line_data = NULL;
	num = 0;
	for(curr = arp_list; curr != NULL; curr = curr->next)
	{
		arp = curr->data.ptrvalue;
		arp->g_left = map_linear_xpos (arp->gr.left, bsp_len, left, right);
		arp->g_right = map_linear_xpos (arp->gr.right, bsp_len, left, right);
		if(align_has_status)
			arp->line = find_global_alignment_position (&line_data, arp->g_left, arp->g_right, (right-left+1), left);
		else
			++num;
	}
	ValNodeFreeData(line_data);
	if(align_has_status)
		return (arp_list);

	/*for alignment that does not have status*/
	y_pos = MemNew((size_t)(num*2) * sizeof(Int4));
	for(curr = arp_list; curr != NULL; curr = curr->next)
	{
		arp = curr->data.ptrvalue;
		t_left = arp->g_left;
		t_right = arp->g_right;
		arp->line = find_f_pos(t_left, t_right, y_pos, 1, (Int2)num);
	}
	MemFree(y_pos);
	return (arp_list); 
}


static ValNodePtr extract_align_range(ValNodePtr PNTR arp_list, Uint1 displayOrder)
{
	ValNodePtr curr, prev = NULL, list = NULL, p_list, next;
	AlignRegionPtr arp;

	curr = *arp_list;
	p_list = NULL;
	prev = NULL;
	while(curr)
	{
		next = curr->next;
		arp = curr->data.ptrvalue;
		if(arp->displayOrder == displayOrder)
		{
			if(p_list == NULL)
				list = curr;
			else
				p_list->next = curr;
			p_list = curr;
			curr->next = NULL;
			if(prev == NULL)
				*arp_list = next;
			else
				prev->next = next;
		}
		else
			prev = curr;
		curr = next;
	}

	return list;
}



static Boolean layout_alignment_view(ValNodePtr arp_list, 
	Int4 bsp_len, Int4 left, Int4 right, Boolean align_has_status)
{
	ValNodePtr curr, prev, next;
	AlignRegionPtr arp, n_arp;

	if(arp_list == NULL)
		return FALSE;
	curr = arp_list;
	while(curr)
	{
		arp = curr->data.ptrvalue;
		next = curr->next;
		prev = curr;
		while(next)
		{
			n_arp = next->data.ptrvalue;
			if(n_arp->displayOrder == arp->displayOrder)
			{
				prev = next;
				next = next->next;
			}
			else
				break;
		}

		prev->next = NULL;
		layout_one_alignment_view(curr, bsp_len, 
			left, right, align_has_status);
		prev->next = next;
		curr = next;
	}

	return TRUE;
}



/****************************************************************
*
*	There are two cases that the alignments and repeats
*	need to be displayed together with the sequences in the 
*	global view
*	1) single sequence, show both repeats and alignments
*	2) multiple maps with FISH mapping data, show the alignments 
*	   for the FISH mapping. No repeats
*
****************************************************************/
static GlobalBspPtr find_bsp_for_repeat_align(ValNodePtr gbp_list, BoolPtr show_repeats)
{
	GlobalBspPtr gbp;

	*show_repeats = FALSE;
	if(gbp_list == NULL)
		return NULL;
	if(gbp_list->next == NULL)
	{ /*single sequence*/
		gbp = gbp_list->data.ptrvalue;
		if(gbp->bsp == NULL)
			return NULL;
		*show_repeats = TRUE;
		return gbp;
	}
	else
	{	/*check for the FISH alignment*/
		while(gbp_list != NULL)
		{
			gbp = gbp_list->data.ptrvalue;
			if(gbp->bsp)
			{
				if (get_FISH_align (gbp->bsp))
				{	
					gbp->has_fish_align = TRUE;	
					return gbp;
				}
			}
			gbp_list = gbp_list->next;
		}

	}
	return NULL;
}


/***************************************************************************
*
*	CreateGlobalDrawData(msp_list, ext_align, user_list, need_free)
*	Create the GlobalDrawPtr structure to draw the global view
*	msp_list: externally loaded Seq-entry and its file name (used for saving)
*	ext_align: the externally loaded sequence-alignment for equiv maps
*	user_list: a list of user-selected gene symbols
*	need_free: if TRUE, the free callback will free msp_list, ext_align and 
*	user_list.
*	if(for_picture), it will create the stuff for drawing the global view
*	otherwise, landmakrs, repeats, etc will not be created
*
****************************************************************************/		
GlobalDrawPtr CreateGlobalDrawData(ValNodePtr msp_list, ValNodePtr ext_align, ValNodePtr user_list, Boolean need_free, Boolean for_picture, Uint1 equiv_align_option)
{
	ValNodePtr bsp_list = NULL;
	ValNodePtr e_align = NULL;
	MuskSepPtr msp;
	ValNodePtr vnp, new;
	ValNodePtr landmarks = NULL;
	Boolean show_map_unit;
	Boolean check_cyto;
	Uint1 bsptype = 0;
	BioseqPtr bsp = NULL;
	ValNodePtr global_bsp_data = NULL;
	GeneDataPtr gdata_p = NULL;
	Boolean is_circle = FALSE;
	GlobalDrawPtr gdraw_p;
	GlobalBspPtr gbp;
	SeqLocPtr m_loc;
	Int4 min_inconsist_sp;	/*mininal space between the inconsistent marks*/
	Boolean show_repeats;


	if(msp_list == NULL)
		return NULL;
		
	/*get all the Bioseqs and equiv-aligns in Seq-entry*/
	for(vnp = msp_list; vnp != NULL; vnp = vnp->next)
	{
		msp = (MuskSepPtr)(vnp->data.ptrvalue);
		make_Bioseq_list (msp->sep, &bsp_list, &e_align);
	}
	if(bsp_list == NULL)
		return NULL;
	if(ext_align != NULL)
		ValNodeLink(&e_align, ext_align);
 
		
	/*load the landmark genes*/
	min_inconsist_sp = -1;
	if(for_picture)
	{
		for(vnp = msp_list; vnp != NULL; vnp = vnp->next)
		{
			msp = (MuskSepPtr)(vnp->data.ptrvalue);
			new = LoadLandMarkGene(msp->sep);
			ValNodeLink(&landmarks, new);
		}
		if(user_list != NULL)	/*link the user-supplied list of landmarks to the end*/
			ValNodeLink(&landmarks, user_list);
		if(landmarks != NULL)
		{
			gdata_p = create_gene_data (msp_list, landmarks);
			/*set the minimal space required for displaying the inconsistent marks*/
			if(bsp_list != NULL && equiv_align_option == EQUIV_ALIGN_DEFAULT)
			{
				bsp = (BioseqPtr)(bsp_list->data.ptrvalue);
				min_inconsist_sp = (bsp->length/5);
			}
			else
				min_inconsist_sp = -1;

			collect_equiv_align_for_landmark(gdata_p, e_align, min_inconsist_sp);
			if(user_list != NULL)
				separate_vnp_list(&landmarks, user_list);
			if(landmarks != NULL)
				ValNodeFreeData(landmarks);
		}
	}

	
	/*load data for individual Bioseq*/
	for(vnp = bsp_list; vnp != NULL; vnp = vnp->next)
	{
		bsp = (BioseqPtr)(vnp->data.ptrvalue);
		bsptype = get_Bioseq_type (bsp);
		switch(bsptype)
		{
			case RESTRICTION_MAP:
				show_map_unit = FALSE;
				check_cyto = FALSE;
				break;
			case CYTO_MAP:
				show_map_unit = FALSE;
				check_cyto = TRUE;
				break;
			default:
				show_map_unit = TRUE;
				check_cyto = FALSE;
				break;
		}
		if(GlobalDataNew(bsp, bsptype, show_map_unit, check_cyto, gdata_p, &global_bsp_data, (Uint2)(vnp->choice)))
			is_circle = TRUE;
	}
	ValNodeFree(bsp_list);
	
	gdraw_p = MemNew(sizeof(GlobalDraw));
	gdraw_p->gbp_list = global_bsp_data;
	gdraw_p->ext_align = ext_align;
	gdraw_p->equiv_align_list = e_align;
	gdraw_p->msp_list = msp_list;
	gdraw_p->free_data = need_free;
	gdraw_p->g_data = gdata_p;
	gdraw_p->g_list = user_list;
	gdraw_p->is_circle = is_circle;
	
	/*collect the repeats and alignments for global display */
	if(!is_circle && for_picture)
	{
		gbp = find_bsp_for_repeat_align(gdraw_p->gbp_list, &show_repeats);
		if(gbp != NULL)
		{
			m_loc = SeqLocIntNew(0, gbp->bsp->length-1, Seq_strand_plus, SeqIdFindBest(gbp->bsp->id, 0));
			if(show_repeats)
				collect_repeats_and_align (m_loc, &(gbp->rrp_list),  &(gbp->arp_list), 0, msp->sep, &(gbp->align_has_status));
			else
				collect_repeats_and_align (m_loc, NULL,  &(gbp->arp_list), 0, msp->sep, &(gbp->align_has_status));
			SeqLocFree(m_loc);
		}
	}

	return gdraw_p;

}


/***************************************************************************
*
*	functions related to the layout of GlobalDrawPtr
*
*	LayoutGlobalDrawData(gdraw_p, pan_width, pan_height)
*	gdraw_p: the GlobalDrawPtr
*	pan_width: the width of the viewer
*	pan_height: the height of the viewer
*
****************************************************************************/		

static Int4 get_max_len(ValNodePtr bsp_data)
{
	GlobalBspPtr gbp;
	Int4 len = 0;

	while(bsp_data)
	{
		gbp = (GlobalBspPtr)(bsp_data->data.ptrvalue);
		if(gbp && gbp->hide == FALSE)
			len = MAX(len, gbp->bsp->length);
		bsp_data = bsp_data->next;
	}
	return len;
}

static Int2 get_curr_right(Int4 bsp_len, Int4 max_len, Int2 left, Int2 right)
{
	FloatHi tmp_1;

	if(bsp_len == max_len)
		return right;
	tmp_1 = (FloatHi)(bsp_len)/(FloatHi)(max_len);
	tmp_1 *= (FloatHi)(right - left);
	
	return (left + (Int2)tmp_1);
}

static Int4 get_max_label_len(ValNodePtr gbp_list)
{
	Uint4 max_len = 0;
	CharPtr seq_label = NULL;
	GlobalBspPtr gbp;
	FonT font;
  	Char label[21];
  	Uint1 format;
  	Int2 label_size;

	font = (FonT)GetMuskCParam(MSM_SEQUENCE, MSM_SLABEL, MSM_FONT);
	SelectFont(font);

	format = (Uint1)GetMuskCParam(MSM_SEQUENCE, MSM_SLABEL, MSM_STYLE);
	label_size = MIN(20, (Int2)GetMuskCParam(MSM_TOPSTYLE, MSM_LABEL, MSM_NUM));
	while(gbp_list)
	{
		gbp = gbp_list->data.ptrvalue;
		if(gbp && gbp->hide == FALSE )
		{
			if(gbp->label != NULL)
				gbp->label = MemFree(gbp->label);
			if(MuskSeqIdWrite (gbp->bsp->id, label, label_size, format, TRUE, FALSE))
			{
				gbp->label = StringSave(label);
				if(StringLen(gbp->label) > max_len)
				{
					seq_label = gbp->label;
					max_len = StringLen(gbp->label);
				}
			}
		}
		gbp_list = gbp_list->next;
	}
	if(seq_label != NULL)
		return StringWidth(seq_label);
	else
		return 0;
}


/* space for the Font plus the length of the tick mark. That is the 
   immediate lines after the sequence */
static Int4 get_minimal_spacing(void)
{
	FonT font;
	Int4 font_height;

	font = (FonT)GetMuskCParam(MSM_SEQUENCE, MSM_SLABEL, MSM_FONT);
	SelectFont(font);
	font_height= FontHeight();
	
	return (font_height + TICK_LEN);
}

	
/***********************************************************************
*
*	calculate the maximum vertical space needed for displaying the 
*	alignment globally (for powblast application)
*
***********************************************************************/
static Int4 get_max_alignment_yspace(ValNodePtr arp_list, Boolean is_fish_align)
{
	Int4 line_space = 6;
	AlignRegionPtr arp;
	Int4 max_space = 0, c_space;
	Int4 pen_width;
	Int4 displayOrder;
	Int4 groupNum, group_space;
	Boolean has_prev;
	FonT font;
	Int4 font_height;

	displayOrder = 0;
	has_prev = FALSE;
	c_space = 0;
	groupNum = 0;
	while(arp_list)
	{
		arp = arp_list->data.ptrvalue;
		if(has_prev == FALSE || arp->displayOrder != displayOrder)
		{
			max_space += c_space;
			c_space = 0;
			displayOrder = arp->displayOrder;
			has_prev = TRUE;
			++groupNum;
		}
		c_space = MAX(c_space, arp->line);
		arp_list = arp_list->next;
	}
	max_space += c_space;
	if(is_fish_align)
	{
		pen_width = 1;
		++max_space;
		return ((max_space +1) * line_space*pen_width);
	}
	else
	{
		if(groupNum > 1)
		{
			font = (FonT)GetMuskCParam(MSM_SEQUENCE, MSM_SLABEL, MSM_FONT);
			SelectFont(font);
			font_height = FontHeight();
			group_space = font_height * groupNum + 2 * groupNum * line_space;
		}
		else
			group_space = 0;
		pen_width = MAX(1, GetMuskCParam(MSM_SEQUENCE, MSM_SEG_BORD, MSM_PENWIDTH));
		return ((max_space +1) * line_space*pen_width + group_space);
	}
}

static Int4 get_legend_space (void)
{
	Int4 font_height;
	FonT font;

	font = (FonT)GetMuskCParam(MSM_SEQUENCE, MSM_SLABEL, MSM_FONT);
	SelectFont(font);
	font_height = FontHeight();
	return (2*font_height + 2*GetMuskCParam(MSM_TOPSTYLE, MSM_SPACE, MSM_HEIGHT));
}


/*
*	show_align_legend: if TRUE, draw a colored bar representing the 
*	status of the alignemnts
*/
static Boolean LayoutLinearDrawing(ValNodePtr g_bsp_list, Int2 pan_width, Int2 pan_height, Boolean show_align_legend)
{
	Int2 space; 
	Int2 extra_space = 0;
	Int2 top, bottom, left, right, c_right;
	GlobalBspPtr gbp;
	ValNodePtr curr;
	Int2 num, lod_num;
	Int4 maxlen, max_label_len;
	Int2 box_height;
	Int2 maxHeight;
	Uint1 label_align;
	Int2 start_height =0;
	Int2 align_space;
	Int4 legend_space;
	Boolean align_has_status;

	

	/*calculate the number of sequences*/
	num = 0;
	lod_num = 0;
	for(curr = g_bsp_list; curr != NULL; curr = curr->next)
	{
		gbp = curr->data.ptrvalue;
		if(gbp->hide == FALSE)
		{
			++num;
			lod_num += (gbp->num_lod_score);
		}
	}
	if(num == 0)
		return FALSE;
	/*figure out the left and right margin*/
	label_align = (Uint1)GetMuskCParam(MSM_TOPSTYLE, MSM_LABEL, MSM_STYLE);
	max_label_len = get_max_label_len(g_bsp_list);
	if(label_align == MSM_LABEL_RIGHT)
	{
		left = max_label_len/2;
		right = pan_width - (Int2)max_label_len;
	}
	else
	{
		left = (Int2)max_label_len;
		right = pan_width -max_label_len/2;
	}
	if(right - left < MIN_WIDTH)
		return FALSE;
	maxlen = get_max_len(g_bsp_list);

	/*layout the alignemnts. Figure out the maximum space 
		required for the alignments*/
	align_space = 0;
	align_has_status = FALSE;
	for(curr = g_bsp_list; curr != NULL; curr = curr->next)
	{
		gbp = curr->data.ptrvalue;
		if(gbp->hide == FALSE)
		{
			c_right = get_curr_right(gbp->bsp->length, maxlen, left, right);
			gbp->rec.left = left;
			gbp->rec.right = c_right;
			if(gbp->arp_list != NULL)
			{
				if(gbp->has_fish_align)
					align_has_status = FALSE;
				else
					align_has_status = gbp->align_has_status;
				layout_alignment_view(gbp->arp_list,
					gbp->bsp->length, left, c_right, align_has_status);
				align_space += (Int2)get_max_alignment_yspace(gbp->arp_list, gbp->has_fish_align);
			}
		}
	}

	if(show_align_legend && align_has_status)
	{
		legend_space = get_legend_space ();
	}
	else
		legend_space = 0;

	extra_space = (Int2)GetMuskCParam(MSM_TOPSTYLE, MSM_SPACE, MSM_HEIGHT);
	space = (Int2)get_minimal_spacing();


	box_height = (Int2)GetMuskCParam(MSM_SEQUENCE, MSM_SEGMENT, MSM_HEIGHT);
	if(box_height < 6)
		box_height = 6;
	maxHeight = num*( box_height + space * 2) + (num -1) * extra_space + align_space + legend_space + lod_num * (box_height + extra_space * 2);
	if(legend_space > 0 || g_bsp_list->next == NULL || pan_height <= maxHeight)
		start_height = 0;
	else
		start_height = (pan_height - maxHeight)/(num + 1);

	top = (Int2)legend_space;
	for(curr = g_bsp_list; curr != NULL; curr = curr->next)
	{
		gbp = curr->data.ptrvalue;
		if(gbp->hide == FALSE)
		{
			top += (space + start_height);
			if(gbp->arp_list != NULL)
				align_space = (Int2)get_max_alignment_yspace(gbp->arp_list, gbp->has_fish_align);
			else
				align_space = 0;

			bottom = top + box_height;
			gbp->rec.top = top;
			gbp->rec.bottom = bottom;
			top = bottom + space + extra_space + align_space;

			if(gbp->num_lod_score > 0)
			{
				top += ((box_height + extra_space * 2)*gbp->num_lod_score);
			}
		}
	}

	return TRUE;
}

static Int2 get_circle_max_label(Int2 num)
{
	Char label[21];
	Int2 i;
	FonT font;

	font = (FonT)GetMuskCParam(MSM_SEQUENCE, MSM_SLABEL, MSM_FONT);
	SelectFont(font);
	num = MIN(20, num);
	for(i = 0; i<num; ++i)
		label[i] = 'X';
	label[i] = '\0';
	return (Int2)StringWidth(label);
}
		
			
static Boolean LayoutCircleDrawing(ValNodePtr g_bsp_list, Int2 pan_width, Int2 pan_height)
{
	Int2 space, extra_space, max_space;
	Int2 radius;
	Int2 max_radius, min_radius;
	Int2 max_label_width;
	GlobalBspPtr gbp;
	Int2 num;
	ValNodePtr curr;

	num = 0;
	for(curr = g_bsp_list; curr != NULL; curr = curr->next)
	{
		gbp = curr->data.ptrvalue;
		if(gbp->hide == FALSE)
			++num;
	}
	if( num == 0)
		return FALSE;
		
	max_label_width = get_circle_max_label(10);
	if(pan_width - max_label_width > pan_height)
	{
		pan_width = pan_height;
		max_radius = pan_height/2;
	}
	else
	{
		pan_width -= max_label_width;
		max_radius = pan_width/2;
	}
	if(pan_width < MIN_WIDTH)
		return FALSE;
	space = max_label_width/2 + TICK_LEN;
	extra_space = (Int2)GetMuskCParam(MSM_TOPSTYLE, MSM_SPACE, MSM_HEIGHT);


	max_space = 0;
	for(curr = g_bsp_list; curr != NULL; curr = curr->next)
	{
		gbp = curr->data.ptrvalue;
		if(gbp->hide == FALSE)
		{
			if(gbp->show_map_unit)
				max_space += space *2;
			max_space += extra_space;
		}
	}
	if(max_space != 0)	/*get rid of the last extra_space*/
		max_space -= extra_space;
	min_radius = max_radius - max_space;
	radius = MAX(min_radius, 20);

	
	for(curr = g_bsp_list; curr != NULL; curr = curr->next)
	{
		gbp = curr->data.ptrvalue;
		if(gbp->hide == FALSE)
		{
			if(gbp->show_map_unit)
				radius += space;
			gbp->radius = radius;
			if(gbp->show_map_unit)
				radius += space;
			radius += extra_space;
		}
	}

	return TRUE;
}

			
Boolean LayoutGlobalDrawData(GlobalDrawPtr gdraw_p, Int2 pan_width, Int2 pan_height)
{

	if(gdraw_p == NULL)
		return FALSE;
	if(gdraw_p->is_circle)
	{
		if(!LayoutCircleDrawing(gdraw_p->gbp_list, pan_width, pan_height))
			return FALSE;
		gdraw_p->center.x = pan_width/2;
		gdraw_p->center.y = pan_height/2;
	}
	else
		return LayoutLinearDrawing(gdraw_p->gbp_list, pan_width, pan_height, gdraw_p->draw_align_legend);

	return TRUE;
}


	

/************************************************************************
*
*   Draw_Global_Linear(bsp, show_map_unit, marks, check_cyto, p, rec)
*	draw the global view of a linear molecule
*	bsp: the map Bioseq
*	show_map_unit:	label the map unit
*	marks:	label any marks, such as genes associated with the map
*	check_cyto:	check if it is a cytogenetic map
*	p:	the PaneL for drawing
*	rec:	the Rectangle that contains the map Bioseq
*
************************************************************************/
static void draw_linear_seqmark(SegmenT seg, Int4 length, Int4 left, Int4 top, Int4 right, Int4 bottom, NumberingPtr np)
{
  Int4 scale, ruler;
  Int4 i;
  Int4 pos, x_pos;

  Char str[40];
  Int4 tick_len;
  Boolean use_kb;
  FonT font;


	scale = length/(right - left);
	if (length%(right - left) != 0)
		++scale;
	ruler = calculate_ruler(scale)/10;
	use_kb = (ruler >= 200);
	AddAttribute (seg, COLOR_ATT, BLACK_COLOR, 0, 0, 0, 0);
	font = (FonT)GetMuskCParam(MSM_SEQUENCE, MSM_SLABEL, MSM_FONT);

	i=0;
	pos = 0;
	while(pos < length)
	{				/**mark the scales**/
		tick_len = (i%5==0) ? TICK_LEN: TICK_LEN/2;
		x_pos = map_linear_xpos(pos, length, left, right);
		AddLine(seg, x_pos, bottom, x_pos, (bottom - tick_len), FALSE, 0);
		if(i%5 ==0)	/*label the the interval*/
		{
			map_unit_label(pos, np, str, use_kb);
			AddTextLabel(seg, x_pos, (bottom - tick_len), str, font, 0, LOWER_CENTER, 0);
		}

		++i;
		pos += ruler;
	}
}


static Boolean label_linear_mark(SegmenT seg, Int4 seqlen, ValNodePtr marks, Int4 left, Int4 top, Int4 right, Int4 bottom, Boolean ck_overlap)
{
	FonT font;
	MapMarkPtr mmp;
	Int4 tick_len = TICK_LEN;
	Int4 x_pos, p_pos;
	Boolean drawit;
	Int4 pwidth = -1, width;

	font = (FonT)GetMuskCParam(MSM_SEQUENCE, MSM_SLABEL, MSM_FONT);
	SelectFont(font);
	p_pos = -1;
	while(marks)
	{
		mmp = marks->data.ptrvalue;
		x_pos = map_linear_xpos (mmp->pos, seqlen, left, right);
		drawit = TRUE;
		width = (Int2)StringWidth(mmp->label);
		if(p_pos != -1 && ck_overlap && pwidth != -1)
			drawit = ((x_pos - p_pos -2) > (pwidth + width)/2);
		if(drawit)
		{
			if(mmp->inward)
			{
				AddLine(seg, x_pos, bottom, x_pos, (bottom - TICK_LEN), FALSE, 0);
				AddTextLabel(seg, x_pos, (bottom - TICK_LEN), mmp->label, font, 0, LOWER_CENTER, 0);
			}
			else
			{
				AddLine(seg, x_pos, top, x_pos, (top + TICK_LEN), FALSE, 0);
				AddTextLabel(seg, x_pos, (top + TICK_LEN), mmp->label, font, 0, UPPER_CENTER, 0);
			}
			pwidth = width;
			p_pos = x_pos;
		}

		marks = marks->next;
	}

	return TRUE;
}



static void DrawInterval(Int4 start, Int4 stop, Int4 length, Int4 left, Int4 right, Int4 top, Int4 bottom, Boolean fill, SegmenT seg)
{
	Int4 c_left, c_right;
	
	c_left = map_linear_xpos (start, length, left, right);
	c_right = map_linear_xpos (stop, length, left, right);
	AddRectangle(seg, c_left, top, c_right, bottom, NO_ARROW, fill, 0);
}	



static Boolean draw_human_cyto_map(BioseqPtr bsp, Int4 left, Int4 top, Int4 right, Int4 bottom, SegmenT seg)
{
	SeqFeatPtr sfp;
	UserObjectPtr uop;
	Int4 m_start, m_stop, length;
	Int2 i = 0;
	Uint1 band;
	Boolean draw_band;
	Uint1Ptr color;
	Uint1 shading;


	if(bsp->repr != Seq_repr_map)
		return FALSE;
	sfp = bsp->seq_ext;
	if(sfp->data.choice != 14)
		return FALSE;

	length = bsp->length;
	for(sfp = bsp->seq_ext; sfp !=NULL; sfp = sfp->next)
	{
		++i;
		if(sfp->data.choice == 14)
		{
			uop = sfp->data.value.ptrvalue;
			band  = get_band_type(uop);
			draw_band = TRUE;
			switch(band)
			{
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
				default:
					draw_band = FALSE;
					break;
			}
			if(draw_band)
			{
				m_start = SeqLocStart(sfp->location);
				m_stop = SeqLocStop(sfp->location);
				AddAttribute (seg, SHADING_ATT | COLOR_ATT, color, 0, shading, 0, NO_MODE);
				DrawInterval(m_start, m_stop, length, left, right, top, bottom, TRUE, seg);
			}
		}
	}

	return TRUE;
}

static Boolean draw_fly_cyto_map(BioseqPtr bsp, Int4 left, Int4 top, Int4 right, Int4 bottom, SegmenT seg)
{
	SeqFeatPtr sfp;
	UserObjectPtr uop;
	UserFieldPtr ufp;
	Int4 m_start, m_stop, length;
	Char subdiv[100];
	CharPtr curdiv;
	Boolean newseg;
	Uint1 band_type, prev_band;
	Int4 div =0;
	Boolean has_prev;
	Uint1Ptr color;
	Uint1 shading;
	Int2 num_subdiv = 0;


	if(bsp->repr != Seq_repr_map)
		return FALSE;
	sfp = bsp->seq_ext;
	if(sfp->data.choice != 14)
		return FALSE;

	length = bsp->length;
	subdiv[0] = '\0';
	has_prev = FALSE;
	div = 0;
	for(sfp = bsp->seq_ext; sfp !=NULL; sfp = sfp->next)
	{
		uop = sfp->data.value.ptrvalue;
		band_type = get_band_type(uop);
		if(band_type ==TEL || band_type ==CEN)
		{
			if(has_prev)
				DrawInterval(m_start, m_stop, length, left, right, top, bottom, TRUE, seg);
			AddAttribute (seg, SHADING_ATT | COLOR_ATT, RED_COLOR, NO_LINE_STYLE, SOLID_SHADING, 0, NO_MODE);
			m_start = SeqLocStart(sfp->location);
			m_stop = SeqLocStop(sfp->location);
			DrawInterval(m_start, m_stop, length, left, right, top, bottom, TRUE, seg);
			has_prev = FALSE;
			subdiv[0] = '\0';
		}
		else
		{
			if(has_prev)
				newseg = (prev_band != band_type);
			else
				newseg = TRUE;
				
			for(ufp = uop->data; ufp!=NULL; ufp = ufp->next)
			{
				if(is_label_match(ufp->label, "Subdivision"))
				{
					curdiv = ufp->data.ptrvalue;
					if(StringCmp(subdiv, curdiv) !=0)
						newseg = TRUE;
					if(newseg)
						StringCpy(subdiv, curdiv);
				}
				if(is_label_match(ufp->label, "Division"))
					div = ufp->data.intvalue;
			}
			if(newseg)
			{
				if(has_prev)
					DrawInterval(m_start, m_stop, length, left, right, top, bottom, TRUE, seg);
				m_start = SeqLocStart(sfp->location);
				m_stop = SeqLocStop(sfp->location);
				has_prev = TRUE;
				prev_band = band_type;
				if(band_type == BND)
				{
					if(num_subdiv%2 == 0)	
						shading = THIN_NESW_SHADING;
					else
						shading = THIN_NWSE_SHADING;
					color = get_seg_color((Int2)div);
					++num_subdiv;
				}
				else	/*for herterogen*/
				{
					shading = THIN_HORIZ_SHADING;
					color = BLACK_COLOR;
				}
				AddAttribute (seg, SHADING_ATT | COLOR_ATT, color, 0, shading, 0, NO_MODE);
			}
			else
				m_stop = SeqLocStop(sfp->location);
		}
	}
	if(has_prev)
		DrawInterval(m_start, m_stop, length, left, right, top, bottom, TRUE, seg);
	return TRUE;
}
		

static Boolean draw_cyto_map(BioseqPtr bsp, Int4 left, Int4 top, Int4 right, Int4 bottom, SegmenT seg)
{
	SeqFeatPtr sfp;
	Uint1 cyto_type;

	if(bsp->repr == Seq_repr_map)
	{
		sfp = (SeqFeatPtr)(bsp->seq_ext);
		cyto_type = ck_cyto_type(sfp);
		switch(cyto_type)
		{
			case HUMAN_CYTO:
				draw_human_cyto_map(bsp, left, top, right, bottom, seg);
				break;
			case FLY_CYTO:
				draw_fly_cyto_map(bsp, left, top, right, bottom, seg);
				break;
			default:
				return FALSE;
		}
		add_attribute_pen(seg, MSM_SEQUENCE, MSM_SEG_BORD);
		AddRectangle(seg, left, top, right, bottom, FALSE, FALSE, 0);
		return TRUE;
	}

	return FALSE;
}


/**********************************************************
*
*	add_alignment_to_global: add the arp_list to the 
*	global drawing
*	bottom: the current y position of the picture
*	add_to_bottom: TRUE, add the alignment to the bottom 
*	of the picture, such as the regular sequence alignment
*	FALSE: add to the top of the picture. ONLY for the FISH 
*	mapping data
*	return the next available y position
*
***********************************************************/
static Int4 add_alignment_to_global(SegmenT seg, ValNodePtr arp_list, 
	Int4 bottom, Boolean add_to_bottom, Int4 left, Int4 right, Int4 length)
{
	Uint1 p_val;
	Int4 extra_space;
	Int4 line_count, maxline;
 	Uint1 displayOrder;
	Int2 order;
	Boolean has_prev;	
	FonT font;
	Int4 font_height;
	Int4 g_top, g_bottom;
	CharPtr annotDB;
	Int4 m_start, m_stop, center;
	Int4 h_bottom;
	AlignRegionPtr arp;
	Int4 y_pos;
	Int4 pen_width;
	Int4 c_left, c_right;
	ValNodePtr intervals;
	GatherRangePtr grp;
	Uint1Ptr color;

	if(arp_list == NULL)
		return bottom;

	extra_space = GetMuskCParam(MSM_TOPSTYLE, MSM_SPACE, MSM_HEIGHT);
	h_bottom = bottom;
	if(add_to_bottom)
		bottom -= (extra_space + get_minimal_spacing());
	font = (FonT)GetMuskCParam(MSM_SEQUENCE, MSM_SLABEL, MSM_FONT);
	SelectFont(font);
	font_height = FontHeight();
	if(add_to_bottom)
		pen_width = MAX(1, GetMuskCParam(MSM_SEQUENCE, MSM_SEG_BORD, MSM_PENWIDTH));
	else
		pen_width = 1;
		
	p_val = 1;
	color = BLACK_COLOR;
	displayOrder = 0;
	has_prev = FALSE;
	order = 0;
	maxline = 0;
	line_count = 0;

	AddAttribute (seg, COLOR_ATT|STYLE_ATT|SHADING_ATT|WIDTH_ATT, BLACK_COLOR, SOLID_LINE, SOLID_SHADING, 1, COPY_MODE); 
	while(arp_list)
	{
		arp = arp_list->data.ptrvalue;
		if(has_prev == FALSE || arp->displayOrder != displayOrder)
		{	/*start of a new Seq-align cluster */
			displayOrder = arp->displayOrder;
			if(has_prev && add_to_bottom)
			{	/*add the AlignNode box to the previous box*/
				g_bottom = bottom - (maxline + line_count)* (6+pen_width-1) - 8;
				color = get_seg_color(order);
				AddAttribute (seg, COLOR_ATT|STYLE_ATT|SHADING_ATT|WIDTH_ATT, color, SOLID_LINE, SOLID_SHADING, 1, COPY_MODE); 
				AddRectangle (seg, left, g_top, right, g_bottom, FALSE, FALSE, 0);
				if(annotDB[0] != 0)
				{
					AddAttribute (seg, COLOR_ATT|STYLE_ATT|SHADING_ATT|WIDTH_ATT, WHITE_COLOR, SOLID_LINE, SOLID_SHADING, 1, COPY_MODE); 
				
					c_left = (left + right)/2 - StringWidth(annotDB)/2 -10;
					c_right = c_left + StringWidth(annotDB) + 10;
					AddRectangle (seg, c_left, g_top+font_height/2, c_right, g_top-font_height/2, FALSE, TRUE, 0);
					
					
					AddAttribute (seg, COLOR_ATT, color, 0, 0, 0, COPY_MODE); 
					AddTextLabel(seg, (left+right)/2, g_top, annotDB, font, 0, MIDDLE_CENTER, 0);
				}
				p_val = 0;
				bottom -= 16;
				line_count += maxline;
			}
			has_prev = TRUE;
			annotDB = arp->annotDB;
			++order;
			if(add_to_bottom)
			{
				/* add_attribute_pen(seg, MSM_SEQUENCE, MSM_SEG_BORD); */
				g_top = bottom - line_count * (6 + pen_width-1);
				if(annotDB[0] != '\0')
				{
					g_top -= font_height/2;
					bottom -= font_height;
				}
				bottom -= 8;
			}
			else
			{
				AddAttribute (seg, COLOR_ATT|STYLE_ATT|SHADING_ATT|WIDTH_ATT, BLACK_COLOR, SOLID_LINE, SOLID_SHADING, 1, NO_MODE); 
				bottom -= 6;
			}
			maxline = 0;

		}
		maxline = MAX(maxline, arp->line);


		switch(arp->status)
		{
			case 0:
				color = BLACK_COLOR;
				break;
			case 1:
				color = BLUE_COLOR;
				break;
			case 2:
				color = GREEN_COLOR;
				break;
			case 3:
				color = MAGENTA_COLOR;
				break;
			default:
				color = RED_COLOR;
				break;
		}
		m_start = arp->g_left; 
		m_stop = arp->g_right;

		/* m_start = arp->gr.left;
		m_stop = arp->gr.right;
		m_start = map_linear_xpos (m_start, length, left, right);
		m_stop = map_linear_xpos (m_stop, length, left, right); */
		y_pos = bottom - (arp->line + line_count) *(6 + pen_width-1);
		if(add_to_bottom)
		{	/*adding the info for the image maps*/
			arp->g_left = m_start;
			arp->g_right = m_stop;
			arp->g_top = y_pos;
			arp->g_bottom = arp->g_top - pen_width;
		}
		/*the sequence hits more than one region*/
		intervals = arp->intervals;
		if(add_to_bottom && (intervals != NULL && intervals->next != NULL))
		{
			if(pen_width == 1)
			{
				AddAttribute(seg, COLOR_ATT|SHADING_ATT|MODE_ATT, YELLOW_COLOR, 0, DARK_SHADING, 0, COPY_MODE);
				AddLine(seg, m_start, y_pos, m_stop, y_pos, 0, FALSE);
			}
			else
			{
				AddAttribute(seg, COLOR_ATT|SHADING_ATT|MODE_ATT, BLACK_COLOR, 0, THICK_NESW_SHADING, 0, COPY_MODE);
				AddRectangle (seg, m_start, y_pos, m_stop, y_pos-pen_width, FALSE, TRUE, 0);
			}
		}

		AddAttribute(seg, COLOR_ATT|STYLE_ATT|SHADING_ATT|MODE_ATT, color, SOLID_LINE , SOLID_SHADING, 0, COPY_MODE);
		while(intervals)
		{
			grp = intervals->data.ptrvalue;
			m_start = map_linear_xpos(grp->left, length, left, right);
			m_stop = map_linear_xpos(grp->right, length, left, right);
			if(pen_width == 1 || !add_to_bottom)
				AddLine(seg, m_start, y_pos, m_stop, y_pos, 0, FALSE);
			else
				AddRectangle (seg, m_start, y_pos, m_stop, y_pos-pen_width, FALSE, TRUE, 0);
			intervals = intervals->next;
		}


			
		if(add_to_bottom == FALSE && y_pos != h_bottom)
		{
			/*add the lines to the FISH mapping */
			center = (m_start + m_stop)/2;
			AddLine(seg, center, y_pos, center, h_bottom, 0, FALSE);
		}
		/* if(p_val > 1)
			AddLine(seg, m_start, y_pos+1, m_stop, y_pos+1, 0, FALSE); */
		arp_list = arp_list->next;
	}

	if(order > 1 && add_to_bottom)
	{
		g_bottom = bottom - extra_space - (maxline + line_count)* (6 + pen_width - 1) -8;
		color = get_seg_color(order);
		AddAttribute (seg, COLOR_ATT|STYLE_ATT|SHADING_ATT|WIDTH_ATT, color, SOLID_LINE, SOLID_SHADING, 1, COPY_MODE); 
		AddRectangle (seg, left, g_top, right, g_bottom, FALSE, FALSE, 0);
		if(annotDB[0] != 0)
		{
			AddAttribute (seg, COLOR_ATT|STYLE_ATT|SHADING_ATT|WIDTH_ATT, WHITE_COLOR, SOLID_LINE, SOLID_SHADING, 1, COPY_MODE); 
		
			c_left = (left + right)/2 - StringWidth(annotDB)/2 -10;
			c_right = c_left + StringWidth(annotDB) + 10;
			AddRectangle (seg, c_left, g_top+font_height/2, c_right, g_top-font_height/2, FALSE, TRUE, 0);
					
			AddAttribute (seg, COLOR_ATT, color, 0, 0, 0, COPY_MODE); 
			AddTextLabel(seg, (left+right)/2, g_top, annotDB, font, 0, MIDDLE_CENTER, 0);
		}
		return g_bottom;
	}
	else
		return (bottom - (maxline + line_count +1) * 6 );

}

static Boolean get_print_score(FloatHi min_score, FloatHi max_score, CharPtr label, Uint1 status)
{
	FloatHi val;

	if(min_score == max_score)
		return FALSE;
	if(status > 4)
		return FALSE;

	switch(status)
	{
		case 0:
			val = min_score;
			break;
		case 4:
			val = max_score;
			break;
		default:
			val = min_score + (FloatHi)status * (max_score - min_score)/5.0;
			break;
	}

	sprintf(label, "%.1lf", val);
	return TRUE;
}



static void DrawAlignScoreLegend(SegmenT seg, Int4 left, Int4 right, FloatHi min_score, FloatHi max_score)
{
	FonT font;
	Int4 font_height, t_right;
	Int4 y_pos, box_width, box_height;
	Char buf[101], label[101];
	Int4 width;
	Int2 i;
	Int4 extra_space;

	font = (FonT)GetMuskCParam(MSM_SEQUENCE, MSM_SLABEL, MSM_FONT);
	SelectFont(font);
	font_height = FontHeight();
	/* box_height = (Int2)GetMuskCParam(MSM_SEQUENCE, MSM_SEGMENT, MSM_HEIGHT);
	box_height = MIN(font_height, box_height); */
	box_height = font_height;
	extra_space = GetMuskCParam(MSM_TOPSTYLE, MSM_SPACE, MSM_HEIGHT);
	t_right = right;

	StringCpy(buf, "Color Key for Alignment Scores");
	AddTextLabel(seg, (left+right)/2, 0, buf, font, 0, LOWER_CENTER, 0);
	y_pos = 0 - font_height - extra_space;
	width = 0;
	left += width/2;
	right -= width/2;
	box_width = (right - left)/5;

	/* StringCpy(buf, "lowest");
	AddAttribute(seg, COLOR_ATT, BLACK_COLOR, 0, 0, 0, 0);
	AddTextLabel(seg, left, y_pos-box_height/2, buf, font, 0, MIDDLE_LEFT, 0); */

	for(i = 0; i<5; ++i)
	{
		right = left + box_width -1;
		switch(i)
		{
			case 0:
				AddAttribute(seg, COLOR_ATT, BLACK_COLOR, 0, 0, 0, 0);
				StringCpy(label, "<40");
				break;
			case 1:
				AddAttribute(seg, COLOR_ATT, BLUE_COLOR, 0, 0, 0, 0);
				StringCpy(label, "40-50");
				break;
			case 2:
				StringCpy(label, "50-80");
				AddAttribute(seg, COLOR_ATT, GREEN_COLOR, 0, 0, 0, 0);
				break;
			case 3:
				StringCpy(label, "80-200");
				AddAttribute(seg, COLOR_ATT, MAGENTA_COLOR, 0, 0, 0, 0);
				break;
			default:
				StringCpy(label, ">=200");
				AddAttribute(seg, COLOR_ATT, RED_COLOR, 0, 0, 0, 0);
				break;
		}
		AddRectangle (seg, left, y_pos, right, y_pos-box_height, FALSE, TRUE, 0);
		switch(i)
		{
			case 0:
			case 1:
				AddAttribute(seg, COLOR_ATT, WHITE_COLOR, 0, 0, 0, 0);
				break;
			default:
				AddAttribute(seg, COLOR_ATT, BLACK_COLOR, 0, 0, 0, 0);
				break;
		}
		AddTextLabel(seg, (left+right)/2, y_pos-box_height/2-3, label, font, 0, MIDDLE_CENTER, 0);
		left = right + 1;
	}
	AddAttribute(seg, COLOR_ATT, BLACK_COLOR, 0, 0, 0, 0);
	AddLine(seg, 0, y_pos-font_height-extra_space, t_right, y_pos-font_height-extra_space, 0, FALSE);
}
		
	
static Boolean get_min_max_score (ValNodePtr arp_list, FloatHiPtr min_score, FloatHiPtr max_score)
{
	AlignRegionPtr arp;
	ValNodePtr curr;
	Uint1 displayOrder;
	Boolean has_prev;

	*min_score = -1.0;
	*max_score = -1.0;
	has_prev = FALSE;
	displayOrder = 0;

	for(curr = arp_list; curr != NULL; curr = curr->next)
	{
		arp = curr->data.ptrvalue;
		if(!has_prev)
		{
			displayOrder = arp->displayOrder;
			has_prev = TRUE;
		}
		else if(displayOrder != arp->displayOrder)	/*more than one group*/
		{
			*min_score = -1.0;
			*max_score = -1.0;
			return FALSE;
		}
			
		if(*min_score == -1.0 || *min_score > arp->score)
			*min_score = arp->score;
		if(*max_score == -1.0 || *max_score < arp->score)
			*max_score = arp->score;
	}

	return (*min_score < *max_score);
}


static Uint1Ptr MapLodScoreColor(SeqFeatPtr sfp)
{
	Uint1 bit_val;

	bit_val =  GetLODScoreBitValue(sfp);
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
	
	

static Boolean  Draw_Global_Linear(GlobalBspPtr gbp, SegmenT seg, Boolean draw_align_legend)
{
  BioseqPtr bsp;
  Boolean show_map_unit;
  Boolean check_cyto;
  FonT font;
  Uint1 label_align;
  Int4 middle;
  Uint1Ptr color;

  
  Int4 left, top, right, bottom;
  NumberingPtr np = NULL;
  Int4 length;
  Int2 i;

  SeqLocPtr slp;
  Int4 m_start, m_stop, pos;
  Boolean is_gap = FALSE;
  SeqIdPtr maybe_mapid;	/*a possible id for map*/
  BioseqPtr t_bsp;
  
  /*for adding the repeat regions*/
  Uint1 p_val, shading;
  RepeatRegionPtr rrp;
  AlignRegionPtr arp;
  ValNodePtr curr;
  CharPtr label;
  Int4 seglen;
  Boolean skip;

  ValNodePtr rrp_list;
  FloatHi min_score, max_score;
  ValNodePtr delta_node;
  SeqLitPtr slitp;

  SeqAnnotPtr annot;
  SeqFeatPtr sfp;
  Int4 extra_space, space;


	if(gbp == NULL || gbp->bsp == NULL || gbp->hide)
		return FALSE;
	bsp = gbp->bsp;
	show_map_unit = gbp->show_map_unit;
	check_cyto = gbp->check_cyto;
	left = (Int4)(gbp->rec.left);
	right = (Int4)(gbp->rec.right);
	top = -(Int4)(gbp->rec.top);
	bottom = -(Int4)(gbp->rec.bottom);
	length = bsp->length;

	/*draw the legend for alignment first*/
	if(gbp->has_fish_align == FALSE && gbp->arp_list != NULL)
	{
		if(draw_align_legend)
		{
			for(curr = gbp->arp_list; curr != NULL; curr = curr->next)
			{
				arp = curr->data.ptrvalue;
				if(arp->status > 0)
				{
					/*draw the legend for alignment score*/
					get_min_max_score (gbp->arp_list, &min_score, &max_score);
					DrawAlignScoreLegend(seg, left, right, min_score, max_score);
					break;
				}
			}
		}
	}
		

	if(gbp->has_fish_align && gbp->arp_list != NULL)
	{
		top = add_alignment_to_global(seg, gbp->arp_list, top, FALSE, left, right, length);
		bottom = top - ABS(gbp->rec.top - gbp->rec.bottom);
	}

	/*add the sequence label*/
	label = gbp->label;
	if(label != NULL)
	{
		AddAttribute (seg, COLOR_ATT, BLACK_COLOR, 0, 0, 0, 0);
		font = (FonT)GetMuskCParam(MSM_SEQUENCE, MSM_SLABEL, MSM_FONT);
		label_align = (Uint1)GetMuskCParam(MSM_TOPSTYLE, MSM_SLABEL, MSM_STYLE);
		middle = (top + bottom)/2;
		if(label_align == MSM_LABEL_RIGHT)
			AddTextLabel(seg, right, middle, label, font, 0, MIDDLE_RIGHT, 0);
		else
			AddTextLabel(seg, left, middle, label, font, 0, MIDDLE_LEFT, 0);
	}
	
	if(bsp->repr != Seq_repr_seg && bsp->repr !=Seq_repr_delta)	
		/*not a segmented entry or delta seq*/
	{
		if(check_cyto)
			draw_cyto_map(bsp, left, top, right, bottom, seg);
		else
		{
			add_attribute_pen(seg, MSM_SEQUENCE, MSM_SEGMENT);
			AddRectangle (seg, left, top, right, bottom, FALSE, TRUE, 0);
			if(GetMuskCParam(MSM_SEQUENCE, MSM_SEG_BORD, MSM_TRUEFALSE))
			{
				add_attribute_pen(seg, MSM_SEQUENCE, MSM_SEG_BORD);
				AddRectangle(seg, left, top, right, bottom, FALSE, FALSE, 0);
			}
		}
	}
	else if(bsp->repr == Seq_repr_seg)/*draw the segmented sequence*/
	{
		slp = bsp->seq_ext;
		maybe_mapid = figure_map_seqid(slp);
		m_start = 0;
		m_stop = -1;
		i = 0;
		while(slp)
		{
			++i;
			if(slp->choice != SEQLOC_NULL && slp->choice != SEQLOC_EMPTY)
			{
				seglen = SeqLocLen(slp);
	  			m_stop += seglen;
				if(seglen >10000 || (seglen * 10000 > bsp->length))
				{
					is_gap = is_map_segment(slp);
					if(!is_gap && maybe_mapid != NULL)
						is_gap = (SeqIdMatch(maybe_mapid, SeqLocId(slp)));
					if(!is_gap)
					{
						color = get_seg_color(i);
						AddAttribute (seg, COLOR_ATT, color, 0, 0, 0, 0);
						DrawInterval(m_start, m_stop, length, left, right, top, bottom, TRUE, seg);
					}
				}
				m_start = m_stop + 1;
			}
			slp = slp->next;
		}
		skip = FALSE;
		if(maybe_mapid != NULL)
		{
			t_bsp = BioseqFind(maybe_mapid);
			if(t_bsp != NULL && t_bsp->repr == Seq_repr_map)
				skip = TRUE;
		}
		if(!skip)
		{
		/*add the gaps*/
		m_start = 0;
		m_stop = -1;
		for(slp = bsp->seq_ext; slp != NULL; slp = slp->next)
		{
			pos = -1;
			if(slp->choice == SEQLOC_NULL || slp->choice == SEQLOC_EMPTY)
			{
				pos = map_linear_xpos (m_start, length, left, right);
			}
			else
			{
				seglen = SeqLocLen(slp);
				m_stop += seglen;
				is_gap = is_map_segment(slp);
				
				if(is_gap && seglen * (right - left)/length < 4)
				{
					pos = map_linear_xpos((m_start+m_stop)/2, length, left, right);
				}
				m_start = m_stop + 1;
			}
			if(pos != -1)
				AddSymbol(seg, pos, (top+bottom)/2, DIAMOND_SYMBOL, FALSE, MIDDLE_CENTER, 0);
		}
		}
					
		add_attribute_pen(seg, MSM_SEQUENCE, MSM_SEG_BORD);
		AddRectangle (seg, left, top, right, bottom, FALSE, FALSE, 0);
	}
	else	/*it is a delta seq*/
	{
		delta_node = bsp->seq_ext;
		m_start = 0;
		m_stop = -1;
		i = 0;
		while(delta_node)
		{
			is_gap = FALSE;
			if(delta_node->choice == 1)
			{
				slp = delta_node->data.ptrvalue;
				if(slp->choice != SEQLOC_NULL && slp->choice != SEQLOC_EMPTY)
					seglen = SeqLocLen(slp);
				else
					is_gap = TRUE;
			}
			else	/*it is a literal seq*/
			{
				slitp = delta_node->data.ptrvalue;
				if(slitp->length == 0 || slitp->seq_data == NULL)
				{
					is_gap = TRUE;
					if(slitp->length != 0)
					{
						m_stop += slitp->length;
						m_start = m_stop + 1;
					}
				}
				else
					seglen = slitp->length;
			}

			if(!is_gap)
			{
				m_stop += seglen;
				color = get_seg_color(i);
				AddAttribute (seg, COLOR_ATT, color, 0, 0, 0, 0);
				DrawInterval(m_start, m_stop, length, left, right, top, bottom, TRUE, seg);
				++i;
				m_start = m_stop + 1;
			}
			delta_node = delta_node->next;
		}

		/*looking into small gaps*/
		m_start = 0;
		m_stop = -1;
		delta_node = (ValNodePtr)(bsp->seq_ext);
		while(delta_node)
		{
			is_gap = FALSE;
			if(delta_node->choice == 1)
			{
				slp = delta_node->data.ptrvalue;
				if(slp->choice != SEQLOC_NULL && slp->choice != SEQLOC_EMPTY)
					seglen = SeqLocLen(slp);
				else
					is_gap = TRUE;
			}
			else	/*it is a literal seq*/
			{
				slitp = delta_node->data.ptrvalue;
				if(slitp->length == 0 || slitp->seq_data == NULL)
				{
					is_gap = TRUE;
					if(slitp->length != 0)
					{
						if(slitp->length * (right - left)/length >= 4)
							is_gap = FALSE;
						m_stop += slitp->length;
						m_start = m_stop + 1;
					}
				}
				else
					seglen = slitp->length;
			}

			if(is_gap)
			{
				pos = map_linear_xpos (m_start, length, left, right);
				AddSymbol(seg, pos, (top+bottom)/2, DIAMOND_SYMBOL, FALSE, MIDDLE_CENTER, 0);
			}
			else
			{
				m_stop += seglen;
				m_start = m_stop + 1;
			}
			delta_node = delta_node->next;
		}
	}
	

	AddAttribute (seg, COLOR_ATT|STYLE_ATT|SHADING_ATT|WIDTH_ATT, BLACK_COLOR, SOLID_LINE, SOLID_SHADING, 1, COPY_MODE); 
	if(show_map_unit)
	{
		np = getBioseqNumbering(bsp);
		draw_linear_seqmark(seg, length, left, top, right, bottom, np);
	}

	if(gbp->cyto_marks != NULL)
	{
		AddAttribute (seg, COLOR_ATT, BLACK_COLOR, 0, 0, 0, 0);
		label_linear_mark(seg, length, gbp->cyto_marks, left, top, right, bottom, TRUE);
	}
	if(gbp->l_marks != NULL)
	{
		AddAttribute (seg, COLOR_ATT, BLACK_COLOR, 0, 0, 0, 0);
		label_linear_mark(seg, length, gbp->l_marks, left, top, right, bottom, TRUE);
	}
	if(gbp->g_marks != NULL)	/*user selected marks*/
	{
		AddAttribute (seg, COLOR_ATT, RED_COLOR, 0, 0, 0, 0);
		label_linear_mark(seg, length, gbp->g_marks, left, top, right, bottom, FALSE);
	}
	
	/*add the repeat unit*/
	if(gbp->rrp_list)
	{
		add_attribute_pen(seg, MSM_SEQUENCE, MSM_SEGMENT);
		shading = LIGHT_SHADING;
		AddAttribute(seg, SHADING_ATT, 0, 0, shading, 0, 0);
		p_val = 0;
		rrp_list = gbp->rrp_list;
		while(rrp_list)
		{
			rrp = rrp_list->data.ptrvalue;
			if(rrp_list->choice != p_val)
			{
				p_val = rrp_list->choice;
				/*shading = p_val%4 + 6;
				shading = LIGHT_SHADING;
				AddAttribute(seg, SHADING_ATT, 0, 0, shading, 0, 0); */
			}
			m_start = rrp->gr.left;
			m_stop = rrp->gr.right;
			DrawInterval(m_start, m_stop, length, left, right, top, bottom, TRUE, seg);
			rrp_list = rrp_list->next;
		}
	}

	/*add the global alignment as a result from powblast*/
	if(gbp->has_fish_align == FALSE && gbp->arp_list != NULL)
		top = add_alignment_to_global(seg, gbp->arp_list, top, TRUE, left, right, length);


	/*add the LOD score plot*/
	if(gbp->num_lod_score > 0)
	{
		extra_space = GetMuskCParam(MSM_TOPSTYLE, MSM_SPACE, MSM_HEIGHT);
		space = get_minimal_spacing();
		bottom -= space;
		for(annot = bsp->annot; annot != NULL; annot = annot->next)
		{
			if(is_lod_score_annot(annot))
			{
				top = bottom -  2* extra_space;
				bottom = top - (gbp->rec.bottom - gbp->rec.top);
				for(sfp = annot->data; sfp != NULL; sfp = sfp->next)
				{
					color = MapLodScoreColor(sfp);
					AddAttribute (seg, COLOR_ATT, color, 0, 0, 0, 0);
					m_start = SeqLocStart(sfp->location);
					m_stop = SeqLocStop(sfp->location);
					DrawInterval(m_start, m_stop, length, left, right, top, bottom, TRUE, seg);
				}
				add_attribute_pen(seg, MSM_SEQUENCE, MSM_SEG_BORD);
				AddRectangle (seg, left, top, right, bottom, FALSE, FALSE, 0);

				label = GetAnnotTitle(annot);
				if(label != NULL)
				{
					AddAttribute (seg, COLOR_ATT, BLACK_COLOR, 0, 0, 0, 0);
					font = (FonT)GetMuskCParam(MSM_SEQUENCE, MSM_SLABEL, MSM_FONT);
					label_align = (Uint1)GetMuskCParam(MSM_TOPSTYLE, MSM_SLABEL, MSM_STYLE);
					middle = (top + bottom)/2;
					if(label_align == MSM_LABEL_RIGHT)
						AddTextLabel(seg, right, middle, label, font, 0, MIDDLE_RIGHT, 0);
					else
						AddTextLabel(seg, left, middle, label, font, 0, MIDDLE_LEFT, 0);
				}
			}
		}
	}
				
	return TRUE;
}


/*************************************************************************
*
*  Boolean  Draw_Global_Circle(bsp, show_map_unit, marks, p, radius, center)
*	draw the global view of a circular genome. 
*	bsp:	the map Bioseq
*	show_map_unit:	display the map unit or not
*	marks:	the marks, such as genes, to be displayed on the map
*	p:	the drawing PaneL
*	radius:	the radius of the physical map
*	center:	the center of the circle
*
**************************************************************************/
static FloatHi cal_angle_on_circle(Int4 pos, Int4 length)
{
	return (FloatHi)pos/length * (FloatHi)2 * PI;
}

static Int4 cal_angle_with_mm_degree(Int4 pos, Int4 length)
{
	FloatHi percent;
	Int4 angle;
	
	percent = (FloatHi)pos/(FloatHi)(length-1) * (FloatHi) 360000;
	angle = 90L * 1000L - (Int4)percent;
	if(angle < 0)
		angle = 360000 + angle;
	return angle;
}

static void find_point_on_circle(FloatHi angle, PoinT center, Int2 radius, PointPtr c_pos)
{
	FloatHi temp;

	temp = (FloatHi)radius * sin(angle); 
	c_pos->x  = center.x + (Int2)temp;
 	temp = (FloatHi)radius * cos(angle); 
	c_pos->y = center.y - (Int2)temp;
}
 

static FloatHi  cal_mark_pos(Int4 pos, Int4 length, Int2 radius, PoinT center, Int2 tick_len, PointPtr p_start, PointPtr p_stop, Boolean inward)
{
	FloatHi angle;

	angle = cal_angle_on_circle(pos, length);
	find_point_on_circle(angle, center, radius, p_start);
	if(inward)
	  	radius -=tick_len;
	else
		radius += tick_len;

	find_point_on_circle(angle, center, radius, p_stop);
	return angle;
}


static Uint1 get_circle_label_align(FloatHi angle, Boolean inward)
{
	Uint1 align;
	
	if((angle >= 0 && angle <= (PI*0.25)) || (angle >= (PI*1.75) && angle <= (PI *2.0)))
		align = inward ? LOWER_CENTER : UPPER_CENTER;
	if(angle > (PI * 0.25) && angle <= (PI * 0.75))
		align = inward ? MIDDLE_LEFT : MIDDLE_RIGHT;
	if(angle > (PI * 0.75) && angle <= (PI * 1.25))
		align = inward ? UPPER_CENTER : LOWER_CENTER;
	if(angle > (PI * 1.25) && angle <= (PI * 1.75))
		 align = inward ? MIDDLE_RIGHT: MIDDLE_LEFT;
	return align;
}


static void draw_circle_seqmark(SegmenT seg, Int4 length, PoinT center, Int2 radius, NumberingPtr np)
{
  FloatHi temp;
  Int4 circle_len, scale, ruler;
  Boolean use_kb;
  Int4 i;
  Int4 pos;

  PoinT p_start, p_stop;
  FonT font;
  Char str[40];
  FloatHi angle;
  Int2 tick_len;
  
  Int4 left, top, right, bottom;
  Uint1 label_align;


	temp = 2.0 * PI * (FloatHi)radius;
	circle_len = (Int4)temp;
	scale = length/circle_len;
	if (length%circle_len!= 0)
		++scale;
	ruler = calculate_ruler(scale)/10;
	use_kb = (ruler >= 200);
	AddAttribute (seg, COLOR_ATT, BLACK_COLOR, 0, 0, 0, 0);
	font = (FonT)GetMuskCParam(MSM_SEQUENCE, MSM_SLABEL, MSM_FONT);

	i=0;
	pos = 0;
	while(pos < length)
	{				/**mark the scales**/
		tick_len = (i%5==0) ? TICK_LEN: TICK_LEN/2;
		angle = cal_mark_pos(pos, length, radius, center, tick_len, &p_start, &p_stop, TRUE);
		left = (Int4)(p_stop.x);
		right = (Int4)(p_start.x);
		top = - (Int4)(p_stop.y);
		bottom = - (Int4)(p_start.y);
		AddLine(seg, left, top, right, bottom, 0, FALSE);
		if(i%5 ==0)	/*label the the interval*/
		{
			map_unit_label(pos, np, str, use_kb);
			label_align = get_circle_label_align(angle, TRUE);
			AddTextLabel(seg, left, top, str, font, 0, label_align, 0);
		}
		
		++i;
		pos += ruler;
	}
}

static Boolean label_circle_mark(SegmenT seg, Int4 seqlen, ValNodePtr marks, PoinT center, Int2 radius, Boolean ck_overlap)
{
	FonT font;
	MapMarkPtr mmp;
	Int2 tick_len = TICK_LEN;
	PoinT p_start, p_stop;
	FloatHi angle;
	Int4 p_top = -1, p_width = -1, p_left = -1;
	Int4 first_top, first_width, first_left;	/*first marker*/
	Int4 left, right, top, bottom;
	Int4 width;
	Int4 font_height;
	Boolean drawit;
	Uint1 label_align, p_align, first_align;
	Boolean has_prev = FALSE;
	Int4 t_width;


	font = (FonT)GetMuskCParam(MSM_SEQUENCE, MSM_SLABEL, MSM_FONT);
	SelectFont(font);
	font_height = FontHeight();
	
	while(marks)
	{
		mmp = marks->data.ptrvalue;
		angle = cal_mark_pos(mmp->pos, seqlen, radius, center, tick_len, &p_start, &p_stop, mmp->inward);
		width = StringWidth(mmp->label);
		left = (Int4)(p_stop.x);
		right = (Int4)(p_start.x);
		top = - (Int4)(p_stop.y);
		bottom = - (Int4)(p_start.y);
		drawit = TRUE;
		label_align = get_circle_label_align(angle, mmp->inward);

		if(ck_overlap && has_prev)
		{
			t_width = (p_width + width)/2;
			if(label_align != p_align)
			{
				if(p_top != -1)
					if(ABS(p_top - top )< (font_height+3))
						if(ABS(p_left - left) < (t_width +3))
							drawit  = FALSE;
			}
			else
			{
				switch(label_align)
				{
					case LOWER_CENTER : 
					case UPPER_CENTER :
						if(ABS(p_left - left) < (t_width +3))
							drawit = FALSE;
						break;
					case MIDDLE_LEFT :
					case MIDDLE_RIGHT:
						if(ABS(p_top - top )< (font_height+3))
							drawit = FALSE;
						break;
					default:
						break;
				}
			}
	
			if(drawit && marks->next == NULL)
			{
				t_width = (first_width + width)/2;
				if(label_align != first_align)
				{
					if(first_top != -1)
						if(ABS(first_top - top )< (font_height+3))
							if(ABS(first_left - left) < (t_width +3))
								drawit  = FALSE;
				}
				else
				{
					switch(label_align)
					{
						case LOWER_CENTER : 
						case UPPER_CENTER :
							if(ABS(first_left - left) < (t_width +3))
							drawit = FALSE;
							break;
						case MIDDLE_LEFT :
						case MIDDLE_RIGHT:
							if(ABS(first_top - top )< (font_height+3))
								drawit = FALSE;
							break;
						default:
							break;
					}
				}
			}
		}
		if(drawit)
		{
			p_top = top;
			p_left = left;
			p_width = width;
			AddLine(seg, left, top, right, bottom, 0, FALSE);
			AddTextLabel(seg, left, top, mmp->label, font, 0, label_align, 0);
			p_align = label_align;
			if(!has_prev)
			{
				first_top = top;
				first_left = left;
				first_width = width;
				first_align = label_align;
			}
			has_prev = TRUE;
		}
		marks = marks->next;
	}

	return TRUE;
}

		

static Boolean  Draw_Global_Circle(GlobalBspPtr gbp, SegmenT seg, PoinT center)
{
  BioseqPtr bsp;
  Boolean show_map_unit;
  Int2 radius;
  
  NumberingPtr np = NULL;
  Int4 length;
  Int4 start_angle, stop_angle;

  SeqLocPtr slp;
  Int4 m_start, m_stop;
  Boolean is_gap;
  Int2 i;
  Uint1Ptr color;
  SeqIdPtr maybe_mapid;	/*a possible id for map*/
  Int4 center_x, center_y;
 


	if(gbp == NULL || gbp->bsp == NULL || gbp->hide)
		return FALSE;
	bsp = gbp->bsp;
	show_map_unit = gbp->show_map_unit;
	radius = gbp->radius;	
	length = bsp->length;
	add_attribute_pen(seg, MSM_SEQUENCE, MSM_SEGMENT);

	center_x = (Int4)(center.x);
	center_y = -(Int4)(center.y);
	if(bsp->repr != Seq_repr_seg)	/*not a segmented entry*/
		AddOval(seg, center_x, center_y, radius, radius, FALSE, 0);
	else	/*draw the segmented sequence*/
	{
		slp = bsp->seq_ext;
		maybe_mapid = figure_map_seqid(slp);
		m_start = 0;
		m_stop = -1;
		i = 0;
		while(slp)
		{
			++i;
	  		m_stop += SeqLocLen(slp);
			is_gap = is_map_segment(slp);
			if(!is_gap && maybe_mapid != NULL)
				is_gap = (SeqIdMatch(maybe_mapid, SeqLocId(slp)));
			if(!is_gap)
			{
				start_angle = cal_angle_with_mm_degree(m_stop, length);
				stop_angle = cal_angle_with_mm_degree(m_start, length);
				if(start_angle == stop_angle)
				{
					if(slp == bsp->seq_ext)
					{
						start_angle = 0;
						stop_angle = 360000;
					}
				}
				color = get_seg_color(i);
				AddAttribute (seg, COLOR_ATT, color, 0, 0, 0, 0);
				AddArc(seg, center_x, center_y, radius, radius, start_angle, stop_angle, FALSE, i);
			}
			m_start = m_stop +1;
			slp = slp->next;
		}
	}

	if(show_map_unit)
	{
		np = getBioseqNumbering(bsp);
		draw_circle_seqmark(seg, length, center, radius, np);
	}

	if(gbp->l_marks != NULL)
	{
		AddAttribute (seg, COLOR_ATT, BLACK_COLOR, 0, 0, 0, 0);
		label_circle_mark(seg, length, gbp->l_marks, center, radius, TRUE);
	}
	if(gbp->g_marks != NULL)
	{
		AddAttribute (seg, COLOR_ATT, RED_COLOR, 0, 0, 0, 0);
		label_circle_mark(seg, length, gbp->g_marks, center, radius, FALSE);
	}
	
	return TRUE;
}



static void AddGeneEquivAlign(GeneDataPtr gdp, ValNodePtr gbsp_list, Boolean is_circle, PoinT center, SegmenT seg, Uint1 type)
{
	StdSegPtr ssp;
	SeqLocPtr locs;
	SeqIdPtr sip;
	ValNodePtr curr;
	GlobalBspPtr gbsp;
	Int2 j;
	FloatHi angle;
	Int4 start, stop;
	Int4 length;
	Int4 left, right, top, bottom;
	PoinT pt;
	AlignPos ap;
	
	
	while(gdp)
	{
		if(gdp->align_seg != NULL && gdp->align_seg->choice == type)
		{
			ssp = (StdSegPtr)(gdp->align_seg->data.ptrvalue);
			j =0;
			for(locs = ssp->loc; locs != NULL; locs = locs->next)
			{
				sip = SeqLocId(locs);
				start = SeqLocStart(locs);
				stop = SeqLocStop(locs);
				for(curr = gbsp_list; curr != NULL; curr = curr->next)
				{
					gbsp = curr->data.ptrvalue;
					if(BioseqMatch(gbsp->bsp, sip))
					{
						length = gbsp->bsp->length;
						if(is_circle)
						{
							angle = cal_angle_on_circle(start, length);
							find_point_on_circle(angle, center, gbsp->radius, &pt);
							ap.left[j] = (Int4)(pt.x);
							ap.top[j] = 0 - (Int4)(pt.y);
							
							angle = cal_angle_on_circle(stop, length);
							find_point_on_circle(angle, center, gbsp->radius, &pt);
							ap.right[j] = (Int4)(pt.x);
							ap.bottom[j] = 0- (Int4)(pt.y);
						}
						else
						{
							left = (Int4)(gbsp->rec.left);
							right = (Int4)(gbsp->rec.right);
							top = - (Int4)(gbsp->rec.top);
							bottom = - (Int4)(gbsp->rec.bottom);
							ap.left[j] = map_linear_xpos (start, length, left, right);
							ap.top[j] = top;
							ap.right[j] = map_linear_xpos (stop, length, left, right);
							ap.bottom[j] = bottom;
						}
						++j;
						break;
					}
				}
			}
			if(j >1)
			{
				SortAlignPosition(&ap, j);
				draw_one_align(ap, j, 0, seg);
			}
		}
		gdp = gdp->next;
	}
}

static GlobalBspPtr find_global_bsp(GlobalDrawPtr gdraw_p, SeqIdPtr sip)
{
	ValNodePtr curr;
	GlobalBspPtr gbp;
	
	for(curr = gdraw_p->gbp_list; curr != NULL; curr = curr->next)
	{
		gbp = curr->data.ptrvalue;
		if(gbp && gbp->bsp != NULL)
		{
			if(BioseqMatch(gbp->bsp, sip))
				return gbp;
		}
	}
	return NULL;
}
	
	
/*******************************************************************
*
*	map the selected regions into Points that will be used 
*	for drawing the Polygons
*
*******************************************************************/
static PntPtr map_selected_points(ValNodePtr slp_list, GlobalDrawPtr gdraw_p, Int2Ptr num)
{
	PntPtr points;
	Int2 n;
	SeqLocPtr slp;
	PntPtr pnt;
	GlobalBspPtr gbp;

	if(gdraw_p->is_circle)
		return NULL;

	n = get_vnp_num(slp_list);
	if(n == 0)
		return NULL;
	++n;	/*for the last point*/
	points = MemNew((size_t)2*n * sizeof(PntInfo));

	n = 0;
	while(slp_list)
	{
		slp = slp_list->data.ptrvalue;
		gbp = find_global_bsp(gdraw_p, SeqLocId(slp));
		if(gbp != NULL)
		{
			if(gdraw_p->is_circle == FALSE)
			{
				pnt = &(points[n]);
				pnt->x = map_linear_xpos(SeqLocStart(slp), 
					gbp->bsp->length, gbp->rec.left, gbp->rec.right);
				pnt->y = -(Int4)(gbp->rec.top);
				++n;

				pnt = &(points[n]);
				pnt->x = map_linear_xpos(SeqLocStop(slp), 
					gbp->bsp->length, gbp->rec.left, gbp->rec.right);
				pnt->y = -(Int4)(gbp->rec.top);
				++n;

				if(slp_list->next == NULL)	/*add the last one*/
				{
					/*the last point*/
					pnt = &(points[n]);
					pnt->x = points[n-2].x;
					pnt->y = -(Int4)(gbp->rec.bottom);

					++n;
					pnt = &(points[n]);
					pnt->x = points[n-2].x;
					pnt->y = -(Int4)(gbp->rec.bottom);
					++n;
				}
			}
		}

		slp_list = slp_list->next;
	}

	*num = n;
	return points;
}
		
/************************************************************************
*
*	Draw_Global(gdraw_p)
*	return a picture created from GlobalDrawPtr
*
************************************************************************/
SegmenT Draw_Global(GlobalDrawPtr gdraw_p)
{
	SegmenT pic;
	Boolean is_circle;
	GlobalBspPtr g_bsp;
	ValNodePtr gbp_list;
	PntPtr points;
	PntInfo pi[4];
	Int2 num, i;
	
	if(gdraw_p == NULL)
	{
		Message(MSG_ERROR, "No Input Data");
		return NULL;
	}
	
	pic = CreatePicture();
	


	/*draw the high-light of the selected region*/
	is_circle = gdraw_p->is_circle;
	if(gdraw_p->slp_list != NULL && !is_circle)
	{
		points = map_selected_points(gdraw_p->slp_list, gdraw_p, &num);
		if(points && num >= 4)
		{
			/* AddAttribute(pic, COLOR_ATT|STYLE_ATT|WIDTH_ATT, YELLOW_COLOR, DASHED_LINE, 0, 4, 0); */
			AddAttribute(pic, COLOR_ATT, YELLOW_COLOR, 0, 0, 0, 0);
			/* for(i = 0; i<(num/2-1); ++i)
			{
				pnt_1 = &(points[2*i]);
				pnt_2 = &(points[2*(i+1)]);
				AddLine(pic, pnt_1->x, pnt_1->y, pnt_2->x, pnt_2->y, FALSE, 0);
				pnt_1 = &(points[2*i+1]);
				pnt_2 = &(points[2*(i+1)+1]);
				AddLine(pic, pnt_1->x, pnt_1->y, pnt_2->x, pnt_2->y, FALSE, 0);
			} */
			for(i = 0; i<(num/2-1); ++i)
			{
				MemCopy(&(pi[0]), points+2*i, sizeof(PntInfo)); 
				MemCopy(&(pi[1]), points+2*i+1, sizeof(PntInfo)); 
				MemCopy(&(pi[2]), points+2*i+3, sizeof(PntInfo)); 
				MemCopy(&(pi[3]), points+2*i+2, sizeof(PntInfo)); 
				AddPolygon(pic, 4, (PntPtr)pi, TRUE, 0); 
			}
		}
		if(points)
			MemFree(points);
	}

	/*draw the green lines first*/
	add_attribute_pen(pic, MSM_ALIGNMENT, MSM_ALINE);
	AddAttribute(pic, STYLE_ATT, 0, SOLID_LINE, 0, 0, 0);
	AddGeneEquivAlign(gdraw_p->g_data, gdraw_p->gbp_list, is_circle, gdraw_p->center, pic, 1);
	AddAttribute(pic, COLOR_ATT|STYLE_ATT, RED_COLOR, DOTTED_LINE, 0, 0, 0);
	AddGeneEquivAlign(gdraw_p->g_data, gdraw_p->gbp_list, is_circle, gdraw_p->center, pic, 2);
 
	AddAttribute(pic, COLOR_ATT|STYLE_ATT, BLACK_COLOR, SOLID_LINE, 0, 0, 0);
	AddGeneEquivAlign(gdraw_p->g_data, gdraw_p->gbp_list, is_circle, gdraw_p->center, pic, 4);

	for(gbp_list = gdraw_p->gbp_list; gbp_list != NULL; gbp_list = gbp_list->next)
	{
		g_bsp = gbp_list->data.ptrvalue;
		if(is_circle)
			Draw_Global_Circle(g_bsp, pic, gdraw_p->center);
		else
			Draw_Global_Linear(g_bsp, pic, gdraw_p->draw_align_legend);
	}
	
	/* add_attribute_pen(pic, MSM_ALIGNMENT, MSM_ALINE);
	AddGeneEquivAlign(gdraw_p->g_data, gdraw_p->gbp_list, is_circle, gdraw_p->center, pic);*/

	return pic;



}


SegmenT MuskGlobalPicture(ValNodePtr msp_list, ValNodePtr ext_align, Boolean need_free, Int2 panel_width, Int2 panel_height, ValNodePtr user_list, GlobalDrawPtr PNTR vwr_extra, Uint1 equiv_align_option, Boolean draw_align_legend)
{
	GlobalDrawPtr gdraw_p;
	SegmenT pic;
	
	gdraw_p = CreateGlobalDrawData (msp_list, ext_align, user_list, need_free, TRUE, equiv_align_option);
	if(gdraw_p == NULL)
		return NULL;
	
	gdraw_p->draw_align_legend = draw_align_legend;
	if(!LayoutGlobalDrawData (gdraw_p, panel_width, panel_height))
		return NULL;
	pic = Draw_Global (gdraw_p);
	
	if(vwr_extra != NULL)
		*vwr_extra = gdraw_p;
	else
	{
		gdraw_p->free_data = FALSE;
		free_global_draw (gdraw_p);
	}
	
	return pic;
}
	

SegmenT SequinGlobalPicture (SeqEntryPtr sep, Int2 panel_width, Int2 panel_height, ValNodePtr user_list, GlobalDrawPtr PNTR vwr_extra, Boolean draw_align_legend)
{
	MuskSepPtr msp;
	ValNodePtr msp_list = NULL;
	SegmenT pic;
	
	msp = MemNew(sizeof(MuskSep));
	msp->sep = sep;
	msp->is_bin = TRUE;
	StringCpy(msp->file_name, "local.asn");
	ValNodeAddPointer(&msp_list, 0, (Pointer)msp);
	
	pic = MuskGlobalPicture(msp_list, NULL, FALSE, panel_width, panel_height, user_list, vwr_extra, 0, draw_align_legend);
	
	if(pic == NULL)
		ValNodeFreeData(msp_list);
		
	return pic;
}


SegmenT GlobalPictureUpdate(GlobalDrawPtr gdraw_p, ValNodePtr new_gene_list, Int2 panel_width, Int2 panel_height)
{
	GeneDataPtr g_data;
	GlobalBspPtr gbp;
	ValNodePtr curr;

	if(gdraw_p == NULL)
		return NULL;

	if(gdraw_p->gbp_list != NULL && gdraw_p->msp_list != NULL)
	{
		curr = gdraw_p->gbp_list;
		g_data = create_gene_data (gdraw_p->msp_list, new_gene_list);

		if(g_data != NULL)
		{
			while(curr)
			{
				gbp = (GlobalBspPtr)(curr->data.ptrvalue);
				gbp->g_marks = ValNodeFreeData(gbp->g_marks);
				add_genedata_to_GlobalBsp(gbp, g_data);
				curr = curr->next;
			}
			GeneDataFree(g_data);
		} else {
			while(curr)
			{
				gbp = (GlobalBspPtr)(curr->data.ptrvalue);
				gbp->g_marks = ValNodeFreeData(gbp->g_marks);
				curr = curr->next;
			}
		}
		gdraw_p->g_list = new_gene_list;
		if(!LayoutGlobalDrawData (gdraw_p, panel_width, panel_height))
			return NULL;
		return Draw_Global (gdraw_p);
	}
	
	return NULL;
		
}


/**********************************************************************
*
*
*   Boolean find_map_pos(m_start, m_stop, start, stop, bsp_data, center)
*	map the two points start, stop selected by rubber-band to an interval 
*	on the sequence (m_start, m_stop). If selected region contains the 
*	zero point across a circular molecule, *m_start > *m_stop
*	bsp_data provide the mapping info. center is the center of a 
*	circle. 
*	return TRUE for success, FALSE for fail.
*
************************************************************************/
static Boolean check_linear_pnt(PoinT start, PoinT stop, RecT rec)
{
	/*Int2 y_max, y_min;

	y_max = MAX(start.y, stop.y);
	y_min = MIN(start.y, stop.y);
	
	if(y_min > rec.top || y_max < rec.bottom)
		return FALSE;
	else
		return TRUE;*/
	if((start.x >= rec.right) && (stop.x >= rec.right))
		return FALSE;
	if((start.x <= rec.left) && (stop.x <= rec.left))
		return FALSE;
	return TRUE;
}

static Int4 convert_pnt_linear(PoinT pnt, RecT rec, Int4 seq_len)
{
  Int2 x;
  Int2 start, stop, length;
  FloatHi temp;

	start = rec.left;
	stop = rec.right;
	length = stop - start;
	x = pnt.x;
	if(pnt.x < start)
	  	x = start;
	if(pnt.x > stop)
	  	x = stop;
	temp = (FloatHi)(x - start)/(FloatHi)length * (FloatHi)seq_len;
	return (Int4)temp;
}
	

static Int4 circle_pnt_to_map(FloatHi x_pos, FloatHi y_pos, Int2 radius, Int4 seq_len, PoinT center)
{

   FloatHi angle;
   FloatHi temp;
   FloatHi center_x, center_y;
   Boolean x_minus, y_minus;

	center_x = (FloatHi)(center.x);
	center_y = (FloatHi)(center.y);
	x_minus = ((x_pos - center_x) < 0.0);
	y_minus = ((y_pos - center_y) < 0.0);
	temp = (x_pos - center_x)/(FloatHi)radius;
	if(x_minus)
		temp = 0.0 - temp;
	angle = asin(temp);
	if(x_minus && y_minus)
		angle = 2.0 * PI - angle;
	if(x_minus && !y_minus)
		angle = PI + angle;
	if(!x_minus && !y_minus)
		angle = PI - angle;
	temp = (FloatHi)seq_len * angle/((FloatHi)2.0 * PI);
	return (Int4)temp;
}


static Boolean get_cross_point(Int2 pos, Int2 border_1, Int2 border_2, Int4Ptr val_1, Int4Ptr val_2, Boolean is_x, Int2Ptr num_cross, Int2 radius, PoinT center, Int4 seq_len)
{
    FloatHi val;
    FloatHi cross_offset;
    FloatHi x_pos, y_pos;
    FloatHi cross_val[2], cur_pos;
    FloatHi temp;
    Int2 i;
    Int4 map_pos;
   

	if(*num_cross == 2)
	   	return TRUE;

	if(is_x)
	{
		if(pos < (center.x - radius) || pos > (center.x + radius))
			return FALSE;
		val = (FloatHi)(pos - center.x);
	}
	else
	{
		if(pos < (center.y - radius) || pos > (center.y + radius))
			return FALSE;
		val = (FloatHi)(pos - center.y);
	}
	temp = (FloatHi)radius * radius;
	cross_offset = sqrt(temp - val*val);
	if(cross_offset == 0.0)
		Message(MSG_OK, "radius is %d, val is %lf", radius, val);
	if(is_x)
	{
		cross_val[0] = (FloatHi)center.y + cross_offset;
		cross_val[1] = (FloatHi)center.y - cross_offset;
	}
	else
	{
		cross_val[0] = (FloatHi)center.x + cross_offset;
		cross_val[1] = (FloatHi)center.x - cross_offset;
	}



	 
	for(i = 0; i<2 && (*num_cross <2); ++i)
	{
	 	cur_pos = cross_val[i];
		if(cur_pos >= (FloatHi)border_1 && cur_pos <= (FloatHi)border_2)
		{
			x_pos = is_x ? (FloatHi)pos : cur_pos;
			y_pos = is_x ? cur_pos : (FloatHi)pos;
			map_pos = circle_pnt_to_map(x_pos, y_pos, radius, seq_len, center);
			if(*num_cross == 0)
			{
				*val_1 = map_pos;
				++ (*num_cross);
			}
			else
			{
				if(map_pos != *val_1)
				{
					*val_2 = map_pos;
					++ (*num_cross);
				}
			}
		}
	}
			
	return TRUE;

}


static Boolean get_point_on_circle(PoinT start, PoinT stop, Int4Ptr val_1, Int4Ptr val_2, Int2 radius, PoinT center, Int4 seq_len)
{
   Int2 num =0;
   Int2 x_start, x_stop, y_start, y_stop;

	x_start = MIN(start.x, stop.x);
	x_stop = MAX(start.x, stop.x);
	y_start = MIN(start.y, stop.y);
	y_stop = MAX(start.y, stop.y);

	get_cross_point(x_start, y_start, y_stop, val_1, val_2, TRUE, &num, radius, center, seq_len);
	get_cross_point(x_stop, y_start, y_stop, val_1, val_2, TRUE, &num, radius, center, seq_len);
	get_cross_point(y_start, x_start, x_stop, val_1, val_2, FALSE, &num, radius, center, seq_len);
	get_cross_point(y_stop, x_start, x_stop, val_1, val_2, FALSE, &num, radius, center, seq_len);

	
	return (num ==2);	/*if two points on the circle are found or not*/

}


static  Int4 get_min_size(Int4 seq_len)
{
	if(seq_len <= 350000)
		return seq_len;

	return MAX(350000, seq_len/8);
}
	
static SeqLocPtr load_seq_loc(Int4 val_1, Int4 val_2, Int4 seq_len, Boolean has_zero, SeqIdPtr sip, BoolPtr show_msg, Boolean add_interval)
{
  Int4 m_start, m_stop;
  Uint1 m_strand = Seq_strand_plus;
  SeqLocPtr slp = NULL, loc;
  Int4 min_len;
  Int4 temp;
  Int4 len;

	min_len = get_min_size(seq_len);

	
	if(has_zero)
	{
		if(val_1 < val_2)
		{
			temp = val_2;
			val_2 = val_1;
			val_1 = temp;
		}
		if(add_interval)
		{
			val_1 -= seq_len/20;
			val_2 += seq_len/20;
		}
		len = (seq_len - 1 - val_1 +1 + val_2 +1);
		if(len > min_len)
		{
			if(*show_msg)
				Message(MSG_OK, "The selected region is too large. Reduced to %ld base pairs", min_len);
			val_1 = seq_len - 1 - min_len/2;
			val_2= min_len/2 -1;
		}
		m_start = val_1;
		m_stop = seq_len -1;
		loc = SeqLocIntNew(m_start, m_stop, m_strand, sip);
		m_start = 0;
		m_stop = val_2;
		loc->next = SeqLocIntNew(m_start, m_stop, m_strand, sip);
		slp = loc;
	}
	else
	{
		m_start = MIN(val_1, val_2);
		m_start = MAX(0, m_start);

		m_stop = MAX(val_1, val_2);
		m_stop = MIN(seq_len-1, m_stop);
		len = m_stop - m_start + 1;
		if(add_interval)
		{
			len += seq_len/10;
			m_start = MAX(0, (m_start - seq_len/20));
			m_stop = m_start + len -1;
		}

		/* if(len > min_len)
		{
			if(*show_msg)
				Message(MSG_OK, "The selected region is too large. Reduced to %ld base pairs", min_len);
			center = (m_start + m_stop)/2;
			m_start = center - min_len/2;
			m_stop = m_start + min_len -1;
		} */
		slp = SeqLocIntNew(m_start, m_stop, m_strand, sip);
	}
	*show_msg = FALSE;
	return slp;
}

static Boolean select_whole_circle(PoinT start, PoinT stop, PoinT center, Int2 r)
{
	if(start.x < (center.x -r) && start.y < (center.y -r))
	{
		if(stop.x > (center.x +r) && stop.y >(center.y+r))
			return TRUE;
	}

	if(stop.x < (center.x -r) && stop.y < (center.y -r))
	{
		if(start.x > (center.x +r) && start.y >(center.y+r))
			return TRUE;
	}

	return FALSE;
}
	

static Boolean check_two_point(PoinT start, PoinT stop)
{
	if(ABS(start.x - stop.x) < 3 && ABS(start.y - stop.y) < 3)
		return TRUE;
	else
		return FALSE;
}

static ValNodePtr find_circle_pos(PoinT start, PoinT stop, ValNodePtr gbp_list, PoinT center)
{

  RecT or;
  PoinT zero_pnt;
  Int4 val_1, val_2;
  FloatHi percent_1, percent_2;
  FloatHi temp;

  ValNodePtr curr;
  GlobalBspPtr gbp;
  BioseqPtr bsp;
  Int4 seq_len;
  Boolean is_found = FALSE;
  Boolean has_zero = FALSE;
  SeqLocPtr slp;
  ValNodePtr slp_list = NULL;
  Boolean show_msg = TRUE;
  Boolean add_interval;


	add_interval = check_two_point(start, stop);
	for(curr = gbp_list; curr !=NULL && !is_found; curr = curr->next)
	{
		gbp = curr->data.ptrvalue;
		bsp = gbp->bsp;
		seq_len = bsp->length;
		if(select_whole_circle(start, stop, center, gbp->radius))
		{
			is_found = TRUE;
			val_1 =0;
			val_2 = seq_len -1;
		}
		else
		{
			zero_pnt.x = center.x;
			zero_pnt.y = center.y - gbp->radius;
  			LoadRect (&or, start.x, start.y, stop.x, stop.y);
	  		has_zero = PtInRect(zero_pnt, &or);
			is_found = get_point_on_circle(start, stop, &val_1, &val_2, gbp->radius, center, seq_len);
		}
	}


	if(is_found)
	{
		percent_1 = (FloatHi)(val_1)/(FloatHi)(seq_len);
		percent_2 = (FloatHi)(val_2)/(FloatHi)(seq_len);
		
		for(curr = gbp_list; curr !=NULL; curr = curr->next)
		{
			gbp = curr->data.ptrvalue;
			bsp = gbp->bsp;
			seq_len = bsp->length;
			temp = percent_1 * (FloatHi)seq_len;
			val_1 = (Int4)temp;
			temp = percent_2 * (FloatHi)seq_len;
			val_2 = (Int4)temp;
				
			slp = load_seq_loc(val_1, val_2, seq_len, has_zero, bsp->id, &show_msg, add_interval);
			if(slp != NULL)
				ValNodeAddPointer((Pointer)(&slp_list), 0, slp);
		}
	}
	return slp_list;
}


/*********************************************************************
*
*
*	find_map_pos(convert the start_pnt, stop_pnt into a list of 
*	Seq-locs)
*
*********************************************************************/
ValNodePtr find_map_pos(GlobalDrawPtr gdraw_p, PntInfo start_info, PntInfo stop_info)
{

	Int4 val_1, val_2;
	ValNodePtr gbp_list; 
	PoinT center;
	ValNodePtr curr;
	GlobalBspPtr gbp;
	BioseqPtr bsp;
	Int4 seq_len;
	SeqLocPtr slp;
	ValNodePtr slp_list = NULL;
	Boolean show_msg = TRUE;
	Boolean add_interval;
	PoinT start_pnt, stop_pnt;


	if(gdraw_p == NULL)
		return NULL;
	start_pnt.x = (Int2)(start_info.x);
	start_pnt.y = (Int2)(start_info.y);
	stop_pnt.x = (Int2)(stop_info.x);
	stop_pnt.y = (Int2)(stop_info.y);
	start_pnt.y = ABS(start_pnt.y);
	stop_pnt.y = ABS(stop_pnt.y);

	gbp_list = gdraw_p->gbp_list;
	center = gdraw_p->center;
	if(gdraw_p->is_circle)
		return find_circle_pos(start_pnt, stop_pnt, gbp_list, center);
	
	add_interval = check_two_point(start_pnt, stop_pnt);
	for(curr = gbp_list; curr !=NULL; curr = curr->next)
	{
		gbp = curr->data.ptrvalue;
		bsp = gbp->bsp;
		seq_len = bsp->length;
		if(check_linear_pnt(start_pnt, stop_pnt, gbp->rec))
		{
			val_1 = convert_pnt_linear(start_pnt, gbp->rec, seq_len);
			val_2 = convert_pnt_linear(stop_pnt, gbp->rec, seq_len);
			slp = load_seq_loc(val_1, val_2, seq_len, FALSE, SeqIdFindBest(bsp->id, 0), &show_msg, add_interval);
			if(slp != NULL)
			ValNodeAddPointer((Pointer)&slp_list, (Uint1)(gbp->priority), slp);
		}
	}
	return slp_list;
}

/*
*
*	if there is a Bioseq in the list that records the cytogenetic 
*	band, return the Bioseq
*
*/
BioseqPtr has_cyto_map (GlobalDrawPtr gdraw_p)
{
	ValNodePtr curr;
	GlobalBspPtr gbp;

	for(curr = gdraw_p->gbp_list; curr != NULL; curr = curr->next)
	{
		gbp = curr->data.ptrvalue;
		if(gbp->bsp != NULL && gbp->map_type == CYTO_MAP)
			return gbp->bsp;
	}
	return NULL;
}



