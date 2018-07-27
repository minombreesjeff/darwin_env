/*   asn2graphic.c
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
* File Name:  asn2graphic.c
* Author:  Eric Northup
*
* Version Creation Date:   11/8/01
*
* $Revision: 6.17 $
*
* File Description: 
*
* Modifications:  
* --------------------------------------------------------------------------
*
* ==========================================================================
*/

#include <asn2graphicp.h>
#include <objsset.h>
#include <objsub.h>
#include <objfdef.h>
#include <seqport.h>
#include <sequtil.h>
#include <sqnutils.h>
#include <subutil.h>
#include <tofasta.h>
#include <explore.h>
#include <utilpub.h>
#include <jzmisc.h>
#include <gbfeat.h>
#include <gbftdef.h>
#include <edutil.h>
#include <sqnutils.h>
#include <drawingp.h>
#include <viewerp.h>

#define RELEVANT_FEATS_PER_CHUNK 128

/* these do not apply to individual features; they is used, ie in "AddFeatureToFilter (FEATDEF_ALL_RNA. . .)
   this will make the filter include _all_ types of RNA */
#define FEATDEF_ANY_RNA (FEATDEF_MAX + 2)
#define FEATDEF_ANY_PROT (FEATDEF_MAX + 1)

typedef struct renderInput {
  AppearanceItemPtr AIP;
  RelevantFeatureItemPtr RFIP;
  SegmenT         labelSeg;
  SegmenT         drawSeg;
  Int2            yStart;
  Int2            rowHeight;
} RenderInput, PNTR RenderInputPtr;

typedef struct filterProcessState {
  FilterItemPtr   currentFIP;
  ValNodePtr      currentFilterVNP;
  ValNodePtr      currentRFIPblockVNP;
  Uint4           featureBlockOffset;
  Uint2           indexInBlock;
  SegmenT         labelSegs [FEATDEF_MAX];
  SegmenT         drawSegs [FEATDEF_MAX];
  Int2            ceiling;
  RenderInput     renderParm;
  BoolPtr         featuresProcessed;    /*points to an array of boolean [vContext->featureCount] */
} FilterProcessState, PNTR FilterProcessStatePtr;

typedef struct viewerInstanceData {
  BioseqPtr       BSP;
  Uint4           viewScale;
  Uint4           seqLength;
  SeqAnnotPtr PNTR sapList;
  Uint2           sapCount;
  SegmenT         drawOnMe;
  SegmenT         topLevelSeg;
  Uint2           featureCount;
  ValNodePtr      featVNP;      /* data.ptrvalue == RelevantFeatureItem [RELEVANT_FEATS_PER_CHUNK] */
  AppearancePtr   AppPtr;
  FilterPtr       FltPtr;
  LayoutAlgorithm overrideLayout;
} ViewerContext, PNTR ViewerContextPtr;

typedef void (*RenderFuncPtr)    (RenderInputPtr rip, ViewerContextPtr vContext);
typedef void (*GetDimensionsPtr) (RenderInputPtr, Int4Ptr start, Int4Ptr stop, Uint2Ptr height, ViewerContextPtr vContext);

typedef struct renderClass {
  RenderFuncPtr RenderFunc;
  GetDimensionsPtr GetDimensions;
} RenderClass, PNTR RenderClassPtr;

typedef struct internalRow {
  Uint2           rowFeatureCount;
  DataVal         layoutData;
  ValNodePtr      feats;        /* data.ptrvalue == RelevantFeatureItemPtr */
  struct internalRow PNTR next;
} InternalRow, PNTR InternalRowPtr;

/* returns the total number of _rows_ */
typedef Uint2   (PNTR LayoutFunction) (InternalRowPtr firstRow, FilterProcessStatePtr FPSP, ViewerContextPtr vContext);


static CharPtr  LayoutStrings [] = {
  "inherit",
  "diagonal",
  "sawtooth",
  "onetypeperline",
  "onelineonly",
  "packupward",
  "groupcorrespondingfeatures",
  NULL
};

static LayoutAlgorithm LayoutValues [] = {
  Layout_Inherit,
  Layout_DiagonalOneFeatPerLine,
  Layout_DiagonalSawtooth,
  Layout_FeatTypePerLine,
  Layout_AllInOneLine,
  Layout_PackUpward,
  Layout_GroupCorrespondingFeats,
};

static CharPtr  LlocStrings [] = {
  "none",
  "inside",
  "above",
  "below",
  "left", 
  "right",
  NULL
};

static LabelLocEnum LlocValues [] = { 
  LabelNone,
  LabelInside,
  LabelAbove,
  LabelBelow,
  LabelLeft,
  LabelRight
};

static CharPtr  BoolStrings [] = {
  "yes",
  "true",
  "on",
  "no",
  "false",
  "off",
  NULL
};

static Boolean  BoolValues [] = {
  TRUE,
  TRUE,
  TRUE,
  FALSE,
  FALSE,
  FALSE
};

static CharPtr  RenderStrings [] = {
  "none",
  "line",
  "cappedline",
  "box",
  "outlinebox",
  NULL
};

static RenderAlgorithm RenderValues [] = { 
  Do_Not_Render,
  Render_Line,
  Render_CappedLine,
  Render_Box,
  Render_OutlineBox
};

static CharPtr NewGphLayoutNames [] = {
  "Inherit", /* Do not override layout in filter */
  "Diagonal", /* One feature per row */
  "Compact", /* Overlapping features are drawn in different rows, but this trys to minimize the number of rows, */
  "Smear", /* one row contains all features of a given type (overlapping features will render as an anonymous smear */
  "By Type", /* like "compact", except that no row will contain _different_ types of features (this may use more rows) */
  "Single Row", /* _all_ features are rendered in a single row (equivalent to "smear" if given only one type of features)*/
  "Gene Products", /* not yet implemented. . . */
  NULL
};

static LayoutAlgorithm NewGphLayoutValues [] = {
  Layout_Inherit,
  Layout_DiagonalOneFeatPerLine,
  Layout_PackUpward,
  Layout_FeatTypePerLine,
  Layout_FeatTypePerLineGroup,
  Layout_AllInOneLine,
  Layout_GroupCorrespondingFeats,
  Layout_Inherit
};

static Char     config_filename [] = "asn2gph";
extern Boolean  DisableSmearing;        /* for demonstration purposes only */

/* COLOR must point to an array of Uint1 [3]; */
static Boolean ParseColor (
  CharPtr string,
  Uint1Ptr color
)

{
  unsigned  sscanfOffset, localColor [3] = { 0 }; /* "unsigned", to match %u and %n*/
  Uint1  offset;
  Uint1  i;

  if (string == NULL || color == NULL) return FALSE;
  offset = 0;
  for (i = 0; i < 3; i++) {
    for (; !IS_DIGIT (string [offset]); offset++) continue;
    if (string [offset] == '\0') return FALSE;
    sscanf (string + offset, "%u%n", localColor + i, &sscanfOffset);
    offset += sscanfOffset;
  }
  color [0] = localColor [0];
  color [1] = localColor [1];
  color [2] = localColor [2];
  return TRUE;
}

static Int1 StringIndexInStringList (CharPtr testString, CharPtr PNTR stringList) {

  Int1  i;

  if (testString == NULL || stringList == NULL)
    return -1;
  i = 0;
  while (stringList [i] != NULL) {
    if (StringICmp (testString, stringList [i]) == 0) return i;
    i++;
  }
  return -1;
}

NLM_EXTERN FilterPtr FindFilterByName (
  CharPtr name,
  ViewerConfigsPtr VCP
)

{
  Uint1  i;

  if (VCP == NULL || StringHasNoText (name) || !VCP->ArraysPopulated) return NULL;
  i = StringIndexInStringList (name, VCP->FilterNameArray);
  if (i < VCP->FilterCount && i >= 0) return (VCP->FilterArray[i]);
  return NULL;
}

NLM_EXTERN AppearancePtr FindAppearanceByName (
  CharPtr name,
  ViewerConfigsPtr VCP
)

{
  Uint1  i;

  if (VCP == NULL || StringHasNoText (name) || !VCP->ArraysPopulated) return NULL;
  i = StringIndexInStringList (name, VCP->AppearanceNameArray);
  if (i < VCP->AppearanceCount && i>= 0) return (VCP->AppearanceArray[i]);
  return NULL;
}

static FilterPtr FindFilterByName_T (
  CharPtr name,
  ViewerConfigsPtr VCP
)

{
  ValNodePtr  nameVNP, filtVNP;

  if (VCP == NULL || StringHasNoText (name)) return NULL;
  for (nameVNP = VCP->FilterNameList, filtVNP = VCP->FilterList;
       nameVNP != NULL && filtVNP != NULL;
       nameVNP = nameVNP->next, filtVNP = filtVNP->next) {
    if (nameVNP->data.ptrvalue != NULL &&
        StringICmp (name, nameVNP->data.ptrvalue) == 0) {
      return ((FilterPtr) filtVNP->data.ptrvalue);
    }
  }
  return NULL;
}

static AppearancePtr FindAppearanceByName_T (
  CharPtr name,
  ViewerConfigsPtr VCP
)

{
  ValNodePtr  nameVNP, appVNP;

  if (VCP == NULL || StringHasNoText (name)) return NULL;
  for (nameVNP = VCP->AppearanceNameList, appVNP = VCP->AppearanceList;
       nameVNP != NULL && appVNP != NULL;
       nameVNP = nameVNP->next, appVNP = appVNP->next) {
    if (nameVNP->data.ptrvalue != NULL &&
        StringICmp (name, nameVNP->data.ptrvalue) == 0) {
      return ((AppearancePtr) appVNP->data.ptrvalue);
    }
  }
  return NULL;
}

NLM_EXTERN AppearancePtr CreateAppearance (
  CharPtr newname,
  ViewerConfigsPtr VCP
)

{
  AppearancePtr   AP;

  if (VCP == NULL || StringHasNoText (newname)) return NULL;
  if (FindAppearanceByName_T (newname, VCP) != NULL) return NULL;  /* don't allow duplicate names */
  AP = (AppearancePtr) MemNew (sizeof (Appearance));
  if (AP == NULL) return NULL;
  AP->name = StringSave (newname);
  VCP->AppearanceCount++;
  ValNodeAddPointer (&VCP->AppearanceList, VCP->AppearanceCount, AP);
  ValNodeAddPointer (&VCP->AppearanceNameList, VCP->AppearanceCount, AP->name);
  return AP;
}

static AppearanceItemPtr ParseAppearanceItem (
  CharPtr sectionName,
  AppearanceItemPtr inheritFromMe,
  Boolean recursing,
  ViewerConfigsPtr VCP
)

