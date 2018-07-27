/*   e2docsum.c
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
* File Name:  e2docsum.c
*
* Author:  Jonathan Kans, Greg Schuler, Jonathan Epstein, Tim Ford
*
* Version Creation Date:   10/30/01
*
* $Revision: 6.48 $
*
* File Description: 
*
* Modifications:  
* --------------------------------------------------------------------------
*
* ==========================================================================
*/

#include <objmgr.h>
#include <asn2gnbk.h>
#include <explore.h>
#include <tomedlin.h>
#include <tofasta.h>
#include <simple.h>
#include <objproj.h>
#include <mmdbapi.h>
#include <blast.h>
#include <blastpri.h>
#include <simutil.h>
#include <bspview.h>
#include <medview.h>
#include <vibrant.h>
#include <document.h>
#include <asn.h>
#include <ent2api.h>
#include <urlquery.h>
#include <dlogutil.h>
#include <pmfapi.h>
#include <strucapi.h>
#include <mimapi.h>
#include <objmime.h>
#include <cn3dopen.h>

#include <entrez2.h>

#define MAX_DBS            64
#define MAX_UIDS           30000
#define DocUid             Int4
#define WORST_ID_BUFF_SIZE 256

#define FETCH_MODE         1
#define EVAL_MODE          2

#define NO_AUTHOR_STR      "[No Author Available]"
#define NO_TITLE_STR       "[No Title Available]"
#define NO_SOURCE_STR      "[No Source Available]"
#define NO_DATE_STR        "[No Date Available]"
#define NO_VOLUME_STR      "[No Volume Available]"
#define NO_PAGE_STR        "[No Page Available]"
#define NO_CAPTION_STR     "[No Caption Available]"
#define NO_UID_STR         "[No UID Available]"

/*----------------------*/
/* Set up static arrays */
/*----------------------*/

static ParData  docsumParFmt = { TRUE, FALSE, FALSE, FALSE, FALSE, 0, 0 };
static ColData  docsumColFmt [] = {
  {0, 0, 15, 0, NULL, 'l', TRUE, FALSE, FALSE, FALSE, FALSE},   /* caption  */
  {0, 5, 65, 0, NULL, 'l', TRUE, FALSE, FALSE, FALSE, TRUE}     /* term     */
};
static ColData  textColFmt [] = {
  {0, 0, 80, 0, NULL, 'l', FALSE, FALSE, FALSE, FALSE, TRUE}    /* text     */
};

/*--------------*/
/* Define icons */
/*--------------*/

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

static Uint1  popseticon [] = {
  0x7F, 0xFF, 0xFF, 0xFE, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x7F, 0xFF, 0xFF, 0xFE
};

static Uint1  omimicon [] = {
  0x7F, 0xFF, 0xFF, 0xFE, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x7F, 0xFF, 0xFF, 0xFE
};

static Uint1  taxonomyicon [] = {
  0x7F, 0xFF, 0xFF, 0xFE, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x7F, 0xFF, 0xFF, 0xFE
};

static Uint1  booksicon [] = {
  0x7F, 0xFF, 0xFF, 0xFE, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x7F, 0xFF, 0xFF, 0xFE
};

static Uint1  probeicon [] = {
  0x7F, 0xFF, 0xFF, 0xFE, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x7F, 0xFF, 0xFF, 0xFE
};

static Uint1  domainicon [] = {
  0x7F, 0xFF, 0xFF, 0xFE, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x7F, 0xFF, 0xFF, 0xFE
};

static Uint1  stsicon [] = {
  0x7F, 0xFF, 0xFF, 0xFE, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x7F, 0xFF, 0xFF, 0xFE
};

static Uint1  cddicon [] = {
  0x7F, 0xFF, 0xFF, 0xFE, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x7F, 0xFF, 0xFF, 0xFE
};

static Uint1  snpicon [] = {
  0x7F, 0xFF, 0xFF, 0xFE, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x7F, 0xFF, 0xFF, 0xFE
};

static Uint1  journalsicon [] = {
  0x7F, 0xFF, 0xFF, 0xFE, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x7F, 0xFF, 0xFF, 0xFE
};

static Uint1  unigeneicon [] = {
  0x7F, 0xFF, 0xFF, 0xFE, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x7F, 0xFF, 0xFF, 0xFE
};

static Uint1  pmcicon [] = {
  0x7F, 0xFF, 0xFF, 0xFE, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x7F, 0xFF, 0xFF, 0xFE
};

static Uint1  ncbisearchicon [] = {
  0x7F, 0xFF, 0xFF, 0xFE, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x7F, 0xFF, 0xFF, 0xFE
};

static Uint1  meshicon [] = {
  0x7F, 0xFF, 0xFF, 0xFE, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x7F, 0xFF, 0xFF, 0xFE
};

static Uint1  geneicon [] = {
  0x7F, 0xFF, 0xFF, 0xFE, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x7F, 0xFF, 0xFF, 0xFE
};

static Uint1  genericon [] = {
  0x7F, 0xFF, 0xFF, 0xFE, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x7F, 0xFF, 0xFF, 0xFE
};

/*-----------------------------------------*/
/* Define the possible documents types for */
/* each database and the functions used to */
/* retrieve these documents.               */
/*-----------------------------------------*/

static CharPtr  defaultRadios [] = { "Summary", "Fields", "Unique ID", NULL };
static CharPtr  medRadios [] = { "Summary", "Fields", "Abstract", "Citation", "MEDLINE", "PubMed ID", NULL };
static CharPtr  prtRadios [] = { "Summary", "Fields", "GenPept", "FASTA", "Protein ID", NULL };
static CharPtr  nucRadios [] = { "Summary", "Fields", "GenBank", "EMBL", "FASTA", "CDS FASTA", "Nucleotide ID", NULL };
static CharPtr  strucRadios [] = { "Summary", "Fields", "Report", "Structure ID", NULL };
static CharPtr  genRadios [] = { "Summary", "Fields", "Genome ID", NULL };
static CharPtr  popsetRadios [] = { "Summary", "Fields", "Sequence ID", NULL };
static CharPtr  omimRadios [] = { "Summary", "Fields", "MIM ID", NULL };
static CharPtr  taxonomyRadios [] = { "Summary", "Fields", "Taxon ID", NULL };
static CharPtr  bookRadios [] = { "Summary", "Fields", "Book ID", NULL };
static CharPtr  probeRadios [] = { "Summary", "Fields", "ProbeSet ID", NULL };
static CharPtr  domainRadios [] = { "Summary", "Fields", "3D Domain ID", NULL };
static CharPtr  stsRadios [] = { "Summary", "Fields", "UniSTS ID", NULL };
static CharPtr  cddRadios [] = { "Summary", "Fields", "Conserved Domain ID", NULL };
static CharPtr  snpRadios [] = { "Summary", "Fields", "SNP ID", NULL };
static CharPtr  journalsRadios [] = { "Summary", "Fields", "Journals ID", NULL };
static CharPtr  unigeneRadios [] = { "Summary", "Fields", "UniGene ID", NULL };
static CharPtr  pmcRadios [] = { "Summary", "Fields", "PMC ID", NULL };
static CharPtr  ncbisearchRadios [] = { "Summary", "Fields", "Unique ID", NULL };
static CharPtr  meshRadios [] = { "Summary", "Fields", "MeSH ID", NULL };
static CharPtr  geneRadios [] = { "Summary", "Fields", "Gene ID", NULL };
static CharPtr  localBioseqRadios [] = { "FASTA", NULL };

static CharPtr  defaultLaunch [] = { "Web Entrez", NULL };
static CharPtr  medLaunch [] = { "Local", "Web Entrez", "ASN.1", NULL };
static CharPtr  prtLaunch [] = { "Local", "Sequin", "Web Entrez", "ASN.1", NULL };
static CharPtr  nucLaunch [] = { "Local", "Sequin", "Web Entrez", "ASN.1", NULL };
static CharPtr  strucLaunch [] = { "Cn3D", "Web Entrez", "ASN.1", NULL };
static CharPtr  genLaunch [] = { "Local", "Sequin", "Web Entrez", "ASN.1", NULL };
static CharPtr  popsetLaunch [] = { "Local", "Sequin", "Web Entrez", "ASN.1", NULL };
static CharPtr  omimLaunch [] = { "Web Entrez", "ASN.1", NULL };
static CharPtr  taxonomyLaunch [] = { "Web Entrez", NULL };
static CharPtr  bookLaunch [] = { "Web Entrez", NULL };
static CharPtr  probeLaunch [] = { "Web Entrez", NULL };
static CharPtr  domainLaunch [] = { "Web Entrez", NULL };
static CharPtr  stsLaunch [] = { "Web Entrez", NULL };
static CharPtr  cddLaunch [] = { "Web Entrez", NULL };
static CharPtr  snpLaunch [] = { "Web Entrez", NULL };
static CharPtr  journalsLaunch [] = { "Web Entrez", NULL };
static CharPtr  unigeneLaunch [] = { "Web Entrez", NULL };
static CharPtr  pmcLaunch [] = { "Web Entrez", NULL };
static CharPtr  ncbisearchLaunch [] = { "Web Entrez", NULL };
static CharPtr  meshLaunch [] = { "Web Entrez", NULL };
static CharPtr  geneLaunch [] = { "Web Entrez", NULL };

/*-----------------------------------*/
/* Data structures used to keep info */
/* about the current window.         */
/*-----------------------------------*/

typedef struct docsumstatedata {
  unsigned int    checked:1;
  unsigned int    hasAbstract:1;
  unsigned int    noSuchUid:1;
} DocSumStateData, PNTR DocSumStateDataPtr;

typedef struct summformdata {
  FORM_MESSAGE_BLOCK
  E2NamedUidListProc    refineUidProc;
  PopuP                 target;
  DoC                   docsum;
  PopuP                 formatPopups [MAX_DBS + 1];
  PopuP                 launchPopups [MAX_DBS + 1];
  CharPtr               label;
  GrouP                 controls;

  Int2                  dsClickItem;
  Int2                  dsClickRow;
  Int2                  dsClickCol;
  Boolean               wasDoubleClick;

  Boolean               usingDelay;
  Int2                  retrieveMode;

  ButtoN                retrieve;
  ButtoN                refine;

  EnumFieldAssocPtr     dbalist;

  Int4Ptr               uids;
  BoolPtr               formatted;
  SimpleSeqPtr PNTR     simple;
  DocSumStateDataPtr    state;
  Int2                  numUids;
  Int2                  numParents;

  Int4Ptr               neighbors;
  Int2                  numNeighbors;
  Int2                  neighborDb;
  Int2                  currDb;

  Int2                  lineHeight;
  Int2                  linesPerIcon;

  CharPtr               historyFile;
  Int4Ptr               historyOffsets;
  Int2                  generations;
  Int2                  present;

  ButtoN                nextBtn;
  ButtoN                prevBtn;

  FonT                  docsumFont;

  Entrez2DocsumListPtr  tempE2DLP;
} SummFormData, PNTR SummFormPtr;


extern Entrez2ReplyPtr SpecialEntrezSynchronousQuery (
  Entrez2RequestPtr e2rq
);

/*==================================================================*/
/*                                                                  */
/*  BioseqFindEntityByGi () -                                       */
/*                                                                  */
/*==================================================================*/

static Uint2 BioseqFindEntityByGi (Int4 uid, Uint2Ptr itemIDptr)

{
  ValNode  vn;

  if (uid <= 0) return 0;
  vn.choice = SEQID_GI;
  vn.data.intvalue = uid;
  return BioseqFindEntity (&vn, itemIDptr);
}

/*==================================================================*/
/*                                                                  */
/*  SetDefaultFailureMessage () -                                   */
/*                                                                  */
/*==================================================================*/

static CharPtr SetDefaultFailureMessage (SummFormPtr sfp, Int2 item, CharPtr prefix)

{
  Char          buf [40];
  CharPtr       dbname = NULL;
  SimpleSeqPtr  ssp;
  Char          tmp [64];
  Int4          uid;

  tmp [0] = '\0';
  if (prefix == NULL)
    prefix = "";

  if (sfp == NULL)
    return StringSave ("?");

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
      sprintf ("%sIncorrect seq-entry for database %s", prefix, dbname);
    } else {
      StringCpy (buf, "?");
      if (ssp->numid > 0 && ssp->bestid < ssp->numid) {
        StringNCpy_0 (buf, ssp->id [ssp->bestid], sizeof (buf));
      }
      sprintf ("%sProblem with sequence %s in %s database", prefix, buf, dbname);
    }
  } else {
    sprintf ("%sInternal confusion for database %s", prefix, dbname);
  }

  return StringSave (tmp);
}

/*==================================================================*/
/*                                                                  */
/*  DoDrawCheck () -                                                */
/*                                                                  */
/*==================================================================*/

static void DoDrawCheck (SummFormPtr sfp, RectPtr r, Int2 item, Int2 frst, Boolean docsum)

