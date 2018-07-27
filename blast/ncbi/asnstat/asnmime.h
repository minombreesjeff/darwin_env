/***********************************************************************
*
**
*        Automatic header module from ASNTOOL
*
************************************************************************/

#ifndef _ASNTOOL_
#include <asn.h>
#endif

static char * asnfilename = "asnmime.h12";
static AsnValxNode avnx[16] = {
    {20,"docsum" ,1,0.0,&avnx[1] } ,
    {20,"genbank" ,2,0.0,&avnx[2] } ,
    {20,"genpept" ,3,0.0,&avnx[3] } ,
    {20,"fasta" ,4,0.0,&avnx[4] } ,
    {20,"asn1" ,5,0.0,&avnx[5] } ,
    {20,"graphic" ,6,0.0,&avnx[6] } ,
    {20,"alignment" ,7,0.0,&avnx[7] } ,
    {20,"globalview" ,8,0.0,&avnx[8] } ,
    {20,"report" ,9,0.0,&avnx[9] } ,
    {20,"medlars" ,10,0.0,&avnx[10] } ,
    {20,"embl" ,11,0.0,&avnx[11] } ,
    {20,"pdb" ,12,0.0,&avnx[12] } ,
    {20,"kinemage" ,13,0.0,NULL } ,
    {20,"ncbi-backbone" ,2,0.0,&avnx[14] } ,
    {20,"ncbi-all-atom" ,3,0.0,&avnx[15] } ,
    {20,"pdb-model" ,4,0.0,NULL } };

