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
* File Name:  mmdborgcpy.c
*
* Author:  Yanli Wang
*
* Initial Version Creation Date: 04/19/1997
*
* File Description:
*         Copy taxonomy assignment from structure bioseq to biostruc
*
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: mmdborgcpy.c,v $
* Revision 1.1  2002/12/17 15:13:43  ywang
* pdbeast related tools updated
*
*
* ==========================================================================
*/

/*----------------- get org from Bioseq and put them in Biostruc -------------*/
#include <ncbi.h>
#include <ncbistr.h>
#include <taxinc.h>
#include <objseq.h>
#include <objsset.h>
#include <objfeat.h>
#include <objblock.h>
#include <objloc.h>
#include <objmmdb1.h>
#include <objmmdb2.h>
#include <objmmdb3.h>
#include <mmdbapi1.h>

Args myargs[9] = {
    {"Text Input Seq-entry File Directory","/net/keystone/usr/people/ywang/work/taxonomy/RawBioseq/", NULL,NULL,TRUE,'h',ARG_STRING,0.0,0,NULL},
    {"Binary Input Seq-entry File Directory","/net/clobber/usr/people8/bryant/MMDB.Bioseq/", NULL,NULL,TRUE,'i',ARG_STRING,0.0,0,NULL},
    {"Text Input Biostruc File Directory","/net/keystone/usr/people/ywang/work/taxonomy/Biostr_text/", NULL,NULL,TRUE,'h',ARG_STRING,0.0,0,NULL},
    {"Binary Input Biostruc File Directory","/net/clobber/usr/people8/bryant/MMDB/", NULL,NULL,TRUE,'i',ARG_STRING,0.0,0,NULL},
    {"Text Output Biostruc File Directory","/net/keystone/usr/people/ywang/work/taxonomy/Bioseq_text/", NULL,NULL,TRUE,'n',ARG_STRING,0.0,0,NULL},
    {"Binary Output Biostruc File Directory","/net/clobber/usr/people10/ywang/work/taxonomy/MMDB/", NULL,NULL,TRUE,'o',ARG_STRING,0.0,0,NULL},
    {"Entry Code List File Directory","/net/keystone/usr/people/ywang/work/taxonomy/index/", NULL,NULL,TRUE,'b',ARG_STRING,0.0,0,NULL},
    {"List File Name","all.OCT96", NULL,NULL,TRUE,'c',ARG_FILE_IN,0.0,0,NULL},
    {"Log errors to file named:","log.pdbeast.April",NULL,NULL,TRUE,'l',ARG_FILE_OUT, 0.0,0,NULL} };

#define chain_choice_num  1
#define biosource_choice_num 6
#define molecule_type_num 9

