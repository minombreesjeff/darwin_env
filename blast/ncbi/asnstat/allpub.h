/***********************************************************************
*
**
*        Automatic header module from ASNTOOL
*
************************************************************************/

#ifndef _ASNTOOL_
#include <asn.h>
#endif

static char * asnfilename = "allpub.h64";
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

static AsnType atx[322] = {
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
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[46],NULL,0,NULL} ,
  {401, "Cit-art" ,1,0,0,0,0,1,0,0,NULL,&atx[13],&atx[78],0,&atx[123]} ,
  {0, "title" ,128,0,0,1,0,0,0,0,NULL,&atx[79],NULL,0,&atx[91]} ,
  {410, "Title" ,1,0,0,0,0,1,0,0,NULL,&atx[45],&atx[80],0,&atx[96]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[14],&atx[81],0,NULL} ,
  {0, "name" ,128,0,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[82]} ,
  {0, "tsub" ,128,1,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[83]} ,
  {0, "trans" ,128,2,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[84]} ,
  {0, "jta" ,128,3,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[85]} ,
  {0, "iso-jta" ,128,4,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[86]} ,
  {0, "ml-jta" ,128,5,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[87]} ,
  {0, "coden" ,128,6,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[88]} ,
  {0, "issn" ,128,7,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[89]} ,
  {0, "abr" ,128,8,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[90]} ,
  {0, "isbn" ,128,9,0,0,0,0,0,0,NULL,&atx[2],NULL,0,NULL} ,
  {0, "authors" ,128,1,0,1,0,0,0,0,NULL,&atx[92],NULL,0,&atx[121]} ,
  {427, "Auth-list" ,1,0,0,0,0,0,0,0,NULL,&atx[13],&atx[93],0,&atx[126]} ,
  {0, "names" ,128,0,0,0,0,0,0,0,NULL,&atx[14],&atx[94],0,&atx[120]} ,
  {0, "std" ,128,0,0,0,0,0,0,0,NULL,&atx[66],&atx[95],0,&atx[116]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[96],NULL,0,NULL} ,
  {411, "Author" ,1,0,0,0,0,1,0,0,NULL,&atx[13],&atx[97],0,&atx[170]} ,
  {0, "name" ,128,0,0,0,0,0,0,0,NULL,&atx[98],NULL,0,&atx[99]} ,
  {413, "Person-id" ,1,0,0,0,0,0,1,0,NULL,&atx[15],NULL,0,&atx[128]} ,
  {0, "level" ,128,1,0,1,0,0,0,0,NULL,&atx[42],&avnx[7],0,&atx[100]} ,
  {0, "role" ,128,2,0,1,0,0,0,0,NULL,&atx[42],&avnx[9],0,&atx[101]} ,
  {0, "affil" ,128,3,0,1,0,0,0,0,NULL,&atx[102],NULL,0,&atx[115]} ,
  {431, "Affil" ,1,0,0,0,0,0,0,0,NULL,&atx[14],&atx[103],0,&atx[140]} ,
  {0, "str" ,128,0,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[104]} ,
  {0, "std" ,128,1,0,0,0,0,0,0,NULL,&atx[13],&atx[105],0,NULL} ,
  {0, "affil" ,128,0,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[106]} ,
  {0, "div" ,128,1,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[107]} ,
  {0, "city" ,128,2,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[108]} ,
  {0, "sub" ,128,3,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[109]} ,
  {0, "country" ,128,4,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[110]} ,
  {0, "street" ,128,5,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[111]} ,
  {0, "email" ,128,6,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[112]} ,
  {0, "fax" ,128,7,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[113]} ,
  {0, "phone" ,128,8,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[114]} ,
  {0, "postal-code" ,128,9,0,1,0,0,0,0,NULL,&atx[2],NULL,0,NULL} ,
  {0, "is-corr" ,128,4,0,1,0,0,0,0,NULL,&atx[60],NULL,0,NULL} ,
  {0, "ml" ,128,1,0,0,0,0,0,0,NULL,&atx[66],&atx[117],0,&atx[118]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[2],NULL,0,NULL} ,
  {0, "str" ,128,2,0,0,0,0,0,0,NULL,&atx[66],&atx[119],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[2],NULL,0,NULL} ,
  {0, "affil" ,128,1,0,1,0,0,0,0,NULL,&atx[102],NULL,0,NULL} ,
  {0, "from" ,128,2,0,0,0,0,0,0,NULL,&atx[14],&atx[122],0,&atx[165]} ,
  {0, "journal" ,128,0,0,0,0,0,0,0,NULL,&atx[123],NULL,0,&atx[151]} ,
  {402, "Cit-jour" ,1,0,0,0,0,1,0,0,NULL,&atx[13],&atx[124],0,&atx[152]} ,
  {0, "title" ,128,0,0,0,0,0,0,0,NULL,&atx[79],NULL,0,&atx[125]} ,
  {0, "imp" ,128,1,0,0,0,0,0,0,NULL,&atx[126],NULL,0,NULL} ,
  {428, "Imprint" ,1,0,0,0,0,0,0,0,NULL,&atx[13],&atx[127],0,&atx[161]} ,
  {0, "date" ,128,0,0,0,0,0,0,0,NULL,&atx[128],NULL,0,&atx[129]} ,
  {414, "Date" ,1,0,0,0,0,0,1,0,NULL,&atx[0],NULL,0,&atx[184]} ,
  {0, "volume" ,128,1,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[130]} ,
  {0, "issue" ,128,2,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[131]} ,
  {0, "pages" ,128,3,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[132]} ,
  {0, "section" ,128,4,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[133]} ,
  {0, "pub" ,128,5,0,1,0,0,0,0,NULL,&atx[102],NULL,0,&atx[134]} ,
  {0, "cprt" ,128,6,0,1,0,0,0,0,NULL,&atx[128],NULL,0,&atx[135]} ,
  {0, "part-sup" ,128,7,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[136]} ,
  {0, "language" ,128,8,0,0,1,0,0,0,&avnx[13],&atx[2],NULL,0,&atx[137]} ,
  {0, "prepub" ,128,9,0,1,0,0,0,0,NULL,&atx[42],&avnx[14],0,&atx[138]} ,
  {0, "part-supi" ,128,10,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[139]} ,
  {0, "retract" ,128,11,0,1,0,0,0,0,NULL,&atx[140],NULL,0,&atx[143]} ,
  {432, "CitRetract" ,1,0,0,0,0,0,0,0,NULL,&atx[13],&atx[141],0,NULL} ,
  {0, "type" ,128,0,0,0,0,0,0,0,NULL,&atx[42],&avnx[17],0,&atx[142]} ,
  {0, "exp" ,128,1,0,1,0,0,0,0,NULL,&atx[2],NULL,0,NULL} ,
  {0, "pubstatus" ,128,12,0,1,0,0,0,0,NULL,&atx[144],NULL,0,&atx[145]} ,
  {424, "PubStatus" ,1,0,0,0,0,0,0,0,NULL,&atx[6],&avnx[21],0,&atx[148]} ,
  {0, "history" ,128,13,0,1,0,0,0,0,NULL,&atx[146],NULL,0,NULL} ,
  {426, "PubStatusDateSet" ,1,0,0,0,0,0,0,0,NULL,&atx[45],&atx[147],0,&atx[92]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[148],NULL,0,NULL} ,
  {425, "PubStatusDate" ,1,0,0,0,0,0,0,0,NULL,&atx[13],&atx[149],0,&atx[146]} ,
  {0, "pubstatus" ,128,0,0,0,0,0,0,0,NULL,&atx[144],NULL,0,&atx[150]} ,
  {0, "date" ,128,1,0,0,0,0,0,0,NULL,&atx[128],NULL,0,NULL} ,
  {0, "book" ,128,1,0,0,0,0,0,0,NULL,&atx[152],NULL,0,&atx[157]} ,
  {403, "Cit-book" ,1,0,0,0,0,1,0,0,NULL,&atx[13],&atx[153],0,&atx[185]} ,
  {0, "title" ,128,0,0,0,0,0,0,0,NULL,&atx[79],NULL,0,&atx[154]} ,
  {0, "coll" ,128,1,0,1,0,0,0,0,NULL,&atx[79],NULL,0,&atx[155]} ,
  {0, "authors" ,128,2,0,0,0,0,0,0,NULL,&atx[92],NULL,0,&atx[156]} ,
  {0, "imp" ,128,3,0,0,0,0,0,0,NULL,&atx[126],NULL,0,NULL} ,
  {0, "proc" ,128,2,0,0,0,0,0,0,NULL,&atx[158],NULL,0,NULL} ,
  {408, "Cit-proc" ,1,0,0,0,0,1,0,0,NULL,&atx[13],&atx[159],0,&atx[227]} ,
  {0, "book" ,128,0,0,0,0,0,0,0,NULL,&atx[152],NULL,0,&atx[160]} ,
  {0, "meet" ,128,1,0,0,0,0,0,0,NULL,&atx[161],NULL,0,NULL} ,
  {429, "Meeting" ,1,0,0,0,0,0,0,0,NULL,&atx[13],&atx[162],0,&atx[200]} ,
  {0, "number" ,128,0,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[163]} ,
  {0, "date" ,128,1,0,0,0,0,0,0,NULL,&atx[128],NULL,0,&atx[164]} ,
  {0, "place" ,128,2,0,1,0,0,0,0,NULL,&atx[102],NULL,0,NULL} ,
  {0, "ids" ,128,3,0,1,0,0,0,0,NULL,&atx[166],NULL,0,NULL} ,
  {423, "ArticleIdSet" ,1,0,0,0,0,0,0,0,NULL,&atx[45],&atx[167],0,&atx[144]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[168],NULL,0,NULL} ,
  {416, "ArticleId" ,1,0,0,0,0,0,0,0,NULL,&atx[14],&atx[169],0,&atx[172]} ,
  {0, "pubmed" ,128,0,0,0,0,0,0,0,NULL,&atx[170],NULL,0,&atx[171]} ,
  {412, "PubMedId" ,1,0,0,0,0,1,0,0,NULL,&atx[6],NULL,0,&atx[98]} ,
  {0, "medline" ,128,1,0,0,0,0,0,0,NULL,&atx[172],NULL,0,&atx[173]} ,
  {417, "MedlineUID" ,1,0,0,0,0,0,0,0,NULL,&atx[6],NULL,0,&atx[174]} ,
  {0, "doi" ,128,2,0,0,0,0,0,0,NULL,&atx[174],NULL,0,&atx[175]} ,
  {418, "DOI" ,1,0,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[176]} ,
  {0, "pii" ,128,3,0,0,0,0,0,0,NULL,&atx[176],NULL,0,&atx[177]} ,
  {419, "PII" ,1,0,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[178]} ,
  {0, "pmcid" ,128,4,0,0,0,0,0,0,NULL,&atx[178],NULL,0,&atx[179]} ,
  {420, "PmcID" ,1,0,0,0,0,0,0,0,NULL,&atx[6],NULL,0,&atx[180]} ,
  {0, "pmcpid" ,128,5,0,0,0,0,0,0,NULL,&atx[180],NULL,0,&atx[181]} ,
  {421, "PmcPid" ,1,0,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[182]} ,
  {0, "pmpid" ,128,6,0,0,0,0,0,0,NULL,&atx[182],NULL,0,&atx[183]} ,
  {422, "PmPid" ,1,0,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[166]} ,
  {0, "other" ,128,7,0,0,0,0,0,0,NULL,&atx[184],NULL,0,NULL} ,
  {415, "Dbtag" ,1,0,0,0,0,0,1,0,NULL,&atx[17],NULL,0,&atx[168]} ,
  {404, "Cit-pat" ,1,0,0,0,0,1,0,0,NULL,&atx[13],&atx[186],0,&atx[205]} ,
  {0, "title" ,128,0,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[187]} ,
  {0, "authors" ,128,1,0,0,0,0,0,0,NULL,&atx[92],NULL,0,&atx[188]} ,
  {0, "country" ,128,2,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[189]} ,
  {0, "doc-type" ,128,3,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[190]} ,
  {0, "number" ,128,4,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[191]} ,
  {0, "date-issue" ,128,5,0,1,0,0,0,0,NULL,&atx[128],NULL,0,&atx[192]} ,
  {0, "class" ,128,6,0,1,0,0,0,0,NULL,&atx[66],&atx[193],0,&atx[194]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[2],NULL,0,NULL} ,
  {0, "app-number" ,128,7,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[195]} ,
  {0, "app-date" ,128,8,0,1,0,0,0,0,NULL,&atx[128],NULL,0,&atx[196]} ,
  {0, "applicants" ,128,9,0,1,0,0,0,0,NULL,&atx[92],NULL,0,&atx[197]} ,
  {0, "assignees" ,128,10,0,1,0,0,0,0,NULL,&atx[92],NULL,0,&atx[198]} ,
  {0, "priority" ,128,11,0,1,0,0,0,0,NULL,&atx[66],&atx[199],0,&atx[204]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[200],NULL,0,NULL} ,
  {430, "Patent-priority" ,1,0,0,0,0,0,0,0,NULL,&atx[13],&atx[201],0,&atx[102]} ,
  {0, "country" ,128,0,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[202]} ,
  {0, "number" ,128,1,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[203]} ,
  {0, "date" ,128,2,0,0,0,0,0,0,NULL,&atx[128],NULL,0,NULL} ,
  {0, "abstract" ,128,12,0,1,0,0,0,0,NULL,&atx[2],NULL,0,NULL} ,
  {405, "Cit-let" ,1,0,0,0,0,1,0,0,NULL,&atx[13],&atx[206],0,&atx[209]} ,
  {0, "cit" ,128,0,0,0,0,0,0,0,NULL,&atx[152],NULL,0,&atx[207]} ,
  {0, "man-id" ,128,1,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[208]} ,
  {0, "type" ,128,2,0,1,0,0,0,0,NULL,&atx[42],&avnx[34],0,NULL} ,
  {406, "Id-pat" ,1,0,0,0,0,1,0,0,NULL,&atx[13],&atx[210],0,&atx[215]} ,
  {0, "country" ,128,0,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[211]} ,
  {0, "id" ,128,1,0,0,0,0,0,0,NULL,&atx[14],&atx[212],0,&atx[214]} ,
  {0, "number" ,128,0,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[213]} ,
  {0, "app-number" ,128,1,0,0,0,0,0,0,NULL,&atx[2],NULL,0,NULL} ,
  {0, "doc-type" ,128,2,0,1,0,0,0,0,NULL,&atx[2],NULL,0,NULL} ,
  {407, "Cit-gen" ,1,0,0,0,0,1,0,0,NULL,&atx[13],&atx[216],0,&atx[158]} ,
  {0, "cit" ,128,0,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[217]} ,
  {0, "authors" ,128,1,0,1,0,0,0,0,NULL,&atx[92],NULL,0,&atx[218]} ,
  {0, "muid" ,128,2,0,1,0,0,0,0,NULL,&atx[6],NULL,0,&atx[219]} ,
  {0, "journal" ,128,3,0,1,0,0,0,0,NULL,&atx[79],NULL,0,&atx[220]} ,
  {0, "volume" ,128,4,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[221]} ,
  {0, "issue" ,128,5,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[222]} ,
  {0, "pages" ,128,6,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[223]} ,
  {0, "date" ,128,7,0,1,0,0,0,0,NULL,&atx[128],NULL,0,&atx[224]} ,
  {0, "serial-number" ,128,8,0,1,0,0,0,0,NULL,&atx[6],NULL,0,&atx[225]} ,
  {0, "title" ,128,9,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[226]} ,
  {0, "pmid" ,128,10,0,1,0,0,0,0,NULL,&atx[170],NULL,0,NULL} ,
  {409, "Cit-sub" ,1,0,0,0,0,1,0,0,NULL,&atx[13],&atx[228],0,&atx[79]} ,
  {0, "authors" ,128,0,0,0,0,0,0,0,NULL,&atx[92],NULL,0,&atx[229]} ,
  {0, "imp" ,128,1,0,1,0,0,0,0,NULL,&atx[126],NULL,0,&atx[230]} ,
  {0, "medium" ,128,2,0,1,0,0,0,0,NULL,&atx[42],&avnx[37],0,&atx[231]} ,
  {0, "date" ,128,3,0,1,0,0,0,0,NULL,&atx[128],NULL,0,&atx[232]} ,
  {0, "descr" ,128,4,0,1,0,0,0,0,NULL,&atx[2],NULL,0,NULL} ,
  {401, "Medline-entry" ,1,0,0,0,0,1,0,0,NULL,&atx[13],&atx[234],0,&atx[258]} ,
  {0, "uid" ,128,0,0,1,0,0,0,0,NULL,&atx[6],NULL,0,&atx[235]} ,
  {0, "em" ,128,1,0,0,0,0,0,0,NULL,&atx[236],NULL,0,&atx[237]} ,
  {405, "Date" ,1,0,0,0,0,0,1,0,NULL,&atx[0],NULL,0,&atx[242]} ,
  {0, "cit" ,128,2,0,0,0,0,0,0,NULL,&atx[238],NULL,0,&atx[239]} ,
  {403, "Cit-art" ,1,0,0,0,0,0,1,0,NULL,&atx[77],NULL,0,&atx[266]} ,
  {0, "abstract" ,128,3,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[240]} ,
  {0, "mesh" ,128,4,0,1,0,0,0,0,NULL,&atx[45],&atx[241],0,&atx[250]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[242],NULL,0,NULL} ,
  {406, "Medline-mesh" ,1,0,0,0,0,0,0,0,NULL,&atx[13],&atx[243],0,&atx[252]} ,
  {0, "mp" ,128,0,0,0,1,0,0,0,&avnx[42],&atx[60],NULL,0,&atx[244]} ,
  {0, "term" ,128,1,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[245]} ,
  {0, "qual" ,128,2,0,1,0,0,0,0,NULL,&atx[45],&atx[246],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[247],NULL,0,NULL} ,
  {409, "Medline-qual" ,1,0,0,0,0,0,0,0,NULL,&atx[13],&atx[248],0,&atx[276]} ,
  {0, "mp" ,128,0,0,0,1,0,0,0,&avnx[43],&atx[60],NULL,0,&atx[249]} ,
  {0, "subh" ,128,1,0,0,0,0,0,0,NULL,&atx[2],NULL,0,NULL} ,
  {0, "substance" ,128,5,0,1,0,0,0,0,NULL,&atx[45],&atx[251],0,&atx[256]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[252],NULL,0,NULL} ,
  {407, "Medline-rn" ,1,0,0,0,0,0,0,0,NULL,&atx[13],&atx[253],0,&atx[271]} ,
  {0, "type" ,128,0,0,0,0,0,0,0,NULL,&atx[42],&avnx[44],0,&atx[254]} ,
  {0, "cit" ,128,1,0,1,0,0,0,0,NULL,&atx[2],NULL,0,&atx[255]} ,
  {0, "name" ,128,2,0,0,0,0,0,0,NULL,&atx[2],NULL,0,NULL} ,
  {0, "xref" ,128,6,0,1,0,0,0,0,NULL,&atx[45],&atx[257],0,&atx[261]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[258],NULL,0,NULL} ,
  {402, "Medline-si" ,1,0,0,0,0,1,0,0,NULL,&atx[13],&atx[259],0,&atx[238]} ,
  {0, "type" ,128,0,0,0,0,0,0,0,NULL,&atx[42],&avnx[47],0,&atx[260]} ,
  {0, "cit" ,128,1,0,1,0,0,0,0,NULL,&atx[2],NULL,0,NULL} ,
  {0, "idnum" ,128,7,0,1,0,0,0,0,NULL,&atx[45],&atx[262],0,&atx[263]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[2],NULL,0,NULL} ,
  {0, "gene" ,128,8,0,1,0,0,0,0,NULL,&atx[45],&atx[264],0,&atx[265]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[2],NULL,0,NULL} ,
  {0, "pmid" ,128,9,0,1,0,0,0,0,NULL,&atx[266],NULL,0,&atx[267]} ,
  {404, "PubMedId" ,1,0,0,0,0,0,1,0,NULL,&atx[170],NULL,0,&atx[236]} ,
  {0, "pub-type" ,128,10,0,1,0,0,0,0,NULL,&atx[45],&atx[268],0,&atx[269]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[2],NULL,0,NULL} ,
  {0, "mlfield" ,128,11,0,1,0,0,0,0,NULL,&atx[45],&atx[270],0,&atx[279]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[271],NULL,0,NULL} ,
  {408, "Medline-field" ,1,0,0,0,0,0,0,0,NULL,&atx[13],&atx[272],0,&atx[247]} ,
  {0, "type" ,128,0,0,0,0,0,0,0,NULL,&atx[6],&avnx[61],0,&atx[273]} ,
  {0, "str" ,128,1,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[274]} ,
  {0, "ids" ,128,2,0,1,0,0,0,0,NULL,&atx[66],&atx[275],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[276],NULL,0,NULL} ,
  {410, "DocRef" ,1,0,0,0,0,0,0,0,NULL,&atx[13],&atx[277],0,NULL} ,
  {0, "type" ,128,0,0,0,0,0,0,0,NULL,&atx[6],&avnx[64],0,&atx[278]} ,
  {0, "uid" ,128,1,0,0,0,0,0,0,NULL,&atx[6],NULL,0,NULL} ,
  {0, "status" ,128,12,0,0,1,0,0,0,&avnx[70],&atx[6],&avnx[67],0,NULL} ,
  {401, "Pub" ,1,0,0,0,0,1,0,0,NULL,&atx[14],&atx[281],0,&atx[307]} ,
  {0, "gen" ,128,0,0,0,0,0,0,0,NULL,&atx[282],NULL,0,&atx[283]} ,
  {411, "Cit-gen" ,1,0,0,0,0,0,1,0,NULL,&atx[215],NULL,0,&atx[301]} ,
  {0, "sub" ,128,1,0,0,0,0,0,0,NULL,&atx[284],NULL,0,&atx[285]} ,
  {413, "Cit-sub" ,1,0,0,0,0,0,1,0,NULL,&atx[227],NULL,0,&atx[306]} ,
  {0, "medline" ,128,2,0,0,0,0,0,0,NULL,&atx[286],NULL,0,&atx[287]} ,
  {404, "Medline-entry" ,1,0,0,0,0,0,1,0,NULL,&atx[233],NULL,0,&atx[289]} ,
  {0, "muid" ,128,3,0,0,0,0,0,0,NULL,&atx[6],NULL,0,&atx[288]} ,
  {0, "article" ,128,4,0,0,0,0,0,0,NULL,&atx[289],NULL,0,&atx[290]} ,
  {405, "Cit-art" ,1,0,0,0,0,0,1,0,NULL,&atx[77],NULL,0,&atx[291]} ,
  {0, "journal" ,128,5,0,0,0,0,0,0,NULL,&atx[291],NULL,0,&atx[292]} ,
  {406, "Cit-jour" ,1,0,0,0,0,0,1,0,NULL,&atx[123],NULL,0,&atx[293]} ,
  {0, "book" ,128,6,0,0,0,0,0,0,NULL,&atx[293],NULL,0,&atx[294]} ,
  {407, "Cit-book" ,1,0,0,0,0,0,1,0,NULL,&atx[152],NULL,0,&atx[295]} ,
  {0, "proc" ,128,7,0,0,0,0,0,0,NULL,&atx[295],NULL,0,&atx[296]} ,
  {408, "Cit-proc" ,1,0,0,0,0,0,1,0,NULL,&atx[158],NULL,0,&atx[297]} ,
  {0, "patent" ,128,8,0,0,0,0,0,0,NULL,&atx[297],NULL,0,&atx[298]} ,
  {409, "Cit-pat" ,1,0,0,0,0,0,1,0,NULL,&atx[185],NULL,0,&atx[299]} ,
  {0, "pat-id" ,128,9,0,0,0,0,0,0,NULL,&atx[299],NULL,0,&atx[300]} ,
  {410, "Id-pat" ,1,0,0,0,0,0,1,0,NULL,&atx[209],NULL,0,&atx[282]} ,
  {0, "man" ,128,10,0,0,0,0,0,0,NULL,&atx[301],NULL,0,&atx[302]} ,
  {412, "Cit-let" ,1,0,0,0,0,0,1,0,NULL,&atx[205],NULL,0,&atx[284]} ,
  {0, "equiv" ,128,11,0,0,0,0,0,0,NULL,&atx[303],NULL,0,&atx[305]} ,
  {403, "Pub-equiv" ,1,0,0,0,0,1,0,0,NULL,&atx[45],&atx[304],0,&atx[286]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[280],NULL,0,NULL} ,
  {0, "pmid" ,128,12,0,0,0,0,0,0,NULL,&atx[306],NULL,0,NULL} ,
  {414, "PubMedId" ,1,0,0,0,0,0,1,0,NULL,&atx[170],NULL,0,NULL} ,
  {402, "Pub-set" ,1,0,0,0,0,1,0,0,NULL,&atx[14],&atx[308],0,&atx[303]} ,
  {0, "pub" ,128,0,0,0,0,0,0,0,NULL,&atx[45],&atx[309],0,&atx[310]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[280],NULL,0,NULL} ,
  {0, "medline" ,128,1,0,0,0,0,0,0,NULL,&atx[45],&atx[311],0,&atx[312]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[286],NULL,0,NULL} ,
  {0, "article" ,128,2,0,0,0,0,0,0,NULL,&atx[45],&atx[313],0,&atx[314]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[289],NULL,0,NULL} ,
  {0, "journal" ,128,3,0,0,0,0,0,0,NULL,&atx[45],&atx[315],0,&atx[316]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[291],NULL,0,NULL} ,
  {0, "book" ,128,4,0,0,0,0,0,0,NULL,&atx[45],&atx[317],0,&atx[318]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[293],NULL,0,NULL} ,
  {0, "proc" ,128,5,0,0,0,0,0,0,NULL,&atx[45],&atx[319],0,&atx[320]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[295],NULL,0,NULL} ,
  {0, "patent" ,128,6,0,0,0,0,0,0,NULL,&atx[45],&atx[321],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[297],NULL,0,NULL} };

