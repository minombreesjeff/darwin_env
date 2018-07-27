/*   cn3dshim.h
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
* File Name:  cn3dshim.h
*
* Author: Lewis Geer
*
* Version Creation Date:   1/26/99
*
* File Description: Files use in Viewer3D -> OpenGL shim
*
* Modifications:
* $Log: cn3dshim.h,v $
* Revision 6.22  2000/07/08 20:43:57  vakatov
* Get all "#include" out of the 'extern "C" { }' scope;  other cleanup...
*
* Revision 6.21  2000/04/04 22:18:42  lewisg
* add defline to ddv, fix seq import bugs, set boundbox
*
* Revision 6.20  2000/03/20 18:18:34  thiessen
* fixed header problem causing network unavailability
*
* Revision 6.19  2000/03/18 00:06:00  lewisg
* add blast, new help, new menus
*
* Revision 6.18  2000/03/15 19:32:20  lewisg
* launch only single udv window
*
* Revision 6.17  2000/03/08 21:46:15  lewisg
* cn3d saves viewport, misc bugs
*
* Revision 6.16  2000/02/19 01:23:59  lewisg
* use ibm, add row tracking code, various bug fixes
*
* Revision 6.15  2000/02/05 01:32:21  lewisg
* add viewmgr, move place freeing is done in ddv, modify visual c++ projects
*
* Revision 6.14  2000/01/06 00:04:43  lewisg
* selection bug fixes, update message outbound, animation APIs moved to vibrant
*
* Revision 6.13  2000/01/04 15:55:51  lewisg
* don't hang on disconnected network and fix memory leak/hang at exit
*
* Revision 6.12  1999/12/27 23:14:12  lewisg
* add colormgr show/hide in Cn3D
*
* Revision 6.11  1999/12/23 21:40:33  lewisg
* move animation controls to dialog
*
* Revision 6.10  1999/12/08 22:58:01  thiessen
* added quality settings for OpenGL rendering
*
* Revision 6.9  1999/12/03 23:17:23  lewisg
* Patrick's new global update msg, argument passing when launching ddv, experimental editing
*
* Revision 6.8  1999/12/01 16:15:54  lewisg
* interim checkin to fix blocking memory leak
*
* Revision 6.7  1999/11/22 14:46:44  thiessen
* moved _OPENGL code blocks to only vibrant and ncbicn3d libraries
*
* Revision 6.6  1999/10/29 14:15:31  thiessen
* ran all Cn3D source through GNU Indent to prettify
*
* Revision 6.5  1999/10/18 15:32:50  lewisg
* move ClearSequences() to cn3dshim.c
*
* Revision 6.4  1999/10/05 23:18:25  lewisg
* add ddv and udv to cn3d with memory management
*
* Revision 6.3  1999/09/21 18:09:15  lewisg
* binary search added to color manager, various bug fixes, etc.
*
* Revision 6.2  1999/04/06 14:23:30  lewisg
* add opengl replacement for viewer3d
*
* Revision 6.1  1999/02/10 23:49:44  lewisg
* use RGB values instead of indexed palette
*
*
* ===========================================================================  */

#ifndef _CN3DSHIM_
#define _CN3DSHIM_

#include <shim3d.h>
#include <viewmgr.h>

#ifndef _OPENGL
#include <viewer3d.h>
#endif


#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _OPENGL
extern Viewer3D Cn3D_v3d;   /* the 3d view pane */
extern void Cn3D_SaveActiveCam(void);
#endif

typedef struct _Cn3D_AnimateDlg {
    WindoW Cn3D_wAnimate;
} Cn3D_AnimateDlg;


typedef struct _Cn3D_ColorData {
    DDV_ColorGlobal *pDDVColorGlobal;
    Boolean IsUserData;     /* is the DDV_ColorGlobal userdata? */
    SeqAnnot *sap;          /* the current seqalign */
    ValNode *pvnsep;        /* the current seqentry */
    Boolean StandAlone;     /* is Cn3D running standalone? */
    Uint2 sapprocid, sepprocid, userkey;
    WindoW Cn3D_w;
    Cn3D_AnimateDlg AnimateDlg;
    Boolean UseEntrez;  /* turn on entrez use */
    Boolean EntrezOn;   /* is entrez on? */
    Int4 rows;          /* number of rows in initial seqalign */
    Boolean AlignMode;  /* is Cn3D showing alignments or sequences? */
    Boolean IBM;        /* are we using intersect by master? */
    IteM BlastMany;     /* blast many menu item */
#ifdef _OPENGL
    TOGL_Data *OGL_Data;    /* pointer to OGL data */
#endif
} TCn3D_ColorData;

