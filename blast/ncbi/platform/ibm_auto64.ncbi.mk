#
# $Id: ibm_auto64.ncbi.mk,v 1.3 2004/01/09 19:01:51 beloslyu Exp $
#
# Initial version ddas@us.ibm.com 08/19/1999
# Replace r6k with ibm_auto or ibm_pwr3:  cpsosa@us.ibm.com Jun-2001
# -D_LARGE_FILES flag added: F. Parpia, K. Tzeng, C. Strauss Aug-2001
# OPENGL Targets to build CN3D: K. Tzeng and C. Strauss Aug-2001
# 64-bit version added: C. P. Sosa and D. Beloslyudtsev May-2003
#
# Add -DIBM_DISABLE_MMAP flag to NCBI_OPTFLAG if you wish 
# to use read instead of mmap
#
NCBI_DEFAULT_LCL = ibm
NCBI_MAKE_SHELL = /bin/sh
NCBI_AR = ar -X 64
NCBI_CC = xlc_r -q64 -qlanglvl=stdc99
NCBI_CFLAGS1 = -c -qcpluscmt
NCBI_LDFLAGS1 = -D_LARGE_FILES
NCBI_OPTFLAG = -O3 -qmaxmem=-1 -qarch=auto -qcache=auto -DPOSIX_THREADS_AVAIL -D_LARGE_FILES
NCBI_INCDIR = /usr/ncbi/include
NCBI_LIBDIR = /usr/ncbi/lib
NCBI_ALTLIB = /usr/ncbi/altlib
#will work only when you have Motif installed!
NCBI_VIBFLAG = -DWIN_MOTIF
NCBI_VIBLIBS = -lXm -lXmu -lXt -lX11
NCBI_DISTVIBLIBS = -lXm -lXmu -lXt -lX11
NCBI_OTHERLIBS = -lm
NCBI_RANLIB = ranlib
# Used by makedis.csh
NCBI_MT_OTHERLIBS = -lpthread
NCBI_THREAD_OBJ = ncbithr.o
NETENTREZVERSION = 2.02c2ASN1SPEC6 
# uncomment OPENGL_TARGETS to build OpenGL apps; do not change
# OPENGL_NCBI_LIBS! However, may need to set
# OPENGL_INCLUDE and OPENGL_LIBS to your local environment
#OPENGL_TARGETS = Cn3D
OPENGL_NCBI_LIBS = LIB400=libvibrantOGL.a LIB3000=libncbicn3dOGL.a
OPENGL_INCLUDE =
OPENGL_LIBS = -lGL -lGLU -lXGLW -lglut -lglobalm5
NCBI_OGLLIBS = -lGL -lGLU -lXGLW -lglut -lglobalm5

NCBI_LBSM_SRC = ncbi_lbsmd_stub.c
NCBI_LBSM_OBJ = ncbi_lbsmd_stub.o
