/*   saledit.h
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
* File Name:  saledit.h
*
* Author:  Colombe Chappey
*
* Version Creation Date:   2/19/97
*
* $Revision: 6.30 $
*
* File Description: 
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* ==========================================================================
*/

#ifndef _SALEDIT_
#define _SALEDIT_

#include <ncbi.h>
#include <objseq.h>
#include <objmgr.h>
#include <objfdef.h>
#include <gather.h>
#include <vibrant.h>
#include <salsa.h>
#include <viewer.h>

typedef struct seqeditview {

  FORM_MESSAGE_BLOCK
  PaneL            pnl;
  ValNodePtr       data;
  VieweR           graph;

  PrompT           pos;
  TexT             gototxt,
                   lookattxt;
  TexT             fromtxt, totxt;

  ButtoN           gotobt,
                   lookatbt;

  GrouP            btngp;
  ButtoN           showfeatbt;
  ButtoN           translatebt;
  ButtoN           savefeatbt;
  ButtoN           refreshbt;
  ButtoN           closebt;
  ButtoN           svclosebt;
  ButtoN           gap_choicebn;
  ButtoN           keep_protid1,
                   keep_protid2;

  MenU             menu_align;
  IteM             editmodeitem,
                   viewmodeitem;
  IteM             prefitem;
  IteM             undoitem;
  IteM             cutitem, delitem, copyitem, pasteitem, insitem;
  IteM             rfitem[10];
  IteM             conscolor;
  IteM             importseq,
                   importalg,
                   importnet,
                   expfsagitem,
                   expalgitem,
                   expasnitem;
  IteM             showfeatitem,
                   hidefeatitem;
  IteM             propaitem;
  IteM             tmpcdsnitem, tmpcdspitem;
  IteM             selmaster;
  IteM             selall,
                   selsubs;
  IteM             showdifitem,
                   showallitem;
  IteM             dotmat;
  IteM             alreport;
/** not used **/
  IteM             translateitem;
  IteM             codonstitem;
  IteM             savefeatitem;

  GrouP            replaceMergeCopyGroup;
  GrouP            rmcExtra [3];
  ButtoN           acceptBtn;

  Boolean          extended_align_menu;
  Boolean          extended_dist_menu;
  Boolean          extended_tree_menu;
} SeqEditViewForm, PNTR SeqEditViewFormPtr;


typedef struct dialogboxdata
{
  FORM_MESSAGE_BLOCK
  WindoW      w; 
  TexT        txt1, txt2, txt3, txt4, txt5, txt6;
  LisT        lst1, lst2, lst3;
  ButtoN      bt;
  ButtoN      bt2;
  PopuP       color[10];
  Int2        align_format, mol_type;
  Int2        matrix, separator;
  Boolean     db_bool;
  PrompT      prompt;

} DialogBoxData, PNTR DialogBoxDataPtr;


/*
*  The SeqEditViewProcsPtr may be registered with a call to SetAppProperty
*  e.g., SetAppProperty ("SeqEditDisplayForm", &viewprocs), where viewprocs
*  is a persistent structure filled with callback function pointers specific
*  for a given application.
*/

typedef SeqEntryPtr (LIBCALLBACK *SeqEdDwnloadProc) (CharPtr program, CharPtr accession, Int4 uid, Boolean is_na, BoolPtr is_new);

typedef struct seqeditviewprocs {

  WndActnProc      activateForm;
  FormMessageFunc  handleMessages;
  Int2             minPixelWidth;
  Int2             minPixelHeight;

  Uint1            col1r, col2r, col3r, col4r;
  Uint1            col1g, col2g, col3g, col4g;
  Uint1            col1b, col2b, col3b, col4b;
  Uint1            col5r, col5g, col5b;
  Uint1            col6r, col6g, col6b;
  Uint1            col7r, col7g, col7b;
 
  Boolean          viewer_mode;
  Boolean          showfeat;
  Boolean          extended_align_menu;
  Boolean          extended_dist_menu;
  Boolean          extended_tree_menu;

  SeqEdDwnloadProc download; /* application links to Entrez network service */

  Boolean          Cn3D_On; 
  IteM             conscolor;
  Uint1            colorR_HL, colorG_HL, colorB_HL; /* yanli added to store highlight color */
  ValNodePtr       seqinfo;   /* yanli added */
                       /* data initially filled by cn3d to store SeqId, entityID, itemID, visible status and color arrays for those sequences loaded into memory by cn3d. seqinfo->data.ptrvalue is MediaInfoPtr(see "cn3dmsg.h"), each node is for one sequence */  

} SeqEditViewProcs, PNTR SeqEditViewProcsPtr;


#define REGISTER_NEW_BIOSEQ_EDIT ObjMgrProcLoad(OMPROC_EDIT,"New Edit Bioseq","BioseqEditor",OBJ_BIOSEQ,Seq_repr_raw,OBJ_BIOSEQ,Seq_repr_raw,NULL,SeqEditFunc,PROC_PRIORITY_DEFAULT)

#define REGISTER_NEW_SEQALIGN_EDIT ObjMgrProcLoad(OMPROC_EDIT, "New Edit SeqAlign","SeqAlignEditor",OBJ_SEQALIGN,0,OBJ_SEQALIGN,0,NULL,AlgEditFunc,PROC_PRIORITY_DEFAULT)

