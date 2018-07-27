/* $Id: cddutil.h,v 1.52 2003/12/09 22:21:35 bauer Exp $
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
* $Revision: 1.52 $
*
* File Description: Header file for cdd api utility functions  
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: cddutil.h,v $
* Revision 1.52  2003/12/09 22:21:35  bauer
* added CddCountResTypes
*
* Revision 1.51  2003/08/25 19:09:47  bauer
* added SeqAlignReadFromFile
*
* Revision 1.50  2003/05/21 17:25:21  bauer
* optional ObjMgr in CddReadDBGetBioseq
*
* Revision 1.49  2003/04/25 14:36:20  bauer
* impalaScaling now returns boolean value
*
* Revision 1.48  2003/02/06 21:04:27  bauer
* fixed bug in reindexing to consensus
*
* Revision 1.47  2002/12/03 14:36:31  bauer
* added CddMSLMixedToMSLDenDiag
*
* Revision 1.46  2002/11/22 21:35:23  bauer
* added SeqAnnotReadFromFile and preservation of scores in DenseSeg to DenseDiag conversion
*
* Revision 1.45  2002/10/10 20:38:19  bauer
* changes to accomodate new spec items
* - old-root node
* - curation-status
*
* Revision 1.44  2002/10/02 17:32:21  bauer
* avoid merging blocks when reindexing alignments
*
* Revision 1.43  2002/08/17 11:55:08  bauer
* backed out changes
*
* Revision 1.42  2002/08/16 19:51:46  bauer
* added Ben's CddSrvGetStyle2
*
* Revision 1.41  2002/07/31 14:58:58  bauer
* BLAST DB Sequence Retrieval
*
* Revision 1.40  2002/07/17 18:54:11  bauer
* CddFeaturesAreConsistent now returns explicit error messages
*
* Revision 1.39  2002/07/10 15:34:31  bauer
* made SipIsConsensus public
*
* Revision 1.38  2002/07/09 21:12:40  bauer
* added CddDenDiagCposComp2KBP to return Karlin-Altschul parameters together with PSSM
*
* Revision 1.37  2002/07/05 21:09:26  bauer
* added GetAlignmentSize
*
* Revision 1.36  2002/05/06 16:59:51  bauer
* remove blanks in inferred CD short names
*
* Revision 1.35  2002/04/22 16:37:31  bauer
* added check for missing structure alignments
*
* Revision 1.34  2002/04/18 21:00:27  bauer
* added check CddFeaturesAreConsistent
*
* Revision 1.33  2002/04/12 14:02:43  bauer
* added update_date case to CddAssignDescr
*
* Revision 1.32  2002/04/11 14:34:26  bauer
* added CddRemoveConsensus
*
* Revision 1.31  2002/02/20 22:27:28  bauer
* utility functions for the CD-Server
*
* Revision 1.30  2002/02/12 23:00:47  bauer
* added missing break in CddRelocateSeqLoc
*
* Revision 1.29  2002/02/06 19:35:37  bauer
* some more CddGet.. functionality
*
* Revision 1.28  2002/02/05 23:15:41  bauer
* added a few CddGet.. utility functions, changes to CddAddDescr allow to extend scrapbook line by line
*
* Revision 1.27  2002/01/05 14:49:44  bauer
* made SeqAlignDup a local function
*
* Revision 1.26  2002/01/04 19:46:56  bauer
* added functions to interconvert PSSM-Ids and accessions
*
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
/*#include <objcn3d.h> */
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
  Int4         *starts;
} CddExpAlign, PNTR CddExpAlignPtr;

