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
* $Revision: 6.125 $
*
* File Description:
*
* Modifications:
* --------------------------------------------------------------------------
*
* ==========================================================================
*/

#include <asn2graphicp.h>
#include <sequtil.h>
#include <sqnutils.h>
#include <explore.h>
#include <drawingp.h>
#include <alignmgr2.h>

/* type used to accumulate align score weights, should be small to save space, but large enough we do not overflow. */
typedef Int2 AccumValue_t;
typedef AccumValue_t PNTR AccumValuePtr_t;
#define ACCUMVALUE_MAX  INT4_MAX

/*
  Given a sequence alignment, look at its score
  and return a suitable integer (AccumValue_t) weight.'
  scoretype gives which part of the score to use.
*/
enum {
  NLM_SCORE_COUNT = 0,
  NLM_SCORE_SCORE,
  NLM_SCORE_BIT,
  NLM_SCORE_EVALUE,
  NLM_SCORE_NUMBER,
  NLM_SCORE_TOOBIG
};


#define RELEVANT_FEATS_PER_CHUNK 128

/* these do not apply to individual features; they are used, ie in "AddFeatureToFilter (FEATDEF_ANY_RNA. . .)
   that would make the filter include _all_ types of RNA */
#define FEATDEF_ANY_PROT (APPEARANCEITEM_MAX + 1)
#define FEATDEF_ANY_RNA (APPEARANCEITEM_MAX + 2)

typedef struct renderInput {
  AppearanceItemPtr AIP;
  FilterItemPtr     FIP;
  RelevantFeatureItemPtr RFIP;
  SegmenT         labelSeg;
  SegmenT         drawSeg;
  Int4            yStart;
  Int4            decorationLeft;
  Int4            decorationRight;
  Int4            decorationHeight;
  Uint2           featureOffset;
  Uint2           Height, rowHeight;
} RenderInput, PNTR RenderInputPtr;

typedef struct viewerInstanceData {
  BioseqPtr       BSP;
  Uint4           viewScale;
  Uint4           seqLength;
  Int4Ptr         ceiling;
  SeqAnnotPtr PNTR sapList;
  Uint2           sapCount;
  SeqAnnotPtr     currentSAP;
  SegmenT         drawOnMe;
  SegmenT         sanLevelSeg;
  SegmenT         topLevelSeg;
  Uint4           featureCount;
  ValNodePtr      featVNP;      /* data.ptrvalue == RelevantFeatureItem [RELEVANT_FEATS_PER_CHUNK] */
  ValNodePtr      BSPsegmentVNP;
  AppearancePtr   AppPtr;
  FilterPtr       FltPtr;
  LayoutAlgorithm overrideLayout;
  Int4            gphheight;
  SegmenT         gphseg;
  Int4            gphyOffset;
  Int4            from;
  Int4            to;
  Boolean         allFeatures;
  GraphicViewExtrasPtr extras;
} ViewerContext, PNTR ViewerContextPtr;

typedef struct featureFilterState {
  ValNodePtr       currentRFIPblockVNP;
  Uint4            featureBlockOffset;
  Uint2            indexInBlock;
} FeatureFilterState, PNTR FeatureFilterStatePtr;

#define MAX_ALIGN_SORT_LABEL  20
typedef struct seq_align_sort_info {
  SeqAlignPtr sap;
  Int4  start, stop;
  Uint1 strand;
  Char  label[MAX_ALIGN_SORT_LABEL + 1];
  AccumValue_t  normScore;
} SeqAlignSortInfo, PNTR SeqAlignSortInfoPtr;

typedef struct AlignmentFilterState {
 /* SeqAlignPtr      SAPhead, SAPcurrent; /* obsolete */
  SeqAlignSortInfoPtr alignSorted;  /* array of SeqAlignPtrs & Info about them, sorted */
  Int4             alignSortedLen;  /* len of alignSorted */
  Int4             alignIndex;      /* index of current alignment in alignSorted */
  Uint1            scoreType;       /* what kind of score will we filter and weight by? */
  Int2             cutoffPercent;   /* include alignments in alignSorted only if their score is in the top cutoffPercent */
  AccumValue_t     cutoffScore;     /* normalised score Below which aligns won't get into alignSorted. */
  AccumValue_t     cutoffScoreHi;   /* normalised score Above which aligns won't get into alignSorted. */
  Int4             minScore, maxScore;  /* un-normalised min & max scores seen on these alignments */
} AlignmentFilterState, PNTR AlignmentFilterStatePtr;

/*
Bioseq's are still special-case for now.  todo: make bioseq segments use this mechanism
*/

typedef union filterState {
  FeatureFilterState feat;
  AlignmentFilterState align;
} FilterState, PNTR FilterStatePtr;  

typedef struct filterProcessState {
  FilterState      state;
  FilterItemPtr    currentFIP;
  ValNodePtr       currentFilterVNP;
  ValNodePtr       needFreeList; /* things to be freed with ValNodeFreeData() after finishing this filter */
  ValNodePtr       lastInFreeList;
  SegmenT          labelSegs [APPEARANCEITEM_MAX];
  SegmenT          drawSegs [APPEARANCEITEM_MAX];
  Int4             ceiling;
  RenderInput      renderParm;
  Uint4            featuresProcessedCount;
  BoolPtr          featuresProcessed;    /*points to an array of boolean [vContext->featureCount] */
  ViewerContextPtr vContext;
  Boolean          groupBoxDrawn;
  Boolean          groupLabelDrawn;
  Boolean          annotBoxDrawn;
  Boolean          annotLabelDrawn;
} FilterProcessState, PNTR FilterProcessStatePtr;

typedef void (*RenderFuncPtr)    (RenderInputPtr rip, ViewerContextPtr vContext);
typedef void (*GetDimensionsPtr) (RenderInputPtr, Int4Ptr start, Int4Ptr stop, Uint2Ptr height, ViewerContextPtr vContext);

typedef struct renderClass {
  RenderFuncPtr RenderFunc;
  GetDimensionsPtr GetDimensions;
} RenderClass, PNTR RenderClassPtr;

typedef struct internalRow {
  Uint4           rowFeatureCount;
  DataVal         layoutData;
  ValNodePtr      feats;        /* data.ptrvalue == RelevantFeatureItemPtr */
  struct internalRow PNTR next;
} InternalRow, PNTR InternalRowPtr;

/* returns the total number of _rows_ */
typedef Uint2   (PNTR LayoutFunction) (InternalRowPtr firstRow, FilterProcessStatePtr FPSP);


static CharPtr LayoutStrings [] = {
  "Inherit", /* Do not override layout in filter */
  "Diagonal", /* One feature per row */
  /* "DiagonalSawtooth" */
  "ByType", /* like "compact", except that no row will contain _different_ types of features (this may use more rows) */
  "Smear", /* one row contains all features of a given type (overlapping features will render as an anonymous smear */
  /*  "Single Row", -- not working currently*/ /* _all_ features are rendered in a single row (equivalent to "smear" if given only one type of features)*/
  "Compact", /* Overlapping features are drawn in different rows, but this tries to minimize the number of rows, */
  "GeneProducts", /* Group associated mRNA and CDS regions . . . */
  "GeneProductsX", /* Like GeneProducts, but may display the same mRNA multiple times if it maps to multiple CDS regions. . . */
  NULL
};

static LayoutAlgorithm LayoutValues [] = {
  Layout_Inherit,
  Layout_Diagonal,
  /* Layout_DiagonalSawtooth, */
  Layout_FeatTypePerLineGroup,
  Layout_FeatTypePerLine,
  /*  Layout_AllInOneLine, not working currently (and not all that useful, either)*/
  Layout_PackUpward,
  Layout_GroupCorrespondingFeats,
  Layout_GroupCorrespondingFeatsRepeat
};

static CharPtr AlnScoreStrings [] = {
    "None",
    "Score",
    "Bit Score",
    "E-value",
    "Number",
    NULL
};

/*
static CharPtr AlnScoreCutoffStrings[] = {
    "100%",
    "50%",
    "20%",
    "10%",
    "5%",
    "2%",
    "1%",
    NULL
};

static Int2 AlnScoreCutoffValues [] = {
    100, 
    50,
    20, 
    10, 
    5, 
    2, 
    1, 
    0
};
*/


static CharPtr AlnScoreCutoffStrings[] = {
    "None",
    "40",
    "50",
    "60",
    "70",
    "80",
    "100",
    "200",
    "400",
    NULL
};

static Int2 AlnScoreCutoffValues [] = {
    -1,
    40,
    50,
    60,
    70,
    80,
    100,
    200,
    400,
    0
};

static CharPtr  LlocStrings [] = {
  "none",
  "clip",
  "cull",
  "inside",
  "above",
  "below",
  "left",
  "right",
  NULL
};

