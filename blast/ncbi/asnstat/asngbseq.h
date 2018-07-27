/***********************************************************************
*
**
*        Automatic header module from ASNTOOL
*
************************************************************************/

#ifndef _ASNTOOL_
#include <asn.h>
#endif

static char * asnfilename = "asngbseq.h65";
static AsnValxNode avnx[19] = {
    {20,"not-set" ,0,0.0,&avnx[1] } ,
    {20,"single-stranded" ,1,0.0,&avnx[2] } ,
    {20,"double-stranded" ,2,0.0,&avnx[3] } ,
    {20,"mixed-stranded" ,3,0.0,NULL } ,
    {3,NULL,0,0.0,NULL } ,
    {20,"nucleic-acid" ,0,0.0,&avnx[6] } ,
    {20,"dna" ,1,0.0,&avnx[7] } ,
    {20,"rna" ,2,0.0,&avnx[8] } ,
    {20,"trna" ,3,0.0,&avnx[9] } ,
    {20,"rrna" ,4,0.0,&avnx[10] } ,
    {20,"mrna" ,5,0.0,&avnx[11] } ,
    {20,"urna" ,6,0.0,&avnx[12] } ,
    {20,"snrna" ,7,0.0,&avnx[13] } ,
    {20,"snorna" ,8,0.0,&avnx[14] } ,
    {20,"peptide" ,9,0.0,NULL } ,
    {3,NULL,0,0.0,NULL } ,
    {20,"linear" ,1,0.0,&avnx[17] } ,
    {20,"circular" ,2,0.0,NULL } ,
    {3,NULL,1,0.0,NULL } };

static AsnType atx[70] = {
  {401, "GBSeq" ,1,0,0,0,0,0,0,0,NULL,&atx[44],&atx[1],0,&atx[19]} ,
  {0, "locus" ,128,0,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[3]} ,
  {323, "VisibleString" ,0,26,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "length" ,128,1,0,0,0,0,0,0,NULL,&atx[4],NULL,0,&atx[5]} ,
  {302, "INTEGER" ,0,2,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "strandedness" ,128,2,0,0,1,0,0,0,&avnx[4],&atx[4],&avnx[0],0,&atx[6]} ,
  {0, "moltype" ,128,3,0,0,1,0,0,0,&avnx[15],&atx[4],&avnx[5],0,&atx[7]} ,
  {0, "topology" ,128,4,0,0,1,0,0,0,&avnx[18],&atx[4],&avnx[16],0,&atx[8]} ,
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
  {402, "GBSeqid" ,1,0,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[23]} ,
  {312, "SEQUENCE OF" ,0,16,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "secondary-accessions" ,128,15,0,1,0,0,0,0,NULL,&atx[20],&atx[22],0,&atx[24]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[23],NULL,0,NULL} ,
  {403, "GBSecondary-accn" ,1,0,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[26]} ,
  {0, "keywords" ,128,16,0,1,0,0,0,0,NULL,&atx[20],&atx[25],0,&atx[27]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[26],NULL,0,NULL} ,
  {404, "GBKeyword" ,1,0,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[33]} ,
  {0, "segment" ,128,17,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[28]} ,
  {0, "source" ,128,18,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[29]} ,
  {0, "organism" ,128,19,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[30]} ,
  {0, "taxonomy" ,128,20,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[31]} ,
  {0, "references" ,128,21,0,0,0,0,0,0,NULL,&atx[20],&atx[32],0,&atx[45]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[33],NULL,0,NULL} ,
  {405, "GBReference" ,1,0,0,0,0,0,0,0,NULL,&atx[44],&atx[34],0,&atx[51]} ,
  {0, "reference" ,128,0,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[35]} ,
  {0, "authors" ,128,1,0,1,0,0,0,0,NULL,&atx[20],&atx[36],0,&atx[38]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[37],NULL,0,NULL} ,
  {407, "GBAuthor" ,1,0,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[56]} ,
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
  {406, "GBFeature" ,1,0,0,0,0,0,0,0,NULL,&atx[44],&atx[52],0,&atx[37]} ,
  {0, "key" ,128,0,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[53]} ,
  {0, "location" ,128,1,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[54]} ,
  {0, "intervals" ,128,2,0,1,0,0,0,0,NULL,&atx[20],&atx[55],0,&atx[61]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[56],NULL,0,NULL} ,
  {408, "GBInterval" ,1,0,0,0,0,0,0,0,NULL,&atx[44],&atx[57],0,&atx[63]} ,
  {0, "from" ,128,0,0,1,0,0,0,0,NULL,&atx[4],NULL,0,&atx[58]} ,
  {0, "to" ,128,1,0,1,0,0,0,0,NULL,&atx[4],NULL,0,&atx[59]} ,
  {0, "point" ,128,2,0,1,0,0,0,0,NULL,&atx[4],NULL,0,&atx[60]} ,
  {0, "accession" ,128,3,0,0,0,0,0,0,NULL,&atx[2],NULL,0,NULL} ,
  {0, "quals" ,128,3,0,1,0,0,0,0,NULL,&atx[20],&atx[62],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[63],NULL,0,NULL} ,
  {409, "GBQualifier" ,1,0,0,0,0,0,0,0,NULL,&atx[44],&atx[64],0,&atx[68]} ,
  {0, "name" ,128,0,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[65]} ,
  {0, "value" ,128,1,0,1,0,0,0,0,NULL,&atx[2],NULL,0,NULL} ,
  {0, "sequence" ,128,27,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[67]} ,
  {0, "contig" ,128,28,0,1,0,0,0,0,NULL,&atx[2],NULL,0,NULL} ,
  {410, "GBSet" ,1,0,0,0,0,0,0,0,NULL,&atx[20],&atx[69],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[0],NULL,0,NULL} };

