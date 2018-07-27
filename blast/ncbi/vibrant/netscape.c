/* $Id: netscape.c,v 6.7 2001/03/19 23:15:46 vakatov Exp $
 * Copyright © 1996 Netscape Communications Corporation, all rights reserved.
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
* File Name:  $RCSfile: netscape.c,v $
*
* Author:  Sergei Shavirin
*
* Initial Version Creation Date: 11/08/1997
*
* $Revision: 6.7 $
*
* File Description:
*        API to remote-control Netscape(run Netscape browsers, open URLs)
*
* $Log: netscape.c,v $
* Revision 6.7  2001/03/19 23:15:46  vakatov
* NS_LoadNetscape() -- try execlp() in addition to execl(), and do not forget
* to exit on error. The bug was caught by H.Feldman <feldman@mshri.on.ca>.
*
* Revision 6.6  2000/01/13 23:37:14  beloslyu
* changes because of port to HP-UX 11.0
*
* Revision 6.5  1999/11/08 21:04:40  sinyakov
* Corrected const char* to char* assignmetn warning
*
* Revision 6.4  1999/10/15 21:19:40  sinyakov
* Fixed NS_SendCommand() to support long URLs
*
* Revision 6.3  1998/05/29 15:43:11  vakatov
* Patched this funny unstable prototype for "gethostname()"
*
* Revision 6.2  1998/01/22 17:06:43  vakatov
* Added "Xatom.h" header;  use TRACE instead of fprintf() & perror()
*
* 01/21/98 D.Vakatov: cleaned up, Purify'd, CoreLib'd, prepared to ci, ci'd
* ==========================================================================
*/

#include <vibtypes.h>
#include <vibprocs.h>
#include <vibincld.h>
#include <netscape.h>


#ifdef WIN_X

#include <X11/Xatom.h>
#if !defined(__hpux)
#include <X11/Xmu/WinUtil.h>
#endif

#define MOZILLA_VERSION_PROP   "_MOZILLA_VERSION"
#define MOZILLA_LOCK_PROP      "_MOZILLA_LOCK"
#define MOZILLA_COMMAND_PROP   "_MOZILLA_COMMAND"
#define MOZILLA_RESPONSE_PROP  "_MOZILLA_RESPONSE"
#define MOZILLA_URL_PROP       "_MOZILLA_URL"

#ifdef OS_UNIX_IRIX
#define NETSCAPE_PATH "/usr/ncbi/bin/netscape"
#else
#define NETSCAPE_PATH "/usr/ncbi/X11/bin/netscape"
#endif

#define DEFAULT_URL "http://www.ncbi.nlm.nih.gov"

typedef struct NS_WindowTag {
    Display *dpy;
    Window  window;
} NS_WindowStruct;

static Atom XA_MOZILLA_URL      = 0;
static Atom XA_MOZILLA_VERSION  = 0;
static Atom XA_MOZILLA_LOCK     = 0;
static Atom XA_MOZILLA_COMMAND  = 0;
static Atom XA_MOZILLA_RESPONSE = 0;

#define NO_NETSCAPE_LOADED 0
#define EXACT_URL_MATCH    1
#define SOME_WINDOW_FOUND  2

static Boolean NS_Init_atoms (Display *dpy)
{
    
    if (! XA_MOZILLA_URL)
        XA_MOZILLA_URL = XInternAtom (dpy, MOZILLA_URL_PROP, False);
    if (! XA_MOZILLA_VERSION)
        XA_MOZILLA_VERSION = XInternAtom (dpy, MOZILLA_VERSION_PROP, False);
    if (! XA_MOZILLA_LOCK)
        XA_MOZILLA_LOCK = XInternAtom (dpy, MOZILLA_LOCK_PROP, False);
    if (! XA_MOZILLA_COMMAND)
        XA_MOZILLA_COMMAND = XInternAtom (dpy, MOZILLA_COMMAND_PROP, False);
    if (! XA_MOZILLA_RESPONSE)
        XA_MOZILLA_RESPONSE = XInternAtom (dpy, MOZILLA_RESPONSE_PROP, False);

    return TRUE;
}

