/***********************************************************************
*
**
*        Automatic header module from ASNTOOL
*
************************************************************************/

#ifndef _ASNTOOL_
#include <asn.h>
#endif

static char * asnfilename = "cdrom.h60";
static AsnValxNode avnx[18] = {
    {3,NULL,0,0.0,NULL } ,
    {3,NULL,0,0.0,NULL } ,
    {2,NULL,0,0.0,NULL } ,
    {2,NULL,0,0.0,NULL } ,
    {2,NULL,0,0.0,NULL } ,
    {3,NULL,0,0.0,NULL } ,
    {20,"sort" ,0,0.0,&avnx[7] } ,
    {20,"unsort" ,1,0.0,&avnx[8] } ,
    {20,"hier" ,2,0.0,&avnx[9] } ,
    {20,"text" ,3,0.0,&avnx[10] } ,
    {20,"other" ,255,0.0,NULL } ,
    {2,NULL,0,0.0,NULL } ,
    {2,NULL,0,0.0,NULL } ,
    {2,NULL,0,0.0,NULL } ,
    {2,NULL,0,0.0,NULL } ,
    {2,NULL,0,0.0,NULL } ,
    {2,NULL,0,0.0,NULL } ,
    {2,NULL,0,0.0,NULL } };

static AsnType atx[107] = {
  {401, "Cdrom-inf" ,1,0,0,0,0,1,0,0,NULL,&atx[37],&atx[1],0,&atx[88]} ,
  {0, "volume-label" ,128,0,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[3]} ,
  {323, "VisibleString" ,0,26,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "version" ,128,1,0,0,0,0,0,0,NULL,&atx[4],NULL,0,&atx[5]} ,
  {302, "INTEGER" ,0,2,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "issue" ,128,2,0,0,0,0,0,0,NULL,&atx[4],NULL,0,&atx[6]} ,
  {0, "format" ,128,3,0,0,0,0,0,0,NULL,&atx[4],NULL,0,&atx[7]} ,
  {0, "descr" ,128,4,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[8]} ,
  {0, "no-compression" ,128,5,0,1,0,0,0,0,NULL,&atx[9],NULL,0,&atx[10]} ,
  {301, "BOOLEAN" ,0,1,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "huff-count" ,128,6,0,1,0,0,0,0,NULL,&atx[4],NULL,0,&atx[11]} ,
  {0, "huff-left" ,128,7,0,1,0,0,0,0,NULL,&atx[13],&atx[12],0,&atx[14]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[4],NULL,0,NULL} ,
  {312, "SEQUENCE OF" ,0,16,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "huff-right" ,128,8,0,1,0,0,0,0,NULL,&atx[13],&atx[15],0,&atx[16]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[4],NULL,0,NULL} ,
  {0, "type-count" ,128,9,0,0,0,0,0,0,NULL,&atx[4],NULL,0,&atx[17]} ,
  {0, "type-names" ,128,10,0,1,0,0,0,0,NULL,&atx[13],&atx[18],0,&atx[19]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[2],NULL,0,NULL} ,
  {0, "type-bucket-size" ,128,11,0,1,0,0,0,0,NULL,&atx[4],NULL,0,&atx[20]} ,
  {0, "field-count" ,128,12,0,0,0,0,0,0,NULL,&atx[4],NULL,0,&atx[21]} ,
  {0, "field-names" ,128,13,0,1,0,0,0,0,NULL,&atx[13],&atx[22],0,&atx[23]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[2],NULL,0,NULL} ,
  {0, "field-bucket-size" ,128,14,0,1,0,0,0,0,NULL,&atx[4],NULL,0,&atx[24]} ,
  {0, "types" ,128,15,0,1,0,0,0,0,NULL,&atx[13],&atx[25],0,&atx[38]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[26],NULL,0,NULL} ,
  {403, "Typedata" ,1,0,0,0,0,0,0,0,NULL,&atx[37],&atx[27],0,&atx[39]} ,
  {0, "num" ,128,0,0,0,0,0,0,0,NULL,&atx[4],NULL,0,&atx[28]} ,
  {0, "num-uids" ,128,1,0,0,0,0,0,0,NULL,&atx[4],NULL,0,&atx[29]} ,
  {0, "minuid" ,128,2,0,0,0,0,0,0,NULL,&atx[4],NULL,0,&atx[30]} ,
  {0, "maxuid" ,128,3,0,0,0,0,0,0,NULL,&atx[4],NULL,0,&atx[31]} ,
  {0, "num-bucket" ,128,4,0,0,0,0,0,0,NULL,&atx[4],NULL,0,&atx[32]} ,
  {0, "fields" ,128,5,0,0,0,0,0,0,NULL,&atx[13],&atx[33],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[34],NULL,0,NULL} ,
  {409, "Fielddata" ,1,0,0,0,0,0,0,0,NULL,&atx[37],&atx[35],0,&atx[64]} ,
  {0, "num-terms" ,128,0,0,0,0,0,0,0,NULL,&atx[4],NULL,0,&atx[36]} ,
  {0, "num-bucket" ,128,1,0,0,0,0,0,0,NULL,&atx[4],NULL,0,NULL} ,
  {311, "SEQUENCE" ,0,16,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "release-date" ,128,16,0,1,0,0,0,0,NULL,&atx[39],NULL,0,&atx[43]} ,
  {404, "Cd-date" ,1,0,0,0,0,0,0,0,NULL,&atx[37],&atx[40],0,&atx[46]} ,
  {0, "year" ,128,0,0,0,0,0,0,0,NULL,&atx[4],NULL,0,&atx[41]} ,
  {0, "month" ,128,1,0,0,1,0,0,0,&avnx[0],&atx[4],NULL,0,&atx[42]} ,
  {0, "day" ,128,2,0,0,1,0,0,0,&avnx[1],&atx[4],NULL,0,NULL} ,
  {0, "close-date" ,128,17,0,1,0,0,0,0,NULL,&atx[39],NULL,0,&atx[44]} ,
  {0, "type-info" ,128,18,0,1,0,0,0,0,NULL,&atx[13],&atx[45],0,&atx[52]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[46],NULL,0,NULL} ,
  {405, "Type-info" ,1,0,0,0,0,0,0,0,NULL,&atx[37],&atx[47],0,&atx[54]} ,
  {0, "id" ,128,0,0,0,0,0,0,0,NULL,&atx[4],NULL,0,&atx[48]} ,
  {0, "tag" ,128,1,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[49]} ,
  {0, "name" ,128,2,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[50]} ,
  {0, "descr" ,128,3,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[51]} ,
  {0, "asntype" ,128,4,0,0,0,0,0,0,NULL,&atx[2],NULL,0,NULL} ,
  {0, "field-info" ,128,19,0,1,0,0,0,0,NULL,&atx[13],&atx[53],0,&atx[66]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[54],NULL,0,NULL} ,
  {406, "Field-info" ,1,0,0,0,0,0,0,0,NULL,&atx[37],&atx[55],0,&atx[69]} ,
  {0, "id" ,128,0,0,0,0,0,0,0,NULL,&atx[4],NULL,0,&atx[56]} ,
  {0, "tag" ,128,1,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[57]} ,
  {0, "name" ,128,2,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[58]} ,
  {0, "descr" ,128,3,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[59]} ,
  {0, "single-token" ,128,4,0,0,1,0,0,0,&avnx[2],&atx[9],NULL,0,&atx[60]} ,
  {0, "has-special" ,128,5,0,0,1,0,0,0,&avnx[3],&atx[9],NULL,0,&atx[61]} ,
  {0, "hier-avail" ,128,6,0,0,1,0,0,0,&avnx[4],&atx[9],NULL,0,&atx[62]} ,
  {0, "hier-id" ,128,7,0,1,0,0,0,0,NULL,&atx[4],NULL,0,&atx[63]} ,
  {0, "post-type" ,128,8,0,0,1,0,0,0,&avnx[5],&atx[64],NULL,0,NULL} ,
  {410, "Post-type" ,1,0,0,0,0,0,0,0,NULL,&atx[65],&avnx[6],0,&atx[105]} ,
  {310, "ENUMERATED" ,0,10,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "div-count" ,128,20,0,1,0,0,0,0,NULL,&atx[4],NULL,0,&atx[67]} ,
  {0, "div-info" ,128,21,0,1,0,0,0,0,NULL,&atx[13],&atx[68],0,&atx[76]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[69],NULL,0,NULL} ,
  {407, "Div-info" ,1,0,0,0,0,0,0,0,NULL,&atx[37],&atx[70],0,&atx[79]} ,
  {0, "tag" ,128,0,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[71]} ,
  {0, "descr" ,128,1,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[72]} ,
  {0, "reldate" ,128,2,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[73]} ,
  {0, "date" ,128,3,0,1,0,0,0,0,NULL,&atx[39],NULL,0,&atx[74]} ,
  {0, "docs" ,128,4,0,1,0,0,0,0,NULL,&atx[13],&atx[75],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[4],NULL,0,NULL} ,
  {0, "link-count" ,128,22,0,1,0,0,0,0,NULL,&atx[4],NULL,0,&atx[77]} ,
  {0, "link-info" ,128,23,0,1,0,0,0,0,NULL,&atx[13],&atx[78],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[79],NULL,0,NULL} ,
  {408, "Link-info" ,1,0,0,0,0,0,0,0,NULL,&atx[37],&atx[80],0,&atx[34]} ,
  {0, "id" ,128,0,0,0,0,0,0,0,NULL,&atx[4],NULL,0,&atx[81]} ,
  {0, "tag" ,128,1,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[82]} ,
  {0, "name" ,128,2,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[83]} ,
  {0, "descr" ,128,3,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[84]} ,
  {0, "dbfrom" ,128,4,0,0,0,0,0,0,NULL,&atx[4],NULL,0,&atx[85]} ,
  {0, "dbto" ,128,5,0,0,0,0,0,0,NULL,&atx[4],NULL,0,&atx[86]} ,
  {0, "datasize" ,128,6,0,0,0,0,0,0,NULL,&atx[4],NULL,0,&atx[87]} ,
  {0, "reciprocal" ,128,7,0,0,0,0,0,0,NULL,&atx[4],NULL,0,NULL} ,
  {402, "Docsum" ,1,0,0,0,0,1,0,0,NULL,&atx[37],&atx[89],0,&atx[26]} ,
  {0, "no-abstract" ,128,0,0,0,1,0,0,0,&avnx[11],&atx[9],NULL,0,&atx[90]} ,
  {0, "translated-title" ,128,1,0,0,1,0,0,0,&avnx[12],&atx[9],NULL,0,&atx[91]} ,
  {0, "no-authors" ,128,2,0,0,1,0,0,0,&avnx[13],&atx[9],NULL,0,&atx[92]} ,
  {0, "caption" ,128,3,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[93]} ,
  {0, "title" ,128,4,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[94]} ,
  {0, "extra" ,128,5,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[95]} ,
  {0, "non-document" ,128,6,0,0,1,0,0,0,&avnx[14],&atx[9],NULL,0,&atx[96]} ,
  {0, "is-segmented" ,128,7,0,0,1,0,0,0,&avnx[15],&atx[9],NULL,0,&atx[97]} ,
  {0, "is-partial" ,128,8,0,0,1,0,0,0,&avnx[16],&atx[9],NULL,0,&atx[98]} ,
  {0, "create" ,128,9,0,1,0,0,0,0,NULL,&atx[39],NULL,0,&atx[99]} ,
  {0, "modify" ,128,10,0,1,0,0,0,0,NULL,&atx[39],NULL,0,&atx[100]} ,
  {0, "link-count" ,128,11,0,1,0,0,0,0,NULL,&atx[13],&atx[101],0,&atx[102]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[4],NULL,0,NULL} ,
  {0, "uid" ,128,12,0,1,0,0,0,0,NULL,&atx[4],NULL,0,&atx[103]} ,
  {0, "secondaryUid" ,128,13,0,1,0,0,0,0,NULL,&atx[4],NULL,0,&atx[104]} ,
  {0, "not-yet-neighbored" ,128,14,0,0,1,0,0,0,&avnx[17],&atx[9],NULL,0,NULL} ,
  {411, "Docsum-set" ,1,0,0,0,0,0,0,0,NULL,&atx[13],&atx[106],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[88],NULL,0,NULL} };

