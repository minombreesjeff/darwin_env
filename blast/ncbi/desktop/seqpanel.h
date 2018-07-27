/* $Id: seqpanel.h,v 6.12 2005/03/02 17:19:35 bollin Exp $
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
* Author:  Vlad Lebedev
**
* ==========================================================================
*/

#ifndef seqpanel_h
#define seqpanel_h
extern PaneL CreateSeqViewPanel (GrouP g, Int2 w, Int2 h);

extern void WriteAlignmentContiguousToFile
(SeqAlignPtr salp,
 FILE        *fp,
 Int4        seq_chars_per_row,
 Boolean     show_substitutions);
extern void 
WriteAlignmentInterleaveToFile 
(SeqAlignPtr salp,
 FILE        *fp,
 Int4        seq_chars_per_row,
 Boolean     show_substitutions); 

extern ForM CreateSeqEditorWindow (Int2 left, Int2 top, CharPtr windowname, BioseqPtr bsp);

NLM_EXTERN void SeqEdTranslateOneCDS (SeqFeatPtr sfp, BioseqPtr featbsp, Uint2 entityID);
NLM_EXTERN void SeqEdRemapLocation (SeqAlignPtr salp, SeqLocPtr slp, Int4 seq_len);
NLM_EXTERN Boolean SeqEdFixProteinFeatures (BioseqPtr oldbsp, BioseqPtr newbsp, Boolean force_fix);
NLM_EXTERN SeqAlignPtr Sequin_GlobalAlign2Seq (BioseqPtr bsp1, BioseqPtr bsp2, BoolPtr revcomp);
extern ForM CreateAlnEditorWindow (Int2 left, Int2 top, CharPtr windowname, SeqAlignPtr salp, Uint2 entityID);
#endif
