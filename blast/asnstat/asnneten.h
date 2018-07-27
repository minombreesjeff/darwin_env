/***********************************************************************
*
**
*        Automatic header module from ASNTOOL
*
************************************************************************/

#ifndef _ASNTOOL_
#include <asn.h>
#endif

static char * asnfilename = "asnneten.h60";
static AsnValxNode avnx[38] = {
    {20,"medline" ,0,0.0,&avnx[1] } ,
    {20,"aa" ,1,0.0,&avnx[2] } ,
    {20,"na" ,2,0.0,&avnx[3] } ,
    {20,"st" ,3,0.0,&avnx[4] } ,
    {20,"genome" ,4,0.0,NULL } ,
    {20,"lparen" ,1,0.0,&avnx[6] } ,
    {20,"rparen" ,2,0.0,&avnx[7] } ,
    {20,"andsymbl" ,3,0.0,&avnx[8] } ,
    {20,"orsymbl" ,4,0.0,&avnx[9] } ,
    {20,"butnotsymbl" ,5,0.0,NULL } ,
    {20,"word" ,0,0.0,&avnx[11] } ,
    {20,"mesh" ,1,0.0,&avnx[12] } ,
    {20,"keyword" ,2,0.0,&avnx[13] } ,
    {20,"author" ,3,0.0,&avnx[14] } ,
    {20,"journal" ,4,0.0,&avnx[15] } ,
    {20,"org" ,5,0.0,&avnx[16] } ,
    {20,"accn" ,6,0.0,&avnx[17] } ,
    {20,"gene" ,7,0.0,&avnx[18] } ,
    {20,"prot" ,8,0.0,&avnx[19] } ,
    {20,"ecno" ,9,0.0,&avnx[20] } ,
    {20,"hierarchy" ,10,0.0,&avnx[21] } ,
    {20,"pubdate" ,11,0.0,&avnx[22] } ,
    {20,"fkey" ,12,0.0,&avnx[23] } ,
    {20,"prop" ,13,0.0,&avnx[24] } ,
    {20,"subs" ,14,0.0,&avnx[25] } ,
    {20,"mloc" ,15,0.0,NULL } ,
    {2,NULL,0,0.0,NULL } ,
    {20,"fetch-from-id" ,-2,0.0,&avnx[28] } ,
    {20,"unmatched" ,-1,0.0,&avnx[29] } ,
    {20,"entry" ,0,0.0,&avnx[30] } ,
    {20,"bioseq" ,1,0.0,&avnx[31] } ,
    {20,"bioseq-set" ,2,0.0,&avnx[32] } ,
    {20,"nuc-prot" ,3,0.0,&avnx[33] } ,
    {20,"pub-set" ,4,0.0,NULL } ,
    {3,NULL,0,0.0,NULL } ,
    {2,NULL,0,0.0,NULL } ,
    {2,NULL,0,0.0,NULL } ,
    {2,NULL,0,0.0,NULL } };

