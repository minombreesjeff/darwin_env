/*  edutil.h
* ===========================================================================
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
* File Name:  edutil.h
*
* Author:  James Ostell
*   
* Version Creation Date: 2/2/94
*
* $Revision: 6.15 $
*
* File Description:  Sequence editing utilities
*
* Modifications:  
* --------------------------------------------------------------------------
* Date       Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
* $Log: edutil.h,v $
* Revision 6.15  2005/04/28 20:10:32  bollin
* added new function AdjustFeaturesForInsertion which is called by BioseqInsert
* and also by a new function in sequin3.c for converting a raw bioseq to a delta
* and inserting gaps
*
* Revision 6.14  2005/04/06 19:33:15  bollin
* made it possible to insert and remove gaps from delta sequences
*
* Revision 6.13  2004/10/08 16:04:16  bollin
* added ability to check when an action will remove a feature
*
* Revision 6.12  2004/09/29 18:49:57  bollin
* fixed bugs in sequence editing, can now undo a nucleotide deletion that
* removes an entire feature location (feature will be restored)
*
* Revision 6.11  2004/09/23 14:59:51  bollin
* moved functions that depend on functions that depend on BLAST functions
* into seqpanel.c, made function scalled by those functions extern
*
* Revision 6.10  2004/09/22 18:20:32  bollin
* added functions for playing and unplaying a sequence editor action to translate
* a CDS
*
* Revision 6.9  2004/07/30 18:46:55  bollin
* added function for reordering intervals after they have been dragged by
* the sequence editor
*
* Revision 6.8  2004/07/28 20:06:19  bollin
* added journaling for undo/redo of dragged sequence location changes
*
* Revision 6.7  2004/07/28 15:22:15  bollin
* moved functions for moving feature locations around to edutil.c from
* seqpanel.c
*
* Revision 6.6  2004/07/22 15:34:41  bazhin
* Added function prototype GapToSeqLocEx() to handle gaps of unknown
* lengths within locations.
*
* Revision 6.5  2004/07/12 12:29:45  bollin
* moved new sequence editor editing functions here
*
* Revision 6.4  2003/02/10 22:57:45  kans
* added BioseqCopyEx, which takes a BioseqPtr instead of a SeqIdPtr for the source
*
* Revision 6.3  2002/07/02 13:23:43  kans
* added SeqLocDeleteEx
*
* Revision 6.2  1997/11/10 19:50:23  kans
* Fixed incorrect comment for ISAGappedSeqLoc() function (SB).
*
* Revision 6.1  1997/10/24 19:15:01  bazhin
* Added descriptors for three easy functions GapToSeqLoc(...),
* ISAGappedSeqLoc(...) and GappedSeqLocsToDeltaSeqs(...) for
* processing "gap(...)" tokens in CONTIG line.
*
* Revision 6.0  1997/08/25 18:05:28  madden
* Revision changed to 6.0
*
* Revision 5.6  1997/07/25 20:34:56  kans
* added SegLocToPartsEx
*
* Revision 5.5  1997/06/19 18:37:34  vakatov
* [WIN32,MSVC++]  Adopted for the "NCBIOBJ.LIB" DLL'ization
*
* Revision 5.4  1996/10/09 17:31:37  kans
* was missing an ifdef cplusplus at the bottom
*
 * Revision 5.3  1996/10/09  16:34:59  chappey
 * added SeqLocReplaceID() that replaces the Seq-Id of a Seq-Loc
 *
 * Revision 5.2  1996/06/12  18:29:34  epstein
 * move SeqLocIntNew() and SeqLocPntNew() from edutil to sequtil
 *
 * Revision 5.1  1996/06/10  15:07:22  epstein
 * replace make_seq_loc() with SeqLocIntNew() and make_pnt_loc with SeqLocPntNew()
 *
 * Revision 5.0  1996/05/28  13:23:23  ostell
 * Set to revision 5.0
 *
 * Revision 4.7  1996/03/12  22:14:22  ostell
 * added SeqLocToParts()
 *
 * Revision 4.5  1996/01/30  16:24:04  ostell
 * changed name of SeqLocPack() to SeqLocPackage()
 *
 * Revision 4.4  1996/01/29  22:03:52  ostell
 * revised SeqLocAdd
 * added SeqLocPack
 *
 * Revision 4.3  1996/01/10  22:25:25  ostell
 * added SeqLocIntNew()
 *
 * Revision 4.2  1995/12/29  21:31:44  ostell
 * added mapping functions between delta seq and seq loc, for editing utilities
 *
 * Revision 4.1  1995/11/15  20:40:20  ostell
 * fixed SeqLocCopyPart so it correctly handles SEQLOC_NULL in segmented
 * records
 *
 * Revision 4.0  1995/07/26  13:49:01  ostell
 * force revision to 4.0
 *
 * Revision 1.6  1995/05/15  21:46:05  ostell
 * added Log line
 *
*
*
*
* ==========================================================================
*/
#ifndef _NCBI_EdUtil_
#define _NCBI_EdUtil_

