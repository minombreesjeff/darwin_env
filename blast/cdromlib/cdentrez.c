/*   cdentrez.c                                                          
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
 * RCS $Id: cdentrez.c,v 6.2 1999/03/11 23:20:07 kans Exp $
 *
 * Author:  Ostell, Kans
 *
 * Version Creation Date:   10/15/91
 *
 * File Description: 
 *   	entrez index access library for Entrez CDROM
 *
 * Modifications:  
 * --------------------------------------------------------------------------
 * Date     Name        Description of modification
 * -------  ----------  -----------------------------------------------------
 * 07-07-94 Schuler     Added CdEntrezGetInfo function
 * 07-12-94 Schuler     Added #ifdef _NEW_CdEntrez_/_OLD_CdEntrez
 * 07-13-94 Schuler     Moved CdTermFree here from cdromlib.c
 * 09-22-94 Schuler     CdEntrezFini: set _nouveau to FALSE
 * 11-22-94 Schuler     Cleaned up some integer size problems
 *
 * 05-19-95 Schuler     Added rcs Log directive for automatic insertion of
 *                      modification comments.
 *
 * Revision $Log: cdentrez.c,v $
 * Revision Revision 6.2  1999/03/11 23:20:07  kans
 * Revision sprintf cast
 * Revision
 * Revision Revision 6.1  1998/08/24 18:42:15  kans
 * Revision fixed -v -fd warnings
 * Revision
 * Revision Revision 6.0  1997/08/25 18:12:52  madden
 * Revision Revision changed to 6.0
 * Revision
 * Revision Revision 5.5  1997/06/26 21:55:31  vakatov
 * Revision [PC] DLL'd "ncbicdr.lib", "ncbiacc.lib", "ncbinacc.lib" and "ncbicacc.lib"
 * Revision
 * Revision Revision 5.4  1997/03/07 17:16:10  epstein
 * Revision always choose the highest GI in EntrezFindSeqId
 * Revision
 * Revision 5.3  1997/01/14  21:26:07  epstein
 * plug memory leak when performing Entrez set-difference operations
 *
 * Revision 5.2  1996/11/22  18:02:19  epstein
 * change algorithm for looking up PDB accessions
 *
 * Revision 5.1  1996/08/14  19:56:41  epstein
 * add APIs for fetching pieces of biostruc annots (mostly written by Chris Hogue)
 *
 * Revision 5.0  1996/05/28  13:55:34  ostell
 * Set to revision 5.0
 *
 * Revision 4.18  1996/05/14  21:01:12  epstein
 * use SQID index to and docsum to convert back-and-forth between GIs and SeqIds, per Jim Ostell
 *
 * Revision 4.17  1996/04/01  21:02:31  epstein
 * remove dead CdEntrezBiostrucAnnotSetGet() code
 *
 * Revision 4.16  1996/04/01  20:59:38  epstein
 * Schuler/Epstein changes for cleaner CdEntrezBiostrucAnnotSetGet retrieval
 *
 * Revision 4.15  1996/03/29  18:52:12  epstein
 * add support for structure alignments (includes kludge for now)
 *
 * Revision 4.14  1995/10/23  21:39:56  epstein
 * another tweak for PC 16-bit addressing
 *
 * Revision 4.13  1995/10/23  14:17:52  epstein
 * fix 16/32-bit portability problems
 *
 * Revision 4.12  1995/10/03  14:12:40  epstein
 * repair term-checking logic to avoid erroneous caching
 *
 * Revision 4.11  1995/10/02  15:25:56  epstein
 * correct range-checking logic due to semantics of StringXCmp()
 *
 * Revision 4.10  1995/10/02  12:51:23  epstein
 * fix endpoints for range scanning
 *
 * Revision 4.9  1995/10/02  12:49:44  epstein
 * add memory-based performance enhancements
 *
 * Revision 4.8  1995/10/02  02:35:33  epstein
 * add range-checking
 *
 * Revision 4.7  1995/09/19  13:27:51  epstein
 * add truncation limit
 *
 * Revision 4.6  1995/08/30  20:54:53  epstein
 * search TYP_CH database if retcode is -1
 *
 * Revision 4.5  1995/08/28  23:20:47  kans
 * includes new mmdbapi headers
 *
 * Revision 4.4  1995/08/28  17:44:01  epstein
 * add code so that when retcode is -1, we perform less validation on the retrieve Seq-entry
 *
 * Revision 4.3  1995/08/24  20:44:10  epstein
 * add more stuff for genomes
 *
 * Revision 4.2  1995/08/18  17:41:17  epstein
 * fix (?) parsing of PDB accession per Brandon's observation
 *
 * Revision 4.1  1995/08/11  20:26:18  epstein
 * add max-models support for biostrucs
 *
 * Revision 4.0  1995/07/26  13:50:32  ostell
 * force revision to 4.0
 *
 * Revision 2.62  1995/07/19  22:07:00  kans
 * added (probably superfluous) casts to some sprintf calls
 *
 * Revision 2.61  1995/06/29  15:57:03  epstein
 * added Complexity argument when fetching structures
 *
 * Revision 2.60  95/06/27  11:54:35  kans
 * replaced _OLD_CDEntrez_ with _OLD_CdEntrez_
 * 
 * Revision 2.59  1995/06/23  16:02:43  kans
 * support for accmmdbs.c stub to resolve symbols without MMDB link
 *
 * Revision 2.58  1995/06/23  13:22:25  kans
 * Biostruc_CD_supported symbol needed for local MMDB access
 *
 * Revision 2.57  1995/05/16  14:36:20  schuler
 * Automatic comment insertion enabled
 *
 *
 * ==========================================================================
 */

#define REVISION_STR  "$Revision: 6.2 $"

static char * _this_module = "CdEntrez";
#define THIS_MODULE _this_module
static char * _this_file = __FILE__;
#define THIS_FILE _this_file

#include <accentr.h>
#include <cdentrez.h>
#include <sequtil.h>
#include <objall.h>

typedef struct posting {
  ByteStorePtr  uids;
  DocUidPtr     buffer;
  Int4          bufsize;
  Int4          index;
  Int4          count;
} Posting, PNTR PostingPtr;

#define SCAN_MAX 200

/* the 1023 below is not a typo; it's needed to avoid overflowing 16-bit
   addressing on PCs */
#define DEF_CDENTREZ_MEMUSAGE (64 * 1023L)
#define MAX_CDENTREZ_UID_LIST_SIZE (cdMemUsage)
#define CDENTREZ_TERM_MAX (cdMemUsage / 4)
#define MAX_CDENTREZ_BYTESTORE (cdMemUsage / 4)
#define MAX_CDENTREZ_SMALL_LIST (cdMemUsage / 2)

typedef struct scanData {
  Int4          specialCount;
  Int4          totalCount;
  Int4          offset;
  ByteStorePtr  specialPtr;
  ByteStorePtr  remainderPtr;
} ScanData, PNTR ScanPtr;

static Int2          db;
static Int2          fld;

static DocUidPtr     uidPtr;

static Int2          searchTermLen;
static Int4          cdMemUsage = 32768;

static ByteStorePtr  specialPost;
static ByteStorePtr  remainPost;

static Char          selection [256];
static Char          wildcard [256];
static Char          topOfRange [256];
static Boolean       rangeScanning = FALSE;

static ScanPtr       scanPtr;
static Int4          scanCount;
static Boolean       scanOk;
static CdTermProc    userScanProc;

static CdTermPtr     eset;

static ValNodePtr    cachedExpr = NULL;
static ByteStorePtr  cachedBsp = NULL;

static void NEAR NextNode PROTO((void));
static ByteStorePtr NEAR Factor PROTO((void));
static ByteStorePtr NEAR Term PROTO((void));        
static ByteStorePtr NEAR Diff PROTO((void));
static ByteStorePtr NEAR Expression PROTO((void));
static CdTermPtr NEAR FindTermNode PROTO((CharPtr term, DocType type, DocField field, CharPtr highRange));
static ValNodePtr CdTLExprFree PROTO((ValNodePtr elst));

static PostingPtr NEAR NewPost PROTO((ByteStorePtr lst, Int4 defsize));
static PostingPtr NEAR FreePost PROTO((PostingPtr pst));
static Int4 NEAR PostLength PROTO((PostingPtr pst));
static void NEAR RewindPost PROTO((PostingPtr pst));
static DocUid NEAR ReadItem PROTO((PostingPtr pst));
static void NEAR WriteItem PROTO((PostingPtr pst, DocUid value));
static void NEAR FlushItems PROTO((PostingPtr pst));
static void NEAR SavePostingList PROTO((FILE *f, ByteStorePtr bsp));

static Boolean NEAR CdEntrezMergeTerm PROTO((DocType type, DocField field, CharPtr term, Int4Ptr spcl, Int4Ptr totl, CdTermProc userProc));
static void NEAR SingleSpaces PROTO((CharPtr str));
static void NEAR TermTruncate PROTO((CharPtr str));
static void NEAR QuickSortSmall PROTO((DocUidPtr uids, Int4 l, Int4 r));
static Int4 NEAR CompressSmall PROTO((DocUidPtr uids, Int4 count));
static Int4 NEAR UniqueSmall PROTO((DocUidPtr uids, Int4 count));
static ByteStorePtr NEAR MergeSmallLists PROTO((ByteStorePtr bsp, ByteStorePtr small));
static Boolean NEAR MergeSeveralLists PROTO((Int4 i, Int4 count));
static Boolean NEAR MergeSeveralOrderedLists PROTO((Int4 i, Int4 count));
static Boolean NEAR MergeUnorderedLists PROTO((Int4 i, Int4 count));
static Boolean NEAR ProcessScanResults PROTO((void));
static Boolean  WildCardProc PROTO((CdTermPtr trmp));
static Boolean  ScanOnlyProc PROTO((CdTermPtr trmp));
static Boolean  ScanAndFreeProc PROTO((CdTermPtr trmp));

/**** Moved from cdentrez.h ********************/

static CdTermPtr NEAR CdEntrezCreateTerm PROTO((CharPtr term, DocType type, DocField field, ByteStorePtr special, ByteStorePtr remainder, CharPtr highRange));
static ByteStorePtr NEAR LoadPostingList PROTO((FILE *f, Int4 special, Int4 total));
static ByteStorePtr NEAR FreePostingList PROTO((ByteStorePtr lst));
static ByteStorePtr NEAR MergePostingLists PROTO((ByteStorePtr lst1, ByteStorePtr lst2));
static ByteStorePtr NEAR IntersectPostingLists PROTO((ByteStorePtr lst1, ByteStorePtr lst2));
static ByteStorePtr NEAR DifferencePostingLists PROTO((ByteStorePtr lst1, ByteStorePtr lst2));

static ValNodePtr currNode;
static Uint1 currChoice;

/************************* moved from old cdml.c ****************************/
static AsnTypePtr  MEDLINE_ENTRY = NULL;
static AsnTypePtr  MEDLINE_ENTRY_cit = NULL;
static AsnTypePtr  MEDLINE_ENTRY_abstract = NULL;
static AsnTypePtr  TITLE_E_trans = NULL;
static AsnTypePtr  AUTH_LIST_names_ml_E = NULL;
static AsnTypePtr  AUTH_LIST_names_str_E = NULL;
static AsnTypePtr  DATE_STD_year = NULL;
static AsnTypePtr  DATE_str = NULL;
static AsnTypePtr  TITLE_E_name = NULL;
static AsnTypePtr  MEDLINE_ENTRY_mesh = NULL;
static AsnTypePtr  MEDLINE_ENTRY_substance = NULL;
static AsnTypePtr  MEDLINE_ENTRY_xref = NULL;
static AsnTypePtr  MEDLINE_ENTRY_idnum = NULL;
static AsnTypePtr  MEDLINE_ENTRY_gene = NULL;

static DocSumPtr NEAR MedSumAsnRead PROTO((AsnIoPtr aip, DocUid uid));
static void NEAR StripAuthor PROTO((CharPtr author));
static void NEAR FindAsnType PROTO((AsnTypePtr PNTR atp, AsnModulePtr amp, CharPtr str));

static DocSumPtr NEAR CdEntMlSumGet PROTO((Int4 uid));

/************************* moved from old cdseq.c ****************************/
static AsnTypePtr  SEQ_ENTRY = NULL;
static AsnTypePtr  SEQ_ENTRY_seq = NULL;
static AsnTypePtr  SEQ_ENTRY_set = NULL;
static AsnTypePtr  TEXTSEQ_ID_name = NULL;
static AsnTypePtr  TEXTSEQ_ID_accession = NULL;
static AsnTypePtr  SEQ_DESCR_E_title = NULL;
static AsnTypePtr  GIIMPORT_ID_id = NULL;
static AsnTypePtr  BIOSEQ_inst = NULL;
static AsnTypePtr  SEQ_INST_mol = NULL;
static AsnTypePtr  SEQ_INST_repr = NULL;
static AsnTypePtr  SEQ_ID_gibbsq = NULL;
static AsnTypePtr  SEQ_ID_gibbmt = NULL;
static AsnTypePtr  SEQ_ID_genbank = NULL;
static AsnTypePtr  SEQ_ID_gi = NULL;
static AsnTypePtr  SEQ_ID_embl = NULL;
static AsnTypePtr  SEQ_ID_ddbj = NULL;
static AsnTypePtr  SEQ_ID_pir = NULL;
static AsnTypePtr  SEQ_ID_swissprot = NULL;
static AsnTypePtr  PDB_BLOCK_compound_E = NULL;
static AsnTypePtr  PDB_SEQ_ID_MOL = NULL;
static AsnTypePtr  BIOSEQ_id = NULL;
static AsnTypePtr  BIOSEQ_id_E = NULL;
static AsnTypePtr  CIT_PAT_title = NULL;

static DocSumPtr NEAR CdEntSeqSumGet PROTO((Int4 uid, DocType type));

extern int _nouveau;

/*****************************************************************************
*
*   CdEntrezInit ()
*       Creates linked list of CdTerm nodes, creates temporary file for
*       postings lists, saves file name in first node.  When creating new
*       nodes, posting file is appended to temporary file, node offset then
*       points to temporary file location of posting information.
*
*****************************************************************************/

NLM_EXTERN Boolean  CdEntrezInit (Boolean no_warnings)

