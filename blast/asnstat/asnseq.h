/***********************************************************************
*
**
*        Automatic header module from ASNTOOL
*
************************************************************************/

#ifndef _ASNTOOL_
#include <asn.h>
#endif

static char * asnfilename = "asnseq.h64";
static AsnValxNode avnx[143] = {
    {20,"unknown" ,0,0.0,&avnx[1] } ,
    {20,"genomic" ,1,0.0,&avnx[2] } ,
    {20,"pre-mRNA" ,2,0.0,&avnx[3] } ,
    {20,"mRNA" ,3,0.0,&avnx[4] } ,
    {20,"rRNA" ,4,0.0,&avnx[5] } ,
    {20,"tRNA" ,5,0.0,&avnx[6] } ,
    {20,"snRNA" ,6,0.0,&avnx[7] } ,
    {20,"scRNA" ,7,0.0,&avnx[8] } ,
    {20,"peptide" ,8,0.0,&avnx[9] } ,
    {20,"other-genetic" ,9,0.0,&avnx[10] } ,
    {20,"genomic-mRNA" ,10,0.0,&avnx[11] } ,
    {20,"other" ,255,0.0,NULL } ,
    {20,"dna" ,0,0.0,&avnx[13] } ,
    {20,"rna" ,1,0.0,&avnx[14] } ,
    {20,"extrachrom" ,2,0.0,&avnx[15] } ,
    {20,"plasmid" ,3,0.0,&avnx[16] } ,
    {20,"mitochondrial" ,4,0.0,&avnx[17] } ,
    {20,"chloroplast" ,5,0.0,&avnx[18] } ,
    {20,"kinetoplast" ,6,0.0,&avnx[19] } ,
    {20,"cyanelle" ,7,0.0,&avnx[20] } ,
    {20,"synthetic" ,8,0.0,&avnx[21] } ,
    {20,"recombinant" ,9,0.0,&avnx[22] } ,
    {20,"partial" ,10,0.0,&avnx[23] } ,
    {20,"complete" ,11,0.0,&avnx[24] } ,
    {20,"mutagen" ,12,0.0,&avnx[25] } ,
    {20,"natmut" ,13,0.0,&avnx[26] } ,
    {20,"transposon" ,14,0.0,&avnx[27] } ,
    {20,"insertion-seq" ,15,0.0,&avnx[28] } ,
    {20,"no-left" ,16,0.0,&avnx[29] } ,
    {20,"no-right" ,17,0.0,&avnx[30] } ,
    {20,"macronuclear" ,18,0.0,&avnx[31] } ,
    {20,"proviral" ,19,0.0,&avnx[32] } ,
    {20,"est" ,20,0.0,&avnx[33] } ,
    {20,"sts" ,21,0.0,&avnx[34] } ,
    {20,"survey" ,22,0.0,&avnx[35] } ,
    {20,"chromoplast" ,23,0.0,&avnx[36] } ,
    {20,"genemap" ,24,0.0,&avnx[37] } ,
    {20,"restmap" ,25,0.0,&avnx[38] } ,
    {20,"physmap" ,26,0.0,&avnx[39] } ,
    {20,"other" ,255,0.0,NULL } ,
    {20,"concept-trans" ,1,0.0,&avnx[41] } ,
    {20,"seq-pept" ,2,0.0,&avnx[42] } ,
    {20,"both" ,3,0.0,&avnx[43] } ,
    {20,"seq-pept-overlap" ,4,0.0,&avnx[44] } ,
    {20,"seq-pept-homol" ,5,0.0,&avnx[45] } ,
    {20,"concept-trans-a" ,6,0.0,&avnx[46] } ,
    {20,"other" ,255,0.0,NULL } ,
    {3,NULL,1,0.0,NULL } ,
    {2,NULL,0,0.0,NULL } ,
    {2,NULL,1,0.0,NULL } ,
    {20,"not-set" ,0,0.0,&avnx[51] } ,
    {20,"sources" ,1,0.0,&avnx[52] } ,
    {20,"aligns" ,2,0.0,NULL } ,
    {20,"seq" ,0,0.0,&avnx[54] } ,
    {20,"sites" ,1,0.0,&avnx[55] } ,
    {20,"feats" ,2,0.0,&avnx[56] } ,
    {20,"no-target" ,3,0.0,NULL } ,
    {3,NULL,0,0.0,NULL } ,
    {20,"unknown" ,0,0.0,&avnx[59] } ,
    {20,"genomic" ,1,0.0,&avnx[60] } ,
    {20,"pre-RNA" ,2,0.0,&avnx[61] } ,
    {20,"mRNA" ,3,0.0,&avnx[62] } ,
    {20,"rRNA" ,4,0.0,&avnx[63] } ,
    {20,"tRNA" ,5,0.0,&avnx[64] } ,
    {20,"snRNA" ,6,0.0,&avnx[65] } ,
    {20,"scRNA" ,7,0.0,&avnx[66] } ,
    {20,"peptide" ,8,0.0,&avnx[67] } ,
    {20,"other-genetic" ,9,0.0,&avnx[68] } ,
    {20,"genomic-mRNA" ,10,0.0,&avnx[69] } ,
    {20,"cRNA" ,11,0.0,&avnx[70] } ,
    {20,"snoRNA" ,12,0.0,&avnx[71] } ,
    {20,"transcribed-RNA" ,13,0.0,&avnx[72] } ,
    {20,"other" ,255,0.0,NULL } ,
    {3,NULL,0,0.0,NULL } ,
    {20,"unknown" ,0,0.0,&avnx[75] } ,
    {20,"standard" ,1,0.0,&avnx[76] } ,
    {20,"est" ,2,0.0,&avnx[77] } ,
    {20,"sts" ,3,0.0,&avnx[78] } ,
    {20,"survey" ,4,0.0,&avnx[79] } ,
    {20,"genemap" ,5,0.0,&avnx[80] } ,
    {20,"physmap" ,6,0.0,&avnx[81] } ,
    {20,"derived" ,7,0.0,&avnx[82] } ,
    {20,"concept-trans" ,8,0.0,&avnx[83] } ,
    {20,"seq-pept" ,9,0.0,&avnx[84] } ,
    {20,"both" ,10,0.0,&avnx[85] } ,
    {20,"seq-pept-overlap" ,11,0.0,&avnx[86] } ,
    {20,"seq-pept-homol" ,12,0.0,&avnx[87] } ,
    {20,"concept-trans-a" ,13,0.0,&avnx[88] } ,
    {20,"htgs-1" ,14,0.0,&avnx[89] } ,
    {20,"htgs-2" ,15,0.0,&avnx[90] } ,
    {20,"htgs-3" ,16,0.0,&avnx[91] } ,
    {20,"fli-cdna" ,17,0.0,&avnx[92] } ,
    {20,"htgs-0" ,18,0.0,&avnx[93] } ,
    {20,"htc" ,19,0.0,&avnx[94] } ,
    {20,"other" ,255,0.0,NULL } ,
    {3,NULL,0,0.0,NULL } ,
    {20,"unknown" ,0,0.0,&avnx[97] } ,
    {20,"complete" ,1,0.0,&avnx[98] } ,
    {20,"partial" ,2,0.0,&avnx[99] } ,
    {20,"no-left" ,3,0.0,&avnx[100] } ,
    {20,"no-right" ,4,0.0,&avnx[101] } ,
    {20,"no-ends" ,5,0.0,&avnx[102] } ,
    {20,"other" ,255,0.0,NULL } ,
    {3,NULL,0,0.0,NULL } ,
    {20,"not-set" ,0,0.0,&avnx[105] } ,
    {20,"virtual" ,1,0.0,&avnx[106] } ,
    {20,"raw" ,2,0.0,&avnx[107] } ,
    {20,"seg" ,3,0.0,&avnx[108] } ,
    {20,"const" ,4,0.0,&avnx[109] } ,
    {20,"ref" ,5,0.0,&avnx[110] } ,
    {20,"consen" ,6,0.0,&avnx[111] } ,
    {20,"map" ,7,0.0,&avnx[112] } ,
    {20,"delta" ,8,0.0,&avnx[113] } ,
    {20,"other" ,255,0.0,NULL } ,
    {20,"not-set" ,0,0.0,&avnx[115] } ,
    {20,"dna" ,1,0.0,&avnx[116] } ,
    {20,"rna" ,2,0.0,&avnx[117] } ,
    {20,"aa" ,3,0.0,&avnx[118] } ,
    {20,"na" ,4,0.0,&avnx[119] } ,
    {20,"other" ,255,0.0,NULL } ,
    {20,"not-set" ,0,0.0,&avnx[121] } ,
    {20,"linear" ,1,0.0,&avnx[122] } ,
    {20,"circular" ,2,0.0,&avnx[123] } ,
    {20,"tandem" ,3,0.0,&avnx[124] } ,
    {20,"other" ,255,0.0,NULL } ,
    {3,NULL,1,0.0,NULL } ,
    {20,"not-set" ,0,0.0,&avnx[127] } ,
    {20,"ss" ,1,0.0,&avnx[128] } ,
    {20,"ds" ,2,0.0,&avnx[129] } ,
    {20,"mixed" ,3,0.0,&avnx[130] } ,
    {20,"other" ,255,0.0,NULL } ,
    {20,"genbank" ,1,0.0,&avnx[132] } ,
    {20,"embl" ,2,0.0,&avnx[133] } ,
    {20,"ddbj" ,3,0.0,&avnx[134] } ,
    {20,"pir" ,4,0.0,&avnx[135] } ,
    {20,"sp" ,5,0.0,&avnx[136] } ,
    {20,"bbone" ,6,0.0,&avnx[137] } ,
    {20,"pdb" ,7,0.0,&avnx[138] } ,
    {20,"other" ,255,0.0,NULL } ,
    {20,"ref" ,1,0.0,&avnx[140] } ,
    {20,"alt" ,2,0.0,&avnx[141] } ,
    {20,"blocks" ,3,0.0,&avnx[142] } ,
    {20,"other" ,255,0.0,NULL } };

