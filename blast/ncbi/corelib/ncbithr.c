/* $Id: ncbithr.c,v 6.38 2003/12/04 18:02:43 rsmith Exp $ */
/*****************************************************************************

    Name: ncbithr.c

    Description: Internal functions for Multi-thread API library

    Authors: Denis Vakatov, Sergei Shavirin

   ***************************************************************************

                          PUBLIC DOMAIN NOTICE
              National Center for Biotechnology Information

    This software/database is a "United States Government Work" under the
    terms of the United States Copyright Act.  It was written as part of
    the author's official duties as a United States Government employee
    and thus cannot be copyrighted.  This software/database is freely
    available to the public for use. The National Library of Medicine and
    the U.S. Government have not placed any restriction on its use or
    reproduction.

    Although all reasonable efforts have been taken to ensure the accuracy
    and reliability of the software and data, the NLM and the U.S.
    Government do not and cannot warrant the performance or results that
    may be obtained by using this software or data. The NLM and the U.S.
    Government disclaim all warranties, express or implied, including
    warranties of performance, merchantability or fitness for any
    particular purpose.

    Please cite the author in any work or product based on this material.

   ***************************************************************************

 Modification History:
-----------------------------------------------------------------------------
* $Log: ncbithr.c,v $
* Revision 6.38  2003/12/04 18:02:43  rsmith
* Move includes out of definition of NlmCPUNumber.
*
* Revision 6.37  2003/09/19 22:58:38  coulouri
* NetBSD does not (yet?) have pthread_attr_setschedpolicy()
*
* Revision 6.36  2003/05/05 11:57:34  rsmith
* Codewarrior and MSC compilers use the same declaration for thread_exit_arr and thread_self.
*
* Revision 6.35  2003/03/06 15:12:27  coulouri
* add mach/mach.h header for codewarrior
*
* Revision 6.34  2003/03/06 14:15:04  coulouri
* add mach support for NlmCPUNumber
*
* Revision 6.33  2002/03/12 15:50:00  ivanov
* Changed a name created mutex from "Nlm_InitLock32" to NULL
* in the NlmMutexInit() under WIN32
*
* Revision 6.32  2001/12/14 21:09:20  ivanov
* Enable threads under WIN32 in MT-configurations
*
* Revision 6.31  2001/11/09 14:18:06  ivanov
* Fixed error in the preprocessor command
*
* Revision 6.30  2001/11/08 21:57:19  ivanov
* Changed NlmCPUNumber() (under UNIX)
*
* Revision 6.29  2001/01/19 20:23:34  kans
* support for OS_UNIX_DARWIN (contributed by William Van Etten)
*
* Revision 6.28  2001/01/08 19:52:26  vakatov
* NlmThreadCreateEx() -- by default, to use BOUND scope scheduling (if
* found) or PROCESS scope scheduling (the last default).
* Use "-DPOSIX_BOUND_THREADS_AVAIL" to enforce SYSTEM scope scheduling.
* {By Haruna N. Cofer;  Applications - Chem/Bio, SGI; haruna@sgi.com}
*
* Revision 6.27  2000/11/06 17:09:20  vakatov
* RW_HISTORY, RW_TRACE -- To gather and printout info on the RW-lock history
*
* Revision 6.26  2000/05/16 20:26:01  vakatov
* [WIN32_THREADS_AVAIL] Changed the locking policy for nested RW-locks.
* // Fully tested on Solaris(native and POSIX), Win-NT, IRIX, and Linux
*
* Revision 6.25  2000/03/01 19:44:44  vakatov
* NlmMutexUnlock() -- fixed by Haruna N. Cofer (Applications - Chem/Bio,
* SGI; haruna@sgi.com) for an intermittent deadlock happening on SGI/IRIX
* and caused by the swapping of two adjacent operators in s_MutexLock()
* by the C compiler optimizer (when compiled with "-O" flag). Thanks, Haruna!
*
* Revision 6.24  1999/11/08 17:03:05  vakatov
* [POSIX] NlmThreadCreateEx() -- fixed a bug leading to the thread
* handle corruption on { BIG_ENDIAN, 64-bit pointer, 32-bit pthread_t}
* platforms (like 64-bit IRIX/SGI/MIPS)
*
* Revision 6.23  1999/10/14 19:43:24  kans
* new headers for mac
*
* Revision 6.22  1999/10/14 19:08:09  kans
* header changes for Mac
*
* Revision 6.21  1999/09/20 17:48:32  vakatov
* [POSIX_THREADS_AVAIL]  Bug fix for R6.20 -- for the case of "R1, R2, U1, W1"
*
* Revision 6.20  1999/08/20 19:55:18  vakatov
* s_ThreadCounter***:  use {counter + semaphore} instead of just
* {RWlock} to count and join all threads.
* [POSIX_THREADS_AVAIL] Changed the locking policy for nested RW-locks.
* // Fully tested on Solaris(native and POSIX), Win-NT, IRIX, OSF1 and Linux
*
* Revision 6.19  1998/12/10 17:04:08  vakatov
* Fixed to compile under LINUX(Red Hat 2.XX, gcc, with POSIX threads)
*
* Revision 6.18  1998/09/22 14:50:58  vakatov
* Use "s_TlsMutex" to protect the TLS key creation in "init_exit_arr()"
* -- instead of former "s_Key_mutex"
*
* Revision 6.17  1998/09/01 16:35:58  vakatov
* [OS_UNUX_IRIX]  By default, create regular unbound(PTHREAD_SCOPE_PROCESS)
* threads as there may be no "capability" to create bound threads.
* Hint:  compile with "-DPOSIX_BOUND_THREADS_AVAIL" if you wanna allow
* bound threads for IRIX(see also CAP_SCHED_MGT in "man capabilities").
*
* Revision 6.16  1998/07/28 20:31:37  vakatov
* Moved(slightly modified) contents of former "ncbithri.h" to "ncbithr.c"
*
* Revision 6.15  1998/07/16 17:43:35  vakatov
* [OS_UNIX_AIX, POSIX]  Dont set thread priority on this platform as
* pthread_attr_setschedparam() returns non-zero(some error?) value here
*
* Revision 6.14  1998/07/13 18:47:28  vakatov
* [POSIX] NlmThreadCreateEx():  call "getschedparam()" before
* "setschedparam()" to make sure that "param" is properly initialized
*
* Revision 6.13  1998/06/11 19:00:01  shavirin
* Fixed some compiler warnings.
*
* Revision 6.12  1998/02/27 17:13:53  vakatov
* [WIN32 DLL]  Declared some functions as NLM_EXTERN(DLL-exportable)
*
* Revision 6.11  1998/02/24 22:38:45  vakatov
* NlmThreadDestroyAll():  check internal RW-locks & mutexes for NULL...
*
* Revision 6.10  1998/02/20 18:56:33  vakatov
* [NCBI_NOTHREADS_AVAIL]  Fixed minor mem.leak in NlmTlsSetValue()
*
* Revision 6.9  1998/02/17 20:13:51  vakatov
* NlmTlsInit() removed from the public API;  instead, the
* NlmTlsSetValue() now initializes the TLS if it is not initialized yet
*
* Revision 6.8  1998/02/13 15:15:32  vakatov
* Added "cleanup" parameter to NlmTlsSetValue() to provide TLS cleanup.
* Removed NlmTlsDestroy(); now destroy all TLSs in NlmThreadDestroyAll()
*
* Revision 6.7  1998/01/08 17:27:53  vakatov
* Added NlmThreadJoinAll() and NlmThreadCleanupAll()
*
* Revision 6.6  1997/12/24 17:17:59  vakatov
* Added TLS(Thread Local Storage) functionality for all platforms(incl.
* non-"thread-capable" ones -- interpreted as a single-thread application)
*
* Revision 6.5  1997/12/23 19:13:29  vakatov
* Thread handling -- revised, fixed:
* removed NlmThreadKill(), NlmThread[Set|Get]Concurrency() and
* NlmThread[Set|Get]Priority() functions;
* NlmThreadCreate() doesn't accept parameter "flags" anymore;
* added "priority" parameter to NlmThreadCreateEx()
*
* Revision 6.4  1997/12/17 19:11:24  vakatov
* Use all-platform THREAD_{RUN|BOUND|DETACHED} instead of the platform
* specific THR_{...} thread creation/running attributes
*
* Revision 6.3  1997/12/16 23:12:35  vakatov
* Mutexes and RW-locks:  revised, rewritten, fixed;  added built-in
* strict run-time checks -- (use -D_DEBUG_HARD)
*
* Revision 6.2  1997/12/05 22:16:21  vakatov
* [SOLARIS_THREADS_AVAIL] Made RWlock functions always return "-1" on error
* [POSIX_THREADS_AVAIL]   Fixed minor mem.leak
* [TEST_MODULE_NCBITHR]   Allow arbitrary N_SPAWN(was restricted to max.6)
*
* Revision 6.1  1997/10/08 14:54:22  vakatov
* [WIN32]  Implemented RW-lock functionality
* [POSIX]  Fixed bug in RW[try[rw|rd]lock() functions
* [DO_RWLOCK] Added test code for RW-lock functionality
*
* Revision 5.23  1997/07/16 21:12:11  vakatov
* #define NULL_thread -- rather than global variable "NULL_thread"
*
* Revision 5.22  1997/05/13 17:29:42  vakatov
* Fixed typo in NlmMutexTryLock()
* [POSIX_THREADS_AVAIL]  Initialize Nlm_lock with PTHREAD_MUTEX_INITIALIZER
*
* Revision 5.21  1997/03/31 18:59:25  vakatov
* Minor fixes(type casts) to pass through the C++ compiler
*
* Revision 5.20  1997/03/17  21:49:04  vakatov
* NlmThreadWrapper():  do not print banner to the logfile when starting
* a new thread.
* [POSIX_THREADS_AVAIL] NlmRWtrywrlock():  fixed "=" -> "==" in IF statement.
*
* Revision 5.19  1997/02/20  16:05:31  shavirin
* Moved to standard 1003.1c for DEC Threads
*
* Revision 5.18  1997/02/19  20:39:16  shavirin
* Changed function pthread_attr_create to pthread_attr_init() for IRIX 6.2
*
* Revision 5.17  1997/02/18  19:02:43  shavirin
* Fixed DETACHED state in POSIX threads
*
* Revision 5.16  1997/01/24  20:41:00  shavirin
* Added detach handling for OSF1
*
* Revision 5.15  1997/01/09  15:14:21  shavirin
* Added global NULL_thread variable
*
* Revision 5.14  1997/01/08  23:07:47  shavirin
* Added support for DEC ALPHA OSF1
*
* Revision 5.13  1996/12/03  21:48:33  vakatov
* Adopted for 32-bit MS-Windows DLLs
*
* Revision 5.12  1996/11/25  19:02:27  vakatov
* Inherit error posting flags and log-file from the parent thread.
* NlmMutexInit() now accepts pointer to mutex as an argument -- to
* guarantee no MT-mess during the mutex initialization.
* Added function NlmMutexLockEx() to lock yet initialized mutex;
* it initialize the mutex before locking, if necessary.
* Semaphor functions, NlmThreadKill(), NlmMutexUnlock(), NlmMutexDestroy():
* got rid of the return value discrepancy between different platforms;
* now return 0 on success, -1 on error on all platforms.
* NlmMutexLock<Ex>():  protected from "self-deadlock" under Solaris/POSIX.
*
* Revision 5.11  1996/10/16  00:54:53  epstein
* add NCBI_NOTHREADS_AVAIL flag to disable threading
*
* Revision 5.10  1996/09/04  19:37:32  epstein
* workaround for SGI4
*
* Revision 5.9  1996/09/04  14:34:44  kans
* trivial fixes to allow compilation under Symantec C++
*
* Revision 5.8  1996/08/22  14:07:53  shavirin
* Finaly NlmThreadsAvailable return TRUE for WIN32
*
* Revision 5.7  1996/08/07  17:07:58  shavirin
* Added function NlmCPUNumber(void), that returns number of CPU
* on calling host
*
* Revision 5.6  1996/08/02  19:52:38  vakatov
* Most of the thread-handling code has been protected from being
* compiled on the non-thread platforms
*
* Revision 5.5  1996/08/02  15:59:38  shavirin
* Added support for POSIX threads. Implemented semaphores and
* Read-Write lock objects for POSIX threads through POSIX mutexes
*
* Revision 5.4  1996/07/16  19:58:50  vakatov
* [WIN32]  NlmThreadSelf() now returns handler rather than pseudo-handler.
* [SOLARIS...] [POSIX...]  init_exit_arr() protected by "mutex" from.
* [POSIX] "mutex"-related stuff implemented.
* NlmThreadMemFree() auxiliary function added.
* The "thread destructors" calling order has been changed from FIFO to LIFO.
* Added NlmThreadCreateEx() function to specify the thread destructor
* from outside the thread.
* The program name property is to be automatially inherited by the threads.
*
* Revision 5.3  1996/07/09  18:13:37  vakatov
* Added a basic MT-support for POSIX and WIN32-SDK(Windows-95 and Windows-NT)
* API -- the thread creation, joining and terminating. The thread joining
* (waiting for the terminating) interface and behaviour has been slightly
* changed.
* Tested the lately introduced "OnExit"-features for the new platforms.
*
* Revision 5.2  1996/07/05  21:09:22  vakatov
* Added NlmThreadAddOnExit() and NlmThreadRemoveOnExit() functions allowing
* the user to specify a set of functions to be automatically executed on the
* thread exiting stage. In addition, the execution of NlmThreadExit() function
* is guaranteed in the case when the thread's main function exits with
* the "return" operator, without calling the NlmThreadExit() explicitly
*
* The concerning stuff is also implemented(however, not tested yet) for
* POSIX and WIN32-SDK(MSVC++ and Borland C++) threads
*
* Tests are added at the end of the file(preprocessor variable TEST_MODULE)
* to provide easy testing of the new(and some of the old) NCBI thread features
*
* 30 Apr 1996 - Shavirin Sergei  -  original written
****************************************************************************/