{
  FILE      *fp;
  Char      str [PATH_MAX];
  Boolean inited = FALSE;
  CharPtr   prop;
  
#ifdef Biostruc_supported
	objmmdb1AsnLoad ();
	objmmdb2AsnLoad ();
	objmmdb3AsnLoad ();
#endif
#ifdef _NEW_CdEntrez_
	_nouveau = GetAppParamBoolean("ncbi","CdEntrez","NewStyle",TRUE);
	if (_nouveau)
	{	
		if (cd3_CdInit())
			inited = TRUE;
		else
			ErrLogPrintf("cd3_CdInit() failed\n");
	}
#endif
	/* In the dual OLD/NEW case, go on to try CdInit if cd3_CdInit failed */
#ifdef _OLD_CdEntrez_
	if (!inited)
	{
		if (CdInit())
		{
			inited = TRUE;
			_nouveau = FALSE;
		}
	}
#endif
	if (!inited)
		return FALSE;
		
	eset = MemNew (sizeof (CdTerm));
	if (eset == NULL)
		return FALSE;
	eset->type = 255;   /* set to not used */
    TmpNam (str);
    eset->term = StringSave (str);
#ifdef WIN_MAC
    FileCreate (str, "????", "NCBI");
#endif
    fp = FileOpen (str, "wb");
	if (fp == NULL) {
		ErrPostEx (SEV_ERROR, ERR_CD_FILEOPEN, 0, "Unable to open temporary file %s", str);
		return FALSE;
	}
    FileClose (fp);
  if ((prop = (CharPtr) GetAppProperty("CdEntrezMemUsage")) != NULL)
  {
      long tmplong;

      sscanf(prop, "%ld", &tmplong);

      cdMemUsage = tmplong;
  } else {
      cdMemUsage = DEF_CDENTREZ_MEMUSAGE;
  }
      cdMemUsage = MIN(cdMemUsage, MAXALLOC);
	return TRUE;
}

/*****************************************************************************
*
*   CdEntrezFini ()
*       Frees linked list of CdTerm nodes and removes temporary posting file.
*
*****************************************************************************/

NLM_EXTERN void  CdEntrezFini (void)

{
  CdTermPtr nxt;
  Char      temp [PATH_MAX];

  if (eset != NULL) {
    if (eset->term != NULL) {
      StringCpy (temp, eset->term);
      FileRemove (temp);
    }
    while (eset != NULL) {
      nxt = eset->next;
      CdTermFree (eset);
      eset = nxt;
    }
  }
  eset = NULL;

  cachedExpr = CdTLExprFree(cachedExpr);
  cachedBsp = BSFree(cachedBsp);
  
#ifdef _NEW_CdEntrez_
	if (_nouveau)
		cd3_CdFini();
#endif
#ifdef _OLD_CdEntrez_
	if (!_nouveau)
		CdFini();
#endif
	_nouveau = FALSE;
}


/*****************************************************************************
*
*   CdEntrezGetInfo ()
*
*****************************************************************************/

NLM_EXTERN EntrezInfo* CdEntrezGetInfo (void)
{
	EntrezInfo *info = NULL;
	
#ifdef _NEW_CdEntrez_
	if (_nouveau)
		info = cd3_CdGetInfo();
#endif

#ifdef _OLD_CdEntrez_
	if (!_nouveau)
		info = CdGetInfo();
#endif
	
	return info;
}

/*****************************************************************************
*
*   CdEntrezDetailedInfo ()
*
*****************************************************************************/

NLM_EXTERN char* CdEntrezDetailedInfo (void)
{
	char *info = NULL;
	
#ifdef _NEW_CdEntrez_
	if (_nouveau)
		info = cd3_CdDetailedInfo();
#endif

#ifdef _OLD_CdEntrez_
	if (!_nouveau)
		info = CdDetailedInfo();
#endif
	
	return info;
}


/*****************************************************************************
*
*   CdEntGetMaxLinks()
*   	returns max links in link set allowed by system
*
*****************************************************************************/
NLM_EXTERN Int4 CdEntGetMaxLinks (void)

{
	return (Int4)(INT_MAX / sizeof(DocUid));
}

/*****************************************************************************
*
*   CdEntrezCreateNamedUidList(term, type, field, num, uids)
*       Creates a term node in the entrez set structure if one does not
*       yet exist, and loads the posting file from the uid parameter.
*
*****************************************************************************/
NLM_EXTERN void  CdEntrezCreateNamedUidList (CharPtr term, DocType type, DocField field, Int4 num, DocUidPtr uids)

{
  Int4          count;
  ByteStorePtr  post;
  Char          str [256];

  if (term != NULL && uids != NULL && num > 0 && num <= 16383) {
    StringNCpy (str, term, sizeof (str) - 1);
    post = BSNew (0);
    if (post != NULL) {
      count = (Int4) num;
      QuickSortSmall (uids, 0, (Int4) (count - 1));
      count = CompressSmall (uids, count);
      count = UniqueSmall (uids, count);
      BSWrite (post, uids, (Int4) (count * sizeof (DocUid)));
      CdEntrezCreateTerm (str, type, field, NULL, post, NULL);
      BSFree (post);
    }
  }
}

/*****************************************************************************
*
*   CdEntrezCreateNamedUidListX(term, type, field, post)
*       Creates a term node in the entrez set structure if one does not
*       yet exist, and loads the posting file from the uid parameter.
*
*****************************************************************************/
NLM_EXTERN void  CdEntrezCreateNamedUidListX (CharPtr term, DocType type, DocField field, ByteStorePtr bsp)

{
  Int4          actual;
  Int4          count;
  ByteStorePtr  post;
  ByteStorePtr  small;
  Char          str [256];
  DocUidPtr     uids;

  if (term != NULL && bsp != NULL) {
    StringNCpy (str, term, sizeof (str) - 1);
    post = BSNew (0);
    if (post != NULL) {
      uids = MemNew (4096 * sizeof (DocUid));
      BSSeek (bsp, 0L, 0);
      actual = BSRead (bsp, uids, (Int4) (4096 * sizeof (DocUid)));
      while (actual > 0) {
        count = (Int4) actual;
        QuickSortSmall (uids, 0, (Int4) (count - 1));
        count = CompressSmall (uids, count);
        count = UniqueSmall (uids, count);
        if (count > 0) {
          small = BSNew (0L);
          if (small != NULL) {
            BSWrite (small, uids, count * sizeof (DocUid));
            post = MergePostingLists (post, small);
          }
        }
        actual = BSRead (bsp, uids, (Int4) (4096 * sizeof (DocUid)));
      }
      CdEntrezCreateTerm (str, type, field, NULL, post, NULL);
      MemFree (uids);
      BSFree (post);
    }
  }
}

/*****************************************************************************
*
*   CdEntTLNew (type)
*       Creates linked list of asn nodes for constructing boolean query on
*       terms.  First node points to the EntrezSetNew-created structure that
*       maps terms to posting lists.  Remaining nodes contain symbols for AND,
*       OR, LEFT PARENTHESIS, RIGHT PARENTHESIS, or a SPECIAL or TOTAL term
*       specification.  The term specification nodes point to a CdTerm node
*       within the entrez set structure.
*
*****************************************************************************/

NLM_EXTERN ValNodePtr  CdEntTLNew (DocType type)

{
  ValNodePtr anp;

  anp = NULL;
  if (eset != NULL) {
    anp = ValNodeNew (NULL);
    if (anp != NULL) {
      anp->choice = NULLSYM;
      anp->data.ptrvalue = (Pointer) eset;
	  eset->type = type;
    }
  }
  return anp;
}

/*****************************************************************************
*
*   CdEntTLAddTerm (elst, term, type, field, special, highRange)
*       Adds a term node to a boolean algebraic term query.
*
*****************************************************************************/

NLM_EXTERN ValNodePtr  CdEntTLAddTerm (ValNodePtr elst, CharPtr term, DocType type, DocField field, Boolean special, CharPtr highRange)

{
  ValNodePtr anp;
  CdTermPtr  trmp;

  anp = NULL;
  if (eset != NULL && elst != NULL) {
	if (type != eset->type)   /* mixed databases */
		return NULL;
    anp = ValNodeNew (elst);
    if (anp != NULL) {
      if (special) {
        anp->choice = SPECIALTERM;
      } else {
        anp->choice = TOTALTERM;
      }
      trmp = FindTermNode (term, type, field, highRange);
      anp->data.ptrvalue = (Pointer) trmp;
    }
  }
  return anp;
}

/*****************************************************************************
*
*   CdEntTLFree (elst)
*       Frees a boolean algebraic term query list.
*
*****************************************************************************/

NLM_EXTERN ValNodePtr  CdEntTLFree (ValNodePtr elst)

{
  if (elst != NULL) {
    ValNodeFree (elst);
	eset->type = 255;   /* set to nothing */
  }
  return NULL;
}

/*****************************************************************************
*
*   CdTLExprFree(elst)
*
*   Free the CdEntrez-style expression, including all of its subordinate terms
****************************************************************************/
static ValNodePtr CdTLExprFree(ValNodePtr elst)
{
  ValNodePtr np;
  CdTermPtr tp;

  for (np = elst; np != NULL; np = np->next) {
    switch (np->choice) {
    case SPECIALTERM:
    case TOTALTERM:
      if ((tp = np->data.ptrvalue) != NULL) {
        MemFree (tp->term);
        MemFree (tp->highRange);
        MemFree (tp);
      }
      break;
    default:
      break;
    }
  }

  ValNodeFree(elst);

  return NULL;
}


/*****************************************************************************
*
*   CdDupExpr(elst)
*
*   Duplicate the input CdEntrez-style expression
****************************************************************************/
static ValNodePtr CdDupExpr(ValNodePtr elst)
{
  ValNodePtr dup = NULL;
  ValNodePtr trailing = NULL;
  ValNodePtr np;
  CdTermPtr tp1, tp2;

  for (; elst != NULL; elst = elst->next) {
    np = ValNodeNew(NULL);
    if (dup == NULL)
      dup = np;
    if (trailing != NULL)
      trailing->next = np;
    trailing = np;
    np->choice = elst->choice;
    switch (elst->choice) {
    case SPECIALTERM:
    case TOTALTERM:
      tp2 = elst->data.ptrvalue;
      if (tp2 != NULL)
      {
        tp1 = MemNew(sizeof(*tp1));
        np->data.ptrvalue = tp1;
        tp1->type = tp2->type;
        tp1->field = tp2->field;
        tp1->term = StringSave(tp2->term);
        tp1->highRange = StringSave(tp2->highRange);
      }
      break;
    default:
      break;
    }
  }

  return dup;
}

static Boolean
EqualTerms (CharPtr x, CharPtr y)
{
  if (x == NULL && y == NULL)
    return TRUE;
  if (x == NULL || y == NULL)
    return FALSE;
  return (StringICmp(x,y) == 0);
}
  

/*****************************************************************************
*
*   CdEntTLExprEqual (elst1, elst2)
*
*   Determine whether two CdEntrez-style boolean expressions are equal
****************************************************************************/

static Boolean
CdTLExprEqual (ValNodePtr elst1, ValNodePtr elst2)
{
  Boolean equal = TRUE;
  CdTermPtr c1, c2;

  for (; elst1 != NULL && elst2 != NULL && equal; elst1 = elst1->next,
       elst2 = elst2->next) {
    if (elst1->choice == elst2->choice) {
      switch (elst1->choice) {
        case SPECIALTERM:
        case TOTALTERM:
          c1 = elst1->data.ptrvalue;
          c2 = elst2->data.ptrvalue;
          equal = c1 != NULL && c2 != NULL && c1->type == c2->type &&
                  c1->field == c2->field && EqualTerms(c1->term, c2->term) &&
                  EqualTerms(c1->highRange, c2->highRange);
          break;
        default:
          break;
      }
    } else {
      equal = FALSE;
    }
  }

  return elst1 == NULL && elst2 == NULL && equal;
}


/*****************************************************************************
*
*   CdEntTLEvalCount (elst)
*       Evaluates a boolean algebraic term query list, returning the
*       count of resulting UIDs.
*
*****************************************************************************/

NLM_EXTERN Int4 CdEntTLEvalCount (ValNodePtr elst)
{
  ByteStorePtr bsp;
  Int4         len;

  len = 0;
  bsp = CdEntTLEvalX(elst);
  if (bsp != NULL) {
    len = BSLen(bsp) / sizeof(DocUid);
    BSFree (bsp);
  }
  return len;
}


/*****************************************************************************
*
*   CdEntTLEvalX (elst)
*       Evaluates a boolean algebraic term query list, returning a pointer to
*       a ByteStore containing the resultant unique identifiers.  The number
*       of UIDs is calculated as BSLen (bsp) / sizeof (DocUid).
*
*****************************************************************************/

NLM_EXTERN ByteStorePtr  CdEntTLEvalX (ValNodePtr elst)

{
  ByteStorePtr bsp;

  bsp = NULL;
  if (eset != NULL && elst != NULL) {
    if (cachedExpr != NULL && CdTLExprEqual(elst, cachedExpr)) {
      BSSeek(cachedBsp, 0L, SEEK_SET);
      bsp = BSDup (cachedBsp);
    } else {
      cachedExpr = CdTLExprFree(cachedExpr);
      cachedExpr = CdDupExpr(elst);
      cachedBsp = BSFree(cachedBsp);

      currNode = elst;
      currChoice = NULLSYM;
      NextNode ();
      if (eset->term != NULL && currNode != NULL) {
        bsp = Expression ();
        BSSeek(bsp, 0L, SEEK_SET);
        cachedBsp = BSDup(bsp);
      }

    }
  }
  return bsp;
}

/*****************************************************************************
*
*   CdEntTLEval (elst)
*       Evaluates a boolean algebraic term query list, returning a pointer to
*       a LinkSet containing the resultant unique identifiers.
*
*****************************************************************************/

NLM_EXTERN LinkSetPtr  CdEntTLEval (ValNodePtr elst)

{
  ByteStorePtr bsp;
  LinkSetPtr lsp = NULL;
  Int4 numlinks;

  bsp = CdEntTLEvalX (elst);
  if (bsp != NULL)
	{
		numlinks = BSLen(bsp) / sizeof(DocUid);
		lsp = LinkSetNew();
		lsp->num = numlinks;
		if (numlinks <= CdEntGetMaxLinks())
		{
			lsp->uids = MemNew((size_t)(numlinks * sizeof(DocUid)));
			BSSeek (bsp, 0L, 0);
			BSRead(bsp, lsp->uids, (numlinks * sizeof(DocUid)));
		}
		BSFree(bsp);
	}
  return lsp;
}

/*****************************************************************************
*
*   DocSumPtr CdDocSum(type, uid)
*
*****************************************************************************/
NLM_EXTERN DocSumPtr  CdDocSum (DocType type, DocUid uid)

{
	DocSum *sum = NULL;
	
#ifdef _NEW_CdEntrez_
	if (_nouveau)
	{
		sum = CdGetDocSum(type,uid);
	}
#endif

#ifdef _OLD_CdEntrez_
	if (!_nouveau)
	{
		if (type == TYP_ML)
			sum = CdEntMlSumGet(uid);
		else
			sum = CdEntSeqSumGet(uid, type);
	}
#endif

	return sum;
}


#ifdef _NEW_CdEntrez_
NLM_EXTERN int  CdDocSumListGet PROTO((DocSum **result, int numuid, DocType type, const DocUid *uids))
{
	int i, n;
	const DocUid *p = uids;
	DocSum **s = result;
		
	ASSERT(result != NULL);
	ASSERT(uids != NULL);
	
	for (i=n=0; i<numuid; ++i)
	{
		if ((*s = CdGetDocSum(type,*p++)) != NULL)
		{
			s++;
			n++;
		}
	}
	return n;	
}
#endif



