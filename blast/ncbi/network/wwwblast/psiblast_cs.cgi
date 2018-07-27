#!/bin/csh -f

#
# $Id: psiblast_cs.cgi,v 1.1 2002/08/06 19:03:52 dondosha Exp $
#

echo "Content-type: text/html"
echo ""

#setenv WWW_ROOT_PATH /blast
#setenv DEBUG_COMMAND_LINE TRUE
setenv BLASTDB db

./psiblast_cs.REAL
