#
# Copyright (c) 2001-2006 Apple Computer, Inc.
#
# Starting with MySQL 3.23.54, the source patch to handle installation
# directories with tildes no longer works. Going forward, this Makefile
# takes the simpler approach of installing into a staging directory in /tmp 
# and then dittoing that into DSTROOT.
#
# The patch for config.h.in is needed regardless of MySQL version; it
# makes MySQL generate correct code for PPC  but not i386 when building fat.
#

# These includes provide the proper paths to system utilities

include $(MAKEFILEPATH)/pb_makefiles/platform.make
include $(MAKEFILEPATH)/pb_makefiles/commands-$(OS).make

PROJECT_NAME	= MySQL
MYSQL_VERSION	= mysql-4.1.22
MYSQL_SRC_PPC   = mysql
MYSQL_SRC_I386  = mysql_i386
MYSQL_SRC_PPC64 = mysql_ppc64
MYSQL_SRC_X86_64   = mysql_x86_64
BUILD_DIR	= /usr
STAGING_DIR 	:= $(shell mktemp -d /tmp/mysql-tmp-XXXXXX)
STAGING_DIR_I386    = $(STAGING_DIR)/archs/i386
STAGING_DIR_PPC64   = $(STAGING_DIR)/archs/ppc64
STAGING_DIR_X86_64  = $(STAGING_DIR)/archs/x86_64
STAGING_USRBIN        = $(STAGING_DIR)/usr/bin
STAGING_USRLIBMYSQL   = $(STAGING_DIR)/usr/lib/mysql
STAGING_USRLIBEXEC    = $(STAGING_DIR)/usr/libexec
STAGING_USRMYSQLTEST  = $(STAGING_DIR)/usr/mysql-test
STAGING_USRSHAREMAN1  = $(STAGING_DIR)/usr/share/man/man1
STAGING_USRSHAREMYSQL = $(STAGING_DIR)/usr/share/mysql
STAGING_USRBIN_I386        = $(STAGING_DIR_I386)/usr/bin
STAGING_USRLIBMYSQL_I386   = $(STAGING_DIR_I386)/usr/lib/mysql
STAGING_USRLIBEXEC_I386    = $(STAGING_DIR_I386)/usr/libexec
STAGING_USRMYSQLTEST_I386  = $(STAGING_DIR_I386)/usr/mysql-test
STAGING_USRSHAREMAN1_I386  = $(STAGING_DIR_I386)/usr/share/man/man1
STAGING_USRSHAREMYSQL_I386 = $(STAGING_DIR_I386)/usr/share/mysql
STAGING_USRBIN_PPC64        = $(STAGING_DIR_PPC64)/usr/bin
STAGING_USRLIBMYSQL_PPC64   = $(STAGING_DIR_PPC64)/usr/lib/mysql
STAGING_USRLIBEXEC_PPC64    = $(STAGING_DIR_PPC64)/usr/libexec
STAGING_USRMYSQLTEST_PPC64  = $(STAGING_DIR_PPC64)/usr/mysql-test
STAGING_USRSHAREMAN1_PPC64  = $(STAGING_DIR_PPC64)/usr/share/man/man1
STAGING_USRSHAREMYSQL_PPC64 = $(STAGING_DIR_PPC64)/usr/share/mysql
STAGING_USRBIN_X86_64        = $(STAGING_DIR_X86_64)/usr/bin
STAGING_USRLIBMYSQL_X86_64   = $(STAGING_DIR_X86_64)/usr/lib/mysql
STAGING_USRLIBEXEC_X86_64    = $(STAGING_DIR_X86_64)/usr/libexec
STAGING_USRMYSQLTEST_X86_64  = $(STAGING_DIR_X86_64)/usr/mysql-test
STAGING_USRSHAREMAN1_X86_64  = $(STAGING_DIR_X86_64)/usr/share/man/man1
STAGING_USRSHAREMYSQL_X86_64 = $(STAGING_DIR_X86_64)/usr/share/mysql
SHARE_DIR	= /usr/share
MYSQL_SHARE_DIR = $(SHARE_DIR)/mysql
VERSIONS_DIR=/usr/local/OpenSourceVersions
LICENSE_DIR=/usr/local/OpenSourceLicenses
LIBEXEC_DIR=/usr/libexec
INSTALL		=/usr/bin/install
DITTO		=/usr/bin/ditto
CHOWN		=/usr/sbin/chown
PATCH		=/usr/bin/patch
MKDIR		=/bin/mkdir
CONFIG_OPTS	= --infodir=/usr/share/info \
		--with-extra-charsets=complex \
		--with-low-memory \
		--enable-thread-safe-client \
		--enable-local-infile \
		--mandir=/usr/share/man \
		--localstatedir=/var/mysql  \
		--sysconfdir=/etc \
		--with-mysqld-user=mysql \
		--without-bench \
		--without-debug \
		--disable-shared \
		--disable-dependency-tracking \
		--with-unix-socket-path=/var/mysql/mysql.sock \
		--prefix=$(BUILD_DIR)
