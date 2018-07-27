/*   algorend.h
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
* File Name:  algorend.h
*
* Author:  Christopher Hogue
*
* Version Creation Date:   1/31/96
*
* File Description: algorithmic rendering structures
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: algorend.h,v $
* Revision 6.40  2000/04/25 00:22:35  thiessen
* save quality settings in config
*
* Revision 6.39  2000/04/19 17:56:48  thiessen
* added background color in OpenGL
*
* Revision 6.38  2000/04/17 15:54:26  thiessen
* add cylinder arrows; misc graphics tweaks
*
* Revision 6.37  2000/04/05 23:11:27  lewisg
* get rid of double redraw for hide row
*
* Revision 6.36  2000/03/23 14:53:04  thiessen
* fix bug in viewer3d camera; added conservation color algorithm to ARS
*
* Revision 6.35  2000/03/22 23:17:49  thiessen
* added ability to save ARS in ASN1
*
* Revision 6.34  2000/03/15 18:49:07  thiessen
* fixed viewer3d color bug
*
* Revision 6.33  2000/03/13 16:04:26  thiessen
* re-implementation of features
*
* Revision 6.32  2000/03/09 17:56:58  thiessen
* changes to palette handling, feature implementation, PARS storage
*
* Revision 6.31  2000/03/08 21:46:14  lewisg
* cn3d saves viewport, misc bugs
*
* Revision 6.30  2000/03/02 21:18:28  thiessen
* installed color-by-conservation on all alignment rows
*
* Revision 6.29  2000/03/01 16:17:54  thiessen
* improved handling of colors; many small fixes
*
* Revision 6.28  2000/02/26 13:30:41  thiessen
* capped cylinders and worms for visible ends
*
* Revision 6.27  2000/02/10 17:47:01  thiessen
* added: color-by-sequence-conservation menu item, zoom-out to OpenGL, misc fixes
*
* Revision 6.26  2000/02/10 15:51:58  lewisg
* cn3d responds and send correct update messages.  many coloring bug fixes
*
* Revision 6.25  2000/02/03 15:07:45  thiessen
* more correct handling of worm bonds
*
* Revision 6.24  2000/01/14 21:40:41  lewisg
* add translucent spheres, ion labels, new cpk, fix misc bugs
*
* Revision 6.23  1999/12/13 23:20:44  lewisg
* bug fixes: duplicate color structures eliminated, clear color doesn't clear case
*
* Revision 6.22  1999/12/11 01:30:34  lewisg
* fix bugs with sharing colors between ddv and cn3d
*
* Revision 6.21  1999/11/01 22:10:27  lewisg
* add ability to call color functions by type, and add this to cn3d
*
* Revision 6.20  1999/10/31 22:37:36  thiessen
* added worm render type to UI
*
* Revision 6.19  1999/10/29 14:15:27  thiessen
* ran all Cn3D source through GNU Indent to prettify
*
* Revision 6.18  1999/09/21 18:09:16  lewisg
* binary search added to color manager, various bug fixes, etc.
*
* Revision 6.17  1999/04/06 20:10:16  lewisg
* fix typo
*
* Revision 6.16  1999/04/06 20:04:39  lewisg
* more opengl
*
* Revision 6.15  1999/03/30 22:36:21  ywang
* add functions to color salsa for NcbiMimeAsn1_strucseqs & code reorganization
*
* Revision 6.14  1999/02/11 18:48:14  lewisg
* delete color index functions
*
* Revision 6.13  1999/02/10 23:49:42  lewisg
* use RGB values instead of indexed palette
*
* Revision 6.12  1998/11/04 00:06:23  ywang
* add function for modeling: change render/color for special residue(s)
*
 * Revision 6.11  1998/10/28  19:29:03  ywang
 * add C_BYSEQCONS macro
 *
 * Revision 6.10  1998/10/28  19:02:07  kans
 * added two prototypes
 *
* Revision 6.8  1998/06/16 18:00:28  lewisg
* moved rendering menus and created a reset presentation menu item
*
* Revision 6.7  1998/05/26 21:35:19  lewisg
* added defaults to render menu, got rid of mouse 3D actions menu item
*
* Revision 6.6  1998/04/27 17:50:06  lewisg
* added color by conservation
*
* Revision 6.5  1998/04/20 22:09:02  lewisg
* got rid of dead code
*
* Revision 6.4  1998/04/15 03:06:14  lewisg
* get rid of dos line breaks
*
* Revision 6.3  1998/04/01 23:26:13  lewisg
* added new startup mode + fixed slave rendering
*
* Revision 6.2  1998/03/06 23:19:22  lewisg
* codewarrior fixes
*
* Revision 6.1  1998/03/06 01:16:58  lewisg
* merge
*
* Revision 6.0  1997/08/25 18:13:26  madden
* Revision changed to 6.0
*
* Revision 5.2  1996/07/22 00:24:10  hogue
* Added an origin 3D item for no-primitives condition and general use.
*
 * Revision 5.1  1996/06/03  21:21:26  hogue
 * Made tubes bigger so they are less likely to look ball-n-stick like
 * than before.
 *
 * Revision 5.0  1996/05/28  14:05:44  ostell
 * Set to revision 5.0
 *
 * Revision 1.11  1996/05/22  21:46:48  hogue
 * Added white button to label controls.
 *
 * Revision 1.10  1996/05/22  20:47:01  hogue
 * Removed HetLabel variables
 *
 * Revision 1.9  1996/05/22  15:56:55  hogue
 * Altered the label structures to make them more useful.
 *
 * Revision 1.8  1996/05/14  15:19:14  hogue
 * Added LabelControls
 *
 * Revision 1.7  1996/05/09  18:33:28  vakatov
 * included <viewer3d.h> to get know the CAMERA_SIZE_I4 actual value
 *
 * Revision 1.6  1996/05/09  15:40:40  hogue
 * Domain rendering enabled.
 *
 * Revision 1.5  1996/04/26  18:41:47  vakatov
 * CN3D sources ported to MS-Windows;
 * the portability errors and warnings fixed, etc.
 *
* ==========================================================================
*/