/*****************************************************************************
*
*   CdLinkUidList(type, link_to_type, numuid, uids)
*   	returns count of input uids processed
*       returns -1 on error
*       if neighbors (type == link_to_type)
*   		sums weights for same uids
*   	if (more than EntrezUserMaxLinks() uids, frees uids and weights,
*           but leaves num set)
*
*****************************************************************************/
NLM_EXTERN Int2  CdLinkUidList (LinkSetPtr PNTR result, DocType type, DocType link_to_type, Int2 numuid, Int4Ptr uids, Boolean mark_missing)      
{
	Int4 max_links = CdEntGetMaxLinks();
	Int4 count;
	
#ifdef _NEW_CdEntrez_
	if (_nouveau)
		count = cd3_CdLinkUidGet(result,type,link_to_type,numuid,uids,mark_missing,max_links);
#endif

#ifdef _OLD_CdEntrez_
	if (!_nouveau)
		count = CdLinkUidGet(result,type,link_to_type,numuid,uids,mark_missing,max_links);
#endif

	return count;
}

/*****************************************************************************
*
*   CdUidLinks()
*   	retrieves links to other uids
*
*****************************************************************************/
NLM_EXTERN LinkSetPtr  CdUidLinks (DocType type, DocUid uid, DocType link_to_type)
{
	LinkSetPtr lsp = NULL;
	DocUid u = uid;

#ifdef _NEW_CdEntrez_
	if (_nouveau)
	  cd3_CdLinkUidGet(&lsp,type,link_to_type,1,&u,FALSE,CdEntGetMaxLinks());
#endif
#ifdef _OLD_CdEntrez_
	if (!_nouveau)
	  CdLinkUidGet(&lsp,type,link_to_type,1,&u,FALSE,CdEntGetMaxLinks());
#endif

	return lsp;
}

static Boolean  TermListPageScanProc PROTO((CdTermPtr trmptr));
static Boolean  TermListTermScanProc PROTO((CdTermPtr trmptr));
static TermListProc trmproc;
static Int4 trmcount;
static Int4 trmmax;
static Boolean trmfound;
static Char trmfirst [80];
static Int4 the_first_page;

/*****************************************************************************
*
*   CdTermListByPage (type, field, page, numpage, proc)
*   	Gets terms starting at page, for numpage, by calling proc
*   	returns number of complete pages read
*
*****************************************************************************/
NLM_EXTERN Int2  CdTermListByPage (DocType type, DocField field, Int2 page, Int2 numpage, TermListProc proc)

{
	trmproc = proc;
	if (trmproc != NULL) {
#ifdef _NEW_CdEntrez_
	if (_nouveau)
		return cd3_CdTermScan(type, field, page, numpage, TermListPageScanProc);
#endif
#ifdef _OLD_CdEntrez_
	if (!_nouveau)
		return CdTermScan(type, field, page, numpage, TermListPageScanProc);
#endif
	} else {
		return 0;
	}

	return 0;
}

/*****************************************************************************
*
*   CdTermListByTerm (type, field, term, numterms, proc, first_page)
*   	Gets Terms starting with at term
*   	returns number of complete pages read
*   	sets first_page to first page read
*
*****************************************************************************/
NLM_EXTERN Int2  CdTermListByTerm (DocType type, DocField field, CharPtr term, Int2 numterms, TermListProc proc, Int2Ptr first_page)

{
	Int4  first;
	Int4  rsult;

	rsult = 0;
#ifdef _NEW_CdEntrez_
	if (_nouveau)
		first = cd3_CdTrmLookup(type, field, term);
#endif
#ifdef _OLD_CdEntrez_
	if (!_nouveau)
		first = CdTrmLookup(type, field, term);
#endif
	the_first_page = first;
	trmproc = proc;
	trmcount = 0;
	if (numterms > 0) {
		trmmax = numterms;
	} else {
		trmmax = INT2_MAX;
	}
	trmfound = FALSE;
	StringNCpy (trmfirst, term, sizeof (trmfirst) - 1);
	if (trmproc != NULL) {
#ifdef _NEW_CdEntrez_
		if (_nouveau)
			rsult = cd3_CdTermScan(type,field,first,0,TermListTermScanProc);
#endif
#ifdef _OLD_CdEntrez_
		if (!_nouveau)
			rsult = CdTermScan(type,field,first,0,TermListTermScanProc);
#endif
	}
	if (first_page != NULL) {
	  *first_page = the_first_page;
	}
	return rsult;
}

/*****************************************************************************
*
*   TermListPageScanProc(trmptr)
*   	Callback for CdTermListByPage
*
*****************************************************************************/
static Boolean  TermListPageScanProc(CdTermPtr trmptr)
{
  Boolean ret = trmproc(trmptr->term,
                        trmptr->special_count, trmptr->total_count);
  MemFree(trmptr);
  return ret;
}

/*****************************************************************************
*
*   TermListTermScanProc(trmptr)
*   	Callback for CdTermListByTerm
*
*****************************************************************************/
static Boolean  TermListTermScanProc(CdTermPtr trmptr)
{
  Boolean ret = TRUE;
  if (! trmfound) {
    if (MeshStringICmp (trmptr->term, trmfirst) >= 0) {
      trmfound = TRUE;
      the_first_page = trmptr->page;
    }
  }
  if (trmfound) {
    ret = trmproc(trmptr->term, trmptr->special_count, trmptr->total_count);
    trmcount++;
  } else {
    MemFree (trmptr->term);
  }
  MemFree(trmptr);
  return (ret && trmcount < trmmax);
}

/*****************************************************************************
*
*   CdEntrezFindTerm(type, field, term, spec, total)
*   	returns count of special and total for a term
*   	if term ends with  "...", does a truncated merge of the term
*   	if term contains '*' or '?', does a wild card merge
*
*****************************************************************************/
NLM_EXTERN Boolean  CdEntrezFindTerm (DocType type, DocField field, CharPtr term, Int4Ptr spcl, Int4Ptr totl)

{
	CharPtr tmp;
	CdTermPtr ctp;

	tmp = term;
	while (*tmp != '\0')
		tmp++;
	tmp -= 3;
	if ((*tmp == '.') && (*(tmp+1) == '.') && (*(tmp+2) == '.')) {
		return CdEntrezMergeTerm (type, field, term, spcl, totl, NULL);
	} else if (StringChr (term, '*') != NULL || StringChr (term, '?') != NULL) {
		return CdEntrezMergeTerm (type, field, term, spcl, totl, WildCardProc);
	} else {
#ifdef _NEW_CdEntrez_
		if (_nouveau)
			ctp = cd3_CdTrmFind(type,field,term);
#endif
#ifdef _OLD_CdEntrez_
		if (!_nouveau)
			ctp = CdTrmFind(type,field,term);
#endif
		if (ctp == NULL)
			return FALSE;
		*spcl = ctp->special_count;
		*totl = ctp->total_count;
		CdTermFree(ctp);
		return TRUE;
	}
}


/*****************************************************************************
*
*   CdTermFree(trmp)
*      frees a CdTerm structure
*
*****************************************************************************/

NLM_EXTERN CdTermPtr  CdTermFree (CdTermPtr trmp)

{
	if (trmp == NULL)
		return NULL;
	if (trmp->term != NULL)
		MemFree (trmp->term);
	if (trmp->highRange != NULL)
		MemFree (trmp->highRange);
	return (CdTermPtr) MemFree(trmp);
}




/*****************************************************************************
*
*   Below are static functions local to this module
*   ===============================================
*
*****************************************************************************/

/*****************************************************************************
*
*   Functions to manipulate Boolean lists
*
*****************************************************************************/

/*****************************************************************************
*
*   NextNode ()
*       Advances to the next node in a term query list.
*
*****************************************************************************/

static void NEAR NextNode (void)

{
  if (currNode != NULL) {
    currNode = currNode->next;
    if (currNode != NULL) {
      currChoice = currNode->choice;
    } else {
      currChoice = NULLSYM;
    }
  } else {
    currChoice = NULLSYM;
  }
}

/*****************************************************************************
*
*   Factor ()
*       Processes individual term nodes or parenthetical expressions in a
*       term query list.
*
*****************************************************************************/

static ByteStorePtr NEAR Factor (void)

{
  ByteStorePtr bsp;
  FILE         *fp;
  CdTermPtr    trmp;

  bsp = NULL;
  if (currChoice == LPAREN) {
    NextNode ();
    bsp = Expression ();
    if (currChoice != RPAREN) {
      ErrPostEx (SEV_ERROR, ERR_CD_BOOL, 0, "Expected right parenthesis");
    } else {
      NextNode ();
    }
  } else if (currChoice == SPECIALTERM || currChoice == TOTALTERM) {
    if (currNode != NULL) {
      trmp = currNode->data.ptrvalue;
      if (trmp != NULL) {
        fp = FileOpen (eset->term, "rb");
        if (fp != NULL) {
          fseek (fp, trmp->offset, SEEK_SET);
          if (currChoice == SPECIALTERM) {
            bsp = LoadPostingList (fp, trmp->special_count, trmp->special_count);
          } else if (currChoice == TOTALTERM) {
            bsp = LoadPostingList (fp, trmp->special_count, trmp->total_count);
          }
          FileClose (fp);
        }
      }
    }
    NextNode ();
  } else {
    NextNode ();
  }
  return bsp;
}

/*****************************************************************************
*
*   Term ()
*       Processes strings of ANDed term nodes in a term query list.
*
*****************************************************************************/

static ByteStorePtr NEAR Term (void)

{
  ByteStorePtr bsp;
  ByteStorePtr fct;

  bsp = Factor ();
  while (currChoice == ANDSYMBL) {
    NextNode ();
    fct = Factor ();
    bsp = IntersectPostingLists (bsp, fct);
  }
  return bsp;
}

/*****************************************************************************
*
*   Diff ()
*       Processes strings of ORed term nodes in a term query list.
*
*****************************************************************************/

static ByteStorePtr NEAR Diff (void)

{
  ByteStorePtr bsp;
  ByteStorePtr trm;

  bsp = Term ();
  while (currChoice == ORSYMBL) {
    NextNode ();
    trm = Term ();
    bsp = MergePostingLists (bsp, trm);
  }
  return bsp;
}


/*****************************************************************************
*
*   Expression ()
*       Processes strings of BUTNOTed term nodes in a term query list.
*
*****************************************************************************/

static ByteStorePtr NEAR Expression (void)

{
  ByteStorePtr bsp;
  ByteStorePtr trm;

  bsp = Diff ();
  while (currChoice == BUTNOTSYMBL) {
    NextNode ();
    trm = Diff ();
    bsp = DifferencePostingLists (bsp, trm);
  }
  return bsp;
}


/*****************************************************************************
*
*   Low level functions to manipulate postings lists.
*
*****************************************************************************/

static PostingPtr NEAR NewPost (ByteStorePtr lst, Int4 defsize)

{
  PostingPtr  pst;

  pst = NULL;
  if (lst != NULL) {
    pst = MemNew (sizeof (Posting));
    if (pst != NULL) {
      pst->uids = lst;
      pst->buffer = NULL;
      if (defsize == 0) {
        pst->bufsize = (Int4) MIN (16384L, BSLen (lst));
      } else {
        pst->bufsize = (Int4) MIN (16384L, defsize);
      }
      pst->count = 0;
      pst->index = 0;
    }
  }
  return pst;
}

static PostingPtr NEAR FreePost (PostingPtr pst)

{
  if (pst != NULL) {
    if (pst->uids != NULL) {
      BSFree (pst->uids);
    }
    if (pst->buffer != NULL) {
      MemFree (pst->buffer);
    }
    MemFree (pst);
  }
  return NULL;
}

static Int4 NEAR PostLength (PostingPtr pst)

{
  Int4  k;

  k = 0;
  if (pst != NULL) {
    k = (Int4) (BSLen (pst->uids) / (Int4) sizeof (DocUid));
  }
  return k;
}

static void NEAR RewindPost (PostingPtr pst)

{
  if (pst != NULL) {
    if (pst->uids != NULL) {
      BSSeek (pst->uids, 0L, 0);
    }
    pst->count = 0;
    pst->index = 0;
  }
}

static DocUid NEAR ReadItem (PostingPtr pst)

{
  DocUid  rsult;

  rsult = INT4_MAX;
  if (pst != NULL && pst->uids != NULL) {
    if (pst->buffer == NULL) {
      pst->buffer = MemNew ((size_t) pst->bufsize);
      pst->count = 0;
      pst->index = 0;
    }
    if (pst->count <= 0) {
      pst->count = (Int4) BSRead (pst->uids, pst->buffer, pst->bufsize);
      pst->index = 0;
    }
    if (pst->count > 0) {
      rsult = pst->buffer [pst->index];
      (pst->index)++;
      (pst->count) -= sizeof (DocUid);
    }
  }
  return rsult;
}

static void NEAR WriteItem (PostingPtr pst, DocUid value)

{
  if (pst != NULL && pst->uids != NULL) {
    if (pst->buffer == NULL) {
      pst->buffer = MemNew ((size_t) pst->bufsize);
      pst->count = 0;
      pst->index = 0;
    }
    pst->buffer [pst->index] = value;
    (pst->index)++;
    (pst->count) += sizeof (DocUid);
    if (pst->count >= pst->bufsize) {
      BSWrite (pst->uids, pst->buffer, pst->count);
      pst->count = 0;
      pst->index = 0;
    }
  }
}

static void NEAR FlushItems (PostingPtr pst)

{
  if (pst != NULL && pst->uids != NULL && pst->buffer != NULL) {
    BSWrite (pst->uids, pst->buffer, pst->count);
    if (pst->buffer != NULL) {
      pst->buffer = MemFree (pst->buffer);
    }
    pst->count = 0;
    pst->index = 0;
  }
}

static ByteStorePtr NEAR MergePostingLists (ByteStorePtr lst1, ByteStorePtr lst2)

{
  PostingPtr    buf1;
  PostingPtr    buf2;
  PostingPtr    buf3;
  Int4          k;
  Int4          k1;
  Int4          k2;
  DocUid        pstar;
  DocUid        qstar;
  ByteStorePtr  rsult;

  ProgMon ("MergePostingLists");
  rsult = NULL;
  if (lst1 != NULL && lst2 != NULL) {
    buf1 = NewPost (lst1, 0);
    buf2 = NewPost (lst2, 0);
    k1 = PostLength (buf1);
    k2 = PostLength (buf2);
    k = k1 + k2;
    rsult = BSNew (k * sizeof (DocUid));
    buf3 = NewPost (rsult, k * (Int4) sizeof (DocUid));
    if (rsult != NULL && buf1 != NULL && buf2 != NULL && buf3 != NULL) {
      RewindPost (buf1);
      RewindPost (buf2);
      pstar = ReadItem (buf1);
      qstar = ReadItem (buf2);
      while (k > 0) {
        if (pstar < qstar) {
          WriteItem (buf3, pstar);
          k--;
          pstar = ReadItem (buf1);
        } else if (qstar < pstar) {
          WriteItem (buf3, qstar);
          k--;
          qstar = ReadItem (buf2);
        } else {
          WriteItem (buf3, pstar);
          k -= 2;
          pstar = ReadItem (buf1);
          qstar = ReadItem (buf2);
        }
      }
      FlushItems (buf3);
    } else {
      ErrPostEx (SEV_ERROR, ERR_CD_BOOL, 0, "List is too large to merge");
    }
    if (buf1 != NULL) {
      FreePost (buf1);
    }
    if (buf2 != NULL) {
      FreePost (buf2);
    }
    if (buf3 != NULL) {
      buf3->uids = NULL;
      FreePost (buf3);
    }
  } else if (lst1 != NULL) {
    rsult = lst1;
  } else if (lst2 != NULL) {
    rsult = lst2;
  }
  return rsult;
}

