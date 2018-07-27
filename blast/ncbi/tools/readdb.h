/*
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
*/

/*****************************************************************************

File name: readdb.h

Author: Tom Madden

Contents: defines and prototypes used by readdb.c and formatdb.c.

******************************************************************************/

/*
* File Name: readdb.h
*
* Author: Tom Madden
*
* Version Creation Date:   3/21/95
*
* $Revision: 6.157 $
*
* File Description: 
*       Functions to rapidly read databases from files produced by formatdb.
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
* ==========================================================================
*
*
* RCS Modification History:
* $Log: readdb.h,v $
* Revision 6.157  2005/02/22 14:15:48  camacho
* Pass bioseq data type by reference to FDBAddBioseq
*
* Revision 6.156  2004/12/04 03:41:09  camacho
* Add extra enum for fastacmd -D option for error checking
*
* Revision 6.155  2004/12/03 04:57:57  camacho
* Fix name conflict in enumeration for fastacmd dump types
*
* Revision 6.154  2004/12/02 20:37:31  camacho
* + fastacmd feature to dump list of gis
*
* Revision 6.153  2004/09/27 16:29:34  madden
* Make title on SI_Record dynamically allocated
*
* Revision 6.152  2004/08/25 14:45:23  camacho
* Refactorings to allow formatdb process multiple deflines
*
* Revision 6.151  2004/07/14 18:35:12  camacho
* Added comments for readdb_get_header_ex
*
* Revision 6.150  2004/07/09 17:09:12  camacho
* Updated documentation for last_oid_assigned
*
* Revision 6.149  2004/07/08 19:49:03  camacho
* Contributions from ID1 Group:
* 1) SI_Record structure.
* 2) Refactoring of FDBAddSequence2 to allow addition of non-redundant sequences
* when creating BLAST databases.
*
* Revision 6.148  2004/06/29 20:59:23  camacho
* Added last_oid_assigned to ReadDBSharedInfo structure
*
* Revision 6.147  2004/04/16 18:14:50  camacho
* Made division field in DI_Record larger
*
* Revision 6.146  2004/02/24 14:06:01  camacho
* Added support for approximate sequence length calculation for nucleotide
* sequences.
*
* Revision 6.145  2004/02/04 15:35:05  camacho
* Rollback to fix problems in release 2.2.7
*
* Revision 6.143  2003/07/08 18:42:40  camacho
* Elaborated fastacmd return values
*
* Revision 6.142  2003/06/13 19:56:48  dondosha
* Removed unneeded argument in FastaToBlastDB
*
* Revision 6.141  2003/04/25 18:55:27  camacho
* 1. Added readdb_merge_gifiles to deal with Microbial blast database issues.
* 2. Minor fixes to Int4List functions.
*
* Revision 6.140  2003/04/22 21:30:14  camacho
* Added Int4 list utilities
*
* Revision 6.139  2003/04/22 19:04:57  camacho
* Moved GiList structure to generic list of 4-byte integers
*
* Revision 6.138  2003/04/16 15:39:37  coulouri
* fix compiler warning
*
* Revision 6.137  2003/04/15 19:09:13  camacho
* Completed implementation of PIG interface
*
* Revision 6.136  2003/04/10 15:11:37  camacho
* Include PIG interface in __cplusplus
*
* Revision 6.135  2003/04/09 21:46:00  camacho
* Added basic PIG interface
*
* Revision 6.134  2003/04/08 19:45:35  camacho
* Defined invalid PIG
*
* Revision 6.133  2003/04/08 15:37:15  camacho
* Extended FDBAddSequence2 to take pig
*
* Revision 6.132  2003/04/01 21:51:36  camacho
* Made fastacmd functions & structure non-static
*
* Revision 6.131  2003/03/27 22:26:04  camacho
* Add error messages and non-zero return value on error for fastacmd
*
* Revision 6.130  2003/03/26 19:11:22  camacho
* Minor change to previous commit
*
* Revision 6.129  2003/03/26 18:50:07  camacho
* Added eFDBCleanOpt to formatdb API
*
* Revision 6.128  2003/01/30 21:57:28  camacho
* Added more detailed comment to readdb_new_ex2
*
* Revision 6.127  2003/01/22 19:41:21  camacho
* Added function to build multi-volume db list for creating alias files
*
* Revision 6.126  2002/12/20 14:37:34  coulouri
* Fix prototype for RDBTaxInfoInit()
*
* Revision 6.125  2002/12/17 20:33:25  camacho
* Removed unnecessary function attribute
*
* Revision 6.124  2002/12/16 20:22:48  camacho
* Removed unused options in formatdb options structure
*
* Revision 6.123  2002/12/16 05:01:55  camacho
* Fixes to previous commit
*
* Revision 6.122  2002/12/13 13:43:25  camacho
* Changes to set links and membership bits in formatdb API
*
* Revision 6.121  2002/11/25 17:23:28  camacho
* 1) Changed file access to blast taxonomy databases: only 2 files are loaded
*    for an entire chain of rdfp's.
* 2) Fixed memory leak in FindBlastDBFile.
* 3) Protect NlmOpenMFILE against NULL argument.
*
* Revision 6.120  2002/10/25 16:49:45  camacho
* Added Michael Kimelman's FDBAddSequence2
*
* Revision 6.119  2002/10/03 14:13:44  camacho
* Added support for gilist field in alias file in multivolume databases
*
* Revision 6.118  2002/09/26 02:14:42  camacho
* Allow limiting the number of sequences per volume
*
* Revision 6.117  2002/09/25 20:14:20  camacho
* Fix for multivolume databases with non-parseable seqids
*
* Revision 6.116  2002/07/30 15:28:50  camacho
* Added fastacmd function to parse SeqLocs
*
* Revision 6.115  2002/07/29 15:45:19  camacho
* Made readdb_get_taxnames a LIBCALL function
*
* Revision 6.114  2002/07/24 19:31:48  raytseli
* much simpler and more efficient approach to using madvise()
* .
*
* Revision 6.113  2002/07/22 13:06:42  raytseli
* explicitly allow setting of the advice type for madvise()
* .
*
* Revision 6.112  2002/07/18 17:39:54  raytseli
* changed ifdef OS_UNIX_SUN to ifdef OS_UNIX_SOL for madvise()
* .
*
* Revision 6.111  2002/07/18 15:54:26  raytseli
* added function to explicitly set madvise() block size, and madvise() sync mode.
*
* Revision 6.110  2002/07/18 15:01:54  raytseli
* correct problem with pointer format "%p" ErrPostEx() handling on linux.
* Add extern func to allow explicit madvise() functionality activation.
*
* Revision 6.109  2002/07/17 17:15:06  raytseli
* only allow madvise()-related stuff on SUN or Linux.
* .
*
* Revision 6.108  2002/07/17 16:54:54  raytseli
* additional #ifdefs to allow compilation.
*
* Revision 6.106  2002/07/17 14:36:54  raytseli
* incorporated madvise into readdb
* .
*
* Revision 6.105  2002/07/14 21:02:08  camacho
* Added extra features to fastacmd
*
* Revision 6.104  2002/07/09 16:41:52  camacho
* Made taxonomy databases multi-thread safe
*
* Revision 6.103  2002/06/26 00:45:37  camacho
*
* Added readdb_get_totals_ex2 to allow recalculation of database length as
* well as total number of sequences after the virtual oidlist has been
* created.
*
* Revision 6.102  2002/06/04 21:45:39  dondosha
* Corrected the readdb_get_sequence_number function in case of multiple-volume databases
*
* Revision 6.101  2002/06/04 20:22:56  camacho
* Fixed taxonomy databases to work w/o mmap
*
* Revision 6.100  2002/05/15 20:23:47  camacho
* Added wgs_{mouse,anthrax} criteria functions
*
* Revision 6.99  2002/05/02 21:52:06  camacho
* Support for genmask's new month/subset mask combinations
*
* Revision 6.98  2002/04/18 19:35:07  camacho
* 1. Added fdfilter/genmask callbacks for wgs subsets
* 2. Modified fdfilter/genmask refseq_protein callback function
* 3. Fixed problem in readdb_read_alias_file to read multiple oidlists
*
* Revision 6.97  2002/03/08 16:58:50  camacho
* Added accessions to dump info files *.[pn]di
*
* Revision 6.96  2002/01/25 17:06:57  camacho
* Added new criteria to create new refseq databases
*
* Revision 6.95  2002/01/24 18:47:48  camacho
* Moved RDBTaxNamesFree from readdb.[ch] to txalign.[ch]
*
* Revision 6.94  2002/01/11 19:22:26  camacho
* 1. Added preferred_gi field to ReadDBFILE structure.
* 2. Modified FDReadDeflineAsn to return the preferred gi as the
*    first element of the list of BlastDefLine structures (if set).
*
* Revision 6.93  2001/12/18 13:01:51  camacho
* Added new flag -D to dump blast database in FASTA format
*
* Revision 6.92  2001/12/10 19:17:13  camacho
* Added option to allow fastacmd to use Ctrl-As as defline separators.
*
* Revision 6.91  2001/11/09 19:05:35  dondosha
* ReadDBFreeSharedInfo and ReadDBOpenMHdrAndSeqFiles made static in readdb.c
*
* Revision 6.90  2001/11/02 18:30:12  dondosha
* Added prototypes for readdb_get_sequence_number, PrintDbInformationWithRID
*
* Revision 6.89  2001/10/19 13:40:31  camacho
* Updated the DI_Record structure and moved some function prototypes to allow their use by fdfilter
*
* Revision 6.88  2001/10/01 18:43:37  camacho
* Added BlastDBToFasta function
* Added readdb_get_header_ex function
*
* Revision 6.87  2001/10/01 18:37:32  camacho
* readdb.h
*
* Revision 6.86  2001/07/12 19:27:45  madden
* Add alias_file_name to Options
*
* Revision 6.85  2001/06/21 18:27:28  shavirin
* Moved into files txalign.[c,h] functions returning taxonomy names
* from Bioseq created from Blast database.
*
* Revision 6.84  2001/06/14 16:22:46  madden
* Add prototype for FD_MakeAliasFile
*
* Revision 6.83  2001/05/23 21:17:24  shavirin
* Added definitions for bits related to sequence-to-database affiliation.
*
* Revision 6.82  2001/05/11 19:59:41  madden
* Add gi_file_bin to FDOptions, oidlist and gifile to FD_CreateAliasFileEx
*
* Revision 6.81  2001/05/10 17:19:53  madden
* Add number_seqs arg to FD_CreateAliasFileEx
*
* Revision 6.80  2001/05/08 21:58:28  shavirin
* Added possibility to generate tax_id for every definition in Blast FASTA
* definition set in ASN.1 structured definition lines.
*
* Revision 6.79  2001/05/02 16:22:05  dondosha
* Add NSEQ and LENGTH to alias files in case of multiple inputs to formatdb
*
* Revision 6.78  2001/04/11 21:00:53  dondosha
* Made functions FD_CreateAliasFile(Ex) public
*
* Revision 6.77  2001/04/11 20:14:06  dondosha
* Added volume information to FDB_options structure
*
* Revision 6.76  2001/03/29 20:15:59  madden
* Removed unneeded #define
*
* Revision 6.75  2001/03/23 17:23:54  madden
* Move FDGetDeflineAsnFromBioseq to txalign.[ch]
*
* Revision 6.74  2001/02/05 18:52:01  shavirin
* Blast database size was changed from Uint4 to Uint8 - this corrected
* invalidly printed database size for large databases.
*
* Revision 6.73  2000/12/12 23:14:42  shavirin
* Added functions to initialize taxonomy names database and search functions
* to get all taxonomy names given tax_id using this database.
*
* Revision 6.72  2000/12/08 22:25:01  shavirin
* Added code for creation Taxonomy lookup database using formatdb API.
*
* Revision 6.71  2000/11/28 18:20:40  madden
* Comments from Sergei on FDB_options
*
* Revision 6.70  2000/11/24 15:41:58  shavirin
* Added parameter tax_id into function FDBAddBioseq().
*
* Revision 6.69  2000/11/22 19:52:44  shavirin
* Added definition of the new function FDGetDeflineAsnFromBioseq()
*
* Revision 6.68  2000/10/26 18:30:50  dondosha
* Added gifile member to ReadDBFILE structure
*
* Revision 6.67  2000/10/13 17:31:52  shavirin
* Adjusted calls to readdb_get_header for ASN.1 structured deflines.
*
* Revision 6.66  2000/09/29 16:38:30  shavirin
* Added new function FDB_FreeCLOptions(FDB_optionsPtr options).
*
* Revision 6.65  2000/09/16 15:20:17  shavirin
* Added AsnIoPtr structure for ASN.1 structured deflines.
*
* Revision 6.64  2000/09/07 20:49:58  shavirin
* Added parameters to support ASN.1 defline dump for blast db. FORMATDB_VER 3->4
* Added parameter FORMATDB_VER_TEXT for backward compatibility.
*
* Revision 6.63  2000/07/18 19:29:29  shavirin
* Added new parameter test_non_unique to suppress check for non-unique
* strings ids in the database - default - TRUE.
*
* Revision 6.62  2000/07/07 21:20:08  vakatov
* Get all "#include" out of the 'extern "C" { }' scope!
*
* Revision 6.61  2000/06/28 16:55:50  madden
* Add function Fastacmd_Search_ex, gi_target to ReadDBFILEPtr
*
* Revision 6.60  2000/06/19 20:06:43  madden
* Add ready Boolean to readdb_get_sequence_ex, for nucl. sequence the data is then in blastna format with sentinel bytes
*
* Revision 6.59  2000/05/22 18:46:23  dondosha
* Merged all Boolean members in ReadDBFILE structure into a single Int4
*
* Revision 6.58  2000/05/09 15:54:20  shavirin
* Added function ReadDBBioseqSetDbGeneticCode().
*
* Revision 6.57  2000/05/03 16:18:34  dondosha
* Added prototype for FastaToBlastDB
*
* Revision 6.56  2000/03/13 18:36:38  madden
* Added insert_ctrlA Boolean to readdb_get_bioseq_ex
*
* Revision 6.55  2000/03/10 18:52:11  madden
* Add prototype for readdb_get_filebits
*
* Revision 6.54  2000/02/09 19:35:52  madden
* Added readdb_MakeGiFileBinary
*
* Revision 6.53  2000/01/12 21:03:52  egorov
* 1. Introduce Fastacmd API function - Fastacmd_Search
* 2. Rearrange order of functions to have Fastacmd, ID1, and CommonIndex stuff separate.
*
* Revision 6.52  2000/01/07 16:00:25  madden
* Alias db length is Int8 instead of Uint4
*
* Revision 6.51  2000/01/03 15:46:16  lewisg
* add prototype for readdb_get_num_entries_total_real
*
* Revision 6.50  1999/12/31 14:23:21  egorov
* Add support for using mixture of real and maks database with gi-list files:
* 1. Change logic of creating rdfp list.
* 2. BlastGetDbChunk gets real databases first, then masks.
* 3. Propoper calculation of database sizes using alias files.
* 4. Change to CommonIndex to support using of mask databases.
* 5. Use correct gis in formated output (BlastGetAllowedGis()).
* 6. Other small changes
*
* Revision 6.49  1999/12/22 20:34:34  dondosha
* Add full_filename and shared_info to ReadDBFile structure, plus prototypes of related routines
*
* Revision 6.48  1999/12/21 20:00:27  egorov
* Add new parameter into readdb_gi2seq()
*
* Revision 6.47  1999/12/17 21:33:01  egorov
* Add support for the 'month' subset.
*
* Revision 6.46  1999/12/15 17:34:32  egorov
* 1. Introduce MASK_WORD_SIZE constant variable.
* 2. Introduce DI_Record structure for fileld of DI index file.
* 3. Introduce UpdateIndexStruct which is used in callback for UpdateCommonIndexFile.
* 4. Add new field to ReadDbFile structure - aliasfilename, which used
*    while deciding which gi to use.
*
* Revision 6.45  1999/11/26 22:06:59  madden
* Added READDB_UNPACK_BASE_N macro
*
* Revision 6.44  1999/11/23 22:02:27  madden
* Added readdb_get_totals_ex that may use alias file values
*
* Revision 6.43  1999/11/23 21:51:24  madden
* Changes for freeing OIDlist
*
* Revision 6.42  1999/11/12 14:16:14  madden
* Allow other initialization states in readdb_new_ex2
*
* Revision 6.41  1999/09/24 18:59:16  egorov
* Add functions prototypes
*
* Revision 6.40  1999/09/23 15:02:53  egorov
* Use more descriptive name
*
* Revision 6.39  1999/09/22 21:50:57  egorov
* Add mask DB stuff
*
* Revision 6.38  1999/09/13 16:18:40  shavirin
* Added function readdb_get_bioseq_ex, which has possibility
* to bypass ObjMgr registration.
*
* Revision 6.37  1999/09/10 16:30:18  shavirin
* Fixed problems with formating proteins by formatdb
*
* Revision 6.36  1999/09/09 18:25:05  shavirin
* Added functions to parse ASN.1 with formatdb
*
* Revision 6.35  1999/08/25 20:17:39  shavirin
* Added option to create and retrieve from sparse indexes.
*
* Revision 6.34  1999/08/02 13:33:58  shavirin
* Rolled back last changes.
*
* Revision 6.32  1999/05/27 15:51:29  shavirin
* Added function readdb_get_defline ()
*
* Revision 6.31  1999/05/18 20:35:31  madden
* Changes to read an alias file for multiple db searches and ordinal ID lists
*
* Revision 6.30  1999/05/13 19:31:14  shavirin
* More changes toward dump from ID.
*
* Revision 6.29  1999/05/12 15:48:03  shavirin
* Changed parameter in function FDBAddSequence().
*
* Revision 6.28  1999/05/06 15:25:27  egorov
* Remove static function declaration
*
* Revision 6.27  1999/04/26 14:36:29  shavirin
* Added ability to dump statistics.
*
* Revision 6.26  1999/04/21 22:55:39  kans
* was not checked in
*
* Revision 6.25  1999/02/22 21:48:03  egorov
* Optimize GIs2OIDs not reinitializing ISAM indicies for non-exclisive databases, but use already initialized rdfp's field for that.
*
* Revision 6.24  1999/02/05 13:47:05  madden
* Add basename for formatdb
*
* Revision 6.23  1998/12/14 21:49:23  egorov
* new max gi number memeber in CommonIndexHead structure and therefore no need for COMMON_INDEX_TABLE_SIZE
*
* Revision 6.22  1998/12/14 16:05:36  egorov
* *** empty log message ***
*
* Revision 6.21  1998/09/14 15:11:19  egorov
* Add support for Int8 length databases; remove unused variables
*
* Revision 6.20  1998/08/27 15:02:37  madden
* Added LIBCALL for readdb_get_sequence_ex
*
* Revision 6.19  1998/08/24 14:59:57  madden
* readdb_get_sequence_ex function
*
* Revision 6.18  1998/08/11 17:49:48  madden
* is_na becomes is_aa
*
* Revision 6.17  1998/07/01 14:03:07  egorov
* Fix bug with a thread freeing CommonIndex: add new flag to rdfp
*
* Revision 6.16  1998/06/26 16:51:15  egorov
* Fix CommonIndex bugs
*
* Revision 6.15  1998/06/24 21:03:40  egorov
* Remove memory leaks
*
* Revision 6.12  1998/05/22 20:19:54  madden
* Changes to fix multi-db search bug
*
* Revision 6.11  1998/02/26 22:34:24  madden
* Changes for 16 bit windows
*
* Revision 6.10  1998/02/11 17:49:38  madden
* Added structures and prototypes for formatdb to take ASN.1 as input
*
* Revision 6.9  1998/01/16 22:03:00  madden
* Added init_indices Boolean
*
* Revision 6.8  1997/11/26 22:48:38  madden
* Added readdb_parse_db_names for multiple db searches
*
* Revision 6.7  1997/11/07 16:16:36  shavirin
* Added definition of new function readdb_acc2fastaEx()
*
* Revision 6.6  1997/10/24 19:08:16  madden
* Added ReadDBGetDb and ReadDBGetDbId
*
* Revision 6.5  1997/09/24 22:37:06  madden
* Added readdb_destruct_element
*
* Revision 6.4  1997/09/16 16:31:40  madden
* More changes for multiple db runs
*
* Revision 6.3  1997/09/12 19:55:38  madden
* Added readdb_compare
*
* Revision 6.2  1997/09/11 18:49:40  madden
* Changes to enable searches against multiple databases.
*
* Revision 6.1  1997/08/27 14:46:59  madden
* Changes to enable multiple DB searches
*
* Revision 6.0  1997/08/25 18:53:59  madden
* Revision changed to 6.0
*
* Revision 1.26  1997/05/12 21:34:05  madden
* readdb_new allows indeterminate database type
*
* Revision 1.25  1997/05/12 21:11:42  shavirin
* Added definition for function readdb_acc2fasta()
*
* Revision 1.23  1997/05/07 21:04:02  madden
* Added prototype for SeqId2OrdinalId and changed FORMATDB_VER 2->3
*
* Revision 1.22  1997/05/01 17:26:58  shavirin
* Added definition for the function readdb_seqid2fasta()
*
 * Revision 1.21  1997/02/25  22:16:32  shavirin
 * Changes in accordance to ISAM API changes
 *
 * Revision 1.20  1997/02/25  16:28:38  shavirin
 * Added new entries in ReadDBFILEPtr structure to do search by gi
 * number.
 *
 * Revision 1.19  1996/12/19  16:29:56  madden
 * Changes to eliminate ".nac" file for nucl.
 *
 * Revision 1.18  1996/12/17  21:34:46  madden
 * Changes to allow deflines for inidividual entries to be retrieved.
 *
 * Revision 1.17  1996/12/11  18:42:36  madden
 * Added prototypes for BioseqFetch functions.
 *
 * Revision 1.16  1996/11/27  16:39:11  madden
 * Added functions to return filename and date. FORMATDB_VER 1->2
 *
 * Revision 1.15  1996/11/26  19:54:27  madden
 * Added check for database in standard places.
 *
 * Revision 1.14  1996/11/22  19:05:48  madden
 * removed ifdef for OLD_BIT_ORDER.
 *
 * Revision 1.13  1996/11/08  21:45:03  madden
 * Removed function readdb_get_partial_unpacked_sequence.
 *
 * Revision 1.12  1996/11/07  22:33:00  madden
 * Added prototype for readdb_ambchar_present.
 *
 * Revision 1.11  1996/11/04  18:50:20  shavirin
 * Added definitions for ambiguity information pointers
 *
 * Revision 1.10  1996/10/31  16:29:55  shavirin
 * Changed definitions due to reverce of residues in BLAST database
 * for nucleotide sequences from (4321) to (1234)
 * New dumper now required to create BLAST databases.
 *
 * Revision 1.9  1996/09/27  19:12:17  madden
 * Added function readdb_get_bioseq to obtain a BioseqPtr from the BLAST databases.
 *
 * Revision 1.8  1996/09/26  15:09:21  madden
 * Corrected misplaced comment.
 *
 * Revision 1.7  1996/09/23  14:37:35  madden
 * Replaced CharPtr (for sequence) with Uint1Ptr.
 *
 * Revision 1.6  1996/09/20  21:59:16  madden
 * *** empty log message ***
 *
 * Revision 1.5  1996/09/13  20:01:52  madden
 * defined READDB_COMPRESSION_RATIO
 *
 * Revision 1.4  1996/09/13  18:55:04  madden
 * Added function readdb_get_partial_unpacked_sequence.
 *
 * Revision 1.3  1996/08/29  20:42:01  madden
 * memory mapping moved to the corelib (in ncbimem.[ch]).
 *
 * Revision 1.2  1996/08/07  18:32:05  madden
 * Moved define of MMAP_AVAIL from readdb.h to readdb.c
 *
 * Revision 1.1  1996/08/05  19:48:21  madden
 * Initial revision
 *
 * Revision 1.12  1996/08/02  14:20:06  madden
 * Added readdb_attach function.
 *
 * Revision 1.11  1996/07/31  13:09:17  madden
 * Changes for partial copy of ReadDB structure.
 *
 * Revision 1.10  1996/07/25  20:45:20  madden
 * Change to arguments of readdb_get_sequence.
 *
 * Revision 1.9  1996/07/25  12:56:15  madden
 * readdb_get_sequence changed to allow for systems w/o mmap.
 *
 * Revision 1.8  1996/06/20  17:00:11  madden
 * Added "__cplusplus" define.
 *
 * Revision 1.7  1996/06/20  16:16:36  madden
 * Replaced int's with Int4's.
 *
 * Revision 1.6  1996/05/16  19:50:15  madden
 * Added documentation block.
 *
 * Revision 1.5  1996/04/22  21:42:07  madden
 * New prototype for readdb_get_sequence
 *
 * Revision 1.4  1996/04/11  14:30:06  madden
 * Memory-mapping added.
 *
 * Revision 1.3  1996/03/29  21:28:30  madden
 * Added function readdb_get_sequence_length.
 *
 * Revision 1.2  1996/03/28  20:42:36  madden
 * Added functions readdb_get_title, readdb_is_prot and
 * readdb_get_formatdb_version.
 *
 * Revision 1.1  1996/03/26  19:38:08  madden
 * Initial revision
 *
 *
*/

