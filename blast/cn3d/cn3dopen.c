/*   cn3dopen.c
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
* File Name:  cn3dopen.c
*
* Author:  Christopher Hogue, Yanli Wang, Lewis Geer
*
* First Version Creation Date:   1/31/96
*
* $Revision: 6.112 $
*
* File Description: Cn3d file opening routines 
*                   
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
* $Log: cn3dopen.c,v $
* Revision 6.112  2000/07/24 22:30:20  thiessen
* fix header conflict
*
* Revision 6.111  2000/07/21 18:55:14  thiessen
* allow dynamic slave->master transformation
*
* Revision 6.110  2000/06/16 14:57:03  lewisg
* move entrez calls out of desktop
*
* Revision 6.109  2000/05/16 20:24:23  thiessen
* don't display alignseq
*
* Revision 6.108  2000/05/16 18:13:32  lewisg
* fix cdd load bug
*
* Revision 6.107  2000/05/16 12:44:55  lewisg
* fix coloring bug for strucseqs
*
* Revision 6.106  2000/05/16 11:08:44  wheelan
* changed sap to salp in SAM_ReplaceGI call at line 505
*
* Revision 6.105  2000/05/15 23:39:34  lewisg
* shred cblast, add menu items for gapped/ungapped, fix pdbheaders
*
* Revision 6.104  2000/04/20 23:27:45  lewisg
* misc bug fixes
*
* Revision 6.103  2000/04/11 20:05:39  lewisg
* fix freeing bug on bioseq import, misc. tweaks
*
* Revision 6.102  2000/04/10 20:33:40  lewisg
* fix show/hide for blast multiple, make blast multiple API generic
*
* Revision 6.101  2000/04/08 00:37:30  lewisg
* multiple seqentries, NEWSEQ message, etc.
*
* Revision 6.100  2000/04/07 17:48:39  thiessen
* make 'identity' color default for strucseqs
*
* Revision 6.99  2000/04/04 22:18:42  lewisg
* add defline to ddv, fix seq import bugs, set boundbox
*
* Revision 6.98  2000/04/04 17:51:54  lewisg
* fix various seq import bugs
*
* Revision 6.97  2000/04/03 21:05:53  lewisg
* fix launch of cn3d from sequin
*
* Revision 6.96  2000/04/03 14:56:22  lewisg
* fix sequence import hang
*
* Revision 6.95  2000/03/28 21:18:54  lewisg
* more bug fixes for strucseqs
*
* Revision 6.94  2000/03/28 19:28:13  lewisg
* prep new imported sa
*
* Revision 6.93  2000/03/28 16:23:04  thiessen
* store row number in MMD after BLAST import
*
* Revision 6.92  2000/03/27 22:15:04  lewisg
* add show/hide row dialog
*
* Revision 6.91  2000/03/27 17:15:42  thiessen
* fix layer display bug
*
* Revision 6.90  2000/03/24 20:34:56  lewisg
* add blast from file, bug fixes, get rid of redundant code, etc.
*
* Revision 6.89  2000/03/23 21:01:07  thiessen
* add row number (1) to structure of strucseqs mime type
*
* Revision 6.88  2000/03/22 23:17:49  thiessen
* added ability to save ARS in ASN1
*
* Revision 6.87  2000/03/20 18:18:33  thiessen
* fixed header problem causing network unavailability
*
* Revision 6.86  2000/03/18 22:39:50  lewisg
* copy blast3 headers in makeall.unx, add blast3 to entrez link
*
* Revision 6.84  2000/03/18 00:06:00  lewisg
* add blast, new help, new menus
*
* Revision 6.83  2000/03/15 04:43:55  thiessen
* various minor fixes
*
* Revision 6.82  2000/03/14 18:03:13  thiessen
* add target row to MMD; remove rowmgr
*
* Revision 6.81  2000/03/13 22:03:21  lewisg
* add globalpars to network biostruc open
*
* Revision 6.80  2000/03/09 20:32:26  lewisg
* fix saving strucseq
*
* Revision 6.79  2000/03/09 17:56:58  thiessen
* changes to palette handling, feature implementation, PARS storage
*
* Revision 6.78  2000/03/08 21:46:14  lewisg
* cn3d saves viewport, misc bugs
*
* Revision 6.77  2000/03/02 21:11:06  lewisg
* use bandalign for import sequence, make standalone ddv use viewmgr, make dialogs modal, send color update
*
* Revision 6.76  2000/03/01 22:49:41  lewisg
* import bioseq, neatlyindex, get rid of dead code
*
* Revision 6.75  2000/03/01 16:17:55  thiessen
* improved handling of colors; many small fixes
*
* Revision 6.74  2000/02/19 21:25:57  thiessen
* split of cn3dmodl into cn3dmodl and cn3dstyl
*
* Revision 6.73  2000/02/15 22:40:57  lewisg
* add ability to launch udv so that it colors by row, fixes to colormgr, track rows from viewmgr, fix visual c projects
*
* Revision 6.72  2000/02/10 17:47:02  thiessen
* added: color-by-sequence-conservation menu item, zoom-out to OpenGL, misc fixes
*
* Revision 6.71  2000/02/05 01:32:21  lewisg
* add viewmgr, move place freeing is done in ddv, modify visual c++ projects
*
* Revision 6.70  2000/01/21 15:59:05  lewisg
* add check for binary/ascii files
*
* Revision 6.69  2000/01/14 21:40:41  lewisg
* add translucent spheres, ion labels, new cpk, fix misc bugs
*
* Revision 6.68  2000/01/04 15:55:51  lewisg
* don't hang on disconnected network and fix memory leak/hang at exit
*
* Revision 6.67  1999/12/28 23:06:35  lewisg
* udv/cn3d communication
*
* Revision 6.66  1999/12/28 15:55:21  lewisg
* remove remaining mediainfo code
*
* Revision 6.65  1999/12/15 23:17:47  lewisg
* make cn3d launch udv, fix launching of non-autonomous udv, add mouseup message
*
* Revision 6.64  1999/12/13 23:20:44  lewisg
* bug fixes: duplicate color structures eliminated, clear color doesn't clear case
*
* Revision 6.63  1999/12/11 01:30:35  lewisg
* fix bugs with sharing colors between ddv and cn3d
*
* Revision 6.62  1999/12/01 16:15:54  lewisg
* interim checkin to fix blocking memory leak
*
* Revision 6.61  1999/11/24 15:23:19  lewisg
* added color selection dialogs for SS
*
* Revision 6.60  1999/11/15 18:30:08  lewisg
* get rid of extra redraws when selecting
*
* Revision 6.59  1999/11/10 23:19:41  lewisg
* rewrite of selection code for ddv
*
* Revision 6.58  1999/11/02 23:06:07  lewisg
* fix cn3d to launch correctly if there is no seqentry associated with bioseq
*
* Revision 6.57  1999/10/29 14:15:30  thiessen
* ran all Cn3D source through GNU Indent to prettify
*
* Revision 6.56  1999/10/18 16:01:09  lewisg
* fix bssp to bsssp
*
* Revision 6.55  1999/10/18 15:32:50  lewisg
* move ClearSequences() to cn3dshim.c
*
* Revision 6.54  1999/10/15 20:56:39  lewisg
* append DDV_ColorGlobal as userdata.  free memory when cn3d terminates.
*
* Revision 6.53  1999/10/05 23:18:24  lewisg
* add ddv and udv to cn3d with memory management
*
* Revision 6.52  1999/09/16 17:16:21  ywang
* open multiple salsa window for data with multiple seq-annot data
*
* Revision 6.51  1999/08/04 21:18:01  lewisg
* modularized open operations to allow sequin to launch cn3d
*
* Revision 6.50  1999/07/12 19:37:44  ywang
* fix bug
*
* Revision 6.49  1999/07/07 20:45:37  ywang
* clear domaindata, mediadata, special feature before reading in new data in cn3d
*
* Revision 6.48  1999/07/07 16:41:30  ywang
* reset Num_Bioseq as 0 when sequences are freed
*
* Revision 6.47  1999/07/06 18:30:45  chappey
* Send FLUSH message on each Bioseq to close windows
*
* Revision 6.46  1999/07/06 17:18:38  ywang
* send OM_MSG_FLUSH message to salsa to close all its windows before new sequence window launched
*
* Revision 6.45  1999/07/02 20:58:37  ywang
*  return NULL sep upon NULL pmsdThis in Cn3DFetchSeqEntry
*
* Revision 6.44  1999/07/02 19:57:43  ywang
* go through every node of SeqEntryPtr for free
*
* Revision 6.43  1999/07/02 14:39:14  ywang
* clean seq-entry fetching and OM register for netopen and local read(non mimetype) functioning
*
* Revision 6.42  1999/07/01 22:14:58  ywang
* back to previous version to use EntrezSeqEntryGet to get sequences for additional loading, before this free existant sequences
*
* Revision 6.41  1999/07/01 22:08:08  lewisg
* add resize before all redraws
*
* Revision 6.40  1999/07/01 21:44:30  ywang
* work around EntrezSeqEntryGet core dump on redunant sequence loading and free existant sequences before additional loading
*
* Revision 6.39  1999/07/01 14:07:25  ywang
* cn3dwin.c
*
* Revision 6.38  1999/06/15 19:08:17  kans
* instantiate Cn3D_useEntrez in library
*
* Revision 6.37  1999/06/15 17:57:54  ywang
* rename useEntrez as Cn3D_useEntrez
*
* Revision 6.36  1999/06/14 17:40:26  ywang
* assign mimetype to structure data read in locally or via network with respect to availability of sequences
*
* Revision 6.35  1999/04/06 20:14:09  lewisg
* more opengl
*
* Revision 6.34  1999/03/30 22:36:19  ywang
* add functions to color salsa for NcbiMimeAsn1_strucseqs & code reorganization
*
* Revision 6.33  1999/03/18 22:28:58  ywang
* add functions for saveout+readin+index user defined features
*
* Revision 6.32  1999/03/03 23:17:22  lewisg
* one master struct at a time, list slaves in structure info, bug fixes
*
* Revision 6.31  1999/02/25 23:14:31  ywang
* move around menu item and callback function, change menu item names
*
* Revision 6.30  1999/02/24 23:00:49  ywang
* record mime type at MSD node
*
* Revision 6.29  1999/01/20 18:21:20  ywang
* include salmedia.h due to the move around of MediaInfo from cn3dmsg.h to the new created salmedia.h
*
* Revision 6.28  1999/01/19 23:42:48  ywang
* fix bugs over improving color msg
*
* Revision 6.27  1999/01/14 19:07:17  kans
* network availability is configurable
*
* Revision 6.26  1998/12/22 15:40:32  ywang
* restore sequences pointer for strucseq
*
* Revision 6.25  1998/12/21  18:47:37  addess
* fixed strucseq bug found by Aron
*
* Revision 6.24  1998/12/16  17:02:57  ywang
* pick up strucseqs mime type
*
* Revision 6.23  1998/10/21  15:49:21  ywang
* attach the whole vast alignment data to master structure
*
* Revision 6.22  1998/09/23  18:38:50  ywang
* add functions to control display on domain level
*
* Revision 6.21  1998/06/30  23:29:22  ywang
* fix bugs regarding to read in more structures
*
* Revision 6.20  1998/06/30  14:48:04  ywang
* launch salsa automatically and accordingly when more structures are readin
*
* Revision 6.19  1998/06/29  19:28:00  lewisg
* on the fly update of conservation color
*
* Revision 6.17  1998/06/15 14:26:06  ywang
* automatic launch salsa when mime data got in either through command line or via local reading in
*
* Revision 6.16  1998/06/12  21:21:56  ywang
* change sCompare to make cn3d accept mime strucseq from local readin
*
* Revision 6.15  1998/06/04  16:48:36  ywang
* fix bug triggered by automatic salsa launch
*
* Revision 6.14  1998/05/27  22:15:16  ywang
* add Cn3dObjRegister() to several places
*
* Revision 6.13  1998/05/18  22:09:14  ywang
* move codes around
*
* Revision 6.12  1998/05/12  21:47:05  lewisg
* stricter conservation coloring
*
* Revision 6.11  1998/05/12 16:34:01  ywang
* take strucseq mime data
*
* Revision 6.10  1998/05/06  23:50:24  lewisg
* fixed launching problem with sequin
*
* Revision 6.9  1998/05/06 14:20:09  lewisg
* get rid of NULL structure bugs
*
* Revision 6.8  1998/04/28 22:47:21  lewisg
* master/slave color in sync
*
* Revision 6.7  1998/04/28 19:40:01  lewisg
* codewarrior fixes
*
* Revision 6.6  1998/04/28 19:38:39  lewisg
* codewarrior fixes
*
* Revision 6.5  1998/04/28 18:53:25  ywang
* take NcbiMimeAsn1_alignseq to view alignment only
*
* Revision 6.4  1998/04/28  15:20:37  kans
* cast needed on FileGets for CodeWarrior
*
* Revision 6.3  1998/04/28 15:14:29  lewisg
* moved OpenMimeFileWithDeletion to cn3dopen
*
* Revision 6.2  1998/04/27 23:23:06  lewisg
* added ability to open mime files
*
* Revision 6.1  1998/04/16 00:32:25  lewisg
* corrected neighbor mode bugs
*
* Revision 6.0  1997/08/25 18:13:38  madden
* Revision changed to 6.0
*
* Revision 5.0  1996/05/28 14:05:44  ostell
* Set to revision 5.0
*
* Revision 1.5  1996/04/18  17:01:10  hogue
* Fixed calls to FetchBS for later integration into Entrez, added feature to turn off redundant backbone model for faster spinning...
*
* Revision 1.4  1996/03/30  23:41:01  hogue
* Redraw now saves camera
*
* Revision 1.3  1996/03/29  20:01:24  hogue
* Added call to reset active structure upon opening.
*
* Revision 1.2  1996/02/02  19:40:19  hogue
* Initial Revision
*
*
* ==========================================================================
*/

