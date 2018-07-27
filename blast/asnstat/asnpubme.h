/***********************************************************************
*
**
*        Automatic header module from ASNTOOL
*
************************************************************************/

#ifndef _ASNTOOL_
#include <asn.h>
#endif

static char * asnfilename = "asnpubme.h60";
static AsnType atx[15] = {
  {401, "Pubmed-entry" ,1,0,0,0,0,1,0,0,NULL,&atx[12],&atx[1],0,&atx[9]} ,
  {0, "pmid" ,128,0,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[3]} ,
  {403, "PubMedId" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[4]} ,
  {0, "medent" ,128,1,0,1,0,0,0,0,NULL,&atx[4],NULL,0,&atx[5]} ,
  {404, "Medline-entry" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,NULL} ,
  {0, "publisher" ,128,2,0,1,0,0,0,0,NULL,&atx[6],NULL,0,&atx[7]} ,
  {323, "VisibleString" ,0,26,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "urls" ,128,3,0,1,0,0,0,0,NULL,&atx[13],&atx[8],0,&atx[14]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[9],NULL,0,NULL} ,
  {402, "Pubmed-url" ,1,0,0,0,0,1,0,0,NULL,&atx[12],&atx[10],0,&atx[2]} ,
  {0, "location" ,128,0,0,1,0,0,0,0,NULL,&atx[6],NULL,0,&atx[11]} ,
  {0, "url" ,128,1,0,0,0,0,0,0,NULL,&atx[6],NULL,0,NULL} ,
  {311, "SEQUENCE" ,0,16,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {314, "SET OF" ,0,17,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "pubid" ,128,4,0,1,0,0,0,0,NULL,&atx[6],NULL,0,NULL} };

static AsnModule ampx[1] = {
  { "NCBI-PubMed" , "asnpubme.h60",&atx[0],NULL,NULL,0,0} };

static AsnValxNodePtr avn = NULL;
static AsnTypePtr at = atx;
static AsnModulePtr amp = ampx;



/**************************************************
*
*    Defines for Module NCBI-PubMed
*
**************************************************/

#define PUBMED_ENTRY &at[0]
#define PUBMED_ENTRY_pmid &at[1]
#define PUBMED_ENTRY_medent &at[3]
#define PUBMED_ENTRY_publisher &at[5]
#define PUBMED_ENTRY_urls &at[7]
#define PUBMED_ENTRY_urls_E &at[8]
#define PUBMED_ENTRY_pubid &at[14]

#define PUBMED_URL &at[9]
#define PUBMED_URL_location &at[10]
#define PUBMED_URL_url &at[11]
