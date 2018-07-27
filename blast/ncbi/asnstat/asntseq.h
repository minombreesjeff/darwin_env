/***********************************************************************
*
**
*        Automatic header module from ASNTOOL
*
************************************************************************/

#ifndef _ASNTOOL_
#include <asn.h>
#endif

static char * asnfilename = "asntseq.h61";
static AsnValxNode avnx[2] = {
    {20,"nucleotide" ,1,0.0,&avnx[1] } ,
    {20,"protein" ,2,0.0,NULL } };

static AsnType atx[18] = {
  {401, "TSeq" ,1,0,0,0,0,0,0,0,NULL,&atx[14],&atx[1],0,&atx[15]} ,
  {0, "seqtype" ,128,0,0,0,0,0,0,0,NULL,&atx[2],&avnx[0],0,&atx[3]} ,
  {310, "ENUMERATED" ,0,10,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "gi" ,128,1,0,1,0,0,0,0,NULL,&atx[4],NULL,0,&atx[5]} ,
  {302, "INTEGER" ,0,2,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "accver" ,128,2,0,1,0,0,0,0,NULL,&atx[6],NULL,0,&atx[7]} ,
  {323, "VisibleString" ,0,26,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "sid" ,128,3,0,1,0,0,0,0,NULL,&atx[6],NULL,0,&atx[8]} ,
  {0, "local" ,128,4,0,1,0,0,0,0,NULL,&atx[6],NULL,0,&atx[9]} ,
  {0, "taxid" ,128,5,0,1,0,0,0,0,NULL,&atx[4],NULL,0,&atx[10]} ,
  {0, "orgname" ,128,6,0,1,0,0,0,0,NULL,&atx[6],NULL,0,&atx[11]} ,
  {0, "defline" ,128,7,0,0,0,0,0,0,NULL,&atx[6],NULL,0,&atx[12]} ,
  {0, "length" ,128,8,0,0,0,0,0,0,NULL,&atx[4],NULL,0,&atx[13]} ,
  {0, "sequence" ,128,9,0,0,0,0,0,0,NULL,&atx[6],NULL,0,NULL} ,
  {311, "SEQUENCE" ,0,16,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {402, "TSeqSet" ,1,0,0,0,0,0,0,0,NULL,&atx[17],&atx[16],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[0],NULL,0,NULL} ,
  {312, "SEQUENCE OF" ,0,16,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} };

static AsnModule ampx[1] = {
  { "NCBI-TSeq" , "asntseq.h61",&atx[0],NULL,NULL,0,0} };

static AsnValxNodePtr avn = avnx;
static AsnTypePtr at = atx;
static AsnModulePtr amp = ampx;



/**************************************************
*
*    Defines for Module NCBI-TSeq
*
**************************************************/

#define TSEQ &at[0]
#define TSEQ_seqtype &at[1]
#define TSEQ_gi &at[3]
#define TSEQ_accver &at[5]
#define TSEQ_sid &at[7]
#define TSEQ_local &at[8]
#define TSEQ_taxid &at[9]
#define TSEQ_orgname &at[10]
#define TSEQ_defline &at[11]
#define TSEQ_length &at[12]
#define TSEQ_sequence &at[13]

#define TSEQSET &at[15]
#define TSEQSET_E &at[16]
