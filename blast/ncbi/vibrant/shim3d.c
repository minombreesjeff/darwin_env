/*   shim3d.c
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
* File Name:  shim3d.c
*
* Author:  Lewis Geer
*
* Version Creation Date:   1/26/99
*
* $Revision: 6.79 $
*
* File Description: Shim functions to replace Viewer3D with OpenGL
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: shim3d.c,v $
* Revision 6.79  2002/03/28 13:35:48  kans
* only include MoreCarbonAccessors.h if not OS_UNIX_DARWIN
*
* Revision 6.78  2001/05/25 19:46:58  vakatov
* Nested comment typo fixed
*
* Revision 6.77  2001/04/23 16:05:45  juran
* Include MoreCarbonAccessors.h, which now has GetPortText{Font,Face,Size}().
*
* Revision 6.76  2001/04/21 02:36:10  juran
* Very simple Carbon fixes.
*
* Revision 6.75  2001/04/18 16:33:54  kans
* moved define to first column
*
* Revision 6.74  2000/07/28 21:05:54  lewisg
* more c++ fixes
*
* Revision 6.73  2000/07/27 16:34:46  lewisg
* more c++ fixes
*
* Revision 6.72  2000/07/27 13:37:31  lewisg
* more c++ fixes
*
* Revision 6.71  2000/07/25 12:38:26  thiessen
* change C++-style comments to C
*
* Revision 6.70  2000/07/24 22:31:21  thiessen
* fix header conflict
*
* Revision 6.69  2000/07/22 20:13:42  thiessen
* fix header conflict
*
* Revision 6.68  2000/07/21 18:55:58  thiessen
* allow dynamic slave->master transformation
*
* Revision 6.67  2000/05/16 17:38:44  thiessen
* do glGenLists after context init on X11 - for Mesa 3.2
*
* Revision 6.66  2000/04/20 18:53:57  thiessen
* misc tweaks/fixes
*
* Revision 6.65  2000/04/20 17:47:18  thiessen
* tweak OpenGL drawing region position
*
* Revision 6.64  2000/04/19 17:56:46  thiessen
* added background color in OpenGL
*
* Revision 6.63  2000/04/17 15:54:27  thiessen
* add cylinder arrows; misc graphics tweaks
*
* Revision 6.62  2000/04/03 18:23:46  thiessen
* add arrowheads to strand bricks
*
* Revision 6.61  2000/03/27 14:47:31  thiessen
* widen logo slightly
*
* Revision 6.60  2000/03/24 20:34:59  lewisg
* add blast from file, bug fixes, get rid of redundant code, etc.
*
* Revision 6.59  2000/03/24 19:59:20  thiessen
* draw new logo in OpenGL
*
* Revision 6.58  2000/03/22 23:42:22  lewisg
* timing loop for animation
*
* Revision 6.57  2000/03/15 16:59:53  thiessen
* fix highlighting, other minor bugs
*
* Revision 6.56  2000/03/09 17:55:18  thiessen
* changes to palette handling for 8-bit OpenGL
*
* Revision 6.55  2000/03/08 21:46:13  lewisg
* cn3d saves viewport, misc bugs
*
* Revision 6.54  2000/03/06 18:35:22  thiessen
* fixes for 8-bit color
*
* Revision 6.53  2000/02/28 19:53:08  kans
* if macintosh, include <gl.h> and <glu.h>, not equivalent <GL/gl.h> and <GL/glu.h>
*
* Revision 6.52  2000/02/26 13:30:41  thiessen
* capped cylinders and worms for visible ends
*
* Revision 6.51  2000/02/26 00:01:41  thiessen
* OpenGL improvements, progress on cleanup of Show/Hide
*
* Revision 6.50  2000/02/16 14:01:40  thiessen
* warning on OGL color black or alpha 0 (if _DEBUG)
*
* Revision 6.49  2000/02/10 17:48:10  thiessen
* added OGL zoom out
*
* Revision 6.48  2000/01/25 22:58:13  thiessen
* added animation of conf-ensembles
*
* Revision 6.47  2000/01/19 15:22:33  thiessen
* working off-screen GL rendering and PNG output on Mac
*
* Revision 6.46  2000/01/18 14:57:41  lewisg
* move OGL_qobj initialization to OGL_CreateViewer
*
* Revision 6.45  2000/01/14 21:40:39  lewisg
* add translucent spheres, ion labels, new cpk, fix misc bugs
*
* Revision 6.44  2000/01/12 15:13:00  thiessen
* fixed minor OpenGL memory leak
*
* Revision 6.43  2000/01/12 14:58:14  thiessen
* added progress monitor for PNG save
*
* Revision 6.42  2000/01/11 02:55:15  thiessen
* working off-screen OpenGL rendering on Win32
*
* Revision 6.41  2000/01/07 19:37:06  thiessen
* fix minor OpenGL problems
*
* Revision 6.40  2000/01/07 16:28:36  thiessen
* fixed broken header conflicts
*
* Revision 6.39  2000/01/07 00:22:45  thiessen
* fixes for LessTif and OpenGL X visual selection
*
* Revision 6.38  2000/01/06 17:41:53  kans
* Mac complained about True and False, changed to TRUE and FALSE
*
* Revision 6.37  2000/01/06 17:23:36  thiessen
* various OpenGL improvements
*
* Revision 6.36  2000/01/06 00:04:41  lewisg
* selection bug fixes, update message outbound, animation APIs moved to vibrant
*
* Revision 6.35  2000/01/03 14:41:10  thiessen
* fixed selection pointer inaccuracy
*
* Revision 6.34  1999/12/17 20:25:01  thiessen
* put in preliminary PNG output (for Cn3D)
*
* Revision 6.33  1999/12/15 20:02:49  thiessen
* added missing prototype
*
* Revision 6.32  1999/12/15 19:18:48  thiessen
* bug fix for previous revision
*
* Revision 6.30  1999/12/08 22:57:59  thiessen
* added quality settings for OpenGL rendering
*
* Revision 6.29  1999/12/07 19:18:58  thiessen
* fixed font color problem in OpenGL on SGI
*
* Revision 6.28  1999/12/07 15:46:17  thiessen
* fonts working in OpenGL on Mac
*
* Revision 6.27  1999/12/06 14:43:59  thiessen
* made OpenGL font selection work in X/Motif
*
* Revision 6.26  1999/12/03 15:55:01  thiessen
* added font styles and prettified OpenGL label panel
*
* Revision 6.25  1999/12/02 23:56:52  thiessen
* added font selection for OpenGL/Win32 labels
*
* Revision 6.24  1999/11/23 21:18:04  thiessen
* fixed Mac prototype problem
*
* Revision 6.23  1999/11/23 19:24:16  thiessen
* better solution to OpenGL render rect setting on Mac
*
* Revision 6.22  1999/11/23 18:41:34  thiessen
* fixed Mac OpenGL render rect bug
*
* Revision 6.21  1999/11/19 18:07:23  thiessen
* added label capability for OpenGL version on Mac and Motif
*
* Revision 6.20  1999/11/16 14:30:28  thiessen
* avoid white-blanking of OpenGL window on redraw
*
* Revision 6.19  1999/11/14 19:26:29  thiessen
* fixed broken select mode in OpenGL
*
* Revision 6.18  1999/11/10 17:17:27  thiessen
* fixed diffuse/ambient coloring in 8-bit opengl
*
* Revision 6.17  1999/11/10 15:25:38  thiessen
* partial fix for 8-bit OpenGL coloring
*
* Revision 6.16  1999/11/09 14:36:59  lewisg
* NT faults on write of const array
*
* Revision 6.15  1999/11/08 20:43:05  thiessen
* added much more thorough (but optional) GL error checking; see #define DEBUG_GL
*
* Revision 6.14  1999/11/08 19:46:30  thiessen
* fixed OpenGL transformation bug; also added check for GL error flag
*
* Revision 6.13  1999/11/08 16:43:20  thiessen
* major rearrangement of OpenGL color/material/lighting; also added 3-d (thick) brick
*
* Revision 6.12  1999/11/03 17:00:39  thiessen
* added capped cylinders for 'helix' object
*
* Revision 6.11  1999/10/31 22:39:34  thiessen
* added wifreframe worm capability to viewer3d
*
* Revision 6.10  1999/10/15 17:37:43  thiessen
* put in splined 'worm' model for virtual BB
*
* Revision 6.9  1999/10/04 18:05:34  thiessen
* fix minor glX problem with Motif
*
* Revision 6.8  1999/10/04 14:27:16  thiessen
* hacked partial compatibility with Mesa OpenGL implementation - does *not* work when rendering inside vibrant window in Motif
*
* Revision 6.7  1999/09/27 18:28:29  thiessen
* Made 24-bit and doublebuffered OpenGL modes work on Mac; also should select
* mode like X
*
* Revision 6.6  1999/09/27 16:29:29  thiessen
* added OpenGL buffer swap for X
*
* Revision 6.5  1999/09/22 14:22:49  lewisg
* fixed forward declaration of TOGL_Layers to compile on SGI
*
* Revision 6.4  1999/09/21 13:45:31  thiessen
* port of Lewis's OpenGL code to X/Motif
*
* Revision 6.3  1999/09/20 20:12:56  lewisg
* change typedefs for a colorcell, add triangle generator, fix incorrect return values
*
* Revision 6.2  1999/06/14 23:15:11  lewisg
* moved useful helper functions out of the ifdef
*
* Revision 6.1  1999/04/06 14:23:28  lewisg
* add opengl replacement for viewer3d
*
*
*/

#ifdef _OPENGL

#if defined(WIN32)              /* braindead windows dependency */
#include <windows.h>

#elif defined(macintosh)
#include <agl.h>
#include <fonts.h>
# if !defined(OS_UNIX_DARWIN)
#include "MoreCarbonAccessors.h"
#endif

#elif defined(WIN_MOTIF)
#include <GL/glx.h>
#include <X11/Xlib.h>
#ifdef Status   /* avoid name conflict from Xlib */
#undef Status
#endif

#endif

/*
*  Include the GL dependencies.  GL has its own typedef's for basic types, just like the toolkit.
*  If you get warnings about type mismatch, this should be investigated.  The GL typedef's can't
*  be included in general toolkit code because of the windows.h dependency for WIN32 which
*  causes all sorts of name collisions.
*/

#if defined(macintosh)
#include <gl.h>
#include <glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#ifdef _PNG
#include <png.h> /* must go berore ncbi headers */
#endif

/* from ncbimisc.h */
#include <ncbi.h>
NLM_EXTERN void LIBCALL Nlm_HeapSort PROTO((VoidPtr base, size_t nel, size_t width,
                                           int (LIBCALLBACK *cmp) (VoidPtr, VoidPtr) ));

#endif                          /* _OPENGL */

#include <math.h>
#include <shim3d.h>
#include <stdio.h>
#include <ddvcolor.h>

#if defined(_OPENGL) && defined(_PNG)
TOGL_Data *Cn3D_GetCurrentOGLData(void); /* in cn3dxprt.c */
#endif


/* VRML functions */

void VRML_ColorToString(Char * pString, DDV_ColorCell * pColor)
{
    sprintf(pString, "%f %f %f", pColor->rgb[0] / 255.0,
            pColor->rgb[1] / 255.0, pColor->rgb[2] / 255.0);
}

void VRML_AddSphere3D(DDV_ColorCell * pColor, FloatHi x, FloatHi y,
                      FloatHi z, FloatHi radius)
{
    Char szColor[256];

    printf("Transform {\ntranslation %f %f %f\nchildren [\nShape{\n", x, y,
           z);
    printf("appearance Appearance {\nmaterial Material {\n");
    VRML_ColorToString(szColor, pColor);
    printf("diffuseColor %s\n}\n}\n", szColor);
    printf("geometry Sphere {\n radius %f\n}\n", radius);
    printf("}\n]\n}\n");
}

void VRML_AddCylinder3D(DDV_ColorCell * pColor,
                        Nlm_FloatHi x1, Nlm_FloatHi y1, Nlm_FloatHi z1,
                        Nlm_FloatHi x2, Nlm_FloatHi y2, Nlm_FloatHi z2,
                        Nlm_FloatHi radius)
{
    Char szColor[256];
    FloatHi Rotate[16], Translate[3], length, *Cross;
    FloatHi z[3] = { 0.0, 0.0, 1.0 };

    OGL_CreateCTransform(x1, y1, z1, x2, y2, z2, Rotate, Translate,
                         &length);

    Cross = OGL_CrossProduct(&Rotate[8], z);
    if (Cross == NULL)
        return;

    printf("Transform {\nrotation %f %f %f %f", Cross[0], Cross[1],
           Cross[2], acos(Rotate[10]));
    printf("\ntranslation %f %f %f\nchildren [\nShape{\n", Translate[0],
           Translate[1], Translate[2]);
    printf("appearance Appearance {\nmaterial Material {\n");
    VRML_ColorToString(szColor, pColor);
    printf("diffuseColor %s\n}\n}\n", szColor);
    printf("geometry Cylinder {\n radius %f\nheight %f\ntop FALSE\n}\n",
           radius, length);
    printf("}\n]\n}\n");

    MemFree(Cross);
}


/* function in common with vrml and opengl */

FloatHi *OGL_CrossProduct(Nlm_FloatHi * v1, Nlm_FloatHi * v2)
{
    Nlm_FloatHi *RetVal;

    if (v1 == NULL || v2 == NULL)
        return NULL;
    RetVal = MemNew(3 * sizeof(Nlm_FloatHi));
    if (RetVal == NULL)
        return NULL;

    RetVal[0] = v1[1] * v2[2] - v1[2] * v2[1];
    RetVal[1] = v1[2] * v2[0] - v1[0] * v2[2];
    RetVal[2] = v1[0] * v2[1] - v1[1] * v2[0];

    return RetVal;
}

void OGL_CreateCTransform(Nlm_FloatHi x1, Nlm_FloatHi y1, Nlm_FloatHi z1,
                          Nlm_FloatHi x2, Nlm_FloatHi y2, Nlm_FloatHi z2,
                          Nlm_FloatHi * Rotate, Nlm_FloatHi * Translate,
                          Nlm_FloatHi * length)
{
    FloatHi a, b, c;            /* the normal z */
    FloatHi yy2, yy3;           /* the normal y */
    FloatHi xx1, xx2, xx3;      /* the normal x */
    Int4 iCount;

    if (Rotate == NULL || Translate == NULL || length == NULL)
        return;

    Translate[0] = (x1 + x2) / 2;
    Translate[1] = (y1 + y2) / 2;
    Translate[2] = (z1 + z2) / 2;

    *length =
        sqrt(OGL_SQR(x1 - x2) + OGL_SQR(y1 - y2) + OGL_SQR(z1 - z2)) / 2.0;

    for (iCount = 0; iCount < 16; iCount++)
        Rotate[iCount] = 0.0;
    Rotate[15] = 1;             /* identity */

    /* create the normal z */
    a = (x1 - Translate[0]) / (*length);
    b = (y1 - Translate[1]) / (*length);
    c = (z1 - Translate[2]) / (*length);

    /* create the normal y */

    yy2 = sqrt(1.0 / (1.0 + OGL_SQR(b) / OGL_SQR(c)));
    yy3 = -(b / c) * yy2;

    /* create the normal x */

    xx2 =
        sqrt(1.0 /
             (pow(c, 4.0) / (OGL_SQR(a) * OGL_SQR(b)) +
              2.0 * OGL_SQR(c) / OGL_SQR(a)
              + OGL_SQR(b) / OGL_SQR(a) + 1 + OGL_SQR(c) / OGL_SQR(b)));
    xx3 = xx2 * c / b;
    xx1 = (-OGL_SQR(c) / (a * b) - b / a) * xx2;


    /* now use the normals to make the rotation matrix */

    Rotate[0] = xx1;
    Rotate[1] = xx2;
    Rotate[2] = xx3;

    Rotate[4] = 0.0;
    Rotate[5] = yy2;
    Rotate[6] = yy3;

    Rotate[8] = a;
    Rotate[9] = b;
    Rotate[10] = c;
}



#ifdef _OPENGL


static Nlm_VoidPtr OGL_CurrentName = NULL;



/* define this to do (frequent) checking of GL error status - but this is
   very expensive, so be sure to turn off for production! */
/* #define DEBUG_GL 1 */
#if defined(_DEBUG) && !defined(DEBUG_GL)
#define DEBUG_GL 1
#endif

/* for now, just print warning if any GL error flag is set */
static Boolean OGL_CheckForErrors(void)
{
    GLenum errCode;
    const GLubyte *errString;
    Boolean hadErrors = FALSE;

    while ((errCode = glGetError()) != GL_NO_ERROR) {
        errString = gluErrorString(errCode);
        Message(MSG_POST, "OpenGL error: %s", errString);
        hadErrors = TRUE;
    }
    return hadErrors;
}

/*
*   Various helper functions used in drawing
*/

typedef struct _TOGL_Layers
/* this struct contains the information used to manage the different layers of the display */
{
    GLuint FirstLayer;
    GLuint LastLayer;
    GLuint SelectedLayer;
    Nlm_Boolean IsOn[OGLMAXLAYERS];
} TOGL_Layers;


