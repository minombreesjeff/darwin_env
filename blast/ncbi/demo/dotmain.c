/* dotmain.c 
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
* File Name:  dotmain.c
*
* Author:  Fasika Aklilu
*
* Version Creation Date:   8/8/01
*
* $Revision: 6.6 $
*
* File Description:  contains main function for dot matrix viewer

* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* ==========================================================================
*/

#include <dotviewer.h>
#include <accid1.h>
#include <lsqfetch.h>

/****************************************************************************

       GLOBAL VARS                                                                  
 ***************************************************************************/
#define NUMARGS  (sizeof(myargs)/sizeof(myargs[0]))

static Args myargs[] = {
    /* 0 */
    {"Sequence1 --  filename",NULL,NULL,NULL,TRUE,'q',ARG_FILE_IN,0.0,0,NULL},
    /* 1*/
    {"Sequence2 -- filename",NULL, NULL,NULL,TRUE,'s',ARG_FILE_IN,0.0,0,NULL},
    /* 2 */
    {"Orientation -- 1(+/+), 2(+/-)","1", NULL,NULL,TRUE,'o',ARG_INT,0.0,0,NULL},
    /* 3 */
    {"Start -- on sequence1","0", NULL,NULL,TRUE,'a',ARG_INT,0.0,0,NULL},
    /* 4 */
    {"Stop -- on sequence1","5", NULL,NULL,TRUE,'b',ARG_INT,0.0,0,NULL},
    /* 5 */
    {"Start -- on sequence2","0", NULL,NULL,TRUE,'c',ARG_INT,0.0,0,NULL},
    /* 6 */
    {"Stop -- on sequence2", "5", NULL,NULL,TRUE,'d',ARG_INT,0.0,0,NULL},
    /* 7 */
    { "Word size -- nuc[4 - 11], prot[1 or 2], default 8",  "8", NULL, NULL, TRUE, 'w', ARG_INT, 0.0, 0, NULL},
    /*8 */
    { "Hit size",  "100000", NULL, NULL, TRUE, 'k', ARG_INT, 0.0, 0, NULL},
    /* 9*/
    {"Alignment as seqannot -- filename",NULL, NULL,NULL,TRUE,'l',ARG_FILE_IN,0.0,0,NULL},
    /* 10 */
    {"Sequence1 -- GI or Accession",NULL,NULL,NULL,TRUE,'i',ARG_STRING,0.0,0,NULL},
    /* 11 */
    {"Sequence2 -- GI or Accession",NULL, NULL,NULL,TRUE,'j',ARG_STRING,0.0,0,NULL},
    /* 12 */
    {"Program -- Dotmatrix=1 Blast2seqs=2","1", NULL,NULL,TRUE,'p',ARG_INT,0.0,0,NULL},

};

typedef struct dotstartdata{
  WindoW  hStart;
  ButtoN  dot;
  ButtoN  reg_blast;
  ButtoN  auto_blast;
  ButtoN  GorL;
  ButtoN  mask_repeats;
  ButtoN  mask_simple;
  ButtoN  megablast;
  ButtoN  seqa;
  ButtoN  entrez;
  ButtoN  file;
  GrouP  plusorminus;
  GrouP  gapped;
  TexT    id1, id2;
  TexT    hitsize;
  TexT    wordsize;
  TexT    eval;
  TexT    querystart;
  TexT    querystop;
  TexT    subjectstart;
  TexT    subjectstop;
  TexT    progname;
} DOTStartData, PNTR DOTStartDataPtr;

static FILE        *qfile=NULL, *sfile=NULL, *afile=NULL;
static Int4        q_start=0,q_stop=5,s_start=0, s_stop=5, tree_limit=10, word_size=11;
static CharPtr     id1=NULL, id2=NULL;
static Boolean     Bool_dotplot=TRUE;
static char *date_of_compilation = __DATE__;
static char *time_of_compilation = __TIME__;



static  void DOT_quitProgram(ButtoN b)
{
  QuitProgram();
}


static BioseqPtr DOT_GetBspFromGIOrAcc(CharPtr str)
{
   BioseqPtr  bsp;
   Int4       gi;
   Char       ptr;
   SeqIdPtr   sip;
   ValNode    vn;
   LinkSetPtr lsp;
   Int4       uid;

   str = TrimSpacesAroundString(str);
   ptr = *str;
   if (IS_ALPHA(ptr))  /* accession */
   {
      sip = SeqIdFromAccessionDotVersion(str);   
      bsp = BioseqLockById(sip);
   } else  /* it's a GI */
   {
      gi = atoi(str);
      vn.choice = SEQID_GI;
      vn.data.intvalue = gi;
      vn.next = NULL;
      bsp = BioseqLockById(&vn);
   }
   return bsp;
}

