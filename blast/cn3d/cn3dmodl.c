/* $Id: cn3dmodl.c,v 6.92 2001/04/27 20:07:59 juran Exp $
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
* =========================================================================== 
*
* File Name: $Id: cn3dmodl.c,v 6.92 2001/04/27 20:07:59 juran Exp $
*
* Author: Yanli Wang
*
* Version Creation Date: 18/9/1998
*
* $Log: cn3dmodl.c,v $
* Revision 6.92  2001/04/27 20:07:59  juran
* Heed warnings.
*
* Revision 6.91  2000/04/08 00:37:31  lewisg
* multiple seqentries, NEWSEQ message, etc.
*
* Revision 6.89  2000/03/23 18:27:25  thiessen
* show-by-selected now works for hets/ions/water
*
* Revision 6.88  2000/03/15 04:43:55  thiessen
* various minor fixes
*
* Revision 6.87  2000/03/14 18:03:13  thiessen
* add target row to MMD; remove rowmgr
*
* Revision 6.86  2000/03/10 18:46:59  lewisg
* add show/hide
*
* Revision 6.85  2000/02/28 14:47:59  thiessen
* fixed "show selected only" in show/hide
*
* Revision 6.84  2000/02/26 00:01:40  thiessen
* OpenGL improvements, progress on cleanup of Show/Hide
*
* Revision 6.83  2000/02/25 02:53:14  thiessen
* check AlignMgr rather than bAligned flag to see if a residue is aligned
*
* Revision 6.82  2000/02/19 21:25:56  thiessen
* split of cn3dmodl into cn3dmodl and cn3dstyl
*
* Revision 6.81  2000/02/14 12:32:31  thiessen
* fix solvent identification; progress on seqcons module
*
* Revision 6.80  2000/02/11 15:52:28  thiessen
* transparent solvent if partial occupancy; move alt conf behaviour checkbox to Show/Hide panel
*
* Revision 6.79  2000/02/10 15:51:59  lewisg
* cn3d responds and send correct update messages.  many coloring bug fixes
*
* Revision 6.78  2000/02/07 20:17:37  lewisg
* minor bug fixes, use gui font for win32
*
* Revision 6.77  1999/12/28 23:06:35  lewisg
* udv/cn3d communication
*
* Revision 6.76  1999/12/28 15:08:43  lewisg
* remove remaining mediainfo code
*
* Revision 6.75  1999/12/27 23:14:11  lewisg
* add colormgr show/hide in Cn3D
*
* Revision 6.74  1999/12/16 15:25:22  thiessen
* removed unused buttons/popups from OpenGL version
*
* Revision 6.73  1999/12/11 01:30:34  lewisg
* fix bugs with sharing colors between ddv and cn3d
*
* Revision 6.72  1999/11/22 14:46:43  thiessen
* moved _OPENGL code blocks to only vibrant and ncbicn3d libraries
*
* Revision 6.71  1999/11/15 18:30:08  lewisg
* get rid of extra redraws when selecting
*
* Revision 6.70  1999/11/10 23:19:41  lewisg
* rewrite of selection code for ddv
*
* Revision 6.69  1999/11/03 16:24:21  lewisg
* get rid of non-existing includes
*
* Revision 6.68  1999/11/02 23:06:07  lewisg
* fix cn3d to launch correctly if there is no seqentry associated with bioseq
*
* Revision 6.67  1999/10/29 14:15:28  thiessen
* ran all Cn3D source through GNU Indent to prettify
*
* Revision 6.66  1999/10/15 20:56:39  lewisg
* append DDV_ColorGlobal as userdata.  free memory when cn3d terminates.
*
* Revision 6.65  1999/10/05 23:18:20  lewisg
* add ddv and udv to cn3d with memory management
*
* Revision 6.64  1999/09/21 18:09:15  lewisg
* binary search added to color manager, various bug fixes, etc.
*
* Revision 6.63  1999/09/10 13:49:38  lewisg
* eliminate spaces before preprocessor directives
*
* Revision 6.62  1999/07/27 14:19:09  ywang
* fix previous in another way so that not to break any existant code
*
* Revision 6.61  1999/07/26 21:42:47  vakatov
* Fixed a multiple ValNode list deallocation in
* ClearSpecialFeature/SpecialFeatureFree
*
* Revision 6.60  1999/07/15 15:59:41  ywang
* improve feature control, call Cn3D_DeHighlightAll after ObjMgrDeSelectAll
*
* Revision 6.59  1999/07/12 14:15:37  ywang
* initialize ButtoN b for internal call Cn3D_ListDomainProc & Cn3D_ListStrucProc
*
* Revision 6.58  1999/07/07 20:45:35  ywang
* clear domaindata, mediadata, special feature before reading in new data in cn3d
*
* Revision 6.57  1999/07/01 16:01:08  ywang
* improve display control panel
*
* Revision 6.56  1999/06/16 15:13:29  ywang
* for feature editing: direct return for 'Cancel' 'Delete' 'Edit' 'Save' if no residues or no feature name selected
*
* Revision 6.55  1999/06/07 21:25:14  ywang
* add iUserDefinedFeatureOld, FeatureOnOld to protect saved features being overwritten by temporary feature
*
* Revision 6.54  1999/06/07 20:26:11  ywang
* change prompt for featue editing
*
* Revision 6.53  1999/06/07 19:39:50  ywang
* remove obsolete user defined features, an obsolete feature means a feature whose region has been completely overwirtten by a later defined feature
*
* Revision 6.52  1999/06/03 20:29:24  ywang
* fix bug in aligned domain display when they are from same structure
*
* Revision 6.51  1999/05/27 16:11:22  ywang
* initilize all local variables at defined
*
* Revision 6.50  1999/05/03 16:54:23  ywang
* fix bug for recording and matching mol-id for user defined features
*
* Revision 6.49  1999/04/22 21:10:44  ywang
* check NULL pointer on domain counting
*
* Revision 6.48  1999/04/21 21:15:09  ywang
* replace Cn3D_DeHighlightAll with ObjMgrDeSelectAll
*
* Revision 6.46  1999/04/16 22:21:23  ywang
* update residue aligned status on mediadata upon sequence SHOW/HIDE
*
* Revision 6.45  1999/04/15 21:37:35  ywang
* add AssignDomainAlignedStatusForStrucSeqs
*
* Revision 6.44  1999/04/14 17:43:26  ywang
* synchronize object show/hide upon action 'Show Selected Only' action
*
* Revision 6.43  1999/04/14 17:19:15  ywang
* synchronize object show/hide upon action 'Show Selected Only' action
*
* Revision 6.42  1999/04/13 23:06:14  ywang
* fix bug for coloring strucseqs data
*
* Revision 6.41  1999/04/06 20:11:02  lewisg
* more opengl
*
* Revision 6.40  1999/04/02 20:51:57  ywang
* fix warnings
*
* Revision 6.39  1999/03/22 22:41:52  ywang
* redesign feature page, fix bugs
*
* Revision 6.38  1999/03/18 23:13:03  ywang
* modify feature window
*
* Revision 6.37  1999/03/18 22:28:56  ywang
* add functions for saveout+readin+index user defined features
*
* Revision 6.36  1999/03/08 21:16:21  ywang
* initialize variables
*
* Revision 6.35  1999/03/08 19:34:35  ywang
* redesign Feature page
*
* Revision 6.34  1999/03/03 23:17:21  lewisg
* one master struct at a time, list slaves in structure info, bug fixes
*
* Revision 6.33  1999/03/01 20:21:30  ywang
* put in options for residue on/off control
*
* Revision 6.32  1999/02/24 23:00:02  ywang
* minor name change for user defined feature at MGD nodecn3dmodl.c
*
* Revision 6.31  1999/02/12 15:42:56  ywang
* change void static to static void for ClearSpecialFeaturewithMGD(void)
*
* Revision 6.30  1999/02/11 22:38:51  ywang
* fix bug on display highlight residues only--if no res are highlighted, cn3d sets that button status as FALSE and draw whole structurescn3dwin.c
*
* Revision 6.29  1999/02/08 18:42:25  ywang
* improve feature editing
*
* Revision 6.28  1999/02/04 16:14:44  ywang
* support delete added features
*
* Revision 6.27  1999/02/03 23:09:24  ywang
* add functions to allow editing 'added feature' in Model menu
*
* Revision 6.26  1999/02/02 22:25:27  ywang
* improve feature edit function
*
* Revision 6.25  1999/02/01 21:10:09  ywang
* correct mistake
*
* Revision 6.24  1999/02/01 20:43:25  ywang
* improve 'Model' menu
*
* Revision 6.23  1999/01/27 21:51:51  ywang
* add label to 'Model' menu
*
* Revision 6.22  1999/01/27 16:21:55  ywang
* redesign 'Model' menu
*
* Revision 6.21  1999/01/26 17:14:35  ywang
* redesign Display menu and add 'display highlight residues only' function
*
* Revision 6.20  1999/01/20 18:21:19  ywang
* include salmedia.h due to the move around of MediaInfo from cn3dmsg.h to the new created salmedia.h
*
 * Revision 6.19  1998/12/16  22:49:37  ywang
 * fix compiling warnings on Win32
 *
 * Revision 6.17  1998/11/09  22:03:31  ywang
 * fix bugs for modeling
 *
 * Revision 6.16  1998/11/06  23:01:05  ywang
 * fix bugs for modeling
 *
 * Revision 6.15  1998/11/04  13:03:30  kans
 * fixed variable name
 *
* Revision 6.14  1998/11/04 00:06:19  ywang
* add function for modeling: change render/color for special residue(s)
*
 * Revision 6.13  1998/10/21  15:48:23  ywang
 * rearrange View Control menu
 *
 * Revision 6.12  1998/10/20  17:50:37  ywang
 * fix MMDB/VAST domain number inconsistency problem
 *
 * Revision 6.11  1998/10/20  16:54:39  ywang
 * fix bug in AssignDomainAlignedStatus
 *
 * Revision 6.10  1998/10/13  14:44:17  ywang
 * fix bugs for single struc case in AssignDomainAlignedStatus
 *
 * Revision 6.9  1998/10/08  21:39:15  ywang
 * fix bug for assign domain aligned status
 *
 * Revision 6.8  1998/10/08  00:07:19  ywang
 * fix domain aligned status bug
 *
 * Revision 6.6  1998/10/02  18:25:15  kans
 * some functions did not have void as a return, which the Mac compiler detected
 *
* Revision 6.5  1998/10/01 21:55:39  ywang
* put function for object display control
*
 * Revision 6.4  1998/09/30  22:10:46  ywang
 * control display on three levels: structure, chain, domain
 *
 * Revision 6.3  1998/09/23  22:04:02  ywang
 * synchronize show/hide between cn3d and salsa when display complexity is changed
 *
 * Revision 6.2  1998/09/23  18:38:48  ywang
 * add functions to control display on domain level
 *
 * Revision 6.1  1998/09/22  18:02:54  ywang
 * panels and functions for display control
 *
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
#include <cn3dmodl.h>
#include <objmime.h>
#include <cn3dshim.h>
#include <alignmgr.h>


static ButtoN Cn3D_bDisplayHighlight;
static Boolean Cn3D_NoSingleHL = FALSE;
static ButtoN Cn3D_bAlignOn, Cn3D_bUnalignOn;
static ButtoN Cn3D_bDisplayAlignedDomain;
static LisT Cn3D_lStruc;
static LisT Cn3D_lFeature;

static DomainInfo **domaindata;
static Int4 iDomainCount = 0;

Boolean Cn3D_fAlignOn, Cn3D_fUnalignOn; /* globals  for above buttons */

