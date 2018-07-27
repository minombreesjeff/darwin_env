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
* File Name:  alignmgr2.h
*
* Author:  Sarah Wheelan
*
* Version Creation Date:  10/01 
*
* $Revision: 6.21 $
*
* File Description: SeqAlign indexing, access, and manipulation functions
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: alignmgr2.h,v $
* Revision 6.21  2003/10/09 13:46:39  rsmith
* Add AlnMgr2GetFirstNForSipList.
*
* Revision 6.20  2003/04/23 20:37:06  rsmith
* Added four functions in section 11 to allow examination of Std-Seg alignments.
*
* Revision 6.19  2003/03/31 20:17:11  todorov
* Added AlnMgr2IndexSeqAlignEx
*
* Revision 6.18  2002/08/07 21:57:33  kans
* added AlignMgr2GetFirstNForStdSeg
*
* Revision 6.17  2002/07/11 14:35:51  kans
* fixed Mac complaints about prototypes
*
* Revision 6.16  2002/07/11 12:55:33  wheelan
* added support for std-seg alignments
*
* Revision 6.15  2002/05/21 12:26:25  wheelan
* added n5 field to AMSmallPtr
*
* Revision 6.14  2002/04/09 18:21:55  wheelan
* changed params for AlnMgr2IndexAsRows
*
* Revision 6.13  2002/03/04 17:19:29  wheelan
* added AlnMgr2FuseSet, changed behavior of RemoveInconsistent
*
* Revision 6.12  2002/01/30 19:12:20  wheelan
* added RemoveInconsistentAlnsFromSet, ExtractPairwiseSeqAlign, changed behavior of GetSubAlign, changed structures and behavior of GetNextAlnBit, added GetInterruptInfo
*
* Revision 6.11  2001/12/28 22:53:46  wheelan
* added AlnMgr2DupAlnAndIndexes, changed amaip struct
*
* Revision 6.10  2001/12/14 12:38:35  wheelan
* added functions for ddv
*
* Revision 6.9  2001/11/30 16:55:07  wheelan
* added AlnMgr2PadConservatively
*
* Revision 6.8  2001/11/29 17:37:02  wheelan
* added ExtendToCoords and MergeTwoAlignments
*
* Revision 6.7  2001/11/13 14:35:33  wheelan
* added new field to AMSmall structure
*
* Revision 6.6  2001/11/08 19:55:32  wheelan
* added AlnMgr2GetNthRowSpanInSA
*
* Revision 6.5  2001/10/23 12:13:24  wheelan
* added #define AM_HARDSTOP
*
* Revision 6.4  2001/10/16 12:00:00  wheelan
* added GetParent and FreeEitherIndex
*
* Revision 6.3  2001/10/08 18:43:25  wheelan
* added comments
*
* Revision 6.2  2001/10/03 18:12:51  wheelan
* changed some colliding defines
*
* Revision 6.1  2001/10/03 14:20:30  wheelan
* initial checkin
*
* ==========================================================================
*/

#ifndef _ALIGNMGR2_
#define _ALIGNMGR2_

#include <ncbi.h>
#include <sqnutils.h>
#include <salutil.h>
#include <salpedit.h>
#include <samutil.h>
#include <sequtil.h>

#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* defines for frequency matrix sizes */
#define AM_NUCSIZE   6
#define AM_PROTSIZE  26

/* max seqport window */
#define AM_SEQPORTSIZE 20000

/* defines for AlnMgr2ComputeScoreForPairwiseSeqAlign */
#define AM_GAPOPEN  -11
#define AM_GAPEXT   -1

/* defines for saip->indextype */
#define INDEX_CHILD   1
#define INDEX_PARENT  2

/* defines for amp->type and interrupt->type */
#define AM_SEQ        1
#define AM_GAP        2
#define AM_INSERT     3
#define AM_UNALIGNED  4

#define AM2_LEFT_TAIL   0
#define AM2_RIGHT_TAIL  1

#define AM2_LEFT        1
#define AM2_RIGHT       2

/* defines for AlnMgr2AddInNewSA */
#define AM_START    -1
#define AM_STOP      1
#define AM_HARDSTOP  3

