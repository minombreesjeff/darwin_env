static char const rcsid[] = "$Id: formatdb.c,v 6.92 2004/01/29 14:56:44 camacho Exp $";

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

   File Name:  formatdb.c

   Author:  Sergei B. Shavirin
   
   Version Creation Date: 10/01/96

   $Revision: 6.92 $

   File Description:  formats FASTA databases for use by BLAST

   $Log: formatdb.c,v $
   Revision 6.92  2004/01/29 14:56:44  camacho
   Removed -A option, FORMATDB_VER_TEXT no longer supported

   Revision 6.91  2003/10/01 18:59:56  camacho
   Fix to creation of custom databases using a gi list and alias files when the
   source database spans multiple volumes.

   Revision 6.90  2003/09/12 20:18:55  camacho
   This change enables the generation of alias files for multiple ASN.1 inputs.

   Revision 6.89  2003/05/30 17:31:09  coulouri
   add rcsid

   Revision 6.88  2003/05/13 16:02:42  coulouri
   make ErrPostEx(SEV_FATAL, ...) exit with nonzero status

   Revision 6.87  2003/05/08 16:02:17  camacho
   Use conditional compilation for reading .formatdbrc

   Revision 6.86  2003/04/03 19:10:59  camacho
   Fixed typo

   Revision 6.85  2003/03/26 18:51:45  camacho

   1. Minor bug fixes.
   2. Added eFDBCleanOpt parameter to FDBOptionsNew.

   Revision 6.84  2003/03/20 14:03:02  camacho
   Allow users to set the membership and link bits

   Revision 6.83  2003/03/11 18:27:45  madden
   Cast value to Int8 to prevent rollover

   Revision 6.82  2003/02/25 15:53:40  beloslyu
   final coma is not allowed after the last element of enum (at least AIX compiler barks)

   Revision 6.81  2003/01/27 20:17:06  camacho
   Bug fix in alias file creation

   Revision 6.80  2003/01/23 22:01:29  camacho
   Minor change

   Revision 6.79  2003/01/22 19:44:51  camacho

   1. Added check for number of volumes greater than 100.
   2. Fixed bug when creating alias files if multiple FASTA inputs are used.
   3. Implemented correct creation of alias files when creating custom
      blast databases with a gi list.

   Revision 6.78  2003/01/16 19:46:07  kans
   changed NULL to 0 to fix Mac compiler error

   Revision 6.77  2003/01/07 17:20:26  camacho
   Added error message when gi file is not found

   Revision 6.76  2002/12/18 15:16:17  camacho
   Minor fixes of command-line arguments

   Revision 6.75  2002/12/16 20:22:19  camacho
   Better error handling when creating alias files

   Revision 6.74  2002/12/13 13:44:50  camacho
   Use FDBOptionsNew to create options structure

   Revision 6.73  2002/12/02 22:43:42  camacho
   Added warning message when no sequences are found when creating alias files

   Revision 6.72  2002/11/06 21:27:46  ucko
   Make 4294967295 explicitly unsigned to avoid warnings.

   Revision 6.71  2002/09/26 02:14:43  camacho
   Allow limiting the number of sequences per volume

   Revision 6.70  2002/09/25 20:14:20  camacho
   Fix for multivolume databases with non-parseable seqids

   Revision 6.69  2002/08/09 19:41:25  camacho
   1) Added blast version number to command-line options
   2) Added explanations for some default parameters

   Revision 6.68  2002/07/24 20:52:55  coulouri
   Change database volume size parameter to megabases

   Revision 6.67  2002/04/19 13:10:43  madden
   Make new database format the default

   Revision 6.66  2002/02/15 22:01:19  beloslyu
   fix from HP

   Revision 6.65  2001/11/06 15:24:20  dondosha
   Roll back previous change - it was not needed

   Revision 6.64  2001/11/05 22:14:49  dondosha
   Allow stdin as input

   Revision 6.63  2001/11/02 19:27:45  camacho
   Fixed problem that would corrupt the BlastDefLine structures for the new database format

   Revision 6.62  2001/07/12 19:35:51  madden
   Set alias_file_name

   Revision 6.61  2001/07/06 19:58:57  madden
   Add NLM_GENERATED_CODE_PROTO and include for fdlobj.h, remove unused variables

   Revision 6.60  2001/06/14 14:18:48  madden
   Replace FD_MakeAliasFile with FD_CreateAliasFile

   Revision 6.59  2001/06/07 20:52:07  shavirin
   Fixed problem with truncation of definition lines when TEXT mode is
   chosen for dump.

   Revision 6.58  2001/06/07 13:13:02  madden
   Set limits for -v arg, set bases_in_volume to UINT4_MAX if not set

   Revision 6.57  2001/05/17 20:21:46  dondosha
   Do not add .00 extension when only one volume created

   Revision 6.56  2001/05/11 20:02:35  madden
   Add oidlist and gifile to FD_CreateAliasFileEx

   Revision 6.55  2001/05/11 18:19:02  madden
   Add option to output binary gifile

   Revision 6.54  2001/05/10 17:28:33  madden
   Make gifile optional

   Revision 6.53  2001/05/10 17:21:11  madden
   Add options to produce alias file from a gifile

   Revision 6.52  2001/05/08 21:56:43  shavirin
   Added possibility to generate tax_id for every definition in Blast FASTA
   definition set in ASN.1 structured definition lines.

   Revision 6.51  2001/05/02 16:22:04  dondosha
   Add NSEQ and LENGTH to alias files in case of multiple inputs to formatdb

   Revision 6.50  2001/04/11 21:00:53  dondosha
   Made functions FD_CreateAliasFile(Ex) public

   Revision 6.49  2001/04/11 20:45:35  dondosha
   Moved appending of .00 for the first volume to FormatDBInit function

   Revision 6.48  2001/04/11 20:14:41  dondosha
   Processing of volumes moved to lower level

   Revision 6.47  2001/03/27 21:11:40  dondosha
   Allow multiple input files for formatting

   Revision 6.46  2001/02/01 22:25:50  shavirin
   Added (uncommented) option to create ASN.1 structured deflines in
   BLAST databases.

   Revision 6.45  2001/01/25 21:08:09  madden
   Fix an ABR

   Revision 6.44  2000/12/12 23:12:09  shavirin
   Fixed some FMR error.

   Revision 6.43  2000/12/08 22:34:35  shavirin
   Added possibility to create Taxonomy lookup database.

   Revision 6.42  2000/11/22 20:53:19  shavirin
   Added possibility to use Taxonomy client/server for creation of ASN.1
   structured deflines with taxonomy ID (Using define TAX_CS_LOOKUP).

   Revision 6.41  2000/11/13 21:37:23  madden
   Use ErrPostEx

   Revision 6.40  2000/11/03 18:13:22  madden
   Print list of bad characters in FASTA input

   Revision 6.39  2000/09/29 16:40:16  shavirin
   Fixed problem with multivolume database creation.

   Revision 6.38  2000/09/18 20:46:36  kans
   added back #include <sqnutils.h>, needed for UseLocalAsnloadDataAndErrMsg

   Revision 6.37  2000/09/12 15:38:35  shavirin
   Error message level set to SEV_WARNING

   Revision 6.36  2000/07/18 19:32:28  shavirin
   Added new option -V to enable check for non-unique string ids in the
   FASTA database. Default is FALSE.

   Revision 6.35  2000/02/17 17:20:59  sicotte
   Change Calling convention for FastaToSeqEntryForDb

   Revision 6.34  2000/02/04 21:52:58  madden
   Use FastaToSeqEntryForDb

   Revision 6.33  1999/12/21 18:31:38  madden
   Fixed bug with writing alias file.

   Revision 6.32  1999/12/17 20:48:54  egorov
   Fix 'gcc -Wall' warnings and remove old stuff.

   Revision 6.31  1999/12/16 15:53:23  egorov
   Typo fixed

   Revision 6.30  1999/09/10 16:30:35  shavirin
   Fixed problems with formating proteins by formatdb

   Revision 6.29  1999/09/09 18:25:51  shavirin
   Changed way to parse ASN.1. Added possibility to parse
   delta sequences.

   Revision 6.28  1999/08/25 20:20:27  shavirin
   Added -s option to create sparse indexes.

   Revision 6.27  1999/08/18 15:00:11  shavirin
   If title missing from args *.pal file will have basename as title.

   Revision 6.26  1999/08/03 16:38:56  shavirin
   Added function FD_CreateAliasFile() for multivolume formating.

   Revision 6.24  1999/07/23 18:59:01  shavirin
   Added support for creation of multivolume databases.

   Revision 6.23  1999/05/13 19:34:19  shavirin
   More changes towards dump from ID.

   Revision 6.21  1999/05/12 15:46:52  shavirin
   Changed parameter in function FDBAddSequence().

   Revision 6.20  1999/04/26 21:06:19  shavirin
   Fixed minor bug.

   Revision 6.19  1999/04/26 19:37:45  shavirin
   Dumping info set to FALSE.

   Revision 6.18  1999/04/26 14:53:16  shavirin
   Fixed memory leaks in FDBAddSequence() function.

   Revision 6.17  1999/04/21 21:44:34  shavirin
   Many functions were moved to "readdb.c" file.

   Revision 6.16  1999/03/21 19:16:59  madden
   Fix problem on round numbers

   Revision 6.15  1999/03/05 21:34:48  madden
   Changes for accession.version

   Revision 6.14  1999/02/04 18:01:48  madden
   Add -n option for basename

   Revision 6.13  1998/11/16 18:34:42  madden
   Add return-value checks

   Revision 6.12  1998/07/13 15:32:17  egorov
   make error message more understandable

   Revision 6.10  1998/06/19 21:05:46  egorov
   Fix MemFree() bug

   Revision 6.9  1998/05/05 13:57:37  madden
   Print version number to log file

   Revision 6.8  1998/04/20 19:14:05  egorov
   Fix just one, but huge MLK

   Revision 6.7  1998/02/23 16:49:14  egorov
   Changes to make the tofasta.c independent on readdb.h

   Revision 6.6  1998/02/18 15:29:31  madden
   Added const to prototype for FormatdbCreateStringIndex

   Revision 6.5  1998/02/11 18:05:32  madden
   Changed program to take ASN.1 as input

   Revision 6.3  1997/12/08 21:55:00  madden
   Parse naked (no bars) as IDs

   Revision 6.2  1997/11/06 18:11:17  madden
   Added indices for naked gnl|PID and backbone entries

   Revision 6.1  1997/10/30 18:15:08  madden
   Changes to SeqIdE2Index to allow lookups by accession strings

   Revision 6.0  1997/08/25 18:20:04  madden
   Revision changed to 6.0

   Revision 1.20  1997/07/28 18:36:55  madden
   Replaced printf with ErrPostEx and fprintf

   Revision 1.19  1997/07/28 14:35:37  vakatov
   Added LIBCALLBACK to the ID_Compare() proto

   Revision 1.18  1997/06/10 18:44:11  shavirin
   Fixed return value from UpdateLookupInfo()

   Revision 1.17  1997/05/19 21:16:30  shavirin
   Changed content of string index file due to E2Iindex API logic

   Revision 1.16  1997/05/12 19:57:38  shavirin
   Added additional dump of Accessions/Locuses into string indexes

   Revision 1.15  1997/05/07 21:08:15  madden
   flipped parse argument default

   Revision 1.14  1997/05/05 17:01:42  shavirin
   Added ability to format "non-parced" seqid-deflines
   Removed not-used d if#defs  with FASTA_ASN

 * Revision 1.13  1997/05/01  17:31:32  shavirin
 * Added dumping of 2 more files: String ISAM SeqId index
 *
 * Revision 1.12  1997/02/25  22:20:39  shavirin
 * Changes in accordance to ISAM API changes
 *
 * Revision 1.11  1997/02/24  21:22:57  shavirin
 * Added dump of numeric ISAM information.
 *
 * Revision 1.10  1996/12/20  00:31:19  madden
 * Protected ambiguity data against big/little endian changes.
 *
 * Revision 1.9  1996/12/19  16:30:36  madden
 * Changes to eliminate ".nac" file for nucl.
 *
 * Revision 1.8  1996/11/27  16:40:19  madden
 * Save build date, Make "o" argument FALSE by default.
 *
 * Revision 1.7  1996/11/26  20:08:08  madden
 * BioseqRawConvert(bsp, Seq_code_ncbistdaa); only called for protein alphabets.
 *
 * Revision 1.6  1996/11/26  19:52:10  madden
 * Removed FORMATDB_VER and added readdb.h (which contains same);
 * Changed phd or nhd to phr or nhr
 *
 * Revision 1.5  1996/11/18  20:53:58  shavirin
 * Forced output protein code to Seq_code_ncbistdaa.
 *
 * Revision 1.4  1996/11/06  23:15:34  shavirin
 * Removed bug with reallocation of index tables
 *

