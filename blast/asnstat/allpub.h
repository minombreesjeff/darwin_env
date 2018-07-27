/***********************************************************************
*
**
*        Automatic header module from ASNTOOL
*
************************************************************************/

#ifndef _ASNTOOL_
#include <asn.h>
#endif

static char * asnfilename = "allpub.h63";
static AsnValxNode avnx[71] = {
    {20,"unk" ,0,0.0,&avnx[1] } ,
    {20,"gt" ,1,0.0,&avnx[2] } ,
    {20,"lt" ,2,0.0,&avnx[3] } ,
    {20,"tr" ,3,0.0,&avnx[4] } ,
    {20,"tl" ,4,0.0,&avnx[5] } ,
    {20,"circle" ,5,0.0,&avnx[6] } ,
    {20,"other" ,255,0.0,NULL } ,
    {20,"primary" ,1,0.0,&avnx[8] } ,
    {20,"secondary" ,2,0.0,NULL } ,
    {20,"compiler" ,1,0.0,&avnx[10] } ,
    {20,"editor" ,2,0.0,&avnx[11] } ,
    {20,"patent-assignee" ,3,0.0,&avnx[12] } ,
    {20,"translator" ,4,0.0,NULL } ,
    {1,"ENG" ,0,0.0,NULL } ,
    {20,"submitted" ,1,0.0,&avnx[15] } ,
    {20,"in-press" ,2,0.0,&avnx[16] } ,
    {20,"other" ,255,0.0,NULL } ,
    {20,"retracted" ,1,0.0,&avnx[18] } ,
    {20,"notice" ,2,0.0,&avnx[19] } ,
    {20,"in-error" ,3,0.0,&avnx[20] } ,
    {20,"erratum" ,4,0.0,NULL } ,
    {20,"received" ,1,0.0,&avnx[22] } ,
    {20,"accepted" ,2,0.0,&avnx[23] } ,
    {20,"epublish" ,3,0.0,&avnx[24] } ,
    {20,"ppublish" ,4,0.0,&avnx[25] } ,
    {20,"revised" ,5,0.0,&avnx[26] } ,
    {20,"pmc" ,6,0.0,&avnx[27] } ,
    {20,"pmcr" ,7,0.0,&avnx[28] } ,
    {20,"pubmed" ,8,0.0,&avnx[29] } ,
    {20,"pubmedr" ,9,0.0,&avnx[30] } ,
    {20,"aheadofprint" ,10,0.0,&avnx[31] } ,
    {20,"premedline" ,11,0.0,&avnx[32] } ,
    {20,"medline" ,12,0.0,&avnx[33] } ,
    {20,"other" ,255,0.0,NULL } ,
    {20,"manuscript" ,1,0.0,&avnx[35] } ,
    {20,"letter" ,2,0.0,&avnx[36] } ,
    {20,"thesis" ,3,0.0,NULL } ,
    {20,"paper" ,1,0.0,&avnx[38] } ,
    {20,"tape" ,2,0.0,&avnx[39] } ,
    {20,"floppy" ,3,0.0,&avnx[40] } ,
    {20,"email" ,4,0.0,&avnx[41] } ,
    {20,"other" ,255,0.0,NULL } ,
    {2,NULL,0,0.0,NULL } ,
    {2,NULL,0,0.0,NULL } ,
    {20,"nameonly" ,0,0.0,&avnx[45] } ,
    {20,"cas" ,1,0.0,&avnx[46] } ,
    {20,"ec" ,2,0.0,NULL } ,
    {20,"ddbj" ,1,0.0,&avnx[48] } ,
    {20,"carbbank" ,2,0.0,&avnx[49] } ,
    {20,"embl" ,3,0.0,&avnx[50] } ,
    {20,"hdb" ,4,0.0,&avnx[51] } ,
    {20,"genbank" ,5,0.0,&avnx[52] } ,
    {20,"hgml" ,6,0.0,&avnx[53] } ,
    {20,"mim" ,7,0.0,&avnx[54] } ,
    {20,"msd" ,8,0.0,&avnx[55] } ,
    {20,"pdb" ,9,0.0,&avnx[56] } ,
    {20,"pir" ,10,0.0,&avnx[57] } ,
    {20,"prfseqdb" ,11,0.0,&avnx[58] } ,
    {20,"psd" ,12,0.0,&avnx[59] } ,
    {20,"swissprot" ,13,0.0,&avnx[60] } ,
    {20,"gdb" ,14,0.0,NULL } ,
    {20,"other" ,0,0.0,&avnx[62] } ,
    {20,"comment" ,1,0.0,&avnx[63] } ,
    {20,"erratum" ,2,0.0,NULL } ,
    {20,"medline" ,1,0.0,&avnx[65] } ,
    {20,"pubmed" ,2,0.0,&avnx[66] } ,
    {20,"ncbigi" ,3,0.0,NULL } ,
    {20,"publisher" ,1,0.0,&avnx[68] } ,
    {20,"premedline" ,2,0.0,&avnx[69] } ,
    {20,"medline" ,3,0.0,NULL } ,
    {3,NULL,3,0.0,NULL } };