static AsnType atx[206] = {
  {401, "Bioseq" ,1,0,0,0,0,1,0,0,NULL,&atx[32],&atx[1],0,&atx[166]} ,
  {0, "id" ,128,0,0,0,0,0,0,0,NULL,&atx[4],&atx[2],0,&atx[5]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[3],NULL,0,NULL} ,
  {421, "Seq-id" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[134]} ,
  {314, "SET OF" ,0,17,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "descr" ,128,1,0,1,0,0,0,0,NULL,&atx[6],NULL,0,&atx[97]} ,
  {404, "Seq-descr" ,1,0,0,0,0,1,0,0,NULL,&atx[4],&atx[7],0,&atx[8]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[8],NULL,0,NULL} ,
  {405, "Seqdesc" ,1,0,0,0,0,1,0,0,NULL,&atx[50],&atx[9],0,&atx[24]} ,
  {0, "mol-type" ,128,0,0,0,0,0,0,0,NULL,&atx[10],NULL,0,&atx[12]} ,
  {409, "GIBB-mol" ,1,0,0,0,0,1,0,0,NULL,&atx[11],&avnx[0],0,&atx[81]} ,
  {310, "ENUMERATED" ,0,10,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "modif" ,128,1,0,0,0,0,0,0,NULL,&atx[4],&atx[13],0,&atx[15]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[14],NULL,0,NULL} ,
  {430, "GIBB-mod" ,1,0,0,0,0,0,0,0,NULL,&atx[11],&avnx[12],0,&atx[16]} ,
  {0, "method" ,128,2,0,0,0,0,0,0,NULL,&atx[16],NULL,0,&atx[17]} ,
  {431, "GIBB-method" ,1,0,0,0,0,0,0,0,NULL,&atx[11],&avnx[40],0,&atx[92]} ,
  {0, "name" ,128,3,0,0,0,0,0,0,NULL,&atx[18],NULL,0,&atx[19]} ,
  {323, "VisibleString" ,0,26,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "title" ,128,4,0,0,0,0,0,0,NULL,&atx[18],NULL,0,&atx[20]} ,
  {0, "org" ,128,5,0,0,0,0,0,0,NULL,&atx[21],NULL,0,&atx[22]} ,
  {419, "Org-ref" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[90]} ,
  {0, "comment" ,128,6,0,0,0,0,0,0,NULL,&atx[18],NULL,0,&atx[23]} ,
  {0, "num" ,128,7,0,0,0,0,0,0,NULL,&atx[24],NULL,0,&atx[51]} ,
  {406, "Numbering" ,1,0,0,0,0,1,0,0,NULL,&atx[50],&atx[25],0,&atx[88]} ,
  {0, "cont" ,128,0,0,0,0,0,0,0,NULL,&atx[26],NULL,0,&atx[33]} ,
  {433, "Num-cont" ,1,0,0,0,0,0,0,0,NULL,&atx[32],&atx[27],0,&atx[34]} ,
  {0, "refnum" ,128,0,0,0,1,0,0,0,&avnx[47],&atx[28],NULL,0,&atx[29]} ,
  {302, "INTEGER" ,0,2,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "has-zero" ,128,1,0,0,1,0,0,0,&avnx[48],&atx[30],NULL,0,&atx[31]} ,
  {301, "BOOLEAN" ,0,1,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "ascending" ,128,2,0,0,1,0,0,0,&avnx[49],&atx[30],NULL,0,NULL} ,
  {311, "SEQUENCE" ,0,16,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "enum" ,128,1,0,0,0,0,0,0,NULL,&atx[34],NULL,0,&atx[39]} ,
  {434, "Num-enum" ,1,0,0,0,0,0,0,0,NULL,&atx[32],&atx[35],0,&atx[40]} ,
  {0, "num" ,128,0,0,0,0,0,0,0,NULL,&atx[28],NULL,0,&atx[36]} ,
  {0, "names" ,128,1,0,0,0,0,0,0,NULL,&atx[38],&atx[37],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[18],NULL,0,NULL} ,
  {312, "SEQUENCE OF" ,0,16,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "ref" ,128,2,0,0,0,0,0,0,NULL,&atx[40],NULL,0,&atx[44]} ,
  {435, "Num-ref" ,1,0,0,0,0,0,0,0,NULL,&atx[32],&atx[41],0,&atx[45]} ,
  {0, "type" ,128,0,0,0,0,0,0,0,NULL,&atx[11],&avnx[50],0,&atx[42]} ,
  {0, "aligns" ,128,1,0,1,0,0,0,0,NULL,&atx[43],NULL,0,NULL} ,
  {415, "Seq-align" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[140]} ,
  {0, "real" ,128,3,0,0,0,0,0,0,NULL,&atx[45],NULL,0,NULL} ,
  {436, "Num-real" ,1,0,0,0,0,0,0,0,NULL,&atx[32],&atx[46],0,&atx[107]} ,
  {0, "a" ,128,0,0,0,0,0,0,0,NULL,&atx[47],NULL,0,&atx[48]} ,
  {309, "REAL" ,0,9,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "b" ,128,1,0,0,0,0,0,0,NULL,&atx[47],NULL,0,&atx[49]} ,
  {0, "units" ,128,2,0,1,0,0,0,0,NULL,&atx[18],NULL,0,NULL} ,
  {315, "CHOICE" ,0,-1,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "maploc" ,128,8,0,0,0,0,0,0,NULL,&atx[52],NULL,0,&atx[53]} ,
  {412, "Dbtag" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[171]} ,
  {0, "pir" ,128,9,0,0,0,0,0,0,NULL,&atx[54],NULL,0,&atx[55]} ,
  {424, "PIR-block" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[79]} ,
  {0, "genbank" ,128,10,0,0,0,0,0,0,NULL,&atx[56],NULL,0,&atx[57]} ,
  {423, "GB-block" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[54]} ,
  {0, "pub" ,128,11,0,0,0,0,0,0,NULL,&atx[58],NULL,0,&atx[72]} ,
  {403, "Pubdesc" ,1,0,0,0,0,1,0,0,NULL,&atx[32],&atx[59],0,&atx[6]} ,
  {0, "pub" ,128,0,0,0,0,0,0,0,NULL,&atx[60],NULL,0,&atx[61]} ,
  {418, "Pub-equiv" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[21]} ,
  {0, "name" ,128,1,0,1,0,0,0,0,NULL,&atx[18],NULL,0,&atx[62]} ,
  {0, "fig" ,128,2,0,1,0,0,0,0,NULL,&atx[18],NULL,0,&atx[63]} ,
  {0, "num" ,128,3,0,1,0,0,0,0,NULL,&atx[24],NULL,0,&atx[64]} ,
  {0, "numexc" ,128,4,0,1,0,0,0,0,NULL,&atx[30],NULL,0,&atx[65]} ,
  {0, "poly-a" ,128,5,0,1,0,0,0,0,NULL,&atx[30],NULL,0,&atx[66]} ,
  {0, "maploc" ,128,6,0,1,0,0,0,0,NULL,&atx[18],NULL,0,&atx[67]} ,
  {0, "seq-raw" ,128,7,0,1,0,0,0,0,NULL,&atx[68],NULL,0,&atx[69]} ,
  {351, "StringStore" ,64,1,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "align-group" ,128,8,0,1,0,0,0,0,NULL,&atx[28],NULL,0,&atx[70]} ,
  {0, "comment" ,128,9,0,1,0,0,0,0,NULL,&atx[18],NULL,0,&atx[71]} ,
  {0, "reftype" ,128,10,0,0,1,0,0,0,&avnx[57],&atx[28],&avnx[53],0,NULL} ,
  {0, "region" ,128,12,0,0,0,0,0,0,NULL,&atx[18],NULL,0,&atx[73]} ,
  {0, "user" ,128,13,0,0,0,0,0,0,NULL,&atx[74],NULL,0,&atx[75]} ,
  {414, "User-object" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[43]} ,
  {0, "sp" ,128,14,0,0,0,0,0,0,NULL,&atx[76],NULL,0,&atx[77]} ,
  {426, "SP-block" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[84]} ,
  {0, "dbxref" ,128,15,0,0,0,0,0,0,NULL,&atx[52],NULL,0,&atx[78]} ,
  {0, "embl" ,128,16,0,0,0,0,0,0,NULL,&atx[79],NULL,0,&atx[80]} ,
  {425, "EMBL-block" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[76]} ,
  {0, "create-date" ,128,17,0,0,0,0,0,0,NULL,&atx[81],NULL,0,&atx[82]} ,
  {410, "Date" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[103]} ,
  {0, "update-date" ,128,18,0,0,0,0,0,0,NULL,&atx[81],NULL,0,&atx[83]} ,
  {0, "prf" ,128,19,0,0,0,0,0,0,NULL,&atx[84],NULL,0,&atx[85]} ,
  {427, "PRF-block" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[86]} ,
  {0, "pdb" ,128,20,0,0,0,0,0,0,NULL,&atx[86],NULL,0,&atx[87]} ,
  {428, "PDB-block" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[98]} ,
  {0, "het" ,128,21,0,0,0,0,0,0,NULL,&atx[88],NULL,0,&atx[89]} ,
  {407, "Heterogen" ,1,0,0,0,0,1,0,0,NULL,&atx[18],NULL,0,&atx[152]} ,
  {0, "source" ,128,22,0,0,0,0,0,0,NULL,&atx[90],NULL,0,&atx[91]} ,
  {420, "BioSource" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[3]} ,
  {0, "molinfo" ,128,23,0,0,0,0,0,0,NULL,&atx[92],NULL,0,NULL} ,
  {432, "MolInfo" ,1,0,0,0,0,0,0,0,NULL,&atx[32],&atx[93],0,&atx[26]} ,
  {0, "biomol" ,128,0,0,0,1,0,0,0,&avnx[73],&atx[28],&avnx[58],0,&atx[94]} ,
  {0, "tech" ,128,1,0,0,1,0,0,0,&avnx[95],&atx[28],&avnx[74],0,&atx[95]} ,
  {0, "techexp" ,128,2,0,1,0,0,0,0,NULL,&atx[18],NULL,0,&atx[96]} ,
  {0, "completeness" ,128,3,0,0,1,0,0,0,&avnx[103],&atx[28],&avnx[96],0,NULL} ,
  {0, "inst" ,128,2,0,0,0,0,0,0,NULL,&atx[98],NULL,0,&atx[164]} ,
  {429, "Seq-inst" ,1,0,0,0,0,0,0,0,NULL,&atx[32],&atx[99],0,&atx[14]} ,
  {0, "repr" ,128,0,0,0,0,0,0,0,NULL,&atx[11],&avnx[104],0,&atx[100]} ,
  {0, "mol" ,128,1,0,0,0,0,0,0,NULL,&atx[11],&avnx[114],0,&atx[101]} ,
  {0, "length" ,128,2,0,1,0,0,0,0,NULL,&atx[28],NULL,0,&atx[102]} ,
  {0, "fuzz" ,128,3,0,1,0,0,0,0,NULL,&atx[103],NULL,0,&atx[104]} ,
  {411, "Int-fuzz" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[52]} ,
  {0, "topology" ,128,4,0,0,1,0,0,0,&avnx[125],&atx[11],&avnx[120],0,&atx[105]} ,
  {0, "strand" ,128,5,0,1,0,0,0,0,NULL,&atx[11],&avnx[126],0,&atx[106]} ,
  {0, "seq-data" ,128,6,0,1,0,0,0,0,NULL,&atx[107],NULL,0,&atx[129]} ,
  {437, "Seq-data" ,1,0,0,0,0,0,0,0,NULL,&atx[50],&atx[108],0,&atx[130]} ,
  {0, "iupacna" ,128,0,0,0,0,0,0,0,NULL,&atx[109],NULL,0,&atx[110]} ,
  {446, "IUPACna" ,1,0,0,0,0,0,0,0,NULL,&atx[68],NULL,0,&atx[111]} ,
  {0, "iupacaa" ,128,1,0,0,0,0,0,0,NULL,&atx[111],NULL,0,&atx[112]} ,
  {447, "IUPACaa" ,1,0,0,0,0,0,0,0,NULL,&atx[68],NULL,0,&atx[113]} ,
  {0, "ncbi2na" ,128,2,0,0,0,0,0,0,NULL,&atx[113],NULL,0,&atx[115]} ,
  {448, "NCBI2na" ,1,0,0,0,0,0,0,0,NULL,&atx[114],NULL,0,&atx[116]} ,
  {304, "OCTET STRING" ,0,4,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "ncbi4na" ,128,3,0,0,0,0,0,0,NULL,&atx[116],NULL,0,&atx[117]} ,
  {449, "NCBI4na" ,1,0,0,0,0,0,0,0,NULL,&atx[114],NULL,0,&atx[118]} ,
  {0, "ncbi8na" ,128,4,0,0,0,0,0,0,NULL,&atx[118],NULL,0,&atx[119]} ,
  {450, "NCBI8na" ,1,0,0,0,0,0,0,0,NULL,&atx[114],NULL,0,&atx[120]} ,
  {0, "ncbipna" ,128,5,0,0,0,0,0,0,NULL,&atx[120],NULL,0,&atx[121]} ,
  {451, "NCBIpna" ,1,0,0,0,0,0,0,0,NULL,&atx[114],NULL,0,&atx[122]} ,
  {0, "ncbi8aa" ,128,6,0,0,0,0,0,0,NULL,&atx[122],NULL,0,&atx[123]} ,
  {452, "NCBI8aa" ,1,0,0,0,0,0,0,0,NULL,&atx[114],NULL,0,&atx[124]} ,
  {0, "ncbieaa" ,128,7,0,0,0,0,0,0,NULL,&atx[124],NULL,0,&atx[125]} ,
  {453, "NCBIeaa" ,1,0,0,0,0,0,0,0,NULL,&atx[68],NULL,0,&atx[126]} ,
  {0, "ncbipaa" ,128,8,0,0,0,0,0,0,NULL,&atx[126],NULL,0,&atx[127]} ,
  {454, "NCBIpaa" ,1,0,0,0,0,0,0,0,NULL,&atx[114],NULL,0,&atx[128]} ,
  {0, "ncbistdaa" ,128,9,0,0,0,0,0,0,NULL,&atx[128],NULL,0,NULL} ,
  {455, "NCBIstdaa" ,1,0,0,0,0,0,0,0,NULL,&atx[114],NULL,0,&atx[169]} ,
  {0, "ext" ,128,7,0,1,0,0,0,0,NULL,&atx[130],NULL,0,&atx[151]} ,
  {438, "Seq-ext" ,1,0,0,0,0,0,0,0,NULL,&atx[50],&atx[131],0,&atx[132]} ,
  {0, "seg" ,128,0,0,0,0,0,0,0,NULL,&atx[132],NULL,0,&atx[135]} ,
  {439, "Seg-ext" ,1,0,0,0,0,0,0,0,NULL,&atx[38],&atx[133],0,&atx[136]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[134],NULL,0,NULL} ,
  {422, "Seq-loc" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[56]} ,
  {0, "ref" ,128,1,0,0,0,0,0,0,NULL,&atx[136],NULL,0,&atx[137]} ,
  {440, "Ref-ext" ,1,0,0,0,0,0,0,0,NULL,&atx[134],NULL,0,&atx[138]} ,
  {0, "map" ,128,2,0,0,0,0,0,0,NULL,&atx[138],NULL,0,&atx[141]} ,
  {441, "Map-ext" ,1,0,0,0,0,0,0,0,NULL,&atx[38],&atx[139],0,&atx[142]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[140],NULL,0,NULL} ,
  {416, "Seq-feat" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[201]} ,
  {0, "delta" ,128,3,0,0,0,0,0,0,NULL,&atx[142],NULL,0,NULL} ,
  {442, "Delta-ext" ,1,0,0,0,0,0,0,0,NULL,&atx[38],&atx[143],0,&atx[144]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[144],NULL,0,NULL} ,
  {443, "Delta-seq" ,1,0,0,0,0,0,0,0,NULL,&atx[50],&atx[145],0,&atx[147]} ,
  {0, "loc" ,128,0,0,0,0,0,0,0,NULL,&atx[134],NULL,0,&atx[146]} ,
  {0, "literal" ,128,1,0,0,0,0,0,0,NULL,&atx[147],NULL,0,NULL} ,
  {444, "Seq-literal" ,1,0,0,0,0,0,0,0,NULL,&atx[32],&atx[148],0,&atx[156]} ,
  {0, "length" ,128,0,0,0,0,0,0,0,NULL,&atx[28],NULL,0,&atx[149]} ,
  {0, "fuzz" ,128,1,0,1,0,0,0,0,NULL,&atx[103],NULL,0,&atx[150]} ,
  {0, "seq-data" ,128,2,0,1,0,0,0,0,NULL,&atx[107],NULL,0,NULL} ,
  {0, "hist" ,128,8,0,1,0,0,0,0,NULL,&atx[152],NULL,0,NULL} ,
  {408, "Seq-hist" ,1,0,0,0,0,1,0,0,NULL,&atx[32],&atx[153],0,&atx[10]} ,
  {0, "assembly" ,128,0,0,1,0,0,0,0,NULL,&atx[4],&atx[154],0,&atx[155]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[43],NULL,0,NULL} ,
  {0, "replaces" ,128,1,0,1,0,0,0,0,NULL,&atx[156],NULL,0,&atx[160]} ,
  {445, "Seq-hist-rec" ,1,0,0,0,0,0,0,0,NULL,&atx[32],&atx[157],0,&atx[109]} ,
  {0, "date" ,128,0,0,1,0,0,0,0,NULL,&atx[81],NULL,0,&atx[158]} ,
  {0, "ids" ,128,1,0,0,0,0,0,0,NULL,&atx[4],&atx[159],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[3],NULL,0,NULL} ,
  {0, "replaced-by" ,128,2,0,1,0,0,0,0,NULL,&atx[156],NULL,0,&atx[161]} ,
  {0, "deleted" ,128,3,0,1,0,0,0,0,NULL,&atx[50],&atx[162],0,NULL} ,
  {0, "bool" ,128,0,0,0,0,0,0,0,NULL,&atx[30],NULL,0,&atx[163]} ,
  {0, "date" ,128,1,0,0,0,0,0,0,NULL,&atx[81],NULL,0,NULL} ,
  {0, "annot" ,128,3,0,1,0,0,0,0,NULL,&atx[4],&atx[165],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[166],NULL,0,NULL} ,
  {402, "Seq-annot" ,1,0,0,0,0,1,0,0,NULL,&atx[32],&atx[167],0,&atx[58]} ,
  {0, "id" ,128,0,0,1,0,0,0,0,NULL,&atx[4],&atx[168],0,&atx[174]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[169],NULL,0,NULL} ,
  {456, "Annot-id" ,1,0,0,0,0,0,0,0,NULL,&atx[50],&atx[170],0,&atx[177]} ,
  {0, "local" ,128,0,0,0,0,0,0,0,NULL,&atx[171],NULL,0,&atx[172]} ,
  {413, "Object-id" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[74]} ,
  {0, "ncbi" ,128,1,0,0,0,0,0,0,NULL,&atx[28],NULL,0,&atx[173]} ,
  {0, "general" ,128,2,0,0,0,0,0,0,NULL,&atx[52],NULL,0,NULL} ,
  {0, "db" ,128,1,0,1,0,0,0,0,NULL,&atx[28],&avnx[131],0,&atx[175]} ,
  {0, "name" ,128,2,0,1,0,0,0,0,NULL,&atx[18],NULL,0,&atx[176]} ,
  {0, "desc" ,128,3,0,1,0,0,0,0,NULL,&atx[177],NULL,0,&atx[194]} ,
  {457, "Annot-descr" ,1,0,0,0,0,0,0,0,NULL,&atx[4],&atx[178],0,&atx[179]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[179],NULL,0,NULL} ,
  {458, "Annotdesc" ,1,0,0,0,0,0,0,0,NULL,&atx[50],&atx[180],0,&atx[189]} ,
  {0, "name" ,128,0,0,0,0,0,0,0,NULL,&atx[18],NULL,0,&atx[181]} ,
  {0, "title" ,128,1,0,0,0,0,0,0,NULL,&atx[18],NULL,0,&atx[182]} ,
  {0, "comment" ,128,2,0,0,0,0,0,0,NULL,&atx[18],NULL,0,&atx[183]} ,
  {0, "pub" ,128,3,0,0,0,0,0,0,NULL,&atx[58],NULL,0,&atx[184]} ,
  {0, "user" ,128,4,0,0,0,0,0,0,NULL,&atx[74],NULL,0,&atx[185]} ,
  {0, "create-date" ,128,5,0,0,0,0,0,0,NULL,&atx[81],NULL,0,&atx[186]} ,
  {0, "update-date" ,128,6,0,0,0,0,0,0,NULL,&atx[81],NULL,0,&atx[187]} ,
  {0, "src" ,128,7,0,0,0,0,0,0,NULL,&atx[3],NULL,0,&atx[188]} ,
  {0, "align" ,128,8,0,0,0,0,0,0,NULL,&atx[189],NULL,0,&atx[193]} ,
  {459, "Align-def" ,1,0,0,0,0,0,0,0,NULL,&atx[32],&atx[190],0,NULL} ,
  {0, "align-type" ,128,0,0,0,0,0,0,0,NULL,&atx[28],&avnx[139],0,&atx[191]} ,
  {0, "ids" ,128,1,0,1,0,0,0,0,NULL,&atx[4],&atx[192],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[3],NULL,0,NULL} ,
  {0, "region" ,128,9,0,0,0,0,0,0,NULL,&atx[134],NULL,0,NULL} ,
  {0, "data" ,128,4,0,0,0,0,0,0,NULL,&atx[50],&atx[195],0,NULL} ,
  {0, "ftable" ,128,0,0,0,0,0,0,0,NULL,&atx[4],&atx[196],0,&atx[197]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[140],NULL,0,NULL} ,
  {0, "align" ,128,1,0,0,0,0,0,0,NULL,&atx[4],&atx[198],0,&atx[199]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[43],NULL,0,NULL} ,
  {0, "graph" ,128,2,0,0,0,0,0,0,NULL,&atx[4],&atx[200],0,&atx[202]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[201],NULL,0,NULL} ,
  {417, "Seq-graph" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[60]} ,
  {0, "ids" ,128,3,0,0,0,0,0,0,NULL,&atx[4],&atx[203],0,&atx[204]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[3],NULL,0,NULL} ,
  {0, "locs" ,128,4,0,0,0,0,0,0,NULL,&atx[4],&atx[205],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[134],NULL,0,NULL} };

