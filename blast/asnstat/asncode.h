/***********************************************************************
*
**
*        Automatic header module from ASNTOOL
*
************************************************************************/

#ifndef _ASNTOOL_
#include <asn.h>
#endif

static char * asnfilename = "asncode.h60";
static AsnValxNode avnx[13] = {
    {20,"iupacna" ,1,0.0,&avnx[1] } ,
    {20,"iupacaa" ,2,0.0,&avnx[2] } ,
    {20,"ncbi2na" ,3,0.0,&avnx[3] } ,
    {20,"ncbi4na" ,4,0.0,&avnx[4] } ,
    {20,"ncbi8na" ,5,0.0,&avnx[5] } ,
    {20,"ncbipna" ,6,0.0,&avnx[6] } ,
    {20,"ncbi8aa" ,7,0.0,&avnx[7] } ,
    {20,"ncbieaa" ,8,0.0,&avnx[8] } ,
    {20,"ncbipaa" ,9,0.0,&avnx[9] } ,
    {20,"iupacaa3" ,10,0.0,&avnx[10] } ,
    {20,"ncbistdaa" ,11,0.0,NULL } ,
    {3,NULL,0,0.0,NULL } ,
    {3,NULL,0,0.0,NULL } };

static AsnType atx[31] = {
  {401, "Seq-code-table" ,1,0,0,0,0,1,0,0,NULL,&atx[14],&atx[1],0,&atx[18]} ,
  {0, "code" ,128,0,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[4]} ,
  {404, "Seq-code-type" ,1,0,0,0,0,0,0,0,NULL,&atx[3],&avnx[0],0,NULL} ,
  {310, "ENUMERATED" ,0,10,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "num" ,128,1,0,0,0,0,0,0,NULL,&atx[5],NULL,0,&atx[6]} ,
  {302, "INTEGER" ,0,2,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "one-letter" ,128,2,0,0,0,0,0,0,NULL,&atx[7],NULL,0,&atx[8]} ,
  {301, "BOOLEAN" ,0,1,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "start-at" ,128,3,0,0,1,0,0,0,&avnx[11],&atx[5],NULL,0,&atx[9]} ,
  {0, "table" ,128,4,0,0,0,0,0,0,NULL,&atx[15],&atx[10],0,&atx[16]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[14],&atx[11],0,NULL} ,
  {0, "symbol" ,128,0,0,0,0,0,0,0,NULL,&atx[12],NULL,0,&atx[13]} ,
  {323, "VisibleString" ,0,26,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "name" ,128,1,0,0,0,0,0,0,NULL,&atx[12],NULL,0,NULL} ,
  {311, "SEQUENCE" ,0,16,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {312, "SEQUENCE OF" ,0,16,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "comps" ,128,5,0,1,0,0,0,0,NULL,&atx[15],&atx[17],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[5],NULL,0,NULL} ,
  {402, "Seq-map-table" ,1,0,0,0,0,1,0,0,NULL,&atx[14],&atx[19],0,&atx[25]} ,
  {0, "from" ,128,0,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[20]} ,
  {0, "to" ,128,1,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[21]} ,
  {0, "num" ,128,2,0,0,0,0,0,0,NULL,&atx[5],NULL,0,&atx[22]} ,
  {0, "start-at" ,128,3,0,0,1,0,0,0,&avnx[12],&atx[5],NULL,0,&atx[23]} ,
  {0, "table" ,128,4,0,0,0,0,0,0,NULL,&atx[15],&atx[24],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[5],NULL,0,NULL} ,
  {403, "Seq-code-set" ,1,0,0,0,0,1,0,0,NULL,&atx[14],&atx[26],0,&atx[2]} ,
  {0, "codes" ,128,0,0,1,0,0,0,0,NULL,&atx[28],&atx[27],0,&atx[29]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[0],NULL,0,NULL} ,
  {314, "SET OF" ,0,17,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "maps" ,128,1,0,1,0,0,0,0,NULL,&atx[28],&atx[30],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[18],NULL,0,NULL} };

static AsnModule ampx[1] = {
  { "NCBI-SeqCode" , "asncode.h60",&atx[0],NULL,NULL,0,0} };

static AsnValxNodePtr avn = avnx;
static AsnTypePtr at = atx;
static AsnModulePtr amp = ampx;



/**************************************************
*
*    Defines for Module NCBI-SeqCode
*
**************************************************/

#define SEQ_CODE_TABLE &at[0]
#define SEQ_CODE_TABLE_code &at[1]
#define SEQ_CODE_TABLE_num &at[4]
#define SEQ_CODE_TABLE_one_letter &at[6]
#define SEQ_CODE_TABLE_start_at &at[8]
#define SEQ_CODE_TABLE_table &at[9]
#define SEQ_CODE_TABLE_table_E &at[10]
#define SEQ_CODE_TABLE_table_E_symbol &at[11]
#define SEQ_CODE_TABLE_table_E_name &at[13]
#define SEQ_CODE_TABLE_comps &at[16]
#define SEQ_CODE_TABLE_comps_E &at[17]

#define SEQ_MAP_TABLE &at[18]
#define SEQ_MAP_TABLE_from &at[19]
#define SEQ_MAP_TABLE_to &at[20]
#define SEQ_MAP_TABLE_num &at[21]
#define SEQ_MAP_TABLE_start_at &at[22]
#define SEQ_MAP_TABLE_table &at[23]
#define SEQ_MAP_TABLE_table_E &at[24]

#define SEQ_CODE_SET &at[25]
#define SEQ_CODE_SET_codes &at[26]
#define SEQ_CODE_SET_codes_E &at[27]
#define SEQ_CODE_SET_maps &at[29]
#define SEQ_CODE_SET_maps_E &at[30]

#define SEQ_CODE_TYPE &at[2]
