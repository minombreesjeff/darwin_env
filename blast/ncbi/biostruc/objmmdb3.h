#ifndef _objmmdb3_ 
#define _objmmdb3_ 

#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif


#ifdef __cplusplus
extern "C" { /* } */
#endif


/**************************************************
*
*    Generated objects for Module MMDB-Features
*    Generated using ASNCODE Revision: 6.8 at Mar 6, 2000  9:42 AM
*
**************************************************/

NLM_EXTERN Boolean LIBCALL
objmmdb3AsnLoad PROTO((void));


/**************************************************
*
*    BiostrucFeatureSet
*
**************************************************/
typedef struct struct_Biostruc_feature_set {
   struct struct_Biostruc_feature_set PNTR next;
   Uint4 OBbits__;
   Int4   id;
   ValNodePtr   descr;
   struct struct_Biostruc_feature PNTR   features;
} BiostrucFeatureSet, PNTR BiostrucFeatureSetPtr;


NLM_EXTERN BiostrucFeatureSetPtr LIBCALL BiostrucFeatureSetFree PROTO ((BiostrucFeatureSetPtr ));
NLM_EXTERN BiostrucFeatureSetPtr LIBCALL BiostrucFeatureSetNew PROTO (( void ));
NLM_EXTERN BiostrucFeatureSetPtr LIBCALL BiostrucFeatureSetAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL BiostrucFeatureSetAsnWrite PROTO (( BiostrucFeatureSetPtr , AsnIoPtr, AsnTypePtr));

typedef ValNodePtr ChemGraphPntrsPtr;
typedef ValNode ChemGraphPntrs;
#define ChemGraphPntrs_atoms 1
#define ChemGraphPntrs_residues 2
#define ChemGraphPntrs_molecules 3


NLM_EXTERN ChemGraphPntrsPtr LIBCALL ChemGraphPntrsFree PROTO ((ChemGraphPntrsPtr ));
NLM_EXTERN ChemGraphPntrsPtr LIBCALL ChemGraphPntrsAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ChemGraphPntrsAsnWrite PROTO (( ChemGraphPntrsPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    AtomPntrs
*
**************************************************/
typedef struct struct_Atom_pntrs {
   Uint4 OBbits__;
   Int4   number_of_ptrs;
   ValNodePtr   molecule_ids;
   ValNodePtr   residue_ids;
   ValNodePtr   atom_ids;
} AtomPntrs, PNTR AtomPntrsPtr;


NLM_EXTERN AtomPntrsPtr LIBCALL AtomPntrsFree PROTO ((AtomPntrsPtr ));
NLM_EXTERN AtomPntrsPtr LIBCALL AtomPntrsNew PROTO (( void ));
NLM_EXTERN AtomPntrsPtr LIBCALL AtomPntrsAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL AtomPntrsAsnWrite PROTO (( AtomPntrsPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    ChemGraphAlignment
*
**************************************************/
typedef struct struct_Chem_graph_alignment {
   Uint4 OBbits__;
   Int4   dimension;
   ValNodePtr   biostruc_ids;
   ValNodePtr   alignment;
   ValNodePtr   domain;
   struct struct_Transform PNTR   transform;
   struct struct_Align_stats PNTR   aligndata;
} ChemGraphAlignment, PNTR ChemGraphAlignmentPtr;


NLM_EXTERN ChemGraphAlignmentPtr LIBCALL ChemGraphAlignmentFree PROTO ((ChemGraphAlignmentPtr ));
NLM_EXTERN ChemGraphAlignmentPtr LIBCALL ChemGraphAlignmentNew PROTO (( void ));
NLM_EXTERN ChemGraphAlignmentPtr LIBCALL ChemGraphAlignmentAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ChemGraphAlignmentAsnWrite PROTO (( ChemGraphAlignmentPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    Sphere
*
**************************************************/
typedef struct struct_Sphere {
   Uint4 OBbits__;
   struct struct_Model_space_point PNTR   center;
   struct struct_RealValue PNTR   radius;
} Sphere, PNTR SpherePtr;


NLM_EXTERN SpherePtr LIBCALL SphereFree PROTO ((SpherePtr ));
NLM_EXTERN SpherePtr LIBCALL SphereNew PROTO (( void ));
NLM_EXTERN SpherePtr LIBCALL SphereAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL SphereAsnWrite PROTO (( SpherePtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    Cone
*
**************************************************/
typedef struct struct_Cone {
   Uint4 OBbits__;
   struct struct_Model_space_point PNTR   axis_top;
   struct struct_Model_space_point PNTR   axis_bottom;
   struct struct_RealValue PNTR   radius_bottom;
} Cone, PNTR ConePtr;


NLM_EXTERN ConePtr LIBCALL ConeFree PROTO ((ConePtr ));
NLM_EXTERN ConePtr LIBCALL ConeNew PROTO (( void ));
NLM_EXTERN ConePtr LIBCALL ConeAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ConeAsnWrite PROTO (( ConePtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    Cylinder
*
**************************************************/
typedef struct struct_Cylinder {
   Uint4 OBbits__;
   struct struct_Model_space_point PNTR   axis_top;
   struct struct_Model_space_point PNTR   axis_bottom;
   struct struct_RealValue PNTR   radius;
} Cylinder, PNTR CylinderPtr;


NLM_EXTERN CylinderPtr LIBCALL CylinderFree PROTO ((CylinderPtr ));
NLM_EXTERN CylinderPtr LIBCALL CylinderNew PROTO (( void ));
NLM_EXTERN CylinderPtr LIBCALL CylinderAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL CylinderAsnWrite PROTO (( CylinderPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    Brick
*
**************************************************/
typedef struct struct_Brick {
   Uint4 OBbits__;
   struct struct_Model_space_point PNTR   corner_000;
   struct struct_Model_space_point PNTR   corner_001;
   struct struct_Model_space_point PNTR   corner_010;
   struct struct_Model_space_point PNTR   corner_011;
   struct struct_Model_space_point PNTR   corner_100;
   struct struct_Model_space_point PNTR   corner_101;
   struct struct_Model_space_point PNTR   corner_110;
   struct struct_Model_space_point PNTR   corner_111;
} Brick, PNTR BrickPtr;


NLM_EXTERN BrickPtr LIBCALL BrickFree PROTO ((BrickPtr ));
NLM_EXTERN BrickPtr LIBCALL BrickNew PROTO (( void ));
NLM_EXTERN BrickPtr LIBCALL BrickAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL BrickAsnWrite PROTO (( BrickPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    Transform
*
**************************************************/
typedef struct struct_Transform {
   struct struct_Transform PNTR next;
   Uint4 OBbits__;
   Int4   id;
   ValNodePtr   moves;
} Transform, PNTR TransformPtr;


NLM_EXTERN TransformPtr LIBCALL TransformFree PROTO ((TransformPtr ));
NLM_EXTERN TransformPtr LIBCALL TransformNew PROTO (( void ));
NLM_EXTERN TransformPtr LIBCALL TransformAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL TransformAsnWrite PROTO (( TransformPtr , AsnIoPtr, AsnTypePtr));

typedef ValNodePtr BiostrucFeatureSetDescrPtr;
typedef ValNode BiostrucFeatureSetDescr;
#define BiostrucFeatureSetDescr_name 1
#define BiostrucFeatureSetDescr_pdb_comment 2
#define BiostrucFeatureSetDescr_other_comment 3
#define BiostrucFeatureSetDescr_attribution 4


NLM_EXTERN BiostrucFeatureSetDescrPtr LIBCALL BiostrucFeatureSetDescrFree PROTO ((BiostrucFeatureSetDescrPtr ));
NLM_EXTERN BiostrucFeatureSetDescrPtr LIBCALL BiostrucFeatureSetDescrAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL BiostrucFeatureSetDescrAsnWrite PROTO (( BiostrucFeatureSetDescrPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    BiostrucFeature
*
**************************************************/
typedef struct struct_Biostruc_feature {
   struct struct_Biostruc_feature PNTR next;
   Uint4 OBbits__;
#define OB__Biostruc_feature_id 0

   Int4   id;
   CharPtr   name;
#define OB__Biostruc_feature_type 1

   Int4   type;
   ValNodePtr   Property_property;
   ValNodePtr   Location_location;
} BiostrucFeature, PNTR BiostrucFeaturePtr;


NLM_EXTERN BiostrucFeaturePtr LIBCALL BiostrucFeatureFree PROTO ((BiostrucFeaturePtr ));
NLM_EXTERN BiostrucFeaturePtr LIBCALL BiostrucFeatureNew PROTO (( void ));
NLM_EXTERN BiostrucFeaturePtr LIBCALL BiostrucFeatureAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL BiostrucFeatureAsnWrite PROTO (( BiostrucFeaturePtr , AsnIoPtr, AsnTypePtr));


#ifdef NLM_GENERATED_CODE_PROTO

typedef ValNodePtr Location_locationPtr;
typedef ValNode Location_location;

#endif /* NLM_GENERATED_CODE_PROTO */

#define Location_location_subgraph 1
#define Location_location_region 2
#define Location_location_alignment 3
#define Location_location_similarity 4
#define Location_location_indirect 5

#ifdef NLM_GENERATED_CODE_PROTO

static Location_locationPtr LIBCALL Location_locationFree PROTO ((Location_locationPtr ));
static Location_locationPtr LIBCALL Location_locationAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
static Boolean LIBCALL Location_locationAsnWrite PROTO (( Location_locationPtr , AsnIoPtr, AsnTypePtr));

#endif /* NLM_GENERATED_CODE_PROTO */


#ifdef NLM_GENERATED_CODE_PROTO

typedef ValNodePtr Property_propertyPtr;
typedef ValNode Property_property;

#endif /* NLM_GENERATED_CODE_PROTO */

#define Property_property_color 1
#define Property_property_render 2
#define Property_property_transform 3
#define Property_property_camera 4
#define Property_property_script 5
#define Property_property_user 6

#ifdef NLM_GENERATED_CODE_PROTO

static Property_propertyPtr LIBCALL Property_propertyFree PROTO ((Property_propertyPtr ));
static Property_propertyPtr LIBCALL Property_propertyAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
static Boolean LIBCALL Property_propertyAsnWrite PROTO (( Property_propertyPtr , AsnIoPtr, AsnTypePtr));

#endif /* NLM_GENERATED_CODE_PROTO */



/**************************************************
*
*    ColorProp
*
**************************************************/
typedef struct struct_Color_prop {
   Uint4 OBbits__;
#define OB__Color_prop_r 0

   Int4   r;
#define OB__Color_prop_g 1

   Int4   g;
#define OB__Color_prop_b 2

   Int4   b;
   CharPtr   name;
} ColorProp, PNTR ColorPropPtr;


NLM_EXTERN ColorPropPtr LIBCALL ColorPropFree PROTO ((ColorPropPtr ));
NLM_EXTERN ColorPropPtr LIBCALL ColorPropNew PROTO (( void ));
NLM_EXTERN ColorPropPtr LIBCALL ColorPropAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ColorPropAsnWrite PROTO (( ColorPropPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    Camera
*
**************************************************/
typedef struct struct_Camera {
   Uint4 OBbits__;
   Int4   x;
   Int4   y;
   Int4   distance;
   Int4   angle;
   Int4   scale;
   struct struct_GL_matrix PNTR   modelview;
} Camera, PNTR CameraPtr;


NLM_EXTERN CameraPtr LIBCALL CameraFree PROTO ((CameraPtr ));
NLM_EXTERN CameraPtr LIBCALL CameraNew PROTO (( void ));
NLM_EXTERN CameraPtr LIBCALL CameraAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL CameraAsnWrite PROTO (( CameraPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    BiostrucScript
*
**************************************************/
typedef struct struct_BiostrucScriptStep BiostrucScript;
typedef struct struct_BiostrucScriptStep PNTR BiostrucScriptPtr;
#define BiostrucScriptNew() BiostrucScriptStepNew() 

#ifdef NLM_GENERATED_CODE_PROTO

NLM_EXTERN BiostrucScriptPtr LIBCALL BiostrucScriptFree PROTO ((BiostrucScriptPtr ));
NLM_EXTERN BiostrucScriptPtr LIBCALL BiostrucScriptNew PROTO (( void ));
NLM_EXTERN BiostrucScriptPtr LIBCALL BiostrucScriptAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL BiostrucScriptAsnWrite PROTO (( BiostrucScriptPtr , AsnIoPtr, AsnTypePtr));

#endif /* NLM_GENERATED_CODE_PROTO */



/**************************************************
*
*    RegionPntrs
*
**************************************************/
typedef struct struct_Region_pntrs {
   struct struct_Region_pntrs PNTR next;
   Uint4 OBbits__;
   Int4   model_id;
   ValNodePtr   Region_region;
} RegionPntrs, PNTR RegionPntrsPtr;


NLM_EXTERN RegionPntrsPtr LIBCALL RegionPntrsFree PROTO ((RegionPntrsPtr ));
NLM_EXTERN RegionPntrsPtr LIBCALL RegionPntrsNew PROTO (( void ));
NLM_EXTERN RegionPntrsPtr LIBCALL RegionPntrsAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL RegionPntrsAsnWrite PROTO (( RegionPntrsPtr , AsnIoPtr, AsnTypePtr));


#ifdef NLM_GENERATED_CODE_PROTO

typedef ValNodePtr Region_regionPtr;
typedef ValNode Region_region;

#endif /* NLM_GENERATED_CODE_PROTO */

#define Region_region_site 1
#define Region_region_boundary 2

#ifdef NLM_GENERATED_CODE_PROTO

static Region_regionPtr LIBCALL Region_regionFree PROTO ((Region_regionPtr ));
static Region_regionPtr LIBCALL Region_regionAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
static Boolean LIBCALL Region_regionAsnWrite PROTO (( Region_regionPtr , AsnIoPtr, AsnTypePtr));

#endif /* NLM_GENERATED_CODE_PROTO */



/**************************************************
*
*    RegionSimilarity
*
**************************************************/
typedef struct struct_Region_similarity {
   Uint4 OBbits__;
   Int4   dimension;
   ValNodePtr   biostruc_ids;
   struct struct_Region_pntrs PNTR   similarity;
   struct struct_Transform PNTR   transform;
} RegionSimilarity, PNTR RegionSimilarityPtr;


NLM_EXTERN RegionSimilarityPtr LIBCALL RegionSimilarityFree PROTO ((RegionSimilarityPtr ));
NLM_EXTERN RegionSimilarityPtr LIBCALL RegionSimilarityNew PROTO (( void ));
NLM_EXTERN RegionSimilarityPtr LIBCALL RegionSimilarityAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL RegionSimilarityAsnWrite PROTO (( RegionSimilarityPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    OtherFeature
*
**************************************************/
typedef struct struct_Other_feature {
   struct struct_Other_feature PNTR next;
   Uint4 OBbits__;
   ValNodePtr   biostruc_id;
   Int4   set;
   Int4   feature;
} OtherFeature, PNTR OtherFeaturePtr;


NLM_EXTERN OtherFeaturePtr LIBCALL OtherFeatureFree PROTO ((OtherFeaturePtr ));
NLM_EXTERN OtherFeaturePtr LIBCALL OtherFeatureNew PROTO (( void ));
NLM_EXTERN OtherFeaturePtr LIBCALL OtherFeatureAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL OtherFeatureAsnWrite PROTO (( OtherFeaturePtr , AsnIoPtr, AsnTypePtr));

typedef ValNodePtr ResiduePntrsPtr;
typedef ValNode ResiduePntrs;
#define ResiduePntrs_explicit 1
#define ResiduePntrs_interval 2


NLM_EXTERN ResiduePntrsPtr LIBCALL ResiduePntrsFree PROTO ((ResiduePntrsPtr ));
NLM_EXTERN ResiduePntrsPtr LIBCALL ResiduePntrsAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ResiduePntrsAsnWrite PROTO (( ResiduePntrsPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    MoleculePntrs
*
**************************************************/
typedef struct struct_Molecule_pntrs {
   Uint4 OBbits__;
   Int4   number_of_ptrs;
   ValNodePtr   molecule_ids;
} MoleculePntrs, PNTR MoleculePntrsPtr;


NLM_EXTERN MoleculePntrsPtr LIBCALL MoleculePntrsFree PROTO ((MoleculePntrsPtr ));
NLM_EXTERN MoleculePntrsPtr LIBCALL MoleculePntrsNew PROTO (( void ));
NLM_EXTERN MoleculePntrsPtr LIBCALL MoleculePntrsAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL MoleculePntrsAsnWrite PROTO (( MoleculePntrsPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    ResidueExplicitPntrs
*
**************************************************/
typedef struct struct_Residue_explicit_pntrs {
   Uint4 OBbits__;
   Int4   number_of_ptrs;
   ValNodePtr   molecule_ids;
   ValNodePtr   residue_ids;
} ResidueExplicitPntrs, PNTR ResidueExplicitPntrsPtr;


NLM_EXTERN ResidueExplicitPntrsPtr LIBCALL ResidueExplicitPntrsFree PROTO ((ResidueExplicitPntrsPtr ));
NLM_EXTERN ResidueExplicitPntrsPtr LIBCALL ResidueExplicitPntrsNew PROTO (( void ));
NLM_EXTERN ResidueExplicitPntrsPtr LIBCALL ResidueExplicitPntrsAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ResidueExplicitPntrsAsnWrite PROTO (( ResidueExplicitPntrsPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    ResidueIntervalPntr
*
**************************************************/
typedef struct struct_Residue_interval_pntr {
   struct struct_Residue_interval_pntr PNTR next;
   Uint4 OBbits__;
   Int4   molecule_id;
   Int4   from;
   Int4   to;
} ResidueIntervalPntr, PNTR ResidueIntervalPntrPtr;


NLM_EXTERN ResidueIntervalPntrPtr LIBCALL ResidueIntervalPntrFree PROTO ((ResidueIntervalPntrPtr ));
NLM_EXTERN ResidueIntervalPntrPtr LIBCALL ResidueIntervalPntrNew PROTO (( void ));
NLM_EXTERN ResidueIntervalPntrPtr LIBCALL ResidueIntervalPntrAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ResidueIntervalPntrAsnWrite PROTO (( ResidueIntervalPntrPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    RegionCoordinates
*
**************************************************/
typedef struct struct_Region_coordinates {
   struct struct_Region_coordinates PNTR next;
   Uint4 OBbits__;
   Int4   model_coord_set_id;
#define OB__Region_coordinates_number_of_coords 0

   Int4   number_of_coords;
   ValNodePtr   coordinate_indices;
} RegionCoordinates, PNTR RegionCoordinatesPtr;


NLM_EXTERN RegionCoordinatesPtr LIBCALL RegionCoordinatesFree PROTO ((RegionCoordinatesPtr ));
NLM_EXTERN RegionCoordinatesPtr LIBCALL RegionCoordinatesNew PROTO (( void ));
NLM_EXTERN RegionCoordinatesPtr LIBCALL RegionCoordinatesAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL RegionCoordinatesAsnWrite PROTO (( RegionCoordinatesPtr , AsnIoPtr, AsnTypePtr));

typedef ValNodePtr RegionBoundaryPtr;
typedef ValNode RegionBoundary;
#define RegionBoundary_sphere 1
#define RegionBoundary_cone 2
#define RegionBoundary_cylinder 3
#define RegionBoundary_brick 4


NLM_EXTERN RegionBoundaryPtr LIBCALL RegionBoundaryFree PROTO ((RegionBoundaryPtr ));
NLM_EXTERN RegionBoundaryPtr LIBCALL RegionBoundaryAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL RegionBoundaryAsnWrite PROTO (( RegionBoundaryPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    AlignStats
*
**************************************************/
typedef struct struct_Align_stats {
   struct struct_Align_stats PNTR next;
   Uint4 OBbits__;
   CharPtr   descr;
#define OB__Align_stats_scale_factor 0

   Int4   scale_factor;
#define OB__Align_stats_vast_score 1

   Int4   vast_score;
#define OB__Align_stats_vast_mlogp 2

   Int4   vast_mlogp;
#define OB__Align_stats_align_res 3

   Int4   align_res;
#define OB__Align_stats_rmsd 4

   Int4   rmsd;
#define OB__Align_stats_blast_score 5

   Int4   blast_score;
#define OB__Align_stats_blast_mlogp 6

   Int4   blast_mlogp;
#define OB__Align_stats_other_score 7

   Int4   other_score;
} AlignStats, PNTR AlignStatsPtr;


NLM_EXTERN AlignStatsPtr LIBCALL AlignStatsFree PROTO ((AlignStatsPtr ));
NLM_EXTERN AlignStatsPtr LIBCALL AlignStatsNew PROTO (( void ));
NLM_EXTERN AlignStatsPtr LIBCALL AlignStatsAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL AlignStatsAsnWrite PROTO (( AlignStatsPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    ModelSpacePoint
*
**************************************************/
typedef struct struct_Model_space_point {
   Uint4 OBbits__;
   Int4   scale_factor;
   Int4   x;
   Int4   y;
   Int4   z;
} ModelSpacePoint, PNTR ModelSpacePointPtr;


NLM_EXTERN ModelSpacePointPtr LIBCALL ModelSpacePointFree PROTO ((ModelSpacePointPtr ));
NLM_EXTERN ModelSpacePointPtr LIBCALL ModelSpacePointNew PROTO (( void ));
NLM_EXTERN ModelSpacePointPtr LIBCALL ModelSpacePointAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ModelSpacePointAsnWrite PROTO (( ModelSpacePointPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    RealValue
*
**************************************************/
typedef struct struct_RealValue {
   Uint4 OBbits__;
   Int4   scale_factor;
   Int4   scaled_integer_value;
} RealValue, PNTR RealValuePtr;


NLM_EXTERN RealValuePtr LIBCALL RealValueFree PROTO ((RealValuePtr ));
NLM_EXTERN RealValuePtr LIBCALL RealValueNew PROTO (( void ));
NLM_EXTERN RealValuePtr LIBCALL RealValueAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL RealValueAsnWrite PROTO (( RealValuePtr , AsnIoPtr, AsnTypePtr));

typedef ValNodePtr MovePtr;
typedef ValNode Move_t;
#define Move_rotate 1
#define Move_translate 2


NLM_EXTERN MovePtr LIBCALL MoveFree PROTO ((MovePtr ));
NLM_EXTERN MovePtr LIBCALL MoveAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL MoveAsnWrite PROTO (( MovePtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    RotMatrix
*
**************************************************/
typedef struct struct_Rot_matrix {
   Uint4 OBbits__;
   Int4   scale_factor;
   Int4   rot_11;
   Int4   rot_12;
   Int4   rot_13;
   Int4   rot_21;
   Int4   rot_22;
   Int4   rot_23;
   Int4   rot_31;
   Int4   rot_32;
   Int4   rot_33;
} RotMatrix, PNTR RotMatrixPtr;


NLM_EXTERN RotMatrixPtr LIBCALL RotMatrixFree PROTO ((RotMatrixPtr ));
NLM_EXTERN RotMatrixPtr LIBCALL RotMatrixNew PROTO (( void ));
NLM_EXTERN RotMatrixPtr LIBCALL RotMatrixAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL RotMatrixAsnWrite PROTO (( RotMatrixPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    TransMatrix
*
**************************************************/
typedef struct struct_Trans_matrix {
   Uint4 OBbits__;
   Int4   scale_factor;
   Int4   tran_1;
   Int4   tran_2;
   Int4   tran_3;
} TransMatrix, PNTR TransMatrixPtr;


NLM_EXTERN TransMatrixPtr LIBCALL TransMatrixFree PROTO ((TransMatrixPtr ));
NLM_EXTERN TransMatrixPtr LIBCALL TransMatrixNew PROTO (( void ));
NLM_EXTERN TransMatrixPtr LIBCALL TransMatrixAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL TransMatrixAsnWrite PROTO (( TransMatrixPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    GLMatrix
*
**************************************************/
typedef struct struct_GL_matrix {
   Uint4 OBbits__;
   Int4   scale;
   Int4   m11;
   Int4   m12;
   Int4   m13;
   Int4   m14;
   Int4   m21;
   Int4   m22;
   Int4   m23;
   Int4   m24;
   Int4   m31;
   Int4   m32;
   Int4   m33;
   Int4   m34;
   Int4   m41;
   Int4   m42;
   Int4   m43;
   Int4   m44;
} GLMatrix, PNTR GLMatrixPtr;


NLM_EXTERN GLMatrixPtr LIBCALL GLMatrixFree PROTO ((GLMatrixPtr ));
NLM_EXTERN GLMatrixPtr LIBCALL GLMatrixNew PROTO (( void ));
NLM_EXTERN GLMatrixPtr LIBCALL GLMatrixAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL GLMatrixAsnWrite PROTO (( GLMatrixPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    BiostrucScriptStep
*
**************************************************/
typedef struct struct_Biostruc_script_step {
   struct struct_Biostruc_script_step PNTR next;
   Uint4 OBbits__;
   Int4   step_id;
   CharPtr   step_name;
   struct struct_Other_feature PNTR   feature_do;
   struct struct_Transform PNTR   camera_move;
   Int4   pause;
   Uint1   waitevent;
   Int4   extra;
#define OB__Biostruc_script_step_jump 0

   Int4   jump;
} BiostrucScriptStep, PNTR BiostrucScriptStepPtr;


NLM_EXTERN BiostrucScriptStepPtr LIBCALL BiostrucScriptStepFree PROTO ((BiostrucScriptStepPtr ));
NLM_EXTERN BiostrucScriptStepPtr LIBCALL BiostrucScriptStepNew PROTO (( void ));
NLM_EXTERN BiostrucScriptStepPtr LIBCALL BiostrucScriptStepAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL BiostrucScriptStepAsnWrite PROTO (( BiostrucScriptStepPtr , AsnIoPtr, AsnTypePtr));

#ifdef __cplusplus
/* { */ }
#endif

#endif /* _objmmdb3_ */

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