static ByteStorePtr NEAR IntersectPostingLists (ByteStorePtr lst1, ByteStorePtr lst2)

{
  PostingPtr    buf1;
  PostingPtr    buf2;
  PostingPtr    buf3;
  Int4          k;
  Int4          k1;
  Int4          k2;
  DocUid        pstar;
  DocUid        qstar;
  ByteStorePtr  rsult;

  ProgMon ("UnionPostingLists");
  rsult = NULL;
  if (lst1 != NULL && lst2 != NULL) {
    buf1 = NewPost (lst1, 0);
    buf2 = NewPost (lst2, 0);
    k1 = PostLength (buf1);
    k2 = PostLength (buf2);
    k = MIN (k1, k2);
    rsult = BSNew (k * sizeof (DocUid));
    buf3 = NewPost (rsult, k * (Int4) sizeof (DocUid));
    if (rsult != NULL && buf1 != NULL && buf2 != NULL && buf3 != NULL) {
      RewindPost (buf1);
      RewindPost (buf2);
      pstar = ReadItem (buf1);
      qstar = ReadItem (buf2);
      while (k1 > 0 && k2 > 0) {
        if (pstar < qstar) {
          k1--;
          pstar = ReadItem (buf1);
        } else if (qstar < pstar) {
          k2--;
          qstar = ReadItem (buf2);
        } else {
          WriteItem (buf3, pstar);
          k1--;
          k2--;
          pstar = ReadItem (buf1);
          qstar = ReadItem (buf2);
        }
      }
      FlushItems (buf3);
    } else {
      ErrPostEx (SEV_ERROR, ERR_CD_BOOL, 0, "List is too large to intersect");
    }
    if (buf1 != NULL) {
      FreePost (buf1);
    }
    if (buf2 != NULL) {
      FreePost (buf2);
    }
    if (buf3 != NULL) {
      buf3->uids = NULL;
      FreePost (buf3);
    }
  }
  return rsult;
}

static ByteStorePtr NEAR DifferencePostingLists (ByteStorePtr lst1, ByteStorePtr lst2)

{
  PostingPtr    buf1;
  PostingPtr    buf2;
  PostingPtr    buf3;
  Int4          k;
  Int4          k1;
  Int4          k2;
  DocUid        pstar;
  DocUid        qstar;
  ByteStorePtr  rsult;

  ProgMon ("DiffPostingLists");
  rsult = NULL;
  if (lst1 != NULL && lst2 != NULL) {
    buf1 = NewPost (lst1, 0);
    buf2 = NewPost (lst2, 0);
    k1 = PostLength (buf1);
    k2 = PostLength (buf2);
    k = k1 + k2;
    rsult = BSNew (k * sizeof (DocUid));
    buf3 = NewPost (rsult, k * (Int4) sizeof (DocUid));
    if (rsult != NULL && buf1 != NULL && buf2 != NULL && buf3 != NULL) {
      RewindPost (buf1);
      RewindPost (buf2);
      pstar = ReadItem (buf1);
      qstar = ReadItem (buf2);
      while (k > 0) {
        if (pstar < qstar) {
          WriteItem (buf3, pstar);
          k--;
          pstar = ReadItem (buf1);
        } else if (qstar < pstar) {
          k--;
          qstar = ReadItem (buf2);
        } else {
          k -= 2;
          pstar = ReadItem (buf1);
          qstar = ReadItem (buf2);
        }
      }
      FlushItems (buf3);
    } else {
      ErrPostEx (SEV_ERROR, ERR_CD_BOOL, 0, "List is too large to difference");
    }
    if (buf1 != NULL) {
      FreePost (buf1);
    }
    if (buf2 != NULL) {
      FreePost (buf2);
    }
    if (buf3 != NULL) {
      buf3->uids = NULL;
      FreePost (buf3);
    }
  } else if (lst1 != NULL) {
    rsult = lst1;
  }
  return rsult;
}

static ByteStorePtr NEAR FreePostingList (ByteStorePtr lst)

{
  if (lst != NULL) {
    BSFree (lst);
  }
  return NULL;
}

static ByteStorePtr NEAR LoadPostingList (FILE *f, Int4 special, Int4 total)

{
  VoidPtr       bufr;
  Int4          cnt;
  Int4          cntr;
  Int4          k1;
  Int4          k2;
  ByteStorePtr  lst1;
  ByteStorePtr  lst2;
  ByteStorePtr  rsult;

  rsult = NULL;
  if (f != NULL && special >= 0 && total >= 0) {
    bufr = MemNew (MAX_CDENTREZ_BYTESTORE * sizeof (DocUid));
    if (bufr != NULL) {
      k1 = special;
      k2 = total - special;
      lst1 = BSNew (k1 * sizeof (DocUid));
      if (lst1 != NULL) {
        cntr = k1;
        cnt = MIN (k1, (long) MAX_CDENTREZ_BYTESTORE);
        while (cnt > 0) {
          FileRead (bufr, sizeof (DocUid), (size_t) cnt, f);
          BSWrite (lst1, bufr, cnt * sizeof (DocUid));
          cntr -= cnt;
          cnt = MIN (cntr, (long) MAX_CDENTREZ_BYTESTORE);
        }
      } else {
        ErrPostEx (SEV_ERROR, ERR_CD_BOOL, 0, "List is too large to load");
      }
      lst2 = BSNew (k2 * sizeof (DocUid));
      if (lst2 != NULL) {
        cntr = k2;
        cnt = MIN (k2, (long) MAX_CDENTREZ_BYTESTORE);
        while (cnt > 0) {
          FileRead (bufr, sizeof (DocUid), (size_t) cnt, f);
          BSWrite (lst2, bufr, cnt * sizeof (DocUid));
          cntr -= cnt;
          cnt = MIN (cntr, (long) MAX_CDENTREZ_BYTESTORE);
        }
      } else {
        ErrPostEx (SEV_ERROR, ERR_CD_BOOL, 0, "List is too large to load");
      }
      rsult = MergePostingLists (lst1, lst2);
    }
    MemFree (bufr);
  }
  return rsult;
}

/*****************************************************************************
*
*   CdEntrezCreateTerm (term, type, field, special, remainder)
*       Creates a CdTerm node in the entrez set structure if one does not yet
*       exist, and loads the posting file from two ByteStorePtr posting lists.
*
*****************************************************************************/

static void NEAR SavePostingList (FILE *f, ByteStorePtr bsp)

{
  VoidPtr  bufr;
  Int4     cnt;
  Int4     cntr;

  if (f != NULL && bsp != NULL) {
    bufr = MemNew (MAX_CDENTREZ_BYTESTORE * sizeof (DocUid));
    if (bufr != NULL) {
      cntr = (BSLen (bsp) / (Int4) sizeof (DocUid));
      cnt = MIN (cntr, (long) MAX_CDENTREZ_BYTESTORE);
      BSSeek (bsp, 0L, 0);
      while (cnt > 0) {
        BSRead (bsp, bufr, cnt * sizeof (DocUid));
        FileWrite (bufr, sizeof (DocUid), (size_t) cnt, f);
        cntr -= cnt;
        cnt = MIN (cntr, (long) MAX_CDENTREZ_BYTESTORE);
      }
    }
    MemFree (bufr);
  }
}

static CdTermPtr NEAR CdEntrezCreateTerm (CharPtr term, DocType type, DocField field, ByteStorePtr special, ByteStorePtr remainder, CharPtr highRange)

{
  FILE      *fp;
  Boolean   goOn;
  CdTermPtr last;
  Int4      remainderCount;
  Int4      specialCount;
  CdTermPtr trmp;

  trmp = NULL;
  if (eset != NULL && term != NULL) {
    trmp = eset->next;
    last = eset;
    goOn = TRUE;
    while (trmp != NULL && goOn) {
      if (trmp->type == type && trmp->field == field &&
          EqualTerms (trmp->term, term) &&
          EqualTerms (trmp->highRange, highRange)) {
        goOn = FALSE;
      } else {
        last = trmp;
        trmp = trmp->next;
      }
    }
    if (goOn) {
      trmp = MemNew (sizeof (CdTerm));
      if (trmp != NULL) {
        specialCount = 0;
        remainderCount = 0;
        if (special != NULL) {
          specialCount = (BSLen (special) / (Int4) sizeof (DocUid));
        }
        if (remainder != NULL) {
          remainderCount = (BSLen (remainder) / (Int4) sizeof (DocUid));
        }
        trmp->type = type;
        trmp->field = field;
        trmp->term = StringSave (term);
        trmp->special_count = specialCount;
        trmp->total_count = specialCount + remainderCount;
        trmp->highRange = StringSave(highRange);
        trmp->next = NULL;
        last->next = trmp;
        fp = FileOpen (eset->term, "ab");
        if (fp != NULL) {
          fseek (fp, 0, SEEK_END);
          trmp->offset = ftell (fp);
          SavePostingList (fp, special);
          SavePostingList (fp, remainder);
          FileClose (fp);
        } else {
          trmp->offset = 0;
        }
      }
    }
  }
  return trmp;
}

/*****************************************************************************
*
*   FindTermNode (term, type, field, highRange)
*       Returns a pointer to a CdTerm node in the entrez set structure,
*       creating the node and loading the posting file, if necessary.  The
*       value of the offset field becomes the offset into the temporary file.
*
*****************************************************************************/

static CdTermPtr NEAR FindTermNode (CharPtr term, DocType type, DocField field, CharPtr highRange)

{
  FILE      *fp;
  Boolean   goOn;
  CdTermPtr last;
  Int4      offset;
  Int4      remain;
  Int4      special;
  CharPtr   tmp;
  Int4      total;
  CdTermPtr trmp;

  trmp = NULL;
  if (eset != NULL && term != NULL) {
    trmp = eset->next;
    last = eset;
    goOn = TRUE;
    while (trmp != NULL && goOn) {
      if (trmp->type == type && trmp->field == field &&
          EqualTerms (trmp->term, term) &&
          EqualTerms (trmp->highRange, highRange)) {
        goOn = FALSE;
      } else {
        last = trmp;
        trmp = trmp->next;
      }
    }
    if (goOn) {
      tmp = term;
      while (*tmp != '\0')
        tmp++;
      tmp -= 3;
      rangeScanning = FALSE;
      if (highRange != NULL) {
          rangeScanning = TRUE;
          StrNCpy(topOfRange, highRange, sizeof(topOfRange));
          CdEntrezMergeTerm (type, field, term, NULL, NULL, NULL);
      } else {
        if ((*tmp == '.') && (*(tmp+1) == '.') && (*(tmp+2) == '.')) {
          CdEntrezMergeTerm (type, field, term, NULL, NULL, NULL);
        } else if (StringChr (term, '*') != NULL || StringChr (term, '?') != NULL) {
          CdEntrezMergeTerm (type, field, term, NULL, NULL, WildCardProc);
        }
      }
      trmp = eset->next;
      last = eset;
      goOn = TRUE;
      while (trmp != NULL && goOn) {
        if (trmp->type == type && trmp->field == field &&
            EqualTerms (trmp->term, term) &&
            EqualTerms (trmp->highRange, highRange)) {
          goOn = FALSE;
        } else {
          last = trmp;
          trmp = trmp->next;
        }
      }
    }
    if (goOn) {
#ifdef _NEW_CdEntrez_
		if (_nouveau)
			trmp = cd3_CdTrmFind(type,field,term);
#endif
#ifdef _OLD_CdEntrez_
		if (!_nouveau)
			trmp = CdTrmFind(type,field,term);
#endif
      if (trmp != NULL) {
        if (field != FLD_ORGN) {
          last->next = trmp;
          fp = FileOpen (eset->term, "rb");
          if (fp != NULL) {
            fseek (fp, 0, SEEK_END);
            offset = ftell (fp);
            FileClose (fp);
          } else {
            offset = 0;
          }
#ifdef _NEW_CdEntrez_
          if (_nouveau)
            cd3_CdTrmUidsFil (type, field, trmp->offset, trmp->total_count, eset->term, TRUE);
#endif
#ifdef _OLD_CdEntrez_
          if (!_nouveau)
            CdTrmUidsFil (type, field, trmp->offset, trmp->total_count, eset->term, TRUE);
#endif
          trmp->offset = offset;
        } else {
          db = type;
          fld = field;
          uidPtr = MemNew ((size_t) MAX_CDENTREZ_UID_LIST_SIZE);
          if (uidPtr != NULL) {
            scanPtr = MemNew (SCAN_MAX * sizeof (ScanData));
            if (scanPtr != NULL) {
              scanOk = TRUE;
              scanCount = 0;
              specialPost = NULL;
              remainPost = NULL;
              ScanOnlyProc (trmp);
              if (scanCount > 0) {
                ProcessScanResults ();
              }
              if (specialPost != NULL && remainPost != NULL) {
                remainPost = DifferencePostingLists (remainPost, specialPost);
              }
              if (specialPost == NULL) {
                specialPost = BSNew (0);
              }
              if (remainPost == NULL) {
                remainPost = BSNew (0);
              }
              special = BSLen (specialPost) / sizeof (DocUid);
              remain = BSLen (remainPost) / sizeof (DocUid);
              total = special + remain;
              scanPtr = MemFree (scanPtr);
            }
            uidPtr = MemFree (uidPtr);
            if (scanOk && total > 0) {
              trmp = CdTermFree (trmp);
              trmp = CdEntrezCreateTerm (term, db, fld, specialPost, remainPost, highRange);
            }
            specialPost = BSFree (specialPost);
            remainPost = BSFree (remainPost);
          }
        }
      }
    }
  }
  return trmp;
}

/*****************************************************************************
*
*   CdEntrezPreloadMerge (term, type, field, spcl, totl)
*       Creates a CdTerm node in the entrez set structure if one does not yet
*       exist, and loads the posting file by merging multiple postings files.
*
*****************************************************************************/

static void NEAR SingleSpaces (CharPtr str)

{
  Char  ch;
  Int2  i;
  Int2  j;
  Int2  k;

  i = 0;
  j = 0;
  k = 0;
  ch = str [i];
  while (ch != '\0') {
    if (ch == ' ') {
      if (k == 0) {
        str [j] = ch;
        j++;
      }
      k++;
      i++;
    } else {
      k = 0;
      str [j] = ch;
      i++;
      j++;
    }
    ch = str [i];
  }
  str [j] = '\0';
}

static void NEAR TermTruncate (CharPtr str)

{
  if (str != NULL && str [0] != '\0') {
    SingleSpaces (str);
    if (searchTermLen < (Int2) StringLen (str)) {
      str [searchTermLen] = '\0';
    }
  }
}

