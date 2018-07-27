/*   $Id: samutil.c,v 1.76 2005/04/26 21:33:52 kans Exp $
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
* File Name:  $Id: samutil.c,v 1.76 2005/04/26 21:33:52 kans Exp $
*
* Author:  Lewis Geer
*
* Version Creation Date:   8/12/99
*
* $Revision: 1.76 $
*
* File Description: Utility functions for AlignIds and SeqAlignLocs
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: samutil.c,v $
* Revision 1.76  2005/04/26 21:33:52  kans
* added SEQID_GPIPE
*
* Revision 1.75  2001/10/03 00:15:47  vakatov
* Replaced some ASSERTs by VERIFYs
*
* Revision 1.74  2001/08/07 17:22:52  kans
* added support for third party annotation SeqIDs
*
* Revision 1.73  2001/05/25 19:05:36  vakatov
* Nested comment typo fixed
*
* Revision 1.72  2001/02/07 16:51:36  hurwitz
* bug fix
*
* Revision 1.71  2001/02/06 20:52:01  hurwitz
* memory leak fix
*
* Revision 1.70  2001/01/26 23:26:17  hurwitz
* bug fix for DDE_GetMsaTxtNodeGivenBioseqCoord
*
* Revision 1.69  2001/01/24 23:04:01  hurwitz
* added a couple utility functions
*
* Revision 1.68  2001/01/10 23:38:39  lewisg
* fix seqid and various memory leaks
*
* Revision 1.67  2001/01/03 22:16:52  hurwitz
* fix potential memory leak
*
* Revision 1.66  2000/12/29 00:43:20  hurwitz
* got rid of some asserts
*
* Revision 1.65  2000/11/01 20:50:21  hurwitz
* made functions for get block starts and stops from populated paraGs
*
* Revision 1.64  2000/10/16 23:45:02  hurwitz
* fixed DDE_GetStart for case of no left tail
*
* Revision 1.63  2000/10/05 21:27:42  hurwitz
* bug fix for making ruler, added functions to get bioseq start and len of each aligned block
*
* Revision 1.62  2000/09/08 21:47:57  hurwitz
* added DDE_GetNumResidues function
*
* Revision 1.61  2000/08/25 22:10:32  hurwitz
* added utility function
*
* Revision 1.60  2000/08/24 19:15:46  hurwitz
* bug fix: undo and redo functions cause quit query dialog
*
* Revision 1.59  2000/08/23 20:06:49  hurwitz
* fixes to DDE_PopListFree
*
* Revision 1.58  2000/08/07 23:02:39  hurwitz
* when merging panels fails, scroll to spot where alignments differ
*
* Revision 1.57  2000/08/03 17:12:37  hurwitz
* added functions to check if alignments are mergeable
*
* Revision 1.56  2000/07/17 17:49:12  hurwitz
* fixed bug.  when copying ParaG's need to copy all rows so edits can be saved properly
*
* Revision 1.55  2000/07/05 19:23:12  lewisg
* add two panes to ddv, update msvc project files
*
* Revision 1.54  2000/07/05 18:42:16  hurwitz
* added split block function to DDV
*
* Revision 1.53  2000/06/30 22:31:51  hurwitz
* added merge block function for DDV
*
* Revision 1.52  2000/06/22 20:56:50  hurwitz
* assorted bug fixes
*
* Revision 1.51  2000/06/21 21:20:56  hurwitz
* a couple bug fixes
*
* Revision 1.50  2000/06/15 20:26:05  hurwitz
* added left/right/center justify for DDE
*
* Revision 1.49  2000/06/08 20:04:39  hurwitz
* made warning about converting to true multiple alignment into a Message window, and other small fixes
*
* Revision 1.48  2000/06/07 19:09:35  hurwitz
* made DDE_ReMakeRuler work with linked list of ParaGs
*
* Revision 1.47  2000/05/31 23:07:26  hurwitz
* made NoGaps a runtime parameter, fixed bug with vertical scroll of show/hide list, save edits query is not performed if nothing to save
*
* Revision 1.46  2000/05/25 21:40:42  hurwitz
* rows hidden in DDV are hidden in DDE, can save edits when rows are hidden in DDE
*
* Revision 1.45  2000/05/19 13:48:32  hurwitz
* made a version of DDE that doesn't allow aligned gaps, changed wording for adding new rows
*
* Revision 1.44  2000/05/16 19:43:02  hurwitz
* grey out create block, delete block, undo, and redo as needed
*
* Revision 1.43  2000/05/12 21:18:13  hurwitz
* added window asking if user wants to save unsaved edits for dde
*
* Revision 1.42  2000/05/10 21:54:54  hurwitz
* free memory when DDE is closed
*
* Revision 1.41  2000/05/08 16:28:25  wheelan
* fix SAM_ReplaceGI for dense-diag alignments
*
* Revision 1.40  2000/05/05 20:24:14  hurwitz
* some bug fixes, also redraw proper block in DDE after a save operation that causes a merge of 2 blocks
*
* Revision 1.39  2000/05/04 22:43:38  hurwitz
* don't launch DDE on top of DDV, change some wording, redraw DDE after save to AlnMgr
*
* Revision 1.38  2000/05/02 19:50:37  hurwitz
* fixed some bugs with launching DDE from DDV, added new alnMgr fn for positioning DDE on proper column
*
* Revision 1.37  2000/04/26 21:53:22  hurwitz
* added save function to tell AlnMgr about edits made in DDE
*
* Revision 1.36  2000/04/21 23:00:51  hurwitz
* can launch DDE from DDV
*
* Revision 1.35  2000/04/13 22:03:32  hurwitz
* a couple more small bug fixes
*
* Revision 1.34  2000/04/13 18:57:03  hurwitz
* for DDE: many bug fixes, also get rid of columns that are all unaligned gaps
*
* Revision 1.33  2000/04/10 20:58:42  hurwitz
* added GUI controls for DeleteBlock in DDE
*
* Revision 1.32  2000/04/07 16:21:08  hurwitz
* made delete block faster, added delete block to edit menu
*
* Revision 1.31  2000/04/05 20:52:35  hurwitz
* added GUI control for shifting left and right alignment boundaries
*
* Revision 1.30  2000/04/03 22:26:31  hurwitz
* can now shift a row with click and drag
*
* Revision 1.29  2000/03/29 20:02:47  hurwitz
* keep track of master during move row operations
*
* Revision 1.28  2000/03/25 00:22:09  hurwitz
* put DDE_StackPtr in DDV_Main, add to stack inside DDE api's, added insert char, delete char, home and end keyboard control
*
* Revision 1.27  2000/03/23 00:00:00  hurwitz
* DDE api's are called with stack now
*
* Revision 1.26  2000/03/20 22:22:48  hurwitz
* added more checks to verify subroutine, 1 bug fix
*
* Revision 1.25  2000/03/16 15:51:31  hurwitz
* added function to create an aligned block
*
* Revision 1.24  2000/03/14 22:08:21  hurwitz
* undo and redo working properly, restore-original function added
*
* Revision 1.23  2000/03/10 23:01:43  hurwitz
* added undo and redo functions, first pass
*
* Revision 1.22  2000/03/09 22:28:40  hurwitz
* added shift block and delete block, a bug fix too
*
* Revision 1.21  2000/03/08 22:02:07  hurwitz
* added verify function, debugging, handle align_start != 0
*
* Revision 1.20  2000/03/06 22:45:58  hurwitz
* can shift right boundary of an aligned block left and right, DDVRuler updates added
*
* Revision 1.19  2000/03/01 22:49:40  lewisg
* import bioseq, neatlyindex, get rid of dead code
*
* Revision 1.18  2000/02/29 21:13:06  hurwitz
* added low level functions for shifting left and right the left alignment boundary
*
* Revision 1.17  2000/02/28 16:28:39  hurwitz
* added functions for deleting an aligned gap
*
* Revision 1.16  2000/02/24 23:37:00  hurwitz
* added ability to insert gaps
*
* Revision 1.15  2000/02/18 16:06:22  hurwitz
* for editing multiple sequence alignments: shift row right now working
*
* Revision 1.14  2000/02/15 17:43:49  hurwitz
* reverted to 1.12
*
* Revision 1.12  2000/02/14 23:09:08  hurwitz
* got rid of calls to DDV_RulerDescrNew() because of library conflicts
*
* Revision 1.11  2000/02/14 20:58:57  hurwitz
* added functions for editing multiple sequence alignments: hide/show row, move row, shift row left
*
* Revision 1.10  2000/01/24 20:54:34  vakatov
* SAM_ViewString::  made #define to fix for the DLL build on PC
*
* Revision 1.9  2000/01/24 16:11:13  lewisg
* speed up seqid comparison in color manager, make fast windows version of SetColor()
*
* Revision 1.8  1999/12/11 01:30:34  lewisg
* fix bugs with sharing colors between ddv and cn3d
*
* Revision 1.7  1999/12/03 23:17:24  lewisg
* Patrick's new global update msg, argument passing when launching ddv, experimental editing
*
* Revision 1.6  1999/11/24 21:24:30  vakatov
* Fixed for the C++ and/or MSVC DLL compilation
*
* Revision 1.5  1999/10/05 23:18:15  lewisg
* add ddv and udv to cn3d with memory management
*
* Revision 1.4  1999/09/27 17:53:08  kans
* seqalign entityID/itemID/itemtype now in GatherIndex substructure
*
* Revision 1.3  1999/09/27 17:49:12  lewisg
* fix denseseg constructor, bug in valnode loops, add SAM_ValNodeByPosition
*
* Revision 1.2  1999/09/21 19:33:53  lewisg
* fix broken declarations
*
* Revision 1.1  1999/09/21 18:09:14  lewisg
* binary search added to color manager, various bug fixes, etc.
*
* Revision 1.4  1999/09/03 23:27:32  lewisg
* minor speedups by avoiding casts
*
* Revision 1.3  1999/09/03 14:01:40  lewisg
* use faster seqid compare SAM_CompareID
*
* Revision 1.2  1999/09/01 23:02:59  lewisg
* binary search in color functions
*
* Revision 1.1  1999/08/13 22:08:16  lewisg
* color manager updated to use alignment coords
*
*
* ==========================================================================
*/

#include <samutil.h>
#include <sequtil.h>

/*****************************************************************************

Function: SAM_ReplaceGI()

Purpose: Tries to replace all gi's in a seqalign with the best accession ID's
  
Parameters: salp, the seqalign to be worked over.

Returns: This function only works on a non-indexed list of seqaligns

*****************************************************************************/

NLM_EXTERN void SAM_ReplaceGI(SeqAlign *salp)
{
    DenseSegPtr dsp;
    DenseDiagPtr ddp;
    SeqId *sip, *sipBest, *sipPrev, *wholesip;
    Int4 gi;
    
    for(;salp != NULL; salp = salp->next) {
        if (salp->segtype == SAS_DENSEG) {
            dsp = (DenseSegPtr)(salp->segs);
            sipPrev = NULL;
            for(sip = dsp->ids; sip != NULL; sipPrev = sip, sip = sip->next) {
                gi = GetGIForSeqId(sip);
                wholesip = GetSeqIdForGI(gi);
                if(wholesip == NULL) continue;
                sipBest = SeqIdFindBestAccession(wholesip);
                if(sipBest == NULL) {
                    SeqIdFree(wholesip);
                    continue;
                }
                sipBest = SeqIdDup(sipBest);

                if(sipPrev == NULL) dsp->ids = sipBest;
                else sipPrev->next = sipBest;

                sipBest->next = sip->next;
                sip->next = NULL;
                SeqIdFree(sip);
                SeqIdFree(wholesip);
                sip = sipBest;
            }

        } else if (salp->segtype == SAS_DENDIAG) {
            ddp = (DenseDiagPtr)(salp->segs);
            while (ddp != NULL)
            {
                sipPrev = NULL;
                for(sip = ddp->id; sip != NULL; sipPrev = sip, sip = sip->next) {
                    gi = GetGIForSeqId(sip);
                    wholesip = GetSeqIdForGI(gi);
                    if(wholesip == NULL) continue;
                    sipBest = SeqIdFindBestAccession(wholesip);
                    if(sipBest == NULL) {
                        SeqIdFree(wholesip);
                        continue;
                    }
                    sipBest = SeqIdDup(sipBest);

                    if(sipPrev == NULL) ddp->id = sipBest;
                    else sipPrev->next = sipBest;

                    sipBest->next = sip->next;
                    sip->next = NULL;
                    SeqIdFree(sip);
                    SeqIdFree(wholesip);
                    sip = sipBest;
                }
                ddp = ddp->next;
            }
        }
    }
}



/*****************************************************************************

Function: SAM_ExtractSips()

Purpose: Return a ValNode list containing a SeqId for each Bioseq
         contained in a SeqEntry.
  
Parameters: sep, pointer to the SeqEntry to explore

Returns: ValNode list of pointers to SeqId's.  Do NOT deallocate, these are
         not duplicates!

*****************************************************************************/

typedef struct _SAM_ExtractStruc {
    ValNode *sips;
} SAM_ExtractStruc;

static void SAM_ExtractSipsCallback(SeqEntryPtr sep, Pointer mydata,
                            Int4 index, Int2 indent)
{
    Bioseq *bsp;
    SAM_ExtractStruc *pExtract;

    pExtract = (SAM_ExtractStruc *) mydata;   
    if(sep == NULL || pExtract == NULL) return;
    bsp = (BioseqPtr) sep->data.ptrvalue;
    if(bsp == NULL) return;
    ValNodeAddPointer(&pExtract->sips, 0, bsp->id);
}

ValNode * SAM_ExtractSips(SeqEntry *sep)
{
    SAM_ExtractStruc Extract;
    
    Extract.sips = NULL;
    BioseqExplore(sep, (void *)&Extract, SAM_ExtractSipsCallback);
    
    return Extract.sips;
}


/*****************************************************************************

Function: SAM_MakeViewerFree()

Purpose: Set an object to use OM_OPT_FREE_IF_NO_VIEW flag.
  
Parameters: data, pointer to the object to be flagged.

Returns: 1 if OK, 0 otherwise.

*****************************************************************************/

NLM_EXTERN Int4 SAM_MakeViewerFree (void *data)
{
    ObjMgrData *omdp;
    ObjMgr *omp;
    Uint2 entityID;
    Uint2 options;

    omp = ObjMgrReadLock();
    if(omp == NULL) ThrowError;
    omdp = ObjMgrFindByData(omp, data);
    if(omdp == NULL) ThrowError;
    if(!ObjMgrUnlock()) ThrowError;
    entityID = omdp->EntityID;
    options = ObjMgrGetOptions(entityID);
    options |= OM_OPT_FREE_IF_NO_VIEW;
    ObjMgrSetOptions(options, entityID);

    return 1;

error:
    ErrPostEx(SEV_ERROR, 0, 0, "Error");
    return 0;
}

/*****************************************************************************

Function: SAM_MakeTemp()

Purpose: Make an object temporary loaded.
  
Parameters: data, pointer to the object to be flagged.

Returns: 1 if OK, 0 otherwise.

*****************************************************************************/

NLM_EXTERN Int4 SAM_MakeTemp (void *data)
{
    ObjMgr *omp;
 
    omp = ObjMgrWriteLock();
    if(omp == NULL) ThrowError;
    if(!ObjMgrSetTempLoad (omp, data)) ThrowError;
    if(!ObjMgrUnlock()) ThrowError;

    return 1;

error:
    ErrPostEx(SEV_ERROR, 0, 0, "Error");
    return 0;
}

/*****************************************************************************

Function: SAM_ValNodePut()

Purpose: Put a ValNode in a list of ValNodes by cardinal order.
  
Parameters: ppvnHead, pointer to head of ValNode list
            Num, the number of the ValNode to insert before.  begins with 0.
            pvnInsert, the valnode to insert

Returns: 1 if OK, 0 otherwise.

Notes: Hangs on loops that don't include the head.

*****************************************************************************/

NLM_EXTERN Int4 SAM_ValNodePut
(ValNode **ppvnHead, ValNode *pvnInsert, Int4 Num)
{
    ValNode *pvn, *pvnPrevious = NULL;
    Boolean First = TRUE;
    Int4 i;
    
    if(ppvnHead == NULL || pvnInsert == NULL) return 0;
    
    for(pvn = *ppvnHead, i = 0; pvn != NULL && i != Num;
            pvn = pvn->next, i++) {
        if(!First && pvn == *ppvnHead) return 0;  /* loop */
        First = FALSE;
        pvnPrevious = pvn;
    }

    if(pvn == NULL && i != Num) return 0;  /* nobody home */
    if(pvn == NULL && i == Num) {  /* at the end */
        pvnPrevious->next = pvnInsert;
        pvnInsert->next = NULL;
        return 1;
    }
    if(i == 0) { /* at the beginning */
        pvnInsert->next = *ppvnHead;
        *ppvnHead = pvnInsert;
        return 1;
    }
    /* somewhere in the middle */
    pvnPrevious->next = pvnInsert;
    pvnInsert->next = pvn;
    return 1;
}

/*****************************************************************************

Function: SAM_ValNodeExtract()

Purpose: Takes a ValNode out of a list of ValNodes by cardinal order.
  
Parameters: ppvnHead, pointer to head of ValNode list
            Num, the number of the ValNode to extract (the 2nd, 3rd, etc.)

Returns: The extracted ValNode. NULL otherwise.

Notes: Hangs on loops that don't include the head.

*****************************************************************************/

NLM_EXTERN ValNode * SAM_ValNodeExtract(ValNode **ppvnHead, Int4 Num)
{
    ValNode *pvn, *pvnPrevious = NULL;
    Boolean First = TRUE;
    Int4 i;
    
    if(ppvnHead == NULL) return NULL;
    
    for(pvn = *ppvnHead, i = 0; pvn != NULL; pvn = pvn->next, i++) {
        if(!First && pvn == *ppvnHead) return NULL;  /* loop */
        First = FALSE;
        if (i == Num) break;
        pvnPrevious = pvn;
    }
    if(pvn == NULL) return NULL;  /* nobody home */
    
    if(pvnPrevious != NULL) pvnPrevious->next = pvn->next;
    else *ppvnHead = pvn->next;
    
    pvn->next = NULL;
    return pvn;
}

/*****************************************************************************

Function: SAM_ValNodeByPosition()

Purpose: Return pointer to ValNode in a list of ValNodes by cardinal order.
  
Parameters: ppvnHead, pointer to head of ValNode list
            Num, the number of the ValNode to get (the 2nd, 3rd, etc.).
                 starts at 0.

Returns: The ValNode at position NUM. NULL otherwise.

Notes: Hangs on loops that don't include the head.

*****************************************************************************/

NLM_EXTERN ValNode * SAM_ValNodeByPosition(ValNode **ppvnHead, Int4 Num)
{
    ValNode *pvn;
    Boolean First = TRUE;
    Int4 i;
    
    if(ppvnHead == NULL) return NULL;
    
    for(pvn = *ppvnHead, i = 0; pvn != NULL; pvn = pvn->next, i++) {
        if(!First && pvn == *ppvnHead) return NULL;  /* loop */
        First = FALSE;
        if (i == Num) break;
    }
    return pvn;
}


/*****************************************************************************

Function: SAM_SeqAlignExtract()

Purpose: Takes a SeqAlign out of a list of SeqAligns.  The extracted SeqAlign
         is the first one pointer matches the passed SeqAlign pointer.
  
Parameters: psalpHead, pointer to head of SeqAlign list
            salpCheck, points to the SeqAlign to be extracted

Returns: The extracted SeqAlign. NULL otherwise

Notes: Hangs on loops that don't include the head.

*****************************************************************************/

NLM_EXTERN SeqAlign * SAM_SeqAlignExtract
(SeqAlign **psalpHead, SeqAlign *salpCheck)
{
    SeqAlign *salp, *salpPrevious = NULL;
    Boolean fFirst = TRUE;
    
    if(psalpHead == NULL) return NULL;
    
    for(salp = *psalpHead; salp != NULL; salp = salp->next) {
        if(!fFirst && salp == *psalpHead) return NULL;  /* loop */
        fFirst = FALSE;
        if (salp == salpCheck) break;
        salpPrevious = salp;
    }
    if(salp == NULL) return NULL;  /* nobody home */
    
    if(salpPrevious != NULL) salpPrevious->next = salp->next;
    else *psalpHead = salp->next;
    
    salp->next = NULL;
    return salp;
}

/*****************************************************************************
*
*   Adds a SeqAlign newnode to the end of a SeqAlign chain started by head.
*   
*   If the head is NULL, makes the newnode the head.
*   Returns the head of the SeqAlign chain, otherwise returns NULL on error.
*
*****************************************************************************/

NLM_EXTERN SeqAlign * SAM_Add2SeqAlign(SeqAlign ** head, SeqAlign *newnode)
{   
    SeqAlign *salp;
    
    if (head == NULL)
        return NULL;
    salp = *head;
    if (salp != NULL )   {
        while (salp->next != NULL) salp = salp->next;
        salp->next = newnode;
    }
    else
        *head = newnode;
    return *head;
}

/*****************************************************************************
*
*   Frees a list of SeqId's.  Returns the remaining SeqId * if fails,
*   Otherwise NULL. 
*
*****************************************************************************/

NLM_EXTERN SeqId * SAM_FreeSeqIdSet(SeqId *sip)
{
    for(;sip != NULL; sip = sip->next) {
        if(SeqIdFree(sip) != NULL) return sip;
    }
    return NULL;
}

/*****************************************************************************
*
*   Retrieves SeqIds from a set of seqlocs, duplicates the first SeqId, then
*   appends it to a list of SeqId's, which is returned.  
*
*   Returns NULL if the seqlocs are of any type other that SEQLOC_INT or the
*   SeqLoc doesn't contain a SeqId.
*
*****************************************************************************/

NLM_EXTERN SeqId * SAM_SeqIdFromSeqLoc(SeqLoc *slp, Int4 * NumSeqs)
{
    SeqInt *pSeqInt;
    SeqId *sip, *sipHead;

    sip = NULL;
    sipHead = NULL;
    *NumSeqs = 0;
    for(; slp != NULL; slp = slp->next) {

        if(slp->choice != SEQLOC_INT) goto error;

        pSeqInt = (SeqInt *)slp->data.ptrvalue;
        if (pSeqInt == NULL || pSeqInt->id == NULL) goto error;
        
        sip = SeqIdDup(pSeqInt->id);
        if(sip == NULL) goto error;
        sip->next = NULL;
        
        ValNodeLink(&sipHead, sip);
        (*NumSeqs)++;
    }    
    return sipHead;

error:
    ErrPostEx(SEV_ERROR, 0, 0, 
        "SAM_SeqIdFromSeqLoc: Error");
    SAM_FreeSeqIdSet(sipHead);
    return NULL;
}


/*****************************************************************************

Function: SAM_NewDenseSeg()

Purpose: Constructs a new DenseSeg, all arrays initialized to sizes based on
          NumSeqs and NumSegs

Parameters: NumSeqs, the number of sequences
            NumSegs, the number of Segments
            Strands, if TRUE, initialize the strands array.

Returns: The new DenseSeg. NULL otherwise.

*****************************************************************************/

NLM_EXTERN DenseSeg *SAM_NewDenseSeg
(Int4 NumSeqs, Int4 NumSegs, Boolean Strands)
{
    DenseSeg *pDenseSeg = NULL;

    pDenseSeg = DenseSegNew();
    if(pDenseSeg == NULL) goto error;

    pDenseSeg->starts = MemNew(sizeof(Int4)*NumSeqs*NumSegs);
    pDenseSeg->lens = MemNew(sizeof(Int4)*NumSegs);
    if(pDenseSeg->starts == NULL || pDenseSeg->lens == NULL) goto error;
    if(Strands) {
        pDenseSeg->strands = MemNew(sizeof(Uint1)*NumSeqs);
        if(pDenseSeg->strands == NULL) goto error;
    }
    else pDenseSeg->strands = NULL;

    pDenseSeg->dim = NumSeqs;
    pDenseSeg->numseg = NumSegs;
    pDenseSeg->scores = NULL;
    pDenseSeg->ids = NULL;
    return pDenseSeg;

error:
    ErrPostEx(SEV_ERROR, 0, 0, 
        "SAM_NewDenseSeg: Error");
    DenseSegFree(pDenseSeg);
    return NULL;

}

/*****************************************************************************
*
*   Constructs a new SeqAlign of type type and segment type segtype.  Add segs
*   to the seg pointer and sets the dimension of the SeqAlign to dim.
*
*   Returns NULL on error
*
*****************************************************************************/

NLM_EXTERN SeqAlign *SAM_NewSeqAlign
(Uint1 type, Uint1 segtype, Pointer segs, Int2 dim)
{
    SeqAlign *pSeqAlign;

    pSeqAlign = MemNew(sizeof(SeqAlign));

    pSeqAlign->type = type;

    pSeqAlign->segtype = segtype;
    pSeqAlign->dim = dim;
    pSeqAlign->segs = segs;

    pSeqAlign->score = NULL;
    pSeqAlign->next = NULL;
	pSeqAlign->bounds = NULL;
    pSeqAlign->master = NULL;
    pSeqAlign->saip = NULL;
    pSeqAlign->idx.entityID = 0;
    pSeqAlign->idx.itemID = 0;
    pSeqAlign->idx.itemtype = 0;

    return pSeqAlign;
}

