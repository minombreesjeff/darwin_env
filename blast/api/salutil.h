/*   salutil.h
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
* File Name:  salutil.h
*
* Author:  Colombe Chappey, H. Sicotte
*
* Version Creation Date:   1/27/96
*
* $Revision: 6.2 $
*
* File Description: 
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* ==========================================================================
*/
#ifndef _SALUTIL_
#define _SALUTIL_

#include <salsa.h>
#include <txalign.h>


/*********************************************************
***
***   Read SeqPort.bsp from SeqPort.start to stop
***   in : spp, from + to in seq coordinates 
***   out: length of buffer + buffer
***
**********************************************************/
NLM_EXTERN Int4 ReadBufferFromSep (SeqPortPtr spp, CharPtr buffer, Int4 from, Int4 to, Int4 buffsegstart);
NLM_EXTERN CharPtr ReadBufferFromSap (CharPtr str, CharPtr buffer, SeqAlignPtr salp, SeqIdPtr sip, Int4 from, Int4 to, Int4 *ncar);

/*******************************************
***
***   String Proc
***
********************************************/
NLM_EXTERN Boolean CCStrToInt (CharPtr str, Int2Ptr intval);
NLM_EXTERN Boolean CCStrToLong (CharPtr str, Int4Ptr longval);

NLM_EXTERN CharPtr dashedstring (CharPtr buffer, Int4 maxbufflength);
NLM_EXTERN CharPtr emptystring (CharPtr buffer, Int4 maxbufflength);
NLM_EXTERN Boolean not_empty_string (CharPtr str, Int4 lens);
NLM_EXTERN Int1    getgapsfromstring (CharPtr str,Int4 from,Int4 to, BoolPtr *gapline);
NLM_EXTERN Boolean stringhasnochar (CharPtr str, Int4 from, Int4 to);
NLM_EXTERN Boolean stringhasnocharplus (CharPtr str);
NLM_EXTERN CharPtr purge_string (CharPtr *str);
NLM_EXTERN CharPtr reverse_string (CharPtr str);
NLM_EXTERN CharPtr to_lower (CharPtr str);
NLM_EXTERN CharPtr complement_string (CharPtr str);
NLM_EXTERN Int4    compare_string (CharPtr str1, CharPtr str2, Int4 *bestscorep);
NLM_EXTERN CharPtr load_seq_data (SeqIdPtr sip, Int4 from, Int4 to, Boolean is_prot, Int4 *lenp);
NLM_EXTERN Boolean IS_protSeqLoc (SeqLocPtr slp);
NLM_EXTERN SeqEntryPtr StringToSeqEntry (CharPtr str, SeqIdPtr sip, Int4 length_align, Uint1 mol_type);

/*******************************************
***
***   ValNode Proc
***
********************************************/
NLM_EXTERN ValNodePtr   ValNodeFind (ValNodePtr head, Int2 start, Int2 index);
NLM_EXTERN ValNodePtr   ValNodeFreeType (ValNodePtr *head, Int2 seqtype);

NLM_EXTERN SeqLocPtr    seqloc2fuzzloc(SeqLocPtr slp, Boolean is_from, Boolean is_to);
NLM_EXTERN TextAlignBufPtr TextAlignBufFind (ValNodePtr anpvnp, Uint2 entityID, Uint2 itemID, Uint2 itemtype);
NLM_EXTERN CharPtr PNTR buf2array (ValNodePtr list, Int2 seq1, Int2 seq2);
NLM_EXTERN AlignNodePtr AlignNodeFind (ValNodePtr anpvnp, Uint2 entityID, Uint2 itemID, Uint2 itemtype);

NLM_EXTERN Int2         AlignNodeIndex (ValNodePtr anpvnp, Uint2 entityID, Uint2 itemID, Uint2 itemtype);
NLM_EXTERN void         OrderFeatProc (ValNodePtr vnpanp);
NLM_EXTERN ValNodePtr   SeqfeatlistFree (ValNodePtr feathead);
NLM_EXTERN ValNodePtr   SeqfeatlistFree_fromID (ValNodePtr feathead, Uint2 entityID);
NLM_EXTERN SelEdStructPtr get_feat_fromid (ValNodePtr feat_vnp, Uint2 feattype, Uint2 ei, Uint2 ii, Int4 pos, SelEdStructPtr *prec);

