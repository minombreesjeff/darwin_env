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
* File Name:  pdbeast.c
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
* $Log: pdbeast.c,v $
* Revision 1.2  2002/12/17 16:42:08  ywang
* add warning message for object dumping
*
* Revision 1.1  2002/12/17 15:13:43  ywang
* pdbeast related tools updated
*
*
* ==========================================================================
*/
#include <ncbi.h>
#include <ncbistr.h>
#include <taxinc.h>
#include <objseq.h>
#include <objsset.h>
#include <objfeat.h>
#include <objblock.h>
#include <objloc.h>
#include <vibrant.h>
#include <document.h>
#include <bspview.h>
#include "para.h"
#include "pdbeast.h"

extern Args myargs[8];     
extern WindoW w;
extern TexT pdb_source;
extern DoC org;
extern DoC chain_panel;
extern ButtoN  Tax_accept;
extern ButtoN  Tax_reject;
extern ButtoN  ReLookup_accept;
extern ButtoN  ReLookup_reject;
extern Char seq_entry[MaxEntryNum][12], seq_entry_TaxState[MaxEntryNum];
extern Int4 current_seq_entry_num;
extern Int4 index_num[MaxEntryNum];
extern FILE *log_pdbeast;
/*-------------- Initialize -----------------------*/
Init()
{

/*      if (!GetArgs("getorg", 1, arglist)) {
             fprintf(stderr, "Fail to parse argument list in the command line\n");
             return -1;
        }   */

        printf("Initializing ......SeqEntryLoad......\n");

        SeqEntryLoad();    /* important!  */

        printf("Conneting to Taxonomy Server......\n");

        printf("Initializing ......");
        if(!tax1_init()) {
                printf("Can not make connection to taxonomy server\n");
                exit (1);
        }
        printf("Taxonomy server connected\n\n");
}
/*------------- Close Tax ----------------------*/
Close()
{
        tax1_fini();
        printf("All done\n");

}
/*------------ IO Initialize --------------*/
IoOpen()
{
    if(TextInEntry) aip1 = AsnIoOpen(RawSeqEntryName, "r");
    else aip1 = AsnIoOpen(RawSeqEntryName, "rb");

/*  if(TextOutEntry) aip2 = AsnIoOpen(SeqEntryName, "w");
    else aip2 = AsnIoOpen(SeqEntryName, "wb");  */

    return;
}
/*------------ Initialize Booleans ------------------*/
InitBoolean()
{
    IsBioseq = FALSE;
    IsBioseqSet = FALSE;
    Pdbsource = FALSE;
    Biosource = FALSE;
    Bio_source = FALSE;
    ReplaceSource = TRUE;
    OrgFound = TRUE;
    AllAssign = FALSE;
    PartAssign = FALSE;
    SingleChain = FALSE;

    return;
}
/*------------ Close AsnOpen File --------------*/
IoClose()
{

    AsnIoClose(aip1);
    AsnIoClose(aip2);
    
    return;
}
/*------------- Initialize Structure Status ----------------*/
Boolean Get_SS(int state_num, int choice)
{

    if(choice == state_num) return (TRUE);
    else return(FALSE);

}
/*-----------  Get OrgRef pointer by searching Taxonomy Database -----------*/
OrgRefPtr GetOrgRef(CharPtr str)
{
	Int4 tax_id = 0, n = 0, i = 0;

	Int4Ptr Ids;
	CharPtr orgName;

	Taxon1DataPtr t_data = NULL;
	OrgRefPtr my_org = NULL, first_org = NULL;

    AsnIoPtr aip_org;
    FILE *prompt;
    Char path[PATH_MAX];

/*  printf("Search Taxonomy Database with ' %s '\n", str);  */

    TmpNam(path);

    n= tax1_getTaxId4Str((CharPtr) str, &orgName, &Ids);     


    if(n <= 0) {

         Hide(org);
         Reset(org);
         Show(org);

/*       AppendText(org," Organism not found! ", NULL, NULL, systemFont);     */

         prompt = FileOpen(path, "w");     
     
         fprintf(prompt, "Organism not found! \n\n");
         fprintf(prompt, "Skip This Entry or Search Taxonomy Database with Another Name\n");

         FileClose(prompt);
         DisplayFile(org, path, systemFont);
         FileRemove(path);
 
         Message(MSG_OK, " Organism not found! Skip This Entry or Search Taxonomy Database with Another Name");

         Enable(ReLookup_accept);
         Enable(ReLookup_reject);    

         OrgFound = FALSE;     

         return(NULL);
    }
    else {
/*      printf("\tOrg Name: %s\n\tTax id:", orgName);    */
        for(i= 0; i < n; i++) {
/*         printf(" %d", Ids[i]);  */
        }

        StringCpy(CurrentOrgName, orgName);
        MemFree(orgName);

        Enable(Tax_accept);
        Enable(Tax_reject);

    }

    if(n == 1){
      tax_id = Ids[0];  /* just use the first Id, how about if n > 0 ??? */

      t_data = tax1_getbyid(tax_id);
      if(t_data == NULL) printf("can not retrieve through ID\n");

      my_org = t_data->org;

      MemFree(Ids);
      MemFree(t_data->div);
      MemFree(t_data->embl_code);
      MemFree(t_data);

      aip_org = AsnIoOpen(path, "w");
      OrgRefAsnWrite(my_org, aip_org, NULL);
      AsnIoClose(aip_org);
      DisplayFile(org, path, systemFont);
      FileRemove (path);
   
      return(my_org);
   }
   else {
     prompt = FileOpen(path, "w");
     fprintf(prompt, "Warning!! Return multiple organisms!\n\n");
     fprintf(prompt, "Please chose one from the following 'OrgRef' list, do 'Search Taxonomy Database' with the right string to get a unique taxa node!\n\n");
     fprintf(prompt, "If not doing so, the first valid taxa node will be returned if 'Yes' clicked!\n\n");
     FileClose(prompt);

     for(i = 0; i < n ; i++){

       prompt = FileOpen(path, "a");
       aip_org = AsnIoNew(ASNIO_TEXT_OUT, prompt, NULL, NULL, NULL);

       tax_id = Ids[i];
       t_data = tax1_getbyid(tax_id);
       if(t_data == NULL) fprintf(aip_org->fp, "can not retrieve through ID\n");
       my_org = t_data->org;
       if(i == 0 && my_org != NULL) first_org = my_org;
       else if (!first_org && my_org) first_org = my_org; 

       OrgRefAsnWrite(my_org, aip_org, NULL);
       AsnIoClose(aip_org);

       if(i > 0 && t_data->org) OrgRefFree(t_data->org);
       MemFree(t_data->div);
       MemFree(t_data->embl_code);
       MemFree(t_data);

       prompt = FileOpen(path, "a");
       fprintf(prompt, "\n");
       FileClose(prompt);
       
    } 

    MemFree(Ids);

    DisplayFile(org, path, systemFont);
    FileRemove (path);

    return(first_org);
  }
}
/*--- Modifying SeqEntry By Adding Org to Descriptor ------*/
OrgAdd(ValNodePtr descr, OrgRefPtr my_org)
{
    ValNodeAddPointer(&descr, org_choice_num, my_org);

     return;
}
/*--- Modifying SeqEntry By Adding BioSource to Descriptor ------*/
BioSourceAdd(ValNodePtr descr, BioSourcePtr my_biosource)
{
    ValNodeAddPointer(&descr, biosource_choice_num,  my_biosource);

     return;
}
/*-------------------- Get Org and BioSource -------------------------*/
GetOrgBioSource(ValNodePtr descr)
{

    BioSourcePtr my_bio_sourcep;
    OrgRefPtr    dummy_org;

    if(AllAssign) dummy_org = AsnIoMemCopy(my_org, (AsnReadFunc) OrgRefAsnRead, (AsnWriteFunc) OrgRefAsnWrite);

    else if(PartAssign) dummy_org = AsnIoMemCopy(my_org_chain[current_chain_num], (AsnReadFunc) OrgRefAsnRead, (AsnWriteFunc)OrgRefAsnWrite);

    if(dummy_org != NULL){
            my_bio_sourcep = (BioSourcePtr)BioSourceNew();
            my_bio_sourcep->org = dummy_org;
            BioSourceAdd(descr, my_bio_sourcep);
    }

    return;
}

