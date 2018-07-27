/*   fstyle.c
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
* File Name:  fstyle.c
*
* Author:  Sergei Egorov, Alex Smirnov
*
* Version Creation Date:   9/23/94
*
* $Revision: 6.1 $
*
* File Description: 
*
* Modifications:  
* --------------------------------------------------------------------------
* $Log: fstyle.c,v $
* Revision 6.1  2000/09/07 21:44:51  shavirin
* Fixed bug when function Nlm_InitMuskStyles() called more, than 1 time.
*
* Revision 6.0  1997/08/25 18:22:01  madden
* Revision changed to 6.0
*
* Revision 5.3  1997/04/03 19:54:31  vakatov
* Added MSM_M_SCALE(max. number of alignments) for MSM_ALIGNMENT|MSM_NOSUBCLASS
*
*
* ==========================================================================
*/

/**************************************************************************/
/* INCLUDE */
/**************************************************************************/
#include <vibrant.h>
#include <picture.h> 
#include <fstyle.h> 
#include <fstylep.h> 

/**************************************************************************/
/* DEFINES */
/**************************************************************************/
#define MSM_TOTAL_SUBCLASSES   (MSM_TOTAL_CLASSES*MSM_SUBCLASSES_NUM)
#define MSM_TOTAL_SUBCLASSES_R (MSM_TOTAL_CLASSES_R*MSM_SUBCLASSES_NUM)
#define MSM_INDEX_OFFSET       (MSM_TOTAL_NOTPOS-1)
#define MSM_BUFFER_SIZE        128
#define MSM_CP_INDEX(i)        ((i+MSM_INDEX_OFFSET)*MSM_SUBCLASSES_NUM)

/* Parameter types: */
#define MSM_M_TRUEFALSE  0x1
#define MSM_M_COLOR      0x2
#define MSM_M_LTYPE      0x4
#define MSM_M_SHADING    0x8
#define MSM_M_PENWIDTH   0x10
#define MSM_M_NUM        0x20
#define MSM_M_STYLE      0x40
#define MSM_M_FONT       0x80
#define MSM_M_HEIGHT     0x100
#define MSM_M_SCALE      0x200
#define MSM_M_STRING     0x4000

#define DEF_ENDS_SCALE   100
#define DEF_MAXALIGN      50

/**************************************************************************/
/* TYPEDEFS */
/**************************************************************************/
typedef struct msmparams {
  CharPtr str;
  Int4    height;
  Int4    scale;
  Int4    style;
  Int4    color;
  FonT    font;
  Int2    num;
  Int1    ltype;
  Int1    shading;
  Int1    penwidth;
  Boolean truefalse;
} Nlm_MsmParams, PNTR Nlm_MsmParamsPtr;

typedef struct msmmember {
  struct msmmember PNTR next;
  Char                  name[64];
} Nlm_MsmMember, PNTR Nlm_MsmMemberPtr;

typedef struct {
  Int2  tclass;
  Int2  subclass;
  Int2  musk;
} Nlm_MsmToLoad, PNTR Nlm_MsmToLoadPtr;

typedef struct msmfont {
  struct msmfont PNTR next;
  Nlm_FontSpec        fontspec;
  FonT                fnt;
  Int2                fontnum;
  Int2                size;
  Boolean             newfont;
} Nlm_MsmFont, PNTR Nlm_MsmFontPtr;

/**************************************************************************/
/* GLOBAL VARIABLE */
/**************************************************************************/
CharPtr mSM_allfeatureNames[] =
{
/*  0 */
"",       "GENE", "ORG",  "CDS",  "PROT",
"preRNA", "mRNA", "tRNA", "rRNA", "snRNA",
/* 10 */
"scRNA",  "otherRNA",   "PUB",      "SEQ",         "IMP",
"allele", "attenuator", "C_region", "CAAT_signal", "Imp_CDS",
/* 20 */
"conflict",  "D_loop", "D_segment", "enhancer",   "exon",
"GC_signal", "iDNA",   "intron",    "J_segment",  "LTR",
/* 30 */
"mat_peptide", "misc_binding", "misc_difference", "misc_feature","misc_recomb",
"misc_RNA",    "misc_signal",  "misc_structure",  "modified_base", "mutation",
/* 40 */
"N_region",    "old_sequence", "polyA_signal",  "polyA_site", "precursor_RNA", 
"prim_transcript", "primer_bind", "promoter", "protein_bind", "RBS",
/* 50 */
"repeat_region", "repeat_unit", "rep_origin",  "S_region", "satellite",
"sig_peptide",   "source",      "stem_loop",   "STS",      "TATA_signal",
/* 60 */
"terminator", "transit_peptide", "unsure", "V_region", "V_segment",
"variation",  "virion",          "3clip",  "3UTR",     "5clip",
/* 70 */
"5UTR",    "10_signal", "35_signal", "site_ref", "REGION", 
"COMMENT", "BOND",      "SITE",      "RSITE",    "USER",
/* 80 */
"TXINIT", "NUM",        "PSEC_STR",       "NON_STD_RESIDUE", "HET",
"BIOSRC", "preprotein", "mat_peptide_aa", "sig_peptide_aa","transit_peptide_aa"
};

/**************************************************************************/
/* STATIC VARIABLE */
/**************************************************************************/
static Int2 featDefOrder[] =
{      /*  0   1   2   3   4   5   6   7   8   9 */
/*  0 */   0,  3,  0,  1,  1,  1,  2,  2,  3,  4,
/* 10 */   5,  0,  6,  7,  8,  9, 10, 11, 12,  0,
/* 20 */  13, 14, 15, 16, 17, 18, 19, 20, 21, 22,
/* 30 */   0, 23, 24, 25, 26, 27, 28,  0, 29, 30,
/* 40 */  31, 32, 33, 34, 35, 36,  0, 37, 38, 39,
/* 50 */  40, 41, 42, 43, 44,  0,  0, 45, 46, 47,
/* 60 */  48,  0, 49, 50, 51, 52, 53, 54, 55, 56,
/* 70 */  57, 58, 59,  0, 60, 61, 62, 63, 64, 65,
/* 80 */  66, 67, 68, 69, 70, 71,  2,  3,  4,  5
};

/* Names */
static CharPtr fileBaseName   = "nlmstman";
static CharPtr styleListName  = "SYSTEM";
static CharPtr fontTotalName  = "fonts";
static CharPtr styleTotalName = "styles";
static CharPtr fontName       = "name";
static CharPtr fontSizeName   = "size";
static CharPtr fontStyleName  = "style";
static CharPtr fontCharSetName= "charset";
static CharPtr fontPitchName  = "pitch";
static CharPtr fontFamilyName = "family";
static CharPtr empStr         = "";

