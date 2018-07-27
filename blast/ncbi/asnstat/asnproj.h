/***********************************************************************
*
**
*        Automatic header module from ASNTOOL
*
************************************************************************/

#ifndef _ASNTOOL_
#include <asn.h>
#endif

static char * asnfilename = "asnproj.h63";
static AsnType atx[70] = {
  {401, "Project" ,1,0,0,0,0,1,0,0,NULL,&atx[19],&atx[1],0,&atx[21]} ,
  {0, "descr" ,128,0,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[20]} ,
  {411, "Project-descr" ,1,0,0,0,0,0,0,0,NULL,&atx[19],&atx[3],0,&atx[5]} ,
  {0, "id" ,128,0,0,0,0,0,0,0,NULL,&atx[7],&atx[4],0,&atx[8]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[5],NULL,0,NULL} ,
  {412, "Project-id" ,1,0,0,0,0,0,0,0,NULL,&atx[6],NULL,0,&atx[11]} ,
  {323, "VisibleString" ,0,26,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {314, "SET OF" ,0,17,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "name" ,128,1,0,1,0,0,0,0,NULL,&atx[6],NULL,0,&atx[9]} ,
  {0, "descr" ,128,2,0,1,0,0,0,0,NULL,&atx[7],&atx[10],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[11],NULL,0,NULL} ,
  {413, "Projdesc" ,1,0,0,0,0,0,0,0,NULL,&atx[18],&atx[12],0,NULL} ,
  {0, "pub" ,128,0,0,0,0,0,0,0,NULL,&atx[13],NULL,0,&atx[14]} ,
  {408, "Pubdesc" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[54]} ,
  {0, "date" ,128,1,0,0,0,0,0,0,NULL,&atx[15],NULL,0,&atx[16]} ,
  {403, "Date" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[37]} ,
  {0, "comment" ,128,2,0,0,0,0,0,0,NULL,&atx[6],NULL,0,&atx[17]} ,
  {0, "title" ,128,3,0,0,0,0,0,0,NULL,&atx[6],NULL,0,NULL} ,
  {315, "CHOICE" ,0,-1,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {311, "SEQUENCE" ,0,16,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "data" ,128,1,0,0,0,0,0,0,NULL,&atx[21],NULL,0,NULL} ,
  {402, "Project-item" ,1,0,0,0,0,1,0,0,NULL,&atx[18],&atx[22],0,&atx[15]} ,
  {0, "pmuid" ,128,0,0,0,0,0,0,0,NULL,&atx[7],&atx[23],0,&atx[25]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[24],NULL,0,NULL} ,
  {302, "INTEGER" ,0,2,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "protuid" ,128,1,0,0,0,0,0,0,NULL,&atx[7],&atx[26],0,&atx[27]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[24],NULL,0,NULL} ,
  {0, "nucuid" ,128,2,0,0,0,0,0,0,NULL,&atx[7],&atx[28],0,&atx[29]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[24],NULL,0,NULL} ,
  {0, "sequid" ,128,3,0,0,0,0,0,0,NULL,&atx[7],&atx[30],0,&atx[31]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[24],NULL,0,NULL} ,
  {0, "genomeuid" ,128,4,0,0,0,0,0,0,NULL,&atx[7],&atx[32],0,&atx[33]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[24],NULL,0,NULL} ,
  {0, "structuid" ,128,5,0,0,0,0,0,0,NULL,&atx[7],&atx[34],0,&atx[35]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[24],NULL,0,NULL} ,
  {0, "pmid" ,128,6,0,0,0,0,0,0,NULL,&atx[7],&atx[36],0,&atx[38]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[37],NULL,0,NULL} ,
  {404, "PubMedId" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[40]} ,
  {0, "protid" ,128,7,0,0,0,0,0,0,NULL,&atx[7],&atx[39],0,&atx[41]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[40],NULL,0,NULL} ,
  {405, "Seq-id" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[67]} ,
  {0, "nucid" ,128,8,0,0,0,0,0,0,NULL,&atx[7],&atx[42],0,&atx[43]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[40],NULL,0,NULL} ,
  {0, "seqid" ,128,9,0,0,0,0,0,0,NULL,&atx[7],&atx[44],0,&atx[45]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[40],NULL,0,NULL} ,
  {0, "genomeid" ,128,10,0,0,0,0,0,0,NULL,&atx[7],&atx[46],0,&atx[47]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[40],NULL,0,NULL} ,
  {0, "structid" ,128,11,0,0,0,0,0,0,NULL,&atx[48],NULL,0,&atx[49]} ,
  {305, "NULL" ,0,5,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "pment" ,128,12,0,0,0,0,0,0,NULL,&atx[7],&atx[50],0,&atx[52]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[51],NULL,0,NULL} ,
  {410, "Pubmed-entry" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[2]} ,
  {0, "protent" ,128,13,0,0,0,0,0,0,NULL,&atx[7],&atx[53],0,&atx[55]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[54],NULL,0,NULL} ,
  {409, "Seq-entry" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[51]} ,
  {0, "nucent" ,128,14,0,0,0,0,0,0,NULL,&atx[7],&atx[56],0,&atx[57]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[54],NULL,0,NULL} ,
  {0, "seqent" ,128,15,0,0,0,0,0,0,NULL,&atx[7],&atx[58],0,&atx[59]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[54],NULL,0,NULL} ,
  {0, "genomeent" ,128,16,0,0,0,0,0,0,NULL,&atx[7],&atx[60],0,&atx[61]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[54],NULL,0,NULL} ,
  {0, "structent" ,128,17,0,0,0,0,0,0,NULL,&atx[48],NULL,0,&atx[62]} ,
  {0, "seqannot" ,128,18,0,0,0,0,0,0,NULL,&atx[7],&atx[63],0,&atx[65]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[64],NULL,0,NULL} ,
  {407, "Seq-annot" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[13]} ,
  {0, "loc" ,128,19,0,0,0,0,0,0,NULL,&atx[7],&atx[66],0,&atx[68]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[67],NULL,0,NULL} ,
  {406, "Seq-loc" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[64]} ,
  {0, "proj" ,128,20,0,0,0,0,0,0,NULL,&atx[7],&atx[69],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[0],NULL,0,NULL} };

static AsnModule ampx[1] = {
  { "NCBI-Project" , "asnproj.h63",&atx[0],NULL,NULL,0,0} };

static AsnValxNodePtr avn = NULL;
static AsnTypePtr at = atx;
static AsnModulePtr amp = ampx;



/**************************************************
*
*    Defines for Module NCBI-Project
*
**************************************************/

#define PROJECT &at[0]
#define PROJECT_descr &at[1]
#define PROJECT_data &at[20]

#define PROJECT_ITEM &at[21]
#define PROJECT_ITEM_pmuid &at[22]
#define PROJECT_ITEM_pmuid_E &at[23]
#define PROJECT_ITEM_protuid &at[25]
#define PROJECT_ITEM_protuid_E &at[26]
#define PROJECT_ITEM_nucuid &at[27]
#define PROJECT_ITEM_nucuid_E &at[28]
#define PROJECT_ITEM_sequid &at[29]
#define PROJECT_ITEM_sequid_E &at[30]
#define PROJECT_ITEM_genomeuid &at[31]
#define PROJECT_ITEM_genomeuid_E &at[32]
#define PROJECT_ITEM_structuid &at[33]
#define PROJECT_ITEM_structuid_E &at[34]
#define PROJECT_ITEM_pmid &at[35]
#define PROJECT_ITEM_pmid_E &at[36]
#define PROJECT_ITEM_protid &at[38]
#define PROJECT_ITEM_protid_E &at[39]
#define PROJECT_ITEM_nucid &at[41]
#define PROJECT_ITEM_nucid_E &at[42]
#define PROJECT_ITEM_seqid &at[43]
#define PROJECT_ITEM_seqid_E &at[44]
#define PROJECT_ITEM_genomeid &at[45]
#define PROJECT_ITEM_genomeid_E &at[46]
#define PROJECT_ITEM_structid &at[47]
#define PROJECT_ITEM_pment &at[49]
#define PROJECT_ITEM_pment_E &at[50]
#define PROJECT_ITEM_protent &at[52]
#define PROJECT_ITEM_protent_E &at[53]
#define PROJECT_ITEM_nucent &at[55]
#define PROJECT_ITEM_nucent_E &at[56]
#define PROJECT_ITEM_seqent &at[57]
#define PROJECT_ITEM_seqent_E &at[58]
#define PROJECT_ITEM_genomeent &at[59]
#define PROJECT_ITEM_genomeent_E &at[60]
#define PROJECT_ITEM_structent &at[61]
#define PROJECT_ITEM_seqannot &at[62]
#define PROJECT_ITEM_seqannot_E &at[63]
#define PROJECT_ITEM_loc &at[65]
#define PROJECT_ITEM_loc_E &at[66]
#define PROJECT_ITEM_proj &at[68]
#define PROJECT_ITEM_proj_E &at[69]

#define PROJECT_DESCR &at[2]
#define PROJECT_DESCR_id &at[3]
#define PROJECT_DESCR_id_E &at[4]
#define PROJECT_DESCR_name &at[8]
#define PROJECT_DESCR_descr &at[9]
#define PROJECT_DESCR_descr_E &at[10]

#define PROJECT_ID &at[5]

#define PROJDESC &at[11]
#define PROJDESC_pub &at[12]
#define PROJDESC_date &at[14]
#define PROJDESC_comment &at[16]
#define PROJDESC_title &at[17]
