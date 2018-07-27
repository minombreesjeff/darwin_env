
#ifndef _DRAWSEQ_
#define _DRAWSEQ_

/*local include files*/
#include <jzcoll.h>
#include <fstyle.h>
#include <layout.h>

/* #include <viewer.h> */
#include <picture.h>
#include <objmgr.h>


/**************************************************************************
*
*	DrawSequinMap(slp_list, sep, scale)
*	return a picture with the interface defined by Jonathan K. 
*	slp_list: a ValNode list of Seq-loc
*	sep: the Seq-entry pointer
*	scale: the scale of the picture
*
**************************************************************************/
SegmenT DrawSequinMap PROTO((ValNodePtr slp_list, SeqEntryPtr sep, Int4 scale, GeneDataPtr gdata, ValNodePtr PNTR ftype_list));

SegmenT DrawSequinMapEx PROTO((ValNodePtr slp_list, SeqEntryPtr sep, Int4 scale, GeneDataPtr gdata, ValNodePtr PNTR ftype_list,
Boolean forceSeglevelsTo1, ValNodePtr extraEntityList));


/*######################################################################
#
#       functions for setting up the color for different object
#
######################################################################*/

/************************************************************************
*
*       get_seg_color(order)
*       set up the color for a segmented sequence
*
*************************************************************************/
extern Uint1 MUSK_COLOR[3];
Uint1Ptr get_seg_color PROTO((Int2 order));



/**********************************************************************
*
*       get the color for different enzymes
*       used in drawing the restriction map
*
**********************************************************************/
Uint1Ptr get_enz_color PROTO((Int2 enzID));


void add_attribute_pen PROTO((SegmenT seg, Int2 p_class, Int2 sub_class));


/*####################################################################
#
#	functions related to the drawing of FeatNode and AlignNode
#
#####################################################################*/

/**********************************************************************
*
*       DrawFlatNode(vnp, seg, label_pos, downward)
*       Draw a list of FeatNode as a stacked gene view
*       vnp: a list of FeatNode
*       seg: the Segment for drawing each item
*       label_pos: if(TURE), label the position of each cluster
*       downward: if(TRUE), label the cluster underneath the tickmark
*
***********************************************************************/
Boolean DrawFlatNode PROTO((ValNodePtr vnp, SegmenT seg, Uint1 label_type, Int4 scale, GeneDataPtr gdata));

/*************************************************************************
*
*       DrawFeatNode(vnp, type, rec_width, maxScale, seg, label_type, fill)
*       Draw a list of featnode, return TRUE for success
*       vnp: a list of featnode
*       rec_width: width for drawing a rectangle. if <2, draw a line
*       maxScale: max scale
*       seg: drawing segment
*       label_type: Add label on the segment
*       fill: fill the rectangle?
*
*************************************************************************/
Boolean DrawFeatNode PROTO((ValNodePtr fnp_node, SegmenT seg, Uint1 label_type, Boolean show_arrow, Int4 scale, GeneDataPtr gdata));
/***********************************************************************
*
*       DrawFeatures(features, mpp, pic, flat, simple, compact, f_order,
*       maxScale)
*
*       Draw the features in the a sequence
*       features: the FeatNdoe contains the features
*       mpp: the graphic position of the current sequence
*       pic: the drawing segment
*       flat: if TRUE, stack features on top
*       simple: if TRUE, draw the feature intervals as line
*       compact: if TRUE, does not distinguish subtype of different features
*       f_order: the order for drawing different features
*       maxScale: the maximum scale for drawing
*       image_list: the list of AlignRect to store the image information
*
*********************************************************************/
Boolean DrawFeatures PROTO((ValNodePtr features, MapPosPtr mpp, SegmenT pic, Boolean flat, Uint1Ptr featureOrder, Uint1Ptr groupOrder, Int4 scale, GeneDataPtr gdata, ValNodePtr PNTR image_list));


/**********************************************************************
*
*	DrawAlignNode(vnp, scale, seg)
*	Draw a list of AlignNode. 
*	1) It always labels the sequences
*	2) the label is always on top of the sequence
*	3) right now, it does NOT show the truncation
*
**********************************************************************/
Boolean DrawAlignNode PROTO((ValNodePtr vnp, Int4 scale, SegmenT seg));
Boolean DrawFlatAlign PROTO((SegmenT seg, ValNodePtr anp_list));



