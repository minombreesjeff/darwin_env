/*  satutil.c
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
* File Name:  satutil.c
*
* Author:  Jinghui Zhang
*
* Version Creation Date: 4/25/96
*
* $Revision: 6.2 $
*
* File Description:  Copy Seq-feat based on sequence alignment
*
* Modifications:
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* $Log: satutil.c,v $
* Revision 6.2  1999/02/22 23:07:05  chappey
* fix in SeqAlignReplaceId
*
* Revision 6.1  1998/03/17 19:49:03  zjing
* adjust the protein product coordinates to the stop codon in CDS for feature propagation
*
* Revision 6.0  1997/08/25 18:07:01  madden
* Revision changed to 6.0
*
* Revision 5.11  1997/07/22 18:08:46  zjing
* changes CopyFeatureFromAlign
*
* Revision 5.10  1997/06/19 18:38:40  vakatov
* [WIN32,MSVC++]  Adopted for the "NCBIOBJ.LIB" DLL'ization
*
* Revision 5.9  1996/11/22 17:56:36  zjing
* correct an error in mapping the reading frame for coding region propagation
*
 * Revision 5.8  1996/10/22  21:57:14  zjing
 * modify map_one_loc to take care of the end gaps
 *
 * Revision 5.7  1996/10/09  17:00:32  chappey
 * add a new function SeqAlignReplaceId ()
 *
 * Revision 5.6  1996/08/29  21:35:38  zjing
 * make function map_on_location external
 *
 * Revision 5.1  1996/06/10  18:39:36  zjing
 * changes in map_one_location
 *
 * Revision 5.0  1996/05/28  13:23:23  ostell
 * Set to revision 5.0
 *
 * Revision 1.2  1996/04/26  15:52:06  kans
 * added public domain notice, added casts to eliminate PC warnings
 *
*
*
* ==========================================================================
*/

#include <satutil.h>
#include <gather.h>


/************************************************************************
*
*	make_fuzz_intloc(start, stop, strand, sip, is_from, is_to)
*	make Seqloc including the fuzziness information
*
*************************************************************************/
static void load_fuzz_info(SeqLocPtr loc, Boolean fuzz_from, Boolean fuzz_to)
{
	SeqLocPtr slp, last_slp;
   	SeqIntPtr sint;
        IntFuzzPtr ifp;
	Boolean first;
	Uint1 strand;

	if(!fuzz_from && !fuzz_to)
		return;
	slp = NULL;
	last_slp = NULL;
	first = TRUE;
	strand = SeqLocStrand(loc);
	while((slp = SeqLocFindNext(loc, slp)) != NULL)
	{
		if(slp->choice == SEQLOC_INT)
		{
			if(first == TRUE)
			{
				sint = slp->data.ptrvalue;
				if(strand == Seq_strand_minus && fuzz_to)
				{
                			sint->if_to = IntFuzzNew();
                			ifp = sint->if_to;
                			ifp->choice =4;
                			ifp->a =1;
				}
				if(strand != Seq_strand_minus && fuzz_from)
				{
                			sint->if_from = IntFuzzNew();
                			ifp = sint->if_from;
                			ifp->choice = 4;
                			ifp->a =2;
				}
			}
			first = FALSE;
			last_slp = slp;
		}
	}

	if(last_slp != NULL)
	{
		sint = last_slp->data.ptrvalue;
		if(strand == Seq_strand_minus && fuzz_from)
		{
                	sint->if_from = IntFuzzNew();
                	ifp = sint->if_from;
                	ifp->choice = 4;
                	ifp->a =2;
		}
		if(strand != Seq_strand_minus && fuzz_to)
		{
                	sint->if_to = IntFuzzNew();
                	ifp = sint->if_to;
                	ifp->choice =4;
                	ifp->a =1;
		}
	}
}