static AsnType atx[288] = {
  {401, "Seq-entry" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[1]} ,
  {402, "Seq-id" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[2]} ,
  {403, "Medline-entry" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[3]} ,
  {404, "Link-set" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[4]} ,
  {405, "Biostruc" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[5]} ,
  {406, "Biostruc-annot-set" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[6]} ,
  {407, "Bioseq" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[7]} ,
  {408, "Date" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[8]} ,
  {409, "Cdrom-inf" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[9]} ,
  {410, "Docsum" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[10]} ,
  {411, "Entrez-request" ,1,0,0,0,0,0,0,0,NULL,&atx[42],&atx[11],0,&atx[18]} ,
  {0, "init" ,128,0,0,0,0,0,0,0,NULL,&atx[14],&atx[12],0,&atx[15]} ,
  {0, "version" ,128,0,0,1,0,0,0,0,NULL,&atx[13],NULL,0,NULL} ,
  {323, "VisibleString" ,0,26,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {311, "SEQUENCE" ,0,16,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "maxlinks" ,128,1,0,0,0,0,0,0,NULL,&atx[16],NULL,0,&atx[17]} ,
  {305, "NULL" ,0,5,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "eval" ,128,2,0,0,0,0,0,0,NULL,&atx[18],NULL,0,&atx[53]} ,
  {412, "Entrez-termget" ,1,0,0,0,0,0,0,0,NULL,&atx[14],&atx[19],0,&atx[54]} ,
  {0, "max" ,128,0,0,1,0,0,0,0,NULL,&atx[20],NULL,0,&atx[21]} ,
  {302, "INTEGER" ,0,2,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "cls" ,128,1,0,0,0,0,0,0,NULL,&atx[22],NULL,0,&atx[24]} ,
  {429, "Entrez-class" ,1,0,0,0,0,0,0,0,NULL,&atx[23],&avnx[0],0,&atx[25]} ,
  {310, "ENUMERATED" ,0,10,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "terms" ,128,2,0,0,0,0,0,0,NULL,&atx[25],NULL,0,&atx[44]} ,
  {430, "Entrez-term-list" ,1,0,0,0,0,0,0,0,NULL,&atx[43],&atx[26],0,&atx[45]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[42],&atx[27],0,NULL} ,
  {0, "operator" ,128,0,0,0,0,0,0,0,NULL,&atx[28],NULL,0,&atx[29]} ,
  {433, "Entrez-operator" ,1,0,0,0,0,0,0,0,NULL,&atx[23],&avnx[5],0,&atx[30]} ,
  {0, "sp-operand" ,128,1,0,0,0,0,0,0,NULL,&atx[30],NULL,0,&atx[36]} ,
  {434, "Special-operand" ,1,0,0,0,0,0,0,0,NULL,&atx[14],&atx[31],0,&atx[37]} ,
  {0, "term" ,128,0,0,0,0,0,0,0,NULL,&atx[13],NULL,0,&atx[32]} ,
  {0, "fld" ,128,1,0,0,0,0,0,0,NULL,&atx[33],NULL,0,&atx[34]} ,
  {427, "Entrez-field" ,1,0,0,0,0,0,0,0,NULL,&atx[20],&avnx[10],0,&atx[162]} ,
  {0, "type" ,128,2,0,0,0,0,0,0,NULL,&atx[22],NULL,0,&atx[35]} ,
  {0, "high-range" ,128,3,0,1,0,0,0,0,NULL,&atx[13],NULL,0,NULL} ,
  {0, "tot-operand" ,128,2,0,0,0,0,0,0,NULL,&atx[37],NULL,0,NULL} ,
  {435, "Total-operand" ,1,0,0,0,0,0,0,0,NULL,&atx[14],&atx[38],0,&atx[168]} ,
  {0, "term" ,128,0,0,0,0,0,0,0,NULL,&atx[13],NULL,0,&atx[39]} ,
  {0, "fld" ,128,1,0,0,0,0,0,0,NULL,&atx[33],NULL,0,&atx[40]} ,
  {0, "type" ,128,2,0,0,0,0,0,0,NULL,&atx[22],NULL,0,&atx[41]} ,
  {0, "high-range" ,128,3,0,1,0,0,0,0,NULL,&atx[13],NULL,0,NULL} ,
  {315, "CHOICE" ,0,-1,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {312, "SEQUENCE OF" ,0,16,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "date-constraints" ,128,3,0,1,0,0,0,0,NULL,&atx[45],NULL,0,NULL} ,
  {431, "Date-constraints" ,1,0,0,0,0,0,0,0,NULL,&atx[14],&atx[46],0,&atx[49]} ,
  {0, "count" ,128,0,0,0,0,0,0,0,NULL,&atx[20],NULL,0,&atx[47]} ,
  {0, "date-vec" ,128,1,0,0,0,0,0,0,NULL,&atx[43],&atx[48],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[49],NULL,0,NULL} ,
  {432, "Date-vector" ,1,0,0,0,0,0,0,0,NULL,&atx[14],&atx[50],0,&atx[28]} ,
  {0, "begin-date" ,128,0,0,1,0,0,0,0,NULL,&atx[7],NULL,0,&atx[51]} ,
  {0, "end-date" ,128,1,0,1,0,0,0,0,NULL,&atx[7],NULL,0,&atx[52]} ,
  {0, "field-abbr" ,128,2,0,0,0,0,0,0,NULL,&atx[13],NULL,0,NULL} ,
  {0, "docsum" ,128,3,0,0,0,0,0,0,NULL,&atx[54],NULL,0,&atx[64]} ,
  {413, "Entrez-docget" ,1,0,0,0,0,0,0,0,NULL,&atx[14],&atx[55],0,&atx[65]} ,
  {0, "class" ,128,0,0,0,0,0,0,0,NULL,&atx[22],NULL,0,&atx[56]} ,
  {0, "mark-missing" ,128,1,0,1,0,0,0,0,NULL,&atx[57],NULL,0,&atx[58]} ,
  {301, "BOOLEAN" ,0,1,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "ids" ,128,2,0,0,0,0,0,0,NULL,&atx[59],NULL,0,&atx[63]} ,
  {426, "Entrez-ids" ,1,0,0,0,0,0,0,0,NULL,&atx[14],&atx[60],0,&atx[33]} ,
  {0, "numid" ,128,0,0,0,0,0,0,0,NULL,&atx[20],NULL,0,&atx[61]} ,
  {0, "ids" ,128,1,0,0,0,0,0,0,NULL,&atx[43],&atx[62],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[20],NULL,0,NULL} ,
  {0, "defer-count" ,128,3,0,1,0,0,0,0,NULL,&atx[20],NULL,0,NULL} ,
  {0, "linkuidlist" ,128,4,0,0,0,0,0,0,NULL,&atx[65],NULL,0,&atx[73]} ,
  {414, "Link-setget" ,1,0,0,0,0,0,0,0,NULL,&atx[14],&atx[66],0,&atx[75]} ,
  {0, "max" ,128,0,0,1,0,0,0,0,NULL,&atx[20],NULL,0,&atx[67]} ,
  {0, "link-to-cls" ,128,1,0,0,0,0,0,0,NULL,&atx[22],NULL,0,&atx[68]} ,
  {0, "query-cls" ,128,2,0,0,0,0,0,0,NULL,&atx[22],NULL,0,&atx[69]} ,
  {0, "mark-missing" ,128,3,0,0,1,0,0,0,&avnx[26],&atx[57],NULL,0,&atx[70]} ,
  {0, "query-size" ,128,4,0,0,0,0,0,0,NULL,&atx[20],NULL,0,&atx[71]} ,
  {0, "query" ,128,5,0,0,0,0,0,0,NULL,&atx[43],&atx[72],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[20],NULL,0,NULL} ,
  {0, "uidlinks" ,128,5,0,0,0,0,0,0,NULL,&atx[65],NULL,0,&atx[74]} ,
  {0, "byterm" ,128,6,0,0,0,0,0,0,NULL,&atx[75],NULL,0,&atx[80]} ,
  {415, "Entrez-term-by-term" ,1,0,0,0,0,0,0,0,NULL,&atx[14],&atx[76],0,&atx[81]} ,
  {0, "type" ,128,0,0,0,0,0,0,0,NULL,&atx[22],NULL,0,&atx[77]} ,
  {0, "fld" ,128,1,0,0,0,0,0,0,NULL,&atx[33],NULL,0,&atx[78]} ,
  {0, "term" ,128,2,0,0,0,0,0,0,NULL,&atx[13],NULL,0,&atx[79]} ,
  {0, "num-terms" ,128,3,0,0,0,0,0,0,NULL,&atx[20],NULL,0,NULL} ,
  {0, "bypage" ,128,7,0,0,0,0,0,0,NULL,&atx[81],NULL,0,&atx[86]} ,
  {416, "Entrez-term-by-page" ,1,0,0,0,0,0,0,0,NULL,&atx[14],&atx[82],0,&atx[87]} ,
  {0, "type" ,128,0,0,0,0,0,0,0,NULL,&atx[22],NULL,0,&atx[83]} ,
  {0, "fld" ,128,1,0,0,0,0,0,0,NULL,&atx[33],NULL,0,&atx[84]} ,
  {0, "page" ,128,2,0,0,0,0,0,0,NULL,&atx[20],NULL,0,&atx[85]} ,
  {0, "num-pages" ,128,3,0,0,0,0,0,0,NULL,&atx[20],NULL,0,NULL} ,
  {0, "findterm" ,128,8,0,0,0,0,0,0,NULL,&atx[87],NULL,0,&atx[91]} ,
  {417, "Term-lookup" ,1,0,0,0,0,0,0,0,NULL,&atx[14],&atx[88],0,&atx[93]} ,
  {0, "type" ,128,0,0,0,0,0,0,0,NULL,&atx[22],NULL,0,&atx[89]} ,
  {0, "fld" ,128,1,0,0,0,0,0,0,NULL,&atx[33],NULL,0,&atx[90]} ,
  {0, "term" ,128,2,0,0,0,0,0,0,NULL,&atx[13],NULL,0,NULL} ,
  {0, "fini" ,128,9,0,0,0,0,0,0,NULL,&atx[16],NULL,0,&atx[92]} ,
  {0, "createnamed" ,128,10,0,0,0,0,0,0,NULL,&atx[93],NULL,0,&atx[98]} ,
  {418, "Entrez-named-list" ,1,0,0,0,0,0,0,0,NULL,&atx[14],&atx[94],0,&atx[100]} ,
  {0, "term" ,128,0,0,0,0,0,0,0,NULL,&atx[13],NULL,0,&atx[95]} ,
  {0, "type" ,128,1,0,0,0,0,0,0,NULL,&atx[22],NULL,0,&atx[96]} ,
  {0, "fld" ,128,2,0,0,0,0,0,0,NULL,&atx[33],NULL,0,&atx[97]} ,
  {0, "uids" ,128,3,0,0,0,0,0,0,NULL,&atx[59],NULL,0,NULL} ,
  {0, "getmle" ,128,11,0,0,0,0,0,0,NULL,&atx[54],NULL,0,&atx[99]} ,
  {0, "getseq" ,128,12,0,0,0,0,0,0,NULL,&atx[100],NULL,0,&atx[104]} ,
  {419, "Entrez-seqget" ,1,0,0,0,0,0,0,0,NULL,&atx[14],&atx[101],0,&atx[106]} ,
  {0, "retype" ,128,0,0,0,1,0,0,0,&avnx[34],&atx[23],&avnx[27],0,&atx[102]} ,
  {0, "mark-missing" ,128,1,0,1,0,0,0,0,NULL,&atx[57],NULL,0,&atx[103]} ,
  {0, "ids" ,128,2,0,0,0,0,0,0,NULL,&atx[59],NULL,0,NULL} ,
  {0, "evalX" ,128,13,0,0,0,0,0,0,NULL,&atx[18],NULL,0,&atx[105]} ,
  {0, "createnamedX" ,128,14,0,0,0,0,0,0,NULL,&atx[106],NULL,0,&atx[112]} ,
  {420, "Entrez-named-listX" ,1,0,0,0,0,0,0,0,NULL,&atx[14],&atx[107],0,&atx[118]} ,
  {0, "term" ,128,0,0,0,0,0,0,0,NULL,&atx[13],NULL,0,&atx[108]} ,
  {0, "type" ,128,1,0,0,0,0,0,0,NULL,&atx[22],NULL,0,&atx[109]} ,
  {0, "fld" ,128,2,0,0,0,0,0,0,NULL,&atx[33],NULL,0,&atx[110]} ,
  {0, "uids" ,128,3,0,0,0,0,0,0,NULL,&atx[111],NULL,0,NULL} ,
  {304, "OCTET STRING" ,0,4,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "seqidforgi" ,128,15,0,0,0,0,0,0,NULL,&atx[20],NULL,0,&atx[113]} ,
  {0, "findseqid" ,128,16,0,0,0,0,0,0,NULL,&atx[1],NULL,0,&atx[114]} ,
  {0, "canneighbortext" ,128,17,0,0,0,0,0,0,NULL,&atx[16],NULL,0,&atx[115]} ,
  {0, "expanded-medline" ,128,18,0,0,0,0,0,0,NULL,&atx[16],NULL,0,&atx[116]} ,
  {0, "get-hierarchy" ,128,19,0,0,0,0,0,0,NULL,&atx[87],NULL,0,&atx[117]} ,
  {0, "neighbortext" ,128,20,0,0,0,0,0,0,NULL,&atx[118],NULL,0,&atx[125]} ,
  {421, "Entrez-neighbor-text" ,1,0,0,0,0,0,0,0,NULL,&atx[14],&atx[119],0,&atx[135]} ,
  {0, "fld" ,128,0,0,0,0,0,0,0,NULL,&atx[33],NULL,0,&atx[120]} ,
  {0, "percent-terms-to-use" ,128,1,0,0,0,0,0,0,NULL,&atx[20],NULL,0,&atx[121]} ,
  {0, "max-neighbors" ,128,2,0,0,0,0,0,0,NULL,&atx[20],NULL,0,&atx[122]} ,
  {0, "min-score" ,128,3,0,0,0,0,0,0,NULL,&atx[20],NULL,0,&atx[123]} ,
  {0, "normal-text" ,128,4,0,0,0,0,0,0,NULL,&atx[13],NULL,0,&atx[124]} ,
  {0, "special-text" ,128,5,0,0,0,0,0,0,NULL,&atx[13],NULL,0,NULL} ,
  {0, "eval-count" ,128,21,0,0,0,0,0,0,NULL,&atx[18],NULL,0,&atx[126]} ,
  {0, "initX" ,128,22,0,0,0,0,0,0,NULL,&atx[14],&atx[127],0,&atx[128]} ,
  {0, "version" ,128,0,0,1,0,0,0,0,NULL,&atx[13],NULL,0,NULL} ,
  {0, "getbiostr" ,128,23,0,0,0,0,0,0,NULL,&atx[54],NULL,0,&atx[129]} ,
  {0, "getbiostrX" ,128,24,0,0,0,0,0,0,NULL,&atx[14],&atx[130],0,&atx[133]} ,
  {0, "complexity" ,128,0,0,0,0,0,0,0,NULL,&atx[20],NULL,0,&atx[131]} ,
  {0, "get" ,128,1,0,0,0,0,0,0,NULL,&atx[54],NULL,0,&atx[132]} ,
  {0, "max-models" ,128,2,0,1,0,0,0,0,NULL,&atx[20],NULL,0,NULL} ,
  {0, "extrainfo" ,128,25,0,0,0,0,0,0,NULL,&atx[20],NULL,0,&atx[134]} ,
  {0, "blast" ,128,26,0,0,0,0,0,0,NULL,&atx[135],NULL,0,&atx[142]} ,
  {422, "Entrez-blastreq" ,1,0,0,0,0,0,0,0,NULL,&atx[14],&atx[136],0,&atx[145]} ,
  {0, "bsp" ,128,0,0,0,0,0,0,0,NULL,&atx[6],NULL,0,&atx[137]} ,
  {0, "bsp-database" ,128,1,0,0,0,0,0,0,NULL,&atx[22],NULL,0,&atx[138]} ,
  {0, "program" ,128,2,0,1,0,0,0,0,NULL,&atx[13],NULL,0,&atx[139]} ,
  {0, "database" ,128,3,0,1,0,0,0,0,NULL,&atx[13],NULL,0,&atx[140]} ,
  {0, "options" ,128,4,0,1,0,0,0,0,NULL,&atx[13],NULL,0,&atx[141]} ,
  {0, "showprogress" ,128,5,0,1,0,0,0,0,NULL,&atx[57],NULL,0,NULL} ,
  {0, "docsumX" ,128,27,0,0,0,0,0,0,NULL,&atx[54],NULL,0,&atx[143]} ,
  {0, "getgenome" ,128,28,0,0,0,0,0,0,NULL,&atx[54],NULL,0,&atx[144]} ,
  {0, "cluster-arts" ,128,29,0,0,0,0,0,0,NULL,&atx[145],NULL,0,&atx[151]} ,
  {423, "Cluster-articles" ,1,0,0,0,0,0,0,0,NULL,&atx[14],&atx[146],0,&atx[153]} ,
  {0, "ids" ,128,0,0,0,0,0,0,0,NULL,&atx[59],NULL,0,&atx[147]} ,
  {0, "fld" ,128,1,0,0,0,0,0,0,NULL,&atx[33],NULL,0,&atx[148]} ,
  {0, "min-cluster" ,128,2,0,0,0,0,0,0,NULL,&atx[20],NULL,0,&atx[149]} ,
  {0, "max-cluster" ,128,3,0,0,0,0,0,0,NULL,&atx[20],NULL,0,&atx[150]} ,
  {0, "max-terms" ,128,4,0,0,0,0,0,0,NULL,&atx[20],NULL,0,NULL} ,
  {0, "getbiostrannot" ,128,30,0,0,0,0,0,0,NULL,&atx[20],NULL,0,&atx[152]} ,
  {0, "getbiostr-feat-ids" ,128,31,0,0,0,0,0,0,NULL,&atx[153],NULL,0,&atx[157]} ,
  {424, "Get-feat-ids" ,1,0,0,0,0,0,0,0,NULL,&atx[14],&atx[154],0,&atx[158]} ,
  {0, "mmdbid" ,128,0,0,0,0,0,0,0,NULL,&atx[20],NULL,0,&atx[155]} ,
  {0, "feature-type" ,128,1,0,0,0,0,0,0,NULL,&atx[20],NULL,0,&atx[156]} ,
  {0, "feature-set-id" ,128,2,0,0,0,0,0,0,NULL,&atx[20],NULL,0,NULL} ,
  {0, "getbiostr-annot-by-fid" ,128,32,0,0,0,0,0,0,NULL,&atx[158],NULL,0,NULL} ,
  {425, "Get-by-fid" ,1,0,0,0,0,0,0,0,NULL,&atx[14],&atx[159],0,&atx[59]} ,
  {0, "mmdbid" ,128,0,0,0,0,0,0,0,NULL,&atx[20],NULL,0,&atx[160]} ,
  {0, "feature-id" ,128,1,0,0,0,0,0,0,NULL,&atx[20],NULL,0,&atx[161]} ,
  {0, "feature-set-id" ,128,2,0,0,0,0,0,0,NULL,&atx[20],NULL,0,NULL} ,
  {428, "Cluster-resp" ,1,0,0,0,0,0,0,0,NULL,&atx[14],&atx[163],0,&atx[22]} ,
  {0, "count" ,128,0,0,0,0,0,0,0,NULL,&atx[20],NULL,0,&atx[164]} ,
  {0, "terms" ,128,1,0,0,0,0,0,0,NULL,&atx[43],&atx[165],0,&atx[166]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[13],NULL,0,NULL} ,
  {0, "term-counts" ,128,2,0,0,0,0,0,0,NULL,&atx[43],&atx[167],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[20],NULL,0,NULL} ,
  {436, "Entrez-back" ,1,0,0,0,0,0,0,0,NULL,&atx[42],&atx[169],0,&atx[178]} ,
  {0, "error" ,128,0,0,0,0,0,0,0,NULL,&atx[20],NULL,0,&atx[170]} ,
  {0, "init" ,128,1,0,0,0,0,0,0,NULL,&atx[14],&atx[171],0,&atx[172]} ,
  {0, "e-info" ,128,0,0,1,0,0,0,0,NULL,&atx[8],NULL,0,NULL} ,
  {0, "maxlinks" ,128,2,0,0,0,0,0,0,NULL,&atx[20],NULL,0,&atx[173]} ,
  {0, "eval" ,128,3,0,0,0,0,0,0,NULL,&atx[42],&atx[174],0,&atx[176]} ,
  {0, "bad-count" ,128,0,0,0,0,0,0,0,NULL,&atx[20],NULL,0,&atx[175]} ,
  {0, "link-set" ,128,1,0,0,0,0,0,0,NULL,&atx[3],NULL,0,NULL} ,
  {0, "docsum" ,128,4,0,0,0,0,0,0,NULL,&atx[42],&atx[177],0,&atx[199]} ,
  {0, "ml" ,128,0,0,0,0,0,0,0,NULL,&atx[178],NULL,0,&atx[189]} ,
  {437, "Ml-summary-list" ,1,0,0,0,0,0,0,0,NULL,&atx[14],&atx[179],0,&atx[190]} ,
  {0, "num" ,128,0,0,0,0,0,0,0,NULL,&atx[20],NULL,0,&atx[180]} ,
  {0, "data" ,128,1,0,0,0,0,0,0,NULL,&atx[43],&atx[181],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[182],NULL,0,NULL} ,
  {448, "Ml-summary" ,1,0,0,0,0,0,0,0,NULL,&atx[14],&atx[183],0,&atx[194]} ,
  {0, "muid" ,128,0,0,0,0,0,0,0,NULL,&atx[20],NULL,0,&atx[184]} ,
  {0, "no-abstract" ,128,1,0,0,1,0,0,0,&avnx[35],&atx[57],NULL,0,&atx[185]} ,
  {0, "translated-title" ,128,2,0,0,1,0,0,0,&avnx[36],&atx[57],NULL,0,&atx[186]} ,
  {0, "no-authors" ,128,3,0,0,1,0,0,0,&avnx[37],&atx[57],NULL,0,&atx[187]} ,
  {0, "caption" ,128,4,0,1,0,0,0,0,NULL,&atx[13],NULL,0,&atx[188]} ,
  {0, "title" ,128,5,0,1,0,0,0,0,NULL,&atx[13],NULL,0,NULL} ,
  {0, "seq" ,128,1,0,0,0,0,0,0,NULL,&atx[190],NULL,0,&atx[198]} ,
  {438, "Seq-summary-list" ,1,0,0,0,0,0,0,0,NULL,&atx[14],&atx[191],0,&atx[200]} ,
  {0, "num" ,128,0,0,0,0,0,0,0,NULL,&atx[20],NULL,0,&atx[192]} ,
  {0, "data" ,128,1,0,0,0,0,0,0,NULL,&atx[43],&atx[193],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[194],NULL,0,NULL} ,
  {449, "Seq-summary" ,1,0,0,0,0,0,0,0,NULL,&atx[14],&atx[195],0,&atx[212]} ,
  {0, "id" ,128,0,0,0,0,0,0,0,NULL,&atx[20],NULL,0,&atx[196]} ,
  {0, "caption" ,128,1,0,1,0,0,0,0,NULL,&atx[13],NULL,0,&atx[197]} ,
  {0, "title" ,128,2,0,1,0,0,0,0,NULL,&atx[13],NULL,0,NULL} ,
  {0, "str" ,128,2,0,0,0,0,0,0,NULL,&atx[190],NULL,0,NULL} ,
  {0, "linkuidlist" ,128,5,0,0,0,0,0,0,NULL,&atx[200],NULL,0,&atx[204]} ,
  {439, "Marked-link-set" ,1,0,0,0,0,0,0,0,NULL,&atx[14],&atx[201],0,&atx[206]} ,
  {0, "link-set" ,128,0,0,0,0,0,0,0,NULL,&atx[3],NULL,0,&atx[202]} ,
  {0, "uids-processed" ,128,1,0,0,0,0,0,0,NULL,&atx[20],NULL,0,&atx[203]} ,
  {0, "marked-missing" ,128,2,0,1,0,0,0,0,NULL,&atx[59],NULL,0,NULL} ,
  {0, "uidlinks" ,128,6,0,0,0,0,0,0,NULL,&atx[200],NULL,0,&atx[205]} ,
  {0, "byterm" ,128,7,0,0,0,0,0,0,NULL,&atx[206],NULL,0,&atx[216]} ,
  {440, "Entrez-term-resp" ,1,0,0,0,0,0,0,0,NULL,&atx[14],&atx[207],0,&atx[218]} ,
  {0, "num-terms" ,128,0,0,0,0,0,0,0,NULL,&atx[20],NULL,0,&atx[208]} ,
  {0, "first-page" ,128,1,0,1,0,0,0,0,NULL,&atx[20],NULL,0,&atx[209]} ,
  {0, "pages-read" ,128,2,0,0,0,0,0,0,NULL,&atx[20],NULL,0,&atx[210]} ,
  {0, "info" ,128,3,0,0,0,0,0,0,NULL,&atx[43],&atx[211],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[212],NULL,0,NULL} ,
  {450, "Term-page-info" ,1,0,0,0,0,0,0,0,NULL,&atx[14],&atx[213],0,&atx[251]} ,
  {0, "term" ,128,0,0,0,0,0,0,0,NULL,&atx[13],NULL,0,&atx[214]} ,
  {0, "spec-count" ,128,1,0,0,0,0,0,0,NULL,&atx[20],NULL,0,&atx[215]} ,
  {0, "tot-count" ,128,2,0,0,0,0,0,0,NULL,&atx[20],NULL,0,NULL} ,
  {0, "bypage" ,128,8,0,0,0,0,0,0,NULL,&atx[206],NULL,0,&atx[217]} ,
  {0, "findterm" ,128,9,0,0,0,0,0,0,NULL,&atx[218],NULL,0,&atx[222]} ,
  {441, "Term-counts" ,1,0,0,0,0,0,0,0,NULL,&atx[14],&atx[219],0,&atx[225]} ,
  {0, "found" ,128,0,0,0,0,0,0,0,NULL,&atx[57],NULL,0,&atx[220]} ,
  {0, "spec-count" ,128,1,0,0,0,0,0,0,NULL,&atx[20],NULL,0,&atx[221]} ,
  {0, "tot-count" ,128,2,0,0,0,0,0,0,NULL,&atx[20],NULL,0,NULL} ,
  {0, "fini" ,128,10,0,0,0,0,0,0,NULL,&atx[16],NULL,0,&atx[223]} ,
  {0, "createnamed" ,128,11,0,0,0,0,0,0,NULL,&atx[16],NULL,0,&atx[224]} ,
  {0, "getmle" ,128,12,0,0,0,0,0,0,NULL,&atx[225],NULL,0,&atx[230]} ,
  {442, "Entrez-Medline-entry-list" ,1,0,0,0,0,0,0,0,NULL,&atx[14],&atx[226],0,&atx[231]} ,
  {0, "num" ,128,0,0,0,0,0,0,0,NULL,&atx[20],NULL,0,&atx[227]} ,
  {0, "data" ,128,1,0,0,0,0,0,0,NULL,&atx[43],&atx[228],0,&atx[229]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[2],NULL,0,NULL} ,
  {0, "marked-missing" ,128,2,0,1,0,0,0,0,NULL,&atx[59],NULL,0,NULL} ,
  {0, "getseq" ,128,13,0,0,0,0,0,0,NULL,&atx[231],NULL,0,&atx[236]} ,
  {443, "Entrez-Seq-entry-list" ,1,0,0,0,0,0,0,0,NULL,&atx[14],&atx[232],0,&atx[243]} ,
  {0, "num" ,128,0,0,0,0,0,0,0,NULL,&atx[20],NULL,0,&atx[233]} ,
  {0, "data" ,128,1,0,0,0,0,0,0,NULL,&atx[43],&atx[234],0,&atx[235]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[0],NULL,0,NULL} ,
  {0, "marked-missing" ,128,2,0,1,0,0,0,0,NULL,&atx[59],NULL,0,NULL} ,
  {0, "evalX" ,128,14,0,0,0,0,0,0,NULL,&atx[111],NULL,0,&atx[237]} ,
  {0, "createnamedX" ,128,15,0,0,0,0,0,0,NULL,&atx[16],NULL,0,&atx[238]} ,
  {0, "seqidforgi" ,128,16,0,0,0,0,0,0,NULL,&atx[1],NULL,0,&atx[239]} ,
  {0, "findseqid" ,128,17,0,0,0,0,0,0,NULL,&atx[20],NULL,0,&atx[240]} ,
  {0, "canneighbortext" ,128,18,0,0,0,0,0,0,NULL,&atx[57],NULL,0,&atx[241]} ,
  {0, "expanded-medline" ,128,19,0,0,0,0,0,0,NULL,&atx[57],NULL,0,&atx[242]} ,
  {0, "get-hierarchy" ,128,20,0,0,0,0,0,0,NULL,&atx[243],NULL,0,&atx[257]} ,
  {444, "Entrez-Tree" ,1,0,0,0,0,0,0,0,NULL,&atx[14],&atx[244],0,&atx[260]} ,
  {0, "num-in-lineage" ,128,0,0,0,0,0,0,0,NULL,&atx[20],NULL,0,&atx[245]} ,
  {0, "num-children" ,128,1,0,0,0,0,0,0,NULL,&atx[20],NULL,0,&atx[246]} ,
  {0, "term" ,128,2,0,0,0,0,0,0,NULL,&atx[13],NULL,0,&atx[247]} ,
  {0, "lineage" ,128,3,0,0,0,0,0,0,NULL,&atx[43],&atx[248],0,&atx[249]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[13],NULL,0,NULL} ,
  {0, "children" ,128,4,0,0,0,0,0,0,NULL,&atx[43],&atx[250],0,&atx[256]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[251],NULL,0,NULL} ,
  {451, "Entrez-Tree-Child" ,1,0,0,0,0,0,0,0,NULL,&atx[14],&atx[252],0,NULL} ,
  {0, "name" ,128,0,0,0,0,0,0,0,NULL,&atx[13],NULL,0,&atx[253]} ,
  {0, "is-leaf-node" ,128,1,0,0,0,0,0,0,NULL,&atx[57],NULL,0,&atx[254]} ,
  {0, "special" ,128,2,0,0,0,0,0,0,NULL,&atx[20],NULL,0,&atx[255]} ,
  {0, "total" ,128,3,0,0,0,0,0,0,NULL,&atx[20],NULL,0,NULL} ,
  {0, "canonical-form" ,128,5,0,1,0,0,0,0,NULL,&atx[13],NULL,0,NULL} ,
  {0, "neighbortext" ,128,21,0,0,0,0,0,0,NULL,&atx[3],NULL,0,&atx[258]} ,
  {0, "eval-count" ,128,22,0,0,0,0,0,0,NULL,&atx[20],NULL,0,&atx[259]} ,
  {0, "getbiostr" ,128,23,0,0,0,0,0,0,NULL,&atx[260],NULL,0,&atx[265]} ,
  {445, "Entrez-Biostruc-list" ,1,0,0,0,0,0,0,0,NULL,&atx[14],&atx[261],0,&atx[267]} ,
  {0, "num" ,128,0,0,0,0,0,0,0,NULL,&atx[20],NULL,0,&atx[262]} ,
  {0, "data" ,128,1,0,0,0,0,0,0,NULL,&atx[43],&atx[263],0,&atx[264]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[4],NULL,0,NULL} ,
  {0, "marked-missing" ,128,2,0,1,0,0,0,0,NULL,&atx[59],NULL,0,NULL} ,
  {0, "getbiostrX" ,128,24,0,0,0,0,0,0,NULL,&atx[260],NULL,0,&atx[266]} ,
  {0, "extrainfo" ,128,25,0,0,0,0,0,0,NULL,&atx[267],NULL,0,&atx[272]} ,
  {446, "Entrez-extra-info" ,1,0,0,0,0,0,0,0,NULL,&atx[14],&atx[268],0,&atx[278]} ,
  {0, "maxlinks" ,128,0,0,0,0,0,0,0,NULL,&atx[20],NULL,0,&atx[269]} ,
  {0, "canneighbortext" ,128,1,0,0,0,0,0,0,NULL,&atx[57],NULL,0,&atx[270]} ,
  {0, "expanded-medline" ,128,2,0,0,0,0,0,0,NULL,&atx[57],NULL,0,&atx[271]} ,
  {0, "canblast" ,128,3,0,0,0,0,0,0,NULL,&atx[57],NULL,0,NULL} ,
  {0, "blast" ,128,26,0,0,0,0,0,0,NULL,&atx[42],&atx[273],0,&atx[277]} ,
  {0, "bad-count" ,128,0,0,0,0,0,0,0,NULL,&atx[20],NULL,0,&atx[274]} ,
  {0, "link-set" ,128,1,0,0,0,0,0,0,NULL,&atx[3],NULL,0,&atx[275]} ,
  {0, "job-start" ,128,2,0,0,0,0,0,0,NULL,&atx[20],NULL,0,&atx[276]} ,
  {0, "job-progress" ,128,3,0,0,0,0,0,0,NULL,&atx[20],NULL,0,NULL} ,
  {0, "docsumX" ,128,27,0,0,0,0,0,0,NULL,&atx[278],NULL,0,&atx[283]} ,
  {447, "New-summary-list" ,1,0,0,0,0,0,0,0,NULL,&atx[14],&atx[279],0,&atx[182]} ,
  {0, "num" ,128,0,0,0,0,0,0,0,NULL,&atx[20],NULL,0,&atx[280]} ,
  {0, "type" ,128,1,0,0,0,0,0,0,NULL,&atx[22],NULL,0,&atx[281]} ,
  {0, "data" ,128,2,0,0,0,0,0,0,NULL,&atx[43],&atx[282],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[9],NULL,0,NULL} ,
  {0, "getgenome" ,128,28,0,0,0,0,0,0,NULL,&atx[231],NULL,0,&atx[284]} ,
  {0, "cluster-arts" ,128,29,0,0,0,0,0,0,NULL,&atx[162],NULL,0,&atx[285]} ,
  {0, "getbiostrannot" ,128,30,0,0,0,0,0,0,NULL,&atx[5],NULL,0,&atx[286]} ,
  {0, "getbiostr-feat-ids" ,128,31,0,0,0,0,0,0,NULL,&atx[3],NULL,0,&atx[287]} ,
  {0, "getbiostr-annot-by-fid" ,128,32,0,0,0,0,0,0,NULL,&atx[5],NULL,0,NULL} };

static AsnModule ampx[1] = {
  { "NCBI-Entrez" , "asnneten.h60",&atx[0],NULL,NULL,0,0} };

static AsnValxNodePtr avn = avnx;
static AsnTypePtr at = atx;
static AsnModulePtr amp = ampx;



/**************************************************
*
*    Defines for Module NCBI-Entrez
*
**************************************************/

#define ENTREZ_REQUEST &at[10]
#define ENTREZ_REQUEST_init &at[11]
#define ENTREZ_REQUEST_init_version &at[12]
#define ENTREZ_REQUEST_maxlinks &at[15]
#define ENTREZ_REQUEST_eval &at[17]
#define ENTREZ_REQUEST_docsum &at[53]
#define ENTREZ_REQUEST_linkuidlist &at[64]
#define ENTREZ_REQUEST_uidlinks &at[73]
#define ENTREZ_REQUEST_byterm &at[74]
#define ENTREZ_REQUEST_bypage &at[80]
#define ENTREZ_REQUEST_findterm &at[86]
#define ENTREZ_REQUEST_fini &at[91]
#define ENTREZ_REQUEST_createnamed &at[92]
#define ENTREZ_REQUEST_getmle &at[98]
#define ENTREZ_REQUEST_getseq &at[99]
#define ENTREZ_REQUEST_evalX &at[104]
#define ENTREZ_REQUEST_createnamedX &at[105]
#define ENTREZ_REQUEST_seqidforgi &at[112]
#define ENTREZ_REQUEST_findseqid &at[113]
#define ENTREZ_REQUEST_canneighbortext &at[114]
#define ENTREZ_REQUEST_expanded_medline &at[115]
#define ENTREZ_REQUEST_get_hierarchy &at[116]
#define ENTREZ_REQUEST_neighbortext &at[117]
#define ENTREZ_REQUEST_eval_count &at[125]
#define ENTREZ_REQUEST_initX &at[126]
#define ENTREZ_REQUEST_initX_version &at[127]
#define ENTREZ_REQUEST_getbiostr &at[128]
#define ENTREZ_REQUEST_getbiostrX &at[129]
#define REQUEST_getbiostrX_complexity &at[130]
#define ENTREZ_REQUEST_getbiostrX_get &at[131]
#define REQUEST_getbiostrX_max_models &at[132]
#define ENTREZ_REQUEST_extrainfo &at[133]
#define ENTREZ_REQUEST_blast &at[134]
#define ENTREZ_REQUEST_docsumX &at[142]
#define ENTREZ_REQUEST_getgenome &at[143]
#define ENTREZ_REQUEST_cluster_arts &at[144]
#define ENTREZ_REQUEST_getbiostrannot &at[151]
#define REQUEST_getbiostr_feat_ids &at[152]
#define REQUEST_getbiostr_annot_by_fid &at[157]

#define ENTREZ_TERMGET &at[18]
#define ENTREZ_TERMGET_max &at[19]
#define ENTREZ_TERMGET_cls &at[21]
#define ENTREZ_TERMGET_terms &at[24]
#define ENTREZ_TERMGET_date_constraints &at[44]

#define ENTREZ_DOCGET &at[54]
#define ENTREZ_DOCGET_class &at[55]
#define ENTREZ_DOCGET_mark_missing &at[56]
#define ENTREZ_DOCGET_ids &at[58]
#define ENTREZ_DOCGET_defer_count &at[63]

#define LINK_SETGET &at[65]
#define LINK_SETGET_max &at[66]
#define LINK_SETGET_link_to_cls &at[67]
#define LINK_SETGET_query_cls &at[68]
#define LINK_SETGET_mark_missing &at[69]
#define LINK_SETGET_query_size &at[70]
#define LINK_SETGET_query &at[71]
#define LINK_SETGET_query_E &at[72]

#define ENTREZ_TERM_BY_TERM &at[75]
#define ENTREZ_TERM_BY_TERM_type &at[76]
#define ENTREZ_TERM_BY_TERM_fld &at[77]
#define ENTREZ_TERM_BY_TERM_term &at[78]
#define ENTREZ_TERM_BY_TERM_num_terms &at[79]

#define ENTREZ_TERM_BY_PAGE &at[81]
#define ENTREZ_TERM_BY_PAGE_type &at[82]
#define ENTREZ_TERM_BY_PAGE_fld &at[83]
#define ENTREZ_TERM_BY_PAGE_page &at[84]
#define ENTREZ_TERM_BY_PAGE_num_pages &at[85]

#define TERM_LOOKUP &at[87]
#define TERM_LOOKUP_type &at[88]
#define TERM_LOOKUP_fld &at[89]
#define TERM_LOOKUP_term &at[90]

#define ENTREZ_NAMED_LIST &at[93]
#define ENTREZ_NAMED_LIST_term &at[94]
#define ENTREZ_NAMED_LIST_type &at[95]
#define ENTREZ_NAMED_LIST_fld &at[96]
#define ENTREZ_NAMED_LIST_uids &at[97]

#define ENTREZ_SEQGET &at[100]
#define ENTREZ_SEQGET_retype &at[101]
#define ENTREZ_SEQGET_mark_missing &at[102]
#define ENTREZ_SEQGET_ids &at[103]

#define ENTREZ_NAMED_LISTX &at[106]
#define ENTREZ_NAMED_LISTX_term &at[107]
#define ENTREZ_NAMED_LISTX_type &at[108]
#define ENTREZ_NAMED_LISTX_fld &at[109]
#define ENTREZ_NAMED_LISTX_uids &at[110]

#define ENTREZ_NEIGHBOR_TEXT &at[118]
#define ENTREZ_NEIGHBOR_TEXT_fld &at[119]
#define TEXT_percent_terms_to_use &at[120]
#define NEIGHBOR_TEXT_max_neighbors &at[121]
#define ENTREZ_NEIGHBOR_TEXT_min_score &at[122]
#define NEIGHBOR_TEXT_normal_text &at[123]
#define NEIGHBOR_TEXT_special_text &at[124]

#define ENTREZ_BLASTREQ &at[135]
#define ENTREZ_BLASTREQ_bsp &at[136]
#define ENTREZ_BLASTREQ_bsp_database &at[137]
#define ENTREZ_BLASTREQ_program &at[138]
#define ENTREZ_BLASTREQ_database &at[139]
#define ENTREZ_BLASTREQ_options &at[140]
#define ENTREZ_BLASTREQ_showprogress &at[141]

#define CLUSTER_ARTICLES &at[145]
#define CLUSTER_ARTICLES_ids &at[146]
#define CLUSTER_ARTICLES_fld &at[147]
#define CLUSTER_ARTICLES_min_cluster &at[148]
#define CLUSTER_ARTICLES_max_cluster &at[149]
#define CLUSTER_ARTICLES_max_terms &at[150]

#define GET_FEAT_IDS &at[153]
#define GET_FEAT_IDS_mmdbid &at[154]
#define GET_FEAT_IDS_feature_type &at[155]
#define GET_FEAT_IDS_feature_set_id &at[156]

#define GET_BY_FID &at[158]
#define GET_BY_FID_mmdbid &at[159]
#define GET_BY_FID_feature_id &at[160]
#define GET_BY_FID_feature_set_id &at[161]

#define ENTREZ_IDS &at[59]
#define ENTREZ_IDS_numid &at[60]
#define ENTREZ_IDS_ids &at[61]
#define ENTREZ_IDS_ids_E &at[62]

#define ENTREZ_FIELD &at[33]

#define CLUSTER_RESP &at[162]
#define CLUSTER_RESP_count &at[163]
#define CLUSTER_RESP_terms &at[164]
#define CLUSTER_RESP_terms_E &at[165]
#define CLUSTER_RESP_term_counts &at[166]
#define CLUSTER_RESP_term_counts_E &at[167]

#define ENTREZ_CLASS &at[22]

#define ENTREZ_TERM_LIST &at[25]
#define ENTREZ_TERM_LIST_E &at[26]
#define ENTREZ_TERM_LIST_E_operator &at[27]
#define ENTREZ_TERM_LIST_E_sp_operand &at[29]
#define ENTREZ_TERM_LIST_E_tot_operand &at[36]

#define DATE_CONSTRAINTS &at[45]
#define DATE_CONSTRAINTS_count &at[46]
#define DATE_CONSTRAINTS_date_vec &at[47]
#define DATE_CONSTRAINTS_date_vec_E &at[48]

#define DATE_VECTOR &at[49]
#define DATE_VECTOR_begin_date &at[50]
#define DATE_VECTOR_end_date &at[51]
#define DATE_VECTOR_field_abbr &at[52]

#define ENTREZ_OPERATOR &at[28]

#define SPECIAL_OPERAND &at[30]
#define SPECIAL_OPERAND_term &at[31]
#define SPECIAL_OPERAND_fld &at[32]
#define SPECIAL_OPERAND_type &at[34]
#define SPECIAL_OPERAND_high_range &at[35]

#define TOTAL_OPERAND &at[37]
#define TOTAL_OPERAND_term &at[38]
#define TOTAL_OPERAND_fld &at[39]
#define TOTAL_OPERAND_type &at[40]
#define TOTAL_OPERAND_high_range &at[41]

#define ENTREZ_BACK &at[168]
#define ENTREZ_BACK_error &at[169]
#define ENTREZ_BACK_init &at[170]
#define ENTREZ_BACK_init_e_info &at[171]
#define ENTREZ_BACK_maxlinks &at[172]
#define ENTREZ_BACK_eval &at[173]
#define ENTREZ_BACK_eval_bad_count &at[174]
#define ENTREZ_BACK_eval_link_set &at[175]
#define ENTREZ_BACK_docsum &at[176]
#define ENTREZ_BACK_docsum_ml &at[177]
#define ENTREZ_BACK_docsum_seq &at[189]
#define ENTREZ_BACK_docsum_str &at[198]
#define ENTREZ_BACK_linkuidlist &at[199]
#define ENTREZ_BACK_uidlinks &at[204]
#define ENTREZ_BACK_byterm &at[205]
#define ENTREZ_BACK_bypage &at[216]
#define ENTREZ_BACK_findterm &at[217]
#define ENTREZ_BACK_fini &at[222]
#define ENTREZ_BACK_createnamed &at[223]
#define ENTREZ_BACK_getmle &at[224]
#define ENTREZ_BACK_getseq &at[230]
#define ENTREZ_BACK_evalX &at[236]
#define ENTREZ_BACK_createnamedX &at[237]
#define ENTREZ_BACK_seqidforgi &at[238]
#define ENTREZ_BACK_findseqid &at[239]
#define ENTREZ_BACK_canneighbortext &at[240]
#define ENTREZ_BACK_expanded_medline &at[241]
#define ENTREZ_BACK_get_hierarchy &at[242]
#define ENTREZ_BACK_neighbortext &at[257]
#define ENTREZ_BACK_eval_count &at[258]
#define ENTREZ_BACK_getbiostr &at[259]
#define ENTREZ_BACK_getbiostrX &at[265]
#define ENTREZ_BACK_extrainfo &at[266]
#define ENTREZ_BACK_blast &at[272]
#define ENTREZ_BACK_blast_bad_count &at[273]
#define ENTREZ_BACK_blast_link_set &at[274]
#define ENTREZ_BACK_blast_job_start &at[275]
#define ENTREZ_BACK_blast_job_progress &at[276]
#define ENTREZ_BACK_docsumX &at[277]
#define ENTREZ_BACK_getgenome &at[283]
#define ENTREZ_BACK_cluster_arts &at[284]
#define ENTREZ_BACK_getbiostrannot &at[285]
#define ENTREZ_BACK_getbiostr_feat_ids &at[286]
#define BACK_getbiostr_annot_by_fid &at[287]

#define ML_SUMMARY_LIST &at[178]
#define ML_SUMMARY_LIST_num &at[179]
#define ML_SUMMARY_LIST_data &at[180]
#define ML_SUMMARY_LIST_data_E &at[181]

#define SEQ_SUMMARY_LIST &at[190]
#define SEQ_SUMMARY_LIST_num &at[191]
#define SEQ_SUMMARY_LIST_data &at[192]
#define SEQ_SUMMARY_LIST_data_E &at[193]

#define MARKED_LINK_SET &at[200]
#define MARKED_LINK_SET_link_set &at[201]
#define MARKED_LINK_SET_uids_processed &at[202]
#define MARKED_LINK_SET_marked_missing &at[203]

#define ENTREZ_TERM_RESP &at[206]
#define ENTREZ_TERM_RESP_num_terms &at[207]
#define ENTREZ_TERM_RESP_first_page &at[208]
#define ENTREZ_TERM_RESP_pages_read &at[209]
#define ENTREZ_TERM_RESP_info &at[210]
#define ENTREZ_TERM_RESP_info_E &at[211]

#define TERM_COUNTS &at[218]
#define TERM_COUNTS_found &at[219]
#define TERM_COUNTS_spec_count &at[220]
#define TERM_COUNTS_tot_count &at[221]

#define ENTREZ_MEDLINE_ENTRY_LIST &at[225]
#define ENTREZ_MEDLINE_ENTRY_LIST_num &at[226]
#define ENTREZ_MEDLINE_ENTRY_LIST_data &at[227]
#define MEDLINE_ENTRY_LIST_data_E &at[228]
#define ENTRY_LIST_marked_missing &at[229]

#define ENTREZ_SEQ_ENTRY_LIST &at[231]
#define ENTREZ_SEQ_ENTRY_LIST_num &at[232]
#define ENTREZ_SEQ_ENTRY_LIST_data &at[233]
#define ENTREZ_SEQ_ENTRY_LIST_data_E &at[234]
#define SEQ_ENTRY_LIST_marked_missing &at[235]

#define ENTREZ_TREE &at[243]
#define ENTREZ_TREE_num_in_lineage &at[244]
#define ENTREZ_TREE_num_children &at[245]
#define ENTREZ_TREE_term &at[246]
#define ENTREZ_TREE_lineage &at[247]
#define ENTREZ_TREE_lineage_E &at[248]
#define ENTREZ_TREE_children &at[249]
#define ENTREZ_TREE_children_E &at[250]
#define ENTREZ_TREE_canonical_form &at[256]

#define ENTREZ_BIOSTRUC_LIST &at[260]
#define ENTREZ_BIOSTRUC_LIST_num &at[261]
#define ENTREZ_BIOSTRUC_LIST_data &at[262]
#define ENTREZ_BIOSTRUC_LIST_data_E &at[263]
#define BIOSTRUC_LIST_marked_missing &at[264]

#define ENTREZ_EXTRA_INFO &at[267]
#define ENTREZ_EXTRA_INFO_maxlinks &at[268]
#define EXTRA_INFO_canneighbortext &at[269]
#define EXTRA_INFO_expanded_medline &at[270]
#define ENTREZ_EXTRA_INFO_canblast &at[271]

#define NEW_SUMMARY_LIST &at[278]
#define NEW_SUMMARY_LIST_num &at[279]
#define NEW_SUMMARY_LIST_type &at[280]
#define NEW_SUMMARY_LIST_data &at[281]
#define NEW_SUMMARY_LIST_data_E &at[282]

#define ML_SUMMARY &at[182]
#define ML_SUMMARY_muid &at[183]
#define ML_SUMMARY_no_abstract &at[184]
#define ML_SUMMARY_translated_title &at[185]
#define ML_SUMMARY_no_authors &at[186]
#define ML_SUMMARY_caption &at[187]
#define ML_SUMMARY_title &at[188]

#define SEQ_SUMMARY &at[194]
#define SEQ_SUMMARY_id &at[195]
#define SEQ_SUMMARY_caption &at[196]
#define SEQ_SUMMARY_title &at[197]

#define TERM_PAGE_INFO &at[212]
#define TERM_PAGE_INFO_term &at[213]
#define TERM_PAGE_INFO_spec_count &at[214]
#define TERM_PAGE_INFO_tot_count &at[215]

#define ENTREZ_TREE_CHILD &at[251]
#define ENTREZ_TREE_CHILD_name &at[252]
#define ENTREZ_TREE_CHILD_is_leaf_node &at[253]
#define ENTREZ_TREE_CHILD_special &at[254]
#define ENTREZ_TREE_CHILD_total &at[255]
