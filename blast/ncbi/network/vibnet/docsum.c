/*   docsum.c
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
* File Name:  docsum.c
*
* Author:  Jonathan Kans, Jonathan Epstein
*
* Version Creation Date:   9/13/96
*
* $Revision: 6.56 $
*
* File Description: 
*
* Modifications:  
* --------------------------------------------------------------------------
* $Log: docsum.c,v $
* Revision 6.56  2003/12/02 20:42:06  kans
* fixed sprintf calls in SetDefaultFailureMessae
*
* Revision 6.55  2002/11/06 21:30:57  ucko
* ChangeViewerLinkData: when extracting a string from vnp->data, use
* ptrvalue rather than intvalue (which may be the wrong width)
*
* Revision 6.54  2002/01/03 16:53:52  kans
* use left, top passed in
*
* Revision 6.53  2001/11/02 12:51:01  kans
* add EntrezDocSum time log if environment variable set on UNIX
*
* Revision 6.52  2001/02/15 18:06:55  kans
* FileToString fixed for PC
*
* Revision 6.51  2000/11/17 22:58:57  kans
* BioseqFindEntityByGi and others clear vn.next, LaunchSequenceViewer clears scope - somehow remains set on PC - and FetchDocSum dynamically allocates str, since pfam records overflowed the 1K buffer
*
* Revision 6.50  2000/08/18 19:53:52  kans
* do not remove file sent to Cn3D on any platform
*
* Revision 6.49  2000/08/17 22:19:16  kans
* launch Cn3D with system command, not Execv, which requires full path
*
* Revision 6.48  2000/08/17 20:32:24  kans
* launch remote Cn3D
*
* Revision 6.47  2000/04/03 21:05:53  lewisg
* fix launch of cn3d from sequin
*
* Revision 6.46  2000/01/04 15:55:50  lewisg
* don't hang on disconnected network and fix memory leak/hang at exit
*
* Revision 6.45  1999/11/02 21:42:23  kans
* genpept view only gets sep for bsp, not for top of entity
*
* Revision 6.44  1999/09/21 17:20:56  kans
* added progress monitor when exporting reports
*
* Revision 6.43  1999/08/25 18:58:52  kans
* Boolean bit flags are now unsigned int - int for AIX, unsigned since document.[ch] already used that style
*
* Revision 6.42  1999/08/04 21:43:52  kans
* tells Cn3D it can use network Entrez (LYG)
*
* Revision 6.41  1999/06/18 19:04:27  kans
* set message level MAX for SeqEntryToFlat
*
* Revision 6.40  1999/03/03 18:26:02  kans
* calls ClearStructures before MakeAModelstruc
*
* Revision 6.39  1999/01/14 19:18:46  kans
* new parameters for Cn3DWin_Entrez
*
* Revision 6.38  1999/01/13 19:26:46  kans
* added RetrieveSimpleSeqs, switched from tracking SeqEntry to SimpleSeq, to allow lower case, avoid Bioseq ID collision
*
* Revision 6.37  1999/01/11 16:25:49  kans
* forgot to add parameter to ReadAsnFastaOrFlatFile
*
* Revision 6.36  1998/11/20 19:01:06  kans
* set refine button count if setting retrieve button to evaluate
*
* Revision 6.35  1998/11/18 16:21:29  kans
* added UseDelayedNeighbor to control from preference
*
* Revision 6.34  1998/11/18 15:55:06  kans
* added controls for user to change immediate/delayed neighbor mode
*
* Revision 6.33  1998/11/18 15:14:02  kans
* added retrieveMode to Evaluate neighbors/links only on command
*
* Revision 6.32  1998/08/14 16:23:01  kans
* AddBlastAlignment now passed proper query bioseq
*
* Revision 6.31  1998/06/12 00:00:31  kans
* now calling CleanupEntrezPrefsProc, other minor fixes
*
* Revision 6.30  1998/05/11 21:48:10  kans
* calls GetIdStringsForSeqEntryAligns for count to avoid database hit until retrieve
*
* Revision 6.29  1998/05/07 01:38:56  kans
* retrieve generation hides fasta format control of instantiated uid-less bioseqs
*
* Revision 6.28  1998/05/05 15:13:02  kans
* added lgp->onlyFromThis, control (hidden)
*
* Revision 6.27  1998/05/02 23:09:56  kans
* calls SeqnSeqEntrysToFasta to trim long deflines
*
* Revision 6.26  1998/04/29 15:15:57  kans
* double click only if uids present
*
* Revision 6.25  1998/04/23 12:48:08  kans
* read asn/fasta bioseq collection tries to use sep already attached to bsp
*
* Revision 6.24  1998/04/22 18:07:47  kans
* docsum import label says FASTA or Uid List
*
* Revision 6.23  1998/04/22 17:35:14  kans
* docsum import collects bioseqs into a project, refine button disabled if 0 items
*
* Revision 6.22  1998/04/21 12:56:00  kans
* support for blast choices in database alist, neighbor/link control for blasting a record that already has a uid
*
* Revision 6.21  1998/04/19 23:49:29  kans
* added parseFastaSeqId parameter to ReadAsnFastaOrFlatFile
*
* Revision 6.20  1998/04/19 23:28:30  kans
* initial implementation of import docsum function
*
* Revision 6.19  1998/04/19 22:56:11  kans
* cast for ValNodeLink in building project
*
* Revision 6.18  1998/04/19 22:08:42  kans
* nuc, prot flags to DocSumCanSaveFasta, ExportDocSumFasta
*
* Revision 6.17  1998/04/18 02:38:32  kans
* export docsum fasta does not add newlines, for consistency with export bioseq view fasta
*
* Revision 6.16  1998/04/18 02:08:11  kans
* added DocSumCanSaveFasta and ExportDocSumFasta
*
* Revision 6.15  1998/04/17 16:44:36  kans
* empty function for ImportDocSumForm, implemented ExportDocSumForm
*
* Revision 6.14  1998/04/16 19:16:27  kans
* fasta format sets group_segs to handle seg, delta sequences
*
* Revision 6.13  1998/04/14 21:46:48  kans
* added FetchUid, xxx ID radio button choices
*
* Revision 6.12  1998/04/14 18:13:45  kans
* SetDefaultFailureMessage for docsum items
*
* Revision 6.11  1998/04/13 19:26:54  kans
* forgot to NULL out sfp->seqentry when MemFreeing it
*
* Revision 6.10  1998/04/13 16:47:16  kans
* implemented SetDocSumImportExportItems
*
* Revision 6.9  1998/04/11 16:20:03  kans
* can now read uid or sep project, save sep project
*
* Revision 6.8  1998/04/11 00:47:00  kans
* initial work on handling instantiated bioseqs as projects
*
* Revision 6.7  1998/03/25 15:25:33  kans
* use >PubMed instead of >MEDLINE
*
* Revision 6.6  1998/03/01 22:04:49  kans
* blast wordsize 10 for 16-bit Windows
*
* Revision 6.5  1998/02/21 22:11:46  kans
* implement copy to clipboard
*
* Revision 6.4  1998/01/07 19:11:47  kans
* added UpdateViewerLinkTarget
*
* Revision 6.3  1997/12/03 22:47:30  kans
* AddBlastAlignment returns if no alignments to do
*
* Revision 6.2  1997/11/01 22:58:36  kans
* added alignDefault preference control
*
* Revision 6.1  1997/09/22 18:13:45  kans
* removed obsolete explosion mode
*
* Revision 6.0  1997/08/25 18:48:42  madden
* Revision changed to 6.0
*
* Revision 1.20  1997/08/12 18:43:36  kans
* calls SaveDocumentItem if docsums checked
*
* Revision 1.19  1997/07/23 18:09:09  kans
* DS and TL specific Save/LoadUidProcList functions
*
* Revision 1.18  1997/07/22 20:05:43  kans
* new parameter to BLASTSetUpSearch
*
* Revision 1.17  1997/07/21 14:25:58  kans
* load/save uid list now uses text database tag
*
* Revision 1.16  1997/06/27 17:07:05  kans
* do not limit FileToString to 32K
*
* Revision 1.15  1997/06/01 23:25:07  kans
* flat files use displayFont
*
* Revision 1.14  1997/04/29 18:47:15  kans
* uses persistDefault field if parentsPersist not yet set (PC or UNIX)
*
 * Revision 1.13  1997/04/21  15:36:35  kans
 * added LoadUidListProc and SaveUidListProc
 *
 * Revision 1.12  1997/03/31  18:54:00  kans
 * watch cursor/arrow cursor calls flank export and print docsum
 *
 * Revision 1.11  1997/03/20  21:17:44  kans
 * change display font should send redraw message on all platforms
 *
 * Revision 1.10  1997/03/20  19:05:10  vakatov
 * [!WIN16] Included <cn3dentr.h>; now use Cn3DWin_Entrez() instead of Cn3DWin()
 * In FetchPDB() and LaunchStructureViewer(), check for BiostrucAvail() [J.K.]
 *
*
* ==========================================================================
*/

#include <vibrant.h>
#include <document.h>
#include <accentr.h>
#include <objmgr.h>
#include <gather.h>
#include <asn2ff.h>
#include <tomedlin.h>
#include <tofasta.h>
#include <simple.h>
#include <dlogutil.h>
#include <objproj.h>
#include <mmdbapi.h>
#ifndef WIN16
#include <cn3dmain.h>
#include <cn3dopen.h>  /* added by lyg */
#include <algorend.h>
#include <cn3dentr.h>
#include <udviewer.h>
#include <ddvopen.h>
#endif

#include <blast.h>
#include <blastpri.h>
#include <simutil.h>

#include <bspview.h>
#include <medview.h>

#include <entrez.h>

#ifndef MAX_MDLNO
#define MAX_MDLNO 1000
#endif

#define MAX_DBS 6

#define FETCH_MODE        1
#define EVAL_MODE         2

typedef struct docsumstatedata {
  unsigned int       checked     : 1;
  unsigned int       hasAbstract : 1;
  unsigned int       noSuchUid   : 1;
} StateData, PNTR StateDataPtr;

typedef struct summformdata {
  FORM_MESSAGE_BLOCK

  PopuP              target;
  DoC                docsum;
  GrouP              formats [MAX_DBS + 1];
  CharPtr            label;
  GrouP              controls;

  Int2               dsClickItem;
  Int2               dsClickRow;
  Int2               dsClickCol;
  Boolean            wasDoubleClick;

  Boolean            usingDelay;
  Int2               retrieveMode;

  ButtoN             retrieve;
  ButtoN             refine;

  EnumFieldAssocPtr  dbalist;

  Int4Ptr            uids;
  SimpleSeqPtr PNTR  simple;
  StateDataPtr       state;
  Int2               numUids;
  Int2               numParents;

  Int4Ptr            neighbors;
  Int2               numNeighbors;
  Int2               neighborDb;
  Int2               currDb;

  Int2               lineHeight;
  Int2               linesPerIcon;

  CharPtr            historyFile;
  Int4Ptr            historyOffsets;
  Int2               generations;
  Int2               present;

  ButtoN             nextBtn;
  ButtoN             prevBtn;

  FonT               docsumFont;
} SummFormData, PNTR SummFormPtr;

static Uint1  hasabstract [] = {
  0x07, 0xF8, 0x1F, 0xE0, 0x78, 0x07, 0xE0, 0x1E,
  0x40, 0x01, 0x80, 0x02, 0x40, 0x01, 0x80, 0x02,
  0x40, 0x01, 0x80, 0x02, 0x4F, 0xF9, 0x9F, 0xF2,
  0x40, 0x01, 0x80, 0x02, 0x4F, 0xF9, 0x9F, 0xF2,
  0x40, 0x01, 0x80, 0x02, 0x4F, 0xF9, 0x9F, 0xF2,
  0x40, 0x01, 0x80, 0x02, 0x4F, 0xF9, 0x9F, 0xF2,
  0x40, 0x01, 0x80, 0x02, 0x4F, 0xF9, 0x9F, 0xF2,
  0x40, 0x01, 0x80, 0x02, 0x4F, 0xF9, 0x9F, 0xF2,
  0x40, 0x01, 0x80, 0x02, 0x40, 0x01, 0x80, 0x02,
  0x40, 0x01, 0x80, 0x02, 0x47, 0xF9, 0x9F, 0xE2,
  0x7F, 0xFF, 0xFF, 0xFE, 0x7F, 0xFF, 0xFF, 0xFE
};

static Uint1  noabstract [] = {
  0x07, 0xF8, 0x1F, 0xE0, 0x78, 0x07, 0xE0, 0x1E,
  0x40, 0x01, 0x80, 0x02, 0x40, 0x01, 0x80, 0x02,
  0x40, 0x01, 0x80, 0x02, 0x40, 0x01, 0x80, 0x02,
  0x40, 0x01, 0x80, 0x02, 0x40, 0x01, 0x80, 0x02,
  0x40, 0x01, 0x80, 0x02, 0x40, 0x01, 0x80, 0x02,
  0x40, 0x01, 0x80, 0x02, 0x40, 0x01, 0x80, 0x02,
  0x40, 0x01, 0x80, 0x02, 0x40, 0x01, 0x80, 0x02,
  0x40, 0x01, 0x80, 0x02, 0x40, 0x01, 0x80, 0x02,
  0x40, 0x01, 0x80, 0x02, 0x40, 0x01, 0x80, 0x02,
  0x40, 0x01, 0x80, 0x02, 0x47, 0xF9, 0x9F, 0xE2,
  0x7F, 0xFF, 0xFF, 0xFE, 0x7F, 0xFF, 0xFF, 0xFE
};

static Uint1  proteinicon [] = {
  0x7F, 0xFF, 0xFF, 0xFE, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x43, 0x83, 0x80, 0x02,
  0x44, 0x44, 0x40, 0x02, 0x43, 0xC3, 0xC0, 0x02,
  0x44, 0x44, 0x40, 0x02, 0x44, 0x44, 0x40, 0x02,
  0x44, 0x44, 0x40, 0x02, 0x43, 0xE3, 0xE0, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x47, 0xE7, 0xE7, 0xE2,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x47, 0xE7, 0xE7, 0xE2, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x47, 0xE7, 0xE7, 0xE2,
  0x40, 0x00, 0x00, 0x02, 0x7F, 0xFF, 0xFF, 0xFE
};

static Uint1  dnaicon [] = {
  0x7F, 0xFF, 0xFF, 0xFE, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x45, 0x81, 0x00, 0x02,
  0x46, 0x41, 0x00, 0x02, 0x44, 0x47, 0xC0, 0x02,
  0x44, 0x41, 0x00, 0x02, 0x44, 0x41, 0x00, 0x02,
  0x44, 0x41, 0x00, 0x02, 0x44, 0x60, 0xC0, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x4F, 0x7B, 0xDE, 0xF2,
  0x40, 0x00, 0x00, 0x02, 0x4F, 0x7B, 0xDE, 0xF2,
  0x40, 0x00, 0x00, 0x02, 0x4F, 0x7B, 0xDE, 0xF2,
  0x40, 0x00, 0x00, 0x02, 0x4F, 0x7B, 0xDE, 0xF2,
  0x40, 0x00, 0x00, 0x02, 0x7F, 0xFF, 0xFF, 0xFE
};

static Uint1  threedicon [] = {
  0x7F, 0xFF, 0xFF, 0xFE, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x4E, 0x0E, 0x00, 0x02,
  0x41, 0x09, 0x00, 0x02, 0x40, 0x88, 0x80, 0x02,
  0x47, 0x08, 0x80, 0x02, 0x40, 0x88, 0x80, 0x02,
  0x41, 0x09, 0x00, 0x02, 0x4E, 0x0E, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x47, 0xE0, 0x00, 0x02,
  0x40, 0x07, 0xE0, 0x02, 0x40, 0x00, 0x07, 0xE2,
  0x47, 0xE0, 0x00, 0x02, 0x40, 0x07, 0xE0, 0x02,
  0x40, 0x00, 0x07, 0xE2, 0x47, 0xE0, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x7F, 0xFF, 0xFF, 0xFE
};

static Uint1  genomeicon [] = {
  0x7F, 0xFF, 0xFF, 0xFE, 0x40, 0x00, 0x00, 0x02,
  0x40, 0xF0, 0xF0, 0x02, 0x41, 0x01, 0x08, 0x02,
  0x41, 0x01, 0x00, 0x02, 0x41, 0x01, 0x00, 0x02,
  0x41, 0x01, 0x38, 0x02, 0x41, 0x01, 0x10, 0x02,
  0x40, 0xF0, 0xE0, 0x02, 0x40, 0x00, 0x00, 0x02,
#ifdef USE_CHROMOSOME_BANDS_IN_ICON
/* chromosome bands */
  0x40, 0x00, 0x00, 0x02, 0x4F, 0xEF, 0xFF, 0xF2,
  0x50, 0x10, 0xC4, 0xEA, 0x50, 0x10, 0xC4, 0xEA,
  0x50, 0x10, 0xC4, 0xEA, 0x50, 0x10, 0xC4, 0xEA,
  0x50, 0x10, 0xC4, 0xEA, 0x50, 0x10, 0xC4, 0xEA,
  0x50, 0x10, 0xC4, 0xEA, 0x4F, 0xEF, 0xFF, 0xF2,
  0x40, 0x00, 0x00, 0x02, 0x7F, 0xFF, 0xFF, 0xFE
#else
/* show alignment */
  0x40, 0x00, 0x00, 0x02, 0x4F, 0xFF, 0xFF, 0xF2,
  0x48, 0x00, 0x00, 0x12, 0x4F, 0xFF, 0xFF, 0xF2,
  0x40, 0x00, 0x00, 0x02, 0x40, 0xE0, 0xFF, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x4F, 0x0F, 0x80, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x7F, 0xFF, 0xFF, 0xFE
#endif
};

static Uint1  checkmark [] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40,
  0x00, 0x80, 0x01, 0x00, 0x22, 0x00, 0x14, 0x00,
  0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static Uint1Ptr  iconLists [] = {
  noabstract, hasabstract, proteinicon, proteinicon, dnaicon,
  dnaicon, threedicon, threedicon, genomeicon, genomeicon
};

static ParData docsumParFmt = {TRUE, FALSE, FALSE, FALSE, FALSE, 0, 0};
static ColData docsumColFmt [] = {
  {0, 0, 15, 0, NULL, 'l', TRUE, FALSE, FALSE, FALSE, FALSE},  /* caption  */
  {0, 5, 65, 0, NULL, 'l', TRUE, FALSE, FALSE, FALSE, TRUE}    /* term     */
};
static ColData textColFmt [] = {
  {0, 0, 80, 0, NULL, 'l', FALSE, FALSE, FALSE, FALSE, TRUE}    /* text     */
};

static void SetDocSumImportExportItems (SummFormPtr sfp);

static BioseqPtr BioseqLockByGi (Int4 uid)

{
  ValNode  vn;

  if (uid <= 0) return NULL;
  vn.choice = SEQID_GI;
  vn.data.intvalue = uid;
  vn.next = NULL;
  return BioseqLockById (&vn);
}

static Boolean BioseqUnlockByGi (Int4 uid)

{
  ValNode  vn;

  if (uid <= 0) return FALSE;
  vn.choice = SEQID_GI;
  vn.data.intvalue = uid;
  vn.next = NULL;
  return BioseqUnlockById (&vn);
}

static SeqEntryPtr SeqEntryLockByGi (Int4 uid)

