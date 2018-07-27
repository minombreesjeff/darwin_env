/*   pdiagnos.h
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
* File Name:  pdiagnos.h
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

#ifndef _PDIAGNOS_
#define _PDIAGNOS_

#ifndef _DIAGNOST_
#include <diagnost.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
*
*   DEFINES
*
*****************************************************************************/
#define NLM_DDA_MSGLEN  127
#define NLM_DDA_SIZE    16

/*****************************************************************************
*
*   TYPEDEFS
*
*****************************************************************************/
typedef struct Nlm_diagrecord {
  Nlm_Int1   recordType;
  Nlm_Char   recordStr[NLM_DDA_MSGLEN];
}Nlm_DiagRecord, PNTR Nlm_DiagRecordPtr;

typedef struct Nlm_diagarea {
  Nlm_Uint4           keyLong;
  Nlm_Uint2           total;
  Nlm_Uint2           hasErrorRec;
  Nlm_DiagRecord      records[NLM_DDA_SIZE];
}Nlm_DiagArea, PNTR Nlm_DiagAreaPtr;

/*****************************************************************************
*
*   EXTERN FUNCTION
*
*****************************************************************************/
extern void     Nlm_DiagReset           PROTO((void));
extern void     Nlm_DiagPutRecord       PROTO((Nlm_Int1   recordType,
                                               Nlm_CharPtr className,
                                               Nlm_CharPtr functionName,
                                               Nlm_CharPtr msg));

#ifdef __cplusplus
}
#endif

#endif