#include <vibrant.h>
#include <math.h>
#include <mmdbapi.h>
#include <accentr.h>
#include <objalign.h>
#include <objseq.h>
#include <objmgr.h>
#include <sequtil.h>
#include <saledit.h>
#include <lsqfetch.h>
#include <cn3dopen.h>
#include <cn3dmain.h>
#include <algorend.h>
#include <cn3dmsg.h>
#include <salmedia.h>
#include <cn3dshim.h>
#include <cn3dmodl.h>
#include <cn3dstyl.h>
#include <alignmgr.h>
#include <sqnutils.h>
#include <cn3dsave.h>
#include <ddvopen.h>
#include <seqcons.h>
#include <udviewer.h>
#include <tofasta.h>
#include <accutils.h>


static Boolean Cn3D_Open_InUse = FALSE;
static WindoW Cn3D_wNetOpen;
static TexT Cn3D_tOpen;
static GrouP Cn3D_gAccType;
static ButtoN Cn3D_bOpenAccept;
static GrouP Cn3D_gMdlLvl;
static GrouP Cn3D_gMdlNo;

static WindoW Cn3D_wOpen;
static ButtoN Cn3D_bOpenBrowse;

Boolean Mime_ReadIn = FALSE;

Int2 Cn3DMime = 0;


/*****************************************************************************

Function: Cn3D_UseNetwork()

Purpose:  Sets Cn3D_ColorData.UseEntrez, if Cn3D should use the network
  
Returns:  TRUE if yes

*****************************************************************************/

NLM_EXTERN Boolean Cn3D_CheckNetworkUse()
{
    Char str[64];
    Boolean useNetwork = FALSE;

    if (GetAppParam
        ("CN3D", "SETTINGS", "NETWORKAVAILABLE", NULL, str, sizeof(str))) {
        if (StringICmp(str, "TRUE") == 0) useNetwork = TRUE;
    }
    Cn3D_ColorData.UseEntrez = useNetwork;
    Cn3D_ColorData.EntrezOn = FALSE;
    return useNetwork;
}

