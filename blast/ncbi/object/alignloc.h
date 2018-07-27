/***********************************************************************
*
**
*        Automatic header module from ASNTOOL
*
************************************************************************/

#ifndef _ASNTOOL_
#include <asn.h>
#endif

static char * asnfilename = "alignloc.h11";
static AsnValxNode avnx[7] = {
    {20,"unk" ,0,0.0,&avnx[1] } ,
    {20,"gt" ,1,0.0,&avnx[2] } ,
    {20,"lt" ,2,0.0,&avnx[3] } ,
    {20,"tr" ,3,0.0,&avnx[4] } ,
    {20,"tl" ,4,0.0,&avnx[5] } ,
    {20,"circle" ,5,0.0,&avnx[6] } ,
    {20,"other" ,255,0.0,NULL } };

static AsnType atx[99] = {
  {401, "Align-loc-set" ,1,0,0,0,0,1,0,0,NULL,&atx[33],&atx[1],0,&atx[2]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[2],NULL,0,NULL} ,
  {402, "Align-loc" ,1,0,0,0,0,1,0,0,NULL,&atx[24],&atx[3],0,&atx[4]} ,
  {0, "id" ,128,0,0,1,0,0,0,0,NULL,&atx[4],NULL,0,&atx[15]} ,
  {403, "Align-id" ,1,0,0,0,0,1,0,0,NULL,&atx[12],&atx[5],0,&atx[6]} ,
  {0, "id" ,128,0,0,0,0,0,0,0,NULL,&atx[6],NULL,0,&atx[13]} ,
  {404, "Object-id" ,1,0,0,0,0,0,1,0,NULL,&atx[7],NULL,0,&atx[19]} ,
  {403, "Object-id" ,1,0,0,0,0,1,0,0,NULL,&atx[12],&atx[8],0,&atx[44]} ,
  {0, "id" ,128,0,0,0,0,0,0,0,NULL,&atx[9],NULL,0,&atx[10]} ,
  {302, "INTEGER" ,0,2,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "str" ,128,1,0,0,0,0,0,0,NULL,&atx[11],NULL,0,NULL} ,
  {323, "VisibleString" ,0,26,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {315, "CHOICE" ,0,-1,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "itemid" ,128,1,0,0,0,0,0,0,NULL,&atx[9],NULL,0,&atx[14]} ,
  {0, "gi" ,128,2,0,0,0,0,0,0,NULL,&atx[9],NULL,0,NULL} ,
  {0, "loc" ,128,1,0,0,0,0,0,0,NULL,&atx[12],&atx[16],0,NULL} ,
  {0, "whole" ,128,0,0,0,0,0,0,0,NULL,&atx[17],NULL,0,&atx[18]} ,
  {305, "NULL" ,0,5,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "box" ,128,1,0,0,0,0,0,0,NULL,&atx[19],NULL,0,&atx[25]} ,
  {405, "Align-box" ,1,0,0,0,0,0,0,0,NULL,&atx[24],&atx[20],0,&atx[26]} ,
  {0, "row-from" ,128,0,0,0,0,0,0,0,NULL,&atx[9],NULL,0,&atx[21]} ,
  {0, "row-to" ,128,1,0,0,0,0,0,0,NULL,&atx[9],NULL,0,&atx[22]} ,
  {0, "column-from" ,128,2,0,0,0,0,0,0,NULL,&atx[9],NULL,0,&atx[23]} ,
  {0, "column-to" ,128,3,0,0,0,0,0,0,NULL,&atx[9],NULL,0,NULL} ,
  {311, "SEQUENCE" ,0,16,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "rows" ,128,2,0,0,0,0,0,0,NULL,&atx[26],NULL,0,&atx[29]} ,
  {406, "Align-rows" ,1,0,0,0,0,0,0,0,NULL,&atx[24],&atx[27],0,&atx[30]} ,
  {0, "from" ,128,0,0,0,0,0,0,0,NULL,&atx[9],NULL,0,&atx[28]} ,
  {0, "to" ,128,1,0,0,0,0,0,0,NULL,&atx[9],NULL,0,NULL} ,
  {0, "columns" ,128,3,0,0,0,0,0,0,NULL,&atx[30],NULL,0,NULL} ,
  {407, "Align-columns" ,1,0,0,0,0,0,0,0,NULL,&atx[24],&atx[31],0,NULL} ,
  {0, "from" ,128,0,0,0,0,0,0,0,NULL,&atx[9],NULL,0,&atx[32]} ,
  {0, "to" ,128,1,0,0,0,0,0,0,NULL,&atx[9],NULL,0,NULL} ,
  {312, "SEQUENCE OF" ,0,16,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {401, "Date" ,1,0,0,0,0,1,0,0,NULL,&atx[12],&atx[35],0,&atx[42]} ,
  {0, "str" ,128,0,0,0,0,0,0,0,NULL,&atx[11],NULL,0,&atx[36]} ,
  {0, "std" ,128,1,0,0,0,0,0,0,NULL,&atx[37],NULL,0,NULL} ,
  {407, "Date-std" ,1,0,0,0,0,0,0,0,NULL,&atx[24],&atx[38],0,&atx[48]} ,
  {0, "year" ,128,0,0,0,0,0,0,0,NULL,&atx[9],NULL,0,&atx[39]} ,
  {0, "month" ,128,1,0,1,0,0,0,0,NULL,&atx[9],NULL,0,&atx[40]} ,
  {0, "day" ,128,2,0,1,0,0,0,0,NULL,&atx[9],NULL,0,&atx[41]} ,
  {0, "season" ,128,3,0,1,0,0,0,0,NULL,&atx[11],NULL,0,NULL} ,
  {402, "Person-id" ,1,0,0,0,0,1,0,0,NULL,&atx[12],&atx[43],0,&atx[7]} ,
  {0, "dbtag" ,128,0,0,0,0,0,0,0,NULL,&atx[44],NULL,0,&atx[47]} ,
  {404, "Dbtag" ,1,0,0,0,0,1,0,0,NULL,&atx[24],&atx[45],0,&atx[58]} ,
  {0, "db" ,128,0,0,0,0,0,0,0,NULL,&atx[11],NULL,0,&atx[46]} ,
  {0, "tag" ,128,1,0,0,0,0,0,0,NULL,&atx[7],NULL,0,NULL} ,
  {0, "name" ,128,1,0,0,0,0,0,0,NULL,&atx[48],NULL,0,&atx[56]} ,
  {408, "Name-std" ,1,0,0,0,0,0,0,0,NULL,&atx[24],&atx[49],0,&atx[74]} ,
  {0, "last" ,128,0,0,0,0,0,0,0,NULL,&atx[11],NULL,0,&atx[50]} ,
  {0, "first" ,128,1,0,1,0,0,0,0,NULL,&atx[11],NULL,0,&atx[51]} ,
  {0, "middle" ,128,2,0,1,0,0,0,0,NULL,&atx[11],NULL,0,&atx[52]} ,
  {0, "full" ,128,3,0,1,0,0,0,0,NULL,&atx[11],NULL,0,&atx[53]} ,
  {0, "initials" ,128,4,0,1,0,0,0,0,NULL,&atx[11],NULL,0,&atx[54]} ,
  {0, "suffix" ,128,5,0,1,0,0,0,0,NULL,&atx[11],NULL,0,&atx[55]} ,
  {0, "title" ,128,6,0,1,0,0,0,0,NULL,&atx[11],NULL,0,NULL} ,
  {0, "ml" ,128,2,0,0,0,0,0,0,NULL,&atx[11],NULL,0,&atx[57]} ,
  {0, "str" ,128,3,0,0,0,0,0,0,NULL,&atx[11],NULL,0,NULL} ,
  {405, "Int-fuzz" ,1,0,0,0,0,1,0,0,NULL,&atx[12],&atx[59],0,&atx[69]} ,
  {0, "p-m" ,128,0,0,0,0,0,0,0,NULL,&atx[9],NULL,0,&atx[60]} ,
  {0, "range" ,128,1,0,0,0,0,0,0,NULL,&atx[24],&atx[61],0,&atx[63]} ,
  {0, "max" ,128,0,0,0,0,0,0,0,NULL,&atx[9],NULL,0,&atx[62]} ,
  {0, "min" ,128,1,0,0,0,0,0,0,NULL,&atx[9],NULL,0,NULL} ,
  {0, "pct" ,128,2,0,0,0,0,0,0,NULL,&atx[9],NULL,0,&atx[64]} ,
  {0, "lim" ,128,3,0,0,0,0,0,0,NULL,&atx[65],&avnx[0],0,&atx[66]} ,
  {310, "ENUMERATED" ,0,10,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "alt" ,128,4,0,0,0,0,0,0,NULL,&atx[68],&atx[67],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[9],NULL,0,NULL} ,
  {314, "SET OF" ,0,17,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {406, "User-object" ,1,0,0,0,0,1,0,0,NULL,&atx[24],&atx[70],0,&atx[37]} ,
  {0, "class" ,128,0,0,1,0,0,0,0,NULL,&atx[11],NULL,0,&atx[71]} ,
  {0, "type" ,128,1,0,0,0,0,0,0,NULL,&atx[7],NULL,0,&atx[72]} ,
  {0, "data" ,128,2,0,0,0,0,0,0,NULL,&atx[33],&atx[73],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[74],NULL,0,NULL} ,
  {409, "User-field" ,1,0,0,0,0,0,0,0,NULL,&atx[24],&atx[75],0,NULL} ,
  {0, "label" ,128,0,0,0,0,0,0,0,NULL,&atx[7],NULL,0,&atx[76]} ,
  {0, "num" ,128,1,0,1,0,0,0,0,NULL,&atx[9],NULL,0,&atx[77]} ,
  {0, "data" ,128,2,0,0,0,0,0,0,NULL,&atx[12],&atx[78],0,NULL} ,
  {0, "str" ,128,0,0,0,0,0,0,0,NULL,&atx[11],NULL,0,&atx[79]} ,
  {0, "int" ,128,1,0,0,0,0,0,0,NULL,&atx[9],NULL,0,&atx[80]} ,
  {0, "real" ,128,2,0,0,0,0,0,0,NULL,&atx[81],NULL,0,&atx[82]} ,
  {309, "REAL" ,0,9,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "bool" ,128,3,0,0,0,0,0,0,NULL,&atx[83],NULL,0,&atx[84]} ,
  {301, "BOOLEAN" ,0,1,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "os" ,128,4,0,0,0,0,0,0,NULL,&atx[85],NULL,0,&atx[86]} ,
  {304, "OCTET STRING" ,0,4,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "object" ,128,5,0,0,0,0,0,0,NULL,&atx[69],NULL,0,&atx[87]} ,
  {0, "strs" ,128,6,0,0,0,0,0,0,NULL,&atx[33],&atx[88],0,&atx[89]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[11],NULL,0,NULL} ,
  {0, "ints" ,128,7,0,0,0,0,0,0,NULL,&atx[33],&atx[90],0,&atx[91]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[9],NULL,0,NULL} ,
  {0, "reals" ,128,8,0,0,0,0,0,0,NULL,&atx[33],&atx[92],0,&atx[93]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[81],NULL,0,NULL} ,
  {0, "oss" ,128,9,0,0,0,0,0,0,NULL,&atx[33],&atx[94],0,&atx[95]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[85],NULL,0,NULL} ,
  {0, "fields" ,128,10,0,0,0,0,0,0,NULL,&atx[33],&atx[96],0,&atx[97]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[74],NULL,0,NULL} ,
  {0, "objects" ,128,11,0,0,0,0,0,0,NULL,&atx[33],&atx[98],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[69],NULL,0,NULL} };

static AsnModule ampx[2] = {
  { "NCBI-Alignloc" , "alignloc.h11",&atx[0],NULL,&ampx[1],0,0} ,
  { "NCBI-General" , "60",&atx[34],NULL,NULL,0,0} };

static AsnValxNodePtr avn = avnx;
static AsnTypePtr at = atx;
static AsnModulePtr amp = ampx;



/**************************************************
*
*    Defines for Module NCBI-Alignloc
*
**************************************************/

#define ALIGN_LOC_SET &at[0]
#define ALIGN_LOC_SET_E &at[1]

#define ALIGN_LOC &at[2]
#define ALIGN_LOC_id &at[3]
#define ALIGN_LOC_loc &at[15]
#define ALIGN_LOC_loc_whole &at[16]
#define ALIGN_LOC_loc_box &at[18]
#define ALIGN_LOC_loc_rows &at[25]
#define ALIGN_LOC_loc_columns &at[29]

#define ALIGN_ID &at[4]
#define ALIGN_ID_id &at[5]
#define ALIGN_ID_itemid &at[13]
#define ALIGN_ID_gi &at[14]

#define ALIGN_BOX &at[19]
#define ALIGN_BOX_row_from &at[20]
#define ALIGN_BOX_row_to &at[21]
#define ALIGN_BOX_column_from &at[22]
#define ALIGN_BOX_column_to &at[23]

#define ALIGN_ROWS &at[26]
#define ALIGN_ROWS_from &at[27]
#define ALIGN_ROWS_to &at[28]

#define ALIGN_COLUMNS &at[30]
#define ALIGN_COLUMNS_from &at[31]
#define ALIGN_COLUMNS_to &at[32]


/**************************************************
*
*    Defines for Module NCBI-General
*
**************************************************/

#define DATE &at[34]
#define DATE_str &at[35]
#define DATE_std &at[36]

#define PERSON_ID &at[42]
#define PERSON_ID_dbtag &at[43]
#define PERSON_ID_name &at[47]
#define PERSON_ID_ml &at[56]
#define PERSON_ID_str &at[57]

#define OBJECT_ID &at[7]
#define OBJECT_ID_id &at[8]
#define OBJECT_ID_str &at[10]

#define DBTAG &at[44]
#define DBTAG_db &at[45]
#define DBTAG_tag &at[46]

#define INT_FUZZ &at[58]
#define INT_FUZZ_p_m &at[59]
#define INT_FUZZ_range &at[60]
#define INT_FUZZ_range_max &at[61]
#define INT_FUZZ_range_min &at[62]
#define INT_FUZZ_pct &at[63]
#define INT_FUZZ_lim &at[64]
#define INT_FUZZ_alt &at[66]
#define INT_FUZZ_alt_E &at[67]

#define USER_OBJECT &at[69]
#define USER_OBJECT_class &at[70]
#define USER_OBJECT_type &at[71]
#define USER_OBJECT_data &at[72]
#define USER_OBJECT_data_E &at[73]

#define DATE_STD &at[37]
#define DATE_STD_year &at[38]
#define DATE_STD_month &at[39]
#define DATE_STD_day &at[40]
#define DATE_STD_season &at[41]

#define NAME_STD &at[48]
#define NAME_STD_last &at[49]
#define NAME_STD_first &at[50]
#define NAME_STD_middle &at[51]
#define NAME_STD_full &at[52]
#define NAME_STD_initials &at[53]
#define NAME_STD_suffix &at[54]
#define NAME_STD_title &at[55]

#define USER_FIELD &at[74]
#define USER_FIELD_label &at[75]
#define USER_FIELD_num &at[76]
#define USER_FIELD_data &at[77]
#define USER_FIELD_data_str &at[78]
#define USER_FIELD_data_int &at[79]
#define USER_FIELD_data_real &at[80]
#define USER_FIELD_data_bool &at[82]
#define USER_FIELD_data_os &at[84]
#define USER_FIELD_data_object &at[86]
#define USER_FIELD_data_strs &at[87]
#define USER_FIELD_data_strs_E &at[88]
#define USER_FIELD_data_ints &at[89]
#define USER_FIELD_data_ints_E &at[90]
#define USER_FIELD_data_reals &at[91]
#define USER_FIELD_data_reals_E &at[92]
#define USER_FIELD_data_oss &at[93]
#define USER_FIELD_data_oss_E &at[94]
#define USER_FIELD_data_fields &at[95]
#define USER_FIELD_data_fields_E &at[96]
#define USER_FIELD_data_objects &at[97]
#define USER_FIELD_data_objects_E &at[98]
