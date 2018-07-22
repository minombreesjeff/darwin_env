##
# Makefile for iODBC
##

# Configured for Panther (10.3) by default
# To rebuild on Jaguar (10.2) you need to define JAGUAR
#Extra_CC_Flags = -DJAGUAR

# Project info
Project  = iodbc
UserType = Developer
ToolType = Commands
#Install_Prefix = $(USRDIR)

GnuAfterInstall = fixup

# It's a GNU Source project
include $(MAKEFILEPATH)/CoreOS/ReleaseControl/GNUSource.make


#Install_Target = install
Extra_Configure_Flags = --with-iodbc-inidir=/Library/ODBC --disable-gui TMPDIR=$(OBJROOT)

fixup:
	@echo "Trashing RC undesirables *.la files"; \
	rm -f $(RC_Install_Prefix)/lib/*.la
	${MKDIR} $(RC_Install_Man)/man1; \
	${CP} $(SRCROOT)/odbctest.1 $(RC_Install_Man)/man1/; \
	${CHOWN} root:wheel $(RC_Install_Man)/man1/odbctest.1; \
	${CHMOD} 644 $(RC_Install_Man)/man1/odbctest.1
	${CP} $(SRCROOT)/iodbc-config.1 $(RC_Install_Man)/man1/; \
	${CHOWN} root:wheel $(RC_Install_Man)/man1/iodbc-config.1; \
	${CHMOD} 644 $(RC_Install_Man)/man1/iodbc-config.1