NLM_EXTERN SeqLocPtr    CollectSeqLocFromAlignNode (AlignNodePtr anp);
NLM_EXTERN Int4         GetAlignLengthFromAlignNode (AlignNodePtr anp);

NLM_EXTERN SeqIdPtr     SeqIdFromAlignNode (ValNodePtr anp_lst, Uint2 entityID, Uint2 itemID, Uint2 itemtype);
NLM_EXTERN Uint1 StrandFromAlignNode (ValNodePtr anp_lst, Uint2 entityID, Uint2 itemID, Uint2 itemtype);

/*********************************************************
***
***  SeqIdPtr procedures
***    AddSeqId  : create a new seqid and add at the end
***                of the list starting with sip_head
***
***    SeqIdDupList : duplicate a list of SeqIdPtr
***
**********************************************************/
NLM_EXTERN CharPtr      matching_seqid (SeqIdPtr sip1);
NLM_EXTERN CharPtr      check_seqid (Uint2 choice, CharPtr ptr);
NLM_EXTERN SeqIdPtr     ValNodeSeqIdListDup (ValNodePtr id_list);
NLM_EXTERN CharPtr PNTR SeqIdListToCharArray (SeqIdPtr id_list, Int2 n);

NLM_EXTERN SeqIdPtr     SeqIdReplaceID (SeqIdPtr head, SeqIdPtr pre, SeqIdPtr sip, SeqIdPtr next);
NLM_EXTERN BioseqPtr    BioseqReplaceID (BioseqPtr bsp, SeqIdPtr newsip);
NLM_EXTERN SeqEntryPtr  SeqEntryReplaceSeqID (SeqEntryPtr source_sep, SeqIdPtr sip);
/*********************************************************
***
***  ScorePtr procedures
***
**********************************************************/
NLM_EXTERN ScorePtr ScoreDup (ScorePtr sp);
NLM_EXTERN ScorePtr ScoreDupAdd (ScorePtr *sp_head, ScorePtr sp);
NLM_EXTERN ScorePtr ScoreAdd (ScorePtr *sp_head, ScorePtr sp);

/*********************************************************
***
***  SeqLocPtr procedures
***
**********************************************************/
NLM_EXTERN Int2      chkloc (SeqIdPtr sip, Int4 position, ValNodePtr vnp, Int4 *newpos);
NLM_EXTERN SeqLocPtr expand_seq_loc(Int4 start, Int4 stop, Uint1 strand, SeqLocPtr loc);
NLM_EXTERN Int4      MaxLengthSeqLoc (ValNodePtr sqloc_list);
NLM_EXTERN Boolean   SeqLocListMatch (ValNodePtr vnp1, ValNodePtr vnp2, Boolean *Fp, Boolean *Tp);

/***********************************************************************
***    SeqEntryToSeqLoc
***      read SeqEntry (1->Bioseq or 2->BioseqSetPtr)
***      return list of ValNodePtr->SeqLocPtr
************************************************************************/
NLM_EXTERN ValNodePtr SeqEntryToSeqLoc (SeqEntryPtr sep, Int2 *n, Uint1 bsp_mol);

/***********************************************************************
***     SeqLocToSeqId
***        read a list of SeqIdPtr, check if each SIP is NOT already open
***        built a list of SIP and open SeqPort on the SIP
***********************************************************************/
NLM_EXTERN SeqIdPtr SeqLocToSeqId (ValNodePtr sqloc_list);

/*********************************************************
***
***   SelStruct Procedures
***
**********************************************************/
NLM_EXTERN SelStructPtr SelStructNew (Uint2 ssp_ed, Uint2 ssp_id, Uint2 ssp_it, Int4 from, Int4 to, SeqIdPtr sip, Uint1 strand, Boolean is_fuzz);
NLM_EXTERN SelStructPtr SelStructCpy (SelStructPtr ssp, SelStructPtr ssp2);
NLM_EXTERN SelStructPtr SelStructDup (SelStructPtr ssp);
NLM_EXTERN SelStructPtr SelStructAdd (SelStructPtr head, SelStructPtr ssp);
NLM_EXTERN SelStructPtr SelStructDel (SelStructPtr ssp);
NLM_EXTERN SelStructPtr SelStructDelList (SelStructPtr ssp);
NLM_EXTERN void setposition_tossp (SelStructPtr ssp, Int4 from, Int4 to);

