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
* $Revision: 6.8 $
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
#include <bspview.h>

#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif


#ifdef __cplusplus
extern "C" {
#endif

typedef enum layoutAlgorithm {
  Layout_Inherit = 0,           /* inherit layout from higher-level entity */
  Layout_DiagonalOneFeatPerLine,
  Layout_DiagonalSawtooth,
  Layout_FeatTypePerLine,
  Layout_FeatTypePerLineGroup,
  Layout_AllInOneLine,
  Layout_PackUpward,
  Layout_GroupCorrespondingFeats
} LayoutAlgorithm;

typedef enum renderChoice {
  Do_Not_Render = 0,
  Render_Line,
  Render_CappedLine,            /* |---------| */
  Render_Box,
  Render_OutlineBox
} RenderAlgorithm;

typedef enum labelLocEnum {
  LabelNone = 0,
  LabelInside,                  /* |====label====| */
  LabelAbove,
  LabelBelow,
  LabelLeft,
  LabelRight
} LabelLocEnum;

typedef enum vibSymbols {
  SymRectangle = 1,
  SymDiamond,
  SymOval,
  SymLeftTriangle,
  SymRightTriangle,
  SymUpTriangle,
  SymDownTriangle
} VibSymbols;

typedef struct appearanceItem {
  Boolean         AddDescToLabel;       /* label for a gene (xyz) includes "xyz" ? */
  Boolean         AddTypeToLabel;       /* label for a CDS includes "CDS: " ? */
  Uint1           Color [3];
  Uint1           FontColor [3];
  FonT            LabelFont;
  LabelLocEnum    LabelLoc;
  RenderAlgorithm RenderChoice;
  Int1            VibLinestyle;         /* these are as in AddAttribute */
  Uint1           Height;
  Int1            VibShading;
} AppearanceItem , PNTR AppearanceItemPtr;

typedef struct appearance {
  ValNodePtr      AppearanceItemList;   /* data.ptrvalue == AppearanceItemPtr */
  CharPtr         name;
  AppearanceItemPtr FeaturesAppearanceItem [FEATDEF_MAX];
} Appearance, PNTR AppearancePtr;

typedef enum {
  NoLabel = 0,
  LabelOnTop,
  LabelOnBottom,
  LabelOnSide
} GroupLabelLocation;

typedef struct filterItem {
  CharPtr            GroupLabel;
  Boolean            IncludeFeature [FEATDEF_MAX];
  Uint2              IntraRowPaddingPixels;
  LayoutAlgorithm    LayoutChoice;
  GroupLabelLocation LabelLocation;
  Uint1              RectColor [3];
  Boolean            DrawItemRect;
  Boolean            FillItemRect;
} FilterItem, PNTR FilterItemPtr;

typedef struct filter {
  ValNodePtr      FilterItemList;       /* data.ptrvalue == FilterItemPtr */
  CharPtr         name;
  LayoutAlgorithm DefaultLayout;
} Filter, PNTR FilterPtr;

typedef struct relevantFeatureItem {
  Uint4           Left, Right;
  Uint2           decorationHeight;
  Int4            decorationLeft, decorationRight;
  Uint2           Height;
  Boolean         StartOnLeft;
  Boolean         StartPartial, StopPartial;
  CharPtr         ContentLabel;
  Uint1           featdeftype;
  Int4Ptr         ivals;
  Int2            numivals;
  Uint2           entityID, itemID;
  Uint2           localSAPindex;
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
} ViewerConfigs, PNTR ViewerConfigsPtr;

/* Public Functions */

NLM_EXTERN SegmenT CreateGraphicViewFromBsp (
  BioseqPtr bsp,
  Int4 scale,
  FilterPtr FP,
  AppearancePtr AP,
  LayoutAlgorithm overrideLayout
);

/* If the same Bioseq will be rendered multiple times, it will be more efficient to use these functions */

NLM_EXTERN SegmenT CreateGraphicViewInternal (
  BioseqPtr bsp,
  RelevantFeaturesPtr feats,
  VoidPtr aligns, /* NULL for now, will become a RelevantGraphPtr*/
  VoidPtr graphs, /* likewise */
  Int4 scale,
  FilterPtr FP,
  AppearancePtr AP,
  LayoutAlgorithm overrideLayout
);

NLM_EXTERN RelevantFeaturesPtr CollectFeatures (BioseqPtr bsp);
NLM_EXTERN RelevantFeaturesPtr FreeCollectedFeatures (RelevantFeaturesPtr RFP);

/* GetGraphicConfigParseResults() parses the configuration file the 1st time it is called, but returns a cached copy afterward */
NLM_EXTERN ViewerConfigsPtr GetGraphicConfigParseResults (void);
NLM_EXTERN FilterPtr FindFilterByName (CharPtr name, ViewerConfigsPtr VCP);
NLM_EXTERN AppearancePtr FindAppearanceByName (CharPtr name, ViewerConfigsPtr VCP);

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

/* Data structure for adding to Sequin Bioseq viewer form */
NLM_EXTERN BioseqPageData asn2gphGphPageData;


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
