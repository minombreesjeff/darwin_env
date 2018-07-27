/*   $Id: cn3dstyl.c,v 6.10 2000/05/10 14:15:40 thiessen Exp $
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
* File Name:  $Id: cn3dstyl.c,v 6.10 2000/05/10 14:15:40 thiessen Exp $
*
* Author: Yanli Wang
*
* Version Creation Date: 2/19/2000
*
* File Description: code to handle special styles/features
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: cn3dstyl.c,v $
* Revision 6.10  2000/05/10 14:15:40  thiessen
* fix minor GUI bug in Annotate panel
*
* Revision 6.9  2000/04/20 23:27:45  lewisg
* misc bug fixes
*
* Revision 6.8  2000/04/17 21:46:56  lewisg
* do not do double index on viewmgr update, rename menus
*
* Revision 6.7  2000/03/22 23:17:50  thiessen
* added ability to save ARS in ASN1
*
* Revision 6.6  2000/03/17 22:48:59  thiessen
* fix for multi-chain / multi-model features ; added feature-move ; misc bug fixes
*
* Revision 6.5  2000/03/13 16:04:26  thiessen
* re-implementation of features
*
* Revision 6.4  2000/03/08 21:46:15  lewisg
* cn3d saves viewport, misc bugs
*
* Revision 6.3  2000/03/01 16:17:55  thiessen
* improved handling of colors; many small fixes
*
* Revision 6.2  2000/02/26 00:01:40  thiessen
* OpenGL improvements, progress on cleanup of Show/Hide
*
* Revision 6.1  2000/02/19 21:25:58  thiessen
* split of cn3dmodl into cn3dmodl and cn3dstyl
*
*
*
* ==========================================================================
*/

#include <ncbi.h>
#ifdef _OPENGL
#include <shim3d.h>
#else
#include <viewer3d.h>
#endif
#include <cn3dmain.h>
#include <math.h>
#include <mmdbapi.h>
#include <mmdbapi1.h>
#include <mmdbapi2.h>
#include <mmdbapi3.h>
#include <mmdbapi4.h>
#include <algorend.h>
#include <cn3dmsg.h>
#include <salmedia.h>
#include <salutil.h>
#include <cn3dopen.h>
#include <objmime.h>
#include <cn3dshim.h>
#include <cn3dstyl.h>


typedef struct special_feature_info {
    PARS parsSpecial;
    Boolean isOn;
    CharPtr name, description;
    /* parallel lists of models and locations this feature applies to */
    ValNodePtr pvnModels; /* data.ptrvalue holds PMSD */
    ValNodePtr pvnLocations; /* data.ptrvalue holds BiostrucFeaturePtr->Location_location */
} SpecialFeatureInfo, PNTR SpecialFeatureInfoPtr;

typedef ValNodePtr SpecialFeaturePtr;

static void LIBCALL ResetModelCtrls(void);

static ValNodePtr GetBiostrucFeatureLocationForHihglightedResidues(PDNMS pdnmsThis,
                                                                  Int2 iFeature);
static void Cn3DApplyUserDefinedFeatureToMGD(PMSD pmsdThis, ValNodePtr llp,
                                      Int2 iFeature, SpecialFeatureInfoPtr sfip);


static ENUM_ALIST(empty_alist)
  {"              ",     1},
END_ENUM_ALIST

static ENUM_ALIST(all_render_alist)
  {"Wireframe",          1},
  {"Tubes",              2},
  {"Ball & Stick",       3},
  {"Fat Tubes",          4},
  {"Space Fill   ",      5},
END_ENUM_ALIST

static ENUM_ALIST(all_color_alist)
  {"red",           1},
  {"green",         2},
  {"magenta",       3},
  {"sky",           4},
  {"purple",        5},
  {"Select...",     6},
END_ENUM_ALIST

static EnumFieldAssocPtr renderAlist[4] =
    { all_render_alist, all_render_alist, all_render_alist, all_render_alist };

static EnumFieldAssocPtr colorAlist[4] =
    { all_color_alist, all_color_alist, all_color_alist, all_color_alist };

static ButtoN Cn3D_lModelOnOffItem[4]; /* pieces parts to draw */
static PopuP Cn3D_pupModelPBB;  /* protein backbone options */
static PopuP Cn3D_pupModelNABB; /* nucl. acid bb options */
static PopuP Cn3D_pupModelStyleItem;
static PopuP Cn3D_pupModelRenderStyle[4];
static PopuP Cn3D_pupModelColorStyle[4];
static TexT FeatureTitle;
static TexT FeatureDescription;
static ButtoN bResetFeatureTitle;

static ButtoN Cn3D_lOnOffLabel[4];
static PopuP Cn3D_pupLabelAA;
static PopuP Cn3D_pupLabelNT;
static PopuP Cn3D_bLName[4];
static ButtoN Cn3D_bLNum[4];
static ButtoN Cn3D_bLWhite[4];
#ifndef _OPENGL
static ButtoN Cn3D_bLTop[4];
static PopuP Cn3D_pupLabelSize[4];
#endif

static ButtoN Cn3D_bModelApply;

static ButtoN bFeatureAdd;
static ButtoN bFeatureMove;
static ButtoN bFeatureDelete;
static ButtoN bFeatureEdit;
static LisT Cn3D_lFeaturesEdited;
static LisT Cn3D_lFeaturesOnOff;

static SpecialFeaturePtr sfpGlobal = NULL;
static SpecialFeaturePtr sfpThis_head = NULL, sfpThis_tail = NULL;
static PARS parsSpecial = NULL;
static Byte iNumFeatures = 0, iEditedFeature = 0;


/*-----------------------------------------------*/


/*---------------------------------------------------------*/
static void Cn3D_SyncMenuHighlightWithOnOff(void)
{
    SpecialFeaturePtr sfpThis = NULL;
    SpecialFeatureInfoPtr sfipThis = NULL;
    Int4 iCount = 1;

    Reset(Cn3D_lFeaturesOnOff);

    sfpThis = sfpGlobal;
    while (sfpThis) {
        sfipThis = sfpThis->data.ptrvalue;
        ListItem(Cn3D_lFeaturesOnOff, (CharPtr) sfipThis->name);
        SetItemStatus(Cn3D_lFeaturesOnOff, iCount, sfipThis->isOn);
        iCount++;
        sfpThis = sfpThis->next;
    }
}

/*---------------------------------------------------------*/
static void Cn3D_FeatureOnOffProc(LisT l)
{
    SpecialFeaturePtr sfpThis = NULL;
    SpecialFeatureInfoPtr sfipThis = NULL;
    Int4 iCount = 1;

    sfpThis = sfpGlobal;
    while (sfpThis) {
        sfipThis = sfpThis->data.ptrvalue;
        sfipThis->isOn = GetItemStatus(Cn3D_lFeaturesOnOff, iCount);
        iCount++;
        sfpThis = sfpThis->next;
    }
}

/*---------------------------------------------------------*/
static Int2 GetRenderItemIndex(Int2 k)
{
    Int2 i = 1;

    switch (k) {
    case R_WIRE:
        i = 1;
        break;
    case R_STICK:
        i = 2;
        break;
    case R_BALLNSTICK:
        i = 3;
        break;
    case R_THICKWIRE:
        i = 4;
        break;
    case R_SPACE:
        i = 5;
        break;
    default:
        ;
    }

    return i;
}

/*----------------------------------------------*/
static Int2 GetCn3d_PaletteRGBIndex(Uint1Ptr rgb)
{
    Int2 iCount = 0, iColor = 0;

    for (iCount = 0; iCount < CN3D_COLOR_MAX; iCount++) {
        if (*rgb == Cn3d_PaletteRGB[iCount][0] && 
            *(rgb + 1) == Cn3d_PaletteRGB[iCount][1] &&
            *(rgb + 2) == Cn3d_PaletteRGB[iCount][2]) {
            iColor = iCount;
            break;
        }
    }
    return (iColor);
}

/*---------------------------------------------------------*/
static Int2 GetColorItemIndex(Uint1 *rgb)
{
    Int2 i;

    switch (GetCn3d_PaletteRGBIndex(rgb)) {
    case C_red:
        i = 1;
        break;
    case C_green:
        i = 2;
        break;
    case C_magenta:
        i = 3;
        break;
    case C_sky:
        i = 4;
        break;
    case C_purple:
        i = 5;
        break;
    default:
        i = 6;
    }
    return i;
}

/*---------------------------------------------------------*/
static void UpdateEditFeatureList()
{
    SpecialFeaturePtr sfpThis = NULL;
    SpecialFeatureInfoPtr sfipThis = NULL;

    Reset(Cn3D_lFeaturesEdited);
    sfpThis = sfpGlobal;
    while (sfpThis) {
        sfipThis = sfpThis->data.ptrvalue;
        ListItem(Cn3D_lFeaturesEdited, (CharPtr) sfipThis->name);
        sfpThis = sfpThis->next;
    }
    SetValue(Cn3D_lFeaturesEdited, 0);
}

/*---------------------------------------------------------*/
void LIBCALLBACK DecrementFeatureID(PFB pfbThis, Int4 unused,
                                    Int4 iRemovedFeatureID, Pointer ptr)
{
    ValNodePtr pvn = ((PMGD) pfbThis)->pvnPARSList;
    while (pvn && pvn->choice > iRemovedFeatureID) {
        pvn->choice--;
        pvn = pvn->next;
    }
}

/*---------------------------------------------------------*/
/* if a feature is removed, renumber later (higher id) features so
   that feature id's remain consecutive */
static void CollapseFeatureIDs(Int4 iRemovedFeatureID)
{
    SpecialFeaturePtr sfpThis = sfpGlobal;
    PDNMS pdnmsMaster = NULL, pdnmsSlave = NULL;

    /* renumber in feature list */
    while (sfpThis) {
        if (sfpThis->choice > iRemovedFeatureID)
            sfpThis->choice--;
        sfpThis = sfpThis->next;
    }

    /* renumber id's in MG lists */
    pdnmsMaster = GetSelectedModelstruc();
    TraverseGraphs(pdnmsMaster, 0, iRemovedFeatureID, NULL, DecrementFeatureID);
    pdnmsSlave = ((PMSD) pdnmsMaster->data.ptrvalue)->pdnmsSlaves;
    while (pdnmsSlave) {
        TraverseGraphs(pdnmsSlave, 0, iRemovedFeatureID, NULL, DecrementFeatureID);
        pdnmsSlave = pdnmsSlave->next;
    }
}

/*---------------------------------------------------------*/
void LIBCALLBACK RemovePARSFromMGD(PFB pfbThis, Int4 unused,
                                   Int4 iFeature, Pointer ptr)
{
    PMGD pmgdThis = (PMGD) pfbThis;

    if (!pmgdThis) return;
    ValNodeExtract(&(pmgdThis->pvnPARSList), iFeature);
}

/*---------------------------------------------------------*/
/* add to list, always maintaining descending order of "choice" (for now) */
static void AddPARSToMGD(PMGD pmgdThis, Int4 iFeature, PARS pars)
{
    ValNodePtr pvn, before, after;

    if (!pmgdThis) return;
   
    if (pmgdThis->pvnPARSList->choice < iFeature) { /* add to head */
        pvn = ValNodeNew(NULL);
        pvn->choice = iFeature;
        pvn->data.ptrvalue = (VoidPtr) pars;
        pvn->next = pmgdThis->pvnPARSList;
        pmgdThis->pvnPARSList = pvn;
        return;
    }
    for (before = pmgdThis->pvnPARSList; before; before = before->next) {
        after = before->next;
        if (before->choice == iFeature || after->choice == iFeature)
            return; /* already present */
        if (after->choice < iFeature) {
            pvn = ValNodeNew(NULL);
            pvn->choice = iFeature;
            pvn->data.ptrvalue = (VoidPtr) pars;
            pvn->next = after;
            before->next = pvn;
            return;
        }
    }
}

/*---------------------------------------------------------*/
static void RemoveFeatureFromAllMGDs(Int4 iFeature)
{
    PDNMS pdnmsMaster = NULL, pdnmsSlave = NULL;

    pdnmsMaster = GetSelectedModelstruc();
    if (!pdnmsMaster) return;

    TraverseGraphs(pdnmsMaster, 0, iFeature, NULL, RemovePARSFromMGD);
    pdnmsSlave = ((PMSD) pdnmsMaster->data.ptrvalue)->pdnmsSlaves;
    while (pdnmsSlave) {
        TraverseGraphs(pdnmsSlave, 0, iFeature, NULL, RemovePARSFromMGD);
        pdnmsSlave = pdnmsSlave->next;
    }
}

