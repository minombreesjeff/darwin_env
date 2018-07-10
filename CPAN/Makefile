PERL58ORGREATER = $(shell perl -MConfig -e 'print ($$Config{PERL_REVISION} > 5 || ($$Config{PERL_REVISION} == 5 && $$Config{PERL_VERSION} >= 8) ? "YES" : "NO")')


ifneq ($(PERL58ORGREATER),YES)  #check for 5.8.0 or later

$(error You must have perl 5.8.0 or later!)

endif

#
# Some documentation as to how this file works:  (please hold your applause until the end)
#
# This file builds various stock CPAN modules.  Each module is kept in its own subdirectory.
#


#
# You must do 2 things to have your module built:
#
# --1--
# There is a variable, $SUBDIRS, which defines what will get built.  If you're adding a 
# module to be built, you should add it to the $SUBDIRS list, and import it into the CPAN dir.
# Modules will be built in the order which they appear in the $SUBDIRS var.
#
# --2--
# If your module builds via 'perl Makefile.PL; make; make install;' with no modifications, 
# add it to the ExtUtils::MakeMaker section below.
#
# For modules which build with Module::Build, there is a section below for them as well.
#
# If your module requires special treatment, add a special target for them in the SPECIAL TARGETS
# section.
#

SUBDIRS = Convert-UUlib IO-String IO-Zlib Archive-Tar Compress-Zlib \
	Archive-Zip BerkeleyDB MailTools IO-stringy MIME-tools Convert-TNEF \
	Digest-SHA1 HTML-Parser Net-Server Unix-Syslog 

# These are the places where perl wants to install stuff.  
# Shouldn't need to be modified.
INSTALLMAN1DIR=$(shell perl -MConfig -e 'print $$Config{installman1dir}')
INSTALLMAN3DIR=$(shell perl -MConfig -e 'print $$Config{installman3dir}')

# Install in Extras if it exists
INSTALLEXTRASARCH=$(subst Perl,Perl/Extras,$(shell perl -MConfig -e 'print $$Config{installarchlib}'))
INSTALLEXTRAS=$(subst Perl,Perl/Extras,$(shell perl -MConfig -e 'print $$Config{installprivlib}'))

ifeq "$(shell test -d $(INSTALLEXTRAS) && echo YES )" "" 
    INSTALLEXTRASARCH=$(shell perl -MConfig -e 'print $$Config{installsitearch}')
    INSTALLEXTRAS=$(shell perl -MConfig -e 'print $$Config{installsitelib}')
endif


# This is a list of args to perl to add dirs to add to @INC, so that the 
# building modules can see dependent modules
INCARGS= -I$(DSTROOT)/$(INSTALLEXTRASARCH) -I$(DSTROOT)/$(INSTALLEXTRAS)

INSTALLBIN=$(shell perl -MConfig -e 'print $$Config{installbin}')
PREFIX=$(shell perl -MConfig -e 'print $$Config{prefix}')

#
# ExtUtils::MakeMaker section
#
# These modules only require the standard treatment:  perl Makefile.PL; make install;   
# Add your modules which build via that standard here.
#
Archive-Tar Archive-Zip BerkeleyDB Compress-Zlib Convert-TNEF Convert-UUlib Digest-SHA1 HTML-Parser IO-String IO-Zlib IO-stringy MIME-tools MailTools Net-Server Unix-Syslog::
	@echo "=============== Making $@ ==================";				\
	cd $(OBJROOT)/$@;								\
	mv Makefile.PL Makefile.PL.orig;						\
	cat Makefile.PL.orig ../add_rc_constants.pl > Makefile.PL;			\
	PERL_MM_USE_DEFAULT=1 perl $(INCARGS) Makefile.PL 		\
		"PERL=/usr/bin/perl $(INCARGS)"						\
	    INSTALLDIRS=site								\
	    INSTALLSITEARCH=$(DSTROOT)/$(INSTALLEXTRASARCH)				\
        INSTALLSITELIB=$(DSTROOT)/$(INSTALLEXTRAS)					\
        PREFIX=$(DSTROOT)/$(PREFIX)							\
	    INSTALLSITEBIN=$(DSTROOT)/$(INSTALLBIN)					\
	    INSTALLSCRIPT=$(DSTROOT)/$(INSTALLBIN)					\
	    INSTALLSITEMAN1DIR=$(DSTROOT)/$(INSTALLMAN1DIR)				\
	    INSTALLSITEMAN3DIR=$(DSTROOT)/$(INSTALLMAN3DIR);				\
	make all test pure_install;									
	@echo "";

