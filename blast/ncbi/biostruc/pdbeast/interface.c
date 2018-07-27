/*===========================================================================
*
*                            PUBLIC DOMAIN NOTICE
*               National Center for Biotechnology Information
*
*  This software/database is a "United States Government Work" under the
*  terms of the United States Copyright Act.  It was written as part of
*  the author's official duties as a United States Government employee and
*  thus cannot be copyrighted.  This software/database is freely available
*  to the public for use. The National Library of Medicine and the U.S.
*  Government have not placed any restriction on its use or reproduction.
*
*  Although all reasonable efforts have been taken to ensure the accuracy
*  and reliability of the software and data, the NLM and the U.S.
*  Government do not and cannot warrant the performance or results that
*  may be obtained by using this software or data. The NLM and the U.S.
*  Government disclaim all warranties, express or implied, including
*  warranties of performance, merchantability or fitness for any particular
*  purpose.
*
*  Please cite the author in any work or product based on this material.
*
* ===========================================================================
*
* File Name:  interface.c
*
* Author:  Yanli Wang
*
* Initial Version Creation Date: 03/09/1997
*
* File Description:
*         An interactive tool for taxonomy assignment to structure sequence
*
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: interface.c,v $
* Revision 1.1  2002/12/17 15:13:43  ywang
* pdbeast related tools updated
*
*
* ==========================================================================
*/

#include <vibrant.h>
#include <document.h>
#include <ncbi.h>
#include <ncbistr.h>
#include <taxinc.h>
#include <objseq.h>
#include <objsset.h>
#include <objfeat.h>
#include <objblock.h>

#include "para.h"
#include "interface.h"

  
Args myargs[8] = {
    {"Text Input Seq-entry File Directory","/net/keystone/usr/people/ywang/work/taxonomy/RawBioseq/", NULL,NULL,TRUE,'h',ARG_STRING,0.0,0,NULL},
    {"Binary Input Seq-entry File Directory","/net/clobber/usr/people8/bryant/MMDB.Bioseq/", NULL,NULL,TRUE,'i',ARG_STRING,0.0,0,NULL},
    {"Text Output Seq-entry File Directory","/net/keystone/usr/people/ywang/work/taxonomy/Bioseq_text/", NULL,NULL,TRUE,'n',ARG_STRING,0.0,0,NULL},
    {"Binary Output Seq-entry File Directory","/net/keystone/usr/people/ywang/work/taxonomy/Bioseq_binary/", NULL,NULL,TRUE,'o',ARG_STRING,0.0,0,NULL},
    {"PDB File Directory","/usr/ncbi/db/pdb/Mirror/Current/", NULL,NULL,TRUE,'p',ARG_STRING,0.0,0,NULL},
    {"Entry Code List File Directory","/net/keystone/usr/people/ywang/work/taxonomy/index/", NULL,NULL,TRUE,'b',ARG_STRING,0.0,0,NULL},     
    {"List File Name","all.OCT96", NULL,NULL,TRUE,'c',ARG_FILE_IN,0.0,0,NULL},
    {"Log errors to file named:","log.pdbeast.April",NULL,NULL,TRUE,'l',ARG_FILE_OUT, 0.0,0,NULL}};          

extern Char EntryCode[120];
extern Char pdbsource_str[10240];
extern OrgRefPtr my_org;
extern OrgRefPtr my_org_chain[MaxChainNum];
extern Int4 nchain;
extern Char chain[MaxChainNum];
extern Boolean chain_assign[MaxChainNum];
extern Char chain_orgName[MaxChainNum][120];
extern Char CurrentOrgName[120];
extern Char selected_chain, last_click_chain;
extern Int2 selected_chain_num;
extern Boolean AllAssign, PartAssign;
extern Boolean SingleChain;
extern Boolean TextInEntry, TextOutEntry;
extern Boolean TaxAssign, EntryOpen;
extern Boolean SingleEntry, EntryFromList;
extern Boolean BioOrg_chain[MaxChainNum];

static ButtoN  accept;
static ButtoN  ListEntry_start;
static ButtoN  ListEntry_next;
static ButtoN  ListEntry_specific;
static ButtoN  Entry_accept;
       ButtoN  Tax_accept; 
       ButtoN  Tax_all_accept; 
       ButtoN  Tax_part_accept; 
       ButtoN  Tax_finish; 
       ButtoN  Tax_reject; 
       ButtoN  ReLookup_accept;
       ButtoN  ReLookup_reject;
       ButtoN  PdbOpen;
       ButtoN  PdbClose;
static ButtoN  Source_string_accept; 
static ButtoN  Pdbfile_string_accept; 
static GrouP   category;
static PaneL   drawing;
static GrouP   features;
static TexT    species;
static TexT    Entry_code;
static TexT    Source_string;
static TexT    Pdbfile_string;
static ButtoN  check[4];
static PoinT   lastPt;
static MenU    s1;
static MenU    s2;
static MenU    s3;
static LisT    l;
static GrouP   infilemode;
static GrouP   outfilemode;

