/***********************************************************************
*
**
*        Automatic header module from ASNTOOL
*
************************************************************************/

#ifndef _ASNTOOL_
#include <asn.h>
#endif

static char * asnfilename = "id1.h13";
static AsnValxNode avnx[5] = {
    {20,"entry" ,0,0.0,&avnx[1] } ,
    {20,"bioseq" ,1,0.0,&avnx[2] } ,
    {20,"bioseq-set" ,2,0.0,&avnx[3] } ,
    {20,"nuc-prot" ,3,0.0,&avnx[4] } ,
    {20,"pub-set" ,4,0.0,NULL } };

static AsnType atx[42] = {
  {401, "Seq-id" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[1]} ,
  {402, "Seq-entry" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[2]} ,
  {403, "Seq-hist" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[3]} ,
  {404, "ID1server-request" ,1,0,0,0,0,0,0,0,NULL,&atx[22],&atx[4],0,&atx[8]} ,
  {0, "init" ,128,0,0,0,0,0,0,0,NULL,&atx[5],NULL,0,&atx[6]} ,
  {305, "NULL" ,0,5,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "getgi" ,128,1,0,0,0,0,0,0,NULL,&atx[0],NULL,0,&atx[7]} ,
  {0, "getsefromgi" ,128,2,0,0,0,0,0,0,NULL,&atx[8],NULL,0,&atx[17]} ,
  {405, "ID1server-maxcomplex" ,1,0,0,0,0,0,0,0,NULL,&atx[16],&atx[9],0,&atx[10]} ,
  {0, "maxplex" ,128,0,0,0,0,0,0,0,NULL,&atx[10],NULL,0,&atx[12]} ,
  {406, "Entry-complexities" ,1,0,0,0,0,0,0,0,NULL,&atx[11],&avnx[0],0,&atx[23]} ,
  {302, "INTEGER" ,0,2,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "gi" ,128,1,0,0,0,0,0,0,NULL,&atx[11],NULL,0,&atx[13]} ,
  {0, "ent" ,128,2,0,1,0,0,0,0,NULL,&atx[11],NULL,0,&atx[14]} ,
  {0, "sat" ,128,3,0,1,0,0,0,0,NULL,&atx[15],NULL,0,NULL} ,
  {323, "VisibleString" ,0,26,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {311, "SEQUENCE" ,0,16,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "fini" ,128,3,0,0,0,0,0,0,NULL,&atx[5],NULL,0,&atx[18]} ,
  {0, "getseqidsfromgi" ,128,4,0,0,0,0,0,0,NULL,&atx[11],NULL,0,&atx[19]} ,
  {0, "getgihist" ,128,5,0,0,0,0,0,0,NULL,&atx[11],NULL,0,&atx[20]} ,
  {0, "getgirev" ,128,6,0,0,0,0,0,0,NULL,&atx[11],NULL,0,&atx[21]} ,
  {0, "getgistate" ,128,7,0,0,0,0,0,0,NULL,&atx[11],NULL,0,NULL} ,
  {315, "CHOICE" ,0,-1,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {407, "ID1Seq-hist" ,1,0,0,0,0,0,0,0,NULL,&atx[16],&atx[24],0,&atx[25]} ,
  {0, "hist" ,128,0,0,0,0,0,0,0,NULL,&atx[2],NULL,0,NULL} ,
  {408, "ID1server-back" ,1,0,0,0,0,0,0,0,NULL,&atx[22],&atx[26],0,&atx[40]} ,
  {0, "init" ,128,0,0,0,0,0,0,0,NULL,&atx[5],NULL,0,&atx[27]} ,
  {0, "error" ,128,1,0,0,0,0,0,0,NULL,&atx[11],NULL,0,&atx[28]} ,
  {0, "gotgi" ,128,2,0,0,0,0,0,0,NULL,&atx[11],NULL,0,&atx[29]} ,
  {0, "gotseqentry" ,128,3,0,0,0,0,0,0,NULL,&atx[1],NULL,0,&atx[30]} ,
  {0, "gotdeadseqentry" ,128,4,0,0,0,0,0,0,NULL,&atx[1],NULL,0,&atx[31]} ,
  {0, "fini" ,128,5,0,0,0,0,0,0,NULL,&atx[5],NULL,0,&atx[32]} ,
  {0, "gistate" ,128,6,0,0,0,0,0,0,NULL,&atx[11],NULL,0,&atx[33]} ,
  {0, "ids" ,128,7,0,0,0,0,0,0,NULL,&atx[35],&atx[34],0,&atx[36]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[0],NULL,0,NULL} ,
  {314, "SET OF" ,0,17,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "gihist" ,128,8,0,0,0,0,0,0,NULL,&atx[35],&atx[37],0,&atx[38]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[23],NULL,0,NULL} ,
  {0, "girevhist" ,128,9,0,0,0,0,0,0,NULL,&atx[35],&atx[39],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[23],NULL,0,NULL} ,
  {409, "ID1server-debug" ,1,0,0,0,0,0,0,0,NULL,&atx[35],&atx[41],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[25],NULL,0,NULL} };

static AsnModule ampx[1] = {
  { "NCBI-ID1Access" , "id1.h13",&atx[0],NULL,NULL,0,0} };

static AsnValxNodePtr avn = avnx;
static AsnTypePtr at = atx;
static AsnModulePtr amp = ampx;



/**************************************************
*
*    Defines for Module NCBI-ID1Access
*
**************************************************/

#define ID1SERVER_REQUEST &at[3]
#define ID1SERVER_REQUEST_init &at[4]
#define ID1SERVER_REQUEST_getgi &at[6]
#define ID1SERVER_REQUEST_getsefromgi &at[7]
#define ID1SERVER_REQUEST_fini &at[17]
#define REQUEST_getseqidsfromgi &at[18]
#define ID1SERVER_REQUEST_getgihist &at[19]
#define ID1SERVER_REQUEST_getgirev &at[20]
#define ID1SERVER_REQUEST_getgistate &at[21]

#define ID1SERVER_MAXCOMPLEX &at[8]
#define ID1SERVER_MAXCOMPLEX_maxplex &at[9]
#define ID1SERVER_MAXCOMPLEX_gi &at[12]
#define ID1SERVER_MAXCOMPLEX_ent &at[13]
#define ID1SERVER_MAXCOMPLEX_sat &at[14]

#define ENTRY_COMPLEXITIES &at[10]

#define ID1SEQ_HIST &at[23]
#define ID1SEQ_HIST_hist &at[24]

#define ID1SERVER_BACK &at[25]
#define ID1SERVER_BACK_init &at[26]
#define ID1SERVER_BACK_error &at[27]
#define ID1SERVER_BACK_gotgi &at[28]
#define ID1SERVER_BACK_gotseqentry &at[29]
#define ID1SERVER_BACK_gotdeadseqentry &at[30]
#define ID1SERVER_BACK_fini &at[31]
#define ID1SERVER_BACK_gistate &at[32]
#define ID1SERVER_BACK_ids &at[33]
#define ID1SERVER_BACK_ids_E &at[34]
#define ID1SERVER_BACK_gihist &at[36]
#define ID1SERVER_BACK_gihist_E &at[37]
#define ID1SERVER_BACK_girevhist &at[38]
#define ID1SERVER_BACK_girevhist_E &at[39]

#define ID1SERVER_DEBUG &at[40]
#define ID1SERVER_DEBUG_E &at[41]
