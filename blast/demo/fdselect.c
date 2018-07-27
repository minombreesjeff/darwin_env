/* $Id: fdselect.c,v 6.3 2000/11/22 21:10:12 shavirin Exp $ */
/*****************************************************************************

  
                          PUBLIC DOMAIN NOTICE
              National Center for Biotechnology Information

    This software/database is a "United States Government Work" under the
    terms of the United States Copyright Act.  It was written as part of    
    the author's official duties as a United States Government employee
    and thus cannot be copyrighted.  This software/database is freely
    available to the public for use. The National Library of Medicine and
    the U.S. Government have not placed any restriction on its use or
    reproduction.

    Although all reasonable efforts have been taken to ensure the accuracy
    and reliability of the software and data, the NLM and the U.S.
    Government do not and cannot warrant the performance or results that
    may be obtained by using this software or data. The NLM and the U.S.
    Government disclaim all warranties, express or implied, including
    warranties of performance, merchantability or fitness for any
    particular purpose.

    Please cite the author in any work or product based on this material.

   ***************************************************************************

   File Name:  fdfilter.c

   Author:  Sergei B. Shavirin
   
   Version Creation Date: 09/13/99

   $Revision: 6.3 $

   File Description:  Create few subsets of FASTA database.

   $Log: fdselect.c,v $
   Revision 6.3  2000/11/22 21:10:12  shavirin
   Added tax_id parameter into function FDBAddBioseq.

   Revision 6.2  2000/03/13 18:37:37  madden
   Added insert_ctrlA Boolean to readdb_get_bioseq_ex

   Revision 6.1  1999/09/13 16:20:00  shavirin
   Initial version.


*****************************************************************************/
#include <ncbi.h>
#include <readdb.h>
#include <ncbiwww.h>

/* We will use regular WWW encoding of request to make specific
   filtering using database information file 

   The most general query string will look like:

   taxid=555,666,777&owner=5,6,7&div=AAA,BBB,CCC

   */

#define TAXID_LABEL  "taxid"
#define OWNER_LABEL  "owner"
#define DIV_LABEL    "div"
#define DBNAME_LABEL "dbname"

#define MAX_SR_ELEMENTS   36
#define MAX_FLT_DATABASES 64

typedef struct _SR_Info
{
    Int4 taxid[MAX_SR_ELEMENTS];
    Int4 owner[MAX_SR_ELEMENTS];
    Char div[MAX_SR_ELEMENTS][4];
    Char dbname[64];
} SR_Info, PNTR SR_InfoPtr;

typedef struct gilist
{
    Int4    count;
    Int4    allocated;
    Int4Ptr seq_num;
} GiList, *GiListPtr;

typedef struct _NewDBdata
{
    CharPtr dbname;
    GiListPtr glp;
} NewDBdata, PNTR NewDBdataPtr;

#define NUMARG 8

Args flt_args[NUMARG] = {
    { "Title for output database file", 
      NULL, NULL, NULL, TRUE, 't', ARG_STRING, 0.0, 0, NULL},
    {"Input file for the filtering (this parameter must be set)",
     NULL, NULL,NULL,FALSE,'i',ARG_FILE_IN, 0.0,0,NULL},
    {"Configuration file for database subsets creation",
     "fdselect.cfg", NULL,NULL,TRUE,'c',ARG_FILE_IN, 0.0,0,NULL},
    {"Input file with a list of gis",
     NULL, NULL,NULL,TRUE,'g',ARG_FILE_IN, 0.0,0,NULL},
    {"Create sparse indexes in the filtered database",
     "F", NULL,NULL,TRUE,'s',ARG_BOOLEAN, 0.0,0,NULL},
    {"Query string for creating database subset",
     NULL, NULL,NULL,TRUE,'q',ARG_STRING, 0.0,0,NULL},
    {"Input database is protein",
     "T", NULL,NULL,TRUE,'p',ARG_BOOLEAN, 0.0,0,NULL},
    {"Logfile name:",
     "fdselect.log", NULL,NULL,TRUE,'l',ARG_FILE_OUT, 0.0,0,NULL},
};

#define FLT_Input  flt_args[1].strvalue
#define IS_Protein flt_args[6].intvalue


void FDB_optionsFree(FDB_optionsPtr options)
{
    if(options == NULL)
        return;
    
    MemFree(options->db_title);
    MemFree(options->db_file);
    MemFree(options->LogFileName);
    MemFree(options);
    
    return;
}

