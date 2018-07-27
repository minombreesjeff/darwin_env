/* $Id: jzcoll.h,v 6.7 2000/11/16 22:10:38 shavirin Exp $
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
* File Name:  $RCSfile: jzcoll.h,v $
*
* Author:  Jinghui Zhang
*
* Initial Version Creation Date: 03/24/97
*
* $Revision: 6.7 $
*
* File Description:
*         File for various alignments
*
* $Log: jzcoll.h,v $
* Revision 6.7  2000/11/16 22:10:38  shavirin
* Moved many functions from txalign.c - due to move of txalign.c to
* distrib/tools directory and libncbitool.a library.
*
* Revision 6.6  2000/11/01 14:43:12  madden
* Changes from Futamura for psitblastn
*
* Revision 6.5  1999/08/06 17:58:22  egorov
* Print correct GI in formated output when user has specified list of gi's in blast search
*
* Revision 6.4  1998/07/22 18:57:34  kans
* reconciled in new functions from JZ
*
* Revision 6.3  1997/12/30 19:31:48  kans
* added CollectItemForSeqLocEx to restore propagation of features from parts to segmented bioseq
*
* Revision 6.2  1997/11/14 22:13:37  vakatov
* [WIN32,DLL]  Added NLM_EXTERN's
*
* Revision 6.1  1997/09/16 20:03:17  zjing
* new functions
*
* Revision 6.0  1997/08/25 18:06:17  madden
* Revision changed to 6.0
*
* Revision 5.13  1997/08/13 18:45:33  zjing
* add support for tblastx
*
* $Revision: 6.7 $
*
* File Description:
*         File for various alignments
*
* $Log: jzcoll.h,v $
* Revision 6.7  2000/11/16 22:10:38  shavirin
* Moved many functions from txalign.c - due to move of txalign.c to
* distrib/tools directory and libncbitool.a library.
*
* Revision 6.6  2000/11/01 14:43:12  madden
* Changes from Futamura for psitblastn
*
* Revision 6.5  1999/08/06 17:58:22  egorov
* Print correct GI in formated output when user has specified list of gi's in blast search
*
* Revision 6.4  1998/07/22 18:57:34  kans
* reconciled in new functions from JZ
*
* Revision 6.3  1997/12/30 19:31:48  kans
* added CollectItemForSeqLocEx to restore propagation of features from parts to segmented bioseq
*
* Revision 6.2  1997/11/14 22:13:37  vakatov
* [WIN32,DLL]  Added NLM_EXTERN's
*
* Revision 6.1  1997/09/16 20:03:17  zjing
* new functions
*
* Revision 5.10  1997/06/19 18:38:08  vakatov
* [WIN32,MSVC++]  Adopted for the "NCBIOBJ.LIB" DLL'ization
*
* Revision 5.9  1997/05/12 19:53:54  zjing
* change the maximum number of sequences in an alignment to 150.
*
 * Revision 5.8  1997/03/24  20:50:45  shavirin
 * Added protection for usage with C++ compiler. Added NCBI header
 * started logging.
 *
*
* ==========================================================================
*/

#ifndef _JZCOLL_

#define _JZCOLL_

/*local include file*/
#include <maputil.h>
#include <gather.h>
#include <seqport.h>


#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************
*
*       ck_seqfeat_extra: check if there is  extra data, such as
*       Genbank accessions assocated with a GeneRef or Medlines
*       associated with a Seq-feat
*
*******************************************************************/
#define NO_EXTRA_DATA	((Uint4)0)
#define EXTRA_MEDLINE	((Uint4)1)
#define EXTRA_GENBANK	((Uint4)2)

/*the different types of markers*/
#define MARK_TYPE_NUM		7
#define NO_TYPE			0
#define FRAME_WORK		1
#define RECMIN			2
#define LIKELY			3
#define MDUP			4
#define DUP			5
#define CONTIG_STS		6

