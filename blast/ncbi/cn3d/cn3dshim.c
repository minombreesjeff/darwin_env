/*   cn3dshim.c
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
* File Name:  cn3dshim.c
*
* Authors:  Lewis Geer
*
* Version Creation Date:
*
* File Description: Shim functions necessary to convert to OpenGL
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: cn3dshim.c,v $
* Revision 6.30  2001/04/27 20:08:04  juran
* Heed warnings.
*
* Revision 6.29  2000/07/23 12:37:23  thiessen
* allow dynamic slave->master transformation
*
* Revision 6.28  2000/04/08 00:37:31  lewisg
* multiple seqentries, NEWSEQ message, etc.
*
* Revision 6.27  2000/04/04 22:18:42  lewisg
* add defline to ddv, fix seq import bugs, set boundbox
*
* Revision 6.26  2000/03/20 18:18:33  thiessen
* fixed header problem causing network unavailability
*
* Revision 6.25  2000/03/08 21:46:15  lewisg
* cn3d saves viewport, misc bugs
*
* Revision 6.24  2000/02/19 01:23:59  lewisg
* use ibm, add row tracking code, various bug fixes
*
* Revision 6.23  2000/02/05 01:32:21  lewisg
* add viewmgr, move place freeing is done in ddv, modify visual c++ projects
*
* Revision 6.22  2000/01/18 22:49:16  lewisg
* send OM_MSG_FLUSH to ddv/udv, tweak CPK coloration, misc bugs
*
* Revision 6.21  2000/01/08 00:47:53  lewisg
* fixes to selection, update, color
*
* Revision 6.20  2000/01/06 00:04:42  lewisg
* selection bug fixes, update message outbound, animation APIs moved to vibrant
*
* Revision 6.19  2000/01/04 15:55:51  lewisg
* don't hang on disconnected network and fix memory leak/hang at exit
*
* Revision 6.18  1999/12/29 22:55:03  lewisg
* get rid of seqalign id
*
* Revision 6.17  1999/12/28 15:08:44  lewisg
* remove remaining mediainfo code
*
* Revision 6.16  1999/12/27 23:14:12  lewisg
* add colormgr show/hide in Cn3D
*
* Revision 6.15  1999/12/23 21:40:33  lewisg
* move animation controls to dialog
*
* Revision 6.14  1999/12/02 20:31:59  lewisg
* put seqentries into bioseqset and fix calling convention in alignmgr.c
*
* Revision 6.13  1999/12/01 16:15:54  lewisg
* interim checkin to fix blocking memory leak
*
* Revision 6.12  1999/11/30 22:46:37  vakatov
* Cast callback arg in Cn3D_SizeCB() lest to cast the callback func.type
*
* Revision 6.11  1999/11/12 16:06:34  lewisg
* fix sequentry to valnode conversion
*
* Revision 6.10  1999/11/10 23:19:42  lewisg
* rewrite of selection code for ddv
*
* Revision 6.9  1999/11/02 23:06:08  lewisg
* fix cn3d to launch correctly if there is no seqentry associated with bioseq
*
* Revision 6.8  1999/10/29 14:15:30  thiessen
* ran all Cn3D source through GNU Indent to prettify
*
* Revision 6.7  1999/10/18 15:32:50  lewisg
* move ClearSequences() to cn3dshim.c
*
* Revision 6.6  1999/10/15 20:56:40  lewisg
* append DDV_ColorGlobal as userdata.  free memory when cn3d terminates.
*
* Revision 6.5  1999/10/05 23:18:24  lewisg
* add ddv and udv to cn3d with memory management
*
* Revision 6.4  1999/09/22 20:07:39  thiessen
* minor fix for mac compiler
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
*/


#include <cn3dmain.h>
#include <cn3dmsg.h>
#include <ncbi.h>
#include <cn3dshim.h>
#include <seqmgr.h>
#include <salpacc.h>
#include <asn.h>
#include <objall.h>
#include <strimprt.h>
#include <objmmdb1.h>
#include <objmmdb2.h>
#include <objmmdb3.h>
#include <algorend.h>


#ifdef _OPENGL
#include <undefwin.h>
#ifdef _WIN32
#include <windows.h>
#endif

#if defined(macintosh)
#include <gl.h>
#include <glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif
#endif  /* _OPENGL */


