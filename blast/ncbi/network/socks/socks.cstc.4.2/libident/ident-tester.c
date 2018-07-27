/*
** ident-tester.c           A small daemon that can be used to test Ident
**                          servers
**
** Author: Peter Eriksson <pen@lysator.liu.se>, 10 Aug 1992
*/

#include <stdio.h>
#include <netdb.h>
#include <syslog.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "ident.h"

/*
** Return the name of the connecting host, or the IP number as a string.
*/
char *gethost(addr)
  struct in_addr *addr;
{
  struct hostent *hp;

  
  hp = gethostbyaddr(addr, sizeof(struct in_addr), AF_INET);
  if (hp)
    return hp->h_name;
  else
    return inet_ntoa(*addr);
}


main(argc,argv)
  int argc;
  char *argv[];
{
  struct sockaddr_in laddr, faddr;
  int len, res, lport, fport;
  ident_t *id;
  char *identifier, *opsys, *charset;
  

  puts("Welcome to the IDENT server tester, version 1.7\r\n\r");
  fflush(stdout);
  
  len = sizeof(faddr);
  getpeername(0, &faddr, &len);

  len = sizeof(laddr);
  getsockname(0, &laddr, &len);

  printf("Connecting to Ident server at %s...\r\n", inet_ntoa(faddr.sin_addr));
  fflush(stdout);

#ifdef LOG_LOCAL3
  openlog("tidentd", 0, LOG_LOCAL3);
#else
  openlog("tidentd", 0);
#endif
  
  id = id_open(&laddr.sin_addr, &faddr.sin_addr, NULL);
  if (!id)
  {
    perror("id_open()");
    fflush(stderr);
    syslog(LOG_ERR, "Error: id_open(): host=%s, error=%m",
	   gethost(&faddr.sin_addr));
    exit(1);
  }

  printf("Querying for lport %d, fport %d....\r\n",
	 (int) ntohs(faddr.sin_port),
	 (int) ntohs(laddr.sin_port));
  fflush(stdout);
  
  if (id_query(id, ntohs(faddr.sin_port), ntohs(laddr.sin_port), 0) < 0)
  {
    perror("id_query()");
    fflush(stderr);
    syslog(LOG_ERR, "Error: id_query(): host=%s, error=%m",
	   gethost(&faddr.sin_addr));
    exit(1);
  }

  printf("Reading response data...\r\n");
  fflush(stdout);

  res = id_parse(id, NULL,
		   &lport, &fport,
		   &identifier,
		   &opsys,
		   &charset);

  switch (res)
  {
    default:
      perror("id_parse()");
      syslog(LOG_ERR, "Error: id_parse(): host=%s, error=%m",
	     gethost(&faddr.sin_addr));
      break;

    case -2:
      syslog(LOG_ERR, "Error: id_parse(): host=%s, Parse Error: %s",
	     gethost(&faddr.sin_addr),
	     identifier ? identifier : "<no information available>");

      if (identifier)
	printf("Parse error on reply:\n  \"%s\"\n", identifier);
      else
	printf("Unidentifiable parse error on reply.\n");
      break;

    case -3:
      syslog(LOG_ERR, "Error: id_parse(): host=%s, Illegal reply type: %s",
	     gethost(&faddr.sin_addr),
	     identifier);

      printf("Parse error in reply: Illegal reply type: %s\n", identifier);
      break;
	     
    case 0:
      syslog(LOG_ERR, "Error: id_parse(): host=%s, NotReady",
	     gethost(&faddr.sin_addr));
      puts("Not ready. This should not happen...\r");
      break;

    case 2:
      syslog(LOG_INFO, "Reply: Error: host=%s, error=%s",
	     gethost(&faddr.sin_addr), identifier);
      
      printf("Error response is:\r\n");
      printf("   Lport........ %d\r\n", lport);
      printf("   Fport........ %d\r\n", fport);
      printf("   Error........ %s\r\n", identifier);
      break;

    case 1:
      if (charset)
	syslog(LOG_INFO,
	       "Reply: Userid: host=%s, opsys=%s, charset=%s, userid=%s",
	       gethost(&faddr.sin_addr), opsys, charset, identifier);
      else
	syslog(LOG_INFO, "Reply: Userid: host=%s, opsys=%s, userid=%s",
	       gethost(&faddr.sin_addr), opsys, identifier);
      
      printf("Userid response is:\r\n");
      printf("   Lport........ %d\r\n", lport);
      printf("   Fport........ %d\r\n", fport);
      printf("   Opsys........ %s\r\n", opsys);
      printf("   Charset...... %s\r\n", charset ? charset : "<not specified>");
      printf("   Identifier... %s\r\n", identifier);

      if (id_query(id, ntohs(faddr.sin_port), ntohs(laddr.sin_port), 0) >= 0)
      {
	if (id_parse(id, NULL,
		     &lport, &fport,
		     &identifier,
		     &opsys,
		     &charset) == 1)
	  printf("   Multiquery... Enabled\r\n");
      }
  }

  fflush(stdout);
  sleep(1);
  exit(0);
}