static Nlm_MsmToLoad tpLoad[] = {
{ MSM_EXTRA_BOTH,    MSM_SEGMENT,   MSM_M_COLOR | MSM_M_SHADING |
                                    MSM_M_STYLE | MSM_M_HEIGHT},
{ MSM_EXTRA_BOTH,    MSM_SEG_BORD,  MSM_M_TRUEFALSE | MSM_M_COLOR | 
                                    MSM_M_LTYPE | MSM_M_PENWIDTH },
{ MSM_EXTRA_BOTH,    MSM_FGAP,      MSM_M_COLOR | MSM_M_LTYPE |
                                    MSM_M_STYLE },
{ MSM_EXTRA_BOTH,    MSM_FLABEL,    MSM_M_COLOR | MSM_M_STYLE |
                                    MSM_M_FONT },
{ MSM_EXTRA_MEDLINE, MSM_SEGMENT,   MSM_M_COLOR | MSM_M_SHADING |
                                    MSM_M_STYLE | MSM_M_HEIGHT},
{ MSM_EXTRA_MEDLINE, MSM_SEG_BORD,  MSM_M_TRUEFALSE | MSM_M_COLOR | 
                                    MSM_M_LTYPE | MSM_M_PENWIDTH },
{ MSM_EXTRA_MEDLINE, MSM_FGAP,      MSM_M_COLOR | MSM_M_LTYPE |
                                    MSM_M_STYLE },
{ MSM_EXTRA_MEDLINE, MSM_FLABEL,    MSM_M_COLOR | MSM_M_STYLE |
                                    MSM_M_FONT },
{ MSM_EXTRA_GENBANK, MSM_SEGMENT,   MSM_M_COLOR | MSM_M_SHADING |
                                    MSM_M_STYLE | MSM_M_HEIGHT},
{ MSM_EXTRA_GENBANK, MSM_SEG_BORD,  MSM_M_TRUEFALSE | MSM_M_COLOR | 
                                    MSM_M_LTYPE | MSM_M_PENWIDTH },
{ MSM_EXTRA_GENBANK, MSM_FGAP,      MSM_M_COLOR | MSM_M_LTYPE |
                                    MSM_M_STYLE },
{ MSM_EXTRA_GENBANK, MSM_FLABEL,    MSM_M_COLOR | MSM_M_STYLE |
                                    MSM_M_FONT },
{ MSM_EXTRA_GENBANK, MSM_NOSUBCLASS, MSM_M_COLOR | MSM_M_FONT },
{ MSM_SEQUENCE,      MSM_SEGMENT,    MSM_M_COLOR | MSM_M_SHADING |
                                     MSM_M_STYLE | MSM_M_HEIGHT },
{ MSM_SEQUENCE,      MSM_SEG_BORD,   MSM_M_TRUEFALSE | MSM_M_COLOR | 
                                     MSM_M_LTYPE | MSM_M_PENWIDTH },
{ MSM_SEQUENCE,      MSM_SLABEL,     MSM_M_COLOR | MSM_M_STYLE |
                                     MSM_M_FONT },
{ MSM_CCOLOR,        MSM_NOSUBCLASS, MSM_M_NUM },
{ MSM_ALIGNMENT,     MSM_NOSUBCLASS, MSM_M_STYLE | MSM_M_SCALE },
{ MSM_ALIGNMENT,     MSM_ALINE,      MSM_M_COLOR | MSM_M_LTYPE | 
                                     MSM_M_PENWIDTH },
{ MSM_GROUPS,        MSM_NOSUBCLASS, MSM_M_TRUEFALSE | MSM_M_COLOR | 
                                     MSM_M_NUM | MSM_M_FONT },
{ MSM_TOPSTYLE,      MSM_NOSUBCLASS, MSM_M_STYLE },
{ MSM_TOPSTYLE,      MSM_SPACE,      MSM_M_HEIGHT },
{ MSM_TOPSTYLE,      MSM_ENDS,       MSM_M_SCALE},
{ MSM_TOPSTYLE,      MSM_LABEL,      MSM_M_STYLE | MSM_M_NUM},
{ 1, 0, 0 }
};

static Nlm_MsmToLoad featureLoad[] = {
{ 1, MSM_SEGMENT,   MSM_M_COLOR | MSM_M_SHADING | MSM_M_LTYPE |
                    MSM_M_STYLE | MSM_M_HEIGHT | MSM_M_PENWIDTH },
{ 1, MSM_SEG_BORD,  MSM_M_TRUEFALSE | MSM_M_COLOR | 
                    MSM_M_LTYPE | MSM_M_PENWIDTH },
{ 1, MSM_FGAP,      MSM_M_COLOR | MSM_M_LTYPE |
                    MSM_M_PENWIDTH | MSM_M_STYLE },
{ 1, MSM_FLABEL,    MSM_M_COLOR | MSM_M_STYLE |
                    MSM_M_FONT },
{ 1, MSM_FGROUP,    MSM_NUM },
{ 1, MSM_FORDER,    MSM_NUM },
{ 0,0,0 }
};

static Boolean           msmInit = FALSE;
static Int2              msmCurrent;
static Nlm_MsmMember     allStyles;
static Int2              totalStyles;
static Nlm_MsmFontPtr    allFonts;
static Int2              totalFonts;
static Nlm_MsmParams     cpStatic[MSM_TOTAL_SUBCLASSES];
static Nlm_MsmParamsPtr  cpEdit;
static Int2              msmEdit;
static Int2              programLineHeight;

/**************************************************************************/
/* STATIC FUNCTION */
/**************************************************************************/

/**************************************************************************
* Nlm_GetIntAppParam()
*   Get integer configuration parameter
**************************************************************************/
static Int4
/*FCN*/Nlm_GetIntAppParam ( 
  CharPtr fBase,                       /* file base (I) */
  CharPtr sectionName,                 /* section (I) */
  CharPtr pName,                       /* parameter name (I) */
  Int4    defVal                       /* default value */
){
  long lval;
  Int2 i;
  Char buf[32];

  i = GetAppParam ( fBase, sectionName, pName, empStr, buf, 31 );
  if ( i == 0 ) return defVal;
  buf[i] = 0;
  sscanf ( buf, "%ld", &lval );
  return (Int4)lval;
} /* End of Nlm_GetIntAppParam() */

/**************************************************************************
* Nlm_SetIntAppParam()
*   Set integer configuration parameter
**************************************************************************/
static void
/*FCN*/Nlm_SetIntAppParam ( 
  CharPtr fBase,                       /* file base (I) */
  CharPtr sectionName,                 /* section (I) */
  CharPtr pName,                       /* parameter name (I) */
  Int4    val                          /* value */
){
  Char buf[32];

  sprintf ( buf, "%ld", (long)val );
  SetAppParam ( fBase, sectionName, pName, &(buf[0]) );
} /* End of Nlm_SetIntAppParam() */

