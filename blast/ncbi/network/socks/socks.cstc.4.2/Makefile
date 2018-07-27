#DEBUG=-DDEBUG

SHELL=/bin/sh

#SOCKS=-DSOCKS
# or
SOCKS=-Dconnect=Rconnect -Dgetsockname=Rgetsockname -Dlisten=Rlisten -Daccept=Raccept -Drcmd=Rrcmd -Dbind=Rbind -Dselect=Rselect
# If the second definition of SOCKS is used, you MUST also
# define SHORTENED_RBIND

# If your system doesn't have PWD defined, define it here:
#PWD=/projects/insg/socks.cstc.4.2beta
# It should be this current directory.

# If your system has getcwd() but no getwd(), uncomment the next line:
#GETCWD=-DGETCWD

# Define FASCIST if you want ftp (rftp) to log names of all files transferred
#FASCIST=-DFASCIST

# Define RCMD and SUPPORT_RCMD if you want to support Rrcmd, which is required
# for SOCKSified rlogin, rsh, and rcp.
#RCMD=Rrcmd.o
#SUPPORT_RCMD=-DSUPPORT_RCMD

# Define FOR_PS if your system is not SYSV and you want to have the
# command 'ps' show some details of sockd's activity.
#FOR_PS=-DFOR_PS

# Define SHORTENED_RBIND to make Rbind() take exactly the same
# argument list as the regular bind(), i.e., without the additional
# 'remhost' argument.
SHORTENED_RBIND=-DSHORTENED_RBIND

# optimization flag for cc
OPTIMIZE=-g
#OPTIMIZE=-O
# Be careful with the OPTIMIZE flag. SunPro's SC2.0.1, for example, is
# knwon to produce incorrect code when -O is used.

# Define NO_CLIENT_LOG if you don't want SOCKS clients to produce
# log entries of its activities.
#NO_CLIENT_LOG= -Dopenlog= -Dsyslog=

# Define DNS_THROUGH_NIS if your SOCKS client hosts let their NIS
# servers do the DNS loopkup for them. You are in this category if
# your client hosts has no /etc/resolv.conf but can resolve all
# internal and external names.
#DNS_THROUGH_NIS=-DDNS_THROUGH_NIS

# Directory into which to install the man pages
MAN_DEST_DIR = /usr/local/man

# Directory into which the SOCKS server should be installed
SERVER_BIN_DIR = /usr/etc

# Directory into the client programs should be installed
CLIENTS_BIN_DIR = /usr/local/bin

# SunOS 4.1.x should use
CC=cc
#CC=gcc
OTHER_CFLAGS= $(GETCWD) $(FASCIST) $(SHORTENED_RBIND) -DCOMPAT $(MONITOR) $(DEBUG)
RANLIB=ranlib
OS=sun4.1
INSTALL=install
GETPASS=getpass.o
RESOLV_LIB=-lresolv
# ... or 
#RESOLV_LIB=

# IRIX should use
#CC=cc
#RESOLV_LIB=-lsun
#OTHER_CFLAGS=-cckr $(GETCWD) $(FASCIST) $(SHORTENED_RBIND) -DCOMPAT
#RANLIB=/bin/true
#OS=irix4
#INSTALL=bsdinstall
#GETPASS=getpass.o

# Ultrix 4.0 should use
#CC=cc
#OTHER_CFLAGS= $(GETCWD) $(SHORTENED_RBIND) $(FASCIST)
#RANLIB=ranlib
#OS=ultrix4.0
#INSTALL=install
#GETPASS=getpass.o
#RESOLV_LIB=-lresolv

# HP-UX should use
#CC=cc
#OTHER_CFLAGS= $(GETCWD) $(FASCIST) $(SHORTENED_RBIND) -DCOMPAT
#RANLIB=/bin/true
#OS=hpux9
#INSTALL=install
#GETPASS=
#RESOLV_LIB=

# RS/6000 AIX 3.2 should use
#CC=cc
#RESOLV_LIB=-lbsd
#OTHER_CFLAGS=-D_BSD -D_NONSTD_TYPES -D_NO_PROTO -DAIX $(GETCWD) $(FASCIST) $(SHORTENED_RBIND) -DCOMPAT
#RANLIB=ranlib
#OS=aix3.2
#INSTALL=install
#GETPASS=

# PS/2 AIX 1.2 should use
#CC=cc
#RESOLV_LIB=-lbsd
#OTHER_CFLAGS=-D_BSD -D_NONSTD_TYPES -D_NO_PROTO -DAIX $(GETCWD) $(FASCIST) -DCOMPAT -I/usr/local/include -DAIX_PS2
#RANLIB=ranlib
#OS=aix_ps2
#INSTALL=install
#GETPASS=

