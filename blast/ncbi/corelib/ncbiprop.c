/*   ncbiprop.c
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
* File Name:  ncbiprop.c
*
* Author:  Schuler
*
* Version Creation Date:   06-04-93
*
* $Revision: 6.3 $
*
* File Description:		Application Property Functions.
*
*		Application properties were	introduced to allow the NCBI Toolbox 
*		to be implemented as a dynamic link library (DLL).  Under Windows, 
*		global variables located in the DLL are not instantiated for each 
*		application	instance, but instead shared by all applications having 
*		a run-time linkage.  Values that might ordinarily be stored in 
*		global variables can instead be saved as application properties
*		(unless sharing is desired, of course).  An application property
*		is simply a pointer (or other value cast to a pointer) that is
*		identified by a string key.  Properties are kept in a linked list
*		(sorted by key) whose head is stored in an application context block.
*		The application context block is created on-demand and marked 
*		with the process ID of the calling application (or it's creation
*		can be forced by calling InitAppContext() at the beginning of your
*		program.  The linked list of (smallish) application context 
*		structures is the only thing allocated from the DLL's data space, 
*		all other memory is owned by the application that called the DLL 
*		is automatically freed by the system when the application terminates.
*		
*		If this code is not actually compiled as a DLL, but bound at link
*		time to a single application in the normal way, the behavior of all
*		functions will be the same.  The only difference being that the
*		list of application context structures will contain exactly one item.
*
* Modifications:  
* --------------------------------------------------------------------------
* $Log: ncbiprop.c,v $
* Revision 6.3  2001/03/02 19:52:34  vakatov
* Do not use "pid" in the app.context anymore.
* It was needed for 16-bit MS-Win DLLs, a long time ago, and now it's
* just eating resources...
*
* Revision 6.2  1998/08/24 17:42:02  kans
* fixed old style function definition warnings
*
* Revision 6.1  1997/10/29 02:44:20  vakatov
* Type castings to pass through the C++ compiler
*
* Revision 6.0  1997/08/25 18:16:56  madden
* Revision changed to 6.0
*
* Revision 5.6  1997/01/28 21:19:12  kans
* <Desk.h>, <OSEvents.h> and <GestaltEqu.h> are obsolete in CodeWarrior
*
 * Revision 5.5  1997/01/08  22:55:36  shavirin
 * Added threads comparison through NlmThreadCompare
 *
 * Revision 5.4  1996/12/03  21:48:33  vakatov
 * Adopted for 32-bit MS-Windows DLLs
 *
 * Revision 5.3  1996/11/25  19:05:30  vakatov
 * Made Get/ReleaseAppContext() MT-safe
 *
 * Revision 5.2  1996/07/16  19:58:01  vakatov
 * Nlm_GetScratchBuffer() now has a "size" parameter and has a variable
 * size -- thus it works as an fast and smart "ad hoc" memory allocator.
 * Added "corelib.h" header;  thread_id argum. added to new_AppContext(...);
 * delete_AppContext(): new cleaning funcs ReleaseApp{Err|Msg}Info called.
 * Added a code to test error posting functions(they use the ScratchBuffer
 * and Properties intensively) for multiple concurrent threads.
 *
 * Revision 5.1  1996/06/12  20:18:29  shavirin
 * Added multy-thread ability of toolkit to handle error posting
* ==========================================================================
*/

#include "corepriv.h"



/*	----------------------------------------------------------------------
 *	new_AppProperty			allocates/constructs an AppProperty struct
 *	delete_AppProperty		deallocates/destructs an AppProperty struct
 *
 *	Created: 
 *		06-04-93 Schuler
 *	Modified:
 *		00-00-00 YourName	What changes did you make?
 *	----------------------------------------------------------------------	*/

typedef struct _AppProperty_
{
	struct _AppProperty_	*next;
	char *key;
	void *data;
}
AppProperty;

static AppProperty * new_AppProperty PROTO((const char *key, void *data));
static void delete_AppProperty PROTO((AppProperty *prop));

