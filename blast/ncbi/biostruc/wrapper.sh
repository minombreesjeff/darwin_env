#! /bin/sh
#
# $Id: wrapper.sh,v 6.11 1999/06/09 04:07:16 kimelman Exp $
#
# this is CGI handler wrapper. It works as a membrane between httpd and actual
# cgi program and allow to run new technological version of such program in 
# parallel to old one and compare results in "real life" condition. 
#
# $Log: wrapper.sh,v $
# Revision 6.11  1999/06/09 04:07:16  kimelman
# cosmetics
#
# Revision 6.10  1999/04/19 17:32:30  kimelman
# new wrapper
#
# Revision 6.9  1998/10/07 21:58:16  kimelman
# code cleaned & made easy to read.
# check for directory access permission added
# binaries expected names changed
# this version of script is expected to work on public servers
#
# Revision 6.8  1998/06/09 19:16:07  kimelman
# extra bracket removed.
#
# Revision 6.7  1998/06/09 18:50:35  kimelman
# permissions problems workaround added to wrapper
#
#

progname=$0
options="$*"
progdir=`dirname $progname`

. $progdir/wrapper_lib.sh

# redefine the default problem report recipient (victim :) )
# THEvictim="kimelman zimmerma"

# the following line will collect execution time statistics in given file 
# do_timing               # find sutable place (default is ./stats.$basename or log/stats.$basename)
# do_timing ./stats       # coolect stats in file '../stats'

# run EXECs in parallel - diff results , gather time statistics, report deltas to victim
# default mode : EXECs checked for $progname.REAL $progname.NEW $progname.OLD
run_all  
# here is a smarter way
#run_all ${progdir}/${basename}.FF ${progdir}/${basename}.PS ${progdir}/${basename}.OLD

#
# The end
#
exit 0
