/***********************************************************************
*
**
*        Automatic header module from ASNTOOL
*
************************************************************************/

#ifndef _ASNTOOL_
#include <asn.h>
#endif

static char * asnfilename = "asnmdrs.h60";
static AsnType atx[14] = {
  {401, "Medlars-entry" ,1,0,0,0,0,1,0,0,NULL,&atx[12],&atx[1],0,&atx[7]} ,
  {0, "pmid" ,128,0,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[3]} ,
  {403, "PubMedId" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,NULL} ,
  {0, "muid" ,128,1,0,1,0,0,0,0,NULL,&atx[4],NULL,0,&atx[5]} ,
  {302, "INTEGER" ,0,2,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "recs" ,128,2,0,0,0,0,0,0,NULL,&atx[13],&atx[6],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[7],NULL,0,NULL} ,
  {402, "Medlars-record" ,1,0,0,0,0,1,0,0,NULL,&atx[12],&atx[8],0,&atx[2]} ,
  {0, "code" ,128,0,0,0,0,0,0,0,NULL,&atx[4],NULL,0,&atx[9]} ,
  {0, "abbr" ,128,1,0,1,0,0,0,0,NULL,&atx[10],NULL,0,&atx[11]} ,
  {323, "VisibleString" ,0,26,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "data" ,128,2,0,0,0,0,0,0,NULL,&atx[10],NULL,0,NULL} ,
  {311, "SEQUENCE" ,0,16,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {314, "SET OF" ,0,17,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} };

static AsnModule ampx[1] = {
  { "NCBI-Medlars" , "asnmdrs.h60",&atx[0],NULL,NULL,0,0} };

static AsnValxNodePtr avn = NULL;
static AsnTypePtr at = atx;
static AsnModulePtr amp = ampx;



/**************************************************
*
*    Defines for Module NCBI-Medlars
*
**************************************************/

#define MEDLARS_ENTRY &at[0]
#define MEDLARS_ENTRY_pmid &at[1]
#define MEDLARS_ENTRY_muid &at[3]
#define MEDLARS_ENTRY_recs &at[5]
#define MEDLARS_ENTRY_recs_E &at[6]

#define MEDLARS_RECORD &at[7]
#define MEDLARS_RECORD_code &at[8]
#define MEDLARS_RECORD_abbr &at[9]
#define MEDLARS_RECORD_data &at[11]