static AsnModule ampx[1] = {
  { "NCBI-CdRom" , "cdrom.h60",&atx[0],NULL,NULL,0,0} };

static AsnValxNodePtr avn = avnx;
static AsnTypePtr at = atx;
static AsnModulePtr amp = ampx;



/**************************************************
*
*    Defines for Module NCBI-CdRom
*
**************************************************/

#define CDROM_INF &at[0]
#define CDROM_INF_volume_label &at[1]
#define CDROM_INF_version &at[3]
#define CDROM_INF_issue &at[5]
#define CDROM_INF_format &at[6]
#define CDROM_INF_descr &at[7]
#define CDROM_INF_no_compression &at[8]
#define CDROM_INF_huff_count &at[10]
#define CDROM_INF_huff_left &at[11]
#define CDROM_INF_huff_left_E &at[12]
#define CDROM_INF_huff_right &at[14]
#define CDROM_INF_huff_right_E &at[15]
#define CDROM_INF_type_count &at[16]
#define CDROM_INF_type_names &at[17]
#define CDROM_INF_type_names_E &at[18]
#define CDROM_INF_type_bucket_size &at[19]
#define CDROM_INF_field_count &at[20]
#define CDROM_INF_field_names &at[21]
#define CDROM_INF_field_names_E &at[22]
#define CDROM_INF_field_bucket_size &at[23]
#define CDROM_INF_types &at[24]
#define CDROM_INF_types_E &at[25]
#define CDROM_INF_release_date &at[38]
#define CDROM_INF_close_date &at[43]
#define CDROM_INF_type_info &at[44]
#define CDROM_INF_type_info_E &at[45]
#define CDROM_INF_field_info &at[52]
#define CDROM_INF_field_info_E &at[53]
#define CDROM_INF_div_count &at[66]
#define CDROM_INF_div_info &at[67]
#define CDROM_INF_div_info_E &at[68]
#define CDROM_INF_link_count &at[76]
#define CDROM_INF_link_info &at[77]
#define CDROM_INF_link_info_E &at[78]

