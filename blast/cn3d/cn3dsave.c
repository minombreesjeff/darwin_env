/*   cn3dsave.c
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
* File Name:  cn3dsave.c
*
* Author:  Christopher Hogue, Yanli Wang, Lewis Geer
*
* First Version Creation Date:   1/31/96
*
* $Revision: 6.32 $
*
* File Description: Cn3d file saving routines 
*                   
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
* $Log: cn3dsave.c,v $
* Revision 6.32  2001/04/27 20:08:02  juran
* Heed warnings.
*
* Revision 6.31  2000/07/21 18:55:14  thiessen
* allow dynamic slave->master transformation
*
* Revision 6.30  2000/04/19 17:56:48  thiessen
* added background color in OpenGL
*
* Revision 6.29  2000/04/17 15:54:26  thiessen
* add cylinder arrows; misc graphics tweaks
*
* Revision 6.28  2000/04/12 19:53:37  lewisg
* sequence saving uses genbank class
*
* Revision 6.27  2000/04/08 00:37:30  lewisg
* multiple seqentries, NEWSEQ message, etc.
*
* Revision 6.26  2000/03/29 23:38:06  lewisg
* hide/show, fixes to saving and opening
*
* Revision 6.25  2000/03/28 19:28:13  lewisg
* prep new imported sa
*
* Revision 6.24  2000/03/27 22:15:05  lewisg
* add show/hide row dialog
*
* Revision 6.23  2000/03/23 14:53:04  thiessen
* fix bug in viewer3d camera; added conservation color algorithm to ARS
*
* Revision 6.22  2000/03/22 23:17:50  thiessen
* added ability to save ARS in ASN1
*
* Revision 6.21  2000/03/21 21:41:03  lewisg
* enable saving of full cdd's, visual c++ updates
*
* Revision 6.20  2000/03/17 22:48:59  thiessen
* fix for multi-chain / multi-model features ; added feature-move ; misc bug fixes
*
* Revision 6.19  2000/03/08 21:46:14  lewisg
* cn3d saves viewport, misc bugs
*
* Revision 6.18  2000/02/19 21:25:57  thiessen
* split of cn3dmodl into cn3dmodl and cn3dstyl
*
* Revision 6.17  1999/11/22 14:46:44  thiessen
* moved _OPENGL code blocks to only vibrant and ncbicn3d libraries
*
* Revision 6.16  1999/11/11 01:18:40  kans
* cannot valnodeaddpointer to a seqannot - reverting to old instructions for now
*
* Revision 6.15  1999/11/10 23:19:42  lewisg
* rewrite of selection code for ddv
*
* Revision 6.14  1999/10/29 14:15:30  thiessen
* ran all Cn3D source through GNU Indent to prettify
*
* Revision 6.13  1999/10/18 15:32:50  lewisg
* move ClearSequences() to cn3dshim.c
*
* Revision 6.12  1999/10/05 23:18:24  lewisg
* add ddv and udv to cn3d with memory management
*
* Revision 6.11  1999/08/04 21:18:01  lewisg
* modularized open operations to allow sequin to launch cn3d
*
* Revision 6.10  1999/07/01 22:14:47  ywang
* back to previous version to use EntrezSeqEntryGet to get sequences for additional loading, before this free existant sequences
*
* Revision 6.9  1999/07/01 21:44:31  ywang
* work around EntrezSeqEntryGet core dump on redunant sequence loading and free existant sequences before additional loading
*
* Revision 6.8  1999/07/01 14:09:05  ywang
* *** empty log message ***
*
* Revision 6.7  1999/07/01 14:07:25  ywang
* cn3dwin.c
*
* Revision 6.6  1999/06/03 21:31:48  ywang
* fix bug for saving structures--get MS coordinates back for the current run after creating ASN.1 for saving
*
* Revision 6.5  1999/03/19 19:20:11  kans
* prototype needed for Cn3DAddUserDefinedFeature
*
* Revision 6.4  1999/03/18 22:28:58  ywang
* add functions for saveout+readin+index user defined features
*
* Revision 6.3  1999/03/18 21:04:28  lewisg
* reverse transform for saving
*
* Revision 6.2  1999/02/26 15:44:07  kans
* boolean initialized to false, not null
*
* Revision 6.1  1999/02/25 23:13:41  ywang
* make cn3d export the entire input mime data
*
 * Revision 6.0  1997/08/25  18:13:42  madden
 * Revision changed to 6.0
 *
* Revision 5.1  1996/07/22 00:28:11  hogue
* Fixed binary ASN.1 saving.
*
 * Revision 5.0  1996/05/28  14:05:44  ostell
 * Set to revision 5.0
 *
 * Revision 1.3  1996/04/18  17:02:20  hogue
 * removed temporary reference to mmdbapi4.h
 *
 * Revision 1.2  1996/02/02  19:40:41  hogue
 * Initial Revision
 *
*
* ==========================================================================
*/

#include <vibrant.h>
#include <mmdbapi.h>
#include <mmdbapi1.h>
#include <cn3dmain.h>
#include <algorend.h>
#include <cn3dsave.h>
#include <cn3dmsel.h>
#include <cn3dmsg.h>
#include <asnmime.h>
#include <objmime.h>
#include <cn3dopen.h>
#include <cn3dshim.h>
#include <cn3dmodl.h>
#include <cn3dstyl.h>
#include <alignmgr.h>


static void Cn3D_AddGlobalRenderSettingsToBiostruc(PDNMS pdnms);
static void Cn3D_RemoveGlobalRenderSettingsFromBiostruc(PDNMS pdnms);

static Boolean Cn3D_Save_InUse = FALSE;

static WindoW Cn3D_wAsnSave;
static TexT Cn3D_tAsnSave;
static ButtoN Cn3D_bAsnBrowse;
static ButtoN Cn3D_bAsnOk;
static GrouP Cn3D_gBinAscii;
static ButtoN Cn3D_bFeatOn;