#define REGISTER_NEW_SEQANNOT_EDIT ObjMgrProcLoad(OMPROC_EDIT, "New Edit SeqAnnot","SeqAnnotEditor",OBJ_SEQANNOT,0,OBJ_SEQANNOT,0,NULL,AnnotAlgEditFunc,PROC_PRIORITY_DEFAULT)

#define REGISTER_NEW_SEQALIGN_VIEW ObjMgrProcLoad(OMPROC_VIEW, "New View SeqAlign","CCSeqAlignViewer",OBJ_SEQALIGN,0,OBJ_SEQALIGN,0,NULL,AlgViewFunc,PROC_PRIORITY_DEFAULT)


extern EditAlignDataPtr EditAlignDataRepopulateFromSeqAlign (PaneL pnl, EditAlignDataPtr adp, SeqAlignPtr salp);

extern void     SalsaPanelHasResized (PaneL pnl);
extern Boolean  FindIdsInSalsaPanel (PaneL pnl, PoinT pt, Uint2 *entityID, Uint2 *itemID, Uint2 *itemtype);
extern void     SaveSalsaPanel (PaneL pnl);
extern PaneL    SalsaTextPanel (GrouP g, Int2 wid, Int2 hgt);
extern void     PopulateSalsaPanel (PaneL pnl, SeqEntryPtr sep, Boolean all_seq, Uint1 sequence_shown, Uint1 show_feat, Uint1 numbering, FonT font);
extern void     repopulate_panel (WindoW w, EditAlignDataPtr adp, SeqAlignPtr salp);

extern Int2 LIBCALLBACK SeqEditFunc PROTO((Pointer data));
extern Int2 LIBCALLBACK AlgEditFunc PROTO((Pointer data));
extern Int2 LIBCALLBACK AnnotAlgEditFunc PROTO((Pointer data));
extern Int2 LIBCALLBACK AlgViewFunc (Pointer data);
extern void OldAlignmentEditor (IteM i);

extern void LaunchAnnotAlignEditor (SeqAnnotPtr sap);
extern void LaunchAlignEditor (SeqAlignPtr salp);
extern void LaunchAlignViewer (SeqAlignPtr salp);
extern Int2 LIBCALLBACK LaunchAlignEditorFromDesktop (Pointer data);
extern Int2 LIBCALLBACK LaunchAlignEditorFromDesktop2 (Pointer data);
extern Int2 LIBCALLBACK LaunchAlignEditorFromDesktop3 (Pointer data);
extern Int2 LIBCALLBACK LaunchAlignEditorFromDesktop4 (Pointer data);

extern Int2 LIBCALLBACK UpdateSeqAlign (Pointer data);

#endif


/*****************!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
* for debugging purpose, wdpext is a global SeqEditViewProcsPtr
* printEntityIDForMsg prints the OMUserDataPtr's
extern void printEntityIDForMsg (SeqEditViewFormPtr wdp, CharPtr str);
SeqEditViewFormPtr wdpext;
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
extern void printEntityIDForMsg (SeqEditViewFormPtr wdp, CharPtr str)
{
  EditAlignDataPtr   adp;
  AlignNodePtr    anp;
  ValNodePtr      vnp;
  OMUserDataPtr   omudp;
  Uint2           eID;
  SeqEntryPtr     sep;

  FILE *fp;

  if (wdp==NULL) return;
  GetPanelExtra (wdp->pnl, &adp);
  if ( adp == NULL )
     return ;
  if ( adp->seqnumber == 0 ) 
         return;

        fp = FileOpen("ObjMgr.log", "a");

fprintf (fp, "printEntityID %s\n", str);
  for (vnp=adp->anp_list; vnp!=NULL; vnp=vnp->next) {
   anp = (AlignNodePtr) vnp->data.ptrvalue;
   if (anp!=NULL) {
      sep = GetTopSeqEntryForEntityID (anp->seq_entityID);
      if (sep!=NULL) {
         eID = SeqMgrGetEntityIDForSeqEntry(sep);
         omudp = ObjMgrGetUserData (eID, wdp->procid, OMPROC_EDIT, wdp->userkey);
         if (omudp != NULL)
         {
fprintf(fp, "PRINTENTITYID-OMUDP %d %d %d %d\n", eID, anp->entityID, wdp->input_entityID, anp->seq_entityID);
            if (omudp->userdata.ptrvalue == NULL)
fprintf(fp, "    OMUDP->userdata.ptrvalue == NULL \n");
            else
fprintf(fp, "    OMUDP->userdata.ptrvalue NON NULL \n");
            if (omudp->messagefunc == NULL)
fprintf(fp, "    OMUDP->messagefunc == NULL \n");
            else
fprintf(fp, "    OMUDP->messagefunc NON NULL \n");
         }
         else {
            if (eID != wdp->input_entityID) {
fprintf(fp, "PRINTENTITYID-OMUDPNULL %d %d %d %d\n", eID, anp->entityID, wdp->input_entityID, anp->seq_entityID);
            }
         }
      }
   }
  }
  FileClose(fp);
}
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!!!!!!!!!!!!!!!!!******************/