{
  Int2  checked;
  RecT  rct;

  if (sfp == NULL || sfp->state == NULL) return;
  if (item < 1 || item > sfp->numUids) return;
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

/*==================================================================*/
/*                                                                  */
/*  DrawLocalCheck () -                                             */
/*                                                                  */
/*==================================================================*/

static void DrawLocalCheck (SummFormPtr sfp, RectPtr r, Int2 item, Int2 frst, Boolean docsum)

{
  SimpleSeqPtr  ssp;

  if (sfp == NULL || sfp->simple == NULL || sfp->state == NULL) return;
  if (item < 1 || item > sfp->numUids) return;
  ssp = sfp->simple [item - 1];
  if (ssp == NULL) return;
  DoDrawCheck (sfp, r, item, frst, docsum);
}

/*==================================================================*/
/*                                                                  */
/*  DrawUidCheck () -                                               */
/*                                                                  */
/*==================================================================*/

static void DrawUidCheck (SummFormPtr sfp, RectPtr r, Int2 item, Int2 frst, Boolean docsum)

{
  Int4  uid;

  if (sfp == NULL || sfp->uids == NULL || sfp->state == NULL) return;
  if (item < 1 || item > sfp->numUids) return;
  uid = sfp->uids [item - 1];
  if (uid < 1) return;
  DoDrawCheck (sfp, r, item, frst, docsum);
}

/*==================================================================*/
/*                                                                  */
/*  DrawIcon () - Figures out which icon to draw (based on the      */
/*               current db) and then draws it in the proper spot.  */
/*                                                                  */
/*==================================================================*/

static void DrawIcon (SummFormPtr sfp, RectPtr r, Int2 item, Int2 frst)

{
  Int2      db;
  CharPtr   dbName;
  Int2      hasAbstract;
  Uint1Ptr  icon;
  RecT      rct;
  Int4      uid;

  /*------------------------------------*/
  /* Perform sanity check on parameters */
  /*------------------------------------*/

  if (sfp == NULL || sfp->uids == NULL || sfp->state == NULL) return;
  if (item < 1 || item > sfp->numUids) return;
  if (frst >= sfp->linesPerIcon) return;

  /*---------------------------------*/
  /* Get the current state of things */
  /*---------------------------------*/

  db = sfp->currDb;
  uid = sfp->uids [item - 1];
  hasAbstract = sfp->state [item - 1].hasAbstract;
  if (uid < 1) return;

  /*-----------------------------*/
  /* Calculate which icon to use */
  /*-----------------------------*/

  dbName = DBGetNameFromID (db);

  if (StringICmp (dbName, "PubMed") == 0) {
    if (hasAbstract)
      icon = hasabstract;
    else
      icon = noabstract;
  } else if (StringICmp (dbName, "Protein") == 0)
    icon = proteinicon;
  else if (StringICmp (dbName, "Nucleotide") == 0)
    icon = dnaicon;
  else if (StringICmp (dbName, "Structure") == 0)
    icon = threedicon;
  else if (StringICmp (dbName, "Genome") == 0)
    icon = genomeicon;
  else if (StringICmp (dbName, "Popset") == 0)
    icon = popseticon;
  else if (StringICmp (dbName, "OMIM") == 0)
    icon = omimicon;
  else if (StringICmp (dbName, "Taxonomy") == 0)
    icon = taxonomyicon;
  else if (StringICmp (dbName, "Books") == 0)
    icon = booksicon;
  else if (StringICmp (dbName, "geo") == 0)
    icon = probeicon;
  else if (StringICmp (dbName, "domains") == 0)
    icon = domainicon;
  else if (StringICmp (dbName, "unists") == 0)
    icon = stsicon;
  else if (StringICmp (dbName, "cdd") == 0)
    icon = cddicon;
  else if (StringICmp (dbName, "snp") == 0)
    icon = snpicon;
  else if (StringICmp (dbName, "journals") == 0)
    icon = journalsicon;
  else if (StringICmp (dbName, "unigene") == 0)
    icon = unigeneicon;
  else if (StringICmp (dbName, "PMC") == 0)
    icon = pmcicon;
  else if (StringICmp (dbName, "ncbisearch") == 0)
    icon = ncbisearchicon;
  else if (StringICmp (dbName, "mesh") == 0)
    icon = meshicon;
  else if (StringICmp (dbName, "gene") == 0)
    icon = geneicon;
  else
    icon = genericon;

  /*------------------*/
  /* Display the icon */
  /*------------------*/

  rct = *r;
  rct.left += 3 * stdCharWidth - 8;
  rct.right = rct.left + 32;
  rct.bottom = MIN (rct.bottom, rct.top + 22 - frst * sfp->lineHeight);
  if (RectInRgn (&rct, updateRgn)) {
    if (icon != NULL) {
      CopyBits (&rct, icon + 4 * frst * sfp->lineHeight);
    }
  }
}

/*==================================================================*/
/*                                                                  */
/*  DrawTextSum () -                                                */
/*                                                                  */
/*==================================================================*/

static void DrawTextSum (DoC d, RectPtr r, Int2 item, Int2 frst)

{
  SummFormPtr  sfp;

  sfp = (SummFormPtr) GetObjectExtra (d);
  DrawUidCheck (sfp, r, item, frst, FALSE);
}

/*==================================================================*/
/*                                                                  */
/*  DrawLocalSum () -                                               */
/*                                                                  */
/*==================================================================*/

static void DrawLocalSum (DoC d, RectPtr r, Int2 item, Int2 frst)

{
  SummFormPtr  sfp;

  sfp = (SummFormPtr) GetObjectExtra (d);
  DrawLocalCheck (sfp, r, item, frst, FALSE);
}

/*==================================================================*/
/*                                                                  */
/*  DrawDocSum () -                                                 */
/*                                                                  */
/*==================================================================*/

static void DrawDocSum (DoC d, RectPtr r, Int2 item, Int2 frst)

{
  SummFormPtr  sfp;

  sfp = (SummFormPtr) GetObjectExtra (d);
  DrawUidCheck (sfp, r, item, frst, TRUE);
  DrawIcon (sfp, r, item, frst);
}

/*==================================================================*/
/*                                                                  */
/*  Query_FetchDocSum () -                                          */
/*  Query_FetchFields () -                                          */
/*                                                                  */
/*==================================================================*/

static CharPtr FormatDocsum (Entrez2DocsumPtr e2DocsumPtr)

{
  Entrez2DocsumDataPtr  e2DocsumDataPtr;
  CharPtr               accession;
  CharPtr               accession1;
  CharPtr               authors;
  CharPtr               book;
  CharPtr               caption;
  Char                  ch;
  CharPtr               clustid;
  Int2                  commas;
  CharPtr               comname;
  CharPtr               cpt;
  CharPtr               dcsum;
  CharPtr               description;
  CharPtr               etal;
  CharPtr               extra;
  size_t                len;
  CharPtr               marker;
  CharPtr               mrnasrc1;
  CharPtr               mrnasrc2;
  CharPtr               name;
  CharPtr               oid;
  CharPtr               pdbacc;
  CharPtr               pdbdescr;
  CharPtr               rank;
  CharPtr               sciname;
  CharPtr               scopenote;
  CharPtr               snpid;
  CharPtr               str;
  CharPtr               taxid;
  CharPtr               title;
  CharPtr               title1;
  CharPtr               ttl;
  CharPtr               tmp;
  Uint4                 uid;
  Char                  uidbuf [32];

  if (e2DocsumPtr == NULL) return NULL;

  uid = e2DocsumPtr->uid;

  cpt = NULL;
  ttl = NULL;

  accession = NULL;
  accession1 = NULL;
  authors = NULL;
  book = NULL;
  caption = NULL;
  comname = NULL;
  extra = NULL;
  mrnasrc1 = NULL;
  mrnasrc2 = NULL;
  oid = NULL;
  pdbacc = NULL;
  pdbdescr = NULL;
  rank = NULL;
  sciname = NULL;
  taxid = NULL;
  clustid = NULL;
  marker = NULL;
  snpid = NULL;
  dcsum = NULL;
  title = NULL;
  title1 = NULL;
  name = NULL;
  description = NULL;
  scopenote = NULL;

  for (e2DocsumDataPtr = e2DocsumPtr->docsum_data; e2DocsumDataPtr != NULL; e2DocsumDataPtr = e2DocsumDataPtr->next) {
    if (StringHasNoText (e2DocsumDataPtr->field_value)) continue;
    if (StringICmp (e2DocsumDataPtr->field_name, "Caption") == 0) {
      caption = e2DocsumDataPtr->field_value;
    } else if (StringICmp (e2DocsumDataPtr->field_name, "Title") == 0) {
      title = e2DocsumDataPtr->field_value;
    } else if (StringICmp (e2DocsumDataPtr->field_name, "Authors") == 0 ||
               StringICmp (e2DocsumDataPtr->field_name, "Author") == 0) {
      authors = e2DocsumDataPtr->field_value;
    } else if (StringICmp (e2DocsumDataPtr->field_name, "Accession") == 0) {
      accession = e2DocsumDataPtr->field_value;
    } else if (StringICmp (e2DocsumDataPtr->field_name, "Extra") == 0) {
      extra = e2DocsumDataPtr->field_value;
    } else if (StringICmp (e2DocsumDataPtr->field_name, "TaxID") == 0) {
      taxid = e2DocsumDataPtr->field_value;
    } else if (StringICmp (e2DocsumDataPtr->field_name, "CID") == 0 ||
               StringICmp (e2DocsumDataPtr->field_name, "CLUSTERID") == 0) {
      clustid = e2DocsumDataPtr->field_value;
    } else if (StringICmp (e2DocsumDataPtr->field_name, "ScientificName") == 0) {
      sciname = e2DocsumDataPtr->field_value;
    } else if (StringICmp (e2DocsumDataPtr->field_name, "CommonName") == 0) {
      comname = e2DocsumDataPtr->field_value;
    } else if (StringICmp (e2DocsumDataPtr->field_name, "Rank") == 0) {
      rank = e2DocsumDataPtr->field_value;
    } else if (StringICmp (e2DocsumDataPtr->field_name, "Oid") == 0) {
      oid = e2DocsumDataPtr->field_value;
    } else if (StringICmp (e2DocsumDataPtr->field_name, "Accession1") == 0) {
      accession1 = e2DocsumDataPtr->field_value;
    } else if (StringICmp (e2DocsumDataPtr->field_name, "Title1") == 0) {
      title1 = e2DocsumDataPtr->field_value;
    } else if (StringICmp (e2DocsumDataPtr->field_name, "mRNASource1") == 0) {
      mrnasrc1 = e2DocsumDataPtr->field_value;
    } else if (StringICmp (e2DocsumDataPtr->field_name, "mRNASource2") == 0) {
      mrnasrc2 = e2DocsumDataPtr->field_value;
    } else if (StringICmp (e2DocsumDataPtr->field_name, "Book") == 0) {
      book = e2DocsumDataPtr->field_value;
    } else if (StringICmp (e2DocsumDataPtr->field_name, "Marker_Title") == 0) {
      marker = e2DocsumDataPtr->field_value;
    } else if (StringICmp (e2DocsumDataPtr->field_name, "SNP_ID") == 0) {
      snpid = e2DocsumDataPtr->field_value;
    } else if (StringICmp (e2DocsumDataPtr->field_name, "DOCSUM") == 0) {
      dcsum = e2DocsumDataPtr->field_value;
    } else if (StringICmp (e2DocsumDataPtr->field_name, "Name") == 0) {
      name = e2DocsumDataPtr->field_value;
    } else if (StringICmp (e2DocsumDataPtr->field_name, "Description") == 0) {
      description = e2DocsumDataPtr->field_value;
    } else if (StringICmp (e2DocsumDataPtr->field_name, "ScopeNote") == 0) {
      scopenote = e2DocsumDataPtr->field_value;
    } else if (StringICmp (e2DocsumDataPtr->field_name, "PdbAcc") == 0) {
      pdbacc = e2DocsumDataPtr->field_value;
    } else if (StringICmp (e2DocsumDataPtr->field_name, "PdbDescr") == 0) {
      pdbdescr = e2DocsumDataPtr->field_value;
    }
  }

  cpt = caption;

  ttl = title;

  if (StringHasNoText (cpt)) {
    cpt = accession1;
  }
  if (StringHasNoText (cpt)) {
    cpt = authors;
  }
  if (StringHasNoText (cpt)) {
    cpt = accession;
  }
  if (StringHasNoText (cpt)) {
    cpt = clustid;
  }
  if (StringHasNoText (cpt)) {
    cpt = taxid;
  }
  if (StringHasNoText (cpt)) {
    cpt = oid;
  }
  if (StringHasNoText (cpt)) {
    cpt = book;
  }
  if (StringHasNoText (cpt)) {
    cpt = snpid;
  }
  if (StringHasNoText (cpt) && (! StringHasNoText (title)) && (! StringHasNoText (description))) {
    cpt = title;
    ttl = description;
  }
  if (StringHasNoText (cpt) && (! StringHasNoText (name)) && (! StringHasNoText (description))) {
    cpt = name;
    ttl = description;
  }
  if (StringHasNoText (cpt)) {
    cpt = pdbacc;
  }
  if (StringHasNoText (cpt)) {
    sprintf (uidbuf, "%ld", uid);
    cpt = uidbuf;
  }
  if (StringHasNoText (cpt)) {
    cpt = "?";
  }

  if (StringHasNoText (ttl)) {
    ttl = sciname;
  }
  if (StringHasNoText (ttl)) {
    ttl = mrnasrc1;
  }
  if (StringHasNoText (ttl)) {
    ttl = title1;
  }
  if (StringHasNoText (ttl)) {
    ttl = marker;
  }
  if (StringHasNoText (ttl)) {
    ttl = dcsum;
  }
  if (StringHasNoText (ttl)) {
    ttl = scopenote;
  }
  if (StringHasNoText (ttl)) {
    ttl = pdbdescr;
  }
  if (StringHasNoText (ttl)) {
    ttl = "?";
  }

  len = StringLen (cpt) + StringLen (ttl) + StringLen (extra) +
        StringLen (mrnasrc2) + StringLen (comname) +
        StringLen (", et al.") + StringLen ("vs. ") +
        StringLen (" []") + StringLen ("rs");
  str = MemNew (len + 10);
  if (str != NULL) {
    str [0] = '\0';
    if (cpt == snpid) {
      StringCat (str, "rs");
    }
    StringCat (str, cpt);
    if (cpt == authors) {
      if (StringLen (str) > 64) {
        commas = 0;
        etal = NULL;
        for (tmp = str, ch = *tmp; ch != '\0'; tmp++, ch = *tmp) {
          if (ch == ',') {
            commas++;
            if (etal == NULL) {
              etal = tmp;
            }
          }
        }
        if (commas > 2 && etal != NULL) {
          *etal = '\0';
          StringCat (str, ", et al.");
         }
      }
    }
    StringCat (str, "\t");
    StringCat (str, ttl);
    if (ttl == mrnasrc1) {
      if (! StringHasNoText (mrnasrc2)) {
        StringCat (str, "vs. ");
        StringCat (str, mrnasrc2);
      }
    } else if (ttl == title) {
      if (! StringHasNoText (extra)) {
        StringCat (str, "\r");
        StringCat (str, extra);
      }
    } else if (ttl == sciname) {
      if (StringICmp (rank, "species") == 0) {
        if (! StringHasNoText (comname)) {
          StringCat (str, " [");
          StringCat (str, comname);
          StringCat (str, "]");
        }
      }
    }
  }

  return str;
}

static CharPtr FormatFields (Entrez2DocsumPtr e2DocsumPtr)

{
  Entrez2DocsumDataPtr  e2DocsumDataPtr;
  size_t                len;
  CharPtr               str;

  if (e2DocsumPtr == NULL) return NULL;

  len = 0;
  for (e2DocsumDataPtr = e2DocsumPtr->docsum_data; e2DocsumDataPtr != NULL; e2DocsumDataPtr = e2DocsumDataPtr->next) {
    if (StringHasNoText (e2DocsumDataPtr->field_name)) continue;
    if (StringHasNoText (e2DocsumDataPtr->field_value)) continue;
    len += StringLen (e2DocsumDataPtr->field_name) + StringLen (e2DocsumDataPtr->field_value) + 3;
  }
  str = MemNew (len + 5);
  if (str != NULL) {
    for (e2DocsumDataPtr = e2DocsumPtr->docsum_data; e2DocsumDataPtr != NULL; e2DocsumDataPtr = e2DocsumDataPtr->next) {
      if (StringHasNoText (e2DocsumDataPtr->field_name)) continue;
      if (StringHasNoText (e2DocsumDataPtr->field_value)) continue;
      StringCat (str, e2DocsumDataPtr->field_name);
      StringCat (str, "\t");
      StringCat (str, e2DocsumDataPtr->field_value);
      StringCat (str, "\n");
    }
  }

  return str;
}

typedef CharPtr (*FormatE2DSPProc) (Entrez2DocsumPtr e2DocsumPtr);

static CharPtr Query_FetchDocSumCommon (DoC d, Int2 item, FormatE2DSPProc proc)

{
  Int2                  attributes;
  Entrez2RequestPtr     e2RequestPtr = NULL;
  Entrez2ReplyPtr       e2ReplyPtr;
  Entrez2DocsumDataPtr  e2DocsumDataPtr;
  Entrez2DocsumListPtr  e2DocsumListPtr;
  Entrez2DocsumPtr      e2DocsumPtr;
  CharPtr               dbName;
  CharPtr               failed;
  Int2                  i;
  Int2                  j;
  Int2                  numToFetch;
  SummFormPtr           sfp;
  CharPtr               str;
  Int4                  uid;
  Int4                  uids [30];

  /*------------------------------------------*/
  /* Make sure that initial conditions are ok */
  /*------------------------------------------*/

  sfp = (SummFormPtr) GetObjectExtra (d);
  failed = SetDefaultFailureMessage (sfp, item, "\r\t");
  if (proc == NULL) return failed;
  if (sfp == NULL || sfp->uids == NULL ||
      sfp->state == NULL || sfp->formatted == NULL) return failed;
  if (item < 1) return failed;

  /*----------------------------*/
  /* Fetch the document summary */
  /* for the request UID.       */
  /*----------------------------*/

  uid = sfp->uids [item - 1];
  dbName = DBGetNameFromID (sfp->currDb);

  /* check existing cache */

  if (sfp->tempE2DLP != NULL) {
    e2DocsumListPtr = sfp->tempE2DLP;
    for (e2DocsumPtr = e2DocsumListPtr->list; e2DocsumPtr != NULL; e2DocsumPtr = e2DocsumPtr->next) {
      if (uid == e2DocsumPtr->uid) {
        str = proc (e2DocsumPtr);
        /* sfp->state [item - 1].hasAbstract = (Boolean) (! e2DocsumPtr->no_abstract); */
        sfp->state [item - 1].hasAbstract = FALSE;
        for (e2DocsumDataPtr = e2DocsumPtr->docsum_data; e2DocsumDataPtr != NULL; e2DocsumDataPtr = e2DocsumDataPtr->next) {
          if (StringHasNoText (e2DocsumDataPtr->field_value)) continue;
          if (StringICmp (e2DocsumDataPtr->field_name, "Attributes") == 0) {
            if (StrToInt (e2DocsumDataPtr->field_value, &attributes)) {
              if ((attributes & 1) != 0) {
                sfp->state [item - 1].hasAbstract = TRUE;
              }
            }
          }
        }
        sfp->formatted [item - 1] = TRUE;
        MemFree (failed);
        return str;
      }
    }
  }

  sfp->tempE2DLP = Entrez2DocsumListFree (sfp->tempE2DLP);

  /* create new request */

  uids [0] = uid;
  numToFetch = 1;
  for (i = item, j = 0; i < sfp->numUids && j < 20 && numToFetch < 20; i++, j++) {
    if (! sfp->formatted [i]) {
      uids [numToFetch] = sfp->uids [i];
      numToFetch++;
    }
  }
  for (i = item - 1, j = 0; i >= 0 && j < 20 && numToFetch < 30; i--, j++) {
    if (! sfp->formatted [i]) {
      uids [numToFetch] = sfp->uids [i];
      numToFetch++;
    }
  }

  /* accelerate docsum page requesting several at once */

#ifdef WIN_MOTIF
  if ((getenv ("NCBI_ENTREZ_SLOW_DOCSUMS")) == NULL) {
    e2RequestPtr = EntrezCreateDocSumRequest (dbName, 0, (Int4) numToFetch, uids, NULL);
  }
#else
  e2RequestPtr = EntrezCreateDocSumRequest (dbName, 0, (Int4) numToFetch, uids, NULL);
#endif

  /* one at a time request if not grouping, to make sure server did not slow down again */

  if (e2RequestPtr == NULL) {
    e2RequestPtr = EntrezCreateDocSumRequest (dbName, uid, 0, NULL, NULL);
  }

  if (e2RequestPtr == NULL) return failed;

  if (ShowASN () == TRUE)
    DisplayEntrezRequest (e2RequestPtr);

  e2ReplyPtr = SpecialEntrezSynchronousQuery (e2RequestPtr);
  if (e2ReplyPtr == NULL) return failed;

  if (ShowASN () == TRUE)
    DisplayEntrezReply (e2ReplyPtr);

  sfp->tempE2DLP = EntrezExtractDocsumReply (e2ReplyPtr);
  Entrez2RequestFree (e2RequestPtr);
  if (sfp->tempE2DLP == NULL) return failed;

  /* check new cache */

  if (sfp->tempE2DLP != NULL) {
    e2DocsumListPtr = sfp->tempE2DLP;
    for (e2DocsumPtr = e2DocsumListPtr->list; e2DocsumPtr != NULL; e2DocsumPtr = e2DocsumPtr->next) {
      if (uid == e2DocsumPtr->uid) {
        str = proc (e2DocsumPtr);
        /* sfp->state [item - 1].hasAbstract = (Boolean) (! e2DocsumPtr->no_abstract); */
        sfp->state [item - 1].hasAbstract = FALSE;
        for (e2DocsumDataPtr = e2DocsumPtr->docsum_data; e2DocsumDataPtr != NULL; e2DocsumDataPtr = e2DocsumDataPtr->next) {
          if (StringHasNoText (e2DocsumDataPtr->field_value)) continue;
          if (StringICmp (e2DocsumDataPtr->field_name, "Attributes") == 0) {
            if (StrToInt (e2DocsumDataPtr->field_value, &attributes)) {
              if ((attributes & 1) != 0) {
                sfp->state [item - 1].hasAbstract = TRUE;
              }
            }
          }
        }
        sfp->formatted [item - 1] = TRUE;
        MemFree (failed);
        return str;
      }
    }
  }

  return failed;
}

static CharPtr Query_FetchDocSum (DoC d, Int2 item, Pointer ptr)

{
  return Query_FetchDocSumCommon (d, item, FormatDocsum);
}

static CharPtr Query_FetchFields (DoC d, Int2 item, Pointer ptr)

{
  return Query_FetchDocSumCommon (d, item, FormatFields);
}

/*==================================================================*/
/*                                                                  */
/*  FileToString () -                                               */
/*                                                                  */
/*==================================================================*/

static CharPtr FileToString (CharPtr path)

{
  Int2     actual;
  FILE     *fp;
  Int4     len;
  CharPtr  ptr;

  if (path == NULL) return NULL;
  ptr = NULL;
  len = FileLength (path);
  if (len > 0 && len < MAXALLOC) {
    fp = FileOpen (path, "r");
    if (fp != NULL) {
      ptr = MemNew (sizeof (Char) * (size_t) (len + 4));
      if (ptr != NULL) {
        actual = FileRead (ptr, 1, (size_t) len, fp);
        if (actual > 0 && actual <= len) {
          ptr [actual] = '\0';
        }
      }
      FileClose (fp);
    }
  }
  return ptr;
}

/*==================================================================*/
/*                                                                  */
/*  FetchUid () -                                                   */
/*                                                                  */
/*==================================================================*/

static CharPtr FetchUid (DoC d, Int2 item, Pointer ptr)

{
  CharPtr         failed;
  SummFormPtr     sfp;
  Char            str [64];
  Int4            uid;

  /*--------------------------*/
  /* Check initial conditions */
  /*--------------------------*/

  sfp = (SummFormPtr) GetObjectExtra (d);
  failed = SetDefaultFailureMessage (sfp, item, NULL);
  if (sfp == NULL || sfp->uids == NULL || sfp->state == NULL) return failed;
  if (item < 1) return failed;

  /*------------------------------------*/
  /* Get the UID for the requested item */
  /*------------------------------------*/

  uid = sfp->uids [item - 1];
  if (uid < 1) return failed;
  sprintf (str, "%ld\n", (long) uid);

  /*----------------------------------*/
  /* Clean up and return successfully */
  /*----------------------------------*/

  MemFree (failed);
  return StringSave (str);
}

/*==================================================================*/
/*                                                                  */
/*  FetchPubMed () -                                                */
/*                                                                  */
/*==================================================================*/

typedef Boolean (*FormatMedlineProc) (MedlineEntryPtr, FILE*);

static CharPtr FetchPubMed (DoC d, Int2 item, Pointer ptr, FormatMedlineProc func)

{
  CharPtr         failed;
  FILE           *fp;
  Char            path [PATH_MAX];
  PubmedEntryPtr  pep;
  SummFormPtr     sfp;
  CharPtr         str;
  Int4            uid;

  /*--------------------------*/
  /* Check initial conditions */
  /*--------------------------*/

  sfp = (SummFormPtr) GetObjectExtra (d);
  failed = SetDefaultFailureMessage (sfp, item, NULL);

  if (sfp == NULL || sfp->uids == NULL || sfp->state == NULL) return failed;
  if (item < 1) return failed;

  uid = sfp->uids [item - 1];

  if ((pep = PubMedSynchronousQuery (uid)) == NULL) return failed;

  /*------------------------*/
  /* Store to temp file and */
  /* then read back in.     */
  /*------------------------*/

  str = NULL;
  TmpNam (path);
  fp = FileOpen (path, "w");
  if (fp != NULL) {
    if (func ((MedlineEntryPtr) pep->medent, fp)) {
      FileClose (fp);
      str = FileToString (path);
    } else {
      FileClose (fp);
    }
  }
  FileRemove (path);
  PubmedEntryFree (pep);

  if (str == NULL) return failed;

  MemFree (failed);
  return str;
}

static CharPtr FetchAbstract (DoC d, Int2 item, Pointer ptr)

{
  return FetchPubMed (d, item, ptr, MedlineEntryToAbsFile);
}

static CharPtr FetchCitation (DoC d, Int2 item, Pointer ptr)

{
  return FetchPubMed (d, item, ptr, MedlineEntryToDocFile);
}

static CharPtr FetchMedline (DoC d, Int2 item, Pointer ptr)

{
  return FetchPubMed (d, item, ptr, MedlineEntryToDataFile);
}

/*==================================================================*/
/*                                                                  */
/*  GetBioseqForUid () -                                            */
/*                                                                  */
/*==================================================================*/

static BioseqPtr GetBioseqForUid (SeqEntryPtr topsep, Int4 uid)

{
  BioseqPtr       bsp;
  SeqEntryPtr     oldsep;
  ValNode         vn;

  if (topsep == NULL || uid == 0)
    return NULL;
  vn.choice = SEQID_GI;
  vn.extended = 0;
  vn.data.intvalue = uid;
  vn.next = NULL;
  oldsep = SeqEntrySetScope (topsep);
  bsp = BioseqFind (&vn);
  SeqEntrySetScope (oldsep);
  return bsp;
}

/*==================================================================*/
/*                                                                  */
/*  GetSequenceComplexity () -                                      */
/*                                                                  */
/*==================================================================*/

static Int2 GetSequenceComplexity (void)

{
  Entrez2GlobalsPtr  egp;
  Int2               retcode = 0;
  Int2               val;

  egp = (Entrez2GlobalsPtr) GetAppProperty ("Entrez2Globals");
  if (egp == NULL || egp->seqComplex == NULL) return retcode;
  val = GetValue (egp->seqComplex);
  switch (val) {
    case 1 :
      retcode = 0;
      break;
    case 2 :
      retcode = SEQENTRY_READ_NUC_PROT;
      break;
    case 3 :
      retcode = SEQENTRY_READ_SEG_SET;
      break;
    case 4 :
      retcode = SEQENTRY_READ_BIOSEQ;
     break;
    default :
      retcode = 0;
      break;
  }
  return retcode;
}

/*==================================================================*/
/*                                                                  */
/*  FetchSequence () -                                              */
/*                                                                  */
/*==================================================================*/

static void FormatCdsSeq (SeqFeatPtr sfp, Pointer userdata)

{
  BioseqPtr          bsp;
  Char               buf [512];
  SeqMgrFeatContext  cdscontext;
  SeqFeatPtr         gene = NULL;
  SeqMgrFeatContext  genecontext;
  GeneRefPtr         grp;
  FILE               *fp;
  SeqIdPtr           sip;
  SeqPortPtr         spp;
  Char               tmp [32];

  if (sfp->data.choice != SEQFEAT_CDREGION) return;
  fp = (FILE *) userdata;
  bsp = BioseqFindFromSeqLoc (sfp->location);
  if (bsp == NULL) return;
  if (sfp != SeqMgrGetDesiredFeature (0, bsp, 0, 0, sfp, &cdscontext)) return;
  grp = SeqMgrGetGeneXref (sfp);
  if (grp == NULL || (! SeqMgrGeneIsSuppressed (grp))) {
    gene = SeqMgrGetOverlappingGene (sfp->location, &genecontext);
  }
  spp = SeqPortNewByLoc (sfp->location, Seq_code_iupacna);
  if (spp == NULL) return;
  sip = SeqIdFindWorst (bsp->id);
  SeqIdWrite (sip, buf, PRINTID_TEXTID_ACC_VER, sizeof (buf) - 1);
  sprintf (tmp, "_cds_%ld", (long) sfp->idx.itemID);
  StringCat (buf, tmp);
  FastaFileFunc (bsp, FASTA_ID, buf, sizeof (buf), (Pointer) fp);
  buf [0] = '\0';
  if (! StringHasNoText (cdscontext.label)) {
    StringCat (buf, "[protein=");
    StringCat (buf, cdscontext.label);
    StringCat (buf, "] ");
  }
  if (! StringHasNoText (genecontext.label)) {
    StringCat (buf, "[gene=");
    StringCat (buf, genecontext.label);
    StringCat (buf, "] ");
  }
  TrimSpacesAroundString (buf);
  FastaFileFunc (bsp, FASTA_DEFLINE, buf, sizeof (buf), (Pointer) fp);
  while (FastaSeqLine (spp, buf, 70, TRUE)) {
    FastaFileFunc (bsp, FASTA_SEQLINE, buf, sizeof (buf), (Pointer) fp);
  }
  SeqPortFree (spp);
  FastaFileFunc (bsp, FASTA_EOS, buf, sizeof (buf), (Pointer)fp);
}

static CharPtr FetchSequence (DoC d, Int2 item, Pointer ptr, FmtType format,
                              Boolean do_fasta, Boolean do_cds, Boolean is_na)

{
  BioseqPtr    bsp;
  CharPtr      failed;
  FILE         *fp;
  Int4         flags = -1;
  Uint1        group_segs = 0;
  ErrSev       level;
  Boolean      okay = FALSE;
  Char         path [PATH_MAX];
  Int2         retcode;
  SeqEntryPtr  seqEntryPtr;
  SeqEntryPtr  sep;
  SummFormPtr  sfp;
  CharPtr      str;
  Int4         uid;
  CharPtr      currDbName;

  sfp = (SummFormPtr) GetObjectExtra (d);
  failed = SetDefaultFailureMessage (sfp, item, NULL);
  if (sfp == NULL || sfp->uids == NULL || sfp->state == NULL) return failed;
  if (item < 1) return failed;
  uid = sfp->uids [item - 1];

  currDbName = DBGetNameFromID (sfp->currDb);
  str = NULL;

  retcode = GetSequenceComplexity ();
  if ((seqEntryPtr = PubSeqSynchronousQuery (uid, retcode, flags)) == NULL) return failed;

  bsp = GetBioseqForUid (seqEntryPtr, uid);
  sep = SeqMgrGetSeqEntryForData (bsp);
  if (sep == NULL) {
    SeqEntryFree (seqEntryPtr);
    return failed;
  }

  if ((! do_fasta) && (! do_cds)) {
    LookupFarSeqIDs (seqEntryPtr, TRUE, TRUE, TRUE, TRUE, TRUE);
  }

  /*------------------------*/
  /* Store to temp file and */
  /* then read back in.     */
  /*------------------------*/

  str = NULL;
  TmpNam (path);
  fp = FileOpen (path, "w");
  if (fp != NULL) {
    level = ErrSetMessageLevel (SEV_MAX);
    if (do_fasta) {
      if (bsp->repr == Seq_repr_seg) {
        group_segs = 1;
      } else if (bsp->repr == Seq_repr_delta) {
        group_segs = 3;
      }
      okay = SeqEntrysToFasta (seqEntryPtr, fp, is_na, group_segs);
    } else if (do_cds) {
      SeqMgrIndexFeatures (0, (Pointer) bsp);
      VisitFeaturesInSep (seqEntryPtr, (Pointer) fp, FormatCdsSeq);
      okay = TRUE;
    } else {
      okay = SeqEntryToGnbk (sep, NULL, format, RELEASE_MODE, NORMAL_STYLE, 0,
                             LOOKUP_FAR_COMPONENTS | LOOKUP_FAR_LOCATIONS |
                             LOOKUP_FAR_PRODUCTS | LOOKUP_FAR_HISTORY,
                             0, NULL, fp);
    }
    if (okay) {
      FileClose (fp);
      str = FileToString (path);
    } else {
      FileClose (fp);
    }
    ErrSetMessageLevel (level);
  }
  FileRemove (path);
  SeqEntryFree (seqEntryPtr);

  if (str == NULL) return failed;

  MemFree (failed);
  return str;
}

static CharPtr FetchGenBank (DoC d, Int2 item, Pointer ptr)

{
  return FetchSequence (d, item, ptr, GENBANK_FMT, FALSE, FALSE, FALSE);
}

static CharPtr FetchEmbl (DoC d, Int2 item, Pointer ptr)

{
  return FetchSequence (d, item, ptr, EMBL_FMT, FALSE, FALSE, FALSE);
}

static CharPtr FetchGenPept (DoC d, Int2 item, Pointer ptr)

{
  return FetchSequence (d, item, ptr, GENPEPT_FMT, FALSE, FALSE, FALSE);
}

static CharPtr FetchFastaNuc (DoC d, Int2 item, Pointer ptr)

{
  return FetchSequence (d, item, ptr, 0, TRUE, FALSE, TRUE);
}

static CharPtr FetchFastaProt (DoC d, Int2 item, Pointer ptr)

{
  return FetchSequence (d, item, ptr, 0, TRUE, FALSE, FALSE);
}

static CharPtr FetchFastaCDS (DoC d, Int2 item, Pointer ptr)

{
  return FetchSequence (d, item, ptr, 0, FALSE, TRUE, TRUE);
}

/*==================================================================*/
/*                                                                  */
/*  FetchPDB () -                                                   */
/*                                                                  */
/*==================================================================*/

static CharPtr FetchPDB (DoC d, Int2 item, Pointer ptr)

{
  BiostrucPtr     bsp = NULL;
  BiostrucSeqPtr  bsqp = NULL;
  CharPtr         failed;
  FILE            *fp;
  Char            path [PATH_MAX];
  PDNMS           pdnms;
  SummFormPtr     sfp;
  CharPtr         str;
  Int4            uid;

  str = NULL;
  sfp = (SummFormPtr) GetObjectExtra (d);
  failed = SetDefaultFailureMessage (sfp, item, NULL);
  if (sfp == NULL || sfp->uids == NULL || sfp->state == NULL) return failed;
  if (item < 1) return failed;
  uid = sfp->uids [item - 1];
  bsqp = StrucSynchronousQuery (uid);
  if (bsqp == NULL)  return failed;
  bsp = bsqp->structure;
  if (bsp == NULL) return failed;

  ClearStructures ();
  pdnms = MakeAModelstruc (bsp);
  if (pdnms == NULL) return failed;

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

/*==================================================================*/
/*                                                                  */
/*  FetchLocalBioseq () -                                           */
/*                                                                  */
/*==================================================================*/

static CharPtr FetchLocalBioseq (DoC d, Int2 item, Pointer ptr)

{
  CharPtr       failed;
  FILE          *fp;
  Char          path [PATH_MAX];
  SummFormPtr   sfp;
  SimpleSeqPtr  ssp;
  CharPtr       str;

  /*--------------------------*/
  /* Check initial conditions */
  /*--------------------------*/

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
    } else
      FileClose (fp);
  }
  FileRemove (path);

  if (str == NULL) return failed;

  MemFree (failed);
  return str;
}

