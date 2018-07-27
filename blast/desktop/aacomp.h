#ifndef _SEQAAC_

#define _SEQAAC_

#ifndef _NCBI_Seqres_
#include <objres.h>
#endif

#ifndef _NCBI_Seq_
#include <objseq.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* prototypes */

extern Int4 ReadAAC (CharPtr filename, FloatHiPtr scr, CharPtr res);

extern FloatHiPtr AAComposition (SeqPortPtr spp,
                                 Int4 start, Int4 end, Int4 numval,
                                 FloatHiPtr scr, CharPtr res, Int4 window);

#ifdef __cplusplus
}
#endif

#endif
