/*   diagnost.c
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
* File Name:  diagnost.c
*
* Author:  Alex Smirnov
*
* Version Creation Date:   03/29/95
*
* $Revision: 6.1 $
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

#include <vibrant.h>
#ifndef _PDIAGNOS_
#include <pdiagnos.h>
#endif

/*****************************************************************************
*
*   LOCAL VARIABLE
*
*****************************************************************************/
static Nlm_DiagArea diagArea;

/*****************************************************************************
*
*   EXTERN FUNCTION
*
*****************************************************************************/
void Nlm_DiagReset ( void )
{
  diagArea.total = (Uint2)0;
  diagArea.hasErrorRec = 0;
}

void Nlm_DiagPutRecord ( Int1 recordType,
                         CharPtr className, CharPtr functionName,
                         CharPtr msg )
{
  CharPtr daMsg;
  Int2 i, pos;

  switch ( recordType ) {
    case DA_ERROR:
      diagArea.hasErrorRec = 1;
    case DA_WARNING:
      break;
    default:
      return;
  }
  i = (Int2)diagArea.total;
  if ( i == NLM_DDA_SIZE ) i--;
  diagArea.total = (Uint2)(i+1);
  diagArea.records[i].recordType = recordType;
  daMsg = &(diagArea.records[i].recordStr[0]);
  *daMsg = daMsg[NLM_DDA_MSGLEN - 1 ] = (Char)0;
  pos = 0;
  if ( className != NULL ){
    StringNCpy_0 ( &(diagArea.records[i].recordStr[pos]), className, 
                 NLM_DDA_MSGLEN - pos - 1 );
    pos = (Int2)StringLen ( &(diagArea.records[i].recordStr[0]) );
    if ( pos == (NLM_DDA_MSGLEN - 1) ) return;
    StringCpy ( &(diagArea.records[i].recordStr[pos]), ":");
    pos++;
  }
  if ( (functionName != NULL) && (pos < (NLM_DDA_MSGLEN-1)) ){
    StringNCpy_0 ( &(diagArea.records[i].recordStr[pos]), functionName, 
                 NLM_DDA_MSGLEN - pos - 1 );
    pos = (Int2)StringLen ( &(diagArea.records[i].recordStr[0]) );
    if ( pos == (NLM_DDA_MSGLEN - 1) ) return;
    StringCpy ( &(diagArea.records[i].recordStr[pos]), ":");
    pos++;
  }
  if ( (msg != NULL) && (pos < (NLM_DDA_MSGLEN-1)) ){
    StringNCpy_0 ( &(diagArea.records[i].recordStr[pos]), msg, 
                 NLM_DDA_MSGLEN - pos - 1 );
  }
}

Uint1 Nlm_DiagGetRecordCount ( void )
{
  return (Uint1)(diagArea.total);
}

Boolean Nlm_DiagHasErrorRec ( void )
{
  if ( diagArea.hasErrorRec ) return TRUE;
  return FALSE;
}

Int1 Nlm_DiagGetRecordType ( Uint1 recordIndex )
{
  if ( recordIndex < (Uint1)(diagArea.total) ){
    return diagArea.records[recordIndex].recordType;
  } 
  return DA_NONE;
}

CharPtr Nlm_DiagGetRecordStr ( Uint1 recordIndex )
{
  if ( recordIndex < (Uint1)(diagArea.total) ){
    return &(diagArea.records[recordIndex].recordStr[0]);
  }
  return NULL;
}