static ButtoN Cn3D_altConfIdOverlay;
Nlm_Boolean allowAltConfIdOverlay(void)
{
    return GetStatus(Cn3D_altConfIdOverlay);
}

static ButtoN Cn3D_bDisplayApply;


/*-------------------------------------------------------------------*/

void LIBCALLBACK Cn3DCheckNoSingleHighlight(PFB pfbThis, Int4 iModel,
                                            Int4 iIndex, Pointer ptr)
{
    PMGD pmgdThis = NULL;
    PDNMG pdnmgThis = NULL;

    pmgdThis = (PMGD) pfbThis;
    if (pmgdThis->bHighlighted == 1)
        Cn3D_NoSingleHL = FALSE;
}

/*-----------------------------------------------*/
void Cn3DCheckHighlighted(void)
{
    PDNMS pdnmsMaster = NULL, pdnmsSlave = NULL;
    PMSD pmsdMaster = NULL, pmsdSlave = NULL;

    /* replace old function which depends on ObjMgr */
    /* by doing so, highlight for non ObjMgr registered residues will */
    /* also be picked up when do Cn3D_Redraw */

    Cn3D_NoSingleHL = TRUE;

    pdnmsMaster = GetSelectedModelstruc();
    if (pdnmsMaster != NULL) {
        pmsdMaster = pdnmsMaster->data.ptrvalue;

        TraverseGraphs(pdnmsMaster, 0, 0, NULL,
                       Cn3DCheckNoSingleHighlight);
        pdnmsSlave = pmsdMaster->pdnmsSlaves;
        while (pdnmsSlave && Cn3D_NoSingleHL) {
            TraverseGraphs(pdnmsSlave, 0, 0, NULL,
                           Cn3DCheckNoSingleHighlight);
            pdnmsSlave = pdnmsSlave->next;
        }
    }
}

