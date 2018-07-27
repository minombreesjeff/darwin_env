/* $Id: wwwbutl.c,v 1.21 2004/01/16 17:35:20 dondosha Exp $
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
* File Name:  $RCSfile: wwwbutl.c,v $
*
* Author:  Sergei Shavirin
*
* Initial Version Creation Date: 04/21/2000
*
* $Revision: 1.21 $
*
* File Description:
*         WWW BLAST/PSI/PHI utilities
*
* $Log: wwwbutl.c,v $
* Revision 1.21  2004/01/16 17:35:20  dondosha
* Fixed mouseover problems
*
* Revision 1.20  2003/11/20 22:19:35  dondosha
* Pass www_root_path to the PrintDefLines... function
*
* Revision 1.19  2003/11/20 19:10:01  dondosha
* Never print header for XML output
*
* Revision 1.18  2003/08/13 14:35:33  dondosha
* When no percent identity cutoff, default value for mismatch penalty is -3 for blastn
*
* Revision 1.17  2003/07/15 15:07:39  dondosha
* Added NO_REFERENCE URL option to skip printing citation in BLAST report
*
* Revision 1.16  2003/06/16 19:19:29  dondosha
* Fixed memory allocation for lines and words when reading config file
*
* Revision 1.15  2003/05/21 18:49:25  dondosha
* Bug fix in setting subsequence range
*
* Revision 1.14  2003/05/13 22:00:37  dondosha
* More changes to make alternative links work
*
* Revision 1.13  2003/05/13 21:17:01  dondosha
* Pass www_blast_type to txalign in all cases
*
* Revision 1.12  2003/05/09 21:09:05  dondosha
* Removed unused variables
*
* Revision 1.11  2003/05/09 17:50:41  dondosha
* Fixed bug for database genetic code option
*
* Revision 1.10  2003/05/05 18:14:38  dondosha
* Added subsequence options
*
* Revision 1.9  2003/05/02 23:04:03  dondosha
* Fixed paths for non-standard setups, removed blast_form.map
*
* Revision 1.8  2003/04/29 15:55:56  dondosha
* Always use root path in links
*
* Revision 1.7  2003/04/04 22:40:44  dondosha
* Removed limit for the length of a line in the config file
*
* Revision 1.6  2003/03/03 21:44:26  dondosha
* Removed "/blast" from path to nph-viewgif.cgi in PSI/PHI output
*
* Revision 1.5  2003/02/10 21:48:36  dondosha
* Correction for multiple queries in form of accession
*
* Revision 1.4  2003/01/10 21:50:22  bealer
* - Modify to return errors from BLASTGetUidsFromQuery instead of logging them.
*
* Revision 1.3  2003/01/03 22:11:23  dondosha
* Allow multiple lines with database lists for the same program
*
* Revision 1.2  2002/12/02 21:21:22  dondosha
* 1. Use centralized utility to parse other advanced options;
* 2. Made it possible to use URL variable names same as in the NCBI BLAST URL API.
*
* Revision 1.1  2002/12/02 18:05:53  dondosha
* Moved from different locations to a common one
*
* Revision 6.33  2002/08/06 20:10:10  dondosha
* Added handling of the discontiguous word options
*
* Revision 6.32  2002/05/09 15:38:08  dondosha
* Call BLASTOptionNewEx instead of BLASTOptionNew, so megablast defaults are set in a central place
*
* Revision 6.31  2002/05/02 22:45:47  dondosha
* Do not break out of loop if SeqPortRead returns negative value, just skip the non-residue and continue
*
* Revision 6.30  2002/04/19 17:47:24  dondosha
* Removed restriction on the allowed number of databases in the config file
*
* Revision 6.29  2002/03/19 23:30:19  dondosha
* Do not increment options->wordsize by 4 for megablast any more
*
* Revision 6.28  2002/01/08 22:36:24  dondosha
* Added tabular output functionality
*
* Revision 6.27  2001/09/06 20:24:34  dondosha
* Removed threshold_first
*
* Revision 6.26  2001/07/20 19:56:04  dondosha
* Scale cutoff_s2 for megablast if match reward not 1
*
* Revision 6.25  2001/02/16 15:53:17  dondosha
* Cosmetic change
*
* Revision 6.24  2001/01/05 18:18:28  dondosha
* Change reward and penalty scores depending on percent identity
*
* Revision 6.23  2001/01/05 18:07:15  dondosha
* Added handling of word size and percent identity fields for options creation
*
* Revision 6.22  2000/11/16 22:35:40  dondosha
* Added lower case masking option and endpoints results option
*
* Revision 6.21  2000/10/31 20:21:26  shavirin
* Fixed bug with RedoAlignmentCore filtering.
*
* Revision 6.20  2000/10/16 22:18:35  shavirin
* Added possibility to perform OOF blastx
*
* Revision 6.19  2000/10/16 20:26:35  shavirin
* Added possibility to rum RPS Blast.
*
* Revision 6.18  2000/09/28 16:32:53  dondosha
* Compiler warning fix
*
* Revision 6.17  2000/09/27 22:18:03  shavirin
* Added possibility to limit search to results of entrez query.
*
* Revision 6.16  2000/09/12 22:01:42  dondosha
* Use matrix returned from search during formatting
*
* Revision 6.15  2000/09/08 20:16:59  dondosha
* Print informative error messages for bad accessions, still do search if at least one accession is good
*
* Revision 6.14  2000/09/08 17:46:54  dondosha
* Allow multiple accessions in input
*
* Revision 6.13  2000/09/07 18:02:58  dondosha
* If query has many sequences, put them in a SeqLoc list
*
* Revision 6.12  2000/09/01 21:47:34  dondosha
* Make check for wordsize too small; add 4 to user-supplied wordsize for megablast
*
* Revision 6.11  2000/09/01 17:50:59  dondosha
* No part of SeqEntry can be freed before the very end
*
* Revision 6.10  2000/09/01 17:30:26  dondosha
* Small corrections for megablast with Entrez client
*
* Revision 6.9  2000/08/30 22:20:24  dondosha
* Small changes for megablast web page
*
* Revision 6.8  2000/08/28 20:17:42  dondosha
* Added functionality for megablast web page
*
* Revision 6.7  2000/08/10 18:17:19  shavirin
* Used correct (fake) Bioseq in printing alignmenets.
*
* Revision 6.6  2000/08/10 14:50:37  shavirin
* Fixed 64 dependent bug
*
* Revision 6.4  2000/08/09 20:49:01  shavirin
* Added support for S&W Blast and XML output.
*
* Revision 6.3  2000/07/31 20:39:23  shavirin
* Some formating changes from Haruna Cofer (haruna@detroit.sgi.com)
*
* Revision 6.2  2000/07/26 02:26:16  shavirin
* Changes in accordance to Alejandro's S&W Blast.
*
* Revision 6.1  2000/05/17 15:53:40  shavirin
* Initial revision.
*
*
* ==========================================================================
*/

#include <wwwblast.h>

void WWWBlastInfoFree(WWWBlastInfoPtr theInfo)
{
    Int4 i;
    WWWInfoFree(theInfo->info);
    BLASTOptionDelete(theInfo->options);
    MemFree(theInfo->database);
    MemFree(theInfo->program);

    for(i = 0; i < theInfo->blast_config->num_dbs; i++) {
        MemFree(theInfo->blast_config->allow_db[i]);
    }
    MemFree(theInfo->blast_config->allow_db);
    MemFree(theInfo->blast_config);

    /* if(!theInfo->believe_query)
       fake_bsp = BlastDeleteFakeBioseq(fake_bsp); */

    MemFree(theInfo->blast_type);
    MemFree(theInfo->www_blast_type);
    MemFree(theInfo->ConfigFile);
    SeqLocSetFree(theInfo->query_slp);
    MemFree(theInfo->www_root_path);
    MemFree(theInfo);

    return;
}

void WWWBlastErrMessage(BLASTErrCode error_code, CharPtr error_msg)
{
   WWWBlastErrMessageEx(error_code, error_msg, NULL);
}

void WWWBlastErrMessageEx(BLASTErrCode error_code, CharPtr error_msg,
                          CharPtr seq_info)
{
    CharPtr delim = "<BR>";

    if(error_code == BLASTNoError)
	return;

    fprintf(stdout, "<FONT color=red><h3>");
    fprintf(stdout, "Error %ld in submitting BLAST query", labs(error_code));
    if (seq_info)
       fprintf(stdout, "</h3></FONT> <BR> <b> Accession: %s </b> <BR> <HR>\n<b>", seq_info);
    else
       fprintf(stdout, "</h3></FONT><HR>\n<b>");
    fprintf(stdout, "Short error description:");
    
    fprintf(stdout, "</b><BR><BR>\n");
    switch(error_code) {
        
    case BLASTEntrez:
        
        fprintf(stdout,
                "Your input sequence may not be found in Entrez %s"
                "or Entrez access interface currently unavailable. %s"
                "Please send message to blast_help@ncbi.nlm.nih.gov %s"
                "with description of your query", 
                delim, delim, delim);
        break;
        
    case BLASTFastaToSE:
        
        fprintf(stdout,
                "Your input sequence formatted incorrectly. %s"
                "Please read blast help if you have problems with formatting %s"
                "or send request to blast help account.", 
                delim, delim);
        break;
        
    case BLASTErrNoSequence:
        
        fprintf(stdout,
                "Input sequence for the BLAST search, probably missing. %s"
                "Please see the blast help for a description %s"
                "of the FASTA sequence format.", 
                delim, delim);
        break;
        
    case BLASTErrCombination:
        
        fprintf(stdout, 
                "The combination of database and program, that you provided in your %s"
                "message is invalid or not acceptable by BLAST search system. %s"
                "Please look at current possible combinations in BLAST help. ",
                delim, delim);
        break;
        
    case BLASTErrAccType:
        
        fprintf(stdout, 
                "You specified a protein (or nucleotide) sequence identifier, %s"
                "but a nucleotide (or protein) sequence is required for your search.",
                delim);
        
        break;
        
    case BLASTErrDatalib:
        
        fprintf(stdout, "No database was specified.  ");
        break;
        
    case BLASTErrNoQueue:
        fprintf(stdout, 
                "Unable to accept more BLAST jobs right now, %s"
                "Queue overloaded. Please try again later.", 
                delim);
        break;

    case BLASTOptionStr:
        
        if(error_msg != NULL) {
            fprintf(stdout, "%s", error_msg);
        }
        break;
        
    case BLASTMiscError:
    default:
        
        if(error_msg != NULL) {
            fprintf(stdout, "%s %s", error_msg, delim);
        } else {
            fprintf(stdout, 
                    "There were some internal software problems while processing %s"
                    "your request. Please contact blast support with a full %s"
                    "description of your query to BLAST as soon as possible.",
                    delim, delim); 
        }
        break;
    }
    
    fprintf(stdout, "\n<HR>\n");

    printf("</BODY>\n");
    printf("</HTML>\n");
    fflush(stdout);
    return;
}

Boolean BLAST_Time(CharPtr string, Int4 len, time_t seconds)
{
    CharPtr chptr;
    
    if(string == NULL || len < 25)
	return FALSE;
    
    if(!seconds) {
	seconds = GetSecs();
    }
    
    if((chptr = ctime(&seconds)) != NULL) 
        StringCpy(string, chptr);
    
    string[24] = NULLB;
    return TRUE;
}

WWWBlastInfoPtr WWWBlastReadArgs(CharPtr type)
{
    WWWBlastInfoPtr theInfo;
    CharPtr blast_type, hostname, chptr;
    Char tmp[256];
    FILE *log_fd;

    theInfo = MemNew(sizeof(WWWBlastInfo));
    
    if(WWWGetArgs(&theInfo->info) != WWWErrOk) {
        WWWBlastErrMessage(BLASTMiscError, NULL);
        return NULL;    
    }

    if((chptr = WWWGetQuery(theInfo->info)) == NULL || *chptr == NULLB) {
        fprintf(stdout, "<META HTTP-EQUIV=\"Refresh\" "
                "CONTENT=\"2; URL=%s.html\">", type ? type : "blast");
        return NULL;
    }
    
#ifdef PRINT_ALL_INPUT  /* Printing out all coming data for debugging */
  {
    Int4 index;
    for(index= 0; index < WWWGetNumEntries(theInfo->info); index ++) {
        printf("%s : %s<BR>", 
               WWWGetNameByIndex(theInfo->info, index), 
               WWWGetValueByIndex(theInfo->info, index));
    }
  }
#endif
    
    if(getenv("DEBUG_COMMAND_LINE") != NULL) {
        FILE *fd;
        fd = FileOpen("/tmp/__web.in", "w");
        fprintf(fd, "%s", ((WWWInfoDataPtr)theInfo->info)->query);
        FileClose(fd);
    }

    /* Root path for PSI/PHI Blast images */ 
    if ((chptr = getenv("WWW_ROOT_PATH")) != NULL)
       theInfo->www_root_path = StringSave(chptr);
    else
       theInfo->www_root_path = StringSave("..");

    if ( !ErrSetLogfile ("/dev/null", ELOG_APPEND) ) {
        fprintf(stdout, "Cannot set logfile exiting....\n");
        return FALSE;
    } else {
        ErrSetOpts (ERR_CONTINUE, ERR_LOG_ON);
    }
    
    /* Config file with program/database relationsship */
    
    blast_type = WWWGetValueByName(theInfo->info, "BLAST_TYPE");
    
    if(blast_type == NULL || *blast_type == NULLB) {
        theInfo->blast_type = StringSave(type ? type : "blast");
        sprintf(tmp, "%s.rc", theInfo->blast_type); 
        theInfo->ConfigFile = StringSave(tmp);
    } else {
        sprintf(tmp, "%s.rc", blast_type);
        theInfo->blast_type = StringSave(blast_type);
        theInfo->ConfigFile = StringSave(tmp);
    }
    
    sprintf(tmp, "%s.log", blast_type == NULL? "wwwblast" : blast_type);

    /* The following is used for links in formatted output */
    theInfo->www_blast_type = WWWGetValueByName(theInfo->info, "WWW_BLAST_TYPE");

    log_fd = FileOpen(tmp, "a"); 

    if(log_fd == NULL) /* If log_fd == NULL - no problem */
        return theInfo;
    
    BLAST_Time(tmp, sizeof(tmp), 0);

    if((hostname = getenv("PROXIED_IP")) == NULL)
        hostname = WWWGetAddress(theInfo->info);
    
    fprintf(log_fd, "\n%d|%s|%s|%s",
            getpid(), tmp, hostname == NULL? "host_not_set" : hostname,
            WWWGetAgent(theInfo->info));
    
    FileClose(log_fd);
    
    return theInfo;
}

