static char const rcsid[] = "$Id: blastpgp.c,v 6.127 2005/04/04 14:57:47 papadopo Exp $";

/* $Id: blastpgp.c,v 6.127 2005/04/04 14:57:47 papadopo Exp $ */
/**************************************************************************
*                                                                         *
*                             COPYRIGHT NOTICE                            *
*                                                                         *
* This software/database is categorized as "United States Government      *
* Work" under the terms of the United States Copyright Act.  It was       *
* produced as part of the author's official duties as a Government        *
* employee and thus can not be copyrighted.  This software/database is    *
* freely available to the public for use without a copyright notice.      *
* Restrictions can not be placed on its present or future use.            *
*                                                                         *
* Although all reasonable efforts have been taken to ensure the accuracy  *
* and reliability of the software and data, the National Library of       *
* Medicine (NLM) and the U.S. Government do not and can not warrant the   *
* performance or results that may be obtained by using this software,     *
* data, or derivative works thereof.  The NLM and the U.S. Government     *
* disclaim any and all warranties, expressed or implied, as to the        *
* performance, merchantability or fitness for any particular purpose or   *
* use.                                                                    *
*                                                                         *
* In any work or product derived from this material, proper attribution   *
* of the author(s) as the source of the software or data would be         *
* appreciated.                                                            *
*                                                                         *
**************************************************************************
 * $Revision: 6.127 $ 
 * $Log: blastpgp.c,v $
 * Revision 6.127  2005/04/04 14:57:47  papadopo
 * remove requirement for a fasta format query file if restarting from scoremat
 *
 * Revision 6.126  2004/10/12 15:14:39  papadopo
 * add gap open and extend penalties to [BC]posComputation calls
 *
 * Revision 6.125  2004/07/19 17:16:19  papadopo
 * add capability to perform input and output of residue frequencies in scoremat form
 *
 * Revision 6.124  2004/06/30 12:33:30  madden
 * Add include for blfmtutl.h
 *
 * Revision 6.123  2004/06/25 20:58:49  dondosha
 * Ungapped option not supported for multi-iterational search, but OK otherwise
 *
 * Revision 6.122  2004/06/24 21:48:16  dondosha
 * Made ungapped search option deprecated
 *
 * Revision 6.121  2004/06/24 19:02:23  dondosha
 * Exit with error status if PGPReadBlastOptions returns NULL
 *
 * Revision 6.120  2004/04/29 19:56:00  dondosha
 * Mask filtered locations in query sequence lines in XML output
 *
 * Revision 6.119  2004/03/31 17:59:32  dondosha
 * Fix for XML output for multiple queries: append full XML outputs in one file
 *
 * Revision 6.118  2003/09/26 16:01:34  madden
 * Change threshold to 0.002
 *
 * Revision 6.117  2003/05/30 17:31:09  coulouri
 * add rcsid
 *
 * Revision 6.116  2003/05/13 16:02:42  coulouri
 * make ErrPostEx(SEV_FATAL, ...) exit with nonzero status
 *
 * Revision 6.115  2003/03/20 14:47:16  madden
 * StringSave on asn1_mode
 *
 * Revision 6.114  2003/03/20 13:44:23  madden
 * Fix -m 10/11 output to make them SeqAnnots
 *
 * Revision 6.113  2002/12/31 22:47:16  boemker
 * Added support for printing output as ASN (text, with -m 10, or binary, with
 * -m 11).
 *
 * Revision 6.112  2002/11/12 16:04:24  dondosha
 * Do not print the no hits found message for tabular output
 *
 * Revision 6.111  2002/09/23 21:00:00  madden
 * Fix to correctly output SeqAlign for every query and iteration
 *
 * Revision 6.110  2002/09/19 13:25:00  camacho
 * Fix incorrect index into myargs array
 *
 * Revision 6.109  2002/09/18 20:34:30  camacho
 * Restored -P option
 *
 * Revision 6.108  2002/08/09 19:41:25  camacho
 * 1) Added blast version number to command-line options
 * 2) Added explanations for some default parameters
 *
 * Revision 6.107  2002/06/19 22:50:17  dondosha
 * Added all queries information for tabular output with multiple queries
 *
 * Revision 6.106  2002/04/29 19:55:25  madden
 * Use ARG_FLOAT for db length
 *
 * Revision 6.105  2002/03/13 22:48:06  dondosha
 * Avoid freeing masking locations after each iteration with XML output
 *
 * Revision 6.104  2001/10/12 14:55:41  dondosha
 * Changed description of the -t option
 *
 * Revision 6.103  2001/08/29 19:06:37  madden
 * added variable posComputationcalled in Main, added parameter posComputationCalled to PGPrintPosMatrix
 *
 * Revision 6.102  2001/08/28 17:34:35  madden
 * Add -m 9 as tabular output with comments
 *
 * Revision 6.101  2001/07/30 16:27:42  dondosha
 * Do not call PGPOutTextMessages with XML output option
 *
 * Revision 6.100  2001/07/25 19:40:15  dondosha
 * Multiply hitlist_size by 2 when going to next query if when tweak_parameters set
 *
 * Revision 6.99  2001/07/24 18:16:55  madden
 * Set error_return when freeing
 *
 * Revision 6.98  2001/07/09 19:37:31  kans
 * return 0 instead of NULL to fix Mac compiler error
 *
 * Revision 6.97  2001/07/03 20:50:33  madden
 * Commented out call to PrintTabularOutputHeader
 *
 * Revision 6.96  2001/06/22 13:48:39  dondosha
 * Declare variable vnp
 *
 * Revision 6.95  2001/06/21 21:56:37  dondosha
 * Fixed memory leak: destroy all error returns; uncommented ObjMgrFreeCache
 *
 * Revision 6.94  2001/06/15 21:19:28  dondosha
 * Added tabular output option -m 8
 *
 * Revision 6.93  2001/06/11 20:31:19  shavirin
 * Added possibility to make Batch searches in iteractive mode.
 *
 * Revision 6.92  2001/04/13 20:46:01  madden
 * Changed default e-value threshold for inclusion in multipass model from 0.002 to 0.005, Changed default constant in pseudocounts for multipass version from 7 to 9
 *
 * Revision 6.91  2001/04/10 19:20:52  madden
 * Unsuppress some options suppressed for the release
 *
 * Revision 6.90  2001/03/30 22:02:04  madden
 * Suppress options not yet ready for prime-time
 *
 * Revision 6.89  2000/11/30 17:10:13  shavirin
 * Initialized to NULL xml pointer.
 *
 * Revision 6.88  2000/11/28 20:48:49  shavirin
 * Adopted for usage with many-iterations XML.
 *
 * Revision 6.87  2000/11/22 21:57:08  shavirin
 * Added passing of iteration number into XML output.
 *
 * Revision 6.86  2000/11/22 21:41:52  shavirin
 * Removed problem with mutiple iteration XML output.
 *
 * Revision 6.85  2000/11/22 17:34:32  madden
 * Change NULL to 0 for an intvalue
 *
 * Revision 6.84  2000/11/22 17:28:31  madden
 * Enable decline-to-align
 *
 * Revision 6.83  2000/10/27 21:26:50  shavirin
 * Produce valid XML output if no hits found.
 *
 * Revision 6.82  2000/10/27 19:14:41  madden
 * Change description of -b option
 *
 * Revision 6.81  2000/10/24 13:51:33  shavirin
 * Removed unused parameter from the function BXMLPrintOutput().
 *
 * Revision 6.80  2000/10/23 19:58:22  dondosha
 * Open and close AsnIo outside of call(s) to BXMLPrintOutput
 *
 * Revision 6.79  2000/10/17 19:39:20  shavirin
 * Fixed compilation problems on Mac.
 *
 * Revision 6.78  2000/10/13 20:58:01  madden
 * Add YES_TO_DECLINE_TO_ALIGN define and disable same
 *
 * Revision 6.77  2000/09/28 15:48:30  dondosha
 * Open <PRE> block in PGPFormatFooter
 *
 * Revision 6.76  2000/09/27 19:31:44  dondosha
 * Use original square substitution matrix to pass to txalign on all iterations
 *
 * Revision 6.75  2000/09/21 17:54:46  dondosha
 * Do not pass matrix to txalign in case of query-anchored formatting
 *
 * Revision 6.74  2000/09/13 18:34:35  dondosha
 * Create BLAST_Matrix from ScoreBlk before converting it to txalign-style matrix
 *
 * Revision 6.73  2000/09/12 21:51:38  dondosha
 * Pass the correct scoring matrix to ShowTextAlignFromAnnot
 *
 * Revision 6.72  2000/08/28 21:55:01  shavirin
 * Added option (m = 7) to print XML output.
 *
 * Revision 6.71  2000/08/22 20:18:21  shavirin
 * Added support for HTML output if decline-to-align parameter is set.
 *
 * Revision 6.70  2000/08/18 21:31:12  madden
 * no longer need to raise E-value threshold for composition-based statistics
 *
 * Revision 6.69  2000/08/08 21:47:55  shavirin
 * Added parameter decline_align and possibility to print then
 * discontinuous alignment.
 *
 * Revision 6.68  2000/07/26 19:04:14  shavirin
 * Set overriding default threshold only if input value != 0.
 *
 * Revision 6.67  2000/07/25 18:14:06  shavirin
 * WARNING: This is no-turning-back changed related to S&W Blast from
 * Alejandro Schaffer
 *
 * Revision 6.66  2000/07/21 20:46:40  madden
 * Threshold set to default if arg is zero
 *
 * Revision 6.65  2000/06/27 15:25:19  madden
 * Changed master-slave to query-anchored
 *
 * Revision 6.64  2000/03/29 17:32:55  madden
 * Commented out yet another call to ObjMgrFreeCache
 *
 * Revision 6.63  2000/03/27 21:32:00  shavirin
 * Use function ShowTextAlignFromAnnot2 instead of ShowTextAlignFromAnnot3
 *
 * Revision 6.62  2000/03/23 15:30:22  shavirin
 * Removed call to ObjMgrFreeCache()
 *
 * Revision 6.61  2000/03/07 21:59:07  shavirin
 * Now will use PSSM Matrix to show positives in PSI Blast
 *
 * Revision 6.60  2000/03/02 21:06:09  shavirin
 * Added -U option, that allows to consider low characters in FASTA files
 * as filtered regions (for blastn, blastp and tblastn).
 *
 * Revision 6.58  2000/01/07 22:01:04  shavirin
 * Fixed problem with printing alignment.
 *
 * Revision 6.57  1999/12/21 21:34:05  shavirin
 * Fixed some memory leaks.
 *
 * Revision 6.56  1999/11/08 19:12:41  shavirin
 * Fixed minor SGI warning.
 *
 * Revision 6.55  1999/10/21 20:27:52  shavirin
 * Fixed bug resulted in failue to print out seqannot. (-O)
 *
 * Revision 6.53  1999/10/14 15:52:51  shavirin
 * Added possibility to make search by list of gis. Fixed some bugs.
 *
 * Revision 6.52  1999/10/05 17:41:08  shavirin
 * Corrected in accordance to blast.c changes.
 *
 * Revision 6.51  1999/09/24 16:06:15  shavirin
 * Matrix is set to NULL if no matrix calculation produced.
 *
 * Revision 6.50  1999/09/22 17:53:25  shavirin
 * Now functions will collect messages in ValNodePtr before printing out.
 *
 * Revision 6.49  1999/09/21 16:01:46  shavirin
 * Rearanged all file. Main function was disintegrated into few small
 * functions.
 *
 * Revision 6.48  1999/08/27 18:17:42  shavirin
 * Added new parameter to command line - decline_align.
 *
 * Revision 6.47  1999/08/26 14:58:06  madden
 * Use float for db length
 *
 * Revision 6.46  1999/08/04 13:26:49  madden
 * Added -B option
 *
 * Revision 6.45  1999/03/31 16:58:04  madden
 * Removed static FindProt and FindNuc
 *
 * Revision 6.44  1999/03/24 18:16:33  madden
 * zero-out groupOrder and featureOrder
 *
 * Revision 6.43  1999/03/21 19:43:23  madden
 * Added -Q option to store ASCII version of last position-specific matrix in a file
 *
 * Revision 6.42  1999/03/04 14:17:20  egorov
 * Added new parameter to BlastMaskTheResidues() function for correct masking
 * when query is seqloc.  The paramter is not used in this file and is 0.
 *
 * Revision 6.41  1999/02/18 21:13:11  madden
 * Check for non-pattern search before call to BlastPruneSapStructDestruct
 *
 * Revision 6.40  1999/02/10 21:12:27  madden
 * Added HTML and GI list option, fixed filtering
 *
 * Revision 6.39  1999/01/22 17:24:51  madden
 * added line breaks for alignment views
 *
 * Revision 6.38  1998/12/29 20:03:14  kans
 * calls UseLocalAsnloadDataAndErrMsg at startup
 *
 * Revision 6.37  1998/12/17 21:54:39  madden
 * Added call to BlastPruneHitsFromSeqAlign for other than first round
 *
 * Revision 6.36  1998/12/16 14:13:36  madden
 * Fix for more than one pattern in query
 *
 * Revision 6.35  1998/11/19 14:04:34  madden
 * Changed message level to SEV_WARNING
 *
 * Revision 6.34  1998/11/16 16:29:41  madden
 * Added ErrSetMessageLevel(SEV_INFO) and fixed call to PrintKAParametersExtra
 *
 * Revision 6.33  1998/09/28 12:33:02  madden
 * Used BlastErrorPrint
 *
 * Revision 6.31  1998/09/17 19:54:31  madden
 * Removed fillCandLambda
 *
 * Revision 6.29  1998/09/10 22:38:24  madden
 * Moved convertSeqAlignListToValNodeList and convertValNodeListToSeqAlignList
 *
 * Revision 6.28  1998/09/09 21:20:02  madden
 * AS fixed warnings, removed stderr fprintf, added call to PrintKAParametersExtra
 *
 * Revision 6.27  1998/09/09 16:10:49  madden
 * PHI-BLAST changes
 *
 * Revision 6.26  1998/07/17 15:41:36  madden
 * Added effective search space flag
 *
 * Revision 6.24  1998/06/14 19:44:46  madden
 * Checkpointing fix
 *
 * Revision 6.23  1998/06/12 21:32:27  madden
 * Removed extra FnPtr cast
 *
 * Revision 6.22  1998/06/10 13:33:39  madden
 * change -h from 0.01 to 0.001
 *
 * Revision 6.21  1998/06/05 21:48:43  madden
 * Added -K and -L options
 *
 * Revision 6.20  1998/05/18 18:01:05  madden
 * Changed args to allow filter options to be changed
 *
 * Revision 6.19  1998/05/01 18:31:03  egorov
 * Add new parametes to BLASTOptionSetGapParam()
 *
 * Revision 6.18  1998/04/30 14:32:33  madden
 * init_buff_ex arg changed to 90 for reference
 *
 * Revision 6.17  1998/04/29 14:29:31  madden
 * Made reference line longer
 *
 * Revision 6.16  1998/04/01 22:49:13  madden
 * Print No hits found message
 *
 * Revision 6.15  1998/02/25 20:50:50  madden
 * Added arg for db length
 *
 * Revision 6.14  1998/02/24 22:48:36  madden
 * Removed options for culling
 *
 * Revision 6.13  1998/01/02 14:33:37  madden
 * Added comma
 *
 * Revision 6.12  1997/12/31 17:48:56  madden
 * Added wordsize option
 *
 * Revision 6.11  1997/12/23 21:09:44  madden
 * Added -K and -L for range-dependent blast
 *
 * Revision 6.10  1997/12/23 20:57:44  madden
 * Changes for checkpointing
 *
 * Revision 6.9  1997/11/19 14:26:46  madden
 * Removed extra break statement
 *
 * Revision 6.8  1997/11/18 22:24:24  madden
 * Added call to BLASTOptionSetGapParams
 *
 * Revision 6.7  1997/11/08 22:04:43  madden
 * Called BlastOtherReturnsPrepare earlier to before posMatrix is deleted
 *
 * Revision 6.6  1997/10/27 22:26:49  madden
 * Added call to ObjMgrFreeCache(0)
 *
 * Revision 6.5  1997/10/23 20:26:15  madden
 * Use of init_buff_ex rather than init_buff
 *
 * Revision 6.4  1997/10/22 21:56:49  madden
 * Changed default values
 *
 * Revision 6.3  1997/10/07 21:33:34  madden
 * Added BLUNT option
 *
 * Revision 6.2  1997/09/18 22:25:02  madden
 * b and v options now work
 *
 * Revision 6.1  1997/09/16 16:40:50  madden
 * Dbinfo printing changed for multiple db searches
 *
 * Revision 6.0  1997/08/25 18:19:19  madden
 * Revision changed to 6.0
 *
 * Revision 1.24  1997/08/24 19:38:23  madden
 * Used function BlastOtherReturnsPrepare
 *
 * Revision 1.23  1997/08/14 21:48:57  madden
 * Added descriptions and alignments options
 *
 * Revision 1.22  1997/07/29 19:33:05  madden
 * Added TXALIGN_SHOW_QS flag
 *
 * Revision 1.21  1997/07/28 18:36:45  madden
 * Replaced printf with ErrPostEx and fprintf
 *
 * Revision 1.20  1997/07/28 16:59:21  madden
 * Added include for simutil.h
 *
 * Revision 1.19  1997/07/28 16:50:51  madden
 * Changed call to ShowTextAlignFromAnnot
 *
 * Revision 1.18  1997/07/22 19:18:40  madden
 * printing version, etc.
 *
 * Revision 1.17  1997/06/25 14:06:21  madden
 * minor changes to check convergence
 *
 * Revision 1.16  1997/06/23 20:51:29  madden
 * Added matrix option
 *
 * Revision 1.15  1997/06/20 19:30:00  madden
 * Added align_type info, support for SeqAligns
 *
 * Revision 1.14  1997/05/23 20:54:48  madden
 * Added -Z option for final gapped alignment
 *
 * Revision 1.13  1997/05/07 15:06:01  madden
 * replacement of search by compactSearch
 *
 * Revision 1.12  1997/04/21  14:09:27  madden
 * Added four new master-slave alignment types.
 *
 * Revision 1.11  1997/04/11  19:03:47  madden
 * Changes to ignore query ID and show master-slave alignments.
 *
 * Revision 1.10  1997/04/10  19:28:28  madden
 * COMMAND_LINE replaced by ALL_ROUNDS.
 *
 * Revision 1.9  1997/04/10  13:27:32  madden
 * Added COMMAND_LINE define, option for multiple alignments.
 *
 * Revision 1.8  1997/04/07  21:44:50  madden
 * Removed unused variable stats.
 *
 * Revision 1.7  1997/04/04  21:13:33  madden
 * Used function BioseqBlastEngineCore, remove PrivateScoreFunc.
 *
 * Revision 1.6  1997/04/04  16:38:04  madden
 * Changed filtering option, ObjMgrHold.
 *
 * Revision 1.5  1997/03/21  20:30:02  madden
 * Expect value arg made a float.
 *
 * Revision 1.4  1997/03/13  21:18:51  madden
 * Changed default extension value to 1 from 2.
 *
 * Revision 1.3  1997/02/19  21:43:04  madden
 * Extensive changes for psi-blast.  blastp runs now occur multiple times.
 *
 * Revision 1.2  1997/02/12  15:16:58  madden
 * Changed from blast2 to new formatting.
 *
 * Revision 1.1  1997/01/16  21:35:42  madden
 * Initial revision
 *
 * Revision 1.1  1997/01/16  21:34:23  madden
 * Initial revision
 *
*/
#include <ncbi.h>
#include <objseq.h>
#include <objsset.h>
#include <sequtil.h>
#include <seqport.h>
#include <tofasta.h>
#include <blast.h>
#include <blastpri.h>
#include <txalign.h>
#include <simutil.h>
#include <gapxdrop.h>
#include <posit.h>
#include <seed.h>
#include <sqnutils.h>
#include <xmlblast.h>
#include <ddvcreate.h>
#include <blfmtutl.h>
#include <objscoremat.h>

