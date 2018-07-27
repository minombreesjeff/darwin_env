#include <mconsist.h>
#include <maputil.h>

/****************************************************************************
*
*	get_anchor_coordinates(h_align, anchor_id, sip, x_anchor_pos, x_pos, num)
*
*	load the positions of framework marker of the current sequence and its 
*	corresponding positions in the anchor map (normally the NCBI sequence) into 
*	two arays to extrapolate the positions for map integration
*	h_align:	the alignment of consistent markers (a.k.a good alignment)
*	anchor_id:	the Seq-id for the anchor map (normally, the NCBI sequence map)
*	sip:		the Seq-id for the current sequence
*	x_anchor_pos: the array to store the positions on the anchor map
*	x_pos:		  the array to store the positions on the current map. each 
*				  member of x_anchor_pos and x_pos is a matching pair
*				  both arrays are ordered
*	num:		  the total number of framework markers in the current map. 
*				  This is also the size of x_anchor_pos and x_pos
*
*	return TRUE for success and FALSE for fail
*
****************************************************************************/ 
Boolean get_anchor_coordinates(SeqAlignPtr h_align, SeqIdPtr anchor_id, SeqIdPtr sip, Int4Ptr PNTR x_anchor_pos, Int4Ptr PNTR x_pos, Uint2Ptr num)
{
	StdSegPtr ssp;
	SeqLocPtr slp;
	SeqIdPtr id;
	Int4 n, i, j;
	Int4 a_pos, t_pos;
	SeqAlignPtr align;
	Int4Ptr anchor_pos, pos;
	Boolean change;
	Int4 temp;

	*x_anchor_pos = NULL;
	*x_pos = NULL;
	*num = 0;

	align = h_align;
	if(align == NULL)
		return FALSE;

	/* count the number of the consistent markers for the 
	current sip */
	n = 0;
	while(align)
	{
		if(align->segtype == 3)
		{	/*Std-segs*/
			ssp = align->segs;
			for(slp = ssp->loc; slp != NULL; slp = slp->next)
			{
				id = SeqLocId(slp);
				if(SeqIdForSameBioseq(id, sip))
				{
					++n;
					break;
				}
			}
		}
		align = align->next;
	}

	if(n == 0)
		return FALSE;
	anchor_pos = MemNew((size_t)n * sizeof(Int4));
	pos = MemNew((size_t)n * sizeof(Int4));

	/*load the data*/
	n = 0;
	align = h_align;
	while(align)
	{
		if(align->segtype == 3)
		{
			ssp = align->segs;
			a_pos = -1;
			t_pos = -1;
			for(slp = ssp->loc; slp != NULL; slp = slp->next)
			{
				id = SeqLocId(slp);
				if(SeqIdForSameBioseq(id, sip))
					t_pos = (SeqLocStart(slp) + SeqLocStop(slp))/2;
				else if(SeqIdForSameBioseq(id, anchor_id))
					a_pos = SeqLocStart(slp);
			}

			/*each time, load a matching pair*/
			if(t_pos >= 0 && a_pos >=0)
			{
				anchor_pos[n] = a_pos;
				pos[n] = t_pos;
				++n;
			}
		}
		align = align->next;
	}

	/*sort the array*/
	change = TRUE;
	for(i = 0; i<n-1 && change == TRUE; ++i)
	{
		change = FALSE;
		for(j = 0; j<n-i-1; ++j)
		{ 
			if(anchor_pos[j] > anchor_pos[j+1])
			{
				change = TRUE;
				temp = anchor_pos[j+1];
				anchor_pos[j+1] = anchor_pos[j];
				anchor_pos[j] = temp;

				temp = pos[j+1];
				pos[j+1] = pos[j];
				pos[j] = temp;
			}
		}
	}
				
	/*check the linear relationship between anchor_pos and pos*/
	temp = pos[0];
	for(i = 1, j= 1; i<n; ++i)
	{
		if(pos[i] >= temp)
		{
			temp = pos[i];
			pos[j] = temp;
			anchor_pos[j] = anchor_pos[i];
			++j;
		}
	}
	*x_anchor_pos = anchor_pos;
	*x_pos = pos;
	*num = (Uint2)j;
	return TRUE;
}


/****************************************************************************
*
*	find_this_position_by_anchor(anchor_pos, pos, num, this_pos, length)
*	map the position on the current map to the anchor map (sequence map)
*	anchor_pos, pos: the two arrays of matching positions on the anchor mark 
*					 and the current map for the consistent markers
*	num:	number of the consistent markers on the current map. (same as the 
*			size of the anchor marker)
*	this_pos:	the position on the current map
*	length:		the length of the anchor map
*
*	return the position mapped to the anchor marker
*
*****************************************************************************/
Int4 find_this_position_by_anchor (Int4Ptr anchor_pos, Int4Ptr pos, Uint2 num, Int4 this_pos, Int4 length)
{
	Uint2 i;
	Int4 prev, curr;
	Int4 a_prev, a_curr;
	FloatHi val;

	/*assumes that there are 0-0 mapping at the start*/
	prev = 0;
	a_prev = 0;
	for(i = 0; i<num; ++i)
	{
		curr = pos[i];
		a_curr = anchor_pos[i];
		if(this_pos == prev)
			return a_prev;
		if(this_pos == curr)
			return a_curr;
		if(this_pos >prev && this_pos <curr)
		{
			if(a_curr == a_prev)
				return a_curr;
			val = (FloatHi)(a_curr - a_prev) * (FloatHi)(this_pos - prev) / (FloatHi)(curr - prev);
			return (a_prev + (Int4)val);
		}
		prev = curr;
		a_prev = a_curr;
	}

	curr = length-1;
	a_curr = length-1;
	val = (FloatHi)(a_curr - a_prev) * (FloatHi)(this_pos - prev) / (FloatHi)(curr - prev);
	return (a_prev + (Int4)val);
}


