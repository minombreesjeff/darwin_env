/*
** id_parse.c                    Receive and parse a reply from an IDENT server
**
** Author: Peter Eriksson <pen@lysator.liu.se>
** Fiddling: Pär Emanuelsson <pell@lysator.liu.se>
*/

#include <stdio.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>

#include "ident.h"

/* >>> Ian Dunkin  */
#if defined(ultrix) || defined(AIX_PS2)
char *strdup(s)
char *s;
{
        char *malloc();
        static char *p;

        if ((p = malloc(strlen(s) + 1)) != NULL)
                strcpy(p, s);
        return(p);
}
#endif
/* <<< Ian Dunkin */

static char *xstrtok
#ifdef __STDC__
  (char *cp, char *cs, char *dc)
#else
  (cp, cs, dc)
char *cp;
char *cs;
char *dc;
#endif
{
  static char *bp = 0;

  if (cp)
    bp = cp;

  /*
  ** No delimitor cs - return whole buffer and point at end
  */
  if (!cs)
  {
    while (*bp)
      bp++;
    return cs;
  }
  
  /*
  ** Skip leading spaces
  */
  while (isspace(*bp))
    bp++;

  /*
  ** No token found?
  */
  if (!*bp)
    return 0;

  cp = bp;
  while (*bp && !index(cs, *bp))
    bp++;

  /*
  ** Remove trailing spaces
  */
  *dc = *bp;
  for (dc = bp-1; dc > cp && isspace(*dc); dc--)
    ;
  *++dc = '\0';
  
  bp++;

  return cp;
}


int id_parse
#ifdef __STDC__
  (ident_t *id,
   struct timeval *timeout,
   int *lport,
   int *fport,
   char **identifier,
   char **opsys,
   char **charset)
#else
  (id, timeout, lport, fport, identifier, opsys, charset)
ident_t *id;
struct timeval *timeout;
int *lport;
int *fport;
char **identifier;
char **opsys;
char **charset;
#endif
{
  char c, *cp, *tmp_charset;
  fd_set rs;
  int pos, res, lp, fp;

  errno = 0;
  
  tmp_charset = 0;

  if (!id) return -1;
  if (lport) *lport = 0;
  if (fport) *fport = 0;
  if (identifier) *identifier = 0;
  if (opsys) *opsys = 0;
  if (charset) *charset = 0;
  
  pos = strlen(id->buf);

  if (timeout)
  {
    FD_ZERO(&rs);
    FD_SET(id->fd, &rs);

    if ((res = select(FD_SETSIZE, &rs, (fd_set *)0, (fd_set *)0, timeout)) < 0)
      return -1;

    if (res == 0)
    {
      errno = ETIMEDOUT;
      return -1;
    }
  }

  while (pos < sizeof(id->buf) &&
	 (res = read(id->fd, id->buf + pos, 1)) == 1 &&
	 id->buf[pos] != '\n')
    pos++;

  if (res < 0)
    return -1;

  if (res == 0)
  {
    errno = ENOTCONN;
    return -1;
  }
  
  if (id->buf[pos] != '\n')
    return 0;

  id->buf[pos++] = '\0';

  /*
  ** Get first field (<lport> , <fport>)
  */
  cp = xstrtok(id->buf, ":", &c);
  if (!cp)
    return -2;

  if (sscanf(cp, " %d , %d", &lp, &fp) != 2)
  {
    if (identifier) *identifier = strdup(cp);
    return -2;
  }

  if (lport) *lport = lp;
  if (fport) *fport = fp;

  /*
  ** Get second field (USERID or ERROR)
  */
  cp = xstrtok((char *)0, ":", &c);
  if (!cp)
    return -2;

  if (strcmp(cp, "ERROR") == 0)
  {
    cp = xstrtok((char *)0, "\n\r", &c);
    if (!cp)
      return -2;

    if (identifier) *identifier = strdup(cp);
    
    return 2;
  }
  else if (strcmp(cp, "USERID") == 0)
  {
    /*
    ** Get first subfield of third field <opsys>
    */
    cp = xstrtok((char *)0, ",:", &c);
    if (!cp)
      return -2;

    if (opsys) *opsys = strdup(cp);

    /*
    ** We have a second subfield (<charset>)
    */
    if (c == ',')
    {
      cp = xstrtok((char *)0, ":", &c);
      if (!cp)
	return -2;

      tmp_charset = cp;
      if (charset) *charset = strdup(cp);

      /*
      ** We have even more subfields - ignore them
      */
      if (c == ',')
	xstrtok((char *)0, ":", &c);
    }

    if (tmp_charset && strcmp(tmp_charset, "OCTET") == 0)
      cp = xstrtok((char *)0, (char *)0, &c);
    else
      cp = xstrtok((char *)0, "\n\r", &c);

    if (identifier) *identifier = strdup(cp);
    return 1;
  }
  else
  {
    if (identifier) *identifier = strdup(cp);
    return -3;
  }
}