/*****************************************************************************
*
*   Checks to see how a postion is inside or is in front of or in back of a
*   range.
*   If it is inside, return SAM_TOTALLAP
*   If it doesn't, return SAM_NOLAP & SAM_NOLAPFRONT if in front
*   If it doesn't and is in back return SAM_NOLAP & SAM_NOLAPBACK
*
*****************************************************************************/

NLM_EXTERN Int4 SAM_InRange(Int4 Position, Int4 From, Int4 To)
{
    return SAM_RangeOverlap(Position, Position, From, To);
}

/*****************************************************************************
*
*   Checks to see if Range1 overlaps Range2.
*   If it does completely, return SAM_TOTALLAP
*   If it doesn't, return SAM_NOLAP & SAM_NOLAPFRONT if in front
*   If it doesn't and is in back return SAM_NOLAP & SAM_NOLAPBACK
*   If pRange1 overlaps the front of pRange2, return SAM_FRONTLAP
*   If pRange1 overlaps the rear of pRange2, return SAM_BACKLAP
*
*****************************************************************************/

NLM_EXTERN Int4 SAM_RangeOverlap(Int4 From1, Int4 To1, Int4 From2, Int4 To2)
{
    if(To1 < From2) return SAM_NOLAPFRONT;
    if(From1 > To2) return SAM_NOLAPBACK;
    if(From1 >= From2) {
        if(To1 <= To2) return SAM_TOTALLAP;
        else return SAM_BACKLAP;
    }
    return SAM_FRONTLAP;
}

/*****************************************************************************
*
*   Lexically compare two SeqId's.  Checks ALL sips on both chains.
*   Returns -1 if sip1 <  sip2
*            0 if sip1 == sip2
*            1 if sip1 >  sip2
*
*****************************************************************************/

NLM_EXTERN Int4 SAM_LexicalComp(SeqId *sip1, SeqId *sip2)
{
    Char Id1[SAM_SIPBUF], Id2[SAM_SIPBUF];
    SeqId *sipThis1, *sipThis2;
    Int4 Compare;

    for(sipThis1 = sip1, sipThis2 = sip2;;
        sipThis1 = sipThis1->next, sipThis2 = sipThis2->next) {

        MakeReversedSeqIdString (sipThis1, Id1, (size_t) SAM_SIPBUF);
        MakeReversedSeqIdString (sipThis2, Id2, (size_t) SAM_SIPBUF);
        
        Compare = StrCmp(Id1, Id2);
        
        if(Compare == 0) {
            if(sipThis1->next == NULL && sipThis2->next == NULL) return 0;
            else if(sipThis1->next == NULL) return -1;
            else if(sipThis2->next == NULL) return 1;
            continue;
        }
        return Compare;
    }
}


/*****************************************************************************
*
*   Orders two SeqId's for binary searches, etc.  DOES check the full
*   ValNode lists.  The ordering is arbitrary but consistent.
*   Returns -1 if sip1 <  sip2
*            0 if sip1 == sip2
*            1 if sip1 >  sip2
*
*****************************************************************************/

NLM_EXTERN Int4 SAM_OrderSeqIDChain (SeqId *sip1, SeqId *sip2)
{
    Int4 retval = 1;

    for(;sip1 != NULL && sip2 != NULL; sip1 = sip1->next, sip2 = sip2->next) {        
        retval = SAM_OrderSeqID(sip1, sip2);
        if(retval != 0) return retval;
    }
    if(sip1 == NULL && sip2 == NULL) return 0;
    else if(sip2 == NULL) return -1;
    else return 1;
}

/*****************************************************************************
*
*   Orders two SeqId's for binary searches, etc. Does NOT check the full
*   ValNode lists.  The ordering is arbitrary but consistent.
*   Returns -1 if sip1 <  sip2
*            0 if sip1 == sip2
*            1 if sip1 >  sip2
*
*****************************************************************************/

NLM_EXTERN Int4 SAM_OrderSeqID(SeqId *sip1, SeqId *sip2)
{
    Char Buf1[SAM_SIPBUF], Buf2[SAM_SIPBUF];
    Int4 retval = 1;
    
    if(sip1->choice == sip2->choice ) goto check;
    
    if((sip1->choice == SEQID_GENBANK ||
        sip1->choice == SEQID_EMBL || sip1->choice == SEQID_DDBJ) && 
        (sip2->choice == SEQID_GENBANK || sip2->choice == SEQID_EMBL ||
        sip2->choice == SEQID_DDBJ || sip2->choice == SEQID_TPG ||
        sip2->choice == SEQID_TPE || sip2->choice == SEQID_TPD ||
        sip2->choice == SEQID_GPIPE)) goto check;
    goto nocheck;
    
check:
    switch (sip1->choice) {
    case SEQID_GI:
    case SEQID_GIBBSQ:
    case SEQID_GIBBMT:
        retval = sip1->data.intvalue - sip2->data.intvalue;
        if (retval == 0) break;
        if (retval > 0) return 1;
        return -1;
        
    case SEQID_LOCAL:
    case SEQID_GIIM:
    case SEQID_GENERAL:
    case SEQID_PDB:
    case SEQID_PATENT:
    case SEQID_PRF:
    case SEQID_DDBJ:
    case SEQID_OTHER:
    case SEQID_EMBL:
    case SEQID_GENBANK:
    case SEQID_PIR:
    case SEQID_SWISSPROT:
    case SEQID_TPG:
    case SEQID_TPE:
    case SEQID_TPD:
    case SEQID_GPIPE:
        SeqIdWrite (sip1, Buf1, PRINTID_FASTA_SHORT, SAM_SIPBUF);
        SeqIdWrite (sip2, Buf2, PRINTID_FASTA_SHORT, SAM_SIPBUF);
        retval = StrCmp(Buf1, Buf2);
        if(retval != 0) return retval;
        break;
        
    default:
        retval = 1;
        break;
    }
    return retval;
    
nocheck:
    if (sip1->choice > sip2->choice) return 1;
    return -1;
}


/*****************************************************************************
*
*   Compare two SeqId to make sure all valnodes compare exactly
*
*****************************************************************************/

NLM_EXTERN Boolean SAM_SeqIdCompareAll(SeqId *sip1, SeqId *sip2)
{
    SeqId *sip;
    Boolean retval = TRUE;

    if(sip1 == NULL || sip2 == NULL) return FALSE;
    if(ValNodeLen(sip1) != ValNodeLen(sip2)) return FALSE;

    for(sip = sip1; sip != NULL; sip = sip->next)
        if(!SeqIdIn(sip, sip2)) retval = FALSE;

    return retval;
}


/*****************************************************************************
*
*   Compare two AlignId to make sure all valnodes compare exactly
*
*****************************************************************************/

NLM_EXTERN Boolean SAM_AlignIdCompare(AlignId *saip1, AlignId *saip2)
{
    SeqId *saip;
    Boolean retval = TRUE;

    if(saip1 == NULL || saip2 == NULL) return FALSE;
    if(ValNodeLen(saip1) != ValNodeLen(saip2)) return FALSE;

    for(saip = saip1; saip != NULL; saip = saip->next)
        if(!SAM_AlignIdIn(saip, saip2)) retval = FALSE;

    return retval;
}

/*****************************************************************************
*
*     Looks for single AlignId, "a" in chain of AlignIds, "b"
*
*****************************************************************************/

NLM_EXTERN Boolean SAM_AlignIdIn (AlignId *a, AlignId *b)
{
	AlignId *now;
	Uint1 retval;

	if (a == NULL)
	    return FALSE;

	for (now =b; now != NULL; now = now -> next)
	{
        retval = SAM_AlignIdComp(a, now);
		switch (retval)
		{
			case SIC_YES:
				return TRUE;
			case SIC_NO:
				return FALSE;
		}
    }
    return FALSE;
}


/*****************************************************************************
*
*   	Compares a to b and returns
*
*   SIC_DIFF   = different types, could not be compared
*   SIC_NO     = types could be compared, and ids are different
*   SIC_YES    = types could be compared, and ids are the same
*
*****************************************************************************/

NLM_EXTERN Uint1 SAM_AlignIdComp (AlignId *a, AlignId *b)
{
    Uint1 choice;

    if ((a == NULL) || (b == NULL))
        return SIC_DIFF;

	choice = a->choice;
	if (choice != b->choice) return SIC_DIFF;
    switch (choice)
    {
        case AlignId_id:   
            if (ObjectIdMatch((ObjectIdPtr)a->data.ptrvalue, (ObjectIdPtr)b->data.ptrvalue))
				return SIC_YES;
			else
				return SIC_NO;
        case AlignId_gi:  /* gi */
            if (a->data.intvalue == b->data.intvalue)
				return SIC_YES;
			else
				return SIC_NO;
        case AlignId_itemid:  /* "permanent" itemid */
            if (a->data.intvalue == b->data.intvalue)
				return SIC_YES;
			else
				return SIC_NO;
		default:
			ErrPostEx(SEV_ERROR, 0,0, "AlignIdComp: unsupported type [%d]",
				(int)choice);
			return SIC_DIFF;
     }
}

/*******************************************************
*
*   duplicate a list of AlignId *
*
*******************************************************/

NLM_EXTERN AlignId * SAM_AlignIdDupList (AlignId *id_list)
{
  SeqId *sip=NULL;
  SeqId *sid;

  for (sid = id_list; sid != NULL; sid = sid->next) {
         ValNodeLink(&sip, SAM_AlignIdDup(sid));  
  }
  return sip;
}

/*******************************************************
*
*   Duplicates one AlignId
*
*******************************************************/

NLM_EXTERN AlignId * SAM_AlignIdDup (AlignId *oldid)
{
	AlignId *newid = NULL;

    if (oldid == NULL)
        return oldid;

	newid = ValNodeNew(NULL);
	if (newid == NULL) return newid;
	MemCopy(newid, oldid, sizeof(ValNode));
	newid->next = NULL;    /* not in chain */
    switch (oldid->choice)
    {
        case AlignId_id:
			newid->data.ptrvalue = ObjectIdDup((ObjectIdPtr)oldid->data.ptrvalue);
			break;
        case AlignId_itemid:
        case AlignId_gi:
            break;
     }
	return newid;
}


NLM_EXTERN ParaGPtr DDE_ParaGNew(ParaGPtr pParaG) {
/*----------------------------------------------------------------------------
*  makes a copy of a ParaG.
*
*  ParaG has many pointers.  only allocates space for a full copy
*  when the copy may differ from the original.  otherwise, it's sufficient
*  to copy the pointers.
*
*  Full copies are needed for:  ptxtList, and szEditSeq.
*
*  returns pointer to new ParaG.
*---------------------------------------------------------------------------*/
  ParaGPtr    pParaGCopy;
  ValNodePtr  pValNode;

  /* allocate space for the new ParaG */
  pParaGCopy = MemNew(sizeof(ParaG));
  ASSERT(pParaGCopy != NULL);

  /* as default, make duplicate of the structure */
  MemCopy(pParaGCopy, pParaG, sizeof(ParaG));

  /* make new copy of the ptxtList linked list */
  /* head points to new ValNode which points to a new copy of an MsaTxtDisp */
  pParaGCopy->ptxtList = NULL;
  if (pParaG->ptxtList != NULL) {
    pParaGCopy->ptxtList = 
      ValNodeAddPointer(NULL, 0, UDV_MsaTxtDispNew(pParaG->ptxtList->data.ptrvalue));
  }
  /* do while there's another node to add */
  pValNode = ValNodeFindNext(pParaG->ptxtList, pParaG->ptxtList, 0);
  while (pValNode != NULL) {
    /* make new node, point it to new MsaTxtDisp, add it to linked list */
    ValNodeAddPointer(&(pParaGCopy->ptxtList), 0, UDV_MsaTxtDispNew(pValNode->data.ptrvalue));
    /* get next node */
    pValNode = ValNodeFindNext(pParaG->ptxtList, pValNode, 0);
  }

  /* copy the sequence in szEditSeq */
  pParaGCopy->szEditSeq = MemNew(StringLen(pParaG->szEditSeq)+1);
  ASSERT(pParaGCopy->szEditSeq != NULL);
  StringCpy(pParaGCopy->szEditSeq, pParaG->szEditSeq);

  /* return pointer to the new copy of ParaG */
  return(pParaGCopy);
}


NLM_EXTERN ParaGPtr DDE_ParaGFree(ParaGPtr pParaG) {
/*----------------------------------------------------------------------------
*  frees all memory allocated for a ParaG in DDE_ParaGNew().
*  returns NULL for successful completion.
*---------------------------------------------------------------------------*/
  /* free the whole ptxtList linked list */
  /* each MsaTxtDisp is free'd here (no need to call UDV_MsaTxtDispFree()) */
  if (pParaG->ptxtList != NULL) {
    ValNodeFreeData(pParaG->ptxtList);
  }

  /* free the szEditSeq char array */
  if (pParaG->szEditSeq != NULL) {
    MemFree(pParaG->szEditSeq);
  }

  /* free the SeqId -- actually, it's not safe to free it */
/*
  if (pParaG->sip != NULL) {
    SeqIdFree(pParaG->sip);
  }
*/

  /* free the ParaG */
  if (pParaG != NULL) {
    MemFree(pParaG);
  }

  return(NULL);
}


NLM_EXTERN MsaParaGPopListPtr DDE_PopListNew(MsaParaGPopListPtr pPopList, Int4 TotalNumRows) {
/*----------------------------------------------------------------------------
*  makes a copy of PopList.
*
*  PopList has many pointers.  only allocates space for a full copy
*  when the copy may differ from the original.  otherwise, it's sufficient
*  to copy the pointers.
*
*  Full copies are needed for:  TableHead, RulerDescr, and entitiesTbl.
*
*  returns pointer to new PopList.
*---------------------------------------------------------------------------*/
  MsaParaGPopListPtr  pPopListCopy;
  Int4                i;
  ValNodePtr          pValNode;
  DDVRulerDescrPtr    pCopy;
  
  /* allocate space for the new MsaParaGPopList */
  pPopListCopy = MemNew(sizeof(MsaParaGPopList));
  ASSERT(pPopListCopy != NULL);

  /* as default, make duplicate of the structure */
  MemCopy(pPopListCopy, pPopList, sizeof(MsaParaGPopList));

  /* allocate space for array of ValNodePtr's */
  pPopListCopy->TableHead = MemNew(TotalNumRows * sizeof(ValNodePtr));
  ASSERT(pPopListCopy->TableHead != NULL);

  /* for each ValNodePtr in TableHead */
  for (i=0; i<TotalNumRows; i++) {
    /* make new ValNode, point it to copy of ParaG, add it to linked list */
    /* note:  there is only one ParaG per row for DDE */
    pPopListCopy->TableHead[i] =
      ValNodeAddPointer(NULL, 0, DDE_ParaGNew(pPopList->TableHead[i]->data.ptrvalue));
  }

  /* create a new linked list of RulerDescr structures */
  /* first node points to a copy of a DDVRulerDescr */
  pCopy = MemNew(sizeof(DDVRulerDescr));
  MemCopy(pCopy, pPopList->RulerDescr->data.ptrvalue, sizeof(DDVRulerDescr));
  pPopListCopy->RulerDescr = ValNodeAddPointer(NULL, 0, pCopy);
  /* do while there's another node to add */
  pValNode = ValNodeFindNext(pPopList->RulerDescr, pPopList->RulerDescr, 0);
  while (pValNode != NULL) {
    /* make new node, point it to new RulerDescr, add it to linked list */
    pCopy = MemNew(sizeof(DDVRulerDescr));
    MemCopy(pCopy, pValNode->data.ptrvalue, sizeof(DDVRulerDescr));
    ValNodeAddPointer(&(pPopListCopy->RulerDescr), 0, pCopy);
    /* get next node */
    pValNode = ValNodeFindNext(pPopList->RulerDescr, pValNode, 0);
  }

  /* make copy of array of bioseq ID's (entitiesTbl) */
  pPopListCopy->entitiesTbl = MemNew(TotalNumRows * sizeof(Uint4));
  ASSERT(pPopListCopy->entitiesTbl != NULL);
  MemCopy(pPopListCopy->entitiesTbl, pPopList->entitiesTbl, TotalNumRows*sizeof(Uint4));
  
  /* return pointer to the new copy of MsaParaGPopList */
  return(pPopListCopy);
}


NLM_EXTERN MsaParaGPopListPtr DDE_PopListFree(MsaParaGPopListPtr pPopList, Int4 TotalNumRows) {
/*----------------------------------------------------------------------------
*  frees all memory allocated for an MsaParaGPopList in DDE_PopListNew().
*  returns NULL for successful completion.
*---------------------------------------------------------------------------*/
  Int4        i;
  ValNodePtr  vnp;

  /* for each ValNodePtr, free the linked list of ParaG's the ValNode points to, free the ValNode */
  for (i=0; i<TotalNumRows; i++) {
    vnp = pPopList->TableHead[i];
    while (vnp != NULL) {
      if (vnp->data.ptrvalue != NULL) {
        DDE_ParaGFree(vnp->data.ptrvalue);
      }
      vnp = vnp->next;
    }
    if (pPopList->TableHead[i] != NULL) {
      ValNodeFree(pPopList->TableHead[i]);
    }
  }

  /* now free the array of ValNodePtr's (i.e. the TableHead array) */
  ASSERT(pPopList->TableHead != NULL);
  MemFree(pPopList->TableHead);

  /* free the whole RulerDescr linked list */
  /* each DDVRulerDescr is free'd here (no need to call DDV_RulerDescrFree()) */
  if (pPopList->RulerDescr != NULL) {
    ValNodeFreeData(pPopList->RulerDescr);
  }

  /* free the entitiesTbl array */
  if (pPopList->entitiesTbl != NULL) {
    MemFree(pPopList->entitiesTbl);
  }

  /* free the PopList */
  ASSERT(pPopList != NULL);
  MemFree(pPopList);
  return(NULL);
}


NLM_EXTERN void DDE_Verify(DDE_InfoPtr pEditInfo) {
/*----------------------------------------------------------------------------
*  verify function -- makes some checks to see that things are ok.
*---------------------------------------------------------------------------*/
#if defined(_DEBUG)
  Int4           DispCoordStart, DispCoordStop, StopStop=-1;
  Int4           i, NumRows, save_to;
  ValNodePtr     vnp, next_vnp, pRuler1, pRuler2;
  MsaTxtDispPtr  msap, next_msap;
  ParaGPtr       pgp;

  /* check that the display coordinates in the MsaTxtDisp's are correct */
  /* check that the last display coordinate stop is the same for each row */
  /* check that the last display coordinate stop agrees with StopLetter */
  NumRows = pEditInfo->TotalNumRows;
  for (i=0; i<NumRows; i++) {
    DispCoordStart = 0;
    DispCoordStop = -1;
    vnp = DDE_GetTxtListPtr(pEditInfo, i);
    pgp = DDE_GetParaGPtr(pEditInfo, i);
    while (vnp != NULL) {
      msap = (MsaTxtDispPtr) vnp->data.ptrvalue;
      DispCoordStart = DispCoordStop + 1;
      DispCoordStop =  DispCoordStart + (msap->to - msap->from);
      if (DispCoordStop > StopStop) {
        StopStop = DispCoordStop;
        ASSERT(i == 0);
      }
      if (msap->IsGap) {
        ASSERT(msap->from == DispCoordStart);
        ASSERT(msap->to   == DispCoordStop);
      }
      vnp = vnp->next;
    }
    ASSERT(DispCoordStop == StopStop);
    ASSERT(DispCoordStop == pgp->StopLetter);
  }

  /* check that there are no gaps in the bioseq coords */
  for (i=0; i<NumRows; i++) {
    vnp = DDE_GetTxtListPtr(pEditInfo, i);
    save_to = -1;
    while (vnp != NULL) {
      msap = (MsaTxtDispPtr) vnp->data.ptrvalue;
      if (!msap->IsGap) {
        if (save_to != -1) {
          ASSERT(save_to+1 == msap->from);
        }
        save_to = msap->to;
      }
      vnp = vnp->next;
    }
  }

  /* check that no 2 adjacent nodes need to be merged */
  for (i=0; i<NumRows; i++) {
    vnp = DDE_GetTxtListPtr(pEditInfo, i);
    next_vnp = vnp->next;
    while (next_vnp != NULL) {
      msap = (MsaTxtDispPtr) vnp->data.ptrvalue;
      next_msap = (MsaTxtDispPtr) next_vnp->data.ptrvalue;
      /* make sure alignment and gap status of 2 adjacent nodes aren't identical */
      ASSERT( ! ((msap->IsGap == next_msap->IsGap) && (msap->IsUnAligned == next_msap->IsUnAligned)));
      vnp = next_vnp;
      next_vnp = vnp->next;
    }
  }

  /* remake the RulerDescr for row 0.  this is the reference. */
  /* use arbitrary align start of 0.  it matters, but not here */
  pRuler1 = DDE_ReMakeRulerForRow(pEditInfo->pPopList, 0, 0);
  for (i=1; i<NumRows; i++) {
    /* remake the RulerDescr for each row */
    pRuler2 = DDE_ReMakeRulerForRow(pEditInfo->pPopList, i, 0);
    /* make sure the 2 RulerDescr's match */
    ASSERT(DDE_AreIdenticalRulers(pRuler1, pRuler2));
    VERIFY(ValNodeFreeData(pRuler2) == NULL);
  }
  VERIFY(ValNodeFreeData(pRuler1) == NULL);

#endif
}


NLM_EXTERN Int4 DDE_GetFirstDisplayCoord(MsaParaGPopListPtr pPopList) {
/*----------------------------------------------------------------------------
*  look at the RulerDescr linked list to get the first display coordinate
*  for an aligned block.  if there is no aligned region return -1.
*---------------------------------------------------------------------------*/
  ValNodePtr        vnp;
  DDVRulerDescrPtr  rdp;

  vnp = pPopList->RulerDescr;
  while (vnp != NULL) {
    rdp = (DDVRulerDescrPtr) vnp->data.ptrvalue;
    if (rdp->align_start != -1) {
      return(rdp->disp_start);
    }
    vnp = vnp->next;
  }
  return(-1);
}


NLM_EXTERN Int4 DDE_GetFirstAlignIndex(MsaParaGPopListPtr pPopList) {
/*----------------------------------------------------------------------------
*  look at the RulerDescr linked list to get the first alignment index.
*  if there is no aligned region return -1.
*---------------------------------------------------------------------------*/
  ValNodePtr        vnp;
  DDVRulerDescrPtr  rdp;

  vnp = pPopList->RulerDescr;
  while (vnp != NULL) {
    rdp = (DDVRulerDescrPtr) vnp->data.ptrvalue;
    if (rdp->align_start != -1) {
      return(rdp->align_start);
    }
    vnp = vnp->next;
  }
  return(-1);
}


NLM_EXTERN Boolean DDE_AreSimilarRulerDescrs(DDVRulerDescrPtr p1, DDVRulerDescrPtr p2) {
/*----------------------------------------------------------------------------
*  compare 2 DDVRulerDescr's.
*  return FALSE if only 1 is aligned.
*  return FALSE if they're aligned, and the aligned starts and stops differ.
*  return TRUE if they're unaligned.
*---------------------------------------------------------------------------*/
  /* if only 1 is aligned return FALSE */
  if (p1->bUnAligned != p2->bUnAligned) {
    return(FALSE);
  }
  /* if they're both aligned, have same align starts, and are same length return TRUE */
  if (!p1->bUnAligned) {
    if (p1->align_start == p2->align_start) {
      if ((p1->disp_stop - p1->disp_start) == (p2->disp_stop - p2->disp_start)) {
        return(TRUE);
      }
    }
    return(FALSE);
  }
  /* if they're both unaligned return TRUE */
  else {
    return(TRUE);
  }
}


NLM_EXTERN Boolean DDE_AreIdenticalRulerDescrs(DDVRulerDescrPtr p1, DDVRulerDescrPtr p2) {
/*----------------------------------------------------------------------------
*  compare 2 DDVRulerDescr's.
*  return TRUE if they're identical.
*---------------------------------------------------------------------------*/
  if ((p1->disp_start)  == (p2->disp_start)   &&
      (p1->disp_stop)   == (p2->disp_stop)    &&
      (p1->align_start) == (p2->align_start)  &&
      (p1->bUnAligned)  == (p2->bUnAligned)) {
    return(TRUE);
  }
  else {
    return(FALSE);
  }
}


