/* ingenext.c 

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
* File Name:  ingenext.c
*
* Author:  Fasika Aklilu
*
* Version Creation Date:   8/10/01
*
* $Revision: 6.8 $
*
* File Description:  runs external programs for ingenue
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* ==========================================================================
*/

#include <ingenext.h>



/************************************************

Function: Blast against database functions

Purpose: blast sequence in view against blastp or blastn database

************************************************/
WindoW progressW = NULL;

static Boolean LIBCALLBACK Ing_BlastCallback (BlastResponse *brp,
                                               Boolean *cancel)
{
    BlastProgressPtr bpp;
    Int4 completed = 0;
    Boolean retval;
    
    *cancel=FALSE;
    switch (brp->choice) {
    case BlastResponse_start:
        bpp = (BlastProgressPtr) brp->data.ptrvalue;
        if (bpp != NULL) {
            completed = bpp->completed;
        }
        if (completed == 0) {
            completed = 100;
        }
        return TRUE;
    case BlastResponse_progress:
        if (progressW != NULL) {
            bpp = (BlastProgressPtr) brp->data.ptrvalue;
            if (bpp != NULL) {
                completed = bpp->completed;
            }
            Ing_WinTimerProc(progressW);
        }
        return TRUE;
    case BlastResponse_queued:
      if (progressW != NULL) {
        bpp = (BlastProgressPtr) brp->data.ptrvalue;
        if (bpp != NULL) {
          completed = bpp->completed;
        }
        Ing_WinTimerProc(progressW);
      }
      return TRUE;
    case BlastResponse_done:
      return TRUE;
    default:
      break;
    }
    return FALSE;
}



static void Ing_MegaBlastDefaults(ButtoN b)
{
  Ing_BlastDlgData  * bddp;

  bddp=(Ing_BlastDlgData *)GetObjectExtra(b);

  if (GetStatus(b)){
    SetStatus(bddp->bGap, TRUE);
    SetTitle(bddp->wordsize, "28");
  }
  else {
    SetTitle(bddp->wordsize, "11");
  }
}


static void Ing_StdCancelProc(ButtoN b)
{
  Remove(ParentWindow(b));
}


static QBQUEUE  qbquerylist = NULL;
static QUEUE  urlquerylist = NULL;
static Int4 pendingqueries = 0;
static Int4 querytimerlimit = 100;
static Int4 querytimercount = 0;
static Uint2 query_entityID = 0;
static Uint2 query_itemID = 0;
static Int2  max_tokeep = 20;

extern void IngenueCheckSocketsProc (WindoW w)

{
  Int4  remaining;

  remaining = QUERY_CheckQueue (&urlquerylist);
  if (remaining < pendingqueries) {
    Beep ();
    pendingqueries--;
  }
  QBlastCheckQueue (&qbquerylist);
  
}


static void LIBCALLBACK Ing_QBlastAnnounce (CharPtr requestID, CharPtr seqID, Int2 estimatedSeconds)

{
  if (StringHasNoText (requestID)) {
    requestID = "?";
  }
  if (StringHasNoText (seqID)) {
    seqID = "?";
  }
  Message (MSG_POST, "Queued rID %s, seqID %s, estimated seconds = %d",
           requestID, seqID, (int) estimatedSeconds);
}

static SeqAlignPtr Ing_KeepTheBestAlignments(SeqAlignPtr sap)
{
  SeqAlignPtr   sap_new = NULL;
  SeqAlignPtr   salp_prev = NULL, salp_head = NULL;
  AMAlignIndex2Ptr amaip = NULL;
  SCP_ResultPtr scp;
  SCP_ResultPtr scp_large;
  SCP_ResultPtr scp_small;
  Int4          i;
  Int4          nkept = 0;

  if (sap->saip != NULL)
    AMAlignIndex2Free2(sap);
  sap->saip = NULL;
  AlnMgr2IndexLite(sap);
  AlnMgr2SortAlnSetByNthRowPos(sap, 1);
  
  amaip = (AMAlignIndex2Ptr)sap->saip;
  scp = (SCP_ResultPtr) MemNew(sizeof(SCP_Result));
  scp_large = (SCP_ResultPtr) MemNew(sizeof(SCP_Result));
  scp_small = (SCP_ResultPtr) MemNew(sizeof(SCP_Result));

  SCP_OrganizeAlnsInSet(sap, 50, scp, 1);
  SCP_OrganizeAlnsInSet(sap, 50, scp_large, 1);
  SCP_OrganizeAlnsInSet(sap, 50, scp_small, 1);

  for (i=0; i<scp->numsaps; i++)
    {
      if (scp->saps[i] != NULL)
        {
          scp->saps[i]->next = NULL;
          if (nkept <= max_tokeep){
            if (salp_prev != NULL)
              {
                salp_prev->next = scp->saps[i];
                salp_prev = salp_prev->next;
              } else
                salp_head = salp_prev = scp->saps[i];
            nkept++;
          }
          else {
            SeqAlignFree(scp->saps[i]);
          }
        }
    }

  for (i=0; i<scp->numlarge_outliers  && nkept <= max_tokeep; i++){
    if (scp->large_outliers[i] != NULL)
      {
        scp->large_outliers[i]->next = NULL;
        if (nkept <= max_tokeep){
          if (salp_prev != NULL)
            {
              salp_prev->next = scp->large_outliers[i];
              salp_prev = salp_prev->next;
            } else
              salp_head = salp_prev = scp->large_outliers[i];
          nkept++;
        }
        else {
          SeqAlignFree(scp->large_outliers[i]);
        }
      }
  }

  for (i=0; i<scp->numsmall_outliers; i++){
    if (scp->small_outliers[i] != NULL)
      {
        scp->small_outliers[i]->next = NULL;
        if (nkept <= max_tokeep){
          if (salp_prev != NULL)
            {
              salp_prev->next = scp->small_outliers[i];
              salp_prev = salp_prev->next;
            } else
              salp_head = salp_prev = scp->small_outliers[i];
          nkept++;
        }
        else {
          SeqAlignFree(scp->small_outliers[i]);
        }
      }
  }

  sap_new = SeqAlignNew();
  sap_new->segtype = SAS_DISC;
  sap_new->segs = (Pointer)(salp_head);
  sap_new->saip = NULL;
  AlnMgr2IndexLite(sap_new);

  return sap_new;
}

static Boolean Ing_HandleQBlastResults(CharPtr filename){
  /* read in seqannot file */
  FILE *fp;
  SeqAnnotPtr  sanp = NULL;
  BioseqPtr    bsp = NULL;
  Pointer      dataptr = NULL;
  Uint2        datatype;
  ValNodePtr   vnp = NULL, head = NULL;
  ValNodePtr   vnp_cur = NULL;
  SeqAlignPtr  sap = NULL;
  SeqAlignPtr  sap_best = NULL;
  Boolean      bSaved = FALSE;

  if (filename == NULL) return (FALSE);
  fp = FileOpen(filename, "r");
  if (fp == NULL) return (FALSE);
  while (dataptr = ReadAsnFastaOrFlatFile(fp, &datatype, NULL, FALSE, TRUE, TRUE, TRUE)){
    if (datatype != OBJ_SEQANNOT || dataptr != NULL){
      if (head == NULL)
        vnp_cur = ValNodeAddPointer(&head, datatype, dataptr);
      else 
        vnp_cur = ValNodeAddPointer(&vnp_cur, datatype, dataptr);
    }
  }
  for (vnp = head; vnp != NULL; vnp = vnp->next){
    dataptr = (Pointer)vnp->data.ptrvalue;
    if (vnp->choice == OBJ_SEQANNOT){
      sanp = (SeqAnnotPtr)(dataptr);
      sap = (SeqAlignPtr)sanp->data;
      if (!sap) return (FALSE);

      /* weed out low scoring hits */
      sap_best = Ing_KeepTheBestAlignments(sap);
      /* free old parent */
      AMAlignIndex2Free2(sap);
      sap->segs = NULL;
      SeqAlignFree(sap);

      sanp->data = (Pointer) sap_best;
      sanp->name = StringSave("Blast vs. db");
      bsp = (BioseqPtr)GetBioseqGivenIDs(query_entityID, query_itemID, OBJ_BIOSEQ);
      Ing_AttachSeqAnnotToSeqEntry(query_entityID, sanp, bsp);
      bSaved = TRUE;
    }
  }
  if (query_entityID != 0 && bSaved){
    ObjMgrSetDirtyFlag(query_entityID, TRUE);
    ObjMgrSendMsg(OM_MSG_UPDATE, query_entityID, 0, 0);
  }

  FileClose(fp);
  Beep();
  if (bSaved)
    Message (MSG_OK, "QBlast task is complete");
  else
    Message (MSG_OK, "QBlast task is complete -- no hits found");

  return TRUE;
}


static Boolean LIBCALLBACK Ing_QBlastCallback (
  CharPtr filename,
  VoidPtr userdata,
  CharPtr requestID,
  CharPtr seqID,
  Boolean success
)

{
  if (StringHasNoText (requestID)) {
    requestID = "?";
  }
  if (StringHasNoText (seqID)) {
    seqID = "?";
  }
  if (success) {
    Ing_HandleQBlastResults (filename);
  } else {
    Message (MSG_POST, "Failure of rID '%s', seqID %s", requestID, seqID);
  }
  return TRUE;
}


static void Ing_QBlastAccept(ButtoN g)
{
    Ing_BlastDlgData *bddp;
    Int2 value;
    WindoW hOpenDlg;
    Int4 i;
    ValNodePtr pvn;
    BioseqPtr  bsp;
    Char max[11]={""}, *program;
    IngGenomeViewerPtr igvp=NULL;

    hOpenDlg=(WindoW)ParentWindow(g);
    if (!hOpenDlg) return;
    
    Hide(hOpenDlg);
    bddp=(Ing_BlastDlgData *)GetObjectExtra (g);
    igvp=(IngGenomeViewerPtr)GetObjectExtra(ParentWindow(g));
    if (bddp == NULL || igvp==NULL) {
        ValNodeFree(bddp->pvnSips);
        Remove(hOpenDlg);
        return;
    }
    

/*     BlastInit("Ingenue", &bl3hp, &bl3rp); */
    value = GetValue(bddp->bsp_list);

    for(pvn = bddp->pvnSips, i = 1; i != value && pvn != NULL;
        i++, pvn = pvn->next);

    if(pvn == NULL) {
        ValNodeFree(bddp->pvnSips);
        Remove(hOpenDlg);
        return;
    }

    bsp = BioseqLockById((SeqId *)pvn->data.ptrvalue);
    query_entityID = igvp->entityID;
    query_itemID = igvp->itemID;

    if(bsp == NULL) {
        ValNodeFree(bddp->pvnSips);
        return;
    }

    if(ISA_aa(bsp->mol)) program = "blastp";
    else program = "blastn";

    GetTitle(bddp->tMax, max, 10);
    max_tokeep = atoi(max);

    value = GetValue(bddp->db_list);
    QBlastAsynchronousRequest (ig_nucdbs[value-1], program, bsp, &qbquerylist, Ing_QBlastCallback, Ing_QBlastAnnounce, NULL);
    Remove(hOpenDlg);
}



extern void Ing_QBlastProc(IteM t)
{
  Ing_BlastDlgData     *bddp;
  GrouP                h2,h,h1, h3, h4, h5, maskg;
  WindoW               d;
  ButtoN               b;
  Char                 szName[31]={""};
  IngGenomeViewerPtr   igvp=NULL;
  ValNode             *pvn;
  Int4                 i;

    bddp = (Ing_BlastDlgData *)MemNew(sizeof(Ing_BlastDlgData));
    if (bddp == NULL) return;
    igvp=(IngGenomeViewerPtr)GetObjectExtra(ParentWindow(t));
    bddp->pvnSips = igvp->entity_list->Sips;
    d = MovableModalWindow(-50, -33 ,-10, -10, "Blast", NULL);
    if (d != NULL){
        /*create some controls*/
        h = HiddenGroup(d, 1, 5,  NULL);
        h1 = HiddenGroup(h, 1, 2,  NULL);
        StaticPrompt(h1,"Choose a sequence :",0,0,programFont,'l');
        
        bddp->bsp_list = SingleList(h1,20,6,NULL);

        h3 = HiddenGroup(h, 1, 1, NULL);
        bddp->bGap = CheckBox(h3, "Gapped", NULL);
        SetStatus(bddp->bGap, TRUE);

        h4 = HiddenGroup(h, 2, 2, NULL);
        StaticPrompt(h4,"Expectation value:",0,0,programFont,'l');
        bddp->tExpect = DialogText(h4, "10", 8, (TxtActnProc) NULL); 
/*         StaticPrompt(h4,"Maximum # of hits:",0,0,programFont,'l'); */
/*         bddp->tMax = DialogText(h4, "20", 5, (TxtActnProc) NULL); */
        StaticPrompt(h4,"Wordsize:",0,0,programFont,'l');
        bddp->wordsize = DialogText(h4, "11", 5, (TxtActnProc) NULL);
        maskg=HiddenGroup(h4, 0, 2, NULL);
        bddp->maskrep = CheckBox(maskg, "Mask Repeats", NULL);
        SetStatus(bddp->maskrep, FALSE);
        bddp->masksimple = CheckBox(maskg, "Mask Simple Sequence", NULL);
        SetStatus(bddp->masksimple, TRUE);
        bddp->megablast= CheckBox(h4, "Mega Blast", Ing_MegaBlastDefaults);
        SetStatus(bddp->megablast, FALSE);
        SetObjectExtra (bddp->megablast, (Pointer) bddp, NULL);
        Disable(h4); /* until QBlast can take input parameters */

        h4 = HiddenGroup (h, 2, 2, NULL);
 
        StaticPrompt(h4,"# of hits to keep:",0,0,programFont,'l');
        bddp->tMax = DialogText(h4, "20", 5, (TxtActnProc) NULL);

        StaticPrompt (h4, "Choose Database:", 0, 0, programFont, 'c');
        
        bddp->db_list = PopupList(h4, TRUE, NULL);

        for (i = 0; i < Ing_NUM_DBS && ig_nucdbs [i] != NULL; i++)
          PopupItem(bddp->db_list, ig_nucdbsnames[i]);
        SetValue(bddp->db_list,1);

        h2 = HiddenGroup(h, 2, 1, NULL);
        b=DefaultButton(h2, "Accept", Ing_QBlastAccept);
        Enable(b);
        PushButton(h2, "Cancel", Ing_StdCancelProc);
        SetObjectExtra (b, (Pointer) bddp, StdCleanupExtraProc);

        h5 = HiddenGroup(h, 1, 1, NULL);
        StaticPrompt(h5,"Warning: may take several minutes!",0,0,programFont,'l');

        /*fill in the list box*/
        for(pvn = bddp->pvnSips; pvn != NULL; pvn = pvn->next) {
            SeqIdWrite((SeqId *)pvn->data.ptrvalue,szName,
                PRINTID_FASTA_LONG,30);
            ListItem(bddp->bsp_list, szName);
        }
        SetValue(bddp->bsp_list,1);
        
        SetObjectExtra (d, (Pointer) igvp, NULL);
        RealizeWindow(d);
        Select(d);
        Show(d);
    }

}

/************************************************

Function: tRNAscan functions

Purpose: run tRNAscan (url), check for, and handle results

************************************************/

typedef struct trsdata {
  Boolean fastaNucOK;
  Boolean fastaProtOK;
  Boolean onlyBspTarget;
  CharPtr host_machine;
  Uint2   host_port;
  CharPtr host_path;
  Uint2   timeoutsec;
  Int2    format;
  ValNodePtr paramlist;
  QueryResultProc resultproc;
  CharPtr prefix;
  CharPtr suffix;
  CharPtr title;
  GrouP   controls;
  IngGenomeViewerPtr igvp;
} tRSdata, PNTR tRSdataPtr;

typedef struct iurlparamdata {
  Uint1          type;     /* 1 = text, 2 = checkbox, 3 = popup, 4 = radio, 5 = list */
  CharPtr        param;
  CharPtr        prompt;   /* if no prompt, use param */
  CharPtr        dfault;
  CharPtr        choices;  /* choices if param is popup */
  CharPtr        group;    /* used for grouping related controls */
  CharPtr        help;
} IUrlParamData, PNTR IUrlParamPtr;

static Uint2 urlquery_entityID = 0;
static Uint2 urlquery_itemID = 0;

static void Ing_HandleResults(CharPtr path)
{
  FILE *fp;
  SeqAnnotPtr  sanp = NULL;
  BioseqPtr    bsp = NULL;
  Pointer      dataptr = NULL;
  Uint2        datatype;
  ValNodePtr   vnp = NULL, head = NULL;
  ValNodePtr   vnp_cur = NULL;
  Boolean      bSaved = FALSE;

  if (path == NULL) return;
  fp = FileOpen(path, "r");
  if (fp == NULL) return;
  while (dataptr = ReadAsnFastaOrFlatFile(fp, &datatype, NULL, FALSE, TRUE, TRUE, TRUE)){
    if (datatype != OBJ_SEQANNOT || dataptr != NULL){
      if (head == NULL)
        vnp_cur = ValNodeAddPointer(&head, datatype, dataptr);
      else 
        vnp_cur = ValNodeAddPointer(&vnp_cur, datatype, dataptr);
    }
  }
  for (vnp = head; vnp != NULL; vnp = vnp->next){
    dataptr = (Pointer)vnp->data.ptrvalue;
    if (vnp->choice == OBJ_SEQANNOT){
      sanp = (SeqAnnotPtr)(dataptr);
      sanp->name = (CharPtr)StringSave("tRNAscan-SE");
      bsp = (BioseqPtr)GetBioseqGivenIDs(urlquery_entityID, urlquery_itemID, OBJ_BIOSEQ);
      Ing_AttachSeqAnnotToSeqEntry(urlquery_entityID, sanp, bsp);
      bSaved = TRUE;
    }
  }
  if (urlquery_entityID != 0 && bSaved){
    ObjMgrSetDirtyFlag(urlquery_entityID, TRUE);
    ObjMgrSendMsg(OM_MSG_UPDATE, urlquery_entityID, 0, 0);
  }

  FileClose(fp);
}

static Boolean LIBCALLBACK Ing_HandletRNAscanResults (CONN conn, VoidPtr userdata, EIO_Status status)
{
  FILE  *fp;
  Char  path [PATH_MAX];

  TmpNam (path);
  fp = FileOpen (path, "w");
  QUERY_CopyResultsToFile (conn, fp);
  FileClose (fp);
  Ing_HandleResults(path);
  FileRemove (path);
  return TRUE;

}

static void Ing_FinishURLProc (tRSdataPtr trs, CharPtr arguments, CharPtr path)
{
  CONN             conn;
  FILE             *fp;
  Char             progname [50]={"INGENUE"};

  QueryResultProc  resultproc;
  EMIME_SubType    subtype;
  Int4             n_written;
  Char             str [256];


  resultproc = trs->resultproc;

  if (trs->format == 1) {
    subtype = eMIME_Fasta;
  } else if (trs->format == 2) {
    subtype = eMIME_AsnText;
  } else {
    subtype = eMIME_Unknown;
  }

  conn = QUERY_OpenUrlQuery (trs->host_machine, trs->host_port,
                             trs->host_path, arguments,
                             progname, trs->timeoutsec,
                             eMIME_T_NcbiData, subtype, eENCOD_Url,
                             fHCC_UrlDecodeInput | fHCC_UrlEncodeOutput);
  if (conn == NULL) return;

  fp = FileOpen (path, "rb");
  QUERY_CopyFileToQuery (conn, fp);
  FileClose (fp);

  QUERY_SendQuery (conn);
  QUERY_AddToQueue (&urlquerylist, conn, resultproc, NULL, TRUE);
  pendingqueries++;
  FileRemove (path);
}

extern Boolean Ing_CanSaveFasta (IngGenomeViewerPtr igvp, Boolean nucs, Boolean prots, Boolean onlyTarget)
{
  Uint2              entityID;
  SeqEntryPtr        sep;
  BioseqPtr          bsp;

  if (igvp != NULL) {
    bsp = igvp->bsp;
    if (bsp == NULL) return FALSE;
    sep = SeqMgrGetSeqEntryForData (bsp);
    if (! onlyTarget) {
      entityID = ObjMgrGetEntityIDForChoice (sep);
      sep = GetTopSeqEntryForEntityID (entityID);
    }
    if (sep == NULL) return FALSE;
    if (nucs && SeqEntryHasNucs (sep)) return TRUE;
    if (prots && SeqEntryHasProts (sep)) return TRUE;
  }
  return FALSE;
}


extern Boolean Ing_ExportFasta (IngGenomeViewerPtr igvp, CharPtr filename, Boolean nucs, Boolean prots, Boolean onlyTarget)

{
  BioseqPtr          bsp;
  Uint2              entityID;
  FILE               *fp;
  Uint1              group_segs;
  Char               path [PATH_MAX];
  SeqEntryPtr        sep;

  path [0] = '\0';
  StringNCpy_0 (path, filename, sizeof (path));
  bsp = igvp->bsp;
  if (bsp == NULL) return FALSE;
  sep = SeqMgrGetSeqEntryForData (bsp);
  if (! onlyTarget) {
    entityID = ObjMgrGetEntityIDForChoice (sep);
    sep = GetTopSeqEntryForEntityID (entityID);
  }
  if (path [0] != '\0' || GetOutputFileName (path, sizeof (path), NULL)) {
#ifdef WIN_MAC
    fp = FileOpen (path, "r");
    if (fp != NULL) {
      FileClose (fp);
    } else {
      FileCreate (path, "TEXT", "ttxt");
    }
#endif
    fp = FileOpen (path, "w");
    if (fp != NULL) {
      WatchCursor ();
      Update ();
      group_segs = 0;
      if (bsp->repr == Seq_repr_seg) {
        group_segs = 2;
      }
      if (nucs) {
        SeqnSeqEntrysToFasta (sep, fp, TRUE, group_segs);
      }
      if (prots) {
        SeqnSeqEntrysToFasta (sep, fp, FALSE, 0);
      }
      FileClose (fp);
      ArrowCursor ();
      Update ();
      return TRUE;
    }
  }
  
  return FALSE;
}