static BioseqPtr DOT_FetchBioseqGivenString(CharPtr id, Int2Ptr rtn)
{
  BioseqPtr bsp = NULL;

   bsp = DOT_GetBspFromGIOrAcc(id);
   if (bsp == NULL)
     *rtn = -1;
   else
     *rtn = 1;

   return bsp;
}

static void DOT_AlignTwoSeqsAndDisplay(BioseqPtr bsp1, BioseqPtr bsp2, SeqLocPtr slp1, SeqLocPtr slp2, Int4 word_size, Int4 tree_limit, CharPtr program)
{
   SCP_ResultPtr        scp=NULL;

   scp=SCP_CompareOrderOrganizeBioseqs(bsp1, bsp2, slp1, slp2, program, word_size, tree_limit);
   if (scp == NULL){
     Message (MSG_OK, "Blast 2 Seqs - return NULL");
     DOT_quitProgram(NULL);
     return;
   }
   DOT_AlignPlotGivenScp(scp);
}


static void DOT_CenterString (RectPtr rptr, CharPtr text, FonT fnt, Int2 inc)

{
  if (fnt != NULL) {
    SelectFont (fnt);
  }
  rptr->bottom = rptr->top + LineHeight ();
  DrawString (rptr, text, 'c', FALSE);
  rptr->top = rptr->bottom + inc;
}

static FloatHi DOT_get_eval(Int4 exp)
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

static SeqAlignPtr DOT_RunBlast2Seqs(BioseqPtr bsp1, BioseqPtr bsp2, SeqLocPtr slp1, SeqLocPtr slp2, DOTStartDataPtr sdata)
{
  BLAST_OptionsBlkPtr options;
  Char                eval[10];
  Char                progname[10];
  Int4                e;
  Int2                i;
  SeqAlignPtr         sap;
  Uint2               entityID;


  GetTitle(sdata->progname, progname, 10);
  options = BLASTOptionNew(progname, TRUE);
  
  i = GetValue(sdata->gapped);
  if (i == 1)
    options->gapped_calculation = TRUE;
  else {
    BLASTOptionDelete(options);
    options = BLASTOptionNew(progname, FALSE);
    options->gapped_calculation = FALSE;
  }

  GetTitle(sdata->eval, eval, 14);
  if (eval != NULL)
    {
      e = atoi(eval);
      options->expect_value = DOT_get_eval(e);
    }

  GetTitle(sdata->wordsize, eval, 5);
  if (eval != NULL)
    options->wordsize = atoi(eval);
  if (GetStatus(sdata->mask_repeats) == TRUE)
    {
      if (GetStatus(sdata->mask_simple) == TRUE)
        options->filter_string = StringSave("m L;R");
      else
        options->filter_string = StringSave("m R");
    } else if (GetStatus(sdata->mask_simple) == TRUE)
      options->filter_string = StringSave("m L");

  if (GetStatus(sdata->megablast))
    options->is_megablast_search=TRUE;

  if (slp1 == NULL || slp2 == NULL)
    sap = BlastTwoSequences(bsp1, bsp2, progname, options);
  else
    sap = BlastTwoSequencesByLoc(slp1, slp2, progname, options);
  
  BLASTOptionDelete(options);

  if (sap == NULL)
    return NULL;
    
  entityID = ObjMgrRegister(OBJ_SEQALIGN, (Pointer)sap);

  sap->saip = NULL;
  AlnMgrIndexLite(sap);
  AssignIDsInEntity(entityID, OBJ_SEQALIGN, (Pointer)sap);

  return sap;
}

