/*   $Id: blocks.h,v 6.2 1999/11/26 15:42:24 vakatov Exp $
* ===========================================================================
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
*  any work or product based on this material
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
* File Name:  blocks.h
*
* Author:  Sarah Wheelan
*
* Version Creation Date:   
*
* $Revision: 6.2 $
*
* File Description: Creating an editable version of a seqalign
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: blocks.h,v $
* Revision 6.2  1999/11/26 15:42:24  vakatov
* Fixed for the C++ and/or MSVC DLL compilation
*
* Revision 6.1  1999/07/06 19:49:14  kans
* initial public checkin
*
* Revision 1.19  1999/07/01 20:58:34  wheelan
* added include salpedit.h, added FlipSequence, CalcMinusBounds, PropagateStrandInfo, and Patrick Durand`s SABlock printing function
*
* Revision 1.18  1999/06/25 20:56:39  wheelan
* took out unnecessary includes
*
* Revision 1.17  1999/06/24 20:44:45  wheelan
* added IndexNewBlocks to correctly index SABlocks made from DenseDiags
*
* Revision 1.16  1999/06/21 12:07:28  wheelan
* added SeqAlignListMergeAll function
*
* Revision 1.15  1999/06/11 17:02:54  wheelan
* Added DeBlockify and GetSeqIdList functions
*
* Revision 1.13  1999/06/07 20:59:30  lewisg
* fix typo in traverser fcn typedefs
*
* Revision 1.12  1999/05/29 00:09:15  lewisg
* new editing functions
*
* Revision 1.11  1999/05/24 23:10:50  lewisg
* more initialization for AddEmptyBlocks
*
* Revision 1.9  1999/05/24 15:33:20  lewisg
* make Segment.gap hold gap count
*
* Revision 1.8  1999/05/21 14:53:59  lewisg
* added functions, removed ssp
*
*
* ==========================================================================
*/


#ifndef _NCBI_DDV_BLOCKS_
#define _NCBI_DDV_BLOCKS_

#include <ncbi.h>
#include <salsap.h>
#include <salutil.h>
#include <objseq.h>
#include <salpedit.h>

#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif

#ifdef __cplusplus
extern "C" {
#endif


/*
  Gapless segment for the Display SeqAlign
 */
typedef struct _segment {
    Int4       from;
    Int4       to;
    Int4       gap;
    /* lyg: we should delete sip.  this is kept elsewhere */
    SeqIdPtr   sip;
    /* lyg: deleted pointer to standard seg */
    Int4       BspID;
    struct _segment* next;
    struct _segment* prev;
    struct _segment* bsp_next;
    struct _segment* bsp_prev;
    Uint1      strand;
    /* lyg: visible should be deleted also */
    Uint1      visible;
    Boolean    link;
    Boolean    move;   /* lyg: does this Segment have a pending edit? */
} Segment, PNTR SegmentPtr;

/*
  Head of Linked List for Each Segment
  */
typedef struct _seqalignblock {
    Int4        from;
    Int4        to;
    SegmentPtr  segp_head;
    Int4        SegID;
    Int4        AlignID;
    Int4        RegionID;
    struct _seqalignblock* next;
    struct _seqalignblock* prev;
    Boolean     aligned;  /* lyg: indicates that the column is aligned */
    /* lyg: visible and highlighted should be kept elsewhere */
    Uint1       visible;
    Boolean     highlighted;
} SABlock, PNTR SABlockPtr;

/*
  Anciliary Information for each "row" Bioseq : Not Part of SeqAlign 
 */
typedef struct _ddv_seginfo {
    Int4 BspID;
    SeqPortPtr spp;
    SeqIdPtr sip;
    BioseqPtr bsp;              /* Optional, Only if locked */
    struct _ddv_segInfo * next;
    Uint2 entityID;
    Uint2 itemID;
} DDV_SegInfo, * DDV_SegInfoPtr;

/* Transform an N-dim Multiple SeqAlign into the Block SeqAlign (dsp only) */
NLM_EXTERN SABlockPtr Blockify(SeqAlignPtr sap);

NLM_EXTERN SABlockPtr IndexBlocks(SABlockPtr sabp);
NLM_EXTERN SABlockPtr IndexNewBlocks(SABlockPtr sabp);
NLM_EXTERN void GetBlockInfo(SABlockPtr sabp, Int4Ptr length, Int4Ptr numbioseqs, Int4Ptr numblocks, Int4Ptr validate);
NLM_EXTERN void CleanupBlocks(SABlockPtr sabp);
NLM_EXTERN SABlockPtr SAMergeBlocks(SABlockPtr sabp1, SABlockPtr sabp2);
NLM_EXTERN Int4 GetBlockOrientation(Int4Ptr shift, SegmentPtr segp1, SegmentPtr segp2, Int4 index1, Int4 index2);
NLM_EXTERN SABlockPtr SquishBlocks(SABlockPtr sabp);
NLM_EXTERN SABlockPtr SARemoveSequence(Int4 BspID, SeqIdPtr sip, SABlockPtr sabp);
NLM_EXTERN SABlockPtr RearrangeSegments(Int4 BspID, SABlockPtr sabp, Int4 position);
NLM_EXTERN SABlockPtr TeenyBlock(SABlockPtr sabp, Int4 from, Int4 to);
NLM_EXTERN SABlockPtr SASplitBlock(SABlockPtr sabp, Int4 from);
NLM_EXTERN SABlockPtr AddEmptyBlocks(SABlockPtr sabp, Int4 n, Boolean beginning, Boolean end);
NLM_EXTERN SABlockPtr AddEmptySegments(SABlockPtr sabp);
NLM_EXTERN SABlockPtr FillInUnaligned(SABlockPtr sabp);
NLM_EXTERN SeqAlignPtr DeBlockify(SABlockPtr sabp);
NLM_EXTERN SeqIdPtr GetSeqIdList(SABlockPtr sabp);
NLM_EXTERN SeqAlignPtr SeqAlignListMergeAll(SeqAnnotPtr sap);
NLM_EXTERN SegmentPtr PropagateStrandInfo(SegmentPtr segp);


/*******************************************************************************

  Function : blk_PrintSABP()
  
  Purpose : display an indexed seqalign as a matrix (debug purpose only)
  
  Parameters :  sabp;header of the indexed seqalign
                                
  Return value : -

*******************************************************************************/
NLM_EXTERN void blk_PrintSABP(SABlockPtr sabp);

/*****************************************************************************
*
*   Notes on changes by lyg:
*   - removed ssp in Segment
*   - initialized RegionID in Blockify
*   - added SAReturnBlock() and SAReturnSegment()
*   - added traversal routine
*   - set SABlock.gap equal to the length of the gap
*   - preserve RegionID in AddEmptyBlocks(), initialize more members
*
*****************************************************************************/


/*****************************************************************************
*
*   For a given column, return a pointer to the block that contains the
*   position.  Returns NULL if a containing block is not found. 
*
*****************************************************************************/

NLM_EXTERN SABlock * SAReturnBlock(SABlock *sabpHead, Int4 lColumn);

/*****************************************************************************
*
*   For a given position, return a pointer to the Segment that contains the
*   position.  Returns NULL if a containing Segment is not found. Note that
*   the lRow argument is position, not BspID.
*
*****************************************************************************/

NLM_EXTERN Segment * SAReturnSegment(SABlock *sabpHead, Int4 lColumn, Int4 lRow);

/*****************************************************************************
*
*   Generic Segment traversal function with callback
*
*****************************************************************************/

typedef void (*pfnSAFunction)(Segment *pSegment, void *pData);

NLM_EXTERN Int4 SATraverse(SABlock *sabpHead, pfnSAFunction pFunction, void *pData);

/*****************************************************************************
*
*   Traverser to clear the move bit
*
*****************************************************************************/

NLM_EXTERN void SAClearMove(Segment *pSegment, void *pData);

/*****************************************************************************
*
*   Generic SABlock traversal function with callback
*
*****************************************************************************/

typedef void (*pfnSABlockFunction)(SABlock *sabp, void *pData);

NLM_EXTERN Int4 SATraverseBlock
(SABlock *sabpHead, pfnSABlockFunction pFunction, void *pData);

/*****************************************************************************
*
*   Traverser to find the highest valued RegionID
*
*****************************************************************************/

NLM_EXTERN void SARegionBounds(SABlock *sabp, void *pData);


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