static void Ing_DoAnalysisProc(tRSdataPtr trs, CharPtr arguments)
{
  BioseqPtr bsp;
  Char      path1[PATH_MAX];
  SeqEntryPtr  sep;
  AsnIoPtr     aop;
  IngGenomeViewerPtr igvp = NULL;


  igvp = trs->igvp;
  if (Ing_CanSaveFasta (igvp, trs->fastaNucOK, trs->fastaProtOK, trs->onlyBspTarget)) {
    TmpNam (path1);
    switch (trs->format){
    case 1: 
      Ing_ExportFasta (igvp , path1, trs->fastaNucOK, trs->fastaProtOK, trs->onlyBspTarget);
      break;
    case 2 :
      sep = NULL;
      if (trs->onlyBspTarget) {
        bsp =  GetBioseqGivenIDs (igvp->entityID, igvp->itemID, OBJ_BIOSEQ);
        sep = SeqMgrGetSeqEntryForData (bsp);
      } else {
        sep = GetTopSeqEntryForEntityID (igvp->entityID);
      }
      if (sep != NULL) {
        aop = AsnIoOpen (path1, "w");
        SeqEntryAsnWrite (sep, aop, NULL);
        AsnIoFlush (aop);
        AsnIoClose (aop);
      }
      break;
    default :
      break;
    }
    urlquery_entityID = igvp->entityID;
    urlquery_itemID = igvp->itemID;
    Ing_FinishURLProc (trs, arguments, path1);

  } else {
    ErrPostEx (SEV_ERROR, 0, 0, "BioseqView cannot save fasta format");
    ErrShow();
  }
}


static void Ing_AccepttRNAargsProc(ButtoN b)
{
  CharPtr        args = NULL;
  CharPtr        arguments = NULL;
  GrouP          grp;
  tRSdataPtr     trs = NULL;
  ValNodePtr     head = NULL;
  ValNodePtr     paramlist = NULL;
  ValNodePtr     ppt;
  ValNodePtr     vnp = NULL;
  Int2           val;
  Int2           i;
  Char           ch;
  Char           cpy [256];
  CharPtr        last;
  Int4           len;
  Char           tmp [256];
  CharPtr        itms;
  CharPtr        ptr;
  Boolean        notFirst = FALSE;
  IUrlParamPtr   upp = NULL;
  WindoW         w;

  
  w = ParentWindow(b);
  trs = (tRSdataPtr)GetObjectExtra(w);
  if (!trs) return;
  Hide(w);
   if (! StringHasNoText (trs->prefix)) {
      ValNodeCopyStr (&head, 0, trs->prefix);
    }
   ppt = trs->paramlist;
   upp = (IUrlParamPtr) ppt->data.ptrvalue;

   grp = trs->controls;
   val = GetValue (grp);
   if (val > 0) {
     i = 0;
     itms = upp->choices;
     StringNCpy_0 (tmp, itms, sizeof (tmp));
     last = tmp;
     ptr = last;
     ch = *ptr;
     while (ch != '\0') {
       if (ch == ',') {
         *ptr = '\0';
         i++;
         if (val == i) {
           sprintf (cpy, "%s=%s", upp->param, last);
           ValNodeCopyStr (&head, 0, cpy);
         }
         ptr++;
         last = ptr;
         ch = *ptr;
       } else {
         ptr++;
         ch = *ptr;
       }
     }
     if (! StringHasNoText (last)) {
       i++;
       if (val == i) {
         sprintf (cpy, "%s=%s", upp->param, last);
         ValNodeCopyStr (&head, 0, cpy);
       }
     }
   }
    if (! StringHasNoText (trs->suffix)) {
      ValNodeCopyStr (&head, 0, trs->suffix);
    }
    for (len = 0, vnp = head; vnp != NULL; vnp = vnp->next) {
      len += StringLen ((CharPtr) vnp->data.ptrvalue) + 1;
    }
    if (len > 0) {
      arguments = MemNew (len + 5);
      if (arguments != NULL) {
        vnp = head;
        notFirst = FALSE;
        while (vnp != NULL) {
          if (notFirst) {
            StringCat (arguments, "&");
          }
          StringCat (arguments, (CharPtr) vnp->data.ptrvalue);
          notFirst = TRUE;
          vnp = vnp->next;
        }
      }
    }
    args = StringSave (arguments);
    MemFree (arguments);
    Ing_DoAnalysisProc (trs, args);
    MemFree (args);
    Remove(w);
}

static ValNodePtr Ing_RearrangeParamList (ValNodePtr paramlist)

{
  ValNodePtr       curr;
  CharPtr          group;
  ValNodePtr       head = NULL;
  ValNodePtr       list;
  ValNodePtr       next;
  ValNodePtr PNTR  prev;
  ValNodePtr       ppt;
  IUrlParamPtr      upp;

  ppt = paramlist;
  while (ppt != NULL) {
    list = ppt->next;
    ppt->next = NULL;
    ValNodeLink (&head, ppt);
    upp = (IUrlParamPtr) ppt->data.ptrvalue;
    if (upp == NULL) {
      ppt = list;
      continue;
    }
    group = upp->group;
    curr = list;
    prev = &list;
    while (curr != NULL) {
      next = curr->next;
      upp = (IUrlParamPtr) curr->data.ptrvalue;
      if (upp == NULL) {
        prev = &(curr->next);
        curr = next;
        continue;
      }
      if (StringICmp (upp->group, group) == 0) {
        *prev = next;
        curr->next = NULL;
        ValNodeLink (&head, curr);
      } else {
        prev = &(curr->next);
      }
      curr = next;
    }
    ppt = list;
  }
  return head;
}

static void Ing_ShowArgumentHelp (ButtoN b)

{
  ValNodePtr     ppt;
  CharPtr        str;
  tRSdataPtr     trs;
  IUrlParamPtr    upp;
  ValNodePtr     vnp;

  trs = (tRSdataPtr) GetObjectExtra (ParentWindow(b));
  if (trs == NULL) return;

  ppt = trs->paramlist;
  upp = (IUrlParamPtr) ppt->data.ptrvalue;
  if (upp == NULL) return;
  str = upp->help;
  Message (MSG_OK, "%s", str);
  return;
  
}

static void Ing_tRNAscanProc(IteM i)
{
  CharPtr       args = NULL;
  size_t        len;
  tRSdataPtr    trs = NULL;

  trs = (tRSdataPtr) GetObjectExtra (i);
  if (trs == NULL) return;
  {{ /* build arguement form */
  ButtoN             b;
  ButtoN             btn;
  GrouP              c;
  Char               ch;
  CharPtr            def;
  Int2               delta;
  TexT               first = NULL;
  GrouP              g;
  GrouP              grp;
  GrouP              h;
  Int2               i;
  CharPtr            itms;
  CharPtr            last;
  CharPtr            lastGroup = " ";
  LisT               lst;
  GrouP              m;
  Int2               max;
  Int2               min;
  ValNodePtr         moveMe = NULL;
  Nlm_Handle         obj1, obj2;
  PopuP              pop;
  PrompT             prmpt;
  ValNodePtr         ppt;
  CharPtr            ptr;
  RecT               r;
  StdEditorProcsPtr  sepp;
  CharPtr            str;
  Char               tmp [128];
  TexT               txt;
  IUrlParamPtr        upp;
  Int2               val;
  ValNodePtr         vnp;
  WindoW             w;


  trs->paramlist = Ing_RearrangeParamList (trs->paramlist);
 
  w = FixedWindow (-50, -33, -10, -10, "Arguments", NULL);
  SetObjectExtra (w, trs, NULL);
  
  m = HiddenGroup (w, 1, 0, NULL);

  g = NULL;
  for (ppt = trs->paramlist;
       ppt != NULL;
       ppt = ppt->next) {
    upp = (IUrlParamPtr) ppt->data.ptrvalue;
    if (upp == NULL) continue;
    if (StringICmp (upp->group, lastGroup) != 0) {
      if (StringHasNoText (upp->group)) {
        if (StringHasNoText (lastGroup)) {
          g = HiddenGroup (m, 3, 0, NULL);
        } else {
          g = NormalGroup (m, 3, 0, "", programFont, NULL);
        }
      } else {
        g = NormalGroup (m, 3, 0, upp->group, programFont, NULL);
      }
      lastGroup = upp->group;
    }
    if (g == NULL) {
      g = HiddenGroup (m, 3, 0, NULL);
    }

        str = upp->prompt;
        StaticPrompt (g, str, 0, dialogTextHeight, programFont, 'l');
        h = HiddenGroup (g, 1, 0, NULL);
        grp = HiddenGroup (h, -3, 0, NULL);
        def = upp->dfault;
        val = 0;
        i = 0;
        itms = upp->choices;
        StringNCpy_0 (tmp, itms, sizeof (tmp));
        last = tmp;
        ptr = last;
        ch = *ptr;
        while (ch != '\0') {
          if (ch == ',') {
            *ptr = '\0';
            RadioButton (grp, last);
            i++;
            if (StringICmp (def, last) == 0) {
              val = i;
            }
            ptr++;
            last = ptr;
            ch = *ptr;
          } else {
            ptr++;
            ch = *ptr;
          }
        }
        if (! StringHasNoText (last)) {
          RadioButton (grp, last);
          i++;
          if (StringICmp (def, last) == 0) {
            val = i;
          }
        }
        if (val > 0) {
          SetValue (grp, val);
        }
        trs->controls = grp;
  }


  b = PushButton (g, "?", Ing_ShowArgumentHelp);
  c = HiddenGroup (w, 2, 0, NULL);
  SetGroupSpacing (c, 10, 3);
  b = PushButton (c, "Accept", Ing_AccepttRNAargsProc);
  PushButton (c, "Cancel", Ing_StdCancelProc);

  AlignObjects (ALIGN_CENTER, (HANDLE) m, (HANDLE) c, NULL);
  RealizeWindow (w);
  Show (w);
  Select (w);
  }}
}


static void Ing_AddtRNAscanItem (MenU m, Boolean nucOK, Boolean protOK,  Boolean onlyBspTarget, CharPtr host, Uint2 port, CharPtr path, CharPtr program, Uint2 timeoutsec, Int2 format, QueryResultProc resultproc, ValNodePtr paramlist,CharPtr prefix,CharPtr suffix,CharPtr title, ItmActnProc actn)
{
  IteM i;
  tRSdataPtr trs = NULL;
  IngGenomeViewerPtr  igvp = NULL;
  Int4       len;
  CharPtr    tmp = NULL;
  
  igvp = (IngGenomeViewerPtr)GetObjectExtra(ParentWindow(m));
  if (!igvp) return;
  
  trs = (tRSdataPtr)MemNew(sizeof(tRSdata));
  trs->igvp = igvp;
  trs->resultproc = resultproc;
  trs->fastaNucOK = nucOK;
  trs->fastaProtOK = protOK;
  trs->onlyBspTarget = onlyBspTarget;
  trs->host_machine = StringSave(host);
  trs->host_port = port;
  len = StringLen(path);
  tmp = MemNew(len + StringLen(program) + 5);
  if (tmp != NULL){
    StringCpy(tmp, path);
    if (len > 1 && tmp[len-1] != '/'){
      StringCat(tmp, "/");
    }
    StringCat(tmp, program);
  }
  trs->host_path = StringSave(tmp);
  MemFree(tmp);
  trs->timeoutsec = timeoutsec;
  trs->format = format;
  trs->paramlist = paramlist;
  trs->prefix = StringSaveNoNull(prefix);
  trs->suffix = StringSaveNoNull(suffix);

  i = CommandItem(m, title, actn);
  SetObjectExtra(i, (Pointer)trs, StdCleanupExtraProc);

}

static ValNodePtr Ing_GetConfigParams (CharPtr sect)
{

  Int2         i;
  ValNodePtr   paramlist = NULL;
  Uint1        paramtype;
  Char         title [512];
  Char         tmp [32];
  IUrlParamPtr  upp;

  if (sect == NULL) return NULL;
  i = 1;
  sprintf (tmp, "PARAM_%d", (int) i);
  while (GetAppParam ("SEQNCGIS", sect, tmp, NULL, title, sizeof (title) - 1)) {
    upp = (IUrlParamPtr) MemNew (sizeof (IUrlParamData));
    if (upp == NULL) continue;
    upp->param = StringSave (title);
    sprintf (tmp, "TYPE_%d", (int) i);
    paramtype = 1;
    if (GetAppParam ("SEQNCGIS", sect, tmp, NULL, title, sizeof (title) - 1)) {
      if (StringICmp (title, "text") == 0) {
        paramtype = 1;
      } else if (StringICmp (title, "checkbox") == 0) {
        paramtype = 2;
      } else if (StringICmp (title, "popup") == 0) {
        paramtype = 3;
      } else if (StringICmp (title, "radio") == 0) {
        paramtype = 4;
      } else if (StringICmp (title, "list") == 0) {
        paramtype = 5;
      }
    }
    upp->type = paramtype;
    sprintf (tmp, "PROMPT_%d", (int) i);
    if (GetAppParam ("SEQNCGIS", sect, tmp, NULL, title, sizeof (title) - 1)) {
      upp->prompt = StringSave (title);
    } else {
      upp->prompt = StringSave (upp->param);
    }
    sprintf (tmp, "DEFAULT_%d", (int) i);
    if (GetAppParam ("SEQNCGIS", sect, tmp, NULL, title, sizeof (title) - 1)) {
      upp->dfault = StringSave (title);
    } else {
      upp->dfault = StringSave (" ");
    }
    sprintf (tmp, "CHOICES_%d", (int) i);
    if (GetAppParam ("SEQNCGIS", sect, tmp, NULL, title, sizeof (title) - 1)) {
      upp->choices = StringSave (title);
    } else {
      upp->choices = StringSave (" ");
    }
    sprintf (tmp, "GROUP_%d", (int) i);
    if (GetAppParam ("SEQNCGIS", sect, tmp, NULL, title, sizeof (title) - 1)) {
      upp->group = StringSave (title);
    } else {
      upp->group = StringSave (" ");
    }
    sprintf (tmp, "HELP_%d", (int) i);
    if (GetAppParam ("SEQNCGIS", sect, tmp, NULL, title, sizeof (title) - 1)) {
      upp->help = StringSave (title);
    } else {
      upp->help = StringSave (" ");
    }
    ValNodeAddPointer (&paramlist, i, (Pointer) upp);
    i++;
    sprintf (tmp, "PARAM_%d", (int) i);
  }
  return paramlist;

}


static void Ing_ReadtRNAConfigFile(MenU m)
{
  Boolean     demomode = FALSE;
  Int2        format = 1;
  Char        host [128];
  Boolean     nucOK = FALSE;
  Boolean     onlyBspTarget = FALSE;
  ValNodePtr  paramlist = NULL;
  Char        program [128];
  Char        path [256];
  Uint2       port = 80;
  Char        prefix [128];
  Boolean     protOK = FALSE;
  Char        sect[256];
  Char        submenu [128];
  Char        suffix [128];
  Uint2       timeoutsec = 30;
  Char        title [128];
  Char        tmp [32];
  Uint4       val;

    if (! GetAppParam ("SEQNCGIS", "ORDER", "ORDER_1", NULL, sect, sizeof (sect) - 1)){
      return;
    }
    if (! GetAppParam ("SEQNCGIS", sect, "TITLE", NULL, title, sizeof (title) - 1)) {
    StringNCpy_0 (title, sect, sizeof (title));
  }
  if (GetAppParam ("SEQNCGIS", sect, "HOST", NULL, host, sizeof (host) - 1)) {
    if (GetAppParam ("SEQNCGIS", sect, "FLAGS", NULL, tmp, sizeof (tmp) - 1)) {
      if (StringStr (tmp, "NUC") != NULL) {
        nucOK = TRUE;
      }
      if (StringStr (tmp, "PRT") != NULL) {
        protOK = TRUE;
      }
      if (StringStr (tmp, "TRG") != NULL) {
        onlyBspTarget = TRUE;
      }
    }

    if (GetAppParam ("SEQNCGIS", sect, "PORT", NULL, tmp, sizeof (tmp) - 1) && 
        sscanf (tmp, "%u", &val) == 1) {
      port = (Uint2) val;
    } else {
      port = 80;
    }
    if (GetAppParam ("SEQNCGIS", sect, "FORMATIN", NULL, tmp, sizeof (tmp) - 1)) {
      if (StringICmp (tmp, "FASTA") == 0) {
        format = 1;
      } else if (StringICmp (tmp, "ASN.1") == 0) {
        format = 2;
      }
    }
    if (GetAppParam ("SEQNCGIS", sect, "TIMEOUT", NULL, tmp, sizeof (tmp) - 1) && 
        sscanf (tmp, "%u", &val) == 1) {
      timeoutsec = (Uint2) val;
    } else {
      timeoutsec = 30;
    }
    submenu [0] = '\0';
    GetAppParam ("SEQNCGIS", sect, "SUBMENU", NULL, submenu, sizeof (submenu) - 1);
    if (GetAppParam ("SEQNCGIS", sect, "DEMO", NULL, tmp, sizeof (tmp) - 1)) {
      if (StringICmp (tmp, "TRUE") == 0) {
        demomode = TRUE;
      }
    }

    if (GetAppParam ("SEQNCGIS", sect, "PATH", NULL, path, sizeof (path) - 1)) {
      if (GetAppParam ("SEQNCGIS", sect, "PROGRAM", NULL, program, sizeof (program) - 1)) {
        paramlist = Ing_GetConfigParams (sect);
        prefix [0] = '\0';
        GetAppParam ("SEQNCGIS", sect, "PREFIX", NULL, prefix, sizeof (prefix) - 1);
        suffix [0] = '\0';
        GetAppParam ("SEQNCGIS", sect, "SUFFIX", NULL, suffix, sizeof (suffix) - 1);
        Ing_AddtRNAscanItem (m,
                         nucOK, protOK, onlyBspTarget,
                         host, port, path, program, timeoutsec, format,
                         Ing_HandletRNAscanResults, paramlist, prefix, suffix,
                         title, Ing_tRNAscanProc);
      }
    }
  }

}

extern void Ing_tRNAscanMenu (MenU m)
{

  ValNodePtr    head1 = NULL, head2 = NULL;
  Int2          i;
  size_t        len;
  Char          path1 [PATH_MAX];
  Char          path2 [PATH_MAX];
  CharPtr       ptr;
  Char          sect [256];
  ValNodePtr    vnp;

 ProgramPath (path1, sizeof (path1));
  ptr = StringRChr (path1, DIRDELIMCHR);
  if (ptr != NULL) {
    ptr++;
    *ptr = '\0';
  }
  FileBuildPath (path1, "services", NULL);
  head1 = DirCatalog (path1);

  if (GetAppParam ("SEQNCGIS", "SERVICES", "PATH", NULL, path2, sizeof (path2) - 1)) {
    len = StringLen (path2);
    if (path2 [len - 1] != DIRDELIMCHR) {
      StringCat (path2, DIRDELIMSTR);
    }
    if (StringCmp (path1, path2) != 0) {
      head2 = DirCatalog (path2);
    }
  }

  if (head1 == NULL && head2 == NULL) {
    if (! GetAppParam ("SEQNCGIS", "ORDER", NULL, NULL, sect, sizeof (sect) - 1)) {
      return;
    }
  }
  Ing_ReadtRNAConfigFile(m);

}

/***************************************************

  Function : Blast 2 seqs Functions
  
  Purpose : compare two bioseqs

***************************************************/


static FloatHi Ing_get_eval(Int4 exp)
{
  FloatHi eval;
  Int4 i;

  eval = 1;
  for (i=1; i<=exp; i++)
  {
     eval = eval/10;
  }
  return eval;
}

static Boolean Ing_RunBlast2SeqsJob(SeqLocPtr slp1,  Uint2 entityID, BioseqPtr bsp1, BioseqPtr bsp2, CharPtr text, BLAST_OptionsBlkPtr options)
{
  SeqAlignPtr         sap;
  SeqAnnotPtr         sanp;
  SeqLocPtr           slp2 = NULL;

  if (slp1==NULL)
    sap = BlastTwoSequences(bsp1, bsp2, text, options);
  else {
    slp2 = SeqLocIntNew(0, bsp2->length-1, Seq_strand_plus, bsp2->id);
    sap = BlastTwoSequencesByLoc(slp1, slp2, text, options);
  }
   if (sap == NULL)
     {
       ErrPostEx(SEV_WARNING, 0, 0, "No BLAST hits found");
       ErrShow();
       return FALSE;
     }

   sap->saip=NULL;
   AlnMgr2IndexLite(sap);
   sanp = SeqAnnotForSeqAlign (sap);
   sanp->name=StringSave("Blast 2 seqs");
   if (sanp == NULL) return FALSE;
   entityID=Ing_AttachSeqAnnotToSeqEntry(entityID, sanp, bsp1);
   SeqMgrIndexFeatures (entityID, NULL); 
   return TRUE;
}