static AsnType atx[81] = {
  {401, "Ncbi-mime-asn1" ,1,0,0,0,0,1,0,0,NULL,&atx[16],&atx[1],0,&atx[13]} ,
  {0, "entrez" ,128,0,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[22]} ,
  {410, "Entrez-general" ,1,0,0,0,0,0,0,0,NULL,&atx[21],&atx[3],0,&atx[23]} ,
  {0, "title" ,128,0,0,1,0,0,0,0,NULL,&atx[4],NULL,0,&atx[5]} ,
  {323, "VisibleString" ,0,26,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "data" ,128,1,0,0,0,0,0,0,NULL,&atx[16],&atx[6],0,&atx[17]} ,
  {0, "ml" ,128,0,0,0,0,0,0,0,NULL,&atx[7],NULL,0,&atx[8]} ,
  {407, "Medline-entry" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[35]} ,
  {0, "prot" ,128,1,0,0,0,0,0,0,NULL,&atx[9],NULL,0,&atx[10]} ,
  {405, "Seq-entry" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[33]} ,
  {0, "nuc" ,128,2,0,0,0,0,0,0,NULL,&atx[9],NULL,0,&atx[11]} ,
  {0, "genome" ,128,3,0,0,0,0,0,0,NULL,&atx[9],NULL,0,&atx[12]} ,
  {0, "structure" ,128,4,0,0,0,0,0,0,NULL,&atx[13],NULL,0,&atx[14]} ,
  {402, "Biostruc" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[15]} ,
  {0, "strucAnnot" ,128,5,0,0,0,0,0,0,NULL,&atx[15],NULL,0,NULL} ,
  {403, "Biostruc-annot-set" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[77]} ,
  {315, "CHOICE" ,0,-1,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "style" ,128,2,0,0,0,0,0,0,NULL,&atx[18],NULL,0,&atx[20]} ,
  {417, "Entrez-style" ,1,0,0,0,0,0,0,0,NULL,&atx[19],&avnx[0],0,NULL} ,
  {310, "ENUMERATED" ,0,10,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "location" ,128,3,0,1,0,0,0,0,NULL,&atx[4],NULL,0,NULL} ,
  {311, "SEQUENCE" ,0,16,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "alignstruc" ,128,1,0,0,0,0,0,0,NULL,&atx[23],NULL,0,&atx[38]} ,
  {411, "Biostruc-align" ,1,0,0,0,0,0,0,0,NULL,&atx[21],&atx[24],0,&atx[39]} ,
  {0, "master" ,128,0,0,0,0,0,0,0,NULL,&atx[13],NULL,0,&atx[25]} ,
  {0, "slaves" ,128,1,0,0,0,0,0,0,NULL,&atx[27],&atx[26],0,&atx[28]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[13],NULL,0,NULL} ,
  {314, "SET OF" ,0,17,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "alignments" ,128,2,0,0,0,0,0,0,NULL,&atx[15],NULL,0,&atx[29]} ,
  {0, "sequences" ,128,3,0,0,0,0,0,0,NULL,&atx[27],&atx[30],0,&atx[31]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[9],NULL,0,NULL} ,
  {0, "seqalign" ,128,4,0,0,0,0,0,0,NULL,&atx[27],&atx[32],0,&atx[34]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[33],NULL,0,NULL} ,
  {406, "Seq-annot" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[7]} ,
  {0, "style-dictionary" ,128,5,0,1,0,0,0,0,NULL,&atx[35],NULL,0,&atx[36]} ,
  {408, "Cn3d-style-dictionary" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[37]} ,
  {0, "user-annotations" ,128,6,0,1,0,0,0,0,NULL,&atx[37],NULL,0,NULL} ,
  {409, "Cn3d-user-annotations" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[2]} ,
  {0, "alignseq" ,128,2,0,0,0,0,0,0,NULL,&atx[39],NULL,0,&atx[46]} ,
  {412, "Biostruc-align-seq" ,1,0,0,0,0,0,0,0,NULL,&atx[21],&atx[40],0,&atx[47]} ,
  {0, "sequences" ,128,0,0,0,0,0,0,0,NULL,&atx[27],&atx[41],0,&atx[42]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[9],NULL,0,NULL} ,
  {0, "seqalign" ,128,1,0,0,0,0,0,0,NULL,&atx[27],&atx[43],0,&atx[44]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[33],NULL,0,NULL} ,
  {0, "style-dictionary" ,128,2,0,1,0,0,0,0,NULL,&atx[35],NULL,0,&atx[45]} ,
  {0, "user-annotations" ,128,3,0,1,0,0,0,0,NULL,&atx[37],NULL,0,NULL} ,
  {0, "strucseq" ,128,3,0,0,0,0,0,0,NULL,&atx[47],NULL,0,&atx[53]} ,
  {413, "Biostruc-seq" ,1,0,0,0,0,0,0,0,NULL,&atx[21],&atx[48],0,&atx[54]} ,
  {0, "structure" ,128,0,0,0,0,0,0,0,NULL,&atx[13],NULL,0,&atx[49]} ,
  {0, "sequences" ,128,1,0,0,0,0,0,0,NULL,&atx[27],&atx[50],0,&atx[51]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[9],NULL,0,NULL} ,
  {0, "style-dictionary" ,128,2,0,1,0,0,0,0,NULL,&atx[35],NULL,0,&atx[52]} ,
  {0, "user-annotations" ,128,3,0,1,0,0,0,0,NULL,&atx[37],NULL,0,NULL} ,
  {0, "strucseqs" ,128,4,0,0,0,0,0,0,NULL,&atx[54],NULL,0,&atx[62]} ,
  {414, "Biostruc-seqs" ,1,0,0,0,0,0,0,0,NULL,&atx[21],&atx[55],0,&atx[63]} ,
  {0, "structure" ,128,0,0,0,0,0,0,0,NULL,&atx[13],NULL,0,&atx[56]} ,
  {0, "sequences" ,128,1,0,0,0,0,0,0,NULL,&atx[27],&atx[57],0,&atx[58]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[9],NULL,0,NULL} ,
  {0, "seqalign" ,128,2,0,0,0,0,0,0,NULL,&atx[27],&atx[59],0,&atx[60]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[33],NULL,0,NULL} ,
  {0, "style-dictionary" ,128,3,0,1,0,0,0,0,NULL,&atx[35],NULL,0,&atx[61]} ,
  {0, "user-annotations" ,128,4,0,1,0,0,0,0,NULL,&atx[37],NULL,0,NULL} ,
  {0, "general" ,128,5,0,0,0,0,0,0,NULL,&atx[63],NULL,0,NULL} ,
  {415, "Biostruc-seqs-aligns-cdd" ,1,0,0,0,0,0,0,0,NULL,&atx[21],&atx[64],0,&atx[66]} ,
  {0, "seq-align-data" ,128,0,0,0,0,0,0,0,NULL,&atx[16],&atx[65],0,&atx[78]} ,
  {0, "bundle" ,128,0,0,0,0,0,0,0,NULL,&atx[66],NULL,0,&atx[76]} ,
  {416, "Bundle-seqs-aligns" ,1,0,0,0,0,0,0,0,NULL,&atx[21],&atx[67],0,&atx[18]} ,
  {0, "sequences" ,128,0,0,1,0,0,0,0,NULL,&atx[27],&atx[68],0,&atx[69]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[9],NULL,0,NULL} ,
  {0, "seqaligns" ,128,1,0,1,0,0,0,0,NULL,&atx[27],&atx[70],0,&atx[71]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[33],NULL,0,NULL} ,
  {0, "strucaligns" ,128,2,0,1,0,0,0,0,NULL,&atx[15],NULL,0,&atx[72]} ,
  {0, "imports" ,128,3,0,1,0,0,0,0,NULL,&atx[27],&atx[73],0,&atx[74]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[33],NULL,0,NULL} ,
  {0, "style-dictionary" ,128,4,0,1,0,0,0,0,NULL,&atx[35],NULL,0,&atx[75]} ,
  {0, "user-annotations" ,128,5,0,1,0,0,0,0,NULL,&atx[37],NULL,0,NULL} ,
  {0, "cdd" ,128,1,0,0,0,0,0,0,NULL,&atx[77],NULL,0,NULL} ,
  {404, "Cdd" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[9]} ,
  {0, "structures" ,128,1,0,1,0,0,0,0,NULL,&atx[27],&atx[79],0,&atx[80]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[13],NULL,0,NULL} ,
  {0, "structure-type" ,128,2,0,1,0,0,0,0,NULL,&atx[19],&avnx[13],0,NULL} };

