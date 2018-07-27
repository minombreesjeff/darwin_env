/*   cn3dmain.c
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
* File Name:  cn3dmain.c
*
* Author:  Christopher Hogue
*
* Version Creation Date:   1/31/96
*
* File Description: Main entry point for Cn3d
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: cn3dmain.c,v $
* Revision 6.57  2000/06/12 23:02:41  hurwitz
* enable launch of DDE from Cn3D, swith from DDV_ComputeRuler to DDE_ReMakeRuler, get rid of styles option for DDE
*
* Revision 6.56  2000/06/08 21:00:59  lewisg
* force dde into nogap mode
*
* Revision 6.55  2000/03/20 18:18:33  thiessen
* fixed header problem causing network unavailability
*
* Revision 6.54  2000/03/15 19:32:20  lewisg
* launch only single udv window
*
* Revision 6.53  2000/03/08 21:46:14  lewisg
* cn3d saves viewport, misc bugs
*
* Revision 6.52  2000/02/17 15:41:11  thiessen
* added CSC algorithm selection submenu
*
* Revision 6.51  2000/01/11 01:16:46  lewisg
* fix color selection in Cn3D, other misc. bugs
*
* Revision 6.50  2000/01/08 00:47:53  lewisg
* fixes to selection, update, color
*
* Revision 6.49  2000/01/04 15:55:50  lewisg
* don't hang on disconnected network and fix memory leak/hang at exit
*
* Revision 6.48  1999/11/22 14:46:41  thiessen
* moved _OPENGL code blocks to only vibrant and ncbicn3d libraries
*
* Revision 6.46  1999/11/10 23:19:40  lewisg
* rewrite of selection code for ddv
*
* Revision 6.45  1999/11/03 16:24:20  lewisg
* get rid of non-existing includes
*
* Revision 6.44  1999/10/29 14:15:27  thiessen
* ran all Cn3D source through GNU Indent to prettify
*
* Revision 6.43  1999/10/05 23:18:19  lewisg
* add ddv and udv to cn3d with memory management
*
* Revision 6.42  1999/09/21 18:09:16  lewisg
* binary search added to color manager, various bug fixes, etc.
*
* Revision 6.41  1999/09/21 13:59:22  thiessen
* port of Lewis's OpenGL code to X/Motif
*
* Revision 6.40  1999/09/16 17:16:20  ywang
* open multiple salsa window for data with multiple seq-annot data
*
* Revision 6.39  1999/08/04 21:18:01  lewisg
* modularized open operations to allow sequin to launch cn3d
*
* Revision 6.38  1999/07/02 22:11:24  ywang
* call EntrezInit besides EntrezBioseqFetchEnable
*
* Revision 6.37  1999/06/15 19:08:17  kans
* instantiate Cn3D_useEntrez in library
*
* Revision 6.36  1999/06/15 17:57:53  ywang
* rename useEntrez as Cn3D_useEntrez
*
* Revision 6.35  1999/06/14 17:38:43  ywang
* remove 'static' on Cn3D_useEntrez
*
* Revision 6.34  1999/05/10 21:50:30  chappey
* bug in SeqEdDownload
*
* Revision 6.32  1999/04/27 20:47:37  lewisg
* add cn3d gif batch mode to cn3d
*
* Revision 6.31  1999/02/10 23:49:42  lewisg
* use RGB values instead of indexed palette
*
* Revision 6.30  1999/01/20 18:21:19  ywang
* include salmedia.h due to the move around of MediaInfo from cn3dmsg.h to the new created salmedia.h
*
* Revision 6.29  1999/01/14 19:07:16  kans
* network availability is configurable
*
* Revision 6.28  1998/12/08 16:53:09  kans
* new params to ShowNetConfigForm to simplify configuration
*
* Revision 6.27  1998/08/03 18:33:12  lewisg
* added netentcf to cn3d
*
* Revision 6.26  1998/06/17 22:41:26  kans
* now uses UseLocalAsnloadDataAndErrMsg from sqnutils.h
*
* Revision 6.25  1998/06/15 14:26:06  ywang
* automatic launch salsa when mime data got in either through command line or via local reading in
*
 * Revision 6.24  1998/06/11  20:07:38  chappey
 * Added SetAppProperty ("SeqEditDisplayForm") to pass flags to sequence editor
 *
* Revision 6.23  1998/06/05 21:14:22  ywang
* A solution for ensure getting bsp
*
 * Revision 6.22  1998/06/04  16:48:34  ywang
 * fix bug triggered by automatic salsa launch
 *
 * Revision 6.20  1998/05/06  23:50:21  lewisg
 * fixed launching problem with sequin
 *
* Revision 6.19  1998/04/28 19:39:58  lewisg
* codewarrior fixes
*
* Revision 6.18  1998/04/28 19:38:35  lewisg
* codewarrior fixes
*
* Revision 6.17  1998/04/28 18:54:01  ywang
* slight modification
*
 * Revision 6.15  1998/04/28  15:14:25  lewisg
 * moved OpenMimeFileWithDeletion to cn3dopen
 *
* Revision 6.14  1998/04/27 23:23:02  lewisg
* added ability to open mime files
*
* Revision 6.13  1998/04/21 23:00:56  lewisg
* added show aligned/unaligned
*
* Revision 6.12  1998/04/15 00:51:36  lewisg
* bug fixes for multiple alignment mode and alignment pane
*
* Revision 6.11  1998/04/04 18:07:45  lewisg
* get rid of typo
*
* Revision 6.10  1998/04/04 05:57:52  lewisg
* got rid of dos line breaks
*
* Revision 6.9  1998/04/04 00:53:45  lewisg
* added support for multiple alignments
*
* Revision 6.8  1998/04/01 23:26:16  lewisg
* added new startup mode + fixed slave rendering
*
* Revision 6.7  1998/03/26 22:42:10  lewisg
* added seqentry and seq annot to msd
*
* Revision 6.6  1998/03/13 22:30:34  lewisg
* fix neighbor mode
*
* Revision 6.5  1998/03/07 20:43:51  kans
* moved Cn3D_fEntrezOn to cn3dwin.c
*
* Revision 6.4  1998/03/06 23:19:14  lewisg
* codewarrior fixes
*
* Revision 6.3  1998/03/06 01:19:32  lewisg
* merge
*
* Revision 6.2  1997/10/09 13:01:54  epstein
*  add return values for OpenMimeFile
*
* Revision 6.1  1997/09/30 20:09:21  epstein
* ADD ABILITY TO PERFORM cN3d demos driven from configuration files
*
* Revision 6.0  1997/08/25 18:13:31  madden
* Revision changed to 6.0
*
* Revision 5.20  1997/07/16 20:55:48  vakatov
* Use "Nlm_GetArg[vc]()" instead of "arg[vc]"
*
* Revision 5.19  1997/03/20 19:04:08  vakatov
* Now contains only standalone-specific code;  the generic Cn3D code has
* been moved to "cn3dwin.c", and the Entrez-specific code -- to "cn3dentr.c".
*
*
* ==========================================================================
*/

