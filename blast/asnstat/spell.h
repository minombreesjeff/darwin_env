/***********************************************************************
*
**
*        Automatic header module from ASNTOOL
*
************************************************************************/

#ifndef _ASNTOOL_
#include <asn.h>
#endif

static char * asnfilename = "spell.h";
static AsnType atx[15] = {
  {401, "Spell-request" ,1,0,0,0,0,0,0,0,NULL,&atx[6],&atx[1],0,&atx[7]} ,
  {0, "init" ,128,0,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[3]} ,
  {305, "NULL" ,0,5,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "spelltext" ,128,1,0,0,0,0,0,0,NULL,&atx[4],NULL,0,&atx[5]} ,
  {323, "VisibleString" ,0,26,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "fini" ,128,2,0,0,0,0,0,0,NULL,&atx[2],NULL,0,NULL} ,
  {315, "CHOICE" ,0,-1,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {402, "Spell-response" ,1,0,0,0,0,0,0,0,NULL,&atx[6],&atx[8],0,NULL} ,
  {0, "error" ,128,0,0,0,0,0,0,0,NULL,&atx[9],NULL,0,&atx[10]} ,
  {302, "INTEGER" ,0,2,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "init" ,128,1,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[11]} ,
  {0, "spelltext" ,128,2,0,0,0,0,0,0,NULL,&atx[13],&atx[12],0,&atx[14]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[4],NULL,0,NULL} ,
  {312, "SEQUENCE OF" ,0,16,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "fini" ,128,3,0,0,0,0,0,0,NULL,&atx[2],NULL,0,NULL} };

static AsnModule ampx[1] = {
  { "NCBI-SPELL" , "spell.h",&atx[0],NULL,NULL,0,0} };

static AsnValxNodePtr avn = NULL;
static AsnTypePtr at = atx;
static AsnModulePtr amp = ampx;



/**************************************************
*
*    Defines for Module NCBI-SPELL
*
**************************************************/

#define SPELL_REQUEST &at[0]
#define SPELL_REQUEST_init &at[1]
#define SPELL_REQUEST_spelltext &at[3]
#define SPELL_REQUEST_fini &at[5]

#define SPELL_RESPONSE &at[7]
#define SPELL_RESPONSE_error &at[8]
#define SPELL_RESPONSE_init &at[10]
#define SPELL_RESPONSE_spelltext &at[11]
#define SPELL_RESPONSE_spelltext_E &at[12]
#define SPELL_RESPONSE_fini &at[14]