/*------------------------------------------------------*/
Boolean Cn3D_UsingEntrez(void)
{
    return Cn3D_ColorData.UseEntrez;
}

/*------------------------------------------------------*/
Boolean Cn3D_EntrezOn(void)
{
    return Cn3D_ColorData.EntrezOn;
}

/*------------------------------------------------------*/
SeqEntryPtr Cn3DFetchSeqEntry(PMSD pmsdThis)
{
    PDNMM pdnmmHead = NULL;
    PMMD pmmdThis = NULL;
    SeqId si;
    Bioseq *bsp;

    if (pmsdThis == NULL)
        return NULL;

    pdnmmHead = pmsdThis->pdnmmHead;
    while (pdnmmHead) {
        pmmdThis = pdnmmHead->data.ptrvalue;
        if (pmmdThis == NULL)
            goto errot;
        if (pmmdThis->bWhat != (Byte) AM_PROT
            && pmmdThis->bWhat != (Byte) AM_RNA
            && pmmdThis->bWhat != (Byte) AM_DNA)
            goto errot;
        if (pmmdThis->iGi != 0) {
            si.choice = SEQID_GI;
            si.data.intvalue = pmmdThis->iGi;
            bsp = BioseqLockById(&si);
            BioseqUnlock(bsp);

            if (bsp != NULL)
                return SeqEntryFind(&si);
        }

      errot:
        pdnmmHead = pdnmmHead->next;
    }

    return NULL;
}

static SeqAnnot * Cn3D_ShredCBlast(SeqAnnot *sap)
{
    SeqAlign *salp;
    SeqAnnot *sapout, *saptmp;

    if (sap == NULL) return NULL;
    
    saptmp = sap;
    while (saptmp) {
        if (saptmp->type == 2) {
            salp = saptmp->data;
            break;
        }
        saptmp = saptmp->next;
    }
    
    if (salp) {
        if(am_guess_numrows(salp) > 2) return sap;
        SAM_ReplaceGI(salp);
        AlnMgrIndexSeqAlign(salp);
        AlnMgrMakeMultipleByScore(salp);
        AlnMgrDeleteHidden(salp, FALSE);
        sapout = SeqAnnotNew();
        sapout->data = DDV_ShredAln(salp);
        if(sapout->data == NULL) return sap;
        sapout->type = 2;
        return sapout;
    } else return NULL;
}


/*
sets alignment bits in master structure given pairwise seqaligns
*/
static void Cn3D_MarkStrucSeqs(PDNMS pdnmsThis, SeqAnnot *sap)
{
    PMSD pmsdThis = NULL;
    PMMD pmmdThis = NULL;
    PMGD pmgdThis = NULL;
    SeqIdPtr sip = NULL;
    SeqAlignPtr salp = NULL;
    DenseSegPtr dssp = NULL;
    DenseDiagPtr ddp = NULL;
    Int4 nres, numseg;

    if (sap == NULL || pdnmsThis == NULL) return;
    pmsdThis = pdnmsThis->data.ptrvalue;
    if (pmsdThis == NULL) return;

    while (sap) {
        if (sap->type == 2) {
            salp = sap->data;
            break;
        }
        sap = sap->next;
    }

    if (salp) {
        if(salp->segtype == SAS_DENDIAG) {
            ddp = salp->segs;
            sip = ddp->id;
        }
        else if(salp->segtype == SAS_DENSEG) {
            dssp = salp->segs;
            sip = dssp->ids;
        }
        else return;
        pmmdThis = GetMMFromMSDBySeqId(pmsdThis, sip);
        /* assume the first sip is for the sequence with known structure */
    } else return;

    if (pmmdThis == NULL) return;

    while (salp) {
        pmsdThis->bAligned++;
        if(salp->segtype == SAS_DENSEG) {
            dssp = salp->segs;
            if(dssp->dim != 2) continue;
            
            for (numseg = 0; numseg < dssp->numseg; numseg++) {
                if (dssp->starts[numseg*2] == -1  
                    || dssp->starts[numseg*2+1] == -1 ) continue;
                for (nres = dssp->starts[numseg*2];
                        nres < dssp->starts[numseg*2] + dssp->lens[numseg];
                        nres++) {
                    pmgdThis = GetMGFromMM(pmmdThis, nres + 1);
                    if (pmgdThis) pmgdThis->bReserved++;
                }
            }
        }
        else /* densediag */ {
            ddp = salp->segs;
            if(ddp->dim != 2) continue;
            if (ddp->starts[0] != -1 && ddp->starts[1] != -1) {
                for (nres = ddp->starts[0]; nres < ddp->starts[0] + ddp->len;
                nres++) {
                    pmgdThis = GetMGFromMM(pmmdThis, nres + 1);
                    if (pmgdThis) pmgdThis->bReserved++;
                }
            }
        }
        salp = salp->next;
    }
}


/**********
fnMarkAlignedResidue()
Given a master and model, mark all of the residues in the slave and master that are aligned.
**********/

ValNodePtr fnMarkAlignedResidues(PDNMS pdnmsMaster, PDNMS pdnmsSlave,
                                 BiostrucFeaturePtr pbsfThis)
{

    ValNodePtr pvnAlignment;
    ValNodePtr pvnThis = NULL;
    ChemGraphPntrsPtr pcgpThis;
    ValNodePtr pvnListMaster = NULL, pvnListSlave = NULL,
        pvnListMasterHead, pvnListSlaveHead;
    PFB pfbMaster = NULL, pfbSlave = NULL;
    ChemGraphAlignmentPtr pcgaSlave;


    /* find the corresponding alignment */
    pvnAlignment =
        ValNodeFindNext(pbsfThis->Location_location, NULL,
                        Location_location_alignment);
    if (pvnAlignment == NULL)
        return NULL;


    pcgaSlave = pvnAlignment->data.ptrvalue;
    pvnThis = pcgaSlave->alignment;
    if (pvnThis) {
        pcgpThis = (ChemGraphPntrsPtr) pvnThis;
        pvnListMasterHead = pvnListMaster = 
            MakeChemGraphNodeList(pdnmsMaster, pcgpThis);
        pcgpThis = (ChemGraphPntrsPtr) pvnThis->next;
        pvnListSlaveHead = pvnListSlave = 
            MakeChemGraphNodeList(pdnmsSlave, pcgpThis);
        if (!pvnListMaster || !pvnListSlave)
            return NULL;
        while (pvnListMaster && pvnListSlave) {
            pfbMaster = (PFB) pvnListMaster->data.ptrvalue;
            pfbMaster->bReserved++;
            pfbSlave = (PFB) pvnListSlave->data.ptrvalue;
            pfbSlave->bReserved++;
            ((PMGD) pfbSlave)->pbMasterReserved = &(pfbMaster->bReserved); /* use unused feature pointer to point slave back at master */
            pvnListSlave = pvnListSlave->next;
            pvnListMaster = pvnListMaster->next;
        }                       /* while pvnListMaster */
        ValNodeFree(pvnListMasterHead);
        ValNodeFree(pvnListSlaveHead);
    }                           /* while pvnThis */
    return pvnAlignment;
}

/************
fnClearMarkedResidues
callback function used to clear all of the alignment pointers and counters used for protein conservation calculations
************/

void LIBCALLBACK fnClearMarkedResidues(PFB pfbThis, Int4 iModel,
                                       Int4 iIndex, Pointer ptr)
{
    PMGD pmgdThis;

    pfbThis->bReserved = 0;
    pmgdThis = (PMGD) pfbThis;
    pmgdThis->pbMasterReserved = NULL;
}

/******************************************************************************
*
* Traverses the given MSD and sets it up for displaying one model or animation
* Turns off backbone model if all atoms is loaded in?
*
******************************************************************************/

