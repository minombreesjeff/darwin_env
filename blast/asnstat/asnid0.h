/***********************************************************************
*
**
*        Automatic header module from ASNTOOL
*
************************************************************************/

#ifndef _ASNTOOL_
#include <asn.h>
#endif

static char * asnfilename = "asnid0.h60";
static AsnValxNode avnx[5] = {
    {20,"entry" ,0,0.0,&avnx[1] } ,
    {20,"bioseq" ,1,0.0,&avnx[2] } ,
    {20,"bioseq-set" ,2,0.0,&avnx[3] } ,
    {20,"nuc-prot" ,3,0.0,&avnx[4] } ,
    {20,"pub-set" ,4,0.0,NULL } };

static AsnType atx[26] = {
  {401, "Seq-id" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[1]} ,
  {402, "Seq-entry" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[2]} ,
  {403, "ID0server-request" ,1,0,0,0,0,0,0,0,NULL,&atx[15],&atx[3],0,&atx[7]} ,
  {0, "init" ,128,0,0,0,0,0,0,0,NULL,&atx[4],NULL,0,&atx[5]} ,
  {305, "NULL" ,0,5,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "getgi" ,128,1,0,0,0,0,0,0,NULL,&atx[0],NULL,0,&atx[6]} ,
  {0, "getsefromgi" ,128,2,0,0,0,0,0,0,NULL,&atx[7],NULL,0,&atx[14]} ,
  {404, "ID0server-maxcomplex" ,1,0,0,0,0,0,0,0,NULL,&atx[13],&atx[8],0,&atx[9]} ,
  {0, "maxplex" ,128,0,0,0,0,0,0,0,NULL,&atx[9],NULL,0,&atx[11]} ,
  {405, "Entry-complexities" ,1,0,0,0,0,0,0,0,NULL,&atx[10],&avnx[0],0,&atx[16]} ,
  {310, "ENUMERATED" ,0,10,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "gi" ,128,1,0,0,0,0,0,0,NULL,&atx[12],NULL,0,NULL} ,
  {302, "INTEGER" ,0,2,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {311, "SEQUENCE" ,0,16,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "fini" ,128,3,0,0,0,0,0,0,NULL,&atx[4],NULL,0,NULL} ,
  {315, "CHOICE" ,0,-1,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {406, "ID0server-back" ,1,0,0,0,0,0,0,0,NULL,&atx[15],&atx[17],0,&atx[23]} ,
  {0, "init" ,128,0,0,0,0,0,0,0,NULL,&atx[4],NULL,0,&atx[18]} ,
  {0, "error" ,128,1,0,0,0,0,0,0,NULL,&atx[12],NULL,0,&atx[19]} ,
  {0, "gotgi" ,128,2,0,0,0,0,0,0,NULL,&atx[12],NULL,0,&atx[20]} ,
  {0, "gotseqentry" ,128,3,0,0,0,0,0,0,NULL,&atx[1],NULL,0,&atx[21]} ,
  {0, "gotdeadseqentry" ,128,4,0,0,0,0,0,0,NULL,&atx[1],NULL,0,&atx[22]} ,
  {0, "fini" ,128,5,0,0,0,0,0,0,NULL,&atx[4],NULL,0,NULL} ,
  {407, "ID0server-debug" ,1,0,0,0,0,0,0,0,NULL,&atx[25],&atx[24],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[16],NULL,0,NULL} ,
  {314, "SET OF" ,0,17,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} };

static AsnModule ampx[1] = {
  { "NCBI-ID0Access" , "asnid0.h60",&atx[0],NULL,NULL,0,0} };

static AsnValxNodePtr avn = avnx;
static AsnTypePtr at = atx;
static AsnModulePtr amp = ampx;



/**************************************************
*
*    Defines for Module NCBI-ID0Access
*
**************************************************/

#define ID0SERVER_REQUEST &at[2]
#define ID0SERVER_REQUEST_init &at[3]
#define ID0SERVER_REQUEST_getgi &at[5]
#define ID0SERVER_REQUEST_getsefromgi &at[6]
#define ID0SERVER_REQUEST_fini &at[14]

#define ID0SERVER_MAXCOMPLEX &at[7]
#define ID0SERVER_MAXCOMPLEX_maxplex &at[8]
#define ID0SERVER_MAXCOMPLEX_gi &at[11]

#define ENTRY_COMPLEXITIES &at[9]

#define ID0SERVER_BACK &at[16]
#define ID0SERVER_BACK_init &at[17]
#define ID0SERVER_BACK_error &at[18]
#define ID0SERVER_BACK_gotgi &at[19]
#define ID0SERVER_BACK_gotseqentry &at[20]
#define ID0SERVER_BACK_gotdeadseqentry &at[21]
#define ID0SERVER_BACK_fini &at[22]

#define ID0SERVER_DEBUG &at[23]
#define ID0SERVER_DEBUG_E &at[24]