/* Set of functions to handle BLAST custom configuration file */
static BLASTConfigPtr BLASTConfigNew(void)
{
    BLASTConfigPtr config;
    
    if((config = (BLASTConfigPtr) MemNew(sizeof(BLASTConfig))) == NULL)
	return NULL;
    
    config->run_max = DEFAULT_RUN_MAX;
    config->queue_max = DEFAULT_QUEUE_MAX;
    config->num_cpu = NUM_CPU_TO_USE;
    config->allow_db = (CharPtr PNTR) MemNew(INIT_DB_NUM*sizeof(CharPtr));
    config->db_num_allocated = INIT_DB_NUM;

    return config;
}
static Int4 BLASTEatWs (FILE* fp)
{
    Int4 ch;

    while ((ch = fgetc (fp)) != EOF) {
	if (ch != ' ' && ch != '\t')
	    return ch;
    }
    return ch;
}

static void BLASTConfigGetWord(CharPtr word, CharPtr PNTR line_ptr) 
{
    Int4 x = 0, y = 0;
    CharPtr line = *line_ptr;
    
    for(x=0; line[x] && IS_WHITESP(line[x]); x++);

    while(TRUE) {
       if(!(word[y] = line[x]))
          break;
       if(IS_WHITESP(line[x]))
          if((!x) || (line[x-1] != '\\'))
             break;
       if(line[x] != '\\') ++y;
       ++x;
    }
    word[y] = '\0';
    
    while(line[x] && IS_WHITESP(line[x])) ++x;

    *line_ptr = &line[x];
}

static Int4 BLASTConfigGetLine (CharPtr PNTR line, Int4Ptr size, FILE* fp)
{
    int   len = 0, ch;
    CharPtr s = *line, new_s;
    Int4 n = *size;

    ch = BLASTEatWs(fp);

    while (TRUE) {
       if (ch == EOF || ch == '\n') {
          if (len && s[len - 1] == ' ') s[len - 1] = '\0';
          else s[len] = '\0';
          break;
       } else if (len >= n-2) {
          if ((new_s = Realloc(s, 2*n)) == NULL) {
             if (len && s[len - 1] == ' ') s[len - 1] = '\0';
             else s[len] = '\0';
             break;
          }
          n *= 2;
          s = new_s;
       }
       s[len++] = ch;
       ch = fgetc (fp);
       
       if (ch == '\t' || ch == ' ') {
          s[len++] = ch;
          ch = BLASTEatWs(fp);
       }
    }
    *line = s;
    *size = n;

    return feof(fp) ? 1 : 0;
}

#define MAX_LINE_SIZE 2048
static BLASTConfigPtr BLASTReadConfigFile(CharPtr filename, CharPtr program)
{
    FILE *fd;
    BLASTConfigPtr config;
    CharPtr line, word, line_start;
    Int4 value, i;
    Int4 line_size = MAX_LINE_SIZE;
    
    if(filename == NULL)
	return NULL;
    
    if((config = BLASTConfigNew()) == NULL)
       return NULL;
    
    if((fd = FileOpen(filename, "r")) == NULL)
       return NULL;

    line_start = line = Malloc(line_size);
    word = Malloc(line_size);

    while(!(BLASTConfigGetLine(&line_start, &line_size, fd))) {
       line = line_start;
       if((line[0] != '#') && (line[0] != '\0')) {
          BLASTConfigGetWord(word, &line);
          
          if(!StringICmp(word, "RunMaxProcesses") && 
             (value = atoi(line)) != 0) {
             config->run_max = value;
          } else if(!StringICmp(word, "QueueMaxJobs") && 
                    (value = atoi(line)) != 0) {
             config->queue_max = value;
          } else if(!StringICmp(word, "NumCpuToUse") && 
                    (value = atoi(line)) != 0) {
             config->num_cpu = value;
          } else if(!StringICmp(word, "NiceValue") && 
                    (value = atoi(line)) != 0) {
             config->niceval = value;
          } else if(!StringICmp(word, program)) {
             for(i = config->num_dbs; line[0] != NULLB; i++) {
                BLASTConfigGetWord(word, &line);
                if (i >= config->db_num_allocated) {
                   CharPtr PNTR ptr;
                   if ((ptr = (CharPtr PNTR)
                        Realloc(config->allow_db, 
                                2*config->db_num_allocated*sizeof(CharPtr)))
                       != NULL) {
                      config->allow_db = ptr;
                      config->db_num_allocated *= 2;
                   } else {
                      break;
                   }
                }
                config->allow_db[i] = StringSave(word);
             }
             config->num_dbs = i;
          }
       }
    }

    MemFree(line_start);
    MemFree(word);

    FileClose(fd);
    return config;
}

static Boolean ValidateCombinationsEx(WWWBlastInfoPtr theInfo, 
                                      CharPtr database)
{
    Int4 i;
    
    for(i = 0; i < theInfo->blast_config->num_dbs; i++) {
	if(!StringICmp(database, theInfo->blast_config->allow_db[i]))
	    return TRUE;
    }
    return FALSE;
}

/* This will work if search require to choose few databases */
static Boolean WWWParseDatabases(WWWBlastInfoPtr theInfo)
{
    Int4  count, index;
    Boolean done, datalib_found;
    Char buffer[4096], buffer1[4096]; /* is 4096 always long enough? XXX */
    CharPtr ptr, chptr;

    count = WWWGetNumEntries(theInfo->info);
    datalib_found = FALSE;
    ptr = buffer;
    
    for (index=0; index<count; index++) {
	chptr = WWWGetNameByIndex(theInfo->info, index);
	if (StringCmp(chptr, "DATALIB") == 0 || 
            StringCmp(chptr, "DATABASE") == 0) {
	    datalib_found = TRUE;
	    chptr = WWWGetValueByIndex(theInfo->info, index);
	    done = FALSE;
            
            /* Parse string if multiple database names. */
	    while (done == FALSE) { 
		done = readdb_parse_db_names(&chptr, buffer1);
		if (ValidateCombinationsEx(theInfo, buffer1) == TRUE) {
                    
		    CharPtr prefix = WWWGetValueByName(theInfo->info, "DB_DIR_PREFIX");
		    Char tmpbuf[1024];
                    
		    if (prefix) {
			sprintf(tmpbuf, "%s%c%s", prefix, DIRDELIMCHR, buffer1);
		    } else {
			sprintf(tmpbuf, "%s", buffer1);
		    }
		    
		    StringCpy(ptr, tmpbuf);
		    ptr += StringLen(tmpbuf);
		    *ptr = ' '; ptr++;
		} else {
                    WWWBlastErrMessage(BLASTErrCombination, NULL);
                    return FALSE;
		}
	    }
	}
    }
    
    if (datalib_found) {
	ptr--;
	*ptr = NULLB;
	theInfo->database = StringSave(buffer);
    } else {
        WWWBlastErrMessage(BLASTErrDatalib, NULL);
	return FALSE;
    }
    
    /* Processing database aliases  */
    
    if(StringStr(theInfo->database, "E.coli") != NULL) {
	MemFree(theInfo->database);
	theInfo->database = StringSave("ecoli");
    }

    return TRUE;
}
#if defined(NCBI_CLIENT_SERVER) || defined (NCBI_ENTREZ_CLIENT)

static Int4 AccessionToGi (CharPtr string) 
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
#endif