static Nlm_Boolean NS_Find_window(NS_Window nswin, const Nlm_Char *url_to_load)
{
    Nlm_Int4 i;
    Window root, root2, parent, *kids;
    unsigned int nkids;
    Window tentative = 0;
    Nlm_Boolean retvalue = NO_NETSCAPE_LOADED;
    
    if (!nswin  ||  !nswin->dpy)
        return retvalue;
    
    root = RootWindowOfScreen (DefaultScreenOfDisplay (nswin->dpy));
 
    if (! XQueryTree (nswin->dpy, root, &root2, &parent, &kids, &nkids)) {
        Message(MSG_ERROR, "NS_Find_window: XQueryTree failed on display %s",
                DisplayString (nswin->dpy));
        return retvalue;
    }
    
    /* root != root2 is possible with virtual root WMs. */
    
    if (!(kids && nkids)) {
        Message(MSG_ERROR, "NS_Find_window: Root window has no children "
                "on display %s",
                DisplayString (nswin->dpy));
        return retvalue;
    }
    
    for (i = nkids-1; i >= 0; i--) {
        Atom type;
        int format;
        unsigned long nitems, bytesafter;
        unsigned char *urlstring = NULL;
        Window w = XmuClientWindow (nswin->dpy, kids[i]);
        
        int status = XGetWindowProperty (nswin->dpy, w, XA_MOZILLA_URL,
                                         0, (65536 / sizeof (long)),
                                         False, XA_STRING,
                                         &type, &format, &nitems, &bytesafter,
                                         &urlstring);
        
        if (status != Success || type == None)
            continue;
        
        if (urlstring == NULL)
            continue;
        
        /* So... We have found at least one Netscape Window ... Cool.. */
        if (!tentative  &&  StringStr((Nlm_CharPtr)urlstring, url_to_load)) {
            nswin->window = w;
            XFree(urlstring);
            retvalue = SOME_WINDOW_FOUND;
            continue;
        } else {
            nswin->window = w;
            XFree(urlstring);
            retvalue = EXACT_URL_MATCH;
            break;
        }
    }
    XFree(kids);
    return retvalue;
}

static Nlm_Boolean NS_Check_window (NS_Window nswin)
{
    Atom type;
    int format;
    unsigned long nitems, bytesafter;
    unsigned char *version = 0;
    int status;

    if (!nswin  ||  !nswin->dpy  ||  !nswin->window)
        return FALSE;
    
    status = XGetWindowProperty (nswin->dpy, nswin->window, 
                                 XA_MOZILLA_VERSION,
                                 0, (65536 / sizeof (long)),
                                 False, XA_STRING,
                                 &type, &format, &nitems, &bytesafter,
                                 &version);
    if (status != Success || !version) {
        if(version != NULL)
            XFree (version);
        return FALSE;
    }
    return TRUE;
}


static char *lock_data = 0;


/* patch to old SunOS/Solaris proto(cut&paste from Solaris 2.6 "unistd.h") */
#ifdef __cplusplus
extern "C" {
#endif

#if defined(OS_UNIX_SOL) || defined(OS_UNIX_SUN)
#if defined(_XPG4_2)
extern int gethostname(char *, size_t);
#elif  defined(__EXTENSIONS__) || \
        (!defined(_POSIX_C_SOURCE) && !defined(_XOPEN_SOURCE))
extern int gethostname(char *, int);
#endif
#endif

#ifdef __cplusplus
}
#endif


