/* $Id: ncbithr.h,v 6.11 2002/07/09 15:33:46 kans Exp $ */
/*****************************************************************************

    Name: ncbithr.h

    Description: Function and type declarations for Multi-Thread API library.

    Author: Sergei Shavirin, Denis Vakatov

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
* $Log: ncbithr.h,v $
* Revision 6.11  2002/07/09 15:33:46  kans
* use Nlm_ prefixes now that it is indirectly included by vibwndws.c
*
* Revision 6.10  2000/11/06 17:09:21  vakatov
* RW_HISTORY, RW_TRACE -- To gather and printout info on the RW-lock history
*
* Revision 6.9  1999/10/14 18:44:36  vakatov
* For better consistency, include <ncbilcl.h> to <ncbistd.h>
*
* Revision 6.8  1998/02/27 17:13:58  vakatov
* [WIN32 DLL]  Declared some functions as NLM_EXTERN(DLL-exportable)
*
* Revision 6.7  1998/02/17 20:13:55  vakatov
* NlmTlsInit() removed from the public API;  instead, the
* NlmTlsSetValue() now initializes the TLS if it is not initialized yet
*
* Revision 6.6  1998/02/13 15:15:36  vakatov
* Added "cleanup" parameter to NlmTlsSetValue() to provide TLS cleanup.
* Removed NlmTlsDestroy(); now destroy all TLSs in NlmThreadDestroyAll()
*
* Revision 6.5  1998/01/08 17:17:08  vakatov
* Added NlmThreadJoinAll() and NlmThreadCleanupAll()
*
* Revision 6.4  1997/12/24 17:18:01  vakatov
* Added TLS(Thread Local Storage) functionality for all platforms(incl.
* non-"thread-capable" ones -- interpreted as a single-thread application)
*
* Revision 6.3  1997/12/23 19:13:32  vakatov
* Thread handling -- revised, fixed:
* removed NlmThreadKill(), NlmThread[Set|Get]Concurrency() and
* NlmThread[Set|Get]Priority() functions;
* NlmThreadCreate() doesn't accept parameter "flags" anymore;
* added "priority" parameter to NlmThreadCreateEx()
*
* Revision 6.2  1997/12/17 19:11:27  vakatov
* Use all-platform THREAD_{RUN|BOUND|DETACHED} instead of the platform
* specific THR_{...} thread creation/running attributes
*
* Revision 6.1  1997/12/16 23:12:38  vakatov
* Mutexes and RW-locks:  revised, rewritten, fixed;  added built-in
* strict run-time checks -- (use -D_DEBUG_HARD)
*
*
* 20 February of 1996 - Shavirin  -  originaly written
*****************************************************************************/

#ifndef __NLM_THREAD__
#define __NLM_THREAD__

#include <ncbistd.h>

#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/****************************************************************************/
/* TYPEDEFS */
/****************************************************************************/

typedef Nlm_VoidPtr TNlmThread;         /* handle(id) of the thread */
  
#define NULL_thread ((TNlmThread)0) /* for error handling */

typedef Nlm_VoidPtr TNlmSemaphore;      /* handle (id) of the semaphore */

struct TNlmRWlockTag;                         /* internal RW-lock storage  */
typedef struct TNlmRWlockTag PNTR TNlmRWlock; /* handle(id) of the RW-lock */
typedef TNlmRWlock PNTR TNlmRWlockPtr;        /* pointer to RW-lock handle */

struct TNlmMutexTag;                        /* internal mutex storage  */
typedef struct TNlmMutexTag PNTR TNlmMutex; /* handle(id) of the mutex */
typedef TNlmMutex PNTR TNlmMutexPtr;        /* pointer to mutex handle */

struct TNlmTlsTag;                      /* hidden internal TLS structure */
typedef struct TNlmTlsTag PNTR TNlmTls; /* handle(id) of the mutex       */
typedef TNlmTls PNTR TNlmTlsPtr;        /* pointer to mutex handle       */

/* pointer to the thread function */
typedef Nlm_VoidPtr (*TNlmThreadStart)(Nlm_VoidPtr arg);

/* pointer to the thread finishing function */
typedef void (*TNlmThreadOnExit)(Nlm_VoidPtr user_arg);