#define MAX_NUM_QUERIES 16383 /* == 1/2 INT2_MAX */
Boolean WWWCreateSearchOptions(WWWBlastInfoPtr theInfo)
{
    CharPtr chptr, sequence, outptr;
    SeqEntryPtr sep;
    Boolean gapped_set;
    BLAST_OptionsBlkPtr options;
    Char tmp[128];
    Int4 value;
    Boolean is_megablast = FALSE, mask_lower_case = FALSE;
    SeqLocPtr query_lcase_mask = NULL;
    Int4 query_from, query_to;
    
    /* PROGRAM */
    
    if((chptr = WWWGetValueByName(theInfo->info, "PROGRAM")) != NULL) {
	theInfo->program = StringSave(chptr);
    } else {
        theInfo->program = StringSave("blastn");
        /*WWWBlastErrMessage(BLASTErrProgram, NULL);
          return FALSE;*/
    }

    /* Is it MEGABLAST? */

    if (WWWGetValueByName(theInfo->info, "MEGABLAST") != NULL)
       is_megablast = TRUE;

    /* Configuration file set program/database relations */

    if((theInfo->blast_config = 
        BLASTReadConfigFile(theInfo->ConfigFile, theInfo->program)) == NULL) {
        WWWBlastErrMessage(BLASTConfigFile, NULL);
	return FALSE;
    }

    /* DATALIB */
    if(!WWWParseDatabases(theInfo))
        return FALSE;
    
    /* SEQUENCE or SEQFILE */
    
    if((sequence = WWWGetValueByName(theInfo->info, "SEQUENCE")) == NULL)
       sequence = WWWGetValueByName(theInfo->info, "QUERY");

    if (sequence == NULL || sequence[0] == NULLB) {
        if((sequence = WWWGetValueByName(theInfo->info, "SEQFILE")) == NULL)
           sequence = WWWGetValueByName(theInfo->info, "QUERY_FILE");
        if (sequence == NULL || sequence[0] == NULLB) {
            WWWBlastErrMessage(BLASTErrNoSequence, NULL);
            return FALSE;
        }
    }

    /* Subsequence coordinates */
    if ((chptr = WWWGetValueByName(theInfo->info, "QUERY_FROM")) != NULL
         && *chptr != NULLB) {
       query_from = MAX(atoi(chptr), 0);
    } else {
       query_from = 0;
    }

    if ((chptr = WWWGetValueByName(theInfo->info, "QUERY_TO")) != NULL
         && *chptr != NULLB) {
       query_to = MAX(atoi(chptr), -1);
    } else {
       query_to = -1;
    }
        
    theInfo->align_type = BlastGetTypes(theInfo->program, &theInfo->query_is_na, &theInfo->db_is_na);
#if defined(NCBI_CLIENT_SERVER) || defined (NCBI_ENTREZ_CLIENT)
    
    if((chptr = WWWGetValueByName(theInfo->info, "INPUT_TYPE")) != NULL && 
       !StringNICmp(chptr, "Accession", 9)) {

        Int4 gi, number, title_length, id_length, query_num = 0;
        CharPtr accession, new_defline;
        BioseqPtr bsp_tmp, query_bsp;
        SeqIdPtr sip;
        ObjectIdPtr  oid;
        SeqPortPtr spp;
        Int2 retval, buf_length=512;
        Uint1 buf[512];
        Char tmp[255];
        Boolean first_query = TRUE;
        Char delimiters[7];

	/* This is request by Accession/GI - asking ID */
        
        if (!ID1BioseqFetchEnable("web-blasts", TRUE)) {
            WWWBlastErrMessage(BLASTEntrez, NULL);
	    return FALSE;
        }
        
	chptr = sequence;
        theInfo->query_slp = NULL;
        StrCpy(delimiters, " \t\n,;\r");

        while (outptr = StringTokMT(chptr, delimiters, &chptr)) {

           accession = outptr;

           /* Strip off non-alphanumerics, except for '_' (used in SP) and '.' (soon to be used by  the collaboration. */
           while (IS_ALPHANUM(*outptr) || *outptr == '_' || *outptr == '.')
              outptr++;
           *outptr = NULLB; 
           
           sip = NULL;
           gi = AccessionToGi(accession);
           
           if (gi > 0) {
              ValNodeAddInt(&sip, SEQID_GI, gi);
           } else {
              WWWBlastErrMessageEx(BLASTEntrez, NULL, accession);
              if (!theInfo->query_slp && !chptr) 
                 return FALSE;
              else continue;
           }	
           
           /* If id is not found - it is not found - ID1 is down? */
           
           if((bsp_tmp = BioseqLockById(sip)) == NULL) {
              WWWBlastErrMessageEx(BLASTEntrez, NULL, accession);
              if (!theInfo->query_slp && !chptr) 
                 return FALSE;
              else continue;
           }
           
           if (ISA_na(bsp_tmp->mol) != theInfo->query_is_na) {
              WWWBlastErrMessageEx(BLASTErrAccType, NULL, accession);
              if (!theInfo->query_slp && !chptr) 
                 return FALSE;
              else continue;
           }
           
           query_bsp = BioseqNew();
           query_bsp->length = bsp_tmp->length;
           query_bsp->mol = bsp_tmp->mol;
           query_bsp->repr = Seq_repr_raw;
           query_bsp->seq_data = BSNew(query_bsp->length);
           
           if (ISA_na(query_bsp->mol)) {
              spp = SeqPortNew(bsp_tmp, 0, -1, Seq_strand_plus, 
                               Seq_code_iupacna);
              query_bsp->seq_data_type = Seq_code_iupacna;
           } else {
              spp = SeqPortNew(bsp_tmp, 0, -1, Seq_strand_unknown, 
                               Seq_code_ncbieaa);
              query_bsp->seq_data_type = Seq_code_ncbieaa;
           }
           
           SeqPortSet_do_virtual(spp, TRUE);
           number = 0;
           while (number < query_bsp->length) {
              retval = SeqPortRead(spp, buf, buf_length);
              if (retval < 0)
                 continue;
              if (retval == 0)
                 break;
              BSWrite(query_bsp->seq_data, buf, retval);
              number += retval;
           }
           
           SeqPortFree(spp);
           
           title_length = StringLen(BioseqGetTitle(bsp_tmp));
           SeqIdWrite(bsp_tmp->id, tmp, PRINTID_FASTA_LONG, 255);
           id_length = StringLen(tmp);
           title_length += id_length;
           title_length +=3;
           new_defline = (CharPtr) MemNew(title_length*sizeof(Char));
           StringCpy(new_defline, tmp);
           *(new_defline+id_length) = ' ';
           StringCpy(new_defline+id_length+1, BioseqGetTitle(bsp_tmp)); 
           *(new_defline+title_length-1) = NULLB;
           query_bsp->descr = ValNodeAddStr(NULL, Seq_descr_title, 
                                            new_defline);
           query_bsp->id = ValNodeNew(NULL);
           oid = ObjectIdNew();
           query_num++;
           oid->str = (CharPtr) Malloc(6);
           sprintf(oid->str, "%d", query_num);
           query_bsp->id->choice = SEQID_LOCAL;
           query_bsp->id->data.ptrvalue = (Pointer) oid;
        
           SeqMgrDeleteFromBioseqIndex(bsp_tmp);
           
           BioseqUnlock(bsp_tmp);
           
           BioseqPack(query_bsp);
           if (first_query) {
              theInfo->query_bsp = query_bsp;
              first_query = FALSE;
           }
        
           if (query_from == 0 && query_to == -1) {
              ValNodeAddPointer(&theInfo->query_slp, SEQLOC_WHOLE,
                 SeqIdSetDup(SeqIdFindBest(query_bsp->id, SEQID_GI)));
           } else {
              if (query_to == -1 || query_to > query_bsp->length)
                 query_to = query_bsp->length - 1;
              theInfo->query_slp = 
                 SeqLocIntNew(query_from, query_to, Seq_strand_both, 
                    SeqIdSetDup(SeqIdFindBest(query_bsp->id, SEQID_GI)));
              /* Only one query permitted if subsequence coordinates are
                 provided */
              break;
           }
        }
        
        ID1BioseqFetchDisable();
    }
#endif

    /* LCASE_MASK */
    if (WWWGetValueByName(theInfo->info, "LCASE_MASK") != NULL)
       mask_lower_case = TRUE;


    /* Creating Bioseq */
        
    if (theInfo->query_bsp == NULL) {
	  Int4 num_queries;
	  BioseqPtr query_bsp = NULL;
	  SeqLocPtr last_mask, mask_slp;
	  Int2 ctr = 1;
	  Char prefix[2];
	  
          outptr = NULL;
	  StrCpy(prefix, "");
	  SeqMgrHoldIndexing(TRUE);
	  mask_slp = last_mask = NULL;
	  num_queries = 0;
	  while ((sep=FastaToSeqBuffForDb(sequence, &outptr,
					  theInfo->query_is_na, NULL,
					  theInfo->believe_query, 
                                          prefix, &ctr, &mask_slp)) != NULL) {
             sequence = outptr;
	     if (mask_slp) {
                if (!mask_lower_case)
                   SeqLocFree(mask_slp);
		else if (!last_mask)
		   query_lcase_mask = last_mask = mask_slp;
		else {
		   last_mask->next = mask_slp;
		   last_mask = last_mask->next;
		}
		mask_slp = NULL;
	     }
	     query_bsp = NULL;
	     if (theInfo->query_is_na) 
		SeqEntryExplore(sep, &query_bsp, FindNuc);
	     else
		SeqEntryExplore(sep, &query_bsp, FindProt);
	     
	     if (query_bsp == NULL) {
		ErrPostEx(SEV_FATAL, 0, 0, "Unable to obtain bioseq\n");
		return 2;
	     }
	     num_queries++;
	     if (num_queries > MAX_NUM_QUERIES) {
		WWWBlastErrMessage(BLASTOptionStr, 
				   "Maximal number of queries exceeded. At most 16383 are allowed");
		break;
	     }
	     if (!theInfo->query_slp) /* I.e. if first query */
		theInfo->query_bsp = query_bsp;

             if (query_from == 0 && query_to == -1) {
                ValNodeAddPointer(&theInfo->query_slp, SEQLOC_WHOLE,
                   SeqIdSetDup(SeqIdFindBest(query_bsp->id, SEQID_GI)));
             } else {
                if (query_to == -1 || query_to > query_bsp->length)
                   query_to = query_bsp->length - 1;
                theInfo->query_slp = 
                   SeqLocIntNew(query_from, query_to, Seq_strand_both, 
                      SeqIdSetDup(SeqIdFindBest(query_bsp->id, SEQID_GI)));
                /* Only one query permitted if subsequence coordinates are
                   provided */
                break;
             }
	  }
	  SeqMgrHoldIndexing(FALSE);
    } else if (is_megablast && !theInfo->query_slp) {
       if (query_from == 0 && query_to == -1) {
          ValNodeAddPointer(&theInfo->query_slp, SEQLOC_WHOLE,
             SeqIdSetDup(SeqIdFindBest(theInfo->query_bsp->id, SEQID_GI)));
       } else {
          if (query_to == -1 || query_to > theInfo->query_bsp->length)
             query_to = theInfo->query_bsp->length - 1;
          theInfo->query_slp = 
             SeqLocIntNew(query_from, query_to, Seq_strand_both, 
                SeqIdSetDup(SeqIdFindBest(theInfo->query_bsp->id, SEQID_GI)));
       }
    }
       
    /* The last check of Bioseq - if length of sequence too small ? */
    
    if(theInfo->query_bsp == NULL || 
       theInfo->query_bsp->length <= 0) {
        WWWBlastErrMessage(BLASTFastaToSE, NULL);
        return FALSE;
    }
    
    /* This will prevent from incorrect formating in case when input
       sequence has valid SeqId, but in fact this SeqId do not correspond
       to the real sequence  - XXX */

    if(!theInfo->believe_query)
        theInfo->fake_bsp = BlastMakeFakeBioseq(theInfo->query_bsp, NULL);
    else
        theInfo->fake_bsp = theInfo->query_bsp;
    
    /* OVERVIEW */
    
    if (WWWGetValueByName(theInfo->info, "OVERVIEW") != NULL ||
        WWWGetValueByName(theInfo->info, "SHOW_OVERVIEW") != NULL)
        theInfo->show_overview = TRUE;

    /* UNGAPPED_ALIGNMENT */
    gapped_set = TRUE;
    if(WWWGetValueByName(theInfo->info, "UNGAPPED_ALIGNMENT") != NULL)
	gapped_set = FALSE;

    if((options = BLASTOptionNewEx(theInfo->program, gapped_set, is_megablast)) == NULL) {
        WWWBlastErrMessage(BLASTErrOptions, NULL);
	return FALSE; 
    }
    
    if (is_megablast) {
       options->query_lcase_mask = query_lcase_mask;
       if((chptr = WWWGetValueByName(theInfo->info, "WORD_SIZE")) != NULL &&
          StringStr(chptr, "default") == NULL) {
          options->wordsize = atoi(chptr);
       }
       /* Mega BLAST with no traceback (endpoints only)? */
       if (WWWGetValueByName(theInfo->info, "ENDPOINTS") != NULL)
          options->no_traceback = TRUE;
       if ((chptr = WWWGetValueByName(theInfo->info, "PERC_IDENT")) != NULL && 
           StringStr(chptr, "default") == NULL)
          options->perc_identity = (FloatLo) atof(chptr);
    }

    theInfo->options = options;

    /* Set default values for matrix and gap parameters */
    BLASTOptionSetGapParams (options, NULL, 0, 0);
    
    /* Read MAT_PARAM if set */
    
    if(StringICmp("blastn", theInfo->program) && 
       (chptr = WWWGetValueByName(theInfo->info, "MAT_PARAM")) != NULL) {
        Char    matrixname[64];
        Int4    opencost, extendedcost;
        /* Get matrix name and gap costs */
        if (chptr[1] != '-' || chptr[2] != '-') {
            sscanf(chptr, "%s\t %d\t %d", matrixname, &opencost, 
                   &extendedcost);
            /* set the parameters */
            options->gap_open  = opencost;
            options->gap_extend  = extendedcost;
            if (options->matrix)
                MemFree(options->matrix);
            options->matrix = StringSave(matrixname);
        }
    } 

    if((chptr = WWWGetValueByName(theInfo->info, "GAP_OPEN")) != NULL &&
	    StringStr(chptr, "default") == NULL) {
	options->gap_open = atoi(chptr);
    }

    if((chptr = WWWGetValueByName(theInfo->info, "GAP_EXTEND")) != NULL &&
	    StringStr(chptr, "default") == NULL) {
	options->gap_extend = atoi(chptr);
    }

    if((chptr = WWWGetValueByName(theInfo->info, "GAP_VALUES")) != NULL &&
	    StringStr(chptr, "default") == NULL) {
	sscanf(chptr, "%d,%d", &options->gap_open, &options->gap_extend);
    }

    if((chptr = WWWGetValueByName(theInfo->info, "X_DROPOFF")) != NULL &&
	    StringStr(chptr, "default") == NULL) {
	options->gap_x_dropoff = atoi(chptr);
    }

    if (!StringICmp(theInfo->program, "blastn")) {
       if (options->perc_identity >= 95.0 || options->perc_identity == 0.0) {
          options->reward = 1;
          options->penalty = -3;
       } else if (options->perc_identity >= 85.0) {
          options->reward = 1;
          options->penalty = -2;
       } else if (options->perc_identity >= 80.0) {
          options->reward = 2;
          options->penalty = -3;
       } else if (options->perc_identity >= 75.0) {
          options->reward = 4;
          options->penalty = -5;
       } else if (options->perc_identity >= 60.0) {
          options->reward = 2;
          options->penalty = -2;
       }
    }

    /* The discontiguous word options */
    if ((chptr = WWWGetValueByName(theInfo->info, "TEMPLATE_LENGTH")) != NULL)
       options->mb_template_length = atoi(chptr);

    if (WWWGetValueByName(theInfo->info, "TWO_HITS") != NULL)
       options->window_size = 40;
    if ((chptr = WWWGetValueByName(theInfo->info, "TEMPLATE_TYPE")) != NULL)
       options->mb_disc_type = (MBDiscWordType) atoi(chptr);

    /* Set the gap penalties depending on the percent identity cutoff, if
       the discontiguous word approach is used. Also make sure non-greedy
       dynamic programming algorithm is used for gapped extension */
    if (options->mb_template_length > 0 && options->wordsize <= 12 && 
        options->wordsize >= 11 && options->perc_identity < 90.0) {
       options->mb_use_dyn_prog = TRUE;
       if (options->perc_identity >= 85.0) {
          options->gap_open = 4;
          options->gap_extend = 1;
       } else if (options->perc_identity >= 80.0) {
          options->gap_open = 5;
          options->gap_extend = 2;
       } else if (options->perc_identity >= 75) {
          options->gap_open = 7;
          options->gap_extend = 2;
       } else if (options->perc_identity >= 60) {
          options->gap_open = 3;
          options->gap_extend = 1;
       }
    }

    if((chptr = WWWGetValueByName(theInfo->info, "GAP_SIZE")) != NULL &&
	    StringStr(chptr, "default") == NULL) {
	options->gap_size = atoi(chptr);
    }

    if((chptr = WWWGetValueByName(theInfo->info, 
	    "WINDOW_SIZE")) != NULL &&
	    StringStr(chptr, "default") == NULL) {
	options->window_size = atoi(chptr);
    }

    /* For BLASTX we could set genetic code */

    if (!StringICmp(theInfo->program, "blastx")) {
        BioSourcePtr source;
        
	options->genetic_code = 1;

	if((chptr = WWWGetValueByName(theInfo->info, 
		"GENETIC_CODE")) != NULL &&
		(StringStr(chptr, "default") == NULL)) {
	    chptr = StringChr(chptr, '(');
	    sscanf(chptr, "(%d", &value);
	    if(value != 0) {
		options->genetic_code = value; 

		source = BioSourceNew();
		source->org = OrgRefNew();
		source->org->orgname = OrgNameNew();
		source->org->orgname->gcode = options->genetic_code;
		ValNodeAddPointer(&theInfo->query_bsp->descr, 
			Seq_descr_source, source);
	    }
	}
    }

    /* For BLASTX we could set genetic code */

    if (!StringICmp(theInfo->program, "blastx")) {
	if((chptr = WWWGetValueByName(theInfo->info, "OOF_ALIGN")) != NULL) {
            options->is_ooframe = TRUE;
            options->shift_pen = atoi(chptr);

            if(options->shift_pen == 0) {
                options->is_ooframe = FALSE;
            }
        }
    }

    /* For TBLASTN and TBLASTX we could set DB_GENETIC_CODE */

    if (!StringICmp(theInfo->program, "tblastn") || 
        !(StringICmp(theInfo->program, "tblastx"))) {
	options->db_genetic_code = 1;
	if((chptr = WWWGetValueByName(theInfo->info, 
                                      "DB_GENETIC_CODE")) != NULL &&
           (StringStr(chptr, "default") == NULL)) {
	    chptr = StringChr(chptr, '(');
	    sscanf(chptr, "(%d", &value);
	    if(value != 0) {
		options->db_genetic_code = value; 
	    }      
	}
    }

    if((chptr = WWWGetValueByName(theInfo->info, 
	    "THRESHOLD_2")) != NULL &&
	    (StringStr(chptr, "default") == NULL)) {
	options->threshold_second = atoi(chptr);
    }

    if((chptr = WWWGetValueByName(theInfo->info, 
	    "REQUIRED_START")) != NULL &&
	    StringStr(chptr, "default") != NULL) {
	options->required_start = atoi(chptr);
    }

    if((chptr = WWWGetValueByName(theInfo->info, 
	    "REQUIRED_END")) != NULL &&
	    StringStr(chptr, "default") != NULL) {
	options->required_end = atoi(chptr);
    }

    if((chptr = WWWGetValueByName(theInfo->info, 
	    "DROPOFF_1")) != NULL &&
	    (StringStr(chptr, "default") == NULL)) {
	options->dropoff_1st_pass = atof(chptr);
    }

    if((chptr = WWWGetValueByName(theInfo->info, 
	    "CUTOFF")) != NULL &&
	    (StringStr(chptr, "default") == NULL)) {
	options->cutoff_s = atof(chptr);
    }


    if((chptr = WWWGetValueByName(theInfo->info, 
	    "DROPOFF_2")) != NULL &&
	    (StringStr(chptr, "default") == NULL)) {
	options->dropoff_2nd_pass = atof(chptr);
    }

    /* MATRIX: */
    if ((chptr = WWWGetValueByName(theInfo->info, "MATRIX")) == NULL)
       chptr = WWWGetValueByName(theInfo->info, "MATRIX_NAME");

    if(chptr != NULL && (StringStr(chptr, "default") == NULL)) {
	options->matrix = StringSave(chptr);
    }

    /* EXPECT */

    options->expect_value  = DEFAULT_EXPECT;

    if((chptr = WWWGetValueByName(theInfo->info, 
	    "EXPECT")) != NULL &&
	    StringStr(chptr, "default") == NULL) {
	options->expect_value = atof(chptr);
    }

    /* NUMBER OF BITS: */

    if((chptr = WWWGetValueByName(theInfo->info, 
	    "NUM_OF_BITS")) != NULL &&
	    (StringStr(chptr, "default") == NULL)) {
	options->number_of_bits = atof(chptr);
    }

    /* RPSBLAST */

    if (WWWGetValueByName(theInfo->info, "RPSBLAST") != NULL)
        options->is_rps_blast = TRUE;

    /* ENTREZ_QUERY */
    
    if ((chptr = WWWGetValueByName(theInfo->info, "ENTREZ_QUERY")) != NULL) {
        options->entrez_query = StringSave(chptr);
    
#if defined(NCBI_CLIENT_SERVER) || defined (NCBI_ENTREZ_CLIENT)
        {{
            Int4 i;
            Int4Ptr uids;

            theInfo->gi_list_total = 
                BLASTGetUidsFromQuery(chptr, &uids, theInfo->db_is_na, FALSE, 0);

            if(theInfo->gi_list_total > 0) {

                theInfo->gi_list = MemNew(theInfo->gi_list_total*sizeof(BlastDoubleInt4));
                for(i = 0; i < theInfo->gi_list_total; i++)
                    theInfo->gi_list[i].gi = uids[i];
            }
        }}
#endif
    }
    /* ----------------------------------- */

    /* Number of CPU to use in BLAST Search: */

    if(theInfo->blast_config->num_cpu != 0)
        options->number_of_cpus = theInfo->blast_config->num_cpu;
    else
        options->number_of_cpus = NUM_CPU_TO_USE;

    /* CPU time limit. */
    
    options->cpu_limit = DEFAULT_CPU_LIMIT;

    /* FILTER: */


    options->filter = FILTER_NONE;

    if(WWWGetMethod(theInfo->info) == WWW_GET ||
	    (chptr = WWWGetValueByName(theInfo->info, "FSET")) != NULL) {
	if (!StringICmp(theInfo->program, "blastn")) {
	    options->filter = FILTER_DUST;
	} else {
	    options->filter = FILTER_SEG;
	}
    }
    
    {
        Char	tmpbuf[4096];
        /* Filter settings */
	int	i, num_entries = WWWGetNumEntries(theInfo->info);
        
	StringCpy(tmpbuf, "");

	for(i = 0; i < num_entries; i++) {
	    if((chptr = WWWGetNameByIndex(theInfo->info, i)) != NULL && 
		    !StringICmp(chptr, "FILTER")) {

		chptr = WWWGetValueByIndex(theInfo->info, i);
		/* add the filter */
		StringCat(tmpbuf, chptr);
		StringCat(tmpbuf, ";");
	    }
	}
	options->filter_string = StringSave(tmpbuf);
    }

    /* DESCRIPTIONS: */

    theInfo->number_of_descriptions = DEFAULT_DESCRIPTIONS;

    if((chptr = WWWGetValueByName(theInfo->info, 
                                  "DESCRIPTIONS")) != NULL && 
       StringStr(chptr, "default") == NULL) {
	theInfo->number_of_descriptions = atoi(chptr);
    }
    
    /* ALIGNMENTS */
    theInfo->number_of_alignments = DEFAULT_ALIGNMENTS;

    if((chptr = WWWGetValueByName(theInfo->info, "ALIGNMENTS")) != NULL &&
       StringStr(chptr, "default") == NULL) {
	theInfo->number_of_alignments = atoi(chptr);  
    }
    
    /* Now processing OTHER_ADVANCED_OPTIONS */
    
    if((chptr = WWWGetValueByName(theInfo->info, 
                                  "OTHER_ADVANCED")) != NULL) {
        CharPtr ErrorMessage = NULL;
        int other_adv_descriptions = -1, other_adv_alignments = -1;

        parse_blast_options(options, chptr, &ErrorMessage, NULL,
                            &other_adv_descriptions, &other_adv_alignments);

        if (ErrorMessage) {
            WWWBlastErrMessage(BLASTOptionStr, ErrorMessage);
	    return FALSE;
	}
        if (other_adv_alignments != -1) { /* if defined */
           theInfo->number_of_alignments = other_adv_alignments;
        }

        if (other_adv_descriptions != -1) { /* if defined */
           theInfo->number_of_descriptions = other_adv_descriptions;
        }
    }

    /* HITLIST_SIZE: */
    
    options->hitlist_size = MAX(theInfo->number_of_descriptions, 
                                theInfo->number_of_alignments);
    
    options->perform_culling = FALSE;

    /* Parameters for Smith-Waterman BLAST */

    /* TWEAK_PARAMETERS */
    
    if (WWWGetValueByName(theInfo->info, "TWEAK_PARAMETERS") != NULL || 
        WWWGetValueByName(theInfo->info, "COMPOSITION_BASED_STATISTICS") 
        != NULL)
        options->tweak_parameters = TRUE;

    /* Adjustment of expect value and hitlist size */
    if (options->tweak_parameters) {
        options->hitlist_size *= 2; /*allows for extra matches*/
        options->original_expect_value = 
            options->expect_value;
        if (options->expect_value < 0.1) {
            options->expect_value = 
                MIN(0.1, 10 * options->expect_value);
        }
    }
    
    /* SMITH_WATERMAN */
    
    if (WWWGetValueByName(theInfo->info, "SMITH_WATERMAN") != NULL)
        options->smith_waterman = TRUE;

    /* Some values for PSI-Blast */
    value = 0;
    if((chptr = WWWGetValueByName(theInfo->info, "STEP_NUMBER")) != NULL)
	value = atoi(chptr);
    
    sprintf(tmp, "PSI_BEGIN%d", value-1);
    
    if((chptr = WWWGetValueByName(theInfo->info, tmp)) != NULL)
	options->required_start = atoi(chptr) - 1;
    
    sprintf(tmp, "PSI_END%d", value-1);
    if((chptr = WWWGetValueByName(theInfo->info, tmp)) != NULL)
	options->required_end = atoi(chptr) - 1;    
    
    if((chptr = WWWGetValueByName(theInfo->info, "E_THRESH")) != NULL)
	options->ethresh = atof(chptr);
    
    if((chptr = WWWGetValueByName(theInfo->info, "PHI_BLAST")) != NULL) {
	theInfo->is_phi_blast = TRUE;
        options->number_of_cpus = 1;
    }
    
    /* ------------------------ */

    /* ALIGNMENT VIEWS */
    
    if((chptr = WWWGetValueByName(theInfo->info, 
                                  "ALIGNMENT_VIEW")) != NULL &&
       StringStr(chptr, "default") == NULL) {
	theInfo->align_view = (BLASTAlignView) atoi(chptr);
        
        if(theInfo->align_view == BlastXML) {
            theInfo->xml_output = TRUE;
        }
    }

    if (WWWGetValueByName(theInfo->info, "XML_OUTPUT") != NULL)
	theInfo->xml_output = TRUE;

    if(options->is_ooframe) {
        theInfo->xml_output = FALSE;
        theInfo->align_view = Pairwise;
    }

    if (WWWGetValueByName(theInfo->info, "NCBI_GI") != NULL)
	theInfo->show_gi = TRUE;
    
    if (WWWGetValueByName(theInfo->info, "TAX_BLAST") != NULL)
        theInfo->show_tax_blast = TRUE;
    
    /* COLOR_SCHEMA */
    if((chptr = WWWGetValueByName(theInfo->info, "COLOR_SCHEMA")) != NULL &&
       StringStr(chptr, "No color schema") == NULL) {
	theInfo->color_schema = atoi(chptr);  
    }
    /* Now seting print and align options for BLAST output printing */

    theInfo->print_options = 0;
    theInfo->align_options = 0;
    theInfo->align_options += TXALIGN_COMPRESS;
    theInfo->align_options += TXALIGN_END_NUM;

    if (StringICmp("blastx", theInfo->program) == 0) {
        theInfo->align_options += TXALIGN_BLASTX_SPECIAL;
    }

    if (theInfo->show_gi) {
        theInfo->align_options += TXALIGN_SHOW_GI;
        theInfo->print_options += TXALIGN_SHOW_GI;
    }

    theInfo->print_options += TXALIGN_SHOW_LINKOUT;

    if (!gapped_set)
        theInfo->print_options += TXALIGN_SHOW_NO_OF_SEGS;
    
    if (theInfo->align_view != Pairwise) {
        theInfo->align_options += TXALIGN_MASTER;
        if (theInfo->align_view == QueryAnchoredIdent || 
            theInfo->align_view == FlatQueryAnchoredIdent)
            theInfo->align_options += TXALIGN_MISMATCH;
        if (theInfo->align_view == FlatQueryAnchoredIdent || 
            theInfo->align_view == FlatQueryAnchoredNoIdent || 
            theInfo->align_view == FlatQueryAnchoredBluntEnd)
            theInfo->align_options += TXALIGN_FLAT_INS;
        if (theInfo->align_view == QueryAnchoredBluntEnd || 
            theInfo->align_view == FlatQueryAnchoredBluntEnd)
            theInfo->align_options += TXALIGN_BLUNT_END;
    } else {
        theInfo->align_options += TXALIGN_MATRIX_VAL;
        theInfo->align_options += TXALIGN_SHOW_QS;
    }
    
    /* Always HTML in WWW case */
#if 1    
    theInfo->align_options += TXALIGN_HTML;
    theInfo->print_options += TXALIGN_HTML;
#endif    
    return TRUE;
}