/*****************************************************************************

Function: Cn3D_SetVisible()

Purpose: Sets the visible bit for a chain in the biostruc AND the color
        manager
  
Parameters: pColorGlobal
            pmmdThis: the chain
            fVisible: TRUE if the chain is to be visible

*****************************************************************************/

NLM_EXTERN void Cn3D_SetVisible(DDV_ColorGlobal *pColorGlobal, PMMD pmmdThis,
                                Boolean fVisible)
{
    if(pmmdThis == NULL) return;
    if(pmmdThis->pSeqId == NULL) return;
    DDV_SetVisible(pColorGlobal, pmmdThis->pSeqId, -1, fVisible);
    pmmdThis->bVisible = fVisible;
}


/*****************************************************************************

Function: Cn3D_IsVisible()

Purpose: Gets the visible bit for a chain in the biostruc.
  
Parameters: pColorGlobal
            pmmdThis: the chain

Returns:  TRUE if visible

*****************************************************************************/

NLM_EXTERN Boolean Cn3D_IsVisible(DDV_ColorGlobal *pColorGlobal,
                                   PMMD pmmdThis)
{
    if(pmmdThis == NULL) return FALSE;
    return pmmdThis->bVisible;
}

/*****************************************************************************

Function: ClearSequences()

Purpose: Deletes the Cn3D messagefunc from userdata on the SeqAnnots and
         SeqEntries presently displayed.
  
*****************************************************************************/

void ClearSequences(void)
{
    Uint2 entityID;
    SeqAnnot *sap;
    SeqAlign *salp;
    SeqEntry *sep;
    BioseqSet *bssp;
    Bioseq *bsp;
    ValNode *pvnsep, *pvnnext;
    
    if (Cn3D_ColorData.sap != NULL) {
        for (sap = Cn3D_ColorData.sap; sap != NULL; sap = sap->next) {        
            if (sap->data == NULL) continue;
            salp = sap->data;
            entityID = ObjMgrGetEntityIDForPointer(salp);
            ObjMgrFreeUserData(entityID, Cn3D_ColorData.sapprocid, OMPROC_EDIT,
                Cn3D_ColorData.userkey);               
            ObjMgrSendMsg(OM_MSG_FLUSH, entityID, 0, 0);
            ObjMgrSendMsg (OM_MSG_DEL, entityID, 0, 0);
        }
    }
    Cn3D_ColorData.sap = NULL;

    /* this is a hack, but seems harmless?  objects are not freed until
    the viewer windows die.  Viewer windows don't die until the event loop.
    This is because there could be a few mouse clicks remaining.  The problem
    is that when loading a new sequence when the old sequence is identical,
    the object manager gets completely confused because the old sequence is not
    dead yet.  The following loop makes it play dead. */

    if(Cn3D_ColorData.pvnsep != NULL) { 
        bssp = Cn3D_ColorData.pvnsep->data.ptrvalue;
        for(sep = bssp->seq_set; sep != NULL; sep = sep->next) {
            bsp = sep->data.ptrvalue;
            SeqMgrDeleteFromBioseqIndex(bsp);
        }
    }
 
    for(pvnsep = Cn3D_ColorData.pvnsep; pvnsep != NULL;
            pvnsep = pvnnext) {  
        pvnnext = pvnsep->next;
        entityID =
            ObjMgrGetEntityIDForPointer((void *) pvnsep);
        ObjMgrFreeUserData(entityID, Cn3D_ColorData.sepprocid, OMPROC_VIEW,
            Cn3D_ColorData.userkey);
        ObjMgrSendMsg(OM_MSG_FLUSH, entityID, 0, 0);
        ObjMgrSendMsg (OM_MSG_DEL, entityID, 0, 0);
    }
    
    if(Cn3D_ColorData.IsUserData == FALSE)
        DDV_DeleteColorGlobal(Cn3D_ColorData.pDDVColorGlobal);
    Cn3D_ColorData.pDDVColorGlobal = NULL;
    Cn3D_ColorData.pvnsep = NULL;
    
    return;
}


/* constructor for DDV_ColorEntry structure */
void Cn3D_ConstructColorData(TCn3D_ColorData * ColorData
#ifdef _OPENGL
                             , TOGL_Data * OGL_Data
#endif
                             , Boolean StandAlone)
{

    if (ColorData == NULL)
        return;
#ifdef _OPENGL
    if (OGL_Data == NULL)
        return;
#endif
    ColorData->sap = NULL;
    ColorData->pvnsep = NULL;
    ColorData->StandAlone = StandAlone;
    ColorData->pDDVColorGlobal = NULL;
    ColorData->IsUserData = FALSE;
#ifdef _OPENGL
    ColorData->OGL_Data = OGL_Data;
#endif
    ColorData->AnimateDlg.Cn3D_wAnimate = NULL;
    ColorData->Cn3D_w = NULL;
}

