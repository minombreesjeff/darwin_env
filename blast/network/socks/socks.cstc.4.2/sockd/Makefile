#CC=gcc

SOCKS_LIB=../lib/libsocks.a
IDENT_LIB=../libident/libident.a

# Directory into which sockd and test_sockd_conf programs
# will be installed:
SERVER_BIN_DIR=/usr/etc

# Directory into which the man page files of sockd and test_sockd_conf
# will be installed:
MAN_DEST_DIR=/usr/local/man

INC=../include/socks.h

# Uncomment next line to support Rbind() without the extra 'remhost' arg.
SHORTENED_RBIND=-DSHORTENED_RBIND

# Remember to include -Dindex=strchr in OTHER_CFLAGS if
# you don't have index() (Sys-V camp)

# Define FOR_PS if your system is not SYSV and you want to have the
# command 'ps' show some details of sockd's activity.
FOR_PS=-DFOR_PS

# SunOS should use
#RESOLV_LIB=-lresolv
OTHER_CFLAGS=$(SHORTENED_RBIND)

# IRIX should use
#OTHER_CFLAGS=-cckr $(SHORTENED_RBIND)
#RESOLV_LIB=-lsun

# HPUX should use
#OTHER_CFLAGS=-Ac $(SHORTENED_RBIND)

# AIX should use
#RESOLV_LIB=-lbsd
#OTHER_CFLAGS=-D_BSD -D_NONSTD_TYPES -D_NO_PROTO -DAIX $(SHORTENED_RBIND)

# SOLARIS should use
#RESOLV_LIB=-lresolv -lnsl -lsocket
#OTHER_CFLAGS=-DSOLARIS -Dindex=strchr -Drindex=strrchr -DUSE_DIRENT  $(SHORTENED_RBIND)

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

# define SUPPORT_RCMD to build a server that can handle SOCKSified
# rlogin, rsh, and rcp.
SUPPORT_RCMD=-DSUPPORT_RCMD

OPTIMIZE=-g
CFLAGS = -I../include -I../libident ${OPTIMIZE} ${OTHER_CFLAGS} ${SUPPORT_RCMD}

# The 'install' command is assumed to be the BSD variety (using -m to
# set the file mode). If the default 'install' on your system doesn't
# do that, you have to either specify an alternative one in the line below
# (e.g., /usr/ucb/install) or modify the install instructions.
INSTALL= install

#==================================================

SRC=sockd.c
OBJ=sockd.o
PROG=sockd test_sockd_conf flip_cfmasks

all: echocwd $(PROG)

sockd: $(SRC) $(INC) $(SOCKS_LIB) $(IDENT_LIB)
	$(CC) $(CFLAGS) -o $@ $(SRC) $(SOCKS_LIB) $(IDENT_LIB) $(RESOLV_LIB)

test_sockd_conf: $(SRC) $(INC) $(SOCKS_LIB)
	$(CC) $(CFLAGS) -DTEST -o $@ $(SRC) $(SOCKS_LIB) $(RESOLV_LIB)

flip_cfmasks: flip_cfmasks.c $(INC) $(SOCKS_LIB)
	$(CC) $(CFLAGS) -o $@ flip_cfmasks.c $(SOCKS_LIB) $(RESOLV_LIB)

install: echocwd sockd test_sockd_conf
	$(INSTALL) -m 111 sockd $(SERVER_BIN_DIR)
	$(INSTALL) -m 111 test_sockd_conf $(SERVER_BIN_DIR)
	$(INSTALL) -m 111 flip_cfmasks $(SERVER_BIN_DIR)

install.man: echocwd
	$(INSTALL) -m 444 ../doc/sockd.8 $(MAN_DEST_DIR)/man8
	$(INSTALL) -m 444 ../doc/test_sockd_conf.8 $(MAN_DEST_DIR)/man8
	$(INSTALL) -m 444 ../doc/sockd.conf.5 $(MAN_DEST_DIR)/man5
	$(INSTALL) -m 444 ../doc/sockd.route.5 $(MAN_DEST_DIR)/man5
	

clean: echocwd
	rm -f $(OBJ) $(PROG) core

echocwd:
	@pwd