static Nlm_Boolean NS_Obtain_lock (NS_Window nswin)
{
    Bool locked = False;
    Bool waited = False;
    
    if (! lock_data) {
        lock_data = (char *) malloc (255);
        sprintf (lock_data, "pid%d@", (int)getpid());
        if (gethostname (lock_data + strlen (lock_data), 100)) {
            TRACE ( "gethostname() failed" );
            return FALSE;
        }
    }
    
    do {
        int result;
        Atom actual_type;
        int actual_format;
        unsigned long nitems, bytes_after;
        unsigned char *data = 0;
        
        /* ################################# DANGER! */
        XGrabServer (nswin->dpy);
        
        result = XGetWindowProperty (nswin->dpy, nswin->window, 
                                     XA_MOZILLA_LOCK,
                                     0, (65536 / sizeof (long)),
                                     False, /* don't delete */
                                     XA_STRING,
                                     &actual_type, &actual_format,
                                     &nitems, &bytes_after,
                                     &data);
        if (result != Success || actual_type == None) {
            /* It's not now locked - lock it. */
#ifdef DEBUG_PROPS
            TRACE ( "NS_Obtain_lock: (writing " MOZILLA_LOCK_PROP
                    " \"%s\" to 0x%x)\n",
                    lock_data, (unsigned int) window);
#endif
            XChangeProperty (nswin->dpy, nswin->window, 
                             XA_MOZILLA_LOCK, XA_STRING, 8,
                             PropModeReplace, (unsigned char *) lock_data,
                             strlen (lock_data));
            locked = True;
        }
        
        XUngrabServer (nswin->dpy); 
        /* ################################# danger over */

        XSync (nswin->dpy, False);
        
        if (! locked) {
            /* We tried to grab the lock this time, and failed because someone
               else is holding it already.  So, wait for a PropertyDelete event
               to come in, and try again. */
            
            Message(MSG_OK, "NS_Obtain_lock: window 0x%x is "
                    "locked by %s; waiting...\n",
                    (unsigned int) nswin->window, data);
            
            waited = True;
            
            while (TRUE) {
                XEvent event;
                XNextEvent (nswin->dpy, &event);
                if (event.xany.type == DestroyNotify &&
                    event.xdestroywindow.window == nswin->window) {
                    Message(MSG_ERROR, "NS_Obtain_lock: "
                            "window 0x%x unexpectedly destroyed.\n",
                            (unsigned int) nswin->window);
                    return FALSE;
                } else if (event.xany.type == PropertyNotify &&
                           event.xproperty.state == PropertyDelete &&
                           event.xproperty.window == nswin->window &&
                           event.xproperty.atom == XA_MOZILLA_LOCK) {
                    /* Ok!  Someone deleted their lock, so now we can try
                       again. */
#ifdef DEBUG_PROPS
                    TRACE ( "NS_Obtain_lock: "
                            "(0x%x unlocked, trying again...)\n",
                            (unsigned int) nswin->window);
#endif
                    break;
                }
            }
        }
        if (data)
            XFree (data);
    } while (! locked);

#ifdef DEBUG_PROPS
    if (waited) {
        TRACE ( "NS_Obtain_lock: obtained lock.\n");
    }
#endif

    return TRUE;
}

static Boolean NS_Free_lock (NS_Window nswin)
{
    int result;
    Atom actual_type;
    int actual_format;
    unsigned long nitems, bytes_after;
    unsigned char *data = 0;
    
#ifdef DEBUG_PROPS
    TRACE ( "NS_Free_lock: (deleting %s \"%s\" from 0x%x)\n",
            MOZILLA_LOCK_PROP, lock_data, (unsigned int) nswin->window);
#endif

    result = XGetWindowProperty (nswin->dpy, nswin->window, XA_MOZILLA_LOCK,
                                 0, (65536 / sizeof (long)),
                                 True, /* atomic delete after */
                                 XA_STRING,
                                 &actual_type, &actual_format,
                                 &nitems, &bytes_after,
                                 &data);
    if (result != Success) {
        TRACE ( "NS_Free_lock: unable to read and delete %s property\n", 
                MOZILLA_LOCK_PROP);
        return FALSE;
    } else if (!data || !*data) {
        TRACE ( "NS_Free_lock: invalid data on %s of window 0x%x.\n",
                MOZILLA_LOCK_PROP, (unsigned int) nswin->window);
        return FALSE;
    } else if (strcmp ((char *) data, lock_data)) {
        TRACE ( "NS_Free_lock: %s was stolen! Expected \"%s\", "
                "saw \"%s\"!\n", MOZILLA_LOCK_PROP, lock_data, data);
        return FALSE;
    }
    
    if (data)
        XFree (data);

    return TRUE;
}


