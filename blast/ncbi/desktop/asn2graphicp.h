/*   asn2graphicp.h
* ===========================================================================
*
*                            PUBLIC DOMAIN NOTICE
*            National Center for Biotechnology Information (NCBI)
*
*  This software/database is a "United States Government Work" under the
*  terms of the United States Copyright Act.  It was written as part of
*  the author's official duties as a United States Government employee and
*  thus cannot be copyrighted.  This software/database is freely available
*  to the public for use. The National Library of Medicine and the U.S.
*  Government do not place any restriction on its use or reproduction.
*  We would, however, appreciate having the NCBI and the author cited in
*  any work or product based on this material
*
*  Although all reasonable efforts have been taken to ensure the accuracy
*  and reliability of the software and data, the NLM and the U.S.
*  Government do not and cannot warrant the performance or results that
*  may be obtained by using this software or data. The NLM and the U.S.
*  Government disclaim all warranties, express or implied, including
*  warranties of performance, merchantability or fitness for any particular
*  purpose.
*
* ===========================================================================
*
* File Name:  asn2graphicp.h
*
* Author:  Eric Northup
*
* Version Creation Date:   11/8/01
*
* $Revision: 6.40 $
*
* File Description: 
*
* Modifications:  
* --------------------------------------------------------------------------
*
* ==========================================================================
*/

#ifndef _ASN2GRAPHICP_
#define _ASN2GRAPHICP_

#include <asn2graphic.h>
#include <objfdef.h>

#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif


