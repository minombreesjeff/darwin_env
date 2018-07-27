#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/signal.h>
#include <syslog.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <netdb.h>
#include <stdio.h>
#include <ctype.h>
#if (defined(sun) && !defined(SOLARIS)) || defined(sgi)
#include <strings.h>
#else
#include <string.h>
#endif
#if defined(SUPPORT_RCMD)
#include <fcntl.h>
#endif
#include "socks.h"

#include "ident.h"
#define IDENTD_TIMEOUT 15 /* 15 seconds */
static int	use_identd = 0;

#define STREQ(a, b)	(strcmp(a, b) == 0)

char socks_cmd[] = "connect";

#ifdef OLD_CONF_MASK
static char conf_mask[] = OLD_CONF_MASK;
#endif
static char *sockd_conf = SOCKD_CONF;
static char server_version[] = "4.1";
static unsigned short socks_port;
static unsigned short socks_client_port;
extern char	*porttoserv();
extern char	*saddrtoname();
extern	void	mkargs();
extern	int	GetAddr();
extern	long	GetPort();
extern	int	check_user();
#define NAMELEN	128
char	socks_src_name[NAMELEN], socks_src_user[NAMELEN];
char	socks_real_user[NAMELEN];
char	socks_dst_name[NAMELEN], socks_dst_serv[NAMELEN];

static char log_msg[1024];

#ifdef DEBUG
static char buf[1024];
#endif

#ifndef TEST

	static u_int32	from_in = 0L, from_out = 0L;

/*
**  Current version for response messages
*/
int		Version = 0;

void die()
{
	syslog(LOG_LOW, "timed-out -- %s", log_msg);
	exit(1);
}


#ifdef FOR_PS
main(argc, argv, envp)
int	argc;
char	*argv[];
char	*envp[];
#else /* FOR_PS not defined */
main(argc, argv)
int	argc;
char	*argv[];
#endif /* #ifdef FOR_PS */
{
	char			c;
	int			inp, in, out, nindex=0;
	int			i, n, len = sizeof(struct sockaddr_in);
	struct sockaddr_in	sin, from, dstsin;
	int			fromlen = sizeof(struct sockaddr_in);
	Socks_t			dst;
	int			one = 1;
	struct servent	*sp;
	int	permit;

#if defined(FOR_PS) && !defined(SYSV)
#define MAXUSERENVIRON 100
	char *UserEnviron[MAXUSERENVIRON+1];    /* saved user environment */
	extern char **environ;
	char ps_buf[1024];
	char **Argv = NULL;   /* pointer to argument vector */
	char *LastArgv = NULL;        /* end of argv */
#define newstr(s)    strcpy(malloc(strlen(s) + 1), s)
  
	for (i = 0; i < MAXUSERENVIRON && envp[i] != NULL; i++)
		UserEnviron[i] = newstr(envp[i]);
	UserEnviron[i] = NULL;
	environ = UserEnviron;

	/*
	**  Save start and extent of argv for setproctitle.
	*/

	Argv = argv;
	if (i > 0)
		LastArgv = envp[i - 1] + strlen(envp[i - 1]);
	else
		LastArgv = argv[argc - 1] + strlen(argv[argc - 1]);
#endif /* FOR_PS && !SYSV */

	socks_port = htons(SOCKS_DEF_PORT);
	bzero((char *)&sin, sizeof(sin));
	bzero((char *)&from, sizeof(from));
	bzero((char *)&dstsin, sizeof(dstsin));

	if (argc >= 2) {
		if (strcmp(argv[1],"-ver") == 0) {
#ifdef MULTIHOMED_SERVER
		printf("\tCSTC multi-homed SOCKS proxy server version %s.\n", CSTC_RELEASE);
#else /* MULTIHOMED_SERVER not defined */
		printf("\tCSTC single-homed SOCKS proxy server version %s.\n", CSTC_RELEASE);
#endif /* #ifdef MULTIHOMED_SERVER */

#if defined(SUPPORT_RCMD)
		printf("\tSupports clients that use Rrcmd().\n");
#else /* SUPPORT_RCMD not defined */
		printf("\tDoes not support clients that use Rrcmd().\n");
#endif /* #if defined(SUPPORT_RCMD) */
		exit(1);
		} else if (strcmp(argv[1], "-i") == 0)
			use_identd = 1;
		else if (strcmp(argv[1], "-I") == 0)
			use_identd = 2; /* strict use of identd */
		else ;
	}
	strcpy(socks_real_user,"unknown");

	if ((sp = getservbyname("socks", "tcp")) != NULL)
		socks_port = sp->s_port;

#ifndef LOG_DAEMON
	(void) openlog("sockd", LOG_PID);
#else
	(void) openlog("sockd", LOG_PID, SYSLOG_FAC);
#endif

#ifdef NOT_THROUGH_INETD
	inp  = socket(AF_INET, SOCK_STREAM, 0);
	sin.sin_family = AF_INET;
	sin.sin_port = socks_port;
	sin.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(inp, &sin, sizeof(sin)) < 0) {
		syslog(LOG_LOW, "error -- main bind() %m");
		exit(1);
	}

	if (listen(inp, MAX_CLIENTS) < 0) {
		syslog(LOG_LOW, "error -- main listen() %m");
		exit(1);
	}

	if ((in = accept(inp, &sin, &len)) < 0) {
		syslog(LOG_LOW, "error -- main accept() %m");
		exit(1);
	}