/* destructor for Color structure */
void Cn3D_DestructColorData(TCn3D_ColorData * ColorData)
{
    if (ColorData == NULL)
        return;
    DDV_DeleteColorGlobal(ColorData->pDDVColorGlobal);
}

/*****************************************************************************

Function: Cn3D_FindFeature

Purpose: Finds a feature of a given type in the feature set
  
Parameters: bsfsp, the head of the BiostrucFeatureSets to search
            type, the type of feature to look for
            num, the nth occurrence of the feature type

Returns: The feature if found,  NULL otherwise
            
*****************************************************************************/
NLM_EXTERN BiostrucFeature *Cn3D_FindFeature(BiostrucFeatureSetPtr bsfsp,
                                             Int4 type, Int4 num)
{
    BiostrucFeature *bsfp;
    Int4 i = 0;

    for(;bsfsp != NULL; bsfsp = bsfsp->next) {
        for(bsfp = bsfsp->features; bsfp != NULL; bsfp = bsfp->next) {
            if(bsfp->type == type) {
                i++;
                if(i >= num) return bsfp;
            }
        }
    }
    return NULL;
}

/*****************************************************************************

Function: Cn3D_AddFeatureSet

Purpose: Adds a new BiostrucFeatureSet the feature set
  
Parameters: bsfspp, pointer to the head of the BiostrucFeatureSets

Returns: the feature set
            
*****************************************************************************/
NLM_EXTERN BiostrucFeatureSetPtr Cn3D_AddFeatureSet(
                                       BiostrucFeatureSetPtr *bsfspp)
{
    BiostrucFeatureSetPtr bsfsp;

    if(*bsfspp == NULL) {
        *bsfspp = BiostrucFeatureSetNew();
        return *bsfspp;
    }
    else {
        for (bsfsp = *bsfspp; bsfsp->next != NULL; bsfsp = bsfsp->next) continue;
        bsfsp->next = BiostrucFeatureSetNew();
        return bsfsp->next;
    }
}


/************************* OpenGL specific functions *****************************/

#ifdef _OPENGL




static void LIBCALLBACK Cn3D_SizeCB(PFB pfbThis, Nlm_Int4 iModel,
                                    Nlm_Int4 iIndex,
                                    Pointer ptr)
/* callback used to find the bounding box of the atoms of a structure */
{
    TOGL_BoundBox* BoundBox = (TOGL_BoundBox*) ptr;
    PMAD pmadThis = NULL;
    PALD paldThis = NULL;
    Nlm_FloatLoPtr pflvDataThis = NULL;
    Nlm_Int4 i;

    if (pfbThis == NULL || BoundBox == NULL)
        return;
    if (pfbThis->bMe == (Byte) AM_MAD) { /* is this molecular atom data? *//* iIndex isn't used */
        pmadThis = (PMAD) pfbThis;
        paldThis = GetAtomLocs(pmadThis, iModel);
        while (paldThis) {
            pflvDataThis = paldThis->pflvData;
            if (BoundBox->set == FALSE) { /* if the bounding box has not been used, set it */
                for (i = 0; i < 6; i++) {
                    BoundBox->x[i] = pflvDataThis[i / (int) 2];
                    BoundBox->set = TRUE;
                }
            } else {
                for (i = 0; i < 6; i += 2) { /* check to see if the atom lies outside the bound box */
                    if (pflvDataThis[i / (Nlm_Int4) 2] < BoundBox->x[i])
                        BoundBox->x[i] = pflvDataThis[i / (Nlm_Int4) 2];
                    if (pflvDataThis[i / (Nlm_Int4) 2] >
                        BoundBox->x[i + 1]) BoundBox->x[i + 1] =
                            pflvDataThis[i / (Nlm_Int4) 2];
                }
            }
            paldThis = paldThis->next;
        }
    }
}