*****************************************************************************/
#define NLM_GENERATED_CODE_PROTO
#include <ncbi.h>
#include <tofasta.h>
#include <sequtil.h>
#include <readdb.h>
#include <sqnutils.h>
#include <taxblast.h>
#include <blastdef.h>
#include <mblast.h>
#include <fdlobj.h>

/* program's arguments */

#define NUMARG (sizeof(dump_args)/sizeof(dump_args[0]))

Args dump_args[] = {
    { "Title for database file",
      NULL, NULL, NULL, TRUE, 't', ARG_STRING, 0.0, 0, NULL},
    {"Input file(s) for formatting",
     NULL, NULL,NULL,TRUE,'i',ARG_FILE_IN, 0.0,0,NULL},
    {"Logfile name:",
     "formatdb.log", NULL,NULL,TRUE,'l',ARG_FILE_OUT, 0.0,0,NULL},
    {"Type of file\n"
     "         T - protein   \n"
     "         F - nucleotide", 
     "T", NULL,NULL,TRUE,'p',ARG_BOOLEAN,0.0,0,NULL},
    {"Parse options\n"
     "         T - True: Parse SeqId and create indexes.\n"
     "         F - False: Do not parse SeqId. Do not create indexes.\n",
     "F", NULL,NULL,TRUE,'o',ARG_BOOLEAN,0.0,0,NULL},
    {"Input file is database in ASN.1 format (otherwise FASTA is expected)\n"
     "         T - True, \n"
     "         F - False.\n",
     "F", NULL,NULL,TRUE,'a',ARG_BOOLEAN,0.0,0,NULL},
    {"ASN.1 database in binary mode\n"
     "         T - binary, \n"
     "         F - text mode.\n",
     "F", NULL,NULL,TRUE,'b',ARG_BOOLEAN,0.0,0,NULL},
    {"Input is a Seq-entry",
     "F", NULL ,NULL ,TRUE,'e',ARG_BOOLEAN,0.0,0,NULL},
    { "Base name for BLAST files",
      NULL, NULL, NULL, TRUE, 'n', ARG_STRING, 0.0, 0, NULL},
    { "Database volume size in millions of letters",
      "0", "0", NULL, TRUE, 'v', ARG_INT, 0.0, 0, NULL},
    { "Create indexes limited only to accessions - sparse",
      "F", NULL, NULL, TRUE, 's', ARG_BOOLEAN, 0.0, 0, NULL},
    { "Verbose: check for non-unique string ids in the database",
      "F", NULL, NULL, TRUE, 'V', ARG_BOOLEAN, 0.0, 0, NULL},
    { "Create an alias file with this name\n"
      "        use the gifile arg (below) if set to calculate db size\n"
      "        use the BLAST db specified with -i (above)", 
      NULL, NULL, NULL, TRUE, 'L', ARG_FILE_OUT, 0.0, 0, NULL},
    {"Gifile (file containing list of gi's)",
     NULL, NULL,NULL,TRUE,'F',ARG_FILE_IN, 0.0,0,NULL},
    {"Binary Gifile produced from the Gifile specified above",
     NULL, NULL,NULL,TRUE,'B',ARG_FILE_OUT, 0.0,0,NULL},
#if 0
     /* disabled for this release of the NCBI C toolkit */
    {"Clean up options for new blast database generation\n"
     "         0 - Never: Do not clean up any 'basename.*' blast db files.\n"
     "         1 - Always: Remove all 'basename.*' blast db files.\n"
     "         2 - Prompt: If any 'basename.*' blast db files are found,\n"
     "                     prompt user.\n",
     "0", "0","2",TRUE,'c',ARG_INT,0.0,0,NULL},
#endif
};

