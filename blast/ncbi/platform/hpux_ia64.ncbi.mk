# $Id: hpux_ia64.ncbi.mk,v 1.5 2003/06/27 14:55:47 coulouri Exp $
#
# tested on:
# HP-UX spe175 B.11.22 U ia64 0319544412 unlimited-user license
#
# originally contributed by balaji_veeraraghavan@hp.com
#
NCBI_DEFAULT_LCL = hp_ia64
NCBI_MAKE_SHELL = /bin/sh
NCBI_AR=ar
NCBI_CC = cc
NCBI_CFLAGS1 = -c
NCBI_LDFLAGS1 = -Ae +DD64 +DSmckinley -fast -DHPUX -DHPUX_IA64
NCBI_OPTFLAG = -Ae +DD64 +DSmckinley -fast -DHPUX -DHPUX_IA64
NCBI_INCDIR = /usr/ncbi/include
NCBI_LIBDIR = /usr/ncbi/lib
NCBI_ALTLIB = /usr/ncbi/altlib
#will work only when you have Motif installed!
NCBI_VIBFLAG = -I/usr/include/X11R6 -I/usr/dt/include -L/usr/lib/X11R6 -L/usr/dt/lib -DWIN_MOTIF
NCBI_VIBLIBS = -lXm -lXt -lX11
NCBI_DISTVIBLIBS = -L/usr/lib/X11R6 -L/usr/dt/lib -lXmu -lXm -lXt -lX11
NCBI_OTHERLIBS = -lm
NCBI_RANLIB = ranlib
# Used by makedis.csh
NCBI_MT_OTHERLIBS = -lpthread
NCBI_THREAD_OBJ = ncbithr.o
NETENTREZVERSION = 2.02c2ASN1SPEC6 

NCBI_LBSM_SRC = ncbi_lbsmd_stub.c
NCBI_LBSM_OBJ = ncbi_lbsmd_stub.o
