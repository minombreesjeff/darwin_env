/***********************************************************************
*
**
*        Automatic header module from ASNTOOL
*
************************************************************************/

#ifndef _ASNTOOL_
#include <asn.h>
#endif

static char * asnfilename = "asntax3.h12";
static AsnValxNode avnx[5] = {
    {20,"none" ,0,0.0,&avnx[1] } ,
    {20,"info" ,1,0.0,&avnx[2] } ,
    {20,"warn" ,2,0.0,&avnx[3] } ,
    {20,"error" ,3,0.0,&avnx[4] } ,
    {20,"fatal" ,4,0.0,NULL } };

static AsnType atx[48] = {
  {401, "Org-ref" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[1]} ,
  {402, "Taxon3-request" ,1,0,0,0,0,0,0,0,NULL,&atx[12],&atx[2],0,&atx[4]} ,
  {0, "request" ,128,0,0,0,0,0,0,0,NULL,&atx[11],&atx[3],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[4],NULL,0,NULL} ,
  {403, "T3Request" ,1,0,0,0,0,0,0,0,NULL,&atx[10],&atx[5],0,&atx[13]} ,
  {0, "taxid" ,128,0,0,0,0,0,0,0,NULL,&atx[6],NULL,0,&atx[7]} ,
  {302, "INTEGER" ,0,2,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "name" ,128,1,0,0,0,0,0,0,NULL,&atx[8],NULL,0,&atx[9]} ,
  {323, "VisibleString" ,0,26,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "org" ,128,2,0,0,0,0,0,0,NULL,&atx[0],NULL,0,NULL} ,
  {315, "CHOICE" ,0,-1,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {312, "SEQUENCE OF" ,0,16,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {311, "SEQUENCE" ,0,16,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {404, "Taxon3-reply" ,1,0,0,0,0,0,0,0,NULL,&atx[12],&atx[14],0,&atx[16]} ,
  {0, "reply" ,128,0,0,0,0,0,0,0,NULL,&atx[11],&atx[15],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[16],NULL,0,NULL} ,
  {405, "T3Reply" ,1,0,0,0,0,0,0,0,NULL,&atx[10],&atx[17],0,&atx[18]} ,
  {0, "error" ,128,0,0,0,0,0,0,0,NULL,&atx[18],NULL,0,&atx[25]} ,
  {406, "T3Error" ,1,0,0,0,0,0,0,0,NULL,&atx[12],&atx[19],0,&atx[26]} ,
  {0, "level" ,128,0,0,0,0,0,0,0,NULL,&atx[20],&avnx[0],0,&atx[21]} ,
  {310, "ENUMERATED" ,0,10,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "message" ,128,1,0,0,0,0,0,0,NULL,&atx[8],NULL,0,&atx[22]} ,
  {0, "taxid" ,128,2,0,1,0,0,0,0,NULL,&atx[6],NULL,0,&atx[23]} ,
  {0, "name" ,128,3,0,1,0,0,0,0,NULL,&atx[8],NULL,0,&atx[24]} ,
  {0, "org" ,128,4,0,1,0,0,0,0,NULL,&atx[0],NULL,0,NULL} ,
  {0, "data" ,128,1,0,0,0,0,0,0,NULL,&atx[26],NULL,0,NULL} ,
  {407, "T3Data" ,1,0,0,0,0,0,0,0,NULL,&atx[12],&atx[27],0,&atx[32]} ,
  {0, "org" ,128,0,0,0,0,0,0,0,NULL,&atx[0],NULL,0,&atx[28]} ,
  {0, "blast-name-lineage" ,128,1,0,1,0,0,0,0,NULL,&atx[11],&atx[29],0,&atx[30]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[8],NULL,0,NULL} ,
  {0, "status" ,128,2,0,1,0,0,0,0,NULL,&atx[11],&atx[31],0,&atx[39]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[32],NULL,0,NULL} ,
  {408, "T3StatusFlags" ,1,0,0,0,0,0,0,0,NULL,&atx[12],&atx[33],0,&atx[40]} ,
  {0, "property" ,128,0,0,0,0,0,0,0,NULL,&atx[8],NULL,0,&atx[34]} ,
  {0, "value" ,128,1,0,0,0,0,0,0,NULL,&atx[10],&atx[35],0,NULL} ,
  {0, "bool" ,128,0,0,0,0,0,0,0,NULL,&atx[36],NULL,0,&atx[37]} ,
  {301, "BOOLEAN" ,0,1,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "int" ,128,1,0,0,0,0,0,0,NULL,&atx[6],NULL,0,&atx[38]} ,
  {0, "str" ,128,2,0,0,0,0,0,0,NULL,&atx[8],NULL,0,NULL} ,
  {0, "refresh" ,128,3,0,1,0,0,0,0,NULL,&atx[40],NULL,0,NULL} ,
  {409, "T3RefreshFlags" ,1,0,0,0,0,0,0,0,NULL,&atx[12],&atx[41],0,NULL} ,
  {0, "taxid-changed" ,128,0,0,0,0,0,0,0,NULL,&atx[36],NULL,0,&atx[42]} ,
  {0, "taxname-changed" ,128,1,0,0,0,0,0,0,NULL,&atx[36],NULL,0,&atx[43]} ,
  {0, "division-changed" ,128,2,0,0,0,0,0,0,NULL,&atx[36],NULL,0,&atx[44]} ,
  {0, "lineage-changed" ,128,3,0,0,0,0,0,0,NULL,&atx[36],NULL,0,&atx[45]} ,
  {0, "gc-changed" ,128,4,0,0,0,0,0,0,NULL,&atx[36],NULL,0,&atx[46]} ,
  {0, "mgc-changed" ,128,5,0,0,0,0,0,0,NULL,&atx[36],NULL,0,&atx[47]} ,
  {0, "orgmod-changed" ,128,6,0,0,0,0,0,0,NULL,&atx[36],NULL,0,NULL} };

static AsnModule ampx[1] = {
  { "NCBI-Taxon3" , "asntax3.h12",&atx[0],NULL,NULL,0,0} };

static AsnValxNodePtr avn = avnx;
static AsnTypePtr at = atx;
static AsnModulePtr amp = ampx;



/**************************************************
*
*    Defines for Module NCBI-Taxon3
*
**************************************************/

#define TAXON3_REQUEST &at[1]
#define TAXON3_REQUEST_request &at[2]
#define TAXON3_REQUEST_request_E &at[3]

#define T3REQUEST &at[4]
#define T3REQUEST_taxid &at[5]
#define T3REQUEST_name &at[7]
#define T3REQUEST_org &at[9]

#define TAXON3_REPLY &at[13]
#define TAXON3_REPLY_reply &at[14]
#define TAXON3_REPLY_reply_E &at[15]

#define T3REPLY &at[16]
#define T3REPLY_error &at[17]
#define T3REPLY_data &at[25]

#define T3ERROR &at[18]
#define T3ERROR_level &at[19]
#define T3ERROR_message &at[21]
#define T3ERROR_taxid &at[22]
#define T3ERROR_name &at[23]
#define T3ERROR_org &at[24]

#define T3DATA &at[26]
#define T3DATA_org &at[27]
#define T3DATA_blast_name_lineage &at[28]
#define T3DATA_blast_name_lineage_E &at[29]
#define T3DATA_status &at[30]
#define T3DATA_status_E &at[31]
#define T3DATA_refresh &at[39]

#define T3STATUSFLAGS &at[32]
#define T3STATUSFLAGS_property &at[33]
#define T3STATUSFLAGS_value &at[34]
#define T3STATUSFLAGS_value_bool &at[35]
#define T3STATUSFLAGS_value_int &at[37]
#define T3STATUSFLAGS_value_str &at[38]

#define T3REFRESHFLAGS &at[40]
#define T3REFRESHFLAGS_taxid_changed &at[41]
#define T3REFRESHFLAGS_taxname_changed &at[42]
#define T3REFRESHFLAGS_division_changed &at[43]
#define T3REFRESHFLAGS_lineage_changed &at[44]
#define T3REFRESHFLAGS_gc_changed &at[45]
#define T3REFRESHFLAGS_mgc_changed &at[46]
#define T3REFRESHFLAGS_orgmod_changed &at[47]