static AppProperty * new_AppProperty (const char *key, void *data)
{
  AppProperty *prop = (AppProperty *)Calloc(1, sizeof(AppProperty));
  ASSERT_HARD(key != NULL);
  ASSERT_HARD(*key != '\0');
	
  if (prop != NULL)
    {
      if ( (prop->key = StrSave(key)) == NULL )
        {
          Free(prop);
          prop = NULL;
        }
      else
        prop->data = data;
    }

  return prop;
}


static void delete_AppProperty (AppProperty *prop)
{
	ASSERT_HARD(prop != NULL);
	MemFree(prop->key);
        MemFill(prop, '\xFF', sizeof(*prop));
	Free(prop);
}



/*	----------------------------------------------------------------------
 *	new_AppContext			allocates/constructs an AppContext struct
 *	delete_AppContext		deallocates/destructs an AppContext struct
 *
 *  Notes:  (1) If a valid context cannot be created, the application halts.
 *	    (2) The memory allocated for the AppContext struct is owned by
 *		the DLL and the scratch buffer is owned by the application.
 *	    (3) It is important that this function NOT call TRACE, Message,
 *		or ErrPost (or anything else that could result in any of these
 *		being called) as they use the applicaion context's scratch buffer.
 *         
 *          For every non-thread capable platform tid == 0
 *
 *	Created: 
 *		06-04-93 Schuler
 *	Modified:
 *		00-00-00 YourName	What changes did you make?
                06-12-96 Shavirin       Added TNlmThread tid element to
                                        distinguish contexes belong to diffrent threads
 *	----------------------------------------------------------------------	*/

/*
#define SCRATCH_SIZE_DEFAULT	(2*KBYTE)
*/

typedef struct _AppContext_
{
	struct _AppContext_ 	*next;
	struct _AppProperty_	*proplist;
   TNlmThread tid;

	unsigned	enums	:15; /* number of nested enumerations in-progress */
	unsigned	lock	:1;  /* if TRUE, property list is locked */
	size_t	scratch_size;
	void	*scratch;
}
AppContext;

		/* this is the only global variable in this file: */
static AppContext * g_appList;	/* Application Context List */

static AppContext * new_AppContext(TNlmThread thread_id);
static void delete_AppContext PROTO((AppContext *prop));
INLINE static void  AppContext_Lock PROTO((AppContext *context));
INLINE static void  AppContext_Unlock PROTO((AppContext *context));
INLINE static unsigned  AppContext_IsLocked PROTO((AppContext *context));


static AppContext * new_AppContext(TNlmThread thread_id)
{
	AppContext *context = (AppContext *)dll_Malloc(sizeof(AppContext));
	
	if (context == NULL)
		AbnormalExit(1);
		
	memset((void*)context,0,sizeof(struct _AppContext_));
   context->tid = thread_id; /* thread id for thread-capable OS */
	context->scratch_size = 0;
	context->scratch = NULL;
	return context;
}

static void delete_AppContext (AppContext *context)
{
  AppProperty *p1, *p2;
	
  ASSERT_HARD(context != NULL);
	
  ReleaseAppErrInfo();
  ReleaseAppMsgInfo();

  for (p1=context->proplist; p1; p1=p2)
    {
      p2 = p1->next;
      delete_AppProperty(p1);
    }

  Nlm_GetScratchBuffer( 0 );
  dll_Free(context);
}

static INLINE void AppContext_Lock (AppContext *context)
{
	ASSERT(context->lock==0);
	context->lock = 1;
}

static INLINE void AppContext_Unlock (AppContext *context)
{
	ASSERT(context->lock==1);
	context->lock = 0;
}

static INLINE unsigned AppContext_IsLocked (AppContext *context)
{
	return context->lock;
}



/*	----------------------------------------------------------------------
 *	InitAppContext -- Initializes a context struct for current application.
 *
 *  Notes:  
 *		If a valid context cannot be created, the application halts. 
 *		Although it is not strictly necessary to call InitAppContext() as
 *		contexts are created on-demand, calling it once at the beginning
 *		of the program may reduce heap fragmentation.
 *
 *	Created: 
 *		06-04-93 Schuler
 *	Modified:
 *              06-12-96 Shavirin   Added multy-thread ability of context handling
 *                                 
 *
 *	----------------------------------------------------------------------	*/

/* helper functions for internal use only */

