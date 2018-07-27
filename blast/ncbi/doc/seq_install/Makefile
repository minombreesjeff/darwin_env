STYLESHEET=main.xslt
XSL=xsl

XMLFILES=					\
	ftp_example.xml				\
	seq_download.xml			\
	seq_ftp_download.xml			\
	seq_mac_download.xml			\
	seq_trouble.xml				\
	seq_unix_download.xml			\
	seq_win_download.xml

HTMLFILES=$(XMLFILES:.xml=.html)
CSSFILES=ncbi_sequin.css $(XMLFILES:.xml=.css)
GRAPHICS=spacer10.GIF left.GIF bg.gif sequin-icon-win.gif
JSFILES=sniff.js
STYLESHEETS=main.xslt

%.html:	%.xml
	$(XSL) -XSL $(STYLESHEET) -IN $? -OUT $@

all:	$(STYLESHEETS) $(HTMLFILES)

listfiles:
	-@echo $(HTMLFILES) $(CSSFILES) $(GRAPHICS)

clean:
	rm -f $(HTMLFILES) *~

install:	all
	scp *.html *.css *.gif *.js *.GIF mjohnson@graceland:html/seqhelp
