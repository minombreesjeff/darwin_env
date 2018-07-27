/*   cn3dmsg.c
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
* File Name:  cn3dmsg.c
*
* Author: Yanli Wang 
*
* Version Creation Date:   3/26/98
*
* File Description: Main functions for building up cn3d/salsa communication
*
* Modifications:
* $Log: cn3dmsg.c,v $
* Revision 6.102  2000/06/16 14:57:03  lewisg
* move entrez calls out of desktop
*
* Revision 6.101  2000/05/19 17:46:57  thiessen
* fix redraw from DDV show/hide bug
*
* Revision 6.100  2000/05/15 23:39:33  lewisg
* shred cblast, add menu items for gapped/ungapped, fix pdbheaders
*
* Revision 6.99  2000/04/27 22:21:57  lewisg
* misc bugs/features
*
* Revision 6.98  2000/04/20 23:27:44  lewisg
* misc bug fixes
*
* Revision 6.97  2000/04/12 19:53:37  lewisg
* sequence saving uses genbank class
*
* Revision 6.96  2000/04/10 21:41:25  lewisg
* move alignment menus into ddv, udv from cn3d
*
* Revision 6.95  2000/04/08 00:37:31  lewisg
* multiple seqentries, NEWSEQ message, etc.
*
* Revision 6.94  2000/04/05 23:11:28  lewisg
* get rid of double redraw for hide row
*
* Revision 6.93  2000/03/18 00:06:00  lewisg
* add blast, new help, new menus
*
* Revision 6.92  2000/03/15 19:32:20  lewisg
* launch only single udv window
*
* Revision 6.91  2000/03/08 21:46:14  lewisg
* cn3d saves viewport, misc bugs
*
* Revision 6.90  2000/03/02 21:11:05  lewisg
* use bandalign for import sequence, make standalone ddv use viewmgr, make dialogs modal, send color update
*
* Revision 6.89  2000/03/01 22:49:41  lewisg
* import bioseq, neatlyindex, get rid of dead code
*
* Revision 6.88  2000/02/19 01:23:59  lewisg
* use ibm, add row tracking code, various bug fixes
*
* Revision 6.87  2000/02/15 22:40:57  lewisg
* add ability to launch udv so that it colors by row, fixes to colormgr, track rows from viewmgr, fix visual c projects
*
* Revision 6.86  2000/02/10 15:51:59  lewisg
* cn3d responds and send correct update messages.  many coloring bug fixes
*
* Revision 6.85  2000/02/07 20:17:37  lewisg
* minor bug fixes, use gui font for win32
*
* Revision 6.84  2000/02/05 01:32:21  lewisg
* add viewmgr, move place freeing is done in ddv, modify visual c++ projects
*
* Revision 6.83  2000/01/18 22:49:16  lewisg
* send OM_MSG_FLUSH to ddv/udv, tweak CPK coloration, misc bugs
*
* Revision 6.82  2000/01/11 01:16:47  lewisg
* fix color selection in Cn3D, other misc. bugs
*
* Revision 6.81  2000/01/08 23:16:26  lewisg
* add missing ifdef
*
* Revision 6.80  2000/01/08 00:47:53  lewisg
* fixes to selection, update, color
*
* Revision 6.79  2000/01/06 01:19:40  kans
* fixed SendUpdate problem found by Mac compiler
*
* Revision 6.78  2000/01/06 00:04:42  lewisg
* selection bug fixes, update message outbound, animation APIs moved to vibrant
*
* Revision 6.77  2000/01/04 15:55:50  lewisg
* don't hang on disconnected network and fix memory leak/hang at exit
*
* Revision 6.76  1999/12/28 23:06:35  lewisg
* udv/cn3d communication
*
* Revision 6.75  1999/12/28 15:08:44  lewisg
* remove remaining mediainfo code
*
* Revision 6.74  1999/12/15 23:17:47  lewisg
* make cn3d launch udv, fix launching of non-autonomous udv, add mouseup message
*
* Revision 6.73  1999/12/11 01:30:35  lewisg
* fix bugs with sharing colors between ddv and cn3d
*
* Revision 6.72  1999/12/03 23:17:23  lewisg
* Patrick's new global update msg, argument passing when launching ddv, experimental editing
*
* Revision 6.71  1999/12/02 20:31:59  lewisg
* put seqentries into bioseqset and fix calling convention in alignmgr.c
*
* Revision 6.70  1999/12/01 19:06:07  thiessen
* added alignmgr.h header
*
* Revision 6.69  1999/12/01 16:15:54  lewisg
* interim checkin to fix blocking memory leak
*
* Revision 6.68  1999/11/24 21:44:19  vakatov
* Minor formal fixes
*
* Revision 6.67  1999/11/24 15:23:19  lewisg
* added color selection dialogs for SS
*
* Revision 6.66  1999/11/22 17:29:54  lewisg
* add back color selection dialogs, fix viewer3d half-selection bug
*
* Revision 6.65  1999/11/22 14:46:44  thiessen
* moved _OPENGL code blocks to only vibrant and ncbicn3d libraries
*
* Revision 6.64  1999/11/18 00:21:42  lewisg
* draw speedups and selection on mouseup
*
* Revision 6.63  1999/11/15 18:30:08  lewisg
* get rid of extra redraws when selecting
*
* Revision 6.62  1999/11/12 16:06:34  lewisg
* fix sequentry to valnode conversion
*
* Revision 6.61  1999/11/10 23:19:41  lewisg
* rewrite of selection code for ddv
*
* Revision 6.60  1999/11/03 18:15:10  kans
* added prototypes needed by other files
*
* Revision 6.59  1999/11/02 23:06:07  lewisg
* fix cn3d to launch correctly if there is no seqentry associated with bioseq
*
* Revision 6.58  1999/11/01 22:10:27  lewisg
* add ability to call color functions by type, and add this to cn3d
*
* Revision 6.57  1999/10/29 14:15:29  thiessen
* ran all Cn3D source through GNU Indent to prettify
*
* Revision 6.56  1999/10/15 20:56:39  lewisg
* append DDV_ColorGlobal as userdata.  free memory when cn3d terminates.
*
* Revision 6.55  1999/10/06 00:01:25  kans
* minor changes
*
* Revision 6.54  1999/10/05 23:18:20  lewisg
* add ddv and udv to cn3d with memory management
*
* Revision 6.53  1999/09/21 18:09:15  lewisg
* binary search added to color manager, various bug fixes, etc.
*
* Revision 6.52  1999/08/04 21:18:01  lewisg
* modularized open operations to allow sequin to launch cn3d
*
* Revision 6.51  1999/07/09 20:50:55  ywang
* set highlight color for salsa in Cn3dObjRegiste
*
* Revision 6.50  1999/07/08 20:41:02  ywang
* ignore message on features
*
* Revision 6.49  1999/07/07 20:45:36  ywang
* clear domaindata, mediadata, special feature before reading in new data in cn3d
*
* Revision 6.48  1999/06/14 17:38:01  ywang
* comment out 'BioseqLockById failed in MediaLaunch'
*
* Revision 6.47  1999/05/27 16:11:21  ywang
* initilize all local variables at defined
*
* Revision 6.46  1999/05/27 14:13:09  ywang
* change comment in ErrPost
*
* Revision 6.45  1999/05/25 22:04:57  ywang
* always check mediadata->length before assigning residue information for mediadata to protect memory from corruption upon weird data
*
* Revision 6.44  1999/04/22 21:10:07  ywang
* fix breaks
*
* Revision 6.43  1999/03/30 22:36:19  ywang
* add functions to color salsa for NcbiMimeAsn1_strucseqs & code reorganization
*
* Revision 6.42  1999/03/03 23:17:22  lewisg
* one master struct at a time, list slaves in structure info, bug fixes
*
* Revision 6.41  1999/01/20 18:21:20  ywang
* include salmedia.h due to the move around of MediaInfo from cn3dmsg.h to the new created salmedia.h
*
* Revision 6.40  1999/01/14 19:07:17  kans
* network availability is configurable
*
* Revision 6.39  1999/01/14 14:15:16  kans
* do not call EntrezBioseqFetchEnable/Disable, calls GatherSpecificProcLaunch for Seq-Struc Communication
*
* Revision 6.38  1998/12/16 20:13:33  ywang
* remove return warning for FillSeqinfoForSeqEditViewProcs
*
* Revision 6.37  1998/12/09  20:41:18  ywang
* get rid of GetGIForSeqId
*
* Revision 6.36  1998/10/27  15:55:52  ywang
* add functions for testing color by sequence conservation
*
* Revision 6.35  1998/10/21  21:16:07  ywang
* put highlight RGB in the application property structure
*
* Revision 6.34  1998/10/19  20:16:05  ywang
* add function FillSeqinfoForSeqEditViewProcs so that salsa can get color array
*
* Revision 6.33  1998/10/16  22:06:08  ywang
* make global color array for sequence display
*
* Revision 6.32  1998/09/23  22:08:42  ywang
* to record checkin log
*
* ===========================================================================  */

