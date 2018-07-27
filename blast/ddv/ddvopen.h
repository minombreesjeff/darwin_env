/*  $Id: ddvopen.h,v 1.23 2000/07/17 17:46:48 hurwitz Exp $
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
* File Name:  ddvopen.h
*
* Author:  Patrick Durand
*
* Version Creation Date:   06/19/99
*
* $Revision: 1.23 $
*
* File Description: 
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: ddvopen.h,v $
* Revision 1.23  2000/07/17 17:46:48  hurwitz
* made it so show/hide dialog only updates view when ok is clicked
*
* Revision 1.22  2000/07/08 20:43:58  vakatov
* Get all "#include" out of the 'extern "C" { }' scope;  other cleanup...
*
* Revision 1.21  2000/06/16 14:57:03  lewisg
* move entrez calls out of desktop
*
* Revision 1.20  2000/05/15 23:39:34  lewisg
* shred cblast, add menu items for gapped/ungapped, fix pdbheaders
*
* Revision 1.19  2000/05/15 22:00:36  hurwitz
* put save-edits query into DDV_SlaveQuit where it belongs
*
* Revision 1.18  2000/05/11 16:28:44  wheelan
* made DDV_ShredAln NLM_EXTERN
*
* Revision 1.17  2000/04/17 13:30:42  durand
* removed g_hParent and unused functions DDV_LaunchAlignViewer and DDV_LaunchAlignEditor
*
* Revision 1.16  2000/04/08 00:37:32  lewisg
* multiple seqentries, NEWSEQ message, etc.
*
* Revision 1.15  2000/03/27 22:15:08  lewisg
* add show/hide row dialog
*
* Revision 1.14  2000/03/24 20:34:58  lewisg
* add blast from file, bug fixes, get rid of redundant code, etc.
*
* Revision 1.13  2000/01/26 13:38:54  durand
* update the GUI for the editor. Add functions to create the data to be used by the editor
*
* Revision 1.12  2000/01/12 21:52:17  durand
* add import function; update menus when DDV is loaded from Cn3D
*
* Revision 1.11  1999/12/30 21:08:45  lewisg
* bioseq import dialog
*
* Revision 1.10  1999/12/29 22:55:03  lewisg
* get rid of seqalign id
*
* Revision 1.9  1999/12/20 20:20:41  lewisg
* allow cn3d to do color and ddv to do case when both are running
*
* Revision 1.8  1999/12/03 23:17:23  lewisg
* Patrick's new global update msg, argument passing when launching ddv, experimental editing
*
* Revision 1.7  1999/11/29 15:26:26  durand
* designed a new GUI to fix problems under MacOS, Linux and SGI
*
* Revision 1.6  1999/11/03 21:29:47  durand
* add CTRL and SHFT keys for mouse selection. redesign the loader functions of DDV to properly register the message callbacks
*
* Revision 1.5  1999/10/29 19:04:21  durand
* move DDVUpdateMSG in objmgr.h
*
* Revision 1.4  1999/10/29 14:15:39  durand
* add simple mouse selection functions
*
* Revision 1.3  1999/10/20 13:17:18  durand
* add display for disc. SeqAlign tails
*
* Revision 1.2  1999/10/16 15:02:25  durand
* fixes due to toolkit build failed
*
* Revision 1.1  1999/09/30 14:10:28  durand
* add ddv to toolkit
*
* Revision 1.8  1999/09/28 17:29:41  durand
* add include of udviewer.h
*
* Revision 1.7  1999/09/16 13:07:53  durand
* add File|Close and File|Open|Network commands
*
* Revision 1.6  1999/09/09 21:55:06  durand
* instantiate the Fle|Close command of DDV
*
* Revision 1.5  1999/07/29 12:43:07  durand
* update DDV_GetAndCheckSeqAlign
*
* Revision 1.4  1999/07/01 15:28:29  durand
* validate function loaders of DDV
*
* Revision 1.2  1999/06/28 22:07:21  durand
* add loader functions and clean the code with Lint and Purify
*
* Revision 1.1  1999/06/19 17:21:07  durand
* add Vibrant DDV code
*
*
*
* ==========================================================================
*/

#ifndef _DDVOPEN_
#define _DDVOPEN_

#include <ncbi.h>
#include <udviewer.h>
#include <samutil.h>
#include <accentr.h>
#include <pgppop.h>
#include <alignmgr.h>

#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif

