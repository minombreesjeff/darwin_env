#!/bin/csh -f

#
# $Id: psiblast.cgi,v 1.1 2002/08/06 19:03:51 dondosha Exp $
#

echo "Content-type: text/html"
echo ""

#setenv WWW_ROOT_PATH /blast
#setenv DEBUG_COMMAND_LINE TRUE
setenv BLASTDB db

./psiblast.REAL
