
#include <vibrant.h>
#include <mapgene.h>
#include <prtgene.h>
#include <seqmgr.h>

/*the directory where the gif images are stored*/
#define GIF_DIR "http://sgiweb.ncbi.nlm.nih.gov:80/Zjing/"
/*the file that maps the name of Eric's contig to CONTIGA-V*/
#define ERIC_CONTIG_MAP "eric_contig.map"	/*file that maps Eric Green's 
					Contig from Ctg[sWSS] to CONTIGA-U*/
#define FISH_MAP_FILE "fish.band"
#define SEQ_CENTER_LINK_FILE "sqcenter.link"
#define YAC2BAC_FILE "yac2bac.lst"

static void OrderInt4(Int4Ptr x, Int4Ptr y) /* replace jzmisc: swap */
{
  Int4 temp;

	if((*x) > (*y)){
	  temp = *x;
	  *x = *y;
	  *y = temp;
	}
}

static void print_this_id (SeqIdPtr sip)
{
	Char buf[101];

	MuskSeqIdWrite(sip, buf, 40, PRINTID_FASTA_LONG, FALSE, FALSE);
	/* printf("id = %s\n", buf); */
}


static Boolean is_db_map (SeqIdPtr sip, CharPtr db)
{
	DbtagPtr db_tag;

	if(sip == NULL)
		return FALSE;
	if(sip->choice == SEQID_GENERAL)
	{
		db_tag = sip->data.ptrvalue;
		if(StringCmp(db_tag->db, db) == 0 )
			return TRUE;
	}
	return FALSE;
}

static Boolean match_gene_loc(SeqLocPtr gene_loc, SeqLocPtr align_loc)
{

	return (SeqLocCompare(align_loc, gene_loc) != SLC_NO_MATCH);
}

typedef struct figure_site {	
	SeqLocPtr slp;	/* the Seq-loc that 
				were figured out by left-right boundary */
	Int4 m_left;	/* left position mapped to the matched Seq-loc */
	Int4 m_right;	/* right position mapped to the matched Seq-loc */
	Uint1 priority;
	struct figure_site PNTR next;
}FigureSite, PNTR FigureSitePtr;

typedef struct match_sts_ist {
	SeqLocPtr slp;
	Uint1 priority;
	FigureSitePtr fsp;	/*the list of Seq-locs that were figured out 
				  based on the boundaries */
}MatchSite, PNTR MatchSitePtr;


static void add_figure_site PROTO((MatchSitePtr msp, Int4 m_left, 
	Int4 m_right, SeqLocPtr slp));

static Boolean load_new_align(StdSegPtr ssp, ValNodePtr msp_list, ValNodePtr PNTR missing_ids, Boolean ck_fsp)
{
	SeqLocPtr slp, match_slp, t_slp;
	ValNodePtr curr, prev, next;
	SeqIdPtr sip;
	Boolean match_list;
	MatchSitePtr msp, m_msp;
	FigureSitePtr m_fsp, fsp;

	while(ssp)
	{
		match_list = FALSE;
		match_slp = NULL;
		m_fsp = NULL;
		m_msp = NULL;

		/*check if the Seq-loc in alignment mathces anything that were 
		  already mapped*/
		for(slp = ssp->loc; !match_list && slp != NULL; slp = slp->next)
		{
			for(curr = msp_list; curr != NULL; curr = curr->next)
			{
				msp = curr->data.ptrvalue;
				if(match_gene_loc(msp->slp, slp))
				{
					match_list = TRUE;
					break;
				}
			}
		}
		if(match_list == FALSE && ck_fsp)
		{
			for(slp = ssp->loc; !match_list && slp != NULL; slp = slp->next)
			{
				for(curr = msp_list; curr != NULL; curr = curr->next)
				{
					msp = curr->data.ptrvalue;
					for(fsp = msp->fsp; fsp != NULL; fsp = fsp->next)
					{
						if(match_gene_loc(fsp->slp, slp))
						{
							match_list = TRUE;
							m_msp = msp;
							m_fsp = fsp;
							break;
						}
					}
					if(match_list)
						break;
				}
				if(match_list)
					break;
			}
		}
		
		if(match_list)
		{
			/*find if it contains the missing Seq-id*/
			for(slp = ssp->loc; slp != NULL; slp = slp->next)
			{
				sip = SeqLocId(slp);
				prev = NULL;
				curr = *missing_ids;
				while(curr)
				{
					next = curr->next;
					if(SeqIdMatch((SeqIdPtr)(curr->data.ptrvalue), sip))
					{
						if(m_fsp == NULL)
						{
							msp = MemNew(sizeof(MatchSite));
							msp->slp = SeqLocIntNew(SeqLocStart(slp), SeqLocStop(slp), SeqLocStrand(slp), sip);
							ValNodeAddPointer(&msp_list, 0, msp);
						}
						else
						{
							t_slp = SeqLocIntNew(SeqLocStart(slp), SeqLocStop(slp), SeqLocStrand(slp), sip);
							add_figure_site(m_msp, m_fsp->m_left, m_fsp->m_right, t_slp);
						}
						if(prev == NULL)
							*missing_ids = next;
						else
							prev->next = next;
						curr->next = NULL;
						ValNodeFree(curr);
						if(*missing_ids == NULL)
							return TRUE;
						break;
					}
					else
						prev = curr;
					curr = next;
				}
			}
		}



		ssp = ssp->next;
	}

	return FALSE;
}
			
	
static SeqIdPtr find_anchor_id(BioseqPtr bsp)
{
	SeqIdPtr sip;
	DbtagPtr db_tag;

	for(sip = bsp->id; sip != NULL; sip = sip->next)
	{
		if(sip->choice == SEQID_GENERAL)
		{
			db_tag = sip->data.ptrvalue;
			if(StringICmp(db_tag->db, "NCBI") == 0)
				return sip;
		}
	}

	return NULL;
}

static Int4 get_center_pointer(SeqLocPtr slp)
{
	Int4 total, num;

	total = 0;
	num = 0;

	while(slp)
	{
		total += (SeqLocStart(slp) + SeqLocStop(slp))/2;
		++num;
		slp = slp->next;
	}

	return total/num;
}

static Boolean is_id_in_msp_list(SeqIdPtr sip, ValNodePtr msp_list)
{
	MatchSitePtr msp;

	while(msp_list)
	{
		msp = msp_list->data.ptrvalue;
		if(SeqIdMatch(SeqLocId(msp->slp), sip))
			return TRUE;
		msp_list = msp_list->next;
	}

	return FALSE;
}


typedef struct match_fish {
	SeqIdPtr missing_id;
	MatchSitePtr msp;
}MatchFish, PNTR MatchFishPtr;


/*********************************************************
*
*	return a list of MatchFishPtr that were included 
*	in the FISH map alignment
*	align is the alignment of the FISH mapping data
*	slp_list: a list of already mapped locations
*	missing_ids: the list of Seq-ids for the Bioseqs 
*	that can not find the query marks
*
**********************************************************/
static ValNodePtr make_matching_for_fish (SeqAlignPtr align, ValNodePtr msp_list, ValNodePtr 
	missing_ids)
{

	StdSegPtr ssp;
	SeqLocPtr slp;
	ValNodePtr vnp, curr;
	SeqIdPtr sip;
	ValNodePtr fish_list;
	MatchFishPtr mfp;
	MatchSitePtr t_msp, msp;
	ValNodePtr match_msp_list = NULL;

	if(align == NULL || align->segtype != 3)
		return NULL;

	/*find the proper interval in the FISH alignment */
	ssp = align->segs;
	msp = NULL;
	for(slp = ssp->loc; slp != NULL; slp = slp->next)
	{
		sip = SeqLocId(slp);
		for(vnp = msp_list; vnp != NULL; vnp = vnp->next)
		{
			t_msp = vnp->data.ptrvalue;
			if(SeqIdMatch(SeqLocId(t_msp->slp), sip))
			{
				ValNodeAddPointer(&match_msp_list, 0, t_msp);
				if(SeqLocStart(slp) == SeqLocStop(slp))
				{	/* it is a point, prefer point to interval */
					msp  = t_msp;
				}
				else if(msp == NULL)
					msp = t_msp;
				break;
			}
		}
	}
	if(match_msp_list == NULL)
		return NULL;
	if(msp == NULL)
		return NULL;

	fish_list = NULL;
	for(vnp = missing_ids; vnp != NULL; vnp = vnp->next)
	{
		sip = vnp->data.ptrvalue;
		for(slp = ssp->loc; slp != NULL; slp = slp->next)
		{
			if(SeqIdMatch(sip, SeqLocId(slp)))
			{
				/*load the point first*/
				mfp = MemNew(sizeof(MatchFish));
				mfp->msp = msp;
				mfp->missing_id = sip;
				ValNodeAddPointer(&fish_list, 0, mfp);

				/*load the interval ones now*/
				for(curr = match_msp_list; curr != NULL; curr = curr->next)
				{
					t_msp = curr->data.ptrvalue;
					if(t_msp != msp)
					{
						mfp = MemNew(sizeof(MatchFish));
						mfp->msp = t_msp;
						mfp->missing_id = sip;
						ValNodeAddPointer(&fish_list, 0, mfp);
					}
				}
				break;
			}
		}
	}

	ValNodeFree(match_msp_list);
	return fish_list;
}

		
static void add_figure_site(MatchSitePtr msp, Int4 m_left, Int4 m_right, 
	SeqLocPtr slp)
{
	FigureSitePtr fsp, curr;

	fsp = MemNew(sizeof(FigureSite));
	fsp->m_left = m_left;
	fsp->m_right = m_right;
	fsp->slp = slp;

	if(msp->fsp == NULL)
		msp->fsp = fsp;
	else
	{
		curr = msp->fsp;
		while(curr->next != NULL)
			curr = curr->next;
		curr->next = fsp;
	}
}


static void get_match_range(MatchSitePtr msp, Int4Ptr m_left, Int4Ptr m_right)
{
	FigureSitePtr fsp;

	*m_left = SeqLocStart(msp->slp);
	*m_right = SeqLocStop (msp->slp);

	for(fsp = msp->fsp; fsp != NULL; fsp = fsp->next)
	{
		*m_left = MIN(*m_left, fsp->m_left);
		*m_right = MAX(*m_right, fsp->m_right);
	}
}


static void cal_new_pos_by_percent (SeqLocPtr left_mloc, SeqLocPtr right_mloc, 
	SeqLocPtr left_sloc, SeqLocPtr right_sloc, SeqLocPtr match_slp, Int4Ptr pstart, Int4Ptr pstop)
{
	Int4 m_left, m_right;
	Int4 start, stop;
	Int4 pos;
	FloatHi percent, val;

	m_left = SeqLocStart(left_mloc);
	m_right = SeqLocStop(right_mloc);
	start = SeqLocStart(left_sloc);
	stop = SeqLocStop(right_sloc);

	pos = SeqLocStart(match_slp);
	percent = (FloatHi)(pos - m_left)/(FloatHi)(m_right - m_left + 1);
	val = percent * (FloatHi)(stop - start + 1);
	*pstart = start + (Int4)val;

	if(SeqLocStop(match_slp) == pos)
		*pstop = *pstart;
	else
	{
		pos = SeqLocStop(match_slp);
		percent = (FloatHi)(pos - m_left)/(FloatHi)(m_right - m_left + 1);
		val = percent * (FloatHi)(stop - start + 1);
		*pstop = start + (Int4)val;
	}
}

typedef struct matchlocdata {
	SeqIdPtr match_sip;
	SeqLocPtr match_slp;
	ValNodePtr overlap_list;
	ValNodePtr range_match_list;
	SeqLocPtr left_mloc, right_mloc;
	SeqLocPtr left_sloc, right_sloc;
	Int4 m_left, m_right;
	Int4 m_start, m_stop;
	MatchSitePtr msp;

}MatchLocData, PNTR MatchLocDataPtr;


static ValNodePtr BuildMatchLocData (ValNodePtr msp_list, Boolean use_figure_site)
{
	ValNodePtr list = NULL;
	MatchSitePtr msp;
	MatchLocDataPtr mldp;
	Int4 m_left, m_right;
	
	FigureSitePtr fsp;

	while(msp_list)
	{
		msp = msp_list->data.ptrvalue;
		if(msp && msp->slp != NULL)
		{
			get_match_range(msp, &m_left, &m_right);
			if(!use_figure_site)
			{
				mldp = MemNew(sizeof(MatchLocData));
				mldp->match_slp = msp->slp;
				mldp->match_sip = SeqLocId(msp->slp);
				mldp->msp = msp;
				mldp->m_left = m_left;
				mldp->m_right = m_right;
				mldp->m_start = SeqLocStart(msp->slp);
				mldp->m_stop = SeqLocStop(msp->slp);
				ValNodeAddPointer(&list, 0, mldp);
			}
			else	/*try the original data first, and then use figure site*/
			{
				for(fsp = msp->fsp; fsp != NULL; fsp = fsp->next)
				{
					if(fsp->slp != NULL)
					{
						mldp = MemNew(sizeof(MatchLocData));
						mldp->match_slp = fsp->slp;
						mldp->match_sip = SeqLocId(fsp->slp);
						mldp->msp = msp;
						mldp->m_left = m_left;
						mldp->m_right = m_right;
						mldp->m_start = SeqLocStart(fsp->slp);
						mldp->m_stop = SeqLocStop(fsp->slp);
						ValNodeAddPointer(&list, 0, mldp);
					}
				}
			} 
		}
		msp_list = msp_list->next;
	}

	return list;
}

static void FreeMatchLocData (ValNodePtr mldp_list)
{
	ValNodePtr vnp;
	MatchLocDataPtr mldp;

	for(vnp = mldp_list; vnp != NULL; vnp = vnp->next)
	{
		mldp = vnp->data.ptrvalue;
		if(mldp->overlap_list != NULL)
			ValNodeFree(mldp->overlap_list);
		if(mldp->range_match_list != NULL)
			ValNodeFree(mldp->range_match_list);
		MemFree(mldp);
	}

	ValNodeFree(mldp_list);
}

static Int4 get_flanking_size (SeqLocPtr left_mloc, SeqLocPtr right_mloc)
{
	BioseqPtr bsp;

	if(right_mloc == NULL)
		return SeqLocStop(right_mloc);
	else if(left_mloc != NULL)
	{
		bsp = BioseqFindCore(SeqLocId(left_mloc));
		if(bsp != NULL)
			return (bsp->length -1 - SeqLocStart(left_mloc));
	}
	return -1;
}