#define DOCSUM &at[88]
#define DOCSUM_no_abstract &at[89]
#define DOCSUM_translated_title &at[90]
#define DOCSUM_no_authors &at[91]
#define DOCSUM_caption &at[92]
#define DOCSUM_title &at[93]
#define DOCSUM_extra &at[94]
#define DOCSUM_non_document &at[95]
#define DOCSUM_is_segmented &at[96]
#define DOCSUM_is_partial &at[97]
#define DOCSUM_create &at[98]
#define DOCSUM_modify &at[99]
#define DOCSUM_link_count &at[100]
#define DOCSUM_link_count_E &at[101]
#define DOCSUM_uid &at[102]
#define DOCSUM_secondaryUid &at[103]
#define DOCSUM_not_yet_neighbored &at[104]

#define TYPEDATA &at[26]
#define TYPEDATA_num &at[27]
#define TYPEDATA_num_uids &at[28]
#define TYPEDATA_minuid &at[29]
#define TYPEDATA_maxuid &at[30]
#define TYPEDATA_num_bucket &at[31]
#define TYPEDATA_fields &at[32]
#define TYPEDATA_fields_E &at[33]

#define CD_DATE &at[39]
#define CD_DATE_year &at[40]
#define CD_DATE_month &at[41]
#define CD_DATE_day &at[42]

