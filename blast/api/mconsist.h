#ifndef _MCONSIST_
#define _MCONSIST_

#include <sequtil.h>
#include <objalign.h>


#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif

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
NLM_EXTERN Boolean get_anchor_coordinates PROTO((SeqAlignPtr h_align, SeqIdPtr anchor_id, SeqIdPtr sip, Int4Ptr PNTR x_anchor_pos, Int4Ptr PNTR x_pos, Uint2Ptr num));

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
NLM_EXTERN Int4 find_this_position_by_anchor PROTO((Int4Ptr anchor_pos, Int4Ptr pos, Uint2 num, Int4 this_pos, Int4 length));

NLM_EXTERN Boolean find_flanking_anchor_pos PROTO((Int4Ptr anchor_pos, Uint2 num, Int4 c_pos, Int4 length, Int4Ptr f_left, Int4Ptr f_right));

/**********************************************************************
*
*       MapLocToAnchor(annot, slp, anchor)
*       map the current slp to a position on the anchor Bioseq
*       annot: Seq-annot that may contain the alignment of the consistent markers
*       slp: the current Bioseq
*       anchor_id: the Seq-id for the anchor Bioseq, that is the sequece map
*
************************************************************************/
NLM_EXTERN SeqLocPtr MapLocToAnchor PROTO((SeqAnnotPtr annot, SeqLocPtr slp, BioseqPtr anchor_bsp));


/*****************************************************************
*
*       map a position on the anchor_bsp (anchor_pos) to a
*       position on the other_bsp. It is the reverse operation of
*       MapLocToAnchor
*       return -1 for failure
*
******************************************************************/
NLM_EXTERN Int4 MapAnchorToLoc PROTO((SeqAnnotPtr annot, Int4 anchor_pos, BioseqPtr anchor_bsp, BioseqPtr other_bsp));

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

#endif