NLM_EXTERN Boolean DDE_AreSimilarRulers(ValNodePtr pRuler1, ValNodePtr pRuler2,
                                        Int4* pDispCoord1,  Int4* pDispCoord2) {
/*----------------------------------------------------------------------------
*  compare 2 linked lists of DDVRulerDescr's.
*  return TRUE if THE ALIGNED SECTIONS are identical.
*  if this function returns FALSE, Display Coordinates are returned.
*  These coordinates indicate where the first mismatch between rulers
*  was found.
*---------------------------------------------------------------------------*/
  DDVRulerDescrPtr  pNode1, pNode2;
  ValNodePtr        vnp1, vnp2;

  vnp1 = pRuler1;
  vnp2 = pRuler2;

  /* for return of TRUE, give illegal display coordinates */
  *pDispCoord1 = -1;
  *pDispCoord2 = -1;

  /* skip over the left tails, in case one ruler's tail is on and not the other's */
  pNode1 = (DDVRulerDescrPtr) vnp1->data.ptrvalue;
  pNode2 = (DDVRulerDescrPtr) vnp2->data.ptrvalue;
  if (pNode1->bUnAligned) {
    vnp1 = vnp1->next;
  }
  if (pNode2->bUnAligned) {
    vnp2 = vnp2->next;
  }

  /* check each aligned node */
  while (vnp1 != NULL) {
    pNode1 = (DDVRulerDescrPtr) vnp1->data.ptrvalue;
    pNode2 = (DDVRulerDescrPtr) vnp2->data.ptrvalue;
    if (!DDE_AreSimilarRulerDescrs(pNode1, pNode2)) {
      *pDispCoord1 = pNode1->disp_start;
      *pDispCoord2 = pNode2->disp_start;
      return(FALSE);
    }
    vnp1 = vnp1->next;
    vnp2 = vnp2->next;
    /* if just the right tail of ruler1 remains, return TRUE */
    if ((vnp2 == NULL) && (vnp1 != NULL)) {
      pNode1 = (DDVRulerDescrPtr) vnp1->data.ptrvalue;
      if ((pNode1->bUnAligned) && (vnp1->next == NULL)) {
        return(TRUE);
      }
    }
    /* if just the right tail of ruler2 remains, return TRUE */
    if ((vnp1 == NULL) && (vnp2 != NULL)) {
      pNode2 = (DDVRulerDescrPtr) vnp2->data.ptrvalue;
      if ((pNode2->bUnAligned) && (vnp2->next == NULL)) {
        return(TRUE);
      }
    }
  }

  /* make sure they're the same length */
  if (vnp2 == NULL) {
    return(TRUE);
  }
  *pDispCoord1 = pNode1->disp_start;
  *pDispCoord2 = pNode2->disp_start;
  return(FALSE);
}


NLM_EXTERN Boolean DDE_AreIdenticalRulers(ValNodePtr pRuler1, ValNodePtr pRuler2) {
/*----------------------------------------------------------------------------
*  compare 2 linked lists of DDVRulerDescr's.
*  return TRUE if they're identical.
*---------------------------------------------------------------------------*/
  DDVRulerDescrPtr  pNode1, pNode2;
  ValNodePtr        vnp1, vnp2;

  vnp1 = pRuler1;
  vnp2 = pRuler2;
  while (vnp1 != NULL) {
    pNode1 = (DDVRulerDescrPtr) vnp1->data.ptrvalue;
    pNode2 = (DDVRulerDescrPtr) vnp2->data.ptrvalue;
    if (!DDE_AreIdenticalRulerDescrs(pNode1, pNode2)) {
      return(FALSE);
    }
    vnp1 = vnp1->next;
    vnp2 = vnp2->next;
  }
  /* make sure they're the same length */
  if (vnp2 == NULL) {
    return(TRUE);
  }
  return(FALSE);
}


NLM_EXTERN ValNodePtr DDE_ReMakeRuler(MsaParaGPopListPtr pPopList, Boolean Replace, Int4 AlignStart) {
/*----------------------------------------------------------------------------
*  remake the RulerDescr linked list of pPopListEdit with an arbitrary Row.
*  save pointer to head of linked-list, and return it.
*  if Replace is TRUE, PopList's RulerDescr is replaced with the new one.
*  if Replace is TRUE, AlignStart comes from the AlignStart in the
*  current RulerDescr.  Otherwise, AlignStart passed here is used.
*---------------------------------------------------------------------------*/
  ValNodePtr  pRulerHead;

  /* if there's a valid AlignStart in the current RulerDescr, use it */
  if (Replace) {
    AlignStart = DDE_GetFirstAlignIndex(pPopList);
  }

  /* make a new linked list */
  pRulerHead = DDE_ReMakeRulerForRow(pPopList, 0, AlignStart);

  if (Replace) {
    /* free the old RulerDescr linked list */
    ValNodeFreeData(pPopList->RulerDescr);
    /* save the new linked list */
    pPopList->RulerDescr = pRulerHead;
  }

  return(pRulerHead);
}


NLM_EXTERN ValNodePtr DDE_ReMakeRulerForRow(MsaParaGPopListPtr pPopList, Int4 Row, Int4 AlignStart) {
/*----------------------------------------------------------------------------
*  remake the RulerDescr linked list of pPopListEdit.
*  the linked list is made by scanning through a single Row.
*  return a pointer to the new linked-list.
*  (the old one is NOT free'd here.)
*---------------------------------------------------------------------------*/
  ValNodePtr     head=NULL, vnp, pRulerLast=NULL, pRulerHead=NULL;
  MsaTxtDispPtr  msap;
  Int4           DispStart=0, DispStop=-1;
  Int4           AlignStop;
  Boolean        InAlignedRegion=FALSE;
  ValNodePtr     pg_vnp;
  ParaGPtr       pgp;

  AlignStop = AlignStart - 1;

  /* get the head of the linked-list of ParaG's */
  pg_vnp =  (ValNodePtr) (pPopList->TableHead[Row]);

  while (pg_vnp != NULL) {
    /* get head of linked-list of MsaTxtDisp's for Row */
    pgp =        (ParaGPtr) (pg_vnp->data.ptrvalue);
    vnp = (ValNodePtr) (pgp->ptxtList);
    if (head == NULL) {
      head = vnp;
    }
    while (vnp != NULL) {
      msap = (MsaTxtDispPtr) vnp->data.ptrvalue;
      if (!msap->IsUnAligned) {
        /* if entering an aligned region */
        if (!InAlignedRegion) {
          if (vnp != head) {
            /* add unaligned RulerDescr to end of linked list */
            pRulerLast = DDE_AddRulerDescrNode(pRulerLast, DispStart, DispStop, -1, TRUE, FALSE);
            if (pRulerHead == NULL) {
              pRulerHead = pRulerLast;
            }
          }
          InAlignedRegion = TRUE;
          /* adjust start when entering aligned region */
          AlignStart = AlignStop + 1;
          DispStart = DispStop + 1;
        }
        /* adjust stop every time node is aligned */
        AlignStop =  AlignStop + 1 + (msap->to - msap->from);
        DispStop =  DispStop + 1 + (msap->to - msap->from);
      }
      else {
        /* if entering an unaligned region */
        if (InAlignedRegion) {
          /* add aligned RulerDescr to end of linked list */
          pRulerLast = DDE_AddRulerDescrNode(pRulerLast, DispStart, DispStop, AlignStart, FALSE, FALSE);
          if (pRulerHead == NULL) {
            pRulerHead = pRulerLast;
          }
          /* adjust start when entering unaligned region */
          DispStart = DispStop + 1;
        }
        InAlignedRegion = FALSE;
        /* adjust stop every time node is unaligned */
        DispStop =  DispStop + 1 + (msap->to - msap->from);
      }
      vnp = vnp->next;
    }
    pg_vnp = pg_vnp->next;
  }

  /* add last RulerDescr node to linked list */
  if (InAlignedRegion) {
    pRulerLast = DDE_AddRulerDescrNode(pRulerLast, DispStart, DispStop, AlignStart, FALSE, FALSE);
  }
  else {
    pRulerLast = DDE_AddRulerDescrNode(pRulerLast, DispStart, DispStop, -1, TRUE, FALSE);
  }

  /* if there's only 1 node, make it the head */
  if (pRulerHead == NULL) {
    pRulerHead = pRulerLast;
  }

  return(pRulerHead);
}


NLM_EXTERN DDE_InfoPtr DDE_New(MsaParaGPopListPtr pPopList, Int4 TotalNumRows) {
/*----------------------------------------------------------------------------
*  allocates memory for a DDE_Info.
*  a copy of PopList is made using DDE_PopListNew().
*  The Visible and RowOrder arrays are also allocated and initialized.
*---------------------------------------------------------------------------*/
  DDE_InfoPtr  pEditInfo;
  Int4         i, NumRows;

  /* allocate space for the new DDE_Info */
  pEditInfo = MemNew(sizeof(DDE_Info));
  ASSERT(pEditInfo != NULL);

  /* set total number of rows */
  NumRows = pPopList->nBsp;
  pEditInfo->TotalNumRows = TotalNumRows;

  /* create a copy of PopList for editing */
  pEditInfo->pPopList = DDE_PopListNew(pPopList, TotalNumRows);

  /* allocate space for the Visible and RowOrder arrays */
  pEditInfo->pVisible =     MemNew(TotalNumRows*sizeof(Boolean));
  pEditInfo->pRowOrder =    MemNew(TotalNumRows*sizeof(Int4));
  pEditInfo->pRowOrderLUT = MemNew(TotalNumRows*sizeof(Int4));

  /* initialize the Visible array to all true */
  /* memfill(dest, val, nBytes) */
  ASSERT(sizeof(Boolean) == 1);
  MemFill(pEditInfo->pVisible, TRUE, NumRows);

  /* initialize the RowOrder array to 0,1,2,...N-1 */
  /* initialize the LUT to match the initial RowOrder */
  for (i=0; i<NumRows; i++) {
    pEditInfo->pRowOrder[i] = i;
    pEditInfo->pRowOrderLUT[i] = i;
  }

  /* return pointer to new DDE_Info object */
  return(pEditInfo);
}


NLM_EXTERN DDE_InfoPtr DDE_Copy(DDE_InfoPtr pEditInfo, Int4 TotalNumRows) {
/*---------------------------------------------------------------------------------
*  make a duplicate of pEditInfo.
*--------------------------------------------------------------------------------*/
  DDE_InfoPtr  pDup;

  /* make a duplicate of pEditInfo */
  pDup = DDE_New(pEditInfo->pPopList, TotalNumRows);

  /* make copies of the arrays */
  MemCopy(pDup->pVisible, pEditInfo->pVisible, TotalNumRows*sizeof(Boolean));
  MemCopy(pDup->pRowOrder, pEditInfo->pRowOrder, TotalNumRows*sizeof(Int4));
  MemCopy(pDup->pRowOrderLUT, pEditInfo->pRowOrderLUT, TotalNumRows*sizeof(Int4));
  return(pDup);
}


NLM_EXTERN DDE_InfoPtr DDE_Free(DDE_InfoPtr pEditInfo, Int4 TotalNumRows) {
/*----------------------------------------------------------------------------
*  frees all memory allocated for the DDE_Info.
*  returns NULL for successful completion.
*---------------------------------------------------------------------------*/
  DDE_PopListFree(pEditInfo->pPopList, TotalNumRows);
  MemFree(pEditInfo->pVisible);
  MemFree(pEditInfo->pRowOrder);
  MemFree(pEditInfo->pRowOrderLUT);
  MemFree(pEditInfo);
  return(NULL);
}


NLM_EXTERN DDE_StackPtr DDE_NewStack(MsaParaGPopListPtr pPopList) {
/*----------------------------------------------------------------------------
*  allocate memory for the DDE_Stack.
*  allocate space for an array of DDE_Info pointers.
*  init some vars.
*  save a copy of pEditInfo as original, and one for editing.
*---------------------------------------------------------------------------*/
  Int4          i;
  DDE_StackPtr  pStack;

  /* allocate memory for new DDE_Stack */
  pStack = MemNew(sizeof(DDE_Stack));

  /* allocate memory for pointers to DDE_Info's, NULL them */
  pStack->pArray = MemNew(sizeof(DDE_InfoPtr) * DDE_STACK_SIZE);
  for (i=0; i<DDE_STACK_SIZE; i++) {
    pStack->pArray[i] = NULL;
  }

  /* init some vars */
  pStack->NumInStack = 0;
  pStack->NumFromStart = 0;
  pStack->StackIndex = 0;
  pStack->StackMin = 0;
  pStack->StackMax = 0;

  /* make copies of pEditInfo */
  pStack->pOrig = DDE_New(pPopList, pPopList->nBsp);
  pStack->pEdit = DDE_New(pPopList, pPopList->nBsp);

  /* add pStack->pEdit to the stack */
  DDE_Add(pStack);
  pStack->SomethingToSave = FALSE;
  return(pStack);
}


NLM_EXTERN DDE_StackPtr DDE_FreeStack(DDE_StackPtr pStack) {
/*----------------------------------------------------------------------------
*  free all memory allocated for the DDE_Stack.
*---------------------------------------------------------------------------*/
  Int4  i, Index;

  if(!pStack) return pStack;
  /* free each DDE_Info that's been allocated */
  Index = pStack->StackMin;
  for (i=0; i<pStack->NumInStack; i++) {
    /* free a DDE_Info */
    ASSERT(pStack->pArray[Index] != NULL);
    DDE_Free(pStack->pArray[Index], pStack->pOrig->TotalNumRows);
    Index = ++Index % DDE_STACK_SIZE;
  }

  /* free the copy of the original and the one that was being edited */
  DDE_Free(pStack->pOrig, pStack->pOrig->TotalNumRows);

  /* let DDV free the one being edited */
/*  DDE_Free(pStack->pEdit, pStack->pEdit->TotalNumRows); */

  MemFree(pStack->pArray);
  /* now free the stack */
  return(MemFree(pStack));
}


NLM_EXTERN void DDE_GetOriginal(DDE_StackPtr pStack, Boolean Save) {
/*----------------------------------------------------------------------------
*  free pStack->pEdit.
*  put a copy of pStack->pOrig into pStack->pEdit.
*  Save indicates if this edit is stored on the stack.
*---------------------------------------------------------------------------*/
  /* free pStack->pEdit */
  DDE_Free(pStack->pEdit, pStack->pEdit->TotalNumRows);

  /* copy pStack->pOrig into pStack->pEdit */
  pStack->pEdit = DDE_Copy(pStack->pOrig, pStack->pOrig->TotalNumRows);

  /* add pStack->pEdit to the stack */
  if (Save) {
    DDE_Add(pStack);
  }
  return;
}


NLM_EXTERN Boolean DDE_Add(DDE_StackPtr pStack) {
/*----------------------------------------------------------------------------
*  make a copy of pStack->pEdit and add it to the stack.
*  if the stack is full, store new record on top of the oldest record.
*  return TRUE if the stack is full.
*---------------------------------------------------------------------------*/

  /* if there's something on the stack at current position, free it */
  if (pStack->pArray[pStack->StackIndex] != NULL) {
    DDE_Free(pStack->pArray[pStack->StackIndex], pStack->pOrig->TotalNumRows);
  }

  /* put a copy of pStack->pEdit on the stack */
  pStack->pArray[pStack->StackIndex] = DDE_Copy(pStack->pEdit, pStack->pOrig->TotalNumRows);

  /* set StackMax to this slot */
  pStack->StackMax = pStack->StackIndex;

  /* if oldest record was overwritten, inc StackMin */
  if (pStack->NumFromStart == DDE_STACK_SIZE) {
    pStack->StackMin = ++pStack->StackMin % DDE_STACK_SIZE;
  }

  /* calculate number on stack */
  pStack->NumInStack = (pStack->StackMax - pStack->StackMin) + 1;
  if (pStack->NumInStack < 1) {
    pStack->NumInStack = pStack->NumInStack + DDE_STACK_SIZE;
  }
  pStack->NumFromStart = pStack->NumInStack;

  /* point to next slot */
  pStack->StackIndex = ++pStack->StackIndex % DDE_STACK_SIZE;

  /* return TRUE if the stack is full */
  if (pStack->NumInStack == DDE_STACK_SIZE) {
    return(TRUE);
  }

  /* when something is added to stack, indicate there are edits to save */
  pStack->SomethingToSave = TRUE;

  return(FALSE);
}


NLM_EXTERN Boolean DDE_AtStartOfStack(DDE_StackPtr pStack) {
/*----------------------------------------------------------------------------
*  return TRUE if can't back up further
*---------------------------------------------------------------------------*/
  if (pStack->NumFromStart == 1) {
    return(TRUE);
  }
  return(FALSE);
}


NLM_EXTERN Boolean DDE_AtEndOfStack(DDE_StackPtr pStack) {
/*----------------------------------------------------------------------------
*  return TRUE if can't advance further
*---------------------------------------------------------------------------*/
  if (pStack->NumFromStart == pStack->NumInStack) {
    return(TRUE);
  }
  return(FALSE);
}


NLM_EXTERN Boolean DDE_Prev(DDE_StackPtr pStack) {
/*----------------------------------------------------------------------------
*  free pStack->pEdit.
*  make a copy of the previous DDE_Info in the stack.
*  put it in pStack->pEdit.
*  return TRUE if something was retrieved from the stack.
*---------------------------------------------------------------------------*/
  /* return FALSE if can't back up further */
  if (pStack->NumFromStart == 1) {
    return(FALSE);
  }

  /* keep track of where we are in the stack */
  pStack->NumFromStart--;

  /* back up 2 because StackIndex is index of next free slot */
  pStack->StackIndex -= 2;
  if (pStack->StackIndex < 0) {
    pStack->StackIndex = DDE_STACK_SIZE - ABS(pStack->StackIndex);
  }

  /* free pStack->pEdit */
  DDE_Free(pStack->pEdit, pStack->pEdit->TotalNumRows);

  /* put a copy of an item from the stack into pStack->pEdit */
  pStack->pEdit = DDE_Copy(pStack->pArray[pStack->StackIndex], pStack->pOrig->TotalNumRows);

  /* advance 1 to point to slot after the one that's retrieved */
  pStack->StackIndex = ++pStack->StackIndex % DDE_STACK_SIZE;

  /* when the position on the stack changes, indicate there are edits to save */
  pStack->SomethingToSave = TRUE;
  
  return(TRUE);
}


NLM_EXTERN Boolean DDE_Next(DDE_StackPtr pStack) {
/*----------------------------------------------------------------------------
*  free pStack->pEdit.
*  make a copy of the next DDE_Info in the stack.
*  put it in pStack->pEdit.
*  return TRUE if something was retrieved from the stack.
*---------------------------------------------------------------------------*/
  /* return FALSE if can't advance further */
  if (pStack->NumFromStart == pStack->NumInStack) {
    return(FALSE);
  }

  /* keep track of where we are in the stack */
  pStack->NumFromStart++;

  /* free pStack->pEdit */
  DDE_Free(pStack->pEdit, pStack->pEdit->TotalNumRows);

  /* put a copy of an item from the stack into pStack->pEdit */
  pStack->pEdit = DDE_Copy(pStack->pArray[pStack->StackIndex], pStack->pOrig->TotalNumRows);

  /* advance 1 to point to slot after the one that's retrieved */
  pStack->StackIndex = ++pStack->StackIndex % DDE_STACK_SIZE;

  /* when the position on the stack changes, indicate there are edits to save */
  pStack->SomethingToSave = TRUE;

  return(TRUE);
}


NLM_EXTERN Int4 DDE_GetRowOrder(DDE_InfoPtr pEditInfo, Int4 Row) {
/*----------------------------------------------------------------------------
*
*  look through RowOrder for Row.
*  return the index.
*
*---------------------------------------------------------------------------*/
  Int4  NumRows, i;

  NumRows = pEditInfo->TotalNumRows;
  ASSERT(Row < NumRows);
  for (i=0; i<NumRows; i++) {
    if (pEditInfo->pRowOrder[i] == Row) {
      return(i);
    }
  }
  ASSERT(FALSE);  /* should never get here */
  return(-1);
}


NLM_EXTERN Int4 DDE_GetDisplayRow(DDE_InfoPtr pEditInfo, Int4 Row) {
/*----------------------------------------------------------------------------
*
*  look through RowOrderLUT for Row.
*  return the index.
*
*---------------------------------------------------------------------------*/
  Int4  NumRows, i;

  NumRows = pEditInfo->TotalNumRows;
  ASSERT(Row < NumRows);
  for (i=0; i<NumRows; i++) {
    if (pEditInfo->pRowOrderLUT[i] == Row) {
      return(i);
    }
  }
  ASSERT(FALSE);  /* should never get here */
  return(-1);
}


NLM_EXTERN Boolean DDE_IsBefore(DDE_InfoPtr pEditInfo, Int4 Row1, Int4 Row2) {
/*----------------------------------------------------------------------------
*
*  look through RowOrder to find if Row1 is before Row2
*  return TRUE if it is, FALSE if it's not.
*
*---------------------------------------------------------------------------*/
  Int4  NumRows, i;

  /* get total number of rows */
  NumRows = pEditInfo->TotalNumRows;
  ASSERT(Row1 < NumRows);
  ASSERT(Row2 < NumRows);

  /* look through RowOrder, if Row1 is found first return TRUE */
  for (i=0; i<NumRows; i++) {
    if (pEditInfo->pRowOrder[i] == Row1) {
      return(TRUE);
    }
    if (pEditInfo->pRowOrder[i] == Row2) {
      return(FALSE);
    }
  }
  ASSERT(FALSE);  /* should never get here */
  return(FALSE);
}


NLM_EXTERN Int4 DDE_GetInsertRow(DDE_InfoPtr pEditInfo, Int4 Row) {
/*----------------------------------------------------------------------------
*
*  look through RowOrderLUT to find where to put Row
*  warning:  this is O(N**2) and potentially slow for large number of rows
*
*---------------------------------------------------------------------------*/
  Int4  NumRows, i;

  /* get number of visible rows */
  NumRows = DDE_GetNumVisibleRows(pEditInfo);

  /* if no visible rows, then insert at top */
  if (NumRows == 0) {
    return(0);
  }

  /* look through RowOrderLUT for first row that Row should preceed */
  for (i=0; i<NumRows; i++) {
    if (DDE_IsBefore(pEditInfo, Row, pEditInfo->pRowOrderLUT[i])) {
      return(i);
    }
  }
  return(NumRows);
}


NLM_EXTERN Int4 DDE_GetNumVisibleRows(DDE_InfoPtr pEditInfo) {
/*----------------------------------------------------------------------------
*
*  count up the number of visible rows using the Visible array
*
*---------------------------------------------------------------------------*/
  Int4  NumRows, i, Cnt;

  NumRows = pEditInfo->TotalNumRows;
  Cnt = 0;
  for (i=0; i<NumRows; i++) {
    if (pEditInfo->pVisible[i]) {
      Cnt++;
    }
  }
  return(Cnt);
}


NLM_EXTERN Boolean DDE_HideNewRow(DDE_StackPtr pStack, Int4 Row, Boolean Save) {
/*----------------------------------------------------------------------------
*  Hide Row, where Row refers to the new row order.
*  Save indicates if this edit is stored on the stack.
*---------------------------------------------------------------------------*/
  Boolean  RetVal;

  RetVal = DDE_HideRow(pStack, pStack->pEdit->pRowOrder[Row], FALSE);

  /* add pStack->pEdit to the stack */
  if (Save) {
    DDE_Add(pStack);
  }
  return(RetVal);
}


NLM_EXTERN Boolean DDE_ShowNewRow(DDE_StackPtr pStack, Int4 Row, Boolean Save) {
/*----------------------------------------------------------------------------
*  Show Row, where Row refers to the new row order.
*  Save indicates if this edit is stored on the stack.
*---------------------------------------------------------------------------*/
  Boolean  RetVal;

  RetVal = DDE_ShowRow(pStack, pStack->pEdit->pRowOrder[Row], FALSE);

  /* add pStack->pEdit to the stack */
  if (Save) {
    DDE_Add(pStack);
  }
  return(RetVal);
}


NLM_EXTERN Boolean DDE_HideAllRows(DDE_StackPtr pStack, Boolean Save) {
/*----------------------------------------------------------------------------
*  hide all rows
*---------------------------------------------------------------------------*/
  Int4  i, NumRows;

  NumRows = pStack->pEdit->TotalNumRows;
  for (i=0; i<NumRows; i++) {
    DDE_HideRow(pStack, i, FALSE);
  }

  if (Save) {
    DDE_Add(pStack);
  }
  return(TRUE);
}


NLM_EXTERN Boolean DDE_ShowAllRows(DDE_StackPtr pStack, Boolean Save) {
/*----------------------------------------------------------------------------
*  show all rows
*---------------------------------------------------------------------------*/
  Int4  i, NumRows;

  NumRows = pStack->pEdit->TotalNumRows;
  for (i=0; i<NumRows; i++) {
    DDE_ShowRow(pStack, i, FALSE);
  }

  if (Save) {
    DDE_Add(pStack);
  }
  return(TRUE);
}


NLM_EXTERN Boolean DDE_HideRow(DDE_StackPtr pStack, Int4 Row, Boolean Save) {
/*----------------------------------------------------------------------------
*  Make Row invisible, where Row is the original row number (0-based).
*  Save indicates if this edit is stored on the stack.
*  return(True) for successful completion.
*  return(False) if row is already hidden.
*---------------------------------------------------------------------------*/
  Int4                i, NumRows, TotalRows, DisplayRow, tmp1, tmp2;
  Int4                NumRowsHigher, StartRow;
  MsaParaGPopListPtr  pPopList;
  ValNodePtr          tmp3;

  /* shorthand for the MsaParaGPopList being edited */
  pPopList = pStack->pEdit->pPopList;
  
  /* make sure not trying to index past array dimension */
  TotalRows = pStack->pOrig->pPopList->nBsp;
  ASSERT(Row < TotalRows);

  /* get number of rows currently displayed */
  /* if no rows are displayed return FALSE */
  NumRows = pPopList->nBsp;
  if (NumRows < 1) {
    return(FALSE);
  }

  /* if Row's already hidden return FALSE */
  /* otherwise, make it hidden */
  if (!pStack->pEdit->pVisible[Row]) {
    return(FALSE);
  }
  pStack->pEdit->pVisible[Row] = FALSE;
  
  /* adjust the LUT, TableHead pointers, and entitiesTbl */
  /* for Row disappearing and each row after it moving 1 higher */
  /* save the info at the bottom of the table for later use */
  DisplayRow = DDE_GetDisplayRow(pStack->pEdit, Row);
  tmp1 = pStack->pEdit->pRowOrderLUT[DisplayRow];
  tmp2 = pPopList->entitiesTbl[DisplayRow];
  tmp3 = pPopList->TableHead[DisplayRow];
  NumRowsHigher = ((ParaGPtr)(pPopList->TableHead[DisplayRow]->data.ptrvalue))->nLines;
  StartRow = ((ParaGPtr)(pPopList->TableHead[DisplayRow]->data.ptrvalue))->StartLine;
  for (i=DisplayRow; i<TotalRows-1; i++) {
    pStack->pEdit->pRowOrderLUT[i] = pStack->pEdit->pRowOrderLUT[i+1];
    pPopList->entitiesTbl[i] =   pPopList->entitiesTbl[i+1];
    pPopList->TableHead[i] =     pPopList->TableHead[i+1];
    StartRow = ((ParaGPtr)(pPopList->TableHead[i]->data.ptrvalue))->StartLine;
    ((ParaGPtr)(pPopList->TableHead[i]->data.ptrvalue))->StartLine -= NumRowsHigher;
  }
  i = TotalRows-1;
  pStack->pEdit->pRowOrderLUT[i] = tmp1;
  pPopList->entitiesTbl[i]   = tmp2;
  pPopList->TableHead[i]     = tmp3;
  ((ParaGPtr)(pPopList->TableHead[i]->data.ptrvalue))->StartLine = StartRow;

  /* decrement number of rows currently displayed */
  (pPopList->nBsp)--;
  
  if (Save) {
    DDE_Add(pStack);
  }
  return(TRUE);
}