/* Used by the callback function. */
FILE *global_fp=NULL;
/*
  Callback to print out ticks, in UNIX only due to file systems
  portability issues.
  */

static int LIBCALLBACK
tick_callback(Int4 sequence_number, Int4 number_of_positive_hits)
{
    
#ifdef OS_UNIX
    
    fprintf(global_fp, "%s", ".");
    fflush(global_fp);
#endif
    return 0;
}

static int LIBCALLBACK
star_callback(Int4 sequence_number, Int4 number_of_positive_hits)
{
#ifdef OS_UNIX
    fprintf(global_fp, "%s", "*");
    fflush(global_fp);
#endif
    return 0;
}

#define YES_TO_DECLINE_TO_ALIGN

#define NUMARG (sizeof(myargs)/sizeof(myargs[0]))

static Args myargs[] = {
    { "Database",               /* 0 */
      "nr", NULL, NULL, FALSE, 'd', ARG_STRING, 0.0, 0, NULL},
    { "Query File (not needed if restarting from scoremat)", /* 1 */
      "stdin", NULL, NULL, TRUE, 'i', ARG_FILE_IN, 0.0, 0, NULL},
    { "Multiple Hits window size", /* 2 */
      "40", NULL, NULL, FALSE, 'A', ARG_INT, 0.0, 0, NULL},
    { "Threshold for extending hits", /* 3 */
      "11", NULL, NULL, FALSE, 'f', ARG_INT, 0.0, 0, NULL},
    { "Expectation value (E)",  /* 4 */
      "10.0", NULL, NULL, FALSE, 'e', ARG_FLOAT, 0.0, 0, NULL},
    { "alignment view options:\n0 = pairwise,\n1 = query-anchored showing identities,\n2 = query-anchored no identities,\n3 = flat query-anchored, show identities,\n4 = flat query-anchored, no identities,\n5 = query-anchored no identities and blunt ends,\n6 = flat query-anchored, no identities and blunt ends,\n7 = XML Blast output,\n8 = Tabular output, \n9 = Tabular output with comments\n10 = ASN, text\n11 = ASN, binary", /* 5 */
      "0", NULL, NULL, FALSE, 'm', ARG_INT, 0.0, 0, NULL},
    { "Output File for Alignment", /* 6 */
      "stdout", NULL, NULL, TRUE, 'o', ARG_FILE_OUT, 0.0, 0, NULL},
    { "Dropoff (X) for blast extensions in bits (default if zero)", /* 7 */
      "7.0", NULL, NULL, FALSE, 'y', ARG_FLOAT, 0.0, 0, NULL},
    { "0 for multiple hit, 1 for single hit",/* 8 */
       "0",  NULL, NULL, FALSE, 'P', ARG_INT, 0.0, 0, NULL},
    { "Filter query sequence with SEG", /* 9 */
      "F", NULL, NULL, FALSE, 'F', ARG_STRING, 0.0, 0, NULL},
    { "Cost to open a gap",     /* 10 */
      "11", NULL, NULL, FALSE, 'G', ARG_INT, 0.0, 0, NULL},
    { "Cost to extend a gap",   /* 11 */
      "1", NULL, NULL, FALSE, 'E', ARG_INT, 0.0, 0, NULL},
    { "X dropoff value for gapped alignment (in bits)", /* 12 */
      "15", NULL, NULL, FALSE, 'X', ARG_INT, 0.0, 0, NULL},
    { "Number of bits to trigger gapping", /* 13 */
      "22.0", NULL, NULL, FALSE, 'N', ARG_FLOAT, 0.0, 0, NULL},
    { "Gapped",                 /* 14 */
      "T", NULL, NULL, FALSE, 'g', ARG_BOOLEAN, 0.0, 0, NULL},
    { "Start of required region in query", /* 15 */
      "1", NULL, NULL, FALSE, 'S', ARG_INT, 0.0, 0, NULL},
    { "End of required region in query (-1 indicates end of query)", /* 16 */
      "-1", NULL, NULL, FALSE, 'H', ARG_INT, 0.0, 0, NULL},
    { "Number of processors to use", /* 17 */
      "1", NULL, NULL, FALSE, 'a', ARG_INT, 0.0, 0, NULL},
    { "Show GI's in deflines",  /* 18 */
      "F", NULL, NULL, FALSE, 'I', ARG_BOOLEAN, 0.0, 0, NULL},
    { "e-value threshold for inclusion in multipass model", /* 19 */
      "0.002", NULL, NULL, FALSE, 'h', ARG_FLOAT, 0.0, 0, NULL},
    { "Constant in pseudocounts for multipass version", /* 20 */
      "9", NULL, NULL, FALSE, 'c', ARG_INT, 0.0, 0, NULL},
    { "Maximum number of passes to use in  multipass version", /* 21 */
      "1", NULL, NULL, FALSE, 'j', ARG_INT, 0.0, 0, NULL},
    { "Believe the query defline", /* 22 */
      "F", NULL, NULL, FALSE, 'J', ARG_BOOLEAN, 0.0, 0, NULL},
    { "X dropoff value for final gapped alignment (in bits)", /* 23 */
      "25", NULL, NULL, FALSE, 'Z', ARG_INT, 0.0, 0, NULL},
    { "SeqAlign file ('Believe the query defline' must be TRUE)", /* 24 */
      NULL, NULL, NULL, TRUE, 'O', ARG_FILE_OUT, 0.0, 0, NULL},
    { "Matrix",                 /* 25 */
      "BLOSUM62", NULL, NULL, FALSE, 'M', ARG_STRING, 0.0, 0, NULL},
    { "Number of database sequences to show one-line descriptions for (V)", /* 26 */
      "500", NULL, NULL, FALSE, 'v', ARG_INT, 0.0, 0, NULL},
    { "Number of database sequence to show alignments for (B)", /* 27 */
      "250", NULL, NULL, FALSE, 'b', ARG_INT, 0.0, 0, NULL},
    { "Output File for PSI-BLAST Checkpointing", /* 28 */
      NULL, NULL, NULL, TRUE, 'C', ARG_FILE_OUT, 0.0, 0, NULL},
    { "Input File for PSI-BLAST Restart", /* 29 */
      NULL, NULL, NULL, TRUE, 'R', ARG_FILE_IN, 0.0, 0, NULL},
    { "Word size", /* 30 */
      "3", NULL, NULL, FALSE, 'W', ARG_INT, 0.0, 0, NULL},
    { "Effective length of the database (use zero for the real size)", /* 31 */
      "0", NULL, NULL, FALSE, 'z', ARG_FLOAT, 0.0, 0, NULL},
    { "Number of best hits from a region to keep", /* 32 */
      "0", NULL, NULL, FALSE, 'K', ARG_INT, 0.0, 0, NULL},
    { "Compute locally optimal Smith-Waterman alignments", /* 33 */
      "F", NULL, NULL, FALSE, 's', ARG_BOOLEAN, 0.0, 0, NULL},
    { "Effective length of the search space (use zero for the real size)", /* 34 */
      "0", NULL, NULL, FALSE, 'Y', ARG_FLOAT, 0.0, 0, NULL},
    { "program option for PHI-BLAST", /* 35 */
      "blastpgp", NULL, NULL, FALSE, 'p', ARG_STRING, 0.0, 0, NULL},
    { "Hit File for PHI-BLAST", /* 36 */
      "hit_file", NULL, NULL, FALSE, 'k', ARG_FILE_IN, 0.0, 0, NULL},
    { "Produce HTML output",    /* 37 */
      "F", NULL, NULL, FALSE, 'T', ARG_BOOLEAN, 0.0, 0, NULL},
    { "Output File for PSI-BLAST Matrix in ASCII", /* 38 */
      NULL, NULL, NULL, TRUE, 'Q', ARG_FILE_OUT, 0.0, 0, NULL},
    { "Input Alignment File for PSI-BLAST Restart", /* 39 */
      NULL, NULL, NULL, TRUE, 'B', ARG_FILE_IN, 0.0, 0, NULL},
    { "Restrict search of database to list of GI's", /* 40 */
      NULL, NULL, NULL, TRUE, 'l', ARG_STRING, 0.0, 0, NULL},
    {"Use lower case filtering of FASTA sequence",    /* 41 */
     "F", NULL,NULL,TRUE,'U',ARG_BOOLEAN, 0.0,0,NULL},
    { "Use composition based statistics", /* 42 */
      "T", NULL, NULL, FALSE, 't', ARG_BOOLEAN, 0.0, 0, NULL},
    { "ASN.1 Scoremat input of checkpoint data:\n"
      "0: no scoremat input\n"
      "1: Restart is from ASCII scoremat checkpoint file,\n"
      "2: Restart is from binary scoremat checkpoint file", /* 43 */
      "0", NULL, NULL, TRUE, 'q', ARG_INT, 0.0, 0, NULL},
    { "ASN.1 Scoremat output of checkpoint data:\n"
      "0: no scoremat output\n"
      "1: Output is ASCII scoremat checkpoint file (requires -J),\n"
      "2: Output is binary scoremat checkpoint file (requires -J)", /* 44 */
      "0", NULL, NULL, TRUE, 'u', ARG_INT, 0.0, 0, NULL},
#ifdef YES_TO_DECLINE_TO_ALIGN
    { "Cost to decline alignment (disabled when 0)", /* 45 */
      "0", NULL, NULL, FALSE, 'L', ARG_INT, 0.0, 0, NULL},
#endif
};

