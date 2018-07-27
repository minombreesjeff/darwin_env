#include <sys/types.h>
#include <sys/time.h>
#include "socks.h"
#if defined(DEBUG)
#include <sys/errno.h>
extern int errno;
#endif

#define NULL ((char *)0)

int SendDst(s, dst)
int	s;
Socks_t	*dst;
{
	char c[sizeof(Socks_t)];
	char *p = c;
	int i = sizeof(Socks_t), n, ret;
	fd_set	fds;
	int	fdsbits = s + 1;
	struct	timeval timeout;

	c[0] = dst->version;
	c[1] = dst->cmd;
	bcopy(&dst->port, c+2, sizeof(dst->port));
	bcopy(&dst->host, c+2+sizeof(dst->port), sizeof(dst->host));

	while ( i > 0) {
		FD_ZERO(&fds);
		FD_SET(s, &fds);
		timeout.tv_sec = 15;
		timeout.tv_usec = 0;
		while ((ret = select(fdsbits, NULL, &fds, NULL, &timeout)) < 0) {
/*
			perror("select in SendDst");
			exit(-1);
*/
#if defined(DEBUG)
			if (errno != EINTR)
#endif
			return(-1);
		}
		if (ret == 0)
			continue;
		if((n = write(s, p, i)) <= 0) {
/*
			perror("write in SendDst");
			exit(-1);
*/
			return (-2);
		}
		p += n;
		i -= n;
	}
	return(0);
}

int GetDst(s, dst)
int	s;
Socks_t	*dst;
{
	char	c[sizeof(Socks_t)];
	char	*p = c;
	int	i = sizeof(Socks_t), n, ret;
	fd_set	fds;
	int	fdsbits = s + 1;
	struct	timeval timeout;

	while ( i > 0) {
		FD_ZERO(&fds);
		FD_SET(s, &fds);
		timeout.tv_sec = 15;
		timeout.tv_usec = 0;
		while ((ret = select(fdsbits, &fds, NULL, NULL, &timeout)) < 0) {
/*
			perror("select in GetDst");
			exit(-1);
*/
#if defined(DEBUG)
			if (errno != EINTR)
#endif
			return(-1);
		}
		if (ret == 0)
			continue;
		if((n = read(s, p, i)) <= 0) {
/*
			perror("read in GetDst");
			exit(-1);
*/
			return(-2);
		}
		p += n;
		i -= n;
	}

	dst->version = c[0];
	dst->cmd = c[1];
	bcopy(c+2, &dst->port, sizeof(dst->port));
	bcopy(c+2+sizeof(dst->port), &dst->host, sizeof(dst->host));
	return(0);
}

