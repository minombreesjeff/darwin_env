#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <sys/file.h>
#include <errno.h>
#include "socks.h"

#if defined(SOCKS)
#define connect	Rconnect
#endif

extern int	errno;
extern char	*strrchr();
char		*Progname;
extern char	*getenv();

main(argc, argv)
int	argc;
char	**argv;
{
	char			buf[1024];
	unsigned long		l;
	struct sockaddr_in	sin;
	struct hostent		*hp;
	struct servent		*sp;
	extern int		optind;
	extern char		*optarg;
	int			c, v, n, i;
	int			s;
	extern int		optind;
	FILE			*in;
	char			*cp;
#ifdef WHOIS_SERVER
	char			*service = "whois", *host = WHOIS_SERVER;
#else
	char			*service = "finger", *host = "localhost";
#endif
	struct in_addr	dstaddr[21];

	if ((Progname = strrchr(argv[0], '/')) == NULL)
		Progname = argv[0];
	else
		Progname++;

#if defined(SOCKS) || defined(connect)
	SOCKSinit(Progname);
#endif

#ifdef WHOIS_SERVER
	while ((i = getopt(argc,argv,"h:")) != EOF) {
		switch (i) {
		case 'h':
			host = optarg;
			break;
		default:
			fprintf(stderr,"Usage: %s [-h host] name\n", Progname);
			exit(1);
		}
	}
#endif

	buf[0] = '\0';
#if defined(LINUX)
	for (i = 1; i < argc; i++) 
#else
	for (i = optind; i < argc; i++) 
#endif
		strcat(buf, argv[i]);
	
#ifndef WHOIS_SERVER
	if ((cp = strrchr(buf, '@')) != NULL) {
		*cp = '\0';
		host = cp + 1;
	} else {
		if ((cp = getenv("ORIG_FINGER")) == NULL)
			cp = ORIG_FINGER;
		execvp(cp, argv);
		fprintf(stderr, "Unable to run %s\n", cp);
		exit(1);
	}
#endif

	sin.sin_family = AF_INET;


	if ((sp = getservbyname(service, "tcp")) == NULL) {
		if ((v = atoi(service)) == 0) {
			fprintf(stderr, "%s: unknown service '%s'\n",
				Progname, service);
			exit(1);
		}
		sin.sin_port = htons((short) v);
	} else {
		sin.sin_port = sp->s_port;
	}

	if ((hp = gethostbyname(host)) == NULL) {
		if ((l = inet_addr(host)) == -1) {
			fprintf(stderr, "%s: unknown host '%s'\n",
				Progname, host);
			exit(1);
		}
/* you are out of luck if your system doesn't use 2's complement in
   integer representation */
		sin.sin_addr.s_addr = l ;
	} else {
		for (i = 0; (i < 20) && *hp->h_addr_list;
			   i++, hp->h_addr_list++)
			bcopy(*hp->h_addr_list, &(dstaddr[i].s_addr), hp->h_length);
		dstaddr[i].s_addr = 0;	
		i = 0;
		sin.sin_addr.s_addr = dstaddr[i++].s_addr;
	}

	if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		fprintf(stderr, "%s: ", Progname);
		perror("socket");
		exit(1);
	}

	while (connect(s, &sin, sizeof(sin)) < 0) {
		if ((errno == ETIMEDOUT) && (hp != NULL) && 
		    ((sin.sin_addr.s_addr = dstaddr[i++].s_addr) != 0)) {
			continue;
		}
		fprintf(stderr, "%s: ", Progname);
		perror("connect");
		exit(1);
	}

	strcat(buf, "\r\n");

	if (write(s,buf,strlen(buf)) < 0) {
		fprintf(stderr, "%s: ", Progname);
		perror("write");
	}

	while ((n = read(s, buf, sizeof(buf) - 1)) > 0) {
		write(1, buf, n);
	}

	close(s);
}