static MatchLocDataPtr FindBestLeftRightLoc (ValNodePtr mldp_list)
{
	MatchLocDataPtr mldp, t_mldp;
	Int4 width, t_width;

	mldp = NULL;
	while(mldp_list != NULL)
	{
		t_mldp = mldp_list->data.ptrvalue;
		if(mldp == NULL)
			mldp = t_mldp;
		else
		{
			if(t_mldp->left_mloc!= NULL && t_mldp->right_mloc != NULL)
			{
				if(mldp->left_mloc == NULL || mldp->right_mloc == NULL)
					mldp = t_mldp;
				else
				{	/*find the smaller interval*/
					width = SeqLocStop(mldp->right_mloc) - SeqLocStart(mldp->left_mloc);
					t_width = SeqLocStop(t_mldp->right_mloc) - SeqLocStart(t_mldp->left_mloc);
					if(width > t_width)
						mldp = t_mldp;
				}
			}
			else if(t_mldp->left_mloc != NULL || t_mldp->right_mloc == NULL)
			{
				if(mldp->left_mloc == NULL && mldp->right_mloc == NULL)
					mldp = t_mldp;
				if(mldp->left_mloc == NULL || mldp->right_mloc == NULL)
				{	/*both have only one location*/
					t_width = get_flanking_size (t_mldp->left_mloc, t_mldp->right_mloc);
					if(t_width != -1)
					{
						width = get_flanking_size (mldp->left_mloc, mldp->right_mloc);
						if(width == -1 || t_width < width)
							mldp = t_mldp;
					}
				}
			}
		}
		mldp_list = mldp_list->next;
	}

	return mldp;
}

static Int4 get_offset_in_overlap (SeqLocPtr slp, SeqLocPtr match_slp)
{
	Int4 val;

	val = 0;
	val += ABS(SeqLocStart(slp) - SeqLocStart(match_slp));
	val += ABS(SeqLocStop(slp) - SeqLocStop(match_slp));

	return val;
}


static Boolean bad_overlap (SeqLocPtr slp, SeqLocPtr match_slp)
{
	Int4 start, stop;

	start = MAX(SeqLocStart(slp), SeqLocStart(match_slp));
	stop = MIN(SeqLocStop(slp), SeqLocStop(match_slp));

	/*at least 10% of the matched slp should be covered*/
	return (SeqLocLen(match_slp)/(stop - start + 1) > 10);
}



/*******************************************************************
*
*	map_one_fish_loc(align, msp, missing_id)
*	This will first look into if there is any alignments in 
*	the msp->slp, and then look for the flanking markers
*	the msp->l_extremes, msp->r_extremes will be updated 
*	as well
*	if(use_figure_site), use the FigureSite in msp_list. Otherwise
*	only use the results from direct mapping
*
********************************************************************/
static SeqLocPtr map_one_fish_loc(SeqAlignPtr align, ValNodePtr msp_list, 
	SeqIdPtr missing_id, Int4 window_size, Boolean use_figure_site)
{
	ValNodePtr mldp_list;
	MatchLocDataPtr mldp, m_mldp;
	ValNodePtr vnp;
	StdSegPtr ssp;
	SeqLocPtr slp;
	SeqLocPtr m_loc, s_loc;
	ValNodePtr curr;
	Int4 start, stop;
	Int4 t_start, t_stop;
	Int4 m_left, m_right;
	BioseqPtr bsp;
	Boolean record_master;
	Int4 center;
	Int2 val, i;
	Int4 offset, min_offset;
	Uint1 overlap_order[] = {SLC_A_EQ_B, SLC_A_IN_B, SLC_B_IN_A}; 
	Boolean load;
	 
	

	if(align == NULL || msp_list == NULL || missing_id == NULL)
		return NULL;
	/*do not use the figure site*/
	mldp_list = BuildMatchLocData (msp_list, use_figure_site);
	if(mldp_list == NULL)
		return NULL;
	while(align)
	{
		ssp = align->segs;
		for(vnp = mldp_list; vnp != NULL; vnp = vnp->next)
		{
			mldp = vnp->data.ptrvalue;
			m_loc = NULL;
			s_loc = NULL;
			for(slp = ssp->loc; slp != NULL; slp = slp->next)
			{
				if(SeqIdMatch(SeqLocId(slp), mldp->match_sip))
					m_loc = slp;
				else if(SeqIdMatch(SeqLocId(slp), missing_id))
					s_loc = slp;
			}
			if(m_loc != NULL && s_loc != NULL)
			{
				val = SeqLocCompare(m_loc, mldp->match_slp);
				if(val!= SLC_NO_MATCH && 
					(val != SLC_A_OVERLAP_B || !bad_overlap (m_loc, mldp->match_slp)))
					ValNodeAddPointer(&(mldp->overlap_list), (Uint1)val, (Pointer)align);
				else
				{
					if(SeqLocStart(m_loc) >= mldp->m_left && SeqLocStop(m_loc) <= mldp->m_right)
						ValNodeAddPointer(&(mldp->range_match_list), 0, align);
					if(mldp->m_start > SeqLocStop(m_loc))
					{
						if(mldp->left_mloc == NULL || 
							SeqLocStart(mldp->left_mloc) < SeqLocStart (m_loc))
						{
							mldp->left_mloc = m_loc;
							mldp->left_sloc = s_loc;
						}
					}
					else if(mldp->m_stop < SeqLocStart(m_loc))
					{
						if(mldp->right_mloc == NULL ||
							SeqLocStop(mldp->right_mloc) > SeqLocStop(m_loc))
						{
							mldp->right_mloc = m_loc;
							mldp->right_sloc = s_loc;
						}
					}
				}
			}
		}
		align = align->next;
	}

	/*check for perfect matches in the overlap list. If more than one 
	  perfect match was found, load the data with the smallest interval 
	  on the missing id */
	m_mldp = NULL;
	start = -1;
	stop = -1;
	m_left = -1;
	m_right = -1;
	min_offset = -1;
  for(i = 0; i<3; ++i)
  {
	val = overlap_order[i];
	for(vnp = mldp_list; vnp != NULL; vnp = vnp->next)
	{
		mldp = vnp->data.ptrvalue;

		/*find if there is anything that matches the master perfectly*/
		for(curr = mldp->overlap_list; curr != NULL; curr = curr->next)
		{
			if(curr->choice == (Uint1)val)	/*perfect match to the match loc*/
			{
				align = (SeqAlignPtr)(curr->data.ptrvalue);
				ssp = align->segs;
				m_loc = NULL;
				s_loc = NULL;
				for(slp = ssp->loc;  slp != NULL; slp = slp->next)
				{
					if(SeqIdMatch(SeqLocId(slp), missing_id))
						s_loc = slp;
					else if(SeqIdMatch(SeqLocId(slp), mldp->match_sip))
						m_loc = slp;
				}
				if(m_loc != NULL && s_loc != NULL)
				{
					if(val != SLC_A_EQ_B) /*the contained within type*/
						offset = ABS(SeqLocLen(m_loc) - SeqLocLen(mldp->match_slp));
					else
						offset = 0;
					load = FALSE;
					if(start == -1)
						load = TRUE;
					else
					{
						if(val == SLC_A_EQ_B)
						{
							/*select for a smaller interval*/
							if(SeqLocLen(s_loc) < (stop - start +1))
								load = TRUE;
						}
						else
						{
							if(min_offset == -1 || min_offset > offset)
								load = TRUE;
							else if(SeqLocLen(s_loc) < (stop - start +1))
								load = TRUE;
						}
					}
					if(load)
					{
						m_left = SeqLocStart(m_loc);
						m_right = SeqLocStop(m_loc);
						start = SeqLocStart(s_loc);
						stop = SeqLocStop(s_loc);
						m_mldp = mldp;
						min_offset = offset;
					}
				}
			}
		}
	}
	/*find the perfect match*/
	if(m_mldp != NULL)
	{
		if(val == SLC_A_EQ_B)
		{
			slp = SeqLocIntNew(start, stop, Seq_strand_plus, missing_id);
			add_figure_site(m_mldp->msp, m_left, m_right, slp);
			FreeMatchLocData(mldp_list);
			return slp;
		}
		else	/*one is contained within the other*/
		{
			/*get the offset of the center*/
			offset = (SeqLocStart(m_mldp->match_slp) + 
				SeqLocStop(m_mldp->match_slp))/2 - (m_right + m_left)/2;
			start += offset;
			stop += offset;
			slp = SeqLocIntNew(start, stop, Seq_strand_plus, missing_id);
			add_figure_site(m_mldp->msp, SeqLocStart(m_mldp->match_slp), SeqLocStop(m_mldp->match_slp),  slp);
			FreeMatchLocData(mldp_list);
			return slp;
		}
	}
  }


	/*check for the overlap list that has the smallest offset in overlap*/
	min_offset = -1;
	m_mldp = NULL;
	start = -1;
	stop = -1;
	m_left = -1;
	m_right = -1;
	for(vnp = mldp_list; vnp != NULL; vnp = vnp->next)
	{
		mldp = vnp->data.ptrvalue;
		for(curr = mldp->overlap_list; curr != NULL; curr = curr->next)
		{
			align = (SeqAlignPtr)(curr->data.ptrvalue);
			ssp = align->segs;
			m_loc = NULL;
			s_loc = NULL;
			for(slp = ssp->loc;  slp != NULL; slp = slp->next)
			{
				if(SeqIdMatch(SeqLocId(slp), missing_id))
					s_loc = slp;
				else if(SeqIdMatch(SeqLocId(slp), mldp->match_sip))
					m_loc = slp;
			}
			if(m_loc != NULL && s_loc != NULL)
			{
				record_master = FALSE;
				offset = get_offset_in_overlap (slp, mldp->match_slp);
				if(min_offset == -1 || offset < min_offset)
					record_master = TRUE;
				else if(min_offset == offset)
				{
					if(start == -1 || (SeqLocLen(s_loc) < (stop - start +1)))
						record_master = TRUE;
				}
				if(record_master)
				{
					m_left = SeqLocStart(m_loc);
					m_right = SeqLocStop(m_loc);
					start = SeqLocStart(s_loc);
					stop = SeqLocStop(s_loc);
					m_mldp = mldp;
				}
			}
		}
	}
	if(m_mldp != NULL)
	{
		slp = SeqLocIntNew(start, stop, Seq_strand_plus, missing_id);
		add_figure_site(m_mldp->msp, m_left, m_right, slp);
		FreeMatchLocData(mldp_list);
		return slp;
	}

	/*check the range matches*/
	for(vnp = mldp_list; vnp != NULL; vnp = vnp->next)
	{
		mldp = vnp->data.ptrvalue;
		start = -1;
		stop = -1;
		m_left = -1;
		m_right = -1;

		for(curr = mldp->range_match_list; curr != NULL; curr = curr->next)
		{
			align = (SeqAlignPtr)(curr->data.ptrvalue);
			ssp = align->segs;
			record_master = FALSE;
			for(slp = ssp->loc;  slp != NULL; slp = slp->next)
			{
				t_start = SeqLocStart(slp);
				t_stop = SeqLocStop(slp);
				if(SeqIdMatch(SeqLocId(slp), missing_id))
				{
					if(start == -1 || (SeqLocLen(slp) < (t_stop - t_start +1)))
					{
						start = t_start;
						stop = t_stop;
						record_master = TRUE;
					}
				}
				else if(record_master && SeqIdMatch(SeqLocId(slp), mldp->match_sip))
				{
					m_left = SeqLocStart(slp);
					m_right = SeqLocStop(slp);
				}
			}
		}
		if(start != -1 && stop != -1 && m_left != -1 && m_right != -1)
		{
			slp = SeqLocIntNew(start, stop, Seq_strand_plus, missing_id);
			add_figure_site(mldp->msp, m_left, m_right, slp);
			FreeMatchLocData(mldp_list);
			return slp;
		}
	}


	/*check for the left and right locations*/
	mldp = FindBestLeftRightLoc (mldp_list);
	if(mldp != NULL && (mldp->left_mloc != NULL || mldp->right_mloc != NULL))
	{
		if(mldp->left_mloc == NULL)
		{
			if(SeqLocStop(mldp->right_mloc) > window_size)
			{
				FreeMatchLocData(mldp_list);
				return NULL;
			}
			m_left = 0;
			m_right = SeqLocStop(mldp->right_mloc);
			slp = SeqLocIntNew(0, SeqLocStop(mldp->right_sloc), Seq_strand_plus, missing_id);
			add_figure_site(mldp->msp, m_left, m_right, slp);
			FreeMatchLocData(mldp_list);
			return slp;
		}
		if(mldp->right_mloc == NULL)
		{
			bsp = BioseqFindCore(SeqLocId(mldp->left_mloc));
			if(bsp->length -1 - SeqLocStart(mldp->left_mloc) > window_size)
			{
				FreeMatchLocData(mldp_list);
				return NULL;
			}
			m_left = SeqLocStart(mldp->left_mloc);
			if(window_size > 0)
				m_right = MIN(m_left + window_size -1, bsp->length -1);
			else
				m_right = bsp->length-1;
			start = SeqLocStart(mldp->left_sloc);
			if(window_size > 0)
				stop = MIN(start+window_size-1, bsp->length-1);
			else
				stop = bsp->length-1;
			slp = SeqLocIntNew(start, stop, Seq_strand_plus, missing_id);
			add_figure_site(mldp->msp, m_left, m_right, slp);
			FreeMatchLocData(mldp_list);
			return slp;
		}

		m_left = SeqLocStart(mldp->left_mloc);
		m_right = SeqLocStop(mldp->right_mloc);
		start = SeqLocStart(mldp->left_sloc);
		stop = SeqLocStop(mldp->right_sloc);

		/*as a protection for the over-size left, right alignment */
		if(window_size > 0 && stop - start > window_size + window_size/4)
		{
			cal_new_pos_by_percent (mldp->left_mloc, mldp->right_mloc, mldp->left_sloc, 
				mldp->right_sloc, mldp->match_slp, &start, &stop);
			m_left = SeqLocStart(mldp->match_slp);
			m_right = m_left + window_size -1;
			if(stop - start > window_size)
			{
				center = (start + stop)/2;
				start = center - window_size/2;
				stop = start + window_size -1;
			}
		}

		slp = SeqLocIntNew(start, stop, Seq_strand_plus, missing_id);
		add_figure_site(mldp->msp, m_left, m_right, slp);
		FreeMatchLocData(mldp_list);
		return slp;
	}
	FreeMatchLocData(mldp_list);
	return NULL;
	
}

static ValNodePtr create_msp_list_for_one_fish(ValNodePtr fish_list, SeqIdPtr missing_id)
{
	ValNodePtr list = NULL;
	MatchFishPtr mfp;

	while(fish_list)
	{
		mfp = fish_list->data.ptrvalue;
		if(SeqIdMatch(mfp->missing_id, missing_id))
			ValNodeAddPointer(&list, 0, mfp->msp);
		fish_list = fish_list->next;
	}
	return list;
}
		