static int LIBCALLBACK HeapCompare (VoidPtr ptr1, VoidPtr ptr2)

{
  DocUidPtr  uid1;
  DocUidPtr  uid2;

  if (ptr1 != NULL && ptr2 != NULL) {
    uid1 = (DocUidPtr) ptr1;
    uid2 = (DocUidPtr) ptr2;
    if (*uid1 > *uid2) {
      return 1;
    } else if (*uid1 < *uid2) {
      return -1;
    } else {
      return 0;
    }
  } else {
    return 0;
  }
}

static void NEAR QuickSortSmall (DocUidPtr uids, Int4 l, Int4 r)

{
  HeapSort (uids + l, (size_t) (r - l + 1), sizeof (DocUid), HeapCompare);
}

/*
static Boolean NEAR AlreadyInOrder (DocUidPtr uids, Int4 l, Int4 r)

{
  DocUid   last;
  Boolean  rsult;

  rsult = TRUE;
  if (l < r) {
    last = 0;
    while (l <= r) {
      if (uids [l] < last) {
        rsult = FALSE;
      }
      last = uids [l];
      l++;
    }
  }
  return rsult;
}

static void NEAR QuickSortSmall (DocUidPtr uids, Int4 l, Int4 r)

{
  DocUid  a;
  DocUid  b;
  DocUid  c;
  Int4    i;
  Int4    j;
  DocUid  temp;
  DocUid  x;

  if (AlreadyInOrder (uids, l, r)) {
    return;
  }
  i = l;
  j = r;
  a = uids [l];
  b = uids [(l + r) / 2];
  c = uids [r];
  if (a > b) {
    if (c > a) {
      x = a;
    } else if (c < b) {
      x = b;
    } else {
      x = c;
    }
  } else {
    if (c < a) {
      x = a;
    } else if (c > b) {
      x = b;
    } else {
      x = c;
    }
  }
  do {
    while (uids [i] < x) {
      i++;
    }
    while (x < uids [j]) {
      j--;
    }
    if (i <= j) {
      temp = uids [i];
      uids [i] = uids [j];
      uids [j] = temp;
      i++;
      j--;
    }
  } while (i <= j);
  if (i - l < r - j) {
    if (l < j) {
      QuickSortSmall (uids, l, j);
    }
    if (i < r) {
      QuickSortSmall (uids, i, r);
    }
  } else {
    if (i < r) {
      QuickSortSmall (uids, i, r);
    }
    if (l < j) {
      QuickSortSmall (uids, l, j);
    }
  }
}
*/

static Int4 NEAR CompressSmall (DocUidPtr uids, Int4 count)

{
  Int4  i;
  Int4  j;

  i = 0;
  j = 0;
  while (i < count) {
    if (uids [i] > 0) {
      uids [j] = uids [i];
      i++;
      j++;
    } else {
      i++;
    }
  }
  i = j;
  while (j < count) {
    uids [j] = 0;
    j++;
  }
  return i;
}

static Int4 NEAR UniqueSmall (DocUidPtr uids, Int4 count)

{
  Int4    i;
  Int4    j;
  DocUid  last;

  i = 0;
  if (count <= 1) {
    i = count;
  } else {
    i = 0;
    j = 0;
    last = 0;
    while (i < count) {
      if (uids [i] != last) {
        uids [j] = uids [i];
        last = uids [i];
        i++;
        j++;
      } else {
        i++;
      }
    }
    i = j;
    while (j < count) {
      uids [j] = 0;
      j++;
    }
  }
  return i;
}

static ByteStorePtr NEAR MergeSmallLists (ByteStorePtr bsp, ByteStorePtr small)

{
  Int4       count;
  Int4       len;
  DocUidPtr  uids;

  if (small != NULL) {
    len = BSLen (small) / (Int4) sizeof (DocUid);
    if (len <= (long) MAX_CDENTREZ_SMALL_LIST && len > 0) {
      count = (Int4) len;
      uids = MemNew ((size_t) count * sizeof (DocUid));
      if (uids != NULL) {
        BSMerge (small, (VoidPtr) uids);
        small = BSFree (small);
        QuickSortSmall (uids, 0, (Int4) (count - 1));
        count = CompressSmall (uids, count);
        count = UniqueSmall (uids, count);
        if (count > 0) {
          small = BSNew (0L);
          BSWrite (small, uids, count * sizeof (DocUid));
        }
        uids = MemFree (uids);
        if (small != NULL) {
          bsp = MergePostingLists (bsp, small);
        }
      } else {
        ErrPostEx (SEV_ERROR, ERR_CD_BOOL, 0, "MergeSmallLists memory failure");
      }
    } else if (len > (long) MAX_CDENTREZ_SMALL_LIST) {
      ErrPostEx (SEV_ERROR, ERR_CD_BOOL, 0, "MergeSmallLists > %d", MAX_CDENTREZ_SMALL_LIST);
    }
  }
  return bsp;
}

static Boolean NEAR MergeUnorderedLists (Int4 i, Int4 count)

{
  BytePtr       bptr;
  Int4          finish;
  Boolean       goOn;
  Int4          j;
  Int4          len;
  Int4          max;
  DocUidPtr     mptr;
  Int4          number;
  Int4          offset;
  ByteStorePtr  remainLarge;
  ByteStorePtr  remainSmall;
  Int4          smallCount;
  Int4          start;
  Int4          total;

  goOn = TRUE;
  j = i + count - 1;
  max = scanPtr [j].offset + scanPtr [j].totalCount *
        (Int4) sizeof (DocUid) - scanPtr [i].offset;
  if (max <= MAX_CDENTREZ_UID_LIST_SIZE) {
    offset = scanPtr [i].offset;
    len = (Int4) (max / (Int4) sizeof (DocUid));
#ifdef _NEW_CdEntrez_
    if (_nouveau)
      cd3_CdTrmUidsMem (db, fld, offset, (Int4) len, uidPtr);
#endif
#ifdef _OLD_CdEntrez_
    if (!_nouveau)
      CdTrmUidsMem (db, fld, offset, (Int4) len, uidPtr);
#endif
    remainSmall = NULL;
    smallCount = 0;
    for (j = i; j < i + count; j++) {
      scanPtr [j].offset -= offset;
      total = scanPtr [j].totalCount;
      bptr = ((BytePtr) uidPtr) + scanPtr [j].offset;
      mptr = (DocUidPtr) bptr;
      if (smallCount + total > MAX_CDENTREZ_SMALL_LIST) {
        if (remainSmall != NULL) {
          remainPost = MergeSmallLists (remainPost, remainSmall);
          remainSmall = NULL;
        }
        smallCount = 0;
      }
      if (total > 100) {
        start = 0;
        number = 0;
        while (start < total) {
          finish = start + 1;
          while (finish < total && mptr [finish - 1] < mptr [finish]) {
            finish++;
          }
          number = finish - start;
          if (number > 100) {
            remainLarge = BSNew (number * sizeof (DocUid));
            BSWrite (remainLarge, (mptr + start), number * sizeof (DocUid));
            remainPost = MergePostingLists (remainPost, remainLarge);
          } else {
            smallCount += number;
            if (number > 0) {
              if (remainSmall == NULL) {
                remainSmall = BSNew (0L);
              }
              BSWrite (remainSmall, (mptr + start), number * sizeof (DocUid));
            }
            if (smallCount > MAX_CDENTREZ_SMALL_LIST) {
              if (remainSmall != NULL) {
                remainPost = MergeSmallLists (remainPost, remainSmall);
                remainSmall = NULL;
              }
              smallCount = 0;
            }
          }
          start = finish;
        }
      } else {
        smallCount += total;
        if (total > 0) {
          if (remainSmall == NULL) {
            remainSmall = BSNew (0L);
          }
          BSWrite (remainSmall, mptr, total * sizeof (DocUid));
        }
      }
    }
    if (remainSmall != NULL) {
      remainPost = MergeSmallLists (remainPost, remainSmall);
      remainSmall = NULL;
    }
  } else {
    ErrPostEx (SEV_ERROR, ERR_CD_BOOL, 0, "Cannot merge > 32 K element");
    scanOk = FALSE;
    goOn = FALSE;
  }
  return goOn;
}

static Boolean NEAR MergeSeveralOrderedLists (Int4 i, Int4 count)

{
  BytePtr       bptr;
  Boolean       goOn;
  Int4          j;
  Int4          len;
  Int4          max;
  DocUidPtr     mptr;
  Int4          offset;
  Int4          remainder;
  ByteStorePtr  remainLarge;
  ByteStorePtr  remainSmall;
  Int4          smallCount;
  Int4          special;
  ByteStorePtr  specialLarge;
  ByteStorePtr  specialSmall;
  Int4          total;

  goOn = TRUE;
  j = i + count - 1;
  max = scanPtr [j].offset + scanPtr [j].totalCount *
        (Int4) sizeof (DocUid) - scanPtr [i].offset;
  if (max <= MAX_CDENTREZ_UID_LIST_SIZE) {
    offset = scanPtr [i].offset;
    len = (Int4) (max / (Int4) sizeof (DocUid));
#ifdef _NEW_CdEntrez_
    if (_nouveau)
      cd3_CdTrmUidsMem (db, fld, offset, (Int4) len, uidPtr);
#endif
#ifdef _OLD_CdEntrez_
    if (!_nouveau)
      CdTrmUidsMem (db, fld, offset, (Int4) len, uidPtr);
#endif
    specialSmall = NULL;
    remainSmall = NULL;
    smallCount = 0;
    for (j = i; j < i + count; j++) {
      scanPtr [j].offset -= offset;
      special = scanPtr [j].specialCount;
      total = scanPtr [j].totalCount;
      remainder = total - special;
      bptr = ((BytePtr) uidPtr) + scanPtr [j].offset;
      mptr = (DocUidPtr) bptr;
      if (smallCount + total > MAX_CDENTREZ_SMALL_LIST) {
        if (specialSmall != NULL) {
          specialPost = MergeSmallLists (specialPost, specialSmall);
          specialSmall = NULL;
        }
        if (remainSmall != NULL) {
          remainPost = MergeSmallLists (remainPost, remainSmall);
          remainSmall = NULL;
        }
        smallCount = 0;
      }
      if (total > 100) {
        specialLarge = BSNew (special * sizeof (DocUid));
        BSWrite (specialLarge, mptr, special * sizeof (DocUid));
        specialPost = MergePostingLists (specialPost, specialLarge);
        remainLarge = BSNew (remainder * sizeof (DocUid));
        BSWrite (remainLarge, (mptr + special),
                 remainder * sizeof (DocUid));
        remainPost = MergePostingLists (remainPost, remainLarge);
      } else {
        smallCount += total;
        if (special > 0) {
          if (specialSmall == NULL) {
            specialSmall = BSNew (0L);
          }
          BSWrite (specialSmall, mptr, special * sizeof (DocUid));
        }
        if (remainder > 0) {
          if (remainSmall == NULL) {
            remainSmall = BSNew (0L);
          }
          BSWrite (remainSmall, (mptr + special), remainder * sizeof (DocUid));
        }
      }
    }
    if (specialSmall != NULL) {
      specialPost = MergeSmallLists (specialPost, specialSmall);
      specialSmall = NULL;
    }
    if (remainSmall != NULL) {
      remainPost = MergeSmallLists (remainPost, remainSmall);
      remainSmall = NULL;
    }
  } else {
    ErrPostEx (SEV_ERROR, ERR_CD_BOOL, 0, "Cannot merge > %ld element", (long) MAX_CDENTREZ_UID_LIST_SIZE);
    scanOk = FALSE;
    goOn = FALSE;
  }
  return goOn;
}

static Boolean NEAR MergeSeveralLists (Int4 i, Int4 count)

{
  if (fld != FLD_ORGN) {
    return MergeSeveralOrderedLists (i, count);
  } else {
    return MergeUnorderedLists (i, count);
  }
}

static Boolean NEAR ProcessScanResults (void)

{
  Boolean  goOn;
  Int4     i;
  Int4     j;
  Int4     max;

  ProgMon ("ProcessScanResults");
  goOn = TRUE;
  i = 0;
  j = 0;
  max = 0;
  while (j < scanCount) {
    if (scanPtr [j].offset < scanPtr [i].offset) {
      goOn = MergeSeveralLists (i, (Int4) (j - i));
      max = 0;
      i = j;
    } else {
      max = scanPtr [j].offset + scanPtr [j].totalCount *
            (Int4) sizeof (DocUid) - scanPtr [i].offset;
      if (max >= MAX_CDENTREZ_UID_LIST_SIZE) {
        if (j == i) {
          goOn = MergeSeveralLists (i, 1);
          j++;
          i = j;
          max = 0;
        } else {
          goOn = MergeSeveralLists (i, (Int4) (j - i));
          i = j;
          max = 0;
        }
      } else {
        j++;
      }
    }
  }
  if (max > 0) {
    goOn = MergeSeveralLists (i, (Int4) (j - i));
  }
  scanCount = 0;
  return goOn;
}

static Boolean  ScanOnlyProc (CdTermPtr trmp)

{
  Int4     count;
  Boolean  goOn;

  goOn = TRUE;
  if (scanCount >= SCAN_MAX) {
    goOn = ProcessScanResults ();
  }
  if (scanCount < SCAN_MAX) {
    if (trmp->total_count >= CDENTREZ_TERM_MAX) {
      while (trmp->special_count > 0) {
        if (scanCount >= SCAN_MAX) {
          goOn = ProcessScanResults ();
        }
        count = MIN (trmp->special_count, (long) CDENTREZ_TERM_MAX);
        scanPtr [scanCount].specialCount = count;
        scanPtr [scanCount].totalCount = count;
        scanPtr [scanCount].offset = trmp->offset;
        scanPtr [scanCount].specialPtr = NULL;
        scanPtr [scanCount].remainderPtr = NULL;
        scanCount++;
        trmp->special_count -= count;
        trmp->total_count -= count;
        trmp->offset += count * sizeof (DocUid);
      }
      while (trmp->total_count > 0) {
        if (scanCount >= SCAN_MAX) {
          goOn = ProcessScanResults ();
        }
        count = MIN (trmp->total_count, (long) CDENTREZ_TERM_MAX);
        scanPtr [scanCount].specialCount = 0;
        scanPtr [scanCount].totalCount = count;
        scanPtr [scanCount].offset = trmp->offset;
        scanPtr [scanCount].specialPtr = NULL;
        scanPtr [scanCount].remainderPtr = NULL;
        scanCount++;
        trmp->total_count -= count;
        trmp->offset += count * sizeof (DocUid);
      }
    } else {
      if (scanCount >= SCAN_MAX) {
        goOn = ProcessScanResults ();
      }
      scanPtr [scanCount].specialCount = trmp->special_count;
      scanPtr [scanCount].totalCount = trmp->total_count;
      scanPtr [scanCount].offset = trmp->offset;
      scanPtr [scanCount].specialPtr = NULL;
      scanPtr [scanCount].remainderPtr = NULL;
      scanCount++;
    }
  }
  return goOn;
}

static Boolean  WildCardProc (CdTermPtr trmp)

