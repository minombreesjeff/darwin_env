/***********************************************************************
*
**
*        Automatic header module from ASNTOOL
*
************************************************************************/

#ifndef _ASNTOOL_
#include <asn.h>
#endif

static char * asnfilename = "cn3d.h11";
static AsnValxNode avnx[35] = {
    {20,"off" ,1,0.0,&avnx[1] } ,
    {20,"trace" ,2,0.0,&avnx[2] } ,
    {20,"partial" ,3,0.0,&avnx[3] } ,
    {20,"complete" ,4,0.0,NULL } ,
    {20,"wire" ,1,0.0,&avnx[5] } ,
    {20,"tubes" ,2,0.0,&avnx[6] } ,
    {20,"ball-and-stick" ,3,0.0,&avnx[7] } ,
    {20,"space-fill" ,4,0.0,&avnx[8] } ,
    {20,"wire-worm" ,5,0.0,&avnx[9] } ,
    {20,"tube-worm" ,6,0.0,&avnx[10] } ,
    {20,"with-arrows" ,7,0.0,&avnx[11] } ,
    {20,"without-arrows" ,8,0.0,NULL } ,
    {20,"element" ,1,0.0,&avnx[13] } ,
    {20,"object" ,2,0.0,&avnx[14] } ,
    {20,"molecule" ,3,0.0,&avnx[15] } ,
    {20,"domain" ,4,0.0,&avnx[16] } ,
    {20,"secondary-structure" ,5,0.0,&avnx[17] } ,
    {20,"user-select" ,6,0.0,&avnx[18] } ,
    {20,"aligned" ,7,0.0,&avnx[19] } ,
    {20,"identity" ,8,0.0,&avnx[20] } ,
    {20,"variety" ,9,0.0,&avnx[21] } ,
    {20,"weighted-variety" ,10,0.0,&avnx[22] } ,
    {20,"information-content" ,11,0.0,&avnx[23] } ,
    {20,"fit" ,12,0.0,&avnx[24] } ,
    {20,"temperature" ,13,0.0,&avnx[25] } ,
    {20,"hydrophobicity" ,14,0.0,&avnx[26] } ,
    {20,"charge" ,15,0.0,&avnx[27] } ,
    {20,"rainbow" ,16,0.0,NULL } ,
    {3,NULL,255,0.0,NULL } ,
    {3,NULL,255,0.0,NULL } ,
    {20,"one-letter" ,1,0.0,&avnx[31] } ,
    {20,"three-letter" ,2,0.0,NULL } ,
    {20,"none" ,0,0.0,&avnx[33] } ,
    {20,"sequential" ,1,0.0,&avnx[34] } ,
    {20,"pdb" ,2,0.0,NULL } };

