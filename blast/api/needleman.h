/*
  Compute the a global alignment between two SeqLocs using
  the Needleman-Wunsch algorithm.
  This algorithms requires temporary storage of order
  N*M/4 bytes for the traceback. Do not use it for large
  SeqLocs!!
  
  The alignment is assumed to exist between the orientation specified
  in the slp.. If one of the sequences need to be reversed, then the
  slp should be on the minus strand.
  
  In finding the optimal alignments end-gaps are scored.. but
  they are not counted in the returned score.
  
  The alignment returned will start and end at the endpoints of the
  SeqLocs, but may include terminal gaps in order to do so.
  
  One way to obtain the correct Karlin Altschul parameters is to
  call the function in blastkar.c, given a valid matric name.
  Int2 LIBCALL BlastKarlinGetMatrixValues(CharPtr matrix, Int4Ptr open, Int4Ptr extension, FloatHiPtr lambda, FloatHiPtr K, FloatHiPtr H)
  
  if Kappa || Lambda ==0, defaults are provided.. appropriate for nucleotides.

$Revision: 6.7 $
$Log: needleman.h,v $
Revision 6.7  2000/10/31 21:20:05  vakatov
[WIN32] DLL'zation

Revision 6.6  2000/08/28 19:36:52  kans
moved revision log into comment section

Revision 6.5  2000/08/28 17:59:44  sicotte
added revision log
*/


#ifndef _NEEDLEMAN_
#define _NEEDLEMAN_

#include <ncbi.h>
#include <objseq.h>
#include <objsset.h>

#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif

#ifdef __cplusplus
extern "C" {
#endif


/* BLAST_Score must be a signed datatype 
typedef Int4    BLAST_Score, PNTR BLAST_ScorePtr;
*/
NLM_EXTERN SeqAlignPtr LIBCALL NeedlemanWunschQuadraticByLoc(SeqLocPtr slp1,SeqLocPtr slp2, Int4 **matrix, Int4 gapOpen, Int4 gapExtend,  Int4 *score, Nlm_FloatHi Kappa, Nlm_FloatHi Lambda);
Uint1Ptr FetchSequenceByLoc(SeqLocPtr slp,Int4* length,Int4* BspLength);


#ifdef __cplusplus
}
#endif

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

#endif
