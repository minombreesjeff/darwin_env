/*   pubdesc.h
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
* File Name:  pubdesc.h
*
* Author:  John Kuzio, Jonathan Kans
*
* Version Creation Date:   7/28/95
*
* $Revision: 6.1 $
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

#ifndef _PUBDESC_
#define _PUBDESC_

#include <dlogutil.h>


#ifdef __cplusplus
extern "C" {
#endif

/*
 *  The PubdescEditProcsPtr may be registered with a call to SetAppProperty
 *  e.g., SetAppProperty ("PubdescEditForm", &pubdescprocs), where pubdescprocs
 *  is a persistent structure filled with parameters and callback function
 *  pointers specific for a given application.
 */

typedef ValNodePtr (*PubEquivLookupProc) PROTO((ValNodePtr pep));
typedef Boolean (*LookupJournalProc) PROTO((CharPtr title, size_t maxsize, Int1Ptr jtaType));

typedef struct pubdesceditprocs {
  PubEquivLookupProc  lookupArticle;
  LookupJournalProc   lookupJournal;
  Boolean             replaceThis;
} PubdescEditProcs, PNTR PubdescEditProcsPtr;

#define REGISTER_PUBDESC_DESC_EDIT ObjMgrProcLoad(OMPROC_EDIT,"Edit Pubdesc Desc","Publication",OBJ_SEQDESC,Seq_descr_pub,OBJ_SEQDESC,Seq_descr_pub,NULL,PubdescGenFunc,PROC_PRIORITY_DEFAULT)
#define REGISTER_PUBDESC_FEAT_EDIT ObjMgrProcLoad(OMPROC_EDIT,"Edit Pubdesc Feat","Publication",OBJ_SEQFEAT,FEATDEF_PUB,OBJ_SEQFEAT,FEATDEF_PUB,NULL,PubdescGenFunc,PROC_PRIORITY_DEFAULT)

extern ForM CreatePubdescInitForm (Int2 left, Int2 top, CharPtr title,
                                   ValNodePtr sdp, SeqFeatPtr sfp,
                                   SeqEntryPtr sep, Uint2 itemtype,
                                   FormActnFunc actproc,
                                   PubdescEditProcsPtr pepp);
extern ForM CreatePubdescDescForm (Int2 left, Int2 top, CharPtr title,
                                   Uint1 reftype,
                                   Uint1 pub_status,
                                   Int2 pub_choice,
                                   Boolean flagPubDelta,
                                   Boolean flagSerial,
                                   ValNodePtr sdp, SeqEntryPtr sep,
                                   FormActnFunc actproc,
                                   PubdescEditProcsPtr pepp);
extern ForM CreatePubdescFeatForm (Int2 left, Int2 top, CharPtr title,
                                   Uint1 reftype,
                                   Uint1 pub_status,
                                   Int2 pub_choice,
                                   Boolean flagPubDelta,
                                   Boolean flagSerial,
                                   SeqFeatPtr sfp, SeqEntryPtr sep,
                                   FormActnFunc actproc,
                                   PubdescEditProcsPtr pepp);

extern Int2 LIBCALLBACK PubdescGenFunc (Pointer data);


#ifdef __cplusplus
}
#endif

#endif /* ndef _PUBDESC_ */