void OGL_Normalize(Nlm_FloatHi * v)
/* normalize a vector */
{
    Nlm_FloatHi Length;

    if (v == NULL)
        return;
    Length = sqrt(OGL_SQR(v[0]) + OGL_SQR(v[1]) + OGL_SQR(v[2]));
    v[0] /= Length;
    v[1] /= Length;
    v[2] /= Length;
}


FloatHi *OGL_MakeNormal(Nlm_FloatHi * origin, Nlm_FloatHi * v1,
                        Nlm_FloatHi * v2)
/* creates a normal to the given 3 vertices */
{
    Nlm_FloatHi Vector1[3], Vector2[3], *RetValue;

    if (origin == NULL || v1 == NULL || v2 == NULL)
        return NULL;
    Vector1[0] = v1[0] - origin[0];
    Vector1[1] = v1[1] - origin[1];
    Vector1[2] = v1[2] - origin[2];

    Vector2[0] = v2[0] - origin[0];
    Vector2[1] = v2[1] - origin[1];
    Vector2[2] = v2[2] - origin[2];

    RetValue = OGL_CrossProduct(Vector1, Vector2);
    OGL_Normalize(RetValue);
    return RetValue;
}


static void ColorCell2Array(GLfloat * array, DDV_ColorCell * color)
/* copies a color cell to a GL array */
{
    if (array == NULL || color == NULL)
        return;
    array[0] = (GLfloat) (color->rgb[0] / 255.0);
    array[1] = (GLfloat) (color->rgb[1] / 255.0);
    array[2] = (GLfloat) (color->rgb[2] / 255.0);
}


/* these are used for both matrial colors and light colors */
static const GLfloat Color_Off[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
static const GLfloat Color_MostlyOff[4] = { 0.05f, 0.05f, 0.05f, 1.0f };
static const GLfloat Color_MostlyOn[4] = { 0.95f, 0.95f, 0.95f, 1.0f };
static const GLfloat Color_On[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

/* cache previous color, to avoid unnecessary calls to glMaterial */
void OGL_SetColor(TOGL_Data * OGL_Data, DDV_ColorCell * color, GLenum type,
                  GLfloat alpha)
{
#ifdef DEBUG_GL
    if (OGL_CheckForErrors()) Message(MSG_POST, "GL error entering SetColor");
#endif

    if (!OGL_Data) return;
    if (OGL_Data->IndexMode == FALSE ) {
        static GLfloat pr, pg, pb, pa;
        static GLenum pt = GL_NONE;
        static GLfloat rgb[4];

        if (!color) {
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, Color_Off);
            pt = GL_NONE;
            return;
        }
        ColorCell2Array(rgb, color);

#ifdef _DEBUG
        if (rgb[0] == 0.0 && rgb[1] == 0.0 && rgb[2] == 0.0)
            Message(MSG_POST, "Warning: OGL_Setcolor request color (0,0,0)");
        if (alpha == 0.0)
            Message(MSG_POST, "Warning: OGL_SetColor request alpha 0.0");
#endif

        rgb[3] = alpha;
        if (rgb[0] != pr || rgb[1] != pg || rgb[2] != pb || rgb[3] != pa || type != pt) {
            if (type != pt) {
                if (type == GL_DIFFUSE) {
                    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, Color_MostlyOff);
                } else if (type == GL_AMBIENT) {
                    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Color_Off);
                } else {
                    printf("don't know how to handle material type %i\n",type);
                }
                pt = type;
            }
            glMaterialfv(GL_FRONT_AND_BACK, type, rgb);
            if (type == GL_AMBIENT) {
                /* this is necessary so that fonts are rendered in correct
                   color in SGI's OpenGL implementation, and maybe others */
                glColor4f(rgb[0], rgb[1], rgb[2], rgb[3]);
            }
            pr = rgb[0];
            pg = rgb[1];
            pb = rgb[2];
            pa = rgb[3];
        }

    } else { /* color index mode */
        ValNodePtr PaletteIndex;
        GLint indx[3];
        static GLint pi0 = -1, pi1, pi2;
        static GLint pt;

        if (!color) {
            pi0 = -1;
            return;
        }

        PaletteIndex = OGL_SearchPaletteIndex(OGL_Data->PaletteIndex, color);
        if (!PaletteIndex) {
            Message(MSG_POST, "Couldn't find color in PaletteIndex!");
            return;
        }
        if (type == GL_DIFFUSE) {
            indx[0] = ((TOGL_PaletteIndex *) (PaletteIndex->data.ptrvalue))->Begin;
            indx[1] = ((TOGL_PaletteIndex *) (PaletteIndex->data.ptrvalue))->End;
            indx[2] = ((TOGL_PaletteIndex *) (PaletteIndex->data.ptrvalue))->End;
        } else if (type == GL_AMBIENT) {
            indx[0] = ((TOGL_PaletteIndex *) (PaletteIndex->data.ptrvalue))->End;
            indx[1] = ((TOGL_PaletteIndex *) (PaletteIndex->data.ptrvalue))->End;
            indx[2] = ((TOGL_PaletteIndex *) (PaletteIndex->data.ptrvalue))->End;
        } else {
            Message(MSG_POST, "don't know how to handle material type %i\n",type);
        }

#ifdef WIN32
        /* on Windows, need to skip over the first ten static palette colors */
        indx[0] += 10;
        indx[1] += 10;
        indx[2] += 10;
#endif

        if (indx[0] != pi0 || indx[1] != pi1 || indx[2] != pi2 || type != pt) {
            glMaterialiv(GL_FRONT_AND_BACK, GL_COLOR_INDEXES, indx);
            pi0 = indx[0];
            pi1 = indx[1];
            pi2 = indx[2];
            pt = type;
        }
    }

#ifdef DEBUG_GL
    if (OGL_CheckForErrors()) Message(MSG_POST, "GL error leaving SetColor");
#endif
}

/* only need single once-allocated quadric */
static GLUquadricObj *OGL_qobj = NULL;

/*
 *  Functions used to draw various primitives
 */

void OGL_AddQuad3D(TOGL_Data * OGL_Data, DDV_ColorCell * color,
                   Nlm_FloatHi * v1, Nlm_FloatHi * v2, Nlm_FloatHi * v3,
                   Nlm_FloatHi * v4)
/* draws a quadralateral with the 4 given vertices of form double v1[3] */
{
    Nlm_FloatHi *Normal;

    if (v1 == NULL || v2 == NULL || v3 == NULL || v4 == NULL
        || OGL_Data == NULL || color == NULL)
        return;

    OGL_SetColor(OGL_Data, color, GL_DIFFUSE, 1.0);

    /*glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);*/

    glBegin(GL_QUADS);
    Normal = OGL_MakeNormal(v1, v2, v4);
    if (Normal != NULL) {
        glNormal3dv(Normal);
        MemFree(Normal);
    }
    glVertex3dv(v1);
    glVertex3dv(v2);
    glVertex3dv(v3);
    glVertex3dv(v4);
    glEnd();

    /*glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);*/
}

void OGL_AddBrick3D(TOGL_Data * OGL_Data, DDV_ColorCell * color,
                    Nlm_FloatHi * Nterm, Nlm_FloatHi * Cterm,
                    Nlm_FloatHi * norm, Nlm_FloatHi width,
                    Nlm_FloatHi thickness, Nlm_Boolean doArrow)
{
    static const double arrowLen = 2.8, arrowWidthProp = 1.6;
    
    GLdouble c000[3], c001[3], c010[3], c011[3],
             c100[3], c101[3], c110[3], c111[3], n[3];
    Nlm_FloatHi a[3], *h;
    int i;

#ifdef DEBUG_GL
    if (OGL_CheckForErrors()) Message(MSG_POST, "GL error entering AddBrick");
#endif

    if (Nterm == NULL || Cterm == NULL || norm == NULL ||
        OGL_Data == NULL || color == NULL ||
        width*thickness == 0.0)
        return;

    OGL_SetColor(OGL_Data, color, GL_DIFFUSE, 1.0);

    /* in this brick's world coordinates, the long axis (N-C direction) is
       along +Z, with N terminus at Z=0; width is in the X direction, and
       thickness in Y. Arrowhead at C-terminus, of course. */
           
    OGL_Normalize(norm);

    for (i=0; i<3; i++) {
        a[i] = Cterm[i] - Nterm[i];
    }
    OGL_Normalize(a);
    h = OGL_CrossProduct(norm, a);
    if (!h) return;

    if (doArrow)
        for (i=0; i<3; i++)
            Cterm[i] -= a[i] * arrowLen;
            
    for (i=0; i<3; i++) {
        c000[i] = Nterm[i] - h[i]*width/2 - norm[i]*thickness/2;
        c001[i] = Cterm[i] - h[i]*width/2 - norm[i]*thickness/2;
        c010[i] = Nterm[i] - h[i]*width/2 + norm[i]*thickness/2;
        c011[i] = Cterm[i] - h[i]*width/2 + norm[i]*thickness/2;
        c100[i] = Nterm[i] + h[i]*width/2 - norm[i]*thickness/2;
        c101[i] = Cterm[i] + h[i]*width/2 - norm[i]*thickness/2;
        c110[i] = Nterm[i] + h[i]*width/2 + norm[i]*thickness/2;
        c111[i] = Cterm[i] + h[i]*width/2 + norm[i]*thickness/2;
    }

    glBegin(GL_QUADS);

    for (i=0; i<3; i++) n[i] = norm[i];
    glNormal3dv(n);
    glVertex3dv(c010);
    glVertex3dv(c011);
    glVertex3dv(c111);
    glVertex3dv(c110);

    for (i=0; i<3; i++) n[i] = -norm[i];
    glNormal3dv(n);
    glVertex3dv(c000);
    glVertex3dv(c100);
    glVertex3dv(c101);
    glVertex3dv(c001);

    for (i=0; i<3; i++) n[i] = h[i];
    glNormal3dv(n);
    glVertex3dv(c100);
    glVertex3dv(c110);
    glVertex3dv(c111);
    glVertex3dv(c101);

    for (i=0; i<3; i++) n[i] = -h[i];
    glNormal3dv(n);
    glVertex3dv(c000);
    glVertex3dv(c001);
    glVertex3dv(c011);
    glVertex3dv(c010);

    for (i=0; i<3; i++) n[i] = -a[i];
    glNormal3dv(n);
    glVertex3dv(c000);
    glVertex3dv(c010);
    glVertex3dv(c110);
    glVertex3dv(c100);

    if (!doArrow) {
        for (i=0; i<3; i++) n[i] = a[i];
        glNormal3dv(n);
        glVertex3dv(c001);
        glVertex3dv(c101);
        glVertex3dv(c111);
        glVertex3dv(c011);

    } else {
        GLdouble FT[3], LT[3], RT[3], FB[3], LB[3], RB[3];
        Nlm_FloatHi *nL, *nR;
    
        for (i=0; i<3; i++) {
            FT[i] = Cterm[i] + norm[i]*thickness/2 + a[i]*arrowLen;
            LT[i] = Cterm[i] + norm[i]*thickness/2 + h[i]*arrowWidthProp*width/2;
            RT[i] = Cterm[i] + norm[i]*thickness/2 - h[i]*arrowWidthProp*width/2;
            FB[i] = Cterm[i] - norm[i]*thickness/2 + a[i]*arrowLen;
            LB[i] = Cterm[i] - norm[i]*thickness/2 + h[i]*arrowWidthProp*width/2;
            RB[i] = Cterm[i] - norm[i]*thickness/2 - h[i]*arrowWidthProp*width/2;
        }

        for (i=0; i<3; i++) n[i] = -a[i];
        glNormal3dv(n);
        glVertex3dv(c111);
        glVertex3dv(LT);
        glVertex3dv(LB);
        glVertex3dv(c101);

        glVertex3dv(c011);
        glVertex3dv(c001);
        glVertex3dv(RB);
        glVertex3dv(RT);

        for (i=0; i<3; i++) h[i] = FT[i] - LT[i];
        if (!(nL = OGL_CrossProduct(norm, h))) return;
        OGL_Normalize(nL);
        for (i=0; i<3; i++) n[i] = nL[i];
        glNormal3dv(n);
        glVertex3dv(FT);
        glVertex3dv(FB);
        glVertex3dv(LB);
        glVertex3dv(LT);
        MemFree(nL);

        for (i=0; i<3; i++) h[i] = FT[i] - RT[i];
        if (!(nR = OGL_CrossProduct(h, norm))) return;
        OGL_Normalize(nR);
        for (i=0; i<3; i++) n[i] = nR[i];
        glNormal3dv(n);
        glVertex3dv(FT);
        glVertex3dv(RT);
        glVertex3dv(RB);
        glVertex3dv(FB);
        MemFree(nR);

        glEnd();
        glBegin(GL_TRIANGLES);
        
        for (i=0; i<3; i++) n[i] = norm[i];
        glNormal3dv(n);
        glVertex3dv(FT);
        glVertex3dv(LT);
        glVertex3dv(RT);
        
        for (i=0; i<3; i++) n[i] = -norm[i];
        glNormal3dv(n);
        glVertex3dv(FB);
        glVertex3dv(RB);
        glVertex3dv(LB);
    }

    glEnd();

    MemFree(h);

#ifdef DEBUG_GL
    if (OGL_CheckForErrors()) Message(MSG_POST, "GL error leaving AddBrick");
#endif
}

void OGL_AddTri3D(TOGL_Data * OGL_Data, DDV_ColorCell * color,
                  Nlm_FloatHi * v1, Nlm_FloatHi * v2, Nlm_FloatHi * v3,
                  Nlm_FloatHi * Normal)
/* draws a triangle given 3 vertices of form double v1[3] and the normal */
{
    if (v1 == NULL || v2 == NULL || v3 == NULL || OGL_Data == NULL ||
        color == NULL)
        return;

    OGL_SetColor(OGL_Data, color, GL_DIFFUSE, 1.0);

    /*glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);*/

    glBegin(GL_TRIANGLES);
    if (Normal != NULL)
        glNormal3dv(Normal);
    glVertex3dv(v1);
    glVertex3dv(v2);
    glVertex3dv(v3);
    glEnd();

    /*glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);*/
}


void OGL_AddCylinder3D(TOGL_Data * OGL_Data, DDV_ColorCell * color,
                       Nlm_FloatHi x1, Nlm_FloatHi y1, Nlm_FloatHi z1, Nlm_Boolean cap1,
                       Nlm_FloatHi x2, Nlm_FloatHi y2, Nlm_FloatHi z2, Nlm_Boolean cap2,
                       Nlm_FloatHi radius, Nlm_Int4 sides, Nlm_Boolean doArrow)
                        /* create a cylinder with given endcaps and radius */
{
    static const double arrowLen = 4.0,
        arrowWidthPropBase = 1.2, arrowWidthPropTip = 0.4;

    Nlm_Int4 iCount;
    GLdouble length;

#ifdef DEBUG_GL
    if (OGL_CheckForErrors()) Message(MSG_POST, "GL error entering AddCylinder");
#endif

    if (OGL_Data == NULL || color == NULL)
        return;

    OGL_SetColor(OGL_Data, color, GL_DIFFUSE, 1.0);

    glPushMatrix();

    length = sqrt(
        (x2 - x1) * (x2 - x1) +
        (y2 - y1) * (y2 - y1) +
        (z2 - z1) * (z2 - z1)
    );
    if (length < 0.000001) return;

    /* to translate into place */
    glTranslated(x1, y1, z1);

    /* to rotate from initial position, so bond points right direction;
       handle special case where both ends share ~same x,y */
#define DEGREES(rad) ((rad)*180.0/3.14159265358979323846)
    if (fabs(y1 - y2) < 0.000001 &&
        fabs(x2 - x1) < 0.000001) {
        if (z2 - z1 < 0.0) glRotated(180.0,1.0,0.0,0.0);
    } else {
        glRotated(DEGREES(acos((z2 - z1) / length)),
                  y1 - y2, x2 - x1, 0.0);
    }

    if (doArrow) length -= arrowLen;
    gluCylinder(OGL_qobj, radius, radius, length, sides, 1);

    if (cap1) {
        glPushMatrix();
        glRotated(180.0, 0.0, 1.0, 0.0);
        gluDisk(OGL_qobj, 0.0, radius, sides, 1);
        glPopMatrix();
    }
    if (doArrow) {
        glPushMatrix();
        glTranslated(0.0, 0.0, length);
        if (arrowWidthPropBase > 1.0) {
            glPushMatrix();
            glRotated(180.0, 0.0, 1.0, 0.0);
            gluDisk(OGL_qobj, 0.0, radius*arrowWidthPropBase, sides, 1);
            glPopMatrix();
        }
        gluCylinder(OGL_qobj, radius*arrowWidthPropBase,
            radius*arrowWidthPropTip, arrowLen, sides, 10);
        if (arrowWidthPropTip > 0.0) {
            glTranslated(0.0, 0.0, arrowLen);
            gluDisk(OGL_qobj, 0.0, radius*arrowWidthPropTip, sides, 1);
        }
        glPopMatrix();
    } else if (cap2) {
        glPushMatrix();
        glTranslated(0.0, 0.0, length);
        gluDisk(OGL_qobj, 0.0, radius, sides, 1);
        glPopMatrix();
    }

    glPopMatrix();

#ifdef DEBUG_GL
    if (OGL_CheckForErrors()) Message(MSG_POST, "GL error leaving AddCylinder");
#endif
}


