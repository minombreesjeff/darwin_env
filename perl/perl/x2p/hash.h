/* $RCSfile: hash.h,v $$Revision: 1.5 $$Date: 2003/05/20 22:54:50 $
 *
 *    Copyright (C) 1991, 1992, 1993, 1994, 1995, 1999, 2000,
 *    by Larry Wall and others
 *
 *    You may distribute under the terms of either the GNU General Public
 *    License or the Artistic License, as specified in the README file.
 *
 * $Log: hash.h,v $
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

#define FILLPCT 60		/* don't make greater than 99 */

#ifdef DOINIT
char coeff[] = {
		61,59,53,47,43,41,37,31,29,23,17,13,11,7,3,1,
		61,59,53,47,43,41,37,31,29,23,17,13,11,7,3,1,
		61,59,53,47,43,41,37,31,29,23,17,13,11,7,3,1,
		61,59,53,47,43,41,37,31,29,23,17,13,11,7,3,1,
		61,59,53,47,43,41,37,31,29,23,17,13,11,7,3,1,
		61,59,53,47,43,41,37,31,29,23,17,13,11,7,3,1,
		61,59,53,47,43,41,37,31,29,23,17,13,11,7,3,1,
		61,59,53,47,43,41,37,31,29,23,17,13,11,7,3,1};
#else
extern char coeff[];
#endif

typedef struct hentry HENT;

struct hentry {
    HENT	*hent_next;
    char	*hent_key;
    STR		*hent_val;
    int		hent_hash;
};

struct htbl {
    HENT	**tbl_array;
    int		tbl_max;
    int		tbl_fill;
    int		tbl_riter;	/* current root of iterator */
    HENT	*tbl_eiter;	/* current entry of iterator */
};

bool hdelete (HASH *tb, char *key);
STR * hfetch ( HASH *tb, char *key );
int hiterinit ( HASH *tb );
char * hiterkey ( HENT *entry );
HENT * hiternext ( HASH *tb );
STR * hiterval ( HENT *entry );
HASH * hnew ( void );
void hsplit ( HASH *tb );
bool hstore ( HASH *tb, char *key, STR *val );