static void Ing_DoBlast2Seqs (ButtoN b)
{
  IngGenomeViewerPtr  igvp=NULL;
  WindoW              w;
  SeqEntryPtr         sep;
  SeqLocPtr           slp1=NULL;
  BioseqPtr           bsp_2;
  Pointer             dataptr;
  Uint2               datatype=0, entityID=0;
  IngBlast2InfoPtr    bip=NULL;
  FILE       *        fp;
  CharPtr             path, str, ptr;
  CharPtr             text;
  Boolean             is_local, first_bsp=TRUE;
  Char                eval[8];
  BLAST_OptionsBlkPtr options;
  Int4                e;
  Int2                i;
  IngEntityPtr        e_list, e_next, e_head, e_tmp;
  Int4                start, stop, temp;


  WatchCursor();
  w=(WindoW)ParentWindow(b);

  bip = (IngBlast2InfoPtr)GetObjectExtra(b);
  if (bip == NULL || bip->bsp1 == NULL)
    goto end;
  igvp=(IngGenomeViewerPtr)GetObjectExtra(w);
  Ing_ProgressNew(igvp, "Running Blast 2 Sequences...");

  if (igvp==NULL) goto end;

  Hide(w);

  i = GetValue(bip->localorglobal);
  if (i==1)
    is_local=TRUE;
  else
    is_local=FALSE;

  text = StringSave("blastn"); /* can only do blastn for now */

  options = BLASTOptionNew(text, TRUE);
  
  i = GetValue(bip->gapped);
  if (i == 1)
    {
      options->gapped_calculation = TRUE;
    } else if (i == 2){
      options = BLASTOptionNew(text, FALSE);
      options->gapped_calculation = FALSE;
    } else
      goto end;
  
  start = Ing_GetValue (bip->from);
  stop = Ing_GetValue (bip->to);
  if (start > stop){
    temp = stop;
    stop = start;
    start = temp;
  }
  if (start < 0)
    start = 0;
  if (stop > bip->bsp1->length-1)
    stop = bip->bsp1->length-1;
  
  slp1= SeqLocIntNew(start, stop, 1, bip->bsp1->id);

  GetTitle(bip->eval, eval, 14);
  if (eval != NULL)
    {
      e = atoi(eval);
      options->expect_value = Ing_get_eval(e);
    }
  
  GetTitle(bip->wordsize, eval, 5);
  if (eval != NULL)
    options->wordsize = atoi(eval);
  if (GetStatus(bip->maskrep) == TRUE)
    {
      if (GetStatus(bip->masksimple) == TRUE)
        options->filter_string = StringSave("m L;R");
      else
        options->filter_string = StringSave("m R");
    } else if (GetStatus(bip->masksimple) == TRUE)
      options->filter_string = StringSave("m L");
  if (GetStatus(bip->megablast))
    options->is_megablast_search=TRUE;

   e_list=NULL;
   if (bip->path){

    if (!(fp = FileOpen(bip->path, "r"))) goto end;

     while ((dataptr = ReadAsnFastaOrFlatFile (fp, &datatype, &entityID, FALSE, FALSE, TRUE, FALSE)) != NULL) 
    { 
      if (datatype == OBJ_BIOSEQ)
        {
          bsp_2 = (BioseqPtr)dataptr;
          sep= GetTopSeqEntryForEntityID(entityID);
          e_next=(IngEntityPtr)MemNew(sizeof(IngEntity));
          e_next->Sips=SAM_ExtractSips(sep);
          if (first_bsp){
            e_list=e_next;
            e_head=e_next;
            first_bsp=FALSE;
          }
          else{
            e_list->next=e_next;
            e_list=e_list->next;
          }
          if (!bsp_2) goto end;
          if (bsp_2 && ISA_na(bsp_2->mol))
             {
               Ing_RunBlast2SeqsJob(slp1, igvp->entityID, bip->bsp1, bsp_2, text, options);
             } 
        }
    }
     MemFree(bip->path);
   }
   else if (bip->GI_list){
     str=bip->GI_list;

     ptr = strtok(str, " ");
     while(ptr != NULL)
       {
         bsp_2 = Ing_GetBspFromGIOrAcc(ptr);
          if (bsp_2 != NULL)
            {
              e_next=(IngEntityPtr)MemNew(sizeof(IngEntity));
              sep = SeqMgrGetSeqEntryForData (bsp_2);
              e_next->Sips=SAM_ExtractSips(sep);
              if (first_bsp){
                e_list=e_next;
                e_head=e_next;
                first_bsp=FALSE;
              }
              else{
                e_list->next=e_next;
                e_list=e_list->next;
              }
              if (!bsp_2) goto end;
              if (bsp_2 && ISA_na(bsp_2->mol))
                {
                  Ing_RunBlast2SeqsJob(slp1, igvp->entityID, bip->bsp1, bsp_2, text, options);
                } 
            }
          ptr = strtok(NULL, " ");
          
       }
     MemFree(bip->GI_list);
   }
   
   e_tmp=igvp->entity_list;
   if (e_tmp){
     while(e_tmp->next!=NULL)
       e_tmp=e_tmp->next;
     e_tmp->next=e_head;
   }
   else{
     igvp->entity_list=e_list;
   }
   
   BLASTOptionDelete(options);
   
   if (bip->path)
     MemFree(bip->path);
   else if (bip->GI_list)
     MemFree(bip->GI_list);
   SeqLocFree(slp1);
   Remove(w);
   Enable(igvp->d_Win->mrnaorexons);
   igvp->talign.update = TRUE;
   Ing_PopulateOverviewPage((Pointer)igvp, FALSE);
   Ing_PopulateDetailedPage((Pointer)igvp);
 end:
  ArrowCursor();
  Ing_ProgressEnd(igvp);
  return;
  
}



static void Ing_MegaBlast2Defaults(ButtoN b)
{
  IngBlast2InfoPtr bip;

  bip=(IngBlast2InfoPtr)GetObjectExtra(b);

  if (GetStatus(b)){
    SetValue(bip->gapped, 1);
    SetTitle(bip->wordsize, "28");
  }
  else {
    SetTitle(bip->wordsize, "11");
  }
}

extern void Ing_CreateBlast2SeqsForm(IngGenomeViewerPtr igvp, BioseqPtr bsp1, CharPtr path, CharPtr GI_list)
{
   IngBlast2InfoPtr  bip;
   ButtoN            b;
   ButtoN            b1;
   GrouP             maingroup, globalg, localg, gapsg, eANDwg;
   GrouP             submitg, maskg, g1, g2, g3;
   GrouP             leng, g;
   WindoW            w;
   Char              lenbuf[40]={""}, szbuf[40]={""};
   Int4              start, stop;
   
   if (!(bip = (IngBlast2InfoPtr)MemNew(sizeof(IngBlast2Info)))) return;

   bip->bsp1 = bsp1;
   if (!StringHasNoText(path)){
     bip->path=StringSave(path);
     bip->GI_list=NULL;
   }
   else{
     bip->GI_list=StringSave(GI_list);
     bip->path=NULL;
   }
   w = MovableModalWindow(-50, -33 ,-10, -10, "Blast2Seqs", NULL);
   maingroup = HiddenGroup(w, 1, 7, NULL);  
   SetGroupMargins(maingroup, 10, 10);
   SetGroupSpacing(maingroup, 10, 20);

   MemSet((Pointer)lenbuf, '\0', sizeof(lenbuf));
   sprintf(lenbuf, "Query. length=%d", bsp1->length);
   
   MemSet((Pointer)szbuf, '\0', sizeof(szbuf));
   SeqIdWrite(bsp1->id, szbuf, PRINTID_FASTA_SHORT,40);

   leng = NormalGroup(maingroup,1, 2, lenbuf, NULL, NULL);
   SetGroupMargins(leng, 10, 10);
   SetGroupSpacing(leng, 10, 20);
 
   StaticPrompt(leng, szbuf,0,0,programFont,'l');

   g=HiddenGroup(leng, 4, 1, NULL);
   StaticPrompt(g, "from:", 0, 0, programFont, 'l');
   if (igvp->slp){
     start = SeqLocStart(igvp->slp);
     stop = SeqLocStop(igvp->slp);
   }
   else {
     start = 1;
     stop = bsp1->length;
   }

   sprintf(szbuf, "%d", start);
   bip->from = DialogText(g, szbuf, 5, (TxtActnProc) NULL);
   StaticPrompt(g, "  to:", 0, 0, programFont, 'l');
   sprintf(szbuf, "%d", stop);
   bip->to = DialogText(g, szbuf, 5, (TxtActnProc) NULL); 
   
   globalg = NormalGroup(maingroup,1, 1, "Alignment Type",  programFont,NULL);
   bip->localorglobal = HiddenGroup(globalg, 2, 1, NULL);
   RadioButton(bip->localorglobal, "Local");
   RadioButton(bip->localorglobal, "Global");
   SetValue(bip->localorglobal, 1);

   localg = NormalGroup(maingroup, 1, 3, "Local Alignment Options", programFont, NULL);
   g1 = NormalGroup(localg, 1,1, "",  programFont,NULL);
   gapsg = HiddenGroup(g1, 3, 2, NULL);
   bip->gapped = HiddenGroup(gapsg, 0, 2, NULL);
   RadioButton(bip->gapped, "gapped");
   RadioButton(bip->gapped, "ungapped");
   SetValue(bip->gapped, 1);

   g2 = NormalGroup(maingroup, 1,1, "Mask Regions",  programFont,NULL);
   maskg=HiddenGroup(g2, 0, 2, NULL);
   bip->maskrep = CheckBox(maskg, "Mask Repeats", NULL);
   SetStatus(bip->maskrep, FALSE);
   bip->masksimple = CheckBox(maskg, "Mask Simple Sequence", NULL);
   SetStatus(bip->masksimple, TRUE);
   bip->megablast= CheckBox(maingroup, "Mega Blast", Ing_MegaBlast2Defaults);
   SetObjectExtra(bip->megablast, (Pointer)bip, NULL);
   SetStatus(bip->megablast, FALSE);

   g3 = NormalGroup(maingroup, 1,1, "",  programFont,NULL);
   eANDwg = HiddenGroup(g3, 2, 2, NULL);
   StaticPrompt(eANDwg, "E-value:  e-", 0, 0, programFont, 'l');
   bip->eval = DialogText(eANDwg, "1", 5, NULL);
   StaticPrompt(eANDwg, "wordsize:", 0, 0, programFont, 'l');
   bip->wordsize = DialogText(eANDwg, "11", 5, NULL);


   submitg=HiddenGroup(maingroup, 2, 0, NULL);
   b = DefaultButton(submitg, "Accept", Ing_DoBlast2Seqs);
   Enable(b);
   b1 = PushButton(submitg, "Cancel", Ing_StdCancelProc);
   bip->igvp=igvp;
   SetObjectExtra(b1, (Pointer)bip, StdCleanupExtraProc);
   SetObjectExtra(b, (Pointer)bip, StdCleanupExtraProc);
   SetObjectExtra(w, (Pointer)igvp, NULL);
   Select(w);
   Show(w);

   return;

}


extern void Ing_Blast2SeqsProc(IteM i)
{
  IngGenomeViewerPtr   igvp;

  igvp = GetObjectExtra (ParentWindow(i));
  if (igvp == NULL) return;
  igvp->filetype=ALIGN_BLAST2SEQ;
  Ing_OpenFromFileORNetwork(igvp);
}

/***************************************************

  Function : Dot Matrix/ Dot Diag Functions
  
  Purpose : compare two bioseqs

  Return value :

***************************************************/

extern void Ing_DotDiagProc(IteM i)
{
  Int2            ret;
  SeqAlignPtr     sap=NULL;
  SelStructPtr    ssp;
  IngGenomeViewerPtr igvp;
  ObjMgrPtr         omp=NULL;
  OMProcControl     ompc;
  ObjMgrProcPtr     ompp=NULL;
  

  REGISTER_DiagsDisplay;

  igvp=(IngGenomeViewerPtr)GetObjectExtra(ParentWindow(i));
  if (!igvp) {
    ArrowCursor();
    return;
  }

  ssp = ObjMgrGetSelected();
  if (ssp == NULL){
    ArrowCursor();
    return;
  }
  if (ssp->itemtype != OBJ_SEQALIGN)
    {
      ErrPostEx(SEV_ERROR, 0, 0, "MUST select seqalign");
      ErrShow();
      ArrowCursor();
      return;
    }
  if (ssp)
    {
      sap = (SeqAlignPtr)GetPointerForIDs(ssp->entityID, ssp->itemID, OBJ_SEQALIGN);
      if (sap == NULL)
        {
          ErrPostEx(SEV_ERROR, 0, 0, "Error in copying seqalign");
          ErrShow();
          ArrowCursor();
          return;
        }
      
      if (sap){
        omp = ObjMgrReadLock();
        ompp = ObjMgrProcFind(omp, 0, "DOT_DiagsDisplay", 0);
        ompc.input_itemtype = OBJ_SEQALIGN;
        ompc.input_data = (Pointer)sap;
        ompc.input_entityID = ssp->entityID;
        ompc.proc = ompp;
        ObjMgrUnlock();
        if (ompp != NULL)
          {
            ret = (*(ompp->func))((Pointer)&ompc);
            switch(ret)
              {
              case OM_MSG_RET_ERROR:
                ErrShow();
                break;
              default:
                break;
              }
          }
        /*         DOT_AlignPlotGivenSeqAlign(sap); */
      }

      igvp->update=FALSE;
      ObjMgrSetDirtyFlag (igvp->entityID, TRUE);
      ObjMgrSendMsg (OM_MSG_UPDATE,  igvp->entityID, 0, 0);
    }
  
  ArrowCursor();
  return;

}

static void Ing_DotMatrixAccept(ButtoN b)
{
  DotInfo *            dip;
  DOTMainDataPtr       mip;
  Int4                 wordsize=0, numhits=0, f1=0, f2=0, t1=0, t2=0;
  SeqLocPtr            slp1=NULL, slp2=NULL;
  WindoW               w;
  SeqAlignPtr          sap = NULL, sap_tmp = NULL;
  IngGenomeViewerPtr   igvp = NULL;
  DOTAlignInfoPtr      alp;

  w=ParentWindow(b);
  igvp=(IngGenomeViewerPtr)GetObjectExtra(w);
  if (!igvp) return;

  Hide(w);
  dip=(DotInfo *)GetObjectExtra(b);
  wordsize=Ing_GetValue(dip->wordsize);
  numhits=Ing_GetValue(dip->numhits);
  f1=Ing_GetValue(dip->from1);
  f2=Ing_GetValue(dip->from2);
  t1=Ing_GetValue(dip->to1);
  t2=Ing_GetValue(dip->to2);
  if (Ing_GetValue(dip->strand)==Seq_strand_plus)
    slp1= SeqLocIntNew(f1, t1, 1, dip->sip1);
  else
    slp1= SeqLocIntNew(f1, t1, 2, dip->sip1);
  slp2 = SeqLocIntNew(f2, t2,1, dip->sip2);


  if (slp1==NULL || slp2==NULL || wordsize==0 || numhits==0 || 
     (f1==0 && t1==0) || (f2==0 && t2==0)){
    ErrPostEx(SEV_ERROR, 0, 0, "Missing values");
    ErrShow();
    Show(w);
    return;
  }
  sap=dip->sap;
  Remove(w);
  mip = DOT_CreateAndStorebyLoc (slp1, slp2, wordsize, numhits);
  if (mip){
    if (sap){
      sap_tmp=sap->next; 
      sap->next=NULL;
      alp=DOT_AlignInfoNew(); 
      alp->sap=sap; 
      DOT_FillAlignInfoPointer(alp);
      DOT_MakeMainViewer(mip, alp);
      sap->next=sap_tmp;
    }
    else
      DOT_MakeMainViewer(mip, NULL);

  }
}



extern void Ing_CreateDotMatrixForm(IngGenomeViewerPtr igvp, SeqAlignPtr sap, CharPtr path, CharPtr GI_list)
{
  WindoW               DlgW;
  SeqIdPtr             sip1=NULL, sip2=NULL;
  Int4                 start, stop;
  Char                 lenbuf[50]={""}, szbuf[41]={""};
  DotInfo *            dip;
  FILE                *fp = NULL;
  ButtoN               b;
  GrouP                g, d, g_top;
  BioseqPtr            bsp1 = NULL, bsp2 = NULL;
  Pointer              dataptr = NULL; 
  Uint2                datatype = 0;
  CharPtr              ptr = NULL;
  SeqEntryPtr          sep = NULL;
  Uint1                strand;


  DlgW = MovableModalWindow(-50, -33 ,-10, -10, "Dot Matrix", NULL);
  dip=(DotInfo *)MemNew(sizeof(DotInfo));

  if (sap != NULL){
    dip->sap=sap;
    strand = AlnMgr2GetNthStrand(sap, 2);
    sip1=AlnMgr2GetNthSeqIdPtr(sap, 1);
    if (!sip1) goto end;
    dip->sip1=sip1;
    bsp1=BioseqLockById(sip1);
    AlnMgr2GetNthSeqRangeInSA(sap, 1, &start, &stop);
  }
  else {
    dip->sap = NULL;
    bsp1 = igvp->bsp;
    sip1 = bsp1->id;
    dip->sip1 = sip1;
    start = 0;
    stop = bsp1->length - 1;
    strand = Seq_strand_plus;
  }

  if (bsp1){
    MemSet((Pointer)lenbuf, '\0', sizeof(lenbuf));
    sprintf(lenbuf, "Query len = %d", bsp1->length);
  }
  else goto end;

  MemSet((Pointer)szbuf, '\0', sizeof(szbuf));
  SeqIdWrite(sip1, szbuf, PRINTID_FASTA_SHORT,41);
  d=HiddenGroup(DlgW, 1, 4, NULL);
  SetGroupMargins(d, 10, 10);
  SetGroupSpacing(d, 10, 20);

  g_top = NormalGroup(d,1, 2, lenbuf, NULL, NULL);
  SetGroupMargins(g_top, 10, 10);
  SetGroupSpacing(g_top, 10, 20); 

  g=HiddenGroup(g_top, 5, 1, NULL);
  StaticPrompt(g, szbuf,0,0,programFont,'l');
  StaticPrompt(g, "from:", 0, 0, programFont, 'l');
  sprintf(szbuf, "%d", start);
  dip->from1 = DialogText(g, szbuf, 5, (TxtActnProc) NULL);
  StaticPrompt(g, "  to:", 0, 0, programFont, 'l');
  sprintf(szbuf, "%d", stop);
  dip->to1 = DialogText(g, szbuf, 5, (TxtActnProc) NULL); 
  BioseqUnlock(bsp1);

  if (sap != NULL){
    sip2=AlnMgr2GetNthSeqIdPtr(sap, 2);  
    if (!sip2) goto end;
    dip->sip2=sip2;
    bsp2=BioseqLockById(sip2);
    AlnMgr2GetNthSeqRangeInSA(sap, 2, &start, &stop);
  }
  else {
    if (!StringHasNoText(path)){
      fp = FileOpen(path, "r");
      if (!fp) goto end;
      while (dataptr = ReadAsnFastaOrFlatFile(fp, &datatype, NULL, FALSE, FALSE, TRUE, FALSE))
     {
       if (datatype == OBJ_BIOSEQ){
         bsp2 = (BioseqPtr)dataptr;
       }
       else if (datatype == OBJ_SEQENTRY){
         sep=(SeqEntryPtr)dataptr;
         bsp2=(BioseqPtr)sep->data.ptrvalue;
       }
       else {
         Message (MSG_OK, "File is not a bioseq or seqentry");
         return;
       }
     }
   }
   else if (GI_list){
     ptr = strtok(GI_list, " ");
     while(ptr != NULL)
       {
         bsp2 = Ing_GetBspFromGIOrAcc(ptr);
         if (bsp2!=NULL)
         {
           if (!ISA_na(bsp2->mol)){
             Message (MSG_OK, "Sequence is not nucleotide");
             return;
           }
         }
         ptr = strtok(NULL, " ");
       }
   }
    sip2 = bsp2->id;
    dip->sip2 = sip2;
    start = 0;
    stop = bsp2->length - 1;
  } 
  
  if (bsp2){
    MemSet((Pointer)lenbuf, '\0', sizeof(lenbuf));
    sprintf(lenbuf, "Subject len = %d", bsp2->length);
  }
  else goto end;

  MemSet((Pointer)szbuf, '\0', sizeof(szbuf));
  SeqIdWrite(sip2, szbuf, PRINTID_FASTA_SHORT,41);

  if (start>bsp2->length)
    start=0;
  if (stop>bsp2->length)
    stop=bsp2->length-1;
  BioseqUnlock(bsp2);

  g_top = NormalGroup(d,1, 1, lenbuf, NULL, NULL);
  SetGroupMargins(g_top, 10, 10);
  SetGroupSpacing(g_top, 10, 20); 

  g=HiddenGroup(g_top, 5, 1, NULL);
  StaticPrompt(g, szbuf,0,0,programFont,'l');
  StaticPrompt(g, "  from:", 0, 0, programFont, 'l');
  sprintf(szbuf, "%d", start);
  dip->from2 = DialogText(g, szbuf, 5, (TxtActnProc) NULL);
  StaticPrompt(g, "  to:", 0, 0, programFont, 'l');
  sprintf(szbuf, "%d", stop);
  dip->to2 = DialogText(g, szbuf, 5, (TxtActnProc) NULL); 

  g_top = NormalGroup(d,1, 1, "Dot Plot parameters", NULL, NULL);
  SetGroupMargins(g_top, 10, 10);
  SetGroupSpacing(g_top, 10, 20); 

  g=HiddenGroup(g_top, 2, 3, NULL);
  StaticPrompt(g, "Word size:", 0, 0, programFont, 'l');
  dip->wordsize = DialogText(g, "11", 5, (TxtActnProc) NULL);
  StaticPrompt(g, "Max # of hits", 0, 0, programFont, 'l');
  dip->numhits = DialogText(g, "1000000", 5, (TxtActnProc) NULL); 
  StaticPrompt(g, "plus-plus=1/plus-minus=2", 0, 0, programFont, 'l');
  if (strand == Seq_strand_minus)
    dip->strand = DialogText(g, "2", 5, (TxtActnProc) NULL);
  else 
    dip->strand = DialogText(g, "1", 5, (TxtActnProc) NULL);

  g = HiddenGroup(d, 2, 1, NULL);
  b=DefaultButton(g, "Accept", Ing_DotMatrixAccept);
  Enable(b);
  PushButton(g, "Cancel", Ing_StdCancelProc);

  SetObjectExtra(b, (Pointer)dip, StdCleanupExtraProc);
  SetObjectExtra(DlgW, (Pointer)igvp, NULL);


  RealizeWindow(DlgW);
  Select(DlgW);
  Show(DlgW);
  return;
 end:
  Message (MSG_OK, "Invalid alignment");

}


extern void Ing_GetFileForDotMatrix(IteM i)
{
  IngGenomeViewerPtr igvp;

  igvp=(IngGenomeViewerPtr)GetObjectExtra(ParentWindow(i));
  igvp->filetype=Ing_DOTMATRIX;
  Ing_OpenFromFileORNetwork(igvp);
  
}

