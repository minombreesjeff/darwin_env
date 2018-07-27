/***********************************************************************
*
**
*        Automatic header module from ASNTOOL
*
************************************************************************/

#ifndef _ASNTOOL_
#include <asn.h>
#endif

static char * asnfilename = "blstxml.h";
static AsnType atx[75] = {
  {401, "BlastOutput" ,1,0,0,0,0,0,0,0,NULL,&atx[24],&atx[1],0,&atx[12]} ,
  {0, "program" ,128,0,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[3]} ,
  {323, "VisibleString" ,0,26,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "version" ,128,1,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[4]} ,
  {0, "reference" ,128,2,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[5]} ,
  {0, "db" ,128,3,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[6]} ,
  {0, "query-ID" ,128,4,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[7]} ,
  {0, "query-def" ,128,5,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[8]} ,
  {0, "query-len" ,128,6,0,0,0,0,0,0,NULL,&atx[9],NULL,0,&atx[10]} ,
  {302, "INTEGER" ,0,2,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "query-seq" ,128,7,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[11]} ,
  {0, "param" ,128,8,0,0,0,0,0,0,NULL,&atx[12],NULL,0,&atx[25]} ,
  {402, "Parameters" ,1,0,0,0,0,0,0,0,NULL,&atx[24],&atx[13],0,&atx[27]} ,
  {0, "matrix" ,128,0,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[14]} ,
  {0, "expect" ,128,1,0,0,0,0,0,0,NULL,&atx[15],NULL,0,&atx[16]} ,
  {309, "REAL" ,0,9,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "include" ,128,2,0,1,0,0,0,0,NULL,&atx[15],NULL,0,&atx[17]} ,
  {0, "sc-match" ,128,3,0,1,0,0,0,0,NULL,&atx[9],NULL,0,&atx[18]} ,
  {0, "sc-mismatch" ,128,4,0,1,0,0,0,0,NULL,&atx[9],NULL,0,&atx[19]} ,
  {0, "gap-open" ,128,5,0,0,0,0,0,0,NULL,&atx[9],NULL,0,&atx[20]} ,
  {0, "gap-extend" ,128,6,0,0,0,0,0,0,NULL,&atx[9],NULL,0,&atx[21]} ,
  {0, "filter" ,128,7,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[22]} ,
  {0, "pattern" ,128,8,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[23]} ,
  {0, "entrez-query" ,128,9,0,1,0,0,0,0,NULL,&atx[2],NULL,0,NULL} ,
  {311, "SEQUENCE" ,0,16,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "iterations" ,128,9,0,0,0,0,0,0,NULL,&atx[63],&atx[26],0,&atx[74]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[27],NULL,0,NULL} ,
  {403, "Iteration" ,1,0,0,0,0,0,0,0,NULL,&atx[24],&atx[28],0,&atx[65]} ,
  {0, "iter-num" ,128,0,0,0,0,0,0,0,NULL,&atx[9],NULL,0,&atx[29]} ,
  {0, "query-ID" ,128,1,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[30]} ,
  {0, "query-def" ,128,2,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[31]} ,
  {0, "query-len" ,128,3,0,1,0,0,0,0,NULL,&atx[9],NULL,0,&atx[32]} ,
  {0, "hits" ,128,4,0,1,0,0,0,0,NULL,&atx[63],&atx[33],0,&atx[64]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[34],NULL,0,NULL} ,
  {405, "Hit" ,1,0,0,0,0,0,0,0,NULL,&atx[24],&atx[35],0,&atx[42]} ,
  {0, "num" ,128,0,0,0,0,0,0,0,NULL,&atx[9],NULL,0,&atx[36]} ,
  {0, "id" ,128,1,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[37]} ,
  {0, "def" ,128,2,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[38]} ,
  {0, "accession" ,128,3,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[39]} ,
  {0, "len" ,128,4,0,0,0,0,0,0,NULL,&atx[9],NULL,0,&atx[40]} ,
  {0, "hsps" ,128,5,0,1,0,0,0,0,NULL,&atx[63],&atx[41],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[42],NULL,0,NULL} ,
  {406, "Hsp" ,1,0,0,0,0,0,0,0,NULL,&atx[24],&atx[43],0,NULL} ,
  {0, "num" ,128,0,0,0,0,0,0,0,NULL,&atx[9],NULL,0,&atx[44]} ,
  {0, "bit-score" ,128,1,0,0,0,0,0,0,NULL,&atx[15],NULL,0,&atx[45]} ,
  {0, "score" ,128,2,0,0,0,0,0,0,NULL,&atx[15],NULL,0,&atx[46]} ,
  {0, "evalue" ,128,3,0,0,0,0,0,0,NULL,&atx[15],NULL,0,&atx[47]} ,
  {0, "query-from" ,128,4,0,0,0,0,0,0,NULL,&atx[9],NULL,0,&atx[48]} ,
  {0, "query-to" ,128,5,0,0,0,0,0,0,NULL,&atx[9],NULL,0,&atx[49]} ,
  {0, "hit-from" ,128,6,0,0,0,0,0,0,NULL,&atx[9],NULL,0,&atx[50]} ,
  {0, "hit-to" ,128,7,0,0,0,0,0,0,NULL,&atx[9],NULL,0,&atx[51]} ,
  {0, "pattern-from" ,128,8,0,1,0,0,0,0,NULL,&atx[9],NULL,0,&atx[52]} ,
  {0, "pattern-to" ,128,9,0,1,0,0,0,0,NULL,&atx[9],NULL,0,&atx[53]} ,
  {0, "query-frame" ,128,10,0,1,0,0,0,0,NULL,&atx[9],NULL,0,&atx[54]} ,
  {0, "hit-frame" ,128,11,0,1,0,0,0,0,NULL,&atx[9],NULL,0,&atx[55]} ,
  {0, "identity" ,128,12,0,1,0,0,0,0,NULL,&atx[9],NULL,0,&atx[56]} ,
  {0, "positive" ,128,13,0,1,0,0,0,0,NULL,&atx[9],NULL,0,&atx[57]} ,
  {0, "gaps" ,128,14,0,1,0,0,0,0,NULL,&atx[9],NULL,0,&atx[58]} ,
  {0, "align-len" ,128,15,0,1,0,0,0,0,NULL,&atx[9],NULL,0,&atx[59]} ,
  {0, "density" ,128,16,0,1,0,0,0,0,NULL,&atx[9],NULL,0,&atx[60]} ,
  {0, "qseq" ,128,17,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[61]} ,
  {0, "hseq" ,128,18,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[62]} ,
  {0, "midline" ,128,19,0,1,0,0,0,0,NULL,&atx[2],NULL,0,NULL} ,
  {312, "SEQUENCE OF" ,0,16,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "stat" ,128,5,0,1,0,0,0,0,NULL,&atx[65],NULL,0,&atx[73]} ,
  {404, "Statistics" ,1,0,0,0,0,0,0,0,NULL,&atx[24],&atx[66],0,&atx[34]} ,
  {0, "db-num" ,128,0,0,0,0,0,0,0,NULL,&atx[9],NULL,0,&atx[67]} ,
  {0, "db-len" ,128,1,0,0,0,0,0,0,NULL,&atx[9],NULL,0,&atx[68]} ,
  {0, "hsp-len" ,128,2,0,0,0,0,0,0,NULL,&atx[9],NULL,0,&atx[69]} ,
  {0, "eff-space" ,128,3,0,0,0,0,0,0,NULL,&atx[15],NULL,0,&atx[70]} ,
  {0, "kappa" ,128,4,0,0,0,0,0,0,NULL,&atx[15],NULL,0,&atx[71]} ,
  {0, "lambda" ,128,5,0,0,0,0,0,0,NULL,&atx[15],NULL,0,&atx[72]} ,
  {0, "entropy" ,128,6,0,0,0,0,0,0,NULL,&atx[15],NULL,0,NULL} ,
  {0, "message" ,128,6,0,1,0,0,0,0,NULL,&atx[2],NULL,0,NULL} ,
  {0, "mbstat" ,128,10,0,1,0,0,0,0,NULL,&atx[65],NULL,0,NULL} };

