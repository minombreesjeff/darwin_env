dnl
dnl "$Id: cups-mdns.m4,v 1.2 2004/05/31 21:02:20 jlovell Exp $"
dnl
dnl   Multicast DNS (aka Rendezvous) stuff for the Common UNIX Printing System (CUPS).
dnl
dnl   http://www.multicastdns.org/
dnl   http://www.dns-sd.org
dnl   http://developer.apple.com/macosx/rendezvous
dnl
dnl   Copyright 1997-2002 by Easy Software Products, all rights reserved.
dnl
dnl   These coded instructions, statements, and computer programs are the
dnl   property of Easy Software Products and are protected by Federal
dnl   copyright law.  Distribution and use rights are outlined in the file
dnl   "LICENSE.txt" which should have been included with this file.  If this
dnl   file is missing or damaged please contact Easy Software Products
dnl   at:
dnl
dnl       Attn: CUPS Licensing Information
dnl       Easy Software Products
dnl       44141 Airport View Drive, Suite 204
dnl       Hollywood, Maryland 20636-3111 USA
dnl
dnl       Voice: (301) 373-9603
dnl       EMail: cups-info@cups.org
dnl         WWW: http://www.cups.org
dnl

AC_ARG_ENABLE(mdns, [  --enable-mdns            turn on Multicast DNS support, default=yes])
AC_ARG_WITH(mdns-libs, [  --with-mdns-libs        set directory for Multicast DNS library],
    LDFLAGS="-L$withval $LDFLAGS"
    DSOFLAGS="-L$withval $DSOFLAGS",)
AC_ARG_WITH(mdns-includes, [  --with-mdns-includes    set directory for Multicast DNS includes],
    CFLAGS="-I$withval $CFLAGS"
    CXXFLAGS="-I$withval $CXXFLAGS"
    CPPFLAGS="-I$withval $CPPFLAGS",)

MDNSLIBS=""

if test x$enable_mdns != xno; then
    AC_CHECK_HEADER(dns_sd.h, 
            AC_DEFINE(HAVE_MDNS)
	    MDNSLIBS="-framework CoreFoundation")
fi

AC_SUBST(MDNSLIBS)

dnl
dnl End of "$Id: cups-mdns.m4,v 1.2 2004/05/31 21:02:20 jlovell Exp $".
dnl
