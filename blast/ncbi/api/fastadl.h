/***********************************************************************
*
**
*        Automatic header module from ASNTOOL
*
************************************************************************/

#ifndef _ASNTOOL_
#include <asn.h>
#endif

static char * asnfilename = "fastadl.h";
static AsnType atx[18] = {
  {401, "Blast-def-line-set" ,1,0,0,0,0,1,0,0,NULL,&atx[8],&atx[1],0,&atx[2]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[2],NULL,0,NULL} ,
  {402, "Blast-def-line" ,1,0,0,0,0,1,0,0,NULL,&atx[17],&atx[3],0,&atx[7]} ,
  {0, "title" ,128,0,0,1,0,0,0,0,NULL,&atx[4],NULL,0,&atx[5]} ,
  {323, "VisibleString" ,0,26,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "seqid" ,128,1,0,0,0,0,0,0,NULL,&atx[8],&atx[6],0,&atx[9]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[7],NULL,0,NULL} ,
  {403, "Seq-id" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,NULL} ,
  {312, "SEQUENCE OF" ,0,16,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "taxid" ,128,2,0,1,0,0,0,0,NULL,&atx[10],NULL,0,&atx[11]} ,
  {302, "INTEGER" ,0,2,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "memberships" ,128,3,0,1,0,0,0,0,NULL,&atx[8],&atx[12],0,&atx[13]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[10],NULL,0,NULL} ,
  {0, "links" ,128,4,0,1,0,0,0,0,NULL,&atx[8],&atx[14],0,&atx[15]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[10],NULL,0,NULL} ,
  {0, "other-info" ,128,5,0,1,0,0,0,0,NULL,&atx[8],&atx[16],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[10],NULL,0,NULL} ,
  {311, "SEQUENCE" ,0,16,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} };

static AsnModule ampx[1] = {
  { "NCBI-BlastDL" , "fastadl.h",&atx[0],NULL,NULL,0,0} };

static AsnValxNodePtr avn = NULL;
static AsnTypePtr at = atx;
static AsnModulePtr amp = ampx;



/**************************************************
*
*    Defines for Module NCBI-BlastDL
*
**************************************************/

#define BLAST_DEF_LINE_SET &at[0]
#define BLAST_DEF_LINE_SET_E &at[1]

#define BLAST_DEF_LINE &at[2]
#define BLAST_DEF_LINE_title &at[3]
#define BLAST_DEF_LINE_seqid &at[5]
#define BLAST_DEF_LINE_seqid_E &at[6]
#define BLAST_DEF_LINE_taxid &at[9]
#define BLAST_DEF_LINE_memberships &at[11]
#define BLAST_DEF_LINE_memberships_E &at[12]
#define BLAST_DEF_LINE_links &at[13]
#define BLAST_DEF_LINE_links_E &at[14]
#define BLAST_DEF_LINE_other_info &at[15]
#define BLAST_DEF_LINE_other_info_E &at[16]
