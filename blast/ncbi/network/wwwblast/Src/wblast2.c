/* $Id: wblast2.c,v 1.20 2005/04/11 19:14:37 dondosha Exp $
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
* File Name:  $RCSfile: wblast2.c,v $
*
* Initial Creation Date: 10/23/2000
*
* $Revision: 1.20 $
*
* File Description:
*        BLAST 2 Sequences CGI program
*
* $Log: wblast2.c,v $
* Revision 1.20  2005/04/11 19:14:37  dondosha
* Changed blast_form.map to IMG USEMAP for standalone version
*
* Revision 1.19  2005/04/04 15:16:01  dondosha
* If search returned error, display main page with an error and exit
*
* Revision 1.18  2005/03/15 21:36:42  dondosha
* Rolled back last change, because Bioseq retrieved from BLAST database does not have feature information
*
* Revision 1.16  2005/01/18 17:13:36  dondosha
* Set hint to eNone, so blastn is always used if megablast checkbox is not checked; fixed memory leaks; fixed uninitialized variable bug
*
* Revision 1.15  2004/10/26 15:51:35  dondosha
* Removed unsupported matrices
*
* Revision 1.14  2004/10/12 20:49:55  dondosha
* Correction due to change in SummaryReturn structure name and contents
*
* Revision 1.13  2004/08/16 19:39:35  dondosha
* Use CreateMaskByteStore function from txalign.h; fixed bug in masking filtered locations for translated queries
*
* Revision 1.12  2004/08/09 15:40:51  dondosha
* Propagated change in progotype for BLAST_TwoSeqLocSets
*
* Revision 1.11  2004/06/28 16:11:17  dondosha
* Fixed constant name
*
* Revision 1.10  2004/06/24 21:17:07  dondosha
* Boolean argument in ScoreAndEvalueToBuffers changed to Uint1
*
* Revision 1.9  2004/05/14 17:25:07  dondosha
* Allow use of new BLAST engine
*
* Revision 1.8  2003/12/19 18:12:37  coulouri
* fix name collision in aix
*
* Revision 1.7  2003/07/14 18:43:17  dondosha
* Changed Entrez references syntax
*
* Revision 1.6  2003/07/01 20:32:59  dondosha
* Fix in javascript for matrix and strand menus when program is changed
*
* Revision 1.5  2003/07/01 16:33:55  dondosha
* Changed logging; corrected CPU limit handling for Linux
*
* Revision 1.4  2003/05/09 22:12:25  dondosha
* Corrections for Darwin compilation
*
* Revision 1.3  2003/05/09 21:09:04  dondosha
* Removed unused variables
*
* Revision 1.2  2002/12/02 22:40:19  dondosha
* Changed uint64_t to a portable Uint8
*
* Revision 1.1  2002/12/02 18:05:53  dondosha
* Moved from different locations to a common one
*
* Revision 6.34  2002/12/02 17:58:30  dondosha
* No logging or connection thread for standalone WWW server version
*
* Revision 6.33  2002/10/04 21:24:55  dondosha
* 1. Added logging;
* 2. Added user disconnect handling: SIGPIPE, plus a poll thread to detect disconnection while nothing is yet being returned to the browser.
*
* Revision 6.32  2002/09/24 15:48:59  dondosha
* Changed path to standalone executables ftp site
*
* Revision 6.31  2002/09/04 20:13:37  dondosha
* Corrected the order of memory deallocation before exit
*
* Revision 6.30  2002/08/05 21:54:02  dondosha
* Added an #ifdef for QBLAST related stuff
*
* Revision 6.29  2002/06/18 22:18:31  dondosha
* Typo correction
*
* Revision 6.28  2002/05/09 15:38:53  dondosha
* Call BLASTOptionNewEx instead of BLASTOptionNew, so megablast defaults are set in a central place
*
* Revision 6.27  2002/05/07 14:53:56  dondosha
* Copying bioseq was wrong - loses protein link for CDS translation; need only to pack the sequence itself
*
* Revision 6.26  2002/05/02 22:40:18  dondosha
* Copy bioseqs if retrieved from ID1, removing non-residues from sequence
*
* Revision 6.25  2002/04/24 19:24:17  dondosha
* Use QBlast API to retrieve sequences when RID is provided in the URL
*
* Revision 6.24  2002/03/19 23:31:06  dondosha
* Do not increment options->wordsize by 4 for megablast any more
*
* Revision 6.23  2002/03/18 18:08:52  dondosha
* 1. Cleaned up code differentiating between NCBI and 'standalone' versions
* 2. Added DBLEN URL parameter to specify effective database length
* 3. Added RID URL parameter to allow fetching sequences from BLAST Queue
*
* Revision 6.22  2002/01/09 23:26:57  dondosha
* Change effective 12/27: get nr database size possibly using alias file
*
* Revision 6.21  2001/10/03 19:39:11  dondosha
* Tiny correction in formatting of minus strand alignments
*
* Revision 6.20  2001/07/31 16:47:39  dondosha
* Function FastaCheckDna made public for use in Web BLAST
*
* Revision 6.19  2001/07/27 16:06:42  dondosha
* Roll back use of fake subject id, instead use SeqIdFindBest function
*
* Revision 6.18  2001/07/25 21:33:29  dondosha
* Use fake subject Bioseq
*
* Revision 6.17  2001/07/23 20:22:02  dondosha
* Corrected score data computation when query is masked
*
* Revision 6.16  2001/03/14 16:43:11  dondosha
* Minor correction
*
* Revision 6.15  2001/03/02 20:48:55  dondosha
* Cosmetic changes suggested by Roma Tatusov
*
* Revision 6.14  2001/02/26 21:32:09  dondosha
* Do not remove non-alphanumeric characters from sequence in FastaCheckDna
*
* Revision 6.13  2001/02/16 20:21:01  dondosha
* 1. Added a strand option menu
* 2. Fixed bug in sequence id manipulation
*
* Revision 6.12  2001/01/09 21:01:20  dondosha
* For megablast, subtract 4 from wordsize when displaying next page
*
* Revision 6.11  2001/01/04 18:28:28  dondosha
* For tblastx, split seqalign into a linked list of seqaligns
*
* Revision 6.10  2000/12/21 15:57:32  dondosha
* Test chptr for being NULL
*
* Revision 6.9  2000/12/07 17:58:04  dondosha
* Enabled Mega BLAST extension choice for blastn program
*
* Revision 6.8  2000/11/22 19:35:11  dondosha
* Minor fixes of purify errors and memory leaks
*
* Revision 6.7  2000/11/16 23:27:50  dondosha
* Minor corrections from 11/07/2000
*
* Revision 6.6  2000/11/03 22:20:42  shavirin
* Added return value from Main() function.
*
* Revision 6.5  2000/11/03 20:46:41  dondosha
* A few bug fixes
*
* Revision 6.4  2000/11/03 16:39:17  shavirin
* Added alternative formating for Standalone WWW Blast server.
*
* Revision 6.3  2000/11/02 21:45:41  dondosha
* Removed printing content from code - should be done in the cgi script
*
* Revision 6.2  2000/11/02 20:51:00  dondosha
* Significant clean up of the code
*
* Revision 6.1  2000/10/31 19:37:31  dondosha
* Changed revision to 6.1
*
* Revision 1.4  2000/10/31 19:33:49  dondosha
* Changed gif images paths to current directory
*
* Revision 1.2  2000/10/23 21:22:31  dondosha
* Turned on CVS logging
*
* Revision 6.1
* Source code for the Blast 2 Sequences Web page
*
* ==========================================================================
*/

#include <ncbi.h>
#include <tofasta.h>
#include <wwwblast.h>
#include <id1arch.h>
#include <txalign.h>

/* For rlimit stuff. */
#if defined(OS_UNIX)
#include <sys/resource.h>
#endif
#include <signal.h>
#include <stdarg.h>
#ifndef BL2SEQ_STANDALONE
#include <poll.h>
#endif
#include <time.h>

#include <blastpat.h>
#ifndef BL2SEQ_STANDALONE
#include <qblastnet.h>
#endif

#ifndef USE_OLD_BLAST
#include <algo/blast/api/twoseq_api.h>
#endif

#define MY_BLOSUM62 0
#define MY_PAM30 1
#define MY_PAM70 2
#define MY_BLOSUM45 3
#define MY_BLOSUM80 4

#define NR_SIZE_NA 2385885539
#define NR_SIZE_AA 181542687

typedef struct prym{
	Int2		len;
	Int2		color;
	CharPtr 	sym;
	Boolean		noleft, noright;
} Prym, PNTR PrymPtr;


static void JavaScriptFun()
{
   printf("<SCRIPT LANGUAGE=\"Javascript\">\n");
   
   printf("<!-- HIDE\n");
   printf("function chan(a)\n");
   printf("{\n");
   printf("	if (a.value == 'off') {\n");
   printf("		document.bl2.gopen.value=11;\n");
   printf("		document.bl2.gext.value=1;\n");
   printf("		document.bl2.match.value=\"\";\n");
   printf("		document.bl2.msmatch.value=\"\";\n");
   printf("		a.value = 'on';\n");
   printf("	} else {\n");
   printf("		document.bl2.gopen.value=5;\n");
   printf("		document.bl2.gext.value=2;\n");
   printf("		document.bl2.match.value=\"1\";\n");
   printf("		document.bl2.msmatch.value=\"-2\";\n");
   printf("		a.value = 'off';\n");
   printf("	}\n");
   printf("}\n");
   
   printf("function chan_prog(a)\n");
   printf("{\n");
   printf("	if (a.selectedIndex == 0) {\n");
   printf("		document.bl2.gopen.value = 5;\n");
   printf("		document.bl2.gext.value = 2;\n");
   printf("		document.bl2.match.value=1;\n");
   printf("		document.bl2.msmatch.value=\"-2\";\n");
   printf("		document.bl2.word.value=\"11\";\n");
   printf("             document.bl2.matrix.options.length = 1;\n");
   printf("             document.bl2.matrix.options[0] = null;\n");
   printf("             document.bl2.matrix.options[0] = new Option('Not Applicable');\n");
   printf("             document.bl2.matrix.options[0].text='Not Applicable';\n");
   printf("             document.bl2.matrix.options[0].value=-1;\n");
   printf("             document.bl2.strand.options.length = 4;\n");
   printf("             document.bl2.strand.options[0] = null;\n");
   printf("             document.bl2.strand.options[0] = new Option('Both strands');\n");
   printf("             document.bl2.strand.options[0].text='Both strands';\n");
   printf("             document.bl2.strand.options[0].value=3;\n");
   printf("             document.bl2.strand.options[1].text='Top strand';\n");
   printf("             document.bl2.strand.options[1].value=1;\n");
   printf("             document.bl2.strand.options[2].text='Reverse strand';\n");
   printf("             document.bl2.strand.options[2].value=2;\n");
   printf("	} else if (a.selectedIndex >= 1) {\n");
   printf("             document.bl2.matrix.options.length = 6;\n");
   printf("             document.bl2.matrix.options[0] = null;\n");
   printf("             document.bl2.matrix.options[0] = new Option('BLOSUM62');\n");
   printf("             document.bl2.matrix.options[0].selected = true;\n");
   printf("             document.bl2.matrix.options[0].text='BLOSUM62';\n");
   printf("             document.bl2.matrix.options[0].value=0;\n");
   printf("             document.bl2.matrix.options[1].text='PAM30';\n");
   printf("             document.bl2.matrix.options[1].value=1;\n");
   printf("             document.bl2.matrix.options[2].text='PAM70';\n");
   printf("             document.bl2.matrix.options[2].value=2;\n");
   printf("             document.bl2.matrix.options[3].text='BLOSUM45';\n");
   printf("             document.bl2.matrix.options[3].value=3;\n");
   printf("             document.bl2.matrix.options[4].text='BLOSUM80';\n");
   printf("             document.bl2.matrix.options[4].value=4;\n");
   printf("		document.bl2.word.value=\"3\";\n");
   printf("		if (document.bl2.matrix.selectedIndex == 1) {\n");
   printf("			document.bl2.gopen.value = 9;\n");
   printf("			document.bl2.gext.value = 1;\n");
   printf("		} else if (document.bl2.matrix.selectedIndex == 2) {\n");
   printf("			document.bl2.gopen.value = 10;\n");
   printf("			document.bl2.gext.value = 1;\n");
   printf("		} else if (document.bl2.matrix.selectedIndex == 3) {\n");
   printf("			document.bl2.gopen.value = 15;\n");
   printf("			document.bl2.gext.value = 2;\n");
   printf("		} else if (document.bl2.matrix.selectedIndex == 4) {\n");
   printf("			document.bl2.gopen.value =10;\n");
   printf("			document.bl2.gext.value = 1;\n");
   printf("		} else {\n");
   printf("			document.bl2.gopen.value = 11;\n");
   printf("			document.bl2.gext.value = 1;\n");
   printf("		}\n"); 
   printf("		document.bl2.match.value=\"\";\n");
   printf("		document.bl2.msmatch.value=\"\";\n");
   printf("		document.bl2.megablast.checked = 0;\n");
   printf("             document.bl2.strand.options.length = 1;\n");
   printf("             document.bl2.strand.options[0] = null;\n");
   printf("             document.bl2.strand.options[0] = new Option('Not Applicable');\n");
   printf("             document.bl2.strand.options[0].selected = true;\n");
   printf("             document.bl2.strand.options[0].text='Not Applicable';\n");
   printf("             document.bl2.strand.options[0].value=0;\n");
   printf("	}\n");
   printf("}\n");
   
   printf("function update_mtrx(a)\n");
   printf("{\n");
   printf("	if (document.bl2.program.selectedIndex == 0) {\n");
   printf("		return;\n");
   printf("	}\n");
   printf("     document.bl2.word.value = 3;\n");
   printf("	if (a.selectedIndex == 0) {\n");
   printf("		document.bl2.gopen.value = 11;\n");
   printf("		document.bl2.gext.value = 1;\n");
   printf("	} else if (a.selectedIndex == 1) {\n");
   printf("		document.bl2.gopen.value = 9;\n");
   printf("		document.bl2.gext.value = 1;\n");
   printf("	} else if (a.selectedIndex == 2) {\n");
   printf("		document.bl2.gopen.value = 10;\n");
   printf("		document.bl2.gext.value = 1;\n");
   printf("	} else if (a.selectedIndex == 3) {\n");
   printf("		document.bl2.gopen.value = 14;\n");
   printf("		document.bl2.gext.value = 2;\n");
   printf("	} else if (a.selectedIndex == 4) {\n");
   printf("		document.bl2.gopen.value = 10;\n");
   printf("		document.bl2.gext.value = 1;\n");
   printf("	} else if (a.selectedIndex == 5) {\n");
   printf("		document.bl2.gopen.value = 13;\n");
   printf("		document.bl2.gext.value = 2;\n");
   printf("	}\n");
   printf("}\n");

   /* Function clear_sequence() */
   printf("function clear_sequence() {\n");
   printf("    document.bl2.seqfile1.value=''\n");
   printf("    document.bl2.seqfile2.value=''\n");
   printf("    document.bl2.sseq.value=''\n");
   printf("    document.bl2.seq.value=''\n");
   printf("    document.bl2.one.value=''\n");
   printf("    document.bl2.two.value=''\n");
   printf("    document.bl2.to.value=''\n");
   printf("    document.bl2.tto.value=''\n");
   printf("    document.bl2.from.value=''\n");
   printf("    document.bl2.ffrom.value=''\n");
   printf("    document.bl2.seq.focus()\n");
   printf("}\n");
   
   /* Function megablast_update(a) */
   printf("function megablast_update(a)\n");
   printf("{\n");
   printf("	if (a.checked == 0) {\n");
   printf("         document.bl2.word.value = 11;\n");
   printf("         document.bl2.gopen.value = 5;\n");
   printf("         document.bl2.gext.value = 2;\n");
   printf("	} else {\n");
   printf("         if (document.bl2.program.selectedIndex != 0) {\n");
   printf("            document.bl2.program.selectedIndex = 0;\n");     
   printf("            chan_prog(document.bl2.program);\n");
   printf("         }\n");
   printf("         document.bl2.word.value = 28;\n");
   printf("         document.bl2.gopen.value = \"\";\n");
   printf("         document.bl2.gext.value = \"\";\n");
   printf("     }\n");
   printf("}\n");

   printf("// -->\n");
    
   printf("</SCRIPT>\n");
   return;
}