{
  BioseqPtr    bsp;
  Uint2        entityID;
  SeqEntryPtr  sep;

  bsp = BioseqLockByGi (uid);
  if (bsp == NULL) return NULL;
  entityID = ObjMgrGetEntityIDForPointer (bsp);
  if (entityID == 0) return NULL;
  sep = GetTopSeqEntryForEntityID (entityID);
  return sep;
}

static Uint2 BioseqFindEntityByGi (Int4 uid, Uint2Ptr itemIDptr)

{
  ValNode  vn;

  if (uid <= 0) return 0;
  vn.choice = SEQID_GI;
  vn.data.intvalue = uid;
  vn.next = NULL;
  return BioseqFindEntity (&vn, itemIDptr);
}

static void DoDrawCheck (SummFormPtr sfp, RectPtr r, Int2 item, Int2 frst, Boolean docsum)

{
  Int2  checked;
  RecT  rct;

  if (sfp == NULL || sfp->state == NULL) return;
  if (item < 1) return;
  if (item > sfp->numUids) return;
  checked = sfp->state [item - 1].checked;
  if (frst == 0) {
    rct = *r;
    rct.right = rct.left + sfp->lineHeight;
    rct.bottom = rct.top + sfp->lineHeight;
    if (RectInRgn (&rct, updateRgn)) {
      FrameRect (&rct);
      if (checked) {
        MoveTo (rct.left, rct.top);
        LineTo (rct.right - 1, rct.bottom - 1);
        MoveTo (rct.left, rct.bottom - 1);
        LineTo (rct.right - 1, rct.top);
      }
    }
    if (item <= sfp->numParents) {
      rct = *r;
      if (docsum) {
        rct.left += docsumColFmt [0].pixInset - 6;
        rct.top += (sfp->lineHeight - 4) / 2;
        rct.right = rct.left + 4;
        rct.bottom = rct.top + 4;
      } else {
        rct.left += textColFmt [0].pixInset - 6;
        rct.top += (sfp->lineHeight - 4) / 2;
        rct.right = rct.left + 4;
        rct.bottom = rct.top + 4;
      }
      if (RectInRgn (&rct, updateRgn)) {
        PaintOval (&rct);
      }
    }
  }
}

static void DrawUidCheck (SummFormPtr sfp, RectPtr r, Int2 item, Int2 frst, Boolean docsum)

{
  Int4  uid;

  if (sfp == NULL || sfp->uids == NULL || sfp->state == NULL) return;
  if (item < 1) return;
  if (item > sfp->numUids) return;
  uid = sfp->uids [item - 1];
  if (uid < 1) return;
  DoDrawCheck (sfp, r, item, frst, docsum);
}

static void DrawLocalCheck (SummFormPtr sfp, RectPtr r, Int2 item, Int2 frst, Boolean docsum)

{
  SimpleSeqPtr  ssp;

  if (sfp == NULL || sfp->simple == NULL || sfp->state == NULL) return;
  if (item < 1) return;
  if (item > sfp->numUids) return;
  ssp = sfp->simple [item - 1];
  if (ssp == NULL) return;
  DoDrawCheck (sfp, r, item, frst, docsum);
}

static void DrawIcon (SummFormPtr sfp, RectPtr r, Int2 item, Int2 frst)

{
  Int2      db;
  Int2      hasAbstract;
  Uint1Ptr  icon;
  Int2      index;
  RecT      rct;
  Int4      uid;

  if (sfp == NULL || sfp->uids == NULL || sfp->state == NULL) return;
  if (item < 1) return;
  if (item > sfp->numUids) return;
  db = sfp->currDb;
  uid = sfp->uids [item - 1];
  hasAbstract = sfp->state [item - 1].hasAbstract;
  if (uid < 1) return;
  if (frst < sfp->linesPerIcon) {
    if (db >= 0 && db <= 4) {
      index = db * 2;
      if (hasAbstract) {
        index++;
      }
      rct = *r;
      /*
      rct.left += sfp->lineHeight + 4;
      */
      rct.left += 3 * stdCharWidth - 8;
      rct.right = rct.left + 32;
      rct.bottom = MIN (rct.bottom, rct.top + 22 - frst * sfp->lineHeight);
      if (RectInRgn (&rct, updateRgn)) {
        icon = iconLists [index];
        if (icon != NULL) {
          CopyBits (&rct, icon + 4 * frst * sfp->lineHeight);
        }
      }
    }
  }
}

static void DrawDocSum (DoC d, RectPtr r, Int2 item, Int2 frst)

{
  SummFormPtr  sfp;

  sfp = (SummFormPtr) GetObjectExtra (d);
  DrawUidCheck (sfp, r, item, frst, TRUE);
  DrawIcon (sfp, r, item, frst);
}

static void DrawTextSum (DoC d, RectPtr r, Int2 item, Int2 frst)

{
  SummFormPtr  sfp;

  sfp = (SummFormPtr) GetObjectExtra (d);
  DrawUidCheck (sfp, r, item, frst, FALSE);
}

static void DrawLocalSum (DoC d, RectPtr r, Int2 item, Int2 frst)

{
  SummFormPtr  sfp;

  sfp = (SummFormPtr) GetObjectExtra (d);
  DrawLocalCheck (sfp, r, item, frst, FALSE);
}

static CharPtr SetDefaultFailureMessage (SummFormPtr sfp, Int2 item, CharPtr prefix)

{
  Char          buf [40];
  CharPtr       dbname = NULL;
  SimpleSeqPtr  ssp;
  Char          tmp [64];
  Int4          uid;

  tmp [0] = '\0';
  if (prefix == NULL) {
    prefix = "";
  }
  if (sfp == NULL) return StringSave ("?");
  if (sfp->currDb >= 0 && sfp->currDb < MAX_DBS) {
    dbname = GetEnumName ((UIEnum) sfp->currDb, sfp->dbalist);
  }
  if (dbname == NULL) {
    dbname = "[?]";
  }
  if (item < 1) {
    sprintf (tmp, "%sItem < 1 for database %s", prefix, dbname);
  } else if (sfp->uids != NULL && sfp->simple == NULL) {
    uid = sfp->uids [item - 1];
    if (uid < 1) {
      sprintf (tmp, "%sID < 1 for database %s", prefix, dbname);
    } else {
      sprintf (tmp, "%sFailure for ID %ld in %s database", prefix, (long) uid, dbname);
    }
  } else if (sfp->simple != NULL && sfp->uids == NULL) {
    ssp = sfp->simple [item - 1];
    if (ssp == NULL) {
      sprintf (tmp, "%sIncorrect seq-entry for database %s", prefix, dbname);
    } else {
      StringCpy (buf, "?");
      if (ssp->numid > 0 && ssp->bestid < ssp->numid) {
        StringNCpy_0 (buf, ssp->id [ssp->bestid], sizeof (buf));
      }
      sprintf (tmp, "%sProblem with sequence %s in %s database", prefix, buf, dbname);
    }
  } else {
    sprintf (tmp, "%sInternal confusion for database %s", prefix, dbname);
  }
  return StringSave (tmp);
}

static CharPtr FetchDocSum (DoC d, Int2 item, Pointer ptr)

{
  DocSumPtr    dsp;
  CharPtr      failed;
  size_t       len;
  Int2         j;
  SummFormPtr  sfp;
  CharPtr      str;
  Int4         uid;
#ifdef OS_UNIX
  Boolean      logtimes;
  Int4         starttime;
  Int4         stoptime;
#endif

  sfp = (SummFormPtr) GetObjectExtra (d);
  failed = SetDefaultFailureMessage (sfp, item, "\r\t");
  if (sfp == NULL || sfp->uids == NULL || sfp->state == NULL) return failed;
  if (item < 1) return failed;
  uid = sfp->uids [item - 1];

#ifdef OS_UNIX
  logtimes = (Boolean) ((getenv ("NCBI_LOG_SYNC_QUERY_TIMES")) != NULL);
  if (logtimes) {
    starttime = ComputerTime ();
  }
#endif

  dsp = EntrezDocSum (sfp->currDb, uid);
  if (dsp == NULL) return failed;

#ifdef OS_UNIX
  if (logtimes) {
    stoptime = ComputerTime ();
    printf ("EntrezDocSum %ld\n", (long) (stoptime - starttime));
  }
#endif

  if (dsp->caption == NULL && dsp->title == NULL && dsp->extra == NULL) {
    DocSumFree (dsp);
    return failed;
  }
  MemFree (failed);
  len = StringLen (dsp->caption) + StringLen (dsp->title) + StringLen (dsp->extra) + 20;
  str = MemNew (sizeof (Char) * len);
  if (str == NULL) return failed;
  if (dsp->caption != NULL) {
    StringCat (str, dsp->caption);
  }
  j = 0;
  while (str [j] != '\0' && str [j] != ',') {
    j++;
  }
  if (str [j] == ',' && str [j + 1] == ' ') {
    str [j + 1] = '\r';
  }
  StringCat (str, "\r");
  StringCat (str, "\t");
  if (dsp->title != NULL || dsp->extra != NULL) {
    if (dsp->title != NULL) {
      j = 0;
      while (dsp->title [j] != '\0') {
        if (dsp->title [j] == '\n' || dsp->title [j] == '\r') {
          dsp->title [j] = ' ';
        }
        j++;
      }
      StringCat (str, dsp->title);
    }
    if (dsp->extra != NULL) {
      StringCat (str, "\r");
      j = 0;
      while (dsp->extra [j] != '\0') {
        if (dsp->extra [j] == '\n' || dsp->extra [j] == '\r') {
          dsp->extra [j] = ' ';
        }
        j++;
      }
        StringCat (str, dsp->extra);
    }
  } else {
    StringCat (str, "?");
  }
  StringCat (str, "\n");
  sfp->state [item - 1].hasAbstract = (Boolean) (! dsp->no_abstract);
  DocSumFree (dsp);
  return /* StringSave (str) */ str;
}

static CharPtr FetchUid (DoC d, Int2 item, Pointer ptr)

{
  CharPtr      failed;
  SummFormPtr  sfp;
  Char         str [64];
  Int4         uid;

  sfp = (SummFormPtr) GetObjectExtra (d);
  failed = SetDefaultFailureMessage (sfp, item, NULL);
  if (sfp == NULL || sfp->uids == NULL || sfp->state == NULL) return failed;
  if (item < 1) return failed;
  uid = sfp->uids [item - 1];
  if (uid < 1) return failed;
  MemFree (failed);
  sprintf (str, "%ld\n", (long) uid);
  return StringSave (str);
}

static CharPtr FileToString (CharPtr path)

{
  Int2     actual;
  FILE     *fp;
  Int4     len;
  CharPtr  ptr;

  if (path == NULL) return NULL;
  ptr = NULL;
  len = FileLength (path);
  if (len > 0 && len < MAXALLOC /* && len < 32760 */) {
    fp = FileOpen (path, "r");
    if (fp != NULL) {
      ptr = MemNew (sizeof (Char) * (size_t) (len + 4));
      if (ptr != NULL) {
        actual = FileRead (ptr, 1, (size_t) len, fp);
        if (actual > 0 && actual <= len) {
          /*
          ptr [actual] = '\n';
          ptr [actual + 1] = '\0';
          */
          ptr [actual] = '\0';
        }
      }
      FileClose (fp);
    }
  }
  return ptr;
}

static CharPtr FetchAbstract (DoC d, Int2 item, Pointer ptr)

{
  CharPtr          failed;
  FILE             *fp;
  MedlineEntryPtr  mep;
  Char             path [PATH_MAX];
  SummFormPtr      sfp;
  CharPtr          str;
  Int4             uid;

  sfp = (SummFormPtr) GetObjectExtra (d);
  failed = SetDefaultFailureMessage (sfp, item, NULL);
  if (sfp == NULL || sfp->uids == NULL || sfp->state == NULL) return failed;
  if (item < 1) return failed;
  uid = sfp->uids [item - 1];
  mep = EntrezMedlineEntryGet (uid);
  if (mep == NULL) return failed;
  str = NULL;
  TmpNam (path);
  fp = FileOpen (path, "w");
  if (fp != NULL) {
    if (MedlineEntryToAbsFile (mep, fp)) {
      FileClose (fp);
      str = FileToString (path);
    } else {
      FileClose (fp);
    }
  }
  FileRemove (path);
  MedlineEntryFree (mep);
  if (str == NULL) return failed;
  MemFree (failed);
  return str;
}

static CharPtr FetchCitation (DoC d, Int2 item, Pointer ptr)

{
  CharPtr          failed;
  FILE             *fp;
  MedlineEntryPtr  mep;
  Char             path [PATH_MAX];
  SummFormPtr      sfp;
  CharPtr          str;
  Int4             uid;

  sfp = (SummFormPtr) GetObjectExtra (d);
  failed = SetDefaultFailureMessage (sfp, item, NULL);
  if (sfp == NULL || sfp->uids == NULL || sfp->state == NULL) return failed;
  if (item < 1) return failed;
  uid = sfp->uids [item - 1];
  mep = EntrezMedlineEntryGet (uid);
  if (mep == NULL) return failed;
  str = NULL;
  TmpNam (path);
  fp = FileOpen (path, "w");
  if (fp != NULL) {
    if (MedlineEntryToDocFile (mep, fp)) {
      FileClose (fp);
      str = FileToString (path);
    } else {
      FileClose (fp);
    }
  }
  FileRemove (path);
  MedlineEntryFree (mep);
  if (str == NULL) return failed;
  MemFree (failed);
  return str;
}

static CharPtr FetchMedline (DoC d, Int2 item, Pointer ptr)

{
  CharPtr          failed;
  FILE             *fp;
  MedlineEntryPtr  mep;
  Char             path [PATH_MAX];
  SummFormPtr      sfp;
  CharPtr          str;
  Int4             uid;

  sfp = (SummFormPtr) GetObjectExtra (d);
  failed = SetDefaultFailureMessage (sfp, item, NULL);
  if (sfp == NULL || sfp->uids == NULL || sfp->state == NULL) return failed;
  if (item < 1) return failed;
  uid = sfp->uids [item - 1];
  mep = EntrezMedlineEntryGet (uid);
  if (mep == NULL) return failed;
  str = NULL;
  TmpNam (path);
  fp = FileOpen (path, "w");
  if (fp != NULL) {
    if (MedlineEntryToDataFile (mep, fp)) {
      FileClose (fp);
      str = FileToString (path);
    } else {
      FileClose (fp);
    }
  }
  FileRemove (path);
  MedlineEntryFree (mep);
  if (str == NULL) return failed;
  MemFree (failed);
  return str;
}

static CharPtr FetchGenBank (DoC d, Int2 item, Pointer ptr)

{
  CharPtr      failed;
  FILE         *fp;
  ErrSev       level;
  Char         path [PATH_MAX];
  SeqEntryPtr  sep;
  SummFormPtr  sfp;
  CharPtr      str;
  Int4         uid;

  sfp = (SummFormPtr) GetObjectExtra (d);
  failed = SetDefaultFailureMessage (sfp, item, NULL);
  if (sfp == NULL || sfp->uids == NULL || sfp->state == NULL) return failed;
  if (item < 1) return failed;
  uid = sfp->uids [item - 1];
  /*
  retcode = GetSequenceComplexity (sfp);
  sep = EntrezSeqEntryGet (uid, retcode);
  */
  sep = SeqEntryLockByGi (uid);
  if (sep == NULL) return failed;
  str = NULL;
  TmpNam (path);
  fp = FileOpen (path, "w");
  if (fp != NULL) {
    level = ErrSetMessageLevel (SEV_MAX);
    if (SeqEntryToFlat (sep, fp, GENBANK_FMT, RELEASE_MODE)) {
      FileClose (fp);
      str = FileToString (path);
    } else {
      FileClose (fp);
    }
    ErrSetMessageLevel (level);
  }
  FileRemove (path);
  BioseqUnlockByGi (uid);
  if (str == NULL) return failed;
  MemFree (failed);
  return str;
}

static CharPtr FetchEmbl (DoC d, Int2 item, Pointer ptr)

{
  CharPtr      failed;
  FILE         *fp;
  ErrSev       level;
  Char         path [PATH_MAX];
  SeqEntryPtr  sep;
  SummFormPtr  sfp;
  CharPtr      str;
  Int4         uid;

  sfp = (SummFormPtr) GetObjectExtra (d);
  failed = SetDefaultFailureMessage (sfp, item, NULL);
  if (sfp == NULL || sfp->uids == NULL || sfp->state == NULL) return failed;
  if (item < 1) return failed;
  uid = sfp->uids [item - 1];
  /*
  retcode = GetSequenceComplexity (sfp);
  sep = EntrezSeqEntryGet (uid, retcode);
  */
  sep = SeqEntryLockByGi (uid);
  if (sep == NULL) return failed;
  str = NULL;
  TmpNam (path);
  fp = FileOpen (path, "w");
  if (fp != NULL) {
    level = ErrSetMessageLevel (SEV_MAX);
    if (SeqEntryToFlat (sep, fp, EMBL_FMT, RELEASE_MODE)) {
      FileClose (fp);
      str = FileToString (path);
    } else {
      FileClose (fp);
    }
    ErrSetMessageLevel (level);
  }
  FileRemove (path);
  BioseqUnlockByGi (uid);
  if (str == NULL) return failed;
  MemFree (failed);
  return str;
}

static CharPtr FetchGenPept (DoC d, Int2 item, Pointer ptr)

{
  BioseqPtr    bsp;
  CharPtr      failed;
  FILE         *fp;
  ErrSev       level;
  Char         path [PATH_MAX];
  SeqEntryPtr  sep;
  SummFormPtr  sfp;
  CharPtr      str;
  Int4         uid;

  sfp = (SummFormPtr) GetObjectExtra (d);
  failed = SetDefaultFailureMessage (sfp, item, NULL);
  if (sfp == NULL || sfp->uids == NULL || sfp->state == NULL) return failed;
  if (item < 1) return failed;
  uid = sfp->uids [item - 1];
  /*
  retcode = GetSequenceComplexity (sfp);
  sep = EntrezSeqEntryGet (uid, retcode);
  */
  bsp = BioseqLockByGi (uid);
  if (bsp == NULL) return failed;
  sep = SeqMgrGetSeqEntryForData (bsp);
  if (sep == NULL) return failed;
  str = NULL;
  TmpNam (path);
  fp = FileOpen (path, "w");
  if (fp != NULL) {
    if (spop == NULL) {
      spop = StdPrintOptionsNew (NULL);
      if (spop != NULL) {
        spop->newline = "\r";
        spop->indent = "";
      } else {
        Message (MSG_ERROR, "StdPrintOptionsNew failed");
      }
    }
    level = ErrSetMessageLevel (SEV_MAX);
    if (SeqEntryToFlat (sep, fp, GENPEPT_FMT, RELEASE_MODE)) {
      FileClose (fp);
      str = FileToString (path);
    } else {
      FileClose (fp);
    }
    ErrSetMessageLevel (level);
  }
  FileRemove (path);
  BioseqUnlockByGi (uid);
  if (str == NULL) return failed;
  MemFree (failed);
  return str;
}

static CharPtr FetchFasta (DoC d, Int2 item, Pointer ptr, Boolean is_na)

