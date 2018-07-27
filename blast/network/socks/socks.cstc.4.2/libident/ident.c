/*
** ident.c	High-level calls to the ident lib
**
** Author: Pär Emanuelsson <pell@lysator.liu.se>
*/

#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>

#include "ident.h"

#if defined(AIX_PS2) || defined(ultrix)
extern char *strdup(char *);
#endif

/* Do a complete ident query and return result */

IDENT *ident_lookup
#ifdef __STDC__
  (int fd,
   int timeout)
#else
  (fd, timeout)
	int fd; int timeout;
#endif
{
  struct sockaddr_in localaddr, remoteaddr;
  int len, res;
  ident_t *id;
  struct timeval timout;
  IDENT *ident=0;

  len = sizeof(remoteaddr);
  if (getpeername(fd, (struct sockaddr *)&remoteaddr, &len) < 0)
    return 0;
  
  len = sizeof(localaddr);
  if (getsockname(fd, (struct sockaddr *)&localaddr, &len) < 0)
    return 0;
  
  timout.tv_sec = timeout; timout.tv_usec = 0;

  if (timeout)
    id = id_open(&localaddr.sin_addr, &remoteaddr.sin_addr, &timout);
  else
    id = id_open(&localaddr.sin_addr, &remoteaddr.sin_addr, (struct timeval *)0);

  if (!id) {
    errno = EINVAL;
    return 0;
  }
  
  if (timeout)
    res = id_query(id,
		   ntohs(remoteaddr.sin_port),
		   ntohs(localaddr.sin_port),
		   &timout);
  else
    res = id_query(id,
		   ntohs(remoteaddr.sin_port),
		   ntohs(localaddr.sin_port),
		   (struct timeval *) 0);

  if (res < 0) {
    id_close(id);
    return 0;
  }

  ident = (IDENT *) malloc(sizeof(IDENT));
  if (!ident) {
    id_close(id);
    return 0;
  }

  if (timeout)
    res = id_parse(id, &timout,
		   &ident->lport,
		   &ident->fport,
		   &ident->identifier,
		   &ident->opsys,
		   &ident->charset);
  else
    res = id_parse(id, (struct timeval *) 0,
		   &ident->lport,
		   &ident->fport,
		   &ident->identifier,
		   &ident->opsys,
		   &ident->charset);
      
  if (res != 1) {
    free(ident);
    id_close(id);
    return 0;
  }

  id_close(id);
  return ident;			/* At last! */
}

char *ident_id
#ifdef __STDC__
  (int fd,
   int timeout)
#else
(fd, timeout)
	int fd; int timeout;
#endif
{
  IDENT *ident;
  char *id=0;

  ident = ident_lookup(fd, timeout);
  if (ident && ident->identifier && *ident->identifier)
    id = strdup(ident->identifier);
  ident_free(ident);
  return id;
}

void ident_free
#ifdef __STDC__
  (IDENT *id)
#else
  (id)
	IDENT *id;
#endif
{
  if (!id) return;
  if (id->identifier) free(id->identifier);
  if (id->opsys) free(id->opsys);
  if (id->charset) free(id->charset);
  free(id);
}