/****************************************************************************/
/* FINCTION DEFINITIONS */
/****************************************************************************/

/*
 *  Auxiliary function to be used in the NlmThreadAddOnExit()
 *  function call to provide memory deallocation on the thread exit
 */
NLM_EXTERN void NlmThreadMemFree(Nlm_VoidPtr ptr);


/* --------------------  NlmThreadAddOnExit  ----------------------
   Purpose:     Adds a function to be called on the thread termination
   Parameters:  The function "func" will be called with the "arg" argument
                when the thread is terminating;  then, the later registered
                "destroying function" will be called earlier (LIFO order)
   Returns:     TRUE on success...
   NOTE:        Must be called from inside the thread
  ------------------------------------------------------------------*/
NLM_EXTERN Nlm_Boolean NlmThreadAddOnExit(TNlmThreadOnExit func, Nlm_VoidPtr arg);


/* --------------------  NlmThreadRemoveOnExit  -------------------
   Purpose:     Removes the thread's termination function(s)
   Description: This function removes all previosly registered(for this
                thread) terminating functions whose attributes
                matches both "func" and "arg"
   NOTE:        Must be called from inside the thread
  ------------------------------------------------------------------*/
NLM_EXTERN void NlmThreadRemoveOnExit(TNlmThreadOnExit func, Nlm_VoidPtr arg);


/* --------------------  NlmThreadCreate  ---------------------------
   Purpose:     To create new thread
   Parameters:  theStartFunction - address of the thread function
                *arg             - arguments for the new thread
   Returns:     New thread handle. Will be used to identify the thread.
   Description: This function creates new thread - independent process
                inside current process, that share most resources of the
                parent thread.
  ------------------------------------------------------------------*/
NLM_EXTERN TNlmThread NlmThreadCreate(TNlmThreadStart theStartFunction, Nlm_VoidPtr arg);


/* --------------------  NlmThreadCreateEx  -------------------------
   Purpose:     To create new thread
   Parameters:  "theStartFunction", "arg"  -- see NlmThreadCreate;
                "flags" bit mask of {THREAD_RUN, THREAD_BOUND, THREAD_DETACHED}
                "priority" -- specifies the thread's relative priority;
                "exit_func", "exit_arg"  -- these two allow one to add a
                function to be called when the thread is exiting -- it's
                the only convenient way to do this from outside of thread
                (see also the NlmThreadAddOnExit() function to get know
                how to do it from the thread's inside).
   Description: This is an extendend version of NlmThreadCreate()
   NOTE:        It's guaranteed that the "exit_func"(if non-NULL) will
                be called *after* any other exit function specified
                from inside the thread
   HINTS:       THREAD_BOUND    may not be supported on OSF1 and IRIX
                THREAD_DETACHED may not be supported on OSF1
                Priorities may not be supported for POSIX threads on Solaris
  ------------------------------------------------------------------*/
#define THREAD_RUN      0x0  /* default == { unbound & joinable }           */
#define THREAD_BOUND    0x1  /* run the thread bound to an LPW (UNIX only)  */
#define THREAD_DETACHED 0x2  /* run the thread detached(note: non-joinable) */

typedef enum { /* NOTE:  priority doesn't apply to "bound" threads on UNIX */
  eTP_Idle,
  eTP_Lowest,
  eTP_Low,
  eTP_Normal,
  eTP_High,
  eTP_Highest,
  eTP_Default
} EThreadPriority;
    
NLM_EXTERN TNlmThread NlmThreadCreateEx(TNlmThreadStart theStartFunction, Nlm_VoidPtr arg, Nlm_Int4 flags, EThreadPriority priority, TNlmThreadOnExit exit_func, Nlm_VoidPtr exit_arg);


/* ----------------------  NlmThreadSelf  --------------------------
   Purpose:     To get thread handle
   Returns:     the thread's handle(NULL on error)
   Description: NlmThreadSelf() returns the handle of the calling thread
  -----------------------------------------------------------------*/
NLM_EXTERN TNlmThread NlmThreadSelf(void);


