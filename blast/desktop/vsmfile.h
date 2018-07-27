/*   vsmfile.h
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
* File Name:  vsmfile.h
*
* Author:  Jim Ostell
*
* Version Creation Date:   11-29-94
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
*
* ==========================================================================
*/


#ifndef _NCBI_VSMFile_
#define _NCBI_VSMFile_

#ifndef _NCBI_ObjMgr_
#include <objmgr.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
*
*   VSMFileInit()
*		initializes the LIBCALLBACK routines below by registering them
*          as OPEN and SAVE procs with objmgr.
*       These require VIBRANT and VSM
*
*****************************************************************************/

Boolean LIBCALL VSMFileInit PROTO((void));

Int2 LIBCALLBACK VSMGenericTextAsnOpen PROTO((Pointer data));
Int2 LIBCALLBACK VSMGenericBinAsnOpen PROTO((Pointer data));
Int2 LIBCALLBACK VSMFastaProtOpen PROTO((Pointer data));
Int2 LIBCALLBACK VSMFastaNucOpen PROTO((Pointer data));

Int2 LIBCALLBACK VSMGenericTextAsnSave PROTO((Pointer data));
Int2 LIBCALLBACK VSMGenericBinAsnSave PROTO((Pointer data));
Int2 LIBCALLBACK VSMFastaProtSave PROTO((Pointer data));
Int2 LIBCALLBACK VSMFastaNucSave PROTO((Pointer data));

#ifdef __cplusplus
}
#endif

#endif
