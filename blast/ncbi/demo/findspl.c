/*   findspl.c
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
* File Name:  findspl.c
*
* Author:  Epstein, Weisemann
*
* Version Creation Date:   1/4/94
*
* $Revision: 6.2 $
*
* File Description: 
*       Find protein splice sites
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
* $Log: findspl.c,v $
* Revision 6.2  1998/08/26 18:41:09  kans
* fixed remaining -v -fd warning
*
* Revision 6.1  1998/08/24 20:43:43  kans
* fixed -v -fd warnings
*
* Revision 6.0  1997/08/25 18:20:00  madden
* Revision changed to 6.0
*
* Revision 1.2  1996/06/24 18:37:50  epstein
* move splicing functionality to sequtil.c
*
 * Revision 1.1  1996/06/19  15:23:21  epstein
 * Initial revision
 *
*
* ==========================================================================
*/

#include <accentr.h>
#include <sequtil.h>

#define NUMARGS 3
Args myargs[NUMARGS] = {
        {"Gi number of protein","0","1","99999999",FALSE,'g',ARG_INT,0.0,0,NULL},
	{"Input file containing protein GIs","stdin","NULL","NULL",FALSE,'i',ARG_FILE_IN,0.0,0,NULL},
	{"Output file","stdout","NULL","NULL",FALSE,'o',ARG_FILE_OUT,0.0,0,NULL}};


static void FindAndPrint(FILE *ofp, Int4 gi)
{
	SeqLocPtr slp, current;
	SeqEntryPtr sep;

        if ((sep = EntrezSeqEntryGet(gi, 0)) != NULL)
        {
            fprintf (ofp, "%ld", (long) gi);
	    slp = FindSpliceSites(sep, /* find on protein */ TRUE);
            for (current = SeqLocFindNext(slp, NULL); current != NULL;
                 current = SeqLocFindNext(slp, current))
	    {
		fprintf (ofp, " %ld", (long) SeqLocStart(current) + 1);
            }

            fprintf (ofp, "\n");
            SeqEntryFree(sep);
            SeqLocFree(slp);
        }
}

Int2 Main(void)
{
	long longgi;
	char line[9];
	Int4 gi;
	FILE *ofp;
	FILE *ifp = NULL;

	if ( !GetArgs("findspl", NUMARGS, myargs) ) return 1;

	gi = myargs[0].intvalue;
	if (gi <= 0)
	{
		if ((ifp = FileOpen(myargs[1].strvalue, "r")) == NULL)
		{
			Message(MSG_FATAL, "Unable to open input file %s",
		                myargs[1].strvalue);
			return 1;
		}
	}

	if ((ofp = FileOpen(myargs[2].strvalue, "w")) == NULL)
	{
		Message(MSG_FATAL, "Unable to open output file %s",
	                myargs[2].strvalue);
		return 1;
	}

	if (! EntrezInit("findspl", FALSE, NULL))
	{
		Message(MSG_FATAL, "Unable to open Entrez database");
		FileClose(ofp);
		return 1;
	}

	if (gi > 0)
	{
            FindAndPrint(ofp, gi);
	} else {
	    while (fgets(line,sizeof(line),ifp) != NULL) {
	        sscanf(line, "%ld", &longgi);
	        gi = (Int4) longgi;
                FindAndPrint(ofp, gi);
	    }
	    FileClose(ifp);
	}

	FileClose(ofp);
	EntrezFini();

	return 0;
}
