/***********************************************************************
*
**
*        Automatic header module from ASNTOOL
*
************************************************************************/

#ifndef _ASNTOOL_
#include <asn.h>
#endif

static char * asnfilename = "asnacces.h60";
static AsnType atx[9] = {
  {401, "Link-set" ,1,0,0,0,0,1,0,0,NULL,&atx[8],&atx[1],0,NULL} ,
  {0, "num" ,128,0,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[3]} ,
  {302, "INTEGER" ,0,2,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "uids" ,128,1,0,1,0,0,0,0,NULL,&atx[5],&atx[4],0,&atx[6]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[2],NULL,0,NULL} ,
  {312, "SEQUENCE OF" ,0,16,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "weights" ,128,2,0,1,0,0,0,0,NULL,&atx[5],&atx[7],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[2],NULL,0,NULL} ,
  {311, "SEQUENCE" ,0,16,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} };

static AsnModule ampx[1] = {
  { "NCBI-Access" , "asnacces.h60",&atx[0],NULL,NULL,0,0} };

static AsnValxNodePtr avn = NULL;
static AsnTypePtr at = atx;
static AsnModulePtr amp = ampx;



/**************************************************
*
*    Defines for Module NCBI-Access
*
**************************************************/

#define LINK_SET &at[0]
#define LINK_SET_num &at[1]
#define LINK_SET_uids &at[3]
#define LINK_SET_uids_E &at[4]
#define LINK_SET_weights &at[6]
#define LINK_SET_weights_E &at[7]
