#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

char *porttoserv(sin_port, name, namelen)
int	sin_port; /* port number in network byte order */
char	*name;
int	namelen;
{
	struct	servent	*serv;
	int	port = ntohs(sin_port);

	if ((serv = getservbyport(port, "tcp")) != (struct servent *)0)
		strncpy(name, serv->s_name, namelen);
	else
		sprintf(name, "%u", port);
	return(name);
}