static AsnModule ampx[1] = {
  { "NCBI-Sequence" , "asnseq.h64",&atx[0],NULL,NULL,0,0} };

static AsnValxNodePtr avn = avnx;
static AsnTypePtr at = atx;
static AsnModulePtr amp = ampx;



/**************************************************
*
*    Defines for Module NCBI-Sequence
*
**************************************************/

#define BIOSEQ &at[0]
#define BIOSEQ_id &at[1]
#define BIOSEQ_id_E &at[2]
#define BIOSEQ_descr &at[5]
#define BIOSEQ_inst &at[97]
#define BIOSEQ_annot &at[164]
#define BIOSEQ_annot_E &at[165]

#define SEQ_ANNOT &at[166]
#define SEQ_ANNOT_id &at[167]
#define SEQ_ANNOT_id_E &at[168]
#define SEQ_ANNOT_db &at[174]
#define SEQ_ANNOT_name &at[175]
#define SEQ_ANNOT_desc &at[176]
#define SEQ_ANNOT_data &at[194]
#define SEQ_ANNOT_data_ftable &at[195]
#define SEQ_ANNOT_data_ftable_E &at[196]
#define SEQ_ANNOT_data_align &at[197]
#define SEQ_ANNOT_data_align_E &at[198]
#define SEQ_ANNOT_data_graph &at[199]
#define SEQ_ANNOT_data_graph_E &at[200]
#define SEQ_ANNOT_data_ids &at[202]
#define SEQ_ANNOT_data_ids_E &at[203]
#define SEQ_ANNOT_data_locs &at[204]
#define SEQ_ANNOT_data_locs_E &at[205]

