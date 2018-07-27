#######################################################################
#
# $Id: devel.mk,v 6.4 1999/11/09 17:44:35 kimelman Exp $ 
#

# SGI options

NCBI_SYBLIBS_CT = -L$(NCBI_SYBASE)/lib -lblk -lct -lcs -ltcl -lcomn -lintl

include $(NCBI)/ncbi.mk

SHELL=$(NCBI_MAKE_SHELL)
CC = $(NCBI_CC)
INCPATH = $(NCBI_INCDIR)

# here is sybase and related libriries required for pubstruct access
PSLIBS=-lnlmzip -lctutils $(SYBLIBRARY)

CONFIG=.makeconf
STATE=.options

#
# DEFAULT target
#

default: compile

$(CONFIG):
	@echo "You should run ./make-tool.sh --config  $(CONFIG) [options..] to set environment"
	@echo "./make-tool.sh --help give you details"
	@echo "or run make with either 'production' or 'debug' target to set it"
	@echo "setting 'debug' default ...."
	./make-tool.sh --config $(CONFIG) --mode=development

include $(CONFIG)

CFLAGS  = -I. $(CFLAGS1)  $(CFLAGS_C)  $(OPTFLAG) -I$(INCPATH) $(NCBI_SYBFLAG)
LDFLAGS =     $(LDFLAGS1) $(LDFLAGS_C) $(OPTFLAG) -L$(LIBPATH)

.c.o :
	$(CC) $(CFLAGS) -c $<


#
# customization targets 
# 
production:
	./make-tool.sh --config $(CONFIG) --mode=production
debug:
	./make-tool.sh --config $(CONFIG) --mode=development


#
# source update target 
#
update index/st_configure.sh :
	@[ -d index   ] || cvs -q checkout -d index   internal/pubmed/pmentrez/scripts/indexing
	@cvs -q update

#
# internal targets : serve for recompilation enforcement if CFLAGS changed
# 

$(STATE):
	@echo "$(CFLAGS) $(LDFLAGS) $(TARGETS) $(PURIFY)" >$(STATE)

checks : $(STATE)
	@echo "$(CFLAGS) $(LDFLAGS) $(TARGETS)" >.$(STATE)
	@diff .$(STATE) $(STATE) >/dev/null || ( echo updating '$(STATE)' ; cp .$(STATE) $(STATE) ; rm .recompile )
	@rm -f .$(STATE)

.recompile: $(STATE) devel.mk $(CONFIG)
	@echo updating '.recompile'
	@-rm -f *.o
	@date >$@
prolog: checks .recompile

##
##  
##

compile: $(TARGET:=.compile)
install: $(TARGET:=.install)
try    : $(TARGET:=.test)

clean  :
	- rm -f *.o *~ *% \#* *.a *.test *.install
	rm $(STATE)
	for fn in mmdbsrv mmdbsrv.FF  mmdbsrv.PS vastsrv vastsrv.FF vastsrv.PS loader ; do [ ! -f $$fn ] || rm -f $$fn ; done

#
# web scripts settings
# 
WEB_SCRIPTS=wrapper.sh wrapper_lib.sh st_configure.sh
WEB_SCRIPTS_SRC=wrapper.sh wrapper_lib.sh index/st_configure.sh
$(webdir)/.syb_set : $(WEB_SCRIPTS_SRC)
	[ ! -f $@ ] || rm -rf $@
	cd ${webdir} ; rm -f $(WEB_SCRIPTS) ;
	cp $(WEB_SCRIPTS_SRC) $(webdir)
	cd ${webdir} ; chmod a+x $(WEB_SCRIPTS) 
	cd ${webdir} ; SYBASE=$(NCBI_SYBASE) ; export SYBASE ; ./st_configure.sh

web: $(webdir) $(webdir)/.syb_set


# 
# MMDBSRV
# 
MMDBLIBS= -lncbimmdb -lncbiNacc -lnetentr -lnetcli -lncbicdr -lncbiobj -lncbi $(NCBI_OTHERLIBS)
MMDB_OBJ= mmdbsrv.o 

mmdbsrv.FF: mmdbsrv.o mmdbFF.o 
	$(PURIFY) $(CC) -o $@ $(LDFLAGS)  mmdbsrv.o mmdbFF.o $(MMDBLIBS)

mmdbsrv.PS: mmdbsrv.o mmdb_pubstruct.o PubStructAsn.o
	$(PURIFY) $(CC) -o $@ $(LDFLAGS)  mmdbsrv.o mmdb_pubstruct.o PubStructAsn.o $(PSLIBS) $(MMDBLIBS)

mmdbsrv.compile: prolog mmdbsrv.$(RMODE)

mmdbsrv.install: mmdbsrv.compile web
	./make-tool.sh --install mmdbsrv.$(RMODE) $(webdir) $(mode)

MMDBTST=./make-tool.sh --test $(webdir) mmdbsrv.$(RMODE) $(mode)
mmdbsrv.test: mmdbsrv.install
	$(MMDBTST) 'uid=1' 
	$(MMDBTST) 'uid=1144&form=6&db=t&Dopt=s'
	$(MMDBTST) 'uid=1144'
	$(MMDBTST) 'uid=9524&form=6&db=t&save=See&dopt=k&Complexity=Cn3D+Subset&KinemageColor=Molecule+Number&KinemageRender=63'
	$(MMDBTST) 'uid=1042&form=6&db=t&save=&dopt=j&Complexity=Cn3D+Subset&KinemageColor=Molecule+Number&KinemageRender=63'   
	$(MMDBTST) 'uid=9660'

#
# VASTSRV
#