static AsnType atx[321] = {
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
  {404, "Dbtag" ,1,0,0,0,0,1,0,0,NULL,&atx[13],&atx[18],0,&atx[34]} ,
  {0, "db" ,128,0,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[19]} ,
  {0, "tag" ,128,1,0,0,0,0,0,0,NULL,&atx[20],NULL,0,NULL} ,
  {403, "Object-id" ,1,0,0,0,0,1,0,0,NULL,&atx[14],&atx[21],0,&atx[17]} ,
  {0, "id" ,128,0,0,0,0,0,0,0,NULL,&atx[6],NULL,0,&atx[22]} ,
  {0, "str" ,128,1,0,0,0,0,0,0,NULL,&atx[2],NULL,0,NULL} ,
  {0, "name" ,128,1,0,0,0,0,0,0,NULL,&atx[24],NULL,0,&atx[32]} ,
  {408, "Name-std" ,1,0,0,0,0,0,0,0,NULL,&atx[13],&atx[25],0,&atx[50]} ,
  {0, "last" ,128,0,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[26]} ,
  {0, "first" ,128,1,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[27]} ,
  {0, "middle" ,128,2,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[28]} ,
  {0, "full" ,128,3,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[29]} ,
  {0, "initials" ,128,4,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[30]} ,
  {0, "suffix" ,128,5,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[31]} ,
  {0, "title" ,128,6,0,1,0,0,0,0,NULL,&atx[2],NULL,0,NULL} ,
  {0, "ml" ,128,2,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[33]} ,
  {0, "str" ,128,3,0,0,0,0,0,0,NULL,&atx[2],NULL,0,NULL} ,
  {405, "Int-fuzz" ,1,0,0,0,0,1,0,0,NULL,&atx[14],&atx[35],0,&atx[45]} ,
  {0, "p-m" ,128,0,0,0,0,0,0,0,NULL,&atx[6],NULL,0,&atx[36]} ,
  {0, "range" ,128,1,0,0,0,0,0,0,NULL,&atx[13],&atx[37],0,&atx[39]} ,
  {0, "max" ,128,0,0,0,0,0,0,0,NULL,&atx[6],NULL,0,&atx[38]} ,
  {0, "min" ,128,1,0,0,0,0,0,0,NULL,&atx[6],NULL,0,NULL} ,
  {0, "pct" ,128,2,0,0,0,0,0,0,NULL,&atx[6],NULL,0,&atx[40]} ,
  {0, "lim" ,128,3,0,0,0,0,0,0,NULL,&atx[41],&avnx[0],0,&atx[42]} ,
  {310, "ENUMERATED" ,0,10,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "alt" ,128,4,0,0,0,0,0,0,NULL,&atx[44],&atx[43],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[6],NULL,0,NULL} ,
  {314, "SET OF" ,0,17,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {406, "User-object" ,1,0,0,0,0,1,0,0,NULL,&atx[13],&atx[46],0,&atx[4]} ,
  {0, "class" ,128,0,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[47]} ,
  {0, "type" ,128,1,0,0,0,0,0,0,NULL,&atx[20],NULL,0,&atx[48]} ,
  {0, "data" ,128,2,0,0,0,0,0,0,NULL,&atx[65],&atx[49],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[50],NULL,0,NULL} ,
  {409, "User-field" ,1,0,0,0,0,0,0,0,NULL,&atx[13],&atx[51],0,NULL} ,
  {0, "label" ,128,0,0,0,0,0,0,0,NULL,&atx[20],NULL,0,&atx[52]} ,
  {0, "num" ,128,1,0,1,0,0,0,0,NULL,&atx[6],NULL,0,&atx[53]} ,
  {0, "data" ,128,2,0,0,0,0,0,0,NULL,&atx[14],&atx[54],0,NULL} ,
  {0, "str" ,128,0,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[55]} ,
  {0, "int" ,128,1,0,0,0,0,0,0,NULL,&atx[6],NULL,0,&atx[56]} ,
  {0, "real" ,128,2,0,0,0,0,0,0,NULL,&atx[57],NULL,0,&atx[58]} ,
  {309, "REAL" ,0,9,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "bool" ,128,3,0,0,0,0,0,0,NULL,&atx[59],NULL,0,&atx[60]} ,
  {301, "BOOLEAN" ,0,1,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "os" ,128,4,0,0,0,0,0,0,NULL,&atx[61],NULL,0,&atx[62]} ,
  {304, "OCTET STRING" ,0,4,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "object" ,128,5,0,0,0,0,0,0,NULL,&atx[45],NULL,0,&atx[63]} ,
  {0, "strs" ,128,6,0,0,0,0,0,0,NULL,&atx[65],&atx[64],0,&atx[66]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[2],NULL,0,NULL} ,
  {312, "SEQUENCE OF" ,0,16,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "ints" ,128,7,0,0,0,0,0,0,NULL,&atx[65],&atx[67],0,&atx[68]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[6],NULL,0,NULL} ,
  {0, "reals" ,128,8,0,0,0,0,0,0,NULL,&atx[65],&atx[69],0,&atx[70]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[57],NULL,0,NULL} ,
  {0, "oss" ,128,9,0,0,0,0,0,0,NULL,&atx[65],&atx[71],0,&atx[72]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[61],NULL,0,NULL} ,
  {0, "fields" ,128,10,0,0,0,0,0,0,NULL,&atx[65],&atx[73],0,&atx[74]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[50],NULL,0,NULL} ,
  {0, "objects" ,128,11,0,0,0,0,0,0,NULL,&atx[65],&atx[75],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[45],NULL,0,NULL} ,
  {401, "Cit-art" ,1,0,0,0,0,1,0,0,NULL,&atx[13],&atx[77],0,&atx[122]} ,
  {0, "title" ,128,0,0,1,0,0,0,0,NULL,&atx[78],NULL,0,&atx[90]} ,
  {410, "Title" ,1,0,0,0,0,1,0,0,NULL,&atx[44],&atx[79],0,&atx[95]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[14],&atx[80],0,NULL} ,
  {0, "name" ,128,0,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[81]} ,
  {0, "tsub" ,128,1,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[82]} ,
  {0, "trans" ,128,2,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[83]} ,
  {0, "jta" ,128,3,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[84]} ,
  {0, "iso-jta" ,128,4,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[85]} ,
  {0, "ml-jta" ,128,5,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[86]} ,
  {0, "coden" ,128,6,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[87]} ,
  {0, "issn" ,128,7,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[88]} ,
  {0, "abr" ,128,8,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[89]} ,
  {0, "isbn" ,128,9,0,0,0,0,0,0,NULL,&atx[2],NULL,0,NULL} ,
  {0, "authors" ,128,1,0,1,0,0,0,0,NULL,&atx[91],NULL,0,&atx[120]} ,
  {427, "Auth-list" ,1,0,0,0,0,0,0,0,NULL,&atx[13],&atx[92],0,&atx[125]} ,
  {0, "names" ,128,0,0,0,0,0,0,0,NULL,&atx[14],&atx[93],0,&atx[119]} ,
  {0, "std" ,128,0,0,0,0,0,0,0,NULL,&atx[65],&atx[94],0,&atx[115]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[95],NULL,0,NULL} ,
  {411, "Author" ,1,0,0,0,0,1,0,0,NULL,&atx[13],&atx[96],0,&atx[169]} ,
  {0, "name" ,128,0,0,0,0,0,0,0,NULL,&atx[97],NULL,0,&atx[98]} ,
  {413, "Person-id" ,1,0,0,0,0,0,1,0,NULL,&atx[15],NULL,0,&atx[127]} ,
  {0, "level" ,128,1,0,1,0,0,0,0,NULL,&atx[41],&avnx[7],0,&atx[99]} ,
  {0, "role" ,128,2,0,1,0,0,0,0,NULL,&atx[41],&avnx[9],0,&atx[100]} ,
  {0, "affil" ,128,3,0,1,0,0,0,0,NULL,&atx[101],NULL,0,&atx[114]} ,
  {431, "Affil" ,1,0,0,0,0,0,0,0,NULL,&atx[14],&atx[102],0,&atx[139]} ,
  {0, "str" ,128,0,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[103]} ,
  {0, "std" ,128,1,0,0,0,0,0,0,NULL,&atx[13],&atx[104],0,NULL} ,
  {0, "affil" ,128,0,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[105]} ,
  {0, "div" ,128,1,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[106]} ,
  {0, "city" ,128,2,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[107]} ,
  {0, "sub" ,128,3,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[108]} ,
  {0, "country" ,128,4,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[109]} ,
  {0, "street" ,128,5,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[110]} ,
  {0, "email" ,128,6,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[111]} ,
  {0, "fax" ,128,7,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[112]} ,
  {0, "phone" ,128,8,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[113]} ,
  {0, "postal-code" ,128,9,0,1,0,0,0,0,NULL,&atx[2],NULL,0,NULL} ,
  {0, "is-corr" ,128,4,0,1,0,0,0,0,NULL,&atx[59],NULL,0,NULL} ,
  {0, "ml" ,128,1,0,0,0,0,0,0,NULL,&atx[65],&atx[116],0,&atx[117]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[2],NULL,0,NULL} ,
  {0, "str" ,128,2,0,0,0,0,0,0,NULL,&atx[65],&atx[118],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[2],NULL,0,NULL} ,
  {0, "affil" ,128,1,0,1,0,0,0,0,NULL,&atx[101],NULL,0,NULL} ,
  {0, "from" ,128,2,0,0,0,0,0,0,NULL,&atx[14],&atx[121],0,&atx[164]} ,
  {0, "journal" ,128,0,0,0,0,0,0,0,NULL,&atx[122],NULL,0,&atx[150]} ,
  {402, "Cit-jour" ,1,0,0,0,0,1,0,0,NULL,&atx[13],&atx[123],0,&atx[151]} ,
  {0, "title" ,128,0,0,0,0,0,0,0,NULL,&atx[78],NULL,0,&atx[124]} ,
  {0, "imp" ,128,1,0,0,0,0,0,0,NULL,&atx[125],NULL,0,NULL} ,
  {428, "Imprint" ,1,0,0,0,0,0,0,0,NULL,&atx[13],&atx[126],0,&atx[160]} ,
  {0, "date" ,128,0,0,0,0,0,0,0,NULL,&atx[127],NULL,0,&atx[128]} ,
  {414, "Date" ,1,0,0,0,0,0,1,0,NULL,&atx[0],NULL,0,&atx[183]} ,
  {0, "volume" ,128,1,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[129]} ,
  {0, "issue" ,128,2,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[130]} ,
  {0, "pages" ,128,3,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[131]} ,
  {0, "section" ,128,4,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[132]} ,
  {0, "pub" ,128,5,0,1,0,0,0,0,NULL,&atx[101],NULL,0,&atx[133]} ,
  {0, "cprt" ,128,6,0,1,0,0,0,0,NULL,&atx[127],NULL,0,&atx[134]} ,
  {0, "part-sup" ,128,7,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[135]} ,
  {0, "language" ,128,8,0,0,1,0,0,0,&avnx[13],&atx[2],NULL,0,&atx[136]} ,
  {0, "prepub" ,128,9,0,1,0,0,0,0,NULL,&atx[41],&avnx[14],0,&atx[137]} ,
  {0, "part-supi" ,128,10,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[138]} ,
  {0, "retract" ,128,11,0,1,0,0,0,0,NULL,&atx[139],NULL,0,&atx[142]} ,
  {432, "CitRetract" ,1,0,0,0,0,0,0,0,NULL,&atx[13],&atx[140],0,NULL} ,
  {0, "type" ,128,0,0,0,0,0,0,0,NULL,&atx[41],&avnx[17],0,&atx[141]} ,
  {0, "exp" ,128,1,0,1,0,0,0,0,NULL,&atx[2],NULL,0,NULL} ,
  {0, "pubstatus" ,128,12,0,1,0,0,0,0,NULL,&atx[143],NULL,0,&atx[144]} ,
  {424, "PubStatus" ,1,0,0,0,0,0,0,0,NULL,&atx[6],&avnx[21],0,&atx[147]} ,
  {0, "history" ,128,13,0,1,0,0,0,0,NULL,&atx[145],NULL,0,NULL} ,
  {426, "PubStatusDateSet" ,1,0,0,0,0,0,0,0,NULL,&atx[44],&atx[146],0,&atx[91]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[147],NULL,0,NULL} ,
  {425, "PubStatusDate" ,1,0,0,0,0,0,0,0,NULL,&atx[13],&atx[148],0,&atx[145]} ,
  {0, "pubstatus" ,128,0,0,0,0,0,0,0,NULL,&atx[143],NULL,0,&atx[149]} ,
  {0, "date" ,128,1,0,0,0,0,0,0,NULL,&atx[127],NULL,0,NULL} ,
  {0, "book" ,128,1,0,0,0,0,0,0,NULL,&atx[151],NULL,0,&atx[156]} ,
  {403, "Cit-book" ,1,0,0,0,0,1,0,0,NULL,&atx[13],&atx[152],0,&atx[184]} ,
  {0, "title" ,128,0,0,0,0,0,0,0,NULL,&atx[78],NULL,0,&atx[153]} ,
  {0, "coll" ,128,1,0,1,0,0,0,0,NULL,&atx[78],NULL,0,&atx[154]} ,
  {0, "authors" ,128,2,0,0,0,0,0,0,NULL,&atx[91],NULL,0,&atx[155]} ,
  {0, "imp" ,128,3,0,0,0,0,0,0,NULL,&atx[125],NULL,0,NULL} ,
  {0, "proc" ,128,2,0,0,0,0,0,0,NULL,&atx[157],NULL,0,NULL} ,
  {408, "Cit-proc" ,1,0,0,0,0,1,0,0,NULL,&atx[13],&atx[158],0,&atx[226]} ,
  {0, "book" ,128,0,0,0,0,0,0,0,NULL,&atx[151],NULL,0,&atx[159]} ,
  {0, "meet" ,128,1,0,0,0,0,0,0,NULL,&atx[160],NULL,0,NULL} ,
  {429, "Meeting" ,1,0,0,0,0,0,0,0,NULL,&atx[13],&atx[161],0,&atx[199]} ,
  {0, "number" ,128,0,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[162]} ,
  {0, "date" ,128,1,0,0,0,0,0,0,NULL,&atx[127],NULL,0,&atx[163]} ,
  {0, "place" ,128,2,0,1,0,0,0,0,NULL,&atx[101],NULL,0,NULL} ,
  {0, "ids" ,128,3,0,1,0,0,0,0,NULL,&atx[165],NULL,0,NULL} ,
  {423, "ArticleIdSet" ,1,0,0,0,0,0,0,0,NULL,&atx[44],&atx[166],0,&atx[143]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[167],NULL,0,NULL} ,
  {416, "ArticleId" ,1,0,0,0,0,0,0,0,NULL,&atx[14],&atx[168],0,&atx[171]} ,
  {0, "pubmed" ,128,0,0,0,0,0,0,0,NULL,&atx[169],NULL,0,&atx[170]} ,
  {412, "PubMedId" ,1,0,0,0,0,1,0,0,NULL,&atx[6],NULL,0,&atx[97]} ,
  {0, "medline" ,128,1,0,0,0,0,0,0,NULL,&atx[171],NULL,0,&atx[172]} ,
  {417, "MedlineUID" ,1,0,0,0,0,0,0,0,NULL,&atx[6],NULL,0,&atx[173]} ,
  {0, "doi" ,128,2,0,0,0,0,0,0,NULL,&atx[173],NULL,0,&atx[174]} ,
  {418, "DOI" ,1,0,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[175]} ,
  {0, "pii" ,128,3,0,0,0,0,0,0,NULL,&atx[175],NULL,0,&atx[176]} ,
  {419, "PII" ,1,0,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[177]} ,
  {0, "pmcid" ,128,4,0,0,0,0,0,0,NULL,&atx[177],NULL,0,&atx[178]} ,
  {420, "PmcID" ,1,0,0,0,0,0,0,0,NULL,&atx[6],NULL,0,&atx[179]} ,
  {0, "pmcpid" ,128,5,0,0,0,0,0,0,NULL,&atx[179],NULL,0,&atx[180]} ,
  {421, "PmcPid" ,1,0,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[181]} ,
  {0, "pmpid" ,128,6,0,0,0,0,0,0,NULL,&atx[181],NULL,0,&atx[182]} ,
  {422, "PmPid" ,1,0,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[165]} ,
  {0, "other" ,128,7,0,0,0,0,0,0,NULL,&atx[183],NULL,0,NULL} ,
  {415, "Dbtag" ,1,0,0,0,0,0,1,0,NULL,&atx[17],NULL,0,&atx[167]} ,
  {404, "Cit-pat" ,1,0,0,0,0,1,0,0,NULL,&atx[13],&atx[185],0,&atx[204]} ,
  {0, "title" ,128,0,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[186]} ,
  {0, "authors" ,128,1,0,0,0,0,0,0,NULL,&atx[91],NULL,0,&atx[187]} ,
  {0, "country" ,128,2,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[188]} ,
  {0, "doc-type" ,128,3,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[189]} ,
  {0, "number" ,128,4,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[190]} ,
  {0, "date-issue" ,128,5,0,1,0,0,0,0,NULL,&atx[127],NULL,0,&atx[191]} ,
  {0, "class" ,128,6,0,1,0,0,0,0,NULL,&atx[65],&atx[192],0,&atx[193]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[2],NULL,0,NULL} ,
  {0, "app-number" ,128,7,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[194]} ,
  {0, "app-date" ,128,8,0,1,0,0,0,0,NULL,&atx[127],NULL,0,&atx[195]} ,
  {0, "applicants" ,128,9,0,1,0,0,0,0,NULL,&atx[91],NULL,0,&atx[196]} ,
  {0, "assignees" ,128,10,0,1,0,0,0,0,NULL,&atx[91],NULL,0,&atx[197]} ,
  {0, "priority" ,128,11,0,1,0,0,0,0,NULL,&atx[65],&atx[198],0,&atx[203]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[199],NULL,0,NULL} ,
  {430, "Patent-priority" ,1,0,0,0,0,0,0,0,NULL,&atx[13],&atx[200],0,&atx[101]} ,
  {0, "country" ,128,0,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[201]} ,
  {0, "number" ,128,1,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[202]} ,
  {0, "date" ,128,2,0,0,0,0,0,0,NULL,&atx[127],NULL,0,NULL} ,
  {0, "abstract" ,128,12,0,1,0,0,0,0,NULL,&atx[2],NULL,0,NULL} ,
  {405, "Cit-let" ,1,0,0,0,0,1,0,0,NULL,&atx[13],&atx[205],0,&atx[208]} ,
  {0, "cit" ,128,0,0,0,0,0,0,0,NULL,&atx[151],NULL,0,&atx[206]} ,
  {0, "man-id" ,128,1,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[207]} ,
  {0, "type" ,128,2,0,1,0,0,0,0,NULL,&atx[41],&avnx[34],0,NULL} ,
  {406, "Id-pat" ,1,0,0,0,0,1,0,0,NULL,&atx[13],&atx[209],0,&atx[214]} ,
  {0, "country" ,128,0,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[210]} ,
  {0, "id" ,128,1,0,0,0,0,0,0,NULL,&atx[14],&atx[211],0,&atx[213]} ,
  {0, "number" ,128,0,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[212]} ,
  {0, "app-number" ,128,1,0,0,0,0,0,0,NULL,&atx[2],NULL,0,NULL} ,
  {0, "doc-type" ,128,2,0,1,0,0,0,0,NULL,&atx[2],NULL,0,NULL} ,
  {407, "Cit-gen" ,1,0,0,0,0,1,0,0,NULL,&atx[13],&atx[215],0,&atx[157]} ,
  {0, "cit" ,128,0,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[216]} ,
  {0, "authors" ,128,1,0,1,0,0,0,0,NULL,&atx[91],NULL,0,&atx[217]} ,
  {0, "muid" ,128,2,0,1,0,0,0,0,NULL,&atx[6],NULL,0,&atx[218]} ,
  {0, "journal" ,128,3,0,1,0,0,0,0,NULL,&atx[78],NULL,0,&atx[219]} ,
  {0, "volume" ,128,4,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[220]} ,
  {0, "issue" ,128,5,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[221]} ,
  {0, "pages" ,128,6,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[222]} ,
  {0, "date" ,128,7,0,1,0,0,0,0,NULL,&atx[127],NULL,0,&atx[223]} ,
  {0, "serial-number" ,128,8,0,1,0,0,0,0,NULL,&atx[6],NULL,0,&atx[224]} ,
  {0, "title" ,128,9,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[225]} ,
  {0, "pmid" ,128,10,0,1,0,0,0,0,NULL,&atx[169],NULL,0,NULL} ,
  {409, "Cit-sub" ,1,0,0,0,0,1,0,0,NULL,&atx[13],&atx[227],0,&atx[78]} ,
  {0, "authors" ,128,0,0,0,0,0,0,0,NULL,&atx[91],NULL,0,&atx[228]} ,
  {0, "imp" ,128,1,0,1,0,0,0,0,NULL,&atx[125],NULL,0,&atx[229]} ,
  {0, "medium" ,128,2,0,1,0,0,0,0,NULL,&atx[41],&avnx[37],0,&atx[230]} ,
  {0, "date" ,128,3,0,1,0,0,0,0,NULL,&atx[127],NULL,0,&atx[231]} ,
  {0, "descr" ,128,4,0,1,0,0,0,0,NULL,&atx[2],NULL,0,NULL} ,
  {401, "Medline-entry" ,1,0,0,0,0,1,0,0,NULL,&atx[13],&atx[233],0,&atx[257]} ,
  {0, "uid" ,128,0,0,1,0,0,0,0,NULL,&atx[6],NULL,0,&atx[234]} ,
  {0, "em" ,128,1,0,0,0,0,0,0,NULL,&atx[235],NULL,0,&atx[236]} ,
  {405, "Date" ,1,0,0,0,0,0,1,0,NULL,&atx[0],NULL,0,&atx[241]} ,
  {0, "cit" ,128,2,0,0,0,0,0,0,NULL,&atx[237],NULL,0,&atx[238]} ,
  {403, "Cit-art" ,1,0,0,0,0,0,1,0,NULL,&atx[76],NULL,0,&atx[265]} ,
  {0, "abstract" ,128,3,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[239]} ,
  {0, "mesh" ,128,4,0,1,0,0,0,0,NULL,&atx[44],&atx[240],0,&atx[249]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[241],NULL,0,NULL} ,
  {406, "Medline-mesh" ,1,0,0,0,0,0,0,0,NULL,&atx[13],&atx[242],0,&atx[251]} ,
  {0, "mp" ,128,0,0,0,1,0,0,0,&avnx[42],&atx[59],NULL,0,&atx[243]} ,
  {0, "term" ,128,1,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[244]} ,
  {0, "qual" ,128,2,0,1,0,0,0,0,NULL,&atx[44],&atx[245],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[246],NULL,0,NULL} ,
  {409, "Medline-qual" ,1,0,0,0,0,0,0,0,NULL,&atx[13],&atx[247],0,&atx[275]} ,
  {0, "mp" ,128,0,0,0,1,0,0,0,&avnx[43],&atx[59],NULL,0,&atx[248]} ,
  {0, "subh" ,128,1,0,0,0,0,0,0,NULL,&atx[2],NULL,0,NULL} ,
  {0, "substance" ,128,5,0,1,0,0,0,0,NULL,&atx[44],&atx[250],0,&atx[255]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[251],NULL,0,NULL} ,
  {407, "Medline-rn" ,1,0,0,0,0,0,0,0,NULL,&atx[13],&atx[252],0,&atx[270]} ,
  {0, "type" ,128,0,0,0,0,0,0,0,NULL,&atx[41],&avnx[44],0,&atx[253]} ,
  {0, "cit" ,128,1,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[254]} ,
  {0, "name" ,128,2,0,0,0,0,0,0,NULL,&atx[2],NULL,0,NULL} ,
  {0, "xref" ,128,6,0,1,0,0,0,0,NULL,&atx[44],&atx[256],0,&atx[260]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[257],NULL,0,NULL} ,
  {402, "Medline-si" ,1,0,0,0,0,1,0,0,NULL,&atx[13],&atx[258],0,&atx[237]} ,
  {0, "type" ,128,0,0,0,0,0,0,0,NULL,&atx[41],&avnx[47],0,&atx[259]} ,
  {0, "cit" ,128,1,0,1,0,0,0,0,NULL,&atx[2],NULL,0,NULL} ,
  {0, "idnum" ,128,7,0,1,0,0,0,0,NULL,&atx[44],&atx[261],0,&atx[262]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[2],NULL,0,NULL} ,
  {0, "gene" ,128,8,0,1,0,0,0,0,NULL,&atx[44],&atx[263],0,&atx[264]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[2],NULL,0,NULL} ,
  {0, "pmid" ,128,9,0,1,0,0,0,0,NULL,&atx[265],NULL,0,&atx[266]} ,
  {404, "PubMedId" ,1,0,0,0,0,0,1,0,NULL,&atx[169],NULL,0,&atx[235]} ,
  {0, "pub-type" ,128,10,0,1,0,0,0,0,NULL,&atx[44],&atx[267],0,&atx[268]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[2],NULL,0,NULL} ,
  {0, "mlfield" ,128,11,0,1,0,0,0,0,NULL,&atx[44],&atx[269],0,&atx[278]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[270],NULL,0,NULL} ,
  {408, "Medline-field" ,1,0,0,0,0,0,0,0,NULL,&atx[13],&atx[271],0,&atx[246]} ,
  {0, "type" ,128,0,0,0,0,0,0,0,NULL,&atx[6],&avnx[61],0,&atx[272]} ,
  {0, "str" ,128,1,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[273]} ,
  {0, "ids" ,128,2,0,1,0,0,0,0,NULL,&atx[65],&atx[274],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[275],NULL,0,NULL} ,
  {410, "DocRef" ,1,0,0,0,0,0,0,0,NULL,&atx[13],&atx[276],0,NULL} ,
  {0, "type" ,128,0,0,0,0,0,0,0,NULL,&atx[6],&avnx[64],0,&atx[277]} ,
  {0, "uid" ,128,1,0,0,0,0,0,0,NULL,&atx[6],NULL,0,NULL} ,
  {0, "status" ,128,12,0,0,1,0,0,0,&avnx[70],&atx[6],&avnx[67],0,NULL} ,
  {401, "Pub" ,1,0,0,0,0,1,0,0,NULL,&atx[14],&atx[280],0,&atx[306]} ,
  {0, "gen" ,128,0,0,0,0,0,0,0,NULL,&atx[281],NULL,0,&atx[282]} ,
  {411, "Cit-gen" ,1,0,0,0,0,0,1,0,NULL,&atx[214],NULL,0,&atx[300]} ,
  {0, "sub" ,128,1,0,0,0,0,0,0,NULL,&atx[283],NULL,0,&atx[284]} ,
  {413, "Cit-sub" ,1,0,0,0,0,0,1,0,NULL,&atx[226],NULL,0,&atx[305]} ,
  {0, "medline" ,128,2,0,0,0,0,0,0,NULL,&atx[285],NULL,0,&atx[286]} ,
  {404, "Medline-entry" ,1,0,0,0,0,0,1,0,NULL,&atx[232],NULL,0,&atx[288]} ,
  {0, "muid" ,128,3,0,0,0,0,0,0,NULL,&atx[6],NULL,0,&atx[287]} ,
  {0, "article" ,128,4,0,0,0,0,0,0,NULL,&atx[288],NULL,0,&atx[289]} ,
  {405, "Cit-art" ,1,0,0,0,0,0,1,0,NULL,&atx[76],NULL,0,&atx[290]} ,
  {0, "journal" ,128,5,0,0,0,0,0,0,NULL,&atx[290],NULL,0,&atx[291]} ,
  {406, "Cit-jour" ,1,0,0,0,0,0,1,0,NULL,&atx[122],NULL,0,&atx[292]} ,
  {0, "book" ,128,6,0,0,0,0,0,0,NULL,&atx[292],NULL,0,&atx[293]} ,
  {407, "Cit-book" ,1,0,0,0,0,0,1,0,NULL,&atx[151],NULL,0,&atx[294]} ,
  {0, "proc" ,128,7,0,0,0,0,0,0,NULL,&atx[294],NULL,0,&atx[295]} ,
  {408, "Cit-proc" ,1,0,0,0,0,0,1,0,NULL,&atx[157],NULL,0,&atx[296]} ,
  {0, "patent" ,128,8,0,0,0,0,0,0,NULL,&atx[296],NULL,0,&atx[297]} ,
  {409, "Cit-pat" ,1,0,0,0,0,0,1,0,NULL,&atx[184],NULL,0,&atx[298]} ,
  {0, "pat-id" ,128,9,0,0,0,0,0,0,NULL,&atx[298],NULL,0,&atx[299]} ,
  {410, "Id-pat" ,1,0,0,0,0,0,1,0,NULL,&atx[208],NULL,0,&atx[281]} ,
  {0, "man" ,128,10,0,0,0,0,0,0,NULL,&atx[300],NULL,0,&atx[301]} ,
  {412, "Cit-let" ,1,0,0,0,0,0,1,0,NULL,&atx[204],NULL,0,&atx[283]} ,
  {0, "equiv" ,128,11,0,0,0,0,0,0,NULL,&atx[302],NULL,0,&atx[304]} ,
  {403, "Pub-equiv" ,1,0,0,0,0,1,0,0,NULL,&atx[44],&atx[303],0,&atx[285]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[279],NULL,0,NULL} ,
  {0, "pmid" ,128,12,0,0,0,0,0,0,NULL,&atx[305],NULL,0,NULL} ,
  {414, "PubMedId" ,1,0,0,0,0,0,1,0,NULL,&atx[169],NULL,0,NULL} ,
  {402, "Pub-set" ,1,0,0,0,0,1,0,0,NULL,&atx[14],&atx[307],0,&atx[302]} ,
  {0, "pub" ,128,0,0,0,0,0,0,0,NULL,&atx[44],&atx[308],0,&atx[309]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[279],NULL,0,NULL} ,
  {0, "medline" ,128,1,0,0,0,0,0,0,NULL,&atx[44],&atx[310],0,&atx[311]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[285],NULL,0,NULL} ,
  {0, "article" ,128,2,0,0,0,0,0,0,NULL,&atx[44],&atx[312],0,&atx[313]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[288],NULL,0,NULL} ,
  {0, "journal" ,128,3,0,0,0,0,0,0,NULL,&atx[44],&atx[314],0,&atx[315]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[290],NULL,0,NULL} ,
  {0, "book" ,128,4,0,0,0,0,0,0,NULL,&atx[44],&atx[316],0,&atx[317]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[292],NULL,0,NULL} ,
  {0, "proc" ,128,5,0,0,0,0,0,0,NULL,&atx[44],&atx[318],0,&atx[319]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[294],NULL,0,NULL} ,
  {0, "patent" ,128,6,0,0,0,0,0,0,NULL,&atx[44],&atx[320],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[296],NULL,0,NULL} };

static AsnModule ampx[4] = {
  { "NCBI-General" , "allpub.h63",&atx[0],NULL,&ampx[1],0,0} ,
  { "NCBI-Biblio" , NULL,&atx[76],NULL,&ampx[2],0,0} ,
  { "NCBI-Medline" , NULL,&atx[232],NULL,&ampx[3],0,0} ,
  { "NCBI-Pub" , NULL,&atx[279],NULL,NULL,0,0} };

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

#define OBJECT_ID &at[20]
#define OBJECT_ID_id &at[21]
#define OBJECT_ID_str &at[22]

#define DBTAG &at[17]
#define DBTAG_db &at[18]
#define DBTAG_tag &at[19]

#define INT_FUZZ &at[34]
#define INT_FUZZ_p_m &at[35]
#define INT_FUZZ_range &at[36]
#define INT_FUZZ_range_max &at[37]
#define INT_FUZZ_range_min &at[38]
#define INT_FUZZ_pct &at[39]
#define INT_FUZZ_lim &at[40]
#define INT_FUZZ_alt &at[42]
#define INT_FUZZ_alt_E &at[43]

#define USER_OBJECT &at[45]
#define USER_OBJECT_class &at[46]
#define USER_OBJECT_type &at[47]
#define USER_OBJECT_data &at[48]
#define USER_OBJECT_data_E &at[49]

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

#define USER_FIELD &at[50]
#define USER_FIELD_label &at[51]
#define USER_FIELD_num &at[52]
#define USER_FIELD_data &at[53]
#define USER_FIELD_data_str &at[54]
#define USER_FIELD_data_int &at[55]
#define USER_FIELD_data_real &at[56]
#define USER_FIELD_data_bool &at[58]
#define USER_FIELD_data_os &at[60]
#define USER_FIELD_data_object &at[62]
#define USER_FIELD_data_strs &at[63]
#define USER_FIELD_data_strs_E &at[64]
#define USER_FIELD_data_ints &at[66]
#define USER_FIELD_data_ints_E &at[67]
#define USER_FIELD_data_reals &at[68]
#define USER_FIELD_data_reals_E &at[69]
#define USER_FIELD_data_oss &at[70]
#define USER_FIELD_data_oss_E &at[71]
#define USER_FIELD_data_fields &at[72]
#define USER_FIELD_data_fields_E &at[73]
#define USER_FIELD_data_objects &at[74]
#define USER_FIELD_data_objects_E &at[75]


/**************************************************
*
*    Defines for Module NCBI-Biblio
*
**************************************************/

#define CIT_ART &at[76]
#define CIT_ART_title &at[77]
#define CIT_ART_authors &at[90]
#define CIT_ART_from &at[120]
#define CIT_ART_from_journal &at[121]
#define CIT_ART_from_book &at[150]
#define CIT_ART_from_proc &at[156]
#define CIT_ART_ids &at[164]

#define CIT_JOUR &at[122]
#define CIT_JOUR_title &at[123]
#define CIT_JOUR_imp &at[124]

#define CIT_BOOK &at[151]
#define CIT_BOOK_title &at[152]
#define CIT_BOOK_coll &at[153]
#define CIT_BOOK_authors &at[154]
#define CIT_BOOK_imp &at[155]

#define CIT_PAT &at[184]
#define CIT_PAT_title &at[185]
#define CIT_PAT_authors &at[186]
#define CIT_PAT_country &at[187]
#define CIT_PAT_doc_type &at[188]
#define CIT_PAT_number &at[189]
#define CIT_PAT_date_issue &at[190]
#define CIT_PAT_class &at[191]
#define CIT_PAT_class_E &at[192]
#define CIT_PAT_app_number &at[193]
#define CIT_PAT_app_date &at[194]
#define CIT_PAT_applicants &at[195]
#define CIT_PAT_assignees &at[196]
#define CIT_PAT_priority &at[197]
#define CIT_PAT_priority_E &at[198]
#define CIT_PAT_abstract &at[203]

#define CIT_LET &at[204]
#define CIT_LET_cit &at[205]
#define CIT_LET_man_id &at[206]
#define CIT_LET_type &at[207]

#define ID_PAT &at[208]
#define ID_PAT_country &at[209]
#define ID_PAT_id &at[210]
#define ID_PAT_id_number &at[211]
#define ID_PAT_id_app_number &at[212]
#define ID_PAT_doc_type &at[213]

#define CIT_GEN &at[214]
#define CIT_GEN_cit &at[215]
#define CIT_GEN_authors &at[216]
#define CIT_GEN_muid &at[217]
#define CIT_GEN_journal &at[218]
#define CIT_GEN_volume &at[219]
#define CIT_GEN_issue &at[220]
#define CIT_GEN_pages &at[221]
#define CIT_GEN_date &at[222]
#define CIT_GEN_serial_number &at[223]
#define CIT_GEN_title &at[224]
#define CIT_GEN_pmid &at[225]

#define CIT_PROC &at[157]
#define CIT_PROC_book &at[158]
#define CIT_PROC_meet &at[159]

#define CIT_SUB &at[226]
#define CIT_SUB_authors &at[227]
#define CIT_SUB_imp &at[228]
#define CIT_SUB_medium &at[229]
#define CIT_SUB_date &at[230]
#define CIT_SUB_descr &at[231]

#define TITLE &at[78]
#define TITLE_E &at[79]
#define TITLE_E_name &at[80]
#define TITLE_E_tsub &at[81]
#define TITLE_E_trans &at[82]
#define TITLE_E_jta &at[83]
#define TITLE_E_iso_jta &at[84]
#define TITLE_E_ml_jta &at[85]
#define TITLE_E_coden &at[86]
#define TITLE_E_issn &at[87]
#define TITLE_E_abr &at[88]
#define TITLE_E_isbn &at[89]

#define AUTHOR &at[95]
#define AUTHOR_name &at[96]
#define AUTHOR_level &at[98]
#define AUTHOR_role &at[99]
#define AUTHOR_affil &at[100]
#define AUTHOR_is_corr &at[114]

#define PUBMEDID &at[169]

#define ARTICLEID &at[167]
#define ARTICLEID_pubmed &at[168]
#define ARTICLEID_medline &at[170]
#define ARTICLEID_doi &at[172]
#define ARTICLEID_pii &at[174]
#define ARTICLEID_pmcid &at[176]
#define ARTICLEID_pmcpid &at[178]
#define ARTICLEID_pmpid &at[180]
#define ARTICLEID_other &at[182]

#define MEDLINEUID &at[171]

#define DOI &at[173]

#define PII &at[175]

#define PMCID &at[177]

#define PMCPID &at[179]

#define PMPID &at[181]

#define ARTICLEIDSET &at[165]
#define ARTICLEIDSET_E &at[166]

#define PUBSTATUS &at[143]

#define PUBSTATUSDATE &at[147]
#define PUBSTATUSDATE_pubstatus &at[148]
#define PUBSTATUSDATE_date &at[149]

#define PUBSTATUSDATESET &at[145]
#define PUBSTATUSDATESET_E &at[146]

#define AUTH_LIST &at[91]
#define AUTH_LIST_names &at[92]
#define AUTH_LIST_names_std &at[93]
#define AUTH_LIST_names_std_E &at[94]
#define AUTH_LIST_names_ml &at[115]
#define AUTH_LIST_names_ml_E &at[116]
#define AUTH_LIST_names_str &at[117]
#define AUTH_LIST_names_str_E &at[118]
#define AUTH_LIST_affil &at[119]

#define IMPRINT &at[125]
#define IMPRINT_date &at[126]
#define IMPRINT_volume &at[128]
#define IMPRINT_issue &at[129]
#define IMPRINT_pages &at[130]
#define IMPRINT_section &at[131]
#define IMPRINT_pub &at[132]
#define IMPRINT_cprt &at[133]
#define IMPRINT_part_sup &at[134]
#define IMPRINT_language &at[135]
#define IMPRINT_prepub &at[136]
#define IMPRINT_part_supi &at[137]
#define IMPRINT_retract &at[138]
#define IMPRINT_pubstatus &at[142]
#define IMPRINT_history &at[144]

#define MEETING &at[160]
#define MEETING_number &at[161]
#define MEETING_date &at[162]
#define MEETING_place &at[163]

#define PATENT_PRIORITY &at[199]
#define PATENT_PRIORITY_country &at[200]
#define PATENT_PRIORITY_number &at[201]
#define PATENT_PRIORITY_date &at[202]

#define AFFIL &at[101]
#define AFFIL_str &at[102]
#define AFFIL_std &at[103]
#define AFFIL_std_affil &at[104]
#define AFFIL_std_div &at[105]
#define AFFIL_std_city &at[106]
#define AFFIL_std_sub &at[107]
#define AFFIL_std_country &at[108]
#define AFFIL_std_street &at[109]
#define AFFIL_std_email &at[110]
#define AFFIL_std_fax &at[111]
#define AFFIL_std_phone &at[112]
#define AFFIL_std_postal_code &at[113]

#define CITRETRACT &at[139]
#define CITRETRACT_type &at[140]
#define CITRETRACT_exp &at[141]


/**************************************************
*
*    Defines for Module NCBI-Medline
*
**************************************************/

#define MEDLINE_ENTRY &at[232]
#define MEDLINE_ENTRY_uid &at[233]
#define MEDLINE_ENTRY_em &at[234]
#define MEDLINE_ENTRY_cit &at[236]
#define MEDLINE_ENTRY_abstract &at[238]
#define MEDLINE_ENTRY_mesh &at[239]
#define MEDLINE_ENTRY_mesh_E &at[240]
#define MEDLINE_ENTRY_substance &at[249]
#define MEDLINE_ENTRY_substance_E &at[250]
#define MEDLINE_ENTRY_xref &at[255]
#define MEDLINE_ENTRY_xref_E &at[256]
#define MEDLINE_ENTRY_idnum &at[260]
#define MEDLINE_ENTRY_idnum_E &at[261]
#define MEDLINE_ENTRY_gene &at[262]
#define MEDLINE_ENTRY_gene_E &at[263]
#define MEDLINE_ENTRY_pmid &at[264]
#define MEDLINE_ENTRY_pub_type &at[266]
#define MEDLINE_ENTRY_pub_type_E &at[267]
#define MEDLINE_ENTRY_mlfield &at[268]
#define MEDLINE_ENTRY_mlfield_E &at[269]
#define MEDLINE_ENTRY_status &at[278]

#define MEDLINE_SI &at[257]
#define MEDLINE_SI_type &at[258]
#define MEDLINE_SI_cit &at[259]

#define MEDLINE_MESH &at[241]
#define MEDLINE_MESH_mp &at[242]
#define MEDLINE_MESH_term &at[243]
#define MEDLINE_MESH_qual &at[244]
#define MEDLINE_MESH_qual_E &at[245]

#define MEDLINE_RN &at[251]
#define MEDLINE_RN_type &at[252]
#define MEDLINE_RN_cit &at[253]
#define MEDLINE_RN_name &at[254]

#define MEDLINE_FIELD &at[270]
#define MEDLINE_FIELD_type &at[271]
#define MEDLINE_FIELD_str &at[272]
#define MEDLINE_FIELD_ids &at[273]
#define MEDLINE_FIELD_ids_E &at[274]

#define MEDLINE_QUAL &at[246]
#define MEDLINE_QUAL_mp &at[247]
#define MEDLINE_QUAL_subh &at[248]

#define DOCREF &at[275]
#define DOCREF_type &at[276]
#define DOCREF_uid &at[277]


/**************************************************
*
*    Defines for Module NCBI-Pub
*
**************************************************/

#define PUB &at[279]
#define PUB_gen &at[280]
#define PUB_sub &at[282]
#define PUB_medline &at[284]
#define PUB_muid &at[286]
#define PUB_article &at[287]
#define PUB_journal &at[289]
#define PUB_book &at[291]
#define PUB_proc &at[293]
#define PUB_patent &at[295]
#define PUB_pat_id &at[297]
#define PUB_man &at[299]
#define PUB_equiv &at[301]
#define PUB_pmid &at[304]

#define PUB_SET &at[306]
#define PUB_SET_pub &at[307]
#define PUB_SET_pub_E &at[308]
#define PUB_SET_medline &at[309]
#define PUB_SET_medline_E &at[310]
#define PUB_SET_article &at[311]
#define PUB_SET_article_E &at[312]
#define PUB_SET_journal &at[313]
#define PUB_SET_journal_E &at[314]
#define PUB_SET_book &at[315]
#define PUB_SET_book_E &at[316]
#define PUB_SET_proc &at[317]
#define PUB_SET_proc_E &at[318]
#define PUB_SET_patent &at[319]
#define PUB_SET_patent_E &at[320]

#define PUB_EQUIV &at[302]
#define PUB_EQUIV_E &at[303]