static int NS_Command(NS_Window nswin, const char *command, Bool raise_p)
{
    int result;
    Bool done = False;
    char *new_command = 0;
    
    /* The -noraise option is implemented by passing a "noraise" argument
       to each command to which it should apply.
       */
    if (! raise_p) {
        char *close;
        new_command = (char *) malloc (strlen (command) + 20);
        strcpy (new_command, command);
        close = strrchr (new_command, ')');
        if (close)
            strcpy (close, ", noraise)");
        else
            strcat (new_command, "(noraise)");
        command = new_command;
    }
    
#ifdef DEBUG_PROPS
    TRACE ( "NS_Command: (writing %s \"%s\" to 0x%x)\n",
            MOZILLA_COMMAND_PROP, command, (unsigned int) nswin->window);
#endif
    
    XChangeProperty (nswin->dpy, nswin->window, XA_MOZILLA_COMMAND, 
                     XA_STRING, 8,
                     PropModeReplace, (unsigned char *) command,
                     strlen (command));
    
    while (!done) {
        XEvent event;
        XNextEvent (nswin->dpy, &event);
        if (event.xany.type == DestroyNotify &&
            event.xdestroywindow.window == nswin->window) {
            /* Print to warn user...*/
            TRACE ( "NS_Command: window 0x%x was destroyed.\n",
                    (unsigned int) nswin->window);
            result = 6;
            goto DONE;
        }
        else if (event.xany.type == PropertyNotify &&
                 event.xproperty.state == PropertyNewValue &&
                 event.xproperty.window == nswin->window &&
                 event.xproperty.atom == XA_MOZILLA_RESPONSE) {
            Atom actual_type;
            int actual_format;
            unsigned long nitems, bytes_after;
            unsigned char *data = 0;
            
            result = XGetWindowProperty (nswin->dpy, nswin->window, 
                                         XA_MOZILLA_RESPONSE,
                                         0, (65536 / sizeof (long)),
                                         True, /* atomic delete after */
                                         XA_STRING,
                                         &actual_type, &actual_format,
                                         &nitems, &bytes_after,
                                         &data);
#ifdef DEBUG_PROPS
	  if (result == Success && data && *data) {
	      TRACE ( "NS_Command: (server sent %s \"%s\" to 0x%x.)\n",
                 MOZILLA_RESPONSE_PROP, data, (unsigned int) nswin->window);
     }
#endif
          
	  if (result != Success) {
	      TRACE ( "NS_Command: failed reading %s from window 0x%0x.\n",
                 MOZILLA_RESPONSE_PROP, (unsigned int)nswin->window);
	      result = 6;
	      done = True;
     } else if (!data || strlen((char *) data) < 5) {
	      TRACE ( "NS_Command: invalid data on %s property of window 0x%0x.\n",
                 MOZILLA_RESPONSE_PROP, (unsigned int)nswin->window);
	      result = 6;
	      done = True;
     } else if (*data == '1') {	/* positive preliminary reply */
	      TRACE ( "NS_Command: %s\n", data + 4);
	      /* keep going */
	      done = False;
     }
	  else if (!strncmp ((char *)data, "200", 3)) /* positive completion */
       {
         result = 0;
         done = True;
       }
	  else if (*data == '2')		/* positive completion */
       {
           TRACE ( "NS_Command: %s\n",data + 4);
           result = 0;
           done = True;
       }
     else if (*data == '3') /* positive intermediate reply */
       {
           TRACE ( "NS_Command: internal error: "
                   "server wants more information?  (%s)\n", data);
           result = 3;
           done = True;
       }
	  else if (*data == '4' ||	/* transient negative completion */
              *data == '5')	/* permanent negative completion */
       {
           TRACE ( "NS_Command: %s\n", data + 4);
           result = (*data - '0');
           done = True;
       }
     else
       {
           TRACE ( "NS_Command: unrecognised %s from "
                   "window 0x%x: %s\n",
                   MOZILLA_RESPONSE_PROP, 
                   (unsigned int) nswin->window, data);
           result = 6;
           done = True;
       }
          
	  if (data)
       XFree (data);
        }
#ifdef DEBUG_PROPS
        else if (event.xany.type == PropertyNotify &&
                 event.xproperty.window == nswin->window &&
                 event.xproperty.state == PropertyDelete &&
                 event.xproperty.atom == XA_MOZILLA_COMMAND) {
            TRACE ( "NS_Command: (server 0x%x has accepted %s.)\n",
                    MOZILLA_COMMAND_PROP, (unsigned int) nswin->window);
        }
#endif /* DEBUG_PROPS */
    }
    
 DONE:
    
    if (new_command)
        free (new_command);
    
    return result;
}


static Boolean NS_Init(NS_WindowPtr window)
{
    NS_Window nswin;
    
    if(window == NULL)
        return FALSE;
    
    if(*window != NULL)
        return TRUE;
    
    *window = nswin = (NS_Window)MemNew(sizeof(NS_WindowStruct));
    
    if((nswin->dpy = XOpenDisplay(NULL)) == NULL) {
        MemFree(nswin);
        *window = NULL;
        return FALSE;
    }

    NS_Init_atoms(nswin->dpy);
    return TRUE;
}


static Boolean NS_LoadNetscape(const char *url)
{
    int childpid = fork();
    
    if (childpid < 0) { 
        return FALSE;
    }
    
    if (childpid > 0) {
        /* ----- parent process  ---------- */
        /* Nothing much to be done here ... */
        return TRUE;
        /* -------------------------------- */
    }

    /* ---------- child process ------------ */
    if (execlp("netscape", "netscape", url, NULL) < 0  &&
        execl(NETSCAPE_PATH, NETSCAPE_PATH, url, NULL) < 0) {
        Message(MSG_ERROR, "Failure to open URL in netscape window");
        exit(1);
    }

    /* ----- must never get to this code! ----- */
    ASSERT(0);
    return FALSE;
}


extern void NS_ResetWindow(NS_Window window)
{
    if ( window )
        window->window = 0;
}


