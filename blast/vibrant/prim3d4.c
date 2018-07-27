/*   prim3d4.c
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
* File Name:  prim3d4.c
*
* Author:  Alex Smirnov
*
* Version Creation Date:   04/03/95
*
* $Revision: 6.3 $
*
* File Description: 
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* ==========================================================================
*/

#ifndef _VIBRANT_
#include <vibrant.h>
#endif

#ifndef _PPICT3D_
#include <ppict3d.h>
#endif

#ifndef _PDIAGNOS_
#include <pdiagnos.h>
#endif

/*****************************************************************************
*
*   DEFINES
*
*****************************************************************************/
#define SETPIXEL(zptr,imageptr,z,color) \
if((Uint2)z>=*((Uint2Ptr)zptr)) \
{*((Uint2Ptr)zptr)=(Uint2)z;*((Uint1Ptr)imageptr)=(Uint1)color;}

#define SETPIXELID(zptr,imageptr,z,curID) \
if((Uint2)z>=*((Uint2Ptr)zptr)) \
{*((Uint2Ptr)zptr)=(Uint2)z;*((VoidPtr*)imageptr)=curID;}

/*****************************************************************************
*
*   TYPEDEFS
*
*****************************************************************************/

/*****************************************************************************
*
*   GLOBAL VARIABLE
*
*****************************************************************************/
extern Nlm_Context3D Nlm_stCon;

/*****************************************************************************
*
*   STATIC VARIABLE
*
*****************************************************************************/
static CharPtr Pict3Dclass = "Picture3D";
static Char *vectFont[95] = {
  /*  32 ' ' */  "",
  /*  33 '!' */  "ChdhdicichCkdkewbwck",
  /*  34 '"' */  "CvbvbwcwcubtFvevewfwfuet",
  /*  35 '#' */  "AlmlAqmqDidtJijt",
  /*  36 '$' */  "AjcihijjkljnhodobparbtduiuktFgfw",
  /*  37 '%' */  "AgmwIhlhmimllmimhlhiihCqfqgrgufvcvbubrcq",
  /*  38 '&' */  "Kgihbsbtcvewfwgugsfqboamakbichegfghhkk",
  /*  39 ''' */  "Evdvdweweucs",
  /*  40 '(' */  "Egchbiakasbucvew",
  /*  41 ')' */  "Agchdiekesducvaw",
  /*  42 '*' */  "AplpCkjuCujk",
  /*  43 '+' */  "CokoGkgs",
  /*  44 ',' */  "Dgcgchdhdgdfce",
  /*  45 '-' */  "Coko",
  /*  46 '.' */  "Ggfgfhghgg",
  /*  47 '/' */  "Agmw",
  /*  48 '0' */  "EwcvbtbjchegigkhljltkviwewChkv",
  /*  49 '1' */  "DtgwggDgjg",
  /*  50 '2' */  "Kgagaibkhojpkrktjvhwdwbvat",
  /*  51 '3' */  "AtbvdwhwjvktkrjphoeoHojnklkjjhhgdgbhaj",
  /*  52 '4' */  "Klalhwhg",
  /*  53 '5' */  "Kwawaocphpjokmkjjhhgcgah",
  /*  54 '6' */  "Jvhwdwbvatajbhdghgjhkjkmjohpdpboan",
  /*  55 '7' */  "Awkwdg",
  /*  56 '8' */  "DwbvatarbpdohojpkrktjvhwdwDobnalajbhdghgjhkjkljnho",
  /*  57 '9' */  "Bhdghgjhkjktjvhwdwbvataqbodnhnjokq",
  /*  58 ':' */  "GififjgjgiGpfpfqgqgp",
  /*  59 ';' */  "DicicjdjdidhcgCpdpdqcqcp",
  /*  60 '<' */  "Lgbolw",
  /*  61 '=' */  "AsisAnjn",
  /*  62 '>' */  "Bglobw",
  /*  63 '?' */  "ArasbucvewgwivjuktkrjphognflfjFhfgggghfh",
  /*  64 '@' */  "Knjlhkflenepfrhsjrkqkkljmjnkomosnumvkwewcvbuasakbichegkg",
  /*  65 'A' */  "AgcmhwmmogCmmm",
  /*  66 'B' */  "AgigkhlimklmkniokplqmslukviwawBwbgboio",
  /*  67 'C' */  "Milhjgegchbiakasbucvewjwlvmu",
  /*  68 'D' */  "AgigkhlimkmslukviwawBwbg",
  /*  69 'E' */  "KwawagkgAoho",
  /*  70 'F' */  "KwawagAoho",
  /*  71 'G' */  "MilhjgegchbiakasbucvewjwlvmuMgmmHmom",
  /*  72 'H' */  "AgawKgkwAoko",
  /*  73 'I' */  "AgggAwgwDgdw",
  /*  74 'J' */  "AmakbichegghhiikiwDwnw",
  /*  75 'K' */  "AgawAmkwCokg",
  /*  76 'L' */  "Kgagaw",
  /*  77 'M' */  "Agawhmowog",
  /*  78 'N' */  "Agawmgmw",
  /*  79 'O' */  "Akbichegigkhlimkmslukviwewcvbuasak",
  /*  80 'P' */  "Agawiwkvlumsmrlpkoinan",
  /*  81 'Q' */  "AkbichegigkhlimkmslukviwewcvbuasakHlmg",
  /*  82 'R' */  "AgawiwkvlumsmrlpkoinanInmg",
  /*  83 'S' */  "Ajchegigkhlimklmkncpbqasbucvewiwkvmt",
  /*  84 'T' */  "HghwAwow",
  /*  85 'U' */  "Awakbichegigkhlimkmw",
  /*  86 'V' */  "Awggmw",
  /*  87 'W' */  "Aweghokgow",
  /*  88 'X' */  "AwkgAgkw",
  /*  89 'Y' */  "AwfokwFgfo",
  /*  90 'Z' */  "Awkwagkg",
  /*  91 '[' */  "Igdgdwiw",
  /*  92 '\' */  "Awmg",
  /*  93 ']' */  "Dgigiwdw",
  /*  94 '^' */  "Cqgwkq",
  /*  95 '_' */  "Bglg",
  /*  96 '`' */  "Cvdvdwcwcues",
  /*  97 'a' */  "BqdrirjqkokhlgKhigdgbhajbldmimkk",
  /*  98 'b' */  "BichegggihjikkkmjoipgqeqcpboAwbwbgag",
  /*  99 'c' */  "Jqhrercqbpanakbicheghgjh",
  /* 100 'd' */  "KiggegchbiakanbpcqerhrjqkoLgkgkwlw",
  /* 101 'e' */  "Kiihggegchbiakanbpcqerhrjqkokmam",
  /* 102 'f' */  "BgdgCgcrdtfuhtirAnfn",
  /* 103 'g' */  "KiihggegchbiakambodphpjokmLpkpkdjbhaeacbbc",
  /* 104 'h' */  "AwbwbgagBncpeqhqjpknkg",
  /* 105 'i' */  "AgegCgcpbpCrcsbsbrcr",
  /* 106 'j' */  "GshshrgrgsGphphegcebdbbcae",
  /* 107 'k' */  "AgbgbwawBjhpIgem",
  /* 108 'l' */  "AgegCgcubu",
  /* 109 'm' */  "AqbqbgBncpeqfqhpiniginjplqmqoppnpg",
  /* 110 'n' */  "AqbqbgagBncpeqhqjpknkg",
  /* 111 'o' */  "Akbichegggihjikkkmjoipgqeqcpboamak",
  /* 112 'p' */  "BichegggihjikkkmjoipgqeqcpboAqbqbaaa",
  /* 113 'q' */  "KiggegchbiakanbpcqerhrjqkoLpkpkdlbna",
  /* 114 'r' */  "AqbqbgBncpeqhqjo",
  /* 115 's' */  "Ahcgggihjjikbmanbpdqhqjp",
  /* 116 't' */  "AqiqEuehfghgih",
  /* 117 'u' */  "AqakbichegggihjikkKqkglg",
  /* 118 'v' */  "Arfgkr",
  /* 119 'w' */  "Areghmkgor",
  /* 120 'x' */  "ArkgAgkr",
  /* 121 'y' */  "AqakbichegggihjikkKqkdjbhaeacbbc",
  /* 122 'z' */  "Arkragkg",
  /* 123 '{' */  "Egchbjblcnaocpbrbtcvew",
  /* 124 '|' */  "Gggw",
  /* 125 '}' */  "Agchdjdlcneocpdrdtcvaw",
  /* 126 '~' */  "Arbtcueuftfsgrirjsku"
};