/**************************************************************************
* Nlm_LoadDefMuskStyle()
*   loads default style into cpCur
**************************************************************************/
static void 
/*FCN*/Nlm_LoadDefMuskStyle ( 
  Nlm_MsmParamsPtr cpCur               /* style (O) */
){
  Int2             i;
  Nlm_MsmParamsPtr cptr;

  for (i = 0;  i < MSM_TOTAL_SUBCLASSES;  i++)
    MemFree( cpCur[i].str );

  MemSet ( cpCur, 0, sizeof(Nlm_MsmParams)*MSM_TOTAL_SUBCLASSES );
  cptr = &(cpCur[MSM_CP_INDEX(MSM_TOPSTYLE)]);

  cptr[MSM_NOSUBCLASS].style = MSM_HISTORY|MSM_SEGMENTS;
  cptr[MSM_SPACE].height = 10;
  cptr[MSM_ENDS].scale = DEF_ENDS_SCALE;
  cptr[MSM_LABEL].num = 32;

  cptr = &(cpCur[MSM_CP_INDEX(MSM_GROUPS)]);

  cptr[MSM_NOSUBCLASS].num = 3;
  cptr[MSM_NOSUBCLASS].font = Nlm_programFont;
  cptr[MSM_NOSUBCLASS].height = programLineHeight;
  cptr[1].num = 1;
  cptr[1].str = StringSave ( "GENE-mRNA-CDS" );   /*NEED FIX*/
  cptr[2].num = 2;
  cptr[2].str = StringSave ( "PROTEINs" );  /*NEED FIX*/
  cptr[3].num = 3;
  cptr[3].str = StringSave ( "Other features" );  /*NEED FIX*/

  cptr = &(cpCur[MSM_CP_INDEX(MSM_ALIGNMENT)]);

  cptr[MSM_NOSUBCLASS].style = MSM_SEQHIST;
  cptr[MSM_NOSUBCLASS].scale = DEF_MAXALIGN;
  cptr[MSM_ALINE].ltype = SOLID_LINE;
  cptr[MSM_ALINE].penwidth = 1;
  cptr[MSM_ALINE].color = 0xFF00;

  cptr = &(cpCur[MSM_CP_INDEX(MSM_CCOLOR)]);

  cptr[MSM_NOSUBCLASS].num = 4;
  cptr[1].color = 0xFF0000;
  cptr[2].color = 0;
  cptr[3].color = 0xFF;
  cptr[4].color = 0xFF00FF;

  cptr = &(cpCur[MSM_CP_INDEX(MSM_SEQUENCE)]);

  cptr[MSM_SEGMENT].shading = SOLID_SHADING;
  cptr[MSM_SEGMENT].style = MSM_SEG_BOX;
  cptr[MSM_SEGMENT].height = 10;
  cptr[MSM_SEGMENT].color = 0;
  cptr[MSM_SEG_BORD].truefalse = FALSE;
  cptr[MSM_SEG_BORD].ltype = SOLID_LINE;
  cptr[MSM_SEG_BORD].penwidth = 1;
  cptr[MSM_SEG_BORD].color = 0;
  cptr[MSM_SLABEL].style = PRINTID_FASTA_SHORT;
  cptr[MSM_SLABEL].font = Nlm_programFont;
  cptr[MSM_SLABEL].height = programLineHeight;

  for(i = MSM_EXTRA_GENBANK; i>= MSM_EXTRA_BOTH; --i) {

    cptr = &(cpCur[MSM_CP_INDEX(i)]);

    cptr[MSM_SEGMENT].shading = SOLID_SHADING;
    cptr[MSM_SEGMENT].style = MSM_SEG_BOX|MSM_SEG_SHOWORIENT;
    cptr[MSM_SEGMENT].height = 8;
    cptr[MSM_SEG_BORD].truefalse = FALSE;
    cptr[MSM_SEG_BORD].ltype = SOLID_LINE;
    cptr[MSM_SEG_BORD].penwidth = 1;
    cptr[MSM_FGAP].ltype = SOLID_LINE;
    cptr[MSM_FGAP].penwidth = 1;
    cptr[MSM_FGAP].style = MSM_GAP_LINE;
    cptr[MSM_FLABEL].style = MSM_LABEL_CONTENT;
    cptr[MSM_FLABEL].font = Nlm_programFont;
    cptr[MSM_FLABEL].color = 0xFF00FF;
    cptr[MSM_FLABEL].height = programLineHeight;
  }

  for ( i=1; i<=MSM_TOTAL_POSITIVE; i++ ){

    cptr = &(cpCur[MSM_CP_INDEX(i)]);

    cptr[MSM_SEGMENT].shading = SOLID_SHADING;
    cptr[MSM_SEGMENT].style = MSM_SEG_BOX|MSM_SEG_SHOWORIENT;
    cptr[MSM_SEGMENT].height = 8;
    cptr[MSM_SEG_BORD].truefalse = FALSE;
    cptr[MSM_SEG_BORD].ltype = SOLID_LINE;
    cptr[MSM_SEG_BORD].penwidth = 1;
    cptr[MSM_FGAP].ltype = SOLID_LINE;
    cptr[MSM_FGAP].penwidth = 1;
    cptr[MSM_FGAP].style = MSM_GAP_LINE;
    cptr[MSM_FLABEL].style = MSM_LABEL_CONTENT;
    cptr[MSM_FLABEL].font = Nlm_programFont;
    cptr[MSM_FLABEL].height = programLineHeight;
    cptr[MSM_FGROUP].num = 3;
    cptr[MSM_FORDER].num = featDefOrder[i];
  }

  cptr = &(cpCur[MSM_CP_INDEX(FEATDEF_CDS)]);
  cptr[MSM_SEGMENT].color = 0xFF00FF;
  cptr[MSM_FGROUP].num = 1;
  cptr = &(cpCur[MSM_CP_INDEX(FEATDEF_mRNA)]);
  cptr[MSM_SEGMENT].color = 0xFF;
  cptr[MSM_FGROUP].num = 1;
  cptr = &(cpCur[MSM_CP_INDEX(FEATDEF_GENE)]);
  cptr[MSM_FGROUP].num = 1;

  cptr = &(cpCur[MSM_CP_INDEX(FEATDEF_PROT)]);
  cptr[MSM_FGROUP].num = 2;
  cptr = &(cpCur[MSM_CP_INDEX(FEATDEF_preprotein)]);
  cptr[MSM_FGROUP].num = 2;
  cptr = &(cpCur[MSM_CP_INDEX(FEATDEF_mat_peptide_aa)]);
  cptr[MSM_FGROUP].num = 2;
  cptr = &(cpCur[MSM_CP_INDEX(FEATDEF_sig_peptide_aa)]);
  cptr[MSM_FGROUP].num = 2;
  cptr = &(cpCur[MSM_CP_INDEX(FEATDEF_transit_peptide_aa)]);
  cptr[MSM_FGROUP].num = 2;

}  /* End of Nlm_LoadDefMuskStyle() */

/**************************************************************************
* Nlm_GetLoadedMuskFont()
*   returns hande of loaded font or NULL if not found
**************************************************************************/
static FonT
/*FCN*/Nlm_GetLoadedMuskFont ( 
  Int2     fnum,                       /* Font number (I)*/
  Int2Ptr  fsize                       /* Font size (O) */
){
  Nlm_MsmFontPtr p;

  p = allFonts;
  while ( p != NULL ){
    if ( fnum == p->fontnum ) {
      if ( fsize != NULL ) *fsize = p->size;
      return p->fnt;
    }
    p = p->next;
  }
  return NULL;
} /* End of Nlm_GetLoadedMuskFont() */

/**************************************************************************
* Nlm_FreeAllFont()
*   frees all font resourses
**************************************************************************/
static void
/*FCN*/Nlm_FreeAllFont ( 
  void
){
  Nlm_MsmFontPtr p1, p2;

  p1 = allFonts;
  while ( p1 != NULL ){
    p2 = p1->next;
    if ( p1->fnt != Nlm_programFont ) p1->fnt = Nlm_DeleteFont ( p1->fnt );
    MemFree ( p1 );
    p1 = p2;
  }
} /* End of Nlm_FreeAllFont() */

