/***********************************************************************
*
**
*        Automatic header module from ASNTOOL
*
************************************************************************/

#ifndef _ASNTOOL_
#include <asn.h>
#endif

static char * asnfilename = "asnblock.h60";
static AsnValxNode avnx[45] = {
    {20,"embl" ,0,0.0,&avnx[1] } ,
    {20,"genbank" ,1,0.0,&avnx[2] } ,
    {20,"ddbj" ,2,0.0,&avnx[3] } ,
    {20,"geninfo" ,3,0.0,&avnx[4] } ,
    {20,"medline" ,4,0.0,&avnx[5] } ,
    {20,"swissprot" ,5,0.0,&avnx[6] } ,
    {20,"pir" ,6,0.0,&avnx[7] } ,
    {20,"pdb" ,7,0.0,&avnx[8] } ,
    {20,"epd" ,8,0.0,&avnx[9] } ,
    {20,"ecd" ,9,0.0,&avnx[10] } ,
    {20,"tfd" ,10,0.0,&avnx[11] } ,
    {20,"flybase" ,11,0.0,&avnx[12] } ,
    {20,"prosite" ,12,0.0,&avnx[13] } ,
    {20,"enzyme" ,13,0.0,&avnx[14] } ,
    {20,"mim" ,14,0.0,&avnx[15] } ,
    {20,"ecoseq" ,15,0.0,&avnx[16] } ,
    {20,"hiv" ,16,0.0,&avnx[17] } ,
    {20,"other" ,255,0.0,NULL } ,
    {20,"not-set" ,0,0.0,&avnx[19] } ,
    {20,"standard" ,1,0.0,&avnx[20] } ,
    {20,"unannotated" ,2,0.0,&avnx[21] } ,
    {20,"other" ,255,0.0,NULL } ,
    {3,NULL,1,0.0,NULL } ,
    {20,"fun" ,0,0.0,&avnx[24] } ,
    {20,"inv" ,1,0.0,&avnx[25] } ,
    {20,"mam" ,2,0.0,&avnx[26] } ,
    {20,"org" ,3,0.0,&avnx[27] } ,
    {20,"phg" ,4,0.0,&avnx[28] } ,
    {20,"pln" ,5,0.0,&avnx[29] } ,
    {20,"pri" ,6,0.0,&avnx[30] } ,
    {20,"pro" ,7,0.0,&avnx[31] } ,
    {20,"rod" ,8,0.0,&avnx[32] } ,
    {20,"syn" ,9,0.0,&avnx[33] } ,
    {20,"una" ,10,0.0,&avnx[34] } ,
    {20,"vrl" ,11,0.0,&avnx[35] } ,
    {20,"vrt" ,12,0.0,&avnx[36] } ,
    {20,"pat" ,13,0.0,&avnx[37] } ,
    {20,"est" ,14,0.0,&avnx[38] } ,
    {20,"sts" ,15,0.0,&avnx[39] } ,
    {20,"other" ,255,0.0,NULL } ,
    {20,"not-set" ,0,0.0,&avnx[41] } ,
    {20,"standard" ,1,0.0,&avnx[42] } ,
    {20,"prelim" ,2,0.0,&avnx[43] } ,
    {20,"other" ,255,0.0,NULL } ,
    {2,NULL,0,0.0,NULL } };

