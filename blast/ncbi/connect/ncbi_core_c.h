#ifndef CONNECT___NCBI_CORE_C__H
#define CONNECT___NCBI_CORE_C__H

/*  $Id: ncbi_core_c.h,v 6.4 2002/06/12 19:23:28 lavr Exp $
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
 * File description:
 *   C->C conversion functions for basic CORE connect stuff:
 *     Registry
 *     Logging
 *     Locking
 *
 */

#include <connect/ncbi_core.h>
#include <ncbithr.h>


#ifdef __cplusplus
extern "C" {
#endif


extern REG         REG_c2c(const char* conf_file);
extern LOG         LOG_c2c(void);
extern MT_LOCK MT_LOCK_c2c(TNlmRWlock lock, int/*bool*/ pass_ownership);

extern void   CONNECT_Init(const char* conf_file);


#ifdef __cplusplus
}  /* extern "C" */
#endif


/*
 * ---------------------------------------------------------------------------
 * $Log: ncbi_core_c.h,v $
 * Revision 6.4  2002/06/12 19:23:28  lavr
 * API functions enclosed in extern "C" for C++ compiler
 *
 * Revision 6.3  2002/06/12 16:41:37  lavr
 * +CONNECT_Init()
 *
 * Revision 6.2  2002/05/07 18:44:45  lavr
 * Refined API; Change log moved to the end
 *
 * Revision 6.1  2002/05/07 18:17:54  lavr
 * Initial revision
 *
 * ===========================================================================
 */

#endif  /* CONNECT___NCBI_CORE_C__H */
