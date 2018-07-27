/*  $Id: ncbi_sendmail_.c,v 6.3 2001/07/25 19:21:06 lavr Exp $
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
 * Author:  Anton Lavrentiev
 *
 * File Description:
 *   Wrapper for "ncbi_sendmail.c" under NCBI C Toolkit
 *
 * ===========================================================================
 */


/* Configuration
 */
#include <ncbilcl.h>

#if defined(OS_UNIX)
#  define NCBI_OS_UNIX 1
#elif defined(OS_MSWIN)
#  define NCBI_OS_MSWIN 1
#elif defined(OS_MAC)
#  define NCBI_OS_MAC 1
#endif
#define NCBI_SENDMAIL_TOOLKIT "C"

/* Real code
 */
#include "ncbi_sendmail.c"
