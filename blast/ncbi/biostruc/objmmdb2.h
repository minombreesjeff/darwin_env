#ifndef _objmmdb2_ 
#define _objmmdb2_ 


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
*    Generated objects for Module MMDB-Structural-model
*    Generated using ASNCODE Revision: 4.2 at Aug 14, 1996  5:25 PM
*
**************************************************/

NLM_EXTERN Boolean LIBCALL
objmmdb2AsnLoad PROTO((void));


/**************************************************
*
*    BiostrucModel
*
**************************************************/
typedef struct struct_Biostruc_model {
   struct struct_Biostruc_model PNTR next;
   Uint4 OBbits__;
   Int4   id;
   Int4   type;
   ValNodePtr   descr;
   struct struct_Model_space PNTR   model_space;
   struct struct_Model_coordinate_set PNTR   model_coordinates;
} BiostrucModel, PNTR BiostrucModelPtr;


BiostrucModelPtr LIBCALL BiostrucModelFree PROTO ((BiostrucModelPtr ));
BiostrucModelPtr LIBCALL BiostrucModelNew PROTO (( void ));
BiostrucModelPtr LIBCALL BiostrucModelAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
Boolean LIBCALL BiostrucModelAsnWrite PROTO (( BiostrucModelPtr , AsnIoPtr, AsnTypePtr));

typedef ValNodePtr ModelDescrPtr;
typedef ValNode ModelDescr;
#define ModelDescr_name 1
#define ModelDescr_pdb_reso 2
#define ModelDescr_pdb_method 3
#define ModelDescr_pdb_comment 4
#define ModelDescr_other_comment 5
#define ModelDescr_attribution 6