#include <vibrant.h>
#include <document.h>
#include <vsm.h>
#include <sequtil.h>            /* for sequence load funcs */
#include <objsub.h>
#include <string.h>
#include <objmgr.h>
#include <cn3dmsg.h>
#include <mmdbapi.h>
#include <accentr.h>
#include <lsqfetch.h>
#include <cn3dmain.h>
#include <samutil.h>
#include <sqnutils.h>
#include <algorend.h>
#include <cn3dshim.h>
#include <alignmgr.h>
#include <cn3dopen.h>
#include <cn3dmodl.h>


void Cn3D_RegisterColor(void)
{
    DDV_ColorQueue *pColorQueue;
    Int4 i;

    /* add the regular coloring functions for residues */
    for (i = 0; i < CN3DFUNCNUM; i++) {
        pColorQueue = DDV_NewColorQueue(Cn3D_ColorList[i].Function, 
            Cn3D_ColorList[i].Name, DDV_PRILO, FALSE, Cn3D_ColorData.sepprocid);
        DDV_AddColorFunc(Cn3D_ColorData.pDDVColorGlobal, pColorQueue);
    }

    /* add the user defined colors as an override */
    pColorQueue = DDV_NewColorQueue(Cn3D_ColorSpecial, "Cn3D Special",
                                    DDV_PRIHI, TRUE, Cn3D_ColorData.sepprocid);
    DDV_AddColorFunc(Cn3D_ColorData.pDDVColorGlobal, pColorQueue);
}