static void Cn3D_AsnEnableProc(TexT t)
{
    Char str[32];
    GetTitle(Cn3D_tAsnSave, str, sizeof(str));
    if (StringLen(str) == 0) {
        Disable(Cn3D_bAsnOk);
    } else {
        Enable(Cn3D_bAsnOk);
    }
    return;
}

#ifdef _OPENGL


static void Cn3D_AddMatrix(void)
{
    PMSD pmsdThis;
    PDNMS pdnmsMaster;
    BiostrucFeatureSetPtr bsfsp = NULL, bsfsp_new = NULL;
    BiostrucFeature *bsfp;
    Camera *camera;

    pdnmsMaster = GetSelectedModelstruc();
    if(pdnmsMaster == NULL) return;
    pmsdThis = pdnmsMaster->data.ptrvalue;
    bsfp = Cn3D_FindFeature(pmsdThis->pbsBS->features,
        Feature_type_camera, 1);

    if(bsfp == NULL) {
        bsfsp = Cn3D_AddFeatureSet(&pmsdThis->pbsBS->features);
        if(bsfsp == NULL) return;
        bsfp = BiostrucFeatureNew();
        if(bsfp == NULL) return;
        bsfsp->features = bsfp;
        bsfp->type = Feature_type_camera;
        camera = CameraNew();
        camera->modelview = GLMatrixNew();
        ValNodeAddPointer(&bsfp->Property_property,
            Property_property_camera, camera);
    }
    else camera = (Camera *)bsfp->Property_property->data.ptrvalue;

    Cn3D_Matrix2Asn(camera->modelview, Cn3D_ColorData.OGL_Data->ModelMatrix);
    camera->scale = (Int4)VIEWSCALE;
    camera->distance = (Int4)(Cn3D_ColorData.OGL_Data->CameraDistance * camera->scale);
    camera->angle = (Int4)(Cn3D_ColorData.OGL_Data->CameraAngle * camera->scale);
    camera->x = (Int4)(Cn3D_ColorData.OGL_Data->CameraDirection[0] * camera->scale);
    camera->y = (Int4)(Cn3D_ColorData.OGL_Data->CameraDirection[1] * camera->scale);
}

#endif /* _OPENGL */


static void Cn3D_AsnBrowseProc(ButtoN b)
{
    Char dfault[32];
    Char path[256];

    path[0] = '\0';
    dfault[0] = '\0';
    if (GetOutputFileName(path, sizeof(path), dfault)) {
        SetTitle(Cn3D_tAsnSave, path);
        Cn3D_AsnEnableProc(NULL);
    }
    return;
}

static ValNode *Cn3D_GetSubSet(ValNode *pvnsep, SeqAnnot *sap)
{
    BioseqSet *bssp;
    SeqAlign *salp = NULL;
    Int4 i, n;
    SeqId *sip = NULL, *isip;
    SeqEntry *sep = NULL, *isep;
    ValNode *pvn, *pvnPrev;

    if(sap == NULL) return NULL;
    if(sap) salp = sap->data;
    if(salp) {
        sep = SeqEntryNew();
        bssp = BioseqSetNew();
        if (bssp == NULL) return NULL;
        bssp->_class = BioseqseqSet_class_genbank;
        sep->data.ptrvalue = bssp;
        sep->choice = 2;

        pvnPrev = NULL;
        for(pvn = ((BioseqSet *)pvnsep->data.ptrvalue)->seq_set;
                pvn != NULL; pvn = pvn->next) {
            isep = SeqEntryNew();
            isep->choice = pvn->choice;
            isep->data.ptrvalue = pvn->data.ptrvalue;
            if(pvnPrev) pvnPrev->next = isep;
            else bssp->seq_set = isep;
            pvnPrev = isep;
        }

        salp = ViewMgr_GetBeginIndexed(salp);
        if(salp == NULL) return NULL;
        sip = AlnMgrGetUniqueSeqs(salp, &n);
        for(i = 1, isip = sip->next; i < n; i++, isip = isip->next) {
            isep->next = SeqEntryNew();
            isep = isep->next;
            isep->data.ptrvalue = BioseqLockById(isip);
            isep->choice = 1;
            BioseqUnlock((Bioseq *)isep->data.ptrvalue);
        }
        SeqIdSetFree(sip);
        isep->next = NULL;
        return sep;  /* need cleanup */
    }
    return NULL;
}

static void Cn3D_ClearSubSet(ValNode * sep)
{
    BioseqSet *bssp;
    
    if(sep) {
        if(sep->data.ptrvalue) {
            bssp = (BioseqSet *)sep->data.ptrvalue;
            bssp->seq_set = ValNodeFree(bssp->seq_set);
            sep->data.ptrvalue = BioseqSetFree(bssp);
        }
        ValNodeFree(sep);
    }
}

static SeqAnnot * Cn3D_MakeSaveAnnot(SeqAnnot *sap)
{
    SeqAlign *salp;
    SeqAnnot *sapout, *saphead = NULL;

    /* index the seq aligns */
    for (; sap != NULL; sap = sap->next) {
        if (sap->data == NULL) continue;
        if(saphead == NULL) saphead = sapout = SeqAnnotNew();
        else {
            sapout->next = SeqAnnotNew();
            sapout = sapout->next;
        }
        salp = sap->data;
        sapout->data = ViewMgr_GetBegin(salp);
        sapout->desc = sap->desc;
        sapout->type = sap->type;
    }
    return saphead;
}

static void Cn3D_ClearSaveAnnot(SeqAnnot *sap)
{
    SeqAnnot *sapprev;

    while (sap != NULL) {
        sap->data = NULL;
        sap->desc = NULL;
        sap->type = 0;
        sapprev = sap;
        sap = sap->next;
        SeqAnnotFree(sapprev);
    }
}

