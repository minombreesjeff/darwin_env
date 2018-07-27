#ifndef __id2map_h__
#define __id2map_h__

#include <objsset.h>

#define NLM_EXTERN_LOADS {if (! SeqSetAsnLoad())return FALSE; }

#define struct_ID2Param    struct_ID2_Param
#define struct_ID2Request  struct_ID2_Request
#define blob_id_resolve_exclude_blobs_E id_resolve_exclude_blobs_E
#define SeqLiteralFree     SeqLitFree
#define SeqLiteralAsnRead  SeqLitAsnRead
#define SeqLiteralAsnWrite SeqLitAsnWrite

#include <id2sgen.h>

#endif