/***************************************************************************
*
*	map_one_location(): map source_loc to the target sequence defined
*	by target_sip. Map one Seq-loc
*	The new Seq-loc will be attached to the head_slp
*	return TRUE for success, FALSE for fail
*	source_slp: the source Seq-loc to be mapped
*
***************************************************************************/
static Boolean bk_map_one_location(SeqLocPtr source_loc, SeqAlignPtr align, SeqIdPtr target_sip, Boolean check_gap, Int4Ptr r_start, Int4Ptr r_stop, SeqLocPtr PNTR head_slp)
{
	BioseqPtr t_bsp;
	SeqLocPtr t_loc;
	AlignDataPtr adp, h_adp;
	SeqIdPtr s_sip;
	Int4 s_start, s_stop;
	Uint1 s_strand, strand;
	Int4 start, stop, temp;
	Int4 off_start, off_stop;
	AlignRangePtr arp;
	SeqLocPtr slp;
	Boolean retval;
	
	t_bsp = BioseqFindCore(target_sip);
	if(t_bsp == NULL)
		return FALSE;

	s_sip = SeqLocId(source_loc);
	s_start = SeqLocStart(source_loc);
	s_stop = SeqLocStop(source_loc);
	s_strand = SeqLocStrand(source_loc);

	t_loc = SeqLocIntNew(0, t_bsp->length-1, s_strand, target_sip);
	h_adp = gather_align_data(t_loc, align, 0, check_gap, TRUE);
	SeqLocFree(t_loc);
	if(h_adp == NULL)
	{
		return FALSE;
	}

	
	retval = FALSE;
	adp = h_adp;
	while(adp && !retval)
	{
		if(SeqIdForSameBioseq(adp->sip, s_sip))
		{
			if(!(adp->seqends.start > s_stop || adp->seqends.stop < s_start))
			{	/*there is overlap*/
				*r_start = MAX(adp->seqends.start, s_start);
				*r_stop = MIN(adp->seqends.stop, s_stop);
				strand = adp->extremes.strand;

				if(check_gap)
				{
					arp = adp->arp;
					while(arp)
					{
						if(arp->segtype == REG_SEG || arp->segtype == DIAG_SEG)
						{
							if(!(arp->sr.stop < s_start || arp->sr.start > s_stop))
							{

							off_start = MAX(0, (s_start - arp->sr.start));
							off_stop = MAX(0, (arp->sr.stop - s_stop));
							if(strand == Seq_strand_minus)
							{
								temp = off_stop;
								off_stop = off_start;
								off_start = temp;
							}

							start  = arp->gr.left + off_start;
							stop = arp->gr.right - off_stop;
							slp = SeqLocIntNew(start, stop, strand, target_sip);
							SeqLocAdd(head_slp, slp, TRUE, FALSE);
							retval = TRUE;
							}
							if(arp->sr.stop > s_stop)
								break;
						}
						arp = arp->next;
					}
				}
				else
				{
					off_start = MAX(0, (*r_start - adp->seqends.start));
					off_stop = MAX(0, (adp->seqends.stop - *r_stop ));
					if(strand == Seq_strand_minus)
					{
						temp = off_stop;
						off_stop = off_start;
						off_start = temp;
					}
					start = adp->extremes.left + off_start;
					stop = adp->extremes.right - off_stop;

					slp = SeqLocIntNew(start, stop, strand, target_sip);
					SeqLocAdd(head_slp, slp, TRUE, FALSE);

					retval = TRUE;
				}
			}
		}
		adp = adp->next;
	}

	FreeAlignData(h_adp);
	return retval;
}
				
static SeqLocPtr pack_loc_by_choice PROTO((SeqLocPtr slp, Uint1 choice));

static Boolean get_real_extremes (AlignDataPtr adp, Int4Ptr e_left, Int4Ptr e_right)
{
	Boolean first;
	AlignRangePtr arp, last_gap;

	*e_left = -1;
	*e_right = -1;
	first = TRUE;

	last_gap = NULL;
	for(arp = adp->arp; arp != NULL; arp = arp->next)
	{
		if(arp->segtype != GAP_SEG)
			last_gap = NULL;
		else if(last_gap == NULL)
			last_gap = arp;
	}
	arp = adp->arp;
	while(arp && arp != last_gap)
	{
		if(arp->segtype == GAP_SEG && first)
			arp = arp->next;
		else
		{
			first = FALSE;
			if(*e_left == -1)
				*e_left = arp->gr.left;
			else
				*e_left = MIN(*e_left, arp->gr.left);
			if(*e_right == -1)
				*e_right= arp->gr.right;
			else
				*e_right= MAX(*e_right, arp->gr.right);
			arp = arp->next;
		}
	}

	if(*e_left == -1 || *e_right == -1)
	{
		*e_left = adp->extremes.left;
		*e_right = adp->extremes.right;
		return FALSE;
	}

	return TRUE;
}
			

NLM_EXTERN SeqLocPtr map_one_location(SeqLocPtr source_loc, SeqAlignPtr align, SeqIdPtr target_sip, Uint1 gap_choice, Int4Ptr r_start, Int4Ptr r_stop)
{
	AlignDataPtr adp, h_adp;
	Int4 s_start, s_stop;
	Uint1 s_strand, strand;
	AlignRangePtr arp;
	SeqLocPtr slp;
	SeqLocPtr head_slp = NULL;
	Boolean check_gap;
	Int4 e_left, e_right;
	

	/* check_gap = (gap_choice != IGNORE_GAP_CHOICE); */
	check_gap = TRUE;
	h_adp = gather_align_data(source_loc, align, 0, check_gap, TRUE);
	if(h_adp == NULL)
		return NULL;
	
	adp = h_adp;
	while(adp)
	{
		if(SeqIdForSameBioseq(adp->sip, target_sip))
			break;
		else
			adp = adp->next;
	}
	if(adp != NULL)
	{
		s_start = SeqLocStart(source_loc);
		s_stop = SeqLocStop(source_loc);
		s_strand = SeqLocStrand(source_loc);
		get_real_extremes (adp, &e_left, &e_right);

		if(s_strand  == Seq_strand_minus)
		{
			*r_start = s_start + (SeqLocLen(source_loc) -1 - e_right);
			*r_stop = s_stop - e_left;
			/* *r_start = s_start + (SeqLocLen(source_loc) -1 - adp->extremes.right);
			*r_stop = s_stop - adp->extremes.left; */
		}
		else
		{
			*r_start = s_start + e_left;
			*r_stop = s_stop - (SeqLocLen(source_loc) -1 - e_right);
			/* *r_start = s_start + adp->extremes.left;
			*r_stop = s_stop - (SeqLocLen(source_loc) -1 - adp->extremes.right); */
		}
		strand = adp->extremes.strand;
		if(check_gap)
		{
			arp = adp->arp;
			while(arp)
			{
				if(arp->segtype == REG_SEG || arp->segtype == DIAG_SEG)
				{
					slp = SeqLocIntNew(arp->sr.start, arp->sr.stop, strand, target_sip);
					SeqLocAdd(&head_slp, slp, TRUE, FALSE);
				}
				arp = arp->next;
			}
		}
		else
		{
			slp = SeqLocIntNew(adp->seqends.start, adp->seqends.stop, strand, target_sip);
			SeqLocAdd(&head_slp, slp, TRUE, FALSE);
		}
	}

	FreeAlignData(h_adp);
	if(head_slp != NULL)
		return pack_loc_by_choice(head_slp, gap_choice);
	return NULL;
}
	