static void Cn3D_ExportAsnNow(ButtoN b)
{

    Char path[256];
    Int2 iTest;
    Int4 iCount = 0;
    CharPtr pcSave = NULL;
    Byte bSave = 0;

    Int2Ptr i2Vec = NULL;

    Boolean iCn3d = TRUE, FirstSlave = FALSE;
    CharPtr pcSaveMode = NULL;

    PDNMS pdnmsMaster = NULL, pdnmsSlave = NULL;
    PMSD pmsdSlave = NULL, pmsdMaster = NULL;
    NcbiMimeAsn1Ptr mime = NULL;
    BiostrucAlignPtr bsap = NULL;
    BiostrucSeqPtr bssp = NULL;
    BiostrucSeqsPtr bsssp = NULL;
    BiostrucPtr bsSlaveHead = NULL, bsSlave = NULL, bsSlaveTail = NULL;
    BiostrucFeaturePtr pbsfThis;
    /*PDNTRN pdnTransform = NULL;*/
    AsnIoPtr aip = NULL;
    PDNML pdnmlThis = NULL;

/*   if (GetStatus(Cn3D_bFeatOn) == FALSE)
       bSave = (Byte) (bSave | (Byte) NOT_FEATURES);  */
    /* save feature always for now -yanli */
#ifdef _OPENGL
    Cn3D_AddMatrix();
#endif
    if (GetValue(Cn3D_gBinAscii) == 2)
        bSave = (Byte) (bSave | (Byte) SAVE_BINARY);

    if (bSave & (Byte) SAVE_BINARY)
        pcSaveMode = SAVE_BINARY_STRING;
    else
        pcSaveMode = SAVE_ASCII_STRING;

    i2Vec = PickedModels(&iCount);
    GetTitle(Cn3D_tAsnSave, path, sizeof(path));
    pdnmsMaster = GetSelectedModelstruc();
    iTest =
        WriteAsnModelList(pdnmsMaster, iCount, i2Vec, path, bSave, iCn3d);
    if (!iTest) {
        ErrClear();
        ErrPostEx(SEV_FATAL, 0, 0,
                  "Unable to Export\nPossibly Corrupt Data in Memory!\n");
        ErrShow();

    }
    Cn3D_AddGlobalRenderSettingsToBiostruc(pdnmsMaster);
    Cn3DAddUserDefinedFeatureToBiostruc(pdnmsMaster);

    pmsdMaster = pdnmsMaster->data.ptrvalue;

    aip = AsnIoOpen(path, pcSaveMode);

    mime = (NcbiMimeAsn1Ptr) ValNodeNew(NULL);
    switch (pmsdMaster->iMimeType) {
    case NcbiMimeAsn1_entrez:
        BiostrucAsnWrite(pmsdMaster->pbsBS, aip, NULL);
        break;
    case NcbiMimeAsn1_strucseq:
        mime->choice = NcbiMimeAsn1_strucseq;
        bssp = BiostrucSeqNew();
        bssp->structure = pmsdMaster->pbsBS;
        bssp->sequences = Cn3D_ColorData.pvnsep;
        mime->data.ptrvalue = bssp;
        NcbiMimeAsn1AsnWrite(mime, aip, NULL);
        break;
    case NcbiMimeAsn1_strucseqs:
        mime->choice = NcbiMimeAsn1_strucseqs;
        bsssp = BiostrucSeqsNew();
        bsssp->structure = pmsdMaster->pbsBS;
        bsssp->sequences = Cn3D_GetSubSet(Cn3D_ColorData.pvnsep, Cn3D_ColorData.sap);
        bsssp->seqalign = Cn3D_MakeSaveAnnot(Cn3D_ColorData.sap);
        mime->data.ptrvalue = bsssp;
        NcbiMimeAsn1AsnWrite(mime, aip, NULL);
        Cn3D_ClearSaveAnnot(bsssp->seqalign);
        Cn3D_ClearSubSet(bsssp->sequences);
        break;
    case NcbiMimeAsn1_alignstruc:
        mime->choice = NcbiMimeAsn1_alignstruc;
        bsap = BiostrucAlignNew();
        bsap->master = pmsdMaster->pbsBS;
        pdnmsSlave = pmsdMaster->pdnmsSlaves;
        FirstSlave = TRUE;

        pbsfThis =
            (BiostrucFeaturePtr) pmsdMaster->psaStrucAlignment->features->
            features;


        while (pdnmsSlave) {

            iTest = WriteAsnModelList(pdnmsSlave, iCount, i2Vec, path, bSave, iCn3d);
            if (!iTest) {
                ErrClear();
                ErrPostEx(SEV_FATAL, 0, 0,
                          "Unable to Export\nPossibly Corrupt Data in Memory!\n");
                ErrShow();

            }

            if (!bsSlaveHead) {
                bsSlaveHead =
                    (BiostrucPtr) ((PMSD) pdnmsSlave->data.ptrvalue)->
                    pbsBS;
                bsSlaveTail = bsSlaveHead;
            } else {
                bsSlave =
                    (BiostrucPtr) ((PMSD) pdnmsSlave->data.ptrvalue)->
                    pbsBS;
                bsSlaveTail->next = bsSlave;
                bsSlaveTail = bsSlaveTail->next;
                bsSlaveTail->next = NULL;
            }
            pdnmsSlave = (PDNMS) Cn3DAddUserDefinedFeatureToBiostruc(pdnmsSlave);
            pdnmsSlave = pdnmsSlave->next;
        }
        bsap->slaves = bsSlaveHead;
        bsap->sequences = Cn3D_GetSubSet(Cn3D_ColorData.pvnsep, Cn3D_ColorData.sap);
        bsap->alignments = pmsdMaster->psaStrucAlignment;
        bsap->seqalign = Cn3D_MakeSaveAnnot(Cn3D_ColorData.sap);
        mime->data.ptrvalue = bsap;
        NcbiMimeAsn1AsnWrite(mime, aip, NULL);
        Cn3D_ClearSaveAnnot(bsap->seqalign);
        Cn3D_ClearSubSet(bsap->sequences);
        break;
    }

    aip = AsnIoClose(aip);

    Cn3DRemoveUserDefinedFeatureFromBiostruc(pdnmsMaster);
    Cn3D_RemoveGlobalRenderSettingsFromBiostruc(pdnmsMaster);
    FreeRedundantAsn(pdnmsMaster);
    pdnmsSlave = pmsdMaster->pdnmsSlaves;
    while (pdnmsSlave) {
        Cn3DRemoveUserDefinedFeatureFromBiostruc(pdnmsSlave);
        FreeRedundantAsn(pdnmsSlave);
        pdnmsSlave = pdnmsSlave->next;
    }

    if (i2Vec)
        I2VectorFree(i2Vec, 0);
    Remove(Cn3D_wAsnSave);
    Cn3D_EnableFileOps();
    Cn3D_Save_InUse = FALSE;
    ArrowCursor();
    return;
}