/*==================================================================*/
/*                                                                  */
/*  SetDocSumImportExportItems () -                                 */
/*                                                                  */
/*==================================================================*/

static void SetDocSumImportExportItems (SummFormPtr sfp)

{
  IteM          exportItm;
  IteM          importItm;
  CharPtr       dbName;
  CharPtr PNTR  labels = defaultRadios;
  Boolean       lastChoiceIsUidList = FALSE;
  Char          tmp [64];
  Int2          val;

  if (sfp == NULL)
    return;

  importItm = FindFormMenuItem ((BaseFormPtr) sfp, VIB_MSG_IMPORT);
  if (importItm != NULL)
    SafeSetTitle (importItm, "Import FASTA or Uid List...");

  exportItm = FindFormMenuItem ((BaseFormPtr) sfp, VIB_MSG_EXPORT);
  if (exportItm == NULL) return;

  sfp->label = medRadios [0];
  if (sfp->simple != NULL)
    sfp->label = localBioseqRadios [0];
  else if (sfp->currDb >= 0 && sfp->currDb < MAX_DBS) {
    dbName = DBGetNameFromID (sfp->currDb);
    if (StringICmp (dbName, "PubMed") == 0)
      labels = medRadios;
    else if (StringICmp (dbName, "Protein") == 0)
      labels = prtRadios;
    else if (StringICmp (dbName, "Nucleotide") == 0)
      labels = nucRadios;
    else if (StringICmp (dbName, "Structure") == 0)
      labels = strucRadios;
    else if (StringICmp (dbName, "Genome") == 0)
      labels = genRadios;
    else if (StringICmp (dbName, "Popset") == 0)
      labels = popsetRadios;
    else if (StringICmp (dbName, "OMIM") == 0)
      labels = omimRadios;
    else if (StringICmp (dbName, "Taxonomy") == 0)
      labels = taxonomyRadios;
    else if (StringICmp (dbName, "Books") == 0)
      labels = bookRadios;
    else if (StringICmp (dbName, "geo") == 0)
      labels = probeRadios;
    else if (StringICmp (dbName, "domains") == 0)
      labels = domainRadios;
    else if (StringICmp (dbName, "unists") == 0)
      labels = stsRadios;
    else if (StringICmp (dbName, "cdd") == 0)
      labels = cddRadios;
    else if (StringICmp (dbName, "snp") == 0)
      labels = snpRadios;
    else if (StringICmp (dbName, "journals") == 0)
      labels = journalsRadios;
    else if (StringICmp (dbName, "unigene") == 0)
      labels = unigeneRadios;
    else if (StringICmp (dbName, "PMC") == 0)
      labels = pmcRadios;
    else if (StringICmp (dbName, "ncbisearch") == 0)
      labels = ncbisearchRadios;
    else if (StringICmp (dbName, "mesh") == 0)
      labels = meshRadios;
    else if (StringICmp (dbName, "gene") == 0)
      labels = geneRadios;
    else
      labels = defaultRadios;

    val = GetValue (sfp->formatPopups [sfp->currDb]);
    if (val > 0) {
      sfp->label = labels [val - 1];
      if (labels [val] == NULL)
        lastChoiceIsUidList = TRUE;
    }
  }
  StringCpy (tmp, "Export ");
  StringCat (tmp, sfp->label);
  if (lastChoiceIsUidList)
    StringCat (tmp, " List");
  StringCat (tmp, "...");
  SafeSetTitle (exportItm, tmp);
}

/*==================================================================*/
/*                                                                  */
/*  Query_FetchNeighbors() - Gets UIDs for the neighbors of all the */
/*                           currently selected documents.          */
/*                                                                  */
/*    Input :                                                       */
/*       sfp->currDb           -- The database to link FROM         */
/*       sfp->neighborDb       -- The database to link TO           */
/*       sfp->uids             -- The list of ALL the current       */
/*                                documents.                        */
/*       sfp->numUids          -- Count of documents in sfp->uids   */
/*       sfp->state [n].checked -- Flag to indicate which of the     */
/*                                documents in sfp->uids are        */
/*                                selected.                         */
/*                                                                  */
/*    Output :                                                      */
/*       sfp->numNeighbors -- The number of neighbors found.        */
/*       sfp->neighbors    -- The uids of the neighbors.            */
/*                                                                  */
/*==================================================================*/

static Boolean Query_FetchNeighbors (SummFormPtr sfp, Int2 num)

{
  Int2                i;
  Int4Ptr             uids;
  Boolean             found;
  Entrez2RequestPtr   e2RequestPtr;
  Entrez2ReplyPtr     e2ReplyPtr;
  Entrez2LinkSetPtr   e2LinksPtr;
  CharPtr             currDbName;
  CharPtr             targetDbName;
  Entrez2InfoPtr      e2ip;
  Entrez2DbInfoPtr    e2db;
  Entrez2LinkInfoPtr  dbLinks;

  /*------------------*/
  /* Check conditions */
  /*------------------*/

  if (sfp == NULL) return FALSE;

  e2ip = Query_GetInfo ();
  if (e2ip == NULL) return FALSE;

  /*---------------------------------------*/
  /* Allocate memory for the selected UIDs */
  /*---------------------------------------*/

  uids = MemNew ((size_t) (num + 1) * sizeof (DocUid));
  if (uids == NULL) return FALSE;

  /*----------------------------------*/
  /* Determine the link type based on */
  /* the current (source) and the     */
  /* target db.                       */
  /*----------------------------------*/

  currDbName = DBGetNameFromID (sfp->currDb);
  targetDbName = DBGetNameFromID (sfp->neighborDb);

  found = FALSE;
  for (e2db = e2ip->db_info; e2db != NULL; e2db = e2db->next) {
    if (StringICmp (e2db->db_name, currDbName) == 0) {
      found = TRUE;
      break;
    }
  }

  if (! found) return FALSE;

  found = FALSE;
  for (dbLinks = e2db->links; dbLinks != NULL; dbLinks = dbLinks->next) {
    if (StringICmp (dbLinks->db_to, targetDbName) == 0) {
      found = TRUE;
      break;
    }
  }

  if (! found) return FALSE;

  /*----------------------*/
  /* Get the uids for the */
  /* selected items.      */
  /*----------------------*/

  num = 0;
  for (i = 0; i < sfp->numUids; i++) {
    if (sfp->state [i].checked) {
      uids [num] = sfp->uids [i];
      num++;
    }
  }

  /*----------------------------------------*/
  /* Create a link list request, send it to */
  /* the server, and parse the result.      */
  /*----------------------------------------*/

  e2RequestPtr = EntrezCreateGetLinksRequest (currDbName, 0, num, uids, NULL, dbLinks->link_name, MAX_UIDS, FALSE, TRUE);
  if (e2RequestPtr == NULL) return FALSE;

  if (ShowASN () == TRUE)
    DisplayEntrezRequest (e2RequestPtr);

  if ((e2ReplyPtr = SpecialEntrezSynchronousQuery (e2RequestPtr)) == NULL)
    return FALSE;

  if (ShowASN () == TRUE)
    DisplayEntrezReply (e2ReplyPtr);

  if ((e2LinksPtr = EntrezExtractLinksReply (e2ReplyPtr)) == NULL) return FALSE;

  sfp->numNeighbors = (Int2) e2LinksPtr->ids->num;
  sfp->neighbors = (Int4Ptr) BSMerge (e2LinksPtr->ids->uids, NULL);

  /*----------------------------------*/
  /* Clean up and return successfully */
  /*----------------------------------*/

  MemFree (uids);
  return TRUE;
}

/*==================================================================*/
/*                                                                  */
/*  RecalculateDocSum () -                                          */
/*                                                                  */
/*==================================================================*/

static void RecalculateDocSum (SummFormPtr sfp)

{
  Int2    i;
  Int2    num;
  Int2    targetDb;
  Char    title [32];
  UIEnum  val;

  /*------------------*/
  /* Check conditions */
  /*------------------*/

  if (sfp == NULL) return;

  /*-------------------*/
  /* Get the target DB */
  /*-------------------*/

  if (GetEnumPopup (sfp->target, sfp->dbalist, &val)) {
    targetDb = (Int2) val;
  } else {
    targetDb = sfp->currDb;
  }

  /*-------------------------------------------*/
  /* If there's nothing to lookup, just return */
  /*-------------------------------------------*/

  if (sfp->uids == NULL || sfp->state == NULL) {
    if (targetDb == sfp->currDb)
      SafeSetTitle (sfp->retrieve, "Neighbor 0");
    else
      SafeSetTitle (sfp->retrieve, "Lookup 0");
    SafeDisable (sfp->retrieve);
    return;
  }

  sfp->neighbors = MemFree (sfp->neighbors);
  sfp->numNeighbors = 0;
  sfp->neighborDb = targetDb;

  /*----------------------------------*/
  /* FInd out how many, if any, items */
  /* are marked.                      */
  /*----------------------------------*/

  num = 0;
  for (i = 0; i < sfp->numUids; i++) {
    if (sfp->state [i].checked) {
      num++;
    }
  }

  /*----------------------------------------*/
  /* Update the refine and retrieve buttons */
  /*----------------------------------------*/

  if (sfp->retrieveMode == EVAL_MODE && sfp->usingDelay) {
    if (num == 0) {
      if (targetDb == sfp->currDb)
        SafeSetTitle (sfp->retrieve, "Neighbor 0");
      else
        SafeSetTitle (sfp->retrieve, "Lookup 0");
      SafeDisable (sfp->retrieve);
    } else {
      SafeSetTitle (sfp->retrieve, "Evaluate");
      SafeEnable (sfp->retrieve);
    }
    if (num == 0)
      num = sfp->numUids;
    sprintf (title, "Refine %d", (int) num);
    SafeSetTitle (sfp->refine, title);
    if (num > 0)
      SafeEnable (sfp->refine);
    else
      SafeDisable (sfp->refine);
    return;
  }

  /*------------------------------------------*/
  /* If there's any items to be queried, then */
  /* get a count and a list of UIDs for them. */
  /*------------------------------------------*/

  if (num > 0)
    Query_FetchNeighbors (sfp, num);

  /*--------------------------*/
  /* Update the button labels */
  /*--------------------------*/

  if (targetDb == sfp->currDb)
    sprintf (title, "Neighbor %d", (int) sfp->numNeighbors);
  else
    sprintf (title, "Lookup %d", (int) sfp->numNeighbors);

  SafeSetTitle (sfp->retrieve, title);
  if (sfp->numNeighbors > 0)
    SafeEnable (sfp->retrieve);
  else
    SafeDisable (sfp->retrieve);

  if (num == 0)
    num = sfp->numUids;
  sprintf (title, "Refine %d", (int) num);
  SafeSetTitle (sfp->refine, title);
  if (num > 0)
    SafeEnable (sfp->refine);
  else
    SafeDisable (sfp->refine);
}

