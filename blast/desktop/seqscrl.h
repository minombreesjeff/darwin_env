#ifndef _SEQSCRL_
#define _SEQSCRL_

#include <objseq.h>
#include <picture.h>
#include <viewer.h>
#include <seqgraph.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct seqscroll
{
  WindoW                w;
  BaR                   br;
  TexT                  pc;
  Int2                  curval, maxval, minval;
  BioseqPtr             bsp;
  GraphViewFormPtr      gvp;
  FloatHiPtr            scr;
  CharPtr               res;
  Uint1                 filtertype;
  struct seqscroll PNTR next;
} SeqScrollData, PNTR SeqScrollDataPtr;

extern SeqScrollDataPtr SeqScrollDataNew (Int4 score_number);
extern SeqScrollDataPtr SeqScrollDataFree (SeqScrollDataPtr ssdp);
extern void TextScrollWindowNew (SeqScrollDataPtr ssdp, GrouP g);
extern void AddScrollControl (SeqScrollDataPtr PNTR ssdp, GrouP g,
                              Int2 cur, Int2 min, Int2 max,
                              Int4 score_number);

#ifdef __cplusplus
}
#endif

#endif
