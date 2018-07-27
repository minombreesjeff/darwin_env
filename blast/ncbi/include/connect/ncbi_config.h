#ifndef CONNECT___NCBI_CONFIG__H
#define CONNECT___NCBI_CONFIG__H

/*  $Id: ncbi_config.h,v 6.7 2003/04/02 16:20:53 rsmith Exp $
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
 * Author:  Denis Vakatov
 *
 * File Description:
 *   Configuration wrapper CONNECT library in NCBI C Toolkit
 *
 */

/* Real configuration
 */
#include <ncbilcl.h>

#if defined(OS_UNIX)
#  define NCBI_OS_UNIX 1
#  ifdef OS_UNIX_IRIX
#      define NCBI_OS_IRIX
#  endif
#  ifdef OS_UNIX_SOL
#      define NCBI_OS_SOLARIS
#  endif
#  ifdef OS_UNIX_BEOS
#      define NCBI_OS_BEOS
#  endif
#  ifdef OS_UNIX_DARWIN
#      define NCBI_OS_DARWIN 1
#    ifdef COMP_METRO
#      define NCBI_COMPILER_METROWERKS 1
#    endif
#  endif
#  if !defined(HAVE_GETHOSTBYNAME_R)
#    if   defined(OS_UNIX_SOL)
#      define HAVE_GETHOSTBYNAME_R 5
#      define HAVE_GETHOSTBYADDR_R 7
#    elif defined(OS_UNIX_LINUX)
#      define HAVE_GETHOSTBYNAME_R 6
#      define HAVE_GETHOSTBYADDR_R 8
#    endif
#  endif
#elif defined(OS_MSWIN)
#  define NCBI_OS_MSWIN 1
#elif defined(OS_MAC)
#  define NCBI_OS_MAC 1
#endif

#endif /* CONNECT___NCBI_CONFIG__H */