/* defines for amaip->alnstyle */
#define AM_CONTIG_LINEAR  1
#define AM_INTER_LINEARF  2
#define AM_INTER_LINEAR   3
#define AM_CONTIG_NONLIN  4
#define AM_INTER_NONLINF  5
#define AM_INTER_NONLIN   6
#define AM2_LITE          7
#define AM2_FULLINDEX     8

/* defines for AMEdge.used */
#define AM_NOTUSED   0
#define AM_USED      1
#define AM_CONFLICT  2
#define AM_USED2     3

typedef struct am_sarowdat {
   Uint2Ptr  sect;
   Uint2Ptr  unsect;
   Uint2Ptr  insect;
   Uint2Ptr  unaligned;
   Uint2     numsect;
   Uint2     numunsect;
   Uint2     numinsect;
   Uint2     numunaln;
} SARowDat2, PNTR SARowDat2Ptr;

typedef struct am_saindex {
   Uint1                  indextype;
   SeqAlignIndexFreeFunc  freefunc;
   Uint4Ptr               aligncoords;
   Int4                   anchor;
   SARowDat2Ptr           PNTR srdp;
   Int4                   numrows;
   Int4                   numseg;
   Int4                   numunaln;
   Uint4Ptr               unaln;
   Int4                   numinchain;
   Int4                   numsplitaln;
   Int4                   score;
   Boolean                aligned;
   SeqAlignPtr            top;
   Int4                   tmp;
} SAIndex2, PNTR SAIndex2Ptr;

NLM_EXTERN Boolean LIBCALLBACK SAIndex2Free2(VoidPtr index);

typedef struct am_interrinfo {
   Uint1    strand;
   Int4Ptr  starts;
   Int4Ptr  lens;
   Int4Ptr  types;
   Int4     num;
} AMInterrInfo, PNTR AMInterrInfoPtr;

NLM_EXTERN void AlnMgr2FreeInterruptInfo(AMInterrInfoPtr interrupt);

typedef struct am_insert {
   Int4Ptr  starts;
   Int4Ptr  lens;
   Int4     which_side;
} AMInsert, PNTR AMInsertPtr;

typedef struct am_unalign {
   Int4Ptr  starts;
   Int4Ptr  lens;
   Int4     which_side;
} AMUnalign, PNTR AMUnalignPtr;

typedef struct am_parcel {
   Int4          alnstart;
   Int4          sap_source;
   AMUnalignPtr  unaligned;
   AMInsertPtr   inserts;
} AMParcel, PNTR AMParcelPtr;
   
typedef struct am_alignindex {
   Uint1                  indextype;
   SeqAlignIndexFreeFunc  freefunc;
   Uint2                  alnstyle;
   Int4                   anchor;
   Int4                   numrows;
   SeqIdPtr               PNTR ids;  /* one SeqId per row */
   Int4                   numsaps;
   SeqAlignPtr            PNTR saps;
   Boolean                PNTR aligned; /* for each sap -- is it used in the overall alignment? */
   SeqAlignPtr            sharedaln;
} AMAlignIndex2, PNTR AMAlignIndex2Ptr;

NLM_EXTERN Boolean LIBCALLBACK AMAlignIndex2Free2(VoidPtr index);

typedef struct {
   Int4  insertlen;
   Int4  unalnlen;
   Int4  segnum;
   Int4  row;
   Int4  which_side;
} AMInterrupt, PNTR AMInterruptPtr;

typedef struct am_msg2 {
/* fields filled in by calling function */
   Int4            from_aln; /* from is in alignment coordinates */
   Int4            to_aln;   /* to is in alignment coordinates */
   Int4            row_num; /* which row the function wants to retrieve */

/* fields filled in by AlnMgr2GetNextAlnBit */
   Int4            from_row;
   Int4            to_row;
   Uint1           strand;
   Uint1           type; /* AM_SEQ or AM_GAP */
   AMInterruptPtr  left_interrupt;
   AMInterruptPtr  right_interrupt;

/* fields used internally */
   Int4            len;
   Int4            real_from;
} AlnMsg2, PNTR AlnMsg2Ptr;