NLM_EXTERN Boolean DDE_ShowRow(DDE_StackPtr pStack, Int4 Row, Boolean Save) {
/*----------------------------------------------------------------------------
*  Make Row visible, where Row is the original row number (0-based).
*  Save indicates if this edit is stored on the stack.
*  return(True) for successful completion.
*  return(False) if row is already visible.
*---------------------------------------------------------------------------*/
  Int4                i, NumRows, HiRow, LowRow, tmp1, tmp2;
  Int4                NumRowsLower, StartRow;
  MsaParaGPopListPtr  pPopList;
  ValNodePtr          tmp3;

  /* shorthand for the MsaParaGPopList being edited */
  pPopList = pStack->pEdit->pPopList;

  /* get total number of rows, including invisible ones */
  NumRows = pStack->pOrig->pPopList->nBsp;
  
  /* if Row's already visible return FALSE */
  if (pStack->pEdit->pVisible[Row]) {
    return(FALSE);
  }

  /* get where row is inserted, and where it currently sits */
  LowRow = DDE_GetInsertRow(pStack->pEdit, Row);
  HiRow = DDE_GetDisplayRow(pStack->pEdit, Row);
  ASSERT(LowRow <= HiRow);

  /* make it visible */
  pStack->pEdit->pVisible[Row] = TRUE;
  
  /* adjust the LUT, TableHead pointers, and entitiesTbl */
  /* for Row reappearing and each row after it moving 1 lower */
  tmp1 = pStack->pEdit->pRowOrderLUT[HiRow];
  tmp2 = pPopList->entitiesTbl[HiRow];
  tmp3 = pPopList->TableHead[HiRow];
  NumRowsLower = ((ParaGPtr)(pPopList->TableHead[HiRow]->data.ptrvalue))->nLines;
  StartRow = ((ParaGPtr)(pPopList->TableHead[HiRow]->data.ptrvalue))->StartLine;
  for (i=HiRow; i>LowRow; i--) {
    pStack->pEdit->pRowOrderLUT[i] = pStack->pEdit->pRowOrderLUT[i-1];
    pPopList->entitiesTbl[i] =   pPopList->entitiesTbl[i-1];
    pPopList->TableHead[i] =     pPopList->TableHead[i-1];
    StartRow = ((ParaGPtr)(pPopList->TableHead[i]->data.ptrvalue))->StartLine;
    ((ParaGPtr)(pPopList->TableHead[i]->data.ptrvalue))->StartLine += NumRowsLower;
  }
  pStack->pEdit->pRowOrderLUT[LowRow] = tmp1;
  pPopList->entitiesTbl[LowRow] = tmp2;
  pPopList->TableHead[LowRow] = tmp3;
  ((ParaGPtr)(pPopList->TableHead[LowRow]->data.ptrvalue))->StartLine = StartRow;

  /* increment number of rows currently displayed */
  (pPopList->nBsp)++;

  if (Save) {
    DDE_Add(pStack);
  }
  return(TRUE);
}


NLM_EXTERN Boolean DDE_MoveRow(DDE_StackPtr pStack, Int4 From, Int4 To, Boolean Save) {
/*----------------------------------------------------------------------------
*  if From > To:  Move row From to just before row To.
*  if From < To:  Move row From to just after row To.

*  this is done for symmetry, and also so the top and bottom rows
*  are accesible in one move.

*  From and To are NOT original row numbers!
*  From and To are the order in which the rows are displayed (0-based)
*  (this is a convenience, because that's the way the fn will be used)
*
*  Save indicates if this edit is stored on the stack.
*---------------------------------------------------------------------------*/
  Int4  FromOrig, ToOrig;
  Int4  i, FromIndx, ToIndx, tmp;

  /* get the original row numbers */
  FromOrig = pStack->pEdit->pRowOrderLUT[From];
  ToOrig = pStack->pEdit->pRowOrderLUT[To];

  /* re-arrange the rows */
  FromIndx = DDE_GetRowOrder(pStack->pEdit, FromOrig);
  ToIndx =   DDE_GetRowOrder(pStack->pEdit, ToOrig);
  tmp = pStack->pEdit->pRowOrder[FromIndx];
  if (FromIndx > ToIndx) {
    for (i=FromIndx; i>ToIndx; i--) {
      pStack->pEdit->pRowOrder[i] = pStack->pEdit->pRowOrder[i-1];
    }
  }
  else {
    for (i=FromIndx; i<ToIndx; i++) {
      pStack->pEdit->pRowOrder[i] = pStack->pEdit->pRowOrder[i+1];
    }
  }
  pStack->pEdit->pRowOrder[ToIndx] = tmp;

  /* hide the row that's moving, then show it. */
  /* it gets inserted in the right spot. */
  DDE_HideRow(pStack, FromOrig, FALSE);
  DDE_ShowRow(pStack, FromOrig, FALSE);

  /* add pStack->pEdit to the stack */
  if (Save) {
    DDE_Add(pStack);
  }
  return(TRUE);
}


NLM_EXTERN Boolean DDE_RestoreRowOrder(DDE_StackPtr pStack, Boolean Save) {
/*----------------------------------------------------------------------------
*  restore rows in PopListEdit to their original order.
*  Save indicates if this edit is stored on the stack.
*---------------------------------------------------------------------------*/
  Int4      i, NumRows;
  Boolean*  pVisibleCopy;

  /* make a copy of the visible array */
  NumRows = pStack->pEdit->TotalNumRows;
  ASSERT(sizeof(Boolean) == 1);
  pVisibleCopy = MemNew(NumRows);
  ASSERT(pVisibleCopy != NULL);
  MemCopy(pVisibleCopy, pStack->pEdit->pVisible, NumRows);

  /* hide all rows */
  for (i=0; i<NumRows; i++) {
    DDE_HideRow(pStack, i, FALSE);
  }

  /* re-do the row order */
  for (i=0; i<NumRows; i++) {
    pStack->pEdit->pRowOrder[i] = i;
  }

  /* display the visible rows -- they get inserted in the right spot */
  for (i=0; i<NumRows; i++) {
    if (pVisibleCopy[i]) {
      DDE_ShowRow(pStack, i, FALSE);
    }
  }

  VERIFY(MemFree(pVisibleCopy) == NULL);

  /* add pStack->pEdit to the stack */
  if (Save) {
    DDE_Add(pStack);
  }
  return(TRUE);
}


NLM_EXTERN Boolean DDE_ShiftRow(DDE_StackPtr pStack, Int4 Row, Int4 NumChars, Boolean Save,
                                Boolean ShiftBack) {
/*----------------------------------------------------------------------------
*  shift Row by NumChars.
*  shift right for NumChars > 0, left for NumChars < 0.
*  Save indicates if this edit is stored on the stack.
*  ShiftBack is true except when this function is called recursively.
*---------------------------------------------------------------------------*/
  Int4        i, Mag;
  ValNodePtr  ptxtList, prev_vnp=NULL;
  Boolean     GapsAdded=FALSE, Shifted=FALSE;
  Boolean     RightSide;
  Boolean*    pNoGaps;
  Boolean     NoGaps;

  if (NumChars == 0) {
    return(FALSE);
  }
  
  /* if the first column is aligned */
  if (DDE_FirstColumnIsAligned(pStack->pEdit)) {
    /* pad with unaligned gaps on left */
    /* it's easier to code this way -- I strip them later */
    DDE_AddGapToStartOfAllRows(pStack->pEdit, FALSE);
    GapsAdded = TRUE;
  }

  Mag = ABS(NumChars);
  for (i=0; i<Mag; i++) {
    /* get pointer to linked list of MsaTxtDisp's */
    ptxtList = DDE_GetTxtListPtr(pStack->pEdit, Row);
    if (NumChars > 0) {
      DDE_ShiftRowRight1(pStack->pEdit, Row, ptxtList, prev_vnp, TRUE);
    }
    else {

      /* I have a bug when the aligned block is 1 char wide and     */
      /* the row is shifted left.  e.g.  aBc should become abC.     */
      /* But first, the B is shifted left becoming lowercase.       */
      /* So, when the C's shifted left it doesn't know to become    */
      /* uppercase.  to combat this, I check how wide the block is. */
      /* if it's one char, I do a shift right boundary right, then  */
      /* the shift row, then a shift right boundary left.           */

      if (DDE_GetBlockWidth(pStack->pEdit, 0) == 1) {
        DDE_ShiftRightBoundaryRight1(pStack->pEdit, 0);
        Shifted = TRUE;
      }

      DDE_ShiftRowLeft1(pStack->pEdit, Row, ptxtList, TRUE);

      if (Shifted) {
        DDE_ShiftRightBoundaryLeft1(pStack->pEdit, 0);
      }
    }
  }

  /* here's where I remove those unwanted unaligned gaps */
  if (GapsAdded) {
    DDE_RemoveGapFromStartOfAllRows(pStack->pEdit, FALSE);
  }

  if (ShiftBack) {
    DDE_CleanEnds(pStack->pEdit);
    DDE_Verify(pStack->pEdit);
  }

  pNoGaps = (Boolean*) GetAppProperty("dde_nogaps");
  if (pNoGaps == NULL) {NoGaps = TRUE;}
  else                 {NoGaps = *pNoGaps;}
  if (NoGaps) {
    if (ShiftBack) {
      RightSide = NumChars < 0 ? TRUE : FALSE;
      DDE_RemoveAlignedGapsFromEndOfRow(pStack, Row, RightSide);
      DDE_CleanEnds(pStack->pEdit);
      DDE_Verify(pStack->pEdit);
    }
  }
  
  /* add pStack->pEdit to the stack */
  if (Save) {
    DDE_Add(pStack);
  }
  return(TRUE);
}


NLM_EXTERN ParaGPtr DDE_GetParaGPtr2(MsaParaGPopListPtr pPopList, Int4 Row) {
/*----------------------------------------------------------------------------
* return pointer to ParaG
*---------------------------------------------------------------------------*/
  ValNodePtr    pg_vnp;
  ParaGPtr      pgp;

  pg_vnp =  (ValNodePtr) (pPopList->TableHead[Row]);
  pgp =       (ParaGPtr) (pg_vnp->data.ptrvalue);
  return(pgp);
}


NLM_EXTERN ParaGPtr DDE_GetParaGPtr(DDE_InfoPtr pEditInfo, Int4 Row) {
/*----------------------------------------------------------------------------
* return pointer to ParaG
*---------------------------------------------------------------------------*/
  ValNodePtr    pg_vnp;
  ParaGPtr      pgp;

  pg_vnp =  (ValNodePtr) (pEditInfo->pPopList->TableHead[Row]);
  pgp =       (ParaGPtr) (pg_vnp->data.ptrvalue);
  return(pgp);
}


NLM_EXTERN ValNodePtr  DDE_GetTxtListPtr2(MsaParaGPopListPtr pPopList, Int4 Row) {
/*----------------------------------------------------------------------------
*  return pointer to linked-list of MsaTxtDisp structures
*---------------------------------------------------------------------------*/
  ValNodePtr    pg_vnp, msa_vnp;
  ParaGPtr      pgp;

  pg_vnp =  (ValNodePtr) (pPopList->TableHead[Row]);
  pgp =       (ParaGPtr) (pg_vnp->data.ptrvalue);
  msa_vnp = (ValNodePtr) (pgp->ptxtList);
  return(msa_vnp);
}


NLM_EXTERN ValNodePtr DDE_GetTxtListPtr(DDE_InfoPtr pEditInfo, Int4 Row) {
/*----------------------------------------------------------------------------
*  return pointer to linked-list of MsaTxtDisp structures
*---------------------------------------------------------------------------*/
  ValNodePtr    pg_vnp, msa_vnp;
  ParaGPtr      pgp;

  pg_vnp =  (ValNodePtr) (pEditInfo->pPopList->TableHead[Row]);
  pgp =       (ParaGPtr) (pg_vnp->data.ptrvalue);
  msa_vnp = (ValNodePtr) (pgp->ptxtList);
  return(msa_vnp);
}


NLM_EXTERN void DDE_SetTxtListPtr(DDE_InfoPtr pEditInfo, Int4 Row, ValNodePtr vnp) {
/*----------------------------------------------------------------------------
*  set pointer to linked-list of MsaTxtDisp structures
*---------------------------------------------------------------------------*/
  ValNodePtr    pg_vnp;
  ParaGPtr      pgp;

  pg_vnp =  (ValNodePtr) (pEditInfo->pPopList->TableHead[Row]);
  pgp =       (ParaGPtr) (pg_vnp->data.ptrvalue);
  pgp->ptxtList = vnp;
}


NLM_EXTERN Boolean DDE_ShiftRowLeft1(DDE_InfoPtr pEditInfo, Int4 Row,
                                     ValNodePtr ptxtList, Boolean OkToAddGaps) {
/*----------------------------------------------------------------------------
*  shift Row left by 1 char.
*  return value indicates if gaps were inserted to start of all rows.
*---------------------------------------------------------------------------*/
  ValNodePtr     last_vnp;
  MsaTxtDispPtr  msap, last_msap;
  Boolean        RetVal = FALSE;
  Boolean        IsDeleteChar, IsUnAligned;
  
  /* display_ParaG_content(pEditInfo->pPopListEdit, 2000); */

  if (ptxtList == NULL) {
    return(FALSE);
  }
  
  /* get first node in portion being shifted left */
  msap = (MsaTxtDispPtr) ptxtList->data.ptrvalue;

  /* check for a left-aligned aligned-gap */
  IsDeleteChar = DDE_IsLeftAlignedGapList(ptxtList, FALSE);

  /* if ok to add gaps to start of all rows */
  if (OkToAddGaps) {
    /* if first node is not a gap */
    if (!msap->IsGap) {
      /* and there's no left-aligned aligned-gap */
      if (!IsDeleteChar) {
        RetVal = TRUE;
        /* add an unaligned gap to the start of all rows */
        DDE_AddGapToStartOfAllRows(pEditInfo, TRUE);
        /* need to get pointer again because ptxtList changed */
        ptxtList = DDE_GetTxtListPtr(pEditInfo, Row);
        /* check for a left-aligned aligned-gap again */
        IsDeleteChar = DDE_IsLeftAlignedGapList(ptxtList, FALSE);
      }
    }
  }

  /* before merging and adding nodes, find if the last char is aligned or not */
  last_vnp = DDE_GetLastVNP(ptxtList);
  last_msap = (MsaTxtDispPtr) last_vnp->data.ptrvalue;
  IsUnAligned = last_msap->IsUnAligned;
  
  /* merge and add nodes after the gap, in preparation for shift left */
  DDE_LeftMergeAndAddNodeList(ptxtList, IsDeleteChar);
  /* shift all nodes after the gap left by one, insert gap on right */
  DDE_ShiftLeftList(pEditInfo, Row, ptxtList, IsDeleteChar, IsUnAligned);

  /* if there's a left-aligned aligned-gap to delete then delete it. */
  if (IsDeleteChar) {
    DDE_IsLeftAlignedGapList(ptxtList, TRUE);
  }

  return(RetVal);
}


NLM_EXTERN Boolean DDE_ShiftRowRight1(DDE_InfoPtr pEditInfo, Int4 Row, ValNodePtr ptxtList,
                                      ValNodePtr prev_vnp, Boolean InsertLeftGap) {
/*----------------------------------------------------------------------------
*  shift Row right by 1 char.
*  return value indicates if gaps were added to end of all rows.
*---------------------------------------------------------------------------*/
  ValNodePtr     stop_vnp;
  MsaTxtDispPtr  msap;
  Boolean        IsUnAligned, IsDeleteChar, RetVal=FALSE;
  
  /* check for a right-aligned aligned-gap */
  IsDeleteChar = DDE_IsRightAlignedGapList(&ptxtList, FALSE);

  /* if last node is not a gap */
  msap = (MsaTxtDispPtr) DDE_GetLastVNP(ptxtList)->data.ptrvalue;
  if (!msap->IsGap) {
    /* and there's no right-aligned aligned-gap */
    if (!IsDeleteChar) {
      /* add an unaligned gap to the end of all rows */
      DDE_AddGapToEndOfAllRows(pEditInfo, TRUE);
      RetVal = TRUE;
    }
  }

  /* before merging and adding nodes, find if the 1st char is aligned or not */
  msap = (MsaTxtDispPtr) (ptxtList->data.ptrvalue);
  IsUnAligned = msap->IsUnAligned;
  
  /* merge and add nodes before the gap, in preparation for shift right */
  /* if there's a new first node save it or point to it */
  if (DDE_RightMergeAndAddNodeList(&ptxtList)) {
    if (prev_vnp == NULL) {
      DDE_SetTxtListPtr(pEditInfo, Row, ptxtList);
    }
    else {
      prev_vnp->next = ptxtList;
    }
  }
  /* shift all nodes before the gap right by one. */
  /* if there's a new first node save it or point to it */
  if (IsDeleteChar) { stop_vnp = (ValNodePtr) -1; } /* stop shifting right at right-aligned gap */
  else              { stop_vnp = (ValNodePtr)  1; } /* shift right until the end */
  if (DDE_ShiftRightList(&ptxtList, stop_vnp, IsUnAligned, InsertLeftGap, TRUE)) {
    if (prev_vnp == NULL) {
      DDE_SetTxtListPtr(pEditInfo, Row, ptxtList);
    }
    else {
      prev_vnp->next = ptxtList;
    }
  }
  if (IsDeleteChar) {
    /* remove the first right-aligned aligned-gap */
    DDE_IsRightAlignedGapList(&ptxtList, TRUE);
  }

  return(RetVal);
}


NLM_EXTERN Int4 DDE_GetGapIndex(ValNodePtr ptxtList, ValNodePtr pMidtxtList, Boolean Before) {
/*----------------------------------------------------------------------------
*  get the display coordinate for a gap that would be inserted in the
*  linked list of MsaTxtDisp's.
*
*  ptxtList is the head of the linked list.
*  pMidtxtList is a node in the middle of the linked list.
*  Before tells whether the node goes before or after pMidtxtList.
*
*  return the index of the new gap.
*---------------------------------------------------------------------------*/
  Int4           DispCoordStart=0, DispCoordStop=-1;
  MsaTxtDispPtr  msap;
  ValNodePtr     vnp;
  
  vnp = ptxtList;
  while (vnp != NULL) {
    msap = (MsaTxtDispPtr) vnp->data.ptrvalue;
    DispCoordStart = DispCoordStop + 1;
    DispCoordStop =  DispCoordStart + (msap->to - msap->from);
    if (vnp == pMidtxtList) {
      break;
    }
    vnp = vnp->next;
  }
  return(Before ? DispCoordStart : DispCoordStop + 1);
}


NLM_EXTERN ValNodePtr DDE_GetMsaTxtNodeGivenBioseqCoord(MsaParaGPopListPtr pPopList,
                                                        Int4 BioseqCoord, Int4 Row) {
/*----------------------------------------------------------------------------
*  looked through linked list of ParaG's and linked list of MsaTxtDisp's
*  for the MsaTxtDisp that contains BioseqCoord.
*---------------------------------------------------------------------------*/
  ValNodePtr     pg_vnp, msa_vnp;
  ParaGPtr       pgp;
  MsaTxtDispPtr  msap;

  pg_vnp =  (ValNodePtr) (pPopList->TableHead[Row]);
  while (pg_vnp != NULL) {
    pgp =       (ParaGPtr) (pg_vnp->data.ptrvalue);
    msa_vnp = (ValNodePtr) (pgp->ptxtList);
    while (msa_vnp != NULL) {
      msap = (MsaTxtDispPtr) msa_vnp->data.ptrvalue;
      if (!msap->IsGap) {
        if ((BioseqCoord >= msap->from) && (BioseqCoord <= msap->to)) {
          return(msa_vnp);
        }
      }
      msa_vnp = msa_vnp->next;
    }
    pg_vnp = pg_vnp->next;
  }
  return(NULL);
}


NLM_EXTERN ValNodePtr DDE_GetMsaTxtNode2(ValNodePtr pg_head, Int4 DispCoord, Int4 PNTR pOffset) {
/*----------------------------------------------------------------------------
*  v.similar to DDE_GetMsaTxtNode, except...
*
*  the ValNodePtr passed to this routine is to a linked-list of ParaG's.
*  each ParaG contains a linked-list of MsaTxtDisp's.
*
*  Basically, this routine accomplishes the same as DDE_GetMsaTxtNode,
*  except it's more general, and can handle the case when a sequence is
*  more than 1 ParaG long.
*---------------------------------------------------------------------------*/
  ValNodePtr  vnp, msa_vnp, msa_node;
  ParaGPtr    pgp;

  vnp = pg_head;
  while (vnp != NULL) {
    pgp =       (ParaGPtr) (vnp->data.ptrvalue);
    msa_vnp = (ValNodePtr) (pgp->ptxtList);
    msa_node = DDE_GetMsaTxtNode(msa_vnp, DispCoord, pOffset);
    if (*pOffset != -1) {
      return(msa_node);
    }
    vnp = vnp->next;
  }
  return(NULL);
}


NLM_EXTERN ValNodePtr DDE_GetMsaTxtNode(ValNodePtr head, Int4 DispCoord, Int4 PNTR pOffset) {
/*----------------------------------------------------------------------------
*  head is a pointer to a linked list of MsaTxtDisp's.
*  this function finds the node of the linked list, and the index
*  into that node, where the DispCoord char is found.
*
*  returns ValNodePtr to a node of the linked list.
*  returns Offset which tells which char in the node (zero-based).
*
*  if the DispCoord is less than 0,
*    returns a ValNodePtr = head and Offset = -1
*  if the DispCoord is greater than the last DispCoordStop,
*    returns a NULL ValNodePtr and Offset = -1
*---------------------------------------------------------------------------*/
  ValNodePtr     vnp;
  MsaTxtDispPtr  msap;
  Int4           DispCoordStart=0, DispCoordStop=-1;
  
  /* DispCoord too small */
  if (DispCoord < 0) {
    *pOffset = -1;
    return(head);
  }
  /* walk through linked list, summing coords */
  vnp = head;
  while (vnp != NULL) {
    msap = (MsaTxtDispPtr) vnp->data.ptrvalue;
    DispCoordStart = DispCoordStop + 1;
    DispCoordStop =  DispCoordStart + (msap->to - msap->from);
    /* when DispCoord is in range */
    if ((DispCoord >= DispCoordStart) && (DispCoord <= DispCoordStop)) {
      /* return offset and node pointer */
      *pOffset = DispCoord - DispCoordStart;
      return(vnp);
    }
    vnp = vnp->next;
  }
  /* DispCoord not found in linked list */
  *pOffset = -1;
  return(NULL);
}


NLM_EXTERN ValNodePtr DDE_GetPrevVNP(ValNodePtr head, ValNodePtr curr) {
/*----------------------------------------------------------------------------
*  get vnp prior to curr.
*  return NULL if curr node is the first.
*  return NULL if curr is not found in the list.
*---------------------------------------------------------------------------*/
  ValNodePtr  vnp;

  if (head == curr) {
    return(NULL);
  }
  vnp = head;
  while (vnp != NULL) {
    if (vnp->next == curr) {
      return(vnp);
    }
    vnp = vnp->next;
  }
  return(NULL);
}


NLM_EXTERN void DDE_IncDisplayCoords(ValNodePtr vnp) {
/*----------------------------------------------------------------------------
*  increment display coordinates for the linked list of MsaTxtDisp's
*---------------------------------------------------------------------------*/
  MsaTxtDispPtr  msap;

  while (vnp != NULL) {
    msap = (MsaTxtDispPtr) vnp->data.ptrvalue;
    /* only increment display coords, not bioseq coords */
    if (msap->IsGap) {
      msap->from++;
      msap->to++;
    }
    vnp = vnp->next;
  }
  return;
}


NLM_EXTERN void DDE_DecDisplayCoords(ValNodePtr vnp) {
/*----------------------------------------------------------------------------
*  decrement display coordinates for the linked list of MsaTxtDisp's
*---------------------------------------------------------------------------*/
  MsaTxtDispPtr  msap;

  while (vnp != NULL) {
    msap = (MsaTxtDispPtr) vnp->data.ptrvalue;
    /* only decrement display coords, not bioseq coords */
    if (msap->IsGap) {
      msap->from--;
      msap->to--;
    }
    vnp = vnp->next;
  }
  return;
}


