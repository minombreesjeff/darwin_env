/***********************************************************************
*
**
*        Automatic header module from ASNTOOL
*
************************************************************************/

#ifndef _ASNTOOL_
#include <asn.h>
#endif

static char * asnfilename = "taxon1.h66";
static AsnValxNode avnx[5] = {
    {20,"none" ,0,0.0,&avnx[1] } ,
    {20,"info" ,1,0.0,&avnx[2] } ,
    {20,"warn" ,2,0.0,&avnx[3] } ,
    {20,"error" ,3,0.0,&avnx[4] } ,
    {20,"fatal" ,4,0.0,NULL } };

static AsnType atx[97] = {
  {401, "Org-ref" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[1]} ,
  {402, "Taxon1-req" ,1,0,0,0,0,0,0,0,NULL,&atx[33],&atx[2],0,&atx[20]} ,
  {0, "init" ,128,0,0,0,0,0,0,0,NULL,&atx[3],NULL,0,&atx[4]} ,
  {305, "NULL" ,0,5,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "findname" ,128,1,0,0,0,0,0,0,NULL,&atx[5],NULL,0,&atx[6]} ,
  {323, "VisibleString" ,0,26,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "getdesignator" ,128,2,0,0,0,0,0,0,NULL,&atx[5],NULL,0,&atx[7]} ,
  {0, "getunique" ,128,3,0,0,0,0,0,0,NULL,&atx[5],NULL,0,&atx[8]} ,
  {0, "getidbyorg" ,128,4,0,0,0,0,0,0,NULL,&atx[0],NULL,0,&atx[9]} ,
  {0, "getorgnames" ,128,5,0,0,0,0,0,0,NULL,&atx[10],NULL,0,&atx[11]} ,
  {302, "INTEGER" ,0,2,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "getcde" ,128,6,0,0,0,0,0,0,NULL,&atx[3],NULL,0,&atx[12]} ,
  {0, "getranks" ,128,7,0,0,0,0,0,0,NULL,&atx[3],NULL,0,&atx[13]} ,
  {0, "getdivs" ,128,8,0,0,0,0,0,0,NULL,&atx[3],NULL,0,&atx[14]} ,
  {0, "getgcs" ,128,9,0,0,0,0,0,0,NULL,&atx[3],NULL,0,&atx[15]} ,
  {0, "getlineage" ,128,10,0,0,0,0,0,0,NULL,&atx[10],NULL,0,&atx[16]} ,
  {0, "getchildren" ,128,11,0,0,0,0,0,0,NULL,&atx[10],NULL,0,&atx[17]} ,
  {0, "getbyid" ,128,12,0,0,0,0,0,0,NULL,&atx[10],NULL,0,&atx[18]} ,
  {0, "lookup" ,128,13,0,0,0,0,0,0,NULL,&atx[0],NULL,0,&atx[19]} ,
  {0, "getorgmod" ,128,14,0,0,0,0,0,0,NULL,&atx[20],NULL,0,&atx[25]} ,
  {403, "Taxon1-info" ,1,0,0,0,0,0,0,0,NULL,&atx[24],&atx[21],0,&atx[34]} ,
  {0, "ival1" ,128,0,0,0,0,0,0,0,NULL,&atx[10],NULL,0,&atx[22]} ,
  {0, "ival2" ,128,1,0,0,0,0,0,0,NULL,&atx[10],NULL,0,&atx[23]} ,
  {0, "sval" ,128,2,0,1,0,0,0,0,NULL,&atx[5],NULL,0,NULL} ,
  {311, "SEQUENCE" ,0,16,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "fini" ,128,15,0,0,0,0,0,0,NULL,&atx[3],NULL,0,&atx[26]} ,
  {0, "id4gi" ,128,16,0,0,0,0,0,0,NULL,&atx[10],NULL,0,&atx[27]} ,
  {0, "taxachildren" ,128,17,0,0,0,0,0,0,NULL,&atx[10],NULL,0,&atx[28]} ,
  {0, "taxalineage" ,128,18,0,0,0,0,0,0,NULL,&atx[10],NULL,0,&atx[29]} ,
  {0, "maxtaxid" ,128,19,0,0,0,0,0,0,NULL,&atx[3],NULL,0,&atx[30]} ,
  {0, "getproptypes" ,128,20,0,0,0,0,0,0,NULL,&atx[3],NULL,0,&atx[31]} ,
  {0, "getorgprop" ,128,21,0,0,0,0,0,0,NULL,&atx[20],NULL,0,&atx[32]} ,
  {0, "searchname" ,128,22,0,0,0,0,0,0,NULL,&atx[20],NULL,0,NULL} ,
  {315, "CHOICE" ,0,-1,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {404, "Taxon1-resp" ,1,0,0,0,0,0,0,0,NULL,&atx[33],&atx[35],0,&atx[36]} ,
  {0, "error" ,128,0,0,0,0,0,0,0,NULL,&atx[36],NULL,0,&atx[40]} ,
  {405, "Taxon1-error" ,1,0,0,0,0,0,0,0,NULL,&atx[24],&atx[37],0,&atx[43]} ,
  {0, "level" ,128,0,0,0,0,0,0,0,NULL,&atx[38],&avnx[0],0,&atx[39]} ,
  {310, "ENUMERATED" ,0,10,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "msg" ,128,1,0,1,0,0,0,0,NULL,&atx[5],NULL,0,NULL} ,
  {0, "init" ,128,1,0,0,0,0,0,0,NULL,&atx[3],NULL,0,&atx[41]} ,
  {0, "findname" ,128,2,0,0,0,0,0,0,NULL,&atx[48],&atx[42],0,&atx[49]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[43],NULL,0,NULL} ,
  {406, "Taxon1-name" ,1,0,0,0,0,0,0,0,NULL,&atx[24],&atx[44],0,&atx[67]} ,
  {0, "taxid" ,128,0,0,0,0,0,0,0,NULL,&atx[10],NULL,0,&atx[45]} ,
  {0, "cde" ,128,1,0,0,0,0,0,0,NULL,&atx[10],NULL,0,&atx[46]} ,
  {0, "oname" ,128,2,0,1,0,0,0,0,NULL,&atx[5],NULL,0,&atx[47]} ,
  {0, "uname" ,128,3,0,1,0,0,0,0,NULL,&atx[5],NULL,0,NULL} ,
  {314, "SET OF" ,0,17,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "getdesignator" ,128,3,0,0,0,0,0,0,NULL,&atx[10],NULL,0,&atx[50]} ,
  {0, "getunique" ,128,4,0,0,0,0,0,0,NULL,&atx[10],NULL,0,&atx[51]} ,
  {0, "getidbyorg" ,128,5,0,0,0,0,0,0,NULL,&atx[10],NULL,0,&atx[52]} ,
  {0, "getorgnames" ,128,6,0,0,0,0,0,0,NULL,&atx[48],&atx[53],0,&atx[54]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[43],NULL,0,NULL} ,
  {0, "getcde" ,128,7,0,0,0,0,0,0,NULL,&atx[48],&atx[55],0,&atx[56]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[20],NULL,0,NULL} ,
  {0, "getranks" ,128,8,0,0,0,0,0,0,NULL,&atx[48],&atx[57],0,&atx[58]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[20],NULL,0,NULL} ,
  {0, "getdivs" ,128,9,0,0,0,0,0,0,NULL,&atx[48],&atx[59],0,&atx[60]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[20],NULL,0,NULL} ,
  {0, "getgcs" ,128,10,0,0,0,0,0,0,NULL,&atx[48],&atx[61],0,&atx[62]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[20],NULL,0,NULL} ,
  {0, "getlineage" ,128,11,0,0,0,0,0,0,NULL,&atx[48],&atx[63],0,&atx[64]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[20],NULL,0,NULL} ,
  {0, "getchildren" ,128,12,0,0,0,0,0,0,NULL,&atx[48],&atx[65],0,&atx[66]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[20],NULL,0,NULL} ,
  {0, "getbyid" ,128,13,0,0,0,0,0,0,NULL,&atx[67],NULL,0,&atx[73]} ,
  {407, "Taxon1-data" ,1,0,0,0,0,0,0,0,NULL,&atx[24],&atx[68],0,&atx[79]} ,
  {0, "org" ,128,0,0,1,0,0,0,0,NULL,&atx[0],NULL,0,&atx[69]} ,
  {0, "div" ,128,1,0,0,0,0,0,0,NULL,&atx[5],NULL,0,&atx[70]} ,
  {0, "embl-code" ,128,2,0,1,0,0,0,0,NULL,&atx[5],NULL,0,&atx[71]} ,
  {0, "is-species-level" ,128,3,0,0,0,0,0,0,NULL,&atx[72],NULL,0,NULL} ,
  {301, "BOOLEAN" ,0,1,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "lookup" ,128,14,0,0,0,0,0,0,NULL,&atx[67],NULL,0,&atx[74]} ,
  {0, "getorgmod" ,128,15,0,0,0,0,0,0,NULL,&atx[48],&atx[75],0,&atx[76]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[20],NULL,0,NULL} ,
  {0, "fini" ,128,16,0,0,0,0,0,0,NULL,&atx[3],NULL,0,&atx[77]} ,
  {0, "id4gi" ,128,17,0,0,0,0,0,0,NULL,&atx[10],NULL,0,&atx[78]} ,
  {0, "taxabyid" ,128,18,0,0,0,0,0,0,NULL,&atx[79],NULL,0,&atx[86]} ,
  {408, "Taxon2-data" ,1,0,0,0,0,0,0,0,NULL,&atx[24],&atx[80],0,NULL} ,
  {0, "org" ,128,0,0,1,0,0,0,0,NULL,&atx[0],NULL,0,&atx[81]} ,
  {0, "blast-name" ,128,1,0,1,0,0,0,0,NULL,&atx[83],&atx[82],0,&atx[84]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[5],NULL,0,NULL} ,
  {312, "SEQUENCE OF" ,0,16,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "is-uncultured" ,128,2,0,0,0,0,0,0,NULL,&atx[72],NULL,0,&atx[85]} ,
  {0, "is-species-level" ,128,3,0,0,0,0,0,0,NULL,&atx[72],NULL,0,NULL} ,
  {0, "taxachildren" ,128,19,0,0,0,0,0,0,NULL,&atx[83],&atx[87],0,&atx[88]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[43],NULL,0,NULL} ,
  {0, "taxalineage" ,128,20,0,0,0,0,0,0,NULL,&atx[83],&atx[89],0,&atx[90]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[43],NULL,0,NULL} ,
  {0, "maxtaxid" ,128,21,0,0,0,0,0,0,NULL,&atx[10],NULL,0,&atx[91]} ,
  {0, "getproptypes" ,128,22,0,0,0,0,0,0,NULL,&atx[48],&atx[92],0,&atx[93]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[20],NULL,0,NULL} ,
  {0, "getorgprop" ,128,23,0,0,0,0,0,0,NULL,&atx[48],&atx[94],0,&atx[95]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[20],NULL,0,NULL} ,
  {0, "searchname" ,128,24,0,0,0,0,0,0,NULL,&atx[48],&atx[96],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[43],NULL,0,NULL} };

static AsnModule ampx[1] = {
  { "NCBI-Taxon1" , "taxon1.h66",&atx[0],NULL,NULL,0,0} };

static AsnValxNodePtr avn = avnx;
static AsnTypePtr at = atx;
static AsnModulePtr amp = ampx;



/**************************************************
*
*    Defines for Module NCBI-Taxon1
*
**************************************************/

#define TAXON1_REQ &at[1]
#define TAXON1_REQ_init &at[2]
#define TAXON1_REQ_findname &at[4]
#define TAXON1_REQ_getdesignator &at[6]
#define TAXON1_REQ_getunique &at[7]
#define TAXON1_REQ_getidbyorg &at[8]
#define TAXON1_REQ_getorgnames &at[9]
#define TAXON1_REQ_getcde &at[11]
#define TAXON1_REQ_getranks &at[12]
#define TAXON1_REQ_getdivs &at[13]
#define TAXON1_REQ_getgcs &at[14]
#define TAXON1_REQ_getlineage &at[15]
#define TAXON1_REQ_getchildren &at[16]
#define TAXON1_REQ_getbyid &at[17]
#define TAXON1_REQ_lookup &at[18]
#define TAXON1_REQ_getorgmod &at[19]
#define TAXON1_REQ_fini &at[25]
#define TAXON1_REQ_id4gi &at[26]
#define TAXON1_REQ_taxachildren &at[27]
#define TAXON1_REQ_taxalineage &at[28]
#define TAXON1_REQ_maxtaxid &at[29]
#define TAXON1_REQ_getproptypes &at[30]
#define TAXON1_REQ_getorgprop &at[31]
#define TAXON1_REQ_searchname &at[32]

#define TAXON1_INFO &at[20]
#define TAXON1_INFO_ival1 &at[21]
#define TAXON1_INFO_ival2 &at[22]
#define TAXON1_INFO_sval &at[23]

#define TAXON1_RESP &at[34]
#define TAXON1_RESP_error &at[35]
#define TAXON1_RESP_init &at[40]
#define TAXON1_RESP_findname &at[41]
#define TAXON1_RESP_findname_E &at[42]
#define TAXON1_RESP_getdesignator &at[49]
#define TAXON1_RESP_getunique &at[50]
#define TAXON1_RESP_getidbyorg &at[51]
#define TAXON1_RESP_getorgnames &at[52]
#define TAXON1_RESP_getorgnames_E &at[53]
#define TAXON1_RESP_getcde &at[54]
#define TAXON1_RESP_getcde_E &at[55]
#define TAXON1_RESP_getranks &at[56]
#define TAXON1_RESP_getranks_E &at[57]
#define TAXON1_RESP_getdivs &at[58]
#define TAXON1_RESP_getdivs_E &at[59]
#define TAXON1_RESP_getgcs &at[60]
#define TAXON1_RESP_getgcs_E &at[61]
#define TAXON1_RESP_getlineage &at[62]
#define TAXON1_RESP_getlineage_E &at[63]
#define TAXON1_RESP_getchildren &at[64]
#define TAXON1_RESP_getchildren_E &at[65]
#define TAXON1_RESP_getbyid &at[66]
#define TAXON1_RESP_lookup &at[73]
#define TAXON1_RESP_getorgmod &at[74]
#define TAXON1_RESP_getorgmod_E &at[75]
#define TAXON1_RESP_fini &at[76]
#define TAXON1_RESP_id4gi &at[77]
#define TAXON1_RESP_taxabyid &at[78]
#define TAXON1_RESP_taxachildren &at[86]
#define TAXON1_RESP_taxachildren_E &at[87]
#define TAXON1_RESP_taxalineage &at[88]
#define TAXON1_RESP_taxalineage_E &at[89]
#define TAXON1_RESP_maxtaxid &at[90]
#define TAXON1_RESP_getproptypes &at[91]
#define TAXON1_RESP_getproptypes_E &at[92]
#define TAXON1_RESP_getorgprop &at[93]
#define TAXON1_RESP_getorgprop_E &at[94]
#define TAXON1_RESP_searchname &at[95]
#define TAXON1_RESP_searchname_E &at[96]

#define TAXON1_ERROR &at[36]
#define TAXON1_ERROR_level &at[37]
#define TAXON1_ERROR_msg &at[39]

#define TAXON1_NAME &at[43]
#define TAXON1_NAME_taxid &at[44]
#define TAXON1_NAME_cde &at[45]
#define TAXON1_NAME_oname &at[46]
#define TAXON1_NAME_uname &at[47]

#define TAXON1_DATA &at[67]
#define TAXON1_DATA_org &at[68]
#define TAXON1_DATA_div &at[69]
#define TAXON1_DATA_embl_code &at[70]
#define TAXON1_DATA_is_species_level &at[71]

#define TAXON2_DATA &at[79]
#define TAXON2_DATA_org &at[80]
#define TAXON2_DATA_blast_name &at[81]
#define TAXON2_DATA_blast_name_E &at[82]
#define TAXON2_DATA_is_uncultured &at[84]
#define TAXON2_DATA_is_species_level &at[85]