/*---------------------------------------------------------*/
static Boolean Cn3D_CheckHLStatus(void)
{
    PDNMS pdnmsThis = NULL, pdnmsSlave = NULL;
    PMSD pmsdMaster = NULL, pmsdSlave = NULL;
    PDNMM pdnmmHead = NULL;
    PMMD pmmdThis = NULL;
    PDNMG pdnmgThis = NULL;
    PMGD pmgdThis = NULL;

    pdnmsThis = GetSelectedModelstruc();
    if (pdnmsThis)
        pmsdMaster = (PMSD) pdnmsThis->data.ptrvalue;
    else
        return FALSE;

    pdnmmHead = pmsdMaster->pdnmmHead;
    while (pdnmmHead) {
        pmmdThis = pdnmmHead->data.ptrvalue;
        if (pmmdThis) {
            pdnmgThis = pmmdThis->pdnmgHead;
            while (pdnmgThis) {
                pmgdThis = pdnmgThis->data.ptrvalue;
                if (!(IsGraphAminoAcid(pmgdThis) || IsGraphNABase(pmgdThis)))
                    break; /* only allow features on protein & NA residues for now */
                if (pmgdThis && pmgdThis->bHighlighted == 1) {
                    return TRUE;
                }
                pdnmgThis = pdnmgThis->next;
            }
        }
        pdnmmHead = pdnmmHead->next;
    }

    pdnmsSlave = pmsdMaster->pdnmsSlaves;
    while (pdnmsSlave) {
        pmsdSlave = (PMSD) pdnmsSlave->data.ptrvalue;
        pdnmmHead = pmsdSlave->pdnmmHead;
        while (pdnmmHead) {
            pmmdThis = pdnmmHead->data.ptrvalue;
            if (pmmdThis) {
                pdnmgThis = pmmdThis->pdnmgHead;
                while (pdnmgThis) {
                    pmgdThis = pdnmgThis->data.ptrvalue;
                    if (!(IsGraphAminoAcid(pmgdThis) || IsGraphNABase(pmgdThis)))
                        break;
                    if (pmgdThis && pmgdThis->bHighlighted == 1) {
                        return TRUE;
                    }
                    pdnmgThis = pdnmgThis->next;
                }
            }
            pdnmmHead = pdnmmHead->next;
        }
        pdnmsSlave = pdnmsSlave->next;
    }

    return FALSE;
}

/*---------------------------------------*/
static SpecialFeatureInfoPtr GetThisSpecialAlgorRenderSet(Int2 id)
{
    SpecialFeaturePtr sfpThis = NULL;
    SpecialFeatureInfoPtr sfipThis = NULL;

    sfpThis = sfpGlobal;
    while (sfpThis) {
        if (sfpThis->choice == id) {
            return sfpThis->data.ptrvalue;
        }
        sfpThis = sfpThis->next;
    }

    return NULL;
}

/*---------------------------------------------------------*/
static ValNodePtr CopyLocation(ValNodePtr locOld)
{
    ValNodePtr locNew;
    ChemGraphPntrsPtr pcgpNew, pcgpOld = (ChemGraphPntrsPtr) locOld->data.ptrvalue;
    ValNodePtr rspNew, rspOld = (ValNodePtr) pcgpOld->data.ptrvalue;
    ResidueIntervalPntrPtr rsipNew = NULL, rsipPrev = NULL, rsipNewHead,
        rsipOld = (ResidueIntervalPntrPtr) rspOld->data.ptrvalue;

    while (rsipOld) {
        rsipNew = ResidueIntervalPntrNew();
        if (!rsipNew) return NULL;
        MemCpy(rsipNew, rsipOld, sizeof(ResidueIntervalPntr));
        rsipNew->next = NULL;
        if (rsipPrev) 
            rsipPrev->next = rsipNew;
        else
            rsipNewHead = rsipNew;
        rsipPrev = rsipNew;
        rsipOld = rsipOld->next;
    }
    rspNew = ValNodeNew(NULL);
    if (!rspNew) return NULL;
    MemCpy(rspNew, rspOld, sizeof(ValNode));
    rspNew->data.ptrvalue = (VoidPtr) rsipNewHead;
    pcgpNew = ValNodeNew(NULL);
    if (!pcgpNew) return NULL;
    MemCpy(pcgpNew, pcgpOld, sizeof(ValNode));
    pcgpNew->data.ptrvalue = (VoidPtr) rspNew;
    locNew = ValNodeNew(NULL);
    if (!locNew) return NULL;
    MemCpy(locNew, locOld, sizeof(ValNode));
    locNew->data.ptrvalue = (VoidPtr) pcgpNew;
    return locNew;
}

/*---------------------------------------------------------*/
static void FreeLocations(ValNodePtr pvnLocations)
{
    while (pvnLocations) {
        ChemGraphPntrsPtr pcgpThis = pvnLocations->data.ptrvalue;
        ValNodePtr rsp = pcgpThis->data.ptrvalue;
        ResidueIntervalPntrPtr rsip = rsp->data.ptrvalue;
        ResidueIntervalPntrFree(rsip);
        rsp->data.ptrvalue = NULL;
        ValNodeFree(rsp);
        pcgpThis->data.ptrvalue = NULL;
        ValNodeFree(pcgpThis);
        pvnLocations = pvnLocations->next;
    }
}

/*---------------------------------------------------------*/
static SpecialFeatureInfoPtr SpecialFeatureInfoFree(SpecialFeatureInfoPtr sfipThis)
{
    if (sfipThis->name)
        MemFree(sfipThis->name);
    if (sfipThis->description)
        MemFree(sfipThis->description);
    if (sfipThis->parsSpecial)
        FreeAlgorRenderSet(sfipThis->parsSpecial);
    if (sfipThis->pvnModels)
        ValNodeFree(sfipThis->pvnModels);
    if (sfipThis->pvnLocations)
        FreeLocations(sfipThis->pvnLocations);
    return MemFree(sfipThis);
}

/*---------------------------------------------------------*/
static SpecialFeaturePtr LIBCALL SpecialFeatureFree(SpecialFeaturePtr sfpThis)
{
    SpecialFeatureInfoPtr sfipThis = NULL;

    if (sfpThis == NULL)
        return NULL;
    sfipThis = sfpThis->data.ptrvalue;
    if (sfipThis)
        sfipThis = SpecialFeatureInfoFree(sfipThis);

    sfpThis->data.ptrvalue = NULL;
    sfpThis = ValNodeFree(sfpThis);
    return (sfpThis);
}

/*---------------------------------------------------------*/
static SpecialFeatureInfoPtr LIBCALL SpecialFeatureInfoNew()
{
    return (SpecialFeatureInfoPtr) MemNew(sizeof(SpecialFeatureInfo));
}

/*--------------------------------------------------------*/
static SpecialFeatureInfoPtr GetEditedSpecialFeatureInfo(void)
{
    Int4 iFeature = 0, iCount = 0;
    SpecialFeatureInfoPtr sfipThis = NULL;
    SpecialFeaturePtr sfpThis = NULL;

    iFeature = GetValue(Cn3D_lFeaturesEdited);
    iCount = 0;

    sfpThis = sfpGlobal;
    while (sfpThis) {
        iCount++;
        if (iFeature == iCount) {
            sfipThis = sfpThis->data.ptrvalue;
            iEditedFeature = sfpThis->choice;
            return sfipThis;
        }
        sfpThis = sfpThis->next;
    }
    return NULL;
}

/*---------------------------------------------------------*/
static PARS GetEditedSpecialFeatureInfoPAR(void)
{
    SpecialFeatureInfoPtr sfipThis = NULL;
    PARS parsThis = NULL;

    sfipThis = GetEditedSpecialFeatureInfo();
    if (sfipThis) {
        parsThis = sfipThis->parsSpecial;
        if (parsThis)
            return parsThis;
        else {
            return NULL;
        }
    }
    return NULL;
}

/*---------------------------------------------------------*/
static PARS GetNewSpecialAlgorRenderSet(void)
{
    PARS pars = NULL;

    pars = NewStructureRenderSet();

    pars->PVirtualBBOn = TRUE;
    pars->PPartialBBOn = FALSE;
    pars->PCompleteBBOn = FALSE;

    pars->PBBColor = C_BYCHOICE;
    pars->PResiduesOn = TRUE;
    pars->PResColor = C_BYCHOICE;

    pars->NTVirtualBBOn = TRUE;
    pars->NTPartialBBOn = FALSE;
    pars->NTCompleteBBOn = FALSE;

    pars->NTBBColor = C_BYCHOICE;
    pars->NTResiduesOn = TRUE;
    pars->NTResColor = C_BYCHOICE;

    pars->PBBLabelStyle |= L_WHITE;
    pars->NTBBLabelStyle |= L_WHITE;

    pars->PBBColRGB[0] = pars->PBBColRGB[1] = pars->PBBColRGB[2] =
    pars->PResColRGB[0] = pars->PResColRGB[1] = pars->PResColRGB[2] =
    pars->NTBBColRGB[0] = pars->NTBBColRGB[1] = pars->NTBBColRGB[2] =
    pars->NTResColRGB[0] = pars->NTResColRGB[1] = pars->NTResColRGB[2] = 128;

    return pars;
}

/*---------------------------------------------------------*/
PARS LIBCALL GetSpecialAlgorRenderSet(void)
{
    SpecialFeaturePtr sfpThis = NULL;
    PARS parsThis = NULL;
    if (GetStatus(bFeatureEdit) || GetStatus(bFeatureMove)) {
        return GetEditedSpecialFeatureInfoPAR();
    }
    
    if (parsSpecial == NULL) {
        parsSpecial = GetNewSpecialAlgorRenderSet();
    }
    return parsSpecial;
}

/*---------------------------------------------------------*/
static void FeatureTitleProc(TexT b)
{
    ResetModelCtrls();
}

/*---------------------------------------------------------*/
static void ChangeSpecialRenderProc(PopuP p)
{
    Int2 i = 0, j = 0, k = 0;
    Uint1 r, g, b;
    PARS pars = NULL;
    UIEnum val;

    pars = GetSpecialAlgorRenderSet();
    if (!pars)
        return;

    pars->PResiduesOn = GetStatus(Cn3D_lModelOnOffItem[2]);
    pars->NTResiduesOn = GetStatus(Cn3D_lModelOnOffItem[3]);

    pars->PVirtualBBOn = FALSE;
    pars->PPartialBBOn = FALSE;
    pars->PCompleteBBOn = FALSE;

    i = GetValue(Cn3D_pupModelPBB);
    switch (i) {
    case 1:
        pars->PVirtualBBOn = TRUE; /* alpha c trace */
        break;
    case 2:
        pars->PPartialBBOn = TRUE; /* partial atoms */
        break;
    case 3:
        pars->PCompleteBBOn = TRUE; /* all atoms */
        break;
    default:;                  /* none */
    }

    pars->NTVirtualBBOn = FALSE;
    pars->NTPartialBBOn = FALSE;
    pars->NTCompleteBBOn = FALSE;

    j = GetValue(Cn3D_pupModelNABB);
    switch (j) {
    case 1:
        pars->NTVirtualBBOn = TRUE;
        break;
    case 2:
        pars->NTPartialBBOn = TRUE;
        break;
    case 3:
        pars->NTCompleteBBOn = TRUE;
        break;
    default:;
    }

    for (i = 0; i < 4; i++) {
        if (GetEnumPopup(Cn3D_pupModelColorStyle[i], colorAlist[i], &val)) {
            j = (Int2) val;
        } else {
            j = 1;
        }

        switch (j) {
        case 1:
            k = C_red;
            break;
        case 2:
            k = C_green;
            break;
        case 3:
            k = C_magenta;
            break;
        case 4:
            k = C_sky;
            break;
        case 5:
            k = C_purple;
            break;
        case 6:
        default:
            k = -1;
        }

        if (k != -1 || Cn3D_pupModelColorStyle[i] == p) {

            if (k == -1) {
                Nlm_ChooseColorDialog(&r, &g, &b, FALSE);
            } else {
                r = Cn3d_PaletteRGB[k][0];
                g = Cn3d_PaletteRGB[k][1];
                b = Cn3d_PaletteRGB[k][2];
            }

            switch (i) {
            case 0:                /* prot bb */
                pars->PBBColor = C_BYCHOICE;
                pars->PBBColRGB[0] = r;
                pars->PBBColRGB[1] = g;
                pars->PBBColRGB[2] = b;
                break;
            case 1:                /* prot sc */
                pars->NTBBColor = C_BYCHOICE;
                pars->NTBBColRGB[0] = r;
                pars->NTBBColRGB[1] = g;
                pars->NTBBColRGB[2] = b;
                break;
            case 2:                /* na bb */
                pars->PResColor = C_BYCHOICE;
                pars->PResColRGB[0] = r;
                pars->PResColRGB[1] = g;
                pars->PResColRGB[2] = b;
                break;
            case 3:                /* na sc */
                pars->NTResColor = C_BYCHOICE;
                pars->NTResColRGB[0] = r;
                pars->NTResColRGB[1] = g;
                pars->NTResColRGB[2] = b;
                break;
            }
        }
    }

    for (i = 0; i < 4; i++) {
        if (GetEnumPopup(Cn3D_pupModelRenderStyle[i], renderAlist[i], &val)) {
            j = (Int2) val;
        } else {
            j = 1;
        }
        switch (j) {
        case 1:
            k = R_WIRE;
            break;
        case 2:
            k = R_STICK;
            break;
        case 3:
            k = R_BALLNSTICK;
            break;
        case 4:
            k = R_THICKWIRE;
            break;
        case 5:
            k = R_SPACE;
            break;
        default:
            ;
        }
        switch (i) {
        case 0:                /* prot bb */
            pars->PBBRender = k;
            break;
        case 1:                /* na bb */
            pars->NTBBRender = k;
            break;
        case 2:                /* prot sc */
            pars->PResRender = k;
            break;
        case 3:                /* na sc */
            pars->NTResRender = k;
            break;
        default:
            ;
        }
    }
}

