static char const rcsid[] = "$Id: suggest.c,v 6.11 2003/05/30 17:25:38 coulouri Exp $";

/*   suggest.c
* ===========================================================================
*
*                            PUBLIC DOMAIN NOTICE
*            National Center for Biotechnology Information (NCBI)
*
*  This software/database is a "United States Government Work" under the
*  terms of the United States Copyright Act.  It was written as part of
*  the author's official duties as a United States Government employee and
*  thus cannot be copyrighted.  This software/database is freely available
*  to the public for use. The National Library of Medicine and the U.S.
*  Government do not place any restriction on its use or reproduction.
*  We would, however, appreciate having the NCBI and the author cited in
*  any work or product based on this material
*
*  Although all reasonable efforts have been taken to ensure the accuracy
*  and reliability of the software and data, the NLM and the U.S.
*  Government do not and cannot warrant the performance or results that
*  may be obtained by using this software or data. The NLM and the U.S.
*  Government disclaim all warranties, express or implied, including
*  warranties of performance, merchantability or fitness for any particular
*  purpose.
*
* ===========================================================================
*
* File Name:  suggest.c
*
* Author:  Webb Miller, Karl Sirotkin, Warren Gish, Jonathan Kans, Yuri Sadykov
*
* Version Creation Date:   3/17/94
*
* $Revision: 6.11 $
*
* File Description: 
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* ==========================================================================
*/