/*****************************************************************
*
*	check if the alignment can be mapped by FISH mapping
*
*****************************************************************/
static Boolean load_align_by_fish(SeqAnnotPtr annot, ValNodePtr msp_list, ValNodePtr PNTR missing_ids, Int4 window_size)
{
	SeqAlignPtr align;
	ValNodePtr fish_list;
	ValNodePtr t_msp_list;
	SeqIdPtr sip;
	ValNodePtr curr, prev, next;
	SeqLocPtr slp;

	if(msp_list == NULL)
		return FALSE;
	align = annot->data;
	fish_list = make_matching_for_fish (align, msp_list, *missing_ids);
	if(fish_list == NULL)
		return FALSE;
	prev = NULL;
	curr = *missing_ids;
	while(curr)
	{
		next = curr->next;
		sip = curr->data.ptrvalue;
		t_msp_list = create_msp_list_for_one_fish(fish_list, sip);
		/*use the non-figure site frist*/
		slp = map_one_fish_loc(align, t_msp_list, sip, window_size, FALSE);
		/*fail, use the figure site*/
		if(slp == NULL)
			slp = map_one_fish_loc(align, t_msp_list, sip, window_size, TRUE);
		if(slp != NULL)
		{
			if(prev == NULL)
				*missing_ids = next;
			else
				prev->next = next;
			curr->next = NULL;
			ValNodeFree(curr);
		}
		else
			prev = curr;
		ValNodeFree(t_msp_list);
		curr = next;
	}

	ValNodeFreeData(fish_list);
	return (*missing_ids == NULL);
}

		
/********************************************************************
*
*	figure out the approximate position of the markers
*	try the anchor Bioseq first if it is mapped
*	if failed, try the ones that were mapped directly (not via 
*	the FISH mapping)
*	this does not do any distance extrapolation
*	This was considered the last resort to map a gene to 
*	un-mapped map. missing_ids were not freed
*
********************************************************************/
static void load_align_by_consist (SeqAlignPtr align, ValNodePtr msp_list, ValNodePtr PNTR p_missing_ids, SeqIdPtr anchor_id, Int4 window_size)
{
	SeqIdPtr missing_sip;
	ValNodePtr missing_ids, next, prev;
	Boolean found;


	missing_ids = *p_missing_ids;
	prev = NULL;
	while(missing_ids)
	{
		next = missing_ids->next;
		missing_sip = missing_ids->data.ptrvalue;
		found = FALSE;
		if(map_one_fish_loc(align, msp_list, missing_sip, window_size, FALSE))
			found = TRUE;
		else if(map_one_fish_loc(align, msp_list, missing_sip, window_size, TRUE))
			found = TRUE;

		if(found)
		{
			if(prev == NULL)
				*p_missing_ids = next;
			else
				prev->next = next;
			missing_ids->next = NULL;
			ValNodeFree(missing_ids);
		}
		missing_ids = next;
	}
}
		

/***********************************************************************
*
*	 all the other attempts failed. Try the straight slice
*
***********************************************************************/
static Boolean figure_loc_by_position (ValNodePtr msp_list, ValNodePtr PNTR p_missing_ids, SeqIdPtr anchor_id, ValNodePtr e_align_list, Int4 window_size)
{
	MatchSitePtr msp, anchor_msp;
	SeqLocPtr slp;
	Int4 start, stop, center;
	Int4 m_left, m_right;
	SeqIdPtr sip;
	ValNodePtr curr;
	ValNodePtr vnp;
	SeqAnnotPtr annot;
	SeqAlignPtr align;
	ValNodePtr missing_ids;

	if(msp_list == NULL || *p_missing_ids == NULL)
		return FALSE;
	msp = msp_list->data.ptrvalue;
	start = SeqLocStart(msp->slp);
	stop = SeqLocStop(msp->slp);
	m_left = start;
	m_right = stop;

	if(window_size != -1)
	{
		center = (start + stop)/2;
		start -= window_size/2;
		stop += window_size/2;
	}
		


	/*has both anchor_id and Equiv-align */
	if(anchor_id != NULL && e_align_list != NULL)
	{
		anchor_msp = NULL;
		for(curr = *p_missing_ids; curr != NULL; curr = curr->next)
		{
			sip = curr->data.ptrvalue;
			if(SeqIdMatch(sip, anchor_id))
			{
				anchor_msp = MemNew(sizeof(MatchSite));
				anchor_msp->slp = SeqLocIntNew(start, stop, Seq_strand_plus, sip);
				break;
			}
		}

		if(anchor_msp != NULL)
		{
			vnp = ValNodeNew(NULL);
			vnp->data.ptrvalue = anchor_msp;
			vnp->next = NULL;
			for(curr = e_align_list; curr != NULL; curr = curr->next)
			{
				annot = (SeqAnnotPtr)(curr->data.ptrvalue);
				while(annot)
				{
					if(GetEquivAlignType(annot) == 1)
					{	/*it is a consistent marker */
						align = annot->data;
						load_align_by_consist(align, vnp, p_missing_ids, anchor_id, window_size);
					}
					annot = annot->next;
				}
			}

			ValNodeLink(&msp_list, vnp);
		}
	}

	for(missing_ids = *p_missing_ids; missing_ids != NULL; missing_ids = missing_ids->next)
	{
		sip = missing_ids->data.ptrvalue;
		slp = SeqLocIntNew(start, stop, Seq_strand_plus, sip);
		add_figure_site(msp, m_left, m_right, slp);
	}

	return TRUE;

			
}



static Boolean LoadPriorityData (ValNodePtr gbp_list, SeqIdPtr sip, Uint1Ptr priority, Uint1 max_val)
{
	GlobalBspPtr gbp;
	
	while(gbp_list)
	{
		gbp = gbp_list->data.ptrvalue;
		if(gbp && gbp->bsp != NULL)
		{
			if(BioseqMatch(gbp->bsp, sip))
			{
				if(gbp->is_seqindex_map)
					*priority = SEQINDEX_VAL;
				else
					*priority = MIN((Uint1)(gbp->priority), max_val);
				return TRUE;
			}
		}
		gbp_list = gbp_list->next;
	}

	return FALSE;
}


/*
*
*	use the msp_list->choice to load the search priority for 
*	a marker. The maximum choice is set to be SEQINDEX_VAL-1
*/
static void AddPriorityToMatchList(ValNodePtr msp_list, ValNodePtr gbp_list)
{
	Int2 num;
	ValNodePtr curr, vnp;
	MatchSitePtr msp;
	FigureSitePtr fsp;
	SeqIdPtr sip;

	num = 0;
	for(curr = gbp_list; curr != NULL; curr = curr->next)
		++num;
	num = MIN(num, SEQINDEX_VAL-1);

	for(vnp = msp_list; vnp != NULL; vnp = vnp->next)
	{
		sip = NULL;
		msp = vnp->data.ptrvalue;
		if(msp->slp != NULL)
		{
			sip = SeqLocId(msp->slp);
			msp->priority = (Uint1)num;
			if(sip != NULL)
				LoadPriorityData (gbp_list, sip, &(msp->priority), (Uint1)num);
			for(fsp = msp->fsp; fsp != NULL; fsp = fsp->next)
			{
				fsp->priority = (Uint1)num;
				if(fsp->slp != NULL)
				{
					sip = SeqLocId(fsp->slp);
					if(sip != NULL)
						LoadPriorityData (gbp_list, sip, &(fsp->priority), (Uint1)num);
				}
			}
		}
	}
}
			
		
	
/*
*
*	check into the list of the Bioseqs (gbp_list) to see if 
*	there is any sequence index bsp. If there is, find its location 
*	by duplicating the genethon map
*
*/
static void AddSeqIndexLoc (ValNodePtr msp_list, ValNodePtr gbp_list)
{
	ValNodePtr curr;
	GlobalBspPtr gbp;
	ValNodePtr vnp;
	Boolean found;
	MatchSitePtr msp, g_msp;
	FigureSitePtr fsp, n_fsp, g_fsp;
	
	/*check if the seqindex-loc is in msp_list. If not, add whatever it 
	  is in genethon map */
	for(curr = gbp_list; curr != NULL; curr = curr->next)
	{
		gbp = curr->data.ptrvalue;
		if(gbp->is_seqindex_map == TRUE && gbp->bsp != NULL)
		{
			found = FALSE;
			g_msp = NULL;
			g_fsp = NULL;
			for(vnp = msp_list; !found && vnp != NULL; vnp = vnp->next)
			{
				msp = vnp->data.ptrvalue;
				if(msp != NULL)
				{
					if(msp->slp != NULL)
					{
						if(BioseqMatch(gbp->bsp, 
							SeqLocId(msp->slp)))
						{
							found = TRUE;
							break;
						}
						else if(is_db_map(SeqLocId(msp->slp), "GENETHON"))
							g_msp = msp;
					}
					for(fsp = msp->fsp; fsp != NULL; fsp = fsp->next)
					{
						if(BioseqMatch(gbp->bsp, 
							SeqLocId(fsp->slp)))
							found = TRUE;
						else if(is_db_map(SeqLocId(fsp->slp), "GENETHON"))
							g_fsp = fsp;
					}
				}
			}

			/* duplicate the match site from genethon map*/
			if(!found && (g_msp != NULL || g_fsp != NULL))
			{
				fsp = NULL;
				n_fsp = MemNew(sizeof(FigureSite));
				n_fsp->priority = SEQINDEX_VAL;
				if(g_msp != NULL)
				{
					n_fsp->m_left = SeqLocStart(g_msp->slp);
					n_fsp->m_right = SeqLocStop(g_msp->slp);
					n_fsp->slp = SeqLocIntNew(SeqLocStart(g_msp->slp), 
						SeqLocStop(g_msp->slp), Seq_strand_plus, 
						gbp->bsp->id);
					if(g_msp->fsp == NULL)
						g_msp->fsp = n_fsp;
					else
						fsp = g_msp->fsp;
				}
				else
				{
					n_fsp->m_left = g_fsp->m_left;
					n_fsp->m_right = g_fsp->m_right;
					n_fsp->slp = SeqLocIntNew(SeqLocStart(g_fsp->slp), 
						SeqLocStop(g_fsp->slp), Seq_strand_plus, 
						gbp->bsp->id);
					fsp = g_fsp;
				}
				if(fsp != NULL)
				{
					while(fsp->next != NULL)
						fsp = fsp->next;
					fsp->next = n_fsp;
				}
			}
						
		}
	}

}


static void change_anchor_to_front(ValNodePtr msp_list, SeqIdPtr anchor_id)
{
	ValNodePtr curr, anchor_vnp;
	MatchSitePtr msp;

	anchor_vnp = NULL;
	for(curr = msp_list; curr != NULL; curr = curr->next)
	{
		msp = curr->data.ptrvalue;
		if(SeqIdMatch(SeqLocId(msp->slp), anchor_id))
		{
			anchor_vnp = curr;
			break;
		}
	}

	if(anchor_vnp != NULL)
	{
		if(anchor_vnp != msp_list)
		{
			msp = msp_list->data.ptrvalue;
			msp_list->data.ptrvalue = anchor_vnp->data.ptrvalue;
			anchor_vnp->data.ptrvalue = msp;
		}
	}
}

static int LIBCALLBACK SortMspPriority(VoidPtr ptr1, VoidPtr ptr2)
{
  MatchSitePtr msp1, msp2;
  ValNodePtr   vnp1;
  ValNodePtr   vnp2;

  if (ptr1 != NULL && ptr2 != NULL) {
    vnp1 = *((ValNodePtr PNTR) ptr1);
    vnp2 = *((ValNodePtr PNTR) ptr2);
    if (vnp1 != NULL && vnp2 != NULL) {
	msp1 = vnp1->data.ptrvalue;
	msp2 = vnp2->data.ptrvalue;
	if(msp1->priority > msp2->priority)
		return 1;
	else if(msp1->priority < msp2->priority)
		return -1;
	else
		return 0;
    }
  }

  return 0;
}

static int LIBCALLBACK SortMissingIdPriority(VoidPtr ptr1, VoidPtr ptr2)
{
  ValNodePtr   vnp1;
  ValNodePtr   vnp2;

  if (ptr1 != NULL && ptr2 != NULL) {
    vnp1 = *((ValNodePtr PNTR) ptr1);
    vnp2 = *((ValNodePtr PNTR) ptr2);
    if (vnp1 != NULL && vnp2 != NULL) {
	if(vnp1->choice > vnp2->choice)
		return 0;
	else if(vnp1->choice < vnp2->choice)
		return -1;
	else
		return 0;
    }
  }
  return 0;
}