/***************************************************

  Function : Spidey Related Functions
  
  Purpose : read mRNA sequences, run spidey,  display 
results and delete structure

***************************************************/

static void Ing_AddSpideyAlignsToBioseq(SPI_mRNAPtr spidp, Uint2 entityID, IngGenomeViewerPtr igvp, BioseqPtr bsp_genomic, Boolean isExons)
{
  SPI_mRNAPtr spidp_head = NULL;
  SPI_mRNAPtr spidp_next = NULL;
  SeqAnnotPtr sanp = NULL;


  spidp_head=spidp;
  while(spidp){
    if (!isExons)
      sanp = SeqAnnotForSeqAlign(spidp->continuous);
    else
      sanp = SeqAnnotForSeqAlign(spidp->parent);
    sanp->name=StringSave("Spidey");
    Ing_AttachSeqAnnotToSeqEntry(entityID, (SeqAnnotPtr)sanp, bsp_genomic);
    SeqMgrIndexFeatures (entityID, NULL);
    spidp=spidp->next;
  }
  spidp=spidp_head;

  /* store new spidp to end of linked list */

  if (igvp->data){
    spidp_head=(SPI_mRNAPtr)igvp->data;
    spidp_next=spidp_head;
    while(spidp_next->next!=NULL){
      spidp_next=spidp_next->next;
    }
    spidp_next->next = spidp;
  }
  else {
    igvp->data=(Pointer)spidp;
    Enable(igvp->d_Win->mrnaorexons);
  }
  
}


static void Ing_RunSpidey(BioseqPtr bsp_genomic, BioseqPtr bsp_mRNA, IngGenomeViewerPtr igvp, Uint2 entityID, SPI_OptionsPtr spot)
{
  SPI_mRNAPtr      spidp = NULL;
  SPI_mRNAPtr      spidp_head = NULL;
  SPI_mRNAPtr      spidp_next = NULL;
  SeqAnnotPtr      sanp = NULL;
  Char             str[31]={""}, buf[40]={""};

  Ing_WinTimerProc(igvp->hMain);
  spidp=SPI_AlignmRNAToGenomic(bsp_genomic, bsp_mRNA, spot);

  SeqIdWrite(bsp_mRNA->id, str, PRINTID_FASTA_LONG,30);

  if (!spidp) {
    sprintf(buf, "Spidey returned NULL for  %s", str);
    Message (MSG_OK, buf);
    return;
  }
  Ing_AddSpideyAlignsToBioseq(spidp, entityID, igvp, bsp_genomic, igvp->isExons);
  Enable(igvp->d_Win->mrnaorexons);
  Ing_WinTimerProc(igvp->hMain);

  return;
}


static void Ing_ReadmRNAs_ext(FILE * fp, IngGenomeViewerPtr igvp, CharPtr GI_list, SPI_OptionsPtr spot)
{
  BioseqPtr    bsp_mRNA = NULL;
  Pointer      dataptr = NULL;
  Uint2        datatype = 0;
  CharPtr      ptr=NULL;
  SeqEntryPtr  sep;


   if (fp != NULL){
   while (dataptr = ReadAsnFastaOrFlatFile(fp, &datatype, NULL, FALSE, FALSE, TRUE, FALSE))
     {
       if (datatype == OBJ_BIOSEQ)
         {
           bsp_mRNA = (BioseqPtr)dataptr;
         }
       else if (datatype == OBJ_SEQENTRY){
         sep=(SeqEntryPtr)dataptr;
         if (sep->choice == 1) /* bioseq */
           bsp_mRNA=(BioseqPtr)sep->data.ptrvalue;
         else if (sep->choice == 2)
         {
           Message (MSG_OK, "File is a bioseq-set");
           return;
         }
       }
       else {
         Message (MSG_OK, "File is not a bioseq or seqentry");
         return;
       }
       if (bsp_mRNA->mol == 0)
         bsp_mRNA->mol = Seq_mol_rna;
       if (bsp_mRNA && (ISA_na(bsp_mRNA->mol)))
         {
           Ing_RunSpidey(igvp->bsp, bsp_mRNA, igvp, igvp->entityID, spot);
         }
     }
   }
   else if (GI_list){

     ptr = strtok(GI_list, " ");
     while(ptr != NULL)
       {
         bsp_mRNA = Ing_GetBspFromGIOrAcc(ptr);
         if (bsp_mRNA!=NULL)
         {
           if (bsp_mRNA)
             {
               if (!ISA_na(bsp_mRNA->mol)){
                 Message (MSG_OK, "Sequence is not nucleotide");
                 return;
               }
               Ing_RunSpidey(igvp->bsp, bsp_mRNA, igvp, igvp->entityID, spot);
             }
           BioseqUnlock(bsp_mRNA);
          }
         ptr = strtok(NULL, " ");
       }
   }
}


extern void Ing_ReadmRNAs (FILE * fp, IngGenomeViewerPtr igvp, CharPtr GI_list)
{
  Ing_ReadmRNAs_ext(fp, igvp,  GI_list, NULL);
}


static Boolean LIBCALLBACK Ing_SpideyCallback (SPI_ProgressPtr progress)
{
  IngGenomeViewerPtr igvp=NULL;
  
  
  switch (progress->returncode) {
  case SPI_START:
    return TRUE;
  case SPI_PROGRESS:
    if (progressW != NULL) {
      igvp=(IngGenomeViewerPtr)GetObjectExtra(progressW);
      if (!igvp)
        return TRUE;
      if (progress->percentdone>0){
        sprintf(igvp->progress_counter, "Completed %d%%", progress->percentdone);
      }
      Ing_WinTimerProc(progressW);
    }
    return TRUE;
  case SPI_FINISHED:
    return TRUE;
  default:
    break;
  }
  return FALSE;
}

static FloatHi Ing_GetFloatValue (TexT t)
{
  Char str[20];
  FloatHi val;

  GetTitle (t,  str,  sizeof(str));
  if (StringHasNoText(str))
    {
      ErrPostEx (SEV_WARNING, 0, 0, "%s", "missing parameter(s)");
      ErrShow();
      return 0.0;
    }

  val=atof(str);

  return val;
}

static void Ing_DoSpidey(ButtoN b)
{
  WindoW w;
  IngSpideyFormPtr sfp=NULL;
  SPI_OptionsPtr spot=NULL;
  IngGenomeViewerPtr igvp=NULL;
  FILE *         fp=NULL;
  Int4           start, stop, temp; 

  w=(WindoW)ParentWindow(b);
  WatchCursor();
  sfp = (IngSpideyFormPtr)GetObjectExtra(b);
  if (sfp == NULL || sfp->bsp1 == NULL)
    goto end;
  igvp=(IngGenomeViewerPtr)GetObjectExtra(w);
  if (igvp==NULL) goto end;
  Ing_ProgressNew(igvp, "Running Spidey...");

  Hide(w);
   
  spot = (SPI_OptionsPtr)MemNew(sizeof(SPI_Options));
  spot->firstpasseval = Ing_GetFloatValue(sfp->firstpasseval);
  spot->secpasseval = Ing_GetFloatValue(sfp->secondpasseval);
  spot->thirdpasseval = Ing_GetFloatValue(sfp->thirdpasseval);
  spot->numreturns = Ing_GetValue(sfp->numreturns);
  spot->printaln = TRUE;
  spot->printasn = FALSE;
  spot->idcutoff = Ing_GetValue(sfp->idcutoff);
  spot->lencutoff = Ing_GetValue(sfp->lencutoff);  
  spot->interspecies = GetValue(sfp->inters);
  spot->organism=GetValue(sfp->org);
  spot->callback= Ing_SpideyCallback;

  start = Ing_GetValue(sfp->from); 
  stop = Ing_GetValue(sfp->to); 
  if (start>stop){ 
    temp = start; 
    start = stop; 
    stop = temp; 
  } 
  if (start<0)
    start = 0; 
  if (stop > igvp->bsp->length-1) 
    stop = igvp->bsp->length-1;
  spot->from = start;
  spot->to = stop;

  if (sfp->path){
    fp=FileOpen(sfp->path, "r");
    Ing_ReadmRNAs_ext(fp, igvp, NULL, spot);
    MemFree(sfp->path);
  }
  else if (sfp->GI_list){
    Ing_ReadmRNAs_ext(NULL, igvp, sfp->GI_list, spot);
    MemFree(sfp->GI_list);
  }
  MemFree(spot); 
  Remove(w);
  igvp->talign.update = TRUE;
  Ing_PopulateOverviewPage((Pointer)igvp, FALSE);
  Ing_PopulateDetailedPage((Pointer)igvp);
  
 end:
  ArrowCursor();
  Ing_ProgressEnd(igvp);
  return;
  
}

static void Ing_MarkSeqAnnotbyName(SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)
{
BioseqPtr     bsp=NULL;
BioseqSetPtr  bssp=NULL;
SeqAnnotPtr   sanp=NULL;
CharPtr       name=NULL; 

 name=(CharPtr)mydata;
 if (IS_Bioseq(sep)) {
   bsp = (BioseqPtr)sep->data.ptrvalue;
   sanp = bsp->annot;
 } else if (IS_Bioseq_set(sep)) {
   bssp = (BioseqSetPtr)sep->data.ptrvalue;
   sanp = bssp->annot;
 } else return;

 while (sanp != NULL) {
   if (sanp->type == 2 && (StringCmp(sanp->name, name)==0)) {
     sanp->data=NULL;
     sanp->idx.deleteme=1;
   }
   sanp=sanp->next;
 }
}



extern void Ing_DeleteSpidey(IngGenomeViewerPtr igvp, Int4 index)
{

  SPI_mRNAPtr spidp = NULL;
  SPI_mRNAPtr spidp_head = NULL;
  SPI_mRNAPtr prev_sp, cur_sp, next_sp;
  Int4        i;

  spidp = (SPI_mRNAPtr) igvp->data;
  if (! spidp) return;
  
  spidp_head = spidp;
  
  if (!spidp->next){
    if (igvp->isExons){
      spidp->numexons = 0;
      spidp->parent = NULL;
    }
    else {
      spidp->continuous = NULL;
    }
    SPI_mRNAFree(spidp);
    igvp->data = NULL;
    return;
  }

  for (i=2; i<index; i++)
    spidp = spidp->next;
  prev_sp = spidp;
  cur_sp = spidp->next;
  if (!cur_sp->next){
    if (igvp->isExons){
      cur_sp->numexons = 0;
      cur_sp->parent = NULL;
    }
    else {
      cur_sp->continuous = NULL;
    }
    SPI_mRNAFree(cur_sp);
    prev_sp->next = NULL;
    igvp->data = (Pointer) spidp_head;
    return;
  }

  next_sp = cur_sp->next;
  prev_sp->next = next_sp;
  if (igvp->isExons){
    cur_sp->numexons = 0;
    cur_sp->parent = NULL;
  }
  else {
    cur_sp->continuous = NULL;
  }
  SPI_mRNAFree(cur_sp);

  igvp->data = (Pointer) spidp_head;
  return;

}


static void Ing_ClearSpideyAligns(IngGenomeViewerPtr igvp)
{
  SeqEntryPtr sep=NULL;
  CharPtr     name=NULL;

  /* re-init arrays */
  Ing_InitAlignArrays(&igvp->talign, FALSE);

  /* delete all spidey alignments */
  name = StringSave("Spidey");
  sep = GetTopSeqEntryForEntityID(igvp->entityID); 
  SeqEntryExplore(sep,(Pointer)name, Ing_MarkSeqAnnotbyName);
  DeleteMarkedObjects(igvp->entityID, OBJ_SEQENTRY, (Pointer)sep);

}


extern void Ing_mRNAOrExons(IteM item)
{
  IngGenomeViewerPtr igvp = NULL;
  SPI_mRNAPtr        spidp = NULL;
  IngReportPtr       Report = NULL;
  SelStructPtr       ssp=NULL;

  igvp=(IngGenomeViewerPtr)GetObjectExtra(ParentWindow(item));
  if (!igvp) return;

  ssp = ObjMgrGetSelected();
  if (ssp)
    ObjMgrDeSelectAll();
  spidp = (SPI_mRNAPtr)igvp->data;
  if (spidp == NULL) return;
  igvp->data = NULL;
  Ing_ClearSpideyAligns(igvp);
  if (igvp->isExons){
    igvp->isExons=FALSE;
    SetTitle(igvp->d_Win->mrnaorexons, "Spidey - as Exons");
  }
  else {
    igvp->isExons=TRUE;
    SetTitle(igvp->d_Win->mrnaorexons, "Spidey - as mRNA");
  }

  Ing_AddSpideyAlignsToBioseq(spidp, igvp->entityID, igvp, igvp->bsp, igvp->isExons);

  ObjMgrSetDirtyFlag (igvp->entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, igvp->entityID, 0, 0);
  if (igvp->hReport){
    Report = (IngReportPtr)GetObjectExtra(igvp->hReport);
    if (Report){
      Ing_PopulateReport(Report, igvp);
    }
  }
}


SPI_mRNAPtr Ing_FindSpidpGivenParent(SPI_mRNAPtr spidp_head, SeqAlignPtr sap, Boolean bExons)
{
  SPI_mRNAPtr spidp_temp = NULL;

  if (!spidp_head) return NULL;
  
  spidp_temp = spidp_head;

  if (bExons){
    while (spidp_temp){
      if (spidp_temp->parent == sap){
        return spidp_temp;
      }
      spidp_temp = spidp_temp->next;
    }
  }
  else {
    while (spidp_temp){
      if (spidp_temp->continuous == sap){
        return spidp_temp;
      }
      spidp_temp = spidp_temp->next;
    }
  }
  return NULL;
}


extern SeqLocPtr Ing_MakeLocListFromSpidp(Pointer userdata, SeqAlignPtr parent, Boolean gap_split)
{
  SeqIdPtr sip;
  SeqLocPtr slp = NULL, slp_head = NULL;
  SeqAlignPtr child = NULL;
  SPI_mRNAPtr spidp = NULL, spidp_head = NULL;
  Int4     i, c;
 
  spidp_head = (SPI_mRNAPtr) userdata;
  spidp = Ing_FindSpidpGivenParent(spidp_head, parent, FALSE);
  if (!spidp) return NULL;

  child = (SeqAlignPtr) parent->segs;
  sip = AlnMgr2GetNthSeqIdPtr(child, 1);
  if (gap_split){
    for (i = 0; i < spidp->numexons; i++){
      if (spidp->strand == Seq_strand_minus)
        c = spidp->numexons - i - 1;
      else
        c = i;
      
      slp=SeqLocIntNew(spidp->gstarts[c]-1, spidp->gstops[c]-1, spidp->strand, sip);
      SeqLocAdd(&slp_head, slp, TRUE, FALSE);
    }
  }
  else {
    slp = SeqLocIntNew(spidp->gstarts[0]-1, spidp->gstops[spidp->numexons-1]-1, spidp->strand, sip);
    SeqLocAdd(&slp_head, slp, TRUE, FALSE);
  }

  return slp_head;
}

static SPI_ExonProfPtr Ing_NumMismatches(SPI_ExonProfPtr epp_head, Int4 exon, Int4Ptr nmis)
{
  SPI_ExonProfPtr epp;

  epp=epp_head;
  while(epp){
    if (epp->exonnum==exon){
      *nmis=epp->nummismatches;
      return (epp);
    }
    epp=epp->next;
  }
  return NULL;
}


static Int4 Ing_get_rightend(SeqAlignPtr sap, Int4 mismatch, Int4 row, Int4Ptr nleftp, Int4Ptr nrightp)
{
  Int4 insert_pos = -1, i = 0, j = -1, k = 0;

  while(insert_pos == -1){
    i++;
    insert_pos = AlnMgr2MapBioseqToSeqAlign(sap, mismatch + i, row);
  }
  while (j == -1 && (mismatch - k) > 0){
    k++;
    j = AlnMgr2MapBioseqToSeqAlign(sap, mismatch - k, row);
  }
  if (nleftp != NULL)
    *nleftp = k - 1;
  if (nrightp != NULL)
    *nrightp = i -1;

  return  (insert_pos);
}

static Boolean Ing_IsinRange(Int4 gleft, Int4 gright, Int4 start, Int4 stop)
{
  if (start >  gleft || stop < gright)
    return FALSE;
  else
    return TRUE;
}


static void Ing_ReplaceGapInGenomic(SeqAlignPtr sap, Int4 mmis)
{
  DenseSegPtr  dsp_new = NULL, dsp = NULL;
  Int4         i, j;
  Int4         seg_start, seg_stop;
  Int2         newsegs, offset = 0;
  Boolean      found = FALSE;
  Boolean      gap_filled = FALSE;


  dsp = (DenseSegPtr)(sap->segs);

  dsp_new = DenseSegNew();
  dsp_new->dim = 2;
  dsp_new->numseg = dsp->numseg - 2;
  dsp_new->ids = dsp->ids;
  dsp->ids = NULL;
  dsp_new->starts = (Int4Ptr)MemNew((dsp_new->numseg)*2*sizeof(Int4));
  dsp_new->lens = (Int4Ptr)MemNew((dsp_new->numseg)*sizeof(Int4));
  dsp_new->strands = (Uint1Ptr)MemNew((dsp_new->numseg)*2*sizeof(Uint1));

  for (i = 0, j = 0; i < dsp->numseg; i++){

    if (dsp->starts[i*2] == -1){
      seg_start = dsp->starts[i*2+1];
      seg_stop = dsp->starts[i*2+1] + dsp->lens[i] - 1;
      if (seg_start <= mmis && seg_stop >= mmis){
        found = TRUE;
      }
      else 
        found = FALSE;
    }

    if (!found || gap_filled){
      if (dsp->starts[i*2] != -1)
        dsp_new->starts[j*2] = dsp->starts[i*2] + offset;
      else
        dsp_new->starts[j*2] = dsp->starts[i*2];
      dsp_new->starts[j*2+1] = dsp->starts[i*2+1];
      dsp_new->strands[j*2] = dsp->strands[i*2];
      dsp_new->strands[j*2+1] = dsp->strands[i*2+1];
      dsp_new->lens[j] = dsp->lens[i];
      j++;
    }
    else if (!gap_filled){
      /* extend previous segment over the gap */
      offset += dsp->lens[i];
      if (dsp->strands[i*2+1]  == Seq_strand_minus){
/*         if (dsp->starts[(i+1)*2] == -1) */
/*           dsp_new->starts[(j-1)*2] = dsp->starts[(i+1)*2] + offset; */
/*         else */
/*           dsp_new->starts[(j-1)*2] = dsp->starts[(i+1)*2]; */
        dsp_new->starts[(j-1)*2+1] = dsp->starts[(i+1)*2+1];
        dsp_new->strands[(j-1)*2+1] = dsp->strands[(i+1)*2+1];
      }
      dsp_new->lens[j-1] = dsp->lens[i+1] + dsp->lens[i] + dsp->lens[i-1];

      i++;
      gap_filled = TRUE;
    }

  }
  sap->segs = (Pointer)dsp_new;
  DenseSegFree(dsp);
  SAIndexFree(sap->saip);
  sap->saip = NULL;
  AlnMgr2IndexSingleChildSeqAlign(sap);

}

static void Ing_DeleteInsertInGenomic(SeqAlignPtr sap, Int4 gmis)
{
  DenseSegPtr  dsp_new = NULL, dsp = NULL;
  Int4         i, j;
  Int4         seg_start, seg_stop;
  Int2         newsegs = 0, offset = 0;
  Boolean      found = FALSE;
  Boolean      gap_filled = FALSE;


  dsp = (DenseSegPtr)(sap->segs);

  dsp_new = DenseSegNew();
  dsp_new->dim = 2;
  dsp_new->numseg = dsp->numseg - 2;
  dsp_new->ids = dsp->ids;
  dsp->ids = NULL;
  dsp_new->starts = (Int4Ptr)MemNew((dsp_new->numseg)*2*sizeof(Int4));
  dsp_new->lens = (Int4Ptr)MemNew((dsp_new->numseg)*sizeof(Int4));
  dsp_new->strands = (Uint1Ptr)MemNew((dsp_new->numseg)*2*sizeof(Uint1));

  for (i = 0, j = 0; i < dsp->numseg; i++){

    if (dsp->starts[i*2+1] == -1){
      seg_start = dsp->starts[i*2];
      seg_stop = dsp->starts[i*2] + dsp->lens[i] - 1;
      if (seg_start <= gmis && seg_stop >= gmis)
        found = TRUE;
      else
        found = FALSE;
    }

    if (!found || gap_filled){
      if (dsp->starts[i*2] > 0)
        dsp_new->starts[j*2] = dsp->starts[i*2] - offset;
      else
        dsp_new->starts[j*2] = dsp->starts[i*2];
      dsp_new->starts[j*2+1] = dsp->starts[i*2+1];
      dsp_new->strands[j*2] = dsp->strands[i*2];
      dsp_new->strands[j*2+1] = dsp->strands[i*2+1];
      dsp_new->lens[j] = dsp->lens[i];
      j++;
    }
    else if (!gap_filled){
        /* extend previous segment over the insert */
      offset += dsp->lens[i];
      if (dsp->strands[i*2+1]  == Seq_strand_minus){
        dsp_new->starts[(j-1)*2+1] = dsp->starts[(i+1)*2+1];
        dsp_new->strands[(j-1)*2+1] = dsp->strands[(i+1)*2+1];
      }
      dsp_new->lens[j-1] = dsp->lens[i+1] + dsp->lens[i-1];
      i++;
      gap_filled = TRUE;
    }
  }
  sap->segs = (Pointer)dsp_new;
  DenseSegFree(dsp);
  SAIndexFree(dsp);
  sap->saip = NULL;
  AlnMgr2IndexSingleChildSeqAlign(sap);

}

