/*   suggslp.h
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
* File Name:  suggslp.h
*
* Author:  Webb Miller, Karl Sirotkin, Warren Gish, Jonathan Kans, Yuri Sadykov
*
* Version Creation Date:   11/22/95
*
* $Revision: 6.1 $
*
* File Description: 
*	Implementation of standalone Suggest prediction function.
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* ==========================================================================
*/

#ifndef _SUGGSLP_
#define _SUGGSLP_

#include <ncbi.h>
#include <objseq.h>
#include <suggest.h>

extern MessageHook    fOldMsgHook;
extern IntPtr         intlist;
extern SuggestError   suggestError;
extern SuggestOutput  suggestOut;
extern SuggestRec     suggestRec;

extern void SuggestClientService PROTO((SuggestIntervalsPtr pSIntervals));
extern SeqAnnotPtr SuggestCodingRegion PROTO((BioseqPtr nuc, BioseqPtr prot, Int2 genCode));

extern void SetBatchSuggestNucleotide PROTO((BioseqPtr nuc, Int2 genCode));
extern void ClearBatchSuggestNucleotide PROTO((void));

#endif