static LisT    lBlastChain;
static ButtoN  BlastSearchAccept;

static InitProc();
static void QuitProc(IteM i);
static void EnableProc(GraphiC g);
static void AcceptProc(ButtoN b);
static void ListProc(LisT l);
static void ButtonProc(ButtoN b);
static void MenuProc(IteM i);
static void ListOpenProc(ChoicE c);
static void EntryProc(IteM i); 
static void EntryOpenProc(ButtoN b);
static void AddStringProc(ButtoN b);
static void ReLookupProc(ButtoN b);
static void TaxAcceptProc(ButtoN b);
static void SourceStringAcceptProc(ButtoN b);
static void PdbfileStringAcceptProc(ButtoN b);
static void ReLookupRejectProc(ButtoN b);
static void TaxAllAcceptProc(ButtoN b);
static void TaxPartAcceptProc(ButtoN b);
static void TaxPartAssignProc(LisT lchain);
static void TaxFinishProc(ButtoN b);
static void PdbOpenProc(ButtoN b);
static void PdbCloseProc(ButtoN b);
static void ListEntryStartProc(ButtoN b);
static void ListEntryNextProc(ButtoN b);
static void ListEntrySpecificProc(ButtoN b);
static void InFileTypeProc(GrouP infilemode);
static void OutFileTypeProc(GrouP outfilemode);
static void ListUpdateProc(ChoicE c);

static void ListBlastChainProc(LisT l);
static void BlastSearchAcceptProc(ButtoN b);