/*****************************************************************************

Function: Cn3D_LaunchSeqEntry()

Purpose: Launch the Bioseq viewer on all Bioseqs contained in a SeqEntry.
  
Parameters: pvnsep, the valnode list of SeqEntries

*****************************************************************************/

static void Cn3D_SetRect(RecT *Rect)
{
    RecT cn3drc;
    
    if(Rect == NULL) return;
    ObjectRect(Cn3D_ColorData.Cn3D_w, &cn3drc);
    Rect->top = cn3drc.bottom + 50;
    Rect->bottom = screenRect.bottom - 100;
    if(Rect->bottom - Rect->top < 200) Rect->top =
        Rect->bottom - 200;
    if(Rect->bottom - Rect->top > 300) Rect->top =
        Rect->bottom - 300;
    Rect->left = cn3drc.left;
    Rect->right = screenRect.right - 100;
    if(Rect->right - Rect->left < 400) Rect->left =
        Rect->right - 400;
}

void Cn3D_LaunchSeqEntry(ValNode * pvnsep)
{
    Uint2 entityID;
    SAM_ViewGlobal vg;
    
    MemSet(&vg, 0, sizeof(SAM_ViewGlobal));
    Cn3D_SetRect((RecT *)&vg.Rect);
    vg.MasterViewer = SAMVIEWCN3D;
    vg.FetchProc = NULL;
    vg.NetStartProc = Cn3D_StartNet;
    vg.BlastFile = (void *)Cn3D_ImportBioseqFile;
    vg.BlastFileGap = (void *)Cn3D_ImportBioseqFileGap;
    vg.BlastNet = (void *)Cn3D_ImportBioseq;
    vg.BlastNetGap = (void *)Cn3D_ImportBioseqGap;
#if 0
    vg.BlastMany = (void *)Cn3D_BlastDlg;
#endif
    SetAppProperty(SAM_ViewString, &vg);
    entityID = ObjMgrGetEntityIDForPointer(pvnsep);
    
    GatherSpecificProcLaunch(0, "OneD-Viewer SE", OMPROC_VIEW,
        FALSE, entityID, 0, OBJ_BIOSEQSET);
    RemoveAppProperty(SAM_ViewString);
    
}

