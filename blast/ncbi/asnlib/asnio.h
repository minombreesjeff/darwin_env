/* asnio.h
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
* File Name: asnio.h
*
* Author:  James Ostell
*
* Version Creation Date: 1/1/91
*
* $Revision: 6.1 $
*
* File Description:
*   Typedefs and prototypes used internally by asnio.c
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: asnio.h,v $
* Revision 6.1  1999/07/28 21:01:48  vakatov
* [__cplusplus]  Added `extern "C"'
*
* Revision 5.1  1996/12/03 21:43:48  vakatov
* Adopted for 32-bit MS-Windows DLLs
* ==========================================================================
*/

#ifndef _ASNIO_
#define _ASNIO_

#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif

#ifdef __cplusplus
extern "C" {
#endif


/*****************************************************************************
*
*   prototypes
*   	also in asngen.h
*
*****************************************************************************/
NLM_EXTERN CharPtr AsnIoGets PROTO((AsnIoPtr aip));
NLM_EXTERN void AsnIoPuts PROTO((AsnIoPtr aip));
NLM_EXTERN Int2 AsnIoReadBlock PROTO((AsnIoPtr aip));
NLM_EXTERN Int2 AsnIoWriteBlock PROTO((AsnIoPtr aip));
NLM_EXTERN void CDECL AsnIoErrorMsg VPROTO((AsnIoPtr aip, int errcode, ...));


#ifdef __cplusplus
}
#endif

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

#endif