static AsnType atx[125] = {
  {401, "Cn3d-style-dictionary" ,1,0,0,0,0,1,0,0,NULL,&atx[22],&atx[1],0,&atx[66]} ,
  {0, "global-style" ,128,0,0,0,0,0,0,0,NULL,&atx[2],NULL,0,&atx[59]} ,
  {413, "Cn3d-style-settings" ,1,0,0,0,0,0,0,0,NULL,&atx[22],&atx[3],0,&atx[122]} ,
  {0, "name" ,128,0,0,1,0,0,0,0,NULL,&atx[4],NULL,0,&atx[5]} ,
  {323, "VisibleString" ,0,26,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "protein-backbone" ,128,1,0,0,0,0,0,0,NULL,&atx[6],NULL,0,&atx[23]} ,
  {410, "Cn3d-backbone-style" ,1,0,0,0,0,0,0,0,NULL,&atx[22],&atx[7],0,&atx[25]} ,
  {0, "type" ,128,0,0,0,0,0,0,0,NULL,&atx[8],NULL,0,&atx[10]} ,
  {406, "Cn3d-backbone-type" ,1,0,0,0,0,0,0,0,NULL,&atx[9],&avnx[0],0,&atx[11]} ,
  {310, "ENUMERATED" ,0,10,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "style" ,128,1,0,0,0,0,0,0,NULL,&atx[11],NULL,0,&atx[12]} ,
  {407, "Cn3d-drawing-style" ,1,0,0,0,0,0,0,0,NULL,&atx[9],&avnx[4],0,&atx[13]} ,
  {0, "color-scheme" ,128,2,0,0,0,0,0,0,NULL,&atx[13],NULL,0,&atx[14]} ,
  {408, "Cn3d-color-scheme" ,1,0,0,0,0,0,0,0,NULL,&atx[9],&avnx[12],0,&atx[15]} ,
  {0, "user-color" ,128,3,0,0,0,0,0,0,NULL,&atx[15],NULL,0,NULL} ,
  {409, "Cn3d-color" ,1,0,0,0,0,0,0,0,NULL,&atx[22],&atx[16],0,&atx[6]} ,
  {0, "scale-factor" ,128,0,0,0,1,0,0,0,&avnx[28],&atx[17],NULL,0,&atx[18]} ,
  {302, "INTEGER" ,0,2,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "red" ,128,1,0,0,0,0,0,0,NULL,&atx[17],NULL,0,&atx[19]} ,
  {0, "green" ,128,2,0,0,0,0,0,0,NULL,&atx[17],NULL,0,&atx[20]} ,
  {0, "blue" ,128,3,0,0,0,0,0,0,NULL,&atx[17],NULL,0,&atx[21]} ,
  {0, "alpha" ,128,4,0,0,1,0,0,0,&avnx[29],&atx[17],NULL,0,NULL} ,
  {311, "SEQUENCE" ,0,16,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "nucleotide-backbone" ,128,2,0,0,0,0,0,0,NULL,&atx[6],NULL,0,&atx[24]} ,
  {0, "protein-sidechains" ,128,3,0,0,0,0,0,0,NULL,&atx[25],NULL,0,&atx[31]} ,
  {411, "Cn3d-general-style" ,1,0,0,0,0,0,0,0,NULL,&atx[22],&atx[26],0,&atx[51]} ,
  {0, "is-on" ,128,0,0,0,0,0,0,0,NULL,&atx[27],NULL,0,&atx[28]} ,
  {301, "BOOLEAN" ,0,1,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "style" ,128,1,0,0,0,0,0,0,NULL,&atx[11],NULL,0,&atx[29]} ,
  {0, "color-scheme" ,128,2,0,0,0,0,0,0,NULL,&atx[13],NULL,0,&atx[30]} ,
  {0, "user-color" ,128,3,0,0,0,0,0,0,NULL,&atx[15],NULL,0,NULL} ,
  {0, "nucleotide-sidechains" ,128,4,0,0,0,0,0,0,NULL,&atx[25],NULL,0,&atx[32]} ,
  {0, "heterogens" ,128,5,0,0,0,0,0,0,NULL,&atx[25],NULL,0,&atx[33]} ,
  {0, "solvents" ,128,6,0,0,0,0,0,0,NULL,&atx[25],NULL,0,&atx[34]} ,
  {0, "connections" ,128,7,0,0,0,0,0,0,NULL,&atx[25],NULL,0,&atx[35]} ,
  {0, "helix-objects" ,128,8,0,0,0,0,0,0,NULL,&atx[25],NULL,0,&atx[36]} ,
  {0, "strand-objects" ,128,9,0,0,0,0,0,0,NULL,&atx[25],NULL,0,&atx[37]} ,
  {0, "virtual-disulfides-on" ,128,10,0,0,0,0,0,0,NULL,&atx[27],NULL,0,&atx[38]} ,
  {0, "virtual-disulfide-color" ,128,11,0,0,0,0,0,0,NULL,&atx[15],NULL,0,&atx[39]} ,
  {0, "hydrogens-on" ,128,12,0,0,0,0,0,0,NULL,&atx[27],NULL,0,&atx[40]} ,
  {0, "background-color" ,128,13,0,0,0,0,0,0,NULL,&atx[15],NULL,0,&atx[41]} ,
  {0, "scale-factor" ,128,14,0,0,0,0,0,0,NULL,&atx[17],NULL,0,&atx[42]} ,
  {0, "space-fill-proportion" ,128,15,0,0,0,0,0,0,NULL,&atx[17],NULL,0,&atx[43]} ,
  {0, "ball-radius" ,128,16,0,0,0,0,0,0,NULL,&atx[17],NULL,0,&atx[44]} ,
  {0, "stick-radius" ,128,17,0,0,0,0,0,0,NULL,&atx[17],NULL,0,&atx[45]} ,
  {0, "tube-radius" ,128,18,0,0,0,0,0,0,NULL,&atx[17],NULL,0,&atx[46]} ,
  {0, "tube-worm-radius" ,128,19,0,0,0,0,0,0,NULL,&atx[17],NULL,0,&atx[47]} ,
  {0, "helix-radius" ,128,20,0,0,0,0,0,0,NULL,&atx[17],NULL,0,&atx[48]} ,
  {0, "strand-width" ,128,21,0,0,0,0,0,0,NULL,&atx[17],NULL,0,&atx[49]} ,
  {0, "strand-thickness" ,128,22,0,0,0,0,0,0,NULL,&atx[17],NULL,0,&atx[50]} ,
  {0, "protein-labels" ,128,23,0,1,0,0,0,0,NULL,&atx[51],NULL,0,&atx[57]} ,
  {412, "Cn3d-backbone-label-style" ,1,0,0,0,0,0,0,0,NULL,&atx[22],&atx[52],0,&atx[2]} ,
  {0, "spacing" ,128,0,0,0,0,0,0,0,NULL,&atx[17],NULL,0,&atx[53]} ,
  {0, "type" ,128,1,0,0,0,0,0,0,NULL,&atx[9],&avnx[30],0,&atx[54]} ,
  {0, "number" ,128,2,0,0,0,0,0,0,NULL,&atx[9],&avnx[32],0,&atx[55]} ,
  {0, "termini" ,128,3,0,0,0,0,0,0,NULL,&atx[27],NULL,0,&atx[56]} ,
  {0, "white" ,128,4,0,0,0,0,0,0,NULL,&atx[27],NULL,0,NULL} ,
  {0, "nucleotide-labels" ,128,24,0,1,0,0,0,0,NULL,&atx[51],NULL,0,&atx[58]} ,
  {0, "ion-labels" ,128,25,0,1,0,0,0,0,NULL,&atx[27],NULL,0,NULL} ,
  {0, "style-table" ,128,1,0,1,0,0,0,0,NULL,&atx[65],&atx[60],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[61],NULL,0,NULL} ,
  {416, "Cn3d-style-table-item" ,1,0,0,0,0,0,0,0,NULL,&atx[22],&atx[62],0,&atx[85]} ,
  {0, "id" ,128,0,0,0,0,0,0,0,NULL,&atx[63],NULL,0,&atx[64]} ,
  {415, "Cn3d-style-table-id" ,1,0,0,0,0,0,0,0,NULL,&atx[17],NULL,0,&atx[61]} ,
  {0, "style" ,128,1,0,0,0,0,0,0,NULL,&atx[2],NULL,0,NULL} ,
  {312, "SEQUENCE OF" ,0,16,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {402, "Cn3d-user-annotations" ,1,0,0,0,0,1,0,0,NULL,&atx[22],&atx[67],0,&atx[77]} ,
  {0, "annotations" ,128,0,0,1,0,0,0,0,NULL,&atx[65],&atx[68],0,&atx[90]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[69],NULL,0,NULL} ,
  {420, "Cn3d-user-annotation" ,1,0,0,0,0,0,0,0,NULL,&atx[22],&atx[70],0,&atx[100]} ,
  {0, "name" ,128,0,0,0,0,0,0,0,NULL,&atx[4],NULL,0,&atx[71]} ,
  {0, "description" ,128,1,0,1,0,0,0,0,NULL,&atx[4],NULL,0,&atx[72]} ,
  {0, "style-id" ,128,2,0,0,0,0,0,0,NULL,&atx[63],NULL,0,&atx[73]} ,
  {0, "residues" ,128,3,0,0,0,0,0,0,NULL,&atx[65],&atx[74],0,&atx[89]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[75],NULL,0,NULL} ,
  {419, "Cn3d-object-location" ,1,0,0,0,0,0,0,0,NULL,&atx[22],&atx[76],0,&atx[69]} ,
  {0, "structure-id" ,128,0,0,0,0,0,0,0,NULL,&atx[77],NULL,0,&atx[78]} ,
  {403, "Biostruc-id" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[82]} ,
  {0, "residues" ,128,1,0,0,0,0,0,0,NULL,&atx[65],&atx[79],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[80],NULL,0,NULL} ,
  {418, "Cn3d-molecule-location" ,1,0,0,0,0,0,0,0,NULL,&atx[22],&atx[81],0,&atx[75]} ,
  {0, "molecule-id" ,128,0,0,0,0,0,0,0,NULL,&atx[82],NULL,0,&atx[83]} ,
  {404, "Molecule-id" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[87]} ,
  {0, "residues" ,128,1,0,1,0,0,0,0,NULL,&atx[65],&atx[84],0,NULL} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[85],NULL,0,NULL} ,
  {417, "Cn3d-residue-range" ,1,0,0,0,0,0,0,0,NULL,&atx[22],&atx[86],0,&atx[80]} ,
  {0, "from" ,128,0,0,0,0,0,0,0,NULL,&atx[87],NULL,0,&atx[88]} ,
  {405, "Residue-id" ,1,0,0,0,0,0,1,0,NULL,NULL,NULL,0,&atx[8]} ,
  {0, "to" ,128,1,0,0,0,0,0,0,NULL,&atx[87],NULL,0,NULL} ,
  {0, "is-on" ,128,4,0,0,0,0,0,0,NULL,&atx[27],NULL,0,NULL} ,
  {0, "view" ,128,1,0,1,0,0,0,0,NULL,&atx[91],NULL,0,NULL} ,
  {423, "Cn3d-view-settings" ,1,0,0,0,0,0,0,0,NULL,&atx[22],&atx[92],0,NULL} ,
  {0, "camera-distance" ,128,0,0,0,0,0,0,0,NULL,&atx[93],NULL,0,&atx[94]} ,
  {309, "REAL" ,0,9,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} ,
  {0, "camera-angle-rad" ,128,1,0,0,0,0,0,0,NULL,&atx[93],NULL,0,&atx[95]} ,
  {0, "camera-look-at-X" ,128,2,0,0,0,0,0,0,NULL,&atx[93],NULL,0,&atx[96]} ,
  {0, "camera-look-at-Y" ,128,3,0,0,0,0,0,0,NULL,&atx[93],NULL,0,&atx[97]} ,
  {0, "camera-clip-near" ,128,4,0,0,0,0,0,0,NULL,&atx[93],NULL,0,&atx[98]} ,
  {0, "camera-clip-far" ,128,5,0,0,0,0,0,0,NULL,&atx[93],NULL,0,&atx[99]} ,
  {0, "matrix" ,128,6,0,0,0,0,0,0,NULL,&atx[100],NULL,0,&atx[117]} ,
  {421, "Cn3d-GL-matrix" ,1,0,0,0,0,0,0,0,NULL,&atx[22],&atx[101],0,&atx[118]} ,
  {0, "m0" ,128,0,0,0,0,0,0,0,NULL,&atx[93],NULL,0,&atx[102]} ,
  {0, "m1" ,128,1,0,0,0,0,0,0,NULL,&atx[93],NULL,0,&atx[103]} ,
  {0, "m2" ,128,2,0,0,0,0,0,0,NULL,&atx[93],NULL,0,&atx[104]} ,
  {0, "m3" ,128,3,0,0,0,0,0,0,NULL,&atx[93],NULL,0,&atx[105]} ,
  {0, "m4" ,128,4,0,0,0,0,0,0,NULL,&atx[93],NULL,0,&atx[106]} ,
  {0, "m5" ,128,5,0,0,0,0,0,0,NULL,&atx[93],NULL,0,&atx[107]} ,
  {0, "m6" ,128,6,0,0,0,0,0,0,NULL,&atx[93],NULL,0,&atx[108]} ,
  {0, "m7" ,128,7,0,0,0,0,0,0,NULL,&atx[93],NULL,0,&atx[109]} ,
  {0, "m8" ,128,8,0,0,0,0,0,0,NULL,&atx[93],NULL,0,&atx[110]} ,
  {0, "m9" ,128,9,0,0,0,0,0,0,NULL,&atx[93],NULL,0,&atx[111]} ,
  {0, "m10" ,128,10,0,0,0,0,0,0,NULL,&atx[93],NULL,0,&atx[112]} ,
  {0, "m11" ,128,11,0,0,0,0,0,0,NULL,&atx[93],NULL,0,&atx[113]} ,
  {0, "m12" ,128,12,0,0,0,0,0,0,NULL,&atx[93],NULL,0,&atx[114]} ,
  {0, "m13" ,128,13,0,0,0,0,0,0,NULL,&atx[93],NULL,0,&atx[115]} ,
  {0, "m14" ,128,14,0,0,0,0,0,0,NULL,&atx[93],NULL,0,&atx[116]} ,
  {0, "m15" ,128,15,0,0,0,0,0,0,NULL,&atx[93],NULL,0,NULL} ,
  {0, "rotation-center" ,128,7,0,0,0,0,0,0,NULL,&atx[118],NULL,0,NULL} ,
  {422, "Cn3d-vector" ,1,0,0,0,0,0,0,0,NULL,&atx[22],&atx[119],0,&atx[91]} ,
  {0, "x" ,128,0,0,0,0,0,0,0,NULL,&atx[93],NULL,0,&atx[120]} ,
  {0, "y" ,128,1,0,0,0,0,0,0,NULL,&atx[93],NULL,0,&atx[121]} ,
  {0, "z" ,128,2,0,0,0,0,0,0,NULL,&atx[93],NULL,0,NULL} ,
  {414, "Cn3d-style-settings-set" ,1,0,0,0,0,0,0,0,NULL,&atx[124],&atx[123],0,&atx[63]} ,
  {0, NULL,1,-1,0,0,0,0,0,0,NULL,&atx[2],NULL,0,NULL} ,
  {314, "SET OF" ,0,17,0,0,0,0,0,0,NULL,NULL,NULL,0,NULL} };