Boolean WWWValidateOptions(WWWBlastInfoPtr theInfo)
{
    ValNodePtr error_return=NULL;
    BlastErrorMsgPtr error_msg;
    CharPtr msg = NULL;

    if(theInfo == NULL || theInfo->options == NULL)
	return FALSE;
    
    if(BLASTOptionValidateEx(theInfo->options, theInfo->program, 
                             &error_return) != 0) {
	if (error_return) {
	    error_msg = (BlastErrorMsgPtr) error_return->data.ptrvalue;
            msg = error_msg->msg;
	}
        WWWBlastErrMessage(BLASTErrOptions, msg);

	return FALSE;
    }
    return TRUE;
}

/* Used for PSI/PHI Blast searches */

static Int1 S62ToInt(Uint1 ch)
{
    if(isupper(ch)) /* negative value */
	return(64 - ch);
    else if (isdigit(ch)) /* positive less than 10 */
	return(ch - 48); 
    else if (!isupper(ch)) /* positive more or eq  10 */
	return(ch - 87);
    return 0;
}

static Uint1 IntTo62S(Int1 value)
{
    if(value < 0)
	return(64-value);
    else if (value < 10)
	return(value + 48); 
    else if (value < 36)
	return(value + 87);
    return 0;
}

static Int4 BLASTCharTo4bits(Char ch)
{
    if(ch != '0')
        ch = ch;
    if ((ch >= 'A') && (ch <= 'F'))
        return (((ch - 'A') + 10));
    else if ((ch >= '0') && (ch <= '9'))
        return ((ch - '0'));
    else
        return (Int4)(-1);
}

static Char BLAST4bitsToChar(int value)
{
    if (value < 10)
        return (Char)(value + '0');
    else
        return (Char)(value - 10 + 'A');
}

static Nlm_FloatHi **BLASTDecodePosFreqs(CharPtr CHARPosFreqs,
                                         Int4 length, Int4 size)
{
    Nlm_FloatHi **posFreqs;
    register Int4 i, j, k = 0, l;
    Nlm_FloatLo fvalue;
    Uint4 ivalue = 0;
    
    if(CHARPosFreqs == NULL || CHARPosFreqs[0] == NULLB)
	return NULL;
    
    posFreqs = (Nlm_FloatHi **) MemNew(sizeof(Nlm_FloatHi *)*(length+1));
    
    for(i = 0; i <= length; i++)
	posFreqs[i] = (Nlm_FloatHi *) MemNew(sizeof(Nlm_FloatHi)*size);
    
    for(i = 0, k = 0; i <= length; i++) {
	for(j =0; j < size; j++) {
            for(l = 0; l < 8; l++, k++) {
                ivalue += (BLASTCharTo4bits(CHARPosFreqs[k]) << (l * 4));
                /* ivalue = ivalue << 4; */
            }
            
            MemCpy(&fvalue, &ivalue, 4);
            posFreqs[i][j] = (Nlm_FloatHi) fvalue; /* 4 bytes into 8 bytes */
            ivalue = 0;
	}
    }
    return posFreqs;
}

