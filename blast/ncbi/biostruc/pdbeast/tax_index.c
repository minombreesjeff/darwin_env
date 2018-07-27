/*
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
* File Name:  tax_index.c
*
* Author:  Yanli Wang
*
* Version Creation Date:   06/01/1997
*
* File Description:
* Creat a summary table for listing gis, mmdbId, taxonomy Id and name...
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: tax_index.c,v $
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
#include <objloc.h>     /* SeqIdPtr definition */
#include <objmmdb1.h>
#include <objmmdb2.h>
#include <objmmdb3.h>
#include <mmdbapi1.h>

Args myargs[6] = {
    {"Text Input Biostruc File Directory","/net/keystone/usr/people/ywang/work/taxonomy/Biostruc_text/", NULL,NULL,TRUE,'h',ARG_STRING,0.0,0,NULL},
    {"Binary Input Biostruc File Directory","/net/clobber/usr/people8/bryant/MMDB/", NULL,NULL,TRUE,'i',ARG_STRING,0.0,0,NULL},
    {"Entry Code List Path & File Name","./all.lis", NULL,NULL,TRUE,'h',ARG_STRING,0.0,0,NULL},
    {"File Path & Name For the Table Containing Taxonomy Information","./tax.table", NULL,NULL,TRUE,'i',ARG_STRING,0.0,0,NULL},
    {"File Path & Name For the Table Containing Unassigned Taxonomy Information","./unassign.lis", NULL,NULL,TRUE,'n',ARG_STRING,0.0,0,NULL},
    {"Log errors to file named:","log.mmdb.taxonomy",NULL,NULL,TRUE,'l',ARG_FILE_OUT, 0.0,0,NULL} };

/* choice numbers needed here  */

/* #define BiomolDescr_name 1 */  /* chain choice number  */
/* #define BiomolDescr_molecule_type 9 */
/* BiomolDescr_organism */ 
/* #define BiostrucId_mmdb_id 1 */ 
/* #define SEQID_GI ( (Uint1)12) */

#define gi_choice_num 12  /* from objloc.h  */

