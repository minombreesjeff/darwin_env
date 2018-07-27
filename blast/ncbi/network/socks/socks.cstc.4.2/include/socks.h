/*
 * Default SOCKS server host; you MUST set this for your site.
 * This is overridden at run time by the contents of environment
 * variable SOCKS_SERVER if it exists.
 */
#define SOCKS_DEFAULT_SERVER	"SOCKS.server.for.your.site"

/*
 * Default Domain Nameserver for the SOCKS clients.
 * Leave it undefined if all your client mechines can do general
 * DNS lookup for all Internet hosts correctly with the DNS servers
 * specified in /etc/resolv.conf.
 * Otherwise, define it using the IP ADDRESS (NOT NAME!) of a DNS
 * server which can resolve all Internet hosts and which is IP-reachable
 * from your client machines.
 * This is overriden at run time by the contents of environment
 * variable SOCKS_NS if it exists.
 */
/* #define SOCKS_DEFAULT_NS	"1.2.3.4" */

/* >>> jon r. luini <jonl@hal.com> */
/*
 * Default domain name to use for the resolver to use.
 * Leave it undefined unless you run in an environment where
 * you have a number of clients which will be running the socks
 * utilities without the correct domain name specified in /etc/resolv.conf
 * (or determined automatically by the system). If you try to run
 * socks and it complains that it cannot lookup the local hostname,
 * that is a good indication you need to define this appropriately.
 * This is overriden at run time by the contents of environment
 * variable SOCKS_DNAME if it exists.
 */
/* #define       SOCKS_DEFAULT_DNAME     "hal.COM" */
/* <<< jon r. luini <jonl@hal.com> */

/*
 * Make clients that connect through SOCKS server to destinations
 * outside the firewall, but connect directly to destinations inside
 * the firewall. You can then, for example, rename the regular 'telnet'
 * to 'telnet.orig' and make a symbolic link for 'telnet' to point
 * to 'rtelnet' and from then on use the command 'telnet' no matter
 * whether the hosts you try to connect is inside or outside the
 * firewall.
 */
#define VERSATILE_CLIENTS

/*
 * Full pathname of the regular 'finger' program.
 * If you are making versatile clients, you will have to rename your
 * regular 'finger' program to something else, e.g., from /usr/ucb/finger
 * to /usr/ucb/finger.orig and the pathname you should use here is the
 * new (altered) pathname, i.e., /usr/ucb/finger.orig.
 */
#define ORIG_FINGER	"/usr/ucb/finger.orig"
/* Overridden at runtime by environment variable ORIG_FINGER if it exists. */

/* Control file for versatile clients */
#define SOCKS_CONF	"/etc/socks.conf"

/*
 * Default port number for SOCKS services.
 * On the SOCKS server host, if the server is under inetd control,
 * then the port must be specified in socks/tcp entry in /etc/services.
 * For servers not under inetd control and for all clients,
 * the port number is obtained from socks/tcp entry in /etc/services if
 * it exists, otherwise the number defined by SOCKS_DEF_PORT will be used.
 */
#define SOCKS_DEF_PORT	1080

/*
**  How long (in seconds) to keep a connection around while it is idle
*/
#define SOCKS_TIMEOUT	2*60*60	/* 2hr in seconds */

/* How long before connection attempts timed out */
#define CLIENT_CONN_TIMEOUT 60*2 /* 2 minutes */
#define SOCKD_CONN_TIMEOUT 60*3 /* 3 minutes */
/* You may have to adjust these to fit your network situation */

/*
 * Where the config file lives on the SOCKS server host.
 * This is the file that controls access to the SOCKS server
 * and its services.
 */
#define SOCKD_CONF	"/etc/sockd.conf"

/*
 * Define this if your SOCKS server is multi-homed (i.e.,
 * having two or more network interfaces) and is not behaving
 * as a router (i.e., has its IP forwarding turned off).
 * Leave it undefined otherwise.
 */
/* #define MULTIHOMED_SERVER */

/*
 * For multi-homed servers, you must supply the file /etc/sockd.route
 * to tell the program which interface to use for communicating with
 * which destination networks/hosts. See sockd man pages for details.
 * This has no effects if MULTIHOMED_SERVER is undefined.
 */
#define SOCKD_ROUTE_FILE "/etc/sockd.route"

/* Current SOCKS protocol version */
#define SOCKS_VERSION	4

#define CSTC_RELEASE	"4.2 pre1"

/*
**  Response commands/codes
*/
#define SOCKS_CONNECT	1
#define SOCKS_BIND	2
#define SOCKS_RESULT	90
#define SOCKS_FAIL	91
#define SOCKS_NO_IDENTD	92 /* Failed to connect to Identd on client machine */
#define SOCKS_BAD_ID	93 /* Client's Identd reported a different user-id */
  
#if defined(__alpha)
typedef unsigned int u_int32;
#else
typedef unsigned long u_int32;
#endif

typedef struct {
	u_int32			host; /* in network byte order */
	unsigned short		port; /* in network byte oreder */
	unsigned char		version;
	unsigned char		cmd;
} Socks_t;

/*
 * Define NOT_THROUGH_INETD if you want a standalone SOCKS server,
 * one which is not under the control of inetd.
 * This is not recommended.
 */
/* #define NOT_THROUGH_INETD */

/*
 * Maximum number of concurrent clients a SOCKS server will support.
 * Meaningful only if the server is not under the control
 * of inetd, i.e., when NOT_THROUGH_INETD is defined.
 */
#define MAX_CLIENTS	5

#ifdef SOLARIS
/* for bcopy(), bzero() and bcmp() */
#include "bstring.h"
#endif

/* Define NO_SYSLOG to suppress logging */
/*
#define NO_SYSLOG

#if defined(NO_SYSLOG)
# define syslog
# define openlog
#endif
*/

# define SYSLOG_FAC	LOG_DAEMON
# define LOG_LOW	LOG_NOTICE
# define LOG_HIGH	LOG_ERR