static Int4Ptr PNTR Decode62Matrix(CharPtr Matrix62, Int4 length, Int4 size)
{
    Int4Ptr PNTR posMatrix;
    register Int4 i, j, k = 0;

    if(Matrix62 == NULL || Matrix62[0] == NULLB)
	return NULL;

    posMatrix = (Int4Ptr PNTR) MemNew(sizeof(Int4Ptr)*(length+1));

    for(i = 0; i <= length; i++)
	posMatrix[i] = (Int4Ptr) MemNew(sizeof(Int4Ptr)*size);

    for(i = 0; i <= length; i++) {
	for(j =0; j < size; j++) {
	    if(Matrix62[k] == 'z')
		posMatrix[i][j] = BLAST_SCORE_MIN;
	    else if (Matrix62[k] == 'Z')
		posMatrix[i][j] = BLAST_SCORE_MAX; 
	    else
		posMatrix[i][j]= S62ToInt(Matrix62[k]);
	    k++;
	}
    }
    return posMatrix;
}
static CharPtr BLASTEncodePosFreqs(Nlm_FloatHi **posFreqs, 
                                   Int4 length, Int4 size)
{    
    Int4 i, j, k=0, ivalue, fmask, l;
    CharPtr CHARPosFreqs;
    Nlm_FloatLo fvalue;
    
    /* So... size of the buffer will be eq. to number of elements
       in the posFreqs matrix times 8:  2 characters for every byte */
    CHARPosFreqs = (CharPtr) MemNew((length+1)*(size+1)*sizeof(Nlm_FloatLo)*2);
    
    for(i = 0, k = 0; i <= length; i++) {
	for(j =0; j < size; j++) {
	    fvalue = (Nlm_FloatLo) posFreqs[i][j]; /* truncation to 4 bytes */
            fmask = 0xF;       /* 4 bits */
            if(fvalue != 0.0)
                j=j;
            
            MemCpy(&ivalue, &fvalue, 4);
            for(l = 0; l < 8; l++, k++) {
                CHARPosFreqs[k] = BLAST4bitsToChar((ivalue & fmask) >> l * 4);
                fmask = fmask << 4;
            }
	}
    }

    return CHARPosFreqs;
}
static CharPtr Encode62Matrix(Int4Ptr PNTR posMatrix, Int4 length, Int4 size)
{
    register Int4 i, j, k=0;
    CharPtr Matrix62;

    Matrix62 = (CharPtr) MemNew((length+1)*size+1);

    for(i = 0; i <= length; i++) {
	for(j =0; j < size; j++) {

	    if(posMatrix[i][j] < -26)
		Matrix62[k] = 'z';
	    else if (posMatrix[i][j] > 35)
		Matrix62[k] = 'Z';
	    else
		Matrix62[k] = IntTo62S(posMatrix[i][j]);

	    k++;
	}
    }
    return Matrix62;
}

void BLASTPrintDataFree(BLASTPrintDataPtr data)
{
    GIListPtr glp, glp_next;
    ValNodePtr vnp;

    if(data == NULL)
	return;

    TxDfDbInfoDestruct(data->dbinfo);
    MemFree(data->ka_params);
    MemFree(data->ka_params_gap);
    MemFree(data->buffer);
    ValNodeFreeData(data->info_vnp);
    
    if(data->psidata != NULL) {
        MemFree(data->psidata->matrix62);
        MemFree(data->psidata->CHARPosFreqs);

        for(glp = data->psidata->PrevCheckedGIs; glp != NULL; glp = glp_next) {
            glp_next = glp->next;
            MemFree(glp);
        }

        for(glp = data->psidata->PrevGoodGIs; glp != NULL; glp = glp_next) {
            glp_next = glp->next;
            MemFree(glp);
        }
        MemFree(data->psidata);
    }
    
    if(data->seqalign != NULL)
        SeqAlignSetFree(data->seqalign);
    
    SeqLocFree(data->seqloc);
    
    for(vnp = data->vnp; vnp != NULL; vnp=vnp->next) {
        SeqAlignSetFree((SeqAlignPtr) vnp->data.ptrvalue);
    }
    
    ValNodeFree(data->vnp);
    
    MemFree(data);
    return;
}

/* We got seqalign list, now divide it into two lists:
   the first one will contain alignments with Evalue
   better than threshold, other worse than threshold
*/

Boolean 
SplitSeqAlign(SeqAlignPtr seqalign, SeqAlignPtr *GoodSeqAlignment_ptr, 
              SeqAlignPtr *BadSeqAlignment_ptr, SeqAlignPtr *lastGood_ptr, 
              Int2Ptr *marks_ptr, Int2Ptr countBad_ptr, 
              Int2Ptr countGood_ptr, Nlm_FloatHi ethresh_old)
{
    
    Boolean first_time;
    SeqIdPtr last_id, subject_id;
    SeqAlignPtr         gsl, seqalign_var, last_seqalign;
    SeqAlignPtr		BadSeqAlignments, GoodSeqAlignments, lastGood = NULL;
    Nlm_FloatHi         bit_score, evalue;
    Int2Ptr		marks;
    Int2		countGood, countBad;
    Int4                number, score;

    last_id = NULL;
    first_time = TRUE;
    GoodSeqAlignments = seqalign_var = seqalign;
    
    BadSeqAlignments = NULL;
    while (seqalign_var) {
	subject_id = SeqIdDup(TxGetSubjectIdFromSeqAlign(seqalign_var));
	if (last_id == NULL || SeqIdComp(subject_id, last_id) == SIC_NO) {
            SeqIdSetFree(last_id);
            last_id = subject_id;
            GetScoreAndEvalue(seqalign_var, &score, &bit_score, &evalue, &number);
            if (evalue > ethresh_old) {
                if (first_time == TRUE) {
                    GoodSeqAlignments = NULL;
                    lastGood = NULL;
                    last_seqalign = NULL;	/* split the good and bad lists. */
                } else {
                    lastGood = last_seqalign;
                    last_seqalign->next = NULL;	/* split the good and bad lists. */
                }
                BadSeqAlignments = seqalign_var;
                break;	
            }
	} else {
            SeqIdSetFree(subject_id);
        }

	first_time = FALSE;
	last_seqalign = seqalign_var;
	seqalign_var = seqalign_var->next;
    }
    
    /* count number of good and bad alignments */
    for (gsl = GoodSeqAlignments, countGood = 0; gsl; gsl = gsl->next, 
             countGood++);
    for (gsl = BadSeqAlignments, countBad = 0; gsl; gsl = gsl->next, 
             countBad++);
    
    if (countGood + countBad)
	/* allocate memo for marks array */
	marks = (Int2Ptr) MemNew(sizeof(Int2) * (countGood + countBad));
    else
	marks = NULL;
    
    *GoodSeqAlignment_ptr = GoodSeqAlignments;
    *BadSeqAlignment_ptr = BadSeqAlignments;
    *lastGood_ptr = lastGood;
    *marks_ptr = marks;
    *countBad_ptr = countBad;
    *countGood_ptr = countGood;
    
    return TRUE;
}

Boolean TestSTDOut(void)
{
    if(write(1, "", 0) < 0) {
	return FALSE;
    }
    return TRUE;
}

static int LIBCALLBACK WWWTickCallback(Int4 sequence_number, 
                                       Int4 number_of_positive_hits)
{
    if(!TestSTDOut()) {
	return -1;
    }
    
    /*    fprintf(stdout, "."); */

    printf("<!-- Progress msg from the server %d %d-->\n", 
           sequence_number, number_of_positive_hits);

    fflush(stdout);
    
    return 1;
}

static void PrintRequestHeader(WWWBlastInfoPtr theInfo)
{
    printf("<HTML>\n");
    printf("<HEAD>\n");
    printf("<TITLE>BLAST Search Results </TITLE>\n");
    printf("</HEAD>\n");
    printf("<BODY BGCOLOR=\"#FFFFFF\" LINK=\"#0000FF\" "
           "VLINK=\"#660099\" ALINK=\"#660099\">\n");
    fprintf(stdout, "<map name=img_map0>\n");
    fprintf(stdout, "<area shape=rect coords=2,1,48,21 "
            "href=\"http://www.ncbi.nlm.nih.gov\">\n");
    fprintf(stdout, "<area shape=rect coords=385,1,435,21 "
            "href=\"%s/blast/index.html\">\n", theInfo->www_root_path);
    fprintf(stdout, "<area shape=rect coords=436,1,486,21 "
            "href=\"http://www.ncbi.nlm.nih.gov/Entrez/\">\n");
    fprintf(stdout, "<area shape=rect coords=487,1,508,21 "
            "href=\"%s/blast/docs/blast_help.html\">\n", theInfo->www_root_path);
    fprintf(stdout, "</map>\n"); 
    fprintf(stdout, "<IMG USEMAP=#img_map0 WIDTH=509 HEIGHT=22 "
            "SRC=\"%s/blast/images/psi_blast.gif\" ISMAP> \n", 
            theInfo->www_root_path);
    printf("<BR><BR><PRE>\n");
   
    init_buff_ex(90);

    BlastPrintVersionInfo(theInfo->program, TRUE, stdout);
    fprintf(stdout, "\n");
   
    if (WWWGetValueByName(theInfo->info, "NO_REFERENCE") == NULL) { 
       if (theInfo->is_phi_blast) {
          BlastPrintPhiReference(TRUE, 90, stdout);
       } else {
          BlastPrintReference(TRUE, 90, stdout);
       }
    }
 
    fprintf(stdout, "\n");

    AcknowledgeBlastQuery(theInfo->query_bsp, 70, 
                          stdout, theInfo->believe_query, TRUE);
    
    /*
       dbinfo = GetDbInfoFromReadDb(search_data->database, 
       !search_data->db_is_na);
       PrintDbInformation(dbinfo, 70, search_data->outfp, 
       search_data->html);
     */

    PrintDbInformation(theInfo->database, !theInfo->db_is_na, 70, 
                       stdout, TRUE);
    free_buff();

    fprintf(stdout, "Searching please wait...");
    
    fflush(stdout);
    
    return;
}

static ValNodePtr seed_core_private (BlastSearchBlkPtr search, CharPtr program_name, BLAST_OptionsBlkPtr options, SeqLocPtr *seqloc_ptr, CharPtr patfile, CharPtr pattern, Uint1Ptr query, Uint1Ptr unfilter_query, Int4 queryLength, Boolean show_diagnostics, Nlm_FloatHi *paramC, ValNodePtr PNTR info_vnp)
     
{
    Boolean tmp_file_made = FALSE;
    Char buffer[PATH_MAX];
    posSearchItems *posSearch;
    SeqLocPtr seqloc;

    FILE *patfp; 
    seedSearchItems *seedSearch;
    Int4 program_flag;
    Boolean is_dna = FALSE; /*cannot use DNA queries in blastpgp*/
    Int4 i; /*index over characters*/
    ValNodePtr vnp;
    
    
    if (search == NULL)
        return NULL;
    
    program_flag = convertProgramToFlag(program_name, &is_dna);
    
    if (options->isPatternSearch) {
        if (pattern) {
            /* open and fill a temporary file if there's a pattern. XXX */
            TmpNam(buffer);
            patfp = FileOpen(buffer, "w");
            fprintf(patfp, "ID \n");
            fprintf(patfp, "PA   %s\n", pattern);
            fflush(patfp);
            FileClose(patfp);
            patfp = NULL;
            tmp_file_made = TRUE;
        }

        if (patfile)	/* If a file was give, use it. */
            StringCpy(buffer, patfile);
        if ((patfp = FileOpen(buffer, "r")) == NULL) {
            ErrPostEx(SEV_FATAL, 0, 0, "blast: Unable to open pattern file %s\n", buffer);
            return NULL;
        }
        seedSearch = (seedSearchItems *) MemNew(sizeof(seedSearchItems));
        fillCandLambda(seedSearch, options->matrix, options);
    } else {
        ErrPostEx(SEV_FATAL, 0, 0, "Must be a pattern search");
        return NULL;
    }
    
    if (paramC)
        *paramC = seedSearch->paramC;
    
    search->gap_align = GapAlignBlkNew(1,1);
    search->gap_align->gap_open = options->gap_open;
    search->gap_align->gap_extend = options->gap_extend;
    search->gap_align->decline_align = (-(BLAST_SCORE_MIN));
    search->gap_align->x_parameter = options->gap_x_dropoff
        *NCBIMATH_LN2/seedSearch->paramLambda;
    search->gap_align->matrix = search->sbp->matrix;
    initProbs(seedSearch);
    init_order(search->gap_align->matrix,program_flag,is_dna,seedSearch);

    for(i = 0; i < queryLength; i++)
        query[i] = seedSearch->order[query[i]];
    if (unfilter_query) {
        for(i = 0; i < queryLength; i++)
            unfilter_query[i] = seedSearch->order[unfilter_query[i]];
    }
    
    seqloc = NULL;
    posSearch = (posSearchItems *) MemNew(sizeof(posSearchItems));
    
    vnp = seedEngineCore(search, options, query, unfilter_query, readdb_get_filename(search->rdfp), patfile, program_flag, patfp, is_dna, FALSE, seedSearch, options->ethresh, 0.0, posSearch, &seqloc, show_diagnostics, info_vnp);
    
    if (tmp_file_made)	/* Remove temporary pattern file if it exists. */
        FileRemove(buffer);
    
    MemFree(seedSearch);

    MemFree(posSearch->posResultSequences);
    MemFree(posSearch);

    *seqloc_ptr = seqloc;
    
    return vnp;
}

