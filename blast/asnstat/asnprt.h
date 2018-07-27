/***********************************************************************
*
**
*        Automatic header module from ASNTOOL
*
************************************************************************/

#ifndef _ASNTOOL_
#include <asn.h>
#endif

static char * asnfilename = "asnprt.h60";
static AsnType atx[41] = {
  {401, "PrintTemplate" ,1,0,0,0,0,1,0,0,NULL,&atx[19],&atx[1],0,&atx[39]} ,
  {0, "name" ,128,0,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[4]} ,
  {403, "TemplateName" ,1,0,0,0,0,0,0,0,NULL,&atx[3],NULL,0,&atx[6]} ,
  {323, "VisibleString" ,0,26,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "labelfrom" ,128,1,0,1,0,0,0,0,NULL,&atx[3],NULL,0,&atx[5]} ,
  {0, "format" ,128,2,0,0,0,0,0,0,NULL,&atx[6],NULL,0,NULL} ,
  {404, "PrintFormat" ,1,0,0,0,0,0,0,0,NULL,&atx[19],&atx[7],0,&atx[12]} ,
  {0, "asn1" ,128,0,0,0,0,0,0,0,NULL,&atx[3],NULL,0,&atx[8]} ,
  {0, "label" ,128,1,0,1,0,0,0,0,NULL,&atx[3],NULL,0,&atx[9]} ,
  {0, "prefix" ,128,2,0,1,0,0,0,0,NULL,&atx[3],NULL,0,&atx[10]} ,
  {0, "suffix" ,128,3,0,1,0,0,0,0,NULL,&atx[3],NULL,0,&atx[11]} ,
  {0, "form" ,128,4,0,0,0,0,0,0,NULL,&atx[12],NULL,0,NULL} ,
  {405, "PrintForm" ,1,0,0,0,0,0,0,0,NULL,&atx[38],&atx[13],0,&atx[14]} ,
  {0, "block" ,128,0,0,0,0,0,0,0,NULL,&atx[14],NULL,0,&atx[20]} ,
  {406, "PrintFormBlock" ,1,0,0,0,0,0,0,0,NULL,&atx[19],&atx[15],0,&atx[21]} ,
  {0, "separator" ,128,0,0,1,0,0,0,0,NULL,&atx[3],NULL,0,&atx[16]} ,
  {0, "components" ,128,1,0,0,0,0,0,0,NULL,&atx[18],&atx[17],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[6],NULL,0,NULL} ,
  {312, "SEQUENCE OF" ,0,16,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {311, "SEQUENCE" ,0,16,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "boolean" ,128,1,0,0,0,0,0,0,NULL,&atx[21],NULL,0,&atx[24]} ,
  {407, "PrintFormBoolean" ,1,0,0,0,0,0,0,0,NULL,&atx[19],&atx[22],0,&atx[25]} ,
  {0, "true" ,128,0,0,1,0,0,0,0,NULL,&atx[3],NULL,0,&atx[23]} ,
  {0, "false" ,128,1,0,1,0,0,0,0,NULL,&atx[3],NULL,0,NULL} ,
  {0, "enum" ,128,2,0,0,0,0,0,0,NULL,&atx[25],NULL,0,&atx[28]} ,
  {408, "PrintFormEnum" ,1,0,0,0,0,0,0,0,NULL,&atx[19],&atx[26],0,&atx[29]} ,
  {0, "values" ,128,0,0,1,0,0,0,0,NULL,&atx[18],&atx[27],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[3],NULL,0,NULL} ,
  {0, "text" ,128,3,0,0,0,0,0,0,NULL,&atx[29],NULL,0,&atx[31]} ,
  {409, "PrintFormText" ,1,0,0,0,0,0,0,0,NULL,&atx[19],&atx[30],0,&atx[33]} ,
  {0, "textfunc" ,128,0,0,1,0,0,0,0,NULL,&atx[3],NULL,0,NULL} ,
  {0, "use-template" ,128,4,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[32]} ,
  {0, "user" ,128,5,0,0,0,0,0,0,NULL,&atx[33],NULL,0,&atx[36]} ,
  {410, "UserFormat" ,1,0,0,0,0,0,0,0,NULL,&atx[19],&atx[34],0,NULL} ,
  {0, "printfunc" ,128,0,0,0,0,0,0,0,NULL,&atx[3],NULL,0,&atx[35]} ,
  {0, "defaultfunc" ,128,1,0,1,0,0,0,0,NULL,&atx[3],NULL,0,NULL} ,
  {0, "null" ,128,6,0,0,0,0,0,0,NULL,&atx[37],NULL,0,NULL} ,
  {305, "NULL" ,0,5,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {315, "CHOICE" ,0,-1,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {402, "PrintTemplateSet" ,1,0,0,0,0,1,0,0,NULL,&atx[18],&atx[40],0,&atx[2]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[0],NULL,0,NULL} };

static AsnModule ampx[1] = {
  { "NCBI-ObjPrt" , "asnprt.h60",&atx[0],NULL,NULL,0,0} };

static AsnValxNodePtr avn = NULL;
static AsnTypePtr at = atx;
static AsnModulePtr amp = ampx;



/**************************************************
*
*    Defines for Module NCBI-ObjPrt
*
**************************************************/

#define PRINTTEMPLATE &at[0]
#define PRINTTEMPLATE_name &at[1]
#define PRINTTEMPLATE_labelfrom &at[4]
#define PRINTTEMPLATE_format &at[5]

#define PRINTTEMPLATESET &at[39]
#define PRINTTEMPLATESET_E &at[40]

#define TEMPLATENAME &at[2]

#define PRINTFORMAT &at[6]
#define PRINTFORMAT_asn1 &at[7]
#define PRINTFORMAT_label &at[8]
#define PRINTFORMAT_prefix &at[9]
#define PRINTFORMAT_suffix &at[10]
#define PRINTFORMAT_form &at[11]

#define PRINTFORM &at[12]
#define PRINTFORM_block &at[13]
#define PRINTFORM_boolean &at[20]
#define PRINTFORM_enum &at[24]
#define PRINTFORM_text &at[28]
#define PRINTFORM_use_template &at[31]
#define PRINTFORM_user &at[32]
#define PRINTFORM_null &at[36]

#define PRINTFORMBLOCK &at[14]
#define PRINTFORMBLOCK_separator &at[15]
#define PRINTFORMBLOCK_components &at[16]
#define PRINTFORMBLOCK_components_E &at[17]

#define PRINTFORMBOOLEAN &at[21]
#define PRINTFORMBOOLEAN_true &at[22]
#define PRINTFORMBOOLEAN_false &at[23]

#define PRINTFORMENUM &at[25]
#define PRINTFORMENUM_values &at[26]
#define PRINTFORMENUM_values_E &at[27]

#define PRINTFORMTEXT &at[29]
#define PRINTFORMTEXT_textfunc &at[30]

#define USERFORMAT &at[33]
#define USERFORMAT_printfunc &at[34]
#define USERFORMAT_defaultfunc &at[35]