/**************************************************************************
* Nlm_GetMuskFont()
*   returns font hande
**************************************************************************/
static FonT 
/*FCN*/Nlm_GetMuskFont (
  Int2    fnum,                        /* Font number (I) */
  Int2Ptr fsize                        /* Font size (O) */
){
  Nlm_FontSpec fsp;
  FonT         fnt;
  Int2         i;
  Char         sectionName[64];
  Char         buf[FONT_NAME_SIZE+1];

  fnt = Nlm_GetLoadedMuskFont (fnum, fsize);
  if ( fnt == NULL ){
    sprintf ( sectionName, "SYSTEMFONT:%d", (int)fnum );
    i = GetAppParam ( fileBaseName, sectionName, fontName, empStr, buf, 
                      FONT_NAME_SIZE );
    if ( i == 0 ) {
      *fsize = programLineHeight;
      return NULL;
    }
    buf[i] = 0;
    StringCpy ( fsp.name, buf );
    fsp.size = (Int2)Nlm_GetIntAppParam(fileBaseName, sectionName,
                                        fontSizeName, 12 );
    fsp.style = (Uint1)Nlm_GetIntAppParam(fileBaseName, sectionName,
                                        fontStyleName, STYLE_REGULAR );
    fsp.charset = (Uint1)Nlm_GetIntAppParam(fileBaseName, sectionName,
                                        fontCharSetName, CHARSET_NULL );
    fsp.pitch = (Uint1)Nlm_GetIntAppParam(fileBaseName, sectionName,
                                        fontPitchName, PITCH_NULL );
    fsp.family = (Uint1)Nlm_GetIntAppParam(fileBaseName, sectionName,
                                        fontFamilyName, FAMILY_ROMAN );
    fnt = Nlm_LoadMuskFont ( &fsp, fnum, fsize );
  }
  return fnt;
} /* End of Nlm_GetMuskFont() */

/**************************************************************************
* Nlm_SaveFonts()
*   save new font resourses
**************************************************************************/
static void
/*FCN*/Nlm_SaveFonts ( 
  void
){
  Nlm_MsmFontPtr p;
  Char           sectionName[64];
  Boolean        oldCacheVal;

  oldCacheVal = CacheAppParam(TRUE);

  p = allFonts;
  while ( p != NULL ){
    if ( p->newfont ) {
      sprintf ( sectionName, "SYSTEMFONT:%d", (int)p->fontnum );
      SetAppParam ( fileBaseName, sectionName, fontName, p->fontspec.name ); 
      Nlm_SetIntAppParam ( fileBaseName, sectionName, fontSizeName, 
                    (Int4)p->fontspec.size ); 
      Nlm_SetIntAppParam ( fileBaseName, sectionName, fontStyleName,
                    (Int4)p->fontspec.style ); 
      Nlm_SetIntAppParam ( fileBaseName, sectionName, fontCharSetName, 
                    (Int4)p->fontspec.charset ); 
      Nlm_SetIntAppParam ( fileBaseName, sectionName, fontPitchName, 
                    (Int4)p->fontspec.pitch ); 
      Nlm_SetIntAppParam ( fileBaseName, sectionName, fontFamilyName, 
                    (Int4)p->fontspec.family ); 
      p->newfont = FALSE;
    }
    p = p->next;
  }
  Nlm_SetIntAppParam ( fileBaseName, styleListName, fontTotalName, 
                  (Int4)totalFonts ); 
  CacheAppParam(oldCacheVal);
} /* End of Nlm_SaveFonts() */

/**************************************************************************
* Nlm_GetFontNumber()
*   returns number of font
**************************************************************************/
static Int2
/*FCN*/ Nlm_GetFontNumber (
  FonT  fnt                            /* font handle (I) */
){
  Nlm_MsmFontPtr p;

  p = allFonts;
  while ( p != NULL ){
    if ( p->fnt == fnt ) return p->fontnum;
    p = p->next;
  }
  return 0;
} /* End of Nlm_GetFontNumber () */

/**************************************************************************
* Nlm_Uint1ToStr()
*   convert Uint1 to string
**************************************************************************/
static void 
/*FCN*/ Nlm_Uint1ToStr (
  CharPtr str,                         /* string (O) */
  Uint1   val                          /* value (I) */
){
  *str++ = ((val>>4)&0xF) + 'A';
  *str   = (val&0xF) + 'A';
} /*End of Nlm_Uint1ToStr() */

/**************************************************************************
* Nlm_Uint2ToStr()
*   convert Uint2 to string
**************************************************************************/
static void 
/*FCN*/ Nlm_Uint2ToStr (
  CharPtr str,                         /* string (O) */
  Uint2   val                          /* value (I) */
){
  *str++ = ((val>>12)&0xF) + 'A';
  *str++ = ((val>>8)&0xF) + 'A';
  *str++ = ((val>>4)&0xF) + 'A';
  *str   = (val&0xF) + 'A';
} /*End of Nlm_Uint2ToStr()*/

/**************************************************************************
* Nlm_Uint4ToStr()
*   convert Uint4 to string
**************************************************************************/
static void 
/*FCN*/ Nlm_Uint4ToStr (
  CharPtr str,                         /* string (O) */
  Uint4   val                          /* value (I) */
){
  *str++ = (Char)((val>>28)&0xF) + 'A';
  *str++ = (Char)((val>>24)&0xF) + 'A';
  *str++ = (Char)((val>>20)&0xF) + 'A';
  *str++ = (Char)((val>>16)&0xF) + 'A';
  *str++ = (Char)((val>>12)&0xF) + 'A';
  *str++ = (Char)((val>>8)&0xF) + 'A';
  *str++ = (Char)((val>>4)&0xF) + 'A';
  *str   = (Char)(val&0xF) + 'A';
} /*End of Nlm_Uint2ToStr()*/

/**************************************************************************
* Nlm_StrToUint1()
*   convert string to Uint1
**************************************************************************/
static Uint1
/*FCN*/ Nlm_StrToUint1 (
  CharPtr str                          /* string (I) */
){
  Uint1 val;

  val = (Uint1)(*str++ - 'A')<<4;
  val |= (Uint1)(*str - 'A');
  return val;
} /*End of Nlm_StrToUint1()*/

/**************************************************************************
* Nlm_StrToUint2()
*   convert string to Uint2
**************************************************************************/
static Uint2
/*FCN*/ Nlm_StrToUint2 (
  CharPtr str                          /* string (I) */
){
  Uint2 val;

  val = (Uint2)(*str++ - 'A')<<12;
  val |= (Uint2)(*str++ - 'A')<<8;
  val |= (Uint2)(*str++ - 'A')<<4;
  val |= (Uint2)(*str - 'A');
  return val;
} /*End of Nlm_StrToUint2()*/

/**************************************************************************
* Nlm_StrToUint4()
*   convert string to Uint4
**************************************************************************/
static Uint4
/*FCN*/ Nlm_StrToUint4 (
  CharPtr str                          /* string (I) */
){
  Uint4 val;

  val  = (Uint4)(*str++ - 'A')<<28;
  val |= (Uint4)(*str++ - 'A')<<24;
  val |= (Uint4)(*str++ - 'A')<<20;
  val |= (Uint4)(*str++ - 'A')<<16;
  val |= (Uint4)(*str++ - 'A')<<12;
  val |= (Uint4)(*str++ - 'A')<<8;
  val |= (Uint4)(*str++ - 'A')<<4;
  val |= (Uint4)(*str - 'A');
  return val;
} /*End of Nlm_StrToUint4()*/

