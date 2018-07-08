# Makefile for Apple Build + Integration
#
# We first build Git for each architecture by creating a
# symbolic link farm in subdirectories of $(OBJROOT) (e.g.
# $(OBJROOT)/i386) and building there using Git's own
# Makefiles.  Once all architectures are built, we use
# the first architecture's subdirectory to combine the
# per-architecture binaries into universal binaries.
# Finally, we use Git's own Makefiles to perform the
# installation from the first architecture's subdirectory.
# 
export makefile := $(realpath $(lastword $(MAKEFILE_LIST)))
export srcdir   := $(dir $(makefile))
export mandir   := $(srcdir)/src/git-manpages

.PHONY: all build install installsrc installhdrs root merge \
  install-bin install-man

export SRCROOT ?= $(CURDIR)
export OBJROOT ?= $(CURDIR)/roots/obj
export SYMROOT ?= $(CURDIR)/roots/sym
export DSTROOT ?= $(CURDIR)/roots/dst

PREFIX=$(DEVELOPER_DIR)/usr

ifdef SDKROOT
CC := $(shell xcrun -find -sdk $(SDKROOT) cc)
AR := $(shell xcrun -find -sdk $(SDKROOT) ar)
DSYMUTIL := $(shell xcrun -find -sdk $(SDKROOT) dsymutil)
else
CC := $(shell xcrun -find cc)
AR := $(shell xcrun -find ar)
DSYMUTIL := $(shell xcrun -find dsymutil)
endif

tmp := $(strip $(shell expr '$(RC_ProjectSourceVersion)' : \
  '\([0-9]\{1,4\}\(\.[0-9]\{0,3\}\)\{0,1\}\)$$'))
ifeq (,$(tmp))
override RC_ProjectSourceVersion := 9999
else
override RC_ProjectSourceVersion := $(tmp)
endif
export RC_ProjectSourceVersion
ifndef RC_ARCHS
RC_ARCHS := $(shell uname -m) $(warning using host architecture)
endif
cflags := $(strip $(RC_CFLAGS))
$(foreach arch,$(RC_ARCHS),$(eval cflags := $(subst $(cflags),-arch $(arch) ,)))
export RC_CFLAGS := $(cflags)

STRIP := strip -S
submakevars := -j`sysctl -n hw.activecpu` prefix=$(PREFIX) \
  PYTHON_PATH='MACOSX_DEPLOYMENT_TARGET="" /usr/bin/python' \
  NO_GETTEXT=YesPlease \
  NO_FINK=YesPlease NO_DARWIN_PORTS=YesPlease \
  RUNTIME_PREFIX=YesPlease \
  GITGUI_VERSION=0.12.2 V=1 \
  CFLAGS='-g3 -gdwarf-2 -Os -pipe -Wall -Wformat-security -D_FORTIFY_SOURCE=2'

objarch   := $(foreach arch,$(RC_ARCHS),$(OBJROOT)/$(arch))
firstarch := $(firstword $(objarch))

all: build

ifeq ($(realpath $(SRCROOT)), $(realpath $(srcdir)))
installsrc:
	@echo Nothing to do for installsrc
else
installsrc:
	mkdir -p $(SRCROOT)
	tar -cp --exclude .git --exclude .svn --exclude CVS . | tar -pox -C "$(SRCROOT)"
endif

installhdrs:
	@echo No headers to install.

build: $(OBJROOT)/dsyms.timestamp

install: install-bin install-man install-contrib
	rm -rf "$(DSTROOT)$(PREFIX)/share/git-gui"
	rm -f "$(DSTROOT)$(PREFIX)/libexec/git-core/git-gui"
	rm -f "$(DSTROOT)$(PREFIX)/share/man/man1/git-gui.1"
	install -d -o root -g wheel -m 0755 $(DSTROOT)$(PREFIX)/local/OpenSourceVersions
	install -o root -g wheel -m 0644 $(SRCROOT)/Git.plist $(DSTROOT)$(PREFIX)/local/OpenSourceVersions

