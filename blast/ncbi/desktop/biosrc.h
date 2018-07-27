/*   biosrc.h
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
* File Name:  biosrc.h
*
* Author:  Jonathan Kans
*
* Version Creation Date:   1/22/95
*
* $Revision: 6.5 $
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

#ifndef _BIOSRC_
#define _BIOSRC_

#include <dlogutil.h>


#ifdef __cplusplus
extern "C" {
#endif

#define NUM_GENETIC_CODES 40

extern Int2     numGeneticCodes;
extern Int2     gcIdToIndex [NUM_GENETIC_CODES];
extern Int2     gcIndexToId [NUM_GENETIC_CODES];
extern CharPtr  gcNames [NUM_GENETIC_CODES];

extern Boolean LoadOrganismTable (void);
extern void FreeOrganismTable (void);

extern void SetupGeneticCodes (void);
extern void FreeGeneticCodes (void);
extern ValNodePtr GetGeneticCodeValNodeList (void);

/*
 *  The BioSourceEditProcsPtr may be registered with a call to SetAppProperty
 *  e.g., SetAppProperty ("BioSourcEditForm", &biosrcprocs), where biosrcprocs
 *  is a persistent structure filled with parameters and callback function
 *  pointers specific for a given application.
 */

typedef Boolean (*LookupTaxonomyProc) PROTO((Uint2 entityID));

typedef struct biosrceditprocs {
  LookupTaxonomyProc   lookupTaxonomy;
} BioSourceEditProcs, PNTR BioSourceEditProcsPtr;

#define REGISTER_BIOSOURCE_DESC_EDIT ObjMgrProcLoad(OMPROC_EDIT,"Edit BioSource Desc","Biological Source",OBJ_SEQDESC,Seq_descr_source,OBJ_SEQDESC,Seq_descr_source,NULL,BioSourceGenFunc,PROC_PRIORITY_DEFAULT)
#define REGISTER_BIOSOURCE_FEAT_EDIT ObjMgrProcLoad(OMPROC_EDIT,"Edit BioSource Feat","Biological Source",OBJ_SEQFEAT,FEATDEF_BIOSRC,OBJ_SEQFEAT,FEATDEF_BIOSRC,NULL,BioSourceGenFunc,PROC_PRIORITY_DEFAULT)

extern DialoG CreateSimpleBioSourceDialog (GrouP h, CharPtr title);
extern ForM CreateBioSourceDescForm (Int2 left, Int2 top, Int2 width,
                                     Int2 height, CharPtr title, ValNodePtr sdp,
                                     SeqEntryPtr sep, FormActnFunc actproc,
                                     BioSourceEditProcsPtr bepp);
extern ForM CreateBioSourceFeatForm (Int2 left, Int2 top, Int2 width,
                                     Int2 height, CharPtr title, SeqFeatPtr sfp,
                                     SeqEntryPtr sep, FormActnFunc actproc,
                                     BioSourceEditProcsPtr bepp);

extern Int2 LIBCALLBACK BioSourceGenFunc (Pointer data);
extern Boolean BioSourceDialogToGenBankDivision (DialoG d, CharPtr div, size_t maxsize);

extern Boolean SetBioSourceDialogTaxName (DialoG d, CharPtr taxname);

/* This structure should be passed in as userdata for
 * RemoveTextFromTextFreeSubSourceModifiers, with the
 * members all initialized to FALSE.
 */
typedef struct modtextfix 
{
  Boolean remove_this;
  Boolean move_this;
  Boolean remove_all_germline;
  Boolean remove_all_transgenic;
  Boolean remove_all_environmental;
  Boolean remove_all_rearranged;
  Boolean move_all_germline;
  Boolean move_all_transgenic;
  Boolean move_all_environmental;
  Boolean move_all_rearranged;
} ModTextFixData, PNTR ModTextFixPtr;
extern ModTextFixPtr ModTextFixNew (void);
extern void RemoveTextFromTextFreeSubSourceModifiers (BioSourcePtr biop, Pointer userdata);

#ifdef __cplusplus
}
#endif

#endif /* ndef _BIOSRC_ */