{
  BioseqPtr    bsp;
  CharPtr      failed;
  FILE         *fp;
  Uint1        group_segs = 0;
  Char         path [PATH_MAX];
  SeqEntryPtr  sep;
  SummFormPtr  sfp;
  CharPtr      str;
  Int4         uid;

  sfp = (SummFormPtr) GetObjectExtra (d);
  failed = SetDefaultFailureMessage (sfp, item, NULL);
  if (sfp == NULL || sfp->uids == NULL || sfp->state == NULL) return failed;
  if (item < 1) return failed;
  uid = sfp->uids [item - 1];
  /*
  retcode = GetSequenceComplexity (sfp);
  sep = EntrezSeqEntryGet (uid, retcode);
  */
  /*
  sep = SeqEntryLockByGi (uid);
  if (sep == NULL) return failed;
  */
  bsp = BioseqLockByGi (uid);
  if (bsp == NULL) return failed;
  sep = SeqMgrGetSeqEntryForData (bsp);
  str = NULL;
  TmpNam (path);
  fp = FileOpen (path, "w");
  if (fp != NULL) {
    if (bsp->repr == Seq_repr_seg) {
      group_segs = 1;
    } else if (bsp->repr == Seq_repr_delta) {
      group_segs = 3;
    }
    if (SeqEntrysToFasta (sep, fp, is_na, group_segs)) {
      FileClose (fp);
      str = FileToString (path);
    } else {
      FileClose (fp);
    }
  }
  FileRemove (path);
  BioseqUnlockByGi (uid);
  if (str == NULL) return failed;
  MemFree (failed);
  return str;
}

static CharPtr FetchFastaNuc (DoC d, Int2 item, Pointer ptr)

{
  return FetchFasta (d, item, ptr, TRUE);
}

static CharPtr FetchFastaProt (DoC d, Int2 item, Pointer ptr)

{
  return FetchFasta (d, item, ptr, FALSE);
}

static CharPtr FetchPDB (DoC d, Int2 item, Pointer ptr)

{
  BiostrucPtr  bsp;
  Int4         complexity;
  CharPtr      failed;
  FILE         *fp;
  Int2         maxModels;
  Char         path [PATH_MAX];
  PDNMS        pdnms;
  SummFormPtr  sfp;
  CharPtr      str;
  Int4         uid;

  if (! BiostrucAvail ()) {
    return StringSave ("Structure libraries are not linked in.");
  }
  sfp = (SummFormPtr) GetObjectExtra (d);
  failed = SetDefaultFailureMessage (sfp, item, NULL);
  if (sfp == NULL || sfp->uids == NULL || sfp->state == NULL) return failed;
  if (item < 1) return failed;
  uid = sfp->uids [item - 1];
  /* complexity = GetBiostrucComplexity (sfp); */
  complexity = VECMODEL;
  /* maxModels = GetBiostrucMaxModels (sfp); */
  maxModels = 1;
  bsp = EntrezBiostrucGet (uid, complexity, maxModels);
  if (bsp == NULL) return failed;
  ClearStructures ();
  pdnms = MakeAModelstruc (bsp);
  if (pdnms == NULL) return failed;
  str = NULL;
  TmpNam (path);
  fp = FileOpen (path, "w");
  if (fp != NULL) {
    WriteStructSummary (pdnms, fp);
    fprintf (fp, "\n\n\n");
    WritePDBRemarks (pdnms, fp);
    FileClose (fp);
    str = FileToString (path);
  }
  FileRemove (path);
  FreeAModelstruc (pdnms);
  if (str == NULL) return failed;
  MemFree (failed);
  return str;
}

static CharPtr FetchLocalBioseq (DoC d, Int2 item, Pointer ptr)

{
  CharPtr       failed;
  FILE          *fp;
  Char          path [PATH_MAX];
  SummFormPtr   sfp;
  SimpleSeqPtr  ssp;
  CharPtr       str;

  sfp = (SummFormPtr) GetObjectExtra (d);
  failed = SetDefaultFailureMessage (sfp, item, NULL);
  if (sfp == NULL || sfp->simple == NULL || sfp->state == NULL) return failed;
  if (item < 1) return failed;
  ssp = sfp->simple [item - 1];
  if (ssp == NULL) return failed;
  TmpNam (path);
  fp = FileOpen (path, "w");
  if (fp != NULL) {
    if (SimpleSeqPrint (ssp, fp, TRUE)) {
      FileClose (fp);
      str = FileToString (path);
    } else {
      FileClose (fp);
    }
  }
  FileRemove (path);
  if (str == NULL) return failed;
  MemFree (failed);
  return str;
}

static DocPrntProc mlDocProcs [] = {FetchDocSum, FetchAbstract, FetchCitation, FetchMedline, FetchUid, NULL};
static DocPrntProc aaDocProcs [] = {FetchDocSum, FetchGenPept, FetchFastaProt, FetchUid, NULL};
static DocPrntProc ntDocProcs [] = {FetchDocSum, FetchGenBank, FetchEmbl, FetchFastaNuc, FetchUid, NULL};
static DocPrntProc stDocProcs [] = {FetchDocSum, FetchPDB, FetchUid, NULL};
static DocPrntProc chDocProcs [] = {FetchDocSum, FetchUid, NULL};

static void RepopulateDocSum (SummFormPtr sfp, Boolean needToReset)

{
  ColPtr               colFmt;
  DocDrawProc          ddp;
  Int2                 estLines;
  Int2                 firstLine;
  Int2                 firstShown;
  FonT                 font;
  MedlineViewProcsPtr  mvpp;
  DocPrntProc          proc;
  BaR                  sb;
  Int4                 startsAt;
  SeqViewProcsPtr      svpp;
  Int2                 typ_aa;
  Int2                 typ_ch;
  Int2                 typ_ml;
  Int2                 typ_nt;
  Int2                 typ_st;
  Int2                 val;

  if (sfp == NULL) return;
  WatchCursor ();
  SafeHide (sfp->docsum);
  Update ();
  if (! GetScrlParams4 (sfp->docsum, NULL, &firstShown, &firstLine)) {
    firstShown = 0;
    firstLine = 0;
  }
  sb = GetSlateVScrollBar ((SlatE) sfp->docsum);
  if (needToReset) {
    Reset (sfp->docsum);
    SetDocShade (sfp->docsum, NULL, NULL, NULL, NULL);
    SetDocCache (sfp->docsum, NULL, NULL, NULL);
  }
  /*
  for (i = 0; i < num; i++) {
    sprintf (str, "Item %d UID %ld\n", (int) (i + 1), (long) uids [i]);
    AppendText (sfp->docsum, str, &docsumParFmt, docsumColFmt, programFont);
  }
  */
  typ_ml = DatabaseFromName ("MEDLINE");
  typ_aa = DatabaseFromName ("Protein");
  typ_nt = DatabaseFromName ("Nucleotide");
  typ_st = DatabaseFromName ("Structure");
  typ_ch = DatabaseFromName ("Genome");
  proc = FetchDocSum;
  ddp = DrawDocSum;
  colFmt = docsumColFmt;
  estLines = 3;
  font = sfp->docsumFont;
  if (sfp->simple != NULL) {
    proc = FetchLocalBioseq;
    ddp = DrawLocalSum;
    colFmt = textColFmt;
    svpp = (SeqViewProcsPtr) GetAppProperty ("SeqDisplayForm");
    if (svpp != NULL && svpp->displayFont != NULL) {
      font = svpp->displayFont;
    }
  } else if (sfp->currDb >= 0 && sfp->currDb < MAX_DBS) {
    val = GetValue (sfp->formats [sfp->currDb]);
    if (val > 0) {
      if (val > 1) {
        ddp = DrawTextSum;
        colFmt = textColFmt;
        estLines = 30;
        if (sfp->currDb == typ_ml && val == 4) {
          mvpp = (MedlineViewProcsPtr) GetAppProperty ("MedlineDisplayForm");
          if (mvpp != NULL && mvpp->displayFont != NULL) {
            font = mvpp->displayFont;
          }
        } else if (sfp->currDb == typ_aa || sfp->currDb == typ_nt) {
          svpp = (SeqViewProcsPtr) GetAppProperty ("SeqDisplayForm");
          if (svpp != NULL && svpp->displayFont != NULL) {
            font = svpp->displayFont;
          }
        }
      }
      if (sfp->currDb == typ_ml) {
        proc = mlDocProcs [val - 1];
      } else if (sfp->currDb == typ_aa) {
        proc = aaDocProcs [val - 1];
      } else if (sfp->currDb == typ_nt) {
        proc = ntDocProcs [val - 1];
      } else if (sfp->currDb == typ_st) {
        proc = stDocProcs [val - 1];
      } else if (sfp->currDb == typ_ch) {
        proc = chDocProcs [val - 1];
      }
    }
  }
  BulkAppendItem (sfp->docsum, sfp->numUids, proc, estLines,
                  &docsumParFmt, colFmt, font);
  SetDocShade (sfp->docsum, ddp, NULL, NULL, NULL);
  SetDocCache (sfp->docsum, StdPutDocCache, StdGetDocCache, StdResetDocCache);
  /*
  InvalDocument (sfp->docsum);
  Update ();
  */
  AdjustDocScroll (sfp->docsum);
  GetItemParams4 (sfp->docsum, firstShown, &startsAt, NULL, NULL, NULL, NULL);
  CorrectBarValue (sb, startsAt + firstLine);
  ResetClip ();
  SafeShow (sfp->docsum);
  ArrowCursor ();
  Update ();
}

static void SetPrevAndNextButtons (SummFormPtr sfp)

{
  if (sfp->present > 1) {
    SafeEnable (sfp->prevBtn);
  } else {
    SafeDisable (sfp->prevBtn);
  }
  if (sfp->present < sfp->generations) {
    SafeEnable (sfp->nextBtn);
  } else {
    SafeDisable (sfp->nextBtn);
  }
}

static void RetrieveGeneration (SummFormPtr sfp, Int2 num, Int2 parents, Int4Ptr uids, Int2 db)

{
  Int2  i;
  Char  title [32];

  if (sfp == NULL || uids == NULL) return;
  Reset (sfp->docsum);
  SetDocShade (sfp->docsum, NULL, NULL, NULL, NULL);
  SetDocCache (sfp->docsum, NULL, NULL, NULL);
  sfp->uids = MemFree (sfp->uids);
  if (sfp->simple != NULL) {
    for (i = 0; i < sfp->numUids; i++) {
      SimpleSeqFree (sfp->simple [i]);
    }
    sfp->simple = MemFree (sfp->simple);
  }
  sfp->state = MemFree (sfp->state);
  sfp->numUids = 0;
  sfp->numParents = 0;
  sfp->neighbors = MemFree (sfp->neighbors);
  sfp->numNeighbors = 0;
  SafeSetTitle (sfp->retrieve, "Neighbor 0");
  SafeDisable (sfp->retrieve);
  sprintf (title, "Refine %d", (int) num);
  SafeSetTitle (sfp->refine, title);
  if (num > 0) {
    SafeEnable (sfp->refine);
  } else {
    SafeDisable (sfp->refine);
  }
  SetEnumPopup (sfp->target, sfp->dbalist, (UIEnum) db);
  SafeShow (sfp->target);
  SetPrevAndNextButtons (sfp);
  Update ();
  Hide (sfp->formats [MAX_DBS]);
  for (i = 0; i < MAX_DBS; i++) {
    if (i != db) {
      Hide (sfp->formats [i]);
    }
  }
  if (db < MAX_DBS) {
    Show (sfp->formats [db]);
  }
  if (num > 0) {
    sfp->uids = MemDup ((Pointer) uids, (size_t) (num * sizeof(DocUid)));
    if (sfp->uids == NULL) return;
    sfp->state = MemNew (sizeof (StateData) * (size_t) num);
    if (sfp->state == NULL) return;
    sfp->numUids = num;
    sfp->numParents = parents;
    sfp->currDb = db;
    Show (sfp->form);
    Select (sfp->form);
    RepopulateDocSum (sfp, FALSE);
    SetDocSumImportExportItems (sfp);
    SendMessageToForm (sfp->form, VIB_MSG_CHANGE);
#ifdef WIN_MAC
    if (sfp->activate != NULL) {
      sfp->activate ((WindoW) sfp->form);
    }
#endif
  } else {
    Show (sfp->form);
    Select (sfp->form);
#ifdef WIN_MAC
    if (sfp->activate != NULL) {
      sfp->activate ((WindoW) sfp->form);
    }
#endif
  }
  Update ();
}

extern void RetrieveDocuments (ForM f, Int2 num, Int2 parents, Int4Ptr uids, Int2 db)

{
  FILE         *fp;
  Char         path [PATH_MAX];
  SummFormPtr  sfp;
  Int2         val;

  sfp = (SummFormPtr) GetObjectExtra (f);
  if (sfp == NULL) return;
  (sfp->generations)++;
  sfp->present = sfp->generations;
  if (sfp->historyFile == NULL) {
    TmpNam (path);
    sfp->historyFile = StringSave (path);
#ifdef WIN_MAC
    if (sfp->historyFile != NULL) {
      FileCreate (sfp->historyFile, "????", "ENTZ");
    }
#endif
  }
  if (sfp->historyOffsets == NULL) {
    sfp->historyOffsets = MemNew (100 * sizeof (Int4));
  }
  if (sfp->historyFile != NULL && sfp->historyOffsets != NULL) {
    fp = FileOpen (sfp->historyFile, "ab");
    if (fp != NULL) {
      fseek (fp, 0, SEEK_END);
      sfp->historyOffsets [(sfp->present) % 100] = ftell (fp);
      val = db;
      FileWrite (&val, sizeof (val), 1, fp);
      val = num;
      FileWrite (&val, sizeof (val), 1, fp);
      val = parents;
      FileWrite (&val, sizeof (val), 1, fp);
      FileWrite (uids, sizeof (Int4), (size_t) num, fp);
      FileClose (fp);
    } else {
      sfp->historyOffsets [sfp->present % 100] = 0;
    }
  }
  RetrieveGeneration (sfp, num, parents, uids, db);
}

static void StripNewLine (CharPtr str)

{
  CharPtr  chptr;

  if (StringHasNoText (str)) return;
  chptr = StringRChr (str, '\n');
  if (chptr != NULL) {
    *chptr = '\0';
  }
  chptr = StringRChr (str, '\r');
  if (chptr != NULL) {
    *chptr = '\0';
  }
}

extern void DSLoadUidListProc (IteM i)

{
  ByteStorePtr  bsp;
  Int2          db = -1;
  FILE          *fp;
  Int2          num;
  Char          path [PATH_MAX];
  SummFormPtr   sfp;
  Char          str [32];
  Int4          uid;
  Int4Ptr       uids;


#ifdef WIN_MAC
  sfp = (SummFormPtr) currentFormDataPtr;
#else
  sfp = (SummFormPtr) GetObjectExtra (i);
#endif
  if (sfp == NULL) return;
  if (! GetInputFileName (path, sizeof (path), "", "TEXT")) return;
  fp = FileOpen (path, "r");
  if (fp == NULL) return;
  bsp = BSNew (128);
  if (bsp == NULL) {
    FileClose (fp);
    return;
  }
  if (FileGets (str, sizeof (str), fp)) {
    StripNewLine (str);
    if (StringStr (str, ">PubMed")) {
      db = 0;
    } else if (StringStr (str, ">Protein")) {
      db = 1;
    } else if (StringStr (str, ">Nucleotide")) {
      db = 2;
    } else if (StringStr (str, ">Structure")) {
      db = 3;
    } else if (StringStr (str, ">Genome")) {
      db = 4;
    }
    if (db != -1) {
      while (FileGets (str, sizeof (str), fp)) {
        StripNewLine (str);
        if (str [0] != '\0' && StrToLong (str, &uid)) {
          BSWrite (bsp, &uid, sizeof (DocUid));
        }
      }
      BSSeek (bsp, 0L, 0);
      num = (Int2) ((BSLen (bsp)) / sizeof (Int4));
      uids = (Int4Ptr) BSMerge (bsp, NULL);
      if (uids != NULL) {
        RetrieveDocuments (sfp->form, num, 0, uids, db);
      }
      MemFree (uids);
    } else {
      Message (MSG_POSTERR,
               "First line must be >PubMed, >Protein, >Nucleotide, >Structure, or >Genome");
    }
  }
  BSFree (bsp);
  FileClose (fp);
}

extern void DSSaveUidListProc (IteM i)

{
  FILE         *fp;
  Int2         j;
  Char         path [PATH_MAX];
  SummFormPtr  sfp;
  Char         str [32];


#ifdef WIN_MAC
  sfp = (SummFormPtr) currentFormDataPtr;
#else
  sfp = (SummFormPtr) GetObjectExtra (i);
#endif
  if (sfp == NULL || sfp->uids == NULL) return;
  if (! GetOutputFileName (path, sizeof (path), NULL)) return;
#ifdef WIN_MAC
  FileCreate (path, "TEXT", "ttxt");
#endif
  fp = FileOpen (path, "w");
  if (fp == NULL) return;
  WatchCursor ();
  switch (sfp->currDb) {
    case 0 :
      StringCpy (str, ">PubMed\n");
      break;
    case 1 :
      StringCpy (str, ">Protein\n");
      break;
    case 2 :
      StringCpy (str, ">Nucleotide\n");
      break;
    case 3 :
      StringCpy (str, ">Structure\n");
      break;
    case 4 :
      StringCpy (str, ">Genome\n");
      break;
    default :
      StringCpy (str, ">?\n");
      break;
  }
  FilePuts (str, fp);
  for (j = 0; j < sfp->numUids; j++) {
    sprintf (str, "%ld\n", (long) (sfp->uids [j]));
    FilePuts (str, fp);
  }
  FileClose (fp);
  ArrowCursor ();
}

static void LoadPresentGeneration (SummFormPtr sfp)

{
  Int2     db;
  FILE     *fp;
  Int2     num;
  Int2     parents;
  Int4Ptr  uids;

  if (sfp == NULL) return;
  WatchCursor ();
  if (sfp->historyFile != NULL && sfp->historyOffsets != NULL) {
    fp = FileOpen (sfp->historyFile, "rb");
    if (fp != NULL) {
      fseek (fp, sfp->historyOffsets [(sfp->present) % 100], SEEK_SET);
      FileRead (&db, sizeof (db), 1, fp);
      FileRead (&num, sizeof (num), 1, fp);
      FileRead (&parents, sizeof (parents), 1, fp);
      uids = (Int4Ptr) MemNew ((size_t) num * sizeof (Int4) + 4);
      if (uids != NULL) {
        FileRead (uids, sizeof (Int4), (size_t) num, fp);
      }
      FileClose (fp);
      if (uids != NULL) {
        RetrieveGeneration (sfp, num, parents, uids, db);
        MemFree (uids);
      }
    }
  }
  ArrowCursor ();
}

static void PrevProc (ButtoN b)

{
  SummFormPtr  sfp;

  sfp = (SummFormPtr) GetObjectExtra (b);
  if (sfp == NULL) return;
  (sfp->present)--;
  if (sfp->present < 1) {
    sfp->present = 1;
  }
  SetPrevAndNextButtons (sfp);
  LoadPresentGeneration (sfp);
  SetDocSumImportExportItems (sfp);
}

static void NextProc (ButtoN b)

{
  SummFormPtr  sfp;

  sfp = (SummFormPtr) GetObjectExtra (b);
  if (sfp == NULL) return;
  (sfp->present)++;
  if (sfp->present > sfp->generations) {
    sfp->present = sfp->generations;
  }
  SetPrevAndNextButtons (sfp);
  LoadPresentGeneration (sfp);
  SetDocSumImportExportItems (sfp);
}

