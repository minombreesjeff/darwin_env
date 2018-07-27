#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/errno.h>
#include <stdio.h>
#include <netdb.h>
#include <arpa/nameser.h>
#include <resolv.h>
#include <pwd.h>
#include <syslog.h>
#if (defined(sun) && !defined(SOLARIS)) || defined(sgi)
#include <strings.h>
#else
#include <string.h>
#endif
#include "socks.h"
#ifdef LINUX
#include <linux/time.h>
#endif

/* >>> K. Shackelford */
#if defined(hpux) || defined(ultrix) || defined (__NetBSD__) || defined(AIX) || defined(__bsdi__) || defined(SCO)
extern int h_errno;
#endif
/* <<< K.Shackelford */

#include <signal.h>
#include <sys/wait.h>

#define NAMELEN	128
char socks_dst_name[NAMELEN], socks_dst_serv[NAMELEN];
char socks_src_name[NAMELEN], socks_src_user[NAMELEN], socks_real_user[NAMELEN];
char *socks_def_server;
char *socks_server;
char *socks_serverlist;

char socks_cmd[] = "connect";

extern int 	errno;
extern char	*getenv();
extern char	*getlogin();
static struct sockaddr_in	cursin;
u_int32		SocksHost;
static unsigned short socks_port;
static int socks_conn_sock = 0;
static int socks_conn_init = 0;
static unsigned short	socks_conn_port = 0;
static u_int32	socks_conn_host = 0;
static int		socks_conn_code = 0;
static unsigned short	socks_last_conn_port = 0;
static u_int32	socks_last_conn_host = 0;
static int	socks_init_done = 0;

struct sockaddr_in socks_nsin;
static struct sockaddr_in	me;
static struct passwd		*pw;
static int	direct = 0;

extern	char	*porttoserv();
extern	char	*saddrtoname();


int	check_result(code)
char code;
{
	switch (code) {
		case SOCKS_FAIL:
/*
			errno = ETIMEDOUT;
*/
			errno = ECONNREFUSED;
			return -1;
		case SOCKS_NO_IDENTD:
			errno = ECONNREFUSED;
			fprintf(stderr, "Error: SOCKS proxy server cannot connect to identd on your machine.\n");
			return -1;
		case SOCKS_BAD_ID:
			errno = ECONNREFUSED;
			fprintf(stderr, "Error: user-id does not agree with the one reported by identd on your machine.\n");
			return -1;
		default:
			return 0;
	}
}

/*
	SOCKSinit() must be called once in the application program.
 */

