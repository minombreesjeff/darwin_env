# $Id: import.t,v 1.2 2003/03/20 15:17:37 petdance Exp $
use strict;

use Test::More tests => 6;

BEGIN {
    use_ok( 'Mac::Errors', qw( $MacError %MacErrors fnfErr ) );
}

my $count = keys %MacErrors;
ok( $count > 0, 'There are at least some errors' );



my $err = -43;
$! = $err;

my $error = $MacErrors{$err};

cmp_ok( $error->number, '==', $err );
cmp_ok( $error->number, '==', fnfErr() );
is( $error->description, $MacError );
is( $error->symbol, "fnfErr" );