NLM_EXTERN void MMDB_OpenTraverse(PMSD pmsd)
{
    PDNML pdnmlThis = NULL;
    PMLD pmldThis = NULL;
    PMLD pmldOne = NULL;
    PMLD pmldAll = NULL;

    pdnmlThis = pmsd->pdnmlModels;
    /* set up for doing one model or animation */
    while (pdnmlThis) {
        pmldThis = (PMLD) pdnmlThis->data.ptrvalue;
        if (pmldThis->iType == Model_type_ncbi_backbone)
            pmldOne = pmldThis;
        if (pmldThis->iType == Model_type_ncbi_all_atom)
            pmldAll = pmldThis;
        pdnmlThis = pdnmlThis->next;
    }

    if (pmldOne && pmldAll)
        pmldOne->bSelected &= (Byte) 0xFE;
}

/******************************************************************************
*
* Sets up Cn3D for reading in a new structure and any potential alignments
* and bioseqs.
*
******************************************************************************/

NLM_EXTERN void Cn3D_OpenStart()
{
    Int4 iBioseq = 0;

#ifndef _OPENGL
    Cn3D_SaveActiveCam();
#endif

    ObjMgrDeSelectAll();
    Mime_ReadIn = FALSE;

    ClearSequences();
    ClearStructures();
    ClearRest();
}

NLM_EXTERN Boolean Cn3D_StartNet(Boolean UseNetwork)
{
    if (Cn3D_ColorData.EntrezOn) return TRUE;
    if (Cn3D_ColorData.UseEntrez) {
        if(EntrezInit("Cn3D", TRUE, NULL)) {
            if(EntrezBioseqFetchEnable("Cn3D", FALSE)) {
                Cn3D_ColorData.EntrezOn = TRUE;
                return TRUE;
            }
        }
    }
    return FALSE;
}

/******************************************************************************
*
* Finishes setting up Cn3D. Called after reading in a new structure and any
* alignments and bioseqs.  If necessary, will load in a sequence via
* netentrez if a network connection is open.
*
******************************************************************************/

/* call back to replace gi's with full seqid's */
static void LIBCALLBACK fnRewireSips(PFB pfbThis, Int4 iModel,
                                       Int4 iIndex, Pointer ptr)
{
    PMMD pmmdThis;
    Bioseq *bsp;
    SeqId *sip;
    
    pmmdThis = (PMMD) pfbThis;
    if(pmmdThis->pSeqId == NULL) return;
    bsp = BioseqLockById(pmmdThis->pSeqId);
    if(bsp == NULL) return;
    sip = SeqIdDupList(bsp->id);
    BioseqUnlock(bsp);
    if(sip == NULL) {
        return;
    }
    SeqIdFree(pmmdThis->pSeqId);
    pmmdThis->pSeqId = sip;
    return;
}


NLM_EXTERN void Cn3D_OpenEnd()
{
    PMSD pmsdMaster = NULL;
    PDNMS pdnmsMaster = NULL, pdnmsSlave = NULL;
    SeqEntryPtr sep = NULL;
#ifdef _OPENGL
    BiostrucFeaturePtr bsfp;
    Camera *camera;
#endif

    Cn3DIndexUserDefinedFeature();

#ifdef _OPENGL
    OGL_Reset(Cn3D_ColorData.OGL_Data);
#else
    ZoomAll3D(Cn3D_v3d);
#endif
        
    pdnmsMaster = GetSelectedModelstruc();
    if(pdnmsMaster == NULL) return;
    pmsdMaster = pdnmsMaster->data.ptrvalue;
    if(pmsdMaster == NULL) return;

    Cn3D_GetRenderSettingsFromBiostruc(pdnmsMaster);

    if (!Mime_ReadIn || pmsdMaster->iMimeType == NcbiMimeAsn1_entrez) {
        Cn3D_StartNet(TRUE);
        if (Cn3D_ColorData.EntrezOn) {
            sep = Cn3DFetchSeqEntry(pmsdMaster);
            if (sep != NULL) {
                pmsdMaster->iMimeType = NcbiMimeAsn1_strucseq;
                Cn3D_RegisterSeqEntry(sep);
            }
            else pmsdMaster->iMimeType = NcbiMimeAsn1_entrez;
        }
        else pmsdMaster->iMimeType = NcbiMimeAsn1_entrez;
    }

    Cn3D_ColorData.IsUserData = TRUE;
    if(Cn3D_ColorData.pDDVColorGlobal == NULL) {
        Cn3D_ColorData.pDDVColorGlobal = DDV_CreateColorGlobal(FALSE,
            NULL);
        DDV_LoadSSColor(Cn3D_ColorData.pDDVColorGlobal, "CN3D");
        Cn3D_ColorData.IsUserData = FALSE;
        Cn3D_RegisterColor();
    }
    
    if (pmsdMaster->iMimeType != NcbiMimeAsn1_entrez) {    
        TraverseMolecules(pdnmsMaster, 0, 0, NULL,
            (pNodeFunc) fnRewireSips);
        for(pdnmsSlave = pmsdMaster->pdnmsSlaves; pdnmsSlave != NULL;
        pdnmsSlave = pdnmsSlave->next)
            TraverseMolecules(pdnmsSlave, 0, 0, NULL,
            (pNodeFunc) fnRewireSips);
    }

    Cn3D_ResetActiveStrucProc();
    Cn3D_RedrawEx(TRUE);          /* always a new structure */

#ifdef _OPENGL
    bsfp = Cn3D_FindFeature(pmsdMaster->pbsBS->features,
        Feature_type_camera, 1);

    if(bsfp != NULL) {
        camera = (Camera *)bsfp->Property_property->data.ptrvalue;
        if(camera->scale != 0) {
            Cn3D_Asn2Matrix(Cn3D_ColorData.OGL_Data->ModelMatrix, camera->modelview);
            Cn3D_ColorData.OGL_Data->CameraDistance = ((FloatLo)camera->distance)
                /camera->scale;
            Cn3D_ColorData.OGL_Data->CameraAngle = ((FloatLo)camera->angle)
                /camera->scale;
            Cn3D_ColorData.OGL_Data->CameraDirection[0] = ((FloatLo)camera->x)
                /camera->scale;
            Cn3D_ColorData.OGL_Data->CameraDirection[1] = ((FloatLo)camera->y)
                /camera->scale;
            Cn3D_ColorData.OGL_Data->NeedCameraSetup = TRUE;
        }
    }
    OGL_AllLayerOnProc(Cn3D_ColorData.OGL_Data);
#endif

    Cn3D_RedrawNUpdate(FALSE);
    if (pmsdMaster->iMimeType != NcbiMimeAsn1_entrez) LaunchSequenceWindow();
    
}

static void Cn3D_StoreAlignRowNumsInMMDs(SeqAnnot * sanp)
{
    PDNMS pdnmsMaster, pdnmsSlave;
    PMMD pmmdThis;
    Int4 row = 1;
    SeqAlignPtr salp = NULL;
    SeqIdPtr sip;
        
    for (; sanp != NULL; sanp = sanp->next) {
        if (sanp->data == NULL) continue;
        salp = sanp->data;
        break;
    }
    if (!salp) return;

    pdnmsMaster = GetSelectedModelstruc();
    if (pdnmsMaster == NULL) return;

    /* assume master is row #1 */
    sip = AlnMgrGetNthSeqIdPtr(salp, row);
    pmmdThis = GetMMFromMSDBySeqId((PMSD) pdnmsMaster->data.ptrvalue, sip);
    if (!pmmdThis) {
        Message(MSG_ERROR, "error matching master MMD to target row 0");
        return;
    }
    pmmdThis->iTargetRow = row;
    row++;

    /* assume slaves are rows 2..N in order */
    for(pdnmsSlave = ((PMSD) pdnmsMaster->data.ptrvalue)->pdnmsSlaves; 
        pdnmsSlave;
        pdnmsSlave = pdnmsSlave->next, row++) {

        sip = AlnMgrGetNthSeqIdPtr(salp, row);
        pmmdThis = GetMMFromMSDBySeqId((PMSD) pdnmsSlave->data.ptrvalue, sip);
        if (!pmmdThis) {
            Message(MSG_ERROR, "error matching slave MMD to target row %i", row);
            return;
        }
        pmmdThis->iTargetRow = row;
    }
}