/*********************************************************
*
*	return a list of msp_list which include the 
*	directly matched markers together with the 
*	indirectly matched ones and their left-right 
*	margins on the master sequence
*	the choice of msp_list stores the search priority
*	if(fish_first), check the FISH mapping data
*
**********************************************************/
static ValNodePtr find_gene_location (GlobalDrawPtr gdraw_p, 
	GeneDataPtr g_data, Int4 window_size, Boolean fish_first)
{
	SeqIdPtr sip;
	ValNodePtr curr, list;
	SeqAnnotPtr annot;
	GlobalBspPtr gbp;
	SeqLocPtr slp;
    Int4 start, stop;
	Uint1 strand;
	SeqIdPtr anchor_id;

	ValNodePtr missing_ids;
	SeqAlignPtr align;
	StdSegPtr ssp;
	ValNodePtr msp_list;
	MatchSitePtr msp;
	

	/*trying to figure out the locations of mapped genes*/
	msp_list = NULL;
	while(g_data)
	{
		if(g_data->location != NULL)
		{
			sip = SeqLocId(g_data->location);
			if(!is_id_in_msp_list(sip, msp_list))
			{
				start = SeqLocStart(g_data->location);
				stop = SeqLocStop(g_data->location);
				strand = Seq_strand_plus;
				slp = SeqLocIntNew(start, stop, strand, sip);
				msp = MemNew(sizeof(MatchSite));
				msp->slp = slp;
				ValNodeAddPointer(&msp_list, 0, msp);
			}
		}
		g_data = g_data->next;
	}
	if(msp_list == NULL)
		return NULL;
	/*adding the priority and then sort the msp_list by the priority*/
	AddPriorityToMatchList(msp_list, gdraw_p->gbp_list);
	if(msp_list->next != NULL)
		msp_list = ValNodeSort(msp_list, SortMspPriority);

	missing_ids = NULL;	/*store the Seq-ids that can not be found*/
	anchor_id = NULL;
	for(curr = gdraw_p->gbp_list; curr != NULL; curr = curr->next)
	{
		/*exclude the seqindex map*/
		gbp = (GlobalBspPtr)(curr->data.ptrvalue);
		if(gbp && gbp->is_seqindex_map == FALSE)
		{
			if(!is_id_in_msp_list(gbp->bsp->id, msp_list))
				ValNodeAddPointer(&missing_ids, gbp->priority, gbp->bsp->id);
			if(gbp->bsp != NULL && anchor_id == NULL)
				anchor_id = find_anchor_id(gbp->bsp);
		}
	}

	/*it has not missed anything yet*/
	if(missing_ids == NULL)
	{
		AddPriorityToMatchList(msp_list, gdraw_p->gbp_list);
		AddSeqIndexLoc (msp_list, gdraw_p->gbp_list);
		return msp_list;
	}
	else
		missing_ids = ValNodeSort(missing_ids, SortMissingIdPriority);

	/* if(anchor_id != NULL)
		change_anchor_to_front(msp_list, anchor_id); */


	if(!fish_first)
	{
		/*check the Equiv-align to see if there is any direct link .
		  the FISH alignment is EXCLUDED */
		for(list = gdraw_p->equiv_align_list; 
			list != NULL && missing_ids != NULL; list = list->next)
		{
			annot = (SeqAnnotPtr)(list->data.ptrvalue);
			while(annot)
			{
				if(GetEquivAlignType(annot) != 4)
				{	/* a non-FISH alignment */
					align = annot->data;
					while(align)
					{
						if(align->segtype == 3)
						{
							ssp = align->segs;
							if(load_new_align(ssp, msp_list, &missing_ids, FALSE))
								break;
						}
						align = align->next;
					}
					if(missing_ids == NULL)
						break;
				}
				annot = annot->next;
			}
		}
	}

	/* the direct approaches failes, do the approximation */

	/*check anything that were mapped as something between the FISH map */
	for(list = gdraw_p->equiv_align_list; 
		list != NULL && missing_ids != NULL; list = list->next)
	{

		annot = (SeqAnnotPtr)(list->data.ptrvalue);
		while(annot)
		{
			if(GetEquivAlignType(annot) == 4)
			{	
				if(load_align_by_fish(annot, msp_list, &missing_ids, window_size))
					break;
			}
			annot = annot->next;
		}
	} 

	if(missing_ids == NULL)
	{
		AddPriorityToMatchList(msp_list, gdraw_p->gbp_list);
		AddSeqIndexLoc (msp_list, gdraw_p->gbp_list);
		return msp_list;
	}

	for(list = gdraw_p->equiv_align_list; 
		list != NULL && missing_ids != NULL; list = list->next)
	{
		annot = (SeqAnnotPtr)(list->data.ptrvalue);
		while(annot)
		{
			if(GetEquivAlignType(annot) != 4)
			{	/* a non-FISH alignment */
				align = annot->data;
				while(align)
				{
					if(align->segtype == 3)
					{
						ssp = align->segs;
						if(load_new_align(ssp, msp_list, &missing_ids, TRUE))
							break;
					}
					align = align->next;
				}
				if(missing_ids == NULL)
					break;
			}
			annot = annot->next;
		}
	}

	/* try to find the interval by the flanking the consistent markers */
	for(list = gdraw_p->equiv_align_list; 
		list != NULL && missing_ids != NULL; list = list->next)
	{

		annot = (SeqAnnotPtr)(list->data.ptrvalue);
		while(annot)
		{
			if(GetEquivAlignType(annot) == 1)	/*check the consistent only*/
			{	
				align = annot->data;
				load_align_by_consist (align, msp_list, &missing_ids, anchor_id, window_size);
				break;
			}
			annot = annot->next;
		}
	}

	if(missing_ids != NULL)
	{
		figure_loc_by_position (msp_list, &missing_ids, anchor_id, 
			gdraw_p->equiv_align_list, window_size);
		if(missing_ids != NULL)
			ValNodeFree(missing_ids);
	}
	AddPriorityToMatchList(msp_list, gdraw_p->gbp_list);
	AddSeqIndexLoc (msp_list, gdraw_p->gbp_list);
	return msp_list;
}


static ValNodePtr cvt_loc_for_single_gene PROTO((ValNodePtr msp_list, Int4 window_size));


/*********************************************************
*
*	map the location of the current slp to the 
*	list of slps based on consistent alignment, FISH
*	etc
*
*********************************************************/
ValNodePtr map_one_location_to_slp_list(SeqEntryPtr sep, SeqLocPtr slp, Int4 window_size)
{
	SeqAnnotPtr annot;
	SeqAlignPtr align;
	StdSegPtr ssp;
	ValNodePtr bsp_list;
	ValNodePtr vnp;
	BioseqPtr bsp;
	ValNodePtr e_align;
	ValNodePtr slp_list;
	SeqLocPtr t_loc;
	ValNodePtr missing_ids;
	MatchSitePtr msp;
	ValNodePtr msp_list;
	Int4 start, stop;
	SeqIdPtr sip;

	bsp_list = NULL;
	e_align = NULL;
	slp_list = NULL;
	missing_ids = NULL;

	make_Bioseq_list (sep, &bsp_list, &e_align);
	for(vnp = bsp_list; vnp != NULL; vnp = vnp->next)
	{
		bsp = (BioseqPtr)(vnp->data.ptrvalue);
		if(!BioseqMatch(bsp, SeqLocId(slp)))
			ValNodeAddPointer(&missing_ids, 0, bsp->id);
	}
	ValNodeFree(bsp_list);

	/* there is only one sequence */
	if(missing_ids == NULL)
	{
		ValNodeFree(e_align);
		t_loc = SeqLocIntNew(SeqLocStart(slp), SeqLocStop(slp), SeqLocStrand(slp), SeqLocId(slp));
		ValNodeAddPointer(&slp_list, 0, t_loc);
		return slp_list;
	}

	if(e_align == NULL)
	{
		start = SeqLocStart(slp);
		stop = SeqLocStop(slp);
		for(vnp = missing_ids; vnp != NULL; vnp = vnp->next)
		{
			sip = vnp->data.ptrvalue;
			t_loc = SeqLocIntNew(start, stop, Seq_strand_plus, sip);
			ValNodeAddPointer(&slp_list, 0, t_loc);
		}
		ValNodeFree(missing_ids);
	}

	msp = MemNew(sizeof(MatchSite));
	msp->slp = slp;
	msp_list = NULL;
	ValNodeAddPointer(&msp_list, 0, msp);
	
	for(vnp = e_align; vnp != NULL && missing_ids != NULL; vnp = vnp->next)
	{
		annot = (SeqAnnotPtr)(vnp->data.ptrvalue);
		while(annot)
		{
			if(GetEquivAlignType(annot) != 4)
			{	/* a non-FISH alignment */
				align = annot->data;
				while(align)
				{
					if(align->segtype == 3)
					{
						ssp = align->segs;
						if(load_new_align(ssp, msp_list, &missing_ids, FALSE))
							break;
					}
					align = align->next;
				}
				if(missing_ids == NULL)
					break;
			}
			annot = annot->next;
		}
	}

	/* the direct approaches failes, do the approximation */

	/*check anything that were mapped as something between the FISH map */
	for(vnp = e_align; vnp != NULL && missing_ids != NULL; vnp = vnp->next)
	{

		annot = (SeqAnnotPtr)(vnp->data.ptrvalue);
		while(annot)
		{
			if(GetEquivAlignType(annot) == 4)
			{	
				if(load_align_by_fish(annot, msp_list, &missing_ids, window_size))
					break;
			}
			annot = annot->next;
		}
	} 


	/* try to find the interval by the flanking the consistent markers */
	for(vnp = e_align; vnp != NULL && missing_ids != NULL; vnp = 
		vnp->next)
	{
		annot = (SeqAnnotPtr)(vnp->data.ptrvalue);
		while(annot)
		{
			if(GetEquivAlignType(annot) == 1)	/*check the consistent only*/
			{	
				align = annot->data;
				load_align_by_consist (align, msp_list, &missing_ids, SeqLocId(slp), window_size);
				break;
			}
			annot = annot->next;
		}
	}

	if(missing_ids != NULL)
	{
		figure_loc_by_position (msp_list, &missing_ids, NULL, 
			e_align, window_size);
		if(missing_ids != NULL)
			ValNodeFree(missing_ids);
	}
	ValNodeFree(e_align);

	return cvt_loc_for_single_gene(msp_list, window_size);
}

static ValNodePtr extract_slp_for_bsp(ValNodePtr PNTR slp_list, BioseqPtr bsp)
{
	ValNodePtr curr, prev;
	SeqLocPtr slp;

	prev = NULL;
	curr = *slp_list;
	while(curr)
	{
		slp = curr->data.ptrvalue;
		if(BioseqMatch(bsp, SeqLocId(slp)))
		{
			if(prev != NULL)
				prev->next = curr->next;
			else
				*slp_list = curr->next;
			curr->next = NULL;
			return curr;
		}
		else
			prev = curr;
		curr = curr->next;
	}

	return NULL;
}
/*****************************************************************
*
*	sort the slp_list so that the display order is the same 
*	as the order of the Bioseqs
*
*****************************************************************/
static ValNodePtr sort_slp_display_order(ValNodePtr PNTR slp_list, GlobalDrawPtr gdraw_p)
{
	GlobalBspPtr gbp;
	ValNodePtr curr;
	ValNodePtr head = NULL;
	ValNodePtr t_list;

	for(curr = gdraw_p->gbp_list; curr != NULL; curr = curr->next)
	{
		gbp = (GlobalBspPtr)(curr->data.ptrvalue);
		t_list = extract_slp_for_bsp(slp_list, gbp->bsp);
		if(t_list != NULL)
			ValNodeLink(&head, t_list);
	}

	return head;
}


/**********************************************************
*
*	merge_slp_list(): if there are two genes, find the 
*	maximum range
*
***********************************************************/
static Boolean merge_slp_list(ValNodePtr PNTR gene_slp_list, ValNodePtr slp_list)
{
	SeqLocPtr slp;
	SeqLocPtr g_slp;
	ValNodePtr curr;
	SeqIntPtr sint, g_sint;

	if(*gene_slp_list == NULL)
	{
		*gene_slp_list = slp_list;
		return FALSE;
	}
	for(curr = *gene_slp_list; curr != NULL; curr = curr->next)
	{
		g_slp = curr->data.ptrvalue;
		g_sint = g_slp->data.ptrvalue;
		slp = slp_list->data.ptrvalue;
		sint = slp->data.ptrvalue;
		g_sint->from = MIN(g_sint->from, sint->from);
		g_sint->to = MAX(g_sint->to, sint->to);
		slp_list = slp_list->next;
	}
	return TRUE;
}
		
static void link_gene_data(GeneDataPtr PNTR head, GeneDataPtr gdp)
{
	GeneDataPtr curr;

	if(*head == NULL)
		*head = gdp;
	else
	{
		curr = *head;
		while(curr->next != NULL)
			curr = curr->next;
		curr->next = gdp;
	}
}

static void adjust_slp_list(ValNodePtr slp_list)
{
	Int4 max_len = -1;
	SeqLocPtr slp;
	SeqIntPtr sint;
	Int4 offset;
	BioseqPtr bsp;
	ValNodePtr curr;

	for(curr = slp_list; curr != NULL; curr = curr->next)
	{
		slp = curr->data.ptrvalue;
		max_len = MAX(max_len, SeqLocLen(slp));
	}

	for(curr = slp_list; curr != NULL; curr = curr->next)
	{
		slp = curr->data.ptrvalue;
		offset = max_len - SeqLocLen(slp);
		sint = slp->data.ptrvalue;
		bsp = BioseqFind(sint->id);
		sint->from = MAX(0, sint->from - offset/2);
		sint->to = MIN(bsp->length-1, sint->to + offset/2);

	}
}
		

static Boolean adjust_align_offset(SeqIntPtr sint, Int4Ptr align_offset, Int4Ptr offset)
{
	if(*align_offset == 0  && *offset <= 0)
		return FALSE;
	if(ABS(*align_offset) < (*offset))
	{
		if(*align_offset < 0)
			sint->to -= *align_offset;
		else
			sint->from -= *align_offset;
		*offset -= ABS(*align_offset);
		*align_offset = 0;
	}
	else
	{
		if(*align_offset < 0)
		{
			sint->to += *offset;
			*align_offset += *offset;
		}
		else
		{
			sint->from -= *offset;
			*align_offset -= *offset;
		}

		*offset = 0;
	}

	if(*offset > 0)
	{
		sint->from -= (*offset/2);
		sint->to += (*offset/2);
	}
	return TRUE;

}
		
	
	
/***********************************************************
*
*	if(free_seq_loc == TRUE), all the Seq-locs are freed
*	else Not freed
*
***********************************************************/
static ValNodePtr free_map_site_list(ValNodePtr msp_list, Boolean free_seq_loc)
{
	MatchSitePtr msp;
	FigureSitePtr fsp, fsp_next;
	ValNodePtr next;

	while(msp_list)
	{
		next = msp_list->next;
		msp = msp_list->data.ptrvalue;

		fsp = msp->fsp;
		while(fsp)
		{
			fsp_next = fsp->next;
			if(free_seq_loc && fsp->slp)
				SeqLocFree(fsp->slp);
			MemFree(fsp);
			fsp = fsp_next;
		}

		if(free_seq_loc && msp->slp)
			SeqLocFree(msp->slp);
		MemFree(msp);
		MemFree(msp_list);
		msp_list = next;
	}
	return NULL;
}


static Boolean adjust_this_seqloc (SeqLocPtr slp)
{
	SeqIntPtr sint;
	BioseqPtr bsp;

	sint = slp->data.ptrvalue;
	if(sint == NULL)
		return FALSE;
	bsp = BioseqFind(sint->id);
	if(bsp == NULL)
		return FALSE;

	if(sint->from < 0)
	{
		sint->to += ABS(sint->from);
		sint->from = 0;
	}
	if(sint->to > bsp->length -1)
	{
		sint->from -= (sint->to - (bsp->length -1));
		sint->to = bsp->length -1;
	}

	sint->from = MAX(0, sint->from);
	sint->to = MIN(sint->to, bsp->length-1);
	return TRUE;
}
	
static ValNodePtr make_slp_list_from_msp_list(ValNodePtr msp_list)
{
	ValNodePtr slp_list = NULL;
	MatchSitePtr msp;
	FigureSitePtr fsp;

	while(msp_list)
	{
		msp = msp_list->data.ptrvalue;
		adjust_this_seqloc (msp->slp);
		ValNodeAddPointer(&slp_list, msp->priority, msp->slp);

		for(fsp = msp->fsp; fsp != NULL; fsp = fsp->next)
		{
			adjust_this_seqloc (fsp->slp);
			ValNodeAddPointer(&slp_list, fsp->priority, fsp->slp);
		}
		msp_list = msp_list->next;
	}

	return slp_list;
}
		