static SeqAnnotPtr tie_next_annot(SeqAnnotPtr head, SeqAnnotPtr next)
{
   SeqAnnotPtr v;
   
   if (head == NULL) {
      return next;
   }
   for (v = head; v->next != NULL; v = v->next) {
      v = v;
   }
   v->next = next;
   return head;
}

static void AbortPage(CharPtr mess)
{
   printf("<TITLE>ERROR</TITLE>\n");
   printf("<h2>\n");
   printf("<img src='images/confused.gif' align=middle>\n");
   printf("%s</h2>\n", mess);
   exit(1);
}

#ifndef BL2SEQ_STANDALONE/* No logging for the standalone WWW Server version */
typedef struct LogInfo {
   CharPtr filename;
   time_t time_start;
   clock_t cpu_time;
   Int4 pid;
   CharPtr program;
   Int4 q_length;
   Int4 s_length;
   CharPtr user_IP;
   Int4 size;
} LogInfo;

static LogInfo loginfo;

static void LogInfoFree()
{
   MemFree(loginfo.filename);
   MemFree(loginfo.program);
   MemFree(loginfo.user_IP);
}

#endif

#ifndef BL2SEQ_STANDALONE/* No logging for the standalone WWW Server version */
void logmsg(int status)
{
    FILE *logfp;

    if ((logfp = FileOpen(loginfo.filename, "a")) != NULL) {
       if (loginfo.time_start == 0) {
          loginfo.cpu_time = clock();
          loginfo.time_start = time(NULL);
          fprintf(logfp, "%ld|%d|%s|%ld|%ld|%s|start\n", loginfo.time_start, 
                  loginfo.pid, loginfo.program, loginfo.q_length,
                  loginfo.s_length, loginfo.user_IP);
       } else {
          struct tm tt;
          char tmstamp[128];
          memcpy(&tt, localtime(&loginfo.time_start), sizeof(tt));
          strftime(tmstamp, sizeof(tmstamp), "%Y-%m-%d %T", &tt);
          loginfo.cpu_time = (clock() - loginfo.cpu_time) / CLOCKS_PER_SEC;
 
          fprintf(logfp, "%ld|%s|%ld|%d|%s|%ld|%ld|%s|done|%d|%ld|%ld\n", 
                  loginfo.time_start, tmstamp, loginfo.cpu_time, loginfo.pid, 
                  loginfo.program, loginfo.q_length, loginfo.s_length, 
                  loginfo.user_IP, status, time(NULL) - loginfo.time_start, 
                  loginfo.size);
       }
       FileClose(logfp);
    }
}
#endif

/* Controls the state of the signal handling thread */
static Boolean run_status;
/* Flag to indicate which signals were received */ 
static Uint8 sigflag = 0;
/* set of signal masks */
static Uint8 wb2_sigmask[64];

#define WBLAST2_SEARCH 0
#define WBLAST2_FORMAT 1
#define WBLAST2_DONE   2

/** precompute individual sigmasks */
void PrepareSigmask(void)
{
   int ii;
   for( ii = 0; ii < 64; ii++ ) {
      wb2_sigmask[ii] = 1 << ii;
   }
}

/** */
static int GetSignal()
{
   int ii;
   for( ii = 0; ii < 64; ii++ ) {
      if( sigflag & wb2_sigmask[ii] ) {
         break;
      }
   }
   
   return ii;
}

/** set signal flag */
static void sighandler(int sig)
{
   sigflag |= wb2_sigmask[sig];
}

static void	Blast2SeqMainPage(CharPtr warning, CharPtr seq1, CharPtr seq2, CharPtr one, CharPtr two, ValNodePtr error, Boolean is_prot, BLAST_OptionsBlkPtr options, Int2 mtrx, Int4 from, Int4 to, Int4 ffrom, Int4 tto, Int2 filter, Int2 pagecount) {
/*****************************************************************
	0 - no sequences
	1 - nucleotide sequences in protein alignment
	2 - non-nucleotide sequences in nucleotide alignment
	4 - BLAST Options error mesg handling
	5 - invalid location
	6 - No alignment found
*****************************************************************/
	ValNodePtr vnp;
	BlastErrorMsgPtr error_msg;
	Int4Ptr dgopen=NULL, dgext=NULL;
        Uint1 prog_number;

        if (options)
           prog_number = BlastGetProgramNumber(options->program_name);
        else 
           prog_number = 1;

	switch (mtrx) {
        case MY_PAM30:
           BlastKarlinGetMatrixValues("PAM30", 
                                      &dgopen, &dgext, NULL, NULL, NULL, NULL);
           break;
        case MY_PAM70:
           BlastKarlinGetMatrixValues("MY_PAM70", 
                                      &dgopen, &dgext, NULL, NULL, NULL, NULL);
           break;
        case MY_BLOSUM80:
           BlastKarlinGetMatrixValues("MY_BLOSUM80", 
                                      &dgopen, &dgext, NULL, NULL, NULL, NULL);
           break;
        case MY_BLOSUM45:
           BlastKarlinGetMatrixValues("MY_BLOSUM45", 
                                      &dgopen, &dgext, NULL, NULL, NULL, NULL);
           break;
        case MY_BLOSUM62:
        default:
           BlastKarlinGetMatrixValues("MY_BLOSUM62", 
                                      &dgopen, &dgext, NULL, NULL, NULL, NULL);
           break;
	}

	printf("<html>\n");
	printf("<head>\n");
	printf("<title>Blast 2 Sequences</title>\n");
        printf("<META NAME=\"keywords\" CONTENT=\"NCBI, BLAST, ORF, Bioinformatics\">\n");
#ifndef BL2SEQ_STANDALONE
        printf("<link rel=\"stylesheet\" href=\"http://www.ncbi.nlm.nih.gov/ncbi.css\">\n");
#endif
	JavaScriptFun();
	printf("</HEAD>\n");

#ifndef BL2SEQ_STANDALONE
	printf("<body bgcolor=\"#f0f0fe\">\n");
        if (options) {
           printf("<table border=0 width=600 cellspacing=0 cellpadding=0>\n");
           printf("<tr valign=center> \n");
           printf("<td width=140><A HREF=\"http://www.ncbi.nlm.nih.gov\"><img src=\"http://www.ncbi.nlm.nih.gov/corehtml/left.GIF\" width=130 height=45 border=0 ALT=\"NCBI logo\"></A></td>\n");
           printf("<td width=460 ><h1>Blast 2 Sequences</h1></td>\n");
           printf("</tr>\n");
           printf("</table>\n");
           
           printf("<!--  the quicklinks bar--> \n");
           printf("<table border=0 width=600 cellspacing=0 cellpadding=1 bgcolor=#003366>\n");
           printf("<tr align=\"center\">\n");
           printf("<td width=100><a href=\"http://www.ncbi.nlm.nih.gov/entrez/query.fcgi?db=PubMed\" class=\"BAR\"><FONT COLOR=#FFFFFF>PubMed</FONT></a></td>\n");
           printf("<td width=100><a href=\"http://www.ncbi.nlm.nih.gov/Entrez/\" class=\"BAR\"><FONT COLOR=#FFFFFF>Entrez</FONT></a></td>\n");
           printf("<td width=100><a href=\"http://www.ncbi.nlm.nih.gov/blast/\" class=\"BAR\"><FONT COLOR=#FFFFFF>BLAST</FONT></a></td>\n");
           printf("<td width=100><a href=\"http://www.ncbi.nlm.nih.gov/omim/\" class=\"BAR\"><FONT COLOR=#FFFFFF>OMIM</FONT></a></td>\n");
           printf("<td width=100><a href=\"http://www.ncbi.nlm.nih.gov/Taxonomy/taxonomyhome.html\" class=\"BAR\"><FONT COLOR=#FFFFFF>Taxonomy</FONT></a></td>\n");
           printf("<td width=100><a href=\"http://www.ncbi.nlm.nih.gov/Structure/\" class=\"BAR\"><FONT COLOR=#FFFFFF>Structure</FONT></a></td>\n");
           printf("</tr></table>\n");
        } else {
           printf("<CENTER><TABLE CELLSPACING=2 CELLPADDING=2 WIDTH=100%%>\n");
           printf("<TR ALIGN=CENTER BGCOLOR=#8dc7cc BGCOLOR=#0000ff><TD><B><I>\n");
           printf("<A HREF=http://www.ncbi.nlm.nih.gov/><font color=#ffffff>NCBI</font></A></I></B></TD>\n");
           printf("<TD><B><I>\n");
           printf("<A HREF=http://www.ncbi.nlm.nih.gov/Entrez><font color=#ffffff>Entrez</font></A>\n");
           printf("</I></B></TD>\n");
           printf("<TD><B><I><A HREF=bl2.html><font color=#ffffff>BLAST 2 sequences</font></A></I></B></TD>\n");
           printf("<TD><B><I><A HREF=http://www.ncbi.nlm.nih.gov/blast><font color=#ffffff>BLAST</font></A></I></B></TD>\n");
           printf("<TD><B><I><A HREF=bl2_seg.html><font color=#ffffff>Example</font></A></I></B></TD>\n");
           printf("<TD><B><I>\n");
           printf("<A HREF=http://www.ncbi.nlm.nih.gov/blast/blast_help.html><font color=#ffffff>Help</font></A>\n");
           printf("</I></B></TD></TR></TABLE></CENTER>\n");
        }
        printf("<H2><CENTER><font color=#0000ff> BLAST 2 SEQUENCES</font></H2></CENTER>\n");
        printf("<FORM NAME=\"bl2\" method=\"Post\" action=\"wblast2.cgi?%d\" enctype=\"multipart/form-data\">\n", pagecount);
#else /* defined BL2SEQ_STANDALONE */
        printf("<BODY BGCOLOR=\"#F0F0FE\" LINK=\"#0000FF\" "
               "VLINK=\"#660099\" ALINK=\"#660099\">\n");
        printf("<map name=img_map>\n");
        printf("<area shape=rect coords=2,1,48,21 "
               "href=\"http://www.ncbi.nlm.nih.gov\">\n");
        printf("<area shape=rect coords=385,1,435,21 "
               "href=\"index.html\">\n");
        printf("<area shape=rect coords=436,1,486,21 "
               "href=\"http://www.ncbi.nlm.nih.gov/Entrez/\">\n");
        printf("<area shape=rect coords=487,1,508,21 "
               "href=\"docs/blast_help.html\">\n");
        printf("</map>\n"); 
        printf("<IMG USEMAP=#img_map WIDTH=509 HEIGHT=22 "
               "SRC=\"images/bl2seq.gif\" BORDER=0 ISMAP></A>\n");
        printf("<FORM NAME=\"bl2\" method=\"POST\" "
#ifdef NCBI_ENTREZ_CLIENT
           "action=\"wblast2_cs.cgi?%d\" "
#else
           "action=\"wblast2.cgi?%d\" "
#endif
           "enctype=\"multipart/form-data\">\n", pagecount);
#endif
        if (warning)
           printf("<h3><font color=#EE0000>WARNING:</font><font color=#0000EE> %s</font></h3>", warning);
        else {
           for (vnp=error; vnp; vnp=vnp->next) {
              error_msg = vnp->data.ptrvalue;
              printf("<h3><font color=#EE0000>WARNING:</font><font color=#0000EE> %s</font></h3>", error_msg->msg);
           }
        }
        if (!options) {
           printf("This tool produces the alignment of two given sequences using "
#ifndef BL2SEQ_STANDALONE
"<A HREF=http://www.ncbi.nlm.nih.gov/blast/newblast.html TARGET=one>BLAST</A> engine for local alignment. <BR>The stand-alone executable for blasting two sequences (bl2seq) can be retrieved from <A HREF=ftp://ncbi.nlm.nih.gov/blast/executables> NCBI ftp site</A><br><b><A HREF=http://www.ncbi.nlm.nih.gov/entrez/query.fcgi?cmd=Retrieve&db=PubMed&list_uids=10339815&dopt=Abstract>Reference:</A></b> Tatiana A. Tatusova, Thomas L. Madden (1999), \"Blast 2 sequences - a new tool for comparing protein and nucleotide sequences\", FEMS Microbiol Lett. 174:247-250 \
<br><BR>"
#else
"BLAST<BR>engine for local alignment.<P>\n"
#endif
                  );
        }
#ifndef BL2SEQ_STANDALONE
        printf("<A HREF=http://www.ncbi.nlm.nih.gov/blast/blast_program.html TARGET=one>Program</A>\n");
#else
        printf("<A HREF=\"docs/blast_program.html\">Program</A>\n");
#endif
        printf("<select name=\"program\" onChange=\"chan_prog(this)\">\n");
        printf("<option%s> blastn\n", (prog_number == blast_type_blastn) ? " SELECTED" : "");
        printf("<option%s> blastp\n", (prog_number == blast_type_blastp) ? " SELECTED" : "");
        printf("<option%s> tblastn\n", (prog_number == blast_type_tblastn) ? " SELECTED" : "");
        printf("<option%s> blastx\n", (prog_number == blast_type_blastx) ? " SELECTED" : "");
        printf("<option%s> tblastx\n", (prog_number == blast_type_tblastx) ? " SELECTED" : "");
        printf("</select>\n");
#ifndef BL2SEQ_STANDALONE
        printf("<A HREF=http://www.ncbi.nlm.nih.gov/blast/options.html#matrix TARGET=one>Matrix</A>\n");
#else
        printf("<A HREF=docs/options.html#matrix>Matrix</A>\n");
#endif
        printf("<select name=\"matrix\" onChange=\"update_mtrx(this)\">\n");
        if (prog_number != blast_type_blastn) {
           printf("<option value=0>BLOSUM62\n");
           printf("<option value=1%s>PAM30\n", (mtrx == 1) ? " SELECTED" : "");
           printf("<option value=2%s>PAM70\n", (mtrx == 2) ? " SELECTED" : "");
           printf("<option value=3%s>BLOSUM45\n", (mtrx == 3) ? " SELECTED" : "");
           printf("<option value=4%s>BLOSUM80\n", (mtrx == 4) ? " SELECTED" : "");
        } else 
           printf("<option> Not Applicable\n");
        printf("</select>\n");

        printf("<HR>\n");
        
        /* -------------------------------------------------- */
        printf("Parameters used in <A HREF="
#ifndef BL2SEQ_STANDALONE   
               "http://www.ncbi.nlm.nih.gov/blast/full_options.html#blastn TARGET=one>"
#else
               "docs/full_options.html#blastn> "
#endif
               "BLASTN </A>program only:<BR>\n");
        if (options) {
            printf("<strong>Match:</strong><INPUT type=text size=8 name=\"match\"%s>", 
                   (is_prot == FALSE) ? " value=1" : "");
            printf("<strong>Mismatch:</strong><INPUT type=text size=8 name=\"msmatch\"%s>",
                   (is_prot == FALSE) ? " value=-2" : "");
            printf("<BR>\n");
            printf("Open gap\n");
            printf("<INPUT type=text size=4 name=\"gopen\" value=%d>\n", 
                   options->gap_open);
            printf("and extension gap\n");
            printf("<INPUT type=text size=4 name=\"gext\" value=%d> penalties<BR>\n", options->gap_extend);
            printf("gap x_dropoff\n");
            printf("<INPUT type=text size=4 name=\"dropoff\" value=%d>\n", options->gap_x_dropoff);
            printf("<A HREF=http://www.ncbi.nlm.nih.gov/blast/newoptions.html#expect>expect</A>\n");
            printf("<INPUT type=text size=4 name=\"expect\" value=%f>\n", options->expect_value);
            printf(" word size\n");
            printf("<INPUT type=text size=4 name=\"word\" value=%d>\n",
                   options->wordsize);
        } else {
            printf("<strong>Reward for a match:</strong><INPUT type=text size=8 name=\"match\"%s>", 
                   (is_prot == FALSE) ? " value=1" : "");
            printf("<strong>Penalty for a mismatch:</strong><INPUT type=text size=8 name=\"msmatch\"%s>",
                   (is_prot == FALSE) ? " value=-2" : "");
            printf("<BR><BR>\n");
            printf("<INPUT type=checkbox NAME=megablast onClick=\"megablast_update(this)\"> Use <a href="
#ifndef BL2SEQ_STANDALONE
                   "http://www.ncbi.nlm.nih.gov/blast/megablast.html"
#else
                   "docs/megablast_readme.html"
#endif
                   ">Mega BLAST</a>&nbsp;&nbsp;&nbsp;");
            printf("Strand option <select name=\"strand\">\n");
            printf("<option value=3 SELECTED> Both strands\n");
            printf("<option value=1> Forward strand\n");
            printf("<option value=2> Reverse strand\n");
            printf("</select><HR>\n");

            printf("Open gap\n");
            printf("<INPUT type=text size=4 name=\"gopen\" value=5>\n");
            printf("and extension gap\n");
            printf("<INPUT type=text size=4 name=\"gext\" value=2> penalties<BR>\n");
            printf("gap x_dropoff\n");
            printf("<INPUT type=text size=4 name=\"dropoff\" value=50>\n");
            printf("<A HREF=http://www.ncbi.nlm.nih.gov/blast/newoptions.html#expect>expect</A>\n");
            printf("<INPUT type=text size=4 name=\"expect\" value=10.0>\n");
            printf(" word size\n");
            printf("<INPUT type=text size=4 name=\"word\" value=11>\n");
        }
        printf("<a href="
#ifndef BL2SEQ_STANDALONE
               "http://www.ncbi.nlm.nih.gov/blast"
#else
               "docs"
#endif
               "/newoptions.html#filter>Filter</a>\n");
        printf("<INPUT TYPE=checkbox NAME=Filter VALUE=%d", filter);
        if (filter == 1) {
           printf(" CHECKED>\n");
        } else {
           printf(">\n");
        }
        printf("<INPUT TYPE=\"submit\" VALUE=\"Align\">\n");
        if (options)
           printf("<INPUT TYPE=\"reset\" VALUE=\"Clear Input\">\n");
        
        printf("<HR>\n");
#ifdef NCBI_ENTREZ_CLIENT
        printf("<font color=ff0000>Sequence 1</font> Enter accession or GI <INPUT type=text size=8 name=\"one\"");
        if (one) {
           printf("value=%s>\n", one);
        } else {
           printf(">\n");
        }
        printf("or download from file <INPUT type=file name=\"seqfile1\">");
#else
        printf("Download sequence from file <INPUT type=file name=\"seqfile1\">");
#endif

        printf("<BR>or sequence in FASTA format <font color=ff0000>from:<INPUT type=text size=8 name=\"from\" value=%d>to:<INPUT type=text size=8 name=\"to\" value=%d></font><BR>\n", from, to);
        
        printf("<textarea name=\"seq\" rows=6 cols=60>");
        if (seq1) {
           if (*seq1 == '>') {
              printf("&gt;");
              seq1++;
           }
           printf("%s</textarea>\n", seq1);
        } else {
           printf("</textarea>\n");
        }
        printf("<BR>\n");
#ifdef NCBI_ENTREZ_CLIENT
        printf("<font color=ff0000>Sequence 2</font> Enter accession or GI <INPUT type=text size=8 name=\"two\"");
        if (two) {
           printf("value=%s>\n", two);
        } else {
           printf(">\n");
        }
        printf("or download from file <INPUT type=file name=\"seqfile2\">");
#else
        printf("Download sequence from file <INPUT type=file name=\"seqfile2\">");
#endif

        printf("<BR>or sequence in FASTA format <font color=ff0000>from:<INPUT type=text size=8 name=\"ffrom\" value=%d>to:<INPUT type=text size=8 name=\"tto\" value=%d></font><BR>\n", ffrom, tto);
        printf("<textarea name=\"sseq\" rows=6 cols=60>");
        if (seq2) {
           if (*seq2 == '>') {
              printf("&gt;");
              seq2++;
           }
           printf("%s</textarea>\n", seq2);
        } else {
           printf("</textarea>\n");
        }

        printf("<BR>\n");
        printf("<INPUT TYPE=\"submit\" VALUE=\"Align\">\n");
        printf("<INPUT TYPE=\"reset\" VALUE=\"Clear Input\">\n");
        printf("<INPUT TYPE=hidden name=\"page\" value=\"%d\">\n", pagecount+1);
        if (options)
            printf("<INPUT TYPE=hidden name=\"program\" value=\"%s\">\n",
                   options->program_name);
        else
            printf("<INPUT TYPE=hidden name=\"program\" value=\"blastn\">\n");
	
        printf("</form>\n");
        printf("<HR>\n");
        printf("<ADDRESS> Comments and suggestions to");
        printf(" <A HREF=\"mailto:blast-help@ncbi.nlm.nih.gov\">blast-help@ncbi.nlm.nih.gov</A>\n");
        printf("<BR>\n");
        printf("</body>\n");
        printf("</html>\n");
        fflush(stdout);

        options = BLASTOptionDelete(options);

        exit (0);
}