/*
* =======================================================
* File Name: suggest.c
*
* Author: This is actually a good question!
*     The initial version (called exons.c) was coded by Webb Miller.
*     Warren Gish then incorporated it into the Markup screen package.
*     Karl Sirotkin then modified it. 
*
*     The first modification was to convert a single unassigned
*     codon to a split codon assignment, if this gave a good mapping.
*     It was then realized that even if no unassigned codons exeisted,
*     a better mapping might split codons.
*     
*     The blow by blow is detailed below, but other additions
*     included allowing single mismatched assignemnts within
*     an interval and relaxing some error conditions.
*     
*     If it can be easily done, I (Karl) hope to keep the
*     intermediate solutions in a way that the indexers can
*     use to deal more effectively with strange papers.
*     
*     A detailed change log follows:
*     $Log: suggest.c,v $
*     Revision 6.11  2003/05/30 17:25:38  coulouri
*     add rcsid
*
*     Revision 6.10  2003/05/13 16:02:54  coulouri
*     make ErrPostEx(SEV_FATAL, ...) exit with nonzero status
*
*     Revision 6.9  2002/02/06 21:10:38  kans
*     PlusStrand and MinusStrand prefixed with Suggest to avoid collision with asn2graphic viewer symbols
*
*     Revision 6.8  2001/05/25 19:43:53  vakatov
*     Nested comment typo fixed
*
*     Revision 6.7  1998/06/17 21:55:47  kans
*     fixed 64-bit SGI compiler warnings
*
*     Revision 6.6  1998/06/12 20:42:18  kans
*     fixed most unix compiler warnings
*
*     Revision 6.5  1997/12/02 17:43:52  kans
*     added cast to sprintf, explicit int return values
*
*     Revision 6.4  1997/08/27 18:26:19  kans
*     temporarily convert * to X to avoid bad triptab index
*
*     Revision 6.3  1997/08/27 14:50:23  kans
*     added second protection against table overflow
*
*     Revision 6.2  1997/08/26 23:56:02  kans
*     added protection to bld_table based on purify array bounds violations
*
*     Revision 6.1  1997/08/26 19:37:36  kans
*     added batch functions to process nucleotide once for many proteins
*
*     Revision 6.0  1997/08/25 18:54:58  madden
*     Revision changed to 6.0
*
*     Revision 5.4  1997/08/21 20:28:44  kans
*     fixed correction for alt start codon (Karl)
*
*     Revision 5.3  1996/10/16 15:09:05  kans
*     finished cleaning up linked lists headed by triptab arrays
*
 * Revision 5.2  1996/10/15  22:39:24  kans
 * cleaned up most memory leaks detected by purify (one big one remains)
 *
 * Revision 5.1  1996/09/27  19:05:18  kans
 * adjusts first interval if first protein residue isn't included, under
 * the assumption that it's using an alternative start codon
 *
 * Revision 5.0  1996/05/28  13:43:15  ostell
 * Set to revision 5.0
 *
 * Revision 1.5  1996/03/27  19:40:25  vakatov
 * Some unused stuff has been eliminated
 *
 * Revision 1.4  1996/03/25  22:13:06  vakatov
 * One more  free() -> MemFree()  correction...
 *
 * Revision 1.3  1996/03/25  21:43:06  vakatov
 * The "reverse" function slightly changed to avoid pointing to the outside
 * of array "r[]" lower limit (to please the Win-16 segmentation quirks)
 *
 * Revision 1.2  1996/03/13  21:07:40  epstein
 * change printfs to ErrPostEx()
 *
 * Revision 1.1  1995/11/28  22:19:10  kans
 * Initial revision
 *
 * Revision 1.4  1995/09/22  21:46:16  kans
 * split big array into 26 small arrays, other cleanups
 *
 * Revision 1.3  1995/08/29  16:29:51  epstein
 * eliminate troublesome header
 *
 * Revision 1.2  1995/08/18  19:52:32  epstein
 * *** empty log message ***
 *
 * Revision 1.1  1995/08/14  18:24:21  epstein
 * Initial revision
 *
 * Revision 1.8  1995/07/07  14:44:05  kans
 * updated genetic codes
 *
 * Revision 1.7  1994/07/26  18:04:55  kans
 * *** empty log message ***
 *
 * Revision 1.6  1994/07/01  14:38:22  kans
 * *** empty log message ***
 *
 * Revision 1.5  1994/06/30  22:36:54  kans
 * *** empty log message ***
 *
 * Revision 1.4  1994/06/24  23:03:46  kans
 * *** empty log message ***
 *
 * Revision 1.3  1994/05/12  18:21:31  kans
 * *** empty log message ***
 *
 * Revision 1.2  1994/04/08  16:24:37  kans
 * *** empty log message ***
 *
 * Revision 1.1  1994/03/17  16:33:30  kans
 * Initial revision
 *
 * Revision 2.90  92/07/22  10:46:21  sirotkin
 * forgot to put in earlier
 * 
 * Revision 2.86  92/04/16  15:37:39  sirotkin
 * looks for stops activated
 * 
 * Revision 2.85  92/02/26  14:57:25  sirotkin
 * fixed desending checks in Int_INt2Ext
 * 
 * Revision 2.83  92/01/13  10:42:47  sirotkin
 * lonum guarenteed to be <= hunum
 * 
 * Revision 2.82  91/11/26  17:22:43  sirotkin
 * passed saber, adds terminal stop, conditionally
 * 
 * Revision 2.81  91/11/18  16:00:34  sirotkin
 * Fixed problem pub 34736 which caused dramtic sugest failure
 * 
 * Revision 2.3  91/10/22  13:14:10  sirotkin
 * Now picks up and rewrites minimum exon and range to explore
 * 
 * Revision 2.2  91/10/11  09:50:00  sirotkin
 * *** empty log message ***
 * 
 * Revision 2.1  91/10/11  09:48:30  sirotkin
 * getting ready for range limitation
 * 
 * Revision 2.0  91/04/25  17:21:02  sirotkin
 * To set revision of procution version
 * 
 * Revision 1.104  91/03/26  15:46:13  sirotkin
 * fixed Dramatic failure test to use pro_len and not dna_len
 * 
 * Revision 1.103  91/03/21  10:36:06  sirotkin
 * changed field string expected on form from
 * exception to genetic_code
 * 
 * Revision 1.102  91/03/01  10:49:02  sirotkin
 * ready to install
 * 
 * Revision 1.101  91/02/19  09:05:00  sirotkin
 * Shoulod be ready for next install
 * 
 * Revision 1.100  91/02/15  09:07:13  sirotkin
 * after bug fixes in redundant with mismatches
 * 
 * Revision 1.99  91/02/14  16:25:15  sirotkin
 * removes redunant intervals, coaleses multiple mismatches
 * removes during consisten set insure one at a time
 * more stringent during weirdness pass 1
 * if unkonw genetic code, uses standard
 * 
 * Revision 1.91  91/01/30  14:10:25  sirotkin
 * This version works with ambigous bases.  When an
 * ambiguous base still allows knowledge of amino acid, that
 * amino acid is used.  Matches ambiguous DNA with unknown amino
 * acids.  Suggests last 2 bases of incomplete codon when will
 * code for correct amino acid.
 * 
 * Revision 1.90  91/01/22  16:23:27  sirotkin
 * markup rev 1.90 mini-load for 23 jan 1991
 * 
 * Revision 1.26  91/01/18  16:05:03  sirotkin
 * changed MAX_DNA to 1000000 from (3*32*1024)
 * 
 * Revision 1.25  91/01/16  17:28:31  sirotkin
 * added checks to genetic code
 * 
 * Revision 1.27  91/01/10  15:13:30  sirotkin
 * back to variable length only
 * 
 * Revision 1.25  90/12/28  15:43:54  sirotkin
 * This version has the variable minimum exon length installed
 * 
 * Revision 1.24  90/12/28  09:03:06  sirotkin
 * Before making TooShort a variable which is set depending upon
 * the aa length and the combined dna_length
 * 
 * Revision 1.23  90/12/21  16:48:41  sirotkin
 * change to keep compiler happy
 * 
 * Revision 1.23  90/12/21  15:28:27  sirotkin
 * tweaks for compiler
 * 
 * Revision 1.22  90/12/20  16:47:43  sirotkin
 * minor cosmetic stuff
 * 
 * Revision 1.22  90/12/19  15:12:27  sirotkin
 * ready to install
 * 
 * Revision 1.21  90/11/26  15:02:12  sirotkin
 * with changes to interval group and writing frame to form
 * 
 * Revision 1.20  90/11/24  16:04:06  sirotkin
 * frame moved from intervals to form
 * 
 * Revision 1.19  90/11/23  12:20:37  sirotkin
 * before moving frame from intervals to form
 * 
 * Revision 1.18  90/11/02  15:34:18  sirotkin
 * commented out DEBUG
 * 
 * Revision 1.17  90/10/31  13:54:11  sirotkin
 * Segmented sequences now work.
 * 
 * Revision 1.14  90/10/22  15:06:38  sirotkin
 * Now reports revision number in the testname field
 * 
 * Revision 1.12  90/10/04  18:00:21  sirotkin
 * Unexpectedly, this version worked for id_seq 9121, the
 * last three intervals (shorter gene)
 * 
 * Revision 1.11  90/10/02  15:30:30  sirotkin
 * Only one change, but important enough for a separate version:
 * changed the constant TooShort from 6 to 4, allowing 5 codon
 * segments.
 * 
 * Revision 1.10  90/09/24  14:41:58  sirotkin
 * logic of selecting best set, fixing overlaps
 * by looking at likely splice sites and tehn
 * insuring consistent set by deleting, fi necessary,
 * all seems to work in this version
 * 
* only important changes left in below
revision 1.6        
date: 90/09/19 11:48:09;  author: sirotkin;  state: Exp;  lines added/del: 450/40
partially retro-fitted function headers
----------------------------
revision 1.1        
date: 90/09/07 09:18:17;  author: sirotkin;  state: Exp;  
Initial revision
=============================================================================
 * 
==========================================
* exons.c - map an amino acid sequence to a DNA coding region
*
* The command has the form
*	exons datafile
* where datafile is organized as follows.
*
* The first line contains an integer giving the estimated number of exons
* (this information is not used by the exons program).  The next line gives
* the genetic code to use.  For example, the usual code is given by the line:
* FFLLSSSSYY**CC*WLLLLPPPPHHQQRRRRIIIMTTTTNNKKSSRRVVVVAAAADDEEGGGG
* since TTT codes for F, TTC for F, TTA for L, TTG for L, TCT for S, etc.
* The third line contains the protein sequence ID (an integer), followed by
* one or more lines giving the protein sequence (in upper case letters).  Then
* comes the nucleic acid sequence ID (another integer) followed by the nucleic
* acid sequence itself (an arbitrary number of lines).
*
* The computed mapping from the amino acid sequence to the DNA sequence has
* the form:
*
* 4
* 3827
* 5162 4917
* 4570 3671
* residue 383 (D) is unassigned
* 3668 2694
*
* The first two lines merely echo the protein sequence ID and the nucleic acid
* sequence ID.  Each of the following lines gives either the end positions in
* the nucleic acid sequence of a proposed exon, in order of ascending positions
* in the amino acid sequence, or a region of the amino acid sequence that was
* not assigned to a sequence of codons.  In this hypothetical output, the
* protein was located on the reverse complement strand of DNA.  There are
* apparently only two exons, but for some reason (e.g., sequencing or
* typographical errors) the second exon was corrupted by deletion of a base.

  SPLIT_CODON_FOR_UNASSIGNED 7 Sept 90  Karl Sirotkin

	LOGIC_NOTES
(There is an <asc> in column one of the above line which
signals 'enwrap' that a new title follows)

  In the case where the exon boundary splits a codon, this code left a
  single unassigned aa.  To fix this, a routine was added before report
  which looks for single unassigned bases and looks to see if the intervals
  can be altered to split either 1/2 or 2/1 (in bases) to pick up the missing
  amino acid.  If both work, the one which generates the exon/gtINTRONag/exon
  consensus (or at least the gs will be used).  Because for struct match
  (for m_stk[]) a single unit implies 3 bases, two additional char fields have
  been added to keep track of the ragged boundaries.  This is, the base end will
  be 3 X Number_of_codons +/- these new char values.

  Search on SPLIT_CODON_FOR_UNASSIGNED to find all changes related to this.

	 The changes have become more extensive than this.  It will no longer be
	 practical to search on a particular term to find a change.  However,
	 the debugging prints will allow at least a partical thread of individual changes
	 to be followed.  The additional logic changes follow:

		1. Intervals that are unassigned and leave 3 bases in between will
		   be coalesced, as these are likely to be do to simple errors of
		   one sort or another.

		2. Even if unsplit codons occur at boundaries, a more likely break
		   may occur with split codons.  Related to this, occasionally
		   (see id_seq 8970) overlapping intervals are created by chance
		   when looking at consensus splice juctions can resolve overlap.

		3. Relax some error conditions so that:
			a. Inverted or tandem partial repeats do not cause reportable errors.
			b. Best alignment, if possibly partially correct will be reported:
				i. Ignore gaps at extreme end of aa sequence.

		4. Track segmented entries by concatenating dna sequences together, but
		   then separating them before reporting

SEGMENTED ENTRIES

This capibility was added by Sirotkin in the following way:

	1. These variables were made arrays to hold info for each
	segment:
 nt_id_seq = (Int4 *) ckalloc (max_num_nts * sizeof(Int4) );
 ntjsr = (JobSeqRec *) ckalloc ( max_num_nts * sizeof(JobSeqRec));
	nt_orient = (DBTINYINT * ) 
		ckalloc ( max_num_nts * sizeof(DBTINYINT));
 dbnt_seq = (SeqPtr*) ckalloc ( max_num_nts * sizeof(SeqPtr) );
	nt_dna_len = (Int4 * ) ckalloc (max_num_nts * sizeof(Int4));
	nt_len_offsets = (Int4 * ) ckalloc (max_num_nts * sizeof(Int4));

	SeqPtrsForIntLst = (SeqPtr*) ckalloc (MaxRows * sizeof(SeqPtr) );
	JobPtrsForIntLst = (JobSeqRec*) ckalloc (MaxRows * sizeof(JobSeqRec) );
	
	2. The plus strand of each segment will be concatenated in order.

	3. Old login then untouched.

	4. Modify the AddIntervals call and code to recover
	   the information from segments, being prepared to split 
	   a suggest interval into two database intervals, if a segment
	   boundary is crossed.

    
A new function SeqRevComp() within seq.c is required
to complement the sequence, if the minus strand was given
for any particular segment

*/