/*****************************************************************
*
*	convert msp_list to slp_list for the single gene
*
*****************************************************************/
static ValNodePtr cvt_loc_for_single_gene(ValNodePtr msp_list, Int4 window_size)
{
	ValNodePtr curr;
	MatchSitePtr msp;
	FigureSitePtr fsp;
	SeqLocPtr slp;
	SeqIntPtr sint;
	Int4 m_left, m_right;
	Int4 shift = 0;
	Int4 offset, align_offset, p_align_offset;

	p_align_offset = 0;
	for(curr = msp_list; curr != NULL; curr = curr->next)
	{
		msp = curr->data.ptrvalue;
		slp = msp->slp;
		sint = slp->data.ptrvalue;

		align_offset =0;
		offset = window_size -  (sint->to - sint->from +1);
		if(offset > 0)
		{
			adjust_align_offset(sint, &align_offset, &offset);
			p_align_offset = align_offset;
		}



		m_left = sint->from;
		m_right = sint->to;
		for(fsp = msp->fsp; fsp != NULL; fsp = fsp->next)
		{
			align_offset = (fsp->m_left - m_left) - (m_right - fsp->m_right);
			offset = window_size - SeqLocLen(fsp->slp);
			slp = fsp->slp;
			sint = slp->data.ptrvalue;

			if(offset > 0)
			{
				slp = fsp->slp;
				sint = slp->data.ptrvalue;
				adjust_align_offset(sint, &align_offset, &offset);
			}
		}
	}
	return make_slp_list_from_msp_list(msp_list);
}


static SeqLocPtr find_other_seqloc(ValNodePtr msp_list, SeqIdPtr sip)
{
	MatchSitePtr msp;
	FigureSitePtr fsp;
	Int4 m_left, m_right;

	while(msp_list)
	{
		msp = msp_list->data.ptrvalue;
		if(SeqIdMatch(SeqLocId(msp->slp), sip))
		{
			get_match_range(msp, &m_left, &m_right);
			return SeqLocIntNew(m_left, m_right, Seq_strand_plus, sip);
		}
		else
		{
			for(fsp = msp->fsp; fsp != NULL; fsp = fsp->next)
				if(SeqIdMatch(SeqLocId(fsp->slp), sip))
					return SeqLocIntNew(SeqLocStart(fsp->slp), 
						SeqLocStop(fsp->slp), Seq_strand_plus, sip);
		}

		msp_list = msp_list->next;
	}

	return NULL;
}


static ValNodePtr cvt_loc_for_two_gene(ValNodePtr msp_1_list, ValNodePtr msp_2_list)
{
	ValNodePtr curr;
	MatchSitePtr msp_1;
	FigureSitePtr fsp;
	Int4 m_left, m_right;
	SeqLocPtr slp;
	SeqIntPtr sint;
	ValNodePtr slp_list;

	slp_list = NULL;
	for(curr = msp_1_list; curr != NULL; curr = curr->next)
	{
		msp_1 = curr->data.ptrvalue;
		get_match_range(msp_1, &m_left, &m_right);
		slp = find_other_seqloc(msp_2_list, SeqLocId(msp_1->slp));
		if(slp != NULL)
		{
			sint = slp->data.ptrvalue;
			sint->from = MIN(sint->from, m_left);
			sint->to = MAX(sint->to, m_right);
		}
		else
			slp = SeqLocIntNew(m_left, m_right, Seq_strand_plus, 
				SeqLocId(msp_1->slp));
		adjust_this_seqloc (slp);
		ValNodeAddPointer(&slp_list, 0, slp);
		for(fsp = msp_1->fsp; fsp != NULL; fsp = fsp->next)
		{
			slp = find_other_seqloc(msp_2_list, SeqLocId(fsp->slp));
			if(slp != NULL)
			{
				sint = slp->data.ptrvalue;
				sint->from = MIN(sint->from, SeqLocStart(fsp->slp));
				sint->to = MAX(sint->to, SeqLocStop(fsp->slp));
			}
			else
			{
				slp = fsp->slp;
				fsp->slp = NULL;
			}
			adjust_this_seqloc (slp);
			ValNodeAddPointer(&slp_list, 0, slp);
		}
	}

	return slp_list;
}

static Int4 get_default_window_size(GlobalDrawPtr gdraw_p)
{
	GlobalBspPtr gbp;
	ValNodePtr curr;
	Int4 max_len = 0, val;

	for(curr = gdraw_p->gbp_list; curr != NULL; curr = curr->next)
	{
		gbp = (GlobalBspPtr)(curr->data.ptrvalue);
		max_len = MAX(max_len, gbp->bsp->length);
	}

	val = MIN(max_len, 10000);
	return MAX(max_len/20, val);
}

static Boolean is_legal_band(CharPtr band_name)
{
	if(StrStr(band_name, "cen") || StrStr(band_name, "er"))
		return FALSE;
	else
		return TRUE;
}

static SeqLocPtr find_band_location (BioseqPtr bsp, CharPtr band_name, Uint1Ptr end_band)
{
	Int4 left = -1, right = -1;
	CharPtr name;
	SeqFeatPtr sfp;
	UserObjectPtr uop;
	Boolean has_prev;
	Boolean has_next;

	/*need to go through the whole chain because of the band may separated by 
	  the centromere
	*/
	*end_band = 0;
	has_prev = FALSE;
	has_next = FALSE;
	for(sfp = bsp->seq_ext; sfp != NULL; sfp = sfp->next)
	{
		if(sfp->data.choice == 14)
		{
			uop = sfp->data.value.ptrvalue;
			name = get_band_name(uop);
			if(name != NULL && is_legal_band(name))
			{
				if(StringCmp(name, band_name) == 0)
				{
					if(!has_prev)
						*end_band = 1;	/*the first band*/
					if(left == -1)
					{
						left = SeqLocStart(sfp->location);
						right = SeqLocStop(sfp->location);
					}
					else
					{
						left = MIN(left, SeqLocStart(sfp->location));
						right = MAX(right, SeqLocStop(sfp->location));
					}
				}
				else if(left != -1)	/*already found the band*/
					has_next = TRUE;
				has_prev = TRUE;
			}
		}
	}
	if(*end_band == 0 && has_next == FALSE)
		*end_band = 2;

	if(left == -1 || right == -1)
		return NULL;
	else
		return SeqLocIntNew(left, right, Seq_strand_plus, bsp->id);
}

static ValNodePtr straight_map_location(SeqLocPtr slp, GlobalDrawPtr gdraw_p)
{
	ValNodePtr slp_list = NULL;
	GlobalBspPtr gbp;
	ValNodePtr curr;
	Int4 start, stop;
	SeqLocPtr c_slp;

	start = SeqLocStart(slp);
	stop = SeqLocStop(slp);
	for(curr = gdraw_p->gbp_list; curr != NULL; curr = curr->next)
	{
		gbp = curr->data.ptrvalue;
		if(gbp->bsp != NULL)
		{
			c_slp = SeqLocIntNew(start, stop, Seq_strand_plus, 
				SeqIdFindBest(gbp->bsp->id, 0));
			ValNodeAddPointer(&slp_list, 0, c_slp);
		}
	}

	return slp_list;
}
			
		

/*map a cytogenetic band to a location on the physical map
 *return TRUE for success and FALSE for failure. If the results are 
 *success, gdraw_p->slp will be replaced with the newly mapped slp_list*/
Boolean MapCytoBand (CharPtr band_name, GlobalDrawPtr gdraw_p, BioseqPtr cyto_bsp, Boolean correct_distortion, Uint1Ptr end_band)
{
	ValNodePtr msp_list;
	Int4 window_size;
	GeneDataPtr gdata;
	SeqLocPtr slp;
	ValNodePtr slp_list, t_slp_list;

	if(band_name == NULL || gdraw_p == NULL || cyto_bsp == NULL)
		return FALSE;
	slp = find_band_location (cyto_bsp, band_name, end_band);
	if(slp == NULL)
		return FALSE;
	if(!correct_distortion)
	{
		slp_list = straight_map_location(slp, gdraw_p);
		SeqLocFree(slp);
	}
	else
	{
		window_size = SeqLocLen(slp);
		gdata = MemNew(sizeof(GeneData));
		gdata->location = slp;
		msp_list = find_gene_location(gdraw_p, gdata, window_size, TRUE);
		MemFree(gdata);
		if(msp_list == NULL)
		{
			ErrPostEx(SEV_ERROR, 0, 0, "Fail to find location for band %s", band_name);
			SeqLocFree(slp);
			return  FALSE;
		}
		slp_list = cvt_loc_for_single_gene(msp_list, window_size);
		free_map_site_list(msp_list, FALSE);
	}
	t_slp_list = sort_slp_display_order(&slp_list, gdraw_p);
	
	free_slp_list(slp_list);
	free_slp_list(gdraw_p->slp_list);
	gdraw_p->slp_list = t_slp_list;
	SeqLocFree(slp);
	return TRUE;
}


static Boolean PrintMarkerToFile PROTO((ValNodePtr msp_1_list, ValNodePtr msp_2_list, GeneDataPtr g_data, FILE *fp, Int2 line_len, Boolean is_html));
/************************************************************
*
*	map the one or two genes to a list Seq-locs
*	the slp_list will be stored at gdraw_p->slp_list
*	gdraw_p: the Drawing structure
*	g_list: a list of genes
*	pgdata: store the GeneDataPtr created
*	fp:	if not NULL, dump out the text between a region
*	is_html: if TRUE, dump out a html page for the fp
*
************************************************************/
Boolean map_gene_location (GlobalDrawPtr gdraw_p, ValNodePtr g_list, GeneDataPtr PNTR pgdata, FILE *fp, Boolean is_html)
{
	GeneDataPtr gdata;
	CharPtr str;
	ValNodePtr curr, next;
	ValNodePtr msp_list;
	ValNodePtr msp_1_list, msp_2_list;
	ValNodePtr slp_list,  t_slp_list;
	Int4 window_size;
	

	*pgdata = NULL;
	window_size = get_default_window_size(gdraw_p);

	/* query for one gene */
	if(g_list->next == NULL)
	{
		gdata = create_gene_data(gdraw_p->msp_list, g_list);
		if(gdata == NULL)
		{
			str = g_list->data.ptrvalue;
			if (!is_html) {
				ErrPostEx(SEV_ERROR, 0, 0, "Fail to find gene %s", str);
			}
			/*ValNodeFreeData(g_list); */
			return FALSE;
		}

		msp_list = find_gene_location(gdraw_p, gdata, window_size, FALSE);
		if(msp_list == NULL)
		{
			GeneDataFree(gdata);
			str = g_list->data.ptrvalue;
			if (!is_html) {
				ErrPostEx(SEV_ERROR, 0, 0, "Fail to find location for gene %s", str);
			}
		/*	ValNodeFreeData(g_list);*/
			return FALSE;
		}
		*pgdata = gdata;
		slp_list = cvt_loc_for_single_gene(msp_list, window_size);
		free_map_site_list(msp_list, FALSE);
		t_slp_list = sort_slp_display_order(&slp_list, gdraw_p);

		free_slp_list(slp_list);
		free_slp_list(gdraw_p->slp_list);
		gdraw_p->slp_list = t_slp_list;
		return TRUE;
	}
	else
	{
		curr = g_list;
		msp_1_list = NULL;
		msp_2_list = NULL;
		while(curr)
		{
			next = curr->next;
			curr->next = NULL;
			gdata = create_gene_data(gdraw_p->msp_list, curr);
			if(gdata == NULL)
			{
				str = g_list->data.ptrvalue;
				if (!is_html) {
					ErrPostEx(SEV_ERROR, 0, 0, "Fail to find gene %s", str);
				}
			}
			else
			{
				msp_list = find_gene_location(gdraw_p, gdata, window_size, FALSE);
				if(msp_list == NULL)
				{
					GeneDataFree(gdata);
					str = curr->data.ptrvalue;
					if (!is_html) {
						ErrPostEx(SEV_ERROR, 0, 0, "Fail to find location for gene %s", str);
					}
				}
				else
					link_gene_data(pgdata, gdata);
				if(curr == g_list)	/*the first gene */
					msp_1_list = msp_list;
				else
					msp_2_list = msp_list;
			}
			curr->next = next;
			curr = next;
		}

		if(msp_1_list == NULL && msp_2_list == NULL)
		{
			/*ValNodeFreeData(g_list);*/
			return FALSE;
		}

		/*deal with the case that only one gene was found */
		msp_list = NULL;
		if(msp_1_list == NULL)
			msp_list = msp_2_list;
		if(msp_2_list == NULL)
			msp_list = msp_1_list;
		slp_list = NULL;
		window_size = get_default_window_size(gdraw_p);
		if(msp_list != NULL)
		{
			slp_list = cvt_loc_for_single_gene(msp_list, window_size);
			free_map_site_list(msp_list, FALSE);
		}
		else
		{
			if(fp != NULL)
			{
				if(is_html)
					fprintf(fp, "Summary for the Markers Between <B>%s</B> and <B>%s</B>\n", 
					(CharPtr)(g_list->data.ptrvalue), 
					(CharPtr)(g_list->next->data.ptrvalue));
				else
					fprintf(fp, "Summary for the Markers Between %s and %s\n", 
					(CharPtr)(g_list->data.ptrvalue), 
					(CharPtr)(g_list->next->data.ptrvalue));
				PrintMarkerToFile (msp_1_list, msp_2_list, *pgdata, fp, 70, is_html);
			}

			slp_list = cvt_loc_for_two_gene(msp_1_list, msp_2_list);
			free_map_site_list(msp_1_list, TRUE);
			free_map_site_list(msp_2_list, TRUE);
		}

		if(slp_list == NULL)
			return FALSE;

		t_slp_list = sort_slp_display_order(&slp_list, gdraw_p);
		free_slp_list(slp_list);
		free_slp_list(gdraw_p->slp_list);
		gdraw_p->slp_list = t_slp_list;
		return TRUE;
	}
}

static Boolean is_id_in_list (ValNodePtr id_list, SeqIdPtr sip)
{
	while(id_list)
	{
		if(SeqIdMatch((SeqIdPtr)(id_list->data.ptrvalue), sip))
			return TRUE;
		id_list = id_list->next;
	}

	return FALSE;
}