SOCKSinit(Progname)
char *Progname; /* name of the calling program, "rfinger", "rftp", etc. */
{
#ifdef SOCKS_DEFAULT_NS
	static char	defaultNS[] = SOCKS_DEFAULT_NS;
#endif
#ifdef SOCKS_DEFAULT_DNAME
	static char	defaultDNAME[] = SOCKS_DEFAULT_DNAME;
#endif
	static char	defaultSERVER[] = SOCKS_DEFAULT_SERVER;
	char		*cp, *ns, *dp;
	struct hostent	*hp;
	struct servent	*sp;
	int		v,uid;

/* >>> YDL 94/01/25 */
	if (socks_init_done)
		return;
	socks_init_done = 1;
/* <<< YDL 94/01/25 */
	socks_port = htons(SOCKS_DEF_PORT);

	bzero((char *)&cursin, sizeof(cursin));
	bzero((char *)&socks_nsin, sizeof(socks_nsin));
	bzero((char *)&me, sizeof(me));
	/* skip the path if included in Progname */
	if( (cp = rindex(Progname, '/')) == NULL)
		cp = Progname;
	else
		cp++;

#ifndef LOG_DAEMON
	(void) openlog(cp, LOG_PID);
#else
	(void) openlog(cp, LOG_PID, SYSLOG_FAC);
#endif

	gethostname(socks_src_name, sizeof(socks_src_name));
	if ( (hp = gethostbyname(socks_src_name)) == NULL ) {
		fprintf (stderr, "gethostbyname(%s): error #%d\n",
			socks_src_name, h_errno);
		return (1);
	}
	bcopy(hp->h_addr_list[0], &me.sin_addr.s_addr, hp->h_length);

#if !defined(DNS_THROUGH_NIS)

	if ((ns = getenv("SOCKS_NS")) == NULL) {
#ifdef SOCKS_DEFAULT_NS
		ns = defaultNS;
#else
		;
#endif
	}
	if ((dp = getenv("SOCKS_DNAME")) == NULL) {
#ifdef SOCKS_DEFAULT_DNAME
		dp = defaultDNAME;
#else
		;
#endif
	}

	if ((ns != NULL) || (dp != NULL)) {
		res_init();
#ifdef sgi
		sethostresorder("local:nis:bind");
#endif
	}

	if (ns != NULL) {
#ifdef ultrix
		_res.ns_list[0].addr.sin_addr.s_addr = inet_addr(ns);
#else
		_res.nsaddr_list[0].sin_addr.s_addr = inet_addr(ns);
#endif
		_res.nscount = 1;
	}
	if (dp != NULL) {
		strncpy(_res.defdname, dp, sizeof(_res.defdname)-1);
	}

/* >>> jon r. luini <jonl@hal.com> */
/*
#ifdef	SOCKS_DEFAULT_DNAME
	bzero (_res.defdname, sizeof (_res.defdname));

	if ( (cp = getenv("SOCKS_DNAME")) != NULL ) 
	{
	    strncpy (_res.defdname, cp, sizeof (_res.defdname)-1);
	}
	else
	{
	    strncpy (_res.defdname, SOCKS_DEFAULT_DNAME,
	        sizeof (_res.defdname)-1);
	}
#endif
*/
/* <<< jon r. luini <jonl@hal.com> */

#endif /* #if !defined(DNS_THROUGH_NIS) */

	if ((socks_def_server = getenv("SOCKS_SERVER")) == NULL)
		socks_def_server = defaultSERVER;
	socks_server = socks_def_server;
	if ((cp = getenv("SOCKS_BANNER")) != NULL) {
		fprintf(stderr, "CSTC version %s SOCKS client. Default SOCKS server: %s\n",
			CSTC_RELEASE, socks_def_server);
	}

	if ((hp = gethostbyname(socks_server)) == NULL) {
		SocksHost = inet_addr(socks_server);
	} else {
		bcopy(hp->h_addr_list[0], &SocksHost, hp->h_length);
	}

	if ((sp = getservbyname("socks", "tcp")) != NULL)
		socks_port = sp->s_port;

	if ((cp = getlogin()) == NULL) {
		if ((pw = getpwuid(uid=getuid())) == NULL) {
			fprintf(stderr, "Unknown user-id %d\n",uid);
			return (1);
		}
		cp = pw->pw_name;
	}
	strncpy(socks_real_user, cp, sizeof(socks_real_user));

	if ((pw = getpwuid(uid=geteuid())) == NULL) {
		fprintf(stderr, "Unknown user-id %d\n",uid);
		return (1);
	}
	strncpy(socks_src_user, pw->pw_name, sizeof(socks_src_user));

	socks_nsin.sin_family = AF_INET;
	socks_nsin.sin_port = socks_port;
	socks_nsin.sin_addr.s_addr = SocksHost;
	
}


int connect_sockd(sock, lport)
int	sock;
int	*lport;
/* returns 0 if successfully connected to a SOCKS server,
   returns -1 otherwise
 */
{
#ifndef VERSATILE_CLIENTS
	if (connect(sock, &socks_nsin, sizeof(struct sockaddr_in)) == 0)
		return 0;
	else {
		syslog(LOG_LOW, "Failed to connect to sockd at %s: %m",
			socks_server);
		return -1;
	}
#else /* Now the version when VERSATILE_CLIENTS is defined */
	int	last = 0;
	int	new_sock;
	struct hostent	*hp;

	while (socks_server = socks_serverlist) {
		if (socks_serverlist = index(socks_serverlist, ','))
			*socks_serverlist++ = '\0';
		if ((hp = gethostbyname(socks_server)) == NULL)
			socks_nsin.sin_addr.s_addr = inet_addr(socks_server);
		else
			bcopy(hp->h_addr_list[0], &socks_nsin.sin_addr, hp->h_length);
		if (connect(sock, (struct sockaddr *)&socks_nsin, sizeof(struct sockaddr_in)) == 0)
			return 0;
		else {
#ifdef SVR4
			if ((errno == EISCONN) || (errno == EINPROGRESS) || (errno == EAGAIN))
#else
			if ((errno == EISCONN) || (errno == EINPROGRESS))
#endif
				return -1;
			syslog(LOG_LOW, "Failed to connect to sockd at %s: %m",
				socks_server);
			if (!(socks_serverlist)) {
				return -1;
			}
#if defined(SUPPORT_RCMD)
			if ((*lport < IPPORT_RESERVED) && (*lport >= IPPORT_RESERVED/2))
				new_sock = rresvport(lport);
			else 
				new_sock = socket(PF_INET, SOCK_STREAM, 0);
#else /* SUPPORT_RCMD is not defined */
			new_sock = socket(PF_INET, SOCK_STREAM, 0);
#endif /* #if defined(SUPPORT_RCMD) */
			if (new_sock < 0) {
				return -1;
			}
			if (dup2(new_sock, sock) < 0) {
				close(new_sock);
				return -1;
			} else {
				close(new_sock);
			}
		}
	}
	errno = ECONNREFUSED;
	return -1;
#endif /* #ifndef VERSATILE_CLIENTS */
	
}

