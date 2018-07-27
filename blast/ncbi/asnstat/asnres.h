/***********************************************************************
*
**
*        Automatic header module from ASNTOOL
*
************************************************************************/

#ifndef _ASNTOOL_
#include <asn.h>
#endif

static char * asnfilename = "asnres.h60";
static AsnType atx[39] = {
  {401, "Seq-graph" ,1,0,0,0,0,1,0,0,NULL,&atx[23],&atx[1],0,&atx[5]} ,
  {0, "title" ,128,0,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[3]} ,
  {323, "VisibleString" ,0,26,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "comment" ,128,1,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[4]} ,
  {0, "loc" ,128,2,0,0,0,0,0,0,NULL,&atx[5],NULL,0,&atx[6]} ,
  {402, "Seq-loc" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[16]} ,
  {0, "title-x" ,128,3,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[7]} ,
  {0, "title-y" ,128,4,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[8]} ,
  {0, "comp" ,128,5,0,1,0,0,0,0,NULL,&atx[9],NULL,0,&atx[10]} ,
  {302, "INTEGER" ,0,2,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "a" ,128,6,0,1,0,0,0,0,NULL,&atx[11],NULL,0,&atx[12]} ,
  {309, "REAL" ,0,9,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "b" ,128,7,0,1,0,0,0,0,NULL,&atx[11],NULL,0,&atx[13]} ,
  {0, "numval" ,128,8,0,0,0,0,0,0,NULL,&atx[9],NULL,0,&atx[14]} ,
  {0, "graph" ,128,9,0,0,0,0,0,0,NULL,&atx[38],&atx[15],0,NULL} ,
  {0, "real" ,128,0,0,0,0,0,0,0,NULL,&atx[16],NULL,0,&atx[24]} ,
  {403, "Real-graph" ,1,0,0,0,0,0,0,0,NULL,&atx[23],&atx[17],0,&atx[25]} ,
  {0, "max" ,128,0,0,0,0,0,0,0,NULL,&atx[11],NULL,0,&atx[18]} ,
  {0, "min" ,128,1,0,0,0,0,0,0,NULL,&atx[11],NULL,0,&atx[19]} ,
  {0, "axis" ,128,2,0,0,0,0,0,0,NULL,&atx[11],NULL,0,&atx[20]} ,
  {0, "values" ,128,3,0,0,0,0,0,0,NULL,&atx[22],&atx[21],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[11],NULL,0,NULL} ,
  {312, "SEQUENCE OF" ,0,16,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {311, "SEQUENCE" ,0,16,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "int" ,128,1,0,0,0,0,0,0,NULL,&atx[25],NULL,0,&atx[31]} ,
  {404, "Int-graph" ,1,0,0,0,0,0,0,0,NULL,&atx[23],&atx[26],0,&atx[32]} ,
  {0, "max" ,128,0,0,0,0,0,0,0,NULL,&atx[9],NULL,0,&atx[27]} ,
  {0, "min" ,128,1,0,0,0,0,0,0,NULL,&atx[9],NULL,0,&atx[28]} ,
  {0, "axis" ,128,2,0,0,0,0,0,0,NULL,&atx[9],NULL,0,&atx[29]} ,
  {0, "values" ,128,3,0,0,0,0,0,0,NULL,&atx[22],&atx[30],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[9],NULL,0,NULL} ,
  {0, "byte" ,128,2,0,0,0,0,0,0,NULL,&atx[32],NULL,0,NULL} ,
  {405, "Byte-graph" ,1,0,0,0,0,0,0,0,NULL,&atx[23],&atx[33],0,NULL} ,
  {0, "max" ,128,0,0,0,0,0,0,0,NULL,&atx[9],NULL,0,&atx[34]} ,
  {0, "min" ,128,1,0,0,0,0,0,0,NULL,&atx[9],NULL,0,&atx[35]} ,
  {0, "axis" ,128,2,0,0,0,0,0,0,NULL,&atx[9],NULL,0,&atx[36]} ,
  {0, "values" ,128,3,0,0,0,0,0,0,NULL,&atx[37],NULL,0,NULL} ,
  {304, "OCTET STRING" ,0,4,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {315, "CHOICE" ,0,-1,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} };

static AsnModule ampx[1] = {
  { "NCBI-Seqres" , "asnres.h60",&atx[0],NULL,NULL,0,0} };

static AsnValxNodePtr avn = NULL;
static AsnTypePtr at = atx;
static AsnModulePtr amp = ampx;



/**************************************************
*
*    Defines for Module NCBI-Seqres
*
**************************************************/

#define SEQ_GRAPH &at[0]
#define SEQ_GRAPH_title &at[1]
#define SEQ_GRAPH_comment &at[3]
#define SEQ_GRAPH_loc &at[4]
#define SEQ_GRAPH_title_x &at[6]
#define SEQ_GRAPH_title_y &at[7]
#define SEQ_GRAPH_comp &at[8]
#define SEQ_GRAPH_a &at[10]
#define SEQ_GRAPH_b &at[12]
#define SEQ_GRAPH_numval &at[13]
#define SEQ_GRAPH_graph &at[14]
#define SEQ_GRAPH_graph_real &at[15]
#define SEQ_GRAPH_graph_int &at[24]
#define SEQ_GRAPH_graph_byte &at[31]

#define REAL_GRAPH &at[16]
#define REAL_GRAPH_max &at[17]
#define REAL_GRAPH_min &at[18]
#define REAL_GRAPH_axis &at[19]
#define REAL_GRAPH_values &at[20]
#define REAL_GRAPH_values_E &at[21]

#define INT_GRAPH &at[25]
#define INT_GRAPH_max &at[26]
#define INT_GRAPH_min &at[27]
#define INT_GRAPH_axis &at[28]
#define INT_GRAPH_values &at[29]
#define INT_GRAPH_values_E &at[30]

#define BYTE_GRAPH &at[32]
#define BYTE_GRAPH_max &at[33]
#define BYTE_GRAPH_min &at[34]
#define BYTE_GRAPH_axis &at[35]
#define BYTE_GRAPH_values &at[36]