/*corresponding type in extra field*/
#define EXTRA_FRAME_WORK	((Uint4)4)	/*it is  a frame work marker*/
#define EXTRA_RECMIN		((Uint4)8)	/*it is a recmin marker, for CHLC*/
#define EXTRA_LIKELY		((Uint4)16)	/*it is a likely loci marker, for CHLC*/
#define EXTRA_MDUP		((Uint4)32)
#define EXTRA_DUP		((Uint4)64)
#define EXTRA_CONTIG_STS	((Uint4)128)
#define EXTRA_LOD_SCORE		((Uint4)256)


/*the different types for a marker in its genetic context, EG for Eric Green*/
#define EG_YAC_END	1
#define EG_RANDOME	2
#define EG_GENETIC	3
#define EG_GENE		4
#define EG_EST		5
#define EG_MISC		6

#define EXTRA_YAC_END	((Uint4)256)
#define EXTRA_RANDOM	((Uint4)512)
#define EXTRA_GENETIC	((Uint4)1024)
#define EXTRA_GENE	((Uint4)2048)
#define EXTRA_EST	((Uint4)4096)
#define EXTRA_MISC	((Uint4)8192)

NLM_EXTERN Uint4 ck_seqfeat_extra PROTO((SeqFeatPtr sfp));
NLM_EXTERN Uint1 get_map_type PROTO((Uint4 extra));


#define ALL_LABEL       1
#define STR_LABEL       2
#define NUM_LABEL       3

#define NO_LABEL	255
#define UPPER_LABEL	1
#define LOWER_LABEL	2


#define NO_FILTER	0
typedef struct collectseqoption {	/*option for drawing the sequence+feature*/
	Boolean nointerval;	/*do not collect the intervals, only the two ends*/
	Uint1 slabel_format;		/*styles for label the sequence*/
	Uint1 flabel_format[FEATDEF_ANY];	/*the format for labels*/
	Uint1 seglevels;	/*level of segmentation to go down*/
	Int2 label_size;
	Int2 filter_level;	/*the level to be filtered out*/
	Boolean features[FEATDEF_ANY];	/*for filtering unwanted features*/
	Uint1 bsp_type;		/*type of Bioseq. This is to decide 
				 if the features need to be collected on 
				 the cytogenetic map*/
}CollectSeqOption, PNTR CollectSeqOptionPtr;


#define COLLECT_HISTORY 1	/*as a history*/
#define COLLECT_MP		2	/*multiple pairwise*/
#define COLLECT_MD		3	/*multi-dimension*/
#define COLLECT_FIXED	4	/*for the fixed format*/
#define COLLECT_MP_FLAT	5	/*flat out the mulitple pairwise alignment*/


#define DEFAULT_ALIGN_NUM 150        /*maximum number of alignment to be displayed*/
typedef struct collectalignoption {	/*option for drawing the sequence+feature*/
	Uint1 slabel_format;		/*format for label a sequence in the alignment*/
	Boolean nointerval;	/*do not collect the intervals, only the two ends*/
	Boolean only_history;	/*skip the Seq-annot?*/
	Boolean map_insert;	/*map the insertions*/
	Boolean flat_insert;	/*to flatout the insertions*/
	Boolean map_graphic;	/*map the coordinate to the graphic?*/

	Boolean show_mismatch;
	
	Boolean show_feature;		/*show any feature?*/
	Int4 	align_num;	/*the maximun number of alignment to collect*/
	Int4 curr_align_num;	/*the current number of the alignments*/
	SeqLocPtr segloc;			/*for marking the regions used in a segmented sequence*/
	Int2 label_size;
	Int4 graphic_offset;
	Boolean no_sort;		/*if TRUE, do NOT sort the AlignNode*/
	CollectSeqOptionPtr csop;	/*for collecting feature-related information*/

}CollectAlignOption, PNTR CollectAlignOptionPtr;