typedef struct _pgp_blast_options {
    BLAST_OptionsBlkPtr options;
    CharPtr blast_database;
    BioseqPtr query_bsp, fake_bsp;
    Int4 number_of_descriptions, number_of_alignments;
    FILE *infp, *outfp;
    AsnIoPtr aip_out;
    Boolean html;
    Boolean believe_query;
    Uint4 align_options, print_options;
    /* PHI-PSI Blast variables */
    Uint1 featureOrder[FEATDEF_ANY];
    Uint1 groupOrder[FEATDEF_ANY];
    Int4 program_flag;
    CharPtr patfile;
    FILE *patfp; 
    seedSearchItems *seedSearch;
    Boolean is_xml_output;
    Boolean is_asn1_output;
    char* asn1_mode;  /* "w" or "wb" */
} PGPBlastOptions, PNTR PGPBlastOptionsPtr;

void PGPGetPrintOptions(Boolean gapped, Uint4Ptr align_options_out, 
                        Uint4Ptr print_options_out)
{
    Uint4 print_options, align_options;

    print_options = 0;
    if (gapped == FALSE)
        print_options += TXALIGN_SHOW_NO_OF_SEGS;
    
    align_options = 0;
    align_options += TXALIGN_COMPRESS;
    align_options += TXALIGN_END_NUM;

    if (myargs[18].intvalue) {
        align_options += TXALIGN_SHOW_GI;
        print_options += TXALIGN_SHOW_GI;
    } 
    
    if (myargs[37].intvalue) {
        align_options += TXALIGN_HTML;
        print_options += TXALIGN_HTML;
    }
    
    if (myargs[5].intvalue != 0) {
        align_options += TXALIGN_MASTER;
        if (myargs[5].intvalue == 1 || myargs[5].intvalue == 3)
            align_options += TXALIGN_MISMATCH;
        if (myargs[5].intvalue == 3 || myargs[5].intvalue == 4 || myargs[5].intvalue == 6)
            align_options += TXALIGN_FLAT_INS;
        if (myargs[5].intvalue == 5 || myargs[5].intvalue == 6)
            align_options += TXALIGN_BLUNT_END;
    } else {
        align_options += TXALIGN_MATRIX_VAL;
        align_options += TXALIGN_SHOW_QS;
    }

    *align_options_out = align_options;
    *print_options_out = print_options;

    return;
}
void PGPFreeBlastOptions(PGPBlastOptionsPtr bop)
{

    bop->options = BLASTOptionDelete(bop->options);
    
    FileClose(bop->infp);
    FileClose(bop->outfp);
    bop->aip_out = AsnIoClose(bop->aip_out);
    
    MemFree(bop->blast_database);
    MemFree(bop->patfile);
    MemFree(bop->seedSearch);

    MemFree(bop);

    return;
} 
    