static AsnModule ampx[4] = {
  { "NCBI-General" , "allpub.h64",&atx[0],NULL,&ampx[1],0,0} ,
  { "NCBI-Biblio" , NULL,&atx[77],NULL,&ampx[2],0,0} ,
  { "NCBI-Medline" , NULL,&atx[233],NULL,&ampx[3],0,0} ,
  { "NCBI-Pub" , NULL,&atx[280],NULL,NULL,0,0} };

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


/**************************************************
*
*    Defines for Module NCBI-Biblio
*
**************************************************/

#define CIT_ART &at[77]
#define CIT_ART_title &at[78]
#define CIT_ART_authors &at[91]
#define CIT_ART_from &at[121]
#define CIT_ART_from_journal &at[122]
#define CIT_ART_from_book &at[151]
#define CIT_ART_from_proc &at[157]
#define CIT_ART_ids &at[165]

#define CIT_JOUR &at[123]
#define CIT_JOUR_title &at[124]
#define CIT_JOUR_imp &at[125]

#define CIT_BOOK &at[152]
#define CIT_BOOK_title &at[153]
#define CIT_BOOK_coll &at[154]
#define CIT_BOOK_authors &at[155]
#define CIT_BOOK_imp &at[156]

#define CIT_PAT &at[185]
#define CIT_PAT_title &at[186]
#define CIT_PAT_authors &at[187]
#define CIT_PAT_country &at[188]
#define CIT_PAT_doc_type &at[189]
#define CIT_PAT_number &at[190]
#define CIT_PAT_date_issue &at[191]
#define CIT_PAT_class &at[192]
#define CIT_PAT_class_E &at[193]
#define CIT_PAT_app_number &at[194]
#define CIT_PAT_app_date &at[195]
#define CIT_PAT_applicants &at[196]
#define CIT_PAT_assignees &at[197]
#define CIT_PAT_priority &at[198]
#define CIT_PAT_priority_E &at[199]
#define CIT_PAT_abstract &at[204]