{
  Int4     diff;
  Boolean  goOn;
  CharPtr  src;
  CharPtr  tgt;

  goOn = FALSE;
  src = selection;
  tgt = trmp->term;
  diff = 0;
  while (*src != '\0' && *tgt != '\0' && diff == 0) {
    if (*src != '?') {
      diff = TO_UPPER (*src) - TO_UPPER (*tgt);
    }
    if (diff == 0) {
      src++;
      tgt++;
    }
  }
  if (diff != 0) {
    if (*src == '*') {
      goOn = TRUE;
    }
  } else if (*src == '*') {
    goOn = TRUE;
  } else if (*src == '\0' && *tgt == '\0') {
    goOn = TRUE;
  } else {
    goOn = FALSE;
  }
  return goOn;
}

static Boolean  ScanAndFreeProc (CdTermPtr trmp)

{
  Int4     compare;
  Boolean  goOn;
  Char     str [256];

  goOn = TRUE;
  if (trmp != NULL && trmp->term != NULL) {
    if (rangeScanning) {
      compare = MeshStringICmp (trmp->term, selection);
      if (compare >= 0) {
        if (topOfRange[0] == '\0')
          compare = -1;
        else
          compare = MeshStringICmp (trmp->term, topOfRange);
        if (compare > 0)
          goOn = FALSE;
        else
          goOn = ScanOnlyProc (trmp);
      }
    } else {
      StringNCpy (str, trmp->term, sizeof (str));
      TermTruncate (str);
      if (userScanProc != NULL) {
        compare = MeshStringICmp (str, wildcard);
      } else {
        compare = MeshStringICmp (str, selection);
      }
      if (compare > 0) {
        str [searchTermLen] = '\0';
        if (userScanProc != NULL) {
          compare = MeshStringICmp (str, wildcard);
        } else {
          compare = MeshStringICmp (str, selection);
        }
        if (compare > 0) {
          goOn = FALSE;
        }
      } else if (compare == 0) {
        if (userScanProc != NULL) {
          if (userScanProc (trmp)) {
            goOn = ScanOnlyProc (trmp);
          }
        } else {
          goOn = ScanOnlyProc (trmp);
        }
      }
    }
  }
  trmp = CdTermFree (trmp);
  return goOn;
}

static Boolean NEAR CdEntrezMergeTerm (DocType type, DocField field, CharPtr term,
                                        Int4Ptr spcl, Int4Ptr totl, CdTermProc userProc)

{
  Char  ch;
  Int4  remain;
  Int4  special;
  Char  str [256];
  Int4  total;
  Int4  termpage;
  CharPtr tmp;
  Int4  limit = 0;
  CharPtr prop;
  Boolean retval = FALSE;

  if (spcl != NULL) {
    *spcl = 0;
  }
  if (totl != NULL) {
    *totl = 0;
  }
  db = type;
  fld = field;
  userScanProc = userProc;
  StringNCpy (str, term, sizeof (str));
  tmp = str;
  while (*tmp != '\0') {
    tmp++;
  }
  tmp -= 3;
  if ((*tmp == '.') && (*(tmp+1) == '.') && (*(tmp+2) == '.')) {
    *tmp = '\0';
  }
  SingleSpaces (str);
  if (userProc != NULL) {
    searchTermLen = 0;
    ch = str [searchTermLen];
    while (ch != '\0' && ch != '*' && ch != '?') {
      searchTermLen++;
      ch = str [searchTermLen];
    }
  } else {
    searchTermLen = (Int4) StringLen (str);
  }
  if ((prop = (CharPtr) GetAppProperty("CdEntrezTruncLimit")) != NULL)
  {
    limit = atoi(prop);
  }
  if (searchTermLen > limit || str [0] == '?' || str [0] == '*' ||
      rangeScanning) {
    scanOk = TRUE;
    uidPtr = MemNew ((size_t) MAX_CDENTREZ_UID_LIST_SIZE);
    if (uidPtr != NULL) {
      scanPtr = MemNew (SCAN_MAX * sizeof (ScanData));
      if (scanPtr != NULL) {
        scanCount = 0;
        specialPost = NULL;
        remainPost = NULL;
        StringNCpy (selection, str, sizeof (selection));
        StringNCpy (wildcard, str, sizeof (wildcard));
        wildcard [searchTermLen] = '\0';
#ifdef _NEW_CdEntrez_
		if (_nouveau)
          termpage = cd3_CdTrmLookup (db, fld, wildcard);
#endif
#ifdef _OLD_CdEntrez_
		if (!_nouveau)
          termpage = CdTrmLookup (db, fld, wildcard);
#endif
        if (fld == FLD_MESH) {
          ch = str [0];
          str [0] = TO_UPPER (ch);
        }
        if (termpage >= 0) {
#ifdef _NEW_CdEntrez_
			if (_nouveau)
				cd3_CdTermScan (db, fld, termpage, (Int4)0, ScanAndFreeProc);
#endif
#ifdef _OLD_CdEntrez_
			if (!_nouveau)
				CdTermScan (db, fld, termpage, (Int4)0, ScanAndFreeProc);
#endif
        }
        if (scanCount > 0) {
          ProcessScanResults ();
        }
        if (specialPost != NULL && remainPost != NULL) {
          remainPost = DifferencePostingLists (remainPost, specialPost);
        }
        if (specialPost == NULL) {
          specialPost = BSNew (0);
        }
        if (remainPost == NULL) {
          remainPost = BSNew (0);
        }
        special = BSLen (specialPost) / sizeof (DocUid);
        remain = BSLen (remainPost) / sizeof (DocUid);
        total = special + remain;
        scanPtr = MemFree (scanPtr);
      }
      uidPtr = MemFree (uidPtr);
      if (scanOk && total > 0) {
		retval = TRUE;
		if (userProc == NULL && !rangeScanning) {
			StringCat (str, "...");
		}
        CdEntrezCreateTerm (str, db, fld, specialPost, remainPost, rangeScanning ? topOfRange : NULL);
        if (spcl != NULL) {
          *spcl = special;
        }
        if (totl != NULL) {
          *totl = total;
        }
      }
      specialPost = BSFree (specialPost);
      remainPost = BSFree (remainPost);
    }
  }
  return retval;
}

/*****************************************************************************
*
*   CdEntMedlineEntryListGet (result, numuid, uids, mark_missing)
*   	returns a count of entries read
*   	if (mark_missing) ids which could not be located are made negative
*
*****************************************************************************/
NLM_EXTERN Int2  CdEntMedlineEntryListGet (MedlineEntryPtr PNTR result, Int2 numuid, Int4Ptr uids, Boolean mark_missing)

{
	MedlineEntryPtr mep;
	Int2 count = 0, ctr;
	AsnIoPtr aip;
	DocType db = TYP_ML;

	if (! MedlineAsnLoad())
		return 0;

	for (ctr = 0; ctr < numuid; ctr++)
	{
		mep = NULL;

#ifdef _NEW_CdEntrez_
		if (_nouveau)
			aip = cd3_CdDocAsnOpen(db, uids[ctr]);
#endif
#ifdef _OLD_CdEntrez_
		if (!_nouveau)
			aip = CdDocAsnOpen(db, uids[ctr]);
#endif
		if (aip != NULL)
		{
		 	mep = MedlineEntryAsnRead(aip, NULL);
#ifdef _NEW_CdEntrez_
		 	if (_nouveau)
	 			cd3_CdDocAsnClose(aip);
#endif
#ifdef _OLD_CdEntrez_
		 	if (!_nouveau)
	 			CdDocAsnClose(aip);
#endif
		}
		if (mep == NULL)    /* didn't get it */
		{
			if (mark_missing)
				uids[ctr] *= -1;
		}
		else
		{
			count++;
			result[ctr] = mep;
		}
	}
	
	return count;
}

/*****************************************************************************
*
*   CdEntSeqEntryListGet (result, numuid, uids, retcode, mark_missing)
*   	returns a count of entries read
*   	if (mark_missing) ids which could not be located are made negative
*       retcode is defined in objsset.h 
*
*****************************************************************************/
static AsnIo* CdSeqAsnOpen (DocType *type, DocUid uid, Boolean isGenome);

NLM_EXTERN Int2  CdEntSeqEntryListGet (SeqEntryPtr PNTR result, Int2 numuid, Int4Ptr uids, Int2 retcode, Boolean mark_missing)
{
	SeqEntryPtr sep;
	Int2 count = 0, ctr;
	AsnIoPtr aip;
	DocType db = TYP_SEQ;
	ValNode an;

	if (! SeqSetAsnLoad())
		return 0;

    an.data.intvalue = 0;
    an.choice = SEQID_GI;

	for (ctr = 0; ctr < numuid; ctr++)
	{
		sep = NULL;
		aip = CdSeqAsnOpen(&db, uids[ctr], retcode == -1);
		if (aip != NULL)
		{
			an.data.intvalue = uids[ctr];
			if (retcode == -1)
		 		sep = SeqEntryAsnRead(aip, NULL);
			else
		 		sep = SeqEntryAsnGet(aip, NULL, &an, retcode);
#ifdef _NEW_CdEntrez_
			if (_nouveau)
	 			cd3_CdDocAsnClose(aip);
#endif
#ifdef _OLD_CdEntrez_
			if (!_nouveau)
	 			CdDocAsnClose(aip);
#endif
		}
		if (sep == NULL)    /* didn't get it */
		{
			if (mark_missing)
				uids[ctr] *= -1;
		}
		else
		{
			count++;
			result[ctr] = sep;
		}
	}
	
	return count;
}


static AsnIo* CdSeqAsnOpen (DocType *type, DocUid uid, Boolean isGenome)
{
	AsnIo *aio = NULL;
	
#ifdef _NEW_CdEntrez_
	if (_nouveau)
	{
		if (isGenome) {
			if ((aio = cd3_CdDocAsnOpen(TYP_CH,uid)) != NULL)
				*type = TYP_CH;
		} else {
			if (*type != TYP_SEQ)
			{
				aio = cd3_CdDocAsnOpen(*type,uid);
			}
			else
			{
				if ((aio = cd3_CdDocAsnOpen(TYP_AA,uid)) != NULL)
					*type = TYP_AA;
				else if ((aio = cd3_CdDocAsnOpen(TYP_NT,uid)) != NULL)
						*type = TYP_NT;
			}
		}
	}
#endif

#ifdef _OLD_CdEntrez_
	if (!_nouveau)
		aio = CdDocAsnOpen(*type,uid);
#endif

	return aio;
}

/*****************************************************************************
*
*   CdEntMlSumListGet (result, numuid, uids)
*   	returns a count of entries read
*   	head of linked list is in result
*
*****************************************************************************/

NLM_EXTERN Int2  CdEntMlSumListGet (DocSumPtr PNTR result, Int2 numuid, Int4Ptr uids)          /* Gi numbers */
{
	Int2 count = 0;

#ifdef _NEW_CdEntrez_
	if (_nouveau)
		count = CdDocSumListGet(result,numuid,TYP_ML,uids);
#endif

#ifdef _OLD_CdEntrez_
	if (!_nouveau)
	{
		Int2 ctr;
		DocType db = TYP_ML;
		AsnIoPtr aip;
	
		for (ctr = 0; ctr < numuid; ctr++)
		{
			result[ctr] = NULL;
			aip = CdDocAsnOpen (db, uids[ctr]);
		    if (aip != NULL)
			{
				result[ctr] = MedSumAsnRead(aip, uids[ctr]);
				CdDocAsnClose(aip);
				if (result[ctr] != NULL)
					count++;
			}
		}
	}
#endif
	
	return count;
}


/*****************************************************************************
*
*   CdEntMlSumGet(uid)
*   	get one MlSummary
*
*****************************************************************************/
#ifdef _OLD_CdEntrez_

static DocSumPtr NEAR CdEntMlSumGet (Int4 uid)
{
	DocSumPtr dsp = NULL;

	CdEntMlSumListGet(&dsp, 1, &uid);
	return dsp;
}

#endif

/*****************************************************************************
*
*   void StripAuthor(author)
*
*****************************************************************************/
static void NEAR StripAuthor (CharPtr author)

{
  CharPtr  p1, p2;

  p1 = author;
  while ((p1 = StringChr (p1, ' ')) != NULL) {
    for (p2 = p1 + 1; *p2 != '\0'; p2++) {
      if (*p2 == ' ') break;
      if (IS_ALPHA (*p2) && IS_LOWER (*p2)) break;
    }
    if (*p2 == '\0' || *p2 == ' ') {
      *p1 = '\0';
      return;
    }
    p1++;
  }
}

/*****************************************************************************
*
*   MedSumAsnRead(aip, uid)
*
*****************************************************************************/
static void NEAR FindAsnType (AsnTypePtr PNTR atp, AsnModulePtr amp, CharPtr str)

{
  if (atp != NULL && (*atp) == NULL) {
    *atp = AsnTypeFind (amp, str);
  }
}


static DocSumPtr NEAR MedSumAsnRead (AsnIoPtr aip, DocUid uid)

