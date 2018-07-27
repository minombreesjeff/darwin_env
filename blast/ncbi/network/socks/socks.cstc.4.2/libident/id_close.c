/*
** id_close.c                            Close a connection to an IDENT server
**
** Author: Peter Eriksson <pen@lysator.liu.se>
*/

#include "ident.h"

int id_close
#ifdef __STDC__
  (ident_t *id)
#else
  (id)
ident_t *id;
#endif
{
  int res;
  
  res = close(id->fd);
  free(id);

  return res;
}


