/*   cn3dmatn.c
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
* File Name:  cn3dmatn.c
*
* Author: Yanli Wang
*
* Version Creation Date:   3/26/98
*
* File Description: Functions for cn3d/salsa communication
*
* Modifications:
* $Log: cn3dmatn.c,v $
* Revision 6.93  2000/03/14 16:20:56  lewisg
* bug fix: selection on non-aligned residues
*
* Revision 6.92  2000/02/28 14:47:58  thiessen
* fixed "show selected only" in show/hide
*
* Revision 6.91  2000/01/11 01:16:46  lewisg
* fix color selection in Cn3D, other misc. bugs
*
* Revision 6.90  1999/11/23 16:16:17  vakatov
* Fixed a (dangerous!) cast of the callback function Cn3D_FindMMCB()
*
* Revision 6.89  1999/11/18 00:21:42  lewisg
* draw speedups and selection on mouseup
*
* Revision 6.88  1999/11/15 18:30:08  lewisg
* get rid of extra redraws when selecting
*
* Revision 6.87  1999/11/10 23:19:40  lewisg
* rewrite of selection code for ddv
*
* Revision 6.86  1999/10/29 14:15:28  thiessen
* ran all Cn3D source through GNU Indent to prettify
*
* Revision 6.85  1999/10/05 23:18:20  lewisg
* add ddv and udv to cn3d with memory management
*
* Revision 6.84  1999/09/21 18:09:16  lewisg
* binary search added to color manager, various bug fixes, etc.
*
* Revision 6.83  1999/07/07 15:44:00  ywang
* set Cn3D_ObjMgrOpen FALSE as GatherProcLaunch returns OM_MSG_RET_ERROR or OM_MSG_RET_NOPROC
*
* Revision 6.82  1999/05/27 16:11:21  ywang
* initilize all local variables at defined
*
* Revision 6.81  1999/05/25 22:04:58  ywang
* always check mediadata->length before assigning residue information for mediadata to protect memory from corruption upon weird data
*
* Revision 6.80  1999/05/24 16:55:15  ywang
* work around last letter selection problem
*
* Revision 6.79  1999/05/24 15:16:58  ywang
* work around messaging problem on C-terminal dashes selection
*
* Revision 6.78  1999/05/10 20:24:26  ywang
* initialize iRes Cn3DColorSalsaForStrucSeqs
*
* Revision 6.77  1999/05/04 23:12:47  ywang
* fix selection retaining problem on show/hide
*
* Revision 6.76  1999/04/23 14:25:11  ywang
* fix bug for Cn3DObjMgrGetSelected
*
* Revision 6.75  1999/04/21 21:08:42  ywang
* fix small memory leak
*
* Revision 6.74  1999/04/16 22:21:24  ywang
* update residue aligned status on mediadata upon sequence SHOW/HIDE
*
* Revision 6.73  1999/04/01 17:48:23  ywang
* fix bug for coloring salsa for strucseqs data
*
* Revision 6.72  1999/03/30 22:36:18  ywang
* add functions to color salsa for NcbiMimeAsn1_strucseqs & code reorganization
*
* Revision 6.71  1999/03/22 22:41:14  ywang
* remove argument in MediaObjSelect
*
* Revision 6.70  1999/02/12 15:11:52  ywang
* send color message to salsa when user changes highlight color from cn3d
*
* Revision 6.69  1999/02/11 22:42:39  ywang
* explicitly call ObjMgrDeSelect to DeHighlight
*
* Revision 6.68  1999/02/11 22:40:15  ywang
* rename functions
*
* Revision 6.67  1999/02/11 18:48:15  lewisg
* delete color index functions
*
* Revision 6.66  1999/02/10 23:49:43  lewisg
* use RGB values instead of indexed palette
*
* Revision 6.65  1999/02/10 17:04:21  ywang
* work around (Uint1) max number 255 problem for valnode choice number
*
* Revision 6.64  1999/01/20 22:57:25  ywang
* customize color for secondary structure & rearrange Option menu
*
* Revision 6.63  1999/01/20 18:21:19  ywang
* include salmedia.h due to the move around of MediaInfo from cn3dmsg.h to the new created salmedia.h
*
* Revision 6.62  1999/01/19 23:42:49  ywang
* fix bugs over improving color msg
*
* Revision 6.61  1999/01/19 18:25:26  kans
* send default rgb to ObjMgrSetColor
*
* Revision 6.60  1999/01/19 17:51:06  ywang
* fix bug in Cn3DSendColorMsg on NULL sip
*
* Revision 6.59  1999/01/19 17:31:51  ywang
* switch color message from many to once
*
* Revision 6.58  1998/12/16  22:49:39  ywang
* fix compiling warnings on Win32
*
* Revision 6.57  1998/12/16  19:32:20  ywang
* improve highlight residues function when rerendering
*
* Revision 6.56  1998/10/27  15:55:51  ywang
* add functions for testing color by sequence conservation
*
* Revision 6.55  1998/10/21  15:51:26  ywang
* reset residue color for salsa before cn3d redraws so that residues shown in salsa will become black if they are not shown in cn3d window
*
* Revision 6.54  1998/10/19  20:16:06  ywang
* add function FillSeqinfoForSeqEditViewProcs so that salsa can get color array
*
* Revision 6.53  1998/10/19  17:43:02  kans
* prototype needed for Cn3DSendColorMsg
*
* Revision 6.52  1998/10/16 22:06:09  ywang
* make global color array for sequence display
*
* Revision 6.51  1998/10/07  21:19:50  kans
* ObjMgrAlsoSelect changes (CC)
*
* Revision 6.50  1998/09/23 22:07:32  ywang
* fix file name error
* 
* ===========================================================================  */