NLM_EXTERN AlnMsg2Ptr AlnMsgNew2(void);
NLM_EXTERN AlnMsg2Ptr AlnMsgFree2(AlnMsg2Ptr amp);
NLM_EXTERN void AlnMsgReNew2(AlnMsg2Ptr amp);

typedef struct am_small {
   Int4  n1;
   Int4  n2;
   Int4  n3;
   Int4  n4;
   Int4  n5;
   struct am_small PNTR next;
} AM_Small2, PNTR AM_Small2Ptr;

typedef struct am_consistset {
   Int4         numrows;
   Int4Ptr      starts;
   Int4Ptr      stops;
   Uint1Ptr     strands;
   SeqAlignPtr  sap;
   Int4         used;
   Int4Ptr      which;
   struct am_consistset PNTR next;
} AMConsSet, PNTR AMConsSetPtr;

typedef struct am_coreinf {
   Int4      start_core;
   Int4      len;
   Int4      sap_num;
   Int4      row;
   SeqIdPtr  sip;
   Int4      start_aln;
   Int4      left;
   Int4      right;
} AM_Core, PNTR AM_CorePtr;

typedef struct am_rowinf {
   Int4  from;
   Int4  len;
   struct am_rowinf PNTR next;
} AMRowInfo, PNTR AMRowInfoPtr; 

typedef struct am_condenserow {
   SeqIdPtr  sip;
   Uint1     strand;
   Int4      rownum;
} AMCdRow, PNTR AMCdRowPtr;

typedef struct am_interval {
   Int4   from;
   Int4   to;
   Uint1  strand;
   struct am_interval PNTR next;
} AMInterval, PNTR AMIntervalPtr;

typedef struct am_intervalset {
   SeqIdPtr       sip;
   AMIntervalPtr  int_head;
   Uint1          strand;
   struct am_intervalset PNTR next;
} AMIntervalSet, PNTR AMIntervalSetPtr;

typedef struct am_edge {
   Int4         vertex1;
   Int4         vertex2;
   Int4         weight;
   Int4         used;
   Boolean      aligned;
   SeqAlignPtr  sap;
   struct am_edge PNTR next;
} AMEdge, PNTR AMEdgePtr;

typedef struct am_vertex {
   SeqIdPtr  sip;
   Int4      from;
   Int4      to;
   Uint1     strand;
   Int4      numedges;
   Boolean   used;
   Boolean   visited;
   struct am_vertex PNTR next;
} AMVertex, PNTR AMVertexPtr;

typedef struct am_queue {
   AMVertexPtr  vertex;
   struct am_queue PNTR next;
} AMQueue, PNTR AMQueuePtr;

typedef struct am_segment {
   Int4         len;
   Int4         which_row;
   SeqAlignPtr  sap;
   Int4         aligncoord;
   struct am_segment PNTR next;
} AMSegment, PNTR AMSegmentPtr;

typedef struct am_rowstart {
   AMSegmentPtr  segment;
   SeqIdPtr      sip;
   struct am_rowstart PNTR next;
} AMRowStart, PNTR AMRowStartPtr;

typedef struct am_frequency {
   Int4Ptr  PNTR freq;
   Int4     len;  /* second dimension */
   Int4     size; /* first dimension */
   Boolean  isna;
} AMFreq, PNTR AMFreqPtr;

typedef struct am_bit {
   Int4  n;
   Int4  num1;
   Int4  num2;
   Int4  num3;
   struct am_bit PNTR next;
} AMBitty2, PNTR AMBitty2Ptr;