#ifndef _NCBI_SeqPort_
#include <seqport.h>           /* other utilities */
#endif

#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include "explore.h"

/*****************************************************************************
*
*   Sequence Editing Utilties
*       High Level Cut, Copy, Paste
*
*****************************************************************************/


/*****************************************************************************
*
*   BioseqDelete (target, from, to, do_feat, do_split)
*      Deletes the region of sequence between from-to, inclusive, on the
*        Bioseq whose SeqId is target.
*      If do_feat, the feature table is updated to reflect the deletion
*        using SeqEntryDelFeat()
*      If do_split, the features across the deleted region are split into
*        two intervals on either side. If not, the feature is just shortened.
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL BioseqDelete (SeqIdPtr target, Int4 from, Int4 to, Boolean do_feat, Boolean do_split);



/*****************************************************************************
*
*   BioseqCopy(newid, sourceid, from, to, strand, do_feat)
*      Creates a new Bioseq from sourceid in the range from-to inclusive.
*      If strand==Seq_strand_minus, reverse complements the sequence in
*        the copy and (if do_feat) corrects the feature table appropriately.
*      Names new Bioseq as newid, if not NULL
*        else Creates seqid.local = "Clipboard" if newid is NULL
*      If do_feat == TRUE copies appropriate region of feature table from
*        sourceid to new copy using SeqFeatsCopy().
*
*
*   BioseqCopyEx(newid, oldbsp, from, to, strand, do_feat)
*      Internal function called by BioseqCopy that takes BioseqPtr
*
*****************************************************************************/
NLM_EXTERN BioseqPtr LIBCALL BioseqCopy (SeqIdPtr newid, SeqIdPtr sourceid, Int4 from, Int4 to,
                               Uint1 strand, Boolean do_feat);

NLM_EXTERN BioseqPtr LIBCALL BioseqCopyEx (SeqIdPtr newid, BioseqPtr oldbsp, Int4 from, Int4 to,
                               Uint1 strand, Boolean do_feat);

/*****************************************************************************
*
*	SeqLocCopyPart (the_segs, from, to, strand, group, first_segp, last_segp)
*      cuts out from the_segs the part from offset from to offset to
*      reverse complements resulting seqloc if strand == Seq_strand_minus
*      if (group) puts resulting intervals into a new Seq-loc (of type
*        PACKED_INT if no SEQLOC_NULL, else SEQLOC_MIX).
*      Currently this always makes intervals or nulls. Is really for segmented and
*        reference sequence extensions
*      If first_segp and last_segp are not NULL, then they are filled in with the
*        ordinal number of the source segments that remain in the copy, based
*        on SeqLocFindNext, where 1 is the first one. Thus if the third and
*        fourth segments were copied, first is 3 and last is 4. If the
*        location was reverse complemented, first is 4 and last is 3.
*
*****************************************************************************/
NLM_EXTERN SeqLocPtr LIBCALL SeqLocCopyPart PROTO((SeqLocPtr the_segs, Int4 from, Int4 to,
         Uint1 strand, Boolean group, Int2Ptr first_segp, Int2Ptr last_segp));

/* This function is used by BioseqInsert to adjust features on a sequence after
 * an insertion.  pos indicates the start of the insertion, len indicates
 * the length of the inserted sequence, and do_split indicates whether features
 * that cover the insertion should be split at either side of the insertion.
 */
