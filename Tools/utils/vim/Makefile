##
# Makefile for vim
##

# Project info
Project  = vim
#CommonNoInstallSource = YES

Extra_CC_Flags = -no-cpp-precomp
Extra_Configure_Flags = --enable-cscope --enable-gui=no --without-x
GnuAfterInstall = symlink

lazy_install_source:: shadow_source

# It's a GNU Source project
include $(MAKEFILEPATH)/CoreOS/ReleaseControl/GNUSource.make

Install_Target  = install
Install_Prefix = /usr
RC_Install_Prefix = $(Install_Prefix)

symlink:
	cd $(DSTROOT)/usr/bin && ln -s vim vi