NLM_EXTERN Boolean set_option_for_collect_align PROTO((CollectAlignOptionPtr caop, Int2 label_size, Uint1 style));
/*######################################################################
#
#	collect functions
#
######################################################################*/

/***********************************************************************
*
*	CollectItemForSeqLoc(slp, entityID, left, is_aa, csop)
*	Collect sequences, features for a Seq-loc
*	slp: the target Seq-loc
*	entityID: the top level entityID for the current sequence
*	left: the left offset on the graph
*	is_aa: if TRUE, set get_feats_product flag to TRUE
*	csop: the collection option
*
*
***********************************************************************/	
NLM_EXTERN ValNodePtr CollectItemForSeqLoc PROTO((SeqLocPtr slp, Uint2 entityID, Int4 left, Boolean is_aa, SeqIdPtr maybe_mapid, CollectSeqOptionPtr csop, GeneDataPtr gdata, Uint2 priority));
NLM_EXTERN ValNodePtr CollectItemForSeqLocEx PROTO((SeqLocPtr slp, Uint2 entityID, Int4 left, Boolean is_aa, SeqIdPtr maybe_mapid, CollectSeqOptionPtr csop, GeneDataPtr gdata, Uint2 priority, Boolean forceSeglevelsTo1));



/*****************************************************************************
*
*	cllect_master_align_node(m_loc, featureOrder, groupOrder)
*	in the master-slave alignment, a fake Seq-align is created for the 
*	master sequence where the master is aligned to itself. The AlignNode
*	can be computed for this faked alignment. When this is done, the fake
*	Seq-align will be freed
*
*	m_loc: the Seq-loc for the master sequence
*	featureOrder: the selected features
*
******************************************************************************/
NLM_EXTERN ValNodePtr collect_master_align_node PROTO((CollectAlignOptionPtr caop, SeqLocPtr m_loc, Uint1 obj_type, Uint2 entityID));


NLM_EXTERN ValNodePtr CollectItemForAlignment PROTO((SeqLocPtr slp, Uint2 entityID, Int4 left, CollectAlignOptionPtr caop, Boolean take_all_annot));

NLM_EXTERN ValNodePtr collect_anpnode_with_option PROTO((CollectAlignOptionPtr caop, SeqLocPtr m_loc, Uint2 entityID, Int4 style, Uint1 itemType, Uint1Ptr f_order, Uint1Ptr g_order, Boolean take_all_annot));

typedef struct featnode {       /*for collecting the features*/
  Uint2       itemID;
  Uint2       entityID;
  Uint1       feattype;         /*type for Seq-feat*/
  Uint1	      subtype;		/*subtype for Seq-feat*/
  Int2            band;         /*band type for CytoGenetic Map, defined in maputil.h*/
  GatherRange extremes;
  ValNodePtr  interval; /*the intervals, data->ptrvalue is GatherRange*/
  CharPtr     label;
  CharPtr     pos_label;        /*label the position of a map */
  Int4        line;
  Int4	      top;		/*the top of the box*/
  Int4	      bottom;		/*the bottom of the box*/
  Int4        labelHeight;	/*the height of a label*/
  Int4        label_len;	/*the PIXEL length of the label*/
  Boolean     draw_tick;	/*draw the tick mark in a map*/
  Boolean     has_product;
  Boolean     follower;		/*is it a follower?*/
  Uint4       extra_data;	/*flags for extra data such as medline*/
  Boolean     landmark;		/*is it a landmark?*/
  Uint2       bin_order;	/*order for the 1000:1 bin data*/
  Char        annotDB[21];		/*the descriptor for the parent*/
  Int4	      ef_left;		/*extreme left, with the label font offset*/
  Int4        ef_right;
  ValNodePtr  supress_node;	/*the FeatNode uppressed to avoid stacking*/
} FeatNode, PNTR FeatNodePtr;

typedef struct ivalnode {	/*intervals in a feature*/
  GatherRange gr;
  Int4 line;
} IvalNode, PNTR IvalNodePtr;