/*---------------------------------------------------------*/
static void Cn3D_SelectDomainProc(LisT l)
{
    SetStatus(Cn3D_bDisplayAlignedDomain, FALSE);
}

/*---------------------------------------------------------*/
/* fill out domain list */
NLM_EXTERN void Cn3D_ListDomainProc(void)
{
    Char MolName[20];
    Int4 iCount = 0, iCountActive = 0;

    Reset(Cn3D_lFeature);

    for (iCount = 0; iCount < iDomainCount; iCount++) {
        domaindata[iCount]->bVisibleParent =
            GetItemStatus(Cn3D_lStruc,
                          domaindata[iCount]->iStrucIndex + 1);
    }

    for (iCount = 0; iCount < iDomainCount; iCount++) {
        if (domaindata[iCount]->iDomain == 0)
            sprintf(MolName, "%4s  %2s", domaindata[iCount]->pcPDBName,
                    domaindata[iCount]->pcMolName);
        else
            sprintf(MolName, "%4s  %2s  %2d",
                    domaindata[iCount]->pcPDBName,
                    domaindata[iCount]->pcMolName,
                    domaindata[iCount]->iDomain);
        if (domaindata[iCount]->bVisibleParent) {
            ListItem(Cn3D_lFeature, (CharPtr) MolName);
            iCountActive++;
            if (!GetStatus(Cn3D_bDisplayAlignedDomain))
                SetItemStatus(Cn3D_lFeature, iCountActive,
                              domaindata[iCount]->bVisible);
            else {
                if (domaindata[iCount]->bAligned) {
                    SetItemStatus(Cn3D_lFeature, iCountActive, TRUE);
                    domaindata[iCount]->bVisible = TRUE;
                } else if (!domaindata[iCount]->bAligned) {
                    SetItemStatus(Cn3D_lFeature, iCountActive, FALSE);
                    domaindata[iCount]->bVisible = FALSE;
                }
            }
        }

    }
    return;
}

/*---------------------------------------------------------*/
static void Cn3D_ListAlignedDomainProc(ButtoN b)
{
    Cn3D_ListDomainProc();
}

/*---------------------------------------------------------*/
static void FillFeatureListProc(LisT l)
{
    Cn3D_ListDomainProc();
}

/*---------------------------------------------------------*/
static void SetDomainParentStatus(Int4 Index, Byte bVisible)
{
    Int4 iCount = 0;

    for (iCount = 0; iCount < iDomainCount; iCount++) {
        if (domaindata[iCount]->iStrucIndex == Index)
            domaindata[iCount]->bVisibleParent = bVisible;
    }

}

/*---------------------------------------------------------*/
static void Cn3D_ReSetModVisibleStatus(PVNMO pvnmoThis, Byte bVisible)
{
    PMOD pmodThis = NULL;

    while (pvnmoThis) {
        pmodThis = (PMOD) pvnmoThis->data.ptrvalue;
        pmodThis->bVisible = bVisible;
        pvnmoThis = pvnmoThis->next;
    }
}

/*---------------------------------------------------------*/
static void Cn3D_ReSetMmVisibleStatus(PDNMM pdnmmHead, Byte bVisible)
{
    PMMD pmmdThis = NULL;
    PDNMG pdnmg = NULL;
    PMGD pmgd = NULL;

    for (; pdnmmHead; pdnmmHead = pdnmmHead->next) {
        pmmdThis = pdnmmHead->data.ptrvalue;
        if (pmmdThis) {
            if (pmmdThis->bWhat != (Byte) AM_PROT
                && pmmdThis->bWhat != (Byte) AM_RNA
                && pmmdThis->bWhat != (Byte) AM_DNA)
                continue;

            Cn3D_SetVisible(Cn3D_ColorData.pDDVColorGlobal, pmmdThis, bVisible);

            for (pdnmg = pmmdThis->pdnmgHead; pdnmg != NULL;
                 pdnmg = pdnmg->next) {
                pmgd = (PMGD) pdnmg->data.ptrvalue;
                pmgd->bVisible = bVisible;
            }
        }
    }
}

/*---------------------------------------------------------*/
static void Cn3D_ReSetVisibleStatus(void)
{
    PDNMS pdnmsMaster = NULL, pdnmsSlave = NULL;
    PMSD pmsdMaster = NULL, pmsdSlave = NULL;
    PDNMM pdnmmHead = NULL;
    PMMD pmmdThis = NULL;
    PDNMG pdnmg = NULL;
    PMGD pmgd = NULL;

    PVNMO pvnmoThis = NULL;

    pdnmsMaster = GetSelectedModelstruc();
    if (pdnmsMaster != NULL) {
        pmsdMaster = (PMSD) pdnmsMaster->data.ptrvalue;

        pmsdMaster->bVisible = 1;

        pvnmoThis = pmsdMaster->pvnmoHead;
        Cn3D_ReSetModVisibleStatus(pvnmoThis, 1);

        pdnmmHead = pmsdMaster->pdnmmHead;
        Cn3D_ReSetMmVisibleStatus(pdnmmHead, 1);

        pdnmsSlave = pmsdMaster->pdnmsSlaves;
        while (pdnmsSlave) {
            pmsdSlave = (PMSD) pdnmsSlave->data.ptrvalue;

            pmsdSlave->bVisible = 1;

            pvnmoThis = pmsdSlave->pvnmoHead;
            Cn3D_ReSetModVisibleStatus(pvnmoThis, 1);

            pdnmmHead = pmsdSlave->pdnmmHead;
            Cn3D_ReSetMmVisibleStatus(pdnmmHead, 1);

            pdnmsSlave = pdnmsSlave->next;
        }
    }

}