extern Boolean UsingDelayedNeighbor (ForM f)

{
  SummFormPtr  sfp;

  sfp = (SummFormPtr) GetObjectExtra (f);
  if (sfp == NULL) return FALSE;
  return sfp->usingDelay;
}

extern void UseDelayedNeighbor (ForM f, Boolean delayMode)

{
  SummFormPtr  sfp;

  sfp = (SummFormPtr) GetObjectExtra (f);
  if (sfp == NULL) return;
  sfp->usingDelay = delayMode;
}

static void NeighborDelayProc (ChoicE c)

{
  SummFormPtr  sfp;

#ifdef WIN_MAC
  sfp = (SummFormPtr) currentFormDataPtr;
#else
  sfp = (SummFormPtr) GetObjectExtra (c);
#endif
  if (sfp == NULL) return;
  if (GetValue (c) == 1) {
    sfp->usingDelay = FALSE;
  } else {
    sfp->usingDelay = TRUE;
  }
}

extern ChoicE CreateNeighborDelayChoice (MenU m, BaseFormPtr bfp)

{
  ChoicE  c;

  c = ChoiceGroup (m, NeighborDelayProc);
  SetObjectExtra (c, bfp, NULL);
  ChoiceItem (c, "Immediate");
  ChoiceItem (c, "Delayed");
  SetValue (c, 1);
  return c;
}

static void RecalculateDocSum (SummFormPtr sfp)

{
  Int2        i;
  LinkSetPtr  lsp;
  Int2        num;
  Int2        targetDb;
  Char        title [32];
  Int4Ptr     uids;
  UIEnum      val;

  if (sfp == NULL) return;
  if (GetEnumPopup (sfp->target, sfp->dbalist, &val)) {
    targetDb = (Int2) val;
  } else {
    targetDb = sfp->currDb;
  }
  if (sfp->uids == NULL || sfp->state == NULL) {
    if (targetDb == sfp->currDb) {
      SafeSetTitle (sfp->retrieve, "Neighbor 0");
    } else {
      SafeSetTitle (sfp->retrieve, "Lookup 0");
    }
    SafeDisable (sfp->retrieve);
    return;
  }
  sfp->neighbors = MemFree (sfp->neighbors);
  sfp->numNeighbors = 0;
  sfp->neighborDb = targetDb;

  uids = NULL;
  num = 0;
  for (i = 0; i < sfp->numUids; i++) {
    if (sfp->state [i].checked) {
      num++;
    }
  }

  if (sfp->retrieveMode == EVAL_MODE && sfp->usingDelay) {
    if (num == 0) {
      if (targetDb == sfp->currDb) {
        SafeSetTitle (sfp->retrieve,  "Neighbor 0");
       } else {
        SafeSetTitle (sfp->retrieve, "Lookup 0");
       }
       SafeDisable (sfp->retrieve);
    } else {
      SafeSetTitle (sfp->retrieve, "Evaluate");
      SafeEnable (sfp->retrieve);
    }
    if (num == 0) {
      num = sfp->numUids;
    }
    sprintf (title, "Refine %d", (int) num);
    SafeSetTitle (sfp->refine, title);
    if (num > 0) {
      SafeEnable (sfp->refine);
    } else {
      SafeDisable (sfp->refine);
    }
    return;
  }

  if (num > 0) {
    uids = MemNew ((size_t) (num + 1) * sizeof (DocUid));
    if (uids != NULL) {
      num = 0;
      for (i = 0; i < sfp->numUids; i++) {
        if (sfp->state [i].checked) {
          uids [num] = sfp->uids [i];
          num++;
        }
      }
      lsp = NULL;
      EntrezLinkUidList (&lsp, sfp->currDb, targetDb,
                         num, uids, FALSE);
      if (lsp != NULL) {
        sfp->numNeighbors = (Int2) lsp->num;
        sfp->neighbors = lsp->uids;
        lsp->uids = NULL;
        lsp->weights = MemFree (lsp->weights);
        LinkSetFree (lsp);
      }
    }
  }
  if (targetDb == sfp->currDb) {
    sprintf (title, "Neighbor %d", (int) sfp->numNeighbors);
  } else {
    sprintf (title, "Lookup %d", (int) sfp->numNeighbors);
  }
  SafeSetTitle (sfp->retrieve, title);
  if (sfp->numNeighbors > 0) {
    SafeEnable (sfp->retrieve);
  } else {
    SafeDisable (sfp->retrieve);
  }
  if (num == 0) {
    num = sfp->numUids;
  }
  sprintf (title, "Refine %d", (int) num);
  SafeSetTitle (sfp->refine, title);
  if (num > 0) {
    SafeEnable (sfp->refine);
  } else {
    SafeDisable (sfp->refine);
  }
  MemFree (uids);
}

static void ChangeTarget (PopuP p)

{
  SummFormPtr  sfp;

  sfp = (SummFormPtr) GetObjectExtra (p);
  if (sfp == NULL) return;
  sfp->retrieveMode = EVAL_MODE;
  RecalculateDocSum (sfp);
}

static Boolean UidInList (Int4 uid, Int4Ptr sorted, Int2 num)

{
  Boolean  found;
  Int2     left;
  Int2     mid;
  Int2     right;

  found = FALSE;
  if (sorted != NULL && num > 0) {
    mid = 0;
    left = 0;
    right = num - 1;
    while (left <= right) {
      mid = (left + right) / 2;
      if (uid <= sorted [mid]) {
        right = mid - 1;
      }
      if (uid >= sorted [mid]) {
        left = mid + 1;
      }
    }
    if (left > right + 1) {
      found = TRUE;
    }
  }
  return found;
}

static void RetrieveNeighbors (ButtoN b)

{
  EntrezGlobalsPtr  egp;
  Int2              i, j;
  Int2              num;
  Int2              parents;
  Boolean           persist;
  SummFormPtr       sfp;
  Int4Ptr           sorted;
  Int4              tmp;
  Int4Ptr           uids;

  sfp = (SummFormPtr) GetObjectExtra (b);
  if (sfp == NULL) return;
  if (sfp->retrieveMode == EVAL_MODE && sfp->usingDelay) {
    WatchCursor ();
    Update ();
    sfp->retrieveMode = FETCH_MODE;
    RecalculateDocSum (sfp);
    ArrowCursor ();
    Update ();
    return;
  }
  egp = (EntrezGlobalsPtr) GetAppProperty ("EntrezGlobals");
  if (egp == NULL || egp->retrieveDocsProc == NULL) return;
  persist = FALSE;
  if (sfp->currDb == sfp->neighborDb) {
    persist = GetStatus (egp->parentsPersist);
  }
  parents = 0;
  if (persist) {
    for (i = 0; i < sfp->numUids; i++) {
      if (sfp->state [i].checked) {
        parents++;
      }
    }
  }
  uids = NULL;
  sorted = NULL;
  num = parents + sfp->numNeighbors;
  if (num > 0) {
    uids = MemNew ((size_t) (num + 1) * sizeof (DocUid));
    sorted = MemNew ((size_t) (parents + 1) * sizeof (DocUid));
    if (uids != NULL && sorted != NULL) {
      num = 0;
      if (persist && sfp->uids != NULL) {
        for (i = 0; i < sfp->numUids; i++) {
          if (sfp->state [i].checked) {
            uids [num] = sfp->uids [i];
            sorted [num] = sfp->uids [i];
            num++;
          }
        }
        for (i = 1; i < parents; i++) {
          for (j = 0; j < i; j++) {
            if (sorted [j] > sorted [i]) {
              tmp = sorted [i];
              sorted [i] = sorted [j];
              sorted [j] = tmp;
            }
          }
        }
      }
      if (sfp->neighbors != NULL) {
        for (i = 0; i < sfp->numNeighbors; i++) {
          if (! UidInList (sfp->neighbors [i], sorted, parents)) {
            uids [num] = sfp->neighbors [i];
            num++;
          }
        }
      }
    }
  }
  sfp->neighbors = NULL;
  sfp->numNeighbors = 0;
  egp->retrieveDocsProc (sfp->form, num, parents, uids, sfp->neighborDb);
  MemFree (uids);
  MemFree (sorted);
}

static int refinement = 0; /* guarantees that each refinement has a unique name */

static void RefineProc (ButtoN b)

{
  EntrezGlobalsPtr  egp;
  Int2              i;
  Int2              num;
  SummFormPtr       sfp;
  Char              str [64];
  Int4Ptr           uids;

  sfp = (SummFormPtr) GetObjectExtra (b);
  if (sfp == NULL) return;
  egp = (EntrezGlobalsPtr) GetAppProperty ("EntrezGlobals");
  if (egp == NULL || egp->loadNamedUidProc == NULL) return;
  num = 0;
  for (i = 0; i < sfp->numUids; i++) {
    if (sfp->state [i].checked) {
      num++;
    }
  }
  refinement++;
  sprintf (str, "*generation_%d_refinement_%d", (int) sfp->present, (int) refinement);
  if (num > 0) {
    uids = MemNew ((size_t) (num + 1) * sizeof (DocUid));
    if (uids != NULL) {
      num = 0;
      for (i = 0; i < sfp->numUids; i++) {
        if (sfp->state [i].checked) {
          uids [num] = sfp->uids [i];
          num++;
        }
      }
      egp->loadNamedUidProc (sfp->form, str, num, uids, sfp->currDb);
    }
    MemFree (uids);
  } else {
    egp->loadNamedUidProc (sfp->form, str, sfp->numUids, sfp->uids, sfp->currDb);
  }
}

static void CopyDocsumToClipboard (BaseFormPtr bfp)

{
  FILE         *fp;
  Int2         i;
  Boolean      newline;
  Int2         num;
  Char         path [PATH_MAX];
  SummFormPtr  sfp;

  sfp = (SummFormPtr) bfp;
  if (sfp == NULL) return;
  TmpNam (path);
  fp = FileOpen (path, "w");
  if (fp != NULL) {
    num = 0;
    for (i = 0; i < sfp->numUids; i++) {
      if (sfp->state [i].checked) {
        num++;
      }
    }
    if (num == 0) {
      SaveDocument (sfp->docsum, fp);
    } else {
      newline = FALSE;
      for (i = 0; i < sfp->numUids; i++) {
        if (sfp->state [i].checked) {
          if (newline) {
            fputc ('\n', fp);
          }
          SaveDocumentItem (sfp->docsum, fp, i + 1);
          newline = TRUE;
        }
      }
    }
    FileClose (fp);
    FileToClipboard (path);
  }
  FileRemove (path);
}

static void ClickDocSum (DoC d, PoinT pt)

{
  Int2         col;
  Int2         item;
  Int2         row;
  SummFormPtr  sfp;

  sfp = (SummFormPtr) GetObjectExtra (d);
  if (sfp == NULL) return;
  MapDocPoint (d, pt, &item, &row, &col, NULL);
  sfp->dsClickItem = item;
  sfp->dsClickRow = row;
  sfp->dsClickCol = col;
  sfp->wasDoubleClick = dblClick;
}

static Int4Ptr GetCheckedUids (SummFormPtr sfp, Int2Ptr nump)

{
  Int2     i;
  Int2     num;
  Int4Ptr  uids;

  if (sfp == NULL || nump == NULL) return NULL;
  uids = NULL;
  num = 0;
  for (i = 0; i < sfp->numUids; i++) {
    if (sfp->state [i].checked) {
      num++;
    }
  }
  if (num > 0) {
    uids = MemNew ((size_t) (num + 1) * sizeof (DocUid));
    if (uids != NULL) {
      num = 0;
      for (i = 0; i < sfp->numUids; i++) {
        if (sfp->state [i].checked) {
          uids [num] = sfp->uids [i];
          num++;
        }
      }
    }
  }
  if (nump != NULL) {
    *nump = num;
  }
  return uids;
}

static void ReleaseDocSum (DoC d, PoinT pt)

{
  Int2              checked;
  Int2              col;
  Int2              db;
  EntrezGlobalsPtr  egp;
  Int2              item;
  Int2              num;
  RecT              r;
  Int2              row;
  SummFormPtr       sfp;
  Int4              uid;
  Int4Ptr           uids;

  sfp = (SummFormPtr) GetObjectExtra (d);
  if (sfp == NULL) return;
  egp = (EntrezGlobalsPtr) GetAppProperty ("EntrezGlobals");
  if (egp == NULL || egp->launchViewerProc == NULL) return;
  MapDocPoint (d, pt, &item, &row, &col, &r);
  if (item < 1 || item > sfp->numUids || row == 0 || col == 0) return;
  if (sfp->dsClickItem != item) return;
  if (row == 1 && col == 1 &&
      sfp->dsClickRow == 1 && sfp->dsClickCol == 1) {
    r.right = r.left + sfp->lineHeight;
    r.bottom = r.top + sfp->lineHeight;
    if (PtInRect (pt, &r)) {
      checked = sfp->state [item - 1].checked;
      sfp->state [item - 1].checked = (Boolean) (! checked);
      ObjectRect (d, &r);
      InsetRect (&r, 4, 4);
      r.right = r.left + sfp->lineHeight;
      InsetRect (&r, -1, -1);
      InvalRect (&r);
      ResetClip ();
      WatchCursor ();
      Update ();
      sfp->retrieveMode = EVAL_MODE;
      RecalculateDocSum (sfp);
      ArrowCursor ();
      Update ();
      return;
    }
  }
  if (sfp->wasDoubleClick && sfp->uids != NULL && sfp->simple == NULL) {
    db = sfp->currDb;
    uid = sfp->uids [item - 1];
    uids = GetCheckedUids (sfp, &num);
    egp->launchViewerProc (sfp->form, uid, num, uids, db);
    MemFree (uids);
  }
}

static void RefreshAndRecalculateDocSum (SummFormPtr sfp)

{
    RecT  r;

  if (sfp == NULL) return;
  Select (sfp->docsum);
  ObjectRect (sfp->docsum, &r);
  InsetRect (&r, 4, 4);
  r.right = r.left + sfp->lineHeight;
  InsetRect (&r, -1, -1);
  InvalRect (&r);
  ResetClip ();
  WatchCursor ();
  Update ();
  sfp->retrieveMode = EVAL_MODE;
  RecalculateDocSum (sfp);
  ArrowCursor ();
  Update ();
}

static void SelectAllProc (ButtoN b)

{
  Int2         i;
  SummFormPtr  sfp;

  sfp = (SummFormPtr) GetObjectExtra (b);
  if (sfp == NULL) return;
  for (i = 0; i < sfp->numUids; i++) {
    sfp->state [i].checked = TRUE;
  }
  RefreshAndRecalculateDocSum (sfp);
}

static void SelectParentsProc (ButtoN b)

{
  Int2         i;
  SummFormPtr  sfp;

  sfp = (SummFormPtr) GetObjectExtra (b);
  if (sfp == NULL) return;
  for (i = 0; i < sfp->numUids; i++) {
    sfp->state [i].checked = (Boolean) (i < sfp->numParents);
  }
  RefreshAndRecalculateDocSum (sfp);
}

static void ClearAllProc (ButtoN b)

{
  Int2         i;
  SummFormPtr  sfp;

  sfp = (SummFormPtr) GetObjectExtra (b);
  if (sfp == NULL) return;
  for (i = 0; i < sfp->numUids; i++) {
    sfp->state [i].checked = FALSE;
  }
  RefreshAndRecalculateDocSum (sfp);
}

static void ChangeFormat (GrouP g)

{
  SummFormPtr  sfp;

  sfp = (SummFormPtr) GetObjectExtra (g);
  if (sfp == NULL) return;
  RepopulateDocSum (sfp, TRUE);
  SetDocSumImportExportItems (sfp);
}

static CharPtr medRadios [] = {"Summary", "Abstract", "Citation", "MEDLINE", "PubMed ID", NULL};

static CharPtr prtRadios [] = {"Summary", "GenPept", "FASTA", "Protein ID", NULL};

static CharPtr nucRadios [] = {"Summary", "GenBank", "EMBL", "FASTA", "Nucleotide ID", NULL};

static CharPtr strucRadios [] = {"Summary", "Report", "Structure ID", NULL};

static CharPtr genRadios [] = {"Summary", "Genome ID", NULL};

static CharPtr localBioseqRadios [] = {"FASTA", NULL};

static CharPtr PNTR  radioLabels [] = {
  medRadios, prtRadios, nucRadios, strucRadios, genRadios, NULL
};

static void ProcessProjectAsn (ProjectPtr proj)

{
  Int2              db = -1;
  EntrezGlobalsPtr  egp;
  Int4              i;
  ValNodePtr        list;
  Int4              num;
  ValNodePtr        pip;
  Int4Ptr           uids;
  ValNodePtr        vnp;

  if (proj == NULL) return;
  egp = (EntrezGlobalsPtr) GetAppProperty ("EntrezGlobals");
  if (egp == NULL) return;
  pip = proj->data;
  if (pip == NULL) return;
  list = (ValNodePtr) pip->data.ptrvalue;
  if (list == NULL) return;
  if (pip->choice >= ProjectItem_protent && pip->choice <= ProjectItem_genomeent) {
    if (egp->retrieveProjectProc == NULL) return;
    egp->retrieveProjectProc (NULL, (Pointer) proj);
    Update ();
    return;
  }
  if (egp->retrieveDocsProc == NULL) return;
  switch (pip->choice) {
    case ProjectItem_pmuid :
      db = 0;
      break;
    case ProjectItem_protuid :
      db = 1;
      break;
    case ProjectItem_nucuid :
      db = 2;
      break;
    case ProjectItem_genomeuid :
      db = 4;
      break;
    case ProjectItem_structuid :
      db = 3;
      break;
    default :
      break;
  }
  if (db == -1) return;
  num = ValNodeLen (list);
  uids = MemNew ((size_t) (num * sizeof (Int4)));
  if (uids == NULL) return;
  for (i = 0, vnp = list; i < 32766 && vnp != NULL; i++, vnp = vnp->next) {
    uids [i] = vnp->data.intvalue;
  }
  if (egp->retrieveDocsProc != NULL) {
    egp->retrieveDocsProc (NULL, (Int2) num, 0, uids, db);
  }
  MemFree (uids);
  Update ();
}

static Boolean ImportDocSumForm (ForM f, CharPtr filename)

{
  Uint1             choice = 0;
  Pointer           dataptr;
  Uint2             datatype;
  EntrezGlobalsPtr  egp;
  FILE              *fp;
  ValNodePtr        head = NULL;
  Boolean           is_na = TRUE;
  Char              path [PATH_MAX];
  SeqEntryPtr       sephead = NULL;
  ValNodePtr        simples;
  SummFormPtr       sfp;
  ValNodePtr        vnp;

  path [0] = '\0';
  StringNCpy_0 (path, filename, sizeof (path));
  sfp = (SummFormPtr) GetObjectExtra (f);
  if (sfp != NULL) {
    if (path [0] != '\0' || GetInputFileName (path, sizeof (path), "", "TEXT")) {
      fp = FileOpen (path, "r");
      if (fp != NULL) {
        while ((dataptr = ReadAsnFastaOrFlatFile (fp, &datatype, NULL, FALSE,
                                                  FALSE, TRUE, TRUE)) != NULL) {
          ValNodeAddPointer (&head, datatype, dataptr);
        }
        FileClose (fp);
        simples = ValNodeExtractList (&head, OBJ_FASTA);
        if (simples != NULL) {
          egp = (EntrezGlobalsPtr) GetAppProperty ("EntrezGlobals");
          if (egp != NULL && egp->retrieveSimpleProc != NULL) {
            egp->retrieveSimpleProc (NULL, simples);
          }
        }
        ValNodeFree (simples);
        if (head != NULL) {
          for (vnp = head; vnp != NULL; vnp = vnp->next) {
            datatype = vnp->choice;
            dataptr = vnp->data.ptrvalue;
            if (datatype == OBJ_PROJECT) {
              ProcessProjectAsn ((ProjectPtr) dataptr);
            }
          }
        }
        ValNodeFree (head);
        return TRUE;
      }
    }
  }
  return FALSE;
}

