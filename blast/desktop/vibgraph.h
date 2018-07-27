#ifndef _VIBGRAPH_
#define _VIBGRAPH_

#include <picture.h>
#include <viewer.h>

#include <urkgraph.h>

#ifdef __cplusplus
extern "C" {
#endif

#define REGISTER_GRAPH_ANNOT_EDIT ObjMgrProcLoad (OMPROC_EDIT, \
        "View Graph", "Graph", \
        OBJ_SEQANNOT, 3, OBJ_SEQANNOT, 3, \
        NULL, GraphGenFunc, PROC_PRIORITY_DEFAULT)

#define REGISTER_GRAPH_SGP_EDIT ObjMgrProcLoad (OMPROC_EDIT, \
        "View Graph", "Graph", \
         OBJ_SEQGRAPH, 0,  OBJ_SEQGRAPH, 0, \
        NULL, GraphGenFunc, PROC_PRIORITY_DEFAULT)

#define REGISTER_GRAPH_ANNOTDESC_EDIT ObjMgrProcLoad (OMPROC_EDIT, \
        "View Graph", "Graph", \
        OBJ_ANNOTDESC, 3, OBJ_ANNOTDESC, 3, \
        NULL, GraphGenFunc, PROC_PRIORITY_DEFAULT)

#define REGISTER_GROUP_GRAPH_FILTER ObjMgrProcLoadEx (OMPROC_FILTER, \
        "View Graph", "Graph", \
        OBJ_ANNOTDESC, 3, OBJ_ANNOTDESC, 3, \
        NULL, GraphGenFunc, PROC_PRIORITY_DEFAULT, "Graph Filter")

typedef struct graphviewform
{
  FORM_MESSAGE_BLOCK
  VieweR         v;               /* graph viewer */
  SegmenT        s, sy;           /* graph picture, y scale */
  GraphInfoPtr   gip;             /* graph information */
  Int4           maxzoom, zoom;   /* */
  Int4           maxpixelwidth;   /* */
  PopuP          ZoomPopUp;       /* */
  Uint2          entityID, itemID, procID, userKEY;
} GraphViewForm, PNTR GraphViewFormPtr;

extern void GraphInfoPtrToGraphViewForm (ForM f, Pointer data);

extern ForM xCreateGraphViewForm (Int2 left, Int2 top, CharPtr title);

extern Int2 LIBCALLBACK GraphGenFunc (Pointer data);

#ifdef __cplusplus
}
#endif

#endif
