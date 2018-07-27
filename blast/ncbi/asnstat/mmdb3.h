/***********************************************************************
*
**
*        Automatic header module from ASNTOOL
*
************************************************************************/

#ifndef _ASNTOOL_
#include <asn.h>
#endif

static char * asnfilename = "mmdb3.h63";
static AsnValxNode avnx[48] = {
    {20,"helix" ,1,0.0,&avnx[1] } ,
    {20,"strand" ,2,0.0,&avnx[2] } ,
    {20,"sheet" ,3,0.0,&avnx[3] } ,
    {20,"turn" ,4,0.0,&avnx[4] } ,
    {20,"site" ,5,0.0,&avnx[5] } ,
    {20,"footnote" ,6,0.0,&avnx[6] } ,
    {20,"comment" ,7,0.0,&avnx[7] } ,
    {20,"subgraph" ,100,0.0,&avnx[8] } ,
    {20,"region" ,101,0.0,&avnx[9] } ,
    {20,"core" ,102,0.0,&avnx[10] } ,
    {20,"supercore" ,103,0.0,&avnx[11] } ,
    {20,"color" ,150,0.0,&avnx[12] } ,
    {20,"render" ,151,0.0,&avnx[13] } ,
    {20,"label" ,152,0.0,&avnx[14] } ,
    {20,"transform" ,153,0.0,&avnx[15] } ,
    {20,"camera" ,154,0.0,&avnx[16] } ,
    {20,"script" ,155,0.0,&avnx[17] } ,
    {20,"alignment" ,200,0.0,&avnx[18] } ,
    {20,"similarity" ,201,0.0,&avnx[19] } ,
    {20,"multalign" ,202,0.0,&avnx[20] } ,
    {20,"indirect" ,203,0.0,&avnx[21] } ,
    {20,"cn3dstate" ,254,0.0,&avnx[22] } ,
    {20,"other" ,255,0.0,NULL } ,
    {20,"default" ,0,0.0,&avnx[24] } ,
    {20,"wire" ,1,0.0,&avnx[25] } ,
    {20,"space" ,2,0.0,&avnx[26] } ,
    {20,"stick" ,3,0.0,&avnx[27] } ,
    {20,"ballNStick" ,4,0.0,&avnx[28] } ,
    {20,"thickWire" ,5,0.0,&avnx[29] } ,
    {20,"hide" ,9,0.0,&avnx[30] } ,
    {20,"name" ,10,0.0,&avnx[31] } ,
    {20,"number" ,11,0.0,&avnx[32] } ,
    {20,"pdbNumber" ,12,0.0,&avnx[33] } ,
    {20,"objWireFrame" ,150,0.0,&avnx[34] } ,
    {20,"objPolygons" ,151,0.0,&avnx[35] } ,
    {20,"colorsetCPK" ,225,0.0,&avnx[36] } ,
    {20,"colorsetbyChain" ,226,0.0,&avnx[37] } ,
    {20,"colorsetbyTemp" ,227,0.0,&avnx[38] } ,
    {20,"colorsetbyRes" ,228,0.0,&avnx[39] } ,
    {20,"colorsetbyLen" ,229,0.0,&avnx[40] } ,
    {20,"colorsetbySStru" ,230,0.0,&avnx[41] } ,
    {20,"colorsetbyHydro" ,231,0.0,&avnx[42] } ,
    {20,"colorsetbyObject" ,246,0.0,&avnx[43] } ,
    {20,"colorsetbyDomain" ,247,0.0,&avnx[44] } ,
    {20,"other" ,255,0.0,NULL } ,
    {3,NULL,10,0.0,NULL } ,
    {3,NULL,2,0.0,NULL } ,
    {3,NULL,2,0.0,NULL } };

