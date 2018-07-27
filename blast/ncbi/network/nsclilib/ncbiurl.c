/*   ncbiurl.c
* ===========================================================================
*
*                            PUBLIC DOMAIN NOTICE
*            National Center for Biotechnology Information (NCBI)
*
*  This software/database is a "United States Government Work" under the
*  terms of the United States Copyright Act.  It was written as part of
*  the author's official duties as a United States Government employee and
*  thus cannot be copyrighted.  This software/database is freely available
*  to the public for use. The National Library of Medicine and the U.S.
*  Government do not place any restriction on its use or reproduction.
*  We would, however, appreciate having the NCBI and the author cited in
*  any work or product based on this material
*
*  Although all reasonable efforts have been taken to ensure the accuracy
*  and reliability of the software and data, the NLM and the U.S.
*  Government do not and cannot warrant the performance or results that
*  may be obtained by using this software or data. The NLM and the U.S.
*  Government disclaim all warranties, express or implied, including
*  warranties of performance, merchantability or fitness for any particular
*  purpose.
*
* ===========================================================================
*
* File Name:  ncbiurl.c
*
* Author:  Jonathan Kans
*
* Version Creation Date:   8/12/98
*
* $Revision: 6.10 $
*
* File Description: 
*
* Modifications:  
* --------------------------------------------------------------------------
* $Log: ncbiurl.c,v $
* Revision 6.10  1999/03/11 15:20:21  vakatov
* Added "timeout" arg to SOCK_Create() and SOCK_Reconnect()
*
* Revision 6.9  1999/01/07 17:52:36  kans
* added programName and mydata parameters, fixed bad userAgentName code that Alexey caught and CodeWarrior missed
*
* Revision 6.8  1999/01/05 21:35:40  kans
* pass program name as User-Agent
*
* Revision 6.7  1998/11/19 22:51:45  kans
* name is supplemental_input
*
* Revision 6.6  1998/11/19 21:49:34  kans
* switching from GET to POST
*
* Revision 6.5  1998/11/18 20:42:39  kans
* if still needs question mark before arguments, then does not need ampersand
*
* Revision 6.4  1998/11/09 19:16:05  vakatov
* Added "NLM_EXTERN" to function prototypes
*
* ==========================================================================
*/

#include <ncbiurl.h>

/*****************************************************************************
*
*   SOCK_SendURLQuery is passed the host machine (without http://), the port
*     (80 is the conventional public port), and a path.  The path can include
*     the full query (including a question mark to separate path from query),
*     or a separate query string (without a question mark) can be passed in.
*     An additional arguments parameter can also be passed in, and an ampersand
*     is used to separate the query and arguments parameters.
*
*   SOCK_CheckURLQuery returns the number of pending URL queries.  Call this
*     function at least once every few seconds.  This function polls the sockets,
*     and calls the designated result procedure (written by the user) to handle
*     the resulting data file.
*
*****************************************************************************/

#define URL_QUERY_BUFLEN  2048

typedef struct querydata {
  Nlm_Uint4      timeoutsec;
  URLResultProc  resultproc;
  SOCK           sock;
  Nlm_Boolean    receiveBinary;
  Nlm_VoidPtr    mydata;
} QueryData, PNTR QueryPtr;

static ValNodePtr  urlquerylist = NULL;

static Nlm_Boolean HasNoText (Nlm_CharPtr str)

{
  Nlm_Char  ch;

  if (str != NULL) {
    ch = *str;
    while (ch != '\0') {
      if (ch > ' ') {
        return FALSE;
      }
      str++;
      ch = *str;
    }
  }
  return TRUE;
}

NLM_EXTERN Nlm_Boolean SOCK_SendURLQuery (Nlm_CharPtr host_machine, Nlm_Uint2 host_port,
                                      Nlm_CharPtr host_path, Nlm_CharPtr query,
                                      Nlm_CharPtr arguments, Nlm_CharPtr data_file,
                                      URLResultProc resultproc, Nlm_Uint4 timeoutsec,
                                      Nlm_Boolean sendBinary, Nlm_Boolean receiveBinary,
                                      Nlm_CharPtr programName, Nlm_VoidPtr mydata)