extern void 
AdjustFeaturesForInsertion 
(BioseqPtr tobsp, 
 SeqIdPtr  to_id,
 Int4 pos, 
 Int4 len, 
 Boolean do_split);

/*****************************************************************************
*
* BioseqInsert (from_id, from, to, strand, to_id, pos, from_feat, to_feat,
*                                                                     do_split)
*       Inserts a copy the region "from"-"to" on "strand" of the Bioseq
*          identified by "from_id" into the Bioseq identified by "to_id"
*          before "pos".
*          To append to the end of the sequence, use APPEND_RESIDUE for "pos".
*       if from_feat = TRUE, copies the feature table from "from" and updates
*          to locations to point to the proper residues in "to_id"
*       If to_feat = TRUE, updates feature table on "to_id" as well.
*          if do_split == TRUE, then splits features in "to_id" (to_feat must
*             be TRUE as well). Otherwise expands features at insertion.
*
*       All operations are copies. "frombsp" is unchanged.
*       Insert will only occur between certain Bioseq.repr classes as below
*
*   From Bioseq.repr                      To Bioseq.repr
*   
*                          virtual       raw      segmented        map
*                       +---------------------------------------------------
*             virtual   |   length       inst      SeqLoc         length
*                       +---------------------------------------------------
*                 raw   |   error        copy      SeqLoc         error
*                       +---------------------------------------------------
*           segmented   |   error        inst      SeqLoc*        error
*                       +---------------------------------------------------
*                 map   |   error        inst*     SeqLoc         copy
*                       +---------------------------------------------------
*
*   length = changes length of "to" by length of "from"
*   error  = insertion not allowed
*   inst   = "from" instantiated as residues ("N" or "X" for virtual "from")
*   inst*  = as above, but a restriction map can instantiate other bases
*            than "N" for known restriction recognition sites.
*   copy   = copy of "from" inserted into "to"
*   SeqLoc = a SeqLoc added to "to" which points to "from". No copy of residues.
*   SeqLoc* = as above, but note that "to" points to "from" directly, not
*             what "from" itself may point to.
*   
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL BioseqInsert (SeqIdPtr from_id, Int4 from, Int4 to, Uint1 strand, SeqIdPtr to_id, Int4 pos,
            Boolean from_feat, Boolean to_feat, Boolean do_split);


/*****************************************************************************
*
*   BioseqOverwrite (target, pos, residue, seqcode)
*      Overwrites the residue at pos with residue in the
*        Bioseq whose SeqId is target.
*      residue is iupacna for DNA or ncbieaa for protein
*      target MUST be a raw Bioseq right now
*      no changes are made to the feature table
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL BioseqOverwrite (SeqIdPtr target, Int4 pos, Uint1 residue);

/*****************************************************************************
*
*       Some defined values are provided for the Int4 values passed as
*           from, to, and pos in functions above. Defined in seqport.h
*   
*       FIRST_RESIDUE   0	(first residue of sequence)
*       LAST_RESIDUE    -1  (last residue of sequence.. interpreted as
*                              length - 1)
*       APPEND_RESIDUE  -2  (interpreted as length.. off the end of the
*                              sequence. Only for "pos" in BioseqInsert
*                              to append to the end of the sequence )
*   
*
*****************************************************************************/





/*****************************************************************************
*
*   Sequence Editing Utilties
*       Lower Level Support Routines
*
*****************************************************************************/


/*****************************************************************************
*
*   SeqFeatDelete()
*     0 = no changes made to location or product
*     1 = changes made but feature still has some location
*     2 = all of sfp->location in deleted interval
*
*   if (merge)
*      1) correct numbers > to by subtraction
*      2) do not split intervals spanning the deletion
*   else
*      1) do not change numbers > to
*      2) split intervals which span the deletions
*
*****************************************************************************/
NLM_EXTERN Int2 LIBCALL SeqFeatDelete (SeqFeatPtr sfp, SeqIdPtr target, Int4 from, Int4 to, Boolean merge);

NLM_EXTERN Boolean LIBCALL SeqInsertByLoc (SeqIdPtr target, Int4 offset, SeqLocPtr fragment);