PGPBlastOptionsPtr PGPReadBlastOptions(void)
{
    PGPBlastOptionsPtr bop;
    BLAST_OptionsBlkPtr options;
    SeqEntryPtr sep;
    Boolean is_dna;
    ObjectIdPtr obidp;
    Int4 align_view;

    align_view = myargs[5].intvalue;

    bop = MemNew(sizeof(PGPBlastOptions));
    
    bop->blast_database   = StringSave(myargs[0].strvalue);

    if (align_view != 7 && align_view != 10 && align_view != 11 && myargs[6].strvalue != NULL) {
        if ((bop->outfp = FileOpen(myargs[6].strvalue, "w")) == NULL) {
            ErrPostEx(SEV_FATAL, 1, 0, "blast: Unable to open output "
                      "file %s\n", myargs[6].strvalue);
            return NULL;
        }
    }
    
    bop->number_of_descriptions = myargs[26].intvalue;
    bop->number_of_alignments = myargs[27].intvalue;
    
    if (myargs[22].intvalue != 0)
        bop->believe_query = TRUE;
    
    if (myargs[43].intvalue != NO_SCOREMAT_IO && 
        myargs[43].intvalue != ASCII_SCOREMAT &&
        myargs[43].intvalue != BINARY_SCOREMAT) {
        ErrPostEx(SEV_FATAL, 1, 0,"Invalid choice for scoremat input\n");
        return NULL;
    }
    if (myargs[44].intvalue != NO_SCOREMAT_IO && 
        myargs[44].intvalue != ASCII_SCOREMAT &&
        myargs[44].intvalue != BINARY_SCOREMAT) {
        ErrPostEx(SEV_FATAL, 1, 0,"Invalid choice for scoremat output\n");
        return NULL;
    }
    if (myargs[44].intvalue != NO_SCOREMAT_IO) {
        if (bop->believe_query == FALSE) {
            ErrPostEx(SEV_FATAL, 1, 0, 
                      "-J option must be TRUE for scoremat output");
            return NULL;
        }
    }

    if (myargs[24].strvalue != NULL) {
        
        if (bop->believe_query == FALSE) {
            ErrPostEx(SEV_FATAL, 1, 0, 
                      "-J option must be TRUE for ASN.1 output");
            return NULL;
        }
        
        if ((bop->aip_out = AsnIoOpen (myargs[24].strvalue,"w")) == NULL) {
            ErrPostEx(SEV_FATAL, 1, 0, "blast: Unable to open output "
                      "file %s\n", myargs[24].strvalue);
            return NULL;
        }
    }
    else if (align_view == 10 || align_view == 11)
    {
        const char* mode = (align_view == 10) ? "w" : "wb";
        if (bop->believe_query == FALSE) {
            ErrPostEx(SEV_FATAL, 1, 0, 
                      "-J option must be TRUE for ASN.1 output");
            return NULL;
        }
        if ((bop->aip_out = AsnIoOpen(myargs[6].strvalue, (char*) mode)) == NULL) {
                ErrPostEx(SEV_FATAL, 1, 0, "blast: Unable to open output file %s\n", myargs[6].strvalue);
                return NULL;
        }
	bop->is_asn1_output = TRUE;
        if(align_view == 10)
            bop->asn1_mode = StringSave("w");
        else 
            bop->asn1_mode = StringSave("wb");
    }

    if (!myargs[14].intvalue && myargs[21].intvalue > 1) {
       ErrPostEx(SEV_ERROR, 1, 0, "Ungapped search option is not supported"
                 " with more than one iteration\n");
       return NULL;
    }

    options = BLASTOptionNew("blastp", TRUE);
    bop->options = options;
    
    /* read the query sequence */

    if (myargs[43].intvalue == NO_SCOREMAT_IO) {
        if ((bop->infp = FileOpen(myargs[1].strvalue, "r")) == NULL) {
            ErrPostEx(SEV_FATAL, 1, 0, "blast: Unable to open input file %s\n", 
                      myargs[1].strvalue);
            return NULL;
        }
        if(myargs[41].intvalue) {
            if((sep = FastaToSeqEntryForDb (bop->infp, FALSE, NULL, bop->believe_query, NULL, NULL, &options->query_lcase_mask)) == NULL) {
                ErrPostEx(SEV_FATAL, 1, 0, "Unable to read input FASTA file\n");
                return NULL;
            }
        } else {
            if((sep = FastaToSeqEntryEx(bop->infp, FALSE, NULL, bop->believe_query)) == NULL) {
                ErrPostEx(SEV_FATAL, 1, 0, "Unable to read input FASTA file\n");
                return NULL;
            }
        }
        
        SeqEntryExplore(sep, &bop->query_bsp, FindProt);    
        if (bop->query_bsp == NULL) {
            ErrPostEx(SEV_FATAL, 1, 0, "Unable to obtain bioseq\n");
            return NULL;
        }    
    }
    else {                      /* recover the query sequence from scoremat */
        AsnIoPtr scorematfile;
        PssmWithParametersPtr scoremat = NULL;

        if (myargs[29].strvalue == NULL) {
            ErrPostEx(SEV_FATAL, 1, 0, "No restart file specified\n");
            return NULL;
        }

        if (myargs[43].intvalue == ASCII_SCOREMAT)
            scorematfile = AsnIoOpen(myargs[29].strvalue, "r");
        else  /* binary scoremat */
            scorematfile = AsnIoOpen(myargs[29].strvalue, "rb");

        if (scorematfile == NULL) {
            ErrPostEx(SEV_FATAL, 1, 0, "Unable to open scoremat file\n");
            return NULL;
        }    
        
        scoremat = PssmWithParametersAsnRead(scorematfile, NULL);
        AsnIoClose(scorematfile);
        if (scoremat == NULL) {
            ErrPostEx(SEV_FATAL, 1, 0, "Could not read scoremat "
                                       "for query sequence\n");
            return NULL;
        }

        if (scoremat->pssm == NULL ||
            scoremat->pssm->query == NULL ||
            scoremat->pssm->query->data.ptrvalue == NULL) {
            ErrPostEx(SEV_FATAL, 1, 0, "Cannot read query sequence "
                                       "from scoremat\n");
            return NULL;
        }

        /* use the bioseq from 'scoremat'; remove from that
           structure so the bioseq is not freed along with the
           rest of the scoremat. Note that only the first
           bioseq in a bioseq-set is used */

        bop->query_bsp = (BioseqPtr)(scoremat->pssm->query->data.ptrvalue);
        scoremat->pssm->query = ValNodeFree(scoremat->pssm->query);
        PssmWithParametersFree(scoremat);
    }
    
    /* without a bioseq we cannot continue */
    if (bop->query_bsp == NULL) {
        ErrPostEx(SEV_FATAL, 1, 0, "Unable to obtain bioseq\n");
        return NULL;
    }    

    /* Set default gap params for matrix. */
    BLASTOptionSetGapParams(options, myargs[25].strvalue, 0, 0);

    if(myargs[5].intvalue == 7) {
        bop->is_xml_output = TRUE;
    } else if (myargs[5].intvalue != 8 && myargs[5].intvalue != 9) {
        PGPGetPrintOptions(options->gapped_calculation, &bop->align_options, 
                           &bop->print_options);
    }

    /* decrement by one to agree with program values. */
    options->required_start = myargs[15].intvalue - 1;
    options->required_end = myargs[16].intvalue;
    if (options->required_end != -1) {
        options->required_end--;
    }
    
    options->window_size = myargs[2].intvalue;

    if(myargs[3].intvalue)    
        options->threshold_second = (Int4) myargs[3].intvalue;
    
    options->dropoff_2nd_pass  = myargs[7].floatvalue;
    options->expect_value  = (Nlm_FloatHi) myargs[4].floatvalue;
    options->hitlist_size = MAX(bop->number_of_descriptions, 
                                bop->number_of_alignments);
    
    if (myargs[8].intvalue == 1) {
       options->two_pass_method  = FALSE;
       options->multiple_hits_only  = FALSE;
    } else { 
       /* all other inputs, including the default 0 use 2-hit method */
       options->two_pass_method  = FALSE;
       options->multiple_hits_only  = TRUE;
    }
    options->gap_open = myargs[10].intvalue;
    options->gap_extend = myargs[11].intvalue;
    
#ifdef YES_TO_DECLINE_TO_ALIGN
    if(myargs[45].intvalue != 0) {
       options->discontinuous = TRUE;
       options->decline_align = myargs[45].intvalue;
    } else {
       options->discontinuous = FALSE;
       options->decline_align = INT2_MAX;
    }
#endif
    
    options->gap_x_dropoff = myargs[12].intvalue;
    options->gap_x_dropoff_final = myargs[23].intvalue;
    options->gap_trigger = myargs[13].floatvalue;
    
    if (StringICmp(myargs[9].strvalue, "T") == 0) {
        options->filter_string = StringSave("S");
    } else {
        options->filter_string = StringSave(myargs[9].strvalue);
    }
    
    options->number_of_cpus = (Int2) myargs[17].intvalue;
    
    
    options->isPatternSearch = FALSE;
    
    if (0 != (StringCmp("blastpgp",myargs[35].strvalue))) {
        options->isPatternSearch = TRUE;
        options->discontinuous = FALSE;
        options->decline_align = INT2_MAX;    
        bop->program_flag = convertProgramToFlag(myargs[35].strvalue, 
                                                 &is_dna);
    }
    
    if (options->isPatternSearch) {
        bop->patfile = StringSave(myargs[36].strvalue);
        if ((bop->patfp = FileOpen(bop->patfile, "r")) == NULL) {
            ErrPostEx(SEV_FATAL, 1, 0, "blast: Unable to open pattern "
                      "file %s\n", bop->patfile);
            return NULL;
        }
        
        bop->seedSearch = (seedSearchItems *) 
            ckalloc(sizeof(seedSearchItems));
    }
    
    if(options->isPatternSearch)
        fillCandLambda(bop->seedSearch, myargs[25].strvalue, options);
    
    options->ethresh = (Nlm_FloatHi) myargs[19].floatvalue;
    options->pseudoCountConst = myargs[20].intvalue;
    options->maxNumPasses = myargs[21].intvalue;
    /*zero out e-value threshold if it will not be used*/
    if (options->maxNumPasses == 1)
        options->ethresh = 0.0;
    if (myargs[30].intvalue)
        options->wordsize = myargs[30].intvalue;
    if (myargs[31].floatvalue)
        options->db_length = (Int8) myargs[31].floatvalue;
    
    options->hsp_range_max  = myargs[32].intvalue;
    if (options->hsp_range_max != 0)
        options->perform_culling = TRUE;

    options->block_width  = 20; /* Default value - previously '-L' parameter */
    
    if (myargs[34].floatvalue)
        options->searchsp_eff = (Nlm_FloatHi) myargs[34].floatvalue;

    options->tweak_parameters = (Boolean) myargs[42].intvalue;
    options->smith_waterman = (Boolean) myargs[33].intvalue;

    if (bop->options->tweak_parameters) {
      /*allows for extra matches in first pass of screening,
        hitlist_size */
      bop->options->original_expect_value = bop->options->expect_value;
      bop->options->hitlist_size *= 2; 
    }


    /* Seting list of gis to restrict search */
    
    if (myargs[40].strvalue) {
        options->gifile = StringSave(myargs[40].strvalue);
    }
    
    options = BLASTOptionValidate(options, "blastp");
    
    if (options == NULL)
        return NULL;

    if(bop->believe_query) {
        bop->fake_bsp = bop->query_bsp;
    } else {
        bop->fake_bsp = BlastMakeFakeBioseq(bop->query_bsp, NULL);
        
        BLASTUpdateSeqIdInSeqInt(bop->options->query_lcase_mask, 
                                 bop->fake_bsp->id);
    }

    return bop;
}

