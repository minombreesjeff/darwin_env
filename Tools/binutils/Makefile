.PHONY: all clean configure build install installsrc installhdrs \
	headers build-core build-binutils \
	install-frameworks-headers\
	install-frameworks-macosx \
	install-binutils-macosx \
	install-chmod-macosx install-clean install-source \
	check

ifndef RC_ARCHS
RC_ARCHS=$(shell /usr/bin/arch)
endif

ifndef SRCROOT
SRCROOT=.
endif

ifndef OBJROOT
OBJROOT=./obj
endif

ifndef SYMROOT
SYMROOT=./sym
endif

ifndef DSTROOT
DSTROOT=./dst
endif

INSTALL=$(SRCROOT)/src/install-sh

CANONICAL_ARCHS := $(foreach arch,$(RC_ARCHS),$(foreach os,$(RC_OS),$(foreach release,$(RC_RELEASE),$(os):$(arch):$(release))))

CANONICAL_ARCHS := $(subst macos:i386:$(RC_RELEASE),i386-apple-darwin,$(CANONICAL_ARCHS))
CANONICAL_ARCHS := $(subst macos:ppc:$(RC_RELEASE),powerpc-apple-darwin,$(CANONICAL_ARCHS))

CANONICAL_ARCHS := $(subst powerpc-apple-darwin i386-apple-darwin,i386-apple-darwin powerpc-apple-darwin,$(CANONICAL_ARCHS))

SRCTOP = $(shell cd $(SRCROOT) && pwd)
OBJTOP = $(shell (test -d $(OBJROOT) || $(INSTALL) -c -d $(OBJROOT)) && cd $(OBJROOT) && pwd)
SYMTOP = $(shell (test -d $(SYMROOT) || $(INSTALL) -c -d $(SYMROOT)) && cd $(SYMROOT) && pwd)
DSTTOP = $(shell (test -d $(DSTROOT) || $(INSTALL) -c -d $(DSTROOT)) && cd $(DSTROOT) && pwd)

ARCH_SAYS := $(shell /usr/bin/arch)
ifeq (i386,$(ARCH_SAYS))
BUILD_ARCH := i386-apple-darwin
else
ifeq (ppc,$(ARCH_SAYS))
BUILD_ARCH := powerpc-apple-darwin
else
BUILD_ARCH := $(ARCH_SAYS)
endif
endif

BINUTILS_VERSION = 2.13-20021117
APPLE_VERSION = 45

BINUTILS_VERSION_STRING = "$(BINUTILS_VERSION) (Apple version binutils-$(APPLE_VERSION))"

BINUTILS_BINARIES = objdump objcopy addr2line nm-new size strings cxxfilt
BINUTILS_MANPAGES = 

FRAMEWORKS = mmalloc liberty bfd opcodes binutils

CPP = cpp
CC = cc
LD = ld
AR = ar
RANLIB = ranlib
NM = nm
CC_FOR_BUILD = cc
CDEBUGFLAGS = -g
CFLAGS = $(CDEBUGFLAGS) $(RC_CFLAGS)
HOST_ARCHITECTURE = UNKNOWN

RC_CFLAGS_NOARCH = $(shell echo $(RC_CFLAGS) | sed -e 's/-arch [a-z0-9]*//g')

SYSTEM_FRAMEWORK = -L../intl -L./intl -L../intl/.libs -L./intl/.libs -lintl -framework System
FRAMEWORK_PREFIX =
FRAMEWORK_SUFFIX =
FRAMEWORK_VERSION = A
FRAMEWORK_VERSION_SUFFIX =

DEVEXEC_DIR=UNKNOWN
DOCUMENTATION_DIR=UNKNOWN
LIBEXEC_BINUTILS_DIR=UNKNOWN
LIBEXEC_LIB_DIR=UNKNOWN
MAN_DIR=UNKNOWN
LIB_DIR=UNKNOWN
PRIVATE_FRAMEWORKS_DIR=UNKNOWN

NATIVE_TARGET = unknown--unknown

NATIVE_TARGETS = $(foreach arch1,$(CANONICAL_ARCHS),$(arch1)--$(arch1))

CROSS_TARGETS = $(foreach arch1,$(CANONICAL_ARCHS),$(foreach arch2,$(filter-out $(arch1),$(CANONICAL_ARCHS)),$(arch1)--$(arch2)))

PPC_TARGET=UNKNOWN
I386_TARGET=UNKNOWN