FILE *afp, *tax_index, *no_tax_index;
Boolean TextInStr = FALSE, TextOutStr = FALSE;
BiostrucPtr  root_bstrucp;
AsnIoPtr aip1, aip2, aip3;
Char EntryCode[12];
Int4 mmdb_id, gi;
Int2 current_chain;
/*----------------   Initialize --------------------------*/
void Init()
{
    if(!tax1_init()) {
        printf("Can not make connection to taxonomy server\n");
        exit(1);
    }

    objmmdb1AsnLoad();
    objmmdb2AsnLoad();
    objmmdb3AsnLoad();
}
/*-------------------------------------------------*/
void PrintArg(){

       printf("\t -aid.str       Text Input Biostruc File Directory\n");
       printf("\t -bid.str       Binary Input Biostruc File Directory\n");
       printf("\t -ListFile      Entry Code List Path & File Name\n");
       printf("\t -AssignFile    File Path & Name For the Table Containing Taxonomy Information\n");
       printf("\t -UnAssignFile  File Path & Name For the Table Containing Unassigned Taxonomy Information\n");
       printf("\t -im.str        Input Mode of Biostruc File: 'a' for text / 'b' for binary\n");
       printf("\t -Logfile       File Name For Log Errors\n");

       return;

}
/*---------------    Get Arguments ------------------*/
void InitArg(argc, argv)
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
            if(StringCmp(*argv, "-aid.str") == 0) {
                sscanf(*++argv, "%s", my_argu);
                myargs[0].strvalue = StringSave(my_argu);
            }
            else if (StringCmp(*argv, "-bid.str") == 0) {
                sscanf(*++argv, "%s", my_argu);
                myargs[1].strvalue = StringSave(my_argu);
            }
            if(StringCmp(*argv, "-ListFile") == 0) {
                sscanf(*++argv, "%s", my_argu);
                myargs[2].strvalue = StringSave(my_argu);
            }
            else if (StringCmp(*argv, "-AssignFile") == 0) {
                sscanf(*++argv, "%s", my_argu);
                myargs[3].strvalue = StringSave(my_argu);
            }
            else if (StringCmp(*argv, "-UnAssignFile") == 0) {
                sscanf(*++argv, "%s", my_argu);
                myargs[4].strvalue = StringSave(my_argu);
            }
            else if (StringCmp(*argv, "-Logfile") == 0) {
                sscanf(*++argv, "%s", my_argu);
                myargs[5].strvalue = StringSave(my_argu);
            }
            else if (StringCmp(*argv, "-im") == 0) {
                sscanf(*++argv, "%s", str);
                if(StringCmp(str, "a") == 0) TextInStr = TRUE;
                else if(StringCmp(str, "b") == 0)TextInStr = FALSE;
            }
       }
    }

}
/*------------------------------------------------------*/
void AccessBiostruc( BiostrucPtr my_bstrucp)
{

/*  BiostrucGraphPtr bgp;   */
    MoleculeGraphPtr mgp;
    OrgRefPtr org;
    BioSourcePtr biosource;
    ValNodePtr id, seq_id, descr;
    CharPtr chainp;
    Char orgname[120], common_name[120];
    Int4 molecule_type, tax_id = 0, tax_id_ori = 0;
    ValNodePtr db = NULL;
    DbtagPtr dtp = NULL;
    ObjectIdPtr oip = NULL;
    Boolean TaxExist = FALSE;

    if(my_bstrucp->id != NULL) id = my_bstrucp->id;
    while(id != NULL){
        if(id->choice == BiostrucId_mmdb_id) {
            mmdb_id = id->data.intvalue; 
            break;
        }

        id = id->next;
    }

    mgp = my_bstrucp->chemical_graph->molecule_graphs;

    while(mgp != NULL){
        if(mgp->descr != NULL){
            descr = mgp->descr;
            while(descr != NULL){
                if(descr->choice == BiomolDescr_name){
                    chainp = descr->data.ptrvalue;
                    current_chain = *chainp;
                }
                if(descr->choice == BiomolDescr_molecule_type) molecule_type = descr->data.intvalue;     
                if(descr->choice == BiomolDescr_organism) {
                    biosource = descr->data.ptrvalue;
                    org = biosource->org;
                    
       /* check up taxonomy record consistency */
                    db = org->db;
                    while(db){
                       dtp = db->data.ptrvalue;
                       if(strcmp(dtp->db, "taxon")==0){
                         oip = dtp->tag;
                         tax_id_ori = oip->id;
                       }
                       db = db->next;
                    }

                    if(tax_id_ori == 0){
                      fprintf(afp, "Warning: %4s %c--taxonomy Id in current record is 0 or not found!\n", EntryCode, current_chain);
                    }

                    tax_id = tax1_getTaxIdByOrgRef(org);  
                    /* tax_id = tax1_getTaxIdByName(org->taxname); */

                    if(tax_id == 0 && tax_id_ori > 0) {
                       fprintf(afp, "Warning: %4s %c--taxonomy Id look-up with tax1_getTaxIdByOrgRef(org) does not find any node, the taxonomy Id in the original record is retained!\n", EntryCode, current_chain);
                       tax_id = tax_id_ori;
                    }
                    else if(tax_id < 0 && tax_id_ori > 0){
                       fprintf(afp, "Warning: %4s %c--taxonomy Id look up using tax1_getTaxIdByOrgRef(org) gives negative value, %d, meaning multiple taxon records with same name found, the taxonomy Id in original record, %d, is retained!\n", EntryCode, current_chain, tax_id, tax_id_ori);
                       tax_id = tax_id_ori;
                    }
                    else if(tax_id > 0 &&tax_id != tax_id_ori){
                       fprintf(afp, "Warning: %4s %c--taxonomy Id, %d, returned by tax1_getTaxIdByOrgRef(org) conflicts with the one, %d, in the original record, taxonomy Id swapped!\n", EntryCode, current_chain, tax_id, tax_id_ori);
                    }

                    if(org->common != NULL) StringCpy(common_name, org->common);
                    else StringCpy(common_name, "    ");     

                    TaxExist = TRUE;
                }

                descr = descr->next;

            }

            seq_id = mgp->seq_id;
            gi = 000000;
            while(seq_id != NULL){
                if(seq_id->choice == SEQID_GI){
                    gi = seq_id->data.intvalue;
                    break;
                }
                seq_id = seq_id->next;
            } 

            if( molecule_type  == Molecule_type_protein || molecule_type  == Molecule_type_dna || molecule_type  == Molecule_type_rna || molecule_type  == Molecule_type_other_biopolymer) {
                if(TaxExist) fprintf(tax_index, "%6d  %4s  %c  %10d  %8d  %s\n", mmdb_id, EntryCode, current_chain, gi, tax_id, org->taxname);
                else {
                  fprintf(no_tax_index, " %-6d %4s %c %-8d\n", mmdb_id, EntryCode, current_chain, gi);
                }
            }

        }
      
        mgp = mgp->next;
        TaxExist = FALSE;

    }


    return;

}
/*------------------------------------------------------*/
void GoToBiostruc()
{

    Char fname[120];
    CharPtr option;


    if(TextInStr){
        StringCpy(fname, myargs[0].strvalue);
        StringCat(fname, "a");
    }
    else{
        StringCpy(fname, myargs[1].strvalue);
        StringCat(fname, "b");
    }

    StringCat(fname, EntryCode);

    if(TextInStr){
        if(!(aip2 = AsnIoOpen(fname, "r"))) {
            printf("Can not open Input Biostruc File %s\n", fname);
            return;   
        }
    }
    else {
        if(!(aip2 = AsnIoOpen(fname, "rb"))) {
            printf("Can not open Input Biostruc File %s\n", fname);
            return;   
        }
    }

    root_bstrucp = BiostrucAsnRead(aip2, NULL);

    aip2 = AsnIoClose(aip2);

    AccessBiostruc(root_bstrucp);

    root_bstrucp = BiostrucFree(root_bstrucp);

    return;

}
/*----------------   Main Starts Here ------------------*/
Int2 Main()
{
    Char fname[120], code[120];
    FILE *CodeList;

    Init();
    InitArg(GetArgc(), GetArgv());

    afp = FileOpen(myargs[5].strvalue, "w");
    tax_index = FileOpen(myargs[3].strvalue, "w");
    fprintf(tax_index, "#              Organism Summary For Structure Database\n");
    fprintf(tax_index, "#\n");
    fprintf(tax_index, "#col_1: MMDB ID\n");
    fprintf(tax_index, "#col_2: PDB Code\n");
    fprintf(tax_index, "#col_3: Chain ID\n");
    fprintf(tax_index, "#col_4: GI\n");
    fprintf(tax_index, "#col_5: Tax ID\n");
    fprintf(tax_index, "#col_6: Scientific Organism Name\n");
    fprintf(tax_index, "#-----------------------------------------------------------------------\n");
    fprintf(tax_index, "#col_1 col_2 col_3   col_4      col_5       col_6\n");
    fprintf(tax_index, "#-----------------------------------------------------------------------\n");
/*  fprintf(tax_index, "<MMDB.ID>  <PDB.ID>  <PDB.CHAIN>  <GI>  <TAXON.ID>    <Scientific name>\n");
    fprintf(tax_index, "-----------------------------------------------------------------------\n");  */
    no_tax_index = FileOpen(myargs[4].strvalue, "w");

    CodeList = FileOpen(myargs[2].strvalue, "r");
    if(CodeList == NULL){
        printf("Problem reading file: %s \n", fname);
        return;
    }

    while(fscanf(CodeList, "%s", EntryCode) != EOF){
        fprintf(afp, "\tEntry: %s\n", EntryCode);
printf("%s\n", EntryCode);

        GoToBiostruc(); 
    }
    fprintf(tax_index, "#-----------------------------------------------------------------------\n");
    FileClose(afp);
    FileClose(tax_index);
    FileClose(no_tax_index);

}