static Boolean ExportDocSumForm (ForM f, CharPtr filename)

{
  FILE          *fp;
  Int2          i;
  CharPtr PNTR  labels;
  Boolean       lastChoiceIsUidList = FALSE;
  MonitorPtr    mon;
  Boolean       newline;
  Int2          num;
  Char          path [PATH_MAX];
  SummFormPtr   sfp;
  Char          str [32];
  Int2          val;

  path [0] = '\0';
  StringNCpy_0 (path, filename, sizeof (path));
  sfp = (SummFormPtr) GetObjectExtra (f);
  if (sfp != NULL) {
    if (path [0] != '\0' || GetOutputFileName (path, sizeof (path), NULL)) {
#ifdef WIN_MAC
      fp = FileOpen (path, "r");
      if (fp != NULL) {
        FileClose (fp);
      } else {
        FileCreate (path, "TEXT", "ttxt");
      }
#endif
      fp = FileOpen (path, "w");
      if (fp != NULL) {
        WatchCursor ();
        Update ();
        if (sfp->simple != NULL) {
        } else if (sfp->currDb >= 0 && sfp->currDb < MAX_DBS) {
          labels = radioLabels [sfp->currDb];
          val = GetValue (sfp->formats [sfp->currDb]);
          if (val > 0) {
            if (labels [val] == NULL) {
              lastChoiceIsUidList = TRUE;
            }
          }
        }
        if (lastChoiceIsUidList) {
          switch (sfp->currDb) {
            case 0 :
              fprintf (fp, ">PubMed\n");
              break;
            case 1 :
              fprintf (fp, ">Protein\n");
              break;
            case 2 :
              fprintf (fp, ">Nucleotide\n");
              break;
            case 3 :
              fprintf (fp, ">Structure\n");
              break;
            case 4 :
              fprintf (fp, ">Genome\n");
              break;
            default :
              fprintf (fp, ">?\n");
              break;
          }
        }
        num = 0;
        for (i = 0; i < sfp->numUids; i++) {
          if (sfp->state [i].checked) {
            num++;
          }
        }
        if (lastChoiceIsUidList) {
          if (num == 0) {
            for (i = 0; i < sfp->numUids; i++) {
              sprintf (str, "%ld\n", (long) (sfp->uids [i]));
              FilePuts (str, fp);
            }
          } else {
            for (i = 0; i < sfp->numUids; i++) {
              if (sfp->state [i].checked) {
                sprintf (str, "%ld\n", (long) (sfp->uids [i]));
                FilePuts (str, fp);
              }
            }
          }
        } else {
          mon = MonitorIntNewEx ("Saving Documents", 0, sfp->numUids, FALSE);
          newline = FALSE;
          for (i = 0; i < sfp->numUids; i++) {
            if (num == 0 || sfp->state [i].checked) {
              if (newline) {
                fputc ('\n', fp);
              }
              SaveDocumentItem (sfp->docsum, fp, i + 1);
              MonitorIntValue (mon, i);
              newline = TRUE;
            }
          }
          MonitorFree (mon);
        }
        FileClose (fp);
        sfp->filepath = MemFree (sfp->filepath);
        sfp->filepath = StringSave (path);
        ArrowCursor ();
        Update ();
        return TRUE;
      } else {
        Message (MSG_ERROR, "Unable to write file.");
        return FALSE;
      }
    }
  }
  return FALSE;
}

static void PrintDocsumProc (BaseFormPtr bfp)

{
  SummFormPtr  sfp;

  if (bfp == NULL) return;
  sfp = (SummFormPtr) bfp;
  WatchCursor ();
  Update ();
  PrintDocument (sfp->docsum);
  ArrowCursor ();
  Update ();
}

extern void LoadDocsumOptionsMenu (MenU m)

{
  EntrezGlobalsPtr  egp;
  MenU              sub;

  egp = (EntrezGlobalsPtr) GetAppProperty ("EntrezGlobals");
  if (egp == NULL) return;
  egp->parentsPersist = StatusItem (m, "Parents Persist", NULL);
  SetStatus (egp->parentsPersist, egp->persistDefault);
  egp->alignWithChecked = StatusItem (m, "Align Checked Items", NULL);
  SetStatus (egp->alignWithChecked, egp->alignDefault);
  SeparatorItem (m);
  /*
  sub = SubMenu (m, "Sequence Complexity");
  egp->seqComplex = ChoiceGroup (sub, NULL);
  ChoiceItem (egp->seqComplex, "NucProt");
  ChoiceItem (egp->seqComplex, "SegSet");
  ChoiceItem (egp->seqComplex, "BioSeq");
  SetValue (egp->seqComplex, 2);
  SeparatorItem (m);
  */
  sub = SubMenu (m, "Structure Complexity");
  egp->strucComplex = ChoiceGroup (sub, NULL);
  ChoiceItem (egp->strucComplex, "NCBI one XYZ per atom model");
  ChoiceItem (egp->strucComplex, "NCBI backbone model");
  ChoiceItem (egp->strucComplex, "Original PDB models 1-n");
  ChoiceItem (egp->strucComplex, "NCBI vector model");
  ChoiceItem (egp->strucComplex, "Everything");
  SetValue (egp->strucComplex, 1);
  sub = SubMenu (m, "Number of Models");
  egp->strucModels = ChoiceGroup (sub, NULL);
  ChoiceItem (egp->strucModels, "1");
  ChoiceItem (egp->strucModels, "2");
  ChoiceItem (egp->strucModels, "5");
  ChoiceItem (egp->strucModels, "10");
  ChoiceItem (egp->strucModels, "15");
  ChoiceItem (egp->strucModels, "20");
  ChoiceItem (egp->strucModels, "maximum");
  SetValue (egp->strucModels, 7);
}

extern void DocSumFontChangeProc (IteM i)

{
  BaseFormPtr       bfp;
  EntrezGlobalsPtr  egp;
  Uint2             flags;
  FonT              fnt;
  FontSpec          fs;
  Char              str [FONT_NAME_SIZE + 10];

  egp = (EntrezGlobalsPtr) GetAppProperty ("EntrezGlobals");
#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp != NULL) {
    MemSet ((Pointer) (&fs), 0, sizeof (FontSpec));
    fnt = programFont;
    if (egp != NULL && egp->docsumFont != NULL) {
      fnt = egp->docsumFont;
    }
    flags = 0;
    fs.name [0] = '\0';
    fs.size = 0;
    fs.style = 0;
    fs.charset = 0;
    fs.pitch = 0;
    fs.family = 0;
    if (GetFontSpec (fnt, &fs)) {
      flags = CFF_READ_FSP;
    }
    if (ChooseFont (&fs, flags, NULL)) {
      sprintf (str, "%s,%d", fs.name, (int) fs.size);
      if ((fs.style & STYLE_BOLD) != 0 ||
          (fs.style & STYLE_ITALIC) != 0 ||
          (fs.style & STYLE_UNDERLINE) != 0) {
        StringCat (str, ",");
        if ((fs.style & STYLE_BOLD) != 0) {
          StringCat (str, "b");
        }
        if ((fs.style & STYLE_ITALIC) != 0) {
          StringCat (str, "i");
        }
        if ((fs.style & STYLE_UNDERLINE) != 0) {
          StringCat (str, "u");
        }
      }
      /* SetEntrezAppParam ("FONTS", "FETCHED", str); */
      if (egp != NULL) {
        egp->docsumFont = CreateFont (&fs);
      }
      SendMessageToForm (bfp->form, VIB_MSG_REDRAW);
    }
  }
}

extern void DisplayFontChangeProc (IteM i)

{
  BaseFormPtr          bfp;
  Uint2                flags;
  FonT                 fnt;
  FontSpec             fs;
  MedlineViewProcsPtr  mvpp;
  Char                 str [FONT_NAME_SIZE + 10];
  SeqViewProcsPtr      svpp;

  svpp = (SeqViewProcsPtr) GetAppProperty ("SeqDisplayForm");
  mvpp = (MedlineViewProcsPtr) GetAppProperty ("MedlineDisplayForm");
#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp != NULL) {
    MemSet ((Pointer) (&fs), 0, sizeof (FontSpec));
    fnt = programFont;
    if (svpp != NULL && svpp->displayFont != NULL) {
      fnt = svpp->displayFont;
    } else if (mvpp != NULL && mvpp->displayFont != NULL) {
      fnt = mvpp->displayFont;
    }
    flags = 0;
    fs.name [0] = '\0';
    fs.size = 0;
    fs.style = 0;
    fs.charset = 0;
    fs.pitch = 0;
    fs.family = 0;
    if (GetFontSpec (fnt, &fs)) {
      flags = CFF_READ_FSP | CFF_MONOSPACE;
    }
    if (ChooseFont (&fs, flags, NULL)) {
      sprintf (str, "%s,%d", fs.name, (int) fs.size);
      if ((fs.style & STYLE_BOLD) != 0 ||
          (fs.style & STYLE_ITALIC) != 0 ||
          (fs.style & STYLE_UNDERLINE) != 0) {
        StringCat (str, ",");
        if ((fs.style & STYLE_BOLD) != 0) {
          StringCat (str, "b");
        }
        if ((fs.style & STYLE_ITALIC) != 0) {
          StringCat (str, "i");
        }
        if ((fs.style & STYLE_UNDERLINE) != 0) {
          StringCat (str, "u");
        }
      }
      /* SetEntrezAppParam ("FONTS", "DISPLAY", str); */
      if (svpp != NULL) {
        svpp->displayFont = CreateFont (&fs);
      }
      if (mvpp != NULL) {
        mvpp->displayFont = CreateFont (&fs);
      }
      SendMessageToForm (bfp->form, VIB_MSG_REDRAW);
    }
  }
}

extern Boolean DocSumCanSaveFasta (ForM f, Boolean nucs, Boolean prots)

{
  Int2          i;
  SummFormPtr   sfp;
  SimpleSeqPtr  ssp;

  sfp = (SummFormPtr) GetObjectExtra (f);
  if (sfp == NULL) return FALSE;
  if (sfp->uids != NULL && sfp->simple == NULL) {
    switch (sfp->currDb) {
      case 1 :
        if (prots) return TRUE;
        break;
      case 2 :
      case 4 :
        if (nucs) return TRUE;
        break;
      default :
        break;
    }
  } else if (sfp->simple != NULL && sfp->uids == NULL) {
    for (i = 0; i < sfp->numUids; i++) {
      ssp = sfp->simple [i];
      if (ssp != NULL) {
        return TRUE;
      }
    }
    return FALSE;
  }
  return FALSE;
}

extern Boolean ExportDocSumFasta (ForM f, CharPtr filename, Boolean nucs, Boolean prots)

{
  BioseqPtr     bsp;
  Boolean       fastaOK = FALSE;
  Boolean       fastaNucOK;
  Boolean       fastaPrtOK;
  FILE          *fp;
  Uint1         group_segs;
  Int2          i;
  Int2          num;
  Char          path [PATH_MAX];
  SeqEntryPtr   sep;
  SummFormPtr   sfp;
  SimpleSeqPtr  ssp;
  Int4          uid;

  path [0] = '\0';
  StringNCpy_0 (path, filename, sizeof (path));
  sfp = (SummFormPtr) GetObjectExtra (f);
  if (sfp != NULL) {
    if (path [0] != '\0' || GetOutputFileName (path, sizeof (path), NULL)) {
#ifdef WIN_MAC
      fp = FileOpen (path, "r");
      if (fp != NULL) {
        FileClose (fp);
      } else {
        FileCreate (path, "TEXT", "ttxt");
      }
#endif
      fp = FileOpen (path, "w");
      if (fp != NULL) {
        WatchCursor ();
        Update ();
        num = 0;
        for (i = 0; i < sfp->numUids; i++) {
          if (sfp->state [i].checked) {
            num++;
          }
        }
        for (i = 0; i < sfp->numUids; i++) {
          fastaNucOK = FALSE;
          fastaPrtOK = FALSE;
          if (sfp->state [i].checked || num == 0) {
            if (sfp->uids != NULL && sfp->simple == NULL) {
              uid = sfp->uids [i];
              bsp = BioseqLockByGi (uid);
              if (bsp != NULL) {
                sep = SeqMgrGetSeqEntryForData (bsp);
                if (sep != NULL) {
                  if (ISA_na (bsp->mol) && nucs) {
                    group_segs = 0;
                    if (bsp->repr == Seq_repr_seg) {
                      group_segs = 1;
                    } else if (bsp->repr == Seq_repr_delta) {
                      group_segs = 3;
                    }
                    fastaNucOK = SeqnSeqEntrysToFasta (sep, fp, TRUE, group_segs);
                  }
                  if (ISA_aa (bsp->mol) && prots) {
                    fastaPrtOK = SeqnSeqEntrysToFasta (sep, fp, FALSE, 0);
                  }
                }
              }
              BioseqUnlockByGi (uid);
            } else if (sfp->simple != NULL && sfp->uids == NULL) {
              ssp = sfp->simple [i];
              if (ssp != NULL) {
                SimpleSeqPrint (ssp, fp, TRUE);
              }
            }
          }
          fastaOK |= fastaNucOK || fastaPrtOK;
        }
        FileClose (fp);
        ArrowCursor ();
        Update ();
        return fastaOK;
      }
    }
  }
  return FALSE;
}

extern void RetrieveSimpleSeqs (ForM f, ValNodePtr simpleSeqs)

{
  Int4         i;
  Int4         num;
  SummFormPtr  sfp;
  ValNodePtr   vnp;

  sfp = (SummFormPtr) GetObjectExtra (f);
  if (sfp == NULL) return;
  Reset (sfp->docsum);
  SetDocShade (sfp->docsum, NULL, NULL, NULL, NULL);
  SetDocCache (sfp->docsum, NULL, NULL, NULL);
  sfp->uids = MemFree (sfp->uids);
  if (sfp->simple != NULL) {
    for (i = 0; i < sfp->numUids; i++) {
      SimpleSeqFree (sfp->simple [i]);
    }
    sfp->simple = MemFree (sfp->simple);
  }
  sfp->state = MemFree (sfp->state);
  sfp->numUids = 0;
  sfp->numParents = 0;
  sfp->neighbors = MemFree (sfp->neighbors);
  sfp->numNeighbors = 0;
  SafeSetTitle (sfp->retrieve, "Neighbor 0");
  SafeDisable (sfp->retrieve);
  SafeSetTitle (sfp->refine, "Refine 0");
  SafeDisable (sfp->refine);
  SafeHide (sfp->target);
  SetValue (sfp->target, 0);
  SetPrevAndNextButtons (sfp);
  Update ();
  for (i = 0; i < MAX_DBS; i++) {
    Hide (sfp->formats [i]);
  }
  Show (sfp->formats [MAX_DBS]);
  if (simpleSeqs == NULL) return;
  num = ValNodeLen (simpleSeqs);
  if (num > 0) {
    sfp->simple = MemNew ((size_t) (num * sizeof (SimpleSeqPtr)));
    if (sfp->simple == NULL) {
      Show (sfp->form);
      Select (sfp->form);
      SendMessageToForm (sfp->form, VIB_MSG_CHANGE);
#ifdef WIN_MAC
      if (sfp->activate != NULL) {
        sfp->activate ((WindoW) sfp->form);
      }
#endif
      return;
    }
    sfp->state = MemNew (sizeof (StateData) * (size_t) num);
    if (sfp->state == NULL) return;
    for (i = 0, vnp = simpleSeqs; i < 32766 && vnp != NULL; i++, vnp = vnp->next) {
      sfp->simple [i] = (SimpleSeqPtr) vnp->data.ptrvalue;
    }
    sfp->numUids = num;
    sfp->numParents = 0;
    sfp->currDb = -1;
    Show (sfp->form);
    Select (sfp->form);
    RepopulateDocSum (sfp, FALSE);
    SetDocSumImportExportItems (sfp);
    SendMessageToForm (sfp->form, VIB_MSG_CHANGE);
#ifdef WIN_MAC
    if (sfp->activate != NULL) {
      sfp->activate ((WindoW) sfp->form);
    }
#endif
  } else {
    Show (sfp->form);
    Select (sfp->form);
#ifdef WIN_MAC
    if (sfp->activate != NULL) {
      sfp->activate ((WindoW) sfp->form);
    }
#endif
  }
  Update ();
}

static void ProjectPtrToDocSumForm (ForM f, Pointer data)

{
  /*
  Int4         i;
  ValNodePtr   list;
  Int4         num;
  ValNodePtr   pip;
  ProjectPtr   proj;
  SummFormPtr  sfp;
  ValNodePtr   vnp;

  sfp = (SummFormPtr) GetObjectExtra (f);
  if (sfp == NULL) return;
  Reset (sfp->docsum);
  SetDocShade (sfp->docsum, NULL, NULL, NULL, NULL);
  SetDocCache (sfp->docsum, NULL, NULL, NULL);
  sfp->uids = MemFree (sfp->uids);
  if (sfp->simple != NULL) {
    for (i = 0; i < sfp->numUids; i++) {
      SimpleSeqFree (sfp->simple [i]);
    }
    sfp->simple = MemFree (sfp->simple);
  }
  sfp->state = MemFree (sfp->state);
  sfp->numUids = 0;
  sfp->numParents = 0;
  sfp->neighbors = MemFree (sfp->neighbors);
  sfp->numNeighbors = 0;
  SafeSetTitle (sfp->retrieve, "Neighbor 0");
  SafeDisable (sfp->retrieve);
  SafeSetTitle (sfp->refine, "Refine 0");
  SafeDisable (sfp->refine);
  SafeHide (sfp->target);
  SetValue (sfp->target, 0);
  SetPrevAndNextButtons (sfp);
  Update ();
  for (i = 0; i < MAX_DBS; i++) {
    Hide (sfp->formats [i]);
  }
  Show (sfp->formats [MAX_DBS]);
  proj = (ProjectPtr) data;
  if (proj == NULL) return;
  pip = proj->data;
  if (pip == NULL) return;
  list = (ValNodePtr) pip->data.ptrvalue;
  if (list == NULL) return;
  num = ValNodeLen (list);
  if (num > 0) {
    if (pip->choice >= ProjectItem_protent && pip->choice <= ProjectItem_genomeent) {
      sfp->simple = MemNew ((size_t) (num * sizeof (SimpleSeqPtr)));
      if (sfp->seqentry == NULL) {
        Show (sfp->form);
        Select (sfp->form);
        SendMessageToForm (sfp->form, VIB_MSG_CHANGE);
#ifdef WIN_MAC
        if (sfp->activate != NULL) {
          sfp->activate ((WindoW) sfp->form);
        }
#endif
        return;
      }
      sfp->state = MemNew (sizeof (StateData) * (size_t) num);
      if (sfp->state == NULL) return;
      for (i = 0, vnp = list; i < 32766 && vnp != NULL; i++, vnp = vnp->next) {
        sfp->seqentry [i] = vnp;
      }
      sfp->numUids = num;
      sfp->numParents = 0;
      sfp->currDb = -1;
      Show (sfp->form);
      Select (sfp->form);
      RepopulateDocSum (sfp, FALSE);
      SetDocSumImportExportItems (sfp);
      SendMessageToForm (sfp->form, VIB_MSG_CHANGE);
#ifdef WIN_MAC
      if (sfp->activate != NULL) {
        sfp->activate ((WindoW) sfp->form);
      }
#endif
    } else {
      Show (sfp->form);
      Select (sfp->form);
#ifdef WIN_MAC
      if (sfp->activate != NULL) {
        sfp->activate ((WindoW) sfp->form);
      }
#endif
    }
  } else {
    Show (sfp->form);
    Select (sfp->form);
#ifdef WIN_MAC
    if (sfp->activate != NULL) {
      sfp->activate ((WindoW) sfp->form);
    }
#endif
  }
  Update ();
  */
}

