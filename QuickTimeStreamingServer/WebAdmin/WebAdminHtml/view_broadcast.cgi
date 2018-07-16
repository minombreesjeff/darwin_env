#!/usr/bin/perl

use CGI;

$q = new CGI;

print "HTTP/1.0 200 OK\r\nContent-Type: video/quicktime\r\n\r\n";
$url = $q->param('url');
print "rtsptext\rrtsp://$url";
exit 0;