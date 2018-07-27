#
# $Id: alphaOSF1.ncbi.mk,v 1.13 2003/05/07 17:40:12 beloslyu Exp $
#
NCBI_MAKE_SHELL = /usr/bin/sh
NCBI_DEFAULT_LCL = alf
NCBI_AR=ar
NCBI_CC = cc
NCBI_CFLAGS1 = -c -pthread
NCBI_LDFLAGS1 = -O -pthread
NCBI_OPTFLAG = -O
NCBI_BIN_MASTER = /usr/home/coremake/ncbi/bin
NCBI_BIN_COPY = /usr/home/coremake/ncbi/bin
NCBI_INCDIR = /usr/home/coremake/ncbi/include
NCBI_LIBDIR = /usr/home/coremake/ncbi/lib
NCBI_ALTLIB = /usr/home/coremake/ncbi/altlib
NCBI_VIBFLAG = -DWIN_MOTIF
NCBI_VIBLIBS = -lXm -lXmu -lXt -lX11
NCBI_DISTVIBLIBS = -lXm -lXt -lXmu -lX11
NCBI_OTHERLIBS = -lm
NCBI_OTHERLIBS_MT = -lm -lpthread
NCBI_THR_OBJ = $(NCBI_LIBDIR)/ncbithr.o
NCBI_THR_ALTOBJ = $(NCBI_ALTLIB)/ncbithr.o
NCBI_RANLIB = ls -l
# Used by makedis.csh
NCBI_MT_OTHERLIBS = -lpthread
NCBI_THREAD_OBJ = ncbithr.o
NETENTREZVERSION = 2.02c2ASN1SPEC6 

NCBI_LBSM_SRC = ncbi_lbsmd_stub.c
NCBI_LBSM_OBJ = ncbi_lbsmd_stub.o