/* with the cursor on this line, the following
command adds comments on defines
:+6,+25s/#define DEBUG\([ _A-Z]*\)/ \/\* #define DEBUG\1 \*\/
*/

/* with the cursor on this line, the following
command removes comments on defines
:+3,+22s/ \/\* #define DEBUG\([ _A-Z]*\) \*\//#define DEBUG\1/ */
 /* #define DEBUG  */
 /* #define DEBUG_SCORE */
 /* #define DEBUG_SPLIT_CODON_FOR_UNASSIGNED */
 /* #define DEBUG_SINGLE_MISMATCH */
 /* #define DEBUG_COALESCE_MISMATCH */
 /* #define DEBUG_FIX_OVERLAPPING_ASSIGNMENTS */
 /* #define DEBUG_FIND_SET */
 /* #define DEBUG_FIND_BEST_SET */
 /* #define DEBUG_FIND_BEST_SET_KILL */
 /* #define DEBUG_DETAIL_BEST   */
 /* #define DEBUG_DETAIL_COLLIDE   */
 /* #define DEBUG_DETAIL_OVERLAP_AAS   */
 /* #define DEBUG_DETAIL_WEIRDNESS  */
 /* #define DEBUG_DETAIL_ADD_ENOUGH  */
 /* #define DEBUG_SEGMENTS  */
 /* #define DEBUG_INIT_GET_FRAMES */
 /* #define DEBUG_TOO_SHORT */
 /* #define DEBUG_REDUNDANT */
 /* #define DEBUG_MULIPLE_MISMATCH */
 
#include <ncbi.h>
#include <objall.h>
#include <sequtil.h>
#include <suggest.h>

SuggestOutput	suggestOut = {{NULL}, FALSE};

IntPtr intlist = NULL;

static char
	*frame[3];

Int4	dna_len;

static Int4
	* nt_dna_len,
	* nt_len_offsets ,
	orientation,
	shift;

Match * m_stk = NULL;
Int4 m_top;
Int4 vx[128];
Int4 TooShort;			/* keep matches of length > TooShort */

Int4 pro_len;
static Char *genetic_code = NULL;
static Char *p_seq;
Char *rev_seq;
Char *dna_seq;

typedef struct seqptr {
  CharPtr  seq;
  CharPtr  name;
  Int4     Origin;
  Boolean  hasZero;
  Boolean  ascending;
} *SeqPtr;

static Int4 min_exon_from_form;
static DBTINYINT last_orient;

static char	*module = "APT_suggest";
static DBSMALLINT	exception;
static Int4 * nt_id_seq = NULL;
#ifdef DEBUG_TOO_SHORT 	
static Int4	aa_id_seq;
#endif
static Int4 num_nts; /* number of nucleotides > 1 if segmented */
static Int4 max_num_nts;
static DBTINYINT * nt_orient = NULL;
static SeqPtr		dbaa_seq, * dbnt_seq= NULL;
static	Int4	aori, ahz, aasc;

Int4 Total_len;

#define fatal(m)	{ErrPostEx(SEV_FATAL, 1, 0, "%s:  %s", module, m); goto Error;}

static char * suggest_revision = "$Revision: 6.11 $";

static Int4	suggest_get_frames_init = 1;

Int4 Min_val = -1, Max_val = -1;
Int4 Look_for_stop;


/*****************************************************************************
*
*   TripletNode
*       Element of table to hold residue triplets for fast lookup
*
*****************************************************************************/

typedef struct tripletnode {
  Int4                position;
  struct tripletnode  PNTR link;
} TripletNode, PNTR TripletNodePtr;

/*****************************************************************************
*
*   Static Variables
*
*****************************************************************************/

SuggestRec  suggestRec = {{NULL, NULL, NULL, NULL, 0L, TRUE, FALSE},
			  {NULL, NULL, NULL, NULL, 0L, FALSE, FALSE},
			  NULL, 0L, 0L, 0L, FALSE};

static TripletNodePtr  PNTR tripletTable = NULL;

static IdRecPtr ClearIdRec (IdRecPtr irp)

{
  while (irp != NULL) {
    MemFree (irp->id.accn);
    irp = irp->next;
  }
  return NULL;
}

void ClearSeqRec (SeqRecPtr srp)

{
  if (srp != NULL) {
    srp->rawSeq = BSFree (srp->rawSeq);
    srp->segLens = BSFree (srp->segLens);
    srp->ids = ClearIdRec (srp->ids);
    srp->sequence = MemFree (srp->sequence);
    srp->reverse = MemFree (srp->reverse);
    srp->length = 0L;
    srp->lookForStop = FALSE;
  }
}

/*
* =================================================================
 ----- bld_table () --
* ======================================
*
* Description: build table of residue triples
*              used for FAST lookup
*
* Note that structure trip_node is defined here
*
* Return Value: 1 is memory could not be had, 0 on success
*
* Input Via Formal Parameters:none
*
* Output Via Formal Parameters: none
*
* Input Via Global Variables: p_seq (protein aa seq)
*
* Output Via Global Variables: triptab
*
*   Karl Sirotkin 9/22/95:
*    To workaround the 64K DOS limit, the triptab array
*    is being split into 27 arrays.  See below, the access is
*    either linear, by total size, or by i,j,k triplets.
*    Arbitrarily, the first i will access 26 pointers to arrays.
*     The 27ths array is for 'ZZZ' +1 for stops and unknowns
*     The first 26 will be 26*26 long.
* 
===========================*/


typedef struct trip_node {
	Int4 pos;		/* position of word in protein */
	struct trip_node *link;	/* next occurrence of same three letters */
} TripNode;

static struct trip_node *** triptab = NULL;

/*
	Each residue triple is encoded as:

	tval = (word[0] - 'A')*676 + (word[1] - 'A')*26 + word[2] - 'A';
	stops and unknown use "ZZZ" + 1

  Now, word[0] determines array, (word[1] - 'A')*26 + word[2] - 'A'
  determines index within array.  stops and unknown use position
  [0] of 27th array.
*/
/* static struct trip_node *triptab[26*26*26 +1]; */ /* tval of "ZZZ" is 17575 */
/*  #define USE_STOPS */

static int
bld_table(void)
{
	register struct trip_node	*h;
	register Int4	tval1, tval2;
	register char	*p;
	Boolean lastchar;

	for (p = p_seq; p[WORD_SIZE-1] != '\0'; p++) {
		lastchar = (Boolean) (p[2] == '*');
		if (lastchar) {
		  p[2] = 'X';
		}
		/*  OLD WAY: Karl Sirotkin 9/22/95 tval = (p[0] - 'A')*(26*26) + (p[1] - 'A')*(26) + (p[2] - 'A')*(1); */
		tval1 = (p[0] - 'A');
		tval2 = (p[1] - 'A')*(26) + (p[2] - 'A')*(1); 
		if (lastchar) {
		  p[2] = '*';
		}

		if (tval1 > 25 || tval1 < 0 || tval2 >= 26*26 || tval2 < 0) continue; /* JK 8/26/97 */

#ifdef USE_STOPS
		if (tval1 > 25 || tval1 < 0 || tval2 >= 26*26 || tva2 < 0){
			tval1 = 26;
			tval2 = 0;
		}
#endif
		h = (struct trip_node *) MemNew (sizeof(struct trip_node));
		if (h == NULL)
			return 1;
		h->pos = p - p_seq;
		h->link = (triptab[tval1])[tval2];
		(triptab[tval1])[tval2] = h;
	}
	return 0;
}