#include <ncbistd.h>
#include <ncbimem.h>
#include <ncbierr.h>
#include <ncbistr.h>
#include <ncbiprop.h>

#ifdef WIN32
#include <windows.h>
#if defined(_MT)
#   undef NCBI_NOTHREADS_AVAIL
#endif
#endif


#ifdef NCBI_NOTHREADS_AVAIL

#ifdef SOLARIS_THREADS_AVAIL
#undef SOLARIS_THREADS_AVAIL
#endif
#ifdef POSIX_THREADS_AVAIL
#undef POSIX_THREADS_AVAIL
#endif
#ifdef WIN32_THREADS_AVAIL
#undef WIN32_THREADS_AVAIL
#endif

#else /* def NCBI_NOTHREADS_AVAIL */

#if !defined(WIN32)  &&  !defined(WIN16)  &&  !defined(WIN_MAC)
#include <unistd.h>
#endif /* ndef WIN32, WIN16, WIN_MAC */

#if defined(OS_UNIX_IRIX)
#define PTHREAD_SCOPE_BOUND_NP 2
#endif

#if   defined(SOLARIS_THREADS_AVAIL)
#define NCBI_THREADS_AVAIL
#include <thread.h>
#include <signal.h>
#include <synch.h>

#elif defined(POSIX_THREADS_AVAIL)
#define NCBI_THREADS_AVAIL
#ifdef  _PTHREAD_USE_D4
#undef  _PTHREAD_USE_D4
#endif
#if !defined(POSIX_BOUND_THREADS_AVAIL)  &&  !defined(OS_UNIX_IRIX)
#define POSIX_BOUND_THREADS_AVAIL
#endif
#include <pthread.h>
#include <sched.h>
#include <signal.h>

#elif defined(WIN32_THREADS_AVAIL)
#define NCBI_THREADS_AVAIL
#endif /* def SOLARIS_THREADS_AVAIL elif POSIX_... elif WIN32_... */

#endif /* def else NCBI_NOTHREADS_AVAIL */


#include <ncbithr.h>