static  BLAST_ScorePtr PNTR GetPSIMatrix(PSIDataPtr psidata, WWWBlastInfoPtr theInfo, Nlm_FloatHi *karlinK_out, Nlm_FloatHi ***posFreqs_out) {
    
    CharPtr     chptr, Matrix62_last=NULL, pattern;
    Int4        i;
    Nlm_FloatHi karlinK;
    Int2        num_entries;
    Int4 queryLength; /*length of query sequence*/
    Int4 numSeqAligns;
    SeqAlignPtr	seqalign;
    SeqAlignPtr *lastSeqAligns=NULL;
    SeqLocPtr seg_slp; /*pointer to structure for seg filtering*/
    Uint1Ptr query = NULL; /*query sequence read in*/
    Uint1Ptr unfilter_query = NULL; /*needed if seg will filter query*/
    ValNodePtr info_vnp, phi_vnp = NULL;
    SeqLocPtr phi_seqloc = NULL;
    Uint4      gi;
    SeqIdPtr   sip, all_sip = NULL;

    BlastSearchBlkPtr search;
    BLAST_ScorePtr PNTR posMatrix = NULL;
    compactSearchItems *compactSearch = NULL;
    CharPtr CHARPosFreqs_last;
    Nlm_FloatHi **posFreqs;

    /* first step; return NULL, means to use default matrix */
    if (psidata->StepNumber == 0) 
	return NULL;
        
    /* The second step;  means, that there is list of GI's to 
       recalculate matrix 
       using default or read matrix and list of seqaligns 
       from limited search;  limited search -
       search the query not in whole database but just
       in subset specified byt the list of GI's
     */

    /* Create list of SeqIdPtr for the limited search */

    num_entries = WWWGetNumEntries(theInfo->info);

    for(i = 0; i < num_entries; i++) {
	GIListPtr	good_gil, checked_gil;
        
	if((chptr = WWWGetNameByIndex(theInfo->info, i)) != NULL && 
           !StringICmp(chptr, "checked_GI")) {
            
	    if((chptr = WWWGetValueByIndex(theInfo->info, i)) != NULL) {
                gi = atoi(chptr);                
		sip = ValNodeAddInt(NULL, SEQID_GI, gi);
                ValNodeLink(&all_sip, sip);

		/* Create list of GI's, it will be used when we
		   test convergence */
                
		if (!psidata->PrevCheckedGIs) {
		    /* first one */
		    checked_gil = (GIListPtr) MemNew(sizeof(GIList));
		    checked_gil->gi = gi;
		    checked_gil->next = NULL;
		    psidata->PrevCheckedGIs = checked_gil;
		} else {
		    checked_gil->next = (GIListPtr) MemNew(sizeof(GIList));
		    checked_gil = checked_gil->next;
		    checked_gil->gi = gi;
		    checked_gil->next = NULL;
		}
	    }
	} else if((chptr = WWWGetNameByIndex(theInfo->info, i)) != NULL && 
                  !StringICmp(chptr, "good_GI")) {
            
	    if((chptr = WWWGetValueByIndex(theInfo->info, i)) != NULL) {
                gi = atoi(chptr);
                
		if (!psidata->PrevGoodGIs) {
		    /* first one */
		    good_gil = (GIListPtr) MemNew(sizeof(GIList));
		    good_gil->gi = gi;
		    good_gil->next = NULL;
		    psidata->PrevGoodGIs = good_gil;
		} else {
		    good_gil->next = (GIListPtr) MemNew(sizeof(GIList));
		    good_gil = good_gil->next;
		    good_gil->gi = gi;
		    good_gil->next = NULL;
		}
	    }
	}
    }

    /* So this search will be limited to the list of gis */
    theInfo->options->gilist = all_sip;
    
    /* Some additional parameters required for the matrix recalculation */
    
    theInfo->options->use_best_align = TRUE;
    theInfo->options->use_real_db_size = TRUE;
    
    /* the search */

    if((search = BLASTSetUpSearchWithReadDb(theInfo->fake_bsp, theInfo->program, theInfo->query_bsp->length, theInfo->database, theInfo->options, WWWTickCallback ))  == NULL) {
	return NULL;
    } 
    
    search->positionBased = FALSE;
    
    if (psidata->StepNumber > 1) {
	/* The third and rest of steps;  means, that
	   before we are recalculating matrix, we 
	   should read old matrix from the previous step;
        */
        
	/* Read matrix from the previous step */
        
        Matrix62_last = WWWGetValueByName(theInfo->info, "PSI_MATRIX");
        
	/* Decode read matrix */
        
	if(Matrix62_last != NULL && Matrix62_last[0] != NULLB) {
	    search->positionBased = TRUE;
	    search->sbp->posMatrix = Decode62Matrix (Matrix62_last, search->context[0].query->length, search->sbp->alphabet_size);
	}
        
        CHARPosFreqs_last = WWWGetValueByName(theInfo->info, "POS_FREQS");
        
        if(CHARPosFreqs_last != NULL && CHARPosFreqs_last[0] != NULLB) {
	    search->positionBased = TRUE;
            search->sbp->posFreqs = BLASTDecodePosFreqs(CHARPosFreqs_last, search->context[0].query->length, search->sbp->alphabet_size);

#if 0
        {{
            FILE *fd;
            fd = FileOpen("/tmp/new_freqs.float", "w");
            for(i = 0; i <= search->context[0].query->length; i++) {
                for(j =0; j < search->sbp->alphabet_size; j++) {        
                    fprintf(fd, "%f ", search->sbp->posFreqs[i][j]);
                }
                fprintf(fd, "\n");
            }
        }}
#endif


        }
        
        if((chptr = WWWGetValueByName(theInfo->info, 
                                      "PSI_KARLIN_K")) != NULL) {
            karlinK = atof(chptr);
            search->sbp->kbp_gap_psi[0]->K = karlinK;
            search->sbp->kbp_gap_psi[0]->logK = log(karlinK);
        }
	
    } /* end reread the matrix */    

    search->thr_info->tick_callback = NULL;
    
    pattern = WWWGetValueByName(theInfo->info, "PHI_PATTERN");
    
    /* If pattern is non-NULL, then it is a PHI-BLAST search. */
    if (pattern) {
        query = BlastGetSequenceFromBioseq(theInfo->fake_bsp, &queryLength);
        seg_slp = BlastBioseqFilter(theInfo->fake_bsp, theInfo->options->filter_string);
        unfilter_query = NULL;
        if (seg_slp) {
            unfilter_query = MemNew((queryLength + 1) * sizeof(Uint1));
            for (i = 0; i < queryLength; i++)
                unfilter_query[i] = query[i];
            BlastMaskTheResidues(query,queryLength,21,seg_slp,FALSE, 0);
        }
        
	theInfo->options->isPatternSearch = TRUE;
	phi_vnp = seed_core_private(search, "patseedp", theInfo->options, &phi_seqloc, NULL, pattern, query, unfilter_query, queryLength, FALSE, NULL, &info_vnp);
        ValNodeFreeData(info_vnp);

        MemFree(query);
        MemFree(unfilter_query);
        
	seqalign = convertValNodeListToSeqAlignList(phi_vnp, &lastSeqAligns, &numSeqAligns);
        ValNodeFree(phi_vnp);
    } else {
    	seqalign = BioseqBlastEngineCore(search, theInfo->options, 
                                         search->sbp->posMatrix);
    }
    
    if(search->sbp->posMatrix != NULL) {
        for(i = 0; i <= theInfo->fake_bsp->length; i++) {
            MemFree(search->sbp->posMatrix[i]);
        }    
        MemFree(search->sbp->posMatrix);
        search->sbp->posMatrix = NULL;
    }

    /* Now finaly calculating matrix that will be used at this step */
    
    if(seqalign) {

        ReadDBBioseqFetchEnable("psiblast", theInfo->database, 
                                FALSE, TRUE);
        compactSearch = compactSearchNew(compactSearch);
        copySearchItems(compactSearch, search, theInfo->options->matrix);

        compactSearch->pseudoCountConst = 7;

        if (search->sbp->posFreqs == NULL) {
            search->sbp->posFreqs =  allocatePosFreqs(compactSearch->qlength, 
                                                      compactSearch->alphabetSize);
        }
        
        posMatrix = WposComputation(compactSearch, seqalign, 
                                    search->sbp->posFreqs);

        /* We have to return posFreqs to the upper layer */
        posFreqs =  allocatePosFreqs(compactSearch->qlength, 
                                     compactSearch->alphabetSize);
        copyPosFreqs(search->sbp->posFreqs, posFreqs, compactSearch->qlength, 
                     compactSearch->alphabetSize);

        MemFree(compactSearch->standardProb);
        MemFree(compactSearch);

        ReadDBBioseqFetchDisable();
        
	/* Encode matrix for the use in the next step*/
        
	psidata->matrix62 = Encode62Matrix(posMatrix, 
                                           search->context[0].query->length, 
                                           search->sbp->alphabet_size);

        psidata->CHARPosFreqs = BLASTEncodePosFreqs(posFreqs, search->context[0].query->length, search->sbp->alphabet_size);

#if 0
        {{
            FILE *fd;
            fd = FileOpen("/tmp/old_freqs.float", "w");
            for(i = 0; i <= search->context[0].query->length; i++) {
                for(j =0; j < search->sbp->alphabet_size; j++) {        
                    fprintf(fd, "%f ", posFreqs[i][j]);
                }
                fprintf(fd, "\n");
            }
            FileClose(fd);
            fd = FileOpen("/tmp/old_freqs.buffer", "w");
            fprintf(fd, "%s", psidata->CHARPosFreqs);
            FileClose(fd);            
        }}
#endif
    }
    
    *karlinK_out = search->sbp->kbp_gap_psi[0]->K;
    *posFreqs_out = posFreqs;

    SeqAlignSetFree(seqalign);
    SeqLocFree(phi_seqloc);
    
    search = BlastSearchBlkDestruct(search);

    theInfo->options->use_best_align = FALSE;
    theInfo->options->use_real_db_size = FALSE;

    SeqIdSetFree(theInfo->options->gilist);
    theInfo->options->gilist = NULL;
    theInfo->options->isPatternSearch = FALSE;
    
    return posMatrix;
} /* end of GetPSIMatrix() */

BLASTPrintDataPtr PSIBlastSearch(WWWBlastInfoPtr theInfo)
{
    BLASTPrintDataPtr print_data;
    ValNodePtr vnp, other_returns= NULL;
    Int4 i;
    PSIDataPtr psidata;
    CharPtr    chptr;
    BlastSearchBlkPtr search;
    BLAST_ScorePtr PNTR posMatrix = NULL;
    Nlm_FloatHi karlinK;
    Nlm_FloatHi **posFreqs;
    
    if(theInfo == NULL)
	return NULL;

    if (!theInfo->xml_output)
       PrintRequestHeader(theInfo); 

    print_data = (BLASTPrintDataPtr) MemNew(sizeof(BLASTPrintData));
    
    psidata = MemNew(sizeof(PSIData));
    psidata->PrevGoodGIs = NULL;
    psidata->PrevCheckedGIs = NULL;
    
    /* initialize the search */
    theInfo->options->pseudoCountConst = 7;

    if((search = BLASTSetUpSearchWithReadDb(theInfo->fake_bsp, theInfo->program, theInfo->query_bsp->length, theInfo->database, theInfo->options, WWWTickCallback ))  == NULL) {
	return NULL;
    }

    /* Matrix and StepNumber for PSI-Blast */
    
    if((chptr = WWWGetValueByName(theInfo->info, "STEP_NUMBER")) != NULL)
	psidata->StepNumber = atoi(chptr);
    
    if((posMatrix = GetPSIMatrix(psidata, theInfo, 
                                 &karlinK, &posFreqs)) != NULL) {
        search->positionBased = TRUE;
        search->sbp->kbp_gap_psi[0]->K = karlinK;
        search->sbp->kbp_gap_psi[0]->logK = log(karlinK);
        search->sbp->posFreqs = posFreqs;
    }
    
    search->sbp->posMatrix = posMatrix;
    
    search->thr_info->tick_callback = WWWTickCallback;

    ReadDBBioseqFetchEnable("psiblast", theInfo->database, 
                            FALSE, TRUE);    
    printf("</PRE>\n");
    print_data->seqalign = BioseqBlastEngineCore(search, theInfo->options, posMatrix);

    ReadDBBioseqFetchDisable();

#if 0
    if(seqalign) {

        ReadDBBioseqFetchEnable("psiblast", theInfo->database, 
                                FALSE, TRUE);
        compactSearch = compactSearchNew(compactSearch);
        copySearchItems(compactSearch, search, theInfo->options->matrix);

        compactSearch->pseudoCountConst = 7;

        if (search->sbp->posFreqs == NULL) {
            search->sbp->posFreqs =  allocatePosFreqs(compactSearch->qlength, 
                                                      compactSearch->alphabetSize);
        }
        
        posMatrix = WposComputation(compactSearch, seqalign, 
                                    search->sbp->posFreqs);

        MemFree(compactSearch->standardProb);
        MemFree(compactSearch);

        
	/* Encode matrix for the use in the next step*/
    }
#endif

    if(posMatrix != NULL) {
        for(i = 0; i <= theInfo->fake_bsp->length; i++) {
            MemFree(posMatrix[i]);
        }
        
        MemFree(posMatrix);
        search->sbp->posMatrix = NULL;
    }

    print_data->psidata = psidata;
    
    /*  Blast search */
    
    other_returns = BlastOtherReturnsPrepare(search);

    print_data->mask_loc = NULL;

    for (vnp=other_returns; vnp; vnp = vnp->next) {
	switch (vnp->choice) {
        case TXDBINFO:
            print_data->dbinfo = vnp->data.ptrvalue;
            break;
        case TXKABLK_NOGAP:
            print_data->ka_params = 
                (BLAST_KarlinBlkPtr) vnp->data.ptrvalue;
            break;
        case TXKABLK_GAP:
            print_data->ka_params_gap = 
                (BLAST_KarlinBlkPtr) vnp->data.ptrvalue;
            psidata->karlinK = print_data->ka_params_gap->K;
            break;
        case TXPARAMETERS:
            print_data->buffer = vnp->data.ptrvalue;
            break;
        case TXMATRIX:
            print_data->matrix = (BLAST_MatrixPtr) vnp->data.ptrvalue;
            /*BLAST_MatrixDestruct(matrix);*/
            vnp->data.ptrvalue = NULL;
            break;
        case SEQLOC_MASKING_NOTSET:
        case SEQLOC_MASKING_PLUS1:
        case SEQLOC_MASKING_PLUS2:
        case SEQLOC_MASKING_PLUS3:
        case SEQLOC_MASKING_MINUS1:
        case SEQLOC_MASKING_MINUS2:
        case SEQLOC_MASKING_MINUS3:
            ValNodeAddPointer(&(print_data->mask_loc), vnp->choice, vnp->data.ptrvalue);
            break;
        default:
            break;
	}
    }   

    ValNodeFree(other_returns);

    search = BlastSearchBlkDestruct(search);

    return print_data;
}