void LIBCALL Cn3D_Size(TOGL_BoundBox * BoundBox, PDNMS pdnmsThis)
/* find coordinates of the bounding box of a structure */
{
    PDNMS pdnmsSlave;
    PMSD pmsdThis;

    if (BoundBox == NULL || pdnmsThis == NULL) return;
    OGL_ClearBoundBox(BoundBox);
    BoundBox->set = FALSE;
    TraverseModels(pdnmsThis, TRAVERSE_ATOM, 0, BoundBox, Cn3D_SizeCB);
    /* don't do slaves, since their coordinates are no longer trasnformed onto master
    pmsdThis = pdnmsThis->data.ptrvalue;
    for(pdnmsSlave = pmsdThis->pdnmsSlaves; pdnmsSlave != NULL;
            pdnmsSlave = pdnmsSlave->next)
        TraverseModels(pdnmsSlave, TRAVERSE_ATOM, 0, BoundBox, Cn3D_SizeCB);
    */
}


/* Buttons on the OGL Viewer UI */

static ButtoN OGL_allButton;
static ButtoN OGL_rewindButton;
static ButtoN OGL_prevButton;
static ButtoN OGL_nextButton;
#ifndef WIN_MAC
static ButtoN OGL_playButton;
#endif                          /* ndef WIN_MAC */


/* the OpenGL UI */

static void AllLayerOnProc(Nlm_ButtoN b)
{
    OGL_StopPlaying(Cn3D_ColorData.OGL_Data);
    OGL_AllLayerOnProc(Cn3D_ColorData.OGL_Data);
    OGL_DrawViewer3D(Cn3D_ColorData.OGL_Data);
}

static void RewindLayerProc(Nlm_ButtoN b)
{
    OGL_StopPlaying(Cn3D_ColorData.OGL_Data);
    OGL_RewindLayerProc(Cn3D_ColorData.OGL_Data);
    OGL_DrawViewer3D(Cn3D_ColorData.OGL_Data);
}

static void PrevLayerProc(Nlm_ButtoN b)
{
    OGL_StopPlaying(Cn3D_ColorData.OGL_Data);
    OGL_PrevLayerProc(Cn3D_ColorData.OGL_Data);
    OGL_DrawViewer3D(Cn3D_ColorData.OGL_Data);
}

static void NextLayerProc(Nlm_ButtoN b)
{
    OGL_StopPlaying(Cn3D_ColorData.OGL_Data);
    OGL_NextLayerProc(Cn3D_ColorData.OGL_Data);
    OGL_DrawViewer3D(Cn3D_ColorData.OGL_Data);
}

static void PlayOGL(Nlm_ButtoN b)
{
#ifndef WIN_MAC
    if( OGL_IsPlaying(Cn3D_ColorData.OGL_Data)) {
        OGL_StopPlaying(Cn3D_ColorData.OGL_Data);
        return;
    }
    OGL_StartPlaying(Cn3D_ColorData.OGL_Data);
#endif
}

static void Cn3D_AnimateCloseProc(ButtoN b)
{
    WindoW hAnimateDlg;

	hAnimateDlg= (WindoW)ParentWindow(b);
	if(hAnimateDlg == NULL) return;	

    OGL_StopPlaying(Cn3D_ColorData.OGL_Data);
    Remove(Cn3D_ColorData.AnimateDlg.Cn3D_wAnimate);
    Cn3D_ColorData.AnimateDlg.Cn3D_wAnimate = NULL;
    return;
}


NLM_EXTERN void Cn3D_Animate(IteM i)
{
    GrouP h;
   
    if(Cn3D_ColorData.AnimateDlg.Cn3D_wAnimate != NULL) return;
    Cn3D_ColorData.AnimateDlg.Cn3D_wAnimate =
        FixedWindow(-30, -20, -10, -10, "Model and Animation Controls",
                    NULL);
    OGL_StopPlaying(Cn3D_ColorData.OGL_Data);
    h = HiddenGroup(Cn3D_ColorData.AnimateDlg.Cn3D_wAnimate, 
#ifndef WIN_MAC
        6,
#else
        7,
#endif
        1, NULL);


    OGL_rewindButton = Nlm_PushButton(h, "|<<", RewindLayerProc);
    OGL_prevButton = Nlm_PushButton(h, "|< ", PrevLayerProc);
#ifndef WIN_MAC
    OGL_playButton = Nlm_PushButton(h, " > ", PlayOGL);
#endif
    OGL_nextButton = Nlm_PushButton(h, ">| ", NextLayerProc);
    OGL_allButton = Nlm_PushButton(h, "All", AllLayerOnProc);
    PushButton(h, "Close", (BtnActnProc) Cn3D_AnimateCloseProc);

    Show(Cn3D_ColorData.AnimateDlg.Cn3D_wAnimate);
    return;
}