static void Ing_FillGap(SPI_mRNAPtr spidp, SeqIdPtr gsip, SeqIdPtr msip, Int4 gmis, Int4 mmis, Uint1 mstrand, Int4 nleft, Int4 nright, Int4 i, SeqAlignPtr sap_exon, SeqAlignPtr sap_continuous){

  Int4 ntotal;

  ntotal = nleft + nright + 1;
  BioseqInsert(msip, mmis - nleft, mmis + nright, mstrand, gsip, gmis, FALSE, FALSE, FALSE);
  spidp->exongaps[i] -= ntotal;
  if (spidp->exongaps[i] < 0)
    spidp->exongaps[i] = 0;
  Ing_ReplaceGapInGenomic(sap_exon, mmis);
  Ing_ReplaceGapInGenomic(sap_continuous, mmis);

}

static void Ing_DelInsert(SPI_mRNAPtr spidp, SeqIdPtr gsip, Int4 gmis, Int4 nleft, Int4 nright, Int4 i, SeqAlignPtr sap_exon, SeqAlignPtr sap_continuous)
{
  Int4 ntotal;
  
  ntotal = nleft + nright + 1;
  BioseqDelete(gsip, gmis - nleft, gmis + nright, TRUE, FALSE);
  spidp->exongaps[i] -= ntotal;
  if (spidp->exongaps[i] < 0)
    spidp->exongaps[i] = 0;
  Ing_DeleteInsertInGenomic(sap_exon, gmis);
  Ing_DeleteInsertInGenomic(sap_continuous, gmis);
  
}

extern void Ing_ReplaceAllMismatchedInLocation(IngGenomeViewerPtr igvp, SeqAlignPtr sap, Int4 start, Int4 stop, Boolean All, Boolean fillGaps, Boolean delInserts)
{
  SPI_mRNAPtr spidp = NULL;
  SPI_ExonProfPtr   epp = NULL, epp_cur = NULL;
  AMAlignIndex2Ptr   amaip = NULL;
  SeqIdPtr          gsip = NULL, msip = NULL;
  Uint1             gstrand, mstrand;
  Char              str[200] = {""};
  Int4              exon_count = 0, j, i, k, m;
  Int4              gmis, mmis, nmismatches, nkeep = 0;
  Int4              nleft = 0, nright = 0, ntotal = 0; 
  Int4Ptr           mismatches, keep = NULL, tmp;
  MsgAnswer         ans;
  Boolean           always_false = FALSE; 
  SeqAlignPtr       parent = NULL;
  SeqAlignPtr       sap_exon = NULL, sap_continuous = NULL;

  parent = AlnMgr2GetParent(sap);
  if (parent == NULL) return;

  spidp = Ing_FindSpidpGivenParent((SPI_mRNAPtr)igvp->data, parent, igvp->isExons);
  if (!spidp) return;

  epp = spidp->epp;
  
  gsip = AlnMgr2GetNthSeqIdPtr(spidp->saps[0], 1);
  msip = AlnMgr2GetNthSeqIdPtr(spidp->saps[0], 2);
  gstrand = AlnMgr2GetNthStrand(spidp->saps[0], 1);
  mstrand = AlnMgr2GetNthStrand(spidp->saps[0], 2);
  sap_continuous = (SeqAlignPtr) spidp->continuous->segs;

  /* replace only single exon */
  if (igvp->isExons == TRUE){ 
    exon_count = 1;
    for (j = 0; j< spidp->numexons; j++){
      if (spidp->saps[j] == sap)
        break;
      exon_count++;
    }
    /* replace mismatch */
    epp_cur = Ing_NumMismatches(epp, exon_count, &nmismatches);
    if (nmismatches > 0){
      sap_exon = spidp->saps[exon_count - 1];
      mismatches = epp_cur->mismatches;
      for (j = 0; j < nmismatches; j++){
        gmis = AlnMgr2MapBioseqToSeqAlign(sap_exon, mismatches[j], 1);
        mmis = AlnMgr2MapBioseqToSeqAlign(sap_exon, mismatches[j], 2); 

        if (gmis == -1) /* gap on genomic */ {
          gmis = Ing_get_rightend(sap_exon, mismatches[j], 1, &nleft, &nright);
           ntotal = nleft + nright + 1;

          if (!All)
            if (!Ing_IsinRange(gmis - nleft, gmis + nright, start, stop)) 
              goto keep_exon;
          
          if (fillGaps){
            Ing_FillGap(spidp, gsip, msip, gmis, mmis, mstrand,  nleft, nright, (exon_count - 1), sap_exon, sap_continuous);
          }
          else {
            goto keep_exon;
          }
        }
        else if (mmis == -1) /* gap on mRNA */ {
          Ing_get_rightend(sap_exon, mismatches[j], 2, &nleft, &nright); 
          ntotal = nleft + nright + 1;
          if (!All)
            if (!Ing_IsinRange(gmis - nleft, gmis + nright, start, stop)) 
              goto keep_exon;
          
          if (delInserts){
            Ing_DelInsert(spidp, gsip, gmis, nleft, nright, (exon_count - 1), sap_exon, sap_continuous);

            /* update mismatch coordinates */
            for(k = j+1; k < nmismatches; k++)
              mismatches[k] -= ntotal;
          }
          else {
            goto keep_exon;
          }
        }
        else {
          ntotal = 1;
          if (!All) 
            if (!Ing_IsinRange(gmis, gmis, start, stop)){
              ntotal = 1;
              goto keep_exon;
            }
          SeqLocReplace(msip, mmis, mmis, mstrand, gsip, gmis, gmis, gstrand);
        }
        
        if (always_false){
        keep_exon:

          if (keep != NULL){
            tmp = (Int4Ptr)MemNew(sizeof(Int4)*(nkeep + ntotal));
            for (k = 0; k < nkeep; k++)
              tmp[k] = keep[k];
            MemFree(keep);
            keep = tmp;
          }
          else {
            keep = (Int4Ptr)MemNew(sizeof(Int4)*(nkeep + ntotal));
            k = 0;
          }
          /* add the latest kept mismatches */
          for (m = j; k < nkeep + ntotal; k++, m++)
            keep[k] = mismatches[m];
          
          nkeep += ntotal;
        }

        j += (ntotal - 1);
      }
      MemFree(epp_cur->mismatches);
      epp_cur->mismatches = keep;
      epp_cur->nummismatches = nkeep;
      nkeep = 0;
      keep = NULL;
    }
  }
  else {
    epp_cur = spidp->epp;
    sap_continuous = (SeqAlignPtr)spidp->continuous->segs;
    while (epp_cur){
      nmismatches = epp_cur->nummismatches;

      if (nmismatches > 0){
        mismatches = epp_cur->mismatches;
        i = epp_cur->exonnum - 1;
        sap_exon = spidp->saps[i];
        for (j = 0; j < nmismatches; j++){
          gmis = AlnMgr2MapBioseqToSeqAlign(sap_exon, mismatches[j], 1);
          mmis = AlnMgr2MapBioseqToSeqAlign(sap_exon, mismatches[j], 2); 

          if (gmis == -1) {
            gmis = Ing_get_rightend(sap_exon, mismatches[j], 1, &nleft, &nright);
            ntotal = nleft + nright + 1;

            if (!All)
              if (!Ing_IsinRange(gmis-nleft, gmis+nright, start, stop)) 
                goto keep_continuous;
            
            if (fillGaps){
              Ing_FillGap(spidp, gsip, msip, gmis, mmis, mstrand,  nleft, nright, i, sap_exon, sap_continuous);
            }
            else {
              goto keep_continuous;
            }
          }
          else if (mmis == -1) {
            Ing_get_rightend(sap_exon, mismatches[j], 2, &nleft, &nright);
            ntotal = nleft + nright + 1;
            if (!All)
              if (!Ing_IsinRange(gmis-nleft, gmis+nright, start, stop)) 
                goto keep_continuous;
            
            if (delInserts){
              Ing_DelInsert(spidp, gsip, gmis, nleft, nright, i, sap_exon, sap_continuous);
              
              /* update mismatch coordinates */
              for (k = j+1; k < nmismatches; k++)
                mismatches[k] -= ntotal;
            }
            else {
              goto keep_continuous;
            }
          }
          else {
            ntotal = 1;
          if (!All) 
            if (!Ing_IsinRange(gmis, gmis, start, stop)){
              goto keep_continuous;
            }
            SeqLocReplace(msip, mmis, mmis, mstrand, gsip, gmis, gmis, gstrand);
          }

          if (always_false){
          keep_continuous:

            if (keep != NULL){
              tmp = (Int4Ptr)MemNew(sizeof(Int4)*nkeep + ntotal);
              for (k = 0; k < nkeep; k++)
                tmp[k] = keep[k];
              MemFree(keep);
              keep = tmp;
            }
            else {
              keep = (Int4Ptr)MemNew(sizeof(Int4)*(nkeep + ntotal));
              k = 0;
            }
            /* add the latest kept mismatches */
            for (m = j; k < nkeep + ntotal; k++, m++)
              keep[k] = mismatches[m];
            
            nkeep += ntotal;
          }

          j += (ntotal-1);
        }
        MemFree(epp_cur->mismatches);
        epp_cur->mismatches = keep;
        epp_cur->nummismatches = nkeep;
        nkeep = 0;
        keep = NULL;
      }
      epp_cur = epp_cur->next;
    }
  }

  ObjMgrSetDirtyFlag(igvp->entityID, TRUE);
  ObjMgrSendMsg(OM_MSG_UPDATE, igvp->entityID, 0, 0);
}

/***************************************************

  Function : Report Drawing Functions
  
  Purpose : populate spidey report window

***************************************************/

static IngReportPtr Ing_ReportNew(void)
{
  IngReportPtr Report=NULL;
  
  Report=(IngReportPtr)MemNew(sizeof(IngReport));
  /* initialize */
  Report->hMain=NULL;
  Report->Viewer=NULL;
  Report->Picture=NULL;
  Report->igvp=NULL;
  Report->isPrevSelected=FALSE;
  Report->last_segID=0;
  Report->vert_positions=NULL;
  
  return Report;
}


#define  MIS_NONSENSE 1
#define  MIS_SILENT 2
#define  MIS_FRAMESHIFT_by_insert 3 /* gap on mRNA sequence */
#define  MIS_FRAMESHIFT_by_delete 4 /* gap on genomic sequence */
#define  MIS_MISSENSE 5
#define  MIS_START 6 /* mismatch at start */
#define  MIS_STOP 7 /* mismatch at start */
#define  MIS_GREYZONE 8 /* downstream of frameshift mut */
#define  MIS_BLUEZONE 9 /* downstream of extended cds */
#define  MIS_BROWNZONE 10  /* downstream of nonsense mut */
#define  MIS_NOTINCDS 11
#define  MIS_NOTINCDS_genomic 12 /* gap in mRNA sequence in non-coding region */
#define  MIS_DELETE  13 /* gap on mrna sequence */
#define  MIS_INSERT 14 /* insert on mrna sequence */
#define  MIS_ISNOTAMIS 15

typedef struct ing_mislist {
  Int4  mism_pos;
  Uint1 mismtype;
  Char  from_c;
  Char  to_c;
} IngMisList, PNTR IngMisListPtr;


typedef struct ing_explore{
  Int4        cds_count;
  SeqAlignPtr sap;
  BioseqPtr   bsp_gen, bsp_mrna;
  Int4Ptr     mismatches;
  Int4        nmismatches;
  IngMisListPtr PNTR mismlist;
  Int4        grey_zone;
  Int4        blue_zone;
  Int4        brown_zone; 
} IngExplore;


static IngMisListPtr PNTR Ing_MisListNew(Int4 nmismatches)
{
  IngMisListPtr PNTR mislist;
  Int4               i;

  mislist = (IngMisListPtr PNTR)MemNew(sizeof(IngMisListPtr) * nmismatches);
  for (i = 0; i<nmismatches; i++)
    mislist[i] = (IngMisListPtr)MemNew(sizeof(IngMisList));

  return mislist;
}

static IngMisListPtr PNTR Ing_MisListFree(IngMisListPtr PNTR mislist, Int4 nmismatches)
{
  Int4 i;

  for (i = 0; i<nmismatches; i++)
    MemFree(mislist[i]);

  return MemFree(mislist);
}

static Boolean Ing_is_frameshift(Int4Ptr mismatches, Int4 nmismatches, Int4 index, SeqAlignPtr sap, Int4 row)
{
  Int4 pos;
  Int4 mm, mm_orig;
  Int4 gm, gm_orig;
  Int4 i;
  Int4 gap_count = 1;

  if (row == 1){
    mm_orig = AlnMgr2MapBioseqToSeqAlign(sap, mismatches[index], 2);
    i = index - 1;
    while (i >= 0){
      gm = AlnMgr2MapBioseqToSeqAlign(sap, mismatches[i], 1);
      if (gm == -1){
        mm = AlnMgr2MapBioseqToSeqAlign(sap, mismatches[i], 2);
        if (ABS(mm - mm_orig) == 1)
          gap_count++;
        else
          break;
      }
      else 
        break;
      i--;
    }
    i = index + 1;
    while (i < nmismatches){
      gm = AlnMgr2MapBioseqToSeqAlign(sap, mismatches[i], 1);
      if (gm == -1){
        mm = AlnMgr2MapBioseqToSeqAlign(sap, mismatches[i], 2);
        if (ABS(mm - mm_orig) == 1)
          gap_count++;
        else
          break;
      }
      else 
        break;
      i++;
    }
  }
  else {
    gm_orig = AlnMgr2MapBioseqToSeqAlign(sap, mismatches[index], 1);
    i = 0;
    while (i >= 0){
      mm = AlnMgr2MapBioseqToSeqAlign(sap, mismatches[index] - i, 2);
      if (mm == -1){
        gm = AlnMgr2MapBioseqToSeqAlign(sap, mismatches[index] - i, 1);
        if (ABS(gm - gm_orig) == 1)
          gap_count++;
        else
          break;
      }
      else 
        break;
      i--;
    }
    i = 0;
    while (i < nmismatches){
      mm = AlnMgr2MapBioseqToSeqAlign(sap, mismatches[index] + i, 2);
      if (mm == -1){
        gm = AlnMgr2MapBioseqToSeqAlign(sap, mismatches[index] + i, 1);
        if (ABS(gm - gm_orig) == 1)
          gap_count++;
        else
          break;
      }
      else 
        break;
      i++;
    }
  }

  if ((Boolean)(gap_count % 3))
    return TRUE; /* is frameshift */
  else
    return FALSE; /* gap is multiple of 3 */
}

static Boolean Ing_range(SeqAlignPtr sap, Int4 mismatch, Int4 row)
{
  Int4 pos;
  Int4 start, stop;

  AlnMgr2GetNthSeqRangeInSA(sap, row, &start, &stop);
  pos = AlnMgr2MapBioseqToSeqAlign(sap, mismatch, row);
  if (pos >= start && pos <= stop)
    return TRUE;
  else 
    return FALSE;
}

static Int4 Ing_get_nearest(SeqAlignPtr sap, Int4 mismatch)
{
  Int4 mm;
  Int4 i = 1;
  Int4 start, stop;

  while (Ing_range(sap, mismatch - i, 2) || Ing_range(sap, mismatch + i, 2)){
    mm = AlnMgr2MapBioseqToSeqAlign(sap, mismatch - i, 2);
    if (mm != -1)
      return mm;
    else {
      mm = AlnMgr2MapBioseqToSeqAlign(sap, mismatch + i, 2);
      if (mm != -1)
        return mm;
    }
      i++;
  }
  return 0;
}

static Boolean LIBCALLBACK Ing_MismatchProc(SeqFeatPtr sfp, SeqMgrFeatContextPtr context)
{
  IngExplore  *iep;
  Int4Ptr     mismatches = NULL;
  Int4Ptr     ivals = NULL;
  Int4        mmis, gmis, mm;
  Int4        mstart, gstart;
  Int4        start, stop, temp;
  Int4        i, j, k, pos;
  Int4        L1, L2, L3, R1, R2, R3;
  Int4        nmismatches;
  SeqIdPtr    sip = NULL;
  BioseqPtr   bsp_mrna = NULL, bsp_gen = NULL;
  SeqPortPtr  spp_mrna = NULL, spp_gen = NULL;
  Uint1       gcodon[3], mcodon[3], aa_gen, aa_mrna;
  Uint1       strand;
  SeqAlignPtr sap = NULL;
  Boolean        InRange = FALSE, AreSame = TRUE;
  CharPtr        gcd, gcdi;
  GeneticCodePtr gcp;
  ValNodePtr     vnp;


  if (context->featdeftype==FEATDEF_CDS){
    iep=(IngExplore *)context->userdata;
    iep->cds_count++;
    sap = iep->sap;
    nmismatches = iep->nmismatches;
    mismatches = iep->mismatches;
    strand = AlnMgr2GetNthStrand(sap, 2);

    for (i = 0; i < nmismatches; i++){
      gmis = AlnMgr2MapBioseqToSeqAlign(sap, mismatches[i], 1);
      mmis = AlnMgr2MapBioseqToSeqAlign(sap, mismatches[i], 2);   
 
      /* out of range */
      if (mmis != -1)
        mm = mmis;
      else 
        mm = Ing_get_nearest(sap, mismatches[i]);

      if (context->numivals == 1){
        if (mm > context->left && mm < context->right) {
          InRange = TRUE;
        }
      }
      else {
        ivals = context->ivals;
        for (k = 0; k < (context->numivals*2); k += 2){
          if (mmis > ivals[k] && mmis < ivals[k+1])
            InRange = TRUE;
        }
      }
      if (!InRange) {
        if (mmis != -1){
          iep->mismlist[i]->mism_pos = mmis;
          iep->mismlist[i]->mismtype = MIS_NOTINCDS;
        }
        else {
          iep->mismlist[i]->mism_pos = gmis;
          iep->mismlist[i]->mismtype = MIS_NOTINCDS_genomic;
        }
        continue;
      }

      /* gap on mRNA sequence */
      if (mmis == -1){
        if (Ing_is_frameshift(mismatches, nmismatches, i, sap, 2)){
          iep->mismlist[i]->mism_pos = gmis;
          iep->mismlist[i]->mismtype = MIS_FRAMESHIFT_by_insert;
          continue;
        }
        else {
          iep->mismlist[i]->mism_pos = gmis;
          iep->mismlist[i]->mismtype = MIS_INSERT;
          continue;
        }
      }
      /* gap on genomic sequence */
      if (gmis == -1){
        if (Ing_is_frameshift(mismatches, nmismatches, i, sap,1)){
          iep->mismlist[i]->mism_pos = mmis;
          iep->mismlist[i]->mismtype = MIS_FRAMESHIFT_by_delete;
          continue;
        }
        else {
          iep->mismlist[i]->mism_pos = mmis;
          iep->mismlist[i]->mismtype = MIS_DELETE;
          continue;
        }
      }

      /* downstream of frameshift */
      if (iep->grey_zone == iep->cds_count) {
        iep->mismlist[i]->mism_pos = mmis;
        iep->mismlist[i]->mismtype = MIS_GREYZONE;
        continue;
      }
      /* downstream of extension */
      if (iep->blue_zone == iep->cds_count) {
        iep->mismlist[i]->mism_pos = mmis;
        iep->mismlist[i]->mismtype = MIS_BLUEZONE;
        continue;
      }
      /* downstream of nonsense */
      if (iep->brown_zone == iep->cds_count) {
        iep->mismlist[i]->mism_pos = mmis;
        iep->mismlist[i]->mismtype = MIS_BROWNZONE;
        continue;
      }

      bsp_mrna = iep->bsp_mrna;
      bsp_gen = iep->bsp_gen;

      pos = mmis - context->left;
      if ((pos % 3) == 1) {
        mstart = mmis - 1;
        gstart = (strand == Seq_strand_minus)?gmis + 1 : gmis - 1;
      }
      else if ((pos % 3) == 2) {
        mstart = mmis - 2;
        gstart = (strand == Seq_strand_minus)?gmis + 2 : gmis - 2;
      }
      else if (!(Boolean)(pos % 3)) {
        mstart = mmis;
        gstart = gmis;
      }
      L1 = context->left;
      L2 = context->left + 1;
      L3 = context->left + 2;
      R1 = context->right;
      R2 = context->right-1;
      R3 = context->right-2;

      /* get genetic code */
      gcp =  GeneticCodeFind (1, NULL);
      if (gcp == NULL)
        return FALSE;
      gcd = gcdi = NULL;
      vnp = (ValNodePtr) gcp->data.ptrvalue;
      while (vnp != NULL){
        if (vnp->choice == 6)       /* sncbieaa */
          gcdi = (CharPtr) vnp->data.ptrvalue;
        else if (vnp->choice == 3)  /* ncbieaa */
          gcd = (CharPtr) vnp->data.ptrvalue;
        vnp = vnp->next;
      }

      if (gcd == NULL){
        ErrPostEx (SEV_ERROR, 0, 105, "Could not get genetic code for translation");
        ErrShow ();
        return FALSE;
      }

      AlnMgr2GetNthSeqRangeInSA(sap, 1, &start, &stop);
      if (strand == Seq_strand_minus)
        gstart = start + (stop - gstart);

      spp_gen = SeqPortNew(bsp_gen, start, stop, strand, Seq_code_ncbi4na);
      SeqPortSeek(spp_gen, gstart-start, SEEK_SET);
      gcodon[0] = SeqPortGetResidue(spp_gen);
      gcodon[1] = SeqPortGetResidue(spp_gen);
      gcodon[2] = SeqPortGetResidue(spp_gen);
      aa_gen = AAForCodon(gcodon, gcd);

      AlnMgr2GetNthSeqRangeInSA(sap, 2, &start, &stop);
      spp_mrna = SeqPortNew(bsp_mrna, start, stop, Seq_strand_plus, Seq_code_ncbi4na);
      SeqPortSeek(spp_mrna, mstart-start, SEEK_SET);
      mcodon[0] = SeqPortGetResidue(spp_mrna);
      mcodon[1] = SeqPortGetResidue(spp_mrna);
      mcodon[2] = SeqPortGetResidue(spp_mrna);
      aa_mrna = AAForCodon(mcodon, gcd);

      for (j = 0; j < 3; j++)
        if (gcodon[j] != mcodon[j])
          AreSame = FALSE;

      if (AreSame){
        iep->mismlist[i]->mism_pos = mmis;
        iep->mismlist[i]->mismtype = MIS_ISNOTAMIS;
        continue;
      }
      /* mismatch in start */
      if (mstart == L1 || mstart == L2 ||
          mstart == L3){ 
        if (aa_gen != 'M') { 
          iep->mismlist[i]->mism_pos = mmis;
          iep->mismlist[i]->mismtype = MIS_START;
          iep->grey_zone = iep->cds_count;
          continue;
        }
      } /* mismatch in stop */
      else if (mstart == R1 || mstart == R2 || mstart == R3){
        if (aa_gen != '*'){
          iep->mismlist[i]->mism_pos = mmis;
          iep->mismlist[i]->mismtype = MIS_STOP;
          iep->blue_zone = iep->cds_count;
          continue;
        }
      }
      /* nonsense mismatch */
      if (aa_gen == '*') {
        iep->mismlist[i]->mism_pos = mmis;
        iep->mismlist[i]->mismtype = MIS_NONSENSE;
        iep->brown_zone = iep->cds_count;
        continue;
      }

      /* missense */
      if (aa_gen != aa_mrna) { 
        iep->mismlist[i]->mism_pos = mmis;
        iep->mismlist[i]->mismtype = MIS_MISSENSE;
        iep->mismlist[i]->from_c = (Char) aa_mrna;
        iep->mismlist[i]->to_c = (Char) aa_gen;
      }
      else { /* silent */
        iep->mismlist[i]->mism_pos = mmis;
        iep->mismlist[i]->mismtype = MIS_SILENT;
      }
      SeqPortFree(spp_mrna);
      SeqPortFree(spp_gen);
    }
  }
  return TRUE;
}