Boolean PGPReadNextQuerySequence(PGPBlastOptionsPtr bop)
{
    SeqEntryPtr sep;

    /* Cleaning up stuff from previous query run */

    bop->options->query_lcase_mask = SeqLocSetFree(bop->options->query_lcase_mask);

    if (bop->believe_query == TRUE) { /* Not corect - to be fixed */
        BioseqFree(bop->query_bsp);
    } else {
        BioseqFree(bop->fake_bsp);
    }

    /* scoremats contain only one sequence */
    if(myargs[29].intvalue != NO_SCOREMAT_IO) { 
        return FALSE;
    }

    if(myargs[41].intvalue) {
        if((sep = FastaToSeqEntryForDb (bop->infp, FALSE, NULL, bop->believe_query, NULL, NULL, &bop->options->query_lcase_mask)) == NULL) {
            return FALSE;
        }
    } else {
        if((sep = FastaToSeqEntryEx(bop->infp, FALSE, NULL, 
                                    bop->believe_query)) == NULL) {
            return FALSE;
        }
    }

    bop->query_bsp = NULL;
    SeqEntryExplore(sep, &bop->query_bsp, FindProt);    
    
    if (bop->query_bsp == NULL) {
        ErrPostEx(SEV_FATAL, 1, 0, "Unable to obtain bioseq\n");
        return 0;
    }    
    
    if(bop->believe_query) {
        bop->fake_bsp = bop->query_bsp;
    } else {
        bop->fake_bsp = BlastMakeFakeBioseq(bop->query_bsp, NULL);
        
        BLASTUpdateSeqIdInSeqInt(bop->options->query_lcase_mask, 
                                 bop->fake_bsp->id);
    }

    return TRUE;
}

Boolean PGPFormatHeader(PGPBlastOptionsPtr bop)
{
    Boolean html = myargs[37].intvalue;

    if (bop->outfp == NULL)
	return FALSE;

    if (html)
        fprintf(bop->outfp, "<PRE>\n");
    
    init_buff_ex(90);
    BlastPrintVersionInfo("blastp", html, bop->outfp);
    fprintf(bop->outfp, "\n");
    BlastPrintReference(html, 90, bop->outfp);
    fprintf(bop->outfp, "\n");
    AcknowledgeBlastQuery(bop->query_bsp, 70, 
                          bop->outfp, bop->believe_query, html);
    PrintDbInformation(bop->blast_database, TRUE, 70, bop->outfp, html);
    free_buff();

    return TRUE;
}
Boolean  PGPFormatFooter(PGPBlastOptionsPtr bop, BlastSearchBlkPtr search)
{
    
    ValNodePtr  mask_loc, vnp;
    BLAST_KarlinBlkPtr ka_params=NULL, ka_params_gap=NULL;
    TxDfDbInfoPtr dbinfo=NULL, dbinfo_head;
    CharPtr params_buffer=NULL;
    ValNodePtr other_returns;
    BLAST_MatrixPtr blast_matrix;
    Boolean html = myargs[37].intvalue;

    if (bop->outfp == NULL)
	return FALSE;

    if (html)
        fprintf(bop->outfp, "<PRE>\n");

    other_returns = BlastOtherReturnsPrepare(search);

    mask_loc = NULL;
    for (vnp=other_returns; vnp; vnp = vnp->next) {
        switch (vnp->choice) {
        case TXDBINFO:
            dbinfo = vnp->data.ptrvalue;
            break;
        case TXKABLK_NOGAP:
            ka_params = vnp->data.ptrvalue;
            break;
        case TXKABLK_GAP:
            ka_params_gap = vnp->data.ptrvalue;
            break;
        case TXPARAMETERS:
            params_buffer = vnp->data.ptrvalue;
            break;
        case TXMATRIX:
            blast_matrix = vnp->data.ptrvalue;
            BLAST_MatrixDestruct(blast_matrix);
            break;
        case SEQLOC_MASKING_NOTSET:
        case SEQLOC_MASKING_PLUS1:
        case SEQLOC_MASKING_PLUS2:
        case SEQLOC_MASKING_PLUS3:
        case SEQLOC_MASKING_MINUS1:
        case SEQLOC_MASKING_MINUS2:
        case SEQLOC_MASKING_MINUS3:
            ValNodeAddPointer(&mask_loc, vnp->choice, vnp->data.ptrvalue);
            break;
        default:
            break;
        }
    }	
    
    init_buff_ex(85);
    dbinfo_head = dbinfo;
    while (dbinfo) {
        PrintDbReport(dbinfo, 70, bop->outfp);
        dbinfo = dbinfo->next;
    }
    dbinfo_head = TxDfDbInfoDestruct(dbinfo_head);
    
    if (ka_params && !bop->options->isPatternSearch) {
        PrintKAParameters(ka_params->Lambda, ka_params->K, ka_params->H, 70, bop->outfp, FALSE);
    }
    
    if (ka_params_gap) {
        if (bop->options->isPatternSearch)
            PrintKAParametersExtra(ka_params_gap->Lambda, ka_params_gap->K, ka_params_gap->H, bop->seedSearch->paramC, 70, bop->outfp, FALSE);
        else
            PrintKAParameters(ka_params_gap->Lambda, ka_params_gap->K, ka_params_gap->H, 70, bop->outfp, FALSE);
    }
    
    MemFree(ka_params);
    MemFree(ka_params_gap);

    PrintTildeSepLines(params_buffer, 70, bop->outfp);
    MemFree(params_buffer);
    free_buff();

    return TRUE;
}

