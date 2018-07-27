/***********************************************************************
*
**
*        Automatic header module from ASNTOOL
*
************************************************************************/

#ifndef _ASNTOOL_
#include <asn.h>
#endif

static char * asnfilename = "mmdb1.h60";
static AsnValxNode avnx[138] = {
    {20,"physiological-form" ,1,0.0,&avnx[1] } ,
    {20,"crystallographic-cell" ,2,0.0,&avnx[2] } ,
    {20,"other" ,255,0.0,NULL } ,
    {20,"dna" ,1,0.0,&avnx[4] } ,
    {20,"rna" ,2,0.0,&avnx[5] } ,
    {20,"protein" ,3,0.0,&avnx[6] } ,
    {20,"other-biopolymer" ,4,0.0,&avnx[7] } ,
    {20,"solvent" ,5,0.0,&avnx[8] } ,
    {20,"other-nonpolymer" ,6,0.0,&avnx[9] } ,
    {20,"other" ,255,0.0,NULL } ,
    {20,"single" ,1,0.0,&avnx[11] } ,
    {20,"partial-double" ,2,0.0,&avnx[12] } ,
    {20,"aromatic" ,3,0.0,&avnx[13] } ,
    {20,"double" ,4,0.0,&avnx[14] } ,
    {20,"triple" ,5,0.0,&avnx[15] } ,
    {20,"other" ,6,0.0,&avnx[16] } ,
    {20,"unknown" ,255,0.0,NULL } ,
    {20,"deoxyribonucleotide" ,1,0.0,&avnx[18] } ,
    {20,"ribonucleotide" ,2,0.0,&avnx[19] } ,
    {20,"amino-acid" ,3,0.0,&avnx[20] } ,
    {20,"other" ,255,0.0,NULL } ,
    {20,"h" ,1,0.0,&avnx[22] } ,
    {20,"he" ,2,0.0,&avnx[23] } ,
    {20,"li" ,3,0.0,&avnx[24] } ,
    {20,"be" ,4,0.0,&avnx[25] } ,
    {20,"b" ,5,0.0,&avnx[26] } ,
    {20,"c" ,6,0.0,&avnx[27] } ,
    {20,"n" ,7,0.0,&avnx[28] } ,
    {20,"o" ,8,0.0,&avnx[29] } ,
    {20,"f" ,9,0.0,&avnx[30] } ,
    {20,"ne" ,10,0.0,&avnx[31] } ,
    {20,"na" ,11,0.0,&avnx[32] } ,
    {20,"mg" ,12,0.0,&avnx[33] } ,
    {20,"al" ,13,0.0,&avnx[34] } ,
    {20,"si" ,14,0.0,&avnx[35] } ,
    {20,"p" ,15,0.0,&avnx[36] } ,
    {20,"s" ,16,0.0,&avnx[37] } ,
    {20,"cl" ,17,0.0,&avnx[38] } ,
    {20,"ar" ,18,0.0,&avnx[39] } ,
    {20,"k" ,19,0.0,&avnx[40] } ,
    {20,"ca" ,20,0.0,&avnx[41] } ,
    {20,"sc" ,21,0.0,&avnx[42] } ,
    {20,"ti" ,22,0.0,&avnx[43] } ,
    {20,"v" ,23,0.0,&avnx[44] } ,
    {20,"cr" ,24,0.0,&avnx[45] } ,
    {20,"mn" ,25,0.0,&avnx[46] } ,
    {20,"fe" ,26,0.0,&avnx[47] } ,
    {20,"co" ,27,0.0,&avnx[48] } ,
    {20,"ni" ,28,0.0,&avnx[49] } ,
    {20,"cu" ,29,0.0,&avnx[50] } ,
    {20,"zn" ,30,0.0,&avnx[51] } ,
    {20,"ga" ,31,0.0,&avnx[52] } ,
    {20,"ge" ,32,0.0,&avnx[53] } ,
    {20,"as" ,33,0.0,&avnx[54] } ,
    {20,"se" ,34,0.0,&avnx[55] } ,
    {20,"br" ,35,0.0,&avnx[56] } ,
    {20,"kr" ,36,0.0,&avnx[57] } ,
    {20,"rb" ,37,0.0,&avnx[58] } ,
    {20,"sr" ,38,0.0,&avnx[59] } ,
    {20,"y" ,39,0.0,&avnx[60] } ,
    {20,"zr" ,40,0.0,&avnx[61] } ,
    {20,"nb" ,41,0.0,&avnx[62] } ,
    {20,"mo" ,42,0.0,&avnx[63] } ,
    {20,"tc" ,43,0.0,&avnx[64] } ,
    {20,"ru" ,44,0.0,&avnx[65] } ,
    {20,"rh" ,45,0.0,&avnx[66] } ,
    {20,"pd" ,46,0.0,&avnx[67] } ,
    {20,"ag" ,47,0.0,&avnx[68] } ,
    {20,"cd" ,48,0.0,&avnx[69] } ,
    {20,"in" ,49,0.0,&avnx[70] } ,
    {20,"sn" ,50,0.0,&avnx[71] } ,
    {20,"sb" ,51,0.0,&avnx[72] } ,
    {20,"te" ,52,0.0,&avnx[73] } ,
    {20,"i" ,53,0.0,&avnx[74] } ,
    {20,"xe" ,54,0.0,&avnx[75] } ,
    {20,"cs" ,55,0.0,&avnx[76] } ,
    {20,"ba" ,56,0.0,&avnx[77] } ,
    {20,"la" ,57,0.0,&avnx[78] } ,
    {20,"ce" ,58,0.0,&avnx[79] } ,
    {20,"pr" ,59,0.0,&avnx[80] } ,
    {20,"nd" ,60,0.0,&avnx[81] } ,
    {20,"pm" ,61,0.0,&avnx[82] } ,
    {20,"sm" ,62,0.0,&avnx[83] } ,
    {20,"eu" ,63,0.0,&avnx[84] } ,
    {20,"gd" ,64,0.0,&avnx[85] } ,
    {20,"tb" ,65,0.0,&avnx[86] } ,
    {20,"dy" ,66,0.0,&avnx[87] } ,
    {20,"ho" ,67,0.0,&avnx[88] } ,
    {20,"er" ,68,0.0,&avnx[89] } ,
    {20,"tm" ,69,0.0,&avnx[90] } ,
    {20,"yb" ,70,0.0,&avnx[91] } ,
    {20,"lu" ,71,0.0,&avnx[92] } ,
    {20,"hf" ,72,0.0,&avnx[93] } ,
    {20,"ta" ,73,0.0,&avnx[94] } ,
    {20,"w" ,74,0.0,&avnx[95] } ,
    {20,"re" ,75,0.0,&avnx[96] } ,
    {20,"os" ,76,0.0,&avnx[97] } ,
    {20,"ir" ,77,0.0,&avnx[98] } ,
    {20,"pt" ,78,0.0,&avnx[99] } ,
    {20,"au" ,79,0.0,&avnx[100] } ,
    {20,"hg" ,80,0.0,&avnx[101] } ,
    {20,"tl" ,81,0.0,&avnx[102] } ,
    {20,"pb" ,82,0.0,&avnx[103] } ,
    {20,"bi" ,83,0.0,&avnx[104] } ,
    {20,"po" ,84,0.0,&avnx[105] } ,
    {20,"at" ,85,0.0,&avnx[106] } ,
    {20,"rn" ,86,0.0,&avnx[107] } ,
    {20,"fr" ,87,0.0,&avnx[108] } ,
    {20,"ra" ,88,0.0,&avnx[109] } ,
    {20,"ac" ,89,0.0,&avnx[110] } ,
    {20,"th" ,90,0.0,&avnx[111] } ,
    {20,"pa" ,91,0.0,&avnx[112] } ,
    {20,"u" ,92,0.0,&avnx[113] } ,
    {20,"np" ,93,0.0,&avnx[114] } ,
    {20,"pu" ,94,0.0,&avnx[115] } ,
    {20,"am" ,95,0.0,&avnx[116] } ,
    {20,"cm" ,96,0.0,&avnx[117] } ,
    {20,"bk" ,97,0.0,&avnx[118] } ,
    {20,"cf" ,98,0.0,&avnx[119] } ,
    {20,"es" ,99,0.0,&avnx[120] } ,
    {20,"fm" ,100,0.0,&avnx[121] } ,
    {20,"md" ,101,0.0,&avnx[122] } ,
    {20,"no" ,102,0.0,&avnx[123] } ,
    {20,"lr" ,103,0.0,&avnx[124] } ,
    {20,"other" ,254,0.0,&avnx[125] } ,
    {20,"unknown" ,255,0.0,NULL } ,
    {20,"true" ,1,0.0,&avnx[127] } ,
    {20,"false" ,2,0.0,&avnx[128] } ,
    {20,"unknown" ,255,0.0,NULL } ,
    {20,"single" ,1,0.0,&avnx[130] } ,
    {20,"partial-double" ,2,0.0,&avnx[131] } ,
    {20,"aromatic" ,3,0.0,&avnx[132] } ,
    {20,"double" ,4,0.0,&avnx[133] } ,
    {20,"triple" ,5,0.0,&avnx[134] } ,
    {20,"other" ,6,0.0,&avnx[135] } ,
    {20,"unknown" ,255,0.0,NULL } ,
    {20,"positive" ,1,0.0,&avnx[137] } ,
    {20,"negative" ,2,0.0,NULL } };