# SOLARIS should use
#CC=cc
#RESOLV_LIB=-lresolv -lnsl -lsocket
#OTHER_CFLAGS=-DSOLARIS -Dindex=strchr -Drindex=strrchr $(SHORTENED_RBIND) -DUSE_DIRENT $(GETCWD) $(FASCIST) -DCOMPAT
#RANLIB=/bin/true
#OS=solaris2.2
#INSTALL=install
#GETPASS=getpass.o

# Interactive Systems Unix should use
#OTHER_CFLAGS = -DISC $(GETCWD) $(FASCIST) $(SHORTENED_RBIND) -DCOMPAT
#CC=cc
#RANLIB=ranlib
#OS=sun4.1
#INSTALL=install
#GETPASS=
#RESOLV_LIB=-lresolv

# netBSD should use
#OTHER_CFLAGS = $(GETCWD) $(SHORTENED_RBIND) $(FASCIST)
#CC=cc
#RANLIB=ranlib
#OS=netbsd0.9
#INSTALL=install
#GETPASS=
#RESOLV_LIB=

# LINUX should use
#CC=gcc
#RESOLV_LIB=
#OTHER_CFLAGS=-traditional -DLINUX  $(GETCWD) $(FASCIST) $(SHORTENED_RBIND) -DCOMPAT
#OS=linux
#INSTALL=install
#GETPASS=getpass.o

# UnixWare should use
#CC=cc
#RESOLV_LIB=-lresolv -lnsl -lsocket
#OTHER_CFLAGS= -DSOLARIS -Dindex=strchr -Drindex=strrchr -DUSE_DIRENT $(SHORTENED_RBIND) $(GETCWD) $(FASCIST) -DCOMPAT
#RANLIB=/bin/true
#INSTALL=/usr/ucb/install
#OS=UnixWare
#GETPASS=getpass.o

# Alpha OSF should use:
#CC=cc
#RESOLV_LIB=-lresolv
#OS=alphaOSF
#OTHER_CFLAGS= $(GETCWD) $(FASCIST) $(SHORTENED_RBIND) -DCOMPAT
#RANLIB=ranlib
#INSTALL=install
#GETPASS=

# SCO UNIX/ODT should use:
#CC=cc
#OS=sco
#RESOLV_LIB=-lsocket
#OTHER_CFLAGS= $(GETCWD) $(FASCIST) -DSCO -DSVR3 -Dindex=strchr -Drindex=strrc $(SHORTENED_RBIND) -DUSE_DIRENT
#RANLIB=/bin/true
#GETPASS=
#PWD=/test/chris/socks.cstc.4.1

# BSD/386 should use:
#CC=cc
#OS=bsdi
#RESOLV_LIB=
#OTHER_CFLAGS= $(GETCWD) $(SHORTENED_RBIND) $(FASCIST)
#RANLIB=ranlib
#GETPASS=

# NextStep 3.2, SOCKS server and library only
# (William Lewis, wiml@omnigroup.com)
#CC=cc
#OTHER_CFLAGS= $(GETCWD) $(FASCIST) $(SHORTENED_RBIND) -DCOMPAT $(MONITOR) $(DEBUG) -Dstrdup=NXCopyStringBuffer
#RANLIB=ranlib
#GETPASS=getpass.o
#RESOLV_LIB=-lresolv
# ... or 
#RESOLV_LIB=

# >>>---------------- Others:

# Define RESOLV_LIB if your system doesn't search resolver
# library automatically.
# Leave it undefined otherwise.
#RESOLV_LIB=-lresolv
# If your compiler or loader complains about _res_init being
# an undefined symbol, then you must define RESOLV_LIB.

#RANLIB=ranlib
# Systems (e.g., IRIX) that do not need (and thus don't have) ranlib should use
#RANLIB=/bin/true

# Comment out defintion of GETPASS if your system has problems
# compiling it, the version built into your system will then be used.
# The getpass() function in most Unix systems truncates passwords
# after 8 characters; the version provided here does not.
# This only affects ftp with non-anonymous login, and telnet.
# Ftp with anonymous login allows long passwords regardless
# of whether GETPASS is defined or not.
#GETPASS=getpass.o

# The 'install' command is assumed to be the BSD variety (using -m to
# set the file mode). If the default 'install' on your system doesn't
# do that, you have to either specify an alternative one in the line below
# (e.g., /usr/ucb/install) or modify the other Makefile.
#INSTALL= install

# Macro OS is used in making rtelnet. See the list near top of
# rtelnet/telnet/Makefile for available choices.

# Remember to include -Dindex=strchr -Drindex=strrchr in OTHER_CFLAGS if
# you don't have index() and rindex() (Sys-V camp)

# <<<----------------

# The Internet Whois server; used to be nic.ddn.mil.
WHOIS_SERVER= WHOIS_SERVER=-DWHOIS_SERVER\'=\"rs.internic.net\"\'