void OGL_AddLine3D(TOGL_Data * OGL_Data, DDV_ColorCell * color,
                   Nlm_FloatHi x1, Nlm_FloatHi y1, Nlm_FloatHi z1,
                   Nlm_FloatHi x2, Nlm_FloatHi y2, Nlm_FloatHi z2)
                    /* draw a single line */
{
#ifdef DEBUG_GL
    if (OGL_CheckForErrors()) Message(MSG_POST, "GL error entering AddLine");
#endif

    if (OGL_Data == NULL || color == NULL)
        return;

    OGL_SetColor(OGL_Data, color, GL_AMBIENT, 1.0);

    glBegin(GL_LINES);
    glVertex3d(x1, y1, z1);
    glVertex3d(x2, y2, z2);
    glEnd();

#ifdef DEBUG_GL
    if (OGL_CheckForErrors()) Message(MSG_POST, "GL error leaving AddLine");
#endif
}


typedef struct _TransparentSphereData {
    DDV_ColorCell color;
    Nlm_FloatHi x, y, z, radius, alpha, distFromCamera;
    Nlm_Int4 slices, stacks;
    Nlm_Int1 layer;
    Nlm_VoidPtr name;
    ValNodePtr transforms;
    struct _TransparentSphereData *next;
} TransparentSphereData, PNTR TransparentSphereDataPtr;

static TransparentSphereDataPtr OGL_transSpheresTail = NULL,
                                OGL_transSpheresHead = NULL,
                                *OGL_transSpheresList = NULL;

int LIBCALLBACK OGL_DistCompareFunc(Nlm_VoidPtr va, Nlm_VoidPtr vb)
{
    TransparentSphereDataPtr a = *((TransparentSphereDataPtr *) va),
                             b = *((TransparentSphereDataPtr *) vb);
    
    if (a->distFromCamera > b->distFromCamera) return -1;
    else if (a->distFromCamera < b->distFromCamera) return 1;
	else return 0;
}

Nlm_Boolean OGL_GetLayer(TOGL_Data *, Nlm_Int4);

static void OGL_RenderTransparentSpheres(TOGL_Data *OGL_Data)
{
#ifdef DEBUG_GL
    if (OGL_CheckForErrors()) Message(MSG_POST, "GL error entering OGL_RenderTransparentSpheres");
#endif

    if (OGL_Data && OGL_transSpheresHead) {
        TransparentSphereDataPtr sph;
        int i, n, iList;
        GLdouble m[16];
        Nlm_FloatHi x, y, z;
        Nlm_Boolean show;

        /* make an array of pointers to sphere data; sort by distance from camera */
        for (n=0, sph=OGL_transSpheresHead; sph; n++, sph = sph->next) {

            /* transform model's xyz into GL-frame coordinates */
            OGL_PushTransformation(sph->transforms);
            glGetDoublev(GL_MODELVIEW_MATRIX, m);
            OGL_PopTransformation();

            x = m[0]*sph->x + m[4]*sph->y + m[8]*sph->z + m[12];
            y = m[1]*sph->x + m[5]*sph->y + m[9]*sph->z + m[13];
            z = m[2]*sph->x + m[6]*sph->y + m[10]*sph->z + m[14];
            sph->distFromCamera =
                sqrt((x * x) + (y * y) +
                     ((z - OGL_Data->CameraDistance) * 
                      (z - OGL_Data->CameraDistance)))
                - sph->radius;
        }

        if (!OGL_transSpheresList) {
            OGL_transSpheresList = (TransparentSphereDataPtr *)
                MemNew(n * sizeof(TransparentSphereDataPtr));
            if (!OGL_transSpheresList) return;
            for (n=0, sph=OGL_transSpheresHead; sph; n++, sph=sph->next)
                OGL_transSpheresList[n] = sph;
        }
        Nlm_HeapSort((Nlm_VoidPtr) OGL_transSpheresList, n,
                     sizeof(TransparentSphereDataPtr), OGL_DistCompareFunc);

        /* turn on blending */
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        /* render the spheres in order, and only for turned-on layers */
        for (i = 0; i < n; i++) {
            show = FALSE;
            if (OGL_Data->Layers->SelectedLayer) {
                if (OGL_Data->Layers->SelectedLayer - OGL_Data->Layers->FirstLayer == 
                    OGL_transSpheresList[i]->layer) {
                    show = TRUE;
                }
            } else {
                for (iList = OGL_Data->Layers->FirstLayer;
                     iList <= OGL_Data->Layers->LastLayer;
                     iList++) {
                    if (iList - OGL_Data->Layers->FirstLayer == OGL_transSpheresList[i]->layer &&
                        OGL_GetLayer(OGL_Data, iList - OGL_Data->Layers->FirstLayer)) {
                        show = TRUE;
                        break;
                    }
                }
            }
            if (show) {
                OGL_LoadName(OGL_transSpheresList[i]->name);
                OGL_SetColor(OGL_Data, &(OGL_transSpheresList[i]->color),
                             GL_DIFFUSE, OGL_transSpheresList[i]->alpha);
                OGL_PushTransformation(OGL_transSpheresList[i]->transforms);
                glTranslated(OGL_transSpheresList[i]->x, OGL_transSpheresList[i]->y,
                             OGL_transSpheresList[i]->z);
                gluSphere(OGL_qobj, OGL_transSpheresList[i]->radius,
                          OGL_transSpheresList[i]->slices, OGL_transSpheresList[i]->stacks);
                OGL_PopTransformation();
            }
        }

        /* blending back off now */
        glDisable(GL_BLEND);
    }

#ifdef DEBUG_GL
    if (OGL_CheckForErrors()) Message(MSG_POST, "GL error leaving OGL_RenderTransparentSpheres");
#endif
}

static Nlm_Int1 OGL_currentLayer;

static void OGL_AddTransparentSphere(DDV_ColorCell * color,
                     Nlm_FloatHi x, Nlm_FloatHi y, Nlm_FloatHi z,
                     Nlm_FloatHi radius, Nlm_Int4 slices, Nlm_Int4 stacks,
                     Nlm_FloatHi alpha, Nlm_VoidPtr name, ValNodePtr transforms)
{
    TransparentSphereDataPtr newSphere = (TransparentSphereDataPtr)
        MemNew(sizeof(TransparentSphereData));
    if (newSphere) {
        DDV_CopyColorCell(&(newSphere->color), color);
        newSphere->x = x;
        newSphere->y = y;
        newSphere->z = z;
        newSphere->transforms = transforms;
        newSphere->radius = radius;
        newSphere->slices = slices;
        newSphere->stacks = stacks;
        newSphere->alpha = alpha;
        newSphere->layer = OGL_currentLayer;
        newSphere->name = name;
        newSphere->next = NULL;
        if (OGL_transSpheresTail)
            OGL_transSpheresTail = OGL_transSpheresTail->next = newSphere;
        else
            OGL_transSpheresTail = OGL_transSpheresHead = newSphere;
    }
}

void OGL_ClearTransparentSpheres(void)
{
    TransparentSphereDataPtr sph = OGL_transSpheresHead, tmp;
    
    while (sph) {
        tmp = sph->next;
        MemFree(sph);
        sph = tmp;
    }
    OGL_transSpheresHead = OGL_transSpheresTail = NULL;
    if (OGL_transSpheresList) {
        MemFree(OGL_transSpheresList);
        OGL_transSpheresList = NULL;
    }
}


void OGL_AddSphere3D(TOGL_Data * OGL_Data, DDV_ColorCell * color,
                     Nlm_FloatHi x, Nlm_FloatHi y, Nlm_FloatHi z,
                     Nlm_FloatHi radius, Nlm_Int4 slices, Nlm_Int4 stacks,
                     Nlm_FloatHi alpha, ValNodePtr transforms)
                      /* draws a sphere */
{
#ifdef DEBUG_GL
    if (OGL_CheckForErrors()) Message(MSG_POST, "GL error entering AddSphere");
#endif

    if (OGL_Data == NULL || color == NULL)
        return;

    if(!OGL_Data->IndexMode && alpha < 1.0) /* no transparency in index mode */
        OGL_AddTransparentSphere(color, x, y, z, radius, slices, stacks,
                                 alpha, OGL_CurrentName, transforms);
    else {
        OGL_SetColor(OGL_Data, color, GL_DIFFUSE, 1.0);
        glPushMatrix();
        glTranslated(x, y, z);
        gluSphere(OGL_qobj, radius, slices, stacks);
        glPopMatrix();
    }

#ifdef DEBUG_GL
    if (OGL_CheckForErrors()) Message(MSG_POST, "GL error leaving AddSphere");
#endif
}


void OGL_AddText3D(TOGL_Data * OGL_Data, DDV_ColorCell * color,
                   Nlm_CharPtr string, Nlm_FloatHi x, Nlm_FloatHi y,
                   Nlm_FloatHi z, Nlm_Int2 flags)
{
    Nlm_Int4 Length, i;

    if (OGL_Data == NULL || color == NULL || string == NULL)
        return;

    OGL_SetColor(OGL_Data, color, GL_AMBIENT, 1.0);

    glListBase(OGLFONTBASE);

    Length = Nlm_StrLen(string);
    glRasterPos3d(x, y, z);
    if (flags & OGLTEXT3D_CENTER) {
        glBitmap(0, 0, 0.0, 0.0,
                 (GLfloat) -0.5 * Length * OGL_Data->SpaceWidth, 0.0,
                 NULL);
    }
    if (flags & OGLTEXT3D_MIDDLE)
        glBitmap(0, 0, 0.0, 0.0,
                 0.0f, (GLfloat) (-0.5 * OGL_Data->SpaceHeight),
                 NULL);

    glCallLists(Length, GL_UNSIGNED_BYTE, string);

    glListBase(0);
}


void OGL_PushTransformation(ValNodePtr transforms)
{
    FloatLoPtr pflv;
    FloatLoPtr *ppflm;
    GLfloat xmat[16];

    glPushMatrix();

    while (transforms) {
        if (transforms->choice == 2) { /* Move_translate */
            pflv = (FloatLoPtr) transforms->data.ptrvalue;
            glTranslatef(pflv[0], pflv[1], pflv[2]);

        } else if (transforms->choice == 1) { /* Move_rotate */
            ppflm = (FloatLoPtr *) transforms->data.ptrvalue;
            xmat[0]=ppflm[0][0]; xmat[4]=ppflm[1][0]; xmat[8]= ppflm[2][0]; xmat[12]=0;
            xmat[1]=ppflm[0][1]; xmat[5]=ppflm[1][1]; xmat[9]= ppflm[2][1]; xmat[13]=0;
            xmat[2]=ppflm[0][2]; xmat[6]=ppflm[1][2]; xmat[10]=ppflm[2][2]; xmat[14]=0;
            xmat[3]=0;           xmat[7]=0;           xmat[11]=0;           xmat[15]=1;
            glMultMatrixf(xmat);
        }

        transforms = transforms->next;
    }
}

void OGL_PopTransformation(void)
{
    glPopMatrix();
}

/*
*   Functions used to manage display lists
*/

void OGL_Start(TOGL_Data * OGL_Data, Nlm_Int1 List)
{
#ifdef DEBUG_GL
    if (OGL_CheckForErrors()) Message(MSG_POST, "GL error entering OGL_Start");
#endif

    if (OGL_Data == NULL)
        return;

    /* begin a display list */
    if (List >= OGLMAXLAYERS)
        return;
    glNewList(List + OGL_Data->Layers->FirstLayer, GL_COMPILE);
    OGL_SetLayer(OGL_Data, List, TRUE);
    OGL_currentLayer = List;

    /* clear all color states */
    OGL_SetColor(OGL_Data, NULL, GL_NONE, 1.0);

#ifdef DEBUG_GL
    if (OGL_CheckForErrors()) Message(MSG_POST, "GL error leaving OGL_Start");
#endif
}

void OGL_End()
/* end a display list */
{
#ifdef DEBUG_GL
    if (OGL_CheckForErrors()) Message(MSG_POST, "GL error entering OGL_End");
#endif
    glEndList();
#ifdef DEBUG_GL
    if (OGL_CheckForErrors()) Message(MSG_POST, "GL error leaving OGL_End");
#endif
}


void OGL_SetLayers(TOGL_Data * OGL_Data, Nlm_Boolean Status)
/* set the status of all the layers */
{
    Nlm_Int4 i;

    if (OGL_Data == NULL)
        return;
    for (i = 0; i < OGLMAXLAYERS; i++)
        OGL_Data->Layers->IsOn[i] = Status;
    return;
}

void OGL_SetLayer(TOGL_Data * OGL_Data, Nlm_Int4 i, Nlm_Boolean Status)
/* set the status of a particular layer -- is it on or off? */
{
    if (OGL_Data == NULL)
        return;
    OGL_Data->Layers->IsOn[i] = Status;
    return;
}

Nlm_Boolean OGL_GetLayer(TOGL_Data * OGL_Data, Nlm_Int4 i)
/* return layer status */
{
    if (OGL_Data == NULL)
        return FALSE;
    return OGL_Data->Layers->IsOn[i];
}


void OGL_SetLayerTop3D(TOGL_Data * OGL_Data, Nlm_Int4 TopLayer)
/* set the highest value layer used */
{
    if (OGL_Data == NULL)
        return;
    OGL_Data->Layers->LastLayer = TopLayer + OGL_Data->Layers->FirstLayer;
    return;
}

void OGL_AllLayerOnProc(TOGL_Data * OGL_Data)
/* turn on all used layers */
{
    if (OGL_Data == NULL)
        return;
    OGL_Data->Layers->SelectedLayer = 0;
    OGL_SetLayers(OGL_Data, TRUE);
    return;
}

void OGL_RewindLayerProc(TOGL_Data * OGL_Data)
/* rewind to the first layer */
{
    if (OGL_Data == NULL)
        return;
    OGL_Data->Layers->SelectedLayer = OGL_Data->Layers->FirstLayer;
    return;
}


void OGL_PrevLayerProc(TOGL_Data * OGL_Data)
/* go back to the previous layer */
{
    if (OGL_Data == NULL)
        return;
    if (OGL_Data->Layers->SelectedLayer) {
        if (OGL_Data->Layers->SelectedLayer > OGL_Data->Layers->FirstLayer)
            OGL_Data->Layers->SelectedLayer--;
        else
            OGL_Data->Layers->SelectedLayer = OGL_Data->Layers->LastLayer;
    } else
        OGL_Data->Layers->SelectedLayer = OGL_Data->Layers->FirstLayer;

    return;
}


void OGL_NextLayerProc(TOGL_Data * OGL_Data)
/* go to the next layer */
{
    if (OGL_Data == NULL)
        return;
    if (OGL_Data->Layers->SelectedLayer) {
        if (OGL_Data->Layers->SelectedLayer < OGL_Data->Layers->LastLayer)
            OGL_Data->Layers->SelectedLayer++;
        else
            OGL_Data->Layers->SelectedLayer = OGL_Data->Layers->FirstLayer;
    } else
        OGL_Data->Layers->SelectedLayer = OGL_Data->Layers->FirstLayer;

    return;
}


void OGL_Play(TOGL_Data * OGL_Data)
/* used to flip through layers in endless loop */
{
    if (OGL_Data == NULL)
        return;
    if (OGL_Data->Layers->SelectedLayer) {
        if (OGL_Data->Layers->SelectedLayer < OGL_Data->Layers->LastLayer)
            OGL_Data->Layers->SelectedLayer++;
        else
            OGL_Data->Layers->SelectedLayer = OGL_Data->Layers->FirstLayer;
    } else
        OGL_Data->Layers->SelectedLayer = OGL_Data->Layers->FirstLayer;

    return;
}


/*
 *  Color manipulation functions
 */


ValNodePtr OGL_SearchPaletteIndex(ValNodePtr PaletteIndex,
                                  DDV_ColorCell * pColorCell)
{
    if (PaletteIndex == NULL || pColorCell == NULL)
        return NULL;
    for (; PaletteIndex; PaletteIndex = PaletteIndex->next)
        if (((TOGL_PaletteIndex *) (PaletteIndex->data.ptrvalue))->
            ColorCell.rgb[0] == pColorCell->rgb[0]
            && ((TOGL_PaletteIndex *) (PaletteIndex->data.ptrvalue))->
            ColorCell.rgb[1] == pColorCell->rgb[1]
            && ((TOGL_PaletteIndex *) (PaletteIndex->data.ptrvalue))->
            ColorCell.rgb[2] == pColorCell->rgb[2]) {
            return PaletteIndex;
        }
    return NULL;
}


/*
 *  The mouse and rotation/translation/zoom code
 */

static TOGL_Data *MAToOGL(MAPtr ma)
/* extracts OGL_Data out of the extra pointer in the mouse data */
{
    return (TOGL_Data *) MA_GetExtra(ma);
}


static TOGL_Data *PanelToOGL(Nlm_PaneL panel)
/* extract OGL_Data out of the panel data */
{
    MAPtr ma;
    Nlm_GetPanelExtra(panel, &ma);

    return MAToOGL(ma);
}


static void OGL_DrawViewer3D_CB(Nlm_PaneL panel)
/* callback */
{
    OGL_DrawViewer3D(PanelToOGL(panel));
}


/*
 *  Move
 */
