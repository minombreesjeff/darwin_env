/*   ingenmain.c
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
* File Name:  ingenmain.c
*
* Author:  Fasika Aklilu
*
* Version Creation Date:   8/8/01
*
* $Revision: 6.3 $
*
* File Description:  contains main function for ingenue
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* ==========================================================================
*/

#include <ingenwin.h>
#include <accid1.h>
#include <lsqfetch.h>



static char *date_of_compilation = __DATE__;
static char *time_of_compilation = __TIME__;





static void Ing_fromNetwork(ButtoN b)
{
  IngGenomeViewerPtr igvp;
  WindoW            hStart;
  
  hStart=ParentWindow(b);
  Hide(hStart);
  if (!(igvp=Ing_MainDataNew()))
    return;
/*   igvp->hProcess=Ing_ProcessingWindow(); */
  Ing_GetFromNetwork(igvp, hStart);
}


static void Ing_CenterString (RectPtr rptr, CharPtr text, FonT fnt, Int2 inc)

{
  if (fnt != NULL) {
    SelectFont (fnt);
  }
  rptr->bottom = rptr->top + LineHeight ();
  DrawString (rptr, text, 'c', FALSE);
  rptr->top = rptr->bottom + inc;
}



static void Ing_DrawAbout (PaneL p)

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
  Ing_CenterString (&r, " ~ Ingenue ~", titleFont, 5);
/*   Ing_CenterString (&r, str, programFont, 10); */
 /*  Ing_CenterString (&r, "National Center for Biotechnology Information", systemFont, 5); */
/*   Ing_CenterString (&r, "National Library of Medicine", systemFont, 5); */
/*   Ing_CenterString (&r, "National Institutes of Health", systemFont, 10); */
  
}

static  void Ing_quitProgram(ButtoN b)
{
  QuitProgram();
}


static void Ing_readExisting(ButtoN b)
{
  IngGenomeViewerPtr igvp=NULL;
  WindoW            hStart=NULL;
  
  hStart=ParentWindow(b);
  Hide(hStart);
  if (!(igvp=Ing_MainDataNew()))
    return;
/*   igvp->hProcess=Ing_ProcessingWindow(); */
  Ing_FileOpenProc(igvp, hStart); 
}

extern void Ing_StartupWindow(void)
{

  WindoW  hStart;
  GrouP   c;
  PaneL           p;

  hStart = FixedWindow (-50, -33l, -10, -10, "Welcome to INGENUE", StdCloseWindowProc);

  p = SimplePanel (hStart, 28 * stdCharWidth, 4 * stdLineHeight, Ing_DrawAbout);

  c = HiddenGroup (hStart, 1, 3, NULL);
  SetGroupMargins(c, 10, 10);
  SetGroupSpacing (c, 10, 10); 
 PushButton (c, "Fetch from Entrez", Ing_fromNetwork);
  PushButton (c, "Open Local File", Ing_readExisting);
  PushButton (c, "Quit Program", Ing_quitProgram);
    
  AlignObjects (ALIGN_CENTER, (HANDLE) p, (HANDLE) c, NULL,  NULL);
  Select(hStart);
  Show(hStart);
  ProcessEvents();
}

