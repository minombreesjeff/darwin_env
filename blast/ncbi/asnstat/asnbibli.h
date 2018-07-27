/***********************************************************************
*
**
*        Automatic header module from ASNTOOL
*
************************************************************************/

#ifndef _ASNTOOL_
#include <asn.h>
#endif

static char * asnfilename = "asnbibli.h62";
static AsnValxNode avnx[35] = {
    {20,"primary" ,1,0.0,&avnx[1] } ,
    {20,"secondary" ,2,0.0,NULL } ,
    {20,"compiler" ,1,0.0,&avnx[3] } ,
    {20,"editor" ,2,0.0,&avnx[4] } ,
    {20,"patent-assignee" ,3,0.0,&avnx[5] } ,
    {20,"translator" ,4,0.0,NULL } ,
    {1,"ENG" ,0,0.0,NULL } ,
    {20,"submitted" ,1,0.0,&avnx[8] } ,
    {20,"in-press" ,2,0.0,&avnx[9] } ,
    {20,"other" ,255,0.0,NULL } ,
    {20,"retracted" ,1,0.0,&avnx[11] } ,
    {20,"notice" ,2,0.0,&avnx[12] } ,
    {20,"in-error" ,3,0.0,&avnx[13] } ,
    {20,"erratum" ,4,0.0,NULL } ,
    {20,"received" ,1,0.0,&avnx[15] } ,
    {20,"accepted" ,2,0.0,&avnx[16] } ,
    {20,"epublish" ,3,0.0,&avnx[17] } ,
    {20,"ppublish" ,4,0.0,&avnx[18] } ,
    {20,"revised" ,5,0.0,&avnx[19] } ,
    {20,"pmc" ,6,0.0,&avnx[20] } ,
    {20,"pmcr" ,7,0.0,&avnx[21] } ,
    {20,"pubmed" ,8,0.0,&avnx[22] } ,
    {20,"pubmedr" ,9,0.0,&avnx[23] } ,
    {20,"aheadofprint" ,10,0.0,&avnx[24] } ,
    {20,"premedline" ,11,0.0,&avnx[25] } ,
    {20,"medline" ,12,0.0,&avnx[26] } ,
    {20,"other" ,255,0.0,NULL } ,
    {20,"manuscript" ,1,0.0,&avnx[28] } ,
    {20,"letter" ,2,0.0,&avnx[29] } ,
    {20,"thesis" ,3,0.0,NULL } ,
    {20,"paper" ,1,0.0,&avnx[31] } ,
    {20,"tape" ,2,0.0,&avnx[32] } ,
    {20,"floppy" ,3,0.0,&avnx[33] } ,
    {20,"email" ,4,0.0,&avnx[34] } ,
    {20,"other" ,255,0.0,NULL } };