static AsnType atx[99] = {
  {401, "EMBL-dbname" ,1,0,0,0,0,1,0,0,NULL,&atx[5],&atx[1],0,&atx[6]} ,
  {0, "code" ,128,0,0,0,0,0,0,0,NULL,&atx[2],&avnx[0],0,&atx[3]} ,
  {310, "ENUMERATED" ,0,10,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "name" ,128,1,0,0,0,0,0,0,NULL,&atx[4],NULL,0,NULL} ,
  {323, "VisibleString" ,0,26,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {315, "CHOICE" ,0,-1,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {402, "EMBL-xref" ,1,0,0,0,0,1,0,0,NULL,&atx[12],&atx[7],0,&atx[13]} ,
  {0, "dbname" ,128,0,0,0,0,0,0,0,NULL,&atx[0],NULL,0,&atx[8]} ,
  {0, "id" ,128,1,0,0,0,0,0,0,NULL,&atx[11],&atx[9],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[10],NULL,0,NULL} ,
  {405, "Object-id" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,NULL} ,
  {312, "SEQUENCE OF" ,0,16,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {311, "SEQUENCE" ,0,16,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {403, "EMBL-block" ,1,0,0,0,0,1,0,0,NULL,&atx[12],&atx[14],0,&atx[17]} ,
  {0, "class" ,128,0,0,0,1,0,0,0,&avnx[22],&atx[2],&avnx[18],0,&atx[15]} ,
  {0, "div" ,128,1,0,1,0,0,0,0,NULL,&atx[2],&avnx[23],0,&atx[16]} ,
  {0, "creation-date" ,128,2,0,0,0,0,0,0,NULL,&atx[17],NULL,0,&atx[18]} ,
  {404, "Date" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[10]} ,
  {0, "update-date" ,128,3,0,0,0,0,0,0,NULL,&atx[17],NULL,0,&atx[19]} ,
  {0, "extra-acc" ,128,4,0,1,0,0,0,0,NULL,&atx[11],&atx[20],0,&atx[21]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[4],NULL,0,NULL} ,
  {0, "keywords" ,128,5,0,1,0,0,0,0,NULL,&atx[11],&atx[22],0,&atx[23]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[4],NULL,0,NULL} ,
  {0, "xref" ,128,6,0,1,0,0,0,0,NULL,&atx[11],&atx[24],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[6],NULL,0,NULL} ,
  {401, "SP-block" ,1,0,0,0,0,1,0,0,NULL,&atx[12],&atx[26],0,&atx[43]} ,
  {0, "class" ,128,0,0,0,0,0,0,0,NULL,&atx[2],&avnx[40],0,&atx[27]} ,
  {0, "extra-acc" ,128,1,0,1,0,0,0,0,NULL,&atx[29],&atx[28],0,&atx[30]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[4],NULL,0,NULL} ,
  {314, "SET OF" ,0,17,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "imeth" ,128,2,0,0,1,0,0,0,&avnx[44],&atx[31],NULL,0,&atx[32]} ,
  {301, "BOOLEAN" ,0,1,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "plasnm" ,128,3,0,1,0,0,0,0,NULL,&atx[29],&atx[33],0,&atx[34]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[4],NULL,0,NULL} ,
  {0, "seqref" ,128,4,0,1,0,0,0,0,NULL,&atx[29],&atx[35],0,&atx[37]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[36],NULL,0,NULL} ,
  {404, "Seq-id" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,NULL} ,
  {0, "dbref" ,128,5,0,1,0,0,0,0,NULL,&atx[29],&atx[38],0,&atx[40]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[39],NULL,0,NULL} ,
  {403, "Dbtag" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[36]} ,
  {0, "keywords" ,128,6,0,1,0,0,0,0,NULL,&atx[29],&atx[41],0,&atx[42]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[4],NULL,0,NULL} ,
  {0, "created" ,128,7,0,1,0,0,0,0,NULL,&atx[43],NULL,0,&atx[44]} ,
  {402, "Date" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[39]} ,
  {0, "sequpd" ,128,8,0,1,0,0,0,0,NULL,&atx[43],NULL,0,&atx[45]} ,
  {0, "annotupd" ,128,9,0,1,0,0,0,0,NULL,&atx[43],NULL,0,NULL} ,
  {401, "PIR-block" ,1,0,0,0,0,1,0,0,NULL,&atx[12],&atx[47],0,&atx[62]} ,
  {0, "had-punct" ,128,0,0,1,0,0,0,0,NULL,&atx[31],NULL,0,&atx[48]} ,
  {0, "host" ,128,1,0,1,0,0,0,0,NULL,&atx[4],NULL,0,&atx[49]} ,
  {0, "source" ,128,2,0,1,0,0,0,0,NULL,&atx[4],NULL,0,&atx[50]} ,
  {0, "summary" ,128,3,0,1,0,0,0,0,NULL,&atx[4],NULL,0,&atx[51]} ,
  {0, "genetic" ,128,4,0,1,0,0,0,0,NULL,&atx[4],NULL,0,&atx[52]} ,
  {0, "includes" ,128,5,0,1,0,0,0,0,NULL,&atx[4],NULL,0,&atx[53]} ,
  {0, "placement" ,128,6,0,1,0,0,0,0,NULL,&atx[4],NULL,0,&atx[54]} ,
  {0, "superfamily" ,128,7,0,1,0,0,0,0,NULL,&atx[4],NULL,0,&atx[55]} ,
  {0, "keywords" ,128,8,0,1,0,0,0,0,NULL,&atx[11],&atx[56],0,&atx[57]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[4],NULL,0,NULL} ,
  {0, "cross-reference" ,128,9,0,1,0,0,0,0,NULL,&atx[4],NULL,0,&atx[58]} ,
  {0, "date" ,128,10,0,1,0,0,0,0,NULL,&atx[4],NULL,0,&atx[59]} ,
  {0, "seq-raw" ,128,11,0,1,0,0,0,0,NULL,&atx[4],NULL,0,&atx[60]} ,
  {0, "seqref" ,128,12,0,1,0,0,0,0,NULL,&atx[29],&atx[61],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[62],NULL,0,NULL} ,
  {402, "Seq-id" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,NULL} ,
  {401, "GB-block" ,1,0,0,0,0,1,0,0,NULL,&atx[12],&atx[64],0,&atx[72]} ,
  {0, "extra-accessions" ,128,0,0,1,0,0,0,0,NULL,&atx[11],&atx[65],0,&atx[66]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[4],NULL,0,NULL} ,
  {0, "source" ,128,1,0,1,0,0,0,0,NULL,&atx[4],NULL,0,&atx[67]} ,
  {0, "keywords" ,128,2,0,1,0,0,0,0,NULL,&atx[11],&atx[68],0,&atx[69]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[4],NULL,0,NULL} ,
  {0, "origin" ,128,3,0,1,0,0,0,0,NULL,&atx[4],NULL,0,&atx[70]} ,
  {0, "date" ,128,4,0,1,0,0,0,0,NULL,&atx[4],NULL,0,&atx[71]} ,
  {0, "entry-date" ,128,5,0,1,0,0,0,0,NULL,&atx[72],NULL,0,&atx[73]} ,
  {402, "Date" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,NULL} ,
  {0, "div" ,128,6,0,1,0,0,0,0,NULL,&atx[4],NULL,0,&atx[74]} ,
  {0, "taxonomy" ,128,7,0,1,0,0,0,0,NULL,&atx[4],NULL,0,NULL} ,
  {401, "PRF-block" ,1,0,0,0,0,1,0,0,NULL,&atx[12],&atx[76],0,&atx[77]} ,
  {0, "extra-src" ,128,0,0,1,0,0,0,0,NULL,&atx[77],NULL,0,&atx[83]} ,
  {402, "PRF-ExtraSrc" ,1,0,0,0,0,0,0,0,NULL,&atx[12],&atx[78],0,NULL} ,
  {0, "host" ,128,0,0,1,0,0,0,0,NULL,&atx[4],NULL,0,&atx[79]} ,
  {0, "part" ,128,1,0,1,0,0,0,0,NULL,&atx[4],NULL,0,&atx[80]} ,
  {0, "state" ,128,2,0,1,0,0,0,0,NULL,&atx[4],NULL,0,&atx[81]} ,
  {0, "strain" ,128,3,0,1,0,0,0,0,NULL,&atx[4],NULL,0,&atx[82]} ,
  {0, "taxon" ,128,4,0,1,0,0,0,0,NULL,&atx[4],NULL,0,NULL} ,
  {0, "keywords" ,128,1,0,1,0,0,0,0,NULL,&atx[11],&atx[84],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[4],NULL,0,NULL} ,
  {401, "PDB-block" ,1,0,0,0,0,1,0,0,NULL,&atx[12],&atx[86],0,&atx[87]} ,
  {0, "deposition" ,128,0,0,0,0,0,0,0,NULL,&atx[87],NULL,0,&atx[88]} ,
  {402, "Date" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[95]} ,
  {0, "class" ,128,1,0,0,0,0,0,0,NULL,&atx[4],NULL,0,&atx[89]} ,
  {0, "compound" ,128,2,0,0,0,0,0,0,NULL,&atx[11],&atx[90],0,&atx[91]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[4],NULL,0,NULL} ,
  {0, "source" ,128,3,0,0,0,0,0,0,NULL,&atx[11],&atx[92],0,&atx[93]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[4],NULL,0,NULL} ,
  {0, "exp-method" ,128,4,0,1,0,0,0,0,NULL,&atx[4],NULL,0,&atx[94]} ,
  {0, "replace" ,128,5,0,1,0,0,0,0,NULL,&atx[95],NULL,0,NULL} ,
  {403, "PDB-replace" ,1,0,0,0,0,0,0,0,NULL,&atx[12],&atx[96],0,NULL} ,
  {0, "date" ,128,0,0,0,0,0,0,0,NULL,&atx[87],NULL,0,&atx[97]} ,
  {0, "ids" ,128,1,0,0,0,0,0,0,NULL,&atx[11],&atx[98],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[4],NULL,0,NULL} };

static AsnModule ampx[6] = {
  { "EMBL-General" , "asnblock.h60",&atx[0],NULL,&ampx[1],0,0} ,
  { "SP-General" , NULL,&atx[25],NULL,&ampx[2],0,0} ,
  { "PIR-General" , NULL,&atx[46],NULL,&ampx[3],0,0} ,
  { "GenBank-General" , NULL,&atx[63],NULL,&ampx[4],0,0} ,
  { "PRF-General" , NULL,&atx[75],NULL,&ampx[5],0,0} ,
  { "PDB-General" , NULL,&atx[85],NULL,NULL,0,0} };

static AsnValxNodePtr avn = avnx;
static AsnTypePtr at = atx;
static AsnModulePtr amp = ampx;



/**************************************************
*
*    Defines for Module EMBL-General
*
**************************************************/

#define EMBL_DBNAME &at[0]
#define EMBL_DBNAME_code &at[1]
#define EMBL_DBNAME_name &at[3]

#define EMBL_XREF &at[6]
#define EMBL_XREF_dbname &at[7]
#define EMBL_XREF_id &at[8]
#define EMBL_XREF_id_E &at[9]

#define EMBL_BLOCK &at[13]
#define EMBL_BLOCK_class &at[14]
#define EMBL_BLOCK_div &at[15]
#define EMBL_BLOCK_creation_date &at[16]
#define EMBL_BLOCK_update_date &at[18]
#define EMBL_BLOCK_extra_acc &at[19]
#define EMBL_BLOCK_extra_acc_E &at[20]
#define EMBL_BLOCK_keywords &at[21]
#define EMBL_BLOCK_keywords_E &at[22]
#define EMBL_BLOCK_xref &at[23]
#define EMBL_BLOCK_xref_E &at[24]


/**************************************************
*
*    Defines for Module SP-General
*
**************************************************/

#define SP_BLOCK &at[25]
#define SP_BLOCK_class &at[26]
#define SP_BLOCK_extra_acc &at[27]
#define SP_BLOCK_extra_acc_E &at[28]
#define SP_BLOCK_imeth &at[30]
#define SP_BLOCK_plasnm &at[32]
#define SP_BLOCK_plasnm_E &at[33]
#define SP_BLOCK_seqref &at[34]
#define SP_BLOCK_seqref_E &at[35]
#define SP_BLOCK_dbref &at[37]
#define SP_BLOCK_dbref_E &at[38]
#define SP_BLOCK_keywords &at[40]
#define SP_BLOCK_keywords_E &at[41]
#define SP_BLOCK_created &at[42]
#define SP_BLOCK_sequpd &at[44]
#define SP_BLOCK_annotupd &at[45]


/**************************************************
*
*    Defines for Module PIR-General
*
**************************************************/

#define PIR_BLOCK &at[46]
#define PIR_BLOCK_had_punct &at[47]
#define PIR_BLOCK_host &at[48]
#define PIR_BLOCK_source &at[49]
#define PIR_BLOCK_summary &at[50]
#define PIR_BLOCK_genetic &at[51]
#define PIR_BLOCK_includes &at[52]
#define PIR_BLOCK_placement &at[53]
#define PIR_BLOCK_superfamily &at[54]
#define PIR_BLOCK_keywords &at[55]
#define PIR_BLOCK_keywords_E &at[56]
#define PIR_BLOCK_cross_reference &at[57]
#define PIR_BLOCK_date &at[58]
#define PIR_BLOCK_seq_raw &at[59]
#define PIR_BLOCK_seqref &at[60]
#define PIR_BLOCK_seqref_E &at[61]


/**************************************************
*
*    Defines for Module GenBank-General
*
**************************************************/

#define GB_BLOCK &at[63]
#define GB_BLOCK_extra_accessions &at[64]
#define GB_BLOCK_extra_accessions_E &at[65]
#define GB_BLOCK_source &at[66]
#define GB_BLOCK_keywords &at[67]
#define GB_BLOCK_keywords_E &at[68]
#define GB_BLOCK_origin &at[69]
#define GB_BLOCK_date &at[70]
#define GB_BLOCK_entry_date &at[71]
#define GB_BLOCK_div &at[73]
#define GB_BLOCK_taxonomy &at[74]


/**************************************************
*
*    Defines for Module PRF-General
*
**************************************************/

#define PRF_BLOCK &at[75]
#define PRF_BLOCK_extra_src &at[76]
#define PRF_BLOCK_keywords &at[83]
#define PRF_BLOCK_keywords_E &at[84]

#define PRF_EXTRASRC &at[77]
#define PRF_EXTRASRC_host &at[78]
#define PRF_EXTRASRC_part &at[79]
#define PRF_EXTRASRC_state &at[80]
#define PRF_EXTRASRC_strain &at[81]
#define PRF_EXTRASRC_taxon &at[82]


/**************************************************
*
*    Defines for Module PDB-General
*
**************************************************/

#define PDB_BLOCK &at[85]
#define PDB_BLOCK_deposition &at[86]
#define PDB_BLOCK_class &at[88]
#define PDB_BLOCK_compound &at[89]
#define PDB_BLOCK_compound_E &at[90]
#define PDB_BLOCK_source &at[91]
#define PDB_BLOCK_source_E &at[92]
#define PDB_BLOCK_exp_method &at[93]
#define PDB_BLOCK_replace &at[94]

#define PDB_REPLACE &at[95]
#define PDB_REPLACE_date &at[96]
#define PDB_REPLACE_ids &at[97]
#define PDB_REPLACE_ids_E &at[98]