NLM_EXTERN Boolean is_samessp (SelStructPtr ssp1, SelStructPtr ssp2);
NLM_EXTERN Boolean is_sameId (Uint2 sei, Uint2 sii, Uint2 sit, Uint2 sist, Uint2 ei, Uint2 ii, Uint2 it, Uint2 ist);
NLM_EXTERN Boolean is_samepos (SelStructPtr ssp1, SelStructPtr ssp2);
NLM_EXTERN ValNodePtr del_ssp_fromid (ValNodePtr headp, Uint2 itemsubtype, SelEdStructPtr target);
NLM_EXTERN Boolean include_ssp (SeqLocPtr slp1, SeqLocPtr slp2);
NLM_EXTERN Int4    overlapp_startssp (SeqLocPtr slp1, SeqLocPtr slp2);
NLM_EXTERN Boolean overlapp_ssp (SeqLocPtr slp1, SeqLocPtr slp2);
NLM_EXTERN Boolean precede_ssp (SeqLocPtr slp1, SeqLocPtr slp2);
NLM_EXTERN Boolean succeed_ssp (SeqLocPtr slp1, SeqLocPtr slp2);

/*********************************************************
***
***   SelEdStruct Procedures
***
**********************************************************/
NLM_EXTERN SelEdStructPtr new_seledstruct (Uint2 ssp_ed, Uint2 ssp_id, Uint2 ssp_it, Uint2 ssp_sit, Uint2 bspiID, Int4 from, Int4 to, SeqIdPtr sip, Uint1 strand, Boolean is_fuzz, CharPtr label, Pointer data, Int4 offset, Uint1 codonstart);
NLM_EXTERN SelEdStructPtr new_seledstruct_fromseqloc (Uint2 ssp_ed, Uint2 ssp_id, Uint2 ssp_it, Uint2 ssp_sit, Uint2 bspiID, SeqLocPtr slp, CharPtr label, Pointer data, Int4 offset, Uint1 codonstart);
NLM_EXTERN SeqLocPtr      sesp_to_slp (SelEdStructPtr ses, SeqAlignPtr salp, ValNodePtr sqlocs, Boolean partial);

NLM_EXTERN SelEdStructPtr SelEdStructDup (SelEdStructPtr ssp);
NLM_EXTERN SelEdStructPtr SelEdStructAdd (SelEdStructPtr head, SelEdStructPtr ssp);
NLM_EXTERN SelEdStructPtr SelEdStructDel (SelEdStructPtr ssp);
NLM_EXTERN SelEdStructPtr SelEdStructListDel (SelEdStructPtr ssp);
NLM_EXTERN void           setposition_toses (SelEdStructPtr ssp, Int4 from, Int4 to);
NLM_EXTERN void           set_seqnot_visible (Uint2 eID, Uint2 iID, SelEdStructPtr sesp);
NLM_EXTERN void           set_seqvisible (Uint2 eID, Uint2 iID, SelEdStructPtr sesp);
NLM_EXTERN Boolean        is_seqvisible (Uint2 eID, Uint2 iID, SelEdStructPtr sesp);
NLM_EXTERN SelEdStructPtr ss_to_ses (SelStructPtr ssp);
NLM_EXTERN SelStructPtr   ses_to_ss (SelEdStructPtr sesp);
NLM_EXTERN Boolean        is_samess_ses (SelStructPtr ssp1, SelEdStructPtr ssp2);
NLM_EXTERN Boolean        is_sameses (SelEdStructPtr ssp1, SelEdStructPtr ssp2);

/*********************************************************
***
***   ObjMgr Procedures
***
**********************************************************/
NLM_EXTERN Boolean        AfterAlsoSelect (void);
NLM_EXTERN void           ObjMgrSelectPrint (void);
NLM_EXTERN void           SelectType (EditAlignDataPtr adp, Uint2 feattype, Int4 slpto);
NLM_EXTERN Int2           GetNumberObjMgrSelect (void);
NLM_EXTERN Int2           checkOMss_for_itemtype (Uint2 itemtype);
NLM_EXTERN SelStructPtr   getOMselect_for_itemtype (Uint2 itemtype);
NLM_EXTERN SelStructPtr   is_selectedbyID (Uint2 entityID, Uint2 itemID, Uint2 itemtype);
NLM_EXTERN SelEdStructPtr getCDSselect (ValNodePtr seqfeathead, ValNodePtr feathead);
NLM_EXTERN Int2           checkCDSselect_forprotein (ValNodePtr seqfeathead, ValNodePtr feathead, Boolean with_prot);
NLM_EXTERN Boolean        checkssp_for_editor (SelStructPtr ssp);
NLM_EXTERN SeqLocPtr      checkseqlocfeature_for_editor (Uint2 entityID, Uint2 itemID, ValNodePtr headfeat);
NLM_EXTERN void           checkselectsequinfeature_for_editor (ValNodePtr headfeat);
NLM_EXTERN Int4           getminpos_fromOMselect (Uint2 itemsubtype);

