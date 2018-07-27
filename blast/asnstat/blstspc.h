/***********************************************************************
*
**
*        Automatic header module from ASNTOOL
*
************************************************************************/

#ifndef _ASNTOOL_
#include <asn.h>
#endif

static char * asnfilename = "blstspc.h22";
static AsnValxNode avnx[20] = {
    {20,"blastn" ,0,0.0,&avnx[1] } ,
    {20,"blastp" ,1,0.0,&avnx[2] } ,
    {20,"blastx" ,2,0.0,&avnx[3] } ,
    {20,"tblastn" ,3,0.0,&avnx[4] } ,
    {20,"tblastx" ,4,0.0,NULL } ,
    {20,"unknown" ,0,0.0,&avnx[6] } ,
    {20,"protein" ,1,0.0,&avnx[7] } ,
    {20,"nucleotide" ,2,0.0,NULL } ,
    {20,"none" ,0,0.0,&avnx[9] } ,
    {20,"info" ,1,0.0,&avnx[10] } ,
    {20,"warn" ,2,0.0,&avnx[11] } ,
    {20,"error" ,3,0.0,&avnx[12] } ,
    {20,"fatal" ,4,0.0,NULL } ,
    {20,"notset" ,0,0.0,&avnx[14] } ,
    {20,"plus1" ,1,0.0,&avnx[15] } ,
    {20,"plus2" ,2,0.0,&avnx[16] } ,
    {20,"plus3" ,3,0.0,&avnx[17] } ,
    {20,"minus1" ,4,0.0,&avnx[18] } ,
    {20,"minus2" ,5,0.0,&avnx[19] } ,
    {20,"minus3" ,6,0.0,NULL } };