/* ----------------------  NlmThreadCompare  --------------------------
   Purpose:     To compare 2 threads
   Parameters:  Thread handles to compare
   Returns:     TRUE if the 2 threads are identical
   Description: In OSF1 thread ID is structure and must be compared 
                in different way
  -----------------------------------------------------------------*/
NLM_EXTERN Nlm_Boolean NlmThreadCompare(TNlmThread thread1, TNlmThread thread2);


/* ----------------------  NlmThreadJoin  --------------------------
   Purpose:     Wait for thread termination
   Parameters:  INPUT:  "wait_for" -- thread handle to wait for termination
                OUTPUT: "status"   -- the thread's exit status
   Returns:     zero value on success;  non-zero value on error
   Description: Blocks the calling thread until the thread specified by
                wait_for terminates. The specified thread must be in the
                current process.
   NOTE:        Multiple threads cannot wait for the same thread to
                terminate. One thread will return successfully and the
                others will fail.
                On WIN32, if a thread is not joined using this function
                it lead to a minor resource leak
  -----------------------------------------------------------------*/
NLM_EXTERN Nlm_Int4 NlmThreadJoin(TNlmThread wait_for, Nlm_VoidPtr *status);


/* ----------------------  NlmThreadJoinAll  -----------------------
   Purpose:     Wait for termination of all threads presently running
                in the current process
   Returns:     zero value on success;  non-zero value on error
   NOTE:        joins only the threads created by NlmThreadCreate[Ex]
                and exited with the NlmThreadExit() or due to the
                normal return from the main thread function;
                deadlocks if called *not* from the main thread!
  -----------------------------------------------------------------*/
NLM_EXTERN Nlm_Int4 NlmThreadJoinAll(void);


/* -------------------  NlmThreadExit  ------------------------------
   Purpose:      To terminate the calling thread
   Parameters:   status - exit data(code) for the calling thread
   Description:  This function terminates th calling thread. All thread-
                 specific data bindings are released. If the initial thread
                 returns from main() then the process exits with a status
                 equal to the return value
   -----------------------------------------------------------------*/
NLM_EXTERN void NlmThreadExit(Nlm_VoidPtr status);


/********************************************************************/
/*                                                                  */
/* =========== Semaphore support for the thread API library ======= */
/*                                                                  */
/********************************************************************/


/* ---------------------  NlmSemaInit  ------------------------------
   Purpose:     To create new semaphore
   Parameters:  count - number of threads that may use semaphore in parallel
   Returns:     Semaphore handle
   Description: To create new semaphore with "count" simultaneous accesses.
   NOTE:        This semaphore can be used to synchronize threads in this
                process only
   -----------------------------------------------------------------*/
NLM_EXTERN TNlmSemaphore NlmSemaInit(Nlm_Uint4 count);


/* ---------------------  NlmSemaDestroy  ---------------------------
   Purpose:     To destroy specified semaphore.
   Parameters:  theSemaphore - handle of the semaphore
   Returns:     Zero on success;  non-zero value on error
   Description: destroys any state associated with the semaphore pointed
                to by theSemaphore
   -----------------------------------------------------------------*/
NLM_EXTERN Nlm_Int4 NlmSemaDestroy(TNlmSemaphore theSemaphore);


/* ---------------------  NlmSemaWait  ------------------------------
   Purpose:      To wait until semaphore is not zero.
   Parameters:   theSemaphore - handle of the semaphore
   Returns:      Zero on success;  non-zero value on error
   Descrription: Blocks the calling thread until the count in the
                 semaphore pointed by theSemaphore becomes greater than zero
                 and then atomically decrements it
   -----------------------------------------------------------------*/
NLM_EXTERN Nlm_Int4 NlmSemaWait(TNlmSemaphore theSemaphore);


/* ---------------------  NlmSemaTryWait  ---------------------------
   Purpose:      To poll semaphore status
   Parameters:   theSemaphore - handle of the semaphore to poll
   Returns:      Zero on success;  non-zero value on error
   Description:  atomically decrements the count in the semaphore
                 pointed to by theSemaphore if the count is greater
                 than zero. Otherwise it returns an error.
  -----------------------------------------------------------------*/
NLM_EXTERN Nlm_Int4 NlmSemaTryWait(TNlmSemaphore theSemaphore);