/*****************************************************************************
*
*   FUNCTIONS
*
*****************************************************************************/
static void Nlm_Empty3D ( Nlm_VoidPtr p )
{
}

/*****************************************************************************
*
*   TEXT
*
*****************************************************************************/
typedef struct Nlm_text3d {
  Nlm_Base3D  base;
  long        x,y,z;
  long        height;
  long        offset;
  Int2        flags;
  Int2        len;
  Char        text[1];
} Nlm_Text3D, PNTR Nlm_Text3DPtr;

static void Nlm_Text3DDraw ( Nlm_Text3DPtr p )
{
  register long reg1;
  register long reg2;
  register long reg3;
  register long reg4;
  long          x1,x2 = 0;
  long          y1,y2 = 0;
  long          dx, dy;
  long          xx1,xx2;
  long          yy1,yy2;
  long          status2;
  long          orient;
  long          endPtr;
  long          colorScale;
  long          color;
  long          offset1;
  long          offset2;
  long          ystep;
  long          zystep;
  long          x,y,z;
  long          height, width, offset;
  long          needCheck;
  CharPtr       ptr;
  CharPtr       lPtr;

  reg1 = (long)p;

  /* Rotate and check */
  reg2 = ((Nlm_Text3DPtr)reg1)->x;
  reg3 = ((Nlm_Text3DPtr)reg1)->y;
  reg4 = ((Nlm_Text3DPtr)reg1)->z;
  x = reg2 / Nlm_stCon.a[0][0] + reg3 / Nlm_stCon.a[0][1] +
      reg4 / Nlm_stCon.a[0][2] + Nlm_stCon.c[0];
  y = reg2 / Nlm_stCon.a[1][0] + reg3 / Nlm_stCon.a[1][1] +
      reg4 / Nlm_stCon.a[1][2] + Nlm_stCon.c[1];
  reg4 = reg2 / Nlm_stCon.a[2][0] + reg3 / Nlm_stCon.a[2][1] +
      reg4 / Nlm_stCon.a[2][2] + Nlm_stCon.c[2];
  z = reg4;
  reg4 = Nlm_stCon.zmaxPersp1-reg4;
  reg2 = ((Nlm_Text3DPtr)reg1)->height / Nlm_stCon.scale;
  reg2 = (reg2*Nlm_stCon.zmaxPersp)/reg4;
  height = reg2;
  width  = (reg2*15)/22;
  reg2 = ((Nlm_Text3DPtr)reg1)->offset / Nlm_stCon.scale;
  reg2 = (reg2*Nlm_stCon.zmaxPersp)/reg4;
  offset = reg2;
  switch ( ((Nlm_Text3DPtr)reg1)->flags & 0x3 ){
    case TEXT3D_LEFT:
      x -= offset + ((Nlm_Text3DPtr)reg1)->len*width;
      break;
    case TEXT3D_RIGTH:
      x += offset;
      break;
    default:
      x -= ((Nlm_Text3DPtr)reg1)->len*width/2;
  } 
  switch ( ((Nlm_Text3DPtr)reg1)->flags & 0x30 ){
    case TEXT3D_UPPER:
      y += offset;
      break;
    case TEXT3D_LOWER:
      y -= offset + height;
      break;
    default:
      y -= height/2;
  } 
  reg2 = Nlm_stCon.width>>1;
  reg3 = x-reg2;
  x = reg2 + (reg3*Nlm_stCon.zmaxPersp)/reg4;
  reg2 = Nlm_stCon.height>>1;
  reg3 = y-reg2;
  y =  reg2 Y_PLUS (reg3*Nlm_stCon.zmaxPersp)/reg4;
  if ( ((Nlm_Text3DPtr)reg1)->flags & TEXT3D_FRONT ){
    z = Nlm_stCon.zmax-1;
  } else {
    z = (z*Nlm_stCon.zmaxPersp)/reg4;
  }
  needCheck = 0;
  reg2 = x;
#ifdef NEGYSCREEN
  reg3 = y;
#else
  reg3 = y-height;
#endif
  if ( (reg2 > Nlm_stCon.xmax) || (reg3 > Nlm_stCon.ymax) ) return;
  if ( (reg2 < Nlm_stCon.xmin) || (reg3 < Nlm_stCon.ymin) ) needCheck=1;
  reg2 += width*((Nlm_Text3DPtr)reg1)->len;
  reg3 += height;
  if ( (reg2 < Nlm_stCon.xmin) || (reg3 < Nlm_stCon.ymin) ) return;
  if ( (reg2 > Nlm_stCon.xmax) || (reg3 > Nlm_stCon.ymax) ) needCheck=1;

  ptr = ((Nlm_Text3DPtr)reg1)->text;
  /* Set color and colorscale */
  color = Nlm_stCon.colorOffset + 
    ((Nlm_Text3DPtr)reg1)->base.color * Nlm_stCon.colorStep + 
    (Nlm_stCon.colorStep >> 1);
  colorScale = Nlm_stCon.colorScale << 1;
  color += (z<<8)/colorScale;
  ystep = Nlm_stCon.width;
  zystep = ystep<<1;

  while ( *ptr ){
    if ( (*ptr < 32) || (*ptr > 126) ) continue;
    lPtr = vectFont[*ptr-32];
    while ( *lPtr ){
      if ( *lPtr < 'a' ){
        x2 = x + ((long)*lPtr++ - (long)'A')*height/22;
        y2 = y Y_PLUS ((long)*lPtr++ - (long)'a')*height/22;
        continue;
      }
      x1 = x2; y1=y2;
      x2 = x + ((long)*lPtr++ - (long)'a')*height/22;
      y2 = y Y_PLUS ((long)*lPtr++ - (long)'a')*height/22;
      if ( needCheck ){
        dx = x2-x1; dy = y2-y1;
        /* Calculate status1(reg1) and status2(reg2) */
        reg1 = 0;
        reg2 = 0;
        reg3 = x1;
        reg4 = Nlm_stCon.xmin;
        if ( reg3 < reg4 ) reg1 = 1;
        else if ( reg3 > Nlm_stCon.xmax ) reg1 = 2;
        reg3 = x2;
        if ( reg3 < reg4 ) reg2 = 1;
        else if ( reg3 > Nlm_stCon.xmax ) reg2 = 2;
        if ( (reg1 == reg2) && (reg1 != 0) ) continue;
        reg3 = y1;
        reg4 = Nlm_stCon.ymin;
        if ( reg3 < reg4 ) reg1 += 6;
        else if ( reg3 > Nlm_stCon.ymax ) reg1 += 3;
        reg3 = y2;
        if ( reg3 < reg4 ) reg2 += 6;
        else if ( reg3 > Nlm_stCon.ymax ) reg2 += 3;
        if ( (reg1>=6) && (reg2>=6) ) continue;
        if ( (reg1>=3) && (reg2>=3) && (reg1<6) && (reg2<6) ) continue;

        /* Save status2*/
        status2 = reg2;

        /* Calculate the first point (xx1,yy1)*/
        switch ( reg1 ) {
          case 0: 
            xx1 = x1; yy1=y1;
            break;
          case 1:
            reg1 = x1; reg2 = Nlm_stCon.xmin;
            reg4 = x2-reg1;
            reg3 = y1 + dy*(reg2-reg1)/reg4;
            yy1 = reg3;
            xx1 = reg2;
            break;
          case 2:
            reg1 = x1; reg2 = Nlm_stCon.xmax;
            reg4 = x2-reg1;
            reg3 = y1 + dy*(reg2-reg1)/reg4;
            yy1 = reg3;
            xx1 = reg2;
            break;
          case 3:
            reg1 = y1; reg2 = Nlm_stCon.ymax;
            reg4 = y2-reg1;
            reg3 = x1 + dx*(reg2-reg1)/reg4;
            xx1 = reg3;
            yy1 = reg2;
            break;
          case 4:
            reg1 = x1; reg2 = Nlm_stCon.xmin;
            reg4 = x2-reg1;
            reg3 = y1 + dy*(reg2-reg1)/reg4;
            yy1 = reg3;
            xx1 = reg2;
            if ( (reg3 < Nlm_stCon.ymin) || (reg3 > Nlm_stCon.ymax) ) {
              reg1 = y1; reg2 = Nlm_stCon.ymax;
              reg4 = y2-reg1;
              reg3 = x1 + dx*(reg2-reg1)/reg4;
              xx1 = reg3;
              yy1 = reg2;
            }
            break;
          case 5:
            reg1 = x1; reg2 = Nlm_stCon.xmax;
            reg4 = x2-reg1;
            reg3 = y1 + dy*(reg2-reg1)/reg4;
            yy1 = reg3;
            xx1 = reg2;
            if ( (reg3 < Nlm_stCon.ymin)||(reg3 > Nlm_stCon.ymax) ) {
              reg1 = y1; reg2 = Nlm_stCon.ymax;
              reg4 = y2-reg1;
              reg3 = x1 + dx*(reg2-reg1)/reg4;
              xx1 = reg3;
              yy1 = reg2;
            }
            break;
          case 6:
            reg1 = y1; reg2 = Nlm_stCon.ymin;
            reg4 = y2-reg1;
            reg3 = x1 + dx*(reg2-reg1)/reg4;
            xx1 = reg3;
            yy1 = reg2;
            break;
          case 7:
            reg1 = x1; reg2 = Nlm_stCon.xmin;
            reg4 = x2-reg1;
            reg3 = y1 + dy*(reg2-reg1)/reg4;
            yy1 = reg3;
            xx1 = reg2;
            if ( (reg3 < Nlm_stCon.ymin) || (reg3 > Nlm_stCon.ymax) ) {
              reg1 = y1; reg2 = Nlm_stCon.ymin;
              reg4 = y2-reg1;
              reg3 = x1 + dx*(reg2-reg1)/reg4;
              xx1 = reg3;
              yy1 = reg2;
            }
            break;
          default:
            reg1 = x1; reg2 = Nlm_stCon.xmax;
            reg4 = x2-reg1;
            reg3 = y1 + dy*(reg2-reg1)/reg4;
            yy1 = reg3;
            xx1 = reg2;
            if ( (reg3 < Nlm_stCon.ymin)||(reg3 > Nlm_stCon.ymax) ) {
              reg1 = y1; reg2 = Nlm_stCon.ymin;
              reg4 = y2-reg1;
              reg3 = x1 + dx*(reg2-reg1)/reg4;
              xx1 = reg3;
              yy1 = reg2;
            }
        }

        /* Calculate the second point (xx2,yy2)*/
        switch ( status2 ) {
          case 0: 
            xx2 = x2; yy2=y2;
            break;
          case 1:
            reg1 = x1; reg2 = Nlm_stCon.xmin;
            reg4 = x2-reg1;
            yy2 = y1 + dy*(reg2-reg1)/reg4;
            xx2 = reg2;
            break;
          case 2:
            reg1 = x1; reg2 = Nlm_stCon.xmax;
            reg4 = x2-reg1;
            yy2 = y1 + dy*(reg2-reg1)/reg4;
            xx2 = reg2;
            break;
          case 3:
            reg1 = y1; reg2 = Nlm_stCon.ymax;
            reg4 = y2-reg1;
            xx2 = x1 + dx*(reg2-reg1)/reg4;
            yy2 = reg2;
            break;
          case 4:
            reg1 = x1; reg2 = Nlm_stCon.xmin;
            reg4 = x2-reg1;
            reg3 = y1 + dy*(reg2-reg1)/reg4;
            yy2 = reg3;
            xx2 = reg2;
            if ( (reg3 < Nlm_stCon.ymin) || (reg3 > Nlm_stCon.ymax) ) {
              reg1 = y1; reg2 = Nlm_stCon.ymax;
              reg4 = y2-reg1;
              reg3 = x1 + dx*(reg2-reg1)/reg4;
              xx2 = reg3;
              yy2 = reg2;
            }
            break;
          case 5:
            reg1 = x1; reg2 = Nlm_stCon.xmax;
            reg4 = x2-reg1;
            reg3 = y1 + dy*(reg2-reg1)/reg4;
            yy2 = reg3;
            xx2 = reg2;
            if ( (reg3 < Nlm_stCon.ymin)||(reg3 > Nlm_stCon.ymax) ) {
              reg1 = y1; reg2 = Nlm_stCon.ymax;
              reg4 = y2-reg1;
              reg3 = x1 + dx*(reg2-reg1)/reg4;
              xx2 = reg3;
              yy2 = reg2;
            }
            break;
          case 6:
            reg1 = y1; reg2 = Nlm_stCon.ymin;
            reg4 = y2-reg1;
            xx2 = x1 + dx*(reg2-reg1)/reg4;
            yy2 = reg2;
            break;
          case 7:
            reg1 = x1; reg2 = Nlm_stCon.xmin;
            reg4 = x2-reg1;
            reg3 = y1 + dy*(reg2-reg1)/reg4;
            yy2 = reg3;
            xx2 = reg2;
            if ( (reg3 < Nlm_stCon.ymin) || (reg3 > Nlm_stCon.ymax) ) {
              reg1 = y1; reg2 = Nlm_stCon.ymin;
              reg4 = y2-reg1;
              reg3 = x1 + dx*(reg2-reg1)/reg4;
              xx2 = reg3;
              yy2 = reg2;
            }
            break;
          default:
            reg1 = x1; reg2 = Nlm_stCon.xmax;
            reg4 = x2-reg1;
            reg3 = y1 + dy*(reg2-reg1)/reg4;
            yy2 = reg3;
            xx2 = reg2;
            if ( (reg3 < Nlm_stCon.ymin)||(reg3 > Nlm_stCon.ymax) ) {
              reg1 = y1; reg2 = Nlm_stCon.ymin;
              reg4 = y2-reg1;
              reg3 = x1 + dx*(reg2-reg1)/reg4;
              xx2 = reg3;
              yy2 = reg2;
            }
        }
      } else {
        xx1 = x1; yy1=y1;
        xx2 = x2; yy2=y2;
      }

      reg3 = xx1;
      reg4 = xx2;
      if ( reg3 < reg4 ) {
        if ( (reg3 < Nlm_stCon.xmin)||(reg4 > Nlm_stCon.xmax) ) continue;
        reg1 = 0;
      } else {
        if ( (reg4 < Nlm_stCon.xmin)||(reg3 > Nlm_stCon.xmax) ) continue;
        reg1 = 1;
      }
      reg3 = yy1;
      reg4 = yy2;
      if ( reg3 < reg4 ) {
        if ( (reg3 < Nlm_stCon.ymin)||(reg4 > Nlm_stCon.ymax) ) continue;
      } else {
        if ( (reg4 < Nlm_stCon.ymin)||(reg3 > Nlm_stCon.ymax) ) continue;
        reg1 |= 0x2;
      }
      orient = reg1;

      /* Calculate offsets and pointers */
      reg1 = yy1 * Nlm_stCon.width + xx1;
      offset1 = reg1;
      reg3 = (long)(Nlm_stCon.image + reg1);
      reg2 = yy2 * Nlm_stCon.width + xx2;
      offset2 = reg2;
      reg4 = (long)(Nlm_stCon.image + reg2);
      if ( reg3 == reg4 ){
        reg2 = (long)(Nlm_stCon.zBuffer + reg1);
        SETPIXEL ( reg2, reg3, z, color );
        continue;
      }
      /* reg3 - statrt reg4 - end */

      /* Will be:
         reg1 - last orientation
         reg3 - start
         reg4 - zPtr
         endPtr saved in endPtr */
      reg1 = orient;
      switch ( reg1 ){
        case 0:
          dy = yy2 - yy1;
          reg2 = xx2 - xx1;
          dx = reg2;
          if ( reg2<=dy ) {
            reg1 = 2;
          }
          endPtr = reg4;
          reg4 = (long)(Nlm_stCon.zBuffer + offset1);
          break;
        case 1:
          dy = yy2 - yy1;
          reg2 = xx1 - xx2;
          if ( reg2 <= dy ) {
            reg1 = 3;
          }
          dx = reg2;
          reg2 = reg3; reg3 = reg4; reg4 = reg2;
          endPtr = reg4;
          reg4 = (long)(Nlm_stCon.zBuffer + offset2);
          break;
        case 2:
          dy = yy1 - yy2;
          reg2 = xx2 - xx1;
          if ( reg2 > dy ){
            reg1 = 1;
          } else {
            reg1 = 3;
          }
          dx = reg2;
          endPtr = reg4;
          reg4 = (long)(Nlm_stCon.zBuffer + offset1);
          break;
        default:
          dy = yy1 - yy2;
          reg2 = xx1 - xx2;
          if ( reg2 > dy ){
            reg1 = 0;
          } else {
            reg1 = 2;
          }
          dx = reg2;
          reg2 = reg3; reg3 = reg4; reg4 = reg2;
          endPtr = reg4;
          reg4 = (long)(Nlm_stCon.zBuffer + offset2);
      }

      reg2 = z;
      /* Will be: reg1 - err */
      switch ( reg1 ){
        case 0:
          reg1 = -(dx>>1);
          do {
            reg1 += dy;
            if ( reg1 > 0 ){
               reg3 += ystep; 
               reg4 += zystep; 
               reg1 -= dx; 
            }
            reg3++; reg4++; reg4++;
            SETPIXEL(reg4,reg3,reg2,color);
          } while ( reg3 != endPtr );
          break;
        case 1:
          reg1 = -(dx>>1);
          do {
            reg1 += dy;
            if ( reg1 > 0 ){
               reg3 -= ystep; 
               reg4 -= zystep; 
               reg1 -= dx; 
            }
            reg3++; reg4++; reg4++;
            SETPIXEL(reg4,reg3,reg2,color);
          } while ( reg3 != endPtr );
          break;
        case 2:
          reg1 = -(dy>>1);
          do {
            reg1 += dx;
            if ( reg1 > 0 ) {
               reg3 ++;
               reg4 ++; reg4++;
               reg1 -= dy; 
            }
            reg3 += ystep; 
            reg4 += zystep; 
            SETPIXEL(reg4,reg3,reg2,color);
          } while ( reg3 != endPtr );
          break;
        default:
          reg1 = -(dy>>1);
          do {
            reg1 += dx;
            if ( reg1 > 0 ) {
               reg3 ++;
               reg4 ++; reg4++;
               reg1 -= dy; 
            }
            reg3 -= ystep; 
            reg4 -= zystep; 
            SETPIXEL(reg4,reg3,reg2,color);
          } while ( reg3 != endPtr );
      }
    }
    ptr++;
    x += width;
  }
}