static AsnModule ampx[1] = {
  { "NCBI-BlastOutput" , "blstxml.h",&atx[0],NULL,NULL,0,0} };

static AsnValxNodePtr avn = NULL;
static AsnTypePtr at = atx;
static AsnModulePtr amp = ampx;



/**************************************************
*
*    Defines for Module NCBI-BlastOutput
*
**************************************************/

#define BLASTOUTPUT &at[0]
#define BLASTOUTPUT_program &at[1]
#define BLASTOUTPUT_version &at[3]
#define BLASTOUTPUT_reference &at[4]
#define BLASTOUTPUT_db &at[5]
#define BLASTOUTPUT_query_ID &at[6]
#define BLASTOUTPUT_query_def &at[7]
#define BLASTOUTPUT_query_len &at[8]
#define BLASTOUTPUT_query_seq &at[10]
#define BLASTOUTPUT_param &at[11]
#define BLASTOUTPUT_iterations &at[25]
#define BLASTOUTPUT_iterations_E &at[26]
#define BLASTOUTPUT_mbstat &at[74]

#define PARAMETERS &at[12]
#define PARAMETERS_matrix &at[13]
#define PARAMETERS_expect &at[14]
#define PARAMETERS_include &at[16]
#define PARAMETERS_sc_match &at[17]
#define PARAMETERS_sc_mismatch &at[18]
#define PARAMETERS_gap_open &at[19]
#define PARAMETERS_gap_extend &at[20]
#define PARAMETERS_filter &at[21]
#define PARAMETERS_pattern &at[22]
#define PARAMETERS_entrez_query &at[23]