Boolean SRReadCharData(CharPtr buffer, CharPtr PNTR div_in)
{
    CharPtr tmp, ch, ch2;
    Int4 j;
    Char div[MAX_SR_ELEMENTS][4];

    tmp = StringSave(buffer);
    MemSet(div, NULLB, sizeof(div));

    for(ch2 = tmp, j = 0; j < MAX_SR_ELEMENTS; j++) {
        
        if((ch = StringChr(ch2, ',')) == NULL) {
            StringNCpy(div[j], ch2, 3);
            break;
        }
        
        *ch = NULLB;
        ch++;
        StringNCpy(div[j], ch2, 3);
        ch2 = ch;
    }

    MemCpy(div_in, div, sizeof(div));

    MemFree(tmp);
    
    return TRUE;
}

Boolean SRReadIntData(CharPtr buffer, Int4Ptr id)
{
    CharPtr tmp, ch, ch2;
    Int4 j;
    tmp = StringSave(buffer);
    
    for(ch2 = tmp, j = 0; j < MAX_SR_ELEMENTS; j++) {
        
        if((ch = StringChr(ch2, ',')) == NULL) {
            id[j] = atol(ch2);
            id[j+1] = -1; /* Terminating character */
            break;
        }
        
        *ch = NULLB;
        ch++;
        id[j] = atol(ch2);
        ch2 = ch;
    }

    MemFree(tmp);
    
    return TRUE;
}

SR_InfoPtr SRReadSRInfo(CharPtr buffer)
{
    WWWInfoPtr info;
    WWWInfoDataPtr info_data;
    SR_InfoPtr srip;
    CharPtr chptr;

    info_data = (WWWInfoDataPtr) MemNew(sizeof(WWWInfoData));
    info_data->query = StringSave(buffer);
    info_data->entries = WWWGetEntries(&info_data->num_entries, 
                                       info_data->query, FALSE);
    info = (VoidPtr) info_data;

    srip = MemNew(sizeof(SR_Info));
    
    if((chptr = WWWGetValueByName(info, TAXID_LABEL)) != NULL)
        SRReadIntData(chptr, srip->taxid);
    if((chptr = WWWGetValueByName(info, OWNER_LABEL)) != NULL)
        SRReadIntData(chptr, srip->owner);
    if((chptr = WWWGetValueByName(info, DIV_LABEL)) != NULL)
        SRReadCharData(chptr, (CharPtr PNTR) srip->div);
    if((chptr = WWWGetValueByName(info, DBNAME_LABEL)) != NULL)
        StringCpy(srip->dbname, chptr);

    /* If no conditions exists - this is an error */
    if(srip->taxid[0] <= 0 && srip->owner[0] <= 0 && **srip->div == NULLB) {
        ErrPostEx(SEV_ERROR, 0, 0, 
                  "No valid conditions exists in query string");
        MemFree(srip);
        srip = NULL;
    }
    WWWInfoFree(info);
    return srip;
}

FDB_optionsPtr FDB_CreateCLOptions(Boolean is_prot)
{
    FDB_optionsPtr options;
    Char buffer[128];

    options = MemNew(sizeof(FDB_options));
    
    options->db_title = StringSave(flt_args[0].strvalue);

    sprintf(buffer, "%s.flt", FLT_Input);
    options->db_file = StringSave(buffer);

    options->LogFileName = StringSave(flt_args[7].strvalue);
    options->is_protein = is_prot;
    options->parse_mode = TRUE;
    options->isASN = FALSE;
    options->asnbin = FALSE;
    options->is_seqentry = FALSE;
    options->base_name = NULL;
    options->dump_info = FALSE;
    options->sparse_idx = flt_args[4].intvalue;

    return options;
}

#define GI_ALLOC_CHUNK 1024

SeqIdPtr MySeqIdFree(SeqIdPtr sip)
{
    SeqIdPtr sip_tmp;
    do {
        sip_tmp = sip->next;
        SeqIdFree(sip);
        sip = sip_tmp;
    } while(sip != NULL);
    
    return NULL;
}

/* Functions used in filtering by gi number */
GiListPtr GiListNew(void)
{
    GiListPtr glp;

    glp = MemNew(sizeof(GiList));
    glp->allocated = GI_ALLOC_CHUNK;
    glp->seq_num = MemNew(sizeof(Int4) * glp->allocated);
    glp->count = 0;
    
    return glp;
}
void GiListFree(GiListPtr glp)
{
    if(glp == NULL)
        return;
    
    MemFree(glp->seq_num);
    MemFree(glp);
    return;
}