DO_PATCH_CONFIG_H	= $(SED) -f ../config.h.sed config.h > config.h.tmp; $(MV) config.h config.h.bak; $(MV) config.h.tmp config.h
DO_PATCH_IBCONFIG_H	= $(SED) -f ../../config.h.sed ib_config.h > ib_config.h.tmp; $(MV) ib_config.h ib_config.h.bak; $(MV) ib_config.h.tmp ib_config.h
DO_PATCH_MYHUGE_CNF	= $(PATCH) -u my-huge.cnf ../../my-huge.cnf.patch
DO_PATCH_MYLARGE_CNF	= $(PATCH) -u my-large.cnf ../../my-large.cnf.patch

FILES_TO_INSTALL		= \
$(MYSQL_VERSION).tar.gz \
Makefile \
MySQL.plist \
MySQL.txt \
mysqlman.1 \
config.h.sed \
my-huge.cnf.patch \
my-large.cnf.patch \
mysqld_safe.patch \
applemysqlcheckcnf \
applemysqlcheckcnf.8

FILES_TO_REMOVE = \
/usr/share/info/dir \
$(MYSQL_SHARE_DIR)/Info.plist \
$(MYSQL_SHARE_DIR)/Makefile \
$(MYSQL_SHARE_DIR)/ReadMe.txt \
$(MYSQL_SHARE_DIR)/StartupParameters.plist \
$(MYSQL_SHARE_DIR)/postinstall \
$(MYSQL_SHARE_DIR)/preinstall \
$(MYSQL_SHARE_DIR)/Description.plist \
$(MYSQL_SHARE_DIR)/make_win_src_distribution \
/usr/bin/make_win_binary_distribution \
/usr/bin/make_win_src_distribution

USRBIN_FILES_TO_RENAME = \
comp_err \
isamchk \
isamlog \
msql2mysql \
my_print_defaults \
myisam_ftdump \
myisamchk \
myisamlog \
myisampack \
mysql \
mysql_client_test \
mysql_config \
mysql_convert_table_format \
mysql_create_system_tables \
mysql_explain_log \
mysql_find_rows \
mysql_fix_extensions \
mysql_fix_privilege_tables \
mysql_install_db \
mysql_secure_installation \
mysql_setpermission \
mysql_tableinfo \
mysql_tzinfo_to_sql \
mysql_waitpid \
mysql_zap \
mysqlaccess \
mysqladmin \
mysqlbinlog \
mysqlbug \
mysqlcheck \
mysqld_multi \
mysqld_safe \
mysqldump \
mysqldumpslow \
mysqlhotcopy \
mysqlimport \
mysqlmanager \
mysqlmanager-pwgen \
mysqlmanagerc \
mysqlshow \
mysqltest \
pack_isam \
perror \
replace \
resolve_stack_dump \
resolveip

USRLIBEXEC_FILES_TO_RENAME = \
mysqld

USRMYSQLTEST_FILES_TO_RENAME = \
install_test_db \
mysql-test-run

USRSHAREMAN1_FILES_TO_RENAME = \
isamchk.1 \
isamlog.1 \
mysql.1 \
mysql_fix_privilege_tables.1 \
mysql_zap.1 \
mysqlaccess.1 \
mysqladmin.1 \
mysqld.1 \
mysqld_multi.1 \
mysqld_safe.1 \
mysqldump.1 \
mysqlshow.1 \
msql2mysql.1 \
myisam_ftdump.1 \
myisamchk.1 \
myisamlog.1 \
myisampack.1 \
mysql.server.1 \
mysql_config.1 \
mysql_explain_log.1 \
mysqlbinlog.1 \
mysqlcheck.1 \
mysqlhotcopy.1 \
mysqlimport.1 \
mysqlman.1 \
pack_isam.1 \
perror.1 \
replace.1 \
safe_mysqld.1

USRSHAREMYSQL_FILES_TO_RENAME = \
mysql.server

USRBIN_FILES_TO_LIPO = \
comp_err \
isamchk \
isamlog \
my_print_defaults \
myisam_ftdump \
myisamchk \
myisamlog \
myisampack \
mysql \
mysql_client_test \
mysql_tzinfo_to_sql \
mysql_waitpid \
mysqladmin \
mysqlbinlog \
mysqlcheck \
mysqldump \
mysqlimport \
mysqlmanager \
mysqlmanager-pwgen \
mysqlmanagerc \
mysqlshow \
mysqltest \
pack_isam \
perror \
replace \
resolve_stack_dump \
resolveip

USRLIBMYSQL_FILES_TO_LIPO = \
libdbug.a \
libheap.a \
libmerge.a \
libmyisam.a \
libmyisammrg.a \
libmysqlclient.a \
libmysqlclient_r.a \
libmystrings.a \
libmysys.a \
libnisam.a \
libvio.a

USRLIBEXEC_FILES_TO_LIPO = \
mysqld

