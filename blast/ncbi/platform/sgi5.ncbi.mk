#
# $Id: sgi5.ncbi.mk,v 1.10 2003/05/07 17:40:12 beloslyu Exp $
#
NCBI_MAKE_SHELL = /bin/sh
NCBI_DEFAULT_LCL = sgi
NCBI_AR=ar
NCBI_CC = cc -mips1 `uname -r | sed 's/6\..*/-32/;s/5\..*//'`
NCBI_SYBASE = /usr/people/sybase_10.0.3
NCBI_SYBLIBS = -L$(NCBI_SYBASE)/lib -lsybdb
NCBI_SYBLIBS_STATIC = $(NCBI_SYBASE)/lib/libsybdb.a
NCBI_SYBFLAG = -I$(NCBI_SYBASE)/include -L$(NCBI_SYBASE)/lib
NCBI_CFLAGS1 = -c
NCBI_LDFLAGS1 = -O
NCBI_OPTFLAG = -O
NCBI_INCDIR = /usr/ncbi/ncbi5/include/NCBI
NCBI_LIBDIR = /usr/ncbi/ncbi5/lib
NCBI_ALTLIB =  /usr/ncbi/ncbi5/altlib
NCBI_VIBFLAG = -DWIN_MOTIF
NCBI_VIBLIBS = -lXm -lXmu -lXt -lX11
#
#if you want to link Motif statically, use that line
#NCBI_DISTVIBLIBS = -B static -lXm -B dynamic -lXmu -lXt -lX11 -lgen
#but if you want to link Motif dynamically, use that line
NCBI_DISTVIBLIBS = -lXm -lXmu -lXt -lX11 -lgen
#
NCBI_OTHERLIBS = -lm -lPW -lsun -lrpcsvc
NCBI_OTHERLIBS_MT = $(NCBI_OTHERLIBS)
NCBI_RANLIB = ls -l

# NULL symbols, so standard make can be thread capable
NCBI_THREAD_OBJ =
NCBI_MT_OTHERLIBS =

NCBI_ALTSRC =  /usr/ncbi/ncbi5/altsrc
#
#FastCGI library
LIBFASTCGI = -lfcgi
NETENTREZVERSION = 2.02c2ASN1SPEC6 

NCBI_LBSM_SRC = ncbi_lbsmd_stub.c
NCBI_LBSM_OBJ = ncbi_lbsmd_stub.o
