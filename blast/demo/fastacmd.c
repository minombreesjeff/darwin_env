/* $Id: fastacmd.c,v 6.18 2001/12/18 13:01:52 camacho Exp $
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
* File Name:  $RCSfile: fastacmd.c,v $
*
* Author:  Sergei Shavirin
*
* Initial Version Creation Date: 05/20/1997
*
* $Revision: 6.18 $
*
* File Description:
*        FASTA retrievel system using ISAM indexes
*
* $Log: fastacmd.c,v $
* Revision 6.18  2001/12/18 13:01:52  camacho
* Added new flag -D to dump blast database in FASTA format
*
* Revision 6.17  2001/12/10 19:17:32  camacho
* Added option to allow fastacmd to use Ctrl-As as defline separators.
*
* Revision 6.16  2001/10/19 19:46:26  camacho
* Added new feature to dump FASTA files from blast databases, added support for the new database format
*
* Revision 6.15  2000/10/16 20:47:35  madden
* Add -o option to write output to file
*
* Revision 6.14  2000/06/28 16:56:52  madden
* Call Fastacmd_Search_ex, Boolean for target gi only
*
* Revision 6.13  2000/03/08 15:26:03  madden
* Add return statement to Main fct, purify nit
*
* Revision 6.12  2000/01/12 21:05:00  egorov
* Use Fastacmd API
*
* Revision 6.11  1999/12/21 21:26:22  egorov
* Use new parameter of readdb_gi2seq function
*
* Revision 6.10  1999/12/17 20:48:54  egorov
* Fix 'gcc -Wall' warnings and remove old stuff.
*
* Revision 6.9  1999/09/28 19:02:47  egorov
* In the new version of the 'fastacmd' there is not need to
* specify database name if search a GI.
*
* Revision 6.8  1999/02/23 17:17:32  madden
* Remove unused static _accession functions
*
* Revision 6.7  1998/02/11 18:06:43  madden
* Fix for reading IDs in from a file
*
* Revision 6.6  1998/02/06 18:26:35  madden
* Removed stripping of white spaces
*
* Revision 6.5  1998/01/29 19:47:02  madden
* Changed second call from BioseqRawToFasta to BioseqRawToFastaExtra
*
* Revision 6.4  1998/01/27 20:27:03  madden
* Added option to specify sequence line length
*
* Revision 6.3  1998/01/23 21:56:16  madden
* Error messages sent to stderr
*
* Revision 6.2  1998/01/16 22:04:20  madden
* Call to readdb_new_ex, fixed FUM
*
* Revision 6.1  1997/11/07 16:19:15  shavirin
* Added possibility to retrieve redundant accessions
*
* Revision 6.0  1997/08/25 18:19:56  madden
* Revision changed to 6.0
*
* Revision 5.2  1997/05/20 21:00:45  shavirin
* Remove spurious error message in accession/locus retrieval
*
* Revision 5.1  1997/05/20 15:47:30  shavirin
* Initial revision
*
*
* ==========================================================================
*/

#include <ncbi.h>
#include <objseq.h>
#include <objsset.h>
#include <sequtil.h>
#include <seqport.h>
#include <tofasta.h>
#include <readdb.h>

#define NUMARG sizeof(myargs)/sizeof(myargs[0])

static Args myargs [] = {
    { "Database", 
      NULL, NULL, NULL, TRUE, 'd', ARG_STRING, 0.0, 0, NULL},
    { "Search string: GIs, accessions and locuses may be used delimited\n"
      "      by comma.",
      NULL, NULL, NULL, TRUE, 's', ARG_STRING, 0.0, 0, NULL},
    { "Input file wilth GIs/accessions/locuses for batch retrieval",
      NULL, NULL, NULL, TRUE, 'i', ARG_STRING, 0.0, 0, NULL},
    { "Retrieve duplicate accessions",
      "F", NULL, NULL, TRUE, 'a', ARG_BOOLEAN, 0.0, 0, NULL},
    { "Line length for sequence", 
      "80", NULL, NULL, TRUE, 'l', ARG_INT, 0.0, 0, NULL},
    { "Definition line should contain target gi only",
      "F", NULL, NULL, TRUE, 't', ARG_BOOLEAN, 0.0, 0, NULL},
    { "Output file", 
      "stdout", NULL, NULL, TRUE, 'o', ARG_FILE_OUT, 0.0, 0, NULL},
    { "Use Ctrl-A's as non-redundant defline separator", 
      "F", NULL, NULL, TRUE, 'c', ARG_BOOLEAN, 0.0, 0, NULL},
    { "Dump the entire database in fasta format", 
      "F", NULL, NULL, TRUE, 'D', ARG_BOOLEAN, 0.0, 0, NULL}
};

Int2 Main (void)
{
    CharPtr	database, searchstr, batchfile;
    Int4	linelen;
    Boolean	dupl, target, use_ctrlAs, dump_all;
    FILE *outfp;

    if (! GetArgs ("fastacmd", NUMARG, myargs)) {
	     return (1);
    }

    if( !ErrSetLogfile ("stderr", ELOG_APPEND) ) {
	     exit(1);
    }

    database = myargs[0].strvalue;
    searchstr = myargs[1].strvalue;
    batchfile = myargs[2].strvalue;
    dupl = myargs[3].intvalue;
    linelen = myargs[4].intvalue;
    target = myargs[5].intvalue;
    use_ctrlAs = myargs[7].intvalue;
    dump_all = myargs[8].intvalue;

    outfp = FileOpen(myargs[6].strvalue, "w");

    Fastacmd_Search_ex (searchstr, database, batchfile, dupl, linelen, outfp, target, use_ctrlAs, dump_all);

    FileClose(outfp);

    return 0;
}