/***************************************************************************
*
*  SECTION 1: Functions for allocating and freeing data structures used
*  by the alignment manager; copying functions are also here.
*
***************************************************************************/
NLM_EXTERN void AMFreqFree(AMFreqPtr afp);
NLM_EXTERN void AMAlignIndexFreeEitherIndex(SeqAlignPtr sap);
NLM_EXTERN SeqAlignPtr AlnMgr2DupAlnAndIndexes(SeqAlignPtr sap);

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
/* SECTION 2a */
/***************************************************************************
*
*  AlnMgr2IndexSingleChildSeqAlign takes a simple dense-seg or dense-diag
*  seqalign, converts it to dense-seg, and then calls
*  AlnMgr2IndexSingleDenseSegSA to create the indexes. If the alignment has
*  already been indexed, this erases that index and reindexes the alignment.
*
***************************************************************************/
NLM_EXTERN Boolean AlnMgr2IndexSingleChildSeqAlign(SeqAlignPtr sap);

/* SECTION 2c */
/***************************************************************************
*
*  AlnMgr2IndexLite takes a seqalign or a list of seqaligns, converts
*  each alignment to a dense-seg structure and indexes it, and then
*  allocates an AMAlignIndex2 structure and fills in the saps array.
*
***************************************************************************/
NLM_EXTERN Boolean AlnMgr2IndexLite(SeqAlignPtr sap);

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
NLM_EXTERN void AlnMgr2IndexSeqAlign(SeqAlignPtr sap);
NLM_EXTERN void AlnMgr2IndexSeqAlignEx(SeqAlignPtr sap, Boolean replace_gi);

/***************************************************************************
*
*  AlnMgr2ReIndexSeqAlign takes an indexed alignment (that has, presumably,
*  been changed), makes sure all child seqaligns are indexed (if they are
*  already indexed they are not reindexed), and reindexes all the child
*  seqaligns as a set.
*
***************************************************************************/
NLM_EXTERN void AlnMgr2ReIndexSeqAlign(SeqAlignPtr sap);

NLM_EXTERN Boolean AlnMgr2IndexAsRows(SeqAlignPtr sap, Uint1 strand, Boolean truncate);

/***************************************************************************
*
*  AlnMgr2IndexIndexedChain takes a linked list of indexed seqaligns
*  and does an in-place transformation to an indexed parent-child
*  seqalign set.
*
***************************************************************************/
NLM_EXTERN void AlnMgr2IndexIndexedChain(SeqAlignPtr sap);

/***************************************************************************
*
*  SECTION 3: Functions for debugging
*
***************************************************************************/
NLM_EXTERN void am_print_sa_index(SeqAlignPtr sap, FILE *ofp);
NLM_EXTERN void AlnMgr2PrintSeqAlign(SeqAlignPtr sap, Int4 linesize, Boolean isnuc, FILE * ofp);

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
NLM_EXTERN Boolean AlnMgr2GetNextAlnBit(SeqAlignPtr sap, AlnMsg2Ptr amp);

/* SECTION 4a */
/***************************************************************************
*
*  AlnMgr2GetInterruptInfo returns a structure describing the inserts and
*  unaligned regions in an interrupt. The structure is allocated by this
*  function and must be freed with AlnMgr2FreeInterruptInfo.
*
***************************************************************************/
NLM_EXTERN AMInterrInfoPtr AlnMgr2GetInterruptInfo(SeqAlignPtr sap, AMInterruptPtr interrupt);

/* SECTION 4b */
/***************************************************************************
*
*  AlnMgr2GetNthStrand takes an indexed seqalign and a row number and 
*  returns the strand of the row indicated. A return of 0 indicates
*  an error.
*
***************************************************************************/
NLM_EXTERN Uint1 AlnMgr2GetNthStrand(SeqAlignPtr sap, Int4 n);

/***************************************************************************
*
*  AlnMgr2GetNthSeqIdPtr returns the seqid (this is a duplicated,
*  allocated seqid that must be freed) of the nth row (1-based) of an
*  indexed parent or child seqalign.
*
***************************************************************************/
NLM_EXTERN SeqIdPtr AlnMgr2GetNthSeqIdPtr(SeqAlignPtr sap, Int4 n);

/***************************************************************************
*
*  AlnMgr2GetNthSeqRangeInSA returns the smallest and largest sequence
*  coordinates contained in the nth row of an indexed seqalign. Either
*  start or stop can be NULL to only retrieve one of the coordinates.
*  If start and stop are -1, there is an error; if they are both -2, the
*  row is just one big insert. RANGE
*
***************************************************************************/
NLM_EXTERN void AlnMgr2GetNthSeqRangeInSA(SeqAlignPtr sap, Int4 n, Int4Ptr start, Int4Ptr stop);