/**************************************************************************
* Nlm_LoadMuskSubClass()
*   loads subclass information from config file
**************************************************************************/
static void 
/*FCN*/LoadMuskSubClass ( 
  CharPtr          name,               /* style name (I) */
  Int2             classNum,           /* class number (I) */
  Int2             subclassNum,        /* subclass number (I) */
  Nlm_MsmParamsPtr p                   /* pointer to parameters (O) */
){
  CharPtr cPtr;
  Int2    i;
  Int2    fspec;
  Char    buf[MSM_BUFFER_SIZE+1];
  Char    pName[9];

  Nlm_Uint2ToStr ( &(pName[0]), classNum );
  Nlm_Uint2ToStr ( &(pName[4]), subclassNum );
  pName[8] = 0;
  i = GetAppParam ( fileBaseName, name, pName, empStr, buf, 
                    MSM_BUFFER_SIZE );
  if ( i >= 4 ){
    buf[i] = 0;
    cPtr = buf;
    i = Nlm_StrToUint2 (cPtr); cPtr += 4;
    if ( i & MSM_M_TRUEFALSE )
      {p->truefalse = (Boolean)Nlm_StrToUint1 (cPtr); cPtr += 2;}
    if ( i & MSM_M_COLOR )
      {p->color = (Int4)Nlm_StrToUint4 (cPtr); cPtr += 8;}
    if ( i & MSM_M_LTYPE )
      {p->ltype = (Int1)Nlm_StrToUint1 (cPtr); cPtr += 2;}
    if ( i & MSM_M_SHADING )
      {p->shading = (Int1)Nlm_StrToUint1 (cPtr); cPtr += 2;}
    if ( i & MSM_M_PENWIDTH )
      {p->penwidth = (Int1)Nlm_StrToUint1 (cPtr); cPtr += 2;}
    if ( i & MSM_M_NUM )
      {p->num = (Int2)Nlm_StrToUint2 (cPtr); cPtr += 4;}
    if ( i & MSM_M_STYLE )
      {p->style = (Int4)Nlm_StrToUint4 (cPtr); cPtr += 8;}
    if ( i & MSM_M_FONT ) {
      fspec = (Int2)Nlm_StrToUint2 (cPtr); cPtr += 4;
      if ( fspec > 0 ){
        p->font = Nlm_GetMuskFont ( fspec, &fspec );
        p->height = (Int4)fspec;
      }
    }
    if ( i & MSM_M_HEIGHT )
      {p->height = (Int4)Nlm_StrToUint4 (cPtr); cPtr += 8;}
    if ( i & MSM_M_SCALE )
      {p->scale = (Int4)Nlm_StrToUint4 (cPtr); cPtr += 8;}
    if ( i & MSM_M_STRING ) {
      MemFree( p->str );
      p->str = StringSave( cPtr );
    }
  }
} /* End of Nlm_LoadMuskSubClass() */

/**************************************************************************
* Nlm_SaveMuskSubClass()
*   save subclass information into config file
**************************************************************************/
static void 
/*FCN*/SaveMuskSubClass ( 
  CharPtr          name,               /* style name (I) */
  Int2             classNum,           /* class number (I) */
  Int2             subclassNum,        /* subclass number (I) */
  Nlm_MsmParamsPtr p,                  /* pointer to parameters (I) */
  Nlm_MsmParamsPtr pdef                /* pointer to default parameters (I) */
){
  CharPtr cPtr;
  Uint2   musk;
  Char    buf[MSM_BUFFER_SIZE+1];
  Char    pName[9];

  Nlm_Uint2ToStr ( &(pName[0]), classNum );
  Nlm_Uint2ToStr ( &(pName[4]), subclassNum );
  pName[8] = 0;
  musk = 0;
  if ( (p->str != NULL) || (pdef->str != NULL) ){
    if ( (p->str == NULL) || (pdef->str == NULL) ||
         StringCmp ( p->str, pdef->str ) ) musk |= MSM_M_STRING;
  }
  if ( pdef->font != NULL ){
    if ( pdef->font != p->font ) musk |= MSM_M_FONT;
  } else if (p->height != pdef->height) musk |= MSM_M_HEIGHT;
  if ( p->scale != pdef->scale ) musk |= MSM_M_SCALE;
  if ( p->style != pdef->style ) musk |= MSM_M_STYLE;
  if ( p->color != pdef->color ) musk |= MSM_M_COLOR;
  if ( p->num != pdef->num ) musk |= MSM_M_NUM;
  if ( p->ltype != pdef->ltype ) musk |= MSM_M_LTYPE;
  if ( p->shading != pdef->shading ) musk |= MSM_M_SHADING;
  if ( p->penwidth != pdef->penwidth ) musk |= MSM_M_PENWIDTH;
  if ( p->truefalse != pdef->truefalse ) musk |= MSM_M_TRUEFALSE;
  if ( musk == 0 ){
    SetAppParam ( fileBaseName, name, pName, NULL );
    return;
  }
  cPtr = buf;
  Nlm_Uint2ToStr (cPtr, musk); cPtr += 4;
  if ( musk & MSM_M_TRUEFALSE ){
    Nlm_Uint1ToStr (cPtr, p->truefalse ); cPtr += 2;}
  if ( musk & MSM_M_COLOR ){
    Nlm_Uint4ToStr (cPtr, p->color ); cPtr += 8;}
  if ( musk & MSM_M_LTYPE ){
    Nlm_Uint1ToStr (cPtr, p->ltype ); cPtr += 2;}
  if ( musk & MSM_M_SHADING ){
    Nlm_Uint1ToStr (cPtr, p->shading ); cPtr += 2;}
  if ( musk & MSM_M_PENWIDTH ){
    Nlm_Uint1ToStr (cPtr, p->penwidth ); cPtr += 2;}
  if ( musk & MSM_M_NUM ){
    Nlm_Uint2ToStr (cPtr, p->num ); cPtr += 4;}
  if ( musk & MSM_M_STYLE ){
    Nlm_Uint4ToStr (cPtr, p->style ); cPtr += 8;}
  if ( musk & MSM_M_FONT ){
    Nlm_Uint2ToStr (cPtr, Nlm_GetFontNumber(p->font)); cPtr += 4;}
  if ( musk & MSM_M_HEIGHT ){
    Nlm_Uint4ToStr (cPtr, p->height ); cPtr += 8;}
  if ( musk & MSM_M_SCALE ){
    Nlm_Uint4ToStr (cPtr, p->scale ); cPtr += 8;}
  if ( musk & MSM_M_STRING ){
    StringCpy ( cPtr, p->str ); cPtr+=StringLen(p->str);}
  *cPtr = 0;
  SetAppParam ( fileBaseName, name, pName, buf );
} /* End of Nlm_SaveMuskSubClass() */

/**************************************************************************
* Nlm_LoadNamedMuskStyle()
*   loads style information from config file
**************************************************************************/
static void 
/*FCN*/Nlm_LoadNamedMuskStyle ( 
  CharPtr          name,               /* style name (I) */
  Nlm_MsmParamsPtr cpCur               /* style (O) */
){
  Nlm_MsmParamsPtr cPtr;
  Nlm_MsmToLoadPtr muskPtr;
  Int2             total;
  Int2             i;

  muskPtr = &(tpLoad[0]);
  while ( muskPtr->tclass <= 0 ){
    cPtr = &(cpCur[MSM_CP_INDEX(muskPtr->tclass) + muskPtr->subclass]);
    LoadMuskSubClass ( name, muskPtr->tclass, muskPtr->subclass,
                       cPtr );
    muskPtr++;
  }
  total = cpCur[MSM_CP_INDEX(MSM_CCOLOR)+MSM_NOSUBCLASS].num;
  cPtr = &(cpCur[MSM_CP_INDEX(MSM_CCOLOR) + 1]);
  for ( i=1; i<=total; i++ ){
    LoadMuskSubClass ( name, MSM_CCOLOR, i, cPtr++ );
  }
  total = cpCur[MSM_CP_INDEX(MSM_GROUPS)+MSM_NOSUBCLASS].num;
  cPtr = &(cpCur[MSM_CP_INDEX(MSM_GROUPS) + 1]);
  if ( total >= MSM_SUBCLASSES_NUM ) total = MSM_SUBCLASSES_NUM-1;
  for ( i=1; i<=total; i++ ){
    LoadMuskSubClass ( name, MSM_GROUPS, i, cPtr++ );
  }
  for ( i=1; i<=MSM_TOTAL_POSITIVE; i++ ){
    muskPtr = &(featureLoad[0]);
    while ( muskPtr->tclass > 0 ){
      cPtr = &(cpCur[MSM_CP_INDEX(i) + muskPtr->subclass]);
      LoadMuskSubClass ( name, i, muskPtr->subclass, cPtr );
      muskPtr++;
    }
  }
} /* End of Nlm_LoadNamedMuskStyle() */

