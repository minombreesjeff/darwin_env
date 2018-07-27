########################################################################
#	demo makefile for NCBI applications
#
#   This assumes the following variables are set in a file called "ncbi.mk"
#     which is included by this one. In this case it resides in a directory
#     set by the environment variable "NCBI"
#
#    NCBI_CC = default compiler to use
#    NCBI_CFLAGS1 = default compiler switches (not optimization)
#    NCBI_LDFLAGS1 = default compiler switches when linking (not optimization)
#    NCBI_OPTFLAG = default optimization (-O)
#    NCBI_INCDIR =	default toolkit include paths
#    NCBI_LIBDIR = default toolkit library paths
#    NCBI_ALTLIB  = debugging toolkit library paths
#    NCBI_VIBFLAG = additional include paths/defines for Vibrant apps
#    NCBI_VIBLIBS  = additional libraries needed to link Vibrant apps
#    NCBI_OTHERLIBS = additional libraries needed to link the system
#
#   This enables platform independent makefiles for ncbi toolkit apps. You
#   can change any default behavior by setting the variable on the command
#   line.
#
#   Use gcc complier.   "make NCBI_CC=gcc" or "make CC=gcc"
#   Change optimization to debug
#                "make NCBI_OPTFLAG="-g" NCBI_LIBPATH=$NCBI_ALTLIB"
#                or
#                "make OPTFLAG="-g" LIBPATH=$NCBI_ALTLIB"
#
#   You can also change the behavior permanently in the makefile by changing
#    the assignment of the internal variable to the one from the include
#    file:
#
#   Use gcc compiler.
#    Change   CC=$(NCBI_CC)
#    To       CC=gcc
#    in the makefile itself
#
#   Make debugging version
#    OPTFLAG=-g
#    LIBPATH=$(NCBI_ALTDIR)
#
#   Current settings are:
#   Sun OS:
#    NCBI_CC = acc
#    NCBI_CFLAGS1 = "-c"
#    NCBI_LDFLAGS1 = ""
#    NCBI_OPTFLAG = "-O"
#    NCBI_INCPATH = "/usr/ncbi/include/NCBI"
#    NCBI_LIBPATH = "/usr/ncbi/lib"
#    NCBI_ALTLIB  = "/am/ncbi/altlib"
#    NCBI_VIBFLAG = "-I/am/Motif/include -L/am/Motif/lib -DWIN_MOTIF"
#    NCBI_VIBLIBS  = "-lXm -lXmu -lXt -lX11"
#    NCBI_OTHERLIBS = "-lm"
#
#   Silicon Graphics:
#    NCBI_CC = cc
#    NCBI_CFLAGS1 = "-c"
#    NCBI_LDFLAGS1 = ""
#    NCBI_OPTFLAG = "-O"
#    NCBI_INCPATH = "/usr/ncbi/include/NCBI"
#    NCBI_LIBPATH = "/usr/ncbi/lib"
#    NCBI_ALTLIB  = "/am/ncbi/altlib"
#    NCBI_VIBFLAG = "-I/am/Motif/include -L/am/Motif/lib -DWIN_MOTIF"
#    NCBI_VIBLIBS  = "-lXm -lXmu -lXt -lX11"
#    NCBI_OTHERLIBS = "-lm -lPW -lsun"
#
#   Solaris:
#    NCBI_CC = cc
#    NCBI_CFLAGS1 = "-c -Xa"
#    NCBI_LDFLAGS1 = "-Xa"
#    NCBI_OPTFLAG = "-O"
#    NCBI_INCPATH = "/netopt/ncbi_tools/ver0.0/ncbi/include/NCBI"
#    NCBI_LIBPATH = "/netopt/ncbi_tools/ver0.0/ncbi/lib"
#    NCBI_ALTLIB  = "/netopt/ncbi_tools/ver0.0/ncbi/altlib"
#    NCBI_VIBFLAG = "-I/netopt/X11R5/include -I/netopt/Motif1.2.2/include -L/netopt/X11R5/lib -L/netopt/Motif1.2.2/lib -DWIN_MOTIF"
#    NCBI_VIBLIBS  = "-lXm -lXmu -lXt -lX11"
#    NCBI_OTHERLIBS = "-lm -lsocket -lnsl"
#
#######################################################################
#
# default flags for compiling and loading
#
#######################################################################

include $(NCBI)/ncbi.mk

CC = $(NCBI_CC)
CFLAGS1 = $(NCBI_CFLAGS1)
LDFLAGS1 = $(NCBI_LDFLAGS1)
OPTFLAG = $(NCBI_OPTFLAG)
OTHERLIBS = $(NCBI_OTHERLIBS)
VIBLIBS= $(NCBI_VIBLIBS)
VIBFLAG= $(NCBI_VIBFLAG)
INCPATH = $(NCBI_INCDIR)
LIBPATH = $(NCBI_LIBDIR)

CFLAGS = $(CFLAGS1) $(OPTFLAG) -I$(INCPATH) $(VIBFLAG)
LDFLAGS = -I$(INCPATH) $(OPTFLAG) -L$(LIBPATH) $(VIBFLAG)

# all NCBI libraries aliased for easy access here

LIB1 = -lncbi
LIB2 = -lncbiobj
LIB3 = -lncbicdr
LIB4 = -lvibrant
LIB5 = -lncbiacc
LIB6 = -lnetcli
LIB7 = -lnetentr
LIB8 = -lncbiNacc
LIB9 = -lncbiCacc
# LIB10 is reserved for NCSA socket library
LIB10 =
LIB11 = -lncbimla
LIB12 = -lncbitax
LIB13 = -lncbiid0
LIB14 = -lncbibls1
LIB15 = -lncbispel

## To clean out the directory without removing make
##

## Implicit actions
##
## if need a .o, compile the .c
##

.c.o :
	$(CC) $(CFLAGS) $<

#####################################################################
##
## Here is where you set up your own things to make
##
#####################################################################

## Default thing to make will be "all", in this case it checks that the
## source files exist, then tries to make "testcds"
##


all : asn2asn


## make individual applications and components
##

## These are non-vibrant versions

asn2asn : asn2asn.c
	$(CC) -o asn2asn $(LDFLAGS) asn2asn.c $(LIB2) $(LIB1) $(OTHERLIBS)

asn2asn.pure : asn2asn.c
	purify3 $(CC) -o asn2asn.pure $(LDFLAGS) asn2asn.c $(LIB2) $(LIB1) $(OTHERLIBS)

## these are vibrant versions
##

asn2asnv : asn2asn.c
	$(CC) -o asn2asn $(LDFLAGS) asn2asn.c $(LIB4) $(LIB2) $(LIB1) $(OTHERLIBS)

asn2asnv.pure : asn2asn.c
	purify3 $(CC) -o asn2asn.pure $(LDFLAGS) asn2asn.c $(LIB4) $(LIB2) $(LIB1) $(OTHERLIBS)