/*********************************************************************** 
*
*       FreeFeatureList(list) 
*       free a list of FeatNode 
* 
***********************************************************************/ 
NLM_EXTERN ValNodePtr FreeFeatureList  PROTO((ValNodePtr list));

/*********************************************************************
*
*	extract_node_list(head, itemType, entityID, feattype, subtype, 
*	label_type)
*	extract a list of featnode from head which will have the 
*	selected itemType, entityID, feattye, subtype, label_type. 
*	set values to 0 if it is not considered in the selection
*
*********************************************************************/
NLM_EXTERN ValNodePtr extract_node_list PROTO((ValNodePtr PNTR head, Uint1 itemType, Uint2 entityID, Uint1 feattype, Uint1 label_type));

NLM_EXTERN ValNodePtr extract_lollipop_feature PROTO((ValNodePtr PNTR head, Int4 scale, BoolPtr lolli_feature));
	
/***********************************************************************
*
*	SortFeatNode(list)
*	sort a list of FeatNode to the ascending order of (extremes.left, 
*	extremes.right)
*
**********************************************************************/
NLM_EXTERN ValNodePtr SortFeatNode PROTO((ValNodePtr list, Uint1Ptr featureOrder, Uint1Ptr groupOrder));


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
NLM_EXTERN ValNodePtr merge_same_itemID PROTO((ValNodePtr PNTR head, Int2 itemID));


typedef struct alignblock{
  GatherRange gr;
  Uint2 order;
  struct alignblock PNTR next;
}AlignBlock, PNTR AlignBlockPtr;
  
#define MISMATCH_LINE	0	/* draw mismatch in alignment as a line*/
#define MISMATCH_OPEN	1	/*draw mismatch as an open circle*/
#define MISMATCH_CLOSE	2	/*draw mismatch as a closed circle*/
#define MISMATCH_SQUARE 3	/*draw mismatch as a square*/
/*value is reserved for Eric Greens Map only */
#define MISMATCH_AMB	4	/*for the ambiguous residue*/


/*the three values were used for the annotinfo.consistent values. 
  thouse were used to define the quality of the alignment between 
  the contigs and the chromosome in Eric Green's physical map. */
#define ALIGN_CONSISTENT	1
#define ALIGN_INCONSISTENT	2
#define ALIGN_UNKNOWN		3
#define ALIGN_FISH		4

#define ALIGN_BLASTN    1
#define ALIGN_BLASTP    2
#define ALIGN_BLASTX    3
#define ALIGN_TBLASTN   4
#define ALIGN_TBLASTX	5
#define ALIGN_PSITBLASTN   6

typedef struct annotinfo {	/*information stored in Seq-annot*/
	Uint1 displayOrder;
	Uint1 consistent;	/*the type of the consistency*/
	Char annotDB[21];
	Uint2 entityID;
	Uint2 itemID;
	Uint1 annot_type;	/*is it a BLAST/Consist/FISH alignment */
	Uint1 blast_type;	/*if it is alignment from BLAST, what is the type*/
	Boolean is_fish_align;	/*is it the alignment for the FISH map */
}AnnotInfo, PNTR AnnotInfoPtr;

/*******************************************************************
*
*	structure used to draw Sequence alignment
*	if follower is set to TRUE, it will be grouped together 
*	with the previous non-follower vnp. That enables the display
*	of the same sequences in multiple alignments
*
********************************************************************/
typedef struct alignseg {
  Uint1 type;			/*type of alignment. GAP? Insertion? Diag?*/
  GatherRange gr;
  Int2 featnum;			/*number of collected features*/
  ValNodePtr mismatch;		/*the position of mismatched residues*/
  Int4 ins_pos;			/*position of insertion*/
  Int4 top;
  Int4 bottom;
  Int4 line;
  ValNodePtr cnp;		/*list of FeatNode associated with the seg*/
  struct alignseg PNTR next;
} AlignSeg, PNTR AlignSegPtr;