/*****************************************************************************

Function: Cn3D_RegisterSeqEntry()

Purpose: Adds a message func to the SeqEntry and makes it
         OM_OPT_FREE_IF_NO_VIEW
  
Parameters: pvnsep, a valnode list of SeqEntries

*****************************************************************************/

void Cn3D_RegisterSeqEntry(ValNode * pvnsep)
{
    Int2 entityID;
    SeqEntry *sep = NULL;
    BioseqSet *bssp;
    
    if(pvnsep == NULL) return;
    if(pvnsep->choice == 2) 
        if(((BioseqSet *)pvnsep->data.ptrvalue)->_class == 
            BioseqseqSet_class_genbank) sep = pvnsep;

    if(!sep) {
        bssp = BioseqSetNew ();
        if (bssp == NULL) return;
        bssp->_class = BioseqseqSet_class_genbank;
        bssp->seq_set = pvnsep;
        
        sep = SeqEntryNew ();
        if (sep == NULL) return;
        sep->choice = 2;  /* Bioseq-set */
        sep->data.ptrvalue = (Pointer) bssp;
        sep->next = NULL;
    }
    
    SeqMgrLinkSeqEntry(sep, 0, NULL);
    
    entityID = ObjMgrGetEntityIDForPointer(sep);
    Cn3D_ColorData.pvnsep = sep;
    
    GatherSpecificProcLaunch(0, "Seq-Struc Communication", OMPROC_VIEW,
        FALSE, entityID, 0, OBJ_BIOSEQSET);
    SAM_MakeViewerFree((void *) sep);
    if(Cn3D_ColorData.pDDVColorGlobal == NULL) {
        Cn3D_ColorData.pDDVColorGlobal = DDV_GetColorGlobalEx((void *) sep);
        DDV_LoadSSColor(Cn3D_ColorData.pDDVColorGlobal, "CN3D");
    }
    Cn3D_RegisterColor();
}


/*****************************************************************************

Function: Cn3D_LaunchSeqAnnot()

Purpose: Launch the SeqAlign viewer on all SeqAligns contained in a SeqAnnot.
  
Parameters: sap, the SeqAnnot

*****************************************************************************/

void Cn3D_LaunchSeqAnnot(SeqAnnot * sap)
{
    Int2 entityID, itemID;
    SeqAlign *salp;
    SAM_ViewGlobal vg;
    RecT cn3drc;

    MemSet(&vg, 0, sizeof(SAM_ViewGlobal));

    for (; sap != NULL; sap = sap->next) {
        if (sap->data == NULL)
            continue;
        salp = sap->data;
        entityID = ObjMgrGetEntityIDForPointer(salp);
        
        itemID =
            GetItemIDGivenPointer(entityID, OBJ_SEQALIGN, (void *) salp);

        ObjectRect(Cn3D_ColorData.Cn3D_w, &cn3drc);
        Cn3D_SetRect((RecT *)&vg.Rect);
        vg.MasterViewer = SAMVIEWCN3D;
        vg.FetchProc = NULL;
        vg.NetStartProc = Cn3D_StartNet;
        vg.BlastFile = (void *)Cn3D_ImportBioseqFile;
        vg.BlastFileGap = (void *)Cn3D_ImportBioseqFileGap;
        vg.BlastNet = (void *)Cn3D_ImportBioseq;
        vg.BlastNetGap = (void *)Cn3D_ImportBioseqGap;
#if 0
        vg.BlastMany = (void *)Cn3D_BlastDlg;
#endif 
        SetAppProperty(SAM_ViewString, &vg);
        GatherSpecificProcLaunch(0, "DDV", OMPROC_VIEW,
            FALSE, entityID, itemID, OBJ_SEQALIGN);

        RemoveAppProperty(SAM_ViewString);
    }
}

