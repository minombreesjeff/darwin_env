use Config;
use Cwd;
use ExtUtils::MakeMaker;
use File::Basename;
use File::Spec::Functions qw(:DEFAULT splitdir);
use Pod::Select;

use strict;
use vars qw($MOD $XS $PM $POD $NAME $C %ARGS);

sub domakefile {
	# extra cleanup stuff
	if ($^O eq 'MacOS' && $] < 5.8) {  # old makemaker syntax?
		$ARGS{'TYPEMAPS'}	= join ' ', @{$ARGS{'TYPEMAPS'}};
	}
	WriteMakefile(%ARGS);

	undef $C;
	undef $MOD;
	undef $XS;
	undef $PM;
	undef $POD;
	undef $NAME;
	undef %ARGS;
}

$MOD  ||= (splitdir(cwd()))[-1];
$XS   ||= "$MOD.xs";
($C ||= $XS) =~ s/\.xs$/.c/;
$PM   ||= "$MOD.pm";
$POD  ||= "$MOD.pod";
$NAME ||= "Mac::$MOD";

%ARGS = (
	'NAME'			=> $NAME,
	'VERSION_FROM'		=> $PM,
	'LINKTYPE'		=> 'static dynamic',
	'XSPROTOARG'		=> '-noprototypes', 		# XXX remove later?
	'NO_META'		=> 1,
);

if ($^O eq 'darwin') {
	$ARGS{'INC'}		= '-I/Developer/Headers/FlatCarbon/';
	$ARGS{'LDDLFLAGS'}	= '-bundle -flat_namespace -undefined suppress -framework Carbon';
	$ARGS{'CCFLAGS'}	= $Config{ccflags} . ' -fpascal-strings';
#	$ARGS{'LDDLFLAGS'}	= '-dynamiclib -prebind -flat_namespace -undefined suppress -framework Carbon';
#	$ARGS{'DLEXT'}          = 'dylib';
	$ARGS{'depend'}{$C}     = catfile(updir(), 'Carbon.h');
}

# let's make a new .pod with the right POD from .pm and .xs
if ($^O ne 'MacOS' && -e $XS) {
	podselect({-output => $POD}, $XS);
	my $xs = do { local $/; open my $fh, $POD or die $!; <$fh> };
	if ($xs) {
		podselect({-output => $POD}, $PM);
		my $pm = do { local $/; open my $fh, $POD or die $!; <$fh> };
		$pm =~ s/=include $XS/$xs/;
		open my $fh, "> $POD" or die $!;
		print $fh $pm;
		$ARGS{'MAN3PODS'} = { $POD =>
			File::Spec->catfile("\$(INST_MAN3DIR)", "$NAME.\$(MAN3EXT)")
		};
		$ARGS{'clean'} = { FILES => $POD };
	} else {
		unlink $POD;
	}
}

# use the right xsubpp for this perl
if ($^O ne 'MacOS') {
	*MY::test = sub { "test ::\n\t\@\$(NOOP)" };

	*MY::tool_xsubpp = sub {
		my($self) = shift;

		return '' unless $self->needs_linking;

		my($xsdir)  = File::Spec->catdir('..', 'xsubpps');

		my(@tmdeps) = File::Spec->catdir(
			$self->{PERL_LIB}, 'ExtUtils', 'typemap'
		);

		if ($self->{TYPEMAPS}){
			foreach my $typemap (@{$self->{TYPEMAPS}}){
				if (! -f  $typemap){
					warn "Typemap $typemap not found.\n";
				} else {
					push(@tmdeps, $typemap);
				}
			}
		}

		push @tmdeps, 'typemap' if -f 'typemap';
		my @tmargs = map "-typemap $_", @tmdeps;

		unshift @tmargs, $self->{XSOPT} if exists $self->{XSOPT};

		my $xsubpp = $] >= 5.008 ? 'xsubpp-5.8.0' : 'xsubpp-5.6.1';

		$self->{XSPROTOARG} ||= '';

		return <<"HERE";
XSUBPPDIR = $xsdir
XSUBPP = \$(XSUBPPDIR)/$xsubpp
XSPROTOARG = $self->{XSPROTOARG}
XSUBPPDEPS = @tmdeps \$(XSUBPP)
XSUBPPARGS = @tmargs
XSUBPP_EXTRA_ARGS =
HERE
	};
}
