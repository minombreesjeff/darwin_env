/***********************************************************************
*
**
*        Automatic header module from ASNTOOL
*
************************************************************************/

#ifndef _ASNTOOL_
#include <asn.h>
#endif

static char * asnfilename = "id1.h11";
static AsnValxNode avnx[5] = {
    {20,"entry" ,0,0.0,&avnx[1] } ,
    {20,"bioseq" ,1,0.0,&avnx[2] } ,
    {20,"bioseq-set" ,2,0.0,&avnx[3] } ,
    {20,"nuc-prot" ,3,0.0,&avnx[4] } ,
    {20,"pub-set" ,4,0.0,NULL } };

static AsnType atx[60] = {
  {401, "Seq-id" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[1]} ,
  {402, "Seq-entry" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[2]} ,
  {403, "Seq-hist" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[3]} ,
  {404, "ID1server-request" ,1,0,0,0,0,0,0,0,NULL,&atx[24],&atx[4],0,&atx[8]} ,
  {0, "init" ,128,0,0,0,0,0,0,0,NULL,&atx[5],NULL,0,&atx[6]} ,
  {305, "NULL" ,0,5,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "getgi" ,128,1,0,0,0,0,0,0,NULL,&atx[0],NULL,0,&atx[7]} ,
  {0, "getsefromgi" ,128,2,0,0,0,0,0,0,NULL,&atx[8],NULL,0,&atx[17]} ,
  {405, "ID1server-maxcomplex" ,1,0,0,0,0,0,0,0,NULL,&atx[16],&atx[9],0,&atx[10]} ,
  {0, "maxplex" ,128,0,0,0,0,0,0,0,NULL,&atx[10],NULL,0,&atx[12]} ,
  {406, "Entry-complexities" ,1,0,0,0,0,0,0,0,NULL,&atx[11],&avnx[0],0,&atx[25]} ,
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
  {0, "getgistate" ,128,7,0,0,0,0,0,0,NULL,&atx[11],NULL,0,&atx[22]} ,
  {0, "getsewithinfo" ,128,8,0,0,0,0,0,0,NULL,&atx[8],NULL,0,&atx[23]} ,
  {0, "getblobinfo" ,128,9,0,0,0,0,0,0,NULL,&atx[8],NULL,0,NULL} ,
  {315, "CHOICE" ,0,-1,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {407, "ID1Seq-hist" ,1,0,0,0,0,0,0,0,NULL,&atx[16],&atx[26],0,&atx[27]} ,
  {0, "hist" ,128,0,0,0,0,0,0,0,NULL,&atx[2],NULL,0,NULL} ,
  {408, "ID1server-back" ,1,0,0,0,0,0,0,0,NULL,&atx[24],&atx[28],0,&atx[43]} ,
  {0, "init" ,128,0,0,0,0,0,0,0,NULL,&atx[5],NULL,0,&atx[29]} ,
  {0, "error" ,128,1,0,0,0,0,0,0,NULL,&atx[11],NULL,0,&atx[30]} ,
  {0, "gotgi" ,128,2,0,0,0,0,0,0,NULL,&atx[11],NULL,0,&atx[31]} ,
  {0, "gotseqentry" ,128,3,0,0,0,0,0,0,NULL,&atx[1],NULL,0,&atx[32]} ,
  {0, "gotdeadseqentry" ,128,4,0,0,0,0,0,0,NULL,&atx[1],NULL,0,&atx[33]} ,
  {0, "fini" ,128,5,0,0,0,0,0,0,NULL,&atx[5],NULL,0,&atx[34]} ,
  {0, "gistate" ,128,6,0,0,0,0,0,0,NULL,&atx[11],NULL,0,&atx[35]} ,
  {0, "ids" ,128,7,0,0,0,0,0,0,NULL,&atx[37],&atx[36],0,&atx[38]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[0],NULL,0,NULL} ,
  {314, "SET OF" ,0,17,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "gihist" ,128,8,0,0,0,0,0,0,NULL,&atx[37],&atx[39],0,&atx[40]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[25],NULL,0,NULL} ,
  {0, "girevhist" ,128,9,0,0,0,0,0,0,NULL,&atx[37],&atx[41],0,&atx[42]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[25],NULL,0,NULL} ,
  {0, "gotsewithinfo" ,128,10,0,0,0,0,0,0,NULL,&atx[43],NULL,0,&atx[57]} ,
  {409, "ID1SeqEntry-info" ,1,0,0,0,0,0,0,0,NULL,&atx[16],&atx[44],0,&atx[45]} ,
  {0, "blob-info" ,128,0,0,0,0,0,0,0,NULL,&atx[45],NULL,0,&atx[56]} ,
  {410, "ID1blob-info" ,1,0,0,0,0,0,0,0,NULL,&atx[16],&atx[46],0,&atx[58]} ,
  {0, "gi" ,128,0,0,0,0,0,0,0,NULL,&atx[11],NULL,0,&atx[47]} ,
  {0, "sat" ,128,1,0,0,0,0,0,0,NULL,&atx[11],NULL,0,&atx[48]} ,
  {0, "sat-key" ,128,2,0,0,0,0,0,0,NULL,&atx[11],NULL,0,&atx[49]} ,
  {0, "satname" ,128,3,0,0,0,0,0,0,NULL,&atx[15],NULL,0,&atx[50]} ,
  {0, "suppress" ,128,4,0,0,0,0,0,0,NULL,&atx[11],NULL,0,&atx[51]} ,
  {0, "withdrawn" ,128,5,0,0,0,0,0,0,NULL,&atx[11],NULL,0,&atx[52]} ,
  {0, "confidential" ,128,6,0,0,0,0,0,0,NULL,&atx[11],NULL,0,&atx[53]} ,
  {0, "blob-state" ,128,7,0,0,0,0,0,0,NULL,&atx[11],NULL,0,&atx[54]} ,
  {0, "comment" ,128,8,0,1,0,0,0,0,NULL,&atx[15],NULL,0,&atx[55]} ,
  {0, "extfeatmask" ,128,9,0,1,0,0,0,0,NULL,&atx[11],NULL,0,NULL} ,
  {0, "blob" ,128,1,0,1,0,0,0,0,NULL,&atx[1],NULL,0,NULL} ,
  {0, "gotblobinfo" ,128,11,0,0,0,0,0,0,NULL,&atx[45],NULL,0,NULL} ,
  {411, "ID1server-debug" ,1,0,0,0,0,0,0,0,NULL,&atx[37],&atx[59],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[27],NULL,0,NULL} };

static AsnModule ampx[1] = {
  { "NCBI-ID1Access" , "id1.h11",&atx[0],NULL,NULL,0,0} };

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
#define ID1SERVER_REQUEST_getsewithinfo &at[22]
#define ID1SERVER_REQUEST_getblobinfo &at[23]

#define ID1SERVER_MAXCOMPLEX &at[8]
#define ID1SERVER_MAXCOMPLEX_maxplex &at[9]
#define ID1SERVER_MAXCOMPLEX_gi &at[12]
#define ID1SERVER_MAXCOMPLEX_ent &at[13]
#define ID1SERVER_MAXCOMPLEX_sat &at[14]

#define ENTRY_COMPLEXITIES &at[10]

#define ID1SEQ_HIST &at[25]
#define ID1SEQ_HIST_hist &at[26]

#define ID1SERVER_BACK &at[27]
#define ID1SERVER_BACK_init &at[28]
#define ID1SERVER_BACK_error &at[29]
#define ID1SERVER_BACK_gotgi &at[30]
#define ID1SERVER_BACK_gotseqentry &at[31]
#define ID1SERVER_BACK_gotdeadseqentry &at[32]
#define ID1SERVER_BACK_fini &at[33]
#define ID1SERVER_BACK_gistate &at[34]
#define ID1SERVER_BACK_ids &at[35]
#define ID1SERVER_BACK_ids_E &at[36]
#define ID1SERVER_BACK_gihist &at[38]
#define ID1SERVER_BACK_gihist_E &at[39]
#define ID1SERVER_BACK_girevhist &at[40]
#define ID1SERVER_BACK_girevhist_E &at[41]
#define ID1SERVER_BACK_gotsewithinfo &at[42]
#define ID1SERVER_BACK_gotblobinfo &at[57]

#define ID1SEQENTRY_INFO &at[43]
#define ID1SEQENTRY_INFO_blob_info &at[44]
#define ID1SEQENTRY_INFO_blob &at[56]

#define ID1BLOB_INFO &at[45]
#define ID1BLOB_INFO_gi &at[46]
#define ID1BLOB_INFO_sat &at[47]
#define ID1BLOB_INFO_sat_key &at[48]
#define ID1BLOB_INFO_satname &at[49]
#define ID1BLOB_INFO_suppress &at[50]
#define ID1BLOB_INFO_withdrawn &at[51]
#define ID1BLOB_INFO_confidential &at[52]
#define ID1BLOB_INFO_blob_state &at[53]
#define ID1BLOB_INFO_comment &at[54]
#define ID1BLOB_INFO_extfeatmask &at[55]

#define ID1SERVER_DEBUG &at[58]
#define ID1SERVER_DEBUG_E &at[59]