NLM_EXTERN ValNodePtr DDE_AddGapAndSplitNode(ValNodePtr pInsertNode, Int4 Offset, Int4 Pos) {
/*----------------------------------------------------------------------------
*  add a gap in the middle of the InsertNode.
*
*  pInsertNode is the node that will be split.
*  Offset tells where InsertNode is split.
*  Pos is the DisplayCoord of the new gap.
*---------------------------------------------------------------------------*/
  ValNodePtr     vnp;
  MsaTxtDispPtr  msap;
  Int4           to_save;
  
  msap = (MsaTxtDispPtr) pInsertNode->data.ptrvalue;
  ASSERT(!msap->IsUnAligned && !msap->IsGap);
  /* make sure node is split after the first char and before the last */
  ASSERT(Offset > 0);
  ASSERT(Offset <= (msap->to - msap->from));

  to_save = msap->to;
  /* shrink InsertNode to the part before the offset */
  msap->to = msap->from + Offset - 1;
  /* add a gap node after the non-gap node */
  vnp = DDE_AddMsaTxtNode(pInsertNode, Pos, Pos, TRUE, FALSE, FALSE);
  /* here's the 2nd half of the split node */
  vnp = DDE_AddMsaTxtNode(vnp, msap->to+1, to_save, FALSE, FALSE, FALSE);
  /* return pointer to 2nd half of split node to handle shift right of this node */
  return(vnp);
}


NLM_EXTERN ValNodePtr DDE_AddAGap(DDE_InfoPtr pEditInfo, Int4 Row,
                                  ValNodePtr ptxtList, Boolean Before, Boolean ForceUnAligned) {
/*----------------------------------------------------------------------------
*  add a gap somewhere in the linked list of MsaTxtDisp's.
*
*  if ForceUnAligned is TRUE and adding a gap after the last node or before the
*  first, add an unaligned gap.  if ForceUnAligned is FALSE and adding a
*  gap after the last node or before the first, the alignment of the new node
*  will agree with the first or last node.  all other gaps must be aligned.
*
*  ptxtList is the node in the list where the gap is added.
*  Before tells if the gap should be added just before or after this node.
*  pEditInfo and Row tell which row in the pPopListEdit is affected.
*  This is needed so that if the head pointer of the row is changed
*  it can be saved.
*
*  return a pointer to the next node whose numbering must be incremented.
*---------------------------------------------------------------------------*/
  Int4           GapIndex;
  ValNodePtr     head, tail, prev, next, return_vnp;
  MsaTxtDispPtr  msap, prev_msap, next_msap;

  /* get the head and tail of the linked-list */
  head = DDE_GetTxtListPtr(pEditInfo, Row);
  tail = DDE_GetLastVNP(head);
  
  msap = (MsaTxtDispPtr) ptxtList->data.ptrvalue;

  /* if adding a gap before the first node */
  if ((ptxtList == head) && (Before)) {
    if (ForceUnAligned) {
      if ((msap->IsGap) && (msap->IsUnAligned)) {
        /* make unaligned gap one longer */
        msap->to++;
      }
      else {
        /* insert unaligned gap */
        head = DDE_AddMsaTxtNode(head, 0, 0, TRUE, TRUE, TRUE);
        DDE_SetTxtListPtr(pEditInfo, Row, head);
      }
    }
    else {
      if (msap->IsGap) {
        /* make gap one longer */
        msap->to++;
      }
      else {
        /* insert gap */
        head = DDE_AddMsaTxtNode(head, 0, 0, TRUE, msap->IsUnAligned, TRUE);
        DDE_SetTxtListPtr(pEditInfo, Row, head);
      }
    }
    return_vnp = head->next;
  }

  /* if adding a gap after the last node */
  else if ((ptxtList == tail) && (!Before)) {
    if (ForceUnAligned) {
      if ((msap->IsGap) && (msap->IsUnAligned)) {
        /* make unaligned gap one longer */
        msap->to++;
      }
      else {
        /* insert unaligned gap */
        GapIndex = DDE_GetGapIndex(head, tail, FALSE);
        tail = DDE_AddMsaTxtNode(tail, GapIndex, GapIndex, TRUE, TRUE, FALSE);
      }
    }
    else {
      if (msap->IsGap) {
        /* make gap one longer */
        msap->to++;
      }
      else {
        /* insert gap */
        GapIndex = DDE_GetGapIndex(head, tail, FALSE);
        tail = DDE_AddMsaTxtNode(tail, GapIndex, GapIndex, TRUE, msap->IsUnAligned, FALSE);
      }
    }
    return_vnp = tail->next;
    ASSERT(return_vnp == NULL);
  }

  /* if this node is an aligned gap, make it one longer */
  else if ((msap->IsGap) && (!msap->IsUnAligned)) {
    msap->to++;
    /* if the next node is unaligned or non-existent */
    next = ptxtList->next;
    if (next != NULL) {
      next_msap = (MsaTxtDispPtr) next->data.ptrvalue;
    }
    if ((next == NULL) || (next_msap->IsUnAligned)) {
      /* this is ugly but I can't think of a better way */
      /* shift this node left one char and return a pointer to this */
      /* node rather than the next.  then shiftrowright1 will shift */
      /* a char from this aligned gap into the unaligned region */
      msap->to--;
      msap->from--;
      return_vnp = ptxtList;
    }
    else {
      return_vnp = ptxtList->next;
    }
  }

  /* if adding before this node */
  else if (Before) {
    /* if prev node is aligned gap make it one longer */
    prev = DDE_GetPrevVNP(head, ptxtList);
    prev_msap = (MsaTxtDispPtr) prev->data.ptrvalue;
    if ((!prev_msap->IsUnAligned) && (prev_msap->IsGap)) {
      prev_msap->to++;
    }
    /* otherwise if either node is aligned, insert an aligned gap */
    else if ((!msap->IsUnAligned) || (!prev_msap->IsUnAligned)) {
      GapIndex = DDE_GetGapIndex(head, ptxtList, TRUE);
      prev->next = DDE_AddMsaTxtNode(ptxtList, GapIndex, GapIndex, TRUE, FALSE, TRUE);
    }
    return_vnp = ptxtList;
  }

  /* if adding after this node */
  else {
    /* if next node is aligned gap make it one longer */
    next = ptxtList->next;
    next_msap = (MsaTxtDispPtr) next->data.ptrvalue;
    if ((!next_msap->IsUnAligned) && (next_msap->IsGap)) {
      next_msap->to++;
    }
    /* otherwise if either node is aligned, insert an aligned gap */
    else if ((!msap->IsUnAligned) || (!next_msap->IsUnAligned)) {
      GapIndex = DDE_GetGapIndex(head, ptxtList, FALSE);
      next = DDE_AddMsaTxtNode(ptxtList, GapIndex, GapIndex, TRUE, FALSE, FALSE);
    }
    return_vnp = next->next;
  }

  return(return_vnp);
}


NLM_EXTERN ValNodePtr DDE_RemoveAGap(DDE_InfoPtr pEditInfo, Int4 Row, ValNodePtr remove_vnp,
                                     Boolean* pNodeDeleted) {
/*----------------------------------------------------------------------------
*  Remove a gap from the remove_vnp.
*  return pointer to the next node whose numbering must be decremented.
*  return NodeDeleted if removing a gap removes a node.
*---------------------------------------------------------------------------*/
  ValNodePtr     head, prev_vnp=NULL, next_vnp, return_vnp;
  MsaTxtDispPtr  msap, prev_msap, next_msap;

  *pNodeDeleted = FALSE;
  
  /* get head of list and pointers to previous and next nodes */
  head = DDE_GetTxtListPtr(pEditInfo, Row);
  prev_vnp = DDE_GetPrevVNP(head, remove_vnp);
  next_vnp = remove_vnp->next;

  /* shrink remove-node by 1 */
  msap = (MsaTxtDispPtr) remove_vnp->data.ptrvalue;
  ASSERT(!msap->IsUnAligned && msap->IsGap);
  msap->to--;
  return_vnp = remove_vnp->next;

  /* if it's empty free it */
  if (msap->from > msap->to) {
    MemFree(msap);
    /* if first node is freed, save new head of list */
    if (remove_vnp == head) {
      DDE_SetTxtListPtr(pEditInfo, Row, remove_vnp->next);
    }
    /* otherwise remove the node from the linked list */
    else {
      ASSERT(prev_vnp != NULL);
      prev_vnp->next = remove_vnp->next;
    }
    /* and remove the node */
    MemFree(remove_vnp);
    *pNodeDeleted = TRUE;
  }

  /* if remove node was freed, try merging flanking nodes */
  if (*pNodeDeleted) {
    /* if there were nodes on either side of the remove-node */
    if ((prev_vnp != NULL) &&  (next_vnp != NULL)) {
      /* if they have the same alignment and gap settings */
      prev_msap = (MsaTxtDispPtr) prev_vnp->data.ptrvalue;
      next_msap = (MsaTxtDispPtr) next_vnp->data.ptrvalue;
      if ((prev_msap->IsUnAligned == next_msap->IsUnAligned) &&
          (prev_msap->IsGap == next_msap->IsGap)) {
        /* join the 2 nodes */
        if (prev_msap->IsGap) {
          /* for gap nodes, need to shift node right of remove-node left by 1 */
          prev_msap->to = next_msap->to-1;
        }
        else {
          prev_msap->to = next_msap->to;
        }
        /* remove next_msap from the linked list */
        MemFree(next_msap);
        prev_vnp->next = next_vnp->next;
        MemFree(next_vnp);
        return_vnp = prev_vnp->next;
      }
    }
  }

  return(return_vnp);
}


NLM_EXTERN Boolean DDE_RemoveGap(DDE_StackPtr pStack, Int4 Row, Int4 Pos, Boolean Save) {
/*----------------------------------------------------------------------------
*  Remove a gap in Row at positon Pos
*  Save indicates if this edit is stored on the stack.
*---------------------------------------------------------------------------*/
  Int4           Offset;
  ValNodePtr     head, remove_vnp, next_vnp, vnp;
  MsaTxtDispPtr  msap;
  Boolean        dummy, NodeDeleted, IsUnAligned, NodeAdded=FALSE;

  /* get node and offset, where gap is deleted */
  head = DDE_GetTxtListPtr(pStack->pEdit, Row);
  remove_vnp = DDE_GetMsaTxtNode(head, Pos, &Offset);
  if (remove_vnp == NULL)  {return(FALSE);}
  msap = (MsaTxtDispPtr) remove_vnp->data.ptrvalue;

  /* make sure node is aligned gap */
  if (msap->IsUnAligned || !msap->IsGap) {
    return(FALSE);
  }

  /* determine if last node is unaligned */
  vnp = DDE_GetLastVNP(head);
  msap = (MsaTxtDispPtr) vnp->data.ptrvalue;
  IsUnAligned = msap->IsUnAligned;
  
  /* remove gap */
  next_vnp = DDE_RemoveAGap(pStack->pEdit, Row, remove_vnp, &NodeDeleted);

  /* shift row (to right of gap) left */
  head = DDE_GetTxtListPtr(pStack->pEdit, Row);
  vnp = DDE_GetPrevVNP(head, next_vnp);
  if (vnp != NULL) {
    msap = (MsaTxtDispPtr) vnp->data.ptrvalue;
    if (!msap->IsUnAligned) {
      if (DDE_LeftMerge(vnp)) {
        next_vnp = vnp->next;
      }
      else if (DDE_LeftAddNode(vnp, &dummy)) {
        NodeAdded = TRUE;
        next_vnp = vnp->next;
      }
    }
  }
  if (next_vnp == NULL) {
    /* this makes sure a gap is shifted in on the right */
    /* even when there's nothing more to shift left */
    DDE_ShiftLeftList(pStack->pEdit, Row, next_vnp, FALSE, IsUnAligned);
  }
  else {
    if (NodeAdded) {
      /* don't do the same node twice */
      DDE_LeftMergeAndAddNodeList(next_vnp->next, FALSE);
    }
    else {
      DDE_LeftMergeAndAddNodeList(next_vnp, FALSE);
    }
    DDE_ShiftLeftList(pStack->pEdit, Row, next_vnp, FALSE, IsUnAligned);
  }

  DDE_CleanEnds(pStack->pEdit);
  DDE_Verify(pStack->pEdit);

  /* add pStack->pEdit to the stack */
  if (Save) {
    DDE_Add(pStack);
  }
  return(TRUE);
}


NLM_EXTERN Boolean DDE_InsertGap(DDE_StackPtr pStack, Int4 Row, Int4 Pos, Boolean Save) {
/*----------------------------------------------------------------------------
*  add a gap to Row just before the display coordinate given by Pos.
*  Save indicates if this edit is stored on the stack.
*---------------------------------------------------------------------------*/
  Int4           Offset;
  ValNodePtr     ptxtList, pInsertNode, next_vnp, prev_vnp;
  MsaTxtDispPtr  msap;

  /* get node and offset, where gap is inserted */
  ptxtList = DDE_GetTxtListPtr(pStack->pEdit, Row);
  pInsertNode = DDE_GetMsaTxtNode(ptxtList, Pos, &Offset);
  if (pInsertNode == NULL)  {return(FALSE);}
  msap = (MsaTxtDispPtr) pInsertNode->data.ptrvalue;

  if (msap->IsUnAligned) {
    /* no gap inserted into an unaligned region */
    return(FALSE);
  }
  if ((Offset == 0) || (msap->IsGap)) {
    /* insert gap prior to an insert node or prior to an aligned gap node */
    next_vnp = DDE_AddAGap(pStack->pEdit, Row, pInsertNode, TRUE, FALSE);
  }
  else {
    /* insert a gap in an aligned non-gap node */
    next_vnp = DDE_AddGapAndSplitNode(pInsertNode, Offset, Pos);
  }
  /* shift row to right of gap right */
  prev_vnp = DDE_GetPrevVNP(ptxtList, next_vnp);
  DDE_ShiftRowRight1(pStack->pEdit, Row, next_vnp, prev_vnp, FALSE);

  DDE_Verify(pStack->pEdit);

  /* add pStack->pEdit to the stack */
  if (Save) {
    DDE_Add(pStack);
  }
  return(TRUE);
}


NLM_EXTERN Boolean DDE_CleanEnds(DDE_InfoPtr pEditInfo) {
/*----------------------------------------------------------------------------
*  remove all columns of unaligned gaps from the start and end of the rows.
*  return TRUE if any columns were removed.
*---------------------------------------------------------------------------*/
  Boolean RetVal=FALSE;

  while (DDE_RemoveGapFromStartOfAllRows(pEditInfo, FALSE)) {
    RetVal = TRUE;
  }
  while (DDE_RemoveGapFromEndOfAllRows(pEditInfo, FALSE)) {
    RetVal = TRUE;
  }
  if (RetVal) {
    DDE_ReMakeRuler(pEditInfo->pPopList, TRUE, -1);
  }
  return(RetVal);
}


NLM_EXTERN Boolean DDE_RemoveGapFromStartOfAllRows(DDE_InfoPtr pEditInfo, Boolean ReMakeRuler) {
/*----------------------------------------------------------------------------
*  if every row starts with an unaligned gap, remove 1 from each row.
*  return TRUE if the removal is performed.
*---------------------------------------------------------------------------*/
  Int4           i, NumRows;
  ParaGPtr       pgp;
  ValNodePtr     vnp, new_vnp;
  MsaTxtDispPtr  msap;

  NumRows = pEditInfo->TotalNumRows;

  /* check if each row starts with unaligned gap */
  for (i=0; i<NumRows; i++) {
    vnp = DDE_GetTxtListPtr(pEditInfo, i);
    msap = (MsaTxtDispPtr) vnp->data.ptrvalue;
    if ((!msap->IsUnAligned) || (!msap->IsGap)) {
      /* return FALSE if any row starts with aligned or non-gap char */
      return(FALSE);
    }
  }

  /* decrement number of columns */
  pEditInfo->pPopList->LengthAli--;

  /* shift each row left */
  for (i=0; i<NumRows; i++) {
    vnp = DDE_GetTxtListPtr(pEditInfo, i);
    msap = (MsaTxtDispPtr) vnp->data.ptrvalue;
    /* remove 1st gap, free node if it's empty */
    msap->to--;
    if (msap->from > msap->to) {
      MemFree(msap);
      new_vnp = vnp->next;
      DDE_SetTxtListPtr(pEditInfo, i, new_vnp);
      MemFree(vnp);
      vnp = new_vnp;
    }
    else {
      vnp = vnp->next;
    }
    DDE_DecDisplayCoords(vnp);
    /* decrement number of columns in this row */
    pgp = DDE_GetParaGPtr(pEditInfo, i);
    pgp->StopLetter--;
  }

  if (ReMakeRuler) {
    DDE_ReMakeRuler(pEditInfo->pPopList, TRUE, -1);
  }

  return(TRUE);
}


NLM_EXTERN Boolean DDE_RemoveGapFromEndOfAllRows(DDE_InfoPtr pEditInfo, Boolean ReMakeRuler) {
/*----------------------------------------------------------------------------
*  if every row ends with an unaligned gap, remove 1 from each row.
*  return TRUE if the removal is performed.
*---------------------------------------------------------------------------*/
  Int4           i, NumRows;
  ParaGPtr       pgp;
  ValNodePtr     vnp, last_vnp, prev_vnp;
  MsaTxtDispPtr  last_msap;

  NumRows = pEditInfo->TotalNumRows;

  /* check if each row ends with unaligned gap */
  for (i=0; i<NumRows; i++) {
    vnp = DDE_GetTxtListPtr(pEditInfo, i);
    last_vnp = DDE_GetLastVNP(vnp);
    last_msap = (MsaTxtDispPtr) last_vnp->data.ptrvalue;
    if ((!last_msap->IsUnAligned) || (!last_msap->IsGap)) {
      /* return FALSE if any row starts with aligned or non-gap char */
      return(FALSE);
    }
  }

  /* decrement number of columns */
  pEditInfo->pPopList->LengthAli--;

  /* remove last char of each row */
  for (i=0; i<NumRows; i++) {
    vnp = DDE_GetTxtListPtr(pEditInfo, i);
    last_vnp = DDE_GetLastVNP(vnp);
    prev_vnp = DDE_GetPrevVNP(vnp, last_vnp);
    last_msap = (MsaTxtDispPtr) last_vnp->data.ptrvalue;
    last_msap->to--;
    if (last_msap->from > last_msap->to) {
      MemFree(last_msap);
      prev_vnp->next = NULL;
      MemFree(last_vnp);
    }
    /* decrement number of columns in this row */
    pgp = DDE_GetParaGPtr(pEditInfo, i);
    pgp->StopLetter--;
  }

  if (ReMakeRuler) {
    DDE_ReMakeRuler(pEditInfo->pPopList, TRUE, -1);
  }

  return(TRUE);
}


NLM_EXTERN void DDE_AddGapToStartOfAllRows(DDE_InfoPtr pEditInfo, Boolean ReMakeRuler) {
/*----------------------------------------------------------------------------
*  add a gap to the start of all rows
*---------------------------------------------------------------------------*/
  Int4              i, NumRows;
  ValNodePtr        ptxtList, vnp;
  ParaGPtr          pgp;

  /* get total number of rows, increment number of columns */
  NumRows = pEditInfo->TotalNumRows;
  pEditInfo->pPopList->LengthAli++;

  /* for each row */
  for (i=0; i<NumRows; i++) {
    /* add a gap before the 1st node */
    ptxtList = DDE_GetTxtListPtr(pEditInfo, i);
    vnp = DDE_AddAGap(pEditInfo, i, ptxtList, TRUE, TRUE);
    /* adjust the numbering on the rest of the nodes */
    DDE_IncDisplayCoords(vnp);
    /* increment number of columns in this row */
    pgp = DDE_GetParaGPtr(pEditInfo, i);
    pgp->StopLetter++;
  }

  if (ReMakeRuler) {
    DDE_ReMakeRuler(pEditInfo->pPopList, TRUE, -1);
  }

  return;
}


NLM_EXTERN void DDE_AddGapToEndOfAllRows(DDE_InfoPtr pEditInfo, Boolean ReMakeRuler) {
/*----------------------------------------------------------------------------
*  add a gap to the end of all rows.
*---------------------------------------------------------------------------*/
  Int4              i, NumRows;
  ValNodePtr        ptxtList, last_vnp, vnp;
  ParaGPtr          pgp;

  /* get total number of rows, increment number of columns */
  NumRows = pEditInfo->TotalNumRows;
  pEditInfo->pPopList->LengthAli++;

  /* for each row */
  for (i=0; i<NumRows; i++) {
    /* add a gap after the last node */
    ptxtList = DDE_GetTxtListPtr(pEditInfo, i);
    last_vnp = DDE_GetLastVNP(ptxtList);
    vnp = DDE_AddAGap(pEditInfo, i, last_vnp, FALSE, TRUE);
    ASSERT(vnp == NULL);
    /* increment number of columns in this row */
    pgp = DDE_GetParaGPtr(pEditInfo, i);
    pgp->StopLetter++;
  }

  if (ReMakeRuler) {
    DDE_ReMakeRuler(pEditInfo->pPopList, TRUE, -1);
  }

  return;
}


NLM_EXTERN ValNodePtr DDE_GetLastVNP(ValNodePtr vnp) {
/*----------------------------------------------------------------------------
*  return pointer to last node.
*---------------------------------------------------------------------------*/
  ValNodePtr  last_vnp;

  while (vnp != NULL) {
    last_vnp = vnp;
    vnp = vnp->next;
  }
  return(last_vnp);
}


NLM_EXTERN Int4 DDE_GetNumResidues(MsaParaGPopListPtr pPopList, Int4 Row) {
/*----------------------------------------------------------------------------
*  get the number of residues in Row of pPopList.
*---------------------------------------------------------------------------*/
  ValNodePtr     vnp;
  MsaTxtDispPtr  msap;
  Int4           from=-1, to;

  vnp = DDE_GetTxtListPtr2(pPopList, Row);
  while (vnp != NULL) {
    msap = (MsaTxtDispPtr) vnp->data.ptrvalue;
    if (!msap->IsGap) {
      if (from == -1) {
        from = msap->from;
      }
      to = msap->to;
    }
    vnp = vnp->next;
  }
  return((to-from) + 1);
}


NLM_EXTERN ValNodePtr DDE_AddRulerDescrNode(ValNodePtr pRulerDescr,
                                            Int4 DispStart, Int4 DispStop, Int4 AlignStart,
                                            Boolean IsUnAligned, Boolean Before) {
/*----------------------------------------------------------------------------
*  Insert a new ValNode into the linked list.
*  The new ValNode points to a DDVRulerDescr.
*  If the linked list is empty (pRulerDescr is NULL), start the linked list.
*  If Before is TRUE, insert the node prior to pRulerDescr,
*  Otherwise, insert the node after the first node in pRulerDescr.
*
*  returns a pointer to the new ValNode.
*---------------------------------------------------------------------------*/
  ValNodePtr        new_vnp, temp_vnp;
  DDVRulerDescrPtr  new_rdp;

  new_rdp = MemNew(sizeof(DDVRulerDescr));
  new_rdp->disp_start = DispStart;
  new_rdp->disp_stop = DispStop;
  new_rdp->align_start = AlignStart;
  new_rdp->bUnAligned = IsUnAligned;
  new_vnp = ValNodeNew(NULL);
  new_vnp->choice = 0;
  new_vnp->data.ptrvalue = new_rdp;
  if (Before || (pRulerDescr == NULL)) {
    new_vnp->next = pRulerDescr;
  }
  else {
    temp_vnp = pRulerDescr->next;
    pRulerDescr->next = new_vnp;
    new_vnp->next = temp_vnp;
  }
  return(new_vnp);
}


NLM_EXTERN void DDE_SetTextStyle(MsaTxtDispPtr msap) {
/*----------------------------------------------------------------------------
*  set the TextStyle of this node, based on its IsGap and IsUnAligned flags.
*---------------------------------------------------------------------------*/
  if (msap->IsGap) {
    if (msap->IsUnAligned) {
      msap->TextStyle = MSA_TXT_STYLE_UAGAP;
    }
    else {
      msap->TextStyle = MSA_TXT_STYLE_GAP;
    }
  }
  else {
    msap->TextStyle = MSA_TXT_STYLE_SEQ;
  }
  return;
}


NLM_EXTERN ValNodePtr DDE_AddMsaTxtNode(ValNodePtr ptxtList, Int4 from, Int4 to,
                                        Boolean IsGap, Boolean IsUnAligned, Boolean Before) {
/*----------------------------------------------------------------------------
*  Insert a new ValNode into the linked list.
*  The new ValNode points to an MsaTxtDisp.
*  If Before is TRUE, insert the node prior to pxtList,
*  Otherwise, insert the node after the first node in ptxtList.
*
*  returns a pointer to the new ValNode.
*---------------------------------------------------------------------------*/
  ValNodePtr     new_vnp, temp_vnp;
  MsaTxtDispPtr  new_msap, msap;

  msap = (MsaTxtDispPtr) ptxtList->data.ptrvalue;
  new_msap = UDV_MsaTxtDispNew(msap);
  new_msap->from = from;
  new_msap->to = to;
  new_msap->IsGap = IsGap;
  new_msap->IsUnAligned = IsUnAligned;
  DDE_SetTextStyle(new_msap);
  new_vnp = ValNodeNew(NULL);
  new_vnp->choice = 0;
  new_vnp->data.ptrvalue = new_msap;
  if (Before) {
    new_vnp->next = ptxtList;
  }
  else {
    temp_vnp = ptxtList->next;
    ptxtList->next = new_vnp;
    new_vnp->next = temp_vnp;
  }
  return(new_vnp);
}


