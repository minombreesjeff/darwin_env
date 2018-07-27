#!/bin/csh -f

#
# $Id: blast.cgi,v 1.1 2002/08/06 19:03:51 dondosha Exp $
#

echo "Content-type: text/html"
echo ""

#setenv DEBUG_COMMAND_LINE TRUE
setenv BLASTDB db

./blast.REAL
