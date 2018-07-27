/* asnbuild.h
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
* File Name: asnbuild.h
*
* Author:  James Ostell
*
* Version Creation Date: 1/1/91
*
* $Revision: 6.0 $
*
* File Description:
*   Includes for building asntool and libraries
*
* Modifications:
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
* $Log: asnbuild.h,v $
* Revision 6.0  1997/08/25 18:09:39  madden
* Revision changed to 6.0
*
* Revision 5.2  1997/04/23 15:44:51  ostell
* commented out references to g_asnlib which caused unecessary error messages
* trying to open asnlib.msg file which does not exist.
*
 * Revision 5.1  1996/12/03  21:43:48  vakatov
 * Adopted for 32-bit MS-Windows DLLs
 *
 * Revision 5.0  1996/05/28  14:00:29  ostell
 * Set to revision 5.0
 *
 * Revision 4.0  1995/07/26  13:47:38  ostell
 * force revision to 4.0
 *
 * Revision 2.3  1995/05/15  18:38:28  ostell
 * added Log line
 *
*
*
* ==========================================================================
*/

#ifndef _ASNBUILD_
#define _ASNBUILD_

#include <asn.h>
#include "asntypes.h"
#include "asnio.h"
#include "asnlex.h"
#include "asnprint.h"
#include "asndebin.h"
#include "asnenbin.h"
#include "asnlext.h"
#include "asnout.h"


#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif


#ifdef __cplusplus
extern "C" {
#endif

/**
NLM_EXTERN char * g_asnlib;
#undef  THIS_MODULE
#define THIS_MODULE g_asnlib
***/


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