static IngMisListPtr PNTR Ing_FillMismatchTypes(Int4Ptr mismatches, Int4 nmismatches, SeqAlignPtr sap, BioseqPtr bsp_mrna)
{
  IngExplore iep;
  Boolean    filter[FEATDEF_MAX];

  iep.mismatches = mismatches;
  iep.nmismatches = nmismatches;
  iep.sap = sap;
  iep.grey_zone = 0;
  iep.blue_zone = 0;
  iep.brown_zone = 0;
  iep.cds_count = 0;
  iep.bsp_mrna = bsp_mrna;
  iep.bsp_gen = BioseqLockById(AlnMgr2GetNthSeqIdPtr(sap, 1));
  iep.mismlist = Ing_MisListNew(nmismatches);

  /* only go through cds features */
  MemSet((Pointer)filter, FALSE, sizeof(filter));
  filter[FEATDEF_CDS] = TRUE;
  SeqMgrExploreFeatures(bsp_mrna, (Pointer)&iep, Ing_MismatchProc, NULL, NULL, NULL);

  return (iep.mismlist);
}

typedef struct ing_reportprg{
  CharPtr  line;
  Boolean  newline;
  struct ing_reportprg PNTR next;
} IngReportPrg, PNTR IngReportPrgPtr;

static Int4 Ing_Print(SegmenT seg, Int4 row, Int4 nlines, Int4 nchars, CharPtr tmpstr, Uint2 primID)
{
  CharPtr ch=NULL, end=NULL;
  CharPtr temp=NULL;
  Int4    i;
  Int4    strlen=0, leftover;
  Int4    lastPos=0;

  strlen=StringLen(tmpstr);
  for (i=0; i<nlines; i++){
    temp=StringSave(tmpstr);
    ch=temp+(i*nchars);
    leftover=strlen-(i*nchars);
    if (leftover<nchars)
      nchars=leftover;
    end=ch+nchars;
    *end='\0';
    AddLabel(seg, 0, row*(-15), ch, SMALL_TEXT, 0, UPPER_RIGHT, primID);
    row++;
    if (i==nlines-1)
      lastPos=StringWidth(ch);
    MemFree(temp);
  }

  return lastPos;
}
static void Ing_DrawShortLine(SegmenT seg, Int4 row, Int4 nchars)
{
  CharPtr temp;

  temp=StringSave("    ........................");
  Ing_Print(seg, row, 1, nchars, temp, 0);
  MemFree(temp); 
}

static void Ing_DrawLongLine(SegmenT seg, Int4 row, Int4 nchars)
{
  CharPtr temp;

  temp=StringSave("---------------------------------------------------------------------------------------------------------------------------------------");
  Ing_Print(seg, row, 1, nchars, temp, 0);
  MemFree(temp); 
}


static Uint1Ptr Ing_MakeColor(Uint1 r, Uint1 g, Uint1 b){
  Uint1Ptr clr;
  
  clr = (Uint1Ptr)MemNew(sizeof(Uint1)*3);
  clr[0]=r;
  clr[1]=g;
  clr[2]=b;
  return (clr);
}

extern Int4 Ing_PrintText(CharPtr str, SegmenT seg, Int4 row, Int4 winsize, Uint1Ptr clr, Uint2 primID) 
{
  CharPtr temp = NULL;
  Int4    swidth, slen;
  Int2    nlines, nchars, cxChar;

  AddAttribute(seg, COLOR_ATT, clr, 0, 0, 0, 0);
  temp=StringSave(str);
  swidth = StringWidth(temp);
  slen = StringLen(temp);
  cxChar = (Int2) ceil((float)swidth/slen);
  nchars = (Int2)((float)winsize/cxChar);

  nlines = (Int2)ceil((float)swidth/winsize);
  Ing_Print(seg, row, nlines, nchars, temp, primID);
  MemFree(temp);

  return nlines;
}


static void Ing_PrintColorCode(void)
{
  Int4 row = 0;
  SegmenT   seg;
  VieweR    v;
  WindoW    w;
  Uint1Ptr  purple, red, lime, cyan, blue, dkblue;
  Uint1Ptr  ltgrey, brown, curry, green, dkgreen;

  /* create colors */
  purple = Ing_MakeColor(163, 52, 190);
  red = Ing_MakeColor(224, 0, 60);
  lime = Ing_MakeColor(224,224,0);
  cyan = Ing_MakeColor(0, 235, 245);
  ltgrey = Ing_MakeColor(127, 127, 127);
  brown =  Ing_MakeColor(200,95,39);
  curry = Ing_MakeColor(210, 154, 14);
  green = Ing_MakeColor(0,200, 12);
  dkgreen = Ing_MakeColor(0,94, 0);
  blue = Ing_MakeColor(0, 125, 245); 
  dkblue = Ing_MakeColor(0, 64, 194);

  w = FixedWindow(-50, -90, -10, -10, "Mismatch Color Codes", StdCloseWindowProc);
  v=CreateViewer(w, 350, 300, FALSE, FALSE);
  seg=CreatePicture();

  AddAttribute(seg, COLOR_ATT, BLACK_COLOR, 0, 0, 0, 0);
  AddLabel(seg, 0, row*(-15), "  -- Mismatch is in Coding Region --  ", SMALL_TEXT, 0, UPPER_RIGHT, 0);
  row++;

  AddAttribute(seg, COLOR_ATT, curry, 0, 0, 0, 0);
  AddLabel(seg, 0, row*(-15), "curry -- silent", SMALL_TEXT, 0, UPPER_RIGHT, 0);
  row++;

  AddAttribute(seg, COLOR_ATT, purple, 0, 0, 0, 0);
  AddLabel(seg, 0, row*(-15), "purple -- nonsense", SMALL_TEXT, 0, UPPER_RIGHT, 0);
  row++;

  AddAttribute(seg, COLOR_ATT, red, 0, 0, 0, 0);
  AddLabel(seg, 0, row*(-15), "red -- frame shift, gap on mRNA", SMALL_TEXT, 0, UPPER_RIGHT, 0);
  row++;

  AddAttribute(seg, COLOR_ATT, dkblue, 0, 0, 0, 0);
  AddLabel(seg, 0, row*(-15), "dkblue -- frame shift, gap on genomic", SMALL_TEXT, 0, UPPER_RIGHT, 0);
  row++;

  AddAttribute(seg, COLOR_ATT, lime, 0, 0, 0, 0);
  AddLabel(seg, 0, row*(-15),"yellow -- missense in start codon" , SMALL_TEXT, 0, UPPER_RIGHT, 0);
  row++;

  AddAttribute(seg, COLOR_ATT, GREEN_COLOR, 0, 0, 0, 0);
  AddLabel(seg, 0, row*(-15),"bright green -- missense in stop codon" , SMALL_TEXT, 0, UPPER_RIGHT, 0);
  row++;

  AddAttribute(seg, COLOR_ATT, cyan, 0, 0, 0, 0);
  AddLabel(seg, 0, row*(-15),"cyan -- missense" , SMALL_TEXT, 0, UPPER_RIGHT, 0);
  row++;

  AddAttribute(seg, COLOR_ATT, MAGENTA_COLOR, 0, 0, 0, 0);
  AddLabel(seg, 0, row*(-15),"pink -- insertion on genomic (in genomic coords)" , SMALL_TEXT, 0, UPPER_RIGHT, 0);
  row++;

  AddAttribute(seg, COLOR_ATT, green, 0, 0, 0, 0);
  AddLabel(seg, 0, row*(-15), "green -- deletion on genomic", SMALL_TEXT, 0, UPPER_RIGHT, 0);
  row++;

  AddAttribute(seg, COLOR_ATT, ltgrey, 0, 0, 0, 0);
  AddLabel(seg, 0, row*(-15), "grey -- downstream of frameshift mutation", SMALL_TEXT, 0, UPPER_RIGHT, 0);
  row++;

  AddAttribute(seg, COLOR_ATT, blue, 0, 0, 0, 0);
  AddLabel(seg, 0, row*(-15), "ltblue -- downstream of missense in stop", SMALL_TEXT, 0, UPPER_RIGHT, 0);
  row++;

  AddAttribute(seg, COLOR_ATT, brown, 0, 0, 0, 0);
  AddLabel(seg, 0, row*(-15), "brown -- downstream of nonsense mutation", SMALL_TEXT, 0, UPPER_RIGHT, 0);
  row+=2;

  AddAttribute(seg, COLOR_ATT, BLACK_COLOR, 0, 0, 0, 0);
  AddLabel(seg, 0, row*(-15), "  -- Mismatch is NOT in Coding Region --  ", SMALL_TEXT, 0, UPPER_RIGHT, 0);
  row++;

  AddAttribute(seg, COLOR_ATT, dkgreen, 0, 0, 0, 0);
  AddLabel(seg, 0, row*(-15), "[dkgreen] -- gap on mRNA (in genomic coords)", SMALL_TEXT, 0, UPPER_RIGHT, 0);
  row++;

  AddAttribute(seg, COLOR_ATT, BLACK_COLOR, 0, 0, 0, 0);
  AddLabel(seg, 0, row*(-15), "black -- default", SMALL_TEXT, 0, UPPER_RIGHT, 0);
  row++;


  AttachPicture (v, seg, INT4_MIN, INT4_MAX, UPPER_RIGHT, 1 , 1, NULL);

  Show(w);
  /* free colors */
  MemFree(purple);
  MemFree(red);
  MemFree(lime);
  MemFree(cyan);
  MemFree(ltgrey);
  MemFree(blue);
  MemFree(brown);
  MemFree(curry);
  MemFree(green);
  MemFree(dkgreen);
  MemFree(dkblue);
  
}

static Int4 Ing_PrintSpideyReport(IngReportPtr Report, SegmenT seg,  SPI_mRNAPtr spidp, BioseqPtr bsp_genomic, Int4 row, Int4 winsize, Int4 nchars, Boolean bSummary)
{
  SPI_ExonProfPtr   epp=NULL, epp_cur=NULL;
  Char              tmpstr[500]={""};
  Char              str4[7][100]={""};
  Char              strG[25]={""}, strM[25]={""};
  CharPtr PNTR      str3;
  Char              defline[255]={""}, idstr[255]={""};
  Char              mismatch_title[100] ={""};
  Char              tmp[255]={""};
  CharPtr           temp=NULL;
  Int4              i, j, splicedon=0, c;
  Int4              k;
  Int4              spliceacc=0;
  Int4              strpos;
  Int4              nmismatches = 0;
  Int4              mmis, gmis, miscount = 0;
  Int4              swidth, xstringPos=0;
  Int4              len=0;
  Int4              top;
  Int4Ptr           mismatches = NULL;
  Uint1             gstrand, mstrand;
  Uint1Ptr          DkGrey, DkGreen, purple;
  Uint1Ptr          red, lime, cyan, blue, ltgrey;
  Uint1Ptr          brown, curry, green, dkgreen, dkblue;
  Int2              cxChar=0, slen=0;
  Int2              nlines;
  SelStructPtr      ssp=NULL, tssp=NULL;
  BioseqPtr         bsp_mrna = NULL;
  SeqIdPtr          sip = NULL;
  IngMisListPtr PNTR mismlist = NULL;
  IngMisListPtr      mismp = NULL;
  enumPrimAddOrder   oldOrder;
  Boolean            bHighlight;

  if (!spidp) return row;

  ssp=ObjMgrGetSelected();

  /* create colors */
  DkGrey = Ing_MakeColor(145, 145, 145);
  DkGreen = Ing_MakeColor(0, 192, 120);
  purple = Ing_MakeColor(163, 52, 190);
  red = Ing_MakeColor(224, 0, 60);
  lime = Ing_MakeColor(224,224,0);
  cyan = Ing_MakeColor(0, 235, 245);
  ltgrey = Ing_MakeColor(127, 127, 127);
  brown =  Ing_MakeColor(200,95,39);
  curry = Ing_MakeColor(210, 154, 14);
  green = Ing_MakeColor(0,127, 12);
  dkgreen = Ing_MakeColor(0, 94, 0);
  blue = Ing_MakeColor(0, 125, 245); 
  dkblue = Ing_MakeColor(0, 64, 194);

  sip = AlnMgr2GetNthSeqIdPtr(spidp->saps[0], 2);
  bsp_mrna = BioseqLockById(sip);
  if (bsp_mrna==NULL) return row;

  /* print analysis name */
  row++;
  MemSet((Pointer)tmpstr, '\0', sizeof(tmpstr));
  temp=StringSave("--   Spidey   --     ");
  swidth=StringWidth(temp);
  slen=StringLen(temp);
  cxChar = (Int2)ceil((float)swidth/slen);
  nlines = (Int2)ceil((float)swidth/winsize);
  nchars = (Int2)ceil((float)winsize/cxChar);
  AddAttribute(seg, COLOR_ATT, RED_COLOR, 0, 0, 0, 0);
  Ing_Print(seg, row, nlines, nchars, temp, 0);
  MemFree(temp);
  row+=nlines + 1;
  
  /* print defline */

  epp=spidp->epp;
  FastaDefLine (bsp_genomic, defline, 200, NULL, NULL, 0);
  SeqIdWrite(bsp_genomic->id, idstr, PRINTID_FASTA_LONG, 41);
  MemSet((Pointer)tmpstr, '\0', sizeof(tmpstr));
  sprintf(tmpstr, "Genomic Sequence: %s ", idstr);
  sprintf(tmp, "%s, ", defline);
  StringCat(tmpstr, tmp);
  sprintf(tmp, "%d bp", bsp_genomic->length);
  StringCat(tmpstr, tmp);
  swidth=StringWidth(tmpstr);
  slen=StringLen(tmpstr);
  cxChar = (Int2)ceil((float)swidth/slen);
  nlines = (Int2)ceil((float)swidth/winsize);
  nchars = (Int2)ceil((float)winsize/cxChar);
  AddAttribute(seg, COLOR_ATT, BLUE_COLOR, 0, 0, 0, 0);
  temp=StringSave(tmpstr);
  Ing_Print(seg, row, nlines, nchars, temp, 0);
  MemFree(temp);
  row+=nlines;
  MemSet((Pointer)tmpstr, '\0', sizeof(tmpstr));
  FastaDefLine (bsp_mrna, defline, 200, NULL, NULL, 0);
  SeqIdWrite(bsp_mrna->id, idstr, PRINTID_FASTA_LONG, 41);
  sprintf(tmpstr, "mRNA Sequence: %s ", idstr);
  sprintf(tmp, "%s, ", defline);
  StringCat(tmpstr, tmp);
  sprintf(tmp, "%d bp", bsp_mrna->length);
  StringCat(tmpstr, tmp);
  swidth=StringWidth(tmpstr);
  nlines= (Int2)ceil((float)swidth/winsize);
  AddAttribute(seg, COLOR_ATT, DkGreen, 0, 0, 0, 0);
  temp=StringSave(tmpstr);
  Ing_Print(seg, row, nlines, nchars, temp, 0);
  MemFree(temp);
  row+=nlines;
  AddAttribute(seg, COLOR_ATT, BLACK_COLOR, 0, 0, 0, 0);
  Ing_DrawShortLine(seg, row, nchars); 

  /* Print exon information */

  if (bSummary){
    sprintf(str4[0], "Alignment is on %s strand of mRNA sequence.", (spidp->strand == Seq_strand_minus?"minus":"plus"));
    sprintf(tmp, "Number of exons: %d.", spidp->numexons);
    StringCat(str4[1], tmp);
    sprintf(tmp, (spidp->polyAtail==0?"No PolyA tail.":"Length of PolyA tail: %d. %s"), ABS(spidp->polyAtail), (spidp->polyAtail<0?"PolyA tail does align.":"PolyA tail doesn't align."));
    StringCat(str4[2], tmp);
    
    sprintf(tmp, "mRNA coverage: %d%%.", spidp->mRNAcoverage);
    StringCat(str4[3], tmp);
    sprintf(tmp, "Overall percent identity: %.1f%%.", (FloatHi)(100) - spidp->mismatch);
    StringCat(str4[4], tmp);
    row++;
    for (i=0; i<5; i++){
      temp=StringSave(str4[i]);
      swidth=StringWidth(temp);
      nlines = (Int2)ceil((float)swidth/winsize);
      Ing_Print(seg, row, nlines, nchars, temp, 0);
      MemFree(temp);
      row+=nlines;
    }
    BioseqUnlock(bsp_mrna);
    return row;
  }
  else {
    
    splicedon = 0;
    str3=(CharPtr PNTR)MemNew(sizeof(CharPtr)*(spidp->numexons+1));

    /* columns */      
    str3[0]=(CharPtr)MemNew(sizeof(Char)*255);
    sprintf(str3[0], "%-6s %-15s %-15s %-8s %-8s %-5s %-12s %-12s %-12s", "Exon", "Genomic", "mRNA", "length", "id", "Gaps", "Splice Acceptor", "Splice Donor", "Mismatches");
    /* content */
    
    for (j=1, i=0; j<spidp->numexons+1; j++, i++)
      {
        if (spidp->strand == Seq_strand_minus)
          c = spidp->numexons - i - 1;
        else
          c = i;
        splicedon += spidp->splicedon[i];
        spliceacc += spidp->spliceacc[i];
        str3[j]=(CharPtr)MemNew(sizeof(Char)*255);
        nmismatches = 0;
        len=spidp->mstops[c]-spidp->mstarts[c]+1;
        epp_cur=Ing_NumMismatches(epp, c+1, &nmismatches);
        sprintf(strG, "%d-%d", spidp->gstarts[c], spidp->gstops[c]);
        sprintf(strM, "%d-%d", spidp->mstarts[c], spidp->mstops[c]);
        sprintf(str3[j], "%-6d %-15s %-15s %-8d %-2.1f%% %5d %12d %12d %12d", i+1, strG, strM, len, spidp->exonid[c], spidp->exongaps[c], spidp->spliceacc[c], spidp->splicedon[c], nmismatches);
      }

    gstrand=spidp->strand;
    if (spidp->revcomp){
      mstrand=Seq_strand_plus;
      if (gstrand==Seq_strand_plus)
        mstrand=Seq_strand_minus;
    }
    else
      mstrand=gstrand;
    
    sprintf(str4[0], "Alignment is on %s strand of genomic sequence and on %s strand of mRNA sequence", (gstrand==Seq_strand_minus?"minus":"plus"), (mstrand==Seq_strand_minus?"minus":"plus."));
    sprintf(tmp, "Number of exons: %d.", spidp->numexons);
    StringCat(str4[1], tmp);
    sprintf(tmp, "Number of splice donor sites: %d.", splicedon);
    StringCat(str4[2], tmp);
    sprintf(tmp, "Number of splice acceptor sites: %d.", spliceacc);
    StringCat(str4[3], tmp);
    sprintf(tmp, (spidp->polyAtail==0?"No PolyA tail.":"Length of PolyA tail: %d. %s"), ABS(spidp->polyAtail), (spidp->polyAtail<0?"PolyA tail does align.":"PolyA tail doesn't align."));
    StringCat(str4[4], tmp);
    
    sprintf(tmp, "mRNA coverage: %d%%.", spidp->mRNAcoverage);
    StringCat(str4[5], tmp);
    sprintf(tmp, "Overall percent identity: %.1f%%.", (FloatHi)(100) - spidp->mismatch);
    StringCat(str4[6], tmp);
    row++;
    for (i=0; i<7; i++){
      temp=StringSave(str4[i]);
      swidth=StringWidth(temp);
      nlines = (Int2)ceil((float)swidth/winsize);
      Ing_Print(seg, row, nlines, nchars, temp, 0);
      MemFree(temp);
      row+=nlines;
    }
  }

  Ing_DrawShortLine(seg, row, nchars);
  row++;

  /* print mismatch title */
  top = row - 1;
  oldOrder=ChangeAddPrimOrder(ADD_TO_HEAD);
  sprintf(mismatch_title, " Mismatches are shown in mRNA coordinates  - double click for color code - ");
  row += Ing_PrintText(mismatch_title, seg, row, winsize, WHITE_COLOR, 0);
  AddAttribute(seg, COLOR_ATT, RED_COLOR, 0, 0, 0, 0);
  AddRectangle(seg, 0, top*(-15), MIN(StringWidth(mismatch_title)+2, winsize + 5), (row - 1)*(-15), NO_ARROW,TRUE,1);
  
  ChangeAddPrimOrder(oldOrder);
  row++;

  /* print column headers */
  row += Ing_PrintText(str3[0], seg, row, winsize,BLUE_COLOR, 0);
  MemFree(str3[0]);

  AddAttribute(seg, COLOR_ATT, BLACK_COLOR, 0, 0, 0, 0);

  for (k=1, i=0; k<spidp->numexons+1; k++, i++){

    /* print exon information */
    swidth=StringWidth(str3[k]);
    nlines = (Int2)ceil((float)swidth/winsize);
    temp=StringSave(str3[k]);
    if (spidp->strand == Seq_strand_minus)
      c = spidp->numexons - k;
    else
      c = i;

    /* if exon is selected, print in red */
    bHighlight = FALSE;
    if (ssp){
      tssp=ssp;
      while (tssp){
        if (tssp->itemID == spidp->saps[c]->idx.itemID){
          if (Report->igvp->isExons)
            bHighlight = TRUE;         
          Report->last_segID=(Uint2)SegmentID(seg);
          Report->isPrevSelected=TRUE;
        }
        tssp=tssp->next;
      }
    }
    if (bHighlight == TRUE){
      AddAttribute(seg, COLOR_ATT, RED_COLOR, 0, 0, 0, 0);
    } 
    else{ 
      AddAttribute(seg, COLOR_ATT, BLACK_COLOR, 0, 0, 0, 0);
    }
    xstringPos=Ing_Print(seg, row, nlines, nchars, temp, 0);
    MemFree(temp);
    if (xstringPos==0)
      row+=nlines;
    else 
      row+=nlines-1;
    nmismatches=0;
    epp_cur=Ing_NumMismatches(epp, c+1, &nmismatches);
    if (nmismatches>0){
      mismatches = epp_cur->mismatches;
      mismlist = Ing_FillMismatchTypes(epp_cur->mismatches, nmismatches, spidp->saps[c], bsp_mrna);
      row++;
      AddAttribute(seg, COLOR_ATT, BLACK_COLOR, 0, 0, 0, 0);
      AddLabel(seg, 0, row*(-15), "  Mismatches:   ", SMALL_TEXT, 0, UPPER_RIGHT, 0);
      xstringPos=StringWidth("  Mismatches:   ");
      
      for (j=0; j<nmismatches; j++){
        miscount++;
        mismp = mismlist[j];
        if (mismp->mismtype == MIS_ISNOTAMIS)
          continue;
        mmis = mismp->mism_pos;
        gmis = AlnMgr2MapBioseqToSeqAlign(spidp->saps[c], mismatches[j], 1);   

        mmis++;
        switch (mismp->mismtype) {
        case MIS_NONSENSE:
          AddAttribute(seg, COLOR_ATT, purple, 0, 0, 0, 0);
          break;
        case MIS_SILENT:
          AddAttribute(seg, COLOR_ATT, curry, 0, 0, 0, 0);
          break;
        case MIS_FRAMESHIFT_by_insert:
          AddAttribute(seg, COLOR_ATT, red, 0, 0, 0, 0);
          break;
        case MIS_FRAMESHIFT_by_delete:
          AddAttribute(seg, COLOR_ATT, dkblue, 0, 0, 0, 0);
          break;
        case MIS_START:
          AddAttribute(seg, COLOR_ATT, lime, 0, 0, 0, 0);
          break;
        case MIS_STOP:
          AddAttribute(seg, COLOR_ATT, GREEN_COLOR, 0, 0, 0, 0);
          break;
        case MIS_MISSENSE:
          AddAttribute(seg, COLOR_ATT, cyan, 0, 0, 0, 0);
          break;
        case MIS_GREYZONE:
          AddAttribute(seg, COLOR_ATT, ltgrey, 0, 0, 0, 0);
          break;
        case MIS_BLUEZONE:
          AddAttribute(seg, COLOR_ATT, blue, 0, 0, 0, 0);
          break;
        case MIS_BROWNZONE:
          AddAttribute(seg, COLOR_ATT, brown, 0, 0, 0, 0);
          break;
        case MIS_NOTINCDS:
          AddAttribute(seg, COLOR_ATT, BLACK_COLOR, 0, 0, 0, 0);
          break;
        case MIS_NOTINCDS_genomic:
          AddAttribute(seg, COLOR_ATT, dkgreen, 0, 0, 0, 0);
          break;
        case MIS_INSERT:
          AddAttribute(seg, COLOR_ATT, MAGENTA_COLOR, 0, 0, 0, 0);
          break;
        case MIS_DELETE:
          AddAttribute(seg, COLOR_ATT, green, 0, 0, 0, 0);
          break;
        }
       
        MemSet ((Pointer)tmp, 0, sizeof(tmp));
        if (mismp->mismtype == MIS_MISSENSE)
          sprintf(tmp, " %d(%c->%c) ", mmis, mismp->from_c, mismp->to_c);
        else if (mismp->mismtype == MIS_NOTINCDS_genomic)
          sprintf(tmp, " [%d] ", mmis);
        else
          sprintf(tmp, " %d ", mmis);
      
        /*
        MemSet ((Pointer)tmp, 0, sizeof(tmp));
        if (mismp->mismtype == MIS_MISSENSE)
          sprintf(tmp, " %d(%c->%c) ", gmis, mismp->from_c, mismp->to_c);
        else if (mismp->mismtype == MIS_NOTINCDS_genomic)
          sprintf(tmp, " [%d] ", gmis);
        else
          sprintf(tmp, " %d ", gmis);
         */
        strpos=xstringPos+StringWidth(tmp);
        if (strpos>winsize){
          row++;
          xstringPos=0;
          strpos=StringWidth(tmp);
        }
        AddLabel (seg, xstringPos, row*(-15), tmp, SMALL_TEXT, 0, UPPER_RIGHT, miscount);
        xstringPos=strpos;
      }   
      Ing_MisListFree(mismlist, nmismatches);
      AddAttribute(seg, COLOR_ATT, BLACK_COLOR, 0, 0, 0, 0);
    }
    MemFree(str3[k]);
    row+=2;

  }

  MemFree(str3);
  /* free colors */
  MemFree(DkGreen);
  MemFree(DkGrey);
  MemFree(purple);
  MemFree(red);
  MemFree(lime);
  MemFree(cyan);
  MemFree(ltgrey);
  MemFree(blue);
  MemFree(brown);
  MemFree(curry);
  MemFree(green);
  MemFree(dkgreen);
  MemFree(dkblue);

  BioseqUnlock(bsp_mrna);
  return row;
}