Boolean PGPrintPosMatrix(CharPtr filename, posSearchItems *posSearch, 
                         compactSearchItems *compactSearch, 
                         Boolean posComputationCalled)
{
    FILE *fp;
    
    if ((fp = FileOpen(filename, "w")) == NULL) {
        ErrPostEx(SEV_FATAL, 1, 0, "Unable to open matrix output file %s\n", 
                  filename);
        return FALSE;
    }

    /* a diagnostic, partly an option with -Q. */
    outputPosMatrix(posSearch, compactSearch, fp, posComputationCalled); 
    FileClose(fp);

    return TRUE;
}

SeqAlignPtr PGPSeedSearch(PGPBlastOptionsPtr bop, BlastSearchBlkPtr search,
                          posSearchItems *posSearch, 
                          SeqLocPtr PNTR seqloc_duplicate,
                          SeqAlignPtr PNTR PNTR lastSeqAligns,
                          Int4Ptr numLastSeqAligns)
{
    Uint1Ptr query = NULL; /*query sequence read in*/
    Uint1Ptr unfilter_query = NULL; /*needed if seg will filter query*/
    SeqLocPtr seg_slp;  /*pointer to structure for seg filtering*/
    Int4 i, queryLength;   /*length of query sequence*/
    SeqAlignPtr head;
    SeqAnnotPtr annot;
    SeqFeatPtr sfp;
    SeqLocPtr seqloc, next;
    ValNodePtr seedReturn; /*return value from seedEngineCore, which
                             is a list of lists of SeqAligns, one
                             list of SeqAligns per pattern occurrence*/
    
    ValNodePtr vnp, info_vnp;  /* Output text messages from seedEngineCore() */
    
    query = BlastGetSequenceFromBioseq(bop->fake_bsp, &queryLength);
    seg_slp = BlastBioseqFilter(bop->fake_bsp, 
                                bop->options->filter_string);
    if (seg_slp) {
        unfilter_query = MemNew((queryLength + 1) * sizeof(Uint1));
        for (i = 0; i < queryLength; i++)
            unfilter_query[i] = query[i];
        BlastMaskTheResidues(query,queryLength,21,seg_slp,FALSE, 0);
    }
    
    search->gap_align = GapAlignBlkNew(1,1);
    search->gap_align->gap_open = bop->options->gap_open;
    search->gap_align->gap_extend = bop->options->gap_extend;


    search->gap_align->decline_align = INT2_MAX;

#ifdef YES_TO_DECLINE_TO_ALIGN
    if(myargs[45].intvalue != 0) {
        search->gap_align->decline_align = myargs[45].intvalue;
    } else {
        search->gap_align->decline_align = INT2_MAX;
    }
#endif
    
    /* search->gap_align->decline_align = (-(BLAST_SCORE_MIN)); */
    /* search->gap_align->decline_align = myargs[41].intvalue; */

    search->gap_align->x_parameter = bop->options->gap_x_dropoff
        *NCBIMATH_LN2/bop->seedSearch->paramLambda;
    search->gap_align->matrix = search->sbp->matrix;
    initProbs(bop->seedSearch);
    init_order(search->gap_align->matrix, bop->program_flag, 
               FALSE, bop->seedSearch);
    
    for(i = 0; i < queryLength; i++)
        query[i] = bop->seedSearch->order[query[i]];
    
    if (unfilter_query) {
        for(i = 0; i < queryLength; i++)
            unfilter_query[i] = bop->seedSearch->order[unfilter_query[i]];
    }
            
    seqloc = NULL;
    seedReturn = seedEngineCore(search, bop->options, query, 
                                unfilter_query,
                                bop->blast_database, bop->patfile, 
                                bop->program_flag, 
                                bop->patfp, FALSE, FALSE, 
                                bop->seedSearch, bop->options->ethresh, 
                                myargs[34].floatvalue, posSearch, 
                                &seqloc, TRUE, &info_vnp);
    if (!bop->is_xml_output)
       PGPOutTextMessages(info_vnp, bop->outfp);
    ValNodeFreeData(info_vnp);
    
    if (search->error_return) {
       BlastErrorPrint(search->error_return);
       for (vnp = search->error_return; vnp; vnp = vnp->next) {
          BlastDestroyErrorMessage((BlastErrorMsgPtr)vnp->data.ptrvalue);
       }
       search->error_return = ValNodeFree(search->error_return);
    }
    *seqloc_duplicate = seqloc;
    head = convertValNodeListToSeqAlignList(seedReturn, lastSeqAligns, 
                                            numLastSeqAligns);

    bop->featureOrder[FEATDEF_REGION] = 1;
    bop->groupOrder[FEATDEF_REGION] = 1;
    annot = bop->fake_bsp->annot = SeqAnnotNew();
    bop->fake_bsp->annot->type = 1;	/* ftable. */
    while (seqloc) {
        next = seqloc->next;
        sfp = SeqFeatNew();
        sfp->location = seqloc;
        sfp->data.choice = SEQFEAT_REGION;
        sfp->data.value.ptrvalue = StringSave("pattern");
        annot->data = sfp;
        if (next) {
            annot->next = SeqAnnotNew();
            annot = annot->next;
            annot->type = 1;
        }
        seqloc = next;
    }

    if (query != NULL)
        MemFree(query);
    if (unfilter_query != NULL)
        unfilter_query = MemFree(unfilter_query);
    
    return head;
}