{
  Char               inputBuffer [128];
  AppearanceItemPtr  newAIP;
  unsigned           i; /* "unsigned" to match printf("%d")*/
  Boolean            changed = FALSE;

  if (! recursing) {
    if (GetAppParam (config_filename, sectionName, "usenamedstyle", NULL, inputBuffer, sizeof (inputBuffer))) {
      return ParseAppearanceItem (inputBuffer, inheritFromMe, TRUE, VCP);
    }
  }
  newAIP = MemNew (sizeof (AppearanceItem));
  if (newAIP == NULL) return NULL;
  MemCopy (newAIP, inheritFromMe, sizeof (AppearanceItem));
  if (GetAppParam (config_filename, sectionName, "label", NULL, inputBuffer, sizeof (inputBuffer))) {
    i = StringIndexInStringList (inputBuffer, LlocStrings);
    if (i < DIM (LlocValues)) {
      newAIP->LabelLoc = LlocValues [i];
      changed = TRUE;
    }
  }
  if (GetAppParam (config_filename, sectionName, "showtype", NULL, inputBuffer, sizeof (inputBuffer))) {
    i = StringIndexInStringList (inputBuffer, BoolStrings);
    if (i < DIM (BoolValues)) {
      newAIP->AddTypeToLabel = BoolValues [i];
      changed = TRUE;
    }
  }
  if (GetAppParam (config_filename, sectionName, "showcontent", NULL, inputBuffer, sizeof (inputBuffer))) {
    i = StringIndexInStringList (inputBuffer, LlocStrings);
    if (i < DIM (LlocValues)) {
      newAIP->AddDescToLabel = LlocValues [i];
      changed = TRUE;
    }
  }
  if (GetAppParam (config_filename, sectionName, "color", NULL, inputBuffer, sizeof (inputBuffer))) {
    changed = ParseColor (inputBuffer, newAIP->Color);
  }
  if (GetAppParam (config_filename, sectionName, "fontcolor", NULL, inputBuffer, sizeof (inputBuffer))) {
    changed = ParseColor (inputBuffer, newAIP->FontColor);
  }
  if (GetAppParam (config_filename, sectionName, "font", NULL, inputBuffer, sizeof (inputBuffer))) {
    newAIP->LabelFont = ParseFont (inputBuffer);
    if (newAIP->LabelFont == NULL) {
      newAIP->LabelFont = systemFont;
    }
    changed = TRUE;
  }
  if (GetAppParam (config_filename, sectionName, "displaywith", NULL, inputBuffer, sizeof (inputBuffer))) {
    i = StringIndexInStringList (inputBuffer, RenderStrings);
    if (i < DIM (RenderValues)) {
      newAIP->RenderChoice = RenderValues [i];
      changed = TRUE;
    }
  }
  if (GetAppParam (config_filename, sectionName, "height", NULL, inputBuffer, sizeof (inputBuffer))) {
    if (inputBuffer != NULL) {
      sscanf (inputBuffer, "%ud", &i);
      newAIP->Height = MIN (i, 6);
      changed = TRUE;
    }
  }
  if (! changed) {
    MemFree (newAIP);
    return NULL;
  }
  return newAIP;
}

static AppearancePtr ParseAppearance (
  CharPtr appearanceNameInFile,
 ViewerConfigsPtr VCP
)

{
  AppearancePtr      AP;
  AppearanceItemPtr  AIP, impAIP, newAIP;
  Char               inputBuffer [128];
  Char               sectionName [128];
  Char               outputBuffer [128];
  AppearanceItem     DefaultAppearanceItem = {
    TRUE, TRUE, {0, 0, 0}, {0, 0, 0}, NULL, LabelAbove, Render_Box, 0, 1, 0
  };
  Uint1              i;

  if (appearanceNameInFile == NULL) return NULL;
  sprintf (sectionName, "%s.master", appearanceNameInFile);
  /* require all styles to have a name, since high-level interface uses the name to identify Filters */
  if (! GetAppParam (config_filename, sectionName, "name", NULL, inputBuffer, sizeof (inputBuffer))) return NULL;
  if (StringHasNoText (inputBuffer)) return NULL;
  AP = CreateAppearance (inputBuffer, VCP);
  if (AP == NULL) return NULL;
  DefaultAppearanceItem.LabelFont = systemFont;
  AIP = ParseAppearanceItem (sectionName, &DefaultAppearanceItem, FALSE, VCP); /*parse xyz.master */
  if (AIP == NULL) {            /* require a "master" style */
    DestroyAppearance (AP, VCP);
    return NULL;
  }
  sprintf (outputBuffer, "%s.imp", appearanceNameInFile);
  impAIP = ParseAppearanceItem (outputBuffer, AIP, FALSE, VCP);
  if (impAIP == NULL) {
    impAIP = AIP;
  } else {
    AddAppearanceItemToAppearance (impAIP, AP, FEATDEF_IMP, VCP);
  }
  for (i = 1; i < FEATDEF_MAX; i++) {
    if (i == FEATDEF_IMP) continue;
    sprintf (outputBuffer, "%s.%s", appearanceNameInFile, FindKeyFromFeatDefType (i, FALSE));
    if (i >= FEATDEF_allele && i <= FEATDEF_35_signal) {        /* is it an imp-feat ? */
      newAIP = ParseAppearanceItem (outputBuffer, impAIP, FALSE, VCP);
      newAIP = newAIP ? newAIP : impAIP;
    } else {
      newAIP = ParseAppearanceItem (outputBuffer, AIP, FALSE, VCP);
      newAIP = newAIP ? newAIP : AIP;
    }
    if (newAIP != NULL) {
      AddAppearanceItemToAppearance (newAIP, AP, i, VCP);
    }
  }
  return AP;
}

NLM_EXTERN FilterPtr CreateFilter (
  CharPtr name,
  ViewerConfigsPtr VCP
)

{
  FilterPtr  FP;

  if (VCP == NULL || StringHasNoText (name)) return NULL;
  if (FindFilterByName_T (name, VCP) != NULL) return NULL;  /* don't allow duplicate names */
  FP = MemNew (sizeof (Filter));
  if (FP == NULL) return FP;
  FP->name = StringSave (name);
  FP->DefaultLayout = Layout_PackUpward;
  VCP->FilterCount++;
  ValNodeAddPointer (&VCP->FilterList, VCP->FilterCount, FP);
  ValNodeAddPointer (&VCP->FilterNameList, VCP->FilterCount, FP->name);
  return FP;
}

static void ChangeFeatureInFilterItem (
  FilterItemPtr FIP,
  Uint1 newFeatdef,
  Boolean includeMe,
  ViewerConfigsPtr VCP
)

{
  Uint1  i;

  if (FIP == NULL) return;
  if (newFeatdef == FEATDEF_ANY) {
    for (i = 0; i < FEATDEF_MAX; i++) {
      FIP->IncludeFeature [i] = includeMe;
    }
  } else if (newFeatdef == FEATDEF_ANY_RNA) {
    for (i = FEATDEF_preRNA; i <= FEATDEF_otherRNA; i++) {
      FIP->IncludeFeature [i] = includeMe;
    }
    FIP->IncludeFeature [FEATDEF_misc_RNA] = includeMe;
    FIP->IncludeFeature [FEATDEF_precursor_RNA] = includeMe;
    FIP->IncludeFeature [FEATDEF_snoRNA] = includeMe;
  } else if (newFeatdef == FEATDEF_ANY_PROT) {
    FIP->IncludeFeature [FEATDEF_PROT] = includeMe;
    for (i = FEATDEF_preprotein; i <= FEATDEF_transit_peptide_aa; i++) {
      FIP->IncludeFeature [i] = includeMe;
    }
  } else if (newFeatdef == FEATDEF_IMP) {
    for (i = FEATDEF_allele; i <= FEATDEF_35_signal; i++) {
      FIP->IncludeFeature [i] = includeMe;
    }
  } else if (newFeatdef < FEATDEF_MAX) { 
    FIP->IncludeFeature [newFeatdef] = includeMe;
  }
}

NLM_EXTERN void AddFeatureToFilterItem (
  FilterItemPtr FIP,
  Uint1 newFeatdef,
  ViewerConfigsPtr VCP
)

{
  ChangeFeatureInFilterItem (FIP, newFeatdef, TRUE, VCP);
}

NLM_EXTERN void RemoveFeatureFromFilterItem (
  FilterItemPtr FIP,
  Uint1 newFeatdef,
  ViewerConfigsPtr VCP
)

{
  ChangeFeatureInFilterItem (FIP, newFeatdef, FALSE, VCP);
}

static void AddFilterItemToFilter (
 FilterItemPtr newFIP,
  FilterPtr parent,
  ViewerConfigsPtr VCP
)

{
  ValNodePtr  newVNP, lastVNP;

  for (lastVNP = parent->FilterItemList;
       lastVNP != NULL && lastVNP->next != NULL;
       lastVNP = lastVNP->next) continue;
  newVNP = ValNodeAdd (&parent->FilterItemList);
  newVNP->data.ptrvalue = newFIP;
}

NLM_EXTERN FilterItemPtr CreateNewFilterItemInFilter (
  CharPtr name,
  FilterPtr parent,
  ViewerConfigsPtr VCP
)

{
  FilterItemPtr  FIP;

  FIP = MemNew (sizeof (FilterItem));
  if (FIP == NULL) return FIP;
  FIP->GroupLabel = StringSaveNoNull (name);
  AddFilterItemToFilter (FIP, parent, VCP);
  return FIP;
}

static FilterItemPtr ParseFilterItem (
  CharPtr filterItemName,
  ViewerConfigsPtr VCP
)

{
  Char           sectionName [128];
  Char           featureNum [128];
  Char           inputbuf [128];
  Uint4          featdeftype;
  Uint4          featureCount = 0;
  FilterItemPtr  FIP;
  unsigned       i; /* to match scanf("%ud") */

  FIP = MemNew (sizeof (FilterItem));
  if (FIP == NULL) return FIP;
  sprintf (sectionName, "filters.%s", filterItemName);
  GetAppParam (config_filename, sectionName, "layout", "inherit", inputbuf, sizeof (inputbuf));
  i = StringIndexInStringList (inputbuf, LayoutStrings);
  if (i < DIM (LayoutStrings) && i >= 0) {
    FIP->LayoutChoice = LayoutValues [i];
  } else {
    FIP->LayoutChoice = Layout_Inherit;
  }
  FIP->IntraRowPaddingPixels = 1;
  if (GetAppParam (config_filename, sectionName, "rowpadding", NULL, inputbuf, sizeof (inputbuf))) {
    if (inputbuf != NULL) {
      sscanf (inputbuf, "%ud", &i);
      i = MIN (i, 100);
      FIP->IntraRowPaddingPixels = i;
    }
  }
  GetAppParam (config_filename, sectionName, "label", NULL, inputbuf, sizeof (inputbuf));
  FIP->GroupLabel = StringSaveNoNull (inputbuf);
  if (GetAppParam (config_filename, sectionName, "groupbox", NULL, inputbuf, sizeof (inputbuf))) {
    i = StringIndexInStringList (inputbuf, BoolStrings);
    if (i < DIM (BoolValues) && i >=0) {
      FIP->DrawItemRect = BoolValues [i];
    } else {
      FIP->DrawItemRect = FALSE;
    }
  }
  if (FIP->DrawItemRect) {
    if (GetAppParam (config_filename, sectionName, "boxcolor", NULL, inputbuf, sizeof (inputbuf))) {
      ParseColor (inputbuf, FIP->RectColor);
    }
    if (GetAppParam (config_filename, sectionName, "fillbox", NULL, inputbuf, sizeof (inputbuf))) {
      i = StringIndexInStringList (inputbuf, BoolStrings);
      if (i < DIM (BoolValues) && i >= 0) {
	FIP->FillItemRect = BoolValues[i];
      } else {
	FIP->FillItemRect = FALSE;
      }
    }
  }
  for (i = 1; i < FEATDEF_MAX; i++) {
    sprintf (featureNum, "feature%d", (unsigned) i);
    if (GetAppParam (config_filename, sectionName, featureNum, NULL, inputbuf, sizeof (inputbuf))) {
      featdeftype = FindFeatDefTypeFromKey (inputbuf);
      if (featdeftype == FEATDEF_BAD) {
	/* insert special-case checks for types of features not found by FindFeatDefTypeFromKey () here */
	if (StringICmp (inputbuf, "everything") == 0
	    || StringICmp (inputbuf, "all") == 0) {
	  featdeftype = FEATDEF_ANY;
	} else if (StringICmp (inputbuf, "rna") == 0) {
	  featdeftype = FEATDEF_ANY_RNA;
	} else if (StringICmp (inputbuf, "prot") == 0) {
	  featdeftype = FEATDEF_ANY_PROT;
	} else continue; /* failed to find a match */
      }
      AddFeatureToFilterItem (FIP, featdeftype, VCP);
      featureCount++;
    }
  }
  if (featureCount == 0) {
    MemFree (FIP);
    return NULL;
  }
  return FIP;
}

