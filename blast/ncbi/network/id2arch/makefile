include $(NCBI)/ncbi.mk

INCLUDES = -I . -I$(NCBI_INCDIR)
CCOPT= -g -DSYSV=1
CFLAGS=$(CCOPT) $(INCLUDES) #-DDEBUG_ASN_PRINT

CODEGEN=$(NCBI)/bin/asncode
ASNTOOL=$(NCBI)/bin/asntool

NCBI_USED=/am/ncbiapdata/asn/seqset.asn
NCBI_REFS=/am/ncbiapdata/asn/seq.asn,/am/ncbiapdata/asn/seqfeat.asn,/am/ncbiapdata/asn/seqloc.asn,/am/ncbiapdata/asn/general.asn,/am/ncbiapdata/asn/seqalign.asn,/am/ncbiapdata/asn/seqcode.asn,/am/ncbiapdata/asn/seqres.asn

all: libncbiid2.a
#id2.o: id2.c id2.h
#	cpp -C -P -D_DEBUG -I. -I$(NCBI)/include id2.c >id2.i
#	cc -xildoff -c -g -D_DEBUG -I. -I$(NCBI)/include id2.c
#	$(NCBI_CC) -g -xildoff -c id2.i
libncbiid2.a : id2.o id2sgen.o
	rm -f $@
	ar -q $@ id2.o
	- rm -f *.l

id2.o: id2.c id2gen.h id2map.h id2sgen.h
id2sgen.o: id2sgen.c id2sgen.h id2map.h seqsplit.h

id2sgen.c id2sgen.h: seqsplit.l all.l 
	$(CODEGEN) -i id2map.h -l seqsplit.l,all.l -o id2sgen -d .

id2.c: id2.l id2.h all.l seqsplit.l
	$(CODEGEN) -i id2map.h -l id2.l,seqsplit.l,all.l -o id2gen -d .
	mv id2gen.c id2.c

id2.l : id2.asn
	$(ASNTOOL) -m id2.asn -l x.h
	cat x.l* | sed 's/x[.]l[0-9]*/id2.l/g' >id2.l
	rm -f x.*

seqsplit.l : seqsplit.asn
	$(ASNTOOL) -m seqsplit.asn -l x.h
	cat x.l* | sed 's/x[.]l[0-9]*/seqsplit.l/g' >seqsplit.l
	rm -f x.*

all.l : 
	cat $(NCBI_USED) >x.asn
	$(ASNTOOL) -m x.asn -M $(NCBI_REFS) -l x.h
	cat x.l* | sed 's/x[.]l[0-9]*/all.l/g' >all.l
	rm -f x.*

id2.h : id2.asn
	$(ASNTOOL) -m id2.asn -o id2.h
seqsplit.h : seqsplit.asn
	$(ASNTOOL) -m seqsplit.asn -o seqsplit.h

clean:
	- rm -f *.l* *.c *.o *.a
	- rm -f id2.h id2gen.h id2sgen.h seqsplit.h
	- rm -f id2generr id2genout
