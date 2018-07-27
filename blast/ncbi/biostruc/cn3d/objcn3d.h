#ifndef _objcn3d_ 
#define _objcn3d_ 

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
*    Generated objects for Module NCBI-Cn3d
*    Generated using ASNCODE Revision: 6.14 at Feb 6, 2003  9:02 AM
*
**************************************************/

NLM_EXTERN Boolean LIBCALL
objcn3dAsnLoad PROTO((void));


/**************************************************
*
*    Cn3dStyleDictionary
*
**************************************************/
typedef struct struct_Cn3d_style_dictionary {
   struct struct_Cn3d_style_settings PNTR   global_style;
   struct struct_Cn3d_style_table_item PNTR   style_table;
} Cn3dStyleDictionary, PNTR Cn3dStyleDictionaryPtr;


NLM_EXTERN Cn3dStyleDictionaryPtr LIBCALL Cn3dStyleDictionaryFree PROTO ((Cn3dStyleDictionaryPtr ));
NLM_EXTERN Cn3dStyleDictionaryPtr LIBCALL Cn3dStyleDictionaryNew PROTO (( void ));
NLM_EXTERN Cn3dStyleDictionaryPtr LIBCALL Cn3dStyleDictionaryAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL Cn3dStyleDictionaryAsnWrite PROTO (( Cn3dStyleDictionaryPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    Cn3dUserAnnotations
*
**************************************************/
typedef struct struct_Cn3d_user_annotations {
   struct struct_Cn3d_user_annotation PNTR   annotations;
   struct struct_Cn3d_view_settings PNTR   view;
} Cn3dUserAnnotations, PNTR Cn3dUserAnnotationsPtr;


NLM_EXTERN Cn3dUserAnnotationsPtr LIBCALL Cn3dUserAnnotationsFree PROTO ((Cn3dUserAnnotationsPtr ));
NLM_EXTERN Cn3dUserAnnotationsPtr LIBCALL Cn3dUserAnnotationsNew PROTO (( void ));
NLM_EXTERN Cn3dUserAnnotationsPtr LIBCALL Cn3dUserAnnotationsAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL Cn3dUserAnnotationsAsnWrite PROTO (( Cn3dUserAnnotationsPtr , AsnIoPtr, AsnTypePtr));

/* following #defines are for enumerated type, not used by object loaders */
#define Cn3d_backbone_type_off 1
#define Cn3d_backbone_type_trace 2
#define Cn3d_backbone_type_partial 3
#define Cn3d_backbone_type_complete 4

/* following #defines are for enumerated type, not used by object loaders */
#define Cn3d_drawing_style_wire 1
#define Cn3d_drawing_style_tubes 2
#define Cn3d_drawing_style_ball_and_stick 3
#define Cn3d_drawing_style_space_fill 4
#define Cn3d_drawing_style_wire_worm 5
#define Cn3d_drawing_style_tube_worm 6
#define Cn3d_drawing_style_with_arrows 7
#define Cn3d_drawing_style_without_arrows 8

/* following #defines are for enumerated type, not used by object loaders */
#define Cn3d_color_scheme_element 1
#define Cn3d_color_scheme_object 2
#define Cn3d_color_scheme_molecule 3
#define Cn3d_color_scheme_domain 4
#define Cn3d_color_scheme_secondary_structure 5
#define Cn3d_color_scheme_user_select 6
#define Cn3d_color_scheme_aligned 7
#define Cn3d_color_scheme_identity 8
#define Cn3d_color_scheme_variety 9
#define Cn3d_color_scheme_weighted_variety 10
#define Cn3d_color_scheme_information_content 11
#define Cn3d_color_scheme_fit 12
#define Cn3d_color_scheme_block_fit 17
#define Cn3d_color_scheme_block_z_fit 18
#define Cn3d_color_scheme_block_row_fit 19
#define Cn3d_color_scheme_temperature 13
#define Cn3d_color_scheme_hydrophobicity 14
#define Cn3d_color_scheme_charge 15
#define Cn3d_color_scheme_rainbow 16



/**************************************************
*
*    Cn3dColor
*
**************************************************/
typedef struct struct_Cn3d_color {
   Int4   scale_factor;
   Int4   red;
   Int4   green;
   Int4   blue;
   Int4   alpha;
} Cn3dColor, PNTR Cn3dColorPtr;


NLM_EXTERN Cn3dColorPtr LIBCALL Cn3dColorFree PROTO ((Cn3dColorPtr ));
NLM_EXTERN Cn3dColorPtr LIBCALL Cn3dColorNew PROTO (( void ));
NLM_EXTERN Cn3dColorPtr LIBCALL Cn3dColorAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL Cn3dColorAsnWrite PROTO (( Cn3dColorPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    Cn3dBackboneStyle
*
**************************************************/
typedef struct struct_Cn3d_backbone_style {
   Uint2   type;
   Uint2   style;
   Uint2   color_scheme;
   struct struct_Cn3d_color PNTR   user_color;
} Cn3dBackboneStyle, PNTR Cn3dBackboneStylePtr;


NLM_EXTERN Cn3dBackboneStylePtr LIBCALL Cn3dBackboneStyleFree PROTO ((Cn3dBackboneStylePtr ));
NLM_EXTERN Cn3dBackboneStylePtr LIBCALL Cn3dBackboneStyleNew PROTO (( void ));
NLM_EXTERN Cn3dBackboneStylePtr LIBCALL Cn3dBackboneStyleAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL Cn3dBackboneStyleAsnWrite PROTO (( Cn3dBackboneStylePtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    Cn3dGeneralStyle
*
**************************************************/
typedef struct struct_Cn3d_general_style {
   Uint1   is_on;
   Uint2   style;
   Uint2   color_scheme;
   struct struct_Cn3d_color PNTR   user_color;
} Cn3dGeneralStyle, PNTR Cn3dGeneralStylePtr;


NLM_EXTERN Cn3dGeneralStylePtr LIBCALL Cn3dGeneralStyleFree PROTO ((Cn3dGeneralStylePtr ));
NLM_EXTERN Cn3dGeneralStylePtr LIBCALL Cn3dGeneralStyleNew PROTO (( void ));
NLM_EXTERN Cn3dGeneralStylePtr LIBCALL Cn3dGeneralStyleAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL Cn3dGeneralStyleAsnWrite PROTO (( Cn3dGeneralStylePtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    Cn3dBackboneLabelStyle
*
**************************************************/
typedef struct struct_Cn3d_backbone_label_style {
   Int4   spacing;
   Uint2   type;
   /* following #defines are for enumerated type, not used by object loaders */
#define Cn3d_backbone_label_style_type_one_letter 1
#define Cn3d_backbone_label_style_type_three_letter 2

   Uint2   number;
   /* following #defines are for enumerated type, not used by object loaders */
#define Cn3d_backbone_label_style_number_none 0
#define Cn3d_backbone_label_style_number_sequential 1
#define Cn3d_backbone_label_style_number_pdb 2

   Uint1   termini;
   Uint1   white;
} Cn3dBackboneLabelStyle, PNTR Cn3dBackboneLabelStylePtr;


NLM_EXTERN Cn3dBackboneLabelStylePtr LIBCALL Cn3dBackboneLabelStyleFree PROTO ((Cn3dBackboneLabelStylePtr ));
NLM_EXTERN Cn3dBackboneLabelStylePtr LIBCALL Cn3dBackboneLabelStyleNew PROTO (( void ));
NLM_EXTERN Cn3dBackboneLabelStylePtr LIBCALL Cn3dBackboneLabelStyleAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL Cn3dBackboneLabelStyleAsnWrite PROTO (( Cn3dBackboneLabelStylePtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    Cn3dStyleSettings
*
**************************************************/
typedef struct struct_Cn3d_style_settings {
   struct struct_Cn3d_style_settings PNTR next;
   CharPtr   name;
   struct struct_Cn3d_backbone_style PNTR   protein_backbone;
   struct struct_Cn3d_backbone_style PNTR   nucleotide_backbone;
   struct struct_Cn3d_general_style PNTR   protein_sidechains;
   struct struct_Cn3d_general_style PNTR   nucleotide_sidechains;
   struct struct_Cn3d_general_style PNTR   heterogens;
   struct struct_Cn3d_general_style PNTR   solvents;
   struct struct_Cn3d_general_style PNTR   connections;
   struct struct_Cn3d_general_style PNTR   helix_objects;
   struct struct_Cn3d_general_style PNTR   strand_objects;
   Uint1   virtual_disulfides_on;
   struct struct_Cn3d_color PNTR   virtual_disulfide_color;
   Uint1   hydrogens_on;
   struct struct_Cn3d_color PNTR   background_color;
   Int4   scale_factor;
   Int4   space_fill_proportion;
   Int4   ball_radius;
   Int4   stick_radius;
   Int4   tube_radius;
   Int4   tube_worm_radius;
   Int4   helix_radius;
   Int4   strand_width;
   Int4   strand_thickness;
   struct struct_Cn3d_backbone_label_style PNTR   protein_labels;
   struct struct_Cn3d_backbone_label_style PNTR   nucleotide_labels;
   Uint1   ion_labels;
} Cn3dStyleSettings, PNTR Cn3dStyleSettingsPtr;


NLM_EXTERN Cn3dStyleSettingsPtr LIBCALL Cn3dStyleSettingsFree PROTO ((Cn3dStyleSettingsPtr ));
NLM_EXTERN Cn3dStyleSettingsPtr LIBCALL Cn3dStyleSettingsNew PROTO (( void ));
NLM_EXTERN Cn3dStyleSettingsPtr LIBCALL Cn3dStyleSettingsAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL Cn3dStyleSettingsAsnWrite PROTO (( Cn3dStyleSettingsPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    Cn3dStyleSettingsSet
*
**************************************************/
typedef struct struct_Cn3d_style_settings Cn3dStyleSettingsSet;
typedef struct struct_Cn3d_style_settings PNTR Cn3dStyleSettingsSetPtr;
#define Cn3dStyleSettingsSetNew() Cn3d_style_settingsNew() 

#ifdef NLM_GENERATED_CODE_PROTO

NLM_EXTERN Cn3dStyleSettingsSetPtr LIBCALL Cn3dStyleSettingsSetFree PROTO ((Cn3dStyleSettingsSetPtr ));
NLM_EXTERN Cn3dStyleSettingsSetPtr LIBCALL Cn3dStyleSettingsSetNew PROTO (( void ));
NLM_EXTERN Cn3dStyleSettingsSetPtr LIBCALL Cn3dStyleSettingsSetAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL Cn3dStyleSettingsSetAsnWrite PROTO (( Cn3dStyleSettingsSetPtr , AsnIoPtr, AsnTypePtr));

#endif /* NLM_GENERATED_CODE_PROTO */



/**************************************************
*
*    Cn3dStyleTableItem
*
**************************************************/
typedef struct struct_Cn3d_style_table_item {
   struct struct_Cn3d_style_table_item PNTR next;
   Int4   id;
   struct struct_Cn3d_style_settings PNTR   style;
} Cn3dStyleTableItem, PNTR Cn3dStyleTableItemPtr;


NLM_EXTERN Cn3dStyleTableItemPtr LIBCALL Cn3dStyleTableItemFree PROTO ((Cn3dStyleTableItemPtr ));
NLM_EXTERN Cn3dStyleTableItemPtr LIBCALL Cn3dStyleTableItemNew PROTO (( void ));
NLM_EXTERN Cn3dStyleTableItemPtr LIBCALL Cn3dStyleTableItemAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL Cn3dStyleTableItemAsnWrite PROTO (( Cn3dStyleTableItemPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    Cn3dResidueRange
*
**************************************************/
typedef struct struct_Cn3d_residue_range {
   struct struct_Cn3d_residue_range PNTR next;
   Int4   from;
   Int4   to;
} Cn3dResidueRange, PNTR Cn3dResidueRangePtr;


NLM_EXTERN Cn3dResidueRangePtr LIBCALL Cn3dResidueRangeFree PROTO ((Cn3dResidueRangePtr ));
NLM_EXTERN Cn3dResidueRangePtr LIBCALL Cn3dResidueRangeNew PROTO (( void ));
NLM_EXTERN Cn3dResidueRangePtr LIBCALL Cn3dResidueRangeAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL Cn3dResidueRangeAsnWrite PROTO (( Cn3dResidueRangePtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    Cn3dMoleculeLocation
*
**************************************************/
typedef struct struct_Cn3d_molecule_location {
   struct struct_Cn3d_molecule_location PNTR next;
   Int4   molecule_id;
   struct struct_Cn3d_residue_range PNTR   residues;
} Cn3dMoleculeLocation, PNTR Cn3dMoleculeLocationPtr;


NLM_EXTERN Cn3dMoleculeLocationPtr LIBCALL Cn3dMoleculeLocationFree PROTO ((Cn3dMoleculeLocationPtr ));
NLM_EXTERN Cn3dMoleculeLocationPtr LIBCALL Cn3dMoleculeLocationNew PROTO (( void ));
NLM_EXTERN Cn3dMoleculeLocationPtr LIBCALL Cn3dMoleculeLocationAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL Cn3dMoleculeLocationAsnWrite PROTO (( Cn3dMoleculeLocationPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    Cn3dObjectLocation
*
**************************************************/
typedef struct struct_Cn3d_object_location {
   struct struct_Cn3d_object_location PNTR next;
   ValNodePtr   structure_id;
   struct struct_Cn3d_molecule_location PNTR   residues;
} Cn3dObjectLocation, PNTR Cn3dObjectLocationPtr;


NLM_EXTERN Cn3dObjectLocationPtr LIBCALL Cn3dObjectLocationFree PROTO ((Cn3dObjectLocationPtr ));
NLM_EXTERN Cn3dObjectLocationPtr LIBCALL Cn3dObjectLocationNew PROTO (( void ));
NLM_EXTERN Cn3dObjectLocationPtr LIBCALL Cn3dObjectLocationAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL Cn3dObjectLocationAsnWrite PROTO (( Cn3dObjectLocationPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    Cn3dUserAnnotation
*
**************************************************/
typedef struct struct_Cn3d_user_annotation {
   struct struct_Cn3d_user_annotation PNTR next;
   CharPtr   name;
   CharPtr   description;
   Int4   style_id;
   struct struct_Cn3d_object_location PNTR   residues;
   Uint1   is_on;
} Cn3dUserAnnotation, PNTR Cn3dUserAnnotationPtr;


NLM_EXTERN Cn3dUserAnnotationPtr LIBCALL Cn3dUserAnnotationFree PROTO ((Cn3dUserAnnotationPtr ));
NLM_EXTERN Cn3dUserAnnotationPtr LIBCALL Cn3dUserAnnotationNew PROTO (( void ));
NLM_EXTERN Cn3dUserAnnotationPtr LIBCALL Cn3dUserAnnotationAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL Cn3dUserAnnotationAsnWrite PROTO (( Cn3dUserAnnotationPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    Cn3dGLMatrix
*
**************************************************/
typedef struct struct_Cn3d_GL_matrix {
   FloatHi   m0;
   FloatHi   m1;
   FloatHi   m2;
   FloatHi   m3;
   FloatHi   m4;
   FloatHi   m5;
   FloatHi   m6;
   FloatHi   m7;
   FloatHi   m8;
   FloatHi   m9;
   FloatHi   m10;
   FloatHi   m11;
   FloatHi   m12;
   FloatHi   m13;
   FloatHi   m14;
   FloatHi   m15;
} Cn3dGLMatrix, PNTR Cn3dGLMatrixPtr;


NLM_EXTERN Cn3dGLMatrixPtr LIBCALL Cn3dGLMatrixFree PROTO ((Cn3dGLMatrixPtr ));
NLM_EXTERN Cn3dGLMatrixPtr LIBCALL Cn3dGLMatrixNew PROTO (( void ));
NLM_EXTERN Cn3dGLMatrixPtr LIBCALL Cn3dGLMatrixAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL Cn3dGLMatrixAsnWrite PROTO (( Cn3dGLMatrixPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    Cn3dVector
*
**************************************************/
typedef struct struct_Cn3d_vector {
   FloatHi   x;
   FloatHi   y;
   FloatHi   z;
} Cn3dVector, PNTR Cn3dVectorPtr;


NLM_EXTERN Cn3dVectorPtr LIBCALL Cn3dVectorFree PROTO ((Cn3dVectorPtr ));
NLM_EXTERN Cn3dVectorPtr LIBCALL Cn3dVectorNew PROTO (( void ));
NLM_EXTERN Cn3dVectorPtr LIBCALL Cn3dVectorAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL Cn3dVectorAsnWrite PROTO (( Cn3dVectorPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    Cn3dViewSettings
*
**************************************************/
typedef struct struct_Cn3d_view_settings {
   FloatHi   camera_distance;
   FloatHi   camera_angle_rad;
   FloatHi   camera_look_at_X;
   FloatHi   camera_look_at_Y;
   FloatHi   camera_clip_near;
   FloatHi   camera_clip_far;
   struct struct_Cn3d_GL_matrix PNTR   matrix;
   struct struct_Cn3d_vector PNTR   rotation_center;
} Cn3dViewSettings, PNTR Cn3dViewSettingsPtr;


NLM_EXTERN Cn3dViewSettingsPtr LIBCALL Cn3dViewSettingsFree PROTO ((Cn3dViewSettingsPtr ));
NLM_EXTERN Cn3dViewSettingsPtr LIBCALL Cn3dViewSettingsNew PROTO (( void ));
NLM_EXTERN Cn3dViewSettingsPtr LIBCALL Cn3dViewSettingsAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL Cn3dViewSettingsAsnWrite PROTO (( Cn3dViewSettingsPtr , AsnIoPtr, AsnTypePtr));

#ifdef __cplusplus
/* { */ }
#endif

#endif /* _objcn3d_ */

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