#include <vibrant.h>
#include <document.h>
#include <vsm.h>
#include <sequtil.h>            /* for sequence load funcs */
#include <objsub.h>
#include <string.h>
#include <saledit.h>
#include <objmgr.h>
#include <mmdbapi.h>            /* the MMDB-API header */
#include <mmdbdata.h>
#include <cn3dmsg.h>
#include <salmedia.h>
#include <sqnutils.h>
#include <cn3dmain.h>
#include <algorend.h>
#include <cn3dshim.h>
#include <objmime.h>
#include <udviewer.h>           /* udv */
#include <ddvopen.h>            /* ddv */

extern Int1 bColorAlignments[];


static void LIBCALLBACK Cn3D_FindMMCB(PFB pfbThis, Nlm_Int4 iModel,
                                      Nlm_Int4 iIndex,
                                      Pointer ptr)
{
    PDNMM* ModelList = (PDNMM*) ptr;
    PMMD pmmdThis = NULL;

    if (pfbThis == NULL) return;
    if (pfbThis->bMe == (Byte) AM_MMD) { /* is this molecular model data? */
        pmmdThis = (PMMD) pfbThis;
        if (pmmdThis->pSeqId)
            DValNodeAddPointer (ModelList, 0, pmmdThis);
    }
}



PDNMM FindMM(void)
/* find coordinates of the bounding box of a structure */
{
    PDNMM ModelList = NULL;
    PDNMS pdnmsThis = NULL, pdnmsThisSlave = NULL;
    PMSD pmsdThis = NULL;

    pdnmsThis = GetSelectedModelstruc();
    if(pdnmsThis == NULL) return NULL;

    TraverseModels(pdnmsThis, TRAVERSE_MOLECULE, 0, &ModelList, Cn3D_FindMMCB);

    pmsdThis = pdnmsThis->data.ptrvalue;
    if(pmsdThis == NULL) return ModelList;

    for(pdnmsThis = pmsdThis->pdnmsSlaves; pdnmsThis != NULL;
            pdnmsThis = pdnmsThis->next) {
        TraverseModels(pdnmsThis, TRAVERSE_MOLECULE, 0, &ModelList,
                       Cn3D_FindMMCB);
    }
    
    return ModelList;
}


/*----------------------------------------------*/
void DoMediaHL(PMMD pmmdThis, Int4 from, Int4 to, Boolean highlight)
{
    PDNMG pdnmgThis = NULL;

    PMGD pmgdThis = NULL;
    PVNMA pvnmaThis = NULL;
    PMAD pmadThis = NULL;
    PVNAL pvnalThis = NULL;
    PALD paldThis = NULL;

    Byte MainAtom = 0;


    pdnmgThis = pmmdThis->pdnmgHead;
    if (pdnmgThis == NULL)
        return;

    pmgdThis = pdnmgThis->data.ptrvalue;
    if ((pmgdThis->bWhat & (Byte) RES_RNA)
        || (pmgdThis->bWhat & (Byte) RES_DNA)) MainAtom = AM_PALPHA;
    if (pmgdThis->bWhat & (Byte) RES_AA)
        MainAtom = AM_CALPHA;

    while (pdnmgThis) {
        if (pdnmgThis->choice <= to && pdnmgThis->choice >= from) {
            pmgdThis = pdnmgThis->data.ptrvalue;
            if (pmgdThis == NULL)
                goto setout;


            fnPreCHLresidue(pdnmgThis, highlight);
        }
      setout:
        pdnmgThis = pdnmgThis->next;
    }


}

