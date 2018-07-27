/***********************************************************************
*
**
*        Automatic header module from ASNTOOL
*
************************************************************************/

#ifndef _ASNTOOL_
#include <asn.h>
#endif

static char * asnfilename = "echo_asn.h60";
static AsnType atx[5] = {
  {401, "ECHO-String" ,1,0,0,0,0,1,0,0,NULL,&atx[4],&atx[1],0,NULL} ,
  {0, "process" ,128,0,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[3]} ,
  {323, "VisibleString" ,0,26,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "text" ,128,1,0,0,0,0,0,0,NULL,&atx[2],NULL,0,NULL} ,
  {311, "SEQUENCE" ,0,16,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} };

static AsnModule ampx[1] = {
  { "TEST-ECHO" , "echo_asn.h60",&atx[0],NULL,NULL,0,0} };

static AsnValxNodePtr avn = NULL;
static AsnTypePtr at = atx;
static AsnModulePtr amp = ampx;



/**************************************************
*
*    Defines for Module TEST-ECHO
*
**************************************************/

#define ECHO_STRING &at[0]
#define ECHO_STRING_process &at[1]
#define ECHO_STRING_text &at[3]
