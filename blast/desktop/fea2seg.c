/*   fea2seg.c
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
* File Name:  fea2seg.c
*
* Author:  Denis Vakatov, Jinghui Zhang
*
* $Revision: 6.4 $
*
* File Description:
*  Draw style-dependent feature images for the feature legend map
*  Draw gene images for the gene legend map
*
* ----------------------------------------------------------------------------
* $Log: fea2seg.c,v $
* Revision 6.4  1999/10/04 17:46:22  kans
* include vibrant.h
*
* Revision 6.3  1999/05/06 21:21:13  vakatov
* Stricter type casting
*
* Revision 6.2  1998/06/16 18:33:46  kans
* fixed unix compiler warnings
*
* Revision 6.1  1997/09/16 21:24:50  vakatov
* SmartDrawText():  restore font after the AddTextLabel() call
* fit_string():  use FitStringWidth() functionality
*
* Revision 1.7  1997/08/18 15:47:32  vakatov
* Fixed the doubling Log and Revision templates
*
* Revision 1.6  1997/07/18 21:44:57  zjing
* make SmartDrawText() external
*
* Revision 1.5  1997/07/16 20:51:40  vakatov
* [_DEBUG] typo fixed
*
* Revision 1.4  1997/03/17  23:00:08  zjing
* correct a bug in the function that draws the map legend
*
* Revision 1.3  1997/03/06  17:28:27  vakatov
* Added funcs gene2segment() and compose_g_legend() to draw gene legend map;
* and test code for these.
* Now use "Int4"(instead of "Int2") for all coords and dimensions.
*
* Revision 1.2  1996/12/11  16:51:35  vakatov
* Use color when drawing symbol primitives;  added <fstyle.h>;
* +NCBI header, +log
* ----------------------------------------------------------------------------
*/

#include <vibrant.h>
#include <fea2seg.h>
#include <fstyle.h>


/*******************************************************************
 *  FEATURE
 */

/* convert_color() and add_attribute_pen() from "drawseq.c" */

#define RGB_B(x) (Uint1)((x)&255)
#define RGB_G(x) (Uint1)(((x)>>8)&255)
#define RGB_R(x) (Uint1)(((x)>>16)&255)

static Boolean convert_color(Int4 val, Uint1Ptr color)
{
  if(val<0 || color == NULL)
    return FALSE;
  color[0] = RGB_R(val);
  color[1] = RGB_G(val);
  color[2] = RGB_B(val);
  return TRUE;
}

static void add_attribute_pen(SegmenT seg, Int2 p_class, Int2 sub_class)
{
  Uint1 color[3];
  Uint1 flags = COLOR_ATT|STYLE_ATT|SHADING_ATT|WIDTH_ATT;
  Int4  c_val;
  Uint1 linestyle;
  Uint1 penwidth;
  Uint1 shading;

  c_val = GetMuskCParam(p_class, sub_class, MSM_COLOR);
  convert_color(c_val, color);

  linestyle = (Uint1)GetMuskCParam(p_class, sub_class, MSM_LTYPE);
  penwidth  = (Uint1)GetMuskCParam(p_class, sub_class, MSM_PENWIDTH);
  shading   = (Uint1)GetMuskCParam(p_class, sub_class, MSM_SHADING);

  AddAttribute(seg, flags, &color[0], linestyle, shading, penwidth, COPY_MODE);
}


static CharPtr feature_name(Int2 feature)
{
  /* from "fstyle.c" */
  extern CharPtr mSM_allfeatureNames[];

  if (feature >= FEATDEF_ANY)
    return NULL;

  if (feature > 0)
    return  mSM_allfeatureNames[feature];

  switch ( feature )
    {
    case MSM_EXTRA_GENBANK:
      return "GenBank";
    case MSM_EXTRA_MEDLINE:
      return "MedLine";
    case MSM_EXTRA_BOTH:
      return "GB+Med";
    }

  return NULL;
}


