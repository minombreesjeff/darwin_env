/* ===========================================================================
*
*                            PUBLIC DOMAIN NOTICE
*            National Center for Biotechnology Information (NCBI)
*
*  This software/database is a "United States Government Work" under the
*  terms of the United States Copyright Act.  It was written as part of
*  the author's official duties as a United States Government employee and
*  thus cannot be copyrighted.  This software/database is freely available
*  to the public for use. The National Library of Medicine and the U.S.
*  Government do not place any restriction on its use or reproduction.
*  We would, however, appreciate having the NCBI and the author cited in
*  any work or product based on this material.
*
*  Although all reasonable efforts have been taken to ensure the accuracy
*  and reliability of the software and data, the NLM and the U.S.
*  Government do not and cannot warrant the performance or results that
*  may be obtained by using this software or data. The NLM and the U.S.
*  Government disclaim all warranties, express or implied, including
*  warranties of performance, merchantability or fitness for any particular
*  purpose.

* ===========================================================================
*
* File Name:  dotseq.h
*
* Author:  Fasika Aklilu
*
* Version Creation Date:   8/9/01
*
* $Revision: 6.4 $
*
* File Description: computes local alignments for dot matrix
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------

$Revision: 6.4 $
$Log: dotseq.h,v $
Revision 6.4  2001/08/09 16:33:18  aklilu
added revision

Revision 6.3  2000/07/26 18:23:10  sicotte
added DOT_SPI_FindBestAlnByDotPlotEx, to return rejected alignments


*/

#ifndef _DOTSEQ_
#define _DOTSEQ_

