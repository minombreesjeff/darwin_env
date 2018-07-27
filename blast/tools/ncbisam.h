/* $Id: ncbisam.h,v 6.3 2000/07/18 19:29:27 shavirin Exp $
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
* File Name:  $RCSfile: ncbisam.h,v $
*
* Author:  Sergei Shavirin
*
* Initial Version Creation Date: 02/24/1997
*
* $Revision: 6.3 $
*
* File Description:
*         External include file for ISAM library
*
* $Log: ncbisam.h,v $
* Revision 6.3  2000/07/18 19:29:27  shavirin
* Added new parameter test_non_unique to suppress check for non-unique
* strings ids in the database - default - TRUE.
*
* Revision 6.2  1999/08/25 20:18:50  shavirin
* Added possibility to store user-specified Int4 options in the index
* header.
*
* Revision 6.1  1997/11/07 16:18:06  shavirin
* Added definition for the function SISAMFindAllData()
*
* Revision 6.0  1997/08/25 18:53:30  madden
* Revision changed to 6.0
*
* Revision 1.8  1997/05/12 19:55:20  shavirin
* Some fixes type-changes to support ISAMCreateDatabase() API
*
* Revision 1.6  1997/05/08 21:18:40  shavirin
* Added definition for function ISAMSearchTerm()
*
 * Revision 1.3  1997/05/01  17:25:05  shavirin
 * Added String ISAM functionality
 *
 * Revision 1.2  1997/02/25  22:17:17  shavirin
 * Changed general API of ISAM library .
 *
 * Revision 1.1  1997/02/24  21:07:07  shavirin
 * Initial revision
 *
*
* ==========================================================================
*/

#ifndef _NCBISAM_H_
#define _NCBISAM_H_ ncbisam

/****************************************************************************/
/* INCLUDES */
/****************************************************************************/

#include <ncbi.h>

/****************************************************************************/
/* DEFINES */
/****************************************************************************/

#define DEFAULT_NISAM_SIZE 256
#define DEFAULT_SISAM_SIZE 64
#define ISAM_FOLLOW_KEY 1
#define ISAM_SHORT_KEY  2
#define ISAM_DATA_CHAR '\2'
#define MEMORY_ONLY_PAGE_SIZE 1
/****************************************************************************/
/* TYPEDEFS */
/****************************************************************************/

typedef enum {
  ISAMNumeric         = 0,
  ISAMNumericNoData   = 1,
  ISAMString          = 2,
  ISAMStringDatabase  = 3,
  ISAMStringBin       = 4
} ISAMType;

typedef enum {
  ISAMNotFound        =  1,
  ISAMNoError         =  0,
  ISAMNoSpace         =  -2,
  ISAMBadFileName     =  -3,
  ISAMNotImplemented  =  -4,
  ISAMMemMap          =  -5,
  ISAMNoOrder         =  -6,
  ISAMNoCorrelation   =  -7,
  ISAMBadParameter    =  -8,
  ISAMNoMemory        =  -9,
  ISAMBadVersion      =  -10,
  ISAMBadType         =  -11,
  ISAMWrongFile       =  -12,
  ISAMFseekFailed     =  -13,
  ISAMInvalidFormat   =  -14,
  ISAMMiscError       =  -99
} ISAMErrorCode;

typedef VoidPtr ISAMObjectPtr;

/****************************************************************************/
/* FINCTION DEFINITIONS */
/****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif
/* ----------------------  ISAMObjectNew --------------------------
   Purpose:     Creates ISAM object structure with default parameters
                
   Parameters:  type - Type of ISAM (Numeric, String etc. )
   Returns:     Poiner to created object structure
   NOTE:        None

  ------------------------------------------------------------------*/
ISAMObjectPtr ISAMObjectNew(ISAMType type,       /* Type of ISAM */
                            CharPtr DBFile,      /* ISAM Database file */
                            CharPtr IndexFile    /* ISAM Index file */
                            );

/* ---------------------- ISAMObjectFree --------------------------
   Purpose:     To terminate all allocated and used buffers
                unmap and close all mapped/opened files
   Parameters:  ISAM object
   Returns:     None
   NOTE:        None
  ------------------------------------------------------------------*/
void ISAMObjectFree(ISAMObjectPtr object);

/* ------------------------ ISAMSearchTerm -------------------------
   Purpose:     Main search function of complete String ISAM

   Parameters:  object - ISAM Object
                term_in - input string
                field_mask - fields to search in 0 and -1 mean search
                all fields
                uid - array of returned uids
                count number of returned uids
   Returns:     ISAM Error Code
   NOTE:        Initialization done with first call to this function
  ------------------------------------------------------------------*/
ISAMErrorCode ISAMSearchTerm(ISAMObjectPtr object, CharPtr term_in, 
                             Uint4 field_mask, Uint4Ptr PNTR uid, 
                             Int4Ptr count);

/* ---------------------- ISAMCreateDatabase ------------------------
   Purpose:     To create coded array/offsets and ISAM database files 
                from input files in special form:
                All files are in sorted order and sorted through
                format: <term><\2><uid><field-bit mask><CR>
   
   Parameters:  ISAM Object 
                files - list of sorted files to process
   Returns:     ISAM itemized error code 
  ------------------------------------------------------------------*/
ISAMErrorCode ISAMCreateDatabase(CharPtr PNTR files,
                                 Int4 num_files,
                                 Int4 MaxOffset,
                                 CharPtr BaseName,
                                 CharPtr DBExt,
                                 CharPtr IndexExt,
                                 CharPtr OffExt,
                                 CharPtr CodeExt);