#ifdef __cplusplus
extern "C" {
#endif


/******************************************************************************

	ERROR / Information messages from DDV_OPEN module 

******************************************************************************/
#define DVV_MSG_O_I_NOMAINDATA   2000
#define DVV_MSG_O_E_READFILE     2001
#define DVV_MSG_O_E_READGI       2002
#define DVV_MSG_O_E_NOFETCHFUNC  2003
#define DVV_MSG_O_E_BADTYPE      2004
#define DVV_MSG_O_E_NOTHINGTODO  2005
#define DVV_MSG_O_E_OPENFILEFAIL 2006

/******************************************************************************

	other defines

******************************************************************************/
	/*use only by the standalone version of DDV */
#define REG_DDV_AUTO_EDIT ObjMgrProcLoad(OMPROC_EDIT, \
		"DDV", "MSA_Editor", OBJ_SEQALIGN, 0, OBJ_SEQALIGN, 0, \
		NULL, DDV_ObjRegMasterEditDDV, 0)	
#define REG_DDV_AUTO_VIEW ObjMgrProcLoad(OMPROC_VIEW, \
		"DDV", "MSA_Viewer", OBJ_SEQALIGN, 0, OBJ_SEQALIGN, 0, \
		NULL, DDV_ObjRegMasterViewDDV, 0)	

	/*slave 1 : the editor in a separate window*/
#define REG_DDV_SLA_EDIT ObjMgrProcLoad(OMPROC_EDIT, \
		"DDV", "MSA_Editor", OBJ_SEQALIGN, 0, OBJ_SEQALIGN, 0, \
		NULL, DDV_ObjRegSlaveEditDDV, 0)	

	/*slave 2 : the viewer in a separate window*/
#define REG_DDV_SLA_VIEW ObjMgrProcLoad(OMPROC_VIEW, \
		"DDV", "MSA_Viewer", OBJ_SEQALIGN, 0, OBJ_SEQALIGN, 0, \
		NULL, DDV_ObjRegSlaveViewDDV, 0)	

/*the following are used to delete data when closing DDV*/
#define DDV_OPENTYPE_NOTRESP ((Uint1)1)/*if DDV is not responsible to delete
                                         data when closing*/
#define DDV_OPENTYPE_FILE    ((Uint1)2)/*if ReadAsnFastaOrFlatFile was used*/
#define DDV_OPENTYPE_SEP     ((Uint1)3)/*if get a SEP*/
#define DDV_OPENTYPE_GI      ((Uint1)4)/*if fetch GI from DB was used*/

/******************************************************************************

	Data structures

******************************************************************************/
typedef struct dlgfileopendata {/*use to manage the FileOpen dialog box*/
	WindoW 		parent;			/*main window of the application*/
	TexT		FNameEditCtrl;	/*handle of the file text control*/
	ButtoN		Ok;				/*handle of the Ok button*/
	GrouP		ReadMode;		/*handle of the file type control*/
} DlgFileOpenData, PNTR DlgFileOpenDataPtr;

typedef struct ddvopendata {
	Uint1           choice;
	SeqEntryPtr     sep;
	ValNodePtr      vnp;
	} DdvOpenData, PNTR DdvOpenDataPtr;

typedef struct ddvupdatelayoutdata{
	/*color display ?*/
	Boolean bUseColors;
	/*styles for a disc. seqalign*/
	Boolean ShowLeftTail;
	Boolean ShowRightTail;
	Uint1   DispDiscStyle;
	Uint1   SpacerSize;
    Uint1   DiscJustification;
	/*style for sequences*/
	Int4    nSeq;	/*number of rows (sequences) to update*/
	Int4Ptr SeqList; /*list of row number; one-based array*/
	Uint1   RulerStyle;
	/*DDV panel handle*/
	PaneL   ddv_panel;
	}DDVUpdateLayoutData, PNTR DDVUpdateLayoutDataPtr;

struct _DDV_ImportDialog;
typedef void (*DDV_ImportProcCB)(struct _DDV_ImportDialog *idp, SeqAlign *salpdest,
                                 SeqAlign *salp);

typedef Int4 (*DDV_AccessionCB)(CharPtr string, Boolean IsAmino);