#ifndef _READDB_
#define _READDB_


/****************************************************************************/
/* INCLUDES */
/****************************************************************************/

#include <ncbi.h>
#include <objloc.h>
#include <sequtil.h>
#include <ncbisam.h>
#include <tofasta.h>
#include <txalign.h>

/* This define should be added here to pacify NT build */
#ifndef NLM_GENERATED_CODE_PROTO
#define NLM_GENERATED_CODE_PROTO 
#endif


#include <fdlobj.h>

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************/
/* Structure of index file header - old version */
/****************************************************************************/

/*
   4 bytes  4 bytes      4 bytes    title_len bytes      n bytes    
  <version><is_protein?><title_len><the_database_title><date_stamp>

(title_len+n)%8 bytes
  <ex_bytes><num_of_seqs><total_len><max_seq_len>

  num_of_seqs*4bytes   num_of_seqs*4bytes     num_of_seqs*4bytes
<defline_offset_table><sequence_offset_table><ambig_offset_table>
*/

/****************************************************************************/
/* DEFINES */
/****************************************************************************/

/* Defines used to retrieve a base out of a packed byte. */
/* x should be unsigned (Uint1) to avoid sign extension problems. */

#define READDB_UNPACK_BASE_1(x) ((x)>>6)
#define READDB_UNPACK_BASE_2(x) (((x)>>4) & 0x03)
#define READDB_UNPACK_BASE_3(x) (((x)>>2) & 0x03)
#define READDB_UNPACK_BASE_4(x) ((x) & 0x03)
#define READDB_UNPACK_BASE_N(x, N) (((x)>>(2*(N))) & 0x03)