static void OGL_Move3D(TOGL_Data * OGL_Data, Nlm_Int2 dx, Nlm_Int2 dy)
{
    GLint viewport[4];
    Nlm_FloatHi pixelSize;

    glGetIntegerv(GL_VIEWPORT, viewport);

    pixelSize =
        tan(OGL_Data->CameraAngle / 2.0) *
        2.0 * OGL_Data->CameraDistance /
        viewport[3];

    OGL_Data->CameraDirection[0] -= dx * pixelSize;
    OGL_Data->CameraDirection[1] += dy * pixelSize;

    OGL_Data->NeedCameraSetup = TRUE;
}


/*
 *  Zoom
 */
extern void Nlm_GetRect (Nlm_GraphiC a, Nlm_RectPtr r);

static void OGL_Zoom3D(TOGL_Data * OGL_Data, Nlm_Int2 x1, Nlm_Int2 y1,
                       Nlm_Int2 x2, Nlm_Int2 y2)
{
    Nlm_FloatHi zoom;
    Nlm_RecT rect;
    GLint viewport[4];

    /* translate window coords as passed to this function into coords
       relative to OpenGL area (the Panel) coords */
    Nlm_GetRect((Nlm_GraphiC)OGL_Data->Panel, &rect);
    x1 -= rect.left;
    x2 -= rect.left;
    y1 -= rect.top;
    y2 -= rect.top;

    /* set new camera direction ... */
    glGetIntegerv(GL_VIEWPORT, viewport);
    OGL_Move3D(OGL_Data,
        (viewport[2] - (x1 + x2)) / 2,
        (viewport[3] - (y1 + y2)) / 2);

    /* ... and angle. (This assumes zoom box aspect ratio is correct!) */
    zoom = ((Nlm_FloatHi) abs(y1 - y2)) / viewport[3];
    OGL_Data->CameraAngle = atan(zoom * tan(OGL_Data->CameraAngle));

    OGL_Data->NeedCameraSetup = TRUE;
}

NLM_EXTERN void OGL_ZoomOut(TOGL_Data *OGL_Data)
{
    OGL_Data->CameraAngle *= 1.5;
    OGL_Data->NeedCameraSetup = TRUE;
    OGL_DrawViewer3D(OGL_Data);
}

NLM_EXTERN void OGL_ZoomIn(TOGL_Data *OGL_Data)
{
    OGL_Data->CameraAngle /= 1.5;
    OGL_Data->NeedCameraSetup = TRUE;
    OGL_DrawViewer3D(OGL_Data);
}

/*
 *  Rotation
 */


typedef enum {
    ROTATE_X,
    ROTATE_Y,
    ROTATE_Z
} OGL_enumRotate3D;

typedef struct {
    OGL_enumRotate3D H;         /* horizontal dragging */
    OGL_enumRotate3D V;         /* vertical   dragging */
} OGL_RotatePivots3D, *OGL_RotatePivots3DPtr;


static void OGL_Rotate(TOGL_Data * OGL_Data, Nlm_Int4 dAngle,
                       OGL_enumRotate3D pivot)
{
#ifdef DEBUG_GL
    if (OGL_CheckForErrors()) Message(MSG_POST, "GL error entering OGL_Rotate");
#endif

    if (!dAngle)
        return;
    if (OGL_Data == NULL)
        return;

    glLoadIdentity();

    switch (pivot) {
    case ROTATE_X:
        glRotatef((GLfloat) (dAngle), 1.0f, 0.0f, 0.0f);
        break;
    case ROTATE_Y:
        glRotatef((GLfloat) (dAngle), 0.0f, 1.0f, 0.0f);
        break;
    case ROTATE_Z:
        glRotatef((GLfloat) (dAngle), 0.0f, 0.0f, 1.0f);
        break;
    }

    glMultMatrixd((GLdouble *) OGL_Data->ModelMatrix);

    glGetDoublev(GL_MODELVIEW_MATRIX, (GLdouble *) OGL_Data->ModelMatrix);

#ifdef DEBUG_GL
    if (OGL_CheckForErrors()) Message(MSG_POST, "GL error leaving OGL_Rotate");
#endif
}



static void OGL_ViewerRotate(Nlm_SlatE panel, Nlm_Int4 delta,
                             OGL_enumRotate3D pivot,
                             Nlm_Boolean adjust_scrollbar,
                             Nlm_Boolean redraw)
{
    TOGL_Data *OGL_Data;

    if (panel == NULL ||
        !Nlm_Visible(panel) || !Nlm_AllParentsVisible(panel)) return;

    OGL_Data = PanelToOGL((Nlm_PaneL) panel);

    if (adjust_scrollbar) {     /* adjust the relevant rotation scrollbar, if any */
        Nlm_BaR sbar = NULL;
        switch (pivot) {
        case ROTATE_X:
            sbar = Nlm_GetSlateVScrollBar(panel);
            break;
        case ROTATE_Y:
            sbar = Nlm_GetSlateHScrollBar(panel);
            break;
        case ROTATE_Z:
            sbar = OGL_Data->Z_rotate;
            break;
        }

        if (sbar) {
            Nlm_ResetClip();
            Nlm_CorrectBarValue(sbar,
                                (Nlm_GetValue(sbar) + delta + 360) % 360);
        }
    }

    /* the coordination transformation (rotation) */
    if (pivot == ROTATE_X)
        delta = -delta;
    OGL_Rotate(OGL_Data, delta, pivot);

    if (redraw) {               /* Draw the viewer */
        OGL_DrawViewer3D(OGL_Data);
    }

    /*  Nlm_DiagReset(); */
}



/* scrollbar callbacks */
static void OGL_ViewerVScrollProc(Nlm_BaR sb, Nlm_SlatE viewer,
                                  Nlm_Int2 newval, Nlm_Int2 oldval)
{
    OGL_ViewerRotate(viewer, newval - oldval, ROTATE_X, FALSE, TRUE);
}

static void OGL_ViewerHScrollProc(Nlm_BaR sb, Nlm_SlatE viewer,
                                  Nlm_Int2 newval, Nlm_Int2 oldval)
{
    OGL_ViewerRotate(viewer, newval - oldval, ROTATE_Y, FALSE, TRUE);
}

static void OGL_ViewerZScrollProc(Nlm_BaR sb, Nlm_GraphiC group,
                                  Nlm_Int2 newval, Nlm_Int2 oldval)
{
    Nlm_SlatE viewer = (Nlm_SlatE) Nlm_GetObjectExtra(sb);
    OGL_ViewerRotate(viewer, newval - oldval, ROTATE_Z, FALSE, TRUE);
}




/*
 *  MOUSE EVENT HANDLERS
 */



/*
 * MOVE
 */

static void OGL_Move_DrawTrace(MA_TracePtr trace)
{
    if (Nlm_EqualPt(trace->start, trace->end))
        return;

#ifdef WIN_MOTIF
    Nlm_SetColor(0xf1);
#endif
    Nlm_InvertMode();           /* turn on xor */
    Nlm_DrawLine(trace->start, trace->end); /* draw to current hdc */
    Nlm_CopyMode();             /* turn off xor */
}


static void OGL_Move_PressMA(MAPtr ma,
                             MA_TracePtr trace, Nlm_PoinT point,
                             Nlm_VoidPtr extra)
{
    trace->start = trace->end = point;
}


static void OGL_Move_DragMA(MAPtr ma,
                            MA_TracePtr trace, Nlm_PoinT point,
                            Nlm_VoidPtr extra)
{
    OGL_Move_DrawTrace(trace);
    trace->end = point;
    OGL_Move_DrawTrace(trace);
}


static void OGL_Move_ReleaseMA(MAPtr ma,
                               MA_TracePtr trace, Nlm_PoinT point,
                               Nlm_VoidPtr extra)
{
    OGL_Move_DrawTrace(trace);
    trace->end = point;

    if (Nlm_EqualPt(trace->start, trace->end))
        return;

    {                           /* do the move transform */
        TOGL_Data *OGL_Data = MAToOGL(ma);

        OGL_Move3D(OGL_Data, (Int2) (trace->end.x - trace->start.x),
                   (Int2) (trace->end.y - trace->start.y));
        OGL_DrawViewer3D(OGL_Data);

    }

    trace->start = trace->end;
}


static void OLG_Move_CancelMA(MAPtr ma,
                              MA_TracePtr trace, Nlm_PoinT point,
                              Nlm_VoidPtr extra)
{
    OGL_Move_DrawTrace(trace);
}


/*
 * ZOOM
 */

static void OGL_Zoom_DrawTrace(MA_TracePtr trace)
{
    Nlm_RecT rubber_box;
    if (Nlm_EqualPt(trace->start, trace->end))
        return;

#ifdef WIN_MOTIF
    Nlm_SetColor(0xf1);
#endif
    Nlm_InvertMode();
    Nlm_LoadRect(&rubber_box, trace->start.x, trace->start.y,
                 trace->end.x, trace->end.y);
    Nlm_FrameRect(&rubber_box); /* draw the frame */
    Nlm_CopyMode();
}

static void OGL_Zoom_PressMA(MAPtr ma,
                             MA_TracePtr trace, Nlm_PoinT point,
                             Nlm_VoidPtr extra)
{
    trace->start = trace->end = point;
}


/* constrain the rubber band shape as it's dragged to match the aspect
   ratio of the OpenGL region */
static void OGL_Zoom_DragMA(MAPtr ma,
                            MA_TracePtr trace, Nlm_PoinT point,
                            Nlm_VoidPtr extra)
{
    TOGL_Data *OGL_Data = MAToOGL(ma);
    GLint viewport[4];

    glGetIntegerv(GL_VIEWPORT, viewport);

    OGL_Zoom_DrawTrace(trace);
    if (point.y >= trace->start.y)
        point.y = trace->start.y +
            abs(trace->end.x - trace->start.x) *
            viewport[3] / viewport[2];
    else
        point.y = trace->start.y -
            abs(trace->end.x - trace->start.x) *
            viewport[3] / viewport[2];
    trace->end = point;
    OGL_Zoom_DrawTrace(trace);
}


static void OGL_Zoom_ReleaseMA(MAPtr ma,
                               MA_TracePtr trace, Nlm_PoinT point,
                               Nlm_VoidPtr extra)
{
    OGL_Zoom_DrawTrace(trace);

    if (Nlm_EqualPt(trace->start, trace->end))
        return;

    {                           /* do the zoom */
        TOGL_Data *OGL_Data = MAToOGL(ma);

        OGL_Zoom3D(OGL_Data, trace->start.x, trace->start.y,
                   trace->end.x, trace->end.y);
        OGL_DrawViewer3D(OGL_Data);

    }
}

static void OGL_Zoom_CancelMA(MAPtr ma,
                              MA_TracePtr trace, Nlm_PoinT point,
                              Nlm_VoidPtr extra)
{
    OGL_Zoom_DrawTrace(trace);
}


/*
 * ROTATE
 */


static void OGL_Rotate_PressMA(MAPtr ma,
                               MA_TracePtr trace, Nlm_PoinT point,
                               Nlm_VoidPtr extra)
{
    trace->start = point;
}



static void OGL_Rotate_DragMA(MAPtr ma,
                              MA_TracePtr trace, Nlm_PoinT point,
                              Nlm_VoidPtr extra)
{
    TOGL_Data *OGL_Data;
    GLint viewport[4];

    OGL_RotatePivots3DPtr pivot;
    if (Nlm_EqualPt(trace->start, point))
        return;

    OGL_Data = MAToOGL(ma);
    pivot = (OGL_RotatePivots3DPtr) extra;

    glGetIntegerv(GL_VIEWPORT, viewport);

    OGL_ViewerRotate((Nlm_SlatE) OGL_Data->Panel,
                     (Int4) ((180.0 * (point.x - trace->start.x)) /
                             viewport[2]), pivot->H, TRUE, FALSE);

    OGL_ViewerRotate((Nlm_SlatE) OGL_Data->Panel,
                     (Int4) ((180.0 * (trace->start.y - point.y)) /
                             viewport[3]), pivot->V, TRUE, TRUE);
    trace->start = point;
}


/*
 * RESET
 */

static void OGL_ResetMA(MAPtr ma,
                        MA_TracePtr trace, Nlm_PoinT point,
                        Nlm_VoidPtr extra)
{
    VERIFY(MA_UnsetAll(ma));
}


static Nlm_Boolean OGL_SetStdMouse(TOGL_Data * OGL_Data,
                                   Nlm_enumStdMAOGL action)
{
    if (OGL_Data == NULL)
        return FALSE;
    return MA_SetGroup(OGL_Data->ma_std_group[action]);
}



/* Initialize MA for the viewer
*/

static Nlm_Boolean OGL_InitializeMA(TOGL_Data * OGL_Data)
{
    MAPtr ma = OGL_Data->ma;

    /* rotate */
    MActionPtr rotate_press =
        MA_AddAction(ma, MK_Normal, MA_Press, OGL_Rotate_PressMA, NULL,
                     NULL);

    static OGL_RotatePivots3D RotateDrag_YX = { ROTATE_Y, ROTATE_X };
    MActionPtr rotate_drag_YX =
        MA_AddAction(ma, MK_Normal, MA_Drag, OGL_Rotate_DragMA,
                     &RotateDrag_YX, NULL);
    MA_GroupPtr rotate_group_YX = MA_AddGroup(ma, "Rotate_YX",
                                              rotate_press, MA_ONLY,
                                              rotate_drag_YX, MA_ONLY,
                                              NULL);

    static OGL_RotatePivots3D RotateDrag_ZX = { ROTATE_Z, ROTATE_X };
    MActionPtr rotate_drag_ZX =
        MA_AddAction(ma, MK_Normal, MA_Drag, OGL_Rotate_DragMA,
                     &RotateDrag_ZX, NULL);
    MA_GroupPtr rotate_group_ZX = MA_AddGroup(ma, "Rotate_ZX",
                                              rotate_press, MA_ONLY,
                                              rotate_drag_ZX, MA_ONLY,
                                              NULL);

    static OGL_RotatePivots3D RotateDrag_YZ = { ROTATE_Y, ROTATE_Z };
    MActionPtr rotate_drag_YZ =
        MA_AddAction(ma, MK_Normal, MA_Drag, OGL_Rotate_DragMA,
                     &RotateDrag_YZ, NULL);
    MA_GroupPtr rotate_group_YZ = MA_AddGroup(ma, "Rotate_YZ",
                                              rotate_press, MA_ONLY,
                                              rotate_drag_YZ, MA_ONLY,
                                              NULL);

    /* move */
    MActionPtr move_press =
        MA_AddAction(ma, MK_Shift, MA_Press, OGL_Move_PressMA, NULL, NULL);
    MActionPtr move_drag =
        MA_AddAction(ma, MK_Shift, MA_Drag, OGL_Move_DragMA, NULL, NULL);
    MActionPtr move_release =
        MA_AddAction(ma, MK_Shift, MA_Release, OGL_Move_ReleaseMA, NULL,
                     NULL);
    MActionPtr move_cancel =
        MA_AddAction(ma, MK_Shift, MA_Cancel, OLG_Move_CancelMA, NULL,
                     NULL);

    MA_GroupPtr move_group = MA_AddGroup(ma, "Move",
                                         move_press, MA_ONLY,
                                         move_drag, MA_ONLY,
                                         move_release, MA_ONLY,
                                         move_cancel, MA_ONLY,
                                         NULL);

    /* zoom */
    MActionPtr zoom_press =
        MA_AddAction(ma, MK_Ctrl, MA_Press, OGL_Zoom_PressMA, NULL, NULL);
    MActionPtr zoom_drag =
        MA_AddAction(ma, MK_Ctrl, MA_Drag, OGL_Zoom_DragMA, NULL, NULL);
    MActionPtr zoom_release =
        MA_AddAction(ma, MK_Ctrl, MA_Release, OGL_Zoom_ReleaseMA, NULL,
                     NULL);
    MActionPtr zoom_cancel =
        MA_AddAction(ma, MK_Ctrl, MA_Cancel, OGL_Zoom_CancelMA, NULL,
                     NULL);

    MA_GroupPtr zoom_group = MA_AddGroup(ma, "Zoom",
                                         zoom_press, MA_ONLY,
                                         zoom_drag, MA_ONLY,
                                         zoom_release, MA_ONLY,
                                         zoom_cancel, MA_ONLY,
                                         NULL);

    /* miscellaneous actions */
/*
 this is done in the main program.  move it here after deleting viewer3d
    MActionPtr bg_hl_dclick =
        MA_AddAction(ma, MK_Normal, MA_DClick,  NULL, NULL,
        "Highlight-Prim or Background");
*/

    /* this group disables all mouse actions when set */
    MActionPtr reset_init =
        MA_AddAction(ma, MK_Normal, MA_Init, OGL_ResetMA, NULL, NULL);

    MA_GroupPtr reset_group = MA_AddGroup(ma, "No Action",
                                          reset_init, MA_SHARED,
                                          NULL);

    if (OGL_Data == NULL)
        return FALSE;

    { {                         /* "No-Action"s */
            int i, j;
            for (i = 0; i < MK_Default; i++)
                for (j = 0; j < MA_Init; j++) {
                    VERIFY(MA_AddAction(ma, (enumMKey) i, (enumMAction) j,
                                        DoNothingMA, NULL, "No Action"));
                }
    }
    }

    /* register the set of standard 3D-viewer groups */
    OGL_Data->ma_std_group[MouseOGL_DoNothing] = reset_group;
    OGL_Data->ma_std_group[MouseOGL_RotateYX] = rotate_group_YX;
    OGL_Data->ma_std_group[MouseOGL_RotateZX] = rotate_group_ZX;
    OGL_Data->ma_std_group[MouseOGL_RotateYZ] = rotate_group_YZ;
    OGL_Data->ma_std_group[MouseOGL_Move] = move_group;
    OGL_Data->ma_std_group[MouseOGL_Zoom] = zoom_group;

    /* Test, Setup defaults and Link viewer panel to MA */
    if (!rotate_press ||
        !rotate_drag_YX || !rotate_group_YX ||
        !rotate_drag_ZX || !rotate_group_ZX ||
        !rotate_drag_YZ || !rotate_group_YZ ||
        !move_press || !move_drag || !move_release ||
        !move_cancel || !move_group ||
        !zoom_press || !zoom_drag || !zoom_release ||
        !zoom_cancel || !zoom_group ||
/*        !bg_hl_dclick    ||*/
        !reset_group ||
        !OGL_SetStdMouse(OGL_Data, MouseOGL_RotateYX) ||
        !OGL_SetStdMouse(OGL_Data, MouseOGL_Move) ||
        !OGL_SetStdMouse(OGL_Data, MouseOGL_Zoom) ||
/*        !MA_SetAction(bg_hl_dclick, FALSE)  ||*/
        !MA_LinkPanel(ma, OGL_Data->Panel)) {
        MA_Reset(ma);
        return FALSE;
    }

    return TRUE;
}


