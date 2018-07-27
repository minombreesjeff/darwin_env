#CC=gcc

# Directory into which rfinger and rwhois programs will be installed:
CLIENTS_BIN_DIR=/usr/local/bin

# Directory into which man page files will be installed:
MAN_DEST_DIR=/usr/local/man

# make SOCKSified client
SOCKS_LIB =../lib/libsocks.a
SOCKS=-DSOCKS
# or
#SOCKS=-Dconnect=Rconnect -Dgetsockname=Rgetsockname -Dlisten=Rlisten -Daccept=Raccept -Drcmd=Rrcmd -Dbind=Rbind -Dselect=Rselect

# Remember to include -Dindex=strchr in OTHER_CFLAGS if
# you don't have index() (Sys-V camp)

# SunOS should use
#RESOLV_LIB=-lresolv
OTHER_CFLAGS= $(SHORTENED_RBIND)

# IRIX should use
#RESOLV_LIB=-lsun
#OTHER_CFLAGS=-cckr $(SHORTENED_RBIND) 

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
#INSTALL=/usr/ucb/install

# >>>---------------- Others:

# Define RESOLV_LIB if your system (e.g., SunOS before 4.1.1)
# doesn't search resolver library automatically.
# Leave it undefined otherwise.
# If your compiler or loader complains about _res_init being
# an undefined symbol, then you must define RESOLV_LIB.
#RESOLV_LIB= -lresolv

# <<<----------------

# The Internet Whois server; used to be nic.ddn.mil.
WHOIS_SERVER= -DWHOIS_SERVER'="rs.internic.net"'

OPTIMIZE=-g
CFLAGS = -I../include ${OPTIMIZE} ${OTHER_CFLAGS} ${SOCKS}

# The 'install' command is assumed to be the BSD variety (using -m to
# set the file mode). If the default 'install' on your system doesn't
# do that, you have to either specify an alternative one in the line below
# (e.g., /usr/ucb/install) or modify the install instructions.
INSTALL= install

#==============================================================================

OBJ = finger.o
SRC = finger.c

all: echocwd rfinger rwhois

rfinger: $(OBJ) $(SOCKS_LIB)
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(SOCKS_LIB) $(RESOLV_LIB)

rwhois: $(SRC) $(SOCKS_LIB)
	$(CC) $(WHOIS_SERVER) $(CFLAGS) -o $@ $(SRC) $(SOCKS_LIB) $(RESOLV_LIB)

install: echocwd rfinger rwhois
	$(INSTALL) -m 111 rfinger $(CLIENTS_BIN_DIR)
	$(INSTALL) -m 111 rwhois $(CLIENTS_BIN_DIR)

install.man: echocwd
	$(INSTALL) -m 444 ../doc/socks_clients.1 $(MAN_DEST_DIR)/man1
	$(INSTALL) -m 444 ../doc/socks.conf.5 $(MAN_DEST_DIR)/man5
	$(INSTALL) -m 444 ../doc/rfinger.1 $(MAN_DEST_DIR)/man1
	$(INSTALL) -m 444 ../doc/rwhois.1 $(MAN_DEST_DIR)/man1

clean: echocwd
	rm -f $(OBJ) rwhois rfinger core

echocwd:
	@pwd