Boolean ReadGiList(ReadDBFILEPtr rdfp, GiListPtr glp, CharPtr filename)
{
    FILE *fd;
    Int4 gi, retvalue, seqnum;

    if((fd = FileOpen(filename, "r")) == NULL)
        return FALSE;

    while((retvalue = fscanf(fd, "%d", &gi)) != EOF) {
        if(retvalue == 0) continue;
        
        if(glp->count >= glp->allocated) {
            glp->allocated += GI_ALLOC_CHUNK;
            glp->seq_num = Realloc(glp->seq_num, 
                                   sizeof(Int4) * glp->allocated);
        }

        seqnum = readdb_gi2seq(rdfp, gi);
            
        if(seqnum < 0) {
            ErrPostEx(SEV_WARNING, 0,0, "Gi %d is not found", gi);
            continue;
        }

        glp->seq_num[glp->count] = seqnum;
        glp->count++;
    }

    FileClose(fd);

    return TRUE;
}
/* Here we will check, that data[2] == tax_id, 
   data[3] == owner, div=div */
Boolean CheckSRCondition(SR_InfoPtr srip, Int4Ptr data, CharPtr div)
{
    Int4    i;
    CharPtr chptr;
    Boolean  cond_ok = FALSE;

    /* checking tax_id */

    if(srip->taxid[0] > 0) { /* At least one element exists */
        cond_ok = FALSE;
        for(i = 0; srip->taxid[i] > 0 && i < MAX_SR_ELEMENTS; i++) {
            if(data[2] == srip->taxid[i]) {
                cond_ok = TRUE;
                break;
            }
        }
        if(cond_ok == FALSE)
            return FALSE;
    }

    /* checking owner */

    if(srip->owner[0] > 0) { /* At least one element exists */
        cond_ok = FALSE;
        for(i = 0; srip->owner[i] > 0 && i < MAX_SR_ELEMENTS; i++) {
            if(data[3] == srip->owner[i]) {
                cond_ok = TRUE;
                break;
            }
        }
        if(cond_ok == FALSE)
            return FALSE;
    }

    /* checking division */

    if(*srip->div[0] != NULLB) { /* At least one element exists */
        cond_ok = FALSE;
        for(i = 0; *srip->div[i] != NULLB && i < MAX_SR_ELEMENTS; i++) {
            if(!StringCmp(div, srip->div[i])) {
                cond_ok = TRUE;
                break;
            }
        }
        if(cond_ok == FALSE)
            return FALSE;
    }
    
    return TRUE;
}

Boolean FDGetGiListByQuery(ReadDBFILEPtr rdfp, SR_InfoPtr srip, 
                           GiListPtr glp, CharPtr filename)
{ 
    FILE *fd;
    Int4 length, data[7];
    Char div[32];
    Char buffer[1024];
    
    if((fd = FileOpen(filename, "r")) == NULL) {
        ErrPostEx(SEV_ERROR, 0,0, "Unable to open input info file");
        return FALSE;
    }
    
    length = sizeof(buffer);
    while(fgets(buffer, length, fd) != NULL) {
        sscanf(buffer, "%d %d %d %d %s %d %d %d",
               &data[0], &data[1], &data[2], &data[3],
               div, &data[4], &data[5], &data[6]);
        
        /* If line agree with condition seq_num added to the list */
        if(CheckSRCondition(srip, data, div)) {
            if(glp->count >= glp->allocated) {
                glp->allocated += GI_ALLOC_CHUNK;
                glp->seq_num = Realloc(glp->seq_num, 
                                       sizeof(Int4) * glp->allocated);
            }
            glp->seq_num[glp->count] = data[0];
            glp->count++;
        }
    }
    
    FileClose(fd);
    
    return TRUE;
}

VoidPtr NewDBThread(VoidPtr data)
{
    NewDBdataPtr ndbp;
    ReadDBFILEPtr rdfp;
    FDB_optionsPtr options;
    FormatDBPtr	fdbp;
    BioseqPtr bsp;
    Int4 i, seqnum;

    if((ndbp = data) == NULL)
        return NULL;

    if((rdfp = readdb_new (FLT_Input, IS_Protein)) == NULL) {
        ErrPostEx(SEV_ERROR, 0, 0, 
                  "Failure to intialise database %s", FLT_Input);
        return NULL;
    }
    
    /* ---------------------------------------------- */
    /* ------- Initializing thread options  --------- */
    /* ---------------------------------------------- */

    if((options = FDB_CreateCLOptions(IS_Protein)) == NULL)
        return NULL;
    
    MemFree(options->db_file);
    options->db_file = StringSave(ndbp->dbname);
    
    /* ---------------------------------------------- */
    /* ----- Initializing formatdb structure  ------- */
    /* ---------------------------------------------- */
    
    if ((fdbp = FormatDBInit(options)) == NULL)
        return NULL;
    
    /* ---------------------------------------------- */
    /* ---------------- Main loop ------------------- */
    /* ---------------------------------------------- */
    
    for(i = 0; i < ndbp->glp->count; i++) {
        
        if((seqnum = ndbp->glp->seq_num[i]) == -1)
            continue;
        
        bsp = readdb_get_bioseq_ex(rdfp, seqnum, FALSE, FALSE);        
        FDBAddBioseq(fdbp, bsp, 0);

        SeqIdSetFree(bsp->id);
        BioseqFreeComponents(bsp);
        MemFree(bsp);

        /* BioseqFree(bsp); */
    }

    GiListFree(ndbp->glp);
    
    if(FormatDBClose(fdbp))
        return NULL;
    
    readdb_destruct(rdfp);
    FDB_optionsFree(options);
    
    return NULL;
}

