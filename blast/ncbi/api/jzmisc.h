/* $Id: jzmisc.h,v 6.4 1998/07/22 00:05:07 zjing Exp $
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
* File Name:  $RCSfile: jzmisc.h,v $
*
* Author:  Jinghui Zhang
*
* Initial Version Creation Date: 03/24/97
*
* $Revision: 6.4 $
*
* File Description:
*         Miscelaneous functions for alignments.
*
* $Log: jzmisc.h,v $
* Revision 6.4  1998/07/22 00:05:07  zjing
* move SortValNode from maputil to jzmisc
*
* Revision 6.3  1998/05/21 22:23:36  zjing
* new function: AddAlignInfoToSeqAnnotEx
*
* Revision 6.2  1997/09/25 02:00:21  vakatov
* Added NLM_EXTERN specifier to some functions(necessary for MS-Win DLLs)
*
* Revision 6.1  1997/09/16 20:04:23  zjing
* move AddAlignInfoToSeqAnnot from simutil.h to jzmisc.h
*
* Revision 6.0  1997/08/25 18:06:25  madden
* Revision changed to 6.0
*
* $Revision: 6.4 $
*
* File Description:
*         Miscelaneous functions for alignments. 
*
* $Log: jzmisc.h,v $
* Revision 6.4  1998/07/22 00:05:07  zjing
* move SortValNode from maputil to jzmisc
*
* Revision 6.3  1998/05/21 22:23:36  zjing
* new function: AddAlignInfoToSeqAnnotEx
*
* Revision 6.2  1997/09/25 02:00:21  vakatov
* Added NLM_EXTERN specifier to some functions(necessary for MS-Win DLLs)
*
* Revision 6.1  1997/09/16 20:04:23  zjing
* move AddAlignInfoToSeqAnnot from simutil.h to jzmisc.h
*
* Revision 5.8  1997/06/19 18:38:14  vakatov
* [WIN32,MSVC++]  Adopted for the "NCBIOBJ.LIB" DLL'ization
*
* Revision 5.7  1997/03/24 20:55:29  shavirin
* Added protection for usage with C++ Compiler. Added NCBI header and
* started logging.
*
*
* ==========================================================================
*/

#ifndef _JZMISC_
#define _JZMISC_


#include <ncbi.h>
#include <objfeat.h>
#include <objseq.h>
#include <objloc.h>
#include <objalign.h>
#include <sequtil.h>

#define IS_SEQ		1
#define IS_CLONE	2
#define IS_YAC		3

#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif

#ifdef __cplusplus
extern "C" {
#endif

NLM_EXTERN Boolean show_locus;	/*show the locus name of the sequnce*/

NLM_EXTERN SeqIdPtr gb_id_make(CharPtr, CharPtr);
NLM_EXTERN SeqIdPtr general_id_make(CharPtr, CharPtr);
NLM_EXTERN void bb_sort(Int4Ptr, Int2);
NLM_EXTERN Boolean get_synonym(ValNodePtr, CharPtr);
NLM_EXTERN Boolean find_synonym(SeqFeatPtr, CharPtr);
NLM_EXTERN Boolean check_syn(ValNodePtr, CharPtr);
NLM_EXTERN SeqIdPtr find_sip(SeqIdPtr);

NLM_EXTERN Boolean  get_seq_name(SeqIdPtr, CharPtr, CharPtr, CharPtr, Boolean);
NLM_EXTERN Boolean get_sip_comment(SeqIdPtr, CharPtr);
NLM_EXTERN void make_empty(CharPtr, Int2);
NLM_EXTERN Boolean is_sequenced(SeqLocPtr);
NLM_EXTERN SeqLocPtr fuzz_loc(Int4, Int4, Uint1, SeqIdPtr, Boolean, Boolean);
NLM_EXTERN Uint1 label_feature(SeqFeatPtr, CharPtr, Int2, Boolean);


NLM_EXTERN SeqLocPtr link_loc(SeqLocPtr, SeqLocPtr);
NLM_EXTERN void swap(Int4Ptr, Int4Ptr);
NLM_EXTERN void s_witch(Int4Ptr, Int4Ptr);

NLM_EXTERN Boolean lr_offset_in_slp(SeqLocPtr, Int4, Int4, Int4Ptr, Int4Ptr);
NLM_EXTERN Uint1 get_r_strand(SeqLocPtr, SeqLocPtr);
NLM_EXTERN Int2 find_f_pos(Int4, Int4, Int4Ptr, Int4, Int2);

NLM_EXTERN Int2 find_group_pos PROTO((Int4Ptr p_pos, Int4 space, Int2 num, Int2 group_size, Int4Ptr group_value));
NLM_EXTERN Int1 loc_offset(SeqLocPtr, SeqLocPtr, Int4Ptr, Int4Ptr, BoolPtr, BoolPtr);
NLM_EXTERN Int1 tloc_offset(SeqLocPtr, SeqLocPtr, Int4Ptr, Int4Ptr);

NLM_EXTERN SeqLocPtr make_range(Int4, Int4, Int4, SeqIdPtr, ValNodePtr PNTR);

NLM_EXTERN SeqAlignPtr find_nth_align(SeqAlignPtr, Int2);
NLM_EXTERN SeqAlignPtr get_bsp_align(BioseqPtr);
NLM_EXTERN SeqAlignPtr get_sep_align(SeqEntryPtr);
NLM_EXTERN Int2 m_id_match(SeqAlignPtr, SeqIdPtr);
NLM_EXTERN SeqIdPtr get_align_id(SeqAlignPtr, Int2);
NLM_EXTERN Uint1 get_align_strand(SeqAlignPtr, Int2);

NLM_EXTERN Uint1 get_clone_type(SeqIdPtr);

NLM_EXTERN SeqFeatPtr get_bsp_feat(BioseqPtr);
NLM_EXTERN SeqFeatPtr make_ext_feat(BioseqPtr);

NLM_EXTERN Boolean  check_match(Char, Char);
NLM_EXTERN Boolean SeqLocReplace(SeqIdPtr, Int4, Int4, Uint1, SeqIdPtr, Int4, Int4, Uint1);

NLM_EXTERN Boolean get_feat_id(SeqFeatPtr, CharPtr, BoolPtr, CharPtr, BoolPtr);
NLM_EXTERN Boolean get_gene_syn(SeqFeatPtr, CharPtr, BoolPtr, CharPtr, BoolPtr);
NLM_EXTERN Boolean get_pnt_val(SeqLocPtr, Int4Ptr, Uint1Ptr, BoolPtr);
NLM_EXTERN Boolean DelNthFeat(SeqFeatPtr PNTR, Int2);
NLM_EXTERN Boolean PackSeqPntInsert(PackSeqPntPtr, Int4);
NLM_EXTERN Boolean insert_new_rsite(BioseqPtr, CharPtr, Int4);
NLM_EXTERN Boolean PackSeqPntDelete(PackSeqPntPtr PNTR, Int2);
NLM_EXTERN Boolean delete_rsite(BioseqPtr, Int2, Int2);
NLM_EXTERN Boolean DeleteNthAlign(SeqAlignPtr PNTR, Int2);

NLM_EXTERN Boolean Is_Local_Seq(SeqIdPtr);
NLM_EXTERN void write_out_put(SeqEntryPtr, CharPtr, Boolean);

NLM_EXTERN Int4 slp_list_len(SeqLocPtr );
NLM_EXTERN Int4 get_node_num(ValNodePtr);

NLM_EXTERN Boolean BioseqHasFeature PROTO((BioseqPtr bsp));

NLM_EXTERN Int2 get_vnp_num PROTO((ValNodePtr vnp));

/*******************************************************************
*
*	make_master(sap_p)
*	return the Seq-id of the sequence with the highest appearance
*	frequency in a list of Seq-align
*	sap_p: a list of Seq-align
*	the return the Seq-id will be treated as the master in a 
*	multiple pairwise alignment
*
*******************************************************************/
NLM_EXTERN SeqIdPtr make_master PROTO(( SeqAlignPtr sap_p));

NLM_EXTERN Boolean use_multiple_dimension PROTO((SeqAlignPtr align));
NLM_EXTERN void get_boundary PROTO((SeqIdPtr m_sip, Int4Ptr start, Int4Ptr stop, SeqAlignPtr align));

/*
*
*	Add the information about the alignment to Seq-annot
*
*/
NLM_EXTERN void AddAlignInfoToSeqAnnot PROTO((SeqAnnotPtr annot, Uint1 align_type));

NLM_EXTERN void AddAlignInfoToSeqAnnotEx PROTO((SeqAnnotPtr annot, Uint1 align_type, CharPtr label));

NLM_EXTERN ValNodePtr SortValNode PROTO((ValNodePtr list, int (LIBCALLBACK *compar )PROTO ((Nlm_VoidPtr, Nlm_VoidPtr ))));

NLM_EXTERN SeqAlignPtr sort_align_by_score PROTO((SeqAlignPtr PNTR halign));

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