void PGPFormatMainOutput(SeqAlignPtr head, PGPBlastOptionsPtr bop,
                         BlastSearchBlkPtr search, Int4 thisPassNum,
                         SeqAlignPtr PNTR lastSeqAligns, 
                         Int4 numLastSeqAligns, SeqLocPtr seed_seqloc,
                         Int2Ptr posRepeatSequences)
{
    SeqAnnotPtr seqannot;
    ValNodePtr pruneSeed, seedReturn;  
    BlastPruneSapStructPtr prune;
    BLAST_MatrixPtr matrix;
    Int4Ptr PNTR txmatrix;
    BioseqPtr query_bsp;

    if(head == NULL && myargs[5].intvalue < 7) {
        fprintf(bop->outfp, "\n\n ***** No hits found ******\n\n");
        return;
    }
    
    if (myargs[5].intvalue == 8 || myargs[5].intvalue == 9) {
       query_bsp = (bop->believe_query) ? bop->query_bsp : bop->fake_bsp;
	if (myargs[5].intvalue == 9)
       		PrintTabularOutputHeader(bop->blast_database, query_bsp, NULL, 
                                "blastp", thisPassNum, bop->believe_query,
                                bop->outfp);
       BlastPrintTabulatedResults(head, query_bsp, NULL, 
                                  bop->number_of_alignments,
                                  "blastp", FALSE,
                                  bop->believe_query, 0, 0, bop->outfp, FALSE);
       return;
    }

    seqannot = SeqAnnotNew();
    seqannot->type = 2;
    AddAlignInfoToSeqAnnot(seqannot, 2);
    seqannot->data = head;

    if (search->pbp->maxNumPasses != 1 && myargs[5].intvalue < 7) {
        fprintf(bop->outfp, "\nResults from round %d\n", 
                thisPassNum);
    }
    
    ObjMgrSetHold();
    init_buff_ex(85);

    /* ------- Printing deflines for the BLAST output ------- */

    if (thisPassNum == 1) {
        search->positionBased = FALSE;
        if (!bop->options->isPatternSearch) {
            prune = BlastPruneHitsFromSeqAlign(head, 
                             bop->number_of_descriptions, NULL);
            PrintDefLinesFromSeqAlign(prune->sap, 80, bop->outfp, 
                                      bop->print_options, FIRST_PASS, NULL);
        } else {
            seedReturn = convertSeqAlignListToValNodeList(head,lastSeqAligns, 
                                                          numLastSeqAligns);
                        
            pruneSeed = SeedPruneHitsFromSeedReturn(seedReturn, 
                                      bop->number_of_descriptions);
            PrintDefLinesExtra(pruneSeed, 80, bop->outfp, bop->print_options, 
                               FIRST_PASS, NULL, seed_seqloc);
        }
    } else {
        prune = BlastPruneHitsFromSeqAlign(head, 
                              bop->number_of_descriptions, NULL);
        if (ALL_ROUNDS) {
            PrintDefLinesFromSeqAlign(prune->sap, 80, bop->outfp, 
                                      bop->print_options, 
                                      NOT_FIRST_PASS_REPEATS, 
                                      posRepeatSequences);
            PrintDefLinesFromSeqAlign(prune->sap, 80, bop->outfp, 
                                      bop->print_options, NOT_FIRST_PASS_NEW, 
                                      posRepeatSequences);
        } else
            PrintDefLinesFromSeqAlign(prune->sap, 80, bop->outfp, 
                                      bop->print_options, FIRST_PASS, NULL);
    } /*thisPassNum == 1 */

    /* ------- --------------------------------------- ------- */

    if (ALL_ROUNDS && search->posConverged && myargs[5].intvalue < 7) {
        fprintf(bop->outfp, "\nCONVERGED!\n");
    }

    free_buff();
    
    matrix = NULL;
    txmatrix = NULL;
    if (search->sbp->matrix) {
       matrix = BLAST_MatrixFill(search->sbp, FALSE);
       txmatrix = BlastMatrixToTxMatrix(matrix);
    }

    if (!(bop->options->isPatternSearch)) {
        prune = BlastPruneHitsFromSeqAlign(head, bop->number_of_alignments, 
                                           prune);
        seqannot->data = prune->sap;

        if(bop->options->discontinuous) {
            if(!DDV_DisplayBlastPairList(prune->sap, search->mask, 
                                         bop->outfp, FALSE, 
                                         bop->align_options,
                                         bop->align_options & TXALIGN_HTML ? 6 : 1)) { 
                fprintf(stdout, 
                        "\n\n!!!\n   "
                        "    --------  Failure to print alignment...  --------"
                        "\n!!!\n\n");
                fflush(stdout);
            }
        } else {   /* Old type formating */
            if (myargs[5].intvalue != 0) {
                ShowTextAlignFromAnnot2(seqannot, 60, bop->outfp, 
                                        bop->featureOrder, bop->groupOrder, 
                                        bop->align_options, NULL, 
                                        search->mask, NULL, 
                                        NULL, NULL);
            } else {
                ShowTextAlignFromAnnot2(seqannot, 60, bop->outfp, 
                                        bop->featureOrder, bop->groupOrder, 
                                        bop->align_options, txmatrix, 
                                        search->mask, FormatScoreFunc, 
                                        NULL, NULL);
            }
        }

        /* seqannot->data = head; */

    } else {
        if (bop->number_of_alignments < bop->number_of_descriptions) {
            pruneSeed = SeedPruneHitsFromSeedReturn(pruneSeed, 
                                                    bop->number_of_alignments);
        }


        if (myargs[5].intvalue != 0) {
            ShowTextAlignFromAnnotExtra(bop->query_bsp, pruneSeed, 
                                        seed_seqloc, 60, bop->outfp, 
                                        bop->featureOrder, bop->groupOrder, 
                                        bop->align_options, NULL, 
                                        search->mask, NULL);
        } else {
            ShowTextAlignFromAnnotExtra(bop->query_bsp, pruneSeed, 
                                        seed_seqloc, 60, bop->outfp, 
                                        bop->featureOrder, bop->groupOrder, 
                                        bop->align_options, txmatrix, 
                                        search->mask, FormatScoreFunc);
        }
    }

    if (!(bop->options->isPatternSearch)) {
        prune = BlastPruneSapStructDestruct(prune);
    }

    search->positionBased = TRUE;
    ObjMgrClearHold();
    ObjMgrFreeCache(0);

    matrix = BLAST_MatrixDestruct(matrix);
    if (txmatrix)
       txmatrix = TxMatrixDestruct(txmatrix);

    seqannot->data = NULL;
    seqannot = SeqAnnotFree(seqannot);

    return;
}

void PGPSeqAlignOut(PGPBlastOptionsPtr bop, SeqAlignPtr head)
{
    SeqAnnotPtr seqannot;

    if (!bop->aip_out || !head)
        return;
    
    seqannot = SeqAnnotNew();
    seqannot->type = 2;
    AddAlignInfoToSeqAnnot(seqannot, 2);
    seqannot->data = head;
    SeqAnnotAsnWrite((SeqAnnotPtr) seqannot, bop->aip_out, NULL);
    AsnIoReset(bop->aip_out);
/*
    bop->aip_out = AsnIoClose(bop->aip_out);
*/
    seqannot->data = NULL;
    seqannot = SeqAnnotFree(seqannot);
}

Int2 Main (void)
     
