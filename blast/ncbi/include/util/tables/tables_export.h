#ifndef UTIL_TABLES___TABLES_EXPORT__H
#define UTIL_TABLES___TABLES_EXPORT__H

/*  $Id: tables_export.h,v 1.1 2003/08/21 19:50:51 ucko Exp $
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
 * Author:  Anatoliy Kuznetsov, Mike DiCuccio
 *
 * File Description:
 *    Defines to provide correct exporting from TABLES DLL in Windows.
 *    These are necessary to compile DLLs with Visual C++ - exports must be
 *    explicitly labeled as such.
 */


/*
 * NULL operations for other cases (C Toolkit)
 */

#  define NCBI_TABLES_EXPORT



/*
 * ==========================================================================
 *
 * $Log: tables_export.h,v $
 * Revision 1.1  2003/08/21 19:50:51  ucko
 * Add C-Toolkit-specific export setup for tables library
 *
 *
 * ==========================================================================
 */

#endif  /*  UTIL_TABLES___TABLES_EXPORT__H  */