#include <ncbi.h>
#include <vibrant.h>            /* for netentcf */
#include <netcnfg.h>
#include <cn3dmain.h>
#include <objmime.h>
#include <accentr.h>
#include <accutils.h>
#include <objalign.h>
#include <objseq.h>
#include <objmgr.h>
#include <sequtil.h>
#include <saledit.h>
#include <lsqfetch.h>
#include <cn3dopen.h>
#include <salmedia.h>
#include <sqnutils.h>
#include <ddvmain.h>

/* note - this module shouldn't include any headers that have #ifdef _OPENGL blocks! */

extern void Cn3D_SaveActiveCam(void);
extern Nlm_Boolean Nlm_Call_SetPosition3D(Nlm_RecT *);
extern Nlm_Boolean Nlm_Call_SaveImageGIF(CharPtr);

static Boolean LIBCALLBACK OpenMimeFile(CharPtr filename)
{
/*#ifdef OS_MAC
    /* the Web browsers on other platforms get upset if you delete the file, */
    /* but apparently on the Mac you must delete it */
    /*return OpenMimeFileWithDeletion(filename, TRUE);
#else */
    return OpenMimeFileWithDeletion(filename, FALSE);
/*#endif */
}

/******* SEQUENCE EDITOR *********/
static SeqEditViewProcs seqedprocs;
/******** END ************/

static void ConfigAccepted(void)
 {
    SetAppParam("CN3D", "SETTINGS", "NETWORKAVAILABLE", "TRUE");
    Message(MSG_OK, "Setting will take affect when you restart Cn3D");
}

static void ConfigCancelled(void)
{
}

static void ConfigTurnedOff(void)
 {
    SetAppParam("CN3D", "SETTINGS", "NETWORKAVAILABLE", "FALSE");
    Message(MSG_OK, "Setting will take affect when you restart Cn3D");
}

static void NetConfigureProc(IteM i)
 {
    Boolean netCurrentlyOn = FALSE;

    if (Cn3D_UsingEntrez())
        ShowNetConfigForm(NULL, NULL, ConfigAccepted, ConfigCancelled, ConfigTurnedOff, TRUE);
    else
        ShowNetConfigForm(NULL, NULL, ConfigAccepted, ConfigCancelled, ConfigTurnedOff, FALSE);
}