static Pointer DocSumFormToProjectPtr (ForM f)

{
  /*
  BioseqPtr    bsp;
  Char         buf [40];
  Char         ch;
  Uint1        choice = 0;
  Boolean      is_na = TRUE;
  Int2         j;
  ObjectIdPtr  oid;
  ValNodePtr   pip;
  ProjectPtr   proj;
  CharPtr      ptr;
  SeqEntryPtr  sep;
  SummFormPtr  sfp;
  SeqIdPtr     sip;
  BioseqPtr    tmp;
  Int4         uid;

  sfp = (SummFormPtr) GetObjectExtra (f);
  if (sfp == NULL) return NULL;
  proj = ProjectNew ();
  if (proj == NULL) return NULL;
  pip = ValNodeNew (NULL);
  if (pip == NULL) return NULL;
  if (sfp->uids != NULL) {
    switch (sfp->currDb) {
      case 1 :
        is_na = FALSE;
        break;
      case 2 :
      case 4 :
        is_na = TRUE;
        break;
      default :
        return NULL;
    }
  } else if (sfp->seqentry != NULL) {
    sep = sfp->seqentry [0];
    if (sep == NULL || (! IS_Bioseq (sep))) return NULL;
    bsp = (BioseqPtr) sep->data.ptrvalue;
    if (bsp == NULL) return NULL;
    is_na = (Boolean) (ISA_na (bsp->mol));
  } else return NULL;
  if (is_na) {
    choice = ProjectItem_nucent;
  } else {
    choice = ProjectItem_protent;
  }
  pip->choice = choice;
  proj->data = pip;
  for (j = 0; j < sfp->numUids; j++) {
    sep = NULL;
    if (sfp->uids != NULL) {
      uid = sfp->uids [j];
      bsp = BioseqLockByGi (uid);
      if (bsp != NULL && bsp->repr == Seq_repr_raw) {
        tmp = BioseqNew ();
        if (tmp != NULL) {
          SeqIdWrite (bsp->id, buf, PRINTID_FASTA_LONG, sizeof (buf) - 1);
          ptr = buf;
          ch = *ptr;
          while (ch != '\0') {
            if (ch == '|') {
              *ptr = '/';
            }
            ptr++;
            ch = *ptr;
          }
          oid = ObjectIdNew ();
          oid->str = StringSave (buf);
          sip = ValNodeNew (NULL);
          sip->choice = SEQID_LOCAL;
          sip->data.ptrvalue = (Pointer) oid;
          tmp->id = sip;
          tmp->repr = bsp->repr;
          tmp->mol = bsp->mol;
          tmp->length = bsp->length;
          tmp->seq_data_type = bsp->seq_data_type;
          tmp->seq_data = BSDup (bsp->seq_data);
          sep = SeqEntryNew ();
          if (sep != NULL) {
            sep->choice = 1;
            sep->data.ptrvalue = (Pointer) tmp;
            SeqMgrSeqEntry (SM_BIOSEQ, (Pointer) tmp, sep);
          }
        }
      }
      BioseqUnlockByGi (uid);
    } else if (sfp->seqentry != NULL) {
      sep = AsnIoMemCopy ((Pointer) sfp->seqentry [j],
                          (AsnReadFunc) SeqEntryAsnRead,
                          (AsnWriteFunc) SeqEntryAsnWrite);
    }
    if (sep != NULL) {
      ValNodeLink ((ValNodePtr PNTR) (&(pip->data.ptrvalue)), sep);
    }
  }
  return (Pointer) proj;
  */
  return NULL;
}

static Boolean SaveDocsumProject (BaseFormPtr bfp, Boolean saveAs)

{
  AsnIoPtr     aop;
  Char         path [PATH_MAX];
  ProjectPtr   proj;
  SummFormPtr  sfp;
#ifdef WIN_MAC
  FILE         *fp;
#endif

  if (bfp != NULL) {
    sfp = (SummFormPtr) bfp;
    path [0] = '\0';
    StringNCpy_0 (path, bfp->filepath, sizeof (path));
    if (StringHasNoText (path) || saveAs) {
      if (! (GetOutputFileName (path, sizeof (path), ""))) return FALSE;
    }
#ifdef WIN_MAC
    fp = FileOpen (path, "r");
    if (fp != NULL) {
      FileClose (fp);
    } else {
      FileCreate (path, "TEXT", "ttxt");
    }
#endif
    WatchCursor ();
    Update ();
    proj = (ProjectPtr) FormToPointer (bfp->form);
    if (proj == NULL) {
      ArrowCursor ();
      Update ();
      Message (MSG_ERROR, "Unable to create project data.");
      return FALSE;
    }
    aop = AsnIoOpen (path, "w");
    if (aop != NULL) {
      ProjectAsnWrite (proj, aop, NULL);
      AsnIoFlush (aop);
      AsnIoClose (aop);
      bfp->filepath = MemFree (bfp->filepath);
      bfp->filepath = StringSave (path);
      ArrowCursor ();
      Update ();
      return TRUE;
    } else {
      ArrowCursor ();
      Update ();
      Message (MSG_ERROR, "Unable to write file.");
      return FALSE;
    }
  }
  return FALSE;
}

static void EntrezDocSumFormMessage (ForM f, Int2 mssg)

{
  BaseFormPtr       bfp;
  EntrezGlobalsPtr  egp;
  SummFormPtr       sfp;

  bfp = (BaseFormPtr) GetObjectExtra (f);
  if (bfp != NULL) {
    switch (mssg) {
      case VIB_MSG_REDRAW :
        egp = (EntrezGlobalsPtr) GetAppProperty ("EntrezGlobals");
        if (egp != NULL) {
          sfp = (SummFormPtr) bfp;
          sfp->docsumFont = egp->docsumFont;
          RepopulateDocSum (sfp, TRUE);
        }
        break;
      case VIB_MSG_COPY :
        CopyDocsumToClipboard (bfp);
        break;
      case VIB_MSG_IMPORT :
        ImportDocSumForm (f, NULL);
        break;
      case VIB_MSG_EXPORT :
        ExportDocSumForm (f, NULL);
        break;
      case VIB_MSG_PRINT :
        PrintDocsumProc (bfp);
        break;
      case VIB_MSG_SAVE_AS :
        SaveDocsumProject (bfp, TRUE);
        break;
      default :
        if (bfp->appmessage != NULL) {
          bfp->appmessage (f, mssg);
        }
        break;
    }
  }
}

static void EntrezDocSumFormActivate (WindoW w)

{
  SummFormPtr  sfp;

  sfp = (SummFormPtr) GetObjectExtra (w);
  if (sfp != NULL) {
    if (sfp->activate != NULL) {
      sfp->activate (w);
    }
    SetDocSumImportExportItems (sfp);
  }
}

static void CleanupEntrezDocSumForm (GraphiC g, VoidPtr data)

{
  Int2         j;
  SummFormPtr  sfp;

  sfp = (SummFormPtr) data;
  if (sfp != NULL) {
    MemFree (sfp->uids);
    if (sfp->simple != NULL) {
      for (j = 0; j < sfp->numUids; j++) {
        SimpleSeqFree (sfp->simple [j]);
      }
      sfp->simple = MemFree (sfp->simple);
    }
    MemFree (sfp->state);
    MemFree (sfp->neighbors);
    FileRemove (sfp->historyFile);
    MemFree (sfp->historyFile);
    MemFree (sfp->historyOffsets);
    if (sfp->dbalist != NULL) {
      for (j = 0; sfp->dbalist [j].name != NULL; j++) {
        MemFree (sfp->dbalist [j].name);
      }
    }
    MemFree (sfp->dbalist);
  }
  StdCleanupFormProc (g, data);
}

static void ResizeDocSumForm (WindoW w)

{
  Int2         diff;
  Int2         gap;
  Int2         height;
  Int2         quarter;
  RecT         r;
  Int2         remaining;
  RecT         s;
  SummFormPtr  sfp;
  RecT         t;
  Int2         width;

  sfp = (SummFormPtr) GetObjectExtra (w);
  if (sfp == NULL) return;
  WatchCursor ();
  ObjectRect (w, &r);
  width = r.right - r.left;
  height = r.bottom - r.top;
  GetPosition (sfp->docsum, &s);
  GetPosition (sfp->controls, &t);
  diff = t.bottom - t.top;
  gap = t.top - s.bottom;
  t.bottom = height - s.left;
  t.top = t.bottom - diff;
  s.right = width - s.left;
  /*
  s.bottom = height - s.left;
  */
  s.bottom = t.top - gap;
  SafeHide (sfp->controls);
  SetPosition (sfp->controls, &t);
  AdjustPrnt (sfp->controls, &t, FALSE);
  SetPosition (sfp->docsum, &s);
  AdjustPrnt (sfp->docsum, &s, FALSE);
  ObjectRect (sfp->docsum, &s);
  InsetRect (&s, 4, 4);
  docsumColFmt [0].pixInset = 6 * stdCharWidth;
  remaining = s.right - s.left - 6 * stdCharWidth;
  quarter = remaining / 4;
  docsumColFmt [0].pixWidth = quarter + 6 * stdCharWidth;
  docsumColFmt [1].pixWidth = remaining - quarter;
  /*
  if (Visible (sfp->docsum) && AllParentsVisible (sfp->docsum)) {
    UpdateDocument (sfp->docsum, 0, 0);
  }
  */
  SafeShow (sfp->controls);
  RepopulateDocSum (sfp, TRUE);
  ArrowCursor ();
  Update ();
}

static void SetDocSumImportExportItems (SummFormPtr sfp)

{
  IteM          exportItm;
  IteM          importItm;
  CharPtr PNTR  labels;
  Boolean       lastChoiceIsUidList = FALSE;
  Char          tmp [64];
  Int2          val;

  if (sfp == NULL) return;
  importItm = FindFormMenuItem ((BaseFormPtr) sfp, VIB_MSG_IMPORT);
  if (importItm != NULL) {
    SafeSetTitle (importItm, "Import FASTA or Uid List...");
  }
  exportItm = FindFormMenuItem ((BaseFormPtr) sfp, VIB_MSG_EXPORT);
  if (exportItm == NULL) return;
  sfp->label = medRadios [0];
  if (sfp->simple != NULL) {
    sfp->label = localBioseqRadios [0];
  } else if (sfp->currDb >= 0 && sfp->currDb < MAX_DBS) {
    labels = radioLabels [sfp->currDb];
    val = GetValue (sfp->formats [sfp->currDb]);
    if (val > 0) {
      sfp->label = labels [val - 1];
      if (labels [val] == NULL) {
        lastChoiceIsUidList = TRUE;
      }
    }
  }
  StringCpy (tmp, "Export ");
  StringCat (tmp, sfp->label);
  if (lastChoiceIsUidList) {
    StringCat (tmp, " List");
  }
  StringCat (tmp, "...");
  SafeSetTitle (exportItm, tmp);
}

extern ForM CreateDocSumForm (Int2 left, Int2 top, CharPtr title,
                              WndActnProc activate, FormMessageFunc messages)

{
  ButtoN            b1, b2, b3;
  EntrezGlobalsPtr  egp;
  EntrezInfoPtr     eip;
  GrouP             g;
  GrouP             h;
  Int2              i;
  Int2              j;
  GrouP             k;
  CharPtr PNTR      labels;
  Boolean           macLike;
  PrompT            ppt, ppt1, ppt2;
  GrouP             q;
  Int2              quarter;
  RecT              r;
  Int2              remaining;
  SummFormPtr       sfp;
  Int2              typ_ml;
  WindoW            w;
  GrouP             x;
  GrouP             y;

  w = NULL;
  eip = EntrezGetInfo ();
  if (eip == NULL || eip->type_info == NULL ||
      eip->types == NULL || eip->field_info == NULL) return NULL;
  egp = (EntrezGlobalsPtr) GetAppProperty ("EntrezGlobals");
  if (egp == NULL) return NULL;
  macLike = egp->popdownBehavior;
  sfp = (SummFormPtr) MemNew (sizeof (SummFormData));
  if (sfp != NULL) {
    w = DocumentWindow (left, top, -10, -10, title,
                        StdSendCloseWindowMessageProc, ResizeDocSumForm);
    SetObjectExtra (w, sfp, CleanupEntrezDocSumForm);
    sfp->form = (ForM) w;
    sfp->toform = ProjectPtrToDocSumForm;
    sfp->fromform = DocSumFormToProjectPtr;
    sfp->formmessage = EntrezDocSumFormMessage;
    sfp->importform = ImportDocSumForm;
    sfp->exportform = ExportDocSumForm;

    sfp->appmessage = messages;
    sfp->activate = activate;
    SetActivate (w, EntrezDocSumFormActivate);

    sfp->docsumFont = egp->docsumFont;
    if (sfp->docsumFont == NULL) {
      sfp->docsumFont = programFont;
    }

    if (egp->createDocSumMenus != NULL) {
      egp->createDocSumMenus (w);
    }

    h = HiddenGroup (w, -1, 0, NULL);

    k = HiddenGroup (h, 2, 0, NULL);
    ppt = StaticPrompt (k, "Format:", 0, 0, programFont, 'l');
    q = HiddenGroup (k, 0, 0, NULL);
    for (i = 0; i < MAX_DBS && radioLabels [i] != NULL; i++) {
      sfp->formats [i] = HiddenGroup (q, 5, 0, ChangeFormat);
      SetObjectExtra (sfp->formats [i], sfp, NULL);
      labels = radioLabels [i];
      for (j = 0; labels [j] != NULL; j++) {
        RadioButton (sfp->formats [i], labels [j]);
      }
      SetValue (sfp->formats [i], 1);
      Hide (sfp->formats [i]);
    }
    sfp->formats [MAX_DBS] = HiddenGroup (q, 5, 0, NULL);
    SetObjectExtra (sfp->formats [MAX_DBS], sfp, NULL);
    labels = localBioseqRadios;
    for (j = 0; labels [j] != NULL; j++) {
       RadioButton (sfp->formats [MAX_DBS], labels [j]);
    }
    SetValue (sfp->formats [MAX_DBS], 1);
    Hide (sfp->formats [MAX_DBS]);
    sfp->label = medRadios [0];
    AlignObjects (ALIGN_MIDDLE, (HANDLE) ppt, (HANDLE) q, NULL);

    g = HiddenGroup (h, -1, 0, NULL);
    SetGroupSpacing (g, 5, 5);

    sfp->dbalist = MakeDatabaseAlist (eip->type_info, eip->type_count, FALSE);

    sfp->docsum = DocumentPanel (g, 32 * stdCharWidth, 20 * stdLineHeight);
    SetObjectExtra (sfp->docsum, sfp, NULL);
    SetDocProcs (sfp->docsum, ClickDocSum, NULL, ReleaseDocSum, NULL);
    SetDocCache (sfp->docsum, NULL, NULL, NULL);
    Hide (sfp->docsum);

    sfp->controls = HiddenGroup (g, 5, 0, NULL);
    SetGroupSpacing (sfp->controls, 10, 10);

    x = HiddenGroup (sfp->controls, 0, 2, NULL);
    sfp->retrieve = PushButton (x, "Neighbor 00000", RetrieveNeighbors);
    SetObjectExtra (sfp->retrieve, sfp, NULL);
    SetTitle (sfp->retrieve, "Neighbor 0");
    Disable (sfp->retrieve);
    sfp->refine = PushButton (x, "Refine 0000", RefineProc);
    SetObjectExtra (sfp->refine , sfp, NULL);
    SetTitle (sfp->refine, "Refine");
    Disable (sfp->refine);
    sfp->retrieveMode = FETCH_MODE;
    sfp->usingDelay = FALSE;

    sfp->neighbors = NULL;
    sfp->numNeighbors = 0;

    sfp->historyFile = NULL;
    sfp->historyOffsets = NULL;
    sfp->generations = 0;
    sfp->present = 0;

    x = HiddenGroup (sfp->controls, 0, -3, NULL);
    SetGroupMargins (x, 3, 0);
    SetGroupSpacing (x, 3, 5);

    y = HiddenGroup (x, 5, 0, NULL);
    ppt1 = StaticPrompt (y, "Target:", 0, popupMenuHeight, programFont, 'l');
    sfp->target = PopupList (y, macLike, ChangeTarget);
    SetObjectExtra (sfp->target, sfp, NULL);
    InitEnumPopup (sfp->target, sfp->dbalist, NULL);
    typ_ml = DatabaseFromName ("MEDLINE");
    SetEnumPopup (sfp->target, sfp->dbalist, (UIEnum) typ_ml);

    y = HiddenGroup (x, 4, 0, NULL);
    SetGroupSpacing (y, 8, 0);
    ppt2 = StaticPrompt  (y, "Select:", 0, 0, programFont, 'l');
    b1 = PushButton (y, "All", SelectAllProc);
    SetObjectExtra (b1, sfp, NULL);
    b2 = PushButton (y, "None", ClearAllProc);
    SetObjectExtra (b2, sfp, NULL);
    b3 = PushButton (y, "Parents", SelectParentsProc);
    SetObjectExtra (b3, sfp, NULL);

    x = HiddenGroup (sfp->controls, 0, 2, NULL);
    sfp->prevBtn = PushButton (x, "Prev", PrevProc);
    SetObjectExtra (sfp->prevBtn, sfp, NULL);
    Disable (sfp->prevBtn);
    sfp->nextBtn = PushButton (x, "Next", NextProc);
    SetObjectExtra (sfp->nextBtn, sfp, NULL);
    Disable (sfp->nextBtn);

    AlignObjects (ALIGN_MIDDLE, (HANDLE) sfp->retrieve, (HANDLE) ppt1,
                  (HANDLE) sfp->target, (HANDLE) sfp->prevBtn, NULL);
    AlignObjects (ALIGN_MIDDLE, (HANDLE) sfp->refine , (HANDLE) ppt2, (HANDLE) b1,
                  (HANDLE) b2, (HANDLE) b3, (HANDLE) sfp->nextBtn, NULL);

    RealizeWindow (w);

    SelectFont (programFont);
    sfp->lineHeight = LineHeight ();
    SelectFont (systemFont);
    sfp->linesPerIcon = (22 + sfp->lineHeight - 1) / sfp->lineHeight;
    docsumParFmt.minLines = sfp->linesPerIcon;

    ObjectRect (sfp->docsum, &r);
    InsetRect (&r, 4, 4);
    docsumColFmt [0].pixInset = 6 * stdCharWidth;
    remaining = r.right - r.left - 6 * stdCharWidth;
    quarter = remaining / 4;
    docsumColFmt [0].pixWidth = quarter + 6 * stdCharWidth;
    docsumColFmt [1].pixWidth = remaining - quarter;
    textColFmt [0].pixInset = 3 * stdCharWidth;
    textColFmt [0].pixWidth = screenRect.right - screenRect.left;

    SetDocAutoAdjust (sfp->docsum, FALSE);
  }
  return (ForM) w;
}