typedef struct _cdd_idx_data {
  Int4     iPssmId;
  CharPtr  cCDDid;
  struct _cdd_idx_data PNTR next;
} CddIdxData, PNTR CddIdxDataPtr;

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
/* drawing style conventions for Cn3D v4.x                                   */
/*---------------------------------------------------------------------------*/
/*
Int4 cdd_def_style[] =
{
    Cn3d_backbone_type_trace,
    Cn3d_drawing_style_tubes,
     Cn3d_color_scheme_weighted_variety,
    10000,5000,5000,5000,10000,
    Cn3d_backbone_type_trace,
    Cn3d_drawing_style_tubes,
    Cn3d_color_scheme_molecule,
    10000,5000,5000,5000,10000,
    TRUE,
    Cn3d_drawing_style_wire,
    Cn3d_color_scheme_weighted_variety,
    10000,5000,5000,5000,10000,
    TRUE,
    Cn3d_drawing_style_wire,
    Cn3d_color_scheme_molecule,
    10000,5000,5000,5000,10000,
    TRUE,
    Cn3d_drawing_style_ball_and_stick,
    Cn3d_color_scheme_element,
    10000,5000,5000,5000,10000,
    FALSE,
    Cn3d_drawing_style_ball_and_stick,
    Cn3d_color_scheme_element,
    10000,5000,5000,5000,10000,
    TRUE,
    Cn3d_drawing_style_tubes,
    Cn3d_color_scheme_user_select,
    10000,9000,9000,10000,10000,
    FALSE,
    Cn3d_drawing_style_with_arrows,
    Cn3d_color_scheme_object,
    10000,5000,5000,5000,10000,
    FALSE,
    Cn3d_drawing_style_with_arrows,
    Cn3d_color_scheme_object,
    10000,5000,5000,5000,10000,
    TRUE,
    10000,9300,5500,500,10000,
    FALSE,
    10000,0,0,0,10000,
    10000,
    10000,
    4000,
    2000,
    3000,
    3000,
    18000,
    20000,
    5000,
    0,
    Cn3d_backbone_label_style_type_three_letter,
    Cn3d_backbone_label_style_number_sequential,
    FALSE,
    TRUE,
    0,
    Cn3d_backbone_label_style_type_three_letter,
    Cn3d_backbone_label_style_number_sequential,
    FALSE,
    TRUE,
    TRUE
};
Int4 cdd_evidence_style[] =
{
    Cn3d_backbone_type_trace,
    Cn3d_drawing_style_tubes,
    Cn3d_color_scheme_user_select,
    10000,0,10000,0,10000,
    Cn3d_backbone_type_complete,
    Cn3d_drawing_style_ball_and_stick,
    Cn3d_color_scheme_user_select,
    10000,0,5019,10000,10000,
    TRUE,
    Cn3d_drawing_style_tubes,
    Cn3d_color_scheme_user_select,
    10000,0,10000,0,10000,
    TRUE,
    Cn3d_drawing_style_ball_and_stick,
    Cn3d_color_scheme_user_select,
    10000,10000,5019,0,10000,
    TRUE,
    Cn3d_drawing_style_space_fill,
    Cn3d_color_scheme_user_select,
    10000,10000,5019,0,10000,
    TRUE,
    Cn3d_drawing_style_ball_and_stick,
    Cn3d_color_scheme_user_select,
    10000,10000,5019,0,10000,
    TRUE,
    Cn3d_drawing_style_tubes,
    Cn3d_color_scheme_user_select,
    10000,9000,9000,10000,10000,
    FALSE,
    Cn3d_drawing_style_with_arrows,
    Cn3d_color_scheme_object,
    10000,5000,5000,5000,10000,
    FALSE,
    Cn3d_drawing_style_with_arrows,
    Cn3d_color_scheme_object,
    10000,5000,5000,5000,10000,
    TRUE,
    10000,9300,5500,500,10000,
    FALSE,
    10000,0,0,0,10000,
    10000,
    10000,
    4000,
    2000,
    3000,
    3000,
    18000,
    20000,
    5000,
    0,
    Cn3d_backbone_label_style_type_three_letter,
    Cn3d_backbone_label_style_number_sequential,
    FALSE,
    TRUE,
    0,
    Cn3d_backbone_label_style_type_three_letter,
    Cn3d_backbone_label_style_number_sequential,
    FALSE,
    TRUE,
    TRUE
};
*/

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* Function prototypes                                                       */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* Cdd asn1 reader and writer wrappers                                       */
/*---------------------------------------------------------------------------*/
Boolean     LIBCALL CddWriteToFile(CddPtr pcdd, CharPtr cFile, Boolean bBin);
CddPtr      LIBCALL CddReadFromFile(CharPtr cFile, Boolean bBin);
SeqAnnotPtr LIBCALL SeqAnnotReadFromFile(CharPtr cFile, Boolean bBin);
SeqAlignPtr LIBCALL SeqAlignReadFromFile(CharPtr cFile, Boolean bBin);

Boolean     LIBCALL CddTreeWriteToFile(CddTreePtr pcddt, CharPtr cFile, Boolean bBin);
CddTreePtr  LIBCALL CddTreeReadFromFile(CharPtr cFile, Boolean bBin);