static Boolean PrintMarkerToFile (ValNodePtr msp_1_list, ValNodePtr msp_2_list, GeneDataPtr g_data, FILE *fp, Int2 line_len, Boolean is_html)
{
	ValNodePtr used_ids;
	ValNodePtr curr_1, curr_2;
	MatchSitePtr msp_1, msp_2;
	SeqIdPtr sip;
	Boolean found;
	SeqLocPtr temp_loc;
	Int4 left, right;
	FigureSitePtr fsp, fsp_2;


	used_ids = NULL;
	if(msp_1_list == NULL || msp_2_list == NULL || fp == NULL)
		return FALSE;

	/*show the regions that were mapped by both markers first */
	for(curr_1 = msp_1_list; curr_1 != NULL; curr_1 = curr_1->next)
	{
		msp_1 = curr_1->data.ptrvalue;
		sip = SeqLocId(msp_1->slp);
		found = FALSE;
		for(curr_2 = msp_2_list; curr_2 != NULL; curr_2 = curr_2->next)
		{
			msp_2 = curr_2->data.ptrvalue;
			if(SeqIdMatch(sip, SeqLocId(msp_2->slp)))
			{
				found = TRUE;
				break;
			}
		}
		if(found)
		{
			ValNodeAddPointer(&used_ids, 0, sip);
			left = MIN(SeqLocStart(msp_1->slp), SeqLocStart(msp_2->slp));
			right = MAX(SeqLocStop(msp_1->slp), SeqLocStop(msp_2->slp));
			OrderInt4 (&left, &right);
			temp_loc = SeqLocIntNew(left, right, Seq_strand_plus, sip);
			print_genome_interval(temp_loc, g_data, is_html, line_len, fp);
			SeqLocFree(temp_loc);
		}
	}

	/*show the regions that one side were mapped, the other computed*/
	for(curr_1 = msp_1_list; curr_1 != NULL; curr_1 = curr_1->next)
	{
		msp_1 = curr_1->data.ptrvalue;
		sip = SeqLocId(msp_1->slp);

		if(!is_id_in_list (used_ids, sip))
		{
			found = FALSE;
			for(curr_2 = msp_2_list; !found && curr_2 != NULL; curr_2 = curr_2->next)
			{
				msp_2 = curr_2->data.ptrvalue;
				for(fsp = msp_2->fsp; !found && fsp != NULL; fsp = fsp->next)
				{
					if(SeqIdMatch(sip, SeqLocId(fsp->slp)))
					{
						found = TRUE;
						break;
					}
				}
			}
			if(found)
			{
				ValNodeAddPointer(&used_ids, 0, sip);
				left = MIN(SeqLocStart(msp_1->slp), SeqLocStart(fsp->slp));
				right = MAX(SeqLocStop(msp_1->slp), SeqLocStop(fsp->slp));
				OrderInt4 (&left, &right);
				temp_loc = SeqLocIntNew(left, right, Seq_strand_plus, sip);
				print_genome_interval(temp_loc, g_data, is_html, line_len, fp);
				SeqLocFree(temp_loc);
			}
		}
	}


	/*show the regions that one side were mapped, the other computed*/
	for(curr_1 = msp_2_list; curr_1 != NULL; curr_1 = curr_1->next)
	{
		msp_1 = curr_1->data.ptrvalue;
		sip = SeqLocId(msp_1->slp);

		if(!is_id_in_list (used_ids, sip))
		{
			found = FALSE;
			for(curr_2 = msp_1_list; !found && curr_2 != NULL; curr_2 = curr_2->next)
			{
				msp_2 = curr_2->data.ptrvalue;
				for(fsp = msp_2->fsp; !found && fsp != NULL; fsp = fsp->next)
				{
					if(SeqIdMatch(sip, SeqLocId(fsp->slp)))
					{
						found = TRUE;
						break;
					}
				}
			}
			if(found)
			{
				ValNodeAddPointer(&used_ids, 0, sip);
				left = MIN(SeqLocStart(msp_1->slp), SeqLocStart(fsp->slp));
				right = MAX(SeqLocStop(msp_1->slp), SeqLocStop(fsp->slp));
				OrderInt4 (&left, &right);
				temp_loc = SeqLocIntNew(left, right, Seq_strand_plus, sip);
				print_genome_interval(temp_loc, g_data, is_html, line_len, fp);
				SeqLocFree(temp_loc);
			}
		}
	}


	/*show the region that both ends were deduced */
		/*show the regions that one side were mapped, the other computed*/
	for(curr_1 = msp_1_list; curr_1 != NULL; curr_1 = curr_1->next)
	{
		msp_1 = curr_1->data.ptrvalue;
		for(fsp = msp_1->fsp; fsp != NULL; fsp = fsp->next)
		{
			sip = SeqLocId(fsp->slp);
			if(!is_id_in_list (used_ids, sip))
			{
				found = FALSE;
				for(curr_2 = msp_2_list; !found && curr_2 != NULL; curr_2 = curr_2->next)
				{
					msp_2 = curr_2->data.ptrvalue;
					for(fsp_2 = msp_2->fsp; !found && fsp_2 != NULL; fsp_2 = fsp_2->next)
					{
						if(SeqIdMatch(sip, SeqLocId(fsp_2->slp)))
						{
							found = TRUE;
							break;
						}
					}
				}
				if(found)
				{
					ValNodeAddPointer(&used_ids, 0, sip);
					left = MIN(SeqLocStart(fsp->slp), SeqLocStart(fsp_2->slp));
					right = MAX(SeqLocStop(fsp->slp), SeqLocStop(fsp_2->slp));
					OrderInt4(&left, &right);
					temp_loc = SeqLocIntNew(left, right, Seq_strand_plus, sip);
					print_genome_interval(temp_loc, g_data, is_html, line_len, fp);
					SeqLocFree(temp_loc);
				}
			}
		}
	}

	ValNodeFree(used_ids);
	return TRUE;
}


/*
*
*	slp is the Seq-loc for a contig on the map. If include_amb, 
*	all the ambiguously mapped YAC on the contig will be 
*	included. Otherwise, they will be filterd from the list 
*	of the alignnode. db can be either YAC_MIT or YAC_NHGRI
*
*/
ValNodePtr LoadContigAlign (SeqLocPtr slp, Boolean include_amb, Uint1 db, SeqIdPtr chr_id)
{
	ValNodePtr aligns;
	CollectAlignOption ca_option;
	Uint2 entityID;
	BioseqPtr bsp;

	if(slp == NULL || db == 0)
		return NULL;

	bsp = BioseqLockById(SeqLocId(slp));
	if(bsp == NULL)
		return NULL;
	entityID = ObjMgrGetEntityIDForPointer((Pointer)bsp);
	if(entityID == 0)
	{
		BioseqUnlock(bsp);
		return NULL;
	}
	set_option_for_collect_align(&ca_option, 20, COLLECT_HISTORY);
	ca_option.only_history = FALSE;
	ca_option.slabel_format = PRINTID_TEXTID_ACCESSION;
	ca_option.nointerval = FALSE;
	ca_option.no_sort = FALSE;
	ca_option.flat_insert = FALSE;
	aligns = collect_anpnode_with_option(&ca_option, slp, entityID, COLLECT_HISTORY, OBJ_SEQALIGN, NULL, NULL, FALSE);
	if(aligns == NULL || !alignode_has_alignments (aligns))
	{
		BioseqUnlock(bsp);
		if(aligns != NULL)
			FreeAlignNode(aligns);
		return NULL;
	}

	/*set the left offset to 0 */
	CollectSegMapSTSAlign(entityID, aligns, slp, 0);

	/*clean up the ambiguous YACs*/
	if(!include_amb || chr_id != NULL)
		CleanUpAmbiguousYAC (&aligns, db, chr_id);

	BioseqUnlock(bsp);
	return aligns;
}

/*
*
*	print out the alignnode for YACs as a tabular form
*	contig name is the name of the contig where the YAC is 
*	mapped. band_name is the name of the chromosome band 
*	that it mapped to
*
*/
Boolean PrintContigAlign (CharPtr band_name, CharPtr contig_name, ValNodePtr anp_list, FILE *fp)
{
	AlignNodePtr anp;
	AnnotInfoPtr info;
	Char db[21];

	if(anp_list == NULL || fp == NULL)
		return FALSE;

	db[0] = '\0';
	while(anp_list)
	{
		if(anp_list->choice == OBJ_SEQANNOT)
		{
			info = anp_list->data.ptrvalue;
			StringCpy(db, info->annotDB);
		}
		else
		{
			anp = anp_list->data.ptrvalue;
			if(band_name != NULL)
				fprintf(fp, "%s\t", band_name);
			if(contig_name != NULL)
				fprintf(fp, "%s\t", contig_name);
			/*the status of the YAC*/
			if(db[0] != '\0')
				fprintf(fp, "%s\t", db);
			/*the name of the YAC*/
			fprintf(fp, "%s\n", anp->label);
		}
		anp_list = anp_list->next;
	}

	return TRUE;
}


/*
#################################################################
#
#	functions related to printing out the YACs to an html page
#
#################################################################
*/
/*
*	yac links to the WI WWW site
*/
#define WI_LINK "http://www.genome.wi.mit.edu:80/"
#define WI_YAC_LINK "http://www.genome.wi.mit.edu:80/cgi-bin/contig/yac_info?yac="
#define WI_CONTIG_LINK "http://www.genome.wi.mit.edu:80/cgi-bin/contig/lookup_contig?database=release&contig="

/*
*	YAC links to the CEPH WWW Site
*/
#define CEPH_YAC_LINK "http://www.cephb.fr/cgi-bin/http_infoclone?"
#define CEPH_YAC_ORDER "http://www.cephb.fr/services/YAC_CLONES.html"

/*
*	YAC links to GDB
*/
#define GDB_LINK "http://gdbwww.gdb.org/gdb-bin/genera/genera/hgd/GenomicSegment?!action=query&displayName="

/*
*	YAC links to the NCHGR WWW site
*/
#define NHGRI_LINK "http://www.nhgri.nih.gov/DIR/GTB/CHR7/YAC-STS/"
#define NHGRI_CONTIG_LINK "http://www.nhgri.nih.gov/DIR/GTB/CHR7/YAC-STS/"
#define ENTREZ_LINK "http://www.ncbi.nlm.nih.gov/htbin-post/Entrez/query?uid=1234471&form=6&db=n&Dopt=g"

/*
*	Chromosome links to the OMIM Gene Map
*/
#define OMIM_MAP_LINK "http://www.ncbi.nlm.nih.gov/htbin-post/Omim/getmap?chromosome="

/*ceph order form*/
#define ORDER_CEPH_LINK "http://www.cephb.fr/services/YAC_CLONES.html"

/*order from Research Genetics*/
#define ORDER_RG_LINK "http://www.resgen.com/"

/*eric green's page on GDB*/
#define ORDER_NHGRI_LINK "http://gdbwww.gdb.org/gdb-bin/genera/genera/registry/DBObject?!key=REG%3A96803&!sub=0"

#define GENETHON_LINK "http://www.genethon.fr/genethon_en.html"
#define TXMAP_LINK "http://www.ncbi.nlm.nih.gov/cgi-bin/SCIENCE96/msrch2?CHR="

static Boolean print_header_for_center(FILE *fp, Uint1 db)
{
	Char center_name[101];
	CharPtr link;
	Char name[21];

	if(db == 0)
		return FALSE;
	if(db == YAC_MIT)
	{
		StringCpy(center_name, "WI/MIT");
		link = WI_LINK;
		StringCpy(name, "WI_MAP");
	}
	else if(db == YAC_NHGRI)
	{
		StringCpy(center_name, "NHGRI");
		link = NHGRI_LINK;
		StringCpy(name, "NHGRI_MAP");
	}
		
	/*print out the header for the genome centers*/	
	fprintf(fp, "<CENTER><H3>\n");
	fprintf(fp, "<A href=\"#TOC\"><IMG align=top border=0 SRC=%sup_arrow.gif></A>\n", GIF_DIR);
	fprintf(fp, "<A NAME=\"%s\">%s Physical Map</A></H3>\n", name, center_name);
	fprintf(fp, "</CENTER>\n");

	/*make the header for the table*/
	fprintf(fp, "<CENTER>\n");
	fprintf(fp, "<TABLE border cellspacing=3 celllpading=3>\n");
	fprintf(fp, "<TR>\n");
	fprintf(fp, "<TH><B>YAC Name</B></TH>\n");
	fprintf(fp, "<TH><B>FISH?</B></TH>\n");
	fprintf(fp, "<TH><B>No. STSs</B></TH>\n");
	fprintf(fp, "<TH><B>CEPH</B></TH>\n");
	fprintf(fp, "<TH><B>GDB</B></TH>\n");
	fprintf(fp, "<TH><B>Status</B></TH>\n");
	fprintf(fp, "<TH><B>Contig</B></TH>\n");
	if(db == YAC_NHGRI)
		fprintf(fp, "<TH><B>B/PAC</B></TH>\n");
	fprintf(fp, "</TR>\n");

	return TRUE;
}

static void create_OMIM_link(CharPtr band_name, CharPtr buf)
{
	sprintf(buf, "%s%s", OMIM_MAP_LINK, band_name);
}

static void create_GDB_YAC_link(CharPtr yac_name, CharPtr buf)
{
	sprintf(buf, "%s%s", GDB_LINK, yac_name);
}

static void create_CEPH_YAC_link(CharPtr yac_name, CharPtr buf)
{
	Char temp[101];
	Int4 i;
	CharPtr str;

	/*CEPH link is case sensative*/
	i = 0;
	for(str = yac_name; str != NULL && *str != '\0'; ++str)
	{
		if(IS_UPPER(*str))
			temp[i++] = TO_LOWER(*str);
		else
			temp[i++] = *str;
	}
	temp[i] = '\0';
	sprintf(buf, "%s%s", CEPH_YAC_LINK, temp);
}

static void strip_Ctg_for_Contig_name(CharPtr Contig_name, CharPtr new_name)
{
	CharPtr str;
	Int4 i;

	/*the input for Contig_name is Ctg[sWSS1361]*/
	if(StringNCmp(Contig_name, "Ctg", 3) == 0)
	{
		i = 0;
		for(str = Contig_name+4; str != NULL && *str != '\0' && *str != ']'; ++str)
			new_name[i++] = *str;
		new_name[i] = '\0';
	}
	else
		StringCpy(new_name, Contig_name);
}



static Boolean FindEricContigMap(CharPtr contig_name, FILE *contig_map_fp, CharPtr link_name)
{
	Char name[31];
	Char alpha_name[31];
	Char mod_name[31];

	/*format is like that: sWSS1361 A, delimited by space*/
	rewind(contig_map_fp);

	strip_Ctg_for_Contig_name(contig_name, mod_name);

	while(fscanf(contig_map_fp, "%s %s\n", name, alpha_name) != EOF)
	{
		if(StringCmp(name, mod_name) == 0)
		{
			sprintf(link_name, "CONTIG%s", alpha_name);
			return TRUE;
		}
	}

	return FALSE;
}
		