static void FindCDSNum(SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent)
{
	ValNodePtr vnp;
	BioseqPtr bsp;
	BioseqSetPtr bssp;
	SeqAnnotPtr annot;
	SeqFeatPtr sfp;

	if(sep->choice == 1)
	{
		bsp = sep->data.ptrvalue;
		annot = bsp->annot;
	}
	else
	{
		bssp = sep->data.ptrvalue;
		annot = bssp->annot;
	}

	vnp = (ValNodePtr)data;
	while(annot)
	{
		if(annot->type == 1)
		{
			sfp = annot->data;
			while(sfp)
			{
				if(sfp->data.choice == 3)
				{
					++vnp->data.intvalue;
					vnp->choice = 1;
				}
				sfp = sfp->next;
			}
		}
		annot = annot->next;
	}
}

static SeqIdPtr MakeNewProteinId(SeqLocPtr location, SeqIdPtr dna_seq)
{
	SeqIdPtr l_sip;
	ValNode vn;
	Int2 order;
	SeqIdPtr sip;
	ObjectIdPtr oip;
	Char p_label[100], name[21];
	BioseqPtr bsp;
	SeqEntryPtr sep;


	
	if(location != NULL)
		l_sip = SeqLocId(location);
	else
		l_sip = NULL;

	if(l_sip == NULL && dna_seq == NULL)
	{
		Message(MSG_ERROR, "No input id available");
		return NULL;
	}

	sep = NULL;
	if(dna_seq != NULL)
		sep = SeqEntryFind(dna_seq);
	else if(l_sip != NULL)
		sep = SeqEntryFind(l_sip);

	order = 0;
	if(sep != NULL)
	{
		vn.choice = 0;
		vn.data.intvalue = 0;
		SeqEntryExplore(sep, &vn, FindCDSNum);
		if(vn.choice == 1)
			order = (Int2)(vn.data.intvalue+1);
	}

	if(order == 0)
		order = 1;
	name[0] = '\0';
	if(dna_seq != NULL)
		SeqIdWrite(dna_seq, name, PRINTID_TEXTID_ACCESSION, 20);
	if(name[0] == '\0' && l_sip != NULL)
		SeqIdWrite(dna_seq, name, PRINTID_TEXTID_ACCESSION, 20);


	if(name[0] == '\0')
		sprintf(p_label, "temp_%d", order);
	else
		sprintf(p_label, "%s_%d", name, order);
	oip = ObjectIdNew();
	oip->str = StringSave(p_label);
	sip = ValNodeNew(NULL);
	sip->choice = SEQID_LOCAL;
	sip->data.ptrvalue = oip;

	bsp = BioseqFindCore(sip);
	while(bsp != NULL)
	{
		SeqIdFree(sip);
		++order;
		sprintf(p_label, "temp_%d", order);
		MemFree(oip->str);
		oip->str = StringSave(p_label);
		bsp = BioseqFindCore(sip);
	}
	return sip;

}

			


/*
static void copy_Xref(SeqFeatPtr new_sfp, BioseqPtr old_prot)
{
	SeqAnnotPtr annot;
	SeqFeatPtr sfp;
        SeqFeatXrefPtr xref, curr;
		
	if(new_sfp->xref != NULL)
		return;
	annot = old_prot->annot;
	while(annot)
	{
		if(annot->type == 1)
		{
			sfp = annot->data;
			while(sfp)
			{
				if(sfp->data.choice == 4)
				{
					xref = SeqFeatToXref(sfp);
					if(new_sfp->xref == NULL)
						new_sfp->xref = xref;
					else
					{
						curr = new_sfp->xref;
						while(curr->next != NULL)
							curr = curr->next;
						curr->next = xref;
					}
				}
				sfp = sfp->next;
			}
		}
		annot = annot->next;
	}
}
*/


static void AddSeqRef(BioseqPtr new_prot, SeqLocPtr old_product)
{
	SeqFeatPtr sfp, curr;
	SeqAnnotPtr annot, prev;
	SeqLocPtr slp;
	

	sfp = SeqFeatNew();
	sfp->data.choice = 7;		/*Seq-ref*/
	slp = ValNodeNew(NULL);
	slp->choice = SEQLOC_WHOLE;
	slp->data.ptrvalue = SeqIdDup(new_prot->id);
	sfp->location = slp;
	sfp->data.value.ptrvalue = old_product;

	annot = new_prot->annot;
	if(annot == NULL)
	{
		annot = SeqAnnotNew();
		annot->type = 1;
		annot->data = sfp;
		new_prot->annot = annot;
		return;
	}
	else
	{
		prev = NULL;
		while(annot)
		{
			if(annot->type ==1)
			{
				curr = annot->data;
				if(curr != NULL)
				{
					while(curr->next != NULL)
						curr = curr->next;
					curr->next =sfp;
					return;
				}
			}
			prev = annot;
			annot = annot->next;
		}
		annot = SeqAnnotNew();
		annot->type = 1;
		annot->data = sfp;
		prev->next = annot;
		return;
	}
}
		
	
static Boolean link_new_bsp(SeqEntryPtr sep, BioseqPtr new_bsp)
{
	BioseqSetPtr bssp, parent = NULL;
	SeqEntryPtr new_sep, curr;
	BioseqPtr bsp;

	if(sep == NULL || new_bsp == NULL)
		return FALSE;

	new_sep = SeqEntryNew();
	new_sep->choice = 1;
	new_sep->data.ptrvalue = new_bsp;
	SeqMgrSeqEntry(SM_BIOSEQ, (Pointer)new_bsp, new_sep);

	
	curr = sep;
	
	while(curr->next != NULL || curr->choice != 1)
	{
		if(curr->choice == 1)
			curr = curr->next;
		else
		{
			bssp = (BioseqSetPtr)curr->data.ptrvalue;
			curr = bssp->seq_set;
			if(curr == NULL)
			{
				bssp->seq_set = new_sep;
				SeqMgrConnect(SM_BIOSEQ, (Pointer)new_bsp, SM_BIOSEQSET, (Pointer)bssp);
				return TRUE;
			}
			else
				parent = bssp;
		}
	}

	curr->next = new_sep;
	if(parent != NULL)
		SeqMgrConnect(SM_BIOSEQ, (Pointer)new_bsp, SM_BIOSEQSET, (Pointer)parent);
	else
	{
		bsp = curr->data.ptrvalue;
		SeqMgrConnect(SM_BIOSEQ, (Pointer)new_bsp, SM_BIOSEQ, (Pointer)bsp);
	}
	return TRUE;
}


