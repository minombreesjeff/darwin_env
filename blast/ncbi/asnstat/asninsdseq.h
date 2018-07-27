/***********************************************************************
*
**
*        Automatic header module from ASNTOOL
*
************************************************************************/

#ifndef _ASNTOOL_
#include <asn.h>
#endif

static char * asnfilename = "asninsdseq.h15";
static AsnType atx[70] = {
  {401, "INSDSeq" ,1,0,0,0,0,0,0,0,NULL,&atx[44],&atx[1],0,&atx[19]} ,
  {0, "locus" ,128,0,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[3]} ,
  {323, "VisibleString" ,0,26,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "length" ,128,1,0,0,0,0,0,0,NULL,&atx[4],NULL,0,&atx[5]} ,
  {302, "INTEGER" ,0,2,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "strandedness" ,128,2,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[6]} ,
  {0, "moltype" ,128,3,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[7]} ,
  {0, "topology" ,128,4,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[8]} ,
  {0, "division" ,128,5,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[9]} ,
  {0, "update-date" ,128,6,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[10]} ,
  {0, "create-date" ,128,7,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[11]} ,
  {0, "update-release" ,128,8,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[12]} ,
  {0, "create-release" ,128,9,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[13]} ,
  {0, "definition" ,128,10,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[14]} ,
  {0, "primary-accession" ,128,11,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[15]} ,
  {0, "entry-version" ,128,12,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[16]} ,
  {0, "accession-version" ,128,13,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[17]} ,
  {0, "other-seqids" ,128,14,0,1,0,0,0,0,NULL,&atx[20],&atx[18],0,&atx[21]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[19],NULL,0,NULL} ,
  {402, "INSDSeqid" ,1,0,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[23]} ,
  {312, "SEQUENCE OF" ,0,16,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "secondary-accessions" ,128,15,0,1,0,0,0,0,NULL,&atx[20],&atx[22],0,&atx[24]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[23],NULL,0,NULL} ,
  {403, "INSDSecondary-accn" ,1,0,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[26]} ,
  {0, "keywords" ,128,16,0,1,0,0,0,0,NULL,&atx[20],&atx[25],0,&atx[27]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[26],NULL,0,NULL} ,
  {404, "INSDKeyword" ,1,0,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[33]} ,
  {0, "segment" ,128,17,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[28]} ,
  {0, "source" ,128,18,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[29]} ,
  {0, "organism" ,128,19,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[30]} ,
  {0, "taxonomy" ,128,20,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[31]} ,
  {0, "references" ,128,21,0,1,0,0,0,0,NULL,&atx[20],&atx[32],0,&atx[45]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[33],NULL,0,NULL} ,
  {405, "INSDReference" ,1,0,0,0,0,0,0,0,NULL,&atx[44],&atx[34],0,&atx[51]} ,
  {0, "reference" ,128,0,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[35]} ,
  {0, "authors" ,128,1,0,1,0,0,0,0,NULL,&atx[20],&atx[36],0,&atx[38]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[37],NULL,0,NULL} ,
  {407, "INSDAuthor" ,1,0,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[56]} ,
  {0, "consortium" ,128,2,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[39]} ,
  {0, "title" ,128,3,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[40]} ,
  {0, "journal" ,128,4,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[41]} ,
  {0, "medline" ,128,5,0,1,0,0,0,0,NULL,&atx[4],NULL,0,&atx[42]} ,
  {0, "pubmed" ,128,6,0,1,0,0,0,0,NULL,&atx[4],NULL,0,&atx[43]} ,
  {0, "remark" ,128,7,0,1,0,0,0,0,NULL,&atx[2],NULL,0,NULL} ,
  {311, "SEQUENCE" ,0,16,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "comment" ,128,22,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[46]} ,
  {0, "primary" ,128,23,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[47]} ,
  {0, "source-db" ,128,24,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[48]} ,
  {0, "database-reference" ,128,25,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[49]} ,
  {0, "feature-table" ,128,26,0,1,0,0,0,0,NULL,&atx[20],&atx[50],0,&atx[66]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[51],NULL,0,NULL} ,
  {406, "INSDFeature" ,1,0,0,0,0,0,0,0,NULL,&atx[44],&atx[52],0,&atx[37]} ,
  {0, "key" ,128,0,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[53]} ,
  {0, "location" ,128,1,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[54]} ,
  {0, "intervals" ,128,2,0,1,0,0,0,0,NULL,&atx[20],&atx[55],0,&atx[61]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[56],NULL,0,NULL} ,
  {408, "INSDInterval" ,1,0,0,0,0,0,0,0,NULL,&atx[44],&atx[57],0,&atx[63]} ,
  {0, "from" ,128,0,0,1,0,0,0,0,NULL,&atx[4],NULL,0,&atx[58]} ,
  {0, "to" ,128,1,0,1,0,0,0,0,NULL,&atx[4],NULL,0,&atx[59]} ,
  {0, "point" ,128,2,0,1,0,0,0,0,NULL,&atx[4],NULL,0,&atx[60]} ,
  {0, "accession" ,128,3,0,0,0,0,0,0,NULL,&atx[2],NULL,0,NULL} ,
  {0, "quals" ,128,3,0,1,0,0,0,0,NULL,&atx[20],&atx[62],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[63],NULL,0,NULL} ,
  {409, "INSDQualifier" ,1,0,0,0,0,0,0,0,NULL,&atx[44],&atx[64],0,&atx[68]} ,
  {0, "name" ,128,0,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[65]} ,
  {0, "value" ,128,1,0,1,0,0,0,0,NULL,&atx[2],NULL,0,NULL} ,
  {0, "sequence" ,128,27,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[67]} ,
  {0, "contig" ,128,28,0,1,0,0,0,0,NULL,&atx[2],NULL,0,NULL} ,
  {410, "INSDSet" ,1,0,0,0,0,0,0,0,NULL,&atx[20],&atx[69],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[0],NULL,0,NULL} };

