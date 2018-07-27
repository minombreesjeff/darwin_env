#
# Makefile for the libident library

# Remember to include -Dindex=strchr in OTHER_CFLAGS if
# you don't have index() (Sys-V camp)

# IRIX should have CCKR defined:
#OTHER_CFLAGS=-cckr
#RANLIB=/bin/true

# AIX should use
#OTHER_CFLAGS=-D_BSD -D_NONSTD_TYPES -D_NO_PROTO -DAIX
#RANLIB=ranlib

# Solaris should use
#OTHER_CFLAGS=-DSOLARIS -Dindex=strchr -Drindex=strrchr -DUSE_DIRENT
#RANLIB=/bin/true

# Interactive Systems Unix should use
# OTHER_CFLAGS = -DISC

# LINUX should use
#CC=gcc
#RESOLV_LIB=
#OTHER_CFLAGS=-traditional -DLINUX 

# UnixWare should use
#RESOLV_LIB=-lresolv -lnsl -lsocket
#OTHER_CFLAGS= -DSOLARIS -Dindex=strchr -Drindex=strrchr -DUSE_DIRENT
#RANLIB=/bin/true
#INSTALL=bsdinstall

# >>>---------------- Others:

# This is slightly tuned for a 4BSD system (like SunOS 4). 
# For systems that do not need (and therefore don't have) 'ranlib',
# comment out the next line and use the line below.
RANLIB=ranlib
#RANLIB=/bin/true

# <<<----------------

# Set LIBDIR, INCDIR, and MANDIR to the direcories into which
# the library (libident.a), the include file (ident.h), and
# the man pages should be installed, respectively.
# Comment out the correponding lines for the items you don't
# want installed.
# This is where you want to install the library
LIBDIR=/usr/local/lib
# And this is where the header file ident.h goes
INCDIR=/usr/local/include
# And the manual page
MANDIR=/usr/local/man/man3

OPTIMIZE=-g
CFLAGS = ${OPTIMIZE} $(OTHER_CFLAGS)

# The 'install' command is assumed to be the BSD variety (using -m to
# set the file mode). If the default 'install' on your system doesn't
# do that, you have to either specify an alternative one in the line below
# (e.g., /usr/ucb/install) or modify the install instructions.
INSTALL= install

#=============================================================
OBJS = ident.o id_open.o id_close.o id_query.o id_parse.o

libident.a: echocwd $(OBJS)
	-rm -f libident.a
	ar cq libident.a $(OBJS)
	$(RANLIB) libident.a

ident.o:	ident.c ident.h
id_open.o:	id_open.c ident.h
id_close.o:	id_close.c ident.h
id_query.o:	id_query.c ident.h
id_parse.o:	id_parse.c ident.h

install: echocwd
	-if [ -d $(LIBDIR) ]; then \
	  ($(INSTALL) -m 644 libident.a $(LIBDIR); \
	   $(RANLIB) -t $(LIBDIR)/libident.a); fi
	-if [ -d $(INCDIR) ]; then \
	  $(INSTALL) -m 644 ident.h $(INCDIR); fi

install.man: echocwd
	-if [ -d $(MANDIR) ]; then \
	  $(INSTALL) -m 644 ident.3 $(MANDIR); fi

clean: echocwd
	-rm -f libident.a *~ core *.o \#*

echocwd:
	@pwd