#
# Module::Build section
#
# These modules use the newer Modules::Build module to build and install
# Add your Module::Build based modules here.
#
NO_TARGETS_HERE_YET::
	@echo "=============== Making $@ ==================";				\
	cd $(OBJROOT)/$@;								\
	perl $(INCARGS) Build.PL							\
	    destdir=$(DSTROOT) installdirs=site 					\
	    --install_path arch=$(INSTALLEXTRASARCH)					\
	    --install_path lib=$(INSTALLEXTRAS)						\
	    --install_path libdoc=$(INSTALLMAN3DIR)					\
	    --install_path bindoc=$(INSTALLMAN1DIR);					\
	perl $(INCARGS) Build;								\
	perl $(INCARGS) Build test;							\
	perl $(INCARGS) Build install;						\
	echo

#
# Custom build section
#
# Below here are modules which need some special treatment.  Add any such modules here.
# Please comment what you add, so that we can understand your reasoning...
#

#
# install, installhdrs, clean and installsrc are standard.
# These are the targets which XBS calls with make
#

install:: echo-config-info install-ditto-phase $(SUBDIRS)
	@if [ $(DSTROOT) ]; then							\
	    echo Stripping symbols from bundles ... ;					\
	    echo find $(DSTROOT) -xdev -name '*.bundle' -print -exec strip -S {} \; ;	\
	    find $(DSTROOT) -xdev -name '*.bundle' -print -exec strip -S {} \; ;	\
	    echo "" ;									\
	    echo Stripping packlists ... ;						\
	    find $(DSTROOT) -xdev -name '.packlist' -print -exec rm -f {} \; ;		\
	fi;										\
											\
											\
	rm -f $(DSTROOT)"$$INSTALLSITEARCH"/perllocal.pod


installhdrs::

clean::
	@for i in $(SUBDIRS); do						\
	    (									\
		echo "=============== Cleaning $$i ==================";		\
		cd $$i;								\
		if [ -e Makefile ]; then					\
		    make realclean;						\
		fi;	 							\
		rm -f Makefile.old;						\
		echo "";							\
	    )									\
	done									\

installsrc::
	ditto . $(SRCROOT)

install-ditto-phase::
	@if [ "$(OBJROOT)" != "." ]; then						\
	    ditto . $(OBJROOT);								\
	fi;
	(cd $(OBJROOT) && ./applyPatches)

echo-config-info::
	@echo "=============== Configuration Information ==============="
	@echo "Perl 5.8.0 or greater: $(PERL58ORGREATER)"
	@echo Building subdirs: $(SUBDIRS)
	@echo INSTALLSITEARCH=$(DSTROOT)/$(INSTALLEXTRASARCH)
	@echo INSTALLSITELIB=$(DSTROOT)/$(INSTALLEXTRAS)	
	@echo PREFIX=$(DSTROOT)/$(PREFIX)
	@echo INSTALLSITEBIN=$(DSTROOT)/$(INSTALLBIN)
	@echo INSTALLSITEMAN1DIR=$(DSTROOT)/$(INSTALLMAN1DIR)
	@echo INSTALLSITEMAN3DIR=$(DSTROOT)/$(INSTALLMAN3DIR)
	@echo 

