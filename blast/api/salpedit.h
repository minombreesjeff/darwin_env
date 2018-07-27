/* salpedit.h */
#ifndef __SALPEDIT__
#define __SALPEDIT__

#ifdef __cplusplus
extern "C" {
#endif

typedef int (LIBCALLBACK *SeqAlignSortCB) (VoidPtr, VoidPtr);

NLM_EXTERN SeqAlignPtr LIBCALL SeqAlignBioseqDeleteById (SeqAlignPtr salphead, SeqIdPtr sip);
NLM_EXTERN Pointer LIBCALL SeqAlignBioseqDeleteByIdFromSeqEntry (SeqEntryPtr sep, SeqIdPtr sip);
NLM_EXTERN ValNodePtr FilterSeqAlign(SeqAlignPtr align, SeqIdPtr s_sip, Uint1Ptr p_strand);

NLM_EXTERN SeqAlignPtr SeqAlignConvertDspToDdpList(SeqAlignPtr align);
NLM_EXTERN SeqAlignPtr FilterDenseSegAlign(SeqAlignPtr align, SeqIdPtr sip);

NLM_EXTERN Boolean is_loc_overlap(SeqLocPtr loc_1, SeqLocPtr loc_2, Int4Ptr min_val, Int4Ptr max_val, Int4 gap_len, Int4 max_align_size, Int4Ptr p_gap_val);
NLM_EXTERN Int4 find_matching_position(DenseSegPtr dsp, Int4 pos, Int2 pos_order);


NLM_EXTERN Boolean select_overlap_loc(SeqLocPtr loc_1_1, SeqLocPtr loc_2_1, SeqLocPtr loc_1_2, SeqLocPtr loc_2_2, Int2Ptr p_order);

NLM_EXTERN  Int4 get_align_len_in_overlap (SeqAlignPtr align, Int4 from, Int4 to, Int2 order);

NLM_EXTERN Boolean merge_two_align(SeqAlignPtr align_1, SeqAlignPtr align_2, Int4 from, Int4 to, Int2 order);
NLM_EXTERN void SeqAlignReverse(SeqAlignPtr align, Int2 order);

NLM_EXTERN Boolean MergeTwoAlignList (SeqAlignPtr h_align_1, SeqAlignPtr PNTR p_align_2, Int4 from, Int4 to, Int2 order);

NLM_EXTERN SeqAlignPtr SeqAlignSort(SeqAlignPtr salp,SeqAlignSortCB sort_fn);

NLM_EXTERN SeqAlignPtr SeqAlignSortByLength(SeqAlignPtr salp);
NLM_EXTERN SeqAlignPtr SeqAlignSortByScore(SeqAlignPtr salp);

NLM_EXTERN SeqAlignPtr SeqAlignSortByEvalue(SeqAlignPtr salp);

NLM_EXTERN SeqAlignPtr SeqAlignSortByStart(SeqAlignPtr salp,Int2 order);
NLM_EXTERN SeqAlignPtr SeqAlignSortByRegion(SeqAlignPtr salp,Int2 order);
NLM_EXTERN SeqAlignPtr find_best_align(SeqAlignPtr align, Int4Ptr pmax_score);
NLM_EXTERN void SeqAlignSwapOrder(SeqAlignPtr align);


NLM_EXTERN SeqAlignPtr LIBCALL SeqAlignListReverseStrand (SeqAlignPtr salp);
NLM_EXTERN SeqAlignPtr LIBCALL SeqAlignDup (SeqAlignPtr salp);
NLM_EXTERN SeqAlignPtr LIBCALL SeqAlignListDup (SeqAlignPtr salp);

#ifdef __cplusplus
}
#endif

#endif
