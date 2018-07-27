#ifndef _LAYOUT_
#define _LAYOUT_
#include <jzcoll.h>

/*#####################################################################
#
#       functions related to the layout for FeatNode
#
######################################################################*/

/**********************************************************************
*
*       LayoutNodeFlat(head, maxScale, top, seq_pos, downward, label_pos)
*       layout the FeatNode as a stacked layout
*       head: the list of FeatNode. Will be resorted
*       maxScale: the maximum scale in the picture
*       top: current available position at y-axis
*       seq_pos: store the position for drawing the sequence after the
*       stacking is over
*       downward: stack the genes below the sequence line
*       label_pos:  if(TRUE), label the map position for each cluster
*
**********************************************************************/
Int4 LayoutNodeFlat PROTO((ValNodePtr PNTR head, Int4 maxScale, Int4 top, Uint1 label_type, Boolean supress));

Int4 LayoutAlignFlat PROTO((ValNodePtr head, Int4 maxScale, Int4 top));

/***********************************************************************
*
*       Int4 LayoutFeatNode(head, top, next_sp, space, maxScale, check_interval)
*       Layout the fnp->line, inp->line according to Maximun packing, return
*       the next availabel space for drawing
*       head: the FeatNode. will be sorted
*       top: the current y position
*       next_sp: min. space for separating two neibouring objects
*       space: the vertical space consumed by each line
*       maxScale: max scale for the picture
*       check_interval: layout each interval?
*
*
************************************************************************/
Int4 LayoutFeatNode PROTO((ValNodePtr head, Int4 top, Int4 maxScale, Int4 next_sp, Boolean show_label, Boolean compress));

void ReSetFeatNode PROTO((ValNodePtr fnp_node));

ValNodePtr GroupFeatNode PROTO((ValNodePtr PNTR head));
ValNodePtr OrderCdsmRNA PROTO((ValNodePtr PNTR head));

ValNodePtr strip_repeat_feature PROTO((ValNodePtr PNTR f_node));
/**********************************************************************
*
*	LayoutAlignNode(head, top, maxScale)
*	head: the list of AlignNode
*	top: the current y position
*	maxScale: the maximum scale
*	the labels on the features of a sequence is not counted in the layout
*
***********************************************************************/
Int4 LayoutAlignNode  PROTO((ValNodePtr head, Int4 top, Int4 maxScale, Int4 height));

void SortSameSeqInAlignNode PROTO((ValNodePtr anp_list));

void modify_insert_fnode PROTO((ValNodePtr fnode, Int4 offset));

Int2 get_current_class PROTO((FeatNodePtr fnp));



#endif