/* sets values in the show/hide list for structures */
NLM_EXTERN void Cn3D_SetStrucList(void)
{
    PDNMS pdnmsMaster, pdnmsSlave;
    PMSD pmsdMaster, pmsdSlave;
    Int4 iCount = 0;

    Reset(Cn3D_lStruc);
    pdnmsMaster = GetSelectedModelstruc();
    if (pdnmsMaster != NULL) {
        pmsdMaster = (PMSD) pdnmsMaster->data.ptrvalue;
        ListItem(Cn3D_lStruc, pmsdMaster->pcPDBName);
        SetItemStatus(Cn3D_lStruc, iCount + 1, pmsdMaster->bVisible);

        iCount++;

        pdnmsSlave = pmsdMaster->pdnmsSlaves;
        while (pdnmsSlave) {
            pmsdSlave = (PMSD) pdnmsSlave->data.ptrvalue;
            ListItem(Cn3D_lStruc, pmsdSlave->pcPDBName);
            SetItemStatus(Cn3D_lStruc, iCount + 1, pmsdSlave->bVisible);

            iCount++;

            pdnmsSlave = pdnmsSlave->next;
        }
    }
}

/*---------------------------------------------------------*/
void AssignDomainAlignedStatusForStrucSeqs(void)
{
    PDNMS pdnmsMaster = NULL;
    PMSD pmsdMaster = NULL;
    PDNMM pdnmmHead = NULL, pdnmmHead_temp = NULL;
    PMMD pmmdThis = NULL;
    PDNMG pdnmg = NULL;
    PMGD pmgd = NULL;

    Int4 iCount = 0, iCountChain = 0;

    pdnmsMaster = GetSelectedModelstruc();
    if (pdnmsMaster == NULL)
        return;

    pmsdMaster = (PMSD) pdnmsMaster->data.ptrvalue;
    pdnmmHead = pmsdMaster->pdnmmHead;

    for (iCount = 0; iCount < iDomainCount; iCount++) {
        domaindata[iCount]->bAligned = FALSE;
        pdnmmHead_temp = pdnmmHead;

        iCountChain = 0;

        while (pdnmmHead_temp) {
            pmmdThis = pdnmmHead_temp->data.ptrvalue;
            if (pmmdThis == NULL)
                break;
            if (pmmdThis->bWhat != (Byte) AM_PROT
                && pmmdThis->bWhat != (Byte) AM_RNA
                && pmmdThis->bWhat != (Byte) AM_DNA)
                goto setout1;

            if (domaindata[iCount]->iChainIndex == iCountChain) {
                for (pdnmg = pmmdThis->pdnmgHead; pdnmg != NULL;
                     pdnmg = pdnmg->next) {
                    pmgd = (PMGD) pdnmg->data.ptrvalue;
                    if (pmgd->iDomain == domaindata[iCount]->iDomain
                        && pmgd->bReserved) {
                        domaindata[iCount]->bAligned = TRUE;
                        goto setout2;
                    }
                }
            }

          setout1:
            pdnmmHead_temp = pdnmmHead_temp->next;
            iCountChain++;
        }
      setout2:
        continue;
    }

}

/*---------------------------------------------------------*/
void AssignDomainAlignedStatus(void)
{
    Int4 iCount = 0, iCountStruc = 0;
    PDNMS pdnmsMaster = NULL;
    PMSD pmsdMaster = NULL;
    BiostrucAnnotSetPtr pbsaThis = NULL;
    BiostrucFeatureSetPtr pbsfsThis = NULL;
    BiostrucFeaturePtr pbsfThis = NULL;

    Int2 iDomainMaster = 0, iDomainSlave = 0;

    pdnmsMaster = GetSelectedModelstruc();
    if (!pdnmsMaster)
        return;
    else
        pmsdMaster = pdnmsMaster->data.ptrvalue;

    pbsaThis = pmsdMaster->psaStrucAlignment;
    if (pbsaThis == NULL)
        return;
    pbsfsThis = pbsaThis->features;
    iDomainMaster = (Int2) (pbsfsThis->id % 100);

    for (iCount = 0; iCount < iDomainCount; iCount++) {
        pbsfThis = pbsfsThis->features;
        iCountStruc = 0;
        while (pbsfThis) {
            iDomainSlave =
                (Int2) (((pbsfThis->id - pbsfThis->id % 10) / 10) % 100);
            iCountStruc++;

            if (domaindata[iCount]->iStrucIndex == 0) {
                if (
                    (StringNCmp
                     (pbsfThis->name, domaindata[iCount]->pcPDBName,
                      4) == 0)
                    && (pbsfThis->name[4] ==
                        domaindata[iCount]->pcMolName[0])) {
/*            if((Char) pbsfThis->name[5] == '0'){   */
                    if (iDomainMaster == 0) {
                        domaindata[iCount]->bAligned = TRUE;
                    }
                        else if (iDomainMaster ==
                                 domaindata[iCount]->iDomain) {
                        domaindata[iCount]->bAligned = TRUE;
                    }
                }
            } else if (domaindata[iCount]->iStrucIndex == iCountStruc) {
                if (
                    (StringNCmp
                     (pbsfThis->name + 7, domaindata[iCount]->pcPDBName,
                      4) == 0)
                    && (pbsfThis->name[11] ==
                        domaindata[iCount]->pcMolName[0])) {
                    if (iDomainSlave == 0) {
                        domaindata[iCount]->bAligned = TRUE;
                    } else if (iDomainSlave == domaindata[iCount]->iDomain) {
                        domaindata[iCount]->bAligned = TRUE;
                    }
                }
            }

            pbsfThis = pbsfThis->next;
        }
    }

}

