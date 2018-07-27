#! /bin/sh
# $Id: fwd_check.sh,v 1.5 2001/12/12 20:36:26 beloslyu Exp $
# Author:  Denis Vakatov (vakatov@ncbi.nlm.nih.gov)
#
# Check for the status of FWDAEMON on the dispatching NCBI servers


delay_sec="$1"
delay_sec=${delay_sec:="10"}

cat <<EOF
http://www.ncbi.nlm.nih.gov/cpp/network/firewall.html

Checking connection to the NCBI Firewall Daemons:
EOF

{
cat <<EOF
130.14.22.1   5853
130.14.22.2   5859
130.14.22.8   5840
130.14.22.30  5810
130.14.22.31  5812
130.14.22.32  5811
130.14.22.12  5845
EOF
} |
while read x_host x_port ; do
    ( echo x ; sleep $delay_sec ; ) | telnet $x_host $x_port 2>&1 |
    grep 'NCBI Firewall Daemon:  Invalid ticket\. Connection closed\.' > /dev/null
    if test $? -eq 0 ; then
        echo "$x_host:$x_port   OK"
    else
        echo "$x_host:$x_port   FAILED  ( telnet $x_host $x_port )"
    fi
done
