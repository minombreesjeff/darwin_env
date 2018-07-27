########################################################################
#	demo makefile for NCBI applications
#       THIS IS A DEMO FOR USING CODECENTER UNDER SOLARIS
#       It makes network entrez. Just copy the entrez.c file from
#        ncbi/demo to your working directory. The make sets the
#        paths to the codecenter libraries, includes, and sources for you
#
#       start codecenter from the directory with the entrez.c file from
#        ncbi/demo and this makefile (called makeentr.cl)
#       Then type "make -f makeentr.cl Nentrez.src"
#       It will load, link, and instrument the whole thing, including
#       all relevant code from the libraries.
#       Do not be put off by the following message:
#          CodeCenter cannot display all the requested information
#          because its size exceeds an internal limit.
#          This display is truncated.
#       It is just a display limit. The libraries are all instrumented
#       ok.
#
#       You can then push "run"
#       to execute it.
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
OPTFLAG = -g
OTHERLIBS = $(NCBI_CLOTHERLIBS)
VIBLIBS= $(NCBI_VIBLIBS)
VIBFLAG= $(NCBI_VIBFLAG)
INCPATH = $(NCBI_CLINCDIR)
LIBPATH = $(NCBI_CLLIB)
# LIBPATH = $(NCBI_ALTLIB)

CFLAGS = $(CFLAGS1) $(OPTFLAG) -I$(INCPATH) $(VIBFLAG)
LDFLAGS = -I./ -I$(INCPATH) $(OPTFLAG) -L$(LIBPATH) $(VIBFLAG)

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
LIB15 = -lnetcliE
LIB20 = -lncbidesk
LIB22 = -lncbimmdb
LIB23 = -lncbitool
LIB30 = -lncbicn3d
LIB31 = -lvibnet

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


SRC =	entrez.c


Nentrez.src : $(SRC)
	#setopt instrument_space 6
	#use ./ $(NCBI_ALTSRC)
	#load $(LDFLAGS) $(SRC) $(LIB31) $(LIB30) $(LIB20) $(LIB22) $(LIB23) $(LIB8) $(LIB7) $(LIB6) $(LIB5) $(LIB4) $(LIB3) $(LIB2) $(LIB1) $(VIBLIBS) $(OTHERLIBS)
	#link
	#instrument all