static FilterPtr ParseFilter (
  CharPtr filterNameInFile,
  ViewerConfigsPtr VCP
)

{

  FilterPtr      FP;
  FilterItemPtr  FIP;
  Uint1          i;
  Uint1          filterItemCount = 0;
  Char           inputBuffer [128];     /* for input *from* GetAppParam */
  Char           outputBuffer [128];    /* paramater *to* GetAppParam */
  Char           sectionName [128];

  if (filterNameInFile == NULL) return NULL;
  sprintf (sectionName, "%s", filterNameInFile);
  /* require all styles to have a name, since high-level interface uses the name to identify Filters */
  if (! GetAppParam (config_filename, sectionName, "name", NULL, inputBuffer, sizeof (inputBuffer))) return NULL;
  FP = CreateFilter (inputBuffer, VCP); /* Createfilter will check for duplucate names */
  if (FP == NULL) return FP;
  GetAppParam (config_filename, sectionName, "defaultlayout", NULL, inputBuffer, sizeof (inputBuffer));
  i = StringIndexInStringList (inputBuffer, LayoutStrings);
  if (i < DIM (LayoutValues) && i >= 0) {
    FP->DefaultLayout = LayoutValues [i];
  } else {
    FP->DefaultLayout = Layout_Inherit;
  }

  for (i = 1; i < FEATDEF_MAX; i++) {
    sprintf (outputBuffer, "%s%d", "filteritem", (unsigned) i);
    if (GetAppParam (config_filename, sectionName, outputBuffer, NULL, inputBuffer, sizeof (inputBuffer))) {
      FIP = ParseFilterItem (inputBuffer, VCP);
      AddFilterItemToFilter (FIP, FP, VCP);
      filterItemCount++;
    }
  }
  if (filterItemCount == 0) {
    DestroyFilter (FP, VCP);
    return NULL;
  }
  return FP;
}

static ViewerConfigsPtr newGraphicViewer_ConfigFileParse_Global = NULL;
/* if this will be used by multiple threads in a multi-threaded application, there needs to be a lock around writing this */

NLM_EXTERN ViewerConfigsPtr GetGraphicConfigParseResults (
  void
)

{
  Uint1             AppearanceCount;
  Uint1             FilterCount;
  Uint1             i;
  void PNTR PNTR    ptr2;
  ViewerConfigsPtr  VCP;
  ValNodePtr        nameVNP;
  ValNodePtr        VNP;

  if (newGraphicViewer_ConfigFileParse_Global != NULL)
    return newGraphicViewer_ConfigFileParse_Global;

  VCP = MemNew (sizeof (ViewerConfigs));
  if (VCP == NULL) return NULL;

  if (ParseConfigFile (VCP) == 0) return NULL;

  AppearanceCount = VCP->AppearanceCount;
  FilterCount = VCP->FilterCount;
  i = (AppearanceCount + FilterCount) * 2 + 2; /* total number of pointers needed (the extra 2 are NULL's to terminate the name lists)*/
  ptr2 = MemNew (i * sizeof (void PNTR));
  if (ptr2 == NULL) {
    MemFree (VCP);
    return NULL;
  }
  VCP->AppearanceArray = (AppearancePtr PNTR) ptr2;
  ptr2 += AppearanceCount;
  VCP->AppearanceNameArray = (CharPtr PNTR) ptr2;
  ptr2 += AppearanceCount + 1; /* add a NULL pointer to terminate the list */
  VCP->FilterArray = (FilterPtr PNTR) ptr2;
  ptr2 += FilterCount;
  VCP->FilterNameArray = (CharPtr PNTR) ptr2;
  VNP = VCP->AppearanceList;
  nameVNP = VCP->AppearanceNameList;
  for (i = 0; i < AppearanceCount; i++) {
    VCP->AppearanceArray[i] = VNP->data.ptrvalue;
    VCP->AppearanceNameArray[i] = nameVNP->data.ptrvalue;
    VNP = VNP->next;
    nameVNP = nameVNP->next;
  }
  
  VNP = VCP->FilterList;
  nameVNP = VCP->FilterNameList;
  for (i = 0; i < FilterCount; i++) {
    VCP->FilterArray[i] = VNP->data.ptrvalue;
    VCP->FilterNameArray[i] = nameVNP->data.ptrvalue;
    VNP = VNP->next;
    nameVNP = nameVNP->next;
  }
  VCP->ArraysPopulated = TRUE;
  newGraphicViewer_ConfigFileParse_Global = VCP;
  return VCP;
}

/* returns count of objects successfully parsed -- so 0 on failure*/
NLM_EXTERN Uint2 ParseConfigFile (
  ViewerConfigsPtr VCP
)

{
  Char     tagBuffer [32];
  Char     nameBuffer [128];
  Uint1    i;
  Uint2    fCount = 0, aCount = 0;
  VoidPtr  tempPtr;

  for (i = 1; i < 110; i++) {   /* do filters first */
    if (i < 10) {
      sprintf (tagBuffer, "group0%d", (unsigned) i);
    } else {
      sprintf (tagBuffer, "group%d", (unsigned) i - 9);
    }
    if (GetAppParam (config_filename, "filters", tagBuffer, NULL, nameBuffer, sizeof (nameBuffer))) {
      tempPtr = ParseFilter (nameBuffer, VCP);
      if (tempPtr == NULL) continue;
      fCount++;
    }
  }
  for (i = 1; i < 110; i++) {
    if (i < 10) {
      sprintf (tagBuffer, "appearance0%d", (unsigned) i);
    } else {
      sprintf (tagBuffer, "appearance%d", (unsigned) i - 9);
    }
    if (GetAppParam (config_filename, "appearances", tagBuffer, NULL, nameBuffer, sizeof (nameBuffer))) {
      tempPtr = ParseAppearance (nameBuffer, VCP);
      if (tempPtr == NULL) continue;
      aCount++;
    }
  }
  return (aCount + fCount);
}

NLM_EXTERN FilterPtr DestroyFilter (
  FilterPtr FP,
  ViewerConfigsPtr VCP
)

{
  FilterItemPtr  FIP;
  ValNodePtr     VNP;
  Uint1          i;

  if (FP == NULL || VCP == NULL) {
    return NULL;
  }
  for (VNP = FP->FilterItemList; VNP; VNP = VNP->next) {        /* free all filterItems, and their labels */
    FIP = (FilterItemPtr) VNP->data.ptrvalue;
    if (FIP == NULL) {
      continue;
    }
    MemFree (FIP->GroupLabel);
    MemFree (FIP);
  }
  for (VNP = VCP->FilterList; VNP != NULL; VNP = VNP->next) {
    if (VNP->data.ptrvalue == FP) {
      i = VNP->choice;
      VNP = ValNodeExtract (&VCP->FilterList, i);
      break;
    }
  }
  if (VNP != NULL) {
    MemFree (VNP);
    VNP = ValNodeExtract (&VCP->FilterNameList, i);
    MemFree (VNP->data.ptrvalue);
    MemFree (VNP);
  }
  MemFree (FP);
  return NULL;
}

NLM_EXTERN void AddAppearanceItemToAppearance (
  AppearanceItemPtr AIP,
  AppearancePtr AP,
  Uint1 newFeatdef,
  ViewerConfigsPtr VCP
)

{
  Uint1       i;
  ValNodePtr  VNP;

  if (AIP == NULL || AP == NULL || VCP == NULL) return;
  if (newFeatdef == FEATDEF_ANY) {
    for (i = 0; i < FEATDEF_MAX; i++) {
      AP->FeaturesAppearanceItem [i] = AIP;
    }
  } else if (newFeatdef == FEATDEF_ANY_RNA) {
    for (i = FEATDEF_preRNA; i <= FEATDEF_otherRNA; i++) {
      AP->FeaturesAppearanceItem [i] = AIP;
    }
    AP->FeaturesAppearanceItem [FEATDEF_misc_RNA] = AIP;
    AP->FeaturesAppearanceItem [FEATDEF_precursor_RNA] = AIP;
    AP->FeaturesAppearanceItem [FEATDEF_snoRNA] = AIP;
  } else if (newFeatdef == FEATDEF_ANY_PROT) {
    AP->FeaturesAppearanceItem [FEATDEF_PROT] = AIP;
    for (i = FEATDEF_preprotein; i <= FEATDEF_transit_peptide_aa; i++) {
      AP->FeaturesAppearanceItem [i] = AIP;
    }
  } else if (newFeatdef == FEATDEF_IMP) {
    for (i = FEATDEF_allele; i <= FEATDEF_35_signal; i++) {
      AP->FeaturesAppearanceItem [i] = AIP;
    }
  } else if (newFeatdef < FEATDEF_MAX) {
    AP->FeaturesAppearanceItem [newFeatdef] = AIP;
  } else return;
  for (VNP = AP->AppearanceItemList; VNP != NULL && VNP->data.ptrvalue != AIP; VNP = VNP->next) continue;
  if (! (VNP != NULL && VNP->data.ptrvalue == AIP)) {    /* AIP was not previously in the AppearanceItemList */
    ValNodeAddPointer (&AP->AppearanceItemList, 0, AIP);
  }
}

NLM_EXTERN AppearancePtr DestroyAppearance (
  AppearancePtr AP,
  ViewerConfigsPtr VCP
)

{
  Uint1       i;
  ValNodePtr  VNP;

  if (AP == NULL || VCP == NULL) return NULL;
  if (AP->AppearanceItemList != NULL) {
    ValNodeFreeData (AP->AppearanceItemList);
  }
  for (VNP = VCP->AppearanceList; VNP != NULL; VNP = VNP->next) {
    if (VNP->data.ptrvalue == AP) {
      i = VNP->choice;
      VNP = ValNodeExtract (&VCP->AppearanceList, i);
      break;
    }
  }
  if (VNP != NULL) {
    MemFree (VNP);
    VNP = ValNodeExtract (&VCP->AppearanceNameList, i);
    MemFree (VNP);
  }
  MemFree (AP);
  return NULL;
}

static void getDim_render_with_line (
  RenderInputPtr RIP,
  Int4Ptr Start,
  Int4Ptr Stop,
  Uint2Ptr height,
  ViewerContextPtr vContext
)