static AsnType atx[163] = {
  {401, "Biostruc" ,1,0,0,0,0,1,0,0,NULL,&atx[27],&atx[1],0,&atx[3]} ,
  {0, "id" ,128,0,0,0,0,0,0,0,NULL,&atx[12],&atx[2],0,&atx[13]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[3],NULL,0,NULL} ,
  {402, "Biostruc-id" ,1,0,0,0,0,1,0,0,NULL,&atx[11],&atx[4],0,&atx[138]} ,
  {0, "mmdb-id" ,128,0,0,0,0,0,0,0,NULL,&atx[5],NULL,0,&atx[7]} ,
  {416, "Mmdb-id" ,1,0,0,0,0,0,0,0,NULL,&atx[6],NULL,0,&atx[21]} ,
  {302, "INTEGER" ,0,2,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "other-database" ,128,1,0,0,0,0,0,0,NULL,&atx[8],NULL,0,&atx[9]} ,
  {414, "Dbtag" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[15]} ,
  {0, "local-id" ,128,2,0,0,0,0,0,0,NULL,&atx[10],NULL,0,NULL} ,
  {413, "Object-id" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[8]} ,
  {315, "CHOICE" ,0,-1,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {312, "SEQUENCE OF" ,0,16,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "descr" ,128,1,0,1,0,0,0,0,NULL,&atx[12],&atx[14],0,&atx[41]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[15],NULL,0,NULL} ,
  {415, "Biostruc-descr" ,1,0,0,0,0,0,0,0,NULL,&atx[11],&atx[16],0,&atx[5]} ,
  {0, "name" ,128,0,0,0,0,0,0,0,NULL,&atx[17],NULL,0,&atx[18]} ,
  {323, "VisibleString" ,0,26,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "pdb-comment" ,128,1,0,0,0,0,0,0,NULL,&atx[17],NULL,0,&atx[19]} ,
  {0, "other-comment" ,128,2,0,0,0,0,0,0,NULL,&atx[17],NULL,0,&atx[20]} ,
  {0, "history" ,128,3,0,0,0,0,0,0,NULL,&atx[21],NULL,0,&atx[39]} ,
  {417, "Biostruc-history" ,1,0,0,0,0,0,0,0,NULL,&atx[27],&atx[22],0,&atx[23]} ,
  {0, "replaces" ,128,0,0,1,0,0,0,0,NULL,&atx[23],NULL,0,&atx[28]} ,
  {418, "Biostruc-replace" ,1,0,0,0,0,0,0,0,NULL,&atx[27],&atx[24],0,&atx[30]} ,
  {0, "id" ,128,0,0,0,0,0,0,0,NULL,&atx[3],NULL,0,&atx[25]} ,
  {0, "date" ,128,1,0,0,0,0,0,0,NULL,&atx[26],NULL,0,NULL} ,
  {412, "Date" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[10]} ,
  {311, "SEQUENCE" ,0,16,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "replaced-by" ,128,1,0,1,0,0,0,0,NULL,&atx[23],NULL,0,&atx[29]} ,
  {0, "data-source" ,128,2,0,1,0,0,0,0,NULL,&atx[30],NULL,0,NULL} ,
  {419, "Biostruc-source" ,1,0,0,0,0,0,0,0,NULL,&atx[27],&atx[31],0,NULL} ,
  {0, "name-of-database" ,128,0,0,0,0,0,0,0,NULL,&atx[17],NULL,0,&atx[32]} ,
  {0, "version-of-database" ,128,1,0,1,0,0,0,0,NULL,&atx[11],&atx[33],0,&atx[35]} ,
  {0, "release-date" ,128,0,0,0,0,0,0,0,NULL,&atx[26],NULL,0,&atx[34]} ,
  {0, "release-code" ,128,1,0,0,0,0,0,0,NULL,&atx[17],NULL,0,NULL} ,
  {0, "database-entry-id" ,128,2,0,0,0,0,0,0,NULL,&atx[3],NULL,0,&atx[36]} ,
  {0, "database-entry-date" ,128,3,0,0,0,0,0,0,NULL,&atx[26],NULL,0,&atx[37]} ,
  {0, "database-entry-history" ,128,4,0,1,0,0,0,0,NULL,&atx[12],&atx[38],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[17],NULL,0,NULL} ,
  {0, "attribution" ,128,4,0,0,0,0,0,0,NULL,&atx[40],NULL,0,NULL} ,
  {411, "Pub" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[26]} ,
  {0, "chemical-graph" ,128,2,0,0,0,0,0,0,NULL,&atx[42],NULL,0,&atx[132]} ,
  {406, "Biostruc-graph" ,1,0,0,0,0,0,1,0,NULL,&atx[43],NULL,0,&atx[157]} ,
  {401, "Biostruc-graph" ,1,0,0,0,0,1,0,0,NULL,&atx[27],&atx[44],0,&atx[46]} ,
  {0, "descr" ,128,0,0,1,0,0,0,0,NULL,&atx[12],&atx[45],0,&atx[58]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[46],NULL,0,NULL} ,
  {402, "Biomol-descr" ,1,0,0,0,0,1,0,0,NULL,&atx[11],&atx[47],0,&atx[101]} ,
  {0, "name" ,128,0,0,0,0,0,0,0,NULL,&atx[17],NULL,0,&atx[48]} ,
  {0, "pdb-class" ,128,1,0,0,0,0,0,0,NULL,&atx[17],NULL,0,&atx[49]} ,
  {0, "pdb-source" ,128,2,0,0,0,0,0,0,NULL,&atx[17],NULL,0,&atx[50]} ,
  {0, "pdb-comment" ,128,3,0,0,0,0,0,0,NULL,&atx[17],NULL,0,&atx[51]} ,
  {0, "other-comment" ,128,4,0,0,0,0,0,0,NULL,&atx[17],NULL,0,&atx[52]} ,
  {0, "organism" ,128,5,0,0,0,0,0,0,NULL,&atx[53],NULL,0,&atx[54]} ,
  {408, "BioSource" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[66]} ,
  {0, "attribution" ,128,6,0,0,0,0,0,0,NULL,&atx[55],NULL,0,&atx[56]} ,
  {407, "Pub" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[53]} ,
  {0, "assembly-type" ,128,7,0,0,0,0,0,0,NULL,&atx[6],&avnx[0],0,&atx[57]} ,
  {0, "molecule-type" ,128,8,0,0,0,0,0,0,NULL,&atx[6],&avnx[3],0,NULL} ,
  {0, "molecule-graphs" ,128,1,0,0,0,0,0,0,NULL,&atx[12],&atx[59],0,&atx[97]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[60],NULL,0,NULL} ,
  {411, "Molecule-graph" ,1,0,0,0,0,0,0,0,NULL,&atx[27],&atx[61],0,&atx[88]} ,
  {0, "id" ,128,0,0,0,0,0,0,0,NULL,&atx[62],NULL,0,&atx[63]} ,
  {404, "Molecule-id" ,1,0,0,0,0,1,0,0,NULL,&atx[6],NULL,0,&atx[71]} ,
  {0, "descr" ,128,1,0,1,0,0,0,0,NULL,&atx[12],&atx[64],0,&atx[65]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[46],NULL,0,NULL} ,
  {0, "seq-id" ,128,2,0,1,0,0,0,0,NULL,&atx[66],NULL,0,&atx[67]} ,
  {409, "Seq-id" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[80]} ,
  {0, "residue-sequence" ,128,3,0,0,0,0,0,0,NULL,&atx[12],&atx[68],0,&atx[86]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[69],NULL,0,NULL} ,
  {413, "Residue" ,1,0,0,0,0,0,0,0,NULL,&atx[27],&atx[70],0,&atx[74]} ,
  {0, "id" ,128,0,0,0,0,0,0,0,NULL,&atx[71],NULL,0,&atx[72]} ,
  {405, "Residue-id" ,1,0,0,0,0,1,0,0,NULL,&atx[6],NULL,0,&atx[94]} ,
  {0, "name" ,128,1,0,1,0,0,0,0,NULL,&atx[17],NULL,0,&atx[73]} ,
  {0, "residue-graph" ,128,2,0,0,0,0,0,0,NULL,&atx[74],NULL,0,NULL} ,
  {414, "Residue-graph-pntr" ,1,0,0,0,0,0,0,0,NULL,&atx[11],&atx[75],0,&atx[76]} ,
  {0, "local" ,128,0,0,0,0,0,0,0,NULL,&atx[76],NULL,0,&atx[77]} ,
  {415, "Residue-graph-id" ,1,0,0,0,0,0,0,0,NULL,&atx[6],NULL,0,&atx[78]} ,
  {0, "biostruc" ,128,1,0,0,0,0,0,0,NULL,&atx[78],NULL,0,&atx[82]} ,
  {416, "Biostruc-graph-pntr" ,1,0,0,0,0,0,0,0,NULL,&atx[27],&atx[79],0,&atx[83]} ,
  {0, "biostruc-id" ,128,0,0,0,0,0,0,0,NULL,&atx[80],NULL,0,&atx[81]} ,
  {410, "Biostruc-id" ,1,0,0,0,0,0,1,0,NULL,&atx[3],NULL,0,&atx[60]} ,
  {0, "residue-graph-id" ,128,1,0,0,0,0,0,0,NULL,&atx[76],NULL,0,NULL} ,
  {0, "standard" ,128,2,0,0,0,0,0,0,NULL,&atx[83],NULL,0,NULL} ,
  {417, "Biostruc-residue-graph-set-pntr" ,1,0,0,0,0,0,0,0,NULL,&atx[27],&atx[84],0,&atx[110]} ,
  {0, "biostruc-residue-graph-set-id" ,128,0,0,0,0,0,0,0,NULL,&atx[80],NULL,0,&atx[85]} ,
  {0, "residue-graph-id" ,128,1,0,0,0,0,0,0,NULL,&atx[76],NULL,0,NULL} ,
  {0, "inter-residue-bonds" ,128,4,0,1,0,0,0,0,NULL,&atx[12],&atx[87],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[88],NULL,0,NULL} ,
  {412, "Inter-residue-bond" ,1,0,0,0,0,0,0,0,NULL,&atx[27],&atx[89],0,&atx[69]} ,
  {0, "atom-id-1" ,128,0,0,0,0,0,0,0,NULL,&atx[90],NULL,0,&atx[95]} ,
  {421, "Atom-pntr" ,1,0,0,0,0,0,0,0,NULL,&atx[27],&atx[91],0,&atx[161]} ,
  {0, "molecule-id" ,128,0,0,0,0,0,0,0,NULL,&atx[62],NULL,0,&atx[92]} ,
  {0, "residue-id" ,128,1,0,0,0,0,0,0,NULL,&atx[71],NULL,0,&atx[93]} ,
  {0, "atom-id" ,128,2,0,0,0,0,0,0,NULL,&atx[94],NULL,0,NULL} ,
  {406, "Atom-id" ,1,0,0,0,0,1,0,0,NULL,&atx[6],NULL,0,&atx[55]} ,
  {0, "atom-id-2" ,128,1,0,0,0,0,0,0,NULL,&atx[90],NULL,0,&atx[96]} ,
  {0, "bond-order" ,128,2,0,1,0,0,0,0,NULL,&atx[6],&avnx[10],0,NULL} ,
  {0, "inter-molecule-bonds" ,128,2,0,1,0,0,0,0,NULL,&atx[12],&atx[98],0,&atx[99]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[88],NULL,0,NULL} ,
  {0, "residue-graphs" ,128,3,0,1,0,0,0,0,NULL,&atx[12],&atx[100],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[101],NULL,0,NULL} ,
  {403, "Residue-graph" ,1,0,0,0,0,1,0,0,NULL,&atx[27],&atx[102],0,&atx[62]} ,
  {0, "id" ,128,0,0,0,0,0,0,0,NULL,&atx[76],NULL,0,&atx[103]} ,
  {0, "descr" ,128,1,0,1,0,0,0,0,NULL,&atx[12],&atx[104],0,&atx[105]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[46],NULL,0,NULL} ,
  {0, "residue-type" ,128,2,0,1,0,0,0,0,NULL,&atx[6],&avnx[17],0,&atx[106]} ,
  {0, "iupac-code" ,128,3,0,1,0,0,0,0,NULL,&atx[12],&atx[107],0,&atx[108]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[17],NULL,0,NULL} ,
  {0, "atoms" ,128,4,0,0,0,0,0,0,NULL,&atx[12],&atx[109],0,&atx[118]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[110],NULL,0,NULL} ,
  {418, "Atom" ,1,0,0,0,0,0,0,0,NULL,&atx[27],&atx[111],0,&atx[120]} ,
  {0, "id" ,128,0,0,0,0,0,0,0,NULL,&atx[94],NULL,0,&atx[112]} ,
  {0, "name" ,128,1,0,1,0,0,0,0,NULL,&atx[17],NULL,0,&atx[113]} ,
  {0, "iupac-code" ,128,2,0,1,0,0,0,0,NULL,&atx[12],&atx[114],0,&atx[115]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[17],NULL,0,NULL} ,
  {0, "element" ,128,3,0,0,0,0,0,0,NULL,&atx[116],&avnx[21],0,&atx[117]} ,
  {310, "ENUMERATED" ,0,10,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "ionizable-proton" ,128,4,0,1,0,0,0,0,NULL,&atx[116],&avnx[126],0,NULL} ,
  {0, "bonds" ,128,5,0,0,0,0,0,0,NULL,&atx[12],&atx[119],0,&atx[124]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[120],NULL,0,NULL} ,
  {419, "Intra-residue-bond" ,1,0,0,0,0,0,0,0,NULL,&atx[27],&atx[121],0,&atx[126]} ,
  {0, "atom-id-1" ,128,0,0,0,0,0,0,0,NULL,&atx[94],NULL,0,&atx[122]} ,
  {0, "atom-id-2" ,128,1,0,0,0,0,0,0,NULL,&atx[94],NULL,0,&atx[123]} ,
  {0, "bond-order" ,128,2,0,1,0,0,0,0,NULL,&atx[6],&avnx[129],0,NULL} ,
  {0, "chiral-centers" ,128,6,0,1,0,0,0,0,NULL,&atx[12],&atx[125],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[126],NULL,0,NULL} ,
  {420, "Chiral-center" ,1,0,0,0,0,0,0,0,NULL,&atx[27],&atx[127],0,&atx[90]} ,
  {0, "c" ,128,0,0,0,0,0,0,0,NULL,&atx[94],NULL,0,&atx[128]} ,
  {0, "n1" ,128,1,0,0,0,0,0,0,NULL,&atx[94],NULL,0,&atx[129]} ,
  {0, "n2" ,128,2,0,0,0,0,0,0,NULL,&atx[94],NULL,0,&atx[130]} ,
  {0, "n3" ,128,3,0,0,0,0,0,0,NULL,&atx[94],NULL,0,&atx[131]} ,
  {0, "sign" ,128,4,0,0,0,0,0,0,NULL,&atx[116],&avnx[136],0,NULL} ,
  {0, "features" ,128,3,0,1,0,0,0,0,NULL,&atx[12],&atx[133],0,&atx[135]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[134],NULL,0,NULL} ,
  {410, "Biostruc-feature-set" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[40]} ,
  {0, "model" ,128,4,0,1,0,0,0,0,NULL,&atx[12],&atx[136],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[137],NULL,0,NULL} ,
  {409, "Biostruc-model" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[134]} ,
  {403, "Biostruc-set" ,1,0,0,0,0,1,0,0,NULL,&atx[27],&atx[139],0,&atx[145]} ,
  {0, "id" ,128,0,0,1,0,0,0,0,NULL,&atx[12],&atx[140],0,&atx[141]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[3],NULL,0,NULL} ,
  {0, "descr" ,128,1,0,1,0,0,0,0,NULL,&atx[12],&atx[142],0,&atx[143]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[15],NULL,0,NULL} ,
  {0, "biostrucs" ,128,2,0,0,0,0,0,0,NULL,&atx[12],&atx[144],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[0],NULL,0,NULL} ,
  {404, "Biostruc-annot-set" ,1,0,0,0,0,1,0,0,NULL,&atx[27],&atx[146],0,&atx[152]} ,
  {0, "id" ,128,0,0,1,0,0,0,0,NULL,&atx[12],&atx[147],0,&atx[148]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[3],NULL,0,NULL} ,
  {0, "descr" ,128,1,0,1,0,0,0,0,NULL,&atx[12],&atx[149],0,&atx[150]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[15],NULL,0,NULL} ,
  {0, "features" ,128,2,0,0,0,0,0,0,NULL,&atx[12],&atx[151],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[134],NULL,0,NULL} ,
  {405, "Biostruc-residue-graph-set" ,1,0,0,0,0,1,0,0,NULL,&atx[27],&atx[153],0,&atx[42]} ,
  {0, "id" ,128,0,0,1,0,0,0,0,NULL,&atx[12],&atx[154],0,&atx[155]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[3],NULL,0,NULL} ,
  {0, "descr" ,128,1,0,1,0,0,0,0,NULL,&atx[12],&atx[156],0,&atx[158]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[157],NULL,0,NULL} ,
  {407, "Biomol-descr" ,1,0,0,0,0,0,1,0,NULL,&atx[46],NULL,0,&atx[160]} ,
  {0, "residue-graphs" ,128,2,0,0,0,0,0,0,NULL,&atx[12],&atx[159],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[160],NULL,0,NULL} ,
  {408, "Residue-graph" ,1,0,0,0,0,0,1,0,NULL,&atx[101],NULL,0,&atx[137]} ,
  {422, "Atom-pntr-set" ,1,0,0,0,0,0,0,0,NULL,&atx[12],&atx[162],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[90],NULL,0,NULL} };

static AsnModule ampx[2] = {
  { "MMDB" , "mmdb1.h60",&atx[0],NULL,&ampx[1],0,0} ,
  { "MMDB-Chemical-graph" , NULL,&atx[43],NULL,NULL,0,0} };

static AsnValxNodePtr avn = avnx;
static AsnTypePtr at = atx;
static AsnModulePtr amp = ampx;



/**************************************************
*
*    Defines for Module MMDB
*
**************************************************/

#define BIOSTRUC &at[0]
#define BIOSTRUC_id &at[1]
#define BIOSTRUC_id_E &at[2]
#define BIOSTRUC_descr &at[13]
#define BIOSTRUC_descr_E &at[14]
#define BIOSTRUC_chemical_graph &at[41]
#define BIOSTRUC_features &at[132]
#define BIOSTRUC_features_E &at[133]
#define BIOSTRUC_model &at[135]
#define BIOSTRUC_model_E &at[136]

#define BIOSTRUC_ID &at[3]
#define BIOSTRUC_ID_mmdb_id &at[4]
#define BIOSTRUC_ID_other_database &at[7]
#define BIOSTRUC_ID_local_id &at[9]

#define BIOSTRUC_SET &at[138]
#define BIOSTRUC_SET_id &at[139]
#define BIOSTRUC_SET_id_E &at[140]
#define BIOSTRUC_SET_descr &at[141]
#define BIOSTRUC_SET_descr_E &at[142]
#define BIOSTRUC_SET_biostrucs &at[143]
#define BIOSTRUC_SET_biostrucs_E &at[144]

#define BIOSTRUC_ANNOT_SET &at[145]
#define BIOSTRUC_ANNOT_SET_id &at[146]
#define BIOSTRUC_ANNOT_SET_id_E &at[147]
#define BIOSTRUC_ANNOT_SET_descr &at[148]
#define BIOSTRUC_ANNOT_SET_descr_E &at[149]
#define BIOSTRUC_ANNOT_SET_features &at[150]
#define BIOSTRUC_ANNOT_SET_features_E &at[151]

#define BIOSTRUC_RESIDUE_GRAPH_SET &at[152]
#define BIOSTRUC_RESIDUE_GRAPH_SET_id &at[153]
#define BIOSTRUC_RESIDUE_GRAPH_SET_id_E &at[154]
#define BIOSTRUC_RESIDUE_GRAPH_SET_descr &at[155]
#define BIOSTRUC_RESIDUE_GRAPH_SET_descr_E &at[156]
#define BIOSTRUC_RESIDUE_GRAPH_SET_residue_graphs &at[158]
#define BIOSTRUC_RESIDUE_GRAPH_SET_residue_graphs_E &at[159]

#define BIOSTRUC_DESCR &at[15]
#define BIOSTRUC_DESCR_name &at[16]
#define BIOSTRUC_DESCR_pdb_comment &at[18]
#define BIOSTRUC_DESCR_other_comment &at[19]
#define BIOSTRUC_DESCR_history &at[20]
#define BIOSTRUC_DESCR_attribution &at[39]

#define MMDB_ID &at[5]

#define BIOSTRUC_HISTORY &at[21]
#define BIOSTRUC_HISTORY_replaces &at[22]
#define BIOSTRUC_HISTORY_replaced_by &at[28]
#define BIOSTRUC_HISTORY_data_source &at[29]

#define BIOSTRUC_REPLACE &at[23]
#define BIOSTRUC_REPLACE_id &at[24]
#define BIOSTRUC_REPLACE_date &at[25]

#define BIOSTRUC_SOURCE &at[30]
#define BIOSTRUC_SOURCE_name_of_database &at[31]
#define BIOSTRUC_SOURCE_version_of_database &at[32]
#define BIOSTRUC_SOURCE_version_of_database_release_date &at[33]
#define BIOSTRUC_SOURCE_version_of_database_release_code &at[34]
#define BIOSTRUC_SOURCE_database_entry_id &at[35]
#define BIOSTRUC_SOURCE_database_entry_date &at[36]
#define BIOSTRUC_SOURCE_database_entry_history &at[37]
#define BIOSTRUC_SOURCE_database_entry_history_E &at[38]


/**************************************************
*
*    Defines for Module MMDB-Chemical-graph
*
**************************************************/

#define BIOSTRUC_GRAPH &at[43]
#define BIOSTRUC_GRAPH_descr &at[44]
#define BIOSTRUC_GRAPH_descr_E &at[45]
#define BIOSTRUC_GRAPH_molecule_graphs &at[58]
#define BIOSTRUC_GRAPH_molecule_graphs_E &at[59]
#define BIOSTRUC_GRAPH_inter_molecule_bonds &at[97]
#define BIOSTRUC_GRAPH_inter_molecule_bonds_E &at[98]
#define BIOSTRUC_GRAPH_residue_graphs &at[99]
#define BIOSTRUC_GRAPH_residue_graphs_E &at[100]

#define BIOMOL_DESCR &at[46]
#define BIOMOL_DESCR_name &at[47]
#define BIOMOL_DESCR_pdb_class &at[48]
#define BIOMOL_DESCR_pdb_source &at[49]
#define BIOMOL_DESCR_pdb_comment &at[50]
#define BIOMOL_DESCR_other_comment &at[51]
#define BIOMOL_DESCR_organism &at[52]
#define BIOMOL_DESCR_attribution &at[54]
#define BIOMOL_DESCR_assembly_type &at[56]
#define BIOMOL_DESCR_molecule_type &at[57]

#define RESIDUE_GRAPH &at[101]
#define RESIDUE_GRAPH_id &at[102]
#define RESIDUE_GRAPH_descr &at[103]
#define RESIDUE_GRAPH_descr_E &at[104]
#define RESIDUE_GRAPH_residue_type &at[105]
#define RESIDUE_GRAPH_iupac_code &at[106]
#define RESIDUE_GRAPH_iupac_code_E &at[107]
#define RESIDUE_GRAPH_atoms &at[108]
#define RESIDUE_GRAPH_atoms_E &at[109]
#define RESIDUE_GRAPH_bonds &at[118]
#define RESIDUE_GRAPH_bonds_E &at[119]
#define RESIDUE_GRAPH_chiral_centers &at[124]
#define RESIDUE_GRAPH_chiral_centers_E &at[125]

#define MOLECULE_ID &at[62]

#define RESIDUE_ID &at[71]

#define ATOM_ID &at[94]

#define MOLECULE_GRAPH &at[60]
#define MOLECULE_GRAPH_id &at[61]
#define MOLECULE_GRAPH_descr &at[63]
#define MOLECULE_GRAPH_descr_E &at[64]
#define MOLECULE_GRAPH_seq_id &at[65]
#define MOLECULE_GRAPH_residue_sequence &at[67]
#define MOLECULE_GRAPH_residue_sequence_E &at[68]
#define MOLECULE_GRAPH_inter_residue_bonds &at[86]
#define MOLECULE_GRAPH_inter_residue_bonds_E &at[87]

#define INTER_RESIDUE_BOND &at[88]
#define INTER_RESIDUE_BOND_atom_id_1 &at[89]
#define INTER_RESIDUE_BOND_atom_id_2 &at[95]
#define INTER_RESIDUE_BOND_bond_order &at[96]

#define RESIDUE &at[69]
#define RESIDUE_id &at[70]
#define RESIDUE_name &at[72]
#define RESIDUE_residue_graph &at[73]

#define RESIDUE_GRAPH_PNTR &at[74]
#define RESIDUE_GRAPH_PNTR_local &at[75]
#define RESIDUE_GRAPH_PNTR_biostruc &at[77]
#define RESIDUE_GRAPH_PNTR_standard &at[82]

#define RESIDUE_GRAPH_ID &at[76]

#define BIOSTRUC_GRAPH_PNTR &at[78]
#define BIOSTRUC_GRAPH_PNTR_biostruc_id &at[79]
#define BIOSTRUC_GRAPH_PNTR_residue_graph_id &at[81]

#define BIOSTRUC_RESIDUE_GRAPH_SET_PNTR &at[83]
#define BIOSTRUC_RESIDUE_GRAPH_SET_PNTR_biostruc_residue_graph_set_id &at[84]
#define BIOSTRUC_RESIDUE_GRAPH_SET_PNTR_residue_graph_id &at[85]

#define ATOM &at[110]
#define ATOM_id &at[111]
#define ATOM_name &at[112]
#define ATOM_iupac_code &at[113]
#define ATOM_iupac_code_E &at[114]
#define ATOM_element &at[115]
#define ATOM_ionizable_proton &at[117]

#define INTRA_RESIDUE_BOND &at[120]
#define INTRA_RESIDUE_BOND_atom_id_1 &at[121]
#define INTRA_RESIDUE_BOND_atom_id_2 &at[122]
#define INTRA_RESIDUE_BOND_bond_order &at[123]

#define CHIRAL_CENTER &at[126]
#define CHIRAL_CENTER_c &at[127]
#define CHIRAL_CENTER_n1 &at[128]
#define CHIRAL_CENTER_n2 &at[129]
#define CHIRAL_CENTER_n3 &at[130]
#define CHIRAL_CENTER_sign &at[131]

#define ATOM_PNTR &at[90]
#define ATOM_PNTR_molecule_id &at[91]
#define ATOM_PNTR_residue_id &at[92]
#define ATOM_PNTR_atom_id &at[93]

#define ATOM_PNTR_SET &at[161]
#define ATOM_PNTR_SET_E &at[162]