/***************************************************************************
*
*  AlnMgr2GetNthRowSpanInSA returns the least and greatest alignment
*  coordinates (inclusive) spanned by the indicated row. Either stop or
*  start can be NULL to retrieve just one of the coordinates.
*
***************************************************************************/
NLM_EXTERN void AlnMgr2GetNthRowSpanInSA(SeqAlignPtr sap, Int4 n, Int4Ptr start, Int4Ptr stop);

NLM_EXTERN Int4 AlnMgr2GetMaxTailLength(SeqAlignPtr sap, Uint1 which_tail);

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
NLM_EXTERN Boolean AlnMgr2GetNthRowTail(SeqAlignPtr sap, Int4 n, Uint1 which_tail, Int4Ptr start, Int4Ptr stop, Uint1Ptr strand);

/* SECTION 4c */
/***************************************************************************
*
*  AlnMgr2GetAlnLength returns the total alignment length of an indexed
*  alignment. If fill_in is TRUE, the function computes the total length
*  of all the internal unaligned regions and adds that to the alignment
*  length; otherwise only the aligned portions are considered.
*
***************************************************************************/
NLM_EXTERN Int4 AlnMgr2GetAlnLength(SeqAlignPtr sap, Boolean fill_in);

/* SECTION 4c functions for DDV */
NLM_EXTERN Boolean AlnMgr2IsSAPDiscAli(SeqAlignPtr sap);
NLM_EXTERN Int4 AlnMgr2GetNumAlnBlocks(SeqAlignPtr sap);
NLM_EXTERN Boolean AlnMgr2GetNthBlockRange(SeqAlignPtr sap, Int4 n, Int4Ptr start, Int4Ptr stop);

/***************************************************************************
*
*  AlnMgr2GetNthUnalignedForNthRow returns the bioseq coordinates for the
*  requested row, in the requested unaligned region. Any error will result
*  in -1 returns for both start and stop.
*
***************************************************************************/
NLM_EXTERN Boolean AlnMgr2GetNthUnalignedForNthRow(SeqAlignPtr sap, Int4 unaligned, Int4 row, Int4Ptr start, Int4Ptr stop);

/***************************************************************************
*
*  AlnMgr2GetNextLengthBit is called in a loop on an indexed alignment, with
*  seg starting at 0, to return the lengths of the aligned and unaligned
*  regions. If the length returned is negative, it's an unaligned region;
*  otherwise it's aligned.
*
***************************************************************************/
NLM_EXTERN Boolean AlnMgr2GetNextLengthBit(SeqAlignPtr sap, Int4Ptr len, Int4Ptr seg);

/***************************************************************************
*
*  AlnMgr2GetNumRows returns the number of rows in an indexed seqalign.
*
***************************************************************************/
NLM_EXTERN Int4 AlnMgr2GetNumRows(SeqAlignPtr sap);

/***************************************************************************
*
*  AlnMgr2GetNumSegs returns the number of gap- or aligned- contiguous
*  segments in the alignment (continuous or not).
*
***************************************************************************/
NLM_EXTERN Int4 AlnMgr2GetNumSegs(SeqAlignPtr sap);

/***************************************************************************
*
*  AlnMgr2GetNumSegsInRange returns the number of alignment segments
*  spanned by the given range (partially or fully). The range is
*  given in alignment coordinates.
*
***************************************************************************/
NLM_EXTERN Int4 AlnMgr2GetNumSegsInRange(SeqAlignPtr sap, Int4 start, Int4 stop, Int4Ptr start_seg);

/***************************************************************************
*
*  AlnMgr2GetNthSegmentRange returns the alignment coordinate range of the
*  Nth segment (count starts at 1) of the seqalign. start and stop are
*  optional arguments (in case only one end is desired).
*
***************************************************************************/
NLM_EXTERN void AlnMgr2GetNthSegmentRange(SeqAlignPtr sap, Int4 n, Int4Ptr start, Int4Ptr stop);

