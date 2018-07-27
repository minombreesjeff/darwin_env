/***********************************************************************
*
**
*        Automatic header module from ASNTOOL
*
************************************************************************/

#ifndef _ASNTOOL_
#include <asn.h>
#endif

static char * asnfilename = "asnalign.h60";
static AsnValxNode avnx[10] = {
    {20,"not-set" ,0,0.0,&avnx[1] } ,
    {20,"global" ,1,0.0,&avnx[2] } ,
    {20,"diags" ,2,0.0,&avnx[3] } ,
    {20,"partial" ,3,0.0,&avnx[4] } ,
    {20,"disc" ,4,0.0,&avnx[5] } ,
    {20,"other" ,255,0.0,NULL } ,
    {3,NULL,2,0.0,NULL } ,
    {3,NULL,2,0.0,NULL } ,
    {3,NULL,2,0.0,NULL } ,
    {3,NULL,2,0.0,NULL } };

static AsnType atx[82] = {
  {401, "Seq-align" ,1,0,0,0,0,1,0,0,NULL,&atx[15],&atx[1],0,&atx[7]} ,
  {0, "type" ,128,0,0,0,0,0,0,0,NULL,&atx[2],&avnx[0],0,&atx[3]} ,
  {310, "ENUMERATED" ,0,10,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "dim" ,128,1,0,1,0,0,0,0,NULL,&atx[4],NULL,0,&atx[5]} ,
  {302, "INTEGER" ,0,2,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "score" ,128,2,0,1,0,0,0,0,NULL,&atx[16],&atx[6],0,&atx[17]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[7],NULL,0,NULL} ,
  {402, "Score" ,1,0,0,0,0,1,0,0,NULL,&atx[15],&atx[8],0,&atx[80]} ,
  {0, "id" ,128,0,0,1,0,0,0,0,NULL,&atx[9],NULL,0,&atx[10]} ,
  {408, "Object-id" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[20]} ,
  {0, "value" ,128,1,0,0,0,0,0,0,NULL,&atx[14],&atx[11],0,NULL} ,
  {0, "real" ,128,0,0,0,0,0,0,0,NULL,&atx[12],NULL,0,&atx[13]} ,
  {309, "REAL" ,0,9,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "int" ,128,1,0,0,0,0,0,0,NULL,&atx[4],NULL,0,NULL} ,
  {315, "CHOICE" ,0,-1,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {311, "SEQUENCE" ,0,16,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {314, "SET OF" ,0,17,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "segs" ,128,3,0,0,0,0,0,0,NULL,&atx[14],&atx[18],0,&atx[78]} ,
  {0, "dendiag" ,128,0,0,0,0,0,0,0,NULL,&atx[25],&atx[19],0,&atx[34]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[20],NULL,0,NULL} ,
  {409, "Dense-diag" ,1,0,0,0,0,0,0,0,NULL,&atx[15],&atx[21],0,&atx[35]} ,
  {0, "dim" ,128,0,0,0,1,0,0,0,&avnx[6],&atx[4],NULL,0,&atx[22]} ,
  {0, "ids" ,128,1,0,0,0,0,0,0,NULL,&atx[25],&atx[23],0,&atx[26]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[24],NULL,0,NULL} ,
  {405, "Seq-id" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[56]} ,
  {312, "SEQUENCE OF" ,0,16,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "starts" ,128,2,0,0,0,0,0,0,NULL,&atx[25],&atx[27],0,&atx[28]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[4],NULL,0,NULL} ,
  {0, "len" ,128,3,0,0,0,0,0,0,NULL,&atx[4],NULL,0,&atx[29]} ,
  {0, "strands" ,128,4,0,1,0,0,0,0,NULL,&atx[25],&atx[30],0,&atx[32]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[31],NULL,0,NULL} ,
  {407, "Na-strand" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[9]} ,
  {0, "scores" ,128,5,0,1,0,0,0,0,NULL,&atx[16],&atx[33],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[7],NULL,0,NULL} ,
  {0, "denseg" ,128,1,0,0,0,0,0,0,NULL,&atx[35],NULL,0,&atx[48]} ,
  {410, "Dense-seg" ,1,0,0,0,0,0,0,0,NULL,&atx[15],&atx[36],0,&atx[50]} ,
  {0, "dim" ,128,0,0,0,1,0,0,0,&avnx[7],&atx[4],NULL,0,&atx[37]} ,
  {0, "numseg" ,128,1,0,0,0,0,0,0,NULL,&atx[4],NULL,0,&atx[38]} ,
  {0, "ids" ,128,2,0,0,0,0,0,0,NULL,&atx[25],&atx[39],0,&atx[40]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[24],NULL,0,NULL} ,
  {0, "starts" ,128,3,0,0,0,0,0,0,NULL,&atx[25],&atx[41],0,&atx[42]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[4],NULL,0,NULL} ,
  {0, "lens" ,128,4,0,0,0,0,0,0,NULL,&atx[25],&atx[43],0,&atx[44]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[4],NULL,0,NULL} ,
  {0, "strands" ,128,5,0,1,0,0,0,0,NULL,&atx[25],&atx[45],0,&atx[46]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[31],NULL,0,NULL} ,
  {0, "scores" ,128,6,0,1,0,0,0,0,NULL,&atx[25],&atx[47],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[7],NULL,0,NULL} ,
  {0, "std" ,128,2,0,0,0,0,0,0,NULL,&atx[25],&atx[49],0,&atx[59]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[50],NULL,0,NULL} ,
  {411, "Std-seg" ,1,0,0,0,0,0,0,0,NULL,&atx[15],&atx[51],0,&atx[60]} ,
  {0, "dim" ,128,0,0,0,1,0,0,0,&avnx[8],&atx[4],NULL,0,&atx[52]} ,
  {0, "ids" ,128,1,0,1,0,0,0,0,NULL,&atx[25],&atx[53],0,&atx[54]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[24],NULL,0,NULL} ,
  {0, "loc" ,128,2,0,0,0,0,0,0,NULL,&atx[25],&atx[55],0,&atx[57]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[56],NULL,0,NULL} ,
  {406, "Seq-loc" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[31]} ,
  {0, "scores" ,128,3,0,1,0,0,0,0,NULL,&atx[16],&atx[58],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[7],NULL,0,NULL} ,
  {0, "packed" ,128,3,0,0,0,0,0,0,NULL,&atx[60],NULL,0,&atx[75]} ,
  {412, "Packed-seg" ,1,0,0,0,0,0,0,0,NULL,&atx[15],&atx[61],0,NULL} ,
  {0, "dim" ,128,0,0,0,1,0,0,0,&avnx[9],&atx[4],NULL,0,&atx[62]} ,
  {0, "numseg" ,128,1,0,0,0,0,0,0,NULL,&atx[4],NULL,0,&atx[63]} ,
  {0, "ids" ,128,2,0,0,0,0,0,0,NULL,&atx[25],&atx[64],0,&atx[65]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[24],NULL,0,NULL} ,
  {0, "starts" ,128,3,0,0,0,0,0,0,NULL,&atx[25],&atx[66],0,&atx[67]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[4],NULL,0,NULL} ,
  {0, "present" ,128,4,0,0,0,0,0,0,NULL,&atx[68],NULL,0,&atx[69]} ,
  {304, "OCTET STRING" ,0,4,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "lens" ,128,5,0,0,0,0,0,0,NULL,&atx[25],&atx[70],0,&atx[71]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[4],NULL,0,NULL} ,
  {0, "strands" ,128,6,0,1,0,0,0,0,NULL,&atx[25],&atx[72],0,&atx[73]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[31],NULL,0,NULL} ,
  {0, "scores" ,128,7,0,1,0,0,0,0,NULL,&atx[25],&atx[74],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[7],NULL,0,NULL} ,
  {0, "disc" ,128,4,0,0,0,0,0,0,NULL,&atx[76],NULL,0,NULL} ,
  {404, "Seq-align-set" ,1,0,0,0,0,1,0,0,NULL,&atx[16],&atx[77],0,&atx[24]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[0],NULL,0,NULL} ,
  {0, "bounds" ,128,4,0,1,0,0,0,0,NULL,&atx[16],&atx[79],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[56],NULL,0,NULL} ,
  {403, "Score-set" ,1,0,0,0,0,1,0,0,NULL,&atx[16],&atx[81],0,&atx[76]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[7],NULL,0,NULL} };

static AsnModule ampx[1] = {
  { "NCBI-Seqalign" , "asnalign.h60",&atx[0],NULL,NULL,0,0} };

static AsnValxNodePtr avn = avnx;
static AsnTypePtr at = atx;
static AsnModulePtr amp = ampx;



/**************************************************
*
*    Defines for Module NCBI-Seqalign
*
**************************************************/

#define SEQ_ALIGN &at[0]
#define SEQ_ALIGN_type &at[1]
#define SEQ_ALIGN_dim &at[3]
#define SEQ_ALIGN_score &at[5]
#define SEQ_ALIGN_score_E &at[6]
#define SEQ_ALIGN_segs &at[17]
#define SEQ_ALIGN_segs_dendiag &at[18]
#define SEQ_ALIGN_segs_dendiag_E &at[19]
#define SEQ_ALIGN_segs_denseg &at[34]
#define SEQ_ALIGN_segs_std &at[48]
#define SEQ_ALIGN_segs_std_E &at[49]
#define SEQ_ALIGN_segs_packed &at[59]
#define SEQ_ALIGN_segs_disc &at[75]
#define SEQ_ALIGN_bounds &at[78]
#define SEQ_ALIGN_bounds_E &at[79]

#define SCORE &at[7]
#define SCORE_id &at[8]
#define SCORE_value &at[10]
#define SCORE_value_real &at[11]
#define SCORE_value_int &at[13]

#define SCORE_SET &at[80]
#define SCORE_SET_E &at[81]

#define SEQ_ALIGN_SET &at[76]
#define SEQ_ALIGN_SET_E &at[77]

#define DENSE_DIAG &at[20]
#define DENSE_DIAG_dim &at[21]
#define DENSE_DIAG_ids &at[22]
#define DENSE_DIAG_ids_E &at[23]
#define DENSE_DIAG_starts &at[26]
#define DENSE_DIAG_starts_E &at[27]
#define DENSE_DIAG_len &at[28]
#define DENSE_DIAG_strands &at[29]
#define DENSE_DIAG_strands_E &at[30]
#define DENSE_DIAG_scores &at[32]
#define DENSE_DIAG_scores_E &at[33]

#define DENSE_SEG &at[35]
#define DENSE_SEG_dim &at[36]
#define DENSE_SEG_numseg &at[37]
#define DENSE_SEG_ids &at[38]
#define DENSE_SEG_ids_E &at[39]
#define DENSE_SEG_starts &at[40]
#define DENSE_SEG_starts_E &at[41]
#define DENSE_SEG_lens &at[42]
#define DENSE_SEG_lens_E &at[43]
#define DENSE_SEG_strands &at[44]
#define DENSE_SEG_strands_E &at[45]
#define DENSE_SEG_scores &at[46]
#define DENSE_SEG_scores_E &at[47]

#define STD_SEG &at[50]
#define STD_SEG_dim &at[51]
#define STD_SEG_ids &at[52]
#define STD_SEG_ids_E &at[53]
#define STD_SEG_loc &at[54]
#define STD_SEG_loc_E &at[55]
#define STD_SEG_scores &at[57]
#define STD_SEG_scores_E &at[58]

#define PACKED_SEG &at[60]
#define PACKED_SEG_dim &at[61]
#define PACKED_SEG_numseg &at[62]
#define PACKED_SEG_ids &at[63]
#define PACKED_SEG_ids_E &at[64]
#define PACKED_SEG_starts &at[65]
#define PACKED_SEG_starts_E &at[66]
#define PACKED_SEG_present &at[67]
#define PACKED_SEG_lens &at[69]
#define PACKED_SEG_lens_E &at[70]
#define PACKED_SEG_strands &at[71]
#define PACKED_SEG_strands_E &at[72]
#define PACKED_SEG_scores &at[73]
#define PACKED_SEG_scores_E &at[74]
