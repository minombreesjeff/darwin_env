#ifndef _SEQGRPHX_
#define _SEQGRPHX_

#ifdef __cplusplus
extern "C" {
#endif

/* structures */

typedef struct seqgraphview
{
  FORM_MESSAGE_BLOCK
  BioseqPtr          bsp;
  GrouP              ftype, mtype, ptype;
  Uint2              entityID, itemID, procID, userKEY;
} SeqGraphView, PNTR SeqGraphViewPtr;

/* prototypes */

Int2 LIBCALLBACK FilterCompGCFunc (Pointer data); 
Int2 LIBCALLBACK FilterDustFunc (Pointer data); 
Int2 LIBCALLBACK PCCPredictFunc (Pointer data); 
Int2 LIBCALLBACK HydrophilicFunc (Pointer data); 
Int2 LIBCALLBACK HydrophobicFunc (Pointer data); 
/* Int2 LIBCALLBACK SeqAnalFunc (Pointer data); */ 

/* sequinx defines */

#define REGISTER_GROUP_FILTERGC ObjMgrProcLoadEx (OMPROC_FILTER, \
        "Percent GC", "GCFilter", \
        OBJ_BIOSEQ, 0, OBJ_BIOSEQ, 0, \
        NULL, FilterCompGCFunc, PROC_PRIORITY_DEFAULT, "Analysis")

#define REGISTER_GROUP_DUST ObjMgrProcLoadEx (OMPROC_FILTER, \
        "DUST", "Dust", \
        OBJ_BIOSEQ, 0, OBJ_BIOSEQ, 0, \
        NULL, FilterDustFunc, PROC_PRIORITY_DEFAULT, "Analysis")

#define REGISTER_GROUP_PCC ObjMgrProcLoadEx (OMPROC_FILTER, \
        "Coiled-coil", "Coiled-coil", \
        OBJ_SEQFEAT, 0, OBJ_SEQFEAT, 0, \
        NULL, PCCPredictFunc, PROC_PRIORITY_DEFAULT, "Analysis")

#define REGISTER_GROUP_HOPP ObjMgrProcLoadEx (OMPROC_FILTER, \
        "Hopp-Woods", "Antigenicity", \
        OBJ_SEQFEAT, 0, OBJ_SEQFEAT, 0, \
        NULL, HydrophilicFunc, PROC_PRIORITY_DEFAULT, "Analysis")

#define REGISTER_GROUP_KYTE ObjMgrProcLoadEx (OMPROC_FILTER, \
        "Kyte-Doolittle", "Hydropathy", \
        OBJ_SEQFEAT, 0, OBJ_SEQFEAT, 0, \
        NULL, HydrophobicFunc, PROC_PRIORITY_DEFAULT, "Analysis")
/*
#define REGISTER_GROUP_ANALYSE ObjMgrProcLoadEx (OMPROC_FILTER, \
        "Sequence Analysis", "Sequence Analysis", \
        OBJ_BIOSEQ, 0, OBJ_BIOSEQ, 0, \
        NULL, SeqAnalFunc, PROC_PRIORITY_DEFAULT, "Analysis")
*/
#ifdef __cplusplus
}
#endif

#endif