/***************************************************************************
*
*  AlnMgr2GetFirstNForSip returns the first row that a seqid occurs on,
*  or -1 if the seqid is not in the alignment or if there is another
*  error.
*
***************************************************************************/
NLM_EXTERN Int4 AlnMgr2GetFirstNForSip(SeqAlignPtr sap, SeqIdPtr sip);

/***************************************************************************
*
*  AlnMgr2GetFirstNForSipList returns the first row that one of a list of seqids occur on,
*  or -1 if none of the seqids are in the alignment or if there is another
*  error. 
*  Handy if sip comes from a BioSeq, where it can point to a linked list
*  of SeqIds.
*
***************************************************************************/
NLM_EXTERN Int4 AlnMgr2GetFirstNForSipList(SeqAlignPtr sap, SeqIdPtr sip);

/***************************************************************************
*
*  AlnMgr2GetParent returns the top-level seqalign associated with a given
*  indexed alignment. It returns the actual pointer, not a copy.
*
***************************************************************************/
NLM_EXTERN SeqAlignPtr AlnMgr2GetParent(SeqAlignPtr sap);

/***************************************************************************
*
*  SECTION 5: Functions to change, assign or retrieve an anchor row.
*    SECTION 5a: functions for child seqaligns
*    SECTION 5b: functions for parent seqaligns
*    SECTION 5c: functions to retrieve anchor row information
*
***************************************************************************/
/* SECTION 5b */

/***************************************************************************
*
*  AlnMgr2AnchorSeqAlign takes an indexed seqalign and a row (1-based) and
*  reindexes the alignment so that there are no gaps in the row indicated.
*  Other rows may contain inserts after this operation. After an alignment
*  is anchored, its length often shrinks.
*
***************************************************************************/
NLM_EXTERN void AlnMgr2AnchorSeqAlign(SeqAlignPtr sap, Int4 which_row);

/* SECTION 5c */
/***************************************************************************
*
*  AlnMgr2FindAnchor returns the row number (1-based) of the anchor row
*  for an indexed seqalign, or -1 if the alignment is unanchored or if
*  there is another type of error.
*
***************************************************************************/
NLM_EXTERN Int4 AlnMgr2FindAnchor(SeqAlignPtr sap);

/***************************************************************************
*
*  SECTION 6: Functions for coordinate conversion (bioseq to seqalign
*  coordinates and vice versa)
*
***************************************************************************/

/***************************************************************************
*
*  AlnMgr2MapBioseqToSeqAlign takes an indexed seqalign, a position in a
*  row of the alignment, and a 1-based row number, and maps the row position
*  to alignment coordinates.
*
***************************************************************************/
NLM_EXTERN Int4 AlnMgr2MapBioseqToSeqAlign(SeqAlignPtr sap, Int4 pos, Int4 row);

/***************************************************************************
*
*  AlnMgr2MapSeqAlignToBioseq takes an indexed seqalign, an alignment
*  coordinate (pos), and the 1-based number of a row, and maps the alignment
*  coordinate to the corresponding bioseq coordinate of the row desired.
*  A return of -1 indicates an error; a return of -2 means that the bioseq
*  is gapped at this alignment position.
*
***************************************************************************/
NLM_EXTERN Int4 AlnMgr2MapSeqAlignToBioseq(SeqAlignPtr sap, Int4 pos, Int4 row);

/***************************************************************************
*
*  AlnMgr2MapRowToRow takes an indexed seqalign, a position in row1, the
*  1-based number of row1, and a target row (row2), and maps the bioseq
*  coordinate in row 1 to the corresponding (aligned) bioseq coordinate in
*  row2. A return of -1 indicates an error while a return of -2 means that
*  the bioseq in row2 is gapped at the desired position.
*
***************************************************************************/
NLM_EXTERN Int4 AlnMgr2MapRowToRow(SeqAlignPtr sap, Int4 pos, Int4 row1, Int4 row2);

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
NLM_EXTERN Boolean AlnMgr2TruncateSeqAlign(SeqAlignPtr sap, Int4 start, Int4 stop, Int4 row);

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
NLM_EXTERN SeqAlignPtr AlnMgr2GetSubAlign(SeqAlignPtr sap, Int4 from, Int4 to, Int4 which_row, Boolean fill_in);