NewDBdataPtr GetNdprByString(CharPtr string, ReadDBFILEPtr rdfp, 
                             CharPtr indname)
{
    NewDBdataPtr ndbp;
    SR_InfoPtr srip;
    
    if((srip = SRReadSRInfo(string)) == NULL)
        return NULL;
    
    ndbp = MemNew(sizeof(NewDBdata));
    ndbp->glp = GiListNew();
    
    if(!FDGetGiListByQuery(rdfp, srip, ndbp->glp, indname))
        return NULL;
    
    /* If database name set - replacing default one */
    if(*srip->dbname != NULLB) {
        ndbp->dbname = StringSave(srip->dbname);
    }
    MemFree(srip);
    return ndbp;
}
void TrimSpaces(CharPtr buffer)
{
    CharPtr chptr;
    Int4 i;
    
    if(buffer == NULL)
        return;
    
    for(chptr = buffer, i = 0; *chptr != NULLB; chptr++) {
        if(IS_WHITESP(*chptr))
            continue;
        buffer[i] = *chptr;
        i++;
    }
    buffer[i] = NULLB;
    return;
}

Int2 Main(void)
{
    Int4 i, glp_count, buflen;
    NewDBdataPtr ndbp[MAX_FLT_DATABASES];
    Char indname[128], buffer[1024];
    FILE *fd;
    ReadDBFILEPtr rdfp;
    
    if ( !GetArgs ("fdselect", NUMARG, flt_args) )
        return NULL;
    
    if ( !ErrSetLog (flt_args[7].strvalue) ) { /* Logfile */
        ErrShow();
    } else {
        ErrSetOpts (ERR_CONTINUE, ERR_LOG_ON);
    }

    /* ---------------------------------------------- */
    /* ------ Initializing readdb structures -------- */
    /* ---------------------------------------------- */

    if((rdfp = readdb_new (FLT_Input, IS_Protein)) == NULL) {
        ErrPostEx(SEV_ERROR, 0, 0, 
                  "Failure to intialise database %s", FLT_Input);
        return NULL;
    }
        
    sprintf(indname, "%s.%cdi", FLT_Input, IS_Protein? 'p' : 'n');
    
    if(flt_args[3].strvalue != NULL) {/* This is list of gis */
        
        ndbp[0] = MemNew(sizeof(NewDBdata));
        ndbp[0]->glp = GiListNew();
        glp_count = 1;
        
        if(!ReadGiList(rdfp, ndbp[0]->glp, flt_args[2].strvalue))
            return -1;
        
    } else if(flt_args[5].strvalue != NULL) {/* This is string for gis */
        
        TrimSpaces(flt_args[5].strvalue);
        
        ndbp[0] = GetNdprByString(flt_args[5].strvalue, rdfp, indname);
        glp_count = 1;

    } else {

        /* Default behaviour is to read and interprete every line of the
           configuration file */
        
        if((fd = FileOpen(flt_args[2].strvalue, "r")) == NULL) {
            ErrPostEx(SEV_ERROR, 0, 0, "Failure to open config file %s",
                      flt_args[2].strvalue);
        }

        buflen = sizeof(buffer);
        for(glp_count = 0; fgets(buffer, buflen, fd) != NULL && 
                glp_count < MAX_FLT_DATABASES;) {
            
            if(*buffer == '#')
                continue;

            TrimSpaces(buffer);
            
            ndbp[glp_count] = GetNdprByString(buffer, rdfp, indname);
            glp_count++;
        }
    }
    
    readdb_destruct(rdfp);

    for(i = 0; i < glp_count; i++) {
        NlmThreadCreate(NewDBThread, (VoidPtr) ndbp[i]);
    }

    NlmThreadJoinAll();

    return 0;
}
 