/* ---------------------  NlmSemaPost  ------------------------------
   Purpose:      To increase the count of semaphore by one
   Parameters:   theSemaphore - handle of the semaphore to post
   Returns:      Zero on success;  non-zero value on error
   Description:  atomically increments the count semaphore pointed to by
                 theSemaphore, one is unblocked.
   -----------------------------------------------------------------*/
NLM_EXTERN Nlm_Int4 NlmSemaPost(TNlmSemaphore theSemaphore);


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


/* Do not pass the file name in the non-debug mode to avoid having
 * numerous strings in the static data segment.
 */
#if defined(_DEBUG)
#  define RW_FILE __FILE__
#else
#  define RW_FILE 0
#endif
#define RW_LINE __LINE__


/* ---------------------  NlmRWlock  --------------------------------
   Purpose:  Initialize readers/writer lock
   Returns:  Handle of the new RW-lock(NULL on error)
   NOTE:     Readers/writer locks MUST be initialized before use.
   -----------------------------------------------------------------*/
NLM_EXTERN TNlmRWlock NlmRWinit(void);


/* ---------------------  NlmRWdestroy  -----------------------------
   Purpose:      Destroy RW-lock
   Parameters:   Handle of RW-lock
   Returns:      Zero on success;  non-zero value on error
   Description:  Destroys any state associated with the RW lock
                 pointed to by RW.
  -----------------------------------------------------------------*/
NLM_EXTERN Nlm_Int4 NlmRWdestroy(TNlmRWlock RW);


/* ---------------------  NlmRWrdlock  ------------------------------
   Purpose:      Acquire a read lock
   Parameters:   Handle of RW-lock 
   Returns:      Zero on success;  non-zero value on error
   Description:  Acquires a read lock on the readers/writer lock
                 pointed to by RW. If the RW lock is already
                 locked for writing, the calling thread blocks until
                 the write lock is released.
   NOTE:         More than one thread may hold a read lock on a RW lock
                 at any one time.
  -----------------------------------------------------------------*/
NLM_EXTERN Nlm_Int4 NlmRWrdlockEx(TNlmRWlock RW,
                              const char* file, int line);
#define NlmRWrdlock(RW)  NlmRWrdlockEx(RW, RW_FILE, RW_LINE)


/* ---------------------  NlmRWwrlock  ------------------------------
   Purpose:      Acquire a write lock
   Parameters:   Handle of RW-lock
   Returns:      Zero on success;  non-zero value on error
   Description:  Acquires a write lock on the readers/writer lock
                 pointed to by RW. If the RW lock is already
                 locked for reading or writing, the calling thread blocks
                 until the lock is released.
   NOTE:         Only one thread may hold a write lock on a RW lock
                 at any one time.
  -----------------------------------------------------------------*/
NLM_EXTERN Nlm_Int4 NlmRWwrlockEx(TNlmRWlock RW,
                              const char* file, int line);
#define NlmRWwrlock(RW)  NlmRWwrlockEx(RW, RW_FILE, RW_LINE)


/* ---------------------  NlmRWunlock  ------------------------------
   Purpose:      Unlock RW-lock
   Parameters:   Handle of RW-lock
   Returns:      Zero on success;  non-zero value on error
   Description:  The RW-lock must be locked and the calling thread must
                 hold the lock either for reading or for writing. If
                 any other threads are waiting for the RW lock to become
                 available, one of them is unblocked.
   NOTE:         If the calling thread does not hold the lock for either
                 reading or writing no error status is returned and the
                 behavior of the program is undefined.
   -----------------------------------------------------------------*/
NLM_EXTERN Nlm_Int4 NlmRWunlockEx(TNlmRWlock RW,
                              const char* file, int line);
#define NlmRWunlock(RW)  NlmRWunlockEx(RW, RW_FILE, RW_LINE)


/* ---------------------  NlmRWtryrdlock  ---------------------------
   Purpose:      Try to acquire a read lock
   Parameters:   Handle of RW-lock
   Returns:      Zero on success;  non-zero value on error
   Description:  Attempts to acquire a read lock on the RW-lock
                 pointed to by RW. If the RW-lock is already
                 locked for writing, it returns an error. Otherwise
                 the read lock is acquired
   -----------------------------------------------------------------*/
