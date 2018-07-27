/*   cn3dxprt.c
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
* File Name:  cn3dxprt.c
*
* Author:  Christopher Hogue
*
* Version Creation Date:   1/31/96
*
* File Description: Cn3d file export to Kinemage / PDB sub menu...
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: cn3dxprt.c,v $
* Revision 6.9  2000/07/28 21:05:56  lewisg
* more c++ fixes
*
* Revision 6.8  2000/03/24 20:34:58  lewisg
* add blast from file, bug fixes, get rid of redundant code, etc.
*
* Revision 6.7  2000/01/07 16:28:37  thiessen
* fixed broken header conflicts
*
* Revision 6.6  1999/12/17 20:25:00  thiessen
* put in preliminary PNG output (for Cn3D)
*
* Revision 6.5  1999/11/22 14:46:45  thiessen
* moved _OPENGL code blocks to only vibrant and ncbicn3d libraries
*
* Revision 6.4  1999/10/29 14:15:32  thiessen
* ran all Cn3D source through GNU Indent to prettify
*
* Revision 6.3  1999/04/06 20:19:26  lewisg
* more opengl
*
* Revision 6.2  1999/03/12 18:37:30  kans
* fixed ErrPostEx problems
*
* Revision 6.1  1999/02/25 23:14:32  ywang
* move around menu item and callback function, change menu item names
*
 * Revision 6.0  1997/08/25  18:13:54  madden
 * Revision changed to 6.0
 *
* Revision 5.0  1996/05/28 14:05:44  ostell
* Set to revision 5.0
*
 * Revision 1.2  1996/04/26  18:43:07  vakatov
 * CN3D sources ported to MS-Windows;
 * the portability errors and warnings fixed, etc.
 *
 * Revision 1.1  1996/02/01  18:47:38  kans
 * Initial revision
 *
*
* ==========================================================================
*/

#include <vibrant.h>
#include <mmdbapi.h>
#include <cn3dxprt.h>
#include <cn3dmsel.h>
#include <cn3dmain.h>
#include <cn3dshim.h>

#if defined(_OPENGL)
#include <shim3d.h>
#endif

static Boolean Cn3D_Export_InUse = FALSE;

static WindoW Cn3D_wPDBSave;
static TexT Cn3D_tPDBSave;
static ButtoN Cn3D_bPDBBrowse;
static ButtoN Cn3D_bPDBOk;


static WindoW Cn3D_wKinSave;
static TexT Cn3D_tKinSave;
static ButtoN Cn3D_bKinBrowse;
static ButtoN Cn3D_bKinOk;
static ButtoN Cn3D_bRender[6];
static GrouP Cn3D_gColorAtoms;
static GrouP Cn3D_gRenderOpts;

static void Cn3D_PDBEnableProc(TexT t)
{
    Char str[32];
    GetTitle(Cn3D_tPDBSave, str, sizeof(str));
    if (StringLen(str) == 0) {
        Disable(Cn3D_bPDBOk);
    } else {
        Enable(Cn3D_bPDBOk);
    }
    return;
}

static void Cn3D_PDBBrowseProc(ButtoN b)
{
    Char dfault[32];
    Char path[256];

    path[0] = '\0';
    dfault[0] = '\0';
    if (GetOutputFileName(path, sizeof(path), dfault)) {
        SetTitle(Cn3D_tPDBSave, path);
        Cn3D_PDBEnableProc(NULL);
    }
    return;
}

static void Cn3D_ExportPDBNow(ButtoN b)
{

    Char path[256];
    FILE *pFile;
    Int2 iTest;
    Int4 iCount = 0;
    PDNMS pdnmsMain = NULL;

    Int2Ptr i2Vec = NULL;


    i2Vec = PickedModels(&iCount);
    if (iCount == 0) {
        ErrClear();
        ErrPostEx(SEV_INFO, 0, 0, "Nothing to save!");
        ErrShow();
        Disable(Cn3D_bPDBOk);
        return;
    }

    GetTitle(Cn3D_tPDBSave, path, sizeof(path));
    pdnmsMain = GetSelectedModelstruc();
#ifdef WIN_MAC
    FileCreate(path, "TEXT", "ttxt");
#endif
    WatchCursor();
    pFile = FileOpen(path, "w");
    if (pFile) {
        iTest = WritePDBModelList(pdnmsMain, pFile, iCount, i2Vec);
        I2VectorFree(i2Vec, 0);
        if (!iTest) {
            ErrClear();
            ErrPostEx(SEV_FATAL, 0, 0,
                      "Unable to Export\nPossibly Corrupt Data in Memory!\n");
            ErrShow();
        }
        fflush(pFile);
        fclose(pFile);
    } else {
        ErrClear();
        ErrPostEx(SEV_INFO, 0, 0, "Sorry, Unable to Open File:\n %s",
                  path);
        ErrShow();
        Disable(Cn3D_bPDBOk);
        I2VectorFree(i2Vec, 0);
        SetTitle(Cn3D_tPDBSave, "\0");
        ArrowCursor();
        return;
    }
    Remove(Cn3D_wPDBSave);
    Cn3D_EnableFileOps();
    Cn3D_Export_InUse = FALSE;
    ArrowCursor();
    return;
}