static void CreateJavaHeadHTML(BioseqPtr query_bsp, BioseqPtr subject_bsp, Int4 from, Int4 to, Int4 ffrom, Int4 tto, Int4 len1, Int4 len2, CharPtr progname)
{
	static Char tmp[128];
		
	printf("<HTML>\n");
	printf("<head>\n");
	printf("<title>Blast Result</title>\n\n");
        printf("<META NAME=\"keywords\" CONTENT=\"NCBI, BLAST, ORF, Bioinformatics\">\n");
#ifndef BL2SEQ_STANDALONE
        printf("<link rel=\"stylesheet\" href=\"http://www.ncbi.nlm.nih.gov/ncbi.css\">\n");
#endif
	JavaScriptFun();
	printf("</head>\n");
#ifndef BL2SEQ_STANDALONE
	printf("<BODY bgcolor=\"#f0f0fe\">\n");
        printf("<table border=0 width=600 cellspacing=0 cellpadding=0>\n");
        printf("<tr valign=center> \n");
        printf("<td width=140><A HREF=\"http://www.ncbi.nlm.nih.gov\"><img src=\"http://www.ncbi.nlm.nih.gov/corehtml/left.GIF\" width=130 height=45 border=0 ALT=\"NCBI logo\"></A></td>\n");
        printf("<td width=460 ><h1>Blast 2 Sequences results</h1></td>\n");
        printf("</tr>\n");
        printf("</table>\n");
        printf("<!--  the quicklinks bar--> \n");
        printf("<table border=0 width=600 cellspacing=0 cellpadding=1 bgcolor=#003366>\n");
        printf("<tr align=\"center\">\n");
        printf("<td width=100><a href=\"http://www.ncbi.nlm.nih.gov/entrez/query.fcgi?db=PubMed\" class=\"BAR\"><FONT COLOR=#FFFFFF>PubMed</FONT></a></td>\n");
        printf("<td width=100><a href=\"http://www.ncbi.nlm.nih.gov/Entrez/\" class=\"BAR\"><FONT COLOR=#FFFFFF>Entrez</FONT></a></td>\n");
        printf("<td width=100><a href=\"http://www.ncbi.nlm.nih.gov/blast/\" class=\"BAR\"><FONT COLOR=#FFFFFF>BLAST</FONT></a></td>\n");
        printf("<td width=100><a href=\"http://www.ncbi.nlm.nih.gov/omim/\" class=\"BAR\"><FONT COLOR=#FFFFFF>OMIM</FONT></a></td>\n");
        printf("<td width=100><a href=\"http://www.ncbi.nlm.nih.gov/Taxonomy/taxonomyhome.html\" class=\"BAR\"><FONT COLOR=#FFFFFF>Taxonomy</FONT></a></td>\n");
        printf("<td width=100><a href=\"http://www.ncbi.nlm.nih.gov/Structure/\" class=\"BAR\"><FONT COLOR=#FFFFFF>Structure</FONT></a></td>\n");
        printf("</tr></TABLE><p>\n");
#else
    printf("<BODY BGCOLOR=\"#F0F0FE\" LINK=\"#0000FF\" "
           "VLINK=\"#660099\" ALINK=\"#660099\">\n");
    
    printf("<A HREF=\"blast_form.map\">"
           "<IMG SRC=\"images/bl2seq.gif\" BORDER=0 ISMAP></A>\n<P>");    
#endif        
        init_buff();
   	sprintf(tmp, "BLAST 2 sequences results version %s", progname);
        BlastPrintVersionInfo(tmp, TRUE, stdout);
	free_buff();

	return;
}

 
static void CreateTailHTML()
{
    printf("<br>\n");
    printf("</BODY>\n");
    printf("</HTML>\n");
}

static int NumToGun1(int n)
{
    return (n < 10) ? n+'0' : (n < 36) ? n-10+'A' : n-36+'a';
}

static CharPtr NumToGun(int n)
{
    static Char str[4];
    
    str[0] = NumToGun1(n/62);
    str[1] = NumToGun1(n%62);
    return str;
}

static int get_int(CharPtr val)
{
	CharPtr ch=NULL;
	
	if (val != NULL) {
		ch = val + 1;
		while (isspace(*ch)) {
			ch++;
		}
		if (*ch == '\0') {
			return 0;
		}
		return (atoi(ch));
	}
	return 0;
}
static FloatHi get_double(CharPtr val)
{
	CharPtr ch=NULL;
	
	if (val != NULL) {
		ch = val + 1;
		while (isspace(*ch)) {
			ch++;
		}
		if (*ch == '\0') {
			return 0;
		}
		return (atof(ch));
	}
	return 0;
}

static CharPtr get_char(CharPtr val)
{
	CharPtr ch=NULL;
	
	if (val != NULL) {
		ch = val + 1;
		while (isspace(*ch)) {
			ch++;
		}
		if (*ch == '\0') {
			ch = NULL;
		}
	}
	return ch;
}

static Boolean sum_for_DenseDiag(DenseDiagPtr ddp, AlignSumPtr asp)
/* special for blast 2 sequences */
{
	SeqInt msi, tsi;
	SeqLoc sl;
	Int2 i;
	Int4 x1, x2, y1, y2;
	Uint1 m_res, t_res;
	SeqPortPtr m_spp, t_spp;

	if(ddp == NULL || asp == NULL)
		return FALSE;
	x1 = ddp->starts [0];
	y1 = ddp->starts [1];
	x2 = x1 + ddp->len - 1;
	y2 = y1 + ddp->len - 1;
	msi.id = ddp->id;
	msi.from = x1;
	msi.to = x2;
	if(ddp->strands != NULL)
		msi.strand = (ddp->strands == NULL) ? 0 : ddp->strands[0];
	sl.choice = SEQLOC_INT;
	sl.data.ptrvalue = &msi;
	m_spp = SeqPortNewByLoc(&sl, 
		(asp->is_aa) ? Seq_code_ncbieaa : Seq_code_iupacna);
	
	tsi.id = ddp->id;
	tsi.from = y1;
	tsi.to = y2;
	if(ddp->strands != NULL)
		tsi.strand = (ddp->strands == NULL) ? 0 : ddp->strands[1];
	sl.choice = SEQLOC_INT;
	sl.data.ptrvalue = &tsi;

	t_spp = SeqPortNewByLoc(&sl, 
		(asp->is_aa) ? Seq_code_ncbieaa : Seq_code_iupacna);

	for(i = 0; i < ddp->len; ++i) {
		m_res = SeqPortGetResidue(m_spp);
		if (!IS_residue(m_res)) {
			continue;
		}
		t_res = SeqPortGetResidue(t_spp);
		if (!IS_residue(t_res)) {
			continue;
		}
		if(m_res == t_res) {
			++(asp->identical);
		} else if(asp->matrix != NULL && asp->is_aa) {
			if(asp->matrix[m_res][t_res] > 0)
				++(asp->positive);
		}
	}

        if (!asp->is_aa && ddp->strands) {
           asp->m_strand = ddp->strands[0];
           asp->t_strand = ddp->strands[1];
        }
	asp->totlen = ddp->len;

	SeqPortFree(m_spp);
	SeqPortFree(t_spp);
	return TRUE;
}

static Boolean sum_for_DenseSeg(DenseSegPtr dsp, AlignSumPtr asp)
{
/* special for blast 2 sequences */
	SeqInt msi, tsi;
	SeqLoc sl;
	Int2 i;
	Int4 j, x1, x2, y1, y2;
	Uint1 m_res, t_res;
	SeqPortPtr m_spp, t_spp;

	if(dsp == NULL || asp == NULL)
		return FALSE;
	for(i = 0; i < dsp->numseg; ++i) {
		x1 = dsp->starts[2*i];
		y1 = dsp->starts[2*i+1];
		x2 = x1 + dsp->lens[i] -1;
		y2 = y1 + dsp->lens[i] -1;
		msi.id = dsp->ids;
		msi.from = x1;
		msi.to = x2;
		msi.strand = (dsp->strands == NULL) ? 0 : dsp->strands[2*i];
                if (x1 != -1) {
                   sl.choice = SEQLOC_INT;
                   sl.data.ptrvalue = &msi;
                   m_spp = SeqPortNewByLoc(&sl, (asp->is_aa) ? Seq_code_ncbieaa
                                           : Seq_code_iupacna);
                } else 
                   m_spp = NULL;
		tsi.id = dsp->ids->next;
		tsi.from = y1;
		tsi.to = y2;
		tsi.strand = (dsp->strands == NULL) ? 0 :
                   dsp->strands[2*i+1];
                if (y1 != -1) {
                   sl.choice = SEQLOC_INT;
                   sl.data.ptrvalue = &tsi;
                   t_spp = SeqPortNewByLoc(&sl, (asp->is_aa) ? Seq_code_ncbieaa : Seq_code_iupacna);
                } else 
                   t_spp = NULL;
		if (x1 == -1 || y1 == -1) {
			asp->gaps += dsp->lens[i];
		} else {
			for(j = 0; j < dsp->lens[i]; ++j) {
				m_res = SeqPortGetResidue(m_spp);
				if (!IS_residue(m_res)) {
					continue;
				}
				t_res = SeqPortGetResidue(t_spp);
				if (!IS_residue(m_res)) {
					continue;
				}
				if(m_res == t_res) {
					++(asp->identical);
				} else if(asp->matrix != NULL && asp->is_aa) {
					if(asp->matrix[m_res][t_res] >0)
						++(asp->positive);
				}
			}
		}
		asp->totlen += dsp->lens[i];
                SeqPortFree(m_spp);
                SeqPortFree(t_spp);
	}
        asp->m_strand = msi.strand;
        asp->t_strand = tsi.strand;
        
	return TRUE;
}