static void DOT_RunRequest(WindoW w, BioseqPtr qbsp, BioseqPtr sbsp, DOTStartDataPtr sdata){
  Char            str1[100]={""}, str2[100]={""};
  SeqLocPtr       slp1=NULL, slp2=NULL;
  SeqAlignPtr     sap = NULL;
  DOTMainDataPtr  mip;
  DOTAlignInfoPtr alp=NULL;
  SCP_ResultPtr    scp=NULL;


  MemSet((Pointer)str1, '\0', sizeof(str1));
  GetTitle (sdata->querystart,  str1,  sizeof(str1));
  if (StringCmp(str1, "All")==0)
     q_start=0; 
  else
    q_start=DOT_GetValue(sdata->querystart);
  MemSet((Pointer)str1, '\0', sizeof(str1));
  GetTitle(sdata->querystop, str1, sizeof(str1));
  if (StringCmp(str1, "All")==0)
    q_stop=qbsp->length-1;
  else
      q_stop=DOT_GetValue(sdata->querystop);
  MemSet((Pointer)str1, '\0', sizeof(str1));
  GetTitle(sdata->subjectstart, str1, sizeof(str1));
  if (StringCmp(str1, "All")==0)
    s_start=0;
  else
    s_start=DOT_GetValue(sdata->subjectstart);
  MemSet((Pointer)str1, '\0', sizeof(str1));
  GetTitle(sdata->subjectstop, str1, sizeof(str1));
  if (StringCmp(str1, "All")==0)
    s_stop=sbsp->length-1;
  else
    s_stop=DOT_GetValue(sdata->subjectstop);
  if (qbsp->length<q_start || sbsp->length<s_start || q_start>q_stop|| s_start>s_stop || s_start<0 || q_start<0)
    {
      ErrPostEx(SEV_FATAL, 1, 0, "Bad sequence offset values");
      return;
    }
  
  if (GetStatus(sdata->reg_blast)){
    if (GetValue(sdata->plusorminus) == 1){
      slp1 = NULL;
      slp2 = NULL;
    }
    else {
      slp1= SeqLocIntNew(q_start, q_stop, 1, qbsp->id);
      if (GetValue(sdata->plusorminus) == 2)
        slp2 = SeqLocIntNew(s_start, s_stop, 1, sbsp->id);
      else
        slp2 = SeqLocIntNew(s_start, s_stop,2, sbsp->id);
    }
  }
  else if (GetStatus(sdata->auto_blast)){
    slp1 = SeqLocIntNew(q_start, q_stop, 1, qbsp->id);
    slp2 = SeqLocIntNew(s_start, s_stop,1, sbsp->id);
  }
  else if (GetStatus(sdata->dot)){
    slp1= SeqLocIntNew(q_start, q_stop, 1, qbsp->id);
    if (GetValue(sdata->plusorminus) == 1)
      slp2 = SeqLocIntNew(s_start, s_stop,1, sbsp->id);
    else
      slp2 = SeqLocIntNew(s_start, s_stop, 2, sbsp->id);
  }
  
  if (GetStatus(sdata->reg_blast)){
    sap = DOT_RunBlast2Seqs(qbsp, sbsp, slp1, slp2, sdata);
    if (sap == NULL){
      Message (MSG_OK, "Blast 2 Seqs -  returned NULL");
      DOT_quitProgram(NULL);
      return;
    }
    DOT_AlignPlotGivenSeqAlign(sap);
  }
  else if (GetStatus(sdata->dot) || GetStatus(sdata->auto_blast)){
    
    word_size=DOT_GetValue(sdata->wordsize);
    tree_limit=DOT_GetValue(sdata->hitsize);
    
    if (GetStatus(sdata->dot)){
      mip=DOT_CreateAndStorebyLoc(slp1, slp2, word_size, tree_limit);
      if (mip == NULL){
        Message (MSG_OK, "Dot Plot analysis returned NULL");
        DOT_quitProgram(NULL);
        return;
      }
      Remove(sdata->hStart);
      Remove(w);
      DOT_MakeMainViewer(mip, alp);
    }
    else if (GetStatus(sdata->auto_blast)){
      scp=SCP_CompareOrderOrganizeBioseqs(qbsp, sbsp, slp1, slp2, "blastn", word_size, tree_limit);
      if (scp == NULL){
        Message (MSG_OK, "Blast 2 Seqs - return NULL");
        DOT_quitProgram(NULL);
        return;
      }
      DOT_AlignPlotGivenScp(scp);
    } 
  } 
}