{
  RelevantFeatureItemPtr RFIP;
  RFIP = RIP->RFIP;

  *Start = RFIP->Left;
  *Stop = RFIP->Right;
  *height = 1;
}

static void render_with_line (
  RenderInputPtr RIP,
  ViewerContextPtr vContext
)

{
  Uint4      StartY;
  PrimitivE  thisPrim;
  RelevantFeatureItemPtr RFIP;

  RFIP = RIP->RFIP;
  StartY = RIP->yStart - (RIP->rowHeight / 2);
  thisPrim = AddLine (RIP->drawSeg, RFIP->Left, StartY, RFIP->Right, StartY, 0, 0);
  SetPrimitiveIDs (thisPrim, RFIP->entityID, RFIP->itemID, OBJ_SEQFEAT, 0);
}

static void getDim_render_with_capped_line (
  RenderInputPtr RIP,
  Int4Ptr Start,
  Int4Ptr Stop,
  Uint2Ptr height,
  ViewerContextPtr vContext
)

{  
  RelevantFeatureItemPtr RFIP;
  AppearanceItemPtr AIP;

  RFIP = RIP->RFIP;
  AIP = RIP->AIP;
  *Start = RFIP->Left;
  *Stop = RFIP->Right;
  *height = AIP->Height;
}

static void render_with_capped_line (
  RenderInputPtr RIP,
  ViewerContextPtr vContext
)

{
  PrimitivE               thisPrim;
  AppearanceItemPtr       AIP;
  RelevantFeatureItemPtr  RFIP;

  render_with_line (RIP, vContext);
  RFIP = RIP->RFIP;
  AIP = RIP->AIP;
  thisPrim = AddLine (RIP->drawSeg, RFIP->Left, RIP->yStart, RFIP->Left, RIP->yStart - AIP->Height, 0, 0);
  SetPrimitiveIDs (thisPrim, RFIP->entityID, RFIP->itemID, OBJ_SEQFEAT, 0);
  thisPrim = AddLine (RIP->drawSeg, RFIP->Right, RIP->yStart, RFIP->Right, RIP->yStart - AIP->Height, 0, 0);
  SetPrimitiveIDs (thisPrim, RFIP->entityID, RFIP->itemID, OBJ_SEQFEAT, 0);
}

static void getDim_render_with_box (
  RenderInputPtr RIP,
  Int4Ptr Start,
  Int4Ptr Stop,
  Uint2Ptr height,
  ViewerContextPtr vContext
)

{
  RelevantFeatureItemPtr RFIP;
  AppearanceItemPtr AIP;

  RFIP = RIP->RFIP;
  AIP = RIP->AIP;
  *Start = RFIP->Left;
  *Stop = RFIP->Right;
  *height = AIP->Height;
}

static void render_with_box (
  RenderInputPtr RIP,
  ViewerContextPtr vContext
)

{
  Uint4                   StartY;
  Uint2                   pieceIValStart, lastPieceIVal = 0;
  PrimitivE               thisPrim;
  Uint2                   i;
  AppearanceItemPtr       AIP;
  RelevantFeatureItemPtr  RFIP;

  RFIP = RIP->RFIP;
  AIP = RIP->AIP;
  StartY = RIP->yStart - (RIP->rowHeight / 2);
  if (RFIP->numivals == 1) {
    thisPrim = AddRectangle (RIP->drawSeg, RFIP->Left, StartY - AIP->Height / 2, RFIP->Right, StartY + AIP->Height / 2, 0, TRUE, 0);
    SetPrimitiveIDs (thisPrim, RFIP->entityID, RFIP->itemID, OBJ_SEQFEAT, 0);
    return;
  } else {
    i = 0;
    while (i < RFIP->numivals) {
      /* collect a group of interval(s) which do not contain any pixels between them */
      pieceIValStart = i;
      /* this tests is the i-plus-1th feature is part of the smear, which goes from pieceIValStart to i, inclusive */
      while (i + 1 < RFIP->numivals && 
             RFIP->ivals [2 * (i + 1)] - RFIP->ivals [(2 * i) + 1] < 3 * vContext->viewScale) {
        i++;
      }

      /* draw the segment and the gap -- drawing the gap first, so that it is overdrawn by the segment */
      if (i + 1 < RFIP->numivals) { /* a gap is present if there are more ivals to consider after i*/
        thisPrim = AddLine (RIP->drawSeg, RFIP->ivals [2 * i + 1] + vContext->viewScale / 2, StartY, RFIP->ivals [2 * (i + 1)] - vContext->viewScale / 2, StartY, 0, 0);
        SetPrimitiveIDs (thisPrim, RFIP->entityID, RFIP->itemID, OBJ_SEQFEAT, 0);
      }
      thisPrim = AddRectangle (RIP->drawSeg, RFIP->ivals [2 * pieceIValStart], StartY - AIP->Height / 2, RFIP->ivals [2 * i + 1], StartY + AIP->Height / 2, 0, TRUE, 0);
      SetPrimitiveIDs (thisPrim, RFIP->entityID, RFIP->itemID, OBJ_SEQFEAT, 0);
      i++;
    }
  }
}

static const RenderClass RenderAlgorithmTable [] = {
  {render_with_line, getDim_render_with_line}, /* do_not_render */
  {render_with_line, getDim_render_with_line}, /* Render_Line */
  {render_with_capped_line, getDim_render_with_capped_line}, /* Render_CappedLine */
  {render_with_box, getDim_render_with_box},   /* Render_Box */
  {render_with_box, getDim_render_with_box},   /* Render_OutlineBox */
  /* these do not exist right now */
  {render_with_line, getDim_render_with_line},
  {render_with_box, getDim_render_with_box},
  {render_with_line, getDim_render_with_line},
  {render_with_line, getDim_render_with_line},
  {render_with_line, getDim_render_with_line},
  {render_with_line, getDim_render_with_line},
  {render_with_line, getDim_render_with_line}
};

static void DrawFeatureAndLabel (
  RenderInputPtr RIP,
  ViewerContextPtr vContext
)

{
  AppearanceItemPtr       AIP;
  RelevantFeatureItemPtr  RFIP;
  CharPtr                 label;
  Char                    tempStringBuffer [256];
  Uint1                   stringFlags = 0;
  Uint4                   textWidthBP;
  Int4                    textStartX;
  Int2                    textStartY;
  Uint1                   labelAlign;
  PrimitivE               thisPrim;

  RFIP = RIP->RFIP;
  AIP = RIP->AIP;

  /*  RIP->drawSeg = CreateSegment (RIP->drawSeg, 0, 0);*/
  /* Place each feature in its own segment.  This is not necessary for simple features
     but perhaps the inefficiency is acceptable because it simplifies the algorithm. */
  (*RenderAlgorithmTable [AIP->RenderChoice].RenderFunc) (RIP, vContext);
  if (AIP->LabelLoc == LabelNone) return;
  if (vContext->AppPtr->FeaturesAppearanceItem [RFIP->featdeftype]->AddDescToLabel) {
    stringFlags |= 1;
  }
  if (vContext->AppPtr->FeaturesAppearanceItem [RFIP->featdeftype]->AddTypeToLabel) {
    stringFlags |= 2;
  }
  label = NULL;
  switch (stringFlags) {
    case 0:                    /* no label */
      return;
    case 1:                    /*comment but not type */
      label = RFIP->ContentLabel;
      break;
    case 2:                    /*only type */
      label = FindKeyFromFeatDefType (RFIP->featdeftype, FALSE);
      break;
    case 3:                    /*add both */
      if (StringCmp (FindKeyFromFeatDefType (RFIP->featdeftype, FALSE), RFIP->ContentLabel) != 0) {
	sprintf (tempStringBuffer, "%s: %s", FindKeyFromFeatDefType (RFIP->featdeftype, FALSE), RFIP->ContentLabel);
	label = tempStringBuffer;
      } else {
	label = RFIP->ContentLabel;
      }
      break;
  }
  if (StringHasNoText (label)) return;
  SelectFont (AIP->LabelFont);
  textWidthBP = StringWidth (label) * vContext->viewScale;
  switch (AIP->LabelLoc) {
    case LabelInside:
      if (textWidthBP + 2 * vContext->viewScale >= (RFIP->Right - RFIP->Left)) {
	  /*!!! add string-chopper here, for now just don't render it */
	return;
      }
      textStartX = (RFIP->Left + RFIP->Right) / 2;
      textStartY = RIP->yStart - RIP->rowHeight / 2;
      labelAlign = MIDDLE_CENTER;
      break;
    case LabelAbove:
      textStartX = (RFIP->Left + RFIP->Right) / 2;
      textStartY = RIP->yStart - (RIP->rowHeight / 2) + RFIP->decorationHeight;
      labelAlign = LOWER_CENTER;
      break;
    case LabelBelow:
      textStartY = RIP->yStart - (RIP->rowHeight / 2) - RFIP->decorationHeight;
      textStartX = (RFIP->Left + RFIP->Right) / 2;
      labelAlign = UPPER_CENTER;
      break;
    case LabelLeft:
      textStartX = (RFIP->Left);
      textStartY = (RIP->yStart) - RIP->rowHeight / 2;
      labelAlign = MIDDLE_LEFT;
      break;
    case LabelRight:
      textStartX = (RFIP->Right);
      textStartY = (RIP->yStart) - RIP->rowHeight / 2;
      labelAlign = MIDDLE_RIGHT;
      break;
    default:
      return;
  }
  thisPrim = AddTextLabel (RIP->labelSeg, textStartX, textStartY, label, AIP->LabelFont, 2, labelAlign, 0);
  SetPrimitiveIDs (thisPrim, RFIP->entityID, RFIP->itemID, OBJ_SEQFEAT, 0);
}

static void GetFeatureAndDecorationDimensions (
  RenderInputPtr RIP,
  ViewerContextPtr vContext
)