#define CIT_LET &at[205]
#define CIT_LET_cit &at[206]
#define CIT_LET_man_id &at[207]
#define CIT_LET_type &at[208]

#define ID_PAT &at[209]
#define ID_PAT_country &at[210]
#define ID_PAT_id &at[211]
#define ID_PAT_id_number &at[212]
#define ID_PAT_id_app_number &at[213]
#define ID_PAT_doc_type &at[214]

#define CIT_GEN &at[215]
#define CIT_GEN_cit &at[216]
#define CIT_GEN_authors &at[217]
#define CIT_GEN_muid &at[218]
#define CIT_GEN_journal &at[219]
#define CIT_GEN_volume &at[220]
#define CIT_GEN_issue &at[221]
#define CIT_GEN_pages &at[222]
#define CIT_GEN_date &at[223]
#define CIT_GEN_serial_number &at[224]
#define CIT_GEN_title &at[225]
#define CIT_GEN_pmid &at[226]

#define CIT_PROC &at[158]
#define CIT_PROC_book &at[159]
#define CIT_PROC_meet &at[160]

#define CIT_SUB &at[227]
#define CIT_SUB_authors &at[228]
#define CIT_SUB_imp &at[229]
#define CIT_SUB_medium &at[230]
#define CIT_SUB_date &at[231]
#define CIT_SUB_descr &at[232]