extern TCn3D_ColorData Cn3D_ColorData; /* where all dynamic color info is kept */



/*****************************************************************************

Function: Cn3D_SetVisible()

Purpose: Sets the visible bit for a chain in the biostruc AND the color
        manager
  
Parameters: pColorGlobal
            pmmdThis: the chain
            fVisible: true if the chain is to be visible

*****************************************************************************/

NLM_EXTERN void Cn3D_SetVisible(DDV_ColorGlobal *pColorGlobal, PMMD pmmdThis,
                                Boolean fVisible);


/*****************************************************************************

Function: Cn3D_IsVisible()

Purpose: Gets the visible bit for a chain in the biostruc.
  
Parameters: pColorGlobal
            pmmdThis: the chain

Returns:  TRUE if visible

*****************************************************************************/

NLM_EXTERN Boolean Cn3D_IsVisible(DDV_ColorGlobal *pColorGlobal,
                                   PMMD pmmdThis);


/*****************************************************************************

Function: ClearSequences()

Purpose: Deletes the Cn3D messagefunc from userdata on the SeqAnnots and
         SeqEntries presently displayed.
  
*****************************************************************************/
extern void ClearSequences(void);

extern void Cn3D_ConstructColorData(TCn3D_ColorData * ColorData
#ifdef _OPENGL
                                    , TOGL_Data * OGL_Data
#endif
                                    , Boolean StandAlone);

/* destructor for Color structure */
extern void Cn3D_DestructColorData(TCn3D_ColorData * ColorData);

extern void fnCHLresidue(PDNMG pdnmgThis,
#ifndef _OPENGL
                         Viewer3D vvv,
#endif
                         Boolean highlight);


/*****************************************************************************

Function: Cn3D_FindFeature

Purpose: Finds a feature of a given type in the feature set
  
Parameters: bsfsp, the head of the BiostrucFeatureSets to search
            type, the type of feature to look for
            num, the nth occurrence of the feature type

Returns: The feature if found,  NULL otherwise
            
*****************************************************************************/
NLM_EXTERN BiostrucFeature *Cn3D_FindFeature(BiostrucFeatureSetPtr bsfsp,
                                             Int4 type, Int4 num);

/*****************************************************************************

Function: Cn3D_AddFeatureSet

Purpose: Adds a new BiostrucFeatureSet the feature set
  
Parameters: bsfspp, pointer to the head of the BiostrucFeatureSets

Returns: the feature set
            
*****************************************************************************/
NLM_EXTERN BiostrucFeatureSetPtr Cn3D_AddFeatureSet(
                                       BiostrucFeatureSetPtr *bsfspp);


#ifdef _OPENGL
extern void LIBCALL Cn3D_Size(TOGL_BoundBox * BoundBox,
                              PDNMS pdnmsThis);
NLM_EXTERN void Cn3D_Animate(IteM i);
extern Nlm_GrouP LIBCALL OGL_Quality(Nlm_GrouP prnt);


/*****************************************************************************

Function: Cn3D_Matrix2Asn

Purpose: Converts an OpenGL matrix to an ASN.1 GLMatrix
  
Parameters: glmatrix, the destination ASN.1 GLMatrix
            modelmatrixin, the source opengl matrix
            
*****************************************************************************/
NLM_EXTERN void Cn3D_Matrix2Asn (GLMatrix *glmatrix, void *modelmatrix);

/*****************************************************************************

Function: Cn3D_Asn2Matrix

Purpose: Converts an ASN.1 GLMatrix to an OpenGL matrix
  
Parameters: modelmatrixin, the destination opengl matrix
            glmatrix, the source ASN.1 GLMatrix

*****************************************************************************/
NLM_EXTERN void Cn3D_Asn2Matrix (void *modelmatrix, GLMatrix *glmatrix);

#endif                          /* _OPENGL */

#ifdef __cplusplus
}
#endif

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

#endif                          /* _CN3DSHIM_ */
