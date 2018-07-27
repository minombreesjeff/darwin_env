/* $Id: spellsrv.c,v 1.2 1998/02/25 19:33:01 shavirin Exp $
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
* File Name:  $RCSfile: spellsrv.c,v $
*
* Author:  Sergei Shavirin
*
* Initial Version Creation Date: 02/24/1998
*
* $Revision: 1.2 $
*
* File Description:
*        Main file for NCBI SPELL Service
*
* $Log: spellsrv.c,v $
* Revision 1.2  1998/02/25 19:33:01  shavirin
* Added additional filtering of words.
*
*
* ==========================================================================
*/
#include <ncbi.h>
#include <ncbinet.h>
#include <objspell.h>
#include <ncbisam.h>

#define SPELL_MAX_WORD_SIZE 512
#define NUMARG 5

#define IS_VALID(c) (isalnum(c) || ((c) == '-') || ((c) == '+') || ((c) == '_') || ((c) == '&') || ((c) == '\'') || ((c) == '`'))

Args spellsrv_args[NUMARG] = {
    { "Input file name for spell database", 
      "termlist", NULL, NULL, TRUE, 't', ARG_STRING, 0.0, 0, NULL},
    {"Input file anme for stop words",
     "stopwords", NULL,NULL, TRUE,'s',ARG_FILE_IN, 0.0,0,NULL},
    {"Logfile name:",
     "/dev/null", NULL,NULL,TRUE,'l',ARG_FILE_OUT, 0.0,0,NULL},
    {"Debug mode",
     NULL, NULL,NULL,TRUE,'d',ARG_BOOLEAN, 0.0,0,NULL},
    {"Standalone mode",
     NULL, NULL,NULL,TRUE,'z',ARG_BOOLEAN, 0.0,0,NULL}
};

#define DB_filename    (CharPtr) spellsrv_args[0].strvalue 
#define STOP_filename  (CharPtr) spellsrv_args[1].strvalue 
#define LogFileName    (CharPtr) spellsrv_args[2].strvalue  
#define DebugMode      (CharPtr) spellsrv_args[3].intvalue  
#define StandAlone     (CharPtr) spellsrv_args[4].intvalue  

typedef struct SPELLData
{
    ISAMObjectPtr term_isamp;
    ISAMObjectPtr stop_isamp;
} SPELLData, PNTR SPELLDataPtr;

Boolean SPELLSendResponse(Pointer ptr, AsnIoPtr asnout, Int4 error)
{
    SpellResponsePtr srp;
    
    srp = ValNodeNew(NULL);
    
    if (error != NULL) {
        srp->choice = SpellResponse_error;
        srp->data.intvalue = error;
    } else {
    	srp->choice = SpellResponse_spelltext;
    	srp->data.ptrvalue = ptr;
    }
    
    SpellResponseAsnWrite(srp, asnout, NULL);
    
    srp->data.ptrvalue = NULL;
    SpellResponseFree(srp);
}

static Int4 SPELLFindWord(SPELLDataPtr spellp, CharPtr word)
{
    Uint4 index;
    ISAMErrorCode error;
    
    if((error = SISAMSearch(spellp->stop_isamp, word, 0, NULL,
                            NULL, &index)) < 0) {
        ErrPostEx(SEV_FATAL, 0, 0, "Failed to search string index "
                  "ISAM Error code is %d\n", error);
        return error;
    }
    
    /* If word found in "stopwords" - skip main search */
    
    if(error == ISAMNoError)
        return 1;
    
    if((error = SISAMSearch(spellp->term_isamp, word, 0, NULL,
                            NULL, &index)) < 0) {
        ErrPostEx(SEV_FATAL, 0, 0, "Failed to search string index "
                  "ISAM Error code is %d\n", error);
        return error;
    }
    
    if(error == ISAMNoError)
        return 1;
    
    return 0;
}