/** Frees the part of the byte store list that has not been used for replacement
 * of Bioseq data.
 */
static void 
ByteStoreListFreeUnused (ValNodePtr bs_list, Uint1 frame_to_skip)
{
   ByteStorePtr bsp;
   ValNodePtr bs_next = NULL;
   Boolean skip_done = FALSE;

   for( ; bs_list; bs_list = bs_next) {
      bs_next = bs_list->next;
      if (skip_done || bs_list->choice != frame_to_skip) {
         bsp = (ByteStorePtr) bs_list->data.ptrvalue;
         if(bsp != NULL)
            BSFree(bsp);
      } else {
         skip_done = TRUE;
      }
      MemFree(bs_list);
   }
}

/*******************************************************/

static void PrintOutScore(SeqAlignPtr sap, Boolean is_aa, Int4Ptr PNTR matrix, ValNodePtr mask_loc)
{
	Int4 number, score;
	Nlm_FloatHi bit_score, evalue; 
   CharPtr eval_buff_ptr;
	Char eval_buff[10], bit_score_buff[10];
	AlignSumPtr asp;
	Int2 percent_identical, percent_positive;
   ValNodePtr bs_list;
   BioseqPtr q_bsp;
   Int4 frame;
   Uint1 code=0;
   Uint1 repr=0;
   ByteStorePtr seq_data=NULL;
   Boolean local_matrix = FALSE;     
   Boolean seq_data_replaced = FALSE;

	printf("<pre>\n");
	GetScoreAndEvalue(sap, &score, &bit_score, &evalue, &number);
   
   /* Evalue buffer for printing may be shifted if a digit is knocked off
      before e. */
	eval_buff_ptr = eval_buff;
   ScoreAndEvalueToBuffers(bit_score, evalue, bit_score_buff, 
                           &eval_buff_ptr, TX_KNOCK_OFF_ALLOWED);

	if (number == 1) {
		printf("Score = %s bits (%ld), Expect = %s<BR>", 
			bit_score_buff, (long) score, eval_buff_ptr);
	} else {
		printf("Score = %s bits (%ld), Expect(%ld) = %s<BR>", 
			bit_score_buff, (long) score, (long) number, eval_buff_ptr);
	}
	asp = MemNew(sizeof(AlignSum));
	asp->matrix = NULL;
   if (is_aa) {
      if (matrix != NULL)
         asp->matrix = matrix;
      else {
         asp->matrix = load_default_matrix();
         local_matrix = TRUE;
      }
	}
	asp->is_aa = is_aa;
	asp->totlen = 0;
	asp->positive = 0;
	asp->identical = 0;
	asp->gaps = 0;
   asp->master_sip = SeqIdDup(TxGetQueryIdFromSeqAlign(sap));
   asp->target_sip = TxGetSubjectIdFromSeqAlign(sap);

   if (mask_loc) { /* Mask the query sequence */
      bs_list = CreateMaskByteStore (mask_loc);
      q_bsp = BioseqLockById(asp->master_sip);
      if (ISA_na(q_bsp->mol) && sap->segtype == SAS_STD) {
         StdSegPtr ssp = (StdSegPtr) sap->segs;
         frame = SeqLocStart(ssp->loc);
         if (SeqLocStrand(ssp->loc) == Seq_strand_minus) {
            frame += SeqLocLen(ssp->loc);
            frame = -(1+(q_bsp->length - frame)%3);
         } else {
            frame = (1+frame%3);
         }
      } else
         frame = 0;
      repr = q_bsp->repr;
      seq_data = q_bsp->seq_data;
      code = q_bsp->seq_data_type;
      seq_data_replaced = replace_bytestore_data(q_bsp, bs_list, (Uint1)frame);
      if (!seq_data_replaced) {
         q_bsp->repr = repr;
         q_bsp->seq_data = seq_data;
         q_bsp->seq_data_type = code;
      }
      BioseqUnlock(q_bsp);
      ByteStoreListFreeUnused(bs_list, (Uint1)frame);
   }
   
   find_score_in_align(sap, 1, asp);

   /* Restore the unmasked sequence data. */
   if (seq_data_replaced) {
      q_bsp = BioseqLockById(asp->master_sip);
      q_bsp->repr = repr;
      q_bsp->seq_data = seq_data;
      q_bsp->seq_data_type = code;
   }

	if (asp && asp->totlen > 0) {
           Char sign1, sign2;           

	   percent_identical = (100*asp->identical)/ (asp->totlen);
	   percent_positive = (100*asp->positive)/ (asp->totlen);
           if (asp->is_aa)
              printf("Identities = %ld/%ld (%ld%%), Positives = %ld/%ld (%ld%%)", (long) asp->identical, (long) asp->totlen, percent_identical,
                     (long) (asp->positive+asp->identical), (long)
                     asp->totlen, (percent_identical+percent_positive));
           else
              printf("Identities = %ld/%ld (%ld%%)", (long) asp->identical, (long) asp->totlen, percent_identical);
	   if (asp->gaps > 0) {
		printf(", Gaps = %ld/%ld (%ld%%)<BR>", (long) asp->gaps, 
			(long) asp->totlen, 
				(long) (100*asp->gaps)/(asp->totlen));
	   } else {
	   		printf("<BR>");
	   }
           
           sign1 = sign2 = '0';
           if (asp->m_frame > 0) sign1 = '+';
           else if (asp->m_frame < 0) sign1 = '-';
           if (asp->t_frame > 0) sign2 = '+';
           else if (asp->t_frame < 0) sign2 = '-';
           if (sign1 != '0' && sign2 != '0')
                 printf(" Frame = %c%d / %c%d", sign1, ABS(asp->m_frame),
                        sign2, ABS(asp->t_frame));
           else if (sign1 != '0')
              printf(" Frame = %c%d", sign1, ABS(asp->m_frame));
           else if (sign2 != '0')
              printf(" Frame = %c%d", sign2, ABS(asp->t_frame));
           else if (asp->m_strand != Seq_strand_unknown && asp->t_strand != Seq_strand_unknown) {
              if (asp->m_strand != asp->t_strand)
                 printf(" Strand = Plus / Minus");
              else
                 printf(" Strand = Plus / Plus");
           }
           printf("<BR><BR>");
	}
	printf("</pre>\n");		
   
   SeqIdFree(asp->master_sip);
   if (local_matrix)
      free_default_matrix(asp->matrix);
   MemFree(asp);
}

static Int2 CreateRectAlign(SeqAlignPtr sap, PrymPtr PNTR rect, PrymPtr PNTR rectY, FloatHi scalex, FloatHi scaley, Int4 l1, Int4 l2, Int2 color, Int4 from, Int4 ffrom, Int4 to, Int4 tto)
{
	DenseDiagPtr ddp;
	DenseSegPtr  dsp;
	Int4         index1;
	Int4         x1, x2, y1, y2;
	Int2         k, sx, sy;
	Boolean first = TRUE;

	if (sap == NULL) {
		return 0;
	}
	k = 0;

	if (sap->segtype == SAS_DENDIAG) {
		ddp = sap->segs;
		x2 = y2 = 0;
		first = TRUE;
	    while (ddp != NULL) {
			x1 = ddp->starts [0];
			y1 = ddp->starts [1];
			sx = ddp->strands[0];
			sy = ddp->strands[1];
			if (first) {
				if (sx == Seq_strand_plus) {
					x1 = ddp->starts[0] - from + 1;
				} else {
					x1 = to - (ddp->starts[0] +  ddp->len) + 1;
				}
				if (sy == Seq_strand_plus) {
					y1 = ddp->starts[1] - from + 1;
				} else {
					y1 = tto - (ddp->starts[1] +  ddp->len) + 1;
				}
				rect[k] = MemNew(sizeof(Prym));
				rect[k]->color = -1;
				rect[k]->len = 0;
				rectY[k] = MemNew(sizeof(Prym));
				rectY[k]->color = -1;
				rectY[k]->len = 0;
			     if (x1 < y1) {
				    rect[k]->len = (y1-x1)*scalex + 0.5;
			     } else {
				    rectY[k]->len = (x1-y1)*scalex + 0.5;
			     }
				 first = FALSE;
				k++;
			 }
			rect[k] = MemNew(sizeof(Prym));
			rect[k]->color = 0;
			rect[k]->len = (x1 - x2)*scalex + 0.5;
			rectY[k] = MemNew(sizeof(Prym));
			rectY[k]->color = 0;
			rectY[k]->len = (y1 - y2)*scaley + 0.5;
			k++;
		    x2 = ddp->starts [0] + ddp->len;
		    y2 = ddp->starts [1] + ddp->len;
			rect[k] = MemNew(sizeof(Prym));
			rect[k]->color = color;
			rect[k]->len = (x2 - x1)*scalex + 0.5;
			rectY[k] = MemNew(sizeof(Prym));
			rectY[k]->color = color;
			rectY[k]->len = (y2 - y1)*scaley + 0.5;
			k++;

		      ddp = ddp->next;
	    }
		rect[k] = MemNew(sizeof(Prym));
		rect[k]->color = 0;
		if (sx != Seq_strand_minus) {
			rect[k]->len = (to - x2)*scalex + 0.5;
		} else {
			rect[k]->len = (x1 - from +1)*scalex + 0.5;
		}
		rectY[k] = MemNew(sizeof(Prym));
		rectY[k]->color = 0;
		if (sy != Seq_strand_minus) {
			rectY[k]->len = (tto - y2)*scaley + 0.5;
		} else {
			rectY[k]->len = (y1 - ffrom +1)*scaley + 0.5;
		}
		k++;
	} else if (sap->segtype == SAS_DENSEG) {
		dsp = sap->segs;
		k = 0;
		for (index1=0; index1 < dsp->numseg; index1++) {
			x1 = dsp->starts[2*index1];
			y1 = dsp->starts[2*index1+1];
			sx = dsp->strands[2*index1];
			sy = dsp->strands[2*index1+1];
			x2 = x1 + dsp->lens[index1];
			y2 = y1 + dsp->lens[index1];
			if (index1 == 0) {
				if (sx == Seq_strand_minus) {
					x1 = to - (dsp->starts[0] +  dsp->lens[0]) + 1;
				} else {
					x1 = dsp->starts[0] - from + 1;
				}
				if (sy == Seq_strand_minus) {
					y1 = tto - (dsp->starts[1] + dsp->lens[0]) + 1;
				} else {
					y1 = dsp->starts[1] - ffrom + 1;
				}
				rect[k] = MemNew(sizeof(Prym));
				rect[k]->color = -1;
				rect[k]->len = 0;
				rectY[k] = MemNew(sizeof(Prym));
				rectY[k]->color = -1;
				rectY[k]->len = 0;
				if (x1 < y1) {
					rect[k]->len = (y1-x1)*scalex + 0.5;
				} else {
				    rectY[k]->len = (x1-y1)*scalex + 0.5;
				}
				k++;
				rect[k] = MemNew(sizeof(Prym));
				rect[k]->color = 0;
				rect[k]->len = x1*scalex + 0.5;
				rectY[k] = MemNew(sizeof(Prym));
				rectY[k]->color = 0;
				rectY[k]->len = y1*scalex + 0.5;
				k++;
			}
			rect[k] = MemNew(sizeof(Prym));
			if (x1 == -1) {
				rect[k]->color = 3;
			} else {
				rect[k]->color = color;
			}
			rect[k]->len = dsp->lens[index1]*scalex + 0.5;
			rectY[k] = MemNew(sizeof(Prym));
			if (y1 == -1) {
				rectY[k]->color = 3;
			} else {
				rectY[k]->color = color;
			}
			rectY[k]->len = dsp->lens[index1]*scalex + 0.5;
			k++;
		}
		rect[k] = MemNew(sizeof(Prym));
		rect[k]->color = 0;
		if (sx == Seq_strand_minus) {
			rect[k]->len = (x1 - from +1)*scalex + 0.5;
		} else {
			rect[k]->len = (to - x2)*scalex + 0.5;
		}
		rectY[k] = MemNew(sizeof(Prym));
		rectY[k]->color = 0;
		if (sy == Seq_strand_minus) {
			rectY[k]->len = (y1 - ffrom +1)*scaley + 0.5;
		} else {
			rectY[k]->len = (tto - y2)*scaley + 0.5;
		}
		k++;
	} else if (sap->segtype == SAS_STD) {
           StdSegPtr ssp = sap->segs;
           SeqLocPtr slp;

           while (ssp) {
              slp = ssp->loc;
              if (!slp || !slp->next) {
                 ssp = ssp->next;
                 continue;
              }
              if (slp->choice == SEQLOC_EMPTY) {
                 x1 = x2 = -1;
                 sx = Seq_strand_unknown;
              } else {
                 x1 = SeqLocStart(slp);
                 sx = SeqLocStrand(slp);
                 x2 = SeqLocStop(slp) + 1;
              }
              if (slp->next->choice == SEQLOC_EMPTY) {
                 y1 = y2 = -1;
                 sy = Seq_strand_unknown;
              } else {
                 y1 = SeqLocStart(slp->next);
                 sy = SeqLocStrand(slp->next);
                 y2 = SeqLocStop(slp->next) + 1;
              }

              if (first) { 
                 if (sx == Seq_strand_plus) {
                    x1 -= from - 1;
                 } else if (sx == Seq_strand_minus) {
                    x1 = to - x2 + 1;
                 }
                 if (sy == Seq_strand_plus) {
                    y1 -= ffrom - 1;
                 } else if (sy == Seq_strand_minus){
                    y1 = tto - y2 + 1;
                 }

                 rect[k] = MemNew(sizeof(Prym));
                 rect[k]->color = -1;
                 rect[k]->len = 0;
                 rectY[k] = MemNew(sizeof(Prym));
                 rectY[k]->color = -1;
                 rectY[k]->len = 0;
                 if (y1*scaley>x1*scalex)
                    rect[k]->len = y1*scaley - x1*scalex + 0.5;
                 else
                    rectY[k]->len = x1*scalex - y1*scaley + 0.5;
                 first = FALSE;
                 k++;
                 rect[k] = MemNew(sizeof(Prym));
                 rect[k]->color = 0;
                 rect[k]->len = x1*scalex + 0.5;
                 rectY[k] = MemNew(sizeof(Prym));
                 rectY[k]->color = 0;
                 rectY[k]->len = y1*scaley + 0.5;
                 k++;
              }
              rect[k] = MemNew(sizeof(Prym));
              if (x1 == -1) {
                 rect[k]->color = 3;
                 rect[k]->len = SeqLocLen(slp->next)*scaley + 0.5;
              } else {
                 rect[k]->color = color;
                 rect[k]->len = SeqLocLen(slp)*scalex + 0.5;
              }

              rectY[k] = MemNew(sizeof(Prym));
              if (y1 == -1) {
                 rectY[k]->color = 3;
                 rectY[k]->len = SeqLocLen(slp)*scalex + 0.5;
              } else {
                 rectY[k]->color = color;
                 rectY[k]->len = SeqLocLen(slp->next)*scaley + 0.5;
              }
              k++;
              ssp = ssp->next;
           }
           rect[k] = MemNew(sizeof(Prym));
           rect[k]->color = 0;
           if (sx != Seq_strand_minus) {
              rect[k]->len = (to - x2)*scalex + 0.5;
           } else {
              rect[k]->len = (x1 - from +1)*scalex + 0.5;
           }
           rectY[k] = MemNew(sizeof(Prym));
           rectY[k]->color = 0;
           if (sy != Seq_strand_minus) {
              rectY[k]->len = (tto - y2)*scaley + 0.5;
           } else {
              rectY[k]->len = (y1 - ffrom +1)*scaley + 0.5;
           }
           k++;
        }

	return k;
}