/******************************************************************************
*
* Opens the file with filename and tries to read in NcbiMimeAsn1.  Doesn't 
* care if it binary or text encoded.  If removeIt is set, deletes the file
* afterwards.  The NcbiMimeAsn1 is read into the object manager and MMDBapi
*
******************************************************************************/

Boolean OpenMimeFileWithDeletion(CharPtr filename, Boolean removeIt)
{
    Boolean retval = FALSE;
    AsnIoPtr aip;
    Char buf[50];
    NcbiMimeAsn1Ptr mime;
    FILE *fp = FileOpen(filename, "r");

    WatchCursor();
    if (!fp)
        return FALSE;

    FileRead(buf, 1, StrLen(PRINT_FORM_MIME_NAME), fp);
    FileClose(fp);
    if (StrNCmp(buf, PRINT_FORM_MIME_NAME, StrLen(PRINT_FORM_MIME_NAME)) ==
        0) aip = AsnIoOpen(filename, "r");
    else
        aip = AsnIoOpen(filename, "rb");
    if (!aip)
        return FALSE;
    mime = NcbiMimeAsn1AsnRead(aip, NULL);
    AsnIoClose(aip);
    if (!mime)
        return FALSE;
    Cn3DMime = mime->choice;

    Cn3D_OpenStart();
    Mime_ReadIn = TRUE;
    retval = MMDB_ReadMime(mime);
    Cn3D_OpenEnd();

    if (removeIt) {
        FileRemove(filename);
    }
    ArrowCursor();

    return retval;
}

/*
 * callback to initialize MG's PARS list with default PARS (passed as ptr)
 */
void LIBCALLBACK fnSetGlobalPARSinMG(PFB pfbThis, Int4 iModel,
                                     Int4 iIndex, Pointer ptr)
{
    ValNodeAddPointer(&(((PMGD) pfbThis)->pvnPARSList), 0, (VoidPtr) ptr);
}

NLM_EXTERN void Cn3D_SetPars(PARS parsThis, PDNMS pdnms)
{
    PMSD pmsdThis;

    if(parsThis == NULL || pdnms == NULL) return;
    pmsdThis = pdnms->data.ptrvalue;
    pmsdThis->pGlobalPARS = parsThis;
    TraverseGraphs(pdnms, 0, 0, (Pointer) parsThis, (pNodeFunc) fnSetGlobalPARSinMG);
}

/******************************************************************************
*
* Meant to be an ncbiobj and mmdb dependent function to process mime input
* into objects and MSD's.  Still has a few dependencies to be eliminated
*
******************************************************************************/
Boolean MMDB_ReadMime(NcbiMimeAsn1Ptr mime)
{
    Boolean retval = FALSE;
    BiostrucFeaturePtr pbsfThis;
    PMSD pmsdSlave = NULL, pmsdMaster = NULL;
    PDNMS pdnmsMaster = NULL, pdnmsSlave = NULL;
    BiostrucAlignSeqPtr pbsasThis = NULL;
    SeqAnnotPtr sap = NULL;


    do { {                      /* TRY */
            EntrezGeneralPtr egp;
            BiostrucAlignPtr pbsaThis;
            BiostrucSeqPtr bssp;
            BiostrucSeqsPtr bsssp;
            PDNMS pdnms = NULL;
            PMSD pmsdThis = NULL;
            PDNML pdnmlThis = NULL;
            ValNodePtr pvnAlignment;
            PDNTRN pdnTransform = NULL;
            PARS parsThis;
            BiostrucPtr pbsThis;

            switch (mime->choice) {

            case NcbiMimeAsn1_entrez:
                egp = (EntrezGeneralPtr) mime->data.ptrvalue;
                if (!egp || !egp->Data_data)
                    break;      /* THROW */

                retval = TRUE;
                switch (egp->Data_data->choice) {
                case Data_data_structure:
                    pdnms =
                        MakeAModelstruc((BiostrucPtr) egp->Data_data->data.ptrvalue);
                    if (!pdnms) break;

                    pmsdThis = (PMSD) pdnms->data.ptrvalue;
                    pmsdThis->iMimeType = NcbiMimeAsn1_entrez;
                    MMDB_OpenTraverse(pmsdThis);
                    Cn3D_SetPars(NewStructureRenderSet(), pdnms);
                    break;

                case Data_data_nuc:
                case Data_data_prot:
                case Data_data_ml:
                case Data_data_genome:
                default:
                    break;
                }
                break;

            case NcbiMimeAsn1_strucseq:
                bssp = (BiostrucSeqPtr) mime->data.ptrvalue;
                pdnms = MakeAModelstruc((BiostrucPtr) bssp->structure);
                if (!pdnms) break;

                pmsdThis = (PMSD) pdnms->data.ptrvalue;
                Cn3D_SetPars(NewStructureRenderSet(), pdnms);
                pmsdThis->iMimeType = NcbiMimeAsn1_strucseq;

                MMDB_OpenTraverse(pmsdThis);
                Cn3D_RegisterSeqEntry(bssp->sequences);
                break;

            case NcbiMimeAsn1_strucseqs:
                bsssp = (BiostrucSeqsPtr) mime->data.ptrvalue;
                pdnms = MakeAModelstruc((BiostrucPtr) bsssp->structure);
                if (!pdnms) break;

                pmsdThis = (PMSD) pdnms->data.ptrvalue;
                pmsdThis->iMimeType = NcbiMimeAsn1_strucseqs;
                MMDB_OpenTraverse(pmsdThis);
                Cn3D_MarkStrucSeqs(pdnms, bsssp->seqalign);

                sap = Cn3D_ShredCBlast(bsssp->seqalign);
                Cn3D_RegisterSeqAnnot(sap, TRUE, TRUE);
                Cn3D_RegisterSeqEntry(bsssp->sequences);
                parsThis = NewStructureRenderSet();
                parsThis->PBBColor = C_BYSEQCONS;
                parsThis->ConsColAlg = CSC_SHOWIDENTITY;
                Cn3D_SetPars(parsThis, pdnms);
                /* after indexing/IBM, store "original" row numbers in corresponding MMD */
                Cn3D_StoreAlignRowNumsInMMDs(sap);
                break;

            case NcbiMimeAsn1_alignseq:
                /*
                pbsasThis = (BiostrucAlignSeqPtr) mime->data.ptrvalue;
                sap = Cn3D_ShredCBlast(pbsasThis->seqalign);
                Cn3D_RegisterSeqAnnot(sap, TRUE, TRUE);
                Cn3D_RegisterSeqEntry(pbsasThis->sequences);
                */
                Message(MSG_ERROR, "Cn3D: Sorry, can't view alignments without structures (yet).");
                exit(-1);
                return retval;

            case NcbiMimeAsn1_alignstruc: /* this is the code that received alignments */
                pbsaThis = (BiostrucAlignPtr) mime->data.ptrvalue;
                if (!pbsaThis) break;      /* THROW */
                retval = TRUE;
                /* load in the master */
                pdnmsMaster = MakeAModelstruc((BiostrucPtr) pbsaThis->master); /* grab the master struc */
                if (!pdnmsMaster) break;

                pmsdMaster = (PMSD) pdnmsMaster->data.ptrvalue;

                pmsdMaster->iMimeType = NcbiMimeAsn1_alignstruc;

                parsThis = NewAlignRenderSet();
                Cn3D_SetPars(parsThis, pdnmsMaster);

                pmsdMaster->bVisible = TRUE;
                MMDB_OpenTraverse(pmsdMaster);

                /* add the alignment seq annot ptr, etc. */
                pmsdMaster->psaStrucAlignment = pbsaThis->alignments;
                Cn3D_RegisterSeqAnnot(pbsaThis->seqalign, TRUE, TRUE);
                Cn3D_RegisterSeqEntry(pbsaThis->sequences);

                SetNeighborOn(); /* turn on neighbor mode */

                SetMasterModelstruc(pdnmsMaster);

                /* do a slave */
                pbsThis = (BiostrucPtr) pbsaThis->slaves;
                pbsfThis = (BiostrucFeaturePtr)
                    pbsaThis->alignments->features->features;
                pvnAlignment = NULL;
                TraverseGraphs(pdnmsMaster, 0, 0, NULL,
                               (pNodeFunc) fnClearMarkedResidues);

                while (pbsThis) {
                    pdnmsSlave = MakeAModelstruc(pbsThis);
                    if (!pdnmsSlave)
                        break;
                    TraverseGraphs(pdnmsSlave, 0, 0, NULL,
                                   (pNodeFunc) fnClearMarkedResidues);
                    pmsdSlave = (PMSD) pdnmsSlave->data.ptrvalue;
                    pmsdSlave->bMaster = FALSE; /* this is not a master struct */
                    pmsdMaster->bAligned++;
                    pmsdSlave->pbAligned = &(pmsdMaster->bAligned);

                    MMDB_OpenTraverse(pmsdSlave);
                    Cn3D_SetPars(parsThis, pdnmsSlave);

                    pmsdSlave->bVisible = TRUE; /* turn them all on by default */

                    pvnAlignment =
                        fnMarkAlignedResidues(pdnmsMaster, pdnmsSlave, pbsfThis);
                    if (!pvnAlignment) break;

                    /* create the spatial transformation */
                    pdnTransform = NULL;
                    TransformToDNTRN(&pdnTransform,
                        ((ChemGraphAlignmentPtr) pvnAlignment->data.ptrvalue)->transform);
                    /* reverse transforms order to pass to Vibrant/shim3d/OpenGL as ValNode */
                    if (pdnTransform) {
                        while (pdnTransform->next) pdnTransform = pdnTransform->next;
                        while (pdnTransform) {
                            ValNodeAddPointer(&(pmsdSlave->pdnSlaveToMasterTransforms), 
                                pdnTransform->choice, pdnTransform->data.ptrvalue);
                            pdnTransform = pdnTransform->last;
                        }
                        
                    }

                    /* loop over the slave's models with the transformation */
                    /*
                    if (pdnTransform == NULL) break;
                    pdnmlThis = pmsdSlave->pdnmlModels;

                    while (pdnmlThis) {
                        TraverseAtoms(pdnmsSlave, pdnmlThis->choice, 0,
                                      pdnTransform, DoApplyTransform);
                        TraverseSolids(pdnmsSlave, pdnmlThis->choice, 0,
                                       pdnTransform, DoApplyTransform);
                        pdnmlThis = pdnmlThis->next;
                    }
                    */

                    FreeDNTRN(pdnTransform);

                    pbsThis = pbsThis->next;
                    pbsfThis = pbsfThis->next;
                }               /*while pbsThis */

                /* after indexing/IBM, store "original" row numbers in corresponding MMD */
                Cn3D_StoreAlignRowNumsInMMDs(pbsaThis->seqalign);
                break;

            default:
                break;
            }

    }
    } while (0);                /* End-of-TRY-block */

    return retval;
}


