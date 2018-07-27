/***********************************************************************
*
**
*        Automatic header module from ASNTOOL
*
************************************************************************/

#ifndef _ASNTOOL_
#include <asn.h>
#endif

static char * asnfilename = "sugmap.h60";
static AsnValxNode avnx[5] = {
    {20,"none" ,0,0.0,&avnx[1] } ,
    {20,"info" ,1,0.0,&avnx[2] } ,
    {20,"warn" ,2,0.0,&avnx[3] } ,
    {20,"error" ,3,0.0,&avnx[4] } ,
    {20,"fatal" ,4,0.0,NULL } };

static AsnType atx[31] = {
  {401, "Seq-entry" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[1]} ,
  {402, "Seq-annot" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[2]} ,
  {403, "Suggest-request" ,1,0,0,0,0,0,0,0,NULL,&atx[20],&atx[3],0,&atx[6]} ,
  {0, "init" ,128,0,0,0,0,0,0,0,NULL,&atx[4],NULL,0,&atx[5]} ,
  {305, "NULL" ,0,5,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "intervals" ,128,1,0,0,0,0,0,0,NULL,&atx[6],NULL,0,&atx[19]} ,
  {404, "Suggest-intervals" ,1,0,0,0,0,0,0,0,NULL,&atx[15],&atx[7],0,&atx[8]} ,
  {0, "params" ,128,0,0,1,0,0,0,0,NULL,&atx[8],NULL,0,&atx[16]} ,
  {405, "Suggest-parameters" ,1,0,0,0,0,0,0,0,NULL,&atx[15],&atx[9],0,&atx[21]} ,
  {0, "size" ,128,0,0,1,0,0,0,0,NULL,&atx[10],NULL,0,&atx[11]} ,
  {302, "INTEGER" ,0,2,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "begin-search" ,128,1,0,1,0,0,0,0,NULL,&atx[10],NULL,0,&atx[12]} ,
  {0, "end-search" ,128,2,0,1,0,0,0,0,NULL,&atx[10],NULL,0,&atx[13]} ,
  {0, "term-stop" ,128,3,0,1,0,0,0,0,NULL,&atx[14],NULL,0,NULL} ,
  {301, "BOOLEAN" ,0,1,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {311, "SEQUENCE" ,0,16,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "dna" ,128,1,0,0,0,0,0,0,NULL,&atx[0],NULL,0,&atx[17]} ,
  {0, "protein" ,128,2,0,0,0,0,0,0,NULL,&atx[0],NULL,0,&atx[18]} ,
  {0, "code" ,128,3,0,0,0,0,0,0,NULL,&atx[10],NULL,0,NULL} ,
  {0, "fini" ,128,2,0,0,0,0,0,0,NULL,&atx[4],NULL,0,NULL} ,
  {315, "CHOICE" ,0,-1,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {406, "Suggest-error" ,1,0,0,0,0,0,0,0,NULL,&atx[15],&atx[22],0,&atx[26]} ,
  {0, "level" ,128,0,0,0,0,0,0,0,NULL,&atx[23],&avnx[0],0,&atx[24]} ,
  {310, "ENUMERATED" ,0,10,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "msg" ,128,1,0,1,0,0,0,0,NULL,&atx[25],NULL,0,NULL} ,
  {323, "VisibleString" ,0,26,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {407, "Suggest-response" ,1,0,0,0,0,0,0,0,NULL,&atx[20],&atx[27],0,NULL} ,
  {0, "init" ,128,0,0,0,0,0,0,0,NULL,&atx[4],NULL,0,&atx[28]} ,
  {0, "error" ,128,1,0,0,0,0,0,0,NULL,&atx[21],NULL,0,&atx[29]} ,
  {0, "intervals" ,128,2,0,0,0,0,0,0,NULL,&atx[1],NULL,0,&atx[30]} ,
  {0, "fini" ,128,3,0,0,0,0,0,0,NULL,&atx[4],NULL,0,NULL} };

static AsnModule ampx[1] = {
  { "NCBI-Suggest" , "sugmap.h60",&atx[0],NULL,NULL,0,0} };

static AsnValxNodePtr avn = avnx;
static AsnTypePtr at = atx;
static AsnModulePtr amp = ampx;



/**************************************************
*
*    Defines for Module NCBI-Suggest
*
**************************************************/

#define SUGGEST_REQUEST &at[2]
#define SUGGEST_REQUEST_init &at[3]
#define SUGGEST_REQUEST_intervals &at[5]
#define SUGGEST_REQUEST_fini &at[19]

#define SUGGEST_INTERVALS &at[6]
#define SUGGEST_INTERVALS_params &at[7]
#define SUGGEST_INTERVALS_dna &at[16]
#define SUGGEST_INTERVALS_protein &at[17]
#define SUGGEST_INTERVALS_code &at[18]

#define SUGGEST_PARAMETERS &at[8]
#define SUGGEST_PARAMETERS_size &at[9]
#define SUGGEST_PARAMETERS_begin_search &at[11]
#define SUGGEST_PARAMETERS_end_search &at[12]
#define SUGGEST_PARAMETERS_term_stop &at[13]

#define SUGGEST_ERROR &at[21]
#define SUGGEST_ERROR_level &at[22]
#define SUGGEST_ERROR_msg &at[24]

#define SUGGEST_RESPONSE &at[26]
#define SUGGEST_RESPONSE_init &at[27]
#define SUGGEST_RESPONSE_error &at[28]
#define SUGGEST_RESPONSE_intervals &at[29]
#define SUGGEST_RESPONSE_fini &at[30]
