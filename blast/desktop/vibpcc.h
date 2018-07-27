#ifndef _VIBPCC__
#define _VIBPCC__

#include <ncbi.h>
#include <objres.h>
#include <picture.h>
#include <viewer.h>

#ifdef __cplusplus
extern "C" {
#endif

extern SeqGraphPtr PccGraph (Int4 start, Int4 end, FloatHiPtr score);

#ifdef __cplusplus
}
#endif

#endif