typedef struct linkgroup {
  DIALOG_MESSAGE_BLOCK
  BaseFormPtr        bfp;
  ButtoN             retrieve;
  PopuP              target;
  LinkSetPtr         lsp;
  Int2               currDb;
  Int2               targetDb;
  Int4               docuid;
  Uint2              align_type;
  ButtoN             onlyFromThis;
  EnumFieldAssocPtr  dbalist;
} LinkGroup, PNTR LinkGroupPtr;

static void RetrieveViewerLinks (ButtoN b)

{
  BaseFormPtr       bfp;
  BioseqPtr         bsp;
  EntrezGlobalsPtr  egp;
  ValNodePtr        head;
  Int2              i;
  LinkGroupPtr      lgp;
  LinkSetPtr        lsp;
  Int2              num;
  Int2              parents;
  Boolean           persist;
  SeqEntryPtr       sep;
  Int4Ptr           uids;
  ValNodePtr        vnp;

  lgp = (LinkGroupPtr) GetObjectExtra (b);
  if (lgp == NULL) return;
  bfp = (BaseFormPtr) GetObjectExtra (ParentWindow (b));
  if (bfp == NULL) return;
  egp = (EntrezGlobalsPtr) GetAppProperty ("EntrezGlobals");
  if (egp == NULL || egp->retrieveDocsProc == NULL) return;
  if (lgp->align_type != 0) {
    uids = NULL;
    sep = NULL;
    if (GetStatus (lgp->onlyFromThis)) {
      bsp =  GetBioseqGivenIDs (bfp->input_entityID, bfp->input_itemID, bfp->input_itemtype);
      if (bsp != NULL) {
        sep = SeqMgrGetSeqEntryForData (bsp);
      }
    } else {
      sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
    }
    head = GetUidsForSeqEntryAligns (sep);
    num = 0;
    for (vnp = head; vnp != NULL; vnp = vnp->next) {
      if (vnp->choice == lgp->align_type && vnp->data.intvalue != lgp->docuid) {
        num++;
      }
    }
    if (num > 0) {
      uids = MemNew ((size_t) (num + 1) * sizeof (DocUid));
      if (uids != NULL) {
        i = 0;
        for (vnp = head; i < num && vnp != NULL; vnp = vnp->next) {
          if (vnp->choice == lgp->align_type && vnp->data.intvalue != lgp->docuid) {
            uids [i] = vnp->data.intvalue;
            i++;
          }
        }
      }
      egp->retrieveDocsProc (bfp->form, num, 0, uids, lgp->targetDb);
      MemFree (uids);
    }
    ValNodeFree (head);
    return;
  }
  lsp = lgp->lsp;
  if (lsp == NULL) return;
  persist = FALSE;
  if (lgp->currDb == lgp->targetDb) {
    if (egp->parentsPersist != NULL) {
      persist = GetStatus (egp->parentsPersist);
    } else {
      persist = egp->persistDefault;
    }
  }
  parents = 0;
  if (persist) {
    parents++;
  }
  uids = NULL;
  num = lsp->num + parents;
  if (num > 0) {
    uids = MemNew ((size_t) (num + 1) * sizeof (DocUid));
    if (uids != NULL) {
      num = 0;
      if (persist) {
        uids [num] = lgp->docuid;
        num++;
      }
      for (i = 0; i < lsp->num; i++) {
        uids [num] = lsp->uids [i];
        num++;
      }
    }
  }
  egp->retrieveDocsProc (bfp->form, num, parents, uids, lgp->targetDb);
  MemFree (uids);
}

static void ChangeViewerLinkData (LinkGroupPtr lgp, Int2 currDb, Int4 theuid)

{
  BaseFormPtr  bfp;
  BioseqPtr    bsp;
  ValNodePtr   head = NULL;
  Int4         num;
  SeqEntryPtr  sep;
  SeqIdPtr     sip;
  Char         str [64];
  Char         title [32];
  Int4         uid;
  UIEnum       val;
  ValNode      vn;
  ValNodePtr   vnp;

  if (lgp == NULL) return;
  if (lgp->lsp != NULL) {
    lgp->lsp = LinkSetFree (lgp->lsp);
  }
  lgp->align_type = 0;
  if (GetEnumPopup (lgp->target, lgp->dbalist, &val)) {
    lgp->targetDb = (Int2) val;
    if (lgp->targetDb >= ALIST_BLASTN && lgp->targetDb <= ALIST_TBLASTX) {
      /* SafeShow (lgp->onlyFromThis); */
      lgp->docuid = theuid;
      lgp->currDb = currDb;
      num = 0;
      bfp = lgp->bfp;
      if (bfp != NULL) {
        sep = NULL;
        if (GetStatus (lgp->onlyFromThis)) {
          bsp =  GetBioseqGivenIDs (bfp->input_entityID, bfp->input_itemID, bfp->input_itemtype);
          if (bsp != NULL) {
            sep = SeqMgrGetSeqEntryForData (bsp);
          }
        } else {
          sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
        }
        head = GetIdStringsForSeqEntryAligns (sep);
        lgp->targetDb = TYP_NT;
        switch (val) {
          case ALIST_BLASTN :
            lgp->align_type = ALIGN_BLASTN;
            lgp->targetDb = TYP_NT;
            break;
          case ALIST_BLASTP :
            lgp->align_type = ALIGN_BLASTP;
            lgp->targetDb = TYP_AA;
            break;
          case ALIST_BLASTX :
            lgp->align_type = ALIGN_BLASTX;
            lgp->targetDb = TYP_AA;
            break;
          case ALIST_TBLASTN :
            lgp->align_type = ALIGN_TBLASTN;
            lgp->targetDb = TYP_NT;
            break;
          default :
            break;
        }
        vn.choice = SEQID_GI;
        vn.data.intvalue = theuid;
        sip = (SeqIdPtr) (&vn);
        SeqIdWrite (sip, str, PRINTID_REPORT, sizeof (str));
        for (vnp = head; vnp != NULL; vnp = vnp->next) {
          if (vnp->choice == lgp->align_type && StringICmp ((CharPtr) vnp->data.ptrvalue, str) != 0) {
            num++;
          }
        }
        ValNodeFreeData (head);
      }
      sprintf (title, "Retrieve %ld", (long) num);
      SafeSetTitle (lgp->retrieve, title);
      if (num > 0) {
        SafeEnable (lgp->retrieve);
      } else {
        SafeDisable (lgp->retrieve);
     }
      return;
    } else {
      /* SafeHide (lgp->onlyFromThis); */
    }
  } else {
    lgp->targetDb = currDb;
  }
  lgp->currDb = currDb;
  lgp->docuid = theuid;
  if (theuid > 0) {
    lgp->lsp = NULL;
    uid = theuid;
    EntrezLinkUidList (&(lgp->lsp), lgp->currDb, lgp->targetDb, 1, &uid, FALSE);
  }
  num = 0;
  if (lgp->lsp != NULL) {
    num = (Int4) lgp->lsp->num;
  }
  if (lgp->targetDb == lgp->currDb) {
    sprintf (title, "Neighbor %ld", (long) num);
  } else {
    sprintf (title, "Lookup %ld", (long) num);
  }
  SafeSetTitle (lgp->retrieve, title);
  if (num > 0) {
    SafeEnable (lgp->retrieve);
  } else {
    SafeDisable (lgp->retrieve);
  }
}

extern void UpdateViewerLinkTarget (GrouP g)

{
  BaseFormPtr   bfp;
  Int2          currDb;
  LinkGroupPtr  lgp;
  Int4          uid = 0;
  UIEnum        val;

  lgp = (LinkGroupPtr) GetObjectExtra (g);
  if (lgp == NULL) return;
  bfp = lgp->bfp;
  if (bfp == NULL) return;
  currDb = bfp->doctype;
  uid = bfp->docuid;
  if (GetEnumPopup (lgp->target, lgp->dbalist, &val)) {
    if (! (val >= ALIST_BLASTN && val <= ALIST_TBLASTX)) {
      SetEnumPopup (lgp->target, lgp->dbalist, (UIEnum) currDb);
    }
  }
  ChangeViewerLinkData (lgp, currDb, uid);
}

static void ChangeViewerLinkTarget (GraphiC g)

{
  BaseFormPtr   bfp;
  Int2          currDb;
  LinkGroupPtr  lgp;
  Int4          uid = 0;

  lgp = (LinkGroupPtr) GetObjectExtra (g);
  if (lgp == NULL) return;
  bfp = lgp->bfp;
  if (bfp == NULL) return;
  currDb = bfp->doctype;
  uid = bfp->docuid;
  ChangeViewerLinkData (lgp, currDb, uid);
}

static void CleanupLinkGroup (GraphiC g, VoidPtr data)

{
  Int2          j;
  LinkGroupPtr  lgp;

  lgp = (LinkGroupPtr) data;
  if (lgp != NULL) {
    LinkSetFree (lgp->lsp);
    if (lgp->dbalist != NULL) {
      for (j = 0; lgp->dbalist [j].name != NULL; j++) {
        MemFree (lgp->dbalist [j].name);
      }
    }
    MemFree (lgp->dbalist);
  }
  MemFree (data);
}

extern GrouP MakeViewerLinkControls (GrouP prnt, BaseFormPtr bfp, Int2 doctype, Int4 uid, Boolean blast)

{
  EntrezGlobalsPtr  egp;
  EntrezInfoPtr     eip;
  LinkGroupPtr      lgp;
  Boolean           macLike;
  GrouP             p;
  PrompT            ppt;

  if (prnt == NULL || bfp == NULL) return NULL;

  eip = EntrezGetInfo ();
  if (eip == NULL || eip->type_info == NULL ||
      eip->types == NULL || eip->field_info == NULL) return NULL;
  egp = (EntrezGlobalsPtr) GetAppProperty ("EntrezGlobals");
  if (egp == NULL) return NULL;
  macLike = egp->popdownBehavior;

  p = HiddenGroup (prnt, 5, 0, NULL);
  SetGroupSpacing (p, 10, 10);

  lgp = (LinkGroupPtr) MemNew (sizeof (LinkGroup));
  if (lgp == NULL) return p;

  SetObjectExtra (p, lgp, CleanupLinkGroup);
  lgp->dialog = (DialoG) p;

  lgp->bfp = bfp;
  lgp->lsp = NULL;
  lgp->dbalist = MakeDatabaseAlist (eip->type_info, eip->type_count, blast);

  lgp->retrieve = PushButton (p, "Neighbor 00000", RetrieveViewerLinks);
  SetObjectExtra (lgp->retrieve, lgp, NULL);
  SetTitle (lgp->retrieve, "Neighbor 0");
  Disable (lgp->retrieve);

  ppt = StaticPrompt (p, "Target:", 0, popupMenuHeight, programFont, 'l');
  lgp->target = PopupList (p, macLike, (PupActnProc) ChangeViewerLinkTarget);
  SetObjectExtra (lgp->target, lgp, NULL);
  InitEnumPopup (lgp->target, lgp->dbalist, NULL);
  SetEnumPopup (lgp->target, lgp->dbalist, (UIEnum) doctype);

  lgp->onlyFromThis = CheckBox (p, "Just from this sequence", (BtnActnProc) ChangeViewerLinkTarget);
  SetObjectExtra (lgp->onlyFromThis, lgp, NULL);
  SetStatus (lgp->onlyFromThis, TRUE);
  SafeHide (lgp->onlyFromThis);

  AlignObjects (ALIGN_MIDDLE, (HANDLE) lgp->retrieve, (HANDLE) ppt,
                (HANDLE) lgp->target, (HANDLE) lgp->onlyFromThis, NULL);

  ChangeViewerLinkData (lgp, doctype, uid);

  return p;
}

static void LaunchMedlineViewer (Int4 uid, Int2 numAlign, Int4Ptr alignuids, Int2 db)

{
  Pointer          dataptr;
  Uint2            datatype;
  Uint2            entityID;
  Int2             handled;
  MedlineEntryPtr  mep;

  WatchCursor ();
  Update ();
  mep = EntrezMedlineEntryGet (uid);
  if (mep == NULL) {
    ArrowCursor ();
    Update ();
    Message (MSG_OK, "Unable to find this record in the database.");
    return;
  }
  datatype = OBJ_MEDLINE_ENTRY;
  dataptr = (Pointer) mep;
  entityID = ObjMgrRegister (datatype, dataptr);
  if (dataptr == NULL || entityID == 0) {
    ArrowCursor ();
    Update ();
    Message (MSG_OK, "Unable to find this record in the database.");
    return;
  }
  handled = GatherProcLaunch (OMPROC_VIEW, FALSE, entityID, 1,
                              OBJ_MEDLINE_ENTRY, 0, 0, OBJ_MEDLINE_ENTRY, 0);
  ArrowCursor ();
  if (handled != OM_MSG_RET_DONE || handled == OM_MSG_RET_NOPROC) {
    Message (MSG_FATAL, "Unable to launch viewer.");
    MedlineEntryFree (mep);
  } else {
    ObjMgrSetOptions (OM_OPT_FREE_IF_NO_VIEW, entityID);
  }
}

static void AddBlastAlignment (Int4 uid, Int2 numAlign, Int4Ptr alignuids, Uint2 entityID, SeqEntryPtr sep, BioseqPtr subject)

{
  Uint1                align_type = 0;
  BioseqPtr            bsp;
  BioseqSetPtr         bssp;
  SeqAnnotPtr          curr;
  Int2                 i;
  BLAST_OptionsBlkPtr  options = NULL;
  SeqAlignPtr          prev;
  BioseqPtr            query;
  SeqAlignPtr          salp;
  SeqAnnotPtr          sap;
  SeqAnnotPtr PNTR     sapp;
  BlastSearchBlkPtr    search;

  if (sep == NULL || numAlign == 0 || alignuids == NULL) return;
  sap = NULL;
  salp = NULL;
  if (subject == NULL) return;
  if (ISA_na (subject->mol)) {
    align_type = 1;
    options = BLASTOptionNew ("blastn", TRUE);
    if (options != NULL) {
      options->gapped_calculation = TRUE;
      options->db_length = 100000000;
#ifdef WIN16
      options->wordsize = 10;
#else
      options->wordsize = 12;
#endif
    }
  } else if (ISA_aa (subject->mol)) {
    align_type = 2;
    options = BLASTOptionNew ("blastp", TRUE);
    if (options != NULL) {
      options->gapped_calculation = TRUE;
      options->db_length = 20000000;
      options->threshold_second = 12;
    }
  } else return;
  search = BLASTSetUpSearch (subject, options->program_name, 0, 0, NULL, options, NULL);

  for (i = 0; i < numAlign; i++) {
    if (alignuids [i] != uid) {
      query = BioseqLockByGi (alignuids [i]);
      if (query != NULL) {
        salp = BlastSequencesOnTheFly (search, query);
        BioseqUnlockByGi (alignuids [i]);
        if (salp != NULL) {
          if (sap == NULL) {
            sap = SeqAnnotNew ();
            if (sap != NULL) {
              sap->type = 2;
            }
          }
          if (sap != NULL) {
            if (sap->data != NULL) {
              prev = sap->data;
              while (prev->next != NULL) {
                prev = prev->next;
              }
              prev->next = salp;
            } else {
              sap->data = (Pointer) salp;
            }
          }
        }
      }
    }
  }
  BLASTOptionDelete (options);
  BlastSearchBlkDestruct (search);

  if (sap == NULL) return;

  AddAlignInfoToSeqAnnot (sap, align_type);
  /*
  ObjMgrRegister (OBJ_SEQANNOT, (Pointer) sap);
  */
  sapp = NULL;
  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    sapp = &(bsp->annot);
  } else if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    sapp = &(bssp->annot);
  }
  if (sapp != NULL) {
    if (*sapp != NULL) {
      curr = *sapp;
      while (curr->next != NULL) {
        curr = curr->next;
      }
      curr->next = sap;
    } else {
      *sapp = sap;
    }
  }
}

/*
static Int2 GetSequenceComplexity (void)

{
  Int2              retcode = 0;
  EntrezGlobalsPtr  egp;
  Int2              val;

  egp = (EntrezGlobalsPtr) GetAppProperty ("EntrezGlobals");
  if (egp == NULL || egp->seqComplex == NULL) return retcode;
  val = GetValue (egp->seqComplex);
  switch (val) {
    case 1 :
      retcode = -2;
      break;
    case 2 :
      retcode = SEQENTRY_READ_SEG_SET;
      break;
    case 3 :
      retcode = SEQENTRY_READ_BIOSEQ;
     break;
    default :
      retcode = 0;
      break;
  }
  return retcode;
}
*/

static void LaunchSequenceViewer (Int4 uid, Int2 numAlign, Int4Ptr alignuids, Int2 db)

{
  BioseqPtr         bsp;
  EntrezGlobalsPtr  egp;
  Uint2             entityID;
  Int2              handled;
  Uint2             itemID;
  SeqEntryPtr       sep;

  WatchCursor ();
  Update ();
  SeqEntrySetScope (NULL);
  /* retcode = GetSequenceComplexity (); */
  bsp = BioseqLockByGi (uid);
  if (bsp == NULL) {
    ArrowCursor ();
    Update ();
    Message (MSG_OK, "Unable to find this record in the database.");
    return;
  }
  entityID = BioseqFindEntityByGi (uid, &itemID);
  if (entityID == 0 || itemID == 0) {
    BioseqUnlockByGi (uid);
    ArrowCursor ();
    Update ();
    Message (MSG_OK, "BioseqFindEntityByGi failed.");
    return;
  }
  egp = (EntrezGlobalsPtr) GetAppProperty ("EntrezGlobals");
  if (egp != NULL) {
    if (GetStatus (egp->alignWithChecked)) {
      sep = GetTopSeqEntryForEntityID (entityID);
      AddBlastAlignment (uid, numAlign, alignuids, entityID, sep, bsp);
    }
  }
  handled = GatherProcLaunch (OMPROC_VIEW, FALSE, entityID, itemID,
                              OBJ_BIOSEQ, 0, 0, OBJ_BIOSEQ, 0);
  ArrowCursor ();
  if (handled != OM_MSG_RET_DONE || handled == OM_MSG_RET_NOPROC) {
    Message (MSG_FATAL, "Unable to launch viewer.");
  } else {
    ObjMgrSetOptions (OM_OPT_FREE_IF_NO_VIEW, entityID);
  }
  BioseqUnlockByGi (uid);
}

static void LaunchGenomeViewer (Int4 uid, Int2 numAlign, Int4Ptr alignuids, Int2 db)

