#! /bin/csh

echo "deny	*=bonnie,clyde	128.12.6.0 255.255.255.0 : /usr/ucb/mail -s 'Rejected SOCKS access by %u' $USER" >! junk.conf
echo 'permit	*=ylee	128.12.6.4 255.255.255.255 137.12.0.0 255.255.0.0' >> junk.conf
echo 'deny	128.12.0.0 255.255.0.0 eq telnet' >> junk.conf
echo 'permit	128.12.0.0 255.255.0.0' >> junk.conf
echo "#NO_IDENTD: /usr/ucb/mail -s 'please run identd on %A' $USER" >> junk.conf
echo "#BAD_ID: /usr/ucb/mail -s '%U pretends to be %u on %A' $USER" >> junk.conf
# 
set prog=./test_sockd_conf

strings $prog |egrep  -s route_file
if ( $status != 0 ) then
	set aa=1
	set opt=""
else
	set aa=0
	set opt = (-R junk.route)
	echo "129.10.1.2	10.0.0.0 255.0.0.0" >! junk.route
	echo "129.10.7.10	137.12.0.0 255.255.0.0" >> junk.route
	echo "129.10.254.1	0.0.0.0 0.0.0.0" >> junk.route
endif

set a=($prog $opt -C junk.conf clyde 128.12.6.34 112.3.24.1 ftp)
echo $a
$a
echo '-------------- should produce:'
echo "USER:clyde,  SRC:128.12.6.34,  DST:112.3.24.1, PORT:21"
echo "Line 1: deny	*=bonnie,clyde	128.12.6.0 255.255.255.0 : /usr/ucb/mail -s 'Rejected SOCKS access by %u' $USER"
echo "A mail message is sent to $USER with the subject line 'Rejected SOCKS"
echo "access by clyde'."
echo "================================================"

set a=($prog $opt -C junk.conf ylee 128.12.6.4 137.12.4.15 telnet)
echo $a
$a
echo '-------------- should produce:'
echo "USER:ylee,  SRC:128.12.6.4,  DST:137.12.4.15, PORT:23"
echo 'Line 2: permit	*=ylee	128.12.6.4 255.255.255.255 137.12.0.0 255.255.0.0'
if ( $aa == 0 ) then
	echo "==== Checking routing file (junk.route)..."
	echo "Line 2: 129.10.7.10	137.12.0.0 255.255.0.0"
endif
echo "================================================"

set a=($prog $opt -C junk.conf jane 128.12.2.13 137.12.4.15 telnet)
echo $a
$a
echo '-------------- should produce:'
echo "USER:jane,  SRC:128.12.2.13,  DST:137.12.4.15, PORT:23"
echo "Line 3: deny    128.12.0.0 255.255.0.0 eq telnet"
echo "================================================"

set a=($prog $opt -C junk.conf jim 128.12.6.4 126.87.13.2 telnet)
echo $a
$a
echo '-------------- should produce:'
echo "USER:jim,  SRC:128.12.6.4,  DST:126.87.13.2, PORT:23"
echo "Line 3: deny    128.12.0.0 255.255.0.0 eq telnet"
echo "================================================"

set a=($prog $opt -C junk.conf ylee 128.12.6.4 126.87.13.2 ftp)
echo $a
$a
echo '-------------- should produce:'
echo "USER:ylee,  SRC:128.12.6.4,  DST:126.87.13.2, PORT:21"
echo "Line 4: permit  128.12.0.0 255.255.0.0"
if ( $aa == 0 ) then
	echo "==== Checking routing file (junk.route)..."
	echo "Line 3: 129.10.254.1	0.0.0.0 0.0.0.0"
endif
echo "================================================"

set a=($prog $opt -C junk.conf sam 128.12.36.7 10.53.23.1 70)
echo $a
$a
echo '-------------- should produce:'
echo "USER:sam,  SRC:128.12.36.7,  DST:10.53.23.1, PORT:70"
echo "Line 4: permit  128.12.0.0 255.255.0.0"
if ( $aa == 0 ) then
	echo "==== Checking routing file (junk.route)..."
	echo "Line 1: 129.10.1.2	10.0.0.0 255.0.0.0"
endif
echo "================================================"

set a=($prog $opt -C junk.conf don 23.2.6.127 10.53.23.1 70)
echo $a
$a
echo '-------------- should produce:'
echo "USER:don,  SRC:23.2.6.127,  DST:10.53.23.1, PORT:70"
echo "*** No match with any line. Access denied."
echo "================================================"

set a=($prog $opt -C junk.conf clyde 128.12.1.62 112.3.24.1 ftp)
echo $a
$a
echo '-------------- should produce:'
echo "USER:clyde,  SRC:128.12.1.62,  DST:112.3.24.1, PORT:21"
echo "Line 4: permit  128.12.0.0 255.255.0.0"
if ( $aa == 0 ) then
	echo "==== Checking routing file (junk.route)..."
	echo "Line 3: 129.10.254.1	0.0.0.0 0.0.0.0"
endif
echo "================================================"

set a=($prog $opt -C junk.conf -I joe 128.12.6.4 126.87.13.2 ftp)
echo $a
$a
echo '-------------- should produce:'
echo "USER:joe,  SRC:128.12.6.4,  DST:126.87.13.2, PORT:21"
echo "Line 4: permit  128.12.0.0 255.255.0.0"
echo "Line 5: #NO_IDENTD: /usr/ucb/mail -s 'please run identd on %a' $USER"
echo "Access denied: cannot verify user-id."
echo "A mail message is sent to $USER of the host 128.12.6.4 with the"
echo "subject line 'please run identd on 128.12.6.4'."
echo "================================================"

set a=($prog $opt -C junk.conf -i joe 128.12.6.4 126.87.13.2 ftp)
echo $a
$a
echo '-------------- should produce:'
echo "USER:joe,  SRC:128.12.6.4,  DST:126.87.13.2, PORT:21"
echo "Line 4: permit          128.12.0.0 0.0.255.255"
echo "Line 5: #NO_IDENTD: /usr/ucb/mail -s 'please run identd on %a' $USER"
if ( $aa == 0 ) then
	echo "==== Checking routing file (junk.route)..."
	echo "Line 3: 129.10.254.1	0.0.0.0 0.0.0.0"
endif
echo "A mail message is sent to $USER of the host 128.12.6.4 with the"
echo "subject line 'please run identd on 128.12.6.4'."
echo "================================================"

set a=($prog $opt -C junk.conf -i jim 128.12.6.4 126.87.13.2 telnet)
echo $a
$a
echo '-------------- should produce:'
echo "USER:jim,  SRC:128.12.6.4,  DST:126.87.13.2, PORT:23"
#echo "Line 5: #NO_IDENTD: /usr/ucb/mail -s 'please run identd on %a' $USER"
echo "Line 3: deny    128.12.0.0 0.0.255.255 eq telnet"
echo "A mail message is sent to $USER of the host 128.12.6.4 with the"
echo "subject line 'please run identd on 128.12.6.4'."
echo "================================================"

set a=($prog $opt -C junk.conf -B joe 128.12.6.4 126.87.13.2 ftp)
echo $a
$a
echo '-------------- should produce:'
echo "USER:joe,  SRC:128.12.6.4,  DST:126.87.13.2, PORT:21"
echo "Line 4: permit  128.12.0.0 255.255.0.0"
echo "Line 6: #BAD_ID: /usr/ucb/mail -s '%U pretends to be %u on %A' $USER"
echo "Access denied: bad user-id."
echo "A mail message is sent to $USER with the"
echo "subject line 'unknown pretends to be joe on 128.12.6.4'."
echo "================================================"
