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
* $Revision: 6.63 $
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

/* choices of format, mode, and style */

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
  SEGMENT_STYLE,
  MASTER_STYLE,
  CONTIG_STYLE
} StlType;

/* bit flags for modifying behavior */

typedef unsigned long FlgType;

#define CREATE_HTML_FLATFILE       1
#define CREATE_XML_GBSEQ_FILE      2
#define CREATE_ASN_GBSEQ_FILE      3

#define SHOW_CONTIG_FEATURES       4
#define SHOW_CONTIG_SOURCES        8

#define SHOW_FAR_TRANSLATION      16
#define TRANSLATE_IF_NO_PRODUCT   32
#define ALWAYS_TRANSLATE_CDS      48

#define SHOW_FAR_TRANSCRIPTION    16
#define TRANSCRIBE_IF_NO_PRODUCT  32
#define ALWAYS_TRANSCRIBE_MRNA    48

#define REFSEQ_CONVENTIONS        64

#define ONLY_NEAR_FEATURES       128
#define FAR_FEATURES_SUPPRESS    256
#define NEAR_FEATURES_SUPPRESS   384

#define COPY_GPS_CDS_UP          512
#define COPY_GPS_GENE_DOWN      1024

#define SHOW_CONTIG_AND_SEQ     2048

#define SHOW_TRANCRIPTION       4096
#define SHOW_PEPTIDE            8192

#define PRODUCE_OLD_GBSEQ      16384

#define DDBJ_VARIANT_FORMAT    32768

/* locking behavior for system performance */

typedef unsigned long LckType;

#define LOCK_FAR_COMPONENTS        2
#define LOCK_FAR_LOCATIONS         4
#define LOCK_FAR_PRODUCTS          8
#define LOOKUP_FAR_COMPONENTS     16
#define LOOKUP_FAR_LOCATIONS      32
#define LOOKUP_FAR_PRODUCTS       64
#define LOOKUP_FAR_HISTORY       128
#define LOOKUP_FAR_OTHERS        256

/* bit flags for unusual customized reports */

typedef unsigned long CstType;

#define HIDE_IMP_FEATS             1
#define HIDE_REM_IMP_FEATS         2

#define HIDE_SNP_FEATS             4
#define HIDE_EXON_FEATS            8
#define HIDE_INTRON_FEATS         16
#define HIDE_MISC_FEATS           32

#define HIDE_CDD_FEATS            64
#define HIDE_CDS_PROD_FEATS      128

#define HIDE_GENE_RIFS           256
#define ONLY_GENE_RIFS           512
#define ONLY_REVIEW_PUBS         768
#define NEWEST_PUBS             1024
#define OLDEST_PUBS             1280

#define SHOW_PROT_FTABLE        2048
#define SHOW_FTABLE_REFS        4096

#define HIDE_SOURCE_FEATS       8192
#define HIDE_GAP_FEATS         16384
#define HIDE_SEQUENCE          32768

/* opaque pointer for special extensions */

struct XtraData;
typedef struct XtraData* XtraPtr;

/* flatfile generation functions */

NLM_EXTERN Boolean SeqEntryToGnbk (
  SeqEntryPtr sep,
  SeqLocPtr slp,
  FmtType format,
  ModType mode,
  StlType style,
  FlgType flags,
  LckType locks,
  CstType custom,
  XtraPtr extra,
  FILE *fp
);

NLM_EXTERN Boolean BioseqToGnbk (
  BioseqPtr bsp,
  SeqLocPtr slp,
  FmtType format,
  ModType mode,
  StlType style,
  FlgType flags,
  LckType locks,
  CstType custom,
  XtraPtr extra,
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