/*---------------------------------------------------------*/
static void ModelPopupOnOffProc(PopuP p)
{
    ChangeSpecialRenderProc(p);
}

/*---------------------------------------------------------*/
static void ModelButtonOnOffProc(ButtoN b)
{
    ChangeSpecialRenderProc(NULL);
}

/*---------------------------------------------------------*/
static PopuP ModelRenderStyle(GrouP h, Int2 i)
{
    PopuP p;

    if (renderAlist[i] != empty_alist) {
        p = PopupList(h, FALSE, ModelPopupOnOffProc);
    } else {
        p = PopupList(h, FALSE, NULL);
    }
    InitEnumPopup(p, renderAlist[i], NULL);
    return p;
}

/*---------------------------------------------------------*/
static PopuP ModelColorStyle(GrouP h, Int2 i)
{
    PopuP p;

    if (colorAlist[i] != empty_alist) {
        p = PopupList(h, FALSE, ModelPopupOnOffProc);
    } else {
        p = PopupList(h, FALSE, NULL);
    }
    InitEnumPopup(p, colorAlist[i], NULL);
    return p;

}

/*---------------------------------------------------------*/
static void ChangeSpecialLabelsProc(void)
{
    Int2 i = 0, k = 0, nameval = 0;
    Uint1 codeval;
    Boolean NumOn = FALSE, NameOn = FALSE, PDBOn = FALSE, WhiteOn =
        FALSE, TopOn = FALSE;
    PARS pars = NULL;

    pars = GetSpecialAlgorRenderSet();
    if (!pars)
        return;

    pars->PBBLabelInterval = (Uint1) GetValue(Cn3D_pupLabelAA);
    pars->NTBBLabelInterval = (Uint1) GetValue(Cn3D_pupLabelNT);

    for (i = 0; i < 2; i++) {

        nameval = GetValue(Cn3D_bLName[i]);
        switch (nameval) {
        case 2:
            codeval = (Uint1) L_3LETR;
            break;
        case 3:
            codeval = (Uint1) L_1LETR;
            break;
        case 4:
            codeval = (Uint1) L_PDB;
            break;
        case 1: default:
            codeval = 0;
        }
        switch (i) {
        case 0:                /* aa */
            /* clear bits */
            pars->PBBLabelStyle &= ~((Uint1) (L_3LETR | L_1LETR | L_PDB));
            /* set bit */
            pars->PBBLabelStyle |= codeval;
            break;
        case 1:                /* na   */
            pars->NTBBLabelStyle &= ~((Uint1) (L_3LETR | L_1LETR | L_PDB));
            pars->NTBBLabelStyle |= codeval;
            break;
        default: ;
        }

        NameOn = (Boolean) (nameval > 1);
        switch (i) {
        case 0:                /* aa */
            if (NameOn)
                pars->PBBLabelStyle |= (Uint1) L_NAME;
            else
                pars->PBBLabelStyle &= (Uint1) ~L_NAME;
            break;
        case 1:                /* na   */
            if (NameOn)
                pars->NTBBLabelStyle |= (Uint1) L_NAME;
            else
                pars->NTBBLabelStyle &= (Uint1) ~L_NAME;
            break;
        default: ;
        }

        NumOn = GetStatus(Cn3D_bLNum[i]);
        switch (i) {
        case 0:                /* aa */
            if (NumOn)
                pars->PBBLabelStyle |= (Uint1) L_NUM;
            else
                pars->PBBLabelStyle &= (Uint1) ~L_NUM;
            break;
        case 1:                /* na   */
            if (NumOn)
                pars->NTBBLabelStyle |= (Uint1) L_NUM;
            else
                pars->NTBBLabelStyle &= (Uint1) ~L_NUM;
            break;
        default: ;
        }

        WhiteOn = GetStatus(Cn3D_bLWhite[i]);
        switch (i) {
        case 0:                /* aa */
            if (WhiteOn)
                pars->PBBLabelStyle |= (Uint1) L_WHITE;
            else
                pars->PBBLabelStyle &= (Uint1) ~L_WHITE;
            break;
        case 1:                /* na   */
            if (WhiteOn)
                pars->NTBBLabelStyle |= (Uint1) L_WHITE;
            else
                pars->NTBBLabelStyle &= (Uint1) ~L_WHITE;
            break;
        default: ;
        }

#ifndef _OPENGL
        TopOn = GetStatus(Cn3D_bLTop[i]);
        switch (i) {
        case 0:                /* aa */
            if (TopOn)
                pars->PBBLabelJust = pars->PBBLabelJust | (Uint1) LA_FRONT;
            else
                pars->PBBLabelJust =
                    pars->PBBLabelJust & (Uint1) ~ LA_FRONT;
            break;
        case 1:                /* na   */
            if (TopOn)
                pars->NTBBLabelJust =
                    pars->NTBBLabelJust | (Uint1) LA_FRONT;
            else
                pars->NTBBLabelJust =
                    pars->NTBBLabelJust & (Uint1) ~ LA_FRONT;
            break;
        default:
            ;
        }
        k = (Int2) GetValue(Cn3D_pupLabelSize[i]);
        switch (i) {
        case 0:                /* prot bb */
            pars->PBBLabelScale = k;
            break;
        case 1:                /* na bb */
            pars->NTBBLabelScale = k;
            break;
        default:
            ;
        }
#endif
    }
}

/*---------------------------------------------------------*/
static void ModelLabelPopupOnOffProc(PopuP p)
{
    ChangeSpecialLabelsProc();
}

/*---------------------------------------------------------*/
static void ModelLabelButtonOnOffProc(ButtoN b)
{
    ChangeSpecialLabelsProc();
}

/*---------------------------------------------------------*/
static void ModelSetRenderCtrls(PARS pars)
{
    Int2 i;

    if (!pars)
        return;

    i = 4;
    if (pars->PVirtualBBOn)
        i = 1;
    else if (pars->PPartialBBOn)
        i = 2;
    else if (pars->PCompleteBBOn)
        i = 3;
    SafeSetValue(Cn3D_pupModelPBB, i);

    i = 4;
    if (pars->NTVirtualBBOn)
        i = 1;
    else if (pars->NTPartialBBOn)
        i = 2;
    else if (pars->NTCompleteBBOn)
        i = 3;
    SafeSetValue(Cn3D_pupModelNABB, i);

    SafeSetStatus(Cn3D_lModelOnOffItem[2], pars->PResiduesOn);
    SafeSetStatus(Cn3D_lModelOnOffItem[3], pars->NTResiduesOn);

    i = GetRenderItemIndex(pars->PBBRender);
    SafeSetValue(Cn3D_pupModelRenderStyle[0], i);
    i = GetColorItemIndex(pars->PBBColRGB);
    SafeSetValue(Cn3D_pupModelColorStyle[0], i);

    i = GetRenderItemIndex(pars->NTBBRender);
    SafeSetValue(Cn3D_pupModelRenderStyle[1], i);
    i = GetColorItemIndex(pars->NTBBColRGB);
    SafeSetValue(Cn3D_pupModelColorStyle[1], i);

    i = GetRenderItemIndex(pars->PResRender);
    SafeSetValue(Cn3D_pupModelRenderStyle[2], i);
    i = GetColorItemIndex(pars->PResColRGB);
    SafeSetValue(Cn3D_pupModelColorStyle[2], i);

    i = GetRenderItemIndex(pars->NTResRender);
    SafeSetValue(Cn3D_pupModelRenderStyle[3], i);
    i = GetColorItemIndex(pars->NTResColRGB);
    SafeSetValue(Cn3D_pupModelColorStyle[3], i);
}

/*---------------------------------------------------------*/
static void ModelSetLabelCtrls(PARS pars)
{
    Int2 i = 0, val = 0, style = 0, size = 0;

    if (!pars)
        return;

    SafeSetValue(Cn3D_pupLabelAA, pars->PBBLabelInterval);
    SafeSetValue(Cn3D_pupLabelNT, pars->NTBBLabelInterval);

    SafeSetStatus(Cn3D_lOnOffLabel[2], pars->PTermLabelOn);
    SafeSetStatus(Cn3D_lOnOffLabel[3], pars->NTTermLabelOn);

    for (i = 0; i < 2; i++) {
        if (i == 0) {
            style = pars->PBBLabelStyle;
            size = pars->PBBLabelScale;
        } else {
            style = pars->NTBBLabelStyle;
            size = pars->NTBBLabelScale;
        }
        val = 1;
        if (style & L_NAME) {
            if (style & L_3LETR) {
                val = 2;
            } else if (style & L_1LETR) {
                val = 3;
            } else if (style & L_PDB) {
                val = 4;
            }
        }
        SafeSetValue(Cn3D_bLName[i], val);
        SafeSetStatus(Cn3D_bLNum[i], (Boolean) (style & L_NUM));
        SafeSetStatus(Cn3D_bLWhite[i], (Boolean) (style & L_WHITE));
#ifndef _OPENGL
        SafeSetStatus(Cn3D_bLTop[i], (Boolean) (style & (Uint1) LA_FRONT));
        SafeSetValue(Cn3D_pupLabelSize[i], size);
#endif
    }
}

/*---------------------------------------------------------*/
static void ModelResetLabelCtrls(void)
{
    PARS pars = NULL;

    pars = GetSpecialAlgorRenderSet();
    if (!pars)
        return;

    ModelSetLabelCtrls(pars);
}

/*---------------------------------------------------------*/
static void bFeatureAddAction(ButtoN b)
{
    if (GetStatus(bFeatureAdd)) {
        SetStatus(bFeatureEdit, FALSE);
        SetStatus(bFeatureDelete, FALSE);
        SetStatus(bFeatureMove, FALSE);
        SafeSetValue(Cn3D_lFeaturesEdited, 0);
    }
}

/*---------------------------------------------------------*/
static void bFeatureDeleteAction(ButtoN b)
{
    if (GetStatus(bFeatureDelete)) {
        SetStatus(bFeatureEdit, FALSE);
        SetStatus(bFeatureAdd, FALSE);
        SetStatus(bFeatureMove, FALSE);
    }
}

/*---------------------------------------------------------*/
static void lFeatureEditedAction(LisT l)
{
    SpecialFeatureInfoPtr sfip = NULL;
    PARS parsThis = NULL;
    Int4 val = 0;

    if (GetStatus(bFeatureEdit) || GetStatus(bFeatureDelete) || GetStatus(bFeatureMove)) {
        sfip = GetEditedSpecialFeatureInfo();
        if (sfip) {
            SetTitle(FeatureTitle, sfip->name);
            SetTitle(FeatureDescription, sfip->description);
        }
        parsThis = GetEditedSpecialFeatureInfoPAR();
        ModelSetRenderCtrls(parsThis);
        ModelSetLabelCtrls(parsThis);
    }
}

/*---------------------------------------------------------*/
static void bFeatureMoveAction(ButtoN b)
{
    SpecialFeaturePtr sfpThis = NULL;
    SpecialFeatureInfoPtr sfip = NULL;
    PARS parsThis = NULL;

    if (GetStatus(bFeatureMove)) {
        SetStatus(bFeatureAdd, FALSE);
        SetStatus(bFeatureDelete, FALSE);
        SetStatus(bFeatureEdit, FALSE);

        sfip = GetEditedSpecialFeatureInfo();
        if (sfip) {
            SetTitle(FeatureTitle, sfip->name);
            SetTitle(FeatureDescription, sfip->description);
        }
        parsThis = GetEditedSpecialFeatureInfoPAR();
        ModelSetRenderCtrls(parsThis);
        ModelSetLabelCtrls(parsThis);
    }
}

