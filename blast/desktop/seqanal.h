#ifndef _SEQANAL_
#define _SEQANAL_

#include <picture.h>
#include <viewer.h>
#include <seqgraph.h>

#ifdef __cplusplus
extern "C" {
#endif

/* defines - button registration */

#define BCOUNT 16

/* structures - button registration */

typedef struct buttonregister
{
  ButtoN  PNTR b;
  GrouP   PNTR g;
  Int4    PNTR c;
} ButReg, PNTR ButRegPtr;

/* defines */
#define ANAL_AA 0
#define ANAL_NA 1

/* structures */

typedef struct seqanalform
{
  FORM_MESSAGE_BLOCK
  GraphViewFormPtr  gvp;
  Int2              anal;
  ButtoN            curbut;
  ButRegPtr         burp;
  BioseqPtr         bsp;
  Uint2             entityID, itemID, procID, userKEY;
} SeqAnalForm, PNTR SeqAnalFormPtr;

/* prototypes */

extern ButRegPtr ButRegNew (void);
extern ButRegPtr ButRegFree (ButRegPtr burp);
extern void ButRegAdd (ButRegPtr burp, ButtoN b, GrouP g, Int4 current);
extern void ButRegSelectProc (ButtoN b);

extern SeqAnalFormPtr SeqAnalFormNew (void);
extern SeqAnalFormPtr SeqAnalFormFree (SeqAnalFormPtr gvp);
extern void BioseqPtrToSeqAnalForm (ForM f, Pointer data);
extern ForM CreateSeqAnalForm (Int2 left, Int2 top, CharPtr title,
                               BioseqPtr bsp);

#ifdef __cplusplus
}
#endif

#endif