FILE *afp;
Boolean IsBioseq, IsBioseqSet, TextInSeq = FALSE, TextInStr = FALSE, TextOutStr = FALSE, SeqOpen;
Int4 nchain, num_assign;
Char chain[100], current_chain;
BioSourcePtr biosource_chain[100];
SeqEntryPtr  root_sep;
BiostrucPtr  root_bstrucp;
AsnIoPtr aip1, aip2, aip3;
/*----------------   Initialize --------------------------*/
Init()
{

    SeqEntryLoad();

    objmmdb1AsnLoad();
    objmmdb2AsnLoad();
    objmmdb3AsnLoad();

    SeqLocAsnLoad();
}
/*-------------------------------------------------*/
PrintArg(){

       printf("\t -aid.seq      Text Input Seq-entry File Directory\n");
       printf("\t -bid.seq      Binary Input Seq-entry File Directory\n");
       printf("\t -aid.str      Text Input Biostruc File Directory\n");
       printf("\t -bid.str      Binary Input Biostruc File Directory\n");
       printf("\t -aod          Text Output Biostruc File Directory\n");
       printf("\t -bod          Binary Output Biostruc File Directory\n");
       printf("\t -ListPath     Entry Code List File Directory\n");
       printf("\t -ListName     Entry Code List File Name\n");
       printf("\t -Logfile      File Name For Log Errors\n");
       printf("\t -im.seq       Input Mode of Seq-entry File: 'a' for text / 'b' for binary\n");
       printf("\t -im.str       Input Mode of Biostruc File: 'a' for text / 'b' for binary\n");
       printf("\t -om           Output Mode of Biostruc File: 'a' for text / 'b' for binary\n");

       return;

}
/*---------------    Get Arguments ------------------*/
InitArg(argc, argv)
int argc;
char *argv[];
{

    Char listname[120], Command[120], str[120];
    Char my_argu[120];

    if(argc == 1){
        PrintArg();
        exit (1);
    }
    else{
        while(--argc > 0){
            *++argv;
            if(StringCmp(*argv, "-aid.seq") == 0) {
                sscanf(*++argv, "%s", my_argu);    
                myargs[0].strvalue = StringSave(my_argu);
            }
            else if (StringCmp(*argv, "-bid.seq") == 0) {
                sscanf(*++argv, "%s", my_argu);
                myargs[1].strvalue = StringSave(my_argu);
            }
            if(StringCmp(*argv, "-aid.str") == 0) {
                sscanf(*++argv, "%s", my_argu);
                myargs[2].strvalue = StringSave(my_argu);
            }
            else if (StringCmp(*argv, "-bid.str") == 0) {
                sscanf(*++argv, "%s", my_argu);
                myargs[3].strvalue = StringSave(my_argu);
            }
            else if (StringCmp(*argv, "-aod") == 0) {
                sscanf(*++argv, "%s", my_argu);
                myargs[4].strvalue = StringSave(my_argu);
            }
            else if (StringCmp(*argv, "-bod") == 0) {
                sscanf(*++argv, "%s", my_argu);
                myargs[5].strvalue = StringSave(my_argu);
            }
            else if (StringCmp(*argv, "-Listpath") == 0) {
                sscanf(*++argv, "%s", my_argu);
                myargs[6].strvalue = StringSave(my_argu);
            }
            else if (StringCmp(*argv, "-ListName") == 0) {
                sscanf(*++argv, "%s", my_argu);
                myargs[7].strvalue = StringSave(my_argu);
            }
            else if (StringCmp(*argv, "-Logfile") == 0) {
                sscanf(*++argv, "%s", my_argu);
                myargs[8].strvalue = StringSave(my_argu);
            }
            else if (StringCmp(*argv, "-im.seq") == 0) {
                sscanf(*++argv, "%s", str);
                if(StringCmp(str, "a") == 0) TextInSeq = TRUE;
                else if(StringCmp(str, "b") == 0) TextInSeq = FALSE;
            }
            else if (StringCmp(*argv, "-im.str") == 0) {
                sscanf(*++argv, "%s", str);
                if(StringCmp(str, "a") == 0) TextInStr = TRUE;
                else if(StringCmp(str, "b") == 0) TextInStr = FALSE;
            }
            else if (StringCmp(*argv, "-om") == 0) {
                sscanf(*++argv, "%s", str);
                if(StringCmp(str, "a") == 0) TextOutStr = TRUE;
                else if(StringCmp(str, "b") == 0)TextOutStr = FALSE;
            }
       }
    }


}
/*------------------------------------------------------*/
InitBoolean()
{

    SeqOpen = FALSE; 
    IsBioseq = FALSE;
    IsBioseqSet = FALSE;

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
/*------------------------------------------------------*/
AccessBioseq(BioseqPtr my_bsp)
{
    BioSourcePtr my_bio_sourcep;
    PDBSeqIdPtr my_pdb_seq_id;
    ValNodePtr descr;



    my_pdb_seq_id = GetPdbSeqId(my_bsp);

    if(my_pdb_seq_id !=NULL) {
        if(my_pdb_seq_id->chain !=NULL) chain[nchain] = (Char) my_pdb_seq_id->chain;
        else printf("No Identifier for This Chain!!!\n");
    }

    descr = my_bsp->descr;

    biosource_chain[nchain] = NULL;

    while(descr != NULL){
        if(descr->choice == 23) {
            biosource_chain[nchain] = descr->data.ptrvalue;
            break;
        }
          
        descr = descr->next;
    }

    return;

}
/*------------------------------------------------------*/
AccessSeqEntry(SeqEntryPtr my_sep)
{

    BioseqPtr  my_bsp;
    BioseqSetPtr my_bssp;
    Int2 choice;

    nchain = 0;
    choice = my_sep->choice;
    if(choice == 1 ) {
        IsBioseq = TRUE;
        my_bsp = my_sep->data.ptrvalue;
        AccessBioseq(my_bsp);
        nchain++;
    }
    else if(choice == 2){
        IsBioseqSet = TRUE;
        my_bssp = my_sep->data.ptrvalue;
        my_sep = my_bssp->seq_set;

        while(my_sep != NULL){
            my_bsp = my_sep->data.ptrvalue;
            AccessBioseq(my_bsp);
            nchain++;
            my_sep = my_sep->next;
        }
    }

}
/*------------------------------------------------------*/
GoToSeqEntry(Char code[120])
{
    Char fname[120];
    CharPtr option;

    InitBoolean();

    if(TextInStr){
        StringCpy(fname, myargs[0].strvalue);
        StringCat(fname, "a");
    }
    else{
        StringCpy(fname, myargs[1].strvalue);
        StringCat(fname, "b");
    }

    StringCat(fname, code);
    StringCat(fname, ".ASN1");

    if(TextInSeq){
        if(!(aip1 = AsnIoOpen(fname, "r"))) {
            printf("Seq-Entry File %s can not be opened!", fname);
            return;
        }
        else SeqOpen = TRUE;
    }
    else{
        if(!(aip1 = AsnIoOpen(fname, "rb"))) {
            printf("Seq-Entry File %s can not be opened!", fname);
            return;
        }
        else SeqOpen = TRUE;
    }

    
    root_sep = SeqEntryAsnRead(aip1, NULL);

    aip1 = AsnIoClose(aip1);

    AccessSeqEntry(root_sep);

}
/*-------------- Delete existed Org -------------------*/
DeleteOrg(MoleculeGraphPtr mgp)
{
    BioSourcePtr my_biosource;
    ValNodePtr descr, last_descr = NULL;

    descr = mgp->descr;

    while(descr != NULL){
        if(descr->choice == biosource_choice_num){
            my_biosource = descr->data.ptrvalue;
            my_biosource = BioSourceFree(my_biosource);
            if(last_descr != NULL) {
                last_descr->next = descr->next;
                MemFree(descr);

                return;
            }
            else {
                mgp->descr = descr->next; 
                MemFree(mgp->descr);
                return;
            }
        }

        last_descr = descr;
        descr = descr->next; 

    }

    return;

}
/*------------------------------------------------------*/
GetBioSource(MoleculeGraphPtr mgp)
{

    Int2 n;
    Boolean OrgExist = FALSE;

    BioSourcePtr dummy_bsourcep;
    BioSourcePtr ThisBiosource;
    ValNodePtr descr;

    descr = mgp->descr;

    while(descr != NULL){
        if(descr->choice == biosource_choice_num){
            ThisBiosource = descr->data.ptrvalue;
            if(ThisBiosource != NULL) {
		DeleteOrg(mgp);
            }
        }
       descr = descr->next;
    }

    for(n = 0 ; n < nchain; n++){
        if(current_chain == chain[n]){
            num_assign++;
            if(biosource_chain[n] != NULL){
                dummy_bsourcep = AsnIoMemCopy(biosource_chain[n], (AsnReadFunc)BioSourceAsnRead, (AsnWriteFunc) BioSourceAsnWrite);     
       
                ValNodeAddPointer(&mgp->descr, biosource_choice_num, dummy_bsourcep);
                break;
            }
            else {
                printf("BioSourcePtr biosource_chain is NULL\n");
                return;
            }
        }
   }


    return;

}    
/*------------------------------------------------------*/
AccessBiostruc(BiostrucPtr my_bstrucp)
{

    BiostrucGraphPtr bgp;
    MoleculeGraphPtr mgp;
    ValNodePtr descr;
    CharPtr chainp;
    Int4 molecule_type;

    num_assign = 0;

    bgp = my_bstrucp->chemical_graph;
    mgp = bgp->molecule_graphs;

    while(mgp != NULL){
        if(mgp->descr != NULL){
            descr = mgp->descr;
            while(descr != NULL){
                if(descr->choice == chain_choice_num){
                    chainp = descr->data.ptrvalue;
                    current_chain = *chainp;
                }
                if(descr->choice == molecule_type_num) molecule_type = descr->data.intvalue;     

                descr = descr->next;

            }

            if( molecule_type  == Molecule_type_protein || molecule_type  == Molecule_type_dna || molecule_type  == Molecule_type_rna || molecule_type  == Molecule_type_other_biopolymer || molecule_type  == Molecule_type_other_nonpolymer) GetBioSource(mgp);    
        }
/*      else {
            descr = ValNodeNew(NULL);
            current_chain = ' ';
            GetBioSource(descr);      
            mgp->descr = descr->next;
        }      */
      
        if(num_assign == nchain ) break;
        else mgp = mgp->next;

    }


    return;

}
/*------------------------------------------------------*/
GoToBiostruc(Char code[120])
{

    Char fname[120];
    CharPtr option;


    if(TextInStr){
        StringCpy(fname, myargs[2].strvalue);
        StringCat(fname, "a");
    }
    else{
        StringCpy(fname, myargs[3].strvalue);
        StringCat(fname, "b");
    }

    StringCat(fname, code);
/*  StringCat(fname, ".ASN1");  */

    if(TextInStr){
        if(!(aip2 = AsnIoOpen(fname, "r"))) {
            printf("Can not open Input Biostruc File %s\n", fname);
            fprintf(afp, "Org has NOT been copied in!\n\n");
            return;   
        }
    }
    else {
        if(!(aip2 = AsnIoOpen(fname, "rb"))) {
            printf("Can not open Input Biostruc File %s\n", fname);
            fprintf(afp, "Org has NOT been copied in!\n\n");
            return;   
        }
    }

    root_bstrucp = BiostrucAsnRead(aip2, NULL);
    aip2 = AsnIoClose(aip2);


    if(TextOutStr){
        StringCpy(fname, myargs[4].strvalue);
        StringCat(fname, "a");
    }
    else{
        StringCpy(fname, myargs[5].strvalue);
        StringCat(fname, "b");
    }

    StringCat(fname, code);
/*  StringCat(fname, ".ASN1");  */
  
    if(TextOutStr){
        if(!(aip3 = AsnIoOpen(fname, "w"))) {
            printf("File %s  can not be opened!\n");
            fprintf(afp, "Org has NOT been copied in!\n\n");
            return; 
        }
    }
    else {
        if(!(aip3 = AsnIoOpen(fname, "wb"))) {
            printf("File %s  can not be opened!\n");
            fprintf(afp, "Org has NOT been copied in!\n\n");
            return;
        }
    }

/*  root_bstrucp = BiostrucAsnRead(aip2, NULL);
    aip2 = AsnIoClose(aip2);   */

    AccessBiostruc(root_bstrucp);

    BiostrucAsnWrite(root_bstrucp, aip3, NULL);
    aip3 = AsnIoClose(aip3);
    fprintf(afp, "Org has been copied in!\n\n");
    
    root_bstrucp = BiostrucFree(root_bstrucp);
    root_sep = SeqEntryFree(root_sep);

    return;

}
/*----------------   Main Starts Here ------------------*/
Int2 Main()
{
    Char fname[120], code[120];
    FILE *CodeList;

    Init();
    InitArg(GetArgc(), GetArgv());

    afp = FileOpen(myargs[8].strvalue, "w");

    StringCpy(fname, myargs[6].strvalue);
    StringCat(fname, myargs[7].strvalue);
 
    CodeList = FileOpen(fname, "r");
    if(CodeList == NULL){
        printf("Problem reading file: %s \n", fname);
        return;
    }

    while(fscanf(CodeList, "%s", code) != EOF){
        fprintf(afp, "\tEntry: %s\n", code);

printf("%s\n", code);

        GoToSeqEntry(code);
        if(SeqOpen) GoToBiostruc(code); 


    }

    FileClose(afp);

}

