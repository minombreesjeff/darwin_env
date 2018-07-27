/***********************************************************************
*
**
*        Automatic header module from ASNTOOL
*
************************************************************************/

#ifndef _ASNTOOL_
#include <asn.h>
#endif

static char * asnfilename = "asnmedli.h60";
static AsnValxNode avnx[29] = {
    {2,NULL,0,0.0,NULL } ,
    {2,NULL,0,0.0,NULL } ,
    {20,"nameonly" ,0,0.0,&avnx[3] } ,
    {20,"cas" ,1,0.0,&avnx[4] } ,
    {20,"ec" ,2,0.0,NULL } ,
    {20,"ddbj" ,1,0.0,&avnx[6] } ,
    {20,"carbbank" ,2,0.0,&avnx[7] } ,
    {20,"embl" ,3,0.0,&avnx[8] } ,
    {20,"hdb" ,4,0.0,&avnx[9] } ,
    {20,"genbank" ,5,0.0,&avnx[10] } ,
    {20,"hgml" ,6,0.0,&avnx[11] } ,
    {20,"mim" ,7,0.0,&avnx[12] } ,
    {20,"msd" ,8,0.0,&avnx[13] } ,
    {20,"pdb" ,9,0.0,&avnx[14] } ,
    {20,"pir" ,10,0.0,&avnx[15] } ,
    {20,"prfseqdb" ,11,0.0,&avnx[16] } ,
    {20,"psd" ,12,0.0,&avnx[17] } ,
    {20,"swissprot" ,13,0.0,&avnx[18] } ,
    {20,"gdb" ,14,0.0,NULL } ,
    {20,"other" ,0,0.0,&avnx[20] } ,
    {20,"comment" ,1,0.0,&avnx[21] } ,
    {20,"erratum" ,2,0.0,NULL } ,
    {20,"medline" ,1,0.0,&avnx[23] } ,
    {20,"pubmed" ,2,0.0,&avnx[24] } ,
    {20,"ncbigi" ,3,0.0,NULL } ,
    {20,"publisher" ,1,0.0,&avnx[26] } ,
    {20,"premedline" ,2,0.0,&avnx[27] } ,
    {20,"medline" ,3,0.0,NULL } ,
    {3,NULL,3,0.0,NULL } };