NLM_EXTERN Boolean DDE_IsLeftAlignedGap(ValNodePtr vnp) {
/*----------------------------------------------------------------------------
*  check if this node is unaligned, followed by an aligned-gap node.
*
*        this-node   next-node
*        ---------   ---------
*            u           -
*            ~           -
*
*    where u = unaligned,      A = aligned
*          ~ = unaligned-gap,  - = aligned-gap
*
*  returns TRUE if it is.
*---------------------------------------------------------------------------*/
  ValNodePtr     next_vnp;
  MsaTxtDispPtr  msap, next_msap;

  next_vnp = vnp->next;
  if (next_vnp == NULL)  {return(FALSE);}
  msap = (MsaTxtDispPtr) vnp->data.ptrvalue;
  next_msap = (MsaTxtDispPtr) next_vnp->data.ptrvalue;

  if (msap->IsUnAligned) {
    if ((!next_msap->IsUnAligned) && (next_msap->IsGap)) {
      return(TRUE);
    }
  }
  return(FALSE);
}


NLM_EXTERN Boolean DDE_IsTerminalRightAlignedGap(ValNodePtr vnp) {
/*----------------------------------------------------------------------------
*  if this node is an aligned-gap node and it terminates the linked list
*  returns TRUE.
*---------------------------------------------------------------------------*/
  MsaTxtDispPtr  msap;

  msap = (MsaTxtDispPtr) vnp->data.ptrvalue;

  if ((!msap->IsUnAligned) && (msap->IsGap)) {
    if (vnp->next == NULL) {
      return(TRUE);
    }
  }
  return(FALSE);
}


NLM_EXTERN Boolean DDE_IsRightAlignedGap(ValNodePtr vnp) {
/*----------------------------------------------------------------------------
*  check if this node is an aligned-gap node, followed by an unaligned node.
*
*        this-node   next-node
*        ---------   ---------
*            -           u
*            -           ~
*
*    where u = unaligned,      A = aligned
*          ~ = unaligned-gap,  - = aligned-gap
*
*  returns TRUE if it is.
*---------------------------------------------------------------------------*/
  ValNodePtr     next_vnp;
  MsaTxtDispPtr  msap, next_msap;

  next_vnp = vnp->next;
  if (next_vnp == NULL)  {return(FALSE);}
  msap = (MsaTxtDispPtr) vnp->data.ptrvalue;
  next_msap = (MsaTxtDispPtr) next_vnp->data.ptrvalue;

  if (next_msap->IsUnAligned) {
    if ((!msap->IsUnAligned) && (msap->IsGap)) {
      return(TRUE);
    }
  }
  return(FALSE);
}


NLM_EXTERN Boolean DDE_IsLeftAlignedGapList(ValNodePtr vnp, Boolean DeleteIt) {
/*----------------------------------------------------------------------------
*  search linked-list of MsaTxt's for an unaligned node followed by
*  an aligned gap node.  If DeleteIt is true, remove the first char from
*  the aligned gap node.  Remove the node if it becomes empty.
*
*  returns TRUE if such a pair of nodes is found in the linked-list.
*---------------------------------------------------------------------------*/
  ValNodePtr     next_vnp;
  MsaTxtDispPtr  next_msap;

  ASSERT(vnp != NULL);
  next_vnp = vnp->next;
  if (next_vnp == NULL)  {return(FALSE);}

  /* look through each node for left-aligned gap */
  while (next_vnp != NULL) {
    if (DDE_IsLeftAlignedGap(vnp)) {
      if (DeleteIt) {
        /* delete a char from the next node */
        next_msap = (MsaTxtDispPtr) next_vnp->data.ptrvalue;
        next_msap->from++;
        /* if it's now empty, delete the node */
        if (next_msap->from > next_msap->to) {
          MemFree(next_msap);
          vnp->next = next_vnp->next;
          MemFree(next_vnp);
        }
      }
      return(TRUE);
    }
    vnp = next_vnp;
    next_vnp = vnp->next;
  }
  /* none found -- return FALSE */
  return(FALSE);
}


NLM_EXTERN Boolean DDE_IsRightAlignedGapList(ValNodePtr PNTR vnpp, Boolean DeleteIt) {
/*----------------------------------------------------------------------------
*  search linked-list of MsaTxt's for an aligned gap node followed by an
*  unaligned node.  If DeleteIt is true, remove a char from the aligned gap
*  node.  Remove the node if it becomes empty.
*
*  returns TRUE if such a pair of nodes is found in the linked-list.
*  returns modified head of list if first node was replaced.
*---------------------------------------------------------------------------*/
  ValNodePtr     vnp, next_vnp, prev_vnp;
  MsaTxtDispPtr  msap;

  vnp = *vnpp;

  prev_vnp = NULL;
  if (vnp == NULL)       {return(FALSE);}
  next_vnp = vnp->next;
  if (next_vnp == NULL)  {return(FALSE);}

  /* look through each node for aligned-gap followed by an unaligned node */
  while (next_vnp != NULL) {
    if (DDE_IsRightAlignedGap(vnp)) {
      if (DeleteIt) {
        /* delete a char from the aligned node */
        msap = (MsaTxtDispPtr) vnp->data.ptrvalue;
        msap->from++;
        /* if it's now empty, delete the node */
        if (msap->from > msap->to) {
          MemFree(msap);
          if (prev_vnp == NULL) {
            /* this handles the extremely remote case that the first node is deleted */
            *vnpp = vnp->next;
          }
          else {
            /* point the previous node to the next one */
            prev_vnp->next = vnp->next;
          }
          MemFree(vnp);
        }
      }
      return(TRUE);
    }
    prev_vnp = vnp;
    vnp = next_vnp;
    next_vnp = vnp->next;
  }
  /* none found -- return FALSE */
  return(FALSE);
}


NLM_EXTERN void DDE_MergeNodesLists(DDE_InfoPtr pEditInfo) {
/*----------------------------------------------------------------------------
*  look through the linked-lists of MsaTxtDisp's seeing if nodes can be merged.
*---------------------------------------------------------------------------*/
  ValNodePtr  vnp;
  Int4        i, NumRows;

  /* get number of rows */
  NumRows = pEditInfo->TotalNumRows;

  /* for each row */
  for (i=0; i<NumRows; i++) {
    /* get head of linked list */
    vnp = DDE_GetTxtListPtr(pEditInfo, i);
    /* check through each node for merger with the one that follows */
    while (vnp != NULL) {
      if (!DDE_MergeNodes(vnp)) {
        /* only advance to next node when modes are not merged */
        /* (if nodes are merged, don't advance -- i.e. check for 3 in a row) */
        vnp = vnp->next;
      }
    }
  }
}


NLM_EXTERN Boolean DDE_MergeNodes(ValNodePtr ptxtList) {
/*----------------------------------------------------------------------------
*  if the first 2 nodes in ptxtList have the same settings for
*  IsUnAligned and IsGap, then merge the nodes and return TRUE.
*---------------------------------------------------------------------------*/
  MsaTxtDispPtr  msap, next_msap;
  ValNodePtr     vnp,  next_vnp;

  vnp = ptxtList;
  next_vnp = vnp->next;
  if (next_vnp == NULL) return(FALSE);
  msap = (MsaTxtDispPtr) vnp->data.ptrvalue;
  next_msap = (MsaTxtDispPtr) next_vnp->data.ptrvalue;

  /* if this node and the next have the same settings */
  if ((msap->IsUnAligned == next_msap->IsUnAligned) &&
      (msap->IsGap == next_msap->IsGap)) {
    /* make this one longer */
    msap->to = next_msap->to;
    /* free the next one */
    MemFree(next_msap);
    vnp->next = next_vnp->next;
    MemFree(next_vnp);
    return(TRUE);
  }
  return(FALSE);
}


NLM_EXTERN Boolean DDE_LeftMerge(ValNodePtr vnp) {
/*----------------------------------------------------------------------------
*
*  This is in preparation for a shift left.
*
*  If this node and the one after it meet one of these conditions:
*        this-node   next-node
*        ---------   ---------
*            u           A
*            A           u
*            -           ~
*
*    where u = unaligned,      A = aligned
*          ~ = unaligned-gap,  - = aligned-gap
*
*  Then:
*    merge this node with the first char in the next node.
*    remove the first char from the next node.
*    remove the next node if it becomes empty.
*
*  Return TRUE if a merge was performed.
*
*---------------------------------------------------------------------------*/
  ValNodePtr     next_vnp;
  MsaTxtDispPtr  msap, next_msap;
  Boolean        Condition = FALSE;

  next_vnp = vnp->next;
  if (next_vnp == NULL) {
    return(FALSE);
  }
  msap = (MsaTxtDispPtr) vnp->data.ptrvalue;
  next_msap = (MsaTxtDispPtr) next_vnp->data.ptrvalue;

  /* check for proper merge condition */
  if (msap->IsGap == next_msap->IsGap) {
    if (msap->IsUnAligned != next_msap->IsUnAligned) {
      if (!DDE_IsLeftAlignedGap(vnp)) {
        Condition = TRUE;
      }
    }
  }

  /* if condition found */
  if (Condition) {
    /* add a char to this node, remove one from the next node */
    msap->to++;
    next_msap->from++;
    /* if next node is now empty, set it free */
    if (next_msap->from > next_msap->to) {
      MemFree(next_msap);
      vnp->next = next_vnp->next;
      MemFree(next_vnp);
    }
  }

  return(Condition);
}


NLM_EXTERN Boolean DDE_RightMerge(ValNodePtr PNTR vnpp, Boolean PNTR pHeadChanged) {
/*----------------------------------------------------------------------------
*
*  This is in preparation for a shift right.
*
*  If this node and the one after it meet one of these conditions:
*        this-node   next-node
*        ---------   ---------
*            A           u
*            u           A
*            ~           -
*
*    where u = unaligned,      A = aligned
*          ~ = unaligned-gap,  - = aligned-gap
*
*  Then:
*    merge the last char in this node with the next node.
*    remove the last char from this node.
*    remove this node if it becomes empty.
*
*  Return TRUE if a merge was performed.
*  Return modified vnp if this node was deleted.
*  Return HeadChanged = TRUE if this node was deleted.
*
*---------------------------------------------------------------------------*/
  ValNodePtr     next_vnp, vnp;
  MsaTxtDispPtr  msap, next_msap;
  Boolean        Condition=FALSE;

  *pHeadChanged = FALSE;
  vnp = *vnpp;

  next_vnp = vnp->next;
  msap = (MsaTxtDispPtr) vnp->data.ptrvalue;
  next_msap = (MsaTxtDispPtr) next_vnp->data.ptrvalue;

  /* check for proper merge condition */
  if (msap->IsGap == next_msap->IsGap) {
    if (msap->IsUnAligned != next_msap->IsUnAligned) {
      if (!DDE_IsRightAlignedGap(vnp)) {
        Condition = TRUE;
      }
    }
  }

  /* if condition found */
  if (Condition) {
    /* remove a char from this node, add one to the next node */
    msap->to--;
    next_msap->from--;
    /* if this node is now empty, set it free */
    if (msap->from > msap->to) {
      MemFree(msap);
      *vnpp = vnp->next;
      MemFree(vnp);
      *pHeadChanged = TRUE;
    }
  }

  return(Condition);
}


NLM_EXTERN Boolean DDE_LeftAddNode(ValNodePtr vnp, Boolean PNTR pNodeDeleted) {
/*----------------------------------------------------------------------------
*
*  This is in preparation for a shift left.
*
*  If this node and the one after it meet one of the conditions below,
*  then a new node of a single char is inserted in between.
*
*        this-node   next-node    new-node
*        ---------   ---------    --------
*            ~           A           u
*            A           ~           -
*            -           u           A
*
*    where u = unaligned,      A = aligned
*          ~ = unaligned-gap,  - = aligned-gap
*
*  If the condition is found:
*    create a new node as shown.
*    remove the first char from the next node.
*    remove the next node if it becomes empty.
*    insert new node between this node and the next.
*
*  Return TRUE if a node was added.
*  Return NodeDeleted = TRUE if a node was deleted as well.
*
*---------------------------------------------------------------------------*/
  ValNodePtr     next_vnp, new_vnp;
  MsaTxtDispPtr  msap, next_msap;
  Boolean        Condition=FALSE;

  *pNodeDeleted = FALSE;

  next_vnp = vnp->next;
  if (next_vnp == NULL) {
    return(FALSE);
  }
  msap = (MsaTxtDispPtr) vnp->data.ptrvalue;
  next_msap = (MsaTxtDispPtr) next_vnp->data.ptrvalue;

  /* check for proper add node condition */
  if (msap->IsGap != next_msap->IsGap) {
    if (msap->IsUnAligned != next_msap->IsUnAligned) {
      if (!DDE_IsLeftAlignedGap(vnp)) {
        Condition = TRUE;
      }
    }
  }

  /* if condition found */
  if (Condition) {
    /* add new node after vnp and before next_vnp */
    /* before adding node:  vnp->next_vnp */
    /* after adding node:   vnp->new_vnp->next_vnp */
    new_vnp = DDE_AddMsaTxtNode(vnp, next_msap->from, next_msap->from, 
                                     next_msap->IsGap, msap->IsUnAligned, FALSE);
    /* remove one char from the next node */
    next_msap->from++;
    /* if next node is now empty, set it free */
    if (next_msap->from > next_msap->to) {
      MemFree(next_msap);
      /* don't forget: new_vnp was inserted */
      new_vnp->next = next_vnp->next;
      MemFree(next_vnp);
      *pNodeDeleted = TRUE;
    }
  }

  return(Condition);
}


NLM_EXTERN Boolean DDE_RightAddNode(ValNodePtr PNTR vnpp, Boolean PNTR pHeadChanged) {
/*----------------------------------------------------------------------------
*
*  This is in preparation for a shift right.
*
*  If this node and the one after it meet one of the conditions below,
*  then a new node of a single char is inserted in between.
*
*        this-node   next-node    new-node
*        ---------   ---------    --------
*            ~           A           -
*            A           ~           u
*            u           -           A
*
*    where u = unaligned,      A = aligned
*          ~ = unaligned-gap,  - = aligned-gap
*
*  If the condition is found:
*    create a new node as shown.
*    remove the last char from this node.
*    remove this node if it becomes empty.
*    insert new node between this node and the next.
*
*  Return TRUE if a node was added.
*  Return modified vnp if this node was deleted.
*  Return HeadChanged = TRUE if this node was deleted.
*
*---------------------------------------------------------------------------*/
  ValNodePtr     vnp, next_vnp, new_vnp;
  MsaTxtDispPtr  msap, next_msap;
  Boolean        Condition=FALSE;

  *pHeadChanged = FALSE;
  vnp = *vnpp;

  next_vnp = vnp->next;
  msap = (MsaTxtDispPtr) vnp->data.ptrvalue;
  next_msap = (MsaTxtDispPtr) next_vnp->data.ptrvalue;

  /* check for proper add node condition */
  if (msap->IsGap != next_msap->IsGap) {
    if (msap->IsUnAligned != next_msap->IsUnAligned) {
      if (!DDE_IsRightAlignedGap(vnp)) {
        Condition = TRUE;
      }
    }
  }

  /* if condition found */
  if (Condition) {
    /* add new node after vnp and before next_vnp */
    /* before adding node:  vnp->next_vnp */
    /* after adding node:   vnp->new_vnp->next_vnp */
    new_vnp = DDE_AddMsaTxtNode(vnp, msap->to, msap->to,
                                     msap->IsGap, next_msap->IsUnAligned, FALSE);
    /* remove one char from this node */
    msap->to--;
    /* if this node is now empty, set it free */
    if (msap->from > msap->to) {
      MemFree(msap);
      *vnpp = vnp->next;
      MemFree(vnp);
      *pHeadChanged = TRUE;
    }
  }

  return(Condition);
}


NLM_EXTERN void DDE_LeftMergeAndAddNodeList(ValNodePtr vnp, Boolean AfterGap) {
/*----------------------------------------------------------------------------
*  Search linked-list of MsaTxtDisp's.  Add nodes and merge nodes where
*  required, in preparation for a shift left.  If AfterGap is true, only add
*  or merge nodes after the first left-aligned-gap.
*---------------------------------------------------------------------------*/
  ValNodePtr  next_vnp;
  Int4        DeleteGapCnt=0;
  Boolean     RetVal1, RetVal2, IsLeftAlignedGap, NodeDeleted;

  if (vnp == NULL) {
    return;
  }
  next_vnp = vnp->next;
  if (next_vnp == NULL) {
    return;
  }

  /* look through each node */
  while (next_vnp != NULL) {
    /* check for gap to delete, before adding and merging nodes*/
    IsLeftAlignedGap = DDE_IsLeftAlignedGap(vnp);
    /* if not looking for left aligned gap or found one */
    if ((!AfterGap) || ((DeleteGapCnt > 0) && (AfterGap))) {
      /* add extra node, if required */
      RetVal1 = DDE_LeftAddNode(vnp, &NodeDeleted);
      /* skip a node if one was added and none was removed */
      /*if (RetVal1 && (!NodeDeleted)) { */
      /* skip a node if one was added */
      if (RetVal1) {
        vnp = vnp->next;
        next_vnp = vnp->next;
      }
      /* merge this node with 1st char of next, if required. */
      /* merging nodes and adding nodes are mutually exclusive. */
      if (!RetVal1) {
        RetVal2 = DDE_LeftMerge(vnp);
        /* left merge could delete the last node */
        if (vnp->next == NULL) {
          break;
        }
      }
    }
    /* increment gap count, after adding and merging nodes */
    if (IsLeftAlignedGap) {
      DeleteGapCnt++;
    }
    vnp = vnp->next;
    if (vnp == NULL) {next_vnp = NULL;}
    else             {next_vnp = vnp->next;}
  }
  return;
}


NLM_EXTERN Boolean DDE_RightMergeAndAddNodeList(ValNodePtr PNTR vnpp) {
/*----------------------------------------------------------------------------
*  Search linked list of MsaTxtDisp's.  Add nodes and merge nodes where
*  required, in preparation for a shift right.  Only add or merge nodes
*  before the first right-aligned-gap.
*
*  return TRUE if the head of the linked list changed.
*---------------------------------------------------------------------------*/
  ValNodePtr  vnp, next_vnp, prev_vnp;
  Boolean     RetVal=FALSE, RetVal1, RetVal2, HeadChanged;

  vnp = *vnpp;

  /* to start, vnp is 1st node, next_vnp is 2nd node */
  prev_vnp = NULL;
  next_vnp = vnp->next;
  if (next_vnp == NULL)  {return(RetVal);}

  /* look through each node */
  while (next_vnp != NULL) {
    /* if a right-aligned gap is spotted, then finished merging and adding */
    if (DDE_IsRightAlignedGap(vnp)) {
      break;
    }
    /* add extra node, if required */
    RetVal1 = DDE_RightAddNode(&vnp, &HeadChanged);
    /* skip a node if one was added and none was removed */
    if (RetVal1 && (!HeadChanged)) {
      prev_vnp = vnp;
      vnp = vnp->next;
      next_vnp = vnp->next;
    }
    /* merge last char of this node with next node, if required. */
    /* merging nodes and adding nodes are mutually exclusive. */
    if (!RetVal1) {
      RetVal2 = DDE_RightMerge(&vnp, &HeadChanged);
    }
    /* if the head node changed, make sure to link prev node to new head */
    if (HeadChanged) {
      if (prev_vnp == NULL) {
        *vnpp = vnp;
        RetVal = TRUE;
      }
      else {
        prev_vnp->next = vnp;
      }
      next_vnp = vnp->next;
    }
    else {
      prev_vnp = vnp;
      vnp = vnp->next;
      next_vnp = vnp->next;
    }
  }
  return(RetVal);
}


NLM_EXTERN void DDE_ShiftLeftList(DDE_InfoPtr pEditInfo, Int4 Row, ValNodePtr ptxtList,
                                  Boolean AfterGap, Boolean LastCharIsUnAligned) {
/*----------------------------------------------------------------------------
*  Shift each node of linked-list of MsaTxtDisp's left by 1 char starting
*  with ptxtList.
*  If AfterGap is true, only shift nodes left after a left-aligned-gap.
*  LastCharIsUnAligned tells whether gap shifted in on right is aligned.
*---------------------------------------------------------------------------*/
  Int4             GapIndex, DeleteGapCnt=0;
  MsaTxtDispPtr    msap;
  ValNodePtr       vnp, vnp_save, head, prev_vnp, last_vnp;
  Boolean          NewHead = FALSE;

  head = DDE_GetTxtListPtr(pEditInfo, Row);
  
  /* look through each node */
  vnp = ptxtList;
  while (vnp != NULL) {
    /* if not looking for left aligned gap or found one */
    if ((!AfterGap) || ((DeleteGapCnt > 0) && (AfterGap))) {
      /* shift the block left by 1 char */
      msap = (MsaTxtDispPtr) vnp->data.ptrvalue;
      if (msap->IsGap) {
        msap->from = (msap->from == 0 ? 0 : msap->from-1);
        msap->to--;
      }
      /* if the 1st block is empty, remove it */
      /* (only the 1st block can decrease in size) */
      if (msap->from > msap->to) {
        /* make sure it's the first node */
        ASSERT(vnp = ptxtList);
        MemFree(msap);
        if (vnp == head) {
          DDE_SetTxtListPtr(pEditInfo, Row, vnp->next);
          vnp_save = vnp->next;
        }
        else {
          prev_vnp = DDE_GetPrevVNP(head, vnp);
          prev_vnp->next = vnp->next;
          vnp_save = vnp->next;
        }
        MemFree(vnp);
        /* vnp was free'd, so have to reset head of list */
        vnp = vnp_save;
        NewHead = TRUE;
      }
    }
    /* count number of gaps on left edge of aligned block */
    if (DDE_IsLeftAlignedGap(vnp)) {
      DeleteGapCnt++;
    }
    if (!NewHead) {
      vnp = vnp->next;
    }
    NewHead = FALSE;
  }

  /* shift in a gap on the right.  if the last node is a gap with */
  /* the same alignment as LastCharIsUnAligned ... */
  head = DDE_GetTxtListPtr(pEditInfo, Row);
  last_vnp = DDE_GetLastVNP(head);
  msap = (MsaTxtDispPtr) last_vnp->data.ptrvalue;
  GapIndex = DDE_GetGapIndex(head, last_vnp, FALSE);
  if (AfterGap) {
    GapIndex--;
  }
  if ((msap->IsGap) && (msap->IsUnAligned == LastCharIsUnAligned)) {
    /* make last node one longer */
    msap->to++;
    ASSERT(msap->to == GapIndex);
  }
  /* otherwise make a new node after the last one */
  else {
    DDE_AddMsaTxtNode(last_vnp, GapIndex, GapIndex, TRUE, LastCharIsUnAligned, FALSE);
  }

  return;
}


NLM_EXTERN Boolean DDE_ShiftRightList(ValNodePtr PNTR vnpp, ValNodePtr StopNode,
                                      Boolean FirstCharIsUnAligned, Boolean InsertLeftGap,
                                      Boolean OkToShrinkLastNode) {
/*----------------------------------------------------------------------------
*  Shift each node of linked-list of MsaTxtDisp's right by 1 char.
*  If StopNode is -1, shift nodes right before the first right-aligned-gap.
*  Otherwise, shift nodes right before StopNode.
*  InsertLeftGap tells whether or not a char is shifted in on the left.
*  If FirstCharIsUnAligned, shift in an UnAligned gap on the left,
*  and vice-versa.
*
*  return TRUE if a new first node is added.
*  return modified vnpp, if a new first node is added.
*---------------------------------------------------------------------------*/
  MsaTxtDispPtr    msap;
  ValNodePtr       vnp, save_vnp=NULL, prev_vnp=NULL;
  Boolean          DeleteGapFound=FALSE, RetVal=FALSE;

  vnp = *vnpp;
  
  /* look through each node */
  while (vnp != NULL) {
    if (StopNode == (ValNodePtr)-1) {
      /* finish shifting right when a right-aligned gap is found */
      if ((DDE_IsRightAlignedGap(vnp)) ||
          (DDE_IsTerminalRightAlignedGap(vnp))) {
        DeleteGapFound = TRUE;
        break;
      }
    }
    else {
      /* finish shifting right when StopNode is found */
      if (vnp == StopNode) {
        DeleteGapFound = TRUE;
        break;
      }
    }
    /* shift block right by 1 char */
    msap = (MsaTxtDispPtr) vnp->data.ptrvalue;
    if (msap->IsGap) {
      msap->from++;
      msap->to++;
    }
    prev_vnp = save_vnp;
    save_vnp = vnp;
    vnp = vnp->next;
  }

  /* shrink the last node by 1, delete it if it's empty */
  /* last node only shrinks if there is no aligned gap to delete */
  /* and if it's gapped */
  if (!DeleteGapFound && OkToShrinkLastNode) {
    vnp = save_vnp;
    msap = (MsaTxtDispPtr) vnp->data.ptrvalue;
    if (msap->IsGap) {
      msap->to--;
      if (msap->from > msap->to) {
        MemFree(msap);
        if (prev_vnp != NULL) {
          /* make prev node the end of the linked list */
          prev_vnp->next = NULL;
        }
        else {
          /* if there is no previous node the head of the list is the end */
          /* I can't imagine how this would ever happen */
          *vnpp = NULL;
          RetVal = TRUE;
        }
        MemFree(vnp);
      }
    }
  }

  /* shift in a gap on the left */
  /* if the first node is a gap with the same alignment as FirstCharIsUnAligned... */
  if (InsertLeftGap) {
    vnp = *vnpp;
    msap = (MsaTxtDispPtr) vnp->data.ptrvalue;
    if ((msap->IsGap) && (msap->IsUnAligned == FirstCharIsUnAligned)) {
      /* make first node one longer */
      msap->from--;
      ASSERT(msap->from == 0);
    }
    /* otherwise make a new node before the first one */
    else {
      *vnpp = DDE_AddMsaTxtNode(vnp, 0, 0, TRUE, FirstCharIsUnAligned, TRUE);
      RetVal = TRUE;
    }
  }

  return(RetVal);
}


