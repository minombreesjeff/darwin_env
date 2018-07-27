/* $Id: qbatch.c,v 6.11 2004/08/13 02:01:21 beloslyu Exp $
* ===========================================================================
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
* File Name:  $RCSfile: qbatch.c,v $
*
* Author:  Sergei Shavirin
*
* Version Creation Date: 05/04/2000
*
* $Revision: 6.11 $
*
* File Description:
*         WWW and Command-line Batch Entrez using Entre2 and ID1
*
* $Log: qbatch.c,v $
* Revision 6.11  2004/08/13 02:01:21  beloslyu
* Changes for FreeBSD
*
* Revision 6.10  2002/10/24 17:37:36  ucko
* Kludge around Darwin's lack of ctime_r.
*
* Revision 6.9  2001/03/19 16:18:17  beloslyu
* fix the args for ctime_r for OSF1 on alpha
*
* Revision 6.8  2000/08/25 21:02:15  shavirin
* Changed Flat file printing from DUMP_MODE to RELEASE_MODE.
*
* Revision 6.7  2000/06/21 14:34:35  beloslyu
* fix the args for ctime_r on linux
*
* Revision 6.6  2000/05/31 12:37:17  kans
* removed SwapUint4, which somehow got back into the code
*
* Revision 6.5  2000/05/30 16:20:28  kans
* removed cvs merge when incorrect endian code was removed
*
* Revision 6.3  2000/05/26 18:05:53  shavirin
* Added protection against big-little endians for uids from Entrez.
*
* Revision 6.2  2000/05/09 13:37:01  shavirin
* Use functions IS_ntdb_accession() and IS_protdb_accession() to
* verify accession number.
*
* Revision 6.1  2000/05/04 21:14:33  shavirin
* Initial revision.
*
*
* ==========================================================================
*/

#include <ncbi.h>
#include <sequtil.h>
#include <asn2ff.h>
#include <tofasta.h>
#include <ffprint.h>
#include <ent2api.h>
#include <accid1.h>

#define DB_NUCLEOTIDE "Nucleotide"
#define DB_PROTEIN    "Protein"
#define DB_MEDLINE    "Medline"

#define NUMARGS (sizeof(BE_args)/sizeof(BE_args[0]))

Args BE_args[] = {
  {"Database\n"
   "         0 - Nucleotide  \n"
   "         1 - Protein",
   "0", NULL, NULL, FALSE,'s',ARG_INT,0.0,0,NULL},
  {"Format of output\n"
   "         0 - GenBank/GenPept   \n"
   "         1 - FASTA \n" 
   "         2 - ASN.1 \n"
   "         3 - List of GIs\n"
   "         4 - List of Deflines\n"
   "         5 - Single GenBank Bioseq-set ASN.1",
   "0", NULL,NULL,FALSE,'f',ARG_INT, 0.0,0,NULL},
  {"Number of returned entries from single gi/accession\n"
   "         0 - Return single entry specified by gi/accession  \n"
   "         1 - Return all entries in SeqEntry, that available",
   "0", NULL,NULL,FALSE,'n',ARG_INT,0.0,0,NULL},
  {"Text or HTML? (For GenBank/GenPept)\n"
   "         0 - Text output   \n"
   "         1 - HTML output", 
   "0", NULL,NULL,FALSE,'h',ARG_INT, 0.0,0,NULL},
  {"Query string",
   NULL, NULL,NULL,TRUE,'q',ARG_STRING, 0.0,0,NULL},  
  {"ID (accession or gi)",
   NULL, NULL,NULL,TRUE,'u',ARG_STRING, 0.0,0,NULL},
  {"File with list of GIS/Accessions",
   "stdin", NULL,NULL,TRUE,'i',ARG_FILE_IN, 0.0,0,NULL},
  {"Logfile name:",
   "stdout", NULL,NULL,TRUE,'l',ARG_FILE_OUT, 0.0,0,NULL}
};

#define F_GEN       0
#define F_FASTA     1
#define F_ASN1      2
#define F_GILIST    3
#define F_DLIST     4
#define F_ASN1_GENB 5

#define REQ_DEFAULT        0
#define REQ_LIST_OF_GIS    1
#define REQ_ADVANCED_QUERY 2
#define REQ_ORGANISM       3

#define SQL_SRV_NAME "PUBSEQ_OS"