/*****************************************************************************
*
*   SeqDeleteByLoc (slp, do_feat, do_split)
*   	deletes regions referenced by slp
*       if do_feat, deletes features in those regions as well
*       if do_split, splits intervals crossing the deletion
*         else just shortens them.
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL SeqDeleteByLoc (SeqLocPtr slp, Boolean do_feat, Boolean do_split);


/*****************************************************************************
*
*   SeqLocAdd(headptr, slp, merge, do_copy)
*   	creates a linked list of SeqLocs.
*       returns a pointer to the last SeqLoc in the chain
*       if (merge)
*   	  deletes double NULLs or Nulls at start (application must delete at stop)
*         merges adjacent intervals on the same strand
*       if (do_copy)
*   	  Makes copies of incoming SeqLocs
*         if incoming is merged, deletes the incoming SeqLoc
*
*       call SeqLocPack(head) to turn into a SeqLoc from a linked list
*
*****************************************************************************/
NLM_EXTERN SeqLocPtr LIBCALL SeqLocAdd (SeqLocPtr PNTR head, SeqLocPtr slp, Boolean merge, Boolean do_copy);

/*****************************************************************************
*
*   SeqLocPackage(head)
*     head is a chain of 1 or more SeqLocs connected by slp->next
*     Assumes was built by SeqLocAdd to remove redundancy
*     Frees the last element if it is a NULL.
*     If more than one element left, then packages the chain into a SEQLOC_MIX,
*       or SEQLOC_PACKED_INT as appropriate
*     returns pointer to the head of the resulting single SeqLoc
*
*****************************************************************************/
NLM_EXTERN SeqLocPtr LIBCALL SeqLocPackage (SeqLocPtr head);

/*****************************************************************************
*
*   SegLocToPartsEx(BioseqPtr seg, SeqLocPtr slp, Boolean nullsBetween)
*   SegLocToParts(BioseqPtr seg, SeqLocPtr slp)
*   	seg must be a segmented Bioseq
*       slp must be a SeqLoc on it
*       nullsBetween makes order instead of join
*       function maps slp to the components of seg
*       returns a new SeqLocPtr
*       does not delete slp
*
*****************************************************************************/
NLM_EXTERN SeqLocPtr LIBCALL SegLocToPartsEx (BioseqPtr seg, SeqLocPtr slp, Boolean nullsBetween);

NLM_EXTERN SeqLocPtr LIBCALL SegLocToParts (BioseqPtr seg, SeqLocPtr slp);



/*****************************************************************************
*
*   SeqLocDelete()
*       returns altered head or NULL if nothing left.
*   sets changed=TRUE if all or part of loc is deleted
*   does NOT set changed if location coordinates are only moved
*   if (merge) then corrects coordinates upstream of to
*   else
*     splits intervals covering from-to, does not correct upstream of to
*
*****************************************************************************/
NLM_EXTERN SeqLocPtr LIBCALL SeqLocDelete (SeqLocPtr head, SeqIdPtr target, Int4 from, Int4 to, Boolean merge, BoolPtr changed);

NLM_EXTERN SeqLocPtr LIBCALL SeqLocDeleteEx (SeqLocPtr head, SeqIdPtr target, Int4 from, Int4 to, Boolean merge, BoolPtr changed, BoolPtr partial5, BoolPtr partial3);

NLM_EXTERN SeqLocPtr LIBCALL SeqLocSubtract (SeqLocPtr head, SeqLocPtr piece);


/*****************************************************************************
*
*   DeltaSeqsToSeqLocs(dsp)
*   	converts a chain of delta seqs to seqlocs
*   	each SeqLit is converted to SeqLoc of type Int with a SeqId of type
*          Dbtag where db="Seq\tLit" and objectId.id which is the index of the
*          element in the delta seq chain where 1 is the first one.
*   	Returned SeqLoc is of type "mix" and must be freed by caller.
*
*****************************************************************************/
NLM_EXTERN SeqLocPtr LIBCALL DeltaSeqsToSeqLocs (DeltaSeqPtr dsp);

/*****************************************************************************
*
*   SeqLocsToDeltaSeqs(dsp, slp)
*   	converts a chain of seqlocs	generated by DeltaSeqToSeqLocs() back into
*         delta seqs. dsp is the original chain of DeltaSeqs, which is required
*         to convert the delta seqs back.
*
*****************************************************************************/
NLM_EXTERN DeltaSeqPtr LIBCALL SeqLocsToDeltaSeqs (DeltaSeqPtr dsp, SeqLocPtr slp);

