#include <sys/types.h>
#include <syslog.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <ctype.h>
/* >>> YDL, 93/10/26 */
#if (defined(sun) && !defined(SOLARIS)) || defined(sgi)
#include <strings.h>
#else
#include <string.h>
#endif
/* <<< YDL< 93/10/26 */
#include "socks.h"

#define STREQ(a, b)	(strcmp(a, b) == 0)

#ifdef OLD_CONF_MASK
static char conf_mask[] = OLD_CONF_MASK;
#endif
char *socks_conf = SOCKS_CONF;
extern char	*porttoserv();
extern char	*saddrtoname();
extern	void	mkargs();
extern	int	GetAddr();
extern	long	GetPort();
extern	int	check_user();
extern	char	socks_src_user[];
extern char	*socks_server;
extern struct sockaddr_in	socks_nsin;
extern u_int32 SocksHost;
extern char *socks_def_server;
extern char *socks_serverlist;

check_cconf(src, dst)
/* Return 0 if sockd should be used,
	  1 if direct connection should be made,
	 -1 if the connection request should be denied.
 */
struct sockaddr_in	*src, *dst;
{
	FILE		*fd;
	static char	buf[1024];
#ifdef TEST
	char		temp[1024];
#endif
	char		*bp;
	int		linenum = 0, direct;
	char		*argv[10];
	int		argc;
	u_int32	daddr, dmask;
	unsigned short	dport;
	enum 		{ e_lt, e_gt, e_eq, e_neq, e_le, e_ge, e_nil } tst;
	char		*userlist;
	int		next_arg;
	unsigned short	dst_sin_port = ntohs(dst->sin_port); 
	long	p;
	char	*cmdp;
	struct hostent	*hp;
	struct	in_addr self;


	self.s_addr = inet_addr("127.0.0.1");
	if ((dst->sin_addr.s_addr == self.s_addr) || (dst->sin_addr.s_addr == 0))
		return(1);
	if ((fd = fopen(socks_conf, "r")) == NULL)
		return(1);

