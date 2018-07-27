/*   diagnost.h
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
* File Name:  diagnost.h
*
* Author:  Alex Smirnov
*
* Version Creation Date:   03/29/95
*
* $Revision: 6.0 $
*
* File Description: 
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
* ==========================================================================
*/

#ifndef _DIAGNOST_
#define _DIAGNOST_

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
*
*   DEFINES
*
*****************************************************************************/
/* Type of record */
#define DA_NONE     0
#define DA_WARNING  1
#define DA_ERROR    2

/*****************************************************************************
*
*   EXTERN FUNCTION
*
*****************************************************************************/
extern Nlm_Uint1    Nlm_DiagGetRecordCount  PROTO((void));
extern Nlm_Boolean  Nlm_DiagHasErrorRec     PROTO((void));
extern Nlm_Int1     Nlm_DiagGetRecordType   PROTO((Nlm_Uint1 recordIndex));
extern Nlm_CharPtr  Nlm_DiagGetRecordStr    PROTO((Nlm_Uint1 recordIndex));

/*****************************************************************************
*
*   NAME REDIFINITION
*
*****************************************************************************/
#define DiagGetRecordCount Nlm_DiagGetRecordCount 
#define DiagHasErrorRec    Nlm_DiagHasErrorRec
#define DiagGetRecordType  Nlm_DiagGetRecordType  
#define DiagGetRecordStr   Nlm_DiagGetRecordStr   

#ifdef __cplusplus
}
#endif

#endif