VAST_OBJ= vastsrv.o vastlocl.o vast2mage.o vast2pdb.o vast2cn3d.o vastsubs.o \
      vastchkid.o mkbioseq_vs.o mkbioseqA.o mkbioseqB.o

vastsrv.FF: $(VAST_OBJ) mmdbFF.o
	$(PURIFY) $(CC) -o $@ $(LDFLAGS) $(VAST_OBJ)  mmdbFF.o -lncbiwww -lncbimsc1 $(MMDBLIBS)

vastsrv.PS: $(VAST_OBJ) mmdb_pubstruct.o PubStructAsn.o
	$(PURIFY) $(CC) -o $@ $(LDFLAGS) $(VAST_OBJ) mmdb_pubstruct.o PubStructAsn.o $(PSLIBS) -lncbiwww -lncbimsc1 $(MMDBLIBS)

vastsrv.compile: prolog vastsrv.$(RMODE)

vastsrv.install: vastsrv.compile
	./make-tool.sh --install vastsrv.$(RMODE) $(webdir) $(mode)

VASTTST=./make-tool.sh --test $(webdir) vastsrv.$(RMODE) $(mode)
vastsrv.test: vastsrv.install
	$(VASTTST) 'uid=8100&chaindom=81000100'
	$(VASTTST) 'uid=1001&chaindom=10010100'
	$(VASTTST) 'uid=1001&chaindom=10010100&action=0&calltype=a&chn_complexity=1&atm_complexity=1&hit=954102001'
	$(VASTTST) 'uid=4107&chaindom=41070100&action=0&calltype=a&chn_complexity=1&atm_complexity=1&hit=708402001'
	$(VASTTST) 'uid=6104&chaindom=61040100&action=0&calltype=a&chn_complexity=1&atm_complexity=1&hit=929101001'
	$(VASTTST) 'uid=8063&chaindom=80630100&action=0&calltype=a&chn_complexity=1&atm_complexity=1&hit=912701001'

#
# PubStruct Loader
#

LOADLIBS= -lnlmzip -lctutils $(SYBLIBRARY) -lncbimmdb -lncbiobj -lncbi $(NCBI_OTHERLIBS)

loader : loader.o PubStructAsn.o
	$(PURIFY) $(CC) -o $@ $(LDFLAGS) loader.o  PubStructAsn.o $(LOADLIBS)


loader.compile : prolog loader

loader.install: loader.compile loader_check.sh index/st_configure.sh PubStruct_control.sh
	@[ -d $(loaddir) ] || ( \
          if [ x$(loaddir) = xbin ] ; \
          then mkdir bin ; \
          else \
            echo "Error: util tool installation failed - target directory <$(loaddir)> doesn't exist"; exit 1 ; \
          fi )
	cp loader $(loaddir)/st_loader
	cp index/st_configure.sh $(loaddir)
	cp PubStruct_control.sh loader_check.sh $(loaddir)
	chmod a+x $(loaddir)/st_configure.sh $(loaddir)/PubStruct_control.sh
	[ ! -f $(loaddir)/.syb_set ] || ( cd $(loaddir) ; rm -rf .syb_set )
	SYBASE=$(NCBI_SYBASE) ; export SYBASE ; cd $(loaddir) ; ./st_configure.sh

loader.test: loader.install
	[ ! -d mmdb_test ] || rm -rf mmdb_test
	mkdir mmdb_test
	cp /net/vaster/mmdb/mmdb/data/1.val.gz mmdb_test
	echo 1 >.testset
	dsql -S MOZART_SYS10 -U kimelman -P kmlmNu <PubStruct_proc.scr
	$(loaddir)/PubStruct_control.sh --DBserver MOZART_SYS10 --load_list .testset --path ./mmdb_test --enforce
	rm -rf mmdb_test .testset

#
# source dependencies 
# 

PubStructAsn.o : PubStructAsn.c PubStructAsn.h
loader.o : loader.c PubStructAsn.h

#
# service entries
# 

#.SILENT:
bsannotvs.o : vastlocl.h
bsannotvs.o : mmdblocl.h
bsannotvs.o : mmdbdata.h
bsannotvs.o : vast2mage.h
bsannotvs.o : vastsrv.h
loader.o : PubStructAsn.h
mkbioseqA.o : mkbioseq.h
mkbioseqB.o : mkbioseq.h
mkbioseq_vs.o : mkbioseq.h
mmdb_pubstruct.o : PubStructAsn.h
mmdbapi1.o : prunebsc.h
mmdblocl.o : PubStructAsn.h
prunebsc.o : prunebsc.h
vast2cn3d.o : vastlocl.h
vast2cn3d.o : mmdblocl.h
vast2cn3d.o : mmdbdata.h
vast2cn3d.o : vast2mage.h
vast2cn3d.o : vastsrv.h
vast2cn3d.o : mkbioseq.h
vast2mage.o : vastlocl.h
vast2mage.o : mmdblocl.h
vast2mage.o : mmdbdata.h
vast2mage.o : vast2mage.h
vast2mage.o : vastsrv.h
vast2pdb.o : vastlocl.h
vast2pdb.o : mmdblocl.h
vast2pdb.o : mmdbdata.h
vast2pdb.o : vast2mage.h
vast2pdb.o : vast2pdb.h
vast2pdb.o : vastsrv.h
vastchkid.o : vastlocl.h
vastchkid.o : mmdblocl.h
vastchkid.o : mmdbdata.h
vastchkid.o : vast2mage.h
vastchkid.o : vastsrv.h
vastlocl.o : vastlocl.h
vastsrv.o : vastlocl.h
vastsrv.o : mmdblocl.h
vastsrv.o : mmdbdata.h
vastsrv.o : vastsrv.h
