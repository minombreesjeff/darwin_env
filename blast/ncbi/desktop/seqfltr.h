#ifndef _SEQFLTR_

#define _SEQFLTR_

#ifndef _NCBI_Seqres_
#include <objres.h>
#endif

#ifndef _NCBI_Seq_
#include <objseq.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* defines */

#define NA_FILTER_GC     0
#define NA_FILTER_AT     1
#define NA_FILTER_PUR    2
#define NA_FILTER_PYR    3
#define NA_FILTER_DUST   4

#define AA_FILTER_COMP        100
#define AA_FILTER_COMP_HOPP   101
#define AA_FILTER_COMP_KYTE   102

/* prototypes */

extern SeqGraphPtr FilterSeq (SeqPortPtr spp, Int4 start, Int4 end,
                              FloatHiPtr scr, CharPtr res, Int4Ptr exval,
                              Uint1 filtertype);

#ifdef __cplusplus
}
#endif

#endif