extern Nlm_Boolean NS_SendCommand(NS_WindowPtr   window,
                                  const Nlm_Char *cmd, 
                                  const Nlm_Char *url,
                                  const Nlm_Char *wintag,
                                  Nlm_Boolean    raise)
{
    int status = 0, find_status = EXACT_URL_MATCH;
    NS_Window nswin;
    Nlm_CharPtr command = 0;

    if(!window  ||  (!cmd  &&  !url))
        return FALSE;
    
    if(cmd  &&  url)
        NS_ResetWindow(*window);
    
    if ( !url )
        url = DEFAULT_URL;
    
    if ( !NS_Init(window) )
        return FALSE;
    
    nswin = *window;
    if ( !NS_Check_window(nswin) ) {
        find_status = NS_Find_window(nswin, url);
        
        switch(find_status) {    
        case EXACT_URL_MATCH:
            break;
        case SOME_WINDOW_FOUND:
            break;
        case NO_NETSCAPE_LOADED:
        default:
            NS_LoadNetscape(url);
            return TRUE;
        }
    }
    
    /* OK We have valid existing window of Netscape and now play with it */
    XSelectInput(nswin->dpy, nswin->window, 
                 (PropertyChangeMask|StructureNotifyMask));
    
    if ( !cmd ) {
      const Nlm_Char *wintag_t = wintag ? wintag : "VibrantInterface";
      command = (Nlm_CharPtr) MemNew (strlen (url) + strlen (wintag_t) + 20);
      sprintf(command, "openURL(%s, %s)",
              url, wintag_t);
    }
    
    NS_Obtain_lock(nswin); 
    
    status = NS_Command(nswin, cmd ? cmd : command, raise);
    
    /* When status = 6, it means the window has been destroyed */
    /* It is invalid to free the lock when window is destroyed. */
    if (status != 6)
        NS_Free_lock(nswin); 

    if(find_status == SOME_WINDOW_FOUND)
        NS_Find_window (nswin, url);

    MemFree(command);
    
    return status ? FALSE : TRUE;
}


extern Nlm_Boolean NS_OpenURL(NS_WindowPtr window,
                              const Nlm_Char *url, const Nlm_Char *wintag,
                              Nlm_Boolean raise)
{
    return NS_SendCommand(window, NULL, url, wintag, raise);
}


extern void NS_WindowFree(NS_Window window)
{
    if ( window )
        MemFree(window);
}

#else  /* WIN_X */

typedef struct NS_WindowTag {
  int dummy;
} NS_WindowStruct;


extern void NS_ResetWindow(NS_Window window) {
  return;
}
extern Nlm_Boolean NS_SendCommand(NS_WindowPtr   window,
                                  const Nlm_Char *cmd, 
                                  const Nlm_Char *url,
                                  const Nlm_Char *wintag,
                                  Nlm_Boolean    raise) {
  return FALSE;
}
extern Nlm_Boolean NS_OpenURL(NS_WindowPtr window,
                              const Nlm_Char *url, const Nlm_Char *wintag,
                              Nlm_Boolean raise) {
  return FALSE;
}
extern void NS_WindowFree(NS_Window window) {
  return;
}

#endif /* WIN_X */


/************************************************************************/

#ifdef TEST_MODULE_NETSCAPE

#define SB_URL "http://inhouse.ncbi.nlm.nih.gov:6224/cgi-bin/SmartBlast/nph-smblast"
#define SB_ACC_URL "http://inhouse.ncbi.nlm.nih.gov:6224/cgi-bin/SmartBlast/sub_blast.pl"
#define SB_SEARCH_TAG    "search_form"
#define SB_ACCESSION_TAG "accession_list"

static Nlm_Boolean NS_LoadSmartBlast(NS_WindowPtr window, char *accession)
{
    Nlm_Char tmp[512];
    
    sprintf(tmp, "%s?acc=%s", SB_URL, accession);
    
    if(!NS_OpenURL(window, tmp, NULL, TRUE)) {
        Message(MSG_ERROR, "Failure to open URL in netscape window");
        return FALSE;
    }
    return TRUE;
}

Nlm_Int2 Main(void)
{
    NS_Window window = NULL;

    if (Nlm_GetArgc() < 2) {
        TRACE ( "Usage: %s <accession>\n", Nlm_GetArgv()[0] );
        exit(1);
    }
    
    if ( NS_LoadSmartBlast(&window, Nlm_GetArgv()[1]) )
      NS_WindowFree(window);

    return 0;
}
#endif /* TEST_MODULE_NETSCAPE */