/*****************************************************************************
*
*   ISADeltaSeqsToSeqLoc(slp)
*   	returns Index (> 0) if this (one) SeqLoc was converted from a Delta Seq by
*         DeltaSeqsToSeqLocs() by looking for the special Dbtag name
*
*****************************************************************************/
NLM_EXTERN Int4 LIBCALL ISADeltaSeqsToSeqLoc (SeqLocPtr slp);


/*****************************************************************************
*
*   SeqEntryDelFeat(sep, id, from, to, do_split)
*   	Deletes or truncates features on Bioseq (id) in the range
*       from-to, inclusive
*       
*		Moves features > to left to account for decrease in length
*       if do_split, breaks intervals across the deletion
*       else just reduces their size
*
*       If sep == NULL, then calls SeqEntryFind(id) to set scope to look
*       for features.
*   
*****************************************************************************/
NLM_EXTERN Boolean	LIBCALL SeqEntryDelFeat PROTO((SeqEntryPtr sep, SeqIdPtr sip, Int4 from, Int4 to, Boolean do_split));

/*****************************************************************************
*
*   SeqFeatCopy(new, old, from, to, strand)
*
*****************************************************************************/
NLM_EXTERN Int2 LIBCALL SeqFeatsCopy (BioseqPtr newbsp, BioseqPtr oldbsp, Int4 from, Int4 to, Uint1 strand);

NLM_EXTERN SeqLocPtr LIBCALL SeqLocCopyRegion(SeqIdPtr newid, SeqLocPtr head, BioseqPtr oldbsp,
    Int4 from, Int4 to, Uint1 strand, BoolPtr split);

/*****************************************************************************
*
*   IntFuzzClip()
*       returns TRUE if clipped range values
*       in all cases, adjusts and/or complements IntFuzz
*       Designed for IntFuzz on SeqLocs
*
*****************************************************************************/
NLM_EXTERN void LIBCALL IntFuzzClip(IntFuzzPtr ifp, Int4 from, Int4 to, Uint1 strand, BoolPtr split);

/*****************************************************************************
*
*   SeqLocInsert()
*       alters "head" by inserting "len" residues before "pos" in any SeqLoc
*         on the Bioseq "target"
*       all SeqLocs not on "target" are unaltered
*       for SeqLocs on "target"
*          all SeqLocs before "pos" are unaltered
*          all SeqLocs >= "pos" are incremented by "len"
*          all SeqLocs spanning "pos"
*             if "split" == TRUE, are split into two SeqLocs, one to the
*               left of the insertion, the other to right
*             if "split" != TRUE, the SeqLoc is increased in length to cover
*               the insertion
*       returns altered head or NULL if nothing left.
*       if ("newid" != NULL) replaces "target" with "newid" whether the
*          SeqLoc is altered on not.
*
*       Usage hints:
*          1) To update a feature location on "target" when 10 residues of
*               sequence have been inserted before position 5
*          SeqFeatPtr->location = SeqLocInsert ( SeqFeatPtr->location ,
*                "target", 5, 10, TRUE, NULL);  [for some feature types
*                      you may want "split" equal FALSE]
*   
*          2) To insert the complete feature table from "source" into a
*                different Bioseq "dest" before position 20 in "dest"
*          SFP->location = SeqLocInsert(SFP->location, "source", 0, 20,
*                FALSE, "dest");
*   
*
*****************************************************************************/
NLM_EXTERN SeqLocPtr LIBCALL SeqLocInsert (SeqLocPtr head, SeqIdPtr target, Int4 pos, Int4 len,
                                               Boolean split, SeqIdPtr newid);


/********************************************************************
*
* SeqLocReplaceID
*   replaces the Seq-Id in a Seq-Loc (slp) with a new Seq-Id (new_sip)
*
**********************************************************************/
NLM_EXTERN SeqLocPtr SeqLocReplaceID (SeqLocPtr slp, SeqIdPtr new_sip);

/**********************************************************
 *
 *   NLM_EXTERN SeqLocPtr LIBCALL GapToSeqLoc(range):
 *
 *      Gets the size of gap and constructs SeqLoc block with
 *   $(seqlitdbtag) value as Dbtag.db and Dbtag.tag.id = 0.
 *
 **********************************************************/