static Boolean feature2image(SegmenT seg,
                             Int2 feature,
                             const BoxInfo PNTR limits)
{
  BigScalar style, shape;
  Int2      arrow = NO_ARROW;
  Int4      x0 = 0, x1 = 0, x2 = 0, x3 = 0;
  Int4      y = (limits->top + limits->bottom) / 2;
  Int4      x = (limits->left + limits->right) / 2;

  /* get the feature style's attributes */
  style = GetMuskCParam(feature, MSM_SEGMENT, MSM_STYLE);
  shape = style & MSM_SEG_FORM;

  /* special processing -- for BOX and LINE only */ 
  if (shape == MSM_SEG_BOX  ||  shape == MSM_SEG_LINE)
    {
      Int4 prim_width = (limits->right - limits->left) / 3;
      x0 = limits->left;
      x1 = limits->left  + prim_width;
      x2 = limits->right - prim_width;
      x3 = limits->right;

      /* draw gap, if necessary */
      switch ( Nlm_GetMuskCParam(feature, MSM_FGAP, MSM_STYLE) )
        {
        case MSM_GAP_LINE:
          add_attribute_pen(seg, feature, MSM_FGAP);
          AddLine(seg, x1, y, x2, y, FALSE, 0);
          break;
        case MSM_GAP_ANGLE:
          add_attribute_pen(seg, feature, MSM_FGAP);
          AddLine(seg, x1, y, x, limits->top, FALSE, 0);
          AddLine(seg, x2, y, x, limits->top, FALSE, 0);
          break;
        }

      if (style & (Int4)MSM_SEG_SHOWORIENT)
        arrow = RIGHT_ARROW;
      add_attribute_pen(seg, feature, MSM_SEGMENT);
    }
	

  /* draw the feature */
  switch ( shape )
    {
    case MSM_SEG_BOX:
      {
        AddRectangle(seg,
                     x0, limits->top, x1, limits->bottom, FALSE, TRUE, 0);
        AddRectangle(seg,
                     x2, limits->top, x3, limits->bottom, arrow, TRUE, 0);

        if ( Nlm_GetMuskCParam(feature, MSM_SEG_BORD, MSM_TRUEFALSE) )
          {
            add_attribute_pen(seg, feature, MSM_SEG_BORD);
            AddRectangle(seg,  x0, limits->top, x1, limits->bottom,
                         FALSE,     FALSE, 0);
            AddRectangle(seg,  x2, limits->top, x3, limits->bottom,
                         arrow, FALSE, 0);
          }
        break;
      }

    case MSM_SEG_LINE:
      {
        AddLine(seg, x0, y, x1, y, FALSE,     0);
        AddLine(seg, x2, y, x3, y, (Boolean)arrow, 0);
        break;
      }

    default:
      { 
        Int2 symbol;
        switch ( shape )
          {
          case MSM_SEG_SYM_RECT:
            symbol = RECT_SYMBOL;
            break;
          case MSM_SEG_SYM_DIAMOND:
            symbol = DIAMOND_SYMBOL;
            break;
          case MSM_SEG_SYM_OVAL:
            symbol = OVAL_SYMBOL;
            break;
          case MSM_SEG_SYM_TRIANGLE:
            symbol = UP_TRIANGLE_SYMBOL;
            break;
          default:
            return FALSE;  /* unknown feature shape */
          }

        add_attribute_pen(seg, feature, MSM_SEGMENT);
        AddSymbol(seg, x, y, symbol, TRUE, MIDDLE_CENTER, 0);
        break;
      }
  }

  return TRUE;
}


/*
 * Calculate(based on the presently active font) maximum number
 * of characters from the string "str" which can be fit into
 * "*max_width" pixels.
 * Put the size of the resulting string(in pixels) to "*max_width".
 */
static size_t fit_string(const Char *str, Int4 *max_width)
{
  size_t len = FitStringWidth(str, *max_width);
  *max_width = TextWidth((CharPtr)str, len);
  return len;
}


