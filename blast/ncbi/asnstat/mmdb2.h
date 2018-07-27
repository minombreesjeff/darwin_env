/***********************************************************************
*
**
*        Automatic header module from ASNTOOL
*
************************************************************************/

#ifndef _ASNTOOL_
#include <asn.h>
#endif

static char * asnfilename = "mmdb2.h60";
static AsnValxNode avnx[27] = {
    {20,"ncbi-vector" ,1,0.0,&avnx[1] } ,
    {20,"ncbi-backbone" ,2,0.0,&avnx[2] } ,
    {20,"ncbi-all-atom" ,3,0.0,&avnx[3] } ,
    {20,"pdb-model" ,4,0.0,&avnx[4] } ,
    {20,"other" ,255,0.0,NULL } ,
    {20,"angstroms" ,1,0.0,&avnx[6] } ,
    {20,"nanometers" ,2,0.0,&avnx[7] } ,
    {20,"other" ,3,0.0,&avnx[8] } ,
    {20,"unknown" ,255,0.0,NULL } ,
    {20,"b" ,1,0.0,&avnx[10] } ,
    {20,"u" ,2,0.0,&avnx[11] } ,
    {20,"other" ,3,0.0,&avnx[12] } ,
    {20,"unknown" ,255,0.0,NULL } ,
    {20,"fractional" ,1,0.0,&avnx[14] } ,
    {20,"electrons" ,2,0.0,&avnx[15] } ,
    {20,"other" ,3,0.0,&avnx[16] } ,
    {20,"unknown" ,255,0.0,NULL } ,
    {20,"electrons-per-unit-volume" ,1,0.0,&avnx[18] } ,
    {20,"arbitrary-scale" ,2,0.0,&avnx[19] } ,
    {20,"other" ,3,0.0,&avnx[20] } ,
    {20,"unknown" ,255,0.0,NULL } ,
    {20,"x" ,1,0.0,&avnx[22] } ,
    {20,"y" ,2,0.0,&avnx[23] } ,
    {20,"z" ,3,0.0,NULL } ,
    {20,"x" ,1,0.0,&avnx[25] } ,
    {20,"y" ,2,0.0,&avnx[26] } ,
    {20,"z" ,3,0.0,NULL } };