#define ITERATION &at[27]
#define ITERATION_iter_num &at[28]
#define ITERATION_query_ID &at[29]
#define ITERATION_query_def &at[30]
#define ITERATION_query_len &at[31]
#define ITERATION_hits &at[32]
#define ITERATION_hits_E &at[33]
#define ITERATION_stat &at[64]
#define ITERATION_message &at[73]

#define STATISTICS &at[65]
#define STATISTICS_db_num &at[66]
#define STATISTICS_db_len &at[67]
#define STATISTICS_hsp_len &at[68]
#define STATISTICS_eff_space &at[69]
#define STATISTICS_kappa &at[70]
#define STATISTICS_lambda &at[71]
#define STATISTICS_entropy &at[72]

#define HIT &at[34]
#define HIT_num &at[35]
#define HIT_id &at[36]
#define HIT_def &at[37]
#define HIT_accession &at[38]
#define HIT_len &at[39]
#define HIT_hsps &at[40]
#define HIT_hsps_E &at[41]

#define HSP &at[42]
#define HSP_num &at[43]
#define HSP_bit_score &at[44]
#define HSP_score &at[45]
#define HSP_evalue &at[46]
#define HSP_query_from &at[47]
#define HSP_query_to &at[48]
#define HSP_hit_from &at[49]
#define HSP_hit_to &at[50]
#define HSP_pattern_from &at[51]
#define HSP_pattern_to &at[52]
#define HSP_query_frame &at[53]
#define HSP_hit_frame &at[54]
#define HSP_identity &at[55]
#define HSP_positive &at[56]
#define HSP_gaps &at[57]
#define HSP_align_len &at[58]
#define HSP_density &at[59]
#define HSP_qseq &at[60]
#define HSP_hseq &at[61]
#define HSP_midline &at[62]