static void DOT_OpenAcceptProc(ButtoN b)
{
  DOTStartDataPtr sdata;
  WindoW          w;
  Char            str1[100]={""}, str2[100]={""};
  BioseqPtr       qbsp=NULL, sbsp=NULL;
  SeqEntryPtr     qsep=NULL, ssep=NULL;
  Uint2           entityID, entityID1, entityID2;
  SeqLocPtr       slp1=NULL, slp2=NULL;
  Pointer         dataptr=NULL;
  Uint2           datatype=0;
  SeqAlignPtr     sap=NULL;
  SeqAnnotPtr     sanp=NULL;
  DOTAlignInfoPtr alp;

  w=ParentWindow(b);  
  Hide(w);
  sdata=(DOTStartDataPtr)GetObjectExtra(w);
  GetTitle(sdata->id1, str1, sizeof(str1));
  if (!GetStatus(sdata->seqa))
    GetTitle(sdata->id2, str2, sizeof(str2));
  if (StringHasNoText (str1)) {
    if (!GetStatus(sdata->seqa) && StringHasNoText(str2)){
      Message (MSG_ERROR, "invalid filename");
      Show (w);
      Select (w);
      Select (sdata->id1);
      return;
    }
  }
  else{
      qfile = FileOpen(str1, "r");
      if (!qfile){
        Message (MSG_ERROR, "invalid filename");
        Show (w);
        Select (w);
        Select (sdata->id1);
        return;
      }
      if (GetStatus(sdata->seqa)){
        dataptr = ReadAsnFastaOrFlatFile (qfile, &datatype, &entityID, FALSE, TRUE, TRUE, TRUE);
        if (!dataptr || datatype != OBJ_SEQANNOT) goto seqa_end;
        sanp = (SeqAnnotPtr)(dataptr);
        sap = (SeqAlignPtr)(sanp->data);
        AlnMgrIndexLite(sap);
        AssignIDsInEntity(entityID, OBJ_SEQALIGN, (Pointer)sap);

        fclose(afile);
        alp=DOT_AlignInfoNew();
        alp->sap=sap;
        if (!DOT_FillAlignInfoPointer(alp)){
          goto seqa_end;
        }
        DOT_MakeMainViewer(NULL, alp);
      seqa_end:
        fclose(qfile);
        return;
      }
      sfile = FileOpen(str2, "r");
      if (!sfile){
        fclose(qfile);
        Message (MSG_ERROR, "invalid filename");
        Show (w);
        Select (w);
        Select (sdata->id1);
        return;
      }
   while ((dataptr = ReadAsnFastaOrFlatFile (qfile, &datatype, &entityID1, FALSE, FALSE, TRUE, FALSE)) != NULL)
     {
       qsep= GetTopSeqEntryForEntityID(entityID1);
       entityID1 = SeqMgrIndexFeatures(0, qsep);
       qbsp = qsep->data.ptrvalue;
     }
   
   while ((dataptr = ReadAsnFastaOrFlatFile (sfile, &datatype, &entityID2, FALSE, FALSE, TRUE, FALSE)) != NULL)
     {
       ssep= GetTopSeqEntryForEntityID(entityID2);
       entityID2 = SeqMgrIndexFeatures(0, ssep);
       sbsp = ssep->data.ptrvalue;

     }
    if (qbsp== NULL || sbsp== NULL)
    {
        Message (MSG_ERROR, "Can't read sequences");
        Show (w);
        Select (w);
        Select (sdata->id1);
        goto file_end;
    }
   if (qbsp->mol != sbsp->mol)
    {
      Message (MSG_ERROR, "Mismatched molecule types");
      Show (w);
      Select (w);
      Select (sdata->id1);
      goto file_end;
    }
   
   DOT_RunRequest(w, qbsp, sbsp, sdata);
  file_end:
      if (qfile)fclose(qfile);
      if (sfile)fclose(sfile);
      return;
  }
}
  
static void DOT_RespondtoFetch(Int2 rtn, WindoW w, DOTStartDataPtr sdata)
{
  if (rtn==-2){
    Message (MSG_ERROR, "Network is unavailable -- load local files");
  }
  else if (rtn==-1){
    Message (MSG_ERROR, "bad GI or Accession -- try again?");
  }
  
  Show (w);
  Select (w);
  Select (sdata->id1);
  return;
}

static void DOT_AcceptNetworkProc(ButtoN b)
{
  DOTStartDataPtr sdata;
  WindoW          w;
  Char            str1[50]={""}, str2[50]={""};
  BioseqPtr       qbsp=NULL, sbsp=NULL;
  SeqEntryPtr     qsep, ssep;
  Uint2           entityID1, entityID2;
  Int2            rtn=0;
  DOTMainDataPtr  mip;
  SeqLocPtr       slp1=NULL, slp2=NULL;

  w=ParentWindow(b);  
  Hide(w);
  sdata=(DOTStartDataPtr)GetObjectExtra(w);
  GetTitle(sdata->id1, str1, sizeof(str1));
  GetTitle(sdata->id2, str2, sizeof(str2));
  if (StringHasNoText (str1) || StringHasNoText(str2)) {
    Message (MSG_ERROR, "enter GI or Accession number");
    Show (w);
    Select (w);
    Select (sdata->id1);
    return;
  }

  qbsp = DOT_FetchBioseqGivenString(str1, &rtn);
  sbsp = DOT_FetchBioseqGivenString(str2, &rtn);

  if (rtn <0 || !qbsp || !sbsp){
    DOT_RespondtoFetch(rtn, w, sdata);
    return;
  }
  
  DOT_RunRequest(w, qbsp, sbsp, sdata);
  
}

