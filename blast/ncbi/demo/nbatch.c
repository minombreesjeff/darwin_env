/* $Id: nbatch.c,v 6.3 1999/03/18 15:18:17 shavirin Exp $
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
* File Name:  $RCSfile: nbatch.c,v $
*
* Author:  Sergei Shavirin
*
* Version Creation Date: 02/10/1999
*
* $Revision: 6.3 $
*
* File Description:
*         Main file for Command-line Batch Entrez using WWW interface
*
* $Log: nbatch.c,v $
* Revision 6.3  1999/03/18 15:18:17  shavirin
* Fixed problem with large input buffer with list of ids.
*
* Revision 6.1  1999/02/10 18:24:50  shavirin
* Initial revision.
*
*
* Initial revision.
*
*
* ==========================================================================
*/

#include <ncbi.h>
#include <ncbisock.h>

#define NUMARGS 9

Args BE_args[NUMARGS] = {
  {"Database\n"                 /* 0 */
   "         0 - Nucleotide  \n"
   "         1 - Protein",
   NULL, NULL, NULL, FALSE,'s',ARG_INT,0.0,0,NULL},
  {"Format of output\n"         /* 1 */
   "         0 - GenBank/GenPept   \n"
   "         1 - FASTA \n" 
   "         2 - ASN.1 \n"
   "         3 - List of GIs\n"
   "         4 - List of Deflines\n"
   "         5 - Single GenBank Bioseq-set ASN.1",
   "0", NULL,NULL,FALSE,'f',ARG_INT, 0.0,0,NULL},
  {"Number of returned entries from single gi/accession\n" /* 2 */
   "         0 - Return single entry specified by gi/accession  \n"
   "         1 - Return all entries in SeqEntry, that available",
   "0", NULL,NULL,FALSE,'n',ARG_INT,0.0,0,NULL},
  {"Text or HTML? (For GenBank/GenPept)\n" /* 3 */
   "         0 - Text output   \n"
   "         1 - HTML output", 
   "0", NULL,NULL,FALSE,'h',ARG_INT, 0.0,0,NULL},
  {"File with list of GIS/Accessions", /* 4 */
   NULL, NULL,NULL,TRUE,'i',ARG_FILE_IN, 0.0,0,NULL},
  {"Organism name",              /* 5 */
   NULL, NULL,NULL,TRUE,'o',ARG_STRING, 0.0,0,NULL},  
  {"ID (accession or gi)",      /* 6 */
   NULL, NULL,NULL,TRUE,'u',ARG_STRING, 0.0,0,NULL},
  {"Query string",              /* 7 */
   NULL, NULL,NULL,TRUE,'q',ARG_STRING, 0.0,0,NULL},  
  {"Logfile name:",             /* 8 */
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

#define QSRV_LOGFILE_NAME     "qserver.log"

#define LARGE_CHUNK 1024

typedef struct BUFFStore {
    Int4 allocated;
    Int4 used;
    CharPtr buffer;
} BUFFStore, PNTR BUFFStorePtr;

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

#define BE_HOST_NAME    "www.ncbi.nlm.nih.gov"
#define BE_PROGRAM_NAME "/cgi-bin/Entrez/qserver.cgi"
#define HUGE_STRING_LEN 8192

static void BEFreeCLParam(BEDataPtr pBdata)
{
    MemFree(pBdata->query);
    MemFree(pBdata->uids);
    MemFree(pBdata);
    
    return;
}
static BEDataPtr BEMakeCLParam(void)
{
    BEDataPtr pBdata;
    FILE *fd;
    Char tmp[128];

    if (!GetArgs ("Nbatch2", NUMARGS, BE_args))
        return NULL;
    
    if(!ErrSetLogfile (BE_args[8].strvalue, ELOG_APPEND))
        exit(1);
    
    pBdata = MemNew(sizeof(BEData));
    
    pBdata->database      = BE_args[0].intvalue;
    pBdata->format        = BE_args[1].intvalue;
    pBdata->allset        = BE_args[2].intvalue;
    pBdata->html          = (Uchar) BE_args[3].intvalue;
    
    if(BE_args[5].strvalue != NULL) {
        sprintf(tmp, "%s[ORGN]", BE_args[5].strvalue);
        pBdata->query         = StringSave(tmp);
    } else {
        pBdata->query         = StringSave(BE_args[7].strvalue);
    }

    if(BE_args[6].strvalue != NULL) {
        pBdata->uids = StringSave(BE_args[6].strvalue);
    } else if(BE_args[7].strvalue == NULL && BE_args[6].strvalue == NULL && 
              BE_args[4].strvalue != NULL) {
        if((fd = FileOpen(BE_args[4].strvalue, "r")) != NULL) {
            pBdata->uids = WWWReadFileInMemory(fd, 0, FALSE);
            FileClose(fd);
        }
    }

    pBdata->commandline   = TRUE;
    
    if(pBdata->query == NULL && pBdata->uids == NULL) {
        MemFree(pBdata);
        ErrPostEx(SEV_ERROR, 88, 0, 
                  "Error in input parameters. "
                  "Please check, that all required parameters were set\n");
        return NULL;
    }

    return pBdata;
}

static void BEAddToBuffStore(BUFFStorePtr buffs, CharPtr buffer)
{
    Int4 length;

    if(buffs == NULL || buffer == NULL)
        return;
    
    length = StringLen(buffer);
    
    if(length > LARGE_CHUNK) {
        buffs->allocated += length + 1;
        buffs->buffer = Realloc(buffs->buffer, buffs->allocated);
    }
    
    if(buffs->allocated - buffs->used < length + 1) {
        buffs->allocated += LARGE_CHUNK;
        buffs->buffer = Realloc(buffs->buffer, buffs->allocated);
    }

    StringCat(buffs->buffer, buffer);

    buffs->used += length;    

    return;
}
static CharPtr BEPrintWWWBuffer(BEDataPtr pBdata)
{
    BUFFStorePtr buffs;
    CharPtr outbuff;
    Char tmp[1024];

    buffs = MemNew(sizeof(BUFFStore));

    buffs->allocated = LARGE_CHUNK;
    buffs->buffer = MemNew(LARGE_CHUNK);

    /* Printing out database */

    switch(pBdata->database) {
    case 1:
        StringCpy(tmp, "DB=p&");
        break;
    case '0':
    default:
        StringCpy(tmp, "DB=n&");
        break;
    }
    
    BEAddToBuffStore(buffs, tmp);
    
    /* Printing out request type */
    
    switch(pBdata->request_type) {
    case REQ_LIST_OF_GIS:
        StringCpy(tmp, "REQUEST_TYPE=LIST_OF_GIS&");
        break;
    case REQ_ADVANCED_QUERY:
        StringCpy(tmp, "REQUEST_TYPE=ADVANCED_QUERY&");
        break;
    case REQ_ORGANISM:
        StringCpy(tmp, "REQUEST_TYPE=ORGANISM&");
        break;
    default:
        StringCpy(tmp, "REQUEST_TYPE=DEFAULT_TYPE&");
        break;
    }

    BEAddToBuffStore(buffs, tmp);


    /* Printing out query string ... if any */

    if(pBdata->query != NULL && *pBdata->query != NULLB) {
        BEAddToBuffStore(buffs, "QUERY=");
        BEAddToBuffStore(buffs, pBdata->query);
        BEAddToBuffStore(buffs, "&");
    }
    
    if(pBdata->uids != NULL && *pBdata->uids) {
        BEAddToBuffStore(buffs, "UID=");
        BEAddToBuffStore(buffs, pBdata->uids);
        BEAddToBuffStore(buffs, "&");
    }

    /* Format of output */

    sprintf(tmp, "FORMAT=%d&", pBdata->format);
    BEAddToBuffStore(buffs, tmp);
    
    /* HTML output */

    if(pBdata->html)
        BEAddToBuffStore(buffs, "HTML=TRUE&");
    else
        BEAddToBuffStore(buffs, "HTML=FALSE&");
    
    /* Save to disk data header */

    if(pBdata->savetodisk)
        BEAddToBuffStore(buffs, "SAVETO=TRUE&");
    else
        BEAddToBuffStore(buffs, "SAVETO=FALSE&");
    
    /* Maximum documents in the output */


    /* Do not print the header */
    
    if(pBdata->noheader)
        BEAddToBuffStore(buffs, "NOHEADER=TRUE&");

    if(pBdata->maxdocs != 0) {
        sprintf(tmp, "MAXDOCS=%d&", pBdata->maxdocs);
        BEAddToBuffStore(buffs, tmp);
    }

    outbuff = buffs->buffer;
    MemFree(buffs);
    
    return outbuff;
}

static Int4 BE_DoWWWRequest(CharPtr wwwbuf)
{
    ESOCK_ErrCode error, error2;
    SOCK          sock;
    Int4          bytes, first_time = 1;
    STimeout      timeout;
    Char          buffer[HUGE_STRING_LEN];
    CharPtr       chptr;
    FILE          *fd;

    timeout.sec = 30;
    timeout.usec = 0;

    error = SOCK_Create(BE_HOST_NAME, 80, &timeout, &sock);
    
    if(error != eSOCK_ESuccess)
        return -1;
    
    sprintf(buffer, "POST %s HTTP/1.0\n", BE_PROGRAM_NAME);
    error = SOCK_Write(sock, buffer, StringLen(buffer), (Uint4Ptr) &bytes);

    sprintf(buffer, "User-Agent: %s\n", "Nbatch Program");
    error = SOCK_Write(sock, buffer, StringLen(buffer), (Uint4Ptr) &bytes);
    sprintf(buffer, "Connection: Keep-Alive\n");
    error = SOCK_Write(sock, buffer, StringLen(buffer), (Uint4Ptr) &bytes);
    sprintf(buffer, "Content-type: application/x-www-form-urlencoded\n");
    error = SOCK_Write(sock, buffer, StringLen(buffer), (Uint4Ptr) &bytes);
    sprintf(buffer, "Content-Length: %d\n\n", StringLen(wwwbuf));
    error = SOCK_Write(sock, buffer, StringLen(buffer), (Uint4Ptr) &bytes);
    error = SOCK_Write(sock, wwwbuf, StringLen(wwwbuf), (Uint4Ptr) &bytes);

    /* Now reading all data back */
    
    memset(&buffer, 0, sizeof(buffer));

    if((fd = FileOpen("stdout", "w")) == NULL)
        return 0;
    
    while (1) { /* Infinite loop until error */
   
        error = SOCK_Select(sock, eSOCK_OnRead, &timeout);
        
        if (error == eSOCK_ESuccess) {
            if((error2 = SOCK_Read(sock, buffer, HUGE_STRING_LEN, 
                                   (Uint4Ptr) &bytes)) == eSOCK_ESuccess) {
                if(first_time) {
                    if((chptr = strstr(buffer, 
                                       "QSRV_STATUS 900 OK")) != NULL) {
                        chptr = strchr(chptr, '\n');
                        if(FileWrite(chptr + 1, bytes - (chptr + 1 - buffer), 1, fd) <= 0)
                            break;
                    } else { /* Bad data - printing all stuff */
                        if((chptr = strstr(buffer, 
                                           "QSRV_STATUS")) != NULL) {
                            if(FileWrite(chptr, bytes - (chptr - buffer), 1, fd) <= 0)
                                break;
                        } else {
                            if(FileWrite(buffer, bytes, 1, fd) <= 0)
                                break;
                        }
                    }
                    first_time = 0;
                } else {
                    if(FileWrite(buffer, bytes, 1, fd) <= 0)
                        break;
                }                
            } else {
                break;
            }
        } else if(error == eSOCK_ETimeout) {
            continue;
        } else {
            break;
        }
    }

    FileClose(fd);
    SOCK_Close(sock);
    return 1;
}

Int2 Main(void)
{
    BEDataPtr pBdata;
    CharPtr wwwbuf;

    if((pBdata = BEMakeCLParam()) == NULL)
        return 1;
    
    wwwbuf = BEPrintWWWBuffer(pBdata);

    /* printf("%s", wwwbuf); */
    
    BE_DoWWWRequest(wwwbuf);
    MemFree(wwwbuf);

    BEFreeCLParam(pBdata);
    
    return 0;
}