/***************************************************************
*
*	Load a protein Bioseq to replace the Bogo product
*	sfp: the coding region feature
*	new_prot_id: if NULL a new default will be generated
*
***************************************************************/
NLM_EXTERN Boolean LoadProteinForCdRegion(SeqFeatPtr sfp, SeqIdPtr new_prot_id)
{
	SeqIdPtr sip;
	SeqEntryPtr sep;
	BioseqPtr new_prot, old_prot;
	BioseqPtr dna_bsp;
	SeqIdPtr prot_id;
	SeqLocPtr new_product;
	ByteStorePtr bsp;
	SeqDescrPtr desc, new_desc;
	Int4 aa_start, aa_stop;
	

	if(sfp == NULL || sfp->data.choice != 3 || sfp->location == NULL)
		return FALSE;
	if(sfp->product != NULL && !IS_BOGO_Product(sfp->ext))
		return FALSE;
	sip = SeqLocId(sfp->location);
	if(sip == NULL)
		return FALSE;
	dna_bsp = BioseqLockById(sip);
	if(dna_bsp == NULL)
		return FALSE;
	sep = SeqEntryFind(sip);
	if(sep == NULL)
	{
		BioseqUnlock(dna_bsp);
		return FALSE;
	}
	
	bsp = ProteinFromCdRegion(sfp, FALSE);
	if(new_prot_id == NULL)
		prot_id = MakeNewProteinId(sfp->location, dna_bsp->id);
	else
		prot_id = SeqIdDup(new_prot_id);
	new_prot = BioseqNew();
	new_prot->repr = Seq_repr_raw;
        new_prot->mol = Seq_mol_aa;
        new_prot->seq_data_type = Seq_code_ncbieaa;
        new_prot->seq_data = bsp;
        BSSeek(bsp, 0, SEEK_SET);
        new_prot->length = BSLen(bsp);
	new_prot->id = prot_id;

	if(sfp->product != NULL)
	{
		sip = SeqLocId(sfp->product);
		if(sip != NULL)
		{
			old_prot = BioseqLockById(sip);
			if(old_prot != NULL)
			{
				aa_start = SeqLocStart(sfp->product);
				aa_stop = MIN(SeqLocStop(sfp->product), old_prot->length-1);
				SeqFeatsCopy(new_prot, old_prot, aa_start, aa_stop, 0);
				AddSeqRef(new_prot, sfp->product);
				sfp->product = NULL;
				if(new_prot->descr != NULL)
					new_prot->descr = SeqDescrFree(new_prot->descr);
				desc = old_prot->descr;
				while(desc)
				{
					new_desc = AsnIoMemCopy((Pointer)desc, (AsnReadFunc)SeqDescrAsnRead, (AsnWriteFunc)SeqDescrAsnWrite);
					ValNodeLink(&(new_prot->descr), (ValNodePtr)new_desc);
					desc = desc->next;
				} 
				BioseqUnlock(old_prot);
			}
		}
	}

	link_new_bsp(sep, new_prot);
	if(sfp->product != NULL)
		SeqLocFree(sfp->product);
				
	new_product = ValNodeNew(NULL);
	new_product->choice = SEQLOC_WHOLE;
	new_product->data.ptrvalue = SeqIdDup(prot_id);
	sfp->product = new_product;

	DeleteBogoProduct(&(sfp->ext));

	BioseqUnlock(dna_bsp);
	return TRUE;
}


static SeqLocPtr make_seq_loc_by_ends(SeqLocPtr slp)
{
	Int4 m_start, m_stop;
	Uint1 strand;
	SeqIdPtr sip;

	if(slp == NULL)
		return NULL;

	m_start = -1;
	m_stop = -1;
	while(slp)
	{
		if(m_start == -1)
			m_start = SeqLocStart(slp);
		else
			m_start = MIN(m_start, SeqLocStart(slp));
		if(m_stop == -1)
			m_stop = SeqLocStop(slp);
		else
			m_stop = MAX(m_stop, SeqLocStop(slp));
		strand = SeqLocStrand(slp);
		sip = SeqLocId(slp);
		slp = slp->next;
	}

	return SeqLocIntNew(m_start, m_stop, strand, sip);
}