/*==================================================================*/
/*                                                                  */
/*  RepopulateDocSum () -                                           */
/*                                                                  */
/*==================================================================*/

static DocPrntProc defaultDocProcs [] = { Query_FetchDocSum, Query_FetchFields, FetchUid, NULL };
static DocPrntProc mlDocProcs [] = { Query_FetchDocSum, Query_FetchFields, FetchAbstract, FetchCitation, FetchMedline, FetchUid, NULL };
static DocPrntProc aaDocProcs [] = { Query_FetchDocSum, Query_FetchFields, FetchGenPept, FetchFastaProt, FetchUid, NULL };
static DocPrntProc ntDocProcs [] = { Query_FetchDocSum, Query_FetchFields, FetchGenBank, FetchEmbl, FetchFastaNuc, FetchFastaCDS, FetchUid, NULL };
static DocPrntProc stDocProcs [] = { Query_FetchDocSum, Query_FetchFields, FetchPDB, FetchUid, NULL };
static DocPrntProc chDocProcs [] = { Query_FetchDocSum, Query_FetchFields, FetchUid, NULL };
static DocPrntProc popsetDocProcs [] = { Query_FetchDocSum, Query_FetchFields, FetchUid, NULL };
static DocPrntProc omimDocProcs [] = { Query_FetchDocSum, Query_FetchFields, FetchUid, NULL };
static DocPrntProc taxonomyDocProcs [] = { Query_FetchDocSum, Query_FetchFields, FetchUid, NULL };
static DocPrntProc bookDocProcs [] = { Query_FetchDocSum, Query_FetchFields, FetchUid, NULL };
static DocPrntProc probeDocProcs [] = { Query_FetchDocSum, Query_FetchFields, FetchUid, NULL };
static DocPrntProc domainDocProcs [] = { Query_FetchDocSum, Query_FetchFields, FetchUid, NULL };
static DocPrntProc stsDocProcs [] = { Query_FetchDocSum, Query_FetchFields, FetchUid, NULL };
static DocPrntProc cddDocProcs [] = { Query_FetchDocSum, Query_FetchFields, FetchUid, NULL };
static DocPrntProc snpDocProcs [] = { Query_FetchDocSum, Query_FetchFields, FetchUid, NULL };
static DocPrntProc journalsDocProcs [] = { Query_FetchDocSum, Query_FetchFields, FetchUid, NULL };
static DocPrntProc unigeneDocProcs [] = { Query_FetchDocSum, Query_FetchFields, FetchUid, NULL };
static DocPrntProc pmcDocProcs [] = { Query_FetchDocSum, Query_FetchFields, FetchUid, NULL };
static DocPrntProc ncbisearchDocProcs [] = { Query_FetchDocSum, Query_FetchFields, FetchUid, NULL };
static DocPrntProc meshDocProcs [] = { Query_FetchDocSum, Query_FetchFields, FetchUid, NULL };
static DocPrntProc geneDocProcs [] = { Query_FetchDocSum, Query_FetchFields, FetchUid, NULL };

static void RepopulateDocSum (SummFormPtr sfp, Boolean needToReset)

{
  ColPtr               colFmt;
  CharPtr              dbName;
  DocDrawProc          ddp;
  Int2                 estLines;
  Int2                 firstLine;
  Int2                 firstShown;
  FonT                 font;
  MedlineViewProcsPtr  mvpp;
  DocPrntProc          retrieveProc;
  BaR                  sb;
  Int4                 startsAt;
  SeqViewProcsPtr      svpp;
  Int2                 val;

  /*------------------------*/
  /* Make sure that initial */
  /* conditions are valid   */
  /*------------------------*/

  if (sfp == NULL) return;

  /*---------------*/
  /*---------------*/

  WatchCursor ();
  SafeHide (sfp->docsum);
  Update ();

  /*----------------*/
  /*----------------*/

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

  /*----------------*/
  /*----------------*/

  dbName = DBGetNameFromID (sfp->currDb);

  retrieveProc = Query_FetchDocSum;
  ddp = DrawDocSum;
  colFmt = docsumColFmt;
  estLines = 3;
  font = sfp->docsumFont;

  if (sfp->simple != NULL) {
    retrieveProc = FetchLocalBioseq;
    ddp = DrawLocalSum;
    colFmt = textColFmt;
    svpp = (SeqViewProcsPtr) GetAppProperty ("SeqDisplayForm");
    if (svpp != NULL && svpp->displayFont != NULL)
      font = svpp->displayFont;
  }

  else if (sfp->currDb >= 0 && sfp->currDb < MAX_DBS) {
    val = GetValue (sfp->formatPopups [sfp->currDb]);
    if (val > 0) {
      if (val > 2) {
        ddp = DrawTextSum;
        colFmt = textColFmt;
        estLines = 30;
        if ((StringICmp (dbName, "PubMed") == 0) && (val == 5)) {
          mvpp = (MedlineViewProcsPtr)
            GetAppProperty ("MedlineDisplayForm");
          if (mvpp != NULL && mvpp->displayFont != NULL)
            font = mvpp->displayFont;
        } else if ((StringICmp (dbName, "Protein") == 0) || (StringICmp (dbName, "Nucleotide") == 0)) {
          svpp = (SeqViewProcsPtr) GetAppProperty ("SeqDisplayForm");
          if (svpp != NULL && svpp->displayFont != NULL)
            font = svpp->displayFont;
        }
      }

      if (StringICmp (dbName, "PubMed") == 0)
        retrieveProc = mlDocProcs [val - 1];
      else if (StringICmp (dbName, "Protein") == 0)
        retrieveProc = aaDocProcs [val - 1];
      else if (StringICmp (dbName, "Nucleotide") == 0)
        retrieveProc = ntDocProcs [val - 1];
      else if (StringICmp (dbName, "Structure") == 0)
        retrieveProc = stDocProcs [val - 1];
      else if (StringICmp (dbName, "Genome") == 0)
        retrieveProc = chDocProcs [val - 1];
      else if (StringICmp (dbName, "Popset") == 0)
        retrieveProc = popsetDocProcs [val - 1];
      else if (StringICmp (dbName, "OMIM") == 0)
        retrieveProc = omimDocProcs [val - 1];
      else if (StringICmp (dbName, "Taxonomy") == 0)
        retrieveProc = taxonomyDocProcs [val - 1];
      else if (StringICmp (dbName, "Books") == 0)
        retrieveProc = bookDocProcs [val - 1];
      else if (StringICmp (dbName, "geo") == 0)
        retrieveProc = probeDocProcs [val - 1];
      else if (StringICmp (dbName, "domains") == 0)
        retrieveProc = domainDocProcs [val - 1];
      else if (StringICmp (dbName, "unists") == 0)
        retrieveProc = stsDocProcs [val - 1];
      else if (StringICmp (dbName, "cdd") == 0)
        retrieveProc = domainDocProcs [val - 1];
      else if (StringICmp (dbName, "snp") == 0)
        retrieveProc = snpDocProcs [val - 1];
      else if (StringICmp (dbName, "journals") == 0)
        retrieveProc = journalsDocProcs [val - 1];
      else if (StringICmp (dbName, "unigene") == 0)
        retrieveProc = unigeneDocProcs [val - 1];
      else if (StringICmp (dbName, "PMC") == 0)
        retrieveProc = pmcDocProcs [val - 1];
      else if (StringICmp (dbName, "ncbisearch") == 0)
        retrieveProc = ncbisearchDocProcs [val - 1];
      else if (StringICmp (dbName, "mesh") == 0)
        retrieveProc = meshDocProcs [val - 1];
      else if (StringICmp (dbName, "gene") == 0)
        retrieveProc = geneDocProcs [val - 1];
      else
        retrieveProc = defaultDocProcs [val - 1];

    }
  }

  /*--------------------------------*/
  /* Retrieve the docs and put them */
  /* into the docsum window.        */
  /*--------------------------------*/

  MemSet ((Pointer) sfp->formatted, 0, sizeof (Boolean) * (size_t) sfp->numUids);

  BulkAppendItem (sfp->docsum, sfp->numUids, retrieveProc, estLines, &docsumParFmt, colFmt, font);

  /*-------------------------------------*/
  /* Draw and display the document panel */
  /*-------------------------------------*/

  SetDocShade (sfp->docsum, ddp, NULL, NULL, NULL);
  SetDocCache (sfp->docsum, StdPutDocCache, StdGetDocCache, StdResetDocCache);

  AdjustDocScroll (sfp->docsum);
  GetItemParams4 (sfp->docsum, firstShown, &startsAt, NULL, NULL, NULL, NULL);
  CorrectBarValue (sb, startsAt + firstLine);
  ResetClip ();
  SafeShow (sfp->docsum);
  ArrowCursor ();
  Update ();
}

/*==================================================================*/
/*                                                                  */
/*  ChangeTarget () -                                               */
/*                                                                  */
/*==================================================================*/

static void ChangeTarget (PopuP p)

{
  SummFormPtr  sfp;

  sfp = (SummFormPtr) GetObjectExtra (p);
  if (sfp == NULL) return;
  sfp->retrieveMode = EVAL_MODE;
  RecalculateDocSum (sfp);
}

/*==================================================================*/
/*                                                                  */
/*  ChangeFormat () -                                               */
/*                                                                  */
/*==================================================================*/

static void ChangeFormat (PopuP p)

{
  SummFormPtr  sfp;

  sfp = (SummFormPtr) GetObjectExtra (p);
  if (sfp == NULL) return;
  RepopulateDocSum (sfp, TRUE);
  SetDocSumImportExportItems (sfp);
}

/*==================================================================*/
/*                                                                  */
/*  ResizeDocSumForm () -                                            */
/*                                                                  */
/*==================================================================*/

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
  SafeShow (sfp->controls);
  RepopulateDocSum (sfp, TRUE);
  ArrowCursor ();
  Update ();
}

/*==================================================================*/
/*                                                                  */
/*  CleanupEntrezDocSumForm () -                                    */
/*                                                                  */
/*==================================================================*/

static void CleanupEntrezDocSumForm (GraphiC g, VoidPtr data)

{
  Int2         j;
  SummFormPtr  sfp;

  sfp = (SummFormPtr) data;
  if (sfp != NULL) {
    MemFree (sfp->uids);
    MemFree (sfp->formatted);
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
    Entrez2DocsumListFree (sfp->tempE2DLP);
  }
  StdCleanupFormProc (g, data);
}

/*==================================================================*/
/*                                                                  */
/*  ClickDocSum () -                                                */
/*                                                                  */
/*==================================================================*/

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

/*==================================================================*/
/*                                                                  */
/*  GetCheckedUids () -                                             */
/*                                                                  */
/*==================================================================*/

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

/*==================================================================*/
/*                                                                  */
/*  LaunchMedlineViewer () -                                        */
/*                                                                  */
/*==================================================================*/

static void LaunchMedlineViewer (Int4 uid)

{
  Pointer          dataptr;
  Uint2            datatype;
  Uint2            entityID;
  Int2             handled;
  MedlineEntryPtr  mep;
  PubmedEntryPtr   pep;

  WatchCursor ();
  Update ();
  pep = PubMedSynchronousQuery (uid);
  if (pep == NULL) {
    ArrowCursor ();
    Update ();
    Message (MSG_OK, "Unable to find this record in the database.");
    return;
  }
  datatype = OBJ_MEDLINE_ENTRY;
  mep = (MedlineEntryPtr) pep->medent;
  if (mep != NULL && mep->uid == 0) {
    mep->uid = uid;
  }
  dataptr = (Pointer) mep;
  entityID = ObjMgrRegister (datatype, dataptr);
  if (dataptr == NULL || entityID == 0) {
    ArrowCursor ();
    Update ();
    Message (MSG_OK, "Unable to find this record in the database.");
    return;
  }
  handled = GatherProcLaunch (OMPROC_VIEW, FALSE, entityID, 1, OBJ_MEDLINE_ENTRY, 0, 0, OBJ_MEDLINE_ENTRY, 0);
  ArrowCursor ();
  if (handled != OM_MSG_RET_DONE || handled == OM_MSG_RET_NOPROC) {
    Message (MSG_ERROR, "Unable to launch viewer.");
    PubmedEntryFree (pep);
  } else {
    ObjMgrSetOptions (OM_OPT_FREE_IF_NO_VIEW, entityID);
  }
}

/*==================================================================*/
/*                                                                  */
/*  BioseqLockByGi () -                                             */
/*                                                                  */
/*==================================================================*/

static BioseqPtr BioseqLockByGi (Int4 uid)

{
  ValNode  vn;

  if (uid <= 0) return NULL;
  vn.choice = SEQID_GI;
  vn.data.intvalue = uid;
  return BioseqLockById (&vn);
}