/*********************************************************
*** * * * * * * * * * * * * * * * * * * * * * * * * * * * 
***
***  SeqCoordToAlignCoord procedures
***  AlignCoordToSeqCoord procedures
***
*** * * * * * * * * * * * * * * * * * * * * * * * * * * * 
**********************************************************/
NLM_EXTERN Boolean  locate_in_seqalignds  (Int4 pos, Int2 dim, Int2 dspnumseg, Int4Ptr *dspstart, Int4Ptr *dsplens, Int2 *numseg_before, Int2 *subdsplens);
NLM_EXTERN Boolean  locate_in_seqalign (Int4 pos, Int2 dim, Int2 dspnumseg, BoolPtr *dspstart, Int4Ptr *dsplens, Int2 *numseg_before, Int2 *subdsplens, Int4 *sumdsplens_before);
NLM_EXTERN Int4 SeqCoordToAlignCoord (Int4 position, SeqIdPtr sip, SeqAlignPtr salp, Int2 intersalpwidth, Int2 is_end);
NLM_EXTERN Int4 AlignCoordToSeqCoord (Int4 position, SeqIdPtr sip, SeqAlignPtr salp, ValNodePtr sqloc_list, Int2 intersalpwidth);
NLM_EXTERN Int4 AlignCoordToSeqCoord2 (Int4 position, SeqIdPtr sip, SeqAlignPtr salp,ValNodePtr sqloc_list, Int2 intersalpwidth);
NLM_EXTERN Boolean GetAlignCoordFromSeqLoc (SeqLocPtr slp, SeqAlignPtr salp, Int4 *start, Int4 *stop);
NLM_EXTERN Boolean  SeqPosToLineColumn (Uint2 itemID, Uint2 entityID, Uint2 itemtype, 
           Int4 pos, Int4 *line, Int4 *column, Int4 hoffset, EditAlignDataPtr adp);
NLM_EXTERN Boolean  SeqPosInLineColumn (Int4 pos, Int4 alignline, Int4 *column, 
           Int4 hoffset, EditAlignDataPtr adp);
NLM_EXTERN Boolean  LocateInSeqAlign (Int4 pos, Int2 dim, Int2 dspnumseg, BoolPtr *dspstart, Int4Ptr *dsplens, Int2 *numseg_before, Int4 *subdsplens, Int4 *sumdsplens_before);
NLM_EXTERN Boolean LocateInSeqAlignDenSeg (Int4 pos, Int2 dim, Int2 dspnumseg, Int4Ptr *dspstart, Int4Ptr *dsplens, Int2 *numseg_before, Int4 *subdsplens);

/*********************************************************
***
***   Find Matching Pattern Procedures
***
**********************************************************/
NLM_EXTERN SelStructPtr SetupMatchPatList (ValNodePtr match_pat, ValNodePtr anp_list);
NLM_EXTERN SelStructPtr ShowNextPattern (SelStructPtr match_pat, SelStructPtr cur_pat, Int4 start);
NLM_EXTERN SelStructPtr ShowPrecPattern (SelStructPtr match_pat, SelStructPtr cur_pat, Int4 start);
/*
NLM_EXTERN ValNodePtr EditFindPattern (CharPtr str, ValNodePtr sqloc_list, Uint1 mol_type);
*/

/*********************************************************
***
***   Editing Procedures
***
**********************************************************/
NLM_EXTERN CharPtr char_to_insert (Char *ch, Int4 lens, Uint1 mol_type);
NLM_EXTERN Boolean insertchar (CharPtr str, Int4 pos, SeqIdPtr target, Uint1 mol_type, Boolean spliteditmode);
NLM_EXTERN Boolean insertchar_atcaret (CharPtr str, EditAlignDataPtr adp);

NLM_EXTERN SeqEntryPtr getfirst_sep(SeqEntryPtr sep, Uint1 bsp_mol);

#endif