static SeqLocPtr pack_loc_by_choice(SeqLocPtr slp, Uint1 choice)
{
	SeqLocPtr loc, prev, new_slp, next;
	Int4 start, stop, p_start = -1, p_stop =-1;
	Int4 gap_len = 0;
	SeqIntPtr sint;
	Boolean del_loc;


	if(choice == DEFAULT_GAP_CHOICE)
	{
		loc = slp;
		prev = NULL;
		while(loc)
		{
			next = loc->next;
			del_loc = FALSE;
			start = SeqLocStart(loc);
			stop = SeqLocStop(loc);
			if(p_start == -1 || p_stop == -1)
			{
				start = p_start;
				stop = p_stop;
			}
			else
			{
				if(start > p_stop)
				{
					gap_len = start - (p_stop +1);
					if(gap_len <= MAX_GAP_LEN_BY_DEFAULT)
					{
						sint = prev->data.ptrvalue;
						sint->to = stop;
						del_loc = TRUE;
					}
				}
				else if(stop < p_start)
				{
					gap_len = p_start - (stop +1);
					if(gap_len <= MAX_GAP_LEN_BY_DEFAULT)
					{
						sint = prev->data.ptrvalue;
						sint->from = start;
						del_loc = TRUE;
					}
				}
			}

			if(del_loc)
			{
				if(prev == NULL)
					slp = loc->next;
				else
					prev->next = loc->next;
				loc->next = NULL;
				SeqLocFree(loc);
			}
			else
				prev = loc;
			p_start = start;
			p_stop = stop;
			loc = next;

		}
		
	}
	else if(choice == IGNORE_GAP_CHOICE)
	{
		new_slp = make_seq_loc_by_ends(slp);
		SeqLocSetFree(slp);
		slp = new_slp;
	}
	return slp;
}

static Boolean merge_to_prev_loc(SeqLocPtr prev_slp, SeqLocPtr slp)
{
	SeqIntPtr prev_sint, sint;

	if(SeqLocStrand(prev_slp) != SeqLocStrand(slp))
		return FALSE;
	prev_sint = prev_slp->data.ptrvalue;
	sint = slp->data.ptrvalue;
	if(SeqLocStrand(prev_slp) == Seq_strand_minus)
	{
		if(prev_sint->from == sint->to + 1)
		{
			prev_sint->from = sint->from;
			return TRUE;
		}
		else
			return FALSE;
	}
	else
	{
		if(prev_sint->to +1 == sint->from )
		{
			prev_sint->to = sint->to;
			return TRUE;
		}
		else
			return FALSE;
	}
}

static Boolean CDS_HAS_STOP_CODON(SeqFeatPtr sfp, Int4Ptr paa_len)
{
	CdRegionPtr crp;
	SeqLocPtr slp, prev;
	SeqIntPtr sint;
	Int4 start, stop;
	Int4 cds_len, aa_len;
	SeqPortPtr spp;
	BioseqPtr bsp;
	Int4 bsp_len;
	Char last_res[4];
	SeqIdPtr sip;
	Boolean retval = FALSE;

	*paa_len = 0;
	if(sfp == NULL || sfp->data.choice != 3 || sfp->product == NULL)
		return FALSE;
	crp = sfp->data.value.ptrvalue;
	if(crp == NULL)
		return FALSE;
	cds_len = SeqLocLen(sfp->location);
	if(cds_len < 1)
		return FALSE;
	if(crp->frame > 1)
	{
		cds_len -= (crp->frame -1);
	}
	aa_len = cds_len/3;
	*paa_len = aa_len;

	sip = SeqLocId(sfp->product);
	if(sip != NULL)
	{
		bsp = BioseqLockById(SeqLocId(sfp->product));
		if(bsp != NULL)
		{
			bsp_len = bsp->length;
			BioseqUnlock(bsp);
			*paa_len = bsp_len;
			return (aa_len == bsp_len + 1);
		}
	}

	slp = NULL;
	prev = NULL;
	while((slp = SeqLocFindNext(sfp->location, slp)) != NULL)
	{
		prev = slp;
	}

	if(prev == NULL || prev->choice != SEQLOC_INT)
		return FALSE;

	sint = prev->data.ptrvalue;
	if(sint != NULL)
	{
	if(sint->strand == Seq_strand_minus)
	{
		if(sint->if_from)
			return FALSE;
		start = sint->from;
		stop = sint->to - 2;
	}
	else
	{
		if(sint->if_to)
			return FALSE;
		stop = sint->to;
		start = sint->from;
	}

	bsp = BioseqLockById(sint->id);
	last_res[0] = '\0';
	if(bsp != NULL)
	{
		spp = SeqPortNew(bsp, start, stop, sint->strand, Seq_code_iupacna);
		if(spp != NULL)
		{
			last_res[0] = SeqPortGetResidue(spp);
			last_res[1] = SeqPortGetResidue(spp);
			last_res[2] = SeqPortGetResidue(spp);
			last_res[0] = '\0';
			SeqPortFree(spp);
			BioseqUnlock(bsp);
		}
	}

	if(last_res[0] != '\0')
	{
		if(StringICmp(last_res, "TAA") == 0)
			retval = TRUE;
		else if(StringICmp(last_res, "TAG") == 0)
			retval = TRUE;
		else if(StringICmp(last_res, "TGA") == 0)
			retval = TRUE;
		if(retval)
			*paa_len -=1 ;
	
	}
	}

	return retval;
}

	

	