#define QSRV_LOGFILE_NAME     "qserver.log"
#define QSRV_LOGFILE_STD_NAME "/tmp/qserver_std.log"

typedef VoidPtr QSRVHandlePtr; 

static CharPtr BE_Dbname[] = {"Nucleotide", "Protein", "Medline"};

typedef struct BEData {
    Int4     database;
    CharPtr  query;
    CharPtr  uids;
    Int4     format;
    Int4     allset;
    Int4     request_type;
    Boolean  html;
    Int4     savetodisk;
    Int4     maxdocs;
    Int4     noheader;
    Int4     commandline;
    Int4     count;
} BEData, PNTR BEDataPtr;

typedef struct BGenBank {
    AsnIoPtr   aip;
    AsnTypePtr atp;
    AsnTypePtr atp_bioseq_set_seq_set;
    AsnTypePtr atp_bioseq_set;
} BGenBank, PNTR BGenBankPtr;

#define MACRO_atp_find(atp,name)\
        if((atp = AsnTypeFind(amp, #name))==NULL){\
                ErrPostEx(SEV_ERROR,0,0,\
                        "Could not find type <%s>", #name);\
                return NULL; \
        }

SeqEntryPtr BESeqEntryGet(Int4 gi)
{
    SeqEntryPtr sep = NULL;
    
    if((sep = ID1SeqEntryGet(gi, 0)) == NULL) {
        fprintf(stderr, "Sequence ID %d cannot be retrieved "
                "from the database", gi);
    }
    
    return sep;
}

BGenBankPtr BGenBankInit(void)
{
    BGenBankPtr bgbp;
    AsnModulePtr amp;

    AsnTypePtr       atp_bioseq_set;
    AsnTypePtr       atp_bioseq_set_level;
    AsnTypePtr       atp_bioseq_set_class;
    AsnTypePtr       atp_bioseq_set_release;
    AsnTypePtr       atp_bioseq_set_date;
    AsnTypePtr       atp_bioseq_set_seq_set;
    AsnTypePtr       atp_bioseq_set_seq_set_E;

    Char  release[] = "Q-server Production";
    Char  date_time[128];
    NCBI_Date       date;
    DataVal dv;

    DateClean(&date);

    bgbp = (BGenBankPtr)MemNew(sizeof(BGenBank));
    bgbp->aip = AsnIoNew(ASNIO_TEXT_OUT, stdout, NULL, NULL, NULL);
    
    amp = AsnAllModPtr();

    MACRO_atp_find(atp_bioseq_set,Bioseq-set);
    MACRO_atp_find(atp_bioseq_set_level,Bioseq-set.level);
    MACRO_atp_find(atp_bioseq_set_class,Bioseq-set.class);
    MACRO_atp_find(atp_bioseq_set_release,Bioseq-set.release);
    MACRO_atp_find(atp_bioseq_set_date,Bioseq-set.date);
    MACRO_atp_find(atp_bioseq_set_seq_set,Bioseq-set.seq-set);
    MACRO_atp_find(atp_bioseq_set_seq_set_E,Bioseq-set.seq-set.E);

    if(!AsnOpenStruct(bgbp->aip,atp_bioseq_set,NULL))     
        return NULL;
    dv.intvalue = 0;

    if(!AsnWrite(bgbp->aip,atp_bioseq_set_level,&dv))     
        return NULL;
    dv.intvalue = 7;

    if(!AsnWrite(bgbp->aip,atp_bioseq_set_class,&dv))     
        return NULL;
    dv.ptrvalue = &release;

    if(!AsnWrite(bgbp->aip,atp_bioseq_set_release,&dv))     
        return NULL;

    Nlm_DayTimeStr(date_time,TRUE,TRUE);
    date.str=date_time;

    if(!DateAsnWrite(&date,bgbp->aip,atp_bioseq_set_date))        
        return NULL;
    
    if(!AsnOpenStruct(bgbp->aip,atp_bioseq_set_seq_set,NULL))     
        return NULL;
    AsnIoFlush(bgbp->aip);
    
    bgbp->atp =atp_bioseq_set_seq_set_E;
    bgbp->atp_bioseq_set_seq_set = atp_bioseq_set_seq_set;
    bgbp->atp_bioseq_set = atp_bioseq_set;
    
    return bgbp;
}
void BGenBankClose(BGenBankPtr bgbp)
{
    AsnCloseStruct(bgbp->aip, bgbp->atp_bioseq_set_seq_set,NULL);
    AsnCloseStruct(bgbp->aip, bgbp->atp_bioseq_set,NULL);
    
    AsnIoClose(bgbp->aip);
    MemFree(bgbp);
    return;
}

void BEPrintIds(BEDataPtr pBdata, Uint4 *ids, int count)
{
    Int4 i;
    SeqEntryPtr sep, sep_all;
    Boolean retvalue = TRUE;
    SeqIdPtr sip = NULL;
    BioseqPtr bsp;
    BGenBankPtr bgbp;
    AsnIoPtr aip;
    Boolean is_na = FALSE;
    
    if(pBdata->format == F_GILIST) {
        for(i = 0; i < count; i++)
            fprintf(stdout, "%d\n", (int) ids[i]);
        return;
    }
    
    if(pBdata->database == 0)
        is_na = TRUE;

    if(pBdata->format == F_ASN1_GENB)
        bgbp = BGenBankInit();
    
    for(i = 0; i < count; i++) {
        
        sep_all = BESeqEntryGet(ids[i]);
        
        if(sep_all == NULL) {
            ErrPostEx(SEV_ERROR, 88, 67, "Retrieving of blob for the "
                      "gi=%d failed", (int)ids[i]);
            continue;
        }
        
        if(!pBdata->allset) {
            ObjMgrRegister(OBJ_SEQENTRY, sep_all);
            sip = ValNodeNew(NULL);
            sip->choice = SEQID_GI;
            sip->data.intvalue = ids[i];
            
            if((bsp = BioseqFind(sip)) == NULL) {
                ErrPostEx(SEV_ERROR, 88, 67, 
                          "Error finding bioseq for gi=%d\n", (int)ids[i]);
                continue;
            }

            sep = SeqEntryNew();
            sep->choice = 1; /* Bioseq */
            sep->data.ptrvalue = bsp;
        } else {
            sep = sep_all;
        }
        
        switch(pBdata->format) {        
        case F_FASTA:               /* 1 */

            if(!SeqEntryToFasta(sep, stdout, is_na)) {
                if(!SeqEntryToFasta(sep, stdout, !is_na)) {
                    ErrPostEx(SEV_ERROR, 88, 67, "Printing of FASTA format "
                              "(gi=%d) failed\r\n", (int)ids[i]);
                }
            }

            break;
        case F_ASN1:                /* 2 */

            aip = AsnIoNew(ASNIO_TEXT_OUT, stdout, NULL, NULL, NULL);
            SeqEntryAsnWrite(sep, aip, NULL);
            AsnIoClose(aip);

            break;
        case F_GILIST:              /* 3 */
            for(i = 0; i < count; i++)
                fprintf(stdout, "%d\n", (int) ids[i]);
            break;
        case F_DLIST:               /* 4 */
            if (IS_Bioseq(sep))
                retvalue = SeqEntrysToDefline(sep, stdout, is_na, 3);
            else
                retvalue = SeqEntrysToDefline(sep, stdout, is_na, 0);
            
            if(retvalue == FALSE) {
                if (IS_Bioseq(sep))
                    retvalue = SeqEntrysToDefline(sep, stdout, !is_na, 3);
                else
                    retvalue = SeqEntrysToDefline(sep, stdout, !is_na, 0);
            }
            break;
        case F_ASN1_GENB:           /* 5 */
            retvalue = SeqEntryAsnWrite(sep, bgbp->aip, bgbp->atp);
            break;
        default:
        case F_GEN:                 /* 0 */
            if(!SeqEntryToFlatEx(sep_all, stdout, 
                                 is_na ? GENBANK_FMT : GENPEPT_FMT, 
                                 RELEASE_MODE, sip, FF_REGULAR)) {
                
                if(!SeqEntryToFlatEx(sep_all, stdout, 
                                     is_na ? GENPEPT_FMT : GENBANK_FMT, 
                                     RELEASE_MODE, sip, FF_REGULAR)) {
                }
            }
            break;
        }
        
        SeqEntryFree(sep_all);
        ValNodeFree(sip);
    }

    if(pBdata->format == F_ASN1_GENB)
        BGenBankClose(bgbp);
    
    return;
}
void BEFreeCLParam(BEDataPtr pBdata)
{
    MemFree(pBdata->query);
    MemFree(pBdata->uids);
    MemFree(pBdata);
    
    return;
}
BEDataPtr BEMakeCLParam(void)
{
    BEDataPtr pBdata;
    FILE *fd;

    if (!GetArgs ("qserver", NUMARGS, BE_args))
        return NULL;
    
    if(!ErrSetLogfile (BE_args[7].strvalue, ELOG_APPEND))
        exit(1);
    
    pBdata = (BEDataPtr)MemNew(sizeof(BEData));

    pBdata->database      = BE_args[0].intvalue;
    pBdata->format        = BE_args[1].intvalue;
    pBdata->allset        = BE_args[2].intvalue;
    pBdata->html          = (Uchar) BE_args[3].intvalue;
    pBdata->query         = StringSave(BE_args[4].strvalue);

    if(BE_args[5].strvalue != NULL)
        pBdata->uids = StringSave(BE_args[5].strvalue);
    else if(BE_args[4].strvalue == NULL) {
        fd = FileOpen(BE_args[6].strvalue, "r");
        pBdata->uids = WWWReadFileInMemory(fd, 0, FALSE);
        FileClose(fd);
    }

    pBdata->commandline   = TRUE;

    if(pBdata->query == NULL && pBdata->uids == NULL) {
        MemFree(pBdata);
        ErrPostEx(SEV_ERROR, 88, 0, 
                  "Error in reading parameters. "
                  "Please check, that query string was set\n");
        return NULL;
    }

    return pBdata;
}
static void CleanCRLF(CharPtr query)
{
    CharPtr chptr;
    
    if(query == NULL)
        return;
    
    for(chptr = query; *chptr != NULLB; chptr++) {
        if(*chptr == '\n' || *chptr == '\r')
            *chptr = ' ';
    }
    return;
}

BEDataPtr BEMakeWWWParam(WWWInfoPtr info)
{
    BEDataPtr pBdata;
    CharPtr   chptr;
    Char tmp[512];

    pBdata = (BEDataPtr)MemNew(sizeof(BEData));

    /* Database to search */

    if((chptr = WWWGetValueByName(info, "DATABASE")) == NULL) {
        if((chptr = WWWGetValueByName(info, "DB")) == NULL)
            if((chptr = WWWGetValueByName(info, "DATALIB")) == NULL) {
                chptr = "n"; /* Default to nucleotides */
            }
    }
    
    switch(*chptr) {
    case 'n':
        pBdata->database = 0;
        break;
    case 'p':
        pBdata->database = 1;
        break;
    default:
        pBdata->database = 0;
        break;
    }

    if((chptr = WWWGetValueByName(info, "REQUEST_TYPE")) != NULL) {
        if(!StringICmp(chptr, "LIST_OF_GIS"))
            pBdata->request_type = REQ_LIST_OF_GIS;
        else if(!StringICmp(chptr, "ADVANCED_QUERY"))
            pBdata->request_type = REQ_ADVANCED_QUERY;
        else if(!StringICmp(chptr, "ORGANISM"))
            pBdata->request_type = REQ_ORGANISM;
    } else {
        pBdata->request_type = REQ_DEFAULT;
    }

    switch(pBdata->request_type) {
    case REQ_ADVANCED_QUERY:
    case REQ_LIST_OF_GIS:
    case REQ_DEFAULT:
        /* Query string */
        
        if((chptr = WWWGetValueByName(info, "TERM")) == NULL)
            chptr = WWWGetValueByName(info, "QUERY");
        
        if(chptr != NULL && *chptr != NULLB) {
            pBdata->query = StringSave(chptr);
            CleanCRLF(pBdata->query);
        }
        
        /* List of UIDs */
        
        if((chptr = WWWGetValueByName(info, "UID")) != NULL && 
           *chptr != NULLB) {
            pBdata->uids = StringSave(chptr);
        }
        break;
    case REQ_ORGANISM:
        /* Query for organism retrieval */
        
        if(((chptr = WWWGetValueByName(info, "ORGNAME")) != NULL && 
            *chptr != NULLB) || 
           ((chptr = WWWGetValueByName(info, "LIST_ORG")) != NULL && 
            *chptr != NULLB && StringICmp (chptr, "(None)"))) {
            sprintf(tmp, "%s[ORGN]", chptr);
            pBdata->query = StringSave(tmp);
        }
        break;
    }

    /* Checking for data consistency */
    
    if(pBdata->request_type == REQ_LIST_OF_GIS && pBdata->uids == NULL)
        goto fail_return;
    if((pBdata->request_type == REQ_ADVANCED_QUERY || 
        pBdata->request_type == REQ_ORGANISM) && pBdata->query == NULL)
        goto fail_return;
    if(pBdata->query == NULL && pBdata->uids == NULL)
        goto fail_return;
    
    /* Format of output */
    
    if((chptr = WWWGetValueByName(info, "FORMAT")) != NULL ||
       (chptr = WWWGetValueByName(info, "DOPT")) != NULL) {
        pBdata->format = atoi(chptr);
    } else {
        pBdata->format = F_GILIST;
    }
    
    /* HTML output */
    
    if((chptr = WWWGetValueByName(info, "HTML")) != NULL) {
        if(!StringICmp(chptr, "NO") || !StringICmp(chptr, "FALSE") ||
           !StringICmp(chptr, "0"))
            pBdata->html = FALSE;
        else
            pBdata->html = TRUE;
    }

    /* Output type */
    
    if((chptr = WWWGetValueByName(info, "SAVETO")) != NULL) {
        if(!StringICmp(chptr, "NO") || !StringICmp(chptr, "FALSE") ||
           !StringICmp(chptr, "0"))
            pBdata->savetodisk = FALSE;
        else
            pBdata->savetodisk = TRUE;
    }


    if((chptr = WWWGetValueByName(info, "ALLSET")) != NULL) {
        if(!StringICmp(chptr, "NO") || !StringICmp(chptr, "FALSE") ||
           !StringICmp(chptr, "0"))
            pBdata->allset = FALSE;
        else
            pBdata->allset = TRUE;
    }
    
    if((chptr = WWWGetValueByName(info, "MAXDOCS")) != NULL ||
       (chptr = WWWGetValueByName(info, "DISPMAX")) != NULL)
        pBdata->maxdocs = atol(chptr);

    if((chptr = WWWGetValueByName(info, "NOHEADER")) != NULL) {
        if(!StringICmp(chptr, "NO") || !StringICmp(chptr, "FALSE") ||
           !StringICmp(chptr, "0"))
            pBdata->noheader = FALSE;
        else
            pBdata->noheader = TRUE;
    }
    
    return pBdata;

 fail_return:
    MemFree(pBdata);
    return NULL;
}

/* This function is interface to the Entrez2 engine. It may be used
   to get list of gis corresponding to the Entrez Boolean string or
   just number of such hits in the Entrez database */

static Int4 BEGetUidsFromQuery(CharPtr query, Uint4Ptr PNTR uids, 
                               Boolean is_na, Boolean count_only)
{
    Entrez2ReplyPtr e2ry;
    Entrez2RequestPtr  e2rq;
    E2ReplyPtr e2rp;
    Int4 count = 0, i;
    Entrez2BooleanReplyPtr e2br;
    Entrez2IdListPtr e2idlist;
    
    *uids = NULL;
    
    EntrezSetProgramName ("BLAST API");
    EntrezSetServer ("www.ncbi.nlm.nih.gov", 80, 
                     "/entrez/utils/entrez2server.fcgi");
    
    e2rq = EntrezCreateBooleanRequest (!count_only, FALSE, 
                                       is_na? "Nucleotide" : "Protein", 
                                       query, 0, 0, NULL, 0, 0);
    
    e2ry = EntrezSynchronousQuery (e2rq);
    
    if (e2ry == NULL) {
        ErrPostEx(SEV_ERROR, 0, 0, 
                  "NULL returned from EntrezSynchronousQuery()");
        return -1;
    }

    if((e2rp = e2ry->reply) == NULL) {
        ErrPostEx(SEV_ERROR, 0, 0, "Invalid ASN.1: E2ReplyPtr==NULL");
        return -1;
    }
    
    switch(e2rp->choice) {
        
    case E2Reply_error:
        ErrPostEx(SEV_ERROR, 0, 0, (CharPtr) e2rp->data.ptrvalue);
        count = -1;
        break;
    case E2Reply_eval_boolean:
        e2br = (Entrez2BooleanReplyPtr) e2rp->data.ptrvalue;
        count = e2br->count;
        if((e2idlist = e2br->uids) != NULL) {
            count = e2idlist->num;
            *uids = MemNew(sizeof(Int4)*count);
            BSSeek((ByteStorePtr) e2idlist->uids, 0, SEEK_SET);
            BSRead((ByteStorePtr) e2idlist->uids, *uids, sizeof(Int4)*count);

        }
        break;
    default:
        ErrPostEx(SEV_ERROR, 0, 0, "Invalid reply type from the server: %d", e2rp->choice);
        count = -1;
        break;
        
    }

    Entrez2ReplyFree(e2ry);
    Entrez2RequestFree(e2rq);

    return count;
}
static Int4 BEAccessionToGi (CharPtr string) 
{
    Char buffer[32];
    CharPtr chptr;
    Int2 version;
    Int4 gi, index;
    SeqIdPtr sip;
    TextSeqIdPtr tsip;
    PDBSeqIdPtr  psip;
    long tmplong;
    Boolean digit;

    for(chptr = string, digit = TRUE; *chptr != NULLB; chptr++) {
        if(!IS_DIGIT(*chptr)) {
            digit = FALSE;
            break;
        }
    }
        
    if(digit) {
        if((gi = atol(string)) > 0)
            return gi;
    }

    /* all letters in accesion should be upper */
    string = Nlm_StringUpper(string);
    
    gi = 0;

    if((sip = ValNodeNew (NULL)) == NULL)
        return -1;
    
    index = 0; version = 0;
    while (*string != '\0' && index < 16) {
        if (*string == '.')
            break;
        buffer[index] = *string;
        string++;
        index++;
    }

    buffer[index] = '\0';
    if (*string == '.' && *(string+1) != '\0') {
        sscanf((string+1), "%ld", &tmplong);
        version = (Int2) tmplong;
    }
    
    if((tsip = TextSeqIdNew ()) == NULL)
        return -1;
    
    tsip->accession = StringSave(buffer);
    tsip->version = version;
    
    /* GenBank, EMBL, and DDBJ. */
    sip->choice = SEQID_GENBANK;
    sip->data.ptrvalue = (Pointer) tsip;
    gi = ID1FindSeqId (sip);
    
    if (gi == 0) {
        /* SwissProt. */
        sip->choice = SEQID_SWISSPROT;
        gi = ID1FindSeqId (sip);
    } else {
        goto retpoint;
    }

    if (gi == 0) {
        /* PIR */
        sip->choice = SEQID_PIR;
        gi = ID1FindSeqId (sip);
    } else {
        goto retpoint;
    }

    if (gi == 0) {
        /* PRF */
        sip->choice = SEQID_PRF;
        gi = ID1FindSeqId (sip);
    } else {
        goto retpoint;
    }

    if (gi == 0) {
        /* OTHER, probably 'ref' */
        sip->choice = SEQID_OTHER;
        gi = ID1FindSeqId (sip);
    }

    if(gi != 0)
        goto retpoint;

    /* OK. We failed to find gi using string as TextSeqId. Now trying
       last time - with PDBSeqIdPtr */

    if((psip = PDBSeqIdNew()) == NULL)
        return -1;
    
    sip->choice = SEQID_PDB;
    sip->data.ptrvalue = psip;
    
    psip->mol = StringSave(buffer);
    psip->chain = version;

    gi = ID1FindSeqId (sip);

    SeqIdFree(sip);

 retpoint:
    TextSeqIdFree(tsip);
    return gi;
}

Int4 BE_AccToGi(CharPtr accession, Uint4 **giptr, Int4 database)
{
    Int4 count;
    Int4 gi;
    Char tmp[512];
    
    /* Checking if this is gi number */
    
    if((gi = atol(accession)) != 0) {
        *giptr = (Uint4 *)MemNew(sizeof(Uint4));
        (*giptr)[0] = gi;
        return 1;
    }
    sprintf(tmp, "%s[ACCN]", accession);    
    
    count = BEGetUidsFromQuery(tmp, giptr, 
                               database == 0, /* Nucleotide ? */
                               FALSE);
    
    return count > 0 ? count : 0;
}

#define UID_BUFF_SIZE 2048

Int4 BE_ReadIds(BEDataPtr pBdata, Uint4 **ids_out)
{
    Uint4 *uids, *giptr;
    Int4 length, NumNotValid = 0, gi;
    Int4 i, j, k, allocated, count = 0;
    Char TmpBuff[16];
    CharPtr buffer;

    if((buffer = pBdata->uids) == NULL || *buffer == NULLB) {
        *ids_out = NULL;
        return 0;
    }
    
    length = StringLen(buffer);
    
    allocated = UID_BUFF_SIZE;
    uids = (Uint4 *)MemNew(allocated * sizeof(Uint4));

    for(i = 0; i < length; i++) {
        
        if(isspace(buffer[i]) || buffer[i] == ',') /* Rolling spaces */
            continue;
        
        /* This is defence from badly formatted requests */
        
        if(NumNotValid > 10) {
            printf("**** ERROR: Too many invalid Gis/Accessions, "
                   "parsing aborted\n");
            *ids_out = NULL;
            return 0;
        }
        
        /* Rolling spaces */
        
        j= 0;
        while (!isspace(buffer[i]) && j < 10  && i < length) { 
            TmpBuff[j] = buffer[i];
            j++; i++;
            if(buffer[i] == ',')  /* Comma is valid delimiter */
                break;
        }
        TmpBuff[j] = NULLB;
        
        
        /* Ignore strings like ">Protein" */
        
        if(j > 0 && TmpBuff[0] == '>' && IS_ALPHA(TmpBuff[1]))
            continue;
        
        /* Is gi/accession too long ??? */
        
        if(j == 10) {
            NumNotValid++;
            
            while(!isspace(buffer[i]) || 
                  buffer[i] == ',' || 
                  buffer[i] == NULLB) /* Rolling until spaces */
                i++;
            continue;  /* Next may be valid ... who knows...?? */   
        }
        
        /* Now validating accession/gi */
        
        for(k =0; k < j; k++) {
            if(!isdigit(TmpBuff[k])) {
                break;
            }
        }
        if(k != j) { 
            if(!IS_ntdb_accession(TmpBuff) && !IS_protdb_accession(TmpBuff)) {
                NumNotValid++;
                printf("**** WARNING: Gi/Accession \"%s\" is not valid\n", 
                       TmpBuff);
                continue;
            }
        }
        
        /* If this is valid Accession check and tranfer it to gi */
        
        giptr = NULL;

        if((gi = BEAccessionToGi (TmpBuff)) < 0) {
            printf("**** WARNING: Gi/Accession %s is not found "
                   "in database----\r\n", TmpBuff);
            /* NumNotValid++; */
            continue;
        } else {
            if(count == allocated) {
                allocated += UID_BUFF_SIZE;
                uids = (Uint4*)Realloc(uids, allocated * sizeof(Uint4));
            }
            uids[count] = gi;
            count++;
        }
    }

    if(NumNotValid) {
        printf("**** %d invalid Gi%s/Accession%s present in Entrez-batch "
               "request\r\n", 
               (int)NumNotValid,
               NumNotValid == 1 ? "" : "s",
               NumNotValid == 1 ? "" : "s"
               );
    }
    
    *ids_out = uids;
    return count;    
}
static void BatchHead(VoidPtr pointer, FILE *fd)
{
  return;
}
static void BatchTail(VoidPtr pointer, FILE *fd)
{
  return;
}

Boolean QSRV_Time(CharPtr string, Int4 len, time_t seconds)
{
    if(string == NULL || len < 25)
        return FALSE;
    
    if(!seconds) {
        seconds = GetSecs();
    }
    
#if defined(OS_UNIX_IRIX) || defined(OS_UNIX_LINUX) || defined(OS_UNIX_OSF1) || defined(OS_UNIX_FREEBSD)
    ctime_r(&seconds, string);
#elif defined(OS_UNIX_DARWIN) /* no ctime_r :-/ */
    strncpy(string, ctime(&seconds), len - 1);
    string[len - 1] = '\0';
#else
    ctime_r(&seconds, string, len);
#endif
    
    string[24] = NULLB;
    return TRUE;
}

void QSRVWriteLogInfo(BEDataPtr pBdata)
{
    FILE *fd;
    Char tmp[128];
    Char timebuf[64];
    
    if(pBdata == NULL)
        return;
    
    if((fd = FileOpen(QSRV_LOGFILE_NAME, "a")) == NULL) {
        sprintf(tmp, "/tmp/%s", QSRV_LOGFILE_NAME);
        if((fd = FileOpen(tmp, "a")) == NULL)
            return;
    }
    
    QSRV_Time(timebuf, sizeof(timebuf), 0);
    
    fprintf(fd, 
            "%s|db=%d|term=\"%s\"|format=%d|count=%d|%d|%d|%d|%d|%d|%d\n", 
            timebuf, pBdata->database,
            pBdata->query == NULL? "(null)" : pBdata->query, 
            pBdata->format, pBdata->count, pBdata->allset, 
            pBdata->request_type, pBdata->html, pBdata->savetodisk, 
            pBdata->noheader, pBdata->commandline);
    
    fflush(fd);
    FileClose(fd);

    return;
}

Int2 Main(void)
{
    WWWInfoPtr info;
    WWWErrorCode error;
    BEDataPtr pBdata;
    Int4 count;
    Uint4 *ids;
    Char tmp[512];

    if((error = WWWReadPosting(&info)) != WWWErrOk) {
        ErrPostEx(SEV_FATAL, 88, 0, "Error in processing WWW request");
        return 1;
    }

    if(WWWGetMethod(info) == COMMAND_LINE) {
        WWWInfoFree(info);
        
        if((pBdata = BEMakeCLParam()) == NULL)
            return 1;
        
    } else {
        
        if(!ErrSetLogfile (QSRV_LOGFILE_STD_NAME, ELOG_APPEND))
            return 1;
        
        if((pBdata = BEMakeWWWParam(info)) == NULL) {
            printf("Content-type: text/html\n\n");
            printf("QSRV_STATUS 802 Invalid input parameters <PRE><BR>\n");
            fflush(stdout);
            
            ErrPostEx(SEV_ERROR, 88, 0, 
                      "Error in reading parameters. "
                      "Please check, that query string was set\n");
            return 1;
        }
    }

    if(!pBdata->commandline) {
        if(pBdata->savetodisk) 
            printf("Content-type: chemical/seq-%s-genbank\n\n", 
                   pBdata->database == 0 ? "na" : "aa");
        else {
            printf("Content-type: text/html\n\n");
            printf("<HTML><HEADER><TITLE>Batch Entrez results"
                   "</TITLE></HEADER><PRE>\n");
        }
    }

    if (! ID1BioseqFetchEnable("Nbatch", TRUE)) {
        printf("Cannot initialize ID1\n");
        return 1;
    }

    if(pBdata->html) {
        init_www(); /* initializing WWW mode */
        head_tail_ff(NULL, BatchHead, BatchTail);
      
    }

    SeqEntryLoad();

    switch(pBdata->request_type) {
    case REQ_DEFAULT:
        if(pBdata->uids == NULL) {
            count = BEGetUidsFromQuery(pBdata->query, &ids, 
                               pBdata->database == 0, /* Nucleotide ? */
                               FALSE);
        } else {
            count = BE_ReadIds(pBdata, &ids);
        }
        break;
    case REQ_ADVANCED_QUERY:
    case REQ_ORGANISM:
        count = BEGetUidsFromQuery(pBdata->query, &ids, 
                                   pBdata->database == 0, /* Nucleotide ? */
                                   FALSE);
        break;
    case REQ_LIST_OF_GIS:
        count = BE_ReadIds(pBdata, &ids);
        break;    
    }
    pBdata->count = count;
    
    QSRVWriteLogInfo(pBdata);
    
    if(count < 0) {
        if(pBdata->commandline)
            ErrPostEx(SEV_ERROR, 0, count, "Error in searching the database");
        else
            printf("QSRV_STATUS %d Error in searching database\n", count);
        return 1;
    }
    
    if(count == 0) {
        if(pBdata->commandline)
            ErrPostEx(SEV_INFO, 0,0, "No entries found");
        else
            printf("QSRV_STATUS 901 OK No entries found\n", count);
        return 1;
    }
    
    if((count > 20000 && pBdata->format == F_GEN) ||
       (count > 100000 && pBdata->format != F_GILIST)) {

        if(pBdata->commandline)
            ErrPostEx(SEV_WARNING, 0,0, "Number of sequences %d exceed limit", 
                      count);
        else
            printf("QSRV_STATUS 803 Error Number of sequences %d "
                   "exceed limit\n", count);
        
        pBdata->format = F_GILIST;
    }

    if(!pBdata->noheader && !pBdata->commandline) 
        printf("QSRV_STATUS 900 OK: %d entr%s found <PRE><BR>\n", 
               count, count == 1? "y" : "ies");

    fflush(stdout);
    
    /* Printing results */
    
    BEPrintIds(pBdata, ids, count);
    
    /* Clearing memory */

    MemFree(ids);
    BEFreeCLParam(pBdata);

    return 0;
}
