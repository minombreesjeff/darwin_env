#!/usr/bin/perl
#
# Fix up the version numbers on all subsidiary modules.  Meant for
# internal developer use.
#
# $Id: set-version.pl,v 1.3 2004/09/15 14:01:55 dfs Exp $

use strict;

sub fix_version($$) {
    my($fname, $version) = @_;

    my $didit = 0;

    # Ignore main file!
    if ($fname eq "lib/MIME/Tools.pm") {
	return;
    }

    open(IN, "<$fname") or die("Can't open $fname for input: $!");
    open(OUT, ">$fname.new") or die("Can't open $fname.new for output: $!");
    while(<IN>) {
	if (/^\$VERSION =/) {
	    if ($_ eq "\$VERSION = \"$version\";\n") {
		print STDERR "No need to update $fname -- already at $version\n";
		print OUT;
	    } else {
		$didit = 1;
		print STDERR "Updated VERSION in $fname\n";
		print OUT "\$VERSION = \"$version\";\n";
	    }
	} else {
	    print OUT;
	}
    }
    close(IN);
    close(OUT);
    if ($didit) {
	rename("$fname.new", "$fname") or die("Can't rename $fname.new to $fname: $!");
    } else {
	unlink("$fname.new");
    }
}

do './lib/MIME/Tools.pm';
my $version = MIME::Tools::version();

open(FIND, "find lib/MIME -name '*.pm'|");
while(<FIND>) {
    chomp;
    fix_version($_, $version);
}
close(FIND);