static void Cn3D_OpenEnableProc(TexT t)
{
    Char str[32];
    GetTitle(Cn3D_tOpen, str, sizeof(str));
    if (StringLen(str) == 0) {
        Disable(Cn3D_bOpenAccept);
    } else {
        Enable(Cn3D_bOpenAccept);
    }
    return;
}

static void Cn3D_NetOpenAcceptProc(ButtoN b)
{
    Char str[32];
    BiostrucPtr pbsBiostruc = NULL;
    PDNMS pdnmsModelstruc = NULL;
    PMSD pmsdThis = NULL;
    Int4 MdlNo, MdlLvl;

    WatchCursor();
    Cn3D_OpenStart();

    GetTitle(Cn3D_tOpen, str, sizeof(str));

    switch (GetValue(Cn3D_gMdlLvl)) {
    case 1:
        MdlLvl = ONECOORDATOM;
        break;
    case 2:
        MdlLvl = ONECOORDRES;
        break;
    case 3:
        MdlLvl = ALLMDL;
        break;
    case 4:
        MdlLvl = VECMODEL;
        break;
    case 5:
        MdlLvl = ALLSIMPLEMDL;
        break;
    case 6:
        MdlLvl = BSEVERYTHING;
        break;
    default:
        MdlLvl = ONECOORDATOM;  /* set from config-file default */
    }
    switch (GetValue(Cn3D_gMdlNo)) {
    case 1:
        MdlNo = 1;
        break;
    case 2:
        MdlNo = 2;
        break;
    case 3:
        MdlNo = 5;
        break;
    case 4:
        MdlNo = 10;
        break;
    case 5:
        MdlNo = 15;
        break;
    case 6:
        MdlNo = 20;
        break;
    case 7:
        MdlNo = MAX_MDLNO;
        break;
    case 8:
    default:
        MdlNo = 1;              /* set from config-file default */
    }
    if (GetValue(Cn3D_gMdlLvl) == 6)
        MdlNo = MAX_MDLNO;      /* get all */
    if (GetValue(Cn3D_gAccType) == 1) { /* PDB */
        pbsBiostruc = FetchBiostrucPDB(str, MdlLvl, MdlNo);
    } else {                    /* MMDB */

        pbsBiostruc =
            FetchBS(str, INP_GI, MdlLvl, MdlNo, GetMMDBAPIbExtent());
    }
    ArrowCursor();
    if (pbsBiostruc != NULL) {
        WatchCursor();
        pdnmsModelstruc = MakeAModelstruc(pbsBiostruc);
        ArrowCursor();
    }
    if (pdnmsModelstruc == NULL) {
        /* return a not found error here */
        Remove(Cn3D_wNetOpen);
        Cn3D_EnableFileOps();
        Cn3D_Open_InUse = FALSE;
        return;
    }
    if (GetValue(Cn3D_gMdlLvl) == 5) { /* turn off backbone model if "All" models present */
        pmsdThis = (PMSD) pdnmsModelstruc->data.ptrvalue;
        MMDB_OpenTraverse(pmsdThis);
    }
    Cn3D_SetPars(NewStructureRenderSet(), pdnmsModelstruc);

    Remove(Cn3D_wNetOpen);
    Cn3D_EnableFileOps();

    /*CALL TO initialize the view */

    Cn3D_Open_InUse = FALSE;

    Cn3D_OpenEnd();

    return;
}


static void Cn3D_NetOpenCancelProc(GraphiC g)
{
    Remove(Cn3D_wNetOpen);
    Cn3D_EnableFileOps();
    Cn3D_Open_InUse = FALSE;
    return;
}

