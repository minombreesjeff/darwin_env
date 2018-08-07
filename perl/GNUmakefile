##---------------------------------------------------------------------
# GNUmakefile for perl
# Call Makefile to do the work, but for the install case, unpack the
# tarball to create the project source directory
##---------------------------------------------------------------------
PROJECT = perl
VERSION = 5.8.4
FIX = $(SRCROOT)/fix
PROJVERS = $(PROJECT)-$(VERSION)
TARBALL = $(PROJVERS).tar.bz2
EXTRAS = /System/Library/Perl/Extras/$(VERSION)
LIBRARYPERL = /Library/Perl/$(VERSION)
APPENDFILE = AppendToPath
PREPENDFILE = PrependToPath

no_target:
	@$(MAKE) -f Makefile

##---------------------------------------------------------------------
# We patch hints/darwin.sh to install in $(DSTROOT), and to force putting
# things in the right place.
##---------------------------------------------------------------------
install:
	@if [ ! -d $(OBJROOT)/$(PROJECT) ]; then \
	    echo ditto $(SRCROOT) $(OBJROOT); \
	    ditto $(SRCROOT) $(OBJROOT); \
	    echo cd $(OBJROOT); \
	    cd $(OBJROOT); \
	    echo bzcat $(TARBALL) \| gnutar xf -; \
	    bzcat $(TARBALL) | gnutar xf -; \
	    echo rm -rf $(PROJECT); \
	    rm -rf $(PROJECT); \
	    echo mv $(PROJVERS) $(PROJECT); \
	    mv $(PROJVERS) $(PROJECT); \
	    echo Patching hints/darwin.sh; \
	    cat hints.append >> $(PROJECT)/hints/darwin.sh; \
	    echo Patching perl.c; \
	    chmod u+w $(PROJECT)/perl.c; \
	    ed - $(PROJECT)/perl.c < perl.c.ed; \
	fi
	$(MAKE) -C $(OBJROOT) -f Makefile install SRCROOT=$(OBJROOT) \
		OBJROOT="$(OBJROOT)/$(PROJECT)" VERSION=$(VERSION) \
		PREPENDFILE=$(PREPENDFILE) APPENDFILE=$(APPENDFILE)
	install -d $(DSTROOT)$(LIBRARYPERL)
	echo '$(EXTRAS)' > $(DSTROOT)$(LIBRARYPERL)/$(PREPENDFILE)

.DEFAULT:
	@$(MAKE) -f Makefile $@