extern Boolean SmartDrawText(SegmenT seg,
                             const Char *text, FonT font,
                             Int4 left, Int4 top,
                             Int4Ptr width, Int4Ptr height)
{
  Int4 x_height    = (height  &&  *height > 0) ? *height : INT4_MAX;
  Int4 line_height = LineHeight();
  Int4 n, n_lines  = x_height / line_height;
  Int4 max_width   = 0;
  Nlm_CharPtr str, stream;

  SelectFont( font );
  if (!seg  ||  !text  ||  !*text  ||  !width  ||  line_height < 0  ||
      x_height < line_height  ||  *width < MaxCharWidth()  ||
      (stream = str = text2stream( text )) == NULL)
    {
      if ( height )
        *height = 0;
      return FALSE;
    }

  for (n = 0;  *str != '\0'  &&  n < n_lines;  n++, top -= line_height)
    {
      Int4    x_width = *width;
      size_t  n_fit   = fit_string(str, &x_width);
      int     dash;
      size_t  n_print = stream2text(str, n_fit, &dash);
      CharPtr s       = (CharPtr)MemNew(n_fit + 1);
      Nlm_CharPtr ruled_str;

      ASSERT ( n_print <= n_fit ); 

      MemCpy(s, str, n_print);
      s[n_print] = '\0';
      if ( dash )
        {
          size_t pos = n_print;
          while (--pos  &&  !IS_WHITESP(s[pos]));
          if ( pos )
            s[n_print = pos] = '\0';
          else
            s[--n_print] = '-';
        }
      str += n_print;

      ruled_str = rule_line(s, n_fit, RL_Spread);
      MemFree( s );

      x_width = StringWidth( ruled_str );
      if (x_width > max_width)
        max_width = x_width;

      AddTextLabel(seg, left, top, ruled_str, font, 0, LOWER_RIGHT, 0);
      MemFree( ruled_str );
      SelectFont( font ); /* restore -- as it gets reset in AddTextLabel() */
    }
  MemFree( stream );

  *width = max_width;
  if ( height )
    *height = n * line_height;

  return (Boolean)(max_width > 0);
}


/*
 *  The font must be already selected by the moment of the function call
 */
static Boolean feature2label(SegmenT seg, Int2 feature,
                             BigScalar align,
                             Int4 left, Int4 top, Int4 width,
                             const Char *name)
{
  FonT font  = (FonT)GetMuskCParam(feature, MSM_FLABEL, MSM_FONT);
  Int4 color = (Int4)GetMuskCParam(feature, MSM_FLABEL, MSM_COLOR);
  const Char *str  = name ? name : feature_name( feature );
  Uint1  color3arr[3];
  Boolean ok, free_str = FALSE;

  if (str == NULL  ||  *str == '\0'  ||
      !convert_color(color, color3arr)  ||  width < 1)
    return FALSE;

  /* fit the label into the given maximum width(in pixels) */
  SelectFont( font );
  {{
    size_t max_length = fit_string(str, &width);
    if ( !max_length )
      return FALSE;

    if (max_length < StringLen( str ))
      {
        str = StringNCat((char*)MemNew(max_length + 1), str, max_length);
        if ( !str )
          return FALSE;
        free_str = TRUE;
      }
  }}

  /* draw label */
  AddAttribute(seg, COLOR_ATT, color3arr, 0, 0, 0, 0);
  ok = (Boolean)(AddTextLabel(seg, left, top, (CharPtr)str, font,
                              0, (Int2)align, 0) != NULL);

  if ( free_str )
    MemFree( (CharPtr)str );
  return ok;
}