static AsnModule ampx[1] = {
  { "INSD-INSDSeq" , "asninsdseq.h15",&atx[0],NULL,NULL,0,0} };

static AsnValxNodePtr avn = NULL;
static AsnTypePtr at = atx;
static AsnModulePtr amp = ampx;



/**************************************************
*
*    Defines for Module INSD-INSDSeq
*
**************************************************/

#define INSDSEQ &at[0]
#define INSDSEQ_locus &at[1]
#define INSDSEQ_length &at[3]
#define INSDSEQ_strandedness &at[5]
#define INSDSEQ_moltype &at[6]
#define INSDSEQ_topology &at[7]
#define INSDSEQ_division &at[8]
#define INSDSEQ_update_date &at[9]
#define INSDSEQ_create_date &at[10]
#define INSDSEQ_update_release &at[11]
#define INSDSEQ_create_release &at[12]
#define INSDSEQ_definition &at[13]
#define INSDSEQ_primary_accession &at[14]
#define INSDSEQ_entry_version &at[15]
#define INSDSEQ_accession_version &at[16]
#define INSDSEQ_other_seqids &at[17]
#define INSDSEQ_other_seqids_E &at[18]
#define INSDSEQ_secondary_accessions &at[21]
#define INSDSEQ_secondary_accessions_E &at[22]
#define INSDSEQ_keywords &at[24]
#define INSDSEQ_keywords_E &at[25]
#define INSDSEQ_segment &at[27]
#define INSDSEQ_source &at[28]
#define INSDSEQ_organism &at[29]
#define INSDSEQ_taxonomy &at[30]
#define INSDSEQ_references &at[31]
#define INSDSEQ_references_E &at[32]
#define INSDSEQ_comment &at[45]
#define INSDSEQ_primary &at[46]
#define INSDSEQ_source_db &at[47]
#define INSDSEQ_database_reference &at[48]
#define INSDSEQ_feature_table &at[49]
#define INSDSEQ_feature_table_E &at[50]
#define INSDSEQ_sequence &at[66]
#define INSDSEQ_contig &at[67]

#define INSDSEQID &at[19]

#define INSDSECONDARY_ACCN &at[23]

#define INSDKEYWORD &at[26]

#define INSDREFERENCE &at[33]
#define INSDREFERENCE_reference &at[34]
#define INSDREFERENCE_authors &at[35]
#define INSDREFERENCE_authors_E &at[36]
#define INSDREFERENCE_consortium &at[38]
#define INSDREFERENCE_title &at[39]
#define INSDREFERENCE_journal &at[40]
#define INSDREFERENCE_medline &at[41]
#define INSDREFERENCE_pubmed &at[42]
#define INSDREFERENCE_remark &at[43]

#define INSDFEATURE &at[51]
#define INSDFEATURE_key &at[52]
#define INSDFEATURE_location &at[53]
#define INSDFEATURE_intervals &at[54]
#define INSDFEATURE_intervals_E &at[55]
#define INSDFEATURE_quals &at[61]
#define INSDFEATURE_quals_E &at[62]

#define INSDAUTHOR &at[37]

#define INSDINTERVAL &at[56]
#define INSDINTERVAL_from &at[57]
#define INSDINTERVAL_to &at[58]
#define INSDINTERVAL_point &at[59]
#define INSDINTERVAL_accession &at[60]

#define INSDQUALIFIER &at[63]
#define INSDQUALIFIER_name &at[64]
#define INSDQUALIFIER_value &at[65]

#define INSDSET &at[68]
#define INSDSET_E &at[69]
