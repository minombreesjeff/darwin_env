/****************************************************************************
* sim_aln.c modified from the sim_aln_old.c. Input to CSIM will be Bioseq 
* object instead of two sequence strings. CSIM is also modified
*
*
*			Jinghui Zhang		2/27/94
*****************************************************************************/
#include <ncbi.h>
#include <objseq.h>
#include <objloc.h>
#include <sequtil.h>
#include <seqport.h>
#include <objalign.h>


typedef struct simdata{
	Int4Ptr rec_a1;
	Int4Ptr rec_a2;
	Uint1 strand;
	Int2    num;
	FloatHi score;
	struct simdata PNTR next;
}SimData, PNTR SimDataPtr;