/*****************************************************************************

Function: Cn3D_CountRows()

Purpose: figures out the number of rows in the SeqAlign
  
Parameters: salp, the SeqAlign corresponding to the vast alignment.

*****************************************************************************/

NLM_EXTERN Int4 Cn3D_CountRows(SeqAlign *salp)
{
    SeqAlign *salptmp;
    Int4 retval;
    
    if(salp == NULL) return 0;
    
    salptmp = SeqAlignListDup(salp);
    AlnMgrNeatlyIndex(salptmp);
    AlnMgrMakeMultByIntersectOnMaster(salptmp, TRUE);
    retval = AlnMgrGetNumRows(salptmp);
    SeqAlignListFree(salptmp);
    return retval;
}


/*****************************************************************************

Function: Cn3D_RegisterSeqAnnot()

Purpose: Adds a message func to the SeqAnnot and makes it
         OM_OPT_FREE_IF_NO_VIEW
  
Parameters: sap, the SeqAnnot pointer

*****************************************************************************/

void Cn3D_RegisterSeqAnnot(SeqAnnot *sap, Boolean Neat, Boolean IBM)
{
    Int2 entityID;
    SeqAlign *salp;

    Cn3D_ColorData.sap = sap;
    
    /* index the seq aligns */
    for (; sap != NULL; sap = sap->next) {
        if (sap->data == NULL) continue;
        salp = sap->data;
        entityID = ObjMgrRegister(OBJ_SEQALIGN, (void *) salp);
        GatherSpecificProcLaunch(0, "Cn3D SeqAnnot", OMPROC_VIEW,
                             FALSE, entityID, 0, OBJ_SEQALIGN);
        SAM_MakeViewerFree((void *) salp);
        if(Cn3D_ColorData.pDDVColorGlobal == NULL) {
            Cn3D_ColorData.pDDVColorGlobal = DDV_GetColorGlobalEx((void *) salp);
            DDV_LoadSSColor(Cn3D_ColorData.pDDVColorGlobal, "CN3D");
        }
        else DDV_GetColorGlobalEx((void *) salp);

/*        if(!AlnMgrIndexSeqAlign(salp)) {
            ErrPostEx(SEV_ERROR, 0, 0, "Alignment not in correct format");
            return;
        } */
        ViewMgr_RemoveSA(salp);  /* make sure it isn't there */
        ViewMgr_Attach(salp, Neat, IBM, 0, 0);
    }
}


/*****************************************************************************

Function: Cn3D_SendUpdate()

Purpose: Sends an update message to everyone about everything.
  
*****************************************************************************/

NLM_EXTERN void Cn3D_SendUpdate()
{
    DDVUpdateMSG  dump;
    Uint2 entityID, itemID;
    SeqAlign *salp;
    SeqAnnot *sap;
    ValNode *pvnSips, *pvnSipsHold, *pvnsep;
    SeqId *sip;

    dump.data = NULL;
	dump.type = UPDATE_TYPE_COLOR;
    
    for (sap = Cn3D_ColorData.sap; sap != NULL; sap = sap->next) {        
        if (sap->data == NULL) continue;
        salp = sap->data;
        entityID = ObjMgrGetEntityIDForPointer(salp);
        itemID =
            GetItemIDGivenPointer(entityID, OBJ_SEQALIGN, (void *) salp);
               
        ObjMgrSendProcMsg(OM_MSG_UPDATE, entityID,itemID,
            OBJ_SEQALIGN,0,0,(Pointer)&dump);
    }
    
    for(pvnsep = Cn3D_ColorData.pvnsep; pvnsep != NULL;
            pvnsep = pvnsep->next) {
        pvnSips = SAM_ExtractSips(Cn3D_ColorData.pvnsep);
        if (pvnSips != NULL) {
            pvnSipsHold = pvnSips;
            
            for (; pvnSips != NULL; pvnSips = pvnSips->next) {
                sip = (SeqId *) pvnSips->data.ptrvalue;
                if (sip == NULL) continue;
                entityID = BioseqFindEntity(sip, &itemID);

                ObjMgrSendProcMsg(OM_MSG_UPDATE, entityID,itemID,
                    OBJ_BIOSEQ,0,0,(Pointer)&dump);
            }
            ValNodeFree(pvnSipsHold);            
        }
    }
}


