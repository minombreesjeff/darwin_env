#ifndef _SEQGRAPH_
#define _SEQGRAPH_

#include <picture.h>
#include <viewer.h>

#ifdef __cplusplus
extern "C" {
#endif

/* defines */
#define GRAPH_FILTER     0
#define GRAPH_MATRIX_AA  1
#define GRAPH_MATRIX_NA  2
/*
#define GRAPH_PATTERN_AA 3
#define GRAPH_PATTERN_NA 4
*/
/* structures */

typedef struct graphviewform
{
  FORM_MESSAGE_BLOCK
  SeqGraphPtr    sgp;
  Int4           height, width, window;
  Uint1          type;
  Uint1          graphtype;
  BioseqPtr      bsp;
  SegmenT        seg;
  VieweR         viewer;
  BaR            br;
  TexT           pc;
  DialoG         cutoffs;
  Boolean        flagNewClick;
  Uint2          HLRange, primIDMax;
  Int2           level;
  PopuP          scale;
  Int2           zoom;
  Int4           margin, start, stop;
  SeqLocPtr      seqloc, slp;
  Uint2          entityID, itemID, procID, userKEY;
} GraphViewForm, PNTR GraphViewFormPtr;

/* prototypes */

extern GraphViewFormPtr GraphViewFormNew (void);
extern GraphViewFormPtr GraphViewFormFree (GraphViewFormPtr gvp,
                                           Boolean flagFree);
extern ForM CreateGraphViewForm (Int2 left, Int2 top, CharPtr title,
                                 BioseqPtr bsp, Uint1 graphtype);
extern void BioseqPtrToGraphViewForm (ForM f, Pointer data);

#ifdef __cplusplus
}
#endif

#endif
