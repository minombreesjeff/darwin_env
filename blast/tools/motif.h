/*   motif.h
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
* File Name:  motif.h
*
* Author:  Sarah Wheelan
*
* Version Creation Date:   8/8/01
*
* $Revision: 6.1 $
*
* File Description:  sequence motif search functions
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* ==========================================================================
*/

#include <ncbi.h>
#include <sequtil.h>
#include <seqport.h>
#include <actutils.h>
#include <blast.h>

#define MOT_WINSIZE 15000
#define MOT_KOZTHRESH  0.05
#define MOT_KOZLEN  9
#define MOT_SIGTHRESH  0.1
#define MOT_SIGLEN 15
#define MOT_MINPOLYASIZE  5 /* minimum #A's to call a poly(A)+ tail */
#define MOT_MAXPOLYASIZE  200 /* maximum number of nucleotides to bother */
                              /* scanning for a tail                     */
#define MOT_LINKERSIZE  8 /* maximum number of non-A's to allow on end of tail */

typedef struct motif_info {
   Int4     start;
   Int4     stop;
   FloatHi  score;
   Uint1    strand;
   struct motif_info PNTR next;
} MotifInfo, PNTR MotifInfoPtr;

NLM_EXTERN MotifInfoPtr MOT_MotifSearch(CharPtr motif, BioseqPtr bsp, Boolean is_prot);
NLM_EXTERN MotifInfoPtr MOT_FindRepeats(BioseqPtr bsp);
NLM_EXTERN MotifInfoPtr MOT_FindKozak(BioseqPtr bsp);
NLM_EXTERN MotifInfoPtr MOT_FindSignalPeptide(BioseqPtr bsp);
NLM_EXTERN MotifInfoPtr MOT_FindPolyA(BioseqPtr bsp);