NLM_EXTERN void Cn3D_NetOpenBiostruc(IteM i)
{
    GrouP g, hg;
    ButtoN b;

    if (Cn3D_Open_InUse)
        return;
    else
        Cn3D_Open_InUse = TRUE;

    Cn3D_StartNet(TRUE);
    if (!Cn3D_ColorData.EntrezOn) return;

    Cn3D_wNetOpen =
        MovableModalWindow(-30, -20, -10, -10, " Internet retrieve from MMDB ",
                    NULL);
    hg = HiddenGroup(Cn3D_wNetOpen, 3, 0, NULL);
    SetGroupSpacing(hg, 30, 30);
    g = NormalGroup(hg, 1, 0, " Enter accession code:", systemFont, NULL);
    SetGroupMargins(g, 10, 15);
    Cn3D_tOpen = DialogText(g, "", 10, (TxtActnProc) Cn3D_OpenEnableProc);
    Cn3D_gAccType =
        NormalGroup(hg, 1, 2, " accession type", systemFont, NULL);
    SetGroupMargins(Cn3D_gAccType, 10, 10);
    RadioButton(Cn3D_gAccType, "PDB Code");
    RadioButton(Cn3D_gAccType, "MMDB ID");

    g = HiddenGroup(hg, 1, 2, NULL);
    SetGroupSpacing(g, 15, 15);
    Cn3D_bOpenAccept =
        DefaultButton(g, "OK", (BtnActnProc) Cn3D_NetOpenAcceptProc);
    b = PushButton(g, "Cancel", (BtnActnProc) Cn3D_NetOpenCancelProc);

    Cn3D_gMdlLvl =
        NormalGroup(Cn3D_wNetOpen, 2, 3, " model complexity", systemFont,
                    NULL);
    SetGroupMargins(Cn3D_gMdlLvl, 10, 10);
    SetGroupSpacing(Cn3D_gMdlLvl, 10, 5);
    RadioButton(Cn3D_gMdlLvl, "a) NCBI one XYZ per atom model");
    RadioButton(Cn3D_gMdlLvl, "c) NCBI backbone model");
    RadioButton(Cn3D_gMdlLvl, "b) original PDB models 1-n");
    RadioButton(Cn3D_gMdlLvl, "d) NCBI vector model");
    RadioButton(Cn3D_gMdlLvl, "Viewing Subset (a, c and d)");
    RadioButton(Cn3D_gMdlLvl, "Everything");

    Cn3D_gMdlNo =
        NormalGroup(Cn3D_wNetOpen, 7, 0, " n = ", systemFont, NULL);
    RadioButton(Cn3D_gMdlNo, "1");
    RadioButton(Cn3D_gMdlNo, "2");
    RadioButton(Cn3D_gMdlNo, "5");
    RadioButton(Cn3D_gMdlNo, "10");
    RadioButton(Cn3D_gMdlNo, "15");
    RadioButton(Cn3D_gMdlNo, "20");
    RadioButton(Cn3D_gMdlNo, "maximum");


    SetValue(Cn3D_gMdlNo, 7);
    SetValue(Cn3D_gAccType, 1);
    SetValue(Cn3D_gMdlLvl, 5);
    Disable(Cn3D_bOpenAccept);
    Cn3D_DisableFileOps();
    Select(Cn3D_tOpen);
    Show(Cn3D_wNetOpen);
    return;
}

/*********************************************/
/* below this are the file i/o open-er procs */
/*********************************************/

NLM_EXTERN void Cn3D_OpenBiostruc(IteM i)
{
    Char str[PATH_MAX];
    unsigned char szBegin[10];
    BiostrucPtr pbsBiostruc;
    PDNMS pdnmsModelstruc;
    PMSD pmsdThis = NULL;
    Int4 MdlNo = MAX_MDLNO;
    FILE *hFile;
    Char buf[50];

    StrCpy(str,"");
    GetInputFileName(str, sizeof(str), NULL, NULL);
    if(StrCmp(str,"") == 0) return;
    WatchCursor();
    hFile = FileOpen(str, "rb");
    if(hFile == NULL) return;
    FileGets((CharPtr) szBegin, 2, hFile);
    if (hFile == NULL) szBegin[0] = (Char) 0;
    FileClose(hFile);
    if(szBegin[0] == 31 && szBegin[1] == 139) {
        Message(MSG_ERROR, 
            "This file may require a newer version of Cn3D.  Go to http://www.ncbi.nlm.nih.gov/Structure/CN3D for more information");     
        return;
    }
    /* to make cn3d to take strucseq for which szBegin[0] is 78 */
    /* for single biostruc szBegin[0] is 48 */
    if (szBegin[0] > 70) { /* mime */
        if (!OpenMimeFileWithDeletion(str, FALSE)) {
            return;
        }
    } else {                    /* not mime */
        
        Cn3D_OpenStart();
        hFile = FileOpen(str, "r");
        
        FileRead(buf, 1, StrLen(PRINT_FORM_BIOSTRUC), hFile);
        FileClose(hFile);
        if (StrNCmp(buf, PRINT_FORM_BIOSTRUC, StrLen(PRINT_FORM_BIOSTRUC)) ==
            0) { /* ASCII */
            /* these get everything in the file , ignoring modellevel */
            pbsBiostruc =
                FetchBS(str, INP_ASCII_FILE, BSEVERYTHING, MdlNo,
                CONVERT_ALL);
        } else {                /* Binary */
            pbsBiostruc =
                FetchBS(str, INP_BINARY_FILE, BSEVERYTHING, MdlNo,
                CONVERT_ALL);
        }
        ArrowCursor();
        if (pbsBiostruc != NULL) {
            WatchCursor();
            pdnmsModelstruc = MakeAModelstruc(pbsBiostruc);
            pmsdThis = (PMSD) pdnmsModelstruc->data.ptrvalue;
            MMDB_OpenTraverse(pmsdThis);
            Cn3D_SetPars(NewStructureRenderSet(), pdnmsModelstruc);

            ArrowCursor();
        } else return;

        Cn3D_OpenEnd();
    }                           /* switch between mime and non-mime */
}

/* generic set up for new alignment */
static void Cn3D_ImportSAEnd(SeqAlign *salp, Boolean Neat)
{
    SeqAnnot * annot;
    PMSD pmsdThis;
    PDNMS pdnmsThis;
    Uint2 entityID;
    PARS pars = NULL;
        
    if (salp == NULL) return;
    pdnmsThis = GetSelectedModelstruc();
    if (!pdnmsThis) return;
    
    /* close the sequence window */
    entityID =
        ObjMgrGetEntityIDForPointer((void *) Cn3D_ColorData.pvnsep);
    ObjMgrSendMsg(OM_MSG_FLUSH, entityID, 0, 0);

    /* fetch the active structure */
    pars = (PARS) ((PMSD) pdnmsThis->data.ptrvalue)->pGlobalPARS;
    if (pars) {
    pars->ConsColAlg = (Nlm_Int2) 0;
    pars->PBBColor = C_BYSEQCONS;
    pars->PResColor = C_BYSEQCONS;
    pars->ObjectOn = FALSE;
    }

    annot = SeqAnnotNew();
    if(annot == NULL) return;
    annot->type = 2;
    annot->data = salp;

    /* change mime type for saving */
    if(pdnmsThis) {
        pmsdThis = pdnmsThis->data.ptrvalue;
        if(pmsdThis) pmsdThis->iMimeType = NcbiMimeAsn1_strucseqs;
        Cn3D_MarkStrucSeqs(pdnmsThis, annot);
    }

    Cn3D_ColorData.pDDVColorGlobal = NULL; /* force creation of new color object */
    Cn3D_RegisterSeqAnnot(annot, Neat, Neat); /* register annot? */
    Cn3D_StoreAlignRowNumsInMMDs(annot);
    Cn3D_LaunchSeqAnnot(annot);
    ViewMgr_Update(salp);
    Disable(Cn3D_ColorData.BlastMany);
}


#if 0

static void Cn3D_ImportSAEndCB(UDV_BlastDlgData *bddp)
{
    if(bddp) Cn3D_ImportSAEnd(bddp->salp, bddp->IBM);
}


/*******************************************************************************

Function : Cn3D_BlastDlg()
  
Purpose : Blast alignment dialog for multiple alignment
  
Parameters : i; menu

*******************************************************************************/
NLM_EXTERN void Cn3D_BlastDlg(IteM i)
{
    UDV_BlastDlgData *bddp;
    
    if(Cn3D_ColorData.sap) return;
    Cn3D_StartNet(TRUE);
    if (!Cn3D_ColorData.EntrezOn) {
        Message(MSG_ERROR, "Blast requires network access");
        return;
    }

    bddp = (UDV_BlastDlgData *)MemNew(sizeof(UDV_BlastDlgData));
    if (bddp == NULL) return;
    bddp->pvnSips = SAM_ExtractSips(Cn3D_ColorData.pvnsep);
    bddp->callback = Cn3D_ImportSAEndCB;
    bddp->IBM = FALSE;

    UDV_BlastDlg(bddp);
}