#define PUBDESC &at[58]
#define PUBDESC_pub &at[59]
#define PUBDESC_name &at[61]
#define PUBDESC_fig &at[62]
#define PUBDESC_num &at[63]
#define PUBDESC_numexc &at[64]
#define PUBDESC_poly_a &at[65]
#define PUBDESC_maploc &at[66]
#define PUBDESC_seq_raw &at[67]
#define PUBDESC_align_group &at[69]
#define PUBDESC_comment &at[70]
#define PUBDESC_reftype &at[71]

#define SEQ_DESCR &at[6]
#define SEQ_DESCR_E &at[7]

#define SEQDESC &at[8]
#define SEQDESC_mol_type &at[9]
#define SEQDESC_modif &at[12]
#define SEQDESC_modif_E &at[13]
#define SEQDESC_method &at[15]
#define SEQDESC_name &at[17]
#define SEQDESC_title &at[19]
#define SEQDESC_org &at[20]
#define SEQDESC_comment &at[22]
#define SEQDESC_num &at[23]
#define SEQDESC_maploc &at[51]
#define SEQDESC_pir &at[53]
#define SEQDESC_genbank &at[55]
#define SEQDESC_pub &at[57]
#define SEQDESC_region &at[72]
#define SEQDESC_user &at[73]
#define SEQDESC_sp &at[75]
#define SEQDESC_dbxref &at[77]
#define SEQDESC_embl &at[78]
#define SEQDESC_create_date &at[80]
#define SEQDESC_update_date &at[82]
#define SEQDESC_prf &at[83]
#define SEQDESC_pdb &at[85]
#define SEQDESC_het &at[87]
#define SEQDESC_source &at[89]
#define SEQDESC_molinfo &at[91]