#define TITLE &at[79]
#define TITLE_E &at[80]
#define TITLE_E_name &at[81]
#define TITLE_E_tsub &at[82]
#define TITLE_E_trans &at[83]
#define TITLE_E_jta &at[84]
#define TITLE_E_iso_jta &at[85]
#define TITLE_E_ml_jta &at[86]
#define TITLE_E_coden &at[87]
#define TITLE_E_issn &at[88]
#define TITLE_E_abr &at[89]
#define TITLE_E_isbn &at[90]

#define AUTHOR &at[96]
#define AUTHOR_name &at[97]
#define AUTHOR_level &at[99]
#define AUTHOR_role &at[100]
#define AUTHOR_affil &at[101]
#define AUTHOR_is_corr &at[115]

#define PUBMEDID &at[170]

#define ARTICLEID &at[168]
#define ARTICLEID_pubmed &at[169]
#define ARTICLEID_medline &at[171]
#define ARTICLEID_doi &at[173]
#define ARTICLEID_pii &at[175]
#define ARTICLEID_pmcid &at[177]
#define ARTICLEID_pmcpid &at[179]
#define ARTICLEID_pmpid &at[181]
#define ARTICLEID_other &at[183]

#define MEDLINEUID &at[172]

#define DOI &at[174]

#define PII &at[176]