/*---------------------------------------------------------*/
static void bFeatureEditAction(ButtoN b)
{
    SpecialFeaturePtr sfpThis = NULL;
    SpecialFeatureInfoPtr sfip = NULL;
    PARS parsThis = NULL;

    if (GetStatus(bFeatureEdit)) {
        SetStatus(bFeatureAdd, FALSE);
        SetStatus(bFeatureDelete, FALSE);
        SetStatus(bFeatureMove, FALSE);

        sfip = GetEditedSpecialFeatureInfo();
        if (sfip) {
            SetTitle(FeatureTitle, sfip->name);
            SetTitle(FeatureDescription, sfip->description);
        }
        parsThis = GetEditedSpecialFeatureInfoPAR();
        ModelSetRenderCtrls(parsThis);
        ModelSetLabelCtrls(parsThis);
    }
}

/*---------------------------------------------------------*/
 static ValNodePtr GetFeatureLocationForModel(SpecialFeatureInfoPtr sfip, PMSD pmsd)
 {
    ValNodePtr pvnLoc, pvnModel;

    for (pvnModel = sfip->pvnModels, pvnLoc = sfip->pvnLocations;
         pvnModel && pvnLoc;
         pvnModel = pvnModel->next, pvnLoc = pvnLoc->next) {
        if (pvnModel->data.ptrvalue == (VoidPtr) pmsd)
            return (ValNodePtr) pvnLoc->data.ptrvalue;
    }
    return NULL;
 }

/*---------------------------------------------------------*/
static void Cn3D_FeatureApplyProc(ButtoN b)
{
    SpecialFeatureInfoPtr sfip = NULL;
    SpecialFeaturePtr sfpThis = NULL;
    PARS pars = NULL;
    PDNMS pdnmsThis, pdnmsSlave;
    PMSD pmsdThis;
    ValNodePtr pvnLoc;
#ifdef _DEBUG
    int iCount = 0;
#endif

    /* delete a feature */
    if (GetStatus(bFeatureDelete)) {
        SetStatus(bFeatureDelete, FALSE);
        if (sfpGlobal == NULL) return;
        sfip = GetEditedSpecialFeatureInfo();
        if (sfip == NULL) return;
        if (sfpGlobal->next == NULL) {
            sfpGlobal = SpecialFeatureFree(sfpGlobal);
        } else {
            sfpThis = ValNodeExtract(&sfpGlobal, iEditedFeature);
            sfpThis = SpecialFeatureFree(sfpThis);
        }
        RemoveFeatureFromAllMGDs(iEditedFeature);
        CollapseFeatureIDs(iEditedFeature);
        UpdateEditFeatureList();
        Cn3D_SyncMenuHighlightWithOnOff();
        iNumFeatures--;
        fnCn3D_RedrawWrapper(FALSE);
        return;
    }

    /* if add more move but nothing selected, return */
    if (GetStatus(bFeatureAdd) || GetStatus(bFeatureMove)) {
        if (!Cn3D_CheckHLStatus()) {
            SetStatus(bFeatureAdd, FALSE);
            SetStatus(bFeatureMove, FALSE);
            return;
        }
    }

    pars = GetSpecialAlgorRenderSet();
    if (!pars) {
        SetStatus(bFeatureEdit, FALSE);
        SetStatus(bFeatureAdd, FALSE);
        SetStatus(bFeatureMove, FALSE);
        return;
    }
    ChangeSpecialRenderProc(NULL);
    ChangeSpecialLabelsProc();
    pdnmsThis = GetSelectedModelstruc();
    pmsdThis = (PMSD) pdnmsThis->data.ptrvalue;

    if (GetStatus(bFeatureAdd) || GetStatus(bFeatureEdit) || GetStatus(bFeatureMove)) {
        Char str[1024];

        if (GetStatus(bFeatureEdit)) {
            sfip = GetEditedSpecialFeatureInfo();
        }

        else if (GetStatus(bFeatureMove)) {
            sfip = GetEditedSpecialFeatureInfo();
            RemoveFeatureFromAllMGDs(iEditedFeature);
            FreeLocations(sfip->pvnLocations);
            ValNodeFree(sfip->pvnModels);
        }
        
        else { /* add */
            sfip = SpecialFeatureInfoNew();
            sfip->parsSpecial = pars;
            iNumFeatures++;
            ValNodeAddPointer(&sfpGlobal, iNumFeatures, (VoidPtr) sfip);
            iEditedFeature= iNumFeatures;
        }

        sfip->isOn = TRUE;

        if (GetStatus(bFeatureAdd) || GetStatus(bFeatureMove)) {
            sfip->pvnLocations = sfip->pvnModels = NULL;
            pvnLoc = GetBiostrucFeatureLocationForHihglightedResidues(pdnmsThis, iEditedFeature);
            if (pvnLoc) {
                ValNodeAddPointer(&(sfip->pvnLocations), 0, (VoidPtr) pvnLoc);
                ValNodeAddPointer(&(sfip->pvnModels), 0, (VoidPtr) pmsdThis);
            }
            for (pdnmsSlave = pmsdThis->pdnmsSlaves; pdnmsSlave; pdnmsSlave = pdnmsSlave->next) {
                pvnLoc = GetBiostrucFeatureLocationForHihglightedResidues(pdnmsSlave, iEditedFeature);
                if (pvnLoc) {
                    ValNodeAddPointer(&(sfip->pvnLocations), 0, (VoidPtr) pvnLoc);
                    ValNodeAddPointer(&(sfip->pvnModels), 0, pdnmsSlave->data.ptrvalue);
                }
            }
            ObjMgrDeSelectAll();
        }

        GetTitle(FeatureTitle, str, sizeof(str));
        sfip->name = StringSave(str);
        GetTitle(FeatureDescription, str, sizeof(str));
        sfip->description = StringSave(str);

        SetStatus(bFeatureAdd, FALSE);
        SetStatus(bFeatureEdit, FALSE);
        SetStatus(bFeatureMove, FALSE);

        parsSpecial = NULL;
        SetTitle(FeatureTitle, "name?");
        SetTitle(FeatureDescription, "");
    }

    UpdateEditFeatureList();
    SafeSetValue(Cn3D_lFeaturesEdited, iEditedFeature);
    ResetModelCtrls();

    /* set show/hide of each feature depending on selection status */
    Cn3D_SyncMenuHighlightWithOnOff();
    sfpThis = sfpGlobal;
    while (sfpThis) {
#ifdef _DEBUG
        /* to verify that sfp's are always numbered consecutively (from 1) - many
           functions depend on this (and many can change it) */
        iCount++;
        if (iCount != sfpThis->choice)
            Message(MSG_POST, "internal error: discontinuity in sfp id: #%i has choice %i",
                iCount, sfpThis->choice);
#endif
        sfip = sfpThis->data.ptrvalue;
        pvnLoc = GetFeatureLocationForModel(sfip, pmsdThis);
        if (pvnLoc)
            Cn3DApplyUserDefinedFeatureToMGD(pmsdThis, pvnLoc, sfpThis->choice, sfip);
        pdnmsSlave = pmsdThis->pdnmsSlaves;
        while (pdnmsSlave) {
            PMSD pmsdSlave = (PMSD) pdnmsSlave->data.ptrvalue;
            pvnLoc = GetFeatureLocationForModel(sfip, pmsdSlave);
            if (pvnLoc)
                Cn3DApplyUserDefinedFeatureToMGD(pmsdSlave, pvnLoc, sfpThis->choice, sfip);
            pdnmsSlave = pdnmsSlave->next;
        }
        sfpThis = sfpThis->next;
    }

    fnCn3D_RedrawWrapper(FALSE);
}


