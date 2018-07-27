/* $Id: ncbisami.h,v 6.3 2000/07/18 19:29:28 shavirin Exp $
* ===========================================================================
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
* File Name:  $RCSfile: ncbisami.h,v $
*
* Author:  Sergei Shavirin
*
* Initial Version Creation Date: 02/24/1997
*
* $Revision: 6.3 $
*
* File Description:
*         Internal include file for ISAM library
*
* $Log: ncbisami.h,v $
* Revision 6.3  2000/07/18 19:29:28  shavirin
* Added new parameter test_non_unique to suppress check for non-unique
* strings ids in the database - default - TRUE.
*
* Revision 6.2  1999/08/25 20:19:24  shavirin
* Added parameter for user options to the internal structure.
*
* Revision 6.1  1999/02/19 22:01:59  madden
* Add NlmMFILEPtr to ISAMData typedef
*
* Revision 6.0  1997/08/25 18:53:32  madden
* Revision changed to 6.0
*
* Revision 1.6  1997/05/12 19:55:32  shavirin
* Some fixes type-changes to support ISAMCreateDatabase() API
*
* Revision 1.5  1997/05/07 21:14:18  shavirin
* Added definitions for fields array encoding and ISAMCreateDatabase()
* function.
*
* Revision 1.4  1997/05/06 21:36:50  shavirin
* Added definitions of functions for Codded Array compression
* implementation
*
 * Revision 1.3  1997/05/05  18:17:35  shavirin
 * Added support for platforms without memory mapping
 *
 * Revision 1.2  1997/05/01  17:26:16  shavirin
 * Added String ISAM index functionality
 *
 * Revision 1.1  1997/02/24  21:07:17  shavirin
 * Initial revision
 *
*
* ==========================================================================
*/

#ifndef _NCBISAMI_H_
#define _NCBISAMI_H_ ncbisami

/****************************************************************************/
/* INCLUDES */
/****************************************************************************/

#include <ncbisam.h>

/****************************************************************************/
/* INTERNAL FINCTION DEFINITIONS */
/****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif
/****************************************************************************/
/* DEFINES */
/****************************************************************************/

#define MAX_FILENAME_LEN 256
#define LINE_SIZE_CHUNK  4096
#define BUFF_SIZE_CHUNK  1024
#define UID_NUM_CHUNK    1024

#define ISAM_VERSION 1
#define ISAM_DATA_CHAR '\2'
#define ENDS_ISAM_KEY(Ptr) ((*Ptr == NULLB) || (*Ptr == ISAM_DATA_CHAR) || (*Ptr == '\n') || (*Ptr == '\r'))

#define Log2(N) (log(N)/log(2.0))
#define CA_TMP_CHUNK 4096
#define FA_Mask 0x7F
#define DEFAULT_CA_MAX_OFFSET 10000000

/****************************************************************************/
/* TYPEDEFS */
/****************************************************************************/

typedef struct NISAMKeyData
{
    Uint4 key;
    Uint4 data;
} NISAMKeyData, PNTR NISAMKeyDataPtr;

typedef struct ISAMUidField
{
    Uint4 uid;
    Uint4 field;
} ISAMUidField, PNTR ISAMUidFieldPtr;

typedef struct ISAMData
{
    Int4            type;           /* Type of ISAM index */
  
    CharPtr         DBFileName;     /* Filename of database file */
    CharPtr         IndexFileName;  /* Filename of ISAM index file */

    CharPtr         CAName;         /* Common filename-directory for CA */
    CharPtr         CADBExt;        /* Extention for CA/FA files */
    CharPtr         CAOffExt;       /* Extention for CA-Offset files */
    Int4            CAMaxOffset;    /* Offset for switch CA DB/Offset file */

    Nlm_MemMapPtr   mmp;            /* Memory map pointer to index file */
    NlmMFILEPtr     mfp;            /* Memory map pointer to database file for numeric search. */
    CharPtr         FileStart;      /* Pointer to index file if no memmap */
    Int4            NumTerms;       /* Number of terms in database */
    Int4            NumSamples;     /* Number of terms in ISAM index */
    Int4            PageSize;       /* Page size of ISAM index */
    FILE            *db_fd;         /* File pointer of ISAM database */
    Boolean         initialized;    /* Is this structure was initialized 
                                       for ISAM Search ? */
    Uint4Ptr        KeySamples;     /* Pointer to first sample offset 
                                       in ISAM index */
    NISAMKeyDataPtr KeyDataSamples; /* Pointer to first NISAMKeyData structure
                                       in ISAM index (for search with data) */
    
    CharPtr         line;           /* Temporary buffer to work with strings */
    Int4            max_line_size;  /* Maximum string length in the database */
    Int4            idx_option;    /* Options set by upper layer */
    Boolean       test_non_unique; /* Check if data for String ISAM sorted */
} ISAMData, PNTR ISAMDataPtr;

typedef struct ISAMTmpCA
{
    Uint1Ptr buffer;  /* Buffer with coded array bytes */
    Int4 allocated;   /* Memory size allocated for the buffer */
    Int4 length;      /* Final length of CA buffer */
    Int4 num_uids;    /* Number of coded uids in CA */
    Int4 num_bits;    /* Number of bits used for CA compression */
    Int4 byte_num;    /* Temorary value for into CA buffer */
    Int4 bit_num;     /* Temporary value for CA buffer */
} ISAMTmpCA, PNTR ISAMTmpCAPtr;

static Uint4 PowersOfTwo[] = {01,02,04,010,020,040,0100,0200,0400,01000,
                               02000,04000,010000,020000,040000,0100000,
                               0200000,0400000,01000000,02000000,
                               04000000,010000000,020000000,040000000,
                               0100000000,0200000000,0400000000,
                               01000000000,02000000000,
                               04000000000,010000000000, 020000000000};

static Uint1 OneBit[] = {0x80 , 0x40, 0x20, 0x10, 0x8, 0x4, 0x2, 0x1};

#endif