static void DrawRectAlign(PrymPtr PNTR rect, Int2 k, Int2 color, Int2 height, Int2 index)
{
	Int2 l;
	
	for (l=0; l < k; l++) {
		if (rect[l]->len <= 0) {
			if (l == 0 && rect[1] && rect[1]->len == 0) {
				printf("<img\n height=%d width=1 src='images/00.gif'>", height);
			}
                        MemFree(rect[l]);
			continue;
		}
		if (rect[l]->len-1 == 0) {
			rect[l]->len++;
		}
		if (rect[l]->color == -1) {
			printf("<img\n height=2 src='images/0.gif'");
			printf(" width=%d>", rect[l]->len);
		} else if (rect[l]->color == 3) {
			if (rect[l+1]->color == rect[l]->color) {
				printf("<img\n height=5 src='images/3.gif'");
				printf(" width=%d>", rect[l]->len);
			} else {
				printf("<img\n height=5 src='images/3.gif'");
				printf(" width=%d>", rect[l]->len-1);
				printf("<img\n height=%d width=1 src='images/00.gif'>", height);
			}
		} else if (rect[l]->color == 0) {
			if (rect[l+1] &&  rect[l+1]->color != -1) {
				printf("<img\n height=2 src='images/00.gif'");
				printf(" width=%d>", rect[l]->len-1);
				printf("<img\n height=%d width=1 src='images/00.gif'>", height);
			} else {
				printf("<img\n height=2 src='images/00.gif'");
				printf(" width=%d>", rect[l]->len);
			}
		} else if (rect[l]->len > 0) {
			if (rect[l+1]->color == rect[l]->color) {
				if (index != -1) {
				printf("<A HREF=#%d><img height=%d src='images/%d.gif' BORDER=0", 
													index, height, color);
					printf(" width=%d></a>", rect[l]->len);
				} else {
					printf("<img height=%d src='images/%d.gif'", 
													height, color);
					printf(" width=%d>", rect[l]->len);
				}
			} else {
				if (index != -1) {
				printf("<A HREF=#%d><img height=%d src='images/%d.gif' BORDER=0",
						index, height, color);
					printf(" width=%d></a>", rect[l]->len-1);
				} else {
					printf("<img height=%d src='images/%d.gif'",
						height, color);
					printf(" width=%d>", rect[l]->len-1);
				}
				printf("<img\n height=%d width=1 src='images/00.gif'>", height);
			}
		}
                MemFree(rect[l]);
	}
}

static void PrintRectAlign(PrymPtr PNTR rect, Int2 k, Int2 color, Int2 height, Int2 index)
{
	Int2 l;
	
	for (l=0; l < k; l++) {
		if (rect[l]->len <= 0) {
			continue;
		}
		if (rect[l]->len-1 == 0) {
			rect[l]->len++;
		}
		if (rect[l]->color == -1) {
			printf("images/0.gif");
			printf(" width=%d>", rect[l]->len);
		} else if (rect[l]->color == 3) {
			if (rect[l+1]->color == rect[l]->color) {
				printf("images/3.gif");
				printf(" width=%d", rect[l]->len);
			} else {
				printf("images/3.gif");
				printf(" width=%d>", rect[l]->len-1);
			}
		} else if (rect[l]->color == 0) {
			if (rect[l+1] &&  rect[l+1]->color != -1) {
				printf(" width=%d>", rect[l]->len-1);
			} else {
				printf(" width=%d>", rect[l]->len);
			}
		} else if (rect[l]->len > 0) {
			if (rect[l+1]->color == rect[l]->color) {
				if (index != -1) {
					printf(" RECT:width=%d></a>", rect[l]->len);
				} else {
					printf(" RECT:width=%d>", rect[l]->len);
				}
			} else {
				if (index != -1) {
					printf(" RECT width=%d></a>", rect[l]->len-1);
				} else {
					printf("RECT width=%d>", rect[l]->len-1);
				}
			}
		}
	}
}

#define LOCAL_BUFLEN 255
static BioseqPtr 
FindSeqByAccession(CharPtr accver, Int2 id_num, Boolean is_na)
{
   BioseqPtr bsp = NULL;

#ifdef NCBI_ENTREZ_CLIENT
   CharPtr accession;
   Int4 version=0, gi, number;
   SeqIdPtr sip = NULL;
   TextSeqIdPtr tsip;
   PDBSeqIdPtr  psip;
   SeqPortPtr spp;
   Int2 retval, buf_length=512;
   Uint1 buf[512];
   ByteStorePtr old_seq_data = NULL;

   ID1BioseqFetchEnable ("wblast2", TRUE);
      
   if ((gi = atoi(accver)) == 0) {
      accession = StringTokMT(accver, ".", &accver);
      
      if (accver)
         version = atoi(accver);

      if((sip = ValNodeNew (NULL)) == NULL)
         return NULL;
      if((tsip = TextSeqIdNew ()) == NULL)
         return NULL;
      
      tsip->accession = StringSave(accession);
      tsip->version = version;
      /* GenBank, EMBL, and DDBJ. */
      sip->choice = SEQID_GENBANK;
      sip->data.ptrvalue = (Pointer) tsip;

      gi = ID1FindSeqId (sip);
      
      if (gi == 0) {
         /* SwissProt. */
         sip->choice = SEQID_SWISSPROT;
         gi = ID1FindSeqId (sip);
      }
      if (gi == 0) {
         /* PIR */
         sip->choice = SEQID_PIR;
         gi = ID1FindSeqId (sip);
      }
      
      if (gi == 0) {
         /* PRF */
         sip->choice = SEQID_PRF;
         gi = ID1FindSeqId (sip);
      }
      
      if (gi == 0) {
         /* OTHER, probably 'ref' */
         sip->choice = SEQID_OTHER;
        gi = ID1FindSeqId (sip);
      }
      
      if(gi == 0) {
         /* OK. We failed to find gi using string as TextSeqId. Now trying
            last time - with PDBSeqIdPtr */
         
         if((psip = PDBSeqIdNew()) == NULL)
            return NULL;
         
         sip->choice = SEQID_PDB;
         tsip = TextSeqIdFree(tsip);
         sip->data.ptrvalue = psip;
         
         psip->mol = accession;
         psip->chain = version;
         
         gi = ID1FindSeqId (sip);
      }
    
      sip = SeqIdFree(sip);
   }
   if (gi > 0) {
      ValNodeAddInt(&sip, SEQID_GI, gi);
      bsp = BioseqLockById(sip);
      SeqIdFree(sip);
   } 

   if (bsp == NULL)
      return NULL;
   
   if (ISA_na(bsp->mol) != is_na) {
      BioseqUnlock(bsp);
      return NULL;
   }

   /* We need to keep this bioseq to preserve the protein information for the 
      CDS translation feature. However we also need to pack the sequence data,
      so bsp->seq_data has to be modified.
   */
   if (ISA_na(bsp->mol)) {
      spp = SeqPortNew(bsp, 0, -1, Seq_strand_plus, 
                       Seq_code_iupacna);
      bsp->seq_data_type = Seq_code_iupacna;
   } else {
      spp = SeqPortNew(bsp, 0, -1, Seq_strand_unknown, 
                       Seq_code_ncbieaa);
      bsp->seq_data_type = Seq_code_ncbieaa;
   }
    
   SeqPortSet_do_virtual(spp, TRUE);
   number = 0;
   
   bsp->repr = Seq_repr_raw;
   old_seq_data = bsp->seq_data;
   bsp->seq_data = BSNew(bsp->length);

   while (number < bsp->length) {
      retval = SeqPortRead(spp, buf, buf_length);
      if (retval < 0)
         continue;
      if (retval == 0)
         break;
      BSWrite(bsp->seq_data, buf, retval);
      number += retval;
   }
    
   SeqPortFree(spp);
   old_seq_data = BSFree(old_seq_data);
   BioseqPack(bsp);
   ID1BioseqFetchDisable();
   
#endif

    return bsp;
}

static void BLASTOptionValidateHTML(BLAST_OptionsBlkPtr options, CharPtr progname, CharPtr seq1, CharPtr seq2, CharPtr one, CharPtr two, Boolean is_prot, Int2 mtrx, Int2 from, Int2 to, Int2 ffrom, Int2 tto, Int2 filter, Int2 pagecount)
{
    Int2 status;
    ValNodePtr error_return=NULL;
    
    status = BLASTOptionValidateEx(options, progname, &error_return);
    if (status != 0) {
        Blast2SeqMainPage(NULL, seq1, seq2, one, two, error_return, is_prot, options, mtrx, from, to, ffrom, tto, filter, pagecount);
    }
    return;
}

static void PrintParam(Boolean is_prot, Int2 mtrx, Int2 ma, Int2 ms, BLAST_OptionsBlkPtr options, CharPtr seq_2, CharPtr seq_1, CharPtr one, CharPtr two, BioseqPtr query_bsp, BioseqPtr subject_bsp, Int4 len1, Int4 len2, Int4 from, Int4 to, Int4 ffrom, Int4 tto, Int2 pagecount)
{
    ValNodePtr vnp;
    CharPtr s;
    static Char buf[41];
    Int4 gi;

    printf("<FORM NAME= bl2 method=\"POST\" action="    
#if defined (BL2SEQ_STANDALONE) && defined (NCBI_ENTREZ_CLIENT)
           "\"wblast2_cs.cgi"
#else 
           "\"wblast2.cgi"
#endif
           "?%d\" enctype=\"multipart/form-data\">\n\n", pagecount);

    printf("<INPUT TYPE=hidden name=\"page\" value=\"%d\">\n", pagecount+1);
    if (is_prot == TRUE) {
#if defined (BL2SEQ_STANDALONE)
        printf("<A HREF=docs/options.html#matrix>Matrix</A>\n");
#else
        printf("<A HREF=http://www.ncbi.nlm.nih.gov/BLAST/options.html#matrix TARGET=one>Matrix</A>\n");
#endif
        printf("<select name=\"matrix\" onChange=\"update_mtrx(this)\">\n");
        
        printf("<option value=0>BLOSUM62\n");
        printf("<option value=1%s>PAM30\n", (mtrx == 1) ? " SELECTED" : "");
        printf("<option value=2%s>PAM70\n", (mtrx == 2) ? " SELECTED" : "");
        printf("<option value=3%s>BLOSUM45\n", (mtrx == 3) ? " SELECTED" : "");
        printf("<option value=4%s>BLOSUM80\n", (mtrx == 4) ? " SELECTED" : "");
        printf("</select>\n");
    } else {
       printf("Match:<INPUT TYPE=text name=match size=2 value=%d>\n", ma); 
       printf("Mismatch:<INPUT TYPE=text name=msmatch size=2 value=%d>\n", ms);
    }
    printf("gap open:<INPUT TYPE=text name=gopen size=2 value=%d>\n", options->gap_open);
    printf("gap extension: <INPUT TYPE=text size=2 name=\"gext\" value=%d> <BR>\n", options->gap_extend);
    printf("x_dropoff: <INPUT TYPE=text size=2 name=\"dropoff\" value=\"%d\">\n", options->gap_x_dropoff);
    printf("expect:<INPUT TYPE=text size=4 name=\"expect\" value=\"%f\">\n", options->expect_value);
    printf("wordsize: <INPUT type=text size=2 name=\"word\" "); 
    printf("value=%d>\n\n", options->wordsize);
#if defined (BL2SEQ_STANDALONE)
    printf("<a href=docs/newoptions.html#filter>Filter</a>");
#else
    printf("<a href=http://www.ncbi.nlm.nih.gov/blast/newoptions.html#filter>Filter</a>");
#endif

    printf(" <INPUT TYPE=checkbox NAME=Filter VALUE=1");
    if (options->filter == 1) {
        printf(" CHECKED>\n");
    } else {
        printf(">\n");
    }
    printf("<INPUT TYPE=hidden name=\"program\" value=\"%s\">\n",
           options->program_name);
    
    printf("<INPUT TYPE=hidden name=\"matrix\" value=\"%d\">\n", mtrx);
    
    printf("<INPUT TYPE=submit VALUE=Align><HR>\n");
    if (seq_1 != NULL && *seq_1 != NULLB) {
        printf("<INPUT TYPE=hidden name=\"seq\" value=\"");
        for (s = seq_1; *s != '\0'; s++) {
            if (*s == '>') {
                printf("&gt;");
            } else {
                printf("%c", *s);
            }
        }
        printf("\">\n");
    } else if (one != NULL) {
        printf("<INPUT TYPE=hidden name=\"one\" value=\"%s\">\n", one);
    }
    if (seq_2 != NULL && *seq_2 != NULLB) {
        printf("<INPUT TYPE=hidden name=\"sseq\" value=\"");
        for (s = seq_2; *s != '\0'; s++) {
            if (*s == '>') {
                printf("&gt;");
            } else {
                printf("%c", *s);
            }
        }
        printf("\">\n");
    } else if (two != NULL) {
        printf("<INPUT TYPE=hidden name=\"two\" value=\"%s\">\n", two);
    }
    printf("<INPUT TYPE=hidden name=\"from\" value=\"%d\">\n", from);
    printf("<INPUT TYPE=hidden name=\"to\" value=\"%d\">\n", to);
    printf("<INPUT TYPE=hidden name=\"ffrom\" value=\"%d\">\n", ffrom);
    printf("<INPUT TYPE=hidden name=\"tto\" value=\"%d\">\n", tto);
    printf("<TABLE>\n");
    printf("<TR><TD><strong> Sequence 1</strong></TD>\n");
    if ((gi = GetGIForSeqId(SeqIdFindBest(query_bsp->id, SEQID_GI))) != 0) {
        printf("<TD>gi<A HREF=http://www.ncbi.nlm.nih.gov/entrez/viewer.fcgi?val=%ld> %ld</A></TD>", gi, gi);
    } else {
        SeqIdWrite(query_bsp->id, buf, PRINTID_FASTA_LONG, 40);
        printf("<TD>%s</TD>", buf);
    }
    for (vnp=query_bsp->descr; vnp; vnp=vnp->next) {
        if (vnp->choice == Seq_descr_title) {
            printf("<TD>%s</TD>", vnp->data.ptrvalue);
            break;
        }
    }
    if (vnp == NULL) {
        printf("<TD></TD>");
    }
    printf("<TD><strong>Length</strong></TD><TD>%ld</TD>\n", len1);
    if (from != 0 || to != 0) {
        printf("<TD>(%ld .. %ld)</TD>\n", from, to);
    }
    printf("</TR><TR>\n");
    printf("<TR><TD><strong> Sequence 2</strong></TD>\n");
    if ((gi = GetGIForSeqId(SeqIdFindBest(subject_bsp->id, SEQID_GI))) != 0) {
        printf("<TD>gi <A HREF=http://www.ncbi.nlm.nih.gov/entrez/viewer.fcgi?val=%ld>%ld</A></TD>", gi, gi);
    } else {
        SeqIdWrite(subject_bsp->id, buf, PRINTID_FASTA_LONG, 40);
        printf("<TD>%s</TD>", buf);
    }
    for (vnp=subject_bsp->descr; vnp; vnp=vnp->next) {
        if (vnp->choice == Seq_descr_title) {
            printf("<TD>%s</TD>", vnp->data.ptrvalue);
            break;
        }
    }
    if (vnp == NULL) {
        printf("<TD></TD>");
    }
    printf("<TD><strong>Length</strong></TD><TD>%ld</TD>\n", len2);
    if (ffrom != 0 || tto != 0) {
        printf("<TD>(%ld .. %ld)</TD>\n", ffrom, tto);
    }
    printf("</TR></TABLE>\n");

    return;
}