/*---------------------------------------------------------*/
GrouP LIBCALL ModelControls(Nlm_GrouP prnt)
{
    GrouP g, h1, h2, h3, h4;
    GrouP h5, h6, h7;
    Int2 k = 0;
    PrompT ppt[10], ppt0[10];
    PrompT ppt1, ppt2, ppt3, ppt4, ppt5, ppt6, ppt7, ppt8, ppt9, ppt10,
        ppt11;
    PrompT ppt12, ppt13, ppt14;
    RecT pptPos, btnPos;
    Int2 delta = 0;

    g = HiddenGroup(prnt, -1, 0, NULL);
    if (!g)
        return NULL;
    SetGroupSpacing(g, 3, 9);
#ifdef WIN_MOTIF
    SetGroupMargins(g, 4, 1);
    SetGroupSpacing(g, 2, 1);
#else
    SetGroupMargins(g, 1, 1);
    SetGroupSpacing(g, 0, 0);
#endif

    h5 = HiddenGroup(g, 0, -3, NULL);
    SetGroupSpacing(h5, 20, 3);
    ppt12 =
        StaticPrompt(h5,
                     "Define Annotations: Use Mouse to Select Residues First",
                     0, stdLineHeight + 5, systemFont, 'c');
    h2 = HiddenGroup(h5, 0, -1, NULL);
    SetGroupSpacing(h2, 5, 3);
    StaticPrompt(h2, "    Name             ", 0, stdLineHeight + 5,
                 systemFont, 'c');
    FeatureTitle = DialogText(h2, "", 10, FeatureTitleProc);
    SetTitle(FeatureTitle, "name?");

    h3 = HiddenGroup(h5, 0, -1, NULL);
    SetGroupSpacing(h3, 5, 3);
    StaticPrompt(h3, "    Description ", 0, stdLineHeight + 5, systemFont,
                 'c');
    FeatureDescription = DialogText(h3, "", 20, NULL);
    AlignObjects(ALIGN_LEFT, (HANDLE) ppt12, (HANDLE) h2, (HANDLE) h3,
                 NULL);
    AlignObjects(ALIGN_LEFT, (HANDLE) h2, (HANDLE) h3, NULL);

/*StaticPrompt(g, "         ",0, stdLineHeight + 5, systemFont, 'l'); */
    h1 = HiddenGroup(g, 0, -5, NULL);
    SetGroupSpacing(h1, 5, 3);

    Cn3D_lModelOnOffItem[0] = NULL; /* Cn3D_pupModelPBB includes off setting */
    Cn3D_lModelOnOffItem[1] = NULL; /* Cn3D_pupModelNABB includes off setting */

    ppt1 =
        StaticPrompt(h1, "    Set Style", 0, stdLineHeight + 5,
                     systemFont, 'c');
    ppt[0] =
        StaticPrompt(h1, "  Protein Backbone", 0, popupMenuHeight,
                     systemFont, 'l');
    ppt[1] =
        StaticPrompt(h1, "  Nucleotide Backbone", 0, popupMenuHeight,
                     systemFont, 'l');
    ppt[2] =
        StaticPrompt(h1, "  Protein Sidechains", 0, popupMenuHeight,
                     systemFont, 'l');
    ppt[3] =
        StaticPrompt(h1, "  Nucleotide Sidechains", 0, popupMenuHeight,
                     systemFont, 'l');
/*AlignObjects (ALIGN_CENTER, (HANDLE) ppt1, (HANDLE) ppt [1], NULL); */

    ppt2 =
        StaticPrompt(h1, "On/Off", 0, stdLineHeight + 5, systemFont, 'c');
    Cn3D_pupModelPBB = PopupList(h1, FALSE, ModelPopupOnOffProc);
    PopupItem(Cn3D_pupModelPBB, "alpha C trace");
    PopupItem(Cn3D_pupModelPBB, "partial atom");
    PopupItem(Cn3D_pupModelPBB, "all atoms");
    PopupItem(Cn3D_pupModelPBB, "none");
    SafeSetValue(Cn3D_pupModelPBB, 1);
    Cn3D_pupModelNABB = PopupList(h1, FALSE, ModelPopupOnOffProc);
    PopupItem(Cn3D_pupModelNABB, "P trace");
    PopupItem(Cn3D_pupModelNABB, "partial atom");
    PopupItem(Cn3D_pupModelNABB, "all atoms");
    PopupItem(Cn3D_pupModelNABB, "none");
    SafeSetValue(Cn3D_pupModelNABB, 3);

    for (k = 2; k < 4; k++) {
        Cn3D_lModelOnOffItem[k] = CheckBox(h1, "", ModelButtonOnOffProc);
    }
    SafeSetStatus(Cn3D_lModelOnOffItem[3], TRUE);
    for (k = 2; k < 4; k++) {
        GetPosition(Cn3D_lModelOnOffItem[k], &btnPos);
        GetPosition(ppt[k], &pptPos);
        delta =
            (pptPos.bottom + pptPos.top) / 2 - (btnPos.bottom +
                                                btnPos.top) / 2;
        if (delta != 0) {
            OffsetRect(&btnPos, 0, delta);
            SetPosition(Cn3D_lModelOnOffItem[k], &btnPos);
            AdjustPrnt((Nlm_GraphiC) Cn3D_lModelOnOffItem[k], &btnPos,
                       FALSE);
        }
    }
    AlignObjects(ALIGN_CENTER, (HANDLE) ppt2, (HANDLE) Cn3D_pupModelPBB,
                 (HANDLE) Cn3D_lModelOnOffItem[2],
                 (HANDLE) Cn3D_lModelOnOffItem[3], NULL);

    ppt3 =
        StaticPrompt(h1, "Drawing Style", 0, stdLineHeight + 5, systemFont, 'c');
    for (k = 0; k < 4; k++) {
        Cn3D_pupModelRenderStyle[k] = ModelRenderStyle(h1, k);
    }
    AlignObjects(ALIGN_CENTER, (HANDLE) ppt3,
                 (HANDLE) Cn3D_pupModelRenderStyle[0], NULL);

    ppt4 =
        StaticPrompt(h1, "Color", 0, stdLineHeight + 5, systemFont, 'c');
    for (k = 0; k < 4; k++) {
        Cn3D_pupModelColorStyle[k] = ModelColorStyle(h1, k);
    }
    AlignObjects(ALIGN_CENTER, (HANDLE) ppt4,
                 (HANDLE) Cn3D_pupModelColorStyle[0], NULL);

/*StaticPrompt(g, "",0,0,Nlm_systemFont,'l'); */

    h4 = HiddenGroup(g, 0, -3, NULL);
    SetGroupSpacing(h4, 5, 3);

    ppt5 =
        StaticPrompt(h4, "    Set Label", 0, stdLineHeight + 5, systemFont,
                     'c');
    ppt0[0] =
        StaticPrompt(h4, "  Amino Acid", 0, popupMenuHeight, systemFont,
                     'l');
    ppt0[1] =
        StaticPrompt(h4, "  Nucleic Acid", 0, popupMenuHeight, systemFont,
                     'l');
/*AlignObjects (ALIGN_CENTER, (HANDLE) ppt5, (HANDLE) ppt0 [1], NULL);  */

    ppt6 =
        StaticPrompt(h4, "On/Off", 0, stdLineHeight + 5, systemFont, 'c');
    Cn3D_pupLabelAA = PopupList(h4, FALSE, ModelLabelPopupOnOffProc);
    PopupItem(Cn3D_pupLabelAA, "none");
    PopupItem(Cn3D_pupLabelAA, "every AA");
    PopupItem(Cn3D_pupLabelAA, "every 5");
    PopupItem(Cn3D_pupLabelAA, "every 10");
    PopupItem(Cn3D_pupLabelAA, "every 20");
    PopupItem(Cn3D_pupLabelAA, "every 50");
    Cn3D_pupLabelNT = PopupList(h4, FALSE, ModelLabelPopupOnOffProc);
    PopupItem(Cn3D_pupLabelNT, "none");
    PopupItem(Cn3D_pupLabelNT, "every NA");
    PopupItem(Cn3D_pupLabelNT, "every 5");
    PopupItem(Cn3D_pupLabelNT, "every 10");
    PopupItem(Cn3D_pupLabelNT, "every 20");
    PopupItem(Cn3D_pupLabelNT, "every 50");
    AlignObjects(ALIGN_CENTER, (HANDLE) ppt6, (HANDLE) Cn3D_pupLabelAA,
                 (HANDLE) Cn3D_pupLabelNT, NULL);

    ppt7 = StaticPrompt(h4, "Name", 0, stdLineHeight + 5, systemFont, 'c');
    for (k = 0; k < 2; k++) {
        Cn3D_bLName[k] = PopupList(h4, FALSE, ModelLabelPopupOnOffProc);
        PopupItem(Cn3D_bLName[k], "none");
        PopupItem(Cn3D_bLName[k], "3 letter");
        PopupItem(Cn3D_bLName[k], "1 letter");
        PopupItem(Cn3D_bLName[k], "PDB");
    }
    AlignObjects(ALIGN_CENTER, (HANDLE) ppt7, (HANDLE) Cn3D_bLName[0],
                 (HANDLE) Cn3D_bLName[1], NULL);

    ppt8 = StaticPrompt(h4, "Num", 0, stdLineHeight + 5, systemFont, 'c');
    for (k = 0; k < 2; k++) {
        Cn3D_bLNum[k] = CheckBox(h4, "", ModelLabelButtonOnOffProc);
    }
    for (k = 0; k < 2; k++) {
        GetPosition(Cn3D_bLNum[k], &btnPos);
        GetPosition(ppt0[k], &pptPos);
        delta =
            (pptPos.bottom + pptPos.top) / 2 - (btnPos.bottom +
                                                btnPos.top) / 2;
        if (delta != 0) {
            OffsetRect(&btnPos, 0, delta);
            SetPosition(Cn3D_bLNum[k], &btnPos);
            AdjustPrnt((Nlm_GraphiC) Cn3D_bLNum[k], &btnPos, FALSE);
        }
    }
    AlignObjects(ALIGN_CENTER, (HANDLE) ppt8, (HANDLE) Cn3D_bLNum[0],
                 (HANDLE) Cn3D_bLNum[1], NULL);

#ifndef _OPENGL
    ppt9 =
        StaticPrompt(h4, "On Top", 0, stdLineHeight + 5, systemFont, 'c');
    for (k = 0; k < 2; k++) {
        Cn3D_bLTop[k] = CheckBox(h4, "", ModelLabelButtonOnOffProc);
    }
    for (k = 0; k < 2; k++) {
        GetPosition(Cn3D_bLTop[k], &btnPos);
        GetPosition(ppt0[k], &pptPos);
        delta =
            (pptPos.bottom + pptPos.top) / 2 - (btnPos.bottom +
                                                btnPos.top) / 2;
        if (delta != 0) {
            OffsetRect(&btnPos, 0, delta);
            SetPosition(Cn3D_bLTop[k], &btnPos);
            AdjustPrnt((Nlm_GraphiC) Cn3D_bLTop[k], &btnPos, FALSE);
        }
    }
    AlignObjects(ALIGN_CENTER, (HANDLE) ppt9, (HANDLE) Cn3D_bLTop[0],
                 (HANDLE) Cn3D_bLTop[1], NULL);
#endif

    ppt10 =
        StaticPrompt(h4, "White", 0, stdLineHeight + 5, systemFont, 'c');
    for (k = 0; k < 2; k++) {
        Cn3D_bLWhite[k] = CheckBox(h4, "", ModelLabelButtonOnOffProc);
    }
    for (k = 0; k < 2; k++) {
        GetPosition(Cn3D_bLWhite[k], &btnPos);
        GetPosition(ppt0[k], &pptPos);
        delta =
            (pptPos.bottom + pptPos.top) / 2 - (btnPos.bottom +
                                                btnPos.top) / 2;
        if (delta != 0) {
            OffsetRect(&btnPos, 0, delta);
            SetPosition(Cn3D_bLWhite[k], &btnPos);
            AdjustPrnt((Nlm_GraphiC) Cn3D_bLWhite[k], &btnPos, FALSE);
        }
    }
    AlignObjects(ALIGN_CENTER, (HANDLE) ppt10, (HANDLE) Cn3D_bLWhite[0],
                 (HANDLE) Cn3D_bLWhite[1], NULL);

#ifndef _OPENGL
    ppt11 =
        StaticPrompt(h4, "Size", 0, stdLineHeight + 5, systemFont, 'c');
    for (k = 0; k < 2; k++) {
        Cn3D_pupLabelSize[k] =
            PopupList(h4, FALSE, ModelLabelPopupOnOffProc);
        PopupItem(Cn3D_pupLabelSize[k], "1");
        PopupItem(Cn3D_pupLabelSize[k], "2");
        PopupItem(Cn3D_pupLabelSize[k], "3");
        PopupItem(Cn3D_pupLabelSize[k], "4");
        PopupItem(Cn3D_pupLabelSize[k], "5");
        PopupItem(Cn3D_pupLabelSize[k], "6");
        PopupItem(Cn3D_pupLabelSize[k], "7");
        PopupItem(Cn3D_pupLabelSize[k], "8");
        PopupItem(Cn3D_pupLabelSize[k], "9");
        PopupItem(Cn3D_pupLabelSize[k], "10");
    }
    AlignObjects(ALIGN_CENTER, (HANDLE) ppt11,
                 (HANDLE) Cn3D_pupLabelSize[0],
                 (HANDLE) Cn3D_pupLabelSize[1], NULL);
#endif /* ndef _OPENGL */

    StaticPrompt(g, "", 0, 0, Nlm_systemFont, 'l');

    h7 = HiddenGroup(g, 0, -2, NULL);
    ppt13 =
        StaticPrompt(h7, " Edit Annotation:", 0, stdLineHeight + 5,
                     systemFont, 'c');

    h6 = HiddenGroup(h7, 0, -2, NULL);
    StaticPrompt(h6, "   ", 0, 0, Nlm_systemFont, 'l');
    StaticPrompt(h6, "   ", 0, 0, Nlm_systemFont, 'l');

    Cn3D_lFeaturesEdited = SingleList(h6, 5, 3, lFeatureEditedAction);
    StaticPrompt(h6, "", 0, 0, Nlm_systemFont, 'l');

    bFeatureAdd = CheckBox(h6, "Add", bFeatureAddAction);
    bFeatureDelete = CheckBox(h6, "Delete", bFeatureDeleteAction);
    bFeatureEdit = CheckBox(h6, "Edit", bFeatureEditAction);
    bFeatureMove = CheckBox(h6, "Move", bFeatureMoveAction);

    ppt14 =
        StaticPrompt(h7, " Display Annotation:", 0, stdLineHeight + 5,
                     systemFont, 'c');
    Cn3D_lFeaturesOnOff = MultiList(h7, 5, 3, Cn3D_FeatureOnOffProc);
    AlignObjects(ALIGN_CENTER, (HANDLE) g, (HANDLE) ppt14,
                 (HANDLE) Cn3D_lFeaturesOnOff, NULL);

    Cn3D_bModelApply = PushButton(g, "Apply!", Cn3D_FeatureApplyProc);

    AlignObjects(ALIGN_CENTER, (HANDLE) Cn3D_bModelApply, (HANDLE) h1,
                 NULL);

    ModelResetLabelCtrls();
    ResetModelCtrls();
    return g;
}

/*---------------------------------------------------------*/
void LIBCALL ResetModelCtrls(void)
{
    /* this function may not be neccessary */
    Int2 k = 0;
    PARS pars = NULL;;

    pars = GetSpecialAlgorRenderSet();
    if (!pars) {
        SafeSetValue(Cn3D_pupModelPBB, 1);
        SafeSetValue(Cn3D_pupModelNABB, 1);

        for (k = 2; k < 4; k++) {
            SetItemStatus(Cn3D_lModelOnOffItem[k], 1, FALSE);
        }

        SafeSetValue(Cn3D_pupModelPBB, 4);
        SafeSetValue(Cn3D_pupModelNABB, 4);

        for (k = 0; k < 4; k++) {
            SafeSetValue(Cn3D_pupModelRenderStyle[k], 1);
            SafeSetValue(Cn3D_pupModelColorStyle[k], 1);
        }
    }
    return;
}

/*-------------------------------------------*/
static BiostrucFeatureSetDescrPtr MakeDescrForUserDefinedFeature(SpecialFeaturePtr
                                                         sfpThis)
{
    ValNodePtr descr = NULL, descr_head = NULL, descr_tail = NULL;
    SpecialFeatureInfoPtr sfipThis = NULL;

    if (!sfpThis)
        return NULL;
    sfipThis = sfpThis->data.ptrvalue;
    if (!sfipThis)
        return NULL;

    descr = ValNodeNew(NULL);
    descr->choice = 1;
    descr->data.ptrvalue = StringSave("User Defined Features");

    descr_head = descr;
    descr_tail = descr;

    if (sfipThis->description) {
        descr = ValNodeNew(NULL);
        descr->choice = 3;
        descr->data.ptrvalue = StringSave(sfipThis->description);
        descr_tail->next = descr;
        descr_tail = descr;
    }

    if (sfipThis->isOn) {
        descr = ValNodeNew(NULL);
        descr->choice = 3;
        descr->data.ptrvalue = StringSave("On");
        descr_tail->next = descr;
        descr_tail = descr;
    }

    return descr_head;
}