static AsnType atx[220] = {
  {401, "Biostruc-feature-set" ,1,0,0,0,0,1,0,0,NULL,&atx[29],&atx[1],0,&atx[106]} ,
  {0, "id" ,128,0,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[4]} ,
  {410, "Biostruc-feature-set-id" ,1,0,0,0,0,1,0,0,NULL,&atx[3],NULL,0,&atx[19]} ,
  {302, "INTEGER" ,0,2,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "descr" ,128,1,0,1,0,0,0,0,NULL,&atx[14],&atx[5],0,&atx[15]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[6],NULL,0,NULL} ,
  {420, "Biostruc-feature-set-descr" ,1,0,0,0,0,0,0,0,NULL,&atx[13],&atx[7],0,&atx[17]} ,
  {0, "name" ,128,0,0,0,0,0,0,0,NULL,&atx[8],NULL,0,&atx[9]} ,
  {323, "VisibleString" ,0,26,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "pdb-comment" ,128,1,0,0,0,0,0,0,NULL,&atx[8],NULL,0,&atx[10]} ,
  {0, "other-comment" ,128,2,0,0,0,0,0,0,NULL,&atx[8],NULL,0,&atx[11]} ,
  {0, "attribution" ,128,3,0,0,0,0,0,0,NULL,&atx[12],NULL,0,NULL} ,
  {419, "Pub" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[6]} ,
  {315, "CHOICE" ,0,-1,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {312, "SEQUENCE OF" ,0,16,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "features" ,128,2,0,0,0,0,0,0,NULL,&atx[14],&atx[16],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[17],NULL,0,NULL} ,
  {421, "Biostruc-feature" ,1,0,0,0,0,0,0,0,NULL,&atx[29],&atx[18],0,&atx[24]} ,
  {0, "id" ,128,0,0,1,0,0,0,0,NULL,&atx[19],NULL,0,&atx[20]} ,
  {411, "Biostruc-feature-id" ,1,0,0,0,0,1,0,0,NULL,&atx[3],NULL,0,&atx[93]} ,
  {0, "name" ,128,1,0,1,0,0,0,0,NULL,&atx[8],NULL,0,&atx[21]} ,
  {0, "type" ,128,2,0,1,0,0,0,0,NULL,&atx[3],&avnx[0],0,&atx[22]} ,
  {0, "property" ,128,3,0,1,0,0,0,0,NULL,&atx[13],&atx[23],0,&atx[104]} ,
  {0, "color" ,128,0,0,0,0,0,0,0,NULL,&atx[24],NULL,0,&atx[30]} ,
  {422, "Color-prop" ,1,0,0,0,0,0,0,0,NULL,&atx[29],&atx[25],0,&atx[31]} ,
  {0, "r" ,128,0,0,1,0,0,0,0,NULL,&atx[3],NULL,0,&atx[26]} ,
  {0, "g" ,128,1,0,1,0,0,0,0,NULL,&atx[3],NULL,0,&atx[27]} ,
  {0, "b" ,128,2,0,1,0,0,0,0,NULL,&atx[3],NULL,0,&atx[28]} ,
  {0, "name" ,128,3,0,1,0,0,0,0,NULL,&atx[8],NULL,0,NULL} ,
  {311, "SEQUENCE" ,0,16,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "render" ,128,1,0,0,0,0,0,0,NULL,&atx[31],NULL,0,&atx[32]} ,
  {423, "Render-prop" ,1,0,0,0,0,0,0,0,NULL,&atx[3],&avnx[23],0,&atx[57]} ,
  {0, "transform" ,128,2,0,0,0,0,0,0,NULL,&atx[33],NULL,0,&atx[56]} ,
  {409, "Transform" ,1,0,0,0,0,1,0,0,NULL,&atx[29],&atx[34],0,&atx[2]} ,
  {0, "id" ,128,0,0,0,0,0,0,0,NULL,&atx[3],NULL,0,&atx[35]} ,
  {0, "moves" ,128,1,0,0,0,0,0,0,NULL,&atx[14],&atx[36],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[37],NULL,0,NULL} ,
  {438, "Move" ,1,0,0,0,0,0,0,0,NULL,&atx[13],&atx[38],0,&atx[39]} ,
  {0, "rotate" ,128,0,0,0,0,0,0,0,NULL,&atx[39],NULL,0,&atx[50]} ,
  {439, "Rot-matrix" ,1,0,0,0,0,0,0,0,NULL,&atx[29],&atx[40],0,&atx[51]} ,
  {0, "scale-factor" ,128,0,0,0,0,0,0,0,NULL,&atx[3],NULL,0,&atx[41]} ,
  {0, "rot-11" ,128,1,0,0,0,0,0,0,NULL,&atx[3],NULL,0,&atx[42]} ,
  {0, "rot-12" ,128,2,0,0,0,0,0,0,NULL,&atx[3],NULL,0,&atx[43]} ,
  {0, "rot-13" ,128,3,0,0,0,0,0,0,NULL,&atx[3],NULL,0,&atx[44]} ,
  {0, "rot-21" ,128,4,0,0,0,0,0,0,NULL,&atx[3],NULL,0,&atx[45]} ,
  {0, "rot-22" ,128,5,0,0,0,0,0,0,NULL,&atx[3],NULL,0,&atx[46]} ,
  {0, "rot-23" ,128,6,0,0,0,0,0,0,NULL,&atx[3],NULL,0,&atx[47]} ,
  {0, "rot-31" ,128,7,0,0,0,0,0,0,NULL,&atx[3],NULL,0,&atx[48]} ,
  {0, "rot-32" ,128,8,0,0,0,0,0,0,NULL,&atx[3],NULL,0,&atx[49]} ,
  {0, "rot-33" ,128,9,0,0,0,0,0,0,NULL,&atx[3],NULL,0,NULL} ,
  {0, "translate" ,128,1,0,0,0,0,0,0,NULL,&atx[51],NULL,0,NULL} ,
  {440, "Trans-matrix" ,1,0,0,0,0,0,0,0,NULL,&atx[29],&atx[52],0,&atx[64]} ,
  {0, "scale-factor" ,128,0,0,0,0,0,0,0,NULL,&atx[3],NULL,0,&atx[53]} ,
  {0, "tran-1" ,128,1,0,0,0,0,0,0,NULL,&atx[3],NULL,0,&atx[54]} ,
  {0, "tran-2" ,128,2,0,0,0,0,0,0,NULL,&atx[3],NULL,0,&atx[55]} ,
  {0, "tran-3" ,128,3,0,0,0,0,0,0,NULL,&atx[3],NULL,0,NULL} ,
  {0, "camera" ,128,3,0,0,0,0,0,0,NULL,&atx[57],NULL,0,&atx[82]} ,
  {424, "Camera" ,1,0,0,0,0,0,0,0,NULL,&atx[29],&atx[58],0,&atx[83]} ,
  {0, "x" ,128,0,0,0,0,0,0,0,NULL,&atx[3],NULL,0,&atx[59]} ,
  {0, "y" ,128,1,0,0,0,0,0,0,NULL,&atx[3],NULL,0,&atx[60]} ,
  {0, "distance" ,128,2,0,0,0,0,0,0,NULL,&atx[3],NULL,0,&atx[61]} ,
  {0, "angle" ,128,3,0,0,0,0,0,0,NULL,&atx[3],NULL,0,&atx[62]} ,
  {0, "scale" ,128,4,0,0,0,0,0,0,NULL,&atx[3],NULL,0,&atx[63]} ,
  {0, "modelview" ,128,5,0,0,0,0,0,0,NULL,&atx[64],NULL,0,NULL} ,
  {441, "GL-matrix" ,1,0,0,0,0,0,0,0,NULL,&atx[29],&atx[65],0,&atx[85]} ,
  {0, "scale" ,128,0,0,0,0,0,0,0,NULL,&atx[3],NULL,0,&atx[66]} ,
  {0, "m11" ,128,1,0,0,0,0,0,0,NULL,&atx[3],NULL,0,&atx[67]} ,
  {0, "m12" ,128,2,0,0,0,0,0,0,NULL,&atx[3],NULL,0,&atx[68]} ,
  {0, "m13" ,128,3,0,0,0,0,0,0,NULL,&atx[3],NULL,0,&atx[69]} ,
  {0, "m14" ,128,4,0,0,0,0,0,0,NULL,&atx[3],NULL,0,&atx[70]} ,
  {0, "m21" ,128,5,0,0,0,0,0,0,NULL,&atx[3],NULL,0,&atx[71]} ,
  {0, "m22" ,128,6,0,0,0,0,0,0,NULL,&atx[3],NULL,0,&atx[72]} ,
  {0, "m23" ,128,7,0,0,0,0,0,0,NULL,&atx[3],NULL,0,&atx[73]} ,
  {0, "m24" ,128,8,0,0,0,0,0,0,NULL,&atx[3],NULL,0,&atx[74]} ,
  {0, "m31" ,128,9,0,0,0,0,0,0,NULL,&atx[3],NULL,0,&atx[75]} ,
  {0, "m32" ,128,10,0,0,0,0,0,0,NULL,&atx[3],NULL,0,&atx[76]} ,
  {0, "m33" ,128,11,0,0,0,0,0,0,NULL,&atx[3],NULL,0,&atx[77]} ,
  {0, "m34" ,128,12,0,0,0,0,0,0,NULL,&atx[3],NULL,0,&atx[78]} ,
  {0, "m41" ,128,13,0,0,0,0,0,0,NULL,&atx[3],NULL,0,&atx[79]} ,
  {0, "m42" ,128,14,0,0,0,0,0,0,NULL,&atx[3],NULL,0,&atx[80]} ,
  {0, "m43" ,128,15,0,0,0,0,0,0,NULL,&atx[3],NULL,0,&atx[81]} ,
  {0, "m44" ,128,16,0,0,0,0,0,0,NULL,&atx[3],NULL,0,NULL} ,
  {0, "script" ,128,4,0,0,0,0,0,0,NULL,&atx[83],NULL,0,&atx[102]} ,
  {425, "Biostruc-script" ,1,0,0,0,0,0,0,0,NULL,&atx[14],&atx[84],0,&atx[140]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[85],NULL,0,NULL} ,
  {442, "Biostruc-script-step" ,1,0,0,0,0,0,0,0,NULL,&atx[29],&atx[86],0,&atx[87]} ,
  {0, "step-id" ,128,0,0,0,0,0,0,0,NULL,&atx[87],NULL,0,&atx[88]} ,
  {443, "Step-id" ,1,0,0,0,0,0,0,0,NULL,&atx[3],NULL,0,NULL} ,
  {0, "step-name" ,128,1,0,1,0,0,0,0,NULL,&atx[8],NULL,0,&atx[89]} ,
  {0, "feature-do" ,128,2,0,1,0,0,0,0,NULL,&atx[14],&atx[90],0,&atx[96]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[91],NULL,0,NULL} ,
  {428, "Other-feature" ,1,0,0,0,0,0,0,0,NULL,&atx[29],&atx[92],0,&atx[120]} ,
  {0, "biostruc-id" ,128,0,0,0,0,0,0,0,NULL,&atx[93],NULL,0,&atx[94]} ,
  {412, "Biostruc-id" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[112]} ,
  {0, "set" ,128,1,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[95]} ,
  {0, "feature" ,128,2,0,0,0,0,0,0,NULL,&atx[19],NULL,0,NULL} ,
  {0, "camera-move" ,128,3,0,1,0,0,0,0,NULL,&atx[33],NULL,0,&atx[97]} ,
  {0, "pause" ,128,4,0,0,1,0,0,0,&avnx[45],&atx[3],NULL,0,&atx[98]} ,
  {0, "waitevent" ,128,5,0,0,0,0,0,0,NULL,&atx[99],NULL,0,&atx[100]} ,
  {301, "BOOLEAN" ,0,1,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "extra" ,128,6,0,0,0,0,0,0,NULL,&atx[3],NULL,0,&atx[101]} ,
  {0, "jump" ,128,7,0,1,0,0,0,0,NULL,&atx[87],NULL,0,NULL} ,
  {0, "user" ,128,5,0,0,0,0,0,0,NULL,&atx[103],NULL,0,NULL} ,
  {418, "User-object" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[12]} ,
  {0, "location" ,128,4,0,1,0,0,0,0,NULL,&atx[13],&atx[105],0,NULL} ,
  {0, "subgraph" ,128,0,0,0,0,0,0,0,NULL,&atx[106],NULL,0,&atx[139]} ,
  {402, "Chem-graph-pntrs" ,1,0,0,0,0,1,0,0,NULL,&atx[13],&atx[107],0,&atx[108]} ,
  {0, "atoms" ,128,0,0,0,0,0,0,0,NULL,&atx[108],NULL,0,&atx[119]} ,
  {403, "Atom-pntrs" ,1,0,0,0,0,1,0,0,NULL,&atx[29],&atx[109],0,&atx[188]} ,
  {0, "number-of-ptrs" ,128,0,0,0,0,0,0,0,NULL,&atx[3],NULL,0,&atx[110]} ,
  {0, "molecule-ids" ,128,1,0,0,0,0,0,0,NULL,&atx[14],&atx[111],0,&atx[113]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[112],NULL,0,NULL} ,
  {413, "Molecule-id" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[115]} ,
  {0, "residue-ids" ,128,2,0,0,0,0,0,0,NULL,&atx[14],&atx[114],0,&atx[116]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[115],NULL,0,NULL} ,
  {414, "Residue-id" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[118]} ,
  {0, "atom-ids" ,128,3,0,0,0,0,0,0,NULL,&atx[14],&atx[117],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[118],NULL,0,NULL} ,
  {415, "Atom-id" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[142]} ,
  {0, "residues" ,128,1,0,0,0,0,0,0,NULL,&atx[120],NULL,0,&atx[134]} ,
  {429, "Residue-pntrs" ,1,0,0,0,0,0,0,0,NULL,&atx[13],&atx[121],0,&atx[135]} ,
  {0, "explicit" ,128,0,0,0,0,0,0,0,NULL,&atx[122],NULL,0,&atx[128]} ,
  {431, "Residue-explicit-pntrs" ,1,0,0,0,0,0,0,0,NULL,&atx[29],&atx[123],0,&atx[130]} ,
  {0, "number-of-ptrs" ,128,0,0,0,0,0,0,0,NULL,&atx[3],NULL,0,&atx[124]} ,
  {0, "molecule-ids" ,128,1,0,0,0,0,0,0,NULL,&atx[14],&atx[125],0,&atx[126]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[112],NULL,0,NULL} ,
  {0, "residue-ids" ,128,2,0,0,0,0,0,0,NULL,&atx[14],&atx[127],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[115],NULL,0,NULL} ,
  {0, "interval" ,128,1,0,0,0,0,0,0,NULL,&atx[14],&atx[129],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[130],NULL,0,NULL} ,
  {432, "Residue-interval-pntr" ,1,0,0,0,0,0,0,0,NULL,&atx[29],&atx[131],0,&atx[146]} ,
  {0, "molecule-id" ,128,0,0,0,0,0,0,0,NULL,&atx[112],NULL,0,&atx[132]} ,
  {0, "from" ,128,1,0,0,0,0,0,0,NULL,&atx[115],NULL,0,&atx[133]} ,
  {0, "to" ,128,2,0,0,0,0,0,0,NULL,&atx[115],NULL,0,NULL} ,
  {0, "molecules" ,128,2,0,0,0,0,0,0,NULL,&atx[135],NULL,0,NULL} ,
  {430, "Molecule-pntrs" ,1,0,0,0,0,0,0,0,NULL,&atx[29],&atx[136],0,&atx[122]} ,
  {0, "number-of-ptrs" ,128,0,0,0,0,0,0,0,NULL,&atx[3],NULL,0,&atx[137]} ,
  {0, "molecule-ids" ,128,1,0,0,0,0,0,0,NULL,&atx[14],&atx[138],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[112],NULL,0,NULL} ,
  {0, "region" ,128,1,0,0,0,0,0,0,NULL,&atx[140],NULL,0,&atx[187]} ,
  {426, "Region-pntrs" ,1,0,0,0,0,0,0,0,NULL,&atx[29],&atx[141],0,&atx[211]} ,
  {0, "model-id" ,128,0,0,0,0,0,0,0,NULL,&atx[142],NULL,0,&atx[143]} ,
  {416, "Model-id" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[148]} ,
  {0, "region" ,128,1,0,0,0,0,0,0,NULL,&atx[13],&atx[144],0,NULL} ,
  {0, "site" ,128,0,0,0,0,0,0,0,NULL,&atx[14],&atx[145],0,&atx[152]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[146],NULL,0,NULL} ,
  {433, "Region-coordinates" ,1,0,0,0,0,0,0,0,NULL,&atx[29],&atx[147],0,&atx[154]} ,
  {0, "model-coord-set-id" ,128,0,0,0,0,0,0,0,NULL,&atx[148],NULL,0,&atx[149]} ,
  {417, "Model-coordinate-set-id" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[103]} ,
  {0, "number-of-coords" ,128,1,0,1,0,0,0,0,NULL,&atx[3],NULL,0,&atx[150]} ,
  {0, "coordinate-indices" ,128,2,0,1,0,0,0,0,NULL,&atx[14],&atx[151],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[3],NULL,0,NULL} ,
  {0, "boundary" ,128,1,0,0,0,0,0,0,NULL,&atx[14],&atx[153],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[154],NULL,0,NULL} ,
  {434, "Region-boundary" ,1,0,0,0,0,0,0,0,NULL,&atx[13],&atx[155],0,&atx[200]} ,
  {0, "sphere" ,128,0,0,0,0,0,0,0,NULL,&atx[156],NULL,0,&atx[167]} ,
  {405, "Sphere" ,1,0,0,0,0,1,0,0,NULL,&atx[29],&atx[157],0,&atx[168]} ,
  {0, "center" ,128,0,0,0,0,0,0,0,NULL,&atx[158],NULL,0,&atx[163]} ,
  {436, "Model-space-point" ,1,0,0,0,0,0,0,0,NULL,&atx[29],&atx[159],0,&atx[164]} ,
  {0, "scale-factor" ,128,0,0,0,0,0,0,0,NULL,&atx[3],NULL,0,&atx[160]} ,
  {0, "x" ,128,1,0,0,0,0,0,0,NULL,&atx[3],NULL,0,&atx[161]} ,
  {0, "y" ,128,2,0,0,0,0,0,0,NULL,&atx[3],NULL,0,&atx[162]} ,
  {0, "z" ,128,3,0,0,0,0,0,0,NULL,&atx[3],NULL,0,NULL} ,
  {0, "radius" ,128,1,0,0,0,0,0,0,NULL,&atx[164],NULL,0,NULL} ,
  {437, "RealValue" ,1,0,0,0,0,0,0,0,NULL,&atx[29],&atx[165],0,&atx[37]} ,
  {0, "scale-factor" ,128,0,0,0,0,0,0,0,NULL,&atx[3],NULL,0,&atx[166]} ,
  {0, "scaled-integer-value" ,128,1,0,0,0,0,0,0,NULL,&atx[3],NULL,0,NULL} ,
  {0, "cone" ,128,1,0,0,0,0,0,0,NULL,&atx[168],NULL,0,&atx[172]} ,
  {406, "Cone" ,1,0,0,0,0,1,0,0,NULL,&atx[29],&atx[169],0,&atx[173]} ,
  {0, "axis-top" ,128,0,0,0,0,0,0,0,NULL,&atx[158],NULL,0,&atx[170]} ,
  {0, "axis-bottom" ,128,1,0,0,0,0,0,0,NULL,&atx[158],NULL,0,&atx[171]} ,
  {0, "radius-bottom" ,128,2,0,0,0,0,0,0,NULL,&atx[164],NULL,0,NULL} ,
  {0, "cylinder" ,128,2,0,0,0,0,0,0,NULL,&atx[173],NULL,0,&atx[177]} ,
  {407, "Cylinder" ,1,0,0,0,0,1,0,0,NULL,&atx[29],&atx[174],0,&atx[178]} ,
  {0, "axis-top" ,128,0,0,0,0,0,0,0,NULL,&atx[158],NULL,0,&atx[175]} ,
  {0, "axis-bottom" ,128,1,0,0,0,0,0,0,NULL,&atx[158],NULL,0,&atx[176]} ,
  {0, "radius" ,128,2,0,0,0,0,0,0,NULL,&atx[164],NULL,0,NULL} ,
  {0, "brick" ,128,3,0,0,0,0,0,0,NULL,&atx[178],NULL,0,NULL} ,
  {408, "Brick" ,1,0,0,0,0,1,0,0,NULL,&atx[29],&atx[179],0,&atx[33]} ,
  {0, "corner-000" ,128,0,0,0,0,0,0,0,NULL,&atx[158],NULL,0,&atx[180]} ,
  {0, "corner-001" ,128,1,0,0,0,0,0,0,NULL,&atx[158],NULL,0,&atx[181]} ,
  {0, "corner-010" ,128,2,0,0,0,0,0,0,NULL,&atx[158],NULL,0,&atx[182]} ,
  {0, "corner-011" ,128,3,0,0,0,0,0,0,NULL,&atx[158],NULL,0,&atx[183]} ,
  {0, "corner-100" ,128,4,0,0,0,0,0,0,NULL,&atx[158],NULL,0,&atx[184]} ,
  {0, "corner-101" ,128,5,0,0,0,0,0,0,NULL,&atx[158],NULL,0,&atx[185]} ,
  {0, "corner-110" ,128,6,0,0,0,0,0,0,NULL,&atx[158],NULL,0,&atx[186]} ,
  {0, "corner-111" ,128,7,0,0,0,0,0,0,NULL,&atx[158],NULL,0,NULL} ,
  {0, "alignment" ,128,2,0,0,0,0,0,0,NULL,&atx[188],NULL,0,&atx[210]} ,
  {404, "Chem-graph-alignment" ,1,0,0,0,0,1,0,0,NULL,&atx[29],&atx[189],0,&atx[156]} ,
  {0, "dimension" ,128,0,0,0,1,0,0,0,&avnx[46],&atx[3],NULL,0,&atx[190]} ,
  {0, "biostruc-ids" ,128,1,0,0,0,0,0,0,NULL,&atx[14],&atx[191],0,&atx[192]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[93],NULL,0,NULL} ,
  {0, "alignment" ,128,2,0,0,0,0,0,0,NULL,&atx[14],&atx[193],0,&atx[194]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[106],NULL,0,NULL} ,
  {0, "domain" ,128,3,0,1,0,0,0,0,NULL,&atx[14],&atx[195],0,&atx[196]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[106],NULL,0,NULL} ,
  {0, "transform" ,128,4,0,1,0,0,0,0,NULL,&atx[14],&atx[197],0,&atx[198]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[33],NULL,0,NULL} ,
  {0, "aligndata" ,128,5,0,1,0,0,0,0,NULL,&atx[14],&atx[199],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[200],NULL,0,NULL} ,
  {435, "Align-stats" ,1,0,0,0,0,0,0,0,NULL,&atx[29],&atx[201],0,&atx[158]} ,
  {0, "descr" ,128,0,0,1,0,0,0,0,NULL,&atx[8],NULL,0,&atx[202]} ,
  {0, "scale-factor" ,128,1,0,1,0,0,0,0,NULL,&atx[3],NULL,0,&atx[203]} ,
  {0, "vast-score" ,128,2,0,1,0,0,0,0,NULL,&atx[3],NULL,0,&atx[204]} ,
  {0, "vast-mlogp" ,128,3,0,1,0,0,0,0,NULL,&atx[3],NULL,0,&atx[205]} ,
  {0, "align-res" ,128,4,0,1,0,0,0,0,NULL,&atx[3],NULL,0,&atx[206]} ,
  {0, "rmsd" ,128,5,0,1,0,0,0,0,NULL,&atx[3],NULL,0,&atx[207]} ,
  {0, "blast-score" ,128,6,0,1,0,0,0,0,NULL,&atx[3],NULL,0,&atx[208]} ,
  {0, "blast-mlogp" ,128,7,0,1,0,0,0,0,NULL,&atx[3],NULL,0,&atx[209]} ,
  {0, "other-score" ,128,8,0,1,0,0,0,0,NULL,&atx[3],NULL,0,NULL} ,
  {0, "similarity" ,128,3,0,0,0,0,0,0,NULL,&atx[211],NULL,0,&atx[219]} ,
  {427, "Region-similarity" ,1,0,0,0,0,0,0,0,NULL,&atx[29],&atx[212],0,&atx[91]} ,
  {0, "dimension" ,128,0,0,0,1,0,0,0,&avnx[47],&atx[3],NULL,0,&atx[213]} ,
  {0, "biostruc-ids" ,128,1,0,0,0,0,0,0,NULL,&atx[14],&atx[214],0,&atx[215]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[93],NULL,0,NULL} ,
  {0, "similarity" ,128,2,0,0,0,0,0,0,NULL,&atx[14],&atx[216],0,&atx[217]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[140],NULL,0,NULL} ,
  {0, "transform" ,128,3,0,0,0,0,0,0,NULL,&atx[14],&atx[218],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[33],NULL,0,NULL} ,
  {0, "indirect" ,128,4,0,0,0,0,0,0,NULL,&atx[91],NULL,0,NULL} };

static AsnModule ampx[1] = {
  { "MMDB-Features" , "mmdb3.h63",&atx[0],NULL,NULL,0,0} };

static AsnValxNodePtr avn = avnx;
static AsnTypePtr at = atx;
static AsnModulePtr amp = ampx;



/**************************************************
*
*    Defines for Module MMDB-Features
*
**************************************************/

#define BIOSTRUC_FEATURE_SET &at[0]
#define BIOSTRUC_FEATURE_SET_id &at[1]
#define BIOSTRUC_FEATURE_SET_descr &at[4]
#define BIOSTRUC_FEATURE_SET_descr_E &at[5]
#define BIOSTRUC_FEATURE_SET_features &at[15]
#define BIOSTRUC_FEATURE_SET_features_E &at[16]

#define CHEM_GRAPH_PNTRS &at[106]
#define CHEM_GRAPH_PNTRS_atoms &at[107]
#define CHEM_GRAPH_PNTRS_residues &at[119]
#define CHEM_GRAPH_PNTRS_molecules &at[134]

#define ATOM_PNTRS &at[108]
#define ATOM_PNTRS_number_of_ptrs &at[109]
#define ATOM_PNTRS_molecule_ids &at[110]
#define ATOM_PNTRS_molecule_ids_E &at[111]
#define ATOM_PNTRS_residue_ids &at[113]
#define ATOM_PNTRS_residue_ids_E &at[114]
#define ATOM_PNTRS_atom_ids &at[116]
#define ATOM_PNTRS_atom_ids_E &at[117]

#define CHEM_GRAPH_ALIGNMENT &at[188]
#define CHEM_GRAPH_ALIGNMENT_dimension &at[189]
#define CHEM_GRAPH_ALIGNMENT_biostruc_ids &at[190]
#define CHEM_GRAPH_ALIGNMENT_biostruc_ids_E &at[191]
#define CHEM_GRAPH_ALIGNMENT_alignment &at[192]
#define CHEM_GRAPH_ALIGNMENT_alignment_E &at[193]
#define CHEM_GRAPH_ALIGNMENT_domain &at[194]
#define CHEM_GRAPH_ALIGNMENT_domain_E &at[195]
#define CHEM_GRAPH_ALIGNMENT_transform &at[196]
#define CHEM_GRAPH_ALIGNMENT_transform_E &at[197]
#define CHEM_GRAPH_ALIGNMENT_aligndata &at[198]
#define CHEM_GRAPH_ALIGNMENT_aligndata_E &at[199]

#define SPHERE &at[156]
#define SPHERE_center &at[157]
#define SPHERE_radius &at[163]

#define CONE &at[168]
#define CONE_axis_top &at[169]
#define CONE_axis_bottom &at[170]
#define CONE_radius_bottom &at[171]

#define CYLINDER &at[173]
#define CYLINDER_axis_top &at[174]
#define CYLINDER_axis_bottom &at[175]
#define CYLINDER_radius &at[176]

#define BRICK &at[178]
#define BRICK_corner_000 &at[179]
#define BRICK_corner_001 &at[180]
#define BRICK_corner_010 &at[181]
#define BRICK_corner_011 &at[182]
#define BRICK_corner_100 &at[183]
#define BRICK_corner_101 &at[184]
#define BRICK_corner_110 &at[185]
#define BRICK_corner_111 &at[186]

#define TRANSFORM &at[33]
#define TRANSFORM_id &at[34]
#define TRANSFORM_moves &at[35]
#define TRANSFORM_moves_E &at[36]

#define BIOSTRUC_FEATURE_SET_ID &at[2]

#define BIOSTRUC_FEATURE_ID &at[19]

#define BIOSTRUC_FEATURE_SET_DESCR &at[6]
#define BIOSTRUC_FEATURE_SET_DESCR_name &at[7]
#define BIOSTRUC_FEATURE_SET_DESCR_pdb_comment &at[9]
#define BIOSTRUC_FEATURE_SET_DESCR_other_comment &at[10]
#define BIOSTRUC_FEATURE_SET_DESCR_attribution &at[11]

#define BIOSTRUC_FEATURE &at[17]
#define BIOSTRUC_FEATURE_id &at[18]
#define BIOSTRUC_FEATURE_name &at[20]
#define BIOSTRUC_FEATURE_type &at[21]
#define BIOSTRUC_FEATURE_property &at[22]
#define BIOSTRUC_FEATURE_property_color &at[23]
#define BIOSTRUC_FEATURE_property_render &at[30]
#define BIOSTRUC_FEATURE_property_transform &at[32]
#define BIOSTRUC_FEATURE_property_camera &at[56]
#define BIOSTRUC_FEATURE_property_script &at[82]
#define BIOSTRUC_FEATURE_property_user &at[102]
#define BIOSTRUC_FEATURE_location &at[104]
#define BIOSTRUC_FEATURE_location_subgraph &at[105]
#define BIOSTRUC_FEATURE_location_region &at[139]
#define BIOSTRUC_FEATURE_location_alignment &at[187]
#define BIOSTRUC_FEATURE_location_similarity &at[210]
#define BIOSTRUC_FEATURE_location_indirect &at[219]

#define COLOR_PROP &at[24]
#define COLOR_PROP_r &at[25]
#define COLOR_PROP_g &at[26]
#define COLOR_PROP_b &at[27]
#define COLOR_PROP_name &at[28]

#define RENDER_PROP &at[31]

#define CAMERA &at[57]
#define CAMERA_x &at[58]
#define CAMERA_y &at[59]
#define CAMERA_distance &at[60]
#define CAMERA_angle &at[61]
#define CAMERA_scale &at[62]
#define CAMERA_modelview &at[63]

#define BIOSTRUC_SCRIPT &at[83]
#define BIOSTRUC_SCRIPT_E &at[84]

#define REGION_PNTRS &at[140]
#define REGION_PNTRS_model_id &at[141]
#define REGION_PNTRS_region &at[143]
#define REGION_PNTRS_region_site &at[144]
#define REGION_PNTRS_region_site_E &at[145]
#define REGION_PNTRS_region_boundary &at[152]
#define REGION_PNTRS_region_boundary_E &at[153]

#define REGION_SIMILARITY &at[211]
#define REGION_SIMILARITY_dimension &at[212]
#define REGION_SIMILARITY_biostruc_ids &at[213]
#define REGION_SIMILARITY_biostruc_ids_E &at[214]
#define REGION_SIMILARITY_similarity &at[215]
#define REGION_SIMILARITY_similarity_E &at[216]
#define REGION_SIMILARITY_transform &at[217]
#define REGION_SIMILARITY_transform_E &at[218]

#define OTHER_FEATURE &at[91]
#define OTHER_FEATURE_biostruc_id &at[92]
#define OTHER_FEATURE_set &at[94]
#define OTHER_FEATURE_feature &at[95]

#define RESIDUE_PNTRS &at[120]
#define RESIDUE_PNTRS_explicit &at[121]
#define RESIDUE_PNTRS_interval &at[128]
#define RESIDUE_PNTRS_interval_E &at[129]

#define MOLECULE_PNTRS &at[135]
#define MOLECULE_PNTRS_number_of_ptrs &at[136]
#define MOLECULE_PNTRS_molecule_ids &at[137]
#define MOLECULE_PNTRS_molecule_ids_E &at[138]

#define RESIDUE_EXPLICIT_PNTRS &at[122]
#define RESIDUE_EXPLICIT_PNTRS_number_of_ptrs &at[123]
#define RESIDUE_EXPLICIT_PNTRS_molecule_ids &at[124]
#define RESIDUE_EXPLICIT_PNTRS_molecule_ids_E &at[125]
#define RESIDUE_EXPLICIT_PNTRS_residue_ids &at[126]
#define RESIDUE_EXPLICIT_PNTRS_residue_ids_E &at[127]

#define RESIDUE_INTERVAL_PNTR &at[130]
#define RESIDUE_INTERVAL_PNTR_molecule_id &at[131]
#define RESIDUE_INTERVAL_PNTR_from &at[132]
#define RESIDUE_INTERVAL_PNTR_to &at[133]

#define REGION_COORDINATES &at[146]
#define REGION_COORDINATES_model_coord_set_id &at[147]
#define REGION_COORDINATES_number_of_coords &at[149]
#define REGION_COORDINATES_coordinate_indices &at[150]
#define REGION_COORDINATES_coordinate_indices_E &at[151]

#define REGION_BOUNDARY &at[154]
#define REGION_BOUNDARY_sphere &at[155]
#define REGION_BOUNDARY_cone &at[167]
#define REGION_BOUNDARY_cylinder &at[172]
#define REGION_BOUNDARY_brick &at[177]

#define ALIGN_STATS &at[200]
#define ALIGN_STATS_descr &at[201]
#define ALIGN_STATS_scale_factor &at[202]
#define ALIGN_STATS_vast_score &at[203]
#define ALIGN_STATS_vast_mlogp &at[204]
#define ALIGN_STATS_align_res &at[205]
#define ALIGN_STATS_rmsd &at[206]
#define ALIGN_STATS_blast_score &at[207]
#define ALIGN_STATS_blast_mlogp &at[208]
#define ALIGN_STATS_other_score &at[209]

#define MODEL_SPACE_POINT &at[158]
#define MODEL_SPACE_POINT_scale_factor &at[159]
#define MODEL_SPACE_POINT_x &at[160]
#define MODEL_SPACE_POINT_y &at[161]
#define MODEL_SPACE_POINT_z &at[162]

#define REALVALUE &at[164]
#define REALVALUE_scale_factor &at[165]
#define REALVALUE_scaled_integer_value &at[166]

#define MOVE &at[37]
#define MOVE_rotate &at[38]
#define MOVE_translate &at[50]

#define ROT_MATRIX &at[39]
#define ROT_MATRIX_scale_factor &at[40]
#define ROT_MATRIX_rot_11 &at[41]
#define ROT_MATRIX_rot_12 &at[42]
#define ROT_MATRIX_rot_13 &at[43]
#define ROT_MATRIX_rot_21 &at[44]
#define ROT_MATRIX_rot_22 &at[45]
#define ROT_MATRIX_rot_23 &at[46]
#define ROT_MATRIX_rot_31 &at[47]
#define ROT_MATRIX_rot_32 &at[48]
#define ROT_MATRIX_rot_33 &at[49]

#define TRANS_MATRIX &at[51]
#define TRANS_MATRIX_scale_factor &at[52]
#define TRANS_MATRIX_tran_1 &at[53]
#define TRANS_MATRIX_tran_2 &at[54]
#define TRANS_MATRIX_tran_3 &at[55]

#define GL_MATRIX &at[64]
#define GL_MATRIX_scale &at[65]
#define GL_MATRIX_m11 &at[66]
#define GL_MATRIX_m12 &at[67]
#define GL_MATRIX_m13 &at[68]
#define GL_MATRIX_m14 &at[69]
#define GL_MATRIX_m21 &at[70]
#define GL_MATRIX_m22 &at[71]
#define GL_MATRIX_m23 &at[72]
#define GL_MATRIX_m24 &at[73]
#define GL_MATRIX_m31 &at[74]
#define GL_MATRIX_m32 &at[75]
#define GL_MATRIX_m33 &at[76]
#define GL_MATRIX_m34 &at[77]
#define GL_MATRIX_m41 &at[78]
#define GL_MATRIX_m42 &at[79]
#define GL_MATRIX_m43 &at[80]
#define GL_MATRIX_m44 &at[81]

#define BIOSTRUC_SCRIPT_STEP &at[85]
#define BIOSTRUC_SCRIPT_STEP_step_id &at[86]
#define BIOSTRUC_SCRIPT_STEP_step_name &at[88]
#define BIOSTRUC_SCRIPT_STEP_feature_do &at[89]
#define BIOSTRUC_SCRIPT_STEP_feature_do_E &at[90]
#define BIOSTRUC_SCRIPT_STEP_camera_move &at[96]
#define BIOSTRUC_SCRIPT_STEP_pause &at[97]
#define BIOSTRUC_SCRIPT_STEP_waitevent &at[98]
#define BIOSTRUC_SCRIPT_STEP_extra &at[100]
#define BIOSTRUC_SCRIPT_STEP_jump &at[101]

#define STEP_ID &at[87]
