#
# $Id: darwin.ncbi.mk,v 1.5 2001/07/26 16:40:50 beloslyu Exp $
#
# Tested on Darwin localhost 1.2 Darwin Kernel Version 1.2: Wed Aug 30 23:32:53 PDT 2000;
# root:xnu/xnu-103.obj~1/RELEASE_PPC  Power Macintosh powerpc
#
NCBI_DEFAULT_LCL = dwn
NCBI_MAKE_SHELL = /bin/sh
NCBI_CC = cc -pipe
NCBI_CFLAGS1 = -c -g -no-cpp-precomp -faltivec
NCBI_LDFLAGS1 = -O2
NCBI_OPTFLAG = -O3
NCBI_BIN_MASTER = /home/coremake/ncbi/bin
NCBI_BIN_COPY = /home/coremake/ncbi/bin
NCBI_INCDIR = /home/coremake/ncbi/include
NCBI_LIBDIR = /home/coremake/ncbi/lib
NCBI_ALTLIB = /home/coremake/ncbi/altlib
#will work only when you have Motif installed!
#NCBI_VIBFLAG = -I/usr/X11R6/include -L/usr/X11R6/lib -DWIN_MOTIF
#NCBI_VIBLIBS = -lXm -lXmu -lXp -lXpm -lXt -lX11 -lXext 
#NCBI_DISTVIBLIBS = -L/usr/X11R6/lib /usr/X11R6/lib/libXm.a /usr/X11R6/lib/libXp.a /usr/X11R6/lib/libXpm.a -lXmu -lXt -lX11 -lXext
NCBI_THREAD_OTHERLIBS = -lc
NCBI_OTHERLIBS = -lc
NCBI_RANLIB = ranlib
# Used by makedis.csh
NCBI_MT_OTHERLIBS = 
NCBI_THREAD_OBJ = ncbithr.o
NETENTREZVERSION = 2.02c2ASN1SPEC6 

NCBI_LBSM_SRC = ncbi_service_lbsmd_stub.c
NCBI_LBSM_OBJ = ncbi_service_lbsmd_stub.o