static void Cn3D_PrintImage(Char * str)
/* this is only used in batch mode to create a gifs of the structure */
{
    BiostrucPtr pbsBiostruc = NULL;
    PDNMS pdnmsModelstruc = NULL;
    PMSD pmsdThis = NULL;
    Int4 MdlNo, MdlLvl;
    PDNML pdnmlThis = NULL;
    PDNML pdnmlFirst = NULL;
    PMLD pmldThis = NULL;
    PMLD pmldOne = NULL;
    PMLD pmldAll = NULL;
    PMLD pmldVec = NULL;

    SetNeighborOff();           /* we don't load in neighbors thru this route */
    Cn3D_SaveActiveCam();
    ClearStructures();

    MdlLvl = ALLSIMPLEMDL;
    MdlNo = 1;

    pbsBiostruc = FetchBS(str, INP_GI, MdlLvl, MdlNo, GetMMDBAPIbExtent());
    if (pbsBiostruc != NULL) {
        pdnmsModelstruc = MakeAModelstruc(pbsBiostruc);
    }
    if (pdnmsModelstruc == NULL) {
        /* return a not found error here */
        return;
    }
    /* turn off backbone model if "All" models present */
    pmsdThis = (PMSD) pdnmsModelstruc->data.ptrvalue;
    pdnmlThis = pmsdThis->pdnmlModels;
    /* set up for doing one model or animation */
    while (pdnmlThis) {
        pmldThis = (PMLD) pdnmlThis->data.ptrvalue;
        if (pmldThis->iType == Model_type_ncbi_vector)
            pmldVec = pmldThis;
        if (pmldThis->iType == Model_type_ncbi_backbone)
            pmldOne = pmldThis;
        if (pmldThis->iType == Model_type_ncbi_all_atom)
            pmldAll = pmldThis;
        pdnmlThis = pdnmlThis->next;
    }
    if ((pmldOne != NULL) && (pmldAll != NULL))
        pmldOne->bSelected &= (Byte) 0xFE;

    Cn3D_ResetActiveStrucProc();
    Cn3D_Redraw(TRUE);          /* always a new structure */

    return;
}



Int2 Main(void)
{
    char buffer[PATH_MAX];
    WindoW www;
    Boolean netCurrentlyOn = FALSE;
    ObjMgrPtr omp;
    Boolean NoGaps;


    ErrSetFatalLevel(SEV_MAX);

    UseLocalAsnloadDataAndErrMsg();
    if (!OpenMMDBAPI(0, NULL)) {
        /* make user hit OK, so has time to read MMDBAPI's error postings */
        Message(MSG_ERROR, "Unable to open MMDB-API");
        return 1;
    }

    SeqAsnLoad();

#ifdef WIN_MAC
    DeskAccGroup(AppleMenu(NULL));
#endif

    /* increment maximum # of objects held in memory */
    omp = ObjMgrWriteLock();
    if (omp != NULL)
        omp->maxtemp = DDV_MAXTEMP;
    ObjMgrUnlock();

    /*register the sequence and alignment viewers */
    REGISTER_UDV_AUTONOMOUS;    /* udv */
    REG_DDV_SLA_VIEW;           /* ddv */
    REG_DDV_SLA_EDIT;           /* dde */
    REGISTER_UDV_SEQENTRY;      /* udv launch seqentry */

    /* force DDE to not allow gaps in alignments */
    NoGaps = TRUE;
    SetAppProperty("dde_nogaps",(void*)&NoGaps);

    Cn3D_CheckNetworkUse();

    www = Cn3DWin(NULL, NULL, NetConfigureProc, TRUE);
    if (www) {
        if (GetAppParam
            ("Cn3D", "demo", "mandatory_file", "", buffer,
             sizeof(buffer)) > 0) {
            OpenMimeFileWithDeletion(buffer, FALSE);
        } else {
#if defined(OS_UNIX) || defined(WIN_MSWIN)
            if (GetArgc() == 2)
                OpenMimeFile(GetArgv()[1]);
            else if (GetArgc() == 4) {
                /*
                   runs Cn3D in batch mode to produce gifs.  Arguments are mmdb id, width, and height.
                   gifs are saved with a file name constructed using the mmdb id.
                 */
                CharPtr defname;
                Nlm_RecT rect;

                rect.left = 0;
                rect.right = atoi(GetArgv()[2]);
                rect.top = 0;
                rect.bottom = atoi(GetArgv()[3]);
                if (rect.right < 1 || rect.bottom < 1)
                    return 0;

                defname = StringSave(GetArgv()[1]);
                if (atoi(defname) < 1)
                    return 0;
                Cn3D_PrintImage(defname);
                StringCat(defname, ".gif");
                Show(www);
                Cn3D_Redraw(TRUE);
                Nlm_Call_SetPosition3D(&rect);
                Update();
                Nlm_Call_SaveImageGIF(defname);
                return 0;
            }
            else {
                if (GetAppParam
                    ("Cn3D", "demo", "optional_file", "", buffer,
                     sizeof(buffer)) > 0) {
                    OpenMimeFileWithDeletion(buffer, FALSE);
                }
            }
#endif
#if defined(WIN_MAC)  ||  defined(WIN_MSWIN)
            RegisterDropProc(OpenMimeFile);
#endif
        }

        Show(www);
        ProcessEvents();
    }

    CloseMMDBAPI();


    /* to do: delete OGL global information in viewer3D.  lyg */
    if (Cn3D_EntrezOn()) {
        EntrezBioseqFetchDisable();
        EntrezFini();
    }

    return 0;
}
