#
# $Id: linux-alpha.ncbi.mk,v 1.1 2004/05/13 17:10:26 ucko Exp $
#
NCBI_DEFAULT_LCL = lnx
NCBI_MAKE_SHELL = /bin/sh
#warning, the flags -D__USE_FILE_OFFSET64 -D__USE_LARGEFILE64 will allow
#you to work with large (>4Gb) files only if you have glibc version >= 2.1
#NCBI_CC = gcc -pipe -D__USE_FILE_OFFSET64 -D__USE_LARGEFILE64
#it appears the flags above do not work anymore with newer libc,
#the new flags should work. Dima. 08/23/01
NCBI_AR=ar
NCBI_CC = gcc -pipe -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE
NCBI_CFLAGS1 = -c
NCBI_LDFLAGS1 = -O3 -mieee
NCBI_OPTFLAG = -O3 -mieee
NCBI_BIN_MASTER = /home/coremake/ncbi/bin
NCBI_BIN_COPY = /home/coremake/ncbi/bin
NCBI_INCDIR = /home/coremake/ncbi/include
NCBI_LIBDIR = /home/coremake/ncbi/lib
NCBI_ALTLIB = /home/coremake/ncbi/altlib
#will work only when you have Motif installed!
NCBI_VIBFLAG = -I/usr/X11R6/include -L/usr/X11R6/lib -DWIN_MOTIF
NCBI_VIBLIBS = -lXmu -lXm -lXt -lSM -lICE -lXext -lXp -lX11 -ldl
#warning! If you have only dynamic version of Motif or Lesstif
#you should delete -Wl,-Bstatic sentence from the next line:
NCBI_DISTVIBLIBS = -L/usr/X11R6/lib -lXmu -lXm -lXt -lSM -lICE -lXext -lXp -lX11 -ldl
NCBI_OTHERLIBS = -lm
NCBI_RANLIB = ranlib
# Used by makedis.csh
NCBI_MT_OTHERLIBS = -lpthread
NCBI_OTHERLIBS_MT = $(NCBI_MT_OTHERLIBS) -lm
NCBI_THREAD_OBJ = ncbithr.o
NETENTREZVERSION = 2.02c2ASN1SPEC6 

# uncomment OPENGL_TARGETS to build OpenGL apps; do not change
# OPENGL_NCBI_LIBS! However, may need to set
# OPENGL_INCLUDE and OPENGL_LIBS to suit local environment
# OPENGL_TARGETS = Cn3D
OPENGL_NCBI_LIBS = LIB400=libvibrantOGL.a LIB3000=libncbicn3dOGL.a
OPENGL_INCLUDE = -I/usr/X11R6/include
OPENGL_LIBS = -L/usr/X11R6/lib -lGL -lGLU
NCBI_OGLLIBS = -L/usr/X11R6/lib -lGL -lGLU

# uncomment (and change appropriately) these lines to build PNG
# output support into Cn3D (OpenGL version only)
#LIBPNG_DIR = /home/paul/Programs/libpng
#ZLIB_DIR = /home/paul/Programs/zlib

NCBI_LBSM_SRC = ncbi_lbsmd_stub.c
NCBI_LBSM_OBJ = ncbi_lbsmd_stub.o