enum {
    title_arg,
    input_arg,
    logfile_arg,
    is_prot_arg,
    parse_arg,
    asn_arg,
    asnbin_arg,
    seqentry_arg,
    basename_arg,
    dbsize_arg,
    sparse_arg,
    nonunique_arg,
    alias_fn_arg,
    gifile_arg,
    bin_gifile_arg,
    cleanup_arg
};

static Int4 GetGiFromSeqId(SeqIdPtr sip)
{
    SeqIdPtr sip_tmp;
    Int4 gi;
    
    gi = -1;
    for(sip_tmp = sip; sip_tmp != NULL; sip_tmp = sip_tmp->next) {
        if(sip_tmp->choice == SEQID_GI) {
            gi = sip_tmp->data.intvalue;
            break;
        }
    }
    return gi;
}

BlastDefLinePtr FDBGetDefAsnFromBioseq(BioseqPtr bsp)
{
    BlastDefLinePtr bdp = NULL, bdp_last, bdp_head;
    CharPtr title, chptr, orig_title;

    if(bsp == NULL)
        return NULL;

    bdp = BlastDefLineNew();
    bdp_head = bdp;

    bdp->seqid = SeqIdSetDup(bsp->id);
    title = BioseqGetTitle(bsp);

    orig_title = title = StringSave(title);

    chptr = NULL;
    if((chptr = StringChr(title, '\1')) != NULL) {
        *chptr = NULLB;
        chptr++;
    }
    bdp->title = StringSave(title);
    bdp_last = bdp;


    while(chptr != NULL) {

        bdp = BlastDefLineNew();
        
        title = chptr;
        
        if((chptr = StringChr(title, ' ')) != NULL) {
            *chptr = NULLB;
            chptr++;
        }
        bdp->seqid = SeqIdParse(title);
        title = chptr;

        if((chptr = StringChr(title, '\1')) != NULL) {
            *chptr = NULLB;
            chptr++;
        }
        if(title != NULL)
            bdp->title = StringSave(title);
        else
            bdp->title = StringSave("No definition found");

        bdp_last->next = bdp;
        bdp_last = bdp;
    }

    MemFree(orig_title);
    return bdp_head;
}