/******************************************************************
*
*	CopyFeatureFromAlign(source_sfp, target_id, source_id, align, 
*	gap_choice, check_trunc
*
*	Copy the source_sfp to the target sequence based on their 
*	alignment
*	source_sfp: Seq-feat of the source
*	target_id:  Seq-id of the target
*	source_id:  Seq-id of the source in the alignment. This is 
*	neccessary when the source_sfp may composed from different 
*	segment of a Bioseq-set
*	align:      The Seq-align that contains alignment of the source 
*	and the target
*	gap_choice: choice to select to map the gap to the new feature 
*	can have default, ignore or take
*	check_trunc: load the fuzziness of the new Seq-feat
*
******************************************************************/
NLM_EXTERN SeqFeatPtr CopyFeatureFromAlign (SeqFeatPtr source_sfp, SeqIdPtr target_id, SeqIdPtr s_id, SeqAlignPtr align, Uint1 gap_choice, Boolean check_trunc)
{
	GatherRange gr;
	SeqLocPtr slp, source_slp, c_slp;
	SeqLocPtr process_slp, new_slp;
	SeqLocPtr prev_slp;
	Boolean check_gap;
	
	SeqIdPtr sip, source_id;
	Boolean map_to_source;
	BioseqPtr source_bsp;

	SeqFeatPtr new_sfp;
	CodeBreakPtr cbp, p_cbp, n_cbp;
	CdRegionPtr crp;
	RnaRefPtr rrp;
	tRNAPtr trp;
	Int4 cds_len;
	Int4 aa_start, aa_stop;
	Int4 a_start, a_stop;
	Int4 r_start, r_stop, e_start, e_stop;
	Int4 frame_offset, c_frame_offset;
	Int4 offset;
	Uint1 new_frame;
	Boolean stop_here;
	Boolean had_first_seg;
	Boolean fuzz_from, fuzz_to;
	Int4 max_aa_len = 0;

	if(source_sfp == NULL || target_id == NULL || align == NULL)
		return NULL;

	if(source_sfp->location == NULL)
		return NULL;

	sip = SeqLocId(source_sfp->location);
	if(sip == NULL)
		return NULL;

	if(s_id == NULL)
		source_id = sip;
	else
		source_id = s_id;

	source_bsp = BioseqLockById(source_id);

	if(source_bsp == NULL)
	{
		Message(MSG_ERROR, "Fail to get the source Bioseq");
		return NULL;
	}

	map_to_source = FALSE;	/*segmented sequence may have locations on different segment sequences*/

	if(!BioseqMatch(source_bsp, sip))
	{
		Message(MSG_ERROR, "Source Bioseq does not match the Seq-id of the Source Seq-feat");
		BioseqUnlock(source_bsp);
		return NULL;
	}
	else
		map_to_source = TRUE;

	source_slp = NULL;
	if(!map_to_source)
		source_slp = SeqLocIntNew(0, source_bsp->length-1, Seq_strand_plus, sip);
	if(source_sfp->data.choice == 3)
	{
		crp = source_sfp->data.value.ptrvalue;
		CDS_HAS_STOP_CODON(source_sfp, &max_aa_len);
	}
	else
		crp = NULL;

	if(gap_choice == DEFAULT_GAP_CHOICE)
	{
		if(source_sfp->data.choice == SEQFEAT_CDREGION || source_sfp->data.choice == SEQFEAT_RNA)
			check_gap = TRUE;
		else
			check_gap = FALSE;
	}
	else
		check_gap = (Boolean) (gap_choice == TAKE_GAP_CHOICE);

	slp = NULL;
	process_slp = NULL;
	prev_slp = NULL;
	cds_len = 0;
	new_frame = 0;
	a_start = -1;
	a_stop = -1;
	e_start = -1;
	e_stop = -1;
	stop_here = FALSE;
	had_first_seg = FALSE;
	while((slp = SeqLocFindNext(source_sfp->location, slp)) != NULL && !stop_here)
	{
		c_slp = NULL;
		new_slp = NULL;

		if(map_to_source == FALSE)
		{	/* map the location to the coordinates on the source */
			if(SeqLocOffset(source_slp, slp, &gr, 0))
			{
				if(gr.l_trunc == FALSE && gr.r_trunc == FALSE)
					c_slp = SeqLocIntNew(gr.left, gr.right, gr.strand, source_id);
			}
		}
		else
			c_slp = slp;

		if(c_slp != NULL)
		{
			new_slp = map_one_location(c_slp, align, target_id, gap_choice, &r_start, &r_stop);
			if(new_slp != NULL)
			{
				if(prev_slp == NULL)
				{
					process_slp = new_slp;
					prev_slp = new_slp;
				}
				else
				{
					if(merge_to_prev_loc(prev_slp, new_slp))
						SeqLocFree(new_slp);
					else
					{
						prev_slp->next = new_slp;
						prev_slp = new_slp;
					}
				}
				/* ValNodeLink(&process_slp, new_slp); */
				if(e_start == -1)
					e_start = r_start;
				else
					e_start = MIN(e_start, r_start);
				if(e_stop == -1)
					e_stop = r_stop;
				else
					e_stop = MAX(e_stop, r_stop);

		   		if(crp != NULL)	/*for coding region features*/
		   		{
					/*calculate the frame for the first exon*/
					if(!had_first_seg)
					{ 
						if(crp->frame > 1)
							frame_offset = (Int4)crp->frame -1L;
						else
							frame_offset = 0L;

						c_frame_offset = frame_offset;
						if(cds_len > 0)
						{
							c_frame_offset = (cds_len - frame_offset)%3;
						}
						if(SeqLocStrand(c_slp) == Seq_strand_minus)
							frame_offset = (SeqLocStop(c_slp) - r_stop - c_frame_offset)%3;
						else
							frame_offset = (r_start - SeqLocStart(c_slp) - c_frame_offset)%3;
						if(frame_offset < 0)
							c_frame_offset = frame_offset + 3;
						else
							c_frame_offset = frame_offset;
						/* if(cds_len > 0)
						{
							c_frame_offset = (cds_len - frame_offset)%3;
							if(c_frame_offset > 0)
								c_frame_offset = 3 - c_frame_offset;
						}
						if(SeqLocStrand(c_slp) == Seq_strand_minus)
							frame_offset = (SeqLocStop(c_slp) - r_stop)%3;
						else
							frame_offset = (r_start - SeqLocStart(c_slp))%3;
						if(frame_offset > 0)
							c_frame_offset = (frame_offset + c_frame_offset)%3;
						if(c_frame_offset > 1) */
						if(c_frame_offset > 0)
							new_frame = 4 - (Uint1) (c_frame_offset);
						else
							new_frame = 1;
					}

					/*calculate the position in the amino acid*/
					if(SeqLocStrand(c_slp) == Seq_strand_minus)
						offset = SeqLocStop(c_slp) - r_stop;
					else
						offset = r_start - SeqLocStart(c_slp);
						
					if(crp->frame > 1)
						frame_offset = (Int4)crp->frame -1L;
					else
						frame_offset = 0L;
					aa_start = (cds_len + offset - frame_offset)/3;
					if(aa_start < 0)
						aa_start = 0 ;

					if(SeqLocStrand(c_slp) == Seq_strand_minus)
						offset = SeqLocStop(c_slp) - r_start;
					else
						offset = r_stop- SeqLocStart(c_slp);
					aa_stop= (cds_len + offset - frame_offset)/3;
					if(aa_stop < 0 )
						aa_stop = 0;
					if(a_start == -1)
						a_start = aa_start;
					else
						a_start = MIN(a_start, aa_start);

					if(a_stop == -1)
						a_stop = aa_stop;
					else
						a_stop = MAX(a_stop, aa_stop);

		  		}/*finishing processing the CDS region*/
				had_first_seg = TRUE;

			}
			else
				stop_here = TRUE;
		}
		else
			stop_here = TRUE;
		cds_len += SeqLocLen(c_slp);
		if(c_slp != NULL && !map_to_source)
			SeqLocFree(c_slp);
	}
						

	if(process_slp == NULL)
	{
		if(source_slp != NULL)
			SeqLocFree(source_slp);
		BioseqUnlock(source_bsp);
		return NULL;
	}

	new_sfp = AsnIoMemCopy((Pointer)source_sfp, (AsnReadFunc)SeqFeatAsnRead, (AsnWriteFunc)SeqFeatAsnWrite);
	SeqLocFree(new_sfp->location);
	new_sfp->location = SeqLocPackage(process_slp);
	if(check_trunc)
	{
		if(e_start != -1 && e_stop != -1)
		{
			fuzz_from = (Boolean) (SeqLocStart(source_sfp->location) < e_start);
			fuzz_to = (Boolean) (SeqLocStop(source_sfp->location) > e_stop);
			load_fuzz_info(new_sfp->location, fuzz_from, fuzz_to);
		}
	}


	if(new_sfp->data.choice == 3)	/*it is cdregion*/
	{
		if(new_sfp->product != NULL)
		{
			if(a_start != -1 && a_stop != -1)
			{
				a_start += SeqLocStart(source_sfp->product);
				a_stop += SeqLocStart(source_sfp->product);
				if(max_aa_len > 0)
					a_stop = MIN(a_stop, max_aa_len -1);
				new_slp = SeqLocIntNew(a_start, a_stop, 0, SeqLocId(new_sfp->product));
				SeqLocFree(new_sfp->product);
				new_sfp->product = new_slp;
			}

			make_bogo_product(&(new_sfp->ext));

		}

		crp = new_sfp->data.value.ptrvalue;
		crp->frame = new_frame;
		if(crp != NULL && crp->code_break != NULL)		/*correct the code-break*/
		{
			cbp = crp->code_break;
			p_cbp = NULL;
			while(cbp)
			{
				n_cbp = cbp->next;
				c_slp = NULL;
				new_slp = NULL;
				if(map_to_source == FALSE)
				{	/* map the location to the coordinates on the source */
					if(SeqLocOffset(source_slp, cbp->loc, &gr, 0))
					{
						if(gr.l_trunc == FALSE && gr.r_trunc == FALSE)
							c_slp = SeqLocIntNew(gr.left, gr.right, gr.strand, source_id);
					}
				}
				else
					c_slp = cbp->loc;
	
				new_slp = NULL;
				new_slp = map_one_location(c_slp, align, target_id, gap_choice, &r_start, &r_stop);
				if(new_slp != NULL)
				{
					SeqLocFree(cbp->loc);
					cbp->loc = SeqLocPackage(new_slp);
					p_cbp= cbp;
				}
				else
				{
					if(p_cbp == NULL)
						crp->code_break = cbp->next;
					else
						p_cbp->next = cbp->next;
					cbp->next = NULL;
					CodeBreakFree(cbp);
				}
				if(map_to_source)
					SeqLocFree(c_slp);
				cbp = n_cbp;
			}
		}
	}

	if(new_sfp->data.choice == SEQFEAT_RNA) /*fixing the antidoon in tRNA*/
	{
		rrp = (RnaRefPtr)(new_sfp->data.value.ptrvalue);
		if(rrp->ext.choice == 2)
		{
			trp = (tRNAPtr)(rrp->ext.value.ptrvalue);
			if(trp->anticodon != NULL)
			{
				new_slp = map_one_location(c_slp, align, target_id, gap_choice, &r_start, &r_stop);
				SeqLocFree(trp->anticodon);
				trp->anticodon = new_slp;
			}
		}
	}

	BioseqUnlock(source_bsp);
	return new_sfp;
}
			
		
/********************************************************
*
*	functions related to the Bogo product
*
*********************************************************/