static AsnModule ampx[1] = {
  { "NCBI-Cn3d" , "cn3d.h11",&atx[0],NULL,NULL,0,0} };

static AsnValxNodePtr avn = avnx;
static AsnTypePtr at = atx;
static AsnModulePtr amp = ampx;



/**************************************************
*
*    Defines for Module NCBI-Cn3d
*
**************************************************/

#define CN3D_STYLE_DICTIONARY &at[0]
#define CN3D_STYLE_DICTIONARY_global_style &at[1]
#define CN3D_STYLE_DICTIONARY_style_table &at[59]
#define CN3D_STYLE_DICTIONARY_style_table_E &at[60]

#define CN3D_USER_ANNOTATIONS &at[66]
#define CN3D_USER_ANNOTATIONS_annotations &at[67]
#define CN3D_USER_ANNOTATIONS_annotations_E &at[68]
#define CN3D_USER_ANNOTATIONS_view &at[90]

#define CN3D_BACKBONE_TYPE &at[8]

#define CN3D_DRAWING_STYLE &at[11]

#define CN3D_COLOR_SCHEME &at[13]

#define CN3D_COLOR &at[15]
#define CN3D_COLOR_scale_factor &at[16]
#define CN3D_COLOR_red &at[18]
#define CN3D_COLOR_green &at[19]
#define CN3D_COLOR_blue &at[20]
#define CN3D_COLOR_alpha &at[21]