static Boolean create_CONTIG_link(CharPtr contig_name, Uint1 db, CharPtr buf, FILE *contig_map_fp)
{
	CharPtr link;
	Char link_name[31];

	if(db == YAC_MIT)	/*I have reverted order of database and contig*/ 
	{
		link = WI_CONTIG_LINK;
		sprintf(buf, "%s%s", link, contig_name);
		return TRUE;
	}
	else if(FindEricContigMap(contig_name, contig_map_fp, link_name))
	{
		link = NHGRI_CONTIG_LINK;
		sprintf(buf, "%s%s", link, link_name);
		return TRUE;
	}

	return FALSE;
}

static void create_YAC_link(CharPtr yac_name, Uint1 db, CharPtr buf)
{
	CharPtr link;

	if(db == YAC_MIT)	/*I have reverted order of database and contig*/ 
		link = WI_YAC_LINK;
	else
		return;
	sprintf(buf, "%s%s", link, yac_name);
}

static Int4 get_YAC_sts_num(AlignNodePtr anp)
{
	Int4 num = 0;
	AlignSegPtr asp;
	ValNodePtr vnp;

	for(asp = anp->segs; asp != NULL; asp = asp->next)
	{
		for(vnp = asp->mismatch; vnp != NULL; vnp = vnp->next)
		{
			if(vnp->choice != MISMATCH_OPEN)
				++num;
		}
	}
	return num;
}

typedef struct fish_info{
	Char band_name[21];
	Char yac_name[21];
	Char author[21];
	Int4 muid;
}FishInfo, PNTR FishInfoPtr;

/*
*
*	create a list of FISH mapped information
*
*/
static ValNodePtr CreateFishInfoList (CharPtr band_name)
{
	FILE *fp;
	FishInfoPtr fip;
	FishInfo fi;
	ValNodePtr list;
	long muid;
	Int4 val;

	fp = FileOpen(FISH_MAP_FILE, "r");
	if(fp == NULL)
		return NULL;
	list = NULL;
	while(fscanf(fp, "%s\t%s\t%s\t%ld\n", fi.band_name, fi.yac_name, fi.author, &muid) != EOF)
	{
		fi.muid = muid;
		val = StringCmp(fi.band_name, band_name);
		if(val == 0)
		{
			fip = MemNew(sizeof(FishInfo));
			MemCopy(fip, &fi, sizeof(FishInfo));
			ValNodeAddPointer(&list, 0, fip);
		}
		else if(val > 0)
			return list;
	}

	return list;
}
	

/*
*	if the YAC extend to the previous band, change the label to *label. 
*	if it extend to the next band, change the label to label*
*/
static void modify_label_for_partial(AlignNodePtr anp, CharPtr n_label)
{
	if(anp->extremes.l_trunc)
		sprintf(n_label, "*%s", anp->label);
	else
		StringCpy(n_label, anp->label);
	if(anp->extremes.r_trunc)
		StringCat(n_label, "*");
}

static void create_muid_link(CharPtr buf, Int4 muid)
{
	sprintf(buf, "http://www.ncbi.nlm.nih.gov/htbin-post/Entrez/query?uid=%ld&form=6&db=m&Dopt=r", (long) muid);
}

static FishInfoPtr check_fish_mapping(CharPtr yac_name, ValNodePtr fish_list)
{
	FishInfoPtr fip;

	while(fish_list)
	{
		fip = fish_list->data.ptrvalue;
		if(StringICmp(fip->yac_name, yac_name) == 0)
			return fip;
		fish_list = fish_list->next;
	}

	return NULL;
}

typedef struct yac_bac{
        CharPtr YAC;
        CharPtr www_add;
}YACBac, PNTR YACBacPtr;

static YACBacPtr bin_search_yac(CharPtr yac_name, ValNodePtr PNTR list, Int4 low, Int4 high)
{
	YACBacPtr sbp;
	Int4 mid;
	ValNodePtr vnp;
	Int4 val;

	if(low > high)
		return NULL;
	mid = (low + high)/2;
	vnp = list[mid];
	sbp = vnp->data.ptrvalue;
	
	val = StringCmp(yac_name, sbp->YAC);
	if(val == 0)
		return sbp;
	if(val < 0)
		return bin_search_yac(yac_name, list, low, mid-1);
	else
		return bin_search_yac(yac_name, list, mid+1, high);
}

static void PrintHTMLContigAlign(CharPtr band_name, CharPtr contig_name, 
	ValNodePtr anp_list, FILE *fp, Uint1 db, ValNodePtr fish_list, ValNodePtr PNTR yac_list, Int4 yac_num)
{
	AnnotInfoPtr info;
	AlignNodePtr anp;
	Char status[101];
	Char buf[1001];
	Char mod_name[101];
	Int4 sts_num;
	FILE* contig_map_fp;	/*map the Eric Green's contig name to CONTIGA-V*/
	FishInfoPtr fip;
	YACBacPtr ybp;

	if(db == YAC_NHGRI)
		contig_map_fp = FileOpen(ERIC_CONTIG_MAP, "r");
	else
		contig_map_fp = NULL;

	status[0] = '\0';
	while(anp_list)
	{
		if(anp_list->choice == OBJ_SEQANNOT)
		{
			info = anp_list->data.ptrvalue;
			if(info->annotDB[0] != '\0')
				StringCpy(status, info->annotDB);
		}
		else
		{
			anp = anp_list->data.ptrvalue;

		fprintf(fp, "<TR>\n");
		/*YAC*/
		modify_label_for_partial(anp, mod_name);
		fip = check_fish_mapping(anp->label, fish_list);
		if(db == YAC_MIT)
		{
			create_YAC_link(anp->label, db, buf);
			if(fip != NULL)
				fprintf(fp, "<TD><A href=%s><B><FONT color=red>%s</FONT></B></A></TD>\n", buf, mod_name);
			else
				fprintf(fp, "<TD><A href=%s>%s</A></TD>\n", buf, mod_name);
		}
		else /*there is no link to YAC for the NCHGRI map yet*/
		{
			if(fip != NULL)
				fprintf(fp, "<TD><B><FONT color=red>%s</FONT></B></TD>\n", mod_name);
			else
				fprintf(fp, "<TD>%s</TD>\n", mod_name);
		}

		if(fip == NULL)
			fprintf(fp, "<TD><CENTER>-</CENTER></TD>\n");
		else
		{
			if(fip->muid != 0)
			{
				create_muid_link(buf, fip->muid);
				fprintf(fp, "<TD><CENTER><A href=%s>%s</A></CENTER></TD>\n", buf, fip->author);
			}
			else
				fprintf(fp, "<TD><CENTER>%s</CENTER></TD>\n", fip->author);
		}
		/*No. of STS*/
		sts_num = get_YAC_sts_num(anp);
		fprintf(fp, "<TD align=center>%ld</TD>\n", (long) sts_num);

		/*CEPH orders*/
		/* fprintf(fp, "<TD align=center><A href=%s><IMG border=0 SRC=%smark2.gif></A></TD>\n", CEPH_YAC_ORDER, GIF_DIR); */

		/*CEPH YAC link*/
		if(db == YAC_MIT)
		{
			create_CEPH_YAC_link(anp->label, buf);
			fprintf(fp, "<TD align=center><A href=%s><IMG border=0 SRC=%smark1.gif></A></TD>\n",buf,  GIF_DIR);
		}
		else
		{
			fprintf(fp, "<TD align=center>-</TD>\n");
		}
		/*GDB YAC link*/
		create_GDB_YAC_link(anp->label, buf);
		fprintf(fp, "<TD align=center><A href=%s><IMG border=0 SRC=%smark3.gif></A></TD>\n", buf, GIF_DIR);

		/*Status*/
		if(status[0] != '\0')
			fprintf(fp, "<TD>%s</TD>\n", status);
		else	/*no status, empty field*/
			fprintf(fp, "<TD></TD>\n");

		/*Contig*/
		if(create_CONTIG_link(contig_name, db, buf, contig_map_fp))
			fprintf(fp, "<TD><A href=%s>%s</A></TD>\n", buf, contig_name);
		else
			fprintf(fp, "<TD>%s</TD>\n", contig_name);

		/*the YAC/BAC table*/
		if(db == YAC_NHGRI)
		{
			ybp = NULL;
			if(yac_list != NULL && yac_num > 0)
				ybp = bin_search_yac(anp->label, yac_list, 0, yac_num-1);
			if(ybp == NULL)
				fprintf(fp, "<TD align=center>-</TD>\n");
			else
				fprintf(fp, "<TD align=center><A href=%s target=\"Jinghui\"><IMG border=0 SRC=%smark1.gif></A></TD>\n", ybp->www_add,  GIF_DIR);
		}

		/*end of the table*/
		fprintf(fp, "</TR>\n");
		}

		anp_list = anp_list->next;
	}
	if(contig_map_fp != NULL)
		FileClose(contig_map_fp);
}

		

static ValNodePtr PNTR load_yac_bac_info(CharPtr f_name, Int4Ptr num)
{
	Int4 i;
	ValNodePtr list;
	ValNodePtr PNTR p_list;
	FILE *fp;
	YACBacPtr sbp;
	Char STS[101];
	Char www_add[101];

	*num = 0;
	fp = FileOpen(f_name, "r");
	if(fp == NULL)
		return NULL;
	list = NULL;
	i = 0;
	while(fscanf(fp, "%s\t%s\n", STS, www_add) != EOF)
	{
		sbp = MemNew(sizeof(YACBac));
		sbp->YAC= StringSave(STS);
		sbp->www_add = StringSave(www_add);
		ValNodeAddPointer(&list, 0, sbp);
		++i;
	}
	FileClose(fp);

	if(i == 0)
		return NULL;
	*num = i;

	p_list = MemNew((size_t)i * sizeof(ValNodePtr));
	i = 0;
	while(list)
	{
		p_list[i++] = list;
		list = list->next;
	}

	return p_list;
}

static void free_yac_bac_list (ValNodePtr PNTR list, Int4 num)
{
	ValNodePtr vnp;
	YACBacPtr  ypp;
	Int4 i;

	for(i = 0; i<num; ++i)
	{
		vnp = list[i];
		ypp = vnp->data.ptrvalue;
		MemFree(ypp->YAC);
		MemFree(ypp->www_add);
		MemFree(ypp);
		MemFree(vnp);
	}

	MemFree(list);
}

	


static Boolean print_contig_info (ValNodePtr slp_list, 
	CharPtr band_name, SeqIdPtr chr_id, Boolean include_amb, FILE *fp)
{
	Boolean first;
	SeqLocPtr slp;
	ValNodePtr anp_list;
	Uint1 db;
	Char contig_name[21];
	ValNodePtr fish_list;
	ValNodePtr PNTR yac_list;
	Int4 yac_num;

	if(chr_id == NULL)
		return FALSE;
	db = FindContigDB(chr_id);
	if(db == 0)	/*only the WI map and the Eric Green's map has the data*/
		return FALSE;

	if(db == YAC_NHGRI)
		yac_list = load_yac_bac_info(YAC2BAC_FILE, &yac_num);
	else
		yac_list = NULL;
		
	fish_list = CreateFishInfoList (band_name);
	first = TRUE;
	while(slp_list)
	{
		slp = slp_list->data.ptrvalue;
		anp_list = LoadContigAlign (slp, include_amb, db, chr_id);
		if(anp_list != NULL)
		{
			if(first)
			{
				/*print out the header and the table header*/
				print_header_for_center(fp, db);
				first = FALSE;
			}
			MuskSeqIdWrite(SeqLocId(slp), contig_name, 20, 
				PRINTID_TEXTID_ACCESSION, FALSE, FALSE);
			PrintHTMLContigAlign(band_name, contig_name, anp_list, fp, db, fish_list, yac_list, yac_num);
			FreeAlignNode(anp_list);
		}
		slp_list = slp_list->next;
	}
	if(fish_list != NULL)
		ValNodeFreeData(fish_list);

	if(yac_list != NULL)
		free_yac_bac_list (yac_list, yac_num);

	if(!first)
	{
		fprintf(fp, "</TABLE>\n");
		fprintf(fp, "</CENTER>\n");
		return TRUE;
	}
	else
		return FALSE;
}

void print_contig_html_color (FILE *fp, CharPtr band_name)
{
	fprintf(fp, "<BODY bgcolor=\"#FFFFFF\" text=\"#000000\" link=\"#0000f0\" vlink=\"#6000b0\" alink=\"#f00000\">\n");
}

static Boolean convert_center_name_to_gifname (CharPtr center_name, CharPtr gif_name)
{
	if(StringICmp(center_name, "Baylor") == 0)
	{
		StringCpy(gif_name, "Baylor.gif");
		return TRUE;
	}
	if(StringICmp(center_name, "WashU") == 0)
	{
		StringCpy(gif_name, "WashU.gif");
		return TRUE;
	}
	if(StringICmp(center_name, "Sanger") == 0)
	{
		StringCpy(gif_name, "Sanger.gif");
		return TRUE;
	}

	return FALSE;
}

static Boolean add_link_to_seqcenter(FILE *fp, CharPtr band_name)
{
	FILE *s_fp;
	Char center_name[21];
	Char band[21];
	Char www_add[201];
	Int4 val;
	Boolean first = TRUE;

	s_fp = FileOpen(SEQ_CENTER_LINK_FILE, "r");
	if(s_fp == NULL)
		return FALSE;
	while(fscanf(s_fp, "%s\t%s\t%s\n", band, center_name, www_add) != EOF)
	{
		val = StringCmp(band, band_name);
		if(val == 0)
		{
			if(first)
			{
				fprintf(fp, "<LI>Sequencing Effort\n");
				fprintf(fp, "<UL>\n");
				first = FALSE;
			}
			fprintf(fp, "<LI><A href=%s target=\"Jinghui\"><I>%s</I></A>", www_add, center_name);
		}
		else if(val > 0)
		{
			FileClose(s_fp);
			if(first == FALSE)
				fprintf(fp, "</UL>\n");
			return TRUE;
		}
	}

	FileClose(s_fp);
	if(first == FALSE)
		fprintf(fp, "</UL>\n");
	return TRUE;
}

static Boolean get_tx_interval (CharPtr tx_buf, CharPtr buf)
{
	CharPtr str;

	str = StrStr(tx_buf, "IVL=");
	if(str == NULL)
		return FALSE;
	StringCpy(buf, str+4);
	return TRUE;
}