/*make the bogo product UserObject and link it to the chain*/
NLM_EXTERN void make_bogo_product(UserObjectPtr PNTR head)
{
	UserObjectPtr uop, curr;
	UserFieldPtr ufp;
	ObjectIdPtr oip;

	uop = *head;
	if(uop != NULL)
	{
		while(uop)
		{
			oip = uop->type;
			if(oip->str != NULL)
			{
				if(StringCmp(oip->str, "Bogo Product") == 0)
					return;
			}
			uop = uop->next;
		}
	}

	ufp = UserFieldNew();
	oip = ObjectIdNew();
	oip->str = StringSave("Bogo Product");
	ufp->label = oip;
	ufp->choice = 4;
	ufp->data.boolvalue = TRUE;

	uop = UserObjectNew();
	uop->data = ufp;
	oip = ObjectIdNew();
	oip->str = StringSave("Bogo Product");
	uop->type = oip;

	if(*head == NULL)
		*head = uop;
	else
	{
		curr = *head;
		while(curr->next != NULL)
			curr = curr->next;
		curr->next = uop;
	}

}


/*************************************************************
*
*	check if the UserObject contains the identifyer for the 
*	BOGO product
*
**************************************************************/
NLM_EXTERN Boolean IS_BOGO_Product(UserObjectPtr uop)
{
	ObjectIdPtr oip;
	UserFieldPtr ufp;

	while(uop)
	{
		oip = uop->type;
		if(oip != NULL && oip->str != NULL)
		{
			if(StringCmp(oip->str, "Bogo Product") == 0)
			{
				ufp = uop->data;
				if(ufp->choice == 4)
					return (ufp->data.boolvalue);
			}
		}

		uop = uop->next;
	}
	return FALSE;
}


