# Set these variables as needed, then include this file, then:
#

# Project info
Project         = samba/source
UserType        = Administration
ToolType        = Services

GnuNoChown      = YES
GnuAfterInstall = install-startup-xinetd install-config install-logdir install-strip plugins

Extra_CC_Flags  = -no-cpp-precomp -L$(OBJROOT) -I$(SRCROOT)/dlcompat -I$(SRCROOT)/libopendirectorycommon \
		-DWITH_OPENDIRECTORY -DUSES_RECVFROM -DUSES_PWRITE -DUSES_PREAD

Extra_Configure_Flags = --with-swatdir="$(SHAREDIR)/swat"			\
			--with-sambabook="$(SHAREDIR)/swat/using_samba"		\
			--with-privatedir="$(VARDIR)/db/samba"			\
			--libdir="/etc"						\
			--with-lockdir="$(SPOOLDIR)/lock"			\
			--with-logfilebase="$(LOGDIR)/samba"			\
			--with-piddir="$(RUNDIR)"				\
			--with-cups						\
			--with-ldap						\
			--with-krb5						\
			--with-spinlocks					\
			--with-libiconv						\
			--with-winbind
#			--with-tdbsam						

Extra_Install_Flags   = SWATDIR="$(DSTROOT)$(SHAREDIR)/swat"			\
			SAMBABOOK="$(DSTROOT)$(SHAREDIR)/swat/using_samba"	\
			PRIVATEDIR="$(DSTROOT)$(VARDIR)/db/samba"		\
			VARDIR="$(DSTROOT)$(VARDIR)"				\
			LIBDIR="$(DSTROOT)/private/etc"				\
			PIDDIR="$(DSTROOT)$(RUNDIR)"				\
			MANDIR="$(DSTROOT)/usr/share/man"			\
			LOCKDIR="$(DSTROOT)$(SPOOLDIR)/lock"

include $(MAKEFILEPATH)/CoreOS/ReleaseControl/GNUSource.make

LDFLAGS += -framework DirectoryService -lopendirectorycommon

Install_Target = install

lazy_install_source::
	gcc $(CFLAGS) -c $(SRCROOT)/dlcompat/dl.c -o $(OBJROOT)/dl.o
	libtool -static -o $(OBJROOT)/libdl.a $(OBJROOT)/dl.o
	gcc $(CFLAGS) -c $(SRCROOT)/libopendirectorycommon/libopendirectorycommon.c -o $(OBJROOT)/libopendirectorycommon.o
	libtool -static -o $(OBJROOT)/libopendirectorycommon.a $(OBJROOT)/libopendirectorycommon.o

install-startup-item:
	mkdir -p $(DSTROOT)/System/Library/StartupItems/Samba/Resources/English.lproj
	$(INSTALL) -c -m 555 $(SRCROOT)/Samba.startup_item $(DSTROOT)/System/Library/StartupItems/Samba/Samba
	$(INSTALL) -c -m 444 $(SRCROOT)/StartupParameters.plist $(DSTROOT)/System/Library/StartupItems/Samba/
	$(INSTALL) -c -m 444 $(SRCROOT)/Localizable.strings $(DSTROOT)/System/Library/StartupItems/Samba/Resources/English.lproj

install-startup-xinetd:
	$(INSTALL) -d -m 755 $(DSTROOT)/private/etc/xinetd.d
	$(INSTALL) -c -m 444 $(SRCROOT)/smbd.xinetd $(DSTROOT)/private/etc/xinetd.d/smbd
	$(INSTALL) -c -m 444 $(SRCROOT)/nmbd.xinetd $(DSTROOT)/private/etc/xinetd.d/nmbd
	$(INSTALL) -c -m 444 $(SRCROOT)/smb-direct.xinetd $(DSTROOT)/private/etc/xinetd.d/smb-direct
	$(INSTALL) -c -m 444 $(SRCROOT)/swat.xinetd $(DSTROOT)/private/etc/xinetd.d/swat

install-config:
	$(INSTALL) -c -m 444 $(SRCROOT)/smb.conf.template $(DSTROOT)/private/etc

install-logdir:
	$(INSTALL) -d -m 755 $(DSTROOT)/private/var/log/samba
	mkdir -p $(DSTROOT)/private/var/spool/samba

install-strip:
	for F in $(DSTROOT)/usr/{s,}bin/*; do	\
		cp "$$F" $(SYMROOT); \
		[ -f "$$F" -a -x "$$F" ] && strip -x "$$F";	\
	done
	rmdir $(DSTROOT)/$(RUNDIR)
	rmdir $(DSTROOT)/private/etc/rpc
	rm -f $(DSTROOT)/usr/share/man/man8/smbmnt.8
	rm -f $(DSTROOT)/usr/share/man/man8/smbmount.8
	rm -f $(DSTROOT)/usr/share/man/man8/smbumount.8
	rm -f $(DSTROOT)/usr/share/swat/help/smbmnt.8.html
	rm -f $(DSTROOT)/usr/share/swat/help/smbmount.8.html
	rm -f $(DSTROOT)/usr/share/swat/help/smbumount.8.html

plugins:
	echo "building $@";
	make -C $(SRCROOT)/auth_ods -f auth_ods.make RC_CFLAGS="$(RC_CFLAGS)"
	mkdir -p $(DSTROOT)/usr/lib/samba/auth
	install -c -m 755 $(OBJROOT)/auth_ods.so $(DSTROOT)/usr/lib/samba/auth
	strip -x $(DSTROOT)/usr/lib/samba/auth/auth_ods.so
	make -C $(SRCROOT)/pdb_ods -f pdb_ods.make RC_CFLAGS="$(RC_CFLAGS)"
	mkdir -p $(DSTROOT)/usr/lib/samba/pdb
	install -c -m 755 $(OBJROOT)/pdb_ods.so $(DSTROOT)/usr/lib/samba/pdb
	strip -x $(DSTROOT)/usr/lib/samba/pdb/pdb_ods.so