install-contrib:
	install -d -o root -g wheel -m 0755 $(DSTROOT)$(PREFIX)/share/git-core
	install -o root -g wheel -m 0755 $(SRCROOT)/src/git/contrib/completion/git-completion.bash $(DSTROOT)$(PREFIX)/share/git-core

install-bin: $(OBJROOT)/dsyms.timestamp
	$(MAKE) -C $(firstarch) $(submakevars) \
	  'CC=$(CC) -arch $(firstword $(RC_ARCHS))' \
	  'AR=$(AR)' \
	  'DESTDIR=$(DSTROOT)' \
	  install
	rm -fr $(DSTROOT)$(PREFIX)/System #XXX Bogus perldoc installation

install-man: $(OBJROOT)/dir.timestamp
	install -d -o root -g wheel -m 0755 $(DSTROOT)$(PREFIX)/share/man
	ditto $(mandir) $(DSTROOT)$(PREFIX)/share/man
	chown -R root:wheel $(DSTROOT)$(PREFIX)/share/man

$(OBJROOT)/programs-list: $(firstarch)/build.timestamp
	$(MAKE) -C $(firstarch) -f $(CURDIR)/examine.make $(submakevars) \
	  print-programs > $@

$(OBJROOT)/universal.timestamp: \
  $(OBJROOT)/programs-list \
  $(SYMROOT)/dir.timestamp \
  $(foreach x,$(objarch),$(x)/build.timestamp)
	for prog in $$(cat $<); do \
	    test -h $(firstarch)/$$prog && continue; \
	    (set -x; lipo -create -output $(firstarch)/$$prog.u \
	      $(foreach x,$(objarch),$(x)/$$prog) && \
	      mv $(firstarch)/$$prog.u $(firstarch)/$$prog && \
	      cp $(firstarch)/$$prog $(SYMROOT)/ && \
	      $(STRIP) $(firstarch)/$$prog); \
	done
	touch $@

$(OBJROOT)/dsyms.timestamp: $(OBJROOT)/universal.timestamp
	cd $(SYMROOT) && \
	  find . -type f -perm -001 -print0 | xargs -n 1 -0 $(DSYMUTIL)
	touch $@

clean:: $(OBJROOT)/dir.timestamp
	cd $(OBJROOT) && rm -fr *

root:
	cd $(OBJROOT) && ditto -cz $(DSTROOT) \
	  $(or $(RC_ProjectName),git)-$(RC_ProjectSourceVersion).cpgz

merge:
	ditto -V $(DSTROOT) /

$(OBJROOT)/dir.timestamp $(SYMROOT)/dir.timestamp:
	mkdir -p $(dir $@) && touch $@

define each_arch
$(OBJROOT)/$(1)/dir.timestamp:
	mkdir -p $$(dir $$@) && touch $$@

$(OBJROOT)/$(1)/ditto.timestamp: $(OBJROOT)/$(1)/dir.timestamp
	ditto $$(CURDIR)/src/git $$(dir $$@) && touch $$@

$(OBJROOT)/$(1)/build.timestamp: $(OBJROOT)/$(1)/ditto.timestamp
	cat /dev/null > $$(OBJROOT)/$(1)/program-list
	$$(MAKE) -C $$(dir $$@) $$(submakevars) \
	  'CC=$$(CC) -arch $(1)' \
	  'AR=$$(AR)' \
	  'uname_M=$(1)' 'uname_P=$(1)' \
	  && touch $$@

endef

$(foreach arch,$(RC_ARCHS),$(eval $(call each_arch,$(arch))))

# ;; Local Variables: **
# ;; mode: makefile-gmake **
# ;; mode: ruler **
# ;; fill-column: 72 **
# ;; tab-width: 8 **
# ;; End: **