	while (fgets(buf, sizeof(buf) - 1, fd) != NULL) {
		linenum++;
#ifdef TEST
		strcpy(temp, buf);
#endif
		/*
		**  Comments start with a '#' anywhere on the line
		*/
		cmdp = (char *)0;
		if ((bp = index(buf, '\n')) != NULL)
			*bp = '\0';
		for (bp = buf; *bp != '\0'; bp++) {
			if (*bp == ':') {
				*bp++ = '\0';
				cmdp = bp;
				break;
			} else if (*bp == '#') {
				*bp = '\0';
				break;
			} else if (*bp == '\t')
				*bp = ' ';
		}

		mkargs(buf, &argc, argv, 7);
		if (argc == 0)
			continue;
		if ((argc < 3) || (argc > 7)) {
#ifdef TEST
			printf("Invalid entry at line %d\n", linenum);
#else
			syslog(LOG_LOW, "Invalid entry at line %d", linenum);
#endif
			continue;
		}

		next_arg = 1;
		if (STREQ(argv[0], "sockd")) {
			socks_serverlist = socks_def_server;
			direct = 0;
			if (strncmp(argv[next_arg], "@=", 2) == 0) {
				socks_serverlist = argv[next_arg] + 2;
				if(*socks_serverlist == '\0')
/*
					socks_serverlist = (char *)0;
*/
					socks_serverlist = socks_def_server;
				next_arg++;
			}
		} else if (strncmp(argv[0], "sockd@", 6) == 0) {
			direct = 0;
			socks_serverlist = argv[0] + 6;
			if (*socks_serverlist == '\0')
/*
				socks_serverlist = (char *)0;
*/
				socks_serverlist = socks_def_server;
		} else if (STREQ(argv[0], "direct")) {
			direct = 1;
		} else if (STREQ(argv[0], "deny")) {
			direct = -1;
		} else {
#ifdef TEST
			printf("Invalid sockd/direct/deny field at line %d\n", linenum);
#else
			syslog(LOG_LOW, "Invalid sockd/direct/deny field at line %d", linenum);
#endif
			continue;
		}

		userlist = (char *)0;
		if (strncmp(argv[next_arg], "*=", 2) == 0) {
			if (argv[next_arg][2]) userlist = argv[next_arg] + 2;
			next_arg++;
		}
		if(argc <= next_arg+1) {
#ifdef TEST
			printf("Invalid entry at line %d\n", linenum);
#else
			syslog(LOG_LOW, "Invalid entry at line %d", linenum);
#endif
			continue;
		}
		GetAddr(argv[next_arg++], &daddr);
		GetAddr(argv[next_arg++], &dmask);
		if (argc > next_arg + 1) {
			if (STREQ(argv[next_arg], "eq"))
				tst = e_eq;
			else if (STREQ(argv[next_arg], "neq"))
				tst = e_neq;
			else if (STREQ(argv[next_arg], "lt"))
				tst = e_lt;
			else if (STREQ(argv[next_arg], "gt"))
				tst = e_gt;
			else if (STREQ(argv[next_arg], "le"))
				tst = e_le;
			else if (STREQ(argv[next_arg], "ge"))
				tst = e_ge;
			else {
#ifdef TEST
				printf("Invalid comparison at line %d\n", linenum);
#else
				syslog(LOG_LOW, "Invalid comparison at line %d", linenum);
#endif
				continue;
			}
				
			if (((p = GetPort(argv[next_arg+1])) < 0) ||
				(p >= (1L << 16))) {
#ifdef TEST
				printf("Invalid port number at line %d\n", linenum);
#else
				syslog(LOG_LOW, "Invalid port number at line %d", linenum);
#endif
				continue;
			} else {
				dport = p;
			}
		} else {
			tst = e_nil;
			dport = 0;
		}

#ifdef DEBUG
		{
			char msg[1024];
			if (userlist) 
				sprintf(msg,"%s %s 0x%08x 0x%08x %s %u",
					argv[0], userlist, daddr, dmask,
						tst == e_eq ? "==" :
						tst == e_neq ? "!=" :
						tst == e_lt ? "<" :
						tst == e_gt ? ">" :
						tst == e_le ? "<=" :
						tst == e_ge ? ">=" : "NIL",
						dport);
			else
				sprintf(msg,"%s 0x%08x 0x%08x %s %u",
					argv[0], daddr, dmask,
						tst == e_eq ? "==" :
						tst == e_neq ? "!=" :
						tst == e_lt ? "<" :
						tst == e_gt ? ">" :
						tst == e_le ? "<=" :
						tst == e_ge ? ">=" : "NIL",
						dport);
			syslog(LOG_LOW, "%s", msg);
		}
#endif
	/* comparisons of port numbers must be done in host order */

#ifdef OLD_CONF_MASK
		if((daddr & ~dmask) == (dst->sin_addr.s_addr & ~dmask) &&
#else
		if((daddr & dmask) == (dst->sin_addr.s_addr & dmask) &&
#endif
		    check_user(userlist, socks_src_user)) {
			if (tst == e_nil) 
				goto GotIt;
			if ((tst == e_eq) && (dst_sin_port == dport))
				goto GotIt;
			if ((tst == e_neq) && (dst_sin_port != dport))
				goto GotIt;
			if ((tst == e_lt) && (dst_sin_port < dport))
				goto GotIt;
			if ((tst == e_gt) && (dst_sin_port > dport))
				goto GotIt;
			if ((tst == e_le) && (dst_sin_port <= dport))
				goto GotIt;
			if ((tst == e_ge) && (dst_sin_port >= dport))
				goto GotIt;
		}
	}

	fclose(fd);
#ifdef TEST
	printf("*** No match with any line. Access denied.\n");
#endif
	return -1;

GotIt:
#ifdef TEST
	printf("Line %d: %s", linenum, temp);
#endif
	fclose(fd);
	if (cmdp != (char *)0)
		shell_cmd(cmdp, src, dst);
	return direct;
	
}

