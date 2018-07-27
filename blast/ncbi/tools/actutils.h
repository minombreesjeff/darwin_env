/* $Id: actutils.h,v 6.15 2004/11/22 16:45:24 bollin Exp $
 *===========================================================================
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
*
* ===========================================================================
*
* File Name:  actutils.h
*
* Author:  Sarah Wheelan
*
* Version Creation Date:   2/00
*
* $Revision: 6.15 $
*
* File Description: utility functions for alignments
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: actutils.h,v $
* Revision 6.15  2004/11/22 16:45:24  bollin
* created global alignment function with callback method to allow use of new
* BLAST library
*
* Revision 6.14  2004/11/02 18:53:39  bollin
* made act_get_eval available to other libraries
*
* Revision 6.13  2004/07/30 13:36:47  bollin
* created separate function for reversing features on a sequence, changed code
* to swap plus to minus and minus to plus instead of moving all features to
* minus strand
*
* Revision 6.12  2002/03/27 17:35:39  todorov
* recreated ACT_MakeProfileFromSA
*
* Revision 6.11  2002/03/26 19:56:21  todorov
* alignmgr to alignmgr2 transition
*
* Revision 6.10  2001/09/04 13:47:13  wheelan
* made several functions extern
*
* Revision 6.9  2001/07/13 14:17:54  wheelan
* moved Sqn_GlobalAlign2Seq and associated functions here
*
* Revision 6.8  2000/09/06 18:06:04  sicotte
* Add End gaps for sequin updates
*
* Revision 6.7  2000/08/28 16:19:14  sicotte
* moved AlnMgrSeqAlignMergeTwoPairwiseEx AlnMgrSeqAlignMergeTwoPairwise AlnMgrSeqAlignMergePairwiseSet to actutils.c from alignmgr.c
*
* Revision 6.6  2000/07/08 20:44:11  vakatov
* Get all "#include" out of the 'extern "C" { }' scope;  other cleanup...
*
* Revision 6.5  2000/05/04 16:45:20  wheelan
* changes to profile builder to accomodate IBMed BLAST results
*
* Revision 6.4  2000/04/18 13:57:14  wheelan
* added AlnMgrForcePairwiseContinuousEx
*
* Revision 6.3  2000/03/14 11:25:48  wheelan
* added ACT_ProfileFree functions
*
* Revision 6.2  2000/03/02 21:11:06  lewisg
* use bandalign for import sequence, make standalone ddv use viewmgr, make dialogs modal, send color update
*
* Revision 6.1  2000/02/11 17:31:45  kans
* initial checkin of functions depending upon blast/bandalign (SW)
*
* ==========================================================================
*/

#ifndef __ACTUTILS__
#define __ACTUTILS__

#include <ncbi.h>
#include <objalign.h>
#include <alignmgr2.h>
#include <sqnutils.h>
#include <bandalgn.h>
#include <needleman.h>
#include <dotseq.h>
#include <edutil.h>

#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define CG_WINDOWSIZE 200

#define MAX_LEN 12000


/* defines for profile builder -- how many options are allowed for
*  protein or nucleotide sequences */
#define ACT_NUCLEN 5
#define ACT_PROTLEN 27

typedef struct act_sitelist {
   Int4     start;
   CharPtr  name;
   struct act_sitelist PNTR next;
} ACT_sitelist, PNTR ACT_sitelistPtr;

typedef struct act_topscore {
   FloatHi  score;
   Int4     pos;
   struct act_topscore PNTR next;
} ACT_TopScore, PNTR ACT_TopScorePtr;

typedef struct act_position {
   Int4Ptr  posarray;
   FloatHi  score;
} ACT_Position, PNTR ACT_PositionPtr;

typedef struct act_profile
{
   FloatHiPtr PNTR freq;
   Boolean    nuc;
   Int4       len;
   Int4       numseq;
   FloatHi    confidence;
   struct act_profile PNTR next;
} ACTProfile, PNTR ACTProfilePtr;

typedef struct act_placebounds {
   ACT_TopScorePtr PNTR ats;
   Int4            len;
   ACTProfilePtr   app;
   ACT_PositionPtr  apos;
   Int4Ptr         currpos;
   Int4            currprof;
   Int4            nprof;
   Int4Ptr         boundarray;
   Int4Ptr         numats;
   ACT_TopScorePtr PNTR currats;
} ACT_PlaceBounds, PNTR ACT_PlaceBoundsPtr;