{
  Pointer      dataptr;
  Uint2        datatype;
  Uint2        entityID;
  Int2         handled;
  Uint2        itemID;
  SeqEntryPtr  sep;

  WatchCursor ();
  Update ();
  sep = EntrezSeqEntryGet (uid, -1);
  if (sep == NULL) {
    ArrowCursor ();
    Update ();
    Message (MSG_OK, "Unable to find this record in the database.");
    return;
  }
  if (IS_Bioseq (sep)) {
    datatype = OBJ_BIOSEQ;
  } else if (IS_Bioseq_set (sep)) {
    datatype = OBJ_BIOSEQSET;
  } else {
    ArrowCursor ();
    Update ();
    Message (MSG_OK, "Unable to find this record in the database.");
    return;
  }
  dataptr = (Pointer) sep->data.ptrvalue;
  entityID = ObjMgrRegister (datatype, dataptr);
  if (dataptr == NULL || entityID == 0) {
    ArrowCursor ();
    Update ();
    Message (MSG_OK, "Unable to find this record in the database.");
    return;
  }
  itemID = 1;
  handled = GatherProcLaunch (OMPROC_VIEW, FALSE, entityID, itemID,
                              OBJ_BIOSEQ, 0, 0, OBJ_BIOSEQ, 0);
  ArrowCursor ();
  if (handled != OM_MSG_RET_DONE || handled == OM_MSG_RET_NOPROC) {
    Message (MSG_FATAL, "Unable to launch viewer.");
    SeqEntryFree (sep);
  } else {
    ObjMgrSetOptions (OM_OPT_FREE_IF_NO_VIEW, entityID);
  }
}

static Int4 GetBiostrucComplexity (void)

{
  Int4              complexity = ALLMDL;
  EntrezGlobalsPtr  egp;
  Int2              val;

  egp = (EntrezGlobalsPtr) GetAppProperty ("EntrezGlobals");
  if (egp == NULL) return complexity;
  val = GetValue (egp->strucComplex);
  switch (val) {
    case 1 :
      complexity = ONECOORDATOM;
      break;
    case 2 :
      complexity = ONECOORDRES;
      break;
    case 3 :
      complexity = ALLMDL;
     break;
    case 4 :
      complexity = VECMODEL;
      break;
    case 5 :
      complexity = BSEVERYTHING;
      break;
    default :
      complexity = ONECOORDATOM;
      break;
  }
  return complexity;
}

static Int2 GetBiostrucMaxModels (void)

{
  EntrezGlobalsPtr  egp;
  Int2              maxModels = INT2_MAX;
  Int2              val;

  egp = (EntrezGlobalsPtr) GetAppProperty ("EntrezGlobals");
  if (egp == NULL) return maxModels;
  val = GetValue (egp->strucModels);
  switch (val) {
    case 1 :
      maxModels = 1;
      break;
    case 2 :
      maxModels = 2;
      break;
    case 3 :
      maxModels = 5;
      break;
    case 4 :
      maxModels = 10;
      break;
    case 5 :
      maxModels = 15;
      break;
    case 6 :
      maxModels = 20;
      break;
    case 7:
      maxModels = MAX_MDLNO;
      break;
    default :
      maxModels = 1;
      break;
  }
  return maxModels;
}

static void LaunchStructureViewer (Int4 uid, Int2 numAlign, Int4Ptr alignuids, Int2 db)

{
#if defined(WIN16)
  Message (MSG_OK, "Structure views not supported on this platform.");
#else
  AsnIoPtr         aip;
  BiostrucPtr      bsp;
  Int4             complexity;
  EntrezGeneral    eg;
  Int2             maxModels;
  NcbiMimeAsn1     mime;
  Char             path [PATH_MAX];
  ValNode          vn;
#ifdef WIN_MOTIF
  Char             cmmd [PATH_MAX];
#endif
#ifdef WIN_MSWIN
  Char             prog [PATH_MAX];
#endif

  if (! BiostrucAvail ()) {
    Message (MSG_OK, "Structure libraries are not linked in.");
    return;
  }
  WatchCursor ();
  Update ();
  complexity = GetBiostrucComplexity ();
  maxModels = GetBiostrucMaxModels ();
  bsp = EntrezBiostrucGet (uid, complexity, maxModels);
  if (bsp == NULL) {
    ArrowCursor ();
    Update ();
    Message (MSG_OK, "Unable to find this record in the database.");
    return;
  }

  TmpNam (path);
  aip = AsnIoOpen (path, "w");

  MemSet ((Pointer) &vn, 0, sizeof (ValNode));
  vn.choice = Data_data_structure;
  vn.data.ptrvalue = (Pointer) bsp;
  vn.next = NULL;

  MemSet ((Pointer) &eg, 0, sizeof (EntrezGeneral));
  eg.style = Entrez_style_asn1;
  eg.Data_data = &vn;

  MemSet ((Pointer) &mime, 0, sizeof (NcbiMimeAsn1));
  mime.choice = NcbiMimeAsn1_entrez;
  mime.data.ptrvalue = &eg;
  mime.next = NULL;

  NcbiMimeAsn1AsnWrite (&mime, aip, NULL);
  /*
  BiostrucAsnWrite (bsp, aip, NULL);
  */
  AsnIoClose (aip);

  BiostrucFree (bsp);

  /* Cn3D expects Ncbi-mime-asn1, not Biostruc */

#ifdef WIN_MAC
  Nlm_SendOpenDocAppleEvent (path, "Cn3D");
#endif
#ifdef WIN_MSWIN
  Nlm_GetExecPath ("valfile", prog, sizeof (prog));
  Nlm_MSWin_OpenApplication (prog, path);
#endif
#ifdef WIN_MOTIF
  /*
  argv [0] = path;
  argv [1] = NULL;
  if (! Execv ("Cn3D", argv)) {
    Message (MSG_POST, "Unable to launch Cn3D");
  }
  */
  sprintf (cmmd, "Cn3D %s &", path);
  system (cmmd);
#endif

  ArrowCursor ();
#endif
}

extern void LaunchRecordViewer (Int4 uid, Int2 numAlign, Int4Ptr alignuids, Int2 db)

{
  Int2  typ_aa;
  Int2  typ_ch;
  Int2  typ_ml;
  Int2  typ_nt;
  Int2  typ_st;

  if (uid == 0) return;
  typ_ml = DatabaseFromName ("MEDLINE");
  typ_aa = DatabaseFromName ("Protein");
  typ_nt = DatabaseFromName ("Nucleotide");
  typ_st = DatabaseFromName ("Structure");
  typ_ch = DatabaseFromName ("Genome");
  if (db == typ_ml) {
    LaunchMedlineViewer (uid, numAlign, alignuids, db);
  } else if (db == typ_aa || db == typ_nt) {
    LaunchSequenceViewer (uid, numAlign, alignuids, db);
  } else if (db == typ_st) {
    LaunchStructureViewer (uid, numAlign, alignuids, db);
  } else if (db == typ_ch) {
    LaunchGenomeViewer (uid, numAlign, alignuids, db);
  }
}

typedef struct prefspage {
  DIALOG_MESSAGE_BLOCK
  ButtoN             persist;
  ButtoN             align;
  GrouP              lookup;
  ButtoN             showAsn;
  PopuP              initDb;
  PopuP              initFld;
  PopuP              initMode;
  PopuP              initMed;
  PopuP              initNuc;
  PopuP              initProt;
  PopuP              initGenome;
  TexT               minWidth;
  TexT               minHeight;
  EnumFieldAssocPtr  dbalist;
  EnumFieldAssocPtr  fldalist;
  EnumFieldAssocPtr  nucalist;
  EnumFieldAssocPtr  protalist;
  EnumFieldAssocPtr  gnomalist;
} PrefsPage, PNTR PrefsPagePtr;

static ENUM_ALIST(all_mode_alist)
  {"Automatic",   1},
  {"Lookup",      2},
  {"MeSH Tree",   3},
  {"Range",       4},
  {"Selection",   5},
  {"Taxonomy",    6},
  {"Truncation",  7},
END_ENUM_ALIST

static ENUM_ALIST(med_views_alist)
  {"Abstract",    1},
  {"Citation",    2},
  {"MEDLINE",     3},
  {"ASN.1",       4},
END_ENUM_ALIST

static void PrefsPtrToPrefsPage (DialoG d, Pointer data)

{
  EntrezPrefsPtr  epp;
  PrefsPagePtr    ppp;

  ppp = (PrefsPagePtr) GetObjectExtra (d);
  if (ppp == NULL) return;
  epp = (EntrezPrefsPtr) data;
  if (epp != NULL) {
    SetStatus (ppp->persist, epp->persistDefault);
    SetStatus (ppp->align, epp->alignDefault);
    if (epp->lookupDirect) {
      SetValue (ppp->lookup, 2);
    } else {
      SetValue (ppp->lookup, 1);
    }
    SetStatus (ppp->showAsn, epp->showAsn);
    SetEnumPopupByName (ppp->initDb, ppp->dbalist, epp->initDatabase);
    SetEnumPopupByName (ppp->initFld, ppp->fldalist, epp->initField);
    SetEnumPopupByName (ppp->initMode, all_mode_alist, epp->initMode);
    SetEnumPopupByName (ppp->initMed, med_views_alist, epp->initMedLabel);
    SetEnumPopupByName (ppp->initNuc, ppp->nucalist, epp->initNucLabel);
    SetEnumPopupByName (ppp->initProt, ppp->protalist, epp->initProtLabel);
    SetEnumPopupByName (ppp->initGenome, ppp->gnomalist, epp->initGenomeLabel);
  } else {
    SetStatus (ppp->persist, TRUE);
    SetStatus (ppp->align, TRUE);
    SetValue (ppp->lookup, 1);
    SetStatus (ppp->showAsn, FALSE);
    SetEnumPopupByName (ppp->initDb, ppp->dbalist, "MEDLINE");
    SetEnumPopupByName (ppp->initFld, ppp->fldalist, "All Fields");
    SetEnumPopupByName (ppp->initMode, all_mode_alist, "Automatic");
    SetEnumPopupByName (ppp->initMed, med_views_alist, "Abstract");
    SetEnumPopupByName (ppp->initNuc, ppp->nucalist, "GenBank");
    SetEnumPopupByName (ppp->initProt, ppp->protalist, "GenPept");
    SetEnumPopupByName (ppp->initGenome, ppp->gnomalist, "Map");
  }
}

static Pointer PrefsPageToPrefsPtr (DialoG d)

{
  EntrezPrefsPtr  epp;
  PrefsPagePtr    ppp;

  ppp = (PrefsPagePtr) GetObjectExtra (d);
  if (ppp == NULL) return NULL;
  epp = (EntrezPrefsPtr) MemNew (sizeof (EntrezPrefs));
  if (epp == NULL) return NULL;
  epp->persistDefault = GetStatus (ppp->persist);
  epp->alignDefault = GetStatus (ppp->align);
  epp->lookupDirect = (GetValue (ppp->lookup) == 2);
  epp->showAsn = GetStatus (ppp->showAsn);
  epp->initDatabase = GetEnumPopupByName (ppp->initDb, ppp->dbalist);
  epp->initField = GetEnumPopupByName (ppp->initFld, ppp->fldalist);
  epp->initMode = GetEnumPopupByName (ppp->initMode, all_mode_alist);
  epp->initMedLabel = GetEnumPopupByName (ppp->initMed, med_views_alist);
  epp->initNucLabel = GetEnumPopupByName (ppp->initNuc, ppp->nucalist);
  epp->initProtLabel = GetEnumPopupByName (ppp->initProt, ppp->protalist);
  epp->initGenomeLabel = GetEnumPopupByName (ppp->initGenome, ppp->gnomalist);
  return (Pointer) epp;
}

static EnumFieldAssocPtr MakePageSpecAlist (SeqViewProcsPtr svpp, Boolean nucOK,
                                            Boolean protOK, Boolean genomeOK)

{
  EnumFieldAssocPtr  alist;
  EnumFieldAssocPtr  ap;
  BioseqPagePtr      bpp;
  Int2               i;

  if (svpp == NULL) return NULL;
  bpp = svpp->pageSpecs;
  i = 0;
  while (bpp != NULL) {
    i++;
    bpp = bpp->next;
  }
  alist = MemNew (sizeof (EnumFieldAssoc) * (i + 2));
  if (alist == NULL) return NULL;
  ap = alist;
  bpp = svpp->pageSpecs;
  i = 0;
  while (bpp != NULL) {
    if ((nucOK && bpp->nucOK) ||
        (protOK && bpp->protOK) ||
        (genomeOK && bpp->genomeOK)) {
      ap->name = StringSave (bpp->label);
      ap->value = i;
      ap++;
      i++;
    }
    bpp = bpp->next;
  }
  ap->name = NULL;
  return alist;
}

extern EntrezPrefsPtr EntrezPrefsNew (void)

{
  EntrezPrefsPtr  epp;

  epp = (EntrezPrefsPtr) MemNew (sizeof (EntrezPrefs));
  return epp;
}

extern EntrezPrefsPtr EntrezPrefsFree (EntrezPrefsPtr epp)

{
  if (epp == NULL) return NULL;
  epp->initDatabase = MemFree (epp->initDatabase);
  epp->initField = MemFree (epp->initField);
  epp->initMode = MemFree (epp->initMode);
  epp->initMedLabel = MemFree (epp->initMedLabel);
  epp->initNucLabel = MemFree (epp->initNucLabel);
  epp->initProtLabel = MemFree (epp->initProtLabel);
  epp->initGenomeLabel = MemFree (epp->initGenomeLabel);
  return NULL;
}

static void CleanupEntrezPrefsProc (GraphiC g, VoidPtr data)

{
  PrefsPagePtr  ppp;

  ppp = (PrefsPagePtr) data;
  if (ppp != NULL) {
    ppp->dbalist = FreeEnumFieldAlist (ppp->dbalist);
    ppp->fldalist = FreeEnumFieldAlist (ppp->fldalist);
    ppp->nucalist = FreeEnumFieldAlist (ppp->nucalist);
    ppp->protalist = FreeEnumFieldAlist (ppp->protalist);
    ppp->gnomalist = FreeEnumFieldAlist (ppp->gnomalist);
  }
  StdCleanupExtraProc (g, data);
}

extern DialoG CreateEntrezPrefsDialog (GrouP prnt, CharPtr title)

{
  EntrezGlobalsPtr  egp;
  EntrezInfoPtr     eip;
  GrouP             g1, g2, g3, g4, g5;
  Boolean           macLike;
  GrouP             p;
  PrefsPagePtr      ppp;
  GrouP             s;
  SeqViewProcsPtr   svpp;

  p = HiddenGroup (prnt, 1, 0, NULL);
  SetGroupSpacing (p, 10, 10);

  eip = EntrezGetInfo ();
  if (eip == NULL || eip->type_info == NULL ||
      eip->types == NULL || eip->field_info == NULL) return (DialoG) p;

  egp = (EntrezGlobalsPtr) GetAppProperty ("EntrezGlobals");
  if (egp == NULL) return (DialoG) p;
  macLike = egp->popdownBehavior;

  svpp = (SeqViewProcsPtr) GetAppProperty ("SeqDisplayForm");
  if (svpp == NULL) return (DialoG) p;

  ppp = (PrefsPagePtr) MemNew (sizeof (PrefsPage));
  if (ppp != NULL) {

    SetObjectExtra (p, ppp, CleanupEntrezPrefsProc);
    ppp->dialog = (DialoG) p;
    ppp->todialog = PrefsPtrToPrefsPage;
    ppp->fromdialog = PrefsPageToPrefsPtr;

    ppp->dbalist = MakeDatabaseAlist (eip->type_info, eip->type_count, FALSE);
    ppp->fldalist = MakeFieldAlist (eip->types, eip->field_info, eip->field_count, 0, TRUE);

    ppp->nucalist = MakePageSpecAlist (svpp, TRUE, FALSE, FALSE);
    ppp->protalist = MakePageSpecAlist (svpp, FALSE, TRUE, FALSE);
    ppp->gnomalist = MakePageSpecAlist (svpp, FALSE, FALSE, TRUE);

    if (title != NULL && title [0] != '\0') {
      s = NormalGroup (p, -1, 0, title, systemFont, NULL);
    } else {
      s = HiddenGroup (p, -1, 0, NULL);
    }
    SetGroupSpacing (s, 10, 10);

    g1 = HiddenGroup (s, -1, 0, NULL);
    g5 = HiddenGroup (g1, 3, 0, NULL);
    SetGroupSpacing (g5, 10, 3);
    ppp->persist = CheckBox (g5, "Parents Persist", NULL);
    ppp->align = CheckBox (g5, "Align Checked", NULL);
    ppp->showAsn = CheckBox (g5, "Show ASN.1 Report", NULL);
    g2 = HiddenGroup (g1, 2, 0, NULL);
    StaticPrompt (g2, "Lookup Mode Launches", 0, stdLineHeight, programFont, 'l');
    ppp->lookup = HiddenGroup (g2, 2, 0, NULL);
    RadioButton (ppp->lookup, "DocSum");
    RadioButton (ppp->lookup, "Record");
    AlignObjects (ALIGN_CENTER, (HANDLE) g2, (HANDLE) g5, NULL);

    g3 = HiddenGroup (s, 0, 2, NULL);

    StaticPrompt (g3, "Database", 0, 0, programFont, 'l');
    ppp->initDb = PopupList (g3, macLike, NULL);
    InitEnumPopup (ppp->initDb, ppp->dbalist, NULL);
    SetEnumPopupByName (ppp->initDb, ppp->dbalist, "MEDLINE");

    StaticPrompt (g3, "Field", 0, 0, programFont, 'l');
    ppp->initFld = PopupList (g3, macLike, NULL);
    InitEnumPopup (ppp->initFld, ppp->fldalist, NULL);
    SetEnumPopupByName (ppp->initFld, ppp->fldalist, "All Fields");

    StaticPrompt (g3, "Mode", 0, 0, programFont, 'l');
    ppp->initMode = PopupList (g3, macLike, NULL);
    InitEnumPopup (ppp->initMode, all_mode_alist, NULL);
    SetEnumPopupByName (ppp->initMode, all_mode_alist, "Automatic");

    g4 = HiddenGroup (s, 0, 2, NULL);

    StaticPrompt (g4, "MEDLINE", 0, 0, programFont, 'l');
    ppp->initMed = PopupList (g4, macLike, NULL);
    InitEnumPopup (ppp->initMed, med_views_alist, NULL);
    SetEnumPopupByName (ppp->initMed, med_views_alist, "Abstract");

    StaticPrompt (g4, "Nucleotide", 0, 0, programFont, 'l');
    ppp->initNuc = PopupList (g4, macLike, NULL);
    InitEnumPopup (ppp->initNuc, ppp->nucalist, NULL);
    SetEnumPopupByName (ppp->initNuc, ppp->nucalist, "GenBank");

    StaticPrompt (g4, "Protein", 0, 0, programFont, 'l');
    ppp->initProt = PopupList (g4, macLike, NULL);
    InitEnumPopup (ppp->initProt, ppp->protalist, NULL);
    SetEnumPopupByName (ppp->initProt, ppp->protalist, "GenPept");

    StaticPrompt (g4, "Genome", 0, 0, programFont, 'l');
    ppp->initGenome = PopupList (g4, macLike, NULL);
    InitEnumPopup (ppp->initGenome, ppp->gnomalist, NULL);
    SetEnumPopupByName (ppp->initGenome, ppp->gnomalist, "Map");

    AlignObjects (ALIGN_CENTER, (HANDLE) g1, (HANDLE) g3, (HANDLE) g4, NULL);
  }

  return (DialoG) p;
}

