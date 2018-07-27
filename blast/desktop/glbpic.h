#ifndef _GLBPIC_
#define _GLBPIC_

#include <maputil.h>
#include <gather.h>
#include <jzcoll.h>
#include <picture.h>

#ifdef __cplusplus
extern "C" {
#endif


typedef struct globalbsp{               /*the map drawing unit for global view*/ 
	BioseqPtr bsp;
	CharPtr label;			/*label for the sequence*/
	RecT rec;                       /*for drawing a linear molecule*/
	Int2 radius;                    /*for drawing a circular molecule*/
	Uint1 map_type;                 /*determine the type of the map*/
	Boolean show_map_unit;          /*show the map unit??*/
	Boolean check_cyto;             /*check if it is a cyto-genetic map*/
	ValNodePtr l_marks;             /*the list of MapMarks, such as landmarks, band in 
        								cytogenetic map*/
	ValNodePtr g_marks;				/*the list of marks that are picked by the user*/
	ValNodePtr cyto_marks;			/*marks for the cytogenetic map*/
	Boolean hide;					/*hide the current Bioseq*/
	Uint2 priority;			/*priority for search a particular gene*/
	ValNodePtr rrp_list;	/*list for showing the repeat regions*/
	ValNodePtr arp_list;	/*list for showing the alignment in the global view*/
	Boolean has_fish_align;	/*the arp_list is a FISH alignment*/
	Boolean is_seqindex_map;	/*is it a sequence index map*/
	Boolean align_has_status;	/*if the alignment arp_list has status*/
	Int2 num_lod_score;	/*number of the LOD scores for a genethon map*/
}GlobalBsp, PNTR GlobalBspPtr;


typedef struct globaldraw{
	Boolean is_circle;
	PoinT center;
	ValNodePtr gbp_list;	/*the list of the globalbsp data*/
	GeneDataPtr g_data;
	ValNodePtr g_list;		/*list of user-selected genes*/
	
	Boolean free_data;		/*if TRUE, free msp_list and equiv_align_list*/
	ValNodePtr msp_list;	/*manually loaded Seq-entry*/
	ValNodePtr equiv_align_list;	/*manually loaded Seq-annot + alignment parsed out*/
	ValNodePtr ext_align;	/*alignment taken from the external*/
	
	ValNodePtr slp_list;	/*list of selected intervals */
	Uint1 release_choice;	/* 0 = graphic view, 1=alignment view*/
	Boolean draw_align_legend;	/*add the legend for the colors used in 					  different score scale*/
}GlobalDraw, PNTR GlobalDrawPtr;

typedef struct mapmark{                 /*marks labeled on the global view*/
        Char label[20];                 /*label the mark*/
        Int4 pos;                       /*position in basepair coordinate*/
        Boolean inward;                 /*the orientation for mark label*/
}MapMark, PNTR MapMarkPtr;

/*************************************************************************
*
*	default variables associated with the layout of the global picture
*
**************************************************************************/
#define LINEAR_BSP 	1		/*a linear Bioseq*/
#define CIRCLE_BSP 	2		/* a circular Bioseq*/

#define PI 		((FloatHi)3.14159)

/*options for showing the Equiv-align */
#define EQUIV_ALIGN_DEFAULT	0	/*show the consistent markers and only 
					some of the inconsistent markers*/
#define EQUIV_ALIGN_ALL		1	/*show all the equiv-aligns*/
/***************************************************************************
*
*	CreateGlobalDrawData(msp_list, ext_align, user_list, need_free)
*	Create the GlobalDrawPtr structure to draw the global view
*	msp_list: externally loaded Seq-entry and its file name (used for saving)
*	ext_align: the externally loaded sequence-alignment for equiv maps
*	user_list: a list of user-selected gene symbols
*	need_free: if TRUE, the free callback will free msp_list, ext_align and 
*	user_list.
*
****************************************************************************/		
GlobalDrawPtr CreateGlobalDrawData PROTO((ValNodePtr msp_list, ValNodePtr ext_align, ValNodePtr user_list, Boolean need_free, Boolean for_picture, Uint1 equiv_align_option));

/**************************************************************************
*
*	ree_global_draw(gdraw_p)
*	free all the data related to the global draw
*
***************************************************************************/
GlobalDrawPtr free_global_draw PROTO((GlobalDrawPtr gdraw_p));

BioseqPtr has_cyto_map PROTO((GlobalDrawPtr gdraw_p));


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
Boolean LayoutGlobalDrawData PROTO((GlobalDrawPtr gdraw_p, Int2 pan_width, Int2 pan_height));

/************************************************************************
*
*	Draw_Global(gdraw_p)
*	return a picture created from GlobalDrawPtr
*
************************************************************************/
SegmenT Draw_Global PROTO((GlobalDrawPtr gdraw_p));


SegmenT MuskGlobalPicture PROTO((ValNodePtr msp_list, ValNodePtr ext_align, Boolean need_free, Int2 panel_width, Int2 panel_height, ValNodePtr user_list, GlobalDrawPtr PNTR vwr_extra,  Uint1 equiv_align_option, Boolean draw_align_legend));

SegmenT SequinGlobalPicture PROTO((SeqEntryPtr sep, Int2 panel_width, Int2 panel_height, ValNodePtr user_list, GlobalDrawPtr PNTR vwr_extra, Boolean draw_align_legend));

SegmenT GlobalPictureUpdate PROTO((GlobalDrawPtr gdraw_p, ValNodePtr new_gene_list, Int2 panel_width, Int2 panel_height));


/*********************************************************************
*
*
*       find_map_pos(convert the start_pnt, stop_pnt into a list of
*       Seq-locs)
*
*********************************************************************/
ValNodePtr find_map_pos PROTO((GlobalDrawPtr gdraw_p, PntInfo start_pnt, PntInfo stop_pnt));

GeneDataPtr create_gene_data PROTO((ValNodePtr msp_list, ValNodePtr mark_list));

#ifdef __cplusplus
}
#endif

#endif

