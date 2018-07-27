#ifndef _MUSK_PRT_GENE_
#define _MUSK_PRT_GENE_
#include <tofile.h>
#include <maputil.h>
#include <jzcoll.h>
#include <gather.h>




#define MATCH_NONE  0
#define MATCH_QUERY	1
#define MATCH_ALIGN 2

/* choice for printing the location of a feature */
#define PRINT_LOC_NONE	0	/* do NOT print out the location */
#define PRINT_LOC_BOGO	1	/* print out the bogo base location */
#define PRINT_LOC_NUM	2	/*convert the location to the original numbering 
								system, such as the centiRay and centiMorgam */
/******************************************************************
*
*	print the everything in the interval slp to a text file
*	slp: the interval
*	g_data: the query gene
*	is_html: make it a html page
*	line_len; the length of the file
*	fp: the output file
*
********************************************************************/	
Boolean print_genome_interval PROTO((SeqLocPtr slp, GeneDataPtr g_data, Boolean is_html, Int2 line_len, FILE *fp));

CharPtr WWW_PrintGeneRef  PROTO((SeqFeatPtr sfp, Uint1 match_type, Uint1 print_loc, NumberingPtr np, Boolean is_html));

#endif