static void SPELLGetWord(CharPtr word, CharPtr line, Int4 wordsize) 
{
    Int4 x = 0, y = 0;
    
    /* Looking for alphanumeric in the beginning of the word */

    for(x = 0; line[x] && !isalnum(line[x]); x++)
        continue;
    
    while(TRUE && y < wordsize) {
        if(!(word[y] = line[x]))
            break;
        if(!IS_VALID(line[x]))
            break;
        ++y; ++x;
    }
    
    word[y] = '\0';

    while(line[x] && !isalnum(line[x])) ++x;
    
    for(y=0;(line[y] = line[x]);++x,++y);
}

Boolean SPELLFixWord(CharPtr word)
{
    Int4 i, len;

    StrLower(word);
    len = StringLen(word);
    
    /* Testing for all numerics and accessions */

    if(len == 1)
        return FALSE;
    
    if(isdigit(*word)) {
        for(i = 1; i < len; i++) {
            if(!(isdigit(word[i]) || ispunct(word[i])))
                break;
        }

        if(i == len) return FALSE; /* This is number */

    }
    
    /* Testing, that this is 6 length accession */
    
    if(len == 6 && isalpha(word[0])) {
        for(i = 1; i < len; i++) {
            if(!isdigit(word[i]))
                break;
        }

        if (i == len) return FALSE; /* This is accession */

    }
    
    if(len == 8 && isalpha(word[0])  && isalpha(word[1])) {
        for(i = 2; i < len; i++) {
            if(!isdigit(word[i])) {
                break;
            }
        }
        
        if (i == len) return FALSE; /* This is accession */
        
    }

    return TRUE;
}

static ValNodePtr SPELLCheckString(SPELLDataPtr spellp, CharPtr string)
{
    Int4 len, i, result;
    Char word[SPELL_MAX_WORD_SIZE];
    CharPtr p, chptr, strtmp;
    ValNodePtr head = NULL, vnp, vnp_l;
    
    chptr = strtmp = StringSave(string);

    /* Some usefull filtering */
    
    for (p = strtmp; *p; p++) {
        if (*p < ' ' || *p >= 0x7f)
            *p = ' ';
    }

    /* Removing punctuation in the end of the string */

    do {
        SPELLGetWord(word, strtmp, SPELL_MAX_WORD_SIZE);

        if(!SPELLFixWord(word)) /* FALSE will filter the word */
            continue;

        if((result = SPELLFindWord(spellp, word)) == 0) { /* not found */
            if(head == NULL) {
                head = ValNodeNew(NULL);
                head->data.ptrvalue = StringSave(word);
                vnp_l = head;
            } else {
                vnp = ValNodeNew(NULL);
                vnp->data.ptrvalue = StringSave(word);
                vnp_l->next = vnp;
                vnp_l = vnp;
            }
        } else if(result < 0) {
            break;
        } else {
            continue;
        }
    } while (*strtmp != NULLB);
    
    MemFree(chptr);

    return head;
}

static SPELLDataPtr SPELLInit(CharPtr db_name, CharPtr stop_name)
{
    SPELLDataPtr spellp;
    Char buffer[PATH_MAX], buffer1[PATH_MAX];
    CharPtr path;

    spellp = MemNew(sizeof(SPELLData));
    
    if((path = getenv("SPELLCHECKDIR")) == NULL)
        path = ".";
    
    /* Initializing termlist */
    
    
    sprintf(buffer1, "%s/%s", path, db_name);      
    sprintf(buffer,  "%s/%s.idx", path, db_name);      
    
    if(FileLength(buffer) != 0 && FileLength(buffer1) != 0) {
        if((spellp->term_isamp = ISAMObjectNew(ISAMString, 
                                               buffer1, buffer)) == NULL) {
            ErrPostEx(SEV_ERROR, 993, 1, "Failed to create SISAM object");
            return NULL;
        }
    } else {
        ErrPostEx(SEV_ERROR, 993, 1, 
                  "Database or index term files do not exists");
        return NULL;
    }
    /* Initializing stopwords list */

    sprintf(buffer1, "%s/%s", path, stop_name);      
    sprintf(buffer,  "%s/%s.idx", path, stop_name);
    if(FileLength(buffer) != 0 && FileLength(buffer1) != 0) {
        if((spellp->stop_isamp = ISAMObjectNew(ISAMString, 
                                               buffer1, buffer)) == NULL) {
            ErrPostEx(SEV_ERROR, 993, 1, "Failed to create SISAM object");
            return NULL;
        }
    } else {
        ErrPostEx(SEV_ERROR, 993, 1, 
                  "Database or index term files do not exists");
        return NULL;
    }

    return spellp;
}
static void SPELLFree(SPELLDataPtr spellp)
{
    ISAMObjectFree(spellp->term_isamp);
    ISAMObjectFree(spellp->stop_isamp);
    
    MemFree(spellp);
    return;
}