NLM_EXTERN Nlm_Int4 NlmRWtryrdlockEx(TNlmRWlock RW,
                                 const char* file, int line);
#define NlmRWtryrdlock(RW)  NlmRWtryrdlockEx(RW, RW_FILE, RW_LINE)


/* ---------------------  NlmRWtrywrlock  ---------------------------
   Purpose:       Try to acquire a write lock
   Parameters:    Handle of RW-lock
   Returns:       Zero on success;  non-zero value on error
   Description:   Attempts to acquire a write lock on the RW lock
                  pointed to by RW. If the RW-lock is already
                  locked for reading or writing, it returnes an error.
                  Otherwise the write lock is acquired
   -----------------------------------------------------------------*/
NLM_EXTERN Nlm_Int4 NlmRWtrywrlockEx(TNlmRWlock RW,
                                 const char* file, int line);
#define NlmRWtrywrlock(RW)  NlmRWtrywrlockEx(RW, RW_FILE, RW_LINE)


/* ---------------------  NlmRWprintout  ---------------------------
   Purpose:       Printout the latest RW-lock activity
   Parameters:    Handle of RW-lock
   Returns:       non-NULL dynamically allocated string (caller must free it)
   Description:   Print the latest RW-lock activity (starting from the
                  latest "Unlocked" state), if the "ncbithr.c" was compiled
                  in the RW-lock history tracking mode.
                  Otherwise, just print the current state of RW-lock.
   -----------------------------------------------------------------*/
NLM_EXTERN char* NlmRWprintout(TNlmRWlock RW);



/********************************************************************
 * === MUTEXes ====================================================
 *
 * Description: Mutual exclusion locks (mutexes)  prevent  multiple
 *              threads from  simultaneously  executing  critical
 *              sections  of code which access shared data (i.e.,
 *              mutexes are used to  serialize  the execution of threads).
 * NOTE:        All mutexes MUST be global.
 ********************************************************************/


/* ---------------------  NlmMutexInit  ------------------------------
   Purpose:     To create and initialize mutex, if necessary
   Parameters:  Pointer to the mutex handle
   Returns:     New(or old -- if the referenced mutex already exists)
                mutex handle
   Description: Create new mutex if it doesn's not exist(i.e. if
                *theMutexPtr == NULL); do nothing if it already exists.
   NOTE:        This mutex can be used to synchronize threads in this
                process only.
   -----------------------------------------------------------------*/
NLM_EXTERN TNlmMutex NlmMutexInit(TNlmMutexPtr theMutexPtr);


/* ---------------------  NlmMutexLock  ------------------------------
   Purpose:      To lock mutex.  Block the calling thread if mutex is busy.
   Parameters:   theMutex -- handle of the mutex
   Returns:      Zero on success;  non-zero value on error
   Description:  Blocks code access
   -----------------------------------------------------------------*/
NLM_EXTERN Nlm_Int4 NlmMutexLock (TNlmMutex theMutex);


/* ---------------------  NlmMutexTryLock  ------------------------------
   Purpose:      Try to poll mutex.  Don't block the calling thread.
   Parameters:   theMutex -- handle of the mutex
   Returns:      Zero on success;  non-zero value if already locked
                 by another thread
   Description:  Poll condition of mutex -- do not block if mutex busy
   -----------------------------------------------------------------*/
NLM_EXTERN Nlm_Int4 NlmMutexTryLock(TNlmMutex theMutex);


/* ---------------------  NlmMutexLockEx  ----------------------------
   Purpose:      To lock mutex(initialize it, if necessary)
   Parameters:   theMutex -- handle of the mutex
   Returns:      Zero on success;  non-zero value on error
   Description:  Blocks code access
   -----------------------------------------------------------------*/
NLM_EXTERN Nlm_Int4 NlmMutexLockEx(TNlmMutexPtr theMutexPtr);


/* ---------------------  NlmMutexTryLockEx  ----------------------------
   Purpose:      To try lock mutex(initialize it, if necessary)
   Parameters:   theMutex -- handle of the mutex
   Returns:      Zero on success;  non-zero value  if already locked
                 by another thread
   Description:  Poll condition of mutex -- do not block if mutex busy
   -----------------------------------------------------------------*/