static Int4 Ing_PrintBlastReport(IngReportPtr Report, SegmenT seg, SeqAlignPtr sap, BioseqPtr bsp_genomic, Int4 row, Int4 winsize, Int4 nchars, CharPtr name, Boolean bSummary)
{
  Int4           num_mismatches = 0, num_gap_opens = 0;
  Int4           align_length = 0, num_ident = 0, tot_ident = 0;
  FloatHi        per_ident, bit_score, evalue;
  CharPtr        eval_buff, bit_score_buff;
  SeqAlignPtr    salp = NULL;
  Int4           i, j, k;
  AMAlignIndex2Ptr amaip = NULL;
  BioseqPtr      bsp_mrna = NULL;
  SeqIdPtr       sip = NULL;
  Uint1Ptr       query_seq, subject_seq;
  Uint1          q_strand = 0, s_strand = 0;
  Char           defline[255]={""}, idstr[255]={""};
  Char           tmp[255]={""}, tmpstr[500]={""};
  CharPtr        temp = NULL;
  Char           contents[4][50]={""};
  Int4           swidth;
  Int2           cxChar, slen,  nlines;
  Int4           len;
  Int4           from = 0, to = 0;
  Uint1          DkGreen[3];
  Uint1          DkGray[3];
  AlnMsg2Ptr amp1 = NULL;
  AlnMsg2Ptr amp2 = NULL;
  Boolean        more = FALSE;
  SelStructPtr   ssp = NULL;
  SelStructPtr   tssp = NULL;


  amaip = (AMAlignIndex2Ptr)sap->saip;
  ssp = ObjMgrGetSelected();

  DkGray[0]=145;
  DkGray[1]=145;
  DkGray[2]=145;
  DkGreen[0]=0;
  DkGreen[1]=192;
  DkGreen[2]=120;

  sip=AlnMgr2GetNthSeqIdPtr(amaip->saps[0], 2);
  bsp_mrna=BioseqLockById(sip);
  if (bsp_mrna==NULL) return row;

  /* print analysis name */
  row++;
  MemSet((Pointer)tmpstr, '\0', sizeof(tmpstr));
  sprintf(tmp, "--   %s   --     ", name);
  temp=StringSave(tmp);
  swidth=StringWidth(temp);
  slen=StringLen(temp);
  cxChar = (Int2)ceil((float)swidth/slen);
  nlines = (Int2)ceil((float)swidth/winsize);
  nchars = (Int2)ceil((float)winsize/cxChar);
  AddAttribute(seg, COLOR_ATT, RED_COLOR, 0, 0, 0, 0);
  Ing_Print(seg, row, nlines, nchars, temp, 0);
  MemFree(temp);
  row+=nlines + 1;

  /* print deflines */

  FastaDefLine (bsp_genomic, defline, 200, NULL, NULL, 0);
  SeqIdWrite(bsp_genomic->id, idstr, PRINTID_FASTA_LONG, 41);
  MemSet((Pointer)tmpstr, '\0', sizeof(tmpstr));
  sprintf(tmpstr, "Genomic Sequence: %s ", idstr);
  sprintf(tmp, "%s, ", defline);
  StringCat(tmpstr, tmp);
  sprintf(tmp, "Length %d (1..%d)", bsp_genomic->length, bsp_mrna->length);
  StringCat(tmpstr, tmp);
  swidth=StringWidth(tmpstr);
  slen=StringLen(tmpstr);
  cxChar = (Int2)ceil((float)swidth/slen);
  nlines = (Int2)ceil((float)swidth/winsize);
  nchars = (Int2)ceil((float)winsize/cxChar);
  AddAttribute(seg, COLOR_ATT, BLUE_COLOR, 0, 0, 0, 0);
  temp=StringSave(tmpstr);
  Ing_Print(seg, row, nlines, nchars, temp, 0);
  MemFree(temp);
  row+=nlines;
  MemSet((Pointer)tmpstr, '\0', sizeof(tmpstr));
  FastaDefLine (bsp_mrna, defline, 200, NULL, NULL, 0);
  SeqIdWrite(bsp_mrna->id, idstr, PRINTID_FASTA_LONG, 41);
  sprintf(tmpstr, "mRNA Sequence: %s ", idstr);
  sprintf(tmp, "%s, ", defline);
  StringCat(tmpstr, tmp);
  sprintf(tmp, "Length %d (1..%d)", bsp_mrna->length, bsp_mrna->length);
  StringCat(tmpstr, tmp);
  swidth=StringWidth(tmpstr);
  nlines = (Int2)ceil((float)swidth/winsize);
  AddAttribute(seg, COLOR_ATT, DkGreen, 0, 0, 0, 0);
  temp=StringSave(tmpstr);
  Ing_Print(seg, row, nlines, nchars, temp, 0);
  MemFree(temp);
  row+=nlines;
  AddAttribute(seg, COLOR_ATT, BLACK_COLOR, 0, 0, 0, 0);
  Ing_DrawShortLine(seg, row, nchars); 

  /* print blast output */

  if (bSummary){
    return row;
  }
  else {
    if (!Report->igvp->seqbuf){
      query_seq = Ing_FillSeqBuffer(bsp_genomic, NULL, 0, bsp_genomic->length-1);
    }
    else {
      query_seq = Report->igvp->seqbuf;
    }
    
    subject_seq = Ing_FillSeqBuffer(bsp_mrna, NULL, 0, bsp_mrna->length-1);
    AddAttribute(seg, COLOR_ATT, BLACK_COLOR, 0, 0, 0, 0);
    for (j = 0; j< amaip->numsaps; j++){
      salp = amaip->saps[j];
      if (ssp){
        tssp=ssp;
        while (tssp){
          if (tssp->itemID == salp->idx.itemID){
            AddAttribute(seg, COLOR_ATT, RED_COLOR, 0, 0, 0, 0);
            Report->last_segID=(Uint2)SegmentID(seg);
            Report->isPrevSelected=TRUE;
          }
          tssp=tssp->next;
        }
      }

      Ing_DrawShortLine(seg, row, nchars);
      row++;

      q_strand = AlnMgr2GetNthStrand(salp, 1);
      s_strand = AlnMgr2GetNthStrand(salp, 2);
      if (q_strand == Seq_strand_minus && s_strand == Seq_strand_minus)
        sprintf(contents[3], "Strand = minus / minus");
      else if (q_strand == Seq_strand_plus && s_strand == Seq_strand_minus)
        sprintf(contents[3], "Strand = plus / minus");
      else if (q_strand == Seq_strand_plus && s_strand == Seq_strand_plus)
        sprintf(contents[3], "Strand = plus / plus");
      else if (q_strand == Seq_strand_minus && s_strand == Seq_strand_plus)
        sprintf(contents[3], "Strand = minus / plus");
      
      if (salp->score) {
        Ing_GetScoreAndEvalue(salp, &eval_buff, &bit_score_buff);
        sprintf(contents[0], "Score = %s       Expect = %s", bit_score_buff, eval_buff);
        MemFree(bit_score_buff);
        MemFree(eval_buff);
      }
      else 
        sprintf (contents[0], "score is not available");
     
      amp1 = AlnMsgNew2();
      amp2 = AlnMsgNew2();
      amp1->from_aln = 0;
      amp1->to_aln = -1;
      amp1->row_num = 1;
      amp2->from_aln = 0;
      amp2->to_aln = -1; 
      amp2->row_num = 2;
      i = 1;
      while(AlnMgr2GetNextAlnBit(salp, amp1) && AlnMgr2GetNextAlnBit(salp, amp2)){
        if (amp2->type == AM_SEQ && amp1->type == AM_SEQ){
          len = amp1->to_row - amp1->from_row + 1;
          num_ident = BlastGetNumIdentical(query_seq, subject_seq, amp1->from_row, amp2->from_row, len, FALSE);
          tot_ident += num_ident;
          AlnMgr2GetNthSegmentRange(salp, i, &from, &to);
          align_length += (to-from)+1;
          num_mismatches += len - num_ident;
        }
        else 
          num_gap_opens++;
        i++;
      }

      AlnMsgFree2(amp1);
      AlnMsgFree2(amp2);
      per_ident = (float)tot_ident / align_length * 100;
      sprintf(contents[1], "Identities = %d / %d (%.1f%%)", tot_ident, align_length, per_ident);
      sprintf(contents[2], "Mismatches = %d", num_mismatches);
      for (k = 0; k<4; k++){
        if (k == 2)
          continue;
        swidth=StringWidth(contents[k]);
        nlines = (Int2)ceil((float)swidth/winsize);
        temp = StringSave(contents[k]);
        Ing_Print(seg, row, nlines, nchars, temp, 0);
        MemFree(temp);
        row += nlines;
      }
      tot_ident = 0;
      num_mismatches = 0;
      AddAttribute(seg, COLOR_ATT, BLACK_COLOR, 0, 0, 0, 0);
    }
  }

  BioseqUnlock(bsp_mrna);

  return row;
}



static MsgAnswer Ing_DoReplace(SeqAlignPtr sap, Int4 gm, Int4 mm, Uint1 gstrand, Uint1 mstrand, Int4 mismatch, Int4Ptr nleftp, Int4Ptr nrightp)
{
  Char        str[200]={""}, str2[200]={""};
  SeqPortPtr  gspp = NULL, mspp = NULL;
  BioseqPtr   gbsp, mbsp;
  SeqIdPtr    gsip, msip;
  Int4    gstart, gstop;
  Int4    mstart, mstop;
  Int4    gmis, mmis, i;
  Int4    begin_at, mreal;
  Int4    nleft = 0, nright = 0;
  Int2    gstart_pos = 0;
  Int2    gstop_pos = 0, mstop_pos = 0;
  Uint1   strand;
  Uint1Ptr   gcodon, mcodon;
  CharPtr    letters[] = {"A", "C", "G", "T", "-"};
  MsgAnswer  ans;
  

  AlnMgr2GetNthSeqRangeInSA(sap, 1, &gstart, &gstop);
  AlnMgr2GetNthSeqRangeInSA(sap, 2, &mstart, &mstop);
  gsip = AlnMgr2GetNthSeqIdPtr(sap, 1);
  msip = AlnMgr2GetNthSeqIdPtr(sap, 2);
  strand = AlnMgr2GetNthStrand(sap, 2);

  gbsp = BioseqLockById(gsip);
  mbsp = BioseqLockById(msip);
  
  gspp = SeqPortNew(gbsp, gstart, gstop, Seq_strand_plus, Seq_code_ncbi2na);
  mspp = SeqPortNew(mbsp, mstart, mstop, strand, Seq_code_ncbi2na);

  if (gm == -1){
    gmis = Ing_get_rightend(sap, mismatch, 1, &nleft, &nright);
    mstop_pos = nleft + nright + 1;
  }
  else {
    gmis = gm;
    mstop_pos = 1;
  }

  if (mm == -1){
    mmis = Ing_get_rightend(sap, mismatch, 2, &nleft, &nright);
    gstop_pos = 6 + nleft + nright +1;
  }
  else {
    mmis = mm;
    gstop_pos = 7;
  }

  /* get genomic residues */

  begin_at = (gmis - 3) - (gstart);
  if (begin_at > 0){
    SeqPortSeek(gspp, begin_at, SEEK_SET);
    gstart_pos = 0;
  }
  else {
    gstart_pos = (-1)*(begin_at);
  }
  gcodon = (Uint1Ptr)MemNew(sizeof(Uint1)*gstop_pos);
  for (i = gstart_pos; i < gstop_pos; i++){
    gcodon[i] = SeqPortGetResidue(gspp);
  }

  /* get mRNA residues */

  if (strand == Seq_strand_minus) 
    mreal = mstart + (mstop - mmis); 
  else
    mreal = mmis;

  SeqPortSeek(mspp, mreal - mstart - nleft, SEEK_SET);
  mcodon = (Uint1Ptr)MemNew(sizeof(Uint1)*mstop_pos);
  for(i = 0; i < mstop_pos; i++)
    mcodon[i] = SeqPortGetResidue(mspp);
  

  if (gm == -1){
    sprintf(str, "Replace Gap before pos %d - %s%s%s", 
            (mmis + nright) + 1, letters[gcodon[0]], 
            letters[gcodon[1]], letters[gcodon[2]]);

    for (i = 0; i < mstop_pos; i++)
      StringCat(str, "-");

    sprintf(str2, "%s%s%s  --->   %s%s%s[", 

            letters[gcodon[3]], letters[gcodon[4]],
            letters[gcodon[5]], letters[gcodon[0]], 
            letters[gcodon[1]], letters[gcodon[2]]);
    StringCat(str, str2);

    for (i = 0; i < mstop_pos; i++){
      sprintf(str2, "%s", letters[mcodon[i]]);
      StringCat(str, str2);
    }

    sprintf(str2, "]%s%s%s  ?",
            letters[gcodon[3]], letters[gcodon[4]],
            letters[gcodon[5]]);
    StringCat(str, str2);

  } 
  else if (mm == -1){
    if ((nleft + nright) > 0){
      sprintf(str, "Delete Insert between %d-%d - %s%s%s[", 
              (mmis - nleft) + 1, (mmis + nright) + 1, 
              letters[gcodon[0]], letters[gcodon[1]], 
              letters[gcodon[2]]);
    }
    else {
      sprintf(str, "Delete Insert at %d - %s%s%s[", 
              mmis + 1, letters[gcodon[0]], 
              letters[gcodon[1]], letters[gcodon[2]]);
    }
    for (i = 3; i < gstop_pos-3; i++){
      sprintf(str2, "%s", letters[gcodon[i]]);
      StringCat(str, str2);
    }
    
    sprintf(str2,"]%s%s%s  --->   %s%s%s%s%s%s  ?",
            letters[gcodon[gstop_pos-3]], letters[gcodon[gstop_pos-2]],
            letters[gcodon[gstop_pos-1]], letters[gcodon[0]],
            letters[gcodon[1]], letters[gcodon[2]], 
            letters[gcodon[gstop_pos-3]], letters[gcodon[gstop_pos-2]], 
            letters[gcodon[gstop_pos-1]]);
    StringCat(str, str2);
  }
  else {
    sprintf(str, "Replace Mismatch at pos %d - %s%s%s[%s]%s%s%s   --->   %s%s%s[%s]%s%s%s  ?",
            mmis + 1, letters[gcodon[0]],
            letters[gcodon[1]], letters[gcodon[2]],
            letters[gcodon[3]], letters[gcodon[4]],
            letters[gcodon[5]], letters[gcodon[6]],
            letters[gcodon[0]],
            letters[gcodon[1]], letters[gcodon[2]],
            letters[mcodon[0]], letters[gcodon[4]],
            letters[gcodon[5]], letters[gcodon[6]]);
  }
  

  *nleftp = nleft;
  *nrightp = nright;

  MemFree(gcodon);
  MemFree(mcodon);

  return (Message (MSG_YN, str));
}