static AppContext *GetAppContext (void)
{
  AppContext *p1, *p2;
  AppContext *app;
  TNlmThread thread_id = NlmThreadSelf(); /* thread ID */
	
  NlmMutexLockEx( &corelibMutex );

  /*
   *	First we scan the list of contexts for one with the current
   *	application's process ID.
   */
  for (p1=g_appList,p2=NULL; p1; p1=p1->next)
    {
      if ( NlmThreadCompare(p1->tid, thread_id) )
        {
          NlmMutexUnlock( corelibMutex );
          return p1;
        }
      p2 = p1;
    }
	
  /* 
   *	If we reach this point, the context for current does not 
   *	exist yet, so we need to create one and link it into the list.
   */
  app = new_AppContext(thread_id);

  if (p2 == NULL)	 
    g_appList = app;
  else  
    p2->next = app;	
  app->next = p1;

  NlmMutexUnlock( corelibMutex );

  return app;
}


NLM_EXTERN void LIBCALL Nlm_InitAppContext (void)
{
  GetAppContext ();
}


NLM_EXTERN char * LIBCALL Nlm_GetScratchBuffer(size_t size)
{
  AppContext *context = GetAppContext();
  if (context == NULL)
    abort();

  if ((size == 0  ||  context->scratch_size < size)  &&
      context->scratch != NULL)
    {
      /* reset scratch buffer */
      Free( context->scratch );
      context->scratch_size = 0;
      context->scratch = NULL;
    }

  /* nothing but reset */
  if (size == 0)
    return NULL;

  /* do we have enough allocated memory already? */
  if (context->scratch_size >= size)
    return (char *) context->scratch;

  /* allocate new buffer */
  size = (size + 15) / 16;
  size *= 16;
  if ((context->scratch = (char *) Malloc( size )) == NULL)
    return NULL;

  context->scratch_size = size;
  return (char *) context->scratch;
}



/*	----------------------------------------------------------------------
 *	ReleaseAppContext -- frees application context struct for current app.
 *
 *	Notes:
 *		For most platforms, memory will be recovered automatically by the
 *		operating system.  However, since we cannot guarantee this for
 *		all systems, it might be wise to call ReleaseAppContext() once
 *		just before the application exits.
 *
 *	Created: 
 *		06-04-93 Schuler
 *	Modified:
 *              06-12-96 Shavirin Added multi-thread ability of context handling
 *                                
 *
 *	----------------------------------------------------------------------	*/

NLM_EXTERN void LIBCALL Nlm_ReleaseAppContext (void)
{
  AppContext *p1, *p2;
  TNlmThread thread_id = NlmThreadSelf();

  NlmMutexLockEx( &corelibMutex );

  /*
   *	Scan the list for the context of the current app
   */
  for (p1=g_appList,p2=NULL; p1; p1=p1->next)
    {
      if ( NlmThreadCompare(p1->tid, thread_id) )
        break;
      p2 = p1;
    }
  /*
   *	Adjust links and release memory 
   */
  if (p1 != NULL)
    {
      AppContext *next = p1->next;
      delete_AppContext( p1 );

      if (p2 == NULL)
        g_appList = next;
      else
        p2->next  = next;
    }

  NlmMutexUnlock( corelibMutex );
}



/*	----------------------------------------------------------------------
 *	SetAppProperty -- Sets a data item int the application context's 
 *		property list, replacing the existing one or creating a new
 *		one if no property with that key exists.
 *
 *	Parameters:
 *		key:		key identifying the property
 *		value:		pointer to arbitrary data
 *
 *	Return value:	
 *		Previous value of the property, if any, or NULL otherwise.
 *
 *	Created: 
 *		06-08-93	Schuler
 *	Modified:
 *		00-00-00	YourName	What changes did you make?
 *	----------------------------------------------------------------------	*/