/**************************************************************************
* Nlm_SaveNamedMuskStyle()
*   save style information into config file
**************************************************************************/
static void 
/*FCN*/Nlm_SaveNamedMuskStyle ( 
  CharPtr          name,               /* style name (I) */
  Nlm_MsmParamsPtr cpCur,              /* style (I) */
  Nlm_MsmParamsPtr cpDefCur            /* default style (I) */
){
  Nlm_MsmParamsPtr cPtr;
  Nlm_MsmParamsPtr cDefPtr;
  Nlm_MsmToLoadPtr muskPtr;
  Int2             total;
  Int2             i;
  Boolean          oldCacheVal;

  oldCacheVal = CacheAppParam(TRUE);
  muskPtr = &(tpLoad[0]);
  while ( muskPtr->tclass <= 0 ){
    i = MSM_CP_INDEX(muskPtr->tclass) + muskPtr->subclass;
    cPtr = &(cpCur[i]); cDefPtr = &(cpDefCur[i]);
    SaveMuskSubClass ( name, muskPtr->tclass, muskPtr->subclass,
                       cPtr, cDefPtr );
    muskPtr++;
  }
  total = cpCur[MSM_CP_INDEX(MSM_CCOLOR)+MSM_NOSUBCLASS].num;
  i = MSM_CP_INDEX(MSM_CCOLOR) + 1;
  cPtr = &(cpCur[i]); cDefPtr = &(cpDefCur[i]);
  for ( i=1; i<=total; i++ ){
    SaveMuskSubClass ( name, MSM_CCOLOR, i, cPtr++, cDefPtr++ );
  }
  total = cpCur[MSM_CP_INDEX(MSM_GROUPS)+MSM_NOSUBCLASS].num;
  i = MSM_CP_INDEX(MSM_GROUPS) + 1;
  cPtr = &(cpCur[i]); cDefPtr = &(cpDefCur[i]);
  if ( total >= MSM_SUBCLASSES_NUM ) total = MSM_SUBCLASSES_NUM-1;
  for ( i=1; i<=total; i++ ){
    SaveMuskSubClass ( name, MSM_GROUPS, i, cPtr++, cDefPtr++ );
  }
  for ( total=1; total<=MSM_TOTAL_POSITIVE; total++ ){
    muskPtr = &(featureLoad[0]);
    while ( muskPtr->tclass > 0 ){
      i = MSM_CP_INDEX(total) + muskPtr->subclass;
      cPtr = &(cpCur[i]); cDefPtr = &(cpDefCur[i]);
      SaveMuskSubClass ( name, total, muskPtr->subclass, cPtr, cDefPtr );
      muskPtr++;
    }
  }
  CacheAppParam(oldCacheVal);
} /* End of Nlm_SaveNamedMuskStyle() */

/**************************************************************************
* Nlm_GetMuskCP()
*   get the conf param
**************************************************************************/
static BigScalar
/*FCN*/Nlm_GetMuskCP ( 
  Nlm_MsmParamsPtr style,              /* style (I) */
  Int2 p_class,                        /* class (I) */
  Int2 p_subclass,                     /* subclass (I) */
  Int2 p_type                          /* type (I) */
){
  BigScalar retval = 0;
  Int2      index;

  if ( (p_class > -MSM_TOTAL_NOTPOS) && (p_class <= MSM_TOTAL_POSITIVE_R) ){
    index = (p_class + MSM_INDEX_OFFSET)*MSM_SUBCLASSES_NUM;
  } else return retval;
  if ( (p_subclass >= 0) && (p_subclass < MSM_SUBCLASSES_NUM) ){
    index += p_subclass;
  } else return retval;
  switch ( p_type ){
    case MSM_TRUEFALSE:
      retval = (BigScalar)style[index].truefalse; break;
    case MSM_COLOR:
      retval = (BigScalar)style[index].color; break;
    case MSM_LTYPE:
      retval = (BigScalar)style[index].ltype; break;
    case MSM_SHADING:
      retval = (BigScalar)style[index].shading; break;
    case MSM_PENWIDTH:
      retval = (BigScalar)style[index].penwidth; break;
    case MSM_NUM:
      retval = (BigScalar)style[index].num; break;
    case MSM_STYLE:
      retval = (BigScalar)style[index].style; break;
    case MSM_FONT:
      retval = (BigScalar)style[index].font; 
      if ( retval == (BigScalar)NULL ) retval = (BigScalar)Nlm_programFont;
      break;
    case MSM_HEIGHT:
      retval = (BigScalar)style[index].height; break;
    case MSM_STRING:
      retval = (BigScalar)style[index].str; break;
    case MSM_SCALE:
      retval = (BigScalar)style[index].scale; break;
  }
  return retval;
} /* End of Nlm_GetMuskCP() */

/**************************************************************************
* Nlm_GetStylePtr()
*   return pointer to style member
**************************************************************************/
static Nlm_MsmMemberPtr
/*FCN*/Nlm_GetStylePtr ( 
  Int2 styleNum                        /* style number (I) */
){
  Nlm_MsmMemberPtr list;
  Int2             i;

  list = &allStyles;
  for ( i=0; i<styleNum; i++ ){
    if ( list == NULL ) break;
    list = list->next;
  }
  return list;
} /* End of Nlm_GetStylePtr () */

/**************************************************************************
* Nlm_GetStyleNum()
*   return style number
**************************************************************************/
static Int2
/*FCN*/Nlm_GetStyleNum ( 
  CharPtr styleName                    /* style number (I) */
){
  Nlm_MsmMemberPtr list;
  Int2             i;

  list = &allStyles;
  i = 0;
  while ( list != NULL ) {
    if ( StringCmp ( list->name, styleName ) == 0 ) return i;
    i++;
    list = list->next;
  }
  return -1;
} /* End of Nlm_GetStyleNum () */

/**************************************************************************/
/* GLOBAL FUNCTIONS */
/**************************************************************************/

/**************************************************************************
* Nlm_LoadMuskFont()
*   returns hande of loaded font
**************************************************************************/
FonT
/*FCN*/Nlm_LoadMuskFont ( 
  Nlm_FontSpecPtr fsp,                 /* Pointer to FontSpec (I) */
  Int2            fnum,                /* font number (0 - new) (I) */
  Int2Ptr         fsize                /* font size (O) */
){
  Nlm_MsmFontPtr p;

  if ( fnum == 0 ) {
    p = allFonts;
    while ( p != NULL ){
      if ( Nlm_EqualFontSpec ( fsp, &(p->fontspec) ) ) {
        if ( fsize != NULL ) *fsize = p->size;
        return p->fnt;
      }
      p = p->next;
    }
  }
  p = MemNew (sizeof(Nlm_MsmFont));
  p->fnt = Nlm_CreateFont (fsp);
  if ( p->fnt == NULL ) p->fnt = Nlm_programFont;
  p->fontspec = *fsp;
  if ( fnum ){
    p->fontnum = fnum;
  } else {
    totalFonts++;
    p->fontnum = totalFonts;
    p->newfont = TRUE;
  }
  p->next = allFonts;
  allFonts = p;
  SelectFont ( p->fnt );
  p->size = Nlm_LineHeight();
  if ( fsize != NULL ) *fsize = p->size;
  return p->fnt;
} /* End of Nlm_LoadMuskFont() */