#ifdef __cplusplus
extern "C" {
#endif

  /****************************************************************************

      INCLUDE SECTION                                                                       
   ***************************************************************************/

#include <tofasta.h>
#include <seqport.h>
#include <sequtil.h>
#include <sqnutils.h>
#include <blastpri.h>
#include <explore.h>
#include <seqmgr.h>
#include <lookup.h>
#include <jsavlt.h>

 /****************************************************************************

      DEFINES SECTION                                                                 
 ***************************************************************************/


#define UNDEFINED  25     /* amino acids undefined in BLOSUM62 */
#define MAX_TRIM   200




 /****************************************************************************

      DATA STRUCTURE SECTION                                                               
 ***************************************************************************/


  typedef struct hs_diag {
    Int4 q_start; /* left most value on the graph */
    Int4 s_start; /* left most value on the graph */
    Int4 length;
    Int4 score;
    Int4 rdmKey;
  } DOTDiag, PNTR DOTDiagPtr;



  /* coordinates of old diags -used in history binary tree */

  typedef struct hist {
    Int4    diag_constant;
    Int4    q_stop;
  } DOTHist, PNTR DOTHistPtr;
  

  
  /* main struct */
  
  typedef struct mainseqinfo {
    Int4Ptr  PNTR matrix; /* dna matrix */
    Int2     maxscore; /* highest matrix score */
    Int2     minscore;/* lowest matrix score */
    Boolean     is_na; 
    Int4        qlen; /* length of query sequence */
    Int4        slen; /* length of subject sequence */
    Uint1       qstrand; /* strand of query */
    Uint1       sstrand;  /* strand of subject */
    BioseqPtr   qbsp; /* query bioseq */
    BioseqPtr   sbsp; /* subject bioseq */
    SeqLocPtr   qslp; /* query seqloc pointer */
    SeqLocPtr   sslp; /* subject seqloc pointer */
    Int4        q_start;  /* left position on query bioseq */
    Int4        q_stop;   /* right position on query bioseq */
    Int4        s_start;  /* left position on subject bioseq */
    Int4        s_stop;/* right position on subject bioseq */
    Uint1Ptr    qseq; /* query sequence buffer */
    Uint1Ptr    sseq; /* subject sequence buffer */
    CharPtr     qname; /* query accession */
    CharPtr     sname;/* subject accession */
    /* hash value */
    Int4        word_size; /* size of hash table index */
    /* binary tree data */
    Int4        cutoff_score; /* cutoff to store hits */
    Avl_TreePtr tree; /* binary tree for collecting hits */
    Boolean     first_pass;
    Int4Ptr     score_array;/* array by score for threshold ramp */
    Int4        unique; /* binary tree variable */
    Int4        tree_limit; /* upper limit for size of binary tree */
    /* sorted diag data */
    DOTDiagPtr  PNTR  hitlist; /* dotseq output data -- array of hits */
    Int4        index; /* total number of stored hits in array */
  } DOTMainData, PNTR DOTMainDataPtr;
  
  
  typedef struct dotdata {
    Uint1 xstrand;
    Uint1 ystrand;
    Int4  xstart;
    Int4  xstop;
    Int4  ystart;
    Int4  ystop;
    Int4  index;
    DOTDiagPtr    PNTR  hitlist;
  } DOTData, PNTR DOTDataPtr;
  
  /* information for history binary tree */
  
  typedef struct info{
    DOTMainDataPtr  mip;
    Uint1Ptr    qseq;
    Uint1Ptr    sseq;
    Int4        q_pos;
    Int4        s_pos;
    Int4        wordsize;    
    Avl_TreePtr tree;
    Boolean     first_pass;
  } DOTInfo, PNTR  DOTInfoPtr;
  


 /****************************************************************************

      FUNCTION DECLARATIONS                                                               
 ***************************************************************************/

  /* Function: Compute all matches between two sequences. Input: 2 bioseqptrs.
     Returns: DOTMainDataPtr with hitlist structure with start/stops in bioseq coordinates
   */
DOTMainDataPtr DOT_CreateAndStore (DOTMainDataPtr mip, BioseqPtr qbsp, BioseqPtr sbsp, Int4 q_start, Int4 q_stop, Int4 s_start, Int4 s_stop, Int4 word_size, Int4 tree_limit, Boolean initialize);
  /* Function: Compute all matches between two sequences. Input: 2 seqlocptrs (can specify plus or minus strand in slp).
     Returns: Filled DOTMainDataPtr with hitlist structure with start/stops in bioseq coordinates
  */
DOTMainDataPtr DOT_CreateAndStorebyLoc (SeqLocPtr slp1, SeqLocPtr slp2, Int4 word_size, Int4 tree_limit);
Int2 DOT_BuildHitList(DOTMainDataPtr mip, Boolean do_sort, Boolean do_countscore);
Boolean DOT_GetSeqs (DOTMainDataPtr mip, Boolean is_zoom);
Int2 DOT_FreeMainInfo(DOTMainDataPtr mip);
Int2 DOT_FreeMainInfoPtrEx (DOTMainDataPtr mip);
Int2 DOT_FreeHitsArray (DOTDiagPtr PNTR hitlist, Int4 index);
Boolean DOT_GetSeqs (DOTMainDataPtr mip, Boolean is_zoom);
extern DOTMainDataPtr DOT_InitMainInfo (DOTMainDataPtr mip, BioseqPtr qbsp, BioseqPtr sbsp, Int4 word_size, Int4 tree_limit, Int4 qstart, Int4 qstop, Int4 sstart, Int4 sstop);
SeqAlignPtr DOT_SPI_FindBestAlnByDotPlot(SeqLocPtr slp1, SeqLocPtr slp2, Int4 wordsize, Int4 num_hits);
extern Uint2 DOT_AttachSeqAnnotToSeqEntry (Uint2 entityID, SeqAnnotPtr sap, BioseqPtr bsp);
extern Int4Ptr PNTR DOT_DNAScoringMatrix(Int4 mismatch, Int4 reward,Int4 alsize);

#ifdef __cplusplus
}
#endif

#endif /* ndef _DOTSEQ_ */
