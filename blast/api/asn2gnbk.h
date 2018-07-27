/*   asn2gnbk.h
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
* File Name:  asn2gnbk.h
*
* Author:  Karl Sirotkin, Tom Madden, Tatiana Tatusov, Jonathan Kans
*
* Version Creation Date:   10/21/98
*
* $Revision: 6.30 $
*
* File Description:  New GenBank flatfile generator
*
* Modifications:  
* --------------------------------------------------------------------------
* ==========================================================================
*/

#ifndef _ASN2NGNBK_
#define _ASN2NGNBK_

#include <ncbi.h>
#include <objall.h>

#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* choices of format, mode, style, and bit flags */

typedef enum {
  GENBANK_FMT = 1,
  EMBL_FMT,
  GENPEPT_FMT,
  EMBLPEPT_FMT,
  FTABLE_FMT
} FmtType;

typedef enum {
  RELEASE_MODE = 1,
  ENTREZ_MODE,
  SEQUIN_MODE,
  DUMP_MODE
} ModType;

typedef enum {
  NORMAL_STYLE = 1,
  SEGMENTED_STYLE,
  MASTER_STYLE,
  CONTIG_STYLE
} StlType;

#define CREATE_HTML_FLATFILE    1
#define SHOW_CONTIG_FEATURES    2
#define SHOW_CONTIG_SOURCES     4
#define SHOW_FAR_TRANSLATION    8
#define LOCK_FAR_COMPONENTS    16
#define LOCK_FAR_LOCATIONS     32
#define LOCK_FAR_PRODUCTS      64

typedef unsigned long FlgType;

NLM_EXTERN Boolean SeqEntryToGnbk (
  SeqEntryPtr sep,
  SeqLocPtr slp,
  FmtType format,
  ModType mode,
  StlType style,
  FlgType flags,
  FILE *fp
);

NLM_EXTERN Boolean BioseqToGnbk (
  BioseqPtr bsp,
  SeqLocPtr slp,
  FmtType format,
  ModType mode,
  StlType style,
  FlgType flags,
  FILE *fp
);


#ifdef __cplusplus
}
#endif

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

#endif /* ndef _ASN2NGNBK_ */