/*---------------------------------------------------------------------------*/
/* Cdd Data manipulations and queries                                        */
/*---------------------------------------------------------------------------*/
void       LIBCALL CddAssignDescr(CddPtr pcdd, Pointer pThis, Int4 iWhat, Int4 iIval);
Boolean    LIBCALL CddKillDescr(CddPtr pcdd, Pointer pThis, Int4 iWhat, Int4 iIval);
CharPtr    LIBCALL CddGetAccession(CddPtr pcdd);
Int4       LIBCALL CddGetVersion(CddPtr pcdd);
OrgRefPtr  LIBCALL CddGetOrgRef(CddPtr pcdd);
Int4       LIBCALL CddGetPssmId(CddPtr pcdd);
Int4       LIBCALL CddGetPmIds(CddPtr pcdd, Int4Ptr iPMids);
CharPtr    LIBCALL CddGetDescr(CddPtr pcdd);
DatePtr    LIBCALL CddGetCreateDate(CddPtr pcdd);
DatePtr    LIBCALL CddGetUpdateDate(CddPtr pcdd);
CharPtr    LIBCALL CddGetSource(CddPtr pcdd);
CharPtr    LIBCALL CddGetSourceId(CddPtr pcdd);
Int4       LIBCALL CddGetStatus(CddPtr pcdd);
Int4       LIBCALL CddGetAlignmentLength(CddPtr pcdd);
Int4Ptr    LIBCALL GetAlignmentSize(SeqAlignPtr salp);
ValNodePtr LIBCALL CddGetAnnotNames(CddPtr pcdd);
Boolean    LIBCALL CddHasDescription(CddPtr pcdd, CharPtr pc);
Boolean    LIBCALL CddHasAnnotation(CddPtr pcdd);
Boolean    LIBCALL CddMasterIs3D(CddPtr pcdd);
Int4       LIBCALL CddCount3DAlignments(CddPtr pcdd); 
Boolean    LIBCALL SeqAlignHasConsensus(SeqAlignPtr salp);
Boolean    LIBCALL SipIsConsensus(SeqIdPtr sip);
Boolean    LIBCALL CddHasConsensus(CddPtr pcdd);
void       LIBCALL CddRegularizeFileName(CharPtr cIn, CharPtr cAcc, CharPtr cFn, CharPtr cEx);
Boolean    LIBCALL CddCheckForRepeats(CddPtr pcdd, Int4 width, Int4 GapI, Int4 GapE,
                                      Nlm_FloatHi rthresh, Boolean bOutput);
void       LIBCALL CddTruncStringAtFirstPunct(CharPtr pChar);
void       LIBCALL CddFillBlanksInString(CharPtr pChar);
Boolean    LIBCALL CddFeaturesAreConsistent(CddPtr pcdd, CharPtr errmsg);
Boolean    LIBCALL CddHas3DSuperpos(CddPtr pcdd);
Boolean    LIBCALL CddHasPendingAlignments(CddPtr pcdd);
CddPtr     LIBCALL CddFreeCarefully(CddPtr pcdd);

/*---------------------------------------------------------------------------*/
/* report Errors in processing and exit immediately                          */
/*---------------------------------------------------------------------------*/
void LIBCALL CddSimpleHtmlError(CharPtr cErrTxt);
void LIBCALL CddSevError(CharPtr cErrTxt);

/*---------------------------------------------------------------------------*/
/* extract BioSeqs from the list stored in the CD                            */
/*---------------------------------------------------------------------------*/
BioseqPtr   LIBCALL CddFindSip(SeqIdPtr sip, SeqEntryPtr sep);
BioseqPtr   LIBCALL CddBioseqCopy(SeqIdPtr newid, BioseqPtr oldbsp, Int4 from,
                                  Int4 to, Uint1 strand, Boolean do_feat);
BioseqPtr   LIBCALL CddExtractBioseq(SeqEntryPtr sep, SeqIdPtr sip);
void        LIBCALL CddShrinkBioseq(BioseqPtr bsp);
SeqAnnotPtr LIBCALL CddFindMMDBIdInBioseq(BioseqPtr bsp, Int4 *iMMDBid);