#ifndef _ALGOREND_
#define _ALGOREND_ 1

#ifdef _OPENGL
#include <shim3d.h>
#else
#include <viewer3d.h>
#endif
#include <cn3dmain.h>


#ifdef __cplusplus
extern "C" {
#endif
/***ASN.1 & ANNMM compatible values for Rendering*****/
#define R_DEFAULT 0
#define R_WIRE  1
#define R_SPACE 2
#define R_STICK 3
#define R_BALLNSTICK 4
#define R_THICKWIRE 5
#define R_WORMWIRE 6
#ifdef _OPENGL
#define R_WORMTHIN 7
#define R_WORMFAT 8
#endif
#define R_NAME 10
#define R_NUMBER 11
#define R_PDBNUMBER 12

#define C_default  0
#define C_hotpink  1
#define C_magenta  2
#define C_purple   3
#define C_blue     4
#define C_sky      5
#define C_cyan     6
#define C_sea      7
#define C_green    8
#define C_yellow   9
#define C_gold    10
#define C_orange  11
#define C_red     12
#define C_pink    13
#define C_pinktint 14
#define C_white    15
#define C_black    16
#define C_bluetint 17
#define C_greentint 18
#define C_yellowtint 19
#define C_gray    20
#define C_brown   21
#define C_top     22

/* these set bond draw styles */
#define NO_BOND 0
#define HALF_BOND 1
#define HALF_WORM_BOND 2

/* these set atom widths */
#define ATOM_NONE   0
#define ATOM_SPACE  1
#define ATOM_2XBOND 2
#define ATOM_ISBOND 3
#define HET_BOND_WIDTH   (float)0.4
#define VIRT_BOND_WIDTH  (float)0.6
#define SUPER_BOND_WIDTH (float)1.0
#define CYL_THRESHOLD    (float)0.1
#define EXPAND_ATOM      (float)1.8

/* flags for rendering various parts of the structure */
#define CONNECTON  0
#define VIRTUALBB  1
#define PARTIALBB  2
#define COMPLETEBB 3
#define RESIDUES   4
#define IONSON     5
#define HETSON     6
#define SOLVENTON  7
#define PBBLABELS  8
#define NTBBLABELS 9
/*#define HETLABELS 10*/
#define PTERMLABELS  11
#define NTTERMLABELS 12
#define ANYBACKBONE  13


/* flags for labeling styles */
#define   L_NAME      0x01
#define   L_NUM       0x02
#define   L_PDB       0x04
#define   L_WHITE     0x20
#define   L_3LETR     0x40
#define   L_1LETR     0x80
#define LA_LEFT   0x01
#define LA_RIGHT  0x02
#define LA_UPPER  0x04
#define LA_LOWER  0x08
#define LA_CENTER 0x20
#define LA_FRONT  0x40


/* bit flags for 3d-object arrow styles */
#define ARROW_BRICK 0x01
#define ARROW_CYL   0x02


#define VIEWSCALE 1000000.0


/*
   note that the fields in this structure are mirrored in the ARSFieldData
   structure in cn3dsave.c in order to save as ASN1 - any changes to this
   structure should be accompanied by update to ARSFieldData or 
   save/read won't work!
*/

typedef struct AlgorRenderSet {

/* global settings */
    Boolean HydrogensOn;
    Uint1 BGColRGB[3];
    Int2 ConsColAlg;

/* Protein Renderings */
    Boolean PVirtualBBOn;
    Boolean PPartialBBOn;
    Boolean PCompleteBBOn;
    Boolean PResiduesOn;
    Int2 PBBRender;
    Int2 PBBColor;
    Int2 PResRender;
    Int2 PResColor;
    Uint1 PBBColRGB[3];
    Uint1 PResColRGB[3];

    Int2 PBBLabelInterval;
    Int2 PBBLabelJust;
    Int2 PBBLabelStyle;
    Int2 PBBLabelScale;

    Boolean PTermLabelOn;
    Int2 PTermLabelJust;
    Int2 PTermLabelStyle;
    Int2 PTermLabelScale;

/* DNA/RNA  Renderings */
    Boolean NTVirtualBBOn;
    Boolean NTPartialBBOn;
    Boolean NTCompleteBBOn;
    Boolean NTResiduesOn;
    Int2 NTBBRender;
    Int2 NTBBColor;
    Int2 NTResRender;
    Int2 NTResColor;
    Uint1 NTBBColRGB[3];
    Uint1 NTResColRGB[3];

    Int2 NTBBLabelInterval;
    Int2 NTBBLabelJust;
    Int2 NTBBLabelStyle;
    Int2 NTBBLabelScale;

    Boolean NTTermLabelOn;
    Int2 NTTermLabelJust;
    Int2 NTTermLabelStyle;
    Int2 NTTermLabelScale;

    Boolean HeterogensOn;
    Int2 HetRender;
    Int2 HetColor;

    Boolean IonsOn;
    Int2 IonRender;
    Int2 IonColor;
    Boolean IonLabelOn;

    Boolean ConnectOn;
    Int2 ConnectRender;
    Int2 ConnectColor;

    Boolean SolventOn;
    Int2 SolventRender;
    Int2 SolventColor;

    Boolean ObjectOn;
    Int2 ObjectColor;
    Int2 ObjectArrows;

} ARS, PNTR PARS;



/* this structure keeps data for the rendering callbacks */
typedef struct RenderKeep {
    Byte NodeWhat;
    Byte NodeType;
    DDV_ColorCell Color;    /* a fixed color */
    Byte Bond;              /*  use define */
    Byte Atom;
    FloatLo BondWidth;
    Uint1 LJust;
    Uint1 LStyle;
    Int2 LScale;
} RK, PNTR PRK;


/************function prototypes***********/

extern void LIBCALL SetStructureAlgorRender PROTO((PARS pars));
extern void LIBCALL SetAlignAlgorRender PROTO((PARS pars));
extern PARS LIBCALL NewStructureRenderSet PROTO((void));
extern PARS LIBCALL NewAlignRenderSet PROTO((void));
extern void LIBCALL FreeAlgorRenderSet PROTO((PARS pars));
/*
extern PARS LIBCALL GetAlgorRenderSet PROTO((PDNMS pdnmsThis));
*/
extern void LIBCALL ResetRenderCtrls PROTO((void));
extern GrouP LIBCALL RenderControls PROTO((Nlm_GrouP prnt));
extern void LIBCALL ResetLabelCtrls PROTO((void));
extern GrouP LIBCALL LabelControls PROTO((Nlm_GrouP prnt));
extern PRK LIBCALL NewRenderKeep PROTO((void));
extern PRK LIBCALL CopyRenderKeep PROTO((PRK prkThis));
extern void LIBCALL FreeRenderKeep PROTO((PRK prkThis));
extern void LIBCALL RenderObject PROTO((PVNMO pvnmoThis));
extern void LIBCALL RenderAnAtom
    PROTO((PALD paldAtom, DDV_ColorCell * iColor, FloatLo fRadius,
    FloatHi lfAlpha));
extern Int2 LIBCALL GetGraphNCBIstdaa PROTO((PMGD pmgdThis));
extern Int2 LIBCALL GetGraphNCBI4na PROTO((PMGD pmgdThis));
extern void LIBCALL MakeStrucPalette PROTO((PDNMS pdnmsThis));
#ifdef _OPENGL
extern void LIBCALL AlgorithmicRendering PROTO((void));
extern void Cn3D_SetQualityFromAppParams(void);
#else
extern Picture3D LIBCALL Do3DOrigin PROTO((Picture3D p3d));
extern Picture3D LIBCALL AlgorithmicRendering PROTO((Picture3D p3d));
extern void Cn3d_Lock3DPalette PROTO((Picture3D ppic));
#endif
extern void Cn3D_RedrawProc PROTO((ButtoN b));
NLM_EXTERN void fnCn3D_RedrawWrapper(ButtoN b);
extern void LIBCALL fnMSPLoop PROTO((PDNMS pdnmsThis));
extern void LIBCALL fnARLoop PROTO((PDNMS pdnmsThis));

extern void Cn3D_RenStruc PROTO((IteM i));
extern void Cn3D_RenWire PROTO((IteM i));
extern void Cn3D_RenTube PROTO((IteM i));
extern void Cn3D_RenHier PROTO((IteM i));
extern void Cn3D_RenSpace PROTO((IteM i));
extern void Cn3D_RenBS PROTO((IteM i));
extern void Cn3D_RenDefault PROTO((IteM i));
extern void Cn3D_RenAlign PROTO((IteM i));


/* the number of color functions used by Cn3D*/
#define CN3DFUNCNUM 10

#define C_CPK 225
#define C_BYCHAIN 226
#define C_BYTEMP 227
#define C_BYRES 228
#define C_BYSECSTRUC 230
#define C_BYHYDRO 231
#define C_BYOBJECT 246
#define C_BYDOMAIN 247
#define C_BYSTRUCALIGN 249            /* color by structure alignment */
#define C_BYSEQCONS 250         /* color by sequence conservation */
#define C_BYCHOICE 251 /* user picks - not associated with a color function */

extern void Cn3D_ColCPK PROTO((IteM i));
extern void Cn3D_ColDomain PROTO((IteM i));
extern void Cn3D_ColCyChain PROTO((IteM i));
extern void Cn3D_ColSecStruc PROTO((IteM i));
extern void Cn3D_ColRes PROTO((IteM i));
extern void Cn3D_ColHydro PROTO((IteM i));
extern void Cn3D_ColTemp PROTO((IteM i));
extern void Cn3D_ColObject PROTO((IteM i));
extern void Cn3D_ColStrucAlign PROTO((IteM i));
extern void Cn3D_ColSeqCons PROTO((IteM i));


/* external color functions */
extern void Cn3D_Color_CPK(DDV_ColorGlobal * pColorGlobal, void *pData,
                       DDV_Range * pRange);
extern void Cn3D_Color_BYCHAIN(DDV_ColorGlobal * pColorGlobal, void *pData,
                       DDV_Range * pRange);
extern void Cn3D_Color_BYTEMP(DDV_ColorGlobal * pColorGlobal, void *pData,
                       DDV_Range * pRange);
extern void Cn3D_Color_BYRES(DDV_ColorGlobal * pColorGlobal, void *pData,
                       DDV_Range * pRange);
extern void Cn3D_Color_BYSECSTRUC(DDV_ColorGlobal * pColorGlobal, void *pData,
                       DDV_Range * pRange);
extern void Cn3D_Color_BYHYDRO(DDV_ColorGlobal * pColorGlobal, void *pData,
                       DDV_Range * pRange);
extern void Cn3D_Color_BYOBJECT(DDV_ColorGlobal * pColorGlobal, void *pData,
                       DDV_Range * pRange);
extern void Cn3D_Color_BYDOMAIN(DDV_ColorGlobal * pColorGlobal, void *pData,
                       DDV_Range * pRange);
extern void Cn3D_Color_BYSTRUCCONS(DDV_ColorGlobal * pColorGlobal, void *pData,
                       DDV_Range * pRange);
extern void Cn3D_Color_BYSEQCONS(DDV_ColorGlobal * pColorGlobal, void *pData,
                       DDV_Range * pRange);
extern void Cn3D_ColorSpecial(DDV_ColorGlobal * pColorGlobal, void *pData,
                       DDV_Range * pRange);
/* end external color functions */

extern Char * Cn3D_ColorFuncFind(void);
extern Char * Cn3D_ColorFuncName(Int4 which);

#ifdef __cplusplus
}
#endif
#endif
