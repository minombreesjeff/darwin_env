/* $Id: fmspell.c,v 1.1 1998/02/24 21:01:22 shavirin Exp $
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
* File Name:  $RCSfile: fmspell.c,v $
*
* Author:  Sergei Shavirin
*
* Initial Version Creation Date: 02/24/1998
*
* $Revision: 1.1 $
*
* File Description:
*        Database formatter for NCBI SPELL Service
*
* $Log: fmspell.c,v $
* Revision 1.1  1998/02/24 21:01:22  shavirin
* Initial revision
*
*
* ==========================================================================
*/

#include <ncbi.h>
#include <ncbisort.h>
#include <ncbisam.h>

#define NUMARG 3

Args fmspell_args[NUMARG] = {
    { "Input file name for spell database", 
      "termlist", NULL, NULL, FALSE, 'd', ARG_STRING, 0.0, 0, NULL},
    {"Input file anme for stop words",
     "stopwords", NULL,NULL,FALSE,'s',ARG_FILE_IN, 0.0,0,NULL},
    {"Logfile name:",
     "fmspell.log", NULL,NULL,TRUE,'l',ARG_FILE_OUT, 0.0,0,NULL}
};

#define DB_filename    (CharPtr) fmspell_args[0].strvalue 
#define STOP_filename  (CharPtr) fmspell_args[1].strvalue 
#define LogFileName    (CharPtr) fmspell_args[2].strvalue  

static Boolean FMSpellCreateIndex(CharPtr db_name)
{
    SORTObjectPtr sop;
    Char filenamebuf[FILENAME_MAX], DBName[FILENAME_MAX];
    Char ext1[8], ext2[8], ext3[8], ext4[8];
    FILE *fd_out;
    CharPtr files;
    ISAMErrorCode error;
    ISAMObjectPtr isamp;

    /*  object for unique sorting */
    
    if((sop = SORTObjectNew(NULL, '\0', 0,
                            FALSE, TRUE)) == NULL) { 
        ErrPostEx(SEV_ERROR, 0, 0, "Failed to create SORT Object");
        return FALSE;
    }
    
    sprintf(filenamebuf, "%s.tmp", db_name); 
    
    if((fd_out = FileOpen(filenamebuf, "w")) == NULL)
        return FALSE;
    
    files = db_name;
    
    SORTFiles(&files, 1, fd_out, sop);
    SORTObjectFree(sop);
    
    FileClose(fd_out);
    FileRename(filenamebuf, db_name);
    
    sprintf(filenamebuf, "%s.idx", db_name); 
    
    if((isamp = ISAMObjectNew(ISAMString, db_name, 
                              filenamebuf)) == NULL) {
        ErrPostEx(SEV_ERROR, 0, 0, "Creating of ISAM object failed");
        return FALSE;
    }
    
    if((error = ISAMMakeIndex(isamp, 0)) != ISAMNoError) {
        ErrPostEx(SEV_ERROR, 0, 0, "Creating of index failed with "
                  "error code %ld\n", (long) error);
        ISAMObjectFree(isamp);
        return FALSE;
    } 
    
    ISAMObjectFree(isamp);
    
    return TRUE;
}

Int2 Main(void)
{

    if ( !GetArgs ("fmspell", NUMARG, fmspell_args) ) {
        return -1;
    }
    if ( !ErrSetLog (LogFileName) ) {
        ErrShow();
    } else {
        ErrSetOpts (ERR_CONTINUE, ERR_LOG_ON);
    }
    
    if(!FMSpellCreateIndex(DB_filename))
        return 1;
    
    if(!FMSpellCreateIndex(STOP_filename))
        return 1;
    
    ErrPostEx(SEV_INFO, 0, 0, "Formating finished successfuly");

    return 0;
}