/* Default location of the databases. */
#ifdef OS_UNIX    
#define BLASTDB_DIR "/usr/ncbi/db/blast"
#else
#define BLASTDB_DIR ""
#endif    

/* Compress 4 bytes to one. */
#define READDB_COMPRESSION_RATIO 4

/* Character used to separate deflines from different entries that all
belong to the same sequence. */
#define READDB_DEF_SEPARATOR '\001'

/* Choices for whether it's a protein db or not. */
#define READDB_DB_IS_NUC 0
#define READDB_DB_IS_PROT 1
#define READDB_DB_UNKNOWN 2

#define READDB_CONTENTS_ALLOCATED  0x00000001
#define READDB_IS_PROT             0x00000002
#define READDB_HANDLE_COMMON_INDEX 0x00000004
#define READDB_NOT_FIRST_TIME      0x00000008
#define READDB_NO_SEQ_FILE         0x00000010
#define READDB_KEEP_HDR_AND_SEQ    0x00000020

/*** Choices for how much to initialize on startup in readdb_new_internal. ***/

/* attempt to memory map all files. */
#define READDB_NEW_DO_ALL               ((Uint1) (1<<0))
/* Only open the nin or pin files for a database report. */
#define READDB_NEW_DO_REPORT            ((Uint1) (1<<1))
/* Only open the nin (or pin) and nsq (or psq) files for a search. */
#define READDB_NEW_DO_SEARCH            ((Uint1) (1<<2))
/* Open only index (nin or pin) files for memory mapping */
#define READDB_NEW_INDEX                ((Uint1) (1<<3))
/* Same as above and memory map blast taxonomy db files */
#define READDB_NEW_DO_TAXDB             ((Uint1) (1<<4))

/* The following variables are shared by formatdb and readdb. */
/* version of formatdb.

   Explanations: last text version of defline used for blast database
   was 3 - all subsequent versions use ASN.1 for defline storage.
   For backward compatibility if database version is 3 new program
   will handle it OK. If database version > 3 - exact match of version
   is needed to proceed.
   
*/

#define FORMATDB_VER_TEXT 3
#define FORMATDB_VER      4