NLM_EXTERN void * LIBCALL Nlm_SetAppProperty (const char *key, void *data)
{
	if (key && *key)
	{
		AppContext *context = GetAppContext();
		AppProperty *p1, *p2, *prop;
		void *prev;
		int d;
		
		for (p1=context->proplist, p2=NULL; p1; p1=p1->next)
		{
			d = strcmp(key,p1->key);
			if (d < 0)  break;
			if (d==0)
			{
				prev = p1->data;
				p1->data = data;
				return prev;	/* previous value */
			}
			p2 = p1;
		}
		
		/*
		 *	If we reach here, a property with the given key does not exist, so 
		 *	let's create a new one and link it into the list.
		 */
		
		if (AppContext_IsLocked(context))
		{
			TRACE("SetAppProperty:  ** property list is locked **\n");
		}
		else
		{		
			AppContext_Lock(context);
			if ((prop = new_AppProperty(key,data)) != NULL)
			{
				if (p2 == NULL)	 
					context->proplist = prop;
				else  
					p2->next = prop;	
				prop->next = p1;
			}
			AppContext_Unlock(context);
		}
	}	
	return NULL;	/* no previous value */
}



/*	----------------------------------------------------------------------
 *	GetAppProperty -- Retrieves data value that was set with SetAppProperty.
 *
 *	Parameters:
 *		key:	key identifying the property
 *
 *	Return value:	
 *		Value that was set with SetAppProperty or NULL if no property with 
 *		that key exists.
 *
 *	Created: 
 *		06-08-93	Schuler
 *	Modified:
 *		00-00-00	YourName	What changes did you make?
 *
 *	----------------------------------------------------------------------	*/

NLM_EXTERN void * LIBCALL Nlm_GetAppProperty (const char *key)
{
	if (key && *key)
	{
		AppContext *context = GetAppContext();
		AppProperty *prop;
		
		for (prop=context->proplist; prop; prop=prop->next)
		{
			if (strcmp(prop->key,key) == 0)
				return prop->data;
		}
	}
	return NULL;
}



/*	----------------------------------------------------------------------
 *	RemoveAppProperty -- Removes a property from the application context's
 *		property list (if it exists) and returns the data value that was
 *		set with SetAppParam().
 *
 *	Parameters:
 *		key:	key identifying the property
 *
 *	Return value:	
 *		Value that was set with SetAppProperty or NULL if no property with 
 *		that key exists.
 *
 *	Notes:
 *		It is the responsibiliy of the caller to free whatever resources 
 *		the property's data (return value) may happen to point to.
 *
 *	Created: 
 *		06-08-93	Schuler
 *	Modified:
 *		00-00-00	YourName	What changes did you make?
 *
 *	----------------------------------------------------------------------	*/

NLM_EXTERN void * LIBCALL Nlm_RemoveAppProperty (const char *key)
{
	if (key && *key)
	{
		AppContext *context = GetAppContext();
		
		if (AppContext_IsLocked(context))
		{
			TRACE("RemoveAppProperty:  ** property list is locked **\n");
		}
		else
		{
			AppProperty *p1, *p2;
			int d;
			
			AppContext_Lock(context);
			for (p1=context->proplist, p2=NULL; p1; p1=p1->next)
			{
				d = strcmp(key,p1->key);
				if (d < 0)  break;
				if (d==0)
				{
					void *data = p1->data;
					if (p2 == NULL)
						context->proplist = p1->next;
					else
						p2->next = p1->next;
					delete_AppProperty(p1);
					AppContext_Unlock(context);
					return data;	/* success */
				}
				p2 = p1;
			}
			AppContext_Unlock(context);
		}
	}
	return NULL;	/* failure */		
}



/*	----------------------------------------------------------------------
 *	EnumAppProperties -- Enumerates all application properties, calling
 *		a caller-supplied callback function with the key and data for
 *		each one.
 *
 *	Parameters:
 *		Pointer to the enumeration callback procedure.
 *
 *	Return value:
 *		Number of properties enumerated.
 *
 *	Callback function:
 *
 *		int LIBCALLBACK MyEmumProc (const char *key, void *value)
 *		{
 *			//--- insert your code here ---
 *
 *			// for example:
 *			if (strcmp(key,"MyBigBuffer") ==0)
 *			{
 *				SetAppProperty(key,NULL);
 *				MemFree(data)
 *				return FALSE;	// FALSE to stop enumeration at this point
 *			}
 *			return TRUE;		// TRUE to continue the enumeration.
 *		}
 *
 *	Notes:
 *		It is OK to call SetAppProperty() from within the callback function,
 *		but _only_ to change the value of an existing property.  Any attempt 
 *		to alter the property list, either by calling SetAppProperty() with 
 *		a new key or calling RemoveAppProperty() will fail while an enumera-
 *		tion is in progress.
 *
 *	Created: 
 *		06-09-93	Schuler
 *	Modified:
 *		00-00-00	YourName	What changes did you make?
 *
 *	----------------------------------------------------------------------	*/