static void Cn3D_CancelAsn(ButtoN b)
{
    Remove(Cn3D_wAsnSave);
    Cn3D_EnableFileOps();
    Cn3D_Save_InUse = FALSE;
    return;
}



NLM_EXTERN void Cn3D_SaveBiostruc(IteM i)
{
    PDNMS pdnmsThis = NULL;
    PMSD pmsdThis = NULL;
    Char pcSavestr[60];
    Char pcSavename[32];
    CharPtr Cn3D_pcAsnName;
    GrouP g, g2, g3, g4, g5, g6;
    GrouP gMS;
    ButtoN b;

    if (Cn3D_Save_InUse)
        return;
    else
        Cn3D_Save_InUse = TRUE;

    pdnmsThis = GetSelectedModelstruc();
    if (!pdnmsThis) {
        Cn3D_Save_InUse = FALSE;
        return;
    }
    pmsdThis = (PMSD) pdnmsThis->data.ptrvalue;

    Cn3D_wAsnSave = ModalWindow(-20, -13, -10, -10, NULL);

    /* set up a group encolosing structures - models selection lists and - "info strings" */
    Cn3D_pcAsnName = StringSave(GetStrucStrings(pdnmsThis, PDB_ACC));
    sprintf(pcSavestr, "Save Biostruc %s as an Asn.1 File...",
            Cn3D_pcAsnName);
    g = HiddenGroup(Cn3D_wAsnSave, 0, 4, NULL);
    SetGroupMargins(g, 10, 10);
    SetGroupSpacing(g, 10, 5);
    StaticPrompt(g, pcSavestr, 0, 0, systemFont, 'l');
    g2 = HiddenGroup(g, 2, 0, NULL);
    SetGroupMargins(g2, 10, 10);
    SetGroupSpacing(g2, 10, 5);
    StringNCpy(pcSavename, Cn3D_pcAsnName, 8);
    StringCat(pcSavename, ".prt");
    Cn3D_tAsnSave =
        DialogText(g2, pcSavename, 18, (TxtActnProc) Cn3D_AsnEnableProc);
    MemFree(Cn3D_pcAsnName);

    Cn3D_bAsnBrowse =
        PushButton(g2, " browse...", (BtnActnProc) Cn3D_AsnBrowseProc);
    g3 = HiddenGroup(g, 2, 0, NULL);
    gMS = Cn3D_ModelSelect(g3, TRUE); /*  vector models OK for Asn file saves */
    g4 = HiddenGroup(g3, 0, 2, NULL);
    SetGroupMargins(g4, 10, 10);
    SetGroupSpacing(g4, 10, 5);
    Cn3D_bAsnOk = PushButton(g4, "OK", Cn3D_ExportAsnNow);
    b = PushButton(g4, "Cancel", Cn3D_CancelAsn);
    g5 = HiddenGroup(g, 2, 0, NULL); /* for bin/ascii and features on/off */
    Cn3D_gBinAscii = NormalGroup(g5, 2, 0, "file mode", systemFont, NULL);
    SetGroupMargins(Cn3D_gBinAscii, 10, 0);
    SetGroupSpacing(Cn3D_gBinAscii, 10, 5);
    RadioButton(Cn3D_gBinAscii, "Ascii");
    RadioButton(Cn3D_gBinAscii, "Binary");
    SetValue(Cn3D_gBinAscii, 1);
    g6 = HiddenGroup(g5, 0, 2, NULL);
    SetGroupMargins(g6, 10, 10);
    StaticPrompt(g6, " ", 0, 0, systemFont, 'l');
/*  Cn3D_bFeatOn =  CheckBox(g6, "Include Features", NULL);
    SetStatus(Cn3D_bFeatOn,TRUE);  */
    /* save features always for now - yanli */
    Cn3D_AsnEnableProc(NULL);
    Select(Cn3D_bAsnOk);
    /* disable appropriate stuff here */
    Cn3D_DisableFileOps();
    Show(Cn3D_wAsnSave);

    return;
}




static void Cn3D_ExportDictNow(ButtoN b)
{

    Char path[256];
    Int2 iTest;
    Int4 iCount = 0;
    PDNMS pdnmsMain = NULL;
    CharPtr pcSave = NULL;
    Byte bSave = 0;

    GetTitle(Cn3D_tAsnSave, path, sizeof(path));
    pdnmsMain = GetSelectedModelstruc();
    if (GetValue(Cn3D_gBinAscii) == 2)
        bSave = (Byte) SAVE_BINARY;
    iTest = WriteAsnLocalDict(pdnmsMain, path, bSave, TRUE);
    if (!iTest) {
        ErrClear();
        ErrPostEx(SEV_FATAL, 0, 0,
                  "Unable to Export\nPossibly Corrupt Data in Memory!\n");
        ErrShow();
    }
    Remove(Cn3D_wAsnSave);
    Cn3D_EnableFileOps();
    Cn3D_Save_InUse = FALSE;
    ArrowCursor();
    return;
}