NLM_EXTERN SeqLocPtr LIBCALL GapToSeqLoc(Int4 range);

/**********************************************************
 *
 *   NLM_EXTERN SeqLocPtr LIBCALL GapToSeqLocEx(range, unknown):
 *
 *      Gets the size of gap and constructs SeqLoc block with
 *   $(seqlitdbtag) value in case of gap of known length or
 *   $(unkseqlitdbtag) value if length is unknown as Dbtag.db
 *   and Dbtag.tag.id = 0.
 *
 **********************************************************/
NLM_EXTERN SeqLocPtr LIBCALL GapToSeqLocEx(Int4 range, Boolean unknown);

/**********************************************************
 *
 *   NLM_EXTERN Boolean LIBCALL ISAGappedSeqLoc(slp):
 *
 *      Looks at a single SeqLoc item. If it has the SeqId
 *   of type GENERAL with Dbtag.db == $(seqlitdbtag) and
 *   Dbtag.tag.id == 0, then returns TRUE, otherwise
 *   returns FALSE.
 *
 **********************************************************/
NLM_EXTERN Boolean LIBCALL ISAGappedSeqLoc(SeqLocPtr slp);

/**********************************************************
 *
 *   NLM_EXTERN DeltaSeqPtr LIBCALL GappedSeqLocsToDeltaSeqs(slp):
 *
 *      This functions is used only in the case, if ISAGappedSeqLoc()
 *   has returned TRUE.
 *      Converts SeqLoc set to the sequence of DeltaSeqs.
 *   Gbtag'ed SeqLocs it turns into SeqLits with the only "length"
 *   element. The regular SeqLocs saves as they are. Returns
 *   obtained DeltaSeq.
 *
 **********************************************************/
NLM_EXTERN DeltaSeqPtr LIBCALL GappedSeqLocsToDeltaSeqs(SeqLocPtr slp);

/* the following typedefs and functions are used by the new sequence editor 
 * in desktop/seqpanel.c
 */
typedef enum { eSeqEdInsert, eSeqEdDelete, eSeqEdFeatMove, eSeqEdTranslate,
               eSeqEdInsertGap, eSeqEdDeleteGap,
               eSeqEdJournalStart, eSeqEdJournalEnd } ESeqEdJournalAction ;

typedef struct seqedjournaldata
{
  ESeqEdJournalAction action;              /* indicates action taken */
  Int4                offset;              /* position to the left of deletion or right of insertion */
  Int4                num_chars;           /* number of characters inserted or deleted */
  CharPtr             char_data;           /* characters inserted or removed */
                                           /* when creating a journal entry for deletion, allocate
                                            * space for char_data but do not populate it - it will
                                            * be populated when the journal entry is played */
  Boolean             spliteditmode;       /* if insertion occurs and spliteditmode is true and
                                            * a feature overlaps the insertion position, the location
                                            * of the feature will be discontinuous at the point of
                                            * insertion. */
  SeqFeatPtr          sfp;                 /* A feature that was moved - should be NULL
                                            * unless action == eSeqEdFeatMove */
  SeqLocPtr           slp;                 /* A location for sfp - if the journal has
                                            * already been played, this is the previous
                                            * location, if the journal has been undone,
                                            * this is the location before the redo. */
  ValNodePtr          affected_feats;      /* This is a list of features which were shortened by
                                            * an eSeqEdDelete operation - their locations will
                                            * need to be reconstructed if the operation is undone. */
  Boolean             unknown_gap;         /* This is used only when action is eSeqEdInsertGap
                                            * or eSeqEdDeleteGap.  It indicates whether the gap
                                            * being inserted (or deleted) from a delta sequence
                                            * is unknown. */                                            
  BioseqPtr           bsp;                 /* The Bioseq for which the action is to be/was applied. */
  Uint2               moltype;             /* Molecule type for bsp.  Stored for convenience. */
  Uint2               entityID;            /* entityID for bsp.  Stored for convenience. */
  Pointer             next;                /* Journal entries are a doubly-linked list so that */
  Pointer             prev;                /* we can traverse the list in both directions for  */
                                           /* undo and redo. */
} SeqEdJournalData, PNTR SeqEdJournalPtr;

