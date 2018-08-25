#!/usr/bin/perl -w
use Test::More;
use strict;

BEGIN { plan tests => 12 }

use Mac::Memory;

SKIP: {
#	skip "Mac::Memory", 12;

# test any of the many other memory functions, seldom-used though they are?

	ok(StackSpace(), 'StackSpace');
	ok(FreeMem(), 'FreeMem');
	ok(MaxMem(), 'MaxMem');

	ok(my $h = Handle->new('xyzzy'),	'new handle');
	is($h->get, 'xyzzy',			'get handle');
	is($h->size, 5,				'handle size');
	is($h->get(2, 2), 'zz',			'get handle portion');

	ok($h->append(' wokka wokka'),		'append to handle');
	is($h->get, 'xyzzy wokka wokka',	'get handle');
	is($h->size, 17,			'handle size');
	is($h->get(2, 2), 'zz',			'get handle portion');

	ok(!$h->dispose,			'dispose handle');
}


__END__

Handle->open() doesn't work.  Probably never will again.

$h  = new Handle("xyzzy");
$r = $h->open("r");

# should be "xyzzy"
print <$r>, "\n";

$w = $h->open("w");
print $w "wysiwyg";

# should be "wysiwyg"
print $h->get, "\n";

truncate $w, 0;
# should be " (0)"
printf "%s (%d)\n", $h->get, $h->size;
# should be blank
print <$r>, "\n";
# should be " (0)"
undef $w;
printf "%s (%d)\n", $h->get, $h->size;

truncate $r, 0;
# should be " (0)"
printf "%s (%d)\n", $h->get, $h->size;