/* ---------------------- ISAMUpdateDatabase ------------------------

   Purpose:               
   
   Parameters:          NOT IMPLEMENTED YET!!!
                
   Returns:     
  ------------------------------------------------------------------*/
ISAMErrorCode ISAMUpdateDatabase(CharPtr InFile,
                                 CharPtr NewDBDir, 
                                 Int4 MaxOffset, 
                                 CharPtr BaseName, 
                                 CharPtr DBExt,
                                 CharPtr IndexExt,
                                 CharPtr OffExt,
                                 CharPtr CodeExt);

/* ----------------------  ISAMSetUpCAInfo --------------------------
   Purpose:     Added toISAM object Coded Array filenames information
                
   Parameters:  CAName - Common name for all CA/FA DB and offset files
                CADBExt - exetntio for CA/FA DB files
                CAOffExt - extention for CA/FA offset files
                MaxOffset - threshhold offset for starting write new
                            file

   Returns:     ISAM Error Code
   NOTE:        MaxOffset is set to default value if 10.000.000
  ------------------------------------------------------------------*/
ISAMErrorCode ISAMSetUpCAInfo(ISAMObjectPtr object, Int4 MaxOffset,
                              CharPtr CAName, CharPtr CADBExt,
                              CharPtr CAOffExt);

/* ----------------------  ISAMMakeIndex --------------------------
   Purpose:     To create ISAM Intex file for Database file

   Parameters:  ISAM Object 
   Returns:     ISAM itemized error code 
   NOTE:        Special default rules for UNIX platform
                Page size is set to default value if 0
  ------------------------------------------------------------------*/
ISAMErrorCode ISAMMakeIndex(ISAMObjectPtr object,
                            Int4 page_size,       /* ISAM page size */
                            Int4 idx_option       /* Option for upper layer */
                            );

/* ------------------------ NISAMSearch ----------------------------
   Purpose:     Main search function of Numeric ISAM

   Parameters:  Key - interer to search
                Data - returned value (for NIASM with data)
                Index - internal index in database
   Returns:     ISAM Error Code
   NOTE:        Initialization done with first call to this function
  ------------------------------------------------------------------*/
ISAMErrorCode NISAMSearch(ISAMObjectPtr object,
                          Uint4 Key, 
                          Uint4Ptr Data,
                          Uint4Ptr Index
                          );

/* ----------------------  NISAMSearchList --------------------------
   Purpose:       Perform search of multiple Keys

   Parameters:    NumKeys - number of input keys
                  Keys - array of keys
   Returns:       Data - array of returned values
                  Index - array of internal indexes
   NOTE:          None
  ------------------------------------------------------------------*/
ISAMErrorCode NISAMSearchList(ISAMObjectPtr object, 
                              Int4     NumKeys, 
                              Uint4Ptr Keys, 
                              Uint4Ptr Data,
                              Uint4Ptr index
                              );


ISAMErrorCode SISAMSearch(ISAMObjectPtr object, 
                          CharPtr key_in,
                          Int4 type,
                          CharPtr PNTR key_out,
                          CharPtr PNTR data,
                          Uint4Ptr index);

ISAMErrorCode SISAMFindAllData(ISAMObjectPtr object, 
                               CharPtr term_in,
                               Int4Ptr PNTR ids_out,
                               Int4Ptr count_out);

/* ------------------------  NISAMFindKey ---------------------------
   Purpose:     Return Key value by absolute internal index

   Parameters:  Index - absolute internal index                
   Returns:     Key   - corresponding key value
                Data  - corresponding data value
   NOTE:        
  ------------------------------------------------------------------*/
ISAMErrorCode NISAMFindKey(ISAMObjectPtr object, 
                           Int4 Index,
                           Uint4Ptr Key,
                           Uint4Ptr Data
                           );

/* ----------------------  NISANFindKeys -------------------------
   Purpose:     Retuns set of Key/Data pairs from 
                First to Last internal index

   Parameters:  First - beginning of interval
                Last  - the end of interval
   Returns:     Keys - array of Keys
                Data - array of Data
   NOTE:        None
  ------------------------------------------------------------------*/
ISAMErrorCode NISAMFindKeys(ISAMObjectPtr object, 
                            Int4     First, 
                            Int4     Last, 
                            Uint4Ptr Keys, 
                            Uint4Ptr Data
                            );
  
/* ------------------------  ISAMNumTerms ---------------------------
   Purpose:     Returns total number of terms in ISAM database

   Parameters:  ISAM object
   Returns:     Number of terms
   NOTE:        None
  ------------------------------------------------------------------*/
ISAMErrorCode ISAMNumTerms(ISAMObjectPtr object, Int4Ptr terms);

/* ------------------------ ISAMGetIdxOption ------------------------
   Purpose:     Returns user specified option from ISAM database

   Parameters:  ISAM object
   Returns:     User specified option (set while formating)
   NOTE:        None
  ------------------------------------------------------------------*/
ISAMErrorCode ISAMGetIdxOption(ISAMObjectPtr object, Int4Ptr idx_option);

/* ------------------------ ISAMGetIdxOption ------------------------
   Purpose:     To set option to check or not check for non-unique
                elements
   Parameters:  ISAM object
   Returns:     None
   NOTE:        None
  ------------------------------------------------------------------*/
void ISAMSetCheckForNonUnique(ISAMObjectPtr object, Boolean test_non_unique);

#ifdef __cplusplus
}
#endif

#endif
