#
#	Makefile to install the system-startup code for SecurityServer
#

# wouldn't it be nice if PBX actually $#@?@! defined those?
# (for future ref: CoreOS Makefiles define many standard path vars)
# Note: CORE_SERVICES_DIR should be absolute path in target environment (don't prefix with DSTROOT)
SYSTEM_LIBRARY_DIR=$(DSTROOT)/System/Library
SYSTEM_CORE_SERVICES_DIR=/System/Library/CoreServices
ETC_DIR=$(DSTROOT)/private/etc
AUTHORIZATION_LOCATION=$(ETC_DIR)
AUTHORIZATION_PLIST=$(AUTHORIZATION_LOCATION)/authorization
VARDB=$(DSTROOT)/private/var/db
CANDIDATES=$(VARDB)/CodeEquivalenceCandidates

OLD_STARTUP_DST=$(SYSTEM_LIBRARY_DIR)/StartupItems/securityd
DST=$(ETC_DIR)/mach_init.d
SRC=$(SRCROOT)/etc


#
# The other phases do nothing
#
build:	
	@echo null build.

debug:
	@echo null debug.

profile:
	@echo null profile.

#
# Install
#
install:
	mkdir -p $(DST)
	cp $(SRC)/securityd.plist $(DST)
	# Remove all OLD_STARTUP_DST-related actions when the Disks and 
	# Loginwindow StartupItems have removed their dependencies on 
	# "SecurityServer"
	mkdir -p $(OLD_STARTUP_DST)/Resources/English.lproj
	cp $(SRC)/StartupParameters.plist $(OLD_STARTUP_DST)
	sed -e "s:@@@:$(SYSTEM_CORE_SERVICES_DIR):g" $(SRC)/securityd >$(OLD_STARTUP_DST)/securityd
	cp $(SRC)/Localizable.strings $(OLD_STARTUP_DST)/Resources/English.lproj/Localizable.plist
	chown -R root:wheel $(OLD_STARTUP_DST)
	chmod 755 $(OLD_STARTUP_DST)/securityd
	chmod 644 $(OLD_STARTUP_DST)/StartupParameters.plist
	chmod 644 $(OLD_STARTUP_DST)/Resources/English.lproj/Localizable.plist
	mkdir -p $(AUTHORIZATION_LOCATION)
	cp $(SRC)/authorization.plist $(AUTHORIZATION_PLIST)
	chown root:admin $(AUTHORIZATION_PLIST)
	chmod 644 $(AUTHORIZATION_PLIST)
	mkdir -p $(VARDB)
	cp $(SRC)/CodeEquivalenceCandidates $(CANDIDATES)
	chown root:admin $(CANDIDATES)
	chmod 644 $(CANDIDATES)

installhdrs:
	@echo null installhdrs.

installsrc:
	@echo null installsrc.

clean:
	@echo null clean.
