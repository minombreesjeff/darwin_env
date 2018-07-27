# Directory into which the man pages are to be installed.
MAN_DEST_DIR = /usr/local/man

# The 'install' command is assumed to be the BSD variety (using -m to
# set the file mode). If the default 'install' on your system doesn't
# do that, you have to either specify an alternative one in the line below
# (e.g., /usr/ucb/install) or modify the other Makefile.
INSTALL= install
# UnixWare should use:
#INSTALL=/usr/ucb/install
# IRIX should use:
#INSTALL=bsdinstall
#========================================================

install:
	for i in *.1; do  \
		$(INSTALL) -m 444 $$i $(MAN_DEST_DIR)/man1 ;\
		done
	for i in *.5; do  \
		$(INSTALL) -m 444 $$i $(MAN_DEST_DIR)/man5 ;\
		done
	for i in *.8; do  \
		$(INSTALL) -m 444 $$i $(MAN_DEST_DIR)/man8 ;\
		done

