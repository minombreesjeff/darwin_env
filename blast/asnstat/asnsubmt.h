/***********************************************************************
*
**
*        Automatic header module from ASNTOOL
*
************************************************************************/

#ifndef _ASNTOOL_
#include <asn.h>
#endif

static char * asnfilename = "asnsubmt.h61";
static AsnValxNode avnx[5] = {
    {2,NULL,0,0.0,NULL } ,
    {20,"new" ,1,0.0,&avnx[2] } ,
    {20,"update" ,2,0.0,&avnx[3] } ,
    {20,"revision" ,3,0.0,&avnx[4] } ,
    {20,"other" ,255,0.0,NULL } };

static AsnType atx[47] = {
  {401, "Seq-submit" ,1,0,0,0,0,1,0,0,NULL,&atx[23],&atx[1],0,&atx[4]} ,
  {0, "sub" ,128,0,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[35]} ,
  {410, "Submit-block" ,1,0,0,0,0,0,0,0,NULL,&atx[23],&atx[3],0,NULL} ,
  {0, "contact" ,128,0,0,0,0,0,0,0,NULL,&atx[4],NULL,0,&atx[24]} ,
  {402, "Contact-info" ,1,0,0,0,0,1,0,0,NULL,&atx[23],&atx[5],0,&atx[25]} ,
  {0, "name" ,128,0,0,1,0,0,0,0,NULL,&atx[6],NULL,0,&atx[7]} ,
  {323, "VisibleString" ,0,26,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "address" ,128,1,0,1,0,0,0,0,NULL,&atx[9],&atx[8],0,&atx[10]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[6],NULL,0,NULL} ,
  {312, "SEQUENCE OF" ,0,16,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "phone" ,128,2,0,1,0,0,0,0,NULL,&atx[6],NULL,0,&atx[11]} ,
  {0, "fax" ,128,3,0,1,0,0,0,0,NULL,&atx[6],NULL,0,&atx[12]} ,
  {0, "email" ,128,4,0,1,0,0,0,0,NULL,&atx[6],NULL,0,&atx[13]} ,
  {0, "telex" ,128,5,0,1,0,0,0,0,NULL,&atx[6],NULL,0,&atx[14]} ,
  {0, "owner-id" ,128,6,0,1,0,0,0,0,NULL,&atx[15],NULL,0,&atx[16]} ,
  {406, "Object-id" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[42]} ,
  {0, "password" ,128,7,0,1,0,0,0,0,NULL,&atx[17],NULL,0,&atx[18]} ,
  {304, "OCTET STRING" ,0,4,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "last-name" ,128,8,0,1,0,0,0,0,NULL,&atx[6],NULL,0,&atx[19]} ,
  {0, "first-name" ,128,9,0,1,0,0,0,0,NULL,&atx[6],NULL,0,&atx[20]} ,
  {0, "middle-initial" ,128,10,0,1,0,0,0,0,NULL,&atx[6],NULL,0,&atx[21]} ,
  {0, "contact" ,128,11,0,1,0,0,0,0,NULL,&atx[22],NULL,0,NULL} ,
  {404, "Author" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[29]} ,
  {311, "SEQUENCE" ,0,16,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "cit" ,128,1,0,0,0,0,0,0,NULL,&atx[25],NULL,0,&atx[26]} ,
  {403, "Cit-sub" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[22]} ,
  {0, "hup" ,128,2,0,0,1,0,0,0,&avnx[0],&atx[27],NULL,0,&atx[28]} ,
  {301, "BOOLEAN" ,0,1,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "reldate" ,128,3,0,1,0,0,0,0,NULL,&atx[29],NULL,0,&atx[30]} ,
  {405, "Date" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[15]} ,
  {0, "subtype" ,128,4,0,1,0,0,0,0,NULL,&atx[31],&avnx[1],0,&atx[32]} ,
  {302, "INTEGER" ,0,2,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "tool" ,128,5,0,1,0,0,0,0,NULL,&atx[6],NULL,0,&atx[33]} ,
  {0, "user-tag" ,128,6,0,1,0,0,0,0,NULL,&atx[6],NULL,0,&atx[34]} ,
  {0, "comment" ,128,7,0,1,0,0,0,0,NULL,&atx[6],NULL,0,NULL} ,
  {0, "data" ,128,1,0,0,0,0,0,0,NULL,&atx[46],&atx[36],0,NULL} ,
  {0, "entrys" ,128,0,0,0,0,0,0,0,NULL,&atx[39],&atx[37],0,&atx[40]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[38],NULL,0,NULL} ,
  {409, "Seq-entry" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[2]} ,
  {314, "SET OF" ,0,17,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "annots" ,128,1,0,0,0,0,0,0,NULL,&atx[39],&atx[41],0,&atx[43]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[42],NULL,0,NULL} ,
  {407, "Seq-annot" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[45]} ,
  {0, "delete" ,128,2,0,0,0,0,0,0,NULL,&atx[39],&atx[44],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[45],NULL,0,NULL} ,
  {408, "Seq-id" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[38]} ,
  {315, "CHOICE" ,0,-1,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} };

static AsnModule ampx[1] = {
  { "NCBI-Submit" , "asnsubmt.h61",&atx[0],NULL,NULL,0,0} };

static AsnValxNodePtr avn = avnx;
static AsnTypePtr at = atx;
static AsnModulePtr amp = ampx;



/**************************************************
*
*    Defines for Module NCBI-Submit
*
**************************************************/

#define SEQ_SUBMIT &at[0]
#define SEQ_SUBMIT_sub &at[1]
#define SEQ_SUBMIT_data &at[35]
#define SEQ_SUBMIT_data_entrys &at[36]
#define SEQ_SUBMIT_data_entrys_E &at[37]
#define SEQ_SUBMIT_data_annots &at[40]
#define SEQ_SUBMIT_data_annots_E &at[41]
#define SEQ_SUBMIT_data_delete &at[43]
#define SEQ_SUBMIT_data_delete_E &at[44]

#define CONTACT_INFO &at[4]
#define CONTACT_INFO_name &at[5]
#define CONTACT_INFO_address &at[7]
#define CONTACT_INFO_address_E &at[8]
#define CONTACT_INFO_phone &at[10]
#define CONTACT_INFO_fax &at[11]
#define CONTACT_INFO_email &at[12]
#define CONTACT_INFO_telex &at[13]
#define CONTACT_INFO_owner_id &at[14]
#define CONTACT_INFO_password &at[16]
#define CONTACT_INFO_last_name &at[18]
#define CONTACT_INFO_first_name &at[19]
#define CONTACT_INFO_middle_initial &at[20]
#define CONTACT_INFO_contact &at[21]

#define SUBMIT_BLOCK &at[2]
#define SUBMIT_BLOCK_contact &at[3]
#define SUBMIT_BLOCK_cit &at[24]
#define SUBMIT_BLOCK_hup &at[26]
#define SUBMIT_BLOCK_reldate &at[28]
#define SUBMIT_BLOCK_subtype &at[30]
#define SUBMIT_BLOCK_tool &at[32]
#define SUBMIT_BLOCK_user_tag &at[33]
#define SUBMIT_BLOCK_comment &at[34]