/*-------------- main starts here --------------------------*/
extern Int2 Main (void)
{

RecT r1, r2;

#ifdef WIN_MAC
    m = AppleMenu (NULL);
    DeskAccGroup(m);
    m = PulldownMenu(NULL,  "File");
  
    s = SubMenu(m, "Open");
    i = CommandItem(m, "Quit/Q", QuitProc);
#endif

    InitProc(GetArgc(), GetArgv());

    w = RoundWindow(-50, -33,  -10,  -10,  "PDBeast: Taxonomy Assignment",  NULL);

#ifndef WIN_MAC

    SetUpDrawingTools();
/*  Red();
    SetColor(5);  */
    InvertColors();

    wpdb = RoundWindow(-53, -33, -10,  -10, "PDB File Display", NULL);
    p = StaticPrompt(wpdb, "Pdb File", 0, dialogTextHeight,  systemFont,  '1');
    Advance(wpdb);
    PdbOpen = DefaultButton(wpdb, "Open", PdbOpenProc);
    Break(wpdb);

    p = StaticPrompt(wpdb, "Blast Search", 0, dialogTextHeight,  systemFont,  '1');
    Advance(wpdb);
    lBlastChain = SingleList(wpdb, 10, 3, ListBlastChainProc);
    Advance(wpdb);
    BlastSearchAccept = DefaultButton(wpdb, "Send Query", BlastSearchAcceptProc);
    Break(wpdb);
 
    p = StaticPrompt(wpdb, "String Search", 0, dialogTextHeight,  systemFont,  '1');
    Advance(wpdb);
    Pdbfile_string = DialogText(wpdb, "", 15, NULL);
    Advance(wpdb);
    Pdbfile_string_accept = DefaultButton(wpdb, "Find", PdbfileStringAcceptProc);
    Break(wpdb);

    pdbfile_panel = DocumentPanel(wpdb, 650, 20*stdLineHeight);
    Break(wpdb);
    p = StaticPrompt(wpdb,"                ", 0, dialogTextHeight,  systemFont,  '1');
    Advance(wpdb);
    PdbClose = PushButton(wpdb,"Close", PdbCloseProc);
    Break(wpdb);

    m = PulldownMenu(w, "File");
    s1 = SubMenu(m, "Open");
    i = CommandItem(s1, "Entry", EntryProc);
    s2 = SubMenu(s1, "List");
    c = ChoiceGroup(s2, ListOpenProc); 
    ChoiceItem(c, myargs[6].strvalue);
   
/*  s3 = SubMenu(m, "Update");
    c = ChoiceGroup(s3, ListUpdateProc);
    ChoiceItem(c, myargs[6].strvalue);    */
    
    i = CommandItem(m,  "Quit/Q",  QuitProc);

/*  edit = PulldownMenu(w, "Edit");
    i = CommandItem(edit, "cut", StdCutTextProc);
    i = CommandItem(edit, "copy", StdCopyTextProc);
    i = CommandItem(edit, "paste", StdPasteTextProc);   */
#endif

    infilemode = NormalGroup(w, 3, 0, "Data Mode of Input Entry",systemFont, InFileTypeProc);
    RadioButton(infilemode, "Text"); 
    RadioButton(infilemode, "Binary"); 
/*  SafeSetValue(infilemode, 2);  */
    GetPosition(infilemode, &r1);

    outfilemode = NormalGroup(w, 3, 0, "Data Mode of Output Entry",systemFont, OutFileTypeProc);
    RadioButton(outfilemode, "Text"); 
    RadioButton(outfilemode, "Binary"); 
/*  SafeSetValue(outfilemode, 2);  */
    r2.top = r1.top; r2.bottom = r1.bottom; 
    r2.left = r1.right + 80;
    r2. right = r2.left + r1.right - r1.left;
    SetPosition(outfilemode, &r2);      
    Break(w);

    p = StaticPrompt(w, "Seq-entry", 0, dialogTextHeight,  systemFont,  '1');
    Advance (w);
    l = SingleList(w, 20, 3, ListProc);
    Disable(l);
    Advance(w);
    ListEntry_start = DefaultButton(w, "Start", ListEntryStartProc);
    Disable(ListEntry_start);
    Advance(w);
    ListEntry_next = DefaultButton(w, "Next", ListEntryNextProc);
    Disable(ListEntry_next);
    Advance(w);
    ListEntry_specific = DefaultButton(w, "Specific", ListEntrySpecificProc);
    Disable(ListEntry_specific);
    Break(w);
 
    p = StaticPrompt(w, "Enter Entry Code", 0, dialogTextHeight,  systemFont,  '1');
    Advance (w);
    Entry_code = DialogText(w, "", 15, (TxtActnProc) EnableProc);
    Select(Entry_code);
    Advance (w);
    Entry_accept = DefaultButton(w, "Open", EntryOpenProc);    
    Disable(Entry_accept);
    Break(w);

    p = StaticPrompt(w, "Chain", 0, dialogTextHeight,  systemFont,  '1');
    chain_panel = DocumentPanel(w, 200, stdLineHeight);
    Break(w);
    p = StaticPrompt(w, "Pdb Source:", 0, dialogTextHeight,  systemFont,  '1');
    pdb_source = ScrollText(w, 38, 3, systemFont, TRUE, NULL);
    Advance(w);
    Break(w);
    p = StaticPrompt(w, "Taxonomy Data:", 0, dialogTextHeight,  systemFont,  '1');
    org = DocumentPanel(w,400,6*stdLineHeight);
    Break(w);
    
    p = StaticPrompt(w, "Taxonomy Accept?", 0, dialogTextHeight,  systemFont,  '1');
    Break(w);
    Tax_accept = DefaultButton(w, "Yes", TaxAcceptProc);
    Advance(w);
    p = StaticPrompt(w, "to    ", 0, dialogTextHeight,  systemFont,  '1');
    Advance(w);
    Tax_all_accept = DefaultButton(w, "All", TaxAllAcceptProc);
    Disable(Tax_all_accept);
    Advance(w);
    p = StaticPrompt(w, "         or          ", 0, dialogTextHeight,  systemFont,  '1');
    Advance(w);      
    Tax_part_accept = DefaultButton(w, "Chain", TaxPartAcceptProc);
    Disable(Tax_part_accept);
    Advance(w);
    lchain = SingleList(w, 2, 4, TaxPartAssignProc);
    Advance(w);
    Tax_finish = DefaultButton(w, "All Done", TaxFinishProc);
    Disable(Tax_finish);
    Break(w); 
    Tax_reject = DefaultButton(w, "No", ReLookupProc);
    Disable(Tax_accept);
    Disable(Tax_reject);
    Break(w);

    p = StaticPrompt(w, "Look up Taxonomy Database with another String?", 0, dialogTextHeight,  systemFont,  '1');
    Advance(w);
    ReLookup_accept = DefaultButton(w, "Yes", AddStringProc);
    Advance(w);
    ReLookup_reject = DefaultButton(w, "No", ReLookupRejectProc);
    Disable(ReLookup_accept);
    Disable(ReLookup_reject);
    Break(w);

    p = StaticPrompt(w,"Search Taxonomy Database with", 0, dialogTextHeight,  systemFont,  '1');
    Advance(w);
    Source_string = DialogText(w, "", 15, NULL);
    Advance(w);
    Source_string_accept = DefaultButton(w, "Start", SourceStringAcceptProc);
    Disable(Source_string_accept);
    Break(w);

    Show(w);
    Show(wpdb);
    ProcessEvents();

    return 0;
}
/*-------------------------------------------------*/
PrintArg(){

       printf("\t -aid      Text Input Seq-entry File Directory\n");
       printf("\t -bid      Binary Input Seq-entry File Directory\n");
       printf("\t -aod      Text Output Seq-entry File Directory\n");
       printf("\t -bod      Binary Output Seq-entry File Directory\n");
       printf("\t -PDBpath  PDB File Directory\n");
       printf("\t -ListPath Entry Code List File Directory\n");
       printf("\t -ListName Entry Code List File Name\n");
       printf("\t -Logfile  File Name For Log Errors\n");
       printf("\t -im       Input File Mode: 'a' for text / 'b' for binary \n");
       printf("\t -om       Output File Mode: 'a' for text /'b' for binary \n");

       return;
        
}
/*-------------- Initialize -----------------------*/
static InitProc(argc, argv)
int argc;
char *argv[];
{

    Char listname[120], Command[120], str[120];
    Char my_argu[120];

/*  if (!GetArgs("PDBeast", 8, myargs)) return 1;   */ /* turn off window args */

    TextInEntry = FALSE;
    TextOutEntry = FALSE;


    if(argc == 1){
        PrintArg();
        exit (1);
    }
    else{
        while(--argc > 0){
            *++argv;
            if(StringCmp(*argv, "-aid") == 0) {
                sscanf(*++argv, "%s", my_argu);
                myargs[0].strvalue = StringSave(my_argu);
            }
            else if (StringCmp(*argv, "-bid") == 0) {
                sscanf(*++argv, "%s", my_argu);
                myargs[1].strvalue = StringSave(my_argu);
            }
            else if (StringCmp(*argv, "-aod") == 0) {
                sscanf(*++argv, "%s", my_argu);
                myargs[2].strvalue = StringSave(my_argu);
            }
            else if (StringCmp(*argv, "-bod") == 0) {
                sscanf(*++argv, "%s", my_argu);
                myargs[3].strvalue = StringSave(my_argu);
            }
            else if (StringCmp(*argv, "-PDBpath") == 0) {
                sscanf(*++argv, "%s", my_argu);
                myargs[4].strvalue = StringSave(my_argu);
            }
            else if (StringCmp(*argv, "-ListPath") == 0) {
                sscanf(*++argv, "%s", my_argu);
                myargs[5].strvalue = StringSave(my_argu);
            }
            else if (StringCmp(*argv, "-ListName") == 0) {
                sscanf(*++argv, "%s", my_argu);
                myargs[6].strvalue = StringSave(my_argu);
            }
            else if (StringCmp(*argv, "-Logfile") == 0) {
                sscanf(*++argv, "%s", my_argu);
                myargs[7].strvalue = StringSave(my_argu);
            }
            else if (StringCmp(*argv, "-im") == 0) {
                sscanf(*++argv, "%s", str);
                if(StringCmp(str, "a") == 0) TextInEntry = TRUE;
                else if(StringCmp(str, "b") == 0) TextInEntry = FALSE;
            }
            else if (StringCmp(*argv, "-om") == 0) {
                sscanf(*++argv, "%s", str);
                if(StringCmp(str, "a") == 0) TextOutEntry = TRUE;
                else if(StringCmp(str, "b") == 0)TextOutEntry = FALSE;
            }
       }
    }


	printf("calling Init\n");
    Init();
	printf("returned from Init\n");

    log_pdbeast = FileOpen(myargs[7].strvalue, "w");

    StringCpy(listname, myargs[5].strvalue);
    StringCat(listname, myargs[6].strvalue);

/*  if(FileLength(listname) != 0){
        sprintf(Command, "cp %s %s.bak", listname, listname);
        system(Command);
    }   */
     
}
/*-------------------------------------------------*/
void DoListUpdate()
{

    Int4 n = 0;
    Char fullname[120], str[120];

    SetValue(c, 0);

    StringCpy(fullname, myargs[5].strvalue);
    StringCat(fullname, myargs[6].strvalue);
    seq_list = FileOpen(fullname, "r");

    if(seq_list == NULL) {
           Message(MSG_OK, "File %s can not be opened!", fullname);
           return;
    }

    entry_num = 0;
    while(feof(seq_list) == 0){

        if(fscanf(seq_list, "%s", str) == EOF) break;

        sscanf(str, "%4s", seq_entry[entry_num]);

        if(StringLen(EntryCode) != 0) {
            if(StringNCmp(seq_entry[entry_num], EntryCode, 4) != 0){
                sscanf(str + 4, "%c\n",  &seq_entry_TaxState[entry_num]);
            }
        }

        entry_num++;
    }


    FileClose(seq_list);

    seq_list = FileOpen(fullname, "w");

    if(seq_list == NULL) {
        Message(MSG_OK, "File %s can not be opened!", fullname);
        return;
    }

    n = 0;

    for(n = 0 ; n < entry_num; n++){
        fprintf(seq_list, "%4s%c\n", seq_entry[n], seq_entry_TaxState[n]);
    }

    FileClose(seq_list);
}
/*-------------------------------------------------*/
void CheckUpdate()
{

    MsgAnswer ans;

    if(EntryListOn){
          ans = Message(MSG_YN, "Do you want to update control list?");
          if(ans == ANS_YES){
              DoListUpdate(); 
              EntryListOn = FALSE;
              return;
          }

    }

    return;

}
/*------------------------------------------------------------------*/
static void QuitProc(IteM i)
{

/*  CheckUpdate();  */
 
    Close();

    FileClose(log_pdbeast);

    QuitProgram();
}
/*------------------------------------------------------------------*/
static void EnableProc(GraphiC g)
{
    Char str[32];
    GetTitle (species,  str,  sizeof(str));
    if (StringLen(str) == 0 || GetValue(category) == 0)
    {
	Disable (accept);
    }
    else
    {
	Enable (accept);
    }
}
/*------------------------------------------------------------------*/
static void ButtonProc(ButtoN b) {
}
/*------------------------------------------------------------------*/
static void ListProc(LisT l) {
        Int2 val;

        val = GetValue(l);
        if(val == 0) printf("Chose one entry from the list to start, please!\n");
     else {

            Enable(ListEntry_start);
            Enable(ListEntry_next);
            Enable(ListEntry_specific);
     }

}
/*-----------------------------------------------------------------*/
Int2 CheckProc(Int2 current_zone)
{
     MsgAnswer ans;

     if( !TaxAssign) {
             if(StringLen(EntryCode) != 0){
                     CloseCurrentEntry();
                     if(current_zone == 3) {
                            Message(MSG_OK, "Taxonomy has NOT been assigned for previous entry  ' %s ', You may do it again later! ", EntryCode);
                            fprintf(log_pdbeast, "Taxonomy has NOT been assigned for %s\n\n", EntryCode);
                            return(1);
                     }
                     else if(current_zone == 1) {
                            Message(MSG_OK, "Taxonomy has NOT been assigned forprevious entry  ' %s ', You may do it again later by using the Specific button! ", EntryCode);
                            fprintf(log_pdbeast, "Taxonomy has NOT been assigned for %s\n\n", EntryCode);
                            return(1);
}
                     else {
                             ans = Message(MSG_YN, "Taxonomy has NOT been assigned for current entry ' %s ', Do you want to skip and revise later?", EntryCode);
                             if (ans == ANS_YES) {
                                 seq_entry_TaxState[index_num[current_seq_entry_num]] = '?';
                                 if(EntryFromList) DoListUpdate();
                                 fprintf(log_pdbeast, "Taxonomy has NOT been assigned for %s\n\n", EntryCode);
                                 return(1);
                             }
                             else {
                                    Message(MSG_OK, " Click on Start ", EntryCode);
                                    redo = TRUE;

                                    return(-1);
                             }
                    }
             }
     }

     return;

}
/*------------------------------------------------------------------*/
static void ListOpenProc(ChoicE c)
{
   Int4 val,n;
   Int4 unassign_seq_num;

   Char fullname[120];
   Char str[120];

/* CheckUpdate();   */

   Disable(ListEntry_start);
   Disable(ListEntry_next);
   Disable(ListEntry_specific);

   Hide(Entry_code);
   Reset(Entry_code);
   Show(Entry_code);
   Disable(Entry_accept);

   current_seq_entry_num = 0;

   StringCpy(fullname, myargs[5].strvalue);
   StringCat(fullname, myargs[6].strvalue);
   
   seq_list = FileOpen(fullname, "r");
   if(seq_list == NULL){
       Message(MSG_OK, " Entry list file ' %s ' not found!", fullname);
       exit(1);
   }
   else{
          entry_num = 0;

          while(feof(seq_list) == 0){

              if(fscanf(seq_list, "%s", str) == EOF) break;
              sscanf(str, "%4s%c", seq_entry[entry_num], &seq_entry_TaxState[entry_num]);
 
              entry_num++;

          }      
   }

   FileClose(seq_list);

   Hide(l);
   Reset(l);
   Show(l);

   SetValue(l, 0);
   SetValue(c, 0);

   unassign_seq_num = 0;  

   for( n = 0; n <= entry_num; n++){
          if(seq_entry_TaxState[n] != '+' && seq_entry_TaxState[n] != '*'){
                  ListItem(l, seq_entry[n]);
                  index_num[unassign_seq_num] = n;
                  unassign_seq_num++;
          }
   }

   Enable(l);    

   EntryListOn = TRUE;

   Message(MSG_OK, "Chose one entry from the current list and click button Start please!");


}
/*------------------------------------------------------------------*/
static void ListUpdateProc(ChoicE c)
{
    
    DoListUpdate();

}
/*------------------------------------------------------------------*/
static void EntryProc(IteM i)
{

/*  CheckUpdate();   */

    Hide(l);
    Reset(l);
    Show(l);
    Disable(ListEntry_start);
    Disable(ListEntry_next);
    Disable(ListEntry_specific);

    Hide(Entry_code);
    Reset(Entry_code);
    Show(Entry_code);
    Enable(Entry_accept);

}
/*-----------------------------------------------------------------*/
static void My_Reset1()
{

    TaxAssign = FALSE;
    EntryOpen = FALSE;

    Hide(chain_panel);
    Reset(chain_panel);
    Show(chain_panel);

    Hide(pdb_source);
    Reset(pdb_source);
    Show(pdb_source);

    Hide(org);
    Reset(org);
    Show(org);

    Hide(Source_string);
    Reset(Source_string);
    Show(Source_string);

    Hide(lchain);
    Reset(lchain);
    Show(lchain);

    Hide(pdbfile_panel);
    Reset(pdbfile_panel);
    Show(pdbfile_panel);

    Disable(Tax_accept);
    Disable(Tax_all_accept);
    Disable(Tax_part_accept);
    Disable(Tax_finish);
    Disable(Tax_reject);
    Disable(ReLookup_accept);
    Disable(ReLookup_reject);
    Disable(Source_string_accept);

}
/*------------------------------------------------------------------*/
static void EntryOpenProc(ButtoN b)
{
    Char fullname[120];
    Int2 current_zone, CheckProcReturn;


    current_zone = 3;
    CheckProcReturn = CheckProc(current_zone);
    if(CheckProcReturn == -1) return;

    My_Reset1();

    GetTitle(Entry_code, EntryCode, sizeof(EntryCode));
printf("Entry Code: %s\n", EntryCode);

    SingleEntry = TRUE;
    EntryFromList = FALSE;
    GoToEntry();
    
}  
/*------------------------------------------------------------------*/
static void AddStringProc(ButtoN b)
{

    Enable(Source_string_accept);

}
/*------------------------------------------------------------------*/
static void ReLookupProc(ButtoN b)
{

    Enable(ReLookup_accept);
    Enable(ReLookup_reject);

}
/*------------------------------------------------------------------*/
static void TaxAcceptProc(ButtoN b)
{

     if(SingleChain) {
             AllAssign = TRUE;
             PartAssign = FALSE;

             Disable(Tax_all_accept);
             selected_chain =' ';

             if(AssignOrg()){ 
                 WriteEntry();
                 if(EntryFromList && !SingleEntry) seq_entry_TaxState[index_num[current_seq_entry_num]] = '+';
                 if(EntryFromList) DoListUpdate();
             }

             Disable(Tax_part_accept);
             Disable(Tax_accept);
             Disable(Tax_reject);      


     }


     else {
             Enable(Tax_all_accept);
             Enable(Tax_part_accept);
     }

     Disable(Tax_accept);
     Disable(Tax_reject);

}
/*-----------------------------------------------------------------*/
static void TaxAllAcceptProc(ButtoN b)
{
     Int2 n;

     AllAssign = TRUE;
     PartAssign = FALSE;

     Disable(Tax_all_accept);
     selected_chain =' ';

     for(n = 0; n < nchain; n++){
             chain_assign[n] = TRUE;
     }

     if(AssignOrg()){
         WriteEntry();
         if(EntryFromList && !SingleEntry) seq_entry_TaxState[index_num[current_seq_entry_num]] = '+';
         if(EntryFromList) DoListUpdate();
     }

     Disable(Tax_part_accept);
     Disable(Tax_accept);
     Disable(Tax_reject);

}
/*------------------------------------------------------------------*/
static void TaxPartAcceptProc(ButtoN b)
{

     Int2 n;
     Char single_chain;

     Hide(lchain);
     Reset(lchain);
     Show(lchain);

     Enable(Tax_finish);
     Disable(Tax_all_accept);
     Enable(ReLookup_accept);
     Enable(ReLookup_reject);

     AllAssign = FALSE;
     PartAssign = TRUE;

     
     for(n = 0; n < nchain; n++){
        single_chain = chain[n];
        ListItem(lchain, &single_chain);
     }

}
/*------------------------------------------------------------------*/
static void SourceStringAcceptProc(ButtoN b)
{

    Char str[1024];

    GetTitle(Source_string, str, sizeof(str));
    if (StringLen(str) == 0) Disable(Source_string_accept);
    else{
           StringCpy(pdbsource_str, str);
           my_org = (OrgRefPtr) GetOrgRef(pdbsource_str);
    }

}
/*------------------------------------------------------------------*/
static void ReLookupRejectProc(ButtoN b)
{

    Disable(Tax_accept);
    Disable(Tax_reject);
    Disable(ReLookup_accept);
    Disable(ReLookup_reject);
    Disable(Source_string_accept);
    Disable(ReLookup_reject);
}
/*-----------------------------------------------------------------*/
static void TaxPartAssignProc(LisT lchain)
{

    Int2 val;
    MsgAnswer ans;

/*  AllAssign = FALSE;
    PartAssign = TRUE;   */

    val = GetValue(lchain);

    selected_chain_num = val - 1;
    selected_chain = chain[selected_chain_num];
 
    if(chain_assign[selected_chain_num]){
            ans = Message(MSG_YN, "Tax for chain '%c' has been assigned as ' %s ', do you want to reassign?", chain[selected_chain_num], chain_orgName[selected_chain_num]);  
            if(ans == ANS_NO) return;
    }

    my_org_chain[selected_chain_num] = AsnIoMemCopy(my_org, (AsnReadFunc) OrgRefAsnRead, (AsnWriteFunc) OrgRefAsnWrite); 

    chain_assign[selected_chain_num] = TRUE;
    StringCpy(chain_orgName[selected_chain_num], CurrentOrgName);

    Message(MSG_POST, "Orgname for chain '%c' is ' %s '\n", chain[selected_chain_num], chain_orgName[selected_chain_num]);    

}
/*---------------------------------------------------------*/
static void TaxFinishProc(ButtoN b)
{
     Int2 n, assign = 0, unassign = 0, num_chain_taxed = 0;
     Char unassigned_chain[1024], chain_taxed[1024], str[12];
     MsgAnswer ans;

     StringCpy(unassigned_chain, "Chain: ");
     StringCpy(chain_taxed, "  ");
     for(n = 0; n < nchain; n++){
         if(!chain_assign[n]){
             unassign++;
             sprintf(str, "%c ", chain[n]);
             StringCat(unassigned_chain, str);
             if(BioOrg_chain[n]) {
                 StringCat(chain_taxed, str);
                 num_chain_taxed++;
             }
         }
         else assign++;
     }

     if(unassign > 0){
         if(num_chain_taxed == 0) ans = Message(MSG_YN, "%s has(have) not been assigned tax, Revise now?", unassigned_chain); 
         else if(unassign == num_chain_taxed) ans = Message(MSG_YN, "%s has(have) not been assigned tax, but they all have predefined tax. Revise now?", unassigned_chain);
         else if(unassign > num_chain_taxed) ans = Message(MSG_YN, "%s has(have) not been assigned tax. Among which, chain %s has(have) predefined tax. Revise now?", unassigned_chain, chain_taxed);

         if( ans == ANS_YES) return;
         else {
             ans = Message(MSG_YN, "Revise later?");
             if( ans == ANS_YES){
                 if(EntryFromList && !SingleEntry) seq_entry_TaxState[index_num[current_seq_entry_num]] = '?';
                 if(EntryFromList) DoListUpdate();
                 return;
             }
         }
     }

    
     if(AssignOrg()){
         WriteEntry();
         if(assign == nchain) seq_entry_TaxState[index_num[current_seq_entry_num]] = '+';
         else seq_entry_TaxState[index_num[current_seq_entry_num]] = '*';
         if(EntryFromList) DoListUpdate();
     }

     Disable(ReLookup_accept);
     Disable(ReLookup_reject);
     Disable(Source_string_accept);

     Disable(Tax_all_accept);
     Disable(Tax_part_accept);

     Disable(Tax_accept);
     Disable(Tax_reject);

     Disable(Tax_finish);

}
/*---------convert from upper to low case------------*/
IOtolow( s )
char    s[];
{
        int     m ;

        m = 'a' - 'A';
        while (*s != '\0') {
                if( 'A' <= *s && *s <= 'Z' ) *s += m;
                s++;
        }
}
/*-------------- Display ----------------------------------*/
void DisplayChainForBlastSearch()
{
     Int2 n;
     Char single_chain;

     Hide(lBlastChain);
     Reset(lBlastChain);
     Show(lBlastChain);

     if(nchain == 1) ListItem(lBlastChain, EntryCode);
     else {
        for(n = 0; n < nchain; n++){
           single_chain = chain[n];
           ListItem(lBlastChain, &single_chain);
        }
     }

} 
/*-------------- Display ----------------------------------*/
void PDBFileDisplay()
{
     Char pdbfile[120], line[120];
     Char EntryCode_lc[120];
     FILE *fp, *fp_log;
     Char path[PATH_MAX];

     StringCpy(EntryCode_lc, EntryCode);
     IOtolow(EntryCode_lc);

     StringCpy(pdbfile, myargs[4].strvalue);
     StringCat(pdbfile, "pdb");

     StringCat(pdbfile, EntryCode_lc);
     StringCat(pdbfile, ".ent");

     TmpNam (path);

     fp = FileOpen(pdbfile, "r");
     if( fp != NULL){
              fp_log = FileOpen(path, "w");
              while(fgets(line, 120, fp)){
                  if(StringNCmp(line, "ATOM  ", 6) == 0) break;
                  fprintf(fp_log, "%s\n", line);
              }

              FileClose(fp);
              FileClose(fp_log);
              DisplayFile(pdbfile_panel, path, programFont);
              FileRemove (path);
     }
     else Message(MSG_OK, "PDB file ' %s ' does not exists!", pdbfile);

}
/*---------------------------------------------------------*/
static void PdbOpenProc(ButtoN b)
{
     PDBFileDisplay();
     DisplayChainForBlastSearch();
}
/*--------------------------------------------------------*/
static void PdbCloseProc(ButtoN b)
{

     Hide(pdbfile_panel);
     Reset(pdbfile_panel);
     Show(pdbfile_panel);

}
/*---------------------------------------------------------*/
TextSearch()
{
     Char str[1024], line[120], path[PATH_MAX];
     CharPtr string_search = NULL;
     Int4  BarValue, LineNumber = 0;

     FILE *fp;

     BaR bar;

     bar = GetSlateVScrollBar( (SlatE) pdbfile_panel);
     BarValue = GetBarValue(bar);

     GetTitle(Pdbfile_string, str, sizeof(str));
     if(StringLen(str) == 0) Message(MSG_OK, "put in a string first, please!");
     else {

         TmpNam (path);

         fp = FileOpen(path, "w");
         SaveDocument(pdbfile_panel, fp);
         FileClose(fp);

         fp = FileOpen(path, "r");

         while(fgets(line, 120, fp)){
             if(LineNumber >= BarValue) {
                 string_search = StringSearch(line, str);
             }
             if(string_search != NULL) {
                 BarValue = LineNumber;
                 SetBarValue(bar, BarValue);
                 break;
             }
             LineNumber++;

         }
         if(string_search == NULL) Message(MSG_OK, "Not found!\n");

         FileClose(fp);
     }

     FileRemove (path);

}
/*---------------------------------------------------------*/
static void PdbfileStringAcceptProc(ButtoN b)
{

     TextSearch();

}
/*--------------------------------------------------------*/
static void ListEntryStartProc(ButtoN b)
{
     Int2 val;
     Int2 current_zone, CheckProcReturn;


     Disable(Entry_accept);

     if(!redo){
             current_zone = 1;
             CheckProcReturn = CheckProc(current_zone);
             if(CheckProcReturn == -1) return;
     }

     redo = FALSE;

     My_Reset1();

     Hide(Entry_code);
     Reset(Entry_code);
     Show(Entry_code);

     val = GetValue(l);

     current_seq_entry_num = val - 1;
     StringCpy(EntryCode, seq_entry[index_num[current_seq_entry_num]]);
     SingleEntry = FALSE;
     EntryFromList = TRUE;
     GoToEntry();
     if(index_num[current_seq_entry_num] >= entry_num - 1) {
         Disable(ListEntry_next);
         Message(MSG_OK, "The last entry in the current list has been done! You may quit program or open the updated list to work on the entry which has been missed before!");
     }

     return;

}
/*--------------------------------------------------------*/
static void ListEntryNextProc(ButtoN b)
{

     Int2 current_zone, CheckProcReturn;

     Disable(Entry_accept);

     current_zone = 2;
     CheckProcReturn = CheckProc(current_zone);
     if(CheckProcReturn == -1) return;

     My_Reset1();

     Hide(Entry_code);
     Reset(Entry_code);
     Show(Entry_code);

     current_seq_entry_num++;


     if(index_num[current_seq_entry_num] <= entry_num - 1){
             StringCpy(EntryCode, seq_entry[index_num[current_seq_entry_num]]);
             SetValue(l, current_seq_entry_num + 1);

             SingleEntry = FALSE;
             EntryFromList = TRUE;

             GoToEntry();
     }

     if(index_num[current_seq_entry_num] >= entry_num - 1) {
         Disable(ListEntry_next);
         Message(MSG_OK, "The last entry in the current list has been done! You may quit program or open the updated list to work on the entry which has been missed before!");
     }

}
/*--------------------------------------------------------*/
static void ListEntrySpecificProc(ButtoN b)
{

     if(TaxAssign) {
             My_Reset1();
             TaxAssign =  TRUE;
     }
     else My_Reset1();  

     Enable(Entry_accept);

}
/*-------------------------------------------------------*/
static void InFileTypeProc(GrouP infilemode)
{

     Int2  val;

     val = GetValue(infilemode);

     if(val == 1) TextInEntry = TRUE;
     else if(val == 2) TextInEntry = FALSE;

}
/*-------------------------------------------------------*/
static void OutFileTypeProc(GrouP outfilemode)
{

     Int2  val;

     val = GetValue(outfilemode);

     if(val == 1) TextOutEntry = TRUE;
     else if(val == 2) TextOutEntry = FALSE;

}
/*-------------------------------------------------------*/
static void ListBlastChainProc(LisT l)
{
     Int2 val = 0;

     val = GetValue(l);
     if(val == 0) Disable(BlastSearchAccept);
}
/*-------------------------------------------------------*/
static void BlastSearchAcceptProc(ButtoN b)
{
     Int2 val = 0;

     val = GetValue(lBlastChain);
     selected_chain_num = val - 1;

     PDBeastSendBlastSearch(selected_chain_num);
}