CONFIG_VERBOSE=-v
CONFIG_ALL_BFD_TARGETS=
CONFIG_64_BIT_BFD=--enable-64-bit-bfd
CONFIG_WITH_MMAP=--with-mmap
CONFIG_WITH_MMALLOC=--with-mmalloc
CONFIG_ENABLE_SHARED=--enable-shared
CONFIG_MAINTAINER_MODE=--enable-maintainer-mode
CONFIG_BUILD=--build=$(BUILD_ARCH)
CONFIG_OTHER_OPTIONS=

TAR = gnutar

ifneq ($(findstring darwin,$(CANONICAL_ARCHS)),)
CC = cc -arch $(HOST_ARCHITECTURE) -no-cpp-precomp
CC_FOR_BUILD = NEXT_ROOT= cc -no-cpp-precomp
CDEBUGFLAGS = -g -Os
CFLAGS = $(CDEBUGFLAGS) -Wall -Wimplicit $(RC_CFLAGS_NOARCH)
HOST_ARCHITECTURE = $(shell echo $* | sed -e 's/--.*//' -e 's/powerpc/ppc/' -e 's/-apple-darwin.*//')
endif

MACOSX_FLAGS = \
	DEVEXEC_DIR=usr/bin \
	DOCUMENTATION_DIR=Developer/Documentation/DeveloperTools \
	LIBEXEC_BINUTILS_DIR=usr/libexec/binutils \
	MAN_DIR=usr/share/man \
	LIB_DIR=usr/lib \
	PRIVATE_FRAMEWORKS_DIR=System/Library/PrivateFrameworks \
	SOURCE_DIR=System/Developer/Source/Commands/binutils

CONFIGURE_OPTIONS = \
	$(CONFIG_VERBOSE) \
	$(CONFIG_ALL_BFD_TARGETS) \
	$(CONFIG_64_BIT_BFD) \
	$(CONFIG_WITH_MMAP) \
	$(CONFIG_WITH_MMALLOC) \
	$(CONFIG_ENABLE_SHARED) \
	$(CONFIG_BUILD) \
	$(CONFIG_OTHER_OPTIONS)

MAKE_OPTIONS = \
	prefix='/usr'

EFLAGS = \
	CFLAGS='$(CFLAGS)' \
	CC='$(CC)' \
	CPP='$(CPP)' \
	LD='$(LD)' \
	AR='$(AR)' \
	RANLIB='$(RANLIB)' \
	NM='$(NM)' \
	CC_FOR_BUILD='$(CC_FOR_BUILD)' \
	HOST_ARCHITECTURE='$(HOST_ARCHITECTURE)' \
	NEXT_ROOT='$(NEXT_ROOT)' \
	SRCROOT='$(SRCROOT)' \
	$(MAKE_OPTIONS)

SFLAGS = $(EFLAGS)

FFLAGS = \
	$(SFLAGS) \
	SYSTEM_FRAMEWORK='$(SYSTEM_FRAMEWORK)' \
	FRAMEWORK_PREFIX='$(FRAMEWORK_PREFIX)' \
	FRAMEWORK_SUFFIX='$(FRAMEWORK_SUFFIX)' \
	FRAMEWORK_VERSION_SUFFIX='$(FRAMEWORK_VERSION_SUFFIX)'

FSFLAGS = \
	$(SFLAGS) \
	MMALLOC='-F../mmalloc -framework mmalloc' \
	MMALLOC_DEP='../mmalloc/mmalloc.framework' \
	OPCODES='-F../opcodes -framework opcodes' \
	OPCODES_DEP='../opcodes/opcodes.framework' \
	BFD='-F../bfd -framework bfd' \
	BFD_DEP='../bfd/bfd.framework' \
	LIBIBERTY='-F../libiberty -framework liberty' \
	LIBIBERTY_DEP='../libiberty/liberty.framework'

CONFIGURE_ENV = $(EFLAGS)
MAKE_ENV = $(EFLAGS)

SUBMAKE = $(MAKE_ENV) $(MAKE)

_all: all

$(OBJROOT)/%/stamp-rc-configure:
	$(RM) -r $(OBJROOT)/$*
	$(INSTALL) -c -d $(OBJROOT)/$*
	(cd $(OBJROOT)/$* && \
		$(CONFIGURE_ENV) $(SRCTOP)/src/configure \
			--host=$(shell echo $* | sed -e 's/--.*//') \
			--target=$(shell echo $* | sed -e 's/.*--//') \
			$(CONFIGURE_OPTIONS) \
			)
	touch $@