NLM_EXTERN Int4 DDE_GetNumBlocks2(MsaParaGPopListPtr pPopList) {
/*----------------------------------------------------------------------------
*  get number of alignment blocks.  use the ruler.
*---------------------------------------------------------------------------*/
  ValNodePtr        vnp, head;
  DDVRulerDescrPtr  rdp;
  Int4              Count=0;

  vnp = head = pPopList->RulerDescr;
  /* look through all the nodes of the ruler */
  while (vnp != NULL) {
    rdp = (DDVRulerDescrPtr) vnp->data.ptrvalue;
    /* if an aligned node is found, increment the count */
    if (!rdp->bUnAligned) {
      Count++;
    }
    vnp = vnp->next;
  }
  return(Count);
}


NLM_EXTERN Int4 DDE_GetNumBlocks(DDE_InfoPtr pEditInfo) {
/*----------------------------------------------------------------------------
*  get number of alignment blocks
*  (it's sufficient to check one row)
*---------------------------------------------------------------------------*/
  ValNodePtr  vnp;
  Int4        Count=0;

  vnp = DDE_GetTxtListPtr(pEditInfo, 0);
  /* count aligned to unaligned transitions */
  while (vnp != NULL) {
    if (DDE_IsEndOfAlignment(vnp)) {
      Count++;
    }
    vnp = vnp->next;
  }
  return(Count); 
}


NLM_EXTERN Int4 DDE_GetBlockWidth(DDE_InfoPtr pEditInfo, Int4 BlockIndex) {
/*----------------------------------------------------------------------------
*  return the width of the BlockIndex alignment block.
*  return -1 if BlockIndex not found.
*---------------------------------------------------------------------------*/
  Int4  Start, Stop;

  Start = DDE_GetAlignStart(pEditInfo, BlockIndex);
  Stop =  DDE_GetAlignStop(pEditInfo, BlockIndex);
  if ((Start == -1) || (Stop == -1)) {
    return(-1);
  }
  return(Start - Stop + 1);
}


NLM_EXTERN Int4 DDE_GetAlignStart(DDE_InfoPtr pEditInfo, Int4 BlockIndex) {
/*----------------------------------------------------------------------------
*  return the display coordinate index for the start of the
*  BlockIndex alignment block.  return -1 if BlockIndex not found.
*---------------------------------------------------------------------------*/
  return(DDE_GetAlignStart2(pEditInfo->pPopList, BlockIndex));
}


NLM_EXTERN Int4 DDE_GetAlignStart2(MsaParaGPopListPtr pPopList, Int4 BlockIndex) {
/*----------------------------------------------------------------------------
*  return the display coordinate index for the start of the
*  BlockIndex alignment block.  return -1 if BlockIndex not found.
*---------------------------------------------------------------------------*/
  MsaTxtDispPtr  msap;
  ValNodePtr     head, vnp;
  Int4           Count=0;

  head = vnp = DDE_GetTxtListPtr2(pPopList, 0);
  if (head == NULL) return(-1);

  msap = (MsaTxtDispPtr) vnp->data.ptrvalue;
  if (msap == NULL) return(-1);

  /* for start of first block that's not preceeded by unaligned chars */
  if (BlockIndex==0 && !msap->IsUnAligned && vnp==head) {
    /* block starts at 0 */
    return(0);
  }

  /* look through the linked list of MsaTxtDisp's */
  while (vnp!= NULL) {
    /* if we're at the correct block */
    if (Count == BlockIndex) {
      /* if vnp is unaligned and vnp->next is aligned */
      if (DDE_IsStartOfAlignment(vnp)) {
        /* return display coordinate for char following vnp */
        return(DDE_GetGapIndex(head, vnp, FALSE));
      }
    }
    if (DDE_IsEndOfAlignment(vnp)) {
      /* count unaligned to aligned transitions */
      Count++;
    }
    vnp = vnp->next;
  }
  return(-1);
}


NLM_EXTERN Int4 DDE_GetAlignStop(DDE_InfoPtr pEditInfo, Int4 BlockIndex) {
/*----------------------------------------------------------------------------
*  return the display coordinate index for the start of the
*  BlockIndex alignment block.  return -1 if BlockIndex not found.
*---------------------------------------------------------------------------*/
  return(DDE_GetAlignStop2(pEditInfo->pPopList, BlockIndex));
}


NLM_EXTERN Int4 DDE_GetAlignStop2(MsaParaGPopListPtr pPopList, Int4 BlockIndex) {
/*----------------------------------------------------------------------------
*  return the display coordinate index for the end of the
*  BlockIndex alignment block.  return -1 if BlockIndex not found.
*---------------------------------------------------------------------------*/
  ValNodePtr  head, vnp;
  Int4        Count=0;

  head = vnp = DDE_GetTxtListPtr2(pPopList, 0);

  /* look through the linked list of MsaTxtDisp's */
  while (vnp!= NULL) {
    /* if we're at the correct block */
    if (Count == BlockIndex) {
      /* if vnp is aligned and vnp->next is either unaligned or doesn't exist */
      if (DDE_IsEndOfAlignment(vnp)) {
        /* return display coordinate for last aligned char */
        return(DDE_GetGapIndex(head, vnp, FALSE) - 1);
      }
    }
    if (DDE_IsEndOfAlignment(vnp)) {
      /* count unaligned to aligned transitions */
      Count++;
    }
    vnp = vnp->next;
  }
  return(-1);
}


NLM_EXTERN Boolean DDE_FirstColumnIsAligned(DDE_InfoPtr pEditInfo) {
/*----------------------------------------------------------------------------
*  return TRUE if the first column is aligned.
*  (it's sufficient to check one row)
*---------------------------------------------------------------------------*/
  ValNodePtr     vnp;
  MsaTxtDispPtr  msap;

  vnp = DDE_GetTxtListPtr(pEditInfo, 0);
  msap = (MsaTxtDispPtr) vnp->data.ptrvalue;
  if (msap->IsUnAligned) {
    return(FALSE);
  }
  return(TRUE);
}


NLM_EXTERN Boolean DDE_LastColumnIsAligned(DDE_InfoPtr pEditInfo) {
/*----------------------------------------------------------------------------
*  return TRUE if the last column is aligned.
*  (it's sufficient to check one row)
*---------------------------------------------------------------------------*/
  ValNodePtr     vnp;
  MsaTxtDispPtr  msap;

  vnp = DDE_GetTxtListPtr(pEditInfo, 0);
  vnp = DDE_GetLastVNP(vnp);
  msap = (MsaTxtDispPtr) vnp->data.ptrvalue;
  if (msap->IsUnAligned) {
    return(FALSE);
  }
  return(TRUE);
}


NLM_EXTERN Boolean DDE_IsStartOfAlignment(ValNodePtr vnp) {
/*----------------------------------------------------------------------------
*  return TRUE if this node is unaligned and the next one is aligned.
*---------------------------------------------------------------------------*/
  ValNodePtr     next_vnp;
  MsaTxtDispPtr  msap, next_msap;

  next_vnp = vnp->next;
  if (next_vnp == NULL)  { return(FALSE); }
  msap = (MsaTxtDispPtr) vnp->data.ptrvalue;
  next_msap = (MsaTxtDispPtr) next_vnp->data.ptrvalue;

  if (msap->IsUnAligned && !next_msap->IsUnAligned) {
    return(TRUE);
  }
  return(FALSE);
}


NLM_EXTERN Boolean DDE_IsEndOfAlignment(ValNodePtr vnp) {
/*----------------------------------------------------------------------------
*  return TRUE if this node is aligned and the next one is either
*  unaligned or non-exsistant.
*---------------------------------------------------------------------------*/
  ValNodePtr     next_vnp;
  MsaTxtDispPtr  msap, next_msap;

  msap = (MsaTxtDispPtr) vnp->data.ptrvalue;
  next_vnp = vnp->next;
  if (next_vnp == NULL) {
    if (!msap->IsUnAligned) {
      /* if this node is Aligned and no next node return TRUE */
      return(TRUE);
    }
    return(FALSE);
  }

  next_msap = (MsaTxtDispPtr) next_vnp->data.ptrvalue;
  if (!msap->IsUnAligned && next_msap->IsUnAligned) {
    /* if this node is Aligned and next node is unaligned return TRUE */
    return(TRUE);
  }
  return(FALSE);
}


NLM_EXTERN void DDE_ShiftBlock(DDE_StackPtr pStack, Int4 BlockIndex, Int4 NumChars, Boolean Save) {
/*----------------------------------------------------------------------------
*  shift both the right and left boundaries by NumChars
*  Save indicates if this edit is stored on the stack.
*---------------------------------------------------------------------------*/
  Int4  i, Mag;

  Mag = ABS(NumChars);
  for (i=0; i<Mag; i++) {
    if (NumChars > 0) {
      DDE_ShiftRightBoundary(pStack, BlockIndex, 1, FALSE);
      DDE_ShiftLeftBoundary(pStack, BlockIndex, 1, FALSE);
    }
    else {
      DDE_ShiftLeftBoundary(pStack, BlockIndex, -1, FALSE);
      DDE_ShiftRightBoundary(pStack, BlockIndex, -1, FALSE);
    }
  }

  /* add pStack->pEdit to the stack */
  if (Save) {
    DDE_Add(pStack);
  }
  return;
}


NLM_EXTERN Int4 DDE_DeleteBlock(DDE_StackPtr pStack, Int4 BlockIndex, Boolean Save) {
/*----------------------------------------------------------------------------
*  delete the BlockIndex block
*  Save indicates if this edit is stored on the stack.
*  return value indicates the number of positions the right boundary
*  is shifted in order to delete the block. (0 means block not deleted)
*---------------------------------------------------------------------------*/
  Int4  OldNumBlocks, NewNumBlocks;
  Int4  RetVal=0;

  OldNumBlocks = DDE_GetNumBlocks(pStack->pEdit);

  /* shift the right boundary left until an aligned region disappears */
  while (DDE_ShiftRightBoundaryLeft1(pStack->pEdit, BlockIndex)) {
    RetVal++;
  }

  /* remake the DDVRuler */
  DDE_ReMakeRuler(pStack->pEdit->pPopList, TRUE, -1);

  NewNumBlocks = DDE_GetNumBlocks(pStack->pEdit);

  /* if the number of aligned blocks changed */
  if (NewNumBlocks != OldNumBlocks) {
    /* merge abutting unaligned nodes */
    DDE_MergeNodesLists(pStack->pEdit);
  }
  else {
    RetVal = 0;
  }

  DDE_CleanEnds(pStack->pEdit);
  DDE_Verify(pStack->pEdit);

  if (Save) {
    /* if a block was deleted */
    if (RetVal) {
      /* add pStack->pEdit to the stack */
      DDE_Add(pStack);
    }
  }
  return(RetVal);
}


NLM_EXTERN Boolean DDE_ShiftLeftBoundary(DDE_StackPtr pStack, Int4 BlockIndex, Int4 NumChars, Boolean Save) {
/*----------------------------------------------------------------------------
*  Shift the left boundary of the BlockIndex block by NumChars.
*  Save indicates if this edit is stored on the stack.
*  return TRUE if there's more aligned region.
*  return FALSE if an aligned block disappeared, or there's no aligned block.
*---------------------------------------------------------------------------*/
  Int4     i, Mag;
  Int4     OldNumBlocks, NewNumBlocks;
  Boolean  RetVal=TRUE;
  Boolean* pNoGaps;
  Boolean  NoGaps;

  /* get number of aligned blocks */
  OldNumBlocks = DDE_GetNumBlocks(pStack->pEdit);
  if (OldNumBlocks == 0) {
    return(FALSE);
  }
  
  Mag = ABS(NumChars);
  for (i=0; i<Mag; i++) {
    if (NumChars > 0) {
      DDE_ShiftLeftBoundaryRight1(pStack->pEdit, BlockIndex);
    }
    else {
      DDE_ShiftLeftBoundaryLeft1(pStack->pEdit, BlockIndex);
    }
  }

  pNoGaps = (Boolean*) GetAppProperty("dde_nogaps");
  if (pNoGaps == NULL) {NoGaps = TRUE;}
  else                 {NoGaps = *pNoGaps;}
  if (NoGaps) {
    DDE_RemoveAlignedGapsFromEnds(pStack->pEdit);
  }

  /* every time an alignment boundary changes, remake the DDVRuler */
  DDE_ReMakeRuler(pStack->pEdit->pPopList, TRUE, -1);

  /* get new number of aligned blocks */
  NewNumBlocks = DDE_GetNumBlocks(pStack->pEdit);

  /* if the number of aligned blocks changed */
  if (NewNumBlocks != OldNumBlocks) {
    /* merge abutting unaligned nodes */
    DDE_MergeNodesLists(pStack->pEdit);
    RetVal = FALSE;
  }

  DDE_CleanEnds(pStack->pEdit);
  DDE_Verify(pStack->pEdit);

  if (Save) {
    DDE_Add(pStack);
  }
  return(RetVal);
}


NLM_EXTERN Boolean DDE_ShiftRightBoundary(DDE_StackPtr pStack, Int4 BlockIndex, Int4 NumChars, Boolean Save) {
/*----------------------------------------------------------------------------
*  shift the right boundary of the block by NumChars
*  Save indicates if this edit is stored on the stack.
*  return TRUE if there's more aligned region.
*  return FALSE if an aligned block disappeared, or there's no aligned block.
*---------------------------------------------------------------------------*/
  Int4     i, Mag;
  Int4     OldNumBlocks, NewNumBlocks;
  Boolean  RetVal=TRUE;
  Boolean* pNoGaps;
  Boolean  NoGaps;

  /* get number of aligned blocks */
  OldNumBlocks = DDE_GetNumBlocks(pStack->pEdit);
  if (OldNumBlocks == 0) {
    return(FALSE);
  }
  
  Mag = ABS(NumChars);
  for (i=0; i<Mag; i++) {
    if (NumChars > 0) {
      DDE_ShiftRightBoundaryRight1(pStack->pEdit, BlockIndex);
    }
    else {
      DDE_ShiftRightBoundaryLeft1(pStack->pEdit, BlockIndex);
    }
  }

  pNoGaps = (Boolean*) GetAppProperty("dde_nogaps");
  if (pNoGaps == NULL) {NoGaps = TRUE;}
  else                 {NoGaps = *pNoGaps;}
  if (NoGaps) {
    DDE_RemoveAlignedGapsFromEnds(pStack->pEdit);
  }

  /* every time an alignment boundary changes, remake the DDVRuler */
  DDE_ReMakeRuler(pStack->pEdit->pPopList, TRUE, -1);

  /* get new number of aligned blocks */
  NewNumBlocks = DDE_GetNumBlocks(pStack->pEdit);

  /* if the number of aligned blocks changed */
  if (NewNumBlocks != OldNumBlocks) {
    /* merge abutting unaligned nodes */
    DDE_MergeNodesLists(pStack->pEdit);
    RetVal = FALSE;
  }

  DDE_CleanEnds(pStack->pEdit);
  DDE_Verify(pStack->pEdit);

  if (Save) {
    DDE_Add(pStack);
  }
  return(RetVal);
}


NLM_EXTERN Boolean DDE_ShiftLeftBoundaryLeft1(DDE_InfoPtr pEditInfo, Int4 BlockIndex) {
/*----------------------------------------------------------------------------
*  shift the left boundary of the BlockIndex block left by 1 char.
*  return TRUE if a boundary was shifted.
*---------------------------------------------------------------------------*/
  Int4        i, NumRows, BlockCount=0;
  ValNodePtr  vnp, prev_vnp=NULL;
  Boolean     HeadChanged, RetVal=FALSE;

  /* if the first column is aligned, and shifting left boundary of first block */
  if (DDE_FirstColumnIsAligned(pEditInfo) && (BlockIndex==0)) {
    /* pad with unaligned gaps on left */
    DDE_AddGapToStartOfAllRows(pEditInfo, FALSE);
  }

  NumRows = pEditInfo->TotalNumRows;
  /* look through each row */
  for (i=0; i<NumRows; i++) {
    vnp = DDE_GetTxtListPtr(pEditInfo, i);
    /* look through each node */
    while (vnp != NULL) {
      /* if at an unaligned to aligned transition */
      if (DDE_IsStartOfAlignment(vnp)) {
        /* and at the proper aligned block */
        if (BlockIndex == BlockCount) {
          /* make node adjustments to shift boundary left */
          if (DDE_RightMerge(&vnp, &HeadChanged)) {
            i=i;
          }
          else if (DDE_RightAddNode(&vnp, &HeadChanged)) {
            i=i;
          }
          /* if the head node changed, link prev node to new head */
          if (HeadChanged) {
            if (prev_vnp == NULL) {
              DDE_SetTxtListPtr(pEditInfo, i, vnp);
            }
            else {
              prev_vnp->next = vnp;
            }
          }
          /* skip to next row */
          RetVal = TRUE;
          break;
        }
        else {
          BlockCount++;
        }
      }
      prev_vnp = vnp;
      vnp = vnp->next;
    }
  }
  return(RetVal);
}


NLM_EXTERN Boolean DDE_ShiftLeftBoundaryRight1(DDE_InfoPtr pEditInfo, Int4 BlockIndex) {
/*----------------------------------------------------------------------------
*  shift the left boundary of the BlockIndex block right by 1 char.
*  return TRUE if a boundary was shifted.
*---------------------------------------------------------------------------*/
  Int4        i, NumRows, BlockCount=0;
  ValNodePtr  vnp, prev_vnp=NULL, stop_vnp, save_vnp, head;
  Boolean     dummy, GapsAdded=FALSE, RetVal=FALSE;

  /* if the first column is aligned, and shifting left boundary of first block */
  if (DDE_FirstColumnIsAligned(pEditInfo) && (BlockIndex==0)) {
    /* pad with unaligned gaps on left */
    /* it's easier to code this way -- I strip them later */
    DDE_AddGapToStartOfAllRows(pEditInfo, FALSE);
    GapsAdded = TRUE;
  }

  NumRows = pEditInfo->TotalNumRows;
  /* look through each row */
  for (i=0; i<NumRows; i++) {
    vnp = DDE_GetTxtListPtr(pEditInfo, i);
    head = save_vnp = vnp;
    /* look through each node */
    while (vnp != NULL) {
      /* if at an unaligned to aligned transition */
      if (DDE_IsStartOfAlignment(vnp)) {
        /* and at the proper aligned block */
        if (BlockIndex == BlockCount) {
          /* make node adjustments to shift boundary right */
          if (DDE_LeftMerge(vnp)) {
            i=i;
          }
          else if (DDE_LeftAddNode(vnp, &dummy)) {
            i=i;
          }
          /* this handles the case when the aligned block starts with a gap. */
          /* delete the gap and shift the row prior to the gap right 1 char. */
          else {
            ASSERT(DDE_IsLeftAlignedGap(vnp));
            prev_vnp = DDE_GetPrevVNP(head, save_vnp);
            stop_vnp = DDE_RemoveAGap(pEditInfo, i, vnp->next, &dummy);
            if (DDE_ShiftRightList(&save_vnp, stop_vnp, TRUE, TRUE, FALSE)) {
              if (prev_vnp == NULL) {
                DDE_SetTxtListPtr(pEditInfo, i, save_vnp);
              }
              else {
                prev_vnp->next = save_vnp;
              }
            }
          }
          /* skip to next row */
          RetVal = TRUE;
          break;
        }
        else {
          BlockCount++;
        }
      }
      /* in case not shifting boundary of first block, */
      /* save the point where ShiftRightList begins */
      else if (DDE_IsEndOfAlignment(vnp)) {
        save_vnp = vnp->next;
      }
      prev_vnp = vnp;
      vnp = vnp->next;
    }
  }

  /* here's where I remove those unwanted unaligned gaps */
  if (GapsAdded) {
    DDE_RemoveGapFromStartOfAllRows(pEditInfo, FALSE);
  }
  return(RetVal);
}


NLM_EXTERN Boolean DDE_ShiftRightBoundaryLeft1(DDE_InfoPtr pEditInfo, Int4 BlockIndex) {
/*----------------------------------------------------------------------------
*  shift the right boundary of the BlockIndex block left by 1 char.
*  return TRUE if a boundary was shifted.
*---------------------------------------------------------------------------*/
  Int4           NumBlocks, i, NumRows, Index, BlockCount=0;
  Boolean        HeadChanged, GapsAdded=FALSE, RetVal=FALSE, IsUnAligned, dummy;
  ValNodePtr     head, vnp, prev_vnp=NULL, next_vnp;
  MsaTxtDispPtr  msap;

  /* if the last column is aligned, and shifting right boundary of last block */
  NumBlocks = DDE_GetNumBlocks(pEditInfo);
  if (DDE_LastColumnIsAligned(pEditInfo) && (BlockIndex==(NumBlocks-1))) {
    /* pad with unaligned gaps on right */
    /* it's easier to code this way -- I strip them later */
    DDE_AddGapToEndOfAllRows(pEditInfo, FALSE);
    GapsAdded = TRUE;
  }

  NumRows = pEditInfo->TotalNumRows;
  /* look through each row */
  for (i=0; i<NumRows; i++) {
    head = vnp = DDE_GetTxtListPtr(pEditInfo, i);
    /* look through each node */
    while (vnp != NULL) {
      /* if at an aligned to unaligned transition */
      if (DDE_IsEndOfAlignment(vnp)) {
        /* and at the proper aligned block */
        if (BlockIndex == BlockCount) {
          /* make node adjustments to shift boundary left */
          if (DDE_RightMerge(&vnp, &HeadChanged)) {
            i=i;
          }
          else if (DDE_RightAddNode(&vnp, &HeadChanged)) {
            i=i;
          }
          /* this handles the case when the aligned block ends with a gap. */
          /* delete the gap and shift the rest of the row left 1 char. */
          else {
            ASSERT(DDE_IsRightAlignedGap(vnp));
            Index = DDE_GetGapIndex(head, vnp, TRUE);
            next_vnp = DDE_RemoveAGap(pEditInfo, i, vnp, &dummy);
            if (next_vnp == NULL) {
              /* this makes sure a gap is shifted in on the right */
              /* even when there's nothing more to shift left */
              next_vnp = DDE_GetLastVNP(head);
              msap = (MsaTxtDispPtr) next_vnp->data.ptrvalue;
              IsUnAligned = msap->IsUnAligned;
              DDE_ShiftLeftList(pEditInfo, i, NULL, FALSE, IsUnAligned);
            }
            else {
              DDE_ShiftRowLeft1(pEditInfo, i, next_vnp, FALSE);
            }
          }
          /* if the head node changed, link prev node to new head */
          if (HeadChanged) {
            if (prev_vnp == NULL) {
              DDE_SetTxtListPtr(pEditInfo, i, vnp);
            }
            else {
              prev_vnp->next = vnp;
            }
          }
          /* skip to next row */
          RetVal = TRUE;
          break;
        }
        else {
          BlockCount++;
        }
      }
      prev_vnp = vnp;
      vnp = vnp->next;
    }
  }

  /* here's where I remove those unwanted unaligned gaps */
  if (GapsAdded) {
    DDE_RemoveGapFromEndOfAllRows(pEditInfo, FALSE);
  }
  return(RetVal);
}


NLM_EXTERN Boolean DDE_ShiftRightBoundaryRight1(DDE_InfoPtr pEditInfo, Int4 BlockIndex) {
/*----------------------------------------------------------------------------
*  shift the right boundary of the BlockIndex block right by 1 char.
*  return TRUE if a boundary was shifted.
*---------------------------------------------------------------------------*/
  Int4        i, NumRows, BlockCount=0, NumBlocks;
  ValNodePtr  vnp;
  Boolean     dummy, RetVal=FALSE;

  /* if the last column is aligned, and shifting right boundary of last block */
  NumBlocks = DDE_GetNumBlocks(pEditInfo);
  if (DDE_LastColumnIsAligned(pEditInfo) && (BlockIndex==(NumBlocks-1))) {
    /* pad with unaligned gaps on right */
    DDE_AddGapToEndOfAllRows(pEditInfo, FALSE);
  }

  NumRows = pEditInfo->TotalNumRows;
  /* look through each row */
  for (i=0; i<NumRows; i++) {
    vnp = DDE_GetTxtListPtr(pEditInfo, i);
    /* look through each node */
    while (vnp != NULL) {
      /* if at an aligned to unaligned transition */
      if (DDE_IsEndOfAlignment(vnp)) {
        /* and at the proper aligned block */
        if (BlockIndex == BlockCount) {
          /* make node adjustments to shift boundary right */
          if (DDE_LeftMerge(vnp)) {
            i=i;
          }
          else if (DDE_LeftAddNode(vnp, &dummy)) {
            i=i;
          }
          /* skip to next row */
          RetVal = TRUE;
          break;
        }
        else {
          BlockCount++;
        }
      }
      vnp = vnp->next;
    }
  }
  return(RetVal);
}


