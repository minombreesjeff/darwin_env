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
static AsnValxNode avnx[14] = {
    {2,NULL,1,0.0,NULL } ,
    {2,NULL,0,0.0,NULL } ,
    {3,NULL,1,0.0,NULL } ,
    {20,"unassigned" ,0,0.0,&avnx[4] } ,
    {20,"threshold" ,1,0.0,&avnx[5] } ,
    {20,"minscore" ,2,0.0,&avnx[6] } ,
    {20,"maxscore" ,3,0.0,&avnx[7] } ,
    {20,"meanscore" ,4,0.0,&avnx[8] } ,
    {20,"variance" ,5,0.0,&avnx[9] } ,
    {20,"name" ,10,0.0,&avnx[10] } ,
    {20,"is-optional" ,20,0.0,&avnx[11] } ,
    {20,"other" ,255,0.0,NULL } ,
    {3,NULL,0,0.0,NULL } ,
    {3,NULL,100000,0.0,NULL } };

static AsnType atx[64] = {
  {401, "Pssm" ,1,0,0,0,0,1,0,0,NULL,&atx[24],&atx[1],0,&atx[16]} ,
  {0, "isProtein" ,128,0,0,0,1,0,0,0,&avnx[0],&atx[2],NULL,0,&atx[3]} ,
  {301, "BOOLEAN" ,0,1,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "identifier" ,128,1,0,1,0,0,0,0,NULL,&atx[4],NULL,0,&atx[5]} ,
  {406, "Object-id" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[14]} ,
  {0, "numRows" ,128,2,0,0,0,0,0,0,NULL,&atx[6],NULL,0,&atx[7]} ,
  {302, "INTEGER" ,0,2,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "numColumns" ,128,3,0,0,0,0,0,0,NULL,&atx[6],NULL,0,&atx[8]} ,
  {0, "rowLabels" ,128,4,0,1,0,0,0,0,NULL,&atx[11],&atx[9],0,&atx[12]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[10],NULL,0,NULL} ,
  {323, "VisibleString" ,0,26,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {312, "SEQUENCE OF" ,0,16,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "byRow" ,128,5,0,0,1,0,0,0,&avnx[1],&atx[2],NULL,0,&atx[13]} ,
  {0, "query" ,128,6,0,1,0,0,0,0,NULL,&atx[14],NULL,0,&atx[15]} ,
  {407, "Seq-entry" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[52]} ,
  {0, "intermediateData" ,128,7,0,1,0,0,0,0,NULL,&atx[16],NULL,0,&atx[25]} ,
  {402, "PssmIntermediateData" ,1,0,0,0,0,1,0,0,NULL,&atx[24],&atx[17],0,&atx[26]} ,
  {0, "resFreqsPerPos" ,128,0,0,1,0,0,0,0,NULL,&atx[11],&atx[18],0,&atx[19]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[6],NULL,0,NULL} ,
  {0, "weightedResFreqsPerPos" ,128,1,0,1,0,0,0,0,NULL,&atx[11],&atx[20],0,&atx[22]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[21],NULL,0,NULL} ,
  {309, "REAL" ,0,9,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "freqRatios" ,128,2,0,0,0,0,0,0,NULL,&atx[11],&atx[23],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[21],NULL,0,NULL} ,
  {311, "SEQUENCE" ,0,16,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "finalData" ,128,8,0,1,0,0,0,0,NULL,&atx[26],NULL,0,NULL} ,
  {403, "PssmFinalData" ,1,0,0,0,0,1,0,0,NULL,&atx[24],&atx[27],0,&atx[33]} ,
  {0, "scores" ,128,0,0,0,0,0,0,0,NULL,&atx[11],&atx[28],0,&atx[29]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[6],NULL,0,NULL} ,
  {0, "lambda" ,128,1,0,0,0,0,0,0,NULL,&atx[21],NULL,0,&atx[30]} ,
  {0, "kappa" ,128,2,0,0,0,0,0,0,NULL,&atx[21],NULL,0,&atx[31]} ,
  {0, "h" ,128,3,0,0,0,0,0,0,NULL,&atx[21],NULL,0,&atx[32]} ,
  {0, "scalingFactor" ,128,4,0,0,1,0,0,0,&avnx[2],&atx[6],NULL,0,NULL} ,
  {404, "PssmParameters" ,1,0,0,0,0,1,0,0,NULL,&atx[24],&atx[34],0,&atx[61]} ,
  {0, "pseudocount" ,128,0,0,1,0,0,0,0,NULL,&atx[6],NULL,0,&atx[35]} ,
  {0, "rpsdbparams" ,128,1,0,1,0,0,0,0,NULL,&atx[36],NULL,0,&atx[40]} ,
  {412, "FormatRpsDbParameters" ,1,0,0,0,0,0,0,0,NULL,&atx[24],&atx[37],0,NULL} ,
  {0, "matrixName" ,128,0,0,0,0,0,0,0,NULL,&atx[10],NULL,0,&atx[38]} ,
  {0, "gapOpen" ,128,1,0,1,0,0,0,0,NULL,&atx[6],NULL,0,&atx[39]} ,
  {0, "gapExtend" ,128,2,0,1,0,0,0,0,NULL,&atx[6],NULL,0,NULL} ,
  {0, "constraints" ,128,2,0,1,0,0,0,0,NULL,&atx[41],NULL,0,NULL} ,
  {411, "CoreDef" ,1,0,0,0,0,0,0,0,NULL,&atx[24],&atx[42],0,&atx[36]} ,
  {0, "nblocks" ,128,0,0,0,0,0,0,0,NULL,&atx[6],NULL,0,&atx[43]} ,
  {0, "blocks" ,128,1,0,0,0,0,0,0,NULL,&atx[11],&atx[44],0,&atx[56]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[45],NULL,0,NULL} ,
  {409, "CoreBlock" ,1,0,0,0,0,0,0,0,NULL,&atx[24],&atx[46],0,&atx[58]} ,
  {0, "start" ,128,0,0,0,0,0,0,0,NULL,&atx[6],NULL,0,&atx[47]} ,
  {0, "stop" ,128,1,0,0,0,0,0,0,NULL,&atx[6],NULL,0,&atx[48]} ,
  {0, "minstart" ,128,2,0,1,0,0,0,0,NULL,&atx[6],NULL,0,&atx[49]} ,
  {0, "maxstop" ,128,3,0,1,0,0,0,0,NULL,&atx[6],NULL,0,&atx[50]} ,
  {0, "property" ,128,4,0,1,0,0,0,0,NULL,&atx[11],&atx[51],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[52],NULL,0,NULL} ,
  {408, "BlockProperty" ,1,0,0,0,0,0,0,0,NULL,&atx[24],&atx[53],0,&atx[45]} ,
  {0, "type" ,128,0,0,0,0,0,0,0,NULL,&atx[6],&avnx[3],0,&atx[54]} ,
  {0, "intvalue" ,128,1,0,1,0,0,0,0,NULL,&atx[6],NULL,0,&atx[55]} ,
  {0, "textvalue" ,128,2,0,1,0,0,0,0,NULL,&atx[10],NULL,0,NULL} ,
  {0, "loops" ,128,2,0,0,0,0,0,0,NULL,&atx[11],&atx[57],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[58],NULL,0,NULL} ,
  {410, "LoopConstraint" ,1,0,0,0,0,0,0,0,NULL,&atx[24],&atx[59],0,&atx[41]} ,
  {0, "minlength" ,128,0,0,0,1,0,0,0,&avnx[12],&atx[6],NULL,0,&atx[60]} ,
  {0, "maxlength" ,128,1,0,0,1,0,0,0,&avnx[13],&atx[6],NULL,0,NULL} ,
  {405, "PssmWithParameters" ,1,0,0,0,0,1,0,0,NULL,&atx[24],&atx[62],0,&atx[4]} ,
  {0, "pssm" ,128,0,0,0,0,0,0,0,NULL,&atx[0],NULL,0,&atx[63]} ,
  {0, "params" ,128,1,0,1,0,0,0,0,NULL,&atx[33],NULL,0,NULL} };

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

#define PSSM &at[0]
#define PSSM_isProtein &at[1]
#define PSSM_identifier &at[3]
#define PSSM_numRows &at[5]
#define PSSM_numColumns &at[7]
#define PSSM_rowLabels &at[8]
#define PSSM_rowLabels_E &at[9]
#define PSSM_byRow &at[12]
#define PSSM_query &at[13]
#define PSSM_intermediateData &at[15]
#define PSSM_finalData &at[25]

#define PSSMINTERMEDIATEDATA &at[16]
#define PSSMINTERMEDIATEDATA_resFreqsPerPos &at[17]
#define PSSMINTERMEDIATEDATA_resFreqsPerPos_E &at[18]
#define PSSMINTERMEDIATEDATA_weightedResFreqsPerPos &at[19]
#define PSSMINTERMEDIATEDATA_weightedResFreqsPerPos_E &at[20]
#define PSSMINTERMEDIATEDATA_freqRatios &at[22]
#define PSSMINTERMEDIATEDATA_freqRatios_E &at[23]

#define PSSMFINALDATA &at[26]
#define PSSMFINALDATA_scores &at[27]
#define PSSMFINALDATA_scores_E &at[28]
#define PSSMFINALDATA_lambda &at[29]
#define PSSMFINALDATA_kappa &at[30]
#define PSSMFINALDATA_h &at[31]
#define PSSMFINALDATA_scalingFactor &at[32]

#define PSSMPARAMETERS &at[33]
#define PSSMPARAMETERS_pseudocount &at[34]
#define PSSMPARAMETERS_rpsdbparams &at[35]
#define PSSMPARAMETERS_constraints &at[40]

#define PSSMWITHPARAMETERS &at[61]
#define PSSMWITHPARAMETERS_pssm &at[62]
#define PSSMWITHPARAMETERS_params &at[63]

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

#define FORMATRPSDBPARAMETERS &at[36]
#define FORMATRPSDBPARAMETERS_matrixName &at[37]
#define FORMATRPSDBPARAMETERS_gapOpen &at[38]
#define FORMATRPSDBPARAMETERS_gapExtend &at[39]