extern SegmenT feature2segment(SegmenT parent,
                               const Char PNTR style_name,
                               Int2 feature,
                               Int4 left, Int4 top,
                               Int4 label_width, Int4 image_width,
                               Int4Ptr height)
{
  SegmenT   seg = NULL;
  Int2      actual_style = Nlm_GetMuskCurrentSt();
  Boolean   switch_style = (Boolean)StrCmp(style_name,
                                           Nlm_GetMuskStyleName(actual_style));
  Int4      seg_space;
  BigScalar label_align;
  FonT      label_font;
  Int4      image_height, label_height, max_height;
  BoxInfo   image_box;
  Int4      label_left, label_top;

  if (feature_name( feature ) == NULL)
    return NULL;

  /* create segment to draw in */
  seg = CreateSegment(parent, 0, 0);
  if (seg == NULL)
    return NULL;

  /* set working style, if necessary */
  if ( switch_style )
    SetMuskCurrentSt( (CharPtr)style_name );

  /* get the feature size and alignment attributes */
  seg_space    = (Int4)GetMuskCParam(MSM_TOPSTYLE, MSM_SPACE,   MSM_HEIGHT);
  image_height = (Int4)GetMuskCParam(feature,      MSM_SEGMENT, MSM_HEIGHT);
  label_align  =       GetMuskCParam(MSM_TOPSTYLE, MSM_LABEL,   MSM_STYLE );
  label_font   = (FonT)GetMuskCParam(feature,      MSM_FLABEL,  MSM_FONT  );
  SelectFont( label_font );
  label_height = LineHeight();

  if ((label_align == MSM_LABEL_TOP  ||  label_align == MSM_LABEL_BOTTOM))
    {
      Int2 l_width = StringWidth( feature_name( feature ) );
      if (l_width < label_width)
        label_width = l_width;
      if (label_width < image_width)
        label_width = image_width;
      max_height = label_height + image_height;
    }
  else
    max_height = MAX(label_height, image_height);

  if (height != NULL)
    *height = max_height + seg_space;

  /* deploy the feature's image and label */
  image_box.left = left;
  image_box.top  = top;
  label_left      = left;
  label_top       = top;
  switch ( label_align )
    {
    case MSM_LABEL_BOTTOM:
      label_top      -= image_height;
      label_left     += label_width / 2;
      label_align = LOWER_CENTER;
      break;
    case MSM_LABEL_TOP:
      image_box.top  -= label_height;
      label_left     += label_width / 2;
      label_align = LOWER_CENTER;
      break;
    case MSM_LABEL_LEFT:
      image_box.left += label_width;
      label_left     += label_width;
      label_top      += (label_height - max_height) / 2;
      image_box.top  += (image_height - max_height) / 2;
      label_align = LOWER_LEFT;
      break;
    case MSM_LABEL_RIGHT:
    default:
      label_left     += image_width;
      label_top      += (label_height - max_height) / 2;
      image_box.top  += (image_height - max_height) / 2;
      label_align = LOWER_RIGHT;
      break;
    }
  image_box.right  = image_box.left + image_width;
  image_box.bottom = image_box.top  - image_height;

  /* draw the fearure's image and label */
  if (!feature2image(seg, feature, &image_box)  ||
      !feature2label(seg, feature, label_align,
                     label_left, label_top, label_width, NULL))
    {
      DeleteSegment( seg );
      return NULL;
    }

  return seg;
}
      

extern SegmenT pic_for_f_legend(ValNodePtr list, CharPtr style_name,
                                Int4 image_width)
{
  ValNodePtr curr;
  Int4 max_width = 0;  /* calculate the maximum label width */

  if (list == NULL)
    return NULL;

  for (curr = list;  curr != NULL;  curr = curr->next)
    {
      Int2 type = (Int2)curr->data.intvalue;
      FonT font = (FonT)GetMuskCParam(type, MSM_FLABEL, MSM_FONT);
      SelectFont(font);
      {{
        Int2 x_width = StringWidth( feature_name(type) );
        max_width = MAX(max_width, x_width);
      }}
    }

  {{
    SegmenT pic = CreatePicture();
    Int4 left = 0;
    Int4 top  = 0;
    for (curr = list;  curr != NULL;  curr = curr->next)
      {
        Int4 height;
        Int2 type = (Int2)curr->data.intvalue;
        feature2segment(pic, style_name, type,
                        left, top, max_width, image_width,
                        &height);
        top -= height;
      }

    return pic;
  }}
}


/*******************************************************************
 *  GENE
 */

#include <legend.h>