#ifndef BL2SEQ_STANDALONE
/** Thread to take care of log messages when termination by signal */
static void* ConnectionThreadRun(void *p)
{
   int nfds;
   int rc;
   struct pollfd *fds=NULL;

   nfds = 1;

   fds = (struct pollfd *) 
      malloc( nfds*sizeof(struct pollfd) );


   fds[0].fd = fileno(stdout);
   fds[0].events = POLLOUT;
   fds[0].revents = 0;

   poll(NULL, 0, 2000);

   while (run_status != WBLAST2_DONE) {
      if (sigflag) {
         int signo = GetSignal();

         if (signo == SIGXCPU) {
            if (run_status == WBLAST2_FORMAT) {
               /* Ignore CPU limit if formatting has already started */
               signo = 0;
            } else {
               printf("<HTML>\n<HEAD>\n<TITLE>\nTime Expired\n</TITLE>\n</HEAD>\n");
               printf("<BODY>\n<H1>\nTime expired\n</H1>\n<HR>\n</BODY>\n</HTML>\n");
               fflush(NULL);
            }
         }
         if (signo) {
            logmsg(signo);
            poll(NULL, 0, 1000);
            exit(signo);
         }
      }

      /* If no signal has been received yet, test the stdout socket */
      rc = poll(fds, nfds, 1000);

      if (run_status == WBLAST2_DONE)
         break;

      if (rc < 0 || 
          (fds[0].revents & (POLLHUP | POLLNVAL | POLLERR))) {
         logmsg(fds[0].revents);
         close(fds[0].fd);
         abort();
      } else if (run_status == WBLAST2_SEARCH) {
          char buffer[4096];
          memset(buffer, ' ', 4095);
          buffer[4095] = NULLB;
          /*strcpy(buffer, "      ");*/
          
          fprintf(stdout, "<!--%s-->\n", buffer);
          fflush(stdout);
      }
      if (run_status != WBLAST2_DONE) /* Sleep 4 seconds */
         poll(NULL, 0, 4000);
   }

   MemFree(fds);

   return NULL;
}
#endif

#ifndef USE_OLD_BLAST
static void BLASTOptions2SummaryOptions(BLAST_OptionsBlk* options,
                                        Char* progname,
                                        BLAST_SummaryOptions* s_options)
{
   if (!options || !s_options)
      return;

   s_options->hint = eNone;

   if (options->is_megablast_search)
      s_options->use_megablast = TRUE;

   if (!strcmp(progname, "blastn"))
      s_options->program = eBlastn;
   else if (!strcmp(progname, "blastp"))
      s_options->program = eBlastp;
   else if (!strcmp(progname, "blastx"))
      s_options->program = eBlastx;
   else if (!strcmp(progname, "tblastn"))
      s_options->program = eTblastn;
   else if (!strcmp(progname, "tblastx"))
      s_options->program = eTblastx;
   else
      s_options->program = eChoose;
   
   s_options->strand = options->strand_option;
   s_options->cutoff_evalue = options->expect_value;
   if (options->matrix)
      s_options->matrix = strdup(options->matrix);


   if (options->filter_string)
      s_options->filter_string = strdup(options->filter_string);
   else /* If filtering option not set, assume no filtering! */
      s_options->filter_string = strdup("F");

   s_options->word_size = options->wordsize;
   s_options->gapped_calculation = options->gapped_calculation;
   s_options->nucleotide_match = options->reward;
   s_options->nucleotide_mismatch = options->penalty;
   s_options->gap_open = options->gap_open;
   s_options->gap_extend = options->gap_extend;
   s_options->gap_x_dropoff = options->gap_x_dropoff;
   s_options->db_length = options->db_length;
   s_options->word_threshold = options->threshold_second;

   if (!options->multiple_hits_only)
      s_options->init_seed_method = eOneHit;
}
#endif

#define BL2SEQ_CPU_LIMIT 240