/* check to see if there is any visible row in the pdmnsThis */
static Boolean Cn3D_IsVisibleRow(PDNMS pdnmsThis)
{
    PMSD pmsdThis;
    PDNMM pdvn;
    PMMD pmmdThis;
    Boolean retval = FALSE;

    if(!Cn3D_ColorData.sap || pdnmsThis == NULL) return FALSE;
    pmsdThis = (PMSD) pdnmsThis->data.ptrvalue;
    for(pdvn = pmsdThis->pdnmmHead; pdvn != NULL; pdvn = pdvn->next){
        pmmdThis = (PMMD)pdvn->data.ptrvalue;
        if(pmmdThis) {
            if(ViewMgr_TRow2VRow((SeqAlign *)Cn3D_ColorData.sap->data,
                pmmdThis->iTargetRow) > 0) retval = TRUE;
        }
    }
    pmsdThis->bVisible = retval;
    return retval;
}


/*-----------------------------------------*/
static Int2 LIBCALLBACK SeqStrucMediaMsgFunc(OMMsgStructPtr ommsp)
{
    OMUserDataPtr omudp = NULL;
    DDVUpdateMSGPtr dump = NULL;
    SelStructPtr sel = NULL;
    PDNMS pdnmsThis;
    PMSD pmsdThis;
    omudp = (OMUserDataPtr) (ommsp->omuserdata);

    switch (ommsp->message) {
    case OM_MSG_UPDATE:
			
        dump = (DDVUpdateMSGPtr)(ommsp->procmsgdata);
			
        /* ignore generic UPDATE msg*/
        if (!dump) break;

        switch(dump->type){
        case UPDATE_TYPE_RECOLOR:
            pdnmsThis = GetSelectedModelstruc();
            if (pdnmsThis) {
                Cn3D_IsVisibleRow(pdnmsThis);
                MakeStrucPalette(pdnmsThis);
                pmsdThis = (PMSD)pdnmsThis->data.ptrvalue;
                for (pdnmsThis = pmsdThis->pdnmsSlaves; pdnmsThis != NULL;
                        pdnmsThis = pdnmsThis->next) {
                    Cn3D_IsVisibleRow(pdnmsThis);
                    MakeStrucPalette(pdnmsThis);
                }
            }
            Cn3D_SetStrucList();
            Cn3D_ListDomainProc();
            break;
        case UPDATE_TYPE_VIEWMGR:
#ifdef _OPENGL
            if(OGL_IsPlaying(Cn3D_ColorData.OGL_Data))
                OGL_StopPlaying(Cn3D_ColorData.OGL_Data);
            OGL_AllLayerOnProc(Cn3D_ColorData.OGL_Data);
#else /* _OPENGL */
            if(IsPlaying3D()) StopPlaying3D();
            Nlm_AllLayerSet3D(Cn3D_v3d, TRUE);
#endif /* else _OPENGL */
            Cn3D_DisplayProc(NULL);
            /*Cn3D_Redraw(FALSE);*/

            break;
        default:
            break;
        }
        break;

    case OM_MSG_SELECT:

        if (ommsp->itemtype == OBJ_SEQALIGN)
            return OM_MSG_RET_OK;
        if (ommsp->itemtype == OBJ_SEQFEAT)
            return OM_MSG_RET_OK;

        sel = ObjMgrGetSelected();
        while (sel != NULL) {
            MediaHL(sel, TRUE); /* highlight in cn3d */
            sel = sel->next;
        }

        break;

    case OM_MSG_DESELECT:

        if (ommsp->itemtype == OBJ_SEQALIGN)
            return OM_MSG_RET_OK;
        if (ommsp->itemtype == OBJ_SEQFEAT)
            return OM_MSG_RET_OK;

        sel = (SelStructPtr) MemNew(sizeof(SelStruct));
        sel->entityID = ommsp->entityID;
        sel->itemtype = ommsp->itemtype;
        sel->itemID = ommsp->itemID;
        sel->region = ommsp->region;
        sel->regiontype = ommsp->regiontype;
        if (sel) MediaHL(sel, FALSE); /* highlight in cn3d */

        break;

    case OM_MSG_MOUSEUP:
        if (ommsp->itemtype == OBJ_SEQFEAT)
            return OM_MSG_RET_OK;

#ifdef _OPENGL
        if(OGL_IsPlaying(Cn3D_ColorData.OGL_Data)) {
            OGL_StopPlaying(Cn3D_ColorData.OGL_Data);
            OGL_AllLayerOnProc(Cn3D_ColorData.OGL_Data);
        }
#else /* _OPENGL */
        if(IsPlaying3D()) {
            StopPlaying3D();
            Nlm_AllLayerSet3D(Cn3D_v3d, TRUE);
        }
#endif /* else _OPENGL */
        Cn3D_Redraw(FALSE);
        break;
    }

    return OM_MSG_RET_OK;

}