/*
* =================================================================
 ----- get_frames () --
* ======================================
*
* Description: put amino acid sequences from DNA sequence s in frame[0..2]
*
* Return Value: sequence length on success, 0 on error/failure
*  wrong - returns 0 on success, 1 on failure
*
* Input Via Formal Parameters:s is the dna sequence
*
* Output Via Formal Parameters: none
*
* Input Via Global Variables: dna_len
*
* Output Via Global Variables: the global variable vx is intialized
*                              char * frames[3] is defined
* 
===========================*/

/*SPLIT_CODON_FOR_UNASSIGNED make vx[] global for use in split codon problem */

static int
get_frames(char *s)
{
	register Int4	i, j, val;
	register char	*t;
 register char * t_max = s + dna_len -2;

	if (suggest_get_frames_init) {
#ifdef DEBUG_INIT_GET_FRAMES
	ErrPostEx(SEV_ERROR, 0, 0, "DEBUG_INIT_GET_FRAMES initializing in get_frames\n");
#endif
		suggest_get_frames_init = 0;
		for (i = 0; i < 128; ++i)
			vx[i] = -10000;
		vx['T'] = vx['U'] = vx['t'] = vx['u'] = 0;
		vx['C'] = vx['c'] = 1;
		vx['A'] = vx['a'] = 2;
		vx['G'] = vx['g'] = 3;
	}
	if (genetic_code == NULL) {
	  genetic_code = suggestRec.geneticCode;
	}

	for (i=0; i<3; ++i) {
		if (frame[i] != NULL)
			continue;
		if ((frame[i] = MemNew(dna_len/3 + 10)) == NULL)
			return 1;
	}

	for (i = 0; i <= 2; ++i) {
		for (j = 0, t = s+i;  t <= t_max ; ++j, t += 3) {
			val = vx[t[0]]*16 + vx[t[1]]*4 + vx[(t==t_max)?'N':t[2]];
			if ( val < 0){
				frame[i][j] = 'X';
			}else{
				frame[i][j] =  genetic_code[val] ;
				}
		}
		frame[i][j] = '\0';
	}
	return 0;
}

static char *topframe [3];
static char *botframe [3];
static Boolean usingBatchFrames = FALSE;

void SetBatchSuggestFrames (Char *dna_seq, Char *rev_seq, Int4 len)

{
  Int2  i;

  dna_len = len;
  for (i = 0; i < 3; i++) {
    frame [i] = NULL;
    topframe [i] = NULL;
    botframe [i] = NULL;
  }
  if (get_frames(dna_seq) == 0) {
    for (i = 0; i < 3; i++) {
      topframe [i] = frame [i];
    }
  }
  for (i = 0; i < 3; i++) {
    frame [i] = NULL;
  }
  if (get_frames(rev_seq) == 0) {
    for (i = 0; i < 3; i++) {
      botframe [i] = frame [i];
    }
  }
  for (i = 0; i < 3; i++) {
    frame [i] = NULL;
  }
  usingBatchFrames = TRUE;
}

void ClearBatchSuggestFrames (void)

{
  Int2  i;

  for (i = 0; i < 3; i++) {
    topframe [i] = MemFree (topframe [i]);
    botframe [i] = MemFree (botframe [i]);
  }
  usingBatchFrames = FALSE;
  suggestRec.geneticCode = (CharPtr)MemFree(suggestRec.geneticCode);
  genetic_code = NULL;
}

/* =================================================================
 ----- expand () --
* ======================================
*
* Description: generates list of mappings between aa and dna
*
* Return Value: 1 on error, else 0
*
* Input Via Formal Parameters: describe match of 3 residues
*
* Output Via Formal Parameters: unsure, but unlikely
*
* Input Via Global Variables: most of them
*
* Output Via Global Variables: m_stk[] 
* 
===========================*/

static int expand(char *f_seq, Int4 p, Int4 f)
{
	Int4 i, j, k;

	/* is match inside a match we've already found? */
	for (i = 0; i < m_top; ++i)
		if (m_stk[i].orient == orientation &&
		    m_stk[i].fr_shift == shift &&
		    m_stk[i].p_pos <= p &&
		    p < m_stk[i].p_pos + m_stk[i].length)
			return 0;
	/* expand to the left */
	for (i = p, j = f; i > 0 && j > 0; --i, --j)
		if (p_seq[i-1] != f_seq[j-1])
			break;
	/* expand to the right */
	for (p += 2, f += 2; p_seq[p] != NULLB && f_seq[f] != NULLB; ++p,++f)
		if (p_seq[p] != f_seq[f])
			break;
	/* ignore short matches */
	if (p-i <= TooShort)
		return 0;
	if (m_top >= MAX_EXONS) {
		ErrPostEx(SEV_ERROR, 0, 0, "Too many exon possibilities were encountered; check your work, thenhave programmer raise MAX_EXONS");
		return 1;
	}
	if (j*3 >= Min_val &&  (j+ (p-i))*3 <= Max_val){

	/* keep pieces sorted by protein position */
		for (k = m_top; k > 0 && m_stk[k-1].p_pos > i; --k) {
			copy_m_stk_entry (k,k-1);
		}
		m_stk[k].orient = orientation;
		m_stk[k].fr_shift = shift;
		m_stk[k].p_pos = i;
		m_stk[k].dna_pos = j;
		m_stk[k].length = p - i;
		++m_top;
	}
	return 0;
}

/*
* =================================================================
 ----- search/search1 () --
* ======================================
*
* Description: search() drives search1() in each of 3 frames
*              search1 (through calls to expand() )
*              builds the possible segments of matching aa/dna
*
* Return Value: 
*
* Input Via Formal Parameters:frame for search1
*
* Output Via Formal Parameters: none
*
* Input Via Global Variables: triptab
*
* Output Via Global Variables: m_stk[] (indirectly through expand() )
* 
===========================*/

static int search1(char *f)	/* f is the amino acid sequence in one reading frame */
{
	register char	*t;
	struct trip_node	*h;
	Int4		i, j, k;
	Int4	tval1, tval2;
	Boolean lastchar;

	if (f[0] == NULLB || f[1] == NULLB || f[2] == NULLB)
		return 0;

	for (t = f; t[WORD_SIZE-1] != NULLB; ++t) {

#ifndef USE_STOPS
		/* ignore stop codon and unknown codons */
		if ((i = t[0]-'A') < 0 || i > 26 || (j = t[1]-'A') < 0
		   || j > 26 || (k = t[2]-'A') < 0 || k > 26)
			continue;
#else
		i = t[0]-'A';
		j = t[1]-'A';
		k = t[2]-'A';
#endif
		lastchar = (Boolean) (t[2] == '*');
		if (lastchar) {
		  t[2] = 'X';
		}
		tval1 = (t[0] - 'A');
		tval2 = (t[1] - 'A')*(26) + (t[2] - 'A')*(1); 
		if (lastchar) {
		  t[2] = '*';
		}

		if (tval1 > 25 || tval1 < 0 || tval2 >= 26*26 || tval2 < 0) continue; /* JK 8/26/97 */

#ifdef USE_STOPS
		if (tval1 > 25 || tval1 < 0 || tval2 >= 26*26 || tva2 < 0){
			tval1 = 26;
			tval2 = 0;
		}
#endif
		for (h = (triptab[tval1])[tval2] ; h; h = h->link) {
			if (expand(f, h->pos, t-f) != 0)
				return 1;
		}
	}
	return 0;
}

static int search(void)
{
	for (shift = 0; shift <= 2; ++shift) {
		if (search1(frame[shift]) != 0)
			return 1;
	}
	return 0;
}

/*
* =================================================================
 ----- reverse () --
* ======================================
*
* Description: generates antiparallel complement
*
* Return Value: sequence length on success, 0 on error/failure
*
* Input Via Formal Parameters:n is always dna_len
*
* Output Via Formal Parameters: none
*
* Input Via Global Variables: dna_seq
*
* Output Via Global Variables: rev_seq
* 
===========================*/

