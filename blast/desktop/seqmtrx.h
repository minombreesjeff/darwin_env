#ifndef _SEQMTRX_

#define _SEQMTRX_

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

#define NA_MATRIX           0
#define NA_MATRIX_DONOR1    1
#define NA_MATRIX_DONOR2    2
#define NA_MATRIX_ACCEPT    3
#define NA_MATRIX_BRANCH    4
#define NA_MATRIX_HR        5
#define NA_MATRIX_USERDATA  6

#define AA_MATRIX           100

/* structures */

typedef struct compilematrix
{
  Int4    min;
  Int4    max;
  Int4    res[32];
  struct  compilematrix PNTR next;
} ComMat, PNTR ComMatPtr;

/* prototypes */

extern ComMatPtr ComMatFree (ComMatPtr headcmtp);
extern ComMatPtr CompileMatrix (FloatHi scr[32][32], Int4 len, Int4 maxval);
extern void InvertMatrix (ComMatPtr cmtp, Int4 window);
extern Int4 ReadMatrix (CharPtr res, FloatHi scr[32][32],
                        CharPtr filename);

extern Int4 GridMatchSetUp (CharPtr seq);
extern Int4 GridMatch (CharPtr seqhead, Int4 cur, Int4 end, Int4 matlen,
                       ComMatPtr cmtphead, Int4Ptr matval, Int4 maxval);

extern SeqGraphPtr MatrixSeq (BioseqPtr bsp, SeqGraphPtr sgptr,
                              ComMatPtr cmtp, Int4 window);

Int2 LIBCALLBACK SeqMatFunc (Pointer data);

#define REGISTER_GROUP_MATRIX ObjMgrProcLoadEx (OMPROC_FILTER, \
        "BacEnhancer", "Enhancer Scan", \
        OBJ_BIOSEQ, 0, OBJ_BIOSEQ, 0, \
        NULL, SeqMatFunc, PROC_PRIORITY_DEFAULT, "Analysis")

#ifdef __cplusplus
}
#endif

#endif
