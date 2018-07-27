/* $Id: cddposutil.h,v 1.7 2001/01/18 23:33:20 kans Exp $
*===========================================================================
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
* File Name:  cddposutil.h
*
* Author:  Aron Marchler-Bauer
*
* Initial Version Creation Date: 12/21/1999
*
* $Revision: 1.7 $
*
* File Description: Header file for cdd position-specific matrix utility
*                   functions 
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: cddposutil.h,v $
* Revision 1.7  2001/01/18 23:33:20  kans
* added prototypes for CddposFreeMemory2 and CddposProcessAlignment for Mac compiler complaint
*
* Revision 1.6  2000/12/29 00:52:21  hurwitz
* cleaning memory leaks
*
* Revision 1.5  2000/09/08 21:43:51  hurwitz
* adding PSSM calculation to DDE
*
* Revision 1.4  2000/08/10 22:31:20  bauer
* CddposTakeCheckpoint now declared to return Boolean
*
* Revision 1.3  2000/08/09 21:29:08  hurwitz
* adding cddutil.c to VC++ build
*
* Revision 1.2  2000/07/19 20:04:14  bauer
* added modification logging
*
*
* ==========================================================================
*/

#if !defined(CDDPOSUTIL_H)
#define CDDPOSUTIL_H

#ifdef __cplusplus
extern "C" {
#endif

void LIBCALL CddposAllocateMemory(posSearchItems * posSearch, Int4 alphabetSize,
                                  Int4 querySize, Int4 numSequences);

void LIBCALL CddposFreeMemory(posSearchItems * posSearch);

void LIBCALL CddposFreeMemory2(compactSearchItems * compactSearch);

void LIBCALL CddposProcessAlignment(posSearchItems *posSearch, 
                                    compactSearchItems *compactSearch,
                                    SeqAlignPtr salp, Int4 numSeqs,
                                    Int4 alignLength, BioseqPtr bsp);

void LIBCALL CddcopySearchItems(compactSearchItems *compactSearch,
                                BlastSearchBlkPtr   search);

void LIBCALL CddfindThreshSequences(posSearchItems *posSearch,
                                    SeqAlignPtr listOfSeqAligns, Int4 numalign,
                                    Int4 numseq);

void LIBCALL CddposInitializeInformation(posSearchItems *posSearch,
                                         BLAST_ScoreBlkPtr search,
                                         compactSearchItems *compactSearch);

void LIBCALL CddposDemographics(posSearchItems *posSearch,
                                compactSearchItems *compactSearch,
                                SeqAlignPtr listOfSeqAligns);

BlastSearchBlkPtr LIBCALL CddSetUpSearchWithReadDb(SeqLocPtr query_slp,
                                                   BioseqPtr query_bsp,
                                                   CharPtr prog_name,
                                                   Int4 qlen,
                                                   CharPtr dbname,
                                                   BLAST_OptionsBlkPtr options,
                                                   int (LIBCALLBACK *callback)PROTO((Int4 done, Int4 positives)),
                                                   SeqIdPtr seqid_list,
                                                   BlastDoubleInt4Ptr gi_list,
                                                   Int4 gi_list_total,
                                                   ReadDBFILEPtr rdfp);

void LIBCALL CddfindDenseDiagThreshSequences(posSearchItems *posSearch,
                                             SeqAlignPtr listOfSeqAligns,
                                             Int4 numalign, Int4 numseq);

void LIBCALL CddposDenseDiagDemographics(posSearchItems     *posSearch,
                                         compactSearchItems *compactSearch,
                                         SeqAlignPtr        listOfSeqAligns);

void LIBCALL CddposPurgeMatches(posSearchItems *posSearch,
                                compactSearchItems * compactSearch);

void LIBCALL CddposComputeExtents(posSearchItems *posSearch,
                                  compactSearchItems * compactSearch);

void LIBCALL CddposComputeSequenceWeights(posSearchItems *posSearch,
                                          compactSearchItems * compactSearch);

void LIBCALL CddposCheckWeights(posSearchItems *posSearch,
                                compactSearchItems * compactSearch);

Nlm_FloatHi ** LIBCALL CddposComputePseudoFreqs(posSearchItems *posSearch,
                                                compactSearchItems * compactSearch,
                                                Boolean Cpos);

void LIBCALL CddposFreqsToMatrix(posSearchItems *posSearch,
                                 compactSearchItems * compactSearch);

void LIBCALL CddposScaling(posSearchItems *posSearch,
                           compactSearchItems * compactSearch);

Boolean LIBCALL CddposTakeCheckpoint(posSearchItems * posSearch,
                             compactSearchItems * compactSearch,
                             CharPtr fileName, ValNodePtr *error_return);

#ifdef __cplusplus
}
#endif

#endif /* CDDPOSUTIL_H */