static AsnModule ampx[1] = {
  { "NCBI-Mime" , "asnmime.h12",&atx[0],NULL,NULL,0,0} };

static AsnValxNodePtr avn = avnx;
static AsnTypePtr at = atx;
static AsnModulePtr amp = ampx;



/**************************************************
*
*    Defines for Module NCBI-Mime
*
**************************************************/

#define NCBI_MIME_ASN1 &at[0]
#define NCBI_MIME_ASN1_entrez &at[1]
#define NCBI_MIME_ASN1_alignstruc &at[22]
#define NCBI_MIME_ASN1_alignseq &at[38]
#define NCBI_MIME_ASN1_strucseq &at[46]
#define NCBI_MIME_ASN1_strucseqs &at[53]
#define NCBI_MIME_ASN1_general &at[62]

#define ENTREZ_GENERAL &at[2]
#define ENTREZ_GENERAL_title &at[3]
#define ENTREZ_GENERAL_data &at[5]
#define ENTREZ_GENERAL_data_ml &at[6]
#define ENTREZ_GENERAL_data_prot &at[8]
#define ENTREZ_GENERAL_data_nuc &at[10]
#define ENTREZ_GENERAL_data_genome &at[11]
#define ENTREZ_GENERAL_data_structure &at[12]
#define ENTREZ_GENERAL_data_strucAnnot &at[14]
#define ENTREZ_GENERAL_style &at[17]
#define ENTREZ_GENERAL_location &at[20]

#define BIOSTRUC_ALIGN &at[23]
#define BIOSTRUC_ALIGN_master &at[24]
#define BIOSTRUC_ALIGN_slaves &at[25]
#define BIOSTRUC_ALIGN_slaves_E &at[26]
#define BIOSTRUC_ALIGN_alignments &at[28]
#define BIOSTRUC_ALIGN_sequences &at[29]
#define BIOSTRUC_ALIGN_sequences_E &at[30]
#define BIOSTRUC_ALIGN_seqalign &at[31]
#define BIOSTRUC_ALIGN_seqalign_E &at[32]
#define BIOSTRUC_ALIGN_style_dictionary &at[34]
#define BIOSTRUC_ALIGN_user_annotations &at[36]

#define BIOSTRUC_ALIGN_SEQ &at[39]
#define BIOSTRUC_ALIGN_SEQ_sequences &at[40]
#define BIOSTRUC_ALIGN_SEQ_sequences_E &at[41]
#define BIOSTRUC_ALIGN_SEQ_seqalign &at[42]
#define BIOSTRUC_ALIGN_SEQ_seqalign_E &at[43]
#define ALIGN_SEQ_style_dictionary &at[44]
#define ALIGN_SEQ_user_annotations &at[45]

#define BIOSTRUC_SEQ &at[47]
#define BIOSTRUC_SEQ_structure &at[48]
#define BIOSTRUC_SEQ_sequences &at[49]
#define BIOSTRUC_SEQ_sequences_E &at[50]
#define BIOSTRUC_SEQ_style_dictionary &at[51]
#define BIOSTRUC_SEQ_user_annotations &at[52]

#define BIOSTRUC_SEQS &at[54]
#define BIOSTRUC_SEQS_structure &at[55]
#define BIOSTRUC_SEQS_sequences &at[56]
#define BIOSTRUC_SEQS_sequences_E &at[57]
#define BIOSTRUC_SEQS_seqalign &at[58]
#define BIOSTRUC_SEQS_seqalign_E &at[59]
#define BIOSTRUC_SEQS_style_dictionary &at[60]
#define BIOSTRUC_SEQS_user_annotations &at[61]

#define BIOSTRUC_SEQS_ALIGNS_CDD &at[63]
#define SEQS_ALIGNS_CDD_seq_align_data &at[64]
#define CDD_seq_align_data_bundle &at[65]
#define ALIGNS_CDD_seq_align_data_cdd &at[76]
#define SEQS_ALIGNS_CDD_structures &at[78]
#define SEQS_ALIGNS_CDD_structures_E &at[79]
#define SEQS_ALIGNS_CDD_structure_type &at[80]

#define BUNDLE_SEQS_ALIGNS &at[66]
#define BUNDLE_SEQS_ALIGNS_sequences &at[67]
#define BUNDLE_SEQS_ALIGNS_sequences_E &at[68]
#define BUNDLE_SEQS_ALIGNS_seqaligns &at[69]
#define BUNDLE_SEQS_ALIGNS_seqaligns_E &at[70]
#define BUNDLE_SEQS_ALIGNS_strucaligns &at[71]
#define BUNDLE_SEQS_ALIGNS_imports &at[72]
#define BUNDLE_SEQS_ALIGNS_imports_E &at[73]
#define SEQS_ALIGNS_style_dictionary &at[74]
#define SEQS_ALIGNS_user_annotations &at[75]

#define ENTREZ_STYLE &at[18]