static int send_src_user(s, user)
int s;
char *user;
{
	char *p = user;
	int i, n, ret;
	fd_set	fds;
	int	fdsbits = s + 1;
	struct	timeval timeout;

	i = strlen(user) + 1;
	while ( i > 0) {
		FD_ZERO(&fds);
		FD_SET(s, &fds);
		timeout.tv_sec = 15;
		timeout.tv_usec = 0;
		if ((ret = select(fdsbits, NULL, &fds, NULL, &timeout)) < 0) {
			return(-1);
		}
		if (ret == 0)
			continue;
		if((n = write(s, p, i)) <= 0) {
			return(-2);
		}
		p += n;
		i -= n;
	}
	return(0);
}


static int socksC_proto(s, dst)
int s;
Socks_t	*dst;
{
	int sta;

	if ((sta = SendDst(s, dst)) < 0) {
		if (sta == -1)
			perror("select in SendDst");
		else
			perror("write in SendDst");
		return(sta);
	}
	if ((sta = send_src_user(s, socks_src_user)) < 0) {
		if (sta == -1)
			perror("select in send_src_user");
		else
			perror("write in send_src_user");
		return(sta);
	}
	if ((sta = GetDst(s, dst)) < 0) {
		if (sta == -1)
			perror("select in GetDst");
		else 
			perror("read in GetDst");
		return(sta);
	}
	return(0);
}

static void quit_C_proto()
{
	exit(SOCKS_FAIL);
}

static void do_C_proto(sock, port, addr)
int sock;
unsigned short port;
u_int32 addr;
{
	Socks_t	dst;

	signal(SIGALRM, quit_C_proto);
	alarm(CLIENT_CONN_TIMEOUT);
	dst.version = SOCKS_VERSION;
	dst.cmd = SOCKS_CONNECT;
	dst.port = port;
	dst.host = addr;
	if (socksC_proto(sock, &dst) < 0) {
		alarm(0);
		exit(SOCKS_FAIL);
	}
	alarm(0);
	if ((dst.cmd == SOCKS_FAIL) || (dst.cmd == SOCKS_NO_IDENTD)
	    || (dst.cmd == SOCKS_BAD_ID)) {
		exit(dst.cmd);
	}
	exit(SOCKS_RESULT);

}


