#include "socks.h"

#include <netdb.h>
/* >>> K. Shackelford */
#if defined(hpux) || defined(AIX)
#include <sys/types.h>
#include <netinet/in.h>
#endif
/* <<< K. Shackelford */ 
#include <stdio.h>
#include <ctype.h>
/* >>> YDL, 93/10/26 */
#if (defined(sun) && !defined(SOLARIS)) || defined(sgi)
#include <strings.h>
#else
#include <string.h>
#endif

#ifdef SOLARIS
#include "bstring.h"
#endif

/* <<< YDL< 93/10/26 */
extern char	*porttoserv();
extern char	*saddrtoname();

/*
 * These functions are used by both Validate (for sockd)
 * and check_cconf (for clients).
 */

/*
**  Simple 'mkargs' doesn't handle \, ", or '.
*/
void mkargs(cp, argc, argv, max)
char	*cp;
int	*argc;
char	*argv[];
int	max;
{
	*argc = 0;
	while (isspace(*cp))
		cp++;

	while (*cp != '\0') {
		argv[(*argc)++] = cp;
		if (*argc >= max)
			return;

		while (!isspace(*cp) && (*cp != '\0'))
			cp++;
		while (isspace(*cp))
			*cp++ = '\0';
	}
}

/* 
**  Get address, either numeric or dotted quad, or hex.
 *  Result is in network byte order.
*/
int GetAddr(name, addr)
char		*name;
u_int32	*addr;
{
	struct hostent	*hp;
	struct netent	*np;

	if ((hp = gethostbyname(name)) != NULL) {
		bcopy(hp->h_addr_list[0], addr, sizeof(*addr));
		return *addr;
	}
	if ((np = getnetbyname(name)) != NULL) {
		bcopy(&np->n_net, addr, sizeof(*addr));
		return *addr;
	}
	return *addr = inet_addr(name);
}

long GetPort(name)
char		*name;
/* result is in HOST byte order */
{
	struct servent	*sp;

	if ((sp = getservbyname(name, "tcp")) != NULL) {
		return ntohs(sp->s_port);
	}
	if (!isdigit(*name))
		return -1;
	return atol(name);
}


int check_user(userlist, src_user)

char	*userlist, *src_user;

/*
 * Unless userlist is a null pointer, in which case all users are
 * allowed (return 1), otherwise
 * userlist is a nonempty string containing userids separated by
 * commas, no other separators are allowed in the string.
 * 94/03/02: if userlist starts with '/', it specifies a file
 *	containing userids.
 *
 * Return 1 if src_user is in the userlist; 
 * return 0 if not, or if userfile cannot be open.
 */
{
	char	*p, *q;

	if (!(p = userlist)) {
		return 1;
	}
	do {
		if (q = index(p, ','))
			*q++ = '\0';
		if (*p == '/') {
			switch (check_userfile(p, src_user)) {
			case 1:
				return 1;
			case -1:
				return 0;
			default:
				;
			}
		} else if (strcmp(p, src_user) == 0) {
			return 1;
		}
	} while ( p = q);

	return 0;
}

#include <string.h>
#include <syslog.h>

int check_userfile(userfile, src_user)
char	*userfile, *src_user;
/* return 1 if match, 0 otherwise */
/* return -1 if cannot open file */
{
	FILE	*fd;
#define BUFLEN 1024
	static char buf[BUFLEN];
	char *bp;

	if ((fd = fopen(userfile, "r")) == NULL) {
/*
#ifdef MONITOR
		sendto(mon_sd, &mon_msg, socks_type1_len, 0, mon_un_addr, mon_un_len);
		failure_mon(MON_ERR_NO_USERFILE);
#endif
*/
		syslog(LOG_HIGH,"Unable to open userfile (%s)\n", userfile);
		return (-1);
	}

	while (fgets(buf, BUFLEN, fd) != NULL) {
		if ((bp = index(buf, '\n')) != NULL)
			*bp = '\0';
		if (( bp = index(buf, '#')) != NULL)
			*bp = '\0';

		for (bp = strtok(buf, " ,\t"); bp != NULL;
		     bp = strtok(NULL, " ,\t")) {
			if (strcmp(bp, src_user) == 0) {
				fclose(fd);
				return 1;
			}
		}
	}
	fclose(fd);
	return 0;
}