/*####################################################################
#
#	functions used in chromoscope to draw the sequences, maps
#
####################################################################*/
/************************************************************************* 
* 
*       DrawGeneticMap(features, mpp, pic, maxScale) 
*       draw the genetic map from featnode 
*       features: the list of FeatNode, will be resorted 
*       mpp: the current map position. The drawing will recalculate
*       mpp->seq_top and mpp->bottom 
*       pic: the drawing picture 
*       maxScale: the maximum scale in drawing. Will be used in the layout 
*	image_list: the list of AlignRect to store the image information
* 
*************************************************************************/ 
Boolean DrawGeneticMap PROTO((ValNodePtr PNTR features, MapPosPtr mpp, SegmenT pic, Int4 maxScale, GeneDataPtr gdata, ValNodePtr PNTR image_list));


/*************************************************************************
*
*       DrawPhysicalMap(features, mpp, pic, maxScale)
*       draw the physical map from featnode
*       features: the list of FeatNode, will be resorted
*       mpp: the current map position. The drawing will recalculate
*       mpp->seq_top and mpp->bottom
*       pic: the drawing picture
*       maxScale: the maximum scale in drawing. Will be used in the layout
*       image_list: the list of AlignRect to store the image information
*
*************************************************************************/
Boolean DrawPhysicalMap PROTO((ValNodePtr PNTR features, MapPosPtr mpp, SegmenT pic, Int4 maxScale, ValNodePtr PNTR image_list));


/*********************************************************************** 
* 
*       DrawRestrictionMap(features, mpp, pic, rsite_flat, strand) 
*       draw the restriction map 
*       features: the FeatNode contains the info for restriction map 
*       mpp: the map position 
*       pic: picture
*       rsite_flat: if(TRUE), all the enzymes are shown in one line. 
*       strand: the orientation of the map
* 
***********************************************************************/ 
Boolean DrawRestrictionMap PROTO((ValNodePtr PNTR features, MapPosPtr mpp, SegmenT pic, Boolean rsite_flat, Uint1 strand, Int4 maxScale));



/*********************************************************************** 
* 
*       DrawCytoMap(features, mpp, maxScale, pic) 
*       draw the cytogenetic map
*       features: the FeatNode for cytogenetic map 
*       maxScale: maximum scale, used in layout 
*       pic: the drawing segment
* 
***********************************************************************/
Boolean DrawCytoMap PROTO((ValNodePtr PNTR features, MapPosPtr mpp, Int4 maxScale, SegmenT pic));


/*********************************************************************
*
*       DrawSeqMap(features, mpp, pic, flat, maxScale, is_raw_seq)
*       Draw the real sequence map
*       features: FeatNode from which the Bioseqs can be extracted
*       mpp: the MapPos that contains the current map position 
*       pic: the picture 
*       flat: if TRUE, draw the map with features stacked on top
*       maxScale: maximum scale of the sequence
*       is_raw_seq: if TRUE, it is a raw DNA sequence. (This is  
*       used to distinguish raw sequence from virtual sequence 
* 
*********************************************************************/ 
Boolean DrawSeqMap PROTO((ValNodePtr PNTR features, MapPosPtr mpp, SegmenT pic, Int4 scale, Boolean is_raw_seq, Boolean show_segment, ValNodePtr PNTR image_list));


/***********************************************************************
*
*       DrawSeqScale(seg, slp, left, ypos, scaleX) 
*       draw a scale for the Seq-loc. It can be a list of Seq-loc 
*       seg: the drawing segment
*       slp: the Seq-loc
*       left: the left offset 
*       ypos: the top position 
*       scaleX: the current scale of the picture 
* 
************************************************************************/
void DrawSeqScale PROTO((SegmenT seg, SeqLocPtr slp, Int4 left, Int4 ypos, Int4 scaleX, Boolean add_x_line));



/*************************************************************************
*
*       DrawVerticalAlign (align, pic, mlp)
*       draw alignment among the sequences. (connecting aligned seg by line)
*       align: the Seq-align which contains the alignment among sequence 
*       It is assumed to be a Std-seg 
*       pic: the drawing segment 
*       mlp: the list of the layout of the sequences
*
*************************************************************************/
Boolean DrawVerticalAlign PROTO((SeqAlignPtr align, SegmenT pic, MapLayoutPtr mlp));

/************************************************************
*
* collect_alignnode_from_alp(m_loc)
* collect all the alignment stored as the history of the
* master sequence
* m_loc: the selected location of the master sequence
* return a list of AlignNode
*
************************************************************/
ValNodePtr collect_alignnode_from_slp PROTO((SeqLocPtr m_loc, Uint2Ptr t_entityID, Boolean flat_insert));

SegmenT DrawSeqHistoryAlignment PROTO((SeqLocPtr m_loc, Int4 scale, ValNodePtr PNTR anp_list, Uint2Ptr entityID, Boolean flat_insert));

Boolean load_align_option_for_graphic PROTO((CollectAlignOptionPtr caop, CollectSeqOptionPtr csop, Int4 style, Boolean flat_insert));