Rconnect(sock, sin, size)
int			sock;
struct sockaddr_in	*sin;
int			size;
{
	Socks_t		dst;
	int	i;
	int	res_ret, con_ret, con_errno;
	int	lport = 0;

	int status, wait_ret, child_pid;

	if (socks_init_done == 0)
		SOCKSinit("SOCKSclient");
#ifdef DEBUG
fprintf(stderr, "Rconnect 10\n");
fprintf(stderr,"socks_conn_sock=%d, socks_conn_host=%ld, socks_conn_port=%d, socks_conn_init=%d\n", socks_conn_sock, socks_conn_host, socks_conn_port, socks_conn_init);
fprintf(stderr,"sock=%d, sin->sin_addr.s_addr=%ld, sin->sin_port=%d\n", sock, sin->sin_addr.s_addr, sin->sin_port);
#endif /* #ifdef DEBUG */

	if ((sock != socks_conn_sock) || (sin->sin_port != socks_conn_port)
	    || (sin->sin_addr.s_addr != socks_conn_host)) {
		if (socks_conn_init)
			kill(socks_conn_init, SIGKILL);
		socks_conn_code = 0;
		socks_conn_init = 0;
		strcpy(socks_cmd, "connect");
		saddrtoname(&sin->sin_addr, socks_dst_name, sizeof(socks_dst_name));
		porttoserv(sin->sin_port, socks_dst_serv, sizeof(socks_dst_serv));
#ifdef DEBUG
fprintf(stderr, "Rconnect 100, Rconnect(%d, %s, %s)\n", sock, socks_dst_name, socks_dst_serv);
#endif /* #ifdef DEBUG */
	} else if (status = socks_conn_code) {
		socks_conn_init = 0;
		socks_conn_code = 0;
		socks_conn_sock = 0;
		socks_conn_port = 0;
		socks_conn_host = 0;
		res_ret = check_result(status);
		if (status == SOCKS_RESULT) {
			errno = EISCONN;
			socks_last_conn_host = sin->sin_addr.s_addr;
			socks_last_conn_port = sin->sin_port;
		} else {
			syslog(LOG_LOW, "Connection failed.\n");
		}
		return(-1);
	} else if (socks_conn_init) {
#ifdef DEBUG
fprintf(stderr, "Rconnect 200, Rconnect(%d, %s, %s)\n", sock, socks_dst_name, socks_dst_serv);
#endif /* #ifdef DEBUG */
		wait_ret = waitpid(socks_conn_init, &status, WNOHANG);
		if (wait_ret == 0) {
			errno = EALREADY;
#ifdef DEBUG
fprintf(stderr, "Rconnect 300, ret=-1, errno=EALREADY\n");
#endif /* #ifdef DEBUG */
			return(-1);
		} else if (wait_ret == socks_conn_init) {
			socks_conn_init = 0;
			socks_conn_code = 0;
			socks_conn_sock = 0;
			socks_conn_port = 0;
			socks_conn_host = 0;
			if (status & 0x00ff) {
				kill(socks_conn_init, SIGKILL);
				errno = ECONNREFUSED;
#ifdef DEBUG
fprintf(stderr, "Rconnect 400, ret=-1, errno=ECONNREFUSED\n");
#endif /* #ifdef DEBUG */
				syslog(LOG_LOW, "Connection failed.\n");
				return(-1);
			} else {
				status = (status >> 8) & 0x00ff;
				res_ret = check_result(status);
#ifdef DEBUG
fprintf(stderr, "Rconnect 500, ret=%d, status=%d\n", res_ret, status);
#endif /* #ifdef DEBUG */
				if (res_ret == 0) {
					errno = EISCONN;
					socks_last_conn_host = sin->sin_addr.s_addr;
					socks_last_conn_port = sin->sin_port;
				} else {
					syslog(LOG_LOW, "Connection failed.\n");
				}
				return(-1);
			}
		} else {
			kill(socks_conn_init, SIGKILL);
			errno = ECONNREFUSED;
#ifdef DEBUG
fprintf(stderr, "Rconnect 600, ret=-1, errno=ECONNREFUSED\n");
#endif /* #ifdef DEBUG */
			socks_conn_init = 0;
			socks_conn_code = 0;
			socks_conn_sock = 0;
			socks_conn_port = 0;
			socks_conn_host = 0;
			syslog(LOG_LOW, "Connection failed.\n");
			return(-1);
		}
	}

#ifdef DEBUG
fprintf(stderr, "Rconnect 700, Rconnect(%d, %s, %s)\n", sock, socks_dst_name, socks_dst_serv);
#endif /* #ifdef DEBUG */

#ifdef VERSATILE_CLIENTS
	direct = check_cconf(&me, sin);
#ifdef DEBUG
fprintf(stderr, "Rconnect() 800: direct = %d\n", direct);
#endif /* #ifdef DEBUG */
	if (direct < 0) {
		syslog(LOG_LOW, "refused -- connect() from %s(%s) to %s (%s)",
			socks_src_user, socks_real_user, socks_dst_name, socks_dst_serv);
		errno = ECONNREFUSED;
#ifdef DEBUG
fprintf(stderr, "Rconnect 900, ret=-1, errno=EALREADY\n");
#endif /* #ifdef DEBUG */
		return(-1);
	}

	if (direct == 1) {
		syslog(LOG_LOW, "connect() directly from %s(%s) to %s (%s)",
			socks_src_user, socks_real_user, socks_dst_name, socks_dst_serv);
		con_ret = connect(sock, (struct sockaddr *)sin, size);
		if (con_ret == 0) {
			socks_last_conn_host = sin->sin_addr.s_addr;
			socks_last_conn_port = sin->sin_port;
		}
#ifdef DEBUG
fprintf(stderr, "Rconnect 1000, ret=%d, ", con_ret);
perror(" ");
#endif /* #ifdef DEBUG */
		return(con_ret);
	}
#endif /* #ifdef VERSATILE_CLIENTS */
	
	con_ret = connect_sockd(sock, &lport);
#ifdef DEBUG
	fprintf(stderr, "con_ret=connect_sockd()=%d", con_ret);
	if(con_ret < 0)
		perror("");
	else
		fprintf(stderr,"\n");
#endif /* #ifdef DEBUG */
	if (con_ret == 0) {
		syslog(LOG_LOW, "connect() from %s(%s) to %s (%s) using sockd at %s",
				socks_src_user, socks_real_user, socks_dst_name, socks_dst_serv, socks_server);
		dst.version = SOCKS_VERSION;
		dst.cmd = SOCKS_CONNECT;
		dst.port = sin->sin_port;
		dst.host = sin->sin_addr.s_addr;
		if (socksC_proto(sock, &dst) < 0) {
			return(-1);
		}
		res_ret = check_result(dst.cmd);
#ifdef DEBUG
fprintf(stderr, "Rconnect 1100, ret=%d, ", res_ret);
perror(" ");
#endif /* #ifdef DEBUG */
		if (res_ret == 0) {
			socks_last_conn_host = sin->sin_addr.s_addr;
			socks_last_conn_port = sin->sin_port;
		}
		return(res_ret);
	}
	if ((con_ret < 0) && (errno != EINPROGRESS)) {
#ifdef DEBUG
fprintf(stderr, "Rconnect 1200, ret=%d, ", con_ret);
perror(" ");
#endif /* #ifdef DEBUG */
		return(-1);
	}
/*
	con_errno = errno;
*/
	syslog(LOG_LOW, "connect() from %s(%s) to %s (%s) using sockd at %s",
			socks_src_user, socks_real_user, socks_dst_name, socks_dst_serv, socks_server);

	switch (child_pid = fork()) {
	case -1:
		perror("fork()");
		errno = ECONNREFUSED;
#ifdef DEBUG
fprintf(stderr, "Rconnect 1300, ret=-1, ");
perror(" ");
#endif /* #ifdef DEBUG */
		return(-1);
	case 0:
		do_C_proto(sock, sin->sin_port, sin->sin_addr.s_addr);
	default:
		socks_conn_init = child_pid;
		socks_conn_code = 0;
		socks_conn_sock = sock;
		socks_conn_port = sin->sin_port;
		socks_conn_host = sin->sin_addr.s_addr;
		errno = EINPROGRESS;
#ifdef DEBUG
fprintf(stderr, "Rconnect 1400, ret=-1 ");
perror(" ");
#endif /* #ifdef DEBUG */
		return(-1);
	}
}

