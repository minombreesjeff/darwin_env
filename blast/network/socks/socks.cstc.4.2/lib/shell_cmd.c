 /*
  * shell_cmd() takes a shell command template and performs %x substitutions.
  * The result is executed
  * by a /bin/sh child process, with standard input, standard output and
  * standard error connected to /dev/null.
  * 
  * Diagnostics are reported through syslog(3).
  * 
  * Author: Wietse Venema, Eindhoven University of Technology, The Netherlands.
  *
  * Adapted for use with SOCKS by Ying-Da Lee, NEC Systems Lab, CSTC
  * ylee@syl.dl.nec.com
  *
  */

#ifndef lint
static char sccsid[] = "@(#) shell_cmd.c 1.2 92/06/11 22:21:28";
#endif

/* System libraries. */

#include <sys/types.h>
#include <sys/param.h>
#include <netinet/in.h>
#include <stdio.h>
#include <syslog.h>
#include "socks.h"

extern char *strncpy();
extern void closelog();
extern void exit();

/* Forward declarations. */

static void do_child();

/* shell_cmd - expand %<char> sequences and execute shell command */

void    shell_cmd(string, src, dst)
char   *string;
struct sockaddr_in	*src, *dst;
{
    char    cmd[BUFSIZ];
    static char    alpha_num[] = "abcdefghijklmnopqrstuvwxyz\
ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    int     child_pid;
    int     wait_pid;
    int     daemon_pid = getpid();

    percent_x(cmd, sizeof(cmd), string, src, dst, daemon_pid);
    if (strpbrk(cmd, alpha_num) == NULL) {
	syslog(LOG_HIGH, "error -- shell command \"%s\" contains no alphanumeric characters.", cmd);
	return;
    }
    
    /*
     * Most of the work is done within the child process, to minimize the
     * risk of damage to the parent.
     */

    switch (child_pid = fork()) {
    case -1:					/* error */
	syslog(LOG_HIGH, "error -- shell_cmd fork() %m");
	break;
    case 00:					/* child */
	do_child(daemon_pid, cmd);
	/* NOTREACHED */
    default:					/* parent */
	while ((wait_pid = wait((int *) 0)) != -1 && wait_pid != child_pid)
	     /* void */ ;
    }
}

/* do_child - exec command with { stdin, stdout, stderr } to /dev/null */

static void do_child(daemon_pid, command)
int   daemon_pid;
char   *command;
{
    char   *error = 0;
    int     tmp_fd;

    /*
     * Close a bunch of file descriptors. The Ultrix inetd only passes stdin,
     * but other inetd implementations set up stdout as well. Ignore errors.
     */

    closelog();
    for (tmp_fd = 0; tmp_fd < 10; tmp_fd++)
	(void) close(tmp_fd);

    /* Set up new stdin, stdout, stderr, and exec the shell command. */

    if (open("/dev/null", 2) != 0) {
	error = "open /dev/null: %m";
    } else if (dup(0) != 1 || dup(0) != 2) {
	error = "dup: %m";
    } else {
	(void) execl("/bin/sh", "sh", "-c", command, (char *) 0);
	error = "execl /bin/sh: %m";
    }

    /* We can reach the following code only if there was an error. */

#ifdef LOG_DAEMON
    (void) openlog("sockd", LOG_PID, SYSLOG_FAC);
#else
    (void) openlog("sockd", LOG_PID);
#endif
    syslog(LOG_HIGH, "Cannot execute shell command for pid %d", daemon_pid);
    exit(0);
}
