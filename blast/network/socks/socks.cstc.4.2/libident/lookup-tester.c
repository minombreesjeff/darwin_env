/*
** lookup-tester.c	Tests the high-level ident calls.
**
** Author: Pär Emanuelsson <pell@lysator.liu.se>, 28 March 1993
*/

#include <stdio.h>
#include <sys/types.h>
#include <errno.h>

#include "ident.h"

main(argc,argv)
  int argc;
  char *argv[];
{
  IDENT *ident;
  char *user;

  chdir("/tmp");

  puts("Welcome to the other IDENT server tester, version 1.0\r\n\r");  

  puts("Testing ident_lookup...\r\n\r");
  fflush(stdout);

  ident = ident_lookup(fileno(stdin), 30);

  if (!ident)
    perror("ident");
  else {
    printf("IDENT response is:\r\n");
    printf("   Lport........ %d\r\n", ident->lport);
    printf("   Fport........ %d\r\n", ident->fport);
    printf("   Opsys........ %s\r\n", ident->opsys);
    printf("   Charset...... %s\r\n",
	   ident->charset ? ident->charset : "<not specified>");
    printf("   Identifier... %s\r\n", ident->identifier);
  }

  ident_free(ident);

  puts("\r\nTesting ident_id...\r\n\r");
  fflush(stdout);

  user = ident_id(fileno(stdin), 30);

  if (user)
    printf("IDENT response is identifier = %s\r\n", user);
  else
    puts("IDENT lookup failed!\r");

  fflush(stdout);
  sleep(1);
  exit(0);
}