static LabelLocEnum LlocValues [] = {
  LabelNone,
  LabelAboveClip, /* above, but not wider than the feature*/
  LabelAboveCull, /* above, but only displayed iff wider than the feature */
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

static CharPtr  GapStrings [] = {
  "none",
  "line",
  "angle",
  NULL
};

static GapEnum  GapValues [] = {
  NoGap,
  LineGap,
  AngleGap
};

static CharPtr GroupLabelLocations [] = {
  "above",
  "top",
  "left",
  "below",
  "none",
  NULL
};

static GroupLabelLocation GroupLabelLocationValues [] = {
  LabelOnTop,
  LabelOnTop,
  LabelOnSide,
  LabelOnBottom,
  NoLabel
};

static CharPtr BioseqFormat [] = {
  "accession",
  "accn",
  "fasta",
  "long",
  NULL
};

static Uint1 BioseqFormatValues [] = {
  PRINTID_TEXTID_ACC_VER,
  PRINTID_TEXTID_ACC_VER,
  PRINTID_FASTA_SHORT,
  PRINTID_FASTA_LONG
};

static CharPtr StrandStrings [] = {
  "both",
  "minus",
  "plus",
/* these could be added if people would find them useful. . .
   "-",
   "+",
*/
  NULL
};

static StrandChoice StrandValues [] = {
  BothStrands,
  MinusStrand,
  PlusStrand
};

static CharPtr ColorStrings [] = {
"black",    /*0,0,0*/
"blue",     /*0,0,255*/
"brown",    /*133,62,38*/
"coral",    /*255,127,80*/
"cyan",     /*0,255,255*/
"gray",     /*127,127,127*/
"green",    /*0,255,0*/
"grey",     /*127,127,127*/
"lavender", /*230,230,250*/
"magenta",  /*255,0,255*/
"maroon",   /*176,48,96*/
"orange",   /*255,165,0*/
"olive",    /*107,142,35*/
"pink",     /*255,192,203*/
"purple",   /*175,0,255*/
"red",      /*255,0,0*/
"white",    /*255,255,255*/
"yellow",   /*255,255,0*/
NULL
};

static Uint1 ColorValues[][3] = {
/*black*/    {0,   0,   0},
/*blue*/     {0,   0,   255},
/*brown*/    {133, 62,  38},
/*coral*/    {255, 127, 80},
/*cyan*/     {0,   255, 255},
/*gray*/     {127, 127, 127},
/*green*/    {0,   255, 0},
/*grey*/     {127, 127, 127},
/*lavender*/ {230, 230, 250},
/*magenta*/  {255, 0,   255},
/*maroon*/   {176, 48,  96},
/*orange*/   {255, 165, 0},
/*olive*/    {107, 142, 35},
/*pink*/     {255, 192, 203},
/*purple*/   {175, 0,   255},
/*red*/      {255, 0,   0},
/*white*/    {255, 255, 255},
/*yellow*/   {255, 255, 0}
};

static Char  config_filename [] = "asn2gph";

/*** Static function declarations ****/

static Int4 PixelsBetweenSeqCoords(Int4 left, Int4 right, Uint4 viewScale);
static Boolean IsInsertTic(RelevantFeatureItemPtr RFIP);
static void render_insert_tics(RenderInputPtr RIP);

/* functions for working with alignments */
/* perhaps some of these should be moved to the alignment manager. */
static Boolean SeqAlignContentLabel(SeqAlignPtr sap, SeqIdPtr notThisSID, CharPtr buf, Int4 buflen, Uint1 format);
AccumValue_t NormaliseScore(Int4 rawscore, Int4 min, Int4 max);
static Int4 WeightFromAlignScore(SeqAlignPtr sap, Uint1 scoreType);
static void FindCutoffScore(SeqAlignPtr sap, Int4 alignCnt, AlignmentFilterStatePtr afsp);
static void GatherAlignInfo(
  SeqAlignPtr sap, 
  Int4        alignCnt,
  SeqIdPtr    bioSeqId,
  AlignmentFilterStatePtr afsp
);
static Boolean AlignmentFilterStateInit(SeqAlignPtr sap, SeqIdPtr sid, AlignmentFilterStatePtr afsp, GraphicViewExtrasPtr extras);
static Boolean AlignmentFilterStateDone(AlignmentFilterStatePtr afsp);
static void AlignmentFilterStateFree( AlignmentFilterStatePtr afsp);

typedef struct align_seg_iterator {
  SeqAlignPtr sap;
  SeqIdPtr sip;
  
  Int4    nsegs;
  Int4    start, stop;  /* left and rightmost bioseq coords for this alignment. */
  Uint1   strand;   /* so we iterate the right direction */
  Int4    alignRow; /* from sip on non-stdseg aligns */
  Int4    currentSeg; /* for indexed aligns */
  StdSegPtr currentStdSeg;  /* for std seg aligns */
} AlignSegIterator, PNTR AlignSegIteratorPtr;

static Int4 AlignSegIteratorCreate(SeqAlignPtr sap, SeqIdPtr sip,  AlignSegIteratorPtr asi);
static Boolean AlignSegIteratorNext(AlignSegIteratorPtr asip, Int4Ptr start, Int4Ptr stop, Uint1Ptr strand, Uint1Ptr segType);


static Int1 StringIndexInStringList (CharPtr testString, CharPtr PNTR stringList) {

  Int1  i;

  if (testString == NULL || stringList == NULL) return -1;
  /*
  for (i = 0; stringList [i] != NULL; i++) {
    if (StringICmp (testString, stringList [i]) == 0) return i;
  }
  */
  i = 0;
  while (stringList [i] != NULL) {
    if (StringICmp (testString, stringList [i]) == 0) return i;
    i++;
  }
  return -1;
}

/* this parses fonts either in the format used by Vibrant's ParseFont, or allows "small" "meduim", etc. . . */
static FonT LocalParseFont (
  CharPtr FontSpec
)

{
  if (FontSpec == NULL) return NULL;
  if (StringICmp (FontSpec, "small") == 0) {
    return SetSmallFont ();
  } else if (StringICmp (FontSpec, "medium") == 0) {
    return SetMediumFont ();
  } else if (StringICmp (FontSpec, "large") == 0) {
    return SetLargeFont ();
  } else if (StringICmp (FontSpec, "system") == 0) {
    return systemFont;
  } else if (StringICmp (FontSpec, "program") == 0) {
    return programFont;
  } else {
    return ParseFont (FontSpec);
  }
}

static CharPtr ColorModifiers [] = {
"light",
"lt",
"dark",
"drk",
"dk",
NULL
};

/* COLOR must point to an array of Uint1 [3]; */
static Boolean ParseColor (
  CharPtr string,
  Uint1Ptr color
)

{
  unsigned  sscanfOffset, localColor [3] = { 0, 0, 0 }; /* "unsigned", to match %u and %n*/
  Uint1     offset = 0;
  Uint1     i;
  Boolean   isLight = FALSE, isDark = FALSE;
  Char      ch, modifierBuffer [8];
  CharPtr   tmp;


  if (string == NULL || color == NULL) return FALSE;

  /* first try to parse a human-readable color (ie, light blue)*/
  StringNCpy_0 (modifierBuffer, string, sizeof (modifierBuffer));
  /* truncate at space */
  tmp = modifierBuffer;
  ch = *tmp;
  while (ch != '\0' && ch != ' ') {
    tmp++;
    ch = *tmp;
  }
  *tmp = '\0';

  i = StringIndexInStringList (modifierBuffer, ColorModifiers);
  if (i > 0 && i < DIM (ColorModifiers)) {
    switch (i) {
      case 1 :
      case 2 :
        isLight = TRUE;
        offset = StringLen (modifierBuffer);
        break;
      case 3 :
      case 4 :
      case 5 :
        isDark = TRUE;
        offset = StringLen (modifierBuffer);
        break;
      default :
        break;
    }
  }
  while (string[offset] != '\0' && (! IS_ALPHA (string[offset]))) {
    offset ++;
  }

  i = StringIndexInStringList (string + offset, ColorStrings);
  if (i > 0 && i < DIM (ColorValues)) {
    color [0] = ColorValues [i] [0];
    color [1] = ColorValues [i] [1];
    color [2] = ColorValues [i] [2];
    if (isLight) {
      color [0] = MIN (((3 * color [0]) / 2), 255);
      color [1] = MIN (((3 * color [1]) / 2), 255);
      color [2] = MIN (((3 * color [2]) / 2), 255);
    } else if (isDark) {
      color [0] = (2 * color [0]) / 3;
      color [1] = (2 * color [1]) / 3;
      color [2] = (2 * color [2]) / 3;
    }
  } else {
    offset = 0;
    for (i = 0; i < 3; i++) {
      for (; string[offset] != '\0' && (! IS_DIGIT (string [offset])); offset++) continue;
      if (string [offset] == '\0') return FALSE;
      if (sscanf (string + offset, "%u%n", localColor + i, &sscanfOffset) == 0)  return FALSE;
      offset += sscanfOffset;
    }
    color [0] = localColor [0];
    color [1] = localColor [1];
    color [2] = localColor [2];
  }
  return TRUE;
}


NLM_EXTERN FilterPtr FindFilterByName (
  CharPtr name,
  ViewerConfigsPtr VCP
)

{
  Int1  i;

  if (VCP == NULL || StringHasNoText (name) || (! VCP->ArraysPopulated)) return NULL;
  i = StringIndexInStringList (name, VCP->FilterNameArray);
  if (i < VCP->FilterCount && i >= 0) return (VCP->FilterArray[i]);
  return NULL;
}

NLM_EXTERN AppearancePtr FindAppearanceByName (
  CharPtr name,
  ViewerConfigsPtr VCP
)

{
  Int1  i;

  if (VCP == NULL || StringHasNoText (name) || (! VCP->ArraysPopulated)) return NULL;
  i = StringIndexInStringList (name, VCP->AppearanceNameArray);
  if (i < VCP->AppearanceCount && i>= 0) return (VCP->AppearanceArray[i]);
  return NULL;
}

NLM_EXTERN LayoutAlgorithm FindLayoutByName (
  CharPtr name
)

{
  Int1  i;

  if (StringHasNoText (name)) return Layout_Inherit;
  i = StringIndexInStringList (name, LayoutStrings);
  if (i >= 0 && i < DIM (LayoutValues)) {
    return LayoutValues [i];
  }
  return Layout_Inherit;
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

static CharPtr FindFeatStrFromFeatDefType (Uint1 type)
{
  CharPtr featName;

  featName = FindKeyFromFeatDefType (type, FALSE);
  /* handle locally defined feature types. */
  if (type > FEATDEF_MAX)
  {
    switch (type) {
    case APPEARANCEITEM_Alignment:
      featName = "align";
      break;
    case APPEARANCEITEM_Segment:
      featName = "segment";
      break;
    case APPEARANCEITEM_Graph:
      featName = "graph";
      break;
    }
  }
  return featName;
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

static BioseqAppearanceItemPtr ParseBioseqAppearanceItem (
  CharPtr sect,
  ViewerConfigsPtr VCP
)

{
  Char                     buf [128];
  BioseqAppearanceItemPtr  bioseqAIP;
  Int2                     i;
  unsigned                 val; /* "unsigned" to match sscanf("%ud")*/

  bioseqAIP = MemNew (sizeof (BioseqAppearanceItem));
  if (bioseqAIP == NULL) return NULL;

  bioseqAIP->labelLoc = GroupLabelLocationValues [0];
  if (GetAppParam (config_filename, sect, "label", NULL, buf, sizeof (buf))) {
    i = StringIndexInStringList (buf, GroupLabelLocations);
    if (i >= 0 && i < DIM (GroupLabelLocationValues)) {
      bioseqAIP->labelLoc = GroupLabelLocationValues  [i];
    }
  }

  bioseqAIP->drawScale = TRUE;
  if (GetAppParam (config_filename, sect, "scale", NULL, buf, sizeof (buf))) {
    i = StringIndexInStringList (buf, BoolStrings);
    if (i >= 0 && i < DIM (BoolValues)) {
      bioseqAIP->drawScale = BoolValues [i];
    }
  }

  if (GetAppParam (config_filename, sect, "labelfont", NULL, buf, sizeof (buf))) {
    bioseqAIP->labelFont = LocalParseFont (buf);
  }
  if (bioseqAIP->labelFont == NULL) {
    bioseqAIP->labelFont = systemFont;
  }

  if (GetAppParam (config_filename, sect, "scalefont", NULL, buf, sizeof (buf))) {
    bioseqAIP->scaleFont = LocalParseFont (buf);
  }
  if (bioseqAIP->scaleFont == NULL) {
    bioseqAIP->scaleFont = SetSmallFont ();
  }

  if (GetAppParam (config_filename, sect, "height", NULL, buf, sizeof (buf))) {
    if (buf != NULL) {
      sscanf (buf, "%ud", &val);
      bioseqAIP->height = MIN (val, 16);
    }
  }
  if (bioseqAIP->height == 0) {
    bioseqAIP->height = 10;
  }

  if (GetAppParam (config_filename, sect, "scaleheight", NULL, buf, sizeof (buf))) {
    if (buf != NULL) {
      sscanf (buf, "%ud", &val);
      bioseqAIP->scaleHeight = MIN (val, 16);
    }
  }
  if (bioseqAIP->scaleHeight == 0) {
    bioseqAIP->scaleHeight = 10;
  }

  if (GetAppParam (config_filename, sect, "color", NULL, buf, sizeof (buf))) {
    ParseColor (buf, bioseqAIP->bioseqColor);
  }
  if (GetAppParam (config_filename, sect, "labelcolor", NULL, buf, sizeof (buf))) {
    ParseColor (buf, bioseqAIP->labelColor);
  }
  if (GetAppParam (config_filename, sect, "scalecolor", NULL, buf, sizeof (buf))) {
    ParseColor (buf, bioseqAIP->scaleColor);
  }

  bioseqAIP->format = BioseqFormatValues [0];
  if (GetAppParam (config_filename, sect, "format", NULL, buf, sizeof (buf))) {
    i = StringIndexInStringList (buf, BioseqFormat);
    if (i >= 0 && i < DIM (BioseqFormatValues)) {
      bioseqAIP->format = BioseqFormatValues  [i];
    }
  }

  return bioseqAIP;
}

static AppearanceItemPtr ParseFeatureAppearanceItem (
  CharPtr sect,
  AppearanceItemPtr inheritFromMe,
  Boolean recursing,
  ViewerConfigsPtr VCP
)

{
  Char               buf [128];
  AppearanceItemPtr  newAIP;
  Int2               i;
  Boolean            changed = FALSE;
  unsigned           val; /* "unsigned" to match sscanf("%ud")*/
  AppearanceItemPtr  namedAIP;

  if (! recursing) {
    if (GetAppParam (config_filename, sect, "usenamedstyle", NULL, buf, sizeof (buf))) {
      namedAIP = ParseFeatureAppearanceItem (buf, inheritFromMe, TRUE, VCP);
      if (namedAIP != NULL) {
        inheritFromMe = namedAIP;
        changed = TRUE; /* !!! this will use more memory than necessary */
      }
    }
  }
  newAIP = MemNew (sizeof (AppearanceItem));
  if (newAIP == NULL) return NULL;
  MemCopy (newAIP, inheritFromMe, sizeof (AppearanceItem));
  if (GetAppParam (config_filename, sect, "color", NULL, buf, sizeof (buf))) {
    changed = ParseColor (buf, newAIP->Color);
  }
  if (GetAppParam (config_filename, sect, "labelcolor", NULL, buf, sizeof (buf))) {
    changed = ParseColor (buf, newAIP->LabelColor);
  }

  newAIP->LabelLoc = LabelAbove;
  if (GetAppParam (config_filename, sect, "label", NULL, buf, sizeof (buf))) {
    i = StringIndexInStringList (buf, LlocStrings);
    if (i >= 0 && i < DIM (LlocValues)) {
      newAIP->LabelLoc = LlocValues [i];
    }
  }
  if (GetAppParam (config_filename, sect, "displaywith", NULL, buf, sizeof (buf))) {
    i = StringIndexInStringList (buf, RenderStrings);
    if (i >= 0 && i < DIM (RenderValues)) {
      newAIP->RenderChoice = RenderValues [i];
      changed = TRUE;
    }
  }
  if (GetAppParam (config_filename, sect, "showarrow", NULL, buf, sizeof (buf))) {
    i = StringIndexInStringList (buf, BoolStrings);
    if (i >= 0 && i < DIM (BoolValues)) {
      newAIP->ShowArrow = BoolValues [i];
      changed = TRUE;
    }
  }
  if (GetAppParam (config_filename, sect, "gap", NULL, buf, sizeof (buf))) {
    i = StringIndexInStringList (buf, GapStrings);
    if (i >= 0 && i < DIM (RenderValues)) {
      newAIP->GapChoice = GapValues [i];
      changed = TRUE;
    }
  }
  if (GetAppParam (config_filename, sect, "showtype", NULL, buf, sizeof (buf))) {
    i = StringIndexInStringList (buf, BoolStrings);
    if (i >= 0 && i < DIM (BoolValues)) {
      newAIP->AddTypeToLabel = BoolValues [i];
    }
  }
  if (GetAppParam (config_filename, sect, "showcontent", NULL, buf, sizeof (buf))) {
    i = StringIndexInStringList (buf, BoolStrings);
    if (i >= 0 && i < DIM (BoolValues)) {
      newAIP->AddDescToLabel = BoolValues [i];
    }
  }
  if (GetAppParam (config_filename, sect, "height", NULL, buf, sizeof (buf))) {
    if (buf != NULL) {
      sscanf (buf, "%ud", &val);
      newAIP->Height = MIN (val, 16);
      changed = TRUE;
    }
  }
  if (GetAppParam (config_filename, sect, "labelfont", NULL, buf, sizeof (buf))) {
    newAIP->LabelFont = LocalParseFont (buf);
  }
  if (newAIP->LabelFont == NULL) {
    newAIP->LabelFont = programFont;
  }
  if (newAIP->LabelFont != inheritFromMe->LabelFont) {
    changed = TRUE;
  }
  
  /* this is only meaningful for alignment styles to change the appearance of the alignment's label. */
  newAIP->format = BioseqFormatValues [0];
  if (GetAppParam (config_filename, sect, "format", NULL, buf, sizeof (buf))) {
    i = StringIndexInStringList (buf, BioseqFormat);
    if (i >= 0 && i < DIM (BioseqFormatValues)) {
      newAIP->format = BioseqFormatValues  [i];
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
  Char               buf [128];
  Char               sect [128];
  Char               outputBuffer [128];
  AppearanceItem     DefaultAppearanceItem = {
    {0, 0, 0}, {64, 64, 64}, Render_Box, 0, 5, 0, FALSE, LineGap, TRUE, TRUE, NULL, LabelAbove
  };
  Int1               i;
  unsigned           val;

  if (appearanceNameInFile == NULL) return NULL;
  DefaultAppearanceItem.LabelFont = programFont;
  sprintf (sect, "%s.master", appearanceNameInFile);
  /* require all styles to have a name, since high-level interface uses the name to identify Filters */
  if (! GetAppParam (config_filename, sect, "name", NULL, buf, sizeof (buf))) return NULL;
  if (StringHasNoText (buf)) return NULL;
  AP = CreateAppearance (buf, VCP);
  if (AP == NULL) return NULL;
  AIP = ParseFeatureAppearanceItem (sect, &DefaultAppearanceItem, FALSE, VCP); /*parse xyz.master */
  if (AIP == NULL) {            /* require a "master" style */
    DestroyAppearance (AP, VCP);
    return NULL;
  }
  val = VCP->DefaultMaxScaleForArrow;
  if (GetAppParam (config_filename, sect, "maxarrowscale", NULL, buf, sizeof (buf))) {
    sscanf (buf, "%ud", &val);
  }
  AP->MaxScaleForArrow = val;
  val = VCP->DefaultMinPixelsForArrow;
  if (GetAppParam (config_filename, sect, "minarrowpixels", NULL, buf, sizeof (buf))) {
    sscanf (buf, "%ud", &val);
  }
  AP->MinPixelsForArrow = val;
  AP->ShadeSmears = VCP->DefaultShadeSmears;
  if (GetAppParam (config_filename, sect, "shadesmears", NULL, buf, sizeof (buf))) {
    i = StringIndexInStringList (buf, BoolStrings);
    if (i >= 0 && i < DIM (BoolValues)) {
      AP->ShadeSmears = BoolValues [i];
    }
  }

  /* group box settings */
  if (GetAppParam (config_filename, sect, "groupboxcolor", NULL, buf, sizeof (buf))) {
    ParseColor (buf, AP->GroupBoxColor);
  }

  if (GetAppParam (config_filename, sect, "grouplabelfont", NULL, buf, sizeof (buf))) {
    AP->GroupLabelFont = LocalParseFont (buf);
    if (AP->GroupLabelFont == NULL) {
      AP->GroupLabelFont = programFont;
    }
  }
  if (GetAppParam (config_filename, sect, "grouplabelcolor", NULL, buf, sizeof (buf))) {
    ParseColor (buf, AP->GroupLabelColor);
  }

  /* Named Annotation Style settings */
  if (GetAppParam (config_filename, sect, "annotboxcolor", NULL, buf, sizeof (buf))) {
    ParseColor (buf, AP->AnnotBoxColor);
  }

  if (GetAppParam (config_filename, sect, "annotlabelfont", NULL, buf, sizeof (buf))) {
    AP->AnnotLabelFont = LocalParseFont (buf);
    if (AP->AnnotLabelFont == NULL) {
      AP->AnnotLabelFont = programFont;
    }
  }
  if (GetAppParam (config_filename, sect, "annotlabelcolor", NULL, buf, sizeof (buf))) {
    ParseColor (buf, AP->AnnotLabelColor);
  }


  sprintf (outputBuffer, "%s.bioseq", appearanceNameInFile);
  AP->bioseqAIP = ParseBioseqAppearanceItem (outputBuffer, VCP);
  sprintf (outputBuffer, "%s.imp", appearanceNameInFile);
  impAIP = ParseFeatureAppearanceItem (outputBuffer, AIP, FALSE, VCP);
  if (impAIP == NULL) {
    sprintf (outputBuffer, "%s.import", appearanceNameInFile);
    impAIP = ParseFeatureAppearanceItem (outputBuffer, AIP, FALSE, VCP);
  }
  if (impAIP == NULL) {
    impAIP = AIP;
  } else {
    AddAppearanceItemToAppearance (impAIP, AP, FEATDEF_IMP, VCP);
  }
  for (i = 1; i < APPEARANCEITEM_MAX; i++) {
    if (i == FEATDEF_IMP) continue;
    sprintf (outputBuffer, "%s.%s", appearanceNameInFile, FindFeatStrFromFeatDefType (i));
    if (i >= FEATDEF_allele && i <= FEATDEF_35_signal) {        /* is it an imp-feat ? */
      newAIP = ParseFeatureAppearanceItem (outputBuffer, impAIP, FALSE, VCP);
      newAIP = newAIP ? newAIP : impAIP;
    } else {
      newAIP = ParseFeatureAppearanceItem (outputBuffer, AIP, FALSE, VCP);
      newAIP = newAIP ? newAIP : AIP;
    }
    if (newAIP != NULL) {
      AddAppearanceItemToAppearance (newAIP, AP, i, VCP);
    }
  }
  AddAppearanceItemToAppearance (AIP, AP, FEATDEF_BAD, VCP);
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
  Uint1  order = 0;
  Uint1  orderIncrement = 0;

  if (FIP == NULL) return;

  if (includeMe) {
    orderIncrement = 1;
    for (i = 0; i < APPEARANCEITEM_MAX; i++) {
      order = MAX (order, FIP->IncludeFeature [i]);
    }
    order++; /* the lowest available order index */
  }

  if (newFeatdef == FEATDEF_ANY) {
    for (i = 1; i < APPEARANCEITEM_MAX; i++) {  /* start at 1 since we do not want to ever include FEATDEF_BAD */
      FIP->IncludeFeature [i] = order;
      order += orderIncrement;
    }
  } else if (newFeatdef == FEATDEF_ANY_RNA) {
    for (i = FEATDEF_preRNA; i <= FEATDEF_otherRNA; i++) {
      FIP->IncludeFeature [i] = order;
      order += orderIncrement;
    }
    FIP->IncludeFeature [FEATDEF_misc_RNA] = order;
    order += orderIncrement;
    FIP->IncludeFeature [FEATDEF_precursor_RNA] = order;
    order += orderIncrement;
    FIP->IncludeFeature [FEATDEF_snoRNA] = order;
  } else if (newFeatdef == FEATDEF_ANY_PROT) {
    FIP->IncludeFeature [FEATDEF_PROT] = order;
    order += orderIncrement;
    for (i = FEATDEF_preprotein; i <= FEATDEF_transit_peptide_aa; i++) {
      FIP->IncludeFeature [i] = order;
      order += orderIncrement;
    }
  } else if (newFeatdef == FEATDEF_IMP) {
    for (i = FEATDEF_allele; i <= FEATDEF_35_signal; i++) {
      FIP->IncludeFeature [i] = order;
      order += orderIncrement;
    }
  } else if (newFeatdef < APPEARANCEITEM_MAX) {
    FIP->IncludeFeature [newFeatdef] = order;
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

static CharPtr SpecialFeatures [] = {
"everything",
"all",
"every",
"any",
"imp",
"rna",
"prot",
"bioseq",
"graph",
"align",
NULL
};

static FilterItemPtr ParseFilterItem (
  CharPtr filterItemName,
  Uint2 defaultRowPadding,
  Uint2 defaultGroupPadding,
  LayoutAlgorithm defaultLayout,
  ViewerConfigsPtr VCP
)

{
  Char           sect [128];
  Char           featureNum [128];
  Char           buf [128];
  Uint4          featdeftype;
  Uint4          featureCount = 0;
  FilterItemPtr  FIP;
  Int2           i, j;
  unsigned       val;

  FIP = MemNew (sizeof (FilterItem));
  if (FIP == NULL) return FIP;
  FIP->DrawScale = TristateUnset;
  FIP->Type = FeatureFilter; /* this will get changed if a graph or alignment is discovered instead */
  sprintf (sect, "filters.%s", filterItemName);
  GetAppParam (config_filename, sect, "layout", "inherit", buf, sizeof (buf));
  i = StringIndexInStringList (buf, LayoutStrings);
  if (i >= 0 && i < DIM (LayoutStrings)) {
    FIP->LayoutChoice = LayoutValues [i];
  } else {
    FIP->LayoutChoice = defaultLayout;
  }

  FIP->GroupPadding = defaultGroupPadding;
  if (GetAppParam (config_filename, sect, "grouppadding", NULL, buf, sizeof (buf))) {
    sscanf (buf, "%ud", &val);
    val = MIN (val, 100);
    FIP->GroupPadding = val;
  }

  FIP->IntraRowPaddingPixels = defaultRowPadding;
  if (GetAppParam (config_filename, sect, "rowpadding", NULL, buf, sizeof (buf))) {
    sscanf (buf, "%ud", &val);
    val = MIN (val, 100);
    FIP->IntraRowPaddingPixels = val;
  }
  FIP->DrawGroupBox = FALSE;
  FIP->FillGroupBox = FALSE;
  if (GetAppParam (config_filename, sect, "groupbox", NULL, buf, sizeof (buf))) {
    i = StringIndexInStringList (buf, BoolStrings);
    if (i >= 0 && i < DIM (BoolValues) && i >= 0) {
      FIP->DrawGroupBox = BoolValues [i];
    }
  }
  if (FIP->DrawGroupBox) {
    if (GetAppParam (config_filename, sect, "groupboxcolor", NULL, buf, sizeof (buf))) {
      FIP->GroupBoxColorSet = TRUE;
      ParseColor (buf, FIP->GroupBoxColor);
    }
    if (GetAppParam (config_filename, sect, "fillbox", NULL, buf, sizeof (buf))) {
      i = StringIndexInStringList (buf, BoolStrings);
      if (i >= 0 && i < DIM (BoolValues)) {
        FIP->FillGroupBox = BoolValues[i];
      }
    }
  }

  FIP->GroupLabel = NoLabel;
  FIP->GroupLabelFont = programFont;
  if (GetAppParam (config_filename, sect, "name", NULL, buf, sizeof (buf))) {
    FIP->GroupLabel = StringSaveNoNull (buf);
    FIP->GroupLabelLoc = LabelOnTop;
    if (GetAppParam (config_filename, sect, "grouplabel", NULL, buf, sizeof (buf))) {
      i = StringIndexInStringList (buf, GroupLabelLocations);
      if (i >= 0 && i < DIM (GroupLabelLocationValues)) {
        FIP->GroupLabelLoc = GroupLabelLocationValues[i];
      }
    }
    if (GetAppParam (config_filename, sect, "grouplabelfont", NULL, buf, sizeof (buf))) {
      FIP->GroupLabelFontSet = TRUE;
      FIP->GroupLabelFont = LocalParseFont (buf);
      if (FIP->GroupLabelFont == NULL) {
        FIP->GroupLabelFont = programFont;
      }
    }
    if (GetAppParam (config_filename, sect, "grouplabelcolor", NULL, buf, sizeof (buf))) {
      FIP->GroupLabelColorSet = TRUE;
      ParseColor (buf, FIP->GroupLabelColor);
    }
  }

  FIP->LabelLoc = LabelAbove;
  if (GetAppParam (config_filename, sect, "label", NULL, buf, sizeof (buf))) {
    i = StringIndexInStringList (buf, LlocStrings);
    if (i >= 0 && i < DIM (LlocValues)) {
      FIP->LabelLoc = LlocValues [i];
    }
  }

  FIP->AddTypeToLabel = TristateUnset;
  if (FIP->LabelLoc != LabelNone && GetAppParam (config_filename, sect, "showtype", NULL, buf, sizeof (buf))) {
    i = StringIndexInStringList (buf, BoolStrings);
    if (i >= 0 && i < DIM (BoolValues)) {
      FIP->AddTypeToLabel = BOOL_TO_TRISTATE (BoolValues [i]);
    }
  }
  FIP->AddDescToLabel = TristateUnset;
  if (FIP->LabelLoc != LabelNone && GetAppParam (config_filename, sect, "showcontent", NULL, buf, sizeof (buf))) {
    i = StringIndexInStringList (buf, BoolStrings);
    if (i >= 0 && i < DIM (BoolValues)) {
      FIP->AddDescToLabel = BOOL_TO_TRISTATE (BoolValues [i]);
    }
  }

  FIP->MatchStrand = StrandValues [0];
  if (GetAppParam (config_filename, sect, "strand", NULL, buf, sizeof (buf))) {
    i = StringIndexInStringList (buf, StrandStrings);
    if (i >= 0 && i < DIM (StrandValues)) {
      FIP->MatchStrand = StrandValues [i];
    }
  }
  for (i = 1; i < APPEARANCEITEM_MAX; i++) {
    sprintf (featureNum, "feature%d", (unsigned) i);
    if (GetAppParam (config_filename, sect, featureNum, NULL, buf, sizeof (buf))) {
      featdeftype = FindFeatDefTypeFromKey (buf);
      if (featdeftype == FEATDEF_BAD) {
        /* special-case checks for types of features not found by FindFeatDefTypeFromKey () */
        j = StringIndexInStringList (buf, SpecialFeatures);
        /*
        if (j >= 0 && j < DIM (SpecialFeatures)) {
          switch (j) {
            case 1 :
            case 2 :
            case 3 :
            case 4 :
              featdeftype = FEATDEF_ANY;
              break;
            case 5 :
              featdeftype = FEATDEF_IMP;
              break;
            case 6 :
              featdeftype = FEATDEF_ANY_RNA;
              break;
            case 7 :
              featdeftype = FEATDEF_ANY_PROT;
              break;
            case 8 :
              FIP->Type = BioseqFilter;
              break;
            case 9 :
              FIP->Type = GraphFilter;
              break;
            case 10 :
              FIP->Type = AlignmentFilter;
              break;
            default :
              break;
          }
        } else continue; */
        /* insert special-case checks for types of features not found by FindFeatDefTypeFromKey () here */
        if (StringICmp (buf, "everything") == 0 ||
            StringICmp (buf, "all") == 0 ||
            StringICmp (buf, "every") == 0 ||
            StringICmp (buf, "any") == 0) {
          featdeftype = FEATDEF_ANY;
        } else if (StringICmp (buf, "imp") == 0) {
          featdeftype = FEATDEF_IMP; /* FindFeatDefTypeFromKey matches 'import', but not 'imp' */
        } else if (StringICmp (buf, "rna") == 0) {
          featdeftype = FEATDEF_ANY_RNA;
        } else if (StringICmp (buf, "prot") == 0) {
          featdeftype = FEATDEF_ANY_PROT;
        } else if (StringICmp (buf, "bioseq") == 0) {
          FIP->Type = BioseqFilter;
        } else if (StringICmp (buf, "graph") == 0) {
          featdeftype = APPEARANCEITEM_Graph;
          FIP->Type = GraphFilter;
        } else if (StringICmp (buf, "align") == 0) {
          featdeftype = APPEARANCEITEM_Alignment;
          FIP->Type = AlignmentFilter;
        } else continue; /* failed to find a match */
      }
      AddFeatureToFilterItem (FIP, featdeftype, VCP);
      featureCount++;
    }
  }
  if (FIP->Type == BioseqFilter) {
    FIP->DrawScale = TristateUnset;
    if (GetAppParam (config_filename, sect, "scale", NULL, buf, sizeof (buf))) {
      i = StringIndexInStringList (buf, BoolStrings);
      if (i >= 0 && i < DIM (BoolValues)) {
        FIP->DrawScale = BOOL_TO_TRISTATE (BoolValues [i]);
      }
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

  FilterPtr       FP;
  FilterItemPtr   FIP;
  Int2            i;
  Uint1           filterItemCount = 0;
  Char            buf [128];     /* for input *from* GetAppParam */
  Char            outputBuffer [128];    /* paramater *to* GetAppParam */
  Char            sect [128];
  Boolean         foundBioseqFilter = FALSE;
  Boolean         foundGraphFilter = FALSE;
  Boolean         foundAlignmentFilter = FALSE;
  ValNodePtr      VNP;
  Boolean         createImplicitBioseq = TRUE;
  Boolean         createImplicitGraphs = TRUE;
  unsigned        val; /* to match sscanf ("%d"...)*/
  Uint2           defaultRowPadding;
  Uint2           defaultGroupPadding;
  LayoutAlgorithm defaultLayout;

  if (filterNameInFile == NULL) return NULL;
  sprintf (sect, "%s", filterNameInFile);
  /* require all styles to have a name, since high-level interface uses the name to identify Filters */
  if (! GetAppParam (config_filename, sect, "name", NULL, buf, sizeof (buf))) return NULL;
  FP = CreateFilter (buf, VCP); /* Createfilter will check for duplucate names */
  if (FP == NULL) return FP;
  val = VCP->DefaultMaxScaleWithLabels;
  if (GetAppParam (config_filename, sect, "maxlabelscale", NULL, buf, sizeof (buf))) {
    sscanf (buf, "%ud", &val);
  }
  FP->MaxScaleWithLabels = val;
  
  /* Group by Named Annotation stuff */
  FP->AnnotBoxColorSet = FALSE;
  FP->AnnotLabelFontSet = FALSE;
  FP->AnnotLabelColorSet = FALSE;
  
  FP->GroupByAnnot = TRUE;
  if (GetAppParam (config_filename, sect, "annotgroup", NULL, buf, sizeof (buf))) {
    i = StringIndexInStringList (buf, BoolStrings);
    if (i >= 0 && i < DIM (BoolStrings)) {
      FP->GroupByAnnot =  (BoolValues [i]);
    }
  }
  if (FP->GroupByAnnot) {
    FP->DrawAnnotBox = TRUE;
    if (GetAppParam (config_filename, sect, "annotbox", NULL, buf, sizeof (buf))) {
      i = StringIndexInStringList (buf, BoolStrings);
      if (i >= 0 && i < DIM (BoolStrings)) {
        FP->DrawAnnotBox =  (BoolValues [i]);
      }
    }
    if (FP->DrawAnnotBox) {
      if (GetAppParam (config_filename, sect, "annotboxcolor", NULL, buf, sizeof (buf))) {
        FP->AnnotBoxColorSet = TRUE;
        ParseColor (buf, FP->AnnotBoxColor);
      }
    }
    FP->AnnotLabelLoc = LabelOnTop;
    if (GetAppParam (config_filename, sect, "annotlabel", NULL, buf, sizeof (buf))) {
      i = StringIndexInStringList (buf, GroupLabelLocations);
      if (i >= 0 && i < DIM (GroupLabelLocationValues)) {
        FP->AnnotLabelLoc = GroupLabelLocationValues[i];
      }
    }
    if (FP->AnnotLabelLoc != NoLabel) {
      FP->AnnotLabelFont = programFont;
      if (GetAppParam (config_filename, sect, "annotlabelfont", NULL, buf, sizeof (buf))) {
        FP->AnnotLabelFont = LocalParseFont (buf);
        if (FP->AnnotLabelFont == NULL) {
          FP->AnnotLabelFont = programFont;
        } else {
          FP->AnnotLabelFontSet = TRUE;
        }
      }
      if (GetAppParam (config_filename, sect, "annotlabelcolor", NULL, buf, sizeof (buf))) {
        FP->AnnotLabelColorSet = TRUE;
        ParseColor (buf, FP->AnnotLabelColor);
      }
    }
  }
  
  /* other default values */
  GetAppParam (config_filename, sect, "layout", NULL, buf, sizeof (buf));
  i = StringIndexInStringList (buf, LayoutStrings);
  if (i >= 0 && i < DIM (LayoutValues)) {
    defaultLayout = LayoutValues [i];
  } else {
    defaultLayout = Layout_Inherit;
  }

  val = VCP->DefaultGroupPadding;
  if (GetAppParam (config_filename, sect, "grouppadding", NULL, buf, sizeof (buf))) {
    sscanf (buf, "%ud", &val);
    val = MIN (val, 100);
  }
  defaultGroupPadding = val;

  val = VCP->DefaultRowPadding;
  if (GetAppParam (config_filename, sect, "rowpadding", NULL, buf, sizeof (buf))) {
    sscanf (buf, "%ud", &val);
  }
  defaultRowPadding = val;

  if (GetAppParam (config_filename, sect, "suppressbioseq", NULL, buf, sizeof (buf))) {
    i = StringIndexInStringList (buf, BoolStrings);
    if (i >= 0 && i < DIM (BoolStrings)) {
      createImplicitBioseq = ! (BoolValues [i]);
    }
  }
  if (GetAppParam (config_filename, sect, "suppressgraphs", NULL, buf, sizeof (buf))) {
    i = StringIndexInStringList (buf, BoolStrings);
    if (i >= 0 && i < DIM (BoolStrings)) {
      createImplicitGraphs = ! (BoolValues [i]);
    }
  }

  for (i = 1; i < APPEARANCEITEM_MAX; i++) {
    sprintf (outputBuffer, "%s%d", "group", (unsigned) i);
    if (GetAppParam (config_filename, sect, outputBuffer, NULL, buf, sizeof (buf))) {
      FIP = ParseFilterItem (buf, defaultRowPadding, defaultGroupPadding, defaultLayout, VCP);
      if (FIP == NULL) continue;
      if (FIP->Type == BioseqFilter) {
        foundBioseqFilter = TRUE;
      }
      if (FIP->Type == GraphFilter) {
        foundGraphFilter = TRUE;
      }
      if (FIP->Type == AlignmentFilter) {
        foundAlignmentFilter = TRUE;
      }
      AddFilterItemToFilter (FIP, FP, VCP);
      filterItemCount++;
    }
  }
  if (filterItemCount == 0) {
    DestroyFilter (FP, VCP);
    return NULL;
  }

  if (createImplicitBioseq && ! foundBioseqFilter) {
    VNP = MemNew (sizeof (ValNode));
    FIP = MemNew (sizeof (FilterItem));
    if (VNP == NULL || FIP == NULL) {
      DestroyFilter (FP, VCP);
      return NULL;
    }
    /* insert a Bioseq filter at the head of the list */
    VNP->next = FP->FilterItemList;
    VNP->data.ptrvalue = FIP;
    FP->FilterItemList = VNP;

    FIP->Type = BioseqFilter;
    FIP->IntraRowPaddingPixels = defaultRowPadding;
  }

  if (createImplicitGraphs && ! foundAlignmentFilter) {
    /* insert an alignment filter at the_end of the list */
    FIP = MemNew (sizeof (FilterItem));
    VNP = ValNodeAddPointer (&FP->FilterItemList, 0, FIP);
    if (FIP == NULL || VNP == NULL ) {
      DestroyFilter (FP, VCP);
      return NULL;
    }
    FIP->Type = AlignmentFilter;
    FIP->LayoutChoice = defaultLayout;
    FIP->GroupPadding = defaultGroupPadding;
    FIP->IntraRowPaddingPixels = defaultRowPadding;
    FIP->LabelLoc = LabelAbove;
    FIP->MatchStrand = StrandValues [0];
  }

  if (createImplicitGraphs && ! foundGraphFilter) {
    /* insert a Graph filter at the_end of the list */
    FIP = MemNew (sizeof (FilterItem));
    VNP = ValNodeAddPointer (&FP->FilterItemList, 0, FIP);
    if (FIP == NULL || VNP == NULL ) {
      DestroyFilter (FP, VCP);
      return NULL;
    }
    FIP->Type = GraphFilter;
    FIP->IntraRowPaddingPixels = 5;
  }

  return FP;
}

/* if this will be used by multiple threads in a multi-threaded application, there should be a lock around writing this */
static ViewerConfigsPtr newGraphicViewer_ConfigFileParse_Global = NULL;

static void InitializeDefaultStyle (
  CharPtr configFileName
);

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

  if (newGraphicViewer_ConfigFileParse_Global != NULL) {
    return newGraphicViewer_ConfigFileParse_Global;
  }

  InitializeDefaultStyle (config_filename);

  VCP = MemNew (sizeof (ViewerConfigs));
  if (VCP == NULL) return NULL;

  if (ParseConfigFile (VCP) == 0) return NULL;
  /* this should never happen, because of the static default style*/
  if (VCP->AppearanceCount == 0 || VCP->FilterCount == 0) return NULL;

  AppearanceCount = VCP->AppearanceCount;
  FilterCount = VCP->FilterCount;
  i = (AppearanceCount + FilterCount) * 2 + 2; /* total number of pointers needed (the extra 2 are NULL's to terminate the name lists) */
  ptr2 = MemNew (i * sizeof (VoidPtr));
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

/* returns count of objects successfully parsed -- so 0 on failure */
NLM_EXTERN Uint2 ParseConfigFile (
  ViewerConfigsPtr VCP
)

{
  Char     tag [32];
  Char     name [128];
  Int2     i;
  Uint2    fCount = 0, aCount = 0;
  VoidPtr  tempPtr;
  unsigned val; /* to match scanf("%ud"...) */

  GetAppParam (config_filename, "filters", "maxlabelscale", NULL, tag, sizeof (tag));
  if (sscanf (tag, "%ud", &val) != 1) {
    val = 200;
  }
  VCP->DefaultMaxScaleWithLabels = val;

  GetAppParam (config_filename, "filters", "grouppadding", NULL, tag, sizeof (tag));
  if (sscanf (tag, "%ud", &val) != 1) {
    val = 3;
  }
  VCP->DefaultGroupPadding = val;

  GetAppParam (config_filename, "filters", "rowpadding", NULL, tag, sizeof (tag));
  if (sscanf (tag, "%ud", &val) != 1) {
    val = 5;
  }
  VCP->DefaultRowPadding = val;

  GetAppParam (config_filename, "styles", "maxarrowscale", NULL, tag, sizeof (tag));
  if (sscanf (tag, "%ud", &val) != 1) {
    val = 5;
  }
  VCP->DefaultMaxScaleForArrow = val;

  GetAppParam (config_filename, "styles", "minarrowpixels", NULL, tag, sizeof (tag));
  if (sscanf (tag, "%ud", &val) != 1) {
    val = 5;
  }
  VCP->DefaultMinPixelsForArrow = val;

  VCP->DefaultShadeSmears = FALSE;
  if (GetAppParam (config_filename, "styles", "shadesmears", NULL, tag, sizeof (tag))) {
    i = StringIndexInStringList (tag, BoolStrings);
    if (i >= 0 && i < DIM (BoolValues)) {
      VCP->DefaultShadeSmears = BoolValues [i];
    }
  }

  for (i = 0; i < 110; i++) {   /* do filters first */
    if (i < 10) {
      sprintf (tag, "filter0%d", (unsigned) i);
    } else {
      sprintf (tag, "filter%d", (unsigned) i - 9);
    }
    if (GetAppParam (config_filename, "filters", tag, NULL, name, sizeof (name))) {
      tempPtr = ParseFilter (name, VCP);
      if (tempPtr == NULL) continue;
      fCount++;
    }
  }
  for (i = 0; i < 110; i++) {
    if (i < 10) {
      sprintf (tag, "style0%d", (unsigned) i);
    } else {
      sprintf (tag, "style%d", (unsigned) i - 9);
    }
    if (GetAppParam (config_filename, "styles", tag, NULL, name, sizeof (name))) {
      tempPtr = ParseAppearance (name, VCP);
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
  for (VNP = FP->FilterItemList; VNP; VNP = VNP->next) { /* free all filterItems, and their labels */
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
  --VCP->FilterCount;
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
    for (i = 0; i < APPEARANCEITEM_MAX; i++) {
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
  } else if (newFeatdef < APPEARANCEITEM_MAX) {
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

static void getDim_do_not_render (
  RenderInputPtr RIP,
  Int4Ptr Start,
  Int4Ptr Stop,
  Uint2Ptr height,
  ViewerContextPtr vContext
)

{
  RelevantFeatureItemPtr RFIP;

  RFIP = RIP->RFIP;

  *Start = *Stop = RFIP->Left;
  *height = 1;
}

static void do_not_render (
  RenderInputPtr RIP,
  ViewerContextPtr vContext
)

{
  return;
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

  if (vContext->allFeatures) {
    *Start = RFIP->Left;
    *Stop = RFIP->Right;
  } else {
    *Start = MAX (RFIP->Left, vContext->from);
    *Stop = MAX (RFIP->Right, vContext->to);
  }
  *height = 1;
}

static void render_wrap_around_markers(
  RenderInputPtr RIP,
  ViewerContextPtr vContext
)
{
  Uint4      StartY;
  RelevantFeatureItemPtr RFIP;
  AppearanceItemPtr       AIP;

  RFIP = RIP->RFIP;
  AIP = RIP->AIP;
  StartY = RIP->yStart - (RIP->featureOffset) - AIP->Height / 2 - 1;

  AddSymbol(RIP->drawSeg, RFIP->Left - 3 * vContext->viewScale,  StartY, LEFT_TRIANGLE_SYMBOL,  FALSE, MIDDLE_LEFT, 0);
  AddSymbol(RIP->drawSeg, RFIP->Right + 3 * vContext->viewScale, StartY, RIGHT_TRIANGLE_SYMBOL, FALSE, MIDDLE_RIGHT, 0);
}

static void render_with_line (
  RenderInputPtr RIP,
  ViewerContextPtr vContext
)

{
  Uint4      StartY;
  PrimitivE  thisPrim;
  RelevantFeatureItemPtr RFIP;
  AppearanceItemPtr       AIP;
  Int4       start, stop;

  RFIP = RIP->RFIP;
  AIP = RIP->AIP;

  StartY = RIP->yStart - (RIP->featureOffset) - AIP->Height / 2;
  if (vContext->allFeatures) {
    start = RFIP->Left;
    stop = RFIP->Right;
  } else {
    start = MAX (RFIP->Left, vContext->from);
    stop = MAX (RFIP->Right, vContext->to);
  }

  if (!RFIP->circularSpanningOrigin || RFIP->numivals < 2) {
    thisPrim = AddLine (RIP->drawSeg, start, StartY, stop, StartY, 0, 0);
  SetPrimitiveIDs (thisPrim, RFIP->entityID, RFIP->itemID, RFIP->itemType, 0);
  } else { 
    /* feature spans the origin. draw specially */
    if (RFIP->ivals[0] < stop) {
      thisPrim = AddLine (RIP->drawSeg, RFIP->ivals[0], StartY, stop, StartY, 0, 0); 
      SetPrimitiveIDs (thisPrim, RFIP->entityID, RFIP->itemID, RFIP->itemType, 0);
    }
    if (RFIP->ivals[2* RFIP->numivals - 1] > start) {
      thisPrim = AddLine (RIP->drawSeg, start, StartY, RFIP->ivals[2* RFIP->numivals - 1], StartY, 0, 0); 
      SetPrimitiveIDs (thisPrim, RFIP->entityID, RFIP->itemID, RFIP->itemType, 0);
    }
    render_wrap_around_markers(RIP, vContext);
  }
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


  if (vContext->allFeatures) {
    *Start = RFIP->Left;
    *Stop = RFIP->Right;
  } else {
    *Start = MAX (RFIP->Left, vContext->from);
    *Stop = MAX (RFIP->Right, vContext->to);
  }
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
  Int4                    StartY;

  StartY = RIP->yStart - (RIP->featureOffset);
  render_with_line (RIP, vContext);
  RFIP = RIP->RFIP;
  AIP = RIP->AIP;


  if (!RFIP->circularSpanningOrigin || RFIP->numivals < 2) {
    thisPrim = AddLine (RIP->drawSeg, RFIP->Left, StartY, RFIP->Left, StartY - AIP->Height, 0, 0);
    SetPrimitiveIDs (thisPrim, RFIP->entityID, RFIP->itemID, RFIP->itemType, 0);
    thisPrim = AddLine (RIP->drawSeg, RFIP->Right, StartY, RFIP->Right, StartY - AIP->Height, 0, 0);
    SetPrimitiveIDs (thisPrim, RFIP->entityID, RFIP->itemID, RFIP->itemType, 0);
  } else {
    /* origin spanning feature */
    thisPrim = AddLine (RIP->drawSeg, RFIP->ivals[0], StartY, RFIP->ivals[0], StartY - AIP->Height, 0, 0);
    SetPrimitiveIDs (thisPrim, RFIP->entityID, RFIP->itemID, RFIP->itemType, 0);
    thisPrim = AddLine (RIP->drawSeg, RFIP->ivals[2* RFIP->numivals - 1], StartY, RFIP->ivals[2* RFIP->numivals - 1], StartY - AIP->Height, 0, 0);
    SetPrimitiveIDs (thisPrim, RFIP->entityID, RFIP->itemID, RFIP->itemType, 0);    
  }
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
  if (IsInsertTic(RFIP))
    *height += AIP->Height; 
}

static Boolean IsInsertTic(RelevantFeatureItemPtr RFIP)
{
  int   iival;
  
  /* Insert Tics only on alignments with more than one segment */
  if (RFIP->featdeftype != APPEARANCEITEM_Alignment || RFIP->numivals < 2)
    return FALSE;

  for (iival = 0; iival < RFIP->numivals; ++iival) {
    /* a segment with beginning and end the same is an insert */
    if (RFIP->ivals [2 * iival] == RFIP->ivals [2 * iival + 1] ) {
      return TRUE;
    }
  }
  return FALSE;
}

#if 0 /* TestForSmearOverlap was replaced by PixelsBetweenSeqCoords everwhere it was used */
static Boolean TestForSmearOverlap (
  Int4 PrevEnd,
  Int4 NewStart,
  ViewerContextPtr vContext
)
{
  return PixelsBetweenSeqCoords(PrevEnd, NewStart, vContext->viewScale) < 1;
}
#endif

/* returns TRUE if no visible gap. */
static Boolean NoVisibleGap (
  Int4 x1,
  Int4 x2,
  Uint4 viewScale
)
{
  return abs(PixelsBetweenSeqCoords(x1, x2, viewScale)) < 2;
}

static Boolean TestForSmear (
  RelevantFeatureItemPtr RFIP1,
  RelevantFeatureItemPtr RFIP2,
  Uint4  viewScale
)
{
  Uint4  minSeperation;
  
  minSeperation = 5;  /* do not smear a feature more than 5 pixels wide */

  if (abs(PixelsBetweenSeqCoords(RFIP1->Right, RFIP1->Left, viewScale)) >= minSeperation) return FALSE;
  if (abs(PixelsBetweenSeqCoords(RFIP2->Right, RFIP2->Left, viewScale)) >= minSeperation) return FALSE;

  return (NoVisibleGap (RFIP1->Right, RFIP2->Left, viewScale)
          || NoVisibleGap (RFIP1->Right, RFIP2->Right, viewScale)
          || NoVisibleGap (RFIP1->Left, RFIP2->Right, viewScale)
          || NoVisibleGap (RFIP1->Left, RFIP2->Left, viewScale) );

}

static Int4 PixelsBetweenSeqCoords(Int4 left, Int4 right, Uint4 viewScale)
{
  /* Will be negative if right is really not right of left. */
  /* 0 if they fall on the same pixel. */
  /* Do NOT change this to (right - left)/viewScale. NOT the same. */
  return (right/viewScale - left/viewScale);
}

static void render_with_box_master (
  RenderInputPtr RIP,
  Boolean fillBox,
  ViewerContextPtr vContext
)

{
  Int4                    StartY;
  Uint2                   pieceIValStart;
  PrimitivE               thisPrim;
  Uint2                   i;
  Int4                    mid;
  AppearanceItemPtr       AIP;
  AppearancePtr           AP;
  RelevantFeatureItemPtr  RFIP;
  Boolean                 shade_p;
  Uint1                   arrow;

  RFIP = RIP->RFIP;
  AIP = RIP->AIP;
  AP = vContext->AppPtr;
  StartY = RIP->yStart - (RIP->featureOffset);
  if (RFIP->LeftEnd == EndClipped) {
    thisPrim = AddLine (RIP->drawSeg, vContext->from, StartY - AIP->Height / 2, RFIP->Left, StartY - AIP->Height / 2, 0, 0);
    SetPrimitiveIDs (thisPrim, RFIP->entityID, RFIP->itemID, RFIP->itemType, 0);
  }
  if (RFIP->RightEnd == EndClipped) {
    thisPrim = AddLine (RIP->drawSeg, vContext->to, StartY - AIP->Height / 2, RFIP->Right, StartY - AIP->Height / 2, 0, 0);
    SetPrimitiveIDs (thisPrim, RFIP->entityID, RFIP->itemID, RFIP->itemType, 0);
  }
  if (RFIP->numivals == 1) {
    shade_p = (RFIP->entityID == 0 && RFIP->itemID == 0) ? AP->ShadeSmears : FALSE;
    if (RFIP->entityID == 0 && RFIP->itemID == 0) { /* is this a multi-feature smear? */
      if (shade_p) {
        /*        AddAttribute (RIP->drawSeg, SHADING_ATT, 0, 0, MEDIUM_SHADING, 0, 0);*/
      }
      thisPrim = AddRectangle (RIP->drawSeg, RFIP->Left, StartY, RFIP->Right, StartY - AIP->Height, NO_ARROW, fillBox, 0);
      SetPrimitiveIDs (thisPrim, RFIP->entityID, RFIP->itemID, RFIP->itemType, 0);
      if (shade_p) {
        /*        AddAttribute (RIP->drawSeg, SHADING_ATT, 0, 0, NO_SHADING, 0, 0);*/
      }

    } else { /* nope */
      arrow = NO_ARROW;
      if (RFIP->featstrand == Seq_strand_plus) {
        arrow = RIGHT_ARROW;
      } else if (RFIP->featstrand == Seq_strand_minus) {
        arrow = LEFT_ARROW;
      }
      if (! AIP->ShowArrow) {
        arrow = NO_ARROW;
      }
      if (abs(PixelsBetweenSeqCoords( RFIP->Left, RFIP->Right, vContext->viewScale)) < AP->MinPixelsForArrow) {
        arrow = NO_ARROW;
      }
      if (vContext->viewScale > AP->MaxScaleForArrow) {
        arrow = NO_ARROW;
      }
      thisPrim = AddRectangle (RIP->drawSeg, RFIP->Left, StartY, RFIP->Right, StartY - AIP->Height, arrow, fillBox, 0);
      SetPrimitiveIDs (thisPrim, RFIP->entityID, RFIP->itemID, RFIP->itemType, 0);
    }
    return;
  } else {
    i = 0;
    while (i < RFIP->numivals) {
      /* collect a group of interval(s) which do not contain any pixels between them */
      pieceIValStart = i;
      /* this tests is the i-plus-1th feature is part of the smear, which goes from pieceIValStart to i, inclusive */
      /* On alignments only smear away the gaps caused by inserts. */
      if (RFIP->featdeftype == APPEARANCEITEM_Alignment) {
        while (i + 1 < RFIP->numivals &&
               NoVisibleGap (RFIP->ivals [2 * i + 1], RFIP->ivals [2 * i + 2], vContext->viewScale) &&
               (RFIP->ivals [2 * i] == RFIP->ivals [2 * i + 1]  ||  RFIP->ivals [2 * i + 2] == RFIP->ivals [2 * i + 3])) {
          i++;
        }
      } else {
        while (i + 1 < RFIP->numivals &&
               NoVisibleGap (RFIP->ivals [2 * i + 1], RFIP->ivals [2 * i + 2], vContext->viewScale)) {
          i++;
        }
      }

      /* draw the segment and the gap -- drawing the gap first, so that it is overdrawn by the segment */
      if (i + 1 < RFIP->numivals) { /* a gap is present if there are more ivals to consider after i*/
        if (RFIP->circularSpanningOrigin && RFIP->ivals [2 * i + 1] > RFIP->ivals [2 * i + 2]) {
          /* on a circular bioseq, this gap spans the origin and has to be drawn specially */
          /* RFIP->Left will be 0, RFIP->Right will be the length of the Bioseq */
          if (RFIP->ivals [2 * i + 1] < RFIP->Right - 1) {
            /* Draw the part of the gap at the right end. Might not be any. */
            thisPrim = AddLine (RIP->drawSeg, RFIP->ivals[2 * i + 1], StartY - AIP->Height / 2, RFIP->Right, StartY - AIP->Height / 2, 0, 0);
            SetPrimitiveIDs (thisPrim, RFIP->entityID, RFIP->itemID, RFIP->itemType, 0);
          }
          if (RFIP->Left <  RFIP->ivals [2 * i + 2]) {
            /* draw the part of the gap at the left end. */
            thisPrim = AddLine (RIP->drawSeg, RFIP->Left, StartY - AIP->Height / 2, RFIP->ivals[2 * i + 2], StartY - AIP->Height / 2, 0, 0);
            SetPrimitiveIDs (thisPrim, RFIP->entityID, RFIP->itemID, RFIP->itemType, 0);
          }
          /* No. I didn't put in code to make those lines 'Angle Gap' style. Figured it wouldn't show up very well. You can if you want. */
          
          /* draw markers to show that this wraps around */
          render_wrap_around_markers(RIP, vContext);

        } else {
          /* ordinary gap */
          if (AIP->GapChoice == LineGap) {
            thisPrim = AddLine (RIP->drawSeg, RFIP->ivals [2 * i + 1],                       StartY - AIP->Height / 2, 
                                              RFIP->ivals [2 * i + 2] - vContext->viewScale, StartY - AIP->Height / 2, 0, 0);
            SetPrimitiveIDs (thisPrim, RFIP->entityID, RFIP->itemID, RFIP->itemType, 0);
          } else if (AIP->GapChoice == AngleGap) {
            mid = (RFIP->ivals [2 * i + 2] + RFIP->ivals [2 * i + 1]) / 2;
            thisPrim = AddLine (RIP->drawSeg, RFIP->ivals [2 * i + 1], StartY - AIP->Height / 2, 
                                              mid,                     StartY, 0, 0);
            SetPrimitiveIDs (thisPrim, RFIP->entityID, RFIP->itemID, RFIP->itemType, 0);
            thisPrim = AddLine (RIP->drawSeg, mid,                                           StartY, 
                                              RFIP->ivals [2 * i + 2] - vContext->viewScale, StartY - AIP->Height / 2, 0, 0);
            SetPrimitiveIDs (thisPrim, RFIP->entityID, RFIP->itemID, RFIP->itemType, 0);
          }
        }
      }
      arrow = NO_ARROW;
      if (i == RFIP->numivals - 1  &&  RFIP->featstrand == Seq_strand_plus) {
        arrow = RIGHT_ARROW;
      } else if (RFIP->featstrand == Seq_strand_minus) {
        /* on minus strands, Alignments put their intervals in coordinate order.
          Other features put theirs in biological order (right to  left),
          so we have to draw the arrow differently (or we could sort the intervals so they 
          ended up the same. */
        if ((pieceIValStart == 0 && RFIP->featdeftype == APPEARANCEITEM_Alignment) ||
          (i == RFIP->numivals - 1  &&   RFIP->featdeftype != APPEARANCEITEM_Alignment))
          arrow = LEFT_ARROW;
      }
      if (! AIP->ShowArrow) {
        arrow = NO_ARROW;
      }
      if (ABS (RFIP->Right - RFIP->Left) / vContext->viewScale < AP->MinPixelsForArrow) {
        arrow = NO_ARROW;
      }
      if (vContext->viewScale > AP->MaxScaleForArrow) {
        arrow = NO_ARROW;
      }
      thisPrim = AddRectangle (RIP->drawSeg, RFIP->ivals [2 * pieceIValStart], StartY, RFIP->ivals [2 * i + 1], StartY - AIP->Height, arrow, fillBox, 0);
      SetPrimitiveIDs (thisPrim, RFIP->entityID, RFIP->itemID, RFIP->itemType, 0);
      i++;
    }
    if (RFIP->featdeftype == APPEARANCEITEM_Alignment) {
      render_insert_tics(RIP);
    }
  }
}


static void render_insert_tics(RenderInputPtr RIP)
{
  AppearanceItemPtr       AIP;
  RelevantFeatureItemPtr  RFIP;
  PrimitivE               thisPrim;
  Int4                    StartY;
  Int4                    i;

  RFIP = RIP->RFIP;
  AIP = RIP->AIP;
  StartY = RIP->yStart - (RIP->featureOffset);

  /* if we are drawing an aligment, and the interval is zero length, It is an insert. 
     Draw a vertical line the height of the bar under the bar to show this.  */
  for (i = 0; i < RFIP->numivals; ++i) {
    /* a segment with beginning and end the same is an insert */
    if (RFIP->ivals [2 * i] == RFIP->ivals [2 * i + 1] ) {
      thisPrim = AddLine(RIP->drawSeg, RFIP->ivals [2 * i], StartY - AIP->Height, 
                                       RFIP->ivals [2 * i], StartY - AIP->Height * 2, FALSE, 0);
      SetPrimitiveIDs(thisPrim, RFIP->entityID, RFIP->itemID, RFIP->itemType, 0);
    }
  }
}


static void render_with_box (
  RenderInputPtr   RIP,
  ViewerContextPtr vContext
)

{
  render_with_box_master (RIP, TRUE, vContext);

}

static void render_with_outline_box (
  RenderInputPtr   RIP,
  ViewerContextPtr vContext
)

{
  render_with_box_master (RIP, FALSE, vContext);
}

static const RenderClass RenderAlgorithmTable [] = {
  {do_not_render, getDim_do_not_render}, /* do_not_render */
  {render_with_line, getDim_render_with_line}, /* Render_Line */
  {render_with_capped_line, getDim_render_with_capped_line}, /* Render_CappedLine */
  {render_with_box, getDim_render_with_box},   /* Render_Box */
  {render_with_outline_box, getDim_render_with_box},   /* Render_OutlineBox */
  /* !!! these do not exist right now - can they be removed? !!! */
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
  FilterItemPtr           FIP;
  FilterPtr               FP;
  CharPtr                 label;
  Char                    tempStringBuffer [256];
  Char                    shortLabel [41];
  Uint1                   stringFlags;
  Uint4                   textWidthBP;
  Int4                    textStartX;
  Int4                    textStartY;
  Uint1                   labelAlign, fitChars;
  PrimitivE               thisPrim;
  Boolean                 addType;
  Boolean                 addDesc;

  FIP = RIP->FIP;
  RFIP = RIP->RFIP;
  AIP = RIP->AIP;
  FP = vContext->FltPtr;
  addType = AIP->AddTypeToLabel;
  addDesc = AIP->AddDescToLabel;
  if (FIP->AddTypeToLabel != TristateUnset) {
    addType = BOOL_FROM_SET_TRISTATE (FIP->AddTypeToLabel);
  }
  if (FIP->AddDescToLabel != TristateUnset) {
    addDesc = BOOL_FROM_SET_TRISTATE (FIP->AddDescToLabel);
  }

  /*  RIP->drawSeg = CreateSegment (RIP->drawSeg, 0, 0);*/
  /* Place each feature in its own segment.  This is not necessary for simple features
     but perhaps the inefficiency is acceptable because it simplifies the algorithm. */
  (*RenderAlgorithmTable [AIP->RenderChoice].RenderFunc) (RIP, vContext);
  if (FIP->LabelLoc == LabelNone) return;
  if (vContext->viewScale > FP->MaxScaleWithLabels) return;
  stringFlags = 0;
  if (addDesc && RFIP->ContentLabel != NULL) {
    stringFlags |= 1;
  }
  if (addType) {
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
      label = FindFeatStrFromFeatDefType (RFIP->featdeftype);
      break;
    case 3:                    /*add both */
      if (StringCmp (FindFeatStrFromFeatDefType (RFIP->featdeftype), RFIP->ContentLabel) != 0) {
        sprintf (tempStringBuffer, "%s: %s", FindFeatStrFromFeatDefType (RFIP->featdeftype), RFIP->ContentLabel);
        label = tempStringBuffer;
      } else {
        label = RFIP->ContentLabel;
      }
      break;
  }
  if (StringHasNoText (label)) return;
  LabelCopy (shortLabel, label, sizeof (shortLabel));
  SelectFont (AIP->LabelFont);
  textWidthBP = StringWidth (shortLabel) * vContext->viewScale;
  switch (FIP->LabelLoc) {
    case LabelAboveClip:
      if (textWidthBP + 2 * vContext->viewScale >= (RFIP->Right - RFIP->Left)) {
        fitChars = Nlm_FitStringWidth (shortLabel, (RFIP->Right - RFIP->Left) / vContext->viewScale);
        if (fitChars <= 2 && StringLen (shortLabel) != fitChars) return;
        shortLabel [fitChars] = '>';
        shortLabel [fitChars + 1] = '\0';
      }
      textStartX = (RFIP->Left + RFIP->Right) / 2;
      /*      textStartY = RIP->yStart - RIP->rowHeight / 2; -- change "labelInside" to mean "above, but not wider than"*/
      textStartY = RIP->yStart;
      labelAlign = LOWER_CENTER;
      break;
    case LabelInside:
      if (textWidthBP + 2 * vContext->viewScale >= (RFIP->Right - RFIP->Left)) {
        fitChars = Nlm_FitStringWidth (shortLabel, (RFIP->Right - RFIP->Left) / vContext->viewScale);
        shortLabel [fitChars + 1] = '\0';
      }
      textStartX = (RFIP->Left + RFIP->Right) / 2;
      textStartY = RIP->yStart - RIP->rowHeight / 2;
      labelAlign = MIDDLE_CENTER;
      break;
    case LabelAboveCull:
      if (textWidthBP + 2 * vContext->viewScale >= (RFIP->Right - RFIP->Left)) return;
      /* else fall through and display the label above */
    case LabelAbove:
      textStartX = (RFIP->Left + RFIP->Right) / 2;
      textStartY = RIP->yStart;
      labelAlign = LOWER_CENTER;
      break;
    case LabelBelow:
      textStartY = RIP->yStart - RIP->Height;
      textStartX = (RFIP->Left + RFIP->Right) / 2;
      labelAlign = LOWER_CENTER;
      break;
    case LabelLeft:
      textStartX = (RFIP->Left);
      textStartY = (RIP->yStart);
      labelAlign = LOWER_LEFT;
      break;
    case LabelRight:
      textStartX = (RFIP->Right);
      textStartY = (RIP->yStart);
      labelAlign = LOWER_RIGHT;
      break;
    default:
      return;
  }
  thisPrim = AddTextLabel (RIP->labelSeg, textStartX, textStartY, shortLabel, AIP->LabelFont, 1, labelAlign, 0);
  SetPrimitiveIDs (thisPrim, RFIP->entityID, RFIP->itemID, RFIP->itemType, 0);
}

static RelevantFeatureItemPtr BuildClippedRFIP (
  RelevantFeatureItemPtr inputRFIP,
  FilterProcessStatePtr FPSP,
  ViewerContextPtr vContext
)

{
  RelevantFeatureItemPtr newRFIP;
  ValNodePtr VNP = NULL;
  Uint2 i, newnumivals;
  Int4  from, to;
  Boolean useThis = TRUE, useLast;
  Boolean clippedLeft, clippedRight;
  Boolean lastClippedLeft, lastClippedRight;

  if ((newRFIP = MemNew (sizeof (RelevantFeatureItem))) == NULL) return NULL;
  if ((VNP = MemNew (sizeof (ValNode))) == NULL) {
    MemFree (newRFIP);
    return NULL;
  }
  VNP->data.ptrvalue = newRFIP;
  VNP->next = FPSP->needFreeList;
  FPSP->needFreeList = VNP;
  MemCopy (newRFIP, inputRFIP, sizeof (RelevantFeatureItem));
  if ((inputRFIP->Left <= vContext->from && inputRFIP->Right <= vContext->from)
      || (inputRFIP->Left >= vContext->to && inputRFIP->Right >= vContext->to)) {
    return NULL; /* entire feature removed by clipping */
  }
  newRFIP->LeftEnd = (inputRFIP->Left >= vContext->from) ? inputRFIP->LeftEnd : EndPartial;
  newRFIP->RightEnd  = (inputRFIP->Right <= vContext->to) ?  inputRFIP->RightEnd  : EndPartial;
  if (inputRFIP->numivals == 1) {
    newRFIP->Left  = MAX (vContext->from, MIN (vContext->to, inputRFIP->Left ));
    newRFIP->Right = MAX (vContext->from, MIN (vContext->to, inputRFIP->Right));
    newRFIP->LeftEnd = (inputRFIP->Left >= vContext->from) ? inputRFIP->LeftEnd : EndPartial;
    newRFIP->RightEnd  = (inputRFIP->Right <= vContext->to) ?  inputRFIP->RightEnd  : EndPartial;
  } else {
    newRFIP->Left = vContext->to;
    newRFIP->Right = vContext->from;
    newnumivals = 0;
    for (i = 0; i < inputRFIP->numivals; i++) {
      from = inputRFIP->ivals [2 * i];
      to = inputRFIP->ivals [2 * i + 1];
      if (from <= vContext->to && from >= vContext->from) {
        newnumivals ++;
      } else if (to <=  vContext->to && to >= vContext->from) {
        newnumivals ++;
      }
    }
    newRFIP->numivals = newnumivals;
    if ((newRFIP->ivals = MemNew (2 * newnumivals * sizeof (Int4))) == NULL) return NULL;
    if ((VNP = MemNew (sizeof (ValNode))) == NULL) {
      MemFree (newRFIP->ivals);
      return NULL;
    }
    VNP->data.ptrvalue = newRFIP->ivals;
    VNP->next = FPSP->needFreeList;
    FPSP->needFreeList = VNP;
    newnumivals = 0;
    for (i = 0; i < inputRFIP->numivals; i++) {
      from = inputRFIP->ivals [2 * i];
      to = inputRFIP->ivals [2 * i + 1];
      if (from <= vContext->to && from >= vContext->from) {
        useThis = TRUE;
        clippedLeft = clippedRight = FALSE;
      } else if (to <=  vContext->to && to >= vContext->from) {
        useThis = TRUE;
        clippedLeft = clippedRight = FALSE;
      } else {
        useThis = FALSE;
        clippedLeft = ((from + to) < (vContext->from + vContext->to));
        clippedRight = !clippedLeft;
      }
      if (i == 0) {
        useLast = useThis;
        lastClippedLeft = clippedLeft;
        lastClippedRight = clippedRight;
      }
      if (lastClippedLeft && useThis) {
        newRFIP->LeftEnd  = EndClipped;
      } else if (lastClippedRight && useThis) {
        newRFIP->RightEnd = EndClipped;
      } else if (useLast && clippedLeft) {
        newRFIP->LeftEnd  = EndClipped;
      } else if (useLast && clippedRight) {
        newRFIP->RightEnd = EndClipped;
      }
      if (useThis) {
        from = MAX (vContext->from, MIN (vContext->to, from));
        to   = MAX (vContext->from, MIN (vContext->to, to  ));
        newRFIP->Left = MIN (newRFIP->Left, from);
        newRFIP->Left = MIN (newRFIP->Left, to);
        newRFIP->Right = MAX (newRFIP->Right, to);
        newRFIP->Right = MAX (newRFIP->Right, from);
        newRFIP->ivals [newnumivals++] = from;
        newRFIP->ivals [newnumivals++] = to;
      }
      useLast = useThis;
    }
  }
  if (newRFIP->Left > newRFIP->Right) return NULL;
  return newRFIP;
}


static void GetFeatureAndDecorationDimensions (
  RenderInputPtr RIP,
  ViewerContextPtr vContext
)

{
  AppearanceItemPtr       AIP;
  RelevantFeatureItemPtr  RFIP;
  FilterItemPtr           FIP;
  FilterPtr               FP;
  Int4                    Start, Stop;
  Uint2                   Height;
  Int2                    lineHeight;
  Int4                    textStartX;
  Uint4                   textWidthBP;
  CharPtr                 label = NULL;
  Char                    tempStringBuffer [256];
  Uint1                   stringFlags;
  Uint2                   featureOffset = 0;
  Boolean                 addType;
  Boolean                 addDesc;

  RFIP = RIP->RFIP;
  AIP = RIP->AIP;
  FIP = RIP->FIP;
  FP = vContext->FltPtr;
  addType = AIP->AddTypeToLabel;
  addDesc = AIP->AddDescToLabel;
  if (FIP->AddTypeToLabel != TristateUnset) {
    addType = BOOL_FROM_SET_TRISTATE (FIP->AddTypeToLabel);
  }
  if (FIP->AddDescToLabel != TristateUnset) {
    addDesc = BOOL_FROM_SET_TRISTATE (FIP->AddDescToLabel);
  }

  (*RenderAlgorithmTable [AIP->RenderChoice].GetDimensions) (RIP, &Start, &Stop, &Height, vContext);
  RIP->Height = Height;
  RIP->decorationHeight = Height;
  RIP->decorationLeft = RFIP->Left;
  RIP->decorationRight = RFIP->Right;
  RIP->featureOffset = 0;

  if (FIP->LabelLoc != LabelNone && vContext->viewScale <= FP->MaxScaleWithLabels) {
    stringFlags = 0;
    if (addDesc && RFIP->ContentLabel != NULL) {
      stringFlags |= 1;
    }
    if (addType) {
      stringFlags |= 2;
    }

    switch (stringFlags) {
      case 0:                  /* no label */
        break;
      case 1:                  /*comment but not type */
        label = RFIP->ContentLabel;
        break;
      case 2:                  /*only type */
        label = FindFeatStrFromFeatDefType (RFIP->featdeftype);
        break;
      case 3:                  /*add both */
        if (StringCmp (FindFeatStrFromFeatDefType (RFIP->featdeftype), RFIP->ContentLabel) != 0) {
          sprintf (tempStringBuffer, "%s: %s", FindFeatStrFromFeatDefType (RFIP->featdeftype), RFIP->ContentLabel);
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
    switch (FIP->LabelLoc) {
      case LabelInside:
        Height = MAX (Height + lineHeight, Height);
        featureOffset = lineHeight + 1;
        break;
      case LabelAbove:
        textStartX = (Start + Stop) / 2;
        Start = MIN (Start, (signed)(textStartX - textWidthBP / 2));
        Stop = MAX (Stop, textStartX + textWidthBP / 2);
        featureOffset = lineHeight + 1;
        Height += lineHeight + 3;
        break;
      case LabelAboveClip:
        Height += lineHeight + 3;
        featureOffset = lineHeight + 1;
      case LabelAboveCull:
        if (textWidthBP + 2 * vContext->viewScale >= (Stop - Start)) {
          Height += lineHeight + 3;
          featureOffset = lineHeight + 1;
        }
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
  RIP->decorationLeft = Start;
  RIP->decorationRight = Stop;
  RIP->decorationHeight = Height;
  RIP->featureOffset = featureOffset;
}


static Boolean BuildRenderInputFromRFIP (
  RenderInputPtr target,
  RelevantFeatureItemPtr RFIP,
  FilterProcessStatePtr FPSP
)

{
  AppearancePtr    AppPtr;
  FilterItemPtr    currentFIP;
  ViewerContextPtr vContext;
  RelevantFeatureItemPtr newRFIP;

  vContext = FPSP->vContext;

  if (target == NULL || RFIP == NULL) return FALSE;
  if (!vContext->allFeatures &&
      (
       (RFIP->Left < vContext->from && RFIP->Right < vContext->from)
       || (RFIP->Left > vContext->to && RFIP->Right > vContext->to)
       )) {
    return FALSE; /* this feature is outside the clipping seqloc */
  }
  if (! vContext->allFeatures && (
      (RFIP->Right >= vContext->from || RFIP->Left <= vContext->to)
      || (RFIP->Right < vContext->from && RFIP->Left > vContext->to)
      )) {
    newRFIP = BuildClippedRFIP (RFIP, FPSP, vContext);
    if (newRFIP == NULL) return FALSE;
    RFIP = newRFIP;
  }
  target->RFIP = RFIP;
  target->labelSeg = FPSP->labelSegs [RFIP->featdeftype];
  target->drawSeg = FPSP->drawSegs [RFIP->featdeftype];
  AppPtr = vContext->AppPtr;
  currentFIP = FPSP->currentFIP;
  target->AIP = AppPtr->FeaturesAppearanceItem [RFIP->featdeftype];
  target->FIP = FPSP->currentFIP;
  GetFeatureAndDecorationDimensions (target, vContext);
  target->rowHeight = MAX (target->Height, target->decorationHeight) + currentFIP->IntraRowPaddingPixels;
  return TRUE;
}

/* If this is a named Seq Annotation, return its name. Otherwise, return NULL */
static CharPtr GetSeqAnnotName(SeqAnnotPtr sap)
{
  if (sap != NULL && sap->desc != NULL)
  {
    AnnotDescrPtr  descPtr;
    /* look for a 'name' record */
    for (descPtr = sap->desc; descPtr != NULL; descPtr = descPtr->next)
    {
      if (Annot_descr_name == descPtr->choice) /* name choice */
        return descPtr->data.ptrvalue;
    }
    /* if an alignment look for a Blast Type or a Hist Seqalign user object */
  	if(sap->type ==2) /* not an alignment annotation */
  	{
    	UserObjectPtr uop;
    	ObjectIdPtr oip;
    	UserFieldPtr ufp;
 
    	for (descPtr = sap->desc; descPtr; descPtr = descPtr->next)
    	{
    		if (Annot_descr_user == descPtr->choice)
    		{
    			
    			for (uop = descPtr->data.ptrvalue; uop; uop = uop->next)
    			{
    				if (uop->type)
    				{
    					oip = uop->type;
    					
    					if (StringCmp(oip->str, "Blast Type") == 0)
    					{
    						ufp = uop->data;
    						if (ufp && ufp->choice == 2)
    						{
    							oip = ufp->label;
    							if (oip && oip->str)
    							{
    							  return oip->str;
    							}
    						}
    					}	
    				}	
    			}
    		}
    	}
    	for (descPtr = sap->desc; descPtr; descPtr = descPtr->next)
    	{
    		if (Annot_descr_user == descPtr->choice)
    		{
    			
    			for (uop = descPtr->data.ptrvalue; uop; uop = uop->next)
    			{
    				if (uop->type)
    				{
    					oip = uop->type;
    					
    					if (StringCmp(oip->str, "Hist Seqalign") == 0)
    					{
    						ufp = uop->data;
    						if (ufp && ufp->choice == 4 && ufp->data.boolvalue)
    						{
    							oip = ufp->label;
    							if (oip && oip->str)
    							{
    								return oip->str;
    							}
    						}
    					}
    				}	
    			}
    		}
    	}
  	}
  }
  return NULL;
}

static Boolean GetAndCountFeatures (
  ViewerContextPtr vContext
)

{
  SeqFeatPtr              sfp;
  SeqMgrFeatContext       fContext;
  RelevantFeatureItemPtr  rFeats;
  ValNodePtr              sapList = NULL, VNP, VNPtail;
  SeqAnnotPtr             SAnnP;
  Uint2                   i;
  Int4                    swap;

  if (vContext == NULL) return FALSE;
  vContext->sapCount = 0;
  vContext->featureCount = 0;
  vContext->featVNP = NULL;
  vContext->sapList = NULL;

  /* create list of all named SeqAnnot's in this BioSeq. */
  for (SAnnP = vContext->BSP->annot; SAnnP != NULL; SAnnP = SAnnP->next) 
  {
    if (GetSeqAnnotName(SAnnP)) {
      vContext->sapCount++;
      ValNodeAddPointer (&sapList, 0, SAnnP);
    }
  }
  if (vContext->sapCount > 0) {
    vContext->sapList = MemNew (vContext->sapCount * sizeof (SeqAnnotPtr));
    if (vContext->sapList == NULL) {
      ValNodeFree (sapList);
      return FALSE;
    }
    for (i = 0, VNP = sapList; VNP != NULL && i < vContext->sapCount; VNP = VNP->next, i++) {
      vContext->sapList[i] = VNP->data.ptrvalue;
    }
    ValNodeFree (sapList);
  }
  
  rFeats = MemNew (RELEVANT_FEATS_PER_CHUNK * sizeof (RelevantFeatureItem));
  if (rFeats == NULL) return FALSE;
  ValNodeAddPointer (&vContext->featVNP, 0, rFeats);
  VNPtail = vContext->featVNP;
  i = 0;
  sfp = SeqMgrGetNextFeature (vContext->BSP, NULL, 0, 0, &fContext);
  while (sfp != NULL) {
    vContext->featureCount++;

    rFeats [i].Left = fContext.left;
    rFeats [i].Right = fContext.right;
    rFeats [i].LeftEnd = fContext.partialL ? EndPartial : EndAbsolute;
    rFeats [i].RightEnd  = fContext.partialR ? EndPartial : EndAbsolute;
    rFeats [i].ContentLabel = fContext.label;
    rFeats [i].featdeftype = fContext.featdeftype;
    rFeats [i].entityID = fContext.entityID;
    rFeats [i].itemID = fContext.itemID;
    rFeats [i].itemType = OBJ_SEQFEAT;
    rFeats [i].numivals = fContext.numivals;
    rFeats [i].ivals = fContext.ivals;
    rFeats [i].featstrand = fContext.strand;
    rFeats [i].circularSpanningOrigin = FALSE;
    if (rFeats [i].Left < 0 && fContext.bsp != NULL) {
      /* !!! for features that span origin JK !!! */
      rFeats [i].circularSpanningOrigin = TRUE;
      rFeats [i].Left = 0;
      rFeats [i].Right = fContext.bsp->length;
    }
    if (rFeats [i].Right < rFeats [i].Left) {
      /* protection against (feature indexing vs. trans-spliced features) */
      swap = rFeats [i].Right;
      rFeats [i].Right = rFeats [i].Left;
      rFeats [i].Left = swap;
    }
    /* with trans-spliced genes the left and right values might not span all the intervals. */
    /* we will fix that */
    {
      int ivali;
      Int4  val;
      for (ivali = 0; ivali < rFeats [i].numivals; ++ivali) {
        val = rFeats[i].ivals[2*ivali];
        rFeats[i].Left  = MIN( rFeats[i].Left,  val );
        rFeats[i].Right = MAX( rFeats[i].Right, val );
        val = rFeats[i].ivals[2*ivali+1];
        rFeats[i].Left  = MIN( rFeats[i].Left,  val );
        rFeats[i].Right = MAX( rFeats[i].Right, val );
      }
    }
    if (GetSeqAnnotName(fContext.sap)) { /* save this feature's Annot Ptr if it is a named Seq Annot. */
      rFeats [i].sap = fContext.sap;
    }
    else {
      rFeats [i].sap = NULL;
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
  if (vContext->featureCount == 0) {
    MemFree (rFeats);
    MemFree (vContext->featVNP);
    vContext->featVNP = NULL;
  }
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
  SegmenT parentSegment
)

{
  AppearancePtr     AppPtr;
  AppearanceItemPtr AIP;
  ViewerContextPtr vContext;

  vContext = FPSP->vContext;


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
    AddAttribute (FPSP->labelSegs [featdeftype], COLOR_ATT, AIP->LabelColor, 0, 0, 0, 0);
  }
  return TRUE;
}

/*
  return a string describing the sequences in this alignment,
  concatenating strings from all the seqid's of the sequences in this alignment 
  except for the one in 'notthisRow' which ordinarly will be the bioseq.
*/

static Boolean SeqAlignContentLabel(SeqAlignPtr sap, SeqIdPtr notThisSID, CharPtr buf, Int4 buflen, Uint1 format)
{
  Int4      r, rows, slen;
  Char      localbuf[100];
  SeqIdPtr  sid;
  Char      rowDelim[] = ",";
  Int4      rowDelimLen = sizeof(rowDelim) - 1;
  Boolean   firstTime = TRUE;
  
  if (sap == NULL || buf == NULL) return FALSE;
  
  rows = AlnMgr2GetNumRows(sap);
  if (rows < 1)
    rows = sap->dim;
  if (rows < 1)
    return FALSE;
 
  for (r = 1; r <= rows; ++r)
  {
    sid = AlnMgr2GetNthSeqIdPtr(sap, r);
    if (sid == NULL) {
     sid = AlnMgr2GetNthSeqIdPtrStdSeg(sap, r);
    }
    if (sid == NULL) 
      continue;
    if (SeqIdIn(sid, notThisSID))
      continue;

    SeqIdWrite (sid, localbuf, format, sizeof (localbuf) - 1);
    slen = StringLen(localbuf);
    if (slen < buflen - rowDelimLen - 1)
    {
      if (!firstTime) {
        StringNCat(buf, rowDelim, buflen);
        buflen -= sizeof(rowDelim) - 1;
      }
      firstTime = FALSE;
      StringNCat(buf, localbuf, buflen);
      buflen -= slen + 1;
     }
  }
  
  if (StringLen(buf) <= 0)
    return FALSE;
  return TRUE;
}

#define _DEBUG

static void AccumIvals(Int1* accumulator, Int4 accumBegin, Int4 accumLen, RelevantFeatureItemPtr RFIP);
Int4  CountIvals(Int1* accumulator, Int4 accumLen);
void  MakeIvals(Int1* accumulator, Int4 accumBegin, Int4 accumLen, Int4Ptr ivalsOut, Int4 ivalsLen);

static RelevantFeatureItemPtr GetNextRFIPinAlignmentFilter (
  FilterProcessStatePtr FPSP
)

{
    AlignmentFilterStatePtr alignSP;
    SeqAlignSortInfoPtr     alignSorted;
    SeqAlignPtr             SAlnP;
    Int4                    start, stop;
    Uint1                   segType;
    Int4                    nsegs, i;
    SeqIdPtr                SID;
    BioseqPtr               BSP;
    RelevantFeatureItem     RFI;       /* holder for intermediate values we will merge together */
    RelevantFeatureItemPtr  finalRFIP;  /* Our return value. */
    ViewerContextPtr        vContext;
    FilterItemPtr           currentFIP;
    Boolean                 stdSeg = FALSE;
    AppearanceItemPtr       AIP;
    Char                    labelbuf[150];
    Uint1                   strand;
    AlignSegIterator        asi;
    ValNodePtr              vnp;
    
    if (FPSP == NULL) return NULL;
    vContext = FPSP->vContext;
    currentFIP = FPSP->currentFIP;
    if (vContext == NULL || currentFIP == NULL) return NULL;

    BSP = vContext->BSP;
    SID = BSP->id;
    AIP = vContext->AppPtr->FeaturesAppearanceItem[APPEARANCEITEM_Alignment];

    
    alignSP = &FPSP->state.align;
    alignSorted = alignSP->alignSorted;

    SAlnP = alignSorted[alignSP->alignIndex].sap;

    /* create new RFIP based on this SeqAlignPtr */
    finalRFIP = MemNew (sizeof (RelevantFeatureItem));
    if (finalRFIP == NULL) {
        MemFree (finalRFIP);
        return NULL;
    }
    vnp = ValNodeAddPointer (&FPSP->lastInFreeList, 0, finalRFIP);
    if (vnp == NULL) {
        MemFree (finalRFIP);
        return NULL;
    }
    if (FPSP->needFreeList == NULL) {
      FPSP->needFreeList = FPSP->lastInFreeList;
    }
    FPSP->lastInFreeList = vnp;

   /* where does this alignment start & stop in bioseq coords? */
    nsegs = AlignSegIteratorCreate(SAlnP, SID, &asi);
    strand = asi.strand;
    if (strand == Seq_strand_unknown)
        strand = Seq_strand_plus;

    if (asi.start < 0 || asi.stop < 0) 
      return NULL;
    finalRFIP->Left = asi.start;
    finalRFIP->Right = asi.stop;
    finalRFIP->featstrand = strand;
    finalRFIP->numivals = 1;
    finalRFIP->ivals = NULL;
    finalRFIP->featdeftype = APPEARANCEITEM_Alignment;
    finalRFIP->circularSpanningOrigin = FALSE;
    finalRFIP->entityID = SAlnP->idx.entityID;
    finalRFIP->itemID = SAlnP->idx.itemID;
    finalRFIP->itemType = SAlnP->idx.itemtype;
    labelbuf[0] = 0;
    if (SeqAlignContentLabel(SAlnP, SID, labelbuf, sizeof(labelbuf) - 1, AIP->format))
    {
        finalRFIP->ContentLabel = StringSave(labelbuf);
    }

    if (nsegs > 1) {
        finalRFIP->ivals = MemNew (2 * nsegs * sizeof (Int4));
        if (finalRFIP->ivals == NULL) 
        {
            MemFree(finalRFIP->ContentLabel);
            return NULL;
        } else {
            finalRFIP->numivals = 0;
            i = 0;
            while (AlignSegIteratorNext(&asi, &start, &stop, NULL, &segType))
            {
                /* ignore gaps on the bioseq */
                /* hence we may have less than nsegs ivals */
                if (segType == AM_GAP) 
                    continue;
                
                if (segType == AM_INSERT) {
                  if (i == 0)
                    start = stop = finalRFIP->Left;
                  else
                    start = stop = finalRFIP->ivals[i - 1];
                }
                finalRFIP->ivals[i++] = start;
                finalRFIP->ivals[i++] = stop;
                finalRFIP->numivals++;
            }
        }
    }
                
    ++alignSP->alignIndex;
    /*
        if we are not done with all the alignments and
        the next alignment(s) has the same accession and strand as this one does
        merge their segments together.
    */
    if (! AlignmentFilterStateDone(alignSP) && 
        alignSorted[alignSP->alignIndex - 1].strand ==  alignSorted[alignSP->alignIndex].strand &&
        StrNCmp(alignSorted[alignSP->alignIndex - 1].label, 
                alignSorted[alignSP->alignIndex].label, MAX_ALIGN_SORT_LABEL) == 0) {
        Int4                    *newIvals;  /* pointer to merged ivals */
        Int4                    newIvalsNum;
        Int1                    *accumulator;
        Int4                    accumBegin, accumEnd, accumLen;
        
        /* keep track of where the segments fall in this array */
        /* 0 - a gap. 1 - a segment. 2 - an insert */
        accumBegin = finalRFIP->Left; 
        accumEnd   = vContext->seqLength;
        accumLen = accumEnd - accumBegin;
        accumulator = MemNew( accumLen * sizeof(Int1) );
        if (accumulator == NULL) {
            return NULL;
        }
        
        AccumIvals(accumulator, accumBegin, accumLen, finalRFIP);
        for ( ;
              ! AlignmentFilterStateDone(alignSP) && 
              alignSorted[alignSP->alignIndex - 1].strand ==  alignSorted[alignSP->alignIndex].strand &&
              StrNCmp(alignSorted[alignSP->alignIndex - 1].label, 
                      alignSorted[alignSP->alignIndex].label, MAX_ALIGN_SORT_LABEL) == 0;
              ++alignSP->alignIndex ) {
        
            SAlnP = alignSorted[alignSP->alignIndex].sap;

          /* where does this alignment start & stop in bioseq coords? */
            nsegs = AlignSegIteratorCreate(SAlnP, SID, &asi);

            if (asi.start < 0 || asi.stop < 0) 
                continue;
            RFI.Left = asi.start;
            finalRFIP->Left = MIN(finalRFIP->Left, RFI.Left);
            RFI.Right = asi.stop;
            finalRFIP->Right = MAX(finalRFIP->Right, RFI.Right);
            RFI.numivals = 1;
            RFI.ivals =  NULL;
            
#ifdef _DEBUG
            if (finalRFIP->Right > vContext->seqLength) { 
                printf("finalRFIP->Right too big: %d\n", finalRFIP->Right); /* put a breakpoint here! */
                return NULL;
            }
#endif            

            if (nsegs > 1) {
                RFI.ivals = MemNew (2 * nsegs * sizeof (Int4));
                if (RFI.ivals == NULL) 
                {
                    MemFree(RFI.ivals);
                } else {
                    RFI.numivals = 0;
                    i = 0;
                    while (AlignSegIteratorNext(&asi, &start, &stop, NULL, &segType))
                    {
                        /* ignore gaps on the bioseq */
                        /* hence we may have less than nsegs ivals */
                        if (segType == AM_GAP) 
                            continue;
                        
                        if (segType == AM_INSERT) {
                          if (i == 0)
                            start = stop = RFI.Left;
                          else
                            start = stop = RFI.ivals[i - 1];
                        }
                        RFI.ivals[i++] = start;
                        RFI.ivals[i++] = stop;
                        RFI.numivals++;
                    }
                }
            }
            AccumIvals(accumulator, accumBegin, accumLen, &RFI);
            if (RFI.ivals != NULL)
                MemFree(RFI.ivals);
        }
        newIvalsNum = CountIvals(accumulator, accumLen);
        newIvals = MemNew( 2 * newIvalsNum * sizeof(Int4) );
        if (newIvals == NULL) {
            MemFree(accumulator);
            return NULL;
        }
        MakeIvals(accumulator, accumBegin, accumLen, newIvals, newIvalsNum);
        MemFree(accumulator);
        
        if (finalRFIP->ivals != NULL)
            MemFree(finalRFIP->ivals);
        finalRFIP->numivals = newIvalsNum;
            
        if (newIvalsNum > 1) {
            finalRFIP->ivals = newIvals;
        } else {
            MemFree(newIvals);
            finalRFIP->ivals = NULL;
        }
    }

    /* remember to delete the final ivals array & label space */
    if (finalRFIP->numivals > 1)
    {
        vnp = ValNodeAddPointer(&FPSP->lastInFreeList, 0, finalRFIP->ivals);
        if (vnp == NULL) {
            MemFree(finalRFIP->ContentLabel);
            MemFree(finalRFIP->ivals);
            return NULL;
        }
        if (FPSP->needFreeList == NULL) {
          FPSP->needFreeList = FPSP->lastInFreeList;
        }
        FPSP->lastInFreeList = vnp;
    }
    vnp = ValNodeAddPointer(&FPSP->lastInFreeList, 0, finalRFIP->ContentLabel);
    if (vnp == NULL)
    {
        MemFree(finalRFIP->ContentLabel);
        return NULL;
    }
    if (FPSP->needFreeList == NULL) {
      FPSP->needFreeList = FPSP->lastInFreeList;
    }
    FPSP->lastInFreeList = vnp;
            
    return finalRFIP;
}


void AccumIvals(Int1* accumulator, Int4 accumBegin, Int4 accumLen, RelevantFeatureItemPtr RFIP)
{
    Int4    i, ai;
    Int4    blockstart, blockend;
    
    /* mark everywhere segments spans. 0 in gaps, 1 in blocks, 2 at inserts */
    if (RFIP->numivals == 1) {
        blockstart = RFIP->Left  - accumBegin;
        blockend   = RFIP->Right - accumBegin;
    
        for (ai = blockstart; ai <= blockend; ++ai)
            if (accumulator[ai] == 0)
                accumulator[ai] = 1;
    }
    else {
        for (i = 0; i < RFIP->numivals; ++i) {
            blockstart = RFIP->ivals[2*i] - accumBegin;
            blockend   = RFIP->ivals[2*i + 1] - accumBegin;
            
#ifdef _DEBUG
            if (blockstart < 0 || accumLen < blockstart || blockend < 0 || accumLen < blockend) {
                printf("AccumIvals problem!"); /* put a breakpoint here! */
                return;
            }
#endif                
            if (blockstart == blockend) {
                accumulator[blockstart] = 2;
            } else {
                for (ai = blockstart; ai <= blockend; ++ai)
                    if (accumulator[ai] == 0)
                        accumulator[ai] = 1;
            }
        }
    }
}

Int4  CountIvals(Int1* accumulator, Int4 accumLen)
{
    Int4    numivalsOut;
    Int4    ai;
    
    /* make new ivals */
    numivalsOut = 0;
    
    for (ai = 0; ai < accumLen; ++ai) {
        if (accumulator[ai] == 0)
            continue;
    /* if it is an insert, add it */
        if (accumulator[ai] == 2) {
            ++numivalsOut;
        } else if (accumulator[ai] == 1) { /* beginning of a block */
            /* find end of block */
            for (; ai < accumLen; ++ai) {
                if (accumulator[ai] != 1)
                    break;
            }
            --ai;
            ++numivalsOut;
        }
    }
    return numivalsOut;
}

void  MakeIvals(Int1* accumulator, Int4 accumBegin, Int4 accumLen, Int4Ptr ivalsOut, Int4 ivalsLen)
{
    Int4    numivalsOut;
    Int4    ai, blocklen;
    
    /* make new ivals */
    numivalsOut = 0;
    
    for (ai = 0; ai < accumLen  &&  numivalsOut < ivalsLen; ++ai) {
    /* ignore spots in gaps. */
        if (accumulator[ai] == 0)
            continue;
    /* if it is an insert, add it */
        if (accumulator[ai] == 2) {
            ivalsOut[numivalsOut * 2]     = accumBegin + ai;
            ivalsOut[numivalsOut * 2 + 1] = accumBegin + ai;
            ++numivalsOut;
        } else if (accumulator[ai] == 1) { /* beginning of a block */
            /* find end of block */
            for (blocklen = 0; ai + blocklen < accumLen; ++blocklen) {
                if (accumulator[ai + blocklen] != 1)
                    break;
            }
            ivalsOut[numivalsOut * 2]     = accumBegin + ai;
            if (accumulator[ai + blocklen] == 2)
                ivalsOut[numivalsOut * 2 + 1] = accumBegin + ai + blocklen;
            else
                ivalsOut[numivalsOut * 2 + 1] = accumBegin + ai + blocklen - 1;
            ++numivalsOut;
            ai += blocklen - 1;
        }
    }
}

/*
  For a given SeqAlign and a given SeqId for a Seq that participates in that SeqAlign,
  keep track of where we are in that alignment so we can iterate through all of that 
  alignment's segments, returning information about each segment.
  Hides the differences between normal/indexable and StdSeg alignments.
*/

/* 
  Create an structure to iterate through all of an alignment's segments.
  return a pointer to that iterator struct that must be deallocated.
   return the number of segments.
*/
static 
Int4 AlignSegIteratorCreate(SeqAlignPtr sap, SeqIdPtr sip,  AlignSegIteratorPtr asip)
{
  Boolean             stdSeg;
  Int4                swap;
  
  if (sap == NULL || sip == NULL  || asip == NULL)
    return 0;
      
  AlnMgr2IndexSingleChildSeqAlign(sap);
  asip->sap = sap;
  asip->sip = sip;
  
  stdSeg = (sap->segtype == SAS_STD);
  if ( ! stdSeg) {
    asip->nsegs = AlnMgr2GetNumSegs(sap);
    asip->alignRow = AlnMgr2GetFirstNForSipList (sap, sip);
    if (asip->alignRow == -1) {
      return 0;
    }
    AlnMgr2GetNthSeqRangeInSA(sap, asip->alignRow, &asip->start, &asip->stop);
    asip->strand = AlnMgr2GetNthStrand(sap, asip->alignRow);
  }
  else {
    asip->nsegs = AlnMgr2GetNumStdSegs(sap);
    asip->alignRow = 0;
    AlnMgr2GetSeqRangeForSipInSAStdSeg(sap, sip, &asip->start, &asip->stop, &asip->strand);
  }

  if (asip->stop < asip->start) {
    swap = asip->stop;
    asip->stop = asip->start;
    asip->start = swap;
  }
  
  asip->currentStdSeg = NULL;
  asip->currentSeg = 0;
  
  return asip->nsegs;
}

/*
  Given an AlignSegIterator return the information about the current segment
  in the pointer arguments, and advance the iterator to the next segment.
  Return false if there are no more segments in which case the output arguments
  will not be changed.
*/
static 
Boolean AlignSegIteratorNext(
  AlignSegIteratorPtr asip, 
  Int4Ptr       startOut, 
  Int4Ptr       stopOut, 
  Uint1Ptr      strandOut,
  Uint1Ptr      segTypeOut
)
{
  Int4    start, stop, swap;
  Uint1   strand, segType;
  Boolean stdSeg;
  Int4    iseg;
  
  
  if (asip == NULL) 
    return FALSE; /* bad argument */

  /* iterate */
  asip->currentSeg++;  
  if (asip->currentSeg > asip->nsegs) /* no more segments */
    return FALSE;
    
  if (asip->strand == Seq_strand_minus)
      iseg = asip->nsegs - asip->currentSeg + 1;
  else
      iseg = asip->currentSeg;
  
  stdSeg = (asip->sap->segtype == SAS_STD);
  if ( ! stdSeg) {
    Int4    alignStart, alignStop;
    Int4    row2, seq2Start;
 
    strand = asip->strand; /* all segments have the same strand */
    /* get segment location in alignment coordinates */
    AlnMgr2GetNthSegmentRange(asip->sap, iseg, &alignStart, &alignStop);
    /* convert to bioseq coordinates */
    start = AlnMgr2MapSeqAlignToBioseq(asip->sap, alignStart, asip->alignRow);
    if (alignStart != alignStop)
        stop  = AlnMgr2MapSeqAlignToBioseq(asip->sap, alignStop , asip->alignRow);
    else
        stop = start;

    /* Is this a insert (a gap on the bioseq)? */
    if (start == -2) {
        segType = AM_INSERT;
    } else {
      /* check the other sequence to see if there is a gap there. */
      if (asip->alignRow == 1)
        row2 = 2;
      else 
        row2 = 1;
      seq2Start = AlnMgr2MapSeqAlignToBioseq(asip->sap, alignStart, row2);

      if (seq2Start == -2 ) 
        segType = AM_GAP;
       else
        segType = AM_SEQ;
    }
  }
  else { /* stdSeg */   
    asip->currentStdSeg = AlnMgr2GetNthStdSeg(asip->sap, iseg);
    if (asip->currentStdSeg == NULL)
        return FALSE;    /* Logic Error. should not happen. */
        
    AlnMgr2GetSeqRangeForSipInStdSeg(asip->currentStdSeg, asip->sip, &start, &stop, &strand, &segType);
  }
  if (stop < start) {
    swap = start;
    start = stop;
    stop = swap;
  }
  
  /* take care of output arguments */
  if (startOut)   *startOut   = start;
  if (stopOut)    *stopOut    = stop;
  if (strandOut)  *strandOut  = strand;
  if (segTypeOut) *segTypeOut = segType;
  
  return TRUE;
}

 
static RelevantFeatureItemPtr GetNextRFIPinFeatureFilter (
  FilterProcessStatePtr FPSP
)

{
  ValNodePtr              currentRFIPblockVNP;
  FeatureFilterStatePtr   featSP;
  RelevantFeatureItemPtr  RFIP;
  ViewerContextPtr        vContext;
  FilterItemPtr           currentFIP;
  FilterPtr               FP;

  if (FPSP == NULL) return NULL;
  vContext = FPSP->vContext;
  currentFIP = FPSP->currentFIP;
  if (vContext == NULL || currentFIP == NULL) return NULL;
  FP = vContext->FltPtr;
  if (FP == NULL) return NULL;
  
  featSP = &FPSP->state.feat;
  for (; featSP->featureBlockOffset + featSP->indexInBlock < vContext->featureCount; featSP->indexInBlock++) {
    if (featSP->indexInBlock >= RELEVANT_FEATS_PER_CHUNK) {
      featSP->indexInBlock = 0;
      featSP->featureBlockOffset += RELEVANT_FEATS_PER_CHUNK;
      featSP->currentRFIPblockVNP = featSP->currentRFIPblockVNP->next;
      if (featSP->currentRFIPblockVNP == NULL) return NULL;
    }
    currentRFIPblockVNP = featSP->currentRFIPblockVNP;
    RFIP = (RelevantFeatureItemPtr) (currentRFIPblockVNP->data.ptrvalue) + featSP->indexInBlock;
    
    if (FP->GroupByAnnot && RFIP->sap != vContext->currentSAP) continue;
    
    if (! vContext->allFeatures
        && (RFIP->Right < vContext->from || RFIP->Left > vContext->to)) {
      continue;
    }
    if (FPSP->featuresProcessed [featSP->featureBlockOffset + featSP->indexInBlock]
        || (! currentFIP->IncludeFeature [RFIP->featdeftype])) continue;
    if (currentFIP->MatchStrand != BothStrands) {
      if (currentFIP->MatchStrand == MinusStrand && RFIP->featstrand != Seq_strand_minus) continue;
      if (currentFIP->MatchStrand == PlusStrand  && RFIP->featstrand != Seq_strand_plus) continue;
    }
    FPSP->featuresProcessed [featSP->featureBlockOffset + featSP->indexInBlock] = TRUE;
    break;
  }
  if (featSP->featureBlockOffset + featSP->indexInBlock >= vContext->featureCount) return NULL;
  return RFIP;
}



static RelevantFeatureItemPtr GetNextRFIPinFilterItem (
  FilterProcessStatePtr FPSP
)

{
  AlignmentFilterStatePtr alignSP;
  RelevantFeatureItemPtr  RFIP = NULL;
  ViewerContextPtr        vContext;
  FilterItemPtr           currentFIP;

  /* called as an iterator by the rendering functions -- builds & returns the next feature (in an RFIP), or returns NULL if no more left in this Filter */
  if (FPSP == NULL) return NULL;
  vContext = FPSP->vContext;
  currentFIP = FPSP->currentFIP;
  if (vContext == NULL || currentFIP == NULL) return NULL;
  switch (currentFIP->Type) {
  case InvalidFilter:
  case GraphFilter:
  case BioseqFilter:
    return NULL;
  case AlignmentFilter:
    alignSP = &FPSP->state.align;
    while (!AlignmentFilterStateDone(alignSP)) {
      RFIP = GetNextRFIPinAlignmentFilter (FPSP);
      if (RFIP != NULL) 
        return RFIP;
    } 
    /* note: if control reaches here, then RFIP == NULL */
    break;
  case FeatureFilter:
    RFIP = GetNextRFIPinFeatureFilter (FPSP);
    break;
  }
  if (RFIP != NULL) {
    FPSP->featuresProcessedCount++;
  }
  return RFIP;
}

static Boolean AddFeatureToRow (
  InternalRowPtr row,
  RelevantFeatureItemPtr RFIP,
  Boolean SkipSmearTest,
  FilterProcessStatePtr FPSP
)

{
  ValNodePtr             VNP;
  RelevantFeatureItemPtr newRFIP; /* for representing a multi-feature smear */
  RelevantFeatureItemPtr oldRFIP;
  ViewerContextPtr       vContext;

  vContext = FPSP->vContext;

  if (row == NULL || RFIP == NULL) return FALSE;

  if (!SkipSmearTest && row->rowFeatureCount &&
      TestForSmear (RFIP, (RelevantFeatureItemPtr) row->feats->data.ptrvalue, vContext->viewScale)) {
    /* if the last feature was not a smear-in-progress, allocate newRFIP, else re-use the current one */
    oldRFIP = (RelevantFeatureItemPtr) row->feats->data.ptrvalue;
    if (oldRFIP->entityID == 0 && oldRFIP->itemID == 0) {
      /* oldRFIP is already a smear-in-prorgess, just extend it */
      newRFIP = oldRFIP;
    } else {
      /* need to create a new smear */
      newRFIP = MemNew (sizeof (RelevantFeatureItem));
      VNP = MemNew (sizeof (ValNode));
      if (newRFIP == NULL || VNP == NULL) return FALSE;
      VNP->data.ptrvalue = newRFIP;
      VNP->next = FPSP->needFreeList;
      FPSP->needFreeList = VNP;
      row->feats->data.ptrvalue = newRFIP;
      newRFIP->featdeftype = oldRFIP->featdeftype;
      newRFIP->numivals = 1;
    }

    newRFIP->Left = MIN (oldRFIP->Left, RFIP->Left);
    newRFIP->Right = MAX (oldRFIP->Right, RFIP->Right);
  } else {

    VNP = MemNew (sizeof (ValNode));
    if (VNP == NULL) return FALSE;
    VNP->data.ptrvalue = RFIP;
    VNP->next = row->feats;
    row->feats = VNP;
    row->rowFeatureCount++;
  }
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
  FilterProcessStatePtr FPSP
)

{
  Uint2                   rows = 0;
  InternalRowPtr          thisRow;
  RelevantFeatureItemPtr  RFIP;

  thisRow = firstRow;
  if (thisRow == NULL) return 0;
  thisRow->rowFeatureCount = 0;
  while ((RFIP = GetNextRFIPinFilterItem (FPSP)) != NULL) {
    AddFeatureToRow (thisRow, RFIP, TRUE, FPSP);
    thisRow = AddARow (thisRow);
    rows++;
  }
  return rows;
}

/* this was copied from seqmgr.c (6.181, 27-feb-2002) */
static Boolean CheckInternalExonBoundaries (Int2 numivalsCDS, Int4Ptr ivalsCDS, Int2 numivalsMRNA, Int4Ptr ivalsMRNA)

{
  Int2  i;
  Int2  j;

  if (numivalsCDS > numivalsMRNA) return FALSE;
  if (ivalsCDS == NULL || ivalsMRNA == NULL) return TRUE;

  /* scan first exon-intron boundary against candidate start positions */

  for (i = 0; i <= numivalsMRNA - numivalsCDS; i++) {
    if (ivalsCDS [1] == ivalsMRNA [2 * i + 1]) break;
  }
  if (i > numivalsMRNA - numivalsCDS) return FALSE;

  /* Addition by Eric: the first interval in the CDS must not be larger than the corresponding interval in the mRNA */
  if (ABS (ivalsCDS [0] - ivalsCDS [1]) > ABS (ivalsMRNA [2 * i] - ivalsMRNA [2 * i + 1])) return FALSE;

  /* scan subsequent exon-intron and intron-exon boundaries */

  for (j = 2; j <= 2 * numivalsCDS - 2; j++) {
    if (ivalsCDS [j] != ivalsMRNA [2 * i + j]) return FALSE;
  }

  /* Addition by Eric: the last interval in the CDS must not be larger than the corresponding interval in the mRNA */
  if (ABS (ivalsCDS [j - 1] - ivalsCDS [j]) > ABS (ivalsMRNA [2 * i + j - 1] - ivalsMRNA [2 * i + j])) return FALSE;

  return TRUE;
}

static Boolean mRNAmatchesCDS (
  RelevantFeatureItemPtr mRNA,
  RelevantFeatureItemPtr CDS
)

{
  /* the mRNA must be the larger feature */
  if (CDS->Left < mRNA->Left || CDS->Right > mRNA->Right) return FALSE;
  /* check strands */
  if (CDS->featstrand != mRNA->featstrand) return FALSE;
  /* trivial case */
  if (CDS->numivals == 1 && mRNA->numivals == 1) return TRUE;
  /* . . . and the intervals must line up */
  return CheckInternalExonBoundaries (CDS->numivals, CDS->ivals, mRNA->numivals, mRNA->ivals);
}

typedef struct rFIPentry {
  RelevantFeatureItemPtr RFIP;
  Int4                   decorationLeft;
  Int4                   decorationRight;
  Int4                   Right;
  Int4                   Left;
  Boolean                used;
} RFIPentry, PNTR RFIPentryPtr;

typedef struct rFIPgroup {
  Uint4       memberCount;
  Int4        decorationLeft;
  Int4        decorationRight;
  Int4        Left;
  Int4        Right;
  ValNodePtr  members; /* data.ptrvalue = RFIPentryPtr */
} RFIPgroup, PNTR RFIPgroupPtr;


static Uint2 GeneProductsLayoutInternal (
  InternalRowPtr firstRow,
  FilterProcessStatePtr FPSP,
  Boolean MultiRender
)

{
  Uint2                   rows = 1, i;
  RelevantFeatureItemPtr  RFIP, RFIPcds, RFIPmrna, tRFIP;
  InternalRowPtr          thisRow, thisRow2, lastRow;
  ValNodePtr              fifoHead = NULL, fifoTail = NULL;
  ValNodePtr              bumpedListHead, bumpedListTail;
  ValNodePtr              potCDSvnp, potRNAvnp;
  ValNodePtr              GroupsHead = NULL, GroupsTailVNP, MemberTailVNP;
  ValNodePtr              groupVNP, featVNP;
  RFIPentryPtr            tRFIPentry;
  RFIPentryPtr            bumpedListEntry;
  RFIPentryPtr            potCDS, potRNA, GroupsTailMemberTail;
  Uint2                   bumpedCount;
  Boolean                 foundRows, doneCollecting = FALSE;
  Int4                    newLeft, featureStart, rowMaxRight;
  RenderInput             dummyRI;
  RFIPgroupPtr            currentGroup;
  Uint4                   viewScale;
  
  viewScale =  FPSP->vContext->viewScale;

  if (firstRow == NULL) return 0;

  lastRow = firstRow;
  firstRow->layoutData.intvalue = -2000000000; /* the first feature will _always_ fit in the first row */

  while (1) {
    if (doneCollecting) {
      break;
    }
    RFIP = GetNextRFIPinFilterItem (FPSP);
    if (RFIP == NULL) {
      doneCollecting = TRUE;
      bumpedListHead = fifoHead; /* no more incoming features, bump all features in the queue*/
      bumpedListTail = fifoTail;
      fifoHead = NULL;
    } else {
      if (! BuildRenderInputFromRFIP (&dummyRI, RFIP, FPSP)) {
        continue; /* either we're out of memory or this feature doesn't overlap the clipping SeqLoc */
      }

      if ((tRFIPentry = MemNew (sizeof (RFIPentry))) == NULL) goto bail_out;
      if (fifoHead == NULL) {
        fifoTail = ValNodeAddPointer (&fifoHead, 0, tRFIPentry);
      } else {
        fifoTail = ValNodeAddPointer (&fifoTail, 0, tRFIPentry);
      }
      if (fifoTail == NULL) goto bail_out;
      tRFIPentry->RFIP = RFIP;
      tRFIPentry->Left = RFIP->Left;
      tRFIPentry->Right = RFIP->Right;
      tRFIPentry->decorationLeft = dummyRI.decorationLeft;
      tRFIPentry->decorationRight = dummyRI.decorationRight;

    /*
      now find all features which can not overlap the next feature (in the sequence; decoration overlap doesn't matter)
    */
      newLeft = RFIP->Left;

      bumpedCount = 0;
      bumpedListHead = fifoHead;
      for (featVNP = fifoHead;
           featVNP != NULL;
           featVNP = featVNP->next) {
        bumpedListEntry = featVNP->data.ptrvalue;
        RFIP = bumpedListEntry->RFIP;
        if (RFIP->Right > newLeft) break;
        bumpedListTail = featVNP;
        fifoHead = featVNP->next;
        bumpedCount++;
      }
      if (bumpedCount == 0) continue;
    }
    if (bumpedListTail != NULL) {
      bumpedListTail->next = NULL;
    }
    GroupsHead = GroupsTailVNP = NULL;

    for (potCDSvnp = bumpedListHead; potCDSvnp != NULL; potCDSvnp = potCDSvnp->next) {
      /* this pass is only searching for CDS features (and correspondins mRNAs) */
      potCDS = potCDSvnp->data.ptrvalue;
      RFIPcds = potCDS->RFIP;
      if (RFIPcds->featdeftype != FEATDEF_CDS) continue;
      potCDS->used = TRUE;

      if ((currentGroup = MemNew (sizeof (RFIPgroup))) == NULL) goto bail_out;
      if (GroupsHead == NULL) {
        GroupsTailVNP = ValNodeAddPointer (&GroupsHead, 0, currentGroup);
      } else {
        GroupsTailVNP = ValNodeAddPointer (&GroupsTailVNP, 0, currentGroup);
      }
      if (GroupsTailVNP == NULL) goto bail_out;

      /* do not add the CDS to currentGroup->members yet, b/c we want mRNA features to appear first
         (but remember to add it after!) */

      currentGroup->memberCount = 1;
      currentGroup->members = NULL;

      currentGroup->Left = potCDS->Left;
      currentGroup->Right = potCDS->Right;
      currentGroup->decorationLeft = potCDS->decorationLeft;
      currentGroup->decorationRight = potCDS->decorationRight;

      for (potRNAvnp = bumpedListHead; potRNAvnp != NULL; potRNAvnp = potRNAvnp->next) {
        potRNA = potRNAvnp->data.ptrvalue;
        RFIPmrna = potRNA->RFIP;
        if (RFIPmrna->featdeftype != FEATDEF_mRNA) continue;
        if (!MultiRender && potRNA->used) continue;

        if (mRNAmatchesCDS (RFIPmrna, RFIPcds)) {
          potRNA->used = TRUE;

          if ((tRFIPentry = MemNew (sizeof (RFIPentry))) == NULL) goto bail_out;
          if (ValNodeAddPointer (&currentGroup->members, 0, tRFIPentry) == NULL) goto bail_out;

          MemCopy (tRFIPentry, potRNA, sizeof (RFIPentry));

          currentGroup->memberCount++;
          currentGroup->Left = MIN (currentGroup->Left, RFIPmrna->Left);
          currentGroup->Right = MAX (currentGroup->Right, RFIPmrna->Right);
          currentGroup->decorationLeft = MIN (currentGroup->decorationLeft, potRNA->decorationLeft);
          currentGroup->decorationRight = MAX (currentGroup->decorationRight, potRNA->decorationRight);
        }
      }

      if ((GroupsTailMemberTail = MemNew (sizeof (RFIPentry))) == NULL) goto bail_out;
      if (ValNodeAddPointer (&currentGroup->members, 0, GroupsTailMemberTail) == NULL) goto bail_out;
      MemCopy (GroupsTailMemberTail, potCDS, sizeof (RFIPentry));

    }
    /*
      append all non-matched elements to the Groups list
    */
    for (featVNP = bumpedListHead; featVNP != NULL; featVNP = featVNP->next) {
      tRFIPentry = featVNP->data.ptrvalue;
      /* skip feature if it's been matched already */
      if (tRFIPentry->used) {
        continue;
      } else {
        /* add another singleton entry to Groups */
        tRFIP = tRFIPentry->RFIP;

        if ((currentGroup = MemNew (sizeof (RFIPgroup))) == NULL) goto bail_out;
        if (GroupsHead == NULL) {
          GroupsTailVNP = ValNodeAddPointer (&GroupsHead, 0, currentGroup);
        } else {
          GroupsTailVNP = ValNodeAddPointer (&GroupsTailVNP, 0 ,currentGroup);
        }
        if (GroupsTailVNP == NULL) goto bail_out;

        if ((GroupsTailMemberTail = MemNew (sizeof (RFIPentry))) == NULL) goto bail_out;
        if ((MemberTailVNP = ValNodeAddPointer (&currentGroup->members, 0, GroupsTailMemberTail)) == NULL) goto bail_out;

        currentGroup->memberCount = 1;
        MemCopy (GroupsTailMemberTail, tRFIPentry, sizeof (RFIPentry));

        currentGroup->Left = tRFIP->Left;
        currentGroup->Right = tRFIP->Right;
        currentGroup->decorationLeft = tRFIPentry->decorationLeft;
        currentGroup->decorationRight = tRFIPentry->decorationRight;

      }
    }

    /*
      now, assign each element in Groups to a row.  algorithm is the same as in BubbleUpLayout,
      except that instead of looking for the first matching row, need to find (RFIPgroup->members)
      consecutive free rows.
    */
    for (groupVNP = GroupsHead; groupVNP != NULL; groupVNP = groupVNP->next) {
      currentGroup = groupVNP->data.ptrvalue;

      featureStart = currentGroup->decorationLeft;
      for (thisRow = firstRow; thisRow != NULL; thisRow = thisRow->next) {
        foundRows = TRUE;
        for (i = 0, thisRow2 = thisRow, featVNP = currentGroup->members;
             featVNP != NULL && i < currentGroup->memberCount;
             thisRow2 = thisRow2->next, featVNP = featVNP->next, i++) {
          tRFIPentry = featVNP->data.ptrvalue;
/*
  note: if thisRow2 ends up being NULL then it meas thisRow was a good starting point and we just need to add some rows
*/
          if (thisRow2 == NULL) {
            foundRows = TRUE;
            break;
          }
          rowMaxRight = thisRow2->layoutData.intvalue;
          if ( tRFIPentry->decorationLeft <= rowMaxRight  ||   
                NoVisibleGap(tRFIPentry->decorationLeft, rowMaxRight, viewScale) ) {
            foundRows = FALSE;
            if (thisRow2->next == NULL) {
              rows++;
              if ((lastRow = AddARow (lastRow)) == NULL) goto bail_out;
              lastRow->layoutData.intvalue = -2000000000;
            }
            break;
          }
        }
        if (foundRows) {
          for (i = 0,
                 thisRow2 = thisRow,
                 featVNP = currentGroup->members;
               tRFIPentry != NULL
                 && i < currentGroup->memberCount;
               featVNP = featVNP->next,
                 thisRow2 = thisRow2->next,
                 i++) {
            tRFIPentry = featVNP->data.ptrvalue;
            RFIP = tRFIPentry->RFIP;
            if (thisRow2 == NULL) {
              rows++;
              if ((lastRow = AddARow (lastRow)) == NULL) goto bail_out;
              thisRow2 = lastRow;
            }
            AddFeatureToRow (thisRow2, RFIP, FALSE, FPSP);
            rowMaxRight = tRFIPentry->decorationRight;
            thisRow2->layoutData.intvalue = rowMaxRight;
          }
          break;
        }
      }
    }
    bumpedListHead = ValNodeFreeData (bumpedListHead);
    for (groupVNP = GroupsHead; groupVNP != NULL; groupVNP = groupVNP->next) {
      currentGroup = groupVNP->data.ptrvalue;
      ValNodeFreeData (currentGroup->members);
    }
    GroupsHead = ValNodeFreeData (GroupsHead);
  }
bail_out:
  ValNodeFreeData (bumpedListHead);
  ValNodeFreeData (fifoHead);
  for (groupVNP = GroupsHead; groupVNP != NULL; groupVNP = groupVNP->next) {
    currentGroup = groupVNP->data.ptrvalue;
    ValNodeFreeData (currentGroup->members);
  }
  GroupsHead = ValNodeFreeData (GroupsHead);
  return rows;
}

static Uint2 GeneProductsLayout (
  InternalRowPtr firstRow,
  FilterProcessStatePtr FPSP
)

{
  return GeneProductsLayoutInternal (firstRow, FPSP, FALSE);
}

static Uint2 GeneProductsLayoutX (
  InternalRowPtr firstRow,
  FilterProcessStatePtr FPSP
)

{
  return GeneProductsLayoutInternal (firstRow, FPSP, TRUE);
}

static Uint2 BubbleUpLayout (
  InternalRowPtr firstRow,
  FilterProcessStatePtr FPSP
)

{
  Uint2                   rows = 1;
  Int4                    featureStart, rowMaxRight;
  RelevantFeatureItemPtr  RFIP;
  InternalRowPtr          thisRow, lastRow;
  RenderInput             dummyRI;
  Uint4                   viewScale;
  
  viewScale =  FPSP->vContext->viewScale;
  /*
     This uses InternalRow.layoutData as an Int4, which stores the (x) offset of the last used pixel.
     so a feature starting at (internalRow.layoutData + 1) or greater can be placed in the same row
     without a collision.
   */

  /* add the 1st feature to the 1st row */
  do {
    RFIP = GetNextRFIPinFilterItem (FPSP);
    if (RFIP == NULL) return 0;
  } while (! BuildRenderInputFromRFIP (&dummyRI, RFIP, FPSP));

  AddFeatureToRow (firstRow, RFIP, FALSE, FPSP);
  lastRow = firstRow;
  firstRow->layoutData.intvalue = dummyRI.decorationRight;

  while ((RFIP = GetNextRFIPinFilterItem (FPSP)) != NULL) {
    if (! BuildRenderInputFromRFIP (&dummyRI, RFIP, FPSP)) {
      continue;
    }
    featureStart = dummyRI.decorationLeft;
    for (thisRow = firstRow; thisRow != NULL; thisRow = thisRow->next) {
      rowMaxRight = thisRow->layoutData.intvalue;
      if (featureStart >= rowMaxRight  &&  
            ! NoVisibleGap(featureStart, rowMaxRight, viewScale) ) {
        AddFeatureToRow (thisRow, RFIP, FALSE, FPSP);
        rowMaxRight = MAX (rowMaxRight, dummyRI.decorationRight);
        thisRow->layoutData.intvalue = rowMaxRight;
        RFIP = NULL;
        break;
      }
    }
    if (RFIP != NULL) {
      rows++;
      lastRow = AddARow (lastRow);
      AddFeatureToRow (lastRow, RFIP, FALSE, FPSP);
      rowMaxRight = dummyRI.decorationRight;
      lastRow->layoutData.intvalue = rowMaxRight;
    }
  }
  return rows;
}

static Uint2 SingleRowLayout (
  InternalRowPtr firstRow,
  FilterProcessStatePtr FPSP
)

{
  RelevantFeatureItemPtr RFIP, lastRFIP;
  ValNodePtr       freeVNP, vnp;
  Uint4            smearStart, smearStop;
  Uint1            smearFeatdeftype;
  ViewerContextPtr vContext;
  Uint4            viewScale;
  Boolean          adjacentNoGap;
  Boolean          tooNarrow;
  Boolean          smearing = FALSE, justSmeared = FALSE;

  vContext = FPSP->vContext;
  viewScale = vContext->viewScale;
  lastRFIP = GetNextRFIPinFilterItem (FPSP);
  if (lastRFIP == NULL)
    return 0;
  smearFeatdeftype = lastRFIP->featdeftype;

  while ((RFIP = GetNextRFIPinFilterItem (FPSP)) != NULL) {
    adjacentNoGap =  PixelsBetweenSeqCoords(lastRFIP->Right, RFIP->Left, viewScale) < 2; /* was 3 */
    /* TestForSmearOverlap (lastRFIP->Right + 2 * vContext->viewScale, RFIP->Left, vContext); */
    tooNarrow = PixelsBetweenSeqCoords(RFIP->Left,  RFIP->Right, viewScale) < 5;
    /* TestForSmearOverlap (RFIP->Left + 4 * vContext->viewScale, RFIP->Right, vContext); */
    if (adjacentNoGap && tooNarrow) {
      if (smearing == FALSE) {
        tooNarrow = PixelsBetweenSeqCoords(lastRFIP->Left, lastRFIP->Right, viewScale) < 5;
        /* TestForSmearOverlap (lastRFIP->Left + 4 * vContext->viewScale, lastRFIP->Right, vContext); */
        if (tooNarrow) {
          smearStart = lastRFIP->Left;
        } else {
          smearStart = RFIP->Left;
        }
        smearing = TRUE;
      }
      justSmeared = TRUE;
      if (smearFeatdeftype != RFIP->featdeftype) {
        smearFeatdeftype = FEATDEF_ANY;
      } else if (smearFeatdeftype == 0) {
        smearFeatdeftype = RFIP->featdeftype;
      }
      smearStop = RFIP->Right;
      continue;
    }
    /* VNP is non-NULL during a smear; this tests for regular features*/
    if (! justSmeared) {
      AddFeatureToRow (firstRow, lastRFIP, TRUE, FPSP);
      lastRFIP = RFIP;
    } else { /* we just finished a smear group */
      lastRFIP = MemNew (sizeof (RelevantFeatureItem));
      if (lastRFIP == NULL) return 1;
      lastRFIP->Left = smearStart;
      lastRFIP->Right = smearStop;
      lastRFIP->featstrand = Seq_strand_unknown;
      lastRFIP->circularSpanningOrigin = FALSE;
      lastRFIP->LeftEnd = lastRFIP->RightEnd = EndAbsolute;
      lastRFIP->circularSpanningOrigin = FALSE;
      lastRFIP->featdeftype = smearFeatdeftype;
      lastRFIP->numivals = 1;
      vnp = ValNodeAddPointer (&FPSP->lastInFreeList, 0, lastRFIP);
      if (FPSP->needFreeList == NULL) {
        FPSP->needFreeList = FPSP->lastInFreeList;
      }
      FPSP->lastInFreeList = vnp;
      smearing = FALSE;
      AddFeatureToRow (firstRow, lastRFIP, TRUE, FPSP);
      justSmeared = FALSE;
      lastRFIP = RFIP;
    }
  }
  if (! justSmeared) { /* the last feature was not in a smear group */
    AddFeatureToRow (firstRow, lastRFIP, TRUE, FPSP);
  } else {
    RFIP = MemNew (sizeof (RelevantFeatureItem));
    if (RFIP == NULL) return 1;
    RFIP->Left = smearStart;
    RFIP->Right = smearStop;
    RFIP->featstrand = Seq_strand_unknown;
    RFIP->circularSpanningOrigin = FALSE;
    
    RFIP->LeftEnd = RFIP->RightEnd = EndAbsolute;
    RFIP->featdeftype = smearFeatdeftype;
    RFIP->numivals = 1;

    if ((freeVNP = MemNew (sizeof (ValNode))) == NULL) {
      MemFree (RFIP);
      return 1;
    }
    freeVNP->data.ptrvalue = RFIP;
    freeVNP->next = FPSP->needFreeList;
    FPSP->needFreeList = freeVNP;

    AddFeatureToRow (firstRow, RFIP, TRUE, FPSP);
  }
  return 1;
}


/* 
  Call DrawNameAnnotbox() in a sub-function of FilterAndLayout, when you know that something
  will be drawn in as a result of this call to FilterAndLayout in the current sanLevelSeg.
  return FALSE if there is an error. Return true whether we draw anything or not.
*/
static 
Boolean DrawNamedAnnotBox(FilterProcessStatePtr FPSP)
{
  ViewerContextPtr vContext;
  CharPtr          annotName;
  FilterPtr        FP;
  
  if (FPSP == NULL) /* something wrong !? */
    return FALSE;
  vContext = FPSP->vContext;
  if (vContext == NULL)
    return FALSE;
  if (vContext->sanLevelSeg == NULL)
    return FALSE;
  if ((FP = vContext->FltPtr) == NULL)
    return FALSE;

  if (!FP->GroupByAnnot) /* we are not grouping by named annotations */
    return TRUE;
    
  annotName = GetSeqAnnotName(vContext->currentSAP);
  if (annotName == NULL)  /* we are currently not working on a named Seq Annot. */
    return TRUE;
    
  if (FP->DrawAnnotBox && !FPSP->annotBoxDrawn) /* We should draw the box and it hasn't been done yet. */
  {
    AddAttribute (vContext->sanLevelSeg, COLOR_ATT, FP->AnnotBoxColor, 0, 0, 0, 0);
    AddSilentSegRect (vContext->sanLevelSeg, FALSE, 0);
    FPSP->ceiling -= 5;
    FPSP->annotBoxDrawn = TRUE;
  }
  
  /* Have a label that should be drawn at the top of the box? */
  if (!FPSP->annotLabelDrawn && !StringHasNoText (annotName))
  {
    AddAttribute (vContext->sanLevelSeg, COLOR_ATT, FP->AnnotLabelColor, 0, 0, 0, 0);
    switch (FP->AnnotLabelLoc) {
    case LabelOnTop:
      AddTextLabel (vContext->sanLevelSeg, (vContext->from + vContext->to) / 2, FPSP->ceiling,
                    annotName, FP->AnnotLabelFont, 1, LOWER_CENTER, 0);
      SelectFont(FP->AnnotLabelFont);
      FPSP->ceiling -=  LineHeight () + 4;
      break;
    case LabelOnSide:
      AddTextLabel (vContext->sanLevelSeg, 0, FPSP->ceiling, 
                    annotName, FP->AnnotLabelFont, 1, LOWER_RIGHT, 0);
      break;
    }
    FPSP->annotLabelDrawn = TRUE;
  }
  return TRUE;
}

/* 
  Call DrawFilterItemBoxLabel() in a sub-function of FilterAndLayout, when you know that something
  will be drawn in as a result of this call to FilterAndLayout in the current filterSeg (vc->drawOnMe).
  return FALSE if there is an error. Return true whether we draw anything or not.
*/
static Boolean DrawFilterItemBoxLabel(
  FilterProcessStatePtr FPSP,
  FilterItemPtr FIP
)
{
  ViewerContextPtr    vContext;
  SegmenT             invisibleSeg;

  if (FPSP == NULL) /* something wrong !? */
    return FALSE;
  vContext = FPSP->vContext;
  if (vContext == NULL)
    return FALSE;
  if (vContext->drawOnMe == NULL)
    return FALSE;
  
  if (FIP->DrawGroupBox && !FPSP->groupBoxDrawn) /* want to draw it, or already drawn. */
  {   
    AddAttribute (vContext->drawOnMe, COLOR_ATT, FIP->GroupBoxColor, 0, 0, 0, 0);
    AddSilentSegRect (vContext->drawOnMe, FALSE, 0);
    FPSP->ceiling -= 5;
    /* add invisible line to force width of SegRect */
    invisibleSeg = CreateSegment (vContext->drawOnMe, 0, 0);
    SetSegmentVisibleFlag (invisibleSeg, FALSE);
    AddLine (invisibleSeg, vContext->from - 1 * vContext->viewScale ,  FPSP->ceiling , 
                           vContext->to + 1 * vContext->viewScale ,  FPSP->ceiling, FALSE, 0);
    FPSP->groupBoxDrawn = TRUE;
  }
    
  /* need to draw a label on top? */
  if (FIP->GroupLabelLoc == LabelOnTop && !FPSP->groupLabelDrawn && !StringHasNoText (FIP->GroupLabel)) {
    AddAttribute (vContext->drawOnMe, COLOR_ATT, FIP->GroupLabelColor, 0, 0, 0, 0);
    AddTextLabel (vContext->drawOnMe, (vContext->from + vContext->to) / 2, FPSP->ceiling,
                  FIP->GroupLabel, FIP->GroupLabelFont, 1, LOWER_CENTER, 0);
    SelectFont (FIP->GroupLabelFont);
    FPSP->ceiling -= LineHeight () + 4;
    FPSP->groupLabelDrawn = TRUE;
  }

  return TRUE;
}



static const LayoutFunction LayoutAlgorithmTable [] = {
  BubbleUpLayout,             /* placeholder for Layout_Inherit */
  SimpleDiagonalLayout,       /* Layout_Diagonal */
  /*  SimpleDiagonalLayout,*/ /* Layout_DiagonalSawtooth (to be implemented) */
  SingleRowLayout,            /* Layout_FeatTypePerLine (same as single-row, but features are grouped by type before processing) */
  BubbleUpLayout,             /* Layout_FeatTypePerLineGroup (same as bubble-up, but features are grouped by type before processing) */
  /*SingleRowLayout,*/        /* Layout_AllInOneLine (which isn't working currently, and may be less useful than expected*/
  BubbleUpLayout,             /* Layout_PackUpward */
  GeneProductsLayout,         /* Layout_GroupCorrespondingFeats (?working?) */
  GeneProductsLayoutX         /* Layout_GroupCorrespondingFeatsRepeat (?working?) */
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
  Uint2                   row, rowCount, rowHeight, totalHeight;
  Uint4                   feat;
  InternalRowPtr          firstRow, thisRow;
  ValNodePtr              VNP;
  RelevantFeatureItemPtr  RFIP;
  RenderInput             RI;           /* dummy used while finding height of each row */
  SegmenT                 SegmentTreeTop;
  SegmenT                 SegmentTreeMid;
  SegmenT                 SegmentTreeBot;
  Boolean                 SegmentChanged = TRUE;
  Boolean                 emptyRow = TRUE;
  Boolean                 allEmpty = TRUE;
  FilterItemPtr           FIP;

  firstRow = MemNew (sizeof (InternalRow));
  if (firstRow == NULL) return 0;
  firstRow->feats = NULL;
  VNP = FPSP->currentFilterVNP;
  if (VNP == NULL) return 0;
  FIP = (FilterItemPtr) VNP->data.ptrvalue;

  rowCount = (*LayoutAlgorithmTable [layoutC]) (firstRow, FPSP);

  thisRow = firstRow;
  totalHeight = 0;

  for (row = 0; row < rowCount; row++) {
    if (thisRow == NULL) continue;
    /*First iterate through features to find the row's height */
    VNP = thisRow->feats;
    rowHeight = 0;
    if (thisRow->rowFeatureCount > 0) {
      allEmpty = FALSE;
    }
    for (feat = 0; feat < thisRow->rowFeatureCount; feat++) {
      RFIP = VNP->data.ptrvalue;
      if (VNP == NULL) break;
      VNP = VNP->next;
      if (RFIP == NULL) return 0;
      if (! BuildRenderInputFromRFIP (&RI, RFIP, FPSP)) {
        continue;
      }
      rowHeight = MAX (rowHeight, RI.decorationHeight);
    }
    if (!allEmpty)
    {
      DrawNamedAnnotBox(FPSP);
      DrawFilterItemBoxLabel(FPSP, FIP);
    }

    /*Repeat, but actually draw them this time */
    VNP = thisRow->feats;
    RI.rowHeight = rowHeight;
    RI.yStart = FPSP->ceiling - totalHeight;
    if (thisRow->rowFeatureCount > 0) {
      emptyRow = FALSE;
    } else {
      emptyRow = TRUE;
    }
    I = J = K = 0;
    SegmentChanged = TRUE;
    for (feat = 0; feat < thisRow->rowFeatureCount; feat++) {
      if (VNP == NULL) break;
      if ((RFIP = VNP->data.ptrvalue) == NULL) return 0;
      VNP = VNP->next;
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
        SegmentChanged = FALSE;
      }
      if (! EnsureFeatureHasSegment (FPSP, featdeftype, SegmentTreeBot)) return 0;
      if (! BuildRenderInputFromRFIP (&RI, RFIP, FPSP)) {
        continue;
      }

      DrawFeatureAndLabel (&RI, vContext);
    }
    if (! emptyRow) {
      totalHeight += rowHeight + FPSP->currentFIP->IntraRowPaddingPixels;
    }
    if (thisRow->next == NULL) break;
    thisRow = thisRow->next;
  }
  if (allEmpty) {
    return 0;
  }
  return totalHeight;
}

typedef struct scalesntdata {
  SegmenT                   seg;
  PrimitivE                 snt;
  BoxInfo                   box;
  Int4                      length;
  Int4                      labelOffset;
  BioseqAppearanceItemPtr   bioseqAIP;
  Boolean                   disableLastLabel;
  Boolean                   disableFirstLabel;
  Int4                      offset;
} ScaleSntData, PNTR ScaleSntPtr;

static void ScaleSntDrawProc (
  BigScalar calldata,
  PrimDrawContext pdc
)

{
  Int4                     curScale;
  DrawInfoPtr              dInfoPtr;
  Int4                     i, j;
  RecT                     r;
  PntInfo                  pnt;
  PoinT                    pt;
  ScaleSntPtr              ssp;
  BoxInfo                  tmpBox;
  Uint1                    tickCount;
  Char                     buffer[16];
  BioseqAppearanceItemPtr  bioseqAIP;
  Uint1                    scaleHeight;
  Int4                     from, to;

  ssp = (ScaleSntPtr) calldata;
  if (ssp == NULL) return;

  dInfoPtr = (DrawInfoPtr) pdc;
  tmpBox = ssp->box;
  bioseqAIP = ssp->bioseqAIP;
  scaleHeight = bioseqAIP->scaleHeight;

  from = tmpBox.left;
  to   = tmpBox.right;

  curScale = dInfoPtr->scale.scaleX;
  r.left   = (Int2) ((dInfoPtr->scale.offsetX + tmpBox.left)  / curScale);
  r.right  = (Int2) ((dInfoPtr->scale.offsetX + tmpBox.right)  / curScale);
  SelectFont (bioseqAIP->scaleFont);
  SelectColor (bioseqAIP->scaleColor[0], bioseqAIP->scaleColor[1], bioseqAIP->scaleColor[2]);

  /* if the right-most edge is visible, draw the final tick mark and a right-justified total length label */
  if (dInfoPtr->scale.worldWindow.right >= to - 100 * curScale) {
    pnt.x = to;
    pnt.y = tmpBox.top;
    MapWorldPointToPixel (&pt, &pnt, &dInfoPtr->scale);
    MoveTo (pt.x, pt.y);
    pnt.y = tmpBox.top - scaleHeight;
    MapWorldPointToPixel (&pt, &pnt, &dInfoPtr->scale);
    LineTo (pt.x, pt.y);
    sprintf (buffer, "%ld", (long)to + ssp->labelOffset + 1);
    pt.x += 3 - StringWidth (buffer);
    pt.y += 1 + Ascent ();
    PaintStringEx (buffer, pt.x, pt.y);
  }

  /* if the left-most edge is visible, draw the first label, left-justified */
  if (dInfoPtr->scale.worldWindow.left - 400 * curScale <= from) {
    pnt.x = from;
    pnt.y = tmpBox.top - scaleHeight;
    sprintf (buffer, "%ld", (long)((from == 0) ? 1 : from)  + ssp->labelOffset);
    MapWorldPointToPixel (&pt, &pnt, &dInfoPtr->scale);
    pt.y += 1 + Ascent ();
    PaintStringEx (buffer, pt.x, pt.y);
  }

  if (tmpBox.left > dInfoPtr->scale.worldWindow.right ||
      tmpBox.right < dInfoPtr->scale.worldWindow.left ||
      tmpBox.top < dInfoPtr->scale.worldWindow.bottom ||
      tmpBox.bottom > dInfoPtr->scale.worldWindow.top)
    return;

  if (dInfoPtr->checked == FALSE ) {
    if (tmpBox.left < dInfoPtr->scale.worldWindow16.left)
      tmpBox.left = dInfoPtr->scale.worldWindow16.left;
    if (tmpBox.right > dInfoPtr->scale.worldWindow16.right)
      tmpBox.right = dInfoPtr->scale.worldWindow16.right;
    if (tmpBox.top > dInfoPtr->scale.worldWindow16.top)
      tmpBox.top = dInfoPtr->scale.worldWindow16.top;
    if (tmpBox.bottom < dInfoPtr->scale.worldWindow16.bottom)
      tmpBox.bottom = dInfoPtr->scale.worldWindow16.bottom;
  }

  i = MAX (dInfoPtr->scale.worldWindow.left, tmpBox.left);
  /*  i = i + 10 * curScale - (i % (10 * curScale)) - 1;*/
  while (i % (10 * curScale) != 0) {
    i++; /* !!! do this the right way */
  }
  for (tickCount = (i / (10 * curScale)) % 10;
       i < MIN (dInfoPtr->scale.worldWindow.right, to);
       i += curScale * 10) {
    pnt.x = i;
    pnt.y = tmpBox.top;
    MapWorldPointToPixel (&pt, &pnt, &dInfoPtr->scale);
    MoveTo (pt.x, pt.y);

    if (tickCount == 0 || tickCount == 5) {
      pnt.y = tmpBox.top - scaleHeight; /* draw full-height tick */
      MapWorldPointToPixel (&pt, &pnt, &dInfoPtr->scale);
      LineTo (pt.x, pt.y);
      if (tickCount == 0
          /* if i + 100curScale > the right boundary, this is the last label */
          && (!((i + 100 * curScale >= to) && ssp->disableLastLabel))
          && (!((i - 100 * curScale <= from) && ssp->disableFirstLabel))
          && (i != from)) {
        sprintf (buffer, "%ld", (long)((i == 0) ? 1 : i) + ssp->labelOffset); /* put the origin at 1 instead of 0 */
        pt.x -= StringWidth (buffer) / 2; /* center text around the tick mark*/
        pt.y += 1 + Ascent ();
        PaintStringEx (buffer, pt.x, pt.y);
      }
      /* disable the right-end one in the case of an overlap */
    } else {
      pnt.y = tmpBox.top - scaleHeight / 2;
      MapWorldPointToPixel (&pt, &pnt, &dInfoPtr->scale);
      LineTo (pt.x, pt.y);
    }
    tickCount = (tickCount + 1) % 10;
  }

  /* also, we might need to redraw the labels just to the left or to the right of the update region. */
  i = MAX (dInfoPtr->scale.worldWindow.left, tmpBox.left);
  j = i; /* j = leftmost visible pixel in world coordinates */
  while (i % (100 * curScale) != 0) {
    i--;
  }
  pnt.x = i;
  sprintf (buffer, "%ld", (long)((i == 0) ? 1 : i) + ssp->labelOffset);
  i += curScale * ((StringWidth (buffer) / 2) + 1);
  if (i >= j
      && i > from
      && (!((pnt.x + 100 * curScale >= to) && ssp->disableLastLabel))
      && (!((pnt.x - 100 * curScale <= from) && ssp->disableFirstLabel))
      && pnt.x > from) {
    pnt.y = tmpBox.top - scaleHeight;
    MapWorldPointToPixel (&pt, &pnt, &dInfoPtr->scale);
    pt.x -= StringWidth (buffer) / 2; /* center text around the tick mark*/
    pt.y += 1 + Ascent ();
    PaintStringEx (buffer, pt.x, pt.y);
  }
  /* now check the right side*/

  i = MIN (dInfoPtr->scale.worldWindow.right, to);
  j = i; /* j = rightmost visible pixel in world coordinates */
  while (i % (100 * curScale) != 0) {
    i++;
  }
  pnt.x = i;
  sprintf (buffer, "%ld", (long)i + ssp->labelOffset);
  i -= curScale * ((StringWidth (buffer) / 2) + 1);
  if (i <= j
      && (!((pnt.x + 100 * curScale >= to) && ssp->disableLastLabel))
      && (!((pnt.x - 100 * curScale <= from) && ssp->disableFirstLabel))
      && pnt.x < to) {
    pnt.y = tmpBox.top - scaleHeight;
    MapWorldPointToPixel (&pt, &pnt, &dInfoPtr->scale);
    pt.x -= StringWidth (buffer) / 2; /* center text around the tick mark*/
    pt.y += 1 + Ascent ();
    PaintStringEx (buffer, pt.x, pt.y);
  }
}

static void ScaleSntCleanupProc (
  BigScalar calldata
)

{
  ScaleSntPtr  ssp;

  ssp = (ScaleSntPtr) calldata;
  MemFree (ssp);
}

static Boolean LIBCALLBACK Asn2gphSegmentExploreProc (
  SeqLocPtr slp,
  SeqMgrSegmentContextPtr context
)

{
  /* context->userdata a pointer to vContext */
  RelevantFeatureItemPtr RFIP;
  SeqIdPtr           sip, newid = NULL;
  Int4               gi;
  Char               labelBuf [100];
  Int4               left, right;
  ViewerContextPtr   vContext;
  AppearancePtr      AP;
  BioseqAppearanceItemPtr BioAIP;
  BioseqPtr          bsp;

  if ((RFIP = MemNew (sizeof (RelevantFeatureItem))) == NULL) return FALSE;
  vContext = context->userdata;
  bsp = vContext->BSP;
  ValNodeAddPointer (&vContext->BSPsegmentVNP, 0, RFIP);
  sip = SeqLocId (slp);
  if (sip != NULL && sip->choice == SEQID_GI /* && BioseqFindCore (sip) != NULL */) {
    gi = sip->data.intvalue;
    newid = SeqIdStripLocus (GetSeqIdForGI (gi));
    sip = newid;
    if (newid != NULL) {
      ValNodeAddInt (&newid, SEQID_GI, gi);
    }
  }
  
  AP = vContext->AppPtr;
  BioAIP = AP->bioseqAIP;

  left = context->from;
  right = context->to - left + context->cumOffset;
  left = context->cumOffset;
  if (sip != NULL) {
    if (BioAIP->format != PRINTID_FASTA_LONG) { 
      sip = SeqIdFindWorst (sip);
    }
    SeqIdWrite (sip, labelBuf, BioAIP->format, sizeof (labelBuf) - 1);
    RFIP->ContentLabel = StringSave (labelBuf); /* record this in a list of things to be freed so that we dont leak memory!!!!! */
  }
  SeqIdSetFree (newid);
  RFIP->Left = MIN (left, right);
  RFIP->Right = MAX (left, right);
  RFIP->entityID = context->entityID;
  RFIP->itemID = context->itemID;
  if (bsp->repr == Seq_repr_seg) {
    RFIP->itemType = OBJ_BIOSEQ_SEG;
  } else if (bsp->repr == Seq_repr_delta) {
    RFIP->itemType = OBJ_BIOSEQ_DELTA;
  } else {
    RFIP->itemType = OBJ_BIOSEQ_SEG;
  }
  RFIP->numivals = 1;
  RFIP->featdeftype = 0;
  RFIP->featstrand = context->strand;
  RFIP->circularSpanningOrigin = FALSE;
  return TRUE;
}

static Int4 DrawBioseqSegments (
  Int4 initialYOffset,
  FilterItemPtr FIP,
  ViewerContextPtr vContext
)

{
  ValNodePtr   listHead = NULL, vnpSegment = NULL;
  ValNodePtr   firstRow = NULL;
  Int4         segmentCount;
  Uint2         rowHeight, row = 0; /* row is either 0 or 1 */
  AppearanceItem segmentAI;
  AppearanceItemPtr oldAIPzero;
  AppearancePtr AP;
  BioseqAppearanceItemPtr BioAIP;
  RelevantFeatureItemPtr RFIP;
  FilterProcessState FPS;
  Uint2         oldMaxScale;

  vContext->BSPsegmentVNP = NULL;
  SeqMgrExploreSegments (vContext->BSP, vContext, Asn2gphSegmentExploreProc);
  listHead = vContext->BSPsegmentVNP;
  segmentCount = 0;
  for (vnpSegment = listHead; vnpSegment != NULL; vnpSegment = vnpSegment->next) {
    segmentCount++;
  }

  if (segmentCount <= 1) {
    ValNodeFreeData (listHead);
    return 0;
  }

  oldMaxScale = vContext->FltPtr->MaxScaleWithLabels;
  vContext->FltPtr->MaxScaleWithLabels = 10000;
  AP = vContext->AppPtr;
  BioAIP = AP->bioseqAIP;
  MemCpy (segmentAI.LabelColor, BioAIP->labelColor, sizeof (segmentAI.LabelColor));
  MemCpy (segmentAI.Color, BioAIP->bioseqColor, sizeof (segmentAI.Color));
  segmentAI.RenderChoice = Render_Box;
  segmentAI.Height = BioAIP->height;
  segmentAI.AddDescToLabel = TRUE;
  segmentAI.AddTypeToLabel = FALSE;
  segmentAI.LabelFont = BioAIP->labelFont;
  segmentAI.LabelLoc = FIP->LabelLoc;
  FIP->LabelLoc = LabelAboveClip;

  segmentAI.ShowArrow = FALSE;
  segmentAI.VibLinestyle = SOLID_LINE;
  segmentAI.VibShading = SOLID_SHADING;
  oldAIPzero = vContext->AppPtr->FeaturesAppearanceItem[0];
  vContext->AppPtr->FeaturesAppearanceItem[0] = &segmentAI;

  MemSet (&FPS, 0, sizeof (FPS));
  FPS.currentFIP = FIP;
  FPS.ceiling = initialYOffset;
  FPS.vContext = vContext;

  FPS.renderParm.AIP = &segmentAI;
  FPS.renderParm.FIP = FIP;

  SelectFont (segmentAI.LabelFont);
  rowHeight = LineHeight () + segmentAI.Height + 3 + FIP->IntraRowPaddingPixels;
  row = 0;

  for (vnpSegment = listHead; vnpSegment != NULL; vnpSegment = vnpSegment->next) {

    RFIP = vnpSegment->data.ptrvalue;
    EnsureFeatureHasSegment (&FPS, 0, vContext->drawOnMe);
    if (!BuildRenderInputFromRFIP (&FPS.renderParm, RFIP, &FPS)) {
      continue;
    }
    row = 1 - row; /* alternate between 0 and 1 */
    FPS.renderParm.rowHeight += 2;
    FPS.renderParm.yStart = initialYOffset - (row * rowHeight);
    DrawFeatureAndLabel (&FPS.renderParm, vContext);
  }
  if (FPS.needFreeList != NULL) {
    FPS.needFreeList = ValNodeFreeData (FPS.needFreeList);
    FPS.lastInFreeList = NULL;
  }
  ValNodeFreeData (listHead);
  vContext->BSPsegmentVNP = NULL;
  vContext->AppPtr->FeaturesAppearanceItem[0] = oldAIPzero;
  vContext->FltPtr->MaxScaleWithLabels = oldMaxScale;
  return 2 * rowHeight + 7 + FIP->IntraRowPaddingPixels + FIP->GroupPadding;
}

static Int4 DrawBioseq (
  Int4 initialYOffset,
  FilterItemPtr FIP,
  ViewerContextPtr vContext
)

{
  PrimitivE               thisPrim;
  Char                    labelbuf[128];
  BioseqPtr               bsp;
  ScaleSntPtr             ssp;
  Uint2                   labelLineHeight, lastLabelWidth, firstLabelWidth;
  SegmenT                 seg;
  SeqIdPtr                sip;
  BioseqAppearanceItemPtr bioseqAIP;
  Uint1                   scaleHeight;
  Uint1                   height;
  Int4                    yOffset;
  Uint4                   i, j;
  Boolean                 drawScale;
  AppearancePtr           AP;

  AP = vContext->AppPtr;
  bioseqAIP = AP->bioseqAIP;
  yOffset = initialYOffset;
  bsp = vContext->BSP;

  drawScale = bioseqAIP->drawScale;
  if (FIP->DrawScale != TristateUnset) {
    drawScale = BOOL_FROM_SET_TRISTATE(FIP->DrawScale);
  }

  scaleHeight = bioseqAIP->scaleHeight;
  if (bioseqAIP->format != PRINTID_FASTA_LONG) {
    sip = SeqIdFindWorst (bsp->id);
  } else {
    sip = bsp->id;
  }
  SeqIdWrite (sip, labelbuf, bioseqAIP->format, sizeof (labelbuf) - 1);
  seg = CreateSegment (vContext->topLevelSeg, 0, 0);

  if (bioseqAIP->labelLoc == LabelOnTop) {
    AddAttribute (seg, COLOR_ATT, bioseqAIP->labelColor, 0, 0, 1, 0);
    thisPrim = AddTextLabel (seg, vContext->from, yOffset, labelbuf, bioseqAIP->labelFont, 0, LOWER_RIGHT, 0);
    SetPrimitiveIDs (thisPrim, bsp->idx.entityID, bsp->idx.itemID, OBJ_BIOSEQ, 0);
    SelectFont (bioseqAIP->labelFont);
    height = LineHeight() + 1;
    yOffset = initialYOffset - height;
  } else {
    height = 0;
  }

  AddAttribute (seg, COLOR_ATT, bioseqAIP->bioseqColor, 0, 0, 1, 0);
  thisPrim = AddRectangle (seg, vContext->from, yOffset, vContext->to, yOffset - bioseqAIP->height, NO_ARROW, TRUE, 0);
  SetPrimitiveIDs (thisPrim, bsp->idx.entityID, bsp->idx.itemID, OBJ_BIOSEQ, 0);
  AddAttribute (seg, COLOR_ATT, bioseqAIP->labelColor, 0, 0, 1, 0);

  height += bioseqAIP->height + 2;
  yOffset = initialYOffset - height;

  if (bioseqAIP->labelLoc == LabelOnSide) {
    AddAttribute (seg, COLOR_ATT, bioseqAIP->labelColor, 0, 0, 1, 0);
    thisPrim = AddTextLabel (seg, vContext->from, yOffset, labelbuf, bioseqAIP->labelFont, 1, UPPER_LEFT, 0);
    SetPrimitiveIDs (thisPrim, bsp->idx.entityID, bsp->idx.itemID, OBJ_BIOSEQ, 0);
  } else if (bioseqAIP->labelLoc == LabelOnBottom) {
    AddAttribute (seg, COLOR_ATT, bioseqAIP->labelColor, 0, 0, 1, 0);
    thisPrim = AddTextLabel (seg, 0, yOffset, labelbuf, bioseqAIP->labelFont, 1, LOWER_RIGHT, 0);
    SetPrimitiveIDs (thisPrim, bsp->idx.entityID, bsp->idx.itemID, OBJ_BIOSEQ, 0);
    SelectFont (bioseqAIP->labelFont);
    height += LineHeight() + 2;
    yOffset = initialYOffset - height;
  }

  if (drawScale) {

    ssp = (ScaleSntPtr) MemNew (sizeof (ScaleSntData));
    if (ssp == NULL) return height;
    SelectFont (bioseqAIP->scaleFont);
    labelLineHeight = LineHeight();
    ssp->seg = seg;
    ssp->labelOffset = 0;
    ssp->length = vContext->to - vContext->from;
    ssp->box.left = vContext->from;
    ssp->box.right = vContext->to;
    ssp->box.top = yOffset;
    ssp->box.bottom = yOffset - 2 - scaleHeight - labelLineHeight;
    ssp->bioseqAIP = bioseqAIP;
    
    i = vContext->to - vContext->to % (100 * vContext->viewScale); /* the last labelled tick-mark */
    
    sprintf (labelbuf, "%ld", (long)i + ssp->labelOffset);
    j = StringWidth (labelbuf); /* j is the width (pixels) of the last tick mark's label */
    
    sprintf (labelbuf, "%ld", (long)vContext->to + ssp->labelOffset);
    lastLabelWidth = (StringWidth (labelbuf) + 2) * vContext->viewScale;
    
    if (vContext->from > 0) {
      sprintf (labelbuf, "%ld", (long)vContext->from + ssp->labelOffset);
    } else {
      sprintf (labelbuf, "1");
    }
    firstLabelWidth = StringWidth (labelbuf) * vContext->viewScale;
    
    i += vContext->viewScale * j / 2; /* right-most edge of the centered label */
    j = MAX (j, 10); /* always leave at least 10 pixels between the last 2 labels */
    if (i + j * vContext->viewScale >= vContext->to - lastLabelWidth) {
      ssp->disableLastLabel = TRUE;
    } else {
      ssp->disableLastLabel = FALSE;
    }
    
    i = vContext->from + (100 * vContext->viewScale) - vContext->from % (100 * vContext->viewScale); /* the second label (first is at the origin) */
    sprintf (labelbuf, "%ld", (long)vContext->from + ssp->labelOffset);
    if (i - ((StringWidth (labelbuf) / 2 + 5) * vContext->viewScale) <= vContext->from + firstLabelWidth) {
      ssp->disableFirstLabel = TRUE;
    } else {
      ssp->disableFirstLabel = FALSE;
    }
    
    ssp->snt = AddSntRectangle (seg, vContext->from, yOffset,
                                vContext->to + lastLabelWidth, yOffset - scaleHeight - labelLineHeight,
                                ScaleSntDrawProc, (BigScalar) ssp, ScaleSntCleanupProc, 0);
    /*  height += scaleHeight + labelLineHeight + 4;*/
    yOffset = ssp->box.bottom - 1 - 15;
  } else {
    yOffset++;
  }
  yOffset -= DrawBioseqSegments (yOffset, FIP, vContext);
  return initialYOffset - yOffset;
}

/* -=-=-=-==-=-==-=-= GRAPH STUFF =-=-=-=-=-=-=-=-=-*/

typedef struct gphsentdata {
  Boolean        flagIsGUI;
  SegmenT        seg;
  PrimitivE      snt;
  BoxInfo        box;
  Int4           min;
  Int4           max;
  Int4           axis;
  Int4           bottom;
  FloatHi        a;
  FloatHi        b;
  SeqGraphPtr    sgp;
  Uint1          red, green, blue;
} GphSentData, PNTR GphSentPtr;

static void PlotTheWorkingArray (Int4Ptr xp, Int4 len, Int4 scaleX,
                                 Int4 gmin, RecT r, Uint1 uR, Uint1 uG,
                                 Uint1 uB, AttPData PNTR curattrib)
{
  PoinT          pt;
  Int4           i;
  Int4           j;
  Int4           max;
  Int4           min;
  Int4           val;
  Uint1          curR, curG, curB;

  if (curattrib != NULL)
  {
    curR = curattrib->color[0];
    curG = curattrib->color[1];
    curB = curattrib->color[2];
  }
  else
  {
    curR = 0;
    curG = 0;
    curB = 0;
  }

  SelectColor (uR, uG, uB);
  pt.x = r.left;
  i = 0;
  val = (Int4) *xp;
  pt.y = (Int2) (r.bottom - (Int2) (val - gmin));
  if (pt.y < r.top) {
    pt.y = r.top;
  }
  MoveTo (pt.x, pt.y);
  for (; i < len - scaleX; i += scaleX) {
    val = (Int4) *xp;
    min = (Int4) val;
    max = (Int4) val;
    for (j = 1; j < scaleX; j++) {
      xp++;
      val = (Int4) *xp;
      min = MIN (min, (Int4) val);
      max = MAX (max, (Int4) val);
    }
    xp++;
    pt.y = (Int2) (r.bottom - (Int2) (min - gmin));
    if (pt.y < r.top) {
      pt.y = r.top;
    }
    LineTo (pt.x, pt.y);
    pt.y = (Int2) (r.bottom - (Int2) (max - gmin));
    if (pt.y < r.top) {
      pt.y = r.top;
    }
    LineTo (pt.x, pt.y);
    (pt.x)++;
  }
  SelectColor (curR, curG, curB);
  return;
}

static Int4Ptr MakeWorkingSeqGraphInt4Array (Pointer data, Uint1 type,
                                             Int4 pos, Int4 len,
                                             Boolean usescaleflags,
                                             FloatHi a, FloatHi b)
{
  Int4          i;
  Int4Ptr       xpoints, xp;
  FloatHi       fval;
  Int4          ival;
  Byte          bval;
  ByteStorePtr  bs;
  BytePtr       bp;

  xpoints = (Int4Ptr) MemNew ((size_t) (sizeof (Int4) * (len + 10)));

  if (xpoints == NULL)
    return xpoints;

  xp = xpoints;
  switch (type) {
   default:
   case 1:
     for (i = 0; i < len; i++, pos++, xp++) {
      if (usescaleflags) {
        fval = a * (FloatHi) (((FloatHiPtr) data)[pos]) + b;
      } else {
        fval = (FloatHi) (((FloatHiPtr) data)[pos]);
      }
      *xp = (Int4) fval;
    }
    break;
   case 2:
    for (i = 0; i < len; i++, pos++, xp++) {
      if (usescaleflags) {
        ival = (Int4) (a * (FloatHi) (((Int4Ptr) data)[pos]) + b);
      } else {
        ival = (Int4) (((Int4Ptr) data)[pos]);
      }
      *xp = (Int4) ival;
    }
    break;
   case 3:
    bp = MemNew (sizeof (Byte) * (len + 10));
    bs = (ByteStorePtr) data;
    BSSeek (bs, pos, SEEK_SET);
    len = BSRead (bs, (Pointer) bp, len * sizeof (Byte));
    for (i = 0; i < len; i++, pos++, xp++) {
      if (usescaleflags) {
        bval = (Byte) (a * (FloatHi) (bp [i]) + b);
      } else {
        bval = (Byte) (bp [i]);
      }
      *xp = (Int4) bval;
    }
    MemFree (bp);
    break;
  }
  return xpoints;
}

static void GphSentProc (BigScalar calldata, PrimDrawContext pdc)
{
  BioseqPtr    bsp;
  Int4         curScale;
  DrawInfoPtr  dInfoPtr;
  GphSentPtr   gsp;
  Int4         left;
  Int4         len, pos;
  RecT         r;
  Int4         scaleX;
  Int4         scaleY;
  SeqGraphPtr  sgp;
  BoxInfo      tmpBox;
  Int4Ptr      xpoints;
  Int4         tmpscaleX;

  gsp = (GphSentPtr) calldata;
  if (gsp == NULL) return;
  sgp = gsp->sgp;
  if (sgp == NULL || sgp->values == NULL) return;

  dInfoPtr = (DrawInfoPtr) pdc;
  tmpBox = gsp->box;
  if ( (tmpBox.left   > dInfoPtr->scale.worldWindow.right ) ||
       (tmpBox.right  < dInfoPtr->scale.worldWindow.left  ) ||
       (tmpBox.top    < dInfoPtr->scale.worldWindow.bottom) ||
       (tmpBox.bottom > dInfoPtr->scale.worldWindow.top   ) )
    return;

  if ( dInfoPtr->checked == FALSE ) {
    if ( tmpBox.left < dInfoPtr->scale.worldWindow16.left )
      tmpBox.left = dInfoPtr->scale.worldWindow16.left;
    if ( tmpBox.right > dInfoPtr->scale.worldWindow16.right )
      tmpBox.right = dInfoPtr->scale.worldWindow16.right;
    if ( tmpBox.top > dInfoPtr->scale.worldWindow16.top )
      tmpBox.top = dInfoPtr->scale.worldWindow16.top;
    if ( tmpBox.bottom < dInfoPtr->scale.worldWindow16.bottom )
      tmpBox.bottom = dInfoPtr->scale.worldWindow16.bottom;
  }
  scaleX = dInfoPtr->scale.scaleX;
  scaleY = dInfoPtr->scale.scaleY;

  curScale = dInfoPtr->scale.scaleX;
  r.left   = (Int2)((dInfoPtr->scale.offsetX + tmpBox.left )  / curScale);
  r.right  = (Int2)((dInfoPtr->scale.offsetX + tmpBox.right)  / curScale);
  curScale = dInfoPtr->scale.scaleY;
  r.top    = (Int2)((dInfoPtr->scale.offsetY - tmpBox.top   ) / curScale);
  r.bottom = (Int2)((dInfoPtr->scale.offsetY - tmpBox.bottom) / curScale);

  bsp = BioseqFind (SeqLocId (sgp->loc));
  left = GetOffsetInBioseq (sgp->loc, bsp, SEQLOC_LEFT_END);

  if (gsp->flagIsGUI)
  {
    len = tmpBox.right - tmpBox.left;
    pos = tmpBox.left - left;
  }
  else
  {
/* for non-GUI, plot all the sgp values */
    len = sgp->numval;
    pos = 0;
  }

  xpoints = MakeWorkingSeqGraphInt4Array (sgp->values, sgp->flags[2],
                                          pos, len,
                                          (Boolean) (sgp->flags[1] != 0),
                                          gsp->a, gsp->b);
  if (!gsp->flagIsGUI)
  {
    tmpscaleX = sgp->numval / (Int4) (tmpBox.right - tmpBox.left);
    if (tmpscaleX > scaleX)
      scaleX = tmpscaleX;
  }

  PlotTheWorkingArray (xpoints, len, scaleX, gsp->min, r,
                       gsp->red, gsp->green, gsp->blue,
                       &(dInfoPtr->curattrib));

  MemFree (xpoints);
}

static void CleanGSP (BigScalar calldata)
{
  GphSentPtr  gsp;

  gsp = (GphSentPtr) calldata;
  MemFree (gsp);
}

static GphSentPtr AddGphSentinelToPicture (SeqGraphPtr sgp, BioseqPtr bsp,
                                           SegmenT pict, Int4 scaleX,
                                           Int4 top, Int2 start,
                                           Uint1Ptr uRGB, Boolean drawScale)
{
  Int4        axis;
  GphSentPtr  gsp;
  Int4        i;
  Boolean     is_phrap;
  Int4        max;
  Int4        min;
  SegmenT     seg;
  Char        str [32];
  Int4        leftoff, rightoff;

  if (sgp == NULL || bsp == NULL || pict == NULL)
    return NULL;
  gsp = MemNew (sizeof (GphSentData));
  if (gsp == NULL)
    return NULL;
  gsp->flagIsGUI = VibrantIsGUI ();
  if (uRGB != NULL)
  {
    gsp->red   = uRGB[0];
    gsp->green = uRGB[1];
    gsp->blue  = uRGB[2];
  }
  else
  {
    gsp->red   = 0;
    gsp->green = 0;
    gsp->blue  = 0;
  }

  leftoff = GetOffsetInBioseq (sgp->loc, bsp, SEQLOC_LEFT_END);
  rightoff = GetOffsetInBioseq (sgp->loc, bsp, SEQLOC_RIGHT_END);
  if (!gsp->flagIsGUI)
  {
    leftoff /= scaleX;
    rightoff /= scaleX;
  }
  gsp->box.left = leftoff + start;
  gsp->box.right = rightoff - 1 + start;

  gsp->sgp = sgp;
  gsp->a = sgp->a;
  gsp->b = sgp->b;
  is_phrap = (Boolean) (StringICmp (sgp->title, "Phrap Quality") == 0 ||
                        StringICmp (sgp->title, "Phred Quality") == 0);
  switch (sgp->flags [2]) {
    case 1 :
      min = (Int4) sgp->min.realvalue;
      max = (Int4) sgp->max.realvalue;
      axis = (Int4) sgp->axis.realvalue;
      if (sgp->flags [1] != 0) {
        min = (Int4) (sgp->a * ((FloatHi) min) + sgp->b);
        max = (Int4) (sgp->a * ((FloatHi) max) + sgp->b);
      }
      break;
    case 2 :
      min = (Int4) sgp->min.intvalue;
      max = (Int4) sgp->max.intvalue;
      axis = (Int4) sgp->axis.intvalue;
      if (sgp->flags [1] != 0) {
        min = (Int4) (sgp->a * ((FloatHi) min) + sgp->b);
        max = (Int4) (sgp->a * ((FloatHi) max) + sgp->b);
      }
      break;
    case 3 :
      min = (Int4) sgp->min.intvalue;
      max = (Int4) sgp->max.intvalue;
      if (is_phrap) {
        min = MIN (0, min);
        max = MAX (100, max);
      }
      axis = (Int4) sgp->axis.intvalue;
      if (sgp->flags [1] != 0) {
        min = (Int4) (sgp->a * ((FloatHi) min) + sgp->b);
        max = (Int4) (sgp->a * ((FloatHi) max) + sgp->b);
      }
      break;
    default :
      min = (Int4) 0;
      max = (Int4) 100;
      axis = (Int4) 0;
      break;
  }
  gsp->seg = seg = CreateSegment (pict, 0, 0);
  gsp->bottom = top - (max - min) - 20;

  if (drawScale && sgp->title != NULL)  /* StringHasNoText -- vibforms */
  {
    if (StringLen (sgp->title) > 0)
    {
      AddLabel (seg, 
                /* (gsp->box.left + gsp->box.right) / 2, */
                (start + bsp->length) / 2, 
                top,
                sgp->title, SMALL_TEXT, 0, MIDDLE_CENTER, 0);
    }
  }

  top -= 10;
  gsp->box.top = top;
  gsp->box.bottom = gsp->bottom;
  gsp->min = min;
  gsp->max = max;
  gsp->axis = axis;
  gsp->bottom += 10;

  if (drawScale)
  {
      if (is_phrap)
      {
        for (i = 0; i <=100; i += 20) {
          sprintf (str, "%ld", (long) i);
          AddLabel (seg, gsp->box.left, gsp->bottom + i, str,
                    SMALL_TEXT, 5, MIDDLE_LEFT, 0);
        }
      }
      else
      {
        sprintf (str, "%ld", (long) max);
        AddLabel (seg, gsp->box.left, top-10, str,
                  SMALL_TEXT, 5, MIDDLE_LEFT, 0);
        sprintf (str, "%ld", (long) min);
        AddLabel (seg, gsp->box.left, gsp->bottom-10, str,
                  SMALL_TEXT, 5, MIDDLE_LEFT, 0);
        if (min < 0 && max > 0)
        {
          sprintf (str, "%ld", 0L);
          AddLabel (seg, gsp->box.left, gsp->bottom-min-10, str,
                    SMALL_TEXT, 5, MIDDLE_LEFT, 0);
        }
     }
  }

  gsp->snt = AddSntRectangle (seg, gsp->box.left, gsp->box.top,
                              gsp->box.right, gsp->box.bottom,
                              GphSentProc, (BigScalar) gsp, CleanGSP, 0);
  return gsp;
}

static void VisitAndListGraphs (SeqGraphPtr sgp, Pointer userdata)

{
  GphSentPtr        gsp;
  ViewerContextPtr  vContext;
  Boolean           drawScale = FALSE;
  
  vContext = (ViewerContextPtr) userdata;
  if (vContext == NULL) return;

  if (vContext->gphseg == NULL) {
    vContext->gphseg = CreateSegment (vContext->drawOnMe, 0, 0);
    
    /* 
       this first time 'drawScale' stuff here and in AddGphSentinelToPicture
       assumes that all the graphs on this bioseq are the same type
       use the same legend and have the same label. 
    */
    AddSegRect (vContext->gphseg, FALSE, 0); /* draw box around all the graphs, not each one individually */
    drawScale = TRUE;   /* only draw scale on the first graph */
  }

  gsp = AddGphSentinelToPicture (sgp, vContext->BSP, vContext->gphseg,
                                 vContext->viewScale, vContext->gphyOffset,
                                 0, NULL, drawScale);
  if (gsp == NULL) return;
  vContext->gphheight = MAX (vContext->gphheight, gsp->box.top - gsp->box.bottom);
}

static Int4 DrawGraphs (
  Int4 yOffset,
  ViewerContextPtr vContext
)

{
  vContext->gphheight = 0;
  vContext->gphseg = NULL;
  vContext->gphyOffset = yOffset - 16; /* (workaround) drawing the graphs touches some pixels above gphyOffset  */

  VisitGraphsOnBsp (vContext->BSP, (Pointer) vContext, VisitAndListGraphs);

  return vContext->gphheight + 16;
}

static void ResetFilterState (
  FilterProcessStatePtr FPSP
)

{
  FilterItemPtr FIP;
  ViewerContextPtr vContext;


  if (FPSP == NULL) return;
  vContext = FPSP->vContext;
  FIP = FPSP->currentFIP;
  MemSet (&FPSP->state, 0, sizeof (FilterState));
  if (FIP->Type == FeatureFilter) {
    FPSP->state.feat.currentRFIPblockVNP = vContext->featVNP;
  }
}


/*
  Order by accession (label), strand and location
  for sorting.
*/
static int LIBCALLBACK CompareAlignNameLoc(VoidPtr ptr1, VoidPtr ptr2)
{
  SeqAlignSortInfoPtr saip1, saip2;
  Int2                  labelcomp;
  
  if (ptr1 != NULL && ptr2 != NULL) {
    saip1 = (SeqAlignSortInfoPtr) ptr1;
    saip2 = (SeqAlignSortInfoPtr) ptr2;

    labelcomp = StrNCmp(saip1->label, saip2->label, MAX_ALIGN_SORT_LABEL);
    if (labelcomp != 0)
      return labelcomp;
      
    if (saip1->strand < saip2->strand)
      return -1;
    if (saip1->strand > saip2->strand)
      return 1;
      
    if (saip1->start < saip2->start)
      return -1;
    if (saip1->start > saip2->start)
      return 1;
    if (saip1->stop < saip2->stop)
      return -1;
    if (saip1->stop > saip2->stop)
      return 1;
  }
  return 0; 
}

static int LIBCALLBACK ScoreCompare( VoidPtr p1, VoidPtr p2)
{
  if (p1 != NULL  &&  p2 != NULL) {
    Int4 i1 = * (Int4Ptr) p1;
    Int4 i2 = * (Int4Ptr) p2;
    
    return i1 - i2;
  }
  return 0;
}

/*
  Take a number (rawscore) between min and max (inclusive) 
  and linearly scale it into a number from 1 to ACCUMVALUE_MAX/256.
  We divide by 256 since we will be adding these numbers together when smearing.
  Hopefully we won't have more than 256 with large scores at any one point  :)
  Do not call if min >= max.
*/
AccumValue_t NormaliseScore(Int4 rawscore, Int4 min, Int4 max)
{
    AccumValue_t retval;
    FloatHi fscore = rawscore - min;
    fscore *=  ACCUMVALUE_MAX / 256 - 1;
    fscore /= (max - min);
    retval = fscore + 1; 
    return retval;
}


static void FindCutoffScore(SeqAlignPtr sap, Int4 alignCnt, AlignmentFilterStatePtr afsp)
{
  SeqAlignPtr sapIter;
  Int4Ptr     allScores;
  Int4        i;
  /* Int4        cutoffScore;  /* not normalized */
  
  /* make array to hold all the scores. */
  allScores = MemNew(alignCnt * sizeof(Int4));
  if (allScores == NULL)
      return;

  /* get all the scores */
  i = 0;
  for (sapIter = sap; sapIter != NULL; sapIter = sapIter->next) {
    allScores[i++] = WeightFromAlignScore(sapIter, afsp->scoreType);
  }
    
  /* sort them */
  HeapSort ( allScores, alignCnt, sizeof(Int4), ScoreCompare);
  
  /* find, normalise and return the cutoffPercent score 
     and the min & max scores so we can normalise the rest of the scores later.
  */
  afsp->minScore = allScores[0];
  afsp->maxScore = allScores[alignCnt - 1];
  afsp->cutoffScoreHi = ACCUMVALUE_MAX; /* could add other controls to set this. */
/* Cutoff by actual score, not by percentage.
  if (afsp->cutoffPercent == 100  ||  
      afsp->scoreType == NLM_SCORE_COUNT ||
      afsp->minScore >= afsp->maxScore ) {
    afsp->cutoffScore = 0;
  }
  else {
    cutoffScore = allScores[ (100 - afsp->cutoffPercent) * alignCnt/100 ];
    afsp->cutoffScore = NormaliseScore(cutoffScore, afsp->minScore, afsp->maxScore);
  }
*/  
  MemFree(allScores);
}


/* Gather all the SeqAligns whose normalised score is less than the cutoff score */
static void GatherAlignInfo(
  SeqAlignPtr sap, 
  Int4        alignCnt,
  SeqIdPtr    bioSeqId,
  AlignmentFilterStatePtr afsp
)
{
  SeqAlignSortInfoPtr  infoArray = NULL;
  Int4        infoIndex;
  SeqAlignPtr sapIter;
  Int4        nrows, alignRow;
  Int4        start, stop, swap;
  Uint1       strand;
  Int4        rawScore;
  AccumValue_t  normScore;
  
  infoArray = MemNew(alignCnt * sizeof(SeqAlignSortInfo));
  if (infoArray == NULL)
    return;
  
  infoIndex = 0;
  for (sapIter = sap; sapIter != NULL; sapIter = sapIter->next) 
  {
    /* is this alignment in the top percentile? */
    if (afsp->scoreType != NLM_SCORE_COUNT  &&  afsp->minScore < afsp->maxScore ) {
      rawScore = WeightFromAlignScore(sapIter, afsp->scoreType);
      normScore = NormaliseScore( rawScore, afsp->minScore, afsp->maxScore);
  /*    if (normScore < afsp->cutoffScore ||  afsp->cutoffScoreHi < normScore) */
      if (rawScore < afsp->cutoffScore ||  afsp->cutoffScoreHi < rawScore)
      {
        continue;
      }
    } else {
      normScore = 1;
    }
    
    if (!AlnMgr2IndexSingleChildSeqAlign (sapIter)) continue; /* make sure we are indexed */ 

    /* get dimensions of this alignment (number of sequences aligned) */   
    nrows = AlnMgr2GetNumRows(sapIter);
    if (nrows < 1) {
      nrows = sapIter->dim; 
    }     
    if (nrows != 2) {  /* can't handle 3+ dimension alignments */
      continue;
    }
    
    /* Get the beginning and end points of this alignment in Bioseq coords. */
    if (sapIter->segtype != SAS_STD) {  
      /* not Std Seg alignment, use indexed functions */
      alignRow = AlnMgr2GetFirstNForSipList (sapIter, bioSeqId);
      if (alignRow == -1) {
        continue;
      }
      AlnMgr2GetNthSeqRangeInSA(sapIter, alignRow, &start, &stop);
      strand = AlnMgr2GetNthStrand(sapIter, alignRow);
    } else { 
      /* Std Seg alignment. Use special function */
      AlnMgr2GetSeqRangeForSipInSAStdSeg(sapIter, bioSeqId, &start, &stop, &strand);
    }
    if (start < 0) {
      continue;
    }
    if (stop < start) {
      swap = stop;
      stop = start;
      start = swap;
    }

    /* populate the structure we will use to sort on. */
    infoArray[infoIndex].start = start;
    infoArray[infoIndex].stop  = stop;
    infoArray[infoIndex].strand = strand;
    infoArray[infoIndex].sap   = sapIter;
    infoArray[infoIndex].normScore = normScore;
    if (!SeqAlignContentLabel(sapIter, bioSeqId, infoArray[infoIndex].label, MAX_ALIGN_SORT_LABEL, PRINTID_TEXTID_ACC_VER)) {
      infoArray[infoIndex].label[0] = 0;
    }
    ++infoIndex;
  }
  
  afsp->alignSortedLen = infoIndex;
    
  if (infoIndex == 0) {
    MemFree(infoArray);
    infoArray = NULL;
  }
  afsp->alignSorted = infoArray;
}

static Boolean AlignmentFilterStateInit(
    SeqAlignPtr sap,                /* the head of hte chain of sequence alignments from the bioseq */
    SeqIdPtr sid,                   /* the id of the bioseq so we know how we are viewing the alignments */
    AlignmentFilterStatePtr afsp,   /* What we are initializing. */
    GraphicViewExtrasPtr extras     /* contains the score cuttoff percentage and the kind of scores we use */
)
{  
  SeqAlignPtr   sapIter;
  Int4  alignCnt;
  Uint1 scoreType = NLM_SCORE_COUNT;
  Int2  i = 0;
  
  if (sap == NULL ||  sid == NULL  ||  afsp == NULL)
    return FALSE;
    
  /* what kind of score will we use to weight the alignments? */
  if (extras  &&  extras->alignScoreName) {
    scoreType = StringIndexInStringList (extras->alignScoreName, AlnScoreStrings);
    if (scoreType >= NLM_SCORE_TOOBIG)
        scoreType = NLM_SCORE_COUNT;
  }
  afsp->scoreType = scoreType;
  
  /* what percentile does an alignments score have to be to be displayed? */
  if (extras  && extras->alignScoreCutoff) {
    i = StringIndexInStringList (extras->alignScoreCutoff, AlnScoreCutoffStrings);
    if (i < 0  ||  DIM (AlnScoreCutoffValues) <= i )
        i = 0;
  }
    /*  afsp->cutoffPercent = AlnScoreCutoffValues[i]; */
  if (AlnScoreCutoffValues[i] < 0) {
    afsp->scoreType = NLM_SCORE_COUNT;
  } else {
    afsp->scoreType = NLM_SCORE_BIT;
    afsp->cutoffScore = AlnScoreCutoffValues[i];
  }
  
  /* how many alignments? Count one time now */
  alignCnt = 0;
  for (sapIter = sap; sapIter != NULL; sapIter = sapIter->next) {
    ++alignCnt;
  }

  FindCutoffScore(sap, alignCnt, afsp);
  
  GatherAlignInfo(sap, alignCnt, sid, afsp);
  if (afsp->alignSorted == NULL || afsp->alignSortedLen == 0) {
    /* couldn't allocate memory, or no alignments - nothing to do. */
    return FALSE;
  }
  /* sort the alignments, first by accession, then by location on the bioseq. */
  HeapSort(afsp->alignSorted, afsp->alignSortedLen, sizeof(SeqAlignSortInfo), CompareAlignNameLoc);
  afsp->alignIndex = 0;
  
  return TRUE;
}

static Boolean AlignmentFilterStateDone(AlignmentFilterStatePtr afsp)
{
  if (afsp->alignSorted != NULL &&
      afsp->alignIndex < afsp->alignSortedLen)
    return FALSE;
  return TRUE;
}


static void AlignmentFilterStateFree( AlignmentFilterStatePtr afsp)
{
  if (afsp->alignSorted != NULL)
    MemFree(afsp->alignSorted);
  afsp->alignSorted = NULL;
  afsp->alignSortedLen = 0;
}


  
static Int4 WeightFromAlignScore(SeqAlignPtr sap, Uint1 scoreType)
{
  Int4        weight = 1;
  Int4        score, number;
  Nlm_FloatHi bit_score, evalue;
  
  if (GetScoreAndEvalue(sap, &score, &bit_score, &evalue, &number)) {
    /* evaluate scores and get weight */
    switch (scoreType) {
    case NLM_SCORE_SCORE :
      weight = score;
      break;
    case NLM_SCORE_BIT :
      weight = bit_score;
      break;
    case NLM_SCORE_EVALUE :
      if (evalue > 0) {
        weight = -log(evalue);
      }
      else {  /* 0 is the best value. */
        /*
            We can't use the maxScore here since we do not know it yet
            since this is where we collect the values.
            Our sample data has -log(evalue) ranging from 25 - 429
            so 500 just has to be bigger than anything else we will encounter.
            It will get normalised latter with it as the maxScore.
            
            We could put a special sentinel value (very large or negative) here 
            that gets replaced with the real maxScore later. 
            (But then make sure it gets skipped when calculating the max and min).
        */
        weight = 500;
      }
      break;
    case NLM_SCORE_NUMBER :
      weight = number;
      break;
    case NLM_SCORE_COUNT :
    default:
      weight = 1;
    }
  }
  
  return weight;
}


/*
  Interval Accumulator
  An object which can add or in some way (specified by accumop) intervals on a sequence,
  then from which can be extracted the resulting intervals.
*/

#define ACCUMVALUE_GAP  -1

typedef enum AccumlatorOp {
 NLM_SUM_WEIGHT = 0,
 NLM_MAX_WEIGHT
} AccumulatorOp;


typedef struct AccumNode_s {
  Uint4         coord;
  AccumValue_t  weight;
  struct AccumNode_s * next;  /* makes a single-linked list. */
} AccumNode, *AccumNodePtr;


static AccumNodePtr
InsertNodeAfter(AccumNodePtr node, Uint4 coord, AccumValue_t weight)
{
  AccumNodePtr new_node = (AccumNodePtr) Nlm_MemNew(sizeof(AccumNode));
  if (new_node == NULL) return NULL;
  
  new_node->coord = coord;
  new_node->weight = weight;
  if (node != NULL) {
    new_node->next = node->next;
    node->next = new_node;
  }
  return new_node;
}

static AccumNodePtr
AppendNode(AccumNodePtr head, Uint4 coord, AccumValue_t weight)
{
  while (head->next != NULL)
    head = head->next;
  return InsertNodeAfter(head, coord, weight);
}

static void 
FreeAccumNodes(AccumNodePtr node)
{
  AccumNodePtr del_me;
  while (node) {
    del_me = node;
    node = node->next;
    Nlm_MemFree(del_me);
  }
}

static void
AccumulateWeight(AccumNodePtr node, AccumValue_t w2, AccumulatorOp op)
{
  AccumValue_t w1;
  
  if (w2 == 0)
    return;
  
  w1 = node->weight;
  
  /* gap trumps 0, any weight trumps a gap. */
  if ((w1 == ACCUMVALUE_GAP  &&  w2 <= 0)  ||
      (w2 == ACCUMVALUE_GAP  &&  w1 <= 0)) {
    node->weight = ACCUMVALUE_GAP;
  } else if (w1 == ACCUMVALUE_GAP){
    node->weight = w2; 
  } else if (w2 == ACCUMVALUE_GAP){
    node->weight = w1; 
  } else if (op  == NLM_MAX_WEIGHT) {
    /* max of the weights. */
    if (w2 > w1)
      node->weight = w2;
  } else {
    /* NLM_SUM_WEIGHT */
    if (node->weight < ACCUMVALUE_MAX - w2) /* avoid overflow. */
      node->weight += w2;
    else
      node->weight = ACCUMVALUE_MAX;
  }
}


typedef struct IntervalAccumulator {
  AccumNodePtr        nodes;
  AccumulatorOp       accumOp;
  AccumValue_t        maxWeight;
} IntervalAccumulator, PNTR IntervalAccumulatorPtr;


static IntervalAccumulatorPtr
NewIntervalAccumulator(Uint4 len, AccumulatorOp op)
{
  AccumNodePtr tail_node;
  IntervalAccumulatorPtr IAP;

  IAP = (IntervalAccumulatorPtr) Nlm_MemNew(sizeof(IntervalAccumulator));
  if (IAP == NULL) return NULL;
  IAP->nodes = InsertNodeAfter(NULL, 0, 0); /* header node. coord 0. */
  if (IAP->nodes == NULL) {
    Nlm_MemFree(IAP);
    return NULL;
  }
  tail_node = AppendNode(IAP->nodes, len, 0); /* tail node. max coord. */
  if (tail_node == NULL) {
    FreeAccumNodes(IAP->nodes);
    Nlm_MemFree(IAP);
    return NULL;
  }
  IAP->accumOp = op;
  return IAP;
}

static void
FreeIntervalAccumulator(IntervalAccumulatorPtr *iapp)
{
  IntervalAccumulatorPtr IAP;

  if (iapp == NULL || *iapp == NULL) return;
  IAP = *iapp;
  FreeAccumNodes(IAP->nodes);
  Nlm_MemFree(IAP);
  *iapp = NULL;
}


/* merge in a list of segments. */
static void
AccumSegments(IntervalAccumulatorPtr accum, AccumNodePtr new_nodes)
{

	AccumNodePtr prev_node = accum->nodes; 	/* add our node right before this one. */
  AccumValue_t old_weight = 0;    /* the weight of the last old node passed. */
	AccumValue_t in_weight = 0; 		/* the weight of the last new node entered. */
	
	AccumNodePtr in_node;
	
	in_node = new_nodes;
  /* skip header node in the input, if we have one. */
	if (in_node->weight == 0  &&  in_node->coord == 0)
	  in_node = in_node->next;
		
  for (; in_node != NULL; in_node = in_node->next) {
	
		/* we are adding in_node now. */
		
		/* Find where to insert in_node.  */
		/* while extending the range of the last node added. */
		while (prev_node->next != NULL  && 
			   in_node->coord > prev_node->next->coord) {
			prev_node = prev_node->next;
			old_weight = prev_node->weight;
			AccumulateWeight(prev_node, in_weight, accum->accumOp); 
		}
		in_weight = in_node->weight;

		/* insert at node. */
		if (in_node->coord == prev_node->next->coord) {
		  prev_node = prev_node->next;
			old_weight = prev_node->weight;
			AccumulateWeight(prev_node, in_weight, accum->accumOp); 
		} else {
		  AccumulateWeight(in_node, old_weight, accum->accumOp);
  		InsertNodeAfter(prev_node, in_node->coord, in_node->weight);
  		prev_node = prev_node->next; /* skip the one we just inserted. */
		} 
	}
	
	ASSERT(in_weight > 0);
  /* extend last range to the end of the accumulator. 
	if (in_weight > 0) {
		while (prev_node->next != NULL) {
			prev_node = prev_node->next;
			AccumulateWeight(prev_node, in_weight, accum->accumOp); 
		}
	}
	*/
}




/* Retrieve information. */

static AccumValue_t
GetMaxWeight(const IntervalAccumulatorPtr IAP) 
{
  AccumNodePtr node; 
  AccumValue_t max_weight = 0;

	for (node = IAP->nodes->next; /* skip the header node. */
	     node->next != NULL;  /* skip the last (dummy) node. */
	     node = node->next) { 
    if (max_weight < node->weight)
      max_weight = node->weight;
	}
	return max_weight;
}


static AccumNodePtr
GetIntervalFromAccumulator(
  IntervalAccumulatorPtr iap, 
  AccumNodePtr nextPos, 
  Uint4Ptr startp, 
  Uint4Ptr stopp, 
  AccumValuePtr_t weightp
)
{
  Uint4             start, stop;
  AccumValue_t      weight = 0;
  
  if (NULL == iap  &&  nextPos == NULL) return NULL;

  if (iap  &&  nextPos == NULL)
    nextPos = iap->nodes->next;  /* skip leading 0,0 node. */
  
  /* are we at the end? */
  if (nextPos == NULL  ||  nextPos->next == NULL)  /* nextPos == NULL would be an error. */
    return NULL;
  
  start = nextPos->coord;
  weight = nextPos->weight;
  
  /* go to the next node. */
  nextPos = nextPos->next;
  /* skip redundant nodes. Those with the same weight as the preceding one. */
  while (nextPos->next  &&  nextPos->weight == weight)
    nextPos = nextPos->next;
  stop = nextPos->coord;
  
  if (startp) *startp = start;
  if (stopp)  *stopp  = stop;
  if (weightp) *weightp = weight;
  
  return nextPos;
}



static Uint2 SmearAlignments (
  FilterProcessStatePtr FPSP,
  ViewerContextPtr vContext  
)

{
  SeqAlignPtr              SAP;
  BioseqPtr                BSP;
  SeqIdPtr                 SID;
  AppearanceItemPtr        AIP;
  FilterItemPtr            FIP;
  
  AlignmentFilterStatePtr  alignSP;
  SeqAlignSortInfoPtr      alignSorted;
  Int4                     alignSortedLen;
  Int4                     alignIndex;
  Int4                     start, stop, maxDensity;
  Uint1                    color[3], col;
  Uint1                    minCol = 224; /* density == min -> light gray */
  Uint1                    maxCol = 0;   /* density == max -> black */
  
  IntervalAccumulatorPtr   plusIAP = NULL, minusIAP = NULL;  /* Smear accumulators */
  AccumNodePtr             thisAlignSegs = NULL;  /* input to the accumulators per alignment */
  AccumNodePtr             accumPos;              /* output iterator on the accumulators. */
  Uint4                    begin, end;
  Int4                     space_pixs;
  AccumValue_t             weight;
  AccumValue_t             density;

  Boolean                  smearedAlignsPlus = FALSE, smearedAlignsMinus = FALSE;
  AccumulatorOp            sumOrMax = NLM_SUM_WEIGHT;
  Uint1                    scoreType = NLM_SCORE_COUNT;
  Boolean                  separateStrands;
  const Uint1              minDensity = 1;  /* minimum density we will display. */
  SegmenT                  seg;
  CharPtr                  annotName;
  Int4                     height = 0;
  Uint2                    space_line_height = 2;
  
  if (FPSP == NULL || vContext == NULL) return 0;
  alignSP = &FPSP->state.align;
  alignSorted  = alignSP->alignSorted;
  alignSortedLen = alignSP->alignSortedLen;
 
   /* get the Appearance item for alignments */
  AIP = vContext->AppPtr->FeaturesAppearanceItem[APPEARANCEITEM_Alignment];

  /* get the bioseq's id for picking out the right part of the alignments */
  BSP = vContext->BSP;
  SID = BSP->id;
   
  /* Get this annotation's name to decide how to display this. */ 
  annotName = GetSeqAnnotName(vContext->currentSAP);
  separateStrands = FALSE;
  if (StringStr(annotName, "BLASTX - swissprot"))
    separateStrands = TRUE;
  else if (StringStr(annotName, "BLASTN - mrna"))
    separateStrands = TRUE;
  else if (StringStr(annotName, "BLASTN - est"))
    separateStrands = FALSE;
  else if (StringStr(annotName, "BLASTN - nr"))
    separateStrands = FALSE;

  plusIAP =  NewIntervalAccumulator(vContext->seqLength, sumOrMax);
  if (plusIAP == NULL)  goto smearAlignmentsDone;
  minusIAP = NewIntervalAccumulator(vContext->seqLength, sumOrMax);
  if (minusIAP == NULL)  goto smearAlignmentsDone;
    
  /* 
    for all the sorted alignments (from a single annotation in this BioSeq)
    treat all alignments with the same accession as one alignment
  */   
  for (alignIndex = 0; 
       alignIndex < alignSortedLen; 
       ++alignIndex) 
  {
    AlignSegIterator asi;
    Int4  nsegs;
    Uint1 segType;
    Uint1 strand;
    
    SAP = alignSorted[alignIndex].sap;
    
    /* sanity checks */
    if (alignSorted[alignIndex].start < 0 || alignSorted[alignIndex].stop < 0)
      continue;
      
    weight = alignSorted[alignIndex].normScore;
    /* if (weight == 0)
      continue;
    */  
    /*
      at the begining of an alignment
      if there was another alignment before this one, 
      with the same accession, strand and that alignment ended before this one begins, 
      treat that space as a gap.
    */
    if (alignIndex > 0 && 
        StrNCmp(alignSorted[alignIndex - 1].label, alignSorted[alignIndex].label, MAX_ALIGN_SORT_LABEL) == 0 &&
        alignSorted[alignIndex - 1].strand == alignSorted[alignIndex].strand &&
        alignSorted[alignIndex - 1].stop < alignSorted[alignIndex].start)
    {
      start = alignSorted[alignIndex - 1].stop;
      AppendNode(thisAlignSegs, start, ACCUMVALUE_GAP );
    } else {
      /* we are starting a new alignment.  */
      /* Smear the last alignment's segments. */
      if (alignIndex > 0) {
        AppendNode(thisAlignSegs, alignSorted[alignIndex - 1].stop, 0);
        if (alignSorted[alignIndex - 1].strand != Seq_strand_minus || !separateStrands )
          AccumSegments(plusIAP, thisAlignSegs);
        else
          AccumSegments(minusIAP, thisAlignSegs);
      }
      
      /* Prepare space for the new alignment's segment info.  */
      FreeAccumNodes(thisAlignSegs);
      thisAlignSegs = InsertNodeAfter(NULL, 0, 0);
    }
        
    /*
      for each segment in an alignment
      if the segment is block or a gap map it appropriately in the density arrays.
    */ 
    nsegs = AlignSegIteratorCreate(SAP, SID, &asi);
    if (nsegs == 0)
      continue;
    while (AlignSegIteratorNext(&asi, &start, &stop, &strand, &segType))
    {
      if (segType == AM_INSERT || start < 0 || stop < 0)
        continue;
        
      if (segType == AM_GAP ) {  /* a gap */
        AppendNode(thisAlignSegs, start, ACCUMVALUE_GAP );
      }
      else if (segType == AM_SEQ) { /* a real alignment. */
        AppendNode(thisAlignSegs, start, weight );
        if (strand != Seq_strand_minus || !separateStrands) {
          smearedAlignsPlus = TRUE;
        } else {
          smearedAlignsMinus = TRUE;
        }
      }
  
    } /* for all segments in an alignment */
  } /* for all alignments in an annotation */
  
    /* Smear the last alignment's segments. */
   if (alignIndex > 0) {
    AppendNode(thisAlignSegs, alignSorted[alignIndex - 1].stop, 0);
    if (alignSorted[alignIndex - 1].strand != Seq_strand_minus || !separateStrands )
      AccumSegments(plusIAP, thisAlignSegs);
    else
      AccumSegments(minusIAP, thisAlignSegs);
  }

  if (!smearedAlignsPlus && !smearedAlignsMinus) 
    goto smearAlignmentsDone; /* there was nothing to show. */
  
  DrawNamedAnnotBox(FPSP);
  FIP = (FilterItemPtr) FPSP->currentFilterVNP->data.ptrvalue;
  DrawFilterItemBoxLabel(FPSP, FIP);
  
  seg = CreateSegment ( vContext->drawOnMe, 0, 0);

  if (smearedAlignsPlus || !separateStrands) {
    height += space_line_height;
    maxDensity = GetMaxWeight(plusIAP);

    accumPos = GetIntervalFromAccumulator(plusIAP, NULL, &begin, &end, &density);
    while (accumPos != NULL) {
    
      if (density > 0  && density != ACCUMVALUE_GAP) {
          /* convert  (1 - maxDensity) to "color" number (224 - 0) */
        if (maxDensity == minDensity) {
          col = minCol;
        } else {
          col = (Uint1) (minCol + (density - minDensity)*(maxCol - minCol)/(maxDensity - minDensity));
        }
        color [2] = color [1] = color [0] = col;  /* set to shade of grey. (minCol = 224) */
        AddAttribute (seg, COLOR_ATT, color, 0, 0, 1, 0);
        AddRectangle (seg, begin, FPSP->ceiling - height, 
                           end,   FPSP->ceiling - height - (AIP->Height), NO_ARROW, TRUE, 0);
      }
      else if (density == ACCUMVALUE_GAP) {
        AddAttribute (seg, COLOR_ATT, NULL, 0, 0, 1, 0);
        AddLine (seg, begin, FPSP->ceiling - height - (AIP->Height)/2, 
                      end,   FPSP->ceiling - height - (AIP->Height)/2, NO_ARROW, 0);
      } else { /* density == 0 */
        /* put a small line above spaces between blocks we draw */
        if (0 < begin  &&  end < vContext->seqLength) { /* ignore gaps at beginning and end of bioseq */
          ASSERT(density == 0);
          space_pixs = (end - begin)/vContext->viewScale;
          /* if (3 <= space_pixs  &&  space_pixs <= 10  &&  end < vContext->seqLength) { */
          if (space_pixs <= 20) { /* don't draw this line if it is more than 20 pixels long. */
            if (space_pixs < 3) { /* make sure bar is always at least 3 pixels long */
              int mid_point = end + begin;
              end = (mid_point + 3 * vContext->viewScale) /2;
              begin = (mid_point - 3 * vContext->viewScale) /2;
            }
            AddAttribute (seg, COLOR_ATT, NULL, 0, 0, 1, 0);
            AddLine (seg, begin,                     FPSP->ceiling + space_line_height, 
                          end - vContext->viewScale, FPSP->ceiling + space_line_height, NO_ARROW, 0);
          }
        }
      }
      accumPos = GetIntervalFromAccumulator(NULL, accumPos, &begin, &end, &density);
    }
    /* put a little arrow to show this is the plus strand */
    if (separateStrands) {
      AddAttribute (seg, COLOR_ATT, NULL, 0, 0, 1, 0);
      AddTextLabel (seg, 0 * vContext->viewScale, FPSP->ceiling - height - (AIP->Height)/2, 
                   ">", FIP->GroupLabelFont, 1, MIDDLE_LEFT, 0);
    }
    height += AIP->Height + FIP->IntraRowPaddingPixels;
  }
  
  if (smearedAlignsMinus) {
    maxDensity = GetMaxWeight(minusIAP);

    accumPos = GetIntervalFromAccumulator(minusIAP, NULL, &begin, &end, &density);
    while (accumPos != NULL) {
    
      if (density > 0  && density != ACCUMVALUE_GAP) {
          /* convert  (1 - maxDensity) to "color" number (224 - 0) */
        if (maxDensity == minDensity) {
          col = minCol;
        } else {
          col = (Uint1) (minCol + (density - minDensity)*(maxCol - minCol)/(maxDensity - minDensity));
        }
        color [2] = color [1] = color [0] = col;  /* set to shade of grey. (minCol = 224) */
        AddAttribute (seg, COLOR_ATT, color, 0, 0, 1, 0);
        AddRectangle (seg, begin, FPSP->ceiling - height, 
                           end,   FPSP->ceiling - height - (AIP->Height), NO_ARROW, TRUE, 0);
      }
      else if (density == ACCUMVALUE_GAP) {
        AddAttribute (seg, COLOR_ATT, NULL, 0, 0, 1, 0);
        AddLine (seg, begin, FPSP->ceiling - height - (AIP->Height)/2, 
                      end,   FPSP->ceiling - height - (AIP->Height)/2, NO_ARROW, 0);
      } else { 
        /* put a small line above spaces between blocks we draw */
        if (0 < begin  &&  end < vContext->seqLength) { /* ignore gaps at beginning and end of bioseq */
          ASSERT(density == 0);
          space_pixs = (end - begin)/vContext->viewScale;
          /* if (3 <= space_pixs  &&  space_pixs <= 10  &&  end < vContext->seqLength) { */
          if (space_pixs <= 20) { /* don't draw this line if it is more than 20 pixels long. */
            if (space_pixs < 3) { /* make sure bar is always at least 3 pixels long */
              int mid_point = end + begin;
              end = (mid_point + 3 * vContext->viewScale) /2;
              begin = (mid_point - 3 * vContext->viewScale) /2;
            }
            AddAttribute (seg, COLOR_ATT, NULL, 0, 0, 1, 0);
            AddLine (seg, begin,                     FPSP->ceiling - height - (AIP->Height) - space_line_height - 1, 
                          end - vContext->viewScale, FPSP->ceiling - height - (AIP->Height) - space_line_height - 1, NO_ARROW, 0);
          }
        }
      }
      accumPos = GetIntervalFromAccumulator(NULL, accumPos, &begin, &end, &density);
    }
    AddAttribute (seg, COLOR_ATT, NULL, 0, 0, 1, 0);
    AddTextLabel (seg, 0, FPSP->ceiling - height - (AIP->Height)/2, 
                 "<", FIP->GroupLabelFont, 1, MIDDLE_LEFT, 0);

    height += space_line_height;
    height += AIP->Height + FIP->IntraRowPaddingPixels;
  }
    
smearAlignmentsDone:

  FreeIntervalAccumulator(&plusIAP);
  FreeIntervalAccumulator(&minusIAP);

  return height;
}


static Boolean FilterAndLayout (
  ViewerContextPtr vContext
)

{
  FilterProcessState  FPS;
  FilterItemPtr       FIP;
  FilterPtr           FP;
  AppearancePtr       AP;
  FilterItem          tempFI;
  Int1                featdeftype;
  LayoutAlgorithm     layoutC;
  Int4                height, totalheight;
  SegmenT             filterSeg, invisibleSeg;
  Uint1               featdefOrder;
  Boolean             emptyFilterGroup;
  Int4                undoCeiling;
  BioseqPtr           BSP;
  SeqAnnotPtr         SAnnP;
  SeqAlignPtr         SAlnP;


  if (vContext == NULL) return FALSE;
  MemSet (&FPS, 0, sizeof (FilterProcessState));
  FPS.vContext = vContext;
  if (vContext->ceiling != NULL) {
    FPS.ceiling = *vContext->ceiling;
  } else {
    FPS.ceiling = 0;
  }
  FPS.featuresProcessed = MemNew (vContext->featureCount * sizeof (Boolean));
  if (FPS.featuresProcessed == NULL && vContext->featureCount != 0) return FALSE;
  AP = vContext->AppPtr;
  vContext->sanLevelSeg = CreateSegment (vContext->topLevelSeg, 0, 0);
  FPS.annotBoxDrawn = FALSE;
  FPS.annotLabelDrawn = FALSE;

  FP = vContext->FltPtr;
  if (! FP->AnnotBoxColorSet) {
    MemCopy (FP->AnnotBoxColor, AP->AnnotBoxColor, sizeof (Uint1 [3]));
  }
  if (! FP->AnnotLabelColorSet) {
    MemCopy (FP->AnnotLabelColor, AP->AnnotLabelColor, sizeof (Uint1 [3]));
  }
  if (! FP->AnnotLabelFontSet) {
    FP->AnnotLabelFont = AP->AnnotLabelFont;
  }

  for (FPS.currentFilterVNP = FP->FilterItemList;
       FPS.currentFilterVNP != NULL;
       FPS.currentFilterVNP = FPS.currentFilterVNP->next) {

    if (FPS.currentFilterVNP->data.ptrvalue == NULL) continue; /* this should not happen if config file parsing worked */

    filterSeg = CreateSegment (vContext->sanLevelSeg, 0, 0);
    MemSet (FPS.labelSegs, 0, sizeof (FPS.labelSegs));
    MemSet (FPS.drawSegs, 0, sizeof (FPS.drawSegs));
    vContext->drawOnMe = filterSeg;
    emptyFilterGroup = TRUE;
    undoCeiling = FPS.ceiling;
    FPS.featuresProcessedCount = 0;
    FPS.groupBoxDrawn = FALSE;
    FPS.groupLabelDrawn = FALSE;

    FIP = (FilterItemPtr) FPS.currentFilterVNP->data.ptrvalue;

    if (! FIP->GroupBoxColorSet) {
      MemCopy (FIP->GroupBoxColor, AP->GroupBoxColor, sizeof (Uint1 [3]));
    }
    if (! FIP->GroupLabelColorSet) {
      MemCopy (FIP->GroupLabelColor, AP->GroupLabelColor, sizeof (Uint1 [3]));
    }
    if (! FIP->GroupLabelFontSet) {
      FIP->GroupLabelFont = AP->GroupLabelFont;
    }

    switch (FIP->Type) {
      case InvalidFilter:
        break;
      case BioseqFilter:
        if (vContext->currentSAP) 
          break;
        height = DrawBioseq (FPS.ceiling, FIP, vContext);
        emptyFilterGroup = FALSE;
        break;
      case GraphFilter:
        if (vContext->currentSAP) 
          break;
        height = DrawGraphs (FPS.ceiling, vContext);
        if (height != 0) {
          emptyFilterGroup = FALSE;
        }
        break;
      case AlignmentFilter:
        BSP = vContext->BSP;
        height = 0;
        totalheight = 0;
        layoutC = (vContext->overrideLayout != Layout_Inherit) ? vContext->overrideLayout : FIP->LayoutChoice;
        FPS.currentFIP = FIP;
        
        /*
         *  Or we could move the current SAnnP into FPS.state.align and put all of this logic
         *  into GetNextRFIPinAlignmentFilter() which SmearAlignments would call, making Alignment
         *  processing more like Feature processing.
         */
        for (SAnnP = BSP->annot; SAnnP != NULL; SAnnP = SAnnP->next) 
        {
          if (SAnnP->type != 2) 
            continue;  /* type 2 annotation is an alignment */
          
          if (FP->GroupByAnnot) /* are we grouping by named annotations? */
          {
            if (GetSeqAnnotName(SAnnP)) /* this is a named annotation */
            {
              if (SAnnP != vContext->currentSAP) /* only do the named annotation we are currently doing. */
                continue;
            }
            else if (vContext->currentSAP) /* don't do any unnamed annotation if we are doing a particular named one. */
              continue;
          }
                   
          emptyFilterGroup = FALSE;
          SAlnP = (SeqAlignPtr) SAnnP->data;
                    
          ResetFilterState (&FPS);
          if ( ! AlignmentFilterStateInit(SAlnP, BSP->id ,&FPS.state.align, vContext->extras))
            continue;

          switch (layoutC) {
            case Layout_FeatTypePerLine:
              height = SmearAlignments (&FPS, vContext);
              break;
            default:
              height = ProcessRows (layoutC, &FPS, vContext);
              break;
          }
          AlignmentFilterStateFree(&FPS.state.align);
          
          totalheight += height;
          if (height > 0)
            FPS.ceiling -= height;
        } /* SAnnp, cycle through all SeqAnnots on this Bioseq */
        height = 0;
        break;
      case FeatureFilter:
        layoutC = (vContext->overrideLayout != Layout_Inherit) ? vContext->overrideLayout : FIP->LayoutChoice;
        /*
          Some layouts act to the user as if they are single FilterGroups, but internally use multiple
          consecutive FilterGroups
          (currently, Layout_FeatTypePerLine, Layout_FeatTypePerLineGroup, Layout_GroupCorrespondingFeats, and Layout_GroupCorrespondingFeatsRepeat).
        */
        /*
          FeatTypePerLineGroup is equiv. to using PackUpwards several times, with single-feature filteritems
          FeatTypePerLine is similar (but using AllInOneLine)
        */
        switch (layoutC) {
          case Layout_FeatTypePerLine:
          case Layout_FeatTypePerLineGroup:
            FPS.currentFIP = &tempFI;
            MemCopy (&tempFI, FIP, sizeof (FilterItem)); /* copy the filter . . . */
            MemSet (&tempFI.IncludeFeature, 0, sizeof (tempFI.IncludeFeature));  /* but don't include any features */
            tempFI.AddTypeToLabel = TristateFalse;
            for (featdefOrder = 1; featdefOrder < APPEARANCEITEM_MAX; featdefOrder++) {
              for (featdeftype = 1; featdeftype < APPEARANCEITEM_MAX; featdeftype++) {
                if (FIP->IncludeFeature [featdeftype] == featdefOrder) {
                  ResetFilterState (&FPS);
                  tempFI.IncludeFeature[featdeftype] = TRUE;
                  height = ProcessRows (layoutC, &FPS, vContext);
                  if (FPS.featuresProcessedCount != 0) {
                    emptyFilterGroup = FALSE;
                  }
                  FPS.ceiling -= height;
                  tempFI.IncludeFeature[featdeftype] = FALSE;
                }
              }
            }
            height = 0; /* prevent FPS.ceiling from being bumped again */
            break;
          case Layout_GroupCorrespondingFeats:
          case Layout_GroupCorrespondingFeatsRepeat:
            /*
              This uses 3 FilterItems:
                - All gene features (compact)
                - CDS & mRNA (grouped by products)
                - anything else included in the filter as specified by the user (compact)
            */
            FPS.currentFIP = &tempFI;
            MemCopy (&tempFI, FIP, sizeof (FilterItem)); /* copy the filter . . .*/
            MemSet (&tempFI.IncludeFeature, 0, sizeof (tempFI.IncludeFeature));  /* but don't include any features*/
            tempFI.GroupPadding = 0;
            tempFI.IncludeFeature [FEATDEF_GENE] = FIP->IncludeFeature [FEATDEF_GENE];
            ResetFilterState (&FPS);
            height = ProcessRows (Layout_PackUpward, &FPS, vContext);
            FPS.ceiling -= height;

            ResetFilterState (&FPS);
            tempFI.IncludeFeature [FEATDEF_CDS] = FIP->IncludeFeature [FEATDEF_CDS];
            tempFI.IncludeFeature [FEATDEF_mRNA] = FIP->IncludeFeature [FEATDEF_mRNA];
            height = ProcessRows (layoutC, &FPS, vContext);
            FPS.ceiling -= height;

            ResetFilterState (&FPS);
            MemCopy (&tempFI.IncludeFeature, &FIP->IncludeFeature, sizeof (tempFI.IncludeFeature));
            tempFI.GroupPadding = FIP->GroupPadding;
            height = ProcessRows (Layout_PackUpward, &FPS, vContext);
            FPS.ceiling -= height;
            height = 0;
            if (FPS.featuresProcessedCount != 0) {
              emptyFilterGroup = FALSE;
            }

            break;
          default:
            FPS.currentFIP = FIP;
            ResetFilterState (&FPS);
            height = ProcessRows (layoutC, &FPS, vContext);
            if (FPS.featuresProcessedCount != 0) {
              emptyFilterGroup = FALSE;
            }
            break;
        } /* switch (layoutC) */
        break;
    } /* switch (FIP->type) */
    if (emptyFilterGroup) {
      FPS.ceiling = undoCeiling;
      continue;
    } else {
      switch (FIP->GroupLabelLoc) {
      default: break;
      case LabelOnTop:
        /* already done in DrawFilterItemBoxLabel() */
        break;
      case LabelOnSide:
        AddAttribute (vContext->drawOnMe, COLOR_ATT, FIP->GroupLabelColor, 0, 0, 0, 0);
        AddTextLabel (filterSeg, 0, FPS.ceiling - height / 2, FIP->GroupLabel,
                      FIP->GroupLabelFont, 1, MIDDLE_RIGHT, 0);
        SelectFont (FIP->GroupLabelFont);
        height = MAX (height, LineHeight () + 3);
        FPS.groupLabelDrawn = TRUE;
        break;
      case LabelOnBottom:
        AddAttribute (vContext->drawOnMe, COLOR_ATT, FIP->GroupLabelColor, 0, 0, 0, 0);
        AddTextLabel (filterSeg, (vContext->from + vContext->to) / 2 , FPS.ceiling - height, FIP->GroupLabel,
                      FIP->GroupLabelFont, 1, LOWER_CENTER, 0);
        SelectFont (FIP->GroupLabelFont);
        height += LineHeight () + 3;
        FPS.groupLabelDrawn = TRUE;
        break;
      }
    }
    if (FPS.groupBoxDrawn) {
      FPS.ceiling -= 10;
    }
    FPS.ceiling -= height + FIP->GroupPadding;
    if (FPS.needFreeList != NULL) {
      FPS.needFreeList = ValNodeFreeData (FPS.needFreeList);
      FPS.lastInFreeList = NULL;
    }
  } /* for ( FP->FilterItemList->next ) */

  if (FP->GroupByAnnot && FP->AnnotLabelLoc == LabelOnBottom) {
    CharPtr             annotName;

    annotName = GetSeqAnnotName(vContext->currentSAP);
    if (annotName != NULL && !StringHasNoText(annotName))
    {
      AddAttribute (vContext->sanLevelSeg, COLOR_ATT, FP->AnnotLabelColor, 0, 0, 0, 0);
      AddTextLabel (vContext->sanLevelSeg, (vContext->from + vContext->to) / 2 , FPS.ceiling, annotName,
                    FP->AnnotLabelFont, 1, LOWER_CENTER, 0);
      SelectFont (FP->AnnotLabelFont);
      FPS.ceiling -= LineHeight () + 3;
      FPS.annotLabelDrawn = TRUE;
    }
  }
  if (FPS.annotBoxDrawn) {
    /* add invisible line to force width of SegRect and space under group boxes. */
    invisibleSeg = CreateSegment (vContext->sanLevelSeg, 0, 0);
    SetSegmentVisibleFlag (invisibleSeg, FALSE);
    AddLine (invisibleSeg, vContext->from - 3 * vContext->viewScale ,  FPS.ceiling + 10, 
                           vContext->to + 3 * vContext->viewScale ,  FPS.ceiling + 10, FALSE, 0);
    FPS.ceiling -= 20;
  }
  if (FPS.needFreeList != NULL) {
    FPS.needFreeList = ValNodeFreeData (FPS.needFreeList);
    FPS.lastInFreeList = NULL;
  }

  if (vContext->ceiling != NULL) {
    *vContext->ceiling = FPS.ceiling;
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
)

{
  ViewerContext  VC;
  Int2           sapIndex;
  Int4           theCeiling = 0;

  /*
    Removed checks feats->featureCount == 0 || feats->featVNP == NULL
    to allow display of BSP w/0 features on it.
   */

  if (AP == NULL || FP == NULL) return NULL;
  MemSet ((Pointer) &VC, 0, sizeof (ViewerContext));
  VC.from = MIN (from, to);
  VC.to =   MAX (from, to);
  VC.allFeatures = allFeatures;
  VC.BSP = bsp;
  VC.viewScale = scale;
  VC.sapList = feats->sapList;
  VC.sapCount = feats->sapCount;
  if (topLevel == NULL) {
    VC.drawOnMe = VC.sanLevelSeg = VC.topLevelSeg = CreatePicture ();
  } else {
    VC.drawOnMe = VC.sanLevelSeg = VC.topLevelSeg = topLevel;
  }
  VC.featureCount = feats->featureCount;
  VC.featVNP = feats->featVNP;
  VC.AppPtr = AP;
  VC.FltPtr = FP;
  VC.overrideLayout = overrideLayout;
  VC.seqLength = bsp->length;
  VC.extras = extras;
  
  if (NULL == ceiling)
    VC.ceiling = &theCeiling;
  else
    VC.ceiling = ceiling;
  
  VC.currentSAP = NULL;
  FilterAndLayout (&VC);
  /* do again for named Seq Annot's */
  if (FP->GroupByAnnot)
  {
    for (sapIndex = 0; sapIndex < VC.sapCount; ++sapIndex) {
      VC.currentSAP = VC.sapList[sapIndex];
      FilterAndLayout (&VC);    
    }
  }
  return VC.topLevelSeg;
}

/* returns the 1st segment in a linked list. caller must deallocate it */
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
)

{
  RelevantFeaturesPtr   RFP;
  SegmenT               seg;
  SeqIntPtr             sintp;
  BioseqPtr             parent;
  SeqMgrSegmentContext  context;
  Int4                  from = 0;
  Int4                  to = 0;
  Boolean               allFeatures = TRUE;

  if (location != NULL) {
    bsp = BioseqFindFromSeqLoc (location);
    if (bsp == NULL) return NULL;
    to = bsp->length - 1;

    if (location->choice == SEQLOC_WHOLE) {
      location = NULL; /* no special behavior needed if it's whole */
    } else if (location->choice == SEQLOC_INT) {
      sintp = (SeqIntPtr) location->data.ptrvalue;
      if (sintp != NULL && sintp->from == 0 && sintp->to == bsp->length - 1) {
        location = NULL;
      } else {
        from = sintp->from;
        to = sintp->to;
        allFeatures = FALSE;
      }
    }
  } else if (bsp != NULL) {
    to = bsp->length - 1;
  }
  if (bsp == NULL) return NULL;
  parent = SeqMgrGetParentOfPart (bsp, &context);
  if (parent != NULL) {
    from = context.cumOffset;
    to = from + context.to - context.from;
    allFeatures = FALSE;
    bsp = parent;
  }
  RFP = CollectFeatures (bsp);
  if (RFP == NULL) return NULL;
  seg = CreateGraphicViewInternal (bsp, from, to, allFeatures, RFP, scale, ceiling, topLevel, AP, FP, overrideLayout, extras);
  FreeCollectedFeatures (RFP);
  return seg;
}

NLM_EXTERN SegmenT CreateGraphicView (
  BioseqPtr bsp,
  SeqLocPtr location,
  Int4 scale,
  CharPtr styleName,
  CharPtr filterName,
  CharPtr overrideLayoutName,
  GraphicViewExtrasPtr extras
)

{
  ViewerConfigsPtr  myVCP;
  FilterPtr         FP;
  AppearancePtr     AP;
  LayoutAlgorithm   overrideLayout;
  Int1              i;

  if (bsp == NULL && location == NULL) return NULL;
  myVCP = GetGraphicConfigParseResults ();

  AP = FindAppearanceByName (styleName, myVCP);
  FP = FindFilterByName (filterName, myVCP);
  i = StringIndexInStringList (overrideLayoutName, LayoutStrings);
  if (i >= 0 && i < DIM (LayoutValues)) {
    overrideLayout = LayoutValues[i];
  } else {
    overrideLayout = Layout_Inherit;
  }
      
  return CreateGraphicViewFromBsp (bsp, location, scale, NULL, NULL, AP, FP, overrideLayout, extras);
}

NLM_EXTERN Uint2 GetAppearanceCount (void)

{
  ViewerConfigsPtr VCP;

  VCP = GetGraphicConfigParseResults ();
  if (VCP == NULL) return 0;
  return VCP->AppearanceCount;
}

NLM_EXTERN Uint2 GetFilterCount (void)

{
  ViewerConfigsPtr VCP;

  VCP = GetGraphicConfigParseResults ();
  if (VCP == NULL) return 0;
  return VCP->FilterCount;
}

NLM_EXTERN Uint2 GetLayoutCount (void)

{
  return DIM (LayoutStrings);
}

NLM_EXTERN Uint2 GetAlnScoreCount (void)
{
  return DIM (AlnScoreStrings);
}

NLM_EXTERN Uint2 GetAlnScoreCutoffCount (void)
{
  return DIM (AlnScoreCutoffStrings);
}

NLM_EXTERN CharPtr PNTR GetStyleNameList (void)

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

NLM_EXTERN CharPtr PNTR GetAlnScoreNameList(void)
{
    return AlnScoreStrings;
}

NLM_EXTERN CharPtr PNTR GetAlnScoreCutoffList(void)
{
    return AlnScoreCutoffStrings;
}

NLM_EXTERN CharPtr PNTR GetLayoutNameList (void)

{
  return LayoutStrings;
}


/* -=-=-=-=-=-=-=- append default-style.c contents after this point -=-=-=-=-=-=-=-=- */

/* default-style.c : creates a default style for the new graphic viewer
  This is an automatically generated file, which came from an asn2gph configuration file (asn2gph.default)
  It might be better to edit the input file and then re-run the script create-default-style.tcl than to
  edit this file directly.
*/


typedef struct configFileLine {
  CharPtr key, value;
} ConfigFileLine, PNTR ConfigFileLinePtr;

typedef struct staticConfigFile {
  ConfigFileLinePtr lines;
  CharPtr sectionName;
} StaticConfigFile, PNTR StaticConfigFilePtr;


/* [Styles] */
static ConfigFileLine defaultStyleLines1 [] = {
  {"style00", "defaultStyle"},
  {"style100", "summary"},
  {NULL, NULL}
};

/* [defaultStyle.master] */
static ConfigFileLine defaultStyleLines2 [] = {
  {"name", "Default"},
  {"maxarrowscale", "200"},
  {"minarrowpixels", "5"},
  {"shadesmears", "false"},
  {"color", "black"},
  {"labelfont", "program"},
  {"labelcolor", "black"},
  {"label", "above"},
  {"grouplabelfont", "program"},
  {"grouplabelcolor", "dark gray"},
  {"groupboxcolor", "gray"},
  {"displaywith", "box"},
  {"height", "8"},
  {"gap", "line"},
  {"showarrow", "no"},
  {"showtype", "yes"},
  {"showcontent", "yes"},
  {"shadesmears", "false"},
  {"annotboxcolor", "100, 100, 100"},
  {"annotlabelcolor", "black"},
  {"annotlabelfont", "program"},
  {NULL, NULL}
};

/* [defaultStyle.bioseq] */
static ConfigFileLine defaultStyleLines3 [] = {
  {"label", "left"},
  {"format", "accn"},
  {"scale", "true"},
  {"labelfont", "program"},
  {"scalefont", "small"},
  {"height", "10"},
  {"scaleheight", "10"},
  {"color", "0, 0, 0"},
  {"labelcolor", "64, 64, 255"},
  {"scalecolor", "32, 32, 32"},
  {NULL, NULL}
};

/* [defaultStyle.gene] */
static ConfigFileLine defaultStyleLines4 [] = {
  {"label", "above"},
  {"color", "blue"},
  {"labelcolor", "blue"},
  {"showarrow", "true"},
  {NULL, NULL}
};

/* [defaultStyle.mRNA] */
static ConfigFileLine defaultStyleLines5 [] = {
  {"label", "above"},
  {"color", "cyan"},
  {"labelcolor", "cyan"},
  {"showarrow", "true"},
  {"gap", "line"},
  {NULL, NULL}
};

/* [defaultStyle.cds] */
static ConfigFileLine defaultStyleLines6 [] = {
  {"label", "above"},
  {"color", "magenta"},
  {"labelcolor", "magenta"},
  {"showarrow", "true"},
  {"gap", "angle"},
  {NULL, NULL}
};

/* [defaultStyle.tRNA] */
static ConfigFileLine defaultStyleLines7 [] = {
  {"label", "above"},
  {"color", "green"},
  {"labelcolor", "green"},
  {"showarrow", "true"},
  {"gap", "line"},
  {NULL, NULL}
};

/* [defaultStyle.imp] */
static ConfigFileLine defaultStyleLines8 [] = {
  {"showcontent", "no"},
  {"color", "gray"},
  {"labelcolor", "gray"},
  {NULL, NULL}
};

/* [defaultStyle.exon] */
static ConfigFileLine defaultStyleLines9 [] = {
  {"showcontent", "no"},
  {"color", "dark cyan"},
  {"labelcolor", "dark cyan"},
  {NULL, NULL}
};

/* [defaultStyle.intron] */
static ConfigFileLine defaultStyleLines10 [] = {
  {"showcontent", "no"},
  {"color", "light gray"},
  {"labelcolor", "light gray"},
  {NULL, NULL}
};

/* [defaultStyle.bond] */
static ConfigFileLine defaultStyleLines11 [] = {
  {"displaywith", "cappedline"},
  {NULL, NULL}
};

/* [defaultStyle.align] */
static ConfigFileLine defaultStyleLines12 [] = {
  {"label", "above"},
  {"color", "blue"},
  {"labelcolor", "blue"},
  {"showarrow", "true"},
  {"showtype", "no"},
  {"showcontent", "yes"},
  {"format", "accession"},
  {NULL, NULL}
};

/* [summary.master] */
static ConfigFileLine defaultStyleLines13 [] = {
  {"name", "Summary"},
  {"label", "none"},
  {"height", "3"},
  {"labelfont", "small"},
  {"grouplabelfont", "small"},
  {"annotlabelfont", "small"},
  {NULL, NULL}
};

/* [summary.bioseq] */
static ConfigFileLine defaultStyleLines14 [] = {
  {"label", "above"},
  {"format", "accn"},
  {"scale", "true"},
  {"labelfont", "program"},
  {"scalefont", "small"},
  {"height", "5"},
  {"scaleheight", "5"},
  {"color", "0, 0, 0"},
  {"labelcolor", "64, 64, 255"},
  {"scalecolor", "32, 32, 32"},
  {NULL, NULL}
};

/* [summary.gene] */
static ConfigFileLine defaultStyleLines15 [] = {
  {"label", "above"},
  {"color", "blue"},
  {"labelcolor", "blue"},
  {"showarrow", "true"},
  {NULL, NULL}
};

/* [summary.mRNA] */
static ConfigFileLine defaultStyleLines16 [] = {
  {"label", "above"},
  {"color", "cyan"},
  {"labelcolor", "cyan"},
  {"showarrow", "true"},
  {"gap", "line"},
  {NULL, NULL}
};

/* [summary.cds] */
static ConfigFileLine defaultStyleLines17 [] = {
  {"label", "above"},
  {"color", "magenta"},
  {"labelcolor", "magenta"},
  {"showarrow", "true"},
  {"gap", "angle"},
  {NULL, NULL}
};

/* [summary.tRNA] */
static ConfigFileLine defaultStyleLines18 [] = {
  {"label", "above"},
  {"color", "green"},
  {"labelcolor", "green"},
  {"showarrow", "true"},
  {"gap", "line"},
  {NULL, NULL}
};

/* [summary.align] */
static ConfigFileLine defaultStyleLines19 [] = {
  {"label", "above"},
  {"color", "blue"},
  {"labelcolor", "blue"},
  {"showarrow", "true"},
  {"showtype", "no"},
  {"showcontent", "yes"},
  {"format", "accession"},
  {NULL, NULL}
};

/* [summary.imp] */
static ConfigFileLine defaultStyleLines20 [] = {
  {"showcontent", "no"},
  {"color", "gray"},
  {"labelcolor", "gray"},
  {NULL, NULL}
};

/* [summary.exon] */
static ConfigFileLine defaultStyleLines21 [] = {
  {"showcontent", "no"},
  {"color", "dark cyan"},
  {"labelcolor", "dark cyan"},
  {NULL, NULL}
};

/* [summary.intron] */
static ConfigFileLine defaultStyleLines22 [] = {
  {"showcontent", "no"},
  {"color", "light gray"},
  {"labelcolor", "light gray"},
  {NULL, NULL}
};

/* [summary.bond] */
static ConfigFileLine defaultStyleLines23 [] = {
  {"displaywith", "cappedline"},
  {NULL, NULL}
};

/* [Filters] */
static ConfigFileLine defaultStyleLines24 [] = {
  {"filter00", "defaultFilt"},
  {"filter100", "summary"},
  {"maxlabelscale", "200"},
  {"grouppadding", "2"},
  {"rowpadding", "2"},
  {"annotgroup", "yes"},
  {"annotbox", "yes"},
  {"annotlabel", "above"},
  {NULL, NULL}
};

/* [defaultFilt] */
static ConfigFileLine defaultStyleLines25 [] = {
  {"name", "Default"},
  {"layout", "compact"},
  {"group1", "defaultFilt-operons"},
  {"group2", "defaultFilt-gene-cds-prot-mrna"},
  {"group3", "defaultFilt-other-rnas"},
  {"group4", "defaultFilt-exon-intron-label"},
  {"group5", "defaultFilt-variations"},
  {"group6", "defaultFilt-conflicts"},
  {"group7", "defaultFilt-STS"},
  {"group8", "defaultFilt-impfeats"},
  {"group9", "defaultFilt-alignments"},
  {"group10", "defaultFilt-everything-else-label"},
  {NULL, NULL}
};

/* [filters.defaultFilt-gene-cds-prot-mrna] */
static ConfigFileLine defaultStyleLines26 [] = {
  {"feature1", "gene"},
  {"feature2", "cds"},
  {"feature3", "prot"},
  {"feature4", "mrna"},
  {"name", "Gene-mRNA-CDS-Prots"},
  {"grouplabel", "none"},
  {"layout", "geneproducts"},
  {"showtype", "yes"},
  {"showcontent", "yes"},
  {"label", "above"},
  {NULL, NULL}
};

/* [filters.defaultFilt-other-rnas] */
static ConfigFileLine defaultStyleLines27 [] = {
  {"feature1", "rna"},
  {"name", "Structural RNAs"},
  {"grouplabel", "none"},
  {"label", "above"},
  {NULL, NULL}
};

/* [filters.defaultFilt-operons] */
static ConfigFileLine defaultStyleLines28 [] = {
  {"feature1", "operon"},
  {"name", "Operons"},
  {"grouplabel", "none"},
  {"label", "above"},
  {NULL, NULL}
};

/* [filters.defaultFilt-exon-intron-label] */
static ConfigFileLine defaultStyleLines29 [] = {
  {"feature1", "exon"},
  {"feature2", "intron"},
  {"name", "Introns and Exons"},
  {"grouplabel", "none"},
  {"label", "above"},
  {NULL, NULL}
};

/* [filters.defaultFilt-variations] */
static ConfigFileLine defaultStyleLines30 [] = {
  {"feature1", "variation"},
  {"name", "Variations"},
  {"groupbox", "true"},
  {"boxcolor", "red"},
  {"grouplabel", "above"},
  {"layout", "smear"},
  {"showtype", "no"},
  {"showcontent", "no"},
  {NULL, NULL}
};

/* [filters.defaultFilt-conflicts] */
static ConfigFileLine defaultStyleLines31 [] = {
  {"feature1", "conflict"},
  {"name", "Conflicts"},
  {"groupbox", "true"},
  {"boxcolor", "dark red"},
  {"grouplabel", "above"},
  {"layout", "smear"},
  {"showtype", "no"},
  {"showcontent", "no"},
  {NULL, NULL}
};

/* [filters.defaultFilt-STS] */
static ConfigFileLine defaultStyleLines32 [] = {
  {"feature1", "STS"},
  {"name", "STS"},
  {"groupbox", "true"},
  {"boxcolor", "red"},
  {"grouplabel", "above"},
  {"layout", "smear"},
  {"showtype", "no"},
  {"showcontent", "no"},
  {NULL, NULL}
};

/* [filters.defaultFilt-impfeats] */
static ConfigFileLine defaultStyleLines33 [] = {
  {"feature1", "import"},
  {"name", "Import Features"},
  {"grouplabel", "none"},
  {"label", "above"},
  {NULL, NULL}
};

/* [filters.defaultFilt-alignments] */
static ConfigFileLine defaultStyleLines34 [] = {
  {"feature1", "align"},
  {"name", "Alignments"},
  {"grouplabel", "none"},
  {"label", "above"},
  {"layout", "smear"},
  {"showtype", "no"},
  {NULL, NULL}
};

/* [filters.defaultFilt-everything-else-label] */
static ConfigFileLine defaultStyleLines35 [] = {
  {"feature1", "everything"},
  {"grouplabel", "none"},
  {"label", "above"},
  {NULL, NULL}
};

/* [summary] */
static ConfigFileLine defaultStyleLines36 [] = {
  {"group1", "summary-gene-rna-cds-nolabel"},
  {"group2", "summary-allelse"},
  {"group3", "summary-aligns-nolabel"},
  {"name", "Summary"},
  {"defaultlayout", "compact"},
  {"rowpadding", "3"},
  {"grouppadding", "1"},
  {"label", "none"},
  {"annotlabelfont", "small"},
  {NULL, NULL}
};

/* [filters.summary-gene-rna-cds-nolabel] */
static ConfigFileLine defaultStyleLines37 [] = {
  {"feature1", "gene"},
  {"feature2", "rna"},
  {"feature3", "cds"},
  {"layout", "geneproducts"},
  {"label", "none"},
  {NULL, NULL}
};

/* [filters.summary-aligns-nolabel] */
static ConfigFileLine defaultStyleLines38 [] = {
  {"feature1", "align"},
  {"label", "none"},
  {NULL, NULL}
};

 /* [filters.summary-allelse] */
static ConfigFileLine defaultStyleLines39 [] = {
  {"feature1", "everything"},
  {"layout", "smear"},
  {"label", "none"},
  {NULL, NULL}
};


static StaticConfigFile defaultStyle [] = {
  {defaultStyleLines1, "Styles"},
  {defaultStyleLines2, "defaultStyle.master"},
  {defaultStyleLines3, "defaultStyle.bioseq"},
  {defaultStyleLines4, "defaultStyle.gene"},
  {defaultStyleLines5, "defaultStyle.mRNA"},
  {defaultStyleLines6, "defaultStyle.cds"},
  {defaultStyleLines7, "defaultStyle.tRNA"},
  {defaultStyleLines8, "defaultStyle.imp"},
  {defaultStyleLines9, "defaultStyle.exon"},
  {defaultStyleLines10, "defaultStyle.intron"},
  {defaultStyleLines11, "defaultStyle.bond"},
  {defaultStyleLines12, "defaultStyle.align"},
  {defaultStyleLines13, "summary.master"},
  {defaultStyleLines14, "summary.bioseq"},
  {defaultStyleLines15, "summary.gene"},
  {defaultStyleLines16, "summary.mRNA"},
  {defaultStyleLines17, "summary.cds"},
  {defaultStyleLines18, "summary.tRNA"},
  {defaultStyleLines19, "summary.align"},
  {defaultStyleLines20, "summary.imp"},
  {defaultStyleLines21, "summary.exon"},
  {defaultStyleLines22, "summary.intron"},
  {defaultStyleLines23, "summary.bond"},
  {defaultStyleLines24, "Filters"},
  {defaultStyleLines25, "defaultFilt"},
  {defaultStyleLines26, "filters.defaultFilt-gene-cds-prot-mrna"},
  {defaultStyleLines27, "filters.defaultFilt-other-rnas"},
  {defaultStyleLines28, "filters.defaultFilt-operons"},
  {defaultStyleLines29, "filters.defaultFilt-exon-intron-label"},
  {defaultStyleLines30, "filters.defaultFilt-variations"},
  {defaultStyleLines31, "filters.defaultFilt-conflicts"},
  {defaultStyleLines32, "filters.defaultFilt-STS"},
  {defaultStyleLines33, "filters.defaultFilt-impfeats"},
  {defaultStyleLines34, "filters.defaultFilt-alignments"},
  {defaultStyleLines35, "filters.defaultFilt-everything-else-label"},
  {defaultStyleLines36, "summary"},
  {defaultStyleLines37, "filters.summary-gene-rna-cds-nolabel"},
  {defaultStyleLines38, "filters.summary-aligns-nolabel"},
  {defaultStyleLines39, "filters.summary-allelse"},
  {NULL, NULL}
};


static void InitializeDefaultStyle (
  CharPtr configFileName
)

{
  Uint2             sectionNum, lineNum;
  ConfigFileLinePtr lines;
  CharPtr           sectionName;

  for (sectionNum = 0; defaultStyle [sectionNum].lines != NULL; sectionNum++) {
    lines = defaultStyle [sectionNum].lines;
    sectionName = defaultStyle [sectionNum].sectionName;
    for (lineNum = 0; lines [lineNum].key != NULL && lines [lineNum].value != NULL; lineNum++) {
      TransientSetAppParam (configFileName, sectionName, lines [lineNum].key, lines [lineNum].value);
    }
  }
}

/* End of automatically generated file. */

