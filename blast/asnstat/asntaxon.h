/***********************************************************************
*
**
*        Automatic header module from ASNTOOL
*
************************************************************************/

#ifndef _ASNTOOL_
#include <asn.h>
#endif

static char * asnfilename = "asntaxon.h";
static AsnType atx[62] = {
  {401, "Org-ref" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[1]} ,
  {402, "Taxon0-req" ,1,0,0,0,0,0,0,0,NULL,&atx[11],&atx[2],0,&atx[5]} ,
  {0, "init" ,128,0,0,0,0,0,0,0,NULL,&atx[3],NULL,0,&atx[4]} ,
  {305, "NULL" ,0,5,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "getid" ,128,1,0,0,0,0,0,0,NULL,&atx[5],NULL,0,&atx[12]} ,
  {403, "Taxon-name" ,1,0,0,0,0,0,0,0,NULL,&atx[11],&atx[6],0,&atx[20]} ,
  {0, "taxname" ,128,0,0,0,0,0,0,0,NULL,&atx[7],NULL,0,&atx[8]} ,
  {323, "VisibleString" ,0,26,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "common" ,128,1,0,0,0,0,0,0,NULL,&atx[7],NULL,0,&atx[9]} ,
  {0, "tax-synonym" ,128,2,0,0,0,0,0,0,NULL,&atx[7],NULL,0,&atx[10]} ,
  {0, "com-synonym" ,128,3,0,0,0,0,0,0,NULL,&atx[7],NULL,0,NULL} ,
  {315, "CHOICE" ,0,-1,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "getref" ,128,2,0,0,0,0,0,0,NULL,&atx[13],NULL,0,&atx[14]} ,
  {302, "INTEGER" ,0,2,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "getchildren" ,128,3,0,0,0,0,0,0,NULL,&atx[13],NULL,0,&atx[15]} ,
  {0, "getparents" ,128,4,0,0,0,0,0,0,NULL,&atx[13],NULL,0,&atx[16]} ,
  {0, "getgeneticcode" ,128,5,0,0,0,0,0,0,NULL,&atx[13],NULL,0,&atx[17]} ,
  {0, "gettaxonline" ,128,6,0,0,0,0,0,0,NULL,&atx[13],NULL,0,&atx[18]} ,
  {0, "getdivision" ,128,7,0,0,0,0,0,0,NULL,&atx[13],NULL,0,&atx[19]} ,
  {0, "getcomplete" ,128,8,0,0,0,0,0,0,NULL,&atx[20],NULL,0,&atx[23]} ,
  {404, "Taxon-id-name" ,1,0,0,0,0,0,0,0,NULL,&atx[11],&atx[21],0,&atx[24]} ,
  {0, "id" ,128,0,0,0,0,0,0,0,NULL,&atx[13],NULL,0,&atx[22]} ,
  {0, "name" ,128,1,0,0,0,0,0,0,NULL,&atx[7],NULL,0,NULL} ,
  {0, "fini" ,128,9,0,0,0,0,0,0,NULL,&atx[3],NULL,0,NULL} ,
  {405, "Taxon0-resp" ,1,0,0,0,0,0,0,0,NULL,&atx[11],&atx[25],0,&atx[28]} ,
  {0, "error" ,128,0,0,0,0,0,0,0,NULL,&atx[13],NULL,0,&atx[26]} ,
  {0, "init" ,128,1,0,0,0,0,0,0,NULL,&atx[3],NULL,0,&atx[27]} ,
  {0, "getid" ,128,2,0,0,0,0,0,0,NULL,&atx[28],NULL,0,&atx[33]} ,
  {406, "Taxon-id-list" ,1,0,0,0,0,0,0,0,NULL,&atx[32],&atx[29],0,&atx[36]} ,
  {0, "ids" ,128,0,0,0,0,0,0,0,NULL,&atx[31],&atx[30],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[13],NULL,0,NULL} ,
  {312, "SEQUENCE OF" ,0,16,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {311, "SEQUENCE" ,0,16,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "getref" ,128,3,0,0,0,0,0,0,NULL,&atx[0],NULL,0,&atx[34]} ,
  {0, "gettaxon" ,128,4,0,0,0,0,0,0,NULL,&atx[28],NULL,0,&atx[35]} ,
  {0, "getgeneticcode" ,128,5,0,0,0,0,0,0,NULL,&atx[36],NULL,0,&atx[39]} ,
  {407, "GeneticCodeList" ,1,0,0,0,0,0,0,0,NULL,&atx[32],&atx[37],0,&atx[42]} ,
  {0, "genomic" ,128,0,0,0,0,0,0,0,NULL,&atx[13],NULL,0,&atx[38]} ,
  {0, "mitochondrial" ,128,1,0,1,0,0,0,0,NULL,&atx[13],NULL,0,NULL} ,
  {0, "gettaxonline" ,128,6,0,0,0,0,0,0,NULL,&atx[7],NULL,0,&atx[40]} ,
  {0, "getdivision" ,128,7,0,0,0,0,0,0,NULL,&atx[7],NULL,0,&atx[41]} ,
  {0, "getcomplete" ,128,8,0,0,0,0,0,0,NULL,&atx[42],NULL,0,&atx[59]} ,
  {408, "Tax-complete-list" ,1,0,0,0,0,0,0,0,NULL,&atx[32],&atx[43],0,&atx[60]} ,
  {0, "num" ,128,0,0,0,0,0,0,0,NULL,&atx[13],NULL,0,&atx[44]} ,
  {0, "info" ,128,1,0,0,0,0,0,0,NULL,&atx[31],&atx[45],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[46],NULL,0,NULL} ,
  {410, "Tax-complete" ,1,0,0,0,0,0,0,0,NULL,&atx[32],&atx[47],0,NULL} ,
  {0, "sciname" ,128,0,0,0,0,0,0,0,NULL,&atx[7],NULL,0,&atx[48]} ,
  {0, "comname" ,128,1,0,0,0,0,0,0,NULL,&atx[7],NULL,0,&atx[49]} ,
  {0, "synonyms" ,128,2,0,0,0,0,0,0,NULL,&atx[7],NULL,0,&atx[50]} ,
  {0, "id-gc" ,128,3,0,0,0,0,0,0,NULL,&atx[13],NULL,0,&atx[51]} ,
  {0, "name-gc" ,128,4,0,0,0,0,0,0,NULL,&atx[7],NULL,0,&atx[52]} ,
  {0, "id-mgc" ,128,5,0,0,0,0,0,0,NULL,&atx[13],NULL,0,&atx[53]} ,
  {0, "name-mgc" ,128,6,0,0,0,0,0,0,NULL,&atx[7],NULL,0,&atx[54]} ,
  {0, "gb-div" ,128,7,0,0,0,0,0,0,NULL,&atx[7],NULL,0,&atx[55]} ,
  {0, "embl-code" ,128,8,0,0,0,0,0,0,NULL,&atx[7],NULL,0,&atx[56]} ,
  {0, "lineage" ,128,9,0,0,0,0,0,0,NULL,&atx[7],NULL,0,&atx[57]} ,
  {0, "is-species-level" ,128,10,0,0,0,0,0,0,NULL,&atx[58],NULL,0,NULL} ,
  {301, "BOOLEAN" ,0,1,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "fini" ,128,9,0,0,0,0,0,0,NULL,&atx[3],NULL,0,NULL} ,
  {409, "Taxon-id" ,1,0,0,0,0,0,0,0,NULL,&atx[32],&atx[61],0,&atx[46]} ,
  {0, "id" ,128,0,0,0,0,0,0,0,NULL,&atx[13],NULL,0,NULL} };

static AsnModule ampx[1] = {
  { "NCBI-Taxon0" , "asntaxon.h",&atx[0],NULL,NULL,0,0} };

static AsnValxNodePtr avn = NULL;
static AsnTypePtr at = atx;
static AsnModulePtr amp = ampx;



/**************************************************
*
*    Defines for Module NCBI-Taxon0
*
**************************************************/

#define TAXON0_REQ &at[1]
#define TAXON0_REQ_init &at[2]
#define TAXON0_REQ_getid &at[4]
#define TAXON0_REQ_getref &at[12]
#define TAXON0_REQ_getchildren &at[14]
#define TAXON0_REQ_getparents &at[15]
#define TAXON0_REQ_getgeneticcode &at[16]
#define TAXON0_REQ_gettaxonline &at[17]
#define TAXON0_REQ_getdivision &at[18]
#define TAXON0_REQ_getcomplete &at[19]
#define TAXON0_REQ_fini &at[23]

#define TAXON_NAME &at[5]
#define TAXON_NAME_taxname &at[6]
#define TAXON_NAME_common &at[8]
#define TAXON_NAME_tax_synonym &at[9]
#define TAXON_NAME_com_synonym &at[10]

#define TAXON_ID_NAME &at[20]
#define TAXON_ID_NAME_id &at[21]
#define TAXON_ID_NAME_name &at[22]

#define TAXON0_RESP &at[24]
#define TAXON0_RESP_error &at[25]
#define TAXON0_RESP_init &at[26]
#define TAXON0_RESP_getid &at[27]
#define TAXON0_RESP_getref &at[33]
#define TAXON0_RESP_gettaxon &at[34]
#define TAXON0_RESP_getgeneticcode &at[35]
#define TAXON0_RESP_gettaxonline &at[39]
#define TAXON0_RESP_getdivision &at[40]
#define TAXON0_RESP_getcomplete &at[41]
#define TAXON0_RESP_fini &at[59]

#define TAXON_ID_LIST &at[28]
#define TAXON_ID_LIST_ids &at[29]
#define TAXON_ID_LIST_ids_E &at[30]

#define GENETICCODELIST &at[36]
#define GENETICCODELIST_genomic &at[37]
#define GENETICCODELIST_mitochondrial &at[38]

#define TAX_COMPLETE_LIST &at[42]
#define TAX_COMPLETE_LIST_num &at[43]
#define TAX_COMPLETE_LIST_info &at[44]
#define TAX_COMPLETE_LIST_info_E &at[45]

#define TAXON_ID &at[60]
#define TAXON_ID_id &at[61]

#define TAX_COMPLETE &at[46]
#define TAX_COMPLETE_sciname &at[47]
#define TAX_COMPLETE_comname &at[48]
#define TAX_COMPLETE_synonyms &at[49]
#define TAX_COMPLETE_id_gc &at[50]
#define TAX_COMPLETE_name_gc &at[51]
#define TAX_COMPLETE_id_mgc &at[52]
#define TAX_COMPLETE_name_mgc &at[53]
#define TAX_COMPLETE_gb_div &at[54]
#define TAX_COMPLETE_embl_code &at[55]
#define TAX_COMPLETE_lineage &at[56]
#define TAX_COMPLETE_is_species_level &at[57]