/*-------------------------------------------*/
static ValNodePtr GetBiostrucFeatureLocationForHihglightedResidues(PDNMS pdnmsThis,
                                                           Int2 iFeature)
{

    PMSD pmsdThis = NULL;
    PDNMM pdnmmHead = NULL;
    PMMD pmmdThis = NULL;
    PDNMG pdnmgThis = NULL;
    PMGD pmgdThis = NULL;

    ResidueIntervalPntrPtr rsip = NULL, rsip_head = NULL, rsip_tail = NULL;
    ValNodePtr rsp = NULL;      /* ResiduePntrsPtr */
    ChemGraphPntrsPtr pcgpThis = NULL;
    ValNodePtr llp = NULL;

    Int2 iMol = 0, iMolCurrent = 0;
    Int4 from = 0, to = 0;

    Boolean Started = FALSE, SingleInterval, UserDefinedFeatureFound =
        FALSE;
    Boolean UserDefinedFeatureFoundInThisMolecule = FALSE;

    pmsdThis = pdnmsThis->data.ptrvalue;
    pdnmmHead = pmsdThis->pdnmmHead;
    iMol = 0;
    while (pdnmmHead) {
        pmmdThis = pdnmmHead->data.ptrvalue;
        if (pmmdThis->bWhat != (Byte) AM_PROT
            && pmmdThis->bWhat != (Byte) AM_RNA
            && pmmdThis->bWhat != (Byte) AM_DNA)
            goto setout1;
        Started = FALSE;
        UserDefinedFeatureFoundInThisMolecule = FALSE;
        SingleInterval = TRUE;

        for (pdnmgThis = pmmdThis->pdnmgHead; pdnmgThis != NULL;
             pdnmgThis = pdnmgThis->next) {
            pmgdThis = (PMGD) pdnmgThis->data.ptrvalue;
            if (pmgdThis->bHighlighted) {
                UserDefinedFeatureFound = TRUE;
                iMolCurrent = iMol + 1;
                if (!Started) {
                    from = pdnmgThis->choice;
                    to = pdnmgThis->choice;
                    Started = TRUE;
                    UserDefinedFeatureFoundInThisMolecule = TRUE;
                } else {
                    if ((pdnmgThis->choice - to) == 1) {
                        to = pdnmgThis->choice;
                    } else {
                        SingleInterval = FALSE;
                        rsip = ResidueIntervalPntrNew();
                        rsip->molecule_id = pmmdThis->iChainId;
                        rsip->from = from;
                        rsip->to = to;
                        if (!rsip_head) {
                            rsip_head = rsip;
                            rsip_tail = rsip;
                        } else {
                            rsip_tail->next = rsip;
                            rsip_tail = rsip;
                        }

                        from = pdnmgThis->choice;
                        to = pdnmgThis->choice;
                        iMolCurrent = iMol + 1;
                    }
                }
            }
        }
        if (UserDefinedFeatureFoundInThisMolecule) {
            rsip = ResidueIntervalPntrNew();
            rsip->molecule_id = pmmdThis->iChainId;
            rsip->from = from;
            rsip->to = to;
            if (!rsip_head) {
                rsip_head = rsip;
                rsip_tail = rsip;
            } else {
                rsip_tail->next = rsip;
                rsip_tail = rsip;
            }                   /* for either last region or single region */
        }

        iMol++;
      setout1:
        pdnmmHead = pdnmmHead->next;
    }

    if (!UserDefinedFeatureFound)
        return NULL;

    rsp = ValNodeNew(NULL);
    rsp->choice = ResiduePntrs_interval;
    rsp->data.ptrvalue = rsip_head;

    pcgpThis = ValNodeNew(NULL);
    pcgpThis->choice = ChemGraphPntrs_residues;
    pcgpThis->data.ptrvalue = rsp;

    llp = ValNodeNew(NULL);
    llp->choice = Location_location_subgraph;
    llp->data.ptrvalue = pcgpThis;

    return llp;
}

/*-------------------------------------------*/
static UserFieldPtr FillUserObjectField(UserFieldPtr ufp, ObjectIdPtr oip)
{
    ufp->choice = 1;
    ufp->label = oip;
    return ufp;
}

/*-------------------------------------------*/
static UserObjectPtr MakeUserObjectForUserDefinedFeature(PARS pars)
{
    UserObjectPtr ubp = NULL;
    UserFieldPtr head_ufp = NULL, curr_ufp = NULL, tail_ufp = NULL;
    ObjectIdPtr oip = NULL;
    Char str[20];
    Uint1Ptr rgb = NULL;

    head_ufp = UserFieldNew();
    oip = ObjectIdNew();
    oip->str = StringSave("Protein Backbone Representative");
    FillUserObjectField(head_ufp, oip);

    if (pars->PVirtualBBOn)
        head_ufp->data.ptrvalue = StringSave("Alpha C Trace");
    else if (pars->PPartialBBOn)
        head_ufp->data.ptrvalue = StringSave("partial atom");
    else if (pars->PCompleteBBOn)
        head_ufp->data.ptrvalue = StringSave("all atoms");
    else
        head_ufp->data.ptrvalue = StringSave("none");

    curr_ufp = head_ufp;

    tail_ufp = UserFieldNew();
    oip = ObjectIdNew();
    oip->str = StringSave("Protein Backbone Render");
    FillUserObjectField(tail_ufp, oip);
    if (pars->PBBRender == R_WIRE)
        tail_ufp->data.ptrvalue = StringSave("Wire Frame");
    else if (pars->PBBRender == R_STICK)
        tail_ufp->data.ptrvalue = StringSave("Tubes");
    else if (pars->PBBRender == R_BALLNSTICK)
        tail_ufp->data.ptrvalue = StringSave("Ball & Stick");
    else if (pars->PBBRender == R_THICKWIRE)
        tail_ufp->data.ptrvalue = StringSave("Fat Tubes");
    else if (pars->PBBRender == R_SPACE)
        tail_ufp->data.ptrvalue = StringSave("Space Fill");

    curr_ufp->next = tail_ufp;
    curr_ufp = tail_ufp;

    tail_ufp = UserFieldNew();
    oip = ObjectIdNew();
    oip->str = StringSave("Protein Backbone Color");
    FillUserObjectField(tail_ufp, oip);
    rgb = pars->PBBColRGB;
    sprintf(str, "%d %d %d", *rgb, *(rgb + 1), *(rgb + 2));
    tail_ufp->data.ptrvalue = StringSave(str);

    curr_ufp->next = tail_ufp;
    curr_ufp = tail_ufp;

    tail_ufp = UserFieldNew();
    oip = ObjectIdNew();
    oip->str = StringSave("Protein Side Chain Status");
    FillUserObjectField(tail_ufp, oip);
    if (pars->PResiduesOn)
        tail_ufp->data.ptrvalue = StringSave("On");
    else
        tail_ufp->data.ptrvalue = StringSave("Off");

    curr_ufp->next = tail_ufp;
    curr_ufp = tail_ufp;

    tail_ufp = UserFieldNew();
    oip = ObjectIdNew();
    oip->str = StringSave("Protein Side Chain Render");
    FillUserObjectField(tail_ufp, oip);
    if (pars->PResRender == R_WIRE)
        tail_ufp->data.ptrvalue = StringSave("Wire Frame");
    else if (pars->PResRender == R_STICK)
        tail_ufp->data.ptrvalue = StringSave("Tubes");
    else if (pars->PResRender == R_BALLNSTICK)
        tail_ufp->data.ptrvalue = StringSave("Ball & Stick");
    else if (pars->PResRender == R_THICKWIRE)
        tail_ufp->data.ptrvalue = StringSave("Fat Tubes");
    else if (pars->PResRender == R_SPACE)
        tail_ufp->data.ptrvalue = StringSave("Space Fill");

    curr_ufp->next = tail_ufp;
    curr_ufp = tail_ufp;

    tail_ufp = UserFieldNew();
    oip = ObjectIdNew();
    oip->str = StringSave("Protein Side Chain Color");
    FillUserObjectField(tail_ufp, oip);
    rgb = pars->PResColRGB;
    sprintf(str, "%d %d %d", *rgb, *(rgb + 1), *(rgb + 2));
    tail_ufp->data.ptrvalue = StringSave(str);

    curr_ufp->next = tail_ufp;
    curr_ufp = tail_ufp;

    tail_ufp = UserFieldNew();
    oip = ObjectIdNew();
    oip->str = StringSave("Protein Label Status");
    FillUserObjectField(tail_ufp, oip);
    sprintf(str, "%d", (Uint1) pars->PBBLabelInterval);
    tail_ufp->data.ptrvalue = StringSave(str);

    curr_ufp->next = tail_ufp;
    curr_ufp = tail_ufp;

    tail_ufp = UserFieldNew();
    oip = ObjectIdNew();
    oip->str = StringSave("Protein Label Just");
    FillUserObjectField(tail_ufp, oip);
    sprintf(str, "%d", (Uint1) pars->PBBLabelJust);
    tail_ufp->data.ptrvalue = StringSave(str);

    curr_ufp->next = tail_ufp;
    curr_ufp = tail_ufp;

    tail_ufp = UserFieldNew();
    oip = ObjectIdNew();
    oip->str = StringSave("Protein Label Scale");
    FillUserObjectField(tail_ufp, oip);
    sprintf(str, "%d", (Uint1) pars->PBBLabelScale);
    tail_ufp->data.ptrvalue = StringSave(str);

    curr_ufp->next = tail_ufp;
    curr_ufp = tail_ufp;

    tail_ufp = UserFieldNew();
    oip = ObjectIdNew();
    oip->str = StringSave("Protein Label Style");
    FillUserObjectField(tail_ufp, oip);
    sprintf(str, "%d", (Uint1) pars->PBBLabelStyle);
    tail_ufp->data.ptrvalue = StringSave(str);

    curr_ufp->next = tail_ufp;
    curr_ufp = tail_ufp;

    tail_ufp = UserFieldNew();
    oip = ObjectIdNew();
    oip->str = StringSave("Nucleic Acid Backbone Representative");
    FillUserObjectField(tail_ufp, oip);

    if (pars->NTVirtualBBOn)
        tail_ufp->data.ptrvalue = StringSave("Alpha C Trace");
    else if (pars->NTPartialBBOn)
        tail_ufp->data.ptrvalue = StringSave("partial atom");
    else if (pars->NTCompleteBBOn)
        tail_ufp->data.ptrvalue = StringSave("all atoms");
    else
        tail_ufp->data.ptrvalue = StringSave("none");

    curr_ufp->next = tail_ufp;
    curr_ufp = tail_ufp;

    tail_ufp = UserFieldNew();
    oip = ObjectIdNew();
    oip->str = StringSave("Nucleic Acid Backbone Render");
    FillUserObjectField(tail_ufp, oip);
    if (pars->NTBBRender == R_WIRE)
        tail_ufp->data.ptrvalue = StringSave("Wire Frame");
    else if (pars->NTBBRender == R_STICK)
        tail_ufp->data.ptrvalue = StringSave("Tubes");
    else if (pars->NTBBRender == R_BALLNSTICK)
        tail_ufp->data.ptrvalue = StringSave("Ball & Stick");
    else if (pars->NTBBRender == R_THICKWIRE)
        tail_ufp->data.ptrvalue = StringSave("Fat Tubes");
    else if (pars->NTBBRender == R_SPACE)
        tail_ufp->data.ptrvalue = StringSave("Space Fill");

    curr_ufp->next = tail_ufp;
    curr_ufp = tail_ufp;

    tail_ufp = UserFieldNew();
    oip = ObjectIdNew();
    oip->str = StringSave("Nucleic Acid Backbone Color");
    FillUserObjectField(tail_ufp, oip);
    rgb = pars->NTBBColRGB;
    sprintf(str, "%d %d %d", *rgb, *(rgb + 1), *(rgb + 2));
    tail_ufp->data.ptrvalue = StringSave(str);

    curr_ufp->next = tail_ufp;
    curr_ufp = tail_ufp;

    tail_ufp = UserFieldNew();
    oip = ObjectIdNew();
    oip->str = StringSave("Nucleic Acid Side Chain Status");
    FillUserObjectField(tail_ufp, oip);
    if (pars->NTResiduesOn)
        tail_ufp->data.ptrvalue = StringSave("On");
    else
        tail_ufp->data.ptrvalue = StringSave("Off");

    curr_ufp->next = tail_ufp;
    curr_ufp = tail_ufp;

    tail_ufp = UserFieldNew();
    oip = ObjectIdNew();
    oip->str = StringSave("Nucleic Side Chain Render");
    FillUserObjectField(tail_ufp, oip);
    if (pars->NTResRender == R_WIRE)
        tail_ufp->data.ptrvalue = StringSave("Wire Frame");
    else if (pars->NTResRender == R_STICK)
        tail_ufp->data.ptrvalue = StringSave("Tubes");
    else if (pars->NTResRender == R_BALLNSTICK)
        tail_ufp->data.ptrvalue = StringSave("Ball & Stick");
    else if (pars->NTResRender == R_THICKWIRE)
        tail_ufp->data.ptrvalue = StringSave("Fat Tubes");
    else if (pars->NTResRender == R_SPACE)
        tail_ufp->data.ptrvalue = StringSave("Space Fill");

    curr_ufp->next = tail_ufp;
    curr_ufp = tail_ufp;

    tail_ufp = UserFieldNew();
    oip = ObjectIdNew();
    oip->str = StringSave("Nucleic Acid Side Chain Color");
    FillUserObjectField(tail_ufp, oip);
    rgb = pars->NTResColRGB;
    sprintf(str, "%d %d %d", *rgb, *(rgb + 1), *(rgb + 2));
    tail_ufp->data.ptrvalue = StringSave(str);

    curr_ufp->next = tail_ufp;
    curr_ufp = tail_ufp;

    tail_ufp = UserFieldNew();
    oip = ObjectIdNew();
    oip->str = StringSave("Nucleic Acid Label Status");
    FillUserObjectField(tail_ufp, oip);
    sprintf(str, "%d", (Uint1) pars->NTBBLabelInterval);
    tail_ufp->data.ptrvalue = StringSave(str);

    curr_ufp->next = tail_ufp;
    curr_ufp = tail_ufp;

    tail_ufp = UserFieldNew();
    oip = ObjectIdNew();
    oip->str = StringSave("Nucleic Acid Label Just");
    FillUserObjectField(tail_ufp, oip);
    sprintf(str, "%d", (Uint1) pars->NTBBLabelJust);
    tail_ufp->data.ptrvalue = StringSave(str);

    curr_ufp->next = tail_ufp;
    curr_ufp = tail_ufp;

    tail_ufp = UserFieldNew();
    oip = ObjectIdNew();
    oip->str = StringSave("Nucleic Acid Label Scale");
    FillUserObjectField(tail_ufp, oip);
    sprintf(str, "%d", (Uint1) pars->NTBBLabelScale);
    tail_ufp->data.ptrvalue = StringSave(str);

    curr_ufp->next = tail_ufp;
    curr_ufp = tail_ufp;

    tail_ufp = UserFieldNew();
    oip = ObjectIdNew();
    oip->str = StringSave("Nucleic Acid Label Style");
    FillUserObjectField(tail_ufp, oip);
    sprintf(str, "%d", (Uint1) pars->NTBBLabelStyle);
    tail_ufp->data.ptrvalue = StringSave(str);

    curr_ufp->next = tail_ufp;
    curr_ufp = tail_ufp;
    oip = ObjectIdNew();
    oip->str = StringSave("Cn3D Rendering");

    ubp = UserObjectNew();
    ubp->type = oip;
    ubp->data = head_ufp;
    ubp->_class = StringSave("Property for User Defined Feature");

    return (ubp);
}