static void DOT_CancelProc(ButtoN b)
{
  DOTStartDataPtr sdata;
  sdata=(DOTStartDataPtr)GetObjectExtra(ParentWindow(b));
  Remove(ParentWindow(b));
  Show(sdata->hStart);
}

static void DOT_SetupDotParameters(GrouP g, DOTStartDataPtr sdata, Boolean is_dot)
{
  GrouP g1, g2, g3, gtotal;

  if (is_dot == TRUE){
    gtotal = HiddenGroup(g, 1, 3, NULL);
    
    sdata->plusorminus = NormalGroup(gtotal, 2,1, "",  systemFont,NULL);
    RadioButton(sdata->plusorminus, "plus-plus");
    RadioButton(sdata->plusorminus, "plus-minus");
    SetValue(sdata->plusorminus, 1);
  }
  else
    gtotal = HiddenGroup(g, 1, 2, NULL);

  g2 = NormalGroup(gtotal, 4, 2, "",  systemFont,NULL);
  StaticPrompt(g2, "query start -- ", 0, 0, systemFont, 'l');
  sdata->querystart = DialogText(g2, "All", 5, NULL);
  StaticPrompt(g2, "query stop -- ", 0, 0, systemFont, 'l');
  sdata->querystop = DialogText(g2, "All", 5, NULL);
  StaticPrompt(g2, "subject start -- ", 0, 0, systemFont, 'l');
  sdata->subjectstart = DialogText(g2, "All", 5, NULL);
  StaticPrompt(g2, "subject stop -- ", 0, 0, systemFont, 'l');
  sdata->subjectstop = DialogText(g2, "All", 5, NULL);

  g3 = NormalGroup(gtotal, 2, 2, "",  systemFont,NULL);
  StaticPrompt(g3, "hitsize -- ", 0, 0, systemFont, 'l');
  sdata->hitsize = DialogText(g3, "100000", 5, NULL);
  StaticPrompt(g3, "wordsize -- ", 0, 0, systemFont, 'l');
  sdata->wordsize = DialogText(g3, "11", 5, NULL);
}

static void DOT_MegaBlast2Defaults(ButtoN b)
{
  DOTStartDataPtr sdata;

  sdata=(DOTStartDataPtr )GetObjectExtra(b);

  if (GetStatus(b)){
    SetValue(sdata->gapped, 1);
    SetTitle(sdata->wordsize, "28");
  }
  else {
    SetTitle(sdata->wordsize, "11");
  }
}