#define CN3D_BACKBONE_STYLE &at[6]
#define CN3D_BACKBONE_STYLE_type &at[7]
#define CN3D_BACKBONE_STYLE_style &at[10]
#define CN3D_BACKBONE_STYLE_color_scheme &at[12]
#define CN3D_BACKBONE_STYLE_user_color &at[14]

#define CN3D_GENERAL_STYLE &at[25]
#define CN3D_GENERAL_STYLE_is_on &at[26]
#define CN3D_GENERAL_STYLE_style &at[28]
#define CN3D_GENERAL_STYLE_color_scheme &at[29]
#define CN3D_GENERAL_STYLE_user_color &at[30]

#define CN3D_BACKBONE_LABEL_STYLE &at[51]
#define CN3D_BACKBONE_LABEL_STYLE_spacing &at[52]
#define CN3D_BACKBONE_LABEL_STYLE_type &at[53]
#define CN3D_BACKBONE_LABEL_STYLE_number &at[54]
#define CN3D_BACKBONE_LABEL_STYLE_termini &at[55]
#define CN3D_BACKBONE_LABEL_STYLE_white &at[56]

#define CN3D_STYLE_SETTINGS &at[2]
#define CN3D_STYLE_SETTINGS_name &at[3]
#define CN3D_STYLE_SETTINGS_protein_backbone &at[5]
#define CN3D_STYLE_SETTINGS_nucleotide_backbone &at[23]
#define CN3D_STYLE_SETTINGS_protein_sidechains &at[24]
#define CN3D_STYLE_SETTINGS_nucleotide_sidechains &at[31]
#define CN3D_STYLE_SETTINGS_heterogens &at[32]
#define CN3D_STYLE_SETTINGS_solvents &at[33]
#define CN3D_STYLE_SETTINGS_connections &at[34]
#define CN3D_STYLE_SETTINGS_helix_objects &at[35]
#define CN3D_STYLE_SETTINGS_strand_objects &at[36]
#define CN3D_STYLE_SETTINGS_virtual_disulfides_on &at[37]
#define CN3D_STYLE_SETTINGS_virtual_disulfide_color &at[38]
#define CN3D_STYLE_SETTINGS_hydrogens_on &at[39]
#define CN3D_STYLE_SETTINGS_background_color &at[40]
#define CN3D_STYLE_SETTINGS_scale_factor &at[41]
#define CN3D_STYLE_SETTINGS_space_fill_proportion &at[42]
#define CN3D_STYLE_SETTINGS_ball_radius &at[43]
#define CN3D_STYLE_SETTINGS_stick_radius &at[44]
#define CN3D_STYLE_SETTINGS_tube_radius &at[45]
#define CN3D_STYLE_SETTINGS_tube_worm_radius &at[46]
#define CN3D_STYLE_SETTINGS_helix_radius &at[47]
#define CN3D_STYLE_SETTINGS_strand_width &at[48]
#define CN3D_STYLE_SETTINGS_strand_thickness &at[49]
#define CN3D_STYLE_SETTINGS_protein_labels &at[50]
#define CN3D_STYLE_SETTINGS_nucleotide_labels &at[57]
#define CN3D_STYLE_SETTINGS_ion_labels &at[58]