/*****************************************************************************

Function: Cn3D_Asn2Matrix

Purpose: Converts an ASN.1 GLMatrix to an OpenGL matrix
  
Parameters: modelmatrixin, the destination opengl matrix
            glmatrix, the source ASN.1 GLMatrix

*****************************************************************************/
NLM_EXTERN void Cn3D_Asn2Matrix (void *modelmatrixin, GLMatrix *glmatrix)
{
    GLdouble *modelmatrix;

    if(modelmatrixin == NULL || glmatrix == NULL) return;
    modelmatrix = (GLdouble *)modelmatrixin;

    if(glmatrix->scale == 0) return;

    modelmatrix[0] = glmatrix->m11/(double)glmatrix->scale;
    modelmatrix[1] = glmatrix->m12/(double)glmatrix->scale;
    modelmatrix[2] = glmatrix->m13/(double)glmatrix->scale;
    modelmatrix[3] = glmatrix->m14/(double)glmatrix->scale;

    modelmatrix[4] = glmatrix->m21/(double)glmatrix->scale;
    modelmatrix[5] = glmatrix->m22/(double)glmatrix->scale;
    modelmatrix[6] = glmatrix->m23/(double)glmatrix->scale;
    modelmatrix[7] = glmatrix->m24/(double)glmatrix->scale;

    modelmatrix[8] = glmatrix->m31/(double)glmatrix->scale;
    modelmatrix[9] = glmatrix->m32/(double)glmatrix->scale;
    modelmatrix[10] = glmatrix->m33/(double)glmatrix->scale;
    modelmatrix[11] = glmatrix->m34/(double)glmatrix->scale;

    modelmatrix[12] = glmatrix->m41/(double)glmatrix->scale;
    modelmatrix[13] = glmatrix->m42/(double)glmatrix->scale;
    modelmatrix[14] = glmatrix->m43/(double)glmatrix->scale;
    modelmatrix[15] = glmatrix->m44/(double)glmatrix->scale;

    return;
}

/*****************************************************************************

Function: Cn3D_Matrix2Asn

Purpose: Converts an OpenGL matrix to an ASN.1 GLMatrix
  
Parameters: glmatrix, the destination ASN.1 GLMatrix
            modelmatrixin, the source opengl matrix
            
*****************************************************************************/
NLM_EXTERN void Cn3D_Matrix2Asn (GLMatrix *glmatrix, void *modelmatrixin)
{
    GLdouble *modelmatrix;

    if(modelmatrixin == NULL || glmatrix == NULL) return;
    modelmatrix = (GLdouble *)modelmatrixin;
    glmatrix->scale = (Int4)VIEWSCALE;

    glmatrix->m11 = (Int4)(modelmatrix[0]*glmatrix->scale);
    glmatrix->m12 = (Int4)(modelmatrix[1]*glmatrix->scale);
    glmatrix->m13 = (Int4)(modelmatrix[2]*glmatrix->scale);
    glmatrix->m14 = (Int4)(modelmatrix[3]*glmatrix->scale);

    glmatrix->m21 = (Int4)(modelmatrix[4]*glmatrix->scale);
    glmatrix->m22 = (Int4)(modelmatrix[5]*glmatrix->scale);
    glmatrix->m23 = (Int4)(modelmatrix[6]*glmatrix->scale);
    glmatrix->m24 = (Int4)(modelmatrix[7]*glmatrix->scale);

    glmatrix->m31 = (Int4)(modelmatrix[8]*glmatrix->scale);
    glmatrix->m32 = (Int4)(modelmatrix[9]*glmatrix->scale);
    glmatrix->m33 = (Int4)(modelmatrix[10]*glmatrix->scale);
    glmatrix->m34 = (Int4)(modelmatrix[11]*glmatrix->scale);

    glmatrix->m41 = (Int4)(modelmatrix[12]*glmatrix->scale);
    glmatrix->m42 = (Int4)(modelmatrix[13]*glmatrix->scale);
    glmatrix->m43 = (Int4)(modelmatrix[14]*glmatrix->scale);
    glmatrix->m44 = (Int4)(modelmatrix[15]*glmatrix->scale);

    return;
}


#endif                          /* _OPENGL */