static void Cn3D_SaveDictionary(IteM i)
{
    PDNMS pdnmsThis = NULL;
    PMSD pmsdThis = NULL;
    Char pcSavestr[60];
    Char pcSavename[32];
    CharPtr Cn3D_pcAsnName;
    GrouP g, g2, g3, g4, g5;
    ButtoN b;

    if (Cn3D_Save_InUse)
        return;
    else
        Cn3D_Save_InUse = TRUE;

    pdnmsThis = GetSelectedModelstruc();
    if (!pdnmsThis) {
        Cn3D_Save_InUse = FALSE;
        return;
    }
    pmsdThis = (PMSD) pdnmsThis->data.ptrvalue;

    Cn3D_wAsnSave = ModalWindow(-20, -13, -10, -10, NULL);

    Cn3D_pcAsnName = StringSave(GetStrucStrings(pdnmsThis, PDB_ACC));
    sprintf(pcSavestr, "Save Local Dictionary from %s ...",
            Cn3D_pcAsnName);
    g = HiddenGroup(Cn3D_wAsnSave, 0, 3, NULL);
    SetGroupMargins(g, 10, 10);
    SetGroupSpacing(g, 10, 5);
    StaticPrompt(g, pcSavestr, 0, 0, systemFont, 'l');
    g2 = HiddenGroup(g, 2, 0, NULL);
    SetGroupMargins(g2, 10, 10);
    SetGroupSpacing(g2, 10, 5);
    StringNCpy(pcSavename, Cn3D_pcAsnName, 8);
    StringCat(pcSavename, "dict.prt");
    Cn3D_tAsnSave =
        DialogText(g2, pcSavename, 18, (TxtActnProc) Cn3D_AsnEnableProc);
    MemFree(Cn3D_pcAsnName);
    Cn3D_bAsnBrowse =
        PushButton(g2, " browse...", (BtnActnProc) Cn3D_AsnBrowseProc);

    g3 = HiddenGroup(g, 2, 0, NULL);
    g4 = HiddenGroup(g3, 2, 0, NULL);
    SetGroupMargins(g4, 10, 0);
    Cn3D_gBinAscii = NormalGroup(g4, 2, 1, "file mode", systemFont, NULL);
    SetGroupMargins(Cn3D_gBinAscii, 10, 10);
    RadioButton(Cn3D_gBinAscii, "Ascii");
    RadioButton(Cn3D_gBinAscii, "Binary");
    SetValue(Cn3D_gBinAscii, 1);

    g5 = HiddenGroup(g3, 2, 0, NULL);
    SetGroupMargins(g5, 10, 10);
    SetGroupSpacing(g5, 10, 5);
    Cn3D_bAsnOk = PushButton(g5, "OK", Cn3D_ExportDictNow);
    b = PushButton(g5, "Cancel", Cn3D_CancelAsn);

    Cn3D_AsnEnableProc(NULL);
    Select(Cn3D_bAsnOk);
    Cn3D_DisableFileOps();
    Show(Cn3D_wAsnSave);

    return;
}

MenU LIBCALL Cn3D_SaveSub(MenU m)
{
    IteM i;
    MenU s;

    s = SubMenu(m, "Save");
    i = CommandItem(s, "All/B", Cn3D_SaveBiostruc);
    /* i = CommandItem (s, "Feature-set/F", NULL); */
    i = CommandItem(s, "Dictionary/D", Cn3D_SaveDictionary);
    return s;
}

/*------------------------------------------------------------------------------
 * stuff below implements save/read of an ARS structure to/from ASN1
 *------------------------------------------------------------------------------*/

typedef struct _ARSDataFieldInfo {
    Int1 type;
    CharPtr description;
} ARSDataFieldInfo, PNTR ARSDataFieldInfoPtr;

enum { /* possible types for data in PARS structure */
    TYPE_BOOLEAN = 1,
    TYPE_INT2,
    TYPE_UINT1PTR_3 /* pointer to array of 3 Uint1's */
};


/* map of ARS stucture, with description and type for each field */
static ARSDataFieldInfo ARSFieldData[] = {
    { TYPE_BOOLEAN,     "Display Hydrogens" } ,
    { TYPE_UINT1PTR_3,  "Background Color" } ,
    { TYPE_INT2,        "Conservation Color Algorithm" } ,
    { TYPE_BOOLEAN,     "Display Protein Virtual Backbone" } ,
    { TYPE_BOOLEAN,     "Display Protein Partial Backbone" } ,
    { TYPE_BOOLEAN,     "Display Protein Complete Backbone" } ,
    { TYPE_BOOLEAN,     "Display Protein Residues" } ,
    { TYPE_INT2,        "Protein Backbone Rendering Style" } ,
    { TYPE_INT2,        "Protein Backbone Color Scheme" } ,
    { TYPE_INT2,        "Protein Residue Rendering Style" } ,
    { TYPE_INT2,        "Protein Residue Color Scheme" } ,
    { TYPE_UINT1PTR_3,  "Protein Backbone Custom Color" } ,
    { TYPE_UINT1PTR_3,  "Protein Residue Custom Color" } ,
    { TYPE_INT2,        "Protein Label Interval" } ,
    { TYPE_INT2,        "Protein Label Justification" } ,
    { TYPE_INT2,        "Protein Label Style" } ,
    { TYPE_INT2,        "Protein Label Scale" } ,
    { TYPE_BOOLEAN,     "Display Protein Termini Labels" } ,
    { TYPE_INT2,        "Protein Termini Label Justification" } ,
    { TYPE_INT2,        "Protein Termini Label Style" } ,
    { TYPE_INT2,        "Protein Termini Label Scale" } ,
    { TYPE_BOOLEAN,     "Display Nucleotide Virtual Backbone" } ,
    { TYPE_BOOLEAN,     "Display Nucleotide Partial Backbone" } ,
    { TYPE_BOOLEAN,     "Display Nucleotide Complete Backbone" } ,
    { TYPE_BOOLEAN,     "Display Nucleotide Residues" } ,
    { TYPE_INT2,        "Nucleotide Backbone Rendering Style" } ,
    { TYPE_INT2,        "Nucleotide Backbone Color Scheme" } ,
    { TYPE_INT2,        "Nucleotide Residue Rendering Style" } ,
    { TYPE_INT2,        "Nucleotide Residue Color Scheme" } ,
    { TYPE_UINT1PTR_3,  "Nucleotide Backbone Custom Color" } ,
    { TYPE_UINT1PTR_3,  "Nucleotide Residue Custom Color" } ,
    { TYPE_INT2,        "Nucleotide Label Interval" } ,
    { TYPE_INT2,        "Nucleotide Label Justification" } ,
    { TYPE_INT2,        "Nucleotide Label Style" } ,
    { TYPE_INT2,        "Nucleotide Label Scale" } ,
    { TYPE_BOOLEAN,     "Display Nucleotide Termini Labels" } ,
    { TYPE_INT2,        "Nucleotide Termini Label Justification" } ,
    { TYPE_INT2,        "Nucleotide Termini Label Style" } ,
    { TYPE_INT2,        "Nucleotide Termini Label Scale" } ,
    { TYPE_BOOLEAN,     "Display Heterogens" } ,
    { TYPE_INT2,        "Heterogen Rendering Style" } ,
    { TYPE_INT2,        "Heterogen Color Scheme" } ,
    { TYPE_BOOLEAN,     "Display Ions" } ,
    { TYPE_INT2,        "Ion Rendering Style" } ,
    { TYPE_INT2,        "Ion Color Scheme" } ,
    { TYPE_BOOLEAN,     "Display Ion Labels" } ,
    { TYPE_BOOLEAN,     "Display Connections" } ,
    { TYPE_INT2,        "Connection Rendering Style" } ,
    { TYPE_INT2,        "Connection Color Scheme" } ,
    { TYPE_BOOLEAN,     "Display Solvent" } ,
    { TYPE_INT2,        "Solvent Rendering Style" } ,
    { TYPE_INT2,        "Solvent Color Scheme" } ,
    { TYPE_BOOLEAN,     "Display 3D Objects" } ,
    { TYPE_INT2,        "3D Object Color Scheme" } ,
    { TYPE_INT2,        "3D Object Arrows" } ,
    { 0,                NULL }
};