/*---------------------------------------------------------*/
void LIBCALL Cn3D_CountDomainProc(void)
{
    Int4 iCount = 0;
    Int4 idom = 0;
    Int4 iCountStruc = 0, iCountChain = 0;

    PDNMS pdnmsMaster = NULL, pdnmsSlave = NULL;
    PMSD pmsdMaster = NULL, pmsdSlave = NULL;
    PDNMM pdnmmHead = NULL;
    PMMD pmmdThis = NULL;
    PDNMG pdnmg = NULL;
    PMGD pmgd = NULL;

    Boolean EncounterZeroDomain = FALSE;

    pdnmsMaster = GetSelectedModelstruc();

    if (pdnmsMaster == NULL)
        return;

    if (pdnmsMaster != NULL) {
        pmsdMaster = (PMSD) pdnmsMaster->data.ptrvalue;
        pdnmmHead = pmsdMaster->pdnmmHead;

        while (pdnmmHead) {
            pmmdThis = pdnmmHead->data.ptrvalue;
            if (pmmdThis) {
                if (pmmdThis->bWhat != (Byte) AM_PROT
                    && pmmdThis->bWhat != (Byte) AM_RNA
                    && pmmdThis->bWhat != (Byte) AM_DNA)
                    goto pre_setout1;

                idom = 0;

                EncounterZeroDomain = FALSE;
                for (pdnmg = pmmdThis->pdnmgHead; pdnmg != NULL;
                     pdnmg = pdnmg->next) {
                    pmgd = (PMGD) pdnmg->data.ptrvalue;
                    if (!EncounterZeroDomain) {
                        if (pmgd->iDomain == 0) {
                            iCount++;
                            EncounterZeroDomain = TRUE;
                        }
                    }
                    if (pmgd->iDomain > idom) {
                        idom = (Int4) pmgd->iDomain;
                        iCount++;
                    }
                }

            }
          pre_setout1:
            pdnmmHead = pdnmmHead->next;
        }

        pdnmsSlave = pmsdMaster->pdnmsSlaves;
        while (pdnmsSlave) {
            pmsdSlave = (PMSD) pdnmsSlave->data.ptrvalue;
            pdnmmHead = pmsdSlave->pdnmmHead;

            while (pdnmmHead) {
                pmmdThis = pdnmmHead->data.ptrvalue;
                if (pmmdThis) {
                    if (pmmdThis->bWhat != (Byte) AM_PROT
                        && pmmdThis->bWhat != (Byte) AM_RNA
                        && pmmdThis->bWhat != (Byte) AM_DNA)
                        goto pre_setout2;

                    idom = 0;

                    EncounterZeroDomain = FALSE;

                    for (pdnmg = pmmdThis->pdnmgHead; pdnmg != NULL;
                         pdnmg = pdnmg->next) {
                        pmgd = (PMGD) pdnmg->data.ptrvalue;
                        if (!EncounterZeroDomain) {
                            if (pmgd->iDomain == 0) {
                                iCount++;
                                EncounterZeroDomain = TRUE;
                            }
                        }
                        pmgd = (PMGD) pdnmg->data.ptrvalue;
                        if (pmgd->iDomain > idom) {
                            idom = (Int4) pmgd->iDomain;
                            iCount++;
                        }
                    }

                }
              pre_setout2:
                pdnmmHead = pdnmmHead->next;
            }

            pdnmsSlave = pdnmsSlave->next;
        }
    }

    iDomainCount = iCount;

    /* to allocate memory for domaindata */

    domaindata =
        (Pointer) MemNew((size_t) (iDomainCount * sizeof(Pointer)));
    for (iCount = 0; iCount < iDomainCount; iCount++) {
        domaindata[iCount] = (DomainInfoPtr) MemNew(sizeof(DomainInfo));
    }

    /* to load pdbname, chain id, domain id into domaindata */

    iCount = 0;
    iCountStruc = 0;
    iCountChain = 0;

    pdnmsMaster = GetSelectedModelstruc();
    if (pdnmsMaster != NULL) {
        pmsdMaster = (PMSD) pdnmsMaster->data.ptrvalue;
        pdnmmHead = pmsdMaster->pdnmmHead;

        while (pdnmmHead) {
            pmmdThis = pdnmmHead->data.ptrvalue;
            if (pmmdThis) {
                if (pmmdThis->bWhat != (Byte) AM_PROT
                    && pmmdThis->bWhat != (Byte) AM_RNA
                    && pmmdThis->bWhat != (Byte) AM_DNA)
                    goto setout1;
                Cn3D_SetVisible(Cn3D_ColorData.pDDVColorGlobal, pmmdThis,
                                TRUE);

                idom = 0;

                EncounterZeroDomain = FALSE;

                for (pdnmg = pmmdThis->pdnmgHead; pdnmg != NULL;
                     pdnmg = pdnmg->next) {
                    pmgd = (PMGD) pdnmg->data.ptrvalue;
                    if (!EncounterZeroDomain) {
                        if (pmgd->iDomain == 0) {
                            StringCpy(domaindata[iCount]->pcPDBName,
                                      pmsdMaster->pcPDBName);
                            StringCpy(domaindata[iCount]->pcMolName,
                                      pmmdThis->pcMolName);
                            domaindata[iCount]->iDomain = 0;
                            domaindata[iCount]->iStrucIndex = iCountStruc;
                            domaindata[iCount]->iChainIndex = iCountChain;
                            domaindata[iCount]->bVisible = TRUE;
                            domaindata[iCount]->bVisibleParent = TRUE;
                            iCount++;
                            EncounterZeroDomain = TRUE;
                        }
                    }
                    if (pmgd->iDomain > idom) {
                        idom = (Int4) pmgd->iDomain;
                        StringCpy(domaindata[iCount]->pcPDBName,
                                  pmsdMaster->pcPDBName);
                        StringCpy(domaindata[iCount]->pcMolName,
                                  pmmdThis->pcMolName);
                        domaindata[iCount]->iDomain = pmgd->iDomain;
                        domaindata[iCount]->iStrucIndex = iCountStruc;
                        domaindata[iCount]->iChainIndex = iCountChain;
                        domaindata[iCount]->bVisible = TRUE;
                        domaindata[iCount]->bVisibleParent = TRUE;

                        iCount++;
                    }
                }
                iCountChain++;
            }
          setout1:
            pdnmmHead = pdnmmHead->next;
        }

        iCountStruc++;
        pdnmsSlave = pmsdMaster->pdnmsSlaves;
        while (pdnmsSlave) {
            pmsdSlave = (PMSD) pdnmsSlave->data.ptrvalue;
            pdnmmHead = pmsdSlave->pdnmmHead;

            while (pdnmmHead) {
                pmmdThis = pdnmmHead->data.ptrvalue;
                if (pmmdThis) {
                    if (pmmdThis->bWhat != (Byte) AM_PROT
                        && pmmdThis->bWhat != (Byte) AM_RNA
                        && pmmdThis->bWhat != (Byte) AM_DNA)
                        goto setout2;

                    idom = 0;

                    EncounterZeroDomain = FALSE;

                    for (pdnmg = pmmdThis->pdnmgHead; pdnmg != NULL;
                         pdnmg = pdnmg->next) {
                        pmgd = (PMGD) pdnmg->data.ptrvalue;
                        if (!EncounterZeroDomain) {
                            if (pmgd->iDomain == 0) {
                                StringCpy(domaindata[iCount]->pcPDBName,
                                          pmsdSlave->pcPDBName);
                                StringCpy(domaindata[iCount]->pcMolName,
                                          pmmdThis->pcMolName);
                                domaindata[iCount]->iDomain = 0;
                                domaindata[iCount]->iStrucIndex =
                                    iCountStruc;
                                domaindata[iCount]->iChainIndex =
                                    iCountChain;
                                domaindata[iCount]->bVisible = TRUE;
                                domaindata[iCount]->bVisibleParent = TRUE;
                                iCount++;
                                EncounterZeroDomain = TRUE;
                            }
                        }
                        if (pmgd->iDomain > idom) {
                            idom = (Int4) pmgd->iDomain;
                            StringCpy(domaindata[iCount]->pcPDBName,
                                      pmsdSlave->pcPDBName);
                            StringCpy(domaindata[iCount]->pcMolName,
                                      pmmdThis->pcMolName);
                            domaindata[iCount]->iDomain = pmgd->iDomain;
                            domaindata[iCount]->iStrucIndex = iCountStruc;
                            domaindata[iCount]->iChainIndex = iCountChain;
                            domaindata[iCount]->bVisible = TRUE;
                            domaindata[iCount]->bVisibleParent = TRUE;

                            iCount++;
                        }
/*             if(pmgd->bReserved && !domaindata[iCount - 1]->bAligned) domaindata[iCount - 1]->bAligned = TRUE;  */
                    }
                    iCountChain++;
                }
              setout2:
                pdnmmHead = pdnmmHead->next;
            }

            iCountStruc++;
            pdnmsSlave = pdnmsSlave->next;
        }
    }

    if (pmsdMaster->iMimeType == NcbiMimeAsn1_strucseqs) {
        AssignDomainAlignedStatusForStrucSeqs();
    } else
        AssignDomainAlignedStatus();
}