static AsnType atx[151] = {
  {401, "Biostruc-model" ,1,0,0,0,0,1,0,0,NULL,&atx[32],&atx[1],0,&atx[2]} ,
  {0, "id" ,128,0,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[4]} ,
  {402, "Model-id" ,1,0,0,0,0,1,0,0,NULL,&atx[3],NULL,0,&atx[37]} ,
  {302, "INTEGER" ,0,2,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "type" ,128,1,0,0,0,0,0,0,NULL,&atx[5],NULL,0,&atx[6]} ,
  {414, "Model-type" ,1,0,0,0,0,0,0,0,NULL,&atx[3],&avnx[0],0,&atx[8]} ,
  {0, "descr" ,128,2,0,1,0,0,0,0,NULL,&atx[18],&atx[7],0,&atx[19]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[8],NULL,0,NULL} ,
  {415, "Model-descr" ,1,0,0,0,0,0,0,0,NULL,&atx[17],&atx[9],0,&atx[20]} ,
  {0, "name" ,128,0,0,0,0,0,0,0,NULL,&atx[10],NULL,0,&atx[11]} ,
  {323, "VisibleString" ,0,26,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "pdb-reso" ,128,1,0,0,0,0,0,0,NULL,&atx[10],NULL,0,&atx[12]} ,
  {0, "pdb-method" ,128,2,0,0,0,0,0,0,NULL,&atx[10],NULL,0,&atx[13]} ,
  {0, "pdb-comment" ,128,3,0,0,0,0,0,0,NULL,&atx[10],NULL,0,&atx[14]} ,
  {0, "other-comment" ,128,4,0,0,0,0,0,0,NULL,&atx[10],NULL,0,&atx[15]} ,
  {0, "attribution" ,128,5,0,0,0,0,0,0,NULL,&atx[16],NULL,0,NULL} ,
  {413, "Pub" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[5]} ,
  {315, "CHOICE" ,0,-1,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {312, "SEQUENCE OF" ,0,16,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "model-space" ,128,3,0,1,0,0,0,0,NULL,&atx[20],NULL,0,&atx[33]} ,
  {416, "Model-space" ,1,0,0,0,0,0,0,0,NULL,&atx[32],&atx[21],0,&atx[35]} ,
  {0, "coordinate-units" ,128,0,0,0,0,0,0,0,NULL,&atx[22],&avnx[5],0,&atx[23]} ,
  {310, "ENUMERATED" ,0,10,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "thermal-factor-units" ,128,1,0,1,0,0,0,0,NULL,&atx[22],&avnx[9],0,&atx[24]} ,
  {0, "occupancy-factor-units" ,128,2,0,1,0,0,0,0,NULL,&atx[22],&avnx[13],0,&atx[25]} ,
  {0, "density-units" ,128,3,0,1,0,0,0,0,NULL,&atx[22],&avnx[17],0,&atx[26]} ,
  {0, "reference-frame" ,128,4,0,1,0,0,0,0,NULL,&atx[27],NULL,0,NULL} ,
  {418, "Reference-frame" ,1,0,0,0,0,0,0,0,NULL,&atx[32],&atx[28],0,&atx[42]} ,
  {0, "biostruc-id" ,128,0,0,0,0,0,0,0,NULL,&atx[29],NULL,0,&atx[30]} ,
  {412, "Biostruc-id" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[16]} ,
  {0, "rotation-translation" ,128,1,0,1,0,0,0,0,NULL,&atx[31],NULL,0,NULL} ,
  {411, "Transform" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[29]} ,
  {311, "SEQUENCE" ,0,16,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "model-coordinates" ,128,4,0,1,0,0,0,0,NULL,&atx[18],&atx[34],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[35],NULL,0,NULL} ,
  {417, "Model-coordinate-set" ,1,0,0,0,0,0,0,0,NULL,&atx[32],&atx[36],0,&atx[27]} ,
  {0, "id" ,128,0,0,1,0,0,0,0,NULL,&atx[37],NULL,0,&atx[38]} ,
  {403, "Model-coordinate-set-id" ,1,0,0,0,0,1,0,0,NULL,&atx[3],NULL,0,&atx[97]} ,
  {0, "descr" ,128,1,0,1,0,0,0,0,NULL,&atx[18],&atx[39],0,&atx[40]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[8],NULL,0,NULL} ,
  {0, "coordinates" ,128,2,0,0,0,0,0,0,NULL,&atx[17],&atx[41],0,NULL} ,
  {0, "literal" ,128,0,0,0,0,0,0,0,NULL,&atx[42],NULL,0,&atx[149]} ,
  {419, "Coordinates" ,1,0,0,0,0,0,0,0,NULL,&atx[17],&atx[43],0,&atx[44]} ,
  {0, "atomic" ,128,0,0,0,0,0,0,0,NULL,&atx[44],NULL,0,&atx[94]} ,
  {420, "Atomic-coordinates" ,1,0,0,0,0,0,0,0,NULL,&atx[32],&atx[45],0,&atx[95]} ,
  {0, "number-of-points" ,128,0,0,0,0,0,0,0,NULL,&atx[3],NULL,0,&atx[46]} ,
  {0, "atoms" ,128,1,0,0,0,0,0,0,NULL,&atx[47],NULL,0,&atx[48]} ,
  {405, "Atom-pntrs" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[150]} ,
  {0, "sites" ,128,2,0,0,0,0,0,0,NULL,&atx[49],NULL,0,&atx[57]} ,
  {423, "Model-space-points" ,1,0,0,0,0,0,0,0,NULL,&atx[32],&atx[50],0,&atx[58]} ,
  {0, "scale-factor" ,128,0,0,0,0,0,0,0,NULL,&atx[3],NULL,0,&atx[51]} ,
  {0, "x" ,128,1,0,0,0,0,0,0,NULL,&atx[18],&atx[52],0,&atx[53]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[3],NULL,0,NULL} ,
  {0, "y" ,128,2,0,0,0,0,0,0,NULL,&atx[18],&atx[54],0,&atx[55]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[3],NULL,0,NULL} ,
  {0, "z" ,128,3,0,0,0,0,0,0,NULL,&atx[18],&atx[56],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[3],NULL,0,NULL} ,
  {0, "temperature-factors" ,128,3,0,1,0,0,0,0,NULL,&atx[58],NULL,0,&atx[79]} ,
  {424, "Atomic-temperature-factors" ,1,0,0,0,0,0,0,0,NULL,&atx[17],&atx[59],0,&atx[80]} ,
  {0, "isotropic" ,128,0,0,0,0,0,0,0,NULL,&atx[60],NULL,0,&atx[64]} ,
  {428, "Isotropic-temperature-factors" ,1,0,0,0,0,0,0,0,NULL,&atx[32],&atx[61],0,&atx[65]} ,
  {0, "scale-factor" ,128,0,0,0,0,0,0,0,NULL,&atx[3],NULL,0,&atx[62]} ,
  {0, "b" ,128,1,0,0,0,0,0,0,NULL,&atx[18],&atx[63],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[3],NULL,0,NULL} ,
  {0, "anisotropic" ,128,1,0,0,0,0,0,0,NULL,&atx[65],NULL,0,NULL} ,
  {429, "Anisotropic-temperature-factors" ,1,0,0,0,0,0,0,0,NULL,&atx[32],&atx[66],0,&atx[87]} ,
  {0, "scale-factor" ,128,0,0,0,0,0,0,0,NULL,&atx[3],NULL,0,&atx[67]} ,
  {0, "b-11" ,128,1,0,0,0,0,0,0,NULL,&atx[18],&atx[68],0,&atx[69]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[3],NULL,0,NULL} ,
  {0, "b-12" ,128,2,0,0,0,0,0,0,NULL,&atx[18],&atx[70],0,&atx[71]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[3],NULL,0,NULL} ,
  {0, "b-13" ,128,3,0,0,0,0,0,0,NULL,&atx[18],&atx[72],0,&atx[73]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[3],NULL,0,NULL} ,
  {0, "b-22" ,128,4,0,0,0,0,0,0,NULL,&atx[18],&atx[74],0,&atx[75]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[3],NULL,0,NULL} ,
  {0, "b-23" ,128,5,0,0,0,0,0,0,NULL,&atx[18],&atx[76],0,&atx[77]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[3],NULL,0,NULL} ,
  {0, "b-33" ,128,6,0,0,0,0,0,0,NULL,&atx[18],&atx[78],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[3],NULL,0,NULL} ,
  {0, "occupancies" ,128,4,0,1,0,0,0,0,NULL,&atx[80],NULL,0,&atx[84]} ,
  {425, "Atomic-occupancies" ,1,0,0,0,0,0,0,0,NULL,&atx[32],&atx[81],0,&atx[85]} ,
  {0, "scale-factor" ,128,0,0,0,0,0,0,0,NULL,&atx[3],NULL,0,&atx[82]} ,
  {0, "o" ,128,1,0,0,0,0,0,0,NULL,&atx[18],&atx[83],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[3],NULL,0,NULL} ,
  {0, "alternate-conf-ids" ,128,5,0,1,0,0,0,0,NULL,&atx[85],NULL,0,&atx[88]} ,
  {426, "Alternate-conformation-ids" ,1,0,0,0,0,0,0,0,NULL,&atx[18],&atx[86],0,&atx[90]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[87],NULL,0,NULL} ,
  {430, "Alternate-conformation-id" ,1,0,0,0,0,0,0,0,NULL,&atx[10],NULL,0,&atx[108]} ,
  {0, "conf-ensembles" ,128,6,0,1,0,0,0,0,NULL,&atx[18],&atx[89],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[90],NULL,0,NULL} ,
  {427, "Conformation-ensemble" ,1,0,0,0,0,0,0,0,NULL,&atx[32],&atx[91],0,&atx[60]} ,
  {0, "name" ,128,0,0,0,0,0,0,0,NULL,&atx[10],NULL,0,&atx[92]} ,
  {0, "alt-conf-ids" ,128,1,0,0,0,0,0,0,NULL,&atx[18],&atx[93],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[87],NULL,0,NULL} ,
  {0, "surface" ,128,1,0,0,0,0,0,0,NULL,&atx[95],NULL,0,&atx[137]} ,
  {421, "Surface-coordinates" ,1,0,0,0,0,0,0,0,NULL,&atx[32],&atx[96],0,&atx[138]} ,
  {0, "contents" ,128,0,0,0,0,0,0,0,NULL,&atx[97],NULL,0,&atx[98]} ,
  {404, "Chem-graph-pntrs" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[47]} ,
  {0, "surface" ,128,1,0,0,0,0,0,0,NULL,&atx[17],&atx[99],0,NULL} ,
  {0, "sphere" ,128,0,0,0,0,0,0,0,NULL,&atx[100],NULL,0,&atx[101]} ,
  {407, "Sphere" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[102]} ,
  {0, "cone" ,128,1,0,0,0,0,0,0,NULL,&atx[102],NULL,0,&atx[103]} ,
  {408, "Cone" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[104]} ,
  {0, "cylinder" ,128,2,0,0,0,0,0,0,NULL,&atx[104],NULL,0,&atx[105]} ,
  {409, "Cylinder" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[106]} ,
  {0, "brick" ,128,3,0,0,0,0,0,0,NULL,&atx[106],NULL,0,&atx[107]} ,
  {410, "Brick" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[31]} ,
  {0, "tmesh" ,128,4,0,0,0,0,0,0,NULL,&atx[108],NULL,0,&atx[120]} ,
  {431, "T-mesh" ,1,0,0,0,0,0,0,0,NULL,&atx[32],&atx[109],0,&atx[121]} ,
  {0, "number-of-points" ,128,0,0,0,0,0,0,0,NULL,&atx[3],NULL,0,&atx[110]} ,
  {0, "scale-factor" ,128,1,0,0,0,0,0,0,NULL,&atx[3],NULL,0,&atx[111]} ,
  {0, "swap" ,128,2,0,0,0,0,0,0,NULL,&atx[18],&atx[112],0,&atx[114]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[113],NULL,0,NULL} ,
  {301, "BOOLEAN" ,0,1,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "x" ,128,3,0,0,0,0,0,0,NULL,&atx[18],&atx[115],0,&atx[116]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[3],NULL,0,NULL} ,
  {0, "y" ,128,4,0,0,0,0,0,0,NULL,&atx[18],&atx[117],0,&atx[118]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[3],NULL,0,NULL} ,
  {0, "z" ,128,5,0,0,0,0,0,0,NULL,&atx[18],&atx[119],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[3],NULL,0,NULL} ,
  {0, "triangles" ,128,5,0,0,0,0,0,0,NULL,&atx[121],NULL,0,NULL} ,
  {432, "Triangles" ,1,0,0,0,0,0,0,0,NULL,&atx[32],&atx[122],0,NULL} ,
  {0, "number-of-points" ,128,0,0,0,0,0,0,0,NULL,&atx[3],NULL,0,&atx[123]} ,
  {0, "scale-factor" ,128,1,0,0,0,0,0,0,NULL,&atx[3],NULL,0,&atx[124]} ,
  {0, "x" ,128,2,0,0,0,0,0,0,NULL,&atx[18],&atx[125],0,&atx[126]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[3],NULL,0,NULL} ,
  {0, "y" ,128,3,0,0,0,0,0,0,NULL,&atx[18],&atx[127],0,&atx[128]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[3],NULL,0,NULL} ,
  {0, "z" ,128,4,0,0,0,0,0,0,NULL,&atx[18],&atx[129],0,&atx[130]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[3],NULL,0,NULL} ,
  {0, "number-of-triangles" ,128,5,0,0,0,0,0,0,NULL,&atx[3],NULL,0,&atx[131]} ,
  {0, "v1" ,128,6,0,0,0,0,0,0,NULL,&atx[18],&atx[132],0,&atx[133]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[3],NULL,0,NULL} ,
  {0, "v2" ,128,7,0,0,0,0,0,0,NULL,&atx[18],&atx[134],0,&atx[135]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[3],NULL,0,NULL} ,
  {0, "v3" ,128,8,0,0,0,0,0,0,NULL,&atx[18],&atx[136],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[3],NULL,0,NULL} ,
  {0, "density" ,128,2,0,0,0,0,0,0,NULL,&atx[138],NULL,0,NULL} ,
  {422, "Density-coordinates" ,1,0,0,0,0,0,0,0,NULL,&atx[32],&atx[139],0,&atx[49]} ,
  {0, "contents" ,128,0,0,0,0,0,0,0,NULL,&atx[97],NULL,0,&atx[140]} ,
  {0, "grid-corners" ,128,1,0,0,0,0,0,0,NULL,&atx[106],NULL,0,&atx[141]} ,
  {0, "grid-steps-x" ,128,2,0,0,0,0,0,0,NULL,&atx[3],NULL,0,&atx[142]} ,
  {0, "grid-steps-y" ,128,3,0,0,0,0,0,0,NULL,&atx[3],NULL,0,&atx[143]} ,
  {0, "grid-steps-z" ,128,4,0,0,0,0,0,0,NULL,&atx[3],NULL,0,&atx[144]} ,
  {0, "fastest-varying" ,128,5,0,0,0,0,0,0,NULL,&atx[22],&avnx[21],0,&atx[145]} ,
  {0, "slowest-varying" ,128,6,0,0,0,0,0,0,NULL,&atx[22],&avnx[24],0,&atx[146]} ,
  {0, "scale-factor" ,128,7,0,0,0,0,0,0,NULL,&atx[3],NULL,0,&atx[147]} ,
  {0, "density" ,128,8,0,0,0,0,0,0,NULL,&atx[18],&atx[148],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[3],NULL,0,NULL} ,
  {0, "reference" ,128,1,0,0,0,0,0,0,NULL,&atx[150],NULL,0,NULL} ,
  {406, "Chem-graph-alignment" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[100]} };

static AsnModule ampx[1] = {
  { "MMDB-Structural-model" , "mmdb2.h60",&atx[0],NULL,NULL,0,0} };

static AsnValxNodePtr avn = avnx;
static AsnTypePtr at = atx;
static AsnModulePtr amp = ampx;



/**************************************************
*
*    Defines for Module MMDB-Structural-model
*
**************************************************/

#define BIOSTRUC_MODEL &at[0]
#define BIOSTRUC_MODEL_id &at[1]
#define BIOSTRUC_MODEL_type &at[4]
#define BIOSTRUC_MODEL_descr &at[6]
#define BIOSTRUC_MODEL_descr_E &at[7]
#define BIOSTRUC_MODEL_model_space &at[19]
#define BIOSTRUC_MODEL_model_coordinates &at[33]
#define BIOSTRUC_MODEL_model_coordinates_E &at[34]

#define MODEL_ID &at[2]

#define MODEL_COORDINATE_SET_ID &at[37]

#define MODEL_TYPE &at[5]

#define MODEL_DESCR &at[8]
#define MODEL_DESCR_name &at[9]
#define MODEL_DESCR_pdb_reso &at[11]
#define MODEL_DESCR_pdb_method &at[12]
#define MODEL_DESCR_pdb_comment &at[13]
#define MODEL_DESCR_other_comment &at[14]
#define MODEL_DESCR_attribution &at[15]

#define MODEL_SPACE &at[20]
#define MODEL_SPACE_coordinate_units &at[21]
#define MODEL_SPACE_thermal_factor_units &at[23]
#define MODEL_SPACE_occupancy_factor_units &at[24]
#define MODEL_SPACE_density_units &at[25]
#define MODEL_SPACE_reference_frame &at[26]

#define MODEL_COORDINATE_SET &at[35]
#define MODEL_COORDINATE_SET_id &at[36]
#define MODEL_COORDINATE_SET_descr &at[38]
#define MODEL_COORDINATE_SET_descr_E &at[39]
#define MODEL_COORDINATE_SET_coordinates &at[40]
#define MODEL_COORDINATE_SET_coordinates_literal &at[41]
#define MODEL_COORDINATE_SET_coordinates_reference &at[149]

#define REFERENCE_FRAME &at[27]
#define REFERENCE_FRAME_biostruc_id &at[28]
#define REFERENCE_FRAME_rotation_translation &at[30]

#define COORDINATES &at[42]
#define COORDINATES_atomic &at[43]
#define COORDINATES_surface &at[94]
#define COORDINATES_density &at[137]

#define ATOMIC_COORDINATES &at[44]
#define ATOMIC_COORDINATES_number_of_points &at[45]
#define ATOMIC_COORDINATES_atoms &at[46]
#define ATOMIC_COORDINATES_sites &at[48]
#define ATOMIC_COORDINATES_temperature_factors &at[57]
#define ATOMIC_COORDINATES_occupancies &at[79]
#define ATOMIC_COORDINATES_alternate_conf_ids &at[84]
#define ATOMIC_COORDINATES_conf_ensembles &at[88]
#define ATOMIC_COORDINATES_conf_ensembles_E &at[89]

#define SURFACE_COORDINATES &at[95]
#define SURFACE_COORDINATES_contents &at[96]
#define SURFACE_COORDINATES_surface &at[98]
#define SURFACE_COORDINATES_surface_sphere &at[99]
#define SURFACE_COORDINATES_surface_cone &at[101]
#define SURFACE_COORDINATES_surface_cylinder &at[103]
#define SURFACE_COORDINATES_surface_brick &at[105]
#define SURFACE_COORDINATES_surface_tmesh &at[107]
#define SURFACE_COORDINATES_surface_triangles &at[120]

#define DENSITY_COORDINATES &at[138]
#define DENSITY_COORDINATES_contents &at[139]
#define DENSITY_COORDINATES_grid_corners &at[140]
#define DENSITY_COORDINATES_grid_steps_x &at[141]
#define DENSITY_COORDINATES_grid_steps_y &at[142]
#define DENSITY_COORDINATES_grid_steps_z &at[143]
#define DENSITY_COORDINATES_fastest_varying &at[144]
#define DENSITY_COORDINATES_slowest_varying &at[145]
#define DENSITY_COORDINATES_scale_factor &at[146]
#define DENSITY_COORDINATES_density &at[147]
#define DENSITY_COORDINATES_density_E &at[148]

#define MODEL_SPACE_POINTS &at[49]
#define MODEL_SPACE_POINTS_scale_factor &at[50]
#define MODEL_SPACE_POINTS_x &at[51]
#define MODEL_SPACE_POINTS_x_E &at[52]
#define MODEL_SPACE_POINTS_y &at[53]
#define MODEL_SPACE_POINTS_y_E &at[54]
#define MODEL_SPACE_POINTS_z &at[55]
#define MODEL_SPACE_POINTS_z_E &at[56]

#define ATOMIC_TEMPERATURE_FACTORS &at[58]
#define ATOMIC_TEMPERATURE_FACTORS_isotropic &at[59]
#define ATOMIC_TEMPERATURE_FACTORS_anisotropic &at[64]

#define ATOMIC_OCCUPANCIES &at[80]
#define ATOMIC_OCCUPANCIES_scale_factor &at[81]
#define ATOMIC_OCCUPANCIES_o &at[82]
#define ATOMIC_OCCUPANCIES_o_E &at[83]

#define ALTERNATE_CONFORMATION_IDS &at[85]
#define ALTERNATE_CONFORMATION_IDS_E &at[86]

#define CONFORMATION_ENSEMBLE &at[90]
#define CONFORMATION_ENSEMBLE_name &at[91]
#define CONFORMATION_ENSEMBLE_alt_conf_ids &at[92]
#define CONFORMATION_ENSEMBLE_alt_conf_ids_E &at[93]

#define ISOTROPIC_TEMPERATURE_FACTORS &at[60]
#define ISOTROPIC_TEMPERATURE_FACTORS_scale_factor &at[61]
#define ISOTROPIC_TEMPERATURE_FACTORS_b &at[62]
#define ISOTROPIC_TEMPERATURE_FACTORS_b_E &at[63]

#define ANISOTROPIC_TEMPERATURE_FACTORS &at[65]
#define ANISOTROPIC_TEMPERATURE_FACTORS_scale_factor &at[66]
#define ANISOTROPIC_TEMPERATURE_FACTORS_b_11 &at[67]
#define ANISOTROPIC_TEMPERATURE_FACTORS_b_11_E &at[68]
#define ANISOTROPIC_TEMPERATURE_FACTORS_b_12 &at[69]
#define ANISOTROPIC_TEMPERATURE_FACTORS_b_12_E &at[70]
#define ANISOTROPIC_TEMPERATURE_FACTORS_b_13 &at[71]
#define ANISOTROPIC_TEMPERATURE_FACTORS_b_13_E &at[72]
#define ANISOTROPIC_TEMPERATURE_FACTORS_b_22 &at[73]
#define ANISOTROPIC_TEMPERATURE_FACTORS_b_22_E &at[74]
#define ANISOTROPIC_TEMPERATURE_FACTORS_b_23 &at[75]
#define ANISOTROPIC_TEMPERATURE_FACTORS_b_23_E &at[76]
#define ANISOTROPIC_TEMPERATURE_FACTORS_b_33 &at[77]
#define ANISOTROPIC_TEMPERATURE_FACTORS_b_33_E &at[78]

#define ALTERNATE_CONFORMATION_ID &at[87]

#define T_MESH &at[108]
#define T_MESH_number_of_points &at[109]
#define T_MESH_scale_factor &at[110]
#define T_MESH_swap &at[111]
#define T_MESH_swap_E &at[112]
#define T_MESH_x &at[114]
#define T_MESH_x_E &at[115]
#define T_MESH_y &at[116]
#define T_MESH_y_E &at[117]
#define T_MESH_z &at[118]
#define T_MESH_z_E &at[119]

#define TRIANGLES &at[121]
#define TRIANGLES_number_of_points &at[122]
#define TRIANGLES_scale_factor &at[123]
#define TRIANGLES_x &at[124]
#define TRIANGLES_x_E &at[125]
#define TRIANGLES_y &at[126]
#define TRIANGLES_y_E &at[127]
#define TRIANGLES_z &at[128]
#define TRIANGLES_z_E &at[129]
#define TRIANGLES_number_of_triangles &at[130]
#define TRIANGLES_v1 &at[131]
#define TRIANGLES_v1_E &at[132]
#define TRIANGLES_v2 &at[133]
#define TRIANGLES_v2_E &at[134]
#define TRIANGLES_v3 &at[135]
#define TRIANGLES_v3_E &at[136]
