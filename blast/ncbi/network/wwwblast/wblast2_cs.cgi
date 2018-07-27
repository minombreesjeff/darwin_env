#!/bin/csh -f

#
# $Id: wblast2_cs.cgi,v 1.1 2002/08/06 19:03:52 dondosha Exp $
#

echo "Content-type: text/html"
echo ""

#setenv DEBUG_COMMAND_LINE TRUE
setenv BLASTDB db

./wblast2_cs.REAL