/*
void
reverse(Int4 n)
{
	register char	*s, *t;
	char r[128];

	r['A'] = 'T';
	r['T'] = 'A';
	r['U'] = 'A';
	r['C'] = 'G';
	r['M'] = 'K';
	r['G'] = 'C';
	r['R'] = 'Y';
	r['S'] = 'S';
	r['V'] = 'B';
	r['W'] = 'W';
	r['Y'] = 'R';
	r['H'] = 'D';
	r['K'] = 'M';
	r['D'] = 'H';
	r['B'] = 'V';
	r['N'] = 'N';
	r['-'] = '-';
	for (s = dna_seq + n, t = rev_seq; s > dna_seq; )
		*t++ = r[*--s];
	*t = '\0';
}
*/

static Int4 DoAddInterval (Int4 segid, Int4 lonum, Int4 hinum, Int4 orient)

{
  IntPtr  last;
  IntPtr  this;
  Int4    swapper;

  if (lonum > hinum) {
    swapper = lonum;
    lonum = hinum;
    hinum = swapper;
  }
  swapper = hinum - lonum +1;
  if (swapper < 0) {
    swapper = -swapper;
  }
  Total_len += swapper;
  if (intlist == NULL) {
    intlist = MemNew (sizeof (IntRec));
    if (intlist != NULL) {
      intlist->from = lonum;
      intlist->to = hinum;
      intlist->orient = orient;
      intlist->id = segid;
    }
  } else {
    last = intlist;
    while (last->next != NULL) {
      last = last->next;
    }
    this = MemNew (sizeof (IntRec));
    if (this != NULL) {
      this->from = lonum;
      this->to = hinum;
      this->orient = orient;
      this->id = segid;
    }
    last->next = this;
  }
  return 0;
}

static Int4 AddInterval (Int4 lonum, Int4 hinum, Int4 orient)

{
  Int4    start_nt;
  Int4    start_limit;

  for (start_nt = 0; start_nt < num_nts; start_nt++) {
    if (lonum  > nt_len_offsets [start_nt] &&
        lonum - 1 <= nt_len_offsets [start_nt] + nt_dna_len [start_nt] -1) {
      break;
    }
  }
  if (start_nt >= num_nts) {
    Message (MSG_FATAL, "Could not find segment containing %ld", (long) lonum);
  }
  start_limit = nt_len_offsets [start_nt] + nt_dna_len [start_nt] - 1;
  if (start_limit < hinum - 1) {
    if (AddInterval (lonum, start_limit + 1, orient) != 0) {
      return 1;
    }
    if (AddInterval (start_limit + 2, hinum, orient) != 0) {
      return 1;
    }
  } else {
    if (nt_orient [start_nt] == SuggestMinusStrand) {
      lonum -= nt_len_offsets [start_nt];
      hinum -= nt_len_offsets [start_nt];
      lonum = nt_dna_len [start_nt] - lonum + 1;
      hinum = nt_dna_len [start_nt] - hinum + 1;
      if (orient == SuggestMinusStrand){
        orient = SuggestPlusStrand;
      } else if (orient == SuggestPlusStrand) {
        orient = SuggestMinusStrand;
      }
      lonum += nt_len_offsets [start_nt];
      hinum += nt_len_offsets [start_nt];
    }
    if (DoAddInterval (nt_id_seq [start_nt],
                       lonum - nt_len_offsets [start_nt],
                       hinum - nt_len_offsets [start_nt],
                       orient) != 0) {
      return 1;
    }
  }
  return 0;
}

/* =================================================================
 ----- report () --
* ======================================
*
* Description: adds intervals to list
*
* Return Value: 1 if above conditions, 0 if intervals look ok.
*
* Input Via Formal Parameters: 
*
* Output Via Formal Parameters: 
*
* Input Via Global Variables: m_stk[]
*
* Output Via Global Variables: 
* 
*===============================================*/
static int
report(void)
{
	Int4 i, from, to;
#ifdef OLD_UNASSIGNED_CODE
	Int4 j, k, last, first;
#endif

#ifdef OLD_CHECKING
	for (i = 1; i < m_top; ++i) {
		if (m_stk[i].orient != m_stk[0].orient)	/* case (i) */
			{dump("different orientations"); return 1;}
		/* last dna position of previous exon */
		last = 3*m_stk[i-1].dna_pos + m_stk[i-1].fr_shift +
			3*m_stk[i-1].length - 1;
		/* first dna position of current exon */
		first = 3*m_stk[i].dna_pos + m_stk[i].fr_shift;
		if (last >= first)	/* case (ii) */
			{dump("overlapping nucleotide sequences"); return 1;}
		/* last protein position for previous exon */
		last = m_stk[i-1].p_pos + m_stk[i-1].length - 1;
		/* first protein position for current exon */
		first = m_stk[i].p_pos;
		if (last >= first)	/* case (iii) */
			{dump("overlapping protein sequences");return 1;}
	}
#endif	
	/* as exons are printed (sorted by protein position) check for
	unassigned residues */
	for (i = 0; i < m_top; ++i) {

#ifdef OLD_UNASSIGNED_CODE
		if (i == 0 && (j = m_stk[0].p_pos) > 0)
			unassigned(0, j-1);
#endif

#ifdef OLD_UNASSIGNED_CODE
		if (i > 0 && (j = m_stk[i-1].p_pos + m_stk[i-1].length - 1) <
		    (k = m_stk[i].p_pos) - 1)
	/* SPLIT_CODON_FOR_UNASSIGNED (override unassigned if was fixed */
			if (j+1 != k-1 || ! m_stk[i-1].left_add_bases)
				unassigned(j+1, k-1);
#endif
		/* add 1 so positions start with 1, not with 0 */
		from = 3*m_stk[i].dna_pos + m_stk[i].fr_shift + 1;
		to = from + 3*m_stk[i].length - 1;

	/* SPLIT_CODON_FOR_UNASSIGNED
		the left reffers to the intron boudaries
		this implements the split codon decisions */

		from -= m_stk[i].right_add_bases;
		to += m_stk[i].left_add_bases;
		if ( to > dna_len){
/*---
 * This can happen with ambiguous nucleotide at the end
 *---*/
			to = dna_len;
		}

		if (m_stk[i].orient == SuggestMinusStrand) {
			from = dna_len - from + 1;
			to = dna_len - to + 1;
		}
		/******* Here's the interesting data *******/
		if (AddInterval(from, to , m_stk[i].orient) != 0)
			return 1;
	}
#ifdef OLD_UNASSIGNED_CODE
	if ((j = m_stk[m_top-1].p_pos + m_stk[m_top-1].length - 1) < pro_len-1)
		unassigned(j+1,pro_len-1);
#endif
	return 0;
}

/* =================================================================
 ----- cleanup () --
* ======================================
*
* Description: Clean up our malloc'ed mess
*  ----------------the rest of this bypassed upon retro fit-----
*
* Return Value: 
*
* Input Via Formal Parameters: 
*
* Output Via Formal Parameters: 
*
* Input Via Global Variables: 
*
* Output Via Global Variables: 
* 
*===============================================*/
/*static void SeqFree (SeqPtr seqptr) {
  if (seqptr != NULL) {
    MemFree (seqptr->seq);
    MemFree (seqptr->name);
  }
}
*/