{
    PGPBlastOptionsPtr bop;
    BlastSearchBlkPtr search;
    SeqAlignPtr  head = NULL;
    SeqLocPtr seed_seqloc = NULL;
    Char buf[256] = { '\0' };
    
    /* used for psi-blast */
    Int4 thisPassNum;       
    posSearchItems *posSearch;
    compactSearchItems *compactSearch;
    Boolean  recoverCheckpoint = FALSE;
    Boolean  alreadyRecovered = FALSE;
    Boolean  freqCheckpoint = FALSE;
    Boolean  alignCheckpoint = FALSE;
    Boolean  posComputationCalled = FALSE;
    Boolean  checkReturn = FALSE;
    Boolean  next_query = FALSE;
    Boolean tabular_output;
    AsnIoPtr aip = NULL;

    SeqAlignPtr PNTR lastSeqAligns = NULL; 
                                /*keeps track of the last SeqAlign in
                                  each list of seedReturn so that the
                                  2-level list can be converted to a 1-level
                                  list and then back to 2-level*/
    Int4 numLastSeqAligns = 0;

    PSIXmlPtr psixp = NULL;
    ValNodePtr vnp;

    /* ----- End of definitions ----- */
    
    StringCpy(buf, "blastpgp ");
    StringNCat(buf, BlastGetVersionNumber(), sizeof(buf)-StringLen(buf)-1);
    if (! GetArgs (buf, NUMARG, myargs))
        return (1);
    ErrSetMessageLevel(SEV_WARNING);
    
    UseLocalAsnloadDataAndErrMsg ();

    if (! SeqEntryLoad())
        return 1;    
    
    if ((bop = PGPReadBlastOptions()) == NULL)
       return 1;

    if(bop->is_xml_output) {
       if((aip = AsnIoOpen(myargs[6].strvalue, "wx")) == NULL)
          return 1;
    }
    
    /* Here we will start main do/while loop over many query entries in 
       the input file. If there is an option for checkpoint recover or
       Output File for PSI-BLAST Checkpointing all but first entries in
       the input file will be discarded */

    do {    
        search = BLASTSetUpSearchWithReadDb(bop->fake_bsp, "blastp", 
                                            bop->query_bsp->length, 
                                            bop->blast_database,
                                            bop->options, NULL);
    
        if (search == NULL)
            return 1;

    
        if(bop->is_xml_output) {
           psixp = PSIXmlInit(aip, "blastp", bop->blast_database, 
                              bop->options, 
                              bop->fake_bsp, 0);
        }

        /*AAS*/
        if ((NULL != myargs[29].strvalue) || (NULL != myargs[39].strvalue)) {
            recoverCheckpoint = TRUE;
            if (NULL != myargs[29].strvalue) {
                freqCheckpoint = TRUE;
                alignCheckpoint = FALSE;
            } else {
                freqCheckpoint = FALSE;
                alignCheckpoint = TRUE;
            }
        }
        
        if (recoverCheckpoint)
            search->positionBased = TRUE;
        else
            search->positionBased = FALSE;
        
        global_fp = bop->outfp;
        tabular_output = (myargs[5].intvalue == 8 || myargs[5].intvalue == 9);

        if(!bop->is_xml_output && !tabular_output) {   
            PGPFormatHeader(bop);
        }
        
        posSearch = NULL;
        thisPassNum = 0;
        compactSearch = NULL;
        search->posConverged = FALSE;
        global_fp = bop->outfp;
        search->error_return = NULL;
        /*AAS*/
        if (recoverCheckpoint) {
            posSearch = (posSearchItems *) MemNew(1 * sizeof(posSearchItems));
            compactSearch = compactSearchNew(compactSearch);
            copySearchItems(compactSearch, search, bop->options->matrix);
            posInitializeInformation(posSearch,search);
            /*AAS*/
            if (freqCheckpoint) {
                checkReturn = posReadCheckpoint(posSearch, compactSearch, myargs[29].strvalue, myargs[43].intvalue, &(search->error_return));
                search->sbp->posMatrix = posSearch->posMatrix;
                if (NULL == search->sbp->posFreqs)
                    search->sbp->posFreqs =  allocatePosFreqs(compactSearch->qlength, compactSearch->alphabetSize);
                copyPosFreqs(posSearch->posFreqs,search->sbp->posFreqs, compactSearch->qlength, compactSearch->alphabetSize);
            } else {
                search->sbp->posMatrix = BposComputation(posSearch, search, compactSearch, myargs[39].strvalue, myargs[28].strvalue, myargs[44].intvalue, bop->query_bsp, myargs[10].intvalue, myargs[11].intvalue, &(search->error_return)); 
		posComputationCalled = TRUE;
                if (NULL == search->sbp->posMatrix)
                    checkReturn = FALSE;
                else
                    checkReturn = TRUE;
            }
            

            if (search->error_return) {
               BlastErrorPrint(search->error_return);
               for (vnp = search->error_return; vnp; vnp = vnp->next) {
                  BlastDestroyErrorMessage((BlastErrorMsgPtr)vnp->data.ptrvalue);
               }
               search->error_return = ValNodeFree(search->error_return);
            }
            if (!checkReturn) {
                ErrPostEx(SEV_FATAL, 1, 0, "blast: Error recovering from checkpoint");
                return 1;
            }
        
            /* Print out Pos matrix if necessary */
            if (myargs[38].strvalue != NULL)
                PGPrintPosMatrix(myargs[38].strvalue, posSearch, compactSearch, posComputationCalled);
        }
        
        do {  /*AAS*/
            thisPassNum++;
            if (thisPassNum > 1)
                bop->options->isPatternSearch = FALSE;
            
            if(head != NULL)
                head = SeqAlignSetFree(head);
            
#ifdef OS_UNIX
            if(!bop->is_xml_output && !tabular_output && !bop->is_asn1_output) {
                search->thr_info->tick_callback =  tick_callback;
                fprintf(global_fp, "%s", "Searching");
                fflush(global_fp);
            }
#endif
            if (1 == thisPassNum && (!recoverCheckpoint)) {
                
                posSearch = (posSearchItems *) 
                    MemNew(1 * sizeof(posSearchItems));
            }
            
            /* ----- Here is real BLAST search done ------- */
            
            if (bop->options->isPatternSearch && 
                (1 == thisPassNum && (!recoverCheckpoint))) {
                head = PGPSeedSearch(bop, search, posSearch, 
                                     &seed_seqloc,
                                     &lastSeqAligns, &numLastSeqAligns);
            } else {
                if ((bop->options->tweak_parameters) && (thisPassNum > 1)) {
                    /*allows for extra matches in first pass of screening,
                      hitlist_size will be restored in
                      BioseqBlastEngineCore for the second pass. */
                    bop->options->original_expect_value = 
                        bop->options->expect_value;
                    bop->options->hitlist_size *= 2; 
                }
                
                if ((1 == thisPassNum) && (!recoverCheckpoint))
                    head = BioseqBlastEngineCore(search, bop->options, NULL);
                else
                    head = BioseqBlastEngineCore(search, bop->options, 
                                                 search->sbp->posMatrix);  
            }
            /* ---------------------------------------------- */
            
            if (recoverCheckpoint) {
                compactSearchDestruct(compactSearch);
                recoverCheckpoint = FALSE;
                alreadyRecovered = TRUE;
            }
            
            if (thisPassNum == 1) {
                compactSearch = compactSearchNew(compactSearch);
            } else {
                MemFree(compactSearch->standardProb);
            }
            
            copySearchItems(compactSearch, search, bop->options->matrix);
            
            
            /* The next two calls (after the "if") are diagnostics 
               for Stephen. Don't perform this if only one pass will 
               be made (i.e., standard BLAST) */
            
            if (ALL_ROUNDS && 1 != search->pbp->maxNumPasses) {
                if ((1 == thisPassNum)  && (!alreadyRecovered))
                    posInitializeInformation(posSearch, search);
                posPrintInformation(posSearch, search, thisPassNum);
            }
            
#ifdef OS_UNIX
            if(!bop->is_xml_output && !tabular_output && !bop->is_asn1_output) {
                fprintf(global_fp, "%s", "done\n\n");
            }
#endif
            
            /* AAS */
            if (thisPassNum == 1) {
                ReadDBBioseqFetchEnable ("blastpgp", bop->blast_database, 
                                         FALSE, TRUE);
            } else {
                
                /* Have things converged? */
                if (ALL_ROUNDS && search->pbp->maxNumPasses != 1) {
                    posConvergenceTest(posSearch, search, head, thisPassNum);
                }
            }

            /*AAS*/
            search->positionBased = TRUE;
            
            /* Here is all BLAST formating of the main output done */
            
            if(bop->is_xml_output) {
                ValNodePtr other_returns;
                IterationPtr iterp;
                ValNodePtr vnp = search->mask;
                /* Avoid linking masking locations into other_returns, 
                   lest they will be freed */
                search->mask = NULL;
                other_returns = BlastOtherReturnsPrepare(search);
                search->mask = vnp;

                if (head == NULL) {                
                    iterp = BXMLBuildOneIteration(head, other_returns,
                               bop->options->is_ooframe,
                               !bop->options->gapped_calculation, thisPassNum,
                               "No hits found", search->mask);
                } else {
                    iterp = BXMLBuildOneIteration(head, other_returns,
                               bop->options->is_ooframe,
                               !bop->options->gapped_calculation, thisPassNum,
                               (search->posConverged ? "CONVERGED" : NULL), 
                               search->mask);
                }
                
                IterationAsnWrite(iterp, psixp->aip, psixp->atp);
                AsnIoFlush(psixp->aip);
                
                IterationFree(iterp);
                BlastOtherReturnsFree(other_returns);
            } else if(!bop->is_asn1_output){
                PGPFormatMainOutput(head, bop, search, thisPassNum,
                                    lastSeqAligns, numLastSeqAligns, 
                                    seed_seqloc, posSearch->posRepeatSequences);
            }
            
            if (alreadyRecovered) {
                posCheckpointFreeMemory(posSearch, compactSearch->qlength);
                alreadyRecovered = FALSE;
            }
            
            if (ALL_ROUNDS && thisPassNum > 1) {
                posCleanup(posSearch, compactSearch);
            }
        
            if (!search->posConverged && (search->pbp->maxNumPasses == 0 || 
                                          (thisPassNum < search->pbp->maxNumPasses))) {
                if (ALL_ROUNDS) {
                    search->sbp->posMatrix = 
                        CposComputation(posSearch, search, compactSearch, 
                                        head, myargs[28].strvalue, 
                                        (bop->options->isPatternSearch && 
                                         (1== thisPassNum)),
                                        myargs[44].intvalue, 
                                        bop->query_bsp,  
                                        myargs[10].intvalue,
                                        myargs[11].intvalue,
                                        &(search->error_return), 1.0); /*AAS*/
		    posComputationCalled = TRUE;
                    if (search->error_return) {
                       BlastErrorPrint(search->error_return);
                       for (vnp = search->error_return; vnp; vnp = vnp->next) {
                          BlastDestroyErrorMessage((BlastErrorMsgPtr)vnp->data.ptrvalue);
                       }
                       search->error_return = ValNodeFree(search->error_return);
                    }
                } else {
                    search->sbp->posMatrix = 
                        WposComputation(compactSearch, head, search->sbp->posFreqs); 
		    posComputationCalled = TRUE;
                }
#if 0
                /* DEBUG Printing of the matrix */
                {{
                    Int4 i, j;
                    FILE *fd;
                    
                    fd = FileOpen("omatrix.out", "w");
                    for(i = 0; i < bop->query_bsp->length; i++) {
                        for(j = 0; j < 26; j++) {
                            fprintf(fd, "%d ", search->sbp->posMatrix[i][j]);
                        } 
                        fprintf(fd, "\n");
                    } 
                    FileClose(fd);
                }}
#endif
            } else {
                search->sbp->posMatrix = NULL;
            }
            
            if (ALL_ROUNDS && thisPassNum > 1) {
                MemFree(posSearch->posRepeatSequences);
            }
            
            if (!search->posConverged && (0 == search->pbp->maxNumPasses || 
                                          thisPassNum < search->pbp->maxNumPasses)) {
                
                /* Print out pos matrix if necessary */
                if (ALL_ROUNDS && (myargs[38].strvalue != NULL))
                    PGPrintPosMatrix(myargs[38].strvalue, posSearch, 
                                     compactSearch, posComputationCalled);
            }

        	if ((bop->is_asn1_output || bop->aip_out != NULL) && head != NULL)
            		PGPSeqAlignOut(bop, head);
            
        } while (( 0 == search->pbp->maxNumPasses || thisPassNum < (search->pbp->maxNumPasses)) && (! (search->posConverged)));
        
        head = SeqAlignSetFree(head);
        
        /* Here we will print out footer of BLAST output */
        
        if(!bop->is_xml_output && !tabular_output) {
            PGPFormatFooter(bop, search);
        }

        /* PGPOneQueryCleanup */

        if (bop->options->isPatternSearch) {
            bop->seedSearch = MemFree(bop->seedSearch);
            FileClose(bop->patfp);
        }
        
        if (ALL_ROUNDS) {
            posFreeInformation(posSearch);
            MemFree(posSearch);
        }    
        compactSearchDestruct(compactSearch);
        search = BlastSearchBlkDestruct(search);

        /* If these options are set we are not going to proceed with another
           queryes in the input file if any */
        
        if(recoverCheckpoint || myargs[38].strvalue != NULL)
            break;
        
        next_query = FALSE;
        next_query = PGPReadNextQuerySequence(bop);
        if (bop->options->tweak_parameters)
           bop->options->hitlist_size *= 2;   

        if (psixp) {
           PSIXmlReset(psixp);
        }
    } while (next_query);       /* End of main do {} while (); loop */

    ReadDBBioseqFetchDisable();

    
    if(psixp != NULL) {
        PSIXmlClose(psixp);
    }
    
    PGPFreeBlastOptions(bop);
    
    return 0;
}
	

/* Nothing below this line is executable code */

#ifdef PRINT_ONLY_ALIGNMENT
        {{
            AsnIoPtr aip;
            
            if (seqannot)
                seqannot = SeqAnnotFree(seqannot);
            
            seqannot = SeqAnnotNew();
            seqannot->type = 2;
            AddAlignInfoToSeqAnnot(seqannot, 2);
            seqannot->data = head;
            aip = AsnIoOpen("stdout", "w");
            SeqAnnotAsnWrite(seqannot, aip, NULL);
            AsnIoReset(aip);
            AsnIoClose(aip);
            
            seqannot->data = NULL;
            seqannot = SeqAnnotFree(seqannot);
            
            return 0;
        }}
#endif
