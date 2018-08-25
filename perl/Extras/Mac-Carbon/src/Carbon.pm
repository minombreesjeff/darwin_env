=head1 NAME

Mac::Carbon - Access to Mac OS Carbon API

=head1 SYNOPSIS

	use Mac::Carbon;
	use Mac::Carbon qw(:files :morefiles);


=head1 DESCRIPTION

This module provides documentation of the Mac::Carbon modules, and
acts as a frontend to them.

Mac::Carbon is a collection of perl modules for accessing the Carbon API
under Mac OS X.  It is a port of the Toolbox modules written by Matthias
Neeracher for MacPerl.

This module will load in all the Carbon modules, and export all of the
functions, constants, and other variables.  An export tag is set up for
each module, so they may be selected individually.

This module exists primarily because in Mac OS X, all the Carbon
functions are imported into a C program with a single header,
Carbon.h, so Mac OS X users may prefer to load in the entire Carbon
API with a single module.

For detailed information on the Carbon API (highly recommended, as
a familiarity with Carbon is assumed in the POD), see apple.com.

	http://developer.apple.com/techpubs/macosx/Carbon/

The documentation is also located on your system, if you have the Developer
Tools installed, at /Developer/Documentation/Carbon/.

Also of significant use are the related header files on your system.  Use
the `locate` command to find them.  They contain current documentation and
notes for the API.

The modules were written for Mac OS originally, and are in part being
ported to Carbon.  You may also be interested in the original documentation.

	http://developer.apple.com/techpubs/macos8/


=head1 TOOLBOX MAPPINGS

Swiped from Mac/Toolbox.pod in the MacPerl distribution.

The Macintosh Operating System provides a rich API with thousands of I<toolbox>
calls. The MacPerl toolbox modules aim to make as much as possible of this
functionality available to MacPerl programmers. The mapping of the toolbox 
interfaces into MacPerl is intended to be

=over 4

=item 1.

Convenient to use for Perl programmers.

=item 2.

As close as possible to the C interfaces.

=back

This translates into a mapping strategy which is discussed in the following 
sections.


=head2 Function mappings

MacPerl toolbox calls take their input arguments in the same order as the 
corresponding toolbox functions. Output arguments are never passed by reference, 
but returned from the calls. If there are several output arguments, a list is
returned. If an error occurs, the function returns C<undef> or C<()> and the 
error code is available in the C<$^E> variable.

	$port = GetPort();
	SetPort($port);
	$desc = AECreateDesc("TEXT", "Hello, World") or die $^E;


=head2 Data structure mappings

Complex data structures are mapped into blessed references. Data fields are 
available through member functions which return the value of a field if called
without an argument and change the value if called with an argument.

	$rect = Rect->new(10, 20, 110, 220);
	$rect->top;
	$rect->right(250);



=head1 MAC OS X DIFFERENCES

The modules follow the same API under Mac OS X as Mac OS, except that
the non-Carbon API is not supported (for example, C<NewHandle> is
supported, but C<NewHandleSys> is not).  Calling a function not
supported by Carbon will generate an exception.

In each module's documentation, functions that work only under Mac OS
(non-Carbon) are marked with B<Mac OS only.>  Those that work only
under Mac OS X (Carbon) are marked with B<Mac OS X only.>  A complete
list is at the end of this document.

The MacPerl package is automatically bootstrapped in MacPerl; it is
included here, though the app-specific functions (Reply, Quit) are not
supported, and the MacPerl package must be loaded explicitly (e.g.,
C<use MacPerl;>).  Also, Ask/Answer/Pick are provided via AppleScript,
talking to the SystemUIServer process.

The Mac-specific error codes are put in C<$^E> as in MacPerl, but C<$^E>
does not automatically convert the numeric error into a string in string
context.  See brian d foy's L<Mac::Errors> module on the CPAN for this:

	use Mac::Errors '$MacError';
	my $info1 = FSpGetCatInfo($file1) or die $^E + 0;    # error number
	my $info2 = FSpGetCatInfo($file2) or die $MacError;  # error string

L<Mac::Errors> is not included with or required by Mac::Carbon, but it is
highly recommended.

C<$!> is set at the same time C<$^E> is set.  This is different behavior
from MacPerl, but similar to other platforms.  On MacPerl, C<$^E> is
signed, and on Unix it is unsigned, so to get the numeric value from
C<$^E>, just add 0, as above.  Could be worse.

Files are passed back and forth using Unix/POSIX filespecs (if you care
about the gory details, a portion of the GUSI API has been reimplemented
here, and it handles the conversions).  Similarly, times are converted
back and forth from the Mac OS epoch to the Unix epoch.

The support functions are in F<Carbon.h>.  See that file for descriptions
of the issues, including bugs and possibilities for bugs, involved.


=head1 KNOWN BUGS

See L<http://rt.cpan.org/NoAuth/Bugs.html?Dist=Mac-Carbon> for more information.

=over 4

=item *

[GS]etFileInfo fails on UFS.

=item *

Need more tests for Mac::Components, Mac::MoreFiles, Mac::Resources,
Mac::InternetConfig, Mac::Processes, Mac::Sound.

=item *

In a few places, we need to know a text encoding, and assume it
(such as in LSFindApplicationForInfo(), where Latin-1 is assumed).
This is likely incorrect.

=item *

FSSpecs are limited to 31 characters.  Ugh.  Provide access to newer
FSRef-based APIs.

=item *

Not specific to the Carbon versions: the Mac:: modules define classes
such as C<Handle> which probably should be something else, like
C<Mac::Handle> or C<Mac::Carbon::Handle> or C<Mac::Memory::Handle>
(other examples include C<AEDesc>, C<Point>, C<Rect>).  No one has really
complained before except on principle, but still ...

=item *