{
  AppearanceItemPtr       AIP;
  RelevantFeatureItemPtr  RFIP;
  Int4                    Start, Stop;
  Uint2                   Height;
  Int2                    lineHeight;
  Int4                    textStartX;
  Uint4                   textWidthBP;
  CharPtr                 label = NULL;
  Char                    tempStringBuffer [256];
  Uint1                   stringFlags = 0;

  RFIP = RIP->RFIP;
  AIP = RIP->AIP;
  (*RenderAlgorithmTable [AIP->RenderChoice].GetDimensions) (RIP, &Start, &Stop, &Height, vContext);
  RFIP->Height = Height;
  RFIP->decorationHeight = Height;
  RFIP->decorationLeft = RFIP->Left;
  RFIP->decorationRight = RFIP->Right;
  if (AIP->LabelLoc != LabelNone) {
    if (vContext->AppPtr->FeaturesAppearanceItem [RFIP->featdeftype]->AddDescToLabel) {
      stringFlags |= 1;
    }
    if (vContext->AppPtr->FeaturesAppearanceItem [RFIP->featdeftype]->AddTypeToLabel) {
      stringFlags |= 2;
    }
    switch (stringFlags) {
      case 0:                  /* no label */
        break;
      case 1:                  /*comment but not type */
        label = RFIP->ContentLabel;
        break;
      case 2:                  /*only type */
        label = FindKeyFromFeatDefType (RFIP->featdeftype, FALSE);
        break;
      case 3:                  /*add both */
	if (StringCmp (FindKeyFromFeatDefType (RFIP->featdeftype, FALSE), RFIP->ContentLabel) != 0) {
	  sprintf (tempStringBuffer, "%s: %s", FindKeyFromFeatDefType (RFIP->featdeftype, FALSE), RFIP->ContentLabel);
	  label = tempStringBuffer;
	} else {
	  label = RFIP->ContentLabel;
	}
        break;
      default:
	return;
    }
  }
  if (! StringHasNoText (label)) {
    SelectFont (AIP->LabelFont);
    textWidthBP = StringWidth (label) * vContext->viewScale;
    lineHeight = LineHeight ();
    switch (AIP->LabelLoc) {
      case LabelInside:
	if (Height < lineHeight) {
	  Height = lineHeight;
	}
	break;
      case LabelAbove:
	textStartX = (Start + Stop) / 2;
	Start = MIN (Start, (signed)(textStartX - textWidthBP / 2));
	Stop = MAX (Stop, textStartX + textWidthBP / 2);
	Height += lineHeight + 3;
	break;
      case LabelBelow:
	textStartX = (Start + Stop) / 2;
	Start = MIN (Start, (signed)(textStartX - textWidthBP / 2));
	Stop = MAX (Stop, textStartX + textWidthBP / 2);
	Height += lineHeight + 3;
	break;
      case LabelLeft:
	Start -= textWidthBP;
	Height = MAX (Height + lineHeight, Height);
	break;
      case LabelRight:
	Stop = RFIP->Right + textWidthBP;
	Height = MAX (Height + lineHeight, Height);
	break;
      default:
	return;
    }
  }
  RFIP->decorationLeft = Start;
  RFIP->decorationRight = Stop;
  RFIP->decorationHeight = Height;
}


static Boolean BuildRenderInputFromRFIP (
  RenderInputPtr target,
  RelevantFeatureItemPtr RFIP,
  FilterProcessStatePtr FPSP,
  ViewerContextPtr vContext
)

{
  AppearancePtr  AppPtr;
  FilterItemPtr  currentFIP;

  if (target == NULL || RFIP == NULL) return FALSE;
  target->RFIP = RFIP;
  target->labelSeg = FPSP->labelSegs [RFIP->featdeftype];
  target->drawSeg = FPSP->drawSegs [RFIP->featdeftype];
  AppPtr = vContext->AppPtr;
  currentFIP = FPSP->currentFIP;
  target->AIP = AppPtr->FeaturesAppearanceItem [RFIP->featdeftype];
  GetFeatureAndDecorationDimensions (target, vContext);
  target->rowHeight = MAX (RFIP->Height, RFIP->decorationHeight) + currentFIP->IntraRowPaddingPixels;
  return TRUE;
}

static Boolean GetAndCountFeatures (
  ViewerContextPtr vContext
)

{
  SeqFeatPtr              sfp;
  SeqMgrFeatContext       fContext;
  RelevantFeatureItemPtr  rFeats;
  ValNodePtr              sapList = NULL, VNP, VNPtail;
  Uint2                   i = 0;

  if (vContext == NULL) return FALSE;
  vContext->sapCount = 0;
  vContext->featureCount = 0;
  vContext->featVNP = NULL;

  rFeats = MemNew (RELEVANT_FEATS_PER_CHUNK * sizeof (RelevantFeatureItem));
  if (rFeats == NULL) return FALSE;
  ValNodeAddPointer (&vContext->featVNP, 0, rFeats);
  VNPtail = vContext->featVNP;
  sfp = SeqMgrGetNextFeature (vContext->BSP, NULL, 0, 0, &fContext);
  while (sfp != NULL) {
    vContext->featureCount++;

    rFeats [i].Left = fContext.left;
    rFeats [i].Right = fContext.right;
    rFeats [i].StartPartial = fContext.partialL;
    rFeats [i].StopPartial = fContext.partialR;
    rFeats [i].ContentLabel = fContext.label;
    rFeats [i].featdeftype = fContext.featdeftype;
    rFeats [i].entityID = fContext.entityID;
    rFeats [i].itemID = fContext.itemID;
    rFeats [i].numivals = fContext.numivals;
    rFeats [i].ivals = fContext.ivals;
    rFeats [i].StartOnLeft = (fContext.left <= fContext.right) ? TRUE : FALSE;
    /* look at fContext.sap -- if unique, add to sapList */
    for (VNP = sapList; VNP != NULL; VNP = VNP->next) {
      if (VNP->data.ptrvalue == fContext.sap) break;
    }
    if (VNP == NULL) {          /* fContext.sap was not found */
      vContext->sapCount++;
      ValNodeAddPointer (&sapList, 0, fContext.sap);
    }
    i++;
    if (i >= RELEVANT_FEATS_PER_CHUNK) {
      i = 0;
      rFeats = MemNew (RELEVANT_FEATS_PER_CHUNK * sizeof (RelevantFeatureItem));
      VNPtail = ValNodeNew (VNPtail);
      if (rFeats == NULL || VNPtail == NULL) {
        ValNodeFreeData (vContext->featVNP);
        MemFree (rFeats);
        return FALSE;
      }
      VNPtail->data.ptrvalue = rFeats;
    }
    sfp = SeqMgrGetNextFeature (vContext->BSP, sfp, 0, 0, &fContext);
  }
  vContext->sapList = MemNew (vContext->sapCount * sizeof (SeqAnnotPtr));
  if (vContext->sapList == NULL) {
    MemFree (rFeats);
    ValNodeFree (sapList);
    return FALSE;
  }
  for (i = 0, VNP = sapList; VNP != NULL && i < vContext->sapCount; VNP = VNP->next, i++) {
    vContext->sapList[i] = VNP->data.ptrvalue;
  }
  ValNodeFree (sapList);
  return TRUE;
}

NLM_EXTERN RelevantFeaturesPtr CollectFeatures (
  BioseqPtr bsp
)

{
  RelevantFeaturesPtr  RFP;
  ViewerContext        VC;

  RFP = MemNew (sizeof (RelevantFeatures));
  if (RFP == NULL) return NULL;
  VC.BSP = bsp;
  if (! GetAndCountFeatures (&VC)) return NULL;
  RFP->featureCount = VC.featureCount;
  RFP->featVNP = VC.featVNP;
  RFP->sapCount = VC.sapCount;
  RFP->sapList = VC.sapList;
  return RFP;
} 

static Boolean EnsureFeatureHasSegment (
  FilterProcessStatePtr FPSP,
  Uint1 featdeftype,
  SegmenT parentSegment,
  ViewerContextPtr vContext
)

{
  AppearancePtr     AppPtr;
  AppearanceItemPtr AIP;

  if (parentSegment == NULL) {
    parentSegment = vContext->drawOnMe;
  }

  AppPtr = vContext->AppPtr;
  if (FPSP->drawSegs [featdeftype] == NULL) {
    FPSP->drawSegs [featdeftype] = CreateSegment (parentSegment, 0, 0);
    FPSP->labelSegs [featdeftype] = CreateSegment (parentSegment, 0, 0);
    /* cleaup needed if program is supposed to recover from this !!! */
    if (FPSP->drawSegs [featdeftype] == NULL || FPSP->labelSegs [featdeftype] == NULL) return FALSE; 
    AIP = AppPtr->FeaturesAppearanceItem [featdeftype];
    AddAttribute (FPSP->drawSegs [featdeftype],
                  COLOR_ATT | SHADING_ATT | STYLE_ATT | WIDTH_ATT,
                  AIP->Color, AIP->VibLinestyle, AIP->VibShading, 1, 0);
    AddAttribute (FPSP->labelSegs [featdeftype], COLOR_ATT, AppPtr->FeaturesAppearanceItem [featdeftype]->FontColor, 0, 0, 0, 0);
  }
  return TRUE;
}

static RelevantFeatureItemPtr GetNextRFIPinFilterItem (
  FilterProcessStatePtr FPSP,
  BoolPtr firstTime,
  ViewerContextPtr vContext
)

{
  AppearancePtr           AppPtr;
  RelevantFeatureItemPtr  RFIP;
  RenderInput             dummyRIP; /* used to get the dimensions of RFIP */
  ValNodePtr              currentRFIPblockVNP;

  /* called as an iterator by the rendering functions -- builds & returns the next feature (in an RFIP), or returns NULL if no more left in this Filter */
  if (FPSP == NULL || firstTime == NULL) return NULL;
  for (; FPSP->featureBlockOffset + FPSP->indexInBlock < vContext->featureCount; FPSP->indexInBlock++) {
    if (FPSP->indexInBlock >= RELEVANT_FEATS_PER_CHUNK) {
      FPSP->indexInBlock = 0;
      FPSP->featureBlockOffset += RELEVANT_FEATS_PER_CHUNK;
      FPSP->currentRFIPblockVNP = FPSP->currentRFIPblockVNP->next;
      if (FPSP->currentRFIPblockVNP == NULL) return NULL;
    }
    currentRFIPblockVNP = FPSP->currentRFIPblockVNP;
    RFIP = (RelevantFeatureItemPtr) (currentRFIPblockVNP->data.ptrvalue) + FPSP->indexInBlock;
    if (FPSP->featuresProcessed [FPSP->featureBlockOffset + FPSP->indexInBlock]
        || (! FPSP->currentFIP->IncludeFeature [RFIP->featdeftype])) continue;
    FPSP->featuresProcessed [FPSP->featureBlockOffset + FPSP->indexInBlock] = TRUE;
    break;
  }
  if (FPSP->featureBlockOffset + FPSP->indexInBlock >= vContext->featureCount) return NULL;
  dummyRIP.RFIP = RFIP;
  AppPtr = vContext->AppPtr;
  dummyRIP.AIP = AppPtr->FeaturesAppearanceItem [RFIP->featdeftype];
  GetFeatureAndDecorationDimensions (&dummyRIP, vContext);
  return RFIP;
}

/* !!! add smearing detection here */
static Boolean AddFeatureToRow (
  InternalRowPtr row,
  RelevantFeatureItemPtr RFIP
)

{
  ValNodePtr VNP;

  if (row == NULL || RFIP == NULL) return FALSE;
  VNP = MemNew (sizeof (ValNode));
  if (VNP == NULL) return FALSE;
  VNP->data.ptrvalue = RFIP;
  VNP->next = row->feats;
  row->feats = VNP;
  row->rowFeatureCount++;
  return TRUE;
}

static InternalRowPtr AddARow (
  InternalRowPtr sourceRow
)

{
  InternalRowPtr  IRP;

  if (sourceRow == NULL) return NULL;
  IRP = MemNew (sizeof (InternalRow));
  if (IRP == NULL) return NULL;
  sourceRow->next = IRP;
  return IRP;
}

static Uint2 SimpleDiagonalLayout (
  InternalRowPtr firstRow,
  FilterProcessStatePtr FPSP,
  ViewerContextPtr vContext
)

{
  Uint2                   rows = 0;
  InternalRowPtr          thisRow;
  RelevantFeatureItemPtr  RFIP;
  Boolean                 Initializing = TRUE;

  thisRow = firstRow;
  if (thisRow == NULL) return 0;
  thisRow->rowFeatureCount = 0;
  while ((RFIP = GetNextRFIPinFilterItem (FPSP, &Initializing, vContext)) != NULL) {
    AddFeatureToRow (thisRow, RFIP);
    thisRow = AddARow (thisRow);
    rows++;
  }
  return rows;
}

