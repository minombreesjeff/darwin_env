#
# $Id: hpux.ncbi.mk,v 1.4 2001/06/12 21:11:56 vakatov Exp $
#
# testded on:
# HP-UX heart B.11.00 U 9000/800 71355 unlimited-user license
#
NCBI_DEFAULT_LCL = hp
NCBI_MAKE_SHELL = /bin/sh
NCBI_CC = cc
NCBI_CFLAGS1 = -c -z -Wp,-H500000
NCBI_LDFLAGS1 = 
NCBI_OPTFLAG = -O
NCBI_INCDIR = /usr/ncbi/include
NCBI_LIBDIR = /usr/ncbi/lib
NCBI_ALTLIB = /usr/ncbi/altlib
#will work only when you have Motif installed!
NCBI_VIBFLAG = -I/usr/include/X11R5 -I/usr/dt/include -L/usr/lib/X11R5 -L/usr/dt/lib -DWIN_MOTIF
NCBI_VIBLIBS = -lXm -lXt -lX11
NCBI_DISTVIBLIBS = -L/usr/include/X11R5 -L/usr/dt/lib /usr/lib/X11R4/libXmu.sl -lXm -lXt -lX11
NCBI_OTHERLIBS = -lm
NCBI_RANLIB = ranlib
# Used by makedis.csh
NCBI_MT_OTHERLIBS = -lpthread
NCBI_THREAD_OBJ = ncbithr.o
NETENTREZVERSION = 2.02c2ASN1SPEC6 

NCBI_LBSM_SRC = ncbi_service_lbsmd_stub.c
NCBI_LBSM_OBJ = ncbi_service_lbsmd_stub.o