/*----------------------------------------------*/
void MediaHL(SelStructPtr sel, Boolean highlight)
{
    /* media action -- highlight... */
    SeqLocPtr slp = NULL;
    SeqIntPtr sintp = NULL;
    SeqIdPtr sip = NULL;
    Int4 from = 0, to = 0;
    PDNMM ModelList,iList;
    PMMD pmmdThis;
    Bioseq *bsp;

    slp = sel->region;
    if (slp == NULL) return;
    sintp = slp->data.ptrvalue;
    if(sintp == NULL) return;
    sip = sintp->id;

    from = SeqLocStart(slp);
    to = SeqLocStop(slp);

    if(to == LAST_RESIDUE || to == APPEND_RESIDUE ) {
        bsp = BioseqLockById(sip);
        if (bsp) {
            to = bsp->length-1;
            BioseqUnlock(bsp);
        }
    }

    /* residue starts with number 1 in structure, but with number 0 in sequence */
    from++;
    to++;
        
    ModelList = FindMM();
    
    for(iList = ModelList; iList != NULL; iList = iList->next) {
        pmmdThis = iList->data.ptrvalue;
        if(pmmdThis == NULL) continue;
        if (SeqIdForSameBioseq(sip, pmmdThis->pSeqId))
            DoMediaHL(pmmdThis, from, to, highlight);
    }
    DValNodeFree(ModelList);

}

/* selection originating from Cn3D */
void MediaObjSelect(PDNMG pdnmgThis, Boolean highlight)
{
    PMGD pmgdThis = NULL;
    PMMD pmmdThis = NULL;
    SeqLocPtr slp = NULL;

    Uint2 entityID, itemID;
    Int4 from, to;

    pmgdThis = pdnmgThis->data.ptrvalue;
    pmmdThis = GetParentMol((PFB) pmgdThis);  

    if (pmmdThis == NULL)return;

    entityID = BioseqFindEntity(pmmdThis->pSeqId, &itemID);
    if(pmmdThis->pSeqId && entityID != 0) {
        from = pdnmgThis->choice - 1;
        to = pdnmgThis->choice - 1;
        slp = SeqLocIntNew(from, to, 0, pmmdThis->pSeqId);
        if (highlight)
            ObjMgrAlsoSelect(entityID, itemID, OBJ_BIOSEQ, OM_REGION_SEQLOC,
            slp);
        /* now explicitly use ObjMgrDeSelect */
        else
            ObjMgrDeSelect(entityID, itemID, OBJ_BIOSEQ, OM_REGION_SEQLOC,
            slp);
        ObjMgrSendMsg(OM_MSG_MOUSEUP, entityID, itemID, OBJ_BIOSEQ);
    }
    else {
        fnPreCHLresidue(pdnmgThis, highlight);
#ifdef _OPENGL
        Cn3D_Redraw(FALSE);
#else
        RedrawViewer3D(Cn3D_v3d);
#endif

    }

}


/*-----------------------------------------------*/
void LIBCALLBACK Cn3DCheckAndDoHighlight(PFB pfbThis, Int4 iModel,
                                         Int4 iIndex, Pointer ptr)
{
    PMGD pmgdThis = NULL;
    PDNMG pdnmgThis = NULL;
    PMMD pmmdThis = NULL;
    PMSD pmsdThis = NULL;

    pmgdThis = (PMGD) pfbThis;
    if (pmgdThis && pmgdThis->bHighlighted == 1) {
        pdnmgThis = pmgdThis->pdnmgLink;

        fnPreCHLresidue(pdnmgThis, TRUE);
    }

}

/*-----------------------------------------------*/
void Cn3dObjMgrGetSelected(void)
{
#ifndef _OPENGL
    PDNMS pdnmsMaster = NULL, pdnmsSlave = NULL;
    PMSD pmsdMaster = NULL, pmsdSlave = NULL;

    /* replace old function which depends on ObjMgr */
    /* by doing so, highlight for non ObjMgr registered residues will */
    /* also be picked up when do Cn3D_Redraw */

    pdnmsMaster = GetSelectedModelstruc();
    if (pdnmsMaster != NULL) {
        pmsdMaster = pdnmsMaster->data.ptrvalue;

        if (pmsdMaster->bVisible == 1) {
            TraverseGraphs(pdnmsMaster, 0, 0, NULL,
                           Cn3DCheckAndDoHighlight);
        }
        pdnmsSlave = pmsdMaster->pdnmsSlaves;
        while (pdnmsSlave) {
            pmsdSlave = pdnmsSlave->data.ptrvalue;
            if (pmsdSlave->bVisible == 1) {
                TraverseGraphs(pdnmsSlave, 0, 0, NULL,
                               Cn3DCheckAndDoHighlight);
            }
            pdnmsSlave = pdnmsSlave->next;
        }
    }
#endif
}