/* for the BLAST import dialog */
typedef struct _DDV_ImportDialog {
    WindoW DDV_wImport; /*the import dialog*/
    GrouP DDV_gAccType; /*the type of accession*/
    ButtoN DDV_bImportAccept; /*accept button*/
    TexT DDV_tAccession; /* the accession */
    LisT DDV_lsip;  /* list box of sips */
    SeqId *sip;  /* the master sequence */
    SeqAlign *sap; /* the seqalign to add to */
    SeqId *sipslave;  /* the slave sequence if it exists */
    void *userdata; /* for the update message */
    DDV_ImportProcCB callback;  /* called after alignment created */
    ValNode *pvnSips;  /* list of sips to blast against */
    Int4 mode; /* see below */
    SeqEntry *sep;  /* used to pass new seqentry to call back */
    Bioseq *bsp; /* the bioseq being added */
    Boolean Gap; /* should the alignment used gapped blast? */
    DDV_AccessionCB AccessionCB; /* converts accession to gi */
} DDV_ImportDialog;

/* import to sequence to seqalign */
#define DDVIMPSE2SA 0x0
/* import sequence to sequence */
#define DDVIMPSE2SE 0x1
#define DDVIMP2SE 0x1
/* import net sequence to seqalign */
#define DDVIMPNET2SA 0x2
#define DDVIMPNET 0x2
/* import net sequence to sequence */
#define DDVIMPNET2SE 0x3

struct _DDV_HideDialog;
typedef void (*DDV_HideProcCB)(struct _DDV_HideDialog *hdp);

typedef struct _DDV_HideDialog {
    WindoW DDV_wHide; /*the show/hide dialog*/
    LisT DDV_lsip;  /* multilist box of sips */
    SeqAlign *salp; /* the seqalign that is shown/hid */
    SeqAlign *target; /* the original seqalign */
    AMAlignIndexPtr amaip; /* target index */
    void *userdata; /* for the update message */
    DDV_HideProcCB callback;  /* called on every show/hide */
    Int4 numrows;  /* true number of rows in target seqalign */
    Int4 LastValidRow; /* must be 2 valid rows in list, this and master */
} DDV_HideDialog;

/******************************************************************************

	Extern functions

******************************************************************************/
extern Int2 LIBCALLBACK DDV_ObjRegMasterEditDDV (Pointer data);
extern Int2 LIBCALLBACK DDV_ObjRegMasterViewDDV (Pointer data);
extern Int2 LIBCALLBACK DDV_ObjRegSlaveEditDDV (Pointer data);
extern Int2 LIBCALLBACK DDV_ObjRegSlaveViewDDV (Pointer data);
extern void DDV_OpenFile(IteM i);
extern void DDV_OpenNetwork(IteM i);
extern ValNodePtr DDV_GetAndCheckSeqAlign(FILE *fp,Int4 gi,SeqEntryPtr sep2,
	UdvFetchSeqEntryProc fetchSepProc,DdvOpenDataPtr dodp,Uint2Ptr entityID);
extern WindoW DDV_StartMainWin_Slave(SAM_ViewGlobal *vgp,Boolean bEditor);

extern ValNodePtr DDV_GetSelectedRegions(SelStructPtr om_slp, Uint2 bsp_eID,
	Uint2 bsp_iID);
extern Boolean DDV_IsLetterSelected(ValNodePtr vnp_bsp, Int4 bsp_pos);

NLM_EXTERN void DDV_ImportBioseqDlg(DDV_ImportDialog *idp);
NLM_EXTERN void DDV_ImportBioseq(IteM i);
NLM_EXTERN SeqAlignPtr DDV_ShredAln(SeqAlignPtr sap);
NLM_EXTERN void DDV_DoAlign(DDV_ImportDialog *idp);
NLM_EXTERN void DDV_ImportCB(DDV_ImportDialog *idp, SeqAlign *salpdest,
                             SeqAlign *salp);
NLM_EXTERN SeqAlign *DDV_Blast2Seqs(Bioseq *bsp1, Bioseq *bsp2, Boolean gapped,
                         Char *progname);
extern void DDV_ImportNucSeqAlign(IteM i);
extern void DDV_ImportProtSeqAlign(IteM i);
NLM_EXTERN void DDV_HideDlgItem(IteM i);
NLM_EXTERN void DDV_HideDlg(DDV_HideDialog *hdp);
NLM_EXTERN void DDV_GetCharBufferForEditor(ParaGPtr pgp,CharPtr szEditSeq);
NLM_EXTERN ValNodePtr DDV_GetMtdpListForEditor(ValNodePtr row,Int4 from_disp,
		Int4 to_disp);
NLM_EXTERN MsaParaGPopListPtr DDV_CreateDataForEditor(MsaParaGPopListPtr mpplp, 
	Int4 from_disp,Int4 to_disp);
NLM_EXTERN void DDV_SlaveQuit(WindoW w);


#ifdef __cplusplus
}
#endif

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

#endif /* ndef _DDVOPEN_ */