/*---------------------------------------------------------------------------*/
/* Cdd specific sequence alignment format converters                         */
/*---------------------------------------------------------------------------*/
SeqAlignPtr LIBCALL CddMSLMixedToMSLDenDiag(SeqAlignPtr salp);
SeqAlignPtr LIBCALL CddMSLDenDiagToMSLDenSeg(SeqAlignPtr salp);
SeqAlignPtr LIBCALL CddMSLDenSegToMSLDenDiag(SeqAlignPtr salp);
SeqAlignPtr LIBCALL CddMSLDenDiagToMULDenDiag(SeqAlignPtr salp);
Int2        LIBCALL CddTrimSeqAligns(CddPtr pcdd);

/*---------------------------------------------------------------------------*/
/* various routines for calculating PSSM/Alignment information content       */
/*---------------------------------------------------------------------------*/
Nlm_FloatHi    LIBCALL SeqAlignConservation(SeqAlignPtr salp, Nlm_FloatHi fract,BioseqPtr bsp_master, Boolean bHasConsensus, Int4 offset);
Nlm_FloatHiPtr LIBCALL SeqAlignInform(SeqAlignPtr salp, BioseqPtr bsp_master,Boolean bHasConsensus,Int4 offset);
Int4 **        LIBCALL CddCountResTypes(CddPtr pcdd, Int4 *ncols);
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
static void BlastKarlinBlkCopy(BLAST_KarlinBlkPtr kbp_in,BLAST_KarlinBlkPtr kbp_out);
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
/*---------------------------------------------------------------------------*/
/* calculate a PSSM                                                          */
/*---------------------------------------------------------------------------*/
Seq_Mtf * LIBCALL CddDenDiagCposComp2(BioseqPtr bspFake, Int4 iPseudo,
                                      SeqAlignPtr salp, CddPtr pcdd,
				      BioseqPtr bspOut, double Weight,
				      double ScaleFactor, CharPtr matrix_name);
/*---------------------------------------------------------------------------*/
/* calculate a PSSM and also fill the kbp data structure. Must be initialized*/
/* with BlastKarlinBlkCreate();                                              */
/*---------------------------------------------------------------------------*/
Seq_Mtf * LIBCALL CddDenDiagCposComp2KBP(BioseqPtr bspFake, Int4 iPseudo,
                                         SeqAlignPtr salp, CddPtr pcdd,
				         BioseqPtr bspOut, double Weight,
				         double ScaleFactor, CharPtr matrix_name,
				         BLAST_KarlinBlkPtr kbp);

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
/* and return a list detailing a sequence location in residue numbers        */
/*---------------------------------------------------------------------------*/
static  SeqIdPtr        CddFindSeqIdInSeqLoc(SeqLocPtr location);
        Int4Ptr LIBCALL CddGetFeatLocList(SeqLocPtr location, Int4 *nres);