static void Nlm_Text3DHitT ( Nlm_Text3DPtr p )
{
  register long reg1;
  register long reg2;
  register long reg3;
  register long reg4;
  long          x1,x2 = 0;
  long          y1,y2 = 0;
  long          z1,z2;
  long          dx, dy, dz;
  long          xx1,xx2;
  long          yy1,yy2;
  long          zz1,zz2;
  long          status2;
  long          orient;
  long          endPtr;
  long          offset1;
  long          offset2;
  long          zrate;
  long          ystep;
  long          zystep;
  long          curz;
  long          x,y,z;
  long          height, width, offset;
  long          needCheck;
  CharPtr       ptr;
  CharPtr       lPtr;

  reg1 = (long)p;

  /* Rotate and check */
  reg2 = ((Nlm_Text3DPtr)reg1)->x;
  reg3 = ((Nlm_Text3DPtr)reg1)->y;
  reg4 = ((Nlm_Text3DPtr)reg1)->z;
  x = reg2 / Nlm_stCon.a[0][0] + reg3 / Nlm_stCon.a[0][1] +
      reg4 / Nlm_stCon.a[0][2] + Nlm_stCon.c[0];
  y = reg2 / Nlm_stCon.a[1][0] + reg3 / Nlm_stCon.a[1][1] +
      reg4 / Nlm_stCon.a[1][2] + Nlm_stCon.c[1];
  reg4 = reg2 / Nlm_stCon.a[2][0] + reg3 / Nlm_stCon.a[2][1] +
      reg4 / Nlm_stCon.a[2][2] + Nlm_stCon.c[2];
  z = reg4;
  reg4 = Nlm_stCon.zmaxPersp1-reg4;
  reg2 = ((Nlm_Text3DPtr)reg1)->height / Nlm_stCon.scale;
  reg2 = (reg2*Nlm_stCon.zmaxPersp)/reg4;
  height = reg2;
  width  = (reg2*15)/22;
  reg2 = ((Nlm_Text3DPtr)reg1)->offset / Nlm_stCon.scale;
  reg2 = (reg2*Nlm_stCon.zmaxPersp)/reg4;
  offset = reg2;
  switch ( ((Nlm_Text3DPtr)reg1)->flags & 0x3 ){
    case TEXT3D_LEFT:
      x -= offset + ((Nlm_Text3DPtr)reg1)->len*width;
      break;
    case TEXT3D_RIGTH:
      x += offset;
      break;
    default:
      x -= ((Nlm_Text3DPtr)reg1)->len*width/2;
  } 
  switch ( ((Nlm_Text3DPtr)reg1)->flags & 0x30 ){
    case TEXT3D_UPPER:
      y += offset;
      break;
    case TEXT3D_LOWER:
      y -= offset + height;
      break;
    default:
      y -= height/2;
  } 
  reg2 = Nlm_stCon.width>>1;
  reg3 = x-reg2;
  x = reg2 + (reg3*Nlm_stCon.zmaxPersp)/reg4;
  reg2 = Nlm_stCon.height>>1;
  reg3 = y-reg2;
  y =  reg2 Y_PLUS (reg3*Nlm_stCon.zmaxPersp)/reg4;
  if ( ((Nlm_Text3DPtr)reg1)->flags & TEXT3D_FRONT ){
    z = Nlm_stCon.zmax-1;
  }else{
    z = (z*Nlm_stCon.zmaxPersp)/reg4;
  }
  needCheck = 0;
  reg2 = x;
  reg3 = y;
  if ( (reg2 > Nlm_stCon.xmax) || (reg3 > Nlm_stCon.ymax) ) return;
  if ( (reg2 < Nlm_stCon.xmin) || (reg3 < Nlm_stCon.ymin) ) needCheck=1;
  reg2 += width*((Nlm_Text3DPtr)reg1)->len;
  reg3 += height;
  if ( (reg2 < Nlm_stCon.xmin) || (reg3 < Nlm_stCon.ymin) ) return;
  if ( (reg2 > Nlm_stCon.xmax) || (reg3 > Nlm_stCon.ymax) ) needCheck=1;

  ptr = ((Nlm_Text3DPtr)reg1)->text;
  z1 = z2 = z;
  ystep  = Nlm_stCon.widthCur * sizeof(Nlm_stCon.idBuffer[0]);
  zystep = Nlm_stCon.widthCur * sizeof(Nlm_stCon.zBuffer[0]);

  while ( *ptr ){
    if ( (*ptr < 32) || (*ptr > 126) ) continue;
    lPtr = vectFont[*ptr-32];
    while ( *lPtr ){
      if ( *lPtr < 'a' ){
        x2 = x + ((long)*lPtr++ - (long)'A')*height/22;
        y2 = y Y_PLUS ((long)*lPtr++ - (long)'a')*height/22;
        continue;
      }
      x1 = x2; y1=y2;
      x2 = x + ((long)*lPtr++ - (long)'a')*height/22;
      y2 = y Y_PLUS ((long)*lPtr++ - (long)'a')*height/22;
      if ( needCheck ){
        dx = x2-x1; dy = y2-y1; dz = z2-z1;
        /* Calculate status1(reg1) and status2(reg2) */
        reg1 = 0;
        reg2 = 0;
        reg3 = x1;
        reg4 = Nlm_stCon.xmin;
        if ( reg3 < reg4 ) reg1 = 1;
        else if ( reg3 > Nlm_stCon.xmax ) reg1 = 2;
        reg3 = x2;
        if ( reg3 < reg4 ) reg2 = 1;
        else if ( reg3 > Nlm_stCon.xmax ) reg2 = 2;
        if ( (reg1 == reg2) && (reg1 != 0) ) continue;
        reg3 = y1;
        reg4 = Nlm_stCon.ymin;
        if ( reg3 < reg4 ) reg1 += 6;
        else if ( reg3 > Nlm_stCon.ymax ) reg1 += 3;
        reg3 = y2;
        if ( reg3 < reg4 ) reg2 += 6;
        else if ( reg3 > Nlm_stCon.ymax ) reg2 += 3;
        if ( (reg1>=6) && (reg2>=6) ) continue;
        if ( (reg1>=3) && (reg2>=3) && (reg1<6) && (reg2<6) ) continue;

        /* Save status2*/
        status2 = reg2;

        /* Calculate the first point (xx1,yy1,zz1)*/
        switch ( reg1 ) {
          case 0: 
            xx1 = x1; yy1=y1; zz1=z1;
            break;
          case 1:
            reg1 = x1; reg2 = Nlm_stCon.xmin;
            reg4 = x2-reg1;
            reg3 = y1 + dy*(reg2-reg1)/reg4;
            yy1 = reg3;
            xx1 = reg2;
            zz1 = z1 + dz*(reg2-reg1)/reg4;
            break;
          case 2:
            reg1 = x1; reg2 = Nlm_stCon.xmax;
            reg4 = x2-reg1;
            reg3 = y1 + dy*(reg2-reg1)/reg4;
            yy1 = reg3;
            xx1 = reg2;
            zz1 = z1 + dz*(reg2-reg1)/reg4;
            break;
          case 3:
            reg1 = y1; reg2 = Nlm_stCon.ymax;
            reg4 = y2-reg1;
            reg3 = x1 + dx*(reg2-reg1)/reg4;
            xx1 = reg3;
            yy1 = reg2;
            zz1 = z1 + dz*(reg2-reg1)/reg4;
            break;
          case 4:
            reg1 = x1; reg2 = Nlm_stCon.xmin;
            reg4 = x2-reg1;
            reg3 = y1 + dy*(reg2-reg1)/reg4;
            yy1 = reg3;
            xx1 = reg2;
            if ( (reg3 < Nlm_stCon.ymin) || (reg3 > Nlm_stCon.ymax) ) {
              reg1 = y1; reg2 = Nlm_stCon.ymax;
              reg4 = y2-reg1;
              reg3 = x1 + dx*(reg2-reg1)/reg4;
              xx1 = reg3;
              yy1 = reg2;
            }
            zz1 = z1 + dz*(reg2-reg1)/reg4;
            break;
          case 5:
            reg1 = x1; reg2 = Nlm_stCon.xmax;
            reg4 = x2-reg1;
            reg3 = y1 + dy*(reg2-reg1)/reg4;
            yy1 = reg3;
            xx1 = reg2;
            if ( (reg3 < Nlm_stCon.ymin)||(reg3 > Nlm_stCon.ymax) ) {
              reg1 = y1; reg2 = Nlm_stCon.ymax;
              reg4 = y2-reg1;
              reg3 = x1 + dx*(reg2-reg1)/reg4;
              xx1 = reg3;
              yy1 = reg2;
            }
            zz1 = z1 + dz*(reg2-reg1)/reg4;
            break;
          case 6:
            reg1 = y1; reg2 = Nlm_stCon.ymin;
            reg4 = y2-reg1;
            reg3 = x1 + dx*(reg2-reg1)/reg4;
            xx1 = reg3;
            yy1 = reg2;
            zz1 = z1 + dz*(reg2-reg1)/reg4;
            break;
          case 7:
            reg1 = x1; reg2 = Nlm_stCon.xmin;
            reg4 = x2-reg1;
            reg3 = y1 + dy*(reg2-reg1)/reg4;
            yy1 = reg3;
            xx1 = reg2;
            if ( (reg3 < Nlm_stCon.ymin) || (reg3 > Nlm_stCon.ymax) ) {
              reg1 = y1; reg2 = Nlm_stCon.ymin;
              reg4 = y2-reg1;
              reg3 = x1 + dx*(reg2-reg1)/reg4;
              xx1 = reg3;
              yy1 = reg2;
            }
            zz1 = z1 + dz*(reg2-reg1)/reg4;
            break;
          default:
            reg1 = x1; reg2 = Nlm_stCon.xmax;
            reg4 = x2-reg1;
            reg3 = y1 + dy*(reg2-reg1)/reg4;
            yy1 = reg3;
            xx1 = reg2;
            if ( (reg3 < Nlm_stCon.ymin)||(reg3 > Nlm_stCon.ymax) ) {
              reg1 = y1; reg2 = Nlm_stCon.ymin;
              reg4 = y2-reg1;
              reg3 = x1 + dx*(reg2-reg1)/reg4;
              xx1 = reg3;
              yy1 = reg2;
            }
            zz1 = z1 + dz*(reg2-reg1)/reg4;
        }

        /* Calculate the second point (xx2,yy2,zz2)*/
        switch ( status2 ) {
          case 0: 
            xx2 = x2; yy2=y2; zz2=z2;
            break;
          case 1:
            reg1 = x1; reg2 = Nlm_stCon.xmin;
            reg4 = x2-reg1;
            yy2 = y1 + dy*(reg2-reg1)/reg4;
            xx2 = reg2;
            zz2 = z1 + dz*(reg2-reg1)/reg4;
            break;
          case 2:
            reg1 = x1; reg2 = Nlm_stCon.xmax;
            reg4 = x2-reg1;
            yy2 = y1 + dy*(reg2-reg1)/reg4;
            xx2 = reg2;
            zz2 = z1 + dz*(reg2-reg1)/reg4;
            break;
          case 3:
            reg1 = y1; reg2 = Nlm_stCon.ymax;
            reg4 = y2-reg1;
            xx2 = x1 + dx*(reg2-reg1)/reg4;
            yy2 = reg2;
            zz2 = z1 + dz*(reg2-reg1)/reg4;
            break;
          case 4:
            reg1 = x1; reg2 = Nlm_stCon.xmin;
            reg4 = x2-reg1;
            reg3 = y1 + dy*(reg2-reg1)/reg4;
            yy2 = reg3;
            xx2 = reg2;
            if ( (reg3 < Nlm_stCon.ymin) || (reg3 > Nlm_stCon.ymax) ) {
              reg1 = y1; reg2 = Nlm_stCon.ymax;
              reg4 = y2-reg1;
              reg3 = x1 + dx*(reg2-reg1)/reg4;
              xx2 = reg3;
              yy2 = reg2;
            }
            zz2 = z1 + dz*(reg2-reg1)/reg4;
            break;
          case 5:
            reg1 = x1; reg2 = Nlm_stCon.xmax;
            reg4 = x2-reg1;
            reg3 = y1 + dy*(reg2-reg1)/reg4;
            yy2 = reg3;
            xx2 = reg2;
            if ( (reg3 < Nlm_stCon.ymin)||(reg3 > Nlm_stCon.ymax) ) {
              reg1 = y1; reg2 = Nlm_stCon.ymax;
              reg4 = y2-reg1;
              reg3 = x1 + dx*(reg2-reg1)/reg4;
              xx2 = reg3;
              yy2 = reg2;
            }
            zz2 = z1 + dz*(reg2-reg1)/reg4;
            break;
          case 6:
            reg1 = y1; reg2 = Nlm_stCon.ymin;
            reg4 = y2-reg1;
            xx2 = x1 + dx*(reg2-reg1)/reg4;
            yy2 = reg2;
            zz2 = z1 + dz*(reg2-reg1)/reg4;
            break;
          case 7:
            reg1 = x1; reg2 = Nlm_stCon.xmin;
            reg4 = x2-reg1;
            reg3 = y1 + dy*(reg2-reg1)/reg4;
            yy2 = reg3;
            xx2 = reg2;
            if ( (reg3 < Nlm_stCon.ymin) || (reg3 > Nlm_stCon.ymax) ) {
              reg1 = y1; reg2 = Nlm_stCon.ymin;
              reg4 = y2-reg1;
              reg3 = x1 + dx*(reg2-reg1)/reg4;
              xx2 = reg3;
              yy2 = reg2;
            }
            zz2 = z1 + dz*(reg2-reg1)/reg4;
            break;
          default:
            reg1 = x1; reg2 = Nlm_stCon.xmax;
            reg4 = x2-reg1;
            reg3 = y1 + dy*(reg2-reg1)/reg4;
            yy2 = reg3;
            xx2 = reg2;
            if ( (reg3 < Nlm_stCon.ymin)||(reg3 > Nlm_stCon.ymax) ) {
              reg1 = y1; reg2 = Nlm_stCon.ymin;
              reg4 = y2-reg1;
              reg3 = x1 + dx*(reg2-reg1)/reg4;
              xx2 = reg3;
              yy2 = reg2;
            }
            zz2 = z1 + dz*(reg2-reg1)/reg4;
        }
      } else {
        xx1 = x1; yy1=y1; zz1=z1;
        xx2 = x2; yy2=y2; zz2=z2;
      }

      reg3 = xx1;
      reg4 = xx2;
      if ( reg3 < reg4 ) {
        if ( (reg3 < Nlm_stCon.xmin)||(reg4 > Nlm_stCon.xmax) ) continue;
        reg1 = 0;
      } else {
        if ( (reg4 < Nlm_stCon.xmin)||(reg3 > Nlm_stCon.xmax) ) continue;
        reg1 = 1;
      }
      reg3 = yy1;
      reg4 = yy2;
      if ( reg3 < reg4 ) {
        if ( (reg3 < Nlm_stCon.ymin)||(reg4 > Nlm_stCon.ymax) ) continue;
      } else {
        if ( (reg4 < Nlm_stCon.ymin)||(reg3 > Nlm_stCon.ymax) ) continue;
        reg1 |= 0x2;
      }
      orient = reg1;

      /* Calculate offsets */
      reg3 = Nlm_stCon.xmin;
      xx1 -= reg3;  xx2 -= reg3;
      reg3 = Nlm_stCon.ymin;
      yy1 -= reg3; yy2 -=reg3;
      /* Calculate offsets and pointers */
      reg1 = yy1 * Nlm_stCon.widthCur + xx1;
      offset1 = reg1;
      reg3 = (long)(Nlm_stCon.idBuffer + reg1);
      reg2 = yy2 * Nlm_stCon.widthCur + xx2;
      offset2 = reg2;
      reg4 = (long)(Nlm_stCon.idBuffer + reg2);
      if ( reg3 == reg4 ){
        reg2 = (long)(Nlm_stCon.zBuffer + reg1);
        reg1 = zz1;
        if ( reg1 < zz2 ) reg1 = zz2;
        SETPIXELID ( reg2, reg3, reg1, p );
        continue;
      }
      /* reg3 - statrt reg4 - end */

      /* Will be:
         reg1 - last orientation
         reg2 - zz1
         reg3 - start
         reg4 - zPtr
         endPtr saved in endPtr */
      reg1 = orient;
      switch ( reg1 ){
        case 0:
          dy = yy2 - yy1;
          reg2 = xx2 - xx1;
          dx = reg2;
          if ( reg2>dy ) {
            zrate = ((zz2 - zz1)<<10)/reg2;
          } else {
            zrate = ((zz2 - zz1)<<10)/dy;
            reg1 = 2;
          }
          reg2 = zz1;
          endPtr = reg4;
          reg4 = (long)(Nlm_stCon.zBuffer + offset1);
          break;
        case 1:
          dy = yy2 - yy1;
          reg2 = xx1 - xx2;
          if ( reg2 > dy ) {
            zrate = ((zz1 - zz2)<<10)/reg2;
          } else {
            zrate = ((zz1 - zz2)<<10)/dy;
            reg1 = 3;
          }
          dx = reg2;
          reg2 = reg3; reg3 = reg4; reg4 = reg2;
          reg2 = zz2;
          endPtr = reg4;
          reg4 = (long)(Nlm_stCon.zBuffer + offset2);
          break;
        case 2:
          dy = yy1 - yy2;
          reg2 = xx2 - xx1;
          if ( reg2 > dy ){
            zrate = ((zz2 - zz1)<<10)/reg2;
            reg1 = 1;
          } else {
            zrate = ((zz2 - zz1)<<10)/dy;
            reg1 = 3;
          }
          dx = reg2;
          reg2 = zz1;
          endPtr = reg4;
          reg4 = (long)(Nlm_stCon.zBuffer + offset1);
          break;
        default:
          dy = yy1 - yy2;
          reg2 = xx1 - xx2;
          if ( reg2 > dy ){
            zrate = ((zz1 - zz2)<<10)/reg2;
            reg1 = 0;
          } else {
            zrate = ((zz1 - zz2)<<10)/dy;
            reg1 = 2;
          }
          dx = reg2;
          reg2 = reg3; reg3 = reg4; reg4 = reg2;
          reg2 = zz2;
          endPtr = reg4;
          reg4 = (long)(Nlm_stCon.zBuffer + offset2);
      }

      reg2 <<= 10;
      /* Will be: reg1 - err */
      switch ( reg1 ){
        case 0:
          reg1 = -(dx>>1);
          do {
            reg1 += dy;
            if ( reg1 > 0 ){
               reg3 += ystep; 
               reg4 += zystep; 
               reg1 -= dx; 
            }
            reg3 += sizeof(Nlm_stCon.idBuffer[0]); reg4++; reg4++;
            reg2 += zrate; 
            curz = reg2;
            reg2 >>= 10;
            SETPIXELID(reg4,reg3,reg2,p);
            reg2 = curz;
          } while ( reg3 != endPtr );
          break;
        case 1:
          reg1 = -(dx>>1);
          do {
            reg1 += dy;
            if ( reg1 > 0 ){
               reg3 -= ystep; 
               reg4 -= zystep; 
               reg1 -= dx; 
            }
            reg3 += sizeof(Nlm_stCon.idBuffer[0]); reg4++; reg4++;
            reg2 += zrate; 
            curz = reg2;
            reg2 >>= 10;
            SETPIXELID(reg4,reg3,reg2,p);
            reg2 = curz;
          } while ( reg3 != endPtr );
          break;
        case 2:
          reg1 = -(dy>>1);
          do {
            reg1 += dx;
            if ( reg1 > 0 ) {
               reg3 += sizeof(Nlm_stCon.idBuffer[0]);
               reg4++; reg4++;
               reg1 -= dy; 
            }
            reg3 += ystep; 
            reg4 += zystep; 
            reg2 += zrate; 
            curz = reg2;
            reg2 >>= 10;
            SETPIXELID(reg4,reg3,reg2,p);
            reg2 = curz;
          } while ( reg3 != endPtr );
          break;
        default:
          reg1 = -(dy>>1);
          do {
            reg1 += dx;
            if ( reg1 > 0 ) {
               reg3 += sizeof(Nlm_stCon.idBuffer[0]);
               reg4++; reg4++;
               reg1 -= dy; 
            }
            reg3 -= ystep; 
            reg4 -= zystep; 
            reg2 += zrate; 
            curz = reg2;
            reg2 >>= 10;
            SETPIXELID(reg4,reg3,reg2,p);
            reg2 = curz;
          } while ( reg3 != endPtr );
      }
    }
    ptr++;
    x += width;
  }
}