#else
	in = dup(0);
#endif

	if (getpeername(in, (struct sockaddr *)&from, &fromlen) < 0) {
		syslog(LOG_LOW, "error -- unable to get client address.");
		exit(1);
	}
	socks_client_port = ntohs(from.sin_port);
#ifdef DEBUG
	syslog(LOG_LOW, "socks_client_port=%u", socks_client_port);
#endif /* #ifdef DEBUG */

	saddrtoname(&from.sin_addr, socks_src_name, sizeof(socks_src_name));

	if (GetDst(in, &dst) < 0) {
		syslog(LOG_LOW, "Error in GetDst: %m; from host %s", socks_src_name);
		exit(1);
	}

	if (dst.version != SOCKS_VERSION) {
		syslog(LOG_LOW, "error -- wrong version (0x%2x) from host %s.",
			dst.version, socks_src_name);
		exit(1);
	}

	if (dst.cmd != SOCKS_CONNECT && dst.cmd != SOCKS_BIND) {
		syslog(LOG_LOW, "error -- undefined command (0x%2x) from host %s",
			dst.cmd, socks_src_name);
		exit(1);
	}

	dstsin.sin_family      = AF_INET;
	dstsin.sin_addr.s_addr = dst.host;
	dstsin.sin_port        = dst.port;

	while (read(in, &c, 1) == 1)
		if (c == '\0')
			break;
		else {
			if (nindex < sizeof(socks_src_user) - 1)
			socks_src_user[nindex++] = c;
		}
	socks_src_user[nindex] = '\0';
	if (dstsin.sin_addr.s_addr == 0)
		strcpy(socks_dst_name, "Unspecified.Host");
	else
		saddrtoname(&dstsin.sin_addr, socks_dst_name, sizeof(socks_dst_name));
	porttoserv(dstsin.sin_port, socks_dst_serv, sizeof(socks_dst_serv));

	permit = Validate(&from, &dstsin, in, use_identd);
	if (dst.cmd == SOCKS_CONNECT) {
		strcpy(socks_cmd, "connect");
		sprintf(log_msg, "Connect from %s(%s)@%s to %s (%s)",
			socks_src_user, socks_real_user, socks_src_name, socks_dst_name, socks_dst_serv);
#ifdef FOR_PS
		sprintf(ps_buf, "%s: %s(c) to %s",
			socks_src_user, socks_dst_serv, socks_dst_name);
		setproctitle(ps_buf, Argv, LastArgv);
#endif /* #ifdef FOR_PS */

	} else {
		strcpy(socks_cmd, "bind");
		sprintf(log_msg, "Bind from %s(%s)@%s for %s",
			socks_src_user, socks_real_user, socks_src_name, socks_dst_name, socks_dst_serv);
#ifdef FOR_PS
		sprintf(ps_buf, "%s: %s(b) to %s",
			socks_src_user, socks_dst_serv, socks_dst_name);
		setproctitle(ps_buf, Argv, LastArgv);
#endif /* #ifdef FOR_PS */
	}
	if (permit == 1) 
		;
	else if (permit == 0) {
		syslog(LOG_LOW, "refused -- %s", log_msg);
		exit(1);
	} else if (permit == -1) {
		syslog(LOG_LOW, "cannot connect to identd on %s", socks_src_name);
	} else if (permit == -2) {
		syslog(LOG_LOW, "refused -- %s", log_msg);
		syslog(LOG_LOW, "cannot connect to identd on %s", socks_src_name);
		dst.cmd = SOCKS_NO_IDENTD;
		SendDst(in, &dst);
		exit(1);
	} else if (permit == -3) {
		syslog(LOG_LOW, "refused -- %s", log_msg);
		syslog(LOG_LOW, "*Alert*: real user is %s, not %s", socks_real_user, socks_src_user);
		dst.cmd = SOCKS_BAD_ID;
		SendDst(in, &dst);
		exit(1);
	} else {
		syslog(LOG_HIGH, "refused -- %s", log_msg);
		syslog(LOG_HIGH, "Unexpected result from Validate");
		exit(1);
	}