{
  Nlm_Boolean    needsAmpsnd = TRUE;
  Nlm_Boolean    needsQmark = TRUE;
  Nlm_Boolean    ok;
  Nlm_CharPtr    buffer;
  ESOCK_ErrCode  err_code;
  SOCK           sock;
  FILE           *fp = (FILE*)0;
  size_t         ct;
  Nlm_Int4       len;
  Nlm_Uint4      cnt;
  Nlm_Uint4      n_written;
  Nlm_Char       path [PATH_MAX];
  QueryPtr       qp;
  STimeout       timeout;
  Nlm_CharPtr    userAgentName = NULL;
  Nlm_CharPtr    mode;

  if (HasNoText (host_machine) || HasNoText (host_path)) return FALSE;

  timeout.sec  = timeoutsec;
  timeout.usec =  0;

  /* Connect to the server */  
  if (SOCK_Create(host_machine, host_port, &timeout, &sock)
      != eSOCK_ESuccess) {
    ErrPostEx(SEV_ERROR, 0, 0,
              "[SendURLQuery] Cannot connect to server \"%s\", port %d;",
              host_machine, (int)host_port);
    return FALSE;
  }

  /* Setup the connection's i/o timeout */
  if (SOCK_SetTimeout(sock, eSOCK_OnWrite, &timeout, 0, 0)
      != eSOCK_ESuccess) {
    ErrPostEx(SEV_ERROR, 0, 0,
              "[SendURLQuery] Cannot setup the connection timeout to "
              "server \"%s\", port %d",
              host_machine, (int)host_port);
    SOCK_Close(sock);
    return FALSE;
  }

  /* Allocate heap buffer */
  buffer = MemNew (URL_QUERY_BUFLEN + 1);
  if (buffer == NULL) {
    ErrPostEx(SEV_ERROR, 0, 0,
              "[SendURLQuery] Cannot allocate buffer");
    SOCK_Close(sock);
    return FALSE;
  }
  MemSet ((Nlm_VoidPtr) buffer, 0, (size_t) (URL_QUERY_BUFLEN + 1));

  /* allow the user to specify a program name, otherwise get from ProgramPath */
  if (! HasNoText (programName)) {
    userAgentName = programName;
  } else {
    Nlm_ProgramPath (path, sizeof (path));
    userAgentName = StringRChr (path, DIRDELIMCHR);
    if (userAgentName != NULL) {
      userAgentName++;
    }
  }
  if (HasNoText (userAgentName)) {
    userAgentName = "?";
  }

  /* Sockets are connected;  now compose and send request */
  ok = FALSE;
  do {{ /* TRY */
    /*
    static char X_GET[] = "GET ";
    */
    static char X_GET[] = "POST ";
    static char X_HTTP[] = " HTTP/1.0\n";
    static char X_QUERY[] = "?";
    static char X_AMPRSND[] = "&";

    needsQmark = TRUE;
    needsAmpsnd = TRUE;
    /* write HTTP header */
    if (SOCK_Write(sock, (const void *)X_GET,  StrLen(X_GET ), 0)
        != eSOCK_ESuccess) {
      ErrPostEx(SEV_ERROR, 0, 0,
                "[SendURLQuery]  Error sending HTTP header");
      break;
    }

    if (SOCK_Write(sock, (const void *)host_path,
                   StrLen(host_path), 0) != eSOCK_ESuccess) {
      ErrPostEx(SEV_ERROR, 0, 0,
                "[SendURLQuery]  Error sending HTTP header");
      break;
    }
    if (StringStr (host_path, X_QUERY) != NULL) {
      needsQmark = FALSE;
    }

    if (! HasNoText (query)) {
      if (needsQmark) {
        if (SOCK_Write(sock, (const void *)X_QUERY,  StrLen(X_QUERY ), 0)
            != eSOCK_ESuccess) {
          ErrPostEx(SEV_ERROR, 0, 0,
                    "[SendURLQuery]  Error sending HTTP header");
          break;
        }
      }
      needsQmark = FALSE;
      if (SOCK_Write(sock, (const void *)query,
                     StrLen(query), 0) != eSOCK_ESuccess) {
        ErrPostEx(SEV_ERROR, 0, 0,
                  "[SendURLQuery]  Error sending HTTP header");
        break;
      }
    }

    if (! HasNoText (arguments)) {
      if (needsQmark) {
        if (SOCK_Write(sock, (const void *)X_QUERY,  StrLen(X_QUERY ), 0)
            != eSOCK_ESuccess) {
          ErrPostEx(SEV_ERROR, 0, 0,
                    "[SendURLQuery]  Error sending HTTP header");
          break;
        }
        needsQmark = FALSE;
        needsAmpsnd = FALSE;
      }
      if (needsAmpsnd) {
        if (SOCK_Write(sock, (const void *)X_AMPRSND,  StrLen(X_AMPRSND ), 0)
            != eSOCK_ESuccess) {
          ErrPostEx(SEV_ERROR, 0, 0,
                    "[SendURLQuery]  Error sending HTTP header");
          break;
        }
      }
      if (SOCK_Write(sock, (const void *)arguments,
                     StrLen(arguments), 0) != eSOCK_ESuccess) {
        ErrPostEx(SEV_ERROR, 0, 0,
                  "[SendURLQuery]  Error sending HTTP header");
        break;
      }
    }

    if (sendBinary) {
      mode = "rb";
    } else {
      mode = "r";
    }
    if (sendBinary) {
      len = FileLength (data_file);
    } else {
      /* get actual length after cr/lf pair conversion to newline on PCs in text mode */
      len = 0;
      fp = FileOpen (data_file, mode);
      if (fp != NULL) {
        while ((ct = FileRead(buffer, 1, URL_QUERY_BUFLEN, fp)) > 0) {
          len += ct;
        }
      }
      FileClose (fp);
    }

    if (SOCK_Write(sock, (const void *)X_HTTP,  StrLen(X_HTTP ), 0)
        != eSOCK_ESuccess  ||
        /*
        sprintf(buffer, "Content-type: application/x-www-form-urlencoded"
                "\nContent-Length: %ld\n\n",
                (long) (len + 1))
        */
        sprintf(buffer, "Content-type: multipart/form-data"
                "\nContent-Disposition: form-data; name=supplemental_input"
                "\nUser-Agent: %s"
                "\nContent-Length: %ld\n\n",
                userAgentName, (long) (len + 1))
        <= 0
          ||
        SOCK_Write(sock, (const void *)buffer,    StrLen(buffer), 0)
        != eSOCK_ESuccess) {
      ErrPostEx(SEV_ERROR, 0, 0,
                "[SendURLQuery]  Error sending HTTP header");
      break;
    }

    /* send the service query */
    if ( data_file ) {
      fp = FileOpen (data_file, mode);
      if (fp == NULL) {
        ErrPostEx(SEV_ERROR, 0, 0,
                  "[SendURLQuery]  Error opening data file");
        break;
      }

      err_code = eSOCK_ESuccess;
      cnt = 0;
      while ((ct = FileRead(buffer, 1, URL_QUERY_BUFLEN, fp)) > 0) {
        err_code = SOCK_Write(sock, (const void *)buffer, ct, &n_written);
        cnt += n_written;
      }
      err_code = SOCK_Write(sock, (const void *)"\0", 1, 0);
      cnt++;

      FileClose(fp);

      if (err_code != eSOCK_ESuccess) {
        ErrPostEx(SEV_ERROR, 0, 0,
                  "[SendURLQuery]  Error sending service query (%s)",
                  SOCK_ErrCodeStr(err_code));
        break;
      }
    }

    ok = TRUE;
  }} while ( !ok );  /* end-of-TRY */

  /* Free heap buffer */
  MemFree (buffer);


  /* check for success and return */
  if ( !ok ) {
    SOCK_Close(sock);
    return FALSE;
  }

  qp = (QueryPtr) MemNew (sizeof (QueryData));
  if (qp == NULL) return FALSE;

  qp->timeoutsec = timeoutsec;
  qp->resultproc = resultproc;
  qp->sock = sock;
  qp->receiveBinary = receiveBinary;
  qp->mydata = mydata;

  /* add to polling queue */
  ValNodeAddPointer (&urlquerylist, 0, (Nlm_VoidPtr) qp);
  return TRUE;
}

