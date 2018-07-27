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
*
* ===========================================================================
*
* File Name:  alignmgr2.c
*
* Author:  Sarah Wheelan
*
* Version Creation Date:  10/01 
*
* $Revision: 6.58 $
*
* File Description: SeqAlign indexing, access, and manipulation functions 
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: alignmgr2.c,v $
* Revision 6.58  2005/03/01 13:56:03  bollin
* if the alignment we want to index is a DenseSeg and not a list of alignments,
* just give it a simple index - don't decompose to pairwise and reconstruct it.
*
* Revision 6.57  2005/02/23 14:40:55  bollin
* when condensing columns in AlnMgr2CondenseColumns, make sure we do not
* disturb the ascending order of starts for each row
*
* Revision 6.56  2004/09/15 14:59:19  bollin
* make sure we do not read outside the alignment index arrays
*
* Revision 6.55  2004/05/20 19:46:25  bollin
* removed unused variables
*
* Revision 6.54  2004/05/11 13:19:49  bollin
* update the dimension of the shared alignment after adding a sequence.
*
* Revision 6.53  2004/04/13 14:43:07  kskatz
* Final resolution of revisions 6.51 and 6.52: reverted 6.52; then  cleaned up readability of AlnMgr2SeqPortRead() and ensured that it will never call SeqPortRead for a length > AM_SEQPORTSIZE
*
* Revision 6.52  2004/04/12 19:52:15  kskatz
* Revision 6.51 was right neighborhood,wrong off-by-one: It was in AlnMgr2ComputeFreqMatrix() call to AlnMgr2SeqPortRead() when using l+AM_SEQPORTSIZE instead of l+AM_SEQPORTSIZE-1
*
* Revision 6.51  2004/04/12 17:00:44  kskatz
* Fixed off-by-one error in AlnMgr2SeqPortRead() length passed to SeqPortRead(); stop-start+1 changed to stop-start
*
* Revision 6.50  2004/03/11 14:15:41  bollin
* added extra check in AlnMgr2GetNthSeqIdPtr to avoid core dump if there are
* fewer than N SeqIDs in the alignment.
*
* Revision 6.49  2003/10/20 17:54:34  kans
* AlnMgr2ComputeFreqMatrix protect against dereferencing NULL bsp
*
* Revision 6.48  2003/10/09 13:46:52  rsmith
* Add AlnMgr2GetFirstNForSipList.
*
* Revision 6.47  2003/05/15 18:53:10  rsmith
* in AlnMgr2GetSeqRangeForSipInStdSeg always return start & stop in coordinate order. Do not assume what minus strand will do or not.
*
* Revision 6.46  2003/04/24 20:28:48  rsmith
* made AlnMgr2GetNthStdSeg use 1 based numbering like the other Nth functions.
*
* Revision 6.45  2003/04/23 20:36:13  rsmith
* Added four functions in Section 11 to get information about Std-Seg alignments.
*
* Revision 6.44  2003/03/31 20:17:11  todorov
* Added AlnMgr2IndexSeqAlignEx
*
* Revision 6.43  2003/02/03 12:36:22  kans
* AlnMgr2ComputeScoreForSeqAlign checks return value of AlnMgr2ComputeFreqMatrix, returns -1 if NULL to avoid dereference crash
*
* Revision 6.42  2002/10/23 16:32:19  todorov
* CondenseColumns fixed: needed to move the lens too.
*
* Revision 6.40  2002/10/16 15:54:28  todorov
* use the default dim value if not set
*
* Revision 6.39  2002/08/07 21:57:33  kans
* added AlignMgr2GetFirstNForStdSeg
*
* Revision 6.38  2002/07/11 14:35:51  kans
* fixed Mac complaints about prototypes
*
* Revision 6.37  2002/07/11 12:55:38  wheelan
* added support for std-seg alignments
*
* Revision 6.36  2002/06/04 17:43:07  todorov
* 1) Substituted AddInNewSA with a new and optimized AddInNewPairwiseSA function.
* 2) Fixed a few bugs in other functions.
*
* Revision 6.35  2002/05/17 15:04:42  wheelan
* bug fix in ExtendToCoords
*
* Revision 6.34  2002/05/17 11:02:36  wheelan
* bug fixes in Merge func
*
* Revision 6.32  2002/03/04 17:19:18  wheelan
* added AlnMgr2FuseSet, changed behavior of RemoveInconsistent, fixed GetNextAlnBitBugs
*
* Revision 6.31  2002/01/31 17:41:47  wheelan
* various bug fixes -- no more 0 len segments, better handling of rows that are one big insert, etc.
*
* Revision 6.30  2002/01/30 19:12:53  wheelan
* added RemoveInconsistentAlnsFromSet, ExtractPairwiseSeqAlign, changed behavior of GetSubAlign, changed structures and behavior of GetNextAlnBit, added GetInterruptInfo, added AlnMgr2IndexAsRows, bug fixes in indexing routines
*
* Revision 6.29  2002/01/02 15:05:07  wheelan
* changes to force more efficient ordering in CompareAsp callbacks, plus more stringent checks in AlnMgr2AddInNewSA
*
* Revision 6.28  2001/12/28 22:53:20  wheelan
* bug fixes; added AlnMgr2DupAlnAndIndexes, changed some New and Free funcs
*
* Revision 6.27  2001/12/27 16:07:22  wheelan
* bug fix in ExtendToEnd
*
* Revision 6.26  2001/12/20 19:43:20  wheelan
* bug fix in GetNextAlnBit -- no more incorrect inserts
*
* Revision 6.25  2001/12/18 16:36:57  wheelan
* scattered fixes to unaligned region code
*
* Revision 6.24  2001/12/17 19:36:39  wheelan
* various fixes in AlnMgr2AddInNewSA
*
* Revision 6.23  2001/12/14 12:38:50  wheelan
* added functions for ddv
*
* Revision 6.22  2001/12/05 12:25:49  wheelan
* bug fix in SortByNthRow
*
* Revision 6.21  2001/12/04 19:28:55  wheelan
* bug fixes in AddInNewSA and in IndexSingleDenseSegSA
*
* Revision 6.20  2001/12/04 14:31:27  wheelan
* fixes to avoid mistakenly processing AM2_LITE as real indexed alignments
*
* Revision 6.19  2001/11/30 16:55:21  wheelan
* added AlnMgr2PadConservatively
*
* Revision 6.18  2001/11/29 18:38:47  wheelan
* cleanup as recommended by Mac compiler
*
* Revision 6.17  2001/11/29 17:37:16  wheelan
* added ExtendToCoords and MergeTwoAlignments
*
* Revision 6.16  2001/11/27 15:47:40  wheelan
* bug fixes in AnchorSeqAlign, DoCondense, and AddInNewSA
*
* Revision 6.15  2001/11/15 18:23:06  wheelan
* small change in AlnMgr2GetNthRowSpan
*
* Revision 6.14  2001/11/15 18:09:38  wheelan
* another bug fix in AddInNewSA
*
* Revision 6.13  2001/11/15 15:30:54  wheelan
* many bugs fixed, leaks plugged, plus reworked AddInNewSA to use new AMSmall field
*
* Revision 6.12  2001/11/13 14:36:13  wheelan
* many bug fixes in AddInNewSA and MapBioseqToSeqAlign
*
* Revision 6.11  2001/11/08 19:56:07  wheelan
* added AlnMgr2GetNthRowSpanInSA, fixed various memory errors
*
* Revision 6.10  2001/11/08 01:39:15  wheelan
* many bug fixes in and around AddInNewSA
*
* Revision 6.9  2001/11/02 14:01:30  wheelan
* bug fixes in AlnMgr2AddInNewSA
*
* Revision 6.8  2001/10/31 12:00:46  wheelan
* commented out the mistakenly uncommented comment
*
* Revision 6.7  2001/10/30 20:14:38  wheelan
* bug fixes for minus strands in AddInNewSA, bug fix in GetSubAlign
*
* Revision 6.6  2001/10/23 12:14:27  wheelan
* changes in AlnMgr2GetNextAlnBit as well as tree-based multiple alignment algorithm
*
* Revision 6.5  2001/10/18 15:10:53  wheelan
* fixed AlnMgr2ComputeScoreForSeqAlign
*
* Revision 6.4  2001/10/16 12:00:17  wheelan
* added GetParent and FreeEitherIndex
*
* Revision 6.3  2001/10/08 18:43:29  wheelan
* added comments
*
* Revision 6.2  2001/10/03 18:13:01  wheelan
* changed some colliding defines
*
* Revision 6.1  2001/10/03 14:20:11  wheelan
* initial checkin
*
* ==========================================================================
*
*/

#include <alignmgr2.h>

/***************************************************************************
*
*  static functions
*
***************************************************************************/
/* SECTION 1 */
static SARowDat2Ptr SARowDat2New(void);
static void SARowDat2Free(SARowDat2Ptr srdp);
static SARowDat2Ptr SARowDat2Copy(SARowDat2Ptr srdp);
static SAIndex2Ptr SAIndex2New(void);
static SAIndex2Ptr SAIndex2Copy(VoidPtr index);
static AMAlignIndex2Ptr AMAlignIndex2Copy(VoidPtr index);
static void AMIntervalSetFree(AMIntervalSetPtr amint);
/* SECTION 2 */
static void AlnMgr2ConvertDendiagToDensegChain(SeqAlignPtr sap);
static void AlnMgr2IndexSingleDenseSegSA(SeqAlignPtr sap);
static Boolean AlnMgr2UnpackSeqAlign(SeqAlignPtr sap);
static void AlnMgr2ConvertAllToDenseSeg(SeqAlignPtr sap);
static void AlnMgr2DecomposeToPairwise(SeqAlignPtr sap);
static void AlnMgr2HidePairwiseConflicts(SeqAlignPtr sap);
static void AlnMgr2SortBySeqId(SeqAlignPtr sap);
static int LIBCALLBACK AlnMgr2CompareIds(VoidPtr ptr1, VoidPtr ptr2);
static void AlnMgr2TossWorse(SeqAlignPtr sap, Int4 i, Int4 j);
static AMIntervalSetPtr AlnMgr2MakeIntervals(SeqAlignPtr sap);
static void AlnMgr2SortIntervals(AMIntervalSetPtr amint);
static int LIBCALLBACK AlnMgr2CompareIntervals(VoidPtr ptr1, VoidPtr ptr2);
static AMVertexPtr PNTR  AlnMgr2MakeVerticesFromIntervals(SeqAlignPtr sap, AMIntervalSetPtr amint_head, AMVertexPtr PNTR vertexhead, AMEdgePtr PNTR edgehead, Int4Ptr numvertices);
static void AlnMgr2SortVerticesByNumEdges(AMVertexPtr PNTR vertexarray, Int4 numvertices);
static int LIBCALLBACK AlnMgr2CompareVertices(VoidPtr ptr1, VoidPtr ptr2);
static void AlnMgr2SortEdgesByWeight(AMEdgePtr PNTR edge_head);
static int LIBCALLBACK AlnMgr2CompareEdges(VoidPtr ptr1, VoidPtr ptr2);
static Int4 AlnMgr2MatchToVertex(SeqIdPtr sip, Int4 start, Int4 stop, AMVertexPtr PNTR vertexarray, Int4 numvertices);
static void AlnMgr2UsePrimsAlgorithm(AMVertexPtr PNTR vertexarray, Int4 numvertices, AMEdgePtr edge_head);
static void AlnMgr2RecursePrims(AMVertexPtr PNTR vertexarray, AMEdgePtr edge_head);
static AMVertexPtr AlnMgr2GetBetterVertex(AMVertexPtr PNTR vertexarray, AMEdgePtr edge);
static AMEdgePtr AlnMgr2GetEdgeList(Int4 vertexnum, AMEdgePtr edge_head, AMEdgePtr already_used);
static void AlnMgr2CleanUpLeftovers(AMVertexPtr PNTR vertexarray, Int4 numvertices, AMEdgePtr edge_head);
static Boolean AlnMgr2SameSeq(AMVertexPtr vertex1, AMVertexPtr vertex2);
static void AlnMgr2BuildAlignmentFromTree(AMVertexPtr PNTR vertexarray, Int4 numvertices, AMEdgePtr edge_head, SeqAlignPtr sap);
static AMVertexPtr AlnMgr2GetAdjacentVertices(AMVertexPtr vertex, AMVertexPtr PNTR vertexarray, AMEdgePtr edge_head);
static void AlnMgr2AddInNewSA(SeqAlignPtr parent, SeqAlignPtr sap);
static void AlnMgr2AddInNewPairwiseSA(SeqAlignPtr parent, SeqAlignPtr sap);
static Int4 AlnMgr2MapSegStartToSegStart(SeqAlignPtr sap, Int4 pos, Int4 row1, Int4 row2, Int4 len);
static Int4 AlnMgr2GetSegForStartPos(SeqAlignPtr sap, Int4 pos, Int4 row);
static void AlnMgr2CondenseColumns(DenseSegPtr dsp);
static void AlnMgr2CondenseRows(DenseSegPtr dsp, Int4 whichrow);
static Boolean AlnMgr2DoCondense(DenseSegPtr dsp, Int4 rownum1, Int4 rownum2);
static int LIBCALLBACK AlnMgr2CompareCdRows(VoidPtr ptr1, VoidPtr ptr2);
static int LIBCALLBACK AlnMgr2CompareAsps(VoidPtr ptr1, VoidPtr ptr2);
static int LIBCALLBACK AlnMgr2CompareAspsMinus(VoidPtr ptr1, VoidPtr ptr2);
static void AlnMgr2GetFirstSharedRow(SeqAlignPtr sap1, SeqAlignPtr sap2, Int4Ptr n1, Int4Ptr n2);
static SeqIdPtr AlnMgr2SeqIdListsOverlap(SeqIdPtr sip1, SeqIdPtr sip2);
static Int4 AlnMgr2OrderSeqIds(SeqIdPtr sip1, SeqIdPtr sip2);
static void AlnMgr2SetUnaln(SeqAlignPtr sap);
static int LIBCALLBACK AlnMgr2CompareUnalnAMS(VoidPtr ptr1, VoidPtr ptr2);
/* SECTION 4 */
static Int4 binary_search_on_uint4_list(Uint4Ptr list, Uint4 pos, Uint4 listlen);
static Int4 binary_search_on_uint2_list(Uint2Ptr list, Int4 ele, Uint2 listlen);
static void AlnMgr2GetUnalignedInfo(SeqAlignPtr sap, Int4 segment, Int4 row, Int4Ptr from, Int4Ptr to);
static void AlnMgr2GetNthSeqRangeInSASet(SeqAlignPtr sap, Int4 n, Int4Ptr start, Int4Ptr stop);
static Int4 AlnMgr2GetMaxUnalignedLength(SeqAlignPtr sap, Int4 seg);
/* SECTION 5 */
static void AlnMgr2AnchorChild(SeqAlignPtr sap, Int4 which_row);
/* SECTION 8 */
static Int4 AlnMgr2GetScoreForPair(Int4 res1, Int4 res2, Boolean is_prot);
/* SECTION 9 */
static int LIBCALLBACK AMCompareStarts(VoidPtr ptr1, VoidPtr ptr2);


typedef struct am_seqpieceset AMSeqPieceSet, PNTR AMSeqPieceSetPtr;
typedef struct am_seqpiece AMSeqPiece, PNTR AMSeqPiecePtr;

struct am_seqpiece {
  Int4 beg;
  Int4 end;
  Int4 left;
  Int4 right;
  Int4 orig_left;
  Int4 orig_right;
  Boolean aligned;
  Int4 seg;
  Int4 pos;
  DenseSegPtr alt_dsp;
  Int4 alt_seg;
  Int4 alt_pos;
  AMSeqPiecePtr next;
  AMSeqPiecePtr prev;
  AMSeqPieceSetPtr set;
};

struct am_seqpieceset {
  AMSeqPiecePtr head;
  AMSeqPiecePtr tail;
  DenseSegPtr dsp;
  DenseSegPtr alt_dsp;
  Int4 row;
  Int4 row2;
  Int4 alt_row;
  Int4 alt_row2;
  Uint1 strand;
  Boolean plus;
  Int4 max_pos;
  Boolean sign;
  AMSeqPieceSetPtr next;
};



/***************************************************************************
*
*  SECTION 1: Functions for allocating and freeing data structures used
*  by the alignment manager; copying functions are also here.
*
***************************************************************************/

/* SECTION 1 */
static SARowDat2Ptr SARowDat2New(void)
{
   return (SARowDat2Ptr)MemNew(sizeof(SARowDat2));
}

/* SECTION 1 */
static void SARowDat2Free(SARowDat2Ptr srdp)
{
   if (srdp == NULL)
      return;
   if (srdp->sect != NULL)
      MemFree(srdp->sect);
   if (srdp->unsect != NULL)
      MemFree(srdp->unsect);
   MemFree(srdp->insect);
   MemFree(srdp->unaligned);
   MemFree(srdp);
}

/* SECTION 1 */
static SARowDat2Ptr SARowDat2Copy(SARowDat2Ptr srdp)
{
   Int4          i;
   SARowDat2Ptr  srdp2;

   if (srdp == NULL)
      return NULL;
   srdp2 = SARowDat2New();
   srdp2->numsect = srdp->numsect;
   srdp2->sect = (Uint2Ptr)MemNew(srdp2->numsect*sizeof(Uint2));
   for (i=0; i<srdp2->numsect; i++)
   {
      srdp2->sect[i] = srdp->sect[i];
   }
   srdp2->numunsect = srdp->numunsect;
   srdp2->unsect = (Uint2Ptr)MemNew(srdp2->numunsect*sizeof(Uint2));
   for (i=0; i<srdp2->numunsect; i++)
   {
      srdp2->unsect[i] = srdp->unsect[i];
   }
   srdp2->numinsect = srdp->numinsect;
   srdp2->insect = (Uint2Ptr)MemNew(srdp2->numinsect*sizeof(Uint2));
   for (i=0; i<srdp2->numinsect; i++)
   {
      srdp2->insect[i] = srdp->insect[i];
   }
   srdp2->numunaln = srdp->numunaln;
   srdp2->unaligned = (Uint2Ptr)MemNew(srdp2->numunaln*sizeof(Uint2));
   for (i=0; i<srdp2->numunaln; i++)
   {
      srdp2->unaligned[i] = srdp->unaligned[i];
   }
   return srdp2;
}

/* SECTION 1 */
static SAIndex2Ptr SAIndex2New(void)
{
   SAIndex2Ptr  saip;

   saip = (SAIndex2Ptr)MemNew(sizeof(SAIndex2));
   saip->indextype = INDEX_CHILD;
   saip->freefunc = (SeqAlignIndexFreeFunc)(SAIndex2Free2);
   saip->anchor = -1;
   return saip;
}

/* SECTION 1 */
NLM_EXTERN Boolean LIBCALLBACK SAIndex2Free2(VoidPtr index)
{
   Int4        i;
   SAIndex2Ptr  saip;

   if (index == NULL)
      return TRUE;
   saip = (SAIndex2Ptr)(index);
   MemFree(saip->aligncoords);
   for (i=0; i<saip->numrows; i++)
   {
      SARowDat2Free(saip->srdp[i]);
   }
   MemFree(saip->srdp);
   MemFree(saip);
   return TRUE;
}

/* SECTION 1 */
NLM_EXTERN void AlnMgr2FreeInterruptInfo(AMInterrInfoPtr interr)
{
   if (interr == NULL)
      return;
   MemFree(interr->starts);
   MemFree(interr->lens);
   MemFree(interr->types);
   MemFree(interr);
}
 
/* SECTION 1*/
static SAIndex2Ptr SAIndex2Copy(VoidPtr index)
{
   Int4         i;
   SAIndex2Ptr  saip;
   SAIndex2Ptr  saip2;

   saip2 = SAIndex2New();
   saip = (SAIndex2Ptr)(index);
   saip2->numseg = saip->numseg;
   saip2->aligncoords = (Uint4Ptr)MemNew(saip2->numseg*sizeof(Uint4));
   for (i=0; i<saip2->numseg; i++)
   {
      saip2->aligncoords[i] = saip->aligncoords[i];
   }
   saip2->anchor = saip->anchor;
   saip2->numrows = saip->numrows;
   saip2->numseg = saip->numseg;
   saip2->srdp = (SARowDat2Ptr PNTR)MemNew(saip2->numrows*sizeof(SARowDat2));
   for (i=0; i<saip2->numrows; i++)
   {
      saip2->srdp[i] = SARowDat2Copy(saip->srdp[i]);
   }
   saip2->numunaln = saip->numunaln;
   saip2->unaln = (Uint4Ptr)MemNew(saip2->numunaln*sizeof(Uint4));
   for (i=0; i<saip2->numunaln; i++)
   {
      saip2->unaln[i] = saip->unaln[i];
   }
   saip2->numinchain = saip->numinchain;
   saip2->numsplitaln = saip->numsplitaln;
   saip2->score = saip->score;
   saip2->aligned = saip->aligned;
   return saip2;
}

/* SECTION 1 */
static AMAlignIndex2Ptr AMAlignIndex2New(void)
{
   AMAlignIndex2Ptr  amaip;

   amaip = (AMAlignIndex2Ptr)MemNew(sizeof(AMAlignIndex2));
   amaip->indextype = INDEX_PARENT;
   amaip->freefunc = (SeqAlignIndexFreeFunc)(AMAlignIndex2Free2);
   return amaip;
}

/* SECTION 1 */
NLM_EXTERN Boolean LIBCALLBACK AMAlignIndex2Free2(VoidPtr index)
{
   AMAlignIndex2Ptr  amaip;
   Int4              i;

   if (index == NULL)
      return FALSE;
   amaip = (AMAlignIndex2Ptr)(index);
   for (i=0; i<amaip->numrows; i++)
   {
      SeqIdFree(amaip->ids[i]);
   }
   MemFree(amaip->ids);
   MemFree(amaip->saps);
   MemFree(amaip->aligned);
   SeqAlignFree(amaip->sharedaln);
   MemFree(amaip);
   return TRUE;
}

/* SECTION 1 */
static AMAlignIndex2Ptr AMAlignIndex2Copy(VoidPtr index)
{
   AMAlignIndex2Ptr  amaip;
   AMAlignIndex2Ptr  amaip2;
   Int4              i;

   if (index == NULL)
      return NULL;
   amaip = (AMAlignIndex2Ptr)(index);
   amaip2 = AMAlignIndex2New();
   amaip2->alnstyle = amaip->alnstyle;
   amaip2->anchor = amaip->anchor;
   amaip2->numrows = amaip->numrows;
   amaip2->ids = (SeqIdPtr PNTR)MemNew(amaip2->numrows*sizeof(SeqIdPtr));
   for (i=0; i<amaip2->numrows; i++)
   {
      amaip2->ids[i] = SeqIdDup(amaip->ids[i]);
   }
   amaip2->numsaps = amaip->numsaps;
   amaip2->saps = (SeqAlignPtr PNTR)MemNew(amaip2->numsaps*sizeof(SeqAlignPtr));
   amaip2->aligned = (Boolean PNTR)MemNew(amaip2->numsaps*sizeof(Boolean));
   for (i=0; i<amaip2->numsaps; i++)
   {
      amaip2->saps[i] = SeqAlignDup(amaip->saps[i]);
      amaip2->aligned[i] = amaip->aligned[i];
      if (i>0)
         amaip2->saps[i-1]->next = amaip2->saps[i];
   }
   amaip2->sharedaln = AlnMgr2DupAlnAndIndexes(amaip->sharedaln);
   return amaip2;
}

/* SECTION 1 */
NLM_EXTERN void AMAlignIndexFreeEitherIndex(SeqAlignPtr sap)
{
   if (sap == NULL || sap->saip == NULL)
      return;
   if (sap->saip->indextype == INDEX_PARENT)
      AMAlignIndex2Free2(sap->saip);
   else
      SAIndex2Free2(sap->saip);
   sap->saip = NULL;
}

/* SECTION 1 */
NLM_EXTERN SeqAlignPtr AlnMgr2DupAlnAndIndexes(SeqAlignPtr sap)
{
   AMAlignIndex2Ptr  amaip;
   SAIndex2Ptr       saip;
   SeqAlignPtr       sap_new;

   if (sap == NULL)
      return NULL;
   if (sap->saip == NULL)
      return (SeqAlignDup(sap));
   sap_new = NULL;
   if (sap->saip->indextype == INDEX_CHILD)
   {
      sap_new = SeqAlignDup(sap);
      sap_new->saip = (Pointer)SAIndex2Copy(sap->saip);
      saip = (SAIndex2Ptr)(sap_new->saip);
      saip->top = AlnMgr2GetParent(sap);
   } else if (sap->saip->indextype == INDEX_PARENT)
   {
      sap_new = SeqAlignNew();
      sap_new->type = sap->type;
      sap_new->segtype = sap->segtype;
      sap_new->saip = (Pointer)(AMAlignIndex2Copy(sap->saip));
      amaip = (AMAlignIndex2Ptr)(sap_new->saip);
      sap_new->segs = amaip->saps[0];
   }
   return sap_new;
}

/* SECTION 1 */
NLM_EXTERN AlnMsg2Ptr AlnMsgNew2(void)
{
   AlnMsg2Ptr  amp;

   amp = (AlnMsg2Ptr)MemNew(sizeof(AlnMsg2));
   amp->real_from = -2;
   return amp;
}

/* SECTION 1 */
NLM_EXTERN AlnMsg2Ptr AlnMsgFree2(AlnMsg2Ptr amp)
{
   if (amp->left_interrupt != NULL)
   {
      MemFree(amp->left_interrupt);
      amp->left_interrupt = NULL;
   }
   if (amp->right_interrupt != NULL)
   {
      MemFree(amp->right_interrupt);
      amp->right_interrupt = NULL;
   }
   MemFree(amp);
   return NULL;
}

/* SECTION 1 */
NLM_EXTERN void AlnMsgReNew2(AlnMsg2Ptr amp)
{
   if (amp == NULL)
      return;
   if (amp->left_interrupt != NULL)
   {
      MemFree(amp->left_interrupt);
      amp->left_interrupt = NULL;
   }
   if (amp->right_interrupt != NULL)
   {
      MemFree(amp->right_interrupt);
      amp->right_interrupt = NULL;
   }
   amp->real_from = -2;
   amp->len = -2;
   return;
}

/* SECTION 1 */
static void AMIntervalSetFree(AMIntervalSetPtr amint)
{
   AMIntervalPtr  intv;
   AMIntervalPtr  intv_next;

   intv = amint->int_head;
   while (intv != NULL)
   {
      intv_next = intv->next;
      MemFree(intv);
      intv = intv_next;
   }
   SeqIdFree(amint->sip);
   MemFree(amint);
}

/* SECTION 1 */
NLM_EXTERN void AMFreqFree(AMFreqPtr afp)
{
   Int4  i;

   if (afp == NULL)
      return;
   for (i=0; i<afp->size; i++)
   {
      MemFree(afp->freq[i]);
   }
   MemFree(afp->freq);
   MemFree(afp);
}

/* SECTION 1 */
static void AMSeqPieceSetFree(AMSeqPieceSetPtr s_set)
{
  AMSeqPieceSetPtr s_set_next;
  AMSeqPiecePtr s, s_next;
  
  while (s_set) {
    s = s_set->head;
    while (s) {
      s_next = s->next;
      MemFree(s);
      s = s_next;
    }
    s_set_next = s_set->next;
    MemFree(s_set);
    s_set = s_set_next;
  }
}

/***************************************************************************
*
*  SECTION 2: Functions used to create the indexes for parent and child
*  seqaligns.
*    SECTION 2a: Functions to create indexes for child seqaligns, and
*                to convert seqaligns to dense-seg type
*    SECTION 2b: Functions to unpack and rearrange complicated seqaligns
*                into simple chains of dense-seg and dense-diag types
*    SECTION 2c: Functions to create indexes for parent seqaligns
*    SECTION 2d: Accessory functions for parent indexing
*
***************************************************************************/

/***************************************************************************
*
*  AlnMgr2ConvertDendiagToDensegChain takes a dense-diag style alignment
*  and makes each diag into its own denseg seqalign, then links the new
*  alignments together.
*
***************************************************************************/
/* SECTION 2a */
static void AlnMgr2ConvertDendiagToDensegChain(SeqAlignPtr sap)
{
   DenseDiagPtr  ddp;
   DenseDiagPtr  ddp_next;
   DenseSegPtr   dsp;
   Int4          i;
   SeqAlignPtr   sap_new;
   SeqAlignPtr   sap_next;
   SeqAlignPtr   sap_prev;

   if (sap == NULL || sap->segtype != SAS_DENDIAG)
      return;
   sap_next = sap->next;
   ddp = (DenseDiagPtr)(sap->segs);
   /* convert the first diag to dense-seg and put it in the original alignment */
   dsp = DenseSegNew();
   dsp->ids = ddp->id;
   ddp->id = NULL;
   dsp->dim = ddp->dim;
   dsp->numseg = 1;
   dsp->starts = (Int4Ptr)MemNew((dsp->dim)*(dsp->numseg)*sizeof(Int4));
   dsp->lens = (Int4Ptr)MemNew((dsp->numseg)*sizeof(Int4));
   dsp->strands = (Uint1Ptr)MemNew((dsp->dim)*(dsp->numseg)*sizeof(Uint1));
   for (i=0; i<dsp->dim; i++)
   {
      dsp->starts[i] = ddp->starts[i];
      if (ddp->strands != NULL)
         dsp->strands[i] = ddp->strands[i];
      else
         dsp->strands[i] = Seq_strand_plus;
   }
   dsp->lens[0] = ddp->len;
   sap->segs = (Pointer)(dsp);
   sap->segtype = SAS_DENSEG;
   ddp_next = ddp->next;
   ddp->next = NULL;
   DenseDiagFree(ddp);
   ddp = ddp_next;
   if (ddp == NULL)
      return;
   sap_prev = sap;
   while (ddp)
   {
      sap_new = SeqAlignNew();
      sap_new->type = SAT_PARTIAL;
      sap_new->segtype = SAS_DENSEG;
      sap_new->dim = ddp->dim;
      dsp = DenseSegNew();
      dsp->ids = ddp->id;
      ddp->id = NULL;
      dsp->dim = ddp->dim;
      dsp->numseg = 1;
      dsp->starts = (Int4Ptr)MemNew((dsp->dim)*(dsp->numseg)*sizeof(Int4));
      dsp->lens = (Int4Ptr)MemNew((dsp->numseg)*sizeof(Int4));
      dsp->strands = (Uint1Ptr)MemNew((dsp->dim)*(dsp->numseg)*sizeof(Uint1));
      for (i=0; i<dsp->dim; i++)
      {
         dsp->starts[i] = ddp->starts[i];
         if (ddp->strands != NULL)
            dsp->strands[i] = ddp->strands[i];
         else
            dsp->strands[i] = Seq_strand_plus;
      }
      dsp->lens[0] = ddp->len;
      sap_new->segs = (Pointer)(dsp);
      ddp_next = ddp->next;
      ddp->next = NULL;
      DenseDiagFree(ddp);
      ddp = ddp_next;
      sap_prev->next = sap_new;
      sap_prev = sap_new;
   }
   sap_new->next = sap_next;
}

/* SECTION 2a */
/***************************************************************************
*
*  AlnMgr2IndexSingleDenseSegSA creates the SAIndex2 structure for a given
*  dense-seg seqalign. This structure has binary-searchable indexes into
*  the segs. If the strands are not allocated, this function allocates
*  them and sets them to Seq_strand_plus.
*
***************************************************************************/
static void AlnMgr2IndexSingleDenseSegSA(SeqAlignPtr sap)
{
   DenseSegPtr  dsp;
   Int4         i;
   Int4         j;
   Int4         last;
   Int4         next;
   Int4         row;
   SAIndex2Ptr   saip;
   Boolean      unal;

   if (sap->segtype != SAS_DENSEG)
      return;
   dsp = (DenseSegPtr)(sap->segs);
   if (dsp->strands == NULL)
   {
      dsp->strands = (Uint1Ptr)MemNew(dsp->dim*dsp->numseg*sizeof(Uint1));
      for (i=0; i<dsp->dim*dsp->numseg; i++)
      {
         dsp->strands[i] = Seq_strand_plus;
      }
   }
   saip = SAIndex2New();
   saip->aligncoords = (Uint4Ptr)MemNew((dsp->numseg)*sizeof(Uint4));
   saip->srdp = (SARowDat2Ptr PNTR)MemNew((dsp->dim)*sizeof(SARowDat2Ptr));
   saip->numrows = dsp->dim;
   saip->numseg = dsp->numseg;
   for (i=0; i<dsp->dim; i++)
   {
      saip->srdp[i] = SARowDat2New();
   }
   for (i=0; i<dsp->numseg; i++)
   {
      if (i != 0)
         saip->aligncoords[i] = saip->aligncoords[i-1] + dsp->lens[i-1];
      for (row=0; row<dsp->dim; row++)
      {
         if (dsp->starts[dsp->dim*i + row] != -1)
            saip->srdp[row]->numsect++;
      }
   }
   for (row=0; row<dsp->dim; row++)
   {
      saip->srdp[row]->sect = (Uint2Ptr)MemNew((saip->srdp[row]->numsect)*sizeof(Uint2));
      saip->srdp[row]->unsect = (Uint2Ptr)MemNew((dsp->numseg - saip->srdp[row]->numsect)*sizeof(Uint2));
      saip->srdp[row]->numsect = 0;
      saip->srdp[row]->unaligned = (Uint2Ptr)MemNew(dsp->numseg*sizeof(Uint2));
   }
   for (i=0; i<dsp->numseg; i++)
   {
      for (row=0; row<dsp->dim; row++)
      {
         if (dsp->starts[dsp->dim*i + row] != -1)
         {
            saip->srdp[row]->sect[saip->srdp[row]->numsect] = i;
            saip->srdp[row]->numsect++;
         } else
         {
            saip->srdp[row]->unsect[saip->srdp[row]->numunsect] = i;
            saip->srdp[row]->numunsect++;
         }
      }
   }
   for (row=0; row<dsp->dim; row++)
   {
      for (i=0; i<dsp->numseg; i++)
      {
         unal = FALSE;
         last = -1;
         j = i;  /* only blocks with sequence can have flanking unal. regions */
         if (j >= 0 && dsp->starts[dsp->dim*j+row] != -1)
         {
            if (dsp->strands[row] == Seq_strand_minus)
               last = dsp->starts[dsp->dim*j+row];
            else
               last = dsp->starts[dsp->dim*j+row] + dsp->lens[j];
         }
         if (last > -1)
         {
            next = -1;
            j++;
            /* find next block of aligned sequence in this row */
            for (j; j<dsp->numseg && next == -1; j++)
            {
               if (dsp->starts[dsp->dim*j+row] != -1)
               {
                  if (dsp->strands[row] == Seq_strand_minus)
                     next = dsp->starts[dsp->dim*j+row] + dsp->lens[j];
                  else
                     next = dsp->starts[dsp->dim*j+row];
               }
            }
            if (next > -1) /* look for unaligned seq on right side of this seg */
            {
               if (next != last)
                  unal = TRUE;
            }
         }
         if (unal == TRUE)
         {
            saip->srdp[row]->unaligned[saip->srdp[row]->numunaln] = i;
            saip->srdp[row]->numunaln++;
         }
      }
   }
   sap->saip = (SeqAlignIndexPtr)(saip);
}

/* SECTION 2a */
/***************************************************************************
*
*  AlnMgr2IndexSingleChildSeqAlign takes a simple dense-seg or dense-diag
*  seqalign, converts it to dense-seg, and then calls
*  AlnMgr2IndexSingleDenseSegSA to create the indexes. If the alignment has
*  already been indexed, this erases that index and reindexes the alignment.
*  (SINGCHILD)
*
***************************************************************************/
NLM_EXTERN Boolean AlnMgr2IndexSingleChildSeqAlign(SeqAlignPtr sap)
{
   SeqAlignPtr  salp;
   SeqAlignPtr  salp_prev;
   SeqAlignPtr  sap_next;

   if (sap == NULL)
      return FALSE;
   if (sap->saip != NULL)
   {
      if (sap->saip->indextype != INDEX_CHILD)
         return FALSE;
      SAIndex2Free2(sap->saip);
      sap->saip = NULL;
   }
   sap_next = sap->next;
   sap->next = NULL;
   if (sap->segtype == SAS_DISC)
      return FALSE;
   if (sap->segtype == SAS_DENDIAG)
      AlnMgr2ConvertDendiagToDensegChain(sap);
   salp = sap;
   salp_prev = sap;
   while (salp != NULL)
   {
      AlnMgr2IndexSingleDenseSegSA(salp);
      salp_prev = salp;
      salp = salp->next;
   }
   salp_prev->next = sap_next;
   return TRUE;
}

/***************************************************************************
*
*  AlnMgr2UnpackSeqAlign rearranges any seqalign (except alignments with
*  more than two levels of nested discontinuous alignments) to a simple
*  discontinuous alignment or a linked list of alignments.
*
***************************************************************************/
/* SECTION 2b */
static Boolean AlnMgr2UnpackSeqAlign(SeqAlignPtr sap)
{
   SeqAlignPtr  sap_new;
   SeqAlignPtr  sap_next;
   SeqAlignPtr  sap_segs;
   SeqAlignPtr  sap_segs_head;
   SeqAlignPtr  sap_segs_prev;

   if (sap == NULL)
      return FALSE;
   sap_segs = NULL;
   if (sap->segtype == SAS_DISC)
   {
      sap_segs_head = (SeqAlignPtr)(sap->segs);
      if (sap_segs_head->segtype == SAS_DISC)
      {
         sap_segs_prev = (SeqAlignPtr)(sap_segs_head->segs);
         sap_segs_head->segs = NULL;
         sap_next = sap_segs_head->next;
         sap_segs_head->next = NULL;
         SeqAlignFree(sap_segs_head);
         sap_segs_head = sap_segs_prev;
         sap->segs = (Pointer)(sap_segs_head);
         while (sap_segs_prev->next)
         {
            sap_segs_prev = sap_segs_prev->next;
            if (sap_segs_prev->segtype == SAS_DISC)
               return FALSE;
         }
         sap_segs_prev->next = sap_next;
         sap_segs = sap_next;
      } else
         sap_segs = sap_segs_head->next;
      while (sap_segs)
      {
         if (sap_segs->segtype == SAS_DISC)
         {
            sap_next = sap_segs->next;
            sap_segs->next = NULL;
            sap_segs_prev->next = (SeqAlignPtr)(sap_segs->segs);
            sap_segs->segs = NULL;
            SeqAlignFree(sap_segs);
            while (sap_segs_prev->next)
            {
               sap_segs_prev = sap_segs_prev->next;
               if (sap_segs_prev->segtype == SAS_DISC)
                  return FALSE;
            }
            sap_segs_prev->next = sap_next;
            sap_segs = sap_next;
         } else
            sap_segs = sap_segs->next;
      }
   } else
   {
      sap_new = SeqAlignNew();
      sap_new->type = SAT_GLOBAL;
      sap_new->segtype = sap->segtype;
      sap_new->dim = sap->dim;
      sap_new->segs = sap->segs;
      sap_new->master = sap->master;
      sap_new->bounds = sap->bounds;
      sap_new->next = sap->next;
      sap_new->score = sap->score;
      sap->next = NULL;
      sap->segtype = SAS_DISC;
      sap->type = 0;
      sap->dim = 0;
      sap->master = NULL;
      sap->bounds = NULL;
      sap->score = NULL;
      sap->segs = (Pointer)sap_new;
      sap_segs_prev = sap_new;
      sap_segs = sap_new->next;
      while (sap_segs)
      {
         if (sap_segs->segtype == SAS_DISC)
         {
            sap_next = sap_segs->next;
            sap_segs->next = NULL;
            sap_segs_prev->next = (SeqAlignPtr)(sap_segs->segs);
            sap_segs->segs = NULL;
            SeqAlignFree(sap_segs);
            while (sap_segs_prev->next)
            {
               sap_segs_prev = sap_segs_prev->next;
               if (sap_segs_prev->segtype == SAS_DISC)
                  return FALSE;
            }
            sap_segs_prev->next = sap_next;
            sap_segs = sap_next;
         } else
            sap_segs = sap_segs->next;
      }
   }
   return TRUE;
}

/* SECTION 2b */
static void AlnMgr2UnpackSeqAlignChain(SeqAlignPtr sap)
{
   Int4         i;
   SeqAlignPtr  salp_head;
   SeqAlignPtr  salp_prev;
   SeqAlignPtr  sap_next;
   SeqAlignPtr  sap_orig;
   SeqAlignPtr  sap_prev;

   salp_head = salp_prev = NULL;
   i = 0;
   while (sap != NULL)
   {
      sap_next = sap->next;
      sap->next = NULL;
      AlnMgr2UnpackSeqAlign(sap);
      while (sap != NULL)
      {
         if (salp_prev != NULL)
         {
            salp_prev->next = (SeqAlignPtr)(sap->segs);
            sap->segs = NULL;
            while (salp_prev->next != NULL)
            {
               salp_prev = salp_prev->next;
            }
         } else
         {
            salp_head = salp_prev = (SeqAlignPtr)(sap->segs);
            sap->segs = NULL;
            while (salp_prev->next != NULL)
            {
               salp_prev = salp_prev->next;
            }
         }
         sap_prev = sap;
         sap = sap->next;
         sap_prev->next = NULL;
         if (i>0)
            SeqAlignFree(sap_prev);
         else
            sap_orig = sap_prev; /* this is the pointer that was passed in */
         i++;
      }
      sap = sap_next;
   }
   sap_orig->segs = (Pointer)(salp_head);
}

/* SECTION 2b */
/***************************************************************************
*
*  AlnMgr2ConvertAllToDenseSeg goes through a chain of simple child
*  seqaligns and makes sure that each is a dense-seg seqalign with the
*  strands explicitly allocated; dense-diag alignments are converted and
*  non-allocated strands are allocated and all set to Seq_strand_plus.
*
***************************************************************************/
static void AlnMgr2ConvertAllToDenseSeg(SeqAlignPtr sap)
{
   DenseSegPtr  dsp;
   Int4         i;
   SeqAlignPtr  sap_next;

   while (sap != NULL)
   {
      sap_next = sap->next;
      if (sap->segtype == SAS_DENDIAG)
         AlnMgr2ConvertDendiagToDensegChain(sap);
      else if (sap->segtype == SAS_DENSEG)
      {
         dsp = (DenseSegPtr)(sap->segs);
         if (dsp->strands == NULL)
         {
            dsp->strands = (Uint1Ptr)MemNew((dsp->dim)*(dsp->numseg)*sizeof(Uint1));
            for (i=0; i<(dsp->dim)*(dsp->numseg); i++)
            {
               dsp->strands[i] = Seq_strand_plus;
            }
         }
      }
      sap = sap_next;
   }
}

/* SECTION 2c */
/***************************************************************************
*
*  AlnMgr2IndexLite takes a seqalign or a list of seqaligns, converts
*  each alignment to a dense-seg structure and indexes it, and then
*  allocates an AMAlignIndex2 structure and fills in the saps array.
*
***************************************************************************/
NLM_EXTERN Boolean AlnMgr2IndexLite(SeqAlignPtr sap)
{
   AMAlignIndex2Ptr  amaip;
   Int4              i;
   SAIndex2Ptr       saip;
   SeqAlignPtr       salp;

   if (sap == NULL)
      return FALSE;
   if (!AlnMgr2UnpackSeqAlign(sap))
      return FALSE;
   AlnMgr2ConvertAllToDenseSeg((SeqAlignPtr)sap->segs);
   amaip = AMAlignIndex2New();
   amaip->alnstyle = AM2_LITE;
   salp = (SeqAlignPtr)(sap->segs);
   while (salp != NULL)
   {
      amaip->numsaps++;
      AlnMgr2IndexSingleChildSeqAlign(salp);
      salp = salp->next;
   }
   amaip->saps = (SeqAlignPtr PNTR)MemNew((amaip->numsaps)*sizeof(SeqAlignPtr));
   salp = (SeqAlignPtr)(sap->segs);
   i = 0;
   while (salp != NULL)
   {
      amaip->saps[i] = salp;
      i++;
      saip = (SAIndex2Ptr)(salp->saip);
      saip->numinchain = i;
      saip->top = sap;
      salp = salp->next;
   }
   sap->saip = (SeqAlignIndexPtr)amaip;
   amaip->aligned = (Boolean PNTR)MemNew((amaip->numsaps)*sizeof(Boolean));
   for (i=0; i<amaip->numsaps; i++)
   {
      amaip->aligned[i] = TRUE;
   }
   return TRUE;
}

/* SECTION 2c */
/***************************************************************************
*
*  AlnMgr2IndexSeqAlign takes a seqalign of any type except std-seg and
*  creates indexes on it for easy retrieval of useful information by other
*  AlnMgr2 functions. If the seqalign is a single alignment, that alignment
*  gets a simple index and is left alone otherwise. If the seqalign is
*  a set of alignments or a dense-diag set, the subalignments get
*  individually indexed and then are combined into a (fake) multiple
*  alignment which also gets indexed. The subalignments can now be accessed
*  as a multiple alignment by AlnMgr2 functions.
*
***************************************************************************/

NLM_EXTERN void AlnMgr2IndexSeqAlign(SeqAlignPtr sap)
{
   AlnMgr2IndexSeqAlignEx(sap, TRUE);
}

NLM_EXTERN void AlnMgr2IndexSeqAlignEx(SeqAlignPtr sap, Boolean replace_gi)
{
   AMAlignIndex2Ptr  amaip;
   AMIntervalSetPtr  amint;
   AMIntervalSetPtr  amint_head;
   AMEdgePtr         edge;
   AMEdgePtr         edge_head;
   Int4              i;
   Int4              numvertices;
   AMVertexPtr       vertex_head;
   AMVertexPtr       PNTR vertexarray;

   if (sap == NULL || sap->saip != NULL)
      return;
   if (replace_gi) {
     SAM_ReplaceGI(sap);
   }
   
   if (sap->next == NULL && sap->segtype == SAS_DENSEG)
   {
     AlnMgr2IndexSingleChildSeqAlign(sap);
     return;
   }

   AlnMgr2IndexLite(sap);
   AlnMgr2DecomposeToPairwise(sap);
   amaip = (AMAlignIndex2Ptr)(sap->saip);
   amaip->alnstyle = AM2_FULLINDEX;
   AlnMgr2HidePairwiseConflicts(sap);
   amint_head = AlnMgr2MakeIntervals(sap);
   vertex_head = NULL;
   edge_head = NULL;
   vertexarray = AlnMgr2MakeVerticesFromIntervals(sap, amint_head, &vertex_head, &edge_head, &numvertices);
   while (amint_head != NULL)
   {
      amint = amint_head->next;
      AMIntervalSetFree(amint_head);
      amint_head = amint;
   }
   AlnMgr2UsePrimsAlgorithm(vertexarray, numvertices, edge_head);
   AlnMgr2BuildAlignmentFromTree(vertexarray, numvertices, edge_head, sap);
   for (i=0; i<numvertices; i++)
   {
      SeqIdFree(vertexarray[i]->sip);
      MemFree(vertexarray[i]);
   }
   MemFree(vertexarray);
   while (edge_head != NULL)
   {
      edge = edge_head->next;
      MemFree(edge_head);
      edge_head = edge;
   }
   amaip = (AMAlignIndex2Ptr)(sap->saip);
   amaip->alnstyle = AM2_FULLINDEX;
}

/* SECTION 2c */
/***************************************************************************
*
*  AlnMgr2ReIndexSeqAlign takes an indexed alignment (that has, presumably,
*  been changed), makes sure all child seqaligns are indexed (if they are
*  already indexed they are not reindexed), and reindexes all the child
*  seqaligns as a set.
*
***************************************************************************/
NLM_EXTERN void AlnMgr2ReIndexSeqAlign(SeqAlignPtr sap)
{
   AMAlignIndex2Ptr   amaip;
   AMIntervalSetPtr  amint;
   AMIntervalSetPtr  amint_head;
   AMEdgePtr         edge_head;
   Int4              i;
   Int4              numvertices;
   AMVertexPtr       vertex_head;
   AMVertexPtr       PNTR vertexarray;

   if (sap == NULL)
      return;
   if (sap->saip == NULL)
   {
      AlnMgr2IndexSeqAlign(sap);
      return;
   }
   if (sap->saip->indextype == INDEX_CHILD)
      return;
   amaip = (AMAlignIndex2Ptr)(sap->saip);
   for (i=0; i<amaip->numsaps; i++)
   {
      if (amaip->saps[i]->saip == NULL)
         AlnMgr2IndexSingleChildSeqAlign(amaip->saps[i]);
   }
   if (amaip->alnstyle != AM2_LITE)
      return;
   AlnMgr2DecomposeToPairwise(sap);
   AlnMgr2HidePairwiseConflicts(sap);
   amint_head = AlnMgr2MakeIntervals(sap);
   vertex_head = NULL;
   edge_head = NULL;
   vertexarray = AlnMgr2MakeVerticesFromIntervals(sap, amint_head, &vertex_head, &edge_head, &numvertices);
   while (amint_head != NULL)
   {
      amint = amint_head->next;
      AMIntervalSetFree(amint_head);
      amint_head = amint;
   }
   AlnMgr2UsePrimsAlgorithm(vertexarray, numvertices, edge_head);
   AlnMgr2BuildAlignmentFromTree(vertexarray, numvertices, edge_head, sap);
   MemFree(vertexarray);
}

static int LIBCALLBACK AlnMgr2CompareByAnchor(VoidPtr ptr1, VoidPtr ptr2)
{
   DenseSegPtr  dsp;
   int          ret;
   SAIndex2Ptr  saip1;
   SAIndex2Ptr  saip2;
   SeqAlignPtr  sap1;
   SeqAlignPtr  sap2;
   SeqIdPtr     sip1;
   SeqIdPtr     sip2;
   Int4         start1;
   Int4         start2;
   Int4         stop1;
   Int4         stop2;

   sap1 = *((SeqAlignPtr PNTR)ptr1);
   sap2 = *((SeqAlignPtr PNTR)ptr2);
   saip1 = (SAIndex2Ptr)(sap1->saip);
   saip2 = (SAIndex2Ptr)(sap2->saip);
   dsp = (DenseSegPtr)(sap1->segs);
   if (saip1->tmp == 1)
      sip1 = dsp->ids->next;
   else
      sip1 = dsp->ids;
   dsp = (DenseSegPtr)(sap2->segs);
   if (saip2->tmp == 1)
      sip2 = dsp->ids->next;
   else
      sip2 = dsp->ids;
   ret = AlnMgr2OrderSeqIds(sip1, sip2);
   if (ret != 0)
      return ret;
   /* these share both ids -- put best first */
   if (saip1->score == 0)
      saip1->score = AlnMgr2ComputeScoreForSeqAlign(sap1);
   if (saip2->score == 0)
      saip2->score = AlnMgr2ComputeScoreForSeqAlign(sap2);
   if (saip1->score > saip2->score)
      return -1;
   else if (saip1->score < saip2->score)
      return 1;
   AlnMgr2GetNthSeqRangeInSA(sap1, saip1->tmp, &start1, &stop1);
   AlnMgr2GetNthSeqRangeInSA(sap2, saip2->tmp, &start2, &stop2);
   if (start1 < start2)
      return -1;
   else if (start1 > start2)
      return 1;
   else if (stop1 > stop2)
      return -1;
   else if (stop1 < stop2)
      return 1;
   return 0;
}

/* SECTION 2c */
NLM_EXTERN Boolean AlnMgr2IndexAsRows(SeqAlignPtr sap, Uint1 strand, Boolean truncate)
{
   AMAlignIndex2Ptr  amaip;
   DenseSegPtr       dsp;
   DenseSegPtr       dsp_tmp;
   Boolean           found;
   Int4              i;
   Boolean           impossible;
   Int4              numsaps;
   SAIndex2Ptr       saip;
   SeqAlignPtr       salp;
   SeqAlignPtr       sap_head;
   SeqAlignPtr       sap_prev;
   SeqAlignPtr       sap_tmp;
   SeqAlignPtr       PNTR saparray;
   SeqAlignPtr       set_head;
   SeqAlignPtr       set_prev;
   SeqIdPtr          sharedsip;
   SeqIdPtr          sip;
   SeqIdPtr          sip_next;
   SeqIdPtr          sip_tmp;
   Int4              tmp;

   if (sap == NULL)
      return FALSE;
   if (sap->saip != NULL)
      AMAlignIndexFreeEitherIndex(sap);
   AlnMgr2IndexLite(sap);
   AlnMgr2DecomposeToPairwise(sap);
   /* need to figure out which row is shared by all saps */
   sap_tmp = (SeqAlignPtr)(sap->segs);
   dsp = (DenseSegPtr)(sap_tmp->segs);
   sip = dsp->ids;
   found = FALSE;
   while (!found && sip != NULL)
   {
      sap_tmp = (SeqAlignPtr)(sap->segs);
      sip_next = sip->next;
      sip->next = NULL;
      impossible = FALSE;
      while (!impossible && sap_tmp != NULL)
      {
         dsp_tmp = (DenseSegPtr)(sap_tmp->segs);
         if (AlnMgr2SeqIdListsOverlap(sip, dsp_tmp->ids) == NULL)
            impossible = TRUE;
         sap_tmp = sap_tmp->next;
      }
      sip->next = sip_next;
      if (!impossible) /* found one that matched a row in every alignment */
         found = TRUE;
      else
         sip = sip_next;
   }
   if (!found) /* didn't find a seqid that was contained in all alignments */
      return FALSE;
   /* mark the shared row to make things easier */
   sharedsip = SeqIdDup(sip);
   sap_tmp = (SeqAlignPtr)(sap->segs);
   i = 0;
   while (sap_tmp != NULL)
   {
      saip = (SAIndex2Ptr)(sap_tmp->saip);
      dsp_tmp = (DenseSegPtr)(sap_tmp->segs);
      if (SeqIdComp(sharedsip, dsp_tmp->ids) == SIC_YES)
         saip->tmp = 1;
      else
         saip->tmp = 2;
      sap_tmp = sap_tmp->next;
      i++;
   }
   saparray = (SeqAlignPtr PNTR)MemNew(i*sizeof(SeqAlignPtr));
   sap_tmp = (SeqAlignPtr)(sap->segs);
   i = 0;
   while (sap_tmp != NULL)
   {
      saparray[i] = sap_tmp;
      i++;
      sap_tmp = sap_tmp->next;
   }
   numsaps = i;
   HeapSort(saparray, i, sizeof(SeqAlignPtr), AlnMgr2CompareByAnchor);
   /* now each clump of alignments is a row -- need to eliminate overlaps next */
   sip = NULL;
   i = 0;
   sap_head = sap_prev = NULL;
   while (i<numsaps)
   {
      saparray[i]->next = NULL;
      set_head = set_prev = saparray[i];
      saip = (SAIndex2Ptr)(saparray[i]->saip);
      sip = AlnMgr2GetNthSeqIdPtr(saparray[i], 3-saip->tmp); /* get other seqid */
      i++;
      if (i<numsaps)
         sip_tmp = AlnMgr2GetNthSeqIdPtr(saparray[i], 3-saip->tmp);
      while (i<numsaps && SeqIdComp(sip, sip_tmp) == SIC_YES)
      {
         set_prev->next = saparray[i];
         set_prev = saparray[i];
         saparray[i]->next = NULL;
         i++;
         SeqIdFree(sip_tmp);
         if (i<numsaps)
            sip_tmp = AlnMgr2GetNthSeqIdPtr(saparray[i], 3-saip->tmp);
      }
      AlnMgr2IndexLite(set_head);
      if (!truncate)
         AlnMgr2RemoveInconsistentAlnsFromSet(set_head, 0);
      else
         AlnMgr2RemoveInconsistentAlnsFromSet(set_head, -1);
      sap_tmp = (SeqAlignPtr)(set_head->segs);
      while (sap_tmp != NULL)
      {
         saip = (SAIndex2Ptr)(sap_tmp->saip);
         dsp_tmp = (DenseSegPtr)(sap_tmp->segs);
         if (SeqIdComp(sharedsip, dsp_tmp->ids) == SIC_YES)
            saip->tmp = 1;
         else
            saip->tmp = 2;
         sap_tmp = sap_tmp->next;
      }
      if (sap_head != NULL)
         sap_prev->next = set_head;
      else
         sap_head = sap_prev = set_head;
      while (sap_prev->next != NULL)
      {
         sap_prev = sap_prev->next;
      }
      sap_prev->next = NULL;
   }
   /* now we have lots of freed pointers sitting in the array */
   MemFree(saparray);
   saparray = NULL;
   /* sap_head is the head of a chain of LITE-indexed alignments, each of which is one row */
   /* first make sure that the shared row is on the requested strand */
   sap_tmp = sap_head;
   if (strand == Seq_strand_both || strand == Seq_strand_unknown || strand == 0)
      strand = Seq_strand_plus;
   while (sap_tmp != NULL)
   {
      salp = (SeqAlignPtr)(sap_tmp->segs);
      saip = (SAIndex2Ptr)(salp->saip);
      /* strand is same for all children */
      if (AlnMgr2GetNthStrand(salp, saip->tmp) != strand)
      {
         SeqAlignListReverseStrand(salp);
         while (salp != NULL)
         {
            saip = (SAIndex2Ptr)salp->saip;
            tmp = saip->tmp;
            SAIndex2Free2(salp->saip);
            salp->saip = NULL;
            AlnMgr2IndexSingleChildSeqAlign(salp);
            saip = (SAIndex2Ptr)salp->saip;
            saip->tmp = tmp;
            salp = salp->next;
         }
      }
      sap_tmp = sap_tmp->next;
   }
   sap_tmp = sap_head;
   sap->segs = NULL;
   AMAlignIndex2Free2(sap->saip);
   sap->saip = (SeqAlignIndexPtr)AMAlignIndex2New();
   amaip = (AMAlignIndex2Ptr)(sap->saip);
   amaip->alnstyle = AM2_FULLINDEX;
   set_head = set_prev = NULL;
   while (sap_tmp != NULL)
   {
      salp = (SeqAlignPtr)(sap_tmp->segs);
      while (salp != NULL)
      {
         AlnMgr2AddInNewPairwiseSA(sap, salp);
         if (set_head != NULL)
         {
            set_prev->next = salp;
            set_prev = salp;
         } else
            set_head = set_prev = salp;
         salp = salp->next;
      }
      sap_tmp->segs = NULL;
      sap_tmp = sap_tmp->next;
   }
   AlnMgr2CondenseColumns((DenseSegPtr)(amaip->sharedaln->segs));
   AlnMgr2IndexSingleChildSeqAlign(amaip->sharedaln); 
   set_prev->next = NULL;
   sap->segs = (Pointer)(set_head);
   SeqAlignListFree(sap_head);
   SeqIdFree(sharedsip);
   return TRUE;
}

/* SECTION 2c */
/***************************************************************************
*
*  AlnMgr2IndexIndexedChain takes a linked list of indexed seqaligns
*  and does an in-place transformation to an indexed parent-child 
*  seqalign set.
*
***************************************************************************/
NLM_EXTERN void AlnMgr2IndexIndexedChain(SeqAlignPtr sap)
{
   AMAlignIndex2Ptr  amaip;
   AMIntervalSetPtr  amint;
   AMIntervalSetPtr  amint_head;
   AMEdgePtr         edge_head;
   Int4              numvertices;
   AMVertexPtr       vertex_head;
   AMVertexPtr       PNTR vertexarray;

   if (sap == NULL || sap->saip == NULL || sap->saip->indextype != INDEX_CHILD)
      return;
   AlnMgr2IndexLite(sap);
   AlnMgr2DecomposeToPairwise(sap);
   amaip = (AMAlignIndex2Ptr)(sap->saip);
   amaip->alnstyle = AM2_FULLINDEX;
   AlnMgr2HidePairwiseConflicts(sap);
   amint_head = AlnMgr2MakeIntervals(sap);
   vertex_head = NULL;
   edge_head = NULL;
   vertexarray = AlnMgr2MakeVerticesFromIntervals(sap, amint_head, &vertex_head, &edge_head, &numvertices);
   while (amint_head != NULL)
   {
      amint = amint_head->next;
      AMIntervalSetFree(amint_head);
      amint_head = amint;
   }
   AlnMgr2UsePrimsAlgorithm(vertexarray, numvertices, edge_head);
   AlnMgr2BuildAlignmentFromTree(vertexarray, numvertices, edge_head, sap);
   MemFree(vertexarray);
}

/* SECTION 2c */
/***************************************************************************
*
*  AlnMgr2DecomposeToPairwise takes a parent seqalign and goes through all
*  its children, checking their dimensions. If a child seqalign is found
*  with dimension greater than 2, that alignment is copied into a set of
*  two-row alignments, each new alignment containing the first row of the
*  original alignment and a different row. This function does NOT take out
*  segs with only gaps (is this a problem????). The resulting seqaligns
*  are all individually indexed and then the whole set is indexed lite.
*
***************************************************************************/
static void AlnMgr2DecomposeToPairwise(SeqAlignPtr sap)
{
   DenseSegPtr  dsp;
   DenseSegPtr  dsp_orig;
   Int4         i;
   Int4         j;
   Int4         n;
   SAIndex2Ptr   saip;
   SAIndex2Ptr   saip_orig;
   SeqAlignPtr  salp;
   SeqAlignPtr  salp_new;
   SeqAlignPtr  salp_next;
   SeqAlignPtr  salp_prev;

   if (sap == NULL || sap->saip == NULL || sap->saip->indextype != INDEX_PARENT)
      return;
   salp = (SeqAlignPtr)(sap->segs);
   salp_prev = NULL;
   while (salp)
   {
      n = AlnMgr2GetNumRows(salp);
      if (n > 2)
      {
         salp_next = salp->next;
         saip_orig = (SAIndex2Ptr)(salp->saip);
         for (i=2; i<=n; i++)
         {
            salp_new = SeqAlignNew();
            dsp_orig = (DenseSegPtr)(salp->segs);
            dsp = DenseSegNew();
            dsp->dim = 2;
            dsp->numseg = dsp_orig->numseg;
            dsp->ids = AlnMgr2GetNthSeqIdPtr(salp, 1);
            dsp->ids->next = AlnMgr2GetNthSeqIdPtr(salp, i);
            dsp->starts = (Int4Ptr)MemNew(dsp->numseg*2*sizeof(Int4));
            dsp->lens = (Int4Ptr)MemNew(dsp->numseg*sizeof(Int4));
            dsp->strands = (Uint1Ptr)MemNew(dsp->numseg*2*sizeof(Uint1));
            for (j=0; j<dsp->numseg; j++)
            {
               dsp->lens[j] = dsp_orig->lens[j];
               dsp->starts[2*j] = dsp_orig->starts[dsp_orig->dim*j];
               dsp->starts[2*j+1] = dsp_orig->starts[dsp_orig->dim*j+i-1];
               dsp->strands[2*j] = dsp_orig->strands[dsp_orig->dim*j];
               dsp->strands[2*j+1] = dsp_orig->strands[dsp_orig->dim*j+i-1];
            }
            salp_new = SeqAlignNew();
            salp_new->dim = 2;
            salp_new->segs = (Pointer)dsp;
            salp_new->segtype = SAS_DENSEG;
            AlnMgr2IndexSingleChildSeqAlign(salp_new);
            saip = (SAIndex2Ptr)(salp_new->saip);
            saip->numinchain = saip_orig->numinchain;
            saip->numsplitaln = i-1;
            if (salp_prev == NULL)
            {
               salp_prev = salp_new;
               sap->segs = (Pointer)salp_new;
            } else
            {
               salp_prev->next = salp_new;
               salp_prev = salp_new;
            }
         }
         salp_prev->next = salp_next;
         salp->next = NULL;
         SeqAlignFree(salp);
         salp = salp_next;
      } else
      {
         salp_prev = salp;
         salp = salp->next;
      }
   }
   AMAlignIndex2Free2(sap->saip);
   sap->saip = NULL;
   AlnMgr2IndexLite(sap);
}

/* SECTION 2c */
/***************************************************************************
*
*  AlnMgr2HidePairwiseConflicts looks through a set of indexed seqaligns
*  to find pairs of alignments that share the same seqids and that provide
*  conflicting information. These pairs are then sent to AlnMgr2TossWorse,
*  which hides the worse alignment by unaligning it. Note that the hidden
*  alignments are not destroyed and are not taken out of the set.
*
***************************************************************************/
static void AlnMgr2HidePairwiseConflicts(SeqAlignPtr sap)
{
   AMAlignIndex2Ptr  amaip;
   Int4             i;
   Boolean          inset;
   Int4             j;
   Boolean          match;
   SeqIdPtr         sip11;
   SeqIdPtr         sip12;
   SeqIdPtr         sip21;
   SeqIdPtr         sip22;
   Int4             start11;
   Int4             start12;
   Int4             start21;
   Int4             start22;
   Int4             stop11;
   Int4             stop12;
   Int4             stop21;
   Int4             stop22;
   Int4Ptr          tossed;

   if (sap == NULL || sap->saip == NULL || sap->saip->indextype != INDEX_PARENT)
      return;
   amaip = (AMAlignIndex2Ptr)(sap->saip);
   AlnMgr2SortBySeqId(sap);
   tossed = (Int4Ptr)MemNew(amaip->numsaps*sizeof(Int4));
   for (i=0; i<amaip->numsaps-1; i++)
   {
      for (j=0; j<amaip->numsaps; j++)
      {
         tossed[j] = 0;
      }
      inset = TRUE;
      for (j=i+1; amaip->aligned[i] && j<amaip->numsaps && inset == TRUE; j++)
      {
         if (tossed[j] == 0 && amaip->aligned[i] && amaip->aligned[j])
         {
            sip11 = AlnMgr2GetNthSeqIdPtr(amaip->saps[i], 1);
            sip12 = AlnMgr2GetNthSeqIdPtr(amaip->saps[i], 2);
            sip21 = AlnMgr2GetNthSeqIdPtr(amaip->saps[j], 1);
            sip22 = AlnMgr2GetNthSeqIdPtr(amaip->saps[j], 2);
            match = FALSE;
            if (SeqIdComp(sip11, sip21) == SIC_YES && SeqIdComp(sip12, sip22) == SIC_YES)
            {
               match = TRUE;
               AlnMgr2GetNthSeqRangeInSA(amaip->saps[i], 1, &start11, &stop11);
               AlnMgr2GetNthSeqRangeInSA(amaip->saps[i], 2, &start12, &stop12);
               AlnMgr2GetNthSeqRangeInSA(amaip->saps[j], 1, &start21, &stop21);
               AlnMgr2GetNthSeqRangeInSA(amaip->saps[j], 2, &start22, &stop22);
            } else if (SeqIdComp(sip11, sip22) == SIC_YES && SeqIdComp(sip12, sip21) == SIC_YES)
            {
               match = TRUE;
               AlnMgr2GetNthSeqRangeInSA(amaip->saps[i], 1, &start11, &stop11);
               AlnMgr2GetNthSeqRangeInSA(amaip->saps[i], 2, &start12, &stop12);
               AlnMgr2GetNthSeqRangeInSA(amaip->saps[j], 2, &start21, &stop21);
               AlnMgr2GetNthSeqRangeInSA(amaip->saps[j], 1, &start22, &stop22);
            } else if (SeqIdComp(sip11, sip21) != SIC_YES && SeqIdComp(sip11, sip22) != SIC_YES)
               inset = FALSE;
            if (match == TRUE)
            {
               if ((start11 < start21 && stop11 > stop21) || (start11 < stop21 && stop11 > stop21) || (start11 > start21 && stop11 < stop21))
               {
                  AlnMgr2TossWorse(sap, i, j);
                  if (amaip->aligned[j] == 0) /* j just got tossed -- put it in the list */
                     tossed[j] = 1;
               }
            }
            SeqIdFree(sip11);
	    SeqIdFree(sip12);
	    SeqIdFree(sip21);
	    SeqIdFree(sip22);
         }
      }
      if (amaip->aligned[i] == 0) /* the query alignment got tossed -- restore */
      {                           /* all the ones that it tossed out */
         for (j=0; j<amaip->numsaps; j++)
         {
            if (tossed[j] == 1)
               amaip->aligned[j] = 1;
         }
      }
   }
   MemFree(tossed);
}

/* SECTION 2c */
static void AlnMgr2SortBySeqId(SeqAlignPtr sap)
{
   AMAlignIndex2Ptr  amaip;
   Int4             i;
   SAIndex2Ptr       saip;

   amaip = (AMAlignIndex2Ptr)(sap->saip);
   for (i=0; i<amaip->numsaps; i++)
   {
      saip = (SAIndex2Ptr)(amaip->saps[i]->saip);
      saip->aligned = amaip->aligned[i];
   }
   HeapSort(amaip->saps, amaip->numsaps, sizeof(amaip->saps), AlnMgr2CompareIds);
   for (i=0; i<amaip->numsaps; i++)
   {
      saip = (SAIndex2Ptr)(amaip->saps[i]->saip);
      amaip->aligned[i] = saip->aligned;
   }
}

/* SECTION 2c */
static int LIBCALLBACK AlnMgr2CompareIds(VoidPtr ptr1, VoidPtr ptr2)
{
   Int4         ret;
   SAIndex2Ptr  saip1;
   SAIndex2Ptr  saip2;
   SeqAlignPtr  sap1;
   SeqAlignPtr  sap2;
   SeqIdPtr     sip1;
   SeqIdPtr     sip2;

   if (ptr1 == NULL || ptr2 == NULL)
      return 0;
   sap1 = *((SeqAlignPtr PNTR) ptr1);
   sap2 = *((SeqAlignPtr PNTR) ptr2);
   sip1 = AlnMgr2GetNthSeqIdPtr(sap1, 1);
   sip2 = AlnMgr2GetNthSeqIdPtr(sap2, 1);
   ret = (AlnMgr2OrderSeqIds(sip1, sip2));
   SeqIdFree(sip1);
   SeqIdFree(sip2);
   if (ret != 0)
      return ret;
   saip1 = (SAIndex2Ptr)(sap1->saip);
   saip2 = (SAIndex2Ptr)(sap2->saip);
   if (saip1->score == 0)
      saip1->score = AlnMgr2ComputeScoreForSeqAlign(sap1);
   if (saip2->score == 0)
      saip2->score = AlnMgr2ComputeScoreForSeqAlign(sap2);
   if (saip1->score > saip2->score)
      return -1;
   if (saip1->score < saip2->score)
      return 1;
   return 0;
}

/* SECTION 2c */
/***************************************************************************
*
*  Given an indexed seqalign set, AlnMgr2TossWorse looks at the indicated
*  pair of seqaligns, gets their scores, and sets the unaligned bit of the
*  seqalign with the worse score.
*
***************************************************************************/
static void AlnMgr2TossWorse(SeqAlignPtr sap, Int4 i, Int4 j)
{
   AMAlignIndex2Ptr  amaip;
   SAIndex2Ptr       saip1;
   SAIndex2Ptr       saip2;
   Int4              score1;
   Int4              score2;

   amaip = (AMAlignIndex2Ptr)(sap->saip);
   saip1 = (SAIndex2Ptr)(amaip->saps[i]->saip);
   saip2 = (SAIndex2Ptr)(amaip->saps[j]->saip);
   if (saip1->score == 0)
      saip1->score = score1 = AlnMgr2ComputeScoreForSeqAlign(amaip->saps[i]);
   else
      score1 = saip1->score;
   if (saip1->score == 0)
      saip2->score = score2 = AlnMgr2ComputeScoreForSeqAlign(amaip->saps[j]);
   else
      score2 = saip2->score;
   if (score1 >= score2)
      amaip->aligned[j] = FALSE;
   else if (score2 > score1)
      amaip->aligned[i] = FALSE;
}

/* SECTION 2c */
/***************************************************************************
*
*  AlnMgr2MakeIntervals takes every row from every seqalign and bins it
*  with other sequences with the same seqid and the same strand.
*
***************************************************************************/
static AMIntervalSetPtr AlnMgr2MakeIntervals(SeqAlignPtr sap)
{
   AMAlignIndex2Ptr   amaip;
   AMIntervalSetPtr  amint;
   AMIntervalSetPtr  amint_head;
   AMIntervalSetPtr  amint_prev;
   Boolean           found;
   Int4              i;
   AMIntervalPtr     intv;
   AMIntervalPtr     int_prev;
   Int4              j;
   Int4              k;
   SeqIdPtr          sip;
   Uint1             strand;

   if (sap == NULL || sap->saip == NULL || sap->saip->indextype != INDEX_PARENT)
      return NULL;
   amaip = (AMAlignIndex2Ptr)(sap->saip);
   amint_head = amint_prev = NULL;
   for (i=0; i<amaip->numsaps; i++)
   {
      if (amaip->aligned[i])
      {
         j = AlnMgr2GetNumRows(amaip->saps[i]);
         for (k=0; k<j; k++)
         {
            intv = (AMIntervalPtr)MemNew(sizeof(AMInterval));
            AlnMgr2GetNthSeqRangeInSA(amaip->saps[i], k+1, &(intv->from), &(intv->to));
            sip = AlnMgr2GetNthSeqIdPtr(amaip->saps[i], k+1);
            strand = AlnMgr2GetNthStrand(amaip->saps[i], k+1);
            if (strand != Seq_strand_minus)
               strand = Seq_strand_plus; /* to avoid dealing with Seq_strand_unknown */
            intv->strand = strand;
            if (amint_head != NULL) /* figure out which interval set this goes in */
            {
               amint = amint_head;
               found = FALSE;
               while (amint != NULL && !found)
               {
                  if (SeqIdComp(sip, amint->sip) == SIC_YES && strand == amint->strand)
                     found = TRUE;
                  else
                     amint = amint->next;
               }
               if (found) /* add this to the interval set matched */
               {
                  int_prev = amint->int_head;
                  while (int_prev->next != NULL)
                  {
                     int_prev = int_prev->next;
                  }
                  int_prev->next = intv;
               } else /* make a new interval set */
               {
                  amint = (AMIntervalSetPtr)MemNew(sizeof(AMIntervalSet));
                  amint->sip = SeqIdDup(sip);
                  amint->strand = strand;
                  amint->int_head = intv;
                  amint_prev = amint_head;
                  while (amint_prev->next != NULL)
                  {
                     amint_prev = amint_prev->next;
                  }
                  amint_prev->next = amint;
               }
            } else  /* make a new interval set */
            {
               amint = (AMIntervalSetPtr)MemNew(sizeof(AMIntervalSet));
               amint->sip = SeqIdDup(sip);
               amint->strand = strand;
               amint->int_head = intv;
               amint_head = amint;
            }
            SeqIdFree(sip);
         }
      }
   }
   return amint_head;
}

/* SECTION 2c */
/***************************************************************************
*
*  AlnMgr2MakeVerticesFromIntervals takes the set of intervals created from
*  the alignments, and makes nonoverlapping vertices. Each vertex is a
*  single seqid plus a start and stop (so one seqid may have more than one
*  vertex). Each vertex is also associated with edges, or alignments, which
*  link the vertices together. An edge is simply two vertices plus a weight,
*  which is the alignment quality score. This function creates the vertices,
*  then creates the edges, and sorts the edges and vertices by quality and
*  by number of edges per vertex.
*
***************************************************************************/
static AMVertexPtr PNTR  AlnMgr2MakeVerticesFromIntervals(SeqAlignPtr sap, AMIntervalSetPtr amint_head, AMVertexPtr PNTR vertexhead, AMEdgePtr PNTR edgehead, Int4Ptr numvertices)
{
   AMAlignIndex2Ptr   amaip;
   AMIntervalSetPtr  amint;
   AMEdgePtr         edge;
   AMEdgePtr         edge_head;
   AMEdgePtr         edge_prev;
   Int4              i;
   AMIntervalPtr     intv;
   Int4              j;
   Int4              k;
   Int4              n;
   SAIndex2Ptr       saip;
   SeqIdPtr          sip1;
   SeqIdPtr          sip2;
   Int4              start;
   Int4              stop;
   Int4              v1;
   Int4              v2;
   AMVertexPtr       vertex;
   AMVertexPtr       vertex_head;
   AMVertexPtr       vertex_prev;
   AMVertexPtr       PNTR vertexarray;

   if (sap == NULL || sap->saip == NULL || sap->saip->indextype != INDEX_PARENT)
      return NULL;
   amint = amint_head;
   vertex_head = vertex_prev = NULL;
   while (amint != NULL)
   {
      AlnMgr2SortIntervals(amint);
      vertex = (AMVertexPtr)MemNew(sizeof(AMVertex));
      intv = amint->int_head;
      vertex->sip = SeqIdDup(amint->sip);
      vertex->strand = amint->strand;
      vertex->from = intv->from;
      vertex->to = intv->to;
      intv = intv->next;
      while (intv != NULL)
      {
         if ((intv->from <= vertex->to && intv->from >= vertex->from) || (intv->to <= vertex->to && intv->to >= vertex->from))
         {
            if (intv->from < vertex->from)
               vertex->from = intv->from;
            if (intv->to > vertex->to)
               vertex->to = intv->to;
         } else
         {
            if (vertex_head != NULL)
            {
               vertex_prev->next = vertex;
               vertex_prev = vertex;
            } else
               vertex_head = vertex_prev = vertex;
            vertex = (AMVertexPtr)MemNew(sizeof(AMVertex));
            vertex->from = intv->from;
            vertex->to = intv->to;
            vertex->sip = SeqIdDup(amint->sip);
            vertex->strand = amint->strand;
         }
         intv = intv->next;
      }
      if (vertex_head != NULL)
      {
         vertex_prev->next = vertex;
         vertex_prev = vertex;
      } else
         vertex_head = vertex_prev = vertex;
      amint = amint->next;
   }
   vertex = vertex_head;
   i = 0;
   while (vertex != NULL)
   {
      i++;
      vertex = vertex->next;
   }
   vertexarray = (AMVertexPtr PNTR)MemNew(i*sizeof(AMVertexPtr));
   *numvertices = i;
   vertex = vertex_head;
   i = 0;
   while (vertex != NULL)
   {
      vertexarray[i] = vertex;
      vertex = vertex->next;
      i++;
   }
   amaip = (AMAlignIndex2Ptr)(sap->saip);
   /* now make the edges from the alignments */
   edge_head = NULL;
   for (i=0; i<amaip->numsaps; i++)
   {
      if (amaip->aligned[i])
      {
         j = AlnMgr2GetNumRows(amaip->saps[i]);
         for (k=0; k<j; k++)
         {
            sip1 = AlnMgr2GetNthSeqIdPtr(amaip->saps[i], k+1);
            AlnMgr2GetNthSeqRangeInSA(amaip->saps[i], k+1, &start, &stop);
            v1 = AlnMgr2MatchToVertex(sip1, start, stop, vertexarray, *numvertices);
            for (n=k+1; n<j; n++)
            {
               vertexarray[v1]->numedges++;
               sip2 = AlnMgr2GetNthSeqIdPtr(amaip->saps[i], n+1);
               AlnMgr2GetNthSeqRangeInSA(amaip->saps[i], n+1, &start, &stop);
               v2 = AlnMgr2MatchToVertex(sip2, start, stop, vertexarray, *numvertices);
               vertexarray[v2]->numedges++;
               edge = (AMEdgePtr)MemNew(sizeof(AMEdge));
               edge->vertex1 = v1;
               edge->vertex2 = v2;
               saip = NULL;
               if (amaip->saps[i]->saip != NULL)
                  saip = (SAIndex2Ptr)(amaip->saps[i]->saip);
               if (saip != NULL && saip->score != 0)
                  edge->weight = saip->score;
               else
                  edge->weight = AlnMgr2ComputeScoreForSeqAlign(amaip->saps[i]);
               edge->sap = amaip->saps[i];
               edge->used = 0;
               if (edge_head != NULL)
               {
                  edge_prev->next = edge;
                  edge_prev = edge;
               } else
                  edge_head = edge_prev = edge;
               SeqIdFree(sip2);
            }
            SeqIdFree(sip1);
         }
      }
   }
   AlnMgr2SortEdgesByWeight(&edge_head);
   *vertexhead = vertexarray[0];
   *edgehead = edge_head;
   return vertexarray;
}

/* SECTION 2C */
/***************************************************************************
*
*  AlnMgr2SortVerticesByNumEdges -- the name says it all -- each vertex is
*  associated with one or more edges and the most populated vertices get
*  put first.
*
***************************************************************************/
static void AlnMgr2SortVerticesByNumEdges(AMVertexPtr PNTR vertexarray, Int4 numvertices)
{
   Int4  i;

   HeapSort(vertexarray, numvertices, sizeof(vertexarray), AlnMgr2CompareVertices);
   for (i=0; i<numvertices-1; i++)
   {
      vertexarray[i]->next = vertexarray[i+1];
   }
   vertexarray[numvertices-1]->next = NULL;
}

/* SECTION 2c */
/***************************************************************************
*
*  AlnMgr2CompareVertices is the HeapSort callback for
*  AlnMgr2SortVerticesByNumEdges.
*
***************************************************************************/
static int LIBCALLBACK AlnMgr2CompareVertices(VoidPtr ptr1, VoidPtr ptr2)
{
   AMVertexPtr  vertex1;
   AMVertexPtr  vertex2;

   if (ptr1 != NULL && ptr2 != NULL)
   {
      vertex1 = *((AMVertexPtr PNTR)ptr1);
      vertex2 = *((AMVertexPtr PNTR)ptr2);
      if (vertex1->numedges > vertex2->numedges)
         return -1;
      else if (vertex1->numedges < vertex2->numedges)
         return 1;
      else
         return 0;
   }
   return 0;
}

/* SECTION 2C */
/***************************************************************************
*
*  AlnMgr2SortEdgesByWeight takes a set of edges (alignments) and sorts
*  them by their preset weights (alignment scores), using AlnMgr2CompareEdges
*  as its HeapSort callback.
*
***************************************************************************/
static void AlnMgr2SortEdgesByWeight(AMEdgePtr PNTR edge_head)
{
   AMEdgePtr  edge;
   AMEdgePtr  PNTR edgearray;
   Int4       i;
   Int4       j;

   if (edge_head == NULL || *edge_head == NULL)
      return;
   edge = *edge_head;
   i = 0;
   while (edge != NULL)
   {
      i++;
      edge = edge->next;
   }
   edgearray = (AMEdgePtr PNTR)MemNew(i*sizeof(AMEdgePtr));
   edge = *edge_head;
   i = 0;
   while (edge != NULL)
   {
      edgearray[i] = edge;
      edge = edge->next;
      i++;
   }
   HeapSort(edgearray, i, sizeof(edgearray), AlnMgr2CompareEdges);
   for (j=0; j<i-1; j++)
   {
      edgearray[j]->next = edgearray[j+1];
   }
   edgearray[i-1]->next = NULL;
   *edge_head = edgearray[0];
   MemFree(edgearray);
}

/* SECTION 2c */
/***************************************************************************
*
*  AlnMgr2CompareEdges is the HeapSort callback for AlnMgr2SortEdgesByWeight.
*  It simply compares the preset edge weights.
*
***************************************************************************/
static int LIBCALLBACK AlnMgr2CompareEdges(VoidPtr ptr1, VoidPtr ptr2)
{
   AMEdgePtr  edge1;
   AMEdgePtr  edge2;

   if (ptr1 != NULL && ptr2 != NULL)
   {
      edge1 = *((AMEdgePtr PNTR)ptr1);
      edge2 = *((AMEdgePtr PNTR)ptr2);
      if (edge1->weight > edge2->weight)
         return -1;
      else if (edge1->weight < edge2->weight)
         return 1;
      else
         return 0;
   }
   return 0;
}

/* SECTION 2c */
/***************************************************************************
*
*  AlnMgr2MatchToVertex is called by AlnMgr2MakeVerticesFromIntervals to 
*  figure out which vertex in the array the seqid, start, and stop match to.
*
***************************************************************************/
static Int4 AlnMgr2MatchToVertex(SeqIdPtr sip, Int4 start, Int4 stop, AMVertexPtr PNTR vertexarray, Int4 numvertices)
{
   Int4  i;

   if (sip == NULL || vertexarray == NULL)
      return -1;
   i = 0;
   while (i<numvertices)
   {
      if (SeqIdComp(sip, vertexarray[i]->sip) == SIC_YES)
      {
         if (start >= vertexarray[i]->from && start <= vertexarray[i]->to && stop >= vertexarray[i]->from && stop <= vertexarray[i]->to)
            return i;
      }
      i++;
   }
   return -1;
}

/* SECTION 2c */
/***************************************************************************
*
*  AlnMgr2SortIntervals sorts the AMIntervals by start position within the
*  set, calling AlnMgr2CompareIntervals in a HeapSort.
*
***************************************************************************/
static void AlnMgr2SortIntervals(AMIntervalSetPtr amint)
{
   Int4           i;
   AMIntervalPtr  PNTR intarray;
   AMIntervalPtr  intv;
   AMIntervalPtr  intv_head;
   Int4           j;

   i = 0;
   intv = amint->int_head;
   while (intv != NULL)
   {
      i++;
      intv = intv->next;
   }
   intarray = (AMIntervalPtr PNTR)MemNew(i*sizeof(AMIntervalPtr));
   intv = amint->int_head;
   i = 0;
   while (intv != NULL)
   {
      intarray[i] = intv;
      intv = intv->next;
      i++;
   }
   HeapSort(intarray, i, sizeof(intarray), AlnMgr2CompareIntervals);
   intv_head = intv = intarray[0];
   for (j=1; j<i; j++)
   {
      intv->next = intarray[j];
      intarray[j]->next = NULL;
      intv = intv->next;
   }
   amint->int_head = intv_head;
   MemFree(intarray);
}

/* SECTION 2c */
/***************************************************************************
*
*  AlnMgr2CompareIntervals is the HeapSort callback for
*  AlnMgr2SortIntervals, which sorts a set of AMIntervals by start position.
*
***************************************************************************/
static int LIBCALLBACK AlnMgr2CompareIntervals(VoidPtr ptr1, VoidPtr ptr2)
{
   AMIntervalPtr  intv1;
   AMIntervalPtr  intv2;

   if (ptr1 != NULL && ptr2 != NULL)
   {
      intv1 = *((AMIntervalPtr PNTR)ptr1);
      intv2 = *((AMIntervalPtr PNTR)ptr2);
      if (intv1->from > intv2->from)
         return 1;
      else if (intv1->from < intv2->from)
         return -1;
      else
      {
         if (intv1->to > intv2->to)
            return 1;
         else
            return -1;
      }
   }
   return 0;
}

/* SECTION 2c */
/***************************************************************************
*
*  AlnMgr2UsePrimsAlgorithm takes the set of edges and vertices produced by
*  earlier functions and creates a subset of edges that can be made into
*  a multiple alignment.
*
***************************************************************************/
static void AlnMgr2UsePrimsAlgorithm(AMVertexPtr PNTR vertexarray, Int4 numvertices, AMEdgePtr edge_head)
{
   if (vertexarray == NULL || edge_head == NULL)
      return;
   edge_head->used = AM_USED;
   vertexarray[edge_head->vertex1]->used = TRUE;
   vertexarray[edge_head->vertex2]->used = TRUE;
   AlnMgr2RecursePrims(vertexarray, edge_head);
   AlnMgr2CleanUpLeftovers(vertexarray, numvertices, edge_head);
   return;
}

/* SECTION 2C */
static AMEdgePtr AlnMgr2GetEdgeList(Int4 vertexnum, AMEdgePtr edge_head, AMEdgePtr already_used)
{
   AMEdgePtr  edge;
   AMEdgePtr  list;
   AMEdgePtr  list_head;
   AMEdgePtr  list_prev;

   edge = edge_head;
   list_head = NULL;
   while (edge != NULL)
   {
      if ((edge->vertex1 == vertexnum || edge->vertex2 == vertexnum) && edge != already_used)
      {
         list = (AMEdgePtr)MemNew(sizeof(AMEdge));
         list->vertex1 = edge->vertex1;
         list->vertex2 = edge->vertex2;
         list->weight = edge->weight;
         list->used = edge->used;
         if (list_head != NULL)
         {
            list_prev->next = list;
            list_prev = list;
         } else
            list_head = list_prev = list;
      }
      edge = edge->next;
   }
   return list_head;
}

/* SECTION 2C */
/***************************************************************************
*
*  AlnMgr2GetBetterVertex returns the vertex of the edge indicated that
*  is shared by the largest number of other edges.
*
***************************************************************************/
static AMVertexPtr AlnMgr2GetBetterVertex(AMVertexPtr PNTR vertexarray, AMEdgePtr edge)
{
   if (vertexarray[edge->vertex1]->numedges >= vertexarray[edge->vertex2]->numedges)
      return vertexarray[edge->vertex1];
   else
      return vertexarray[edge->vertex2];
}

/* SECTION 2C */
/***************************************************************************
*
*  AlnMgr2RecursePrims is a simple yet powerful algorithm that builds a
*  minimal spanning tree of the edges and vertexes by starting with a set
*  of edges and vertices, picking the best/shortest edge, then picking
*  other edges one by one that join a vertex in the set with a vertex not
*  in the set, until all edges are used (or deemed impossible).
*
***************************************************************************/
static void AlnMgr2RecursePrims(AMVertexPtr PNTR vertexarray, AMEdgePtr edge_head)
{
   AMEdgePtr  edge;
   Boolean    found;

   edge = edge_head;
   found = FALSE;
   /* find an edge that isn't used, that joins a vertex in the set */
   /* with a vertex outside the set, and add it and the new vertex */
   while (edge != NULL && !found)
   {
      if (edge->used == AM_NOTUSED)
      {
         if (vertexarray[edge->vertex1]->used != vertexarray[edge->vertex2]->used)
         {
            found = TRUE;
            vertexarray[edge->vertex1]->used = TRUE;
            vertexarray[edge->vertex2]->used = TRUE;
            edge->used = AM_USED;
            AlnMgr2RecursePrims(vertexarray, edge_head);
         }
      }
      edge = edge->next;
   }
}

/* SECTION 2C */
/***************************************************************************
*
*  AlnMgr2CleanUpLeftovers takes the edges that are unused after
*  AlnMgr2RecursePrims and looks for edges that duplicate another edge in
*  the set or edges that share a seqid (but not a vertex) with another edge 
*  already in the set. It adds these edges to the set; they don't belong
*  there in tree-based terms but as alignments they are related.
*
***************************************************************************/
static void AlnMgr2CleanUpLeftovers(AMVertexPtr PNTR vertexarray, Int4 numvertices, AMEdgePtr edge_head)
{
   AMEdgePtr  edge;
   AMEdgePtr  edge_tmp;
   Boolean    found;
   Int4       i;
   BoolPtr    tmpverts;

   tmpverts = (BoolPtr)MemNew(numvertices*sizeof(Boolean));
   for (i=0; i<numvertices; i++)
   {
      tmpverts[i] = vertexarray[i]->used;
   }
   edge = edge_head;
   while (edge != NULL)
   {
      if (edge->used == AM_NOTUSED)
      {
         if (tmpverts[edge->vertex1] == TRUE && tmpverts[edge->vertex2] == TRUE)
         {
            /* see if this edge duplicates another edge; if so, add it */
            edge_tmp = edge_head;
            found = FALSE;
            while (edge_tmp != NULL && !found)
            {
               if ((edge->vertex1 == edge_tmp->vertex1 && edge->vertex2 == edge_tmp->vertex2) || (edge->vertex1 == edge_tmp->vertex2 && edge->vertex2 == edge_tmp->vertex1))
               {
                  found = TRUE;
                  edge->used = AM_USED;
               }
               edge_tmp = edge_tmp->next;
            }
            if (!found)
               edge->used = AM_CONFLICT;
         } else if (tmpverts[edge->vertex1] == FALSE && tmpverts[edge->vertex2] == FALSE)
         {
            /* if one of the vertices shares a seqid with a vertex in the set, put both vertices */
            /* and the edge in the set. */
            found = FALSE;
            for (i=0; i<numvertices && !found; i++)
            {
               if (tmpverts[i] == TRUE && (SeqIdComp(vertexarray[i]->sip, vertexarray[edge->vertex1]->sip) == SIC_YES || SeqIdComp(vertexarray[i]->sip, vertexarray[edge->vertex2]->sip) == SIC_YES))
               {
                  found = TRUE;
                  vertexarray[edge->vertex1]->used = TRUE;
                  vertexarray[edge->vertex2]->used = TRUE;
                  edge->used = AM_USED;
               }
            }
            if (!found)
               edge->used = AM_CONFLICT;
         }
      }
      edge = edge->next;
   }
   MemFree(tmpverts);
}

/* SECTION 2C */
/***************************************************************************
*
*  AlnMgr2SameSeq decides whether two vertices come from the same
*  sequence (simple seqid compare).
*
***************************************************************************/
static Boolean AlnMgr2SameSeq(AMVertexPtr vertex1, AMVertexPtr vertex2)
{
   if (vertex1 == NULL || vertex2 == NULL)
      return FALSE;
   if (SeqIdComp(vertex1->sip, vertex2->sip) == SIC_YES)
      return TRUE;
   else
      return FALSE;
}


/* SECTION 2C */
/***************************************************************************
*
*  AlnMgr2BuildAlignmentFromTree performs a breadth-first traversal of
*  the tree, adding edges to the growing alignment as it goes.
*
***************************************************************************/
static void AlnMgr2BuildAlignmentFromTree(AMVertexPtr PNTR vertexarray, Int4 numvertices, AMEdgePtr edge_head, SeqAlignPtr sap)
{
   AMAlignIndex2Ptr  amaip;
   AMVertexPtr      adj;
   AMVertexPtr      adj_head;
   AMEdgePtr        edge;
   Int4             i;
   Int4             j;
   AMQueuePtr       q;
   AMQueuePtr       q_head;
   AMQueuePtr       q_prev;

   amaip = (AMAlignIndex2Ptr)(sap->saip);
   AlnMgr2AddInNewPairwiseSA(sap, edge_head->sap);
   edge_head->aligned = TRUE;
   q_head = (AMQueuePtr)MemNew(sizeof(AMQueue));
   q_head->vertex = AlnMgr2GetBetterVertex(vertexarray, edge_head);
   q_head->vertex->visited = TRUE;
   /* unlink the vertices */
   for (i=0; i<numvertices; i++)
   {
      vertexarray[i]->next = NULL;
   }
   while (q_head != NULL)
   {
      q_prev = q_head;
      while (q_prev->next != NULL)
      {
         q_prev = q_prev->next;
      }
      adj_head = AlnMgr2GetAdjacentVertices(q_head->vertex, vertexarray, edge_head);
      adj = adj_head;
      while (adj != NULL)
      {
         if (adj->visited == FALSE)
         {
            edge = edge_head;
            while (edge != NULL)
            {
               /* if the edge is used in the tree but not yet aligned, and it's adjacent, align it */
               if (edge->aligned == FALSE && edge->used == AM_USED && ((AlnMgr2SameSeq(vertexarray[edge->vertex1], q_head->vertex) && AlnMgr2SameSeq(vertexarray[edge->vertex2], adj)) || (AlnMgr2SameSeq(vertexarray[edge->vertex1], adj) && AlnMgr2SameSeq(vertexarray[edge->vertex2], q_head->vertex))))
               {
                 AlnMgr2AddInNewPairwiseSA(sap, edge->sap);
                 edge->aligned = TRUE;
               }
               edge = edge->next;
            }
            q = (AMQueuePtr)MemNew(sizeof(AMQueue));
            q->vertex = adj;
            q_prev->next = q;
            q_prev = q;
            adj->visited = TRUE;
         }
         adj = adj->next;
      }
      q = q_head->next;
      MemFree(q_head);
      q_head = q;
      if (q_head == NULL) /* look for discontinuous sets -- those will be left over */
      {
         edge = edge_head;
         while (edge != NULL && q_head == NULL)
         {
            if (edge->aligned == FALSE && (vertexarray[edge->vertex1]->visited == FALSE || vertexarray[edge->vertex2]->visited == FALSE))
            {
               q_head = (AMQueuePtr)MemNew(sizeof(AMQueue));
               q_head->vertex = AlnMgr2GetBetterVertex(vertexarray, edge);
               vertexarray[edge->vertex1]->visited = vertexarray[edge->vertex2]->visited = TRUE;
            }
            edge = edge->next;
         }
      }
   }
/* now the vertices are no longer in a linked list -> put them back together */
   for (j=0; j<i-1; j++)
   {
      vertexarray[j]->next = vertexarray[j+1];
      vertexarray[j+1]->next = NULL;
   }
   AlnMgr2CondenseColumns((DenseSegPtr)(amaip->sharedaln->segs));
   AlnMgr2IndexSingleChildSeqAlign(amaip->sharedaln); 
}

/* SECTION 2c */
/***************************************************************************
*
*  AlnMgr2GetAdjacentVertices returns a linked list of all vertices which
*  are adjacent to the given edge; that is, it returns a list of all
*  vertices which are linked by an edge to either vertex of the given edge.
*
***************************************************************************/
static AMVertexPtr AlnMgr2GetAdjacentVertices(AMVertexPtr vertex, AMVertexPtr PNTR vertexarray, AMEdgePtr edge_head)
{
   AMVertexPtr  adj_head;
   AMVertexPtr  adj_prev;
   AMEdgePtr    edge;

   edge = edge_head;
   adj_head = adj_prev = NULL;
   while (edge != NULL)
   {
      if (AlnMgr2SameSeq(vertexarray[edge->vertex1], vertex))
      {
         if (adj_head == NULL)
            adj_head = adj_prev = vertexarray[edge->vertex2];
         else
         {
            adj_prev->next = vertexarray[edge->vertex2];
            adj_prev = adj_prev->next;
         }
      } else if (AlnMgr2SameSeq(vertexarray[edge->vertex2], vertex))
      {
         if (adj_head == NULL)   
            adj_head = adj_prev = vertexarray[edge->vertex1];
         else
         {
            adj_prev->next = vertexarray[edge->vertex1];
            adj_prev = adj_prev->next;
         }
      }
      if (adj_prev != NULL)
         adj_prev->next = NULL;
      edge = edge->next;
   }
   return adj_head;
}

/* SECTION 2c */

static Boolean AlnMgr2GetFirstRowForSeqId(
  DenseSegPtr dsp,
  SeqIdPtr sip,
  Uint1 strand,
  Int4Ptr row_curr,
  SeqIdPtr PNTR sip_curr)
{
  Boolean found = FALSE;

  while (*sip_curr) {
    (*row_curr)++;
    if (SeqIdComp(sip, *sip_curr) == SIC_YES && 
        strand == dsp->strands[*row_curr]) {
      found = TRUE;
    }
    *sip_curr = (*sip_curr)->next;
    if (found) return TRUE;
  }
  return FALSE;
}


static AMSeqPieceSetPtr AlnMgr2CreateSeqPieceSet(DenseSegPtr dsp, Int4 row)
{
  AMSeqPieceSetPtr s_set = (AMSeqPieceSetPtr)MemNew(sizeof(AMSeqPieceSet));
  AMSeqPiecePtr s = (AMSeqPiecePtr)MemNew(sizeof(AMSeqPiece));
  s->beg = -1;
  s->end = -1;
  s->seg = -1;
  s->pos = row - dsp->dim;
  s->set = s_set;
  s->prev = NULL;
  s->next = NULL;
  s->left = -1;
  s->right = -1;
  s->orig_left = -2;
  s->orig_right = -2;
  s->aligned = FALSE;
  s->alt_dsp = NULL;
  s->alt_seg = -1;
  s->alt_pos = -1;

  s->next = NULL;

  s_set->dsp = dsp;
  s_set->row = row;
  s_set->row2 = -1;
  s_set->alt_row = -1;
  s_set->alt_row2 = -1;
  s_set->head = s;
  s_set->tail = s;
  s_set->max_pos = dsp->dim * dsp->numseg;
  s_set->strand = dsp->strands[row];
  s_set->plus = s_set->strand != Seq_strand_minus;
  s_set->next = NULL;

  return s_set;
}

static AMSeqPiecePtr AlnMgr2GetNextSeqPiece(AMSeqPiecePtr s)
{
  DenseSegPtr dsp;
  Int4 max_pos;
  AMSeqPiecePtr s_new;

  dsp = s->set->dsp;
  max_pos = s->set->max_pos;

  if (s->pos < max_pos) {
    s_new = (AMSeqPiecePtr)MemNew(sizeof(AMSeqPiece));
    s_new->pos = s->pos + dsp->dim;
    s_new->seg = s->seg + 1;
    s_new->set = s->set;
    s_new->prev = s;
    s = s->next = s_new;
    s->set->tail = s;

    s->next = NULL;

    /* initialize the following */
    s->left = -1;
    s->right = -1;
    s->aligned = FALSE;
    s->alt_dsp = NULL;
    s->alt_seg = -1;
    s->alt_pos = -1;
    s->orig_left = -2;
    s->orig_right = -2;

    /* find the beg and end */
    while (s->pos < max_pos) {
      if (dsp->starts[s->pos] != -1) {
        s->beg = s->end = dsp->starts[s->pos];
        if (s->set->plus) {
          s->end += dsp->lens[s->seg] - 1;
        } else {
          s->beg += dsp->lens[s->seg] - 1;
        }
        return s;
      } else {
        s->seg++;
        s->pos += dsp->dim;
      }
    }
    s->beg = -1;
    s->end = -1;
    return s;
  }
  return NULL;
}

static AMSeqPiecePtr AlnMgr2GetNextLimitedSeqPiece(
  AMSeqPiecePtr s,
  AMSeqPiecePtr right) 
{
  DenseSegPtr dsp;
  Int4 new_pos, new_seg, max_pos, max_seg;
  AMSeqPiecePtr s_new;

  AMSeqPiecePtr left = right->prev;

  dsp = s->set->dsp;
  max_pos = s->set->max_pos;
  max_seg = right->seg;
  new_pos = s->pos + dsp->dim;
  new_seg = s->seg + 1;

  while (new_pos < max_pos && new_seg <= max_seg) {
    if (dsp->starts[new_pos] != -1) {
      s_new = (AMSeqPiecePtr)MemNew(sizeof(AMSeqPiece));
      s_new->pos = new_pos;
      s_new->seg = new_seg;
      s_new->set = s->set;
      s_new->next = NULL;
      s_new->prev = s;
      s = s->next = s_new;
      s->set->tail = s;
      s->beg = s->end = dsp->starts[s->pos];
      if (s->set->plus) {
        s->end += dsp->lens[s->seg] - 1;
      } else {
        s->beg += dsp->lens[s->seg] - 1;
      }
      /* aligned to a sequence in anchor or not */
      if (s->seg == right->seg) {
        s->aligned = TRUE;
        s->left = right->beg;
        s->right = right->end;
      } else {
        s->aligned = FALSE;
        s->left = left->end;
        s->right = right->beg;
      }
      /* these are not yet used */
      s->orig_left = -2;
      s->orig_right = -2;
      s->alt_dsp = NULL;
      s->alt_seg = -1;
      s->alt_pos = -1;
      return s;
    }
    new_pos += dsp->dim;
    new_seg++;
  }
  return NULL;
}

static Boolean AlnMgr2AddSeqPiece(
  AMSeqPieceSetPtr set,
  AMSeqPiecePtr what)
{
  AMSeqPiecePtr s;
  DenseSegPtr dsp = set->dsp;
  DenseSegPtr alt_dsp = what->set->dsp;

  s = (AMSeqPiecePtr)MemNew(sizeof(AMSeqPiece));
  s->beg = what->beg;
  s->end = what->end;

  if (alt_dsp == dsp) {
    s->seg = what->seg;
    s->pos = what->pos;
    s->alt_dsp = NULL;
    s->alt_seg = -1;
    s->alt_pos = -1;
  } else {
    s->seg = -1;
    s->pos = -1;
    s->alt_dsp = alt_dsp;
    s->alt_seg = what->seg;
    s->alt_pos = what->pos;
  }        
  s->left = what->left;
  s->right = what->right;
  s->orig_left = what->orig_left;
  s->orig_right = what->orig_right;
  s->aligned = what->aligned;
  s->set = set;
  s->next = NULL;
  if ((s->prev = set->tail) != NULL) {
    s->prev->next = s;
  }
  set->tail = s;
}

static Boolean AlnMgr2InsertSeqPiece(
  AMSeqPiecePtr where,
  AMSeqPiecePtr what,
  Int4 end) 
{
  AMSeqPiecePtr s;
  DenseSegPtr dsp = where->set->dsp;
  DenseSegPtr alt_dsp = what->set->dsp;


  s = (AMSeqPiecePtr)MemNew(sizeof(AMSeqPiece));
  s->beg = what->beg;
  s->end = end;

  if (where->beg == what->beg) {
    s->seg = where->seg;
    s->pos = where->pos;
    where->beg = end + (where->set->plus? 1 : -1);
    if (alt_dsp == dsp) {
      s->alt_dsp = NULL;
      s->alt_seg = -1;
      s->alt_pos = -1;
    } else {
      s->alt_dsp = alt_dsp;
      s->alt_seg = what->seg;
      s->alt_pos = what->pos;
    }
  } else {
    if (alt_dsp == dsp) {
      s->seg = what->seg;
      s->pos = what->pos;
      s->alt_dsp = NULL;
      s->alt_seg = -1;
      s->alt_pos = -1;
    } else {
      s->seg = -1;
      s->pos = -1;
      s->alt_dsp = alt_dsp;
      s->alt_seg = what->seg;
      s->alt_pos = what->pos;
    }        
  }
  s->left = what->left;
  s->right = what->right;
  s->orig_left = what->orig_left;
  s->orig_right = what->orig_right;
  s->aligned = what->aligned;
  s->set = where->set;
  s->next = where;
  if ((s->prev = where->prev) != NULL) {
    if (s->prev) {
      s->prev->next = s;
    } else {
      if (s->set->head == where) {
        s->set->head = s;
      }
    }
    where->prev = s;
  }
}

static void AlnMgr2CopySeg(
  DenseSegPtr DSP,
  Int4 PNTR SEG_ptr,
  Int4 PNTR POS_ptr,
  DenseSegPtr Dsp,
  Int4 PNTR Seg_ptr,
  Int4 PNTR Pos_ptr,
  AMSeqPiecePtr PNTR s_ptr)
{
  Int4 i, rdelta, ldelta, POS, Pos, max_Pos, pos2, alt_pos2, SEG, Seg, 
    beg, end;
  AMSeqPiecePtr s;
  Boolean plus;

  POS = *POS_ptr; Pos = *Pos_ptr;
  SEG = *SEG_ptr; Seg = *Seg_ptr;
  s = *s_ptr;

  if (s->set->row != s->set->row2) { /* if not a B */
    if (!(s->next)) {
      *s_ptr = NULL;
      return; /* skip the last A */
    }
  }

  max_Pos = POS+Dsp->dim;

  DSP->lens[SEG] = ABS(s->end - s->beg) + 1;
  
  if (s->set->dsp != Dsp) { /* the extra row for the non-anchor seq */
    for (i = 0; POS < max_Pos; POS++, i++) {
      DSP->starts[POS] = -1;
      DSP->strands[POS] = Dsp->strands[i];
    }
    DSP->starts[POS] = MIN(s->beg, s->end);
    DSP->strands[POS] = s->set->strand;
    POS++;

  } else { /* not dealing with the extra row itself */

    if (s->pos >= 0 && s->set->row != s->set->row2) { /* Dsp involved */
      beg = end = s->set->dsp->starts[s->pos];
      if (s->set->plus) {
        end += s->set->dsp->lens[s->seg]-1;
      } else {
        beg += s->set->dsp->lens[s->seg]-1;
      }
      if (ldelta = ABS(s->beg - beg)) {
        /* need to "continue" from the orig seg */
        Pos = s->pos - s->set->row;
        Seg = s->seg;
      }
      rdelta = ABS(end - s->end);

      for (; POS < max_Pos; POS++, Pos++) {
        DSP->strands[POS] = Dsp->strands[Pos];
        plus = DSP->strands[POS] != Seq_strand_minus;
        if (Dsp->starts[Pos] != -1) {
          DSP->starts[POS] = Dsp->starts[Pos] + (plus ? ldelta : rdelta);
        } else {
          DSP->starts[POS] = -1;
        }
      }
      if (ldelta) {
        /* restore these */
        Pos = *Pos_ptr;
        Seg = *Seg_ptr;
      } else {
        Seg++;
      }

      if (s->alt_dsp) { /* dsp involved too */
        alt_pos2 = 
          s->alt_pos + s->set->alt_row2 - s->set->alt_row;
        beg = end = s->alt_dsp->starts[s->alt_pos];
        if (s->alt_dsp->strands[s->alt_pos] == Seq_strand_minus) {
          beg += s->alt_dsp->lens[s->alt_seg]-1;
        } else {
          end += s->alt_dsp->lens[s->alt_seg]-1;
        }
        ldelta = ABS(s->beg - beg);
        rdelta = ABS(end - s->end);

        if (s->set->row2 != -1) { /* 2nd row merged*/
          pos2 = POS - DSP->dim + s->set->row2;
        } else { /* extra row */
          pos2 = POS;
          POS++;
        }
        DSP->strands[pos2] = s->alt_dsp->strands[alt_pos2];
        plus = DSP->strands[pos2] != Seq_strand_minus;
        if (s->alt_dsp->starts[alt_pos2] != -1) {
          DSP->starts[pos2] = s->alt_dsp->starts[alt_pos2] + 
            (plus ? ldelta : rdelta);
        } else {
          DSP->starts[pos2] = -1;
        }
      } else { /* dsp not involved */
        if (s->set->row2 == -1) { /* 2nd row not merged */
          DSP->starts[POS] = -1;
          DSP->strands[POS] = 
            s->set->alt_dsp->strands[s->set->alt_row2];
          POS++;
        }
      }
    } else { /* Dsp not involved */
      for (i = 0; POS < max_Pos; POS++, i++) {
        DSP->starts[POS] = -1;
        DSP->strands[POS] = Dsp->strands[i];
      }
      if (s->set->row == s->set->row2) { /* if a B */
        if (!(s->alt_dsp)) {
          Pos += s->set->dsp->dim; /* move to next seg */
          Seg++;
        }
      } else { /* not a B */
        alt_pos2 = 
          s->alt_pos + s->set->alt_row2 - s->set->alt_row;

        beg = end = s->alt_dsp->starts[s->alt_pos];
        if (s->alt_dsp->strands[s->alt_pos] == Seq_strand_minus) {
          beg += s->alt_dsp->lens[s->alt_seg]-1;
        } else {
          end += s->alt_dsp->lens[s->alt_seg]-1;
        }
        ldelta = ABS(s->beg - beg);
        rdelta = ABS(end - s->end);

        if (s->set->row2 != -1) { /* merged row2 */
          pos2 = POS - DSP->dim + s->set->row2;
        } else {
          pos2 = POS;
          POS++;
        }
        DSP->strands[pos2] = s->alt_dsp->strands[alt_pos2];
        plus = DSP->strands[pos2] != Seq_strand_minus;
        if (s->alt_dsp->starts[alt_pos2] != -1) {
          DSP->starts[pos2] = s->alt_dsp->starts[alt_pos2] + 
            (plus ? ldelta : rdelta);
        } else {
          DSP->starts[pos2] = -1;
        }
      }
      DSP->starts[POS + s->set->row - DSP->dim] = MIN(s->beg, s->end);
    }
  }
  (*SEG_ptr)++;
  *Seg_ptr = Seg;
  *s_ptr = (*s_ptr)->next;
  *POS_ptr = POS;
  *Pos_ptr = Pos;
}

NLM_EXTERN void AlnMgr2AddInNewPairwiseSA(SeqAlignPtr parent, SeqAlignPtr sap)
{
  AMAlignIndex2Ptr amaip;
  DenseSegPtr dsp, Dsp, DSP;
  Int4 Seg, SEG;
  Int4 Pos, POS, max_POS;
  Int4 A_end, B_beg;
  Int4 anchor, Anchor;
  Int4 row;
  SeqIdPtr sip, extra_sip;
  AMSeqPieceSetPtr a_set, A_set, b_set, B_set_head, B_set;
  AMSeqPiecePtr a, A, b, B;
  Boolean conflict;
  Boolean a_plus, b_plus;
  Int4 upper_limit;
  Int4 extra_segs;

  dsp = (DenseSegPtr)(sap->segs);
  if (dsp->dim != 2) {
    if (dsp->dim == 0) {
      dsp->dim = 2; /* set to default */
    } else {
      ErrPostEx(SEV_ERROR, 0,0,
                "AlnMgr2AddInNewPairwiseSA: dsp->dim (=%d) should be 2.",
                dsp->dim);
      return;
    }
  }
  if (dsp->numseg < 1) {
    ErrPostEx(SEV_ERROR, 0,0,
              "AlnMgr2AddInNewPairwiseSA: dsp->numseg (=%d) should be > 0.",
              dsp->numseg);
    return;
  }

  amaip = (AMAlignIndex2Ptr)(parent->saip);
  if (amaip->sharedaln == NULL) {/* first alignment to be added */
    SeqAlignPtr salp;
    Int4 i;

    salp = SeqAlignDup(sap);
    AlnMgr2IndexSingleChildSeqAlign(salp);
    amaip->sharedaln = salp;
    amaip->numrows = dsp->dim;
    sip = dsp->ids;
    amaip->ids = (SeqIdPtr PNTR)MemNew((dsp->dim)*sizeof(SeqIdPtr));
    i = 0;
    while (sip != NULL) {
      amaip->ids[i] = SeqIdDup(sip);
      sip = sip->next;
      i++;
    }
    MemFree(amaip->saps);
    amaip->saps = (SeqAlignPtr PNTR)MemNew(sizeof(SeqAlignPtr));
    amaip->saps[0] = sap;
    amaip->numsaps = 1;
    MemFree(amaip->aligned);
    amaip->aligned =  (Boolean PNTR) MemNew(sizeof(Boolean));
    amaip->aligned[0] = TRUE;

    return;
  }

  /* add the new sap */
  amaip->numsaps++;
  amaip->saps = (SeqAlignPtr PNTR) MemMore
    (amaip->saps, amaip->numsaps*sizeof(SeqAlignPtr));
  amaip->saps[amaip->numsaps-1] = sap;
  amaip->aligned = (Boolean PNTR) MemMore
    (amaip->aligned, (amaip->numsaps)*sizeof(Boolean));
  amaip->aligned[amaip->numsaps-1] = TRUE;

  Dsp = (DenseSegPtr)(amaip->sharedaln->segs);

  AlnMgr2GetFirstSharedRow(amaip->sharedaln, sap, &Anchor, &anchor);

  {{ /* make sure the shared rows are on the same strand */
    Uint1 Strand, strand;

    Strand = AlnMgr2GetNthStrand(amaip->sharedaln, Anchor);
    if (Strand == Seq_strand_unknown)
      Strand = Seq_strand_plus;
    strand = AlnMgr2GetNthStrand(sap, anchor);
    if (strand == Seq_strand_unknown)
      strand = Seq_strand_plus;
    if (Strand != strand) {
      SeqAlignListReverseStrand(sap);
      SAIndex2Free2(sap->saip);
      sap->saip = NULL;
      AlnMgr2IndexSingleChildSeqAlign(sap);
      dsp = (DenseSegPtr)(sap->segs);
      strand = AlnMgr2GetNthStrand(sap, anchor);
      if (strand == Seq_strand_unknown)
        strand = Seq_strand_plus;
    }
    a_plus = strand != Seq_strand_minus;
  }}
  anchor--; Anchor--; /* make them 0-based */

  /* create new dsp */
  DSP = DenseSegNew();
  DSP->numseg = Dsp->numseg;
  DSP->dim = Dsp->dim;
/*   DSP->ids = SeqIdDupList(Dsp->ids); */

  /* collect other shared seqids */
  b_set = B_set = B_set_head = NULL;
  row = -1; sip = Dsp->ids;
  extra_sip = dsp->ids;
  if (anchor == 0) {
    extra_sip = extra_sip->next;
  }
  while (AlnMgr2GetFirstRowForSeqId
         (Dsp, extra_sip, dsp->strands[1-anchor], &row, &sip)) {
    if (B_set) {
      B_set->next = AlnMgr2CreateSeqPieceSet(Dsp, row);
      B_set = B_set->next;
    } else {
      B_set = B_set_head = AlnMgr2CreateSeqPieceSet(Dsp, row);
    }
  }
  b_plus = dsp->strands[1-anchor] != Seq_strand_minus;

  /* ids */
  DSP->ids = Dsp->ids;
  Dsp->ids = NULL;

  /* collect a, b */
  a_set = AlnMgr2CreateSeqPieceSet(dsp, anchor);
  a = a_set->head;
  b_set = AlnMgr2CreateSeqPieceSet(dsp, 1-anchor);
  while (a = AlnMgr2GetNextSeqPiece(a)) {
    b = b_set->tail;
    while (b = AlnMgr2GetNextLimitedSeqPiece(b, a)) {
      if (!b->aligned) {
        DSP->numseg++;
      }
    }
  }

  /* collect A, B */
  A_set = AlnMgr2CreateSeqPieceSet(Dsp, Anchor);
  A = A_set->head;
  while (A = AlnMgr2GetNextSeqPiece(A)) {
    B_set = B_set_head;
    while (B_set) {
      B = B_set->tail;
      while (B = AlnMgr2GetNextLimitedSeqPiece(B, A)) {};
      B_set=B_set->next;
    }
  }

  /* resolve a, A */
  A_set->alt_row = a_set->row;
  a = a_set->head->next;
  A = A_set->head->next;
  while (a && A && a->next && A->next) {
    if (a_plus ? a->beg < A->beg : a->beg > A->beg) {
      AlnMgr2InsertSeqPiece
        (A, a, a_plus ? MIN(a->end, A->beg-1) : MAX(a->end, A->beg+1));
      DSP->numseg++;
      if (a_plus ? a->end < A->beg : a->end > A->beg) {
        a = a->next;
      } else {
        a->beg = A->beg;
      }
    } else if (a_plus ? A->beg < a->beg : A->beg > a->beg) {
      if (a_plus ? A->end < a->beg : A->end > a->beg) {
        A = A->next;
      } else {
        AlnMgr2InsertSeqPiece(A, A, a_plus ? a->beg - 1 : a->beg + 1);
        DSP->numseg++;
      }
    } else { /* a->beg == A->beg */
      if (a_plus ? a->end < A->end : a->end > A->end) {
        AlnMgr2InsertSeqPiece(A, a, a->end);
        DSP->numseg++;
        a = a->next;
      } else if (a_plus ? a->end > A->end : a->end < A->end) {
        a->beg = A->end + (a_plus ? 1 : -1);
        A->alt_dsp = a->set->dsp;
        A->alt_seg = a->seg;
        A->alt_pos = a->pos;
        A = A->next;
      } else { /* a->end == A->end */
        A->alt_dsp = a->set->dsp;
        A->alt_seg = a->seg;
        A->alt_pos = a->pos;
        a = a->next;
        A = A->next;
      }
    }
  }
  while (a && a->next) {
    AlnMgr2InsertSeqPiece(A, a, a->end);
    DSP->numseg++;
    a = a->next;
  }

  /* set the upper limits */
  if (B_set_head) {
    if (a_plus) {
      upper_limit = 
        A_set->tail->end = A_set->tail->beg = A_set->tail->prev->end + 1;

      b = b_set->tail;
      while (b && b->right == -1) {
        b->right = upper_limit;
        b = b->prev;
      }
      
      B_set = B_set_head;
      while (B_set) {
        B = B_set->tail;
        while (B && B->right == -1) {
          B->right = upper_limit;
          B = B->prev;
        }
        B_set = B_set->next;
      }
      
    } else {
      upper_limit = 
        A_set->head->beg = A_set->head->end = A_set->head->next->beg + 1;
      
      b = b_set->head;
      while (b && b->left == -1) {
        b->left = upper_limit;
        b = b->next;
      }
      
      B_set = B_set_head;
      while (B_set) {
        B = B_set->head;
        while (B && B->left == -1) {
          B->left = upper_limit;
          B = B->next;
        }
        B_set = B_set->next;
      }
      
    }
  }

  /* try to resolve b, B */
  if (B_set_head) {
    b = b_set->head->next;
    B_set = B_set_head;
    while (B_set) {
      B = B_set->head->next;
      conflict = FALSE;
      extra_segs = 0;
      while (b && B) {
        if (b_plus ? b->beg < B->beg : b->beg > B->beg) {
          if (b_plus ? b->end < B->beg : b->end > B->beg) {
            /* trim the limits */
            if (a_plus ? B->left <= b->left : B->left >= b->left) {
              if (a_plus ? B->right < b->left : B->right > b->left) {
                conflict = TRUE; break;
              } else {
                if (B->aligned) {
                  conflict = TRUE; break; /* no trimming allowed */
                } else {
                  B->left = b->left;
                }
              }
              if (a_plus ? b->right > B->right : b->right < B->right) {
                if (b->aligned) {
                  conflict = TRUE; break; /* no trimming allowed */
                } else {
                  b->orig_right = b->right; /* for recovering */
                  b->right = B->right;
                }
              }
            }
            AlnMgr2InsertSeqPiece(B, b, b->end);
            if (!(b->aligned)) extra_segs++;
            b = b->next;
          } else {
            conflict = TRUE; break;
          } 

        } else if (b_plus ? B->beg < b->beg : B->beg > b->beg) {
          if (b_plus ? B->end < b->beg : B->end > b->beg) {
            /* trim the limits */
            if (a_plus ? b->left < B->left : b->left > B->left) {
              if (a_plus ? b->right < B->left : b->right > B->left) {
                conflict = TRUE; break;
              } else {
                if (b->aligned) {
                  conflict = TRUE; break; /* no trimming allowed */
                } else {
                  b->orig_left = b->left; /* for recovering */
                  b->left = B->left;
                }
              }
              if (a_plus ? B->right > b->right : B->right < b->right) {
                if (B->aligned) {
                  conflict = TRUE; break; /* no trimming allowed */
                } else {
                  B->right = b->right;
                }
              }
            }

            B = B->next;

          } else {
            conflict = TRUE; break;
          }
        } else { /* B->beg == b->beg */
          conflict = TRUE; break;
        }
      }
      if (!conflict) {
        while (b) {
          AlnMgr2AddSeqPiece(B_set, b);
          if (!(b->aligned)) extra_segs++;
          b = b->next;
        }
/*         DSP->numseg += extra_segs; */
        break;
      }
      /* conflict, roll back b, recovering limits, try next B */
      if (!b) {
        b = b_set->tail;
      }
      while (b) {
        if (b->orig_left != -2) {
          b->left = b->orig_left;
        }
        if (b->orig_right != -2) {
          b->right = b->orig_right;
        }
        b = b->prev;
      }
      b = b_set->head->next;
      B_set = B_set->next;
    }
  }
  if (B_set) {  /* B_set has no conflict with b_set */
    B = B_set->head->next;
    B_set->row2 = B_set->row; /* mark the set */
    A_set->row2 = B_set->row;
    A_set->alt_row2 = b_set->row; 
  } else {  /* this mean extra row */
    A_set->row2 = -1;
    A_set->alt_row2 = b_set->row;
    A_set->alt_dsp = b_set->dsp;
    DSP->dim++;
    sip = DSP->ids;
    while (sip->next) {
      sip = sip->next;
    }
    AddSeqId(&sip, extra_sip);

    /* fix the index too */
    amaip->numrows = DSP->dim;
    amaip->ids = (SeqIdPtr PNTR)MemMore
      (amaip->ids,amaip->numrows*sizeof(SeqIdPtr));
    amaip->ids[amaip->numrows-1] = SeqIdDup(extra_sip);

    b_set->row2 = b_set->row; /* mark the set */
    B = b_set->head->next;
    B_beg = -1; /* nothing to comp Bs to */
  }
  
  /* allocate memory for the new sharedaln matrix */
  DSP->starts = (Int4Ptr)MemNew(DSP->numseg * DSP->dim * sizeof(Int4));
  DSP->strands = (Uint1Ptr)MemNew(DSP->numseg * DSP->dim * sizeof(Uint1));
  DSP->lens = (Int4Ptr)MemNew(DSP->numseg * sizeof(Int4));

  /* loop through segments */
  POS = 0; Pos = 0; Seg = 0; SEG = 0;
  A = A_set->head->next;
  while (Seg < Dsp->numseg) {

    A_end = Dsp->starts[Pos+A_set->row];
    if (a_plus && A_end >= 0) {
      A_end += Dsp->lens[Seg] - 1;
    }
    if (B_set) {
      B_beg = Dsp->starts[Pos+B_set->row];
    }
    
    if (A_end >= 0) {
      while (A && (a_plus ? A->end <= A_end : A->end >= A_end)) {
        while (B && (a_plus ? B->left < A->beg : B->left > A->beg)) {
          if (B->aligned) {
            B = B->next;
            break; /* the aligned piece should be last */
          } else {
            AlnMgr2CopySeg(DSP, &SEG, &POS, Dsp, &Seg, &Pos, &B);
          }            
        }
        if (B && B->aligned && B->left == A->beg) {
          B = B->next;
        }
        AlnMgr2CopySeg(DSP, &SEG, &POS, Dsp, &Seg, &Pos, &A);
      }
    } else if (B && B_beg >= 0) {
      while (B && (b_plus ? B->beg <= B_beg : B->beg >= B_beg)) {
        while (A && (a_plus ? A->beg <= B->left : A->beg >= B->left)) {
          AlnMgr2CopySeg(DSP, &SEG, &POS, Dsp, &Seg, &Pos, &A);
        }
        if (B->aligned) {
          B = B->next;
        } else {
          AlnMgr2CopySeg(DSP, &SEG, &POS, Dsp, &Seg, &Pos, &B);
        }            
      }
    } else {
      /* just copy the Dsp segment */
      DSP->lens[SEG] = Dsp->lens[Seg];
      max_POS = POS + Dsp->dim;
      for (; POS < max_POS; POS++, Pos++) {
        DSP->starts[POS] = Dsp->starts[Pos];
        DSP->strands[POS] = Dsp->strands[Pos];
      }
      if (DSP->dim > Dsp->dim) {
        DSP->starts[POS] = -1;
        DSP->strands[POS] = dsp->strands[1-anchor];
        POS++;
      }
      SEG++;
      Seg++;
    }
  }
  while (A) {
    while (B && (a_plus ? B->right <= A->beg : B->right >= A->beg)) {
      if (B->aligned) {
        B = B->next;
      } else {
        AlnMgr2CopySeg(DSP, &SEG, &POS, Dsp, &Seg, &Pos, &B);
      }            
    }
    AlnMgr2CopySeg(DSP, &SEG, &POS, Dsp, &Seg, &Pos, &A);
  }
  while (B) {
    if (B->aligned) {
      B = B->next;
    } else {
      AlnMgr2CopySeg(DSP, &SEG, &POS, Dsp, &Seg, &Pos, &B);
    }            
  }

  /* Done */
  AMSeqPieceSetFree(A_set);
  AMSeqPieceSetFree(a_set);
  AMSeqPieceSetFree(B_set_head);
  AMSeqPieceSetFree(b_set);

  amaip->sharedaln->segs = DSP;
  /* update the dim for the shared_aln to match the new DensegPtr */
  amaip->sharedaln->dim = DSP->dim;
  
  DenseSegFree(Dsp);
}

/***************************************************************************
*
*  AlnMgr2AddInNewSA adds a seqalign to an existing seqalign. The new
*  seqalign must share at least one row with the existing seqalign. The
*  new, combined dense-seg structure is computed, and then it is condensed
*  using AlnMgr2CondenseRows to make sure that there are no superfluous rows.
*
***************************************************************************/
static void AlnMgr2AddInNewSA(SeqAlignPtr parent, SeqAlignPtr sap)
{
   AMAlignIndex2Ptr  amaip;
   AM_Small2Ptr      asp;
   AM_Small2Ptr      asp_head;
   AM_Small2Ptr      asp_prev;
   AM_Small2Ptr      asp_tmp;
   AM_Small2Ptr      asp_tmp2;
   AM_Small2Ptr      PNTR asparray;
   Int4             currstop;
   DenseSegPtr      dsp;
   DenseSegPtr      dsp_new;
   DenseSegPtr      dsp_shared;
   Boolean          found;
   Int4             i;
   Int4             j;
   Int4             k;
   Int4             n1;
   Int4             n2;
   Int4             numrows;
   Int4             offset;
   SeqAlignPtr      salp;
   SeqAlignPtr      sap_new;
   SeqAlignPtr      PNTR saptmp;
   SeqIdPtr         sip;
   SeqIdPtr         sip_head;
   SeqIdPtr         sip_tmp;
   Int4             state;
   Int4             stop1;
   Int4             stop2;
   Uint1            strand1;
   Uint1            strand2;

   amaip = (AMAlignIndex2Ptr)(parent->saip);
   if (amaip->sharedaln == NULL) /* this is the first alignment to be added */
   {
      salp = SeqAlignDup(sap);
      AlnMgr2IndexSingleChildSeqAlign(salp);
      dsp = (DenseSegPtr)(salp->segs);
      amaip->sharedaln = salp;
      amaip->numrows = dsp->dim;
      sip = dsp->ids;
      amaip->ids = (SeqIdPtr PNTR)MemNew((dsp->dim)*sizeof(SeqIdPtr));
      i = 0;
      while (sip != NULL)
      {
         amaip->ids[i] = SeqIdDup(sip);
         sip = sip->next;
         i++;
      }
      MemFree(amaip->saps);
      amaip->saps = (SeqAlignPtr PNTR)MemNew(sizeof(SeqAlignPtr));
      amaip->saps[0] = sap;
      amaip->numsaps = 1;
   } else
   {
      /* free ids */
      for (i=0; i<amaip->numrows; i++)
      {
         SeqIdFree(amaip->ids[i]);
      }
      MemFree(amaip->ids);
 
      /* add the new sap */
      saptmp = amaip->saps;
      amaip->saps = (SeqAlignPtr PNTR)MemNew((amaip->numsaps+1)*sizeof(SeqAlignPtr));
      for (i=0; i<amaip->numsaps; i++)
      {
         amaip->saps[i] = saptmp[i];
      }
      amaip->saps[amaip->numsaps] = sap;
      MemFree(saptmp);
      amaip->numsaps++;

      /* dsp, dsp_shared, n1, n2 */
      dsp = (DenseSegPtr)(sap->segs);
      dsp_shared = (DenseSegPtr)(amaip->sharedaln->segs);
      AlnMgr2GetFirstSharedRow(amaip->sharedaln, sap, &n1, &n2);
      if (n1 == n2 && n1 == 0)
         return;

      /* make sure the shared rows are on the same strand */
      strand1 = AlnMgr2GetNthStrand(amaip->sharedaln, n1);
      if (strand1 == Seq_strand_unknown)
         strand1 = Seq_strand_plus;
      strand2 = AlnMgr2GetNthStrand(sap, n2);
      if (strand2 == Seq_strand_unknown)
         strand2 = Seq_strand_plus;
      if (strand1 != strand2)
      {
         SeqAlignListReverseStrand(sap);
         SAIndex2Free2(sap->saip);
         sap->saip = NULL;
         AlnMgr2IndexSingleChildSeqAlign(sap);
         dsp = (DenseSegPtr)(sap->segs);
         strand2 = AlnMgr2GetNthStrand(sap, n2);
         if (strand2 == Seq_strand_unknown)
            strand2 = Seq_strand_plus;
      }

      /* numrows */
      numrows = dsp->dim + dsp_shared->dim - 1; /* for now this works; compress at the end */
      asp_head = NULL;

      /* currstop */
      if (strand1 == Seq_strand_minus)
         AlnMgr2GetNthSeqRangeInSA(amaip->sharedaln, n1, NULL, &currstop);
      else
         currstop = -1;

      /* add asp for each dsp_shared seg */
      for (i=0; i<dsp_shared->numseg; i++)
      {
         asp = (AM_Small2Ptr)MemNew(sizeof(AM_Small2));
         if (dsp_shared->starts[(dsp_shared->dim)*i + n1 - 1] < 0)
         {
            asp->n1 = currstop;
            asp->n2 = i+1;
            asp->n3 = AM_GAP;
            asp->n4 = dsp_shared->lens[i];
            if (asp_head != NULL)
            {
               asp_prev->next = asp;
               /*if (asp_prev->n1 == asp->n1)
                  asp->n5 = asp_prev->n5+1;*/
               asp_prev = asp;
            } else
               asp_head = asp_prev = asp;
         } else
         {
            asp->n1 = dsp_shared->starts[(dsp_shared->dim)*i + n1 - 1];
            asp->n2 = 1;
            asp->n3 = AM_START;
            asp->n4 = dsp_shared->lens[i];
            if (asp_head != NULL)
            {
               asp_prev->next = asp;
               /*if (asp_prev->n1 == asp->n1)
                  asp->n5 = asp_prev->n5+1;*/
               asp_prev = asp;
            } else
               asp_head = asp_prev = asp;
            asp = (AM_Small2Ptr)MemNew(sizeof(AM_Small2));
            asp->n1 = dsp_shared->starts[(dsp_shared->dim)*i + n1 - 1] + dsp_shared->lens[i] - 1;
            asp->n2 = 1;
            j = i+1;
            while (j<dsp_shared->numseg && dsp_shared->starts[(dsp_shared->dim)*j + n1 - 1] == -1)
            {
               j++;
            }
            if (j<dsp_shared->numseg)
            {
               if (dsp_shared->starts[(dsp_shared->dim)*j + n1 - 1] > asp->n1 + 1)
                  asp->n3 = AM_HARDSTOP;
               else
                  asp->n3 = AM_STOP;
            } else
               asp->n3 = AM_HARDSTOP;
            if (asp->n3 == AM_HARDSTOP)
            {
               if (strand1 != Seq_strand_minus)
                  asp->n4 = -(dsp_shared->starts[(dsp_shared->dim)*i+n1-1] + dsp_shared->lens[i]-1);
               else
                  asp->n4 = -dsp_shared->starts[(dsp_shared->dim)*i+n1-1];
            } else
               asp->n4 = -dsp_shared->lens[i];
            if (strand1 != Seq_strand_minus)
               currstop = asp->n1;
            else
               currstop = asp_prev->n1-1;
            asp_prev->next = asp;
            /*if (asp_prev->n1 == asp->n1)
               asp->n5 = asp_prev->n5+1;*/
            asp_prev = asp;
         }
      } /* asp for each dsp_shared seg */
      
      /* currstop = start of sap's n2-th seq */
      if (strand1 == Seq_strand_minus)
         AlnMgr2GetNthSeqRangeInSA(sap, n2, NULL, &currstop);
      else
         AlnMgr2GetNthSeqRangeInSA(sap, n2, &currstop, NULL);

      /* add asp for each dsp seg */
      for (i=0; i<dsp->numseg; i++)
      {
         asp = (AM_Small2Ptr)MemNew(sizeof(AM_Small2));
         if (dsp->starts[(dsp->dim)*i + n2 - 1] < 0)
         {
            asp->n1 = currstop;
            asp->n2 = dsp_shared->numseg+i+1;
            asp->n3 = AM_GAP;
            asp->n4 = dsp->lens[i];
            asp_prev->next = asp;
            /*if (asp_prev->n1 == asp->n1)
               asp->n5 = asp_prev->n5 + 1;*/
            asp_prev = asp;
         } else
         {
            asp->n1 = dsp->starts[(dsp->dim)*i + n2 - 1];
            asp->n2 = 1;
            asp->n3 = AM_START;
            asp->n4 = dsp->lens[i];
            asp_prev->next = asp;
            /*if (asp_prev->n1 == asp->n1)
               asp->n5 = asp_prev->n5+1;*/
            asp_prev = asp;
            asp = (AM_Small2Ptr)MemNew(sizeof(AM_Small2));
            asp->n1 = dsp->starts[(dsp->dim)*i + n2 - 1] + dsp->lens[i] - 1;
            asp->n2 = 1;
            j = i+1;
            while (j<dsp->numseg && dsp->starts[(dsp->dim)* j + n2 - 1] == -1)
            {  
               j++;
            }
            if (j<dsp->numseg)
            {
               if (dsp->starts[(dsp->dim)*j + n2 - 1] > asp->n1 + 1)
                  asp->n3 = AM_HARDSTOP;
               else
                  asp->n3 = AM_STOP;
            } else
               asp->n3 = AM_HARDSTOP;
            if (asp->n3 == AM_HARDSTOP)
            {
               if (strand1 != Seq_strand_minus)
                  asp->n4 = -(dsp->starts[(dsp->dim)*i+n1-1] + dsp->lens[i]-1);
               else
                  asp->n4 = -dsp->starts[(dsp->dim)*i+n1-1];
               /* so if n4 is negative, this is the highest-numbered residue in the interval */
            } else
               asp->n4 = dsp->lens[i];
            if (strand1 != Seq_strand_minus)
               currstop = asp->n1;
            else
               currstop = asp_prev->n1-1;
            asp_prev->next = asp;
            /*if (asp_prev->n1 == asp->n1)
               asp->n5 = asp_prev->n5 + 1;*/
            asp_prev = asp;
         }
      }
      
      /* create asparray and heapsort it */
      asp = asp_head;
      i = 0;
      while (asp != NULL)
      {
         i++;
         asp = asp->next;
      }
      asparray = (AM_Small2Ptr PNTR)MemNew(i*sizeof(AM_Small2Ptr));
      asp = asp_head;
      i = 0;
      while (asp != NULL)
      {
         asparray[i] = asp;
         i++;
         asp = asp->next;
      }
      if (strand1 != Seq_strand_minus)
         HeapSort(asparray, i, sizeof(asparray), AlnMgr2CompareAsps);
      else
         HeapSort(asparray, i, sizeof(asparray), AlnMgr2CompareAspsMinus);
      /* now need to remove redundant (identical) points        */
      /* but still need to count those points toward the states */
      asp = asparray[0];
      asp->next = NULL;
      for (j=0; j<i-1; j++)
      {
         if (asparray[j+1]->n1 != asp->n1 || asparray[j+1]->n3 != asp->n3 || asp->n3 == AM_GAP)
         {
            asp->next = asparray[j+1];
            asp->next->next = NULL;
            asp = asp->next;
         } else
         {
            k = j;
            while (asparray[k] == NULL && k >= 0)
            {
               k--;
            }
            if (k>=0 && asparray[k]->n3 != AM_GAP)
               asparray[k]->n2++;
            MemFree(asparray[j+1]);
            asparray[j+1] = NULL;
         }
      }
      asp_head = asparray[0];
      MemFree(asparray);
      j=0;
      asp = asp_head;
      asp_prev = NULL;
      /* count up the segments; two consecutive stops make a segment */
      state = 0;
      if (strand1 != Seq_strand_minus)
      {
         while (asp != NULL)
         {
            if (asp->n3 == AM_START)
            {
               state += asp->n2;
               j++;
            } else if (asp->n3 == AM_STOP)
            {
               state -= asp->n2;
               asp_tmp = asp->next;
               while (asp_tmp != NULL && asp_tmp->n3 == AM_GAP)
               {
                  asp_tmp = asp_tmp->next;
               }
               if (state != 0 && asp_tmp != NULL && asp_tmp->n1 != asp->n1+1 && (asp_tmp->n3 != AM_HARDSTOP || asp_tmp->n1 != asp->n1))
                  j++;
               else if (state != 0 && asp->next != NULL && asp_tmp != NULL && (asp_tmp->n3 != AM_HARDSTOP || asp_tmp->n1 != asp->n1))
               {
                  asp_tmp2 = asp_tmp;
                  while (asp_tmp2 != NULL && asp->n1+1 == asp_tmp2->n1 && asp_tmp2->n3 != AM_START)
                  {
                     asp_tmp2 = asp_tmp2->next;
                  }
                  if (asp_tmp2 != NULL && ((asp_tmp2->n1 == asp->n1+1 && asp_tmp2->n3 != AM_START) || asp_tmp2->n1 != asp->n1+1) && (asp_tmp->n3 != AM_HARDSTOP || asp_tmp->n1 != asp->n1))
                     j++;
               }
            } else if (asp->n3 == AM_GAP)
               j++;
            else if (asp->n3 == AM_HARDSTOP)
            {
               state -= asp->n2;
               asp_tmp = asp->next;
               while (asp_tmp != NULL && asp_tmp->n3 == AM_GAP)
               {
                  asp_tmp = asp_tmp->next;
               }
               if (state != 0 && asp_tmp != NULL && asp_tmp->n1 != asp->n1+1)
                  j++;
               else if (state != 0 && asp->next != NULL && asp_tmp != NULL)
               {
                  asp_tmp2 = asp_tmp;
                  while (asp_tmp2 != NULL && asp->n1+1 == asp_tmp2->n1 && asp_tmp2->n3 != AM_START)
                  {
                     asp_tmp2 = asp_tmp2->next;
                  }
                  if (asp_tmp2 != NULL && ((asp_tmp2->n1 == asp->n1+1 && asp_tmp2->n3 != AM_START) || asp_tmp2->n1 != asp->n1+1))
                     j++;
                  else if (asp_tmp2 == NULL)
                     j++;
               }
            }
            asp = asp->next;
         }
      } else
      {
         currstop = -1;
         while (asp != NULL)
         {
            if (asp->n3 == AM_STOP || asp->n3 == AM_HARDSTOP)
            {
               if (currstop != asp->n1 && state > 0)
                  j++;
               currstop = asp->n1;
               state += asp->n2;
            } else if (asp->n3 == AM_START)
            {
               state -= asp->n2;
               j++;
               currstop = asp->n1 - 1;
            } else if (asp->n3 == AM_GAP)
               j++;
            asp = asp->next;
         }
      }

      /* dsp_new */
      dsp_new = DenseSegNew();
      dsp_new->dim = numrows;
      dsp_new->numseg = j;
      dsp_new->ids = SeqIdDupList(dsp_shared->ids);
      dsp_new->starts = (Int4Ptr)MemNew((dsp_new->numseg)*(dsp_new->dim)*sizeof(Int4));
      dsp_new->strands = (Uint1Ptr)MemNew((dsp_new->numseg)*(dsp_new->dim)*sizeof(Uint1));
      dsp_new->lens = (Int4Ptr)MemNew((dsp_new->numseg)*sizeof(Int4));

      /* get all the ids except for the duplicated one */
      sip_head = NULL;
      sip_tmp = NULL;
      sip = dsp->ids;
      i=0;
      /* get all the ids except for the duplicated one */
      while (sip != NULL)
      {
         if (i+1 != n2)
         {
            if (sip_tmp != NULL)
            {
               sip_tmp->next = SeqIdDup(sip);
               sip_tmp = sip;
            } else
               sip_head = sip_tmp = SeqIdDup(sip);
         }
         i++;
         sip = sip->next;
      }
      sip = dsp_new->ids;
      while (sip->next != NULL)
      {
         sip = sip->next;
      }
      sip->next = sip_head;

      /* construct starts and lens from asps */
      asp = asp_head;
      i=0;
      state = 0;
      currstop = -1;
      if (strand1 != Seq_strand_minus)
      {
         while (asp != NULL)
         {
            if (asp->n3 == AM_START)
            {
               state += asp->n2;
               dsp_new->starts[dsp_new->dim*i+n1-1] = asp->n1;
               dsp_new->lens[i] = asp->n4;
               i++;
            } else if (asp->n3 == AM_STOP)
            {
               state -= asp->n2;
               asp_tmp = asp->next;
               while (asp_tmp != NULL && asp_tmp->n3 == AM_GAP)
               {
                  asp_tmp = asp_tmp->next;
               }
               if (state != 0 && asp_tmp != NULL && asp_tmp->n1 != asp->n1+1 && (asp_tmp->n3 != AM_HARDSTOP || asp_tmp->n1 != asp->n1))
               {
                  dsp_new->starts[dsp_new->dim*i+n1-1] = asp->n1 + 1;
                  dsp_new->lens[i] = asp->n4;
                  i++;
               } else if (state != 0 && asp->next != NULL && asp_tmp != NULL && i < dsp_new->numseg && (asp_tmp->n3 != AM_HARDSTOP || asp_tmp->n1 != asp->n1))
               {
                  asp_tmp2 = asp_tmp;
                  while (asp_tmp2 != NULL && asp->n1+1 == asp_tmp2->n1 && asp_tmp2->n3 != AM_START)
                  {
                     asp_tmp2 = asp_tmp2->next;
                  }
                  if (asp_tmp2 != NULL && ((asp_tmp2->n1 == asp->n1+1 && asp_tmp2->n3 != AM_START) || asp_tmp2->n1 != asp->n1+1) && (asp_tmp->n3 != AM_HARDSTOP || asp_tmp->n1 != asp->n1))
                  {
                     dsp_new->starts[dsp_new->dim*i+n1-1] = asp->n1 + 1;
                     dsp_new->lens[i] = asp->n4;
                     i++;
                  }
               }
            } else if (asp->n3 == AM_GAP)
            {
               dsp_new->starts[dsp_new->dim*i+n1-1] = -asp->n2;
               if (asp->n2 > dsp_shared->numseg)
                  dsp_new->lens[i] = dsp->lens[(asp->n2-1)-(dsp_shared->numseg)];
               else
                  dsp_new->lens[i] = dsp_shared->lens[asp->n2-1];
               i++;
            } else if (asp->n3 == AM_HARDSTOP)
            {
               state -= asp->n2;
               asp_tmp = asp->next;
               while (asp_tmp != NULL && asp_tmp->n3 == AM_GAP)
               {
                  asp_tmp = asp_tmp->next;
               }
               if (state != 0 && asp->next != NULL && asp_tmp != NULL && asp_tmp->n1 != asp->n1+1 && i < dsp_new->numseg)
               {
                  dsp_new->starts[dsp_new->dim*i+n1-1] = asp->n1 + 1;
                  if (asp->n1 > -asp->n4)
                     dsp_new->lens[i] = asp->n4;
                  i++;
               } else if (state != 0 && asp->next != NULL && asp_tmp != NULL && i < dsp_new->numseg)
               {
                  asp_tmp2 = asp_tmp;
                  while (asp_tmp2 != NULL && asp->n1+1 == asp_tmp2->n1 && asp_tmp2->n3 != AM_START)
                  {
                     asp_tmp2 = asp_tmp2->next;
                  }
                  if (asp_tmp2 != NULL && ((asp_tmp2->n1 == asp->n1+1 && asp_tmp2->n3 != AM_START) || asp_tmp2->n1 != asp->n1+1))
                  {
                     dsp_new->starts[dsp_new->dim*i+n1-1] = asp->n1 + 1;
                     if (asp->n1 > -asp->n4)
                        dsp_new->lens[i] = asp->n4;
                     i++;
                  } else if (asp_tmp2 == NULL)
                  {
                     dsp_new->starts[dsp_new->dim*i+n1-1] = asp->n1 + 1;
                     if (asp->n1 > -asp->n4)
                        dsp_new->lens[i] = asp->n4;
                     i++;
                  }
               }
            }
            asp = asp->next;
         }
         for (i=0; i<dsp_new->numseg; i++)
         {
            found = FALSE;
            for (j=i+1; j<dsp_new->numseg && !found; j++)
            {
               if (dsp_new->starts[dsp_new->dim*j+n1-1] > -1)
               {
                  if (dsp_new->lens[i] == 0)
                     dsp_new->lens[i] = dsp_new->starts[dsp_new->dim*j+n1-1] - dsp_new->starts[dsp_new->dim*i+n1-1];
                  else if (dsp_new->lens[i] > 0)
                     dsp_new->lens[i] = MIN(dsp_new->lens[i], dsp_new->starts[dsp_new->dim*j+n1-1] - dsp_new->starts[dsp_new->dim*i+n1-1]);
                  else if (dsp_new->lens[i] < 0)
                     dsp_new->lens[i] = -dsp_new->lens[i]-dsp_new->starts[dsp_new->dim*i+n1-1]+1;
                  found = TRUE;
               }
            }
            if (!found) /* last segment */
            {
               if (dsp_new->starts[dsp_new->dim*i+n1-1] >= 0)
               {
                  AlnMgr2GetNthSeqRangeInSA(amaip->sharedaln, n1, NULL, &stop1);
                  AlnMgr2GetNthSeqRangeInSA(sap, n2, NULL, &stop2);
                  dsp_new->lens[i] = (MAX(stop1, stop2) + 1) - dsp_new->starts[dsp_new->dim*i+n1-1];
               }
            }
         }
      } else
      {
         while (asp != NULL)
         {
            if (asp->n3 == AM_STOP)
            {
               if (currstop != asp->n1 && state > 0)
               {
                  dsp_new->starts[dsp_new->dim*i+n1-1] = asp->n1+1;
                  dsp_new->lens[i] = currstop - asp->n1;
                  i++;
               }
               currstop = asp->n1;
               state += asp->n2;
            } else if (asp->n3 == AM_START)
            {
               state -= asp->n2;
               dsp_new->starts[dsp_new->dim*i+n1-1] = asp->n1;
               dsp_new->lens[i] = currstop - asp->n1 + 1;
               i++;
               currstop = asp->n1 - 1;
            } else if (asp->n3 == AM_GAP)
            {
               dsp_new->starts[dsp_new->dim*i+n1-1] = -asp->n2;
               if (asp->n2 > dsp_shared->numseg)
                  dsp_new->lens[i] = dsp->lens[(asp->n2-1)-(dsp_shared->numseg)];
               else
                  dsp_new->lens[i] = dsp_shared->lens[asp->n2-1];
               i++;
            } else if (asp->n3 == AM_HARDSTOP)
            {
               if (currstop != asp->n1 && state > 0 && asp->next != NULL)
               {
                  dsp_new->starts[dsp_new->dim*i+n1-1] = asp->n1+1;
                  dsp_new->lens[i] = currstop - asp->n1;
                  i++;
               }
               currstop = asp->n1;
               state += asp->n2;
            }
            asp = asp->next;
         }
      }
      /* now add in the other rows, starting with rows from the sharedaln */
      for (i=0; i<dsp_shared->dim; i++)
      {
         if (i+1 != n1)
         {
            for (j=0; j<dsp_new->numseg; j++)
            {
               if (dsp_new->starts[dsp_new->dim*j+n1-1] >= 0)
                  dsp_new->starts[dsp_new->dim*j+i] = AlnMgr2MapSegStartToSegStart(amaip->sharedaln, dsp_new->starts[dsp_new->dim*j+n1-1], n2, i+1, dsp_new->lens[j]);
               else
               {
                  if (-(dsp_new->starts[dsp_new->dim*j+n1-1]) > dsp_shared->numseg)
                  /* this gap came from the new sap */
                     dsp_new->starts[dsp_new->dim*j+i] = -1;
                  else /* this gap came from the sharedaln */
                     dsp_new->starts[dsp_new->dim*j+i] = dsp_shared->starts[dsp_shared->dim*(-dsp_new->starts[dsp_new->dim*j+n1-1]-1)+i];
               }
               dsp_new->strands[dsp_new->dim*j+i] = AlnMgr2GetNthStrand(amaip->sharedaln, i+1);
            }
         }
      }
      for (i=0; i<dsp->dim; i++)
      {
         if (i+1 != n2)
         {
            if (i+1 > n2)
               offset = 1;
            else
               offset = 0;
            for (j=0; j<dsp_new->numseg; j++)
            {
               if (dsp_new->starts[dsp_new->dim*j+n1-1] >= 0)
                  dsp_new->starts[dsp_new->dim*j+i+dsp_shared->dim-offset] = AlnMgr2MapSegStartToSegStart(sap, dsp_new->starts[dsp_new->dim*j+n1-1], n1, i+1, dsp_new->lens[j]);
               else
               {
                  if (-(dsp_new->starts[dsp_new->dim*j+n1-1]) > dsp_shared->numseg)
                  /* this gap is from the new sap */
                     dsp_new->starts[dsp_new->dim*j+i+dsp_shared->dim-offset] = dsp->starts[dsp->dim*((-dsp_new->starts[dsp_new->dim*j+n1-1])-dsp_shared->numseg-1)+i];
                  else /* this gap is from the shared alignment */
                     dsp_new->starts[dsp_new->dim*j+i+dsp_shared->dim-offset] = -1;
               }
               dsp_new->strands[dsp_new->dim*j+i+dsp_shared->dim-offset] = AlnMgr2GetNthStrand(sap, i+1);
            }
         }
      }
      /* fill in strand info for shared row, and get rid of segment keys (neg numbers) */
      for (j=0; j<dsp_new->numseg; j++)
      {
         dsp_new->strands[dsp_new->dim*j+n1-1] = AlnMgr2GetNthStrand(amaip->sharedaln, n1);
         if (dsp_new->starts[dsp_new->dim*j+n1-1] < 0)
            dsp_new->starts[dsp_new->dim*j+n1-1] = -1;
      }
if (dsp_new->dim > 10)
   dsp_new->dim = dsp_new->dim;
      AlnMgr2CondenseRows(dsp_new, dsp_new->dim);
      sap_new = SeqAlignNew();
      sap_new->segtype = SAS_DENSEG;
      sap_new->segs = (Pointer)(dsp_new);
      AlnMgr2IndexSingleChildSeqAlign(sap_new);
      SeqAlignFree(amaip->sharedaln);
      amaip->sharedaln = sap_new;
      amaip->numrows = dsp_new->dim;
      amaip->ids = (SeqIdPtr PNTR)MemNew(amaip->numrows*sizeof(SeqIdPtr));
      sip = dsp_new->ids;
      for (i=0; i<amaip->numrows; i++)
      {
         amaip->ids[i] = SeqIdDup(sip);
         sip = sip->next;
      }
      while (asp_head != NULL)
      {
         asp = asp_head->next;
         MemFree(asp_head);
         asp_head = asp;
      }
   }
}

/* SECTION 2c */
static Int4 AlnMgr2MapSegStartToSegStart(SeqAlignPtr sap, Int4 pos, Int4 row1, Int4 row2, Int4 len)
{
   Int4         diff;
   DenseSegPtr  dsp;
   Int4         pos2;
   Int4         seg;
   Uint1        strand1;
   Uint1        strand2;

   if (sap == NULL)
      return -1;
   seg = AlnMgr2GetSegForStartPos(sap, pos, row1);
   if (seg < 0)
      return -1;
   dsp = (DenseSegPtr)(sap->segs);
   if (dsp->starts[dsp->dim*seg+row2-1] == -1)
      return -1;
   strand1 = dsp->strands[dsp->dim*seg+row1-1];
   strand2 = dsp->strands[dsp->dim*seg+row2-1];
   if (strand1 != strand2)
      pos = pos + len - 1;
   if (strand1 == Seq_strand_minus)
      diff = dsp->lens[seg] - (pos - dsp->starts[dsp->dim*seg+row1-1]) - 1;
   else
      diff = pos - dsp->starts[dsp->dim*seg+row1-1];
   if (diff > dsp->lens[seg]) /* unaligned here */
      return -1;
   if (strand2 == Seq_strand_minus)
      pos2 = dsp->starts[dsp->dim*seg+row2-1] + dsp->lens[seg] - diff -1;
   else
      pos2 = dsp->starts[dsp->dim*seg+row2-1]+ diff;
   return pos2;
}

/* SECTION 2c */
static Int4 AlnMgr2GetSegForStartPos(SeqAlignPtr sap, Int4 pos, Int4 row)
{
   Uint2Ptr         array;
   DenseSegPtr      dsp;
   Int4             L;
   Int4             mid;
   Int4             offset;
   Int4             R;
   SAIndex2Ptr       saip;
   SARowDat2Ptr      srdp;
   Int4             start;
   Int4             stop;
   Uint1            strand;

   if (sap == NULL || sap->saip == NULL || row < 1)
      return -1;
   AlnMgr2GetNthSeqRangeInSA(sap, row, &start, &stop);
   if (pos < start || pos > stop)
      return -1;
   saip = (SAIndex2Ptr)(sap->saip);
   if (row > saip->numrows)
      return -1;
   srdp = saip->srdp[row-1];
   strand = AlnMgr2GetNthStrand(sap, row);
   dsp = (DenseSegPtr)(sap->segs);
   L = 0;
   R = srdp->numsect - 1;
   if (strand != Seq_strand_minus)
   {
      while (L < R)
      {
         mid = MIN((L + R)/2, srdp->numsect-2);
         if (dsp->starts[(srdp->sect[mid + 1])*(dsp->dim)+row-1] <= pos)
            L = mid+1;
         else
            R = mid;
      }
   } else
   {
      while (L < R)
      {
         mid = (L + R)/2;
         if (dsp->starts[(srdp->sect[mid])*(dsp->dim)+row-1] > pos)
            L = mid + 1;
         else
            R = mid;
      }
   }
   offset = pos - dsp->starts[(srdp->sect[L])*(dsp->dim)+row-1];
   if (offset >= dsp->lens[srdp->sect[L]])
      return -2;  /* this is an insert */
   if (saip->anchor > 0)
   {
      array = saip->srdp[saip->anchor-1]->sect;
      R = binary_search_on_uint2_list(array, srdp->sect[L], saip->srdp[saip->anchor-1]->numsect);
      L = R;
   }
   return srdp->sect[L];
}

static Int4 GetNextStart (DenseSegPtr dsp, Int4 row, Int4 col, Int4Ptr pnext_start_col)
{
  Int4 next_start_col;
  
  if (dsp == NULL || row < 0 || row >= dsp->dim || col < 0 || col >= dsp->numseg)
  {
    return -1;
  }
  
  for (next_start_col = col + 1;
       next_start_col < dsp->numseg 
         && dsp->starts[(next_start_col * dsp->dim) + row] == -1; 
       next_start_col++)
  {
  }
  if (next_start_col < dsp->numseg)
  {
    if (pnext_start_col != NULL)
    {
      *pnext_start_col = next_start_col;
    }
    return dsp->starts[(next_start_col * dsp->dim) + row];
  }
  else
  {
    return -1;
  }
}

static void AlnMgr2CondenseColumns(DenseSegPtr dsp)
/***************************************************************************
*
*  AlnMgr2CondenseColumns finds adjacent columns which appear to align but
*  were not put in one column by the mixing mechanism because the input was
*  a set of pairwise alignment with a gap on the common sequence in this
*  segment. Or graphically:
*
*  ----- ----- ----- -----             -----
*  AACCG ----- ----- -----   becomes   AACCG
*  ----- AACCG ----- -----             AACCG
*  ----- ----- AACCG -----             AACCG
*  ----- ----- ----- AACCG             AACCG
*
***************************************************************************/
{
  int gap_start_seg = -1;
  int gap_end_seg = -1;
  int row, seg, base_col, col, next_start, next_start_col;
  Boolean can_fit;

  for (seg = 0;  seg < dsp->numseg;  ++seg) {
    if (dsp->starts[dsp->dim * seg] == -1) {
      if (gap_start_seg == -1) {
        gap_start_seg = seg;
      }
      else {
        if (seg == dsp->numseg - 1) {
          gap_end_seg = seg + 1;
        }
      }
    }
    else {
      if (gap_start_seg != -1) {
        gap_end_seg = seg;
      }
    }

    if (gap_end_seg != -1) {
      for (base_col = gap_start_seg;  base_col<gap_end_seg;  ++base_col) {
        int len = dsp->lens[base_col];
        for (col = base_col + 1;  col<gap_end_seg;  ++col) {
          if (dsp->lens[col] != len) {
            continue;
          }

          can_fit = TRUE;
          for (row = 0;  row < dsp->dim;  ++row) {
            if (dsp->starts[dsp->dim * col + row] != -1  &&  
                dsp->starts[dsp->dim * base_col + row] != -1) {
              can_fit = FALSE;
              break;
            }
            else if (dsp->starts[dsp->dim * col + row] != -1)
            {
              /* make sure we aren't going to disturb the order of
               * the starts */
              next_start = GetNextStart (dsp, row, base_col, &next_start_col);
              if (next_start > -1
                  && next_start < dsp->starts[dsp->dim * col + row]
                  && next_start_col < col)
              {
                can_fit = FALSE;
              }
            }
          }

          if (can_fit) {
            for (row = 0;  row<dsp->dim;  ++row) {
              if (dsp->starts[dsp->dim * col + row] != -1) {
                dsp->starts[dsp->dim * base_col + row] =
                  dsp->starts[dsp->dim * col + row];
              }
            }
            
            /* remove column col */
            {{
              Int4Ptr       starts, lens;
              Uint1Ptr      strands;
              Uint4         pos, new_pos;

              starts = (Int4Ptr)MemNew(dsp->dim*(dsp->numseg-1)*sizeof(Int4));
              strands = (Uint1Ptr)MemNew(dsp->dim*(dsp->numseg-1)*sizeof(Uint1));
              lens = (Int4Ptr)MemNew((dsp->numseg-1)*sizeof(Int4));
              
              for (pos=0; pos<dsp->dim*col; pos++) {
                starts[pos] = dsp->starts[pos];
                strands[pos] = dsp->strands[pos];
              }
              for (new_pos=pos, pos+=dsp->dim; pos<dsp->dim*dsp->numseg;
                   pos++, new_pos++) {
                starts[new_pos] = dsp->starts[pos];
                strands[new_pos] = dsp->strands[pos];
              }
              
              for (pos=0; pos<col; pos++) {
                lens[pos] = dsp->lens[pos];
              }
              for (new_pos=pos, pos++; pos<dsp->numseg; pos++, new_pos++) {
                lens[new_pos] = dsp->lens[pos];
              }

              MemFree(dsp->starts);
              MemFree(dsp->strands);
              dsp->starts = starts;
              dsp->strands = strands;
              dsp->lens = lens;

              dsp->numseg--;

            }}

            --gap_end_seg;
            --seg;
            --col;
          }
        }
      }
      
      gap_start_seg = -1;
      gap_end_seg = -1;
    }
  }
}

/* SECTION 2c */
/***************************************************************************
*
*  AlnMgr2CondenseRows finds rows of a dense-seg structure that are related
*  and that could be condensed into a single row (or fewer rows). It then
*  calls AlnMgr2DoCondense to condense those rows into continuous or
*  discontinuous rows. whichrow designates which row to merge, if
*  less than 1, the function tries to merge the last row.
*
***************************************************************************/
static void AlnMgr2CondenseRows(DenseSegPtr dsp, Int4 whichrow)
{
   Boolean     done;
   Int4        i;
   Int4        j;
   Int4        k;
   Int4        numrows;
   AMCdRowPtr  row;
   AMCdRowPtr  PNTR rowarray;
   SeqIdPtr    sip;
   SeqIdPtr    targetsip;

   sip = dsp->ids;
   rowarray = (AMCdRowPtr PNTR)MemNew((dsp->dim)*sizeof(AMCdRowPtr));
   if (whichrow < 1 || whichrow > dsp->dim)
      whichrow = dsp->dim;
   for (i=0; i<dsp->dim; i++)
   {
      row = (AMCdRowPtr)MemNew(sizeof(AMCdRow));
      row->sip = SeqIdDup(sip);
      sip = sip->next;
      row->strand = dsp->strands[i];
      row->rownum = i+1;
      rowarray[i] = row;
      if (i+1 == whichrow)
         targetsip = row->sip;
   }
   HeapSort(rowarray, i, sizeof(rowarray), AlnMgr2CompareCdRows);
   numrows = dsp->dim;
   j = -1; /* j marks the first occurrence of each sip */
   for (i=0; j==-1 && i<numrows; i++)
   {
      if (SeqIdComp(rowarray[i]->sip, targetsip) == SIC_YES)
      {
         j = i;
         if (rowarray[i]->rownum == whichrow) /* no other rows w/sip */
         {
            for (i=0; i<numrows; i++)
            {
               SeqIdFree(rowarray[i]->sip);
               MemFree(rowarray[i]);
            }
            MemFree(rowarray);
            return;
         }
      }
   }
   sip = SeqIdDup(rowarray[j]->sip);
   done = FALSE;
   for (i=j; !done && rowarray[i]->rownum < whichrow; i++)
   {
      if (SeqIdComp(rowarray[i]->sip, sip) == SIC_YES)
      {
         if (rowarray[i]->strand == rowarray[j]->strand)
         {
            if (AlnMgr2DoCondense(dsp, rowarray[i]->rownum, whichrow))
            {
               for (k=0; k<numrows; k++)
               {
                  if (rowarray[k]->rownum > rowarray[i]->rownum)
                  {
                     rowarray[k]->rownum--;
                     whichrow--;
                  }
               }
            }
         }
      } else
      {
         done = TRUE;
         SeqIdFree(sip);
         sip = SeqIdDup(rowarray[i]->sip);
         j = i;
      }
   }
   SeqIdFree(sip);
   for (i=0; i<numrows; i++)
   {
      SeqIdFree(rowarray[i]->sip);
      MemFree(rowarray[i]);
   }
   MemFree(rowarray);
}

/* SECTION 2c */
/***************************************************************************
*
*  AlnMgr2DoCondense arithmetically condenses two related rows of a dense-seg
*  structure into a single continuous row, a single discontinuous row, or
*  two rows with different information than before.
*
***************************************************************************/
static Boolean AlnMgr2DoCondense(DenseSegPtr dsp, Int4 rownum1, Int4 rownum2)
{
   Int4          aln;
   SeqAlignPtr   fake_sap;
   Boolean       fits;
   Boolean       found;
   Int4          i;
   SeqIdPtr      id;
   SeqIdPtr      id_head;
   SeqIdPtr      id_prev;
   Int4          j;
   Int4          k;
   Int4          max1;
   Int4          max2;
   Boolean       merged;
   Int4          min1;
   Int4          min2;
   SAIndex2Ptr   saip;
   Boolean       someseq1;
   Boolean       someseq2;
   Int4Ptr       starts;
   Uint1         strand1;
   Uint1         strand2;
   Uint1Ptr      strands;
   AM_Small2Ptr  window;
   AM_Small2Ptr  window_head;
   AM_Small2Ptr  window_prev;

   /* always merge up to rownum1 (better rows are first) */
   if (rownum1 > rownum2)
   {
      i = rownum2;
      rownum2 = rownum1;
      rownum1 = i;
   }
   strand1 = dsp->strands[rownum1-1];
   strand2 = dsp->strands[rownum2-1];
   if (strand1 != strand2)
      return FALSE;
   i = 0;
   window_head = window_prev = NULL;
   while (i < dsp->numseg)
   {
      j = i;
      someseq1 = someseq2 = FALSE;
      if (dsp->starts[dsp->dim*j+rownum1-1] >= 0)
      {
         someseq1 = TRUE;
         while (j<dsp->numseg && dsp->starts[dsp->dim*j+rownum2-1] < 0)
         {
            j++;
         }
      } else if (dsp->starts[dsp->dim*j+rownum2-1] >= 0)
      {
         someseq2 = TRUE;
         while (j<dsp->numseg && dsp->starts[dsp->dim*j+rownum1-1] < 0)
         {
            j++;
         }
      }
      fits = FALSE;
      if (j > i)
      {
         if (strand1 == Seq_strand_minus)
         {
            if (someseq1 == FALSE)
            {
               min1 = -1;
               for (k=j; min1 == -1 && k<dsp->numseg; k++)
               {
                  if (dsp->starts[dsp->dim*k+rownum1-1] > -1)
                     min1 = dsp->starts[dsp->dim*k+rownum1-1]+dsp->lens[k]-1;
               }
               max1 = -1;
               for (k=(i-1); max1 == -1 && k>=0; k--)
               {
                  max1 = dsp->starts[dsp->dim*k+rownum1-1];
               }
            } else
            {
               min1 = -1;
               for (k=j-1; min1 == -1 && k>=i; k--)
               {
                  min1 = dsp->starts[dsp->dim*(k)+rownum1-1];
               }
               max1 = -1;
               for (k=i; min1 == -1 && k<j; k++)
               {
                  if (dsp->starts[dsp->dim*k+rownum1-1] >= 0)
                     max1 = dsp->starts[dsp->dim*k+rownum1-1] + dsp->lens[k] -1;
               }
            }
         } else
         {
            if (someseq1 == FALSE)
            {
               min1 = -1;
               for (k=i-1; min1 == -1 && k >= 0; k--)
               {
                  if (dsp->starts[dsp->dim*k+rownum1-1] > -1)
                     min1 = dsp->starts[dsp->dim*k+rownum1-1]+dsp->lens[k]-1;
               }
               max1 = -1;
               for (k=j; max1 == -1 && k<dsp->numseg; k++)
               {
                  max1 = dsp->starts[dsp->dim*k+rownum1-1];
               }
            } else
            {
               min1 = -1;
               for (k=i; min1 == -1 && k<j; k++)
               {
                  min1 = dsp->starts[dsp->dim*k+rownum1-1];
               }
               max1 = -1;
               for (k=j-1; max1 == -1 && k>i; k--)
               {
                  if (dsp->starts[dsp->dim*k+rownum1-1] >= 0)
                     max1 = dsp->starts[dsp->dim*(k)+rownum1-1] + dsp->lens[k] - 1;
               }
            }
         }
         if (strand2 == Seq_strand_minus)
         {
            if (someseq2 == FALSE)
            {
               min2 = -1;
               for (k=j; min2 == -1 && k<dsp->numseg; k++)
               {
                  if (dsp->starts[dsp->dim*k+rownum2-1] > -1)
                     min2 = dsp->starts[dsp->dim*k+rownum2-1]+dsp->lens[k]-1;
               }
               max2 = -1;
               for (k=(i-1); max2 == -1 && k>=0; k--)
               {
                  max2 = dsp->starts[dsp->dim*k+rownum2-1];
               }
            } else
            {
               min2 = -1;
               for (k=j-1; min2 == -1 && k>=i; k--)
               {
                  min2 = dsp->starts[dsp->dim*(k)+rownum2-1];
               }
               max2 = -1;
               for (k=i; max2 == -1 && k<j; k++)
               {
                  if (dsp->starts[dsp->dim*k+rownum2-1] >= 0)
                     max2 = dsp->starts[dsp->dim*k+rownum2-1] + dsp->lens[k]-1;
               }
            }
         } else
         {
            if (someseq2 == FALSE)
            {
               min2 = -1;
               for (k=i-1; min2 == -1 && k >= 0; k--)
               {
                  if (dsp->starts[dsp->dim*k+rownum2-1] > -1)
                     min2 = dsp->starts[dsp->dim*k+rownum2-1]+dsp->lens[k]-1;
               }
               max2 = -1;
               for (k=j; max2 == -1 && k<dsp->numseg; k++)
               {
                  max2 = dsp->starts[dsp->dim*k+rownum2-1];
               }
            } else
            {
               min2 = -1;
               for (k=i; min2 == -1 && k<j; k++)
               {
                  min2 = dsp->starts[dsp->dim*k+rownum2-1];
               }
               max2 = -1;
               for (k=j-1; max2 == -1 && k>=i; k--)
               {
                  if (dsp->starts[dsp->dim*(k)+rownum2-1] >= 0)
                     max2 = dsp->starts[dsp->dim*(k)+rownum2-1] + dsp->lens[k] - 1;
               }
            }
         }
         if (someseq1 == FALSE)
         {
            if ((min1 < min2 || min2 == -1) && (max1 > max2 || max1 == -1))
               fits = TRUE;
         } else
         {
            if ((min2 < min1 || min1 == -1) && (max2 > max1 || max2 == -1))
               fits = TRUE;
         }
         window = (AM_Small2Ptr)MemNew(sizeof(AM_Small2));
         window->n1 = i;
         window->n2 = j-1;
         if (!fits)
            window->n4 = -1;
         if (window_head != NULL)
         {
            window_prev->next = window;
            window_prev = window;
         } else
            window_head = window_prev = window;
      }
      if (i == j)
         i++;
      else
         i = j;
   }
   if (window_head == NULL)
      return FALSE;
   fake_sap = SeqAlignNew();
   fake_sap->segtype = SAS_DENSEG;
   fake_sap->segs = (Pointer)dsp;
   AlnMgr2IndexSingleChildSeqAlign(fake_sap);
   aln = AlnMgr2GetNumAlnBlocks(fake_sap);
   if (aln == 1) /* only merge if there is a single fitted window flanked by gaps */
   /*or if there are several contiguous fitted windows flanked by gaps */
   {
      if (window_head->next != NULL && window_head->n4 == 0)
      {
         window = window_head->next;
         while (window_head->n2+1 < dsp->numseg && dsp->starts[dsp->dim*(window_head->n2+1)+rownum1-1] == -1 && dsp->starts[dsp->dim*(window_head->n2+1)+rownum2-1] == -1)
         {
            window_head->n2++;
         }
         while (window != NULL && window->n4 == 0 && window->n1 == window_head->n2+1)
         {
            window_head->n2 = window->n2;
            window = window->next;
            while (window_head->n2+1 < dsp->numseg && dsp->starts[dsp->dim*(window_head->n2+1)+rownum1-1] == -1 && dsp->starts[dsp->dim*(window_head->n2+1)+rownum2-1] == -1)
            {
               window_head->n2++;
            }
         }
         if (window != NULL)
         {
            while (window_head != NULL)
            {
               window = window_head->next;
               MemFree(window_head);
               window_head = window;
            }
            fake_sap->segs = NULL;
            SeqAlignFree(fake_sap);
            return FALSE;
         }
      }
      if (window_head->n4 == -1)
      {
         while (window_head != NULL)
         {
            window = window_head->next;
            MemFree(window_head);
            window_head = window;
         }
         fake_sap->segs = NULL;
         SeqAlignFree(fake_sap);
         return FALSE;
      }
      found = FALSE;
      for (i=0; !found && i<window_head->n1; i++)
      {
         if (dsp->starts[dsp->dim*i+rownum1-1] != -1 && dsp->starts[dsp->dim*i+rownum2-1] != -1)
            found = TRUE;
      }
      for (i=window_head->n2+1; !found && i<dsp->numseg; i++)
      {
         if (dsp->starts[dsp->dim*i+rownum1-1] != -1 && dsp->starts[dsp->dim*i+rownum2-1] != -1)
            found = TRUE;
      }
      if (found)
      {
         while (window_head != NULL)
         {
            window = window_head->next;
            MemFree(window_head);
            window_head = window;
         }
         fake_sap->segs = NULL;
         SeqAlignFree(fake_sap);
         return FALSE;
      }
      /* merge whole row up to rownum1 */
      for (i=0; i<dsp->numseg; i++)
      {
         dsp->starts[dsp->dim*i+rownum1-1] = MAX(dsp->starts[dsp->dim*i+rownum1-1], dsp->starts[dsp->dim*i+rownum2-1]);
      }
      starts = (Int4Ptr)MemNew((dsp->dim-1)*(dsp->numseg)*sizeof(Int4));
      strands = (Uint1Ptr)MemNew((dsp->dim-1)*(dsp->numseg)*sizeof(Uint1));
      k = 0;
      for (i=0; i<dsp->dim; i++)
      {
         if (i != rownum2-1)
         {
            for (j=0; j<dsp->numseg; j++)
            {
               starts[(dsp->dim-1)*j+k] = dsp->starts[dsp->dim*j+i];
               strands[(dsp->dim-1)*j+k] = dsp->strands[dsp->dim*j+i];
            }
            k++;
         }
      }
      MemFree(dsp->starts);
      MemFree(dsp->strands);
      dsp->starts = starts;
      dsp->strands = strands;
      dsp->dim--;
      id_head = id_prev = NULL;
      id = dsp->ids;
      j = 0;
      while (id != NULL)
      {
         if (j+1 != rownum2)
         {
            if (id_head != NULL)
            {
               id_prev->next = SeqIdDup(id);
               id_prev = id_prev->next;
            } else
               id_head = id_prev = SeqIdDup(id);
         }
         j++;
         id = id->next;
      }
      SeqIdSetFree(dsp->ids);
      dsp->ids = id_head;
      while (window_head != NULL)
      {
         window = window_head->next;
         MemFree(window_head);
         window_head = window;
      }
      fake_sap->segs = NULL;
      SeqAlignFree(fake_sap);
      return TRUE;
   }
   /* now go through and find the largest piece of every window that can be merged */
   /* (can't split up an aligned region with the merge, though)                    */
   window = window_head;
   saip = (SAIndex2Ptr)(fake_sap->saip);
   while (window != NULL)
   {
      j = k = -1;
      found = FALSE;
      for (i=0; !found && i<window->n1; i++)
      {
         if (dsp->starts[dsp->dim*i+rownum1-1] != -1 && dsp->starts[dsp->dim*i+rownum2-1] != -1)
            found = TRUE;
      }
      if (!found)
         j = window->n1;
      found = FALSE;
      for (i=window->n2+1; !found && i<dsp->numseg; i++)
      {
         if (dsp->starts[dsp->dim*i+rownum1-1] != -1 && dsp->starts[dsp->dim*i+rownum2-1] != -1)
            found = TRUE;
      }
      if (!found)
         k = window->n2;
      if (j == -1)
      {
         found = FALSE;
         for (i = window->n1-1; !found && i<window->n2; i++)
         {
            j = binary_search_on_uint4_list(saip->unaln, i, saip->numunaln);
            if (j == i)
               found = TRUE;
            else
               j = -1;
         }
      }
      if (k == -1)
      {
         found = FALSE;
         for (i = window->n2; !found && i>=window->n1; i++)
         {
            k = binary_search_on_uint4_list(saip->unaln, i, saip->numunaln);
            if (k == i)
               found = TRUE;
            else
               k = -1;
         }
      }
      if (j > -1 && k > -1 && k > j)
      {
         window->n1 = j+1;
         window->n2 = k;
      } else
         window->n1 = -1;
      window = window->next;
   }
   window = window_head;
   while (window != NULL)
   {
      if (window->n4 == -1 && i >= 0) /* see if it fits now */
      {
         i = window->n1;
         j = window->n2+1;
         if (strand1 == Seq_strand_minus)
         {
            if (dsp->starts[dsp->dim*(j-1)+rownum1-1] == -1)
            {
               min1 = -1;
               for (k=j; min1 == -1 && k<dsp->numseg; k++)
               {
                  min1 = dsp->starts[dsp->dim*k+rownum1-1];
               }
               max1 = -1;
               for (k=(i-1); max1 == -1 && k>=0; k--)
               {
                  max1 = dsp->starts[dsp->dim*k+rownum1-1];
               }
            } else
            {
               min1 = dsp->starts[dsp->dim*(j-1)+rownum1-1];
               max1 = dsp->starts[dsp->dim*i+rownum1-1] + dsp->lens[i];
            }
         } else
         {
            if (dsp->starts[dsp->dim*(j-1)+rownum1-1] == -1)
            {
               min1 = -1;
               for (k=i-1; min1 == -1 && k >= 0; k--)
               {
                  min1 = dsp->starts[dsp->dim*k+rownum1-1];
               }
               max1 = -1;
               for (k=j; max1 == -1 && k<dsp->numseg; k++)
               {
                  max1 = dsp->starts[dsp->dim*k+rownum1-1];
               }
            } else
            {
               min1 = dsp->starts[dsp->dim*i+rownum1-1];
               max1 = dsp->starts[dsp->dim*(j-1)+rownum1-1] + dsp->lens[j-1];
            }
         }
         if (strand2 == Seq_strand_minus)
         {
            if (dsp->starts[dsp->dim*(j-1)+rownum2-1] == -1)
            {
               min2 = -1;
               for (k=j; min2 == -1 && k<dsp->numseg; k++)
               {
                  min2 = dsp->starts[dsp->dim*k+rownum2-1];
               }
               max2 = -1;
               for (k=(i-1); max2 == -1 && k>=0; k--)
               {
                  max2 = dsp->starts[dsp->dim*k+rownum2-1];
               }
            } else
            {
               min2 = dsp->starts[dsp->dim*(j-1)+rownum2-1];
               max2 = dsp->starts[dsp->dim*i+rownum2-1] + dsp->lens[i];
            }
         } else
         {
            if (dsp->starts[dsp->dim*(j-1)+rownum2-1] == -1)
            {
               min2 = -1;
               for (k=i-1; min2 == -1 && k >= 0; k--)
               {
                  min2 = dsp->starts[dsp->dim*k+rownum2-1];
               }
               max2 = -1;
               for (k=j; max2 == -1 && k<dsp->numseg; k++)
               {
                  max2 = dsp->starts[dsp->dim*k+rownum2-1];
               }
            } else
            {
               min2 = dsp->starts[dsp->dim*i+rownum2-1];
               max2 = dsp->starts[dsp->dim*(j-1)+rownum2-1] + dsp->lens[j-1];
            }
         }
         if (dsp->starts[dsp->dim*j+rownum1-1] == -1)
         {
            if (min1 < min2 && (max1 > max2 || max1 == -1))
               window->n4 = 0;
         } else
         {
            if (min2 < min1 && (max2 > max1 || max2 == -1))
               window->n4 = 0;
         }
      }
      if (window->n1 >= 0 && window->n4 >= 0)
      {
         for (i=window->n1; i<=window->n2; i++)
         {
            dsp->starts[dsp->dim*i+rownum1-1] = MAX(dsp->starts[dsp->dim*i+rownum1-1], dsp->starts[dsp->dim+i+rownum2-1]);
         }
      }
      window = window->next;
   }
   found = FALSE;
   /* check to see if rownum2 is all gaps now */
   for (i=0; !found && i<dsp->numseg; i++)
   {
      if (dsp->starts[dsp->dim*i+rownum2-1] != -1)
         found = TRUE;
   }
   merged = FALSE;
   if (!found) /* just gaps */
   {
      /* merge whole row up to rownum1 */
      for (i=0; i<dsp->numseg; i++)
      {
         dsp->starts[dsp->dim*i+rownum1-1] = MAX(dsp->starts[dsp->dim*i+rownum1-1], dsp->starts[dsp->dim*i+rownum2-1]);
      }
      starts = (Int4Ptr)MemNew((dsp->dim-1)*(dsp->numseg)*sizeof(Int4));
      strands = (Uint1Ptr)MemNew((dsp->dim-1)*(dsp->numseg)*sizeof(Uint1));
      k = 0;
      for (i=0; i<dsp->dim; i++)
      {
         if (i != rownum2-1)
         {
            for (j=0; j<dsp->numseg; j++)
            {
               starts[dsp->dim*j+k] = dsp->starts[dsp->dim*j+i];
               strands[dsp->dim*j+k] = dsp->strands[dsp->dim*j+i];
            }
            k++;
         }
      }
      MemFree(dsp->starts);
      MemFree(dsp->strands);
      dsp->starts = starts;
      dsp->strands = strands;
      dsp->dim--;
      id_head = id_prev = NULL;
      id = dsp->ids;
      j = 0;
      while (id != NULL)
      {
         if (j+1 != rownum2)
         {
            if (id_head != NULL)
            {
               id_prev->next = SeqIdDup(id);
               id_prev = id_prev->next;
            } else
               id_head = id_prev = SeqIdDup(id);
         }
         j++;
         id = id->next;
      }
      SeqIdSetFree(dsp->ids);
      dsp->ids = id_head;
      merged = TRUE;
   }
   while (window_head != NULL)
   {
      window = window_head->next;
      MemFree(window_head);
      window_head = window;
   }
   fake_sap->segs = NULL;
   SeqAlignFree(fake_sap);
   return merged;
}

/* SECTION 2c */
/***************************************************************************
*
*  AlnMgr2CompareCdRows is the HeapSort callback for AlnMgr2CondenseRows.
*  It puts the CDRows in order first by seqid and secondarily by row number.
*
***************************************************************************/
static int LIBCALLBACK AlnMgr2CompareCdRows(VoidPtr ptr1, VoidPtr ptr2)
{
   Int4        i;
   AMCdRowPtr  row1;
   AMCdRowPtr  row2;

   if (ptr1 == NULL || ptr2 == NULL)
      return 0;
   row1 = *((AMCdRowPtr PNTR)ptr1);
   row2 = *((AMCdRowPtr PNTR)ptr2);
   i = AlnMgr2OrderSeqIds(row1->sip, row2->sip);
   if (i == 0) /* sort from least rownum to greatest within each seqid */
   {
      if (row1->rownum < row2->rownum)
         return -1;
      else
         return 1;
   } else
      return i;
}

/* SECTION 2c */
/***************************************************************************
*
*  AlnMgr2CompareAsps is a HeapSort callback for AlnMgr2AddInNewSA. It
*  compares the starts (n1) of the two AM_Small2Ptrs; if those are the same
*  it compares the types.
*
***************************************************************************/
static int LIBCALLBACK AlnMgr2CompareAsps(VoidPtr ptr1, VoidPtr ptr2)
{
   AM_Small2Ptr  asp1;
   AM_Small2Ptr  asp2;

   if (ptr1 != NULL && ptr2 != NULL)
   {
      asp1 = *((AM_Small2Ptr PNTR)ptr1);
      asp2 = *((AM_Small2Ptr PNTR)ptr2);
      if (asp1->n1 < asp2->n1)
         return -1;
      else if (asp1->n1 > asp2->n1)
         return 1;
      else if (asp1->n5 < asp2->n5)
         return -1;
      else if (asp1->n5 > asp2->n5)
         return 1;
      else
      {
         if (asp1->n3 == AM_GAP && asp2->n3 == AM_GAP)
         {
            if (asp1->n2 < asp2->n2)
               return -1;
            if (asp1->n2 > asp2->n2)
               return 1;
         }
         if (asp1->n3 == AM_START)
         {
            if (asp2->n3 == AM_STOP)
               return -1;
            else if (asp2->n3 == AM_GAP)
               return -1;
            else if (asp2->n3 == AM_HARDSTOP)
               return -1;
            else
               return 0;
         } else if (asp1->n3 == AM_STOP)
         {
            if (asp2->n3 == AM_START)
               return 1;
            else if (asp2->n3 == AM_GAP)
               return 1;
            else if (asp2->n3 == AM_HARDSTOP)
               return -1;
            else
               return 0;
         } else if (asp1->n3 == AM_GAP)
         {
            if (asp2->n3 == AM_START)
               return 1;
            else if (asp2->n3 == AM_STOP)
               return -1;
            else if (asp2->n3 == AM_HARDSTOP)
               return -1;
            else
               return 0;
         } else if (asp1->n3 == AM_HARDSTOP)
         {
            if (asp2->n3 == AM_START)
               return 1;
            else if (asp2->n3 == AM_STOP)
               return 1;
            else if (asp2->n3 == AM_GAP)
               return 1;
            else
               return 0;
         }
      }
   }
   return 0;
}

/* SECTION 2c */
/***************************************************************************
*
*  AlnMgr2CompareAspsMinus is a HeapSort callback for AlnMgr2AddInNewSA. It
*  compares the starts (n1) of the two AM_Small2Ptrs; if those are the same
*  it compares the types. The only difference from AlnMgr2CompareAsps is
*  that it sorts the structures in the opposite order.
*
***************************************************************************/
static int LIBCALLBACK AlnMgr2CompareAspsMinus(VoidPtr ptr1, VoidPtr ptr2)
{
   AM_Small2Ptr  asp1;
   AM_Small2Ptr  asp2;

   if (ptr1 != NULL && ptr2 != NULL)
   {
      asp1 = *((AM_Small2Ptr PNTR)ptr1);
      asp2 = *((AM_Small2Ptr PNTR)ptr2);
      if (asp1->n1 > asp2->n1)
         return -1;
      else if (asp1->n1 < asp2->n1)
         return 1;
      else if (asp1->n5 < asp2->n5)
         return -1;
      else if (asp1->n5 > asp2->n5)
         return 1;
      else
      {
         if (asp1->n3 == AM_GAP && asp2->n3 == AM_GAP)
         {
            if (asp1->n2 < asp2->n2)
               return -1;
            if (asp1->n2 > asp2->n2)
               return 1;
         }
         if (asp1->n3 == AM_START)
         {
            if (asp2->n3 == AM_STOP)
               return 1;
            else if (asp2->n3 == AM_GAP)
               return -1;
            else if (asp2->n3 == AM_HARDSTOP)
               return 1;
            else
               return 0;
         } else if (asp1->n3 == AM_STOP)
         {
            if (asp2->n3 == AM_START)
               return -1;
            else if (asp2->n3 == AM_GAP)
               return -1;
            else if (asp2->n3 == AM_HARDSTOP)
               return 1;
            else
               return 0;
         } else if (asp1->n3 == AM_GAP)
         {
            if (asp2->n3 == AM_START)
               return 1;
            else if (asp2->n3 == AM_STOP)
               return 1;
            else if (asp2->n3 == AM_HARDSTOP)
               return 1;
            else
               return 0;
         } else if (asp1->n3 == AM_HARDSTOP)
         {
            if (asp2->n3 == AM_START)
               return -1;
            else if (asp2->n3 == AM_STOP)
               return -1;
            else if (asp2->n3 == AM_GAP)
               return -1;
            else
               return 0;
         }
      }
   }
   return 0;
}


/* SECTION 2c */
/***************************************************************************
*
*  AlnMgr2GetFirstSharedRow takes two indexed or unindexed dense-seg
*  seqaligns and returns the row numbers of the first sequence that is
*  shared between the two alignments. If the alignments do not share any
*  sequences, both n1 and n2 are set to 0.
*
***************************************************************************/
static void AlnMgr2GetFirstSharedRow(SeqAlignPtr sap1, SeqAlignPtr sap2, Int4Ptr n1, Int4Ptr n2)
{
   DenseSegPtr  dsp1;
   DenseSegPtr  dsp2;
   Int4         i;
   Int4         j;
   SeqIdPtr     sip1;
   SeqIdPtr     sip2;

   dsp1 = (DenseSegPtr)(sap1->segs);
   dsp2 = (DenseSegPtr)(sap2->segs);
   sip1 = dsp1->ids;
   i = 1;
   while (sip1 != NULL)
   {
      j = 1;
      sip2 = dsp2->ids;
      while (sip2 != NULL)
      {
         if (SeqIdComp(sip1, sip2) == SIC_YES)
         {
            *n1 = i;
            *n2 = j;
            return;
         }
         sip2 = sip2->next;
         j++;
      }
      sip1 = sip1->next;
      i++;
   }
   /* nothing found */
   *n1 = 0;
   *n2 = 0;
}

/* SECTION 2d */
static SeqIdPtr AlnMgr2SeqIdListsOverlap(SeqIdPtr sip1, SeqIdPtr sip2)
{
   SeqIdPtr  sip;
   SeqIdPtr  sip_tmp;

   if (sip1 == NULL || sip2 == NULL)
      return NULL;
   sip = sip1;
   while (sip != NULL)
   {
      sip_tmp = sip2;
      while (sip_tmp != NULL)
      {
         if (SeqIdComp(sip, sip_tmp) == SIC_YES)
            return sip;
         sip_tmp = sip_tmp->next;
      }
      sip = sip->next;
   }
   return NULL;
}

/***************************************************************************
*
*  AlnMgr2OrderSeqIds simply alphabetizes printed seqids in order to sort
*  them in order to group identical ones in a set.
*
***************************************************************************/
static Int4 AlnMgr2OrderSeqIds(SeqIdPtr sip1, SeqIdPtr sip2)
{
   Char  txt1[42];
   Char  txt2[42];

   if (sip1 == NULL && sip2 == NULL)
      return 0;
   if (sip1 == NULL && sip2 != NULL)
      return 1;
   if (sip1 != NULL && sip2 == NULL)
      return -1;
   SeqIdWrite(sip1, txt1, PRINTID_TEXTID_ACC_VER, 41);
   SeqIdWrite(sip2, txt2, PRINTID_TEXTID_ACC_VER, 41);
   txt1[41] = txt2[41] = '\0';
   return StringICmp(txt1, txt2);
}

/* SECTION 2d */
/***************************************************************************
*
*  AlnMgr2SetUnaln takes an indexed alignment and sets the numunaln and
*  unaln array fields. The unaligned regions are numbered the same
*  regardless of whether the alignment is anchored, although they will
*  most likely be accessed and displayed differently.
*
***************************************************************************/
static void AlnMgr2SetUnaln(SeqAlignPtr sap)
{
   AMAlignIndex2Ptr  amaip;
   AM_Small2Ptr      ams;
   AM_Small2Ptr      ams_head;
   AM_Small2Ptr      ams_prev;
   AM_Small2Ptr      PNTR amsarray;
   DenseSegPtr       dsp;
   Int4              i;
   Int4              j;
   SAIndex2Ptr       saip;

   if (sap == NULL || sap->saip == NULL)
      return;
   if (sap->saip->indextype == INDEX_CHILD)
   {
      saip = (SAIndex2Ptr)(sap->saip);
      dsp = (DenseSegPtr)(sap->segs);
   } else if (sap->saip->indextype == INDEX_PARENT)
   {
      amaip = (AMAlignIndex2Ptr)(sap->saip);
      if (amaip->alnstyle == AM2_LITE)
         return;
      saip = (SAIndex2Ptr)(amaip->sharedaln->saip);
      dsp = (DenseSegPtr)(amaip->sharedaln->segs);
   } else
      return;
   MemFree(saip->unaln);
   saip->unaln = NULL;
   ams_head = ams_prev = NULL;
   for (i=0; i<saip->numrows; i++)
   {
      for (j=0; j<saip->srdp[i]->numunaln; j++)
      {
         ams = (AM_Small2Ptr)MemNew(sizeof(AM_Small2));
         ams->n1 = saip->srdp[i]->unaligned[j];
         if (ams_head != NULL)
         {
            ams_prev->next = ams;
            ams_prev = ams;
         } else
            ams_head = ams_prev = ams;
      }
   }
   if (ams_head == NULL)
   {
      saip->numunaln = -1;
      return;
   }
   j = 0;
   ams = ams_head;
   while (ams != NULL)
   {
      j++;
      ams = ams->next;
   }
   amsarray = (AM_Small2Ptr PNTR)MemNew(j*sizeof(AM_Small2Ptr));
   j = 0;
   ams = ams_head;
   while (ams != NULL)
   {
      amsarray[j] = ams;
      j++;
      ams = ams->next;
   }
   HeapSort(amsarray, j, sizeof(AM_Small2Ptr), AlnMgr2CompareUnalnAMS);
   saip->numunaln = 1;
   for (i=1; i<j; i++)
   {
      if (amsarray[i]->n1 != amsarray[i-1]->n1)
         saip->numunaln++;
   }
   saip->unaln = (Uint4Ptr)MemNew(saip->numunaln*sizeof(Uint4));
   saip->unaln[0] = amsarray[0]->n1;
   saip->numunaln = 1;
   for (i=1; i<j; i++)
   {
      if (amsarray[i]->n1 != amsarray[i-1]->n1)
      {
         saip->unaln[saip->numunaln] = amsarray[i]->n1;
         saip->numunaln++;
      }
   }
   for (i=0; i<j; i++)
   {
      MemFree(amsarray[i]);
   }
   MemFree(amsarray);
}

/* SECTION 2d */
/***************************************************************************
*
*  AlnMgr2CompareUnalnAMS is the HeapSort callback for AlnMgr2SetUnaln;
*  it simply compares two AM_Small2 structures and orders them by their
*  n1 fields.
*
***************************************************************************/
static int LIBCALLBACK AlnMgr2CompareUnalnAMS(VoidPtr ptr1, VoidPtr ptr2)
{
   AM_Small2Ptr  ams1;
   AM_Small2Ptr  ams2;

   if (ptr1 == NULL || ptr2 == NULL)
      return 0;
   ams1 = *((AM_Small2Ptr PNTR)ptr1);
   ams2 = *((AM_Small2Ptr PNTR)ptr2);
   if (ams1->n1 < ams2->n1)
      return -1;
   else if (ams1->n1 > ams2->n1)
      return 1;
   else
      return 0;
}

/***************************************************************************
*
*  SECTION 3: Functions for debugging
*
***************************************************************************/

/* SECTION 3 */
NLM_EXTERN void am_print_sa_index(SeqAlignPtr sap, FILE *ofp)
{
   Int4        i;
   Int4        j;
   SAIndex2Ptr  saip;

   if (sap == NULL || sap->saip == NULL || sap->saip->indextype != INDEX_CHILD)
      return;
   saip = (SAIndex2Ptr)(sap->saip);
   fprintf(ofp, "Rows: %d\n", saip->numrows);
   fprintf(ofp, "Segments: %d\n", saip->numseg);
   fprintf(ofp, "Anchor: %d\n", saip->anchor);
   fprintf(ofp, "Alignment coordinates: ");
   for (i=0; i<saip->numseg; i++)
   {
      fprintf(ofp, "%d ", saip->aligncoords[i]);
   }
   fprintf(ofp, "\n\n");
   for (i=0; i<saip->numrows; i++)
   {
      fprintf(ofp, "row %d\n", i+1);
      fprintf(ofp, "numsect: %d\n", saip->srdp[i]->numsect);
      for (j=0; j<saip->srdp[i]->numsect; j++)
      {
         fprintf(ofp, "%d ", saip->srdp[i]->sect[j]);
      }
      fprintf(ofp, "\n");
      fprintf(ofp, "numunsect: %d\n", saip->srdp[i]->numunsect);
      for (j=0; j<saip->srdp[i]->numunsect; j++)
      {
         fprintf(ofp, "%d ", saip->srdp[i]->unsect[j]);
      }
      fprintf(ofp, "\n");
      fprintf(ofp, "numinsect: %d\n", saip->srdp[i]->numinsect);
      for (j=0; j<saip->srdp[i]->numinsect; j++)
      {
         fprintf(ofp, "%d ", saip->srdp[i]->insect[j]);
      }
      fprintf(ofp, "\n");
   }
}

/* SECTION 3 */
/***************************************************************************
*
*  AlnMgr2PrintSeqAlign prints an interleaved output of the entire
*  indexed alignment, with 'linesize' characters on each line (max 200).
*
***************************************************************************/
NLM_EXTERN void AlnMgr2PrintSeqAlign(SeqAlignPtr sap, Int4 linesize, Boolean isnuc, FILE *ofp)
{
   AlnMsg2Ptr   amp;
   BioseqPtr   bsp;
   Char        buf[201];
   Int4        ctr;
   Boolean     done;
   Int4        i;
   Int4        j;
   Int4        len;
   Boolean     more;
   Int4        numrows;
   Int4        row;
   Uint1       seqcode;
   SeqIdPtr    sip;
   SeqPortPtr  spp;
   Char        text[42];

   if (sap == NULL || sap->saip == NULL || linesize > 200)
      return;
   if (isnuc)
      seqcode = Seq_code_iupacna;
   else
      seqcode = Seq_code_iupacaa;
   amp = AlnMsgNew2();
   numrows = AlnMgr2GetNumRows(sap);
   len = AlnMgr2GetAlnLength(sap, FALSE);
   for (i=0; i<len; i+=linesize)
   {
      fprintf(ofp, "%d - %d\n", i, MIN(i+linesize-1, len-1));
      for (row=0; row<numrows; row++)
      {
         sip = AlnMgr2GetNthSeqIdPtr(sap, row+1);
         SeqIdWrite(sip, text, PRINTID_FASTA_SHORT, 41);
         done = FALSE;
         for (j=0; j<12; j++)
         {
            if (text[j] == '\0')
               done = TRUE;
            if (done == TRUE)
               fprintf(ofp, " ");
            else
               fprintf(ofp, "%c", text[j]);
         }
         bsp = BioseqLockById(sip);
         AlnMsgReNew2(amp);
         amp->row_num = row+1;
         amp->from_aln = i;
         amp->to_aln = MIN(i+linesize-1, len-1);
         while (more = AlnMgr2GetNextAlnBit(sap, amp))
         {
            if (amp->type == AM_GAP)
            {
               for (j=amp->from_row; j<=amp->to_row; j++)
               {
                  fprintf(ofp, "-");
               }
            } else
            {
               spp = SeqPortNew(bsp, amp->from_row, amp->to_row, amp->strand, seqcode);
               ctr = SeqPortRead(spp, (Uint1Ptr)buf, amp->to_row-amp->from_row+1);
               buf[ctr] = '\0';
               fprintf(ofp, buf);
               SeqPortFree(spp);
            }
         }
         BioseqUnlock(bsp);
         fprintf(ofp, "\n");
      }
      fprintf(ofp, "\n\n");
   }
   AlnMsgFree2(amp);
}

/* SECTION 3 */
NLM_EXTERN void AlnMgr2DumpIndexedAlnToFile(SeqAlignPtr sap, CharPtr filename)
{
   AsnIoPtr          aip;
   AMAlignIndex2Ptr  amaip;
   SeqAlignPtr       sap_tmp;

   if (sap == NULL || sap->saip == NULL)
      return;
   if (sap->saip->indextype == INDEX_CHILD)
   {
      if (sap->dim == 0)
         sap->dim = AlnMgr2GetNumRows(sap);
      aip = AsnIoOpen(filename, "w");
      SeqAlignAsnWrite(sap, aip, NULL);
      AsnIoClose(aip);
      return;
   }
   amaip = (AMAlignIndex2Ptr)(sap->saip);
   aip = AsnIoOpen(filename, "w");
   if (amaip->alnstyle != AM2_LITE)
   {
      amaip->sharedaln->dim = 0;  /* mark it as the sharedaln */
      SeqAlignAsnWrite(amaip->sharedaln, aip, NULL);
   }
   sap_tmp = sap;
   if (sap->dim == 0)
      sap->dim = AlnMgr2GetNumRows(sap);
   while (sap_tmp != NULL)
   {
      SeqAlignAsnWrite(sap_tmp, aip, NULL);
      sap_tmp = sap_tmp->next;
   }
   AsnIoClose(aip);
}

NLM_EXTERN SeqAlignPtr AlnMgr2ReadIndexedAlnFromFile(CharPtr filename)
{
   AsnIoOpen(filename, "r");
}

/***************************************************************************
*
*  SECTION 4: API-level functions (and their helper functions) used to
*  access an indexed alignment.
*    SECTION 4a: AlnMgr2GetNextAlnBit and associated functions
*    SECTION 4b: "GetNth" functions
*    SECTION 4c: other functions for accessing the alignment
*
***************************************************************************/

/* SECTION 4a */
/***************************************************************************
*
*  AlnMgr2GetNextAlnBit takes an indexed seqalign and returns it, piece
*  by piece, in the row and across the range specified in the AlnMsg
*  structure. amp->from_aln and amp->to_aln must be filled in; these are
*  in alignment coordinates. AlnMgr2GetNextAlnBit will return the AlnMsg
*  structure with amp->from_row and amp->to_row filled in. If amp->type is
*  AM_SEQ, these numbers are sequence coordinates; if amp->type is AM_GAP
*  the numbers are alignment coordinates and there is a gap in that row.
*  AlnMgr2GetNextAlnBit returns one continuous piece of sequence or gap
*  at each call, and keeps returning TRUE until it has returned all the
*  information for the piece of the alignment requested.
*
***************************************************************************/
NLM_EXTERN Boolean AlnMgr2GetNextAlnBit(SeqAlignPtr sap, AlnMsg2Ptr amp) /* NEXT */
{
   AMAlignIndex2Ptr  amaip;
   Uint2Ptr         array;
   Int4             arraylen;
   Int4             ctr;
   Int4             disc;
   Int4             disc1;
   DenseSegPtr      dsp;
   Int4             endoffset;
   Boolean          found;
   Int4             i;
   Int4             index;
   Int4             intfrom;
   Int4             intto;
   Int4             j;
   Int4             len;
   Int4             offset;
   SAIndex2Ptr       saip;
   SARowDat2Ptr      srdp;
   Int4             start_sect;
   Int4             stop_sect;
   Uint2Ptr         trans;
   Int4             translen;
   
   if (sap == NULL || sap->saip == NULL || amp == NULL)
      return FALSE;
   if (amp->left_interrupt != NULL)
   {
      MemFree(amp->left_interrupt);
      amp->left_interrupt = NULL;
   }
   if (amp->right_interrupt != NULL)
   {
      MemFree(amp->right_interrupt);
      amp->right_interrupt = NULL;
   }
   if (sap->saip->indextype == INDEX_CHILD)
   {
      dsp = (DenseSegPtr)(sap->segs);
      saip = (SAIndex2Ptr)(sap->saip);
   } else if (sap->saip->indextype == INDEX_PARENT)
   {
      amaip = (AMAlignIndex2Ptr)(sap->saip);
      if (amaip->alnstyle == AM2_LITE)
         return FALSE;
      dsp = (DenseSegPtr)(amaip->sharedaln->segs);
      saip = (SAIndex2Ptr)(amaip->sharedaln->saip);
   }
   /* reality checks */
   if (amp->row_num > saip->numrows)
      return FALSE;
   if (amp->len <= 0)
      amp->len = AlnMgr2GetAlnLength(sap, FALSE);
   if (amp->from_aln < 0 || amp->from_aln > amp->len-1 || amp->real_from > amp->to_aln)
      return FALSE;
   if (amp->to_aln == -1)
      amp->to_aln = amp->len - 1;
   if (amp->to_aln < amp->from_aln || amp->to_aln > amp->len-1)
      return FALSE;
   if (amp->real_from == -2)
      amp->real_from = amp->from_aln;
   amp->strand = AlnMgr2GetNthStrand(sap, amp->row_num);
   srdp = saip->srdp[amp->row_num-1];
   len = 0;
   start_sect = binary_search_on_uint4_list(saip->aligncoords, amp->real_from, saip->numseg);
   offset = amp->real_from - saip->aligncoords[start_sect];
   endoffset = 0;
   stop_sect = binary_search_on_uint4_list(saip->aligncoords, amp->to_aln, saip->numseg);
   /* now figure out whether it starts in sequence or a gap, and figure out how       */
   /* long it continues in the same mode without interruption by inserts or unaligned */
   /* regions; the whole contiguous stretch will be reported                          */
   if (saip->anchor > 0)
   {
      trans = saip->srdp[saip->anchor-1]->sect;
      translen = saip->srdp[saip->anchor-1]->numsect;
   } else
   {
      trans = (Uint2Ptr)MemNew(dsp->numseg*sizeof(Uint2));
      for (i=0; i<dsp->numseg; i++)
      {
         trans[i] = i;
      }
      translen = dsp->numseg;
   }
   arraylen = -1;
   if ((index = binary_search_on_uint2_list(srdp->sect, trans[start_sect], srdp->numsect)) != -1)
   {
      amp->type = AM_SEQ;
      array = srdp->sect;
      arraylen = srdp->numsect;
   } else if ((index = binary_search_on_uint2_list(srdp->unsect, trans[start_sect], srdp->numunsect)) != -1)
   {
      amp->type = AM_GAP;
      array = srdp->unsect;
      arraylen = srdp->numunsect;
   }
   if (arraylen == -1) /* error */
      return FALSE;
   if (amp->row_num == saip->anchor)
   {
      amp->type = AM_SEQ;
      /* find limits of aligned region */
      i = start_sect;
      j = srdp->sect[start_sect];
      disc = binary_search_on_uint2_list(srdp->unaligned, j, srdp->numunaln);
      while (j<srdp->sect[stop_sect] && disc == -1)
      {
         j++;
         disc = binary_search_on_uint2_list(srdp->unaligned, j, srdp->numunaln);
      }
      i = binary_search_on_uint2_list(srdp->sect, j, srdp->numsect);
      if (i == -1)
      {
         i = binary_search_on_uint2_list(srdp->unsect, j, srdp->numunsect);
      }
      endoffset = dsp->lens[trans[i]] - (amp->to_aln - saip->aligncoords[i]) - 1;
      if (endoffset < 0)
         endoffset = 0;
      if (i<stop_sect && endoffset == 0) /* there's an unaligned region here, and we go to the end of the segment */
      {
         AlnMgr2GetUnalignedInfo(sap, trans[i], amp->row_num, &intfrom, &intto);
         amp->right_interrupt = (AMInterruptPtr)MemNew(sizeof(AMInterrupt));
         amp->right_interrupt->row = amp->row_num;
         amp->right_interrupt->unalnlen = intto - intfrom + 1;
         amp->right_interrupt->segnum = trans[i];
         amp->right_interrupt->which_side = AM2_RIGHT;
      }
      stop_sect = i;
      if (start_sect > 0 && offset == 0)
      {
         disc = binary_search_on_uint2_list(srdp->unaligned, trans[start_sect]-1, srdp->numunaln);
         if (disc != -1) /* there is a left unaligned region */
         {
            AlnMgr2GetUnalignedInfo(sap, trans[start_sect]-1, amp->row_num, &intfrom, &intto);
            amp->left_interrupt = (AMInterruptPtr)MemNew(sizeof(AMInterrupt));
            amp->left_interrupt->row = amp->row_num;
            amp->left_interrupt->unalnlen = intto - intfrom + 1;
            amp->left_interrupt->segnum = trans[start_sect];
            amp->left_interrupt->which_side = AM2_LEFT;
         }
      }
      len = 0;
      for (i=start_sect; i<= stop_sect; i++)
      {
         len += dsp->lens[trans[i]];
      }
      len = len - offset - endoffset;
      if (amp->strand == Seq_strand_minus)
         amp->from_row = dsp->starts[trans[stop_sect]*dsp->dim+amp->row_num-1] + endoffset;
      else
         amp->from_row = dsp->starts[trans[start_sect]*dsp->dim+amp->row_num-1] + offset;
      amp->to_row = amp->from_row + len - 1;
      amp->real_from += amp->to_row - amp->from_row + 1;
      if (saip->anchor <= 0)
         MemFree(trans);
      return TRUE;
   }
   /* look for limits of aligned/gapped region */
   i = index;
   j = start_sect+1;
   disc = -1;
   found = FALSE;
   while (i+1<arraylen && disc == -1 && array[i] <= trans[stop_sect] && array[i+1]-1 == array[i])
   {
      disc = binary_search_on_uint2_list(srdp->unaligned, array[i], srdp->numunaln);
      if (disc == -1)
         i++;
   }
   disc = binary_search_on_uint2_list(srdp->unaligned, array[i], srdp->numunaln);
   j = binary_search_on_uint2_list(trans, array[i], translen);
   if (amp->type == AM_SEQ && j <= stop_sect) /* there is an interrupting region, either seq/gap, insert, or unaligned, plus just check last piece */
   {
      i = binary_search_on_uint2_list(srdp->insect, trans[j]+1, srdp->numinsect);
      if (i != -1) /* there's an insert */
      {
         amp->right_interrupt = (AMInterruptPtr)MemNew(sizeof(AMInterrupt));
         amp->right_interrupt->row = amp->row_num;
         amp->right_interrupt->segnum = trans[j];
         amp->right_interrupt->insertlen = dsp->lens[srdp->insect[i]];
         amp->right_interrupt->which_side = AM2_RIGHT;
         /* look for unaligned regions off insert */
         disc1 = -1;
         if (j > 0)
            disc1 = binary_search_on_uint2_list(srdp->unaligned, trans[j]+1, srdp->numunaln);
         if (disc1 != -1)
         {
            AlnMgr2GetUnalignedInfo(sap, srdp->unaligned[disc1], amp->row_num, &intfrom, &intto);
            amp->right_interrupt->unalnlen = intto - intfrom + 1;
         }
         i++;
         ctr = 1;
         while (i<srdp->numinsect && srdp->insect[i] == srdp->insect[i-1]+1)
         {
            amp->right_interrupt->insertlen += dsp->lens[srdp->insect[i]];
            /* look for unaligned regions off insert */
            disc1 = -1;
            if (j > 0) {
              disc1 = binary_search_on_uint2_list(srdp->unaligned, trans[j]+1+ctr, srdp->numunaln);
            }
            if (disc1 != -1)
            {
               AlnMgr2GetUnalignedInfo(sap, srdp->unaligned[disc1], amp->row_num, &intfrom, &intto);
               amp->right_interrupt->unalnlen += intto - intfrom + 1;
            }
            i++;
            ctr++;
         }
      }
      if (disc != -1) /* there's an unaligned region */
      {
         if (amp->right_interrupt == NULL)
            amp->right_interrupt = (AMInterruptPtr)MemNew(sizeof(AMInterrupt));
         amp->right_interrupt->row = amp->row_num;
         amp->right_interrupt->segnum = trans[j];
         amp->right_interrupt->which_side = AM2_RIGHT;
         AlnMgr2GetUnalignedInfo(sap, srdp->unaligned[disc], amp->row_num, &intfrom, &intto);
         amp->right_interrupt->unalnlen += intto - intfrom + 1;
      }
   }
   stop_sect = j;
   /* now look for left-side unaligned or inserted regions if offset == 0 */
   if (amp->type == AM_SEQ && offset == 0)
   {
      disc = -1;
      j = 1;
      i = -1;
      if ((Int2)trans[start_sect]-j > 0)
      i = binary_search_on_uint2_list(srdp->sect, trans[start_sect]-j, srdp->numsect);
      while (i == -1 && (Int2)(trans[start_sect])-j-1 >= 0)
      {
         i = binary_search_on_uint2_list(srdp->sect, trans[start_sect]-j-1, srdp->numsect);
         j++;
      }
      disc = binary_search_on_uint2_list(srdp->unaligned, trans[start_sect]-j, srdp->numunaln);;
      if (disc > -1)
      {
         AlnMgr2GetUnalignedInfo(sap, trans[start_sect]-j, amp->row_num, &intfrom, &intto);
         amp->left_interrupt = (AMInterruptPtr)MemNew(sizeof(AMInterrupt));
         amp->left_interrupt->row = amp->row_num;
         amp->left_interrupt->segnum = trans[start_sect];
         amp->left_interrupt->which_side = AM2_LEFT;
         amp->left_interrupt->unalnlen = intto - intfrom + 1;
      }
      i = binary_search_on_uint2_list(srdp->insect, trans[start_sect]-j, srdp->numinsect);
      if (i != -1) /* there's an insert */
      {
         if (amp->left_interrupt == NULL)
            amp->left_interrupt = (AMInterruptPtr)MemNew(sizeof(AMInterrupt));
         amp->left_interrupt->row = amp->row_num;
         amp->left_interrupt->segnum = trans[start_sect];
         amp->left_interrupt->which_side = AM2_LEFT;
         amp->left_interrupt->insertlen = dsp->lens[srdp->insect[i]];
         /* look for unaligned regions off insert */
         j = trans[start_sect]-j;
         disc1 = binary_search_on_uint2_list(srdp->unaligned, j, srdp->numunaln);
         if (disc1 != -1)
         {
            AlnMgr2GetUnalignedInfo(sap, srdp->unaligned[disc1], amp->row_num, &intfrom, &intto);
            amp->left_interrupt->unalnlen += intto - intfrom + 1;
         }
         i--;
         j--;
         while (i-1>=0 && srdp->insect[i] == srdp->insect[i+1]-1)
         {
            amp->left_interrupt->insertlen += dsp->lens[srdp->insect[i]];
            disc1 = binary_search_on_uint2_list(srdp->unaligned, j, srdp->numunaln);
            if (disc1 != -1)
            {
               AlnMgr2GetUnalignedInfo(sap, srdp->unaligned[disc1], amp->row_num, &intfrom, &intto);
               amp->left_interrupt->unalnlen += intto - intfrom + 1;
            }
            i--;
            j--;
         }
         if (i>=0) /* look one more over for unaligned */
         {
            disc1 = binary_search_on_uint2_list(srdp->unaligned, j, srdp->numunaln);
            if (disc1 != -1)
            {
               AlnMgr2GetUnalignedInfo(sap, srdp->unaligned[disc1], amp->row_num, &intfrom, &intto);
               amp->left_interrupt->unalnlen += intto - intfrom + 1;
            }
         }
      }
   }
   endoffset = dsp->lens[trans[stop_sect]] - (amp->to_aln - saip->aligncoords[stop_sect]) - 1;
   if (endoffset < 0)
      endoffset = 0;
   if (amp->right_interrupt != NULL && endoffset > 0)
   {
      MemFree(amp->right_interrupt);
      amp->right_interrupt = NULL;
   }
   len = 0;
   for (i=start_sect; i<=stop_sect; i++)
   {
      len += dsp->lens[trans[i]];
   }
   len = len - offset - endoffset;
   if (amp->type == AM_GAP)
   {
      amp->from_row = amp->real_from;
      amp->to_row = amp->from_row + len - 1;
   } else
   {
      if (amp->strand == Seq_strand_minus)
      {
         amp->from_row = dsp->starts[trans[stop_sect]*dsp->dim+amp->row_num-1] + endoffset;
         amp->to_row = amp->from_row + len - 1;
      } else
      {
         amp->from_row = dsp->starts[trans[start_sect]*dsp->dim+amp->row_num-1] + offset;
         amp->to_row = amp->from_row + len - 1;
      }
   }
   if (saip->anchor <= 0)
      MemFree(trans);
   amp->real_from += amp->to_row - amp->from_row + 1;
   return TRUE;
}

/* SECTION 4a */
static Int4 binary_search_on_uint4_list(Uint4Ptr list, Uint4 pos, Uint4 listlen)
{
   Uint4  L;
   Uint4  mid;
   Uint4  R;

   if (list == NULL || listlen == 0)
      return 0;
   L = 0;
   R = listlen - 1;
   while (L < R)
   {
      mid = (L+R)/2;
      if (list[mid + 1] <= pos)
         L = mid + 1;
      else
         R = mid;
   }
   return R;
}

/* SECTION 4a */
static Int4 binary_search_on_uint2_list(Uint2Ptr list, Int4 ele, Uint2 listlen)
{
   Uint2  L;
   Uint2  mid;
   Uint2  R;

   if (list == NULL || listlen == 0 || ele < 0)
      return -1;
   L = 0;
   R = listlen - 1;
   while (L < R)
   {
      mid = (L+R)/2;
      if (ele <= list[mid])
         R = mid;
      else
         L = mid+1;
   }
   if (ele == list[R])
      return R;
   else
      return -1;
}

/* SECTION 4a */
static void AlnMgr2GetUnalignedInfo(SeqAlignPtr sap, Int4 segment, Int4 row, Int4Ptr from, Int4Ptr to)
{
   AMAlignIndex2Ptr  amaip;
   DenseSegPtr      dsp;
   Boolean          found;
   Int4             i;
   SAIndex2Ptr       saip;
   Uint1            strand;
   Int4             tmp;

   if (sap == NULL)
      return;
   strand = AlnMgr2GetNthStrand(sap, row);
   if (sap->saip->indextype == INDEX_CHILD)
   {
      saip = (SAIndex2Ptr)(sap->saip);
      dsp = (DenseSegPtr)(sap->segs);
   } else if (sap->saip->indextype == INDEX_PARENT)
   {
      amaip = (AMAlignIndex2Ptr)(sap->saip);
      saip = (SAIndex2Ptr)(amaip->sharedaln->saip);
      dsp = (DenseSegPtr)(amaip->sharedaln->segs);
   }
   found = FALSE;
   *from = *to = -1;
   for (i=segment; i>=0 && !found; i--)
   {
      if (dsp->starts[dsp->dim*i+row-1] != -1)
      {
         found = TRUE;
         if (strand == Seq_strand_minus)
            *to = dsp->starts[dsp->dim*i+row-1]-1;
         else
            *from = dsp->starts[dsp->dim*i+row-1]+dsp->lens[i];
      }
   }
   found = FALSE;
   for (i=segment+1; i<dsp->numseg && !found; i++)
   {
      if (dsp->starts[dsp->dim*i+row-1] != -1)
      {
         found = TRUE;
         if (strand == Seq_strand_minus)
            *from = dsp->starts[dsp->dim*i+row-1]+dsp->lens[i];
         else
            *to = dsp->starts[dsp->dim*i+row-1]-1;
      }
   }
   if (*from > *to)
   {
      tmp = *from;
      *from = *to;
      *to = tmp;
   }
}

/* SECTION 4a */
/***************************************************************************
*
*  AlnMgr2GetInterruptInfo returns a structure describing the inserts and
*  unaligned regions in an interrupt. The structure is allocated by this
*  function and must be freed with AlnMgr2FreeInterruptInfo.
*
***************************************************************************/
NLM_EXTERN AMInterrInfoPtr AlnMgr2GetInterruptInfo(SeqAlignPtr sap, AMInterruptPtr interrupt)
{
   AMAlignIndex2Ptr  amaip;
   Int4              disc;
   Boolean           done;
   DenseSegPtr       dsp;
   Int4              i;
   AMInterrInfoPtr   iip;
   Int4              inserts;
   Int4              intfrom;
   Int4              intto;
   Int4              j;
   Int4              k;
   Int4              n;
   SAIndex2Ptr       saip;
   SARowDat2Ptr      srdp;
   Uint1             strand;
   Uint2Ptr          trans;
   Int4              translen;
   Int4              u;

   if (interrupt == NULL || sap == NULL || sap->saip == NULL)
      return NULL;
   if (sap->saip->indextype == INDEX_CHILD)
   {
      dsp = (DenseSegPtr)(sap->segs);
      saip = (SAIndex2Ptr)(sap->saip);
   } else if (sap->saip->indextype == INDEX_PARENT)
   {
      amaip = (AMAlignIndex2Ptr)(sap->saip);
      if (amaip->alnstyle == AM2_LITE)
         return FALSE;
      dsp = (DenseSegPtr)(amaip->sharedaln->segs);
      saip = (SAIndex2Ptr)(amaip->sharedaln->saip);
   }
   if (dsp->numseg < interrupt->segnum)
      return NULL;
   if (saip->anchor > 0)
   {
      trans = saip->srdp[saip->anchor-1]->sect;
      translen = saip->srdp[saip->anchor-1]->numsect;
   } else
   {
      trans = (Uint2Ptr)MemNew(dsp->numseg*sizeof(Uint2));
      for (i=0; i<dsp->numseg; i++)
      {
         trans[i] = i;
      }
      translen = dsp->numseg;
   }
   strand = AlnMgr2GetNthStrand(sap, interrupt->row-1);
   srdp = saip->srdp[interrupt->row-1];
   /* now look for inserts and unaligned regions on the side indicated */
   if (interrupt->which_side == AM2_RIGHT)
   {
      /* check if this is unaligned */
      disc = binary_search_on_uint2_list(srdp->unaligned, interrupt->segnum, srdp->numunaln);
      /* then look for inserts */
      done = FALSE;
      iip = (AMInterrInfoPtr)MemNew(sizeof(AMInterrInfo));
      if (disc != -1)
         iip->num = 1;
      inserts = 0;
      for (i=interrupt->segnum+1; !done; i++)
      {
         n = binary_search_on_uint2_list(srdp->insect, i, srdp->numinsect);
         if (n == -1)
            n = binary_search_on_uint2_list(srdp->unsect, i, srdp->numunsect);
         if (n == -1)
         {
            done = TRUE;
         } else
         {
            inserts++; /* only increment if region gets interrupted */
            disc = binary_search_on_uint2_list(srdp->unaligned, i, srdp->numunaln);
            if (disc != -1) /* this insert has an unaligned region */
            {
               iip->num += inserts;
               iip->num++;
               inserts = 0;
            }
         }
      }
      if (inserts != 0)
         iip->num++;
      iip->starts = (Int4Ptr)MemNew(iip->num*sizeof(Int4));
      iip->lens = (Int4Ptr)MemNew(iip->num*sizeof(Int4));
      iip->types = (Int4Ptr)MemNew(iip->num*sizeof(Int4));
      k = 0;
      disc = binary_search_on_uint2_list(srdp->unaligned, interrupt->segnum, srdp->numunaln);
      if (disc != -1) /* starts with unaligned */
      {
         AlnMgr2GetUnalignedInfo(sap, interrupt->segnum, interrupt->row, &intfrom, &intto);
         iip->starts[k] = intfrom;
         iip->lens[k] = intto - intfrom + 1;
         iip->types[k] = AM_UNALIGNED;
         k++;
      }
      disc = 0;
      done = FALSE;
      for (i=interrupt->segnum+1; !done; i++)
      {
         n = binary_search_on_uint2_list(srdp->insect, i, srdp->numinsect);
         u = binary_search_on_uint2_list(srdp->unsect, i, srdp->numinsect);
         if (n == -1 && u == -1)
         {
            done = TRUE;
         } else
         {
            if (u == -1)
            {
               if (disc != -1 || strand == Seq_strand_minus) /* only record new start if region gets interrupted or if on minus strand */
                  iip->starts[k] = dsp->starts[dsp->dim*i + interrupt->row-1];
               iip->lens[k] += dsp->lens[i];
               iip->types[k] = AM_INSERT;
               disc = binary_search_on_uint2_list(srdp->unaligned, i, srdp->numunaln);
               if (disc != -1) /* this insert has an unaligned region */
               {
                  k++;
                  AlnMgr2GetUnalignedInfo(sap, i, interrupt->row, &intfrom, &intto);
                  iip->starts[k] = intfrom;
                  iip->lens[k] = intto - intfrom + 1;
                  iip->types[k] = AM_UNALIGNED;
                  k++;
               }
            }
         }
      }
   } else if (interrupt->which_side == AM2_LEFT)
   {
      /* check if the next non-gap segment to the left has unaligned */
      j = 1;
      n = 0;
      while (n != -1 && interrupt->segnum-j >= 0)
      {
         n = binary_search_on_uint2_list(srdp->unsect, interrupt->segnum-j, srdp->numunsect);
         if (n == -1)
            n = binary_search_on_uint2_list(srdp->insect, interrupt->segnum-j, srdp->numinsect);
         if (n != -1)
            j++;
      }
      disc = binary_search_on_uint2_list(srdp->unaligned, interrupt->segnum-j, srdp->numunaln);
      /* then look for inserts */
      done = FALSE;
      iip = (AMInterrInfoPtr)MemNew(sizeof(AMInterrInfo));
      if (disc != -1)
         iip->num = 1;
      inserts = 0;
      for (i=interrupt->segnum-1; !done; i--)
      {
         n = binary_search_on_uint2_list(srdp->insect, i, srdp->numinsect);
         if (n == -1)
            n = binary_search_on_uint2_list(srdp->unsect, i, srdp->numunsect);
         if (n == -1)
         {
            done = TRUE;
         } else
         {
            inserts++; /* only increment if region gets interrupted */
            disc = binary_search_on_uint2_list(srdp->unaligned, i, srdp->numunaln);
            if (disc != -1) /* this insert has an unaligned region */
            {
               iip->num += inserts;
               iip->num++;
               inserts = 0;
            }
         }
      }
      i++;
      iip->starts = (Int4Ptr)MemNew(iip->num*sizeof(Int4));
      iip->lens = (Int4Ptr)MemNew(iip->num*sizeof(Int4));
      iip->types = (Int4Ptr)MemNew(iip->num*sizeof(Int4));
      k = 0;
      disc = 0;
      /* check first non-inserted segment for unaligned */
      if (i >= 0)
      {
         disc = binary_search_on_uint2_list(srdp->unaligned, i, srdp->numunaln);
         if (disc != -1) /* there's an unaligned region */
         {
            AlnMgr2GetUnalignedInfo(sap, i, interrupt->row, &intfrom, &intto);
            iip->starts[k] = intfrom;
            iip->lens[k] = intto - intfrom + 1;
            iip->types[k] = AM_UNALIGNED;
            k++;
         }
      }
      i++; /* start from leftmost end of inserts/unaligned */
      for (i; i<interrupt->segnum; i++)
      {
         u = binary_search_on_uint2_list(srdp->unsect, i, srdp->numunsect);
         if (u == -1)
         {
            if (disc != -1 || strand == Seq_strand_minus) /* only record new start if region gets interrupted or if on minus strand */
               iip->starts[k] = dsp->starts[dsp->dim*i + interrupt->row-1];
            iip->lens[k] += dsp->lens[i];
            iip->types[k] = AM_INSERT;
            disc = binary_search_on_uint2_list(srdp->unaligned, i, srdp->numunaln);
            if (disc != -1) /* this insert has an unaligned region */
            {
               k++;
               AlnMgr2GetUnalignedInfo(sap, binary_search_on_uint2_list(trans, i, translen), interrupt->row, &intfrom, &intto);
               iip->starts[k] = intfrom;
               iip->lens[k] = intto - intfrom + 1;
               iip->types[k] = AM_UNALIGNED;
               k++;
            }
         }
      }
   }
   iip->strand = strand;
   return iip;
}

/* SECTION 4b */
/***************************************************************************
*
*  AlnMgr2GetNthStrand takes an indexed seqalign and a row number and
*  returns the strand of the row indicated. A return of 0 indicates
*  an error.
*
***************************************************************************/
NLM_EXTERN Uint1 AlnMgr2GetNthStrand(SeqAlignPtr sap, Int4 n)
{
   AMAlignIndex2Ptr  amaip;
   DenseSegPtr      dsp;

   if (sap == NULL || sap->saip == NULL || n < 1)
      return 0;
   if (sap->saip->indextype == INDEX_CHILD)
   {
      dsp = (DenseSegPtr)(sap->segs);
      if (n > dsp->dim)
         return 0;
      if (dsp->strands == NULL)
         return Seq_strand_plus;
      return (dsp->strands[n-1]);
   } else if (sap->saip->indextype == INDEX_PARENT)
   {
      amaip = (AMAlignIndex2Ptr)(sap->saip);
      if (amaip->alnstyle == AM2_LITE) /* can't get Nth strand for this */
         return 0;
      dsp = (DenseSegPtr)(amaip->sharedaln->segs);
      if (n > dsp->dim)
         return 0;
      if (dsp->strands == NULL)
         return Seq_strand_plus;
      return (dsp->strands[n-1]);
   }
   return 0;
}

/* SECTION 4b */
/***************************************************************************
*
*  AlnMgr2GetNthSeqIdPtr returns the seqid (this is a duplicated,
*  allocated seqid that must be freed) of the nth row (1-based) of an
*  indexed parent or child seqalign.
*
***************************************************************************/
NLM_EXTERN SeqIdPtr AlnMgr2GetNthSeqIdPtr(SeqAlignPtr sap, Int4 n)
{
   AMAlignIndex2Ptr  amaip;
   DenseSegPtr      dsp;
   Int4             i;
   SeqIdPtr         sip;

   if (sap == NULL || sap->saip == NULL)
      return NULL;
   if (sap->saip->indextype == INDEX_CHILD)
   {
      dsp = (DenseSegPtr)(sap->segs);
      if (n > dsp->dim)
         return NULL;
      sip = dsp->ids;
      for (i=1; i<n && sip != NULL; i++)
      {
         sip = sip->next;
      }
      if (sip == NULL) return NULL;
      return (SeqIdDup(sip));
   } else if (sap->saip->indextype == INDEX_PARENT)
   {
      amaip = (AMAlignIndex2Ptr)(sap->saip);
      if (n > amaip->numrows)
         return NULL;
      sip = SeqIdDup(amaip->ids[n-1]);
      return sip;
   } else
      return NULL;
}

/* SECTION 4b */
/***************************************************************************
*
*  AlnMgr2GetNthSeqRangeInSA returns the smallest and largest sequence
*  coordinates contained in the nth row of an indexed seqalign. Either
*  start or stop can be NULL to only retrieve one of the coordinates. 
*  If start and stop are -1, there is an error; if they are both -2, the
*  row is just one big insert. RANGE
*
***************************************************************************/
NLM_EXTERN void AlnMgr2GetNthSeqRangeInSA(SeqAlignPtr sap, Int4 n, Int4Ptr start, Int4Ptr stop)
{
   AMAlignIndex2Ptr  amaip;
   Int4             beg;
   DenseSegPtr      dsp;
   Int4             end;
   SAIndex2Ptr       saip;
   SARowDat2Ptr      srdp;
   Uint1            strand;

   if (start != NULL)
      *start = -1;
   if (stop != NULL)
      *stop = -1;
   if (sap == NULL || sap->saip == NULL)
      return;
   if (sap->saip->indextype == INDEX_CHILD)
   {
      saip = (SAIndex2Ptr)(sap->saip);
      dsp = (DenseSegPtr)(sap->segs);
   } else if (sap->saip->indextype == INDEX_PARENT)
   {
      amaip = (AMAlignIndex2Ptr)(sap->saip);
      if (amaip->alnstyle == AM2_LITE)
      {
         AlnMgr2GetNthSeqRangeInSASet(sap, n, start, stop);
         return;
      }
      saip = (SAIndex2Ptr)(amaip->sharedaln->saip);
      dsp = (DenseSegPtr)(amaip->sharedaln->segs);
   }
   if (n > saip->numrows || n <= 0)
      return;
   srdp = saip->srdp[n-1];
   beg = -1;
   if (srdp->numsect == 0) /* just one big insert */
      beg = end = -2;
   strand = AlnMgr2GetNthStrand(sap, n);
   if (beg != -2 && strand != Seq_strand_minus)
   {
      beg = dsp->starts[srdp->sect[0]*(dsp->dim) + n-1];
      end = dsp->starts[srdp->sect[srdp->numsect-1]*(dsp->dim) + n-1] + dsp->lens[srdp->sect[srdp->numsect-1]] - 1;
   } else if (beg != -2)
   {
      beg = dsp->starts[srdp->sect[srdp->numsect-1]*(dsp->dim) + n-1];
      end = dsp->starts[srdp->sect[0]*(dsp->dim) + n-1] + dsp->lens[srdp->sect[0]] - 1;
   }
   if (start != NULL)
      *start = beg;
   if (stop != NULL)
      *stop = end;
   return;
}

/* SECTION 4b */
/***************************************************************************
*
*  AlnMgr2GetNthRowSpanInSA returns the least and greatest alignment
*  coordinates (inclusive) spanned by the indicated row. Either stop or
*  start can be NULL to retrieve just one of the coordinates.
*
***************************************************************************/
NLM_EXTERN void AlnMgr2GetNthRowSpanInSA(SeqAlignPtr sap, Int4 n, Int4Ptr start, Int4Ptr stop)
{
   AMAlignIndex2Ptr  amaip;
   DenseSegPtr      dsp;
   Int4             i;
   SAIndex2Ptr       saip;
   SARowDat2Ptr      srdp;

   if (start != NULL)
      *start = -1;
   if (stop != NULL)
      *stop = -1;
   if (sap == NULL || sap->saip == NULL)
      return;
   if (sap->saip->indextype == INDEX_CHILD)
   {
      saip = (SAIndex2Ptr)(sap->saip);
      dsp = (DenseSegPtr)(sap->segs);
   } else if (sap->saip->indextype == INDEX_PARENT)
   {
      amaip = (AMAlignIndex2Ptr)(sap->saip);
      if (amaip->alnstyle == AM2_LITE)
         return;
      saip = (SAIndex2Ptr)(amaip->sharedaln->saip);
      dsp = (DenseSegPtr)(amaip->sharedaln->segs);
   }
   if (n > saip->numrows || n <= 0)
      return;
   srdp = saip->srdp[n-1];
   if (srdp->numsect == 0)
   {
      if (start != NULL)
         *start = -1;
      if (stop != NULL)
         *stop = -1;
      return;
   }
   if (start != NULL)
   {
      if (saip->anchor > 0)
         i = binary_search_on_uint2_list(saip->srdp[saip->anchor-1]->sect, srdp->sect[0], saip->srdp[saip->anchor-1]->numsect);
      else
         i = srdp->sect[0];
      *start = saip->aligncoords[i];
   }
   if (stop != NULL)
   {
      if (saip->anchor > 0)
         i = binary_search_on_uint2_list(saip->srdp[saip->anchor-1]->sect, srdp->sect[srdp->numsect-1], saip->srdp[saip->anchor-1]->numsect);
      else
         i = srdp->sect[srdp->numsect-1];
      *stop = saip->aligncoords[i] + dsp->lens[srdp->sect[srdp->numsect-1]] - 1;
   }
   return;
}

/* SECTION 4b */
static void AlnMgr2GetNthSeqRangeInSASet(SeqAlignPtr sap, Int4 n, Int4Ptr start, Int4Ptr stop)
{
   AMAlignIndex2Ptr  amaip;
   Int4              from;
   Int4              i;
   Int4              max;
   Int4              min;
   Int4              to;

   if (start != NULL)
      *start = -1;
   if (stop != NULL)
      *stop = -1;
   if (sap == NULL || sap->saip == NULL || n < 0)
      return;
   if (sap->saip->indextype == INDEX_CHILD)
   {
      AlnMgr2GetNthSeqRangeInSA(sap, n, start, stop);
      return;
   }
   amaip = (AMAlignIndex2Ptr)(sap->saip);
   min = max = -1;
   for (i=0; i<amaip->numsaps; i++)
   {
      AlnMgr2GetNthSeqRangeInSA(amaip->saps[i], n, &from, &to);
      if (from != -1 && (from < min || min == -1))
         min = from;
      if (to > max)
         max = to;
   }
   if (start != NULL)
      *start = from;
   if (stop != NULL)
      *stop = to;
}

/* SECTION 4b */
NLM_EXTERN Int4 AlnMgr2GetMaxTailLength(SeqAlignPtr sap, Uint1 which_tail)
{
   Int4   i;
   Int4   maxlen;
   Int4   n;
   Int4   start;
   Int4   stop;
   Uint1  strand;

   if (sap == NULL || sap->saip == NULL)
      return 0;
   n = AlnMgr2GetNumRows(sap);
   maxlen = -1;
   for (i=0; i<n; i++)
   {
      AlnMgr2GetNthRowTail(sap, i+1, which_tail, &start, &stop, &strand);
      if (stop - start + 1 > maxlen)
         maxlen = stop - start + 1;
   }
   return maxlen;
}

/* SECTION 4b */
/***************************************************************************
*
*  AlnMgr2GetNthRowTail returns the sequence extremities that are not
*  contained in the alignment (if the alignment starts at 10 in row 2, the
*  tail in that row is 0-9). It takes an indexed seqalign, a 1-based row
*  number, and AM2_LEFT_TAIL or AM2_RIGHT_TAIL, and returns the start, stop,
*  and strand of the tail indicated in the row desired. AlnMgr2GetNthRowTail
*  returns TRUE if the calculations were successfully completed.
*
***************************************************************************/
NLM_EXTERN Boolean AlnMgr2GetNthRowTail(SeqAlignPtr sap, Int4 n, Uint1 which_tail, Int4Ptr start, Int4Ptr stop, Uint1Ptr strand)
{
   BioseqPtr  bsp;
   SeqIdPtr   sip;
   Int4       tmp_start;
   Int4       tmp_stop;
   Uint1      tmp_strand;

   if (sap == NULL || n < 1 || sap->saip == NULL)
      return FALSE;
   tmp_start = tmp_stop = -1;
   AlnMgr2GetNthSeqRangeInSA(sap, n, &tmp_start, &tmp_stop);
   if (tmp_start == -1 || tmp_stop == -1)
      return FALSE;
   tmp_strand = AlnMgr2GetNthStrand(sap, n);
   if (which_tail == AM2_LEFT_TAIL)
   {
      if (tmp_strand == Seq_strand_minus)
      {
         sip = AlnMgr2GetNthSeqIdPtr(sap, n);
         bsp = BioseqLockById(sip);
         SeqIdFree(sip);
         if (bsp == NULL)
            return FALSE;
         if (tmp_stop == bsp->length-1 || stop == NULL)
         {
            if (start)
               *start = -1;
            if (stop)
               *stop = -1;
         } else
         {
            if (start)
               *start = tmp_stop+1;
            if (stop)
               *stop = bsp->length-1;
         }
         BioseqUnlock(bsp);
         if (strand)
            *strand = tmp_strand;
      } else
      {
         if (tmp_start >= 1)
         {
            if (start)
               *start = 0;
            if (stop)
               *stop = tmp_start - 1;
         } else
         {
            if (start)
               *start = -1;
            if (stop)
               *stop = -1;
         }
         if (strand)
            *strand = tmp_strand;
      }
   } else if (which_tail == AM2_RIGHT_TAIL)
   {
      if (tmp_strand == Seq_strand_minus)
      {
         if (tmp_start >= 1)
         {
            if (start)
               *start = 0;
            if (stop)
               *stop = tmp_start - 1;
         } else
         {
            if (start)
               *start = -1;
            if (stop)
               *stop = -1;
         }
         if (strand)
            *strand = tmp_strand;
      } else
      {
         sip = AlnMgr2GetNthSeqIdPtr(sap, n);
         bsp = BioseqLockById(sip);
         SeqIdFree(sip);
         if (bsp == NULL)
            return FALSE;
         if (bsp->length-1 == tmp_stop)
         {
            if (start)
               *start = -1;
            if (stop)
               *stop = -1;
         } else
         {
            if (start)
               *start = tmp_stop + 1;
            if (stop)
               *stop = bsp->length-1;
         }
         if (strand)
            *strand = tmp_strand;
         BioseqUnlock(bsp);
      }
   }
   return TRUE;
}

/* SECTION 4c */
/***************************************************************************
*
*  AlnMgr2GetAlnLength returns the total alignment length of an indexed
*  alignment. If fill_in is TRUE, the function computes the total length
*  of all the internal unaligned regions and adds that to the alignment
*  length; otherwise only the aligned portions are considered. (LENGTH)
*
***************************************************************************/
NLM_EXTERN Int4 AlnMgr2GetAlnLength(SeqAlignPtr sap, Boolean fill_in)
{
   AMAlignIndex2Ptr  amaip;
   DenseSegPtr      dsp;
   Int4             i;
   Uint2            lastseg;
   Int4             len;
   SAIndex2Ptr       saip;
   SeqAlignPtr      salp;

   if (sap == NULL || sap->saip == NULL)
      return -1;
   if (sap->saip->indextype == INDEX_CHILD)
   {
      dsp = (DenseSegPtr)(sap->segs);
      saip = (SAIndex2Ptr)(sap->saip);
      salp = sap;
   } else if (sap->saip->indextype == INDEX_PARENT)
   {
      amaip = (AMAlignIndex2Ptr)(sap->saip);
      if (amaip->alnstyle == AM2_LITE)
         return -1;
      saip = (SAIndex2Ptr)(amaip->sharedaln->saip);
      dsp = (DenseSegPtr)(amaip->sharedaln->segs);
      salp = amaip->sharedaln;
   }
   if (saip->unaln == FALSE || fill_in == FALSE)
   {
      if (saip->anchor == -1)
         return (saip->aligncoords[saip->numseg-1]+dsp->lens[saip->numseg-1]);
      else
      {
         lastseg = saip->srdp[saip->anchor-1]->sect[saip->srdp[saip->anchor-1]->numsect-1];
         return (saip->aligncoords[saip->numseg-1]+dsp->lens[lastseg]);
      }
   } else
   {
      len = 0;
      for (i=0; i<dsp->numseg; i++)
      {
         len += dsp->lens[i];
         len += AlnMgr2GetMaxUnalignedLength(salp, i);
      }
   }
   return len;
}

/* SECTION 4c */ /* FOR DDV */
NLM_EXTERN Boolean AlnMgr2IsSAPDiscAli(SeqAlignPtr sap)
{
   AMAlignIndex2Ptr  amaip;
   SAIndex2Ptr       saip;

   if (sap == NULL || sap->saip == NULL)
      return FALSE;
   if (sap->saip->indextype == INDEX_CHILD)
   {
      saip = (SAIndex2Ptr)(sap->saip);
   } else if (sap->saip->indextype == INDEX_PARENT)
   {
      amaip = (AMAlignIndex2Ptr)(sap->saip);
      if (amaip->alnstyle == AM2_LITE)
         return FALSE;
      saip = (SAIndex2Ptr)(amaip->sharedaln->saip);
   }
   if (saip->numunaln > 0)
      return TRUE;
   return FALSE;
}

/* SECTION 4c */ /* FOR DDV */
NLM_EXTERN Int4 AlnMgr2GetNumAlnBlocks(SeqAlignPtr sap)
{
   AMAlignIndex2Ptr  amaip;
   SAIndex2Ptr       saip;

   if (sap == NULL || sap->saip == NULL)
      return -1;
   if (sap->saip->indextype == INDEX_CHILD)
   {
      saip = (SAIndex2Ptr)(sap->saip);
   } else if (sap->saip->indextype == INDEX_PARENT)
   {
      amaip = (AMAlignIndex2Ptr)(sap->saip);
      if (amaip->alnstyle == AM2_LITE)
         return -1;
      saip = (SAIndex2Ptr)(amaip->sharedaln->saip);
   }
   if (saip->numunaln >= 0)
      return (saip->numunaln + 1);
   else if (saip->numunaln == -1)
      return 1;
   else
      return -1;
}

/* SECTION 4c */ /* FOR DDV */
NLM_EXTERN Boolean AlnMgr2GetNthBlockRange(SeqAlignPtr sap, Int4 n, Int4Ptr start, Int4Ptr stop)
{
   AMAlignIndex2Ptr  amaip;
   SAIndex2Ptr       saip;

   if (sap == NULL || sap->saip == NULL)
      return FALSE;
   if (sap->saip->indextype == INDEX_CHILD)
   {
      saip = (SAIndex2Ptr)(sap->saip);
   } else if (sap->saip->indextype == INDEX_PARENT)
   {
      amaip = (AMAlignIndex2Ptr)(sap->saip);
      if (amaip->alnstyle == AM2_LITE)
         return FALSE;
      saip = (SAIndex2Ptr)(amaip->sharedaln->saip);
   }
   if (!start || !stop)
      return FALSE;
   *start = -1;
   *stop = -1;
   if (n >= saip->numunaln)
      return FALSE;
   if (n < saip->numunaln)
   {
      *start = saip->aligncoords[saip->unaln[n-1]+1];
      *stop = saip->aligncoords[saip->unaln[n]] - 1;
   }
   return TRUE;
}

/* SECTION 4c */ /* FOR DDV */
/***************************************************************************
*
*  AlnMgr2GetNthUnalignedForNthRow returns the bioseq coordinates for the
*  requested row, in the requested unaligned region. Any error will result
*  in -1 returns for both start and stop.
*
***************************************************************************/
NLM_EXTERN Boolean AlnMgr2GetNthUnalignedForNthRow(SeqAlignPtr sap, Int4 unaligned, Int4 row, Int4Ptr start, Int4Ptr stop)
{
   AMAlignIndex2Ptr  amaip;
   DenseSegPtr       dsp;
   Int4              i;
   SAIndex2Ptr       saip;
   Int4              seg;
   Uint1             strand;

   if (sap == NULL || sap->saip == NULL)
      return FALSE;
   if (sap->saip->indextype == INDEX_CHILD)
   {
      saip = (SAIndex2Ptr)(sap->saip);
      dsp = (DenseSegPtr)(sap->segs);
   } else if (sap->saip->indextype == INDEX_PARENT)
   {
      amaip = (AMAlignIndex2Ptr)(sap->saip);
      if (amaip->alnstyle == AM2_LITE)
         return FALSE;
      saip = (SAIndex2Ptr)(amaip->sharedaln->saip);
      dsp = (DenseSegPtr)(amaip->sharedaln->segs);
   }
   if (row > saip->numrows)
      return FALSE;
   if (saip->numunaln == 0) /* not set yet */
      AlnMgr2SetUnaln(sap);
   if (saip->numunaln == -1 || unaligned > saip->numunaln)
   {
      if (start)
         *start = -1;
      if (stop)
         *stop = -1;
      return FALSE;
   }
   seg = -1;
   if (unaligned <= saip->numunaln && unaligned > 0)
      seg = saip->unaln[unaligned-1];
   if (start)
      *start = -1;
   if (stop)
      *stop = -1;
   i = binary_search_on_uint2_list(saip->srdp[row-1]->unaligned, seg, saip->srdp[row-1]->numunaln);
   if (i == -1 || saip->srdp[row-1]->unaligned[i] >= dsp->numseg-1)
      return FALSE;
   strand = AlnMgr2GetNthStrand(sap, row);
   if (strand == Seq_strand_minus)
   {
      *start = dsp->starts[(saip->srdp[row-1]->unaligned[i]+1)*dsp->dim+row-1] + dsp->lens[(saip->srdp[row-1]->unaligned[i])];
      *stop = dsp->starts[(saip->srdp[row-1]->unaligned[i])*dsp->dim+row-1] - 1;
   } else
   {
      *start = dsp->starts[(saip->srdp[row-1]->unaligned[i])*dsp->dim+row-1] + dsp->lens[(saip->srdp[row-1]->unaligned[i])];
      *stop = dsp->starts[(saip->srdp[row-1]->unaligned[i]+1)*dsp->dim+row-1] - 1;
   }
   return TRUE;
}

/* SECTION 4c */ /* FOR DDV */
/***************************************************************************
*
*  AlnMgr2GetNextLengthBit is called in a loop on an indexed alignment, with
*  seg starting at 0, to return the lengths of the aligned and unaligned
*  regions. If the length returned is negative, it's an unaligned region;
*  otherwise it's aligned.
*
***************************************************************************/
NLM_EXTERN Boolean AlnMgr2GetNextLengthBit(SeqAlignPtr sap, Int4Ptr len, Int4Ptr seg)
{
   AMAlignIndex2Ptr  amaip;
   DenseSegPtr       dsp;
   Int4              i;
   Int4              lastseg;
   Int4              maxseg;
   SAIndex2Ptr       saip;

   if (sap == NULL || sap->saip == NULL || seg == NULL)
      return FALSE;
   if (sap->saip->indextype == INDEX_CHILD)
   {
      saip = (SAIndex2Ptr)(sap->saip);
      dsp = (DenseSegPtr)(sap->segs);
   } else if (sap->saip->indextype == INDEX_PARENT)
   {
      amaip = (AMAlignIndex2Ptr)(sap->saip);
      if (amaip->alnstyle == AM2_LITE)
         return FALSE;
      saip = (SAIndex2Ptr)(amaip->sharedaln->saip);
      dsp = (DenseSegPtr)(amaip->sharedaln->segs);
   }
   if (saip->numunaln == -1) /* the whole thing is just one big aligned segment */
   {
      if (*seg != 0)
         return FALSE;
      if (saip->anchor == -1)
      {
         *len = saip->aligncoords[saip->numseg-1]+dsp->lens[saip->numseg-1];
         *seg = 1;
      } else
      {
         lastseg = saip->srdp[saip->anchor-1]->sect[saip->srdp[saip->anchor-1]->numsect-1];
         *len = saip->aligncoords[saip->numseg-1]+dsp->lens[lastseg];
         *seg = 1;
      }
      return TRUE;
   } else
   {
      if (saip->unaln == 0) /* not set */
      {
         AlnMgr2SetUnaln(sap);
         if (saip->numunaln == -1) /* no unaligned regions */
         {
            if (*seg != 0)
               return FALSE;
            if (saip->anchor == -1)
               *len = saip->aligncoords[saip->numseg-1]+dsp->lens[saip->numseg-1];
            else
            {
               lastseg = saip->srdp[saip->anchor-1]->sect[saip->srdp[saip->anchor-1]->numsect-1];
               *len = saip->aligncoords[saip->numseg-1]+dsp->lens[lastseg];
            }
            *seg = 1;
            return TRUE;
         }
      }
      if (*seg > saip->numunaln || -(*seg) > saip->numunaln)
         return FALSE;
      if (*seg >= 0)
      {
         *len = 0;
         if (*seg == 0)
            i = 0;
         else
            i = saip->unaln[*seg-1]+1;
         if (*seg < saip->numunaln)
            maxseg = saip->unaln[*seg];
         else
            maxseg = dsp->numseg-1;
         while (i<=maxseg)
         {
            (*len) += dsp->lens[i];
            i++;
         }
         *seg = -(*seg+1);
         return TRUE;
      } else
      {
         *len = -AlnMgr2GetMaxUnalignedLength(sap, saip->unaln[-(*seg)-1]);
         *seg = -(*seg);
         return TRUE;
      }
   }
}

/* SECTION 4c */
static Int4 AlnMgr2GetMaxUnalignedLength(SeqAlignPtr sap, Int4 seg)
{
   AMAlignIndex2Ptr  amaip;
   DenseSegPtr      dsp;
   Boolean          found;
   Int4             from;
   Int4             i;
   Int4             max;
   Int4             row;
   SAIndex2Ptr       saip;
   Int4             to;

   if (sap == NULL)
      return -1;
   if (sap->saip->indextype == INDEX_CHILD)
   {
      saip = (SAIndex2Ptr)(sap->saip);
      dsp = (DenseSegPtr)(sap->segs);
   } else if (sap->saip->indextype == INDEX_PARENT)
   {
      amaip = (AMAlignIndex2Ptr)(sap->saip);
      if (amaip->alnstyle == AM2_LITE)
         return -1;
      saip = (SAIndex2Ptr)(amaip->sharedaln->saip);
      dsp = (DenseSegPtr)(amaip->sharedaln->segs);
   }
   found = FALSE;
   for (row=0; row<dsp->dim && !found; row++)
   {
      for (i=0; i<saip->srdp[row]->numunaln && !found; i++)
      {
         if (saip->srdp[row]->unaligned[i] == seg)
            found = TRUE;
      }
   }
   if (!found)
      return 0;
   max = 0;
   for (i=0; i<dsp->dim; i++)
   {
      AlnMgr2GetUnalignedInfo(sap, seg, i+1, &from, &to);
      if (to - from > max)
         max = to - from;
   }
   return max;
}

/* SECTION 4c */
/***************************************************************************
*
*  AlnMgr2GetNumRows returns the number of rows in an indexed seqalign.
*
***************************************************************************/
NLM_EXTERN Int4 AlnMgr2GetNumRows(SeqAlignPtr sap)
{
   AMAlignIndex2Ptr  amaip;
   SAIndex2Ptr       saip;
   
   if (sap == NULL || sap->saip == NULL)
      return -1;
   if (sap->saip->indextype == INDEX_CHILD)
   {
      saip = (SAIndex2Ptr)(sap->saip);
      return (saip->numrows);
   } else if (sap->saip->indextype == INDEX_PARENT)
   {
      amaip = (AMAlignIndex2Ptr)(sap->saip);
      return (amaip->numrows);
   }
   return -1;
}

/* SECTION 4c */
/***************************************************************************
*
*  AlnMgr2GetNumSegs returns the number of gap- or aligned- contiguous
*  segments in the alignment (continuous or not).
*
***************************************************************************/
NLM_EXTERN Int4 AlnMgr2GetNumSegs(SeqAlignPtr sap)
{
   AMAlignIndex2Ptr  amaip;
   DenseSegPtr      dsp;

   if (sap == NULL || sap->saip == NULL)
      return -1;
   if (sap->saip->indextype == INDEX_CHILD)
   {
      dsp = (DenseSegPtr)(sap->segs);
      return dsp->numseg;
   } else if (sap->saip->indextype == INDEX_PARENT)
   {
      amaip = (AMAlignIndex2Ptr)(sap->saip);
      if (amaip->alnstyle == AM2_LITE)
         return -1;
      dsp = (DenseSegPtr)(amaip->sharedaln->segs);
      return dsp->numseg;
   }
   return -1;
}

/* SECTION 4c */
/***************************************************************************
*
*  AlnMgr2GetNumSegsInRange returns the number of alignment segments
*  spanned by the given range (partially or fully). The range is
*  given in alignment coordinates.
*
***************************************************************************/
NLM_EXTERN Int4 AlnMgr2GetNumSegsInRange(SeqAlignPtr sap, Int4 from, Int4 to, Int4Ptr start_seg)
{
   Uint4Ptr         aligncoords;
   AMAlignIndex2Ptr  amaip;
   DenseSegPtr      dsp;
   Int4             len;
   SAIndex2Ptr       saip;
   Int4             start;
   Int4             stop;

   if (start_seg != NULL)
      *start_seg = -1;
   if (sap == NULL || sap->saip == NULL)
      return -1;
   len = AlnMgr2GetAlnLength(sap, FALSE);
   if (from < 0 || to > len-1)
      return -1;
   if (sap->saip->indextype == INDEX_CHILD)
   {
      dsp = (DenseSegPtr)(sap->segs);
      saip = (SAIndex2Ptr)(sap->saip);
      aligncoords = saip->aligncoords;
   } else if (sap->saip->indextype == INDEX_PARENT)
   {
      amaip = (AMAlignIndex2Ptr)(sap->saip);
      if (amaip->alnstyle == AM2_LITE)
         return -1;
      dsp = (DenseSegPtr)(amaip->sharedaln->segs);
      saip = (SAIndex2Ptr)(sap->saip);
      aligncoords = saip->aligncoords;
   }
   if (from == 0 && to == len-1) /* whole alignment */
   {
      if (start_seg)
         *start_seg = 0;
      return dsp->numseg;
   }
   start = binary_search_on_uint4_list(aligncoords, from, dsp->numseg);
   stop = binary_search_on_uint4_list(aligncoords, to, dsp->numseg);
   if (start_seg != NULL)
      *start_seg = start;
   return (stop-start+1);
}

/* SECTION 4c */
/***************************************************************************
*
*  AlnMgr2GetNthSegmentRange returns the alignment coordinate range of the
*  Nth segment (count starts at 1) of the seqalign. start and stop are
*  optional arguments (in case only one end is desired).
*
***************************************************************************/
NLM_EXTERN void AlnMgr2GetNthSegmentRange(SeqAlignPtr sap, Int4 n, Int4Ptr start, Int4Ptr stop)
{
   AMAlignIndex2Ptr  amaip;
   Int4             i;
   SAIndex2Ptr       saip;

   if (sap == NULL || sap->saip == NULL)
      return;
   i = AlnMgr2GetNumSegs(sap);
   if (n > i || n < 0)
      return;
   if (sap->saip->indextype == INDEX_CHILD)
   {
      saip = (SAIndex2Ptr)(sap->saip);
      if (start != NULL)
         *start = saip->aligncoords[n-1];
      if (stop != NULL)
      {
         if (i > n) /* not the last segment */
            *stop = saip->aligncoords[n] - 1;
         else
            *stop = AlnMgr2GetAlnLength(sap, FALSE) - 1;
      }
      return;
   } else if (sap->saip->indextype == INDEX_PARENT)
   {
      amaip = (AMAlignIndex2Ptr)(sap->saip);
      if (amaip->alnstyle == AM2_LITE)
         return;
      saip = (SAIndex2Ptr)(amaip->sharedaln->saip);
      if (start != NULL)
         *start = saip->aligncoords[n-1];
      if (stop != NULL)
      {
         if (i > n) /* not the last segment */
            *stop = saip->aligncoords[n] - 1;
         else
            *stop = AlnMgr2GetAlnLength(sap, FALSE) - 1;
      }
      return;
   }
}

/* SECTION 4c */
/***************************************************************************
*
*  AlnMgr2GetFirstNForSip returns the first row that a seqid occurs on,
*  or -1 if the seqid is not in the alignment or if there is another
*  error.
*
***************************************************************************/
NLM_EXTERN Int4 AlnMgr2GetFirstNForSip(SeqAlignPtr sap, SeqIdPtr sip)
{
   AMAlignIndex2Ptr  amaip;
   DenseSegPtr      dsp;
   Int4             i;
   SeqIdPtr         sip_tmp;

   if (sap == NULL || sip == NULL || sap->saip == NULL)
      return -1;
   if (sap->saip->indextype == INDEX_CHILD)
   {
      dsp = (DenseSegPtr)(sap->segs);
      sip_tmp = dsp->ids;
      i = 1;
      while (sip_tmp != NULL)
      {
         if (SeqIdComp(sip, sip_tmp) == SIC_YES)
            return i;
         sip_tmp = sip_tmp->next;
         i++;
      }
   } else if (sap->saip->indextype == INDEX_PARENT)
   {
      amaip = (AMAlignIndex2Ptr)(sap->saip);
      if (amaip->alnstyle == AM2_LITE)
         return -1;
      for (i=0; i<amaip->numrows; i++)
      {
         if (SeqIdComp(sip, amaip->ids[i]) == SIC_YES)
            return (i+1);
      }
   }
   return -1;
}

/***************************************************************************
*
*  AlnMgr2GetFirstNForSipList returns the first row that one of a list of seqids occur on,
*  or -1 if none of the seqids are in the alignment or if there is another
*  error. 
*  Handy if sip comes from a BioSeq, where it can point to a linked list
*  of SeqIds.
*
***************************************************************************/
NLM_EXTERN Int4 AlnMgr2GetFirstNForSipList(SeqAlignPtr sap, SeqIdPtr sip)
{
    Int4    i;
    if (sap == NULL || sap->saip == NULL)
        return -1;
    
    for (; sip; sip = sip->next) {
        i = AlnMgr2GetFirstNForSip(sap, sip);
        if (i != -1)
            return i;
    }
    return -1;
}

/***************************************************************************
*
*  AlnMgr2GetParent returns the top-level seqalign associated with a given
*  indexed alignment. It returns the actual pointer, not a copy.
*
***************************************************************************/
NLM_EXTERN SeqAlignPtr AlnMgr2GetParent(SeqAlignPtr sap)
{
   SAIndex2Ptr  saip;

   if (sap == NULL || sap->saip == NULL)
      return NULL;
   if (sap->saip->indextype == INDEX_PARENT)
      return sap;
   saip = (SAIndex2Ptr)(sap->saip);
   return (saip->top);
}

/***************************************************************************
*
*  SECTION 5: Functions to change, assign or retrieve an anchor row.
*    SECTION 5a: functions for child seqaligns
*    SECTION 5b: functions for parent seqaligns
*    SECTION 5c: functions to retrieve anchor row information
*
***************************************************************************/

/* SECTION 5a */
static void AlnMgr2AnchorChild(SeqAlignPtr sap, Int4 which_row)
{
   AMBitty2Ptr  abp;
   AMBitty2Ptr  abp_head;
   AMBitty2Ptr  abp_head2;
   AMBitty2Ptr  abp_prev;
   AMBitty2Ptr  abp_prev2;
   AMBitty2Ptr  abp_uhead;
   AMBitty2Ptr  abp_uprev;
   Uint2Ptr     anchor_unsect;
   Int4         curr;
   Int4         curr2;
   DenseSegPtr  dsp;
   Int4         i;
   Int4         j;
   Uint2        numunsect;
   SAIndex2Ptr   saip;
   SARowDat2Ptr  srdp;

   if (sap == NULL || sap->saip == NULL || sap->saip->indextype != INDEX_CHILD)
      return;
   saip = (SAIndex2Ptr)(sap->saip);
   if (which_row > saip->numrows)
      return;
   if (saip->anchor == which_row) /*already anchored to the right row */
      return;
   if (saip->anchor != -1 || which_row <= 0)  /* already anchored -- must reindex as a flat alignment first */
   {
      SAIndex2Free2(sap->saip);
      sap->saip = NULL;
      AlnMgr2IndexSingleDenseSegSA(sap);
      if (which_row <= 0)
         return;
      saip = (SAIndex2Ptr)(sap->saip);
   }
   numunsect = saip->srdp[which_row-1]->numunsect;
   if (numunsect > 0)
      anchor_unsect = saip->srdp[which_row-1]->unsect;
   else
      anchor_unsect = NULL;
   for (i=0; i<saip->numrows; i++)
   {
      if (i+1 != which_row)
      {
         abp_head = NULL;
         abp_head2 = NULL;
         abp_uhead = NULL;
         curr = 0;
         curr2 = 0;
         srdp = saip->srdp[i];
         for (j=0; j<srdp->numsect; j++)
         {
            if (anchor_unsect != NULL && curr < numunsect && srdp->sect[j] > anchor_unsect[curr])
            {
               while (curr < numunsect && srdp->sect[j] > anchor_unsect[curr])
               {
                  curr++;
               }
            }
            if (curr < numunsect && anchor_unsect != NULL && srdp->sect[j] == anchor_unsect[curr]) /* this one is an insert */
            {
               abp = (AMBitty2Ptr)MemNew(sizeof(AMBitty2));
               abp->n = srdp->sect[j];
               if (abp_head == NULL)
                  abp_head = abp_prev = abp;
               else
               {
                  abp_prev->next = abp;
                  abp_prev = abp;
               }
               curr++;
            } else /* put it in the keeper pile */
            {
               abp = (AMBitty2Ptr)MemNew(sizeof(AMBitty2));
               abp->n = srdp->sect[j];
               if (abp_head2 == NULL)
                  abp_head2 = abp_prev2 = abp;
               else
               {
                  abp_prev2->next = abp;
                  abp_prev2 = abp;
               }
            }
         }
         for (j=0; j<srdp->numunsect; j++)
         {
            if (anchor_unsect != NULL && curr2 < numunsect && srdp->unsect[j] > anchor_unsect[curr2])
            {
               while (curr2 < numunsect && srdp->unsect[j] > anchor_unsect[curr2])
               {
                  curr2++;
               }
            }
            if (curr2 >= numunsect || (curr2 < numunsect && (anchor_unsect == NULL || srdp->unsect[j] != anchor_unsect[curr2]))) /* these get kept */
            {
               abp = (AMBitty2Ptr)MemNew(sizeof(AMBitty2));
               abp->n = srdp->unsect[j];
               if (abp_uhead == NULL)
                  abp_uhead = abp_uprev = abp;
               else
               {
                  abp_uprev->next = abp;
                  abp_uprev = abp;
               }
            }
         }
         MemFree(srdp->sect);
         MemFree(srdp->unsect);
         srdp->numsect = srdp->numunsect = srdp->numinsect = 0;
         abp = abp_head; /* inserts */
         while (abp != NULL)
         {
            srdp->numinsect++;
            abp = abp->next;
         }
         srdp->insect = (Uint2Ptr)MemNew((srdp->numinsect)*sizeof(Uint2));
         abp = abp_head;
         j = 0;
         while (abp != NULL)
         {
            srdp->insect[j] = abp->n;
            j++;
            abp_prev = abp;
            abp = abp->next;
            MemFree(abp_prev);
         }
         abp = abp_head2; /* aligned sections */
         while (abp != NULL)
         {
            srdp->numsect++;
            abp = abp->next;
         }
         srdp->sect = (Uint2Ptr)MemNew((srdp->numsect)*sizeof(Uint2));
         abp = abp_head2;
         j = 0;
         while (abp != NULL)
         {
            srdp->sect[j] = abp->n;
            j++;
            abp_prev = abp;
            abp = abp->next;
            MemFree(abp_prev);
         }
         abp = abp_uhead; /* aligned gaps */
         while (abp != NULL)
         {
            srdp->numunsect++;
            abp = abp->next;
         }
         srdp->unsect = (Uint2Ptr)MemNew((srdp->numunsect)*sizeof(Uint2));
         abp = abp_uhead;
         j = 0;
         while (abp != NULL)
         {
            srdp->unsect[j] = abp->n;
            j++;
            abp_prev = abp;
            abp = abp->next;
            MemFree(abp_prev);
         }
      } else /* this is the anchor row -- fill in the alignment coords*/
      {
         srdp = saip->srdp[i];
         MemFree(saip->aligncoords);
         saip->numseg = srdp->numsect;
         saip->aligncoords = (Uint4Ptr)MemNew((saip->numseg)*sizeof(Uint4));
         dsp = (DenseSegPtr)(sap->segs);
         for (j=1; j<saip->numseg; j++)
         {
            saip->aligncoords[j] = saip->aligncoords[j-1] + dsp->lens[srdp->sect[j-1]];
         }
         saip->anchor = i+1;
      }
   }
}

/* SECTION 5c */
/***************************************************************************
*
*  AlnMgr2AnchorSeqAlign takes an indexed seqalign and a row (1-based) and
*  reindexes the alignment so that there are no gaps in the row indicated.
*  Other rows may contain inserts after this operation. After an alignment
*  is anchored, its length often shrinks. If which_row is less than 1, the
*  function reindexes the alignment as a flat alignment.
*
***************************************************************************/
NLM_EXTERN void AlnMgr2AnchorSeqAlign(SeqAlignPtr sap, Int4 which_row)
{
   AMAlignIndex2Ptr  amaip;

   if (sap == NULL || sap->saip == NULL)
      return;
   if (sap->saip->indextype == INDEX_CHILD)
      AlnMgr2AnchorChild(sap, which_row);
   else if (sap->saip->indextype == INDEX_PARENT)
   {
      amaip = (AMAlignIndex2Ptr)(sap->saip);
      if (amaip->alnstyle == AM2_LITE)
         return;
      AlnMgr2AnchorChild(amaip->sharedaln, which_row);
      amaip->anchor = which_row;
   }
}

/* SECTION 5c */
/***************************************************************************
*
*  AlnMgr2FindAnchor returns the row number (1-based) of the anchor row
*  for an indexed seqalign, or -1 if the alignment is unanchored or if
*  there is another type of error.
*
***************************************************************************/
NLM_EXTERN Int4 AlnMgr2FindAnchor(SeqAlignPtr sap)
{
   AMAlignIndex2Ptr  amaip;
   SAIndex2Ptr       saip;
   
   if (sap == NULL || sap->saip == NULL)
      return -1;
   if (sap->saip->indextype == INDEX_CHILD)
   {
      saip = (SAIndex2Ptr)(sap->saip);
      return (saip->anchor);
   } else if (sap->saip->indextype == INDEX_PARENT)
   {
      amaip = (AMAlignIndex2Ptr)(sap->saip);
      if (amaip->alnstyle == AM2_LITE)
         return -1;
      saip = (SAIndex2Ptr)(amaip->sharedaln->saip);
      return (saip->anchor);
   } else
      return -1;
}

/***************************************************************************
*
*  SECTION 6: Functions for coordinate conversion (bioseq to seqalign
*  coordinates and vice versa)
*
***************************************************************************/

/* SECTION 6 */
/***************************************************************************
*
*  AlnMgr2MapBioseqToSeqAlign takes an indexed seqalign, a position in a
*  row of the alignment, and a 1-based row number, and maps the row position
*  to alignment coordinates.
*
***************************************************************************/
NLM_EXTERN Int4 AlnMgr2MapBioseqToSeqAlign(SeqAlignPtr sap, Int4 pos, Int4 row)
{
   AMAlignIndex2Ptr  amaip;
   Uint2Ptr         array;
   DenseSegPtr      dsp;
   Int4             L;
   Int4             mid;
   Int4             offset;
   Int4             R;
   Int4             retval;
   SAIndex2Ptr       saip;
   SARowDat2Ptr      srdp;
   Int4             start;
   Int4             stop;
   Uint1            strand;

   if (sap == NULL || sap->saip == NULL || row < 1)
      return -1;
   AlnMgr2GetNthSeqRangeInSA(sap, row, &start, &stop);
   if (pos < start || pos > stop)
      return -1;
   if (sap->saip->indextype == INDEX_CHILD)
   {
      saip = (SAIndex2Ptr)(sap->saip);
      dsp = (DenseSegPtr)(sap->segs);
   } else if (sap->saip->indextype == INDEX_PARENT)
   {
      amaip = (AMAlignIndex2Ptr)(sap->saip);
      if (amaip->alnstyle == AM2_LITE)
         return -1;
      saip = (SAIndex2Ptr)(amaip->sharedaln->saip);
      dsp = (DenseSegPtr)(amaip->sharedaln->segs);
}
   if (row > saip->numrows)
      return -1;
   srdp = saip->srdp[row-1];
   strand = AlnMgr2GetNthStrand(sap, row);
   L = 0;
   R = srdp->numsect - 1;
   if (strand != Seq_strand_minus)
   {
      while (L < R)
      {
         mid = (L + R)/2;
         if (dsp->starts[(srdp->sect[mid+1])*(dsp->dim)+row-1] <= pos)
            L = mid + 1;
         else
            R = mid;
      }
   } else
   {
      while (L < R)
      {
         mid = ceil((L + R)/2);
         if (dsp->starts[(srdp->sect[mid])*(dsp->dim)+row-1] > pos)
            L = mid + 1;
         else
            R = mid;
      }
   }
   offset = pos - dsp->starts[(srdp->sect[L])*(dsp->dim)+row-1];
   if (offset > dsp->lens[srdp->sect[L]])
      return -2;  /* this is an insert */
   if (saip->anchor > 0)
   {
      array = saip->srdp[saip->anchor-1]->sect;
      R = binary_search_on_uint2_list(array, srdp->sect[L], saip->srdp[saip->anchor-1]->numsect);
      L = R;
      srdp = saip->srdp[saip->anchor-1];
      if (strand != Seq_strand_minus)
         retval = (saip->aligncoords[L] + offset);
      else
         retval = (saip->aligncoords[L] + dsp->lens[srdp->sect[L]] - offset - 1);
   } else
   {
      if (strand != Seq_strand_minus)
         retval = saip->aligncoords[srdp->sect[L]] + offset;
      else
         retval = (saip->aligncoords[srdp->sect[L]] + dsp->lens[srdp->sect[L]] - offset - 1);
   }
   return retval;
}

/* SECTION 6 */
/***************************************************************************
*
*  AlnMgr2MapSeqAlignToBioseq takes an indexed seqalign, an alignment
*  coordinate (pos), and the 1-based number of a row, and maps the alignment
*  coordinate to the corresponding bioseq coordinate of the row desired.
*  A return of -1 indicates an error; a return of -2 means that the bioseq
*  is gapped at this alignment position.
*
***************************************************************************/
NLM_EXTERN Int4 AlnMgr2MapSeqAlignToBioseq(SeqAlignPtr sap, Int4 pos, Int4 row)
{
   AMAlignIndex2Ptr  amaip;
   DenseSegPtr      dsp;
   Int4             len;
   Int4             offset;
   SAIndex2Ptr      saip;
   Int4             sect;
   SARowDat2Ptr     srdp;
   Int4             start;
   Uint1            strand;
   Uint2Ptr         trans;

   if (sap == NULL || sap->saip == NULL)
      return -1;
   len = AlnMgr2GetAlnLength(sap, FALSE);
   if (pos < 0 || pos > len - 1)
      return -1;
   if (sap->saip->indextype == INDEX_CHILD)
   {
      saip = (SAIndex2Ptr)(sap->saip);
      dsp = (DenseSegPtr)(sap->segs);
   } else if (sap->saip->indextype == INDEX_PARENT)
   {
      amaip = (AMAlignIndex2Ptr)(sap->saip);
      if (amaip->alnstyle == AM2_LITE)
         return -1;
      saip = (SAIndex2Ptr)(amaip->sharedaln->saip);
      dsp = (DenseSegPtr)(amaip->sharedaln->segs);
   }
   if (row > saip->numrows)
   return -1;

   sect = binary_search_on_uint4_list(saip->aligncoords, pos, saip->numseg);
   offset = pos - saip->aligncoords[sect];
   if (saip->anchor > 0)
   {
      trans = saip->srdp[saip->anchor-1]->sect;
      sect = trans[sect];
   }
   srdp = saip->srdp[row-1];
   start = binary_search_on_uint2_list(srdp->sect, sect, srdp->numsect);
   if (start == -1)
      return -2; /* this row has a gap or insert at this alignment position */
   strand = AlnMgr2GetNthStrand(sap, row);
   if (strand != Seq_strand_minus)
      return (dsp->starts[sect*(dsp->dim)+row-1] + offset);
   else
      return (dsp->starts[sect*(dsp->dim)+row-1] + dsp->lens[sect] - 1 - offset);
}

/* SECTION 6 */
/***************************************************************************
*
*  AlnMgr2MapRowToRow takes an indexed seqalign, a position in row1, the
*  1-based number of row1, and a target row (row2), and maps the bioseq
*  coordinate in row 1 to the corresponding (aligned) bioseq coordinate in
*  row2. A return of -1 indicates an error while a return of -2 means that
*  the bioseq in row2 is gapped at the desired position.
*
***************************************************************************/
NLM_EXTERN Int4 AlnMgr2MapRowToRow(SeqAlignPtr sap, Int4 pos, Int4 row1, Int4 row2)
{
   Int4  alnpos;

   if (sap == NULL)
      return -1;
   alnpos = AlnMgr2MapBioseqToSeqAlign(sap, pos, row1);
   return (AlnMgr2MapSeqAlignToBioseq(sap, alnpos, row2));
}

/***************************************************************************
*
*  SECTION 7: Functions to change an alignment and retrieve parts of an
*    alignment
*
***************************************************************************/

/***************************************************************************
*
*   AlnMgr2TruncateSeqAlign truncates a given seqalign to contain only the
*   bioseq coordinates from start to stop on the indicated row.  Anything
*   before those coordinates is discarded; anything remaining afterwards
*   is made into another seqalign and put in sap->next (the original next,
*   if any, is now at sap->next->next).  Doesn't work on parent seqaligns.
*   The function returns TRUE if the orignal alignment extended past stop.
*
***************************************************************************/
/* SECTION 7 */
NLM_EXTERN Boolean AlnMgr2TruncateSeqAlign(SeqAlignPtr sap, Int4 start, Int4 stop, Int4 row)
{
   DenseDiagPtr  ddp;
   DenseDiagPtr  ddp2;
   DenseSegPtr   dsp;
   Int4          from;
   Int4          i;
   Int4          mstart;
   Int4          mstop;
   SeqAlignPtr   sap1;
   SeqAlignPtr   sap2;
   Int4          tmp;
   Int4          to;

   if (sap == NULL || stop<start || row < 1)
      return FALSE;
   if (sap->segtype == SAS_DENSEG)
   {
      if (sap->saip == NULL)
         AlnMgr2IndexSingleChildSeqAlign(sap);
      AlnMgr2GetNthSeqRangeInSA(sap, row, &mstart, &mstop);
      if (mstart > start || mstop < stop)
         return FALSE;
      if (mstart == start)
      {
         if (mstop == stop)
            return FALSE;
         else if (mstop > stop)
         {
            from = AlnMgr2MapBioseqToSeqAlign(sap, start, row);
            to = AlnMgr2MapBioseqToSeqAlign(sap, stop, row);
            if (to < from)
            {
               tmp = to;
               to = from;
               from = tmp;
            }
            sap1 = AlnMgr2GetSubAlign(sap, from, to, 0, TRUE);
            AlnMgr2IndexSingleChildSeqAlign(sap1);
            from = AlnMgr2MapBioseqToSeqAlign(sap, stop+1, row);
            if (from < 0)
               return FALSE;
            to = AlnMgr2MapBioseqToSeqAlign(sap, mstop, row);
            if (to < from)
            {
               tmp = to;
               to = from;
               from = tmp;
            }
            sap2 = AlnMgr2GetSubAlign(sap, from, to, 0, TRUE);
            sap2->next = sap->next;
            sap->next = sap2;
            dsp = (DenseSegPtr)(sap->segs);
            sap->segs = (Pointer)(sap1->segs);
            sap1->segs = NULL;
            DenseSegFree(dsp);
            SeqAlignFree(sap1);
            AlnMgr2IndexSingleChildSeqAlign(sap);
            AlnMgr2IndexSingleChildSeqAlign(sap2);
            return TRUE;
         }
      } else if (mstart < start) /* throw away the first part */
      {
         from = AlnMgr2MapBioseqToSeqAlign(sap, start, row);
         to = AlnMgr2MapBioseqToSeqAlign(sap, stop, row);
         if (to < from)
         {
            tmp = to;
            to = from;
            from = tmp;
         }
         sap1 = AlnMgr2GetSubAlign(sap, from, to, 0, TRUE);
         if (mstop == stop) /* done */
         {
            dsp = (DenseSegPtr)(sap->segs);
            sap->segs = (Pointer)(sap1->segs);
            sap1->segs = NULL;
            DenseSegFree(dsp);
            SeqAlignFree(sap1);
            AlnMgr2IndexSingleChildSeqAlign(sap);
            return FALSE;
         } else if (mstop > stop)
         {
            from = AlnMgr2MapBioseqToSeqAlign(sap, stop+1, row);
            if (from < 0)
               return FALSE;
            to = AlnMgr2MapBioseqToSeqAlign(sap, mstop, row);
            if (to < from)
            {
               tmp = to;
               to = from;
               from = tmp;
            }
            sap2 = AlnMgr2GetSubAlign(sap, from, to, 0, TRUE);
            sap2->next = sap->next;
            sap->next = sap2;
            AlnMgr2IndexSingleChildSeqAlign(sap2);
            dsp = (DenseSegPtr)(sap->segs);
            sap->segs = (Pointer)(sap1->segs);
            sap1->segs = NULL;
            DenseSegFree(dsp);
            SeqAlignFree(sap1);
            AlnMgr2IndexSingleChildSeqAlign(sap);
            return TRUE;
         }
      }
   } else if (sap->segtype == SAS_DENDIAG)
   {
      ddp = (DenseDiagPtr)(sap->segs);
      if (ddp->dim < row)
         return FALSE;
      mstart = ddp->starts[row-1];
      mstop = mstart + ddp->len - 1;
      if (mstart > start || mstop < stop)
         return FALSE;
      if (mstart == start)
      {
         if (mstop == stop)
            return FALSE;
         else if (mstop > stop)
         {
            ddp2 = DenseDiagNew();
            ddp2->dim = ddp->dim;
            ddp2->starts = (Int4Ptr)MemNew((ddp->dim)*sizeof(Int4));
            ddp2->id = SeqIdDupList(ddp->id);
            ddp2->strands = (Uint1Ptr)MemNew((ddp->dim)*sizeof(Uint1));
            ddp2->scores = ScoreDup(ddp->scores);
            for (i=0; i<ddp->dim; i++)
            {
               ddp2->starts[i] = ddp->starts[i] + ddp->len - (mstop - stop);
               ddp2->strands[i] = ddp->strands[i];
            }
            ddp2->len = mstop - stop;
            ddp->len = ddp->len - (mstop - stop);
            sap2 = SeqAlignNew();
            sap2->type = SAT_PARTIAL;
            sap2->segtype = SAS_DENSEG;
            sap2->segs = (Pointer)ddp2;
            sap2->next = sap->next;
            sap->next = sap2;
            AlnMgr2IndexSingleChildSeqAlign(sap2);
            return TRUE;
         }
      } else if (mstart < start)
      {
         for (i=0; i<ddp->dim; i++)
         {
            ddp->starts[i] = ddp->starts[i] + start - mstart;
         }
         ddp->len = ddp->len - (start - mstart);
         AlnMgr2IndexSingleChildSeqAlign(sap);
         if (mstop == stop)
            return FALSE;
         else if (mstop > stop)
         {
            ddp2 = DenseDiagNew();
            ddp2->dim = ddp->dim;
            ddp2->starts = (Int4Ptr)MemNew((ddp->dim)*sizeof(Int4));
            ddp2->id = SeqIdDupList(ddp->id);
            ddp2->strands = (Uint1Ptr)MemNew((ddp->dim)*sizeof(Uint1));
            ddp2->scores = ScoreDup(ddp->scores);
            for (i=0; i<ddp->dim; i++)
            {
               ddp2->starts[i] = ddp->starts[i] + ddp->len - (mstop - stop);
               ddp2->strands[i] = ddp->strands[i];
            }
            ddp2->len = mstop - stop;
            ddp->len = ddp->len - (mstop - stop);
            sap2 = SeqAlignNew();
            sap2->type = SAT_PARTIAL;
            sap2->segtype = SAS_DENSEG;
            sap2->segs = (Pointer)ddp2;
            sap2->next = sap->next;
            sap->next = sap2;
            AlnMgr2IndexSingleChildSeqAlign(sap2);
            return TRUE;
         }   
      }
   } else
      return FALSE;
   return FALSE;
}

/* SECTION 7 */
/***************************************************************************
*
*  AlnMgr2GetSubAlign retrieves a portion of an indexed alignment, from
*  'from' to 'to' in the row coordinates specified, or if which_row is 0,
*  'from' and 'to' are assumed to be alignment coordinates. If 'to' is -1,
*  the subalignment will go to the end of the specified row (or to the end
*  of the whole alignment). If the alignment is discontinuous and fill_in
*  is FALSE, the alignment will be returned as an SAS_DISC set, each piece
*  represented by a single alignment. If the alignment is discontinuous and
*  fill_in is TRUE, the unaligned regions will be added in to the alignment,
*  with all gaps in all other rows. If the alignment is continuous, it
*  doesn't matter whether fill_in is TRUE or FALSE. (SUBALIGN)
*
***************************************************************************/
NLM_EXTERN SeqAlignPtr AlnMgr2GetSubAlign(SeqAlignPtr sap, Int4 from, Int4 to, Int4 which_row, Boolean fill_in)
{
   Int4             a;
   AMAlignIndex2Ptr  amaip;
   AlnMsg2Ptr        amp;
   Boolean          anchored;
   Int4             currlen;
   DenseSegPtr      dsp;
   DenseSegPtr      dsp_new;
   Int4             from_aln;
   Int4             from_seq;
   Int4             i;
   SeqIdPtr         id;
   Int4             j;
   Int4             k;
   Int4             len;
   Int4             lengthbit;
   Int4             minlen;
   Boolean          more;
   Int4             n;
   Int4             numseg;
   Int4             numunaln;
   AMRowInfoPtr     row;
   AMRowInfoPtr     row_head;
   AMRowInfoPtr     row_prev;
   AMRowInfoPtr     PNTR rowheads;
   AMRowInfoPtr     PNTR rows;
   SeqAlignPtr      salp;
   SeqAlignPtr      salp_head;
   SeqAlignPtr      salp_prev;
   SeqAlignPtr      sap_real;
   Int4             seg;
   Int4             start_seg;
   Uint1            strand;
   SeqAlignPtr      subsalp;
   Int4             tmp;
   Int4             to_aln;
   Int4             to_seq;
   Int4             ustart;
   Int4             ustop;

   if (sap == NULL || sap->saip == NULL)
      return NULL;
   len = AlnMgr2GetAlnLength(sap, FALSE);
   if (which_row == 0 && (to > len-1 || from < 0))
      return NULL;
   n = AlnMgr2GetNumRows(sap);
   if (which_row < 0 || which_row > n)
      return NULL;
   if (to == -1)
   {
      if (which_row == 0)
         to = len-1;
      else
         AlnMgr2GetNthSeqRangeInSA(sap, which_row, NULL, &to);
   }
   if (sap->saip->indextype == INDEX_CHILD)
      sap_real = sap;
   else if (sap->saip->indextype == INDEX_PARENT)
   {
      amaip = (AMAlignIndex2Ptr)(sap->saip);
      if (amaip->alnstyle == AM2_LITE)
         return NULL;
      sap_real = amaip->sharedaln;
      if (from == 0 && to == len-1 && !AlnMgr2IsSAPDiscAli(sap_real))  /* need whole aln -- take a shortcut! */
         return SeqAlignDup(sap_real);
   }
   if ((a = AlnMgr2FindAnchor(sap_real)) > 0)
   {
      anchored = TRUE;
      salp = SeqAlignDup(sap_real);
      AlnMgr2IndexSingleChildSeqAlign(salp);
      if (which_row == 0) /* anchor coordinates */
      {
         AlnMgr2GetNthSeqRangeInSA(salp, a, &from_seq, &to_seq);
         from_aln = AlnMgr2MapBioseqToSeqAlign(salp, from_seq, a);
         to_aln = AlnMgr2MapBioseqToSeqAlign(salp, to_seq, a);
         if (from_aln > to_aln)
         {
            tmp = from_aln;
            from_aln = to_aln;
            to_aln = tmp;
         }
      } else
      {
         from_aln = AlnMgr2MapBioseqToSeqAlign(salp, from, which_row);
         to_aln = AlnMgr2MapBioseqToSeqAlign(salp, to, which_row);
         if (from_aln > to_aln)
         {
            tmp = from_aln;
            from_aln = to_aln;
            to_aln = tmp;
         }
      }
   } else
   {
      anchored = FALSE;
      salp = sap_real;
      if (which_row == 0) /* alignment coordinates */
      {
         from_aln = from;
         to_aln = to;
      } else
      {
         from_aln = AlnMgr2MapBioseqToSeqAlign(salp, from, which_row);
         to_aln = AlnMgr2MapBioseqToSeqAlign(salp, to, which_row);
         if (from_aln > to_aln)
         {
            tmp = from_aln;
            from_aln = to_aln;
            to_aln = tmp;
         }
      }
   }
   rows = (AMRowInfoPtr PNTR)MemNew(n*sizeof(AMRowInfoPtr));
   amp = AlnMsgNew2();
   seg = lengthbit = 0;
   currlen = 0;
   numunaln = 0;
   salp_head = salp_prev = NULL;
   while (AlnMgr2GetNextLengthBit(sap, &lengthbit, &seg))
   {
      if (currlen <= to_aln && seg >= 0 && currlen+lengthbit-1 >= from_aln)
      {
         numseg = AlnMgr2GetNumSegsInRange(sap, currlen, currlen+lengthbit-1, &start_seg);
         numunaln = 0;
         for (i=0; i<n; i++)
         {
            row_head = NULL;
            for (j=start_seg; j<numseg+start_seg; j++)
            {
               AlnMsgReNew2(amp);
               AlnMgr2GetNthSegmentRange(sap, j+1, &amp->from_aln, &amp->to_aln);
               amp->from_aln = MAX(amp->from_aln, from_aln);
               amp->to_aln = MIN(amp->to_aln, to_aln);
               amp->row_num = i+1;
               while ((more = AlnMgr2GetNextAlnBit(salp, amp)) == TRUE)
               {
                  if (amp->right_interrupt != NULL && amp->right_interrupt->unalnlen > 0)
                     numunaln++;
                  row = (AMRowInfoPtr)MemNew(sizeof(AMRowInfo));
                  if (amp->type == AM_GAP)
                     row->from = -1;
                  else
                     row->from = amp->from_row;
                  row->len = amp->to_row - amp->from_row + 1;
                  if (row_head != NULL)
                  {
                     row_prev->next = row;
                     row_prev = row;
                  } else
                     row_head = row_prev = row;
               }
            }
            rows[i] = row_head;
         }
      }
      rowheads = (AMRowInfoPtr PNTR)MemNew(n*sizeof(AMRowInfoPtr));
      for (i=0; i<n; i++)
      {
         rowheads[i] = rows[i];
      }
      while (rows[0] != NULL)
      {
         minlen = -1;
         for (i=0; i<n; i++)
         {
            if (rows[i]->len < minlen || minlen == -1)
               minlen = rows[i]->len;
         }
         for (i=0; i<n; i++)
         {
            if (rows[i]->len > minlen)
            {
               row = (AMRowInfoPtr)MemNew(sizeof(AMRowInfo));
               row->next = rows[i]->next;
               rows[i]->next = row;
               if (rows[i]->from == -1)
                  row->from = -1;
               else if (AlnMgr2GetNthStrand(salp, i) == Seq_strand_minus)
               {
                  row->from = rows[i]->from;
                  rows[i]->from = rows[i]->from + rows[i]->len - 1 - minlen;
               } else
                  row->from = rows[i]->from + minlen;
               row->len = rows[i]->len - minlen;
               rows[i]->len = minlen;
            }
            rows[i] = rows[i]->next;
         }
      }
      for (i=0; i<n; i++)
      {
         rows[i] = rowheads[i];
      }
      MemFree(rowheads);
      dsp = DenseSegNew();
      row = rows[0];
      while (row != NULL)
      {
         dsp->numseg++;
         row = row->next;
      }
      if (fill_in)
         dsp->numseg += numunaln;
      dsp->dim = n;
      dsp->lens = (Int4Ptr)MemNew((dsp->numseg)*sizeof(Int4));
      dsp->starts = (Int4Ptr)MemNew((dsp->numseg)*(dsp->dim)*sizeof(Int4));
      dsp->strands = (Uint1Ptr)MemNew((dsp->numseg)*(dsp->dim)*sizeof(Int4));
      j = 0;
      row = rows[0];
      while (row != NULL)
      {
         dsp->lens[j] = row->len;
         j++;
         row = row->next;
      }
      id = AlnMgr2GetNthSeqIdPtr(salp, 0);
      dsp->ids = id;
      for (i=0; i<n; i++)
      {
         if (i > 0)
         {
            id->next = AlnMgr2GetNthSeqIdPtr(salp, i+1);
            id = id->next;
         }
         row = rows[i];
         j = 0;
         strand = AlnMgr2GetNthStrand(salp, i+1);
         while (row != NULL)
         {
            dsp->starts[n*j + i] = row->from;
            dsp->strands[n*j + i] = strand;
            j++;
            row = row->next;
         }
      }
      if (fill_in)
      {
         for (i=0; i<n; i++)
         {
            AlnMgr2GetNthUnalignedForNthRow(sap, seg+1, i+1, &ustart, &ustop);
            if (ustart >= 0 && ustop >= ustart)
            {
               for (k=0; k<n; k++)
               {
                  dsp->starts[n*j + k] = -1;
                  dsp->strands[n*j + k] = dsp->strands[i];
               }
               dsp->starts[n*j + i] = ustart;
               j++;
            }
         }
      }
      subsalp = SeqAlignNew();
      subsalp->type = SAT_PARTIAL;
      subsalp->segtype = SAS_DENSEG;
      subsalp->dim = n;
      subsalp->segs = (Pointer)(dsp);
      for (i=0; i<n; i++)
      {
         row = rows[i];
         while (row != NULL)
         {
            row_prev = row->next;
            MemFree(row);
            row = row_prev;
         }
      }
      if (seg < 0)
         seg = -seg;
      currlen += lengthbit;
      seg++;
      if (salp_head != NULL)
      {
         salp_prev->next = subsalp;
         salp_prev = subsalp;
      } else
         salp_head = salp_prev = subsalp;
   }
   MemFree(rows);
   AlnMsgFree2(amp);
   if (fill_in && salp_head->next != NULL)  /* stick subsalps together into a big aln */
   {
      j = 0;
      subsalp = salp_head;
      while (subsalp != NULL)
      {
         dsp = (DenseSegPtr)(subsalp->segs);
         j += dsp->numseg;
         subsalp = subsalp->next;
      }
      dsp_new = DenseSegNew();
      dsp_new->dim = n;
      dsp_new->numseg = j;
      dsp_new->lens = (Int4Ptr)MemNew((dsp->numseg)*sizeof(Int4));
      dsp_new->starts = (Int4Ptr)MemNew((dsp->numseg)*(dsp->dim)*sizeof(Int4));
      dsp_new->strands = (Uint1Ptr)MemNew((dsp->numseg)*(dsp->dim)*sizeof(Int4));
      subsalp = salp_head;
      k = 0;
      while (subsalp != NULL)
      {
         dsp = (DenseSegPtr)(subsalp->segs);
         for (j=0; j<dsp->numseg; j++)
         {
            dsp_new->lens[k] = dsp->lens[j];
            for (i=0; i<n; i++)
            {
               dsp_new->starts[k*n+i] = dsp->starts[j*n+i];
               dsp_new->strands[k*n+i] = dsp->strands[j*n+i];
            }
            k++;
         }
         subsalp = subsalp->next;
      }
      subsalp = SeqAlignNew();
      subsalp->type = SAT_PARTIAL;
      subsalp->segtype = SAS_DENSEG;
      subsalp->dim = n;
      subsalp->segs = (Pointer)(dsp_new);
      SeqAlignSetFree(salp_head);
   } else if (!fill_in && salp_head->next != NULL)
   {
      subsalp = SeqAlignNew();
      subsalp->segtype = SAS_DISC;
      subsalp->type = SAT_PARTIAL;
      subsalp->segs = (SeqAlignPtr)(salp_head);
      salp_prev = salp_head;
      while (salp_prev != NULL)
      {
         AMAlignIndexFreeEitherIndex(salp_prev);
         salp_prev = salp_prev->next;
      }
   } else  /* if !salp_head->next */
   {
      subsalp = salp_head;
      subsalp->dim = AlnMgr2GetNumRows(subsalp);
      subsalp->type = SAT_PARTIAL;
      AMAlignIndexFreeEitherIndex(subsalp);
   }
   if (anchored)
      SeqAlignFree(salp);
   return subsalp;
}

/***************************************************************************
*
*  SECTION 8: Miscellaneous functions to compute useful information
*    about an alignment
*
***************************************************************************/
/* SECTION 8 */
/***************************************************************************
*
*  AlnMgr2ComputeScoreForSeqAlign computes an ad hoc numerical score for
*  an indexed alignment by computing a similarity score for the whole
*  alignment (residue pair by residue pair score, from a matrix for proteins
*  and identity for nucleotides) and then subtracting gap open and gap
*  extension penalties.
*
***************************************************************************/
NLM_EXTERN Int4 AlnMgr2ComputeScoreForSeqAlign(SeqAlignPtr sap)
{
   AMFreqPtr    afp;
   DenseSegPtr  dsp;
   Int4         gaplen;
   Int4         i;
   Boolean      is_prot;
   Int4         j;
   Int4         len;
   Int4         mismatch;
   Int4         numgaps;
   Int4         numseqs;
   Boolean      open;
   Int4         res1;
   Int4         res2;
   Int4         score;
   Int4         seqscore;
   
   if (sap->segtype == SAS_DISC)
      return -1;
   if (sap->saip == NULL)
      AlnMgr2IndexSingleChildSeqAlign(sap);
   is_prot = AlnMgr2IsItProtein(sap);
   len = AlnMgr2GetAlnLength(sap, FALSE);
   dsp = (DenseSegPtr)(sap->segs);
   numseqs = dsp->dim;
   open = FALSE;
   gaplen = 0;
   numgaps = 0;
   for (i=0; i<dsp->dim; i++)
   {
      for (j=0; j<dsp->numseg; j++)
      {
         if (dsp->starts[(dsp->dim)*j+i] == -1)
         {
            if (!open)
            {
               gaplen += dsp->lens[j];
               numgaps++;
               open = TRUE;
            } else
               gaplen += dsp->lens[j];
         } else
            open = FALSE;
      }
   }
   mismatch = 0;
   seqscore = 0;
   afp = AlnMgr2ComputeFreqMatrix(sap, 0, -1, 0);
   if (afp == NULL)
     return -1;
   for (i=0; i<afp->len; i++)
   {
      res1 = -1;
      res2 = -1;
      for (j=0; j<afp->size; j++)
      {
         if (afp->freq[j][i] == 1)
         {
            if (res1 == -1)
               res1 = j;
            else
               res2 = j;
         } else if (afp->freq[j][i] == 2)
            res1 = res2 = j;
      }
      if (res1 > 0 && res2 > 0) /* don't penalize gaps */
         seqscore += AlnMgr2GetScoreForPair(res1, res2, is_prot);
   }
   AMFreqFree(afp);
   score = seqscore + numgaps*AM_GAPOPEN + gaplen*AM_GAPEXT;
   return score;
}

static Int4 AlnMgr2SeqPortRead(SeqPortPtr PNTR spp, Uint1Ptr buf, Int4Ptr bufpos, Int4 start, Int4 stop, Uint1 strand, Uint1 code, BioseqPtr bsp)
{
    if (*spp == NULL) /* first call */ {
        if (strand == Seq_strand_minus){
            *spp = SeqPortNew(bsp, MAX(0, stop-AM_SEQPORTSIZE), stop, strand, code);
            *bufpos = MAX(0, stop-AM_SEQPORTSIZE);
        } 
        else {
            *spp = SeqPortNew(bsp, start, MIN(start+AM_SEQPORTSIZE, bsp->length-1), strand, code);
            *bufpos = start;
        }
    }
    /* see if what we need is in current seqport or a new one is needed */ 
    else if ((start < *bufpos) || (start > *bufpos+AM_SEQPORTSIZE) 
             || (stop < *bufpos) || (stop > *bufpos+AM_SEQPORTSIZE)) {
        SeqPortFree(*spp);
        if (strand == Seq_strand_minus) {         
            *spp = SeqPortNew(bsp, MAX(0, stop-AM_SEQPORTSIZE), stop, strand, code);
            *bufpos = MAX(0, stop-AM_SEQPORTSIZE);
        } 
        else {
            *spp = SeqPortNew(bsp, start, MIN(start+AM_SEQPORTSIZE, bsp->length-1), strand, code);
            *bufpos = start;
        }
    }
    return (SeqPortRead(*spp, buf, (MIN(start+AM_SEQPORTSIZE-1, stop)) - start+1));
}

/* SECTION 8 */
/***************************************************************************
*
*  AlnMgr2ComputeFreqMatrix takes an indexed seqalign and returns a matrix
*  indicating nucleotide or amino acid frequency at each position of the
*  alignment. The matrix can be made over only a part of the alignment, if
*  from and to are nonzero, and if row is nonzero, from and to are taken
*  to be bioseq coordinates from that row (if row == 0 from and to are
*  assumed to be alignment coordinates).
*
***************************************************************************/
NLM_EXTERN AMFreqPtr AlnMgr2ComputeFreqMatrix(SeqAlignPtr sap, Int4 from, Int4 to, Int4 row)
{
   AMFreqPtr   afp;
   AlnMsg2Ptr  amp;
   BioseqPtr   bsp;
   Uint1       buf[AM_SEQPORTSIZE];
   Int4        bufpos;
   Uint1       code;
   Int4        counter;
   Int4        ctr;
   Int4        from_a;
   Int4        i;
   Boolean     isna;
   Int4        j;
   Int4        l;
   Int4        len;
   Boolean     more;
   Int4        n;
   Int4        numrows;
   Uint1       res;
   SeqIdPtr    sip;
   SeqPortPtr  spp;
   Int4        tmp;
   Int4        to_a;

   if (sap == NULL || sap->saip == NULL || (from > to && to != -1))
      return NULL;
   numrows = AlnMgr2GetNumRows(sap);
   bufpos = -1;
   if (row > numrows || row < 0)
      return NULL;
   len = AlnMgr2GetAlnLength(sap, FALSE);
   if (to >= len)
      return NULL;
   if (to == -1)
      to = len-1;
   sip = AlnMgr2GetNthSeqIdPtr(sap, 1);
   bsp = BioseqLockById(sip);
   if (bsp != NULL)
      isna = ISA_na(bsp->mol);
   else
   {
      SeqIdFree(sip);
      return NULL;
   }
   BioseqUnlock(bsp);
   SeqIdFree(sip);
   if (isna)
      code = Seq_code_ncbi4na;
   else
      code = Seq_code_ncbistdaa;
   afp = (AMFreqPtr)MemNew(sizeof(AMFreq));
   afp->len = len;
   if (isna)
      afp->size = AM_NUCSIZE;
   else
      afp->size = AM_PROTSIZE;
   afp->freq = (Int4Ptr PNTR)MemNew((afp->size)*sizeof(Int4Ptr));
   for (i=0; i<afp->size; i++)
   {
      afp->freq[i] = (Int4Ptr)MemNew((afp->len)*sizeof(Int4));
   }
   amp = AlnMsgNew2();
   if (row != 0)
   {
      from_a = AlnMgr2MapBioseqToSeqAlign(sap, from, row);
      to_a = AlnMgr2MapBioseqToSeqAlign(sap, to, row);
      if (from_a > to_a)
      {
         tmp = to_a;
         to_a = from_a;
         from_a = tmp;
      }
   } else
   {
      from_a = from;
      to_a = to;
   }
   for (i=0; i<numrows; i++)
   {
      spp = NULL;
      AlnMsgReNew2(amp);
      amp->from_aln = from_a;
      amp->to_aln = to_a;
      amp->row_num = i+1;
      j = 0;
      while ((more = AlnMgr2GetNextAlnBit(sap, amp)))
      {
         if (amp->type == AM_GAP)
         {
            for (n=0; n<(amp->to_row - amp->from_row+1); n++)
            {
               afp->freq[0][j] = afp->freq[0][j]+1;
               j++;
            }
         } else if (amp->type == AM_SEQ)
         {
            sip = AlnMgr2GetNthSeqIdPtr(sap, i+1);
            bsp = BioseqLockById(sip);
            if (bsp != NULL) {
              for (l=amp->from_row; l<=amp->to_row; l+=AM_SEQPORTSIZE)
              {
                 counter = AlnMgr2SeqPortRead(&spp, buf, &bufpos, l, MIN(l+AM_SEQPORTSIZE, amp->to_row), amp->strand, code, bsp);
                 ctr = 0;
                 while (ctr < counter)
                 {
                    res = buf[ctr];
                    if (isna)
                    {
                       if (res == 1 || res == 2)
                          afp->freq[res][j]++;
                       else if (res == 4)
                          afp->freq[3][j]++;
                       else if (res == 8)
                          afp->freq[4][j]++;
                       else
                          afp->freq[5][j]++;
                    } else
                       afp->freq[res][j]++;
                    j++;
                    ctr++;
                 }
              }
              BioseqUnlock(bsp);
            }
            SeqIdFree(sip);
         }
      }
      SeqPortFree(spp);
   }
   AlnMsgFree2(amp);
   return afp;
}

/* SECTION 8 */
/***************************************************************************
*
*  AlnMgr2GetScoreForPair assigns scores to nucleotide and protein residue
*  pairs. Nucleotide pairs are scored according to a standard mismatch
*  penalty, and amino acid pairs are scored according to the BLOSUM62
*  matrix below. This matrix has been rearranged so that the rows and
*  columns appear in alphabetical order, so that it directly correlates
*  to the NCBIstdaa alphabet (with a minus-one difference).
*
***************************************************************************/
static Int4 AlnMgr2GetScoreForPair(Int4 res1, Int4 res2, Boolean is_prot)
{
   Int4  matrix[24][24] = {
     {4, -2, 0, -2, -1, -2, 0, -2, -1, -1, -1, -1, -2, -1, -1, -1, 1, 0, 0, -3, 0, -2, -1, -4},
     {-2, 4, -3, 4, 1, -3, -1, 0, -3, 0, -4, -3, 3, -2, 0, -1, 0, -1, -3, -4, -1, -3, 1, -4},
     {0, -3, 9, -3, -4, -2, -3, -3, -1, -3, -1, -1, -3, -3, -3, -3, -1, -1, -1, -2, -2, -2, -3, -4},
     {-2, 4, -3, 6, 2, -3, -1, -1, -3, -1, -4, -3, 1, -1, 0, -2, 0, -1, -3, -4, -1, -3, 1, -4},
     {-1, 1, -4, 2, 5, -3, -2, 0, -3, 1, -3, -2, 0, -1, 2, 0, 0, -1, -2, -3, -1, -2, 4, -4},
     {-2, -3, -2, -3, -3, 6, -3, -1, 0, -3, 0, 0, -3, -4, -3, -3, -2, -2, -1, 1, -1, 3, -3, -4},
     {0, -1, -3, -1, -2, -3, 6, -2, -4, -2, -4, -3, 0, -2, -2, -2, 0, -2, -3, -2, -1, -3, -2, -4},
     {-2, 0, -3, -1, 0, -1, -2, 8, -3, -1, -3, -2, 1, -2, 0, 0, -1, -2, -3, -2, -1, 2, 0, -4},
     {-1, -3, -1, -3, -3, 0, -4, -3, 4, -3, 2, 1, -3, -3, -3, -3, -2, -1, 3, -3, -1, -1, -3, -4},
     {-1, 0, -3, -1, 1, -3, -2, -1, -3, 5, -2, -1, 0, -1, 1, 2, 0, -1, -2, -3, -1, -2, 1, -4},
     {-1, -4, -1, -4, -3, 0, -4, -3, 2, -2, 4, 2, -3, -3, -2, -2, -2, -1, 1, -2, -1, -1, -3, -4},
     {-1, -3, -1, -3, -2, 0, -3, -2, 1, -1, 2, 5, -2, -2, 0, -1, -1, -1, 1, -1, -1, -1, -1, -4},
     {-2, 3, -3, 1, 0, -3, 0, 1, -3, 0, -3, -2, 6, -2, 0, 0, 1, 0, -3, -4, -1, -2, 0, -4},
     {-1, -2, -3, -1, -1, -4, -2, -2, -3, -1, -3, -2, -2, 7, -1, -2, -1, -1, -2, -4, -2, -3, -1, -4},
     {-1, 0, -3, 0, 2, -3, -2, 0, -3, 1, -2, 0, 0, -1, 5, 1, 0, -1, -2, -2, -1, -1, 3, -4},
     {-1, -1, -3, -2, 0, -3, -2, 0, -3, 2, -2, -1, 0, -2, 1, 5, -1, -1, -3, -3, -1, -2, 0, -4},
     {1, 0, -1, 0, 0, -2, 0, -1, -2, 0, -2, -1, 1, -1, 0, -1, 4, 1, -2, -3, 0, -2, 0, -4},
     {0, -1, -1, -1, -1, -2, -2, -2, -1, -1, -1, -1, 0, -1, -1, -1, 1, 5, 0, -2, 0, -2, -1, -4},
     {0, -3, -1, -3, -2, -1, -3, -3, 3, -2, 1, 1, -3, -2, -2, -3, -2, 0, 4, -3, -1, -1, -2, -4},
     {-3, -4, -2, -4, -3, 1, -2, -2, -3, -3, -2, -1, -4, -4, -2, -3, -3, -2, -3, 11, -2, 2, -3, -4},
     {0, -1, -2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -2, -1, -1, 0, 0, -1, -2, -1, -1, -1, -4},
     {-2, -3, -2, -3, -2, 3, -3, 2, -1, -2, -1, -1, -2, -3, -1, -2, -2, -2, -1, 2, -1, 7, -2, -4},
     {-1, 1, -3, 1, 4, -3, -2, 0, -3, 1, -3, -1, 0, -1, 3, 0, 0, -1, -2, -3, -1, -2, 4, -4},
     {-4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, 1}};

   if (is_prot) /* protein->use BLOSUM62 matrix */
      return matrix[res1-1][res2-1];
   else /* nucleotide->use match score/mismatch penalty */
   {
      if (res1 == 0 || res2 == 0) /* don't count gaps */
         return 0;
      if (res1 == res2)
         return 1;
      else
         return -3;
   }
}

/* SECTION 8 */
/***************************************************************************
*
*  AlnMgr2IsItProtein takes an indexed alignment and quickly decides if
*  it's a protein or nucleotide alignment, returning TRUE for protein.
*
***************************************************************************/
NLM_EXTERN Boolean AlnMgr2IsItProtein(SeqAlignPtr sap)
{
   BioseqPtr  bsp;
   Boolean    is_na;
   SeqIdPtr   sip;

   if (sap == NULL || sap->saip == NULL)
      return FALSE;
   sip = AlnMgr2GetNthSeqIdPtr(sap, 1);
   bsp = BioseqLockById(sip);
   if (bsp == NULL)
      return FALSE;
   is_na = ISA_na(bsp->mol);
   SeqIdFree(sip);
   BioseqUnlock(bsp);
   return (!is_na);
}

/***************************************************************************
*
*  SECTION 9: Sorting functions and other algorithms to help order
*  alignments for various purposes
*
***************************************************************************/

/* SECTION 9 */
static int LIBCALLBACK AMCompareStarts(VoidPtr ptr1, VoidPtr ptr2)
{
   AMBitty2Ptr  bit1;
   AMBitty2Ptr  bit2;

   if (ptr1 != NULL && ptr2 != NULL)
   {
      bit1 = (AMBitty2Ptr)ptr1;
      bit2 = (AMBitty2Ptr)ptr2;
      if (bit1->num2 < bit2->num2)
         return -1;
      else if (bit1->num2 > bit2->num2)
         return 1;
      else if (bit1->num3 > bit2->num3) /* compare aln lengths */
         return -1;
      else if (bit1->num3 < bit2->num3)
         return 1;
      else
         return 0;
   }
   return 0;
}

/* SECTION 9 */
/***************************************************************************
*
*  AlnMgr2SortAlnSetByNthRowPos takes an indexed parent alignment and sorts
*  all the child alignments along the row indicated. If the indicated row
*  is aligned on the plus strand, the alignments are sorted from smaller
*  to larger coordinates along that row; otherwise they are sorted in
*  reverse order.
*
***************************************************************************/
NLM_EXTERN void AlnMgr2SortAlnSetByNthRowPos(SeqAlignPtr sap, Int4 row)
{
   AMAlignIndex2Ptr  amaip;
   AMBitty2Ptr       bit;
   Int4             i;
   SeqAlignPtr      PNTR saparray;
   Uint1            strand;

   if (sap == NULL || sap->saip == NULL || sap->saip->indextype != INDEX_PARENT)
      return;
   amaip = (AMAlignIndex2Ptr)(sap->saip);
   bit = (AMBitty2Ptr)MemNew((amaip->numsaps)*sizeof(AMBitty2));
   saparray = (SeqAlignPtr PNTR)MemNew((amaip->numsaps)*sizeof(SeqAlignPtr));
   for (i=0; i<amaip->numsaps; i++)
   {
      bit[i].num1 = i;
      AlnMgr2GetNthSeqRangeInSA(amaip->saps[i], row, &bit[i].num2, NULL);
      bit[i].num3 = AlnMgr2GetAlnLength(amaip->saps[i], FALSE);
      strand = AlnMgr2GetNthStrand(amaip->saps[i], row);
      if (strand == Seq_strand_minus)
         bit[i].num2 = -bit[i].num2;
      saparray[i] = amaip->saps[i];
   }
   HeapSort(bit, amaip->numsaps, sizeof(AMBitty2), AMCompareStarts);
   for (i=0; i<amaip->numsaps; i++)
   {
      amaip->saps[i] = saparray[bit[i].num1];
   }
   MemFree(saparray);
   MemFree(bit);
   if (amaip->alnstyle != AM2_LITE)
      AlnMgr2ReIndexSeqAlign(sap);
}


/***************************************************************************
*
*  SECTION 10: Basic alignment operations
*
***************************************************************************/

/***************************************************************************
*
*  AlnMgr2MergeTwoAlignments takes two alignments, with identical rows in
*  the same order (otherwise it rejects the alignments), and merges them
*  into a single alignment. If there is unaligned space between the two
*  alignments and this space is the same length for every row, the function
*  aligns those sequences; it rejects alignments when the unaligned spaces
*  are different sizes. The function returns a newly allocated alignment.
*
***************************************************************************/
NLM_EXTERN SeqAlignPtr AlnMgr2MergeTwoAlignments(SeqAlignPtr sap1_orig, SeqAlignPtr sap2_orig)
{
   Int4         c;
   DenseSegPtr  dsp;
   DenseSegPtr  dsp1;
   DenseSegPtr  dsp2;
   DenseSegPtr  dsp_new;
   Int4         i;
   Int4         j;
   Int4         n1;
   Int4         n2;
   SeqAlignPtr  sap1;
   SeqAlignPtr  sap2;
   SeqAlignPtr  sap_new;
   SeqIdPtr     sip1;
   SeqIdPtr     sip2;
   Int4         start1;
   Int4         start2;
   Int4         stop1;
   Int4         stop2;
   Uint1        strand1;
   Uint1        strand2;
   SeqAlignPtr  tmp;

   if (sap1_orig == NULL || sap2_orig == NULL)
      return NULL;
   if (sap1_orig->next != NULL)
   {
      AlnMgr2IndexSeqAlign(sap1_orig);
      sap1 = AlnMgr2GetSubAlign(sap1_orig, 0, -1, 0, TRUE);
   } else
      sap1 = SeqAlignDup(sap1_orig);
   if (sap2_orig->next != NULL)
   {
      AlnMgr2IndexSeqAlign(sap2_orig);
      sap2 = AlnMgr2GetSubAlign(sap2_orig, 0, -1, 0, TRUE);
   } else
      sap2 = SeqAlignDup(sap2_orig);
   AlnMgr2IndexSingleChildSeqAlign(sap1);
   AlnMgr2IndexSingleChildSeqAlign(sap2);
   n1 = AlnMgr2GetNumRows(sap1);
   n2 = AlnMgr2GetNumRows(sap2);
   if (n1 != n2)
   {
      SeqAlignFree(sap1);
      SeqAlignFree(sap2);
      return NULL;
   }
   /* put the alignments in order by the first row */
   AlnMgr2GetNthSeqRangeInSA(sap1, 1, &start1, &stop1);
   AlnMgr2GetNthSeqRangeInSA(sap2, 1, &start2, &stop2);
   strand1 = AlnMgr2GetNthStrand(sap1, 1);
   if (strand1 == Seq_strand_minus)
   {
      if (stop2 > start1)
      {
         tmp = sap1;
         sap1 = sap2;
         sap2 = tmp;
      }
   } else
   {
      if (stop1 > start2)
      {
         tmp = sap1;
         sap1 = sap2;
         sap2 = tmp;
      }
   }
   dsp1 = (DenseSegPtr)(sap1->segs);
   dsp2 = (DenseSegPtr)(sap2->segs);
   sip1 = dsp1->ids;
   sip2 = dsp2->ids;
   while (sip1 != NULL && sip2 != NULL)
   {
      if (SeqIdComp(sip1, sip2) != SIC_YES)
      {
         SeqAlignFree(sap1);
         SeqAlignFree(sap2);
         return NULL;
      }
      sip1 = sip1->next;
      sip2 = sip2->next;
   }
   dsp = DenseSegNew();
   dsp->dim = n1;
   dsp->numseg = 1;
   dsp->starts = (Int4Ptr)MemNew(n1*sizeof(Int4));
   dsp->lens = (Int4Ptr)MemNew(sizeof(Int4));
   dsp->strands = (Uint1Ptr)MemNew(n1*sizeof(Int4));
   for (i=0; i<n1; i++)
   {
      strand1 = AlnMgr2GetNthStrand(sap1, i+1);
      strand2 = AlnMgr2GetNthStrand(sap2, i+1);
      if (strand1 != strand2)
      {
         DenseSegFree(dsp);
         SeqAlignFree(sap1);
         SeqAlignFree(sap2);
         return NULL;
      }
      AlnMgr2GetNthSeqRangeInSA(sap1, i+1, &start1, &stop1);
      AlnMgr2GetNthSeqRangeInSA(sap2, i+1, &start2, &stop2);
      if (strand1 == Seq_strand_minus)
      {
         dsp->starts[i] = stop2 + 1;
         if (i == 0)
            dsp->lens[0] = start2 - (stop2 + 1);
         else
         {
            if (start2 - (stop2 + 1) != dsp->lens[0])
            {
               DenseSegFree(dsp);
               SeqAlignFree(sap1);
               SeqAlignFree(sap2);
               return NULL;
            }
         }
      } else
      {
         dsp->starts[i] = stop1 + 1;
         if (i == 0)
            dsp->lens[0] = start2 - (stop1 + 1);
         else
         {
            if (start2 - (stop1 + 1) != dsp->lens[0])
            {
               DenseSegFree(dsp);
               SeqAlignFree(sap1);
               SeqAlignFree(sap2);
               return NULL;
            }
         }
      }
      dsp->strands[i] = strand1;
   }
   if (dsp->lens[0] == 0)
   {
      DenseSegFree(dsp);
      dsp = NULL;
   }
   dsp_new = DenseSegNew();
   dsp_new->numseg = dsp1->numseg + dsp2->numseg;
   if (dsp != NULL)
      dsp_new->numseg++;
   dsp_new->dim = n1;
   dsp_new->starts = (Int4Ptr)MemNew(dsp_new->dim*dsp_new->numseg*sizeof(Int4));
   dsp_new->lens = (Int4Ptr)MemNew(dsp_new->numseg*sizeof(Int4));
   dsp_new->strands = (Uint1Ptr)MemNew(dsp_new->dim*dsp_new->numseg*sizeof(Uint1));
   for (i=0; i<dsp1->numseg; i++)
   {
      for (j=0; j<n1; j++)
      {
         dsp_new->starts[i*n1 + j] = dsp1->starts[i*n1 + j];
         dsp_new->strands[i*n1 + j] = dsp1->strands[i*n1 + j];
      }
      dsp_new->lens[i] = dsp1->lens[i];
   }
   c = dsp1->numseg;
   if (dsp != NULL)
   {
      for (j=0; j<n1; j++)
      {
         dsp_new->starts[c*n1 + j] = dsp->starts[j];
         dsp_new->strands[c*n1 + j] = dsp->strands[j];
      }
      dsp_new->lens[c] = dsp->lens[0];
      c++;
   }
   for (i=0; i<dsp2->numseg; i++, c++)
   {
      for (j=0; j<n1; j++)
      {
         dsp_new->starts[c*n1 + j] = dsp2->starts[i*n1 + j];
         dsp_new->strands[c*n1 + j] = dsp2->strands[i*n1 + j];
      }
      dsp_new->lens[c] = dsp2->lens[i];
      c++;
   }
   dsp_new->ids = SeqIdDupList(dsp1->ids);
   sap_new = SeqAlignNew();
   sap_new->segtype = SAS_DENSEG;
   sap_new->dim = n1;
   sap_new->segs = (Pointer)dsp_new;
   if (dsp != NULL)
      DenseSegFree(dsp);
   SeqAlignFree(sap1);
   SeqAlignFree(sap2);
   return sap_new;
}

/* SECTION 10 */
/***************************************************************************
*
*  AlnMgr2ExtendToCoords takes an indexed child seqalign and blindly extends
*  it to the coordinates specified on the given row. If other rows are too
*  short to allow this extension, the alignment is extended as far as
*  possible. If to == -1 the extension goes to the end of the sequence
*  specified.
*
***************************************************************************/
NLM_EXTERN void AlnMgr2ExtendToCoords(SeqAlignPtr sap, Int4 from, Int4 to, Int4 row)
{
   BioseqPtr    bsp;
   Int4         diff1;
   Int4         diff2;
   DenseSegPtr  dsp;
   DenseSegPtr  dsp_new;
   Int4         i;
   Int4         j;
   Int4         numrows;
   Int4         numseg;
   Int4         prediff1;
   Int4         prediff2;
   Int4         seg;
   SeqIdPtr     sip;
   Int4         start;
   Int4         stop;

   if (sap == NULL || sap->saip == NULL || sap->saip->indextype != INDEX_CHILD)
      return;
   numrows = AlnMgr2GetNumRows(sap);
   if (row < 1 || row > numrows)
      return;
   AlnMgr2GetNthSeqRangeInSA(sap, row, &start, &stop);
   numseg = 0;
   dsp = (DenseSegPtr)(sap->segs);
   if (start <= from)
      from = start;
   else
      numseg++;
   diff1 = from - start;
   sip = AlnMgr2GetNthSeqIdPtr(sap, row);
   bsp = BioseqLockById(sip);
   if (to == -1)
      to = bsp->length - 1;
   BioseqUnlock(bsp);
   SeqIdFree(sip);
   if (stop >= to)
      to = stop;
   else
      numseg++;
   diff2 = to - stop;
   if (numseg == 0)
      return;
   sip = dsp->ids;
   prediff1 = diff1;
   prediff2 = diff2;
   for (i=0; i<numrows; i++)
   {
      bsp = BioseqLockById(sip);
      if (dsp->strands[i] == Seq_strand_minus)
      {
         if (dsp->starts[i]+dsp->lens[0]+diff1 > bsp->length)
            diff1 = bsp->length - (dsp->starts[i] + dsp->lens[0]);
         if (dsp->starts[(dsp->numseg-1)*dsp->dim+i] > diff2)
            diff2 = dsp->starts[(dsp->numseg-1)*dsp->dim+i];
      } else
      {
         if (dsp->starts[i] < diff1)
            diff1 = dsp->starts[i];
         if (dsp->starts[(dsp->numseg-1)*dsp->dim+i]+dsp->lens[dsp->numseg-1]+diff2 > bsp->length)
            diff2 = bsp->length - (dsp->starts[(dsp->numseg-1)*dsp->dim+i] + dsp->lens[dsp->numseg-1]);
      }
      sip = sip->next;
      BioseqUnlock(bsp);
   }
   if (diff1 == 0 && prediff1 != 0)
      numseg--;
   if (diff2 == 0 && prediff2 != 0)
      numseg--;
   if (numseg == 0)
      return;
   dsp_new = DenseSegNew();
   dsp_new->dim = dsp->dim;
   dsp_new->numseg = dsp->numseg+numseg;
   dsp_new->starts = (Int4Ptr)MemNew(dsp_new->dim*dsp_new->numseg*sizeof(Int4));
   dsp_new->lens = (Int4Ptr)MemNew(dsp_new->numseg*sizeof(Int4));
   dsp_new->strands = (Uint1Ptr)MemNew(dsp_new->dim*dsp_new->numseg*sizeof(Uint1));
   seg = 0;
   if (diff1 > 0)
   {
      for (j=0; j<dsp->dim; j++)
      {
         AlnMgr2GetNthSeqRangeInSA(sap, j+1, &start, &stop);
         if (dsp->strands[j] == Seq_strand_minus)
            dsp_new->starts[j] = stop+1;
         else
            dsp_new->starts[j] = start-diff1;
         dsp_new->strands[j] = dsp->strands[j];
      }
      dsp_new->lens[0] = diff1;
      seg++;
   }
   for (i=0; i<dsp->numseg; i++)
   {
      for (j=0; j<dsp->dim; j++)
      {
         dsp_new->starts[(seg)*dsp->dim+j] = dsp->starts[i*dsp->dim+j];
         dsp_new->strands[(seg)*dsp->dim+j] = dsp->strands[i*dsp->dim+j];
      }
      dsp_new->lens[seg] = dsp->lens[i];
      seg++;
   }
   if (diff2 > 0)
   {
      for (j=0; j<dsp->dim; j++)
      {
         AlnMgr2GetNthSeqRangeInSA(sap, j+1, &start, &stop);
         if (dsp->strands[j] == Seq_strand_minus)
            dsp_new->starts[seg*dsp->dim+j] = start-diff2;
         else
            dsp_new->starts[seg*dsp->dim+j] = stop+1;
         dsp_new->strands[seg*dsp->dim+j] = dsp->strands[j];
      }
      dsp_new->lens[seg] = diff2;
   }
   dsp_new->ids = dsp->ids;
   dsp->ids = NULL;
   DenseSegFree(dsp);
   sap->segs = (Pointer)dsp_new;
   SAIndex2Free2(sap->saip);
   sap->saip = NULL;
   AlnMgr2IndexSingleChildSeqAlign(sap);
}

/* SECTION 10 */
/***************************************************************************
*
*  AlnMgr2PadConservatively extends an alignment so that the whole of
*  all sequences is included. If two sequences have tails on the same
*  side, they are each aligned with columns of all gaps:
*
*   <-new aln region->
*   xxxxxxxx----------xxxxxxxxxxxxxxxxxxxx
*   --------xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
*
*  This function returns a newly allocated alignment and doesn't change
*  the original (except for indexing). If the extension was not done for
*  some reason, the function returns NULL;
*
***************************************************************************/
NLM_EXTERN SeqAlignPtr AlnMgr2PadConservatively(SeqAlignPtr sap)
{
   AMAlignIndex2Ptr  amaip;
   BioseqPtr         bsp;
   Int4              ctr1;
   Int4              ctr2;
   DenseSegPtr       dsp;
   DenseSegPtr       dsp_new;
   Int4              i;
   Int4              j;
   Int4Ptr           lenarray;
   Int4              n1;
   Int4              n2;
   Int4              newseg;
   SeqAlignPtr       sap_new;
   SeqIdPtr          sip;
   Int4              start;
   Int4              stop;
   Uint1             strand;

   if (sap == NULL || sap->next != NULL)
      return NULL;
   if (sap->saip == NULL)
      AlnMgr2IndexSeqAlign(sap);
   if (sap->saip->indextype == INDEX_PARENT)
   {
      amaip = (AMAlignIndex2Ptr)(sap->saip);
      if (amaip->alnstyle == AM2_LITE)
         return NULL;
      dsp = (DenseSegPtr)(amaip->sharedaln->segs);
   } else
      dsp = (DenseSegPtr)(sap->segs);
   newseg = 0;
   lenarray = (Int4Ptr)MemNew(dsp->dim*sizeof(Int4));
   n1 = n2 = 0;
   for (i=0; i<dsp->dim; i++)
   {
      sip = AlnMgr2GetNthSeqIdPtr(sap, i+1);
      bsp = BioseqLockById(sip);
      lenarray[i] = bsp->length;
      BioseqUnlock(bsp);
      SeqIdFree(sip);
      AlnMgr2GetNthSeqRangeInSA(sap, i+1, &start, &stop);
      if (start > 0)
      {
         n1++;
         newseg++;
      }
      if (stop < lenarray[i]-1)
      {
         newseg++;
      }
   }
   if (newseg == 0)
   {
      MemFree(lenarray);
      return NULL;
   }
   dsp_new = DenseSegNew();
   dsp_new->numseg = dsp->numseg + newseg;
   dsp_new->dim = dsp->dim;
   dsp_new->ids = SeqIdDupList(dsp->ids);
   dsp_new->starts = (Int4Ptr)MemNew(dsp_new->numseg*dsp_new->dim*sizeof(Int4));
   dsp_new->lens = (Int4Ptr)MemNew(dsp_new->numseg*sizeof(Int4));
   dsp_new->strands = (Uint1Ptr)MemNew(dsp_new->numseg*dsp_new->dim*sizeof(Uint1));
   n2 = n1+dsp->numseg;
   ctr1 = 0;
   ctr2 = 0;
   for (i=0; i<dsp->dim; i++)
   {
      AlnMgr2GetNthSeqRangeInSA(sap, i+1, &start, &stop);
      strand = AlnMgr2GetNthStrand(sap, i+1);
      if (strand == Seq_strand_minus && lenarray[i]-1-stop > 0)
      {
         for (j=0; j<ctr1; j++)
         {
            dsp_new->starts[dsp->dim*j+i] = -1;
            dsp_new->strands[dsp->dim*j+i] = strand;
         }
         dsp_new->starts[dsp->dim*ctr1+i] = stop+1;
         dsp_new->lens[ctr1] = lenarray[i]-1-stop;
         dsp_new->strands[dsp->dim*ctr1+i] = strand;
         for (j=ctr1+1; j<n1; j++)
         {
            dsp_new->starts[dsp->dim*j+i] = -1;
            dsp_new->strands[dsp->dim*j+i] = strand;
         }
         ctr1++;
      } else if (strand == Seq_strand_plus && start > 0)
      {
         for (j=0; j<ctr1; j++)
         {
            dsp_new->starts[dsp->dim*j+i] = -1;
            dsp_new->strands[dsp->dim*j+i] = strand;
         }
         dsp_new->starts[dsp->dim*ctr1+i] = 0;
         dsp_new->lens[ctr1] = start;
         dsp_new->strands[dsp->dim*ctr1+i] = strand;
         for (j=ctr1+1; j<n1; j++)
         {
            dsp_new->starts[dsp->dim*j+i] = -1;
            dsp_new->strands[dsp->dim*j+i] = strand;
         }
         ctr1++;
      } else /* nothing to add on this row, just fill in with -1s */
      {
         for (j=0; j<n1; j++)
         {
            dsp_new->starts[dsp->dim*j+i] = -1;
            dsp_new->strands[dsp->dim*j+i] = strand;
         }
      }
   /* now fill in the non-extended part of the alignment (copy from original) */
      for (j=0; j<dsp->numseg; j++)
      {
         dsp_new->starts[dsp->dim*(j+n1)+i] = dsp->starts[dsp->dim*j+i];
         dsp_new->lens[j+n1] = dsp->lens[j];
         dsp_new->strands[dsp->dim*(j+n1)+i] = dsp->strands[dsp->dim*j+i];
      }
  /* now the other ends */
      if (strand == Seq_strand_minus && start > 0)
      {
         for (j=n2; j<n2+ctr2; j++)
         {
            dsp_new->starts[dsp->dim*j+i] = -1;
            dsp_new->strands[dsp->dim*j+i] = strand;
         }
         dsp_new->starts[dsp->dim*(ctr2+n2)+i] = 0;
         dsp_new->lens[ctr2+n2] = start;
         dsp_new->strands[dsp->dim*(ctr2+n2)+i] = strand;
         for (j=n2+ctr2+1; j<dsp_new->numseg; j++)
         {
            dsp_new->starts[dsp->dim*j+i] = -1;
            dsp_new->strands[dsp->dim*j+i] = strand;
         }
         ctr2++;
      } else if (strand == Seq_strand_plus && lenarray[i]-1-stop > 0)
      {
         for (j=n2; j<ctr2+n2; j++)
         {
            dsp_new->starts[dsp->dim*j+i] = -1;
            dsp_new->strands[dsp->dim*j+i] = strand;
         }
         dsp_new->starts[dsp->dim*(ctr2+n2)+i] = stop+1;
         dsp_new->lens[ctr2+n2] = lenarray[i]-1-stop;
         dsp_new->strands[dsp->dim*(ctr2+n2)+i] = strand;
         for (j=ctr2+n2+1; j<dsp_new->numseg; j++)
         {
            dsp_new->starts[dsp->dim*j+i] = -1;
            dsp_new->strands[dsp->dim*j+i] = strand;
         }
         ctr2++;
      } else /* nothing to add on this row, just fill in with -1s */
      {
         for (j=n2; j<dsp_new->numseg; j++)
         {
            dsp_new->starts[dsp->dim*j+i] = -1;
            dsp_new->strands[dsp->dim*j+i] = strand;
         }
      }
   }
   sap_new = SeqAlignNew();
   sap_new->dim = dsp->dim;
   sap_new->segtype = SAS_DENSEG;
   sap_new->segs = (Pointer)(dsp_new);
   MemFree(lenarray);
   return sap_new;
}

/* SECTION 10 */
/***************************************************************************
*
*  AlnMgr2ExtractPairwiseSeqAlign takes an indexed alignment (parent or
*  child, but must be fully indexed, not lite) and extracts a pairwise
*  subalignment containing the two requested rows. The subalignment is
*  unindexed and may have internal unaligned regions.
*
***************************************************************************/
NLM_EXTERN SeqAlignPtr AlnMgr2ExtractPairwiseSeqAlign(SeqAlignPtr sap, Int4 n1, Int4 n2)
{
   AMAlignIndex2Ptr  amaip;
   DenseSegPtr       dsp;
   DenseSegPtr       dsp_new;
   Int4              i;
   Int4              j;
   Int4              n;
   SeqAlignPtr       sap_new;

   if (sap == NULL || sap->saip == NULL || n1 == n2 || n1 <= 0 || n2 <= 0)
      return NULL;
   if (sap->saip->indextype == INDEX_CHILD)
      dsp = (DenseSegPtr)(sap->segs);
   else
   {
      amaip = (AMAlignIndex2Ptr)(sap->saip);
      dsp = (DenseSegPtr)(amaip->sharedaln->segs);
   }
   if (n1 > dsp->dim || n2 > dsp->dim)
      return NULL;
   n = 0;
   for (i=0; i<dsp->numseg; i++)
   {
      if (dsp->starts[dsp->dim*i+n1-1] == -1 && dsp->starts[dsp->dim*i+n2-1] == -1)
         n++;
   }
   if (n == dsp->numseg) /* no overlap at all */
      return NULL;
   dsp_new = DenseSegNew();
   dsp_new->numseg = dsp->numseg - n;
   dsp_new->starts = (Int4Ptr)MemNew(2*dsp_new->numseg*sizeof(Int4));
   dsp_new->strands = (Uint1Ptr)MemNew(2*dsp_new->numseg*sizeof(Uint1));
   dsp_new->lens = (Int4Ptr)MemNew(dsp_new->numseg*sizeof(Int4));
   dsp_new->dim = 2;
   dsp_new->ids = AlnMgr2GetNthSeqIdPtr(sap, n1);
   dsp_new->ids->next = AlnMgr2GetNthSeqIdPtr(sap, n2);
   j = 0;
   for (i=0; i<dsp->numseg; i++)
   {
      if (dsp->starts[dsp->dim*i+n1-1] > -1 || dsp->starts[dsp->dim*i+n2-1] > -1)
      {
         dsp_new->starts[2*j] = dsp->starts[dsp->dim*i+n1-1];
         dsp_new->starts[2*j+1] = dsp->starts[dsp->dim*i+n2-1];
         dsp_new->strands[2*j] = dsp->strands[n1-1];
         dsp_new->strands[2*j+1] = dsp->strands[n2-1];
         dsp_new->lens[j] = dsp->lens[i];
         j++;
      }
   }
   sap_new = SeqAlignNew();
   sap_new->dim = 2;
   sap_new->type = SAT_PARTIAL;
   sap_new->segtype = SAS_DENSEG;
   sap_new->segs = (Pointer)dsp_new;
   return sap_new;
}

/* SECTION 10 */
static void amconssetfree(AMConsSetPtr acp)
{
   AMConsSetPtr  acp_next;

   while (acp != NULL)
   {
      acp_next = acp->next;
      MemFree(acp->starts);
      MemFree(acp->stops);
      MemFree(acp->strands);
      MemFree(acp);
      acp = acp_next;
   }
}

static int LIBCALLBACK AlnMgr2SortForConsistent(VoidPtr ptr1, VoidPtr ptr2)
{
   AMConsSetPtr  acp1;
   AMConsSetPtr  acp2;
   SAIndex2Ptr   saip1;
   SAIndex2Ptr   saip2;

   acp1 = *((AMConsSetPtr PNTR)ptr1);
   acp2 = *((AMConsSetPtr PNTR)ptr2);
   saip1 = (SAIndex2Ptr)(acp1->sap->saip);
   saip2 = (SAIndex2Ptr)(acp2->sap->saip);
   if (saip1->score == 0)
      saip1->score = AlnMgr2ComputeScoreForSeqAlign(acp1->sap);
   if (saip2->score == 0)
      saip2->score = AlnMgr2ComputeScoreForSeqAlign(acp2->sap);
   if (saip1->score > saip2->score)
      return -1;
   else if (saip1->score < saip2->score)
      return 1;
   else
      return 0;
}

/* SECTION 10 */
/***************************************************************************
*
*  AlnMgr2RemoveInconsistentAlnsFromSet takes an alignment that is
*  indexed at least at the AM2_LITE level, and prunes the child
*  alignments so that the remaining alignments form a consistent,
*  nonoverlapping set. All alignments must have the same number of rows,
*  and they must be the same rows (although not necessarily in the same
*  order). The function uses a simple greedy algorithm to construct the
*  nonoverlapping set, starting with the highest-scoring alignment.
*  If fuzz is negative, the function creates the best nonoverlapping set
*  by actually truncating alignments.
*
***************************************************************************/
NLM_EXTERN void AlnMgr2RemoveInconsistentAlnsFromSet(SeqAlignPtr sap_head, Int4 fuzz)
{
   AMConsSetPtr  acp;
   AMConsSetPtr  acp_head;
   AMConsSetPtr  acp_prev;
   AMConsSetPtr  PNTR acparray;
   DenseSegPtr   dsp;
   Int4          i;
   Int4          j;
   Int4          k;
   Int4          lfuzz;
   SeqAlignPtr   newsap;
   Int4          numrows;
   Int4          numsaps;
   Int4          orientation;
   Int4          row;
   SAIndex2Ptr   saip;
   SeqAlignPtr   salp_head;
   SeqAlignPtr   salp_prev;
   SeqAlignPtr   sap;
   SeqAlignPtr   sapnext;
   Int4          score;
   SeqIdPtr      sip;
   SeqIdPtr      sip_head;
   Uint1         strand;

   lfuzz = fuzz;
   if (fuzz < 0)
      fuzz = 1;
   sap = (SeqAlignPtr)(sap_head->segs);
   if (sap->next == NULL)
      return;
   dsp = (DenseSegPtr)(sap->segs);
   sip_head = dsp->ids;
   numrows = AlnMgr2GetNumRows(sap);
   acp_head = NULL;
   strand = AlnMgr2GetNthStrand(sap, 1);
   numsaps = 0;
   while (sap != NULL)
   {
      if (AlnMgr2GetNumRows(sap) != numrows)
      {
         amconssetfree(acp_head);
         return;
      }
      numsaps++;
      acp = (AMConsSetPtr)MemNew(sizeof(AMConsSet));
      acp->starts = (Int4Ptr)MemNew(numrows*sizeof(Int4));
      acp->stops = (Int4Ptr)MemNew(numrows*sizeof(Int4));
      acp->strands = (Uint1Ptr)MemNew(numrows*sizeof(Uint1));
      acp->which = (Int4Ptr)MemNew(numrows*sizeof(Int4));
      acp->sap = sap;
      if (acp_head != NULL)
      {
         acp_prev->next = acp;
         acp_prev = acp;
      } else
         acp_head = acp_prev = acp;
      sip = sip_head;
      row = AlnMgr2GetFirstNForSip(sap, sip);
      if (row <= 0)
      {
         amconssetfree(acp_head);
         return;
      }
      if (acp->strands[row] != strand)
      {
         sapnext = acp->sap->next;
         acp->sap->next = NULL;
         score = ((SAIndex2Ptr)(acp->sap->saip))->score;
         SeqAlignListReverseStrand(acp->sap);
         AMAlignIndexFreeEitherIndex(acp->sap);
         AlnMgr2IndexSingleChildSeqAlign(acp->sap);
         saip = (SAIndex2Ptr)(acp->sap->saip);
         saip->score = score;
         acp->strands[row] = strand;
         acp->sap->next = sapnext;
      }
      for (i=0; i<numrows; i++)
      {
         acp->which[i] = row;
         AlnMgr2GetNthSeqRangeInSA(sap, i+1, &acp->starts[i], &acp->stops[i]);
         acp->strands[i] = AlnMgr2GetNthStrand(sap, i+1);
      }
      sap = sap->next;
   }
   acparray = (AMConsSetPtr PNTR)MemNew(numsaps*sizeof(AMConsSetPtr));
   acp = acp_head;
   i = 0;
   while (acp != NULL)
   {
      acparray[i] = acp;
      acp = acp->next;
      i++;
   }
   HeapSort(acparray, numsaps, sizeof(AMConsSetPtr), AlnMgr2SortForConsistent);
   /* orientation -1 means that ith is before jth in ALL rows, 1 means ith is after jth in ALL rows */
   for (i=0; i<numsaps; i++)
   {
      if (acparray[i]->used != -1)
      {
         for (j=i+1; j<numsaps; j++)
         {
            orientation = 0;
            for (k=0; acparray[j]->used != -1 && k<numrows; k++)
            {
               if (acparray[i]->strands[k] != acparray[j]->strands[k])
                  acparray[j]->used = -1;
               if (acparray[i]->starts[k] - fuzz < acparray[j]->starts[k])
               {
                  if (acparray[i]->stops[k] - fuzz < acparray[j]->starts[k])
                  {
                     if ((acparray[i]->strands[k] == Seq_strand_plus && orientation == 1) || (acparray[i]->strands[k] == Seq_strand_minus && orientation == -1))
                        acparray[j]->used = -1;
                     else if (orientation == 0)
                     {
                        if (acparray[i]->strands[k] == Seq_strand_minus)
                           orientation = 1;
                        else
                           orientation = -1;
                     }
                  } else
                  {
                     if (lfuzz >= 0) /* just mark it for deletion */
                        acparray[j]->used = -1;
                     else /* truncate it */
                     {
                        if (acparray[j]->stops[k] > acparray[i]->stops[k])
                        {
                           newsap = AlnMgr2GetSubAlign(acparray[j]->sap, acparray[i]->stops[k]+1, acparray[j]->stops[k], k+1, TRUE);
                           SeqAlignFree(acparray[j]->sap);
                           acparray[j]->sap = newsap;
                           acparray[j]->starts[k] = acparray[i]->stops[k]+1;
                        } else
                           acparray[j]->used = -1;
                     }
                  }
               } else if (acparray[i]->starts[k] - fuzz > acparray[j]->starts[k])
               {
                 if (acparray[i]->starts[k] + fuzz > acparray[j]->stops[k])
                  {
                     if ((acparray[i]->strands[k] == Seq_strand_plus && orientation == -1) || (acparray[i]->strands[k] == Seq_strand_minus && orientation == 1))
                        acparray[j]->used = -1;
                     else if (orientation == 0)
                     {
                        if (acparray[i]->strands[k] == Seq_strand_minus)
                           orientation = -1;
                        else
                           orientation = 1;
                     }
                  } else
                  {
                     if (lfuzz >= 0) /* mark for deletion */
                        acparray[j]->used = -1;
                     else /* truncate */
                     {
                        if (acparray[j]->starts[k] < acparray[i]->starts[k])
                        {
                           newsap = AlnMgr2GetSubAlign(acparray[j]->sap, acparray[j]->starts[k], acparray[i]->starts[k]-1, k+1, TRUE);
                           SeqAlignFree(acparray[j]->sap);
                           acparray[j]->sap = newsap;
                           AlnMgr2IndexSingleChildSeqAlign(newsap);
                           acparray[j]->starts[k] = acparray[i]->stops[k]+1;
                        } else
                           acparray[j]->used = -1;
                     }
                  }
               } else
                  acparray[j]->used = -1;
            }
         }
      }
   }
   /* now free all the unused ones, stick the rest back together, reindex, and return */
   salp_head = salp_prev = NULL;
   for (i=0; i<numsaps; i++)
   {
      if (acparray[i]->used == -1)
      {
         SeqAlignFree(acparray[i]->sap);
         acparray[i]->sap = NULL;
      } else
      {
         if (salp_head != NULL)
         {
            salp_prev->next = acparray[i]->sap;
            salp_prev = acparray[i]->sap;
            salp_prev->next = NULL;
         } else
         {
            salp_head = salp_prev = acparray[i]->sap;
            salp_prev->next = NULL;
         }
      }
   }
   amconssetfree(acp_head);
   MemFree(acparray);
   sap_head->segs = (Pointer)(salp_head);
   AMAlignIndex2Free2(sap_head->saip);
   AlnMgr2IndexLite(sap_head);
}

static int LIBCALLBACK AlnMgr2CompareByScore(VoidPtr ptr1, VoidPtr ptr2)
{
   SAIndex2Ptr  saip1;
   SAIndex2Ptr  saip2;
   SeqAlignPtr  sap1;
   SeqAlignPtr  sap2;

   if (ptr1 == NULL || ptr2 == NULL)
      return 0;
   sap1 = *((SeqAlignPtr PNTR) ptr1);
   sap2 = *((SeqAlignPtr PNTR) ptr2);
   saip1 = (SAIndex2Ptr)(sap1->saip);
   saip2 = (SAIndex2Ptr)(sap2->saip);
   if (saip1->score == 0)
      saip1->score = AlnMgr2ComputeScoreForSeqAlign(sap1);
   if (saip2->score == 0)
      saip2->score = AlnMgr2ComputeScoreForSeqAlign(sap2);
   if (saip1->score > saip2->score)
      return -1;
   if (saip1->score < saip2->score)
      return 1;
   return 0;
}

/***************************************************************************
*
*  AlnMgr2FuseSet takes a set of alignments sharing all their rows and orders
*  the alignments, then fuses together any adjacent alignments. If returnall
*  is TRUE, all pieces are returned; if not, then only the largest piece is
*  returned. This function will work best when called after
*  AlnMgr2RemoveInconsistentAlnsFromSet(sap_head, -1).
*
***************************************************************************/
NLM_EXTERN SeqAlignPtr AlnMgr2FuseSet(SeqAlignPtr sap_head, Boolean returnall)
{
   AMAlignIndex2Ptr  amaip;
   DenseSegPtr       dsp_new;
   DenseSegPtr       dsp1;
   DenseSegPtr       dsp2;
   Boolean           found;
   Int4              i;
   Int4              n;
   Int4              numrows;
   Int4              r;
   SeqAlignPtr       sap_keep;
   SeqAlignPtr       sap_keep_head;
   SeqAlignPtr       sap_keep_prev;
   SAIndex2Ptr       saip;
   SeqAlignPtr       PNTR saparray;
   Int4              start1;
   Int4              start2;
   Int4              stop1;
   Int4              stop2;
   Uint1             strand;

   if (sap_head == NULL || sap_head->saip == NULL)
      return NULL;
   AlnMgr2SortAlnSetByNthRowPos(sap_head, 1);
   amaip = (AMAlignIndex2Ptr)(sap_head->saip);
   sap_keep = amaip->saps[0];
   sap_keep_head = sap_keep_prev = NULL;
   numrows = AlnMgr2GetNumRows(sap_keep);
   for (i=1; i<amaip->numsaps; i++)
   {
      /* check for consistency with sap_keep; fuse if possible */
      found = FALSE;
      for (n=0; !found && n<numrows; n++)
      {
         strand = AlnMgr2GetNthStrand(sap_keep, n+1);
         AlnMgr2GetNthSeqRangeInSA(sap_keep, n+1, &start1, &stop1);
         AlnMgr2GetNthSeqRangeInSA(amaip->saps[i], n+1, &start2, &stop2);
         if (strand == Seq_strand_minus)
         {
            if (stop2+1 != start1)
               found = TRUE;
         } else
         {
            if (start2 != stop1+1)
               found = TRUE;
         }
      }
      if (!found) /* fuse together */
      {
         dsp1 = (DenseSegPtr)(sap_keep->segs);
         dsp2 = (DenseSegPtr)(amaip->saps[i]->segs);
         dsp_new = DenseSegNew();
         dsp_new->dim = dsp1->dim;
         dsp_new->numseg = dsp1->numseg+dsp2->numseg;
         dsp_new->starts = (Int4Ptr)MemNew(dsp_new->numseg*dsp_new->dim*sizeof(Int4));
         dsp_new->lens = (Int4Ptr)MemNew(dsp_new->numseg*sizeof(Int4));
         dsp_new->strands = (Uint1Ptr)MemNew(dsp_new->numseg*dsp_new->dim*sizeof(Int4));
         for (n=0; n<dsp_new->numseg; n++)
         {
            for (r=0; r<dsp_new->dim; r++)
            {
               if (n >= dsp1->numseg)
                  dsp_new->starts[r*n*r] = dsp2->starts[r*(n-dsp1->numseg)+r];
               else
                  dsp_new->starts[r*n+r] = dsp1->starts[r*n+r];
               dsp_new->strands[r*n*r] = dsp1->strands[r];
            }
            if (n >= dsp1->numseg)
               dsp_new->lens[n] = dsp2->lens[n-dsp1->numseg];
            else
               dsp_new->lens[n] = dsp1->lens[n];
         }
         SeqAlignFree(amaip->saps[i]);
         amaip->saps[i] = NULL;
      } else /* add next alignment to keepers pile */
      {
         if (sap_keep_head == NULL)
         {
            if (sap_keep != NULL)
            {
               sap_keep_head = sap_keep;
               sap_keep->next = amaip->saps[i];
               sap_keep_prev = amaip->saps[i];
            } else
               sap_keep_head = sap_keep_prev = amaip->saps[i];
         } else
         {
            sap_keep_prev->next = amaip->saps[i];
            sap_keep_prev = amaip->saps[i];
         }
      }
   }
   if (sap_keep_head == NULL || sap_keep_head->next == NULL) /* everything was fused */
      sap_keep_head = sap_keep;
   if (returnall)
   {
      sap_head->segs = (Pointer)(sap_keep_head);
      return sap_keep_head;
   }
   i=0;
   sap_keep = sap_keep_head;
   while (sap_keep != NULL)
   {
      sap_keep = sap_keep->next;
      i++;
   }
   saparray = (SeqAlignPtr PNTR)MemNew(i*sizeof(SeqAlignPtr));
   i = 0;
   sap_keep = sap_keep_head;
   while (sap_keep != NULL)
   {
      saip = (SAIndex2Ptr)(sap_keep->saip);
      saip->score = 0;
      saparray[i] = sap_keep;
      i++;
      sap_keep = sap_keep->next;
   }
   HeapSort(saparray, i, sizeof(SeqAlignPtr), AlnMgr2CompareByScore);
   sap_keep = saparray[0];
   for (n=1; n<i; n++)
   {
      SeqAlignFree(saparray[n]);
   }
   MemFree(saparray);
   return sap_keep;
}

NLM_EXTERN void AlnMgr2FillInUnaligned(SeqAlignPtr sap)
{
   Int4         curr;
   DenseSegPtr  dsp;
   DenseSegPtr  dsp_new;
   Boolean      found;
   Int4         i;
   Int4         j;
   Int4         k;
   Int4         last;
   Int4         n;
   Int4         offset;
   Int4         start;
   Int4         stop;
   Uint1        strand;

   if (sap == NULL || (sap->saip != NULL && sap->saip->indextype != INDEX_CHILD))
      return;
   n = 0;
   dsp = (DenseSegPtr)(sap->segs);
   for (i=0; i<dsp->dim; i++)
   {
      j = 0;
      AlnMgr2GetNthSeqRangeInSA(sap, i, &start, &stop);
      strand = dsp->strands[i];
      last = -1;
      while (j<dsp->numseg-1)
      {
         if (strand == Seq_strand_minus)
         {
            if (last != -1)
            {
               found = FALSE;
               while (j<dsp->numseg && !found)
               {
                  if (dsp->starts[j*dsp->dim+i] != -1)
                  {
                     if (dsp->starts[j*dsp->dim+i]+dsp->lens[j] != last)
                        n++;
                     found = TRUE;
                  }
                  if (!found)
                     j++;
               }
            } else
               last = dsp->starts[j*dsp->dim+i];
         } else
         {
            if (last != -1)
            {
               found = FALSE;
               while (j<dsp->numseg && !found)
               {
                  if (dsp->starts[j*dsp->dim+i] != -1)
                  {
                     if (dsp->starts[j*dsp->dim+i]+dsp->lens[j] != last)
                        n++;
                     found = TRUE;
                  }
                  if (!found)
                     j++;
               }
            } else
            {
               last = dsp->starts[j*dsp->dim+i];
               if (last != -1)
                  last += dsp->lens[j];
            }
         }
      }
   }
   if (n == 0) /* no unaligned regions */
      return;
   dsp_new = DenseSegNew();
   dsp_new->numseg = dsp->numseg + n;
   dsp_new->dim = dsp->dim;
   dsp_new->starts = (Int4Ptr)MemNew(dsp_new->dim*dsp_new->numseg*sizeof(Int4));
   dsp_new->strands = (Uint1Ptr)MemNew(dsp_new->dim*dsp_new->numseg*sizeof(Uint1));
   for (i=0; i<dsp_new->numseg; i++)
   {
      for (j=0; j<dsp_new->dim; j++)
      {
         dsp_new->strands[i*dsp_new->dim+j] = dsp->strands[j];
      }
   }
   dsp_new->ids = SeqIdDupList(dsp->ids);
   dsp_new->lens = (Int4Ptr)MemNew(dsp_new->numseg*sizeof(Int4));
   curr = 0;
   for (j=0; j<dsp->numseg; j++)
   {
      for (i=0; i<dsp->dim; i++)
      {
         offset = 0;
         strand = dsp->strands[i];
         if (dsp->starts[j*dsp->dim+i] == -1)
            dsp_new->starts[curr*dsp_new->dim+i] = -1;
         else
         {
            k = j+1;
            found = FALSE;
            while (k < dsp->numseg)
            {
               if (dsp->starts[k*dsp->dim+i] != -1)
               {
                  found = TRUE;
                  if (strand == Seq_strand_minus)
                  {
                     if (dsp->starts[k*dsp->dim+i] + dsp->lens[k] != dsp->starts[j*dsp->dim+i])
                     {
                        dsp_new->lens[curr+offset] = dsp->starts[j*dsp->dim+i] - dsp->starts[k*dsp->dim+i] - dsp->lens[k];
                        dsp_new->starts[(curr+offset)*dsp->dim+i] = dsp->starts[k*dsp->dim+i] + dsp->lens[k];
                        offset++;
                     }
                  } else
                  {
                     if (dsp->starts[j*dsp->dim+i] + dsp->lens[j] != dsp->starts[k*dsp->dim+i])
                     {
                        dsp_new->lens[curr+offset] = dsp->starts[k*dsp->dim+i] - dsp->starts[j*dsp->dim+i] - dsp->lens[j];
                        dsp_new->starts[(curr+offset)*dsp->dim+i] = dsp->starts[j*dsp->dim+i] + dsp->lens[j];
                     }
                  }
               }
               k++;
            }
         }
      }
      curr = curr + 1 + offset;
   }
   DenseSegFree(dsp);
   sap->segs = (Pointer)(dsp_new);
   AMAlignIndexFreeEitherIndex(sap);
}

/* SECTION 11 -- functions for std-segs */
NLM_EXTERN SeqIdPtr AlnMgr2GetNthSeqIdPtrStdSeg(SeqAlignPtr sap, Int4 n)
{
   SeqLocPtr  slp;
   StdSegPtr  ssp;

   if (sap == NULL || sap->segtype != SAS_STD)
      return NULL;
   ssp = (StdSegPtr)(sap->segs);
   slp = ssp->loc;
   n--;
   while (n > 0)
   {
      if (slp == NULL)
         return NULL;
      slp = slp->next;
      n--;
   }
   return (SeqIdDup(SeqLocId(slp)));
}

NLM_EXTERN Int4 AlignMgr2GetFirstNForStdSeg(SeqAlignPtr sap, SeqIdPtr sip)
{
   Int4       i;
   SeqIdPtr   sip_tmp;
   StdSegPtr  ssp;

   if (sap == NULL || sap->segtype != SAS_STD)
      return -1;
   ssp = (StdSegPtr)(sap->segs);
   sip_tmp = ssp->ids;
   i = 1;
   while (sip_tmp != NULL)
   {
      if (SeqIdComp(sip, sip_tmp) == SIC_YES)
         return i;
      sip_tmp = sip_tmp->next;
      i++;
   }
   return -1;
}

NLM_EXTERN void AlnMgr2GetNthSeqRangeInSAStdSeg(SeqAlignPtr sap, Int4 n, Int4Ptr start, Int4Ptr stop)
{
   SeqLocPtr  slp;
   StdSegPtr  ssp;

   if (start != NULL)
      *start = -1;
   if (stop != NULL)
      *stop = -1;
   if (sap == NULL || sap->segtype != SAS_STD)
      return;
   ssp = (StdSegPtr)(sap->segs);
   slp = ssp->loc;
   n--;
   while (n > 0)
   {
      if (slp == NULL)
         return;
      slp = slp->next;
      n--;
   }
   if (slp == NULL)
      return;
   if (start != NULL)
      *start = SeqLocStart(slp);
   if (stop != NULL)
      *stop = SeqLocStop(slp);
}


/***************************************************************************
*
*   AlnMgr2GetSeqRangeForSipInSAStdSeg  returns the smallest and largest sequence
*  coordinates in in a Std-Seg seqalign for a given Sequence Id.  Also return the 
*  strand type.  Either start, stop or strand can be NULL to only retrieve some of them.
*  If start and stop are -1, there is an error (not a std-seg), the SeqID does not participate in this
*  alignment or the alignment is one big insert on that id.  Returns true if the sip was found
*  in the alignment with real coordinates, i.e. *start would not be -1.  RANGE
*
***************************************************************************/
NLM_EXTERN Boolean AlnMgr2GetSeqRangeForSipInSAStdSeg(SeqAlignPtr sap, SeqIdPtr sip, Int4Ptr start, Int4Ptr stop, Uint1Ptr strand)
{
    Int4        c_start, c_stop;
    Uint1       c_strand;
    StdSegPtr   ssp;
    Boolean     range_found = FALSE;
    Boolean     strands_inconsistent = FALSE;

    if (start) *start = -1;
    if (stop)  *stop  = -1;
    if (strand) *strand = Seq_strand_unknown;
    
    if (sap->segtype != SAS_STD)
        return FALSE;

    ssp = (StdSegPtr)(sap->segs);
    while (ssp) { 
        if (AlnMgr2GetSeqRangeForSipInStdSeg(ssp, sip, &c_start, &c_stop, &c_strand, NULL) &&
            c_start != -1) /* skip inserts on our bioseq */
        {
             range_found = TRUE;
                
            if (start) {
                if (*start == -1) {
                    *start = c_start;
                } else {
                    *start = MIN(*start, c_start);
                }
            }
            if (stop) {
                *stop = MAX(*stop, c_stop);
            }
            if (strand && ! strands_inconsistent) {
            /* if strands are different each time, ignore them. */
                if (*strand != Seq_strand_unknown && *strand != c_strand) {
                    *strand = Seq_strand_unknown;
                    strands_inconsistent = TRUE;
                } else {
                    *strand = c_strand;
                }
            }
        }
        ssp = ssp->next;
    }
    return range_found;
}


/***************************************************************************
*
*   AlnMgr2GetSeqRangeForSipInStdSeg  returns the start and stop sequence
*  coordinates in a Std-Segment for a given Sequence Id.  Also return the 
*  strand type.  Either start, stop or strand can be NULL to only retrieve some of them.
*  If start and stop are -1, the SeqID was not found in this segment.  
*  Returns true if the sip was found, even if it is a gap (start, stop = -1).  RANGE
*
***************************************************************************/
NLM_EXTERN Boolean AlnMgr2GetSeqRangeForSipInStdSeg(
    StdSegPtr   ssp, 
    SeqIdPtr    sip, 
    Int4Ptr     start, 
    Int4Ptr     stop, 
    Uint1Ptr    strand,
    Uint1Ptr    segType) /* AM_SEQ, AM_GAP, AM_INSERT */
{
    SeqLocPtr   loc;
    Uint1       m_strand;
    Int4        m_start, m_stop, m_swap;
    Boolean     s_present = FALSE;
    Boolean     m_present = FALSE;
    Boolean     found_id = FALSE;
    
    for ( loc = ssp->loc;
          loc != NULL;  
          loc = loc->next ) {
    /* One SeqLoc for each Sequence aligned by this segment. */
        /* find the one that matches the sip parameter. */
        if (SeqIdForSameBioseq(sip, SeqLocId(loc))) {
            m_strand = SeqLocStrand(loc);
            m_start  = SeqLocStart(loc);
            m_stop   = SeqLocStop(loc);
            /* Might have to reverse the order of start and stop on
               minus strands so that start is less than stop. */
            if (m_start > m_stop) {
              m_swap  = m_start;
              m_start = m_stop;
              m_stop = m_swap;
            }
            if (start)  *start  = m_start;
            if (stop)   *stop   = m_stop;
            if (strand) *strand = m_strand;
            if (m_start != -1)
                m_present = TRUE;
                
            /* found our sequence in this segment. */
            found_id = TRUE;
        } else { /* a different sequence */
            if (SeqLocStart(loc) != -1)
                s_present = TRUE;
        }
    }
    
    if (segType) {
        if (m_present && s_present)
            *segType = AM_SEQ;
        else if (!m_present && s_present)
            *segType = AM_INSERT;
        else if (m_present && !s_present)
            *segType = AM_GAP;
        else
            *segType = AM_GAP; /* start will be -1 */
    }
    return found_id;
}


/***************************************************************************
*
*   AlnMgr2GetNthStdSeg  returns the a pointer to the Nth segment of
*   a standard segment alignment.  Numbering starts with 1.
*   returns NULL if not n segments or is not a std-seg aligment.
*   Useful to pass its return value to AlnMgr2GetSeqRangeForSipInStdSeg()
*
***************************************************************************/
NLM_EXTERN StdSegPtr AlnMgr2GetNthStdSeg(SeqAlignPtr sap, Int2 n)
{
    StdSegPtr   ssp;
	Int2        i;

    if (sap == NULL || sap->segtype != SAS_STD || n < 1)
        return NULL;
      
    i = 1;
    ssp = (StdSegPtr)(sap->segs);
    while(ssp)
    {
        if (i == n)
            return ssp;
        ++i;
        ssp = ssp->next;
    }

    return NULL;
}

/***************************************************************************
*
*  AlnMgr2GetNumStdSegs returns the number of segments in a standar-seg alignment.
*   returns -1 if sap is null or not a standard-seg alignment.
*
***************************************************************************/
NLM_EXTERN Int4 AlnMgr2GetNumStdSegs(SeqAlignPtr sap)
{
    Int4        seg_count = 0;
    StdSegPtr   ssp;
    
    if (sap == NULL || sap->segtype != SAS_STD)
        return -1;
        
    ssp = (StdSegPtr)(sap->segs);
	while(ssp)
	{
		++seg_count;
		ssp = ssp->next;
	}
	return seg_count;
}   
   
static SeqLocPtr AlnMgr2GetLongestSeqLoc(SeqAlignPtr sap)
{
   Int4       longest;
   Int4       n;
   SeqLocPtr  slp;
   SeqLocPtr  slp_longest;
   StdSegPtr  ssp;

   if (sap == NULL || sap->segtype != SAS_STD)
      return NULL;
   longest = -1;
   ssp = (StdSegPtr)(sap->segs);
   slp = ssp->loc;
   while (slp != NULL)
   {
      n = SeqLocLen(slp);
      if (n > longest)
      {
         slp_longest = slp;
         longest = n;
      }
      slp = slp->next;
   }
   return slp_longest;
}

/***************************************************************************
*
*  The two mapping functions act a little differently for std-segs. The
*  alignment coordinates are 1:1 linearly correlated with the longest
*  seqloc in the set; the others may be significantly shorter.
*  The mapping functions deal with % lengths, and map those instead of
*  coordinates (which may not be linear);
*
***************************************************************************/
NLM_EXTERN Int4 AlnMgr2MapBioseqToSeqAlignStdSeg(SeqAlignPtr sap, Int4 n, Int4 pos)
{
   SeqLocPtr  slp;
   SeqLocPtr  slp_longest;
   StdSegPtr  ssp;
   Int4       start1;
   Int4       start2;
   Int4       stop1;
   Int4       stop2;

   if (sap == NULL || sap->segtype != SAS_STD)
      return -1;
   slp_longest = AlnMgr2GetLongestSeqLoc(sap);
   start1 = SeqLocStart(slp_longest);
   stop1 = SeqLocStop(slp_longest);
   ssp = (StdSegPtr)(sap->segs);
   slp = ssp->loc;
   n--;
   while (n > 0)
   {
      if (slp == NULL)
         return -1;
      n--;
      slp = slp->next;
   }
   if (slp == NULL)
      return -1;
   start2 = SeqLocStart(slp);
   stop2 = SeqLocStop(slp);
   if (start2 == -1) /* NULL */
      return -1;
   return (((stop1-start1)*(pos - start2))/(stop2-start2));
}

NLM_EXTERN Int4 AlnMgr2MapSeqAlignToBioseqStdSeg(SeqAlignPtr sap, Int4 n, Int4 pos)
{
   SeqLocPtr  slp;
   SeqLocPtr  slp_longest;
   StdSegPtr  ssp;
   Int4       start1;
   Int4       start2;
   Int4       stop1;
   Int4       stop2;

   if (sap == NULL || sap->segtype != SAS_STD)
      return -1;
   slp_longest = AlnMgr2GetLongestSeqLoc(sap);
   start1 = SeqLocStart(slp_longest);
   stop1 = SeqLocStop(slp_longest);
   ssp = (StdSegPtr)(sap->segs);
   slp = ssp->loc;
   n--;
   while (n > 0)
   {
      if (slp == NULL)
         return -1;
      n--;
      slp = slp->next;
   }
   if (slp == NULL)
      return -1;
   start2 = SeqLocStart(slp);
   stop2 = SeqLocStop(slp);
   if (start2 == -1)  /* NULL */
      return -1;
   return (start2 + ((stop2-start2)*(pos-start1))/(stop1-start1));
}

NLM_EXTERN Int4 AlnMgr2GetAlnLengthStdSeg(SeqAlignPtr sap)
{
   SeqLocPtr  slp_longest;

   if (sap == NULL || sap->segtype != SAS_STD)
      return -1;
   slp_longest = AlnMgr2GetLongestSeqLoc(sap);
   return (SeqLocLen(slp_longest));
}