/* 'Magic' number at the beginning of a binary gi list that indicates it is binary. */
#define READDB_MAGIC_NUMBER UINT4_MAX

/****************************************************************************/
/* TYPEDEFS */
/****************************************************************************/

typedef struct nlm_mfile {
	Nlm_MemMapPtr mem_mapp;	/* structure containing mem-map info, 
				produced by Nlm_MemMapInit. */
	FILE PNTR fp;		/* FILE pointer. */
	Uint1Ptr  mmp_begin,	/* beginning of mmap'ed are. */
		  mmp,		/* present position of mmap'ed pointer. */
		  mmp_end;	/* end of mmap'ed area. */
	Int4	  file_size;	/* size of file that is mmap'ed. */
	Boolean   mfile_true;	/* If TRUE then mmap succeeded. */
	Boolean   contents_allocated; /* If TRUE, the contents have been allocated
					and are not merely a copy. */
	Uint1Ptr mmp_madvise_end; /* madvise() file offset */
} NlmMFILE, PNTR NlmMFILEPtr;

/*
Open the file and initialze the memory mapping.
*/
NlmMFILEPtr LIBCALL NlmOpenMFILE PROTO((CharPtr name));

/*
Undo the memory mapping.
*/
NlmMFILEPtr LIBCALL NlmCloseMFILE PROTO((NlmMFILEPtr mfp));

/*
Read "nitems" of size "size" from a memory mapped file into "buffer"
usig the memory-mapped file given by "mfp".
*/
Int4 LIBCALL NlmReadMFILE PROTO((Uint1Ptr buffer, size_t size, Int4 nitems, NlmMFILEPtr mfp));

/*
	"fseek" to a point in the memory mapped file.
*/
Int4 LIBCALL NlmSeekInMFILE PROTO((NlmMFILEPtr mfp, long offset, Int4 ptrname));

/*
        What is the offset (in bytes) to the beginning of the file.
        Analog to ftell.
*/
Int4 LIBCALL NlmTellMFILE PROTO((NlmMFILEPtr mfp));

/* Generic 4-byte integer list */
typedef struct _gilist {
    Int4    count, allocated;
    Int4Ptr i;
} Int4List, *Int4ListPtr;

/* Creates a new list of 4-byte integers */
Int4ListPtr LIBCALL 
Int4ListNew PROTO((void));

/* Creates a new list of 4-byte integers of size s */
Int4ListPtr LIBCALL 
Int4ListNewEx PROTO((Int4 s));

/* Deallocates the list of 4-byte integers */
Int4ListPtr LIBCALL 
Int4ListFree PROTO((Int4ListPtr lp));

/* Reads a list of newline separated 4-byte integers.
 * Caller is responsible for deallocating the return value */
Int4ListPtr LIBCALL 
Int4ListReadFromFile PROTO((CharPtr filename));

/* Appends i to the end of the list, reallocating memory if necessary. Returns
 * FALSE if it cannot allocate more memory */
Boolean LIBCALL 
Int4ListAdd PROTO((Int4ListPtr lp, Int4 i));

/* Returns the concatenation of list1 and list2, freeing both parameters. It
 * returns NULL if both lists are empty and if it cannot allocate more memory */
Int4ListPtr LIBCALL
Int4ListConcat PROTO((Int4ListPtr *list1, Int4ListPtr *list2));

/* Attempts to reallocate new_size elements to the list. Returns NULL on
 * incorrect arguments or if it cannot allocate more memory */
Int4ListPtr LIBCALL
Int4ListResize PROTO((Int4ListPtr listp, Int4 new_size));

/* Performs a binary search for key on lp.
   Returns the index into lp->i where key is located or -1 if key is not found
 */
Int4 LIBCALL
Int4ListBSearch PROTO((Int4ListPtr lp, Int4 key));

/* Ascendingly sorts the list and removes repeated entries */
Int4ListPtr LIBCALL
Int4ListMakeUnique PROTO((Int4ListPtr list));

/* Returns the ascending sorted intersection of list1 and list2, freeing the
 * both parameters */
Int4ListPtr LIBCALL
Int4ListIntersect PROTO((Int4ListPtr *list1, Int4ListPtr *list2));


/*
	Common index structures
 */

/*
#define INDEXFN		"/net/cruncher/usr/ncbi/db/disk.blast/blast2/comindex/mmfile.idx"
#define RECORDFN	"/net/cruncher/usr/ncbi/db/disk.blast/blast2/comindex/mmfile.rec"
*/
#define COMMONINDEX_FN	"comindex.mm"
#define DB_CONFIG_FN	"dblist.txt"

typedef struct  CommonIndex{
    Int4        dbmask; 	/* mask to define which db contains the GI */
    Int4        oftenOID;       /* ordinal ID for the GI in most often DB */
} CommonIndex, *CommonIndexPtr;
 
typedef	struct	CommonIndexResult {
    Int4	gi;	/* GI */
    Int4	oid;	/* OID */
    Int2	dbid;	/* database ID */
    struct CommonIndexResult *next;	/* make a list */
} CommonIndexResult, *CommonIndexResultPtr;

/* Data bases */
 
typedef struct	DataBaseID {
    CharPtr	name;	/* database name like gss, nr, etc */
    Char	id;	/* integer ID, value from 0, to 32, used for bitmasks */
    Boolean	isprot;	/* says TRUE if database contains proteins, FALSE otherwise */
} DataBaseID, *DataBaseIDPtr;

typedef struct	CommonIndexHead {
    CommonIndexPtr	ci;
    Nlm_MemMapPtr	memmap;
    Int2		num_of_DBs;
    DataBaseIDPtr	dbids;
    Int4		maxgi; /* maximum GI number permitted */
} CommonIndexHead, *CommonIndexHeadPtr;

typedef	struct	OIDList {
    CharPtr	filename;	/* name of the file containing OID list */
    Uint4Ptr	list;		/* array of OID's */
    Uint4Ptr	memory;		/* memory to keep the OID's (element list).
				if this is NULL, then list is memory mapped. */
    Int4	total;		/* number of elements in the array */
    NlmMFILEPtr mfp;		/* Used for memory-mapped file. */
} OIDList, *OIDListPtr;

OIDListPtr OIDListFree (OIDListPtr oidlist);

typedef struct read_db_shared_info {
   Int2 nthreads;
   NlmMFILEPtr headerfp, sequencefp;

   /* This is the ordinal id of the last chunk assigned to a thread when
    * iterating over a database via the BlastSeqSrc interface with multiple
    * threads. It should not be used in other contexts. It is analogous to 
    * the db_chunk_last field of the BlastThrInfo structure.
    * Please note that in case of a linked list of ReadDBFILE structures, only
    * the first shared_info->last_oid_assigned field is significant when
    * performing an iteration with multiple threads.
    */
   Uint4 last_oid_assigned;
} ReadDBSharedInfo, *ReadDBSharedInfoPtr;

/* ---------------------------------------------------------------------*/
/* -- Here is set of definitions used with taxonomy info database ----- */
/* ---------------------------------------------------------------------*/

/* The following #define allows for the creation of taxonomy databases along
 * with the blast databases. Please note that the code to create the blast
 * databases is NOT thread-safe! */
/*#define FDB_TAXONOMYDB*/

typedef	struct _RDBTaxId {
    Uint4 taxid;
    Uint4 offset;
} RDBTaxId, PNTR RDBTaxIdPtr;

typedef	struct _RDBTaxInfo {
    Int4        all_taxid_count; /* Total number of taxids in the database */
    Int4        reserved[4];     /* reserved */
    NlmMFILEPtr taxfp;           /* Memory mapped index file */
    RDBTaxIdPtr taxdata;         /* Index tax_id/file offset */
    Boolean     taxdata_alloc;   /* true if taxdata was allocated */
    NlmMFILEPtr name_fd;         /* Pointer to the file with taxonomy names */
    Boolean     taxinfo_alloc;   /* Flag to determine structure ptr ownership */
} RDBTaxInfo, *RDBTaxInfoPtr;

typedef	struct _RDBTaxLookup {
    Int4 all_taxid_count; /* Total number of taxids in the database */
    Int4 taxids_in_db;
    RDBTaxNamesPtr *tax_array; /* This array's index correspond to tax_id and
                                  value of the cell corresponds to tax names 
                                  if  any */
    VoidPtr tax_data;      /* This data may be set and used by the callback */
} RDBTaxLookup, *RDBTaxLookupPtr;

typedef Boolean (*TaxCallbackFunc) (RDBTaxLookupPtr tax_lookup, Int4 tax_id);


/*
 * sequence info record (SI_Record):
 * > contains information about given gi 
 * > most of it will be dumped to *[np]di files
 * > form a linked list for identical gis
 * > used for transferring data into AddSequence interface
 *
 * Contribution from Michael Kimelman/Olga Cherenkov from
 * NCBI's ID1 group.
 */

typedef struct si_record {
    struct  si_record PNTR next;
    Int4    gi;
    char    seqid[256]; /* seqid in FASTA format */
    char*   title;      /* defline */
    Int4    taxid;
    Int4    owner;
    char    div[4];
    Int4    ent;  /* entity [sat_key in the ID] */
} SI_Record, PNTR SI_RecordPtr;

/*    ----
      Here are functions for run-time blast in relation to the
      Taxonomy blast database 
      ----  */

#define TAXDB_ON_FTP "ftp://ftp.ncbi.nih.gov/blast/db/taxdb.tar.gz"
#define BLAST_TAXDB_FILENAME "taxdb"

/* Initialize taxonomy lookup database. returns NULL if failure or
   this database do not exists */
RDBTaxInfoPtr  RDBTaxInfoInit(void);

/* Free memory, unmap files etc. related to the taxonomy database */
void RDBTaxInfoClose(RDBTaxInfoPtr tip);