static AsnType atx[182] = {
  {401, "Blast-search" ,1,0,0,0,0,1,0,0,NULL,&atx[63],&atx[1],0,&atx[81]} ,
  {0, "program" ,128,0,0,0,0,0,0,0,NULL,&atx[2],&avnx[0],0,&atx[3]} ,
  {310, "ENUMERATED" ,0,10,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "query" ,128,1,0,0,0,0,0,0,NULL,&atx[4],NULL,0,&atx[5]} ,
  {411, "Bioseq" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[80]} ,
  {0, "database" ,128,2,0,0,0,0,0,0,NULL,&atx[6],NULL,0,&atx[7]} ,
  {323, "VisibleString" ,0,26,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "parameters" ,128,3,0,1,0,0,0,0,NULL,&atx[8],NULL,0,&atx[64]} ,
  {404, "Blast-parameters" ,1,0,0,0,0,1,0,0,NULL,&atx[63],&atx[9],0,&atx[66]} ,
  {0, "first-threshold" ,128,0,0,1,0,0,0,0,NULL,&atx[10],NULL,0,&atx[11]} ,
  {302, "INTEGER" ,0,2,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "second-threshold" ,128,1,0,1,0,0,0,0,NULL,&atx[10],NULL,0,&atx[12]} ,
  {0, "cutoff" ,128,2,0,1,0,0,0,0,NULL,&atx[16],&atx[13],0,&atx[17]} ,
  {0, "evalue" ,128,0,0,0,0,0,0,0,NULL,&atx[14],NULL,0,&atx[15]} ,
  {309, "REAL" ,0,9,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "score" ,128,1,0,0,0,0,0,0,NULL,&atx[10],NULL,0,NULL} ,
  {315, "CHOICE" ,0,-1,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "cutoff2" ,128,3,0,1,0,0,0,0,NULL,&atx[16],&atx[18],0,&atx[20]} ,
  {0, "evalue" ,128,0,0,0,0,0,0,0,NULL,&atx[14],NULL,0,&atx[19]} ,
  {0, "score" ,128,1,0,0,0,0,0,0,NULL,&atx[10],NULL,0,NULL} ,
  {0, "hitlist-size" ,128,4,0,1,0,0,0,0,NULL,&atx[10],NULL,0,&atx[21]} ,
  {0, "nucl-penalty" ,128,5,0,1,0,0,0,0,NULL,&atx[10],NULL,0,&atx[22]} ,
  {0, "nucl-reward" ,128,6,0,1,0,0,0,0,NULL,&atx[10],NULL,0,&atx[23]} ,
  {0, "genetic-code" ,128,7,0,1,0,0,0,0,NULL,&atx[10],NULL,0,&atx[24]} ,
  {0, "db-genetic-code" ,128,8,0,1,0,0,0,0,NULL,&atx[10],NULL,0,&atx[25]} ,
  {0, "low-complexity-filtering" ,128,9,0,1,0,0,0,0,NULL,&atx[10],NULL,0,&atx[26]} ,
  {0, "gapped-alignment" ,128,10,0,0,0,0,0,0,NULL,&atx[27],NULL,0,&atx[28]} ,
  {301, "BOOLEAN" ,0,1,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "gap-open" ,128,11,0,1,0,0,0,0,NULL,&atx[10],NULL,0,&atx[29]} ,
  {0, "gap-extend" ,128,12,0,1,0,0,0,0,NULL,&atx[10],NULL,0,&atx[30]} ,
  {0, "required-start" ,128,13,0,1,0,0,0,0,NULL,&atx[10],NULL,0,&atx[31]} ,
  {0, "required-end" ,128,14,0,1,0,0,0,0,NULL,&atx[10],NULL,0,&atx[32]} ,
  {0, "ethresh" ,128,15,0,1,0,0,0,0,NULL,&atx[14],NULL,0,&atx[33]} ,
  {0, "max-num-passes" ,128,16,0,1,0,0,0,0,NULL,&atx[10],NULL,0,&atx[34]} ,
  {0, "pseudo-count-const" ,128,17,0,1,0,0,0,0,NULL,&atx[10],NULL,0,&atx[35]} ,
  {0, "other-options" ,128,18,0,1,0,0,0,0,NULL,&atx[6],NULL,0,&atx[36]} ,
  {0, "gilist" ,128,19,0,1,0,0,0,0,NULL,&atx[38],&atx[37],0,&atx[39]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[10],NULL,0,NULL} ,
  {312, "SEQUENCE OF" ,0,16,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "gifile" ,128,20,0,1,0,0,0,0,NULL,&atx[6],NULL,0,&atx[40]} ,
  {0, "matrix" ,128,21,0,1,0,0,0,0,NULL,&atx[6],NULL,0,&atx[41]} ,
  {0, "filter-string" ,128,22,0,1,0,0,0,0,NULL,&atx[6],NULL,0,&atx[42]} ,
  {0, "entrez-query" ,128,23,0,1,0,0,0,0,NULL,&atx[6],NULL,0,&atx[43]} ,
  {0, "word-size" ,128,24,0,1,0,0,0,0,NULL,&atx[10],NULL,0,&atx[44]} ,
  {0, "db-length" ,128,25,0,1,0,0,0,0,NULL,&atx[10],NULL,0,&atx[45]} ,
  {0, "searchsp-eff" ,128,26,0,1,0,0,0,0,NULL,&atx[14],NULL,0,&atx[46]} ,
  {0, "hsp-range-max" ,128,27,0,1,0,0,0,0,NULL,&atx[10],NULL,0,&atx[47]} ,
  {0, "block-width" ,128,28,0,1,0,0,0,0,NULL,&atx[10],NULL,0,&atx[48]} ,
  {0, "perform-culling" ,128,29,0,1,0,0,0,0,NULL,&atx[27],NULL,0,&atx[49]} ,
  {0, "strand-option" ,128,30,0,1,0,0,0,0,NULL,&atx[10],NULL,0,&atx[50]} ,
  {0, "phi-pattern" ,128,31,0,1,0,0,0,0,NULL,&atx[6],NULL,0,&atx[51]} ,
  {0, "use-real-db-size" ,128,32,0,1,0,0,0,0,NULL,&atx[27],NULL,0,&atx[52]} ,
  {0, "use-best-align" ,128,33,0,1,0,0,0,0,NULL,&atx[27],NULL,0,&atx[53]} ,
  {0, "is-rps-blast" ,128,34,0,1,0,0,0,0,NULL,&atx[27],NULL,0,&atx[54]} ,
  {0, "tweak-parameters" ,128,35,0,1,0,0,0,0,NULL,&atx[27],NULL,0,&atx[55]} ,
  {0, "smith-waterman" ,128,36,0,1,0,0,0,0,NULL,&atx[27],NULL,0,&atx[56]} ,
  {0, "is-megablast" ,128,37,0,1,0,0,0,0,NULL,&atx[27],NULL,0,&atx[57]} ,
  {0, "query-lcase-mask" ,128,38,0,1,0,0,0,0,NULL,&atx[38],&atx[58],0,&atx[60]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[59],NULL,0,NULL} ,
  {414, "Seq-loc" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[166]} ,
  {0, "is-ooframe" ,128,39,0,1,0,0,0,0,NULL,&atx[10],NULL,0,&atx[61]} ,
  {0, "endpoint-results" ,128,40,0,1,0,0,0,0,NULL,&atx[27],NULL,0,&atx[62]} ,
  {0, "percent-identity" ,128,41,0,1,0,0,0,0,NULL,&atx[14],NULL,0,NULL} ,
  {311, "SEQUENCE" ,0,16,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "mask" ,128,4,0,1,0,0,0,0,NULL,&atx[59],NULL,0,&atx[65]} ,
  {0, "matrix" ,128,5,0,1,0,0,0,0,NULL,&atx[66],NULL,0,&atx[78]} ,
  {405, "Blast-matrix" ,1,0,0,0,0,1,0,0,NULL,&atx[63],&atx[67],0,&atx[121]} ,
  {0, "is-protein" ,128,0,0,0,0,0,0,0,NULL,&atx[27],NULL,0,&atx[68]} ,
  {0, "name" ,128,1,0,0,0,0,0,0,NULL,&atx[6],NULL,0,&atx[69]} ,
  {0, "comments" ,128,2,0,1,0,0,0,0,NULL,&atx[38],&atx[70],0,&atx[71]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[6],NULL,0,NULL} ,
  {0, "row-length" ,128,3,0,0,0,0,0,0,NULL,&atx[10],NULL,0,&atx[72]} ,
  {0, "column-length" ,128,4,0,0,0,0,0,0,NULL,&atx[10],NULL,0,&atx[73]} ,
  {0, "scores" ,128,5,0,0,0,0,0,0,NULL,&atx[38],&atx[74],0,&atx[75]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[10],NULL,0,NULL} ,
  {0, "karlinK" ,128,6,0,0,0,0,0,0,NULL,&atx[14],NULL,0,&atx[76]} ,
  {0, "posFreqs" ,128,7,0,1,0,0,0,0,NULL,&atx[38],&atx[77],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[14],NULL,0,NULL} ,
  {0, "return-parts" ,128,6,0,1,0,0,0,0,NULL,&atx[27],NULL,0,&atx[79]} ,
  {0, "query-set" ,128,7,0,1,0,0,0,0,NULL,&atx[80],NULL,0,NULL} ,
  {412, "Bioseq-set" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[97]} ,
  {402, "Blast-request" ,1,0,0,0,0,1,0,0,NULL,&atx[16],&atx[82],0,&atx[101]} ,
  {0, "init" ,128,0,0,0,0,0,0,0,NULL,&atx[6],NULL,0,&atx[83]} ,
  {0, "motd" ,128,1,0,0,0,0,0,0,NULL,&atx[84],NULL,0,&atx[85]} ,
  {305, "NULL" ,0,5,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "db-info" ,128,2,0,0,0,0,0,0,NULL,&atx[84],NULL,0,&atx[86]} ,
  {0, "db-info-specific" ,128,3,0,0,0,0,0,0,NULL,&atx[87],NULL,0,&atx[90]} ,
  {417, "Blast-dbinfo-get" ,1,0,0,0,0,0,0,0,NULL,&atx[63],&atx[88],0,&atx[93]} ,
  {0, "name" ,128,0,0,0,0,0,0,0,NULL,&atx[6],NULL,0,&atx[89]} ,
  {0, "type" ,128,1,0,0,0,0,0,0,NULL,&atx[2],&avnx[5],0,NULL} ,
  {0, "matrix-get" ,128,4,0,0,0,0,0,0,NULL,&atx[6],NULL,0,&atx[91]} ,
  {0, "search" ,128,5,0,0,0,0,0,0,NULL,&atx[0],NULL,0,&atx[92]} ,
  {0, "db-seq-get" ,128,6,0,0,0,0,0,0,NULL,&atx[93],NULL,0,&atx[98]} ,
  {418, "Blast-seq-id" ,1,0,0,0,0,0,0,0,NULL,&atx[63],&atx[94],0,&atx[164]} ,
  {0, "is-protein" ,128,0,0,0,0,0,0,0,NULL,&atx[27],NULL,0,&atx[95]} ,
  {0, "database" ,128,1,0,0,0,0,0,0,NULL,&atx[6],NULL,0,&atx[96]} ,
  {0, "id" ,128,2,0,0,0,0,0,0,NULL,&atx[97],NULL,0,NULL} ,
  {413, "Seq-id" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[59]} ,
  {0, "db-redundant-ids-get" ,128,7,0,0,0,0,0,0,NULL,&atx[93],NULL,0,&atx[99]} ,
  {0, "db-redundant-descr-get" ,128,8,0,0,0,0,0,0,NULL,&atx[93],NULL,0,&atx[100]} ,
  {0, "fini" ,128,9,0,0,0,0,0,0,NULL,&atx[84],NULL,0,NULL} ,
  {403, "Blast-response" ,1,0,0,0,0,1,0,0,NULL,&atx[16],&atx[102],0,&atx[8]} ,
  {0, "init" ,128,0,0,0,0,0,0,0,NULL,&atx[103],NULL,0,&atx[106]} ,
  {424, "Blast-version" ,1,0,0,0,0,0,0,0,NULL,&atx[63],&atx[104],0,&atx[174]} ,
  {0, "version" ,128,0,0,0,0,0,0,0,NULL,&atx[6],NULL,0,&atx[105]} ,
  {0, "date" ,128,1,0,0,0,0,0,0,NULL,&atx[6],NULL,0,NULL} ,
  {0, "motd" ,128,1,0,0,0,0,0,0,NULL,&atx[6],NULL,0,&atx[107]} ,
  {0, "error" ,128,2,0,0,0,0,0,0,NULL,&atx[108],NULL,0,&atx[111]} ,
  {409, "Blast-error" ,1,0,0,0,0,1,0,0,NULL,&atx[63],&atx[109],0,&atx[154]} ,
  {0, "level" ,128,0,0,0,0,0,0,0,NULL,&atx[2],&avnx[8],0,&atx[110]} ,
  {0, "msg" ,128,1,0,1,0,0,0,0,NULL,&atx[6],NULL,0,NULL} ,
  {0, "db-seq-get" ,128,3,0,0,0,0,0,0,NULL,&atx[4],NULL,0,&atx[112]} ,
  {0, "db-redundant-ids-get" ,128,4,0,0,0,0,0,0,NULL,&atx[38],&atx[113],0,&atx[114]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[97],NULL,0,NULL} ,
  {0, "db-redundant-descr-get" ,128,5,0,0,0,0,0,0,NULL,&atx[38],&atx[115],0,&atx[119]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[116],NULL,0,NULL} ,
  {423, "Blast-defline" ,1,0,0,0,0,0,0,0,NULL,&atx[63],&atx[117],0,&atx[103]} ,
  {0, "id" ,128,0,0,0,0,0,0,0,NULL,&atx[97],NULL,0,&atx[118]} ,
  {0, "defline" ,128,1,0,0,0,0,0,0,NULL,&atx[6],NULL,0,NULL} ,
  {0, "db-info" ,128,6,0,0,0,0,0,0,NULL,&atx[38],&atx[120],0,&atx[128]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[121],NULL,0,NULL} ,
  {406, "Blast-dbinfo" ,1,0,0,0,0,1,0,0,NULL,&atx[63],&atx[122],0,&atx[133]} ,
  {0, "is-protein" ,128,0,0,0,0,0,0,0,NULL,&atx[27],NULL,0,&atx[123]} ,
  {0, "name" ,128,1,0,0,0,0,0,0,NULL,&atx[6],NULL,0,&atx[124]} ,
  {0, "definition" ,128,2,0,0,0,0,0,0,NULL,&atx[6],NULL,0,&atx[125]} ,
  {0, "date" ,128,3,0,0,0,0,0,0,NULL,&atx[6],NULL,0,&atx[126]} ,
  {0, "total-length" ,128,4,0,0,0,0,0,0,NULL,&atx[10],NULL,0,&atx[127]} ,
  {0, "number-seqs" ,128,5,0,0,0,0,0,0,NULL,&atx[10],NULL,0,NULL} ,
  {0, "db-info-specific" ,128,7,0,0,0,0,0,0,NULL,&atx[121],NULL,0,&atx[129]} ,
  {0, "matrix" ,128,8,0,0,0,0,0,0,NULL,&atx[66],NULL,0,&atx[130]} ,
  {0, "alignment" ,128,9,0,0,0,0,0,0,NULL,&atx[131],NULL,0,&atx[132]} ,
  {416, "Seq-align-set" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[87]} ,
  {0, "mask" ,128,10,0,0,0,0,0,0,NULL,&atx[133],NULL,0,&atx[137]} ,
  {407, "Blast-mask" ,1,0,0,0,0,1,0,0,NULL,&atx[63],&atx[134],0,&atx[138]} ,
  {0, "location" ,128,0,0,0,0,0,0,0,NULL,&atx[38],&atx[135],0,&atx[136]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[59],NULL,0,NULL} ,
  {0, "frame" ,128,1,0,0,0,0,0,0,NULL,&atx[2],&avnx[13],0,NULL} ,
  {0, "kablk" ,128,11,0,0,0,0,0,0,NULL,&atx[138],NULL,0,&atx[143]} ,
  {408, "Blast-KABlk" ,1,0,0,0,0,1,0,0,NULL,&atx[63],&atx[139],0,&atx[108]} ,
  {0, "lambda" ,128,0,0,0,0,0,0,0,NULL,&atx[14],NULL,0,&atx[140]} ,
  {0, "k" ,128,1,0,0,0,0,0,0,NULL,&atx[14],NULL,0,&atx[141]} ,
  {0, "h" ,128,2,0,0,0,0,0,0,NULL,&atx[14],NULL,0,&atx[142]} ,
  {0, "gapped" ,128,3,0,0,0,0,0,0,NULL,&atx[27],NULL,0,NULL} ,
  {0, "parameters" ,128,12,0,0,0,0,0,0,NULL,&atx[6],NULL,0,&atx[144]} ,
  {0, "queued" ,128,13,0,0,0,0,0,0,NULL,&atx[145],NULL,0,&atx[147]} ,
  {421, "Blast-Queued" ,1,0,0,0,0,0,0,0,NULL,&atx[63],&atx[146],0,&atx[148]} ,
  {0, "length" ,128,0,0,0,0,0,0,0,NULL,&atx[10],NULL,0,NULL} ,
  {0, "start" ,128,14,0,0,0,0,0,0,NULL,&atx[148],NULL,0,&atx[150]} ,
  {422, "Blast-Progress" ,1,0,0,0,0,0,0,0,NULL,&atx[63],&atx[149],0,&atx[116]} ,
  {0, "completed" ,128,0,0,0,0,0,0,0,NULL,&atx[10],NULL,0,NULL} ,
  {0, "progress" ,128,15,0,0,0,0,0,0,NULL,&atx[148],NULL,0,&atx[151]} ,
  {0, "done" ,128,16,0,0,0,0,0,0,NULL,&atx[148],NULL,0,&atx[152]} ,
  {0, "fini" ,128,17,0,0,0,0,0,0,NULL,&atx[84],NULL,0,&atx[153]} ,
  {0, "phialign" ,128,18,0,0,0,0,0,0,NULL,&atx[154],NULL,0,&atx[158]} ,
  {410, "Blast-phialign" ,1,0,0,0,0,1,0,0,NULL,&atx[63],&atx[155],0,&atx[4]} ,
  {0, "numaligns" ,128,0,0,0,0,0,0,0,NULL,&atx[10],NULL,0,&atx[156]} ,
  {0, "seqloc" ,128,1,0,0,0,0,0,0,NULL,&atx[38],&atx[157],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[59],NULL,0,NULL} ,
  {0, "parts" ,128,19,0,0,0,0,0,0,NULL,&atx[38],&atx[159],0,&atx[170]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[160],NULL,0,NULL} ,
  {420, "Blast-parts" ,1,0,0,0,0,0,0,0,NULL,&atx[63],&atx[161],0,&atx[145]} ,
  {0, "defline" ,128,0,0,0,0,0,0,0,NULL,&atx[6],NULL,0,&atx[162]} ,
  {0, "sequence" ,128,1,0,0,0,0,0,0,NULL,&atx[169],&atx[163],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[164],NULL,0,NULL} ,
  {419, "Blast-sequence" ,1,0,0,0,0,0,0,0,NULL,&atx[63],&atx[165],0,&atx[160]} ,
  {0, "align" ,128,0,0,0,0,0,0,0,NULL,&atx[166],NULL,0,&atx[167]} ,
  {415, "Seq-align" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[131]} ,
  {0, "db-seq" ,128,1,0,0,0,0,0,0,NULL,&atx[168],NULL,0,NULL} ,
  {304, "OCTET STRING" ,0,4,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {314, "SET OF" ,0,17,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "mbalign" ,128,20,0,0,0,0,0,0,NULL,&atx[171],NULL,0,NULL} ,
  {426, "MegaBlast-results" ,1,0,0,0,0,0,0,0,NULL,&atx[63],&atx[172],0,NULL} ,
  {0, "mbhits" ,128,0,0,0,0,0,0,0,NULL,&atx[38],&atx[173],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[174],NULL,0,NULL} ,
  {425, "MegaBlast-hit" ,1,0,0,0,0,0,0,0,NULL,&atx[63],&atx[175],0,&atx[171]} ,
  {0, "id1" ,128,0,0,0,0,0,0,0,NULL,&atx[6],NULL,0,&atx[176]} ,
  {0, "id2" ,128,1,0,0,0,0,0,0,NULL,&atx[6],NULL,0,&atx[177]} ,
  {0, "query-offset" ,128,2,0,0,0,0,0,0,NULL,&atx[10],NULL,0,&atx[178]} ,
  {0, "subject-offset" ,128,3,0,0,0,0,0,0,NULL,&atx[10],NULL,0,&atx[179]} ,
  {0, "query-end" ,128,4,0,0,0,0,0,0,NULL,&atx[10],NULL,0,&atx[180]} ,
  {0, "subject-end" ,128,5,0,0,0,0,0,0,NULL,&atx[10],NULL,0,&atx[181]} ,
  {0, "score" ,128,6,0,0,0,0,0,0,NULL,&atx[10],NULL,0,NULL} };

static AsnModule ampx[1] = {
  { "NCBI-Blast" , "blstspc.h22",&atx[0],NULL,NULL,0,0} };

static AsnValxNodePtr avn = avnx;
static AsnTypePtr at = atx;
static AsnModulePtr amp = ampx;



/**************************************************
*
*    Defines for Module NCBI-Blast
*
**************************************************/

#define BLAST_SEARCH &at[0]
#define BLAST_SEARCH_program &at[1]
#define BLAST_SEARCH_query &at[3]
#define BLAST_SEARCH_database &at[5]
#define BLAST_SEARCH_parameters &at[7]
#define BLAST_SEARCH_mask &at[64]
#define BLAST_SEARCH_matrix &at[65]
#define BLAST_SEARCH_return_parts &at[78]
#define BLAST_SEARCH_query_set &at[79]

#define BLAST_REQUEST &at[81]
#define BLAST_REQUEST_init &at[82]
#define BLAST_REQUEST_motd &at[83]
#define BLAST_REQUEST_db_info &at[85]
#define BLAST_REQUEST_db_info_specific &at[86]
#define BLAST_REQUEST_matrix_get &at[90]
#define BLAST_REQUEST_search &at[91]
#define BLAST_REQUEST_db_seq_get &at[92]
#define BLAST_REQUEST_db_redundant_ids_get &at[98]
#define BLAST_REQUEST_db_redundant_descr_get &at[99]
#define BLAST_REQUEST_fini &at[100]

#define BLAST_RESPONSE &at[101]
#define BLAST_RESPONSE_init &at[102]
#define BLAST_RESPONSE_motd &at[106]
#define BLAST_RESPONSE_error &at[107]
#define BLAST_RESPONSE_db_seq_get &at[111]
#define BLAST_RESPONSE_db_redundant_ids_get &at[112]
#define BLAST_RESPONSE_db_redundant_ids_get_E &at[113]
#define BLAST_RESPONSE_db_redundant_descr_get &at[114]
#define BLAST_RESPONSE_db_redundant_descr_get_E &at[115]
#define BLAST_RESPONSE_db_info &at[119]
#define BLAST_RESPONSE_db_info_E &at[120]
#define BLAST_RESPONSE_db_info_specific &at[128]
#define BLAST_RESPONSE_matrix &at[129]
#define BLAST_RESPONSE_alignment &at[130]
#define BLAST_RESPONSE_mask &at[132]
#define BLAST_RESPONSE_kablk &at[137]
#define BLAST_RESPONSE_parameters &at[143]
#define BLAST_RESPONSE_queued &at[144]
#define BLAST_RESPONSE_start &at[147]
#define BLAST_RESPONSE_progress &at[150]
#define BLAST_RESPONSE_done &at[151]
#define BLAST_RESPONSE_fini &at[152]
#define BLAST_RESPONSE_phialign &at[153]
#define BLAST_RESPONSE_parts &at[158]
#define BLAST_RESPONSE_parts_E &at[159]
#define BLAST_RESPONSE_mbalign &at[170]

#define BLAST_PARAMETERS &at[8]
#define BLAST_PARAMETERS_first_threshold &at[9]
#define BLAST_PARAMETERS_second_threshold &at[11]
#define BLAST_PARAMETERS_cutoff &at[12]
#define BLAST_PARAMETERS_cutoff_evalue &at[13]
#define BLAST_PARAMETERS_cutoff_score &at[15]
#define BLAST_PARAMETERS_cutoff2 &at[17]
#define BLAST_PARAMETERS_cutoff2_evalue &at[18]
#define BLAST_PARAMETERS_cutoff2_score &at[19]
#define BLAST_PARAMETERS_hitlist_size &at[20]
#define BLAST_PARAMETERS_nucl_penalty &at[21]
#define BLAST_PARAMETERS_nucl_reward &at[22]
#define BLAST_PARAMETERS_genetic_code &at[23]
#define BLAST_PARAMETERS_db_genetic_code &at[24]
#define BLAST_PARAMETERS_low_complexity_filtering &at[25]
#define BLAST_PARAMETERS_gapped_alignment &at[26]
#define BLAST_PARAMETERS_gap_open &at[28]
#define BLAST_PARAMETERS_gap_extend &at[29]
#define BLAST_PARAMETERS_required_start &at[30]
#define BLAST_PARAMETERS_required_end &at[31]
#define BLAST_PARAMETERS_ethresh &at[32]
#define BLAST_PARAMETERS_max_num_passes &at[33]
#define BLAST_PARAMETERS_pseudo_count_const &at[34]
#define BLAST_PARAMETERS_other_options &at[35]
#define BLAST_PARAMETERS_gilist &at[36]
#define BLAST_PARAMETERS_gilist_E &at[37]
#define BLAST_PARAMETERS_gifile &at[39]
#define BLAST_PARAMETERS_matrix &at[40]
#define BLAST_PARAMETERS_filter_string &at[41]
#define BLAST_PARAMETERS_entrez_query &at[42]
#define BLAST_PARAMETERS_word_size &at[43]
#define BLAST_PARAMETERS_db_length &at[44]
#define BLAST_PARAMETERS_searchsp_eff &at[45]
#define BLAST_PARAMETERS_hsp_range_max &at[46]
#define BLAST_PARAMETERS_block_width &at[47]
#define BLAST_PARAMETERS_perform_culling &at[48]
#define BLAST_PARAMETERS_strand_option &at[49]
#define BLAST_PARAMETERS_phi_pattern &at[50]
#define BLAST_PARAMETERS_use_real_db_size &at[51]
#define BLAST_PARAMETERS_use_best_align &at[52]
#define BLAST_PARAMETERS_is_rps_blast &at[53]
#define BLAST_PARAMETERS_tweak_parameters &at[54]
#define BLAST_PARAMETERS_smith_waterman &at[55]
#define BLAST_PARAMETERS_is_megablast &at[56]
#define BLAST_PARAMETERS_query_lcase_mask &at[57]
#define BLAST_PARAMETERS_query_lcase_mask_E &at[58]
#define BLAST_PARAMETERS_is_ooframe &at[60]
#define BLAST_PARAMETERS_endpoint_results &at[61]
#define BLAST_PARAMETERS_percent_identity &at[62]

#define BLAST_MATRIX &at[66]
#define BLAST_MATRIX_is_protein &at[67]
#define BLAST_MATRIX_name &at[68]
#define BLAST_MATRIX_comments &at[69]
#define BLAST_MATRIX_comments_E &at[70]
#define BLAST_MATRIX_row_length &at[71]
#define BLAST_MATRIX_column_length &at[72]
#define BLAST_MATRIX_scores &at[73]
#define BLAST_MATRIX_scores_E &at[74]
#define BLAST_MATRIX_karlinK &at[75]
#define BLAST_MATRIX_posFreqs &at[76]
#define BLAST_MATRIX_posFreqs_E &at[77]

#define BLAST_DBINFO &at[121]
#define BLAST_DBINFO_is_protein &at[122]
#define BLAST_DBINFO_name &at[123]
#define BLAST_DBINFO_definition &at[124]
#define BLAST_DBINFO_date &at[125]
#define BLAST_DBINFO_total_length &at[126]
#define BLAST_DBINFO_number_seqs &at[127]

#define BLAST_MASK &at[133]
#define BLAST_MASK_location &at[134]
#define BLAST_MASK_location_E &at[135]
#define BLAST_MASK_frame &at[136]

#define BLAST_KABLK &at[138]
#define BLAST_KABLK_lambda &at[139]
#define BLAST_KABLK_k &at[140]
#define BLAST_KABLK_h &at[141]
#define BLAST_KABLK_gapped &at[142]

#define BLAST_ERROR &at[108]
#define BLAST_ERROR_level &at[109]
#define BLAST_ERROR_msg &at[110]

#define BLAST_PHIALIGN &at[154]
#define BLAST_PHIALIGN_numaligns &at[155]
#define BLAST_PHIALIGN_seqloc &at[156]
#define BLAST_PHIALIGN_seqloc_E &at[157]

#define BLAST_DBINFO_GET &at[87]
#define BLAST_DBINFO_GET_name &at[88]
#define BLAST_DBINFO_GET_type &at[89]

#define BLAST_SEQ_ID &at[93]
#define BLAST_SEQ_ID_is_protein &at[94]
#define BLAST_SEQ_ID_database &at[95]
#define BLAST_SEQ_ID_id &at[96]

#define BLAST_SEQUENCE &at[164]
#define BLAST_SEQUENCE_align &at[165]
#define BLAST_SEQUENCE_db_seq &at[167]

#define BLAST_PARTS &at[160]
#define BLAST_PARTS_defline &at[161]
#define BLAST_PARTS_sequence &at[162]
#define BLAST_PARTS_sequence_E &at[163]

#define BLAST_QUEUED &at[145]
#define BLAST_QUEUED_length &at[146]

#define BLAST_PROGRESS &at[148]
#define BLAST_PROGRESS_completed &at[149]

#define BLAST_DEFLINE &at[116]
#define BLAST_DEFLINE_id &at[117]
#define BLAST_DEFLINE_defline &at[118]

#define BLAST_VERSION &at[103]
#define BLAST_VERSION_version &at[104]
#define BLAST_VERSION_date &at[105]

#define MEGABLAST_HIT &at[174]
#define MEGABLAST_HIT_id1 &at[175]
#define MEGABLAST_HIT_id2 &at[176]
#define MEGABLAST_HIT_query_offset &at[177]
#define MEGABLAST_HIT_subject_offset &at[178]
#define MEGABLAST_HIT_query_end &at[179]
#define MEGABLAST_HIT_subject_end &at[180]
#define MEGABLAST_HIT_score &at[181]

#define MEGABLAST_RESULTS &at[171]
#define MEGABLAST_RESULTS_mbhits &at[172]
#define MEGABLAST_RESULTS_mbhits_E &at[173]
