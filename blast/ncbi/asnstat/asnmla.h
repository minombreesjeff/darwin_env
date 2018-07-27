/***********************************************************************
*
**
*        Automatic header module from ASNTOOL
*
************************************************************************/

#ifndef _ASNTOOL_
#include <asn.h>
#endif

static char * asnfilename = "asnmla.h61";
static AsnValxNode avnx[20] = {
    {20,"not-set" ,0,0.0,&avnx[1] } ,
    {20,"name" ,1,0.0,&avnx[2] } ,
    {20,"tsub" ,2,0.0,&avnx[3] } ,
    {20,"trans" ,3,0.0,&avnx[4] } ,
    {20,"jta" ,4,0.0,&avnx[5] } ,
    {20,"iso-jta" ,5,0.0,&avnx[6] } ,
    {20,"ml-jta" ,6,0.0,&avnx[7] } ,
    {20,"coden" ,7,0.0,&avnx[8] } ,
    {20,"issn" ,8,0.0,&avnx[9] } ,
    {20,"abr" ,9,0.0,&avnx[10] } ,
    {20,"isbn" ,10,0.0,&avnx[11] } ,
    {20,"all" ,255,0.0,NULL } ,
    {20,"not-found" ,0,0.0,&avnx[13] } ,
    {20,"operational-error" ,1,0.0,&avnx[14] } ,
    {20,"cannot-connect-jrsrv" ,2,0.0,&avnx[15] } ,
    {20,"cannot-connect-pmdb" ,3,0.0,&avnx[16] } ,
    {20,"journal-not-found" ,4,0.0,&avnx[17] } ,
    {20,"citation-not-found" ,5,0.0,&avnx[18] } ,
    {20,"citation-ambiguous" ,6,0.0,&avnx[19] } ,
    {20,"citation-too-many" ,7,0.0,NULL } };

