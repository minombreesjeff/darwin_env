/***********************************************************************
*
**
*        Automatic header module from ASNTOOL
*
************************************************************************/

#ifndef _ASNTOOL_
#include <asn.h>
#endif

static char * asnfilename = "asnsset.h62";
static AsnValxNode avnx[21] = {
    {20,"not-set" ,0,0.0,&avnx[1] } ,
    {20,"nuc-prot" ,1,0.0,&avnx[2] } ,
    {20,"segset" ,2,0.0,&avnx[3] } ,
    {20,"conset" ,3,0.0,&avnx[4] } ,
    {20,"parts" ,4,0.0,&avnx[5] } ,
    {20,"gibb" ,5,0.0,&avnx[6] } ,
    {20,"gi" ,6,0.0,&avnx[7] } ,
    {20,"genbank" ,7,0.0,&avnx[8] } ,
    {20,"pir" ,8,0.0,&avnx[9] } ,
    {20,"pub-set" ,9,0.0,&avnx[10] } ,
    {20,"equiv" ,10,0.0,&avnx[11] } ,
    {20,"swissprot" ,11,0.0,&avnx[12] } ,
    {20,"pdb-entry" ,12,0.0,&avnx[13] } ,
    {20,"mut-set" ,13,0.0,&avnx[14] } ,
    {20,"pop-set" ,14,0.0,&avnx[15] } ,
    {20,"phy-set" ,15,0.0,&avnx[16] } ,
    {20,"eco-set" ,16,0.0,&avnx[17] } ,
    {20,"gen-prod-set" ,17,0.0,&avnx[18] } ,
    {20,"wgs-set" ,18,0.0,&avnx[19] } ,
    {20,"other" ,255,0.0,NULL } ,
    {3,NULL,0,0.0,NULL } };

static AsnType atx[28] = {
  {401, "Bioseq-set" ,1,0,0,0,0,1,0,0,NULL,&atx[27],&atx[1],0,&atx[17]} ,
  {0, "id" ,128,0,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[3]} ,
  {406, "Object-id" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[4]} ,
  {0, "coll" ,128,1,0,1,0,0,0,0,NULL,&atx[4],NULL,0,&atx[5]} ,
  {407, "Dbtag" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[12]} ,
  {0, "level" ,128,2,0,1,0,0,0,0,NULL,&atx[6],NULL,0,&atx[7]} ,
  {302, "INTEGER" ,0,2,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "class" ,128,3,0,0,1,0,0,0,&avnx[20],&atx[8],&avnx[0],0,&atx[9]} ,
  {310, "ENUMERATED" ,0,10,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "release" ,128,4,0,1,0,0,0,0,NULL,&atx[10],NULL,0,&atx[11]} ,
  {323, "VisibleString" ,0,26,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "date" ,128,5,0,1,0,0,0,0,NULL,&atx[12],NULL,0,&atx[13]} ,
  {408, "Date" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,NULL} ,
  {0, "descr" ,128,6,0,1,0,0,0,0,NULL,&atx[14],NULL,0,&atx[15]} ,
  {405, "Seq-descr" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[2]} ,
  {0, "seq-set" ,128,7,0,0,0,0,0,0,NULL,&atx[22],&atx[16],0,&atx[23]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[17],NULL,0,NULL} ,
  {402, "Seq-entry" ,1,0,0,0,0,1,0,0,NULL,&atx[21],&atx[18],0,&atx[19]} ,
  {0, "seq" ,128,0,0,0,0,0,0,0,NULL,&atx[19],NULL,0,&atx[20]} ,
  {403, "Bioseq" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[25]} ,
  {0, "set" ,128,1,0,0,0,0,0,0,NULL,&atx[0],NULL,0,NULL} ,
  {315, "CHOICE" ,0,-1,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {312, "SEQUENCE OF" ,0,16,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "annot" ,128,8,0,1,0,0,0,0,NULL,&atx[26],&atx[24],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[25],NULL,0,NULL} ,
  {404, "Seq-annot" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[14]} ,
  {314, "SET OF" ,0,17,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {311, "SEQUENCE" ,0,16,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} };

static AsnModule ampx[1] = {
  { "NCBI-Seqset" , "asnsset.h62",&atx[0],NULL,NULL,0,0} };

static AsnValxNodePtr avn = avnx;
static AsnTypePtr at = atx;
static AsnModulePtr amp = ampx;



/**************************************************
*
*    Defines for Module NCBI-Seqset
*
**************************************************/

#define BIOSEQ_SET &at[0]
#define BIOSEQ_SET_id &at[1]
#define BIOSEQ_SET_coll &at[3]
#define BIOSEQ_SET_level &at[5]
#define BIOSEQ_SET_class &at[7]
#define BIOSEQ_SET_release &at[9]
#define BIOSEQ_SET_date &at[11]
#define BIOSEQ_SET_descr &at[13]
#define BIOSEQ_SET_seq_set &at[15]
#define BIOSEQ_SET_seq_set_E &at[16]
#define BIOSEQ_SET_annot &at[23]
#define BIOSEQ_SET_annot_E &at[24]

#define SEQ_ENTRY &at[17]
#define SEQ_ENTRY_seq &at[18]
#define SEQ_ENTRY_set &at[20]