/*==================================================================*/
/*                                                                  */
/*  AddBlastAlignment () -                                          */
/*                                                                  */
/*==================================================================*/

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
  if (subject == NULL)
    return;
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
  } else
    return;
  search = BLASTSetUpSearch (subject, options->program_name, 0, 0, NULL, options, NULL);

  for (i = 0; i < numAlign; i++) {
    if (alignuids [i] != uid) {
      query = BioseqLockByGi (alignuids [i]);
      if (query != NULL) {
        salp = BlastSequencesOnTheFly (search, query);
        BioseqUnlock (query);
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

/*==================================================================*/
/*                                                                  */
/*  LaunchSequenceViewer () -                                       */
/*                                                                  */
/*==================================================================*/

static void LaunchSequenceViewer (Int4 uid, Int2 numAlign, Int4Ptr alignuids, CharPtr dbName)

{
  BioseqPtr          bsp;
  Entrez2GlobalsPtr  egp;
  Uint2              entityID;
  Int4               flags = -1;
  Int2               handled;
  Uint2              itemID;
  Int2               retcode;
  SeqEntryPtr        seqEntryPtr;
  ValNode            vn;

  WatchCursor ();
  Update ();
  retcode = GetSequenceComplexity ();
  seqEntryPtr = PubSeqSynchronousQuery (uid, retcode, flags);
  if (seqEntryPtr == NULL) {
    ArrowCursor ();
    Update ();
    Message (MSG_OK, "Unable to find this record in the database.");
    return;
  }
  entityID = BioseqFindEntityByGi (uid, &itemID);
  if (entityID == 0 || itemID == 0) {
    SeqEntryFree (seqEntryPtr);
    ArrowCursor ();
    Update ();
    return;
  }

  MemSet ((Pointer) &vn, 0, sizeof (ValNode));
  vn.choice = SEQID_GI;
  vn.data.intvalue = uid;
  bsp = BioseqFind (&vn);
  if (bsp == NULL) {
    ArrowCursor ();
    Update ();
    Message (MSG_OK, "Unable to find this record in memory.");
    return;
  }

  /*
  LookupFarSeqIDs (seqEntryPtr, TRUE, TRUE, TRUE, TRUE, TRUE);
  */

  egp = (Entrez2GlobalsPtr) GetAppProperty ("Entrez2Globals");
  if (egp != NULL) {
    SeqEntryPtr   sep;

    if (GetStatus (egp->alignWithChecked)) {
      sep = GetTopSeqEntryForEntityID (entityID);
      AddBlastAlignment (uid, numAlign, alignuids, entityID, sep, bsp);
    }
  }

  handled = GatherProcLaunch (OMPROC_VIEW, FALSE, entityID, itemID, OBJ_BIOSEQ, 0, 0, OBJ_BIOSEQ, 0);
  ArrowCursor ();
  if (handled != OM_MSG_RET_DONE || handled == OM_MSG_RET_NOPROC) {
    Message (MSG_ERROR, "Unable to launch viewer.");
    SeqEntryFree (seqEntryPtr);
  } else {
    ObjMgrSetOptions (OM_OPT_FREE_IF_NO_VIEW, entityID);
  }
}

/*==================================================================*/
/*                                                                  */
/*  LaunchSequin () -                                               */
/*                                                                  */
/*==================================================================*/

static void LaunchSequin (Int4 uid, Int2 numAlign, Int4Ptr alignuids, CharPtr dbName)

{
  AsnIoPtr           aip;
  BioseqPtr          bsp;
  Entrez2GlobalsPtr  egp;
  Uint2              entityID;
  Int4               flags = -1;
  Uint2              itemID;
  Char               path [PATH_MAX];
  Int2               retcode;
  SeqEntryPtr        seqEntryPtr;
  ValNode            vn;
#ifdef WIN_MOTIF
  Char               cmmd [PATH_MAX];
#endif

  WatchCursor ();
  Update ();
  retcode = GetSequenceComplexity ();
  seqEntryPtr = PubSeqSynchronousQuery (uid, retcode, flags);
  if (seqEntryPtr == NULL) {
    ArrowCursor ();
    Update ();
    Message (MSG_OK, "Unable to find this record in the database.");
    return;
  }
  entityID = BioseqFindEntityByGi (uid, &itemID);
  if (entityID == 0 || itemID == 0) {
    SeqEntryFree (seqEntryPtr);
    ArrowCursor ();
    Update ();
    return;
  }

  MemSet ((Pointer) &vn, 0, sizeof (ValNode));
  vn.choice = SEQID_GI;
  vn.data.intvalue = uid;
  bsp = BioseqFind (&vn);
  if (bsp == NULL) {
    ArrowCursor ();
    Update ();
    Message (MSG_OK, "Unable to find this record in memory.");
    return;
  }

  egp = (Entrez2GlobalsPtr) GetAppProperty ("Entrez2Globals");
  if (egp != NULL) {
    SeqEntryPtr   sep;

    if (GetStatus (egp->alignWithChecked)) {
      sep = GetTopSeqEntryForEntityID (entityID);
      AddBlastAlignment (uid, numAlign, alignuids, entityID, sep, bsp);
    }
  }

  TmpNam (path);
  aip = AsnIoOpen (path, "w");
  if (aip == NULL) {
    ArrowCursor ();
    FileRemove (path);
    return;
  }

  SeqEntryAsnWrite (seqEntryPtr, aip, NULL);
  AsnIoClose (aip);

#ifdef WIN_MAC
  Nlm_SendOpenDocAppleEvent (path, "SEQN");
#endif
#ifdef WIN_MSWIN
  Nlm_MSWin_OpenApplication ("sequin.exe", path);
#endif
#ifdef WIN_MOTIF
  sprintf (cmmd, "sequin -x < %s &", path);
  system (cmmd);
#endif

  ArrowCursor ();
}

/*==================================================================*/
/*                                                                  */
/*  LaunchStructureViewer () -                                      */
/*                                                                  */
/*==================================================================*/

static Int4 GetBiostrucComplexity (void)

{
  Int4               complexity = ALLMDL;
  Entrez2GlobalsPtr  egp;
  Int2               val;

  egp = (Entrez2GlobalsPtr) GetAppProperty ("Entrez2Globals");
  if (egp == NULL) return complexity;
  val = GetValue (egp->strucComplex);
  switch (val) {
    case 1:
      complexity = ONECOORDATOM;
      break;
    case 2:
      complexity = ONECOORDRES;
      break;
    case 3:
      complexity = ALLMDL;
      break;
    case 4:
      complexity = VECMODEL;
      break;
    case 5:
      complexity = BSEVERYTHING;
      break;
    default:
      complexity = ONECOORDATOM;
      break;
  }
  return complexity;
}

static Int4 GetBiostrucMaxModels (void)

{
  Entrez2GlobalsPtr  egp;
  Int4               maxModels = INT4_MAX;
  Int2               val;

  egp = (Entrez2GlobalsPtr) GetAppProperty ("Entrez2Globals");
  if (egp == NULL) return maxModels;
  val = GetValue (egp->strucModels);
  switch (val) {
    case 1:
      maxModels = 1;
      break;
    case 2:
      maxModels = 2;
      break;
    case 3:
      maxModels = 5;
      break;
    case 4:
      maxModels = 10;
      break;
    case 5:
      maxModels = 15;
      break;
    case 6:
      maxModels = 20;
      break;
    case 7:
      maxModels = MAX_MDLNO;
      break;
    default:
      maxModels = 1;
      break;
  }
  return maxModels;
}

static void LaunchStructureViewer (Int4 uid)

{
#if defined(WIN16)
  Message (MSG_OK, "Structure views not supported on this platform.");
#else
  AsnIoPtr        aip;
  BiostrucSeqPtr  bsqp = NULL;
  NcbiMimeAsn1    mime;
  Char            path [PATH_MAX];

#ifdef WIN_MOTIF
  Char            cmmd [PATH_MAX];
#endif
#ifdef WIN_MSWIN
  Char            prog [PATH_MAX];
#endif

  WatchCursor ();
  Update ();
  bsqp = StrucSynchronousQuery (uid);
  if (bsqp == NULL) {
    ArrowCursor ();
    Update ();
    Message (MSG_OK, "Unable to find this record in the database.");
    return;
  }

  TmpNam (path);
  aip = AsnIoOpen (path, "w");

  MemSet ((Pointer) &mime, 0, sizeof (NcbiMimeAsn1));
  mime.choice = NcbiMimeAsn1_strucseq;
  mime.data.ptrvalue = bsqp;
  mime.next = NULL;

  NcbiMimeAsn1AsnWrite (&mime, aip, NULL);
  AsnIoClose (aip);

  BiostrucSeqFree (bsqp);

  /* Cn3D expects Ncbi-mime-asn1, not Biostruc or BiostrucSeq */

#ifdef WIN_MAC
  Nlm_SendOpenDocAppleEvent (path, "Cn3D");
#endif
#ifdef WIN_MSWIN
  Nlm_GetExecPath ("valfile", prog, sizeof (prog));
  Nlm_MSWin_OpenApplication (prog, path);
#endif
#ifdef WIN_MOTIF
  /*
   argv   [0] = path;
   argv   [1] = NULL;
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

/*==================================================================*/
/*                                                                  */
/*  LaunchXXXASN () -                                               */
/*                                                                  */
/*==================================================================*/

static void LaunchMedlineASN (Int4 uid)

{
  AsnIoPtr        aip;
  Char            path [PATH_MAX];
  PubmedEntryPtr  pep;

  WatchCursor ();
  Update ();
  pep = PubMedSynchronousQuery (uid);
  if (pep == NULL) {
    ArrowCursor ();
    Update ();
    Message (MSG_OK, "Unable to find this record in the database.");
    return;
  }
  TmpNam (path);
  aip = AsnIoOpen (path, "w");
  if (aip != NULL) {
    PubmedEntryAsnWrite (pep, aip, NULL);
    AsnIoClose (aip);
    LaunchGeneralTextViewer (path, "PubMed ASN.1");
  }
  FileRemove (path);
  PubmedEntryFree (pep);
  ArrowCursor ();
}

static void LaunchSequenceASN (Int4 uid)

{
  AsnIoPtr     aip;
  Int4         flags = -1;
  Char         path [PATH_MAX];
  Int2         retcode;
  SeqEntryPtr  seqEntryPtr;

  WatchCursor ();
  Update ();
  retcode = GetSequenceComplexity ();
  seqEntryPtr = PubSeqSynchronousQuery (uid, retcode, flags);
  if (seqEntryPtr == NULL) {
    ArrowCursor ();
    Update ();
    Message (MSG_OK, "Unable to find this record in the database.");
    return;
  }
  TmpNam (path);
  aip = AsnIoOpen (path, "w");
  if (aip != NULL) {
    SeqEntryAsnWrite (seqEntryPtr, aip, NULL);
    AsnIoClose (aip);
    LaunchGeneralTextViewer (path, "Sequence ASN.1");
  }
  FileRemove (path);
  SeqEntryFree (seqEntryPtr);
  ArrowCursor ();
}

static void LaunchStructureASN (Int4 uid)

{
  AsnIoPtr        aip;
  BiostrucSeqPtr  bsqp = NULL;
  Char            path [PATH_MAX];

  WatchCursor ();
  Update ();
  bsqp = StrucSynchronousQuery (uid);
  if (bsqp == NULL) {
    ArrowCursor ();
    Update ();
    Message (MSG_OK, "Unable to find this record in the database.");
    return;
  }
  TmpNam (path);
  aip = AsnIoOpen (path, "w");
  if (aip != NULL) {
    BiostrucSeqAsnWrite (bsqp, aip, NULL);
    AsnIoClose (aip);
    LaunchGeneralTextViewer (path, "Structure ASN.1");
  }
  FileRemove (path);
  BiostrucSeqFree (bsqp);
  ArrowCursor ();
}

static void LaunchMimASN (Int4 uid)

{
  AsnIoPtr     aip;
  MimEntryPtr  mep;
  Char         path [PATH_MAX];

  WatchCursor ();
  Update ();
  mep = MimSynchronousQuery (uid);
  if (mep == NULL) {
    ArrowCursor ();
    Update ();
    Message (MSG_OK, "Unable to find this record in the database.");
    return;
  }
  TmpNam (path);
  aip = AsnIoOpen (path, "w");
  if (aip != NULL) {
    MimEntryAsnWrite (mep, aip, NULL);
    AsnIoClose (aip);
    LaunchGeneralTextViewer (path, "MIM ASN.1");
  }
  FileRemove (path);
  MimEntryFree (mep);
  ArrowCursor ();
}

/*==================================================================*/
/*                                                                  */
/*  LaunchRecViewer () -                                            */
/*                                                                  */
/*==================================================================*/

NLM_EXTERN void LaunchRecViewer (ForM f, Int4 uid, Int2 numAlign, Int4Ptr alignuids, Int2 db, Int2 launchType)

{
  CharPtr   dbName;
  Char      id [41];
  SeqIdPtr  sip;

  if (uid == 0) return;

  if (db >= 0 && db < MAX_DBS) {
    SeqEntrySetScope (NULL);
    dbName = DBGetNameFromID (db);
    if (StringICmp (dbName, "PubMed") == 0) {
      if (launchType == 1) {
        LaunchMedlineViewer (uid);
      } else if (launchType == 2) {
        LaunchEntrezURL ("PubMed", uid, "Abstract");
      } else if (launchType == 3) {
        LaunchMedlineASN (uid);
      }
    } else if (StringICmp (dbName, "Protein") == 0) {
      if (launchType == 1) {
        LaunchSequenceViewer (uid, numAlign, alignuids, dbName);
      } else if (launchType == 2) {
        LaunchSequin (uid, numAlign, alignuids, dbName);
      } else if (launchType == 3) {
        LaunchEntrezURL ("Protein", uid, "GenPept");
      } else if (launchType == 4) {
        LaunchSequenceASN (uid);
      }
    } else if (StringICmp (dbName, "Nucleotide") == 0) {
      if (launchType == 1) {
        LaunchSequenceViewer (uid, numAlign, alignuids, dbName);
      } else if (launchType == 2) {
        LaunchSequin (uid, numAlign, alignuids, dbName);
      } else if (launchType == 3) {
        LaunchEntrezURL ("Nucleotide", uid, "GenBank");
      } else if (launchType == 4) {
        LaunchSequenceASN (uid);
      }
    } else if (StringICmp (dbName, "Structure") == 0) {
      if (launchType == 1) {
        LaunchStructureViewer (uid);
      } else if (launchType == 2) {
        LaunchEntrezURL ("Structure", uid, "DocSum");
      } else if (launchType == 3) {
        LaunchStructureASN (uid);
      }
    } else if (StringICmp (dbName, "Genome") == 0) {
      sprintf (id, "gnl|NCBI_GENOMES|%ld", (long) uid);
      sip = SeqIdParse (id);
      uid = GetGIForSeqId (sip);
      SeqIdFree (sip);
      if (launchType == 1) {
        LaunchSequenceViewer (uid, numAlign, alignuids, "Nucleotide");
      } else if (launchType == 2) {
        LaunchSequin (uid, numAlign, alignuids, dbName);
      } else if (launchType == 3) {
        LaunchEntrezURL ("Genome", uid, "DocSum");
      } else if (launchType == 4) {
        LaunchSequenceASN (uid);
      }
    } else if (StringICmp (dbName, "Popset") == 0) {
      if (launchType == 1) {
        LaunchSequenceViewer (uid, numAlign, alignuids, dbName);
      } else if (launchType == 2) {
        LaunchSequin (uid, numAlign, alignuids, dbName);
      } else if (launchType == 3) {
        LaunchEntrezURL ("Popset", uid, "DocSum");
      } else if (launchType == 4) {
        LaunchSequenceASN (uid);
      }
    } else if (StringICmp (dbName, "OMIM") == 0) {
      if (launchType == 1) {
        LaunchEntrezURL ("OMIM", uid, "DocSum");
      } else if (launchType == 2) {
        LaunchMimASN (uid);
      }
    } else if (StringICmp (dbName, "Taxonomy") == 0) {
      if (launchType == 1) {
        LaunchEntrezURL ("Taxonomy", uid, "DocSum");
      }
    } else if (StringICmp (dbName, "Books") == 0) {
      if (launchType == 1) {
        LaunchEntrezURL ("Books", uid, "DocSum");
      }
    } else if (StringICmp (dbName, "geo") == 0) {
      if (launchType == 1) {
        LaunchEntrezURL ("geo", uid, "DocSum");
      }
    } else if (StringICmp (dbName, "domains") == 0) {
      if (launchType == 1) {
        LaunchEntrezURL ("domains", uid, "DocSum");
      }
    } else if (StringICmp (dbName, "unists") == 0) {
      if (launchType == 1) {
        LaunchEntrezURL ("unists", uid, "DocSum");
      }
    } else if (StringICmp (dbName, "cdd") == 0) {
      if (launchType == 1) {
        LaunchEntrezURL ("cdd", uid, "DocSum");
      }
    } else if (StringICmp (dbName, "snp") == 0) {
      if (launchType == 1) {
        LaunchEntrezURL ("snp", uid, "DocSum");
      }
    } else if (StringICmp (dbName, "journals") == 0) {
      if (launchType == 1) {
        LaunchEntrezURL ("journals", uid, "DocSum");
      }
    } else if (StringICmp (dbName, "unigene") == 0) {
      if (launchType == 1) {
        LaunchEntrezURL ("unigene", uid, "DocSum");
      }
    } else if (StringICmp (dbName, "PMC") == 0) {
      if (launchType == 1) {
        LaunchEntrezURL ("PMC", uid, "DocSum");
      }
    } else if (StringICmp (dbName, "ncbiserach") == 0) {
      if (launchType == 1) {
        LaunchEntrezURL ("ncbisearch", uid, "DocSum");
      }
    } else if (StringICmp (dbName, "mesh") == 0) {
      if (launchType == 1) {
        LaunchEntrezURL ("mesh", uid, "DocSum");
      }
    } else if (StringICmp (dbName, "gene") == 0) {
      if (launchType == 1) {
        LaunchEntrezURL ("gene", uid, "DocSum");
      }
    } else {
      /* default so new databases work without code changes */
      LaunchEntrezURL (dbName, uid, "DocSum");
    }
  }
}

/*==================================================================*/
/*                                                                  */
/*  ReleaseDocSum () -                                              */
/*                                                                  */
/*==================================================================*/

static void ReleaseDocSum (DoC d, PoinT pt)

{
  Int2         checked;
  Int2         col;
  Int2         db;
  Int2         item;
  Int2         num;
  RecT         r;
  Int2         row;
  SummFormPtr  sfp;
  Int4         uid;
  Int4Ptr      uids;
  Int2         val;

  sfp = (SummFormPtr) GetObjectExtra (d);
  if (sfp == NULL) return;
  MapDocPoint (d, pt, &item, &row, &col, &r);
  if (item < 1 || item > sfp->numUids || row == 0 || col == 0) return;
  if (sfp->dsClickItem != item) return;
  if (row == 1 && col == 1 && sfp->dsClickRow == 1 && sfp->dsClickCol == 1) {
    r.right = r.left + sfp->lineHeight;
    r.bottom = r.top + sfp->lineHeight;
    if (PtInRect (pt, &r)) {
      checked = sfp->state [item - 1].checked;
      sfp->state [item - 1].checked = (Boolean) (!checked);
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
    if (sfp->launchPopups != NULL) {
      val = GetValue (sfp->launchPopups [db]);
      LaunchRecViewer (sfp->form, uid, num, uids, db, val);
      MemFree (uids);
      return;
    }
    LaunchRecViewer (sfp->form, uid, num, uids, db, 1);
    MemFree (uids);
  }
}


/*==================================================================*/
/*                                                                  */
/*  RefineProc () -                                                 */
/*                                                                  */
/*==================================================================*/

static int  refinement = 0;  /* Guarantees that each refinement has a unique name. */

static void RefineProc (ButtoN b)

{
  Int2         i;
  Int2         num;
  SummFormPtr  sfp;
  Char         str [64];
  Int4Ptr      uids;

  /*--------------------------*/
  /* Check initial conditions */
  /*--------------------------*/

  sfp = (SummFormPtr) GetObjectExtra (b);
  if (sfp == NULL || sfp->refineUidProc == NULL) return;

  /*------------------------------*/
  /* Get a count of checked items */
  /*------------------------------*/

  num = 0;
  for (i = 0; i < sfp->numUids; i++) {
    if (sfp->state [i].checked) {
      num++;
    }
  }

  /*------------------------------------------*/
  /* If any items are checked, then use them, */
  /* otherwise use all the current UIDs.      */
  /*------------------------------------------*/

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
      sfp->refineUidProc (sfp->form, str, num, uids, sfp->currDb);
    }
    MemFree (uids);
  } else {
    sfp->refineUidProc (sfp->form, str, sfp->numUids, sfp->uids, sfp->currDb);
  }
}

/*==================================================================*/
/*                                                                  */
/*  UidInList () -                                                  */
/*                                                                  */
/*==================================================================*/

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

/*==================================================================*/
/*                                                                  */
/*  RetrieveNeighbors () -                                          */
/*                                                                  */
/*==================================================================*/

static void RetrieveNeighbors (ButtoN b)

{
  Entrez2GlobalsPtr  egp;
  Int2               i, j;
  Int2               num;
  Int2               parents;
  Boolean            persist;
  SummFormPtr        sfp;
  Int4Ptr            sorted;
  Int4               tmp;
  Int4Ptr            uids;
  CharPtr            dbName;

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

  egp = (Entrez2GlobalsPtr) GetAppProperty ("Entrez2Globals");
  if (egp == NULL) return;

  persist = FALSE;
  if (sfp->currDb == sfp->neighborDb)
    persist = GetStatus (egp->parentsPersist);

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
          if (!UidInList (sfp->neighbors [i], sorted, parents)) {
            uids [num] = sfp->neighbors [i];
            num++;
          }
        }
      }
    }
  }
  sfp->neighbors = NULL;
  sfp->numNeighbors = 0;
  dbName = DBGetNameFromID (sfp->neighborDb);
  RetrieveDocs (sfp->form, num, parents, uids, dbName);
  MemFree (uids);
  MemFree (sorted);
}

/*==================================================================*/
/*                                                                  */
/*  EntrezDocSumFormActivate_Callback () = Called when the Document */
/*         summary window is made active. Mostly just calls the     */
/*         function provided by the controlling program.            */
/*                                                                  */
/*==================================================================*/

static void EntrezDocSumFormActivate_Callback (WindoW w)

{
  SummFormPtr  sfp;

  sfp = (SummFormPtr) GetObjectExtra (w);
  if (sfp != NULL) {
    if (sfp->activate != NULL)
      sfp->activate (w);
    SetDocSumImportExportItems (sfp);
  }
}

/*==================================================================*/
/*                                                                  */
/*  CopyDocsumToClipboard () -                                      */
/*                                                                  */
/*==================================================================*/

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

/*==================================================================*/
/*                                                                  */
/*  PrintDocsumProc () -                                            */
/*                                                                  */
/*==================================================================*/

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

/*==================================================================*/
/*                                                                  */
/*  SaveDocsumProject () -                                          */
/*                                                                  */
/*==================================================================*/

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
      if (!(GetOutputFileName (path, sizeof (path), "")))
        return FALSE;
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

/*==================================================================*/
/*                                                                  */
/*  ImportDocSumForm () -                                           */
/*                                                                  */
/*==================================================================*/

static Boolean ImportDocSumForm (ForM f, CharPtr filename)

{
  Pointer            dataptr;
  Uint2              datatype;
  Entrez2GlobalsPtr  egp;
  FILE               *fp;
  ValNodePtr         head = NULL;
  Char               path [PATH_MAX];
  ValNodePtr         simples;
  SummFormPtr        sfp;
  ValNodePtr         vnp;

  path [0] = '\0';
  StringNCpy_0 (path, filename, sizeof (path));
  sfp = (SummFormPtr) GetObjectExtra (f);
  if (sfp != NULL) {
    if (path [0] != '\0' || GetInputFileName (path, sizeof (path), "", "TEXT")) {
      fp = FileOpen (path, "r");
      if (fp != NULL) {
        while ((dataptr = ReadAsnFastaOrFlatFile (fp, &datatype, NULL, FALSE, FALSE, TRUE, TRUE)) != NULL)
          ValNodeAddPointer (&head, datatype, dataptr);
        FileClose (fp);
        simples = ValNodeExtractList (&head, OBJ_FASTA);
        if (simples != NULL) {
          egp = (Entrez2GlobalsPtr) GetAppProperty ("Entrez2Globals");
          if (egp != NULL && egp->retrieveSimpleProc != NULL)
            egp->retrieveSimpleProc (NULL, simples);
        }
        ValNodeFree (simples);
        if (head != NULL) {
          for (vnp = head; vnp != NULL; vnp = vnp->next) {
            datatype = vnp->choice;
            dataptr = vnp->data.ptrvalue;
          }
        }
        ValNodeFree (head);
        return TRUE;
      }
    }
  }

  return FALSE;
}

/*==================================================================*/
/*                                                                  */
/*  ExportDocSumForm () -                                           */
/*                                                                  */
/*==================================================================*/

static Boolean ExportDocSumForm (ForM f, CharPtr filename)

