/* $Id: cddutil.h,v 1.25 2001/11/13 19:51:52 bauer Exp $
*===========================================================================
*
*                            PUBLIC DOMAIN NOTICE
*               National Center for Biotechnology Information
*
*  This software/database is a "United States Government Work" under the
*  terms of the United States Copyright Act.  It was written as part of
*  the author's official duties as a United States Government employee and
*  thus cannot be copyrighted.  This software/database is freely available
*  to the public for use. The National Library of Medicine and the U.S.
*  Government have not placed any restriction on its use or reproduction.
*
*  Although all reasonable efforts have been taken to ensure the accuracy
*  and reliability of the software and data, the NLM and the U.S.
*  Government do not and cannot warrant the performance or results that
*  may be obtained by using this software or data. The NLM and the U.S.
*  Government disclaim all warranties, express or implied, including
*  warranties of performance, merchantability or fitness for any particular
*  purpose.
*
*  Please cite the author in any work or product based on this material.
*
* ===========================================================================
*
* File Name:  cddutil.h
*
* Author:  Aron Marchler-Bauer
*
* Initial Version Creation Date: 12/15/1999
*
* $Revision: 1.25 $
*
* File Description: Header file for cdd api utility functions  
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: cddutil.h,v $
* Revision 1.25  2001/11/13 19:51:52  bauer
* support for annotation transfer in alignment reindexing
*
* Revision 1.24  2001/05/23 21:18:06  bauer
* added functions for alignment block structure control
*
* Revision 1.23  2001/04/10 20:25:57  bauer
* cddutil.c
*
* Revision 1.22  2001/04/10 20:18:09  bauer
* write out ascii-formatted mtx-Files for copymat
*
* Revision 1.21  2001/03/07 16:30:33  bauer
* fixed alignment reindexing bug
*
* Revision 1.20  2001/02/14 17:11:03  bauer
* relaced calls to BioseqCopy with CddBioseqCopy
*
* Revision 1.19  2001/02/13 20:55:10  bauer
* fixed bug when comparing local ids
*
* Revision 1.18  2001/02/06 22:55:35  bauer
* Scoring Matrix now a function parameter in CddDenDiagCposComp2
*
* Revision 1.17  2001/02/05 22:58:47  bauer
* added alignment reindexing function
*
* Revision 1.16  2001/01/17 21:32:02  bauer
* changes to PSSM calculation
*
* Revision 1.15  2001/01/12 01:31:30  bauer
* added data structures for alignment reindexing
*
* Revision 1.14  2001/01/12 00:17:01  bauer
* added routines for information content calculation
*
* Revision 1.13  2001/01/11 23:48:25  bauer
* added check for consensus-Cd
*
* Revision 1.12  2000/12/01 19:36:57  hurwitz
* added scale factor to PSSM calcs
*
* Revision 1.11  2000/11/14 22:08:44  hurwitz
* added weighting for pssm calculation
*
* Revision 1.10  2000/09/08 21:43:51  hurwitz
* adding PSSM calculation to DDE
*
* Revision 1.9  2000/09/01 21:59:12  hurwitz
* re-order columns from PSSM of CDs to column order expected in threading
*
* Revision 1.8  2000/08/30 21:33:55  hurwitz
* added new and free functions for Seq_Mtf and Qry_Seq
*
* Revision 1.7  2000/08/17 19:00:24  hurwitz
* fixed some codewarrior compiler warnings
*
* Revision 1.6  2000/08/14 21:52:05  hurwitz
* added CddCalcPSSM
*
* Revision 1.5  2000/08/14 19:36:26  hurwitz
* got CddCposComp working and tested
*
* Revision 1.4  2000/08/11 19:54:01  hurwitz
* restored CddDenDiagCposComputation and CddCposComputation to original, added CddCposComp which combines the 2
*
* Revision 1.3  2000/08/09 21:29:08  hurwitz
* adding cddutil.c to VC++ build
*
* Revision 1.2  2000/07/19 19:42:08  bauer
* added modification logging
*
*
* ==========================================================================
*/

#if !defined(CDDUTIL_H)
#define CDDUTIL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <objcdd.h>
#include <blastdef.h>
#include <thrdatd.h>
#include <posit.h>

#define CKPTEXT ".chk"
#define CSEQEXT ".csq"
#define MTRXEXT ".mtx"

#define POSFREQ_SCALE 1000000

#define Xscore (-1)

/* column order of residue-types after PSSM is calculated from CD's */
#define InputOrder "-ABCDEFGHIKLMNPQRSTVWXYZU*"