static VoidPtr Cn3D_GetARSFieldAddress(PARS pars, CharPtr descr)
{
    switch (descr[0]) { /* crude hash */
    case 'D':
        if (StrCmp(descr, "Display 3D Objects") == 0) return ((VoidPtr) &(pars->ObjectOn));
        else if (StrCmp(descr, "Display Connections") == 0) return ((VoidPtr) &(pars->ConnectOn));
        else if (StrCmp(descr, "Display Heterogens") == 0) return ((VoidPtr) &(pars->HeterogensOn));
        else if (StrCmp(descr, "Display Hydrogens") == 0) return ((VoidPtr) &(pars->HydrogensOn));
        else if (StrCmp(descr, "Display Ion Labels") == 0) return ((VoidPtr) &(pars->IonLabelOn));
        else if (StrCmp(descr, "Display Ions") == 0) return ((VoidPtr) &(pars->IonsOn));
        else if (StrCmp(descr, "Display Nucleotide Complete Backbone") == 0) return ((VoidPtr) &(pars->NTCompleteBBOn));
        else if (StrCmp(descr, "Display Nucleotide Partial Backbone") == 0) return ((VoidPtr) &(pars->NTPartialBBOn));
        else if (StrCmp(descr, "Display Nucleotide Residues") == 0) return ((VoidPtr) &(pars->NTResiduesOn));
        else if (StrCmp(descr, "Display Nucleotide Termini Labels") == 0) return ((VoidPtr) &(pars->NTTermLabelOn));
        else if (StrCmp(descr, "Display Nucleotide Virtual Backbone") == 0) return ((VoidPtr) &(pars->NTVirtualBBOn));
        else if (StrCmp(descr, "Display Protein Complete Backbone") == 0) return ((VoidPtr) &(pars->PCompleteBBOn));
        else if (StrCmp(descr, "Display Protein Partial Backbone") == 0) return ((VoidPtr) &(pars->PPartialBBOn));
        else if (StrCmp(descr, "Display Protein Residues") == 0) return ((VoidPtr) &(pars->PResiduesOn));
        else if (StrCmp(descr, "Display Protein Termini Labels") == 0) return ((VoidPtr) &(pars->PTermLabelOn));
        else if (StrCmp(descr, "Display Protein Virtual Backbone") == 0) return ((VoidPtr) &(pars->PVirtualBBOn));
        else if (StrCmp(descr, "Display Solvent") == 0) return ((VoidPtr) &(pars->SolventOn));
        break;
    case 'P':
        if (StrCmp(descr, "Protein Backbone Color Scheme") == 0) return ((VoidPtr) &(pars->PBBColor));
        else if (StrCmp(descr, "Protein Backbone Custom Color") == 0) return ((VoidPtr) &(pars->PBBColRGB));
        else if (StrCmp(descr, "Protein Backbone Rendering Style") == 0) return ((VoidPtr) &(pars->PBBRender));
        else if (StrCmp(descr, "Protein Label Interval") == 0) return ((VoidPtr) &(pars->PBBLabelInterval));
        else if (StrCmp(descr, "Protein Label Justification") == 0) return ((VoidPtr) &(pars->PBBLabelJust));
        else if (StrCmp(descr, "Protein Label Scale") == 0) return ((VoidPtr) &(pars->PBBLabelScale));
        else if (StrCmp(descr, "Protein Label Style") == 0) return ((VoidPtr) &(pars->PBBLabelStyle));
        else if (StrCmp(descr, "Protein Residue Color Scheme") == 0) return ((VoidPtr) &(pars->PResColor));
        else if (StrCmp(descr, "Protein Residue Custom Color") == 0) return ((VoidPtr) &(pars->PResColRGB));
        else if (StrCmp(descr, "Protein Residue Rendering Style") == 0) return ((VoidPtr) &(pars->PResRender));
        else if (StrCmp(descr, "Protein Termini Label Justification") == 0) return ((VoidPtr) &(pars->PTermLabelJust));
        else if (StrCmp(descr, "Protein Termini Label Scale") == 0) return ((VoidPtr) &(pars->PTermLabelScale));
        else if (StrCmp(descr, "Protein Termini Label Style") == 0) return ((VoidPtr) &(pars->PTermLabelStyle));
        break;
    case 'N':
        if (StrCmp(descr, "Nucleotide Backbone Color Scheme") == 0) return ((VoidPtr) &(pars->NTBBColor));
        else if (StrCmp(descr, "Nucleotide Backbone Custom Color") == 0) return ((VoidPtr) &(pars->NTBBColRGB));
        else if (StrCmp(descr, "Nucleotide Backbone Rendering Style") == 0) return ((VoidPtr) &(pars->NTBBRender));
        else if (StrCmp(descr, "Nucleotide Label Interval") == 0) return ((VoidPtr) &(pars->NTBBLabelInterval));
        else if (StrCmp(descr, "Nucleotide Label Justification") == 0) return ((VoidPtr) &(pars->NTBBLabelJust));
        else if (StrCmp(descr, "Nucleotide Label Scale") == 0) return ((VoidPtr) &(pars->NTBBLabelScale));
        else if (StrCmp(descr, "Nucleotide Label Style") == 0) return ((VoidPtr) &(pars->NTBBLabelStyle));
        else if (StrCmp(descr, "Nucleotide Residue Color Scheme") == 0) return ((VoidPtr) &(pars->NTResColor));
        else if (StrCmp(descr, "Nucleotide Residue Custom Color") == 0) return ((VoidPtr) &(pars->NTResColRGB));
        else if (StrCmp(descr, "Nucleotide Residue Rendering Style") == 0) return ((VoidPtr) &(pars->NTResRender));
        else if (StrCmp(descr, "Nucleotide Termini Label Justification") == 0) return ((VoidPtr) &(pars->NTTermLabelJust));
        else if (StrCmp(descr, "Nucleotide Termini Label Scale") == 0) return ((VoidPtr) &(pars->NTTermLabelScale));
        else if (StrCmp(descr, "Nucleotide Termini Label Style") == 0) return ((VoidPtr) &(pars->NTTermLabelStyle));
        break;
    default:
        if (StrCmp(descr, "3D Object Color Scheme") == 0) return ((VoidPtr) &(pars->ObjectColor));
        else if (StrCmp(descr, "3D Object Arrows") == 0) return ((VoidPtr) &(pars->ObjectArrows));
        else if (StrCmp(descr, "Background Color") == 0) return ((VoidPtr) &(pars->BGColRGB));
        else if (StrCmp(descr, "Connection Color Scheme") == 0) return ((VoidPtr) &(pars->ConnectColor));
        else if (StrCmp(descr, "Connection Rendering Style") == 0) return ((VoidPtr) &(pars->ConnectRender));
        else if (StrCmp(descr, "Conservation Color Algorithm") == 0) return ((VoidPtr) &(pars->ConsColAlg));
        else if (StrCmp(descr, "Heterogen Color Scheme") == 0) return ((VoidPtr) &(pars->HetColor));
        else if (StrCmp(descr, "Heterogen Rendering Style") == 0) return ((VoidPtr) &(pars->HetRender));
        else if (StrCmp(descr, "Ion Color Scheme") == 0) return ((VoidPtr) &(pars->IonColor));
        else if (StrCmp(descr, "Ion Rendering Style") == 0) return ((VoidPtr) &(pars->IonRender));
        else if (StrCmp(descr, "Solvent Color Scheme") == 0) return ((VoidPtr) &(pars->SolventColor));
        else if (StrCmp(descr, "Solvent Rendering Style") == 0) return ((VoidPtr) &(pars->SolventRender));
    }
    Message(MSG_POST, "Unrecognized ARS field %s", descr);
    return NULL;
}

