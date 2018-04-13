NAME=DarwinInstaller
SRCROOT?=.
OBJROOT?=/tmp/$(NAME).obj
SYMROOT?=/tmp/$(NAME).sym
DSTROOT?=/tmp/$(NAME).dst

CC=gcc
MKDIR=mkdir -p
STRIP=strip
INSTALL=install

.PHONY : install installhdrs pre-install \
	directories readme boot-plist iofindwholemedia rc.cdrom

install: pre-install \
	directories \
	boot-plist \
	iofindwholemedia \
	rc.cdrom \

#autodiskmount.plist (xcdis)


installhdrs:

pre-install: 
	@[ -d "$(SYMROOT)" ] || $(MKDIR) "$(SYMROOT)"
	@[ -d "$(DSTROOT)" ] || $(MKDIR) "$(DSTROOT)"

directories:
	$(MKDIR) "$(DSTROOT)/.vol"
	$(MKDIR) "$(DSTROOT)/Volumes"
	$(MKDIR) "$(DSTROOT)/mnt"
	$(MKDIR) "$(DSTROOT)/System/Installation"
	$(MKDIR) "$(DSTROOT)/Library/Preferences/SystemConfiguration"

readme:
	$(INSTALL) -c -m 0644 APPLE_DRIVER_LICENSE.txt "$(DSTROOT)/"
	$(INSTALL) -c -m 0644 APPLE_LICENSE.txt "$(DSTROOT)/"
	$(INSTALL) -c -m 0644 "Release Notes.txt" "$(DSTROOT)/"
	$(INSTALL) -c -m 0644 "Important Notice.txt" "$(DSTROOT)/"

boot-plist: com.apple.Boot.plist
	$(INSTALL) -c -m 0644 "$^" "$(DSTROOT)/Library/Preferences/SystemConfiguration"

iofindwholemedia: iofindwholemedia.c
	$(CC) $(RC_CFLAGS) -framework IOKit -framework CoreFoundation -o "$(SYMROOT)/$@" $^
	$(MKDIR) "$(DSTROOT)/usr/sbin"
	$(INSTALL) -c -m 0755 "$(SYMROOT)/$@" "$(DSTROOT)/usr/sbin"
	$(STRIP) "$(DSTROOT)/usr/sbin/$@"

rc.cdrom:
	$(MKDIR) "$(DSTROOT)/private/etc"
	$(INSTALL) -c -m 0755 "$@" "$(DSTROOT)/private/etc"
