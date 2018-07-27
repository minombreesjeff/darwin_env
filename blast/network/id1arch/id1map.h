#include <objsset.h>

#define NLM_EXTERN_LOADS {if (! SeqSetAsnLoad())return FALSE; \
                          }
#define struct_Seq_hist seqhist

