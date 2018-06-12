##
# Makefile for libiconv
##

# Project info
Project               = libiconv
UserType              = Administrator
ToolType              = Libraries
Extra_Configure_Flags = --disable-static
#Extra_LD_Flags        = -arch i386 -arch ppc
GnuAfterInstall       = strip

# It's a GNU Source project
include $(MAKEFILEPATH)/CoreOS/ReleaseControl/GNUSource.make

Install_Target = install

strip:
	strip -x $(DSTROOT)/usr/lib/libiconv.2.1.0.dylib
	strip -x $(DSTROOT)/usr/lib/libcharset.1.0.0.dylib
	strip -x $(DSTROOT)/usr/bin/*
	mv $(DSTROOT)/usr/doc $(DSTROOT)/usr/share
