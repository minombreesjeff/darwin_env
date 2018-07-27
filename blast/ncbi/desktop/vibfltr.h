#ifndef _VIBFLTR__
#define _VIBFLTR__

#include <ncbi.h>
#include <objres.h>
#include <picture.h>
#include <viewer.h>

#ifdef __cplusplus
extern "C" {
#endif

extern SeqGraphPtr FilterGraph (Int4 start, Int4 end, FloatHiPtr score,
                                FloatHi cutoff);

#ifdef __cplusplus
}
#endif

#endif