#define NUMBERING &at[24]
#define NUMBERING_cont &at[25]
#define NUMBERING_enum &at[33]
#define NUMBERING_ref &at[39]
#define NUMBERING_real &at[44]

#define HETEROGEN &at[88]

#define SEQ_HIST &at[152]
#define SEQ_HIST_assembly &at[153]
#define SEQ_HIST_assembly_E &at[154]
#define SEQ_HIST_replaces &at[155]
#define SEQ_HIST_replaced_by &at[160]
#define SEQ_HIST_deleted &at[161]
#define SEQ_HIST_deleted_bool &at[162]
#define SEQ_HIST_deleted_date &at[163]

#define GIBB_MOL &at[10]

#define SEQ_INST &at[98]
#define SEQ_INST_repr &at[99]
#define SEQ_INST_mol &at[100]
#define SEQ_INST_length &at[101]
#define SEQ_INST_fuzz &at[102]
#define SEQ_INST_topology &at[104]
#define SEQ_INST_strand &at[105]
#define SEQ_INST_seq_data &at[106]
#define SEQ_INST_ext &at[129]
#define SEQ_INST_hist &at[151]

#define GIBB_MOD &at[14]

#define GIBB_METHOD &at[16]

#define MOLINFO &at[92]
#define MOLINFO_biomol &at[93]
#define MOLINFO_tech &at[94]
#define MOLINFO_techexp &at[95]
#define MOLINFO_completeness &at[96]