/************************************************************
*
*	DeleteBogoProduct(head)
*	Delete the UserObject of Bogo Product from the chain
*
**************************************************************/
NLM_EXTERN void DeleteBogoProduct (UserObjectPtr PNTR head)
{
	UserObjectPtr curr, prev, next;

	curr = *head;
	prev = NULL;

	while(curr)
	{
		next = curr->next;
		if(IS_BOGO_Product(curr))
		{
			if(prev == NULL)
				*head = next;
			else
				prev->next = next;
			curr->next = NULL;
			UserObjectFree(curr);
		}
		else
			prev = curr;

		curr = next;
	}

}

/******************************************************************** 
* 
*  SeqAlignReplaceId () 
*       replaces a Seq-Id (oldId) in a Seq-Id list in a Seq-Align (salp)
*       with a new Seq-Id (newId)
* 
*********************************************************************/
NLM_EXTERN SeqAlignPtr SeqAlignReplaceId (SeqIdPtr oldId, SeqIdPtr the_newId, SeqAlignPtr salp)
{
  SeqAlignPtr  salptmp =NULL;
  DenseSegPtr  dsp = NULL;
  DenseDiagPtr ddp = NULL;
  StdSegPtr    ssp = NULL;
  SeqIdPtr     sip, pre,
               newId;

  if (salp==NULL) return NULL;
  for (salptmp=salp; salptmp!=NULL; salptmp=salptmp->next)
  {
     if (salp->segtype == 1) {
        ddp = (DenseDiagPtr) salptmp->segs;
        pre = NULL;
        for (sip=ddp->id; sip!=NULL; sip=sip->next)
        {
           if (SeqIdComp(sip, oldId) == SIC_YES)
              break;
           pre = sip;
        }
        if (sip!=NULL){
           newId = SeqIdDup (the_newId);
           newId->next = NULL;
           if (pre==NULL)
                 ddp->id = newId;
           else  pre->next = newId;
           if (sip->next) {
              newId->next = sip->next;
              sip->next = NULL;
           }
           SeqIdFree (sip);
        }
     }   
     else if (salp->segtype == 2)
     {   
        dsp = (DenseSegPtr) salptmp->segs;
        pre = NULL;
        for (sip=dsp->ids; sip!=NULL; sip=sip->next)
        {
           if (SeqIdComp(sip, oldId) == SIC_YES)
              break;
           pre = sip;
        }
        if (sip!=NULL){
           newId = SeqIdDup (the_newId);
           newId->next = NULL;
           if (pre==NULL)
                 dsp->ids = newId;
           else  pre->next = newId;
           if (sip->next) {
              newId->next = sip->next;
              sip->next = NULL;
           }
           SeqIdFree (sip);
           sip = newId->next;
        }
     }
     else if (salp->segtype == 3)
     {
        ssp = (StdSegPtr) salptmp->segs;
        pre = NULL;
        for (sip=dsp->ids; sip!=NULL; sip=sip->next)
        {
           if (SeqIdComp(sip, oldId) == SIC_YES)
              break;
           pre = sip;
        }
        if (sip!=NULL){
           newId = SeqIdDup (the_newId);
           newId->next = NULL;
           if (pre==NULL)
                 ssp->ids = newId;
           else  pre->next = newId;
           if (sip->next) {
              newId->next = sip->next;
              sip->next = NULL;
           }
           SeqIdFree (sip);
        }
     }
  }
  return salp;
}  