Boolean find_flanking_anchor_pos(Int4Ptr anchor_pos, Uint2 num, Int4 c_pos, Int4 length, Int4Ptr f_left, Int4Ptr f_right)
{
	Uint2 i;
	Int4 prev;

	prev = 0;
	for(i = 0; i<num; ++i)
	{
		if(anchor_pos[i] == c_pos)
		{
			*f_left = anchor_pos[i];
			*f_right = anchor_pos[i];
			return TRUE;
		}
		if(anchor_pos[i] > c_pos)
		{
			*f_left = prev;
			*f_right = anchor_pos[i];
			return TRUE;
		}

		prev = anchor_pos[i];
	}

	*f_left = prev;
	*f_right = length-1;
	return TRUE;
}



/**********************************************************************
*
*	MapLocToAnchor(annot, slp, anchor)
*	map the current slp to a position on the anchor Bioseq
*	annot: Seq-annot that may contain the alignment of the consistent markers
*	slp: the current Bioseq
*	anchor_id: the Seq-id for the anchor Bioseq, that is the sequece map
*
************************************************************************/ 
SeqLocPtr MapLocToAnchor(SeqAnnotPtr annot, SeqLocPtr slp, BioseqPtr anchor_bsp)
{
	Int2 type;
	Int4Ptr x_a, x;
	Uint2 num;
	SeqIdPtr anchor_id;
	SeqLocPtr t_slp;
	Int4 start, stop;
	SeqAlignPtr align;


	if(annot == NULL || slp == NULL || anchor_bsp == NULL)
		return NULL;
	if(slp->choice != SEQLOC_PNT && slp->choice != SEQLOC_INT)
		return NULL;
	anchor_id = SeqIdFindBest(anchor_bsp->id, SEQID_GI);
	if(anchor_id == NULL)
		anchor_id = anchor_bsp->id;

	while(annot)
	{
		if(annot->type == 2)
		{
			type = GetEquivAlignType(annot);
			if(type == 1)	/*this is consistent*/
			{
				align = annot->data;
				if(!get_anchor_coordinates(align, anchor_id, SeqLocId(slp), &x_a, &x, &num))
					return NULL;
				if(slp->choice == SEQLOC_INT)
				{
					start = find_this_position_by_anchor (x_a, x, num, SeqLocStart(slp), anchor_bsp->length);
					if(SeqLocStart(slp) != SeqLocStop(slp))
						stop = find_this_position_by_anchor (x_a, x, num, SeqLocStop(slp), anchor_bsp->length);
					else
						stop = start;
					t_slp = SeqLocIntNew(start, stop, Seq_strand_plus, anchor_id);
				}
				else
				{
					start = SeqLocStart(slp);
					start = find_this_position_by_anchor (x_a, x, num, start, anchor_bsp->length);
					t_slp = SeqLocPntNew(start, Seq_strand_plus, anchor_id, FALSE);
				}
				MemFree(x_a);
				MemFree(x);
				return t_slp;
			}
		}

		annot = annot->next;
	}

	return NULL;
}


/*****************************************************************
*
*	map a position on the anchor_bsp (anchor_pos) to a 
*	position on the other_bsp. It is the reverse operation of 
*	MapLocToAnchor
*	return -1 for failure
*
******************************************************************/
Int4 MapAnchorToLoc(SeqAnnotPtr annot, Int4 anchor_pos, BioseqPtr anchor_bsp, BioseqPtr other_bsp)
{
	Int2 type;
	Int4Ptr x_a, x;
	Uint2 num;
	SeqIdPtr anchor_id, other_id;
	SeqAlignPtr align;
	Int4 other_pos;


	if(annot == NULL || anchor_bsp == NULL || other_bsp == NULL)
		return -1;
	if(anchor_pos < 0 || anchor_pos > anchor_bsp->length-1)
		return -1;

	if(anchor_bsp == other_bsp)
		return anchor_pos;

	anchor_id = SeqIdFindBest(anchor_bsp->id, SEQID_GI);
	if(anchor_id == NULL)
		anchor_id = anchor_bsp->id;

	other_id = SeqIdFindBest(other_bsp->id, SEQID_GI);
	if(other_id == NULL)
		other_id = other_bsp->id;

	other_pos = -1;
	while(annot)
	{
		if(annot->type == 2)
		{
			type = GetEquivAlignType(annot);
			if(type == 1)	/*this is consistent*/
			{
				align = annot->data;
				if(get_anchor_coordinates(align, anchor_id, other_id, &x_a, &x, &num))
				{
					if(num >= 10)
						other_pos = find_this_position_by_anchor (x, x_a, num, anchor_pos, other_bsp->length);
					MemFree(x_a);
					MemFree(x);
					if(other_pos != -1)
						return other_pos;
				}
			}
		}

		annot = annot->next;
	}

	return other_pos;
}