static int Cn3D_GetARSFieldType(CharPtr descr)
{
    int i;
    for (i = 0; ARSFieldData[i].description != NULL; i++) {
        if (StrCmp(descr, ARSFieldData[i].description) == 0)
            return ARSFieldData[i].type;
    }
    Message(MSG_POST, "Unrecognized ARS field %s", descr);
    return -1;
}

/* returns field as a new string - should be freed by caller */
static CharPtr Cn3D_GetARSFieldContentsAsString(PARS pars, CharPtr descr)
{
    int type;
    VoidPtr address;
    Char str[32];
    CharPtr copy;

    address = Cn3D_GetARSFieldAddress(pars, descr);
    type = Cn3D_GetARSFieldType(descr);
    if (!address || type < 0) {
        Message(MSG_POST, "Unrecognised ARS field %s", descr);
        return NULL;
    }

    switch (type) {
    case TYPE_BOOLEAN:
        if (*((Boolean *) address) == TRUE)
            StrCpy(str, "True");
        else
            StrCpy(str, "False");
        break;
    case TYPE_INT2:
        sprintf(str, "%i", *((Int2 *) address));
        break;
    case TYPE_UINT1PTR_3:
        sprintf(str, "%i %i %i", ((Uint1 *) address)[0],
            ((Uint1 *) address)[1], ((Uint1 *) address)[2]);
        break;
    default:
        Message(MSG_ERROR, "Invalid ARS type (%i) for %s", type, descr);
        return NULL;
    }
    copy = (CharPtr) MemNew((StrLen(str) + 1) * sizeof(Char));
    if (!copy) return NULL;
    StrCpy(copy, str);
    return copy;
}

