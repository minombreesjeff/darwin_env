#
# $Id: hpux.ncbi.mk,v 1.9 2003/05/07 17:40:12 beloslyu Exp $
#
# testded on:
# HP-UX heart B.11.00 U 9000/800 71355 unlimited-user license
#
# Contributed by: Balaji Veeraraghavan <balaji_veeraraghavan@hp.com>
#
NCBI_DEFAULT_LCL = hp_pa
NCBI_MAKE_SHELL = /bin/sh
NCBI_AR=ar
NCBI_CC = cc -Ae +DA2.0 -fast -DHPUX -Wl,-aarchive_shared -ldld -Wl,-E
NCBI_CFLAGS1 = -c -z -Wp,-H500000
NCBI_LDFLAGS1 = 
NCBI_OPTFLAG = -fast
NCBI_INCDIR = /usr/ncbi/include
NCBI_LIBDIR = /usr/ncbi/lib
NCBI_ALTLIB = /usr/ncbi/altlib
#will work only when you have Motif installed!
NCBI_VIBFLAG = -I/usr/include/X11R6 -I/usr/dt/include -L/usr/lib/X11R6 -L/usr/dt/lib -DWIN_MOTIF
NCBI_VIBLIBS = -lXm -lXp -lXext -lXt -lX11
NCBI_DISTVIBLIBS = -L/usr/lib/X11R6 -L/usr/dt/lib /usr/lib/X11R4/libXmu.sl -lXm -lXp -lXext -lXt -lX11
NCBI_OTHERLIBS = -lm
NCBI_RANLIB = ranlib
# Used by makedis.csh
NCBI_MT_OTHERLIBS = -lpthread
NCBI_THREAD_OBJ = ncbithr.o
NETENTREZVERSION = 2.02c2ASN1SPEC6 

NCBI_LBSM_SRC = ncbi_lbsmd_stub.c
NCBI_LBSM_OBJ = ncbi_lbsmd_stub.o