static Boolean Ing_ReplaceByExon(SPI_mRNAPtr spidp, Int4 c, SeqAlignPtr sap_exon, SeqAlignPtr sap_continuous, SPI_ExonProfPtr epp_cur, Int4 nmismatches, Uint2 primID)
{
  Int4               miscount = 0, nkeep = 0;
  Int4               j, k, m;
  Int4               gmis, mmis, mismatch;
  Int4               nleft = 0, nright = 0, ntotal = 0;
  Int4               offset = 0;
  Int4Ptr            mismatches, keep;
  Uint1              mstrand, gstrand;
  SeqIdPtr           gsip, msip;
  MsgAnswer          ans;

  gstrand = AlnMgr2GetNthStrand(sap_exon, 1);
  mstrand = AlnMgr2GetNthStrand(sap_exon, 2);
  gsip = AlnMgr2GetNthSeqIdPtr(sap_exon, 1);
  msip = AlnMgr2GetNthSeqIdPtr(sap_exon, 2);

  mismatches = epp_cur->mismatches;
  for (j=0; j<nmismatches; j++){
    miscount++;
    if (miscount == primID){
      gmis = AlnMgr2MapBioseqToSeqAlign(sap_exon, mismatches[j], 1);
      mmis = AlnMgr2MapBioseqToSeqAlign(sap_exon, mismatches[j], 2); 
      if (gmis == -1){
        ans = Ing_DoReplace(sap_exon, gmis, mmis, gstrand, mstrand, mismatches[j], &nleft, &nright);
        if (ans == ANS_YES){            
          gmis = Ing_get_rightend(sap_exon, mismatches[j], 1, &nleft, &nright);
          ntotal = nleft + nright + 1;

          Ing_FillGap(spidp, gsip, msip, gmis, mmis, mstrand, nleft, nright, c, sap_exon, sap_continuous);

        }
      }
      else if (mmis == -1){
        ans = Ing_DoReplace(sap_exon, gmis, mmis, gstrand, mstrand, mismatches[j], &nleft, &nright);
        if (ans == ANS_YES){
          ntotal = nleft + nright + 1;

          Ing_DelInsert(spidp, gsip, gmis, nleft, nright, c, sap_exon, sap_continuous);

          /* update mismatch coordinates */
          offset = ntotal;
        }
      }
      else {
        ans = Ing_DoReplace(sap_exon, gmis, mmis, gstrand, mstrand, mismatches[j], &nleft, &nright);
        if (ans == ANS_YES){
          SeqLocReplace(msip, mmis, mmis, mstrand, gsip, gmis, gmis, gstrand);
          ntotal = 1;
        }
      }
/*       j += (ntotal - 1); */
      
      if (ans == ANS_YES){
        nkeep = nmismatches - ntotal;
        keep = (Int4Ptr)MemNew(sizeof(Int4)*nkeep);
        mismatch = mismatches[j];
        for (k = 0, m = 0; k<nmismatches; k++){
          if (!(mismatches[k] >= (mismatch - nleft) &&
                mismatches[k] <= (mismatch + nright))){
            if (mismatches[k] > (mismatch + nright))
              keep[m] = mismatches[k] - offset;
            else
              keep[m] = mismatches[k];
            m++;
          }
        }
        if (m != nkeep) /* problem! */
          return FALSE;
        
        MemFree(epp_cur->mismatches);
        epp_cur->mismatches = keep;
        epp_cur->nummismatches = nkeep;
      }
      return TRUE;
    }
  }
  return FALSE;
}

static void Ing_ReplaceMismatched(IngReportPtr Report, Uint2 segID, Uint2 primID)
{
  IngGenomeViewerPtr igvp;
  SPI_mRNAPtr        spidp;
  Int4               i, c;
  Int4               nmismatches;
  SeqAlignPtr        sap_exon = NULL;
  SeqAlignPtr        sap_continuous = NULL;
  SPI_ExonProfPtr    epp=NULL, epp_cur=NULL;
 


  igvp = Report->igvp;
  spidp = (SPI_mRNAPtr)igvp->data;
  /* get to the right spidey alignment */
  if (segID > 1){
    for (i= 0; i<segID - 1; i++)
      spidp = spidp->next;
  }

  epp = spidp->epp;

  sap_continuous = (SeqAlignPtr) spidp->continuous->segs;
  for(i=0; i<spidp->numexons; i++){
    if (spidp->strand == Seq_strand_minus)
      c = spidp->numexons - i - 1;
    else
      c = i;
    sap_exon = spidp->saps[c];
    epp_cur=Ing_NumMismatches(epp, c+1, &nmismatches);
    if (nmismatches == 0 || epp_cur == NULL)
      continue;
    if (Ing_ReplaceByExon(spidp, c, sap_exon, sap_continuous, epp_cur, nmismatches, primID)){
      ObjMgrSetDirtyFlag(igvp->entityID, TRUE);
      ObjMgrSendMsg(OM_MSG_UPDATE, igvp->entityID, 0, 0);
      return;
    }
    primID -= nmismatches;
  }
  
}


static void Ing_ReportClickProc(VieweR v, SegmenT s, PoinT pt)
{
  PrimitivE prim = NULL;
  Uint2     primID = 0, segID = 0;
  IngReportPtr  Report = NULL;

  Report = (IngReportPtr)GetObjectExtra(ParentWindow(v));

  if (FindSegPrim(v, pt, NULL, NULL, &prim)){ 
    FindSegment(v, pt, &segID, &primID, NULL);
    if (!primID && dblClick) {
      Ing_PrintColorCode();
    }
    else if (primID && dblClick && segID){
      Ing_ReplaceMismatched(Report, segID, primID);
/*       Ing_PrintColorCode();  */
    }
  }
}

extern void Ing_PopulateReport(IngReportPtr Report, IngGenomeViewerPtr igvp)
{
  ValNodePtr       vnp = NULL;
  CharPtr PNTR     names = NULL;
  CharPtr          temp = NULL;
  IngSeqAnnotData  sad;
  SeqEntryPtr      sep;
  SeqAlignPtr      sap = NULL;
  SPI_mRNAPtr      spidp = NULL;
  SPI_mRNAPtr      spidp_head = NULL;
  SeqIdPtr         sip=NULL;
  BioseqPtr        bsp_genomic=NULL;
  Int4             winsize;
  RecT             rc;
  Int4             row=0, margin;
  Int4             segID=1;
  Int4             i=0;
  Int4             index = 0;
  SegmenT          CurrentSeg;
  Int4             nchars;
  SelStructPtr     ssp=NULL;
  Boolean          bFirst = TRUE;


  WatchCursor();
  ssp = ObjMgrGetSelected();

  Reset(Report->Viewer);
  DeletePicture(Report->Picture);
  Update();
  
  Report->Picture=CreatePicture();
  margin=3*stdCharWidth;
  GetPosition(Report->Viewer, &rc);
  winsize=rc.right-rc.left-margin;
  SetSmallFont();
  nchars=winsize/stdCharWidth;

  vnp = igvp->talign.showlist;
  while (vnp){
    index++;
    vnp = vnp->next;
  }
  vnp = igvp->talign.showlist;
  names = igvp->talign.namelist;

  if (Report->vert_positions)
    MemFree(Report->vert_positions);
  Report->vert_positions=(Int4Ptr)MemNew(sizeof(Int4)*index+1);
    Report->vert_positions[0]=INT4_MAX;

  if (!vnp){
    AddAttribute(Report->Picture, COLOR_ATT, RED_COLOR, 0, 0, 0, 0);
    AddLabel (Report->Picture, 0, 0, " -- No alignments in data --", SMALL_TEXT, 0, MIDDLE_CENTER, 0);
    goto end;
  }
  AddAttribute(Report->Picture, COLOR_ATT, BLACK_COLOR, 0, 0, 0, 0);

  segID=1;
  i=0;
  if (igvp->data){
    spidp_head=(SPI_mRNAPtr)igvp->data;
  }
  bsp_genomic=GetBioseqGivenIDs (igvp->entityID, igvp->itemID, OBJ_BIOSEQ);
  
  while (vnp){
    sap=(SeqAlignPtr)vnp->data.ptrvalue;
    if (!sap) goto next;
    CurrentSeg=CreateSegment(Report->Picture, segID, 0);
    if (row){
      Report->vert_positions[i+1]=(row+10)*(-15);
    }
    else 
      Report->vert_positions[i]=0;
    if (StringCmp(names[i], "Spidey")==0){
      if (igvp->isExons)
        sap = AlnMgr2GetParent(sap);
      
      spidp = Ing_FindSpidpGivenParent(spidp_head, sap, igvp->isExons);
    }
    if ((StringCmp(names[i], "Spidey")==0) && spidp != NULL){
      if (spidp && !bFirst){
        Ing_DrawLongLine(Report->Picture, row, nchars);
        row++;
      }
      else
        bFirst = FALSE;
      row=Ing_PrintSpideyReport(Report, CurrentSeg, spidp, bsp_genomic, row, winsize, nchars, Report->bSummary);
      row++;
      i++;
      segID++;
      spidp = spidp->next;
    }
    else {
      if (sap && !bFirst){
        Ing_DrawLongLine(Report->Picture, row, nchars);
        row++;
      }
      else
        bFirst = FALSE;
      row = Ing_PrintBlastReport(Report, CurrentSeg, sap, bsp_genomic, row, winsize, nchars, names[i], Report->bSummary);
      row++;
      i++;
      segID++;
    }
  next:
    vnp=vnp->next;
  }
  
  
  Ing_DrawLongLine(Report->Picture, row-1, nchars);
 end:
  AttachPicture (Report->Viewer, Report->Picture, INT4_MIN, Report->vert_positions[Report->last_segID], MIDDLE_CENTER, 1 , 1, NULL);
  SetViewerProcs(Report->Viewer,Ing_ReportClickProc,NULL,NULL, NULL);
  if (ssp && Report->isPrevSelected && Report->last_segID){
    Ing_HighlightReportWindow(Report, FRAME_SEGMENT, Report->last_segID, FALSE);
  }
  ArrowCursor();
}


/*******************************************************************************

  Function : Report window functions
  
  Purpose : create spidey report window

*******************************************************************************/

static void Ing_FreeReport(IngReportPtr r)
{
  if (r->Viewer)DeleteViewer(r->Viewer);
  if (r->Picture)DeleteSegment(r->Picture);
}

static void Ing_CloseReportWin(WindoW w)
{
  IngReportPtr r;

  r=GetObjectExtra(w);
  r->igvp->hReport=NULL;
  if (r) Ing_FreeReport(r);
  r=NULL;
  Remove(w);
}

static void Ing_CloseReport(IteM i)
{
  IngReportPtr r;
  WindoW      w;

  w=ParentWindow(i);
  r=GetObjectExtra(w);
  r->igvp->hReport=NULL;
  if (r) Ing_FreeReport(r);
  r=NULL;
  Remove(w);
}


static void Ing_ResizeReportWindow(IngReportPtr Report)
{
  RecT     rcW, rcR, rcVsb, rcHsb;
  Int2     height,width,gap,vsbWidth;
  Int2     hsbWidth,in;
  BaR      vsb;
  BaR      hsb;


  if (!Report) return;
  ObjectRect(Report->hMain, &rcW);
  width= rcW.right-rcW.left;
  height= rcW.bottom-rcW.top;
  SafeHide(Report->Viewer);
  Update();

  vsb = GetSlateVScrollBar ((SlatE) Report->Viewer);
  hsb = GetSlateHScrollBar ((SlatE) Report->Viewer);
  GetPosition(Report->Viewer, &rcR);
  GetPosition(vsb, &rcVsb);
  GetPosition(vsb, &rcHsb);
  vsbWidth=rcVsb.right-rcVsb.left;
  hsbWidth=rcHsb.right-rcHsb.left;

  gap=10;
  in=2;

  rcR.right=rcR.left+width-in-vsbWidth;
  rcR.bottom=rcR.top+height-in-gap-hsbWidth;
  SetPosition(Report->Viewer, &rcR);

  Ing_PopulateReport(Report, Report->igvp);

  SafeShow(Report->Viewer);
  Update();
}

static void Ing_ResizeReportWin(WindoW w)
{
  IngReportPtr Report=NULL;


  Report=(IngReportPtr)GetObjectExtra(w);
  Ing_ResizeReportWindow(Report);
}

static void Ing_Summary(IteM i)
{
  IngReportPtr Report;

  Report=(IngReportPtr)GetObjectExtra(ParentWindow(i));
  if (!Report->bSummary){
    Report->bSummary=TRUE;
    Ing_PopulateReport(Report, Report->igvp);
  }
}

static void Ing_FullReport(IteM i)
{
  IngReportPtr Report;

  Report=(IngReportPtr)GetObjectExtra(ParentWindow(i));
  if (Report->bSummary){
    Report->bSummary=FALSE;
    Ing_PopulateReport(Report, Report->igvp);
  }
}

static void Ing_CreatePostScript(IteM i)
{
}

static void Ing_SaveReport(IteM i)
{
}


extern void Ing_ReportWindow(IteM i)
{
  IngGenomeViewerPtr igvp;
  WindoW             hReport;
  IngReportPtr       Report;
  MenU               m1;
  Int2               left_margin;
  Char               str[250]={""};

  left_margin=5*stdCharWidth;
  igvp=GetObjectExtra(ParentWindow(i));
  if (!igvp) return;
  if (igvp->hReport){
    Select(igvp->hReport);
    return;
  }
  sprintf(str, "%s", "Spidey Report -- ");
  StringCat(str, igvp->title);
  hReport = DocumentWindow(left_margin,(Int2) -65 ,(Int2) 550, (Int2) 250, str, Ing_CloseReportWin, Ing_ResizeReportWin);
  m1 = PulldownMenu (hReport, "File");
  CommandItem(m1, "Summary Version", Ing_Summary);
  CommandItem(m1, "Full Version", Ing_FullReport);
  SeparatorItem(m1);
  CommandItem(m1, "Print", Ing_CreatePostScript);
  CommandItem(m1, "Save", Ing_SaveReport);
  SeparatorItem(m1);
  CommandItem(m1, "Close Window", Ing_CloseReport);

  Report=Ing_ReportNew();
  Report->hMain=hReport;
  igvp->hReport=hReport;
  Report->igvp=igvp;
  Report->Viewer=CreateViewer(hReport, 500 ,200, TRUE, TRUE);	
  Report->Picture=CreatePicture();
  SetObjectExtra(hReport, (Pointer)Report, StdCleanupExtraProc);
  RealizeWindow(hReport);
  Report->bSummary=FALSE;
  Ing_ResizeReportWindow(Report);
  Show(hReport);
  Update();
}


/*******************************************************************************

  Function : Highlight Report window Functions
  
  Purpose : when the main window is clicked, highlight the spidey text output in the report window.

*******************************************************************************/
typedef struct ing_selreport{
  VieweR viewer;
  Int1 highlight;
  Uint2 segID;
} IngSelReport, PNTR IngSelReportPtr;


static Boolean Ing_SelReport(SegmenT seg, PrimitivE prim, Uint2 segID, Uint2 primID, Uint2 primCt, VoidPtr userdata)
{
  IngSelReportPtr spp;

  spp=(IngSelReportPtr)userdata;

  if (spp->segID==segID){
    HighlightSegment(spp->viewer, seg, spp->highlight);
    return FALSE;
  }
  return TRUE;

}

static Boolean Ing_DeSelReport(SegmenT seg, PrimitivE prim, Uint2 segID, Uint2 primID, Uint2 primCt, VoidPtr userdata)
{
  IngSelReportPtr spp;
  
  spp=(IngSelReportPtr)userdata;
    /* deselect all segments in report window */
  if (spp->segID==0){
    HighlightSegment(spp->viewer, seg, spp->highlight);
  }
  return TRUE;
}

extern void Ing_HighlightReportWindow(IngReportPtr Report, Int1 highlight, Uint2 segID, Boolean rePopulate)
{
  IngSelReport sp;

  Report->last_segID=segID;
  if (rePopulate)
    Ing_PopulateReport(Report, Report->igvp); 
  else
    AttachPicture (Report->Viewer, Report->Picture, INT4_MIN, Report->vert_positions[Report->last_segID], MIDDLE_CENTER, 1 , 1, NULL);
  SetViewerProcs(Report->Viewer,Ing_ReportClickProc,NULL,NULL, NULL);

  sp.highlight=highlight;
  sp.segID=segID;
  sp.viewer=Report->Viewer;
  if (segID == 0 && highlight == PLAIN_SEGMENT)
    ExploreSegment(Report->Picture, (Pointer)&sp, Ing_DeSelReport);
  else 
    ExploreSegment(Report->Picture, (Pointer)&sp, Ing_SelReport);
}


/*******************************************************************************

  Function : Spidey Form functions
  
  Purpose : create spidey input form and get mRNA sequences

*******************************************************************************/

extern Boolean Ing_DoesAlignmentCoverAll(Pointer data)
{
  SPI_mRNAPtr spidp = NULL;

  if (!data) return FALSE;
  spidp = (SPI_mRNAPtr) data;
  return (spidp->holes);
}


extern void Ing_CreateSpideyForm(IngGenomeViewerPtr igvp, BioseqPtr bsp1, CharPtr path, CharPtr GI_list)
{
  IngSpideyFormPtr   sfp;
  ButtoN            b;
  ButtoN            b1;
  GrouP             maingroup, submitg;
  GrouP             g, g2, g3, g4, g5;
  GrouP             leng;
  WindoW            w;
  Int4              start, stop;
  Char              lenbuf[40]={""}, szbuf[40]={""};

   if (!(sfp = (IngSpideyFormPtr)MemNew(sizeof(IngSpideyForm)))) return;

   sfp->bsp1 = bsp1;
   if (!StringHasNoText(path)){
     sfp->path=StringSave(path);
     sfp->GI_list=NULL;
   }
   else{
     sfp->GI_list=StringSave(GI_list);
     sfp->path=NULL;
   }
   w = MovableModalWindow(-50, -33 ,-10, -10, "Spidey", NULL);

   maingroup = HiddenGroup(w, 1, 3, NULL);  


   MemSet((Pointer)lenbuf, '\0', sizeof(lenbuf));
   sprintf(lenbuf, "Query. length=%d", bsp1->length);
   
   MemSet((Pointer)szbuf, '\0', sizeof(szbuf));
   SeqIdWrite(bsp1->id, szbuf, PRINTID_FASTA_SHORT,40);

   leng = NormalGroup(maingroup,1, 2, lenbuf, NULL, NULL);
   SetGroupMargins(leng, 10, 10);
   SetGroupSpacing(leng, 10, 20);
 
   StaticPrompt(leng, szbuf,0,0,programFont,'l');

   g5=HiddenGroup(leng, 4, 1, NULL);
   StaticPrompt(g5, "from:", 0, 0, programFont, 'l');
   if (igvp->slp){
     start = SeqLocStart(igvp->slp);
     stop = SeqLocStop(igvp->slp);
   }
   else {
     start = 1;
     stop = bsp1->length;
   }

   sprintf(szbuf, "%d", start);
   sfp->from = DialogText(g5, szbuf, 5, (TxtActnProc) NULL);
   StaticPrompt(g5, "  to:", 0, 0, programFont, 'l');
   sprintf(szbuf, "%d", stop);
   sfp->to = DialogText(g5, szbuf, 5, (TxtActnProc) NULL); 
   
   g=HiddenGroup(maingroup, 1, 4, NULL);
   g2 = NormalGroup(g, 1,1, "Spidey Options...",  programFont,NULL);
   g3=HiddenGroup(g2, 0, 2, NULL);
   sfp->inters = CheckBox(g3, "Interspecies", NULL);
   SetStatus(sfp->inters, FALSE);

   g2 = NormalGroup(g, 1,1, "Organism...",  programFont,NULL);
   sfp->org = PopupList (g2, TRUE, NULL);
   SetObjectExtra (sfp->org, sfp, NULL);
   PopupItem (sfp->org, "Vertebrate");
   PopupItem (sfp->org, "Fly");
   PopupItem (sfp->org, "Plant");
   PopupItem (sfp->org, "C. elegans");
   SetValue (sfp->org, 1);

   g3 = NormalGroup(g, 1,1, "Parameters...",  programFont,NULL);
   g4 = HiddenGroup(g3, 2, 5, NULL);
   StaticPrompt(g4, "Min % identity", 0, 0, programFont, 'l');
   sfp->idcutoff = DialogText(g4, "0", 5, NULL);
   StaticPrompt(g4, "Min % coverage", 0, 0, programFont, 'l');
   sfp->lencutoff = DialogText(g4, "0", 5, NULL);
   StaticPrompt(g4, "1st. pass eval", 0, 0, programFont, 'l');
   sfp->firstpasseval = DialogText(g4, "0.0000001", 5, NULL);
   StaticPrompt(g4, "2nd. pass eval", 0, 0, programFont, 'l');
   sfp->secondpasseval = DialogText(g4, "0.001", 5, NULL);
   StaticPrompt(g4, "3rd. pass eval", 0, 0, programFont, 'l');
   sfp->thirdpasseval = DialogText(g4, "1", 5, NULL);
   StaticPrompt(g4, "Num returns", 0, 0, programFont, 'l');
   sfp->numreturns = DialogText(g4, "1", 5, NULL);
   submitg=HiddenGroup(maingroup, 2, 0, NULL);
   b = DefaultButton(submitg, "Accept", Ing_DoSpidey);
   Enable(b);
   b1 = PushButton(submitg, "Cancel", Ing_StdCancelProc);
   SetObjectExtra(b1, (Pointer)sfp, NULL);
   SetObjectExtra(b, (Pointer)sfp, StdCleanupExtraProc);
   SetObjectExtra(w, (Pointer)igvp, NULL);
   Select(w);
   Show(w);

   return;

}

extern void Ing_GetFileForSpidey(IteM i)
{
  IngGenomeViewerPtr igvp;

  igvp=(IngGenomeViewerPtr)GetObjectExtra(ParentWindow(i));
  igvp->filetype=Ing_SPIDEY;
  Ing_OpenFromFileORNetwork(igvp);
  
}


/*******************************************************************************

  Function : Ing_OpenUDV()
  
  Purpose : Open UDV - sequence viewer

*******************************************************************************/
extern void Ing_OpenUDV(IteM i)
{
   ObjMgrPtr       omp;
   OMProcControl   ompc;
   ObjMgrProcPtr   ompp;
   IngGenomeViewerPtr igvp;
   Int2            ret;

   igvp=(IngGenomeViewerPtr)GetObjectExtra(ParentWindow(i));
   if (!igvp) return;

   omp = ObjMgrReadLock();
   ompp = ObjMgrProcFind(omp, 0, "OneD-Viewer", 0); 
   ObjMgrUnlock();
   WatchCursor();

   if (ompp != NULL)
   {
     ompc.proc = ompp;
     ompc.input_entityID = igvp->entityID;
     ompc.input_itemtype = OBJ_BIOSEQ;
     ompc.input_itemID = igvp->itemID;
     ompc.input_data = (Pointer)igvp->bsp;
     ret = (*(ompp->func))((Pointer)&ompc);
     switch(ret)
       {
       case OM_MSG_RET_ERROR:
         ErrShow();
         break;
       default:
         break;
       }
   }
   ArrowCursor();
   return;
}