#define PMCID &at[178]

#define PMCPID &at[180]

#define PMPID &at[182]

#define ARTICLEIDSET &at[166]
#define ARTICLEIDSET_E &at[167]

#define PUBSTATUS &at[144]

#define PUBSTATUSDATE &at[148]
#define PUBSTATUSDATE_pubstatus &at[149]
#define PUBSTATUSDATE_date &at[150]

#define PUBSTATUSDATESET &at[146]
#define PUBSTATUSDATESET_E &at[147]

#define AUTH_LIST &at[92]
#define AUTH_LIST_names &at[93]
#define AUTH_LIST_names_std &at[94]
#define AUTH_LIST_names_std_E &at[95]
#define AUTH_LIST_names_ml &at[116]
#define AUTH_LIST_names_ml_E &at[117]
#define AUTH_LIST_names_str &at[118]
#define AUTH_LIST_names_str_E &at[119]
#define AUTH_LIST_affil &at[120]

#define IMPRINT &at[126]
#define IMPRINT_date &at[127]
#define IMPRINT_volume &at[129]
#define IMPRINT_issue &at[130]
#define IMPRINT_pages &at[131]
#define IMPRINT_section &at[132]
#define IMPRINT_pub &at[133]
#define IMPRINT_cprt &at[134]
#define IMPRINT_part_sup &at[135]
#define IMPRINT_language &at[136]
#define IMPRINT_prepub &at[137]
#define IMPRINT_part_supi &at[138]
#define IMPRINT_retract &at[139]
#define IMPRINT_pubstatus &at[143]
#define IMPRINT_history &at[145]