/* Main function to get taxonomy names for given tax_id from
   blast taxonomy database. Returns NULL if tax_id is not in the database */
RDBTaxNamesPtr RDBGetTaxNames(RDBTaxInfoPtr tip, Int4 tax_id);

#define TAX_DB_MAGIC_NUMBER 0x8739

typedef struct read_db_file {
	struct read_db_file PNTR next;
        Int4 parameters; /* All boolean parameters */
   /* Bits: 0 - contents allocated
            1 - is protein
	    2 - handle common index
	    3 - not first time
	    4 - do not open sequence files
	    5 - do not close header and sequence files in readdb_get_link
   */
   /* 0: Are contents of this struct allocated, or not?  Does NOT include
      the actual structure and buffer, below. */
   /* 1: If TRUE, sequence is protein, otherwise dna. */
   /* 2: TRUE only for the initial thread;  needed for proper freeing of the CommonIndex */
   /* 3: For recursive calls to readdb_new_ex2. */
	CharPtr filename;	/* name of the input (w/o extensions). */
	CharPtr aliasfilename;	/* name of the alias of input */
/* The files pointers for "file" (above), the index file, the file 
containing the headers, and the sequence file. */
        NlmMFILEPtr indexfp, headerfp, sequencefp;
	Int4	header_index_offset;	/* offset to beginning of header index in indexfp. */
	CharPtr title,	/* Database Title. */
		date;	/* Date and time database was prepared. */
	Int4 num_seqs, /* Number of sequences in the database. */
	      formatdb_ver;	/* Version of formatdb used. */
	BlastDefLinePtr blast_deflinep;  /* when not NULL, points to the first defline of the seq*/
	Int4 	start,	/* 1st ordinal id in this file. */
		stop;	/* last ordinal id in this file. */
	Int8 totlen;	/* Total length of database. */
	Uint4 maxlen;	/* Length of longest sequence in database. */
	Int8 aliaslen;	/* Length of the database as read from alias file */
	Uint4 aliasnseq;/* Number of seqs of the database as read from alias file */
/* The "index" arrays specify the offsets (in files) of the header and 
sequence information. */
	Uint4Ptr header_index,	sequence_index, ambchar_index;	
	Uint4Ptr header_index_start,	sequence_index_start, ambchar_index_start;	
/* Buffer and allocated amount of this buffer.  These should always be
NULL (i.e., NOT USED) if mem-mapping is used; only used to store sequence
if there is no mem-mapping or it failed. */

    ISAMObjectPtr nisam_opt;  /* Object for numeric search */
    ISAMObjectPtr sisam_opt;  /* Object for string search */
    ISAMObjectPtr isam_pig;   /* Object for PIG search */
    RDBTaxInfoPtr taxinfo;    /* This object if not NULL - pointer to
                                     the taxonomy names database */
	Uint1Ptr buffer;
	Int4 allocated_length;
	CommonIndexHeadPtr  cih;       /* head of the common index */
	Int2	            filebit;   /* bit corresponding to the DB file */
	Int2		    aliasfilebit;/* bit corresponding to the DB alias file */
	OIDListPtr	    oidlist;   /* structure containing a list of ordinal ID's. */
    Int4            membership_bit; /* membership bit read from .[pn]al file for structured asn deflines */
	Int4		    sparse_idx;/* Sparse indexes indicator */
        Char                full_filename[PATH_MAX]; /* Full path for the file */
        ReadDBSharedInfoPtr shared_info;
	Int4 	            gi_target; /* only this gi should be retrieved */
                                       /* if non-zero. */
    CharPtr             gifile;   /* Path to a file with the gi list, should 
									 always be NULL after readdb_new* calls */
    Int4ListPtr     gilist; 	  /* storage for the above file in memory */
    Int4		    preferred_gi; /* this gi should be listed first */
                                  /* in the bioseq if non-zero */
	Int4    last_preloaded; /* starting ordinal id of the last preloaded file block */
} ReadDBFILE, PNTR ReadDBFILEPtr;
    
/* Function prototypes */
Int4    GI2OID(CommonIndexHeadPtr cih, Int4 gi, Int4 dbmask, Int4 alias_dbmask,
	Int2Ptr dbid, Int2Ptr alias_dbid, ReadDBFILEPtr rdfp);
Int2	DBShift(Int2 num_of_DBs, DataBaseIDPtr dbids, CharPtr dbname, Boolean is_prot);
CharPtr	DBName(Int2 num_of_DBs, DataBaseIDPtr dbids, Int2 shift);
Boolean	DBisProt(Int2 num_of_DBs, DataBaseIDPtr dbids, Int2 shift);
CommonIndexResultPtr	GIs2OIDs(CommonIndexHeadPtr cih,
			Int4Ptr gis, Int4 number_of_gis, Int4 dbshift, ReadDBFILEPtr rdfp);
Int2	SeniorBit(Int4	bitmask);
CommonIndexHeadPtr	CommonIndexInit(CharPtr indexfilename);
void	CommonIndexDestruct(CommonIndexHeadPtr cihp);
Int2	bit_engine_firstbit (Int4 word);
Int2Ptr	bit_engine_arr(Int4 word);
Int2	bit_engine_numofbits(Int4 word);
Int2	ParseDBConfigFile(DataBaseIDPtr *dbidsp, CharPtr path);
CharPtr	FindBlastDBFile (CharPtr filename);
CharPtr	FindDBbyGI(CommonIndexHeadPtr cih, Int4 gi, Uint1 *is_prot);
RDBTaxNamesPtr LIBCALL readdb_get_taxnames PROTO((
            ReadDBFILEPtr rdfp, Int4 tax_id));

/* mmap's */
 
NLM_EXTERN Nlm_MemMapPtr EA_MemMapInit(const Nlm_Char PNTR name, Boolean readonly);

/****************************************************************************/
/* FINCTION DEFINITIONS */
/****************************************************************************/
/* Deallocate the memory mapping of header and sequence files */
ReadDBFILEPtr ReadDBCloseMHdrAndSeqFiles PROTO((ReadDBFILEPtr rdfp));

/* 
Intitialize the readdb structure using the database "filename".
If no database is used, set filename to NULL. 
*/
ReadDBFILEPtr LIBCALL readdb_new PROTO((CharPtr filename, Uint1 is_prot));

/*
	init_indices should be TRUE if entire database is to be searched, otherwise
	it can be FALSE.
*/
ReadDBFILEPtr LIBCALL readdb_new_ex PROTO((CharPtr filename, Uint1 is_prot, Boolean init_indices));

/* 
 * Initializes the blast database specified in the argument list.
 * filename: blast database to initialize
 * is_prot: is this database protein ?
 * init_state: bitwise-OR of the READDB_NEW_* values (selectively mmap certain
 *             files)
 * oidlist: Path to the ordinal id list to use (this is mmap'd)
 * gilist: Path to the gi list to use (this is not resolved until the search
 *         is conducted (see BlastProcessGiLists)
*/
ReadDBFILEPtr LIBCALL readdb_new_ex2 PROTO((CharPtr filename, Uint1 is_prot,
            Uint1 init_state, CharPtr oidlist, CharPtr gilist));


/* 
Deallocate the ReadDBFILEPtr. 
*/
ReadDBFILEPtr LIBCALL readdb_destruct PROTO((ReadDBFILEPtr readdb));

ReadDBFILEPtr LIBCALL readdb_destruct_element PROTO((ReadDBFILEPtr rdfp));


/*
        Attach to an already open ReadDBFILEPtr.  Duplicate the
        indexfp, sequencefp, and headerfp structures as the pointers
        there (i.e., mmp) will need to be manipulated.  Do not
        change the FILE PNTR fp.
*/
ReadDBFILEPtr LIBCALL readdb_attach PROTO((ReadDBFILEPtr rdfp));

/*
        Checks whether a ReadDBFILEPtr is the original, or just attaced.
        It does this by checking the rdfp->contents_allocated flag.
*/
Boolean LIBCALL readdb_copy PROTO((ReadDBFILEPtr rdfp));

/*
	Checks two ReadDBFILEPtr to see if they refer to the same
	database.
*/
Boolean LIBCALL readdb_compare PROTO((ReadDBFILEPtr rdfp1, ReadDBFILEPtr rdfp2));


/*
        Get total length and number of sequences in multiple databases.
*/

Boolean LIBCALL readdb_get_totals PROTO((ReadDBFILEPtr rdfp_list, Int8Ptr total_len, Int4Ptr total_num));

/*
        Get total length and number of sequences in multiple databases.
        if 'use_alias' is TRUE, values from the alias file will be used
        if non-zero.
*/

Boolean LIBCALL
readdb_get_totals_ex PROTO((ReadDBFILEPtr rdfp_list, Int8Ptr total_len, Int4Ptr total_num, Boolean use_alias));

/* retrieves the total number of sequences and database length in the
 * rdfp_list. use_alias and use_virtual_oidlist are mutually exclusive
 * options (both of them cannot be true at the same time). If 
 * use_virtual_oidlist is TRUE, this function assumes that this rdfp_list has 
 * been processed by BlastProcessGiLists */
Boolean LIBCALL
readdb_get_totals_ex2 PROTO ((ReadDBFILEPtr rdfp_list, Int8Ptr dblen, 
        Int4Ptr nseq, Boolean use_alias, Boolean use_virtual_oidlist));

/* Enumerated type to determine if the database length (number of
 * bases/residues) should be approximated or calculated exactly by
 * readdb_get_totals_ex3 */
typedef enum {
    eExact,
    eApproximate
} EAccountingMode;

