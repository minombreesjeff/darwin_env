/***********************************************************************
*
**
*        Automatic header module from ASNTOOL
*
************************************************************************/

#ifndef _ASNTOOL_
#include <asn.h>
#endif

static char * asnfilename = "asnpub.h60";
static AsnType atx[45] = {
  {401, "Pub" ,1,0,0,0,0,1,0,0,NULL,&atx[29],&atx[1],0,&atx[30]} ,
  {0, "gen" ,128,0,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[3]} ,
  {411, "Cit-gen" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[22]} ,
  {0, "sub" ,128,1,0,0,0,0,0,0,NULL,&atx[4],NULL,0,&atx[5]} ,
  {413, "Cit-sub" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[28]} ,
  {0, "medline" ,128,2,0,0,0,0,0,0,NULL,&atx[6],NULL,0,&atx[7]} ,
  {404, "Medline-entry" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[10]} ,
  {0, "muid" ,128,3,0,0,0,0,0,0,NULL,&atx[8],NULL,0,&atx[9]} ,
  {302, "INTEGER" ,0,2,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "article" ,128,4,0,0,0,0,0,0,NULL,&atx[10],NULL,0,&atx[11]} ,
  {405, "Cit-art" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[12]} ,
  {0, "journal" ,128,5,0,0,0,0,0,0,NULL,&atx[12],NULL,0,&atx[13]} ,
  {406, "Cit-jour" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[14]} ,
  {0, "book" ,128,6,0,0,0,0,0,0,NULL,&atx[14],NULL,0,&atx[15]} ,
  {407, "Cit-book" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[16]} ,
  {0, "proc" ,128,7,0,0,0,0,0,0,NULL,&atx[16],NULL,0,&atx[17]} ,
  {408, "Cit-proc" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[18]} ,
  {0, "patent" ,128,8,0,0,0,0,0,0,NULL,&atx[18],NULL,0,&atx[19]} ,
  {409, "Cit-pat" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[20]} ,
  {0, "pat-id" ,128,9,0,0,0,0,0,0,NULL,&atx[20],NULL,0,&atx[21]} ,
  {410, "Id-pat" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[2]} ,
  {0, "man" ,128,10,0,0,0,0,0,0,NULL,&atx[22],NULL,0,&atx[23]} ,
  {412, "Cit-let" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[4]} ,
  {0, "equiv" ,128,11,0,0,0,0,0,0,NULL,&atx[24],NULL,0,&atx[27]} ,
  {403, "Pub-equiv" ,1,0,0,0,0,1,0,0,NULL,&atx[26],&atx[25],0,&atx[6]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[0],NULL,0,NULL} ,
  {314, "SET OF" ,0,17,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "pmid" ,128,12,0,0,0,0,0,0,NULL,&atx[28],NULL,0,NULL} ,
  {414, "PubMedId" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,NULL} ,
  {315, "CHOICE" ,0,-1,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {402, "Pub-set" ,1,0,0,0,0,1,0,0,NULL,&atx[29],&atx[31],0,&atx[24]} ,
  {0, "pub" ,128,0,0,0,0,0,0,0,NULL,&atx[26],&atx[32],0,&atx[33]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[0],NULL,0,NULL} ,
  {0, "medline" ,128,1,0,0,0,0,0,0,NULL,&atx[26],&atx[34],0,&atx[35]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[6],NULL,0,NULL} ,
  {0, "article" ,128,2,0,0,0,0,0,0,NULL,&atx[26],&atx[36],0,&atx[37]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[10],NULL,0,NULL} ,
  {0, "journal" ,128,3,0,0,0,0,0,0,NULL,&atx[26],&atx[38],0,&atx[39]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[12],NULL,0,NULL} ,
  {0, "book" ,128,4,0,0,0,0,0,0,NULL,&atx[26],&atx[40],0,&atx[41]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[14],NULL,0,NULL} ,
  {0, "proc" ,128,5,0,0,0,0,0,0,NULL,&atx[26],&atx[42],0,&atx[43]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[16],NULL,0,NULL} ,
  {0, "patent" ,128,6,0,0,0,0,0,0,NULL,&atx[26],&atx[44],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[18],NULL,0,NULL} };

static AsnModule ampx[1] = {
  { "NCBI-Pub" , "asnpub.h60",&atx[0],NULL,NULL,0,0} };

static AsnValxNodePtr avn = NULL;
static AsnTypePtr at = atx;
static AsnModulePtr amp = ampx;



/**************************************************
*
*    Defines for Module NCBI-Pub
*
**************************************************/

#define PUB &at[0]
#define PUB_gen &at[1]
#define PUB_sub &at[3]
#define PUB_medline &at[5]
#define PUB_muid &at[7]
#define PUB_article &at[9]
#define PUB_journal &at[11]
#define PUB_book &at[13]
#define PUB_proc &at[15]
#define PUB_patent &at[17]
#define PUB_pat_id &at[19]
#define PUB_man &at[21]
#define PUB_equiv &at[23]
#define PUB_pmid &at[27]

#define PUB_SET &at[30]
#define PUB_SET_pub &at[31]
#define PUB_SET_pub_E &at[32]
#define PUB_SET_medline &at[33]
#define PUB_SET_medline_E &at[34]
#define PUB_SET_article &at[35]
#define PUB_SET_article_E &at[36]
#define PUB_SET_journal &at[37]
#define PUB_SET_journal_E &at[38]
#define PUB_SET_book &at[39]
#define PUB_SET_book_E &at[40]
#define PUB_SET_proc &at[41]
#define PUB_SET_proc_E &at[42]
#define PUB_SET_patent &at[43]
#define PUB_SET_patent_E &at[44]

#define PUB_EQUIV &at[24]
#define PUB_EQUIV_E &at[25]
