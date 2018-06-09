# $OpenLDAP: pkg/ldap/build/man.mk,v 1.27.2.5 2006/01/03 22:16:01 kurt Exp $
## This work is part of OpenLDAP Software <http://www.openldap.org/>.
##
## Copyright 1998-2006 The OpenLDAP Foundation.
## All rights reserved.
##
## Redistribution and use in source and binary forms, with or without
## modification, are permitted only as authorized by the OpenLDAP
## Public License.
##
## A copy of this license is available in the file LICENSE in the
## top-level directory of the distribution or, alternatively, at
## <http://www.OpenLDAP.org/license.html>.
##---------------------------------------------------------------------------
#
# Makefile Template for Manual Pages
#

MANDIR=$(mandir)/man$(MANSECT)
TMP_SUFFIX=tmp

all-common:
	PAGES=`cd $(srcdir); echo *.$(MANSECT)`; \
	for page in $$PAGES; do \
		$(SED) -e "s%LDVERSION%$(VERSION)%" \
			-e 's%ETCDIR%/etc/openldap%' \
			-e 's%LOCALSTATEDIR%/var/db/openldap%' \
			-e 's%SYSCONFDIR%/etc/openldap%' \
			-e 's%DATADIR%$(datadir)%' \
			-e 's%SBINDIR%$(sbindir)%' \
			-e 's%BINDIR%$(bindir)%' \
			-e 's%LIBDIR%$(libdir)%' \
			-e 's%LIBEXECDIR%/usr/libexec%' \
			-e 's%RELEASEDATE%$(RELEASEDATE)%' \
			$(srcdir)/$$page > $$page.$(TMP_SUFFIX); \
	done

install-common:
	-$(MKDIR) $(DESTDIR)$(MANDIR)
	PAGES=`cd $(srcdir); echo *.$(MANSECT)`; \
	for page in $$PAGES; do \
		echo "installing $$page in $(DESTDIR)$(MANDIR)"; \
		$(RM) $(DESTDIR)$(MANDIR)/$$page; \
		$(INSTALL) $(INSTALLFLAGS) -m 644 $$page.$(TMP_SUFFIX) $(DESTDIR)$(MANDIR)/$$page; \
		if test -f "$(srcdir)/$$page.links" ; then \
			for link in `$(CAT) $(srcdir)/$$page.links`; do \
				echo "installing $$link in $(DESTDIR)$(MANDIR) as link to $$page"; \
				$(RM) $(DESTDIR)$(MANDIR)/$$link ; \
				$(LN_S) $(DESTDIR)$(MANDIR)/$$page $(DESTDIR)$(MANDIR)/$$link; \
			done; \
		fi; \
	done

clean-common:   FORCE
	$(RM) *.tmp all-common

Makefile: $(top_srcdir)/build/man.mk