/*values for the sequence index map*/

/*
*       the four mapping status
*
*/
#define MAP_STATUS_Mapping      1	/*magenta color*/
#define MAP_STATUS_Mapped       2	/*red color*/
#define MAP_STATUS_Paused       3	/*blue color*/
#define MAP_STATUS_None         4	/*black color*/

/*
*       the five sequencing status
*
*/
#define SEQ_STATUS_Plan         1	/*cyan color*/
#define SEQ_STATUS_Sequencing   2	/*magenta color*/
#define SEQ_STATUS_Sequenced    3	/*red color*/
#define SEQ_STATUS_Paused       4	/*blue color*/
#define SEQ_STATUS_None         5	/*black color*/


#define ALIGN_NON_INDEX		0
#define ALING_MAP_INDEX		1
#define ALIGN_SEQ_INDEX		2


typedef struct alignnode {      /*for collecting the alignment*/
  Uint1			index;	/*0, 1, 2 to indicate if it is a sequence/map index or 
				  a normal alignment */
  CharPtr		label;		/*label for the aligned sequence*/
  Boolean		keep_label;	/* says if a function can change the 'label' field */
  Uint1			m_frame;		/*mapped to the frame of translation in the master DNA sequence*/
  Boolean		is_master;	/*is it a master AlignNode*/
  GatherRange	extremes;
  AlignSegPtr	segs;		/*individual segment in alignment*/
  Int4			line;
  Int4			top;		/*the top of the box*/
  Int4			bottom;		/*the bottom of the box*/

  Int4			seqpos;			/*the position of the sequence*/
  SeqIdPtr		sip;			/*the SeqId of the aligned sequence*/
  Uint2			itemID;
  Uint2			entityID;
  Uint2			seq_entityID;		/*entity id for the sequence*/
  Uint2			bsp_itemID;		/*itemID for the current Bioseq*/
  Uint2			seqOrder;
  Uint2			chain;	
  Boolean		seq_has_align;	/*is the aligned segment itself contains alignment*/
  AlignBlockPtr blocks;
  SeqAnnotPtr	pop_sap;		/*a list of Seq-feat propagated from Seq-align*/
  Boolean		use_seq_ids;		/*use the seq_entityID, bsp_itemID to do the segmentation in the graphic view*/
  Boolean		follower;	/*TRUE, it follows the 
					  previous non-follower*/
 Int2			num_follower;	/*number of sequences follow this node*/	
  CharPtr		clone_id;	/*id of the clones*/
  Int4			align_num;	/*number of alignments. for recording the stop position*/
}AlignNode, PNTR AlignNodePtr;
/*********************************************************************
*
*       FreeAlignNode(list)
*       free a list of AlignNodePtr
*
*********************************************************************/
NLM_EXTERN ValNodePtr FreeAlignNode PROTO((ValNodePtr list));

/***********************************************************************
*
*	SortAlignNode(anp_list)
*	sort a list of AlignNode to the ascending order of (extremes.left, 
*	extremes.right)
*
**********************************************************************/
NLM_EXTERN ValNodePtr SortAlignNode PROTO((ValNodePtr anp_list));


/***************************************************************
*
*	CollAlignFromSeqAnnot(annot, m_loc, featureOrder, groupOrder, 
*	style,graphic)
*
*	collect the AlignNode for Seq-aligns stored in Seq-annot
*	annot: the Seq-annot
*	m_loc: the target sequence
*	left: the offset of the leftmost position
*	featureOrder, groupOrde: the features selected to be displayed together 
*	with alignment
*	style: the style of the display. Only valid for multiple-pairwise 
*	and multiple dimension for now
*	graphic: if TRUE, it is designed to show the display on graphic, 
*	so the mismatch data will be collected. Otherwise, it will not 
*	collect mismatch data
*	sort: if TRUE, sort the alignode, else no sorting
*	flat_insert: if TRUE, make the insertions in MP alignment flat
*
****************************************************************/
NLM_EXTERN ValNodePtr CollAlignFromSeqAnnot PROTO((SeqAnnotPtr annot, SeqLocPtr m_loc, Uint1Ptr featureOrder, Uint1Ptr groupOrder, Uint1 style, Boolean graphic, Boolean sort, Boolean flat_insert));