MANPAGE_SYMLINKS = \
client_test \
comp_err \
msql2mysql \
my_print_defaults \
myisam_ftdump \
myisamchk \
myisamlog \
myisampack \
mysql_client_test \
mysql_config \
mysql_convert_table_format \
mysql_create_system_tables \
mysql_explain_log \
mysql_find_rows \
mysql_fix_extensions \
mysql_install_db \
mysql_secure_installation \
mysql_setpermission \
mysql_tableinfo \
mysql_tzinfo_to_sql \
mysql_waitpid \
mysqlbinlog \
mysqlbug \
mysqlcheck \
mysqldumpslow \
mysqlhotcopy \
mysqlimport \
mysqlmanager \
mysqlmanager-pwgen \
mysqlmanagerc \
mysqltest \
pack_isam \
resolve_stack_dump \
resolveip 

default: build

clean:
	@$(ECHO) "#"
	@$(ECHO) "#= = = = = = = = = = = = = = = = = = ="
	@$(ECHO) "#"
	@$(ECHO) "# `date +%Y/%m/%d\ %H:%M:%S` MySQL: Cleaning project..."
	@$(ECHO) "#"
	@$(ECHO) "#= = = = = = = = = = = = = = = = = = ="
	@$(ECHO) "#"
	$(SILENT) -$(RM) -rf $(MYSQL_SRC_PPC) $(MYSQL_SRC_I386) $(MYSQL_SRC_PPC64) $(MYSQL_SRC_X86_64)

installhdrs:
	@$(ECHO) "#"
	@$(ECHO) "#= = = = = = = = = = = = = = = = = = ="
	@$(ECHO) "#"
	@$(ECHO) "# `date +%Y/%m/%d\ %H:%M:%S` MySQL: Install headers in $(SRCROOT)..."
	@$(ECHO) "#"
	@$(ECHO) "#= = = = = = = = = = = = = = = = = = ="
	@$(ECHO) "#"
	@$(ECHO) "# `date +%Y/%m/%d\ %H:%M:%S` MySQL: $(PROJECT_NAME) has no headers to install."

installsrc:
	@$(ECHO) "#"
	@$(ECHO) "#= = = = = = = = = = = = = = = = = = ="
	@$(ECHO) "#"
	@$(ECHO) "# `date +%Y/%m/%d\ %H:%M:%S` MySQL: Installing sources in $(SRCROOT)..."
	@$(ECHO) "#"
	@$(ECHO) "#= = = = = = = = = = = = = = = = = = ="
	@$(ECHO) "#"
	$(SILENT) -$(RM) -rf $(SRCROOT)
	$(SILENT) $(MKDIRS) $(SRCROOT)
	$(SILENT) $(CP) $(FILES_TO_INSTALL) $(SRCROOT)

MYSQL_SRC_DIRS = $(MYSQL_SRC_PPC) $(MYSQL_SRC_I386) $(MYSQL_SRC_PPC64) $(MYSQL_SRC_X86_64)
mysql: $(OBJROOT)
	@$(ECHO) "#"
	@$(ECHO) "#= = = = = = = = = = = = = = = = = = ="
	@$(ECHO) "#"
	@$(ECHO) "# `date +%Y/%m/%d\ %H:%M:%S` MySQL: Extracting sources from archive..."
	@$(ECHO) "#"
	@$(ECHO) "#= = = = = = = = = = = = = = = = = = ="
	@$(ECHO) "#"
	$(SILENT) -$(RM) -rf $(MYSQL_VERSION)
	for i in $(MYSQL_SRC_DIRS); do \
		$(SILENT) -$(RM) -rf $$i; \
		$(SILENT) $(TAR) -xzf $(MYSQL_VERSION).tar.gz; \
		$(MV) $(MYSQL_VERSION) $$i; \
	done

untar: mysql 

$(MYSQL_SRC_PPC)/config.status:
	@$(ECHO) "#"
	@$(ECHO) "#= = = = = = = = = = = = = = = = = = ="
	@$(ECHO) "#"
	@$(ECHO) "# `date +%Y/%m/%d\ %H:%M:%S` MySQL: Configuring ARCH=ppc..."
	@$(ECHO) "#"
	@$(ECHO) "#= = = = = = = = = = = = = = = = = = ="
	@$(ECHO) "#"
	$(SILENT) $(CD) $(MYSQL_SRC_PPC);\
	CFLAGS="-O3 -fno-omit-frame-pointer $$RC_CFLAGS" \
	CXXFLAGS="-O3 -fno-omit-frame-pointer -felide-constructors -fno-exceptions -fno-rtti $$RC_CFLAGS" \
	LDFLAGS="$$RC_CFLAGS" \
	ac_cv_c_bigendian=yes \
	./configure --target=ppc-apple-darwin $(CONFIG_OPTS)
	@$(ECHO) "#"
	@$(ECHO) "# `date +%Y/%m/%d\ %H:%M:%S` MySQL: Patching config.h; ARCH=ppc..."
	@$(ECHO) "#"
	$(SILENT) $(CD) $(MYSQL_SRC_PPC); $(DO_PATCH_CONFIG_H)
	@$(ECHO) "#"
	@$(ECHO) "# `date +%Y/%m/%d\ %H:%M:%S` MySQL: Patching innobase/ib_config.h; ARCH=ppc..."
	@$(ECHO) "#"
	$(SILENT) $(CD) $(MYSQL_SRC_PPC)/innobase; $(DO_PATCH_IBCONFIG_H)

