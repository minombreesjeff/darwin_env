#ifndef _SEQPCC_

#define _SEQPCC_

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

#define AA_PCC    0

/* prototypes */

extern SeqGraphPtr PCCProc (BioseqPtr bsp, SeqFeatPtr sfp, Int4 window);

#ifdef __cplusplus
}
#endif

#endif
