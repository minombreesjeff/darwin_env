#ifndef _UTILPUB_
#define _UTILPUB_

#include <asn.h>
#include <objall.h>
#include <objpubd.h>

#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif

/*------------ the structure used for Pubs ---------------*/

typedef struct pubstruct {
	ValNodePtr pub;
	ValNodePtr Pub;
	PubdescPtr descr;
	Int2 number;
    BioseqPtr PNTR bsp;
    Int2 bspcount;
	SeqFeatPtr PNTR citfeat;
	Int2 citcount;
	SeqFeatPtr PNTR pubfeat;
	Int2 pubcount;
	Int2 start;		/* type of ref: 1-descr, 2- pubfeat, 3 - citfeat */
	DatePtr date;		/* need for CompareByDate */
	Int2	choice; /* 1 - PUB_Gen (unpublished), 2 - PUB_Sub, -1 - other */
	Uint2	entityID;
	Uint2   itemID;
	Uint2	itemtype;
	Boolean no_free;
} PubStruct, PNTR PubStructPtr;

NLM_EXTERN ValNodePtr tie_next PROTO((ValNodePtr head, ValNodePtr next));

NLM_EXTERN ValNodePtr StoreNAPubCit PROTO((ValNodePtr PubOnCit, BioseqPtr bsp, SeqFeatPtr sfp));

NLM_EXTERN ValNodePtr StorePub PROTO((BioseqPtr bsp, ValNodePtr vnp, ValNodePtr pub, SeqFeatPtr sfp, Int2 start, Uint2 entityID, Uint2 itemId, Uint2 itemtype));

NLM_EXTERN void UniquePubs PROTO((ValNodePtr PNTR vnpp));

NLM_EXTERN void FreePubStruct PROTO((PubStructPtr psp));

NLM_EXTERN Int2 SelectBestPub PROTO((ValNodePtr pub1, ValNodePtr pub2));

NLM_EXTERN ValNodePtr MinimizePub PROTO((ValNodePtr pub));

NLM_EXTERN Int2 PubLabelMatch PROTO((ValNodePtr pub1, ValNodePtr pub2));

NLM_EXTERN void FindCit PROTO((SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent));

NLM_EXTERN void ChangeCitQual PROTO((SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent));

NLM_EXTERN void DeleteSites PROTO((SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent));

NLM_EXTERN ValNodePtr OrganizePubList PROTO((ValNodePtr vnp));

NLM_EXTERN void cleanup_pub PROTO((ValNodePtr pub));

NLM_EXTERN void EntryStripSerialNumber PROTO((SeqEntryPtr sep));

NLM_EXTERN void VnpHeapSort PROTO ((ValNodePtr PNTR vnp, int (LIBCALLBACK *compar )PROTO ((Nlm_VoidPtr, Nlm_VoidPtr ))));	

NLM_EXTERN SeqFeatPtr remove_feat PROTO((SeqFeatPtr head, SeqFeatPtr x));

NLM_EXTERN SeqAnnotPtr remove_annot PROTO((SeqAnnotPtr head, SeqAnnotPtr x));

NLM_EXTERN ValNodePtr extract_node PROTO((ValNodePtr PNTR head, ValNodePtr x));

NLM_EXTERN ValNodePtr remove_node PROTO((ValNodePtr head, ValNodePtr x));

NLM_EXTERN CharPtr qvalue_extract PROTO((GBQualPtr PNTR qlist, CharPtr qual));

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

#endif