static void Nlm_Text3DGetL ( Nlm_Text3DPtr p )
{
  register Nlm_Text3DPtr ax;
  register Nlm_PSeg3DPtr bx;
  long                   minXY, maxXY;
  long                   rad;

  ax = p;
  bx = (Nlm_PSeg3DPtr)((Nlm_PSeg3DPtr)ax)->base.parent;
  rad = ax->offset + ax->len * ax->height *15 / 22;
  minXY = ax->x - rad;
  maxXY = ax->x + rad;
  if ( bx->segBox.minX > minXY )
       bx->segBox.minX = minXY;
  if ( bx->segBox.maxX < maxXY )
       bx->segBox.maxX = maxXY;
  minXY = ax->y - rad;
  maxXY = ax->y + rad;
  if ( bx->segBox.minY > minXY )
       bx->segBox.minY = minXY;
  if ( bx->segBox.maxY < maxXY )
       bx->segBox.maxY = maxXY;
  minXY = ax->z - rad;
  maxXY = ax->z + rad;
  if ( bx->segBox.minZ > minXY )
       bx->segBox.minZ = minXY;
  if ( bx->segBox.maxZ < maxXY )
       bx->segBox.maxZ = maxXY;
}

static Nlm_PrimDef3D textDef = {
  (Nlm_PrimDraw3D)Nlm_Text3DDraw,
  (Nlm_PrimHitTest3D)Nlm_Text3DHitT,
  (Nlm_PrimGetLimits3D)Nlm_Text3DGetL,
  (Nlm_PrimCleanup3D)Nlm_Empty3D,
  (Nlm_Int2)TEXT3D
};