static BiostrucFeatureSetPtr Cn3D_MakeBSFSPFromARS(PARS pars)
{
    BiostrucFeatureSetPtr bsfsp;
    BiostrucFeaturePtr bsfp;
    UserObjectPtr pUO;
    UserFieldPtr pUF, pUFprev;
    int i;

    bsfsp = BiostrucFeatureSetNew();
    bsfp = BiostrucFeatureNew();
    if (!bsfp || !bsfp) return NULL;

    /* new BiostrucFeatureSet */
    bsfsp->id = 0;
    ValNodeAddPointer(&(bsfsp->descr), BiostrucFeatureSetDescr_name,
        StringSave("Global Rendering Settings"));
    bsfsp->next = NULL;
    bsfsp->features = bsfp;

    /* new BiostrucFeature */
    bsfp->id = 0;
    bsfp->name = NULL;
    bsfp->Location_location = NULL;
    bsfp->type = Feature_type_other;
    bsfp->next = NULL;

    /* new user property (ValNode with User-object)*/
    bsfp->Property_property = ValNodeNew(NULL);
    if (!bsfp->Property_property) return NULL;
    bsfp->Property_property->choice = Property_property_user;
    pUO = UserObjectNew();
    if (!pUO) return NULL;
    bsfp->Property_property->data.ptrvalue = (VoidPtr) pUO;
    pUO->_class = NULL;
    pUO->type = ObjectIdNew();
    if (!pUO->type) return NULL;
    pUO->type->id = 0;
    pUO->type->str = StringSave("Cn3D AgorithmicRenderSet Data");

    /* fill in User-object data with contents of ARS */
    for (i = 0; ARSFieldData[i].description != NULL; i++) {
        pUF = UserFieldNew();
        if (!pUF) return NULL;
        pUF->num = 0;
        pUF->next = NULL;
        if (i == 0)
            pUO->data = pUF;
        else
            pUFprev->next = pUF;
        pUFprev = pUF;

        pUF->label = ObjectIdNew();
        if (!pUF->label) return NULL;
        pUF->label->id = 0;
        pUF->label->str = StringSave(ARSFieldData[i].description);
        pUF->choice = 1; /* str data */
        pUF->data.ptrvalue = (VoidPtr)
            Cn3D_GetARSFieldContentsAsString(pars, ARSFieldData[i].description);
    }

    return bsfsp;
}

static void Cn3D_AddGlobalRenderSettingsToBiostruc(PDNMS pdnms)
{
    BiostrucFeatureSetPtr bsfsp, bsfspNew;
    PARS pars;
    PMSD pmsd;

    if (!pdnms ||
        !(pmsd = (PMSD) pdnms->data.ptrvalue) ||
        !(pars = pmsd->pGlobalPARS))
        return;

    /* make new feature set and add to end of MSD's feature list */
    bsfspNew = Cn3D_MakeBSFSPFromARS(pars);
    if (!bsfspNew) return;
    bsfsp = pmsd->pbsBS->features;
    if (bsfsp) {
        while (bsfsp->next) bsfsp = bsfsp->next;
        bsfsp->next = bsfspNew;
    } else {
        pmsd->pbsBS->features = bsfspNew;
    }
}

static void Cn3D_RemoveGlobalRenderSettingsFromBiostruc(PDNMS pdnms)
{
    BiostrucPtr bsp;
    BiostrucFeatureSetPtr bsfsp, bsfsp_prev = NULL, bsfsp_curr = NULL;
    ValNodePtr descr = NULL;

    if (!pdnms || !pdnms->data.ptrvalue ||
        !(bsp = ((PMSD) pdnms->data.ptrvalue)->pbsBS))
        return;

    /* assuming this feature set always occur at the tail end of the feature list,
       unhook from the biostruc and free */
    bsfsp = bsp->features;
    while (bsfsp) {
        if (bsfsp->descr != NULL) {
            descr = bsfsp->descr;
            if (descr->choice == BiostrucFeatureSetDescr_name) {
                if (StrCmp(descr->data.ptrvalue, "Global Rendering Settings") == 0) {
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

/* returns TRUE on success, FALSE on error */
static Boolean Cn3D_SetARSFieldContentsFromString(PARS pars,
                                                  CharPtr descr,
                                                  CharPtr data)
{
    int type, i1, i2, i3;
    VoidPtr address;

    if (!pars || !descr || !data ||
        (type = Cn3D_GetARSFieldType(descr)) < 0 ||
        !(address = Cn3D_GetARSFieldAddress(pars, descr)))
        return FALSE;

    switch (type) {
    case TYPE_BOOLEAN:
        *((Boolean *) address) = (StrCmp(data, "True") == 0);
        break;
    case TYPE_INT2:
        *((Int2 *) address) = atoi(data);
        break;
    case TYPE_UINT1PTR_3:
        if (sscanf(data, "%i %i %i", &i1, &i2, &i3) != 3) return FALSE;
        ((Uint1 *) address)[0] = (Uint1) i1;
        ((Uint1 *) address)[1] = (Uint1) i2;
        ((Uint1 *) address)[2] = (Uint1) i3;
        break;
    default:
        Message(MSG_ERROR, "Invalid ARS type (%i) for %s", type, descr);
        return FALSE;
    }
    return TRUE;
}

NLM_EXTERN void Cn3D_GetRenderSettingsFromBiostruc(PDNMS pdnms)
{
    PMSD pmsd;
    BiostrucFeatureSetPtr bsfsp;
    UserObjectPtr pUO;
    UserFieldPtr pUF;

    if (!pdnms || !(pmsd = (PMSD) pdnms->data.ptrvalue)) return;

    /* find feature set in biostruc */
    bsfsp = pmsd->pbsBS->features;
    while (bsfsp) {
        if (bsfsp->descr && bsfsp->descr->data.ptrvalue &&
            bsfsp->descr->choice == BiostrucFeatureSetDescr_name &&
            StrCmp(bsfsp->descr->data.ptrvalue, "Global Rendering Settings") == 0)
            break;
        bsfsp = bsfsp->next;
    }
    if (!bsfsp) return;

    /* find start of User-field list */
    if (!bsfsp->features ||
        !bsfsp->features->Property_property ||
        bsfsp->features->Property_property->choice != Property_property_user || /* check property type */
        !(pUO = (UserObjectPtr) bsfsp->features->Property_property->data.ptrvalue) ||
        !pUO->type || !pUO->type->str || /* check object type */
        StrCmp(pUO->type->str, "Cn3D AgorithmicRenderSet Data") != 0 ||
        !(pUF = pUO->data))
        return;

    /* read ARS fields from User-fields */
    for (; pUF; pUF = pUF->next) {
        if (!pUF->label || !pUF->label->str ||
            pUF->choice != 1 || !pUF->data.ptrvalue)
            continue;
        Cn3D_SetARSFieldContentsFromString(pmsd->pGlobalPARS,
            pUF->label->str, (CharPtr) pUF->data.ptrvalue);
    }

    /* remove now; locally changed settings re-added upon save */
    Cn3D_RemoveGlobalRenderSettingsFromBiostruc(pdnms);
}