extern SegmenT gene2segment(SegmenT parent,
                            const UserField *gene,
                            Int4 left, Int4 top,
                            Int4 image_width, Int4 label_width,
                            Int4Ptr height)
{
  SegmenT seg = NULL;

  typedef struct {
    Int4  shape;
    Int4  num;
    Int4  style;
    Uint4 rgb[3];
  }  *GeneDataPtr;

  GeneDataPtr gene_data;

  if (!gene  ||  gene->num != 6  ||  gene->choice != 8  ||
      !gene->data.ptrvalue)
    return NULL;

  gene_data = (GeneDataPtr)gene->data.ptrvalue;

  /* create the segment to draw in */
  seg = CreateSegment(parent, 0, 0);
  if (seg == NULL)
    return NULL;

  {{ /* set image color */
    static const Uint1 default_rgb[3] = { 0, 0, 0 };
    Uint1 color3arr[3];

    int i;
    for (i = 0;  i < 3;  i++)
      color3arr[i] = (Uint1)(gene_data->rgb[i] != NULL_COLOR ?
                             gene_data->rgb[i] : default_rgb[i]);

    AddAttribute(seg, COLOR_ATT, color3arr, 0, 0, 0, 0); 
  }}

  {{ /* draw the gene image */
    Int2    symbol = (Int2)gene_data->shape;
    Boolean filled = (Boolean)(gene_data->style == SOLID_STYLE);
    Int4    nSym   = gene_data->num;
    if (symbol == NULL_SYMBOL)
      {
        symbol = RECT_SYMBOL;
        filled = TRUE;
        nSym   = 1;
      }

    ASSERT ( nSym > 0 );
    AddSymbol(seg, left, top, symbol, filled, LOWER_RIGHT, 0);
    {{
      Int4 x = left;

#define symbol_width 8
#ifndef symbol_width
      /* (for some weird reason, most of the PrimitivE' "try-get-limits"
         functions return a 4-pixel outset box...) */
      BoxInfo box;
      Int4    symbol_width;
      SegmentBox(seg, &box);
      symbol_width = box.right - box.left;
#endif
      {{
        Int4 max_nSym = image_width / symbol_width;
        if (nSym > max_nSym)
          nSym = max_nSym;
      }}
      while ( --nSym )
        {
          x += symbol_width + 2;
          AddSymbol(seg, x, top, symbol, filled, LOWER_RIGHT, 0);
        }
    }}      
  }}

  /* draw the gene description, if any */
  {{
    FonT font  = (FonT)GetMuskCParam(FEATDEF_GENE, MSM_FLABEL, MSM_FONT);
    Int4 color = (Int4)GetMuskCParam(FEATDEF_GENE, MSM_FLABEL, MSM_COLOR);
    Uint1  color3arr[3];

    if ( convert_color(color, color3arr) )
      AddAttribute(seg, COLOR_ATT, color3arr, 0, 0, 0, 0);

    SmartDrawText(seg, gene->label->str, font,
                    left + image_width, top, &label_width, NULL);
  }}

  /* calculate the resulting height, if necessary */
  if ( height )
    {
      BoxInfo box;
      SegmentBox(seg, &box);
      *height = box.top - box.bottom;
    }

  return seg;
}


extern SegmenT compose_g_legend(SegmenT     parent,
                                const Char *title,
                                FonT        title_font,
                                const UserObject *genes,
                                Int4 left, Int4 top,
                                Int4 max_width, Int4Ptr height)
{
  Int4 x_width  = max_width;
  Int4 x_height = 0;
  Int4 x_top    = top;

  SegmenT seg = CreateSegment(parent, 0, 0);
  if (seg == NULL)
    return NULL;

  /* title */
  if ( SmartDrawText(seg, title, title_font,
                       left, x_top, &x_width, &x_height) )
    {
      x_top -= x_height + 3;
      /* AddLine(seg, left, x_top, left + max_width, x_top, FALSE, 0); 
      x_top -= 5; */
    }

  /* gene images/descriptions */
  {{
    UserFieldPtr ufp;

    for (ufp = genes->data;  ufp;  ufp = ufp->next)
      {
        gene2segment(seg, ufp, left, x_top, 32, max_width-32, &x_height);
        x_top -= x_height;
      }
  }}

  if ( height )
    *height = top - x_top + 1;

  return seg;
}



/*******************************************************************
 *  TESTs
 *
 * Output to window(s):
 *   LIBRARIES:  ncbidesk vibrant ncbiobj ncbi
 *   CPP_DEFS:   -DTEST_MODULE_FEA2SEG
 *
 * Output to GIF-file(s):
 *   LIBRARIES:  ncbidesk vibgif vibrant ncbiobj ncbi
 *   CPP_DEFS:   -DTEST_MODULE_FEA2SEG -DWIN_GIF -UWIN_MOTIF
 *
 * NB:  Use -DNO_TEST_STYLE to suppress feature legend test.
 *      Use -DNO_TEST_GENES to suppress gene    legend test.
 */