static Uint2 SimpleBubbleUpLayout (
  InternalRowPtr firstRow,
  FilterProcessStatePtr FPSP,
  ViewerContextPtr vContext
)

{
  Uint2                   rows = 1;
  Int4                    featureStart, rowMaxRight;
  RelevantFeatureItemPtr  RFIP;
  InternalRowPtr          thisRow, lastRow;
  Boolean                 Initializing = TRUE;

  /*
     This uses InternalRow.layoutData as an Int4, which stores the (x) offset of the last used pixel.
     so a feature starting at (internalRow.layoutData + 1) or greater can be placed in the same row
     without a collision.
   */

  /* add the 1st feature to the 1st row */
  RFIP = GetNextRFIPinFilterItem (FPSP, &Initializing, vContext);
  if (RFIP == NULL) return 0;
  AddFeatureToRow (firstRow, RFIP);
  lastRow = firstRow;
  firstRow->layoutData.intvalue = RFIP->decorationRight;

  while ((RFIP = GetNextRFIPinFilterItem (FPSP, &Initializing, vContext)) != NULL) {
    featureStart = RFIP->decorationLeft;
    for (thisRow = firstRow; thisRow != NULL; thisRow = thisRow->next) {
      rowMaxRight = thisRow->layoutData.intvalue;
      if (featureStart >= rowMaxRight) {
        AddFeatureToRow (thisRow, RFIP);
        rowMaxRight = MAX (rowMaxRight, RFIP->decorationRight);
        thisRow->layoutData.intvalue = rowMaxRight;
        RFIP = NULL;
        break;
      }
    }
    if (RFIP != NULL) {
      rows++;
      lastRow = AddARow (lastRow);
      AddFeatureToRow (lastRow, RFIP);
      rowMaxRight = RFIP->decorationRight;
      lastRow->layoutData.intvalue = rowMaxRight;
    }
  }
  return rows;
}

static Uint2 FeatdefPerRowLayout (
  InternalRowPtr firstRow,
  FilterProcessStatePtr FPSP,
  ViewerContextPtr vContext
)

{
  Uint1           rowFeatdeftype;
  Uint2           rows = 1;
  RelevantFeatureItemPtr RFIP;
  InternalRowPtr  thisRow, lastRow;
  Boolean         Initializing = TRUE;

  /*
     This layout uses InternalRow.layoutData as an Int4 (really an Int1), which is the featdeftype associated with this row
   */

  /* add the 1st feature to the 1st row */
  RFIP = GetNextRFIPinFilterItem (FPSP, &Initializing, vContext);
  if (RFIP == NULL)
    return 0;
  AddFeatureToRow (firstRow, RFIP);
  rowFeatdeftype = RFIP->featdeftype;
  firstRow->layoutData.intvalue = rowFeatdeftype;
  lastRow = firstRow;

  while ((RFIP = GetNextRFIPinFilterItem (FPSP, &Initializing, vContext)) != NULL) {
    for (thisRow = firstRow; thisRow != NULL; thisRow = thisRow->next) {
      rowFeatdeftype = thisRow->layoutData.intvalue;
      if (RFIP->featdeftype == thisRow->layoutData.intvalue) {
        AddFeatureToRow (thisRow, RFIP);
        RFIP = NULL;
        break;
      }
    }
    if (RFIP != NULL) {
      rows++;
      lastRow = AddARow (lastRow);
      AddFeatureToRow (lastRow, RFIP);
      rowFeatdeftype = RFIP->featdeftype;
      lastRow->layoutData.intvalue = rowFeatdeftype;
    }
  }
  return rows;
}


static const LayoutFunction LayoutAlgorithmTable [] = {
  SimpleBubbleUpLayout,         /* placeholder for Layout_Inherit */
  SimpleDiagonalLayout,         /* Layout_DiagonalOneFeatPerLine */
  SimpleDiagonalLayout,         /* Layout_DiagonalSawtooth (to be implemented) */
  FeatdefPerRowLayout,          /* Layout_FeatTypePerLine */
  FeatdefPerRowLayout,          /* Layout_FeatTypePerLineGroup (to be implemented) */
  SimpleDiagonalLayout,         /* Layout_AllInOneLine (to be implemented) */
  SimpleBubbleUpLayout,         /* Layout_PackUpward */
  SimpleBubbleUpLayout,         /* Layout_GroupCorrespondingFeats (to be implemented) */
  SimpleDiagonalLayout,         /* placeholder for future Layout_XXX */
  SimpleDiagonalLayout,
  SimpleDiagonalLayout,
};

/* non-leaf segments contain SEGMENT_TREE_BASE other segments */
#define SEGMENT_TREE_BASE 16
#define SEGMENT_TREE_BASE2 (SEGMENT_TREE_BASE * SEGMENT_TREE_BASE)
#define SEGMENT_TREE_BASE3 (SEGMENT_TREE_BASE * SEGMENT_TREE_BASE * SEGMENT_TREE_BASE)

static Uint2 ProcessRows (
  LayoutAlgorithm layoutC,
  FilterProcessStatePtr FPSP,
  ViewerContextPtr vContext
)

{
  Uint2                   I, J, K, lastI, lastJ, lastK; /* for keeping track of which segment in the tree we're in*/
  Uint1                   featdeftype;
  Int4                    featMidPoint;
  Uint2                   row, rowCount, feat, rowHeight, totalHeight;
  InternalRowPtr          firstRow, thisRow;
  ValNodePtr              VNP;
  RelevantFeatureItemPtr  RFIP;
  RenderInput             RI;           /* dummy used while finding height of each row */
  SegmenT                 SegmentTreeTop;
  SegmenT                 SegmentTreeMid;
  SegmenT                 SegmentTreeBot;
  Boolean                 SegmentChanged = TRUE;

  firstRow = MemNew (sizeof (InternalRow));
  if (firstRow == NULL) return 0;
  firstRow->feats = NULL;

  rowCount = (*LayoutAlgorithmTable [layoutC]) (firstRow, FPSP, vContext);

  thisRow = firstRow;
  totalHeight = 0;

  for (row = 0; row < rowCount; row++) {
    if (thisRow == NULL) continue;
    /*First iterate through features to find the row's height */
    VNP = thisRow->feats;
    rowHeight = 0;
    for (feat = 0; feat < thisRow->rowFeatureCount; feat++) {
      RFIP = VNP->data.ptrvalue;
      if (RFIP == NULL || (! BuildRenderInputFromRFIP (&RI, RFIP, FPSP, vContext))) return 0;
      rowHeight = MAX (rowHeight, RFIP->decorationHeight);
      if (VNP->next == NULL) break;
      VNP = VNP->next;
    }
    if (vContext->overrideLayout == Layout_FeatTypePerLine 
	|| vContext->overrideLayout == Layout_FeatTypePerLineGroup 
	|| (vContext->overrideLayout == Layout_Inherit &&
	(FPSP->currentFIP->LayoutChoice == Layout_FeatTypePerLine
        || FPSP->currentFIP->LayoutChoice == Layout_FeatTypePerLineGroup))) {

      Nlm_SetLargeFont ();
      featdeftype = thisRow->layoutData.intvalue;
      rowHeight = MAX (rowHeight, LineHeight () + 2);
      AddTextLabel (vContext->drawOnMe, 0, FPSP->ceiling - totalHeight - (rowHeight / 2), FindKeyFromFeatDefType (featdeftype, FALSE),
                    Nlm_largeFont, 0, MIDDLE_LEFT, 0);
    }

    /*Repeat, but actually draw them this time */
    VNP = thisRow->feats;
    RI.rowHeight = rowHeight;
    RI.yStart = FPSP->ceiling - totalHeight;
    for (feat = 0; feat < thisRow->rowFeatureCount; feat++) {
      if ((RFIP = VNP->data.ptrvalue) == NULL) return 0;
      featdeftype = RFIP->featdeftype;
      featMidPoint = (RFIP->Left + RFIP->Right) / 2;
      lastI = I;
      lastJ = J;
      lastK = K;
      I = (SEGMENT_TREE_BASE * featMidPoint) / vContext->seqLength;
      J = ((SEGMENT_TREE_BASE2 * featMidPoint) / vContext->seqLength) % SEGMENT_TREE_BASE;
      K = ((SEGMENT_TREE_BASE3 * featMidPoint) / vContext->seqLength) % SEGMENT_TREE_BASE;

      if (I != lastI || SegmentChanged) {
	SegmentTreeTop  = CreateSegment (vContext->drawOnMe, 0, 0);
	SegmentChanged = TRUE;
      }
      if (J != lastJ || SegmentChanged) {
	SegmentTreeMid = CreateSegment (SegmentTreeTop , 0, 0);
	SegmentChanged = TRUE;
      }
      if (K != lastK || SegmentChanged) {
	SegmentTreeBot = CreateSegment (SegmentTreeMid, 0, 0);
	SegmentChanged = TRUE;
      }
      if (SegmentChanged) {
	MemSet (FPSP->drawSegs, 0, sizeof (FPSP->drawSegs));
	MemSet (FPSP->labelSegs, 0, sizeof (FPSP->labelSegs));
      }
      if (! EnsureFeatureHasSegment (FPSP, featdeftype, SegmentTreeBot, vContext)) return 0;
      if (! BuildRenderInputFromRFIP (&RI, RFIP, FPSP, vContext)) return 0;
      
      DrawFeatureAndLabel (&RI, vContext);
      if (VNP->next == NULL) break;
      VNP = VNP->next;
    }
    totalHeight += rowHeight + FPSP->currentFIP->IntraRowPaddingPixels;
    if (thisRow->next == NULL) return totalHeight;
    thisRow = thisRow->next;
  }
  totalHeight += rowHeight + FPSP->currentFIP->IntraRowPaddingPixels;
  return totalHeight;
}

static Boolean FilterAndLayout (
  ViewerContextPtr vContext
)

