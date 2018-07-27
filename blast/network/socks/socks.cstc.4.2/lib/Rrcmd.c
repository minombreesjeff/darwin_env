/*
 * Copyright (c) 1983 Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#if defined(LIBC_SCCS) && !defined(lint)
/*static char *sccsid = "from: @(#)rcmd.c	5.24 (Berkeley) 2/24/91";*/
static char *rcsid = "$Id: rcmd.c,v 1.4 1993/12/05 14:42:26 deraadt Exp $";
#endif /* LIBC_SCCS and not lint */

#include <sys/param.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <fcntl.h>
#include <netdb.h>
#include <pwd.h>
#include <errno.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>

Rrcmd(ahost, rport, locuser, remuser, cmd, fd2p)
	char **ahost;
	u_short rport;
/*
	const char *locuser, *remuser, *cmd;
*/
	char *locuser, *remuser, *cmd;
	int *fd2p;
{
	int s, timo = 1, pid;
	long oldmask;
	struct sockaddr_in sin, from;
	char c;
	int lport = IPPORT_RESERVED - 1;
	struct hostent hoste;
	struct hostent *hp;
	fd_set reads;

	pid = getpid();
	hp = gethostbyname(*ahost);
	if (hp == 0) {
		herror(*ahost);
		return (-1);
	}
	bcopy(hp, &hoste, sizeof(struct hostent));
	hp = &hoste;
	*ahost = hp->h_name;
	oldmask = sigblock(sigmask(SIGURG));
	for (;;) {
#ifdef DEBUG
fprintf(stderr, "Rrcmd 100, lport = %d\n", lport);
#endif /* #ifdef DEBUG */
		s = rresvport(&lport);
#ifdef DEBUG
fprintf(stderr, "Rrcmd 200, s = %d\n", s);
#endif /* #ifdef DEBUG */
		if (s < 0) {
			if (errno == EAGAIN)
				fprintf(stderr, "socket: All ports in use\n");
			else
				perror("rcmd: socket");
			sigsetmask(oldmask);
			return (-1);
		}
#ifdef DEBUG
fprintf(stderr, "Rrcmd 300\n");
#endif /* #ifdef DEBUG */
#if defined(hpux)
		ioctl(s, FIOSSAIOOWN, pid);
#else /* hpux not defined */
		fcntl(s, F_SETOWN, pid);
#endif /* #if defined(hpux) */
		bzero((char *)&sin, sizeof sin);
		sin.sin_family = hp->h_addrtype;
		bcopy(hp->h_addr_list[0], (caddr_t)&sin.sin_addr, hp->h_length);
		sin.sin_port = rport;
#ifdef DEBUG
fprintf(stderr, "Rrcmd 400, sin.sin_family=%d, sin.sin_port=%u, sin.sin_addr=%s\n",sin.sin_family, ntohs(sin.sin_port), inet_ntoa(sin.sin_addr));
#endif /* #ifdef DEBUG */
		if (Rconnect(s, (struct sockaddr *)&sin, sizeof(sin)) >= 0)
			break;
#ifdef DEBUG
fprintf(stderr, "Rrcmd 500, Rconnect failed\n");
#endif /* #ifdef DEBUG */
		(void) close(s);
		if (errno == EADDRINUSE) {
			lport--;
			continue;
		}
		if (errno == ECONNREFUSED && timo <= 16) {
			sleep(timo);
			timo *= 2;
			continue;
		}
		if (hp->h_addr_list[1] != NULL) {
			int oerrno = errno;

			fprintf(stderr,
			    "connect to address %s: ", inet_ntoa(sin.sin_addr));
			errno = oerrno;
			perror(0);
			hp->h_addr_list++;
			bcopy(hp->h_addr_list[0], (caddr_t)&sin.sin_addr,
			    hp->h_length);
			fprintf(stderr, "Trying %s...\n",
				inet_ntoa(sin.sin_addr));
			continue;
		}
		perror(hp->h_name);
		sigsetmask(oldmask);
		return (-1);
	}
	lport--;
#ifdef DEBUG
fprintf(stderr, "Rrcmd 600, f2dp=%u\n", fd2p);
#endif /* #ifdef DEBUG */
	if (fd2p == 0) {
		write(s, "", 1);
		lport = 0;
	} else {
		char num[8];
		int s2 = rresvport(&lport), s3;
		int len = sizeof (from);
		struct sockaddr_in tsin;
		int tlen = sizeof(tsin);

#ifdef DEBUG
fprintf(stderr, "Rrcmd 650, lport=%u\n", lport);
#endif /* #ifdef DEBUG */
		if (s2 < 0)
			goto bad;
		tsin.sin_family = AF_INET;
		tsin.sin_addr.s_addr = INADDR_ANY;
		tsin.sin_port = htons((u_short)lport);
/*
		s2 = socket(AF_INET, SOCK_STREAM, 0);
*/
		if (Rbind(s2, (struct sockaddr *)&tsin, sizeof(tsin),
			sin.sin_addr.s_addr) < 0) {
			perror("Rrcmd: Rbind() ");
			(void) close(s2);
			goto bad;
		}
		if (Rgetsockname(s2, (struct sockaddr *)&tsin, &tlen) < 0) {
			perror("Rrcmd: Rgetsockname() ");
			(void) close(s2);
			goto bad;
		}
		Rlisten(s2, 1);
/*
		(void) sprintf(num, "%d", lport);
*/
		(void) sprintf(num, "%u", ntohs(tsin.sin_port));
#ifdef DEBUG
fprintf(stderr,"Rrcmd 800, num=>%s<\n", num);
#endif /* #ifdef DEBUG */
		if (write(s, num, strlen(num)+1) != strlen(num)+1) {
			perror("write: setting up stderr");
			(void) close(s2);
			goto bad;
		}
		FD_ZERO(&reads);
		FD_SET(s, &reads);
		FD_SET(s2, &reads);
		errno = 0;
		if (select(32, &reads, 0, 0, 0) < 1 ||
		    !FD_ISSET(s2, &reads)) {
			if (errno != 0)
				perror("select: setting up stderr");
			else
			    fprintf(stderr,
				"select: protocol failure in circuit setup.\n");
			(void) close(s2);
			goto bad;
		}
#ifdef DEBUG
fprintf(stderr,"Rrcmd 900, before Raccept()\n");
#endif /* #ifdef DEBUG */
		s3 = Raccept(s2, (struct sockaddr *)&from, &len);
		(void) close(s2);
		if (s3 < 0) {
			perror("accept");
			lport = 0;
			goto bad;
		}
		*fd2p = s3;
		from.sin_port = ntohs((u_short)from.sin_port);
		if (from.sin_family != AF_INET ||
		    from.sin_port >= IPPORT_RESERVED ||
		    from.sin_port < IPPORT_RESERVED / 2) {
			fprintf(stderr,
			    "socket: protocol failure in circuit setup.\n");
			goto bad2;
		}
	}
	(void) write(s, locuser, strlen(locuser)+1);
	(void) write(s, remuser, strlen(remuser)+1);
	(void) write(s, cmd, strlen(cmd)+1);
	if (read(s, &c, 1) != 1) {
		perror(*ahost);
		goto bad2;
	}
	if (c != 0) {
		while (read(s, &c, 1) == 1) {
			(void) write(2, &c, 1);
			if (c == '\n')
				break;
		}
		goto bad2;
	}
	sigsetmask(oldmask);
	return (s);
bad2:
	if (lport)
		(void) close(*fd2p);
bad:
	(void) close(s);
	sigsetmask(oldmask);
	return (-1);
}