static void Cn3D_CancelPDB(ButtoN b)
{
    Remove(Cn3D_wPDBSave);
    Cn3D_EnableFileOps();
    Cn3D_Export_InUse = FALSE;
    return;
}


static void Cn3D_ExportPDB(IteM i)
{
    PDNMS pdnmsThis = NULL;
    PMSD pmsdThis = NULL;
    Char pcSavestr[60];
    Char pcSavename[32];
    CharPtr Cn3D_pcPDBName;
    GrouP g, g2, g3, g4;
    GrouP gMS;
    ButtoN b;

    if (Cn3D_Export_InUse)
        return;
    else
        Cn3D_Export_InUse = TRUE;

    pdnmsThis = GetSelectedModelstruc();
    if (!pdnmsThis) {
        Cn3D_Export_InUse = FALSE;
        return;
    }
    pmsdThis = (PMSD) pdnmsThis->data.ptrvalue;

    Cn3D_wPDBSave = ModalWindow(-20, -13, -10, -10, NULL);

    /* set up a group encolosing structures - models selection lists and - "info strings" */
    Cn3D_pcPDBName = StringSave(GetStrucStrings(pdnmsThis, PDB_ACC));
    sprintf(pcSavestr, "Save %s As a PDB Formatted File ...",
            Cn3D_pcPDBName);
    g = HiddenGroup(Cn3D_wPDBSave, 0, 3, NULL);
    SetGroupMargins(g, 10, 10);
    SetGroupSpacing(g, 10, 5);
    StaticPrompt(g, pcSavestr, 0, 0, systemFont, 'l');
    g2 = HiddenGroup(g, 2, 0, NULL);
    SetGroupMargins(g2, 10, 10);
    SetGroupSpacing(g2, 10, 5);
    StringNCpy(pcSavename, Cn3D_pcPDBName, 8);
    StringCat(pcSavename, ".pdb");
    Cn3D_tPDBSave =
        DialogText(g2, pcSavename, 18, (TxtActnProc) Cn3D_PDBEnableProc);
    MemFree(Cn3D_pcPDBName);

    Cn3D_bPDBBrowse =
        PushButton(g2, " browse...", (BtnActnProc) Cn3D_PDBBrowseProc);
    g3 = HiddenGroup(g, 2, 0, NULL);
    gMS = Cn3D_ModelSelect(g3, FALSE); /* no vector models for PDB files saves */
    g4 = HiddenGroup(g3, 0, 2, NULL);
    SetGroupMargins(g4, 10, 10);
    SetGroupSpacing(g4, 10, 5);
    Cn3D_bPDBOk = PushButton(g4, "OK", Cn3D_ExportPDBNow);
    b = PushButton(g4, "Cancel", Cn3D_CancelPDB);
    Cn3D_PDBEnableProc(NULL);
    Select(Cn3D_bPDBOk);
    /* disable appropriate stuff here */
    Cn3D_DisableFileOps();
    Show(Cn3D_wPDBSave);

    return;
}





static void Cn3D_KinEnableProc(TexT t)
{
    Char str[32];
    GetTitle(Cn3D_tKinSave, str, sizeof(str));
    if (StringLen(str) == 0) {
        Disable(Cn3D_bKinOk);
    } else {
        Enable(Cn3D_bKinOk);
    }
    return;
}

static void Cn3D_KinBrowseProc(ButtoN b)
{
    Char dfault[32];
    Char path[256];

    path[0] = '\0';
    dfault[0] = '\0';
    if (GetOutputFileName(path, sizeof(path), dfault)) {
        SetTitle(Cn3D_tKinSave, path);
        Cn3D_KinEnableProc(NULL);
    }
    return;
}

