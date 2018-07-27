/*  $Id: ncbi_core_c.c,v 6.9 2002/10/31 17:53:33 lavr Exp $
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

#include "ncbi_config.h"
#include <connect/ncbi_ansi_ext.h>
#include <connect/ncbi_core_c.h>
#include <connect/ncbi_util.h>
#include <ncbienv.h>
#include <ncbierr.h>
#include <stdlib.h>
#include <string.h>


/***********************************************************************
 *                              Registry                               *
 ***********************************************************************/

#ifdef __cplusplus
extern "C" {
#endif
    static void s_REG_Get    (void*, const char*, const char*, char*, size_t);
    static void s_REG_Set    (void*, const char*, const char*,
                              const char*, EREG_Storage);
    static void s_REG_Cleanup(void*);
#ifdef __cplusplus
}
#endif

static void s_REG_Get(void* user_data,
                      const char* section, const char* name,
                      char* value, size_t val_size)
{
    const char* conf_file = (const char*) user_data;
    char* dflt = (char*) (*value ? malloc(strlen(value) + 1) : 0);
    if (dflt)
        strcpy(dflt, value);
    Nlm_GetAppParam(conf_file, section, name, dflt, value, val_size);
    if (dflt)
        free(dflt);
}


static void s_REG_Set(void* user_data,
                      const char* section, const char* name,
                      const char* value, EREG_Storage storage)
{
    const char* conf_file = (const char*) user_data;
    Nlm_Boolean result = storage == eREG_Persistent
        ? Nlm_SetAppParam(conf_file, section, name, value)
        : Nlm_TransientSetAppParam(conf_file, section, name, value);
    verify(result);
}


static void s_REG_Cleanup(void* user_data)
{
    free(user_data);
}


extern REG REG_c2c(const char* conf_file)
{
    char* s = 0;

    if (!conf_file)
        conf_file = "ncbi";
    if (!(s = strdup(conf_file)))
        return 0/*failure*/;

    return REG_Create(s, s_REG_Get, s_REG_Set, s_REG_Cleanup, 0);
}


/***********************************************************************
 *                                Logger                               *
 ***********************************************************************/

#ifdef __cplusplus
extern "C" {
#endif
    static void s_LOG_Handler(void*, SLOG_Handler*);
#ifdef __cplusplus
}
#endif

static void s_LOG_Handler(void* user_data/*unused*/, SLOG_Handler* call_data)
{
    enum _ErrSev sev;
    char* msg;

    switch (call_data->level) {
    case eLOG_Trace:
        sev = SEV_INFO; /* FIXME: Should be somewhat different */
        break;
    case eLOG_Note:
        sev = SEV_INFO;
        break;
    case eLOG_Warning:
        sev = SEV_WARNING;
        break;
    case eLOG_Error:
        sev = SEV_ERROR;
        break;
    case eLOG_Critical:
        sev = SEV_REJECT;
        break;
    case eLOG_Fatal:
        /*fallthru*/
    default:
        sev = SEV_FATAL;
        break;
    }

    if (Nlm_ErrSetContext(call_data->module, call_data->file, call_data->line,
                          DBFLAG/*defined in ncbierr.h*/, 0, 0, 0) != 0)
        return/*busy*/;

    if (!(msg = LOG_ComposeMessage(call_data, fLOG_None)))
        return/*failed*/;

    Nlm_ErrPostStr(sev, 0, 0, msg);

    if (call_data->level == eLOG_Trace)
        Nlm_ErrClear();

    free(msg);
}


extern LOG LOG_c2c(void)
{
    return LOG_Create(0, s_LOG_Handler, 0, 0);
}


/***********************************************************************
 *                               MT-Lock                               *
 ***********************************************************************/

#ifdef __cplusplus
extern "C" {
#endif
    static int/*bool*/ s_LOCK_Handler(void*, EMT_Lock);
    static void        s_LOCK_Cleanup(void*);
#ifdef __cplusplus
}
#endif

static int/*bool*/ s_LOCK_Handler(void* user_data, EMT_Lock how)
{
    TNlmRWlock lock = (TNlmRWlock) user_data;
    int/*bool*/ result;
    if ( !lock )
        return -1/*rightful not doing*/;
    switch ( how ) {
    case eMT_Lock:
        result = NlmRWwrlock(lock) == 0;
        break;
    case eMT_LockRead:
        result = NlmRWrdlock(lock) == 0;
        break;
    case eMT_Unlock:
        result = NlmRWunlock(lock) == 0;
        break;
    default:
        assert(0);
        result = 0/*false*/;
        break;
    }
    return result;
}


static void s_LOCK_Cleanup(void* user_data)
{
    NlmRWdestroy((TNlmRWlock) user_data);
}


extern MT_LOCK MT_LOCK_c2c(TNlmRWlock lock, int/*bool*/ pass_ownership)
{
    return MT_LOCK_Create(lock ? lock : NlmRWinit(), s_LOCK_Handler,
                          !lock  ||  pass_ownership ? s_LOCK_Cleanup : 0);
}


/***********************************************************************
 *                                 Init                                *
 ***********************************************************************/

extern void CONNECT_Init(const char* conf_file)
{
    CORE_SetLOCK(MT_LOCK_c2c(0, 1/*true*/));
    CORE_SetLOG(LOG_c2c());
    CORE_SetREG(REG_c2c(conf_file));
}


/*
 * ---------------------------------------------------------------------------
 * $Log: ncbi_core_c.c,v $
 * Revision 6.9  2002/10/31 17:53:33  lavr
 * Clear error in case of a trace message
 *
 * Revision 6.8  2002/09/24 18:07:12  lavr
 * Declare s_LOCK_Handler static in the first place of declaration
 *
 * Revision 6.7  2002/07/05 17:52:46  lavr
 * LOCK handler to return -1 in case of an empty lock object
 *
 * Revision 6.6  2002/07/03 19:53:40  lavr
 * Do not consult environment in registry searches (use only GetAppParam())
 *
 * Revision 6.5  2002/07/02 21:20:53  lavr
 * Use INFO severity for tracing; patch for registry lookup in environment
 *
 * Revision 6.4  2002/06/12 16:41:37  lavr
 * +CONNECT_Init()
 *
 * Revision 6.3  2002/05/08 19:04:37  kans
 * needed to include connect/ncbi_ansi_ext.h
 *
 * Revision 6.2  2002/05/07 18:44:45  lavr
 * Refined API; Change log moved to the end
 *
 * Revision 6.1  2002/05/07 18:17:54  lavr
 * Initial revision
 *
 * ===========================================================================
 */
