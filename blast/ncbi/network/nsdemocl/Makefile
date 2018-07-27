#
#$Id: Makefile,v 6.1 1998/09/14 19:58:14 shavirin Exp $
#

include $(NCBI)/ncbi.mk

CC=cc 

EQUERY_LIBS = -lnetcli -lncbiobj -lncbi -lm -lsocket -lnsl -lresolv
OBJ_FILES =  catalogc.o 

CFLAGS= -g

.c.o: 
	$(CC) -c -g -I. -I$(NCBI_INCDIR) $(CFLAGS) $<

all: catalogc

catalogc.purify: $(OBJ_FILES) Makefile
	purify $(CC) $(OBJ_FILES) -L$(NCBI_ALTLIB) $(EQUERY_LIBS) -o ./catalogc

catalogc:  $(OBJ_FILES) Makefile
	$(CC) $(OBJ_FILES) -L$(NCBI_ALTLIB) $(EQUERY_LIBS) -o ./catalogc