SOCKS_LIB=$(PWD)/lib/libsocks.a
IDENT_LIB=$(PWD)/libident/libident.a

#==============================================================================

all: LIB LIBIDENT server clients

server: LIB LIBIDENT
	(cd sockd; $(MAKE) CC="$(CC)" RESOLV_LIB="$(RESOLV_LIB)" \
		OPTIMIZE="$(OPTIMIZE)" \
		SOCKS_LIB="$(SOCKS_LIB)" SUPPORT_RCMD="$(SUPPORT_RCMD)" \
		IDENT_LIB="$(IDENT_LIB)" \
		OTHER_CFLAGS="$(OTHER_CFLAGS) $(FOR_PS)")

clients: RFINGER RFTP RTELNET

LIB:
	(cd lib; $(MAKE) CC="$(CC)" GETPASS="$(GETPASS)" \
		OPTIMIZE="$(OPTIMIZE)" \
		RCMD="$(RCMD)" SUPPORT_RCMD="$(SUPPORT_RCMD)" \
		DNS_THROUGH_NIS="$(DNS_THROUGH_NIS)" \
		OTHER_CFLAGS="$(OTHER_CFLAGS) $(NO_CLIENT_LOG)" \
		RANLIB="$(RANLIB)")

LIBIDENT:
	(cd libident; $(MAKE) CC="$(CC)" OTHER_CFLAGS="$(OTHER_CFLAGS)" \
		OPTIMIZE="$(OPTIMIZE)" RANLIB="$(RANLIB)")

# This also build rwhois
RFINGER: LIB
	(cd rfinger; $(MAKE) CC="$(CC)" $(WHOIS_SERVER) \
		OPTIMIZE="$(OPTIMIZE)" SOCKS="$(SOCKS)" \
		RESOLV_LIB="$(RESOLV_LIB)" SOCKS_LIB="$(SOCKS_LIB)" \
		OTHER_CFLAGS="$(OTHER_CFLAGS) $(NO_CLIENT_LOG)")

RTELNET: LIB
	(cd rtelnet; $(MAKE) CC="$(CC)" OS="$(OS)" SOCKS_LIB="$(SOCKS_LIB)" \
		OPTIMIZE="$(OPTIMIZE)" SOCKS="$(SOCKS)" \
		RESOLV_LIB="$(RESOLV_LIB)" \
		OTHER_CFLAGS="$(OTHER_CFLAGS) $(NO_CLIENT_LOG)")

RFTP: LIB
	(cd rftp; $(MAKE) CC="$(CC)" SOCKS_LIB="$(SOCKS_LIB)" \
		OPTIMIZE="$(OPTIMIZE)" SOCKS="$(SOCKS)" \
		RESOLV_LIB="$(RESOLV_LIB)" \
		OTHER_CFLAGS="$(OTHER_CFLAGS) $(NO_CLIENT_LOG)")

install.server:
	(cd sockd; $(MAKE) INSTALL="$(INSTALL)" MAN_DEST_DIR="$(MAN_DEST_DIR)" \
		CC="$(CC)" RESOLV_LIB="$(RESOLV_LIB)" \
		OPTIMIZE="$(OPTIMIZE)" \
		SOCKS_LIB="$(SOCKS_LIB)" SUPPORT_RCMD="$(SUPPORT_RCMD)" \
		IDENT_LIB="$(IDENT_LIB)" \
		OTHER_CFLAGS="$(OTHER_CFLAGS) $(FOR_PS)" \
		SERVER_BIN_DIR="$(SERVER_BIN_DIR)" install install.man)

install.clients: install.man
	for i in rfinger rftp rtelnet; do \
		(cd $$i ; $(MAKE) INSTALL="$(INSTALL)" \
			CC="$(CC)" OS="$(OS)" SOCKS_LIB="$(SOCKS_LIB)" \
			OPTIMIZE="$(OPTIMIZE)" SOCKS="$(SOCKS)" \
			RCMD="$(RCMD)" SUPPORT_RCMD="$(SUPPORT_RCMD)" \
			DNS_THROUGH_NIS="$(DNS_THROUGH_NIS)" \
			RESOLV_LIB="$(RESOLV_LIB)" \
			OTHER_CFLAGS="$(OTHER_CFLAGS) $(NO_CLIENT_LOG)" \
			CLIENTS_BIN_DIR="$(CLIENTS_BIN_DIR)" \
			install) done
install.man:
	(cd doc; $(MAKE) INSTALL="$(INSTALL)" MAN_DEST_DIR="$(MAN_DEST_DIR)" \
		install)

clean:
	for i in lib libident sockd rfinger rftp rtelnet ; do \
		( cd $$i ; $(MAKE) clean) done