/*---------------------------------------------------------*/
static void SetDomainDataItemStatus(LisT l)
{
    Int4 iCount = 0, iCountActive = 0;

    for (iCount = 0; iCount < iDomainCount; iCount++) {
        if (domaindata[iCount]->bVisibleParent) {
            domaindata[iCount]->bVisible =
                GetItemStatus(Cn3D_lFeature, iCountActive + 1);
            iCountActive++;
        }
    }

    return;
}

/*---------------------------------------------------------*/
Int4 iCountItemGet(PMSD pmsdThis, PMMD pmmdThis, PMGD pmgdThis,
                   Int4 iCountStruc)
{
    Int4 iCount = 0, iCountLive = 0;
    Int2 iDomain = 0;

    if (pmgdThis == NULL)
        iDomain = 0;
    else
        iDomain = pmgdThis->iDomain;

    for (iCount = 0; iCount < iDomainCount; iCount++) {
        if (!domaindata[iCount]->bVisibleParent)
            continue;
        if (StringCmp(pmsdThis->pcPDBName, domaindata[iCount]->pcPDBName)
            == 0
            && StringCmp(pmmdThis->pcMolName,
                         domaindata[iCount]->pcMolName) == 0
            && iDomain == domaindata[iCount]->iDomain
            && iCountStruc == domaindata[iCount]->iStrucIndex)
            return (iCountLive + 1);
        iCountLive++;

    }

    return 0;
}

