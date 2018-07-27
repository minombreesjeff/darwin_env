/* ===========================================================================
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
* ===========================================================================*/
/*****************************************************************************

File name: blastconcat.h

Author: Karolina Maciag, Aleksandr Morgulis

Contents: type declarations for query multiplexing code.

******************************************************************************/
/* $Revision: 1.2 $ 
*  $Log: blastconcat.h,v $
*  Revision 1.2  2004/04/20 14:55:47  morgulis
*  1. Fixed query offsets in results when -B option is used.
*  2. Fixes for lower case masking handling with -B option.
*
*  Revision 1.1  2003/03/24 20:47:28  madden
*  Utilities for concatenation of blastn/tblastn queries
*
* */


#ifndef _BLASTCONCAT_
#define _BLASTCONCAT_

#include <ncbi.h>

#define LAST_N				15
#define TWO_N				(LAST_N*16 + LAST_N)
#define SPACER_POS		-1
#define DROPOFF_NUMBER_OF_BITS 10.0

typedef SeqAlignPtr *SeqAlignPtrArray;
typedef SeqAlignPtrArray *SeqAlignPtrArrayPtr;
typedef SeqAnnotPtr *SeqAnnotPtrArray;

/* --KM types necessary when the -B option is used for query concatenation */

/* array to store the fake_bsps corresponding to indiv. queries */
typedef BioseqPtr PNTR BspArray; 
/* integer arrays for indiv. query bounds */
typedef Int4 PNTR IntArray;

/* AM: float and Int8 arrays for effective search spaces and effective
       database lengths. */
typedef Int8 PNTR Int8Array;
typedef Uint1 PNTR Uint1Array;
typedef Nlm_FloatHi PNTR FloatArray;

struct queries;	/* Forward declaration of Queries type */

#endif 	/* _BLASTCONCAT_ */