$(MYSQL_SRC_I386)/config.status:
	@$(ECHO) "#"
	@$(ECHO) "#= = = = = = = = = = = = = = = = = = ="
	@$(ECHO) "#"
	@$(ECHO) "# `date +%Y/%m/%d\ %H:%M:%S` MySQL: Configuring ARCH=i386..."
	@$(ECHO) "#"
	@$(ECHO) "#= = = = = = = = = = = = = = = = = = ="
	@$(ECHO) "#"
	$(SILENT) $(CD) $(MYSQL_SRC_I386);\
	CFLAGS="-O3 -fno-omit-frame-pointer $$RC_CFLAGS" \
	CXXFLAGS="-O3 -fno-omit-frame-pointer -felide-constructors -fno-exceptions -fno-rtti $$RC_CFLAGS" \
	LDFLAGS="$$RC_CFLAGS" \
	ac_cv_c_bigendian=yes \
	./configure --target=i386-apple-darwin $(CONFIG_OPTS)
	@$(ECHO) "#"
	@$(ECHO) "# `date +%Y/%m/%d\ %H:%M:%S` MySQL: Patching config.h; ARCH=i386..."
	@$(ECHO) "#"
	$(SILENT) $(CD) $(MYSQL_SRC_I386); $(DO_PATCH_CONFIG_H)
	@$(ECHO) "#"
	@$(ECHO) "# `date +%Y/%m/%d\ %H:%M:%S` MySQL: Patching innobase/ib_config.h; ARCH=i386..."
	@$(ECHO) "#"
	$(SILENT) $(CD) $(MYSQL_SRC_I386)/innobase; $(DO_PATCH_IBCONFIG_H)

$(MYSQL_SRC_PPC64)/config.status:
	@$(ECHO) "#"
	@$(ECHO) "#= = = = = = = = = = = = = = = = = = ="
	@$(ECHO) "#"
	@$(ECHO) "# `date +%Y/%m/%d\ %H:%M:%S` MySQL: Configuring ARCH=ppc64..."
	@$(ECHO) "#"
	@$(ECHO) "#= = = = = = = = = = = = = = = = = = ="
	@$(ECHO) "#"
	$(SILENT) $(CD) $(MYSQL_SRC_PPC64);\
	CFLAGS="-O3 -fno-omit-frame-pointer $$RC_CFLAGS" \
	CXXFLAGS="-O3 -fno-omit-frame-pointer -felide-constructors -fno-exceptions -fno-rtti $$RC_CFLAGS" \
	LDFLAGS="$$RC_CFLAGS" \
	ac_cv_c_bigendian=yes \
	./configure --target=ppc64-apple-darwin $(CONFIG_OPTS)
	@$(ECHO) "#"
	@$(ECHO) "# `date +%Y/%m/%d\ %H:%M:%S` MySQL: Patching config.h; ARCH=ppc64..."
	@$(ECHO) "#"
	$(SILENT) $(CD) $(MYSQL_SRC_PPC64); $(DO_PATCH_CONFIG_H)
	@$(ECHO) "#"
	@$(ECHO) "# `date +%Y/%m/%d\ %H:%M:%S` MySQL: Patching innobase/ib_config.h; ARCH=ppc64..."
	@$(ECHO) "#"
	$(SILENT) $(CD) $(MYSQL_SRC_PPC64)/innobase; $(DO_PATCH_IBCONFIG_H)

$(MYSQL_SRC_X86_64)/config.status:
	@$(ECHO) "#"
	@$(ECHO) "#= = = = = = = = = = = = = = = = = = ="
	@$(ECHO) "#"
	@$(ECHO) "# `date +%Y/%m/%d\ %H:%M:%S` MySQL: Configuring ARCH=x86_64..."
	@$(ECHO) "#"
	@$(ECHO) "#= = = = = = = = = = = = = = = = = = ="
	@$(ECHO) "#"
	$(SILENT) $(CD) $(MYSQL_SRC_X86_64);\
	CFLAGS="-O3 -fno-omit-frame-pointer $$RC_CFLAGS" \
	CXXFLAGS="-O3 -fno-omit-frame-pointer -felide-constructors -fno-exceptions -fno-rtti $$RC_CFLAGS" \
	LDFLAGS="$$RC_CFLAGS" \
	ac_cv_c_bigendian=yes \
	./configure --target=x86_64-apple-darwin $(CONFIG_OPTS)
	@$(ECHO) "#"
	@$(ECHO) "# `date +%Y/%m/%d\ %H:%M:%S` MySQL: Patching config.h; ARCH=x86_64..."
	@$(ECHO) "#"
	$(SILENT) $(CD) $(MYSQL_SRC_X86_64); $(DO_PATCH_CONFIG_H)
	@$(ECHO) "#"
	@$(ECHO) "# `date +%Y/%m/%d\ %H:%M:%S` MySQL: Patching innobase/ib_config.h; ARCH=x86_64..."
	@$(ECHO) "#"
	$(SILENT) $(CD) $(MYSQL_SRC_X86_64)/innobase; $(DO_PATCH_IBCONFIG_H)

configure: untar $(MYSQL_SRC_PPC)/config.status $(MYSQL_SRC_I386)/config.status $(MYSQL_SRC_PPC64)/config.status $(MYSQL_SRC_X86_64)/config.status