static void Cn3D_ExportKinNow(ButtoN b)
{

    Char path[256];
    FILE *pFile;
    Int2 iTest;
    Int4 iCount = 0;
    PDNMS pdnmsMain = NULL;
    Int2 iColor = 0;
    Byte bRender = 0;
    Int2Ptr i2Vec = NULL;


    i2Vec = PickedModels(&iCount);
    if (iCount == 0) {
        ErrClear();
        ErrPostEx(SEV_INFO, 0, 0, "Nothing to save!");
        ErrShow();
        Disable(Cn3D_bKinOk);
        return;
    }
    GetTitle(Cn3D_tKinSave, path, sizeof(path));

    iColor = GetValue(Cn3D_gColorAtoms);
    bRender = KIN_DEFAULT;
    if (GetStatus(Cn3D_bRender[0]) == TRUE)
        bRender = bRender | (Byte) KIN_VIRTUAL;
    if (GetStatus(Cn3D_bRender[1]) == TRUE)
        bRender = bRender | (Byte) KIN_BACKBONE;
    if (GetStatus(Cn3D_bRender[2]) == TRUE)
        bRender = bRender | (Byte) KIN_RESIDUE;
    if (GetStatus(Cn3D_bRender[3]) == TRUE)
        bRender = bRender | (Byte) KIN_HET;
    if (GetStatus(Cn3D_bRender[4]) == TRUE)
        bRender = bRender | (Byte) KIN_ALTCONF;
    if (GetStatus(Cn3D_bRender[5]) == TRUE)
        bRender = bRender | (Byte) KIN_MULTANIM;
    pdnmsMain = GetSelectedModelstruc();
#ifdef WIN_MAC
    FileCreate(path, "TEXT", "ttxt");
#endif
    WatchCursor();
    pFile = FileOpen(path, "w");
    if (pFile) {
        iTest = WriteKinModelList(pdnmsMain, pFile, iColor, bRender,
                                  iCount, i2Vec);
        I2VectorFree(i2Vec, 0);
        if (!iTest) {
            ErrClear();
            ErrPostEx(SEV_FATAL, 0, 0,
                      "Unable to Export\nPossibly Corrupt Data in Memory!\n");
            ErrShow();
        }
        fflush(pFile);
        fclose(pFile);
    } else {
        ErrClear();
        ErrPostEx(SEV_INFO, 0, 0, "Sorry, Unable to Open File:\n %s",
                  path);
        ErrShow();
        Disable(Cn3D_bKinOk);
        I2VectorFree(i2Vec, 0);
        SetTitle(Cn3D_tKinSave, "\0");
        ArrowCursor();
        return;
    }
    Remove(Cn3D_wKinSave);
    Cn3D_EnableFileOps();
    Cn3D_Export_InUse = FALSE;
    ArrowCursor();
    return;
}

static void Cn3D_CancelKin(ButtoN b)
{
    Remove(Cn3D_wKinSave);
    Cn3D_EnableFileOps();
    Cn3D_Export_InUse = FALSE;
    return;
}