static void
cleanup(void)
{
  /* Int2 i; */
  /* rev_seq = MemFree (rev_seq); */
  /*
  for (i = 0; i < 3; i++) {
    frame [i] = MemFree (frame [i]);
  }
  */
/*
	register struct trip_node	*h, *oh;
	register Int4	tripdex, i, nt;

	m_top = 0;

	if (dbaa_seq != NULL) {
		SeqFree(dbaa_seq);
		MemFree (&dbaa_seq);
	}
	for (nt = 0; nt < num_nts; nt ++ ){
		if (dbnt_seq[nt] != NULL) {
			SeqFree(dbnt_seq[nt]);
			MemFree ( &dbnt_seq[nt] );
		}
	}
	MemFree(&p_seq);
	MemFree(&dna_seq);
	MemFree(&rev_seq);

	MemFree(& nt_id_seq );
	MemFree ( &nt_orient );
	MemFree(& nt_dna_len );
	MemFree ( &nt_len_offsets);

	for (i=0; i<3; ++i)
		MemFree(&frame[i]);

	for (tripdex = 0; tripdex < 27; tripdex ++){
		for (i=0; i<26*26; ++i) {
			h = (triptab[tripdex])[i];
			while (h != NULL) {
				oh = h;
				h = h->link;
				MemFree(oh);
			}
			(triptab[tripdex])[i] = NULL;
			if (tripdex == 26 ){
				break;
			}
		}
	}
	*/
}

/*
* =================================================================
  ------ APT_suggest () --
* ======================================
*
*  Description: called from forms package
*               when suggest intervals selected from utilities
*               menu.
*  ----------------the rest of this bypassed upon retro fit-----
* Return Value:
*
* Input Via Formal Parameters:
*
* Output Via Formal Parameters:
*
* Input Via Global Variables:
*
* Output Via Global Variables:
*
=========================== */

static Int4 APT_suggest(SuggestOutputPtr pSuggestOut, Int4 num_nuc)
{
	Int4		i;
	Int4 suggest_revision_len = strlen(suggest_revision);
	Int4 nt;
	DBTINYINT frame_1 =1;
	char nt_msg[512];
	double combined_length;

	/* Get the amino acid sequence's id_seq from the form */


	if (Min_val == -1){
		Min_val = 0;
	}else{
		Min_val --;
	}
	if (Max_val <= 1){
		Max_val = dna_len;
	}else{
		Max_val --  ;
	}

#define USE_VARIABLE_LENGTH
#ifdef USE_VARIABLE_LENGTH
combined_length = (double) pro_len * dna_len;
if ( combined_length < 3.E+4) {
	TooShort = 2;
}else if ( combined_length < 1.e+6) {
	TooShort = 3;
} else if (combined_length < 1.e+7){
	TooShort = 4;
}else if (combined_length < 1.e+8){
	TooShort = 5;
}else {
	TooShort = 6;
}
#else
TooShort = 4;
#endif
if (min_exon_from_form > 1){
		TooShort = min_exon_from_form;
}

#ifdef DEBUG_SEGMENTS
ErrPostEx(SEV_INFO, 0, 0, "DEBUG_SEGMENTS segment id_seq orient   len  offset\n\n");
for ( nt=0; nt < num_nts; nt ++ ){
	ErrPostEx(SEV_INFO, 0, 0, "DEBUG_SEGMENTS   %2d %8d      %2d %5d %5d\n",
	nt, nt_id_seq[nt] , nt_orient[nt], nt_dna_len[nt], 
	nt_len_offsets[nt]);
}
#endif

	if ((dna_len+1)/3 < pro_len){
		if (num_nts==1){
			ErrPostEx(SEV_ERROR, 0, 0, "Note:  nucleotide sequence %d is too short to code for the entire protein.",
			nt_id_seq[0]);
		}else{
			nt_msg [0] = '\0';
			sprintf (nt_msg + strlen(nt_msg),
				"Note:  nucleotide sequences :");
			for ( nt = 0; nt < num_nts; nt ++){
				sprintf (nt_msg + strlen(nt_msg),
				" %ld", (long) nt_id_seq[nt]);
			}
			sprintf (nt_msg + strlen(nt_msg),
				"are too short to code for the entire protein");
			ErrPostEx(SEV_ERROR, 0, 0, nt_msg);
		}
	}

if ( dna_len < 18 || pro_len < 6){
	ErrPostEx(SEV_ERROR, 0, 0, " dna length %d and / or protein length %d too short\n",
dna_len, pro_len);
	goto Error;
}
	if (bld_table() != 0)
		goto Error;

	/* Get the genetic code to use */
	if (exception ==0) exception = 1;	/* change unkown to standard */


	if ( ! genetic_code){
		ErrPostEx(SEV_ERROR, 0, 0, "No known genetic code for suggest to use.");
		goto Error;
	}
	if ( ! * genetic_code){
		ErrPostEx(SEV_ERROR, 0, 0, "No known genetic code for suggest to use.");
		goto Error;
	}

	if (usingBatchFrames) {
		for (i = 0; i < 3; i++) {
			frame [i] = topframe [i];
		}
	} else if (get_frames(dna_seq) != 0)
		goto Error;
	orientation = SuggestPlusStrand;
	if (search() != 0)
		goto Error;

	/*
	rev_seq = MemNew(dna_len+1);
	if (rev_seq == NULL)
		goto Error;
	reverse(dna_len);
	*/
	if (! usingBatchFrames) {
		for (i = 0; i < 3; i++) {
			frame [i] = MemFree (frame [i]);
		}
	}

	if (usingBatchFrames) {
		for (i = 0; i < 3; i++) {
			frame [i] = botframe [i];
		}
	} else if (get_frames(rev_seq) != 0)
		goto Error;
	orientation = SuggestMinusStrand;
	if (search() != 0)
		goto Error;

	if (! usingBatchFrames) {
		for (i = 0; i < 3; i++) {
			frame [i] = MemFree (frame [i]);
		}
	}

#ifdef DEBUG_TOO_SHORT 	
ErrPostEx(SEV_INFO, 0, 0, "DEBUG_TOO_SHORT %d %6d %8d %12g %8d %8d\n", 
TooShort, m_top, aa_id_seq, combined_length, pro_len, dna_len);
#endif
if (m_top){
		/*---
		 * before finding best set, combine
		 * intervals caused by single mismatches
		 *---*/
		fix_single_mismatches();  

		find_best_set(TooShort, pro_len);

		remove_redundant_intervals(TooShort);
		/*---
		 * after finding best set, combine
		 * intervals caused by single mismatches
		 *---*/
		fix_single_unassigned(dna_seq, rev_seq, p_seq, genetic_code);  

		combine_with_long_mismatches(TooShort);
		fix_overlaps_by_splice_sites(dna_seq, rev_seq, p_seq, genetic_code);

		ensure_consistent_set(pro_len);
}

/* check for missing first residue, possibly due to alternative start codon */
if (m_top > 0 && m_stk [0].p_pos == 1) {
  /*
  if (m_stk [0].orient == 1) {
    if (m_stk [0].dna_pos > 0) {
      (m_stk [0].dna_pos)--;
      (m_stk [0].length)++;
    }
  } else {
    if ((3 * m_stk [0].dna_pos + m_stk [0].fr_shift +
        3 * m_stk [0].length - 1) <= dna_len - 3) {
      (m_stk [0].dna_pos)++;
      (m_stk [0].length)++;
    }
  }
  */
    if (m_stk [0].dna_pos > 0) {
      (m_stk [0].dna_pos)--;
      (m_stk [0].length)++;
    }
}

	if (report() != 0)
		goto Error;

	if ( Total_len < pro_len * .1) {
		ErrPostEx(SEV_ERROR, 0, 0, "Dramatic Suggest failure: Please check reverse\n on both aa and DNA sequences.\nPlease check that the proper sequences selected.\n");
		if (min_exon_from_form > -1 || Min_val > 0 || Max_val < dna_len){
			ErrPostEx(SEV_ERROR, 0, 0, "Since Parameters were set, also check them on the verify form.\n");
		}
	}

	if (Min_val <=0)
		Min_val = 1;

	if (Max_val <=0 || Max_val > Total_len)
		Max_val = dna_len ;
	min_exon_from_form = TooShort;

    if (pSuggestOut != NULL) {
	OutLocation(pSuggestOut, num_nuc);
    }

	cleanup();
#ifdef CdCycle
ErrPostEx(SEV_INFO, 0, 0, "Ending Suggest");
#endif
	return 0;
Error:
	cleanup();
#ifdef CdCycle
ErrPostEx(SEV_INFO, 0, 0, "Ending Suggest");
#endif
	return 1;
}

