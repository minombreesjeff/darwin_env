/*   orgmatch.c
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
* File Name:  orgmatch.c
*
* Author:  Yanli Wang
*
* Version Creation Date:   01/19/2000
*
* File Description:
* Function  match text biosource for update pdb file, if true, copy the old 
* taxonomy assignment into new bioseq
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: orgmatch.c,v $
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
#include <objmmdb1.h>
#include <objmmdb2.h>
#include <objmmdb3.h>
#include <mmdbapi1.h>

Args myargs[9] = {
    {"Archived Seq-entry File Directory","/net/vaster/usr/people2/bryant/ASN/MMDB.Bioseq.Archive", NULL,NULL,TRUE,'h',ARG_STRING,0.0,0,NULL},
    {"File Mode for Archived Seq-entry File","/net/vaster/usr/people2/bryant/ASN/MMDB.Bioseq", NULL,NULL,TRUE,'h',ARG_STRING,0.0,0,NULL},
    {"Current Seq-entry File Directory","/net/vaster/usr/people2/bryant/ASN/MMDB.Bioseq.Archive", NULL,NULL,TRUE,'h',ARG_STRING,0.0,0,NULL},
    {"File Mode for Current Seq-entry File","/net/vaster/usr/people2/bryant/ASN/MMDB.Bioseq.Archive", NULL,NULL,TRUE,'h',ARG_STRING,0.0,0,NULL},
    {"Output Seq-entry File Directory","/net/usr/people/ywang/MMDB.Bioseq.PDBeast", NULL,NULL,TRUE,'h',ARG_STRING,0.0,0,NULL},
    {"File Mode for Output Seq-entry File","/net/usr/people/ywang/MMDB.Bioseq.PDBeast", NULL,NULL,TRUE,'h',ARG_STRING,0.0,0,NULL},
    {"SeqEntry List File Name for Checking","./all.lis", NULL,NULL,TRUE,'c',ARG_FILE_IN,0.0,0,NULL},
    {"SeqEntry List File Name for Update","./to.update.lis", NULL,NULL,TRUE,'c',ARG_FILE_OUT,0.0,0,NULL},
    {"Log Errors to File Named:","log.orgmatch",NULL,NULL,TRUE,'l',ARG_FILE_OUT, 0.0,0,NULL} };


#define pdb_seqid_choice 15
#define pdb_source_choice 21
#define bioseq_biosource_choice 23
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

       printf("\t -archived.dir       Archived Seq-entry File Directory\n");
       printf("\t -archived.filemode  File Mode t/b for Archived Seq-entry File\n");
       printf("\t -current.dir        Current Seq-entry File Directory\n");
       printf("\t -current.filemode   File Mode t/b for Current Seq-entry File Directory\n");
       printf("\t -output.dir         Output Seq-entry File Directory\n");
       printf("\t -output.filemode    File Mode t/b for Output Seq-entry File\n");

       printf("\t -inputentry.list    File Name With the List to Be Checked\n");
       printf("\t -list.forupdate     File Name for the Entry List for Reassigning Taxonomy\n");
       printf("\t -Logfile            File Name For Log Errors\n");

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
            if(StringCmp(*argv, "-archived.dir") == 0) {
                sscanf(*++argv, "%s", my_argu);    
                myargs[0].strvalue = StringSave(my_argu);
            }
            else if (StringCmp(*argv, "-archived.filemode") == 0) {
                sscanf(*++argv, "%s",  my_argu);
                myargs[1].strvalue = StringSave( my_argu);
            }
            if(StringCmp(*argv, "-current.dir") == 0) {
                sscanf(*++argv, "%s",  my_argu);
                myargs[2].strvalue = StringSave( my_argu);
            }
            else if (StringCmp(*argv, "-current.filemode") == 0) {
                sscanf(*++argv, "%s",  my_argu);
                myargs[3].strvalue = StringSave( my_argu);
            }
            else if (StringCmp(*argv, "-output.dir") == 0) {
                sscanf(*++argv, "%s",  my_argu);
                myargs[4].strvalue = StringSave(my_argu);
            }
            else if (StringCmp(*argv, "-output.filemode") == 0) {
                sscanf(*++argv, "%s", my_argu);
                myargs[5].strvalue = StringSave(my_argu);
            }
            else if (StringCmp(*argv, "-inputentry.list") == 0) {
                sscanf(*++argv, "%s", my_argu);
                myargs[6].strvalue = StringSave(my_argu);
            }
            else if (StringCmp(*argv, "-list.forupdate") == 0) {
                sscanf(*++argv, "%s", my_argu);
                myargs[7].strvalue = StringSave(my_argu);
            }
            else if (StringCmp(*argv, "-Logfile") == 0) {
                sscanf(*++argv, "%s", my_argu);
                myargs[8].strvalue = StringSave(my_argu);
            }
       }
    }


}
/*------------------------------------------------------*/
DeleteOrg(BioseqPtr bsp)
{
    BioSourcePtr biosource = NULL;
    ValNodePtr descr = NULL, last_descr = NULL;

    descr = bsp->descr;

    while(descr != NULL){
        if(descr->choice == bioseq_biosource_choice){
            biosource = descr->data.ptrvalue;
            biosource = BioSourceFree(biosource);
            if(last_descr != NULL) {
                last_descr->next = descr->next;
                MemFree(descr);

                return;
            }
            else {
                bsp->descr = descr->next;
                MemFree(bsp->descr);
                return;
            }
        }

        last_descr = descr;
        descr = descr->next;

    }

    return;
}
/*------------------------------------------------------*/
DoBiosourceCpy(BioSourcePtr biosource, ValNodePtr descr)
{
    BioSourcePtr biosource_cpy = NULL;

    biosource_cpy = AsnIoMemCopy(biosource, (AsnReadFunc)BioSourceAsnRead, (AsnWriteFunc)BioSourceAsnWrite);

    ValNodeAddPointer(&descr, bioseq_biosource_choice, biosource_cpy);
}
/*------------------------------------------------------*/
BiosourceCpy(BioSourcePtr biosource, BioseqPtr bsp)
{
    ValNodePtr descr = NULL;
    if(biosource == NULL) return;

    if(bsp->descr != NULL){
       DoBiosourceCpy(biosource, bsp->descr);
    }
    else {
       descr = ValNodeNew(NULL);
       DoBiosourceCpy(biosource, descr);
       bsp->descr = descr->next;
       MemFree(descr);
    } 
}
/*------------------------------------------------------*/
BioSourcePtr GetAssginedBiosource(BioseqPtr bsp)
{
    ValNodePtr descr = NULL;

    descr = bsp->descr;
    while(descr != NULL){
       if(descr->choice == bioseq_biosource_choice){
          return(descr->data.ptrvalue);
       }

       descr = descr->next;
    }

    return (NULL);
}
/*------------------------------------------------------*/
PDBSeqIdPtr GetPdbSeqId(BioseqPtr bsp)
{

   SeqIdPtr  seq_id = NULL;
   PDBSeqIdPtr pdb_seqid = NULL;

   seq_id = bsp->id;
   while(seq_id != NULL){
      if(seq_id->choice == pdb_seqid_choice){
         pdb_seqid = seq_id->data.ptrvalue;
         return (pdb_seqid);
      }

      seq_id = seq_id->next;
   }

   return (NULL);

}
/*------------------------------------------------------*/
ValNodePtr GetPdbSource(ValNodePtr descr)
{
    ValNodePtr pdbsource = NULL;
    PdbBlockPtr pdbp = NULL;

    while(descr != NULL){
       if(descr->choice == pdb_source_choice){
          pdbp = descr->data.ptrvalue;
          if(pdbp->source != NULL) {
             pdbsource = pdbp->source;
             return (pdbsource);
          }
          else {
              printf("pdb source does not exist!\n");
              return (NULL);
          }
        }
          descr = descr->next;
     }

     return (NULL);
}
/*------------------------------------------------------*/
Int2 StartBiosourceCpy(FILE *Logfile, BioseqPtr bsp_old, BioseqPtr bsp_current)
{
    PDBSeqIdPtr pdb_seqid_old = NULL, pdb_seqid_current = NULL;
    BioSourcePtr biosource_old = NULL;
    Char chain_old = ' ', chain_current = ' ';

    pdb_seqid_old = GetPdbSeqId(bsp_old);
    pdb_seqid_current = GetPdbSeqId(bsp_current);
    if(pdb_seqid_old != NULL && pdb_seqid_current != NULL){
       chain_old = pdb_seqid_old->chain;
       chain_current = pdb_seqid_current->chain;
    }

    fprintf(Logfile, "  Previous and current chain label: %c, %c\n", chain_old , chain_current);
    if(bsp_old->mol != bsp_current->mol) {
       fprintf(Logfile, "    Molecule type does not match! No orgnism copied!\n");
       return (1);
    }
    if(bsp_old->length != bsp_current->length) {
       fprintf(Logfile, "    Sequence length changed from %d to %d! \n", bsp_old->length, bsp_current->length); 
    }

    DeleteOrg(bsp_current);

    biosource_old = GetAssginedBiosource(bsp_old);
    if(biosource_old != NULL) {
       BiosourceCpy(biosource_old, bsp_current);

       fprintf(Logfile, "    Organism copied!\n", chain_old , chain_current);
       
    }
    else {
       fprintf(Logfile, "    No orgnism assigned!\n");
       return (2);
    }

    return(0);

}
/*------------------------------------------------------*/
Int2 CompareChainNumAndOrder(FILE *Logfile, SeqEntryPtr sep_old, SeqEntryPtr sep_current)
{
    BioseqSetPtr bssp_old = NULL, bssp_current = NULL;
    PDBSeqIdPtr pdb_seqid_old = NULL, pdb_seqid_current = NULL;
    SeqEntryPtr sep_old_temp = NULL, sep_current_temp = NULL;

    Int2 nchain_old = 0, nchain_current = 0;
    Char chain_old = ' ', chain_current = ' ';

    bssp_old = sep_old->data.ptrvalue;
    sep_old = bssp_old->seq_set;

    bssp_current = sep_current->data.ptrvalue;
    sep_current = bssp_current->seq_set;
    

    sep_old_temp = sep_old;
    sep_current_temp = sep_current;

    while(sep_old_temp != NULL){
       nchain_old++;
       sep_old_temp = sep_old_temp->next;
    }
 
    while(sep_current_temp != NULL){
       nchain_current++;
       sep_current_temp = sep_current_temp->next;
    }

    if(nchain_old != nchain_current) {
       fprintf(Logfile, "  Chain number doesn't match in these two seq-entries-%d vs. %d!\n", nchain_old, nchain_current);
       return (1);
    }

    while(sep_current != NULL){
       pdb_seqid_old = GetPdbSeqId(sep_old->data.ptrvalue);
       pdb_seqid_current = GetPdbSeqId(sep_current->data.ptrvalue);
        
       if(pdb_seqid_old->chain != NULL) chain_old = pdb_seqid_old->chain;
       if(pdb_seqid_current->chain != NULL) chain_current = pdb_seqid_current->chain;

       if(chain_old != chain_current) {
          fprintf(Logfile, "  Chain label doesn't match!\n");
          return(1);
       }
 
       sep_old = sep_old->next;
       sep_current = sep_current->next;
    }

    return (0);
}
/*------------------------------------------------------*/
SeqEntryOutput(SeqEntryPtr sep, CharPtr dir, CharPtr mode, CharPtr entrycode)
{
    AsnIoPtr aip = NULL;
    Char fname[120];

    StringCpy(fname, dir);

    if(StringCmp(mode, "b") == 0) { 
       StringCat(fname, "b");
       StringCat(fname, entrycode);
       StringCat(fname, ".ASN1");

       aip = AsnIoOpen(fname, "wb");
    }
    else if(StringCmp(mode, "t") == 0) {
       StringCat(fname, "b");
       StringCat(fname, entrycode);
       StringCat(fname, ".ASN1");

       aip = AsnIoOpen(fname, "w");
    }

    SeqEntryAsnWrite(sep, aip, NULL);
    
    aip = AsnIoClose(aip);
}
/*------------------------------------------------------*/
Int2 MatchOrg(Char code[120], FILE *Logfile)
{
    Char fname[120];
    AsnIoPtr aip1 = NULL, aip2 = NULL;

    SeqEntryPtr sep_old = NULL, sep_current= NULL;
    SeqEntryPtr sep_old_temp = NULL, sep_current_temp= NULL;
    BioseqPtr bsp_old = NULL, bsp_current= NULL;
    BioseqSetPtr bssp_old = NULL, bssp_current = NULL;

    ValNodePtr pdbsource_old = NULL, pdbsource_current = NULL;

    Int2 returncode = 0, org_miss = 0, nchain = 0;
    Boolean allright = FALSE;

    if(strcmp(myargs[1].strvalue, "t") == 0){
        StringCpy(fname, myargs[0].strvalue);
        StringCat(fname, "a");
        StringCat(fname, code);
        StringCat(fname, ".ASN1");

        if(!(aip1 = AsnIoOpen(fname, "r"))) {
           printf("Seq-Entry file %s can not be opened!\n", fname);
           fprintf(Logfile, "  Seq-Entry file %s can not be opened!\n", fname);
           return(2);
         }

         sep_old = SeqEntryAsnRead(aip1, NULL);
         aip1 = AsnIoClose(aip1);
    }
    else if(strcmp(myargs[1].strvalue, "b") == 0){
        StringCpy(fname, myargs[0].strvalue);
        StringCat(fname, "b");
        StringCat(fname, code);
        StringCat(fname, ".ASN1");

        if(!(aip1 = AsnIoOpen(fname, "rb"))) {
           printf("Seq-Entry file %s can not be opened!\n", fname);
           fprintf(Logfile, "Seq-Entry file %s can not be opened!\n", fname);
           return(2);
        }

        sep_old = SeqEntryAsnRead(aip1, NULL);
        aip1 = AsnIoClose(aip1);
    }

    if(strcmp(myargs[3].strvalue, "t") == 0){
        StringCpy(fname, myargs[2].strvalue);
        StringCat(fname, "a");
        StringCat(fname, code);
        StringCat(fname, ".ASN1");

        if(!(aip1 = AsnIoOpen(fname, "r"))) {
           printf("Seq-Entry file %s can not be opened!\n", fname);
           fprintf(Logfile, "Seq-Entry file %s can not be opened!\n", fname);
           return(2);
        }

        sep_current = SeqEntryAsnRead(aip1, NULL);
        aip1 = AsnIoClose(aip1);
    }
    else if(strcmp(myargs[3].strvalue, "b") == 0){
        StringCpy(fname, myargs[2].strvalue);
        StringCat(fname, "b");
        StringCat(fname, code);
        StringCat(fname, ".ASN1");

        if(!(aip1 = AsnIoOpen(fname, "rb"))) {
           printf("Seq-Entry file %s can not be opened!\n", fname);
           fprintf(Logfile, "Seq-Entry file %s can not be opened!\n", fname);
           return(2);
        }

        sep_current = SeqEntryAsnRead(aip1, NULL);
        aip1 = AsnIoClose(aip1);
    }

    if(sep_old->choice == 1 && sep_old->choice != sep_current->choice ){
        fprintf(Logfile, "    Chain number doesn't match in these two seq-entries-single chain vs. multiple chain !\n");
        return(1);
    }
    else if(sep_old->choice == 2 && sep_old->choice != sep_current->choice ){
        fprintf(Logfile, "    Chain number doesn't match in these two seq-entries-multiple chain vs. single chain!\n");
        return(1);
    }

    if(sep_old->choice == 1 && sep_current->choice == 1){   /* both single chain */
       bsp_old = sep_old->data.ptrvalue;
       bsp_current = sep_current->data.ptrvalue;

       pdbsource_old = GetPdbSource(bsp_old->descr);
       pdbsource_current = GetPdbSource(bsp_current->descr);


       if(StringCmp(pdbsource_old->data.ptrvalue, pdbsource_current->data.ptrvalue) == 0){
          returncode = StartBiosourceCpy(Logfile, sep_old->data.ptrvalue, sep_current->data.ptrvalue);
          if(returncode != 1){
             SeqEntryOutput(sep_current, myargs[4].strvalue, myargs[5].strvalue, code );
             if(returncode == 0) fprintf(Logfile, "  Organism copied for this seq-entry! Seq-entry written to output directory!\n");
             else fprintf(Logfile, "  Seq-entry written to output directory!\n");

             sep_old = SeqEntryFree(sep_old);
             sep_current = SeqEntryFree(sep_current);

             return (0);
          }
          else return (1);

       }
       else {
          fprintf(Logfile, "  Biosource text changed!\n");
          fprintf(Logfile, "    Previous: %s\n", pdbsource_old->data.ptrvalue);
          fprintf(Logfile, "    Current:  %s\n", pdbsource_current->data.ptrvalue); 

          sep_old = SeqEntryFree(sep_old);
          sep_current = SeqEntryFree(sep_current);

          return(1);
       }
    }
    else {           /* multiple chain  */

       if(CompareChainNumAndOrder(Logfile, sep_old, sep_current) == 1) return (1);

       bssp_old = sep_old->data.ptrvalue;
       pdbsource_old = GetPdbSource((ValNodePtr) bssp_old->descr);

       bssp_current = sep_current->data.ptrvalue;
       pdbsource_current = GetPdbSource((ValNodePtr) bssp_current->descr);

       if(StringCmp(pdbsource_old->data.ptrvalue, pdbsource_current->data.ptrvalue) != 0){
          fprintf(Logfile, "  Biosource text changed!\n");
          fprintf(Logfile, "    Previous: %s\n", pdbsource_old->data.ptrvalue);
          fprintf(Logfile, "    Current:  %s\n", pdbsource_current->data.ptrvalue); 
          return(1);
       }

       sep_old_temp = bssp_old->seq_set;
       sep_current_temp = bssp_current->seq_set;

       allright = TRUE;
       org_miss = 0; 
       nchain = 0;
       
       while(sep_current_temp != NULL){
          returncode = StartBiosourceCpy(Logfile, sep_old_temp->data.ptrvalue, sep_current_temp->data.ptrvalue);
          if(returncode == 1){    
             allright = FALSE;      
          }
          if(returncode == 2) org_miss++;

          nchain++;
          sep_old_temp = sep_old_temp->next;
          sep_current_temp = sep_current_temp->next;
       }

       if(allright){
          SeqEntryOutput(sep_current, myargs[4].strvalue, myargs[5].strvalue, code);
          if(org_miss == nchain) fprintf(Logfile, "  Seq-entry written to output directory!\n");
          else if(org_miss >0 && org_miss< nchain) fprintf(Logfile, "  Organism copied for some chains of this seq-entry! Seq-entry written to output directory!\n");
          else fprintf(Logfile, "  Organism copied for this seq-entry! Seq-entry written to output directory!\n");

          sep_old = SeqEntryFree(sep_old);
          sep_current = SeqEntryFree(sep_current);

          return (0);
       }
       else return (1);
   }

}
/*----------------   Main Starts Here ------------------*/
Int2 Main()
{
   
    Char fname[120], code[120];
    FILE *CodeList, *ListToBeUpdated, *Logfile;

    Init();
    InitArg(GetArgc(), GetArgv());

    CodeList = FileOpen(myargs[6].strvalue, "r");
    if(CodeList == NULL){
       printf("Failed to read file: %s \n", myargs[6].strvalue);
       exit(0);;
    }

    ListToBeUpdated = FileOpen(myargs[7].strvalue, "w");
    if(ListToBeUpdated == NULL){
       printf("Failed to read file: %s \n", myargs[7].strvalue);
       exit(0);
    }
       
    Logfile = FileOpen(myargs[8].strvalue, "w");
    if(Logfile == NULL){
       printf("Failed to read file: %s \n", myargs[8].strvalue);
       exit(0);
    }


    while(fscanf(CodeList, "%s", code) != EOF){
printf("%s\n", code);
       fprintf(Logfile,"\n");
       fprintf(Logfile,"%s: \n", code);
       if(MatchOrg(code, Logfile) == 1){
          fprintf(Logfile, "  Need to assign taxonomy for this seq-entry\n");
          fprintf(ListToBeUpdated, "%s\n", code);
       }
    }


    FileClose(CodeList);
    FileClose(ListToBeUpdated);
    FileClose(Logfile);
}
