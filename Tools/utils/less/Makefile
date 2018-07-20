##
# Makefile for less
##

# Project info
Project  = less
UserType = Administration
ToolType = Commands
GnuAfterInstall = link

# It's a GNU Source project
include $(MAKEFILEPATH)/CoreOS/ReleaseControl/GNUSource.make

link:
	$(LN) -f $(DSTROOT)/usr/bin/less $(DSTROOT)/usr/bin/more
	$(LN) -f $(DSTROOT)/usr/share/man/man1/less.1 $(DSTROOT)/usr/share/man/man1/more.1
