#
# $Id: sun.ncbi.mk,v 1.10 2003/05/07 17:40:12 beloslyu Exp $
#
NCBI_MAKE_SHELL = /usr/bin/sh
NCBI_DEFAULT_LCL = acc
NCBI_AR=ar
NCBI_CC = /usr/lang/acc
NCBI_CFLAGS1 = -c
NCBI_LDFLAGS1 = -O
NCBI_OPTFLAG = -O
NCBI_INCDIR = /usr/ncbi/include/NCBI
#NCBI_LIBDIR = /usr/ncbi/lib
NCBI_VIBLIBS = -lXm -lXmu -lXt -lX11 -lXext
NCBI_DISTVIBLIBS = -Bstatic -lXm -lXmu -lXt -lX11 -lXext -Bdynamic
NCBI_OTHERLIBS = -lresolv -lm
NCBI_RANLIB = ranlib
NCBI_SYBLIBS = -lsybdb
NCBI_SYBASE = /am/Sybase
NCBI_SYBFLAG = -I$(NCBI_SYBASE)/include -L$(NCBI_SYBASE)/lib
NCBI_VIBFLAG = -I/am/Motif/include -L/am/Motif/lib -DWIN_MOTIF -DMISSING_X_SYMBOLS_BUG
# NULL symbols, so standard make can be thread capable
NCBI_THREAD_OBJ =
NCBI_MT_OTHERLIBS =
NETENTREZVERSION = 2.02c2ASN1SPEC6

NCBI_LBSM_SRC = ncbi_lbsmd_stub.c
NCBI_LBSM_OBJ = ncbi_lbsmd_stub.o