/*#####################################################################
#
#	functions related to the collection of the features of alignment
#
#####################################################################*/

/******************************************************************
*
*	CollectFeatureForAlignNode(slp, anp, csop)
*	collect feature for the alignment
*	slp: the target Seq-loc
*	anp: the AlignNode belong to the target Seq-loc
*	csop: the option for gathering the features
*	
******************************************************************/			
NLM_EXTERN Boolean CollectFeatureForAlignNode PROTO((SeqLocPtr slp, AlignNodePtr anp, CollectSeqOptionPtr csop));

/******************************************************************
*
*	CollectFeatureForAlign(slp, anp, featureOrder, groupOrder)
*	collect feature for the alignment
*	slp: the target Seq-loc
*	anp: the AlignNode belong to the target Seq-loc
*	featureOrder: the order of features
*	groupOrder: the order of the groups
*	
******************************************************************/			
NLM_EXTERN Boolean CollectFeatureForAlign PROTO((SeqLocPtr slp, AlignNodePtr anp, Uint1Ptr featureOrder, Uint1Ptr groupOrder)); 


/******************************************************************
*
*	SortAlignmentFeature(anp_node, featureOrder, groupOrder)
*	sort the list of FeatNode in aligned segment (asp->cnp) to the 
*	proper order of featureOrder and groupOrder
*
*******************************************************************/
NLM_EXTERN void SortAlignmentFeature PROTO((ValNodePtr anp_node, Uint1Ptr featureOrder, Uint1Ptr groupOrder));


/*******************************************************************
*
*	void CollectSegMapSTSAlign( entityID, anp_list)
*	look for the sts alignment from segmap stored as Seq-annot in 
*	in entityID. Add the alignment as the mismatch marker in the 
*	AlignSeg of the anp_list
*
*******************************************************************/
NLM_EXTERN void CollectSegMapSTSAlign PROTO((Uint2 entityID, ValNodePtr anp_list, SeqLocPtr m_loc, Int4 left));

/*****************************************************************
*
*	check if the AlignNode only contains Seq-annot or it 
*	has real sequence alignment. 
*	the empty Seq-annot may be the unaligned contigs in 
*	Eric Green's map
******************************************************************/
NLM_EXTERN Boolean alignode_has_alignments PROTO((ValNodePtr aligns));


/*********************************************************************** 
*
*       find_insert_ypos(left, seglen, ins, l_bound, r_bound, p_pos, space 
*       num) 
*       find the level for placing the insertions. Used in both the layout 
*       for text and graphic 
*       left: to store the left-most position calculated for an insertion 
*       seglen: length of the insertion 
*       ins: the position for insertions 
*       l_bound: the leftmost position in the current line 
*       r_bound: the rightmost position in the current line 
*       p_pos: position for storing all the layout info 
*       num: number of elements in p_pos
*       return the current level found for an insertion 
* 
***********************************************************************/ 
NLM_EXTERN Int2 find_insert_ypos PROTO((Int4Ptr left, Int4 seglen, Int4 ins, Int4 l_bound, Int4 r_bound, Int4Ptr p_pos, Int4 space, Int2 num));




/*****************************************************************
*
*	given the bioseq and its entityID, figure out the 
*	itemID for the Bioseq
*
*****************************************************************/
NLM_EXTERN Uint2 get_bioseq_itemID PROTO((BioseqPtr bsp, Uint2 entityID));


NLM_EXTERN ValNodePtr clean_annot_for_anp PROTO((ValNodePtr PNTR head));

