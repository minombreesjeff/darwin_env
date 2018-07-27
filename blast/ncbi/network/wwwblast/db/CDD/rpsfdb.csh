#!/bin/csh -f
#
# $Id: rpsfdb.csh,v 1.1 2002/08/06 19:04:05 dondosha Exp $
#
# Script for creating BLAST database from Impala *.sn file
# Author: Sergei Shavirin
#
# Usage: rpsfdb.csh <name.sn>
#
# Suffix "sn" will be striped from input filename. All files will
# be copied into file without suffix and BLAST database will be created 
# with this name
#
set Bname = `echo $1 | awk -F. '{print $1}'`
#
cat `cat $1` > $Bname
formatdb -i $Bname