{
  FilterProcessState  FPS;
  FilterItemPtr       FIP;
  LayoutAlgorithm     layoutC;
  Int2                height;
  SegmenT             filterSeg;

  if (vContext == NULL) return FALSE;
  MemSet (&FPS, 0, sizeof (FilterProcessState));
  FPS.ceiling = -20;
  FPS.featuresProcessed = MemNew (vContext->featureCount * sizeof (Boolean));
  if (FPS.featuresProcessed == NULL) return FALSE;
  MemSet (FPS.featuresProcessed, 0, vContext->featureCount * sizeof (Boolean));
  for (FPS.currentFilterVNP = vContext->FltPtr->FilterItemList;
       FPS.currentFilterVNP != NULL && FPS.currentFilterVNP->data.ptrvalue != NULL;
       FPS.currentFilterVNP = FPS.currentFilterVNP->next) {
    
    filterSeg = CreateSegment (vContext->topLevelSeg, 0, 0);
    MemSet (FPS.labelSegs, 0, sizeof (FPS.labelSegs));
    MemSet (FPS.drawSegs, 0, sizeof (FPS.drawSegs));
    vContext->drawOnMe = filterSeg;
    
    FIP = (FilterItemPtr) FPS.currentFilterVNP->data.ptrvalue;
    if (FIP->LabelLocation == LabelOnTop) {
      if (StringHasNoText (FIP->GroupLabel)) break;
      Nlm_SetLargeFont ();
      AddTextLabel (filterSeg, vContext->seqLength / 2, FPS.ceiling, 
		    FIP->GroupLabel, Nlm_largeFont, 1, UPPER_CENTER, 0);
      FPS.ceiling -= LineHeight () - 1;
    }
    if (FIP->DrawItemRect) {
      AddAttribute (filterSeg, COLOR_ATT, FIP->RectColor, 0, 0, 0, 0);
      AddSegRect (filterSeg, FIP->FillItemRect, 0);
      FPS.ceiling -= 6;
    }

    layoutC = (vContext->overrideLayout) ? vContext->overrideLayout : FIP->LayoutChoice;
    FPS.currentFIP = FIP;
    FPS.featureBlockOffset = 0;
    FPS.indexInBlock = 0;
    FPS.currentRFIPblockVNP = vContext->featVNP;

    height = ProcessRows (layoutC, &FPS, vContext);
    
    switch (FIP->LabelLocation) {
      default: break;
      case LabelOnTop: break;
      case LabelOnSide:
	Nlm_SetLargeFont ();
	AddTextLabel (filterSeg, 0, FPS.ceiling - height / 2, FIP->GroupLabel,
		      Nlm_largeFont, 1, MIDDLE_RIGHT, 0);
	break;
      case LabelOnBottom:
	Nlm_SetLargeFont ();
	AddTextLabel (filterSeg, vContext->seqLength / 2 , FPS.ceiling - height, FIP->GroupLabel,
		      Nlm_largeFont, 1, LOWER_CENTER, 0);
	height += LineHeight () + 1;
	break;
    }
    if (FIP->DrawItemRect) {
      height +=6;
    }
    FPS.ceiling -= height;
  }
  return TRUE;
}

NLM_EXTERN RelevantFeaturesPtr FreeCollectedFeatures (
  RelevantFeaturesPtr RFP
)

{
  if (RFP == NULL) return NULL;
  if (RFP->sapList) {
    MemFree (RFP->sapList);
  }
  if (RFP->featVNP != NULL) {
    ValNodeFreeData (RFP->featVNP);
  }
  MemFree (RFP);
  return NULL;
}    
    
NLM_EXTERN SegmenT CreateGraphicViewInternal (
  BioseqPtr bsp,
  RelevantFeaturesPtr feats,
  VoidPtr aligns, /* NULL for now, will become a RelevantGraphPtr*/
  VoidPtr graphs,
  Int4 scale,
  FilterPtr FP,
  AppearancePtr AP,
  LayoutAlgorithm overrideLayout
)

{
  ViewerContext  VC;

  if (feats == NULL || feats->featureCount == 0 || feats->featVNP == NULL || bsp == NULL) return NULL;
  if (FP == NULL) {
    return NULL; /* need static default */
  }
  if (AP == NULL) {
    return NULL; /* need static default */
  }
  VC.viewScale = scale;
  VC.sapList = feats->sapList;
  VC.sapCount = feats->sapCount;
  VC.drawOnMe = VC.topLevelSeg = CreatePicture ();
  VC.featureCount = feats->featureCount;
  VC.featVNP = feats->featVNP;
  VC.AppPtr = AP;
  VC.FltPtr = FP;
  VC.overrideLayout = overrideLayout;
  VC.seqLength = bsp->length;
  FilterAndLayout (&VC);
  return VC.topLevelSeg;
}
  
/* returns the 1st segment in a linked list. caller must deallocate it */
NLM_EXTERN SegmenT CreateGraphicViewFromBsp (
  BioseqPtr bsp,
  Int4 scale,
  FilterPtr FP,
  AppearancePtr AP,
  LayoutAlgorithm overrideLayout
)

{
  RelevantFeaturesPtr  RFP;
  SegmenT              seg;

  if (bsp == NULL) return NULL;
  RFP = CollectFeatures (bsp);
  if (RFP == NULL) return NULL;
  seg = CreateGraphicViewInternal (bsp, RFP, NULL, NULL, scale, FP, AP, overrideLayout);
  FreeCollectedFeatures (RFP);
  return seg;
}

NLM_EXTERN SegmenT EasyGraphicView (
  BioseqPtr bsp,
  Int4 scale,
  CharPtr filterName,
  CharPtr appearanceName,
  CharPtr overrideLayoutName
)

{
  ViewerConfigsPtr  myVCP;  
  FilterPtr         FP;
  AppearancePtr     AP;
  LayoutAlgorithm   overrideLayout;
  Uint1             i;

  if (bsp == NULL) return NULL;
  myVCP =  GetGraphicConfigParseResults ();

  FP = FindFilterByName (filterName, myVCP);
  AP = FindAppearanceByName (appearanceName, myVCP);
  i = StringIndexInStringList (overrideLayoutName, NewGphLayoutNames);
  if (i < DIM(NewGphLayoutValues)) {
    overrideLayout = NewGphLayoutValues[i];
  } else {
    overrideLayout = Layout_Inherit;
  }
  return CreateGraphicViewFromBsp (bsp, scale, FP, AP, overrideLayout);
}

static Uint2 GetAppearanceCount (void)

{
  ViewerConfigsPtr VCP;
  VCP = GetGraphicConfigParseResults ();
  if (VCP == NULL) return 0;
  return VCP->AppearanceCount;
}

static Uint2 GetFilterCount (void)

{
  ViewerConfigsPtr VCP;
  VCP = GetGraphicConfigParseResults ();
  if (VCP == NULL) return 0;
  return VCP->FilterCount;
}

static Uint2 GetLayoutCount (void)

{
  return DIM (NewGphLayoutNames);
}

NLM_EXTERN CharPtr PNTR GetAppearanceNameList (void)

{
  ViewerConfigsPtr VCP;
  VCP = GetGraphicConfigParseResults ();
  if (VCP == NULL) return NULL;
  return VCP->AppearanceNameArray;
}

NLM_EXTERN CharPtr PNTR GetFilterNameList (void)

{
  ViewerConfigsPtr VCP;
  VCP = GetGraphicConfigParseResults ();
  if (VCP == NULL) return NULL;
  return VCP->FilterNameArray;
}

NLM_EXTERN CharPtr PNTR GetLayoutNameList (void)

{
  return NewGphLayoutNames;
}

/* sequin integration */
#include <bspview.h>

#define MAXZOOMSCALEVAL 22

static Int4 zoomScaleVal [MAXZOOMSCALEVAL] = {
   1L, 1L, 2L, 5L, 10L, 20L, 50L, 100L, 200L, 500L,
   1000L, 2000L, 5000L, 10000L, 20000L, 50000L,
   100000L, 200000L, 500000L, 1000000L, 2000000L, 5000000L
};

static void get_viewer_position (
  VieweR viewer,
  Int4Ptr x,
  Int4Ptr y
)

{
     Int4 scale_x, scale_y;
     BoxInfo port, world;
     RecT v_iew;

     ViewerBox (viewer, &world, &port, &v_iew, &scale_x, &scale_y);
     *x = (port.left + port.right) / 2;
     *y = (port.top + port.bottom) / 2;
}

/*
static void Asn2GphSelectRegion (VieweR viewer, SegmenT segment, PrimitivE primitive,
			      SeqLocPtr region, Uint2 entityID)

{
  BioseqPtr  bsp;
  RecT       d;
  ViewPData  extra;
  RecT       r;
  RecT       s;
  SegPPtr    seg;
  WindoW     tempPort;
  BoxInfo    pLimits;

  if (viewer == NULL || segment == NULL || primitive == NULL || region == NULL) return;
  seg = (SegPPtr) segment;
  if (seg->base.code != SEGMENT && seg->base.code != PICTURE) return;
  if (! seg->seg.visible) return;
  if (Visible (viewer) && AllParentsVisible (viewer)) {
    GetPanelExtra ((PaneL) viewer, &extra);
    TryGetPrimitiveLimits ((BasePPtr) primitive, extra.scale.scaleX,
                           extra.scale.scaleY, &pLimits);
    bsp = GetBioseqGivenSeqLoc (region, entityID);
    if (bsp == NULL) return;
    if (ABS (pLimits.left - pLimits.right) < bsp->length) return;
    pLimits.left = SeqLocStart (region);
    pLimits.right = SeqLocStop (region);
    if (! BoxInViewport (&r, &(pLimits), &(extra.scale))) return;
    InsetRect (&r, 0, -3);
    if (ABS (r.left - r.right) < 2) {
      InsetRect (&r, -1, 0);
    }
    ObjectRect (viewer, &s);
    InsetRect (&s, 2, 2);
    SectRect (&r, &s, &d);
    tempPort = SavePort (viewer);
    Select (viewer);
    Dotted ();
    FrameRect (&d);
    Solid ();
    RestorePort (tempPort);
  }
}


static Boolean Asn2GphDrawSelectionPrim (SegmenT seg, PrimitivE prim, Uint2 segID,
                                  Uint2 primID, Uint2 primCt, VoidPtr userdata)

{
  Uint2         entityID;
  Uint2         itemID;
  Uint2         itemtype;
  SelStructPtr  sel;
  SelectPtr     sp;

  sp = (SelectPtr) userdata;
  GetPrimitiveIDs (thisPrim, &entityID, &itemID, &itemtype, NULL);
  for (sel = sp->sel; sel != NULL; sel = sel->next) {
    if (entityID == sel->entityID &&
        itemID == sel->itemID &&
        itemtype == sel->itemtype &&
        sel->region != NULL) {
      SelectRegion (sp->vwr, seg, prim, sel->region, entityID);
    }
  }
  return TRUE;
}

*/

typedef struct selectdata {
  VieweR        vwr;
  SelStructPtr  sel;
  Uint2         entityID;
  Uint2         itemID;
  Uint2         itemtype;
  SeqLocPtr     region;
} SelectData, PNTR SelectPtr;

static Boolean SelectAsn2GphPrim (
  SegmenT seg,
  PrimitivE prim,
  Uint2 segID,
  Uint2 primID,
  Uint2 primCt,
  VoidPtr userdata
)

{
  Uint2      entityID;
  Uint2      itemID;
  Uint2      itemtype;
  SelectPtr  sp;

  sp = (SelectPtr) userdata;
  GetPrimitiveIDs (prim, &entityID, &itemID, &itemtype, NULL);
  if (entityID == sp->entityID &&
      itemID == sp->itemID &&
      itemtype == sp->itemtype) {
    HighlightPrimitive (sp->vwr, seg, prim, FRAME_PRIMITIVE);
    /*
    HighlightSegment (sp->vwr, seg, FRAME_SEGMENT);
    */
  }
  return TRUE;
}

static Boolean DeselectAsn2GphPrim (
  SegmenT seg,
  PrimitivE prim,
  Uint2 segID,
  Uint2 primID,
  Uint2 primCt,
  VoidPtr userdata
)

{
  Uint2      entityID;
  Uint2      itemID;
  Uint2      itemtype;
  SelectPtr  sp;

  sp = (SelectPtr) userdata;
  GetPrimitiveIDs (prim, &entityID, &itemID, &itemtype, NULL);
  if (entityID == sp->entityID &&
      itemID == sp->itemID &&
      itemtype == sp->itemtype) {
    HighlightPrimitive (sp->vwr, seg, prim, PLAIN_PRIMITIVE);
    /*
    HighlightSegment (sp->vwr, seg, PLAIN_SEGMENT);
    */
  }
  return TRUE;
}