#ifdef TEST_MODULE_FEA2SEG

#ifndef WIN_GIF
#include <vibrant.h>
#include <viewer.h>
#else
#include <ncbigif.h>
#include <ncbifile.h>
#endif /* WIN_GIF */

#include <picture.h> 
#include <fstyle.h>
#include <fea2seg.h>

/* Quit/Cleanup for GUI tests
 */
#ifndef WIN_GIF
static void ThatsAll(WindoW w) {
  QuitProgram();
}

typedef struct
{
  SegmenT       ppp;
#ifndef NO_TEST_GENES
  UserObjectPtr uop;
#endif
}  structCleanUp, *structCleanUpPtr;

static void CleanUp(structCleanUpPtr cleanup_data)
{
  DeletePicture( cleanup_data->ppp );
#ifndef NO_TEST_GENES
  UserObjectFree( cleanup_data->uop );
#endif
}

static void CleanUpCB(Nlm_GraphiC www, Nlm_VoidPtr cleanup_data)
{
  CleanUp( (structCleanUpPtr)cleanup_data );
}
#endif


/* Draw feature legend for the given style("style_name")
 */
#ifndef NO_TEST_STYLE
static void test_style(const Char PNTR style_name)
{
  static int style_no = 0;

  SegmenT ppp = CreatePicture();
  SetMuskCurrentSt( (CharPtr)style_name );

  {{  /* draw features one after another */
  Int4 height  = 0;
  Int4 width   = 0;
  Int4 top     = 700;
  Int2 feature = 1;

  {{
  SegmenT seg = NULL;
  seg = feature2segment(ppp, style_name, MSM_EXTRA_MEDLINE,
                        100, top, 70, 70,
                        &height);
  ASSERT ( seg );

  seg = feature2segment(ppp, style_name, MSM_EXTRA_GENBANK,
                        300, top, 70, 70,
                        &height);
  ASSERT ( seg );

  seg = feature2segment(ppp, style_name, MSM_EXTRA_BOTH,
                        500, top, 70, 70,
                        &height);
  ASSERT ( seg );
  }}

  for (top  = 650;  feature <= 85  && top        > height;  top  -= height   )
  {
  Int4 left;
  for (left = 0;    feature <= 85  && left+width <    700;  left += width + 5)
    {
      Int4 image_width = 40;
      Int4 label_width = 70;
      SegmenT seg = feature2segment(ppp, style_name, feature,
                                    left, top, label_width, image_width,
                                    &height);
      ASSERT ( seg );
      width = image_width + label_width;

      feature++;
    }
  }
  }}

  {{  /* write the result to GIF-file or show it in the window */
#ifdef WIN_GIF
    char s[128];
    FILE *f;

    sprintf(s, "%d.gif", (int)Nlm_GetMuskCurrentSt());
    f = FileOpen(s, "wb");
    ASSERT ( f );
    PictureToGIF(f, 700, 710, ppp, -10, -10, LOWER_LEFT, 1, 1, FALSE);
    FileClose( f );

    DeletePicture( ppp );
#else
    WindoW www = DocumentWindow((Int2)(-10 * style_no), (Int2)(-10 * style_no),
                                725, 725,
                                (CharPtr)style_name, ThatsAll, NULL);
    VieweR vvv = CreateViewer(www, 700, 700, FALSE, FALSE);

    AttachPicture(vvv, ppp, 10, 10, LOWER_LEFT, 1, 1, NULL);

    {{
      structCleanUpPtr cleanup_data = (structCleanUpPtr)
        MemNew( sizeof(structCleanUp) );
      cleanup_data->ppp = ppp;
      SetObjectExtra(www, cleanup_data, CleanUpCB);
    }}

    RealizeWindow( www );
    Show( www );
#endif  /* WIN_GIF */
  }}

  style_no++;
}
#endif /* !NO_TEST_STYLE */


/* Draw the gene legend map;  read its description from ASN-file
 * ("legend_asn_file").
 */