#define CN3D_STYLE_SETTINGS_SET &at[122]
#define CN3D_STYLE_SETTINGS_SET_E &at[123]

#define CN3D_STYLE_TABLE_ID &at[63]

#define CN3D_STYLE_TABLE_ITEM &at[61]
#define CN3D_STYLE_TABLE_ITEM_id &at[62]
#define CN3D_STYLE_TABLE_ITEM_style &at[64]

#define CN3D_RESIDUE_RANGE &at[85]
#define CN3D_RESIDUE_RANGE_from &at[86]
#define CN3D_RESIDUE_RANGE_to &at[88]

#define CN3D_MOLECULE_LOCATION &at[80]
#define CN3D_MOLECULE_LOCATION_molecule_id &at[81]
#define CN3D_MOLECULE_LOCATION_residues &at[83]
#define CN3D_MOLECULE_LOCATION_residues_E &at[84]

#define CN3D_OBJECT_LOCATION &at[75]
#define CN3D_OBJECT_LOCATION_structure_id &at[76]
#define CN3D_OBJECT_LOCATION_residues &at[78]
#define CN3D_OBJECT_LOCATION_residues_E &at[79]

#define CN3D_USER_ANNOTATION &at[69]
#define CN3D_USER_ANNOTATION_name &at[70]
#define CN3D_USER_ANNOTATION_description &at[71]
#define CN3D_USER_ANNOTATION_style_id &at[72]
#define CN3D_USER_ANNOTATION_residues &at[73]
#define CN3D_USER_ANNOTATION_residues_E &at[74]
#define CN3D_USER_ANNOTATION_is_on &at[89]

