#
# $Id: hppalinux.ncbi.mk,v 1.3 2003/05/07 17:40:12 beloslyu Exp $
#
# Untested. Contributed by Howard Feldman <feldman@mshri.on.ca>
#
NCBI_DEFAULT_LCL = hlx
NCBI_MAKE_SHELL = /bin/sh
NCBI_AR=ar
NCBI_CC = gcc -pipe
NCBI_CFLAGS1 = -c
NCBI_LDFLAGS1 = -O2
NCBI_OPTFLAG = -O2
NCBI_BIN_MASTER = /home/ncbi/ncbi/bin
NCBI_BIN_COPY = /home/ncbi/ncbi/bin
NCBI_INCDIR = /home/ncbi/ncbi/include
NCBI_LIBDIR = /home/ncbi/ncbi/lib
NCBI_ALTLIB = /home/ncbi/ncbi/altlib
#will work only when you have Motif installed!
NCBI_VIBFLAG = -I/usr/X11R6/include -L/usr/X11R6/lib -DWIN_MOTIF
NCBI_VIBLIBS = -lXm -lXmu -lXt -lX11 -lXext
#warning! If you have only dynamic version of Motif or Lesstif
#you should delete -Wl,-Bstatic sentence from the next line:
NCBI_DISTVIBLIBS = -L/usr/X11R6/lib -Wl,-Bstatic -lXm -Wl,-Bdynamic -lXmu -lXt -lX11 -lXext -lXp
NCBI_OTHERLIBS = -lm -lpthread
NCBI_RANLIB = ranlib
# Used by makedis.csh
NCBI_MT_OTHERLIBS = -lpthread
NCBI_OTHERLIBS_MT = $(NCBI_MT_OTHERLIBS) -lm
NCBI_THREAD_OBJ = ncbithr.o
NETENTREZVERSION = 2.02c2ASN1SPEC6 

# uncomment OPENGL_TARGETS to build OpenGL apps; do not change
# OPENGL_NCBI_LIBS! However, may need to set
# OPENGL_INCLUDE and OPENGL_LIBS to suit local environment
OPENGL_TARGETS = Cn3D
OPENGL_NCBI_LIBS = LIB400=libvibrantOGL.a LIB3000=libncbicn3dOGL.a
OPENGL_INCLUDE =
OPENGL_LIBS = -L/usr/X11R6/lib -lGL -lGLU
NCBI_OGLLIBS = -L/usr/X11R6/lib -lGL -lGLU

# uncomment (and change appropriately) these lines to build PNG
# output support into Cn3D (OpenGL version only)
#LIBPNG_DIR = /usr/lib
#ZLIB_DIR = /usr/lib

NCBI_LBSM_SRC = ncbi_lbsmd_stub.c
NCBI_LBSM_OBJ = ncbi_lbsmd_stub.o