/* column order of residue-types needed for threading */
/* this should match the column order in the contact potential */
#define OutputOrder "ARNDCQEGHILKMFPSTWYV"

/*---------------------------------------------------------------------------*/
/* Structures used in Calculation of a 50/50 Extent/Content consensus seq.   */
/*---------------------------------------------------------------------------*/
typedef struct _cdd_ext_alignment_cell {
  Uint1     aatype;
  Int4      seqpos;
} CddExtAlignCell, PNTR CddExtAlignCellPtr;

typedef struct _cdd_ext_alignment_column {
  Int4      conpos;
  Int4      occpos;
  Int4      ntypes;
  Int4      *typecount;
  FloatHi   *wtypefreq;
  FloatHi   w_occpos;
  Uint4     aatype;
} CddExtAlignCol, PNTR CddExtAlignColPtr;

typedef struct _cdd_alignment_weight {
  FloatHi   weight;
  Int4      nposaligned;
  BioseqPtr bsp;
} CddAlignWeight, PNTR CddAlignWeightPtr;

/*---------------------------------------------------------------------------*/
/* Cdd Explicit Alignment, data structure used for alignment reindexing and  */
/* calculations on pairwise alignments                                       */
/*---------------------------------------------------------------------------*/
typedef struct _cdd_explicit_alignment {
  SeqIdPtr     ids;
  Boolean      bIdAlloc;
  Int4         length;
  Int4         *adata;
} CddExpAlign, PNTR CddExpAlignPtr;

/*---------------------------------------------------------------------------*/
/* removed PHI-Blast specific variables from this data structure             */
/*---------------------------------------------------------------------------*/
typedef struct _pgp_blast_options {
    BLAST_OptionsBlkPtr options;
    CharPtr blast_database;
    BioseqPtr query_bsp, fake_bsp;
    Int4 number_of_descriptions, number_of_alignments;
    FILE *infp, *outfp;
    AsnIoPtr aip_out;
    Boolean html;
    Boolean believe_query;
    Uint4 align_options, print_options;
} PGPBlastOptions, PNTR PGPBlastOptionsPtr;

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* Function prototypes                                                       */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* Cdd asn1 reader and writer wrappers                                       */
/*---------------------------------------------------------------------------*/
Boolean    LIBCALL CddWriteToFile(CddPtr pcdd, CharPtr cFile, Boolean bBin);
CddPtr     LIBCALL CddReadFromFile(CharPtr cFile, Boolean bBin);

Boolean    LIBCALL CddTreeWriteToFile(CddTreePtr pcddt, CharPtr cFile, Boolean bBin);
CddTreePtr LIBCALL CddTreeReadFromFile(CharPtr cFile, Boolean bBin);

/*---------------------------------------------------------------------------*/
/* Cdd Data manipulations and queries                                        */
/*---------------------------------------------------------------------------*/
void    LIBCALL CddAssignDescr(CddPtr pcdd, Pointer pThis, Int4 iWhat, Int4 iIval);
Int4    LIBCALL CddGetStatus(CddPtr pcdd);
Boolean LIBCALL SeqAlignHasConsensus(SeqAlignPtr salp);
Boolean LIBCALL CddHasConsensus(CddPtr pcdd);
void    LIBCALL CddRegularizeFileName(CharPtr cIn, CharPtr cAcc, CharPtr cFn, CharPtr cEx);

/*---------------------------------------------------------------------------*/
/* report Errors in processing and exit immediately                          */
/*---------------------------------------------------------------------------*/
void LIBCALL CddHtmlError(CharPtr cErrTxt);
void LIBCALL CddSevError(CharPtr cErrTxt);

/*---------------------------------------------------------------------------*/
/* extract BioSeqs from the list stored in the CD                            */
/*---------------------------------------------------------------------------*/
BioseqPtr LIBCALL CddFindSip(SeqIdPtr sip, SeqEntryPtr sep);
BioseqPtr LIBCALL CddBioseqCopy (SeqIdPtr newid, BioseqPtr oldbsp, Int4 from,
                                 Int4 to, Uint1 strand, Boolean do_feat);
BioseqPtr LIBCALL CddExtractBioseq(SeqEntryPtr sep, SeqIdPtr sip);
void      LIBCALL CddShrinkBioseq(BioseqPtr bsp);