#endif /* 0 */

static void Cn3D_ImportSeqEntry(DDV_ImportDialog *idp)
{
    DDVUpdateMSG dum;
    Uint2 entityID, entityIDsalp;
    ValNode *pvn;
    
    entityID = ObjMgrGetEntityIDForPointer((void *) idp->bsp);

    if(Cn3D_ColorData.sap && (idp->mode == DDVIMPNET2SA
        || idp->mode == DDVIMPSE2SA)) {
        MemSet(&dum, 0, sizeof(DDVUpdateMSG));
        dum.data = &entityID;
        dum.type = UPDATE_TYPE_NEWSEQ;
        entityIDsalp = ObjMgrGetEntityIDForPointer((void *) Cn3D_ColorData.sap->data);
        ObjMgrSendProcMsg(OM_MSG_UPDATE, entityIDsalp, 0, OBJ_SEQALIGN,
            0, 0, (Pointer)&dum);
    }
    
    if(idp->mode & DDVIMPNET)
    {
        idp->sep = GetBestTopParentForData(entityID, idp->bsp);
        for(pvn = Cn3D_ColorData.pvnsep; pvn != NULL; pvn = pvn->next)
            if(pvn == idp->sep) return;
    } 
    ValNodeLink(&Cn3D_ColorData.pvnsep, idp->sep);
    idp->sep->next = NULL;
    SAM_MakeViewerFree((void *) idp->sep);
    if(IS_Bioseq(idp->sep)) GatherSpecificProcLaunch(0, "Seq-Struc Communication",
        OMPROC_VIEW, FALSE, entityID, 0, OBJ_BIOSEQ);
    else GatherSpecificProcLaunch(0, "Seq-Struc Communication", OMPROC_VIEW,
        FALSE, entityID, 0, OBJ_BIOSEQSET);
}

/*******************************************************************************

  Function : Cn3D_ImportCBSE()
  
  Purpose : callback for importing single sequence into a seqalign

*******************************************************************************/
static void Cn3D_ImportCBSE(DDV_ImportDialog *idp, SeqAlign *salpdest,
                            SeqAlign *salp)
{
    if (salpdest == NULL || salp == NULL) return;
    SeqAlignSetFree(salp->next);
    salp->next = NULL;
    ViewMgr_Add(salpdest, salp);
    Cn3D_ImportSeqEntry(idp);
}

/*******************************************************************************

  Function : Cn3D_ImportCB()
  
  Purpose : callback for importing single sequence and creating a new pairwise
            seqalign

*******************************************************************************/
static void Cn3D_ImportCB(DDV_ImportDialog *idp, SeqAlign *salpdest,
                          SeqAlign *salp)
{
    if (salp == NULL) return;
    SeqAlignSetFree(salp->next);
    salp->next = NULL;
    Cn3D_ImportSAEnd(salp, TRUE);
    Cn3D_ImportSeqEntry(idp);
}

/*******************************************************************************

  Function : Cn3D_ImportBioseq()
  
  Purpose : Import a single bioseq via the net

*******************************************************************************/
static void Cn3D_sImportBioseq(Boolean Gap)
{
    DDV_ImportDialog *idp;

    idp = MemNew(sizeof(DDV_ImportDialog));
    if(idp == NULL) return;

    idp->AccessionCB = Cn3D_Accession2Gi;
    idp->Gap = Gap;
    Cn3D_StartNet(TRUE);
    if (!Cn3D_ColorData.EntrezOn) {
        Message(MSG_ERROR, "Blast requires network access");     
        return;
    }

    if(Cn3D_ColorData.sap == NULL) {
        idp->sap = NULL;
        idp->callback = Cn3D_ImportCB;
        idp->pvnSips = SAM_ExtractSips(Cn3D_ColorData.pvnsep);
        idp->mode = DDVIMPNET2SE;
    }
    else {
        idp->sap = (SeqAlign *)Cn3D_ColorData.sap->data;
        idp->sip = AlnMgrFindMaster(idp->sap);
        idp->callback = Cn3D_ImportCBSE;
        idp->mode = DDVIMPNET2SA;
    }

    DDV_ImportBioseqDlg(idp);
}

NLM_EXTERN void Cn3D_ImportBioseq(IteM i)
{
    Cn3D_sImportBioseq(FALSE);
}

NLM_EXTERN void Cn3D_ImportBioseqGap(IteM i)
{
    Cn3D_sImportBioseq(TRUE);
}


/*******************************************************************************

  Function : Cn3D_ImportBioseqFile()
  
  Purpose : Import a single bioseq from a fasta file

*******************************************************************************/
static void Cn3D_sImportBioseqFile(Boolean Gap)
{
    DDV_ImportDialog *idp;
    SeqEntry *sep;  /* need to deallocate! */
    Char str[PATH_MAX];
    FILE *fp;
    Bioseq *bsp;

    StrCpy(str,"");
    GetInputFileName(str, sizeof(str), NULL, NULL);
    if(StrCmp(str,"") == 0) return;

    fp = FileOpen(str,"r");

    sep = FastaToSeqEntry(fp, FALSE);
    FileClose(fp);
    if (sep == NULL) {
        Message(MSG_ERROR, "Sequence Import Error");     
        return;
    }
       
    if (IS_Bioseq(sep)) {
        bsp = (BioseqPtr) sep->data.ptrvalue;
        if (bsp != NULL) ObjMgrRegister (OBJ_BIOSEQ, (Pointer) bsp);
    } else {
        SeqEntryFree(sep);
        Message(MSG_ERROR, "Sequence Import Error: must be a single sequence");     
        return;
    }

    idp = MemNew(sizeof(DDV_ImportDialog));
    if(idp == NULL) return;
    idp->sep = sep;
    idp->Gap = Gap;
    idp->sipslave = SAM_ExtractSips(sep);
    idp->AccessionCB = Cn3D_Accession2Gi;

    if(Cn3D_ColorData.sap == NULL) {
        idp->sap = NULL;
        idp->callback = Cn3D_ImportCB;
        idp->pvnSips = SAM_ExtractSips(Cn3D_ColorData.pvnsep);
        idp->mode = DDVIMPSE2SE;
        DDV_ImportBioseqDlg(idp);
    }
    else {
        idp->sap = (SeqAlign *)Cn3D_ColorData.sap->data;
        idp->sip = AlnMgrFindMaster(idp->sap);
        idp->callback = Cn3D_ImportCBSE;
        idp->mode = DDVIMPSE2SA;
        DDV_DoAlign(idp);
    }

}

NLM_EXTERN void Cn3D_ImportBioseqFile(IteM i)
{
    Cn3D_sImportBioseqFile(FALSE);
}

NLM_EXTERN void Cn3D_ImportBioseqFileGap(IteM i)
{
    Cn3D_sImportBioseqFile(TRUE);
}


NLM_EXTERN Int4 Cn3D_Accession2Gi (CharPtr string, Boolean IsAmino)
{
   CharPtr str;
   LinkSetPtr lsp;
   Int4 gi;
   DocType AAorNN; /* is this of TYP_AA or TYP_NA */

   if (!EntrezIsInited ()) {
       Message (MSG_ERROR, "Network connection to Entrez unavailable");
       return 0;
   }

   if(IsAmino) AAorNN = TYP_AA;
   else AAorNN = TYP_NT;

   str = MemNew (StringLen (string) + 10);
   sprintf (str, "\"%s\" [ACCN]", string);
   lsp = EntrezTLEvalString (str, AAorNN, -1, NULL, NULL);
   MemFree (str);
   if (lsp == NULL) return 0;
   if (lsp->num <= 0) {
       LinkSetFree (lsp);
       return 0;
   }
   gi = lsp->uids [0];
   LinkSetFree (lsp);
   return gi;
}