Can we support XCMDs etc. via XL?  Do we want to?

=back



=head1 PACKAGES AND EXPORT TAGS

See each individual module for more information on use.  See F<README>
for more information about modules not included here.

	Mac::AppleEvents	appleevents
	Mac::Components		components
	Mac::Files		files
	Mac::Gestalt		gestalt
	Mac::InternetConfig	internetconfig
	Mac::Memory		memory
	Mac::MoreFiles		morefiles
	Mac::Notification	notification
	Mac::OSA		osa	
	Mac::Processes		processes
	Mac::Resources		resources
	Mac::Sound		sound
	Mac::Speech		speech
	Mac::Types		types
	MacPerl			macperl

=cut

package Mac::Carbon;

use strict;
use base 'Exporter';
use vars qw(@EXPORT @EXPORT_OK %EXPORT_TAGS $VERSION);

$VERSION = '0.71';

# we are just a frontend, so loop over the modules, and
# suck up everything in @EXPORT
BEGIN {
	my @modules = qw(
		AppleEvents
		Components
		Files
		Gestalt
		InternetConfig
		Memory
		MoreFiles
		Notification
		OSA
		Processes
		Resources
		Sound
		Speech
		Types
	);

	# oh oh, it's magic ...
	for (@modules) {
		no strict 'refs';
		my $mod = 'Mac::' . $_;
		eval "use $mod";
		die if $@;

		my @export = @{$mod . '::EXPORT'};
		push @EXPORT, @export;
		$EXPORT_TAGS{ lc $_ } = \@export;
	}

	# MacPerl is special, as almost everything is in EXPORT_OK
	use MacPerl ':all';
	push @EXPORT, @MacPerl::EXPORT, @MacPerl::EXPORT_OK;
	$EXPORT_TAGS{ 'macperl' } = [@MacPerl::EXPORT, @MacPerl::EXPORT_OK];

	@EXPORT_OK = @EXPORT;
	$EXPORT_TAGS{ 'all' } = \@EXPORT;
}

1;

__END__

=head1 UNSUPPORTED FUNCTIONS

=head2 Functions supported only in Mac OS

The functions below are supported only in Mac OS, and not in Mac OS X,
either because they are not supported by Carbon, or make no sense
on Mac OS X.

=over 4

=item Mac::AppleEvents::AECountSubDescItems

=item Mac::AppleEvents::AEDescToSubDesc

=item Mac::AppleEvents::AEGetKeySubDesc

=item Mac::AppleEvents::AEGetNthSubDesc

=item Mac::AppleEvents::AEGetSubDescBasicType

=item Mac::AppleEvents::AEGetSubDescData

=item Mac::AppleEvents::AEGetSubDescType

=item Mac::AppleEvents::AESubDescIsListOrRecord

=item Mac::AppleEvents::AESubDescToDesc

=item Mac::Files::Eject

=item Mac::InternetConfig::ICChooseConfig

=item Mac::InternetConfig::ICChooseNewConfig

=item Mac::InternetConfig::ICGeneralFindConfigFile

=item Mac::InternetConfig::ICGeneralFindConfigFile

=item Mac::InternetConfig::ICGetComponentInstance

=item Mac::InternetConfig::ICSetConfigReference

=item Mac::Memory::CompactMemSys

=item Mac::Memory::FreeMemSys

=item Mac::Memory::GetApplLimit

=item Mac::Memory::MaxBlockSys

=item Mac::Memory::MaxBlockSysClear

=item Mac::Memory::MaxMemSys

=item Mac::Memory::NewEmptyHandleSys

=item Mac::Memory::NewHandleSys

=item Mac::Memory::NewHandleSysClear

=item Mac::Memory::NewPtrSys

=item Mac::Memory::NewPtrSysClear

=item Mac::Memory::PurgeMemSys

=item Mac::Memory::ReserveMemSys

=item Mac::Processes::LaunchDeskAccessory

=item Mac::Resources::CreateResFile

=item Mac::Resources::OpenResFile

=item Mac::Resources::RGetResource

=item Mac::Sound::Comp3to1

=item Mac::Sound::Comp6to1

=item Mac::Sound::Exp1to3

=item Mac::Sound::Exp1to6

=item Mac::Sound::MACEVersion

=item Mac::Sound::SndControl

=item Mac::Sound::SndPauseFilePlay

=item Mac::Sound::SndRecordToFile

=item Mac::Sound::SndStartFilePlay

=item Mac::Sound::SndStopFilePlay

=item Mac::Sound::SPBRecordToFile

=item MacPerl::ErrorFormat

=item MacPerl::FAccess

=item MacPerl::LoadExternals

=item MacPerl::Quit

=item MacPerl::Reply

=back


=head2 Functions supported only in Mac OS X

The functions below are supported only in Mac OS X, and not in Mac OS,
either because they are newer APIs, or make no sense on Mac OS.

=over 4

=item Mac::Processes::GetProcessForPID

=item Mac::Processes::GetProcessPID

=item Mac::Processes::LSFundApplicationForInfo

=item Mac::Resources::FSCreateResourceFile

=item Mac::Resources::FSOpenResourceFile

=back


=head1 AUTHOR

The Mac Toolbox modules were written by Matthias Neeracher
E<lt>neeracher@mac.comE<gt>.  They were ported to Mac OS X and
are currently maintained by Chris Nandor E<lt>pudge@pobox.comE<gt>.

=head1 THANKS

Michael Blakeley,
Emmanuel. M. Decarie,
Matthew Drayton,
brian d foy,
David Hand,
Gero Herrmann,
Peter N Lewis,
Paul McCann,
Randal Schwartz,
John Siracusa,
Dan Sugalksi,
Ken Williams,
Steve Zellers.

=head1 SEE ALSO

perl(1).
