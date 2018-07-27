#
# $Id: ibm_pwr3.ncbi.mk,v 1.3 2001/09/13 19:35:57 beloslyu Exp $
#
# Initial version ddas@us.ibm.com 08/19/1999
# Replace r6k with ibm_pwr3 or ibm_auto: cpsosa@us.ibm.com Jun-2001
# -D_LARGE_FILES flag added: F. Parpia, K. Tzeng, C. Strauss Aug-2001
# OPENGL Targets to build CN3D: K. Tzeng and C. Strauss Aug-2001
#
NCBI_DEFAULT_LCL = ibm
NCBI_MAKE_SHELL = /bin/sh
NCBI_CC = xlc_r
NCBI_CFLAGS1 = -c -qcpluscmt
NCBI_LDFLAGS1 = -bmaxdata:0x40000000 -bmaxstack:0x10000000 -D_LARGE_FILES
NCBI_OPTFLAG = -g
NCBI_OPTFLAG = -O3 -qmaxmem=-1 -qarch=pwr3 -qtune=auto -qcache=auto -qcompact -DPOSIX_THREADS_AVAIL -D_LARGE_FILES
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

NCBI_LBSM_SRC = ncbi_service_lbsmd_stub.c
NCBI_LBSM_OBJ = ncbi_service_lbsmd_stub.o