static void Cn3D_ExportKin(IteM i)
{
    PDNMS pdnmsThis = NULL;
    PMSD pmsdThis = NULL;
    Char pcSavestr[60];
    Char pcSavename[32];
    CharPtr Cn3D_pcKinName;
    GrouP g, g2, g3, g4;
    GrouP gMS;
    ButtoN b;
    Int2 iCount;

    if (Cn3D_Export_InUse)
        return;
    else
        Cn3D_Export_InUse = TRUE;

    pdnmsThis = GetSelectedModelstruc();
    if (!pdnmsThis) {
        Cn3D_Export_InUse = FALSE;
        return;
    }
    pmsdThis = (PMSD) pdnmsThis->data.ptrvalue;

    Cn3D_wKinSave = ModalWindow(-20, -13, -10, -10, NULL);

    /* set up a group encolosing structures - models selection lists and - "info strings" */
    Cn3D_pcKinName = StringSave(GetStrucStrings(pdnmsThis, PDB_ACC));
    sprintf(pcSavestr, "Save %s As a Kinemage Rendered File ...",
            Cn3D_pcKinName);
    g = HiddenGroup(Cn3D_wKinSave, 0, 5, NULL);
    SetGroupMargins(g, 10, 10);
    SetGroupSpacing(g, 10, 5);
    StaticPrompt(g, pcSavestr, 0, 0, systemFont, 'l');
    g2 = HiddenGroup(g, 2, 0, NULL);
    SetGroupMargins(g2, 10, 10);
    SetGroupSpacing(g2, 10, 5);
    StringNCpy(pcSavename, Cn3D_pcKinName, 8);
    StringCat(pcSavename, ".kin");
    Cn3D_tKinSave =
        DialogText(g2, pcSavename, 18, (TxtActnProc) Cn3D_KinEnableProc);
    MemFree(Cn3D_pcKinName);

    Cn3D_bKinBrowse =
        PushButton(g2, " browse...", (BtnActnProc) Cn3D_KinBrowseProc);
    g3 = HiddenGroup(g, 2, 0, NULL);
    gMS = Cn3D_ModelSelect(g3, FALSE); /* no vector models for Kin files saves */
    g4 = HiddenGroup(g3, 0, 2, NULL);
    SetGroupMargins(g4, 10, 10);
    SetGroupSpacing(g4, 10, 5);
    Cn3D_bKinOk = PushButton(g4, "OK", Cn3D_ExportKinNow);
    b = PushButton(g4, "Cancel", Cn3D_CancelKin);

    Cn3D_gColorAtoms =
        NormalGroup(g, 2, 2, " color by...", systemFont, NULL);
    SetGroupMargins(Cn3D_gColorAtoms, 10, 10);
    SetGroupSpacing(Cn3D_gColorAtoms, 10, 5);
    RadioButton(Cn3D_gColorAtoms, "Molecule number");
    RadioButton(Cn3D_gColorAtoms, "Secondary Structure & Residue Type");
    RadioButton(Cn3D_gColorAtoms, "Thermal Factors");
    RadioButton(Cn3D_gColorAtoms, "Element");
    SetValue(Cn3D_gColorAtoms, 1);

    Cn3D_gRenderOpts =
        NormalGroup(g, 2, 3, " viewable subsets...", systemFont, NULL);
    Cn3D_bRender[0] = CheckBox(Cn3D_gRenderOpts, "Virtual Backbone", NULL);
    Cn3D_bRender[1] = CheckBox(Cn3D_gRenderOpts, "Real Backbone", NULL);
    Cn3D_bRender[2] = CheckBox(Cn3D_gRenderOpts, "Residues", NULL);
    Cn3D_bRender[3] = CheckBox(Cn3D_gRenderOpts, "Heterogens", NULL);
    Cn3D_bRender[4] =
        CheckBox(Cn3D_gRenderOpts, "Alternate Conformations", NULL);
    Cn3D_bRender[5] = CheckBox(Cn3D_gRenderOpts, "NMR Animations", NULL);
    for (iCount = 0; iCount < 6; iCount++)
        SetStatus(Cn3D_bRender[iCount], TRUE);

    Cn3D_KinEnableProc(NULL);
    Select(Cn3D_bKinOk);
    /* disable appropriate stuff here */
    Cn3D_DisableFileOps();
    Show(Cn3D_wKinSave);

    return;
}

#ifndef _OPENGL
static void Cn3D_GifSaveProc(IteM i)
{
    Char fname[PATH_MAX];
    Char defname[32];
    PDNMS pdnmsThis = GetSelectedModelstruc();

    fname[0] = '\0';
    defname[0] = '\0';

    if (pdnmsThis == NULL)
        StringNCpy_0(defname,
                     pdnmsThis ? GetStrucStrings(pdnmsThis,
                                                 PDB_ACC) : "cn3d",
                     sizeof(defname) - 4);
    StringCat(defname, ".gif");

    if (GetOutputFileName(fname, sizeof(fname), defname)) {
        SaveImageGIF(Nlm_GetViewerImage3D(Cn3D_v3d), fname);
    }
}

#else

#if defined(_PNG)
static void Cn3D_PNGSaveProc(IteM i)
{
    Char fname[PATH_MAX];
    Char defname[32];
    PDNMS pdnmsThis = GetSelectedModelstruc();

    fname[0] = '\0';
    defname[0] = '\0';

    if (pdnmsThis == NULL)
        StringNCpy_0(defname,
                     pdnmsThis ? GetStrucStrings(pdnmsThis,
                                                 PDB_ACC) : "cn3d",
                     sizeof(defname) - 4);
    StringCat(defname, ".png");

    if (GetOutputFileName(fname, sizeof(fname), defname)) {
        Nlm_SaveImagePNG(fname);
    }
}
#endif /* _PNG */

TOGL_Data * Cn3D_GetCurrentOGLData(void)
{
    return Cn3D_ColorData.OGL_Data;
}


#endif /* _OPENGL */

MenU LIBCALL Cn3D_ExportSub(MenU m)
{
    IteM i;
    MenU s;

    s = SubMenu(m, "Export");
    i = CommandItem(s, "PDB File...", Cn3D_ExportPDB);
    i = CommandItem(s, "Kinemage File...", Cn3D_ExportKin);
#ifndef _OPENGL
    i = CommandItem(s, "GIF.../S", Cn3D_GifSaveProc);
#endif
#if defined(_OPENGL) && defined(_PNG)
    i = CommandItem(s, "PNG...", Cn3D_PNGSaveProc);
#endif
    return s;
}