/* This function is identical to readdb_get_totals_ex2 but it uses its last
 * argument to determine if in the case of nucleotide databases the exact
 * database length is required. If eExact is used, the exact database size is
 * calculated, if eApproximate is used, an approximation is returned. This is
 * done to avoid having to touch every last byte of each sequence to determine
 * the exact length of the database when it is restricted by a virtual oidlist.
 * The EAccountingMode argument is irrelevant for protein databases, where this
 * function always return the exact database length. Same assumption about 
 * BlastProcessGiLists as in readdb_get_totals_ex2 applies.
 */
Boolean LIBCALL
readdb_get_totals_ex3 PROTO ((ReadDBFILEPtr rdfp_list, Int8Ptr dblen, 
        Int4Ptr nseq, Boolean use_alias, Boolean use_virtual_oidlist,
        EAccountingMode acc_mode));

/* 
Get the sequence with sequence_number and put it in buffer.  No memory
is allocated for this if memory-mapped files are used, otherwise it is.  
Return the length of the sequence.
*/
Int4 LIBCALL readdb_get_sequence PROTO((ReadDBFILEPtr rdfp, Int4 sequence_number, Uint1Ptr PNTR buffer));

/* 
	Gets the sequence number "sequence_number".  The sequence returned includes
	all ambiguity information.  THis funciton should only be used for nucleic
	acid sequences, for proteins use readdb_get_sequence.

	buffer contains the sequence and is reallocated if *buffer_length is not long enough.

	The length of the sequence requested is the return value.
	protein sequences are always returned as Seq_code_ncbistdaa,
	nucleotide sequences as Seq_code_ncbi4na.
*/
Int4 LIBCALL readdb_get_sequence_ex PROTO((ReadDBFILEPtr rdfp, Int4 sequence_number, Uint1Ptr PNTR buffer, Int4 *buffer_length, Boolean ready));

/* Gets sequence number by gi number. Returnes -1 if gi not found or
   other negative value if NISAM library faults. Non-negative value
   means success. Use numeric ISAM indexes.
*/
Int4 LIBCALL readdb_gi2seq(ReadDBFILEPtr rdfp, Int4 gi, Int4Ptr start);

/* Gets sequence number by SeqId number. Returnes -1 if gi not found or
   other negative value if SISAM library faults. Non-negative value
   means success. Use string ISAM indexes.
*/
Int4 LIBCALL readdb_seqid2fasta(ReadDBFILEPtr rdfp, SeqIdPtr sip);

/* Gets sequence number by Accession/Locus string. Returnes -1 
   if accession not found or
   other negative value if SISAM library faults. Non-negative value
   means success. Use string ISAM indexes.
*/
Int4 LIBCALL readdb_acc2fasta(ReadDBFILEPtr rdfp, CharPtr string);

/* Gets array of sequence numbers by Accession/Locus string. Returnes -1 
   if accession not found or
   other negative value if SISAM library faults. Non-negative value
   means success. Use string ISAM indexes.
*/
Int4 LIBCALL readdb_acc2fastaEx(ReadDBFILEPtr rdfp, CharPtr string,
                                Int4Ptr PNTR ids, Int4Ptr count);

/*
Gets a BioseqPtr containing the sequence in sequence_number.
*/
BioseqPtr LIBCALL readdb_get_bioseq PROTO((ReadDBFILEPtr rdfp, Int4 sequence_number));
BioseqPtr LIBCALL readdb_get_bioseq_ex PROTO((ReadDBFILEPtr rdfp, Int4 sequence_number, Boolean use_objmgr, Boolean insert_ctrlA));

/* 
   Gets the exact sequence length for protein sequences, but for nucleotide
   sequences it gets the length of the sequence +/- at most 3 bases (last byte
   is not examined, therefore the return value is an approximation).
 */
Int4 LIBCALL readdb_get_sequence_length_approx PROTO((ReadDBFILEPtr rdfp,
                                                       Int4 sequence_number));

/*
Get the length of the sequence.
*/
Int4 LIBCALL readdb_get_sequence_length PROTO((ReadDBFILEPtr rdfp, Int4 sequence_number));

/* 
Get the ID and definition for the sequence with sequence_number.
It is the caller's RESPONSIBILITY to DEALLOCATE "id" and "description". 
*/
Boolean LIBCALL readdb_get_descriptor PROTO((ReadDBFILEPtr rdfp, Int4 sequence_number, SeqIdPtr PNTR id, CharPtr PNTR description));
Boolean
readdb_get_defline (ReadDBFILEPtr rdfp, Int4 sequence_number, CharPtr PNTR description);

/*
Get the ID's and headers for a sequence. 
*/
Boolean LIBCALL
readdb_get_header PROTO((ReadDBFILEPtr rdfp, Int4 sequence_number, Uint4Ptr header_index , SeqIdPtr PNTR id, CharPtr PNTR description));

/* 
Get the ID's, headers, taxid, memberships, and links for a sequence.
Returns FALSE if the sequence_number is not applicable in the context of the
database in rdfp (i.e.: masked databases), otherwise it will return TRUE until
there are sequences associated with this sequence_number (then it returns
FALSE).
*/
Boolean LIBCALL 
readdb_get_header_ex PROTO((ReadDBFILEPtr rdfp, Int4 sequence_number, 
                           Uint4Ptr header_index, SeqIdPtr PNTR id, 
                           CharPtr PNTR description, Int4 PNTR taxid, 
                           ValNodePtr PNTR memberships, ValNodePtr PNTR links));

/* 
 Get the Int4Ptr to ambiguity buffer
*/
Boolean  LIBCALL readdb_get_ambchar PROTO((ReadDBFILEPtr rdfp, Int4 sequence_number, Uint4Ptr PNTR ambchar_return));

/*
	Check whether ambiguity characters are present in the sequence. 
*/
Boolean LIBCALL readdb_ambchar_present PROTO((ReadDBFILEPtr rdfp, Int4 sequence_number));

/* 
Get the total length (in bp or residues) of the database. 
*/
Int8 LIBCALL readdb_get_dblen PROTO((ReadDBFILEPtr rdfp));

/* 
Get the number of entries in the database. 
*/
Int4 LIBCALL readdb_get_num_entries PROTO((ReadDBFILEPtr rdfp));

/* 
Get the total number of entries in all the files.
*/
Int4 LIBCALL readdb_get_num_entries_total PROTO((ReadDBFILEPtr rdfp));

/*
Obtains the total number of real database sequences from all the ReadDBFILE structures. 
*/

Int4 LIBCALL readdb_get_num_entries_total_real PROTO((ReadDBFILEPtr rdfp));

/* 
Get the length of the longest sequence in the database. 
*/
Int4 LIBCALL readdb_get_maxlen PROTO((ReadDBFILEPtr rdfp));

/* 
Get the title (i.e., name) of the database. 
NOTE: the CharPtr returned is not owned by the caller! 
*/
CharPtr LIBCALL readdb_get_title PROTO((ReadDBFILEPtr rdfp));

/* 
Get the name of the file used for formatting.
NOTE: the CharPtr returned is not owned by the caller! 
*/
CharPtr LIBCALL readdb_get_filename PROTO((ReadDBFILEPtr rdfp));

/* 
Get the date the database was formatted. 
NOTE: the CharPtr returned is not owned by the caller! 
*/
CharPtr LIBCALL readdb_get_date PROTO((ReadDBFILEPtr rdfp));

/* 
Is this a protein database? 
*/
Boolean LIBCALL readdb_is_prot PROTO((ReadDBFILEPtr rdfp));

/*
        Parses the databases names (if more than one) from
        'filenames' into buffer.  buffer should already be
        long enough and allocated.  The funciton should be
        repeatedly called until TRUE is returned.
*/
Boolean LIBCALL readdb_parse_db_names PROTO((CharPtr PNTR filenames, CharPtr buffer));

/* 
Get the version of formatdb used on this database. 
*/
Int4 LIBCALL readdb_get_formatdb_version PROTO((ReadDBFILEPtr rdfp));

/*
	returns the 'filebits' associated with a certain ordinal number.
	This is done by going to the rdfp for that ordinal id and
	gathering the filebits.
*/
Boolean LIBCALL readdb_get_filebits PROTO((ReadDBFILEPtr rdfp, Int4 ordinal_id, Uint2Ptr filebit, Uint2Ptr aliasfilebit));

/* For the BioseqFetch functions. */

Boolean LIBCALL ReadDBBioseqFetchEnable PROTO((CharPtr program, CharPtr dbname, Boolean is_na, Boolean now));

Boolean LIBCALL ReadDBBioseqSetDbGeneticCode PROTO((Int4 db_genetic_code));

void LIBCALL ReadDBBioseqFetchDisable PROTO((void));

/* Converts a SeqIdPtr to an ordinal_id, which readdb can use to look
up sequences etc.  Negative numbers are returned if the SeqIdPtr
cannot be converted. */
Int4 SeqId2OrdinalId PROTO((ReadDBFILEPtr rdfp, SeqIdPtr sip));

/*
	Returns the ReadDBFILEPtr by the database ID.
*/
ReadDBFILEPtr ReadDBGetDb PROTO((ReadDBFILEPtr list, Int2 db_id));

/*
	Returns the Database ID.
*/
Int2 ReadDBGetDbId PROTO((ReadDBFILEPtr list, ReadDBFILEPtr target));


/********************/
/*     formatdb     */
    
    /* Type definitions */

typedef struct FASTALookup {
    Int4Ptr table;          /* Main buffer for gi/fasta_id pairs */
    Int4    allocated;      /* Nunber of Uint4 allocated */
    Int4    used;           /* Number of Uint4 used      */
} FASTALookup, PNTR FASTALookupPtr;

/* Structure that holds the link information as read from the file */
typedef struct _linkinfo {
    Int4 bit_number;      /* indicates the position in links bit array */
    Int4ListPtr gi_list;  /* update links bit array for gis in this list */
} LinkInfo, *LinkInfoPtr;

/* Structure that holds the membership information */
typedef Boolean (*GMCriteriaFunc) (VoidPtr direc);