#define NUM_CONT &at[26]
#define NUM_CONT_refnum &at[27]
#define NUM_CONT_has_zero &at[29]
#define NUM_CONT_ascending &at[31]

#define NUM_ENUM &at[34]
#define NUM_ENUM_num &at[35]
#define NUM_ENUM_names &at[36]
#define NUM_ENUM_names_E &at[37]

#define NUM_REF &at[40]
#define NUM_REF_type &at[41]
#define NUM_REF_aligns &at[42]

#define NUM_REAL &at[45]
#define NUM_REAL_a &at[46]
#define NUM_REAL_b &at[48]
#define NUM_REAL_units &at[49]

#define SEQ_DATA &at[107]
#define SEQ_DATA_iupacna &at[108]
#define SEQ_DATA_iupacaa &at[110]
#define SEQ_DATA_ncbi2na &at[112]
#define SEQ_DATA_ncbi4na &at[115]
#define SEQ_DATA_ncbi8na &at[117]
#define SEQ_DATA_ncbipna &at[119]
#define SEQ_DATA_ncbi8aa &at[121]
#define SEQ_DATA_ncbieaa &at[123]
#define SEQ_DATA_ncbipaa &at[125]
#define SEQ_DATA_ncbistdaa &at[127]

#define SEQ_EXT &at[130]
#define SEQ_EXT_seg &at[131]
#define SEQ_EXT_ref &at[135]
#define SEQ_EXT_map &at[137]
#define SEQ_EXT_delta &at[141]