ModelDescrPtr LIBCALL ModelDescrFree PROTO ((ModelDescrPtr ));
ModelDescrPtr LIBCALL ModelDescrAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
Boolean LIBCALL ModelDescrAsnWrite PROTO (( ModelDescrPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    ModelSpace
*
**************************************************/
typedef struct struct_Model_space {
   Uint4 OBbits__;
   Uint2   coordinate_units;
   /* following #defines are for enumerated type, not used by object loaders */
#define Model_space_coordinate_units_angstroms 1
#define Model_space_coordinate_units_nanometers 2
#define Model_space_coordinate_units_other 3
#define Model_space_coordinate_units_unknown 255

#define OB__Model_space_thermal_factor_units 0

   Uint2   thermal_factor_units;
   /* following #defines are for enumerated type, not used by object loaders */
#define Model_space_thermal_factor_units_b 1
#define Model_space_thermal_factor_units_u 2
#define Model_space_thermal_factor_units_other 3
#define Model_space_thermal_factor_units_unknown 255

#define OB__Model_space_occupancy_factor_units 1

   Uint2   occupancy_factor_units;
   /* following #defines are for enumerated type, not used by object loaders */
#define Model_space_occupancy_factor_units_fractional 1
#define Model_space_occupancy_factor_units_electrons 2
#define Model_space_occupancy_factor_units_other 3
#define Model_space_occupancy_factor_units_unknown 255

#define OB__Model_space_density_units 2

   Uint2   density_units;
   /* following #defines are for enumerated type, not used by object loaders */
#define Model_space_density_units_electrons_per_unit_volume 1
#define Model_space_density_units_arbitrary_scale 2
#define Model_space_density_units_other 3
#define Model_space_density_units_unknown 255

   struct struct_Reference_frame PNTR   reference_frame;
} ModelSpace, PNTR ModelSpacePtr;


ModelSpacePtr LIBCALL ModelSpaceFree PROTO ((ModelSpacePtr ));
ModelSpacePtr LIBCALL ModelSpaceNew PROTO (( void ));
ModelSpacePtr LIBCALL ModelSpaceAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
Boolean LIBCALL ModelSpaceAsnWrite PROTO (( ModelSpacePtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    ModelCoordinateSet
*
**************************************************/
typedef struct struct_Model_coordinate_set {
   struct struct_Model_coordinate_set PNTR next;
   Uint4 OBbits__;
#define OB__Model_coordinate_set_id 0

   Int4   id;
   ValNodePtr   descr;
   ValNodePtr   Coordinates_coordinates;
} ModelCoordinateSet, PNTR ModelCoordinateSetPtr;


ModelCoordinateSetPtr LIBCALL ModelCoordinateSetFree PROTO ((ModelCoordinateSetPtr ));
ModelCoordinateSetPtr LIBCALL ModelCoordinateSetNew PROTO (( void ));
ModelCoordinateSetPtr LIBCALL ModelCoordinateSetAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
Boolean LIBCALL ModelCoordinateSetAsnWrite PROTO (( ModelCoordinateSetPtr , AsnIoPtr, AsnTypePtr));


#ifdef NLM_GENERATED_CODE_PROTO

typedef ValNodePtr Coordinates_coordinatesPtr;
typedef ValNode Coordinates_coordinates;

#endif /* NLM_GENERATED_CODE_PROTO */

#define Coordinates_coordinates_literal 1
#define Coordinates_coordinates_reference 2

#ifdef NLM_GENERATED_CODE_PROTO

static Coordinates_coordinatesPtr LIBCALL Coordinates_coordinatesFree PROTO ((Coordinates_coordinatesPtr ));
static Coordinates_coordinatesPtr LIBCALL Coordinates_coordinatesAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
static Boolean LIBCALL Coordinates_coordinatesAsnWrite PROTO (( Coordinates_coordinatesPtr , AsnIoPtr, AsnTypePtr));

#endif /* NLM_GENERATED_CODE_PROTO */



/**************************************************
*
*    ReferenceFrame
*
**************************************************/
typedef struct struct_Reference_frame {
   Uint4 OBbits__;
   ValNodePtr   biostruc_id;
   struct struct_Transform PNTR   rotation_translation;
} ReferenceFrame, PNTR ReferenceFramePtr;


ReferenceFramePtr LIBCALL ReferenceFrameFree PROTO ((ReferenceFramePtr ));
ReferenceFramePtr LIBCALL ReferenceFrameNew PROTO (( void ));
ReferenceFramePtr LIBCALL ReferenceFrameAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
Boolean LIBCALL ReferenceFrameAsnWrite PROTO (( ReferenceFramePtr , AsnIoPtr, AsnTypePtr));

typedef ValNodePtr CoordinatesPtr;
typedef ValNode Coordinates;
#define Coordinates_atomic 1
#define Coordinates_surface 2
#define Coordinates_density 3


CoordinatesPtr LIBCALL CoordinatesFree PROTO ((CoordinatesPtr ));
CoordinatesPtr LIBCALL CoordinatesAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
Boolean LIBCALL CoordinatesAsnWrite PROTO (( CoordinatesPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    AtomicCoordinates
*
**************************************************/
typedef struct struct_Atomic_coordinates {
   Uint4 OBbits__;
   Int4   number_of_points;
   struct struct_Atom_pntrs PNTR   atoms;
   struct struct_Model_space_points PNTR   sites;
   ValNodePtr   temperature_factors;
   struct struct_Atomic_occupancies PNTR   occupancies;
   struct struct_Alternate_conformation_id PNTR   alternate_conf_ids;
   struct struct_Conformation_ensemble PNTR   conf_ensembles;
} AtomicCoordinates, PNTR AtomicCoordinatesPtr;


AtomicCoordinatesPtr LIBCALL AtomicCoordinatesFree PROTO ((AtomicCoordinatesPtr ));
AtomicCoordinatesPtr LIBCALL AtomicCoordinatesNew PROTO (( void ));
AtomicCoordinatesPtr LIBCALL AtomicCoordinatesAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
Boolean LIBCALL AtomicCoordinatesAsnWrite PROTO (( AtomicCoordinatesPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    SurfaceCoordinates
*
**************************************************/
typedef struct struct_Surface_coordinates {
   Uint4 OBbits__;
   ValNodePtr   contents;
   ValNodePtr   Surface_surface;
} SurfaceCoordinates, PNTR SurfaceCoordinatesPtr;


SurfaceCoordinatesPtr LIBCALL SurfaceCoordinatesFree PROTO ((SurfaceCoordinatesPtr ));
SurfaceCoordinatesPtr LIBCALL SurfaceCoordinatesNew PROTO (( void ));
SurfaceCoordinatesPtr LIBCALL SurfaceCoordinatesAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
Boolean LIBCALL SurfaceCoordinatesAsnWrite PROTO (( SurfaceCoordinatesPtr , AsnIoPtr, AsnTypePtr));


#ifdef NLM_GENERATED_CODE_PROTO

typedef ValNodePtr Surface_surfacePtr;
typedef ValNode Surface_surface;

#endif /* NLM_GENERATED_CODE_PROTO */

#define Surface_surface_sphere 1
#define Surface_surface_cone 2
#define Surface_surface_cylinder 3
#define Surface_surface_brick 4
#define Surface_surface_tmesh 5
#define Surface_surface_triangles 6

#ifdef NLM_GENERATED_CODE_PROTO

static Surface_surfacePtr LIBCALL Surface_surfaceFree PROTO ((Surface_surfacePtr ));
static Surface_surfacePtr LIBCALL Surface_surfaceAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
static Boolean LIBCALL Surface_surfaceAsnWrite PROTO (( Surface_surfacePtr , AsnIoPtr, AsnTypePtr));

#endif /* NLM_GENERATED_CODE_PROTO */



/**************************************************
*
*    DensityCoordinates
*
**************************************************/
typedef struct struct_Density_coordinates {
   Uint4 OBbits__;
   ValNodePtr   contents;
   struct struct_Brick PNTR   grid_corners;
   Int4   grid_steps_x;
   Int4   grid_steps_y;
   Int4   grid_steps_z;
   Uint2   fastest_varying;
   /* following #defines are for enumerated type, not used by object loaders */
#define Density_coordinates_fastest_varying_x 1
#define Density_coordinates_fastest_varying_y 2
#define Density_coordinates_fastest_varying_z 3

   Uint2   slowest_varying;
   /* following #defines are for enumerated type, not used by object loaders */
#define Density_coordinates_slowest_varying_x 1
#define Density_coordinates_slowest_varying_y 2
#define Density_coordinates_slowest_varying_z 3

   Int4   scale_factor;
   ValNodePtr   density;
} DensityCoordinates, PNTR DensityCoordinatesPtr;


DensityCoordinatesPtr LIBCALL DensityCoordinatesFree PROTO ((DensityCoordinatesPtr ));
DensityCoordinatesPtr LIBCALL DensityCoordinatesNew PROTO (( void ));
DensityCoordinatesPtr LIBCALL DensityCoordinatesAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
Boolean LIBCALL DensityCoordinatesAsnWrite PROTO (( DensityCoordinatesPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    ModelSpacePoints
*
**************************************************/
typedef struct struct_Model_space_points {
   Uint4 OBbits__;
   Int4   scale_factor;
   ValNodePtr   x;
   ValNodePtr   y;
   ValNodePtr   z;
} ModelSpacePoints, PNTR ModelSpacePointsPtr;


ModelSpacePointsPtr LIBCALL ModelSpacePointsFree PROTO ((ModelSpacePointsPtr ));
ModelSpacePointsPtr LIBCALL ModelSpacePointsNew PROTO (( void ));
ModelSpacePointsPtr LIBCALL ModelSpacePointsAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
Boolean LIBCALL ModelSpacePointsAsnWrite PROTO (( ModelSpacePointsPtr , AsnIoPtr, AsnTypePtr));

typedef ValNodePtr AtomicTemperatureFactorsPtr;
typedef ValNode AtomicTemperatureFactors;
#define AtomicTemperatureFactors_isotropic 1
#define AtomicTemperatureFactors_anisotropic 2


AtomicTemperatureFactorsPtr LIBCALL AtomicTemperatureFactorsFree PROTO ((AtomicTemperatureFactorsPtr ));
AtomicTemperatureFactorsPtr LIBCALL AtomicTemperatureFactorsAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
Boolean LIBCALL AtomicTemperatureFactorsAsnWrite PROTO (( AtomicTemperatureFactorsPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    AtomicOccupancies
*
**************************************************/
typedef struct struct_Atomic_occupancies {
   Uint4 OBbits__;
   Int4   scale_factor;
   ValNodePtr   o;
} AtomicOccupancies, PNTR AtomicOccupanciesPtr;


AtomicOccupanciesPtr LIBCALL AtomicOccupanciesFree PROTO ((AtomicOccupanciesPtr ));
AtomicOccupanciesPtr LIBCALL AtomicOccupanciesNew PROTO (( void ));
AtomicOccupanciesPtr LIBCALL AtomicOccupanciesAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
Boolean LIBCALL AtomicOccupanciesAsnWrite PROTO (( AtomicOccupanciesPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    AlternateConformationIds
*
**************************************************/
typedef ValNode AlternateConformationIds;
typedef ValNodePtr AlternateConformationIdsPtr;
#define AlternateConformationIdsNew() ValNodeNew(NULL) 

#ifdef NLM_GENERATED_CODE_PROTO

AlternateConformationIdsPtr LIBCALL AlternateConformationIdsFree PROTO ((AlternateConformationIdsPtr ));
AlternateConformationIdsPtr LIBCALL AlternateConformationIdsNew PROTO (( void ));
AlternateConformationIdsPtr LIBCALL AlternateConformationIdsAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
Boolean LIBCALL AlternateConformationIdsAsnWrite PROTO (( AlternateConformationIdsPtr , AsnIoPtr, AsnTypePtr));

#endif /* NLM_GENERATED_CODE_PROTO */



/**************************************************
*
*    ConformationEnsemble
*
**************************************************/
typedef struct struct_Conformation_ensemble {
   struct struct_Conformation_ensemble PNTR next;
   Uint4 OBbits__;
   CharPtr   name;
   ValNodePtr   alt_conf_ids;
} ConformationEnsemble, PNTR ConformationEnsemblePtr;


ConformationEnsemblePtr LIBCALL ConformationEnsembleFree PROTO ((ConformationEnsemblePtr ));
ConformationEnsemblePtr LIBCALL ConformationEnsembleNew PROTO (( void ));
ConformationEnsemblePtr LIBCALL ConformationEnsembleAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
Boolean LIBCALL ConformationEnsembleAsnWrite PROTO (( ConformationEnsemblePtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    IsotropicTemperatureFactors
*
**************************************************/
typedef struct struct_Isotropic_temperature_factors {
   Uint4 OBbits__;
   Int4   scale_factor;
   ValNodePtr   b;
} IsotropicTemperatureFactors, PNTR IsotropicTemperatureFactorsPtr;


IsotropicTemperatureFactorsPtr LIBCALL IsotropicTemperatureFactorsFree PROTO ((IsotropicTemperatureFactorsPtr ));
IsotropicTemperatureFactorsPtr LIBCALL IsotropicTemperatureFactorsNew PROTO (( void ));
IsotropicTemperatureFactorsPtr LIBCALL IsotropicTemperatureFactorsAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
Boolean LIBCALL IsotropicTemperatureFactorsAsnWrite PROTO (( IsotropicTemperatureFactorsPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    AnisotropicTemperatureFactors
*
**************************************************/
typedef struct struct_Anisotropic_temperature_factors {
   Uint4 OBbits__;
   Int4   scale_factor;
   ValNodePtr   b_11;
   ValNodePtr   b_12;
   ValNodePtr   b_13;
   ValNodePtr   b_22;
   ValNodePtr   b_23;
   ValNodePtr   b_33;
} AnisotropicTemperatureFactors, PNTR AnisotropicTemperatureFactorsPtr;


AnisotropicTemperatureFactorsPtr LIBCALL AnisotropicTemperatureFactorsFree PROTO ((AnisotropicTemperatureFactorsPtr ));
AnisotropicTemperatureFactorsPtr LIBCALL AnisotropicTemperatureFactorsNew PROTO (( void ));
AnisotropicTemperatureFactorsPtr LIBCALL AnisotropicTemperatureFactorsAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
Boolean LIBCALL AnisotropicTemperatureFactorsAsnWrite PROTO (( AnisotropicTemperatureFactorsPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    TMesh
*
**************************************************/
typedef struct struct_T_mesh {
   Uint4 OBbits__;
   Int4   number_of_points;
   Int4   scale_factor;
   ValNodePtr   swap;
   ValNodePtr   x;
   ValNodePtr   y;
   ValNodePtr   z;
} TMesh, PNTR TMeshPtr;


TMeshPtr LIBCALL TMeshFree PROTO ((TMeshPtr ));
TMeshPtr LIBCALL TMeshNew PROTO (( void ));
TMeshPtr LIBCALL TMeshAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
Boolean LIBCALL TMeshAsnWrite PROTO (( TMeshPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    Triangles
*
**************************************************/
typedef struct struct_Triangles {
   Uint4 OBbits__;
   Int4   number_of_points;
   Int4   scale_factor;
   ValNodePtr   x;
   ValNodePtr   y;
   ValNodePtr   z;
   Int4   number_of_triangles;
   ValNodePtr   v1;
   ValNodePtr   v2;
   ValNodePtr   v3;
} Triangles, PNTR TrianglesPtr;


TrianglesPtr LIBCALL TrianglesFree PROTO ((TrianglesPtr ));
TrianglesPtr LIBCALL TrianglesNew PROTO (( void ));
TrianglesPtr LIBCALL TrianglesAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
Boolean LIBCALL TrianglesAsnWrite PROTO (( TrianglesPtr , AsnIoPtr, AsnTypePtr));

#ifdef __cplusplus
/* { */ }
#endif

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

#endif /* _objmmdb2_ */