typedef struct _membinfo {
    Int4 bit_number;    /* indicates the position in the membership bit array */
    GMCriteriaFunc criteria; /* function pointer that is invoked to
                                 determine wheather certain sequence
                                 belongs to the membership represented by
                                 this bit_number */
} MembInfo, *MembInfoPtr;

/* Options to clean up blast database files from a previous instance of the
 * database with the same name as the one about to be created. This has been
 * added to prevent the case in which an alias file might have precedence over
 * a single-volume blast database. */
typedef enum EFDBCleanOpt {
    eCleanNever = 0,    /* don't remove older files of the db to be created,
                           just overwrite them or ignore alias files */
    eCleanAlways,       /* clean up all older files of the db to be created */
    eCleanPrompt,       /* Assumes interactive program */
    eCleanOptMax
} EFDBCleanOpt;

/*** PIG (Protein Identifier Group) interface ***/

#define PIG_NONE        -1          /* No protein identifier group */

/* PIG table structure
 * From this information the formatdb API creates a pair of ISAM files to map 
 * PIGs to ordinal ids */
typedef struct FDBPigTable {
    Int4Ptr     pop;                /* list of pig/ordinal id pairs */
    Int4        count, allocated;   /* keep track of table size */
} FDBPigTable, * FDBPigTablePtr;

/* Allocate a PIG table structure */
FDBPigTablePtr LIBCALL
FDBPigTableNew PROTO((void));

/* Deallocate a PIG table structure */
FDBPigTablePtr LIBCALL
FDBPigTableFree PROTO((FDBPigTablePtr fptp));

/* Add a PIG to the PIG table structure, return FALSE on error */
Boolean LIBCALL
FDBAddPig PROTO((FDBPigTablePtr fptp, Int4 pig, Int4 oid));

/* Retrieve the PIG for a given ordinal id */
Int4 LIBCALL
readdb_get_pig PROTO((ReadDBFILEPtr rdfp, Int4 oid));

/* Retrieve the ordinal id corresponding to a given PIG (analogous to
 * readdb_gi2seq) */
Int4 LIBCALL
readdb_pig2oid PROTO((ReadDBFILEPtr rdfp, Int4 pig, Int4Ptr start));

/************************************************/

typedef struct _FDB_options {
    Int4  version;   /* Version of the database created by formatdb program
	    	 	currently supported are 3 - FORMATDB_VER_TEXT and
	    	 	4 - FORMATDB_VER - for ASN.1 structured deflines */
    CharPtr db_title;    /* Title for the database to be created */
    CharPtr db_file;     /* Name for input data file - 'IN' name */
    Int4 is_protein;     /* Is this protein database ? */
    Int4 parse_mode;     /* Do we assume, that deflines are started from 
                             valid SeqIds ? */
    Int4 isASN;          /* read from file or ASN - used only in formatdb.c */
    Int4 asnbin;         /* What is this type of ASN? used only 
                            in formatdb.c */
    Int4 is_seqentry;    /* What is this type of ASN? used only 
                             in formatdb.c */
    CharPtr base_name;   /* Name for db files to be created 'OUT' name */
    CharPtr	alias_file_name; /* name to be used for BLAST alias-file. */
    Int4  dump_info;     /* To printout file with information about tax_id,
                             owner, hash etc. - used for dump from ID */
                             
    Int4  sparse_idx;    /* To use only limited set of text ids to dump for
                             usage in indexes */
    Int4  test_non_unique;    /* Print messages if FASTA database has
                                 non-unique string ids - accessions, locuses*/
    
    RDBTaxLookupPtr tax_lookup; /* taxonomy lookup table - should be initialized in the main program to be used for creating of taxonomy information*/

    TaxCallbackFunc tax_callback; /* Function to retrieve taxonomy names from
                                     Taxonomy server */
   Int8 bases_in_volume;  /* The maximal number of bases that can be stored in
                             one volume of the database */
   Int4 sequences_in_volume; /* Maximum number of sequences to be stored in a
                                volume */
   Int2 volume;      /* Largest volume */
   Int4 total_num_of_seqs; /* total number of sequences for this database */
   CharPtr	gi_file;        /* Gi file to be used in processing. */ 
   CharPtr  gi_file_bin;	/* Gi file to be used in processing. */

   ValNodePtr    linkbit_listp; /* list of gis and the bits to set */
   ValNodePtr    memb_tblp;     /* Linked list of MembInfo structures */
   VoidPtr       memb_argp;     /* Argument to criteria function in MembInfo
                                   structure */
   EFDBCleanOpt clean_opt;      /* clean up option */

} FDB_options, PNTR FDB_optionsPtr;
    
typedef struct formatdb 
{
    /* CharPtr	dbname;	(db_file)  name of input database */
    /* CharPtr	DbTitle; (db_title) database title */
    
    /* file handlers */

    FILE *fd,
        *fd_ind,
        *fd_seq,
        *fd_def,
        *fd_sdi,  /* This is file for misc. info data */
        *fd_stmp;
    
    /* ASN.1 input, if the "-a" specified */
    AsnIoPtr	aip;

    /* ASN.1 defline output if structured defline */
    AsnIoPtr aip_def;
    
    Int4 num_of_seqs;  /* number of parsed sequences in this volume */
    Int8 TotalLen;
    Int4 MaxSeqLen;
    
    /* offset tables */
    Int4Ptr	DefOffsetTable,	/* definitions */
        	SeqOffsetTable,	/* sequences */
        	AmbOffsetTable;	/* ambiguities */

    /* lookup table */

    FASTALookupPtr	lookup;

    /* Table to map PIGs to ordinal ids */
    FDBPigTablePtr   ptable;   

    /* General formatdb options */

    FDB_optionsPtr options;

    Uint4Ptr	AmbCharPtr;	/* ambiguity characters while
                                 * convert from ncbi2na->ncbi4na */

    Int4 OffsetAllocated; /* storage for allocation size */

    
} FormatDB, *FormatDBPtr;


#define	MASK_WORD_SIZE	32

/* Function prototypes for formatdb library*/

/* --------------------- FDBOptionsNew ----------------------------
   Purpose: Creates formatdb options structure with parameters from
            the argument list.
   Returns: Pointer to initialized structure.
   Notes: If alias_file_name is provided, the function FDB_MakeAlias
          should be called after FDBClose. (FIXME)
   ---------------------------------------------------------------- */
FDB_optionsPtr FDBOptionsNew(
        CharPtr input, /* [in] name of input file */
        Boolean is_prot, /* [in] input contains protein sequences? */
        CharPtr title,  /* [in] title to give this database */
        Boolean is_asn, /* [in] true if input is in ASN.1 */
        Boolean is_asn_bin, /* [in] true if ASN.1 input is binary */
        Boolean is_seqentry, /* [in] true of input is a seqentry */
        Boolean sparse_idx, /* [in] should sparce ISAM indices be used? */
        Boolean test_non_unique, /* [in] test for repeated string identifiers
                                    in database */
        Boolean parse_deflines, /* [in] input contains parseable deflines? */
        CharPtr basename, /* [in] name for the database to create */
        CharPtr alias_file_name, /* [in] name for the alias file to create */
        Int8 bases_per_volume, /* [in] max num of residues/bases per volume */
        Int4 seqs_per_volume, /* [in] max num of sequences per volume */
        Int4 version, /* [in] database version */
        Boolean dump_info, /* [in] should basename.[pn]di be created? */
        EFDBCleanOpt clean_opt);/* [in] should basename.* files be removed ? */

/* --------------------- FDBOptionsFree ---------------------------
   Purpose: Frees the memory allocated for the formatdb options structure.
   Returns: NULL
   ---------------------------------------------------------------- */
FDB_optionsPtr FDBOptionsFree(FDB_optionsPtr options);

/* The next 4 functions are for production database dump ({id,rs}dump_blast) */
ValNodePtr FDBLoadLinksTable(void);
ValNodePtr FDBDestroyLinksTable(ValNodePtr list);
ValNodePtr FDBLoadMembershipsTable(void);
ValNodePtr FDBDestroyMembershipsTable(ValNodePtr tbl);

/* Constructs BlastDefLine structures from Bioseq */
BlastDefLinePtr FDBGetDefAsnFromBioseq(BioseqPtr bsp);

FormatDBPtr	FormatDBInit(FDB_optionsPtr options);

/* For database version FORMATDB_VER (or greater), only the first 5 parameters
 * are used, the latter are kept for the FORMATDB_VER_TEXT version of the BLAST
 * databases. Please note that the seq_data and seq_data_type will be changed
 * if the data passed in doesn't match the format that is required for the
 * BLAST database format (ncbistdaa for proteins, ncbi2na for nucleotides) */
Int2 FDBAddSequence (FormatDBPtr fdbp,  BlastDefLinePtr bdp, 
                     Uint1* seq_data_type, ByteStorePtr *seq_data, 
                     Int4 SequenceLen, 
                     CharPtr seq_id, CharPtr title, 
                     Int4 gi, Int4 tax_id, CharPtr div, Int4 owner, Int4 date);

/**
 * FDBAddSequence2: is an interface to add "non-redundant sequence", i.e
 * common sequence data and multiple sequence information block (1 per gi)
 * This function will NOT alter the seq_data field, it assumes that the data is
 * already provided in the required format
 * @param fdbp target blast db [in]
 * @param srp linked list of sequence information for each gi [in]
 * @param seq_data_type type of the parameter below [in]
 * @param seq_data sequence data itself [in]
 * @param SequenceLen length of the sequence in seq_data [in]
 * @param AmbCharPtr pointer to ambiguity sequence data (nucl only) [in]
 * @param pig_id stable protein group identifier [in]
 * @param hash sequence hash - to allow  resuse of hahs calculated in ID [in]
 * @return 1 on failure, 0 on success
 */