static void Ing_SetupRegFuncs()
{
  FeatDefPtr  curr;
  Uint1       key;
  CharPtr     label = NULL;
  Uint2       subtype;
  Char        proclabel [64];
  Char        procname [64];


 curr = FeatDefFindNext (NULL, &key, &label, FEATDEF_ANY, TRUE);
  while (curr != NULL) {
    if (key != FEATDEF_BAD && curr->seqfeat_key == SEQFEAT_IMP) {
      subtype = curr->featdef_key;
      if (subtype != FEATDEF_source) {
        sprintf (procname, "Edit %s", curr->menulabel);
        StringNCpy_0 (proclabel, curr->typelabel, sizeof (proclabel));
        if (proclabel [0] == '-') {
          proclabel [0] = '~';
        }
        REGISTER_IMPORT_EDIT(procname,proclabel,subtype);
      }
    }
    curr = FeatDefFindNext (curr, &key, &label, FEATDEF_ANY, TRUE);
  }

  curr = FeatDefFindNext (NULL, &key, &label, FEATDEF_ANY, TRUE);
  while (curr != NULL) {
    if (key != FEATDEF_BAD && curr->seqfeat_key == SEQFEAT_RNA) {
      subtype = curr->featdef_key;
      sprintf (procname, "Edit %s", curr->menulabel);
      StringNCpy_0 (proclabel, curr->typelabel, sizeof (proclabel));
      if (proclabel [0] == '-') {
        proclabel [0] = '~';
      }
      REGISTER_RNA_EDIT(procname,proclabel,subtype);
    }
    curr = FeatDefFindNext (curr, &key, &label, FEATDEF_ANY, TRUE);
  }

  curr = FeatDefFindNext (NULL, &key, &label, FEATDEF_ANY, TRUE);
  while (curr != NULL) {
    if (key != FEATDEF_BAD && curr->seqfeat_key == SEQFEAT_PROT) {
      subtype = curr->featdef_key;
      sprintf (procname, "Edit %s", curr->menulabel);
      StringNCpy_0 (proclabel, curr->typelabel, sizeof (proclabel));
      if (proclabel [0] == '-') {
        proclabel [0] = '~';
      }
      REGISTER_PROT_EDIT(procname,proclabel,subtype);
    }
    curr = FeatDefFindNext (curr, &key, &label, FEATDEF_ANY, TRUE);
  }

  REGISTER_GENE_EDIT;
  REGISTER_CDRGN_EDIT;
  REGISTER_RGNFEAT_EDIT;
  REGISTER_CMNTFEAT_EDIT;
  REGISTER_MOLINFO_EDIT;
  REGISTER_TITLE_EDIT;
  REGISTER_COMMENT_EDIT;
  REGISTER_NAME_EDIT;
  REGISTER_REGION_EDIT;
  REGISTER_BOND_EDIT;
  REGISTER_SITE_EDIT;
  REGISTER_PSEC_EDIT;
  REGISTER_GENBANK_EDIT;
  REGISTER_CREATE_DATE_EDIT;
  REGISTER_UPDATE_DATE_EDIT; 
  REGISTER_UDV_AUTONOMOUS; 
  REGISTER_NEW_BIOSEQ_EDIT;
  REGISTER_NEW_SEQALIGN_EDIT;
  REGISTER_NEW_SEQANNOT_EDIT;
  REGISTER_SUBMITBLOCK_EDIT;
  REG_INGENUE;
}

static CharPtr Ing_GetGIlist(CharPtr PNTR argv, Int4Ptr argcnt, Int4 argc)
{
  CharPtr GI_list=NULL;
  Int4    i;

  i=*argcnt;
  GI_list=StringSave(argv[i]); 
  i++;

  for (; i<argc && (StringStr(argv[i], "-") == NULL); i++){
    GI_list=Ing_StringCat(GI_list, " ");
    GI_list=Ing_StringCat(GI_list, argv[i]);
  }
  *argcnt=i;
  return GI_list;
}