/*
 *  Doing selection in OpenGL
 */

void OGL_Select(TOGL_Data * OGL_Data, Nlm_Boolean SelectMode)
{
    if (OGL_Data == NULL)
        return;
    OGL_Data->SelectMode = SelectMode;
    return;
}


void OGL_LoadName(Nlm_VoidPtr PtrValue)
/* load a pointer onto the name stack.  compensate for possible long long */
{
    Nlm_Int4 i;

    for (i = 0; i < sizeof(Nlm_VoidPtr) / sizeof(GLuint); i++)
        glPopName();

    for (i = 0; i < sizeof(Nlm_VoidPtr) / sizeof(GLuint); i++)
        glPushName((GLuint) (((long) PtrValue) >> (i * sizeof(GLuint) * 8))); /* 64 bits? */

    OGL_CurrentName = PtrValue;
}


Nlm_VoidPtr OGL_Hit(TOGL_Data * OGL_Data)
/* this function looks through the hit stack and extracts the nearest hit */
{
    GLuint *Hits, nNames, hit, p = 0, j, ZMin;
    long ZMinName = 0;          /* this is a hack, but should work for 64 bits */

    if (OGL_Data == NULL || OGL_Data->SelectBuffer == NULL)
        return NULL;

    Hits = (GLuint *) OGL_Data->SelectBuffer;
    for (hit=0; hit < OGL_Data->SelectHits; hit++) { /* loop over all hits */
        nNames = Hits[p];
        p++; /* move to zmin */
        /* look for *minimum* depth - that's the top object on the screen */
        if (hit == 0 || Hits[p] < ZMin) {
            ZMin = Hits[p];
            p += 2; /* skip over zmax to name stack */
            ZMinName = 0;
            /* currently only looks at the top of the name stack */
            for (j = 0; j < sizeof(Nlm_VoidPtr) / sizeof(GLuint); j++) {
                ZMinName = ZMinName << (sizeof(GLuint) * 8);
                ZMinName |= Hits[p + j];
            }
            p += nNames * sizeof(Nlm_VoidPtr) / sizeof(GLuint);
        } else
            p += nNames * sizeof(Nlm_VoidPtr) / sizeof(GLuint) + 2;
    }
    return (Nlm_VoidPtr) ZMinName;
}


void OGL_SetSelectPoint(TOGL_Data * OGL_Data, Nlm_PoinT Point)
{
    Nlm_RecT rect;

    if (OGL_Data == NULL)
        return;

    OGL_Data->SelectPoint.x = Point.x;
    OGL_Data->SelectPoint.y = Point.y;

    /* translate window coords as passed to this function into coords
       relative to OpenGL area (the Panel) coords */
    Nlm_GetRect((Nlm_GraphiC)OGL_Data->Panel, &rect);
    OGL_Data->SelectPoint.x -= rect.left;
    OGL_Data->SelectPoint.y -= rect.top;

#ifdef WIN_MSWIN
    /* For some reason, windows requires a fudge to get the mouse pointer
       to be more accurate */
    OGL_Data->SelectPoint.x += 1;
    OGL_Data->SelectPoint.y -= 4;
#endif
}


/*
 *  functions used to create, do, and finish drawing
 */


static void OGL_DeleteViewer3D(TOGL_Data * OGL_Data)
/* delete OGL_Data */
{
    if (OGL_Data == NULL)
        return;

    MA_Destroy(OGL_Data->ma);

    /* to do: someone else is getting rid of Panel
       if ( OGL_Data->Panel )
       Nlm_Remove( OGL_Data->Panel );
     */

    /* delete the display lists */
#ifndef MESA
    /* There's some bug (?) in Mesa that causes a crash here, maybe because
       we're trying to delete non-existent lists? Dunno... */
    glDeleteLists(OGL_Data->Layers->FirstLayer - 1, OGLMAXLAYERS);
#endif

    /* free items on the heap */
    MemFree(OGL_Data->Layers);
    MemFree(OGL_Data->ModelMatrix);
    MemFree(OGL_Data);
}


static void OGL_ResetViewerProc_CB(Nlm_PaneL panel)
{
    TOGL_Data *OGL_Data = PanelToOGL(panel);
    if (!OGL_Data)
        return;

    OGL_Data = NULL;
    OGL_DeleteViewer3D(PanelToOGL(panel));
}


typedef struct {
#ifdef WIN_MAC
    int familyID;
#else
    char *name;
#endif
} OGLFontListItem;

#define MAXFONTS 1000
static OGLFontListItem OGLFontList[MAXFONTS]; /* should really be dynamic... */
static int nFonts = 0;

#if defined(WIN32)
int CALLBACK Nlm_FindFontCB(
  ENUMLOGFONTEX *lpelfe,
  NEWTEXTMETRICEX *lpntme,
  DWORD FontType,
  LPARAM lParam )
{
    OGLFontList[nFonts].name = strdup(lpelfe->elfLogFont.lfFaceName);
    Nlm_PopupItem((Nlm_PopuP)lParam, OGLFontList[nFonts].name);
    nFonts++;
    if (nFonts == MAXFONTS)
        return 0;
    else
        return 1;
}

#elif defined(WIN_MOTIF)
static char *standardXFont = "9x15";
#endif

/* called during app initialization, to set up font family menu */
NLM_EXTERN void Nlm_FindAvailableFonts(Nlm_PopuP pupmenu)
{
#if defined(WIN32)
    LOGFONT lf;
    HDC hdc = wglGetCurrentDC();
    /* to enumerate all styles of all fonts for the ANSI character set */
    lf.lfFaceName[0] = '\0';
    lf.lfCharSet = ANSI_CHARSET;
    lf.lfPitchAndFamily = 0;
    EnumFontFamiliesEx((HDC)hdc, (LPLOGFONT)&lf, (FONTENUMPROC)Nlm_FindFontCB,
                        (LPARAM)pupmenu, (DWORD)0);

#elif defined(WIN_MAC)
	int i;
	char name[255];

#define ADD_FONT(str,ID) do { \
                           OGLFontList[nFonts].familyID = (ID); \
                           Nlm_PopupItem(pupmenu, (str)); \
                           nFonts++; \
                           if (nFonts == MAXFONTS) return; \
                         } while (0)

	/* There's gotta be a better way to get a list of
	   available fonts... this takes forever even on a fast mac! */
	for (i=2; i<=16383; i++) {
		if (i==256) i=1024;
		if (!RealFont(i,12)) continue;
		memset(name, '\0', 255);
		GetFontName(i, (unsigned char *)name);
		if (name[0] != '\0') {
		  ADD_FONT(name+1, i); /* for some reason, name starts at 2nd character? */
		}
	}

#elif defined(WIN_MOTIF)
    extern Display *Nlm_currentXDisplay;
    char **list, fnd_fam[256], *p;
    int i, j, tot;
    XFontStruct *xfs;

#define ADD_FONT(str) do { \
                        OGLFontList[nFonts].name = strdup((str)); \
                        Nlm_PopupItem(pupmenu, (str)); \
                        nFonts++; \
                        if (nFonts == MAXFONTS) return; \
                      } while (0);

    xfs = XLoadQueryFont(Nlm_currentXDisplay, standardXFont);
    if (xfs) {
        ADD_FONT(standardXFont);
        XFreeFont(Nlm_currentXDisplay, xfs);
    }
    list = XListFonts(Nlm_currentXDisplay,
                      "-*-*-*-*-*-*-*-*-*-*-*-*-*-*",
                      100000, &tot);
    for (i=0; i<tot; i++) {
        strncpy(fnd_fam,list[i]+1,256);
        p = strchr(fnd_fam,'-');
        if (!p) continue;
        p = strchr(p+1,'-');
        if (!p) continue;
        *p = '\0';
        /* use foundry-family pair (i.e., adobe-courier)
           as "name" of font to store */
        for (j=0; j<nFonts; j++) {
            if (strcmp(OGLFontList[j].name,fnd_fam) == 0) break;
        }
        if (j == nFonts) ADD_FONT(fnd_fam);
    }
    XFreeFontNames(list);
#endif
}

NLM_EXTERN void SetOGLFont(TOGL_Data *OGL_Data, Nlm_Int2 fontNameIndex, Nlm_Int2 fontSize,
    Nlm_Boolean isBold, Nlm_Boolean isItalic, Nlm_Boolean isUnderlined)
{
    static Nlm_Int2 currentIndex = -1, currentSize = -1;
    static Nlm_Boolean currentBold = FALSE, currentItalic = FALSE,
                       currentUnderlined = FALSE;

    fontNameIndex--; /* vibrant menus start at one */
    if (fontNameIndex == currentIndex && currentSize == fontSize &&
        isBold == currentBold && isItalic == currentItalic &&
        isUnderlined == currentUnderlined)
        return; /* only switch fonts if necessary */
    currentIndex = fontNameIndex;
    currentSize = fontSize;
    currentBold = isBold;
    currentItalic = isItalic;
    currentUnderlined = isUnderlined;

#if defined(WIN32)
    {
        HDC hdc;
        HGDIOBJ currentFont;
        static HGDIOBJ newFont = NULL;
        SIZE TextSize;

        /* delete font used in previous call */
        if (newFont) DeleteObject(newFont);

        hdc = wglGetCurrentDC();
        newFont =
            CreateFont(
                -MulDiv(currentSize, GetDeviceCaps(hdc, LOGPIXELSY), 72),
                0, 0, 0, currentBold ? FW_BOLD : FW_NORMAL,
                currentItalic ? TRUE : FALSE,
                currentUnderlined ? TRUE : FALSE, FALSE, ANSI_CHARSET,
                OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY,
                FF_DONTCARE, (LPCTSTR) OGLFontList[currentIndex].name
            );
        currentFont = SelectObject(hdc, newFont);
        wglUseFontBitmaps(hdc, 0, 255, OGLFONTBASE);

        /* Get the size of an average character */
        GetTextExtentPoint32(hdc, "A", 1, &TextSize);
        OGL_Data->SpaceWidth = TextSize.cx;
        OGL_Data->SpaceHeight = TextSize.cy;

        /* restore previous font */
        SelectObject(hdc, currentFont);
    }
#elif defined(WIN_MAC)
	{
    	FontInfo fInfo;
    	GrafPtr currentPort;
    	GLint ID = OGLFontList[currentIndex].familyID;
    	Style face = 0;

		if (!RealFont(ID, currentSize))
			return;

		if (isBold) face |= bold;
		if (isItalic) face |= italic;
		if (isUnderlined) face |= underline;

    	aglUseFont(aglGetCurrentContext(),
                   ID, face, currentSize,
    	           0, 256, OGLFONTBASE);

    	GetPort(&currentPort); /* store the current port's font info */
    	TextFont(ID);
    	TextFace(face);
    	TextSize(currentSize);
        OGL_Data->SpaceWidth = CharWidth('A');
        GetFontInfo(&fInfo);
        OGL_Data->SpaceHeight = fInfo.ascent + fInfo.descent;
        TextFont(GetPortTextFont(currentPort)); /* restore previous font */
        TextFace(GetPortTextFace(currentPort));
        TextSize(GetPortTextSize(currentPort));
    }

#elif defined(WIN_MOTIF)
    {
        static XFontStruct *fontInfo = NULL;
        extern Display *Nlm_currentXDisplay;
        char query[256], **list, *pos;
        int nFound, ii, ib, size, diff, mindiff, closest;
        static const char
            *bold[] = { "bold", "black", NULL },
            *unbold[] = { "medium", "regular", NULL }, **boldSel,
            *ital[] = { "i", "o", NULL },
            *unital[] = { "r", NULL }, **italSel;

        if (fontInfo) XFreeFont(Nlm_currentXDisplay, fontInfo);

        if (strcmp(OGLFontList[currentIndex].name, standardXFont) == 0) {
            fontInfo = XLoadQueryFont(Nlm_currentXDisplay, standardXFont);
        } else {

            /* first find fonts with right typeface; bold, italic if possible */
            boldSel = currentBold ? bold : unbold;
            italSel = currentItalic ? ital : unital;
            for (ib = 0; boldSel[ib] ; ib++) {
                for (ii = 0; italSel[ii] ; ii++) {
                    sprintf(query,"-%s-%s-%s-*-*-*-*-*-*-*-*-*-*",
                            OGLFontList[currentIndex].name,
                            boldSel[ib], italSel[ii]);
                    list = XListFonts(Nlm_currentXDisplay, query, 1000, &nFound);
                    if (nFound) break;
                }
                if (nFound) break;
            }
            if (!nFound) { /* can't find any of this style; use any available */
                sprintf(query,"-%s-*-*-*-*-*-*-*-*-*-*-*-*",
                        OGLFontList[currentIndex].name);
                list = XListFonts(Nlm_currentXDisplay, query, 1000, &nFound);
            }
            /* now find closest pixelsize to that requested */
            for (ii = 0; ii < nFound; ii++) {
                /* find start of pixelsize field of font string */
                pos = list[ii];
                for (ib = 0; ib < 6; ib++) pos = strchr(pos+1, '-');
                pos++;
                /* store closest size match to requested */
                size = atoi(pos);
                /* use 2 * menu size setting as pixel size */
                diff = abs(size - currentSize*2);
                if (ii == 0 || diff < mindiff) {
                    mindiff = diff;
                    closest = ii;
                }
            }
            fontInfo = XLoadQueryFont(Nlm_currentXDisplay, list[closest]);
            XFreeFontNames(list);
        }

        if (fontInfo->per_char == NULL) {
            OGL_Data->SpaceWidth = fontInfo->max_bounds.width;
            OGL_Data->SpaceHeight = fontInfo->max_bounds.ascent +
                                    fontInfo->max_bounds.descent;
        } else {
            XCharStruct *charA = &(fontInfo->per_char['A']);
            OGL_Data->SpaceWidth = charA->width;
            OGL_Data->SpaceHeight = charA->ascent + charA->descent;
        }

        glXUseXFont(fontInfo->fid,
                    fontInfo->min_char_or_byte2,
                    fontInfo->max_char_or_byte2 -
                      fontInfo->min_char_or_byte2 + 1,
                    OGLFONTBASE + fontInfo->min_char_or_byte2);
    }
#endif
}