{
  FILE          *fp;
  Int2          i;
  CharPtr       dbName;
  CharPtr PNTR  labels = defaultRadios;
  Boolean       lastChoiceIsUidList = FALSE;
  MonitorPtr    mon;
  Boolean       newline;
  Int2          num;
  Char          path [PATH_MAX];
  SummFormPtr   sfp;
  Char          str [32];
  Int2          val;

 /*----------------------*/
  /* Get SummFormPtr info */
 /*----------------------*/

  sfp = (SummFormPtr) GetObjectExtra (f);

  if (sfp == NULL) return FALSE;

 /*--------------------------------*/
  /* If no filename was passed, get */
  /* one from the user.             */
 /*--------------------------------*/

  path [0] = '\0';
  StringNCpy_0 (path, filename, sizeof (path));

  if (path [0] == '\0' && !GetOutputFileName (path, sizeof (path), NULL)) return FALSE;

 /*------------------------------------------*/
  /* On the Mac force the file to get created */
  /* with the desired attributes.             */
 /*------------------------------------------*/

#ifdef WIN_MAC
  fp = FileOpen (path, "r");
  if (fp != NULL)
    FileClose (fp);
  else
    FileCreate (path, "TEXT", "ttxt");
#endif

 /*---------------------------*/
  /* Open the file for writing */
 /*---------------------------*/

  fp = FileOpen (path, "w");
  if (fp == NULL) {
    Message (MSG_ERROR, "Unable to write file.");
    return FALSE;
  }

 /*---------------------------------------------*/
  /* Indicate 'working' state by changing cursor */
 /*---------------------------------------------*/

  WatchCursor ();
  Update ();

 /*---------------------------------*/
  /* Determine whether or not we are */
  /* currently in UID display mode.  */
 /*---------------------------------*/

  if (sfp->simple != NULL) {
  } else if (sfp->currDb >= 0 && sfp->currDb < MAX_DBS) {
    dbName = DBGetNameFromID (sfp->currDb);
    if (StringICmp (dbName, "PubMed") == 0)
      labels = medRadios;
    else if (StringICmp (dbName, "Protein") == 0)
      labels = prtRadios;
    else if (StringICmp (dbName, "Nucleotide") == 0)
      labels = nucRadios;
    else if (StringICmp (dbName, "Structure") == 0)
      labels = strucRadios;
    else if (StringICmp (dbName, "Genome") == 0)
      labels = genRadios;
    else if (StringICmp (dbName, "PopSet") == 0)
      labels = popsetRadios;
    else if (StringICmp (dbName, "OMIM") == 0)
      labels = omimRadios;
    else if (StringICmp (dbName, "Taxonomy") == 0)
      labels = taxonomyRadios;
    else if (StringICmp (dbName, "Books") == 0)
      labels = bookRadios;
    else if (StringICmp (dbName, "geo") == 0)
      labels = probeRadios;
    else if (StringICmp (dbName, "domains") == 0)
      labels = domainRadios;
    else if (StringICmp (dbName, "unists") == 0)
      labels = stsRadios;
    else if (StringICmp (dbName, "cdd") == 0)
      labels = cddRadios;
    else if (StringICmp (dbName, "snp") == 0)
      labels = snpRadios;
    else if (StringICmp (dbName, "journals") == 0)
      labels = journalsRadios;
    else if (StringICmp (dbName, "unigene") == 0)
      labels = unigeneRadios;
    else if (StringICmp (dbName, "PMC") == 0)
      labels = pmcRadios;
    else if (StringICmp (dbName, "ncbisearch") == 0)
      labels = ncbisearchRadios;
    else if (StringICmp (dbName, "mesh") == 0)
      labels = meshRadios;
    else if (StringICmp (dbName, "gene") == 0)
      labels = geneRadios;
    else
      labels = defaultRadios;

    val = GetValue (sfp->formatPopups [sfp->currDb]);
    if (val > 0)
      if (labels [val] == NULL)
        lastChoiceIsUidList = TRUE;
  }

 /*-----------------------------*/
  /* If we're writing a UID list */
  /* then get a header for it.   */
 /*-----------------------------*/

  if (lastChoiceIsUidList) {
    switch (sfp->currDb) {
      case 0:
        fprintf (fp, ">PubMed\n");
        break;
      case 1:
        fprintf (fp, ">Protein\n");
        break;
      case 2:
        fprintf (fp, ">Nucleotide\n");
        break;
      case 3:
        fprintf (fp, ">Structure\n");
        break;
      case 4:
        fprintf (fp, ">Genome\n");
        break;
      case 5:
        fprintf (fp, ">Popset\n");
        break;
      case 6:
        fprintf (fp, ">OMIM\n");
        break;
      case 7:
        fprintf (fp, ">Taxonomy\n");
        break;
      case 8:
        fprintf (fp, ">Books\n");
        break;
      case 9:
        fprintf (fp, ">Probes\n");
        break;
      case 10:
        fprintf (fp, ">Domains\n");
        break;
      case 11:
        fprintf (fp, ">UniSTS\n");
        break;
      case 12:
        fprintf (fp, ">CDD\n");
        break;
      case 13:
        fprintf (fp, ">SNP\n");
        break;
      case 14:
        fprintf (fp, ">Journals\n");
        break;
      case 15:
        fprintf (fp, ">UniGene\n");
        break;
      case 16:
        fprintf (fp, ">PMC\n");
        break;
      case 17:
        fprintf (fp, ">NCBISearch\n");
        break;
      case 18:
        fprintf (fp, ">MeSH\n");
        break;
      case 19:
        fprintf (fp, ">Gene\n");
        break;
      default:
        fprintf (fp, ">?\n");
        break;
    }
  }

 /*-----------------------------*/
  /* Determine how many (if any) */
  /* items have been checked.    */
 /*-----------------------------*/

  num = 0;
  for (i = 0; i < sfp->numUids; i++) {
    if (sfp->state [i].checked) {
      num++;
    }
  }

 /*------------------------------*/
  /* Write to the file either the */
  /* or the document summaries.   */
 /*------------------------------*/

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
        if (newline)
          fputc ('\n', fp);
        SaveDocumentItem (sfp->docsum, fp, i + 1);
        MonitorIntValue (mon, i);
        newline = TRUE;
      }
    }
    MonitorFree (mon);
  }

 /*----------------------------------*/
  /* Clean up and return successfully */
 /*----------------------------------*/

  FileClose (fp);
  sfp->filepath = MemFree (sfp->filepath);
  sfp->filepath = StringSave (path);
  ArrowCursor ();
  Update ();
  return TRUE;

}

/*================================================================*/
/*                                                                */
/*  EntrezDocSumFormMessage_Callback () = This function is set up */
/*       to receive the messages generated by menu items that are */
/*       created with FormCommandItem().                          */
/*                                                                */
/*================================================================*/

static void EntrezDocSumFormMessage_Callback (ForM f, Int2 mssg)

{
  BaseFormPtr        bfp;
  Entrez2GlobalsPtr  egp;
  SummFormPtr        sfp;

  bfp = (BaseFormPtr) GetObjectExtra (f);
  if (bfp != NULL) {
    switch (mssg) {
      case VIB_MSG_REDRAW:
        egp = (Entrez2GlobalsPtr) GetAppProperty ("Entrez2Globals");
        if (egp != NULL) {
          sfp = (SummFormPtr) bfp;
          sfp->docsumFont = egp->docsumFont;
          RepopulateDocSum (sfp, TRUE);
        }
        break;
      case VIB_MSG_COPY:
        CopyDocsumToClipboard (bfp);
        break;
      case VIB_MSG_IMPORT:
        ImportDocSumForm (f, NULL);
        break;
      case VIB_MSG_EXPORT:
        ExportDocSumForm (f, NULL);
        break;
      case VIB_MSG_PRINT:
        PrintDocsumProc (bfp);
        break;
      case VIB_MSG_SAVE_AS:
        SaveDocsumProject (bfp, TRUE);
        break;
      default:
        if (bfp->appmessage != NULL) {
          bfp->appmessage (f, mssg);
        }
        break;
    }
  }
}

/*==================================================================*/
/*                                                                  */
/*  RefreshAndRecalculateDocSum () -                                */
/*                                                                  */
/*==================================================================*/

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

/*==================================================================*/
/*                                                                  */
/*  SelectAllProc () -                                              */
/*                                                                  */
/*==================================================================*/

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

/*==================================================================*/
/*                                                                  */
/*  SelectParentsProc () -                                          */
/*                                                                  */
/*==================================================================*/

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

/*==================================================================*/
/*                                                                  */
/*  ClearAllProcs () -                                              */
/*                                                                  */
/*==================================================================*/

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

/*==================================================================*/
/*                                                                  */
/*  SetPrevAndNextButtons () -                                      */
/*                                                                  */
/*==================================================================*/

static void SetPrevAndNextButtons (SummFormPtr sfp)

{
  if (sfp->present > 1)
    SafeEnable (sfp->prevBtn);
  else
    SafeDisable (sfp->prevBtn);

  if (sfp->present < sfp->generations)
    SafeEnable (sfp->nextBtn);
  else
    SafeDisable (sfp->nextBtn);
}

/*==================================================================*/
/*                                                                  */
/*  RetrieveGeneration () -                                         */
/*                                                                  */
/*==================================================================*/

static void RetrieveGeneration (SummFormPtr sfp, Int2 num, Int2 parents, Int4Ptr uids, Int2 db)

{
  Entrez2InfoPtr  e2ip;
  Int2            i;
  Char            title [32];

  if (sfp == NULL || uids == NULL) return;

  /*----------------------------------*/
  /* Get information on the databases */
  /*----------------------------------*/

  e2ip = Query_GetInfo ();

  /*------------------------------*/
  /* Initialize the DocSum window */
  /*------------------------------*/

  Reset (sfp->docsum);
  SetDocShade (sfp->docsum, NULL, NULL, NULL, NULL);
  SetDocCache (sfp->docsum, NULL, NULL, NULL);
  sfp->uids = MemFree (sfp->uids);
  sfp->formatted = MemFree (sfp->formatted);
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
  if (num > 0)
    SafeEnable (sfp->refine);
  else
    SafeDisable (sfp->refine);
  SetEnumPopup (sfp->target, sfp->dbalist, (UIEnum) db);
  SafeShow (sfp->target);
  SetPrevAndNextButtons (sfp);
  Update ();

  /*-----------------------------------*/
  /* Select the proper DB radio button */
  /*-----------------------------------*/

  Hide (sfp->formatPopups [MAX_DBS]);
  Hide (sfp->launchPopups [MAX_DBS]);
  for (i = 0; i < MAX_DBS; i++) {
    if (i != db && sfp->formatPopups [i] != NULL)
      SafeHide (sfp->formatPopups [i]);
    if (i != db && sfp->launchPopups [i] != NULL)
      SafeHide (sfp->launchPopups [i]);
  }

  if (db < MAX_DBS) {
    Show (sfp->formatPopups [db]);
    Show (sfp->launchPopups [db]);
  }

  /*------------------------------------*/
  /* Load the documents into the window */
  /*------------------------------------*/

  if (num > 0) {
    sfp->uids = MemDup ((Pointer) uids, (size_t) ((num + 1) * sizeof (DocUid)));
    if (sfp->uids == NULL) return;
    sfp->state = MemNew (sizeof (DocSumStateData) * (size_t) (num + 1));
    if (sfp->state == NULL) return;
    sfp->formatted = MemNew (sizeof (Boolean) * (size_t) (num + 1));
    if (sfp->formatted == NULL) return;
    sfp->numUids = num;
    sfp->numParents = parents;
    sfp->currDb = db;
    Show (sfp->form);
    Select (sfp->form);
    RepopulateDocSum (sfp, FALSE);
    SetDocSumImportExportItems (sfp);
    SendMessageToForm (sfp->form, VIB_MSG_CHANGE);
#ifdef WIN_MAC
    if (sfp->activate != NULL)
      sfp->activate ((WindoW) sfp->form);
#endif
  } else {
    Show (sfp->form);
    Select (sfp->form);
#ifdef WIN_MAC
    if (sfp->activate != NULL)
      sfp->activate ((WindoW) sfp->form);
#endif
  }
  Update ();
}

/*==================================================================*/
/*                                                                  */
/*  LoadPresentGeneration ()                                        */
/*                                                                  */
/*==================================================================*/

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
      if (uids != NULL)
        FileRead (uids, sizeof (Int4), (size_t) num, fp);
      FileClose (fp);
      if (uids != NULL) {
        RetrieveGeneration (sfp, num, parents, uids, db);
        MemFree (uids);
      }
    }
  }
  ArrowCursor ();
}

/*==================================================================*/
/*                                                                  */
/*  RetrieveDocs ()                                                 */
/*                                                                  */
/*==================================================================*/

NLM_EXTERN void RetrieveDocs (ForM f, Int2 num, Int2 parents, Int4Ptr uids, CharPtr dbName)

{
  Int2         db;
  FILE         *fp;
  Char         path [PATH_MAX];
  SummFormPtr  sfp;
  Int2         val;

  /*--------------------------*/
  /* Check initial conditions */
  /*--------------------------*/

  if ((sfp = (SummFormPtr) GetObjectExtra (f)) == NULL) return;

  if (uids == NULL) return;

  db = DBGetIDFromName (dbName);

  /*--------------------------*/
  /* Increment the generation */
  /*--------------------------*/

  (sfp->generations)++;
  sfp->present = sfp->generations;

  /*---------------------------------*/
  /* Open the history file, creating */
  /* if necessary.                   */
  /*---------------------------------*/

  if (sfp->historyFile == NULL) {
    TmpNam (path);
    sfp->historyFile = StringSave (path);
#ifdef WIN_MAC
    if (sfp->historyFile != NULL)
      FileCreate (sfp->historyFile, "????", "ENTZ");
#endif
  }

  /*------------------------------*/
  /* Write the current generation */
  /* to the history file.         */
  /*------------------------------*/

  if (sfp->historyOffsets == NULL)
    sfp->historyOffsets = MemNew (100 * sizeof (Int4));

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
    } else
      sfp->historyOffsets [sfp->present % 100] = 0;
  }

  /*--------------------------------*/
  /* Get the new document summaries */
  /*--------------------------------*/

  RetrieveGeneration (sfp, num, parents, uids, db);
}

/*==================================================================*/
/*                                                                  */
/*  PrevButton_Callback () -                                        */
/*                                                                  */
/*==================================================================*/

static void PrevButton_Callback (ButtoN b)

{
  SummFormPtr  sfp;

  sfp = (SummFormPtr) GetObjectExtra (b);
  if (sfp == NULL) return;

  (sfp->present)--;
  if (sfp->present < 1)
    sfp->present = 1;

  SetPrevAndNextButtons (sfp);
  LoadPresentGeneration (sfp);
  SetDocSumImportExportItems (sfp);
}

/*==================================================================*/
/*                                                                  */
/*  NextButton_Callback () -                                        */
/*                                                                  */
/*==================================================================*/

static void NextButton_Callback (ButtoN b)

{
  SummFormPtr  sfp;

  sfp = (SummFormPtr) GetObjectExtra (b);
  if (sfp == NULL) return;

  (sfp->present)++;
  if (sfp->present > sfp->generations)
    sfp->present = sfp->generations;

  SetPrevAndNextButtons (sfp);
  LoadPresentGeneration (sfp);
  SetDocSumImportExportItems (sfp);
}

/*==================================================================*/
/*                                                                  */
/*  NeighborDelayProc () -                                          */
/*                                                                  */
/*==================================================================*/

static void NeighborDelayProc (ChoicE c)

{
  SummFormPtr  sfp;

  sfp = (SummFormPtr) GetObjectExtra (c);
#ifdef WIN_MAC
  if (sfp == NULL) {
    sfp = (SummFormPtr) currentFormDataPtr;
  }
#endif
  if (sfp == NULL) return;
  if (GetValue (c) == 1) {
    sfp->usingDelay = FALSE;
  } else {
    sfp->usingDelay = TRUE;
  }
}

/*==================================================================*/
/*                                                                  */
/*  CreateNeighborDelayChoice () -                                  */
/*                                                                  */
/*==================================================================*/

static ChoicE CreateNeighborDelayChoice (MenU m, BaseFormPtr bfp)

{
  ChoicE       c;
  SummFormPtr  sfp;

  sfp = (SummFormPtr) bfp;

  c = ChoiceGroup (m, NeighborDelayProc);
  SetObjectExtra (c, bfp, NULL);
  ChoiceItem (c, "Immediate");
  ChoiceItem (c, "Delayed");
  if (sfp->usingDelay == TRUE)
    SetValue (c, 2);
  else
    SetValue (c, 1);

  return c;
}

/*==================================================================*/
/*                                                                  */
/*  LoadDocsumOptionsMenu () -                                      */
/*                                                                  */
/*==================================================================*/

static void LoadDocsumOptionsMenu (MenU m)

{
  Entrez2GlobalsPtr  egp;
  MenU               sub;

  egp = (Entrez2GlobalsPtr) GetAppProperty ("Entrez2Globals");
  if (egp == NULL) return;
  egp->parentsPersist = StatusItem (m, "Parents Persist", NULL);
  SetStatus (egp->parentsPersist, egp->persistDefault);
  egp->alignWithChecked = StatusItem (m, "Align Checked Items", NULL);
  SetStatus (egp->alignWithChecked, egp->alignDefault);
  SeparatorItem (m);
  sub = SubMenu (m, "Sequence Complexity");
  egp->seqComplex = ChoiceGroup (sub, NULL);
  ChoiceItem (egp->seqComplex, "Everything");
  ChoiceItem (egp->seqComplex, "NucProt");
  ChoiceItem (egp->seqComplex, "SegSet");
  ChoiceItem (egp->seqComplex, "BioSeq");
  SetValue (egp->seqComplex, 1);
  /*
  SeparatorItem (m);
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
  */
}

/*==================================================================*/
/*                                                                  */
/*  DocSumFontChangeProc () -                                       */
/*                                                                  */
/*==================================================================*/

static void DocSumFontChangeProc (IteM i)

{
  BaseFormPtr        bfp;
  Entrez2GlobalsPtr  egp;
  Uint2              flags;
  FonT               fnt;
  FontSpec           fs;
  Char               str [FONT_NAME_SIZE + 10];

  egp = (Entrez2GlobalsPtr) GetAppProperty ("Entrez2Globals");

  bfp = GetObjectExtra (i);
#ifdef WIN_MAC
  if (bfp == NULL) {
    bfp = currentFormDataPtr;
  }
#endif
  if (bfp != NULL) {
    MemSet ((Pointer) (&fs), 0, sizeof (FontSpec));
    fnt = programFont;
    if (egp != NULL && egp->docsumFont != NULL)
      fnt = egp->docsumFont;

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
      if ((fs.style & STYLE_BOLD) != 0 || (fs.style & STYLE_ITALIC) != 0 || (fs.style & STYLE_UNDERLINE) != 0) {
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
      if (egp != NULL)
        egp->docsumFont = CreateFont (&fs);
      SendMessageToForm (bfp->form, VIB_MSG_REDRAW);
    }
  }
}

/*==================================================================*/
/*                                                                  */
/*  DisplayFontChangeProc () -                                      */
/*                                                                  */
/*==================================================================*/

static void DisplayFontChangeProc (IteM i)

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

  bfp = GetObjectExtra (i);
