#CC=gcc

# Define RCMD and SUPPORT_RCMD if you want to use the library
# for rlogin, rsh, and rcp.
SUPPORT_RCMD=-DSUPPORT_RCMD
RCMD=Rrcmd.o

# Comment out next macro to produce library compatible with
# clients SOCKSified for SOCKS.CSTC 4.1 and 4.0
SHORTENED_RBIND=-DSHORTENED_RBIND

# You should not have to define the next macro
#NO_GUESSING_REMHOST=-DNO_GUESSING_REMHOST

# Directory into which socks_clients's man page files will be installed:
MAN_DEST_DIR=/usr/local/man

# If you want to install the SOCKS library into /usr/locallib,
# uncomment the next line:
#LIB_DEST_DIR=/usr/locallib
# Installing the library in /usr/locallib makes it possible to
# use '-lsocks' with cc or ld command to search the library.
# /usr/lib and /lib may also be used instead of /usr/local/lib.

# Remember to include -Dindex=strchr in OTHER_CFLAGS if
# you don't have index() (Sys-V camp)

# SunOS should use
OTHER_CFLAGS= $(SHORTENED_RBIND)
#RANLIB=ranlib

# IRIX should use
#OTHER_CFLAGS=-cckr $(SHORTENED_RBIND)
#RANLIB=/bin/true

# AIX should use
#OTHER_CFLAGS=-D_BSD -D_NONSTD_TYPES -D_NO_PROTO -DAIX $(SHORTENED_RBIND)
#RANLIB=ranlib

#For SOLARIS
#OTHER_CFLAGS=-DSOLARIS -Dindex=strchr -Drindex=strrchr -DUSE_DIRENT $(SHORTENED_RBIND)
#RANLIB=/bin/true

# Interactive Systems Unix should use
# OTHER_CFLAGS = -DISC $(SHORTENED_RBIND)

# LINUX should use
#CC=gcc
#RESOLV_LIB=
#OTHER_CFLAGS=-traditional -DLINUX  $(SHORTENED_RBIND)

# UnixWare should use
#RESOLV_LIB=-lresolv -lnsl -lsocket
#OTHER_CFLAGS= -DSOLARIS -Dindex=strchr -Drindex=strrchr -DUSE_DIRENT $(SHORTENED_RBIND)
#RANLIB=/bin/true
#INSTALL=bsdinstall

# >>>---------------- Others:

RANLIB=ranlib
# Systems that do not need (and thus don't have) ranlib should use
#RANLIB=/bin/true

# <<<----------------

OPTIMIZE=-g
CFLAGS = -I../include ${OPTIMIZE} ${OTHER_CFLAGS} ${SUPPORT_RCMD} ${NO_GUESSING_REMHOST}

# Comment out defintion of GETPASS if your system has problem
# compiling it. The version built into your system will be used.
# The getpass() function in most Unix systems truncate password
# after 8 characters; the version provided here does not.
# This only affects telnet and ftp with non-anonymous login.
# Ftp with anonymous login allows long passwords regardless
# of whether GETPASS is defined or not.
GETPASS=getpass.o

# The 'install' command is assumed to be the BSD variety (using -m to
# set the file mode). If the default 'install' on your system doesn't
# do that, you have to either specify an alternative one in the line below
# (e.g., bsdinstall or /usr/ucb/install) or modify the install instructions.
INSTALL= install

#==============================================================================

OBJS = Rconnect.o SendGetDst.o saddrtoname.o porttoserv.o check_cconf.o \
	percent_x.o shell_cmd.o check_user.o $(GETPASS) $(RCMD)
SRCS = Rconnect.c SendGetDst.c saddrtoname.c porttoserv.c check_cconf.c \
	percent_x.c shell_cmd.c check_user.c getpass.c
INC =../include/socks.h
LIB = libsocks.a

all: echocwd $(LIB)

$(LIB): $(OBJS) $(INC)
	rm -f $(LIB)
	ar rc $(LIB) $(OBJS)
	$(RANLIB) $(LIB)

install: echocwd $(LIB)
	-if [ -d $(LIB_DEST_DIR) ]; then \
	  ($(INSTALL) -m 644 $(LIB) $(LIB_DEST_DIR); \
	   $(RANLIB) -t $(LIB_DEST_DIR)/$(LIB)); fi

install.man: echocwd
	$(INSTALL) -m 444 ../doc/socks_clients.1 $(MAN_DEST_DIR)/man1
	$(INSTALL) -m 444 ../doc/socks.conf.5 $(MAN_DEST_DIR)/man5

clean: echocwd
	rm -f $(OBJS) $(LIB) core

echocwd:
	@pwd
