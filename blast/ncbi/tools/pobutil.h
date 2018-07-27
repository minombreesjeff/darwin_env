

#ifndef _POBUTIL_
#define _POBUTIL_


#include <seqport.h>
#include <simutil.h>
#include <jzmisc.h>
#include <tofasta.h>
#include <dust.h>


#define LINE 40

#define REPEAT_BREAK_LEN	10000
#define REPEAT_OVERLAP_LEN	100


#define RUN_SIM_1	1
#define RUN_SIM_2	2
#define RUN_SIM_3	3
#define RUN_BANDALGN  8


#define FILE_IO		0
#define STD_IO	1



/****************************************************************
*
*	SortDenseDiagByLocation()
*	Sort the DenseDiag from Blastn and Blastp to ascending order 
*	of the locations
*
*****************************************************************/
void SortAlignByLocation PROTO((SeqAnnotPtr blast_sap));


SeqAnnotPtr  sim_for_blast PROTO((SeqAnnotPtr blast_sap, SeqIdPtr query_id, Int4 which_sim));

/*****************************************************************
*
*	clean_empty_seqalign(): clean up Seq-align that has no 
*	data
*
*****************************************************************/
SeqAlignPtr clean_empty_seqalign PROTO((SeqAlignPtr PNTR head));


/**********************************************************************
*
*	functions related to the filtering of the repeat sequence 
*	such as ALUs in primates
*
***********************************************************************/
ValNodePtr make_repeat_lib PROTO((CharPtr file_name, ValNodePtr PNTR slp_list, Boolean io_type));

void free_alu_list PROTO((ValNodePtr alu_sep_list, ValNodePtr alu_slp_list));
/*****************************************************
*
*       filter_repeats_for_bigloc(slp, max_len, min_overlap)
*       break the slp into smaller pieces and merge the results
*
*       slp: the big Seq-loc
*       max_len: the maximum allowed length for search
*       if -1, set to default
*       min_overlap: the minimum length for search
*       if -1, set to the default
*
*******************************************************/
ValNodePtr filter_repeats_for_bigloc PROTO((SeqLocPtr slp, Int4 max_len, Int4 min_overlap, ValNodePtr alu_slp_list));

SeqLocPtr filter_repeats PROTO((SeqLocPtr slp, ValNodePtr alu_slp_list, ValNodePtr PNTR ends_list));

void SaveRepeats PROTO((BioseqPtr bsp, ValNodePtr ends_list));

void load_dust_bsp PROTO((BioseqPtr dust_bsp, SeqLocPtr dustloc));

/*******************************************************************
* 
*       look for the repeat feature in sep and mask the residue in  
*       dust_bsp 
* 
*******************************************************************/ 
void mask_with_repeat_feature PROTO((BioseqPtr bsp, SeqEntryPtr sep, BioseqPtr dust_bsp));


/*******************************************************************
*
*	misc functions
*
*******************************************************************/
void get_output_name PROTO((CharPtr name, CharPtr buf));
void write_asn_output PROTO((CharPtr name, BioseqPtr bsp, SeqAnnotPtr annot, Int4 option, Boolean io_type));
Boolean save_SeqAnnot PROTO((SeqAnnotPtr annot, CharPtr name, Boolean io_type));
void prt_FASTA_file PROTO((BioseqPtr bsp, SeqLocPtr dustloc, Boolean io_type));



/**************************************************************
*
*	functions related to the pre and post process of the 
*	blast results
*
***************************************************************/
void restore_blast_id PROTO((SeqAnnotPtr sap, SeqIdPtr sip));
void link_blast_sap PROTO((SeqAnnotPtr PNTR blast_sap, SeqAnnotPtr new));
SeqLocPtr break_blast_job PROTO((SeqLocPtr slp, SeqIdPtr sip, Int4 max_len, Int4 overlap_len));
void clean_all_internal_repeats PROTO((SeqAlignPtr PNTR head));

/*******************************************************************
*
*       merge_blast_result(head, new)
*       when the input query is too big, it is broken into several
*       overlapping Seq-loc to be sent to blast. The results can be
*       merged if two Dense-diags are actually represent one HSP
*
********************************************************************/
SeqAnnotPtr merge_blast_result PROTO((SeqAnnotPtr PNTR head, SeqAnnotPtr new));



/***************************************************************
*
*	functions related to the filtering of the organism
*
***************************************************************/
/********************************************************************
*
*       get_organism(bsp)
*       filter the taxname from the organism field in Bioseq
*
********************************************************************/
CharPtr get_organism PROTO((BioseqPtr bsp));


/****************************************************************
*
*       sim2_for_blast(blast_sap, query_id)
*       run sim2 with the Seq-annot from blast
*
****************************************************************/
SeqAnnotPtr  sim2_for_blast PROTO((SeqAnnotPtr blast_sap, SeqIdPtr query_id));


void filter_blast_query PROTO((SeqAnnotPtr blast_sap, BioseqPtr query));


/******************************************************************
*
*	functions related to dust
*
******************************************************************/
BioseqPtr make_dust_bsp PROTO((BioseqPtr bsp, Int4 start, Int4 stop, SeqLocPtr dustloc));

/***********************************************************
*
*  Function for finding human repeat features
*
************************************************************/

Boolean LIBCALL FindHumanRepeats (BioseqPtr bsp, Boolean show_prog_mon);

/*****************************************************************
*
*   AluFeat
*     DeskTop function for creating human repeat features
*
*****************************************************************/
Int2 LIBCALLBACK AluFeat (Pointer data);
#define REGISTER_ALUFEAT ObjMgrProcLoadEx(OMPROC_FILTER,"Find Human Repeats","AluFeat",OBJ_BIOSEQ,2,OBJ_BIOSEQ,2,NULL,AluFeat,PROC_PRIORITY_DEFAULT,"Analysis")


/*****************************************************************
*
*	convert alignment to a faked feature
*
*****************************************************************/
Boolean load_align_to_interval PROTO((SeqAnnotPtr align_annot, Int4 min_gap_len, BioseqPtr bsp));


Int4 get_score_value PROTO((ScorePtr sp));

#endif
