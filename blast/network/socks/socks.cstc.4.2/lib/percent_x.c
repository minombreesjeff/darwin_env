 /*
  * percent_x() takes a string and performs %x subsitutions.
  * It aborts the program when the result of
  * expansion would overflow the output buffer. Because the result of %<char>
  * expansion is typically passed on to a shell process, characters that may
  * confuse the shell are replaced by underscores.
  * 
  * Diagnostics are reported through syslog(3).
  * 
  * Author: Wietse Venema, Eindhoven University of Technology, The Netherlands.
  *
  * Adapted for use with SOCKS by Ying-Da Lee, NEC Systems Lab, CSTC
  * ylee@syl.dl.nec.com
  *
  */

#ifndef lint
static char sccsid[] = "@(#) percent_x.c 1.2 92/08/24 21:46:22";
#endif

/* System libraries. */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <syslog.h>
#include "socks.h"

extern char *strncpy();
extern char *strchr();
extern void exit();

extern char socks_src_name[], socks_src_user[];
extern char socks_real_user[];
extern char socks_dst_name[], socks_dst_serv[];
extern char socks_cmd[];

/* percent_x - do %<char> expansion, abort if result buffer is too small */

void    percent_x(result, result_len, str, src, dst, pid)
char   *result;
int     result_len;
char   *str;
struct sockaddr_in	*src, *dst;
int     pid;
{
    char   *end = result + result_len - 1;	/* end of result buffer */
    char   *expansion;
    int     expansion_len;
    char    pid_buf[10];
    char    port_buf[10];
    static char ok_chars[] = "1234567890!@%-_=+\\:,./\
abcdefghijklmnopqrstuvwxyz\
ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char   *cp;

    /*
     * %A: the client domainname if known, IP address otherwise
     * %a: the client IP address
     * %c: "connect" or "bind"
     * %p: the daemon or client program process id
     * %S: the service name (ftp, telnet,etc.) if known, port number otherwise
     * %s: the destination port number 
     * %U: for sockd, this is the username as reported by identd;
     *	   for client program, this is the name used at login
     * %u: for sockd, this is the username as reported by the client program;
     *	   for client program, this is the username of the effective userid
     * %Z: the destination domainname if known, IP address otherwise
     * %z: the destination IP address
     *
     * %% becomes a %, and %other is ignored. We terminate with a diagnostic if
     * we would overflow the result buffer. Characters that may confuse the
     * shell are mapped to underscores.
     */

    while (*str) {
	if (*str == '%') {
	    str++;
	    expansion =
		*str == 'A' ? (str++, socks_src_name) :
		*str == 'a' ? (str++, inet_ntoa(src->sin_addr)) :
		*str == 'c' ? (str++, socks_cmd) :
		*str == 'p' ? (str++, sprintf(pid_buf, "%d", pid), pid_buf) :
		*str == 'S' ? (str++, socks_dst_serv) :
		*str == 's' ? (str++, sprintf(port_buf, "%u", ntohs(dst->sin_port)), port_buf) :
		*str == 'U' ? (str++, socks_real_user) :
		*str == 'u' ? (str++, socks_src_user) :
		*str == 'Z' ? (str++, socks_dst_name) :
		*str == 'z' ? (str++, inet_ntoa(dst->sin_addr)) :
		*str == '%' ? (str++, "%") :
		*str == 0 ? "" : (str++, "");
	    expansion_len = strlen(expansion);
	    for (cp = expansion; *cp; cp++)
		if (strchr(ok_chars, *cp) == 0)
		    *cp = '_';
	} else {
	    expansion = str++;
	    expansion_len = 1;
	}
	if (result + expansion_len >= end) {
	    syslog(LOG_HIGH, "shell command too long: %30s...", result);
	    exit(0);
	}
	strncpy(result, expansion, expansion_len);
	result += expansion_len;
    }
    *result = 0;
}