static void DOT_SetupBlastParameters(GrouP g, DOTStartDataPtr sdata)
{
  GrouP g0, g1, g2, g3, g4, g5, gtotal;
  GrouP g_sub1, g_sub2;

  gtotal = HiddenGroup(g, 1, 6, NULL);

  sdata->plusorminus = NormalGroup(gtotal, 2,1, "",  systemFont,NULL);
  RadioButton(sdata->plusorminus, "both");
  RadioButton(sdata->plusorminus, "plus-plus");
  RadioButton(sdata->plusorminus, "plus-minus");
  SetValue(sdata->plusorminus, 1);
  
  g0 = HiddenGroup(gtotal, 2, 1, NULL);
  StaticPrompt(g0, "Program name: ", 0, 0, systemFont, 'l');
  sdata->progname = DialogText(g0, "blastn", 7, NULL);


  g1 = NormalGroup(gtotal, 4, 2, "",  systemFont,NULL);
  StaticPrompt(g1, "query start -- ", 0, 0, systemFont, 'l');
  sdata->querystart = DialogText(g1, "All", 5, NULL);
  StaticPrompt(g1, "query stop -- ", 0, 0, systemFont, 'l');
  sdata->querystop = DialogText(g1, "All", 5, NULL);
  StaticPrompt(g1, "subject start -- ", 0, 0, systemFont, 'l');
  sdata->subjectstart = DialogText(g1, "All", 5, NULL);
  StaticPrompt(g1, "subject stop -- ", 0, 0, systemFont, 'l');
  sdata->subjectstop = DialogText(g1, "All", 5, NULL);

  g3 = NormalGroup(gtotal, 1, 3, "Local Alignment Options", programFont, NULL);
  g_sub1 = NormalGroup(g3, 1,1, "",  programFont,NULL);
  g_sub2 = HiddenGroup(g_sub1, 3, 2, NULL);
  sdata->gapped = HiddenGroup(g_sub2, 0, 2, NULL);
  RadioButton(sdata->gapped, "gapped");
  RadioButton(sdata->gapped, "ungapped");
  SetValue(sdata->gapped, 1);
  
  g4 = NormalGroup(gtotal, 1,1, "Mask Regions",  programFont,NULL);
  g_sub1=HiddenGroup(g4, 0, 2, NULL);
  sdata->mask_repeats= CheckBox(g_sub1, "Mask Repeats", NULL);
  SetStatus(sdata->mask_repeats, FALSE);
  sdata->mask_simple = CheckBox(g_sub1, "Mask Simple Sequence", NULL);
  SetStatus(sdata->mask_simple, TRUE);
  
  sdata->megablast= CheckBox(gtotal, "Mega Blast", DOT_MegaBlast2Defaults);
  SetObjectExtra(sdata->megablast, (Pointer)sdata, NULL);
  SetStatus(sdata->megablast, FALSE);

  g5 = NormalGroup(gtotal, 1,1, "",  programFont,NULL);
  g_sub1 = HiddenGroup(g5, 2, 2, NULL);
  StaticPrompt(g_sub1, "E-value:  e-", 0, 0, programFont, 'l');
  sdata->eval = DialogText(g_sub1, "1", 5, NULL);
  StaticPrompt(g_sub1, "wordsize:", 0, 0, programFont, 'l');
  sdata->wordsize = DialogText(g_sub1, "11", 5, NULL);

}


static void DOT_GetFromNetwork(ButtoN b)
{
  WindoW  w;
  GrouP   g, c;
  Char    szAppName  [10]={"D-Viewer"};
  DOTStartDataPtr sdata;

  sdata=(DOTStartDataPtr)GetObjectExtra(ParentWindow(b));
  if (!sdata) return;
  Hide(sdata->hStart);
  w = MovableModalWindow (-50, -33, -10, -10, "Download From NCBI", NULL);
  SetObjectExtra(w, (Pointer)sdata, NULL);
  if (w==NULL) return;
  SetGroupSpacing (w, 10, 10);
  /*accesion*/
  g = NormalGroup (w, 1, 4, "Entrez GI or Accession", systemFont,NULL);
  sdata->id1=DialogText (g, "", 10, NULL);
  sdata->id2=DialogText (g, "", 10, NULL);
  SetObjectExtra(w, (Pointer)sdata, NULL);
  if (GetStatus(sdata->dot))
    DOT_SetupDotParameters(g, sdata, TRUE);
  else if (GetStatus(sdata->auto_blast))
    DOT_SetupDotParameters(g, sdata, FALSE);
  else if (GetStatus(sdata->reg_blast))
    DOT_SetupBlastParameters(g, sdata);
  c = HiddenGroup (g, 4, 0, NULL);
  SetGroupMargins(c, 10, 10);
  SetGroupSpacing (c, 10, 20);
  DefaultButton (c, "Retrieve", DOT_AcceptNetworkProc);
  PushButton (c, "Cancel", DOT_CancelProc);
  AlignObjects (ALIGN_CENTER, (HANDLE) g, (HANDLE) c, NULL);
  RealizeWindow (w);
  Show (w);
  Select (w);
  Update ();
}

static void DOT_OpenBrowse1Proc(ButtoN b)
{
  DOTStartDataPtr sdata=NULL;
  Char   path[PATH_MAX]={""};

  sdata = (DOTStartDataPtr) GetObjectExtra (b);
  
  if (sdata==NULL) return;
  
  if (GetInputFileName (path, sizeof(path), NULL, NULL)){ 
    SetTitle(sdata->id1, path);
  }
}

static void DOT_OpenBrowse2Proc(ButtoN b)
{
  DOTStartDataPtr sdata=NULL;
  Char   path[PATH_MAX]={""};

  sdata = (DOTStartDataPtr) GetObjectExtra (b);
  
  if (sdata==NULL) return;
  
  if (GetInputFileName (path, sizeof(path), NULL, NULL)){ 
    SetTitle(sdata->id2, path);
  }
}

