/* $Id: maputil.h,v 6.8 2001/06/26 16:42:59 vakatov Exp $
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
* File Name:  $RCSfile: maputil.h,v $
*
* Author:  Jinghui Zhang
*
* Initial Version Creation Date: 03/21/97
*
* $Revision: 6.8 $
*
* File Description:
*         External include file for graphical alignments
*
* $Log: maputil.h,v $
* Revision 6.8  2001/06/26 16:42:59  vakatov
* POINT --> BAND_POINT  (to avoid conflicts with MS-Win standard headers)
*
* Revision 6.7  1998/07/22 00:02:37  zjing
* move SortValNode from maputil to jzmisc
*
* Revision 6.6  1997/11/14 22:13:41  vakatov
* [WIN32,DLL]  Added NLM_EXTERN's
*
* Revision 6.5  1997/10/15 19:56:42  zjing
* change get_align_annot_qual parameter
*
* Revision 6.4  1997/10/10 18:32:42  zjing
* add support to collect OBJ_BIOSEQ_DELTA and functions to improve the alignment overview
*
* Revision 6.3  1997/10/06 14:42:39  zjing
* functions for the overview of blast search results
*
* Revision 6.0  1997/08/25 18:06:37  madden
* Revision changed to 6.0
*
* Revision 5.9  1997/07/23 13:45:29  zjing
* look up for the sequence index map
*
* $Revision: 6.8 $
*
* File Description:
*         External include file for graphical alignments
*
* $Log: maputil.h,v $
* Revision 6.8  2001/06/26 16:42:59  vakatov
* POINT --> BAND_POINT  (to avoid conflicts with MS-Win standard headers)
*
* Revision 6.7  1998/07/22 00:02:37  zjing
* move SortValNode from maputil to jzmisc
*
* Revision 6.6  1997/11/14 22:13:41  vakatov
* [WIN32,DLL]  Added NLM_EXTERN's
*
* Revision 6.5  1997/10/15 19:56:42  zjing
* change get_align_annot_qual parameter
*
* Revision 6.4  1997/10/10 18:32:42  zjing
* add support to collect OBJ_BIOSEQ_DELTA and functions to improve the alignment overview
*
* Revision 6.3  1997/10/06 14:42:39  zjing
* functions for the overview of blast search results
*
* Revision 5.8  1997/06/19 18:38:23  vakatov
* [WIN32,MSVC++]  Adopted for the "NCBIOBJ.LIB" DLL'ization
*
* Revision 5.7  1997/03/21 19:47:16  shavirin
* Added NCBI header started logging and added protection for usage
* it with C++ compiler
*
*
* ==========================================================================
*/
#ifndef _MAPUTIL_
#define _MAPUTIL_

/****************************************************************************/
/* INCLUDES */
/****************************************************************************/

#include <ncbi.h>
#include <objfeat.h>
#include <objseq.h>
#include <objloc.h>
#include <objalign.h>
#include <sequtil.h>
#include <objfdef.h>
#include <lsqfetch.h>
#include <objmgr.h>
#include <gather.h>

/*local include file*/
#include <jzmisc.h>

/****************************************************************************/
/* TYPEDEFS */
/****************************************************************************/
typedef struct genedata{
        CharPtr symbol;
        Boolean landmark; /*is it a landmark gene?*/
        Uint2 entityID;
        Uint2 itemID;
        Uint2 itemType;
        Uint2 subtype;
	Uint2 priority;
	Int4 x, y;	/*the x, y coordinates on the picture*/
        SeqLocPtr location;
	SeqFeatPtr sfp;
        ValNodePtr align_seg;
        struct genedata PNTR next;
}GeneData, PNTR GeneDataPtr;

typedef ValNodePtr EnzPtr;

typedef struct enzdata{
        CharPtr name;
        CharPtr  pattern;
        Uint1 cut_pos;
}EnzData, PNTR EnzDataPtr;

typedef struct musksep{	        /*Seq-entries from musk*/
	SeqEntryPtr sep;
	Boolean is_bin;		/*is it loaded from a binary file?*/
	Char file_name[100];	/*the name for the ASN.1 Seq-entry file*/
	Pointer dataptr;
	Uint2 datatype;
} MuskSep, PNTR MuskSepPtr;