NLM_EXTERN int LIBCALL Nlm_EnumAppProperties (Nlm_AppPropEnumProc proc)
{
	int count = 0;
	if (proc != NULL)
	{
		AppContext *context = GetAppContext();
		AppProperty *prop;
		
		if (context->enums==0)
			AppContext_Lock(context);
		context->enums++;
		for (prop=context->proplist; prop; prop=prop->next)
		{
			count ++;
			if ( ! (*proc)(prop->key,prop->data) )
				break;
		}
		context->enums--;
		if (context->enums==0)
			AppContext_Unlock(context);
	}
	return count;
}


/*	----------------------------------------------------------------------
 *	GetAppProcessID  [Schuler, 06-04-93]
 *
 *	Returns an identifier for the current application instance.
 *
 *	Notes:
 *	On the Macintosh, the process ID is a 64-bit value, which is being
 *	condensed down to 32-bits here by XORing the high and low halves
 *	of the value.  I can't guarantee it will be unique (although it
 *	seems to be in practice), but this code is not being dynamically
 *	linked on the Mac, so it doesn't matter.
 *
 *	MODIFICATIONS
 *	04-10-93  Schuler  Added Macintosh version.
 *	12-16-93  Schuler  Added Borland version contributed by M.Copperwhite
 *	----------------------------------------------------------------------	*/

#define USE_GETPID

/* (insert other platform-specific versions here as necessary and #undef USE_GETPID) */


/* ----- Macintosh Version ----- */
#ifdef OS_MAC
#include <Processes.h>
#include <Gestalt.h>

NLM_EXTERN long LIBCALL Nlm_GetAppProcessID (void)
{
	long gval;
	ProcessSerialNumber psn;	/* a 64-bit value*/
	
    if (Gestalt (gestaltSystemVersion, &gval) == noErr && (short) gval >= 7 * 256) {
		GetCurrentProcess(&psn);	
		return (psn.highLongOfPSN ^ psn.lowLongOfPSN);	/* merge to 32-bits */
	} else {
		return 1;
	}
}

#undef USE_GETPID
#endif


/* ----- NCBIDLL mod Borland DLL version - call Windows API to get PSP ----- */
#ifdef _WINDLL
#ifdef COMP_BOR

NLM_EXTERN long LIBCALL Nlm_GetAppProcessID (void)
{
  return GetCurrentPDB();
}

#undef USE_GETPID
#endif
#endif


/* ----- Generic Version ----- */
#ifdef USE_GETPID
#if defined(COMP_MSC) || defined(COMP_BOR)
#include <process.h>
#endif

NLM_EXTERN long LIBCALL Nlm_GetAppProcessID (void)
{
	return getpid();
}

#endif



#ifdef TEST_MODULE_NCBIPROP

/***********************************************************************
 *  TEST
 ***********************************************************************/

#include <stdio.h>
#include <ncbistd.h>
#include <tsprintf.h>

FILE *STDOUT = (FILE *)stdout;
FILE *STDERR = (FILE *)stderr;