NLM_EXTERN ValNodePtr DDE_SplitNode(ValNodePtr vnp, Int4 Offset) {
/*----------------------------------------------------------------------------
*  split vnp into 2 nodes at Offset.
*  return pointer to 2nd part of split.
*---------------------------------------------------------------------------*/
  Int4           Length, old_to;
  MsaTxtDispPtr  msap;
  ValNodePtr     RetVal;

  msap = (MsaTxtDispPtr) vnp->data.ptrvalue;
  Length = (msap->to - msap->from) + 1;
  ASSERT((Offset >= 0) && (Offset <= Length));

  if (Offset == 0) {
    RetVal = vnp;
  }
  else if (Offset == Length) {
    RetVal = vnp->next;
  }
  else {
    old_to = msap->to;
    msap->to = msap->from + (Offset - 1);
    RetVal = DDE_AddMsaTxtNode(vnp, msap->to+1, old_to, msap->IsGap, msap->IsUnAligned, FALSE);
  }
  return(RetVal);
}


NLM_EXTERN void DDE_CreateBlock(DDE_StackPtr pStack, Int4 Left, Int4 Right, Boolean Save) {
/*----------------------------------------------------------------------------
*  create a block from the Left to Right display coordinates.
*  include Left and Right display coordinates in the block.
*  this routine is intended to leave anything that's already aligned
*  as is.  It only adds to it.
*  Save indicates if this edit is stored on the stack.
*---------------------------------------------------------------------------*/
  Int4           i, NumRows, LeftOffset, RightOffset;
  ValNodePtr     vnp, start_vnp, stop_vnp, head;
  MsaTxtDispPtr  msap;
  Boolean*       pNoGaps;
  Boolean        NoGaps;
  
  NumRows = pStack->pOrig->pPopList->nBsp;

  /* add 1 to right boundary so it will be included in the block */
  Right += 1;

  /* for each row */
  for (i=0; i<NumRows; i++) {
    head = DDE_GetTxtListPtr(pStack->pEdit, i);
    start_vnp = DDE_GetMsaTxtNode(head, Left, &LeftOffset);
    start_vnp = DDE_SplitNode(start_vnp, LeftOffset);
    stop_vnp =  DDE_GetMsaTxtNode(head, Right, &RightOffset);
    if (stop_vnp != NULL) {
      stop_vnp =  DDE_SplitNode(stop_vnp, RightOffset);
    }
    /* make intervening nodes aligned */
    vnp = start_vnp;
    while (vnp != stop_vnp) {
      msap = (MsaTxtDispPtr) vnp->data.ptrvalue;
      msap->IsUnAligned = FALSE;
      DDE_SetTextStyle(msap);
      vnp = vnp->next;
    }
  }
  DDE_MergeNodesLists(pStack->pEdit);

  pNoGaps = (Boolean*) GetAppProperty("dde_nogaps");
  if (pNoGaps == NULL) {NoGaps = TRUE;}
  else                 {NoGaps = *pNoGaps;}
  if (NoGaps) {
    DDE_RemoveAlignedGapsFromEnds(pStack->pEdit);
    DDE_MergeNodesLists(pStack->pEdit);
  }

  DDE_ReMakeRuler(pStack->pEdit->pPopList, TRUE, -1);
  DDE_Verify(pStack->pEdit);

  /* add pStack->pEdit to the stack */
  if (Save) {
    DDE_Add(pStack);
  }
  return;
}


NLM_EXTERN Int4 DDE_GetIndexOfMaster(DDE_StackPtr pStack) {
/*----------------------------------------------------------------------------
*  when the multiple sequence alignment data is first loaded,
*  the master is the first (0 index) row.  this function returns
*  the row where that row is now found.
*---------------------------------------------------------------------------*/
  return(DDE_GetDisplayRow(pStack->pEdit, 0));
}


NLM_EXTERN Boolean DDE_IsColValid(MsaParaGPopListPtr pPopList, Int4 Col,
                                  Int4 PNTR pBlockIndex, Boolean PNTR pUnAligned,
                                  Int4 PNTR pOffset, Int4 PNTR pWidth) {
/*----------------------------------------------------------------------------
*  return TRUE if Col is in the legal range.
*  Col is a display coordinate.
*  also return whether the Col is aligned or not.
*  also return the BlockIndex.
*  also return the Offset into the Block.
*  For an aligned column, BlockIndex is the (zero-based) block in which
*  the column resides.
*  For an unaligned column, BlockIndex is the BlockIndex for the preceeding
*  aligned block, plus 1.
*---------------------------------------------------------------------------*/
  ValNodePtr        head, vnp;
  DDVRulerDescrPtr  rdp;

  if (Col < 0) {return(FALSE);}
  
  *pBlockIndex = 0;

  /* look through the nodes of the ruler descriptor */
  vnp = head = pPopList->RulerDescr;
  while (vnp != NULL) {
    rdp = (DDVRulerDescrPtr) vnp->data.ptrvalue;
    *pUnAligned = rdp->bUnAligned;
    if ((Col >= rdp->disp_start) && (Col <= rdp->disp_stop)) {
      /* if col is in range, return TRUE flag, BlockIndex and alignment */
      *pOffset = Col - rdp->disp_start;
      *pWidth = rdp->disp_stop - rdp->disp_start + 1;
      return(TRUE);
    }
    if (!rdp->bUnAligned) {
      /* inc BlockIndex when leaving an aligned region */
      (*pBlockIndex)++;
    }
    vnp = vnp->next;
  }
  return(FALSE);
}


NLM_EXTERN Boolean DDE_GetColStatusForRow(DDE_InfoPtr pEditInfo, Int4 Row, Int4 Col,
                                          Boolean PNTR pUnAligned, Boolean PNTR pGap) {
/*----------------------------------------------------------------------------
*  determine the alignment and gap status of (Row, Col).
*  Col is the display coordinate.
*  return TRUE for successful completion.
*---------------------------------------------------------------------------*/
  Int4           Offset;
  ValNodePtr     vnp, head;
  MsaTxtDispPtr  msap;

  head = DDE_GetTxtListPtr(pEditInfo, Row);
  vnp = DDE_GetMsaTxtNode(head, Col, &Offset);
  if (Offset == -1) {return(FALSE);}
  msap = (MsaTxtDispPtr) vnp->data.ptrvalue;
  *pUnAligned = msap->IsUnAligned;
  *pGap = msap->IsGap;
  return(TRUE);
}


NLM_EXTERN Boolean DDE_GetGapStatusOfRows(DDE_InfoPtr pEditInfo, Boolean PNTR pGapArray, Int4 Col) {
/*----------------------------------------------------------------------------
*  The GapArray is an array of Booleans that indicates the gap status
*  of each row at Col.  Fill the array here.  The array should already
*  have been allocated.  Col is the display coordinate.
*  return TRUE for successful completion.
*---------------------------------------------------------------------------*/
  Int4     i, NumRows;
  Boolean  UnAligned, Gap;

  ASSERT(pGapArray != NULL);
  NumRows = pEditInfo->TotalNumRows;

  for (i=0; i<NumRows; i++) {
    if (DDE_GetColStatusForRow(pEditInfo, i, Col, &UnAligned, &Gap)) {
      pGapArray[i] = Gap;
    }
    else {
      return(FALSE);
    }
  }
  return(TRUE);
}


NLM_EXTERN Boolean DDE_AreArraysSame(Boolean PNTR pArray1, Boolean PNTR pArray2, Int4 Size) {
/*----------------------------------------------------------------------------
*  compare the 2 boolean arrays of size Size.
*  if they match return TRUE.
*---------------------------------------------------------------------------*/
  Int4  i;

  ASSERT(pArray1 != NULL);
  ASSERT(pArray2 != NULL);

  for (i=0; i<Size; i++) {
    if (pArray1[i] != pArray2[i]) {
      return(FALSE);
    }
  }
  return(TRUE);
}


NLM_EXTERN Boolean DDE_GetAlignIndices(DDE_InfoPtr pEditInfo, Int4 PNTR pIndices, Int4 Col) {
/*----------------------------------------------------------------------------
*  put the alignment coordinate for each row at Col in the Indices array.
*  put a -1 in the Indices array for all rows that have gaps at this column.
*  Col is the display coordinate.
*  Indices should already have been allocated.
*  return TRUE for successful completion.
*  return FALSE if this column is unaligned.
*---------------------------------------------------------------------------*/
  Int4           i, NumRows, Offset, Index;
  ValNodePtr     vnp, head;
  MsaTxtDispPtr  msap;

  ASSERT(pIndices != NULL);
  NumRows = pEditInfo->TotalNumRows;
  /* look through each row */
  for (i=0; i<NumRows; i++) {
    Index = DDE_GetDisplayRow(pEditInfo, i);  /* get original order */
    head = DDE_GetTxtListPtr(pEditInfo, Index);
    vnp = DDE_GetMsaTxtNode(head, Col, &Offset);
    if (Offset == -1) {return(FALSE);}
    msap = (MsaTxtDispPtr) vnp->data.ptrvalue;
    if (msap->IsUnAligned) {return(FALSE);}
    if (msap->IsGap) {
      /* if this column is an aligned gap, put -1 in the Starts array */
      pIndices[i] = -1;
    }
    else {
      /* if this column is an aligned non-gap, put alignment coordinate into array */
      pIndices[i] = msap->from + Offset;
    }
  }
  return(TRUE);
}


NLM_EXTERN Boolean DDE_AddIndicesToArray(Int4 PNTR pArray, Int4 PNTR pIndices,
                                         Int4 Size, Int4 ArrayIndex) {
/*----------------------------------------------------------------------------
*  add the contents of the Indices array to Array.
*  Size is the size of the Indices array.
*  ArrayIndex is a count of the number of items already in Array.
*  Sufficient memory should already have been allocated for pArray.
*  return TRUE for successful completion.
*---------------------------------------------------------------------------*/
  if (Size < 0) return(FALSE);
  if (Size == 0) return(TRUE);
  ASSERT(pIndices != NULL);
  ASSERT(pArray != NULL);

  MemCopy(pArray+ArrayIndex, pIndices, Size*sizeof(Int4));
  return(TRUE);
}


NLM_EXTERN Int4 DDE_GetNumSegmentsInBlock(DDE_InfoPtr pEditInfo, Int4 BlockIndex) {
/*----------------------------------------------------------------------------
*  return the number of segments in an aligned Block.
*  return 0 if this Block doesn't exist.
*  A segment is defined by the AlignMgr.  It is the largest division of a
*  block s.t. every piece is either all aligned gaps or aligned sequence.
*---------------------------------------------------------------------------*/
  Int4      i, StartIndex, StopIndex, NumRows, NumSegs=1;
  Boolean*  pArray1;
  Boolean*  pArray2;
  Boolean*  pTemp;

  NumRows = pEditInfo->TotalNumRows;

  StartIndex = DDE_GetAlignStart(pEditInfo, BlockIndex);
  StopIndex = DDE_GetAlignStop(pEditInfo, BlockIndex);
  if ((StartIndex==-1) || (StopIndex==-1)) {return(0);}

  pArray1 = MemNew(NumRows * sizeof(Boolean));
  pArray2 = MemNew(NumRows * sizeof(Boolean));

  DDE_GetGapStatusOfRows(pEditInfo, pArray1, StartIndex);
  /* look through each column of block */
  for (i=StartIndex+1; i<=StopIndex; i++) {
    DDE_GetGapStatusOfRows(pEditInfo, pArray2, i);
    /* if gap status has changed */
    if (!DDE_AreArraysSame(pArray1, pArray2, NumRows)) {
      /* then we're on a new segment */
      NumSegs++;
    }
    /* swap the pointers */
    pTemp =   pArray1;
    pArray1 = pArray2;
    pArray2 = pTemp;
  }

  MemFree(pArray1);
  MemFree(pArray2);

  return(NumSegs);
}


NLM_EXTERN Boolean DDE_CreateArraysForDenseSeg(DDE_InfoPtr pEditInfo, Int4 BlockIndex,
                                               Int4 PNTR pStarts, Int4 PNTR pLens) {
/*----------------------------------------------------------------------------
*  Fill the Starts and Lens array in preparation for creating a DenseSeg.
*  Starts contains all the alignment starts for a block.
*  Lens contains the lengths of each segment.
*  memory for both arrays is already allocated.
*---------------------------------------------------------------------------*/
  Int4      NumRows, NumSegs, i, StartIndex, StopIndex, ArrayIndex=0;
  Int4      SaveCol, SegIndex=0;
  Int4*     pIndices;
  Boolean*  pArray1;
  Boolean*  pArray2;
  Boolean*  pTemp;

  if (pStarts == NULL) return(FALSE);
  if (pLens == NULL) return(FALSE);
  
  NumRows = pEditInfo->TotalNumRows;
  NumSegs = DDE_GetNumSegmentsInBlock(pEditInfo, BlockIndex);

  pIndices = MemNew(NumRows * sizeof(Int4));
  pArray1 =  MemNew(NumRows * sizeof(Boolean));
  pArray2 =  MemNew(NumRows * sizeof(Boolean));

  StartIndex = DDE_GetAlignStart(pEditInfo, BlockIndex);
  StopIndex = DDE_GetAlignStop(pEditInfo, BlockIndex);

  DDE_GetGapStatusOfRows(pEditInfo, pArray1, StartIndex);
  DDE_GetAlignIndices(pEditInfo, pIndices, StartIndex);
  DDE_AddIndicesToArray(pStarts, pIndices, NumRows, ArrayIndex);
  ArrayIndex += NumRows;
  SaveCol = StartIndex;
  /* look through each column of block */
  for (i=StartIndex+1; i<=StopIndex; i++) {
    DDE_GetGapStatusOfRows(pEditInfo, pArray2, i);
    /* if gap status has changed */
    if (!DDE_AreArraysSame(pArray1, pArray2, NumRows)) {
      /* add alignment indices at this column to Starts array */
      DDE_GetAlignIndices(pEditInfo, pIndices, i);
      DDE_AddIndicesToArray(pStarts, pIndices, NumRows, ArrayIndex);
      ArrayIndex += NumRows;
      /* add length of segment to Lens array */
      pLens[SegIndex] = i - SaveCol;
      SaveCol = i;
      SegIndex++;
    }
    /* swap the pointers */
    pTemp =   pArray1;
    pArray1 = pArray2;
    pArray2 = pTemp;
  }
  /* save length of last segment */
  pLens[SegIndex] = (StopIndex - SaveCol) + 1;
  SegIndex++;
  ASSERT(SegIndex == NumSegs);

  MemFree(pIndices);
  MemFree(pArray1);
  MemFree(pArray2);
  return(TRUE);
}


NLM_EXTERN Boolean DDE_IsLeftAlignedGapInRows(DDE_InfoPtr pEditInfo) {
/*----------------------------------------------------------------------------
*  look through each row.  return TRUE if there's a left aligned gap.
*---------------------------------------------------------------------------*/
  Int4        i, NumRows;
  ValNodePtr  vnp;

  NumRows = pEditInfo->TotalNumRows;
  for (i=0; i<NumRows; i++) {
    vnp = DDE_GetTxtListPtr(pEditInfo, i);
    if (DDE_IsLeftAlignedGapList(vnp, FALSE)) {
      return(TRUE);
    }
  }
  return(FALSE);
}


NLM_EXTERN Boolean DDE_IsRightAlignedGapInRows(DDE_InfoPtr pEditInfo) {
/*----------------------------------------------------------------------------
*  look through each row.  return TRUE if there's a right aligned gap.
*---------------------------------------------------------------------------*/
  Int4        i, NumRows;
  ValNodePtr  vnp;

  NumRows = pEditInfo->TotalNumRows;
  for (i=0; i<NumRows; i++) {
    vnp = DDE_GetTxtListPtr(pEditInfo, i);
    if (DDE_IsRightAlignedGapList(&vnp, FALSE)) {
      return(TRUE);
    }
  }
  return(FALSE);
}


NLM_EXTERN void DDE_RemoveAlignedGapsFromEnds(DDE_InfoPtr pEditInfo) {
/*----------------------------------------------------------------------------
*  shift the right and left boundaries closer to the middle, until
*  there are no more aligned gaps on the ends.
*---------------------------------------------------------------------------*/
  Boolean Continue=TRUE;

  /* add gaps to start and end of all rows -- a bit kludgy */
  DDE_AddGapToStartOfAllRows(pEditInfo, FALSE);
  DDE_AddGapToEndOfAllRows(pEditInfo, FALSE);
  
  while (Continue) {
    Continue = FALSE;
    if (DDE_IsLeftAlignedGapInRows(pEditInfo)) {
      DDE_ShiftLeftBoundaryRight1(pEditInfo, 0);
      Continue = TRUE;
    }
    if (DDE_IsRightAlignedGapInRows(pEditInfo)) {
      DDE_ShiftRightBoundaryLeft1(pEditInfo, 0);
      Continue = TRUE;
    }
  }

  /* here's where those gaps are stripped */
  DDE_RemoveGapFromStartOfAllRows(pEditInfo, FALSE);
  DDE_RemoveGapFromEndOfAllRows(pEditInfo, FALSE);
  return;
}


NLM_EXTERN void DDE_RemoveAlignedGapsFromEndOfRow(DDE_StackPtr pStack, Int4 Row, Boolean RightSide) {
/*----------------------------------------------------------------------------
*  shift Row so there are no aligned gaps on the ends of an aligned block
*---------------------------------------------------------------------------*/
  ValNodePtr  vnp;
  Boolean     Continue=TRUE;

  /* add gaps to start and end of all rows -- a bit kludgy */
  DDE_AddGapToStartOfAllRows(pStack->pEdit, FALSE);
  DDE_AddGapToEndOfAllRows(pStack->pEdit, FALSE);
  
  while (Continue) {
    vnp = DDE_GetTxtListPtr(pStack->pEdit, Row);
    Continue = FALSE;
    if (RightSide) {
      if (DDE_IsRightAlignedGapList(&vnp, FALSE)) {
        DDE_ShiftRow(pStack, Row, 1, FALSE, FALSE);
        Continue = TRUE;
      }
    }
    else {
      if (DDE_IsLeftAlignedGapList(vnp, FALSE)) {
        DDE_ShiftRow(pStack, Row, -1, FALSE, FALSE);
        Continue = TRUE;
      }
    }
  }

  /* here's where those gaps are stripped */
  DDE_RemoveGapFromStartOfAllRows(pStack->pEdit, FALSE);
  DDE_RemoveGapFromEndOfAllRows(pStack->pEdit, FALSE);
  return;
}


NLM_EXTERN Boolean DDE_LeftJustify(DDE_StackPtr pStack, Boolean Save) {
/*----------------------------------------------------------------------------
*  Left align the rows.  This only works if there's no aligned block.
*  Save indicates if this edit is stored on the stack.
*---------------------------------------------------------------------------*/
  Int4  i, NumRows, NumBlocks, NumGaps;

  NumBlocks = DDE_GetNumBlocks(pStack->pEdit);
  if (NumBlocks != 0) return(FALSE);
  
  /* shift each row left by the number of leading unaligned gaps */
  NumRows = pStack->pEdit->TotalNumRows;
  for (i=0; i<NumRows; i++) {
    NumGaps = DDE_GetNumLeadingUnAlignedGaps(pStack->pEdit, i);
    DDE_ShiftRow(pStack, i, -NumGaps, FALSE, TRUE);
  }

  /* add pStack->pEdit to the stack */
  if (Save) {
    DDE_Add(pStack);
  }

  return(TRUE);
}


NLM_EXTERN Boolean DDE_RightJustify(DDE_StackPtr pStack, Boolean Save) {
/*----------------------------------------------------------------------------
*  Right align the rows.  This only works if there's no aligned block.
*  Save indicates if this edit is stored on the stack.
*---------------------------------------------------------------------------*/
  Int4  i, NumRows, NumBlocks, NumGaps;

  NumBlocks = DDE_GetNumBlocks(pStack->pEdit);
  if (NumBlocks != 0) return(FALSE);
  
  /* shift each row right by the number of trailing unaligned gaps */
  NumRows = pStack->pEdit->TotalNumRows;
  for (i=0; i<NumRows; i++) {
    NumGaps = DDE_GetNumTrailingUnAlignedGaps(pStack->pEdit, i);
    DDE_ShiftRow(pStack, i, NumGaps, FALSE, TRUE);
  }

  /* add pStack->pEdit to the stack */
  if (Save) {
    DDE_Add(pStack);
  }

  return(TRUE);
}


NLM_EXTERN Boolean DDE_CenterJustify(DDE_StackPtr pStack, Boolean Save) {
/*----------------------------------------------------------------------------
*  Center the rows.  This only works if there's no aligned block.
*  Save indicates if this edit is stored on the stack.
*---------------------------------------------------------------------------*/
  Int4  i, NumRows, NumBlocks, NumLeadingGaps, NumTrailingGaps, Shift;

  NumBlocks = DDE_GetNumBlocks(pStack->pEdit);
  if (NumBlocks != 0) return(FALSE);
  
  NumRows = pStack->pEdit->TotalNumRows;
  for (i=0; i<NumRows; i++) {
    NumLeadingGaps = DDE_GetNumLeadingUnAlignedGaps(pStack->pEdit, i);
    NumTrailingGaps = DDE_GetNumTrailingUnAlignedGaps(pStack->pEdit, i);
    Shift = (NumTrailingGaps - NumLeadingGaps) / 2;
    DDE_ShiftRow(pStack, i, Shift, FALSE, TRUE);
  }

  /* add pStack->pEdit to the stack */
  if (Save) {
    DDE_Add(pStack);
  }

  return(TRUE);
}


NLM_EXTERN Int4 DDE_GetNumLeadingUnAlignedGaps(DDE_InfoPtr pEditInfo, Int4 Row) {
/*----------------------------------------------------------------------------
*  Count the number of unaligned gaps at the start of the row.
*---------------------------------------------------------------------------*/
  Int4     Col=0;
  Boolean  UnAligned, Gap;

  while(DDE_GetColStatusForRow(pEditInfo, Row, Col, &UnAligned, &Gap)) {
    if (UnAligned && Gap) {
      Col++;
    }
    else {
      return(Col);
    }
  }
  return(Col);
}


NLM_EXTERN Int4 DDE_GetNumTrailingUnAlignedGaps(DDE_InfoPtr pEditInfo, Int4 Row) {
/*----------------------------------------------------------------------------
*  Count the number of unaligned gaps at the end of the row.
*---------------------------------------------------------------------------*/
  Int4     Col, NumCols, NumGaps=0;
  Boolean  UnAligned, Gap;

  NumCols = pEditInfo->pPopList->LengthAli;
  Col = NumCols-1;
  
  while(DDE_GetColStatusForRow(pEditInfo, Row, Col, &UnAligned, &Gap)) {
    if (UnAligned && Gap) {
      Col--;
      NumGaps++;
    }
    else {
      return(NumGaps);
    }
  }
  return(NumGaps);
}


NLM_EXTERN Int4 DDE_GetStart(MsaParaGPopListPtr pPopList, Int4 BlockIndex, Int4 Row) {
/*----------------------------------------------------------------------------
*  get the bioseq coordinate for the start of the BlockIndex block of Row.
*---------------------------------------------------------------------------*/
  MsaTxtDispPtr  msap;
  ValNodePtr     head, vnp;
  Int4           Count=0;

  head = vnp = DDE_GetTxtListPtr2(pPopList, Row);
  if (head == NULL) return(-1);

  msap = (MsaTxtDispPtr) vnp->data.ptrvalue;
  if (msap == NULL) return(-1);

  /* for start of 1st block which starts with an aligned region */
  if ((BlockIndex == 0) && (!msap->IsUnAligned)) {
    return(msap->from);
  }

  /* look through the linked list of MsaTxtDisp's */
  while (vnp!= NULL) {
    /* if we're at the correct block */
    if (Count == BlockIndex) {
      /* if vnp is unaligned and vnp->next is aligned */
      if (DDE_IsStartOfAlignment(vnp)) {
        /* return bioseq coord of start of next block */
        msap = (MsaTxtDispPtr) (vnp->next)->data.ptrvalue;
        return(msap->from);
      }
    }
    if (DDE_IsEndOfAlignment(vnp)) {
      /* count unaligned to aligned transitions */
      Count++;
    }
    vnp = vnp->next;
  }
  return(-1);
}


NLM_EXTERN Int4 DDE_GetLen(MsaParaGPopListPtr pPopList, Int4 BlockIndex) {
/*----------------------------------------------------------------------------
*  get the length of the BlockIndex block.
*---------------------------------------------------------------------------*/
  ValNodePtr        vnp;
  DDVRulerDescrPtr  drdp;
  Int4              BlockCount=0;

  vnp = pPopList->RulerDescr;
  while (vnp != NULL) {
    drdp = (DDVRulerDescrPtr) vnp->data.ptrvalue;
    if (!drdp->bUnAligned) {
      if (BlockCount == BlockIndex) {
        return(drdp->disp_stop - drdp->disp_start + 1);
      }
      BlockCount++;
    }
    vnp = vnp->next;
  }
  return(-1);
}