BLASTPrintDataPtr PHIBlastSearch(WWWBlastInfoPtr theInfo)
{
    BLASTPrintDataPtr print_data;
    ValNodePtr vnp, other_returns= NULL;
    PSIDataPtr psidata;
    Nlm_FloatHi paramC;
    Int4 i;
    Int4 queryLength; /*length of query sequence*/
    CharPtr chptr;
    Char	matrixname[64] = "BLOSUM62";
    Int4	opencost = 0, extendedcost = 0;
    SeqLocPtr seg_slp; /*pointer to structure for seg filtering*/
    Uint1Ptr query = NULL; /*query sequence read in*/
    Uint1Ptr unfilter_query = NULL; /*needed if seg will filter query*/
    ValNodePtr info_vnp;
    BlastSearchBlkPtr search;

    if(theInfo == NULL)
	return NULL;
    
    if (!theInfo->xml_output)
       PrintRequestHeader(theInfo); 

    print_data = (BLASTPrintDataPtr) MemNew(sizeof(BLASTPrintData));
    
    psidata = MemNew(sizeof(PSIData));
    psidata->PrevGoodGIs = NULL;
    psidata->PrevCheckedGIs = NULL;
    
    print_data->psidata = psidata;
    
    /* Get matrix name and gap costs */
    
    if((chptr = WWWGetValueByName(theInfo->info, "MAT_PARAM")) != NULL) {
	if (chptr[1] != '-' || chptr[2] != '-')
	    sscanf(chptr, "%s\t %d\t %d", matrixname, &opencost, &extendedcost);
    }

    /* Change matrix parameters */
    
    BLASTOptionSetGapParams (theInfo->options, matrixname, 
                             opencost, extendedcost);
    
    chptr = WWWGetValueByName(theInfo->info, "PHI_PATTERN");
    
    /* Reguilar PHI-Blast search */
    theInfo->options->isPatternSearch = TRUE;
    
    if((search = BLASTSetUpSearchWithReadDb(theInfo->fake_bsp, "blastp", theInfo->query_bsp->length, theInfo->database, theInfo->options, WWWTickCallback))  == NULL) {
	return NULL;
    }
    
    query = BlastGetSequenceFromBioseq(theInfo->fake_bsp, &queryLength);
    seg_slp = BlastBioseqFilter(theInfo->fake_bsp, 
                                theInfo->options->filter_string);
    
    unfilter_query = NULL;
    if (seg_slp) {
        unfilter_query = MemNew((queryLength + 1) * sizeof(Uint1));
        for (i = 0; i < queryLength; i++)
            unfilter_query[i] = query[i];
        BlastMaskTheResidues(query,queryLength,21,seg_slp,FALSE, 0);
    }
    
    print_data->vnp = seed_core_private(search, "patseedp", theInfo->options, &(print_data->seqloc), NULL, chptr, query, unfilter_query, queryLength, TRUE, &paramC, &info_vnp);
    
    print_data->info_vnp = info_vnp;
    
    MemFree(query);
    MemFree(unfilter_query);
    
    /*  Blast search */
    
    other_returns = BlastOtherReturnsPrepare(search);
    print_data->mask_loc = NULL;
    for (vnp=other_returns; vnp; vnp = vnp->next) {
	switch (vnp->choice) {
        case TXDBINFO:
            print_data->dbinfo = vnp->data.ptrvalue;
            break;
        case TXKABLK_GAP:
            print_data->ka_params_gap = vnp->data.ptrvalue;
            /* print_data->ka_params_gap->paramC = paramC; ?? */
            break;
        case TXKABLK_NOGAP:
            print_data->ka_params = vnp->data.ptrvalue;
            break;
        case TXPARAMETERS:
            print_data->buffer = vnp->data.ptrvalue;
            break;
        case TXMATRIX:
            print_data->matrix = (BLAST_MatrixPtr) vnp->data.ptrvalue;
            /*BLAST_MatrixDestruct(matrix);*/
            vnp->data.ptrvalue = NULL;
            break;
        case SEQLOC_MASKING_NOTSET:
        case SEQLOC_MASKING_PLUS1:
        case SEQLOC_MASKING_PLUS2:
        case SEQLOC_MASKING_PLUS3:
        case SEQLOC_MASKING_MINUS1:
        case SEQLOC_MASKING_MINUS2:
        case SEQLOC_MASKING_MINUS3:
            ValNodeAddPointer(&(print_data->mask_loc), vnp->choice, vnp->data.ptrvalue);
            break;
        default:
            break;
        }
    }
    
    search = BlastSearchBlkDestruct(search);
    
    fflush(stdout);
    return print_data;
}

static	void	printSubmitButton(FILE* fp, Int4 step)
{
    fprintf(fp, "<INPUT TYPE=\"submit\" NAME=\"NEXT_I\" "
            "VALUE=\"Run PSI-Blast iteration %d\">\n", 
            step);
}

static Int4 get_number_alignment(SeqAlignPtr align)
{
    Int4 num = 0;
    
    while(align) {
	++num;
	align = align->next;
    }
    
    return num;
}

Boolean PHIPrintOutput(WWWBlastInfoPtr theInfo,
	BLASTPrintDataPtr print_data, 
	ValNodePtr vnp, Nlm_FloatHi ethresh_old)
{
    Uint4 align_options, print_options;
    SeqAnnotPtr seqannot;
    BlastTimeKeeper time_keeper;
    Uint1 f_order[FEATDEF_ANY], g_order[FEATDEF_ANY];
    Char	href[1024];
    Char	f_name[64], title[1024];
    Int4	align_num;
    Int2	countBad, countGood;
    Int2Ptr 	marks;
    Int4 numSeqAligns;
    SeqAlignPtr	seqalign;
    SeqAlignPtr *lastSeqAligns=NULL;
    SeqAlignPtr	lastGood, BadSeqAlignments, GoodSeqAlignments;
    SeqLocPtr	seqloc;
    ValNodePtr	vnp_var;
    Int4Ptr PNTR txmatrix;

    MemSet((Pointer)(g_order), 0, (size_t)(FEATDEF_ANY* sizeof(Uint1)));
    MemSet((Pointer)(f_order), 0, (size_t)(FEATDEF_ANY* sizeof(Uint1)));
    
    if(print_data == NULL) {
        WWWBlastErrMessage(BLASTMiscError, NULL);                
	return FALSE;
    }

    print_options = 0;
    align_options = 0;

    align_options += TXALIGN_COMPRESS;
    align_options += TXALIGN_END_NUM;

    if (theInfo->show_gi) {
	align_options += TXALIGN_SHOW_GI;
	print_options += TXALIGN_SHOW_GI;
    }

    if (theInfo->options->gapped_calculation == FALSE)
	print_options += TXALIGN_SHOW_NO_OF_SEGS;


    if (theInfo->align_view != Pairwise) {
        align_options += TXALIGN_MASTER;
        if (theInfo->align_view == QueryAnchoredIdent || 
            theInfo->align_view == FlatQueryAnchoredIdent)
            align_options += TXALIGN_MISMATCH;
        if (theInfo->align_view == FlatQueryAnchoredIdent || 
            theInfo->align_view == FlatQueryAnchoredNoIdent || 
            theInfo->align_view == FlatQueryAnchoredBluntEnd)
            align_options += TXALIGN_FLAT_INS;
        if (theInfo->align_view == QueryAnchoredBluntEnd || 
            theInfo->align_view == FlatQueryAnchoredBluntEnd)
            align_options += TXALIGN_BLUNT_END;
    } else {
	align_options += TXALIGN_MATRIX_VAL;
	align_options += TXALIGN_SHOW_QS;
    }

    /* align_options += TXALIGN_MATRIX_VAL;
       align_options += TXALIGN_SHOW_QS; */

    align_options += TXALIGN_HTML;
    print_options += TXALIGN_HTML; 

    ReadDBBioseqFetchEnable ("phiblast", 
                             theInfo->database,  theInfo->db_is_na, TRUE);
    
    seqannot = SeqAnnotNew();
    seqannot->type = 2;
    AddAlignInfoToSeqAnnot(seqannot, theInfo->align_type);

    init_buff();

    /*    gethostname(hostname, sizeof(hostname)); */

    sprintf(href, "nph-viewgif.cgi?");
    
    seqalign = convertValNodeListToSeqAlignList(print_data->vnp, &lastSeqAligns, &numSeqAligns);
    seqannot->data = seqalign;
    if (theInfo->show_overview) {
        sprintf(f_name, "%ld%ld.gif", (long)random(), (long)getpid());
        align_num = get_number_alignment((SeqAlignPtr)(seqannot->data)); 
        sprintf(title, "<H3><a href=\"%s/blast/docs/newoptions.html#graphical-overview\"> "
                "Distribution of %ld Blast Hits on the Query Sequence</a></H3>\n", theInfo->www_root_path, (long)align_num);  
        
        PrintAlignmentOverview(seqannot, stdout, "PSI_BLAST", href, f_name, title); 
    }

    seqannot->data = NULL;
    seqannot = SeqAnnotFree(seqannot);
    
    print_data->vnp = convertSeqAlignListToValNodeList(seqalign, lastSeqAligns, numSeqAligns);
    
    print_options += TXALIGN_DO_NOT_PRINT_TITLE; 
    print_options += TXALIGN_CHECK_BOX;
    if (print_data->psidata->StepNumber)
        print_options += TXALIGN_NEW_GIF;
    
    /* submit button */
    printSubmitButton(stdout, 
                      print_data->psidata->StepNumber+1);
    
    if (print_data->psidata->StepNumber && theInfo->number_of_descriptions) {
        printf("<HR><p><b>Legend:</b><p>\
<IMG SRC=\"%s/blast/images/new.gif\" WIDTH=25 HEIGHT=15> - means that \
the alignment score was below the threshold on the previous iteration \
<p>\
<IMG SRC=\"%s/blast/images/checked.gif\" WIDTH=15 HEIGHT=15> - means that \
the alignment was checked on the previous iteration \
</p>", theInfo->www_root_path, theInfo->www_root_path);
    }
    
    /*
      if (theInfo->number_of_descriptions) {
      if (print_data->psidata->StepNumber)
      printf("\n<IMG SRC=\"/BLAST/bg.gif\" WIDTH=65 HEIGHT=15>");
      printf("                                                                     Score    E");
      printf("\nSequences producing significant alignments:");
      if (print_data->psidata->StepNumber)
      printf("<IMG SRC=\"/BLAST/bg.gif\" WIDTH=65 HEIGHT=15>");
      printf("                          (bits) Value\n\n");
      }
    */
    
    vnp_var = vnp;
    seqloc = print_data->seqloc;
    marks = NULL;
    while (vnp_var) {
        SplitSeqAlign((SeqAlignPtr) vnp_var->data.ptrvalue, &GoodSeqAlignments, &BadSeqAlignments, &lastGood, &marks, 
                      &countBad, &countGood, ethresh_old);
	
        printf("<HR><CENTER><b><FONT color=\"green\">"
               "Sequences with pattern at position %d and E-value BETTER than threshold</FONT></b></CENTER>\n",
               SeqLocStart(seqloc)+1);
        
        if (print_data->psidata->StepNumber)
            printf("\n<IMG SRC=\"%s/blast/images/bg.gif\" WIDTH=65 HEIGHT=15>", 
                   theInfo->www_root_path);
        printf("                                                                     Score    E");
        printf("\nSequences producing significant alignments:");
        if (print_data->psidata->StepNumber)
            printf("<IMG SRC=\"%s/blast/images/bg.gif\" WIDTH=65 HEIGHT=15>",
                   theInfo->www_root_path);
        printf("                          (bits) Value\n\n");
        
        fflush(stdout);
        print_options += TXALIGN_CHECK_BOX_CHECKED;
        PrintDefLinesFromSeqAlignWithPath(GoodSeqAlignments, 80, stdout,
           print_options, FIRST_PASS, marks,
           theInfo->number_of_descriptions, theInfo->database,
           theInfo->www_blast_type, theInfo->www_root_path);

        print_options -= TXALIGN_CHECK_BOX_CHECKED;
        
        if (print_data->psidata->StepNumber == 0)
            printf("<a name = Evalue> </a>");
	
        if (theInfo->number_of_descriptions > countGood && BadSeqAlignments) {
            /* submit button */
            printSubmitButton(stdout, print_data->psidata->StepNumber+1);
            
            printf("<HR><CENTER><b><FONT color=\"green\">"
                   "Sequences with pattern at position %d and E-value WORSE than threshold</FONT></b></CENTER>\n", 
                   SeqLocStart(seqloc)+1);
            
            PrintDefLinesFromSeqAlignWithPath(BadSeqAlignments, 80, stdout,
               print_options, FIRST_PASS, &marks[countGood], 
               theInfo->number_of_descriptions - countGood, 
               theInfo->database, theInfo->www_blast_type, 
               theInfo->www_root_path);
        }

        marks = MemFree(marks);
        
        /* merge lists */
        if (lastGood)
            lastGood->next = BadSeqAlignments;
        
        vnp_var = vnp_var->next;
        seqloc = seqloc->next;
    }
    
    if (theInfo->number_of_descriptions) {
        /* submit button */
        printSubmitButton(stdout, 
                          print_data->psidata->StepNumber+1);
    }
    
    free_buff();
    fflush(stdout);

    fprintf(stdout, "<HR>");

    txmatrix = NULL;
    if (print_data->matrix)
       txmatrix = (Int4Ptr PNTR) BlastMatrixToTxMatrix(print_data->matrix);

    if (theInfo->number_of_alignments) {
	fprintf(stdout, "<CENTER><b><FONT color=\"green\">"
		"Alignments</FONT></b></CENTER>\n");
        
        f_order[FEATDEF_REGION] = 1;
        g_order[FEATDEF_REGION] = 1;
        if(theInfo->align_view == Pairwise) {
            ShowTextAlignFromAnnotExtra(theInfo->fake_bsp, 
                                        print_data->vnp, 
                                        print_data->seqloc, 60, 
                                        stdout, 
                                        f_order, g_order, align_options, 
                                        txmatrix, print_data->mask_loc, 
                                        FormatScoreFunc);
        } else {
            ShowTextAlignFromAnnotExtra(theInfo->fake_bsp, 
                                        print_data->vnp, 
                                        print_data->seqloc, 60, 
                                        stdout, 
                                        f_order, g_order, align_options, 
                                        txmatrix, print_data->mask_loc, 
                                        NULL);
            printf("<P>\n");
        }
    }

    fflush(stdout);
    ObjMgrClearHold(); 

    printf("<PRE>\n");

    BlastTimeFillStructure(&time_keeper);

    fprintf(stdout, "CPU time: %8.2f user secs.\t%8.2f sys. "
	    "secs\t%8.2f total secs.\n\n", 
	    time_keeper.user, time_keeper.system, time_keeper.total);    
    
    if (txmatrix)
           txmatrix =  (Int4Ptr PNTR) TxMatrixDestruct(txmatrix);
    print_data->matrix = BLAST_MatrixDestruct(print_data->matrix);

    init_buff();
    PrintDbReport(print_data->dbinfo, 70, stdout);
    
    fflush(stdout);
    if (print_data->ka_params_gap) {
        PrintKAParameters(print_data->ka_params_gap->Lambda, 
                          print_data->ka_params_gap->K, 
                          print_data->ka_params_gap->H, 
                          70, stdout, TRUE);
    }
    fflush(stdout);
    
    PGPOutTextMessages(print_data->info_vnp, stdout);

    PrintTildeSepLines(print_data->buffer, 70, stdout);
    free_buff();

    fflush(stdout);

    ReadDBBioseqFetchDisable();

    return TRUE;
}