#define SEG_EXT &at[132]
#define SEG_EXT_E &at[133]

#define REF_EXT &at[136]

#define MAP_EXT &at[138]
#define MAP_EXT_E &at[139]

#define DELTA_EXT &at[142]
#define DELTA_EXT_E &at[143]

#define DELTA_SEQ &at[144]
#define DELTA_SEQ_loc &at[145]
#define DELTA_SEQ_literal &at[146]

#define SEQ_LITERAL &at[147]
#define SEQ_LITERAL_length &at[148]
#define SEQ_LITERAL_fuzz &at[149]
#define SEQ_LITERAL_seq_data &at[150]

#define SEQ_HIST_REC &at[156]
#define SEQ_HIST_REC_date &at[157]
#define SEQ_HIST_REC_ids &at[158]
#define SEQ_HIST_REC_ids_E &at[159]

#define IUPACNA &at[109]

#define IUPACAA &at[111]

#define NCBI2NA &at[113]

#define NCBI4NA &at[116]

#define NCBI8NA &at[118]

#define NCBIPNA &at[120]

#define NCBI8AA &at[122]

#define NCBIEAA &at[124]

#define NCBIPAA &at[126]

#define NCBISTDAA &at[128]

#define ANNOT_ID &at[169]
#define ANNOT_ID_local &at[170]
#define ANNOT_ID_ncbi &at[172]
#define ANNOT_ID_general &at[173]

#define ANNOT_DESCR &at[177]
#define ANNOT_DESCR_E &at[178]

#define ANNOTDESC &at[179]
#define ANNOTDESC_name &at[180]
#define ANNOTDESC_title &at[181]
#define ANNOTDESC_comment &at[182]
#define ANNOTDESC_pub &at[183]
#define ANNOTDESC_user &at[184]
#define ANNOTDESC_create_date &at[185]
#define ANNOTDESC_update_date &at[186]
#define ANNOTDESC_src &at[187]
#define ANNOTDESC_align &at[188]
#define ANNOTDESC_region &at[193]

#define ALIGN_DEF &at[189]
#define ALIGN_DEF_align_type &at[190]
#define ALIGN_DEF_ids &at[191]
#define ALIGN_DEF_ids_E &at[192]