#ifdef __cplusplus
extern "C" {
#endif

#define APPEARANCEITEM_Segment (FEATDEF_MAX + 1)
#define APPEARANCEITEM_Alignment (FEATDEF_MAX + 2)
#define APPEARANCEITEM_Graph (FEATDEF_MAX + 3)
#define APPEARANCEITEM_MAX (FEATDEF_MAX + 4)

typedef enum {
  Layout_Inherit = 0,           /* inherit layout from higher-level entity */
  Layout_Diagonal,
  /* Layout_DiagonalSawtooth, */
  Layout_FeatTypePerLine,
  Layout_FeatTypePerLineGroup,
  /*  Layout_AllInOneLine, */
  Layout_PackUpward,
  Layout_GroupCorrespondingFeats,
  Layout_GroupCorrespondingFeatsRepeat
} LayoutAlgorithm;

typedef enum {
  Do_Not_Render = 0,
  Render_Line,
  Render_CappedLine,            /* |---------| */
  Render_Box,
  Render_OutlineBox
} RenderAlgorithm;

typedef enum {
  LabelUnset = 0,
  LabelNone,
  LabelAboveClip, /* above, but not wider than the feature*/
  LabelAboveCull, /* above, but only displayed iff wider than the feature */
  LabelInside,
  LabelAbove,
  LabelBelow,
  LabelLeft,
  LabelRight
} LabelLocEnum;

typedef enum {
  NoGap = 0,
  LineGap,
  AngleGap
} GapEnum;

typedef struct appearanceItem {
  Uint1           LabelColor [3];
  Uint1           Color [3];
  RenderAlgorithm RenderChoice;
  Int1            VibLinestyle;         /* these are as in AddAttribute */
  Uint1           Height;
  Int1            VibShading;
  Boolean         ShowArrow;
  GapEnum         GapChoice;
  Boolean         AddDescToLabel;
  Boolean         AddTypeToLabel;
  FonT            LabelFont;
  LabelLocEnum    LabelLoc;
  Uint1           format;         /* label format, currently only valid for alignments. */
} AppearanceItem, PNTR AppearanceItemPtr;
  
typedef enum {
  NoLabel = 0,
  LabelOnTop,
  LabelOnBottom,
  LabelOnSide
} GroupLabelLocation;

typedef struct bioseqAppearanceItem {
  Boolean            drawScale;
  Uint1              bioseqColor [3];
  Uint1              labelColor [3];
  Uint1              scaleColor [3];
  FonT               labelFont;
  FonT               scaleFont;
  GroupLabelLocation labelLoc;
  Uint1              height;
  Uint1              scaleHeight;
  Uint1              format;
} BioseqAppearanceItem, PNTR BioseqAppearanceItemPtr;

typedef struct appearance {
  ValNodePtr               AppearanceItemList;   /* data.ptrvalue == AppearanceItemPtr */
  CharPtr                  name;
  AppearanceItemPtr        FeaturesAppearanceItem [APPEARANCEITEM_MAX];
  BioseqAppearanceItemPtr  bioseqAIP;
  Boolean                  ShadeSmears; /* if FALSE, multi-feature smears (which can't be selected/edited like normal features) will be hollow instead of filled */
  Uint2                    MaxScaleForArrow;
  Uint2                    MinPixelsForArrow;
  Uint1                    GroupLabelColor [3];
  FonT                     GroupLabelFont;  
  Uint1                    GroupBoxColor [3];
  Uint1                    AnnotLabelColor [3];
  FonT                     AnnotLabelFont;  
  Uint1                    AnnotBoxColor [3];
 
} Appearance, PNTR AppearancePtr;

typedef enum {
  InvalidFilter = 0,
  BioseqFilter,
  FeatureFilter,
  GraphFilter,
  AlignmentFilter
} FilterType;

typedef enum {
  InvalidStrand = 0,
  MinusStrand,
  PlusStrand,
  BothStrands
} StrandChoice;

typedef enum {
  TristateUnset = 0,
  TristateTrue,
  TristateFalse
} Tristate;

#define BOOL_TO_TRISTATE(aBool)   ((aBool) ? (TristateTrue) : (TristateFalse))
  /* bool_from_SET_tristate means that the tristate has already been checked, and was not "TristateUnset" */
#define BOOL_FROM_SET_TRISTATE(aTri) ((aTri == TristateTrue) ? (TRUE) : (FALSE))


/* IncludeFeature[featdef_xxx] is either 0 (not included) or a number which indictes its order in this filter (1 = first, ..., APPEARANCEITEM_MAX = last) */
typedef struct filterItem {
  FilterType         Type;
  CharPtr            GroupLabel;
  Uint1              GroupLabelColor [3];
  Boolean            GroupLabelColorSet;
  FonT               GroupLabelFont;  
  Boolean            GroupLabelFontSet;
  Uint1              GroupBoxColor [3];
  Boolean            GroupBoxColorSet;
  Uint1              IncludeFeature [APPEARANCEITEM_MAX];
  Uint2              IntraRowPaddingPixels;       /* number of blank (pixel) rows after _each_ row in this group*/
  Uint2              GroupPadding;               /* number of blank (pixel) rows after this group */
  LayoutAlgorithm    LayoutChoice;
  GroupLabelLocation GroupLabelLoc;
  Boolean            DrawGroupBox;
  Boolean            FillGroupBox;
  StrandChoice       MatchStrand;
  Tristate           AddDescToLabel;
  Tristate           AddTypeToLabel;
  LabelLocEnum       LabelLoc;
  Tristate           DrawScale;
} FilterItem, PNTR FilterItemPtr;

typedef struct filter {
  ValNodePtr      FilterItemList;       /* data.ptrvalue == FilterItemPtr */
  CharPtr         name;
  Uint2           MaxScaleWithLabels;    /* turn off all labels if scale > MaxScaleWithLabels*/
  Boolean             GroupByAnnot;         /* features or alignments in named annotations group separately? */
  Boolean             DrawAnnotBox;         /* draw a box around such features. */
  GroupLabelLocation  AnnotLabelLoc;
  Uint1               AnnotBoxColor[3];
  Boolean             AnnotBoxColorSet;
  FonT                AnnotLabelFont;  
  Boolean             AnnotLabelFontSet;
  Uint1               AnnotLabelColor [3];
  Boolean             AnnotLabelColorSet;
  
} Filter, PNTR FilterPtr;

typedef enum endPointType {
  EndAbsolute = 0,
  EndPartial,
  EndClipped
} EndPointType;  

typedef struct relevantFeatureItem {
  Int4            Left, Right;
  Uint1           featstrand;
  EndPointType    LeftEnd;
  EndPointType    RightEnd;
  CharPtr         ContentLabel;
  Uint1           featdeftype;
  Int4Ptr         ivals;
  Int2            numivals;
  Uint2           entityID, itemType;
  Uint4           itemID;
  Uint1           circularSpanningOrigin;   /* on a bioseq w/ Circular topology, this feature crosses the origin */
  SeqAnnotPtr     sap; /* was this feature found in a named SeqAnnot table?  0 if not, else a pointer to the SeqAnnot */
} RelevantFeatureItem, PNTR RelevantFeatureItemPtr;

typedef struct relevantFeatures {
  Uint4            featureCount;
  ValNodePtr       featVNP;             /* data.ptrvalue == RelevantFeatureItem [RELEVANT_FEATS_PER_CHUNK] */
  Uint1            sapCount;
  SeqAnnotPtr PNTR sapList;
} RelevantFeatures, PNTR RelevantFeaturesPtr;

typedef struct viewerConfigs {
  Boolean            ArraysPopulated;

  Uint1              AppearanceCount;
  AppearancePtr PNTR AppearanceArray;
  CharPtr PNTR       AppearanceNameArray;

  Uint1              FilterCount;
  FilterPtr PNTR     FilterArray;
  CharPtr PNTR       FilterNameArray;

  /* the linked-lists are only used while this structure is being built. */

  ValNodePtr         AppearanceList;       /* data.ptrvalue == CharPtr */
  ValNodePtr         AppearanceNameList;
  ValNodePtr         FilterList; 
  ValNodePtr         FilterNameList;
  Uint2              DefaultMaxScaleWithLabels;
  Uint2              DefaultMaxScaleForArrow;
  Uint2              DefaultMinPixelsForArrow;
  Uint2              DefaultGroupPadding;
  Uint2              DefaultRowPadding;
  Boolean            DefaultShadeSmears;  
} ViewerConfigs, PNTR ViewerConfigsPtr;

/* Public Functions */

NLM_EXTERN SegmenT CreateGraphicViewFromBsp (
  BioseqPtr bsp,
  SeqLocPtr location,
  Int4 scale,
  Int4Ptr ceiling,
  SegmenT topLevel,
  AppearancePtr AP,
  FilterPtr FP,
  LayoutAlgorithm overrideLayout,
  GraphicViewExtrasPtr extras
);

/* If the same Bioseq will be rendered multiple times, it will be more efficient to use these functions */

NLM_EXTERN SegmenT CreateGraphicViewInternal (
  BioseqPtr bsp,
  Int4 from,
  Int4 to,
  Boolean allFeatures,
  RelevantFeaturesPtr feats,
  Int4 scale,
  Int4Ptr ceiling,
  SegmenT topLevel,
  AppearancePtr AP,
  FilterPtr FP,
  LayoutAlgorithm overrideLayout,
  GraphicViewExtrasPtr extras
);

NLM_EXTERN RelevantFeaturesPtr CollectFeatures (BioseqPtr bsp);
NLM_EXTERN RelevantFeaturesPtr FreeCollectedFeatures (RelevantFeaturesPtr RFP);

/* GetGraphicConfigParseResults() parses the configuration file the 1st time it is called, but returns a cached copy afterward */
NLM_EXTERN ViewerConfigsPtr GetGraphicConfigParseResults (void);
NLM_EXTERN FilterPtr FindFilterByName (CharPtr name, ViewerConfigsPtr VCP);
NLM_EXTERN AppearancePtr FindAppearanceByName (CharPtr name, ViewerConfigsPtr VCP);
NLM_EXTERN LayoutAlgorithm FindLayoutByName (CharPtr name);

/* Functions for Manipulating Filter and Appearance Structures */
NLM_EXTERN Uint2 ParseConfigFile (ViewerConfigsPtr VCP);
NLM_EXTERN FilterPtr CreateFilter (CharPtr name, ViewerConfigsPtr VCP);
NLM_EXTERN AppearancePtr CreateAppearance (CharPtr newname, ViewerConfigsPtr VCP);
NLM_EXTERN void AddFeatureToFilterItem (FilterItemPtr FIP, Uint1 newFeatdef, ViewerConfigsPtr VCP);
NLM_EXTERN void RemoveFeatureFromFilterItem (FilterItemPtr FIP, Uint1 newFeatdef, ViewerConfigsPtr VCP);
NLM_EXTERN FilterItemPtr CreateNewFilterItemInFilter (CharPtr name, FilterPtr parent, ViewerConfigsPtr VCP);
NLM_EXTERN FilterPtr DestroyFilter (FilterPtr FP, ViewerConfigsPtr VCP);
NLM_EXTERN AppearancePtr DestroyAppearance (AppearancePtr AP, ViewerConfigsPtr VCP);
NLM_EXTERN void AddAppearanceItemToAppearance (AppearanceItemPtr AIP, AppearancePtr AP, Uint1 newFeatdef, ViewerConfigsPtr VCP);
NLM_EXTERN Uint2 GetAppearanceCount (void);
NLM_EXTERN Uint2 GetFilterCount (void);
NLM_EXTERN Uint2 GetLayoutCount (void);
NLM_EXTERN Uint2 GetAlnScoreCount (void);
NLM_EXTERN Uint2 GetAlnScoreCutoffCount (void);


#ifdef __cplusplus
}
#endif

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

#endif /* _ASN2GRAPHICP_ */