/*---------------------------------------------------------*/
NLM_EXTERN void fnAlignList(LisT l)
/* set the values of the alignment pane */
{
    Int4 iCount = 0;
    PDNMS pdnmsMaster = NULL, pdnmsSlave = NULL;
    PMSD pmsdMaster = NULL, pmsdSlave = NULL;
    BiostrucFeaturePtr pbsfThis = NULL;
    PDNMM pdnmmHead = NULL;
    PMMD pmmdThis = NULL;
    Byte bVisible = 0;
    SeqIdPtr sipThis = NULL;
    Uint2 entityID = 0, itemID = 0, itemtype = 0;

#ifndef _OPENGL
    Cn3D_SaveActiveCam();
#endif

    iCount = 0;
    pdnmsMaster = GetSelectedModelstruc();
    if (pdnmsMaster != NULL) {

        pmsdMaster = (PMSD) pdnmsMaster->data.ptrvalue;

        bVisible = pmsdMaster->bVisible;
        pmsdMaster->bVisible = GetItemStatus(Cn3D_lStruc, iCount + 1);
        SetDomainParentStatus(iCount, pmsdMaster->bVisible);

        pdnmmHead = pmsdMaster->pdnmmHead;
        Cn3D_ReSetMmVisibleStatus(pdnmmHead, pmsdMaster->bVisible);

        if (pmsdMaster->iMimeType != NcbiMimeAsn1_strucseqs) {
            TraverseGraphs(pdnmsMaster, 0, 0, NULL, fnClearMarkedResidues);
            pmsdMaster->bAligned = 0;
        }
        pdnmsSlave = pmsdMaster->pdnmsSlaves;

        if (pmsdMaster->psaStrucAlignment != NULL) {
            pbsfThis = pmsdMaster->psaStrucAlignment->features->features;
            iCount++;
            while (pdnmsSlave) {
                pmsdSlave = (PMSD) pdnmsSlave->data.ptrvalue;

                bVisible = pmsdSlave->bVisible;

                TraverseGraphs(pdnmsSlave, 0, 0, NULL,
                               fnClearMarkedResidues);

                pmsdSlave->bVisible =
                    GetItemStatus(Cn3D_lStruc, iCount + 1);
                SetDomainParentStatus(iCount, pmsdSlave->bVisible);

                if (pmsdMaster->bVisible && pmsdSlave->bVisible) {
                    fnMarkAlignedResidues(pdnmsMaster, pdnmsSlave,
                                          pbsfThis);
                    pmsdMaster->bAligned++;
                }
                pbsfThis = pbsfThis->next;

                if (bVisible != pmsdSlave->bVisible) {
                    pdnmmHead = pmsdSlave->pdnmmHead;
                    Cn3D_ReSetMmVisibleStatus(pdnmmHead,
                                              pmsdSlave->bVisible);
                }

                iCount++;
                pdnmsSlave = pdnmsSlave->next;
            }
        }
    }
#ifndef _OPENGL
    Cn3D_v3d->is_zoomed = TRUE; /* keep the proteins from moving */
#endif
}


/*---------------------------------------------------------*/
static void SynchronizeModVisibleStatus(PFB pfbThis)
{
    PMSD pmsdThis = NULL;
    PMMD pmmdThis = NULL;
    PDNMG pdnmg = NULL;
    PMGD pmgdThis = NULL;

    PVNMO pvnmoThis = NULL;
    PMOD pmodThis = NULL;

    ValNodePtr pvnThis = NULL;

    pmsdThis = ToMSDParent(pfbThis);
    pvnmoThis = pmsdThis->pvnmoHead;

    if (pfbThis->bMe == AM_MGD) {
        while (pvnmoThis) {
            pmodThis = pvnmoThis->data.ptrvalue;
            pmodThis->bVisible = 0;
            pvnThis = pmodThis->pvnContains;
            while (pvnThis) {
                pmgdThis = (PMGD) pvnThis->data.ptrvalue;
                if (GetStatus(Cn3D_bDisplayHighlight))
                    pmodThis->bVisible = (pmgdThis->bVisible && pmgdThis->bHighlighted);
                else
                    pmodThis->bVisible = pmgdThis->bVisible;
                if (pmodThis->bVisible == 1) break;
                pvnThis = pvnThis->next;
            }
            pvnmoThis = pvnmoThis->next;
        }
    }

/* not currently used...
    else if (pfbThis->bMe == AM_MMD) {
        pmmdThis = (PMMD) pfbThis;
        while (pvnmoThis) {
            pmodThis = pvnmoThis->data.ptrvalue;
            if (IsGraphNode((PFB) pmodThis->pvnContains->data.ptrvalue)) {
                if (pmmdThis ==
                    GetParentMol((PFB) pmodThis->pvnContains->data.ptrvalue)) {
                    pmodThis->bVisible =
                        Cn3D_IsVisible(Cn3D_ColorData.pDDVColorGlobal, pmmdThis);
                }
            }
            pvnmoThis = pvnmoThis->next;
        }
    }
*/    
}

/*---------------------------------------------------------*/
void LIBCALL ResetDisplayCtrls(void)
/* set the values of the alignment pane */
{
    Cn3D_ReSetVisibleStatus();
    Cn3D_SetStrucList();

    Cn3D_ListDomainProc();

    SetStatus(Cn3D_bAlignOn, Cn3D_fAlignOn);
    SetStatus(Cn3D_bUnalignOn, Cn3D_fUnalignOn);

    return;
}


/*---------------------------------------------------------*/
GrouP LIBCALL DisplayControls(Nlm_GrouP prnt)
{
    GrouP g, h;
    PrompT ppt1, /*ppt2,*/ ppt3, ppt4;

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

    StaticPrompt(g, "", 0, stdLineHeight, systemFont, 'l');
    StaticPrompt(g, "", 0, stdLineHeight, systemFont, 'l');

    h = HiddenGroup(g, 0, -4, NULL);
    SetGroupSpacing(h, 30, 10);

    ppt1 =
        StaticPrompt(h, "Molecule", 0, stdLineHeight + 5, systemFont, 'c');
    Cn3D_lStruc = MultiList(h, 6, 10, FillFeatureListProc);
    StaticPrompt(h, "", 0, stdLineHeight, systemFont, 'l');
    StaticPrompt(h, "", 0, stdLineHeight, systemFont, 'l');
    AlignObjects(ALIGN_LEFT, (HANDLE) ppt1, (HANDLE) Cn3D_lStruc, NULL);

    ppt3 =
        StaticPrompt(h, "Domain", 0, stdLineHeight + 5, systemFont, 'c');
    Cn3D_bDisplayAlignedDomain =
        CheckBox(h, "Aligned Domain Only", Cn3D_ListAlignedDomainProc);
    Cn3D_lFeature = MultiList(h, 6, 8, Cn3D_SelectDomainProc);
    StaticPrompt(h, "", 0, stdLineHeight, systemFont, 'l');
    AlignObjects(ALIGN_LEFT, (HANDLE) ppt3, (HANDLE) Cn3D_lFeature, NULL);

    ppt4 =
        StaticPrompt(h, "Residue", 0, stdLineHeight + 5, systemFont, 'c');
    Cn3D_bAlignOn = CheckBox(h, "Aligned", NULL);
    Cn3D_bUnalignOn = CheckBox(h, "Unaligned", NULL);
    Cn3D_bDisplayHighlight =
        CheckBox(h, "Show Selected Only", NULL);
    AlignObjects(ALIGN_LEFT, (HANDLE) ppt4,
                 (HANDLE) Cn3D_bDisplayHighlight, NULL);
    SetStatus(Cn3D_bDisplayHighlight, FALSE);
    SetStatus(Cn3D_bAlignOn, Cn3D_fAlignOn);
    SetStatus(Cn3D_bUnalignOn, Cn3D_fUnalignOn);

    Cn3D_altConfIdOverlay = CheckBox(g, 
        "Overlay uncorrelated conformer ensembles", NULL);
    AlignObjects(ALIGN_CENTER, (HANDLE) Cn3D_altConfIdOverlay, (HANDLE) g, NULL);
    SetStatus(Cn3D_altConfIdOverlay, TRUE);

    StaticPrompt(g, "", 0, stdLineHeight + 5, systemFont, 'c');
    Cn3D_bDisplayApply = PushButton(g, "Apply!", Cn3D_DisplayProc);
    AlignObjects(ALIGN_CENTER, (HANDLE) Cn3D_bDisplayApply, (HANDLE) h,
                 NULL);

    ResetDisplayCtrls();

    return g;
}