$(OBJROOT)/%/stamp-build-headers:
	$(SUBMAKE) -C $(OBJROOT)/$* configure-intl configure-mmalloc configure-libiberty configure-bfd configure-opcodes
	$(SUBMAKE) -C $(OBJROOT)/$*/mmalloc $(FFLAGS) stamp-framework-headers 
	$(SUBMAKE) -C $(OBJROOT)/$*/libiberty $(FFLAGS) stamp-framework-headers
	$(SUBMAKE) -C $(OBJROOT)/$*/bfd $(FFLAGS) headers stamp-framework-headers
	$(SUBMAKE) -C $(OBJROOT)/$*/opcodes $(FFLAGS) stamp-framework-headers
	$(SUBMAKE) -C $(OBJROOT)/$* $(FFLAGS) stamp-framework-headers-binutils
	#touch $@

$(OBJROOT)/%/stamp-build-core:
	$(SUBMAKE) -C $(OBJROOT)/$* configure-intl configure-mmalloc configure-libiberty configure-bfd configure-opcodes
	$(SUBMAKE) -C $(OBJROOT)/$*/intl $(SFLAGS) libintl.la
	$(SUBMAKE) -C $(OBJROOT)/$*/mmalloc $(FFLAGS) stamp-framework
	$(SUBMAKE) -C $(OBJROOT)/$*/libiberty $(FFLAGS) stamp-framework
	$(SUBMAKE) -C $(OBJROOT)/$*/bfd $(FFLAGS) headers stamp-framework
	$(SUBMAKE) -C $(OBJROOT)/$*/opcodes $(FFLAGS) stamp-framework
	#touch $@

$(OBJROOT)/%/stamp-build-binutils:
	$(SUBMAKE) -C $(OBJROOT)/$* configure-binutils
	$(SUBMAKE) -C $(OBJROOT)/$*/binutils $(FSFLAGS) VERSION='$(BINUTILS_VERSION)' VERSION_STRING='$(BINUTILS_VERSION_STRING)' all
	$(SUBMAKE) -C $(OBJROOT)/$* $(FFLAGS) stamp-framework-binutils
	#touch $@

install-frameworks-headers:

	$(INSTALL) -c -d $(CURRENT_ROOT)/$(PRIVATE_FRAMEWORKS_DIR)

	set -e;	for i in $(FRAMEWORKS); do \
		framedir=$(CURRENT_ROOT)/$(PRIVATE_FRAMEWORKS_DIR)/$${i}.framework; \
		$(INSTALL) -c -d $${framedir}/Versions/A/PrivateHeaders; \
		$(INSTALL) -c -d $${framedir}/Versions/A/Headers; \
		ln -sf A $${framedir}/Versions/Current; \
		ln -sf Versions/Current/PrivateHeaders $${framedir}/PrivateHeaders; \
		ln -sf Versions/Current/Headers $${framedir}/Headers; \
	done

	set -e; for i in $(FRAMEWORKS); do \
		l=`echo $${i} | sed -e 's/liberty/libiberty/;' -e 's/binutils/\./;' -e 's/gdb/\./;'`; \
		(cd $(OBJROOT)/$(firstword $(NATIVE_TARGETS))/$${l}/$${i}.framework/Versions/A \
		 && $(TAR) --exclude=CVS -cf - Headers) \
		| (cd $(CURRENT_ROOT)/$(PRIVATE_FRAMEWORKS_DIR)/$${i}.framework/Versions/A \
		 && $(TAR) -xf -); \
	done

install-frameworks-resources:

	$(INSTALL) -c -d $(CURRENT_ROOT)/$(PRIVATE_FRAMEWORKS_DIR)

	set -e;	for i in $(FRAMEWORKS); do \
		framedir=$(CURRENT_ROOT)/$(PRIVATE_FRAMEWORKS_DIR)/$${i}.framework; \
		$(INSTALL) -c -d $${framedir}/Versions/A/Resources; \
		ln -sf Versions/Current/Resources $${framedir}/Resources; \
		$(INSTALL) -c -d $${framedir}/Versions/A/Resources/English.lproj; \
		cat $(SRCROOT)/Info-macos.template | sed -e "s/@NAME@/$$i/g" -e 's/@VERSION@/$(APPLE_VERSION)/g' > \
			$${framedir}/Versions/A/Resources/Info-macos.plist; \
	done