/***************************************************************************
*
*  SECTION 8: Miscellaneous functions to compute useful information
*    about an alignment
*
***************************************************************************/
/***************************************************************************
*
*  AlnMgr2ComputeScoreForSeqAlign computes an ad hoc numerical score for
*  an indexed alignment by computing a similarity score for the whole
*  alignment (residue pair by residue pair score, from a matrix for proteins
*  and identity for nucleotides) and then subtracting gap open and gap
*  extension penalties.
*
***************************************************************************/
NLM_EXTERN Int4 AlnMgr2ComputeScoreForSeqAlign(SeqAlignPtr sap);

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
NLM_EXTERN AMFreqPtr AlnMgr2ComputeFreqMatrix(SeqAlignPtr sap, Int4 from, Int4 to, Int4 row);

/***************************************************************************
*
*  AlnMgr2IsItProtein takes an indexed alignment and quickly decides if
*  it's a protein or nucleotide alignment, returning TRUE for protein.
*
***************************************************************************/
NLM_EXTERN Boolean AlnMgr2IsItProtein(SeqAlignPtr sap);

/***************************************************************************
*
*  SECTION 9: Sorting functions and other algorithms to help order
*  alignments for various purposes
*
***************************************************************************/
/***************************************************************************
*
*  AlnMgr2SortAlnSetByNthRowPos takes an indexed parent alignment and sorts
*  all the child alignments along the row indicated. If the indicated row
*  is aligned on the plus strand, the alignments are sorted from smaller
*  to larger coordinates along that row; otherwise they are sorted in
*  reverse order.
*
***************************************************************************/
NLM_EXTERN void AlnMgr2SortAlnSetByNthRowPos(SeqAlignPtr sap, Int4 row);

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
NLM_EXTERN SeqAlignPtr AlnMgr2MergeTwoAlignments(SeqAlignPtr sap1_orig, SeqAlignPtr sap2_orig);

/***************************************************************************
*
*  AlnMgr2ExtendToCoords takes an indexed child seqalign and blindly extends
*  it to the coordinates specified on the given row. If other rows are too
*  short to allow this extension, the alignment is extended as far as
*  possible. If to == -1 the extension goes to the end of the sequence
*  specified.
*
***************************************************************************/
NLM_EXTERN void AlnMgr2ExtendToCoords(SeqAlignPtr sap, Int4 from, Int4 to, Int4 row);

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
NLM_EXTERN SeqAlignPtr AlnMgr2PadConservatively(SeqAlignPtr sap);

/***************************************************************************
*
*  AlnMgr2ExtractPairwiseSeqAlign takes an indexed alignment (parent or
*  child, but must be fully indexed, not lite) and extracts a pairwise
*  subalignment containing the two requested rows. The subalignment is
*  unindexed and may have internal unaligned regions.
*
***************************************************************************/
NLM_EXTERN SeqAlignPtr AlnMgr2ExtractPairwiseSeqAlign(SeqAlignPtr sap, Int4 n1, Int4 n2);

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
NLM_EXTERN void AlnMgr2RemoveInconsistentAlnsFromSet(SeqAlignPtr sap_head, Int4 fuzz);

/***************************************************************************
*
*  AlnMgr2FuseSet takes a set of alignments sharing all their rows and orders
*  the alignments, then fuses together any adjacent alignments. If returnall
*  is TRUE, all pieces are returned; if not, then only the largest piece is
*  returned. This function will work best when called after
*  AlnMgr2RemoveInconsistentAlnsFromSet(sap_head, -1).
*
***************************************************************************/
NLM_EXTERN SeqAlignPtr AlnMgr2FuseSet(SeqAlignPtr sap_head, Boolean returnall);

