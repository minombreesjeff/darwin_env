/** salign.h 
***
***  Header file containing external functions of salign.c
***
************/

#ifndef _SALIGN_
#define _SALIGN_

#include <ncbi.h>
#include <objseq.h>
#include <objsset.h>
#include <objalign.h>

/********************************************************
*** BandAlign Data Structure 
*********************************************************/
typedef struct mash {
   Uint1 band_method;
   Int2 reward;
   Int2 penalty;
   Int4 gap_open;
   Int4 gap_extend;
   Int4 wordsize;
   Int4 filter;
   Int4 gap_x_dropoff;
   Int4 gap_x_dropoff_final;
   Boolean is_prot;
   Boolean translate_prot;
   SeqAlignPtr transalp;
   Boolean use_gapped_blast;
   CharPtr matrixname;
   Int4 lg1_ext;
   Int4 rg1_ext;
   Int4 lg2_ext;
   Int4 rg2_ext;
   Int4 lg1_open; 
   Int4 lg2_open; 
   Int4 rg1_open; 
   Int4 rg2_open;
   Int4    blast_threshold;
   Uint1   choice_blastfilter;
   Boolean splicing;
   Boolean multidim;
   Boolean map_align;
   Boolean align_ends;
} Mash, PNTR MashPtr;

NLM_EXTERN MashPtr MashNew (Boolean is_prot);

NLM_EXTERN SeqAlignPtr SeqLocListToSeqAlign (ValNodePtr sqloc_list, Int2 choice, Pointer param);

NLM_EXTERN SeqLocPtr AlignmRNA2genomic (BioseqPtr bsp1, BioseqPtr bsp2);

NLM_EXTERN SeqAnnotPtr BlastBandAlignFromBlastSeqAlign (SeqAlignPtr salpblast, Boolean align_ends);

NLM_EXTERN SeqAnnotPtr LIBCALL SeqEntryToSeqAlign (SeqEntryPtr sep, Uint1 bsp_mol);

#endif
