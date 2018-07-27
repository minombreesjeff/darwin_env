#
# $Id: sgi-mips4.ncbi.mk,v 1.10 2003/05/07 17:40:12 beloslyu Exp $
#
# The SGI IRIX 6.* in 64-bit mode
#
NCBI_MAKE_SHELL = /bin/sh
NCBI_DEFAULT_LCL = sgi
NCBI_AR=ar
NCBI_CC = cc -mips4 -64 -G0
NCBI_SYBASE = /usr/people/sybase_10.0.3
NCBI_SYBLIBS = -L$(NCBI_SYBASE)/lib -lsybdb
NCBI_SYBLIBS_STATIC = $(NCBI_SYBASE)/lib/libsybdb.a
NCBI_SYBFLAG = -I$(NCBI_SYBASE)/include -L$(NCBI_SYBASE)/lib
NCBI_CFLAGS1 = -c
NCBI_LDFLAGS1 = -O3 -LNO:opt=0 -OPT:wrap_around_unsafe_opt=OFF:IEEE_arithmetic=1
NCBI_OPTFLAG = -O3 -LNO:opt=0 -OPT:wrap_around_unsafe_opt=OFF:IEEE_arithmetic=1
NCBI_INCDIR = /usr/ncbi/ncbi/include/NCBI
NCBI_LIBDIR = /usr/ncbi/ncbi/lib
NCBI_ALTLIB =  /usr/ncbi/ncbi/altlib
NCBI_VIBFLAG = -DWIN_MOTIF
NCBI_VIBLIBS = -lXm -lXmu -lXt -lX11
#
#if you want to link Motif statically, use that line
#NCBI_DISTVIBLIBS = -B static -lXm -B dynamic -lXmu -lXt -lX11 -lgen
#but if you want to link Motif dynamically, use that line
NCBI_DISTVIBLIBS = -lXm -lXmu -lXt -lX11 -lgen
#
NCBI_OTHERLIBS = -lm -lPW -lrpcsvc
NCBI_OTHERLIBS_MT = $(NCBI_OTHERLIBS) -lpthread
# NCBI_MT_OTHERLIBS & NCBI_THREAD_OBJ are only used by master makefiles
NCBI_MT_OTHERLIBS = -lpthread
NCBI_THREAD_OBJ = ncbithr.o
NCBI_THR_OBJ = $(NCBI_LIBDIR)/ncbithr.o
NCBI_THR_ALTOBJ = $(NCBI_ALTLIB)/ncbithr.o

NCBI_RANLIB = ls -l

NCBI_OPTIONAL_LIBS = BLIB42=libctutils.a BLIB44=libidload.a

NCBI_ALTSRC =  /usr/ncbi/ncbi/altsrc

SEQUINDOC = /am/ncbiapdata/sequin.htm
SEQUINIMAGES = /am/ncbiapdata/images

NETENTREZVERSION = 2.02c2ASN1SPEC6 

# uncomment OPENGL_TARGETS to build OpenGL apps; do not change
# OPENGL_NCBI_LIBS! However, may need to set
# OPENGL_INCLUDE and OPENGL_LIBS to suit local environment
# OPENGL_TARGETS = Cn3D
OPENGL_NCBI_LIBS = LIB400=libvibrantOGL.a LIB3000=libncbicn3dOGL.a
OPENGL_INCLUDE =
OPENGL_LIBS = -lGL -lGLU

# uncomment (and change appropriately) these lines to build PNG
# output support into Cn3D (OpenGL version only)
#LIBPNG_DIR = /usr/people/thiessen/Programs/libpng
#ZLIB_DIR = /usr/people/thiessen/Programs/zlib

NCBI_LBSM_SRC = ncbi_lbsmd_stub.c
NCBI_LBSM_OBJ = ncbi_lbsmd_stub.o