.PHONY: build_ppc build_i386 build_ppc64 build_x86_64

build_ppc:
	@$(ECHO) "#"
	@$(ECHO) "#= = = = = = = = = = = = = = = = = = ="
	@$(ECHO) "#"
	@$(ECHO) "# `date +%Y/%m/%d\ %H:%M:%S` MySQL: Building ARCH=ppc..."
	@$(ECHO) "#"
	@$(ECHO) "#= = = = = = = = = = = = = = = = = = ="
	@$(ECHO) "#"
	$(SILENT) $(CD) $(MYSQL_SRC_PPC); make
	@$(ECHO) "#"
	@$(ECHO) "# `date +%Y/%m/%d\ %H:%M:%S` MySQL: Patching support-files; ARCH=ppc..."
	@$(ECHO) "#"
	$(SILENT) $(CD) $(MYSQL_SRC_PPC)/support-files; $(DO_PATCH_MYHUGE_CNF)
	$(SILENT) $(CD) $(MYSQL_SRC_PPC)/support-files; $(DO_PATCH_MYLARGE_CNF)
	@$(ECHO) "#"
	@$(ECHO) "# `date +%Y/%m/%d\ %H:%M:%S` MySQL: Patching scripts/mysqld_safe; ARCH=ppc..."
	@$(ECHO) "#"
	$(SILENT) $(CD) $(MYSQL_SRC_PPC)/scripts; $(PATCH) -u mysqld_safe ../../mysqld_safe.patch

build_i386:
	@$(ECHO) "#"
	@$(ECHO) "#= = = = = = = = = = = = = = = = = = ="
	@$(ECHO) "#"
	@$(ECHO) "# `date +%Y/%m/%d\ %H:%M:%S` MySQL: Building ARCH=i386..."
	@$(ECHO) "#"
	@$(ECHO) "#= = = = = = = = = = = = = = = = = = ="
	@$(ECHO) "#"
	$(SILENT) $(CD) $(MYSQL_SRC_I386); make

build_ppc64:
	@$(ECHO) "#"
	@$(ECHO) "#= = = = = = = = = = = = = = = = = = ="
	@$(ECHO) "#"
	@$(ECHO) "# `date +%Y/%m/%d\ %H:%M:%S` MySQL: Building ARCH=ppc64..."
	@$(ECHO) "#"
	@$(ECHO) "#= = = = = = = = = = = = = = = = = = ="
	@$(ECHO) "#"
	$(SILENT) $(CD) $(MYSQL_SRC_PPC64); make

build_x86_64:
	@$(ECHO) "#"
	@$(ECHO) "#= = = = = = = = = = = = = = = = = = ="
	@$(ECHO) "#"
	@$(ECHO) "# `date +%Y/%m/%d\ %H:%M:%S` MySQL: Building ARCH=x86_64..."
	@$(ECHO) "#"
	@$(ECHO) "#= = = = = = = = = = = = = = = = = = ="
	@$(ECHO) "#"
	$(SILENT) $(CD) $(MYSQL_SRC_X86_64); make

build: configure build_ppc build_i386 build_ppc64 build_x86_64

# For make install, must set DESTDIR to shadow directory

INSTALL_DEP_DIRS	= \
$(STAGING_DIR)$(VERSIONS_DIR) \
$(STAGING_DIR)$(LICENSE_DIR) \
$(STAGING_DIR)$(LIBEXEC_DIR) \
$(STAGING_DIR_I386) \
$(STAGING_DIR_PPC64) \
$(STAGING_DIR_X86_64)

# NOTE: Non-architecture-specific files are copied from the PPC staging dir
.PHONY: install_ppc install_i386 install_ppc64 install_x86_64

install_ppc:
	@$(ECHO) "#"
	@$(ECHO) "#= = = = = = = = = = = = = = = = = = ="
	@$(ECHO) "#"
	@$(ECHO) "# `date +%Y/%m/%d\ %H:%M:%S` MySQL: Installing ARCH=ppc..."
	@$(ECHO) "#"
	@$(ECHO) "#= = = = = = = = = = = = = = = = = = ="
	@$(ECHO) "#"
	$(SILENT) $(CD) $(MYSQL_SRC_PPC);make install DESTDIR=$(STAGING_DIR)

install_i386:
	@$(ECHO) "#"
	@$(ECHO) "#= = = = = = = = = = = = = = = = = = ="
	@$(ECHO) "#"
	@$(ECHO) "# `date +%Y/%m/%d\ %H:%M:%S` MySQL: Installing ARCH=i386..."
	@$(ECHO) "#"
	@$(ECHO) "#= = = = = = = = = = = = = = = = = = ="
	@$(ECHO) "#"
	$(SILENT) $(CD) $(MYSQL_SRC_I386);make install DESTDIR=$(STAGING_DIR_I386)