TOGL_Data *OGL_CreateViewer(Nlm_GrouP prnt,
                            Uint2Ptr width, Uint2 height,
                            Int4 flags,
                            Nlm_MenU ma_group_menu,
                            Nlm_MenU ma_action_menu,
                            Nlm_MAInitOGLFunc ma_init_func,
                            VoidPtr ma_init_data)
                             /* initialize the OpenGL library */
{
    TOGL_Data *OGL_Data;
    Nlm_Uint2 x_width;

    OGL_Data = (TOGL_Data *) MemNew(sizeof(TOGL_Data));
    if (OGL_Data == NULL)
        return NULL;

    OGL_Data->ModelMatrix = (Nlm_VoidPtr) MemNew(16 * sizeof(GLdouble));
    if (OGL_Data->ModelMatrix == NULL)
        return NULL;
    OGL_Data->NeedCameraSetup = FALSE;

    OGL_Data->Layers = (TOGL_Layers *) MemNew(sizeof(TOGL_Layers));
    if (OGL_Data->Layers == NULL)
        return NULL;

    OGL_Data->Layers->SelectedLayer = 0;
    OGL_SetLayers(OGL_Data, FALSE); /* null all the layers out */

    OGL_Data->IsPlaying = FALSE; /* animation off */
    OGL_Data->Tick = 0.01;
    OGL_Data->ParentWindow = Nlm_ParentWindow((Nlm_Handle) prnt);
    OGL_Data->PaletteExpanded = NULL;
    OGL_Data->PaletteIndex = NULL;

    OGL_Data->SelectMode = FALSE;
    OGL_Data->SelectBuffer =
        (Nlm_VoidPtr) MemNew(OGLSELECTBUFFER * sizeof(GLuint));
    if (OGL_Data->SelectBuffer == NULL)
        return NULL;

    OGL_Data->Panel = Nlm_Autonomous3DPanel(prnt,
                                            (Int2) * width, (Int2) height,
                                            OGL_DrawViewer3D_CB,
                                            ((flags & Y_ROTATE_SBAR) ?
                                             OGL_ViewerVScrollProc : NULL),
                                            ((flags & X_ROTATE_SBAR) ?
                                             OGL_ViewerHScrollProc : NULL),
                                            sizeof(MAPtr),
                                            OGL_ResetViewerProc_CB, NULL,
                                            &OGL_Data->IndexMode, &OGL_Data->display,
                                            &OGL_Data->visinfo);


    if (flags & Z_ROTATE_SBAR) {
        OGL_Data->Z_rotate =
            Nlm_ScrollBar(prnt, 1, 0, OGL_ViewerZScrollProc);
        if (!OGL_Data->Z_rotate) {
            MemFree(OGL_Data);
            if (OGL_Data->Panel)
                Nlm_Remove(OGL_Data->Panel);
            return NULL;
        }
        Nlm_SetObjectExtra(OGL_Data->Z_rotate, OGL_Data->Panel, NULL);
    }
     {
        Nlm_RecT rect;
        Nlm_GetPosition(OGL_Data->Panel, &rect);
        rect.right = (Int2) (rect.left + *width);
        rect.bottom = (Int2) (rect.top + height);
        OGL_SetPosition3D(OGL_Data, &rect);
        x_width = (Uint2) (rect.right - rect.left);
    }

    if (flags & X_ROTATE_SBAR) {
        Nlm_BaR sb = Nlm_GetSlateHScrollBar((Nlm_SlatE) OGL_Data->Panel);
        Nlm_CorrectBarValue(sb, 0);
        Nlm_SetRange(sb, 10, 10, 360);
    }
    if (flags & Y_ROTATE_SBAR) {
        Nlm_BaR sb = Nlm_GetSlateVScrollBar((Nlm_SlatE) OGL_Data->Panel);
        Nlm_CorrectBarValue(sb, 0);
        Nlm_SetRange(sb, 10, 10, 360);
    }
    if (flags & Z_ROTATE_SBAR) {
        Nlm_SetRange(OGL_Data->Z_rotate, 10, 10, 360);
        Nlm_CorrectBarValue(OGL_Data->Z_rotate, 180);
    }

    OGL_Data->ma = MA_Create(ma_group_menu, ma_action_menu);
    MA_SetExtra(OGL_Data->ma, OGL_Data);

    if (!OGL_InitializeMA(OGL_Data)) {
        MemFree(OGL_Data);
        if (OGL_Data->Z_rotate)
            Nlm_Remove(OGL_Data->Z_rotate);
        if (OGL_Data->Panel)
            Nlm_Remove(OGL_Data->Panel);
        return NULL;
    }

    if (ma_init_func && !(*ma_init_func) (OGL_Data->ma, ma_init_data)) {
        MemFree(OGL_Data);
        if (OGL_Data->Z_rotate)
            Nlm_Remove(OGL_Data->Z_rotate);
        if (OGL_Data->Panel)
            Nlm_Remove(OGL_Data->Panel);
        return NULL;
    }

    *width = x_width;

    if (!OGL_qobj) {
        /* allocate quadric object (once only) */
        OGL_qobj = gluNewQuadric();
        if (!OGL_qobj) return NULL;
        gluQuadricDrawStyle(OGL_qobj, GLU_FILL);
        gluQuadricNormals(OGL_qobj, GLU_SMOOTH);
        gluQuadricOrientation(OGL_qobj, GLU_OUTSIDE);
    }

    return OGL_Data;
}

void OGL_InitializeLists(TOGL_Data * OGL_Data)
{
    OGL_Data->Layers->FirstLayer = glGenLists((GLsizei) OGLMAXLAYERS);
    OGL_Data->Layers->FirstLayer++; /* avoid weird bug in windows OpenGL */
    OGL_Data->Layers->LastLayer = OGL_Data->Layers->FirstLayer;
}

#ifdef WIN_MAC
static Nlm_Boolean drawingOffscreen = FALSE;
#endif

void OGL_DrawViewer3D(TOGL_Data * OGL_Data)
/* does the drawing */
{
    GLint Viewport[4];
    static GLint prevW = -1, prevH = -1;
    GLfloat LightPosition[4], aspect;
    Nlm_Int4 TotalColors;
    Nlm_Uint4 iList;
    int i;

#ifdef DEBUG_GL
    if (OGL_CheckForErrors()) Message(MSG_POST, "GL error entering DrawViewer3D");
#endif

    if (OGL_Data == NULL) return;

#ifdef WIN_MOTIF
    /* need to know if in color index mode - i.e., after context established */
    if (!glXGetCurrentContext()) return;
#endif

#ifdef WIN_MAC
	if (drawingOffscreen) {
        aglDisable(aglGetCurrentContext(), AGL_BUFFER_RECT);
    } else {
        /* HACK to limit opengl to correct region of the (Cn3D) window */
        GLint wrect[4];
        Nlm_RecT r;
        AGLContext ctx = aglGetCurrentContext();
        Nlm_GetRect((Nlm_GraphiC) OGL_Data->Panel, &r);
        wrect[0] = r.left;
        wrect[1] = r.top - Nlm_hScrollBarHeight  - 4; /* this last column is simply tweaks */
        wrect[2] = r.right - r.left + 1          - 2; /* to make window position look nice */
        wrect[3] = r.bottom - r.top + 1          - 3;
        aglSetInteger(ctx, AGL_BUFFER_RECT, wrect);
        aglEnable(ctx, AGL_BUFFER_RECT);
    }
#endif

    /* set background color */
    if (OGL_Data->IndexMode) {
        TotalColors = ValNodeLen(OGL_Data->PaletteExpanded);
#ifdef WIN32
        glClearIndex((GLfloat) (TotalColors + 10));
#else
        glClearIndex((GLfloat) TotalColors);
#endif

    } else {
        glClearColor(1.0*OGL_Data->Background.rgb[0]/255, 
                     1.0*OGL_Data->Background.rgb[1]/255, 
                     1.0*OGL_Data->Background.rgb[2]/255, 1.0);
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glGetIntegerv(GL_VIEWPORT, Viewport);

    /* only do this (expensive) stuff if we have to */
    if (Viewport[2] != prevW || Viewport[3] != prevH ||
        OGL_Data->SelectMode || OGL_Data->NeedCameraSetup) {

        prevW = Viewport[2];
        prevH = Viewport[3];
        if (OGL_Data->SelectMode)
            OGL_Data->NeedCameraSetup = TRUE; /* will need to redo camera next time */
        else
            OGL_Data->NeedCameraSetup = FALSE;

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        if (OGL_Data->SelectMode) {
            glSelectBuffer((GLsizei) OGLSELECTBUFFER,
                           (GLuint *) OGL_Data->SelectBuffer);
            glRenderMode(GL_SELECT);
            glInitNames();
            for (i = 0; i < sizeof(Nlm_VoidPtr) / sizeof(GLuint); i++)
                glPushName(0);
            gluPickMatrix((GLdouble) (OGL_Data->SelectPoint.x),
                          (GLdouble) (Viewport[3] - OGL_Data->SelectPoint.y),
                          1.0, 1.0, Viewport);
        }

        aspect = ((GLfloat)(Viewport[2])) / Viewport[3];
        gluPerspective(OGL_Data->CameraAngle * 180.0/acos(-1), /* viewing angle (degrees) */
                       aspect,                                 /* w/h aspect    */
                       0.5 * OGL_Data->CameraDistance,         /* near clipping plane */
                       1.5 * OGL_Data->CameraDistance);        /* far clipping plane  */
        gluLookAt(0.0,0.0,OGL_Data->CameraDistance,  /* the camera position */
                  OGL_Data->CameraDirection[0],      /* the "look-at" point */
                  OGL_Data->CameraDirection[1],
                  0.0,
                  0.0,1.0,0.0);                      /* the up direction */

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        /* set up the lighting */
        LightPosition[0] = 0.0; /* same as eye */
        LightPosition[1] = 0.0;
        LightPosition[2] = OGL_Data->CameraDistance;
        LightPosition[3] = 0.0;  /* directional light (faster) when 0.0 */
        glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);


        if (OGL_Data->IndexMode == FALSE) {
            glLightfv(GL_LIGHT0, GL_AMBIENT, Color_Off);
            glLightfv(GL_LIGHT0, GL_DIFFUSE, Color_MostlyOn);
            glLightfv(GL_LIGHT0, GL_SPECULAR, Color_Off);

            /* clear these material colors */
            glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, Color_Off);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Color_Off);

        } else { /* color index mode */
            glLightfv(GL_LIGHT0, GL_AMBIENT, Color_Off);
            glLightfv(GL_LIGHT0, GL_DIFFUSE, Color_On);
            glLightfv(GL_LIGHT0, GL_SPECULAR, Color_Off);
        }

        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, Color_On); /* global ambience */
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);

        glShadeModel(GL_SMOOTH);
        glEnable(GL_DEPTH_TEST);

        /* turn on culling to speed rendering */
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);
    }

    glLoadIdentity();
    glMultMatrixd((GLdouble *) OGL_Data->ModelMatrix);

    /* selectively display the layers */
    if (OGL_Data->Layers->SelectedLayer) {
        glCallList(OGL_Data->Layers->SelectedLayer);
    } else {
        for (iList = OGL_Data->Layers->FirstLayer;
             iList <= OGL_Data->Layers->LastLayer; iList++) {
            if (OGL_GetLayer(OGL_Data, iList - OGL_Data->Layers->FirstLayer)) {
                glCallList(iList);
            }
        }
    }

    /* display transparent spheres */
    if (!OGL_Data->IndexMode) {
        OGL_SetColor(OGL_Data, NULL, GL_NONE, 1.0); /* clear all color states */
        OGL_RenderTransparentSpheres(OGL_Data);
    }

    OGL_CheckForErrors(); /* check GL error status */

    glFlush();

    if (OGL_Data->SelectMode)
        OGL_Data->SelectHits = glRenderMode(GL_RENDER);

    else { /* regular rendering mode */
        /* swap when double buffering */
#if defined(WIN32)
        wglSwapLayerBuffers(wglGetCurrentDC(), WGL_SWAP_MAIN_PLANE);/**/
        /*SwapBuffers(wglGetCurrentDC());*/
#elif defined(WIN_MOTIF)
        glXSwapBuffers((Display *) OGL_Data->display, glXGetCurrentDrawable());
#elif defined(WIN_MAC)
        aglSwapBuffers(aglGetCurrentContext());
#endif

    }

#ifdef DEBUG_GL
    if (OGL_CheckForErrors()) Message(MSG_POST, "GL error leaving DrawViewer3D");
#endif
}


void OGL_Reset(TOGL_Data * OGL_Data)
/* reset the transform matrix */
{
    Nlm_FloatLo diff;

    if (OGL_Data == NULL)
        return;

    OGL_Data->MaxSize = (Nlm_FloatLo)
        fabs(OGL_Data->BoundBox.x[0] - OGL_Data->BoundBox.x[1]);
    diff = fabs(OGL_Data->BoundBox.y[0] - OGL_Data->BoundBox.y[1]);
    if (diff > OGL_Data->MaxSize)
        OGL_Data->MaxSize = diff;
    diff = fabs(OGL_Data->BoundBox.z[0] - OGL_Data->BoundBox.z[1]);
    if (diff > OGL_Data->MaxSize)
        OGL_Data->MaxSize = diff;

    /* set up the initial view point */
    OGL_Data->CameraDistance = (GLfloat) 5.0 *OGL_Data->MaxSize;
    OGL_Data->CameraDirection[0] = 0.0;
    OGL_Data->CameraDirection[1] = 0.0;
    OGL_Data->CameraAngle = 2.0 *
        atan((fabs(OGL_Data->BoundBox.y[0] - OGL_Data->BoundBox.y[1]) / 2.0) /
              OGL_Data->CameraDistance);
    OGL_Data->NeedCameraSetup = TRUE;

    /* translate model so origin is at bounding box center */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(
        (GLfloat) -((OGL_Data->BoundBox.x[0] + OGL_Data->BoundBox.x[1]) / 2.0),
        (GLfloat) -((OGL_Data->BoundBox.y[0] + OGL_Data->BoundBox.y[1]) / 2.0),
        (GLfloat) -((OGL_Data->BoundBox.z[0] + OGL_Data->BoundBox.z[1]) / 2.0)
    );
    glGetDoublev(GL_MODELVIEW_MATRIX, (GLdouble *) OGL_Data->ModelMatrix);
}


Boolean OGL_SetPosition3D(TOGL_Data * OGL_Data, Nlm_RectPtr rect)
/* resizes and positions the 3D window */
{
    Nlm_Int4 width = rect->right - rect->left + 1;
    Nlm_Int4 height = rect->bottom - rect->top + 1;

    if (OGL_Data == NULL || rect == NULL)
        return FALSE;

    if (OGL_Data->Z_rotate) {
        rect->top += Nlm_hScrollBarHeight;
        height -= Nlm_hScrollBarHeight;
    }

    if (Nlm_GetSlateVScrollBar((Nlm_SlatE) OGL_Data->Panel))
        width -= Nlm_vScrollBarWidth + 3;
    if (Nlm_GetSlateHScrollBar((Nlm_SlatE) OGL_Data->Panel))
        height -= Nlm_hScrollBarHeight + 3;

    if (width < 16 || height < 16)
        return FALSE;

#if defined(WIN_MOTIF) && defined(MESA)
    if (glXGetCurrentContext())
#endif
        glViewport(0, 0, width, height);

    rect->right = (Nlm_Int2) (rect->left + width + 3);
    rect->bottom = (Nlm_Int2) (rect->top + height + 3);
    if (Nlm_GetSlateVScrollBar((Nlm_SlatE) OGL_Data->Panel))
        rect->right += Nlm_vScrollBarWidth;
    if (Nlm_GetSlateHScrollBar((Nlm_SlatE) OGL_Data->Panel))
        rect->bottom += Nlm_hScrollBarHeight;

    Nlm_SetPosition(OGL_Data->Panel, rect); /* resize the panel */
    Nlm_ProcessUpdatesFirst(FALSE);
    Nlm_AdjustPrnt(OGL_Data->Panel, rect, FALSE);

    if (OGL_Data->Z_rotate) {   /* if there is a z rotation scroll bar */
        rect->bottom = rect->top;
        rect->top -= Nlm_hScrollBarHeight;
        Nlm_SetPosition(OGL_Data->Z_rotate, rect);
    }

    return TRUE;
}


void OGL_ClearOGL_Data(TOGL_Data * OGL_Data)
/* clear the transforms and bound box in OGL_Data structure */
{
    if (OGL_Data == NULL)
        return;

    OGL_ClearBoundBox(&(OGL_Data->BoundBox));
    OGL_Data->MaxSize = 2.0 * OGL_DEFAULT_SIZE;
    OGL_Data->Background.rgb[0] = 0;
    OGL_Data->Background.rgb[1] = 0;
    OGL_Data->Background.rgb[2] = 0;
}


void OGL_ClearBoundBox(TOGL_BoundBox * BoundBox)
/* initialize a bounds box */
{
    Nlm_Int4 i;

    if (BoundBox == NULL)
        return;
    for (i = 0; i < 2; i++) {
        BoundBox->x[i] = (Nlm_FloatLo) ((i * 2 - 1) * OGL_DEFAULT_SIZE);
        BoundBox->y[i] = (Nlm_FloatLo) ((i * 2 - 1) * OGL_DEFAULT_SIZE);
        BoundBox->z[i] = (Nlm_FloatLo) ((i * 2 - 1) * OGL_DEFAULT_SIZE);
    }
    BoundBox->set = TRUE;       /* we've set up the default values */
}

/* function to start, stop, and check if animation is playing */

NLM_EXTERN Nlm_Boolean OGL_IsPlaying(TOGL_Data *pOGL_Data)
{
#ifdef WIN_MAC
    return FALSE;
#else
    return pOGL_Data->IsPlaying;
#endif
}

NLM_EXTERN void OGL_StopPlaying(TOGL_Data *pOGL_Data)
{
#ifndef WIN_MAC
    pOGL_Data->IsPlaying = FALSE;
#endif
}

NLM_EXTERN void OGL_StartPlaying(TOGL_Data *pOGL_Data)
{
#ifndef WIN_MAC
    Nlm_StopWatchPtr pStopwatch;
    Nlm_FloatHi newtime;

    pOGL_Data->IsPlaying = TRUE;
    pStopwatch = Nlm_StopWatchNew();
    Nlm_StopWatchStart(pStopwatch);
    while (!Nlm_QuittingProgram() && OGL_IsPlaying(pOGL_Data)) {
        OGL_Play(pOGL_Data);
        OGL_DrawViewer3D(pOGL_Data);
        if(pOGL_Data->Tick != 0.0L) {
            do {
                Nlm_StopWatchStop(pStopwatch);
                newtime = Nlm_GetElapsedTime(pStopwatch);
                if (Nlm_EventAvail()) Nlm_ProcessAnEvent();
            } while (newtime < pOGL_Data->Tick);
            Nlm_StopWatchStart(pStopwatch);
        }
        while (Nlm_EventAvail())
            Nlm_ProcessAnEvent();
    }
    Nlm_StopWatchFree(pStopwatch);
#endif
}


