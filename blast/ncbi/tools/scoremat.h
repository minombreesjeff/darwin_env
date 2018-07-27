/***********************************************************************
*
**
*        Automatic header module from ASNTOOL
*
************************************************************************/

#ifndef _ASNTOOL_
#include <asn.h>
#endif

static char * asnfilename = "scoremat.h";
static AsnValxNode avnx[21] = {
    {20,"unassigned" ,0,0.0,&avnx[1] } ,
    {20,"blosum62" ,1,0.0,&avnx[2] } ,
    {20,"blosum45" ,2,0.0,&avnx[3] } ,
    {20,"blosum80" ,3,0.0,&avnx[4] } ,
    {20,"blosum50" ,4,0.0,&avnx[5] } ,
    {20,"blosum90" ,5,0.0,&avnx[6] } ,
    {20,"pam30" ,6,0.0,&avnx[7] } ,
    {20,"pam70" ,7,0.0,&avnx[8] } ,
    {20,"pam250" ,8,0.0,&avnx[9] } ,
    {20,"other" ,255,0.0,NULL } ,
    {20,"unassigned" ,0,0.0,&avnx[11] } ,
    {20,"threshold" ,1,0.0,&avnx[12] } ,
    {20,"minscore" ,2,0.0,&avnx[13] } ,
    {20,"maxscore" ,3,0.0,&avnx[14] } ,
    {20,"meanscore" ,4,0.0,&avnx[15] } ,
    {20,"variance" ,5,0.0,&avnx[16] } ,
    {20,"name" ,10,0.0,&avnx[17] } ,
    {20,"is-optional" ,20,0.0,&avnx[18] } ,
    {20,"other" ,255,0.0,NULL } ,
    {3,NULL,0,0.0,NULL } ,
    {3,NULL,100000,0.0,NULL } };