NLM_EXTERN Nlm_Int4 NlmMutexTryLockEx(TNlmMutexPtr theMutexPtr);


/* ---------------------  NlmMutexUnlock  ------------------------------
   Purpose:      To unlock mutex
   Parameters:   theMutex - handle of the mutex
   Returns:      Zero on success;  non-zero value on error
   Description:  Unblocks code access
   -----------------------------------------------------------------*/
NLM_EXTERN Nlm_Int4 NlmMutexUnlock(TNlmMutex theMutex);


/* ---------------------  NlmMutexDestroy ------------------------------
   Purpose:      To destroy mutex
   Parameters:   theMutex - handle of the mutex
   Returns:      Zero on success;  non-zero value on error
   Description:  Remove the mutex from execution
   -----------------------------------------------------------------*/
NLM_EXTERN Nlm_Int4 NlmMutexDestroy(TNlmMutex theMutex);


/********************************************************************
 * === TLS (Thread Local Storage)  ==================================
 *
 * NOTE:  All TLSs MUST be global and belong to the same process
 * NOTE:  Even if there are no real threads engaged, the TLS would
 *        work properly -- just as if the "main()" was a thread
 ********************************************************************/


/* ---------------------  NlmTlsSetValue  --------------------------
   Purpose:     Set TLS value and its cleanup function; initialize TLS
                if the passed handle is NULL
   Parameters:  "pTLS" -- ptr. to the TLS handle;  "value" -- the value to set;
   		       "cleanup" -- to be called(with the thread's old TLS
                value) if the value is changed or the thread is finished.
   Returns:     TRUE on success, FALSE on error
   NOTE 1:      do nothing(and ret. TRUE) if new value is equal to the old one
   NOTE 2:      inside the "cleanup" function: the new value is set
                already, and NlmTls[SG]etValue(...) are safe to call
   -----------------------------------------------------------------*/
typedef void (*TNlmTlsCleanup)(TNlmTls TLS, Nlm_VoidPtr old_value);
NLM_EXTERN Nlm_Boolean NlmTlsSetValue(TNlmTls *pTLS, Nlm_VoidPtr value, TNlmTlsCleanup cleanup);


/* ---------------------  NlmTlsSetValue  --------------------------
   Purpose:     Get TLS value
   Parameters:  "TLS"       -- handle of the TLS,
                "value_ptr" -- pointer to location where to store
                               the TLS value
   Returns:     TRUE on success, FALSE on error
   NOTE:        Return value_ptr = NULL if the specified value has not
                yet been set(by NlmTlsSetValue) in this thread, or
                if the TLS has not been initialized yet(NULL TLS handle)
   -----------------------------------------------------------------*/
NLM_EXTERN Nlm_Boolean NlmTlsGetValue(TNlmTls TLS, Nlm_VoidPtr *value_ptr);


/********************************************************************
 * === Auxiliaries  =================================================
 ********************************************************************/

/* ---------------------  NlmThreadsAvailable -----------------------
   Purpose:      Check to see if threads are available on this platform
   Returns:      TRUE if threads are available, FALSE otherwise
   Description:  Check to see if threads are available on this platform
   -----------------------------------------------------------------*/
NLM_EXTERN Nlm_Boolean NlmThreadsAvailable(void);


/* ---------------------  NlmCPUNumber ------------------------------
   Purpose:      To get number of CPU on machine
   Returns:      Number of CPU
   -----------------------------------------------------------------*/
NLM_EXTERN Nlm_Int4 NlmCPUNumber(void);


/********************************************************************
 * === CoreLib internals
 ********************************************************************/

/* Joins all NCBI threads, then clean/destroy all internal thread
 * library data(including TLSs and their cleanups); can be called
 * from the main thread only.
 * NOTE: for internal use only! must be called in the very end of the
 * program execution; no NCBI thread library functions can be called
 * after the call to this function!
 */
NLM_EXTERN void NlmThreadDestroyAll(void);


#ifdef __cplusplus
}
#endif /* __cplusplus */


#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

#endif /* __NLM_THREAD__ */

/*EOF*/