Boolean DrawMPAlignment PROTO((ValNodePtr anp_node, Int4 left, Int4 right, SeqLocPtr m_loc, Uint2 entityID, Int4 scale, Int4Ptr cur_pos, Uint1 style, Boolean compress, SegmenT pic));

Boolean DrawHistory PROTO((ValNodePtr aligns, MapPosPtr mpp, Int4 seq_label_len, SegmenT pic, Int4 scale, ValNodePtr PNTR image_list));



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
*       ftype_list: return a list of type of features for the current
*       Seq-loc. This is going to be used for drawing the legend
*       image_list: the list to store the image mapping information

*
*********************************************************************/
SegmenT DrawGenomeMap PROTO((ValNodePtr slp_list, ValNodePtr e_align_list, Int4
scale, GeneDataPtr gdata, MapLayoutPtr PNTR mlp, ValNodePtr PNTR ftyle_list, ValNodePtr PNTR image_list));

SegmenT DrawGenomeMapEx PROTO((ValNodePtr slp_list, ValNodePtr e_align_list, Int4
scale, GeneDataPtr gdata, MapLayoutPtr PNTR mlp, ValNodePtr PNTR ftyle_list, ValNodePtr PNTR image_list,
Boolean forceSeglevelsTo1, ValNodePtr extraEntityList));

/*************************************************************************
*
*       Label_GData(): Label the gene_data node specified by the user
*
*************************************************************************/
Boolean Label_GData PROTO((GeneDataPtr gdata, SeqLocPtr slp, Int4 left, Int4 top, SegmenT seg));

#define MAX_SCALE       5000
Int4 FigureMaxScale PROTO((ValNodePtr slp_list, Int2 view_width, Int4 max_width));
Int4 FigureMinScale PROTO((ValNodePtr slp_list, Int4 max_label));

Int4 CountMaxSeqLabel PROTO((ValNodePtr slp_list));
 
/*************************************************************************
*
*       Find_segment_IDs(): Returns identifier triplet given a mouse point
*
*************************************************************************/
/* extern Boolean Find_segment_IDs PROTO((VieweR viewer, PoinT pt, Uint2Ptr entityID, Uint2Ptr itemID, Uint2Ptr itemType)); */

void draw_one_align PROTO((AlignPos ap, Int2 num, Int2 order, SegmenT seg));


#define UNKNOWN_DB	0
#define ENTREZ_DB	1	/*entrez is the database*/
#define THC_DB		2	/*db is the THC tigr database*/
#define ENTREZ_KLUDGE_ID	3	/*for kludge id in Entrez */
#define ENTREZ_DB_P	4	/*entrez proteins is the database*/
typedef struct alignlabelrect {
	Uint1 link_db;	/*database for the gi*/
	Int4 gi;
	Int4 left;
	Int4 top;
	Int4 right;
	Int4 bottom;
	Uint2 itemType;
	Uint2 itemID;
	CharPtr label;
	Int4 from, to;	/*the interval on the sequence. For showing partial Seq-loc*/
}AlignLabelRect, PNTR AlignLabelRectPtr;

/************************************************************************
*
*       load the information for image mapping
*
************************************************************************/
Boolean load_align_label_rectangle PROTO((ValNodePtr data_list, ValNodePtr PNTR arect_list, Int4 scale, Boolean flat));
/*
*	for the contig maps, the real contig length may not correspond
*	with the mapped interval on the graphics. Need a way to find the 
*	store the interval mapped in proportion to what is in the graphic
*	the intervals are mapped by the function: FindContigList
*/
void AddIntervalForImage PROTO((ValNodePtr contig_list, ValNodePtr image_list));


SegmenT PicForAlignNode PROTO((ValNodePtr anp_list, SeqLocPtr m_loc, Int4 width, Int4Ptr p_scale, Int4Ptr p_maxwidth, ValNodePtr PNTR arect_list));


/***********************************************************************
*
*	functions to draw a compressed alignment viewer
*
************************************************************************/
SegmenT DrawCompressAlignment PROTO((ValNodePtr anp_list, SeqLocPtr m_loc, Int4 width, 
									Int4Ptr p_scale, Int4Ptr p_maxwidth, Uint2 entityID, ValNodePtr PNTR arect_list));

ValNodePtr MakeCompressAlignList PROTO((BioseqPtr query_bsp, SeqAnnotPtr annot, Uint2Ptr p_entityID));

/*produce a picture which shows the contigs that align to the 
* cytogenetic band. It will expand left to the half band size and 
* right to the half band size. pic_width is the width of the picture
*/
SegmenT DrawCytoContigMap PROTO((ValNodePtr slp_list, BioseqPtr cyto_bsp, 
			Int4 pic_width, Int4Ptr pscale, ValNodePtr PNTR image_list));
#endif