#ifdef _PNG

/* callback used by PNG library to report errors */
static void writepng_error_handler(png_structp png_ptr, png_const_charp msg)
{
    Message(MSG_ERROR, "PNG Error: %s", msg);
}

#ifdef WIN_MOTIF
static Nlm_Boolean gotAnXError;

int OGL_XErrorHandler(Display *dpy, XErrorEvent *error)
{
    gotAnXError = TRUE;
    return 0;
}
#endif /* WIN_MOTIF */

static Nlm_MonitorPtr row_monitor;
static int nRows;

void write_row_callback(png_structp png_ptr, png_uint_32 row, int pass)
{
    char message[256];
    float progress;

    if (nRows < 0) { /* if negative, then we're doing interlacing */
        float start, end;
        switch (pass + 1) {
            case 1: start = 0;  end = 1;  break;
            case 2: start = 1;  end = 2;  break;
            case 3: start = 2;  end = 3;  break;
            case 4: start = 3;  end = 5;  break;
            case 5: start = 5;  end = 7;  break;
            case 6: start = 7;  end = 11; break;
            case 7: start = 11; end = 15; break;
        }
        progress = 100.0 * (
            (start / 15) +
            (((float) row) / (-nRows)) * ((end - start) / 15)
        );
    } else { /* not interlaced */
        progress = 100.0 * row / nRows;
    }
    sprintf(message, "Writing PNG file (%i%%) ...", (int) progress);
    Nlm_MonitorStrValue(row_monitor, message);
    Nlm_Update();
}