Int2 Main()
{
  Char              szAppName[50]={"INGENUE"};
  Boolean           Bool=TRUE;
  WindoW            w;
  CharPtr           filename=NULL;
  CharPtr           accorgi=NULL;
  Int4              argbegin=0, argend=0;
  CharPtr           spi_filename=NULL, spi_seqidfile=NULL;
  CharPtr           spi_gilist=NULL;
  Int4              spi_job=0;
  CharPtr           b2s_filename=NULL, b2s_seqidfile=NULL;
  CharPtr           b2s_gilist=NULL;
  Int4              b2s_job=0;
  ErrSev           oldErrSev;


  oldErrSev = ErrSetMessageLevel(SEV_MAX);
  ErrSetFatalLevel (SEV_MAX);
  ErrClearOptFlags (EO_SHOW_USERSTR);
  UseLocalAsnloadDataAndErrMsg ();
  ErrPathReset ();

#ifdef WIN_MOTIF
  {{
    Int4         argc = GetArgc();
    CharPtr PNTR argv = GetArgv();
    Int4         i;
    Boolean      found=FALSE;
    
    for (i = 1;  i < argc;  i++)
      {
        if (StringCmp (argv[i], "-f") == 0){
          i++;
          filename=StringSave(argv[i]);
          found=TRUE;
        }
        else if (StringCmp (argv[i], "-gi") == 0){
          i++;
          accorgi=StringSave(argv[i]);
          found=TRUE;
        }
        else if (StringCmp (argv[i], "-from") == 0 && found){
          i++;
          argbegin=atoi(argv[i]);
        }
        else if (StringCmp (argv[i], "-to") == 0){
          i++;
          argend=atoi(argv[i]);
        }
        else if (StringCmp (argv[i], "-spi_gi")==0){
          i++;
          spi_job=Ing_SPIDEY;
          spi_gilist=Ing_GetGIlist(argv, &i, argc);
        }
        else if (StringCmp(argv[i], "-spi_fgi")==0){
          i++;
          spi_job=Ing_SEQIDLISTFORSPIDEY;
          spi_seqidfile=StringSave(argv[i]);
        } 
        else if (StringCmp(argv[i], "-spi_f")== 0){
          i++;
          spi_job=Ing_SPIDEY;
          spi_filename=StringSave(argv[i]);
        }
        else if (StringCmp(argv[i], "-b2s_gi")==0){
          i++;
          b2s_job=ALIGN_BLAST2SEQ;
          b2s_gilist=Ing_GetGIlist(argv, &i, argc);
        }
        else if (StringCmp(argv[i], "-b2s_fgi")==0){
          i++;
          b2s_job=Ing_SEQIDLISTFORBLAST;
          b2s_seqidfile=StringSave(argv[i]);
        }
        else if (StringCmp(argv[i], "-b2s_f")== 0){
          i++;
          b2s_job=ALIGN_BLAST2SEQ;
          b2s_filename=StringSave(argv[i]);
        }
      }
    
  }}
#endif

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

  Ing_SetupRegFuncs();

  if (!ID1BioseqFetchEnable(szAppName, FALSE))
    {
      ErrPostEx(SEV_FATAL, 1, 0, "Bad sequence offset values");
      return -1;
    }
  LocalSeqFetchInit (FALSE);

  if (accorgi)
    Ing_NetOpenFromCommandline(accorgi, argbegin, argend, spi_job, spi_filename, spi_gilist, spi_seqidfile, b2s_job, b2s_filename, b2s_gilist, b2s_seqidfile);
  else if (filename)
    Ing_FileOpenFromCommandline(filename, argbegin, argend, spi_job, spi_filename, spi_gilist, spi_seqidfile, b2s_job, b2s_filename, b2s_gilist, b2s_seqidfile);
  else
    Ing_StartupWindow();
 end:

  LocalSeqFetchDisable ();
  ID1BioseqFetchDisable();

  /* ingenue check sockets */
/*   Metronome(IngenueCheckSocketsProc); */

  ErrSetMessageLevel(oldErrSev);

  if (accorgi) MemFree(accorgi);
  else if (filename) 
    MemFree(filename);

  if (b2s_filename)
    MemFree(b2s_filename);
  if (b2s_gilist)
    MemFree(b2s_gilist);
  if (b2s_seqidfile)
    MemFree(b2s_seqidfile);
  if (spi_filename)
    MemFree(spi_filename);
  if (spi_gilist)
    MemFree(spi_gilist);
  if (spi_seqidfile)
    MemFree(spi_seqidfile);
  return 0;
}