static AsnModule ampx[1] = {
  { "NCBI-GBSeq" , "asngbseq.h65",&atx[0],NULL,NULL,0,0} };

static AsnValxNodePtr avn = avnx;
static AsnTypePtr at = atx;
static AsnModulePtr amp = ampx;



/**************************************************
*
*    Defines for Module NCBI-GBSeq
*
**************************************************/

#define GBSEQ &at[0]
#define GBSEQ_locus &at[1]
#define GBSEQ_length &at[3]
#define GBSEQ_strandedness &at[5]
#define GBSEQ_moltype &at[6]
#define GBSEQ_topology &at[7]
#define GBSEQ_division &at[8]
#define GBSEQ_update_date &at[9]
#define GBSEQ_create_date &at[10]
#define GBSEQ_update_release &at[11]
#define GBSEQ_create_release &at[12]
#define GBSEQ_definition &at[13]
#define GBSEQ_primary_accession &at[14]
#define GBSEQ_entry_version &at[15]
#define GBSEQ_accession_version &at[16]
#define GBSEQ_other_seqids &at[17]
#define GBSEQ_other_seqids_E &at[18]
#define GBSEQ_secondary_accessions &at[21]
#define GBSEQ_secondary_accessions_E &at[22]
#define GBSEQ_keywords &at[24]
#define GBSEQ_keywords_E &at[25]
#define GBSEQ_segment &at[27]
#define GBSEQ_source &at[28]
#define GBSEQ_organism &at[29]
#define GBSEQ_taxonomy &at[30]
#define GBSEQ_references &at[31]
#define GBSEQ_references_E &at[32]
#define GBSEQ_comment &at[45]
#define GBSEQ_primary &at[46]
#define GBSEQ_source_db &at[47]
#define GBSEQ_database_reference &at[48]
#define GBSEQ_feature_table &at[49]
#define GBSEQ_feature_table_E &at[50]
#define GBSEQ_sequence &at[66]
#define GBSEQ_contig &at[67]

#define GBSEQID &at[19]

#define GBSECONDARY_ACCN &at[23]

#define GBKEYWORD &at[26]

#define GBREFERENCE &at[33]
#define GBREFERENCE_reference &at[34]
#define GBREFERENCE_authors &at[35]
#define GBREFERENCE_authors_E &at[36]
#define GBREFERENCE_consortium &at[38]
#define GBREFERENCE_title &at[39]
#define GBREFERENCE_journal &at[40]
#define GBREFERENCE_medline &at[41]
#define GBREFERENCE_pubmed &at[42]
#define GBREFERENCE_remark &at[43]

#define GBFEATURE &at[51]
#define GBFEATURE_key &at[52]
#define GBFEATURE_location &at[53]
#define GBFEATURE_intervals &at[54]
#define GBFEATURE_intervals_E &at[55]
#define GBFEATURE_quals &at[61]
#define GBFEATURE_quals_E &at[62]

#define GBAUTHOR &at[37]

#define GBINTERVAL &at[56]
#define GBINTERVAL_from &at[57]
#define GBINTERVAL_to &at[58]
#define GBINTERVAL_point &at[59]
#define GBINTERVAL_accession &at[60]

#define GBQUALIFIER &at[63]
#define GBQUALIFIER_name &at[64]
#define GBQUALIFIER_value &at[65]

#define GBSET &at[68]
#define GBSET_E &at[69]