/* saves current OpenGL context window to PNG file */
void Nlm_SaveImagePNG(Nlm_Char *fname)
{
    GLint viewport[4];
    GLboolean isRGBA;
    Nlm_Uchar *row = NULL;
    FILE *out = NULL;
    int i, bytesPerPixel = 3;
    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    Nlm_Boolean doInterlacing = TRUE;
    TOGL_Data *OGL_Data = (TOGL_Data *)(Cn3D_GetCurrentOGLData());

#if defined(WIN_MOTIF)
    GLint glSize;
    int nAttribs, attribs[20];
    XVisualInfo *visinfo;
    Nlm_Boolean localVI = FALSE;
    Pixmap xPixmap;
    GLXContext currentCtx, glCtx = NULL;
    GLXPixmap glxPixmap;
    GLXDrawable currentXdrw;
    extern Display *Nlm_currentXDisplay;
    extern int Nlm_currentXScreen;
    int (*currentXErrHandler)(Display *, XErrorEvent *);

#elif defined(WIN_MSWIN)
    HDC current_hdc = NULL, hdc = NULL;
    HGDIOBJ current_hgdiobj = NULL;
    HBITMAP hbm = NULL;
    PIXELFORMATDESCRIPTOR pfd;
    int nPixelFormat;
    HGLRC hglrc = NULL, current_hglrc = NULL;

#elif defined(WIN_MAC)
	Nlm_Uchar *base = NULL;
    GLint attrib[20], glSize;
    int na = 0;
    AGLPixelFormat fmt = NULL;
    AGLContext ctx = NULL, currentCtx;

#endif

    glGetBooleanv(GL_RGBA_MODE, &isRGBA);
    if (!isRGBA) {
        /* turn this off till I figure out good way to retrieve colormap */
        Message(MSG_ERROR, "8-Bit PNG output currently unavailable");
        return;
    }

    /* determine the size of the window and allocate (platform-dependent)
       off-screen rendering area */
    glGetIntegerv(GL_VIEWPORT, viewport);
    /*
    viewport[2] *= 3;
    viewport[3] *= 3;
    */

#if defined(WIN_MOTIF)
    currentCtx = glXGetCurrentContext(); /* save current context info */
    currentXdrw = glXGetCurrentDrawable();

    currentXErrHandler = XSetErrorHandler(OGL_XErrorHandler);
    gotAnXError = FALSE;

    /* first, try to get a non-doublebuffered visual, to economize on memory */
    nAttribs = 0;
    attribs[nAttribs++] = GLX_USE_GL;
    attribs[nAttribs++] = GLX_RGBA;
    attribs[nAttribs++] = GLX_RED_SIZE;
    glGetIntegerv(GL_RED_BITS, &glSize);
    attribs[nAttribs++] = glSize;
    attribs[nAttribs++] = GLX_GREEN_SIZE;
    attribs[nAttribs++] = glSize;
    attribs[nAttribs++] = GLX_BLUE_SIZE;
    attribs[nAttribs++] = glSize;
    attribs[nAttribs++] = GLX_DEPTH_SIZE;
    glGetIntegerv(GL_DEPTH_BITS, &glSize);
    attribs[nAttribs++] = glSize;
    attribs[nAttribs++] = None;
    visinfo = glXChooseVisual((Display *) OGL_Data->display,
                              DefaultScreen((Display *) OGL_Data->display),
                              attribs);

    /* if that fails, just revert to the one used for the regular window */
    if (visinfo)
        localVI = TRUE;
    else
        visinfo = (XVisualInfo *) (OGL_Data->visinfo);

    xPixmap = XCreatePixmap((Display *) OGL_Data->display,
        RootWindow((Display *) OGL_Data->display, Nlm_currentXScreen),
        viewport[2], viewport[3], visinfo->depth);
    glxPixmap = glXCreateGLXPixmap((Display *) OGL_Data->display,
        visinfo, xPixmap);
    if (gotAnXError) {
        Message(MSG_ERROR, "Got an X error creating GLXPixmap context");
        goto cleanup;
    }
    glCtx = glXCreateContext((Display *) OGL_Data->display,
        visinfo,
        currentCtx, /* share display list with "regular" context */
        GL_FALSE);
    if (!glCtx || !glXMakeCurrent((Display *) OGL_Data->display,
                                  glxPixmap, glCtx)) {
        Message(MSG_ERROR, "Failed to make current GLXPixmap rendering context");
        goto cleanup;
    }

#elif defined(WIN_MSWIN)
    current_hglrc = wglGetCurrentContext(); /* save to restore later */
    current_hdc = wglGetCurrentDC();

    /* create bitmap of same color type as current rendering context */
    hbm = CreateCompatibleBitmap(current_hdc, viewport[2], viewport[3]);
    if (!hbm) {
        Message(MSG_ERROR, "Failed to create rendering BITMAP");
        goto cleanup;
    }
    memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    /* NOT doublebuffered, to save memory */
    pfd.dwFlags = PFD_DRAW_TO_BITMAP | PFD_SUPPORT_OPENGL;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = GetDeviceCaps(current_hdc, BITSPIXEL);
    pfd.iLayerType = PFD_MAIN_PLANE;

    hdc = CreateCompatibleDC(current_hdc);
    if (!hdc) {
        Message(MSG_ERROR, "CreateCompatibleDC failed");
        goto cleanup;
    }

    current_hgdiobj = SelectObject(hdc, hbm);
    if (!current_hgdiobj) {
        Message(MSG_ERROR, "SelectObject failed");
        goto cleanup;
    }
    nPixelFormat = ChoosePixelFormat(hdc, &pfd);
    if (!nPixelFormat) {
        Message(MSG_ERROR, "ChoosePixelFormat failed: %i", GetLastError());
        goto cleanup;
    }
    if (!SetPixelFormat(hdc, nPixelFormat, &pfd)) {
        Message(MSG_ERROR, "SetPixelFormat failed: %i", GetLastError());
        goto cleanup;
    }
    /*DescribePixelFormat(hdc, nPixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd);*/
    hglrc = wglCreateContext(hdc);
    if (!hglrc) {
        Message(MSG_ERROR, "wglCreateContext failed: %i", GetLastError());
        goto cleanup;
    }
    /* need to share display lists with regular window */
    if (!wglShareLists(current_hglrc, hglrc)) {
        Message(MSG_ERROR, "wglShareLists failed: %i", GetLastError());
        goto cleanup;
    }
    if (!wglMakeCurrent(hdc, hglrc)) {
        Message(MSG_ERROR, "wglMakeCurrent failed: %i", GetLastError());
        goto cleanup;
    }

#elif defined(WIN_MAC)
	currentCtx = aglGetCurrentContext();

	/* Mac pixels seem to always be 32-bit */
	bytesPerPixel = 4;

	base = (Nlm_Uchar *) MemNew(viewport[2] * viewport[3] * bytesPerPixel);
	if (!base) {
    	Message(MSG_ERROR, "Failed to allocate image buffer");
    	goto cleanup;
	}

	/* create an off-screen rendering context (NOT doublebuffered) */
	attrib[na++] = AGL_OFFSCREEN;
	attrib[na++] = AGL_RGBA;
	glGetIntegerv(GL_RED_BITS, &glSize);
    attrib[na++] = AGL_RED_SIZE;
    attrib[na++] = glSize;
    attrib[na++] = AGL_GREEN_SIZE;
    attrib[na++] = glSize;
    attrib[na++] = AGL_BLUE_SIZE;
    attrib[na++] = glSize;
	glGetIntegerv(GL_DEPTH_BITS, &glSize);
    attrib[na++] = AGL_DEPTH_SIZE;
    attrib[na++] = glSize;
    attrib[na++] = AGL_NONE;

    if ((fmt=aglChoosePixelFormat(NULL, 0, attrib)) == NULL) {
    	Message(MSG_ERROR, "aglChoosePixelFormat failed");
    	goto cleanup;
    }
    /* share display lists with current "regular" context */
    if ((ctx=aglCreateContext(fmt, currentCtx)) == NULL) {
    	Message(MSG_ERROR, "aglCreateContext failed");
    	goto cleanup;
    }

    /* attach off-screen buffer to this context */
    if (!aglSetOffScreen(ctx, viewport[2], viewport[3],
                         bytesPerPixel * viewport[2], base)) {
    	Message(MSG_ERROR, "aglSetOffScreen failed");
    	goto cleanup;
	}
    if (!aglSetCurrentContext(ctx)) {
    	Message(MSG_ERROR, "aglSetCurrentContext failed");
    	goto cleanup;
	}

#endif

    row = (Nlm_Uchar *) MemNew(viewport[2] * bytesPerPixel);
    if (!row) {
        Message(MSG_ERROR, "Failed to allocate pixel storage for PNG output");
        goto cleanup;
    }

    /* open the output file for writing */
    out = fopen(fname, "wb");
    if (!out) {
        Message(MSG_ERROR, "Can't write to file '%s'", fname);
        goto cleanup;
    }

    /* set up the PNG writing (see libpng.txt) */
    png_ptr = png_create_write_struct(
        PNG_LIBPNG_VER_STRING, NULL, writepng_error_handler, NULL);
    if (!png_ptr) {
        Message(MSG_ERROR, "Can't create PNG write structure");
        goto cleanup;
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        Message(MSG_ERROR, "Can't create PNG info structure");
        goto cleanup;
    }

    if (setjmp(png_ptr->jmpbuf)) {
        Message(MSG_ERROR, "PNG write failed");
        goto cleanup;
    }

    png_init_io(png_ptr, out);

    /* sets callback that's called by PNG after each written row */
    png_set_write_status_fn(png_ptr, write_row_callback);

    png_set_compression_level(png_ptr, Z_BEST_COMPRESSION);

    png_set_IHDR(png_ptr, info_ptr, viewport[2], viewport[3],
        8, PNG_COLOR_TYPE_RGB,
        doInterlacing ? PNG_INTERLACE_ADAM7 : PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    png_write_info(png_ptr, info_ptr);

    /*
       Redraw the model into the new off-screen context, then use glReadPixels
       to retrieve pixel data. It's much easier to use glReadPixels rather than
       trying to read directly from the off-screen buffer, since GL can do all
       the potentially tricky work of translating from whatever pixel format
       the buffer uses into "regular" RGB byte triples.
    */
    row_monitor = Nlm_MonitorStrNewEx("PNG Progress", 20, FALSE);
    Nlm_MonitorStrValue(row_monitor, "Rendering to off-screen buffer...");
    Nlm_Update();
    OGL_Data->NeedCameraSetup = TRUE;
#ifdef WIN_MAC
    drawingOffscreen = TRUE; /* signal redraw not to use agl buffer rect */
#endif
    OGL_DrawViewer3D(OGL_Data);
#ifdef WIN_MAC
    drawingOffscreen = FALSE;
#endif
    glPixelStorei(GL_PACK_ALIGNMENT, 1);

    /*
       Write image row by row to avoid having to allocate another full image's
       worth of memory. Do multiple passes for interlacing, if necessary. Note
       that PNG's rows are stored top down, while GL's are bottom up.
    */
    nRows = viewport[3];
    if (doInterlacing) {
        int pass, r;
        nRows = -nRows; /* signal to monitor that we're interlacing */
        if (png_set_interlace_handling(png_ptr) != 7) {
            Message(MSG_ERROR, "confused by unkown PNG interlace scheme");
            goto cleanup;
        }
        for (pass = 1; pass <= 7; pass++) {
            for (i = viewport[3] - 1; i >= 0; i--) {
                r = viewport[3] - i - 1;
                /* when interlacing, only certain rows are actually read
                   during certain passes - avoid unncessary reads */
                if (
                    ((pass == 1 || pass == 2) && (r % 8 == 0)) ||
                    ((pass == 3) && ((r - 4) % 8 == 0)) ||
                    ((pass == 4) && (r % 4 == 0)) ||
                    ((pass == 5) && ((r - 2) % 4 == 0)) ||
                    ((pass == 6) && (r % 2 == 0)) ||
                    ((pass == 7) && ((r - 1) % 2 == 0))
                   )
                    glReadPixels(viewport[0], i, viewport[2], 1,
                                 GL_RGB, GL_UNSIGNED_BYTE, row);
                /* ... but still have to call this for each row in each pass */
                png_write_row(png_ptr, row);
            }
        }
    } else { /* not interlaced */
        for (i = viewport[3] - 1; i >= 0; i--) {
            glReadPixels(viewport[0], i, viewport[2], 1,
                         GL_RGB, GL_UNSIGNED_BYTE, row);
            png_write_row(png_ptr, row);
        }
    }
    Nlm_MonitorFree(row_monitor);
    Nlm_Update();

    /* finish up */
    png_write_end(png_ptr, info_ptr);


    /* restore context and clean up */
    cleanup:

    if (out) fclose(out);

#if defined(WIN_MOTIF)
    gotAnXError = FALSE;
    if (glCtx) {
        glXMakeCurrent(Nlm_currentXDisplay, currentXdrw, currentCtx);
        glXDestroyContext(Nlm_currentXDisplay, glCtx);
    }
    glXDestroyGLXPixmap(Nlm_currentXDisplay, glxPixmap);
    XFreePixmap(Nlm_currentXDisplay, xPixmap);
    if (localVI && visinfo) XFree(visinfo);
    if (gotAnXError) {
        Message(MSG_ERROR, "Got an X error destroying GLXPixmap context");
    }
    XSetErrorHandler(currentXErrHandler);

#elif defined(WIN_MSWIN)
    if (current_hdc && current_hglrc) wglMakeCurrent(current_hdc, current_hglrc);
    if (hglrc) wglDeleteContext(hglrc);
    /*if (current_hgdiobj) SelectObject(hdc, current_hgdiobj);*/
    if (hbm) DeleteObject(hbm);
    if (hdc) DeleteDC(hdc);

#elif defined(WIN_MAC)
	aglSetCurrentContext(currentCtx);
	if (ctx) aglDestroyContext(ctx);
	if (fmt) aglDestroyPixelFormat(fmt);
	if (base) MemFree(base);

#endif

    if (row) MemFree(row);
    if (png_ptr) {
        if (info_ptr)
            png_destroy_write_struct(&png_ptr, &info_ptr);
        else
            png_destroy_write_struct(&png_ptr, NULL);
    }

    /* redraw in case progress meter overwrote part of GL area */
    OGL_DrawViewer3D(OGL_Data);
}

#endif /* _PNG */


/* create display list with logo */
NLM_EXTERN void OGL_DrawLogo(TOGL_Data *OGL_Data)
{
    DDV_ColorCell colorCell;
    Uint1 logoColor[3] = { 100, 240, 150 };
    int i, n, s, g;

#define LOGO_SIDES 36
    int segments = 180;
    GLdouble bigRad = 12.0, height = 24.0,
        minRad = 0.1, maxRad = 2.0,
        ringPts[LOGO_SIDES * 3], *pRingPts = ringPts,
        prevRing[LOGO_SIDES * 3], *pPrevRing = prevRing, *tmp,
        ringNorm[LOGO_SIDES * 3], *pRingNorm = ringNorm,
        prevNorm[LOGO_SIDES * 3], *pPrevNorm = prevNorm,
        PI = acos(-1), length,
        startRad, midRad, phase, currentRad, CR[3], H[3], V[3];

    if (!OGL_Data || OGL_Data->IndexMode) return;

    /* set up initial camera */
    OGL_Data->CameraDistance = 200.0;
    OGL_Data->CameraDirection[0] = 0.0;
    OGL_Data->CameraDirection[1] = 0.0;
    OGL_Data->CameraAngle = acos(-1) / 14;
    OGL_Data->NeedCameraSetup = TRUE;
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glGetDoublev(GL_MODELVIEW_MATRIX, (GLdouble *) OGL_Data->ModelMatrix);

    glNewList(1, GL_COMPILE);

    /* create logo */
    DDV_SetColorInCell(&colorCell, logoColor);
    OGL_SetColor(OGL_Data, &colorCell, GL_DIFFUSE, 1.0);

    for (n = 0; n < 2; n++) { /* helix strand */
        if (n == 0) {
            startRad = maxRad;
            midRad = minRad;
            phase = 0;
        } else {
            startRad = minRad;
            midRad = maxRad;
            phase = PI;
        }
        for (g = 0; g <= segments; g++) { /* segment (bottom to top) */

            if (g < segments/2)
                currentRad = startRad + (midRad - startRad) *
                    (0.5 - 0.5 * cos(PI * g / (segments/2)));
            else
                currentRad = midRad + (startRad - midRad) *
                    (0.5 - 0.5 * cos(PI * (g - segments/2) / (segments/2)));

            CR[1] = height * g / segments - height/2;
            if (g > 0) phase += PI * 2 / segments;
            CR[2] = bigRad * cos(phase);
            CR[0] = bigRad * sin(phase);

            /* make a strip around the strand circumference */
            for (s = 0; s < LOGO_SIDES; s++) {
                V[0] = CR[0];
                V[2] = CR[2];
                V[1] = 0;
                length = sqrt(V[0]*V[0] + V[1]*V[1] + V[2]*V[2]);
                for (i = 0; i < 3; i++) V[i] /= length;
                H[0] = H[2] = 0;
                H[1] = 1;
                for (i = 0; i < 3; i++) {
                    pRingNorm[3*s + i] = V[i] * cos(PI * 2 * s / LOGO_SIDES) +
                                         H[i] * sin(PI * 2 * s / LOGO_SIDES);
                    pRingPts[3*s + i] = CR[i] + pRingNorm[3*s + i] * currentRad;
                }
            }
            if (g > 0) {
                glBegin(GL_TRIANGLE_STRIP);
                for (s = 0; s < LOGO_SIDES; s++) {
                    glNormal3d(pPrevNorm[3*s], pPrevNorm[3*s + 1], pPrevNorm[3*s + 2]);
                    glVertex3d(pPrevRing[3*s], pPrevRing[3*s + 1], pPrevRing[3*s + 2]);
                    glNormal3d(pRingNorm[3*s], pRingNorm[3*s + 1], pRingNorm[3*s + 2]);
                    glVertex3d(pRingPts[3*s], pRingPts[3*s + 1], pRingPts[3*s + 2]);
                }
                glNormal3d(pPrevNorm[0], pPrevNorm[1], pPrevNorm[2]);
                glVertex3d(pPrevRing[0], pPrevRing[1], pPrevRing[2]);
                glNormal3d(pRingNorm[0], pRingNorm[1], pRingNorm[2]);
                glVertex3d(pRingPts[0], pRingPts[1], pRingPts[2]);
                glEnd();
            }
            
            /* cap the ends */
            glBegin(GL_POLYGON);
            if ((g == 0 && n == 0) || (g == segments && n == 1))
                glNormal3d(-1, 0, 0);
            else
                glNormal3d(1, 0, 0);
            if (g == 0) {
                for (s = 0; s < LOGO_SIDES; s++)
                    glVertex3d(pRingPts[3*s], pRingPts[3*s + 1], pRingPts[3*s + 2]);
            } else if (g == segments) {
                for (s = LOGO_SIDES - 1; s >= 0; s--)
                    glVertex3d(pRingPts[3*s], pRingPts[3*s + 1], pRingPts[3*s + 2]);
            }
            glEnd();

            /* switch pointers to store previous ring */
            tmp = pPrevRing;
            pPrevRing = pRingPts;
            pRingPts = tmp;
            tmp = pPrevNorm;
            pPrevNorm = pRingNorm;
            pRingNorm = tmp;
        }
    }

    glEndList();

    OGL_Data->Layers->SelectedLayer = 1; /* fool DrawViewer3D into thinking there's a single structure */
    OGL_DrawViewer3D(OGL_Data);
}

#endif                          /* _OPENGL */


/* this function "shared" by both vibrant and OpenGL versions */
#ifdef _OPENGL
/* add a thick splined curve from point 1 *halfway* to point 2 */
void Nlm_AddHalfWorm3D(TOGL_Data * OGL_Data, DDV_ColorCell * color,
                       Nlm_FloatHi x0, Nlm_FloatHi y0, Nlm_FloatHi z0,
                       Nlm_FloatHi x1, Nlm_FloatHi y1, Nlm_FloatHi z1,
                       Nlm_FloatHi x2, Nlm_FloatHi y2, Nlm_FloatHi z2,
                       Nlm_FloatHi x3, Nlm_FloatHi y3, Nlm_FloatHi z3,
                       Nlm_Boolean cap1, Nlm_Boolean cap2, Nlm_FloatHi radius,
                       Nlm_Int4 segments, Nlm_Int4 sides)
#else
typedef Nlm_FloatLo GLfloat;
typedef Nlm_FloatHi GLdouble;
void Nlm_AddHalfWorm3D(Nlm_Picture3D pic,
                       Nlm_Segment3D segment, BigScalar userData,
                       Uint1 layer, Uint1 color,
                       Nlm_FloatHi x0, Nlm_FloatHi y0, Nlm_FloatHi z0,
                       Nlm_FloatHi x1, Nlm_FloatHi y1, Nlm_FloatHi z1,
                       Nlm_FloatHi x2, Nlm_FloatHi y2, Nlm_FloatHi z2,
                       Nlm_FloatHi x3, Nlm_FloatHi y3, Nlm_FloatHi z3,
                       Nlm_FloatHi radius, Nlm_Int4 segments)
#endif
{
    Nlm_Int4 i, j, k, m, offset;
    GLdouble len, R1x, R1y, R1z, R2x, R2y, R2z, MG[4][3],
        T[4], t, Qtx, Qty, Qtz, px, py, pz,
        dQtx, dQty, dQtz, /*ddQtx, ddQty, ddQtz,*/
        Hx, Hy, Hz, Vx, Vy, Vz, prevlen,
        cosj, sinj, pi = 3.14159265358979323846;
    GLdouble *Nx = NULL, *Ny, *Nz, *Cx, *Cy, *Cz,
        *pNx, *pNy, *pNz, *pCx, *pCy, *pCz, *tmp, *fblock = NULL;
    /*
     * The Hermite matrix Mh.
     */
    static Nlm_FloatHi Mh[4][4] = {
        { 2, -2,  1,  1},
        {-3,  3, -2, -1},
        { 0,  0,  1,  0},
        { 1,  0,  0,  0}
    };
    /*
     * Variables that affect the curve shape
     *   a=b=0 = Catmull-Rom
     */
    static Nlm_FloatHi a = -0.8, /* tension    (adjustable)  */
     c = 0,                     /* continuity (should be 0) */
     b = 0;                     /* bias       (should be 0) */

#ifdef _OPENGL
    if (radius > 0.0) {         /* if line, color assigned in OGL_AddLine3D */
        if (OGL_Data == NULL || color == NULL)
            return;

        OGL_SetColor(OGL_Data, color, GL_DIFFUSE, 1.0);

        if (sides % 2)
            sides++;         /* must be an even number! */
    }
#endif

    /* First, calculate the coordinate points of the center of the worm,
     * using the Kochanek-Bartels variant of the Hermite curve.
     */
    R1x = 0.5 * (1 - a) * (1 + b) * (1 + c) * (x1 - x0) +
        0.5 * (1 - a) * (1 - b) * (1 - c) * (x2 - x1);
    R1y = 0.5 * (1 - a) * (1 + b) * (1 + c) * (y1 - y0) +
        0.5 * (1 - a) * (1 - b) * (1 - c) * (y2 - y1);
    R1z = 0.5 * (1 - a) * (1 + b) * (1 + c) * (z1 - z0) +
        0.5 * (1 - a) * (1 - b) * (1 - c) * (z2 - z1);
    R2x = 0.5 * (1 - a) * (1 + b) * (1 - c) * (x2 - x1) +
        0.5 * (1 - a) * (1 - b) * (1 + c) * (x3 - x2);
    R2y = 0.5 * (1 - a) * (1 + b) * (1 - c) * (y2 - y1) +
        0.5 * (1 - a) * (1 - b) * (1 + c) * (y3 - y2);
    R2z = 0.5 * (1 - a) * (1 + b) * (1 - c) * (z2 - z1) +
        0.5 * (1 - a) * (1 - b) * (1 + c) * (z3 - z2);
    /*
     * Multiply MG=Mh.Gh, where Gh = [ P(1) P(2) R(1) R(2) ]. This
     * 4x1 matrix of vectors is constant for each segment.
     */
    for (i = 0; i < 4; i++) {   /* calculate Mh.Gh */
        MG[i][0] =
            Mh[i][0] * x1 + Mh[i][1] * x2 + Mh[i][2] * R1x +
            Mh[i][3] * R2x;
        MG[i][1] =
            Mh[i][0] * y1 + Mh[i][1] * y2 + Mh[i][2] * R1y +
            Mh[i][3] * R2y;
        MG[i][2] =
            Mh[i][0] * z1 + Mh[i][1] * z2 + Mh[i][2] * R1z +
            Mh[i][3] * R2z;
    }

    for (i = 0; i <= segments; i++) {

        /* t goes from [0,1] from P(1) to P(2) (and we want to go halfway only),
           and the function Q(t) defines the curve of this segment. */
        t = (0.5 / segments) * i;
        /*
           * Q(t)=T.(Mh.Gh), where T = [ t^3 t^2 t 1 ]
         */
        T[0] = t * t * t;
        T[1] = t * t;
        T[2] = t;
        T[3] = 1;
        Qtx = T[0] * MG[0][0] + T[1] * MG[1][0] + T[2] * MG[2][0] + MG[3][0] /* *T[3] */
            ;
        Qty = T[0] * MG[0][1] + T[1] * MG[1][1] + T[2] * MG[2][1] + MG[3][1] /* *T[3] */
            ;
        Qtz = T[0] * MG[0][2] + T[1] * MG[1][2] + T[2] * MG[2][2] + MG[3][2] /* *T[3] */
            ;

#ifndef _OPENGL
        if (i > 0) {
            Int4 iX0, iY0, iZ0, iX1, iY1, iZ1;

/* number 1000000.0 must be same as VIEWSCALE in algorend.c! */
            iX0 = (Int4) (px * 1000000.0);
            iY0 = (Int4) (py * 1000000.0);
            iZ0 = (Int4) (pz * 1000000.0);
            iX1 = (Int4) (Qtx * 1000000.0);
            iY1 = (Int4) (Qty * 1000000.0);
            iZ1 = (Int4) (Qtz * 1000000.0);
            AddLine3D(pic, segment, userData, layer, color,
                      iX0, iY0, iZ0, iX1, iY1, iZ1);
        }
        /* save to use as previous point for connecting points together */
        px = Qtx;
        py = Qty;
        pz = Qtz;

#else
        if (radius == 0.0) {
            if (i > 0)
                OGL_AddLine3D(OGL_Data, color, px, py, pz, Qtx, Qty, Qtz);
            /* save to use as previous point for connecting points together */
            px = Qtx;
            py = Qty;
            pz = Qtz;

        } else {
            /* construct a circle of points centered at and
               in a plane normal to the curve at t - these points will
               be used to construct the "thick" worm */

            /* allocate single block of storage for two circles of points */
            if (!Nx) {
                fblock = Nx = MemNew(sizeof(Nlm_FloatHi) * 12 * sides);
                if (!Nx)
                    return;
                Ny = &Nx[sides];
                Nz = &Nx[sides * 2];
                Cx = &Nx[sides * 3];
                Cy = &Nx[sides * 4];
                Cz = &Nx[sides * 5];
                pNx = &Nx[sides * 6];
                pNy = &Nx[sides * 7];
                pNz = &Nx[sides * 8];
                pCx = &Nx[sides * 9];
                pCy = &Nx[sides * 10];
                pCz = &Nx[sides * 11];
            }

            /*
             * The first derivative of Q(t), d(Q(t))/dt, is the slope
             * (tangent) at point Q(t); now T = [ 3t^2 2t 1 0 ]
             */
            T[0] = t * t * 3;
            T[1] = t * 2;
            T[2] = 1;
            T[3] = 0;
            dQtx =
                T[0] * MG[0][0] + T[1] * MG[1][0] +
                MG[2][0] /* *T[2] + T[3]*MG[3][0] */ ;
            dQty =
                T[0] * MG[0][1] + T[1] * MG[1][1] +
                MG[2][1] /* *T[2] + T[3]*MG[3][1] */ ;
            dQtz =
                T[0] * MG[0][2] + T[1] * MG[1][2] +
                MG[2][2] /* *T[2] + T[3]*MG[3][2] */ ;

            /* use cross prod't of [1,0,0] x normal as horizontal */
            Hx = 0.0;
            Hy = -dQtz;
            Hz = dQty;
            len = sqrt(Hy * Hy + Hz * Hz);
            if (len < 0.000001) { /* nearly colinear - use [1,0.1,0] instead */
                Hx = 0.1 * dQtz;
                Hy = -dQtz;
                Hz = dQty - 0.1 * dQtx;
            }
            Hx /= len;
            Hy /= len;
            Hz /= len;

            /* and a vertical vector = normal x H */
            Vx = dQty * Hz - dQtz * Hy;
            Vy = dQtz * Hx - dQtx * Hz;
            Vz = dQtx * Hy - dQty * Hx;
            len = sqrt(Vx * Vx + Vy * Vy + Vz * Vz);
            Vx /= len;
            Vy /= len;
            Vz /= len;

            /* finally, the worm circumference points (C) and normals (N) are
               simple trigonomic combinations of H and V */
            for (j = 0; j < sides; j++) {
                cosj = cos(2 * pi * j / sides);
                sinj = sin(2 * pi * j / sides);
                Nx[j] = Hx * cosj + Vx * sinj;
                Ny[j] = Hy * cosj + Vy * sinj;
                Nz[j] = Hz * cosj + Vz * sinj;
                Cx[j] = Qtx + Nx[j] * radius;
                Cy[j] = Qty + Ny[j] * radius;
                Cz[j] = Qtz + Nz[j] * radius;
            }

            /* figure out which points on the previous circle "match" best
               with these, to minimize envelope twisting */
            for (m = 0; m < sides; m++) {
                len = 0.0;
                for (j = 0; j < sides; j++) {
                    k = j + m;
                    if (k >= sides)
                        k -= sides;
                    len += (Cx[k] - pCx[j]) * (Cx[k] - pCx[j]) +
                        (Cy[k] - pCy[j]) * (Cy[k] - pCy[j]) +
                        (Cz[k] - pCz[j]) * (Cz[k] - pCz[j]);
                }
                if (m == 0 || len < prevlen) {
                    prevlen = len;
                    offset = m;
                }
            }

            /* create triangles from points along this and previous circle */
            if (i > 0) {
                glBegin(GL_TRIANGLE_STRIP);
                for (j = 0; j < sides; j++) {
                    k = j + offset;
                    if (k >= sides) k -= sides;
                    glNormal3d(Nx[k], Ny[k], Nz[k]);
                    glVertex3d(Cx[k], Cy[k], Cz[k]);
                    glNormal3d(pNx[j], pNy[j], pNz[j]);
                    glVertex3d(pCx[j], pCy[j], pCz[j]);
                }
                glNormal3d(Nx[offset], Ny[offset], Nz[offset]);
                glVertex3d(Cx[offset], Cy[offset], Cz[offset]);
                glNormal3d(pNx[0], pNy[0], pNz[0]);
                glVertex3d(pCx[0], pCy[0], pCz[0]);
                glEnd();
            }

            /* put caps on the end */
            if (cap1 && i == 0) {
                glBegin(GL_POLYGON);
                len = sqrt(dQtx*dQtx + dQty*dQty + dQtz*dQtz);
                dQtx /= len; dQty /= len; dQtz /= len;
                glNormal3d(-dQtx, -dQty, -dQtz);
                for (j = sides - 1; j >= 0; j--) {
                    k = j + offset;
                    if (k >= sides) k -= sides;
                    glVertex3d(Cx[k], Cy[k], Cz[k]);
                }
                glEnd();
            }
            else if (cap2 && i == segments) {
                glBegin(GL_POLYGON);
                len = sqrt(dQtx*dQtx + dQty*dQty + dQtz*dQtz);
                dQtx /= len; dQty /= len; dQtz /= len;
                glNormal3d(dQtx, dQty, dQtz);
                for (j = 0; j < sides; j++) {
                    k = j + offset;
                    if (k >= sides) k -= sides;
                    glVertex3d(Cx[k], Cy[k], Cz[k]);
                }
                glEnd();
            }

            /* store this circle as previous for next round; instead of copying
               all values, just swap pointers */
#define SWAPPTR(p1,p2) tmp=(p1); (p1)=(p2); (p2)=tmp
            SWAPPTR(Nx, pNx);
            SWAPPTR(Ny, pNy);
            SWAPPTR(Nz, pNz);
            SWAPPTR(Cx, pCx);
            SWAPPTR(Cy, pCy);
            SWAPPTR(Cz, pCz);
        }
#endif

    }
    if (fblock)
        MemFree(fblock);
}