#ifdef DEBUG
	strcpy(buf, inet_ntoa(from.sin_addr));
	syslog(LOG_LOW,"USER:%s,  SRC:%s,  DST:%s, PORT:%u",
		socks_src_user, buf, inet_ntoa(dstsin.sin_addr),
		ntohs(dstsin.sin_port));
#endif

	/*
	**  Kill a connecting off if bind or connect takes too
	**    long to complete
	*/
	signal(SIGALRM, die);
	/*alarm(60*5);*/		/* 5 minutes */
	alarm(60*2);

	if (dst.cmd == SOCKS_CONNECT) {
		DoConnect(in, &dst);
	}
	if (dst.cmd == SOCKS_BIND) {
		DoNewBind(in, &dst);
	}
}

socks_fail(str, in, ndst)
char	*str;
int	in;
Socks_t	*ndst;
{
	syslog(LOG_LOW, "failed -- %s.  Error code: %s %m", log_msg, str);
	ndst->cmd = SOCKS_FAIL;
	SendDst(in, ndst);
	exit(1);
}


/*
** Actually connect a socket to the outside world,
*/
DoConnect(in, dst)
int	in;
Socks_t	*dst;
{
	int			out;
	struct sockaddr_in	sin;
	Socks_t			ndst;
	int	outport = IPPORT_RESERVED - 1;
	int	turnon = 1;

	bzero((char *)&sin, sizeof(sin));
#if defined(SO_OOBINLINE)
	setsockopt(in, SOL_SOCKET, SO_OOBINLINE, &turnon, sizeof(turnon));
#endif
#if defined(SUPPORT_RCMD)
#ifdef DEBUG
	syslog(LOG_LOW, "DoConnect(): client port=%u", socks_client_port);
#endif /* #ifdef DEBUG */
	if ((socks_client_port < IPPORT_RESERVED) && 
		(socks_client_port >= IPPORT_RESERVED/2)) {
		if ((out = rresvport(&outport)) < 0)
			socks_fail("rresvport()", in, &ndst);
#if defined(hpux)
		ioctl(out, FIOSSAIOOWN, getpid());
#else /* hpux not defined */
		fcntl(out, F_SETOWN, getpid());
#endif /* #if defined(hpux) */
	} else if ((out = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		socks_fail("socket()", in, &ndst);
#else /* SUPPORT_RCMD is not defined */
	if ((out = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		socks_fail("socket()", in, &ndst);
#endif /* #if defined(SUPPORT_RCMD) */

	sin.sin_family = AF_INET;
	sin.sin_port = dst->port;
	sin.sin_addr.s_addr = dst->host;

	ndst.version = Version;
	ndst.cmd = SOCKS_RESULT;

	if (connect(out, (struct sockaddr *)&sin, sizeof(struct sockaddr_in)) < 0)
		socks_fail("connect()", in, &ndst);

	syslog(LOG_LOW, "connected -- %s", log_msg);
#if defined(SO_OOBINLINE)
	setsockopt(out, SOL_SOCKET, SO_OOBINLINE, &turnon, sizeof(turnon));
#endif
	SendDst(in, &ndst);
	Pump(in, out);
	syslog(LOG_LOW, "terminated -- %s.", log_msg);
	syslog(LOG_LOW, "%lu bytes from %s, %lu bytes from %s", from_in, socks_src_name, from_out, socks_dst_name);
}

/*
**  Set up a socket to be connected to from the outside world.
**   diffrence between this an the Version1 protocal is that
**   the socket has to be bound from a specific host that
**   is passed.
*/
DoNewBind(in, dst)
int	in;
Socks_t	*dst;
{
	u_int32 sockd_route();
	int			new, out, len = sizeof(struct sockaddr_in);
	struct sockaddr_in	sin;
	Socks_t			ndst;
	char	dsthost[16];
	char	socks_dst_name[NAMELEN], socks_dst_serv[NAMELEN];
	int	outport = IPPORT_RESERVED - 1;
	int	turnon = 1;

	bzero((char *)&sin, sizeof(sin));
#if defined(SO_OOBINLINE)
	setsockopt(in, SOL_SOCKET, SO_OOBINLINE, &turnon, sizeof(turnon));
#endif

	sin.sin_family = AF_INET;
	ndst.version = Version;
	ndst.cmd  = SOCKS_RESULT;
	sin.sin_port = htons(0);
#ifdef MULTIHOMED_SERVER
	sin.sin_addr.s_addr = sockd_route(dst->host);
#else
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
#endif

#if defined(SUPPORT_RCMD)
#ifdef DEBUG
	syslog(LOG_LOW, "DoNewBind(): client port=%u", socks_client_port);
#endif /* #ifdef DEBUG */
	if ((socks_client_port < IPPORT_RESERVED) && (socks_client_port >= IPPORT_RESERVED/2)){
		if((out = rresvport(&outport)) < 0)
			socks_fail("rresrvport()", in, &ndst);
#ifdef DEBUG
		syslog(LOG_LOW, "DoNewBind(): outport=%d", outport);
#endif /* #ifdef DEBUG */
#if defined(hpux)
		ioctl(out, FIOSSAIOOWN, getpid());
#else /* hpux not defined */
		fcntl(out, F_SETOWN, getpid());
#endif /* #if defined(hpux) */
		sin.sin_port = htons((short)outport);
#ifdef DEBUG
		syslog(LOG_LOW, "DoNewBind(): sin.sin_addr=%s, sin.sin_port=%u",
		    inet_ntoa(sin.sin_addr), ntohs(sin.sin_port));
#endif /* #ifdef DEBUG */
	} else {
#endif /* #if defined(SUPPORT_RCMD) */
	if ((out = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		socks_fail("socket()", in, &ndst);

	if (bind(out, (struct sockaddr *)&sin, sizeof(sin)) < 0)
		socks_fail("bind()", in, &ndst);
	if (getsockname(out, (struct sockaddr *)&sin, &len) < 0)
		socks_fail("getsockname()", in, &ndst);
#if defined(SUPPORT_RCMD)
	}
#endif /* if defined(SUPPORT_RCMD) */

	ndst.port = sin.sin_port;
	ndst.host = sin.sin_addr.s_addr;

	if (listen(out, 1) < 0)
		socks_fail("listen()", in, &ndst);

#if defined(SO_OOBINLINE)
	setsockopt(new, SOL_SOCKET, SO_OOBINLINE, &turnon, sizeof(turnon));
#endif
	SendDst(in, &ndst);

	len = sizeof(struct sockaddr_in);
	if ((new = accept(out, (struct sockaddr *)&sin, &len)) < 0)
		socks_fail("accept()", in, &ndst);
	close(out);

	if (sin.sin_addr.s_addr == 0)
		strcpy(socks_dst_name, "Unspecified.Host");
	else
		saddrtoname(&sin.sin_addr, socks_dst_name, sizeof(socks_dst_name));
	porttoserv(sin.sin_port, socks_dst_serv, sizeof(socks_dst_serv));

#ifdef SHORTENED_RBIND
	if ((dst->host != 0L) && (sin.sin_addr.s_addr != dst->host)) {
#else
	if (sin.sin_addr.s_addr != dst->host) {
#endif
/*
#if defined(SOLARIS) || defined(NeXT) || defined(_SEQUENT_)
*/
	{
		struct	in_addr	inaddr;

		inaddr.s_addr = dst->host;
		strncpy(dsthost, inet_ntoa(inaddr), sizeof(dsthost));
	}
/*
#else
		strncpy(dsthost, inet_ntoa(&dst->host), sizeof(dsthost));
#endif
*/
		syslog(LOG_LOW, "failed -- %s. Error: connected to wrong host %s (%s)",
			log_msg, socks_dst_name, socks_dst_serv);
		ndst.cmd = SOCKS_FAIL;
		SendDst(in, &ndst);
		exit(1);
	}

	syslog(LOG_LOW, "connected -- %s (%s)", log_msg, socks_dst_serv);
	ndst.port = sin.sin_port;
	ndst.host = sin.sin_addr.s_addr;
	SendDst(in, &ndst);
	Pump(in, new);
	syslog(LOG_LOW, "terminated -- %s (%s).", log_msg, socks_dst_serv);
	syslog(LOG_LOW, "%lu bytes from %s, %lu bytes from %s", from_in, socks_src_name, from_out,  socks_dst_name);
}

/*
**  Now just pump the packets/character through..
*/
Pump(in, out)
int	in, out;
{
	static char		buf[4096];
	fd_set			fds;
	int			n, fdsbits;
	static struct timeval	tout = { SOCKS_TIMEOUT, 0 };
/* >>> Andy McFadden fadden@uts.amdahl.com */
 	struct linger ling;	/* for linger */
 	int length;		/* for linger */

	alarm(0);

 	/*
 	 * ATM: use SO_LINGER so it won't hang up on client
 	 */
 	ling.l_onoff = 1;   /* turn it on */
 	ling.l_linger = /*3*/  10;
 	length = sizeof(ling);
 	if (setsockopt(in,  SOL_SOCKET, SO_LINGER, &ling, length) < 0)
 		perror("setsockopt (SO_LINGER) in");
 	if (setsockopt(out, SOL_SOCKET, SO_LINGER, &ling, length) < 0)
 		perror("setsockopt (SO_LINGER) out");
/* <<< Andy McFadden fadden@uts.amdahl.com */

	FD_ZERO(&fds);
	if (in > out)
		fdsbits = in + 1;
	else
		fdsbits = out +1;

	while (1) {
		tout.tv_sec = SOCKS_TIMEOUT;
		tout.tv_usec = 0;
		FD_SET(in, &fds);
		FD_SET(out, &fds);
		if ((n = select(fdsbits, &fds, NULL,NULL, &tout)) > 0) {
			if (FD_ISSET(in, &fds)) {
				if ((n = read(in, buf, sizeof buf)) > 0) {
					from_in += n;
					if (write(out, buf, n) < 0) {
						goto bad;
					}
				} else {
					goto bad;
				}
			}
			if (FD_ISSET(out, &fds)) {
				if ((n = read(out, buf, sizeof buf)) > 0) {
					from_out += n;
					if (write(in, buf, n) < 0) {
						goto bad;
					}
				} else {
					goto bad;
				}
			}
		} else {
			if (n != 0)
				syslog(LOG_LOW, "select %m\n");
			goto bad;
		}
	}

bad:
	;	/* Make the goto happy */
}


#ifdef FOR_PS

/*
**  SETPROCTITLE -- set process title for ps
**
**    Parameters:
**	    fmt -- a printf style format string.
**	    a, b, c -- possible parameters to fmt.
**
**    Returns:
**	    none.
**
**      Side Effects:
**	      Clobbers argv of our main procedure so ps(1) will
**	      display the title.
**
**    Stolen from IDA Sendmail - I don't think it's UCB code.
*/

/*VARARGS1*/
setproctitle(buf, Argv, LastArgv)
char *buf;
char **Argv, *LastArgv;
{
#if defined(FOR_PS) && !defined(SYSV)
	register char *p;
	register int i;

	/* make ps print "(sockd)" */
	p = Argv[0];
	*p++ = '-';

	i = strlen(buf);
	if (i > LastArgv - p - 2)
	{
		i = LastArgv - p - 2;
		buf[i] = '\0';
	}
	(void) strcpy(p, buf);
	p += i;
	while (p < LastArgv)
		*p++ = ' ';
#endif /* FOR_PS && !SYSV */
}
#endif /* #ifdef FOR_PS */

#endif /* #ifndef TEST */

#ifdef MULTIHOMED_SERVER

static char *sockd_route_file = SOCKD_ROUTE_FILE;

u_int32 sockd_route(dsthost)
u_int32 dsthost;
{
	FILE	*fd;
	static char buf[1024];
#ifdef TEST
	char	temp[1024];
#endif
	char	*bp;
	int	linenum = 0;
	char	*argv[3];	
	int	argc;
	u_int32	interface, destip, destmask;

#ifdef TEST
	fprintf(stderr,"==== Checking routing file (%s)...\n", sockd_route_file);
#endif

	if ((fd = fopen(sockd_route_file, "r")) == NULL) {
#ifdef TEST
		fprintf(stderr, "Unable to open routing file (%s)\n", sockd_route_file);
		return(0);
#else
		syslog(LOG_HIGH, "Unable to open routing file (%s)", sockd_route_file);
		exit(1);
#endif
	}
	
	while (fgets(buf, sizeof(buf) - 1, fd) != NULL) {
		linenum++;
#ifdef TEST
		strcpy(temp, buf);
#endif
		/* Comment starts with # anywhere in the line */
		if ((bp = index(buf, '\n')) != NULL)
			*bp ='\0';
		for (bp = buf; *bp; bp++ ) {
			if (*bp == '#') {
				*bp = '\0';
				break;
			} else if (*bp == '\t')
				*bp = ' ';
		}
		mkargs(buf, &argc, argv, 3);
		if (argc == 0)
			continue;
		if (argc != 3) {
#ifdef TEST
			fprintf(stderr, "Invalid entry at line %d\n", linenum);
#else
			syslog(LOG_LOW, "Invalid entry at line %d", linenum);
#endif
			continue;
		}
		GetAddr(argv[0], &interface);
		GetAddr(argv[1], &destip);
		GetAddr(argv[2], &destmask);
#ifdef OLD_CONF_MASK
		if ((destip & ~destmask) == (dsthost & ~destmask)) {
#else
		if ((destip & destmask) == (dsthost & destmask)) {
#endif
#ifdef TEST
			fprintf(stderr, "Line %d: %s", linenum, temp);
#endif
			fclose(fd);
			return(interface);
		}
	}
	fclose(fd);
#ifdef TEST
/*
# if defined(SOLARIS) || defined(NeXT) || defined(_SEQUENT_)
*/
    {
	struct in_addr inaddr;

	inaddr.s_addr = dsthost;
	fprintf(stderr, "***Cannot find appropriate interface to communicate with %s\n", inet_ntoa(inaddr));
    }
/*
# else
	fprintf(stderr, "***Cannot find appropriate interface to communicate with %s\n", inet_ntoa(&dsthost));
# endif
*/
	return(0);
#else
/*
# if defined(SOLARIS) || defined(NeXT) || defined(_SEQUENT_)
*/
    {
	struct in_addr inaddr;

	inaddr.s_addr = dsthost;
	syslog(LOG_HIGH, "***Cannot find appropriate interface to communicate with %s\n", inet_ntoa(inaddr));
    }
/*
# else
	syslog(LOG_HIGH, "***Cannot find appropriate interface to communicate with %s\n", inet_ntoa(&dsthost));
# endif
*/
	exit(1);
#endif
}

#endif /* #ifdef MULTIHOMED_SERVER */

check_sp_conf(fd, s, src, dst)
FILE	*fd;
char	*s;
struct sockaddr_in	*src, *dst;
{
	char	buf[1024], *p;
#ifdef TEST
	int		linenum = 0;
#endif
	while (fgets(buf, sizeof(buf) - 1, fd) != NULL) {
		if ((p = index(buf, '\n')) != NULL)
			*p = '\0';
#ifdef TEST
		linenum++;
#endif
		if (strncmp(s, buf, strlen(s)) == 0) {
#ifdef TEST
		fprintf(stderr,"Line %d: %s\n", linenum, buf);
#endif
			shell_cmd(buf+strlen(s), src, dst);
			break;
		}
	}
	return;
}

Validate(src, dst, in, identd)
struct sockaddr_in	*src, *dst;
int	in;
int	identd;
{
	FILE		*fd;
	static char	buf[1024];
#ifdef TEST
	char		temp[1024];
#endif
	char		*bp;
	int		linenum = 0, permit;
	char		*argv[10];
	int		argc;
	u_int32	saddr, smask, daddr, dmask;
	unsigned short	dport;
	enum 		{ e_lt, e_gt, e_eq, e_neq, e_le, e_ge, e_nil } tst;
	char		*userlist;
	int		next_arg;
	unsigned short	dst_sin_port = ntohs(dst->sin_port); /* dst->sin_port in host byte order */
	long	p;
	char	*cmdp;
	int	use_identd;
	IDENT	*ident_lookup(), *idp;


	if ((fd = fopen(sockd_conf, "r")) == NULL) {
#ifdef TEST
		fprintf(stderr,"Unable to open config file (%s)\n", sockd_conf);
#else /* #ifdef TEST */
		syslog(LOG_HIGH, "Unable to open config file (%s)", sockd_conf);
#endif /* #ifdef TEST */

		return 0;
	}
	while (fgets(buf, sizeof(buf) - 1, fd) != NULL) {
		linenum++;
		use_identd = identd;
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

		mkargs(buf, &argc, argv, 8);
		if (argc == 0)
			continue;
		if ((argc < 3) || (argc > 9)) {
#ifdef TEST
			fprintf(stderr, "Invalid entry at line %d\n", linenum);
#else
			syslog(LOG_LOW, "Invalid entry at line %d", linenum);
#endif
			continue;
		}

		if (STREQ(argv[0], "permit")) {
			permit = 1;
		} else if (STREQ(argv[0], "deny")) {
			permit = 0;
		} else {
#ifdef TEST
			fprintf(stderr, "Invalid permit/deny field at line %d\n", linenum);
#else
			syslog(LOG_LOW, "Invalid permit/deny field at line %d", linenum);
#endif
			continue;
		}

		userlist = (char *)0;
		next_arg = 1;

		if (strncmp(argv[next_arg], "?=", 2) == 0) {
			switch (argv[next_arg++][2]) {
#ifdef TEST
				case 'I':
				case 'i':
					break;
#else /* TEST not defined */
				case 'I':
					use_identd = 2;
					break;
				case 'i':
					use_identd = 1;
					break;
#endif /* #ifdef TEST */
				case 'n':
				case '\0':
					use_identd = 0;
					break;
				default:
#ifdef TEST
					fprintf(stderr, "Invalid ?= field at line %d\n", linenum);
#else
					syslog(LOG_LOW, "Invalid ?= field at line %d", linenum);
#endif
					continue;
			}
		}
		if (strncmp(argv[next_arg], "*=", 2) == 0) {
			if (argv[next_arg][2]) userlist = argv[next_arg] + 2;
			next_arg++;
		}
		if(argc <= next_arg+1) {
#ifdef TEST
			fprintf(stderr, "Invalid entry at line %d\n", linenum);
#else
			syslog(LOG_LOW, "Invalid entry at line %d", linenum);
#endif
			continue;
		}

		GetAddr(argv[next_arg++], &saddr);
		GetAddr(argv[next_arg++], &smask);

		if ((argc > next_arg + 1) &&
			!(STREQ(argv[next_arg], "eq") || STREQ(argv[next_arg], "neq") ||
			  STREQ(argv[next_arg], "lt") || STREQ(argv[next_arg], "gt") ||
			  STREQ(argv[next_arg], "le") || STREQ(argv[next_arg], "ge"))) {
			GetAddr(argv[next_arg++], &daddr);
			GetAddr(argv[next_arg++], &dmask);

		} else {
			daddr = 0;
#ifdef OLD_CONF_MASK
			dmask = ~0;
#else	
			dmask = 0;
#endif
		}
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
				fprintf(stderr, "Invalid comparison at line %d\n", linenum);
#else
				syslog(LOG_LOW, "Invalid comparison at line %d", linenum);
#endif
				continue;
			}
				
			if (((p = GetPort(argv[next_arg+1])) < 0) ||
				(p >= (1L << 16))) {
#ifdef TEST
				fprintf(stderr, "Invalid port number at line %d\n", linenum);
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
				sprintf(msg,"%s %s 0x%08x 0x%08x 0x%08x 0x%08x %s %u",
					permit ? "permit" : "deny",
					userlist,
					saddr, smask, daddr, dmask,
						tst == e_eq ? "==" :
						tst == e_neq ? "!=" :
						tst == e_lt ? "<" :
						tst == e_gt ? ">" :
						tst == e_le ? "<=" :
						tst == e_ge ? ">=" : "NIL",
						dport);
			else
				sprintf(msg,"%s 0x%08x 0x%08x 0x%08x 0x%08x %s %u",
					permit ? "permit" : "deny",
					saddr, smask, daddr, dmask,
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
		if((saddr & ~smask) == (src->sin_addr.s_addr & ~smask) &&
		    (daddr & ~dmask) == (dst->sin_addr.s_addr & ~dmask) &&
#else
		if((saddr & smask) == (src->sin_addr.s_addr & smask) &&
		    (daddr & dmask) == (dst->sin_addr.s_addr & dmask) &&
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
	fprintf(stderr, "*** No match with any line. Access denied.\n");
#endif
	return 0;

GotIt:
#ifdef TEST
	fprintf(stderr, "Line %d: %s", linenum, temp);
	fseek(fd, 0L, 0);
	if(permit && use_identd)
		permit = use_identd;
	if (permit == -3)
		check_sp_conf(fd, "#BAD_ID:", src, dst);
	else if ((permit == -1) || (permit == -2))
		check_sp_conf(fd, "#NO_IDENTD:", src, dst);
		
#else /* # TEST not defined */
	if ((use_identd == 0) || (permit == 0)) {
		fclose(fd);
		if (cmdp != (char *)0) {
			shell_cmd(cmdp, src, dst);
		}
		return permit;
	}
	fseek(fd, 0L, 0);
	if ((idp = ident_lookup(in, IDENTD_TIMEOUT)) == ((IDENT *)0)) {
		check_sp_conf(fd, "#NO_IDENTD:", src, dst);
		permit = -use_identd;
	} else {
		strncpy(socks_real_user, idp->identifier, sizeof(socks_real_user));
		if (strcmp(socks_src_user, socks_real_user)) {
#if defined(SUPPORT_RCMD)
			if ((socks_client_port >= IPPORT_RESERVED) ||
			   (socks_client_port < IPPORT_RESERVED/2) ||
			   strcmp(socks_src_user, "root")) {
#endif /* #if defined(SUPPORT_RCMD) */
				permit = -3;
#if defined(SUPPORT_RCMD)
			}
#endif /* #if defined(SUPPORT_RCMD) */
		}
	}
	ident_free(idp);
#endif /* #ifdef TEST */
	fclose(fd);
	if ((permit >= -1) && cmdp)
		shell_cmd(cmdp, src, dst);
	return permit;
}

/*-------------------------------------------------*/

#ifdef TEST

char 	*testpname;

error_and_quit()
{
#ifdef MULTIHOMED_SERVER
	fprintf(stderr, "Usage:\n%s [-i] [-I] [-B] [-R route_file] [-C conf_file] user src_addr dst_addr {service,port}\n", testpname);
#else
	fprintf(stderr, "Usage:\n%s [-i] [-I] [-B] [-C conf_file] user src_addr dst_addr {service,port}\n", testpname);
#endif
		exit(-1);
}

main(argc, argv)
int	argc;
char	*argv[];

/*
 * test_sockd_conf [-i] [-I] [-B] [-R route_file] [-C conf_file] user src_addr dst_addr {service,port}
 *
 * Prints out the line in configuration file that are examined and
 * whether the access is permitted or denied. Sets exit status code
 * to 1 for permit, 0 for deny, -1 for error in command line.
 */

{
	int next = 1;
	long	p;
	u_short	port;
	struct sockaddr_in	from, dstsin;
	char	log_msg[1024];
	char	command = SOCKS_CONNECT;
	int	sim_id = 0;
	char	buf[1024];
	int	permit;
	int	in = 0;

	bzero((char *)&from, sizeof(from));
	bzero((char *)&dstsin, sizeof(dstsin));
	if(testpname = rindex(argv[0], '/'))
		++testpname;
	else
		testpname = argv[0];

	while (--argc && (argv[next][0] == '-')) {
		switch (argv[next++][1]) {
		case 'B':
			sim_id = -3;
			break;
		case 'C':
			if (--argc) {
				sockd_conf =argv[next++];
				break;
			} else
				error_and_quit();
		case 'I':
			sim_id = -2;
			break;
#ifdef MULTIHOMED_SERVER
		case 'R':
			if (--argc) {
				sockd_route_file =argv[next++];
				break;
			} else
				error_and_quit();
#endif
		case 'i':
			sim_id = -1;
			break;
		default:
			error_and_quit();
		}
	}

	if (argc != 4) 
			error_and_quit();

	strncpy(socks_src_user, argv[next++], sizeof(socks_src_user));
	GetAddr(argv[next++], &from.sin_addr.s_addr);
	GetAddr(argv[next++], &dstsin.sin_addr.s_addr);
	if (((p = GetPort(argv[next])) < 0) || (p >= (1L << 16))) {
		fprintf(stderr, "%s: Unknown service or illegal port number '%s'\n",testpname, argv[next]);
		exit(-1);
	}
	dstsin.sin_port = htons(port = p);

	if (sim_id)
		strcpy(socks_real_user,"unknown");
	else
		strcpy(socks_real_user, socks_src_user);

	saddrtoname(&from.sin_addr, socks_src_name, sizeof(socks_src_name));
	saddrtoname(&dstsin.sin_addr, socks_dst_name, sizeof(socks_dst_name));
	porttoserv(dstsin.sin_port, socks_dst_serv, sizeof(socks_dst_serv));
	sprintf(log_msg, "connect from %s(%s)@%s to %s (%s)",
		socks_src_user, socks_real_user, socks_src_name, socks_dst_name, socks_dst_serv);
#ifdef LOG_DAEMON
	openlog(testpname, LOG_PID, SYSLOG_FAC);
#else
	openlog(testpname, LOG_PID);
#endif

	strcpy(buf, inet_ntoa(from.sin_addr));
	fprintf(stderr, "USER:%s,  SRC:%s,  DST:%s, PORT:%u\n",
		socks_src_user, buf, inet_ntoa(dstsin.sin_addr),
		ntohs(dstsin.sin_port));

	permit = Validate(&from, &dstsin, in, sim_id);

	switch (permit) {
	case 0:
		syslog(LOG_LOW, "refused -- %s", log_msg);
		exit(0);
	case -3:
		syslog(LOG_LOW, "*Alert*: real user is %s, not %s", socks_real_user, socks_src_user);
		syslog(LOG_LOW, "refused -- %s", log_msg);
		fprintf(stderr, "Access denied: bad user-id.\n");
		exit(0);
	case -2:
		syslog(LOG_LOW, "cannot connect to identd on %s", socks_src_name);
		syslog(LOG_LOW, "refused -- %s", log_msg);
		fprintf(stderr, "Access denied: cannot verify user-id.\n");
		exit(0);
	case -1:
		syslog(LOG_LOW, "cannot connect to identd on %s", socks_src_name);
		break;
	case 1:
		break;
	default:
		syslog(LOG_HIGH, "refused -- %s", log_msg);
		syslog(LOG_HIGH, "Unexpected result from Validate");
		exit(0);
	}
#ifdef MULTIHOMED_SERVER
	if (sockd_route(dstsin.sin_addr.s_addr)) {
#endif /* #ifdef MULTIHOMED_SERVER */
		syslog(LOG_LOW, "connected -- %s", log_msg);
		exit(1);
#ifdef MULTIHOMED_SERVER
	} else {
		syslog(LOG_LOW, "refused -- %s", log_msg);
		exit(0);
	}
#endif /* #ifdef MULTIHOMED_SERVER */
	
}

#endif /* #ifdef TEST */