Int2 Main(void)
{
    SpellRequestPtr splrp;
    SpellResponsePtr splbp;
    Boolean done = FALSE;
    short erract;
    ErrDesc err;
    NI_HandPtr hp;
    Char buf[100];
    AsnIoPtr asnin;
    AsnIoPtr asnout;
    int read_timeout;
    SPELLDataPtr spellp;
    ValNodePtr vnp;
    
    if (!GetArgs ("spellsrv", NUMARG, spellsrv_args) ) {
        return -1;
    }
    if (!ErrSetLog (LogFileName) ) {
        ErrShow();
    } else {
        ErrSetOpts (ERR_CONTINUE, ERR_LOG_ON);
    }    
    
    if((spellp = SPELLInit(DB_filename, STOP_filename)) == NULL)
        return 1;
    
    if (!StandAlone && !DebugMode) {
        NI_ServerACK();
        
        hp = NI_OpenASNIO();
        
        /* this read-timeout is effectively an idle timeout for */
        /* the server process; the process will terminate upon  */
        /* read-timeout                                         */
        GetAppParam("NCBI", "NET_SERV", "SERV_INACT_TIMER", "10",
                    buf, sizeof buf);
        read_timeout = atoi(buf) * 60; /* param is minutes */
        MsgSetReadTimeout(hp, read_timeout);
        
        asnin = hp->raip;
        asnout = hp->waip;
    } else if (StandAlone) {
        asnin = AsnIoNew(ASNIO_BIN_IN, stdin, NULL, NULL, NULL);
        asnout = AsnIoNew(ASNIO_BIN_OUT, stdout, NULL, NULL, NULL);        
    } else {
        asnin = AsnIoOpen("spellserv.inp", "r");
        asnout = AsnIoOpen("spellserv.out", "w");
    }

    while (!done) {
        /* encountering EOF on reading is a "normal" occurrence, */
        /* and does not merit an error message                   */
        ErrGetOpts(&erract, NULL);
        ErrSetOpts(ERR_IGNORE, 0);
        ErrFetch(&err); /* clear any pending error, which can be ignored */

        splrp = SpellRequestAsnRead(asnin, NULL);
        
        if (ErrFetch(&err))
        {
	    done = TRUE;
            Message(MSG_POST, "Error encountered on AsnReadId %d", err);
            break; /* client terminated */
        }
        ErrSetOpts(erract, 0);

        if (splrp == NULL)
        {
	    done = TRUE;
            Message(MSG_POST, "Null AsnReadId");
            break; /* client terminated */
        }

        switch (splrp->choice) {

        case SpellRequest_init:

            splbp = ValNodeNew(NULL);
            splbp->choice = SpellResponse_init;
            splbp->data.ptrvalue = NULL;
            SpellResponseAsnWrite (splbp, asnout, NULL);
            SpellResponseFree (splbp);
            splrp->data.ptrvalue = NULL;
            break;

        case SpellRequest_spelltext:

            vnp =  SPELLCheckString(spellp, (CharPtr) splrp->data.ptrvalue);
	    SPELLSendResponse((Pointer) vnp, asnout, 0);
	    break;

        case SpellRequest_fini:

            done = TRUE;
            splbp = ValNodeNew(NULL);
            splbp->choice = SpellResponse_fini;
            splbp->data.ptrvalue = NULL;

            SpellResponseAsnWrite (splbp, asnout, NULL);
            SpellResponseFree (splbp);
            splrp->data.ptrvalue = NULL;
            break;
        }

        AsnIoReset (asnout);
        SpellRequestFree (splrp);
    }

    AsnIoClose (asnin);
    AsnIoClose (asnout);
}