/*****************************************************************************
*
*   TEXT
*
*****************************************************************************/
Nlm_Prim3D Nlm_AddText3D ( Nlm_Picture3D pic, Nlm_Segment3D segment, 
                           BigScalar userData, Uint1 layer, Uint1 color,
                           CharPtr string, Int4 x, Int4 y, Int4 z,
                           Uint4 height, Int4 offset, Int2 flags )
{
  Nlm_Text3DPtr l;
  Int2          len;

  Nlm_stCon.pic = pic;
  Nlm_DiagReset();
  if ( (flags & 0x3 ) == 0 ) flags |= 0x2;
  if ( (flags & 0x30) == 0 ) flags |= 0x20;
  len = StringLen( string );
  l = (Nlm_Text3DPtr) MemNew(sizeof(Nlm_Text3D) + len);
  if (l == NULL) {
    Nlm_DiagPutRecord(DA_ERROR, Pict3Dclass, "AddText3D",
                      "Memory allocation error" );
    return NULL;
  }
  l->base.userData = userData;
  l->base.layer    = layer;
  l->base.color    = color;
  l->base.fTable   = &textDef;
  l->base.status   = 0;
  l->x = (long)x;  l->y = (long)y;  l->z = (long)z;
  l->height = (long)height;
  l->offset = (long)offset;
  l->flags  = flags;
  l->len    = len;
  StringCpy(l->text, string);

  {{
    Nlm_Prim3D prim = (Nlm_Prim3D)Nlm_AddPrim3D((Nlm_PSeg3DPtr)segment, 
                                                (Nlm_Base3DPtr)l,
                                                sizeof(Nlm_Text3D) + len, 
                                                "AddText3D");
    MemFree( l );
    return prim;
  }}
}