static AsnType atx[58] = {
  {401, "Medline-entry" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[1]} ,
  {402, "Medlars-entry" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[2]} ,
  {403, "Pubmed-entry" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[3]} ,
  {404, "Medline-si" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[4]} ,
  {405, "Pub" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[5]} ,
  {406, "Title" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[6]} ,
  {407, "PubMedId" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[7]} ,
  {408, "Mla-request" ,1,0,0,0,0,0,0,0,NULL,&atx[35],&atx[8],0,&atx[14]} ,
  {0, "init" ,128,0,0,0,0,0,0,0,NULL,&atx[9],NULL,0,&atx[10]} ,
  {305, "NULL" ,0,5,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "getmle" ,128,1,0,0,0,0,0,0,NULL,&atx[11],NULL,0,&atx[12]} ,
  {302, "INTEGER" ,0,2,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "getpub" ,128,2,0,0,0,0,0,0,NULL,&atx[11],NULL,0,&atx[13]} ,
  {0, "gettitle" ,128,3,0,0,0,0,0,0,NULL,&atx[14],NULL,0,&atx[20]} ,
  {409, "Title-msg" ,1,0,0,0,0,0,0,0,NULL,&atx[19],&atx[15],0,&atx[16]} ,
  {0, "type" ,128,0,0,0,0,0,0,0,NULL,&atx[16],NULL,0,&atx[18]} ,
  {410, "Title-type" ,1,0,0,0,0,0,0,0,NULL,&atx[17],&avnx[0],0,&atx[36]} ,
  {310, "ENUMERATED" ,0,10,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "title" ,128,1,0,0,0,0,0,0,NULL,&atx[5],NULL,0,NULL} ,
  {311, "SEQUENCE" ,0,16,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "citmatch" ,128,4,0,0,0,0,0,0,NULL,&atx[4],NULL,0,&atx[21]} ,
  {0, "fini" ,128,5,0,0,0,0,0,0,NULL,&atx[9],NULL,0,&atx[22]} ,
  {0, "getmriuids" ,128,6,0,0,0,0,0,0,NULL,&atx[11],NULL,0,&atx[23]} ,
  {0, "getaccuids" ,128,7,0,0,0,0,0,0,NULL,&atx[3],NULL,0,&atx[24]} ,
  {0, "uidtopmid" ,128,8,0,0,0,0,0,0,NULL,&atx[11],NULL,0,&atx[25]} ,
  {0, "pmidtouid" ,128,9,0,0,0,0,0,0,NULL,&atx[6],NULL,0,&atx[26]} ,
  {0, "getmlepmid" ,128,10,0,0,0,0,0,0,NULL,&atx[6],NULL,0,&atx[27]} ,
  {0, "getpubpmid" ,128,11,0,0,0,0,0,0,NULL,&atx[6],NULL,0,&atx[28]} ,
  {0, "citmatchpmid" ,128,12,0,0,0,0,0,0,NULL,&atx[4],NULL,0,&atx[29]} ,
  {0, "getmripmids" ,128,13,0,0,0,0,0,0,NULL,&atx[11],NULL,0,&atx[30]} ,
  {0, "getaccpmids" ,128,14,0,0,0,0,0,0,NULL,&atx[3],NULL,0,&atx[31]} ,
  {0, "citlstpmids" ,128,15,0,0,0,0,0,0,NULL,&atx[4],NULL,0,&atx[32]} ,
  {0, "getmleuid" ,128,16,0,0,0,0,0,0,NULL,&atx[11],NULL,0,&atx[33]} ,
  {0, "getmlrpmid" ,128,17,0,0,0,0,0,0,NULL,&atx[6],NULL,0,&atx[34]} ,
  {0, "getmlruid" ,128,18,0,0,0,0,0,0,NULL,&atx[11],NULL,0,NULL} ,
  {315, "CHOICE" ,0,-1,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {411, "Title-msg-list" ,1,0,0,0,0,0,0,0,NULL,&atx[19],&atx[37],0,&atx[41]} ,
  {0, "num" ,128,0,0,0,0,0,0,0,NULL,&atx[11],NULL,0,&atx[38]} ,
  {0, "titles" ,128,1,0,0,0,0,0,0,NULL,&atx[40],&atx[39],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[14],NULL,0,NULL} ,
  {312, "SEQUENCE OF" ,0,16,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {412, "Error-val" ,1,0,0,0,0,0,0,0,NULL,&atx[17],&avnx[12],0,&atx[42]} ,
  {413, "Mla-back" ,1,0,0,0,0,0,0,0,NULL,&atx[35],&atx[43],0,NULL} ,
  {0, "init" ,128,0,0,0,0,0,0,0,NULL,&atx[9],NULL,0,&atx[44]} ,
  {0, "error" ,128,1,0,0,0,0,0,0,NULL,&atx[41],NULL,0,&atx[45]} ,
  {0, "getmle" ,128,2,0,0,0,0,0,0,NULL,&atx[0],NULL,0,&atx[46]} ,
  {0, "getpub" ,128,3,0,0,0,0,0,0,NULL,&atx[4],NULL,0,&atx[47]} ,
  {0, "gettitle" ,128,4,0,0,0,0,0,0,NULL,&atx[36],NULL,0,&atx[48]} ,
  {0, "citmatch" ,128,5,0,0,0,0,0,0,NULL,&atx[11],NULL,0,&atx[49]} ,
  {0, "fini" ,128,6,0,0,0,0,0,0,NULL,&atx[9],NULL,0,&atx[50]} ,
  {0, "getuids" ,128,7,0,0,0,0,0,0,NULL,&atx[40],&atx[51],0,&atx[52]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[11],NULL,0,NULL} ,
  {0, "getpmids" ,128,8,0,0,0,0,0,0,NULL,&atx[40],&atx[53],0,&atx[54]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[11],NULL,0,NULL} ,
  {0, "outuid" ,128,9,0,0,0,0,0,0,NULL,&atx[11],NULL,0,&atx[55]} ,
  {0, "outpmid" ,128,10,0,0,0,0,0,0,NULL,&atx[6],NULL,0,&atx[56]} ,
  {0, "getpme" ,128,11,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[57]} ,
  {0, "getmlr" ,128,12,0,0,0,0,0,0,NULL,&atx[1],NULL,0,NULL} };

static AsnModule ampx[1] = {
  { "NCBI-MedArchive" , "asnmla.h61",&atx[0],NULL,NULL,0,0} };

static AsnValxNodePtr avn = avnx;
static AsnTypePtr at = atx;
static AsnModulePtr amp = ampx;



/**************************************************
*
*    Defines for Module NCBI-MedArchive
*
**************************************************/

#define MLA_REQUEST &at[7]
#define MLA_REQUEST_init &at[8]
#define MLA_REQUEST_getmle &at[10]
#define MLA_REQUEST_getpub &at[12]
#define MLA_REQUEST_gettitle &at[13]
#define MLA_REQUEST_citmatch &at[20]
#define MLA_REQUEST_fini &at[21]
#define MLA_REQUEST_getmriuids &at[22]
#define MLA_REQUEST_getaccuids &at[23]
#define MLA_REQUEST_uidtopmid &at[24]
#define MLA_REQUEST_pmidtouid &at[25]
#define MLA_REQUEST_getmlepmid &at[26]
#define MLA_REQUEST_getpubpmid &at[27]
#define MLA_REQUEST_citmatchpmid &at[28]
#define MLA_REQUEST_getmripmids &at[29]
#define MLA_REQUEST_getaccpmids &at[30]
#define MLA_REQUEST_citlstpmids &at[31]
#define MLA_REQUEST_getmleuid &at[32]
#define MLA_REQUEST_getmlrpmid &at[33]
#define MLA_REQUEST_getmlruid &at[34]

#define TITLE_MSG &at[14]
#define TITLE_MSG_type &at[15]
#define TITLE_MSG_title &at[18]

#define TITLE_TYPE &at[16]

#define TITLE_MSG_LIST &at[36]
#define TITLE_MSG_LIST_num &at[37]
#define TITLE_MSG_LIST_titles &at[38]
#define TITLE_MSG_LIST_titles_E &at[39]

#define ERROR_VAL &at[41]

#define MLA_BACK &at[42]
#define MLA_BACK_init &at[43]
#define MLA_BACK_error &at[44]
#define MLA_BACK_getmle &at[45]
#define MLA_BACK_getpub &at[46]
#define MLA_BACK_gettitle &at[47]
#define MLA_BACK_citmatch &at[48]
#define MLA_BACK_fini &at[49]
#define MLA_BACK_getuids &at[50]
#define MLA_BACK_getuids_E &at[51]
#define MLA_BACK_getpmids &at[52]
#define MLA_BACK_getpmids_E &at[53]
#define MLA_BACK_outuid &at[54]
#define MLA_BACK_outpmid &at[55]
#define MLA_BACK_getpme &at[56]
#define MLA_BACK_getmlr &at[57]
