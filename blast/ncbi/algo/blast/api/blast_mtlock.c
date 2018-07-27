/*  $Id: blast_mtlock.c,v 1.5 2005/03/31 16:18:21 dondosha Exp $
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
 * Author:  Ilya Dondoshansky
 *
 */

/** @file blast_mtlock.c
 * Initialization for the mutex locking interface. 
 */

static char const rcsid[] = 
    "$Id: blast_mtlock.c,v 1.5 2005/03/31 16:18:21 dondosha Exp $";

#include <ncbithr.h>
#include <algo/blast/api/blast_mtlock.h>
#include <algo/blast/core/blast_def.h>

/** @addtogroup CToolkitAlgoBlast
 *
 * @{
 */

/** Mutex lock handler for saving BLAST results. 
 * @param user_data Pointer to the mutex [in]
 * @param how What to do with the mutex? [in]
 */
static int CollectorLockHandler(void* user_data, EMT_Lock how)
{
   int result = 0;
   
   switch ( how ) {
   case eMT_Lock:
      result = NlmMutexLock((TNlmMutex)user_data) == 0;
      break;
   case eMT_Unlock:
      result = NlmMutexUnlock((TNlmMutex)user_data) == 0;
      break;
   default:
      break;
   }
   
   return result;
}

/** Mutex lock clean up handler.
 * @param user_data Pointer to the mutex [in]
 */
static void CollectorLockCleanup(void* user_data)
{
   NlmMutexDestroy((TNlmMutex)user_data);
}

MT_LOCK Blast_MT_LOCKInit()
{
   MT_LOCK lock;
   TNlmMutex results_mutex = NULL;
   results_mutex = NlmMutexInit(&results_mutex);
   lock = MT_LOCK_Create((void*)results_mutex, CollectorLockHandler, 
                     CollectorLockCleanup);
   return lock;
}

/* @} */