/*------------  Find PdbSource and See Whether BioSource exists ----------*/
PdbSourceFind(ValNodePtr descrp, Int4 num)
{
    Int4 sourcestr_num;
    ValNodePtr pdbsourcep;
    PdbBlockPtr pdbp;


    Pdbsource = FALSE;

    while(descrp != NULL){
          if(descrp->choice == pdbsource_choice_num){
                pdbp = descrp->data.ptrvalue;
                if(pdbp->source != NULL) {
                      Pdbsource = TRUE;
                      pdbsourcep = pdbp->source;
                }
                else {
                      printf("pdb source does not exist!\n");

                }
          }

          descrp = descrp->next;
    }


    if(Pdbsource){
          sourcestr_num = 0;
          while(pdbsourcep != NULL){
               sourcestr_num++;
               sprintf(pdbsource_str, "%s", pdbsourcep->data.ptrvalue);
               pdbsourcep = pdbsourcep->next;
          }

          SetTitle(pdb_source, pdbsource_str);
   }
   else SetTitle(pdb_source, "Warning: no pdb source found!"); 
  


   return;

}
/*--------------- Get PdbSource ----------------*/
GetPdbSource(SeqEntryPtr my_sep)
{
    BioseqPtr my_bsp;
    BioseqSetPtr my_bssp;

    ValNodePtr descr;

    IsBioseq = Get_SS(seq_num, my_sep->choice);
    IsBioseqSet = Get_SS(seq_set_num, my_sep->choice);     

    if(IsBioseq) {
            my_bsp = my_sep->data.ptrvalue;
            descr = my_bsp->descr;
            PdbSourceFind(descr, seq_num);
    }
    else if(IsBioseqSet){
            my_bssp = my_sep->data.ptrvalue;
            descr = my_bssp->descr;
            PdbSourceFind(descr, seq_set_num);
    }

    return;
}
/*---------------  Get PDB Seq-id ----------------------------*/
PDBSeqIdPtr GetPdbSeqId(BioseqPtr my_bsp)
{

    SeqIdPtr  my_seq_id;
    PDBSeqIdPtr my_pdb_seq_id;

    my_seq_id = my_bsp->id;

    while(my_seq_id != NULL){
           if(my_seq_id->choice == 15){
                  my_pdb_seq_id = my_seq_id->data.ptrvalue;
                  break;
           }           
           my_seq_id = my_seq_id->next;
    }

    return(my_pdb_seq_id);

}
/*---------------  Get Current Chain Number -------------------*/
GetCurrentChainNum(BioseqPtr my_bsp)
{

    PDBSeqIdPtr my_pdb_seq_id;
    Char current_chain;
    Int2 n;

    my_pdb_seq_id = GetPdbSeqId(my_bsp);

    if(my_pdb_seq_id !=NULL) {
            if(my_pdb_seq_id->chain !=NULL){
                    current_chain = (Char) my_pdb_seq_id->chain;
                    chain[nchain] = (Char) my_pdb_seq_id->chain;
                    my_org_chain[nchain] = NULL;
            }
            else printf("No Identifier for This Chain!!!\n");
    }

    for(n = 0; n < nchain; n++){
            if(current_chain == chain[n]){
                  current_chain_num = n;
                  break;
            }
    }

}
/*--------------- GetBiosource ----------------------*/
GetBiosource(BioseqPtr my_bsp)
{
     ValNodePtr descrp;

     if(my_bsp->descr != NULL){
           GetOrgBioSource(my_bsp->descr);
     }
     else {
           descrp = ValNodeNew(NULL);
           GetOrgBioSource(descrp);
           my_bsp->descr = descrp->next;
           MemFree(descrp);    /*  ???   */
     }

     return;

}
/*---------------- Overlap the Existed Biosource ---------------*/
OverlapOrg(BioseqPtr my_bsp)
{

    ValNodePtr descr;
    BioSourcePtr my_biosource;
    OrgRefPtr dummy_org;

    if(AllAssign) dummy_org = AsnIoMemCopy(my_org, (AsnReadFunc) OrgRefAsnRead,
(AsnWriteFunc) OrgRefAsnWrite);

    else if(PartAssign) dummy_org = AsnIoMemCopy(my_org_chain[current_chain_num], (AsnReadFunc) OrgRefAsnRead, (AsnWriteFunc)OrgRefAsnWrite);

    descr = my_bsp->descr;

    while(descr != NULL){
        if(descr->choice == biosource_choice_num){
            my_biosource = descr->data.ptrvalue;
            my_biosource->org = dummy_org;
        }

        descr = descr->next;
    }

    return;

}
/*---------------  Access Each Bioseq Entry -------------------*/
AccessBioseq(BioseqPtr my_bsp)
{
     ValNodePtr descrp;

     GetCurrentChainNum(my_bsp);

     if(AllAssign) {
         if(my_org == NULL) return;
     }
     else if(PartAssign){
         if(my_org_chain[current_chain_num] == NULL)  return;
     }

     if(SingleChain) {
         if(Biosource) {
             OverlapOrg(my_bsp);
             return;
         }
     }
     else{
         if(Biosource_chain[current_chain_num]) {
             OverlapOrg(my_bsp);
             return;
         }
     }

     GetBiosource(my_bsp);

     return;
}
/*---------------  Access Each BioseqSet Entry -----------------*/
AccessBioseqSet(BioseqSetPtr my_bssp )
{
    SeqEntryPtr my_sep;

    my_sep = my_bssp->seq_set;

    while(my_sep != NULL){
          AccessSeqEntry(my_sep);
          my_sep = my_sep->next;
    }
   
}
/*---------------  Access Each Existed SeqEntry -----------------*/
AccessSeqEntry(SeqEntryPtr my_sep)
{
    BioseqPtr my_bsp;
    BioseqSetPtr my_bssp;

    IsBioseq = Get_SS(seq_num, my_sep->choice);
    IsBioseqSet = Get_SS(seq_set_num, my_sep->choice);

    if(IsBioseq) {
            my_bsp = my_sep->data.ptrvalue;
            AccessBioseq(my_bsp);
    }
    else if(IsBioseqSet){
            my_bssp = my_sep->data.ptrvalue;
            AccessBioseqSet(my_bssp);
    }

    return;
}
/*---------------- Count Chain Number and Get Identifier ------------*/
CountChainNum(SeqEntryPtr my_sep)
{

    BioseqPtr my_bsp;
    PDBSeqIdPtr my_pdb_seq_id;

    if(my_sep->choice !=1) printf("strange!!!\n");
    my_bsp = my_sep->data.ptrvalue;

    my_pdb_seq_id = GetPdbSeqId(my_bsp);

    if(my_pdb_seq_id !=NULL) {
            if(my_pdb_seq_id->chain !=NULL){
                    chain[nchain] = (Char) my_pdb_seq_id->chain;
                    chain_assign[nchain] = FALSE;
                    nchain++;
            }
            else printf("No Identifier for This Chain!!!\n");
    }

    return;

}
/*--------------- Find Biosource ---------------------*/
BiosourceFind(ValNodePtr descr)
{
    BioSourcePtr my_biosource;

    if(SingleChain) Biosource = FALSE;
    else Biosource_chain[nchain] = FALSE;

    while(descr != NULL){
        if(descr->choice == biosource_choice_num){
            my_biosource = descr->data.ptrvalue;
            if(my_biosource != NULL){
                if(SingleChain) Biosource = TRUE;
                else Biosource_chain[nchain] = TRUE;
 
                if(my_biosource->org != NULL){
                    if(SingleChain) BioOrg = TRUE;
                    else BioOrg_chain[nchain] = TRUE;
                }
            }
        }

        descr = descr->next;

    }

    return;

}
/*---------------- Confirm Biosource Exists or Not ---------------------*/
BiosourceConfirm(SeqEntryPtr my_sep)
{

    BioseqPtr my_bsp;
    ValNodePtr descr;

    my_bsp = my_sep->data.ptrvalue;
    descr = my_bsp->descr;
    
    BiosourceFind(descr);

    return;

}
/*---------------- Get Chain Number and Confirm Whether Biosource Exits ------------*/
GetChainNum_BiosourceConfirm(SeqEntryPtr my_sep)
{

    BioseqSetPtr my_bssp;
    FILE *prompt;
    Char path[PATH_MAX];
    Int2 n;

    IsBioseq = Get_SS(seq_num, my_sep->choice);
    IsBioseqSet = Get_SS(seq_set_num, my_sep->choice);

    TmpNam (path);
    prompt = FileOpen(path, "w");

    if (IsBioseq) {
            nchain = 1;
            SingleChain = TRUE;
            BiosourceConfirm(my_sep);
            fprintf(prompt, "Single Chain\n");

    }
    else if (IsBioseqSet){
            my_bssp = my_sep->data.ptrvalue;
            my_sep = my_bssp->seq_set;
            while(my_sep != NULL) {
                   BiosourceConfirm(my_sep);
                   CountChainNum(my_sep);
                   my_sep = my_sep->next;
            }
 
            fprintf(prompt, "Multiple Chain:  ");
            for( n = 0; n < nchain; n++){
                 fprintf(prompt, "%c  ", chain[n]);
            }
            fprintf(prompt, "\n");

    }

    FileClose(prompt);
    DisplayFile(chain_panel, path, systemFont);
    FileRemove (path);

    return;
}
/*---------------- Access a New SeqEntry -----------------*/
AccessNewSeqEntry()
{

    IoOpen();
    InitBoolean();

    if(aip1 == NULL) {
           EntryOpen = FALSE;
           Message(MSG_OK, " File ' %s ' not found!", RawSeqEntryName);
           root_sep = NULL;
           return;
    }
    else EntryOpen = TRUE;

    root_sep = SeqEntryAsnRead(aip1, NULL);
    if((root_sep) == NULL){
          Message(MSG_OK, "Can not read in any information though Seq-entry has been opened! Please quit the program, fix the data or remove this entry from your list, and restart this program!\n");
          return;
    }

    nchain = 0;

    GetChainNum_BiosourceConfirm(root_sep);

    GetPdbSource(root_sep);

    my_org = GetOrgRef(pdbsource_str);    

    return;

}
/*----------------- Add Tax ---------------------*/
Int2 AssignOrg()
{

    if(root_sep != NULL) {
        AccessSeqEntry((SeqEntryPtr) root_sep);
        return (1);
    }
    else return(-1);

}
/*---------------- IO Close and Free Org -------------------*/
CloseCurrentEntry()
{

    Int2 n;

    if(aip1 != NULL) aip1 = AsnIoClose(aip1);
    if(aip2 != NULL) aip2 = AsnIoClose(aip2);

    if(root_sep != NULL) root_sep = SeqEntryFree(root_sep);
    if(my_org != NULL) my_org = OrgRefFree(my_org);

    if(PartAssign){
          for(n = 0; n < nchain; n++){
               if(my_org_chain[n] !=NULL) my_org_chain[n] = OrgRefFree(my_org_chain[n]);
          }
    }

    return;

}   
/*----------------- Add Tax & Write Entry ------------------*/
WriteEntry()
{

    if(root_sep != NULL){

       if(TextOutEntry) aip2 = AsnIoOpen(SeqEntryName, "w");
        else aip2 = AsnIoOpen(SeqEntryName, "wb");

        if(aip2 == NULL){
           Message(MSG_OK, " File ' %s ' can not be opened!\n", SeqEntryName);
           return;
        }

        if(!SeqEntryAsnWrite(root_sep, aip2, NULL)){
          Message(MSG_OK, " Problem occurred when dumping SeqEntry!\n");
        }
        else{ 
          TaxAssign = TRUE;

          fprintf(log_pdbeast, "Taxonomy has been assigned for %s\n\n", EntryCode);
        }

        CloseCurrentEntry();
    }

    return;
}
/*----------------   Main Starts Here ------------------*/
GoToEntry()
{
	Char fullname[120];

         if(TextInEntry) {
              StringCpy(fullname, myargs[0].strvalue);

              StringCat(fullname, "a");
              StringCat(fullname, EntryCode);           
              StringCat(fullname, ".ASN1");
              StringCpy(RawSeqEntryName, fullname);
         }
         else{
              StringCpy(fullname, myargs[1].strvalue);
              StringCat(fullname, "b");
              StringCat(fullname, EntryCode);           
              StringCat(fullname, ".ASN1");
              StringCpy(RawSeqEntryName, fullname);
         }

         if(TextOutEntry){
              StringCpy(fullname, myargs[2].strvalue);
 
              StringCat(fullname, "a");
              StringCat(fullname, EntryCode);
              StringCat(fullname, ".ASN1");
              StringCpy(SeqEntryName, fullname); 
         }
         else{
              StringCpy(fullname, myargs[3].strvalue);

              StringCat(fullname, "b");
              StringCat(fullname, EntryCode);
              StringCat(fullname, ".ASN1");
              StringCpy(SeqEntryName, fullname); 
         }

         fprintf(log_pdbeast, "\tSeq-entry: %s\n", EntryCode);

         AccessNewSeqEntry();

         PDBFileDisplay();
         DisplayChainForBlastSearch();
}
/*-------------------------------------------------------*/
void PDBeastLaunchBlastSearch(CharPtr str, Uint1 mol, Int2 selected_chain_num, Boolean SingleChain)
{
    CharPtr query = NULL;
    Char thischain[5];
    Int4 str_size = 0;
 

    if(mol == Seq_mol_aa) {
       str_size = StrLen(http_arg1) + StrLen(http_arg2) + StrLen(str) + 30;
    }
    else if(mol == Seq_mol_dna || mol == Seq_mol_rna || mol == Seq_mol_na) {
       str_size = StrLen(http_arg1) + StrLen(http_arg3) + StrLen(str) + 30;
    } 

    query = (CharPtr)MemNew(str_size * sizeof(Char));

    StrCpy(query, http_arg1);
    if(SingleChain){
      StrCat(query, "%3Elcl|+");
      StrCat(query, EntryCode);
      StrCat(query, "%0D%0A");
    }
    else {
      StrCat(query, "%3Elcl|+");
      StrCat(query, EntryCode);
      StrCat(query, "+");
      sprintf(thischain, "%c", chain[selected_chain_num]);
      StrCat(query, thischain);
      StrCat(query, "%0D%0A");
    }

    StrCat(query, str);
    if(mol == Seq_mol_aa) StrCat(query, http_arg2);
    else if(mol == Seq_mol_dna || mol == Seq_mol_rna || mol == Seq_mol_na) StrCat(query, http_arg3);

    Nlm_LaunchWebPage(query);

    MemFree(query);

}
/*-------------------------------------------------------*/
void PDBeastSendBlastSearch(Int2 selected_chain_num)
{

     SeqEntryPtr my_sep = NULL;
     BioseqSetPtr my_bssp;
     BioseqPtr my_bsp = NULL;
     ByteStorePtr bysp = NULL;
     SeqIdPtr sip = NULL;

     CharPtr str = NULL;
   
     Boolean SingleChain = TRUE;

     my_sep = root_sep;

     if(my_sep == NULL) {
        Message(MSG_OK, "Assignment has been done with this PDB entry, sequences are freed already, thus no blast search for this entry!");
        return;
     }

     IsBioseq = Get_SS(seq_num, my_sep->choice);
     IsBioseqSet = Get_SS(seq_set_num, my_sep->choice);

     if(IsBioseq) {
            my_bsp = my_sep->data.ptrvalue;
            bysp = my_bsp->seq_data;
            str = (CharPtr) BSMerge (bysp, NULL);
            PDBeastLaunchBlastSearch(str, my_bsp->mol, selected_chain_num, SingleChain);
     }
     else if(IsBioseqSet){
       SingleChain = FALSE;
       my_bssp = my_sep->data.ptrvalue;
       my_sep = my_bssp->seq_set;
       while(my_sep){
          my_bsp = my_sep->data.ptrvalue;
          GetCurrentChainNum(my_bsp);
          if(current_chain_num == selected_chain_num){
             bysp = my_bsp->seq_data;
             str = (CharPtr) BSMerge (bysp, NULL);
             PDBeastLaunchBlastSearch(str, my_bsp->mol, selected_chain_num, SingleChain);
             break;
          }
          my_sep = my_sep->next;
       }

     }
}