/**************************************************************************
* Nlm_InitMuskStyles()
*   init the system
**************************************************************************/
void LIBCALL
/*FCN*/Nlm_InitMuskStyles ( 
  void
){
  Nlm_MsmMemberPtr cp;
  Int2             i,j;
  Char             buf1[MSM_BUFFER_SIZE+1];
  Char             buf2[32];

  if ( !msmInit ){
    SelectFont ( Nlm_programFont );
    programLineHeight = Nlm_stdLineHeight;
    SelectFont ( Nlm_systemFont );
    Nlm_LoadDefMuskStyle( &(cpStatic[0]) );
    totalStyles = Nlm_GetIntAppParam ( fileBaseName, styleListName, 
                                   styleTotalName, 1 ); 
    totalFonts = Nlm_GetIntAppParam ( fileBaseName, styleListName, 
                                   fontTotalName, 0 ); 
    StringCpy ( allStyles.name, styleListName );
    allStyles.next = NULL;
    cp = &allStyles;
    for ( i=1; i<totalStyles; i++ ){
      sprintf ( buf2, "STYLE%d", i );
      j = GetAppParam ( fileBaseName, styleListName, buf2, empStr, buf1, 
                        MSM_BUFFER_SIZE);
      if ( j == 0 ) break;
      if ( j > 63 ) j = 63;
      buf1[j] = 0;
      cp->next = MemNew ( sizeof(Nlm_MsmMember) );
      cp = cp->next;
      cp->next = NULL;
      StringCpy ( cp->name, buf1 );
    }
    msmCurrent = 0;
    allFonts = NULL;
    cpEdit = NULL;
    msmEdit = -1;
    msmInit = TRUE;
  }
}  /* End of Nlm_InitMuskStyles () */

/**************************************************************************
* Nlm_ExitMuskStyles()
*   free all resourses
**************************************************************************/
void LIBCALL
/*FCN*/Nlm_ExitMuskStyles ( void )
{
    Nlm_MsmMemberPtr cp, cpold;
    Nlm_Int2         i;
    
    for (i=0; i<MSM_TOTAL_SUBCLASSES; i++){
        if ( cpStatic[i].str != NULL ) MemFree (cpStatic[i].str);
        cpStatic[i].str = NULL;
    }

    cpold = allStyles.next;
    while ( cpold != NULL ){
        cp = cpold->next;
        MemFree ( cpold );
        cpold = cp;
    }
    allStyles.next = NULL;
    Nlm_FreeAllFont();
    msmInit = FALSE;
}  /* End of Nlm_ExitMuskStyles() */

/**************************************************************************
* Nlm_GetMuskCParam()
*   return configuration parameter
**************************************************************************/
BigScalar 
/*FCN*/Nlm_GetMuskCParam ( 
  Int2 p_class,                        /* class (I) */
  Int2 p_subclass,                     /* subclass (I) */
  Int2 p_type                          /* type (I) */
){
  if ( p_class <= MSM_TOTAL_POSITIVE ){
    return Nlm_GetMuskCP(cpStatic, p_class, p_subclass, p_type );
  }
  return (BigScalar)0;
} /* End of GetMuskCParam() */

/**************************************************************************
* Nlm_GetMuskTotalSt()
*   return total number of styles
**************************************************************************/
Int2 
/*FCN*/Nlm_GetMuskTotalSt ( 
  void
){
  return totalStyles;
} /* End of Nlm_GetMuskTotalSt() */

/**************************************************************************
* Nlm_GetMuskStyleName()
*   return the style name
**************************************************************************/
CharPtr 
/*FCN*/Nlm_GetMuskStyleName ( 
  Int2 styleNum                        /* style number (I) */
){
  Nlm_MsmMemberPtr p;
  CharPtr          name = NULL;

  if ( !msmInit ) Nlm_InitMuskStyles ();
  p = Nlm_GetStylePtr(styleNum);
  if ( p != NULL ){
    name = &(p->name[0]);
  }
  return name;
} /* End of Nlm_GetMuskStyleName () */

/**************************************************************************
* Nlm_GetMuskCurrentSt()
*   return the current style number
**************************************************************************/
Int2 
/*FCN*/Nlm_GetMuskCurrentSt ( 
  void
){
  if ( !msmInit ) Nlm_InitMuskStyles ();
  return msmCurrent;
} /* End of Nlm_GetMuskCurrentSt () */

/**************************************************************************
* Nlm_SetMuskCurrentSt()
*   sets the current style number
**************************************************************************/
void 
/*FCN*/Nlm_SetMuskCurrentSt ( 
  CharPtr name                         /* style number (I) */
){
  Nlm_MsmMemberPtr m;
  Int2             styleNum;

  if ( !msmInit ) Nlm_InitMuskStyles ();
  styleNum = Nlm_GetStyleNum ( name );
  if ( (styleNum != msmCurrent)&&(styleNum >= 0) ){
    Nlm_LoadDefMuskStyle( &(cpStatic[0]) );
    if ( styleNum != 0 ){
      m = Nlm_GetStylePtr ( styleNum );
      Nlm_LoadNamedMuskStyle ( &(m->name[0]), &(cpStatic[0]) );
    }
    msmCurrent = styleNum;
  }
}  /* End of Nlm_SetMuskCurrentSt() */

/**************************************************************************
* Nlm_CopyMuskStyle()
*   copy style
**************************************************************************/
Nlm_Int2
/*FCN*/Nlm_CopyMuskStyle ( 
  Nlm_CharPtr name,                    /* name of new style (I) */
  Nlm_Int2    styleNum                 /* src style */
){
  Nlm_MsmParamsPtr cpCur;
  Nlm_MsmParamsPtr cpDefCur;
  Nlm_MsmMemberPtr list;
  Nlm_Int2         i;
  Char             buf[32];

  if ( !msmInit ) Nlm_InitMuskStyles ();
  if ( Nlm_GetMuskStyleName ( styleNum ) == NULL ) return -1;
  if ( (name == NULL) || (*name == 0) || (StringLen(name)) > 63 ) return -1;
  if ( Nlm_GetStyleNum ( name ) != -1 ) return -1;
  cpCur = MemNew ( MSM_TOTAL_SUBCLASSES * sizeof(Nlm_MsmParams) );
  cpDefCur = MemNew ( MSM_TOTAL_SUBCLASSES * sizeof(Nlm_MsmParams) );
  Nlm_LoadDefMuskStyle( cpCur );
  Nlm_LoadDefMuskStyle( cpDefCur );
  if ( styleNum != 0 ){
    list = Nlm_GetStylePtr ( styleNum );
    if ( list != NULL )  Nlm_LoadNamedMuskStyle (list->name, cpCur);
  }
  list = &(allStyles);
  while ( list->next != NULL ){
    list = list->next;
  }
  list->next = MemNew(sizeof(Nlm_MsmMember));
  list = list->next;
  StringCpy ( list->name, name );
  list->next = NULL;
  Nlm_SaveNamedMuskStyle ( name, cpCur, cpDefCur );
  for (i=0; i<MSM_TOTAL_SUBCLASSES; i++){
    if ( cpCur[i].str != NULL ) MemFree (cpCur[i].str);
    if ( cpDefCur[i].str != NULL ) MemFree (cpDefCur[i].str);
  }
  MemFree ( cpCur );
  MemFree ( cpDefCur );
  sprintf ( buf, "STYLE%d", totalStyles );
  SetAppParam ( fileBaseName, styleListName, buf, name );
  totalStyles++;
  Nlm_SetIntAppParam ( fileBaseName, styleListName, styleTotalName, 
                       totalStyles);
  return totalStyles-1;
} /* End of Nlm_CopyMuskStyle () */

