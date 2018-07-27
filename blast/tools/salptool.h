/* salpedit.h */
#ifndef __SALPTOOL__
#define __SALPTOOL__
#include <ncbi.h>
#include <objalign.h>
#include <blast.h>

typedef struct p_seqaligninfo{
	SeqAlignPtr sap;
	SeqIdPtr sip;
	Boolean used;
	struct p_seqaligninfo PNTR next;
} PSeqAlignInfo, PNTR PSeqAlignInfoPtr;

NLM_EXTERN void SeqAlignReverseOrder(SeqAlignPtr align);
NLM_EXTERN void SeqAlignSwapSeqs(SeqAlignPtr align);
NLM_EXTERN SeqAlignPtr BlastTwoSequencesByLocBOTH(SeqLocPtr slp1, SeqLocPtr slp2, CharPtr program, BLAST_OptionsBlkPtr options);
NLM_EXTERN SeqAlignPtr SeqAlignSplitBlastTwoSeq(SeqLocPtr slp1, SeqLocPtr slp2, 
		Int4 split_len, Int4 overlap_len, BLAST_OptionsBlkPtr options);
NLM_EXTERN PSeqAlignInfoPtr SeqAlignToPSeqAlignInfo (SeqAlignPtr sap);
NLM_EXTERN SeqAlignPtr ReassembleSeqAlignFromPSeqAlignInfo(PSeqAlignInfoPtr alip);
NLM_EXTERN SeqAlignPtr SeqAlignSplitGappedBlast(SeqLocPtr slp1, CharPtr progname, CharPtr database, ValNodePtr *other_returns, ValNodePtr *error_returns, Int4 split_len, Int4 overlap_len, BLAST_OptionsBlkPtr options);
NLM_EXTERN SeqAlignPtr SeqAlignSetGlobalFromLocal(SeqAlignPtr align,SeqLocPtr loc_1, SeqLocPtr loc_2, BioseqPtr bsp_1, BioseqPtr bsp_2, FILE *err_fp,Int4 MaxGap);
NLM_EXTERN SeqAlignPtr compute_alignment(SeqLocPtr loc_1, SeqLocPtr loc_2, BioseqPtr bsp_1, BioseqPtr bsp_2, FILE *err_fp, BLAST_OptionsBlkPtr options, Boolean ck_polyA);

/****************************************************
 ValidateSeqAlignandACC

   validates a SeqAlign using the ValidateSeqAlign function 
   in api directory, and tests for occurrence of ACC string in sequence ID.
   ACC|ACC# will be compared with the corresponding sequence (ACC#)
   in the database and replaced by a far pointer if the sequences
   are identical. 

******************************************************/
NLM_EXTERN Int2 LIBCALLBACK ValidateSeqAlignandACCFromData (Pointer data);

NLM_EXTERN Boolean ValidateSeqAlignandACC (SeqAlignPtr salp, Uint2 entityID, 
                   Boolean message,
                   Boolean msg_success, Boolean find_remote_bsp,
                   Boolean find_acc_bsp,
                   Boolean delete_bsp, Boolean delete_salp, BoolPtr dirty);

NLM_EXTERN Boolean ValidateSeqAlignandACCInSeqEntry (SeqEntryPtr sep, 
                   Boolean message, 
                   Boolean msg_success, Boolean find_remote_bsp, 
                   Boolean find_acc_bsp,
                   Boolean delete_bsp, Boolean delete_salp);

#endif
