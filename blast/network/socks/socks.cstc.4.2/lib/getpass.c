/*
 * Copyright (c) 1988 The Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation,
 * advertising materials, and other materials related to such
 * distribution and use acknowledge that the software was developed
 * by the University of California, Berkeley.  The name of the
 * University may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "@(#)getpass.c	based on 5.3 (Berkeley) 9/22/88";
#endif /* LIBC_SCCS and not lint */

#if defined(__NetBSD__)
#define USE_OLD_TTY
#endif

/* >>> Craig Metz */
#if defined(LINUX)
#define SVR3
#endif
/* <<< Craig Metz */

#if defined(SOLARIS) || defined(hpux)
#include <signal.h>
#include <sgtty.h>
#include <sys/ioctl.h>
#else
#ifdef SVR3
#include <termio.h>
#else
#include <sys/ioctl.h>
#endif
#include <sys/signal.h>
#endif
#include <stdio.h>

char *
getpass(prompt)
	char *prompt;
{
#ifdef SVR3
	struct termio term_struct;
	tcflag_t svflagval;
#else
	struct sgttyb ttyb;
	int svflagval;
#endif
	register int ch;
	register char *p;
	FILE *fp, *outfp;
#ifdef SOLARIS
	sigset_t maskset;
#else
	long omask;
#endif
#define	PASSWD_LEN	128
	static char buf[PASSWD_LEN + 1];

	/*
	 * read and write to /dev/tty if possible; else read from
	 * stdin and write to stderr.
	 */
	if ((outfp = fp = fopen("/dev/tty", "w+")) == NULL) {
		outfp = stderr;
		fp = stdin;
	}
#ifdef SVR3
	(void)ioctl(fileno(fp), TCGETA, &term_struct);
	svflagval = term_struct.c_lflag;
	term_struct.c_lflag &= ~ECHO;
#else
	(void)ioctl(fileno(fp), TIOCGETP, &ttyb);
	svflagval = ttyb.sg_flags;
	ttyb.sg_flags &= ~ECHO;
#endif

#ifdef SOLARIS
	if (sigprocmask(0, (sigset_t *)0, &maskset) || sighold(SIGINT)) {
		perror("Can't block SIGINT in getpass() ");
		exit(1);
	}
#else
	omask = sigblock(sigmask(SIGINT));
#endif

#ifdef SVR3
	(void)ioctl(fileno(fp), TCSETA, &term_struct);
#else
	(void)ioctl(fileno(fp), TIOCSETP, &ttyb);
#endif

	fputs(prompt, outfp);
	rewind(outfp);			/* implied flush */
	for (p = buf; (ch = getc(fp)) != EOF && ch != '\n';)
		if (p < buf + PASSWD_LEN)
			*p++ = ch;
	*p = '\0';
	(void)write(fileno(outfp), "\n", 1);
#ifdef SVR3
	term_struct.c_lflag = svflagval;
	(void)ioctl(fileno(fp), TCSETA, &term_struct);
#else
	ttyb.sg_flags = svflagval;
	(void)ioctl(fileno(fp), TIOCSETP, &ttyb);
#endif

#ifdef SOLARIS
	if (sigprocmask(SIG_SETMASK, &maskset, (sigset_t *)0)) {
		perror("Can't restore signal mask in getpass() ");
		exit(1);
	}
#else
	(void)sigsetmask(omask);
#endif
	if (fp != stdin)
		(void)fclose(fp);
	return(buf);
}