NLM_EXTERN SeqLocPtr LIBCALL 
SeqEdSeqLocInsert 
(SeqLocPtr head,
 BioseqPtr target, 
 Int4 pos,
 Int4 len,
 Boolean split,
 SeqIdPtr newid);

NLM_EXTERN void 
SeqEdInsertAdjustRNA 
(SeqFeatPtr sfp,
 BioseqPtr  bsp,
 Int4       insert_pos,
 Int4       len,
 Boolean    do_split);

NLM_EXTERN void 
SeqEdInsertAdjustCdRgn 
(SeqFeatPtr sfp,
 BioseqPtr  bsp,
 Int4       insert_pos,
 Int4       len,
 Boolean    do_split);
 
NLM_EXTERN SeqLocPtr 
SeqEdSeqLocDelete 
(SeqLocPtr head,
 BioseqPtr target,
 Int4      from,
 Int4      to,
 Boolean   merge,
 BoolPtr   changed,
 BoolPtr   partial5,
 BoolPtr   partial3);

NLM_EXTERN void SeqEdJournalFree (SeqEdJournalPtr sejp);
NLM_EXTERN SeqEdJournalPtr SeqEdJournalNewSeqEdit 
(ESeqEdJournalAction action,
 Int4                offset,
 Int4                num_chars,
 CharPtr             char_data,
 Boolean             spliteditmode,
 BioseqPtr           bsp,
 Uint2               moltype,
 Uint2               entityID);
 
NLM_EXTERN SeqEdJournalPtr SeqEdJournalNewFeatEdit
(ESeqEdJournalAction action,
 SeqFeatPtr          sfp,
 SeqLocPtr           slp,
 BioseqPtr           bsp,
 Uint2               moltype,
 Uint2               entityID);
 
NLM_EXTERN SeqEdJournalPtr SeqEdJournalNewTranslate
(SeqFeatPtr sfp,
 BioseqPtr  bsp,
 Uint2      entityID);

NLM_EXTERN Boolean PlayJournal 
(SeqEdJournalPtr      list, 
 SeqEdJournalPtr PNTR last, 
 Int4                 num_steps,
 BoolPtr              pfeats_deleted);
NLM_EXTERN Boolean UnplayJournal (SeqEdJournalPtr PNTR last, Int4 num_steps);

NLM_EXTERN SeqFeatPtr 
SeqEdGetNextFeature 
(BioseqPtr              bsp, 
 SeqFeatPtr             curr,
 Uint1                  seqFeatChoice,
 Uint1                  featDefChoice,
 SeqMgrFeatContext PNTR context,
 Boolean                byLabel,
 Boolean                byLocusTag,
 Uint2                  entityID);

/* this enum describes the kind of motion for feature adjusts */
typedef enum { eLeftEnd=1, eRightEnd, eSlide } EMoveType;

/* This function moves a feature location */
NLM_EXTERN void SeqEdFeatureAdjust
(SeqFeatPtr sfp,
 SeqLocPtr  orig_loc,
 Int4       change,
 EMoveType  move_type,
 Int4       interval_offset,
 BioseqPtr  bsp);

/* This function locates the endpoints of the Nth interval in a SeqLoc */
NLM_EXTERN Boolean SeqEdGetNthIntervalEndPoints 
(SeqLocPtr slp, Int4 n, Int4Ptr left, Int4Ptr right);

/* this function is used to repair the interval order after a feature location
 * interval has been dragged around.
 */
NLM_EXTERN void SeqEdRepairIntervalOrder (SeqFeatPtr sfp, BioseqPtr bsp);
NLM_EXTERN Boolean SeqEdInsert (SeqEdJournalPtr sejp);
NLM_EXTERN void SeqEdReindexAffectedFeatures (Int4 shift_start, Int4 shift_amt, 
                                          Boolean split, BioseqPtr bsp);
NLM_EXTERN void SeqEdReindexFeature (SeqFeatPtr sfp, BioseqPtr bsp);
NLM_EXTERN Boolean SeqEdDeleteFromBsp (SeqEdJournalPtr sejp, BoolPtr pfeats_deleted);


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