#ifdef _DEBUG_HARD
#define X_ASSERT(expr) ((expr) ? \
  (void)0 : Nlm_AssertionFailed(#expr,THIS_MODULE,THIS_FILE,__LINE__))
#define X_VERIFY X_ASSERT
#else
#define X_ASSERT(expr) ((void)0)
#define X_VERIFY(expr) ((void)(expr))
#endif /* _DEBUG_HARD -- for the internal strict debugging */


TNlmMutex corelibMutex; /* exported in "corepriv.h" */


/***********************************************************************
 *  INTERNAL
 ***********************************************************************/

/* this platform's native mutex type & default initialization */
#if   defined (SOLARIS_THREADS_AVAIL)
  typedef mutex_t MUTEX_T;
#define MUTEX_T_INIT
#elif defined (POSIX_THREADS_AVAIL)
  typedef pthread_mutex_t MUTEX_T ;
#define MUTEX_T_INIT = PTHREAD_MUTEX_INITIALIZER
#elif defined (WIN32_THREADS_AVAIL)
  typedef HANDLE MUTEX_T;
#define MUTEX_T_INIT
#endif


/* POSIX implementation suppose that semaphore and RW objects
 * must be implemented by the user himself
 */
#if defined (POSIX_THREADS_AVAIL)
typedef struct psema_t {
  Int4             count;
  pthread_mutex_t  mutex;
  pthread_cond_t   cond;
} psema_t;
#endif


/* To allow main NCBI thread to wait until all other NCBI threads are exited
 */
#ifdef NCBI_THREADS_AVAIL
/* Counter for the running NCBI threads (excluding the main thread):  >= 0
 */
static Uint4 s_ThreadCounter = 0;

/* Semaphore to wait (at the very end of main thread) for the
 * "s_ThreadCounter" to become zero -- to make sure that all registered
 * NCBI threads are terminated
 */ 
static TNlmSemaphore s_ThreadCounter_sema = 0;

/* Protective mutex for the counter and the semaphore
 */
static TNlmMutex s_ThreadCounter_mutex = 0;
#endif


/*
 *  Functions and data structures providing storage and execution of
 *  the "thread destructor functions"
 */
typedef struct
{
  TNlmThreadOnExit func;
  VoidPtr          arg;
}  TOnExitData;

typedef struct
{
  size_t      size;
  TOnExitData user[1];
}  TOnExitArray, PNTR TOnExitArrayPtr;


#if   defined (SOLARIS_THREADS_AVAIL)
static  thread_key_t thread_exit_key;
#elif defined (POSIX_THREADS_AVAIL)
static pthread_key_t thread_exit_key;
#elif defined (WIN32_THREADS_AVAIL)
#if defined(_MSC_VER)  || defined(COMP_METRO)
static __declspec( thread ) TOnExitArrayPtr thread_exit_arr = NULL;
static __declspec( thread ) HANDLE          thread_self     = NULL;
#else
static TOnExitArrayPtr      __thread        thread_exit_arr = NULL;
static HANDLE               __thread        thread_self     = NULL;
#endif
#else
static TOnExitArrayPtr                      thread_exit_arr = NULL;
#endif


/* protective mutex for all TLS-related objects */
static TNlmMutex s_TlsMutex;


static Boolean init_exit_arr(void)
{
  static Boolean initialized = FALSE;

  if ( initialized )
    return TRUE;

  X_VERIFY( !NlmMutexLockEx(&s_TlsMutex) );
#if   defined (SOLARIS_THREADS_AVAIL)
  if (!initialized  &&
      thr_keycreate(&thread_exit_key, NULL) == 0)
    initialized = TRUE;
#elif defined (POSIX_THREADS_AVAIL)
  if (!initialized  &&
      pthread_key_create(&thread_exit_key, NULL) == 0)
    initialized = TRUE;
#elif defined (WIN32_THREADS_AVAIL)
  initialized = TRUE;
#else
  initialized = TRUE;
#endif
  X_VERIFY( !NlmMutexUnlock(s_TlsMutex) );

  return initialized;
}


static TOnExitArrayPtr get_exit_arr(void)
{
  if ( !init_exit_arr() )
    return NULL;

  {{
#if   defined (SOLARIS_THREADS_AVAIL)
  VoidPtr arr;
  if (thr_getspecific(thread_exit_key, &arr) != 0)
    return NULL;
  return (TOnExitArrayPtr)arr;
#elif defined (POSIX_THREADS_AVAIL)
  return (TOnExitArrayPtr)pthread_getspecific( thread_exit_key );
#elif defined (WIN32_THREADS_AVAIL)
  return thread_exit_arr;
#else
  return thread_exit_arr;
#endif
  }}
}


static void set_exit_arr(TOnExitArrayPtr arr_ptr)
{
#if   defined (SOLARIS_THREADS_AVAIL)
  if (    thr_setspecific(thread_exit_key, (VoidPtr)arr_ptr) != 0)
    abort();
#elif defined (POSIX_THREADS_AVAIL)
  if (pthread_setspecific(thread_exit_key, (VoidPtr)arr_ptr) != 0)
    abort();
#elif defined (WIN32_THREADS_AVAIL)
  thread_exit_arr = arr_ptr;
#else
  thread_exit_arr = arr_ptr;
#endif
}

/* Internal prototypes
 */
static Boolean s_TlsCleanupAll(void);


/***********************************************************************
 *  EXTERNAL
 ***********************************************************************/

NLM_EXTERN Boolean NlmThreadAddOnExit(TNlmThreadOnExit func, VoidPtr arg)
{
  TOnExitArrayPtr arr_ptr = get_exit_arr();
  size_t size = (arr_ptr == NULL) ? 0 : arr_ptr->size;

  size_t i = 0;
  while (i < size  &&  arr_ptr->user[i].func != NULL)
    i++;

  if (i == size)
    {
      TOnExitArrayPtr new_arr = (TOnExitArrayPtr)
        Nlm_MemNew(sizeof(TOnExitArray) + size * sizeof(TOnExitData));
      if (new_arr == NULL)
        return FALSE;

      new_arr->size = size + 1;
      if (size != 0)
        {
          Nlm_MemCpy(new_arr->user, arr_ptr->user,
                     size * sizeof(TOnExitData));
          Nlm_MemFree( arr_ptr );
        }
      arr_ptr = new_arr;
    }

  arr_ptr->user[i].func = func;
  arr_ptr->user[i].arg  = arg;
  set_exit_arr( arr_ptr );
  return TRUE;
}


NLM_EXTERN void NlmThreadRemoveOnExit(TNlmThreadOnExit func, VoidPtr arg)
{
  TOnExitArrayPtr arr_ptr = get_exit_arr();
  size_t size = (arr_ptr == NULL) ? 0 : arr_ptr->size;

  size_t i;
  for (i = 0;  i < size;  i++)
    {
      if (arr_ptr->user[i].func == func  &&
          arr_ptr->user[i].arg  == arg)
        {
          Nlm_MemMove(arr_ptr->user + i, arr_ptr->user + i + 1,
                      (size - i - 1) * sizeof(TOnExitData));
          arr_ptr->user[size-1].func = NULL;
        }
    }
}


NLM_EXTERN void NlmThreadMemFree(VoidPtr ptr)
{
  Nlm_MemFree(ptr);
}


/* --------------------  NlmThreadWrapper  -------------------------
   Purpose:     To wrap the starting thread main function
   Parameters:  The user's main thread function with args and flags
   Returns:     New thread handle. -- to be used to identify the thread.
   Description: This function creates new thread - independent process
                inside current process, that share most resources of the
                parent thread.
   NOTE:        This function must be used in NlmThreadCreate() only!
  ------------------------------------------------------------------*/

#ifdef NCBI_THREADS_AVAIL
typedef struct
{
  TNlmThreadStart  func;
  VoidPtr          arg;
  TNlmThreadOnExit exit_func;
  VoidPtr          exit_arg;
  CharPtr          progName;
  CharPtr          logName;
  ErrOpts          errOpts;
} TNlmThreadWrapperArg, PNTR TNlmThreadWrapperArgPtr;


static
#if defined(SOLARIS_THREADS_AVAIL) || defined(POSIX_THREADS_AVAIL)
VoidPtr
#elif defined(WIN32_THREADS_AVAIL)
DWORD WINAPI
#endif
NlmThreadWrapper(VoidPtr wrapper_arg)
{
  VoidPtr ret_value;
  TNlmThreadWrapperArgPtr user = (TNlmThreadWrapperArgPtr)wrapper_arg;
  TNlmThreadStart startFunc    = user->func;
  VoidPtr         startFuncArg = user->arg;

#ifdef WIN32_THREADS_AVAIL
  /* duplicate and store an actual(not a pseudo-) handle to the thread */
  if ( !DuplicateHandle(GetCurrentProcess(), GetCurrentThread(),
                        GetCurrentProcess(), &thread_self,
                        0, FALSE, DUPLICATE_SAME_ACCESS) )
    abort();
#endif

  /* register the exiting function specified by the thread parent, if any */
  if (user->exit_func != NULL  &&
      !NlmThreadAddOnExit(user->exit_func, user->exit_arg))
    abort();

  /* store program name */
  if (user->progName != NULL)
    Nlm_SetAppProperty("ProgramName", (void *)user->progName);

  /* set the error posting environment */
  if (user->logName != NULL)
    {
      Nlm_ErrSetLogfile(user->logName, ELOG_APPEND);
      user->logName = (CharPtr)Nlm_MemFree( user->logName );
    } 
  ErrRestoreOptions( &user->errOpts );

  Nlm_Free( wrapper_arg );

  /* run the user's thread function */
  ret_value = (*startFunc)( startFuncArg );

  /* the regular exit from the thread */
  NlmThreadExit( ret_value );

  /* should never reach here... */
  abort();
  return 0;
}
#endif  /* NCBI_THREADS_AVAIL */



NLM_EXTERN TNlmThread NlmThreadCreate(TNlmThreadStart theStartFunction,
                                      VoidPtr arg)
{
  return NlmThreadCreateEx(theStartFunction, arg,
                           THREAD_RUN, eTP_Default, NULL, NULL);
}


#if defined(SOLARIS_THREADS_AVAIL) || defined(POSIX_THREADS_AVAIL)

#if defined(OS_UNIX_SOL) && defined(POSIX_THREADS_AVAIL)
#define prio_value(x) (-1)
#else
static int prio_value(EThreadPriority priority)
{
#if defined(SOLARIS_THREADS_AVAIL)
  const int min = 0;
  const int max = 127;
#elif defined(POSIX_THREADS_AVAIL)
#ifdef OS_UNIX_OSF1
  const int min = PRI_OTHER_MIN;
  const int max = PRI_OTHER_MAX;
#elif defined(OS_UNIX_DARWIN)
  const int min = -20;
  const int max = 20;
#else
  int min = sched_get_priority_min(SCHED_OTHER);
  int max = sched_get_priority_max(SCHED_OTHER);
#endif
#endif

  switch ( priority )
    {
    case eTP_Default:
    case eTP_Normal:
      return (max + min) / 2;
    case eTP_Idle:    return min;
    case eTP_Lowest:  return min + (max - min) / 5;
    case eTP_Low:     return min + (max - min) / 3;
    case eTP_High:    return min + (max - min) * 3/4;
    case eTP_Highest: return max;
    }
  X_ASSERT ( FALSE );
  return (max + min) / 2;
}
#endif

#elif defined(WIN32_THREADS_AVAIL)
  static const int prio_value[eTP_Default] = {
    THREAD_PRIORITY_IDLE,         THREAD_PRIORITY_LOWEST,
    THREAD_PRIORITY_BELOW_NORMAL, THREAD_PRIORITY_NORMAL,
    THREAD_PRIORITY_ABOVE_NORMAL, THREAD_PRIORITY_HIGHEST 
  };
#endif


NLM_EXTERN TNlmThread NlmThreadCreateEx(TNlmThreadStart  theStartFunction,
                                        VoidPtr          arg,
                                        Int4             flags,
                                        EThreadPriority  priority,
                                        TNlmThreadOnExit exit_func,
                                        VoidPtr          exit_arg)
{
  TNlmThread thread_handle = NULL_thread;

#ifdef NCBI_THREADS_AVAIL

  /* initialize thread wrapper func */
  TNlmThreadWrapperArgPtr wrapper_data =
    (TNlmThreadWrapperArgPtr) Calloc(1, sizeof(TNlmThreadWrapperArg));
  if (wrapper_data == NULL)
    return NULL_thread;
  wrapper_data->func      = theStartFunction;
  wrapper_data->arg       = arg;
  wrapper_data->exit_func = exit_func;
  wrapper_data->exit_arg  = exit_arg;
  wrapper_data->progName  =
    Nlm_StringSave( (char *)Nlm_GetAppProperty("ProgramName") );
  wrapper_data->logName   = Nlm_StringSave( Nlm_ErrGetLogfile() );
  ErrSaveOptions(&wrapper_data->errOpts);

  /* indicate that one more NCBI thread is running */
  NlmMutexLockEx(&s_ThreadCounter_mutex);
  if ( !s_ThreadCounter_sema ) {
    s_ThreadCounter_sema = NlmSemaInit(0);
  }    
  s_ThreadCounter++;
  NlmMutexUnlock(s_ThreadCounter_mutex);

  /* create new thread */
#if   defined(SOLARIS_THREADS_AVAIL)
  {{
    thread_t tid;
    long x_flags = 0;
    if (flags & THREAD_DETACHED) /* default - attached */
      x_flags |= THR_DETACHED;

    if (flags & THREAD_BOUND)    /* default - unbound */
      x_flags |= THR_BOUND;
    else if (priority != eTP_Default)
      x_flags |= THR_SUSPENDED;  /* create suspended to adjust the priority */

    if (thr_create(NULL, 0, NlmThreadWrapper, (void *)wrapper_data,
                   x_flags, &tid) != 0) {
      tid = 0;
    }
    else if (priority != eTP_Default) {/* adjust priority and resume */
      if (thr_setprio(tid, prio_value(priority)) != 0  ||
          thr_continue(tid) != 0) {
        X_VERIFY ( !thr_kill(tid, SIGKILL) );
        tid = 0;
      }
    }
    thread_handle = (TNlmThread)tid;
  }}

#elif defined(POSIX_THREADS_AVAIL)
  {{
#if !defined(PTHREAD_CREATE_JOINABLE)  &&  defined(PTHREAD_CREATE_UNDETACHED)
#define PTHREAD_CREATE_JOINABLE PTHREAD_CREATE_UNDETACHED
#endif
    pthread_attr_t attr;
    pthread_t      thread_id;
    X_VERIFY ( !pthread_attr_init(&attr) );
    X_VERIFY ( !pthread_attr_setdetachstate(&attr, (flags & THREAD_DETACHED) ?
                                            PTHREAD_CREATE_DETACHED :
                                            PTHREAD_CREATE_JOINABLE) );
#ifdef POSIX_BOUND_THREADS_AVAIL
    if (flags & THREAD_BOUND)
      pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
    else
#elif defined(OS_UNIX_IRIX)
    if (flags & THREAD_BOUND)
      /* SGI-HNC: Check for PTHREAD_SCOPE_BOUND (IRIX 6.5.8)
         or PTHREAD_SCOPE_BOUND_NP (IRIX 6.5.9 and higher) */
     if (pthread_attr_setscope(&attr, PTHREAD_SCOPE_BOUND_NP) != 0)
       /* SGI-HNC: Otherwise default to PTHREAD_SCOPE_PROCESS */
#endif
#ifndef OS_UNIX_NETBSD
      if (!pthread_attr_setscope(&attr, PTHREAD_SCOPE_PROCESS)  &&
          !pthread_attr_setschedpolicy(&attr, SCHED_OTHER)  &&
          priority != eTP_Default) { /* adjust the thread priority */
#ifndef OS_UNIX_AIX
        int new_priority = prio_value(priority);
        if (new_priority != -1) {
          struct sched_param param;
          X_VERIFY ( !pthread_attr_getschedparam(&attr, &param) );
          param.sched_priority = new_priority;
          X_VERIFY ( !pthread_attr_setschedparam(&attr, &param) );
        }
#endif
      }
#endif

    if (pthread_create(&thread_id, &attr,
                       NlmThreadWrapper, (void *)wrapper_data) == 0) {
      thread_handle = (TNlmThread) thread_id;
    } else { 
      X_ASSERT( 0 );
      thread_handle = NULL_thread;
    }
    X_VERIFY ( !pthread_attr_destroy(&attr) );
  }}

#elif defined(WIN32_THREADS_AVAIL)
  {{
    DWORD thread_id;
    thread_handle = (TNlmThread)CreateThread(NULL, 0,
        NlmThreadWrapper, (LPVOID)wrapper_data, 0, &thread_id);

    if (priority != eTP_Default) { /* adjust the priority */
      X_VERIFY ( SetThreadPriority(thread_handle, prio_value[priority]) );
    }
  }}
#endif

  /* is OK? */
  if (NlmThreadCompare(thread_handle, NULL_thread)) {
    X_ASSERT ( FALSE );
    wrapper_data->logName  = (CharPtr)Nlm_MemFree(wrapper_data->logName);
    wrapper_data->progName = (CharPtr)Nlm_MemFree(wrapper_data->progName);
    Nlm_Free(wrapper_data);
    NlmMutexLock(s_ThreadCounter_mutex);
    ASSERT(s_ThreadCounter > 0);
    s_ThreadCounter--;
    NlmMutexUnlock(s_ThreadCounter_mutex);
  }
#endif  /* NCBI_THREADS_AVAIL */

  return thread_handle;
}


NLM_EXTERN TNlmThread NlmThreadSelf(void)
{
#if   defined(SOLARIS_THREADS_AVAIL)
  return (TNlmThread) thr_self();
#elif defined(POSIX_THREADS_AVAIL)
  return (TNlmThread) pthread_self();
#elif defined(WIN32_THREADS_AVAIL)
  return (TNlmThread) thread_self;
#else /* default */
  return NULL_thread;
#endif
}


NLM_EXTERN Boolean NlmThreadCompare(TNlmThread thread1, TNlmThread thread2)
{
  return (Boolean)(thread1 == thread2);
}


NLM_EXTERN Int4 NlmThreadJoin(TNlmThread wait_for, VoidPtr *status)
{
  int err_code;
#if   defined(SOLARIS_THREADS_AVAIL)
  err_code = thr_join((thread_t)wait_for, NULL, status);
#elif defined(POSIX_THREADS_AVAIL)
  err_code = pthread_join((pthread_t)wait_for, status);
#elif defined(WIN32_THREADS_AVAIL)
  HANDLE thread = (HANDLE)wait_for;
  WaitForSingleObject(thread, INFINITE);
  if (GetExitCodeThread(thread, (LPDWORD)status)  &&
      (DWORD)*status != (DWORD)STILL_ACTIVE) {
    X_VERIFY ( CloseHandle(thread) );
    err_code = 0;
  }
  else {
    err_code = (int)GetLastError();
    if (err_code == 0)
      err_code = -1;
  }
#else /* default */
  err_code = -1;
#endif

  X_ASSERT ( err_code == 0 );
  return err_code ? -1 : 0;
}


NLM_EXTERN Int4 NlmThreadJoinAll(void)
{
#ifdef NCBI_THREADS_AVAIL
  /* wait for all threads to exit */
  while (s_ThreadCounter > 0)
    NlmSemaWait(s_ThreadCounter_sema);

  /* make sure the s_ThreadCounter_mutex become unlocked */
  if ( s_ThreadCounter_mutex ) {
    NlmMutexLock(s_ThreadCounter_mutex);
    NlmMutexUnlock(s_ThreadCounter_mutex);
  }

  ASSERT(s_ThreadCounter == 0);
  return (s_ThreadCounter == 0) ? 0 : -1;
#else
  return 0;
#endif
}


NLM_EXTERN void NlmThreadExit(VoidPtr status)
{
  /* execute all terminating functions registered by user */
  TOnExitArrayPtr arr_ptr = get_exit_arr();
  size_t size = (arr_ptr == NULL) ? 0 : arr_ptr->size;
  while ( size-- ) {
    if (arr_ptr->user[size].func != NULL)
      (*arr_ptr->user[size].func)( arr_ptr->user[size].arg );
  }

  if (arr_ptr != NULL) {
    Free(arr_ptr);
    set_exit_arr( (TOnExitArrayPtr)~0 );
  }

#ifdef NCBI_THREADS_AVAIL
  /* exiting from thread must clear "context" structure */
  {{
    const char *progName = (const char *)Nlm_GetAppProperty("ProgramName");
    Nlm_ReleaseAppContext();
    Nlm_MemFree((void *)progName);
  }}

  /* zeroing(and cleaning up) TLS data */
  s_TlsCleanupAll();

  /* indicate that one more thread has exited */
  NlmMutexLock(s_ThreadCounter_mutex);
  ASSERT(s_ThreadCounter > 0);
  s_ThreadCounter--;
  if (s_ThreadCounter == 0) {
    NlmSemaPost(s_ThreadCounter_sema);
  }
  NlmMutexUnlock(s_ThreadCounter_mutex);
#endif  /* NCBI_THREADS_AVAIL */

  /* exit the thread */
#if defined(SOLARIS_THREADS_AVAIL)
  thr_exit(status);
#elif defined(POSIX_THREADS_AVAIL)
  pthread_exit(status);
#elif defined(WIN32_THREADS_AVAIL)
  X_VERIFY ( CloseHandle(thread_self) );
  ExitThread((DWORD)status);
#endif
}


/********************************************************************/
/*                                                                  */
/* =========== Semaphore support for the thread API library ======= */
/*                                                                  */
/********************************************************************/

NLM_EXTERN TNlmSemaphore NlmSemaInit(Uint4 count)
{
#if defined(SOLARIS_THREADS_AVAIL)
  sema_t *theSemaphore = (sema_t *)Malloc(sizeof(sema_t));
  if (theSemaphore == NULL ||
     sema_init(theSemaphore, count, USYNC_THREAD, NULL) != 0)
    return NULL;

  return (TNlmSemaphore)theSemaphore;

#elif defined(POSIX_THREADS_AVAIL)
  psema_t *theSemaphore;
  if ( !(theSemaphore = (psema_t *)Malloc(sizeof(psema_t))) )
    return NULL;

  theSemaphore->count = count;

  if (pthread_mutex_init(&theSemaphore->mutex, NULL) != 0  ||
      pthread_cond_init (&theSemaphore->cond,  NULL) != 0)
    return NULL;
  
  return (TNlmSemaphore)theSemaphore;

#elif defined(WIN32_THREADS_AVAIL)
#define MAX_SEMAPHORE_COUNT 32000
  HANDLE theSemaphore;
  if((theSemaphore =
      CreateSemaphore(NULL, count, MAX_SEMAPHORE_COUNT, NULL)) != NULL)
    return (TNlmSemaphore)theSemaphore;
  else
    return NULL;

#else
  return NULL;
#endif
}


NLM_EXTERN Int4 NlmSemaDestroy(TNlmSemaphore theSemaphore)
{
#if defined(SOLARIS_THREADS_AVAIL)
  Int4 error = sema_destroy( (sema_t *)theSemaphore );
  Free( theSemaphore );
  return error;

#elif defined(POSIX_THREADS_AVAIL)
  psema_t *PSemaphore = (psema_t *)theSemaphore;

  if (pthread_mutex_destroy(&PSemaphore->mutex) != 0  ||
      pthread_cond_destroy (&PSemaphore->cond)  != 0)
    {
      Free( theSemaphore );
      return -1;
    }
  Free( theSemaphore );
  return 0;

#elif defined(WIN32_THREADS_AVAIL)
  return CloseHandle( (HANDLE)theSemaphore ) ? 0 : -1;

#else
  return 0;
#endif
}


NLM_EXTERN Int4 NlmSemaWait(TNlmSemaphore theSemaphore)
{
#if defined(SOLARIS_THREADS_AVAIL)
  return sema_wait((sema_t *)theSemaphore);

#elif defined(POSIX_THREADS_AVAIL)
  psema_t *PSemaphore = (psema_t *)theSemaphore;
  if (pthread_mutex_lock(&PSemaphore->mutex) != 0)
    return -1;

  if (PSemaphore->count > 0)
    PSemaphore->count--;
  else {
    do {
      pthread_cond_wait(&PSemaphore->cond, &PSemaphore->mutex);
    } while (PSemaphore->count == 0);
    PSemaphore->count--;
  }
  return pthread_mutex_unlock(&PSemaphore->mutex) ? -1 : 0;

#elif defined(WIN32_THREADS_AVAIL)
  return
    (WaitForSingleObject((HANDLE)theSemaphore, INFINITE) == WAIT_OBJECT_0) ?
    0 : -1;

#else
  return 0;
#endif
}


NLM_EXTERN Int4 NlmSemaTryWait(TNlmSemaphore theSemaphore)
{
#if defined(SOLARIS_THREADS_AVAIL)
  return sema_trywait((sema_t *)theSemaphore);

#elif defined (POSIX_THREADS_AVAIL)
  Int4 retval;
  psema_t *PSemaphore = (psema_t *)theSemaphore;
  if (pthread_mutex_lock(&PSemaphore->mutex) != 0)
    return -1;

  if (PSemaphore->count > 0) {
    PSemaphore->count--;
    retval = 0;
  }
  else {
    retval = -1;
  }
  return pthread_mutex_unlock(&PSemaphore->mutex) ? -1 : retval;

#elif defined(WIN32_THREADS_AVAIL)
  return
    (WaitForSingleObject((HANDLE)theSemaphore, 0) == WAIT_OBJECT_0) ?
    0 : -1;

#else
  return 0;
#endif
}


NLM_EXTERN Int4 NlmSemaPost(TNlmSemaphore theSemaphore)
{
#if defined(SOLARIS_THREADS_AVAIL)
  return sema_post((sema_t *)theSemaphore);

#elif defined (POSIX_THREADS_AVAIL)
  psema_t *PSemaphore = (psema_t *)theSemaphore;
  if (pthread_mutex_lock(&PSemaphore->mutex) != 0)
    return -1;

  PSemaphore->count++;
  if (pthread_cond_signal(&PSemaphore->cond) != 0) {
    PSemaphore->count--;
    return -1;
  }
  return pthread_mutex_unlock(&PSemaphore->mutex) ? -1 : 0;

#elif defined(WIN32_THREADS_AVAIL)
  return ReleaseSemaphore((HANDLE)theSemaphore, 1, NULL) ? 0 : -1;

#else
  return 0;
#endif
}



/********************************************************************/
/*                                                                  */
/* === Semaphore-like object to keep read/write syncronization ==== */
/*                                                                  */
/********************************************************************/


/* [POSIX and WIN32]  Nested locking policy:
 *   W after R -- never allowed;
 *   W after W -- allowed if the W-lock is owned by the same thread;
 *   R after W -- allowed if the W-lock is owned by the same thread (and,
 *                then this R is treated as if it was W);
 *   R after R -- always allowed (unless there already was a "R after W"
 *                performed in another thread)
 *   U after W -- only if the W-lock is owned by the same thread
 */


#define RW_UNKNOWN_OWNER ((TNlmThread)(~0))


/******************************************************************
 * DEBUG-only feature -- keep track of the RW-locks
 */

#if defined(POSIX_THREADS_AVAIL)  ||  defined(WIN32_THREADS_AVAIL)
#  if defined(_DEBUG_HARD)
#    define RW_HISTORY
#  endif
#endif

#if defined(RW_HISTORY)

#  define RW_HISTORY_SIZE 256
#  define RW_HISTORY_DECL SRWHistoryArray rw_history;
#  define RW_HISTORY_UPDATE(RW,thread,file,line) \
      s_RWHistoryUpdate(RW,thread,file,line)

typedef struct {
    const char* file;
    int         line;
    int         readers;  /* "readers" value *after* the change */
    TNlmThread  thread;   /* ID of the thread caused this change */
} SRWHistory;

typedef struct {
    size_t     size;
    SRWHistory elem[RW_HISTORY_SIZE];
} SRWHistoryArray;

#else   /* def RW_HISTORY */

#  define RW_HISTORY_DECL
#  define RW_HISTORY_UPDATE(RW,thread,file,line) ((void)0)

#endif  /* else RW_HISTORY */



/******************************************************************
 * Internal structure of RW-locks
 */

#if defined(SOLARIS_THREADS_AVAIL)

typedef struct TNlmRWlockTag {
    rwlock_t rwlock; /* native Solaris-style RW-lock */
} structRWlock;

#elif defined(POSIX_THREADS_AVAIL)

typedef struct TNlmRWlockTag {
    /* if W-locked -- keeps the lock owner;
     * also, exclusively for the reason of being easier to debug: 
     *   if R-locked -- may(but may not!) keep the owner of one of the R-locks,
     *   if Unlocked -- keeps what it kept just before the last unlock
     */
    TNlmThread owner;

    /* < 0 -- # of locks by writers or nested writers/readers
     * = 0 -- not locked
     * > 0 -- # of locks by readers
     */
    Int4 readers;

    /* protects members of this RW from being modified by more than one thread
     * at once
     */
    pthread_mutex_t  mutex;

    pthread_cond_t   cond_r;  /* condition variable for readers  */
    pthread_cond_t   cond_w;  /* condition variable for writers  */

    RW_HISTORY_DECL  /* for DEBUG purposes only! */
} structRWlock;

#elif defined(WIN32_THREADS_AVAIL)

typedef struct TNlmRWlockTag {
    TNlmThread owner;   /* see comment above (for POSIX_THREADS_AVAIL) */
    Int4       readers; /* see comment above (for POSIX_THREADS_AVAIL) */
    HANDLE     mutex;   /* mutex to protect "readers" variable */
    HANDLE     sema_rw; /* semaphore for readers and writers   */
    HANDLE     sema_w;  /* semaphore for writers               */
    RW_HISTORY_DECL     /* for DEBUG purposes only! */
} structRWlock;

#else
#  define NO_RWLOCK
#endif


static char* s_RWprintout(TNlmRWlock RW, int/*bool*/ do_mutex);


#if defined(RW_HISTORY)
static void s_RWHistoryUpdate
(TNlmRWlock  RW,
 TNlmThread  thread,
 const char* file,
 int         line)
{
    SRWHistoryArray* arr = &RW->rw_history;
    SRWHistory*      rec = arr->elem + (arr->size % RW_HISTORY_SIZE);

    rec->file    = file;
    rec->line    = line;
    rec->readers = RW->readers;
    rec->thread  = thread;
    arr->size++;
#if defined(RW_TRACE)
    if (RW->readers == 0  ||  (arr->size % RW_HISTORY_SIZE) == 0) {
        char* str = s_RWprintout(RW, 0/*no_mutex*/);
        fprintf(stderr, "%s", str);
        free(str);
    }
#endif
    if (RW->readers == 0) {
        arr->size = 0;
    }
}
#endif


/******************************************************************
 * RW-lock functions
 */

NLM_EXTERN TNlmRWlock NlmRWinit(void)
{
#ifdef NO_RWLOCK
  return NULL;
#else

  int err_code = 0;
  TNlmRWlock RW;
  if ((RW = (TNlmRWlock) Calloc(1, sizeof(structRWlock))) == NULL)
      return NULL;

#if defined(SOLARIS_THREADS_AVAIL)
  err_code = rwlock_init(&RW->rwlock, USYNC_THREAD, NULL);
    
#elif defined(POSIX_THREADS_AVAIL)
  RW->readers = 0;
  err_code = pthread_mutex_init(&RW->mutex, NULL);
  if (err_code == 0) {
      err_code = pthread_cond_init(&RW->cond_r, NULL);
      if (err_code == 0) {
        err_code = pthread_cond_init(&RW->cond_w, NULL);
        if (err_code != 0) { /* error initializing "cond_w" -- clean */
          X_VERIFY ( !pthread_cond_destroy (&RW->cond_r)  );
          X_VERIFY ( !pthread_mutex_destroy(&RW->mutex )  );
        }
      } else { /* error initializing "cond_r" -- clean */
        X_VERIFY ( !pthread_mutex_destroy(&RW->mutex) );
      }
  }

#elif defined (WIN32_THREADS_AVAIL)
  RW->readers = 0;
  if ( !(RW->mutex = CreateMutex(NULL, FALSE, NULL)) ) {
    err_code = 1;
  } else {
    if ( !(RW->sema_rw = CreateSemaphore(NULL, 1, 1, NULL)) ) {
      CloseHandle(RW->mutex);
      err_code = 2;
    } else {
      if ( !(RW->sema_w = CreateSemaphore(NULL, 1, 1, NULL)) ) {
        CloseHandle(RW->sema_rw);
        CloseHandle(RW->mutex);
        err_code = 3;
      }
    }
  }
#endif

  if (err_code != 0) {
    X_ASSERT ( FALSE );
    Free(RW);
    return NULL;
  }

  return RW;
#endif /* else!NO_RWLOCK */
}


NLM_EXTERN Int4 NlmRWdestroy(TNlmRWlock RW)
{
#ifdef NO_RWLOCK
  return 0;
#else

  int err_code = 0;

#if defined(SOLARIS_THREADS_AVAIL)
  err_code = rwlock_destroy(&RW->rwlock);

#elif defined (POSIX_THREADS_AVAIL)
  ASSERT(RW->readers == 0);
  err_code = pthread_mutex_destroy(&RW->mutex);
  if (err_code == 0) {
    err_code = pthread_cond_destroy(&RW->cond_r);
    if (err_code == 0) {
      err_code = pthread_cond_destroy(&RW->cond_w);
    }
  }

#elif defined(WIN32_THREADS_AVAIL)
  ASSERT(RW->readers == 0);
  if ( !CloseHandle(RW->mutex) )
    err_code |= 0x1;
  if ( !CloseHandle(RW->sema_rw) )
    err_code |= 0x2;
  if ( !CloseHandle(RW->sema_w) )
    err_code |= 0x4;
#endif

  X_ASSERT(err_code == 0);
  Free(RW);
  return err_code ? -1 : 0;
#endif /* else!NO_RWLOCK */
}


NLM_EXTERN Int4 NlmRWrdlockEx(TNlmRWlock RW,
                              const char* file, int line)
{
#if defined(NO_RWLOCK)
    return 0;

#elif defined(SOLARIS_THREADS_AVAIL)
    int err_code = rw_rdlock(&RW->rwlock);
    X_ASSERT(err_code == 0);
    return err_code ? -1 : 0;

#elif defined(POSIX_THREADS_AVAIL)
    TNlmThread this_thread = NlmThreadSelf();

    /* protect members of "*RW" from being changed by other threads */
    int err_code = pthread_mutex_lock(&RW->mutex);
    if (err_code != 0) {
        X_ASSERT(0);
        return -1;
    }

    /* obtain R-lock */
    if (RW->readers < 0  &&  RW->owner == this_thread) {
        /* W-locked by this the same thread already */
        RW->readers--;  /* treate it as a W-lock then */
    } else if (RW->readers <= 0) {
        /* Unlocked or W-locked by another thread */
        /* if W-locked by another thread -- wait here until it's Unlocked */
        while (RW->readers < 0) {
            pthread_cond_wait(&RW->cond_r, &RW->mutex);
        }
        /* ...not locked, or it can be R-locked by other thread by now */
        ASSERT(RW->readers >= 0);
        RW->readers++;
        RW->owner = this_thread;
    } else {
        /* R-locked already -- increment # of R-locks;  may reset the owner */
        RW->readers++;
        if (RW->owner == RW_UNKNOWN_OWNER) {
            RW->owner = this_thread;
        }
    }

    /* release the "*RW" protective mutex */
    RW_HISTORY_UPDATE(RW, this_thread, file, line); /* DEBUG */
    X_VERIFY(!pthread_mutex_unlock(&RW->mutex));
    return 0;

#elif defined(WIN32_THREADS_AVAIL)
    TNlmThread this_thread = NlmThreadSelf();

    /* protect members of "*RW" from being changed by other threads */
    if (WaitForSingleObject(RW->mutex, INFINITE) != WAIT_OBJECT_0) {
        X_ASSERT(0);
        return -1;
    }

    /* treate R-lock as W-lock if W-locked by this the same thread already */
    if (RW->readers < 0  &&  RW->owner == this_thread) {
        RW->readers--;
        RW_HISTORY_UPDATE(RW, this_thread, file, line); /* DEBUG */
        X_VERIFY(ReleaseMutex(RW->mutex));
        return 0;
    }

    /* wait if W-locked by another thread */
    if (RW->readers < 0) {
        HANDLE obj[2];
        DWORD  wait_res;
        obj[0] = RW->mutex;
        obj[1] = RW->sema_rw;
        X_VERIFY(ReleaseMutex(RW->mutex));/* allow other threads to do stuff */
        wait_res = WaitForMultipleObjects(2, obj, TRUE, INFINITE);
        if (WAIT_OBJECT_0 <= wait_res  &&  wait_res < WAIT_OBJECT_0 + 2) {
            LONG prev_sema;
            X_VERIFY(ReleaseSemaphore(RW->sema_rw, 1, &prev_sema));
            X_ASSERT(prev_sema == 0);
        } else {
            X_ASSERT(0);
            return -1;
        }
    }
    X_ASSERT(RW->readers >= 0);

    /* first reader:  lock against writers and assign the owner */
    if (RW->readers == 0) {
        if (WaitForSingleObject(RW->sema_w, 0) != WAIT_OBJECT_0) {
            X_VERIFY(ReleaseMutex(RW->mutex));
            X_ASSERT(0);
            return -1;
        }
        RW->owner = this_thread;
    }

    /* increment the "readers" counter */
    RW->readers++;

    /* release the "*RW" protective mutex */
    RW_HISTORY_UPDATE(RW, this_thread, file, line); /* DEBUG */
    X_VERIFY(ReleaseMutex(RW->mutex));
    return 0;
#endif
}


NLM_EXTERN Int4 NlmRWwrlockEx(TNlmRWlock RW,
                              const char* file, int line)
{
#if defined(NO_RWLOCK)
    return 0;

#elif defined(SOLARIS_THREADS_AVAIL)
    int err_code = rw_wrlock(&RW->rwlock);
    X_ASSERT(err_code == 0);
    return err_code ? -1 : 0;

#elif defined(POSIX_THREADS_AVAIL)
    TNlmThread this_thread = NlmThreadSelf();

    /* protect members of "*RW" from being changed by other threads */
    int err_code = pthread_mutex_lock(&RW->mutex);
    if (err_code != 0) {
        X_ASSERT(0);
        return -1;
    }

    /* obtain W-lock */
    if (RW->readers < 0  &&  RW->owner == this_thread) {
        /* W-locked by this the same thread already */
        RW->readers--;
    } else if (RW->readers == 0  ||  RW->owner != this_thread) {
        /* Unlocked or RW-locked by another thread(s) */
        /* RW-locked by another thread(s) -- wait here until it is Unlocked */
        while (RW->readers != 0) {
            pthread_cond_wait(&RW->cond_w, &RW->mutex);
        }
        /* ...not locked now */
        RW->readers = -1;
        RW->owner   = this_thread;
    } else {
        /* already R-locked by this thread (not always detectable) */
        ASSERT(0);
        err_code = -1;
    }

    /* release the "*RW" protective mutex */
    RW_HISTORY_UPDATE(RW, this_thread, file, line); /* DEBUG */
    X_VERIFY(!pthread_mutex_unlock(&RW->mutex));
    return 0;

#elif defined(WIN32_THREADS_AVAIL)
    TNlmThread this_thread = NlmThreadSelf();
    HANDLE obj[3];
    DWORD  wait_res;

    /* protect members of "*RW" from being changed by other threads */
    if (WaitForSingleObject(RW->mutex, INFINITE) != WAIT_OBJECT_0) {
        X_ASSERT(0);
        return -1;
    }

    /* already W-locked by this the same thread already */
    if (RW->readers < 0  &&  RW->owner == this_thread) {
        RW->readers--;
        RW_HISTORY_UPDATE(RW, this_thread, file, line); /* DEBUG */
        X_VERIFY(ReleaseMutex(RW->mutex));
        return 0;
    }

    /* array of sync-objects to wait for */
    /* (on success, both "sema_rw" and "sema_w" will be locked, so that */
    /*  no other thread can acquire either read or write lock) */
    obj[0] = RW->sema_rw;
    obj[1] = RW->sema_w;
    obj[2] = RW->mutex;

    /* not locked by anybody:  sure lock 'n' go */
    if (RW->readers == 0) {
        wait_res = WaitForMultipleObjects(2, obj, TRUE,0);
        if (wait_res < WAIT_OBJECT_0  ||  WAIT_OBJECT_0 + 2 <= wait_res) {
            X_VERIFY(ReleaseMutex(RW->mutex));
            X_ASSERT(0);
            return -1;
        }
        RW->readers = -1;
        RW->owner = this_thread;
        RW_HISTORY_UPDATE(RW, this_thread, file, line); /* DEBUG */
        X_VERIFY(ReleaseMutex(RW->mutex));
        return 0;
    }

    /* RW-locked by other thread(s):  wait until all locks are free */
    X_VERIFY(ReleaseMutex(RW->mutex)); /* allow other threads to do stuff */
    wait_res = WaitForMultipleObjects(3, obj, TRUE, INFINITE);
    if (wait_res < WAIT_OBJECT_0  ||  WAIT_OBJECT_0 + 3 <= wait_res) {
        X_ASSERT(0);
        return -1;
    }
    X_ASSERT(RW->readers == 0);
    RW->readers = -1;
    RW->owner = this_thread;
    RW_HISTORY_UPDATE(RW, this_thread, file, line); /* DEBUG */
    X_VERIFY(ReleaseMutex(RW->mutex));
    return 0;
#endif
}


NLM_EXTERN Int4 NlmRWunlockEx(TNlmRWlock RW,
                              const char* file, int line)
{
#if !defined (NO_RWLOCK)
#  ifdef _TRACE_HARD
    int dummy = fprintf(stderr, "%ld\n", (long) RW->readers);
#  endif
#endif


#if defined(NO_RWLOCK)
    return 0;

#elif defined(SOLARIS_THREADS_AVAIL)
    int err_code =  rw_unlock(&RW->rwlock);
    X_ASSERT(err_code == 0);
    return err_code ? -1 : 0;

#elif defined (POSIX_THREADS_AVAIL)
    TNlmThread this_thread = NlmThreadSelf();

    /* protect members of "*RW" from being changed by other threads */
    int err_code = pthread_mutex_lock(&RW->mutex);
    if (err_code != 0) {
        X_ASSERT(0);
        return -1;
    }

    /* catch the illegal uses of RW-unlock */
    ASSERT(RW->readers != 0);
    if (RW->readers < 0  &&  RW->owner != this_thread) {
        /* attempted to Unlock a W-lock held by another thread -- trouble! */
        X_VERIFY(!pthread_mutex_unlock(&RW->mutex));
        ASSERT(0);
        return -1;
    }

    /* do unlock (R or W) */
    if (RW->readers == -1) {
        /* only one W-lock left -- allow for both R- and W-locks */
        if ((err_code = pthread_cond_broadcast(&RW->cond_r)) == 0)
            err_code = pthread_cond_signal(&RW->cond_w);
        if (err_code == 0)
            RW->readers = 0;
    } else if (RW->readers < 1) {
        /* nested W-lock -- just decrement the # of W-locks left */
        RW->readers++;
    } else if (RW->readers == 1) {
        /* one R-lock left - allow for W-locks (R-locks are allowed already) */
        if ((err_code = pthread_cond_signal(&RW->cond_w)) == 0)
            RW->readers = 0;
    } else {
        /* nested R-lock -- decrement the # of R-locks left; loose the owner */
        RW->owner = RW_UNKNOWN_OWNER;
        RW->readers--;
    }

    /* release the "*RW" protective mutex */
    RW_HISTORY_UPDATE(RW, this_thread, file, line); /* DEBUG */
    X_VERIFY(!pthread_mutex_unlock(&RW->mutex));
    ASSERT(err_code == 0);
    return err_code ? -1 : 0;

#elif defined(WIN32_THREADS_AVAIL)
    TNlmThread this_thread = NlmThreadSelf();

    /* protect members of "*RW" from being changed by other threads */
    if (WaitForSingleObject(RW->mutex, INFINITE) != WAIT_OBJECT_0) {
        X_ASSERT(0);
        return -1;
    }

    /* catch the illegal uses of RW-unlock */
    ASSERT(RW->readers != 0);
    if (RW->readers < 0  &&  RW->owner != this_thread) {
        /* attempted to Unlock a W-lock held by another thread -- trouble! */
        X_VERIFY(ReleaseMutex(RW->mutex));
        ASSERT(0);
        return -1;
    }

    /* do unlock (R or W) */
    if (RW->readers == -1  ||  RW->readers == 1) {
        /* only one R or W lock left -- unlock everything */
        LONG prev_sema;
        if (RW->readers == -1) {
            X_VERIFY(ReleaseSemaphore(RW->sema_rw, 1, &prev_sema));
            X_ASSERT(prev_sema == 0);
        }
        X_VERIFY(ReleaseSemaphore(RW->sema_w,  1, &prev_sema));
        X_ASSERT(prev_sema == 0);
        RW->readers = 0;
    } else {
        /* more than one reader or writer left -- just update the counter */
        X_ASSERT(RW->readers < -1  ||  1 < RW->readers);
        if (RW->readers > 0)
            RW->readers--;
        else
            RW->readers++;
    }

    /* release the "*RW" protective mutex */
    RW_HISTORY_UPDATE(RW, this_thread, file, line); /* DEBUG */
    X_VERIFY(ReleaseMutex(RW->mutex));
    return 0;
#endif
}


NLM_EXTERN Int4 NlmRWtryrdlockEx(TNlmRWlock RW,
                                 const char* file, int line)
{
#if defined(NO_RWLOCK)
    return 0;

#elif defined(SOLARIS_THREADS_AVAIL)
    int err_code = rw_tryrdlock(&RW->rwlock);
    if (err_code != 0  &&  err_code != EBUSY) {
        ASSERT(0);
        return -1;
    }
    return err_code ? -1 : 0;

#elif defined(POSIX_THREADS_AVAIL)
    int err_code;
    TNlmThread this_thread = NlmThreadSelf();

    /* quick check -- dont care to lock here */
    if (RW->readers < 0  &&  RW->owner != this_thread)
        return -1;

    /* protect members of "*RW" from being changed by other threads */
    err_code = pthread_mutex_lock(&RW->mutex);
    if (err_code != 0) {
        X_ASSERT(0);
        return -1;
    }

    /* trying to obtain R-lock */
    if (RW->readers == 0) {
        /* Unlocked -- do R-lock, store ownership to help catch "W after R" */
        RW->readers = 1;
        RW->owner   = this_thread;
    } else if (RW->readers > 0) {
        /* R-locked already -- increment # of R-locks;  may reset the owner */
        RW->readers++;
        if (RW->owner == RW_UNKNOWN_OWNER)
            RW->owner = this_thread;
    } else {
        /* W-locked already -- check if locked by the same thread (nested W) */
        if (RW->owner == this_thread)
            RW->readers--;  /* nested locking (interpret R as W here) */
        else
            err_code = -1;  /* cannot R-lock instantaneously */
    }

    /* release the "*RW" protective mutex */
    if (err_code == 0) {
        RW_HISTORY_UPDATE(RW, this_thread, file, line); /* DEBUG */
    }
    X_VERIFY(!pthread_mutex_unlock(&RW->mutex));
    return err_code;

#elif defined(WIN32_THREADS_AVAIL)
    int err_code;
    TNlmThread this_thread = NlmThreadSelf();

    /* quick check -- dont care to lock here */
    if (RW->readers < 0  &&  RW->owner != this_thread)
        return -1;

    /* protect members of "*RW" from being changed by other threads */
    if (WaitForSingleObject(RW->mutex, INFINITE) != WAIT_OBJECT_0) {
        X_ASSERT(0);
        return -1;
    }

    /* trying to obtain R-lock */
    err_code = 0;

    if (RW->readers < 0) {
        /* W-locked... */
        if (RW->owner == this_thread)
            RW->readers--; /* ...by this thread -- nested;  interpret R as W */
        else
            err_code = -1; /* ...by another thread -- cannot obtain lock now */
    } else if (RW->readers > 0) {
        /* R-locked */
        RW->readers++;
    } else {
        /* not locked -- lock the writer semaphore */
        if (WaitForSingleObject(RW->sema_w, 0) == WAIT_OBJECT_0) {
            RW->readers = 1;
            RW->owner = this_thread;
        } else {
            ASSERT(0);
            err_code = -1;
        }
    }

    /* release the "*RW" protective mutex */
    if (err_code == 0) {
        RW_HISTORY_UPDATE(RW, this_thread, file, line); /* DEBUG */
    }
    X_VERIFY(ReleaseMutex(RW->mutex));
    return err_code;
#endif
}


NLM_EXTERN Int4 NlmRWtrywrlockEx(TNlmRWlock RW,
                                 const char* file, int line)
{
#if defined(NO_RWLOCK)
    return 0;

#elif defined(SOLARIS_THREADS_AVAIL)
    int err_code = rw_trywrlock(&RW->rwlock);
    if (err_code != 0  &&  err_code != EBUSY) {
        ASSERT(0);
        return -1;
    }
    return err_code ? -1 : 0;

#elif defined(POSIX_THREADS_AVAIL)
    int err_code;
    TNlmThread this_thread = NlmThreadSelf();

    /* quick check -- dont care to lock here */
    if (RW->readers > 0  ||
        (RW->readers < 0  &&  RW->owner != this_thread))
        return -1;

    /* protect members of "*RW" from being changed by other threads */
    err_code = pthread_mutex_lock(&RW->mutex);
    if (err_code != 0) {
        X_ASSERT(0);
        return err_code;
    }

    /* trying to obtain W-lock */
    if (RW->readers == 0) {
        /* Unlocked -- do W-lock, store ownership */
        RW->readers = -1;
        RW->owner   = this_thread;
    } else if (RW->readers > 0) {
        /* R-locked already -- cannot W-lock right away */
        err_code = -1;
    } else {
        /* W-locked already -- check if locked by the same thread (nested W) */
        if (RW->owner == this_thread)
            RW->readers--;  /* nested locking (interpret R as W here) */
        else
            err_code = -1;  /* cannot W-lock instantaneously */
    }

    /* release the "*RW" protective mutex */
    if (err_code == 0) {
        RW_HISTORY_UPDATE(RW, this_thread, file, line); /* DEBUG */
    }
    X_VERIFY(!pthread_mutex_unlock(&RW->mutex));
    return err_code;

#elif defined(WIN32_THREADS_AVAIL)
    int err_code;
    TNlmThread this_thread = NlmThreadSelf();

    /* quick check -- dont care to lock here */
    if (RW->readers > 0  ||
        (RW->readers < 0  &&  RW->owner != this_thread))
        return -1;

    /* protect members of "*RW" from being changed by other threads */
    if (WaitForSingleObject(RW->mutex, INFINITE) != WAIT_OBJECT_0) {
        X_ASSERT(0);
        return -1;
    }

    /* trying to obtain W-lock */
    err_code = 0;

    if (RW->readers < 0) {
        /* W-locked... */
        if (RW->owner == this_thread)
            RW->readers--; /* ...by this thread -- nested */
        else
            err_code = -1; /* ...by another thread -- cannot obtain lock now */
    } else if (RW->readers > 0) {
        /* R-locked -- cannot obtain lock now */
        err_code = -1;
    } else {
        /* not locked -- lock writer and reader semaphores */
        HANDLE obj[2];
        DWORD  wait_res;
        obj[0] = RW->sema_rw;
        obj[1] = RW->sema_w;
        wait_res = WaitForMultipleObjects(2, obj, TRUE, 0);
        if (WAIT_OBJECT_0 <= wait_res  &&  wait_res < WAIT_OBJECT_0 + 2) {
            RW->readers = -1;
            RW->owner = this_thread;
        } else {
            X_ASSERT(0);
            err_code = -1;
        }
    }

    /* release the "*RW" protective mutex */
    if (err_code == 0) {
        RW_HISTORY_UPDATE(RW, this_thread, file, line); /* DEBUG */
    }
    X_VERIFY(ReleaseMutex(RW->mutex));
    return err_code;
#endif
}


NLM_EXTERN char* NlmRWprintout(TNlmRWlock RW)
{
    return s_RWprintout(RW, 1/*true*/);
}


static char* s_RWprintout(TNlmRWlock RW, int/*bool*/ do_mutex)
{
    char* str;

    /* Lock the protective mutex */
    if ( do_mutex ) {
#if defined(POSIX_THREADS_AVAIL)
        X_VERIFY(pthread_mutex_lock(&RW->mutex) == 0);
#elif defined(WIN32_THREADS_AVAIL)
        X_VERIFY(WaitForSingleObject(RW->mutex, INFINITE) == WAIT_OBJECT_0);
#endif
    }

    /* Allocate string and print to it */
#if defined(RW_HISTORY)
    if (RW->rw_history.size == 0) {
        ASSERT(RW->readers == 0);
        str = (char*) malloc(64);
        sprintf(str, "#%p UNLOCKED\n", (void*) RW);
    } else {
        /* <file>:<line>:  #<RW-lock>  <readers>  @<thread> */
        SRWHistoryArray* arr = &RW->rw_history;
        size_t len;
        size_t i, i_start, i_stop;
        char*  s;

        if (arr->size <= RW_HISTORY_SIZE) {
            i_start = 0;
            i_stop  = arr->size - 1;
        } else {
            i_start = arr->size;
            i_stop  = i_start + RW_HISTORY_SIZE - 1;
        }

        /* roughly count the message length */
        len = 0;
        for (i = i_start;  i <= i_stop;  i++) {
            SRWHistory* rec = arr->elem + (i % RW_HISTORY_SIZE);
            len += 110;
            len += rec->file ? strlen(rec->file) : 0;
        }
        /* allocate and print */
        str = (char*) malloc(len);
        s = str;
        for (i = i_start;  i <= i_stop;  i++) {
            SRWHistory* rec = arr->elem + (i % RW_HISTORY_SIZE);
            if ( rec->file ) {
                sprintf(s, "%s:%5d: (%3lu) #%p %-+4d @%p\n",
                        rec->file, rec->line, (unsigned long) i,
                        (void*) RW, (int) rec->readers, (void*) rec->thread);
            } else {
                sprintf(s, "line %5d: (%3lu) #%p %-+4d @%p\n",
                        rec->line, (unsigned long) i,
                        (void*) RW, (int) rec->readers, (void*) rec->thread);
            }
            s += strlen(s);
        } 
        *s = '\n';  s++;  *s = '\0';
        ASSERT(strlen(str) < len);
    }
#elif defined(POSIX_THREADS_AVAIL)  ||  defined(WIN32_THREADS_AVAIL)
    str = (char*) malloc(128);
    sprintf(str, "#%p:  readers =%4d,  owner = %p\n",
            (void*) RW, (int) RW->readers,  (void*) RW->owner);
#else
    str = Nlm_StringSave("NlmRWprintout():  "
                         "NOT IMPLEMENTED for Solaris native threads!\n");
#endif

    /* Unlock the protective mutex */
    if ( do_mutex ) {
#if defined(POSIX_THREADS_AVAIL)
        X_VERIFY(!pthread_mutex_unlock(&RW->mutex));
#elif defined(WIN32_THREADS_AVAIL)
        X_VERIFY(ReleaseMutex(RW->mutex));
#endif
    }

    /* Done */
    return str;
}



/********************************************************************
 * === MUTEXes ====================================================
 ********************************************************************/

#if defined(SOLARIS_THREADS_AVAIL)  ||  defined(POSIX_THREADS_AVAIL)  ||  defined(WIN32_THREADS_AVAIL)

struct TNlmMutexTag {
  MUTEX_T    mutex;
  TNlmThread owner;
  long       counter;
} structMutex;

static MUTEX_T s_Init_mutex MUTEX_T_INIT;  /* mutex init protector */

#else

#define NO_MUTEX
#endif


NLM_EXTERN TNlmMutex NlmMutexInit(TNlmMutexPtr theMutexPtr)
{
  if (theMutexPtr == NULL) {
    X_ASSERT ( FALSE );
    return NULL;
  }

  if (*theMutexPtr != NULL)
    return *theMutexPtr;

#if defined(SOLARIS_THREADS_AVAIL)
  {{
    X_VERIFY ( !mutex_lock(&s_Init_mutex) );
    if (*theMutexPtr == NULL) {
      TNlmMutex theMutex = (TNlmMutex) Calloc(1, sizeof(structMutex));
      if (theMutex &&  mutex_init(&theMutex->mutex, USYNC_THREAD, NULL) != 0) {
        Free( theMutex );
        theMutex = NULL;
      }
      if ( theMutex ) {
        theMutex->owner = NlmThreadSelf();
        theMutex->counter = 0;
      }
      *theMutexPtr = theMutex;
    }
    X_VERIFY ( !mutex_unlock(&s_Init_mutex) );
  }}

#elif defined(POSIX_THREADS_AVAIL)
  {{
    X_VERIFY ( !pthread_mutex_lock(&s_Init_mutex) );
    if (*theMutexPtr == NULL) {
      TNlmMutex theMutex = (TNlmMutex) Calloc(1, sizeof(structMutex));
      if (theMutex  &&  pthread_mutex_init(&theMutex->mutex, NULL) != 0) {
        Free( theMutex );
        theMutex = NULL;
      }
      if ( theMutex ) {
        theMutex->owner = NlmThreadSelf();
        theMutex->counter = 0;
      }
      *theMutexPtr = theMutex;
    }
    X_VERIFY ( !pthread_mutex_unlock(&s_Init_mutex) );
  }}

#elif defined(WIN32_THREADS_AVAIL)
  {{
    if (s_Init_mutex == NULL) {
      HANDLE x_Init_mutex = CreateMutex(NULL, FALSE, NULL);
      if ( x_Init_mutex )
        s_Init_mutex = x_Init_mutex;
      else
        X_ASSERT ( s_Init_mutex  &&  GetLastError() == ERROR_ALREADY_EXISTS );
    }
    if (WaitForSingleObject(s_Init_mutex, INFINITE) != WAIT_OBJECT_0)
      return NULL;

    if (*theMutexPtr == NULL) {
      TNlmMutex theMutex = (TNlmMutex) Calloc(1, sizeof(structMutex));
      if (theMutex  &&  !(theMutex->mutex = CreateMutex(NULL,FALSE,NULL))) {
        Free(theMutex);
        theMutex = NULL;
      }
      *theMutexPtr = theMutex;
    }

    ReleaseMutex(s_Init_mutex);
  }}

#else
  *theMutexPtr = (TNlmMutex)(~0);
#endif

  return *theMutexPtr;
}


static Int4 s_MutexLock(TNlmMutex theMutex, Nlm_Boolean no_block)
{
  Int4 err_code = 0;

#ifndef NO_MUTEX
  TNlmThread owner = NlmThreadSelf();

  if (theMutex == NULL) {
    X_ASSERT ( FALSE );
    return -1;
  }

  if (theMutex->counter > 0) {
    if ( NlmThreadCompare(theMutex->owner, owner) )
      { /* protect from deadlock under Solaris || POSIX */
        theMutex->counter++;
        return 0;
      }
    else if ( no_block )
      return -1;
  }

#ifdef _TRACE_HARD
  fprintf(stderr, "Mutex lock , IN = [%2ld :%8ld]\n",
          (long)owner, (long)theMutex);
#endif

  if ( no_block )
    {
#if defined(SOLARIS_THREADS_AVAIL)
      err_code = mutex_trylock(&theMutex->mutex);
#elif defined(POSIX_THREADS_AVAIL)
      err_code = pthread_mutex_trylock(&theMutex->mutex);
#elif defined(WIN32_THREADS_AVAIL)
      {{
        DWORD x_err_code = WaitForSingleObject(theMutex->mutex, 1);
        X_ASSERT ( x_err_code != WAIT_ABANDONED );
        if (x_err_code == WAIT_TIMEOUT)
          err_code = -1;
      }}
#endif
    }
  else
    {
#if defined(SOLARIS_THREADS_AVAIL)
      err_code = mutex_lock(&theMutex->mutex);
#elif defined(POSIX_THREADS_AVAIL)
      err_code = pthread_mutex_lock(&theMutex->mutex);
#elif defined(WIN32_THREADS_AVAIL)
      {{
        DWORD x_err_code = WaitForSingleObject(theMutex->mutex, INFINITE);
        X_ASSERT ( x_err_code == WAIT_OBJECT_0 );
        if (x_err_code == WAIT_TIMEOUT)
          err_code = -1;
      }}
#endif
    }

#ifdef _TRACE_HARD
  fprintf(stderr, "Mutex lock ,OUT = [%2ld :%8ld]\n",
          (long)owner, (long)theMutex);
#endif

  if (err_code == 0) {
    /* surprise! -- these two can be swapped by optimizer (see R6.25) */
    theMutex->owner   = owner;
    theMutex->counter = 1;
  }
#endif /* ndef NO_MUTEX */

  X_ASSERT ( no_block  ||  err_code == 0 );
  return err_code ? -1 : 0;
}


NLM_EXTERN Int4 NlmMutexLock(TNlmMutex theMutex)
{
  return s_MutexLock(theMutex, FALSE);
}


NLM_EXTERN Int4 NlmMutexTryLock(TNlmMutex theMutex)
{
  return s_MutexLock(theMutex, TRUE);
}


NLM_EXTERN Int4 NlmMutexLockEx(TNlmMutexPtr theMutexPtr)
{
  Int4 err_code = 0;
#ifndef NO_MUTEX
  if (!theMutexPtr  ||  (!*theMutexPtr  &&  !NlmMutexInit(theMutexPtr))) {
    X_ASSERT ( FALSE );
    err_code = -1;
  }
  else
    err_code = s_MutexLock(*theMutexPtr, FALSE);
#endif
  return err_code;
}


NLM_EXTERN Int4 NlmMutexTryLockEx(TNlmMutexPtr theMutexPtr)
{
  Int4 err_code = 0;
#ifndef NO_MUTEX
  if (!theMutexPtr  ||  (!*theMutexPtr  &&  !NlmMutexInit(theMutexPtr))) {
    X_ASSERT ( FALSE );
    err_code = -1;
  }
  else
    err_code = s_MutexLock(*theMutexPtr, TRUE);
#endif
  return err_code;
}


NLM_EXTERN Int4 NlmMutexUnlock(TNlmMutex theMutex)
{
  Int4 err_code = 0;

#ifndef NO_MUTEX
  TNlmThread this_thread = NlmThreadSelf();

  if ( !theMutex ) {
    X_ASSERT ( FALSE );
    return 0;
  }

  if (!NlmThreadCompare(theMutex->owner, this_thread)  ||
      theMutex->counter == 0) {
    X_ASSERT ( FALSE );
    return -1;
  }
  
  if (--theMutex->counter != 0)
    return 0;

  theMutex->owner = 0;
#if defined(SOLARIS_THREADS_AVAIL)
  err_code = mutex_unlock(&theMutex->mutex);
#elif defined(POSIX_THREADS_AVAIL)
  err_code = pthread_mutex_unlock(&theMutex->mutex);
#elif defined(WIN32_THREADS_AVAIL)
  if ( !ReleaseMutex(theMutex->mutex) )
    err_code = GetLastError();
#endif

#ifdef _TRACE_HARD
  fprintf(stderr, "Mutex unlock ,  = [%2ld :%8ld / %3ld] :: %10ld\n",
          (long) this_thread, (long) theMutex, (long) theMutex->counter,
          (long) err_code);
#endif

  if (err_code != 0) {
    theMutex->owner = this_thread;
    theMutex->counter++;
  }
#endif /* ndef NO_MUTEX */

  X_ASSERT ( err_code == 0 );
  return err_code ? -1 : 0;
}


#ifndef NO_MUTEX
static Int4 s_MutexDestroy(MUTEX_T *mp)
{
  Int4 err_code = -1;
#if defined(SOLARIS_THREADS_AVAIL)
  err_code = mutex_destroy(mp);
#elif defined(POSIX_THREADS_AVAIL)
  err_code = pthread_mutex_destroy(mp);
#elif defined(WIN32_THREADS_AVAIL)
  err_code = CloseHandle(*mp) ? 0 : GetLastError();
#endif
  X_ASSERT ( err_code == 0 );
  return err_code;
}
#endif


NLM_EXTERN Int4 NlmMutexDestroy(TNlmMutex theMutex)
{
  Int4 err_code = 0;

#ifndef NO_MUTEX
  if (theMutex == NULL) {
    X_ASSERT ( FALSE );
    return 0;
  }
  X_ASSERT ( theMutex->counter == 0 );

  err_code = s_MutexDestroy(&theMutex->mutex);
  if (err_code == 0)
    Free(theMutex);
#endif /* ndef NO_MUTEX */

  return err_code ? -1 : 0;
}


/********************************************************************
 * === TLS (Thread Local Storage)  ==================================
 ********************************************************************/

#define TLS_DATA_FREE (TNlmTlsCleanup)~0

typedef struct {
  VoidPtr        value;
  TNlmTlsCleanup cleanup;
} STlsData;

struct TNlmTlsTag {
#if   defined(SOLARIS_THREADS_AVAIL)
  thread_key_t  key;
#elif defined(POSIX_THREADS_AVAIL)
  pthread_key_t key;
#elif defined(WIN32_THREADS_AVAIL)
  DWORD         index;
#else
  STlsData      data;
#endif
  TNlmTls  next;
} structTls;


/* list of TLS allocated be the program  */
static TNlmTls   s_TlsList = NULL;


/* Initialize system-dependent TLS object; allocate and fill out internals;
 * include the new TLS to the global TLS list
 */
static Nlm_Boolean s_TlsInit(TNlmTls *pTLS)
{
  int err_code = 0;
  TNlmTls TLS = NULL;

  if (NlmMutexLockEx(&s_TlsMutex) != 0)
    return FALSE;

  while ( !TLS ) {{ /* TRY */
    if ( *pTLS ) { /* just init'd by another thread */
      TLS = *pTLS;
      break;
    }
    
    if ((TLS = (TNlmTls)Calloc(1, sizeof(structTls))) == NULL)
        break;

#if defined(SOLARIS_THREADS_AVAIL)
    err_code = thr_keycreate(&TLS->key, NULL);
#elif defined(POSIX_THREADS_AVAIL)
    err_code = pthread_key_create(&TLS->key, NULL);
#elif defined (WIN32_THREADS_AVAIL)
    TLS->index = TlsAlloc();
    if (TLS->index == (DWORD)(-1))
      err_code = GetLastError();
#else
    TLS->data.value   = NULL;
    TLS->data.cleanup = NULL;
#endif

    if (err_code != 0) {
      X_ASSERT ( FALSE );
      Free(TLS);
      TLS = NULL;
      break;
    }

    TLS->next = s_TlsList;
    s_TlsList = TLS;
  }}

  if ( TLS )
    *pTLS = TLS;
  NlmMutexUnlock(s_TlsMutex);
  return (Nlm_Boolean)(TLS ? TRUE : FALSE);
}


/* Retrieve the TLS's internal data(i.e. value+cleanup structure)
 */
static Boolean s_TlsGetData(TNlmTls TLS, STlsData **ppData)
{
  int err_code = 0;
#if   defined (SOLARIS_THREADS_AVAIL)
  err_code = thr_getspecific(TLS->key, (void **)ppData);
#elif defined (POSIX_THREADS_AVAIL)
  *ppData = (STlsData *)pthread_getspecific(TLS->key);
#elif defined (WIN32_THREADS_AVAIL)
  if ( !(*ppData = (STlsData *)TlsGetValue(TLS->index)) )
    err_code = GetLastError(); /* (NOTE:  may be 0 i.e. okay) */
#else
  *ppData = &TLS->data;
#endif

  X_ASSERT ( err_code == 0 );
  return (Boolean)(err_code == 0);
}


/* Setup the TLS's internal data(== value + cleanup) structure
 */
static Boolean s_TlsSetData(TNlmTls TLS, STlsData *pData)
{
  int err_code = 0;
#if   defined (SOLARIS_THREADS_AVAIL)
  err_code = thr_setspecific(TLS->key, (void *)pData);
#elif defined (POSIX_THREADS_AVAIL)
  err_code = pthread_setspecific(TLS->key, (void *)pData);
#elif defined (WIN32_THREADS_AVAIL)
  if ( !TlsSetValue(TLS->index, (void *)pData) )
    err_code = GetLastError();
#else
  /* do nothing; it's always "set" */
#endif

  X_ASSERT ( err_code == 0 );
  return (Boolean)(err_code == 0);
}


NLM_EXTERN Boolean NlmTlsSetValue(TNlmTls *pTLS,
                                  VoidPtr value, TNlmTlsCleanup cleanup)
{
  STlsData *pData;
  if ( *pTLS ) {
    if ( !s_TlsGetData(*pTLS, &pData) )
      return FALSE;
  }
  else { /* not init'd yet -- initialize it here */
    if ( !s_TlsInit(pTLS) )
      return FALSE;
    pData = NULL;
  }

  /* special case -- cleanup and free the internal data structure */
  if (cleanup == TLS_DATA_FREE) {
    X_ASSERT ( value == NULL );
    if ( !pData )
      return TRUE;

    if ( !s_TlsSetData(*pTLS, NULL) )
      return FALSE;

    if ( pData->cleanup )
      pData->cleanup(*pTLS, pData->value);
#ifdef NCBI_THREADS_AVAIL
    Free(pData);
#else
    pData->value   = NULL;
    pData->cleanup = NULL;
#endif
    return TRUE;
  }

  if ( !pData ) {
    /* allocate and setup new local data */
#ifdef NCBI_THREADS_AVAIL
      if ((pData = (STlsData *)Calloc(1, sizeof(STlsData))) == NULL) {
          ASSERT ( FALSE );
          return FALSE;
      }
      if ( !s_TlsSetData(*pTLS, pData) ) {
          Free(pData);
          return FALSE;
      }
#else
    pData = &(*pTLS)->data;
#endif
  }
  else if (pData->value == value) {
    /* do not call cleanup if TLS value did not change */
    pData->cleanup = cleanup;
    return TRUE;
  }

  {{ /* set new value and call cleanup */
    STlsData xData;
    xData = *pData;
    pData->value   = value;
    pData->cleanup = cleanup;
  
    if ( xData.cleanup )
      xData.cleanup(*pTLS, xData.value);
  }}

  return TRUE;
}


NLM_EXTERN Boolean NlmTlsGetValue(TNlmTls TLS, VoidPtr *value_ptr)
{
  STlsData *tls_data;
  *value_ptr = NULL;

  if ( !TLS )
    return TRUE;
    
  if ( !s_TlsGetData(TLS, &tls_data))
    return FALSE;

  if ( tls_data )
    *value_ptr = tls_data->value;

  return TRUE;
}


/* Reset all TLS values to zero(call cleanups if any); for this thread only
 */
static Boolean s_TlsCleanupAll(void)
{
  TNlmTls TLS;

  if (NlmMutexLockEx(&s_TlsMutex) != 0)
    return FALSE;

  for (TLS = s_TlsList;  TLS;  TLS = TLS->next) {
    NlmTlsSetValue(&TLS, NULL, TLS_DATA_FREE);
  }

  NlmMutexUnlock(s_TlsMutex);
  return TRUE;
}

/* Destroy all TLS-related structures(for all threads!)
 */
static Boolean s_TlsDestroyAll(void)
{
  if (NlmMutexLockEx(&s_TlsMutex) != 0)
    return FALSE;

  while ( s_TlsList ) {
    TNlmTls TLS = s_TlsList;
    s_TlsList = s_TlsList->next;

    {{
      int err_code = 0;
#if defined(SOLARIS_THREADS_AVAIL)
      /* do nothing -- will be */
#elif defined(POSIX_THREADS_AVAIL)
      err_code = pthread_key_delete(TLS->key);
#elif defined (WIN32_THREADS_AVAIL)
      if ( !TlsFree(TLS->index) )
        err_code = GetLastError();
#else
      /* nothing to do */
#endif
      X_ASSERT ( err_code == 0 );
    }}

    Free(TLS);
  }

  s_TlsList = NULL;
  NlmMutexUnlock(s_TlsMutex);
  NlmMutexDestroy(s_TlsMutex);
  s_TlsMutex = NULL;
  return TRUE;
}


/********************************************************************
 * === Auxiliaries  =================================================
 ********************************************************************/

NLM_EXTERN Boolean NlmThreadsAvailable(void)
{
#ifdef NCBI_THREADS_AVAIL
  return TRUE;
#else
  return FALSE;
#endif
}


#if defined(OS_UNIX_DARWIN)
#include <mach/mach.h>
#include <mach/mach_host.h>
#include <mach/host_info.h> 
#endif

NLM_EXTERN Int4 NlmCPUNumber(void)
{
#if defined(OS_UNIX_DARWIN)

  host_basic_info_data_t hinfo;
  mach_msg_type_number_t hinfo_count = HOST_BASIC_INFO_COUNT;
  kern_return_t rc;

  rc=host_info( mach_host_self(), HOST_BASIC_INFO, (host_info_t) &hinfo, &hinfo_count);

  if (rc != KERN_SUCCESS)
    return -1;

  return hinfo.avail_cpus;

#elif defined(WIN32)
  SYSTEM_INFO sysInfo;
  GetSystemInfo(&sysInfo);
  return sysInfo.dwNumberOfProcessors;
#elif defined(_SC_NPROC_ONLN)
  return sysconf(_SC_NPROC_ONLN);
#elif defined(_SC_NPROCESSORS_ONLN)
  return sysconf(_SC_NPROCESSORS_ONLN);
#else
  return -1;
#endif
}


/********************************************************************
 * === Internals
 ********************************************************************/

NLM_EXTERN void NlmThreadDestroyAll(void)
{
  X_VERIFY ( !NlmThreadJoinAll() );

  X_VERIFY ( s_TlsCleanupAll() );
  X_VERIFY ( s_TlsDestroyAll() );

#ifdef NCBI_THREADS_AVAIL
  if (s_ThreadCounter_mutex  &&  NlmMutexDestroy(s_ThreadCounter_mutex) == 0)
    s_ThreadCounter_mutex = 0;
  if (s_ThreadCounter_sema  &&  NlmSemaDestroy(s_ThreadCounter_sema) == 0)
    s_ThreadCounter_sema = 0;
  s_ThreadCounter = 0;
  X_VERIFY( !s_MutexDestroy(&s_Init_mutex) );
#endif /* NCBI_THREADS_AVAIL */
}


#ifdef TEST_MODULE_NCBITHR
/***********************************************************************
 *  TEST
 ***********************************************************************/

#ifndef NCBI_THREADS_AVAIL
#error ATTENTION -- BAD PLATFORM TO TEST THREADS  or  THREADS DISABLED !!!!!
#endif


#include <stdio.h>

#ifdef WIN32
#define STDERR stderr_file
static FILE *STDERR = NULL;
#else
#define STDERR stderr
#endif

#define DO_POSTEX
#define DO_STDERR
#define DO_RWLOCK
#define DO_TLS

static void TEST__ThreadExit0(VoidPtr user_arg)
{
#ifdef DO_STDERR
  fprintf(STDERR, "ThreadExit0():  Self=%ld, Arg = %p\n",
          (long)NlmThreadSelf(), user_arg);
#endif
  Nlm_MemFree(user_arg);
}

static void TEST__ThreadExit1(VoidPtr user_arg)
{
#ifdef DO_STDERR
  fprintf(STDERR, "ThreadExit1():  Self=%ld, Arg = %ld\n",
          (long)NlmThreadSelf(), (long)user_arg);
#endif
}

static void TEST__ThreadExit2(VoidPtr user_arg)
{
#ifdef DO_STDERR
  fprintf(STDERR, "ThreadExit2():  Self=%ld, Arg = %ld\n",
          (long)NlmThreadSelf(), (long)user_arg);
#endif
}


#define N_EXIT_FUNC 6
TNlmThreadOnExit exit_func_arr[N_EXIT_FUNC] =
{
  TEST__ThreadExit0,
  TEST__ThreadExit1,
  TEST__ThreadExit2,
  TEST__ThreadExit1,
  TEST__ThreadExit2,
  NULL
};

typedef struct
{
  Int4                  n_exit_func;
  TNlmThreadOnExit PNTR exit_func;
}  MyExitSet, PNTR MyExitSetPtr;


static Int2 TEST__threads(Int4 n_threads);

static TNlmMutex s_SpawnCounter_mutex;


#ifdef DO_RWLOCK
static TNlmRWlock RWlock;
#endif

#ifdef DO_TLS
#define N_TLS 32
static TNlmTls s_TLS[N_TLS];

static void TEST_TlsCleanup(TNlmTls TLS, VoidPtr old_value) {
  VoidPtr new_value;
  VERIFY ( NlmTlsGetValue(TLS, &new_value) );
  ASSERT ( old_value != 0 );
  ASSERT ( new_value == 0  ||  (CharPtr)new_value == (CharPtr)old_value + 1 );
}
#endif


static VoidPtr TEST__MyThread(VoidPtr arg)
{
  MyExitSetPtr exit_func_set = (MyExitSetPtr) arg;
  Int4 i;

#ifdef DO_STDERR
  fprintf(STDERR, "TEST__MyThread():  Thread #%ld started\n",
          (long)NlmThreadSelf());
#endif
#ifdef DO_POSTEX
  Nlm_ErrPostEx(SEV_INFO, 2, 3, "TEST__MyThread():  Thread #%ld started\n",
          (long)NlmThreadSelf());
#endif
#ifdef DO_TLS
  for (i = 0;  i < N_TLS;  i++) {
    VoidPtr tls_value = (VoidPtr)~0;
    VERIFY ( NlmTlsGetValue(s_TLS[i], &tls_value) );
    ASSERT ( tls_value == 0 );
    VERIFY ( NlmTlsSetValue(&s_TLS[i], (CharPtr)tls_value+1, TEST_TlsCleanup));
  }
#endif

  {{ /* if to spawn more threads... */
#if defined(SOLARIS_THREADS_AVAIL) || defined(POSIX_THREADS_AVAIL)
#define NCBI_MAX_THREADS _POSIX_THREAD_THREADS_MAX
#elif defined(WIN32)
#define NCBI_MAX_THREADS 32
#else
#define NCBI_MAX_THREADS INT2_MAX
#endif

#define RESERVE_THREADS 8
#define N_SPAWN         4

    static int nnn_threads = RESERVE_THREADS + N_SPAWN;
    int more;
    VERIFY ( !NlmMutexLockEx(&s_SpawnCounter_mutex) );
    more = (nnn_threads < NCBI_MAX_THREADS - N_SPAWN);
    if ( more )
      nnn_threads += N_SPAWN;
    VERIFY ( !NlmMutexUnlock(s_SpawnCounter_mutex) );
    if ( more )
      TEST__threads(N_SPAWN);
  }}

#ifdef DO_TLS
  for (i = 0;  i < N_TLS;  i++) {
    VoidPtr tls_value = (VoidPtr)~0;
    VERIFY ( NlmTlsGetValue(s_TLS[i], &tls_value) );
    ASSERT ( tls_value == (VoidPtr)1 );
    VERIFY ( NlmTlsSetValue(&s_TLS[i], (CharPtr)tls_value+1, TEST_TlsCleanup));
    VERIFY ( NlmTlsSetValue(&s_TLS[i], (CharPtr)tls_value+2, TEST_TlsCleanup));
    VERIFY ( NlmTlsSetValue(&s_TLS[i], (CharPtr)tls_value+3, TEST_TlsCleanup));
  }
#endif /* DO_TLS */


#ifdef DO_RWLOCK

#  if defined(POSIX_THREADS_AVAIL) || defined(WIN32_THREADS_AVAIL)
#    define RW_SMART
#  endif

  {{ /* test RWlock functionality */
    static int iii = 0;
    static int var = 0;
    if ( ++iii % 5 ) { /* reader */
      for (i = 0;  i < 500;  i++) {
        int x_var, j;

        if (NlmRWtryrdlock(RWlock) != 0)
          VERIFY ( !NlmRWrdlock(RWlock) );
#  ifdef RW_SMART 
        if (i % 6 == 0)
          VERIFY ( !NlmRWrdlock(RWlock) );
        if (i % 12 == 0) {
          VERIFY ( !NlmRWtryrdlock(RWlock) );
          VERIFY ( !NlmRWunlock(RWlock) );
        }
#  endif

        x_var = var;
        for (j = 0;  j < 10;  j++)
          fprintf(STDERR, "%s", "");
        ASSERT ( x_var == var );

#  ifdef RW_SMART 
        if (i % 7 == 0) {
          VERIFY ( !NlmRWtryrdlock(RWlock) );
          VERIFY ( !NlmRWunlock(RWlock) );
        }
        if (i % 6 == 0)
          VERIFY ( !NlmRWunlock(RWlock) );
#  endif
        VERIFY ( !NlmRWunlock(RWlock) );

        for (j = 0;  j < 50;  j++)
          fprintf(STDERR, "%s", "");
        if (x_var != var)
          fprintf(STDERR, "RW\n");
      }
    }
    else { /* writer */
      for (i = 0;  i < 1000;  i++) {
        int j;
        if (NlmRWtrywrlock(RWlock) != 0)
          VERIFY ( !NlmRWwrlock(RWlock) );
#  ifdef RW_SMART 
        if (i % 4 == 0)
          VERIFY ( !NlmRWwrlock(RWlock) );
        if (i % 6 == 0)
          VERIFY ( !NlmRWrdlock(RWlock) );
        if (i % 8 == 0)
          VERIFY ( !NlmRWtrywrlock(RWlock) );
        if (i % 10 == 0)
          VERIFY ( !NlmRWtryrdlock(RWlock) );
#  endif

        var++;
        for (j = 0;  j < 7;  j++, var++)
          fprintf(STDERR, "%s", "");

#  ifdef RW_SMART 
        if (i % 4 == 0)
          VERIFY ( !NlmRWunlock(RWlock) );
        if (i % 6 == 0)
          VERIFY ( !NlmRWunlock(RWlock) );
        if (i % 8 == 0)
          VERIFY ( !NlmRWunlock(RWlock) );
        if (i % 10 == 0)
          VERIFY ( !NlmRWunlock(RWlock) );
#  endif
        VERIFY ( !NlmRWunlock(RWlock) );

        for (j = 0;  j < 20;  j++)
          fprintf(STDERR, "%s", "");
      }
    }
  }}
#endif /* DO_RWLOCK */


#ifdef DO_TLS
  for (i = 0;  i < N_TLS;  i++) {
    VoidPtr tls_value = (VoidPtr)~0;
    VERIFY ( NlmTlsGetValue(s_TLS[i], &tls_value) );
    ASSERT ( tls_value == (VoidPtr)4 );
    VERIFY ( NlmTlsSetValue(&s_TLS[i], (CharPtr)tls_value+1, TEST_TlsCleanup));
    VERIFY ( NlmTlsSetValue(&s_TLS[i], (CharPtr)tls_value+2, TEST_TlsCleanup));
    VERIFY ( NlmTlsSetValue(&s_TLS[i], (CharPtr)tls_value+3, TEST_TlsCleanup));
  }
#endif /* DO_TLS */

  /* register dummy(debug-print only) exit functions;  ungerister some... */
  for (i = exit_func_set->n_exit_func - 1;  i > 0;  i--)
    {
      NlmThreadAddOnExit(exit_func_set->exit_func[i],
                         (VoidPtr)NlmThreadSelf());
      if (i%2 == 0)
        NlmThreadRemoveOnExit(exit_func_set->exit_func[i],
                              (VoidPtr)NlmThreadSelf());
    }

  /* register the exit function to deallocate dynamic memory */
  NlmThreadAddOnExit(exit_func_set->exit_func[0], (VoidPtr)exit_func_set);

  {{ /* sleep for a while before exiting */
    static int do_sleep = 0;
    if ((do_sleep = !do_sleep) == TRUE) {
#if defined (OS_UNIX)
      sleep( 1 );
#elif defined (WIN32)
      Sleep( 1000 );
#endif
    }
  }}

#ifdef DO_POSTEX
  Nlm_ErrPostEx(SEV_INFO, 2, 3, "TEST__MyThread():  Thread #%ld finished\n",
                (long)NlmThreadSelf());
#endif

  return NULL;
}


static Int2 TEST__threads(Int4 n_threads)
{
  Int4 t;
  TNlmThread *threads = (TNlmThread *)
    Nlm_MemNew(n_threads * sizeof(TNlmThread));

  for (t = 0;  t < n_threads;  t++)
    {
      static int xxx_counter = 0;
      MyExitSetPtr exit_set = (MyExitSetPtr) Nlm_MemNew( sizeof(MyExitSet) );
      exit_set->n_exit_func = t % N_EXIT_FUNC;
      exit_set->exit_func   = exit_func_arr;
      threads[t] =
        NlmThreadCreateEx(&TEST__MyThread, (VoidPtr)exit_set,
                          (t == n_threads-1  ||  t%3 == 1) ? THREAD_RUN :
                          (t%3 == 2) ? THREAD_BOUND : THREAD_DETACHED,
                          (EThreadPriority)(xxx_counter++ % eTP_Default),
                          NULL, NULL);
#ifdef DO_STDERR
      fprintf(STDERR, "TEST__threads():  Starting thread #%ld\n",
              (long)threads[t]);
#endif
    }

  for (t = 0;  t < n_threads;  t++)
    /* (detached threads must be joined by NlmThreadJoinAll) */
    if (t == n_threads-1  ||  t%3 != 0) {
      VoidPtr status;
      Int4 err_code = NlmThreadJoin(threads[t], &status);
#ifdef DO_STDERR
      if (err_code != 0) {
        fprintf(STDERR, "TEST__threads():  Cannot join thread #%ld;  \
error code = %ld\n",
                (long)threads[t], (long)err_code);
        ASSERT ( FALSE );
      }

      fprintf(STDERR, "TEST__threads():  Thread #%ld joined;  \
terminated, exit status = %p\n",
              (long)threads[t], status);
#endif
    }

  Nlm_MemFree(threads);

#ifdef DO_STDERR
  fprintf(STDERR, "TEST__threads():  FINISHED\n");
#endif
  return 0;
}


Nlm_Int2 Nlm_Main( void )
{
  Int2 status;

#if defined (WIN32)
  STDERR = fopen("stderr", "w");
#endif

  VERIFY ( Nlm_ErrSetLog("ncbithr.log") );
  Nlm_ErrSetOpts(ERR_TEE, ERR_LOG_ON);
  ErrSetOptFlags(EO_SHOW_FILELINE);
  Nlm_ErrPostEx(SEV_INFO, 2, 3, "NCBI_THREAD test started...\n");

#ifdef DO_RWLOCK
  RWlock = NlmRWinit();
  ASSERT ( RWlock );
#endif

  status = TEST__threads(N_SPAWN);

  VERIFY ( !NlmThreadJoinAll() );

#ifdef DO_RWLOCK
  VERIFY ( !NlmRWdestroy(RWlock) );
#endif
  if ( s_SpawnCounter_mutex )
    VERIFY ( !NlmMutexDestroy(s_SpawnCounter_mutex) );

#if defined (WIN32)
  fclose(STDERR);
#endif
  return status;
}

#endif  /* TEST_MODULE_NCBITHR */

/* EOF */