{
  DataVal       av;
  AsnModulePtr  amp;
  AsnTypePtr    atp;
  Boolean       citFound;
  DocSumPtr     dsp;
  Boolean       goOn;
  Int2          i;
  CharPtr       ptr;
  Char          caption [50];
  Char          author [40];
  Char          year [10];

  if ((aip == NULL) || (! AllObjLoad ()))
    return NULL;

	  amp = AsnAllModPtr ();

	  FindAsnType (&MEDLINE_ENTRY, amp, "Medline-entry");
	  FindAsnType (&MEDLINE_ENTRY_cit, amp, "Medline-entry.cit");
	  FindAsnType (&MEDLINE_ENTRY_abstract, amp, "Medline-entry.abstract");
	  FindAsnType (&TITLE_E_trans, amp, "Title.E.trans");
	  FindAsnType (&AUTH_LIST_names_ml_E, amp, "Auth-list.names.ml.E");
	  FindAsnType (&AUTH_LIST_names_str_E, amp, "Auth-list.names.str.E");
	  FindAsnType (&DATE_STD_year, amp, "Date-std.year");
	  FindAsnType (&DATE_str, amp, "Date.str");
	  FindAsnType (&TITLE_E_name, amp, "Title.E.name");
	  FindAsnType (&MEDLINE_ENTRY_mesh, amp, "Medline-entry.mesh");
	  FindAsnType (&MEDLINE_ENTRY_substance, amp, "Medline-entry.substance");
	  FindAsnType (&MEDLINE_ENTRY_xref, amp, "Medline-entry.xref");
	  FindAsnType (&MEDLINE_ENTRY_idnum, amp, "Medline-entry.idnum");
	  FindAsnType (&MEDLINE_ENTRY_gene, amp, "Medline-entry.gene");

  atp = AsnReadId (aip, amp, MEDLINE_ENTRY);
  AsnReadVal (aip, atp, &av);

  dsp = MemNew (sizeof (DocSum));
  if (dsp != NULL) {
    dsp->no_abstract = TRUE;
    dsp->translated_title = FALSE;
    dsp->no_authors = TRUE;
    author [0] = '\0';
    year [0] = '\0';
    citFound = FALSE;
    goOn = TRUE;
    while (goOn) {
      atp = AsnReadId (aip, amp, atp);
      if (atp == MEDLINE_ENTRY) {
        AsnReadVal (aip, atp, NULL);
        goOn = FALSE;
      } else if (atp == MEDLINE_ENTRY_cit) {
        AsnReadVal (aip, atp, NULL);
        citFound = TRUE;
      } else if (atp == MEDLINE_ENTRY_abstract) {
        AsnReadVal (aip, atp, NULL);
        dsp->no_abstract = FALSE;
        goOn = FALSE;
      } else if (atp == TITLE_E_trans) {
        AsnReadVal (aip, atp, &av);
        dsp->translated_title = TRUE;
        if (dsp->title != NULL) {
          dsp->title = MemFree (dsp->title);
        }
        dsp->title = MemNew ((size_t) StringLen ((CharPtr) av.ptrvalue) + 3);
        ptr = dsp->title;
        *ptr = '[';
        ptr++;
        ptr = StringMove (ptr, (CharPtr) av.ptrvalue);
        *ptr = ']';
        ptr++;
        *ptr = '\0';
        AsnKillValue (atp, &av);
      } else if (atp == AUTH_LIST_names_ml_E) {
        AsnReadVal (aip, atp, &av);
        dsp->no_authors = FALSE;
        if (author [0] == '\0') {
          StringNCpy (author, (CharPtr) av.ptrvalue, sizeof (author));
        }
        AsnKillValue (atp, &av);
      } else if (atp == AUTH_LIST_names_str_E) {
        AsnReadVal (aip, atp, &av);
        dsp->no_authors = FALSE;
        if (author [0] == '\0') {
          StringNCpy (author, (CharPtr) av.ptrvalue, sizeof (author));
        }
        AsnKillValue (atp, &av);
      } else if (atp == DATE_STD_year) {
        AsnReadVal (aip, atp, &av);
        if (citFound) {
          sprintf (year, "%ld", (long) av.intvalue);
        }
      } else if (atp == DATE_str) {
        AsnReadVal (aip, atp, &av);
        if (citFound) {
          i = 0;
          ptr = av.ptrvalue;
          while (ptr [i] != '\0' && ptr [i] != ' ' && i < sizeof (year) - 1) {
            year [i] = ptr [i];
            i++;
          }
          year [i] = '\0';
        }
        AsnKillValue (atp, &av);
      } else if (atp == TITLE_E_name) {
        AsnReadVal (aip, atp, &av);
        if (dsp->title == NULL) {
          dsp->title = StringSave ((CharPtr) av.ptrvalue);
        }
        AsnKillValue (atp, &av);
      } else if (atp == MEDLINE_ENTRY_mesh || atp == MEDLINE_ENTRY_substance ||
                 atp == MEDLINE_ENTRY_xref || atp == MEDLINE_ENTRY_idnum ||
                 atp == MEDLINE_ENTRY_gene) {
        AsnReadVal (aip, atp, NULL);
        goOn = FALSE;
      } else {
        AsnReadVal (aip, atp, NULL);
      }
    }
    if (dsp->no_authors) {
      sprintf (caption, "[%ld], %s", (long) uid, year);
    } else if (author [0] != '\0') {
      StripAuthor (author);
      author [12] = '.';
      author [12] = '\0';
      sprintf (caption, "%s, %s", author, year);
    } else {
      sprintf (caption, "[%ld], %s", (long) uid, year);
    }
    dsp->caption = StringSave (caption);
    dsp->uid = uid;
  }
  AsnIoReset (aip);
  return dsp;
}


/*****************************************************************************
*
*   CdSeqIdForGI(Int4 gi)
*
*****************************************************************************/
NLM_EXTERN SeqIdPtr CdSeqIdForGI (Int4 gi)
{
#ifdef _NEW_CdEntrez_
	DocSum* dsp;
	SeqIdPtr sip = NULL, tmp, next;

	dsp = cd3_CdGetDocSum (TYP_NT, gi);   /* nucleic acid? */
	if (dsp == NULL)
		dsp = cd3_CdGetDocSum (TYP_AA, gi);  /* protein? */
	if (dsp != NULL)
	{
		tmp = SeqIdParse(dsp->extra);
		DocSumFree(dsp);

		while (tmp != NULL)
		{
			next = tmp->next;
			tmp->next = NULL;
			if (tmp->choice == SEQID_GI)
				SeqIdFree(tmp);
			else
				sip = tmp;
			tmp = next;
		}
	}
	return sip;

#else

	SeqIdPtr sip = NULL, ids, curr, best;
	AsnIoPtr aip;
	AsnModulePtr amp;
	AsnTypePtr atp;
	Boolean gotit;
	DocType db = TYP_SEQ;
	GiimPtr gip;

	static Uint1 pick_order[20] = {
 	83, /* 0 = not set */
	65, /* 1 = local Object-id */
	65,  /* 2 = gibbsq */
	65,  /* 3 = gibbmt */
	70, /* 4 = giim Giimport-id */
	60, /* 5 = genbank */
	60, /* 6 = embl */
	60, /* 7 = pir */
	60, /* 8 = swissprot */
	65,  /* 9 = patent */
	65, /* 10 = other TextSeqId */
	65, /* 11 = general Dbtag */
	90,  /* 12 = gi */
	60, /* 13 = ddbj */
	60, /* 14 = prf */
	60,  /* 15 = pdb */
	0,	/* extras for new ids */
	0,
	0,
	0
    };

	if (! AllObjLoad()) return sip;
	amp = AsnAllModPtr();
	FindAsnType (&SEQ_ENTRY, amp, "Seq-entry");
    FindAsnType (&BIOSEQ_id, amp, "Bioseq.id");
    FindAsnType (&BIOSEQ_id_E, amp, "Bioseq.id.E");

	aip = CdSeqAsnOpen (&db, gi, FALSE);
	if (aip == NULL) return sip;

	atp = SEQ_ENTRY;
	while ((atp = AsnReadId(aip, amp, atp)) != NULL)
	{
		if (atp == BIOSEQ_id)
		{
			gotit = FALSE;
		    ids = SeqIdSetAsnRead(aip, atp, BIOSEQ_id_E);
			for (curr = ids; curr != NULL; curr = curr->next)
			{
				if (curr->choice == SEQID_GIIM)
				{
					gip = (GiimPtr)(curr->data.ptrvalue);
					if (gip->id == gi)
					{
						gotit = TRUE;
						break;
					}
				}
				else if (curr->choice == SEQID_GI)
				{
					if (curr->data.intvalue == gi)
					{
						gotit = TRUE;
						break;
					}
				}
			}
			if (gotit)
			{
				best = SeqIdSelect(ids, pick_order, 20);
				sip = ValNodeExtract(&ids, (Int2)(best->choice));
			}
			SeqIdSetFree(ids);
			if (gotit)
				break;

		}
		else
			AsnReadVal(aip, atp, NULL);
		if (! AsnGetLevel(aip))       /* finished reading a Seq-entry */
			break;                    /* failed */
	}
	
#ifdef _NEW_CdEntrez_
	if (_nouveau)
	  cd3_CdDocAsnClose(aip);
#endif
#ifdef _OLD_CdEntrez_
	if (!_nouveau)
	  CdDocAsnClose(aip);
#endif

	return sip;
#endif
}



/*****************************************************************************
*
*   CdEntSeqSumListGet (result, numuid, db, uids)
*   	returns a count of entries read
*   	head of linked list is in result
*
*****************************************************************************/

NLM_EXTERN Int2  CdEntSeqSumListGet (DocSumPtr PNTR result, Int2 numuid, DocType db, Int4Ptr uids)          /* Gi numbers */
{
	Int2 count = 0;
	
#ifdef _NEW_CdEntrez_
	if (_nouveau)
	{
		ASSERT(db != TYP_SEQ);
		count = CdDocSumListGet(result,numuid,db,uids);
	}
#endif
	
#ifdef _OLD_CdEntrez_
	if (!_nouveau)
	{
		Int2 ctr;
		AsnIoPtr aip;
	
		for (ctr = 0; ctr < numuid; ctr++)
		{
			result[ctr] = NULL;
			aip = CdDocAsnOpen (db, uids[ctr]);
		    if (aip != NULL)
			{
				result[ctr] = CdSeqSumAsnRead(aip, uids[ctr]);
				CdDocAsnClose(aip);
				if (result[ctr] != NULL)
					count++;
			}
		}
	}
#endif

	return count;
}

/*****************************************************************************
*
*   CdEntSeqSumGet(uid, type)
*   	get one SeqSummary
*
*****************************************************************************/
#ifdef _OLD_CdEntrez_

static DocSumPtr NEAR CdEntSeqSumGet (Int4 uid, DocType type)
{
	DocSumPtr dsp = NULL;

	CdEntSeqSumListGet(&dsp, 1, type, &uid);
	return dsp;
}

#endif

NLM_EXTERN DocSumPtr CdSeqSumAsnRead (AsnIoPtr aip, DocUid uid)

{
  DataVal       av;
  AsnModulePtr  amp;
  AsnTypePtr    atp;
  DocSumPtr     dsp;
  Boolean       goOn;
  Char          caption [50];
  Char          author [40];
  Char          year [10];
  Char          locus [40];
  Char          cds [10];
  CharPtr       chptr;
  Int2          proteins;
  CharPtr       recentTitle;
  Boolean       backbone;
  Boolean       genBank;
  Boolean       embl;
  Boolean       ddbj;
  Boolean       pir;
  Boolean       swissprot;
  Boolean       isaNA;
  Boolean       isaAA;
  Boolean       isaSEG;
  Boolean		in_id;
  Int2          level;

  if ((aip == NULL) || (! AllObjLoad ()))
    return NULL;

  amp = AsnAllModPtr ();

  FindAsnType (&SEQ_ENTRY, amp, "Seq-entry");
  FindAsnType (&SEQ_ENTRY_seq, amp, "Seq-entry.seq");
  FindAsnType (&SEQ_ENTRY_set, amp, "Seq-entry.set");
  FindAsnType (&TEXTSEQ_ID_name, amp, "Textseq-id.name");
  FindAsnType (&TEXTSEQ_ID_accession, amp, "Textseq-id.accession");
  FindAsnType (&AUTH_LIST_names_str_E, amp, "Auth-list.names.str.E");
  FindAsnType (&DATE_STD_year, amp, "Date-std.year");
  FindAsnType (&DATE_str, amp, "Date.str");
  FindAsnType (&SEQ_DESCR_E_title, amp, "Seq-descr.E.title");
  FindAsnType (&GIIMPORT_ID_id, amp, "Giimport-id.id");
  FindAsnType (&BIOSEQ_inst, amp, "Bioseq.inst");
  FindAsnType (&SEQ_INST_mol, amp, "Seq-inst.mol");
  FindAsnType (&SEQ_INST_repr, amp, "Seq-inst.repr");
  FindAsnType (&SEQ_ID_gibbsq, amp, "Seq-id.gibbsq");
  FindAsnType (&SEQ_ID_gibbmt, amp, "Seq-id.gibbmt");
  FindAsnType (&SEQ_ID_genbank, amp, "Seq-id.genbank");
  FindAsnType (&SEQ_ID_gi, amp, "Seq-id.gi");
  FindAsnType (&SEQ_ID_embl, amp, "Seq-id.embl");
  FindAsnType (&SEQ_ID_ddbj, amp, "Seq-id.ddbj");
  FindAsnType (&SEQ_ID_pir, amp, "Seq-id.pir");
  FindAsnType (&SEQ_ID_swissprot, amp, "Seq-id.swissprot");
  FindAsnType (&PDB_BLOCK_compound_E, amp, "PDB-block.compound.E");
  FindAsnType (&PDB_SEQ_ID_MOL, amp, "PDB-seq-id.mol");
  FindAsnType (&BIOSEQ_id, amp, "Bioseq.id");
  FindAsnType (&CIT_PAT_title, amp, "Cit-pat.title");

  atp = AsnReadId (aip, amp, SEQ_ENTRY);
  AsnReadVal (aip, atp, &av);

  atp = AsnReadId (aip, amp, atp);
  AsnReadVal (aip, atp, &av);

  dsp = MemNew (sizeof (DocSum));
  if (dsp != NULL) {
    dsp->no_abstract = TRUE;
    dsp->translated_title = FALSE;
    dsp->no_authors = TRUE;
    author [0] = '\0';
    year [0] = '\0';
    locus [0] = '\0';
    cds [0] = '\0';
    proteins = 1;
    recentTitle = NULL;
    backbone = FALSE;
    genBank = FALSE;
    embl = FALSE;
	ddbj = FALSE;
    pir = FALSE;
    swissprot = FALSE;
    isaNA = FALSE;
    isaAA = FALSE;
    isaSEG = FALSE;
	in_id = FALSE;
    goOn = TRUE;
    level = AsnGetLevel (aip);
    while (goOn) {
      atp = AsnReadId (aip, amp, atp);
      if (atp == SEQ_ENTRY_seq || atp == SEQ_ENTRY_set) {
        AsnReadVal (aip, atp, NULL);
        if (AsnGetLevel (aip) <= level) {
          goOn = FALSE;
	    }
      } else if (atp == BIOSEQ_id) {
        AsnReadVal (aip, atp, &av);
		if (in_id) {
			in_id = FALSE;
		} else {
			in_id = TRUE;
        }
      } else if (in_id && ((atp == TEXTSEQ_ID_name) ||
 	  		(atp == PDB_SEQ_ID_MOL))) {
        AsnReadVal (aip, atp, &av);
        if (locus [0] == '\0') {
          StringNCpy (locus, (CharPtr) av.ptrvalue, sizeof (locus));
        }
        AsnKillValue (atp, &av);
      } else if (in_id && (atp == TEXTSEQ_ID_accession)) {
        AsnReadVal (aip, atp, &av);
        if (locus [0] == '\0') {
          StringNCpy (locus, (CharPtr) av.ptrvalue, sizeof (locus));
        }
        AsnKillValue (atp, &av);
      } else if (atp == AUTH_LIST_names_str_E) {
        AsnReadVal (aip, atp, &av);
        if (author [0] == '\0') {
          StringNCpy (author, (CharPtr) av.ptrvalue, sizeof (author));
        }
        AsnKillValue (atp, &av);
      } else if (atp == DATE_STD_year) {
        AsnReadVal (aip, atp, &av);
        sprintf (year, "%ld", (long) av.intvalue);
      } else if (atp == DATE_str) {
        AsnReadVal (aip, atp, &av);
        StringNCpy (year, (CharPtr) av.ptrvalue, sizeof (year));
        AsnKillValue (atp, &av);
      } else if ((atp == SEQ_DESCR_E_title) ||
 	  	(atp == PDB_BLOCK_compound_E) || (atp == CIT_PAT_title)) {
        AsnReadVal (aip, atp, &av);
		if (*((CharPtr)av.ptrvalue) != '\0')
		{
	        if (recentTitle != NULL) {
    	      recentTitle = MemFree (recentTitle);
        	}

	        if (dsp->uid == uid && dsp->title == NULL &&
				atp != CIT_PAT_title) {
    	      dsp->title = (CharPtr)av.ptrvalue;
	        }
			else
				recentTitle = (CharPtr)av.ptrvalue;
		}
		else
	        AsnKillValue (atp, &av);
      } else if (atp == GIIMPORT_ID_id || atp == SEQ_ID_gi) {
        AsnReadVal (aip, atp, &av);
        if (av.intvalue == uid) {
          dsp->uid = uid;
        }
      } else if (atp == SEQ_INST_mol) {
        AsnReadVal (aip, atp, &av);
        if ((! isaNA) && (! isaAA) && dsp->uid == uid) {
          isaNA = (Boolean) ISA_na (av.intvalue);
          isaAA = (Boolean) ISA_aa (av.intvalue);
          if (isaAA && cds [0] == '\0') {
            sprintf (cds, " cds%d", (int) proteins);
          }
        }
        if (ISA_aa (av.intvalue)) {
          proteins++;
        }
      } else if (atp == SEQ_INST_repr) {
        AsnReadVal (aip, atp, &av);
        if (av.intvalue == Seq_repr_seg) {
          isaSEG = TRUE;
        }
      } else if (atp == BIOSEQ_inst) {
        AsnReadVal (aip, atp, NULL);
        if (dsp->uid == uid && dsp->title == NULL) {
          dsp->title = recentTitle;
          recentTitle = NULL;
        }
      } else if (atp == SEQ_ID_gibbsq || atp == SEQ_ID_gibbmt) {
        AsnReadVal (aip, atp, NULL);
        backbone = TRUE;
      } else if (atp == SEQ_ID_genbank) {
        AsnReadVal (aip, atp, NULL);
		if (in_id)
	        genBank = TRUE;
      } else if (atp == SEQ_ID_embl) {
        AsnReadVal (aip, atp, NULL);
		if (in_id)
	        embl = TRUE;
      } else if (atp == SEQ_ID_ddbj) {
        AsnReadVal (aip, atp, NULL);
		if (in_id)
	        ddbj = TRUE;
      } else if (atp == SEQ_ID_pir) {
        AsnReadVal (aip, atp, NULL);
		if (in_id)
	        pir = TRUE;
      } else if (atp == SEQ_ID_swissprot) {
        AsnReadVal (aip, atp, NULL);
		if (in_id)
	        swissprot = TRUE;
      } else {
        AsnReadVal (aip, atp, NULL);
      }
      if (dsp->title != NULL && dsp->uid == uid) {
        if (backbone) {
          if (author [0] != '\0' && year [0] != '\0') {
            goOn = FALSE;
          }
        } else if (genBank || embl || ddbj) {
          if (locus [0] != '\0') {
            if (isaAA && cds [0] != '\0') {
              goOn = FALSE;
            } else if (isaNA) {
              goOn = FALSE;
            }
          }
        } else if (pir) {
          if (locus [0] != '\0') {
            goOn = FALSE;
          }
        } else if (swissprot) {
          if (locus [0] != '\0') {
            goOn = FALSE;
          }
        } else if (embl) {
        }
      }
    }
    if (backbone) {
      chptr = StringChr (author, ',');
      if (chptr != NULL) {
        *chptr = '\0';
      }
      chptr = StringChr (year, ' ');
      if (chptr != NULL) {
        *chptr = '\0';
      }
      author [12] = '.';
      author [12] = '\0';
      sprintf (caption, "%s, %s", author, year);
      dsp->caption = StringSave (caption);
    } else if (genBank || embl || ddbj) {
      if (isaAA) {
        sprintf (caption, "%s%s", locus, cds);
      } else if (isaSEG) {
        sprintf (caption, "%s segs", locus);
      } else {
        sprintf (caption, "%s", locus);
      }
      dsp->caption = StringSave (caption);
    } else {
      sprintf (caption, "%s", locus);
      dsp->caption = StringSave (caption);
    }
    dsp->uid = uid;
    if (recentTitle != NULL) {
      recentTitle = MemFree (recentTitle);
    }
  }
  AsnIoReset (aip);
  return dsp;
}