/*-------------------------------------------*/
static ValNodePtr
MakeBiostrucFeaturePropertyForUserDefinedFeature(SpecialFeaturePtr sfpThis)
{
    ValNodePtr ppp = NULL;
    SpecialFeatureInfoPtr sfipThis = NULL;
    PARS parsThis = NULL;

    sfipThis = sfpThis->data.ptrvalue;
    parsThis = sfipThis->parsSpecial;

    ppp = ValNodeNew(NULL);
    ppp->choice = 6;
    ppp->data.ptrvalue = MakeUserObjectForUserDefinedFeature(parsThis);

    return ppp;
}

/*-------------------------------------------*/
static BiostrucFeatureSetPtr
MakeBiostrucFeatureSetForUserDefinedFeature(PDNMS pdnmsThis,
                                            BiostrucPtr bsp,
                                            SpecialFeaturePtr sfpThis,
                                            Int2 iCount)
{
    BiostrucFeatureSetPtr bsfsp = NULL, bsfsp_head = NULL, bsfsp_tail =
        NULL;
    BiostrucFeaturePtr bsfp = NULL;
    ValNodePtr pvnLoc;
    SpecialFeatureInfoPtr sfipThis = NULL;

    sfipThis = sfpThis->data.ptrvalue;

    pvnLoc = GetFeatureLocationForModel(sfipThis, (PMSD) pdnmsThis->data.ptrvalue);
    if (!pvnLoc) return NULL;

    bsfp = BiostrucFeatureNew();
    if (!bsfp) return NULL;
    bsfp->name = StringSave(sfipThis->name);
    bsfp->type = Feature_type_other;
    bsfp->Location_location = CopyLocation(pvnLoc);

    bsfp->Property_property =
        MakeBiostrucFeaturePropertyForUserDefinedFeature(sfpThis);

    bsfsp = BiostrucFeatureSetNew();
    bsfsp->id = iCount;
    bsfsp->descr = MakeDescrForUserDefinedFeature(sfpThis);
    bsfsp->features = bsfp;
    bsfsp->next = NULL;

    return bsfsp;
}

/*-------------------------------------------*/
PDNMS Cn3DAddUserDefinedFeatureToBiostruc(PDNMS pdnmsThis)
{
    BiostrucPtr bsp = NULL;
    PMSD pmsdThis = NULL;
    BiostrucFeatureSetPtr bsfsp = NULL, bsfsp_head = NULL, bsfsp_tail = NULL;
    SpecialFeaturePtr sfpThis = NULL;
    Int2 iCount;

    pmsdThis = pdnmsThis->data.ptrvalue;
    bsp = pmsdThis->pbsBS;

    sfpThis = sfpGlobal;
    iCount = 1;
    while (sfpThis) {
        bsfsp =
            MakeBiostrucFeatureSetForUserDefinedFeature(pdnmsThis, bsp,
                                                       sfpThis, iCount);
        if (bsfsp) {
            if (!bsfsp_head) {
                bsfsp_head = bsfsp;
                bsfsp_tail = bsfsp;
            } else {
                bsfsp_tail->next = bsfsp;
                bsfsp_tail = bsfsp;
            }
        }

        sfpThis = sfpThis->next;
        iCount++;
    }

    bsfsp = bsp->features;
    while (bsfsp->next) {
        bsfsp = bsfsp->next;
    }
    bsfsp->next = bsfsp_head;

    return pdnmsThis;
}

/*---------------------------------------*/
static SpecialFeatureInfoPtr GetUserDefinedFeature(Int4 id)
{
    SpecialFeaturePtr sfpThis = NULL;
    SpecialFeatureInfoPtr sfipThis = NULL;

    sfpThis = sfpThis_head;
    while (sfpThis) {
        if (sfpThis->choice == id) {
            return sfpThis->data.ptrvalue;
        }
        sfpThis = sfpThis->next;
    }
    return NULL;
}

/*-------------------------------------*/
static void Cn3DApplyUserDefinedFeatureToMGD(PMSD pmsdThis, ValNodePtr llp,
                                      Int2 iFeature, SpecialFeatureInfoPtr sfip)
{
    PDNMM pdnmmHead = NULL;
    PMMD pmmdThis = NULL;
    PDNMG pdnmgThis = NULL;
    PMGD pmgdThis = NULL;
    ChemGraphPntrsPtr pcgpThis = NULL;
    ValNodePtr rsp = NULL;
    ResidueIntervalPntrPtr rsip = NULL;

    pcgpThis = llp->data.ptrvalue;
    rsp = pcgpThis->data.ptrvalue;
    rsip = rsp->data.ptrvalue;
    while (rsip) {
        pdnmmHead = pmsdThis->pdnmmHead;
        while (pdnmmHead) {
            pmmdThis = pdnmmHead->data.ptrvalue;
            if (pmmdThis->bWhat != (Byte) AM_PROT
                && pmmdThis->bWhat != (Byte) AM_RNA
                && pmmdThis->bWhat != (Byte) AM_DNA)
                goto setout1;
            if (rsip->molecule_id == pmmdThis->iChainId) {
                for (pdnmgThis = pmmdThis->pdnmgHead; pdnmgThis != NULL;
                     pdnmgThis = pdnmgThis->next) {
                    if (pdnmgThis->choice >= rsip->from
                        && pdnmgThis->choice <= rsip->to) {
                        pmgdThis = (PMGD) pdnmgThis->data.ptrvalue;
                        if (sfip->isOn)
                            AddPARSToMGD(pmgdThis, iFeature, sfip->parsSpecial);
                        else
                            RemovePARSFromMGD((PFB) pmgdThis, 0, iFeature, NULL);
                    }
                }
            }
          setout1:
            pdnmmHead = pdnmmHead->next;
        }
        rsip = rsip->next;
    }
}