/*---------------------------------------------------------------------------*/
/* Cdd specific sequence alignment format converters                         */
/*---------------------------------------------------------------------------*/
SeqAlignPtr LIBCALL CddMSLDenDiagToMSLDenSeg(SeqAlignPtr salp);
SeqAlignPtr LIBCALL CddMSLDenSegToMSLDenDiag(SeqAlignPtr salp);
SeqAlignPtr LIBCALL CddMSLDenDiagToMULDenDiag(SeqAlignPtr salp);
Int2        LIBCALL CddTrimSeqAligns(CddPtr pcdd);

/*---------------------------------------------------------------------------*/
/* various routines for calculating PSSM/Alignment information content       */
/*---------------------------------------------------------------------------*/
Nlm_FloatHiPtr LIBCALL SeqAlignInform(SeqAlignPtr salp, BioseqPtr bsp_master,Boolean bHasConsensus);
Nlm_FloatHiPtr LIBCALL CddAlignInform(CddPtr pcdd, Nlm_FloatHi * Niobs);
Nlm_FloatHiPtr LIBCALL CddPssmInform(CddPtr pcdd);
Nlm_FloatHiPtr LIBCALL CddPosFreqInform(Nlm_FloatHi **posFreq, Int4 ncol, Int4 nrow);

/*---------------------------------------------------------------------------*/
/* Create a BlastOptionsPtr for the PSSM Computations                        */
/*---------------------------------------------------------------------------*/
static PGPBlastOptionsPtr CddReadBlastOptions(BioseqPtr bsp, Int4 iPseudo, CharPtr matrix_name);

/*---------------------------------------------------------------------------*/
/* Calculate PSSMs for DenseDiag (or DenseSeg) Master_Slave alignment sets   */
/*---------------------------------------------------------------------------*/
void LIBCALL CddDenDiagCposComputation(SeqAlignPtr listOfSeqAligns, BioseqPtr bsp,
                                       BioseqPtr bspF, CddPtr pcdd, Int4 pseudoCnt);
void LIBCALL CddCposComputation(SeqAlignPtr listOfSeqAligns, BioseqPtr bsp, CddPtr pcdd);
static void CddputMatrixKbp(FILE *checkFile, BLAST_KarlinBlkPtr kbp,
                            Boolean scaling, Nlm_FloatHi scalingDown);
static void CddputMatrixMatrix(FILE *checkFile, 
                               compactSearchItems *compactSearch,
			       posSearchItems *posSearch,
			       Boolean scaleScores);
static Boolean CddtakeMatrixCheckpoint(compactSearchItems *compactSearch,
                                       posSearchItems *posSearch,
				       BLAST_ScoreBlkPtr sbp, 
                                       Char *fileName,ValNodePtr *error_return,
				       Boolean scaleScores,
				       Nlm_FloatHi scalingFactor);
Seq_Mtf * LIBCALL CddDenDiagCposComp2(BioseqPtr bspFake, Int4 iPseudo,
                                      SeqAlignPtr salp, CddPtr pcdd,
				      BioseqPtr bspOut, double Weight,
				      double ScaleFactor, CharPtr matrix_name);

/*---------------------------------------------------------------------------*/
/* this function combines CddCposComputation and CddDenDiagCposComputation   */
/* into 1 function.  no need to calculate fake bsp beforehand.               */
/*---------------------------------------------------------------------------*/
void LIBCALL CddCposComp(SeqAlignPtr listOfSeqAligns, BioseqPtr bsp, CddPtr pcdd);

/*---------------------------------------------------------------------------*/
/* same as CddCposComp, except pssm is put in a Seq_Mtf instead of a Cdd     */
/*---------------------------------------------------------------------------*/
Seq_Mtf* LIBCALL CddCalcPSSM(SeqAlignPtr listOfSeqAligns, BioseqPtr bsp,
                             double Weight, double ScaleFactor);
Int4     LIBCALL CddGetNewIndexForThreading(char InChar, char* Output);

/*---------------------------------------------------------------------------*/
/* Utility Functions for PSSM calculation                                    */
/*---------------------------------------------------------------------------*/
Int4 LIBCALL CddCountSeqAligns(SeqAlignPtr listOfSeqAligns, Int4 * numSequences);
Int4 LIBCALL CddCountDenDiagSeqAligns(SeqAlignPtr listOfSeqAligns, Int4 * numSequences);

/*---------------------------------------------------------------------------*/
/* assign the range of the master sequence involved in alignments            */
/*---------------------------------------------------------------------------*/
void LIBCALL CddAssignProfileRange(CddPtr pcdd, SeqIdPtr sip);