static AsnType atx[164] = {
  {401, "Cit-art" ,1,0,0,0,0,1,0,0,NULL,&atx[42],&atx[1],0,&atx[53]} ,
  {0, "title" ,128,0,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[17]} ,
  {410, "Title" ,1,0,0,0,0,1,0,0,NULL,&atx[16],&atx[3],0,&atx[22]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[15],&atx[4],0,NULL} ,
  {0, "name" ,128,0,0,0,0,0,0,0,NULL,&atx[5],NULL,0,&atx[6]} ,
  {323, "VisibleString" ,0,26,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "tsub" ,128,1,0,0,0,0,0,0,NULL,&atx[5],NULL,0,&atx[7]} ,
  {0, "trans" ,128,2,0,0,0,0,0,0,NULL,&atx[5],NULL,0,&atx[8]} ,
  {0, "jta" ,128,3,0,0,0,0,0,0,NULL,&atx[5],NULL,0,&atx[9]} ,
  {0, "iso-jta" ,128,4,0,0,0,0,0,0,NULL,&atx[5],NULL,0,&atx[10]} ,
  {0, "ml-jta" ,128,5,0,0,0,0,0,0,NULL,&atx[5],NULL,0,&atx[11]} ,
  {0, "coden" ,128,6,0,0,0,0,0,0,NULL,&atx[5],NULL,0,&atx[12]} ,
  {0, "issn" ,128,7,0,0,0,0,0,0,NULL,&atx[5],NULL,0,&atx[13]} ,
  {0, "abr" ,128,8,0,0,0,0,0,0,NULL,&atx[5],NULL,0,&atx[14]} ,
  {0, "isbn" ,128,9,0,0,0,0,0,0,NULL,&atx[5],NULL,0,NULL} ,
  {315, "CHOICE" ,0,-1,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {314, "SET OF" ,0,17,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "authors" ,128,1,0,1,0,0,0,0,NULL,&atx[18],NULL,0,&atx[51]} ,
  {427, "Auth-list" ,1,0,0,0,0,0,0,0,NULL,&atx[42],&atx[19],0,&atx[56]} ,
  {0, "names" ,128,0,0,0,0,0,0,0,NULL,&atx[15],&atx[20],0,&atx[50]} ,
  {0, "std" ,128,0,0,0,0,0,0,0,NULL,&atx[45],&atx[21],0,&atx[46]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[22],NULL,0,NULL} ,
  {411, "Author" ,1,0,0,0,0,1,0,0,NULL,&atx[42],&atx[23],0,&atx[101]} ,
  {0, "name" ,128,0,0,0,0,0,0,0,NULL,&atx[24],NULL,0,&atx[25]} ,
  {413, "Person-id" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[58]} ,
  {0, "level" ,128,1,0,1,0,0,0,0,NULL,&atx[26],&avnx[0],0,&atx[27]} ,
  {310, "ENUMERATED" ,0,10,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "role" ,128,2,0,1,0,0,0,0,NULL,&atx[26],&avnx[2],0,&atx[28]} ,
  {0, "affil" ,128,3,0,1,0,0,0,0,NULL,&atx[29],NULL,0,&atx[43]} ,
  {431, "Affil" ,1,0,0,0,0,0,0,0,NULL,&atx[15],&atx[30],0,&atx[70]} ,
  {0, "str" ,128,0,0,0,0,0,0,0,NULL,&atx[5],NULL,0,&atx[31]} ,
  {0, "std" ,128,1,0,0,0,0,0,0,NULL,&atx[42],&atx[32],0,NULL} ,
  {0, "affil" ,128,0,0,1,0,0,0,0,NULL,&atx[5],NULL,0,&atx[33]} ,
  {0, "div" ,128,1,0,1,0,0,0,0,NULL,&atx[5],NULL,0,&atx[34]} ,
  {0, "city" ,128,2,0,1,0,0,0,0,NULL,&atx[5],NULL,0,&atx[35]} ,
  {0, "sub" ,128,3,0,1,0,0,0,0,NULL,&atx[5],NULL,0,&atx[36]} ,
  {0, "country" ,128,4,0,1,0,0,0,0,NULL,&atx[5],NULL,0,&atx[37]} ,
  {0, "street" ,128,5,0,1,0,0,0,0,NULL,&atx[5],NULL,0,&atx[38]} ,
  {0, "email" ,128,6,0,1,0,0,0,0,NULL,&atx[5],NULL,0,&atx[39]} ,
  {0, "fax" ,128,7,0,1,0,0,0,0,NULL,&atx[5],NULL,0,&atx[40]} ,
  {0, "phone" ,128,8,0,1,0,0,0,0,NULL,&atx[5],NULL,0,&atx[41]} ,
  {0, "postal-code" ,128,9,0,1,0,0,0,0,NULL,&atx[5],NULL,0,NULL} ,
  {311, "SEQUENCE" ,0,16,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "is-corr" ,128,4,0,1,0,0,0,0,NULL,&atx[44],NULL,0,NULL} ,
  {301, "BOOLEAN" ,0,1,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {312, "SEQUENCE OF" ,0,16,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "ml" ,128,1,0,0,0,0,0,0,NULL,&atx[45],&atx[47],0,&atx[48]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[5],NULL,0,NULL} ,
  {0, "str" ,128,2,0,0,0,0,0,0,NULL,&atx[45],&atx[49],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[5],NULL,0,NULL} ,
  {0, "affil" ,128,1,0,1,0,0,0,0,NULL,&atx[29],NULL,0,NULL} ,
  {0, "from" ,128,2,0,0,0,0,0,0,NULL,&atx[15],&atx[52],0,&atx[96]} ,
  {0, "journal" ,128,0,0,0,0,0,0,0,NULL,&atx[53],NULL,0,&atx[82]} ,
  {402, "Cit-jour" ,1,0,0,0,0,1,0,0,NULL,&atx[42],&atx[54],0,&atx[83]} ,
  {0, "title" ,128,0,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[55]} ,
  {0, "imp" ,128,1,0,0,0,0,0,0,NULL,&atx[56],NULL,0,NULL} ,
  {428, "Imprint" ,1,0,0,0,0,0,0,0,NULL,&atx[42],&atx[57],0,&atx[92]} ,
  {0, "date" ,128,0,0,0,0,0,0,0,NULL,&atx[58],NULL,0,&atx[59]} ,
  {414, "Date" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[115]} ,
  {0, "volume" ,128,1,0,1,0,0,0,0,NULL,&atx[5],NULL,0,&atx[60]} ,
  {0, "issue" ,128,2,0,1,0,0,0,0,NULL,&atx[5],NULL,0,&atx[61]} ,
  {0, "pages" ,128,3,0,1,0,0,0,0,NULL,&atx[5],NULL,0,&atx[62]} ,
  {0, "section" ,128,4,0,1,0,0,0,0,NULL,&atx[5],NULL,0,&atx[63]} ,
  {0, "pub" ,128,5,0,1,0,0,0,0,NULL,&atx[29],NULL,0,&atx[64]} ,
  {0, "cprt" ,128,6,0,1,0,0,0,0,NULL,&atx[58],NULL,0,&atx[65]} ,
  {0, "part-sup" ,128,7,0,1,0,0,0,0,NULL,&atx[5],NULL,0,&atx[66]} ,
  {0, "language" ,128,8,0,0,1,0,0,0,&avnx[6],&atx[5],NULL,0,&atx[67]} ,
  {0, "prepub" ,128,9,0,1,0,0,0,0,NULL,&atx[26],&avnx[7],0,&atx[68]} ,
  {0, "part-supi" ,128,10,0,1,0,0,0,0,NULL,&atx[5],NULL,0,&atx[69]} ,
  {0, "retract" ,128,11,0,1,0,0,0,0,NULL,&atx[70],NULL,0,&atx[73]} ,
  {432, "CitRetract" ,1,0,0,0,0,0,0,0,NULL,&atx[42],&atx[71],0,NULL} ,
  {0, "type" ,128,0,0,0,0,0,0,0,NULL,&atx[26],&avnx[10],0,&atx[72]} ,
  {0, "exp" ,128,1,0,1,0,0,0,0,NULL,&atx[5],NULL,0,NULL} ,
  {0, "pubstatus" ,128,12,0,1,0,0,0,0,NULL,&atx[74],NULL,0,&atx[76]} ,
  {424, "PubStatus" ,1,0,0,0,0,0,0,0,NULL,&atx[75],&avnx[14],0,&atx[79]} ,
  {302, "INTEGER" ,0,2,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "history" ,128,13,0,1,0,0,0,0,NULL,&atx[77],NULL,0,NULL} ,
  {426, "PubStatusDateSet" ,1,0,0,0,0,0,0,0,NULL,&atx[16],&atx[78],0,&atx[18]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[79],NULL,0,NULL} ,
  {425, "PubStatusDate" ,1,0,0,0,0,0,0,0,NULL,&atx[42],&atx[80],0,&atx[77]} ,
  {0, "pubstatus" ,128,0,0,0,0,0,0,0,NULL,&atx[74],NULL,0,&atx[81]} ,
  {0, "date" ,128,1,0,0,0,0,0,0,NULL,&atx[58],NULL,0,NULL} ,
  {0, "book" ,128,1,0,0,0,0,0,0,NULL,&atx[83],NULL,0,&atx[88]} ,
  {403, "Cit-book" ,1,0,0,0,0,1,0,0,NULL,&atx[42],&atx[84],0,&atx[116]} ,
  {0, "title" ,128,0,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[85]} ,
  {0, "coll" ,128,1,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[86]} ,
  {0, "authors" ,128,2,0,0,0,0,0,0,NULL,&atx[18],NULL,0,&atx[87]} ,
  {0, "imp" ,128,3,0,0,0,0,0,0,NULL,&atx[56],NULL,0,NULL} ,
  {0, "proc" ,128,2,0,0,0,0,0,0,NULL,&atx[89],NULL,0,NULL} ,
  {408, "Cit-proc" ,1,0,0,0,0,1,0,0,NULL,&atx[42],&atx[90],0,&atx[158]} ,
  {0, "book" ,128,0,0,0,0,0,0,0,NULL,&atx[83],NULL,0,&atx[91]} ,
  {0, "meet" ,128,1,0,0,0,0,0,0,NULL,&atx[92],NULL,0,NULL} ,
  {429, "Meeting" ,1,0,0,0,0,0,0,0,NULL,&atx[42],&atx[93],0,&atx[131]} ,
  {0, "number" ,128,0,0,0,0,0,0,0,NULL,&atx[5],NULL,0,&atx[94]} ,
  {0, "date" ,128,1,0,0,0,0,0,0,NULL,&atx[58],NULL,0,&atx[95]} ,
  {0, "place" ,128,2,0,1,0,0,0,0,NULL,&atx[29],NULL,0,NULL} ,
  {0, "ids" ,128,3,0,1,0,0,0,0,NULL,&atx[97],NULL,0,NULL} ,
  {423, "ArticleIdSet" ,1,0,0,0,0,0,0,0,NULL,&atx[16],&atx[98],0,&atx[74]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[99],NULL,0,NULL} ,
  {416, "ArticleId" ,1,0,0,0,0,0,0,0,NULL,&atx[15],&atx[100],0,&atx[103]} ,
  {0, "pubmed" ,128,0,0,0,0,0,0,0,NULL,&atx[101],NULL,0,&atx[102]} ,
  {412, "PubMedId" ,1,0,0,0,0,1,0,0,NULL,&atx[75],NULL,0,&atx[24]} ,
  {0, "medline" ,128,1,0,0,0,0,0,0,NULL,&atx[103],NULL,0,&atx[104]} ,
  {417, "MedlineUID" ,1,0,0,0,0,0,0,0,NULL,&atx[75],NULL,0,&atx[105]} ,
  {0, "doi" ,128,2,0,0,0,0,0,0,NULL,&atx[105],NULL,0,&atx[106]} ,
  {418, "DOI" ,1,0,0,0,0,0,0,0,NULL,&atx[5],NULL,0,&atx[107]} ,
  {0, "pii" ,128,3,0,0,0,0,0,0,NULL,&atx[107],NULL,0,&atx[108]} ,
  {419, "PII" ,1,0,0,0,0,0,0,0,NULL,&atx[5],NULL,0,&atx[109]} ,
  {0, "pmcid" ,128,4,0,0,0,0,0,0,NULL,&atx[109],NULL,0,&atx[110]} ,
  {420, "PmcID" ,1,0,0,0,0,0,0,0,NULL,&atx[75],NULL,0,&atx[111]} ,
  {0, "pmcpid" ,128,5,0,0,0,0,0,0,NULL,&atx[111],NULL,0,&atx[112]} ,
  {421, "PmcPid" ,1,0,0,0,0,0,0,0,NULL,&atx[5],NULL,0,&atx[113]} ,
  {0, "pmpid" ,128,6,0,0,0,0,0,0,NULL,&atx[113],NULL,0,&atx[114]} ,
  {422, "PmPid" ,1,0,0,0,0,0,0,0,NULL,&atx[5],NULL,0,&atx[97]} ,
  {0, "other" ,128,7,0,0,0,0,0,0,NULL,&atx[115],NULL,0,NULL} ,
  {415, "Dbtag" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[99]} ,
  {404, "Cit-pat" ,1,0,0,0,0,1,0,0,NULL,&atx[42],&atx[117],0,&atx[136]} ,
  {0, "title" ,128,0,0,0,0,0,0,0,NULL,&atx[5],NULL,0,&atx[118]} ,
  {0, "authors" ,128,1,0,0,0,0,0,0,NULL,&atx[18],NULL,0,&atx[119]} ,
  {0, "country" ,128,2,0,0,0,0,0,0,NULL,&atx[5],NULL,0,&atx[120]} ,
  {0, "doc-type" ,128,3,0,0,0,0,0,0,NULL,&atx[5],NULL,0,&atx[121]} ,
  {0, "number" ,128,4,0,1,0,0,0,0,NULL,&atx[5],NULL,0,&atx[122]} ,
  {0, "date-issue" ,128,5,0,1,0,0,0,0,NULL,&atx[58],NULL,0,&atx[123]} ,
  {0, "class" ,128,6,0,1,0,0,0,0,NULL,&atx[45],&atx[124],0,&atx[125]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[5],NULL,0,NULL} ,
  {0, "app-number" ,128,7,0,1,0,0,0,0,NULL,&atx[5],NULL,0,&atx[126]} ,
  {0, "app-date" ,128,8,0,1,0,0,0,0,NULL,&atx[58],NULL,0,&atx[127]} ,
  {0, "applicants" ,128,9,0,1,0,0,0,0,NULL,&atx[18],NULL,0,&atx[128]} ,
  {0, "assignees" ,128,10,0,1,0,0,0,0,NULL,&atx[18],NULL,0,&atx[129]} ,
  {0, "priority" ,128,11,0,1,0,0,0,0,NULL,&atx[45],&atx[130],0,&atx[135]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[131],NULL,0,NULL} ,
  {430, "Patent-priority" ,1,0,0,0,0,0,0,0,NULL,&atx[42],&atx[132],0,&atx[29]} ,
  {0, "country" ,128,0,0,0,0,0,0,0,NULL,&atx[5],NULL,0,&atx[133]} ,
  {0, "number" ,128,1,0,0,0,0,0,0,NULL,&atx[5],NULL,0,&atx[134]} ,
  {0, "date" ,128,2,0,0,0,0,0,0,NULL,&atx[58],NULL,0,NULL} ,
  {0, "abstract" ,128,12,0,1,0,0,0,0,NULL,&atx[5],NULL,0,NULL} ,
  {405, "Cit-let" ,1,0,0,0,0,1,0,0,NULL,&atx[42],&atx[137],0,&atx[140]} ,
  {0, "cit" ,128,0,0,0,0,0,0,0,NULL,&atx[83],NULL,0,&atx[138]} ,
  {0, "man-id" ,128,1,0,1,0,0,0,0,NULL,&atx[5],NULL,0,&atx[139]} ,
  {0, "type" ,128,2,0,1,0,0,0,0,NULL,&atx[26],&avnx[27],0,NULL} ,
  {406, "Id-pat" ,1,0,0,0,0,1,0,0,NULL,&atx[42],&atx[141],0,&atx[146]} ,
  {0, "country" ,128,0,0,0,0,0,0,0,NULL,&atx[5],NULL,0,&atx[142]} ,
  {0, "id" ,128,1,0,0,0,0,0,0,NULL,&atx[15],&atx[143],0,&atx[145]} ,
  {0, "number" ,128,0,0,0,0,0,0,0,NULL,&atx[5],NULL,0,&atx[144]} ,
  {0, "app-number" ,128,1,0,0,0,0,0,0,NULL,&atx[5],NULL,0,NULL} ,
  {0, "doc-type" ,128,2,0,1,0,0,0,0,NULL,&atx[5],NULL,0,NULL} ,
  {407, "Cit-gen" ,1,0,0,0,0,1,0,0,NULL,&atx[42],&atx[147],0,&atx[89]} ,
  {0, "cit" ,128,0,0,1,0,0,0,0,NULL,&atx[5],NULL,0,&atx[148]} ,
  {0, "authors" ,128,1,0,1,0,0,0,0,NULL,&atx[18],NULL,0,&atx[149]} ,
  {0, "muid" ,128,2,0,1,0,0,0,0,NULL,&atx[75],NULL,0,&atx[150]} ,
  {0, "journal" ,128,3,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[151]} ,
  {0, "volume" ,128,4,0,1,0,0,0,0,NULL,&atx[5],NULL,0,&atx[152]} ,
  {0, "issue" ,128,5,0,1,0,0,0,0,NULL,&atx[5],NULL,0,&atx[153]} ,
  {0, "pages" ,128,6,0,1,0,0,0,0,NULL,&atx[5],NULL,0,&atx[154]} ,
  {0, "date" ,128,7,0,1,0,0,0,0,NULL,&atx[58],NULL,0,&atx[155]} ,
  {0, "serial-number" ,128,8,0,1,0,0,0,0,NULL,&atx[75],NULL,0,&atx[156]} ,
  {0, "title" ,128,9,0,1,0,0,0,0,NULL,&atx[5],NULL,0,&atx[157]} ,
  {0, "pmid" ,128,10,0,1,0,0,0,0,NULL,&atx[101],NULL,0,NULL} ,
  {409, "Cit-sub" ,1,0,0,0,0,1,0,0,NULL,&atx[42],&atx[159],0,&atx[2]} ,
  {0, "authors" ,128,0,0,0,0,0,0,0,NULL,&atx[18],NULL,0,&atx[160]} ,
  {0, "imp" ,128,1,0,1,0,0,0,0,NULL,&atx[56],NULL,0,&atx[161]} ,
  {0, "medium" ,128,2,0,1,0,0,0,0,NULL,&atx[26],&avnx[30],0,&atx[162]} ,
  {0, "date" ,128,3,0,1,0,0,0,0,NULL,&atx[58],NULL,0,&atx[163]} ,
  {0, "descr" ,128,4,0,1,0,0,0,0,NULL,&atx[5],NULL,0,NULL} };

static AsnModule ampx[1] = {
  { "NCBI-Biblio" , "asnbibli.h62",&atx[0],NULL,NULL,0,0} };

static AsnValxNodePtr avn = avnx;
static AsnTypePtr at = atx;
static AsnModulePtr amp = ampx;



/**************************************************
*
*    Defines for Module NCBI-Biblio
*
**************************************************/

#define CIT_ART &at[0]
#define CIT_ART_title &at[1]
#define CIT_ART_authors &at[17]
#define CIT_ART_from &at[51]
#define CIT_ART_from_journal &at[52]
#define CIT_ART_from_book &at[82]
#define CIT_ART_from_proc &at[88]
#define CIT_ART_ids &at[96]

#define CIT_JOUR &at[53]
#define CIT_JOUR_title &at[54]
#define CIT_JOUR_imp &at[55]

#define CIT_BOOK &at[83]
#define CIT_BOOK_title &at[84]
#define CIT_BOOK_coll &at[85]
#define CIT_BOOK_authors &at[86]
#define CIT_BOOK_imp &at[87]

#define CIT_PAT &at[116]
#define CIT_PAT_title &at[117]
#define CIT_PAT_authors &at[118]
#define CIT_PAT_country &at[119]
#define CIT_PAT_doc_type &at[120]
#define CIT_PAT_number &at[121]
#define CIT_PAT_date_issue &at[122]
#define CIT_PAT_class &at[123]
#define CIT_PAT_class_E &at[124]
#define CIT_PAT_app_number &at[125]
#define CIT_PAT_app_date &at[126]
#define CIT_PAT_applicants &at[127]
#define CIT_PAT_assignees &at[128]
#define CIT_PAT_priority &at[129]
#define CIT_PAT_priority_E &at[130]
#define CIT_PAT_abstract &at[135]

#define CIT_LET &at[136]
#define CIT_LET_cit &at[137]
#define CIT_LET_man_id &at[138]
#define CIT_LET_type &at[139]

#define ID_PAT &at[140]
#define ID_PAT_country &at[141]
#define ID_PAT_id &at[142]
#define ID_PAT_id_number &at[143]
#define ID_PAT_id_app_number &at[144]
#define ID_PAT_doc_type &at[145]

#define CIT_GEN &at[146]
#define CIT_GEN_cit &at[147]
#define CIT_GEN_authors &at[148]
#define CIT_GEN_muid &at[149]
#define CIT_GEN_journal &at[150]
#define CIT_GEN_volume &at[151]
#define CIT_GEN_issue &at[152]
#define CIT_GEN_pages &at[153]
#define CIT_GEN_date &at[154]
#define CIT_GEN_serial_number &at[155]
#define CIT_GEN_title &at[156]
#define CIT_GEN_pmid &at[157]

#define CIT_PROC &at[89]
#define CIT_PROC_book &at[90]
#define CIT_PROC_meet &at[91]

#define CIT_SUB &at[158]
#define CIT_SUB_authors &at[159]
#define CIT_SUB_imp &at[160]
#define CIT_SUB_medium &at[161]
#define CIT_SUB_date &at[162]
#define CIT_SUB_descr &at[163]

#define TITLE &at[2]
#define TITLE_E &at[3]
#define TITLE_E_name &at[4]
#define TITLE_E_tsub &at[6]
#define TITLE_E_trans &at[7]
#define TITLE_E_jta &at[8]
#define TITLE_E_iso_jta &at[9]
#define TITLE_E_ml_jta &at[10]
#define TITLE_E_coden &at[11]
#define TITLE_E_issn &at[12]
#define TITLE_E_abr &at[13]
#define TITLE_E_isbn &at[14]

#define AUTHOR &at[22]
#define AUTHOR_name &at[23]
#define AUTHOR_level &at[25]
#define AUTHOR_role &at[27]
#define AUTHOR_affil &at[28]
#define AUTHOR_is_corr &at[43]

#define PUBMEDID &at[101]

#define ARTICLEID &at[99]
#define ARTICLEID_pubmed &at[100]
#define ARTICLEID_medline &at[102]
#define ARTICLEID_doi &at[104]
#define ARTICLEID_pii &at[106]
#define ARTICLEID_pmcid &at[108]
#define ARTICLEID_pmcpid &at[110]
#define ARTICLEID_pmpid &at[112]
#define ARTICLEID_other &at[114]

#define MEDLINEUID &at[103]

#define DOI &at[105]

#define PII &at[107]

#define PMCID &at[109]

#define PMCPID &at[111]

#define PMPID &at[113]

#define ARTICLEIDSET &at[97]
#define ARTICLEIDSET_E &at[98]

#define PUBSTATUS &at[74]

#define PUBSTATUSDATE &at[79]
#define PUBSTATUSDATE_pubstatus &at[80]
#define PUBSTATUSDATE_date &at[81]

#define PUBSTATUSDATESET &at[77]
#define PUBSTATUSDATESET_E &at[78]

#define AUTH_LIST &at[18]
#define AUTH_LIST_names &at[19]
#define AUTH_LIST_names_std &at[20]
#define AUTH_LIST_names_std_E &at[21]
#define AUTH_LIST_names_ml &at[46]
#define AUTH_LIST_names_ml_E &at[47]
#define AUTH_LIST_names_str &at[48]
#define AUTH_LIST_names_str_E &at[49]
#define AUTH_LIST_affil &at[50]

#define IMPRINT &at[56]
#define IMPRINT_date &at[57]
#define IMPRINT_volume &at[59]
#define IMPRINT_issue &at[60]
#define IMPRINT_pages &at[61]
#define IMPRINT_section &at[62]
#define IMPRINT_pub &at[63]
#define IMPRINT_cprt &at[64]
#define IMPRINT_part_sup &at[65]
#define IMPRINT_language &at[66]
#define IMPRINT_prepub &at[67]
#define IMPRINT_part_supi &at[68]
#define IMPRINT_retract &at[69]
#define IMPRINT_pubstatus &at[73]
#define IMPRINT_history &at[76]

#define MEETING &at[92]
#define MEETING_number &at[93]
#define MEETING_date &at[94]
#define MEETING_place &at[95]

#define PATENT_PRIORITY &at[131]
#define PATENT_PRIORITY_country &at[132]
#define PATENT_PRIORITY_number &at[133]
#define PATENT_PRIORITY_date &at[134]

#define AFFIL &at[29]
#define AFFIL_str &at[30]
#define AFFIL_std &at[31]
#define AFFIL_std_affil &at[32]
#define AFFIL_std_div &at[33]
#define AFFIL_std_city &at[34]
#define AFFIL_std_sub &at[35]
#define AFFIL_std_country &at[36]
#define AFFIL_std_street &at[37]
#define AFFIL_std_email &at[38]
#define AFFIL_std_fax &at[39]
#define AFFIL_std_phone &at[40]
#define AFFIL_std_postal_code &at[41]

#define CITRETRACT &at[70]
#define CITRETRACT_type &at[71]
#define CITRETRACT_exp &at[72]