typedef struct act_cginfo {
   Int4             from;
   Int4             to;
   Int4             length;
   Int4             cg;
   Int4             a;
   Int4             c;
   Int4             t;
   Int4             g;
   Int4             n;
   ACT_sitelistPtr  asp;
   CharPtr          sequence;
   struct act_cginfo PNTR next;
} ACT_CGInfo, PNTR ACT_CGInfoPtr;

typedef struct act_statetable
{
   Int4Ptr PNTR  state;
} ACT_Statetab, PNTR ACT_StatetabPtr;

NLM_EXTERN ACT_CGInfoPtr ACT_FindCpG(BioseqPtr bsp);
NLM_EXTERN Uint1 ACT_GetResidue(Int4 pos, Uint1Ptr buf, Int4Ptr offset, BioseqPtr bsp);
NLM_EXTERN ACTProfilePtr ACT_ProfileNew(Boolean nuc);

/***************************************************************************
*
*  ACT_ProfileFree frees a single profile; ACT_ProfileSetFree frees an
*  entire linked list of profiles.
*
***************************************************************************/
NLM_EXTERN ACTProfilePtr ACT_ProfileFree(ACTProfilePtr app);
NLM_EXTERN ACTProfilePtr ACT_ProfileSetFree(ACTProfilePtr app);

NLM_EXTERN void ACT_BuildProfile(SeqLocPtr slp, ACTProfilePtr PNTR app, Int4Ptr count, Int4 length);
NLM_EXTERN FloatHi ACT_ScoreProfile(BioseqPtr bsp, Int4 pos, Uint1 strand, ACTProfilePtr app);
NLM_EXTERN void ACT_EstimateConfidence(ACTProfilePtr app);
NLM_EXTERN ACTProfilePtr ACT_SortProfilesByConfidence(ACTProfilePtr app);
NLM_EXTERN int LIBCALLBACK ACT_CompareProfileConfidence(VoidPtr base, VoidPtr large_son);
NLM_EXTERN ACTProfilePtr ACT_MakeProfileFromSA(SeqAlignPtr sap);
NLM_EXTERN ACT_TopScorePtr PNTR ACT_SortAndTruncate(ACT_TopScorePtr PNTR ats);
NLM_EXTERN ACT_TopScorePtr ACT_FindPeakScores(FloatHiPtr scorearray, Int4 len);
NLM_EXTERN FloatHi act_get_eval(Int4 exp);
NLM_EXTERN ACT_PositionPtr ACT_PlaceByScore(ACT_PlaceBoundsPtr abp);
NLM_EXTERN FloatHi ACT_CalcScore(ACT_PlaceBoundsPtr abp);
NLM_EXTERN SeqAlignPtr Sqn_GlobalAlign2Seq (BioseqPtr bsp1, BioseqPtr bsp2, BoolPtr revcomp);
NLM_EXTERN void ReverseBioseqFeatureStrands (BioseqPtr bsp);
NLM_EXTERN void ACT_GetNthSeqRangeInSASet(SeqAlignPtr sap, Int4 n, Int4Ptr start, Int4Ptr stop);
NLM_EXTERN SeqAlignPtr ACT_FindPiece(BioseqPtr bsp1, BioseqPtr bsp2, Int4 start1, Int4 stop1, Int4 start2, Int4 stop2, Uint1 strand, Int4 which_side);
NLM_EXTERN void SQN_ExtendAlnAlg(SeqAlignPtr sap, Int4 ovl, Int4 which_side, Uint1 strand);
NLM_EXTERN SeqAlignPtr ACT_CleanUpAlignments(SeqAlignPtr sap, Int4 len1, Int4 len2);
extern void ACT_RemoveInconsistentAlnsFromSet (SeqAlignPtr sap, Int4 fuzz, Int4 n);

typedef SeqAlignPtr (LIBCALLBACK *GetAlignmentFunc) (BioseqPtr bsp1, BioseqPtr bsp2);

NLM_EXTERN SeqAlignPtr Sqn_GlobalAlign2SeqEx (BioseqPtr bsp1, BioseqPtr bsp2, BoolPtr revcomp, GetAlignmentFunc aln_func);

/***************************************************************************
*
*   AlnMgr2TruncateSAP truncates a given seqalign to contain only the
*   bioseq coordinates from start to stop on the indicated row.  Anything
*   before those coordinates is discarded; anything remaining afterwards
*   is made into another seqalign and put in sap->next (the original next,
*   if any, is now at sap->next->next). Doesn't work on parent seqaligns.
*   The function returns TRUE if the orignal alignment extended past stop.
*
***************************************************************************/
NLM_EXTERN Boolean AlnMgr2TruncateSAP(SeqAlignPtr sap, Int4 start, Int4 stop, Int4 row);


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