/* Fasta file delimiters */
#define DELIM " "
#define MAX_VOLUMES 99

Boolean FDBCheckFastaInputs(CharPtr fasta_files, Int4 is_prot, Int8
        bases_per_vol, Int4Ptr num_inputs)
{
    Int8 predicted_dblength = 0;
    Char *next_file;

    next_file = StringTokMT(fasta_files, DELIM, &fasta_files);
    predicted_dblength = FileLength(next_file);
    *num_inputs = 1;

    while ((next_file = StringTokMT(fasta_files, DELIM, &fasta_files))) {
        predicted_dblength += FileLength(next_file);
        (*num_inputs)++;
    }

    if (bases_per_vol == 0)
        return TRUE;

    if (!is_prot)
        predicted_dblength /= READDB_COMPRESSION_RATIO;

    if ((predicted_dblength/bases_per_vol) > (MAX_VOLUMES - 10)) {
        ErrPostEx(SEV_ERROR, 0, 0, "Using %s bases per volume will exceed "
                "the maximum number\nof volumes formatdb can create.\n"
                "Please increase this value or do not set it at all.\n",
                Nlm_Int8tostr(bases_per_vol, 0));
        return FALSE;
    }

    return TRUE;
}

Boolean FDBUpdateTaxIdInBdp(BlastDefLinePtr bdp_in)
{

#ifdef TAX_CS_LOOKUP     
    
    BlastDefLinePtr bdp;
    Int4 gi;
    
    for(bdp = bdp_in; bdp != NULL; bdp = bdp->next) {        
        if((gi = GetGiFromSeqId(bdp->seqid)) != NULL) {
            bdp->taxid = tax1_getTaxId4GI(gi);
        }
    }
    
#endif
    return TRUE;
}

