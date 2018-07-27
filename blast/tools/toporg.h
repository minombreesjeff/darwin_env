#ifndef _TOPORG_
#define _TOPORG_

/*  $Id: toporg.h,v 6.3 2001/06/05 21:23:43 vakatov Exp $
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
 * Author:  
 *
 * File Description:
 *   
 *
 * ===========================================================================
 */


#include <ncbi.h>
#include <seqport.h>


#ifdef __cplusplus
extern "C" {
#endif


#define Seq_descr_GIBB_mod_dna         0
#define Seq_descr_GIBB_mod_rna         1
#define Seq_descr_GIBB_mod_partial    10
#define Seq_descr_GIBB_mod_complete   11
#define Seq_descr_GIBB_mod_est        20
#define Seq_descr_GIBB_mod_sts        21
#define Seq_descr_GIBB_mod_gss        22


void toporg(SeqEntryPtr sep);
void ChkSegset(SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent);
void ChkNucProt(SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent);
ValNodePtr GetDescr(ValNodePtr PNTR descr);
Boolean check_GIBB(ValNodePtr descr);
ValNodePtr SrchSegChoice(SeqEntryPtr sep, Uint1 choice);
void SrchSegSeqMol(SeqEntryPtr sep);
Boolean CheckSegDescrChoice(SeqEntryPtr sep, Uint1 choice);
void CleanUpSeqDescrChoice(SeqEntryPtr sep, Uint1 choice);
void StripProtXref(SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent);
void CheckMaps(SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent);
void StripMaps(SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent);
void MapsToGenref(SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent);
void FindNewLineage(SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent);
void FindOldLineage(SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent);
void NewLineage(SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent);
void OldLineage(SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent);
void OldPubs(SeqEntryPtr sep);
void NewPubs(SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent);
void MoveSetPubs(SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent);
void DeletePubs(SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent);
void ChangeCitSub(SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent);
void CheckCitSubNew(ValNodePtr vnp);
void CmpPub(SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent);
void MoveSegmPubs(SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent);
void MoveNPPubs(SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent);
void MovePopPhyMutPubs (SeqEntryPtr sep);
ValNodePtr AddToList(ValNodePtr list, ValNodePtr check, PubdescPtr pdp);
BioSourcePtr BioSourceMerge(BioSourcePtr host, BioSourcePtr guest);
BioSourcePtr BioSourceCommon(BioSourcePtr host, BioSourcePtr guest);
void StripBSfromTop(SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent);
void StripBSfromParts(SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent);
Boolean CmpOrgById(BioSourcePtr b1, BioSourcePtr b2);
extern void NormalizeSegSeqMolInfo(SeqEntryPtr sep);


#ifdef __cplusplus
}
#endif

#endif
