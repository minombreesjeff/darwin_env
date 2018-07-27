/*
** id_open.c                 Establish/initiate a connection to an IDENT server
**
** Author: Peter Eriksson <pen@lysator.liu.se>
** Fixes: Pdr Emanuelsson <pell@lysator.liu.se>
*/


#include <stdio.h>
#include <errno.h>
#include <memory.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/file.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/* SOLARIS */
#if defined(__svr4__) || defined (SOLARIS) || defined(SCO)
#include <fcntl.h>
#endif

/* LINUX */
#ifdef LINUX
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#ifdef FNDLEAY                /*watch out for a ?typo? in fcntl.h !? */
#define FNDELAY FNDLEAY
#endif

/*
 * Structure used for manipulating linger option.
 */
struct  linger {
        int     l_onoff;                /* option on/off */
        int     l_linger;               /* linger time */
};
#endif


#include "ident.h"

extern void *malloc __P((int size));
  

ident_t *id_open
#ifdef __STDC__
  (struct in_addr *laddr, struct in_addr *faddr, struct timeval *timeout)
#else
  (laddr, faddr, timeout)
struct in_addr *laddr;
struct in_addr *faddr;
struct timeval *timeout;
#endif
{
  ident_t *id;
  int res, tmperrno;
  struct sockaddr_in sin_laddr, sin_faddr;
  fd_set rs, ws, es;
#if !defined(SO_DONTLINGER) || defined(SOLARIS)
  struct linger linger;
#endif

#if defined(sgi) || defined (SOLARIS)
    int on = 1;
#endif    

  if ((id = (ident_t *) malloc(sizeof(*id))) == 0)
    return 0;

  if ((id->fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    free(id);
    return 0;
  }

  if (timeout)
  {
    if ((res = fcntl(id->fd, F_GETFL, 0)) < 0)
      goto ERROR;
  
    if (fcntl(id->fd, F_SETFL, res | FNDELAY) < 0)
      goto ERROR;
  }

#if defined(SO_DONTLINGER) && !defined(SOLARIS)
  if (setsockopt(id->fd, SOL_SOCKET, SO_DONTLINGER, 0, 0) < 0)
    goto ERROR;
#else
  linger.l_onoff = 0;
  linger.l_linger = 0;
  
  if (setsockopt(id->fd, SOL_SOCKET, SO_LINGER, &linger, sizeof(linger)) < 0)
      goto ERROR;
#endif
  
#if defined(sgi) || defined(SOLARIS)
    if (setsockopt(id->fd, SOL_SOCKET, SO_REUSEADDR, (char *)&on,
            sizeof(on)) < 0)
#else 
  if (setsockopt(id->fd, SOL_SOCKET, SO_REUSEADDR, 0, 0) < 0)
#endif
    goto ERROR;

  id->buf[0] = '\0';

  memset(&sin_laddr, 0, sizeof(sin_laddr));
  sin_laddr.sin_family = AF_INET;
  sin_laddr.sin_addr = *laddr;
  sin_laddr.sin_port = 0;

  if (bind(id->fd, (struct sockaddr *)&sin_laddr, sizeof(sin_laddr)) < 0)
    goto ERROR;
  
  memset(&sin_faddr, 0, sizeof(sin_faddr));
  sin_faddr.sin_family = AF_INET;
  sin_faddr.sin_addr = *faddr;
  sin_faddr.sin_port = htons(IDPORT);
  
  res = connect(id->fd, (struct sockaddr *)&sin_faddr, sizeof(sin_faddr));
  if (res < 0 && errno != EINPROGRESS)
    goto ERROR;

  if (timeout)
  {
    FD_ZERO(&rs);
    FD_ZERO(&ws);
    FD_ZERO(&es);
    FD_SET(id->fd, &rs);
    FD_SET(id->fd, &ws);
    FD_SET(id->fd, &es);

    if ((res = select(FD_SETSIZE, &rs, &ws, &es, timeout)) < 0)
      goto ERROR;

    if (res == 0)
    {
      errno = ETIMEDOUT;
      goto ERROR;
    }

    if (FD_ISSET(id->fd, &es))
      goto ERROR;

    if (!FD_ISSET(id->fd, &rs) && !FD_ISSET(id->fd, &ws))
      goto ERROR;
  }

  return id;

ERROR:
  tmperrno = errno;		/* Save, so close() won't erase it */
  close(id->fd);
  free(id);
  errno = tmperrno;
  return 0;
}