/* =================================================================
 ----- dump_m_stk/fprintf_m_stk_entry() --
* ======================================
*
* Description: debugging dumper
*
* Return Value: none
*
* Input Via Formal Parameters: msg for printf and FILE *
*
* Output Via Formal Parameters: none
*
* Input Via Global Variables: m_stk[]
*
* Output Via Global Variables: m_stk[]
* 
*===============================================*/
/*---- fprintf_m_stk_entry() -- */
static void
fprintf_m_stk_entry(FILE *fp, Int4 index, char *msg)
{

fprintf(fp,
"%s Interval %ld orient %ld frame %ld p_pos %ld dna_pos %ld length %ld leftt %ld right %ld\n",
		(msg?msg:""),(long) index,
		(long) m_stk[index].orient ,
		(long) m_stk[index].fr_shift ,
		(long) m_stk[index].p_pos ,
		(long) m_stk[index].dna_pos ,
		(long) m_stk[index].length ,
		(long) m_stk[index].left_add_bases ,
 	(long) m_stk[index].right_add_bases );
}


/*
static Int4
dump_m_stk(FILE *fp, char *msg)
{
Int4 index;
for (index=0; index < m_top; index ++ ) {
	fprintf_m_stk_entry(fp,index, msg);
}
return index;
}
*/



#ifdef OLD_UNASSIGNED_CODE
/* =================================================================
 ----- unassigned () --
* ======================================
*
* Description: print unassigned residues
*
* Return Value: 
*
* Input Via Formal Parameters: bounds of unassigned aa
*
* Output Via Formal Parameters: 
*
* Input Via Global Variables: p_seq
*
* Output Via Global Variables: 
* 
*===============================================*/
static
unassigned(Int4 from, Int4 to)
{
	Int4 i, j;
	char	buf[255];

	if (from == to)
		ErrPostEx(SEV_ERROR, 0, 0, "a.a. residue %d (%c) is unassigned",
				int2ext(to, aori, ahz, aasc),
				p_seq[to]);
	else {
		sprintf(buf, "a.a. residues from %d to %d (",
				int2ext(from, aori, ahz, aasc),
				int2ext(to, aori, ahz, aasc));
		if (to - from < 6) {
			j = strlen(buf);
			bcopy(&p_seq[from], &buf[j], to-from+1);
			buf[j+to-from+1] = NULLB;
		}
		else {
			sprintf(&buf[strlen(buf)], "%c%c%c..",
				p_seq[from], p_seq[from+1], p_seq[from+2]);
			sprintf(&buf[strlen(buf)], "%c%c%c",
				p_seq[to-2], p_seq[to-1], p_seq[to]);
		}
		sprintf(&buf[strlen(buf)], ") are unassigned.");
		ErrPostEx(SEV_ERROR, 0, 0, buf);
	}
}
#endif

/* =================================================================
 ----- ensure_clean_start () --
* ======================================
*/
static void	ensure_clean_start(void)
{
Int4 tripdex, i;

 dbaa_seq =NULL;
 dbnt_seq =NULL;
 p_seq =NULL;
 dna_seq =NULL;
 rev_seq =NULL;
 frame[0] =NULL;
 frame [1] =NULL;
 frame [2] =NULL;
	num_nts=0;
 Total_len = 0;
 intlist = NULL;

	for (tripdex = 0; tripdex < 27; tripdex ++){
		for (i=0; i<26*26; ++i) {
			(triptab[tripdex])[i] = NULL;
			if (tripdex == 26){
				break;  /* last array is short */
			}
		}
	}
}

/*****************************************************************************
*
*   Suggest_Intervals (sugrp, f, num_nuc)
*       Master routine for suggesting intervals, this version passes
*       values to older UNIX program of Miller, Gish and Sirotkin
*
*****************************************************************************/

static void Suggest_Intervals (SuggestRecPtr sugrp,
			       SuggestOutputPtr pSuggestOut, Int4 num_nuc)
{
  Boolean    goOn;
  Int4       i;
  Int4       len;
  Int4       nt;
  CharPtr    geneticCode;
  SeqRecPtr  nucleotide;
  SeqRecPtr  protein;
  Int4       tripdex;
  struct trip_node	*h, *oh;

  if (sugrp != NULL) {
    goOn = TRUE;
    nucleotide = &(sugrp->nucleotide);
    protein = &(sugrp->protein);
    geneticCode = sugrp->geneticCode;
    if (nucleotide != NULL && protein != NULL && geneticCode != NULL) {
      if ((nucleotide->length +1) / 3 < protein->length) {
        Message (MSG_ERROR, "Nucleotide is too short to encode protein");
        goOn = FALSE;
      }
      if (nucleotide->length < 18) {
        Message (MSG_ERROR, "Nucleotide length is too short");
        goOn = FALSE;
      }
      if (protein->length < 6) {
        Message (MSG_ERROR, "Protein length is too short");
        goOn = FALSE;
      }

      if (goOn) {
        m_stk = MemNew (MAX_EXONS * sizeof (Match));
		triptab = MemNew(27 * sizeof (struct trip_node **));
		for (tripdex = 0; tripdex < 26; tripdex ++){
			triptab[tripdex] = MemNew ((26*26) * sizeof (struct trip_node *));
		}
		triptab[26] = MemNew ((1) * sizeof (struct trip_node *)); /* stops and unknowns */

        ensure_clean_start();

        min_exon_from_form = sugrp->minExon;
        Look_for_stop = sugrp->lookForStop;
        Min_val = sugrp->minVal;
        Max_val = sugrp->maxVal;

        /*
        dna_len = nucleotide->length;
        */
        dna_seq = nucleotide->sequence;
        rev_seq = nucleotide->reverse;

        pro_len = protein->length;
        p_seq = protein->sequence;

        aori = 0;
        ahz = FALSE;
        aasc = TRUE;

        genetic_code = geneticCode;

        max_num_nts =  BSLen (nucleotide->segLens) / sizeof (Int4);

        nt_id_seq = (Int4 *) MemNew (max_num_nts * sizeof(Int4) );
        nt_orient = (DBTINYINT * ) MemNew ( max_num_nts * sizeof(DBTINYINT));
        dbnt_seq = (SeqPtr*) MemNew ( max_num_nts * sizeof(SeqPtr) );
        nt_dna_len = (Int4 * ) MemNew (max_num_nts * sizeof(Int4));
        nt_len_offsets = (Int4 * ) MemNew (max_num_nts * sizeof(Int4));

        dna_len = 0;
        nt_len_offsets [0] = 0;
        nt_id_seq[0] = 0;
        last_orient = SuggestPlusStrand;
        nt_orient[0] = last_orient;

        m_top = 0;

        num_nts = BSLen (nucleotide->segLens) / sizeof (Int4);
        BSSeek (nucleotide->segLens, 0L, 0);
        for (nt = 0; nt < num_nts; nt ++ ){
          BSRead (nucleotide->segLens, &len, sizeof (len));
		  nt_dna_len[nt] = i = len;
		  if ( nt > 0) {
		    nt_len_offsets[nt] = nt_len_offsets[nt-1] + nt_dna_len[nt-1];
		  }
		  dna_len += i;
          nt_id_seq[nt] = nt;
        }

        APT_suggest (pSuggestOut, num_nuc);

        m_stk = MemFree (m_stk);

		for (tripdex = 0; tripdex < 27; tripdex ++){
			for (i=0; i<26*26; ++i) {
				h = (triptab[tripdex])[i];
				while (h != NULL) {
					oh = h;
					h = h->link;
					MemFree(oh);
				}
				(triptab[tripdex])[i] = NULL;
				if (tripdex == 26 ){
					break;
				}
			}
        	triptab[tripdex] = MemFree (triptab[tripdex] );
		}
		triptab = MemFree (triptab);

        nt_id_seq = MemFree (nt_id_seq);
        nt_orient = MemFree (nt_orient);
        dbnt_seq = MemFree (dbnt_seq);
        nt_dna_len = MemFree (nt_dna_len);
        nt_len_offsets = MemFree (nt_len_offsets);

      }

    }
  }
}