extern void DOT_readExisting(ButtoN b)
{
  WindoW  w;
  GrouP   g, g2, c;
  Char    szAppName  [10]={"D-Viewer"};
  DOTStartDataPtr sdata;

  sdata=(DOTStartDataPtr)GetObjectExtra(ParentWindow(b));
  if (!sdata) return;
  Hide(sdata->hStart);
  w = MovableModalWindow (-50, -33, -10, -10, "Local File", NULL);
  SetObjectExtra(w, (Pointer)sdata, NULL);
 if (w==NULL) return;
  g = NormalGroup(w, 1, 3, "File name:",  systemFont, NULL);
  SetGroupMargins(g, 10, 10);
  SetGroupSpacing(g, 10, 20);  
  g2=HiddenGroup(g, 2, 2, NULL);
  sdata->id1 = DialogText(g2,"",25, NULL);
  b = PushButton(g2, " browse...", DOT_OpenBrowse1Proc);
  SetObjectExtra (b, (Pointer) sdata, NULL);
  if (!GetStatus(sdata->seqa)){
    sdata->id2 = DialogText(g2,"",25, NULL);
    b = PushButton(g2, " browse...", DOT_OpenBrowse2Proc);
    SetObjectExtra (b, (Pointer) sdata, NULL);
    if (GetStatus(sdata->dot))
      DOT_SetupDotParameters(g, sdata, TRUE);
    else if (GetStatus(sdata->auto_blast))
      DOT_SetupDotParameters(g, sdata, FALSE);
    else 
      DOT_SetupBlastParameters(g, sdata);
  }
  c = HiddenGroup (g, 4, 0, NULL);
  DefaultButton(c, "OK",  DOT_OpenAcceptProc);
  PushButton(c, "Cancel",  DOT_CancelProc);
  Select(w);
  Show(w);
}

static void DOT_ButtontoNetwork(ButtoN b){
  
  Remove(ParentWindow(b));
 /*  if (!(igvp=IG_NewMainDataPtr())) */
/*     return; */
/*   IG_GetFromNetwork(igvp); */
}

static void DOT_DrawAbout (PaneL p)

{
  RecT  r;
  Char  str [60];
  FonT  titleFont=NULL;


  if (titleFont == NULL) {
#ifdef WIN_MAC
    titleFont = GetFont ("Geneva", 18, TRUE, TRUE, FALSE, "");
#endif
#ifdef WIN_MSWIN
    titleFont = GetFont ("Arial", 24, TRUE, TRUE, FALSE, "");
#endif
#ifdef WIN_MOTIF
    titleFont = GetFont ("Courier", 24, TRUE, TRUE, FALSE, "");
#endif
  }

  sprintf (str, "Beta 1.05 [%s - %s]", date_of_compilation, time_of_compilation);

  ObjectRect (p, &r);
  InsetRect (&r, 4, 4);
  r.top += 10;
  Magenta ();
  DOT_CenterString (&r, " ~ D-Viewer ~", titleFont, 5);
/*   DOT_CenterString (&r, str, programFont, 10); */

  /*
  CenterString (&r, "National Center for Biotechnology Information", systemFont, 5);
  CenterString (&r, "National Library of Medicine", systemFont, 5);
  CenterString (&r, "National Institutes of Health", systemFont, 10);
  */
}


static void DOT_CheckDot(ButtoN b)
{
  DOTStartDataPtr sdata;
  
  sdata=(DOTStartDataPtr)GetObjectExtra(ParentWindow(b));
  if (!sdata) return;
  if(GetStatus(sdata->dot)){
    Enable(sdata->entrez);
    Enable(sdata->file);
    SetStatus(sdata->reg_blast, FALSE);
    SetStatus(sdata->auto_blast, FALSE);
    SetStatus(sdata->seqa, FALSE);
  }
  else{
    Disable(sdata->entrez);
    Disable(sdata->file);
  }
}
static void DOT_CheckRegBlast(ButtoN b)
{
  DOTStartDataPtr sdata;
  
  sdata=(DOTStartDataPtr)GetObjectExtra(ParentWindow(b));
  if (!sdata) return;
  if(GetStatus(sdata->reg_blast)){
    Enable(sdata->entrez);
    Enable(sdata->file);
    SetStatus(sdata->dot, FALSE);
    SetStatus(sdata->seqa, FALSE);
    SetStatus(sdata->auto_blast, FALSE);
  }
  else{
    Disable(sdata->entrez);
    Disable(sdata->file);
  }
}