/*---------------------------------------------------------------------------*/
/* return a pointer to a specific bioseq from a seq-entry set                */
/*---------------------------------------------------------------------------*/
BioseqPtr LIBCALL CddFindBioseqInSeqEntry(SeqEntryPtr sep, SeqIdPtr sip);

/*---------------------------------------------------------------------------*/
/* Are two Sequence-Id's the same?                                           */
/*---------------------------------------------------------------------------*/
Boolean LIBCALL CddSameSip(SeqIdPtr sip1, SeqIdPtr sip2);

/*---------------------------------------------------------------------------*/
/* add an offset to a Seqalign                                               */
/*---------------------------------------------------------------------------*/
void LIBCALL CddReindexMSLDenSegMaster(SeqAlignPtr salp, Int4 offset);
void LIBCALL CddReindexMSLDenDiagMaster(SeqAlignPtr salp, Int4 offset);

/*---------------------------------------------------------------------------*/
/* Transfer alignment annotation between sequences in the alignment          */
/*---------------------------------------------------------------------------*/
static SeqIdPtr CddFindSeqIdInSeqLoc(SeqLocPtr location);
static SeqIdPtr CddFindSeqIdInAlignAnnot(AlignAnnotPtr oldannot);
void   LIBCALL  CddTransferAlignAnnot(AlignAnnotPtr oldannot,
                                      SeqIdPtr newMaster,
				      SeqAlignPtr salp,
				      BioseqSetPtr bssp);

/*---------------------------------------------------------------------------*/
/* reindex a Seqalign to a new "Master"                                      */
/*---------------------------------------------------------------------------*/
SeqAlignPtr LIBCALL CddReindexSeqAlign(SeqAlignPtr salp, SeqIdPtr sipMaster,
                                       BioseqSetPtr bssp);

/*---------------------------------------------------------------------------*/
/* Make a copy of a DenDiag Master-Slave list alignment                      */
/*---------------------------------------------------------------------------*/
SeqAlignPtr LIBCALL CddCopyMSLDenDiag(SeqAlignPtr salp);

/*---------------------------------------------------------------------------*/
/* Utility functions for Alignment Reindexing and pairwise comparisons       */
/*---------------------------------------------------------------------------*/
CddExpAlignPtr CddExpAlignNew();
CddExpAlignPtr CddExpAlignFree(CddExpAlignPtr pCDea);
void           CddExpAlignAlloc(CddExpAlignPtr pCDea, Int4 iLength);
CddExpAlignPtr CddExpAlignRevert(CddExpAlignPtr pCDea, Int4 iLength);
CddExpAlignPtr CddReindexExpAlign(CddExpAlignPtr pCDea1, Int4 newlength, CddExpAlignPtr pCDea2, Int4 iOuter, Int4 iInner);
SeqAlignPtr    CddExpAlignToSeqAlign(CddExpAlignPtr pCDea, Int4Ptr iBreakAfter);
Int2   LIBCALL CddGetProperBlocks(CddPtr pcdd, Boolean modify, Int4Ptr *iBreakAfter);
FloatHi        CddGetPairId(TrianglePtr pTri, Int4 idx1, Int4 idx2);
static Boolean HitYet(Int4Ptr retlist, Int4 index, Int4 i);
Int4Ptr        CddMostDiverse(TrianglePtr pTri, Int4 length);
Int4Ptr        CddMostSimilarToQuery(ScorePtr psc, Int4 length);
BioseqPtr      CddRetrieveBioseqById(SeqIdPtr sip, SeqEntryPtr sep);
TrianglePtr    CddCalculateTriangle(CddPtr pcdd);
ScorePtr       CddCalculateQuerySim(CddPtr pcdd, SeqAlignPtr salp);

/*---------------------------------------------------------------------------*/
/* rips out and returns a PDBSeqId from a SeqId                              */
/*---------------------------------------------------------------------------*/
PDBSeqIdPtr LIBCALL CddGetPdbSeqId(SeqIdPtr sip);

/*---------------------------------------------------------------------------*/
/* Make a Consensus Sequence and reindex alignment                           */
/*---------------------------------------------------------------------------*/
SeqAlignPtr LIBCALL CddConsensus(SeqAlignPtr salp, SeqEntryPtr sep, BioseqPtr bsp, SeqIntPtr range, BioseqPtr *bspCons, SeqAlignPtr *salpCons);

#ifdef __cplusplus
}
#endif

#endif /* CDDUTIL_H */