install-frameworks-macosx:

	$(SUBMAKE) CURRENT_ROOT=$(SYMROOT) install-frameworks-headers
	$(SUBMAKE) CURRENT_ROOT=$(DSTROOT) install-frameworks-headers
	$(SUBMAKE) CURRENT_ROOT=$(SYMROOT) install-frameworks-resources
	$(SUBMAKE) CURRENT_ROOT=$(DSTROOT) install-frameworks-resources

	set -e; for i in $(FRAMEWORKS); do \
		j=`echo $${i} | sed -e 's/liberty/libiberty/;' -e 's/binutils/\./;'`; \
		lipo -create -output $(SYMROOT)/$(PRIVATE_FRAMEWORKS_DIR)/$${i}.framework/Versions/A/$${i} \
			$(patsubst %,$(OBJROOT)/%/$${j}/$${i}.framework/Versions/A/$${i},$(NATIVE_TARGETS)); \
		strip -S -o $(DSTROOT)/$(PRIVATE_FRAMEWORKS_DIR)/$${i}.framework/Versions/A/$${i} \
			 $(SYMROOT)/$(PRIVATE_FRAMEWORKS_DIR)/$${i}.framework/Versions/A/$${i}; \
		ln -sf Versions/Current/$${i} $(SYMROOT)/$(PRIVATE_FRAMEWORKS_DIR)/$${i}.framework/$${i}; \
		ln -sf Versions/Current/$${i} $(DSTROOT)/$(PRIVATE_FRAMEWORKS_DIR)/$${i}.framework/$${i}; \
	done

	$(INSTALL) -c -d $(SYMROOT)/$(LIB_DIR)
	$(INSTALL) -c -d $(DSTROOT)/$(LIB_DIR)

	lipo -create -output $(SYMROOT)/$(LIB_DIR)/libintl.a \
		$(patsubst %,$(OBJROOT)/%/intl/.libs/libintl.a,$(NATIVE_TARGETS))
	strip -S -o $(DSTROOT)/$(LIB_DIR)/libintl.a \
		 $(SYMROOT)/$(LIB_DIR)/libintl.a
	lipo -create -output $(SYMROOT)/$(LIB_DIR)/libintl.1.0.0.dylib \
		$(patsubst %,$(OBJROOT)/%/intl/.libs/libintl.1.0.0.dylib,$(NATIVE_TARGETS))
	strip -S -o $(DSTROOT)/$(LIB_DIR)/libintl.1.0.0.dylib \
		 $(SYMROOT)/$(LIB_DIR)/libintl.1.0.0.dylib
	ln -sf libintl.1.0.0.dylib $(DSTROOT)/$(LIB_DIR)/libintl.1.dylib
	ln -sf libintl.1.0.0.dylib $(SYMROOT)/$(LIB_DIR)/libintl.1.dylib
	ln -sf libintl.1.0.0.dylib $(DSTROOT)/$(LIB_DIR)/libintl.dylib
	ln -sf libintl.1.0.0.dylib $(SYMROOT)/$(LIB_DIR)/libintl.dylib

install-binutils-common:

	set -e; for dstroot in $(SYMROOT) $(DSTROOT); do \
		\
		docroot=$${dstroot}/$(DOCUMENTATION_DIR)/binutils; \
		\
		$(INSTALL) -c -d $${docroot}/binutils; \
		\
		for i in $(BINUTILS_MANPAGES); do \
			$(INSTALL) -c -m 644 $(SRCROOT)/src/binutils/$${i} $${docroot}/`echo $${i} | sed -e 's/cxxfilt\\.man/cxxfilt.1/'`; \
		done; \
	done;

install-binutils-macosx: install-binutils-common

	set -e; for dstroot in $(SYMROOT) $(DSTROOT); do \
		\
		$(INSTALL) -c -d $${dstroot}/$(LIBEXEC_BINUTILS_DIR); \
		\
		docroot=$${dstroot}/$(DOCUMENTATION_DIR)/binutils; \
		\
		(cd $${docroot}/binutils && \
			$(SRCROOT)/texi2html $(SRCROOT)/src/binutils/binutils.texi); \
	done

	set -e; for i in $(BINUTILS_BINARIES); do \
		instname=`echo $${i} | sed -e 's/\\-new//'`; \
		lipo -create $(patsubst %,$(OBJROOT)/%/binutils/.libs/$${i},$(NATIVE_TARGETS)) \
			-output $(SYMROOT)/$(LIBEXEC_BINUTILS_DIR)/$${instname}; \
		strip -S -o $(DSTROOT)/$(LIBEXEC_BINUTILS_DIR)/$${instname} $(SYMROOT)/$(LIBEXEC_BINUTILS_DIR)/$${instname}; \
	done