Boolean PSIPrintOutput(WWWBlastInfoPtr theInfo,
	BLASTPrintDataPtr print_data, 
	SeqAlignPtr BadSeqAlignments, SeqAlignPtr GoodSeqAlignments,
	SeqAlignPtr lastGood,
	Int2Ptr marks, Int2 countBad, Int2 countGood,
	Nlm_FloatHi ethresh_old)
{
    Uint4 align_options, print_options;
    SeqAnnotPtr seqannot;
    BlastTimeKeeper time_keeper;
    BlastPruneSapStructPtr prune;
    Uint1 f_order[FEATDEF_ANY], g_order[FEATDEF_ANY];
    Char	href[1024];
    Char	f_name[64], title[1024];
    Int4	align_num;
    Int4Ptr PNTR txmatrix;

    MemSet((Pointer)(g_order), 0, (size_t)(FEATDEF_ANY* sizeof(Uint1)));
    MemSet((Pointer)(f_order), 0, (size_t)(FEATDEF_ANY* sizeof(Uint1)));
    
    if(print_data == NULL) {
        WWWBlastErrMessage(BLASTMiscError, NULL);                
	return FALSE;
    }
    
    print_options = 0;
    align_options = 0;
    
    align_options += TXALIGN_COMPRESS;
    align_options += TXALIGN_END_NUM;
    
    if (theInfo->show_gi) {
	align_options += TXALIGN_SHOW_GI;
	print_options += TXALIGN_SHOW_GI;
    }
    
    if (theInfo->options->gapped_calculation == FALSE)
	print_options += TXALIGN_SHOW_NO_OF_SEGS;
    
    if (theInfo->align_view != Pairwise) {
        align_options += TXALIGN_MASTER;
        if (theInfo->align_view == QueryAnchoredIdent || 
            theInfo->align_view == FlatQueryAnchoredIdent)
            align_options += TXALIGN_MISMATCH;
        if (theInfo->align_view == FlatQueryAnchoredIdent || 
            theInfo->align_view == FlatQueryAnchoredNoIdent || 
            theInfo->align_view == FlatQueryAnchoredBluntEnd)
            align_options += TXALIGN_FLAT_INS;
        if (theInfo->align_view == QueryAnchoredBluntEnd || 
            theInfo->align_view == FlatQueryAnchoredBluntEnd)
            align_options += TXALIGN_BLUNT_END;
    } else {
	align_options += TXALIGN_MATRIX_VAL;
	align_options += TXALIGN_SHOW_QS;
    }
    
    /* align_options += TXALIGN_MATRIX_VAL;
       align_options += TXALIGN_SHOW_QS; */

    align_options += TXALIGN_HTML;
    print_options += TXALIGN_HTML; 

    ReadDBBioseqFetchEnable ("psiblast", 
	    theInfo->database, 
	    theInfo->db_is_na, 
	    TRUE);

    seqannot = SeqAnnotNew();
    seqannot->type = 2;
    AddAlignInfoToSeqAnnot(seqannot, theInfo->align_type);
    seqannot->data = print_data->seqalign;

    init_buff();

    /* merge lists */
    if (lastGood)
        lastGood->next = BadSeqAlignments;
    
    /* gethostname(hostname, sizeof(hostname)); */
    
    sprintf(href, "nph-viewgif.cgi?");
    
    if (theInfo->show_overview) {
        sprintf(f_name, "%ld%ld.gif", (long)random(), (long)getpid());
        align_num = get_number_alignment((SeqAlignPtr)(seqannot->data)); 
        sprintf(title, "<H3><a href=\"%s/blast/docs/newoptions.html#graphical-overview\"> "
                "Distribution of %ld Blast Hits on the Query Sequence</a></H3>\n", theInfo->www_root_path, (long)align_num);  
        
        PrintAlignmentOverview(seqannot, stdout, "PSI_BLAST", href, f_name, title); 
    }
    
    /* separate lists */
    if (lastGood)
        lastGood->next = NULL;
    
    print_options += TXALIGN_DO_NOT_PRINT_TITLE; 
    print_options += TXALIGN_CHECK_BOX;
    print_options += TXALIGN_CHECK_BOX_CHECKED;
    if (print_data->psidata->StepNumber)
        print_options += TXALIGN_NEW_GIF;
    
    /* submit button */
    printSubmitButton(stdout, 
                      print_data->psidata->StepNumber+1);
    
    if (print_data->psidata->StepNumber && theInfo->number_of_descriptions) {
        printf("<HR><p><b>Legend:</b><p>\
<IMG SRC=\"%s/blast/images/new.gif\" WIDTH=25 HEIGHT=15> - means that \
the alignment score was below the threshold on the previous iteration \
<p>\
<IMG SRC=\"%s/blast/images/checked.gif\" WIDTH=15 HEIGHT=15> - means that \
the alignment was checked on the previous iteration \
</p>", theInfo->www_root_path, theInfo->www_root_path);
    }
    
    if (theInfo->number_of_descriptions) {
        printf("<HR><CENTER><b><FONT color=\"green\">"
               "Sequences with E-value BETTER than threshold </FONT></b></CENTER>\n");
        if (print_data->psidata->StepNumber)
            printf("\n<IMG SRC=\"%s/blast/images/bg.gif\" WIDTH=65 HEIGHT=15>", 
                   theInfo->www_root_path);
        printf("                                                                     Score    E");
        printf("\nSequences producing significant alignments:");
        if (print_data->psidata->StepNumber)
            printf("<IMG SRC=\"%s/blast/images/bg.gif\" WIDTH=65 HEIGHT=15>", 
                   theInfo->www_root_path);
        printf("                          (bits) Value\n\n");
    }
    
    PrintDefLinesFromSeqAlignWithPath(GoodSeqAlignments, 80, stdout, 
       print_options, FIRST_PASS, marks, theInfo->number_of_descriptions,
       theInfo->database, theInfo->www_blast_type, theInfo->www_root_path);
    
    print_options -= TXALIGN_CHECK_BOX_CHECKED;
    
    if (print_data->psidata->StepNumber == 0)
        printf("<a name = Evalue> </a>");
    
    if (theInfo->number_of_descriptions > countGood && BadSeqAlignments) {

        printSubmitButton(stdout, 
                          print_data->psidata->StepNumber+1);
        
        printf("<HR><CENTER><b><FONT color=\"green\">"
               "Sequences with E-value WORSE than threshold </FONT></b></CENTER>\n");
    
        PrintDefLinesFromSeqAlignWithPath(BadSeqAlignments, 80, stdout, 
           print_options, FIRST_PASS, &marks[countGood], 
           theInfo->number_of_descriptions - countGood, theInfo->database,
           theInfo->www_blast_type, theInfo->www_root_path);
    }

    if (theInfo->number_of_descriptions) {
	printSubmitButton(stdout, 
                          print_data->psidata->StepNumber+1);
    }

    free_buff();
    fflush(stdout);

    /* merge lists */
    if (lastGood)
	lastGood->next = BadSeqAlignments;

    prune = BlastPruneHitsFromSeqAlign((SeqAlignPtr) seqannot->data, theInfo->number_of_alignments, NULL);
    seqannot->data = prune->sap;

    fprintf(stdout, "<HR>");

    txmatrix = NULL;
    if (print_data->matrix)
       txmatrix = (Int4Ptr PNTR) BlastMatrixToTxMatrix(print_data->matrix);

    if (theInfo->number_of_alignments) {
	fprintf(stdout, "<CENTER><b><FONT color=\"green\">"
		"Alignments</FONT></b></CENTER>\n");

        /* New DDV formating requested */
        if(theInfo->color_schema != 0) {
            if(!DDV_DisplayBlastPairList(prune->sap, print_data->mask_loc, 
                                         stdout, 
                                         theInfo->query_is_na, align_options, 
                                         theInfo->color_schema)) { 
                fprintf(stdout, 
                        "\n\n!!!\n   "
                        "    --------  Failure to print alignment...  --------"
                        "\n!!!\n\n");
                fflush(stdout);
            }
        } else {   /* Old type formating */
            if (theInfo->align_view == Pairwise) {
                ShowTextAlignFromAnnot2(seqannot, 60, stdout, f_order,
                                        g_order, align_options, txmatrix, 
                                        print_data->mask_loc, 
                                        FormatScoreFunc, theInfo->database, 
                                        "psiblast");
            } else { 
                ShowTextAlignFromAnnot2(seqannot, 60, stdout, f_order,
                   g_order, align_options, txmatrix, print_data->mask_loc, 
                   NULL, NULL, theInfo->www_blast_type);
                printf("<P>\n");
            }
        }
    }

    /* separate lists */
    if (lastGood)
	lastGood->next = NULL;

    fflush(stdout);
    ObjMgrClearHold(); 

    prune = BlastPruneSapStructDestruct(prune);

    seqannot->data = NULL; 
    seqannot = SeqAnnotFree(seqannot);

    printf("<PRE>\n");

    BlastTimeFillStructure(&time_keeper);

    fprintf(stdout, "CPU time: %8.2f user secs.\t%8.2f sys. "
	    "secs\t%8.2f total secs.\n\n", 
	    time_keeper.user, time_keeper.system, time_keeper.total);    

    print_data->matrix = BLAST_MatrixDestruct(print_data->matrix);
    if (txmatrix)
       txmatrix = (Int4Ptr PNTR) TxMatrixDestruct(txmatrix);

    init_buff();
    PrintDbReport(print_data->dbinfo, 70, stdout);
    
    if (print_data->ka_params) {
	PrintKAParameters(print_data->ka_params->Lambda, 
                          print_data->ka_params->K, 
                          print_data->ka_params->H, 70, 
                          stdout, FALSE);
    }
    
    if (print_data->ka_params_gap) {
	PrintKAParameters(print_data->ka_params_gap->Lambda, 
                          print_data->ka_params_gap->K, 
                          print_data->ka_params_gap->H, 
                          70, stdout, TRUE);
    }

    PrintTildeSepLines(print_data->buffer, 70, stdout);
    free_buff();
    
    fflush(stdout);

    ReadDBBioseqFetchDisable();

    return TRUE;
}