static Nlm_VoidPtr TEST__MyThread(Nlm_VoidPtr arg)
{
  Nlm_Int4 thread_no = (Nlm_Int4) arg;
  Nlm_Int4 n, i;
  Nlm_CharPtr str = NULL;
  const Nlm_Char PNTR s = NULL;
  
  fprintf(STDOUT, "TEST__MyThread():  Thread #%ld(%ld) started\n",
          (long)NlmThreadSelf(), (long)thread_no);

  n = (thread_no + 1) * 20;
  str = (Nlm_CharPtr)Nlm_MemNew( n );
  for (i = 10;  i < n;  i += 10)
    {
      Nlm_MemSet(str, 'a', i);
      str[i] = '\0';
      s = TSPrintf("TEST__MyThread() #%ld: %u %d %e %g %f %p %s +++\n",
                   (long)thread_no, (unsigned int)i, (int)i, (double)i,
                   (double)i, (double)i, (void *)&i, str);
      fprintf(STDOUT, "%p  %ld: %s\n", (void *)s, Nlm_StrLen( s ), s);
    }

  Nlm_MemFree( str );

#if defined (OS_UNIX)
  sleep(thread_no + 1);
#elif defined (WIN32)
  Sleep(1000 * (thread_no + 1));
#endif

  
  fprintf(STDOUT, "TEST__MyThread() after sleep #%ld: %p  %ld: %s\n",
          (long)thread_no, (void *)s, Nlm_StrLen( s ), s);

  switch (thread_no % 4)
    {
    case 0:
      Nlm_Message(MSG_OKC,
                  "TEST__MyThread() #%ld: Nlm_Message(MSG_OKC)\n",
                  (long)thread_no);
      break;

    case 1:
      Nlm_MsgAlert (MSG_YN,  MSG_POST,
                    "Nlm_MsgAlert(): Caption\n",
                    "TEST__MyThread() #%ld: Nlm_MsgAlert(MSG_YN,MSG_POST)\n",
                    (long)thread_no);
      break;

    case 2:
      Nlm_ErrPost (CTX_DEBUG/*CTX_UNKNOWN*/,  1,
                    "TEST__MyThread() #%ld: Nlm_ErrPost(CTX_DEBUG,1)\n",
                    (long)thread_no);
      break;

    case 3:
      Nlm_ErrPostEx (SEV_ERROR, 1, 2,
                    "TEST__MyThread() #%ld: Nlm_ErrPostEx(0,1,2)\n",
                    (long)thread_no);
      break;
    }

  return NULL;
}


static void MyExitMessage(Nlm_VoidPtr arg)
{
  fprintf(STDERR, "\n%s\n", (Nlm_CharPtr)arg);
  Nlm_MemFree( arg ); 
}

static Nlm_Int4 TEST__scratch(Nlm_Int4 n_threads)
{
  Nlm_Int4 t;
  Nlm_Int4 err_code = 0;
  TNlmThread *threads = Nlm_MemNew(n_threads * sizeof(TNlmThread));

  for (t = 0;  t < n_threads;  t++)
    {
      switch (t % 2)
        {
        case 0:
          threads[t] = NlmThreadCreate(TEST__MyThread, (Nlm_VoidPtr)t,
                                       THR_RUN);
          break;
        case 1:
          {
            Nlm_CharPtr exit_message = Nlm_MemNew( 64 );
            sprintf(exit_message, "Exit Message, thread #%d", (int)t);
            threads[t] = NlmThreadCreateEx(
              TEST__MyThread, (Nlm_VoidPtr)t, THR_RUN,
              MyExitMessage, (Nlm_VoidPtr)exit_message);
            break;
          }
        }
      fprintf(STDOUT, "TEST__scratch():  Starting thread #%ld\n",
              (long)threads[t]);
    }

  for (t = 0;  t < n_threads;  t++)
    {
      Nlm_VoidPtr status;
      err_code += NlmThreadJoin(threads[t], &status);
      if (err_code != 0)
        {
          fprintf(STDOUT, "TEST__scratch():  Cannot join thread #%ld;  \
error code = %ld\n",
                  (long)threads[t], (long)err_code);
          break;
        }

      fprintf(STDOUT, "TEST__scratch():  Thread #%ld joined;  \
terminated, exit status = %p\n",
              (long)threads[t], status);
    }

  Nlm_MemFree( threads );  

  fprintf(STDOUT, "TEST__scratch():  FINISHED\n");
  return err_code;
}


Nlm_Int2 Nlm_Main( void )
{
#if defined(WIN32) && defined(_WINDOWS)
  (FILE *)STDOUT = freopen("stdout.w95", "w", stdout);
  (FILE *)STDERR = freopen("stderr.w95", "w", stderr);
  if (!STDOUT  ||  !STDERR)
    abort();
#endif
      
  return (Nlm_Int2)TEST__scratch( 10 );
}

#endif  /* TEST_MODULE_NCBIPROP */