/*-------------------------------------*/
static PARS GetSpecialParsForUserDefinedFeature(UserObjectPtr ubp)
{

    PARS pars = NULL;
    UserFieldPtr ufp = NULL;
    ObjectIdPtr oip = NULL;
    short rgb_short[3], num;

    pars = GetNewSpecialAlgorRenderSet();

    ufp = ubp->data;
    while (ufp) {
        oip = ufp->label;
        if (StringCmp(oip->str, "Protein Backbone Representative") == 0) {
            if (StringCmp(ufp->data.ptrvalue, "Alpha C Trace") == 0) {
                pars->PVirtualBBOn = TRUE;
                pars->PPartialBBOn = FALSE;
                pars->PCompleteBBOn = FALSE;
            } else if (StringCmp(ufp->data.ptrvalue, "all atoms") == 0) {
                pars->PPartialBBOn = FALSE;
                pars->PVirtualBBOn = FALSE;
                pars->PCompleteBBOn = TRUE;
            } else if (StringCmp(ufp->data.ptrvalue, "partial atom") == 0) {
                pars->PCompleteBBOn = FALSE;
                pars->PVirtualBBOn = FALSE;
                pars->PPartialBBOn = TRUE;
            }
        } else if (StringCmp(oip->str, "Protein Backbone Render") == 0) {
            if (StringCmp(ufp->data.ptrvalue, "Wire Frame") == 0)
                pars->PBBRender = R_WIRE;
            else if (StringCmp(ufp->data.ptrvalue, "Tubes") == 0)
                pars->PBBRender = R_STICK;
            else if (StringCmp(ufp->data.ptrvalue, "Ball & Stick") == 0)
                pars->PBBRender = R_BALLNSTICK;
            else if (StringCmp(ufp->data.ptrvalue, "Fat Tubes") == 0)
                pars->PBBRender = R_THICKWIRE;
            else if (StringCmp(ufp->data.ptrvalue, "Space Fill") == 0)
                pars->PBBRender = R_SPACE;
        } else if (StringCmp(oip->str, "Protein Backbone Color") == 0) {
            sscanf(ufp->data.ptrvalue, "%hd %hd %hd", &rgb_short[0],
                   &rgb_short[1], &rgb_short[2]);
            pars->PBBColRGB[0] = (Uint1) rgb_short[0];
            pars->PBBColRGB[1] = (Uint1) rgb_short[1];
            pars->PBBColRGB[2] = (Uint1) rgb_short[2];
            pars->PBBColor = C_BYCHOICE;

        } else if (StringCmp(oip->str, "Protein Side Chain Status") == 0) {
            if (StringCmp(ufp->data.ptrvalue, "On") == 0)
                pars->PResiduesOn = TRUE;
            else
                pars->PResiduesOn = FALSE;
        } else if (StringCmp(oip->str, "Protein Side Chain Render") == 0) {
            if (StringCmp(ufp->data.ptrvalue, "Wire Frame") == 0)
                pars->PResRender = R_WIRE;
            else if (StringCmp(ufp->data.ptrvalue, "Tubes") == 0)
                pars->PResRender = R_STICK;
            else if (StringCmp(ufp->data.ptrvalue, "Ball & Stick") == 0)
                pars->PResRender = R_BALLNSTICK;
            else if (StringCmp(ufp->data.ptrvalue, "Fat Tubes") == 0)
                pars->PResRender = R_THICKWIRE;
            else if (StringCmp(ufp->data.ptrvalue, "Space Fill") == 0)
                pars->PResRender = R_SPACE;
        } else if (StringCmp(oip->str, "Protein Side Chain Color") == 0) {
            sscanf(ufp->data.ptrvalue, "%hd %hd %hd", &rgb_short[0],
                   &rgb_short[1], &rgb_short[2]);
            pars->PResColRGB[0] = (Uint1) rgb_short[0];
            pars->PResColRGB[1] = (Uint1) rgb_short[1];
            pars->PResColRGB[2] = (Uint1) rgb_short[2];
            pars->PResColor = C_BYCHOICE;
        } else if (StringCmp(oip->str, "Protein Label Status") == 0) {
            sscanf(ufp->data.ptrvalue, "%hd", &num);
            pars->PBBLabelInterval = (Uint1) num;
        } else if (StringCmp(oip->str, "Protein Label Just") == 0) {
            sscanf(ufp->data.ptrvalue, "%hd", &num);
            pars->PBBLabelJust = (Uint1) num;
        } else if (StringCmp(oip->str, "Protein Label Scale") == 0) {
            sscanf(ufp->data.ptrvalue, "%hd", &num);
            pars->PBBLabelScale = (Uint1) num;
        } else if (StringCmp(oip->str, "Protein Label Style") == 0) {
            sscanf(ufp->data.ptrvalue, "%hd", &num);
            pars->PBBLabelStyle = (Uint1) num;
        }
        if (StringCmp(oip->str, "Nucleic Acid Backbone Representative") ==
            0) {
            if (StringCmp(ufp->data.ptrvalue, "Alpha C Trace") == 0) {
                pars->NTVirtualBBOn = TRUE;
                pars->NTPartialBBOn = FALSE;
                pars->NTCompleteBBOn = FALSE;
            } else if (StringCmp(ufp->data.ptrvalue, "all atoms") == 0) {
                pars->NTPartialBBOn = FALSE;
                pars->NTVirtualBBOn = FALSE;
                pars->NTCompleteBBOn = TRUE;
            } else if (StringCmp(ufp->data.ptrvalue, "partial atom") == 0) {
                pars->NTCompleteBBOn = FALSE;
                pars->NTVirtualBBOn = FALSE;
                pars->NTPartialBBOn = TRUE;
            }
        }
            else if (StringCmp(oip->str, "Nucleic Acid Backbone Render") ==
                     0) {
            if (StringCmp(ufp->data.ptrvalue, "Wire Frame") == 0)
                pars->NTBBRender = R_WIRE;
            else if (StringCmp(ufp->data.ptrvalue, "Tubes") == 0)
                pars->NTBBRender = R_STICK;
            else if (StringCmp(ufp->data.ptrvalue, "Ball & Stick") == 0)
                pars->NTBBRender = R_BALLNSTICK;
            else if (StringCmp(ufp->data.ptrvalue, "Fat Tubes") == 0)
                pars->NTBBRender = R_THICKWIRE;
            else if (StringCmp(ufp->data.ptrvalue, "Space Fill") == 0)
                pars->NTBBRender = R_SPACE;
        } else if (StringCmp(oip->str, "Nucleic Acid Backbone Color") == 0) {
            sscanf(ufp->data.ptrvalue, "%hd %hd %hd", &rgb_short[0],
                   &rgb_short[1], &rgb_short[2]);
            pars->NTBBColRGB[0] = (Uint1) rgb_short[0];
            pars->NTBBColRGB[1] = (Uint1) rgb_short[1];
            pars->NTBBColRGB[2] = (Uint1) rgb_short[2];
            pars->NTBBColor = C_BYCHOICE;
        }
            else if (StringCmp(oip->str, "Nucleic Acid Side Chain Status")
                     == 0) {
            if (StringCmp(ufp->data.ptrvalue, "On") == 0)
                pars->NTResiduesOn = TRUE;
            else
                pars->NTResiduesOn = FALSE;
        } else if (StringCmp(oip->str, "Nucleic Side Chain Render") == 0) {
            if (StringCmp(ufp->data.ptrvalue, "Wire Frame") == 0)
                pars->NTResRender = R_WIRE;
            else if (StringCmp(ufp->data.ptrvalue, "Tubes") == 0)
                pars->NTResRender = R_STICK;
            else if (StringCmp(ufp->data.ptrvalue, "Ball & Stick") == 0)
                pars->NTResRender = R_BALLNSTICK;
            else if (StringCmp(ufp->data.ptrvalue, "Fat Tubes") == 0)
                pars->NTResRender = R_THICKWIRE;
            else if (StringCmp(ufp->data.ptrvalue, "Space Fill") == 0)
                pars->NTResRender = R_SPACE;
        }
            else if (StringCmp(oip->str, "Nucleic Acid Side Chain Color")
                     == 0) {
            sscanf(ufp->data.ptrvalue, "%hd %hd %hd", &rgb_short[0],
                   &rgb_short[1], &rgb_short[2]);
            pars->NTResColRGB[0] = (Uint1) rgb_short[0];
            pars->NTResColRGB[1] = (Uint1) rgb_short[1];
            pars->NTResColRGB[2] = (Uint1) rgb_short[2];
            pars->NTResColor = C_BYCHOICE;
        } else if (StringCmp(oip->str, "Nucleic Acid Label Status") == 0) {
            sscanf(ufp->data.ptrvalue, "%hd", &num);
            pars->NTBBLabelInterval = (Uint1) num;
        } else if (StringCmp(oip->str, "Nucleic Acid Label Just") == 0) {
            sscanf(ufp->data.ptrvalue, "%hd", &num);
            pars->NTBBLabelJust = (Uint1) num;
        } else if (StringCmp(oip->str, "Nucleic Acid Label Scale") == 0) {
            sscanf(ufp->data.ptrvalue, "%hd", &num);
            pars->NTBBLabelScale = (Uint1) num;
        } else if (StringCmp(oip->str, "Nucleic Acid Label Style") == 0) {
            sscanf(ufp->data.ptrvalue, "%hd", &num);
            pars->NTBBLabelStyle = (Uint1) num;
        }

        ufp = ufp->next;
    }

    return pars;
}

/*-------------------------------------*/
void Cn3DRemoveUserDefinedFeatureFromBiostruc(PDNMS pdnmsThis)
{
    BiostrucPtr bsp;
    BiostrucFeatureSetPtr bsfsp, bsfsp_prev = NULL, bsfsp_curr = NULL;
    ValNodePtr descr = NULL;

    if (!pdnmsThis || !pdnmsThis->data.ptrvalue ||
        !(bsp = ((PMSD) pdnmsThis->data.ptrvalue)->pbsBS))
        return;

    /* assuming these feature data always occur at the tail end of the feature list,
       unhook them from the biostruc and free them */
    bsfsp = bsp->features;
    while (bsfsp) {
        if(bsfsp->descr != NULL) {
            descr = bsfsp->descr;
            if (descr->choice == 1) {
                if (StringCmp(descr->data.ptrvalue, "User Defined Features") == 0) {
                    if (bsfsp_prev)
                        bsfsp_prev->next = NULL;
                    bsfsp_curr = bsfsp;
                    break;
                }
            }
        }
        bsfsp_prev = bsfsp;
        bsfsp = bsfsp->next;
    }
    BiostrucFeatureSetFree(bsfsp_curr);
}

/*-------------------------------------*/
static void Cn3DIndexUserDefinedFeatureForMSD(PDNMS pdnmsThis)
{
    PMSD pmsdThis = NULL;
    BiostrucPtr bsp = NULL;
    BiostrucFeatureSetPtr bsfsp = NULL;
    ValNodePtr descr = NULL;
    SpecialFeaturePtr sfpThis = NULL;
    SpecialFeatureInfoPtr sfipThis = NULL;
    UserObjectPtr ubp = NULL;
    ValNodePtr llp = NULL, ppp = NULL;
    Boolean UserDefinedFeatureFound = FALSE;
    Boolean NewUserDefinedFeature = FALSE;

    if (pdnmsThis == NULL)
        return;

    pmsdThis = pdnmsThis->data.ptrvalue;
    bsp = pmsdThis->pbsBS;
    bsfsp = bsp->features;
    while (bsfsp) {
        descr = bsfsp->descr;
        UserDefinedFeatureFound = FALSE;
        NewUserDefinedFeature = FALSE;

        while (descr) {
            if (descr->choice == BiostrucFeatureSetDescr_name) {
                if (StringCmp(descr->data.ptrvalue, "User Defined Features") == 0) {
                    UserDefinedFeatureFound = TRUE;
                    sfipThis = GetUserDefinedFeature(bsfsp->id);
                    if (sfipThis == NULL) {
                        sfipThis = SpecialFeatureInfoNew();
                        sfipThis->isOn = FALSE;
                        NewUserDefinedFeature = TRUE;
                    }
                }
            } else if (descr->choice == BiostrucFeatureSetDescr_other_comment) {
                if (NewUserDefinedFeature) {
                    if (StringCmp(descr->data.ptrvalue, "On") == 0) {
                        sfipThis->isOn = TRUE;
                    } else if (descr->data.ptrvalue != NULL) {
                        sfipThis->description =
                            StringSave(descr->data.ptrvalue);
                    }
                }
            }
            descr = descr->next;
        }

        if (UserDefinedFeatureFound) {
            BiostrucFeaturePtr bsfp = bsfsp->features;

            if (NewUserDefinedFeature) {
                sfipThis->name = StringSave(bsfp->name);
                ppp = bsfp->Property_property;
                ubp = ppp->data.ptrvalue;
                sfipThis->parsSpecial =
                    GetSpecialParsForUserDefinedFeature(ubp);

                iNumFeatures++;
                sfpThis = ValNodeNew(NULL);
                if (sfpThis) {
                    ValNodePtr sfpBefore = NULL, sfpAfter = sfpThis_head;

                    sfpThis->choice = bsfsp->id;
                    sfpThis->data.ptrvalue = sfipThis;

                    /* add to list in order of id (choice) */
                    while (sfpAfter && sfpAfter->choice < sfpThis->choice) {
                        sfpBefore = sfpAfter;
                        sfpAfter = sfpAfter->next;
                    }
                    sfpThis->next = sfpAfter;
                    if (!sfpAfter) sfpThis_tail = sfpThis;
                    if (sfpBefore)
                        sfpBefore->next = sfpThis;
                    else
                        sfpThis_head = sfpThis;
                }
            }

            llp = CopyLocation(bsfp->Location_location);
            ValNodeAddPointer(&(sfipThis->pvnLocations), 0, (VoidPtr) llp); /* store location for each model */
            ValNodeAddPointer(&(sfipThis->pvnModels), 0, (VoidPtr) pmsdThis);

            Cn3DApplyUserDefinedFeatureToMGD(pmsdThis, llp, bsfsp->id, sfipThis);
        }

        bsfsp = bsfsp->next;
    }

    /* remove features so they can be edited; will be re-added upon save */
    Cn3DRemoveUserDefinedFeatureFromBiostruc(pdnmsThis);
}

/*-------------------------------------*/
void Cn3DIndexUserDefinedFeature(void)
{
    PDNMS pdnmsThis = NULL, pdnmsThisSlave = NULL;
    PMSD pmsdThis = NULL;
    PDNMM pdnmmHead = NULL;
    PMMD pmmdThis = NULL;

    SpecialFeaturePtr sfpThis = NULL;

    pdnmsThis = GetSelectedModelstruc();
    if (!pdnmsThis)
        return;

    iNumFeatures = 0;

    Cn3DIndexUserDefinedFeatureForMSD(pdnmsThis);
    pdnmsThisSlave = ((PMSD) (pdnmsThis->data.ptrvalue))->pdnmsSlaves;
    while (pdnmsThisSlave) {
        Cn3DIndexUserDefinedFeatureForMSD(pdnmsThisSlave);
        pdnmsThisSlave = pdnmsThisSlave->next;
    }

    sfpGlobal = sfpThis_head;
    sfpThis = sfpGlobal;

    Cn3D_SyncMenuHighlightWithOnOff();
    UpdateEditFeatureList();
}

/*-------------------------------------------*/
static void ClearSpecialFeature(void)
{
    SpecialFeaturePtr sfpThis = NULL, next = NULL;

    sfpThis = sfpGlobal;
    while (sfpThis) {
        next = sfpThis->next;
        sfpThis->next = NULL;
        sfpThis = SpecialFeatureFree(sfpThis);
        sfpThis = next;
    }

    sfpGlobal = NULL;
    sfpThis_head = NULL;
    sfpThis_tail = NULL;
    parsSpecial = NULL;
    iNumFeatures = 0;
    iEditedFeature = 0;
}


/*-------------------------------------------*/
extern void ClearDomainData(void); /* in cn3dmodl.c */

void ClearRest(void)
{
    if (sfpGlobal != NULL)
        ClearSpecialFeature();

    ClearDomainData();
}