/*****************************************************************************
*
*   CdEntrezFindSeqId(sip)
*       given a Seq-id, get the uid.
*       returns 0 on failure
*
*****************************************************************************/
NLM_EXTERN Int4 CdEntrezFindSeqId (SeqIdPtr sip)
{
    Int4 uid = 0;
    DocType db = -1;
    TextSeqIdPtr tsip;
    PDBSeqIdPtr psip;
    PatentSeqIdPtr patsip;
    CharPtr locus = NULL;
    Char localbuf[40];
    ValNodePtr lst;
    LinkSetPtr lsp;
    Boolean check_both, done;
    EntrezInfoPtr eip;
    Int4 index;

    if ((eip = CdEntrezGetInfo()) != NULL && eip->field_count > FLD_SQID &&
	eip->types[TYP_NT].fields[FLD_SQID].num_terms > 0)
    {
        done = FALSE;
        check_both = TRUE;
	db = TYP_NT;
	SeqIdWrite(sip, localbuf, PRINTID_FASTA_LONG, sizeof(localbuf));
        while (! done)     /* might need to check 2 types */
        {
            lst = CdEntTLNew(db);
            if (lst == NULL) return uid;
            CdEntTLAddTerm(lst, localbuf, db, FLD_SQID, TRUE, NULL);
            lsp = CdEntTLEval(lst);
            CdEntTLFree(lst);
            if (lsp != NULL)
            {
                for (index = 0; index < lsp->num; index++)
		{ /* choose the highest one */
		    if (lsp->uids[index] > uid)
                        uid = lsp->uids[index];
		}
                LinkSetFree(lsp);
            }
            if ((! check_both) || (uid > 0))
                done = TRUE;
            else
            {
                if (db == TYP_AA)
                    db = TYP_NT;
                else
                    db = TYP_AA;
                check_both = FALSE;
            }
        }
    }

    if (uid > 0)
    {
	return uid;
    }

    check_both = FALSE;
    switch (sip->choice)
    {
        case SEQID_NOT_SET:           /* not set */
        case SEQID_LOCAL:           /* local */
            break;
        case SEQID_GIBBSQ:           /* gibbsq */
        case SEQID_GIBBMT:           /* gibbmt */
            sprintf(localbuf, "B%ld", (long)(sip->data.intvalue));
            locus = (CharPtr)localbuf;
            db = TYP_AA;   /* guess it's a protein */
            check_both = TRUE;
            break;             /* not on cdrom */
        case SEQID_GIIM:           /* giim */
            uid = ((GiimPtr)sip->data.ptrvalue)->id;
            break;
        case SEQID_GI:
            uid = sip->data.intvalue;
            break;
        case SEQID_GENBANK:             /* genbank */
        case SEQID_EMBL:                /* embl */
        case SEQID_DDBJ:
            db = TYP_NT;   /* guess it's a nucleic acid */
            check_both = TRUE;
        case SEQID_PIR:             /* pir */
        case SEQID_SWISSPROT:
        case SEQID_PRF:
            if (db < 0)
                db = TYP_AA;
            tsip = (TextSeqIdPtr)sip->data.ptrvalue;
            if (tsip->accession != NULL)
                locus = tsip->accession;
            else
                locus = tsip->name;
            break;
        case SEQID_PDB:
            psip = (PDBSeqIdPtr)(sip->data.ptrvalue);
            if (psip->chain == '\0' || psip->chain == ' ')
                StrCpy (localbuf, psip->mol);
	    else
                sprintf(localbuf, "%s-%c", psip->mol, (Char)psip->chain);
            locus = localbuf;
            db = TYP_AA;   /* guess protein */
            check_both = TRUE;
            break;
        case SEQID_PATENT:
            patsip = (PatentSeqIdPtr)(sip->data.ptrvalue);
            sprintf(localbuf, "%s%s %d", patsip->cit->country, patsip->cit->number,
                (int)patsip->seqid);
            locus = localbuf;
            db = TYP_AA;   /* guess protein */
            check_both = TRUE;
            break;
        default:
            break;
    }

    if ((! uid) && (locus != NULL))   /* got a term to find */
    {
        done = FALSE;
        while (! done)     /* might need to check 2 types */
        {
            lst = CdEntTLNew(db);
            if (lst == NULL) return uid;
            CdEntTLAddTerm(lst, locus, db, FLD_ACCN, TRUE, NULL);
            lsp = CdEntTLEval(lst);
            CdEntTLFree(lst);
            if (lsp != NULL)
            {
                for (index = 0; index < lsp->num; index++)
		{ /* choose the highest one */
		    if (lsp->uids[index] > uid)
                        uid = lsp->uids[index];
		}
                LinkSetFree(lsp);
            }
            if ((! check_both) || (uid > 0))
                done = TRUE;
            else
            {
                if (db == TYP_AA)
                    db = TYP_NT;
                else
                    db = TYP_AA;
                check_both = FALSE;
            }
        }
    }

    return uid;
}

#ifdef Biostruc_supported
NLM_EXTERN BiostrucPtr CdEntrezBiostrucGet (DocUid uid, Int4 mdlLvl, Int4 maxModels)
{
	Biostruc *struc = NULL;
	AsnIo *stream = NULL;

	if (! BiostrucAvail ()) return NULL;
	stream = cd3_CdDocAsnOpen(TYP_ST,uid);
	if (stream != NULL)
	{
		struc = BiostrucAsnGet(stream,NULL, mdlLvl, maxModels);
		cd3_CdDocAsnClose(stream);
	}
	return struc;
}


#ifdef OS_UNIX

NLM_EXTERN BiostrucAnnotSetPtr CdEntrezBiostrucAnnotSetGet (DocUid uid)
{
	BiostrucAnnotSetPtr retval = NULL;
	AsnIoPtr  aip;
	FILE *pipe;
	char command[PATH_MAX+5];
	char fname[PATH_MAX];
	
	if (CdMountEntrezVolume(1,fname,PATH_MAX-32))
	{
		sprintf(strchr(fname,0), "/vast/%ld.bas.Z", (long) uid);
		if (FileLength(fname) <= 0)
		{
			return NULL;
		}
		sprintf(command,"zcat %s", fname);
		if ((pipe=popen(command,"r")) ==NULL)
		{
			ErrPostEx(SEV_ERROR,0,0,"Unable to open pipe [%s]",command);
			return NULL;
		}
		aip = AsnIoNew(ASNIO_TEXT_IN, pipe, NULL, NULL, NULL);
		if (aip != NULL)
		{
			retval = BiostrucAnnotSetAsnRead(aip, NULL);
		}
		AsnIoFree(aip,FALSE);
		pclose(pipe);
	}
	return retval;
}

#else

NLM_EXTERN BiostrucAnnotSetPtr CdEntrezBiostrucAnnotSetGet (DocUid uid)
{
	return NULL;
}

#endif


NLM_EXTERN BiostrucAnnotSetPtr LIBCALL CdEntrezBiostrucAnnotSetGetByFid (DocUid mmdbid, Int4 feature_id, Int4 feature_set_id)
{
    BiostrucAnnotSetPtr basp = CdEntrezBiostrucAnnotSetGet (mmdbid);
    BiostrucAnnotSetPtr basp2 = NULL;
    BiostrucFeatureSetPtr pbsfs = NULL;
    BiostrucFeaturePtr pbsf = NULL;

    if (basp == NULL)
	return NULL;
 
    pbsfs = basp->features;
    while (pbsfs)
     {
       if (pbsfs->id == feature_set_id)
        {
          pbsf =  pbsfs->features;
          while(pbsf)
            {
              if (pbsf->id == feature_id)
                {  /* found it */
                     basp2 = BiostrucAnnotSetNew();
     		     basp2->id = basp->id;
    		     basp2->descr = basp->descr; 
    		     basp->descr = NULL;  /* unlink the descr from basp object */
    		     basp2->features = BiostrucFeatureSetNew();
                     basp2->features->id = pbsfs->id;
                     basp2->features->descr = pbsfs->descr;
                     pbsfs->descr = NULL; /* unlink the feature-set descr from basp  object */
                     basp2->features->features = BiostrucFeatureNew();
                     basp2->features->features->id = pbsf->id;
                     basp2->features->features->name = StringSave(pbsf->name);
		     basp2->features->features->type = pbsf->type;
		     basp2->features->features->Property_property = pbsf->Property_property;
		     pbsf->Property_property = NULL; /* unlink the property from basp  object */
		     basp2->features->features->Location_location = pbsf->Location_location;
		     pbsf->Location_location = NULL; /* unlink the location from basp  object */ 
		     BiostrucAnnotSetFree(basp);
                     return basp2;
                }
               pbsf = pbsf->next;
            }
        }
       pbsfs = pbsfs->next;
     }
   
    BiostrucAnnotSetFree(basp);
    return basp2;
}


NLM_EXTERN LinkSetPtr LIBCALL CdEntrezBiostrucFeatIds(DocUid mmdbid, Int2 feature_type, Int4 feature_set_id)
{
    BiostrucAnnotSetPtr basp = CdEntrezBiostrucAnnotSetGet (mmdbid);
    LinkSetPtr retval = NULL;
    Int4Ptr ids = NULL;
    Int4Ptr scores = NULL;
    Int4 count = 0;
    BiostrucFeatureSetPtr pbsfs = NULL;
    BiostrucFeaturePtr pbsf = NULL;
    ChemGraphAlignmentPtr  pcga = NULL;
    
    
    if (basp == NULL)
	return NULL;
    
    /* count the number of features of type feature_type */
    pbsfs = basp->features;
    while (pbsfs)
     {
       if (pbsfs->id == feature_set_id)
        {
          pbsf =  pbsfs->features;
          while(pbsf)
            {
              if (pbsf->type == feature_type)
                { 
		   count++;
                }
               pbsf = pbsf->next;
            }
        }
       pbsfs = pbsfs->next;
     }
    
     /* allocate vectors for ids, scores iff alignment data */
   
    ids = (Int4Ptr) MemNew(sizeof(Int4) * count);
    if (feature_type == 200) /* NCBI alignments */
      scores = (Int4Ptr) MemNew(sizeof(Int4) * count);
    
    count = 0;   
    /* collect the feature-id's and scores  */
    pbsfs = basp->features;
    while (pbsfs)
     {
       if (pbsfs->id == feature_set_id)
        {
          pbsf =  pbsfs->features;
          while(pbsf)
            {
              if (pbsf->type == feature_type)
                {
		  
		   ids[count] = pbsf->id;
		   if (feature_type == 200) /* alignment type id */
		    {
		     pcga = (ChemGraphAlignmentPtr) pbsf->Location_location->data.ptrvalue;
		     scores[count] = pcga->aligndata->vast_mlogp;  /* an Int4 already */
		    }
		   count++;
                }
               pbsf = pbsf->next;
            }  /* while feature */
          retval = LinkSetNew();
          retval->num = count;
          retval->uids = ids;
          retval->weights = scores;
          MemFree(basp);
          return retval;
        }  /* if feature_set_id */
       pbsfs = pbsfs->next;
     }  /* while feature_set */
  MemFree(basp);
  return NULL;
}
#endif /* Biostruc_supported */
