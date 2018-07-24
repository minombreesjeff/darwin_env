#!/usr/bin/perl

use Foundation;
use strict;


my %preserve = ();

foreach my $arg (@ARGV) {
    print STDERR "$arg\n";
    my $d = NSDictionary->dictionaryWithContentsOfFile_($arg);
    die "Can't load $arg" unless $d and $$d;

    my $e = $d->allKeys()->objectEnumerator();
    my $o;
    while ($o = $e->nextObject() and $$o) {
	my $d2 = $d->objectForKey_($o);
	my $e2 = $d2->allKeys()->objectEnumerator();

	my $o2;
	while ($o2 = $e2->nextObject() and $$o2) {
	    my $v = $d2->objectForKey_($o2);
	    printf STDERR "%s -> %s\n", $o2->UTF8String(), $v->UTF8String();
	    $preserve{$o2->UTF8String()} = $v->UTF8String();
	}
    }
    
}


printf("const char *preserve_boot_args[] = {\n");

foreach my $key (sort keys %preserve) {
    printf("\t\"%s\",\t/* %s */\n", $key, $preserve{$key});
}

printf("\tNULL\n");
printf("};\n");