#define MEETING &at[161]
#define MEETING_number &at[162]
#define MEETING_date &at[163]
#define MEETING_place &at[164]

#define PATENT_PRIORITY &at[200]
#define PATENT_PRIORITY_country &at[201]
#define PATENT_PRIORITY_number &at[202]
#define PATENT_PRIORITY_date &at[203]

#define AFFIL &at[102]
#define AFFIL_str &at[103]
#define AFFIL_std &at[104]
#define AFFIL_std_affil &at[105]
#define AFFIL_std_div &at[106]
#define AFFIL_std_city &at[107]
#define AFFIL_std_sub &at[108]
#define AFFIL_std_country &at[109]
#define AFFIL_std_street &at[110]
#define AFFIL_std_email &at[111]
#define AFFIL_std_fax &at[112]
#define AFFIL_std_phone &at[113]
#define AFFIL_std_postal_code &at[114]

#define CITRETRACT &at[140]
#define CITRETRACT_type &at[141]
#define CITRETRACT_exp &at[142]


/**************************************************
*
*    Defines for Module NCBI-Medline
*
**************************************************/

#define MEDLINE_ENTRY &at[233]
#define MEDLINE_ENTRY_uid &at[234]
#define MEDLINE_ENTRY_em &at[235]
#define MEDLINE_ENTRY_cit &at[237]
#define MEDLINE_ENTRY_abstract &at[239]
#define MEDLINE_ENTRY_mesh &at[240]
#define MEDLINE_ENTRY_mesh_E &at[241]
#define MEDLINE_ENTRY_substance &at[250]
#define MEDLINE_ENTRY_substance_E &at[251]
#define MEDLINE_ENTRY_xref &at[256]
#define MEDLINE_ENTRY_xref_E &at[257]
#define MEDLINE_ENTRY_idnum &at[261]
#define MEDLINE_ENTRY_idnum_E &at[262]
#define MEDLINE_ENTRY_gene &at[263]
#define MEDLINE_ENTRY_gene_E &at[264]
#define MEDLINE_ENTRY_pmid &at[265]
#define MEDLINE_ENTRY_pub_type &at[267]
#define MEDLINE_ENTRY_pub_type_E &at[268]
#define MEDLINE_ENTRY_mlfield &at[269]
#define MEDLINE_ENTRY_mlfield_E &at[270]
#define MEDLINE_ENTRY_status &at[279]

