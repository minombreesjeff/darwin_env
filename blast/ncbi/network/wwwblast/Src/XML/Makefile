include $(NCBI)/ncbi.mk

CC = $(NCBI_CC) 
OTHERLIBS = $(NCBI_OTHERLIBS)
#OTHERLIBS = $(NCBI_OTHERLIBS_MT)
LIBPATH = $(NCBI_ALTLIB)
#LIBPATH = $(NCBI_LIBDIR)
OPTFLAGS = -g
#OPTFLAGS = $(NCBI_OPTFLAG)
INCPATH = $(NCBI_INCDIR)
ASNTOOL=$(NCBI)/bin/asntool
NCBI_ASNALL=/am/ncbiapdata/asn/asn.all

all: bxmlobj.c

# The present directory is included first to allow the Makefile to find
# the local netblap1.h
CFLAGS = $(CFLAGS1) $(OPTFLAGS) -I./ -I$(INCPATH) $(VIBFLAG) $(NEWASN2FF) -g 
LDFLAGS = -I./ -I$(INCPATH) $(OPTFLAGS) -L$(LIBPATH) $(VIBFLAG) $(NEWASN2FF) -g 

#bxmlobj.o: bxmlobj.c
#	$(NCBI_CC) $(NCBI_CFLAGS1) $(OPTFLAGS) -I. -I$(NCBI_INCDIR) bxmlobj.c

bxmlobj.c: $(NCBI_ASNALL) blstxml.asn
	$(ASNTOOL) -m blstxml.asn -M $(NCBI_ASNALL) -B bxmlobj -G -w100 -Z
	$(ASNTOOL) -m blstxml.asn -o blstxml.h -w 100 -Z
#	$(ASNTOOL) -m blstxml.asn -X NCBI_BlastOutput.mod
#	mv bxmlobj.c ..
#	mv blstxml.h ..
#	mv bxmlobj.h ..
clean:
	rm -f *.h *.o *~ *.c
