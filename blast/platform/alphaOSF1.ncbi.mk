#
# $Id: alphaOSF1.ncbi.mk,v 1.8 2001/06/12 21:11:55 vakatov Exp $
#
NCBI_MAKE_SHELL = /usr/bin/sh
NCBI_DEFAULT_LCL = alf
NCBI_CC = cc
NCBI_CFLAGS1 = -c
NCBI_LDFLAGS1 = -O
NCBI_OPTFLAG = -O
NCBI_BIN_MASTER = /usr/home/coremake/ncbi/bin
NCBI_BIN_COPY = /usr/home/coremake/ncbi/bin
NCBI_INCDIR = /usr/home/coremake/ncbi/include
NCBI_LIBDIR = /usr/home/coremake/ncbi/lib
NCBI_ALTLIB = /usr/home/coremake/ncbi/altlib
#NCBI_VIBFLAG = -I/usr/X11R6/include -L/usr/X11R6/lib -DWIN_MOTIF
NCBI_VIBFLAG = -DWIN_MOTIF
NCBI_VIBLIBS = -lXm -lXmu -lXt -lX11 -lXext
NCBI_DISTVIBLIBS = -non_shared -lXm -lXmu -lXt -lX11 -lXext -lSM -lICE -ldnet_stub
NCBI_OTHERLIBS = -lm
NCBI_OTHERLIBS_MT = -lm -lpthread
NCBI_THR_OBJ = $(NCBI_LIBDIR)/ncbithr.o
NCBI_THR_ALTOBJ = $(NCBI_ALTLIB)/ncbithr.o
NCBI_RANLIB = ls -l
# Used by makedis.csh
NCBI_MT_OTHERLIBS = -lpthread
NCBI_THREAD_OBJ = ncbithr.o
NETENTREZVERSION = 2.02c2ASN1SPEC6 

NCBI_LBSM_SRC = ncbi_service_lbsmd_stub.c
NCBI_LBSM_OBJ = ncbi_service_lbsmd_stub.o