/************************************************************************
*
*       convert_gdata_for_featnode(gdata, cyto_loc, offset)
*       gdata: the GeneDataPtr
*       cyto_loc: the current location on the cytogenetic map
*       offset: the offset of cyto_loc to the graphic viewer1
*       for human cytogenetic map, the markers are not shown. But for
*       the markers that were queried, it will display the interval for
*       gene data
*
************************************************************************/

NLM_EXTERN ValNodePtr convert_gdata_to_featnode PROTO((GeneDataPtr gdata, SeqLocPtr cyto_loc, Int4 offset));


#define ALIGN_NORMAL    0
#define ALIGN_DNA_TO_PROT       1
#define ALIGN_PROT_TO_DNA       2
#define ALIGN_TDNA_TO_TDNA	3
#define ALIGN_MAX_TYPE  3

NLM_EXTERN Uint1 get_alignment_type PROTO((AnnotInfoPtr annot_info));

/**********************************************************************
*
*	for a multiple pairwise alignment, convert the insertions 
*	into gaps on the master sequence
*
**********************************************************************/
NLM_EXTERN Boolean FlatAlignNode PROTO((ValNodePtr anp_list));

NLM_EXTERN void AddOffsetToAlignNode PROTO((AlignNodePtr anp, Int4 offset));
/*
*       Delete all the bad YACs from the list
*       anything on the NHGRI map that is recorded inconsistent will
*       be considered inconsistent. For the Whitehead map, the
*       inconsistent+ambiguous is inconsistent. Inconsistent alone
*       is not considered inconsistent
*	if any alignment is the same as the chromosome id (the segmap 
*	alignment includes the id mapped back to the chromosome), it 
*	will be cleaned as well
*/
NLM_EXTERN void CleanUpAmbiguousYAC PROTO((ValNodePtr PNTR anp_node, Uint1 db, SeqIdPtr chr_id));

/***********************************************************************
*
*	ProcessTextAlignNode(anp, left, right, p_stop, m_buf, locus)
*	process an AlignNode to generate a list of text buffer
*
*	anp: the AlignNode
*	left, right: the range of alignment in process. mapped to 
*	anp->extremes.left, and anp->extremes.right
*	p_stop: the previous stop position in the sequence. It is used 
*	to label the position of line which is a gap
*	m_buf: the buffer of the master sequence. Can be used to compare
*	mismatches
*	locus: if TRUE, use the locus name for sequence
*
*
*
************************************************************************/
NLM_EXTERN ValNodePtr ProcessTextAlignNode PROTO((
                    AlignNodePtr anp, Int4 m_left, 
                    Int4 m_right, Int4Ptr p_stop, 
                    CharPtr m_buf, Int4 line_len, 
                    Int1 m_frame, 
                    Uint4 option, Int4Ptr PNTR matrix
                    ));
NLM_EXTERN ValNodePtr ProcessTextAlignNode2 PROTO((
                    AlignNodePtr anp, Int4 m_left, 
                    Int4 m_right, Int4Ptr p_stop, 
                    CharPtr m_buf, Int4 line_len, 
                    Int1 m_frame, 
                    Uint4 option, Int4Ptr PNTR matrix,
                    Int4Ptr PNTR posMatrix, Int4 q_start
                    ));

NLM_EXTERN ValNodePtr FreeTextAlignList PROTO((ValNodePtr tdp_list));

NLM_EXTERN SeqIdPtr LIBCALL GetUseThisGi PROTO((SeqAlignPtr seqalign));
NLM_EXTERN SeqIdPtr LIBCALL ScorePtrUseThisGi PROTO((ScorePtr sp));


/* setting up the matrix for the positive residue of the alignment */

NLM_EXTERN Int4Ptr PNTR load_default_matrix PROTO((void));
NLM_EXTERN void free_default_matrix PROTO((Int4Ptr PNTR matrix));

#ifdef __cplusplus
}
#endif

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

#endif



 
