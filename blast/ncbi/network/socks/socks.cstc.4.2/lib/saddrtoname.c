#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

char *saddrtoname(addr, name, namelen)
struct	in_addr *addr;
char	*name;
int	namelen;
{
	struct	hostent	*host;

	if ((host = gethostbyaddr((char *)addr, 4, AF_INET)) != (struct hostent *)0)
		strncpy(name, host->h_name, namelen);
	else
		strncpy(name, inet_ntoa(*addr), namelen);
	return(name);
}

