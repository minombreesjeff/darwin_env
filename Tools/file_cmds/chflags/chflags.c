/*-
 * Copyright (c) 1992, 1993, 1994
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
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

#if 0
#ifndef lint
static const char copyright[] =
"@(#) Copyright (c) 1992, 1993, 1994\n\
	The Regents of the University of California.  All rights reserved.\n";
#endif

#ifndef lint
static char sccsid[] = "@(#)chflags.c	8.5 (Berkeley) 4/1/94";
#endif
#endif

#include <sys/cdefs.h>
__FBSDID("$FreeBSD: src/bin/chflags/chflags.c,v 1.23 2005/05/14 23:23:10 dd Exp $");

#include <sys/types.h>
#include <sys/stat.h>

#include <err.h>
#include <errno.h>
#include <fts.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void	usage(void);

int
main(int argc, char *argv[])
{
	FTS *ftsp;
	FTSENT *p;
	u_long clear, set;
	long val;
	int Hflag, Lflag, Rflag, hflag, ch, fts_options, oct, rval;
	char *flags, *ep;
	int (*change_flags)(const char *, u_int);

	Hflag = Lflag = Rflag = hflag = 0;
#ifdef __APPLE__
	while ((ch = getopt(argc, argv, "HLPR")) != -1)
#else /* !__APPLE__ */
	while ((ch = getopt(argc, argv, "HLPRh")) != -1)
#endif /* __APPLE__ */
		switch (ch) {
		case 'H':
			Hflag = 1;
			Lflag = 0;
			break;
		case 'L':
			Lflag = 1;
			Hflag = 0;
			break;
		case 'P':
			Hflag = Lflag = 0;
			break;
		case 'R':
			Rflag = 1;
			break;
#ifndef __APPLE__
		case 'h':
			hflag = 1;
			break;
#endif /* !__APPLE__ */
		case '?':
		default:
			usage();
		}
	argv += optind;
	argc -= optind;

	if (argc < 2)
		usage();

	if (Rflag) {
		fts_options = FTS_PHYSICAL;
		if (hflag)
			errx(1, "the -R and -h options "
			        "may not be specified together");
		if (Hflag)
			fts_options |= FTS_COMFOLLOW;
		if (Lflag) {
			fts_options &= ~FTS_PHYSICAL;
			fts_options |= FTS_LOGICAL;
		}
	} else
		fts_options = FTS_LOGICAL;

	/* XXX: Why don't chflags and lchflags have compatible prototypes? */
#ifndef __APPLE__
	if (hflag)
		change_flags = (int (*)(const char *, u_int))lchflags;
	else
#endif /* !__APPLE__ */
		change_flags = chflags;

	flags = *argv;
	if (*flags >= '0' && *flags <= '7') {
		errno = 0;
		val = strtol(flags, &ep, 8);
		if (val < 0)
			errno = ERANGE;
		if (errno)
                        err(1, "invalid flags: %s", flags);
                if (*ep)
                        errx(1, "invalid flags: %s", flags);
		set = val;
                oct = 1;
	} else {
		if (strtofflags(&flags, &set, &clear))
                        errx(1, "invalid flag: %s", flags);
		clear = ~clear;
		oct = 0;
	}

	if ((ftsp = fts_open(++argv, fts_options , 0)) == NULL)
		err(1, NULL);

	for (rval = 0; (p = fts_read(ftsp)) != NULL;) {
		switch (p->fts_info) {
		case FTS_D:	/* Change it at FTS_DP if we're recursive. */
			if (!Rflag)
				fts_set(ftsp, p, FTS_SKIP);
			continue;
		case FTS_DNR:			/* Warn, chflag, continue. */
			warnx("%s: %s", p->fts_path, strerror(p->fts_errno));
			rval = 1;
			break;
		case FTS_ERR:			/* Warn, continue. */
		case FTS_NS:
			warnx("%s: %s", p->fts_path, strerror(p->fts_errno));
			rval = 1;
			continue;
		case FTS_SL:			/* Ignore. */
		case FTS_SLNONE:
			/*
			 * The only symlinks that end up here are ones that
			 * don't point to anything and ones that we found
			 * doing a physical walk.
			 */
			if (!hflag)
				continue;
			/* FALLTHROUGH */
		default:
			break;
		}
		if (oct) {
			if (!(*change_flags)(p->fts_accpath, set))
				continue;
		} else {
			p->fts_statp->st_flags |= set;
			p->fts_statp->st_flags &= clear;
			if (!(*change_flags)(p->fts_accpath,
				    (u_long)p->fts_statp->st_flags))
				continue;
		}
		warn("%s", p->fts_path);
		rval = 1;
	}
	if (errno)
		err(1, "fts_read");
	exit(rval);
}

void
usage(void)
{
	(void)fprintf(stderr,
#ifdef __APPLE__
	    "usage: chflags [-R [-H | -L | -P]] flags file ...\n");
#else /* !__APPLE__ */
	    "usage: chflags [-h] [-R [-H | -L | -P]] flags file ...\n");
#endif /* __APPLE__ */
	exit(1);
}