static AsnType atx[54] = {
  {401, "Medline-entry" ,1,0,0,0,0,1,0,0,NULL,&atx[20],&atx[1],0,&atx[31]} ,
  {0, "uid" ,128,0,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[3]} ,
  {302, "INTEGER" ,0,2,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "em" ,128,1,0,0,0,0,0,0,NULL,&atx[4],NULL,0,&atx[5]} ,
  {405, "Date" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[11]} ,
  {0, "cit" ,128,2,0,0,0,0,0,0,NULL,&atx[6],NULL,0,&atx[7]} ,
  {403, "Cit-art" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[39]} ,
  {0, "abstract" ,128,3,0,1,0,0,0,0,NULL,&atx[8],NULL,0,&atx[9]} ,
  {323, "VisibleString" ,0,26,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "mesh" ,128,4,0,1,0,0,0,0,NULL,&atx[21],&atx[10],0,&atx[22]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[11],NULL,0,NULL} ,
  {406, "Medline-mesh" ,1,0,0,0,0,0,0,0,NULL,&atx[20],&atx[12],0,&atx[24]} ,
  {0, "mp" ,128,0,0,0,1,0,0,0,&avnx[0],&atx[13],NULL,0,&atx[14]} ,
  {301, "BOOLEAN" ,0,1,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "term" ,128,1,0,0,0,0,0,0,NULL,&atx[8],NULL,0,&atx[15]} ,
  {0, "qual" ,128,2,0,1,0,0,0,0,NULL,&atx[21],&atx[16],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[17],NULL,0,NULL} ,
  {409, "Medline-qual" ,1,0,0,0,0,0,0,0,NULL,&atx[20],&atx[18],0,&atx[49]} ,
  {0, "mp" ,128,0,0,0,1,0,0,0,&avnx[1],&atx[13],NULL,0,&atx[19]} ,
  {0, "subh" ,128,1,0,0,0,0,0,0,NULL,&atx[8],NULL,0,NULL} ,
  {311, "SEQUENCE" ,0,16,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {314, "SET OF" ,0,17,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "substance" ,128,5,0,1,0,0,0,0,NULL,&atx[21],&atx[23],0,&atx[29]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[24],NULL,0,NULL} ,
  {407, "Medline-rn" ,1,0,0,0,0,0,0,0,NULL,&atx[20],&atx[25],0,&atx[44]} ,
  {0, "type" ,128,0,0,0,0,0,0,0,NULL,&atx[26],&avnx[2],0,&atx[27]} ,
  {310, "ENUMERATED" ,0,10,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "cit" ,128,1,0,1,0,0,0,0,NULL,&atx[8],NULL,0,&atx[28]} ,
  {0, "name" ,128,2,0,0,0,0,0,0,NULL,&atx[8],NULL,0,NULL} ,
  {0, "xref" ,128,6,0,1,0,0,0,0,NULL,&atx[21],&atx[30],0,&atx[34]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[31],NULL,0,NULL} ,
  {402, "Medline-si" ,1,0,0,0,0,1,0,0,NULL,&atx[20],&atx[32],0,&atx[6]} ,
  {0, "type" ,128,0,0,0,0,0,0,0,NULL,&atx[26],&avnx[5],0,&atx[33]} ,
  {0, "cit" ,128,1,0,1,0,0,0,0,NULL,&atx[8],NULL,0,NULL} ,
  {0, "idnum" ,128,7,0,1,0,0,0,0,NULL,&atx[21],&atx[35],0,&atx[36]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[8],NULL,0,NULL} ,
  {0, "gene" ,128,8,0,1,0,0,0,0,NULL,&atx[21],&atx[37],0,&atx[38]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[8],NULL,0,NULL} ,
  {0, "pmid" ,128,9,0,1,0,0,0,0,NULL,&atx[39],NULL,0,&atx[40]} ,
  {404, "PubMedId" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[4]} ,
  {0, "pub-type" ,128,10,0,1,0,0,0,0,NULL,&atx[21],&atx[41],0,&atx[42]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[8],NULL,0,NULL} ,
  {0, "mlfield" ,128,11,0,1,0,0,0,0,NULL,&atx[21],&atx[43],0,&atx[53]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[44],NULL,0,NULL} ,
  {408, "Medline-field" ,1,0,0,0,0,0,0,0,NULL,&atx[20],&atx[45],0,&atx[17]} ,
  {0, "type" ,128,0,0,0,0,0,0,0,NULL,&atx[2],&avnx[19],0,&atx[46]} ,
  {0, "str" ,128,1,0,0,0,0,0,0,NULL,&atx[8],NULL,0,&atx[47]} ,
  {0, "ids" ,128,2,0,1,0,0,0,0,NULL,&atx[52],&atx[48],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[49],NULL,0,NULL} ,
  {410, "DocRef" ,1,0,0,0,0,0,0,0,NULL,&atx[20],&atx[50],0,NULL} ,
  {0, "type" ,128,0,0,0,0,0,0,0,NULL,&atx[2],&avnx[22],0,&atx[51]} ,
  {0, "uid" ,128,1,0,0,0,0,0,0,NULL,&atx[2],NULL,0,NULL} ,
  {312, "SEQUENCE OF" ,0,16,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "status" ,128,12,0,0,1,0,0,0,&avnx[28],&atx[2],&avnx[25],0,NULL} };

static AsnModule ampx[1] = {
  { "NCBI-Medline" , "asnmedli.h60",&atx[0],NULL,NULL,0,0} };

static AsnValxNodePtr avn = avnx;
static AsnTypePtr at = atx;
static AsnModulePtr amp = ampx;



/**************************************************
*
*    Defines for Module NCBI-Medline
*
**************************************************/

#define MEDLINE_ENTRY &at[0]
#define MEDLINE_ENTRY_uid &at[1]
#define MEDLINE_ENTRY_em &at[3]
#define MEDLINE_ENTRY_cit &at[5]
#define MEDLINE_ENTRY_abstract &at[7]
#define MEDLINE_ENTRY_mesh &at[9]
#define MEDLINE_ENTRY_mesh_E &at[10]
#define MEDLINE_ENTRY_substance &at[22]
#define MEDLINE_ENTRY_substance_E &at[23]
#define MEDLINE_ENTRY_xref &at[29]
#define MEDLINE_ENTRY_xref_E &at[30]
#define MEDLINE_ENTRY_idnum &at[34]
#define MEDLINE_ENTRY_idnum_E &at[35]
#define MEDLINE_ENTRY_gene &at[36]
#define MEDLINE_ENTRY_gene_E &at[37]
#define MEDLINE_ENTRY_pmid &at[38]
#define MEDLINE_ENTRY_pub_type &at[40]
#define MEDLINE_ENTRY_pub_type_E &at[41]
#define MEDLINE_ENTRY_mlfield &at[42]
#define MEDLINE_ENTRY_mlfield_E &at[43]
#define MEDLINE_ENTRY_status &at[53]

#define MEDLINE_SI &at[31]
#define MEDLINE_SI_type &at[32]
#define MEDLINE_SI_cit &at[33]

#define MEDLINE_MESH &at[11]
#define MEDLINE_MESH_mp &at[12]
#define MEDLINE_MESH_term &at[14]
#define MEDLINE_MESH_qual &at[15]
#define MEDLINE_MESH_qual_E &at[16]

#define MEDLINE_RN &at[24]
#define MEDLINE_RN_type &at[25]
#define MEDLINE_RN_cit &at[27]
#define MEDLINE_RN_name &at[28]

#define MEDLINE_FIELD &at[44]
#define MEDLINE_FIELD_type &at[45]
#define MEDLINE_FIELD_str &at[46]
#define MEDLINE_FIELD_ids &at[47]
#define MEDLINE_FIELD_ids_E &at[48]

#define MEDLINE_QUAL &at[17]
#define MEDLINE_QUAL_mp &at[18]
#define MEDLINE_QUAL_subh &at[19]

#define DOCREF &at[49]
#define DOCREF_type &at[50]
#define DOCREF_uid &at[51]