/* >>> YDL 94/01/25 */
#ifdef SHORTENED_RBIND
Rbind(sock, sin, size)
int			sock;
struct sockaddr_in	*sin;
int			size;
#else
/*
**  Set up a bind for a remote host, add fill 'cursin' in with the
**   remote server information.
**  If using reserved port, the port must have already been reserved
**   through a rresvport() call.
*/
Rbind(sock, sin, size, dsthost)
int			sock;
struct sockaddr_in	*sin;
int			size;
u_int32		dsthost; /* as in sin_addr.s_addr */
#endif /* #ifdef SHORTENED_RBIND */
/* <<< YDL 94/01/25 */
{
	Socks_t			dst;
	struct sockaddr_in	psin;
	int	i;
	int	new_sock, lport;
	u_int32	remhost;
	int	con_ret;
	struct timeval tmo;
	fd_set fds;

	if (socks_init_done == 0)
		SOCKSinit("SOCKSclient");
	bzero((char *)&psin, sizeof(psin));
	lport = ntohs(sin->sin_port);
	strcpy(socks_cmd, "bind");
#ifdef SHORTENED_RBIND
# ifdef NO_GUESSING_REMHOST
	remhost = 0L;
	strcpy(socks_dst_name, "Unspecified.Host");
# else
	remhost = socks_last_conn_host;
	saddrtoname(&remhost, socks_dst_name, sizeof(socks_dst_name));
# endif /* # ifdef NO_GUESSING_REMHOST */
#else
	remhost = dsthost;
	saddrtoname(&remhost, socks_dst_name, sizeof(socks_dst_name));
#endif /* #ifdef SHORTENED_RBIND */
	porttoserv(socks_last_conn_port, socks_dst_serv, sizeof(socks_dst_serv));
#ifdef DEBUG
fprintf(stderr, "Rbind 100, lport=%u, socks_dst_name=>%s<, socks_dst_serv=>%s<\n", lport, socks_dst_name, socks_dst_serv);
fprintf(stderr, "Rbind(%d, %s, %s)\n", sock, socks_dst_name, socks_dst_serv);
#endif /* #ifdef DEBUG */

	psin.sin_addr.s_addr = remhost;
	psin.sin_port = socks_last_conn_port;
#ifdef VERSATILE_CLIENTS
	direct = check_cconf(&me, &psin);
#ifdef DEBUG
fprintf(stderr, "Rbind() 200, direct = %d\n", direct);
#endif /* #ifdef DEBUG */
	if (direct < 0) {
		syslog(LOG_LOW, "Refused -- bind() from %s(%s) for %s (%s)",
			socks_src_user, socks_real_user, socks_dst_name, socks_dst_serv);
		errno = ECONNREFUSED;
		return -1;
	}

	if (direct == 1) {
		syslog(LOG_LOW, "bind() directly from %s(%s) for %s (%s)",
			socks_src_user, socks_real_user, socks_dst_name, socks_dst_serv);
#ifdef DEBUG
fprintf(stderr,"Rbind() 300, direct=1, lport=%u\n", lport);
#endif /* #ifdef DEBUG */
#if defined(SUPPORT_RCMD)
		if ((lport >= IPPORT_RESERVED) || (lport < IPPORT_RESERVED/2))
			return (bind(sock, (struct sockaddr *)sin, size));
		else
			return 0;
#else /* SUPPORT_RCMD not defined */
		return (bind(sock, (struct sockaddr *)sin, size));
#endif /* #if defined(SUPPORT_RCMD) */
	}
#endif /* #ifdef VERSATILE_CLIENTS */
	
	con_ret = connect_sockd(sock, &lport);
	if (con_ret == 0) {
		;
#ifdef SVR4
	} else if ((errno == EINPROGRESS) || (errno == EAGAIN)) {
#else
	} else if (errno == EINPROGRESS) {
#endif
		while (1) {
			tmo.tv_sec = 0;
			tmo.tv_usec = 100000;
			FD_ZERO(&fds);
			FD_SET(sock, &fds);
			select(sock+1, NULL, &fds, NULL, &tmo);
			con_ret = connect(sock, (struct sockaddr *)&socks_nsin, sizeof(struct sockaddr_in));
			if ((con_ret < 0) && (errno == EISCONN)) {
				con_ret = 0;
				break;
#if defined(SVR4)
			} else if ((con_ret < 0) && ((errno == EALREADY) || 
				(errno == EAGAIN))) {
#else /* !defined(SVR4) */
			} else if ((con_ret < 0) && (errno == EALREADY)) {
#endif /* #if defined(SVR4) */
				continue;
			} else
				break;
		}
	}
	if (con_ret < 0) {
		syslog(LOG_LOW, "Failed -- bind() from %s(%s) for %s (%s)",
			socks_src_user, socks_real_user, socks_dst_name, socks_dst_serv);
		errno = ECONNREFUSED;
		return -1;
	}
	syslog(LOG_LOW, "bind() from %s(%s) for %s (%s) using sockd at %s",
		socks_src_user, socks_real_user, socks_dst_name, socks_dst_serv, socks_server);

	dst.version = SOCKS_VERSION;
	dst.cmd     = SOCKS_BIND;
	dst.port    = socks_last_conn_port;
	dst.host    = remhost;
	if (socksC_proto(sock, &dst) < 0)
		return(-1);
	cursin.sin_family = AF_INET;
	cursin.sin_port = dst.port;
	if (ntohl(dst.host) == INADDR_ANY)
		cursin.sin_addr.s_addr = socks_nsin.sin_addr.s_addr;
	else
		cursin.sin_addr.s_addr = dst.host;
/*
        fprintf(stderr,"Rbind interface: %s, port: %u\n",
                inet_ntoa(cursin.sin_addr), htons(cursin.sin_port));
*/

	return (check_result(dst.cmd));
}