/*
*
*	Print out the H1 header for each band. this includes the 
*	link to OMIM and the large sequencing centers
*
*/
void PrintBandHeader(FILE *fp, CharPtr band_name, Uint1 db_option, Boolean graphic, CharPtr tx_buf)
{
	Char buf[101];

	fprintf(fp, "<H1>\n");
	fprintf(fp, "Results for %s", band_name);
	fprintf(fp, "</H1>\n");

	/*print out the table of content*/
	fprintf(fp, "<HR>\n");
	fprintf(fp, "<h3><A NAME=\"TOC\"><strong>TABLE OF CONTENTS</strong></A></h3>\n");
	/*print the list of table of content: physical map, genetic map, transcript map*/
	fprintf(fp, "<B><UL>\n");

	/*physical map for YAC information*/
	if(db_option & YAC_MIT || db_option & YAC_NHGRI)
	{
		fprintf(fp, "<LI>Physical Map\n");
		fprintf(fp, "<UL>\n");
		if(db_option & YAC_MIT)
			fprintf(fp, "<LI><A href=\"#WI_MAP\">WI/MIT STS YAC Content Map</A>\n");
		if(db_option & YAC_NHGRI)
			fprintf(fp, "<LI><A href=\"#NHGRI_MAP\">NHGRI STS YAC Content Map</A>\n");
		if(graphic)
			fprintf(fp, "<LI><A href=\"#YAC_GRAPHIC\">Graphic Overview</A>\n");
		fprintf(fp, "</UL>\n");
	}

	if(db_option & GENETIC_GENETHON || db_option & GENETIC_CHLC)
	{
		fprintf(fp, "<LI>Genetic Map\n");
		fprintf(fp, "<UL>\n");

		if(db_option & GENETIC_GENETHON)
			fprintf(fp, "<LI><A href=\"#GENETHON_MAP\">Genethon Map</A>\n");
		if(db_option & GENETIC_CHLC)
			fprintf(fp, "<LI><A href=\"#CHCL_MAP\">CHLC Map</A>\n");
		fprintf(fp, "</UL>\n");
	}

	if(tx_buf && tx_buf[0] != '\0')
	{
		get_tx_interval (tx_buf, buf);
		fprintf(fp, "<LI>Transcript Map on Interval %s\n", buf);
		fprintf(fp, "<UL>\n");
		fprintf(fp, "<LI><A href=%s target=\"Jinghui\"><I>International RH Cosortium</I></A>\n", tx_buf);
		fprintf(fp, "</UL>\n");
	}

	create_OMIM_link(band_name, buf);
	fprintf(fp, "<LI>Disease Phenotype: ");
	fprintf(fp, "<UL>\n");
	fprintf(fp, "<LI><A href=%s target=\"Jinghui\"><I>OMIM</I></A>\n", buf);
	fprintf(fp, "</UL>\n");
		
	/*link to the sequencing center*/
	add_link_to_seqcenter(fp, band_name);
	fprintf(fp, "<LI><A href=\"#dblink\">Related Database</A>\n");

	fprintf(fp, "</UL></B>\n");

	fprintf(fp, "<HR>\n");
	fprintf(fp, "<B>Note: pressing the <IMG align=top border=0 SRC=%sup_arrow.gif> symbol moves back to table of content</B>\n", GIF_DIR);

}

void PrintDataBaseLink(FILE *fp, Uint1 db_option)
{
	fprintf(fp, "<HR>");
	fprintf(fp, "<A href=\"#TOC\"><IMG align=top border=0 SRC=%sup_arrow.gif></A>\n", GIF_DIR);
	fprintf(fp, "<B><A NAME=\"dblink\">Related Databases</A></B>\n");
	fprintf(fp, "<UL><B>\n");
	/*link to the Genome Mapping Centers*/
	fprintf(fp, "<LI>Genome Mapping Center:");
	fprintf(fp, "    ");
	if(db_option & YAC_MIT)
		fprintf(fp, "<A href=%s target=\"Jinghui\"><I>WI/MIT</I></A>\n", WI_LINK);
	if(db_option & YAC_NHGRI)
		fprintf(fp, "<A href=%s target=\"Jinghui\"><I>NHGRI</I></A>\n", NHGRI_LINK);
	if(db_option & GENETIC_GENETHON)
		fprintf(fp, "<A href=%s target=\"Jinghui\"><I>Genethon</I></A>\n", GENETHON_LINK);
	if(db_option & YAC_MIT || db_option & YAC_NHGRI) 
	{
		fprintf(fp, "<LI>YAC Resources:\n");
		if(db_option & YAC_MIT)
		{
			fprintf(fp, " <A href=%s target=Jinghui><I>CEPH</I></A>\n", ORDER_CEPH_LINK);
			fprintf(fp, " <A href=%s target=Jinghui><I>Research Genetics</I></A>\n", ORDER_RG_LINK);
		}
		if(db_option & YAC_NHGRI)
		{
			fprintf(fp, " <A href=%s target=Jinghui><I>NHGRI</I></A>\n", ORDER_NHGRI_LINK);
		}
	}

	fprintf(fp, "</B></UL>\n");
}
	

			
/*
*
*	print out the html page that displays the YACs for the selected 
*	contigs
*	chr_id: id of the mapping center
*	contig_list: the list of the Seq-loc for contigs
*	band_name: the name of the chromosome band
*	first: if TRUE, print out the HTML header, else don't
*	if(include_amb): including the ambiguous YACs
*
*/ 	
Boolean PrintContigForOneMap(SeqIdPtr chr_id, ValNodePtr contig_list, 
		CharPtr band_name, Boolean include_amb, BoolPtr pfirst, FILE *fp)
{

	if(contig_list == NULL || fp == NULL)
		return FALSE;

	if(*pfirst)
	{
		print_contig_html_color (fp, band_name);
		PrintBandHeader(fp, band_name, 7, FALSE, NULL);
		*pfirst = FALSE;
	}
	fprintf(fp, "<HR>\n"); 
	return print_contig_info (contig_list, band_name, chr_id, include_amb, fp);
}


static Boolean is_genethon_map(SeqIdPtr sip)
{
	DbtagPtr db_tag;

	if(sip->choice == SEQID_GENERAL)
	{
		db_tag = sip->data.ptrvalue;
		if(db_tag->db && StringCmp(db_tag->db, "GENETHON") == 0)
			return TRUE;
	}
	return FALSE;
}



static Boolean is_framework_marker(UserObjectPtr uop)
{
	ObjectIdPtr oip;
	UserFieldPtr ufp;


	while(uop)
	{
		oip = uop->type;
		if(oip && oip->str != NULL)
		{
			if(StringCmp(oip->str, "MapMarkerInfo") == 0)
			{
				ufp = uop->data;
				while(ufp)
				{
					oip = ufp->label;
					if(StringCmp(oip->str, "Marker Type") == 0)
					{
						if(ufp->choice == 2)
						{
							if( ufp->data.intvalue == FRAME_WORK)
								return TRUE;
						}
					}
					ufp = ufp->next;
				}
			}
		}
		uop = uop->next;
	}

	return FALSE;
}

static Boolean marker_has_db (GeneRefPtr grp, CharPtr db, CharPtr name)
{
	ValNodePtr vnp;
	DbtagPtr db_tag;
	ObjectIdPtr oip;

	for(vnp = grp->db; vnp != NULL; vnp = vnp->next)
	{
		db_tag = vnp->data.ptrvalue;
		if(StringCmp(db_tag->db, db) == 0)
		{
			oip = db_tag->tag;
			StringCpy(name, oip->str);
			return TRUE;
		}
	}

	return FALSE;
}

static Boolean get_end_markers(SeqLocPtr slp, BioseqPtr bsp, 
		CharPtr first_name, CharPtr last_name)
{
	SeqFeatPtr sfp;
	SeqFeatPtr l_sfp, r_sfp;
	Int4 left, right;
	GeneRefPtr grp;

	first_name[0] = '\0';
	last_name[0] = '\0';
	l_sfp = NULL;
	r_sfp = NULL;
	left = -1;
	right = -1;

	for(sfp = bsp->seq_ext; sfp != NULL; sfp = sfp->next)
	{
		if(SeqLocCompare(sfp->location, slp) != SLC_NO_MATCH)
		{
			if(left == -1 || SeqLocStart(sfp->location) < left)
			{
				left = SeqLocStart(sfp->location);
				l_sfp = sfp;
			}
			if(right == -1 || SeqLocStop(sfp->location) > right)
			{
				right = SeqLocStop(sfp->location);
				r_sfp = sfp;
			}
		}
	}

	if(l_sfp == NULL || r_sfp == NULL)
		return FALSE;
	else
	{
		grp = l_sfp->data.value.ptrvalue;
		marker_has_db (grp, "GDB", first_name);

		grp = r_sfp->data.value.ptrvalue;
		marker_has_db (grp, "GDB", last_name);
		return TRUE;
	}
}

static Int4 get_uid_for_accession(CharPtr acc)
{
	SeqIdPtr sip;
	BioseqPtr bsp;
	Int4 uid;

	sip = gb_id_make(NULL, acc);
	if(sip == NULL)
		return 0;
	bsp = BioseqLockById(sip);
	SeqIdFree(sip);
	if(bsp == NULL)
		return 0;
	uid = 0;
	for(sip = bsp->id; sip != NULL; sip = sip->next)
	{
		if(sip->choice == SEQID_GI)
		{
			uid = sip->data.intvalue;
			break;
		}
	}
	BioseqUnlock(bsp);
	return uid;
}

static void print_accession(CharPtr acc, CharPtr buf)
{
	Int4 uid;

	uid = get_uid_for_accession(acc);
	if(uid == 0)
		StringCpy(buf, acc);
	else
		sprintf(buf, "<A href=http://www.ncbi.nlm.nih.gov/htbin-post/Entrez/query?uid=%ld&form=6&db=n&Dopt=g>%s</A>", (long) uid, acc);
}


/*
*
*	make the links to the transcript map
*
*/
Boolean MakeTxLink(SeqLocPtr chr_slp, CharPtr buf, Uint1 chr, Uint1 end_band)
{
	BioseqPtr bsp;
	CharPtr link;
	Char name[101];
	Char first_name[101], last_name[101];

	if(chr_slp == NULL || buf == NULL)
		return FALSE;
	if(!is_genethon_map(SeqLocId(chr_slp)))
		return FALSE;
	bsp = BioseqFind(SeqLocId(chr_slp));
	if(bsp == NULL)
		return FALSE;

	/*link to UNIGENE*/
	link = TXMAP_LINK;
	if(chr == 23)
		StringCpy(name, "X");
	else
		sprintf(name, "%d", chr);
	if(!get_end_markers(chr_slp, bsp, 
		first_name, last_name))
	{
		sprintf(buf, "%s%s",link, name);
	}
	else
	{
		if(end_band == 1)
			sprintf(buf, "%s%s&IVL=pTel-%s", link, name, last_name);
		else if(end_band == 2)
			sprintf(buf, "%s%s&IVL=%s-qTel", link, name, first_name);
		else
		{
			if(StringCmp(first_name, last_name) == 0)
				sprintf(buf, "%s%s&IVL=%s", link, name, first_name);
			else
				sprintf(buf, "%s%s&IVL=%s-%s", link, name, first_name, last_name);
		}
	}

	return TRUE;
}




/*
*
*	print the genethon marker to the HTML page
*	it will provide links to the transcript map based on the first
*	and the last genethon markers. if(end_band == 1), it is the first 
*	band to the pTel, if(end_band == 2), it is the last band before
*	qTel. Otherwise, it is an internal band
*
*/
Boolean PrintGeneticMarkerForOneMap(SeqLocPtr chr_slp, Boolean first, 
		CharPtr band_name, Uint1 end_band, Uint1 chr, FILE *fp)
{
	BioseqPtr bsp;
	Char buf[1001];
	SeqFeatPtr sfp;
	GeneRefPtr grp;
	CharPtr link;
	NumberingPtr np = NULL;	/*to convert kb to cM*/
	Char cM_label[101];
	Boolean framework;
	Char html_name[21];
	Char name[101];
	

	if(chr_slp == NULL || fp == NULL)
		return FALSE;
	if(!is_genethon_map(SeqLocId(chr_slp)))
		return FALSE;
	bsp = BioseqFind(SeqLocId(chr_slp));
	if(bsp == NULL)
		return FALSE;
	np = getBioseqNumbering(bsp);
	if(np == NULL)
		return FALSE;

	if(first)
	{
		print_contig_html_color (fp, band_name);
		fprintf(fp, "<H1>Results for %s</H1>\n", band_name);
	}

	/*print out the header for Genethon Map*/
	fprintf(fp, "<HR>\n"); 
	StringCpy(buf, "Genethon Genetic Linkage Map");
	link = GENETHON_LINK;
	StringCpy(html_name, "GENETHON_MAP");
	/*print out the header for the genome centers*/	
	fprintf(fp, "<H3>\n");
	fprintf(fp, "<CENTER>\n");
	fprintf(fp, "<A href=\"#TOC\"><IMG align=top border=0 SRC=%sup_arrow.gif></A>\n", GIF_DIR);
	fprintf(fp, "<A NAME=\"%s\">%s</A>\n", html_name, buf);
	fprintf(fp, "</CENTER></H3>\n");

	
	/*make the header for the table*/
	fprintf(fp, "<CENTER>\n");
	fprintf(fp, "<TABLE border cellspacing=3 celllpading=3>\n");
	fprintf(fp, "<TR>\n");
	/* fprintf(fp, "<TH><B>Cyto Band</B></TH>\n"); */
	fprintf(fp, "<TH><B>Marker</B></TH>\n");
	fprintf(fp, "<TH><B>cM Position</B></TH>\n");
	fprintf(fp, "<TH><B>DSeg Num</B></TH>\n");
	fprintf(fp, "<TH><B>Accession</B></TH>\n");
	fprintf(fp, "</TR>\n");

	for(sfp = bsp->seq_ext; sfp != NULL; sfp = sfp->next)
	{
		if(SeqLocCompare(sfp->location, chr_slp) != SLC_NO_MATCH)
		{
			if(map_unit_label(SeqLocStart(sfp->location), np, cM_label, FALSE))
			{
				framework = is_framework_marker(sfp->ext);
				grp = sfp->data.value.ptrvalue;

				/*print a row for the table*/

				/*the AFM number*/
				if(framework)
					fprintf(fp, "<TD><B>%s</B></TD>\n", grp->locus);
				else
					fprintf(fp, "<TD>%s</TD>\n", grp->locus);

				/*print out the cM position*/
				fprintf(fp, "<TD align=center>%s</TD>\n", cM_label);


				/*print out the DSeg number*/
				if(marker_has_db (grp, "GDB", name))
				{
					create_GDB_YAC_link(name, buf);
					fprintf(fp, "<TD><A href=%s>%s</A></TD>\n", buf, name);
				}
				else
					fprintf(fp, "<TD>-</TD>\n");

				if(marker_has_db(grp, "GenBank", name))
				{
					print_accession(name, buf);
					fprintf(fp, "<TD>%s</TD>\n", buf);
				}
				else
					fprintf(fp, "<TD>-</TD>\n");
				fprintf(fp, "</TR>\n");
			}
		}
	}

	fprintf(fp, "</TABLE>\n");
	fprintf(fp, "</CENTER>\n");
	return TRUE;
}