Int2 FDBAddSequence2 (FormatDBPtr  fdbp,
                      SI_RecordPtr srp,
                      Uint1 seq_data_type,
                      const ByteStorePtr *seq_data,
                      Int4 SequenceLen,
                      Uint4Ptr  AmbCharPtr,
                      Int4 pig_id, 
                      Uint4 hash   
                      );

/* For database version FORMATDB_VER (or greater), the bdp parameter must 
 * be provided. This could be populated from the bsp parameter by calling 
 * FDBGetDefAsnFromBioseq */
Int2 FDBAddBioseq(FormatDBPtr fdbp, BioseqPtr bsp, BlastDefLinePtr bdp);
Int2 FormatDBClose(FormatDBPtr fdbp);


Boolean FDBAddLinksInformation(BlastDefLinePtr bdp, ValNodePtr links_tblp);
Boolean FDBAddMembershipInformation(BlastDefLinePtr bdp, ValNodePtr memb_tblp, 
                                    VoidPtr criteria_arg);

Int2 process_sep (SeqEntryPtr sep, FormatDBPtr fdbp);

NLM_EXTERN Boolean SeqEntrysToBLAST (SeqEntryPtr sep, FormatDBPtr fdbp,
                                     Boolean is_na, Uint1 group_segs);

NLM_EXTERN Boolean BLASTFileFunc (BioseqPtr bsp, Int2 key, CharPtr buf,
                                  Uint4 buflen, Pointer data);

/*
Print a summary of the database used.
*/
Boolean LIBCALL PrintDbInformation PROTO((CharPtr database, Boolean is_aa, Int4 line_length, FILE *outfp, Boolean html));
Boolean LIBCALL PrintDbInformationWithRID PROTO((CharPtr database, Boolean is_aa, Int4 line_length, FILE *outfp, Boolean html, CharPtr rid));

Boolean LIBCALL PrintDbInformationBasic PROTO((CharPtr database, Boolean is_aa, Int4 line_length, CharPtr definition, Int4 number_seqs, Int8 total_length, FILE *outfp, Boolean html));

Boolean FDBAddSeqEntry(FormatDBPtr fdbp, SeqEntryPtr sep);

/* ID1 dump stuff */

typedef	struct di_record {
    Int4    oid;
    Int4    gi;
    Int4    taxid;
    Int4    owner;
    Char    div[4];
    Int4    len;
    Int4    hash;
    Int4    date;
    CharPtr acc; /* accession should not exceed this size */
	Int4	gi_threshold;   /* for 'month' subset */
} DI_Record, *DI_RecordPtr;

/******** genmask structures and functions *********/

/* genmask scans the *.[pn]di files and sets membership bits according to the
   criteria specified by the GMCriteria function (see typedef above). This is
   one example of how to set the membership bits in the new database format.
   Note that the MembInfo structure has a criteria function pointer that
   returns a boolean value and takes a void ptr as an argument to allow
   flexibility in specifying the criteria to belong to a particular
   membership. */

typedef struct {
    Int4           count, allocated;
    CharPtr        *subset_name;
    GMCriteriaFunc *criteria;
    Int4           *membership_bit;
} GMSubsetData, * GMSubsetDataPtr;

Boolean	ScanDIFile(CharPtr difilename, GMSubsetDataPtr gmsubsetdp,
	Boolean(*callback)(DI_RecordPtr direc, VoidPtr data), VoidPtr data,
	FILE *out, Int4 gi_threshold);

CharPtr FDFGetAccessionFromSeqIdChain(SeqIdPtr seqid_list);

/* These functions determine the criteria for the membership bits for genmask.
   Only protein sequences have memberships because they are in non-redundant
   databases */
Boolean is_EST_HUMAN(VoidPtr di_record);
Boolean is_EST_MOUSE(VoidPtr di_record);
Boolean is_EST_OTHERS(VoidPtr di_record);
Boolean is_SWISSPROT(VoidPtr di_record);
Boolean is_MONTH(VoidPtr di_record);
Boolean is_PDB(VoidPtr di_record);
Boolean is_REFSEQ_GENOMIC(VoidPtr di_record);
Boolean is_REFSEQ_RNA(VoidPtr di_record);
Boolean is_REFSEQ_PROTEIN(VoidPtr di_record);
Boolean is_CONTIG(VoidPtr di_record);
Boolean is_WGS_ANOPHELES(VoidPtr di_record);
Boolean is_WGS_RICE(VoidPtr di_record);
Boolean is_WGS_MOUSE(VoidPtr di_record);
Boolean is_WGS_ANTHRAX(VoidPtr di_record);

#if 0
/* The common index is deprecated  - camacho 09/02/2002 */
typedef	struct updateindex_struct {
    FILE	*cifile;/* CommonIndex file */
    Int2	shift;	/* database shift in mask */
    FILE	*fout;	/* output stream */
} UpdateIndexStruct, *UpdateIndexStructPtr;


Int4	UpdateCommonIndexFile (CharPtr dbfilename, Boolean proteins,
		FILE *fout, CharPtr difile, Int4 gi_threshold);
#endif

/************************************************************************/
/*        Fastacmd API                                           */
/************************************************************************/

typedef struct FCMDAccList {
    CharPtr acc;
    Int4 gi;
    struct FCMDAccList *next;
} FCMDAccList, PNTR FCMDAccListPtr;

FCMDAccListPtr LIBCALL GetAccList(CharPtr file, Int4Ptr TotalItems);
void LIBCALL FCMDAccListFree(FCMDAccListPtr falp);

#define FASTACMD_DEFAULT_DB "nr"

#define FASTACMD_SUCCESS 0
#define FASTACMD_ERROR 1
#define FASTACMD_DB_NOT_FOUND 2
#define FASTACMD_FAILED_SEARCH 3
#define FASTACMD_NO_TAXDB 4

/* Fastacmd_Search and Fastacmd_Search_ex return non-zero on failure */
Int2 Fastacmd_Search (CharPtr searchstr, CharPtr database,
	CharPtr batchfile, Boolean dupl, Int4 linelen, FILE *out);

/* Used to specify which kind of data to dump using fastacmd */
typedef enum EBlastDbDumpType {
    eNoDump = 0,        /* Don't dump any data from the database, the default for
                           fastacmd */
    eFasta,             /* dump contents of database as FASTA */
    eGi,                /* List of gis in the database */
    eDumpTypeMax        /* not really a dump type, needed for error checking */
} EBlastDbDumpType;

Int2 Fastacmd_Search_ex (CharPtr searchstr, CharPtr database, Uint1 is_prot,
	CharPtr batchfile, Boolean dupl, Int4 linelen, FILE *out, 
	Boolean use_target, Boolean use_ctrlAs, EBlastDbDumpType dump_db, 
    CharPtr seqlocstr, Uint1 strand, Boolean taxonomy_info_only, 
    Boolean dbinfo_only, Int4 pig);

/**
 * @param rdfp Blast database handle [in]
 * @param fp output FILE pointer [in]
 * @param linelen number of characters to print per line [in]
 * @param use_ctrlAs use Ctrl-A to separate non-redundant deflines? [in]
 * @param dump_type type of information to dump [in]
 */
Int2 DumpBlastDB(const ReadDBFILEPtr rdfp, FILE *fp, Int4 line_length, 
		         Boolean use_ctrlAs, EBlastDbDumpType dump_type);

Int4 LIBCALL readdb_MakeGiFileBinary PROTO((CharPtr input_file, CharPtr
					    output_file));

Int4 FastaToBlastDB PROTO((FDB_optionsPtr options, Int4 Bases_In_Volume));

BlastDefLinePtr FDReadDeflineAsn(ReadDBFILEPtr rdfp, Int4 sequence_number);

CharPtr FD_ConstructMultivolumeDBList(CharPtr basename, Int4 vols);

Boolean FD_CreateAliasFileEx PROTO((CharPtr title, CharPtr basename, 
            Int4 volumes, Boolean is_protein, CharPtr parent,
            Int4 first_oid, Int4 last_oid, Int8 total_length, Int4 number_seqs,
	    CharPtr oidlist, CharPtr gifile));

Boolean FD_CreateAliasFile PROTO((CharPtr title, CharPtr basename, 
                                    Int4 volumes, Boolean is_protein));

/* simple function to make alias file give FDB_optionsPtr, alias file is only made if appropriate. */
Boolean FD_MakeAliasFile PROTO((FDB_optionsPtr options));
Int4 LIBCALL 
readdb_get_sequence_number PROTO((ReadDBFILEPtr rdfp, Int4 first_seq, Int8 offset));

#if defined(OS_UNIX_SOL) || defined(OS_UNIX_LINUX)
#ifdef HAVE_MADVISE

/* enable/disable madvise functionality, -- disabled by default */
void LIBCALL
readdb_madvise_enable PROTO((Boolean enable));

/* set madvise type, -- default eMMA_Normal */
void LIBCALL
readdb_madvise_type PROTO((EMemMapAdvise advice));

/* explicitly set madvise sync mode:
 * default is sync on Solaris, async on Linux 
 */
void LIBCALL
readdb_madvise_sync_mode PROTO((Boolean mode));

/* explicitly set madvise block size, which is the
 * number of sequences preloaded in a single madvise
 * operation, default is 65536
 */
void LIBCALL
readdb_madvise_block PROTO((Int4 nSeqs));

/* call preload directly -- run madvise on a chunk of memory mapped file */
void LIBCALL
readdb_preload PROTO((ReadDBFILEPtr rdfp, Int4 first_db_seq,
				Int4 final_db_seq, EMemMapAdvise advice, Boolean sync));

#endif /* HAVE_MADVISE */
#endif /* SOL || LINUX */

#ifdef __cplusplus
}
#endif

#endif /* _READDB_ */