Int2 Main(void)
{
    SeqEntryPtr query_sep = NULL, subject_sep = NULL;
    BioseqPtr fake_bsp, query_bsp = NULL, subject_bsp = NULL;
    SeqIntPtr sip1, sip2;
    Int2 index = 0;
    Int4 gopen, gext, dropoff, ma=0, ms=0;
    Int4 from=0, to=0, ffrom=0, tto=0;
    Int2 wordsize, filter=0;
    Int2 mtrx = 0, color=1;
    SeqAlignPtr seqalign = NULL, sap, sapnext;
    SeqAnnotPtr hsat= NULL, sat, satnext;
    FloatHi	expect;
    Boolean is_prot=FALSE, is_aa1=FALSE, is_aa2=FALSE, is_na1=TRUE, is_na2=TRUE;
    CharPtr seq_1=NULL, seq_2=NULL, c1, c2, chptr;
    CharPtr sq_1=NULL, sq_2=NULL, one=NULL, two=NULL, sbuf, progname;
    static Char mbuf[12];
    BLAST_OptionsBlkPtr options = NULL;
    Int4 ll, len1, len2, txoption;
    SeqIdPtr sip;
    Int4Ptr PNTR txmatrix = NULL;
    BLAST_MatrixPtr blast_matrix = NULL;
    SeqPortPtr spp;
    Uint1 code1, code2;
    ValNodePtr vnp, error_return=NULL;
    FloatHi       scalex, scaley;
    DenseDiagPtr ddp;
    DenseSegPtr  dsp;
    StdSegPtr    ssp;
    Int4         index1, i2;
    Int4         x, y, xx, yy;
    Int2         k, x_factor, y_factor;
    static PrymPtr PNTR rect;
    static PrymPtr PNTR rectY;
    SeqLocPtr    slp1=NULL, slp2=NULL, slp, sl, qslp = NULL;
    Uint1        align_type;
    ValNodePtr	   other_returns, mask = NULL, mask_head = NULL;
    CharPtr		   buffer = NULL;
    BLAST_KarlinBlkPtr ka_params=NULL, ka_gap_params=NULL;
    TxDfDbInfoPtr      dbinfo = NULL;
    BlastTimeKeeper    time_keeper;
    
    WWWBlastInfoPtr theInfo;
    ReadDBFILEPtr rdfp;
    Int4 dbseq_num;
    CharPtr dbname;
    CharPtr error_msg;
    Int2 pagecount = 0;
    Boolean is_megablast = FALSE;
    CharPtr rid = NULL, database = NULL;
    sigset_t sigset;
    struct sigaction sa;
#ifndef BL2SEQ_STANDALONE
    TNlmThread connection_thread = NULL;
    void *thrstat;
    CharPtr program_log, user_IP;
    int pid, time_start, results_size = 0;
#endif

#ifndef USE_OLD_BLAST 
    BLAST_SummaryOptions* s_options = NULL;
    Blast_SummaryReturn* s_return = NULL;
    Boolean mask_at_hash = FALSE;
    EBlastProgramType program_number; 
#endif

#ifdef RLIMIT_CPU
    struct rlimit rl;
    
#ifdef SIGXCPU
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = sighandler;
    sigaction(SIGXCPU, &sa, NULL);
#endif
    getrlimit(RLIMIT_CPU, &rl);
    rl.rlim_cur = BL2SEQ_CPU_LIMIT;
    rl.rlim_max = BL2SEQ_CPU_LIMIT + 120;
    setrlimit(RLIMIT_CPU, &rl);
#endif

    /* mask all signals when in signal handler */
    sigfillset(&sigset);
    sa.sa_mask = sigset;

    /* Handle SIGPIPE */
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = sighandler;
    sigaction(SIGPIPE, &sa, NULL);

    if (!SeqEntryLoad())
        return 1;
    
    UseLocalAsnloadDataAndErrMsg ();
    
    ErrSetMessageLevel(SEV_WARNING);
    
    PrepareSigmask();

    /* This function will read posting data, set-up config file and
       write small message into logfile (if it exists) */
    
    theInfo = MemNew(sizeof(WWWBlastInfo));
    
    if(WWWGetArgs(&theInfo->info) != WWWErrOk) {
        WWWInfoFree(theInfo->info);
        MemFree(theInfo);
        Blast2SeqMainPage(NULL, NULL, NULL, NULL, NULL, NULL, FALSE, 
                          NULL, 0, 0, 0, 0, 0, 1, 0);
        return 1;    
    }
    if((chptr = WWWGetQuery(theInfo->info)) == NULL || *chptr == NULLB) {
        WWWInfoFree(theInfo->info);
        MemFree(theInfo);
        Blast2SeqMainPage(NULL, NULL, NULL, NULL, NULL, NULL, FALSE, 
                          NULL, 0, 0, 0, 0, 0, 1, 0);
    }
    
    if(getenv("DEBUG_COMMAND_LINE") != NULL) {
        FILE *fd;
        fd = FileOpen("__web.in", "w");
        fprintf(fd, "%s", ((WWWInfoDataPtr)theInfo->info)->query);
        FileClose(fd);
    }
    
    if((chptr = WWWGetValueByName(theInfo->info, "PROGRAM")) != NULL)
        theInfo->program = StringSave(chptr);
    else if ((chptr = WWWGetValueByName(theInfo->info, "PROT")) != NULL)
        theInfo->program = StringSave(chptr);
    else
        theInfo->program = StringSave("blastn");
    
    theInfo->align_type = 
        BlastGetTypes(theInfo->program, &theInfo->query_is_na, 
                      &theInfo->db_is_na);
    
    is_aa1 = !theInfo->query_is_na;
    is_aa2 = !theInfo->db_is_na;
    is_prot = (is_aa1 || is_aa2);
    
    progname = theInfo->program;
    
    /* For tblastx, we still do a protein search, even though both sequences
       are nucleotide. */
    if (!(StringICmp(progname, "tblastx")))
       is_prot = TRUE;


    if ((chptr = WWWGetValueByName(theInfo->info, "MEGABLAST")) != NULL)
       is_megablast = TRUE;
    options = BLASTOptionNewEx(progname, TRUE, is_megablast);
    
    if ((chptr = WWWGetValueByName(theInfo->info, "PAGE")) != NULL)
        pagecount = atoi(chptr);
    
    if ((chptr = WWWGetValueByName(theInfo->info, "GOPEN")) != NULL &&
        StringStr(chptr, "default") == NULL)
        gopen = atoi(chptr);
    else
        gopen = -1;
    
    if ((chptr = WWWGetValueByName(theInfo->info, "GEXT")) != NULL &&
        StringStr(chptr, "default") == NULL)
        gext = atoi(chptr);
    else
        gext = -1;
    
    if((chptr = WWWGetValueByName(theInfo->info, "DROPOFF")) != NULL &&
       StringStr(chptr, "default") == NULL)
        dropoff = atoi(chptr);
    else
        dropoff = 50;
    
    if((chptr = WWWGetValueByName(theInfo->info, "EXPECT")) != NULL &&
       StringStr(chptr, "default") == NULL)
        expect = atof(chptr);
    else
        expect = 10;
    
    if((chptr = WWWGetValueByName(theInfo->info, "MATCH")) != NULL &&
       StringStr(chptr, "default") == NULL)
        ma = atoi(chptr);
    else if (!StrCmp(progname, "blastn"))
        ma = 1;
    if((chptr = WWWGetValueByName(theInfo->info, "MSMATCH")) != NULL &&
       StringStr(chptr, "default") == NULL)
        ms = atoi(chptr);
    else if (!StrCmp(progname, "blastn"))
        ms = -2;
    
    if((chptr = WWWGetValueByName(theInfo->info, "FROM")) != NULL &&
       StringStr(chptr, "default") == NULL)
        from = atoi(chptr);
    else
        from = 0;
    if((chptr = WWWGetValueByName(theInfo->info, "FFROM")) != NULL &&
       StringStr(chptr, "default") == NULL)
        ffrom = atoi(chptr);
    else
        ffrom = 0;
    if((chptr = WWWGetValueByName(theInfo->info, "TO")) != NULL &&
       StringStr(chptr, "default") == NULL)
        to = atoi(chptr);
    else
        to = 0;
    if((chptr = WWWGetValueByName(theInfo->info, "TTO")) != NULL &&
       StringStr(chptr, "default") == NULL)
        tto = atoi(chptr);
    else
        tto = 0;

    if((chptr = WWWGetValueByName(theInfo->info, "STRAND")) != NULL &&
       StringStr(chptr, "default") == NULL)
        options->strand_option = atoi(chptr);
    else
        options->strand_option = Seq_strand_both;

#ifdef NCBI_ENTREZ_CLIENT
    one = WWWGetValueByName(theInfo->info, "ONE");
    two = WWWGetValueByName(theInfo->info, "TWO");
#endif
    options->cpu_limit = BL2SEQ_CPU_LIMIT;
    
    if((chptr = WWWGetValueByName(theInfo->info, "WORD")) != NULL &&
       StringStr(chptr, "default") == NULL)
        wordsize = atoi(chptr);
    else if (!is_megablast)
        wordsize = (is_prot == TRUE) ? 3 : 11;
    else {
        wordsize = 28;
    }

    if (WWWGetValueByName(theInfo->info, "FILTER") != NULL)
        filter =1;
    
    run_status = WBLAST2_SEARCH;

#ifndef BL2SEQ_STANDALONE
    /** fire thread testing connection; not done in the standalone 
        WWW server package */
    connection_thread = NlmThreadCreateEx(ConnectionThreadRun, NULL, 
                           THREAD_RUN|THREAD_BOUND, eTP_Default, NULL, NULL);
#endif
    
    if((chptr = WWWGetValueByName(theInfo->info, "SEQ")) == NULL ||
       *chptr == NULLB)
        chptr = WWWGetValueByName(theInfo->info, "SEQFILE1");
    
    if (chptr) {
       while (IS_WHITESP(*chptr))
          chptr++;
    }
    if (chptr && *chptr != NULLB)
       c1 = StringSave(chptr);
    else
       c1 = NULL;

    if (c1 && *c1 != '>') {
        sbuf = MemNew(StringLen(c1)+8);
        sprintf(sbuf, ">seq_1\n%s", c1);
        seq_1 = StringSave(sbuf);
        MemFree(c1);
        c1 = seq_1;
        MemFree(sbuf);
    }
    
    if((chptr = WWWGetValueByName(theInfo->info, "SSEQ")) == NULL || 
       *chptr == NULLB)
        chptr = WWWGetValueByName(theInfo->info, "SEQFILE2");
    
    if (chptr) {
       while (IS_WHITESP(*chptr))
          chptr++;
    }
    if (chptr && *chptr != NULLB)
        c2 = StringSave(chptr);
    else 
        c2 = NULL;
    
    if (c2 && *c2 != '>') {
        sbuf = MemNew(StringLen(c2)+8);
        sprintf(sbuf, ">seq_2\n%s", c2);
        seq_2 = StringSave(sbuf);
        MemFree(c2);
        c2 = seq_2;
        MemFree(sbuf);
    }
    seq_1 = c1;
    seq_2 = c2;
    if (c1 && c2 && StrNCmp(c1, c2, 6) == 0) {
        if (*c1 == '>')
            seq_1 = seq_1 + 1;
        sbuf = Malloc(StringLen(seq_1)+4);
        sprintf(sbuf, ">1_%s", seq_1);
        seq_1 = StringSave(sbuf);
        sbuf = MemFree(sbuf);
        MemFree(c1);
        if (*c2 == '>')
            seq_2 = seq_2 + 1;
        sbuf = Malloc(StringLen(seq_2)+4);
        sprintf(sbuf, ">2_%s",seq_2);
        seq_2 = StringSave(sbuf);
        MemFree(sbuf);
        MemFree(c2);
    }
    
    if (seq_1 != NULL) {
        query_sep = FastaToSeqBuff(seq_1, &sq_1, !is_aa1);
        query_bsp = (BioseqPtr) query_sep->data.ptrvalue;
        is_na1 = FastaCheckDna(seq_1);
    } else if (one != NULL && *one != NULLB) {
       if ((rid = WWWGetValueByName(theInfo->info, "RID")) != NULL) {
#ifndef BL2SEQ_STANDALONE
          /* Get the query sequence from the QBlast results */
          sip = SeqIdParse(one);
          
          QBlastGetResultsEx(rid, NULL, &query_bsp, NULL, &database, NULL,
                             NULL, sip);
#endif
          if (!query_bsp) {
             error_msg = "The query sequence is not found";
             Blast2SeqMainPage(error_msg, seq_1, seq_2, 
                               one, two, NULL, is_prot, options, mtrx,
                               from, to, ffrom, tto, filter, pagecount);
          }
       } else {
          if ((query_bsp = FindSeqByAccession(one, 1, !is_aa1)) == NULL) {
             error_msg = "The first sequence accession is not found";
             Blast2SeqMainPage(error_msg, seq_1, seq_2, 
                               one, two, NULL, is_prot, options, mtrx,
                               from, to, ffrom, tto, filter, pagecount);
          }
       }
       if (query_bsp)
          is_na1 = ISA_na(query_bsp->mol);
    }
    if (seq_2  != NULL) {
        subject_sep = FastaToSeqBuff(seq_2, &sq_2, !is_aa2);
        subject_bsp = (BioseqPtr) subject_sep->data.ptrvalue;
        is_na2 = FastaCheckDna(seq_2);
    } else if (two != NULL && *two != NULLB) {
       if (rid && database) {
          /* Fetch the subject seqience from the BLAST database by the 
             SeqId provided in the link */
          sip = SeqIdParse(two);
          ReadDBBioseqFetchEnable("wblast2", database, TRUE, TRUE);
          subject_bsp = BioseqLockById(sip);
          ReadDBBioseqFetchDisable();
          if (!subject_bsp) {
             error_msg = "The database sequence is not found";
             Blast2SeqMainPage(error_msg, seq_1, seq_2, 
                               one, two, NULL, is_prot, options, mtrx,
                               from, to, ffrom, tto, filter, pagecount);
          }
       } else {
          subject_bsp = FindSeqByAccession(two, 2, !is_aa2);
       }
       if (subject_bsp == NULL) {
            error_msg = "The second sequence accession is not found";
            Blast2SeqMainPage(error_msg, seq_1, seq_2, 
                              one, two, NULL, is_prot, options, mtrx,
                              from, to, ffrom, tto, filter, pagecount);
        }
        is_na2 = ISA_na(subject_bsp->mol);
    }
    code1 = ((is_aa1 == FALSE) ? Seq_code_iupacna:Seq_code_iupacaa);
    code2 = ((is_aa2 == FALSE) ? Seq_code_iupacna:Seq_code_iupacaa);
    
    if (is_prot) {
        if((chptr = WWWGetValueByName(theInfo->info, "MATRIX")) == NULL ||
           *chptr == NULLB)
            chptr = WWWGetValueByName(theInfo->info, "MTRX");
        if (chptr && *chptr != NULLB)
            mtrx = atoi(chptr);
        else
           mtrx = 0;
        
        switch(mtrx) {
        case 1:
           sprintf(mbuf, "PAM30");
           if (gopen == -1 || gext == -1) {
              gopen = (gopen == -1) ? 9: gopen;
              gext = (gext == -1) ? 1: gext;
           }
           break;
        case 2:
           sprintf(mbuf, "PAM70");
           if (gopen == -1 || gext == -1) {
              gopen = (gopen == -1) ? 10: gopen;
              gext = (gext == -1) ? 1: gext;
           }
           break;
        case 3:
           sprintf(mbuf, "BLOSUM45");
           if (gopen == -1 || gext == -1) {
              gopen = (gopen == -1) ? 15: gopen;
              gext = (gext == -1) ? 2: gext;
           }
           break;
        case 4:
           sprintf(mbuf, "BLOSUM80");
           if (gopen == -1 || gext == -1) {
              gopen = (gopen == -1) ? 10: gopen;
              gext = (gext == -1) ? 1: gext;
           }
           break;
        default:
           sprintf(mbuf, "BLOSUM62");
           if (gopen == -1 || gext == -1) {
              gopen = (gopen == -1) ? 11: gopen;
              gext = (gext == -1) ? 1: gext;
           }
           break;
        }

        StringCpy(options->matrix, mbuf);
        
        /* Use one-pass initial word search with low threshold, except for 
           tblastx. Also do not use multiple hits approach for finding initial 
           seeds. */
        options->two_pass_method  = FALSE;  
        if (StrCmp(progname, "tblastx")) {
           options->threshold_second = 9;
           options->multiple_hits_only  = FALSE;
        }
    } else {
        if (gopen == -1) 
            gopen = (is_megablast) ? 0 : 5;
        if (gext == -1) 
            gext = (is_megablast) ? 0 : 2;
        options->penalty  = ms;
        options->reward  = ma;
    }
    
    if ((chptr = WWWGetValueByName(theInfo->info, "DBLEN")) != NULL) {
       options->db_length = atoi(chptr);
    } else {
       if((chptr = WWWGetValueByName(theInfo->info, "DB")) != NULL && 
          StrCmp(chptr, "=pdb") == 0) {
          dbname = StringSave("pdb");
       } else {
          dbname = StringSave("nr");
       }
    
       rdfp = readdb_new(dbname, is_aa2);
       if (rdfp) {
          readdb_get_totals_ex(rdfp, &options->db_length, &dbseq_num, TRUE);
          readdb_destruct(rdfp);
       } else
          options->db_length = (is_aa2 ? NR_SIZE_AA : NR_SIZE_NA);
       
       MemFree(dbname);
    }

    options->wordsize  = wordsize;
    if (is_megablast) {
        options->cutoff_s2 = options->wordsize*options->reward;
        options->cutoff_s = (options->wordsize + 4)*options->reward;
    }
    options->gap_open  = gopen;
    options->gap_extend  = gext;
    options->gap_x_dropoff  = dropoff;
    options->expect_value = expect;
    options->filter = filter;
    if (filter == 1) {
        options->filter_string = StringSave("T");
    }
    
    if (!StringICmp(progname, "tblastn") || 
        !(StringICmp(progname, "tblastx")))
        options->db_genetic_code = 1;
    
    if (query_bsp == NULL || subject_bsp == NULL) {
        error_msg = "Please enter the sequences";   
        Blast2SeqMainPage(error_msg, seq_1, seq_2, 
                          one, two, NULL, is_prot, options, mtrx,
                          from, to, ffrom, tto, filter, pagecount);
    }
    
    error_return = NULL;
    if (is_aa1 && is_na1) 
        BlastConstructErrorMessage(NULL, "First sequence must be protein for this program", 1, &error_return);  
    if (is_aa2 && is_na2)
        BlastConstructErrorMessage(NULL, "Second sequence must be protein for this program", 1, &error_return);  
    if (!is_aa1 && !is_na1)
        BlastConstructErrorMessage(NULL, "First sequence must be nucleotide for this program", 1, &error_return);  
    if (!is_aa2 && !is_na2)
        BlastConstructErrorMessage(NULL, "Second sequence must be nucleotide for this program", 1, &error_return);  
    if (error_return)
        Blast2SeqMainPage(NULL, seq_1, seq_2, one, two, error_return, is_prot, 
                          options, mtrx, from, to, ffrom, tto, filter, pagecount);
    
    BLASTOptionValidateHTML(options, progname, seq_1, seq_2, one, two, 
                            is_prot, mtrx, from, to, ffrom, tto, filter,
                            pagecount);

    fake_bsp = BlastMakeFakeBioseq(query_bsp, NULL);
    if (from == 0 && to == 0) {
        ValNodeAddPointer(&slp1, SEQLOC_WHOLE, SeqIdDup(fake_bsp->id));
        len1 = fake_bsp->length;
    } else {
        sip1 = SeqIntNew();
        sip1->from = (from > 0) ? from-1 : 0; 
        sip1->to = (to < fake_bsp->length) ? to-1 : fake_bsp->length-1;
        sip1->id = (SeqIdPtr) SeqIdDup (fake_bsp->id);
        sip1->strand = options->strand_option;
        ValNodeAddPointer(&slp1, SEQLOC_INT, sip1);
        len1 = SeqLocLen(slp1);
    }
    if (ffrom == 0 && tto == 0) {
        ValNodeAddPointer(&slp2, SEQLOC_WHOLE,
                          SeqIdDup(SeqIdFindBest(subject_bsp->id, SEQID_GI)));
        len2 = subject_bsp->length;
    } else {
        sip2 = SeqIntNew();
        sip2->from = (ffrom > 0) ? ffrom-1 : 0; 
        sip2->to = (tto < subject_bsp->length) ? tto-1 : subject_bsp->length-1;
        sip2->id = (SeqIdPtr) SeqIdDup(SeqIdFindBest(subject_bsp->id, SEQID_GI));
        sip2->strand = Seq_strand_both;
        ValNodeAddPointer(&slp2, SEQLOC_INT, sip2);
        len2 = SeqLocLen(slp2);
    }
    if ((spp = SeqPortNewByLoc(slp1, code1)) == NULL) {
        error_msg = "The first sequence location is not valid";
        Blast2SeqMainPage(error_msg, seq_1, seq_2, one, two, NULL, is_prot, options, 
                          mtrx, from, to, ffrom, tto, filter, pagecount);
    } else
       SeqPortFree(spp);
    if ((spp = SeqPortNewByLoc(slp2, code2)) == NULL) {
        error_msg = "The second sequence location is not valid";
        Blast2SeqMainPage(error_msg, seq_1, seq_2, one, two, NULL, is_prot, options, 
                          mtrx, from, to, ffrom, tto, filter, pagecount);
    } else
       SeqPortFree(spp);
    
    other_returns = NULL;

#ifndef BL2SEQ_STANDALONE
    loginfo.filename = StringSave("Log/wblast2.log");
    loginfo.time_start = 0;
    if (((loginfo.user_IP = getenv("PROXIED_IP")) == NULL) &&
        ((loginfo.user_IP = getenv("USER_ADDR")) == NULL)) {
       loginfo.user_IP = StringSave("255.255.255.255");
    }
    loginfo.pid = getpid();
    loginfo.q_length = len1;
    loginfo.s_length = len2;
    loginfo.size = 0;

    if (!is_megablast)
       loginfo.program = StringSave(progname);
    else
       loginfo.program = StringSave("mblastn");
    logmsg(0);
#endif

#ifndef USE_OLD_BLAST
    BLAST_SummaryOptionsInit(&s_options);
    BLASTOptions2SummaryOptions(options, progname, s_options);
    BLAST_TwoSeqLocSets(s_options, slp1, slp2, NULL, &seqalign, &mask, &mask_at_hash,
                        &s_return);
    if (s_return->error) {
        /* Print the error message. */
        Blast2SeqMainPage(s_return->error->message, seq_1, seq_2, one, two, NULL,
                          is_prot, options, mtrx, from, to, ffrom, tto, filter, 
                          pagecount);    
    } 

    if (mask_at_hash) {
       /* If masking was done for lookup table only, do not use mask locations
          for formatting. */
       while (mask) {
          SeqLocSetFree(mask->data.ptrvalue);
          mask = mask->next;
       }
    }
        
    BLAST_SummaryOptionsFree(s_options);
#else
    seqalign =  BlastTwoSequencesByLocEx(slp1, slp2, progname, 
                                         options, &other_returns, NULL);
#endif
    run_status = WBLAST2_FORMAT;    
    /*	seqalign =  BlastTwoSequencesEx(query_bsp, subject_bsp, progname, 
        options, &other_returns, NULL);*/
    
    if (seqalign == NULL) {
        if ((chptr = WWWGetValueByName(theInfo->info, "SEQ")) != NULL) {
            MemFree(seq_1);
            seq_1 = StringSave(chptr);
        }
        if((chptr = WWWGetValueByName(theInfo->info, "SSEQ")) != NULL) {
            MemFree(seq_2);
            seq_2 = StringSave(chptr);
        }
        CreateJavaHeadHTML(query_bsp, subject_bsp, from, to, ffrom, tto, 
                           len1, len2, progname);
        PrintParam(is_prot, mtrx, ma, ms, options, seq_2, seq_1, one, two,
                   query_bsp, subject_bsp, len1, len2, from, to, ffrom, tto, pagecount);
        printf("<strong><font color=#0000EE>No significant similarity was found</font></strong>\n");

        goto cleanup;
    }

#ifdef USE_OLD_BLAST
    mask = NULL;
    for (vnp=other_returns; vnp; vnp = vnp->next) {
        switch (vnp->choice) {
           case TXDBINFO:
               dbinfo = vnp->data.ptrvalue;
               break;
        case TXKABLK_NOGAP:
            ka_params = vnp->data.ptrvalue;
            break;
        case TXKABLK_GAP:
            ka_gap_params = vnp->data.ptrvalue;
            break;
        case TXPARAMETERS:
            buffer = vnp->data.ptrvalue;
            break;
        case TXMATRIX:
            blast_matrix = vnp->data.ptrvalue;
            if (blast_matrix)
                txmatrix = BlastMatrixToTxMatrix(blast_matrix);
            break;
        case SEQLOC_MASKING_NOTSET:
        case SEQLOC_MASKING_PLUS1:
        case SEQLOC_MASKING_PLUS2:
        case SEQLOC_MASKING_PLUS3:
        case SEQLOC_MASKING_MINUS1:
        case SEQLOC_MASKING_MINUS2:
        case SEQLOC_MASKING_MINUS3:
            ValNodeAddPointer(&mask, vnp->choice, vnp->data.ptrvalue);
            break;
        default:
            break;
        }
    }	
    
    ValNodeFree(other_returns);
#endif
    
    to = (to >0) ? to : fake_bsp->length;
    tto = (tto >0) ? tto : subject_bsp->length;
    from = (from >0) ? from : 1;
    ffrom = (ffrom >0) ? ffrom : 1;
    
    CreateJavaHeadHTML(query_bsp, subject_bsp, 
                       from, to, ffrom, tto, len1, len2, progname);
    PrintParam(is_prot, mtrx, ma, ms, options, seq_2, seq_1, one, two,
               query_bsp, subject_bsp, len1, len2, from, to, ffrom, tto, pagecount);
    
    align_type = BlastGetProgramNumber(progname);
    
    x_factor = y_factor = 1;
    if (align_type == blast_type_tblastn)
        x_factor = 3;
    else if (align_type == blast_type_blastx)
        y_factor = 3;
    
    x = y = 150;
    ll = MAX(len1*x_factor, len2*y_factor);
    scalex = (FloatHi) x*x_factor/ll;
    scaley = (FloatHi) y*y_factor/ll;
    k = 0;
    color = 1;
    index = 0;
    
    printf("<table>\n");
    printf("<tr><td>\n");
    
    /* For tblastx all alignments are in one seqalign; let's separate them */
    if (align_type == blast_type_tblastx) {
       StdSegPtr nextssp;
       SeqAlignPtr last_sap = NULL, head = NULL;
       sap = seqalign;
       ssp = seqalign->segs;
       while (ssp) {
          nextssp = ssp->next;
          ssp->next = NULL;
          seqalign->segs = ssp;
          sap = SeqAlignDup(seqalign);
          if (last_sap) {
             last_sap->next = sap;
             last_sap = last_sap->next;
          } else
             head = last_sap = sap;
          ssp = nextssp;
       }
       SeqAlignFree(seqalign);
       seqalign = head;
    }

    for (sap=seqalign; sap != NULL; sap = sap->next) {
        if (sap->segtype > 3) {	
            sap = sap->segs;
        }
        if (index%2 == 0) {
            color = 1;
        } else {
            color = 2;
        }
        if (sap->segtype == SAS_DENDIAG) {
            for (ddp = sap->segs, index1=0; ddp; ddp= ddp->next, index1++);
        } else if (sap->segtype == SAS_DENSEG) {
            dsp = sap->segs;
            index1 = dsp->numseg;
        } else if (sap->segtype == SAS_STD) {
            for (ssp = sap->segs, index1=0; ssp; ssp = ssp->next, index1++);
        }
        
        rect = MemNew((2*index1+3)*sizeof(PrymPtr));
        rectY = MemNew((2*index1+3)*sizeof(PrymPtr));
        k = CreateRectAlign(sap, rect, rectY, scalex, scaley,
                            len1, len2, color, from, ffrom, to, tto);
        DrawRectAlign(rect, k, color, 9, index);
        /*	PrintRectAlign(rect, k, color, 9, index);*/
        printf("<BR>\n");
        DrawRectAlign(rectY, k, color, 9, index);
        
        MemFree(rect);
        MemFree(rectY);
        /*	PrintRectAlign(rectY, k, color, 9, index);*/
        printf("<BR><BR>\n"); 
        index++;
    }
    printf("</td><td width=75></td><td valign=top><b>2</b></td><td>\n");
    xx = yy = 150;
    scalex = ((FloatHi) xx) / len1;
    scaley = ((FloatHi) yy) / len2;
    
    /*	printf("<INPUT TYPE=image border=0 SRC=\"bag?");*/
    printf("<IMG border=0 SRC=\"bl2bag.cgi?");
    xx = scalex * len1;
    yy = scaley * len2;
    printf("%s", NumToGun(xx));
    printf("%s", NumToGun(yy));
    printf("CCCCCC-");
    
    printf("rD9D9D9(");
    
    for (slp=qslp; slp != NULL; slp=slp->next) {
        if (slp->choice == SEQLOC_PACKED_INT) {
            for (sl = slp->data.ptrvalue; sl; sl=sl->next) {
                if (SeqLocStart(sl) >= from || SeqLocStop(sl) <= to) {
                    continue;
                }
                x = SeqLocStart(sl) * scalex;
                printf("%s", NumToGun(x));
                printf("00");
                y = SeqLocStop(sl) * scaley;
                printf("%s", NumToGun(x));
                printf("%s", NumToGun(y));
            }
        } else {
            if (SeqLocStart(slp) >= from || SeqLocStop(slp) <= to) {
                continue;
            }
            x = SeqLocStart(slp) * scalex;
            printf("%s", NumToGun(x));
            printf("00");
            y = SeqLocStop(slp) * scaley;
            printf("%s", NumToGun(x));
            printf("%s", NumToGun(y));
        }
    }
    printf(")");
    
    hsat = NULL;
    index = 0;
    
    for (sap=seqalign; sap != NULL; sap = sapnext) {
        if (sap->segtype > 3) {	
            sap = sap->segs;
        }
        sapnext = sap->next;
        sap->next = NULL;
        sat = SeqAnnotNew();
        sat->type = 2;
        sat->data = sap;
        hsat = tie_next_annot(hsat, sat);
        
        if (index%2 == 0) {
            printf("3399CC3(");
        } else {
            printf("8dC7CC3(");
        }
        if (sap->segtype == SAS_DENDIAG) {
            ddp = sap->segs;
            while (ddp != NULL) {
                if (ddp->strands[0] != Seq_strand_minus) {
                    x = ddp->starts [0] * scalex;
                } else {
                    x = (ddp->starts [0] + ddp->len) * scalex;
                }
                if (ddp->strands[1] != Seq_strand_minus) {
                    y = yy - ddp->starts [1] * scaley;
                } else {
                    y = yy - (ddp->starts [1] + ddp->len) * scaley;
                }
                printf("%s", NumToGun(x));
                printf("%s", NumToGun(y));
                if (ddp->strands[0] != Seq_strand_minus) {
                    x = (ddp->starts [0] + ddp->len) * scalex;
                } else {
                    x = ddp->starts [0] * scalex;
                }
                if (ddp->strands[1] != Seq_strand_minus) {
                    y = yy - (ddp->starts [1] + ddp->len) * scaley;
                } else {
                    y = yy - ddp->starts [1] * scaley;
                }
                printf("%s", NumToGun(x));
                printf("%s", NumToGun(y));
                ddp = ddp->next;
            }
            printf(")");
        } else if (sap->segtype == SAS_DENSEG) {
            dsp = sap->segs;
            for (index1=0; index1 < dsp->numseg; index1++) {
                i2 = 2 * index1;
                if (dsp->starts[i2] != -1 && dsp->starts[i2+1] != -1) {
                    if (dsp->strands[i2] != Seq_strand_minus) {
                        x = (dsp->starts[i2] - from) * scalex;
                    } else {
                        x = (dsp->starts[i2] + dsp->lens[index1] - from) * scalex;
                    }
                    if (dsp->strands[i2+1] != Seq_strand_minus) {
                        y = yy - (dsp->starts[i2+1] -ffrom) * scaley;
                    } else {
                        y = yy - (dsp->starts[i2+1] + dsp->lens[index1] - ffrom) * scaley;
                    }
                    printf("%s", NumToGun(x));
                    printf("%s", NumToGun(y));
                    if (dsp->strands[i2] != Seq_strand_minus) {
                        x = (dsp->starts[i2] + dsp->lens[index1] - from) * scalex;
                    } else {
                        x = (dsp->starts[i2] - from) * scalex;
                    }
                    if (dsp->strands[i2+1] != Seq_strand_minus) {
                        y = yy - (dsp->starts[i2+1] + dsp->lens[index1] - ffrom) * scaley;
                    } else {
                        y = yy - (dsp->starts[i2+1] - ffrom)* scaley;
                    }
                    printf("%s", NumToGun(x));
                    printf("%s", NumToGun(y));
                }
            }
            printf(")");
        } else if (sap->segtype == SAS_STD) {
            SeqLocPtr tmp_slp;
            ssp = sap->segs;
            while (ssp) {
                tmp_slp = ssp->loc;
                if (!tmp_slp || !tmp_slp->next) {
                    ssp = ssp->next;
                    continue;
                }
                if (tmp_slp->choice != SEQLOC_EMPTY && 
                    tmp_slp->next->choice != SEQLOC_EMPTY) {
                    
                    if (SeqLocStrand(tmp_slp) != Seq_strand_minus) {
                        x = (SeqLocStart(tmp_slp) - from) * scalex;
                    } else {
                        x = (SeqLocStop(tmp_slp) + 1 - from) * scalex;
                    }
                    if (SeqLocStrand(tmp_slp->next) != Seq_strand_minus) {
                        y = yy - (SeqLocStart(tmp_slp->next) - ffrom) * scaley;
                    } else {
                        y = yy - (SeqLocStop(tmp_slp->next) + 1 - ffrom) * scaley;
                    }
                    printf("%s", NumToGun(x));
                    printf("%s", NumToGun(y));
                    if (SeqLocStrand(tmp_slp) != Seq_strand_minus) {
                        x = (SeqLocStop(tmp_slp) + 1 - from) * scalex;
                    } else {
                        x = (SeqLocStart(tmp_slp) - from) * scalex;
                    }
                    if (SeqLocStrand(tmp_slp->next) != Seq_strand_minus) {
                        y = yy - (SeqLocStop(tmp_slp->next) + 1 - ffrom) * scaley;
                    } else {
                        y = yy - (SeqLocStart(tmp_slp->next) - ffrom) * scaley;
                    }
                    printf("%s", NumToGun(x));
                    printf("%s", NumToGun(y));
                }
                ssp = ssp->next;
            }
            printf(")");
        }
        
        index++;
    }

#ifndef BL2SEQ_STANDALONE
    loginfo.size = index;
#endif

    printf("\">\n");
    printf("</td><td valign=bottom><b>1</b></td></tr></table>\n");
    printf("<BR>");
    
    color = 1;
    index = 0;
    printf("<font color=#FF0000>NOTE:</font>The statistics (bitscore and expect value) is calculated based on the size of nr database<BR><BR>\n");
    
    if (align_type == blast_type_blastn)
       printf("<font color=#FF0000>NOTE:</font>If protein translation is reversed, please repeat the search with reverse strand of the query sequence<BR><BR>\n");

    for (sat=hsat; sat != NULL; sat=satnext) {
        satnext=sat->next;
        sat->next = NULL;
        
        x = y = 450;
        
        ll = MAX(len1*x_factor, len2*y_factor);
        scalex = (FloatHi) x*x_factor/ll;
        scaley = (FloatHi) y*y_factor/ll;
        k = 0;
        
        if ((sap = sat->data) != NULL) {
            if (index%2 == 0) {
                color = 1;
            } else {
                color = 2;
            }
            printf("<a name=%d>\n", index);
            PrintOutScore(sap, is_prot, NULL, mask);
            if (sap->segtype == SAS_DENDIAG) {
                for (ddp = sap->segs, index1=0; ddp; ddp= ddp->next, index1++);
            } else if (sap->segtype == SAS_DENSEG) {
                dsp = sap->segs;
                index1 = dsp->numseg;
            } else if (sap->segtype == SAS_STD) {
                ssp = sap->segs;
                for (ssp = sap->segs, index1=0; ssp; ssp= ssp->next, index1++);
            }
            rect = MemNew((2*index1+3)*sizeof(PrymPtr));
            rectY = MemNew((2*index1+3)*sizeof(PrymPtr));
            
            k = CreateRectAlign(sap, rect, rectY, scalex, scaley,
                                len1, len2, color, from, ffrom, to, tto);
            DrawRectAlign(rect, k, color, 11, -1);
            MemFree(rect);
            printf("<BR>\n");
            DrawRectAlign(rectY, k, color, 11, -1);
            MemFree(rectY);
            printf("<BR><BR>\n");
            index++;
        }
           
        txoption = 0;
        txoption += TXALIGN_COMPRESS;
        txoption += TXALIGN_END_NUM;
        txoption += TXALIGN_SHOW_GI;
        txoption += TXALIGN_MATRIX_VAL;
        txoption += TXALIGN_SHOW_QS;
           
        if (StringICmp(progname, "blastx") == 0)
            txoption += TXALIGN_BLASTX_SPECIAL;
        
        txoption += TXALIGN_HTML;
        
        AddAlignInfoToSeqAnnot(sat, align_type); 
        
        ShowTextAlignFromAnnot(sat, 60, stdout, NULL, NULL,
                               txoption, txmatrix, mask, NULL);
    }

cleanup:
    MemFree(seq_1);
    MemFree(seq_2);
    SeqLocSetFree(slp1);
    SeqLocSetFree(slp2);
    if (one && *one != NULLB)
        BioseqUnlock(query_bsp);
    else
        SeqEntryFree(query_sep);
    if (two && *two != NULLB)
        BioseqUnlock(subject_bsp);
    else
        SeqEntryFree(subject_sep);

    BlastTimeFillStructure(&time_keeper);
    printf("<pre>\n");
    fprintf(stdout, "CPU time: %8.2f user secs.\t%8.2f sys. "
            "secs\t%8.2f total secs.\n\n", 
            time_keeper.user, time_keeper.system, time_keeper.total);
    init_buff();
    /*PrintDbReport(dbinfo, 70, stdout);*/
    
    blast_matrix = BLAST_MatrixDestruct(blast_matrix);
    if (txmatrix)
        txmatrix = TxMatrixDestruct(txmatrix);
    dbinfo = TxDfDbInfoDestruct(dbinfo);
#ifndef USE_OLD_BLAST
    if (s_return) {
        if (s_return->ka_params) {
           PrintKAParameters(s_return->ka_params->Lambda, s_return->ka_params->K, 
                             s_return->ka_params->H, 70, stdout, FALSE);
        }
        if (s_return->ka_params_gap) {
           PrintKAParameters(s_return->ka_params_gap->Lambda, 
                             s_return->ka_params_gap->K, 
                             s_return->ka_params_gap->H, 70, stdout, TRUE);
        }
        BlastProgram2Number(progname, &program_number);
        buffer = Blast_GetParametersBuffer(program_number, s_return);
        PrintTildeSepLines(buffer, 70, stdout);
        sfree(buffer);
        Blast_SummaryReturnFree(s_return);
    }
#else
    if (ka_params) {
        PrintKAParameters(ka_params->Lambda, ka_params->K, ka_params->H, 
                          70, stdout, FALSE);
        MemFree(ka_params);
    }
    if (ka_gap_params) {
        PrintKAParameters(ka_gap_params->Lambda, ka_gap_params->K, 
                          ka_gap_params->H, 70, stdout, TRUE);
        MemFree(ka_gap_params);
    }
    PrintTildeSepLines(buffer, 70, stdout);
    MemFree(buffer);
    free_buff();
#endif
    mask_head = mask;
    while (mask) {
        SeqLocSetFree(mask->data.ptrvalue);
        mask = mask->next;
    }
    ValNodeFree(mask_head);

    CreateTailHTML();
    run_status = WBLAST2_DONE;
    options = BLASTOptionDelete(options);
    WWWInfoFree(theInfo->info);
    MemFree(progname);
    MemFree(theInfo);
#ifndef BL2SEQ_STANDALONE 
    /* Connection thread not spawned for the WWW server version */
    NlmThreadJoin(connection_thread, &thrstat);
    logmsg(0);
    LogInfoFree();
#endif
    return 0;
}