#define CN3D_GL_MATRIX &at[100]
#define CN3D_GL_MATRIX_m0 &at[101]
#define CN3D_GL_MATRIX_m1 &at[102]
#define CN3D_GL_MATRIX_m2 &at[103]
#define CN3D_GL_MATRIX_m3 &at[104]
#define CN3D_GL_MATRIX_m4 &at[105]
#define CN3D_GL_MATRIX_m5 &at[106]
#define CN3D_GL_MATRIX_m6 &at[107]
#define CN3D_GL_MATRIX_m7 &at[108]
#define CN3D_GL_MATRIX_m8 &at[109]
#define CN3D_GL_MATRIX_m9 &at[110]
#define CN3D_GL_MATRIX_m10 &at[111]
#define CN3D_GL_MATRIX_m11 &at[112]
#define CN3D_GL_MATRIX_m12 &at[113]
#define CN3D_GL_MATRIX_m13 &at[114]
#define CN3D_GL_MATRIX_m14 &at[115]
#define CN3D_GL_MATRIX_m15 &at[116]

#define CN3D_VECTOR &at[118]
#define CN3D_VECTOR_x &at[119]
#define CN3D_VECTOR_y &at[120]
#define CN3D_VECTOR_z &at[121]

#define CN3D_VIEW_SETTINGS &at[91]
#define CN3D_VIEW_SETTINGS_camera_distance &at[92]
#define CN3D_VIEW_SETTINGS_camera_angle_rad &at[94]
#define CN3D_VIEW_SETTINGS_camera_look_at_X &at[95]
#define CN3D_VIEW_SETTINGS_camera_look_at_Y &at[96]
#define CN3D_VIEW_SETTINGS_camera_clip_near &at[97]
#define CN3D_VIEW_SETTINGS_camera_clip_far &at[98]
#define CN3D_VIEW_SETTINGS_matrix &at[99]
#define CN3D_VIEW_SETTINGS_rotation_center &at[117]