static void DOT_CheckAutoBlast(ButtoN b)
{
  DOTStartDataPtr sdata;
  
  sdata=(DOTStartDataPtr)GetObjectExtra(ParentWindow(b));
  if (!sdata) return;
  if(GetStatus(sdata->auto_blast)){
    Enable(sdata->entrez);
    Enable(sdata->file);
    SetStatus(sdata->dot, FALSE);
    SetStatus(sdata->seqa, FALSE);
    SetStatus(sdata->reg_blast, FALSE);
  }
  else{
    Disable(sdata->entrez);
    Disable(sdata->file);
  }
}

static void DOT_CheckSA(ButtoN b)
{
  DOTStartDataPtr sdata;
  
  sdata=(DOTStartDataPtr)GetObjectExtra(ParentWindow(b));
  if (!sdata) return;
  if(GetStatus(sdata->seqa)){
    Disable(sdata->entrez);
    Enable(sdata->file);
    SetStatus(sdata->dot, FALSE);
    SetStatus(sdata->reg_blast, FALSE);
    SetStatus(sdata->auto_blast, FALSE);
  }
  else{
    Disable(sdata->file);
  }
}
static void DOT_StartupWindow(void)
{

  WindoW  w;
  GrouP   c, cg;
  DOTStartDataPtr sdata;
  PaneL   p;

  w = FixedWindow (-50, -33, -10, -10, "Welcome to D-Viewer", StdCloseWindowProc);

  p = SimplePanel (w, 28 * stdCharWidth, 4 * stdLineHeight, DOT_DrawAbout);
  sdata=(DOTStartDataPtr)MemNew(sizeof(DOTStartData));
  sdata->hStart=w;
  cg=HiddenGroup(w, 4, 0, NULL);
  sdata->dot=CheckBox(cg, "Dot plot", DOT_CheckDot);
  SetStatus(sdata->dot, TRUE);
  
  sdata->reg_blast=CheckBox(cg, "Bl2Seqs", DOT_CheckRegBlast);
  SetStatus(sdata->reg_blast, FALSE);
  sdata->auto_blast = CheckBox(cg, "Scoop", DOT_CheckAutoBlast);
  SetStatus(sdata->auto_blast, FALSE);

  sdata->seqa=CheckBox(cg, "SeqAnnote File", DOT_CheckSA);
  SetStatus(sdata->seqa, FALSE);
  c = HiddenGroup (w, 1, 3, NULL);
  SetGroupMargins(c, 10, 10);
  SetGroupSpacing (c, 10, 10);
  sdata->entrez=PushButton (c, "Fetch from Entrez", DOT_GetFromNetwork);
  Enable(sdata->entrez);
  sdata->file=PushButton (c, "Open Local Files", DOT_readExisting);
  Enable(sdata->file);
  PushButton (c, "Quit Program", DOT_quitProgram);
    
  SetObjectExtra(w, (Pointer)sdata, StdCleanupExtraProc);
  AlignObjects (ALIGN_CENTER, (HANDLE) p, (HANDLE) c, NULL,  NULL);
  Select(w);
  Show(w);
  ProcessEvents();
}


Int2 Main ()
{

  Int2        orient;
  CharPtr     txt=NULL;
  Boolean     Bool=TRUE;

  ErrSetFatalLevel (SEV_MAX);
  ErrClearOptFlags (EO_SHOW_USERSTR);
  UseLocalAsnloadDataAndErrMsg ();
  ErrPathReset ();

  if (! AllObjLoad ()) { 
    Message (MSG_FATAL, "AllObjLoad failed"); 
    return -1; 
  } 
  if (! SubmitAsnLoad ()) {
    Message (MSG_FATAL, "SubmitAsnLoad failed");
    return 1;
  }
  if (! FeatDefSetLoad ()) {
    Message (MSG_FATAL, "FeatDefSetLoad failed");
    return 1;
  }
  if (! SeqCodeSetLoad ()) {
    Message (MSG_FATAL, "SeqCodeSetLoad failed");
    return 1;
  }
  if (! GeneticCodeTableLoad ()) {
    Message (MSG_FATAL, "GeneticCodeTableLoad failed");
    return 1;
  }

  if (!ID1BioseqFetchEnable("DotMatrix", FALSE))
    {
      ErrPostEx(SEV_FATAL, 1, 0, "Bad sequence offset values");
      return -1;
    }
  LocalSeqFetchInit (FALSE);

   
  DOT_StartupWindow();
  
  LocalSeqFetchDisable ();
  ID1BioseqFetchDisable();

}