void SeqEntryGetLength(SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent)
{
    Int8* length = (Int8*) data;

    if (IS_Bioseq(sep)) {
        BioseqPtr bsp = (BioseqPtr) sep->data.ptrvalue;
        *length += (Int8)bsp->length;
        return;
    } else {
        BioseqSetPtr bssp = (BioseqSetPtr) sep->data.ptrvalue;
        SeqEntryGetLength(bssp->seq_set, data, index, indent);
    }
    return;
}

Int2 Main(void) 
{
    SeqEntryPtr sep;
    FormatDBPtr    fdbp;
    FDB_optionsPtr options;
    BioseqPtr bsp;
    Int2 id_ctr=1;
    Int4 count = 0, sequence_count=0;
    Int4 input_ctr = 0, num_inputfiles = 0;
    Int8 total_length, *lengths = NULL;
    CharPtr error_msg=NULL;
    FILE *fd;
    CharPtr next_db = NULL, file_inputs = NULL, orig_ptr = NULL, tmp = NULL;
    Boolean multiple_inputs = FALSE;
    Char buf[256] = { '\0' };
    Int4Ptr last_oid = NULL;
    CharPtr *inputs = NULL;
    Char tmpbuf1[PATH_MAX], tmpbuf2[PATH_MAX];
    
    /* get arguments */
    StringCpy(buf, "formatdb ");
    StringNCat(buf, BlastGetVersionNumber(), sizeof(buf)-StringLen(buf));
    if (!GetArgs(buf, NUMARG, dump_args))
        return 1;

    if (!SeqEntryLoad())
        return 0;
    
    if (!ErrSetLog(dump_args[logfile_arg].strvalue))
        ErrShow();
    else
        ErrSetOpts(ERR_CONTINUE, ERR_LOG_ON);
    UseLocalAsnloadDataAndErrMsg();
    ErrSetMessageLevel(SEV_WARNING);
    
    /* Parse input string for multiple inputs */
    file_inputs = StringSave(dump_args[input_arg].strvalue);
    tmp = StringTokMT(file_inputs, DELIM, &next_db); 
    if (next_db) {
       if (!dump_args[basename_arg].strvalue) {
          ErrPostEx(SEV_FATAL, 1, 0, "Database base name must be provided "
                  "with multiple input files\n");
          return 1;
       } 
       multiple_inputs = TRUE;
       /* When formatdb takes multiple inputs and writes one blast database,
        * it also writes alias files for each of the inputs passed in. Make
        * sure that none of these inputs has the same name as the basename for
        * the new database */
       do {
           if (!StringCmp(dump_args[basename_arg].strvalue, tmp)) {
               ErrPostEx(SEV_FATAL, 1, 0, "Database base name cannot have the "
                       "same name as one of the input files\n");
               return 1;
           }
       } while ((tmp = StringTokMT(next_db, DELIM, &next_db)));
    }
    MemFree(file_inputs);

    options = FDBOptionsNew(dump_args[input_arg].strvalue,
                            dump_args[is_prot_arg].intvalue,
                            dump_args[title_arg].strvalue,
                            dump_args[asn_arg].intvalue,
                            dump_args[asnbin_arg].intvalue,
                            dump_args[seqentry_arg].intvalue,
                            dump_args[sparse_arg].intvalue,
                            dump_args[nonunique_arg].intvalue,
                            dump_args[parse_arg].intvalue,
                            dump_args[basename_arg].strvalue,
                            dump_args[alias_fn_arg].strvalue,
                            ((Int8)dump_args[dbsize_arg].intvalue)*1000000, 0,
                            FORMATDB_VER, FALSE, 0);
    if (options == NULL)
        return 1;

    options->gi_file = StringSave(dump_args[gifile_arg].strvalue);
    options->gi_file_bin = StringSave(dump_args[bin_gifile_arg].strvalue);
    orig_ptr = options->db_file;
    options->db_file = StringTokMT(options->db_file, DELIM, &next_db); 

    if (options->gi_file && dump_args[alias_fn_arg].strvalue == NULL &&
            options->gi_file_bin == NULL) {
        ErrPostEx(SEV_FATAL, 1,0,"The -F option must be used with either "
                "the -L or -B option\n");
        FDBOptionsFree(options);
        return 1;
    } else if (dump_args[alias_fn_arg].strvalue && options->gi_file_bin) {
        ErrPostEx(SEV_FATAL, 1, 0, "The -L and -B options may not be "
                "specified together");
        FDBOptionsFree(options);
        return 1;
    } else if (options->gi_file_bin && options->gi_file == NULL) {
        ErrPostEx(SEV_FATAL, 1, 0, "The -B option may not be specified "
                "without the -F option");
        FDBOptionsFree(options);
        return 1;
    } else if (dump_args[alias_fn_arg].strvalue && options->gi_file == NULL) {
        ErrPostEx(SEV_FATAL, 1, 0, "The -L option must be specified "
                "with the -F option\n");
        FDBOptionsFree(options);
        return 1;
    }
   
    
    /*** Write alias file using a gilist ***/
    if (options->alias_file_name && options->gi_file) {
        Int8 nletters=0, nletters_tot=0;
        Int4 nseqs=0, nseqs_tot=0;
        CharPtr gifile;
        Int4 i, gi_list_total, ordinal_id, rdfp_ctr = 0;
        BlastDoubleInt4Ptr gi_list;
        ReadDBFILEPtr rdfp, rdfp_tmp;
        Char alias_fn[PATH_MAX], base_fn[PATH_MAX];
    
        rdfp = rdfp_tmp = readdb_new(options->db_file, options->is_protein);
        if (rdfp == NULL) {
            ErrPostEx(SEV_FATAL, 1, 0, "Unable to open BLAST db %s\n", 
                    options->db_file);
            FDBOptionsFree(options);
            return 1;
        }
        if ((gifile = FindBlastDBFile(options->gi_file)) == NULL) {
            ErrPostEx(SEV_FATAL, 1, 0, "Unable to find %s\n", options->gi_file);
            rdfp = readdb_destruct(rdfp);
            FDBOptionsFree(options);
            return 1;
        }
        gi_list = GetGisFromFile(gifile, &gi_list_total);
        gifile = MemFree(gifile);

        /* Iterate through the rdfp's (there might be many) */
        for (; rdfp; rdfp = rdfp->next, rdfp_ctr++) {

            /* Isolate the current rdfp so that we restrict the search for gis
             * to this volume only! */
            ReadDBFILEPtr next = rdfp->next;
            rdfp->next = NULL;

            nseqs = nletters = 0;
            for (i = 0; i < gi_list_total; i++) {
                ordinal_id = readdb_gi2seq(rdfp, gi_list[i].gi, NULL);
                if (ordinal_id >= 0) {
                    nseqs++;
                    nletters += readdb_get_sequence_length(rdfp, ordinal_id);
                }
            }
            rdfp->next = next;  /* restore the next rdfp */

            sprintf(alias_fn, "%s.%02d", options->alias_file_name, rdfp_ctr);
            /* For the base name (DBLIST field in alias file) append the
             * volume number if there are multiple volumes in this database */
            if (rdfp->next || rdfp_ctr != 0)
                sprintf(base_fn, "%s.%02d", options->base_name, rdfp_ctr);
            else
                StringCpy(base_fn, options->base_name);
            FD_CreateAliasFileEx(options->db_title, alias_fn, 0, 
                    options->is_protein, base_fn, 0, 0, 
                    nletters, nseqs, NULL, options->gi_file);
            ErrLogPrintf("Created %s alias file with %ld sequences, %s %s\n",
                    alias_fn, nseqs, Nlm_Int8tostr(nletters,0), 
                    options->is_protein ? "residues" : "bases");
            nseqs_tot += nseqs; nletters_tot += nletters;

        }
        /* Sanity check: Don't write 'ghost' alias files */
        if (nletters_tot == 0 || nseqs_tot == 0) {
            ErrPostEx(SEV_FATAL, 1, 0, "No gis from %s were found in the %s "
                "database", options->gi_file, options->db_file);
            for (i = 0; i <= rdfp_ctr; i++) {
                sprintf(alias_fn,"%s.%02d.%cal", options->alias_file_name, i,
                        options->is_protein ? 'p' : 'n');
                FileRemove(alias_fn);
            }
            FDBOptionsFree(options);
            rdfp_tmp = readdb_destruct(rdfp_tmp);
            return 1;
        }
        /* Adjust alias files if necessary */
        if (rdfp_ctr == 1) { /* single volume database */
            sprintf(tmpbuf1, "%s.%cal", alias_fn, 
                    options->is_protein ? 'p' : 'n');
            sprintf(tmpbuf2, "%s.%cal", options->alias_file_name,
                    options->is_protein ? 'p' : 'n');
            FileRename(tmpbuf1, tmpbuf2);
            ErrLogPrintf("Renamed %s to %s\n", tmpbuf1, tmpbuf2);
        } else { /* multi-volume database */
            Char *p = FD_ConstructMultivolumeDBList(options->alias_file_name,
                    rdfp_ctr);
            /* Create wrapper alias file
             * Note that the total number of sequences and letters is not needed
             * because these will be calculated by readdb when reading the alias
             * files.
             */
            FD_CreateAliasFileEx(options->db_title, options->alias_file_name,
                    0, options->is_protein, p, 0, 0, 0, 0, NULL, NULL);
            ErrLogPrintf("Created wrapper alias file %s for %s\n",
                    options->alias_file_name, p);
            p = MemFree(p);
        }

        gi_list = MemFree(gi_list);
        FDBOptionsFree(options);
        rdfp_tmp = readdb_destruct(rdfp_tmp);
        return 0;
    } else if (options->gi_file_bin && options->gi_file) {
        /*** Convert text gi list to binary format ***/
        Int4 ngis;
        ngis = readdb_MakeGiFileBinary(options->gi_file, options->gi_file_bin);
        ErrLogPrintf("Converted %ld gi(s) to binary format on %s\n",
                ngis, options->gi_file_bin);
        FDBOptionsFree(options);
        return 0;
    }
    
#ifdef TAX_CS_LOOKUP
    if(dump_args[12].intvalue && options->parse_mode) {
        /* These functions will create taxonomy lookup database */
        options->tax_lookup = RDTaxLookupInit();
        options->tax_callback = FDBTaxCallback;
    }
#endif    
    
    /*** Make sure that the inputs will not create too many volumes ***/
    if (!FDBCheckFastaInputs(dump_args[input_arg].strvalue, 
                             options->is_protein,
                             options->bases_in_volume, &num_inputfiles))
        return 1;

    /* Allocate last_oid to keep track of the last ordinal ids that each
     * of the input files had */
    if (multiple_inputs) {
        ASSERT(num_inputfiles > 0);
        last_oid = (Int4Ptr) MemNew(num_inputfiles*sizeof(Int4));
        inputs = (CharPtr *) MemNew(num_inputfiles*sizeof(CharPtr));
        lengths = (Int8Ptr) MemNew(num_inputfiles*sizeof(Int8));
        if (!last_oid || !inputs || !lengths) {
            ErrPostEx(SEV_ERROR, 0, 0, "Out of memory");
            FDBOptionsFree(options);
            return 1;
        }
    }

    /* Initialize formatdb structure */
    if ((fdbp = FormatDBInit(options)) == NULL)
        return 2;        
    
#ifdef SET_ASN1_DEFLINE_BITS
    /* Allow users to set their own membership and link bits using a
     * .formatdbrc file. Useful for formatting purposes */
    if (options->version >= FORMATDB_VER) {
        options->linkbit_listp = FDBLoadLinksTable();
        options->memb_tblp = FDBLoadMembershipsTable();
    }
#endif

    /* Loop on input files */
    while (options->db_file) {
       total_length = 0; 
       /* Input database file maybe either in ASN.1 or in FASTA format */
       if (!options->isASN) {
          /* FASTA format of input database */
          
          if((fd = FileOpen(options->db_file, "r")) == NULL) {
             ErrPostEx(SEV_ERROR, 0, 0, "Could not open %s\n", options->db_file);
             return 3;
          }
          
          /* Get sequences */
          while ((sep = FastaToSeqEntryForDb(fd, 
                                             (Boolean)!options->is_protein,
                                             &error_msg, options->parse_mode, options->base_name, &id_ctr,NULL)) != NULL) {
             
             if(!IS_Bioseq(sep)) { /* Not Bioseq - failure */
                ErrLogPrintf("Error in readind Bioseq Formating failed.\n");
                return 4;
             }
             
             SeqEntrySetScope(sep);
             bsp = (BioseqPtr) sep->data.ptrvalue;            
             
             total_length += bsp->length;
             sequence_count++;
             
             if (error_msg) {
                 Char buffer[42];
                 SeqIdWrite(bsp->id, buffer, PRINTID_FASTA_LONG, 41);
                 ErrPostEx(SEV_WARNING, 0, 0, "Sequence number %ld (%s), %s\n", 
                           sequence_count, buffer, error_msg);
                 error_msg = MemFree(error_msg);
             }
             
             /* the BlastDefLine structure will be built in
              * FDBAddSequence => FDLCreateAsnDF from the bsp's id
              * and title */
             FDBAddBioseq(fdbp, bsp, NULL);             
             SeqEntryFree(sep);
          }
          
          FileClose(fd);
          
          /* Writing multi-volume pointer file */
      FD_MakeAliasFile(options);
          
       } else {
          /* ASN.1 format of input database */
          AsnTypePtr atp, atp2;
          AsnModulePtr amp;
          
          if (! SeqEntryLoad())
             ErrShow();
          
          /* get pointer to all loaded ASN.1 modules */
          amp = AsnAllModPtr();
          
          if (amp == NULL) {
             ErrLogPrintf("Could not load ASN.1 modules.\n");
             return 5;
          }
          
          /* get the initial type pointers */
          
          atp = AsnFind("Bioseq-set");
          if (atp == NULL) {
             ErrLogPrintf("Could not get type pointer for Bioseq-set.\n");
             return 6;
          }
          
          atp2 = AsnFind("Bioseq-set.seq-set.E");
          if (atp2 == NULL) {
             ErrLogPrintf("Could not get type pointer for Bioseq-set.seq-set.E\n");
             return 7;
          }
          
          if ((fdbp->aip = AsnIoOpen (options->db_file, 
                                      options->asnbin ? "rb":"r")) == NULL) {
             ErrLogPrintf("Cannot open input database file. Formating failed...\n");
             return 8;
          }
          
          if (options->is_seqentry) {
             Int8 len = 0;
             /* Seq entry */
             sep = SeqEntryAsnRead(fdbp->aip, NULL);
             FDBAddSeqEntry(fdbp, sep); 
             SeqEntryExplore(sep, (Pointer)&len, SeqEntryGetLength);
             SeqEntryFree(sep);
             sequence_count++;
             total_length += len;
          } else {
             /* Bioseq-set */
             
             while ((atp = AsnReadId(fdbp->aip, amp, atp)) != NULL) {
                if (atp == atp2) {   /* top level Seq-entry */
                   Int8 len = 0;
                   sep = SeqEntryAsnRead(fdbp->aip, atp);
                   
                   FDBAddSeqEntry(fdbp, sep);
                   SeqEntryExplore(sep, (Pointer)&len, SeqEntryGetLength);
                   SeqEntryFree(sep);
                   sequence_count++;
                   total_length += len;
                } else {
                   AsnReadVal(fdbp->aip, atp, NULL);
                }
             }
          } /* end "if Bioseq or Bioseq-set */
          
          
       } /* end "if FASTA or ASN.1" */
       
       if (multiple_inputs) {
           /* record the ordinal ids, input file names and input file lengths
            * in the following arrays. This will be used later to
            * create the multiple alias files in case that the multiple input
            * results in a multi-volume database */
           inputs[input_ctr] = options->db_file;
           lengths[input_ctr] = total_length;
           last_oid[input_ctr++] = sequence_count;
       }
       options->db_file = StringTokMT(next_db, DELIM, &next_db);
       
    } /* Loop on input files */

    /* Dump indexes, deallocate structure, arrays, etc. */

    if(FormatDBClose(fdbp))
        return 9;
    options->db_file = orig_ptr;

    /* If multiple inputs were given, create an alias file for each of the
     * fasta file inputs */
    if (multiple_inputs) {
        ReadDBFILEPtr rdfp, rdfp_tmp;
        Boolean span_multiple_rdfp = FALSE, first_time = TRUE;
        Int4 start_oid = 1, stop_oid, vol_ctr = 0;
        Char basename[PATH_MAX]; /* save the name of individual alias files */
        Char dblist[PATH_MAX]; /* save the name(s) of all alias files when an
                                  input spans multiple volumes */
 
        ASSERT(input_ctr == num_inputfiles);
        rdfp = rdfp_tmp = readdb_new(dump_args[basename_arg].strvalue, 
                                     options->is_protein);
        if (rdfp == NULL) {
            ErrPostEx(SEV_FATAL, 1, 0, 
                    "Cannot create alias files for multiple inputs");
            FDBOptionsFree(options);
            return 1;
        }
 
        /* For each rdfp write the corresponding alias file(s) */
        ErrLogPrintf("\nCreating alias files for multiple FASTA inputs...\n");
        input_ctr = 0;
        MemSet(dblist, 0, sizeof(dblist));
        MemSet(basename, 0, sizeof(basename));

        for (; input_ctr < num_inputfiles; input_ctr++) {
            while (rdfp) {

                sprintf(basename, "%s.%02d", inputs[input_ctr], vol_ctr);

                if (first_time || span_multiple_rdfp) {
                    start_oid = 1;
                    first_time = FALSE;
                } else
                    start_oid = last_oid[input_ctr-1] - rdfp->start+1;

                if (last_oid[input_ctr] > (rdfp->stop+1)) {
                    stop_oid = rdfp->stop+1;
                    span_multiple_rdfp = TRUE;
                    StringCat(dblist, basename); StringCat(dblist, " ");
                } else if (span_multiple_rdfp) {
                    StringCat(dblist, basename); StringCat(dblist, " ");
                    stop_oid = last_oid[input_ctr] - rdfp->start;
                } else {
                    stop_oid = last_oid[input_ctr] - rdfp->start;
                }

                ErrLogPrintf("Input %s (up to %ld) alias file %s (%ld-%ld)\n",
                        inputs[input_ctr], last_oid[input_ctr], basename,
                        start_oid, stop_oid);
                FD_CreateAliasFileEx(NULL, basename, 0, 
                            options->is_protein, rdfp->filename, 
                            start_oid, stop_oid, lengths[input_ctr], 
                            0, NULL, NULL);

                if (last_oid[input_ctr] > rdfp->stop) {
                    rdfp = rdfp->next;
                    vol_ctr++;
                } else 
                    break;
            }

            if (span_multiple_rdfp) {
                /* Create wrapper alias file for the corresponding volumes */
                FD_CreateAliasFileEx(NULL, inputs[input_ctr], 0,
                        options->is_protein, dblist, 0, 0, lengths[input_ctr],
                        0, NULL, NULL);
                span_multiple_rdfp = FALSE;
                ErrLogPrintf("Created wrapper alias file %s for %s\n",
                        inputs[input_ctr], dblist);
            } else {
                /* Rename the alias file just created */
                sprintf(tmpbuf1, "%s.%cal", basename,
                        options->is_protein ? 'p' : 'n');
                sprintf(tmpbuf2, "%s.%cal", inputs[input_ctr],
                        options->is_protein ? 'p' : 'n');
                FileRename(tmpbuf1, tmpbuf2);
                ErrLogPrintf("Renamed %s to %s\n", tmpbuf1, tmpbuf2);
            }
            MemSet(dblist, 0, sizeof(dblist));
        }
        rdfp_tmp = readdb_destruct(rdfp_tmp);
        MemFree(last_oid);
        MemFree(inputs);
        MemFree(lengths);
    }

#ifdef TAX_CS_LOOKUP
    if(dump_args[12].intvalue && options->parse_mode) {
        RDTaxLookupClose(options->tax_lookup);
    }
#endif    

#ifdef SET_ASN1_DEFLINE_BITS
    if (options->version >= FORMATDB_VER) {
        options->linkbit_listp = FDBDestroyLinksTable(options->linkbit_listp);
        options->memb_tblp = FDBDestroyMembershipsTable(options->memb_tblp);
    }
#endif
    FDBOptionsFree(options);

    return 0;
    
} /* main()*/