static AsnType atx[61] = {
  {401, "Score-matrix" ,1,0,0,0,0,1,0,0,NULL,&atx[33],&atx[1],0,&atx[34]} ,
  {0, "is-protein" ,128,0,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[3]} ,
  {301, "BOOLEAN" ,0,1,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "identifier" ,128,1,0,0,0,0,0,0,NULL,&atx[4],NULL,0,&atx[5]} ,
  {403, "Object-id" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[25]} ,
  {0, "comments" ,128,2,0,1,0,0,0,0,NULL,&atx[8],&atx[6],0,&atx[9]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[7],NULL,0,NULL} ,
  {323, "VisibleString" ,0,26,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {312, "SEQUENCE OF" ,0,16,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "nrows" ,128,3,0,0,0,0,0,0,NULL,&atx[10],NULL,0,&atx[11]} ,
  {302, "INTEGER" ,0,2,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "ncolumns" ,128,4,0,0,0,0,0,0,NULL,&atx[10],NULL,0,&atx[12]} ,
  {0, "row-labels" ,128,5,0,1,0,0,0,0,NULL,&atx[8],&atx[13],0,&atx[14]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[7],NULL,0,NULL} ,
  {0, "scores" ,128,6,0,1,0,0,0,0,NULL,&atx[8],&atx[15],0,&atx[16]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[10],NULL,0,NULL} ,
  {0, "score-scale-factor" ,128,7,0,1,0,0,0,0,NULL,&atx[10],NULL,0,&atx[17]} ,
  {0, "posFreqs" ,128,8,0,1,0,0,0,0,NULL,&atx[8],&atx[18],0,&atx[19]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[10],NULL,0,NULL} ,
  {0, "posFreqs-scale-factor" ,128,9,0,1,0,0,0,0,NULL,&atx[10],NULL,0,&atx[20]} ,
  {0, "pseudocounts" ,128,10,0,1,0,0,0,0,NULL,&atx[10],NULL,0,&atx[21]} ,
  {0, "freq-Ratios" ,128,11,0,1,0,0,0,0,NULL,&atx[10],&avnx[0],0,&atx[22]} ,
  {0, "gapOpen" ,128,12,0,1,0,0,0,0,NULL,&atx[10],NULL,0,&atx[23]} ,
  {0, "gapExtend" ,128,13,0,1,0,0,0,0,NULL,&atx[10],NULL,0,&atx[24]} ,
  {0, "query" ,128,14,0,1,0,0,0,0,NULL,&atx[25],NULL,0,&atx[26]} ,
  {404, "Seq-entry" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[52]} ,
  {0, "rawFreqs" ,128,15,0,1,0,0,0,0,NULL,&atx[8],&atx[27],0,&atx[28]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[10],NULL,0,NULL} ,
  {0, "rawFreqs-scale-factor" ,128,16,0,1,0,0,0,0,NULL,&atx[10],NULL,0,&atx[29]} ,
  {0, "weights" ,128,17,0,1,0,0,0,0,NULL,&atx[8],&atx[30],0,&atx[31]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[10],NULL,0,NULL} ,
  {0, "weights-scale-factor" ,128,18,0,1,0,0,0,0,NULL,&atx[10],NULL,0,&atx[32]} ,
  {0, "byrow" ,128,19,0,1,0,0,0,0,NULL,&atx[2],NULL,0,NULL} ,
  {311, "SEQUENCE" ,0,16,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {402, "Score-matrix-parameters" ,1,0,0,0,0,1,0,0,NULL,&atx[33],&atx[35],0,&atx[4]} ,
  {0, "matrix" ,128,0,0,0,0,0,0,0,NULL,&atx[0],NULL,0,&atx[36]} ,
  {0, "lambda" ,128,1,0,1,0,0,0,0,NULL,&atx[37],NULL,0,&atx[38]} ,
  {309, "REAL" ,0,9,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "kappa" ,128,2,0,1,0,0,0,0,NULL,&atx[37],NULL,0,&atx[39]} ,
  {0, "h" ,128,3,0,1,0,0,0,0,NULL,&atx[37],NULL,0,&atx[40]} ,
  {0, "constraints" ,128,4,0,1,0,0,0,0,NULL,&atx[41],NULL,0,NULL} ,
  {408, "CoreDef" ,1,0,0,0,0,0,0,0,NULL,&atx[33],&atx[42],0,NULL} ,
  {0, "nblocks" ,128,0,0,0,0,0,0,0,NULL,&atx[10],NULL,0,&atx[43]} ,
  {0, "blocks" ,128,1,0,0,0,0,0,0,NULL,&atx[8],&atx[44],0,&atx[56]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[45],NULL,0,NULL} ,
  {406, "CoreBlock" ,1,0,0,0,0,0,0,0,NULL,&atx[33],&atx[46],0,&atx[58]} ,
  {0, "start" ,128,0,0,0,0,0,0,0,NULL,&atx[10],NULL,0,&atx[47]} ,
  {0, "stop" ,128,1,0,0,0,0,0,0,NULL,&atx[10],NULL,0,&atx[48]} ,
  {0, "minstart" ,128,2,0,1,0,0,0,0,NULL,&atx[10],NULL,0,&atx[49]} ,
  {0, "maxstop" ,128,3,0,1,0,0,0,0,NULL,&atx[10],NULL,0,&atx[50]} ,
  {0, "property" ,128,4,0,1,0,0,0,0,NULL,&atx[8],&atx[51],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[52],NULL,0,NULL} ,
  {405, "BlockProperty" ,1,0,0,0,0,0,0,0,NULL,&atx[33],&atx[53],0,&atx[45]} ,
  {0, "type" ,128,0,0,0,0,0,0,0,NULL,&atx[10],&avnx[10],0,&atx[54]} ,
  {0, "intvalue" ,128,1,0,1,0,0,0,0,NULL,&atx[10],NULL,0,&atx[55]} ,
  {0, "textvalue" ,128,2,0,1,0,0,0,0,NULL,&atx[7],NULL,0,NULL} ,
  {0, "loops" ,128,2,0,0,0,0,0,0,NULL,&atx[8],&atx[57],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[58],NULL,0,NULL} ,
  {407, "LoopConstraint" ,1,0,0,0,0,0,0,0,NULL,&atx[33],&atx[59],0,&atx[41]} ,
  {0, "minlength" ,128,0,0,0,1,0,0,0,&avnx[19],&atx[10],NULL,0,&atx[60]} ,
  {0, "maxlength" ,128,1,0,0,1,0,0,0,&avnx[20],&atx[10],NULL,0,NULL} };

static AsnModule ampx[1] = {
  { "NCBI-ScoreMat" , "scoremat.h",&atx[0],NULL,NULL,0,0} };

static AsnValxNodePtr avn = avnx;
static AsnTypePtr at = atx;
static AsnModulePtr amp = ampx;



/**************************************************
*
*    Defines for Module NCBI-ScoreMat
*
**************************************************/

#define SCORE_MATRIX &at[0]
#define SCORE_MATRIX_is_protein &at[1]
#define SCORE_MATRIX_identifier &at[3]
#define SCORE_MATRIX_comments &at[5]
#define SCORE_MATRIX_comments_E &at[6]
#define SCORE_MATRIX_nrows &at[9]
#define SCORE_MATRIX_ncolumns &at[11]
#define SCORE_MATRIX_row_labels &at[12]
#define SCORE_MATRIX_row_labels_E &at[13]
#define SCORE_MATRIX_scores &at[14]
#define SCORE_MATRIX_scores_E &at[15]
#define SCORE_MATRIX_score_scale_factor &at[16]
#define SCORE_MATRIX_posFreqs &at[17]
#define SCORE_MATRIX_posFreqs_E &at[18]
#define SCORE_MATRIX_posFreqs_scale_factor &at[19]
#define SCORE_MATRIX_pseudocounts &at[20]
#define SCORE_MATRIX_freq_Ratios &at[21]
#define SCORE_MATRIX_gapOpen &at[22]
#define SCORE_MATRIX_gapExtend &at[23]
#define SCORE_MATRIX_query &at[24]
#define SCORE_MATRIX_rawFreqs &at[26]
#define SCORE_MATRIX_rawFreqs_E &at[27]
#define SCORE_MATRIX_rawFreqs_scale_factor &at[28]
#define SCORE_MATRIX_weights &at[29]
#define SCORE_MATRIX_weights_E &at[30]
#define SCORE_MATRIX_weights_scale_factor &at[31]
#define SCORE_MATRIX_byrow &at[32]

#define SCORE_MATRIX_PARAMETERS &at[34]
#define SCORE_MATRIX_PARAMETERS_matrix &at[35]
#define SCORE_MATRIX_PARAMETERS_lambda &at[36]
#define SCORE_MATRIX_PARAMETERS_kappa &at[38]
#define SCORE_MATRIX_PARAMETERS_h &at[39]
#define SCORE_MATRIX_PARAMETERS_constraints &at[40]

#define BLOCKPROPERTY &at[52]
#define BLOCKPROPERTY_type &at[53]
#define BLOCKPROPERTY_intvalue &at[54]
#define BLOCKPROPERTY_textvalue &at[55]

#define COREBLOCK &at[45]
#define COREBLOCK_start &at[46]
#define COREBLOCK_stop &at[47]
#define COREBLOCK_minstart &at[48]
#define COREBLOCK_maxstop &at[49]
#define COREBLOCK_property &at[50]
#define COREBLOCK_property_E &at[51]

#define LOOPCONSTRAINT &at[58]
#define LOOPCONSTRAINT_minlength &at[59]
#define LOOPCONSTRAINT_maxlength &at[60]

#define COREDEF &at[41]
#define COREDEF_nblocks &at[42]
#define COREDEF_blocks &at[43]
#define COREDEF_blocks_E &at[44]
#define COREDEF_loops &at[56]
#define COREDEF_loops_E &at[57]