static  void            CddRelocateSeqLoc(SeqLocPtr location, SeqIdPtr sip, Int4 *ali);
static  Int4            CddSeqLocInExpAlign(SeqLocPtr location, CddExpAlignPtr eap);
static  SeqIdPtr        CddFindSeqIdInAlignAnnot(AlignAnnotPtr oldannot);
        void    LIBCALL CddTransferAlignAnnot(AlignAnnotPtr oldannot,
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
CddExpAlignPtr         CddExpAlignNew();
CddExpAlignPtr         CddExpAlignFree(CddExpAlignPtr pCDea);
void                   CddExpAlignAlloc(CddExpAlignPtr pCDea, Int4 iLength);
CddExpAlignPtr         CddExpAlignRevert(CddExpAlignPtr pCDea, Int4 iLength);
CddExpAlignPtr         CddReindexExpAlign(CddExpAlignPtr pCDea1, Int4 newlength, CddExpAlignPtr pCDea2, Int4 iOuter, Int4 iInner);
CddExpAlignPtr LIBCALL SeqAlignToCddExpAlign(SeqAlignPtr salp, SeqEntryPtr sep);
CddExpAlignPtr         InvertCddExpAlign(CddExpAlignPtr pCDea, SeqEntryPtr sep);
SeqAlignPtr            CddExpAlignToSeqAlign(CddExpAlignPtr pCDea, Int4Ptr iBreakAfter);
Int2           LIBCALL CddGetProperBlocks(CddPtr pcdd, Boolean modify, Int4Ptr *iBreakAfter);
FloatHi                CddGetPairId(TrianglePtr pTri, Int4 idx1, Int4 idx2);
static Boolean         HitYet(Int4Ptr retlist, Int4 index, Int4 i);
Int4Ptr                CddMostDiverse(TrianglePtr pTri, Int4 length, Int4 maxdiv);
Int4Ptr                CddMostSimilarToQuery(ScorePtr psc, Int4 length);
BioseqPtr              CddRetrieveBioseqById(SeqIdPtr sip, SeqEntryPtr sep);
TrianglePtr            CddCalculateTriangle(CddPtr pcdd);
ScorePtr               CddCalculateQuerySim(CddPtr pcdd, SeqAlignPtr salp);

/*---------------------------------------------------------------------------*/
/* rips out and returns a PDBSeqId from a SeqId                              */
/*---------------------------------------------------------------------------*/
PDBSeqIdPtr LIBCALL CddGetPdbSeqId(SeqIdPtr sip);

/*---------------------------------------------------------------------------*/
/* Make a Consensus Sequence and reindex alignment                           */
/*---------------------------------------------------------------------------*/
SeqAnnotPtr LIBCALL CddSeqAnnotForSeqAlign (SeqAlignPtr salp);
SeqAlignPtr LIBCALL CddSeqAlignDup (SeqAlignPtr salp);
SeqAlignPtr LIBCALL SeqAlignSetDup(SeqAlignPtr salp);
void        LIBCALL CddDegapSeqAlign(SeqAlignPtr salp);
SeqIdPtr    LIBCALL CddSeqIdDupPDBGI(SeqIdPtr sipold);
SeqAlignPtr LIBCALL CddConsensus(SeqAlignPtr salp, SeqEntryPtr sep, BioseqPtr bsp, SeqIntPtr range, BioseqPtr *bspCons, SeqAlignPtr *salpCons);
Boolean     LIBCALL CddRemoveConsensus(CddPtr pcdd);

/*---------------------------------------------------------------------------*/
/* dump out CD contents used for Entrez indexing in pseudo XML               */
/*---------------------------------------------------------------------------*/
static CharPtr CddFixForXML(CharPtr pc);
void LIBCALL CddDumpPMLinks(CddPtr pcdd, FILE *FP);
void LIBCALL CddDumpTaxLinks(CddPtr pcdd, FILE *FP);
void LIBCALL CddDumpXML(CddPtr pcdd, FILE *FP);

/*---------------------------------------------------------------------------*/
/* conversions between PSSM-Ids and CDD accessions                           */
/*---------------------------------------------------------------------------*/
CddIdxDataPtr LIBCALL CddIdxDataNew(CharPtr acc, Int4 uid);
CddIdxDataPtr LIBCALL CddIdxDataLink(CddIdxDataPtr PNTR head, CddIdxDataPtr cidp);
CddIdxDataPtr LIBCALL CddReadIdx(CharPtr CDDidx);
void LIBCALL CddAccFromPssmId(Int4 iPssmId, CharPtr cAcc, CharPtr CDDidx);
void LIBCALL CddPssmIdFromAcc(Int4 *iPssmId, CharPtr cAcc, CharPtr CDDidx);

/*---------------------------------------------------------------------------*/
/* Bioseq retrieval from BLAST db - contributed by Ben                       */
/*---------------------------------------------------------------------------*/
Boolean   LIBCALL SeqHasTax(BioseqPtr bsp);
BioseqPtr LIBCALL CddReadDBGetBioseq(SeqIdPtr query, Int4 index, ReadDBFILEPtr rdfp);
BioseqPtr LIBCALL CddReadDBGetBioseqEx(SeqIdPtr query, Int4 index, ReadDBFILEPtr rdfp, Boolean bUseObjMgr);

/*---------------------------------------------------------------------------*/
/* setting styles for Cn3D v4.x                                              */
/*---------------------------------------------------------------------------*/
/*
Cn3dStyleDictionaryPtr LIBCALL CddSrvGetStyle2(Int4 *styles[], Int4 nstyles);
static Cn3dStyleSettingsPtr CddSrvGetStyle2_Ex(Int4 style[]);
static Cn3dColorPtr MyCn3dColorInit(Int4 scale_factor, Int4 red, Int4 green, Int4 blue,Int4 alpha);
*/

#ifdef __cplusplus
}
#endif

#endif /* CDDUTIL_H */
