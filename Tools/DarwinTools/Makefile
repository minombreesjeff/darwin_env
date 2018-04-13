NAME=DarwinTools
SRCROOT?=.
OBJROOT?=/tmp/$(NAME).obj
SYMROOT?=/tmp/$(NAME).sym
DSTROOT?=/tmp/$(NAME).dst

CC=gcc
MKDIR=mkdir -p
STRIP=strip
INSTALL=install

.PHONY : install installhdrs pre-install \
	plist sw_vers startupfiletool

install: pre-install plist sw_vers startupfiletool

installhdrs:

pre-install: 
	@[ -d "$(SYMROOT)" ] || mkdir -p "$(SYMROOT)"
	@[ -d "$(DSTROOT)" ] || mkdir -p "$(DSTROOT)"

plist: SystemVersion.plist
	$(MKDIR) "$(DSTROOT)/System/Library/CoreServices"
	$(INSTALL) -c -m 0644 "$^" "$(DSTROOT)/System/Library/CoreServices/$^"

sw_vers: sw_vers.c
	$(CC) $(RC_CFLAGS) -framework CoreFoundation -o "$(SYMROOT)/$@" $^
	$(MKDIR) "$(DSTROOT)/usr/bin"
	$(INSTALL) -c -m 0755 "$(SYMROOT)/$@" "$(DSTROOT)/usr/bin"
	$(STRIP) "$(DSTROOT)/usr/bin/$@"

startupfiletool: startupfiletool.c
	$(CC) $(RC_CFLAGS) -o "$(SYMROOT)/$@" $^
	$(MKDIR) "$(DSTROOT)/usr/sbin"
	$(INSTALL) -c -m 0755 "$(SYMROOT)/$@" "$(DSTROOT)/usr/sbin"
	$(STRIP) "$(DSTROOT)/usr/sbin/$@"