#define URL_FORMAT_LEN 128

static Nlm_Boolean PollURLQuery (QueryPtr qp)

{
  Nlm_Char       buffer[URL_QUERY_BUFLEN + 1];
  ESOCK_ErrCode  err_code;
  SOCK           sock;
  FILE           *fp = (FILE*)0;
  size_t         ct;
  STimeout       timeout;
  Nlm_Uint4      n_io_done;
  Nlm_CharPtr    ptr;
  Nlm_Char       ch;
  Nlm_Boolean    inHeader;
  Nlm_Char       path [PATH_MAX];
  Nlm_Boolean    gotIntoLoop = FALSE;
  Nlm_CharPtr    mode;
  Nlm_Char       format [URL_FORMAT_LEN + 1];

  if (qp == NULL) return FALSE;
  sock = qp->sock;

  timeout.sec  = 0;
  timeout.usec =  0;
  err_code = eSOCK_ESuccess;
  if (SOCK_SetTimeout(sock, eSOCK_OnRead, &timeout, 0, 0)
      != eSOCK_ESuccess) {
    ErrPostEx(SEV_ERROR, 0, 0,
              "[PollURLQuery] Cannot setup the connection timeout");
    SOCK_Close(sock);
    return FALSE;
  }

  inHeader = TRUE;
  fp = NULL;
  path [0] = '\0';
  format [0] = '\0';
  err_code = eSOCK_ESuccess;
  while ((err_code = SOCK_Read(sock, buffer, URL_QUERY_BUFLEN, &n_io_done))
         == eSOCK_ESuccess) {
    static char X_GET_3[] = "Content-Type: text/html";
    static char X_GET_4[] = "NCBI-tools-type:";
    gotIntoLoop = TRUE;
    if (fp == NULL) {
      TmpNam (path);
      if (qp->receiveBinary) {
        mode = "wb";
      } else {
        mode = "w";
      }
      fp = FileOpen(path, mode);
      if (fp == NULL) {
        ErrPostEx(SEV_ERROR, 0, 0,
                  "[PollURLQuery]  Error opening FASTA file");
        return FALSE;
      }
      timeout.sec  = qp->timeoutsec;
      timeout.usec =  0;
      if (SOCK_SetTimeout(sock, eSOCK_OnRead, &timeout, 0, 0)
          != eSOCK_ESuccess) {
        ErrPostEx(SEV_ERROR, 0, 0,
                  "[PollURLQuery] Cannot reset the connection timeout");
        SOCK_Close(sock);
        return FALSE;
      }
    }
    if (inHeader) {
      ptr = StringStr(buffer, X_GET_4);
      if (ptr != NULL) {
        ptr += StrLen (X_GET_4);
        ch = *ptr;
        while (ch == ' ') {
          ptr++;
          ch = *ptr;
        }
        ct = 0;
        while (ct < URL_FORMAT_LEN && ch != '\0' && ch != '\n' && ch != '\r') {
          format [ct] = ch;
          ct++;
          ptr++;
          ch = *ptr;
        }
        format [ct] = '\0';
      }
      ptr = StringStr(buffer, X_GET_3);
      if (ptr != NULL) {
        ptr += StrLen (X_GET_3);
        ch = *ptr;
        while (ch != '\0' && ch != '\n' && ch != '\r') {
          ptr++;
          ch = *ptr;
        }
        while (ch == '\n' || ch == '\r') {
          ptr++;
          ch = *ptr;
        }
        ct = FileWrite(ptr, 1, n_io_done - (ptr - buffer), fp);
        ASSERT ( ct == n_io_done - (ptr - buffer) );
        inHeader = FALSE;
      }
    } else {
      ct = FileWrite(buffer, 1, n_io_done, fp);
      ASSERT ( ct == n_io_done );
    }
  }

  if (gotIntoLoop) {
    FileClose(fp);

    if (qp->resultproc != NULL) {
      qp->resultproc (path, format, qp->mydata);
    }
    FileRemove (path);
  }

  /* return (Nlm_Boolean)(err_code != eSOCK_ETimeout); */
  return gotIntoLoop; /* must remove from queue, even if incomplete - but how to warn? */
}

NLM_EXTERN Nlm_Int4 SOCK_CheckURLQuery (void)

{
  Nlm_Int4        count = 0;
  ValNodePtr  next;
  ValNodePtr  PNTR prev;
  QueryPtr    qp;
  ValNodePtr  vnp;

  prev = &urlquerylist;
  vnp = urlquerylist;
  while (vnp != NULL) {
    next = vnp->next;
    qp = (QueryPtr) vnp->data.ptrvalue;
    if (qp != NULL && PollURLQuery (qp)) {
      *(prev) = next;
      vnp->next = NULL;
      ValNodeFreeData (vnp);
      vnp = next;
      /* Beep (); */ /* announce retrieval of URL query results */
    } else {
      prev = &(vnp->next);
      vnp = next;
      count++;
    }
  }
  return count;
}