/*
**  Stub routine since the listen will have alread succeded on the
**   server.
*/
Rlisten(s, n)
int	s, n;
{
#ifdef DEBUG
	fprintf(stderr, "direct=%d, Rlisten(%d, %d)\n", direct, s, n);
#endif
#ifdef VERSATILE_CLIENTS
	if (direct)
		return (listen(s, n));
#endif /* #ifdef VERSATILE_CLIENTS */

	return 0;
}

/*
**  Well we know where we got a connection from.
*/
Rgetsockname(sock, sin, size)
int			sock;
struct sockaddr_in	*sin;
int			*size;
{
#ifdef DEBUG
	saddrtoname(&sin->sin_addr, socks_dst_name, sizeof(socks_dst_name));
	porttoserv(sin->sin_port, socks_dst_serv, sizeof(socks_dst_serv));
	fprintf(stderr, "direct= %d, Rgetsockname(%d, %s, %s)\n",
		direct, sock, socks_dst_name, socks_dst_serv);
#endif
#ifdef VERSATILE_CLIENTS
	if (direct)
		return (getsockname(sock, (struct sockaddr *)sin, size));
#endif /* #ifdef VERSATILE_CLIENTS */

	*size = sizeof(struct sockaddr_in);
	*sin = cursin;

	return 0;
}