install-chmod-macosx:

	set -e;	if [ `whoami` = 'root' ]; then \
		for dstroot in $(SYMROOT) $(DSTROOT); do \
			chown -R root.wheel $${dstroot}; \
			chmod -R  u=rwX,g=rX,o=rX $${dstroot}; \
			for i in $(FRAMEWORKS); do \
				chmod a+x $${dstroot}/$(PRIVATE_FRAMEWORKS_DIR)/$${i}.framework/Versions/A/$${i}; \
			done; \
			true || chmod a+x $${dstroot}/$(LIBEXEC_GDB_DIR)/*; \
			chmod -R a+x $${dstroot}/$(LIBEXEC_BINUTILS_DIR); \
			true || chmod -R a+x $${dstroot}/$(DEVEXEC_DIR); \
		done; \
	fi

install-source:
	$(INSTALL) -c -d $(DSTROOT)/$(SOURCE_DIR)
	$(TAR) --exclude=CVS -C $(SRCROOT) -cf - . | $(TAR) -C $(DSTROOT)/$(SOURCE_DIR) -xf -

all: build

clean:
	$(RM) -r $(OBJROOT)

check-args:
ifeq "$(CANONICAL_ARCHS)" "i386-apple-darwin"
else
ifeq "$(CANONICAL_ARCHS)" "powerpc-apple-darwin"
else
ifeq "$(CANONICAL_ARCHS)" "i386-apple-darwin powerpc-apple-darwin"
else
ifeq "$(CANONICAL_ARCHS)" "powerpc-apple-darwin i386-apple-darwin"
else
	echo "Unknown architecture string: \"$(CANONICAL_ARCHS)\""
	exit 1
endif
endif
endif
endif

configure: 
ifneq ($(NATIVE_TARGETS),)
	$(SUBMAKE) $(patsubst %,$(OBJROOT)/%/stamp-rc-configure, $(NATIVE_TARGETS))
endif

build-headers:
	$(SUBMAKE) configure
ifneq ($(NATIVE_TARGETS),)
	$(SUBMAKE) $(patsubst %,$(OBJROOT)/%/stamp-build-headers, $(NATIVE_TARGETS)) 
endif

build-core:
	$(SUBMAKE) configure
ifneq ($(NATIVE_TARGETS),)
	$(SUBMAKE) $(patsubst %,$(OBJROOT)/%/stamp-build-core, $(NATIVE_TARGETS)) 
endif

build-binutils:
	$(SUBMAKE) configure
ifneq ($(NATIVE_TARGETS),)
	$(SUBMAKE) $(patsubst %,$(OBJROOT)/%/stamp-build-binutils, $(NATIVE_TARGETS))
endif

build:
	$(SUBMAKE) check-args
	$(SUBMAKE) build-core
	$(SUBMAKE) build-binutils

install-clean:
	$(RM) -r $(SYMROOT) $(DSTROOT)

install-macosx:
	$(SUBMAKE) install-clean
ifeq "$(CANONICAL_ARCHS)" "i386-apple-darwin powerpc-apple-darwin"
	$(SUBMAKE) install-frameworks-macosx NATIVE_TARGET=unknown--unknown
	$(SUBMAKE) install-binutils-macosx NATIVE_TARGET=unknown--unknown
else
	$(SUBMAKE) install-frameworks-macosx
	$(SUBMAKE) install-binutils-macosx
endif

install:
	$(SUBMAKE) check-args
	$(SUBMAKE) build
	$(SUBMAKE) $(MACOSX_FLAGS) install-macosx
	$(SUBMAKE) $(MACOSX_FLAGS) install-chmod-macosx

installhdrs:
	$(SUBMAKE) check-args
	$(SUBMAKE) configure 
	$(SUBMAKE) build-headers
	$(SUBMAKE) install-clean
	$(SUBMAKE) $(MACOSX_FLAGS) CURRENT_ROOT=$(SYMROOT) install-frameworks-headers
	$(SUBMAKE) $(MACOSX_FLAGS) CURRENT_ROOT=$(DSTROOT) install-frameworks-headers

installsrc:
	$(SUBMAKE) check
	$(TAR) --dereference --exclude=CVS -cf - . | $(TAR) -C $(SRCROOT) -xf -

check:
	[ -z `find . -name \*~ -o -name .\#\*` ] || \
		(echo 'Emacs or CVS backup files present; not copying.' && exit 1)