#define MEDLINE_SI &at[258]
#define MEDLINE_SI_type &at[259]
#define MEDLINE_SI_cit &at[260]

#define MEDLINE_MESH &at[242]
#define MEDLINE_MESH_mp &at[243]
#define MEDLINE_MESH_term &at[244]
#define MEDLINE_MESH_qual &at[245]
#define MEDLINE_MESH_qual_E &at[246]

#define MEDLINE_RN &at[252]
#define MEDLINE_RN_type &at[253]
#define MEDLINE_RN_cit &at[254]
#define MEDLINE_RN_name &at[255]

#define MEDLINE_FIELD &at[271]
#define MEDLINE_FIELD_type &at[272]
#define MEDLINE_FIELD_str &at[273]
#define MEDLINE_FIELD_ids &at[274]
#define MEDLINE_FIELD_ids_E &at[275]

#define MEDLINE_QUAL &at[247]
#define MEDLINE_QUAL_mp &at[248]
#define MEDLINE_QUAL_subh &at[249]

#define DOCREF &at[276]
#define DOCREF_type &at[277]
#define DOCREF_uid &at[278]


/**************************************************
*
*    Defines for Module NCBI-Pub
*
**************************************************/

#define PUB &at[280]
#define PUB_gen &at[281]
#define PUB_sub &at[283]
#define PUB_medline &at[285]
#define PUB_muid &at[287]
#define PUB_article &at[288]
#define PUB_journal &at[290]
#define PUB_book &at[292]
#define PUB_proc &at[294]
#define PUB_patent &at[296]
#define PUB_pat_id &at[298]
#define PUB_man &at[300]
#define PUB_equiv &at[302]
#define PUB_pmid &at[305]

#define PUB_SET &at[307]
#define PUB_SET_pub &at[308]
#define PUB_SET_pub_E &at[309]
#define PUB_SET_medline &at[310]
#define PUB_SET_medline_E &at[311]
#define PUB_SET_article &at[312]
#define PUB_SET_article_E &at[313]
#define PUB_SET_journal &at[314]
#define PUB_SET_journal_E &at[315]
#define PUB_SET_book &at[316]
#define PUB_SET_book_E &at[317]
#define PUB_SET_proc &at[318]
#define PUB_SET_proc_E &at[319]
#define PUB_SET_patent &at[320]
#define PUB_SET_patent_E &at[321]

#define PUB_EQUIV &at[303]
#define PUB_EQUIV_E &at[304]