static void SelectAsn2GphView (
  BioseqViewPtr bvp,
  Uint2 selentityID,
  Uint2 selitemID,
  Uint2 selitemtype,
  SeqLocPtr region,
  Boolean select,
  Boolean scrollto
)

{
  SelectData  sd;

  if (bvp == NULL) return;
  if (! bvp->highlightSelections) return;
  sd.vwr = bvp->vwr;
  sd.sel = ObjMgrGetSelected ();
  sd.entityID = selentityID;
  sd.itemID = selitemID;
  sd.itemtype = selitemtype;
  sd.region = region;
  if (select) {
    ExploreSegment (bvp->pict, (Pointer) &sd, SelectAsn2GphPrim);
  } else {
    ExploreSegment (bvp->pict, (Pointer) &sd, DeselectAsn2GphPrim);
  }
}

static Boolean InitSelectAsn2GphPrim (
  SegmenT seg,
  PrimitivE prim,
  Uint2 segID,
  Uint2 primID,
  Uint2 primCt,
  VoidPtr userdata
)

{
  Uint2         entityID;
  Uint2         itemID;
  Uint2         itemtype;
  SelStructPtr  sel;
  SelectPtr     sp;

  sp = (SelectPtr) userdata;
  GetPrimitiveIDs (prim, &entityID, &itemID, &itemtype, NULL);
  for (sel = sp->sel; sel != NULL; sel = sel->next) {
    if (entityID == sel->entityID &&
        itemID == sel->itemID &&
        itemtype == sel->itemtype) {
      HighlightPrimitive (sp->vwr, seg, prim, FRAME_PRIMITIVE);
    }
  }
  return TRUE;
}

static void InitSelectAsn2GphView (BioseqViewPtr bvp)

{
  SelectData  sd;

  if (bvp == NULL) return;
  if (! bvp->highlightSelections) return;
  sd.vwr = bvp->vwr;
  sd.sel = ObjMgrGetSelected ();
  sd.entityID = 0;
  sd.itemID = 0;
  sd.itemtype = 0;
  sd.region = NULL;
  ExploreSegment (bvp->pict, (Pointer) &sd, InitSelectAsn2GphPrim);
}

static void DrawAsn2GphSelectionRange (
  VieweR viewer,
  SegmenT segment
)

{
}

static void ClickAsn2GphGraphic (
  VieweR vwr,
  SegmenT seg,
  PoinT pt
)

{
     BioseqViewPtr  bvp;

     bvp = (BioseqViewPtr) GetObjectExtra (vwr);
     if (bvp == NULL) return;
     bvp->wasDoubleClick = dblClick;
     bvp->wasShiftKey = shftKey;
     bvp->old_rect_shown = FALSE;
     bvp->pnt_start = pt;
     bvp->pnt_stop = pt;
}

static void ReleaseAsn2GphGraphic (
  VieweR vwr,
  SegmenT seg,
  PoinT pt
)

{
  BioseqViewPtr  bvp;
  Uint2          entityID;
  Uint2          itemID;
  Uint2          itemtype;
  SeqEntryPtr    sep;
  PrimitivE      thisPrim;
  SegmenT        smallestSeg; /*!! this may be equivalent to the "seg" parameter*/

  bvp = (BioseqViewPtr) GetObjectExtra (vwr);
  if (bvp == NULL) {
    ObjMgrDeSelect (0, 0, 0, 0, NULL);
    return;
  }

  smallestSeg = 
    FindSegPrim (vwr, pt, NULL, NULL, &thisPrim);
  if (smallestSeg == NULL || thisPrim == NULL) {
    ObjMgrDeSelect (0, 0, 0, 0, NULL);
    return;
  }

  GetPrimitiveIDs (thisPrim, &entityID, &itemID, &itemtype, NULL);

  if (entityID == 0 && itemID == 0 && itemtype == 0) {
    ObjMgrDeSelect (0, 0, 0, 0, NULL);
    return;
  }
  if (bvp->wasDoubleClick) {
        
    sep = GetTopSeqEntryForEntityID (entityID);
    if (bvp->launchSubviewers) {
      WatchCursor ();
      Update ();
      LaunchNewBioseqViewer (bvp->bsp, entityID, itemID, itemtype);
      ArrowCursor ();
      Update ();
      return;
    } else if (LaunchViewerNotEditor (bvp, sep, entityID, itemID, itemtype)) {
      WatchCursor ();
      Update ();
      LaunchNewBioseqViewer (bvp->bsp, entityID, itemID, itemtype);
      ArrowCursor ();
      Update ();
      return;
    } else if (bvp->launchEditors) {
      WatchCursor ();
      Update ();
      GatherProcLaunch (OMPROC_EDIT, FALSE, entityID, itemID,
			itemtype, 0, 0, itemtype, 0);
      ArrowCursor ();
      Update ();
    }
  }
  if (! bvp->sendSelectMessages) return;
  if (bvp->wasShiftKey) {
    ObjMgrAlsoSelect (entityID, itemID, itemtype, 0, NULL);
  } else {
    ObjMgrSelect (entityID, itemID, itemtype, 0, NULL);      
  }
}


static void PopulateAsn2GphGraphic (
  BioseqViewPtr bvp
)

{
     BioseqPtr    bsp;
     Uint2        entityID;
     Uint2         i;
     Int4         len;
     Int4         max;
     Int4         min;

     SeqEntryPtr  oldscope = NULL;
     RecT         r;
     Int4         scaleX;

     SeqEntryPtr  sep;
     Char         str[32];
     Int4         vwr_x;
     Int4         vwr_y;
     Int2         vwr_align;

     CharPtr PNTR nameList;
     CharPtr      appearanceName;
     CharPtr      filterName;
     CharPtr      layoutName;

     if (bvp == NULL) return;

     WatchCursor ();

     vwr_x = INT4_MIN;
     vwr_y = INT4_MAX;
     vwr_align = UPPER_LEFT;
     if (bvp->moveToOldPos) {
         if (bvp->pict != NULL) {
             get_viewer_position (bvp->vwr, &(vwr_x), &(vwr_y));
             vwr_align = MIDDLE_CENTER;
         }
         bvp->moveToOldPos = FALSE;
   }

   Reset (bvp->vwr);
   bvp->pict = DeletePicture (bvp->pict);
   Update ();

   bsp = bvp->bsp;
   if (bsp == NULL) {
     ArrowCursor ();
     return;
   }
   if (bvp->scaleNotCalculated) {
      SafeHide (bvp->newGphScale);
      Reset (bvp->newGphScale);
      ObjectRect (bvp->vwr, &r);
      InsetRect (&r, 4, 4);
      len = bsp->length;
      max = (Int4) MAX (len / (Int4) (r.right - r.left), 1) + 2;
      max = (Int4) MIN (max, 50000L);
      min = (Int4) MAX (len / 320000L, 1L);
      bvp->maxScale = max;
      i = 1;
      while (i < MAXZOOMSCALEVAL && min > zoomScaleVal[i]) {
        i++;
      }
      bvp->minIndex = i;
      while (i < MAXZOOMSCALEVAL && max > zoomScaleVal[i - 1]) {
        sprintf (str, "%ld", (long) (zoomScaleVal[i]));
        if (max < zoomScaleVal[i]) {
          sprintf (str, "%ld", (long) zoomScaleVal[i]);
        }
        PopupItem (bvp->newGphScale, str);
        i++;
      }
      SetValue (bvp->newGphScale, MAX (i - bvp->minIndex, 1));
      bvp->scaleNotCalculated = FALSE;
  }
  SafeShow (bvp->newGphScale);
  i = GetValue (bvp->newGphScale) - 1 + bvp->minIndex;
  if (i < MAXZOOMSCALEVAL && i > 0) {
    /*    scaleX = MIN (zoomScaleVal[i], bvp->maxScale);*/
    scaleX = zoomScaleVal[i]; /* always use the scale requested, even if greater that maxScale (!!! is this the right thing to do?) */
  } else {
    scaleX = bvp->minIndex;
  }

  i = GetValue (bvp->newGphFilter) - 1;
  nameList = GetFilterNameList ();
  if (i < GetFilterCount ()) {
    filterName = nameList [i];
  } else {
    filterName = "default"; /* as good a guess as any, and it _should_ exist in the config file*/
  }

  i = GetValue (bvp->newGphStyle) - 1;
  nameList = GetAppearanceNameList ();
  if (i < GetAppearanceCount ()) {
    appearanceName = nameList [i];
  } else {
    appearanceName = "default"; /* as good a guess as any, and it _should_ exist in the config file*/
  }

  i = GetValue (bvp->newGphLayout) - 1;
  nameList = GetLayoutNameList ();
  if (i < GetLayoutCount()) {
    layoutName = nameList[i];
  } else {
    layoutName = NULL; /* no need to pass a dummy string for this value */
  }

  sep = SeqMgrGetSeqEntryForData (bsp);
  entityID = ObjMgrGetEntityIDForChoice (sep);
  if (entityID > 0) {
    sep = GetBestTopParentForData (entityID, bsp);
    oldscope = SeqEntrySetScope (sep);
  }


  bvp->pict = EasyGraphicView (bsp, scaleX, filterName, appearanceName, layoutName);

  if (entityID > 0) {
    SeqEntrySetScope (oldscope);
  }

  AttachPicture (bvp->vwr, bvp->pict, vwr_x, vwr_y, vwr_align, scaleX, 1, bvp->highlightSelections ? DrawAsn2GphSelectionRange : NULL);
  SetViewerProcs (bvp->vwr, ClickAsn2GphGraphic, NULL, ReleaseAsn2GphGraphic, NULL);
  
  InitSelectAsn2GphView (bvp);

  ArrowCursor ();
}

static void ShowAsn2GphGraphic (
  BioseqViewPtr bvp,
  Boolean show
)

{
  if (bvp == NULL)
    return;
  if (show) {
    SafeShow (bvp->vwr);
    SafeShow (bvp->newGphControlGrp);
    EnableDisableLegendItem (bvp, TRUE);
    SafeHide (bvp->findGeneGrp);
    SafeShow (bvp->clickMe);
  } else {
    SafeHide (bvp->vwr);
    Reset (bvp->vwr);
    bvp->pict = DeletePicture (bvp->pict);
    SafeHide (bvp->styleControlGrp);
    SafeHide (bvp->scaleControlGrp);
    EnableDisableLegendItem (bvp, FALSE);
    SafeHide (bvp->findGeneGrp);
    SafeHide (bvp->docTxtControlGrp);
    SafeHide (bvp->baseCtgControlGrp);
    SafeHide (bvp->modeControlGrp);
    SafeHide (bvp->newGphControlGrp);
    SafeHide (bvp->clickMe);
  }
}
  
static void ResizeGraphical (BioseqViewPtr bvp)

{
  if (bvp == NULL) return;
  if (bvp->vwr != NULL) {
    if (Visible (bvp->vwr) && AllParentsVisible (bvp->vwr)) {
      ViewerWasResized (bvp->vwr);
    }
  }
}

NLM_EXTERN BioseqPageData  asn2gphGphPageData = {
  "NewGraphic", TRUE, TRUE, TRUE, FALSE, -1,
  PopulateAsn2GphGraphic, ShowAsn2GphGraphic, SelectAsn2GphView,
  NULL, NULL, NULL, NULL, ResizeGraphical, NULL
};