/****************************************************************************/
/* FINCTION DEFINITIONS */
/****************************************************************************/

#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif

#ifdef __cplusplus
extern "C" {
#endif

NLM_EXTERN Int2 get_seg_num PROTO((SeqLocPtr slp));

NLM_EXTERN Int2 get_seglevels PROTO((BioseqPtr bsp));

NLM_EXTERN Int4Ptr get_priority_order PROTO((SeqEntryPtr sep, Int4Ptr num));

/****************************************************************
*
*	LoadLandMarkGene(sep)
*	get the landmark gene from the User-object in the descriptor
*	all the genes are linked to a ValNode and vnp->choice is set 
*	to 1 to indicate it is a landmark gene
*
******************************************************************/
NLM_EXTERN ValNodePtr LoadLandMarkGene PROTO((SeqEntryPtr sep));

/*****************************************************************
*
*       if sfp is a Gene-ref and contains the gene in g_list,
*       return the string in g_list
*       else return NULL
*
*****************************************************************/
NLM_EXTERN Boolean check_landmark PROTO((SeqFeatPtr sfp, CharPtr mark));


/******************************************************************
*
*	load_gdata_marks(slp, gene_list, seglevels, sep, gdp)
*	Gather the current Seq-entry to create the corresponding list of 
*	GeneDataPtr for the list of gene symbols
*	slp: the target Seq-local. can be set to NULL
*	gene_list: a list of query symbols
*	seglevels: levels of gather
*	sep: the Seq-entry
*	gdp: the header of GeneDataPtr
*
********************************************************************/	
NLM_EXTERN Boolean load_gdata_marks PROTO((SeqLocPtr slp, 
                                           ValNodePtr gene_list, 
                                           Int2 seglevels, 
                                           SeqEntryPtr sep, 
                                           GeneDataPtr PNTR pgdp));

NLM_EXTERN GeneDataPtr LinkGeneData PROTO((GeneDataPtr PNTR head, GeneDataPtr newgdp));

NLM_EXTERN GeneDataPtr GeneDataFree PROTO((GeneDataPtr head));

NLM_EXTERN GeneDataPtr make_gene_data PROTO((ValNodePtr gene_list));

NLM_EXTERN void RefreshGeneData PROTO((GeneDataPtr gdp));

/*************************************************************************
*
*       return the best location of the gene from sep.
*       best is defined as the Bioseq with the highest searching prioirity
*       e_start, e_stop record the extremes of the all the presence of
*       the gene
*
*************************************************************************/
NLM_EXTERN SeqLocPtr get_location_for_query PROTO((SeqEntryPtr sep, 
                                                   CharPtr gene, 
                                                   Int4Ptr e_start, 
                                                   Int4Ptr e_stop));

/*****************************************************************
*
*	Build a list of gene symbols to supply the Find Gene option in 
*   the global view
*
******************************************************************/
NLM_EXTERN ValNodePtr BuildGeneList PROTO((SeqEntryPtr sep));


/****************************************************
*
*       make_enzyme_list(file_name)
*       build a ValNodeList of EnzDataPtr from teh 
*       input file
*
****************************************************/
NLM_EXTERN EnzPtr make_enzyme_list PROTO((CharPtr file_name));

/**************************************************
*
*       free_enzyme_list(enp)
*       Free a list of EnzDataPtr
*
**************************************************/
NLM_EXTERN EnzPtr free_enzyme_list PROTO((EnzPtr enp));


/**********************************************************************
*
*	FreeEquivAlign(ealign_list)
*	Free a list of Seq-annot that is of type Seq-align
*	mostly, those are the externally loaded Seq-align for showing
*	the Equiv map
*
***********************************************************************/
NLM_EXTERN ValNodePtr FreeEquivAlign PROTO((ValNodePtr ealign_list));

/*******************************************************************
*
*       GetEquivAlignType(annot)
*       annot stores the alignments of markers mapped by more than
*       one groups. if return 1, the markers are the consistent markers
*       if return 2, the markers are inconsistent markers
*       if return 0, the alignment simply records the mapping to
*       the sequence map
*       if return -1, unknown status. Will be treated the same as 1
*
*******************************************************************/
NLM_EXTERN Int2 GetEquivAlignType PROTO((SeqAnnotPtr annot));


/***********************************************************************
*
*	FreeMuskSep(sep_list)
*	Free the manually loaded Seq-entries
*	sep_list: a list of MuskSepPtr
*
***********************************************************************/
NLM_EXTERN ValNodePtr FreeMuskSep PROTO((ValNodePtr sep_list));

/*#####################################################################
#
#	functions related to BioseqDraw
#
#
######################################################################*/

/**********************************************************************
*
*       get_Bioseq_type(bsp)
*       return the subclasses of Bioseq, such as the genetic or physical map
*       depending on its bsp->repr type and bsp->seq_ext type.
*
**********************************************************************/
#define PHYSICAL_MAP    1
#define GENETIC_MAP     2
#define RESTRICTION_MAP 3
#define CYTO_MAP        4
#define VIRTUAL_SEQ     5
#define SEG_SEQ         6
#define RAW_SEQ         7
NLM_EXTERN Uint1 get_Bioseq_type PROTO((BioseqPtr bsp));




/*************************************************************************
*
*	MapLayout is an ORDERED list of MapPos, which contains the layout
*	for each map. It is used for determine the layout of map alignment
*
**************************************************************************/
typedef ValNode MapLayout, FAR *MapLayoutPtr;
typedef struct mappos{          /*holding the positin of each map*/
	SeqLocPtr slp;		/*the range of the map on display*/
	Int2 entityID;
	Int4 left, right;	/*the left, right position on the graph*/
	Int4 top, bottom;	/*the top, bottom position on the graph*/
	Int4 seq_top, seq_bottom;	/*the top, bottom position of the sequence*/
}MapPos, PNTR MapPosPtr;
NLM_EXTERN MapLayoutPtr MapLayoutFree PROTO ((MapLayoutPtr head));




#define MAX_DIM         100     /*the maximal dimension of a map alignment*/
typedef struct alignpos{		/*the position of each alignment mapped to the graph*/
	Int4 left[MAX_DIM];
	Int4 right[MAX_DIM];
	Int4 top[MAX_DIM];
	Int4 bottom[MAX_DIM];
}AlignPos, PNTR AlignPosPtr;
/******************************************************************** 
* 
*       SortAlignPosition(app, dim) 
*       Sort out the order of a multiple alignment in the vertical  
*       display mode. It is sorted to the descending order of  
*       app->top. one app correspond to one aligned segment. It can 
*       be of multiple dimensions 
*       app: alignment position 
*       dim: dimention of alignment
*
*********************************************************************/ 
NLM_EXTERN void SortAlignPosition PROTO((AlignPosPtr app, Int2 dim));



/***************************************************************************
*
*	getBioseqNumbering(bsp)
*	get the numbering object from Seq_descr. If no numbering, return NULL
*
****************************************************************************/
NLM_EXTERN NumberingPtr getBioseqNumbering PROTO ((BioseqPtr bsp));	

/**********************************************************************
*
*   IS_NUM_GENE(gene_label): kludge function
*	determine if the gene_label is used as a map unit, such 
*	as the case for C.elegans physical map
*
**********************************************************************/
NLM_EXTERN Boolean IS_NUM_GENE PROTO ((CharPtr gene_label));

/*********************************************************************** 
* 
*       map_unit_label(): create a label for the map unit 
*	if(np == NULL), use the actual position. use_kb =TRUE converts 
*	to kilobase unit
* 
***********************************************************************/ 
NLM_EXTERN Boolean map_unit_label PROTO((Int4 pos, NumberingPtr np, CharPtr label, Boolean use_kb));


#define HUMAN_CYTO 1
#define FLY_CYTO 2
/****************************************************************
*
*	ck_cyto_type() a kludge to determine if the cytogenetic map
*	follows the band pattern of Fly or that of Human
*
****************************************************************/
NLM_EXTERN Uint1 ck_cyto_type PROTO((SeqFeatPtr sfp));


/*band used for the Flybase*/
#define MISC_BND	0	/*band is unknown*/

#define BND 1           /*band*/
#define HET 2 		/*heterochromatin*/
#define TEL 3 		/*telemere*/
#define CEN 4 		/*centromere*/

/*band used for Human chromosome*/
#define BAND_POINT 5		/*inclue the centromere and telemere*/	
#define GIEMSA_POS 6	/*Giemsa positive*/
#define GIEMSA_NEG 7	/*Giemsa negative*/
#define ACRO_CENTRIC 8	/*Acrocentric*/
#define VARIABLE_REG 9	/*VariableReg*/
/**********************************************************************
*
*	get_band_type(UserObjectPtr)
*	get the band type in a Cytogenetic map
*
***********************************************************************/
NLM_EXTERN Uint1 get_band_type PROTO((UserObjectPtr uop));



/*********************************************************************
* 
*       get_band_name(uop) 
*       parse the band name from a cytogenetic map 
* 
*********************************************************************/
NLM_EXTERN CharPtr get_band_name PROTO((UserObjectPtr uop));


/*************************************************************************
*
*       is_label_match(obj_id, label)
*       return TRUE if obj_id->str matches with label
*
**************************************************************************/
NLM_EXTERN Boolean is_label_match PROTO((ObjectIdPtr obj_id, CharPtr label));




/***********************************************************************
*
*	 a set of default parameters used in drawing and layout of the 
*	sequence picture
*
***********************************************************************/
#define LABEL_SPACE     12	/*for layout the mapdata*/
#define TICK_LEN        8       /*length of a tick mark*/

#define BAND_WIDTH      6       /*the width of the band of cytogenetic map*/

#ifdef WIN_MSWIN
#define ENZ_WIDTH       14      /*the width of the enzyme for Windows*/
#else
#define ENZ_WIDTH       10
#endif

#define MAP_SPACE       30      /*the space separate the maps*/
#define LINE_SPACE      4       /*the space among clones if drawn as line*/

#define SIMPLE_LEN      100000  /*the min length for simple drawing*/
#ifdef WIN_MSWIN
#define SEQ_WIDTH       14      /*the width of the enzyme for Windows*/
#else
#define SEQ_WIDTH       10
#endif
 
#define FEAT_WIDTH      5

NLM_EXTERN ValNodePtr free_slp_list PROTO((ValNodePtr slp_list));

NLM_EXTERN ValNodePtr get_equiv_align PROTO((SeqEntryPtr sep));

/*check to see if the alignment is designed to be displayed as the history*/
NLM_EXTERN Boolean is_annot_for_hist_alignment PROTO((SeqAnnotPtr annot));



/************************************************************************ 
* 
*       is_map_segment(slp) 
*       return TRUE if slp is a Seq-loc from a amp 
*       return FALSE if it is not a map or the Bioseq is not loaded to  
*       the memory yet 
*
************************************************************************/ 
NLM_EXTERN Boolean is_map_segment PROTO((SeqLocPtr slp));

/***************************************************************
*
*       figure_map_seqid(ext_loc)
*       a very unreliable way to figure out the if there is a
*       Seq-id for the map. It is dependent on the frequency of
*       the Seq-id in a  segmented sequence
*
****************************************************************/
NLM_EXTERN SeqIdPtr figure_map_seqid PROTO((SeqLocPtr ext_loc));

NLM_EXTERN Boolean make_Bioseq_list PROTO((SeqEntryPtr sep, ValNodePtr PNTR bsp_list, ValNodePtr PNTR equiv_align));

NLM_EXTERN Boolean start_new_stack PROTO((Int4 pre_pos, Int4 pos, Int4 scale, Int2Ptr label_width, Int2 c_width));

NLM_EXTERN Int4 calculate_ruler PROTO((Int4 scaleX));

#define GENE_MARK	0
#define CDS_MARK	1

/***********************************************************************
*
*	for each sequence in alignment stored in Seq-hist, if the aligned 
*	sequence itself contains alignment, it is temporarily loaded as 
*	a user-object in the descriptor of the bioseq. This function extract 
*	the information from the descripor and store it as a list of gi's 
*	plus the kludge offset value
*
************************************************************************/
NLM_EXTERN ValNodePtr get_seqids_with_alignment PROTO((BioseqPtr mbsp));

/***********************************************************************
*
*	map the kludge offet factor for Unigene, RICE, MOUSE ,FlyBase, etc
*
************************************************************************/
NLM_EXTERN Int4 get_kludge_factor PROTO((SeqIdPtr sip, Int4Ptr gi));

NLM_EXTERN Boolean BioseqHasLandMark PROTO((BioseqPtr bsp));


typedef struct repeat_region{
	GatherRange gr;
	Char rep_name[20];
}RepeatRegion, PNTR RepeatRegionPtr;

typedef struct align_region{
	Char annotDB[21];
	Uint1 displayOrder;
	GatherRange gr;	/*extremes for the intervals*/
	ValNodePtr intervals;	/*the intervals. more than one if there are multiple alignments for the same sequence*/
	Char seq_name[20];
	FloatHi score;
	FloatHi p_val;
	FloatHi e_val;
	Int4 line;
	Int4 g_left, g_right, g_top, g_bottom;	/*the rectangle that defines the alignment*/
	Uint1 status;	/*range from 0-4 for the lowest to the highest score*/
}AlignRegion, PNTR AlignRegionPtr;

/*trying to parse the descriptor in Seq-annot for the alignment display*/
#define ANNOT_BLAST	1 /*it is annotation on the Blast search*/
#define ANNOT_CONSIST	2 /*annotates the consistency between a clone and 
			  the genome map. For Eric Green's map*/
#define ANNOT_FISH	3 /*it is the annotation on a FISH map */
NLM_EXTERN Uint1 get_align_annot_qual PROTO((SeqAnnotPtr annot, CharPtr annotDB, Int4 buf_size, Uint1Ptr annot_type));

/**************************************************************************
*
*	collect_repeats_and_align(slp, rrp_list, arp_list, seglevels, sep)
*
*	collect repeat features and alignment for global display
*	rrp_list: the list of the repeat features
*	arp_list: the list of the alignments
*
***************************************************************************/				
NLM_EXTERN Boolean collect_repeats_and_align PROTO((SeqLocPtr slp, ValNodePtr PNTR rrp_list, ValNodePtr PNTR arp_list, Int2 seglevels, SeqEntryPtr sep, Uint1Ptr align_has_status));


/**************************************************************
*
*       get the alignment for the FISH map
*       for the Human Cytogenetic map, if there is a
*       Seq-annot stored as Hist-align and the intervals are
*       aligned to the FISH map, it will return the Seq-align
*       of the alignment to the FISH map
*
***************************************************************/
NLM_EXTERN SeqAlignPtr get_FISH_align PROTO((BioseqPtr bsp));

/*******************************************************
*
*	annot_is_user_defined(annot)
*
*	determine if the Seq-annot contains the features 
*	that were defined by the user. This is to 
*	distinguish the local data from the public data 
*	set
*
********************************************************/
NLM_EXTERN Boolean annot_is_user_defined PROTO((SeqAnnotPtr annot));


/*************************************************************
*
*	determine if the Bioseq in the SeqEntry contains 
*	the UserObject for a Map Legend
*
**************************************************************/
NLM_EXTERN UserObjectPtr BioseqHasMapLegend PROTO((BioseqPtr bsp));
NLM_EXTERN Boolean SeqLocListHasLegend PROTO((ValNodePtr slp_list));

NLM_EXTERN Boolean IsSeqIndexMap PROTO((BioseqPtr bsp));

#define SEQINDEX_VAL    255     /*used to store as the choice in slp_list*/


/*functions related to the yac2band and viewctg*/

/*the YAC contig maps are either from Whitehead or from Eric Green's group*/
#define YAC_MIT	1
#define YAC_NHGRI	2
#define GENETIC_GENETHON	4
#define GENETIC_CHLC	8
/*find a list of the seqlocs on the contig that maps to the
*current chromosome. For now, only the Whitehead map and the
*Eric Green's map is considered. return a list of Seq-locs that
*contains contigs within the region
*/
NLM_EXTERN Uint1 FindContigDB PROTO((SeqIdPtr sip));

/*return a list of Seq-locs which are the contigs mapped to 
*	the current location on the genome
*
*/
NLM_EXTERN ValNodePtr FindContigList PROTO((SeqLocPtr chr_slp));


NLM_EXTERN Boolean is_lod_score_annot PROTO((SeqAnnotPtr annot));
NLM_EXTERN Int2 GetLODScoreNumber PROTO((BioseqPtr bsp));
NLM_EXTERN CharPtr GetAnnotTitle PROTO((SeqAnnotPtr annot));
NLM_EXTERN Uint1 GetLODScoreBitValue PROTO((SeqFeatPtr sfp));


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