/* SECTION 11 -- functions for std-segs */
NLM_EXTERN Int4 AlignMgr2GetFirstNForStdSeg(SeqAlignPtr sap, SeqIdPtr sip);
NLM_EXTERN SeqIdPtr AlnMgr2GetNthSeqIdPtrStdSeg(SeqAlignPtr sap, Int4 n);
NLM_EXTERN void AlnMgr2GetNthSeqRangeInSAStdSeg(SeqAlignPtr sap, Int4 n, Int4Ptr start, Int4Ptr stop);

/***************************************************************************
*
*   AlnMgr2GetSeqRangeForSipInSAStdSeg  returns the smallest and largest sequence
*  coordinates in in a Std-Seg seqalign for a given Sequence Id.  Also return the 
*  strand type if it is the same on every segment, else set it to Seq_strand_unknown.
*  Either start, stop or strand can be NULL to only retrieve some of them.
*  If start and stop are -1, there is an error (not a std-seg), the SeqID does not participate in this
*  alignment or the alignment is one big insert on that id.  Returns true if the sip was found
*  in the alignment with real coordinates, i.e. *start would not be -1.  RANGE
*
***************************************************************************/
NLM_EXTERN Boolean AlnMgr2GetSeqRangeForSipInSAStdSeg(SeqAlignPtr sap, SeqIdPtr sip, Int4Ptr start, Int4Ptr stop, Uint1Ptr strand);

/***************************************************************************
*
*   AlnMgr2GetSeqRangeForSipInStdSeg  returns the start and stop sequence
*  coordinates in a Std-Segment for a given Sequence Id.  Also return the 
*  strand type.  Either start, stop, strand or segType can be NULL to only retrieve some of them.
*  Returns false if the SeqID was not found in this segment, so no meaningful 
*    data was passed back in other arguments.  
*  Returns true if the sip was found, even if it is a gap (start, stop = -1).
*  segType is set to AM_SEQ if the SeqID Sequence is not empty and one of 
*  the other sequences aligned with it is also not empty.  To AM_GAP if
*  the other sequences are all empty, and to AM_INSERT if the main sequence
*  is empty. 
*  RANGE
*
***************************************************************************/
NLM_EXTERN Boolean AlnMgr2GetSeqRangeForSipInStdSeg(
    StdSegPtr   ssp, 
    SeqIdPtr    sip, 
    Int4Ptr     start, 
    Int4Ptr     stop, 
    Uint1Ptr    strand,
    Uint1Ptr    segType); /* AM_SEQ, AM_GAP, AM_INSERT */

/***************************************************************************
*
*   AlnMgr2GetNthStdSeg  returns the a pointer to the Nth segment of
*   a standard segment alignment.
*   returns NULL if not n segments or is not a std-seg aligment.
*   Useful to pass its return value to AlnMgr2GetSeqRangeForSipInStdSeg()
*
***************************************************************************/
NLM_EXTERN StdSegPtr AlnMgr2GetNthStdSeg(SeqAlignPtr sap, Int2 n);

/***************************************************************************
*
*  AlnMgr2GetNumStdSegs returns the number of segments in a standar-seg alignment.
*   returns -1 if sap is null or not a standard-seg alignment.
*   the Std-Seg version of AlnMgr2GetNumSegs
*
***************************************************************************/
NLM_EXTERN Int4 AlnMgr2GetNumStdSegs(SeqAlignPtr sap);

/***************************************************************************
*
*  The two mapping functions act a little differently for std-segs. The
*  alignment coordinates are 1:1 linearly correlated with the longest
*  seqloc in the set; the others may be significantly shorter.
*  The mapping functions deal with % lengths, and map those instead of
*  coordinates (which may not be linear);
*
***************************************************************************/
NLM_EXTERN Int4 AlnMgr2MapBioseqToSeqAlignStdSeg(SeqAlignPtr sap, Int4 n, Int4 pos);
NLM_EXTERN Int4 AlnMgr2MapSeqAlignToBioseqStdSeg(SeqAlignPtr sap, Int4 n, Int4 pos);
NLM_EXTERN Int4 AlnMgr2GetAlnLengthStdSeg(SeqAlignPtr sap);

/***************************************************************************/


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