static void SetResidueVisibilities(PDNMS pdnmsThis, Int4 iCountStruc, SeqAlignPtr salp)
{
    PMSD pmsdThis;
    PDNMM pdnmmHead;
    PMMD pmmdThis = NULL;
    PDNMG pdnmg = NULL;
    PMGD pmgd = NULL;
    PVNMO pvnmoThis = NULL;
    Int4 currentRow = -1, currentAlignLoc = -2;
    Boolean bIsPorNA;

    pmsdThis = (PMSD) pdnmsThis->data.ptrvalue;
    pdnmmHead = pmsdThis->pdnmmHead;

    while (pdnmmHead) {
        pmmdThis = pdnmmHead->data.ptrvalue;
        if (pmmdThis) {
            bIsPorNA = (pmmdThis->bWhat == (Byte) AM_PROT ||
                        pmmdThis->bWhat == (Byte) AM_RNA ||
                        pmmdThis->bWhat == (Byte) AM_DNA);

            if (bIsPorNA)
                Cn3D_SetVisible(Cn3D_ColorData.pDDVColorGlobal, pmmdThis, TRUE);
            else
                pmmdThis->bVisible = TRUE;

            if (bIsPorNA && salp && pmmdThis->pSeqId)
                currentRow = ViewMgr_TRow2VRow(salp, pmmdThis->iTargetRow);
            else
                currentRow = -1;

            for (pdnmg = pmmdThis->pdnmgHead; pdnmg != NULL;
                    pdnmg = pdnmg->next) {
                pmgd = (PMGD) pdnmg->data.ptrvalue;

                /* first check to see if whole structure is visible */
                if (bIsPorNA)
                    pmgd->bVisible = GetItemStatus(Cn3D_lFeature,
                                                   iCountItemGet(pmsdThis,
                                                                 pmmdThis, pmgd,
                                                                 iCountStruc));
                else
                    pmgd->bVisible = TRUE;

                if (pmgd->bVisible && GetStatus(Cn3D_bDisplayHighlight))
                    pmgd->bVisible = pmgd->bHighlighted;

                if (bIsPorNA) {
                    if (salp && currentRow > 0)
                        currentAlignLoc = AlnMgrMapBioseqToSeqAlign(salp, 
                                                                    pdnmg->choice - 1,
                                                                    currentRow,
                                                                    NULL);
                    if ((currentAlignLoc >= 0 && !Cn3D_fAlignOn) ||
                        (currentAlignLoc < 0 && !Cn3D_fUnalignOn))
                        pmgd->bVisible = 0;
                }        
            }
        }

        SynchronizeModVisibleStatus((PFB) pmgd);
        pdnmmHead = pdnmmHead->next;
    }
}

/*---------------------------------------------------------*/
NLM_EXTERN void Cn3D_DisplayProc(ButtoN b)
{
    LisT l = NULL;
    SeqAlignPtr salp = NULL;
    SeqAnnotPtr sanp;
    PDNMS pdnmsMaster = NULL, pdnmsSlave = NULL;
    Int4 iCountStruc = 0;

    Cn3D_fAlignOn = GetStatus(Cn3D_bAlignOn);
    Cn3D_fUnalignOn = GetStatus(Cn3D_bUnalignOn);
    fnAlignList(l);
    SetDomainDataItemStatus(l);

    if (GetStatus(Cn3D_bDisplayHighlight)) {
        Cn3DCheckHighlighted();
        if (Cn3D_NoSingleHL)
            SetStatus(Cn3D_bDisplayHighlight, FALSE);
    }

    /* get pointer to current SeqAlign */
    for (sanp = Cn3D_ColorData.sap; sanp != NULL; sanp = sanp->next) {
        if (sanp->data != NULL) {
            salp = sanp->data;
            break;
        }
    }

    pdnmsMaster = GetSelectedModelstruc();
    if (pdnmsMaster) {
        SetResidueVisibilities(pdnmsMaster, iCountStruc, salp);
        iCountStruc++;

        pdnmsSlave = ((PMSD) pdnmsMaster->data.ptrvalue)->pdnmsSlaves;
        while (pdnmsSlave) {
            SetResidueVisibilities(pdnmsSlave, iCountStruc, salp);
            iCountStruc++;
            pdnmsSlave = pdnmsSlave->next;
        }
    }

#ifndef _OPENGL
    Cn3D_v3d->is_zoomed = TRUE; /* keep the proteins from moving */
#endif
    Cn3dObjMgrGetSelected();
    fnCn3D_RedrawWrapper(FALSE);

    return;

}

/*-------------------------------------------*/
void ClearDomainData(void)
{
    Int4 iCount = 0;

    for (iCount = 0; iCount < iDomainCount; iCount++) {
        MemFree(domaindata[iCount]);
    }

    if (domaindata) MemFree(domaindata);

    domaindata = NULL;
    iDomainCount = 0;
    SetStatus(Cn3D_bDisplayHighlight, FALSE);
    Cn3D_NoSingleHL = FALSE;

    return;
}

