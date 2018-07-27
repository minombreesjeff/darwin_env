static char const rcsid[] = "$Id: sim2.c,v 6.1 2003/05/30 17:25:38 coulouri Exp $";

#include <stdio.h>
#include <ncbi.h>
#include <objseq.h>
#include <objloc.h>
#include <seqport.h>
#include <sequtil.h>
#include "simutil.h"



/* SIM2 - calling routine to falign() */
		
SeqAlignPtr SIM2(SeqLocPtr cslp1, SeqLocPtr cslp2, Int4 k, SimPamPtr a_spp, ValNodePtr PNTR new_list, Uint1 output_type, FilterProc filter)
{
	SeqAlignPtr sap = NULL, sapC=NULL, out_sap, out_sapC;
	Boolean both_strand, is_dna;
	CharPtr A, B, compB = NULL;
	Int4 w, r, o, e, m, g, h;
	FloatHi cut_off;
	Int4 Width = -1;
	SimPam sp, PNTR spp;
	Uint1 m_strand, s_strand;

 
	if(cslp1 == NULL || cslp2 == NULL)
	{
		Message(MSG_ERROR, "In sufficient input");
		return NULL;
	}

	if(cslp1->choice != SEQLOC_INT || cslp2->choice !=SEQLOC_INT)
	{
		Message(MSG_ERROR, "Incorrect type of Seq-loc. Only take Seq-int");
		return NULL;
	}
	if(output_type < OUTPUT_ALIGN || output_type > OUTPUT_BOTH)
	{
		Message(MSG_OK, "Illegal output type");
		output_type = OUTPUT_ALIGN;
	}

	if(new_list !=NULL)
		*new_list = NULL;
	else
	{
		if(output_type == OUTPUT_BOTH || output_type == OUTPUT_ENDS)
		{
			Message(MSG_ERROR, "No space to store the ends");
			return NULL;
		}
	}


	both_strand = check_strand_mol(cslp2, &is_dna);
	if(!is_dna)
	{
		Message(MSG_ERROR, "Sorry, Sim2 can only compare two DNA sequences");
		return NULL;
	}
	if(!both_strand)
		both_strand = check_strand_mol(cslp1, &is_dna);
	if(a_spp == NULL)
	{
		DefaultSimPam(&sp);
		spp = &sp;
	}
	else
		spp = a_spp;
	w = spp->word;
	r = spp->mismatch;
	o = spp->gap_open;
	e = spp->gap_ext;
	m = spp->mismatch_2;
	g = spp->gap_open_2;
	h = spp->gap_ext_2;
	cut_off = spp->cutoff;


	if(both_strand)
	{
		m_strand = SeqLocStrand(cslp1);
		Change_Loc_Strand(cslp1, Seq_strand_plus);
		s_strand = SeqLocStrand(cslp2);
		Change_Loc_Strand(cslp2, Seq_strand_plus);
	}

	A= make_sim_seq(cslp1, TRUE, NULL);
	if(A == NULL)
	{
		if(both_strand)
		{
			Change_Loc_Strand(cslp2, s_strand);
			Change_Loc_Strand(cslp1, m_strand);
		}
		return NULL;
	}
	B = make_sim_seq(cslp2, TRUE, NULL);
	if(B == NULL)
	{
		if(both_strand)
		{
			Change_Loc_Strand(cslp2, s_strand);
			Change_Loc_Strand(cslp1, m_strand);
		}

		if(A != NULL)
			MemFree(A);
		return NULL;
	}
	sap = NULL;
	sap = falign(cslp1, cslp2, A+1, B+1, w, k, r, o, e);
	out_sap = Region(sap, cslp1, cslp2, A+1, B+1, m, g, h, Width, new_list, output_type, filter);
	sap = free_align_list(sap);/* free sap */

	if(both_strand)
	{
		Change_Loc_Strand(cslp2, (Uint1)2);
		B = make_sim_seq(cslp2, TRUE, B);
		sap = falign(cslp1, cslp2, A+1, B+1, w, k, r, o, e);
		out_sapC = Region(sap, cslp1, cslp2, A+1, B+1, m, g, h, Width, new_list, output_type, filter);
		sap = free_align_list(sap);	/*free sap */

		/* concatenate out_sap and out_sapC */
		out_sap = link_align(out_sapC, out_sap);
	}
	
	MemFree(A);
	MemFree(B);
	if(output_type  != OUTPUT_ALIGN)
		*new_list = get_top_K(*new_list, k);
	if(both_strand)
	{
		Change_Loc_Strand(cslp2, s_strand);
		Change_Loc_Strand(cslp1, m_strand);
	}
	return get_top_K_alignment(out_sap, k, cut_off);
}


/**********************************************************************
*
*       SIM2ALN(cslp1, cslp2, K)
*       Computes top K alignment beteeen cslp1 and cslp2, return the Seq-align
*       object only
*
***********************************************************************/
SeqAlignPtr SIM2ALN (SeqLocPtr cslp1, SeqLocPtr cslp2, Int4 K)
{
	return SIM2(cslp1, cslp2, K, NULL, NULL, OUTPUT_ALIGN, NULL);
}





/**********************************************************************
*
*       SIM2ENDS(cslp1, csp2, K, filter)
*       Computes top K alignment between cslp1, cslp2 and return the
*       a linked list (ValNodePtr) of RecordEnds
*       filter: filter based on percent identity. set to NULL when  
*       it is not required
*
**********************************************************************/
ValNodePtr SIM2ENDS (SeqLocPtr cslp1, SeqLocPtr cslp2, Int4 K, FilterProc
filter)
{

	ValNodePtr new_list;	/*the list for storin the ends*/

	SIM2(cslp1, cslp2, K, NULL, &new_list, OUTPUT_ENDS, filter);

	return new_list;
}

