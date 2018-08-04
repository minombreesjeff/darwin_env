#!./perl

# $RCSfile: if.t,v $$Revision: 1.4 $$Date: 2003/05/20 22:53:55 $

print "1..2\n";

# first test to see if we can run the tests.

$x = 'test';
if ($x eq $x) { print "ok 1\n"; } else { print "not ok 1\n";}
if ($x ne $x) { print "not ok 2\n"; } else { print "ok 2\n";}