#define TYPE_INFO &at[46]
#define TYPE_INFO_id &at[47]
#define TYPE_INFO_tag &at[48]
#define TYPE_INFO_name &at[49]
#define TYPE_INFO_descr &at[50]
#define TYPE_INFO_asntype &at[51]

#define FIELD_INFO &at[54]
#define FIELD_INFO_id &at[55]
#define FIELD_INFO_tag &at[56]
#define FIELD_INFO_name &at[57]
#define FIELD_INFO_descr &at[58]
#define FIELD_INFO_single_token &at[59]
#define FIELD_INFO_has_special &at[60]
#define FIELD_INFO_hier_avail &at[61]
#define FIELD_INFO_hier_id &at[62]
#define FIELD_INFO_post_type &at[63]

#define DIV_INFO &at[69]
#define DIV_INFO_tag &at[70]
#define DIV_INFO_descr &at[71]
#define DIV_INFO_reldate &at[72]
#define DIV_INFO_date &at[73]
#define DIV_INFO_docs &at[74]
#define DIV_INFO_docs_E &at[75]

#define LINK_INFO &at[79]
#define LINK_INFO_id &at[80]
#define LINK_INFO_tag &at[81]
#define LINK_INFO_name &at[82]
#define LINK_INFO_descr &at[83]
#define LINK_INFO_dbfrom &at[84]
#define LINK_INFO_dbto &at[85]
#define LINK_INFO_datasize &at[86]
#define LINK_INFO_reciprocal &at[87]

#define FIELDDATA &at[34]
#define FIELDDATA_num_terms &at[35]
#define FIELDDATA_num_bucket &at[36]

#define POST_TYPE &at[64]

#define DOCSUM_SET &at[105]
#define DOCSUM_SET_E &at[106]