install_ppc64:
	@$(ECHO) "#"
	@$(ECHO) "#= = = = = = = = = = = = = = = = = = ="
	@$(ECHO) "#"
	@$(ECHO) "# `date +%Y/%m/%d\ %H:%M:%S` MySQL: Installing ARCH=ppc64..."
	@$(ECHO) "#"
	@$(ECHO) "#= = = = = = = = = = = = = = = = = = ="
	@$(ECHO) "#"
	$(SILENT) $(CD) $(MYSQL_SRC_PPC64);make install DESTDIR=$(STAGING_DIR_PPC64)

install_x86_64:
	@$(ECHO) "#"
	@$(ECHO) "#= = = = = = = = = = = = = = = = = = ="
	@$(ECHO) "#"
	@$(ECHO) "# `date +%Y/%m/%d\ %H:%M:%S` MySQL: Installing ARCH=x86_64..."
	@$(ECHO) "#"
	@$(ECHO) "#= = = = = = = = = = = = = = = = = = ="
	@$(ECHO) "#"
	$(SILENT) $(CD) $(MYSQL_SRC_X86_64);make install DESTDIR=$(STAGING_DIR_X86_64)

install: build $(INSTALL_DEP_DIRS) install_ppc install_i386 install_ppc64 install_x86_64
	@$(ECHO) "#"
	@$(ECHO) "# `date +%Y/%m/%d\ %H:%M:%S` MySQL: Removing achitecture prefixes from installed files..."
	@$(ECHO) "#"
	for i in $(USRBIN_FILES_TO_RENAME); do \
		$(SILENT) $(CD) $(STAGING_USRBIN); $(MV) ppc-apple-darwin-$$i $$i; \
		$(SILENT) $(CD) $(STAGING_USRBIN_I386); $(MV) i386-apple-darwin-$$i $$i; \
		$(SILENT) $(CD) $(STAGING_USRBIN_PPC64); $(MV) ppc64-apple-darwin-$$i $$i; \
		$(SILENT) $(CD) $(STAGING_USRBIN_X86_64); $(MV) x86_64-apple-darwin-$$i $$i; \
	done
	for i in $(USRLIBEXEC_FILES_TO_RENAME); do \
		$(SILENT) $(CD) $(STAGING_USRLIBEXEC); $(MV) ppc-apple-darwin-$$i $$i; \
		$(SILENT) $(CD) $(STAGING_USRLIBEXEC_I386); $(MV) i386-apple-darwin-$$i $$i; \
		$(SILENT) $(CD) $(STAGING_USRLIBEXEC_PPC64); $(MV) ppc64-apple-darwin-$$i $$i; \
		$(SILENT) $(CD) $(STAGING_USRLIBEXEC_X86_64); $(MV) x86_64-apple-darwin-$$i $$i; \
	done
	for i in $(USRMYSQLTEST_FILES_TO_RENAME); do \
		$(SILENT) $(CD) $(STAGING_USRMYSQLTEST); $(MV) ppc-apple-darwin-$$i $$i; \
		$(SILENT) $(CD) $(STAGING_USRMYSQLTEST_I386); $(MV) i386-apple-darwin-$$i $$i; \
		$(SILENT) $(CD) $(STAGING_USRMYSQLTEST_PPC64); $(MV) ppc64-apple-darwin-$$i $$i; \
		$(SILENT) $(CD) $(STAGING_USRMYSQLTEST_X86_64); $(MV) x86_64-apple-darwin-$$i $$i; \
	done
	for i in $(USRSHAREMAN1_FILES_TO_RENAME); do \
		$(SILENT) $(CD) $(STAGING_USRSHAREMAN1); $(MV) ppc-apple-darwin-$$i $$i; \
		$(SILENT) $(CD) $(STAGING_USRSHAREMAN1_I386); $(MV) i386-apple-darwin-$$i $$i; \
		$(SILENT) $(CD) $(STAGING_USRSHAREMAN1_PPC64); $(MV) ppc64-apple-darwin-$$i $$i; \
		$(SILENT) $(CD) $(STAGING_USRSHAREMAN1_X86_64); $(MV) x86_64-apple-darwin-$$i $$i; \
	done
	for i in $(USRSHAREMYSQL_FILES_TO_RENAME); do \
		$(SILENT) $(CD) $(STAGING_USRSHAREMYSQL); $(MV) ppc-apple-darwin-$$i $$i; \
		$(SILENT) $(CD) $(STAGING_USRSHAREMYSQL_I386); $(MV) i386-apple-darwin-$$i $$i; \
		$(SILENT) $(CD) $(STAGING_USRSHAREMYSQL_PPC64); $(MV) ppc64-apple-darwin-$$i $$i; \
		$(SILENT) $(CD) $(STAGING_USRSHAREMYSQL_X86_64); $(MV) x86_64-apple-darwin-$$i $$i; \
	done
	@$(ECHO) "#"
	@$(ECHO) "# `date +%Y/%m/%d\ %H:%M:%S` MySQL: Installing Apple man pages..."
	@$(ECHO) "#"
	$(SILENT) $(CP) mysqlman.1 $(STAGING_DIR)/usr/share/man/man1
	$(SILENT) $(MKDIR) -p -m 755 $(STAGING_DIR)/usr/share/man/man8
	$(SILENT) $(CHOWN) root:wheel $(STAGING_DIR)/usr/share/man/man8
	$(SILENT) $(CP) applemysqlcheckcnf.8 $(STAGING_DIR)/usr/share/man/man8
	@$(ECHO) "#"
	@$(ECHO) "# `date +%Y/%m/%d\ %H:%M:%S` MySQL: Installing version and license info..."
	@$(ECHO) "#"
	$(SILENT) $(INSTALL) -m 444 -o root -g wheel MySQL.plist $(STAGING_DIR)$(VERSIONS_DIR)
	$(SILENT) $(INSTALL) -m 444 -o root -g wheel MySQL.txt $(STAGING_DIR)$(LICENSE_DIR)
	@$(ECHO) "#"
	@$(ECHO) "# `date +%Y/%m/%d\ %H:%M:%S` MySQL: Installing Apple support scripts..."
	@$(ECHO) "#"
	$(SILENT) $(INSTALL) -m 755 -o root -g wheel applemysqlcheckcnf $(STAGING_DIR)$(LIBEXEC_DIR)
	@$(ECHO) "#"
	@$(ECHO) "#= = = = = = = = = = = = = = = = = = ="
	@$(ECHO) "#"
	@$(ECHO) "# `date +%Y/%m/%d\ %H:%M:%S` MySQL: Fixing up mysql, staging from $(STAGING_DIR)..."
	@$(ECHO) "#"
	@$(ECHO) "#= = = = = = = = = = = = = = = = = = ="
	@$(ECHO) "#"
	$(SILENT) -$(MV) $(STAGING_DIR)/$(BUILD_DIR)/mysql-test $(STAGING_DIR)/$(MYSQL_SHARE_DIR)
	for i in $(FILES_TO_REMOVE); do \
		rm -r -f $(STAGING_DIR)/$$i; \
	done
	for i in $(MANPAGE_SYMLINKS); do \
		ln  $(STAGING_DIR)/$(SHARE_DIR)/man/man1/mysqlman.1 $(STAGING_DIR)/$(SHARE_DIR)/man/man1/$$i.1; \
	done
	@$(ECHO) "#"
	@$(ECHO) "# `date +%Y/%m/%d\ %H:%M:%S` MySQL: Creating universal binaries..."
	@$(ECHO) "#"
	for i in $(USRBIN_FILES_TO_LIPO); do \
		$(SILENT) $(CD) $(STAGING_USRBIN); $(MV) $$i $$i.univ; \
		$(SILENT) $(CD) $(STAGING_USRBIN); $(LIPO) -thin ppc $$i.univ -output $$i.thin.ppc; \
		$(SILENT) $(CD) $(STAGING_USRBIN_I386); $(LIPO) -thin i386 $$i -output $$i.thin.i386; \
		$(SILENT) $(CD) $(STAGING_USRBIN_PPC64); $(LIPO) -thin ppc64 $$i -output $$i.thin.ppc64; \
		$(SILENT) $(CD) $(STAGING_USRBIN_X86_64); $(LIPO) -thin x86_64 $$i -output $$i.thin.x86_64; \
		$(SILENT) $(CD) $(STAGING_USRBIN); $(LIPO) -create -arch ppc $$i.thin.ppc \
		                                           -arch i386 $(STAGING_USRBIN_I386)/$$i.thin.i386 \
		                                           -arch ppc64 $(STAGING_USRBIN_PPC64)/$$i.thin.ppc64 \
		                                           -arch x86_64 $(STAGING_USRBIN_X86_64)/$$i.thin.x86_64 \
												   -output $$i; \
		$(SILENT) $(CD) $(STAGING_USRBIN); -$(STRIP) -S $$i > /dev/null 2>&1; \
		$(SILENT) $(CD) $(STAGING_USRBIN); $(RM) -rf $$i.univ $$i.thin.ppc; \
		$(SILENT) $(CD) $(STAGING_USRBIN); $(LIPO) -info $$i; \
	done
	for i in $(USRLIBMYSQL_FILES_TO_LIPO); do \
		$(SILENT) $(CD) $(STAGING_USRLIBMYSQL); $(MV) $$i $$i.univ; \
		$(SILENT) $(CD) $(STAGING_USRLIBMYSQL); $(LIPO) -thin ppc $$i.univ -output $$i.thin.ppc; \
		$(SILENT) $(CD) $(STAGING_USRLIBMYSQL_I386); $(LIPO) -thin i386 $$i -output $$i.thin.i386; \
		$(SILENT) $(CD) $(STAGING_USRLIBMYSQL_PPC64); $(LIPO) -thin ppc64 $$i -output $$i.thin.ppc64; \
		$(SILENT) $(CD) $(STAGING_USRLIBMYSQL_X86_64); $(LIPO) -thin x86_64 $$i -output $$i.thin.x86_64; \
		$(SILENT) $(CD) $(STAGING_USRLIBMYSQL); $(LIPO) -create -arch ppc $$i.thin.ppc \
		                                                -arch i386 $(STAGING_USRLIBMYSQL_I386)/$$i.thin.i386 \
		                                                -arch ppc64 $(STAGING_USRLIBMYSQL_PPC64)/$$i.thin.ppc64 \
		                                                -arch x86_64 $(STAGING_USRLIBMYSQL_X86_64)/$$i.thin.x86_64 \
												        -output $$i; \
		$(SILENT) $(CD) $(STAGING_USRLIBMYSQL); -$(STRIP) -S $$i > /dev/null 2>&1; \
		$(SILENT) $(CD) $(STAGING_USRLIBMYSQL); $(RM) -rf $$i.univ $$i.thin.ppc; \
		$(SILENT) $(CD) $(STAGING_USRLIBMYSQL); $(LIPO) -info $$i; \
	done
	for i in $(USRLIBEXEC_FILES_TO_LIPO); do \
		$(SILENT) $(CD) $(STAGING_USRLIBEXEC); $(MV) $$i $$i.univ; \
		$(SILENT) $(CD) $(STAGING_USRLIBEXEC); $(LIPO) -thin ppc $$i.univ -output $$i.thin.ppc; \
		$(SILENT) $(CD) $(STAGING_USRLIBEXEC_I386); $(LIPO) -thin i386 $$i -output $$i.thin.i386; \
		$(SILENT) $(CD) $(STAGING_USRLIBEXEC_PPC64); $(LIPO) -thin ppc64 $$i -output $$i.thin.ppc64; \
		$(SILENT) $(CD) $(STAGING_USRLIBEXEC_X86_64); $(LIPO) -thin x86_64 $$i -output $$i.thin.x86_64; \
		$(SILENT) $(CD) $(STAGING_USRLIBEXEC); $(LIPO) -create -arch ppc $$i.thin.ppc \
			                                           -arch i386 $(STAGING_USRLIBEXEC_I386)/$$i.thin.i386 \
			                                           -arch ppc64 $(STAGING_USRLIBEXEC_PPC64)/$$i.thin.ppc64 \
			                                           -arch x86_64 $(STAGING_USRLIBEXEC_X86_64)/$$i.thin.x86_64 \
			                                           -output $$i; \
		$(SILENT) $(CD) $(STAGING_USRLIBEXEC); -$(STRIP) -S $$i > /dev/null 2>&1; \
		$(SILENT) $(CD) $(STAGING_USRLIBEXEC); $(RM) -rf $$i.univ $$i.thin.ppc; \
		$(SILENT) $(CD) $(STAGING_USRLIBEXEC); $(LIPO) -info $$i; \
	done
	@$(ECHO) "#"
	@$(ECHO) "# `date +%Y/%m/%d\ %H:%M:%S` MySQL: Fixing mysql_config script..."
	@$(ECHO) "#"
	$(SILENT) $(CP) $(STAGING_USRBIN)/mysql_config $(STAGING_USRBIN)/mysql_config-tmp
	$(SILENT) $(SED) < $(STAGING_USRBIN)/mysql_config-tmp > $(STAGING_USRBIN)/mysql_config -e 's%-arch i386%%' -e 's%-arch ppc%%'
	$(SILENT) $(RM) -r -f $(STAGING_USRBIN)/mysql_config-tmp
	@$(ECHO) "#"
	@$(ECHO) "# `date +%Y/%m/%d\ %H:%M:%S` MySQL: Copying staging dir to DSTROOT..."
	@$(ECHO) "#"
	$(SILENT) $(RM) -rf $(STAGING_DIR)/archs
	$(SILENT) $(CHOWN) -R root:wheel $(STAGING_DIR)/usr/
	$(SILENT) $(DITTO) $(STAGING_DIR) $(DSTROOT)
	$(SILENT) $(RM) -r -f $(STAGING_DIR)
	@$(ECHO) "#"
	@$(ECHO) "#"
	@$(ECHO) "### The latest information about MySQL is available"
	@$(ECHO) "### on the web at http://www.mysql.com."
	@$(ECHO) "###"
	@$(ECHO) "### Use MySQL Manager app to initialize MySQL database."
	@$(ECHO) "#"
	@$(ECHO) "#= = = = = = = = = = = = = = = = = = ="
	@$(ECHO) "#"
	@$(ECHO) "# `date +%Y/%m/%d\ %H:%M:%S` MySQL: BUILD COMPLETE."
	@$(ECHO) "#"
	@$(ECHO) "#= = = = = = = = = = = = = = = = = = ="

$(BUILD_DIR):
	$(SILENT) $(MKDIRS) $@

$(STAGING_DIR_X86_64):
	$(SILENT) $(MKDIRS) $@

$(STAGING_DIR_PPC64):
	$(SILENT) $(MKDIRS) $@

$(STAGING_DIR_I386):
	$(SILENT) $(MKDIRS) $@

$(STAGING_DIR)$(BUILD_DIR):
	$(SILENT) $(MKDIRS) $@

$(STAGING_DIR)$(VERSIONS_DIR):
	$(SILENT) $(MKDIRS) $@

$(STAGING_DIR)$(LICENSE_DIR):
	$(SILENT) $(MKDIRS) $@

$(STAGING_DIR)$(LIBEXEC_DIR):
	$(SILENT) $(MKDIRS) $@

#$(STAGING_DIR):
#	$(SILENT) $(MKDIRS) $@

$(DSTROOT):
	$(SILENT) $(MKDIRS) $@

$(OBJROOT):
	$(SILENT) $(MKDIRS) $@