/*-----------------------------------------*/
extern Int2 LIBCALLBACK SeqStrucMediaFunc(Pointer data)
{
    OMProcControlPtr ompcp = NULL;
    OMUserDataPtr omudp = NULL;

    ompcp = (OMProcControlPtr) data;

    if (ompcp == NULL) {
        Message(MSG_ERROR, "Data NULL [1]");
        return OM_MSG_RET_ERROR;
    }
        
    if( ompcp->proc == NULL) {
        Message(MSG_ERROR, "Data NULL [2]");
        return OM_MSG_RET_ERROR;
    }

    switch (ompcp->input_itemtype) {
    case OBJ_BIOSEQSET:
    case OBJ_BIOSEQ:
        break;
    default:
        return OM_MSG_RET_ERROR;
    }

    if (ompcp->input_data == NULL) {
        Message(MSG_ERROR, "Data NULL [3]");
        return OM_MSG_RET_ERROR;
    }

    Cn3D_ColorData.sepprocid = ompcp->proc->procid;

    omudp =
        ObjMgrAddUserData(ompcp->input_entityID, ompcp->proc->procid,
                          OMPROC_VIEW, Cn3D_ColorData.userkey);
    if (omudp != NULL) {
        omudp->messagefunc = SeqStrucMediaMsgFunc;
    }
    return OM_MSG_RET_OK;

}

/*****************************************************************************

Function: Cn3D_AnnotEditFunc()

Purpose: The object manager callback to register a SeqAnnot.
  
Parameters: data, the OMProcControlPtr.

Returns: OM_MSG_RET_*

*****************************************************************************/

extern Int2 LIBCALLBACK Cn3D_AnnotEditFunc(Pointer data)
{
    OMProcControlPtr ompcp = NULL;
    OMUserDataPtr omudp = NULL;

    ompcp = (OMProcControlPtr) data;
    if (ompcp == NULL) {
        Message(MSG_ERROR, "Data NULL [1]");
        return OM_MSG_RET_ERROR;
    }
        
    if( ompcp->proc == NULL) {
        Message(MSG_ERROR, "Data NULL [2]");
        return OM_MSG_RET_ERROR;
    }

    switch (ompcp->input_itemtype) {
    case OBJ_SEQANNOT:
    case OBJ_SEQALIGN:
        break;
    case 0:
        return OM_MSG_RET_ERROR;
    default:
        return OM_MSG_RET_ERROR;
    }

    if (ompcp->input_data == NULL) {
        Message(MSG_ERROR, "Data NULL [3]");
        return OM_MSG_RET_ERROR;
    }
    Cn3D_ColorData.sapprocid = ompcp->proc->procid;
    omudp = ObjMgrAddUserData(ompcp->input_entityID, ompcp->proc->procid,
                              OMPROC_EDIT, Cn3D_ColorData.userkey);
    if (omudp != NULL) {
        omudp->messagefunc = SeqStrucMediaMsgFunc;
    }

    return OM_MSG_RET_OK;
}