/*
**  Do an accept, which is really a select for some data on
**    the present socket.
*/
Raccept(sock, sin, size)
int			sock;
struct sockaddr_in	*sin;
int			*size;
{
	fd_set		fds;
	Socks_t		dst;
	int		fdsbits = sock + 1;

#ifdef DEBUG
	fprintf(stderr, "direct= %d, Raccept(%d, sin, size)\n",
		direct, sock);
#endif

#ifdef VERSATILE_CLIENTS
	if (direct)
		return(accept(sock, (struct sockaddr *)sin, size));
#endif /* #ifdef VERSATILE_CLIENTS */

	FD_ZERO(&fds);
	FD_SET(sock, &fds);

	if (select(fdsbits, &fds, NULL, NULL, NULL) > 0)
		if (FD_ISSET(sock, &fds)) {
			GetDst(sock, &dst);
			sin->sin_family = AF_INET;
			sin->sin_port = dst.port;
			sin->sin_addr.s_addr = dst.host;
			return(dup(sock));
		}
	return -1;
}

int Rselect(width, readfds, writefds, exceptfds, timeout)
int width;
fd_set *readfds, *writefds, *exceptfds;
struct timeval *timeout;
{
	int wait_ret, status;

	if(!socks_conn_init ) {
		return(select(width, readfds, writefds, exceptfds, timeout));
	}

	if (readfds != NULL)
		FD_CLR(socks_conn_sock, readfds);
	if (exceptfds != NULL)
		FD_CLR(socks_conn_sock, exceptfds);
	if ((writefds == NULL) || !FD_ISSET(socks_conn_sock, writefds))
		return(select(width, readfds, writefds, exceptfds, timeout));

	wait_ret = waitpid(socks_conn_init, &status, WNOHANG);
	if (wait_ret == 0) {
		FD_CLR(socks_conn_sock, writefds);
		return(select(width, readfds, writefds, exceptfds, timeout));
	} else if (wait_ret == socks_conn_init) {
/*
		socks_conn_init = 0;
		socks_conn_sock = 0;
		socks_conn_port = 0;
		socks_conn_host = 0;
*/
		if (status & 0x00ff) {
			kill(socks_conn_init, SIGKILL);
			socks_conn_init = 0;
			socks_conn_code = SOCKS_FAIL;
		} else {
			status = (status >> 8) & 0x00ff;
			if (status == SOCKS_RESULT) {
				socks_last_conn_host = socks_conn_host;
				socks_last_conn_port = socks_conn_port;
			} 
			socks_conn_init = 0;
			socks_conn_code = status;
		}
	} else {
		kill(socks_conn_init, SIGKILL);
		socks_conn_init = 0;
		socks_conn_code = SOCKS_FAIL;
	}

	return(select(width, readfds, writefds, exceptfds, timeout));
}