/* default code (code 0) means use the standard code (code 1) */

static CharPtr geneticCodes [16] = {
  "",
  "FFLLSSSSYY**CC*WLLLLPPPPHHQQRRRRIIIMTTTTNNKKSSRRVVVVAAAADDEEGGGG",
  "FFLLSSSSYY**CCWWLLLLPPPPHHQQRRRRIIMMTTTTNNKKSS**VVVVAAAADDEEGGGG",
  "FFLLSSSSYY**CCWWTTTTPPPPHHQQRRRRIIMMTTTTNNKKSSRRVVVVAAAADDEEGGGG",
  "FFLLSSSSYY**CCWWLLLLPPPPHHQQRRRRIIIMTTTTNNKKSSRRVVVVAAAADDEEGGGG",
  "FFLLSSSSYY**CCWWLLLLPPPPHHQQRRRRIIMMTTTTNNKKSSSSVVVVAAAADDEEGGGG",
  "FFLLSSSSYYQQCC*WLLLLPPPPHHQQRRRRIIIMTTTTNNKKSSRRVVVVAAAADDEEGGGG",
  "",
  "",
  "FFLLSSSSYY**CCWWLLLLPPPPHHQQRRRRIIIMTTTTNNNKSSSSVVVVAAAADDEEGGGG",
  "FFLLSSSSYY**CCCWLLLLPPPPHHQQRRRRIIIMTTTTNNKKSSRRVVVVAAAADDEEGGGG",
  "FFLLSSSSYY**CC*WLLLLPPPPHHQQRRRRIIIMTTTTNNKKSSRRVVVVAAAADDEEGGGG",
  "FFLLSSSSYY**CC*WLLLSPPPPHHQQRRRRIIIMTTTTNNKKSSRRVVVVAAAADDEEGGGG",
  "FFLLSSSSYY**CCWWLLLLPPPPHHQQRRRRIIMMTTTTNNKKSSGGVVVVAAAADDEEGGGG",
  "FFLLSSSSYYY*CCWWLLLLPPPPHHQQRRRRIIIMTTTTNNNKSSSSVVVVAAAADDEEGGGG",
  "FFLLSSSSYY*QCC*WLLLLPPPPHHQQRRRRIIIMTTTTNNKKSSRRVVVVAAAADDEEGGGG"
};

/*****************************************************************************
*
*   Get_Genetic_Code (code, sugrp)
*       Gets a genetic code from a file
*
*****************************************************************************/

Int4 Get_Genetic_Code (Int4 code, SuggestRecPtr sugrp)

{
  GeneticCodePtr  codes;
  GeneticCodePtr  gcp;
  Int4            id;
  Char            rsult [76];
  Char            str [76];
  ValNodePtr      tmp;

  if (sugrp != NULL) {
    if (code == 7) {
      code = 4;
    } else if (code == 8) {
      code = 1;
    }
    rsult [0] = '\0';
    codes = GeneticCodeTableLoad ();
    if (codes != NULL) {
      for (gcp = codes; gcp != NULL; gcp = gcp->next) {
        id = 0;
        str [0] = '\0';
        for (tmp = (ValNodePtr) gcp->data.ptrvalue; tmp != NULL; tmp = tmp->next) {
          switch (tmp->choice) {
            case 2 :
              id = tmp->data.intvalue;
              break;
            case 3 :
              StringNCpy (str, (CharPtr) tmp->data.ptrvalue, sizeof (str) - 1);
              break;
            default :
              break;
          }
        }
        if (code == id) {
          StringCpy (rsult, str);
        }
      }
    }
    if (rsult [0] == '\0') {
      if (code > 0 && code <= 15) {
        StringCpy (rsult, geneticCodes [code]);
      } else {
        StringCpy (rsult, geneticCodes [1]);
		code = 1;
      }
    }
    sugrp->geneticCode = StringSave (rsult);
  }

  return code;
}

/*============================================================================*\
 * Function:
 *	InitSuggest
 *
 * Purpose:
 *	Does initialization of the application
 *
 * Parameters:
 *	none
 *
 * Return:
 *	TRUE if successful,
 *	FALSE otherwise
 *
\*----------------------------------------------------------------------------*/
Boolean
InitSuggest(void)
{
    if (!AllObjLoad ()) {
	Message (MSG_FATAL, "AllObjLoad failed");

	return FALSE;
    }
      
    if (!SeqCodeSetLoad ()) {
	Message (MSG_FATAL, "SeqCodeSetLoad failed");
	
	return FALSE;
    }

    if (!GeneticCodeTableLoad ()) {
        Message (MSG_FATAL, "GeneticCodeTableLoad failed");
	
	return FALSE;
    }

    tripletTable = (TripletNodePtr PNTR)MemNew(sizeof(TripletNodePtr) *
					       (26 * 26 * 26 + 1));
    if (tripletTable == NULL) {
	Message (MSG_FATAL, "Triplet table allocation failed");
	
	return FALSE;
    }

    return TRUE;
}

/*============================================================================*\
 * Function:
 *	ProcessData
 *
 * Purpose:
 *	Processes input data, i.e. finds locations for each protein within
 * given DNA chain.
 *
 * Parameters:
 *	pSuggestOut	- pointer to SuggestOutput structure; this structure
 *		contains pointer to the open output file if this application
 *		is running as a standalone application or pointer to SeqAnnot
 *		otherwise.
 *
 * Return:
 *	TRUE	- if successful,
 *	FALSE	- otherwise;
 *
\*----------------------------------------------------------------------------*/
Boolean
ProcessData(SuggestOutputPtr pSuggestOut, Boolean clearNucSeqRec)
{
    Int4	num_nuc;
    Int4	num_prt;
    Int4	len;
    Int4	i;
    
    suggestRec.protein.sequence = MemFree(suggestRec.protein.sequence);
    suggestRec.protein.length = 0;
    num_nuc = BSLen(suggestRec.nucleotide.segLens) / sizeof(Int4);
    num_prt = BSLen(suggestRec.protein.segLens) / sizeof(Int4);
    BSSeek(suggestRec.protein.segLens, 0L, 0);
    BSSeek(suggestRec.protein.rawSeq, 0L, 0);
    
    for (i = 0; i < num_prt; i++) {
	OutProteinID(pSuggestOut, i);
	BSRead(suggestRec.protein.segLens, &len, sizeof(len));
	suggestRec.protein.sequence = MemNew((len + 1) * sizeof(Uint1));
	len = BSRead(suggestRec.protein.rawSeq,
		     suggestRec.protein.sequence, len);
	suggestRec.protein.length = len;
	Suggest_Intervals(&suggestRec, pSuggestOut, num_nuc);
	suggestRec.minVal = 0L;
	suggestRec.maxVal = 0L;
	suggestRec.tooShort = 0L;
	suggestRec.protein.lookForStop = suggestRec.lookForStop;
	suggestRec.protein.sequence = MemFree(suggestRec.protein.sequence);
	suggestRec.protein.length = 0;
    }
    
    if (clearNucSeqRec) {
      ClearSeqRec(&(suggestRec.nucleotide));
    }
    ClearSeqRec(&(suggestRec.protein));
    suggestRec.geneticCode = (CharPtr)MemFree(suggestRec.geneticCode);
    genetic_code = NULL;

    return TRUE;
}


