/***********************************************************************
*
**
*        Automatic header module from ASNTOOL
*
************************************************************************/

#ifndef _ASNTOOL_
#include <asn.h>
#endif

static char * asnfilename = "asnfdef.h60";
static AsnValxNode avnx[3] = {
    {20,"aa" ,1,0.0,&avnx[1] } ,
    {20,"na" ,2,0.0,&avnx[2] } ,
    {20,"both" ,3,0.0,NULL } };

static AsnType atx[24] = {
  {401, "FeatDef" ,1,0,0,0,0,1,0,0,NULL,&atx[12],&atx[1],0,&atx[13]} ,
  {0, "typelabel" ,128,0,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[3]} ,
  {323, "VisibleString" ,0,26,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "menulabel" ,128,1,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[4]} ,
  {0, "featdef-key" ,128,2,0,0,0,0,0,0,NULL,&atx[5],NULL,0,&atx[6]} ,
  {302, "INTEGER" ,0,2,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "seqfeat-key" ,128,3,0,0,0,0,0,0,NULL,&atx[5],NULL,0,&atx[7]} ,
  {0, "entrygroup" ,128,4,0,0,0,0,0,0,NULL,&atx[5],NULL,0,&atx[8]} ,
  {0, "displaygroup" ,128,5,0,0,0,0,0,0,NULL,&atx[5],NULL,0,&atx[9]} ,
  {0, "molgroup" ,128,6,0,0,0,0,0,0,NULL,&atx[10],NULL,0,NULL} ,
  {405, "FeatMolType" ,1,0,0,0,0,0,0,0,NULL,&atx[11],&avnx[0],0,&atx[21]} ,
  {310, "ENUMERATED" ,0,10,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {311, "SEQUENCE" ,0,16,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {402, "FeatDefSet" ,1,0,0,0,0,1,0,0,NULL,&atx[15],&atx[14],0,&atx[16]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[0],NULL,0,NULL} ,
  {312, "SEQUENCE OF" ,0,16,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {403, "FeatDispGroup" ,1,0,0,0,0,1,0,0,NULL,&atx[12],&atx[17],0,&atx[19]} ,
  {0, "groupkey" ,128,0,0,0,0,0,0,0,NULL,&atx[5],NULL,0,&atx[18]} ,
  {0, "groupname" ,128,1,0,0,0,0,0,0,NULL,&atx[2],NULL,0,NULL} ,
  {404, "FeatDispGroupSet" ,1,0,0,0,0,1,0,0,NULL,&atx[15],&atx[20],0,&atx[10]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[16],NULL,0,NULL} ,
  {406, "FeatDefGroupSet" ,1,0,0,0,0,0,0,0,NULL,&atx[12],&atx[22],0,NULL} ,
  {0, "groups" ,128,0,0,0,0,0,0,0,NULL,&atx[19],NULL,0,&atx[23]} ,
  {0, "defs" ,128,1,0,0,0,0,0,0,NULL,&atx[13],NULL,0,NULL} };

static AsnModule ampx[1] = {
  { "NCBI-FeatDef" , "asnfdef.h60",&atx[0],NULL,NULL,0,0} };

static AsnValxNodePtr avn = avnx;
static AsnTypePtr at = atx;
static AsnModulePtr amp = ampx;



/**************************************************
*
*    Defines for Module NCBI-FeatDef
*
**************************************************/

#define FEATDEF &at[0]
#define FEATDEF_typelabel &at[1]
#define FEATDEF_menulabel &at[3]
#define FEATDEF_featdef_key &at[4]
#define FEATDEF_seqfeat_key &at[6]
#define FEATDEF_entrygroup &at[7]
#define FEATDEF_displaygroup &at[8]
#define FEATDEF_molgroup &at[9]

#define FEATDEFSET &at[13]
#define FEATDEFSET_E &at[14]

#define FEATDISPGROUP &at[16]
#define FEATDISPGROUP_groupkey &at[17]
#define FEATDISPGROUP_groupname &at[18]

#define FEATDISPGROUPSET &at[19]
#define FEATDISPGROUPSET_E &at[20]

#define FEATMOLTYPE &at[10]

#define FEATDEFGROUPSET &at[21]
#define FEATDEFGROUPSET_groups &at[22]
#define FEATDEFGROUPSET_defs &at[23]
