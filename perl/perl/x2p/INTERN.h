/* $RCSfile: INTERN.h,v $$Revision: 1.5 $$Date: 2003/05/20 22:54:50 $
 *
 *    Copyright (C) 1993, 1994, by Larry Wall and others
 *
 *    You may distribute under the terms of either the GNU General Public
 *    License or the Artistic License, as specified in the README file.
 *
 * $Log: INTERN.h,v $
 * Revision 1.5  2003/05/20 22:54:50  emoy
 * Update to Perl 5.8.1, including thread support and two level namespace.
 * Bug #: 3258028
 * Reviewed by: Jordan Hubbard
 *
 * Revision 1.4.2.1  2003/05/17 07:07:55  emoy
 * Branch PR3258028 - updating to Perl 5.8.1.  Turning on ithread support and
 * two level namespace.  Append prefix, installprefix, and standard paths to
 * darwin.hints file.  Use perl script to strip DSTROOT from Config.pm and
 * .packlist.
 *
 */

#undef EXT
#define EXT

#undef INIT
#define INIT(x) = x

#define DOINIT