#ifdef WIN_MAC
  if (bfp == NULL) {
    bfp = currentFormDataPtr;
  }
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
      if ((fs.style & STYLE_BOLD) != 0 || (fs.style & STYLE_ITALIC) != 0 || (fs.style & STYLE_UNDERLINE) != 0) {
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

/*==================================================================*/
/*                                                                  */
/*  DuplicateViewProc () -                                          */
/*                                                                  */
/*==================================================================*/

static void DuplicateViewProc (IteM i)

{
  BaseFormPtr  bfp;
  Int2         handled;
  Uint2        itemID;

  bfp = GetObjectExtra (i);
#ifdef WIN_MAC
  if (bfp == NULL) {
    bfp = currentFormDataPtr;
  }
#endif
  if (bfp == NULL) return;
  if (bfp->input_itemtype == OBJ_BIOSEQ) {
    WatchCursor ();
    itemID = bfp->input_itemID;
    if (itemID == 0) {
      itemID = 1;
    }
    handled = GatherProcLaunch (OMPROC_VIEW, FALSE, bfp->input_entityID, itemID, OBJ_BIOSEQ, 0, 0, OBJ_BIOSEQ, 0);
    ArrowCursor ();
    if (handled != OM_MSG_RET_DONE || handled == OM_MSG_RET_NOPROC) {
      Message (MSG_ERROR, "Unable to launch additional viewer.");
    }
  } else if (bfp->input_itemtype == OBJ_MEDLINE_ENTRY) {
    WatchCursor ();
    itemID = bfp->input_itemID;
    if (itemID == 0) {
      itemID = 1;
    }
    handled = GatherProcLaunch (OMPROC_VIEW, FALSE, bfp->input_entityID, itemID, OBJ_MEDLINE_ENTRY, 0, 0, OBJ_MEDLINE_ENTRY, 0);
    ArrowCursor ();
    if (handled != OM_MSG_RET_DONE || handled == OM_MSG_RET_NOPROC) {
      Message (MSG_ERROR, "Unable to launch additional viewer.");
    }
  }
}

/*==================================================================*/
/*                                                                  */
/*  CloseProc () -                                                  */
/*                                                                  */
/*==================================================================*/

static void CloseProc (BaseFormPtr bfp)

{
  Int2                j;
  Int2                num;
  Boolean             numview;
  ObjMgrPtr           omp;
  ObjMgrDataPtr PNTR  omdpp;
  OMUserDataPtr       omudp;
  ObjMgrDataPtr       tmp;

  if (bfp != NULL) {
    omp = ObjMgrGet ();
    num = omp->currobj;
    for (j = 0, omdpp = omp->datalist; j < num && omdpp != NULL; j++, omdpp++) {
      tmp = *omdpp;
      if (tmp->parentptr == NULL && tmp->dirty && tmp->EntityID == bfp->input_entityID) {
        numview = 0;
        for (omudp = tmp->userdata; omudp != NULL; omudp = omudp->next) {
          if (omudp->proctype == OMPROC_VIEW)
            numview++;
        }
        if (numview < 2)
          if (Message (MSG_OKC, "Closing the window will lose unsaved data.")
              != ANS_OK)
            return;
      }
    }
    numview = 0;
    for (j = 0, omdpp = omp->datalist; j < num && omdpp != NULL; j++, omdpp++) {
      tmp = *omdpp;
      if (tmp->parentptr == NULL)
        for (omudp = tmp->userdata; omudp != NULL; omudp = omudp->next)
          if (omudp->proctype == OMPROC_VIEW)
            numview++;
    }
    Remove (bfp->form);
    if (numview <= 1) {
      /*
         WatchCursor ();
         Update ();
         Message (MSG_OK, "No more viewers present.");
         ArrowCursor ();
         Update ();
       */
    }
  }
}

/*==================================================================*/
/*                                                                  */
/*  MedlineViewFormMessage_Callback () -                            */
/*                                                                  */
/*==================================================================*/

static void MedlineViewFormMessage_Callback (ForM f, Int2 mssg)

{
  BaseFormPtr  bfp;

  bfp = (BaseFormPtr) GetObjectExtra (f);
  if (bfp != NULL) {
    switch (mssg) {
      case VIB_MSG_CLOSE:
        Remove (f);
        break;
      default:
        break;
    }
  }
}

/*==================================================================*/
/*                                                                  */
/*  BioseqViewFormMessage_Callback () -                             */
/*                                                                  */
/*==================================================================*/

static void BioseqViewFormMessage_Callback (ForM f, Int2 mssg)

{
  BaseFormPtr  bfp;

  bfp = (BaseFormPtr) GetObjectExtra (f);
  if (bfp != NULL) {
    switch (mssg) {
      case VIB_MSG_CLOSE:
        CloseProc (bfp);
        break;
      default:
        break;
    }
  }
}

/*==================================================================*/
/*                                                                  */
/*  MedlineViewFormMenus_Callback () -                              */
/*                                                                  */
/*==================================================================*/

static void MedlineViewFormMenus_Callback (WindoW w)

{
  BaseFormPtr  bfp;
  IteM         i = NULL;
  MenU         m;

  bfp = (BaseFormPtr) GetObjectExtra (w);
  if (bfp != NULL) {
    m = PulldownMenu (w, "File");
    FormCommandItem (m, "Close", bfp, VIB_MSG_CLOSE);
    SeparatorItem (m);
    i = CommandItem (m, "Duplicate View", DuplicateViewProc);
    SetObjectExtra (i, bfp, NULL);
    SeparatorItem (m);
    FormCommandItem (m, "Export...", bfp, VIB_MSG_EXPORT);
    SeparatorItem (m);
    FormCommandItem (m, "Print...", bfp, VIB_MSG_PRINT);

    m = PulldownMenu (w, "Edit");
    FormCommandItem (m, COPY_MENU_ITEM, bfp, VIB_MSG_COPY);
  }
}

/*==================================================================*/
/*                                                                  */
/*  BioseqViewFormMenus_Callback () -                               */
/*                                                                  */
/*==================================================================*/

static void BioseqViewFormMenus_Callback (WindoW w)

{
  BaseFormPtr  bfp;
  IteM         i = NULL;
  MenU         m;
  MenU         sub;

  bfp = (BaseFormPtr) GetObjectExtra (w);
  if (bfp != NULL) {
    m = PulldownMenu (w, "File");
    FormCommandItem (m, "Close", bfp, VIB_MSG_CLOSE);
    SeparatorItem (m);
    i = CommandItem (m, "Duplicate View", DuplicateViewProc);
    SetObjectExtra (i, bfp, NULL);
    SeparatorItem (m);
    FormCommandItem (m, "Export...", bfp, VIB_MSG_EXPORT);
    SeparatorItem (m);
    FormCommandItem (m, "Print...", bfp, VIB_MSG_PRINT);

    m = PulldownMenu (w, "Edit");
    FormCommandItem (m, COPY_MENU_ITEM, bfp, VIB_MSG_COPY);

    m = PulldownMenu (w, "Misc");
    sub = SubMenu (m, "Font Selection");
    i = CommandItem (sub, "Display Font...", DisplayFontChangeProc);
    SetObjectExtra (i, bfp, NULL);
    SeparatorItem (m);
    CreateLegendItem (m, bfp);
  }
}

/*==================================================================*/
/*                                                                  */
/*  SetupMenus () - Create and initialize the pulldown menus.       */
/*                                                                  */
/*==================================================================*/

static void SetupMenus (WindoW w)

{
  BaseFormPtr  bfp;
  ChoicE       c;
  IteM         i;
  MenU         m;
  MenU         sub;

  bfp = (BaseFormPtr) GetObjectExtra (w);
  if (bfp != NULL) {
    m = PulldownMenu (w, "File");
    FormCommandItem (m, "Close", bfp, VIB_MSG_CLOSE);
    SeparatorItem (m);
    FormCommandItem (m, "Import...", bfp, VIB_MSG_IMPORT);
    FormCommandItem (m, "Export...", bfp, VIB_MSG_EXPORT);
    SeparatorItem (m);
    FormCommandItem (m, "Print...", bfp, VIB_MSG_PRINT);

    m = PulldownMenu (w, "Edit");
    FormCommandItem (m, COPY_MENU_ITEM, bfp, VIB_MSG_COPY);

    m = PulldownMenu (w, "Options");
    sub = SubMenu (m, "Neighbor Policy");

    c = CreateNeighborDelayChoice (sub, bfp);
    SeparatorItem (m);
    LoadDocsumOptionsMenu (m);

    m = PulldownMenu (w, "Misc");
    sub = SubMenu (m, "Font Selection");
    i = CommandItem (sub, "DocSum Font...", DocSumFontChangeProc);
    SetObjectExtra (i, bfp, NULL);
    i = CommandItem (sub, "Display Font...", DisplayFontChangeProc);
    SetObjectExtra (i, bfp, NULL);
  }
}

/*==================================================================*/
/*                                                                  */
/*  CreateDocsumForm () -                                           */
/*                                                                  */
/*==================================================================*/

NLM_EXTERN ForM CreateDocsumForm (
  Int2 left,
  Int2 top,
  CharPtr title,
  WndActnProc activate,
  FormMessageFunc messages,
  E2NamedUidListProc refineUidCallback,
  Boolean delayedNeighbor
)

{
  ButtoN               b1, b2, b3;
  Entrez2GlobalsPtr    egp;
  Entrez2InfoPtr       e2ip;
  Entrez2DbInfoPtr     e2db;
  GrouP                g;
  GrouP                h;
  Int2                 j;
  GrouP                k;
  CharPtr PNTR         labels = defaultRadios;
  Boolean              macLike;
  PrompT               queryPrompt, launchPrompt, ppt1, ppt2;
  GrouP                q;
  Int2                 quarter;
  RecT                 r;
  Int2                 remaining;
  SummFormPtr          sfp;
  Int2                 typ_ml;
  WindoW               w = NULL;
  GrouP                x;
  GrouP                y;
  Int2                 dbKey;
  SeqViewProcsPtr      svpp;
  MedlineViewProcsPtr  mvpp;

  /*---------------------------------------*/
  /* Make connection to the Entrez2 server */
  /*---------------------------------------*/

  e2ip = Query_GetInfo ();
  if (e2ip == NULL) return NULL;

  /*----------------------------*/
  /* Get global config settings */
  /*----------------------------*/

  egp = (Entrez2GlobalsPtr) GetAppProperty ("Entrez2Globals");
  if (egp == NULL) return NULL;
  macLike = egp->popdownBehavior;

  /*----------------------------------*/
  /* Set up callbacks for SeqView and */
  /* MedlineView menus.               */
  /*----------------------------------*/

  svpp = (SeqViewProcsPtr) GetAppProperty ("SeqDisplayForm");
  /* create menus in window now also on Mac */
  svpp->createMenus = BioseqViewFormMenus_Callback;
  svpp->handleMessages = BioseqViewFormMessage_Callback;

  mvpp = (MedlineViewProcsPtr) GetAppProperty ("MedlineDisplayForm");
  /* create menus in window now also on Mac */
  mvpp->createMenus = MedlineViewFormMenus_Callback;
  mvpp->handleMessages = MedlineViewFormMessage_Callback;

  /*----------------------------------*/
  /* Create a data structure to store */
  /* data that we want to associate   */
  /* with the Document Summary window */
  /*----------------------------------*/

  sfp = (SummFormPtr) MemNew (sizeof (SummFormData));
  if (sfp == NULL) return (ForM) w;

  /*------------------------------------*/
  /* Create the document summary window */
  /*------------------------------------*/

  w = DocumentWindow (left, top, -10, -10, title, StdSendCloseWindowMessageProc, ResizeDocSumForm);

  /*-------------------------------*/
  /* Set window callback functions */
  /* and misc attributes.          */
  /*-------------------------------*/

  SetObjectExtra (w, sfp, CleanupEntrezDocSumForm);
  sfp->form = (ForM) w;
  sfp->formmessage = EntrezDocSumFormMessage_Callback;
  sfp->appmessage = messages;

  sfp->activate = activate;
  SetActivate (w, EntrezDocSumFormActivate_Callback);

  sfp->docsumFont = egp->docsumFont;

  if (sfp->docsumFont == NULL)
    sfp->docsumFont = programFont;

  sfp->refineUidProc = refineUidCallback;
  sfp->usingDelay = delayedNeighbor;

  /*-----------------------------------*/
  /* Set up the document summary menus */
  /*-----------------------------------*/

  SetupMenus (w);

  /*---------------------------------*/
  /* Set up the Format radio buttons */
  /*---------------------------------*/

  for (j = 0; j < MAX_DBS; j++) {
    sfp->formatPopups [j] = NULL;
    sfp->launchPopups [j] = NULL;
  }

  h = HiddenGroup (w, -1, 0, NULL);
  k = HiddenGroup (h, 4, 0, NULL);
  queryPrompt = StaticPrompt (k, "Format:", 0, 0, programFont, 'l');
  q = HiddenGroup (k, 0, 0, NULL);

  for (e2db = e2ip->db_info; e2db != NULL; e2db = e2db->next) {
    if (StringICmp (e2db->db_name, "PubMed") == 0)
      labels = medRadios;
    else if (StringICmp (e2db->db_name, "Protein") == 0)
      labels = prtRadios;
    else if (StringICmp (e2db->db_name, "Nucleotide") == 0)
      labels = nucRadios;
    else if (StringICmp (e2db->db_name, "Structure") == 0)
      labels = strucRadios;
    else if (StringICmp (e2db->db_name, "Genome") == 0)
      labels = genRadios;
    else if (StringICmp (e2db->db_name, "Popset") == 0)
      labels = popsetRadios;
    else if (StringICmp (e2db->db_name, "OMIM") == 0)
      labels = omimRadios;
    else if (StringICmp (e2db->db_name, "Taxonomy") == 0)
      labels = taxonomyRadios;
    else if (StringICmp (e2db->db_name, "Books") == 0)
      labels = bookRadios;
    else if (StringICmp (e2db->db_name, "geo") == 0)
      labels = probeRadios;
    else if (StringICmp (e2db->db_name, "domains") == 0)
      labels = domainRadios;
    else if (StringICmp (e2db->db_name, "unists") == 0)
      labels = stsRadios;
    else if (StringICmp (e2db->db_name, "cdd") == 0)
      labels = cddRadios;
    else if (StringICmp (e2db->db_name, "snp") == 0)
      labels = snpRadios;
    else if (StringICmp (e2db->db_name, "journals") == 0)
      labels = journalsRadios;
    else if (StringICmp (e2db->db_name, "unigene") == 0)
      labels = unigeneRadios;
    else if (StringICmp (e2db->db_name, "PMC") == 0)
      labels = pmcRadios;
    else if (StringICmp (e2db->db_name, "ncbisearch") == 0)
      labels = ncbisearchRadios;
    else if (StringICmp (e2db->db_name, "mesh") == 0)
      labels = meshRadios;
    else if (StringICmp (e2db->db_name, "gene") == 0)
      labels = geneRadios;
    else
      labels = defaultRadios;

    dbKey = DBGetIDFromName (e2db->db_name);

    sfp->formatPopups [dbKey] = PopupList (q, TRUE, ChangeFormat);
    SetObjectExtra (sfp->formatPopups [dbKey], sfp, NULL);
    for (j = 0; labels [j] != NULL; j++)
      PopupItem (sfp->formatPopups [dbKey], labels [j]);
    SetValue (sfp->formatPopups [dbKey], 1);
    Hide (sfp->formatPopups [dbKey]);
  }

  launchPrompt = StaticPrompt (k, "Double Click:", 0, 0, programFont, 'l');
  q = HiddenGroup (k, 0, 0, NULL);


  for (e2db = e2ip->db_info; e2db != NULL; e2db = e2db->next) {
    if (StringICmp (e2db->db_name, "PubMed") == 0)
      labels = medLaunch;
    else if (StringICmp (e2db->db_name, "Protein") == 0)
      labels = prtLaunch;
    else if (StringICmp (e2db->db_name, "Nucleotide") == 0)
      labels = nucLaunch;
    else if (StringICmp (e2db->db_name, "Structure") == 0)
      labels = strucLaunch;
    else if (StringICmp (e2db->db_name, "Genome") == 0)
      labels = genLaunch;
    else if (StringICmp (e2db->db_name, "Popset") == 0)
      labels = popsetLaunch;
    else if (StringICmp (e2db->db_name, "OMIM") == 0)
      labels = omimLaunch;
    else if (StringICmp (e2db->db_name, "Taxonomy") == 0)
      labels = taxonomyLaunch;
    else if (StringICmp (e2db->db_name, "Books") == 0)
      labels = bookLaunch;
    else if (StringICmp (e2db->db_name, "geo") == 0)
      labels = probeLaunch;
    else if (StringICmp (e2db->db_name, "domains") == 0)
      labels = domainLaunch;
    else if (StringICmp (e2db->db_name, "unists") == 0)
      labels = stsLaunch;
    else if (StringICmp (e2db->db_name, "cdd") == 0)
      labels = cddLaunch;
    else if (StringICmp (e2db->db_name, "snp") == 0)
      labels = snpLaunch;
    else if (StringICmp (e2db->db_name, "journals") == 0)
      labels = journalsLaunch;
    else if (StringICmp (e2db->db_name, "unigene") == 0)
      labels = unigeneLaunch;
    else if (StringICmp (e2db->db_name, "PMC") == 0)
      labels = pmcLaunch;
    else if (StringICmp (e2db->db_name, "ncbisearch") == 0)
      labels = ncbisearchLaunch;
    else if (StringICmp (e2db->db_name, "mesh") == 0)
      labels = meshLaunch;
    else if (StringICmp (e2db->db_name, "gene") == 0)
      labels = geneLaunch;
    else
      labels = defaultLaunch;

    dbKey = DBGetIDFromName (e2db->db_name);

    sfp->launchPopups [dbKey] = PopupList (q, TRUE, NULL);
    SetObjectExtra (sfp->launchPopups [dbKey], sfp, NULL);
    for (j = 0; labels [j] != NULL; j++) {
      PopupItem (sfp->launchPopups [dbKey], labels [j]);
    }
    SetValue (sfp->launchPopups [dbKey], 1);
    Hide (sfp->launchPopups [dbKey]);
  }

  sfp->label = medRadios [0];
  AlignObjects (ALIGN_MIDDLE, (HANDLE) queryPrompt, (HANDLE) launchPrompt, (HANDLE) q, NULL);
  g = HiddenGroup (h, -1, 0, NULL);
  SetGroupSpacing (g, 5, 5);

  /*---------------------------*/
  /* Set up the document panel */
  /*---------------------------*/

  sfp->docsum = DocumentPanel (g, 33 * stdCharWidth, 20 * stdLineHeight);
  SetObjectExtra (sfp->docsum, sfp, NULL);
  SetDocProcs (sfp->docsum, ClickDocSum, NULL, ReleaseDocSum, NULL);
  SetDocCache (sfp->docsum, NULL, NULL, NULL);
  Hide (sfp->docsum);

  sfp->controls = HiddenGroup (g, 5, 0, NULL);
  SetGroupSpacing (sfp->controls, 10, 10);

  /*----------------------------------------*/
  /* Create the Neighbor and Refine buttons */
  /*----------------------------------------*/

  x = HiddenGroup (sfp->controls, 0, 2, NULL);
  sfp->retrieve = PushButton (x, "Neighbor 00000", RetrieveNeighbors);
  SetObjectExtra (sfp->retrieve, sfp, NULL);
  SetTitle (sfp->retrieve, "Neighbor 0");
  Disable (sfp->retrieve);
  sfp->refine = PushButton (x, "Refine 0000", RefineProc);
  SetObjectExtra (sfp->refine, sfp, NULL);
  SetTitle (sfp->refine, "Refine");
  Disable (sfp->refine);
  sfp->retrieveMode = FETCH_MODE;

  sfp->neighbors = NULL;
  sfp->numNeighbors = 0;

  sfp->historyFile = NULL;
  sfp->historyOffsets = NULL;
  sfp->generations = 0;
  sfp->present = 0;

  x = HiddenGroup (sfp->controls, 0, -3, NULL);
  SetGroupMargins (x, 3, 0);
  SetGroupSpacing (x, 3, 5);

  /*-------------------------------------*/
  /* Create a pulldown list of databases */
  /*-------------------------------------*/

  sfp->dbalist = CreateDatabaseAlist (e2ip);

  y = HiddenGroup (x, 5, 0, NULL);
  ppt1 = StaticPrompt (y, "Target:", 0, popupMenuHeight, programFont, 'l');
  sfp->target = PopupList (y, macLike, ChangeTarget);
  SetObjectExtra (sfp->target, sfp, NULL);
  InitEnumPopup (sfp->target, sfp->dbalist, NULL);

  typ_ml = DBGetIDFromName ("PubMed");
  SetEnumPopup (sfp->target, sfp->dbalist, (UIEnum) typ_ml);

  /*--------------------------*/
  /* Create selection buttons */
  /*--------------------------*/

  y = HiddenGroup (x, 4, 0, NULL);
  SetGroupSpacing (y, 8, 0);
  ppt2 = StaticPrompt (y, "Select:", 0, 0, programFont, 'l');

  b1 = PushButton (y, "All", SelectAllProc);
  SetObjectExtra (b1, sfp, NULL);
  b2 = PushButton (y, "None", ClearAllProc);
  SetObjectExtra (b2, sfp, NULL);
  b3 = PushButton (y, "Parents", SelectParentsProc);
  SetObjectExtra (b3, sfp, NULL);

  /*----------------------------------*/
  /* Set up Previous and Next buttons */
  /*----------------------------------*/

  x = HiddenGroup (sfp->controls, 0, 2, NULL);

  sfp->prevBtn = PushButton (x, "Prev", PrevButton_Callback);
  SetObjectExtra (sfp->prevBtn, sfp, NULL);
  Disable (sfp->prevBtn);
  sfp->nextBtn = PushButton (x, "Next", NextButton_Callback);
  SetObjectExtra (sfp->nextBtn, sfp, NULL);
  Disable (sfp->nextBtn);

  /*------------------------------------------*/
  /* Align the objects properly in the window */
  /*------------------------------------------*/

  AlignObjects (ALIGN_MIDDLE, (HANDLE) sfp->retrieve, (HANDLE) ppt1, (HANDLE) sfp->target, (HANDLE) sfp->prevBtn, NULL);
  AlignObjects (ALIGN_MIDDLE, (HANDLE) sfp->refine, (HANDLE) ppt2, (HANDLE) b1, (HANDLE) b2, (HANDLE) b3, (HANDLE) sfp->nextBtn, NULL);

  /*---------------------------------*/
  /* Calculate the size of the icons */
  /*---------------------------------*/

  RealizeWindow (w);

  SelectFont (programFont);
  sfp->lineHeight = LineHeight ();
  SelectFont (systemFont);
  sfp->linesPerIcon = (22 + sfp->lineHeight - 1) / sfp->lineHeight;
  docsumParFmt.minLines = sfp->linesPerIcon;

  /*---------------*/
  /*---------------*/

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

  /*---------------------*/
  /* Return successfully */
  /*---------------------*/

  return (ForM) w;
}


/* NetConfig copied from old netcnfg.c file */

typedef struct netconfigdata {
  FORM_MESSAGE_BLOCK

  GrouP           srvConnMode;
  GrouP           netGroup;

  TexT            proxyHost;
  TexT            proxyPort;
  TexT            firewallProxy;

  Char            proxyValue [256];
  Char            portValue [16];
  Char            firewallValue [256];

  ButtoN          dnsAvailable;
  PopuP           timeOut;

  VoidProc        accepted;
  VoidProc        cancelled;
  VoidProc        turnedOff;

  ButtoN          accept;
} NetConfigData, PNTR NetConfigPtr;

static void ConfigMessageProc (ForM f, Int2 mssg)

{
  VoidProc        cancelled;
  NetConfigPtr    ncp;

  ncp = (NetConfigPtr) GetObjectExtra (f);
  if (ncp != NULL) {
    switch (mssg) {
      case VIB_MSG_CLOSE:
        cancelled = ncp->cancelled;
        Remove (f);
        if (cancelled != NULL) {
          cancelled ();
        }
        break;
      case VIB_MSG_CUT:
        StdCutTextProc (NULL);
        break;
      case VIB_MSG_COPY:
        StdCopyTextProc (NULL);
        break;
      case VIB_MSG_PASTE:
        StdPasteTextProc (NULL);
        break;
      case VIB_MSG_DELETE:
        StdDeleteTextProc (NULL);
        break;
      default:
        if (ncp->appmessage != NULL) {
          ncp->appmessage (f, mssg);
        }
        break;
    }
  }
}

static void ConfigFormActivate (WindoW w)

{
  NetConfigPtr    ncp;

  ncp = (NetConfigPtr) GetObjectExtra (w);
  if (ncp != NULL) {
    if (ncp->activate != NULL) {
      ncp->activate (w);
    }
  }
}

static Boolean NoEntryExists (CharPtr type)

{
  Char  str [256];

  if (GetAppParam ("NCBI", "NET_SERV", type, NULL, str, sizeof (str))) {
    if (! StringHasNoText (str)) return FALSE;
  }
  return TRUE;
}

static void AcceptNetConfigForm (ButtoN b)

{
  VoidProc      accepted;
  NetConfigPtr  ncp;
  Char          str [256];
  VoidProc      turnedOff;
  Int2          val;

  ncp = (NetConfigPtr) GetObjectExtra (b);
  if (ncp == NULL) return;

  val = GetValue (ncp->srvConnMode);
  if (val == 1) {
    turnedOff = ncp->turnedOff;
    Remove (ncp->form);
    if (turnedOff != NULL) {
      turnedOff ();
    }
    Update ();
    return;
  } else if (val == 3) {
    SetAppParam ("NCBI", "NET_SERV", "SRV_CONN_MODE", "SERVICE");
    SetAppParam ("NCBI", "NET_SERV", "FIREWALL", "TRUE");
    GetTitle (ncp->proxyHost, str, sizeof (str));
    if (! StringHasNoText (str)) {
      SetAppParam ("NCBI", "NET_SERV", "SRV_HTTP_PROXY_HOST", str);
      GetTitle (ncp->proxyPort, str, sizeof (str));
      if (StringICmp (str, "80") == 0) {
        str [0] = '\0';
      }
      if (! StringHasNoText (str)) {
        SetAppParam ("NCBI", "NET_SERV", "SRV_HTTP_PROXY_PORT", str);
      } else {
        SetAppParam ("NCBI", "NET_SERV", "SRV_HTTP_PROXY_PORT", NULL);
      }
    } else {
      SetAppParam ("NCBI", "NET_SERV", "SRV_HTTP_PROXY_HOST", NULL);
      SetAppParam ("NCBI", "NET_SERV", "SRV_HTTP_PROXY_PORT", NULL);
    }
    GetTitle (ncp->firewallProxy, str, sizeof (str));
    if (! StringHasNoText (str)) {
      SetAppParam ("NCBI", "NET_SERV", "SRV_PROXY_HOST", str);
    } else {
      SetAppParam ("NCBI", "NET_SERV", "SRV_PROXY_HOST", NULL);
    }
  } else {
    SetAppParam ("NCBI", "NET_SERV", "SRV_CONN_MODE", NULL);
    SetAppParam ("NCBI", "NET_SERV", "FIREWALL", NULL);
    SetAppParam ("NCBI", "NET_SERV", "SRV_HTTP_PROXY_HOST", NULL);
    SetAppParam ("NCBI", "NET_SERV", "SRV_HTTP_PROXY_PORT", NULL);
    SetAppParam ("NCBI", "NET_SERV", "SRV_PROXY_HOST", NULL);
  }

  if (GetStatus (ncp->dnsAvailable)) {
    SetAppParam ("NCBI", "NET_SERV", "SRV_ENGINE_HOST", NULL);
  } else {
    SetAppParam ("NCBI", "NET_SERV", "SRV_ENGINE_HOST", "130.14.22.106");
  }

  switch (GetValue (ncp->timeOut)) {
    case 1 :
      SetAppParam ("NCBI", "NET_SERV", "SRV_CONN_TIMEOUT", "10");
      break;
    case 3 :
      SetAppParam ("NCBI", "NET_SERV", "SRV_CONN_TIMEOUT", "60");
      break;
    case 4 :
      SetAppParam ("NCBI", "NET_SERV", "SRV_CONN_TIMEOUT", "300");
      break;
    default :
      SetAppParam ("NCBI", "NET_SERV", "SRV_CONN_TIMEOUT", NULL);
      break;
  }

  if (GetAppParam ("NCBI", "NET_SERV", "SRV_ENGINE_PORT", NULL, str, sizeof (str))) {
    if (StringICmp (str, "80") == 0) {
      SetAppParam ("NCBI", "NET_SERV", "SRV_ENGINE_PORT", NULL);
    }
  }

  if (GetAppParam ("NCBI", "NET_SERV", "SRV_ENGINE_URL", NULL, str, sizeof (str))) {
    if (StringICmp (str, "/Service/nph-dispd.cgi") == 0) {
      SetAppParam ("NCBI", "NET_SERV", "SRV_ENGINE_URL", NULL);
    }
  }

  if (NoEntryExists ("SRV_CONN_MODE") &&
      NoEntryExists ("FIREWALL") &&
      NoEntryExists ("SRV_CONN_TIMEOUT") &&
      NoEntryExists ("SRV_ENGINE_HOST") &&
      NoEntryExists ("SRV_ENGINE_PORT") &&
      NoEntryExists ("SRV_ENGINE_URL") &&
      NoEntryExists ("SRV_HTTP_PROXY_HOST") &&
      NoEntryExists ("SRV_HTTP_PROXY_PORT") &&
      NoEntryExists ("SRV_PROXY_HOST")) {
    SetAppParam ("NCBI", "NET_SERV", NULL, NULL);
  }

  accepted = ncp->accepted;
  Remove (ncp->form);
  if (accepted != NULL) {
    accepted ();
  }
  Update ();
}

static void ChangeConfigControls (NetConfigPtr ncp)

{
  Boolean  hasProxy;
  Int2     val;

  if (ncp == NULL) return;
  val = GetValue (ncp->srvConnMode);
  if (val == 1) {
    SafeHide (ncp->netGroup);
  }
  if (val == 3) {
    if (! Enabled (ncp->proxyHost)) {
      SafeSetTitle (ncp->proxyHost, ncp->proxyValue);
    }
    Enable (ncp->proxyHost);
    if (! Enabled (ncp->firewallProxy)) {
      SafeSetTitle (ncp->firewallProxy, ncp->firewallValue);
    }
    Enable (ncp->firewallProxy);
  } else {
    if (Enabled (ncp->proxyHost)) {
      GetTitle (ncp->proxyHost, ncp->proxyValue, sizeof (ncp->proxyValue));
      SafeSetTitle (ncp->proxyHost, NULL);
    }
    Disable (ncp->proxyHost);
    if (Enabled (ncp->firewallProxy)) {
      GetTitle (ncp->firewallProxy, ncp->firewallValue, sizeof (ncp->firewallValue));
      SafeSetTitle (ncp->firewallProxy, NULL);
    }
    Disable (ncp->firewallProxy);
  }
  hasProxy = (Boolean) (! TextHasNoText (ncp->proxyHost));
  if (hasProxy) {
    if (! Enabled (ncp->proxyHost)) {
      SafeSetTitle (ncp->proxyHost, ncp->proxyValue);
    }
    Enable (ncp->proxyHost);
    if (! Enabled (ncp->proxyPort)) {
      SafeSetTitle (ncp->proxyPort, ncp->portValue);
    }
    Enable (ncp->proxyPort);
  } else {
    if (Enabled (ncp->proxyPort)) {
      GetTitle (ncp->proxyPort, ncp->portValue, sizeof (ncp->portValue));
      SafeSetTitle (ncp->proxyPort, NULL);
    }
    Disable (ncp->proxyPort);
  }
  if (val != 1) {
    SafeShow (ncp->netGroup);
  }
}

static void ChangeConnection (GrouP g)

{
  NetConfigPtr  ncp;

  ncp = (NetConfigPtr) GetObjectExtra (g);
  ChangeConfigControls (ncp);
}

static void ChangeProxy (TexT t)

{
  NetConfigPtr  ncp;

  ncp = (NetConfigPtr) GetObjectExtra (t);
  ChangeConfigControls (ncp);
}

NLM_EXTERN void ShowNewNetConfigForm (
  WndActnProc activate,
  FormMessageFunc messages,
  VoidProc accepted,
  VoidProc cancelled,
  VoidProc turnedOff,
  Boolean netCurrentlyOn
)

{
  ButtoN        b;
  GrouP         c;
  GrouP         g;
  GrouP         h;
  GrouP         j;
  Int2          len;
  NetConfigPtr  ncp;
  PrompT        ppt0, ppt1;
  ButtoN        rb;
  Char          str [256];
  WindoW        w;
  GrouP         x;
  GrouP         z;

  w = NULL;
  ncp = (NetConfigPtr) MemNew (sizeof (NetConfigData));
  if (ncp != NULL) {

    w = FixedWindow (-50, -33, -10, -10, "Network Configuration",
                     StdSendCloseWindowMessageProc);
    SetObjectExtra (w, ncp, StdCleanupFormProc);
    ncp->form = (ForM) w;
    ncp->formmessage = ConfigMessageProc;

    ncp->appmessage = messages;
    ncp->activate = activate;
    SetActivate (w, ConfigFormActivate);

    ncp->accepted = accepted;
    ncp->cancelled = cancelled;
    ncp->turnedOff = turnedOff;

    h = HiddenGroup (w, -1, 0, NULL);
    SetGroupSpacing (h, 5, 10);

    j = HiddenGroup (h, 2, 0, NULL);
    ppt0 = StaticPrompt (j, "Connection", 0, 0, programFont, 'l');
    ncp->srvConnMode = HiddenGroup (j, 4, 0, ChangeConnection);
    SetObjectExtra (ncp->srvConnMode, ncp, NULL);
    rb = RadioButton (ncp->srvConnMode, "None");
    RadioButton (ncp->srvConnMode, "Normal");
    RadioButton (ncp->srvConnMode, "Firewall");
    /* RadioButton (ncp->srvConnMode, "Stateless"); */
    SetValue (ncp->srvConnMode, 2);
    if (turnedOff == NULL) {
      Disable (rb);
    }
    AlignObjects (ALIGN_MIDDLE, (HANDLE) ppt0, (HANDLE) ncp->srvConnMode, NULL);

    ncp->netGroup = HiddenGroup (h, -1, 0, NULL);
    SetGroupSpacing (ncp->netGroup, 5, 10);
  
    SelectFont (programFont);
    len = StringWidth ("HTTP Proxy Server ") + 2;
    SelectFont (systemFont);

    z = HiddenGroup (ncp->netGroup, -2, 0, NULL);
    StaticPrompt (z, "HTTP Proxy Server", len, dialogTextHeight, programFont, 'l');
    ncp->proxyHost = DialogText (z, "", 12, ChangeProxy);
    SetObjectExtra (ncp->proxyHost, ncp, NULL);
    /*
    StaticPrompt (z, "", 0, 0, programFont, 'l');
    StaticPrompt (z, "", 0, 0, programFont, 'l');
    */
    ppt0 = StaticPrompt (z, "HTTP Proxy Port ", len, dialogTextHeight, programFont, 'l');
    ncp->proxyPort = DialogText (z, "", 3, NULL);
    x = MultiLinePrompt (z, "Non-transparent Proxy Server", len, programFont);
    ncp->firewallProxy = DialogText (z, "", 12, NULL);
    AlignObjects (ALIGN_MIDDLE, (HANDLE) x, (HANDLE) ncp->firewallProxy, NULL);
    /*
    AlignObjects (ALIGN_MIDDLE, (HANDLE) ppt0, (HANDLE) ncp->proxyPort,
                  (HANDLE) ncp->firewallProxy, NULL);
    */
    AlignObjects (ALIGN_RIGHT, (HANDLE) ncp->proxyHost, (HANDLE) ncp->firewallProxy, NULL);
    AlignObjects (ALIGN_LEFT, (HANDLE) ncp->proxyHost, (HANDLE) ncp->firewallProxy, NULL);

    g = HiddenGroup (ncp->netGroup, 5, 0, NULL);
    /*
    ppt0 = StaticPrompt (g, "Domain name server", 0, 0, programFont, 'l');
    ncp->dnsAvailable = CheckBox (g, "Available", NULL);
    */
    ncp->dnsAvailable = CheckBox (g, "Domain Name Server", NULL);
    SetStatus (ncp->dnsAvailable, TRUE);
    /* StaticPrompt (g, " ", 0, 0, programFont, 'l'); */
    ppt1 = StaticPrompt (g, "Timeout", 0, popupMenuHeight, programFont, 'l');
    ncp->timeOut = PopupList (g, TRUE, NULL);
    PopupItem (ncp->timeOut, "10 seconds");
    PopupItem (ncp->timeOut, "30 seconds");
    PopupItem (ncp->timeOut, "60 seconds");
    PopupItem (ncp->timeOut, " 5 minutes");
    SetValue (ncp->timeOut, 2);
    AlignObjects (ALIGN_MIDDLE, /* (HANDLE) ppt0, */ (HANDLE) ncp->dnsAvailable,
                  (HANDLE) ppt1, (HANDLE) ncp->timeOut, NULL);

    c = HiddenGroup (w, 4, 0, NULL);
    SetGroupSpacing (c, 10, 2);
    ncp->accept = PushButton (c, "Accept", AcceptNetConfigForm);
    SetObjectExtra (ncp->accept, ncp, NULL);
    b = PushButton (c, "Cancel", StdSendCancelButtonMessageProc);
    SetObjectExtra (b, ncp, NULL);

    /*
    AlignObjects (ALIGN_CENTER, (HANDLE) h, (HANDLE) c, NULL);
    */
    AlignObjects (ALIGN_CENTER, (HANDLE) j, (HANDLE) z, (HANDLE) g, (HANDLE) c, NULL);

    RealizeWindow (w);

    if (! netCurrentlyOn) {
      SafeSetValue (ncp->srvConnMode, 1);
      Hide (ncp->netGroup);
    } else if (GetAppParam ("NCBI", "NET_SERV", "SRV_CONN_MODE", "WWW", str, sizeof (str))) {
      if (StringICmp (str, "FIREWALL") == 0) {
        SafeSetValue (ncp->srvConnMode, 3);
      } else if (StringICmp (str, "SERVICE") == 0) {
        if (GetAppParam ("NCBI", "NET_SERV", "FIREWALL", "WWW", str, sizeof (str))) {
          if (StringICmp (str, "TRUE") == 0) {
            SafeSetValue (ncp->srvConnMode, 3);
          }
        }
      }
    }
    if (GetAppParam ("NCBI", "NET_SERV", "SRV_HTTP_PROXY_HOST", NULL, str, sizeof (str))) {
      if (! StringHasNoText (str)) {
        SafeSetTitle (ncp->proxyHost, str);
        if (GetValue (ncp->srvConnMode) == 2) {
          SafeSetValue (ncp->srvConnMode, 3);
        }
      }
    }
    if (GetAppParam ("NCBI", "NET_SERV", "SRV_HTTP_PROXY_PORT", NULL, str, sizeof (str))) {
      if (! StringHasNoText (str)) {
        SafeSetTitle (ncp->proxyPort, str);
      }
    }
    if (GetAppParam ("NCBI", "NET_SERV", "SRV_PROXY_HOST", NULL, str, sizeof (str))) {
      if (! StringHasNoText (str)) {
        SafeSetTitle (ncp->firewallProxy, str);
      }
    }
    if (GetAppParam ("NCBI", "NET_SERV", "SRV_ENGINE_HOST", NULL, str, sizeof (str))) {
      if (StringICmp (str, "130.14.22.106") == 0 || StringICmp (str, "130.14.22.107") == 0 ) {
        SafeSetStatus (ncp->dnsAvailable, FALSE);
      }
    }
    if (GetAppParam ("NCBI", "NET_SERV", "SRV_CONN_TIMEOUT", "30", str, sizeof (str))) {
      if (StringICmp (str, "10") == 0) {
        SafeSetValue (ncp->timeOut, 1);
      } else if (StringICmp (str, "60") == 0) {
        SafeSetValue (ncp->timeOut, 3);
      } else if (StringICmp (str, "300") == 0) {
        SafeSetValue (ncp->timeOut, 4);
      } else {
        SafeSetValue (ncp->timeOut, 2);
      }
    }

    ChangeConfigControls (ncp);

    Show (w);
    Select (w);
    Select (ncp->proxyHost);
  }
}