#ifndef NO_TEST_GENES
static void test_genes(const Char PNTR legend_asn_file)
{
  static Char title[] = "\
0 1 2 3 4 5 6 7 8 9 a b c d e f g A B C D E F G \
W W W W W W W W W W W W W W W W W W W W W W W W W \
The NCBI (..... ..... .... .....) Software Development Toolkit \
was (!!!! !!!!!! !!!! !!!!) developed (QQQ QQQQQQQQ QQQQQQ QQQQ Q) \
for the production and (???? ????? ????? ? ??? ??? ) \
distribution of GenBank, Entrez, BLAST, and related services by NCBI. We make \
it freely available to the public without restriction to facilitate the";

  AsnIoPtr      aip = AsnIoOpen((CharPtr)legend_asn_file, "r");
  UserObjectPtr uop = UserObjectAsnRead(aip, NULL);
  UserFieldPtr  ufp = uop->data;
  SegmenT       ppp = CreatePicture();

  Int4 left = 10;
  Int4 top  = 700;

  AsnIoClose( aip );

  {{ /* draw gene legends one after(below) another */
    ASSERT ( ufp );
    for ( ;  ufp;  ufp = ufp->next)
      {
        Int4 height = 0;
        VERIFY ( gene2segment(ppp, ufp, left, top, 30, 555, &height) );
        top -= height;
      }

    {{
      FonT helv = CreateFont( Helvetica(10,0) );
      Int4 width  = 150;
      Int4 height = 150;
      AddRectangle(ppp, left, top, left + width, top - height, FALSE,FALSE, 0);
      VERIFY ( SmartDrawText(ppp, title, helv, left, top, &width, &height) );
      AddRectangle(ppp, left, top, left + width, top - height, FALSE,FALSE, 0);

      top -= height + 10;
    }}

    {{
      FonT times = CreateFont( Times(14,0) );
      CharPtr s;
      size_t  i;
      for (i = 1, s = title;  *s;  s++)
        if (*s == '?')
          *s = (Char)i++;
      VERIFY ( compose_g_legend(ppp, title, times, uop, left,top, 250, NULL) );
    }}
  }}


  {{ /* write the result to GIF-file or show it in the window */
#ifdef WIN_GIF
    FILE *f;

    CharPtr s = StringSave( legend_asn_file );
    StringNCpy(s + StringLen(s) - 3, "gif", 3);
    f = FileOpen(s, "wb");
    ASSERT ( f );
    MemFree( s );

    PictureToGIF(f, 700, 710, ppp, -10, -10, LOWER_LEFT, 1, 1, FALSE);
    FileClose( f );

    DeletePicture( ppp );
    UserObjectFree( uop );
#else
    WindoW www = DocumentWindow(-55, -55, 725, 725,
                                (CharPtr)legend_asn_file, ThatsAll, NULL);
    VieweR vvv = CreateViewer(www, 700, 700, FALSE, FALSE);

    AttachPicture(vvv, ppp, 10, 10, LOWER_LEFT, 1, 1, NULL);

    {{
      structCleanUpPtr cleanup_data = (structCleanUpPtr)
        MemNew( sizeof(structCleanUp) );
      cleanup_data->ppp = ppp;
      cleanup_data->uop = uop;
      SetObjectExtra(www, cleanup_data, CleanUpCB);
    }}

    RealizeWindow( www );
    Show( www );
#endif /* WIN_GIF */
  }}
}
#endif /* !NO_TEST_GENES */




extern Int2 Main( void )
{
#ifdef WIN_GIF
  SetUpDrawingTools();
#endif

  InitMuskStyles();

#ifndef NO_TEST_STYLE
  {{
    Int2 style;

    for (style = GetMuskTotalSt();  style ;  )
      {
        test_style( GetMuskStyleName(--style) );
      }
  }}
#endif /* !NO_TEST_STYLE */

#ifndef NO_TEST_GENES
  {{
    test_genes( "fea2seg.asn" );
  }}
#endif

  ExitMuskStyles();

#ifndef WIN_GIF
  ProcessEvents();
#endif

  return 0;
}

#endif /* TEST_MODULE_FEA2SEG */

