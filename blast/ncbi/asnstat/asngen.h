/***********************************************************************
*
**
*        Automatic header module from ASNTOOL
*
************************************************************************/

#ifndef _ASNTOOL_
#include <asn.h>
#endif

static char * asnfilename = "asngen.h64";
static AsnValxNode avnx[7] = {
    {20,"unk" ,0,0.0,&avnx[1] } ,
    {20,"gt" ,1,0.0,&avnx[2] } ,
    {20,"lt" ,2,0.0,&avnx[3] } ,
    {20,"tr" ,3,0.0,&avnx[4] } ,
    {20,"tl" ,4,0.0,&avnx[5] } ,
    {20,"circle" ,5,0.0,&avnx[6] } ,
    {20,"other" ,255,0.0,NULL } };

static AsnType atx[77] = {
  {401, "Date" ,1,0,0,0,0,1,0,0,NULL,&atx[14],&atx[1],0,&atx[15]} ,
  {0, "str" ,128,0,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[3]} ,
  {323, "VisibleString" ,0,26,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "std" ,128,1,0,0,0,0,0,0,NULL,&atx[4],NULL,0,NULL} ,
  {407, "Date-std" ,1,0,0,0,0,0,0,0,NULL,&atx[13],&atx[5],0,&atx[24]} ,
  {0, "year" ,128,0,0,0,0,0,0,0,NULL,&atx[6],NULL,0,&atx[7]} ,
  {302, "INTEGER" ,0,2,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "month" ,128,1,0,1,0,0,0,0,NULL,&atx[6],NULL,0,&atx[8]} ,
  {0, "day" ,128,2,0,1,0,0,0,0,NULL,&atx[6],NULL,0,&atx[9]} ,
  {0, "season" ,128,3,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[10]} ,
  {0, "hour" ,128,4,0,1,0,0,0,0,NULL,&atx[6],NULL,0,&atx[11]} ,
  {0, "minute" ,128,5,0,1,0,0,0,0,NULL,&atx[6],NULL,0,&atx[12]} ,
  {0, "second" ,128,6,0,1,0,0,0,0,NULL,&atx[6],NULL,0,NULL} ,
  {311, "SEQUENCE" ,0,16,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {315, "CHOICE" ,0,-1,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {402, "Person-id" ,1,0,0,0,0,1,0,0,NULL,&atx[14],&atx[16],0,&atx[20]} ,
  {0, "dbtag" ,128,0,0,0,0,0,0,0,NULL,&atx[17],NULL,0,&atx[23]} ,
  {404, "Dbtag" ,1,0,0,0,0,1,0,0,NULL,&atx[13],&atx[18],0,&atx[35]} ,
  {0, "db" ,128,0,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[19]} ,
  {0, "tag" ,128,1,0,0,0,0,0,0,NULL,&atx[20],NULL,0,NULL} ,
  {403, "Object-id" ,1,0,0,0,0,1,0,0,NULL,&atx[14],&atx[21],0,&atx[17]} ,
  {0, "id" ,128,0,0,0,0,0,0,0,NULL,&atx[6],NULL,0,&atx[22]} ,
  {0, "str" ,128,1,0,0,0,0,0,0,NULL,&atx[2],NULL,0,NULL} ,
  {0, "name" ,128,1,0,0,0,0,0,0,NULL,&atx[24],NULL,0,&atx[32]} ,
  {408, "Name-std" ,1,0,0,0,0,0,0,0,NULL,&atx[13],&atx[25],0,&atx[51]} ,
  {0, "last" ,128,0,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[26]} ,
  {0, "first" ,128,1,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[27]} ,
  {0, "middle" ,128,2,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[28]} ,
  {0, "full" ,128,3,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[29]} ,
  {0, "initials" ,128,4,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[30]} ,
  {0, "suffix" ,128,5,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[31]} ,
  {0, "title" ,128,6,0,1,0,0,0,0,NULL,&atx[2],NULL,0,NULL} ,
  {0, "ml" ,128,2,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[33]} ,
  {0, "str" ,128,3,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[34]} ,
  {0, "consortium" ,128,4,0,0,0,0,0,0,NULL,&atx[2],NULL,0,NULL} ,
  {405, "Int-fuzz" ,1,0,0,0,0,1,0,0,NULL,&atx[14],&atx[36],0,&atx[46]} ,
  {0, "p-m" ,128,0,0,0,0,0,0,0,NULL,&atx[6],NULL,0,&atx[37]} ,
  {0, "range" ,128,1,0,0,0,0,0,0,NULL,&atx[13],&atx[38],0,&atx[40]} ,
  {0, "max" ,128,0,0,0,0,0,0,0,NULL,&atx[6],NULL,0,&atx[39]} ,
  {0, "min" ,128,1,0,0,0,0,0,0,NULL,&atx[6],NULL,0,NULL} ,
  {0, "pct" ,128,2,0,0,0,0,0,0,NULL,&atx[6],NULL,0,&atx[41]} ,
  {0, "lim" ,128,3,0,0,0,0,0,0,NULL,&atx[42],&avnx[0],0,&atx[43]} ,
  {310, "ENUMERATED" ,0,10,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "alt" ,128,4,0,0,0,0,0,0,NULL,&atx[45],&atx[44],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[6],NULL,0,NULL} ,
  {314, "SET OF" ,0,17,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {406, "User-object" ,1,0,0,0,0,1,0,0,NULL,&atx[13],&atx[47],0,&atx[4]} ,
  {0, "class" ,128,0,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[48]} ,
  {0, "type" ,128,1,0,0,0,0,0,0,NULL,&atx[20],NULL,0,&atx[49]} ,
  {0, "data" ,128,2,0,0,0,0,0,0,NULL,&atx[66],&atx[50],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[51],NULL,0,NULL} ,
  {409, "User-field" ,1,0,0,0,0,0,0,0,NULL,&atx[13],&atx[52],0,NULL} ,
  {0, "label" ,128,0,0,0,0,0,0,0,NULL,&atx[20],NULL,0,&atx[53]} ,
  {0, "num" ,128,1,0,1,0,0,0,0,NULL,&atx[6],NULL,0,&atx[54]} ,
  {0, "data" ,128,2,0,0,0,0,0,0,NULL,&atx[14],&atx[55],0,NULL} ,
  {0, "str" ,128,0,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[56]} ,
  {0, "int" ,128,1,0,0,0,0,0,0,NULL,&atx[6],NULL,0,&atx[57]} ,
  {0, "real" ,128,2,0,0,0,0,0,0,NULL,&atx[58],NULL,0,&atx[59]} ,
  {309, "REAL" ,0,9,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "bool" ,128,3,0,0,0,0,0,0,NULL,&atx[60],NULL,0,&atx[61]} ,
  {301, "BOOLEAN" ,0,1,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "os" ,128,4,0,0,0,0,0,0,NULL,&atx[62],NULL,0,&atx[63]} ,
  {304, "OCTET STRING" ,0,4,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "object" ,128,5,0,0,0,0,0,0,NULL,&atx[46],NULL,0,&atx[64]} ,
  {0, "strs" ,128,6,0,0,0,0,0,0,NULL,&atx[66],&atx[65],0,&atx[67]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[2],NULL,0,NULL} ,
  {312, "SEQUENCE OF" ,0,16,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "ints" ,128,7,0,0,0,0,0,0,NULL,&atx[66],&atx[68],0,&atx[69]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[6],NULL,0,NULL} ,
  {0, "reals" ,128,8,0,0,0,0,0,0,NULL,&atx[66],&atx[70],0,&atx[71]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[58],NULL,0,NULL} ,
  {0, "oss" ,128,9,0,0,0,0,0,0,NULL,&atx[66],&atx[72],0,&atx[73]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[62],NULL,0,NULL} ,
  {0, "fields" ,128,10,0,0,0,0,0,0,NULL,&atx[66],&atx[74],0,&atx[75]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[51],NULL,0,NULL} ,
  {0, "objects" ,128,11,0,0,0,0,0,0,NULL,&atx[66],&atx[76],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[46],NULL,0,NULL} };

static AsnModule ampx[1] = {
  { "NCBI-General" , "asngen.h64",&atx[0],NULL,NULL,0,0} };

static AsnValxNodePtr avn = avnx;
static AsnTypePtr at = atx;
static AsnModulePtr amp = ampx;



/**************************************************
*
*    Defines for Module NCBI-General
*
**************************************************/

#define DATE &at[0]
#define DATE_str &at[1]
#define DATE_std &at[3]

#define PERSON_ID &at[15]
#define PERSON_ID_dbtag &at[16]
#define PERSON_ID_name &at[23]
#define PERSON_ID_ml &at[32]
#define PERSON_ID_str &at[33]
#define PERSON_ID_consortium &at[34]

#define OBJECT_ID &at[20]
#define OBJECT_ID_id &at[21]
#define OBJECT_ID_str &at[22]

#define DBTAG &at[17]
#define DBTAG_db &at[18]
#define DBTAG_tag &at[19]

#define INT_FUZZ &at[35]
#define INT_FUZZ_p_m &at[36]
#define INT_FUZZ_range &at[37]
#define INT_FUZZ_range_max &at[38]
#define INT_FUZZ_range_min &at[39]
#define INT_FUZZ_pct &at[40]
#define INT_FUZZ_lim &at[41]
#define INT_FUZZ_alt &at[43]
#define INT_FUZZ_alt_E &at[44]

#define USER_OBJECT &at[46]
#define USER_OBJECT_class &at[47]
#define USER_OBJECT_type &at[48]
#define USER_OBJECT_data &at[49]
#define USER_OBJECT_data_E &at[50]

#define DATE_STD &at[4]
#define DATE_STD_year &at[5]
#define DATE_STD_month &at[7]
#define DATE_STD_day &at[8]
#define DATE_STD_season &at[9]
#define DATE_STD_hour &at[10]
#define DATE_STD_minute &at[11]
#define DATE_STD_second &at[12]

#define NAME_STD &at[24]
#define NAME_STD_last &at[25]
#define NAME_STD_first &at[26]
#define NAME_STD_middle &at[27]
#define NAME_STD_full &at[28]
#define NAME_STD_initials &at[29]
#define NAME_STD_suffix &at[30]
#define NAME_STD_title &at[31]

#define USER_FIELD &at[51]
#define USER_FIELD_label &at[52]
#define USER_FIELD_num &at[53]
#define USER_FIELD_data &at[54]
#define USER_FIELD_data_str &at[55]
#define USER_FIELD_data_int &at[56]
#define USER_FIELD_data_real &at[57]
#define USER_FIELD_data_bool &at[59]
#define USER_FIELD_data_os &at[61]
#define USER_FIELD_data_object &at[63]
#define USER_FIELD_data_strs &at[64]
#define USER_FIELD_data_strs_E &at[65]
#define USER_FIELD_data_ints &at[67]
#define USER_FIELD_data_ints_E &at[68]
#define USER_FIELD_data_reals &at[69]
#define USER_FIELD_data_reals_E &at[70]
#define USER_FIELD_data_oss &at[71]
#define USER_FIELD_data_oss_E &at[72]
#define USER_FIELD_data_fields &at[73]
#define USER_FIELD_data_fields_E &at[74]
#define USER_FIELD_data_objects &at[75]
#define USER_FIELD_data_objects_E &at[76]
