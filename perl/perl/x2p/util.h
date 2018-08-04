/* $RCSfile: util.h,v $$Revision: 1.5 $$Date: 2003/05/20 22:54:50 $
 *
 *    Copyright (C) 1991, 1992, 1993, 1994, 1995, 1996, 1999, 2000,
 *    by Larry Wall and others
 *
 *    You may distribute under the terms of either the GNU General Public
 *    License or the Artistic License, as specified in the README file.
 *
 * $Log: util.h,v $
 * Revision 1.5  2003/05/20 22:54:50  emoy
 * Update to Perl 5.8.1, including thread support and two level namespace.
 * Bug #: 3258028
 * Reviewed by: Jordan Hubbard
 *
 * Revision 1.4.2.1  2003/05/17 07:07:56  emoy
 * Branch PR3258028 - updating to Perl 5.8.1.  Turning on ithread support and
 * two level namespace.  Append prefix, installprefix, and standard paths to
 * darwin.hints file.  Use perl script to strip DSTROOT from Config.pm and
 * .packlist.
 *
 */

/* is the string for makedir a directory name or a filename? */

#define fatal Myfatal

#define MD_DIR 0
#define MD_FILE 1

#ifdef SETUIDGID
    int		eaccess();
#endif

char	*getwd();
int	makedir();

char * cpy2 ( char *to, char *from, int delim );
char * cpytill ( char *to, char *from, int delim );
void growstr ( char **strptr, int *curlen, int newlen );
char * instr ( char *big, char *little );
char * safecpy ( char *to, char *from, int len );
char * savestr ( char *str );
void croak ( char *pat, ... );
void fatal ( char *pat, ... );
void warn  ( char *pat, ... );
int prewalk ( int numit, int level, int node, int *numericptr );

Malloc_t safemalloc (MEM_SIZE nbytes);
Malloc_t safecalloc (MEM_SIZE elements, MEM_SIZE size);
Malloc_t saferealloc (Malloc_t where, MEM_SIZE nbytes);
Free_t   safefree (Malloc_t where);
