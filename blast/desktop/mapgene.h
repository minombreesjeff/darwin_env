#ifndef _MAPGENE_
#define _MAPGENE_


#include <glbpic.h>

/************************************************************
*
*	map the one or two genes to a list Seq-locs
*	the slp_list will be stored at gdraw_p->slp_list
*	gdraw_p: the Drawing structure
*	g_list: a list of genes
*	pgdata: store the GeneDataPtr created
*	fp:	if not NULL, dump out the text between a region
*	is_html: if TRUE, dump out a html page for the fp
*
************************************************************/
Boolean map_gene_location PROTO((GlobalDrawPtr gdraw_p, ValNodePtr g_list, GeneDataPtr PNTR pgdata, FILE *fp, Boolean is_html));


/*********************************************************
*
*       map the location of the current slp to the
*       list of slps based on consistent alignment, FISH
*       etc
*
*********************************************************/
ValNodePtr map_one_location_to_slp_list PROTO((SeqEntryPtr sep, SeqLocPtr slp, Int4 window_size));


/*map a cytogenetic band to a location on the physical map
 *return TRUE for success and FALSE for failure. If the results are 
 *success, gdraw_p->slp will be replaced with the newly mapped slp_list*/
Boolean MapCytoBand PROTO((CharPtr band_name, GlobalDrawPtr gdraw_p, BioseqPtr cyto_bsp, Boolean correct_distortion, Uint1Ptr end_band));


/*
*
*       slp is the Seq-loc for a contig on the map. If include_amb,
*       all the ambiguously mapped YAC on the contig will be
*       included. Otherwise, they will be filterd from the list
*       of the alignnode. db can be either YAC_MIT or YAC_NHGRI
*
*/
ValNodePtr LoadContigAlign PROTO((SeqLocPtr slp, Boolean include_amb, Uint1 db, SeqIdPtr chr_id));

/*
*
*       print out the alignnode for YACs as a tabular form
*       contig name is the name of the contig where the YAC is
*       mapped. band_name is the name of the chromosome band
*       that it mapped to
*
*/
Boolean PrintContigAlign PROTO((CharPtr band_name, CharPtr contig_name, ValNodePtr anp_list, FILE *fp));


/*
*
*       Print out the H1 header for each band. this includes the 
*       link to OMIM and the large sequencing centers
*
*/
void PrintBandHeader PROTO((FILE *fp, CharPtr band_name, Uint1 db_option, Boolean graphic, CharPtr tx_buf));

void print_contig_html_color PROTO((FILE *fp, CharPtr band_name));
 
/*
*
*	print out the html page that displays the YACs for the selected 
*	contigs
*	chr_id: id of the mapping center
*	contig_list: the list of the Seq-loc for contigs
*	band_name: the name of the chromosome band
*	first: if TRUE, print out the HTML header, else don't
*	if(include_amb): including the ambiguous YACs
*
*/ 	
Boolean PrintContigForOneMap PROTO((SeqIdPtr chr_id, ValNodePtr contig_list, 
		CharPtr band_name, Boolean include_amb, BoolPtr pfirst, FILE *fp));


/*
*
*       print the genethon marker to the HTML page
*	if(end_band == 0), it is an internal band
*	if(end_band == 1), it is the first band to the ptel
*	if(end_band == 2), it is the last band before qtel
*
*/
Boolean PrintGeneticMarkerForOneMap PROTO((SeqLocPtr chr_slp, Boolean first,
                CharPtr band_name, Uint1 end_band, Uint1 chr, FILE *fp));

void print_database_link PROTO((FILE *fp, Uint1 db_option));

/*
*
*       make the links to the transcript map
*
*/
Boolean MakeTxLink PROTO((SeqLocPtr chr_slp, CharPtr buf, Uint1 chr, Uint1 end_band));


#endif