/**************************************************************************
* Nlm_DeleteMuskStyle()
*   delete style
**************************************************************************/
Boolean
/*FCN*/Nlm_DeleteMuskStyle(
  Nlm_Int2 styleNum                    /* style number (I) */
){
  Nlm_MsmParamsPtr cpDefCur;
  Nlm_MsmMemberPtr mPtr, mPtrD;
  CharPtr          name;
  Int2             i;
  Char             buf[32];

  if ( !msmInit ) Nlm_InitMuskStyles ();
  if ( (styleNum <= 0) || (styleNum == msmCurrent) ) return FALSE;
  name = Nlm_GetMuskStyleName ( styleNum );
  if ( name == NULL ) return FALSE;
  cpDefCur = MemNew ( MSM_TOTAL_SUBCLASSES * sizeof(Nlm_MsmParams) );
  Nlm_LoadDefMuskStyle( cpDefCur );
  Nlm_SaveNamedMuskStyle ( name, cpDefCur, cpDefCur );
  SetAppParam ( fileBaseName, name, NULL, NULL );
  for ( i = styleNum; i<totalStyles; i++ ){
    name = Nlm_GetMuskStyleName ( i+1 );
    sprintf ( buf, "STYLE%d", i );
    SetAppParam ( fileBaseName, styleListName, buf, name );
  }
  if ( msmCurrent > styleNum ) msmCurrent--;
  totalStyles--;
  Nlm_SetIntAppParam ( fileBaseName, styleListName, styleTotalName, 
                       totalStyles);
  mPtr = &(allStyles);
  for ( i=1; i<styleNum; i++ ){
    mPtr = mPtr->next;
  }
  mPtrD = mPtr->next;
  mPtr->next = mPtrD->next;
  MemFree ( mPtrD );
  MemFree ( cpDefCur );
  return TRUE;
} /* End of Nlm_DeleteMuskStyle() */

/**************************************************************************
* Nlm_EditMuskStyle()
*   edit the style
**************************************************************************/
Boolean 
/*FCN*/Nlm_EditMuskStyle (
  Nlm_Int2 styleNum                    /* style number (I) */
){
  CharPtr name;

  if ( !msmInit ) Nlm_InitMuskStyles ();
  if ( (styleNum <= 0) || (cpEdit != NULL) ) return FALSE;
  name = Nlm_GetMuskStyleName ( styleNum );
  if ( name == NULL ) return FALSE;
  msmEdit = styleNum;
  cpEdit = MemNew ( MSM_TOTAL_SUBCLASSES_R * sizeof(Nlm_MsmParams) );
  Nlm_LoadDefMuskStyle( cpEdit );
  Nlm_LoadNamedMuskStyle ( name, cpEdit );
  return TRUE;
}  /* End of Nlm_EditMuskStyle() */

/**************************************************************************
* Nlm_Nlm_SetMuskCParamEd()
*   set config parameter
**************************************************************************/
void 
/*FCN*/Nlm_SetMuskCParamEd ( 
  Nlm_Int2  p_class,                   /* class (I) */
  Nlm_Int2  p_subclass,                /* subclass (I) */
  Nlm_Int2  p_type,                    /* type (I) */
  BigScalar value                      /* value (I) */
){
  Nlm_Int2 index;

  if ( (p_class > -MSM_TOTAL_NOTPOS) && (p_class <= MSM_TOTAL_POSITIVE_R) ){
    index = (p_class + MSM_INDEX_OFFSET)*MSM_SUBCLASSES_NUM;
  } else return;
  if ( (p_subclass >= 0) && (p_subclass < MSM_SUBCLASSES_NUM) ){
    index += p_subclass;
  } else return;
  switch ( p_type ){
    case MSM_TRUEFALSE:
      cpEdit[index].truefalse = (Nlm_Boolean)value; break;
    case MSM_COLOR:
      cpEdit[index].color = (Nlm_Int4)value; break;
    case MSM_LTYPE:
      cpEdit[index].ltype = (Nlm_Int1)value; break;
    case MSM_SHADING:
      cpEdit[index].shading = (Nlm_Int1)value; break;
    case MSM_PENWIDTH:
      cpEdit[index].penwidth = (Nlm_Int1)value; break;
    case MSM_NUM:
      cpEdit[index].num = (Nlm_Int2)value; break;
    case MSM_STYLE:
      cpEdit[index].style = (Nlm_Int4)value; break;
    case MSM_FONT:
      cpEdit[index].font = (Nlm_FonT)value; break;
    case MSM_HEIGHT:
      cpEdit[index].height = (Nlm_Int4)value; break;
    case MSM_STRING:
      if ( cpEdit[index].str != NULL ) MemFree(cpEdit[index].str);
      cpEdit[index].str = NULL;
      if ( (Nlm_CharPtr)value != NULL ) {
        cpEdit[index].str = StringSave((Nlm_CharPtr)value);
      }
      break;
    case MSM_SCALE:
      cpEdit[index].scale = (Nlm_Int4)value; break;
  }
} /* End of Nlm_SetMuskCParam() */

/**************************************************************************
* Nlm_GetMuskCParamEd()
*   return configuration parameter
**************************************************************************/
BigScalar 
/*FCN*/Nlm_GetMuskCParamEd ( 
  Int2 p_class,                        /* class (I) */
  Int2 p_subclass,                     /* subclass (I) */
  Int2 p_type                          /* type (I) */
){
  return Nlm_GetMuskCP(cpEdit, p_class, p_subclass, p_type );
} /* End of GetMuskCParamEd() */

/**************************************************************************
* Nlm_FreeMuskStyleEd()
*   end of edit session
**************************************************************************/
void
/*FCN*/Nlm_FreeMuskStyleEd (
  Boolean save
){
  Nlm_MsmParamsPtr cpDefCur;
  Nlm_Int2         i;

  if ( cpEdit == NULL ) return;
  Nlm_SaveFonts();
  if ( save ) {
    cpDefCur = MemNew ( MSM_TOTAL_SUBCLASSES * sizeof(Nlm_MsmParams) );
    Nlm_LoadDefMuskStyle( cpDefCur );
    Nlm_SaveNamedMuskStyle ( Nlm_GetMuskStyleName(msmEdit), cpEdit, cpDefCur );
    if ( msmEdit == msmCurrent ){
      for (i=0; i<MSM_TOTAL_SUBCLASSES; i++){
        if ( cpStatic[i].str != NULL ) MemFree (cpStatic[i].str);
      }
      MemCopy ( &(cpStatic[0]), cpEdit, 
                MSM_TOTAL_SUBCLASSES * sizeof(Nlm_MsmParams) );
      for (i=0; i<MSM_TOTAL_SUBCLASSES_R; i++){
        cpEdit[i].str = NULL;
      }
    }
    for (i=0; i<MSM_TOTAL_SUBCLASSES; i++){
      if ( cpDefCur[i].str != NULL ) MemFree (cpDefCur[i].str);
    }
    MemFree ( cpDefCur );
  }
  for (i=0; i<MSM_TOTAL_SUBCLASSES_R; i++){
    if ( cpEdit[i].str != NULL ) MemFree (cpEdit[i].str);
  }
  MemFree ( cpEdit );
  cpEdit = NULL;
  msmEdit = -1;
} /*End of Nlm_FreeMuskStyleEd () */

/* END */
