# 
# # Copyright (c) 1999 David Schooley.  All rights reserved.  This program is 
# free software; you can redistribute it and/or modify it under the same 
# terms as Perl itself.

########################################################################
#                                                                      #
#   Do the following:                                                  #
#    See if the app is running, if so, send the GetAETE event to it.   #
#    If it is not running, see if it has a sisz resource,              #
#      if so, launch it and send the GetAETE event to it.              #
#      if not, read in the aete directly.                              #
#                                                                      #
########################################################################

package Mac::AETE::App;

=head1 NAME

Mac::AETE::App - reads the Macintosh Apple event dictionary from an application.


=head1 SYNOPSIS

     use Mac::AETE::App;
     use Mac::AETE::Format::Dictionary;

     $app = App->new("My Application");
     $formatter = Dictionary->new;
     $app->set_format($formatter);
     $app->read;
     $app->write;


=head1 DESCRIPTION

The App module simplifies reading the Apple event dictionary from an application. 
It will launch the application if necessary to obtain the dictionary. 

=head2 Methods

=over 10

=item new

Example: ($name is the name of the application.)

     use Mac::AETE::App;
     use Mac::AETE::Format::Dictionary;

     $app = App->new($aete_handle, $name);

=item read

(Inherited from Mac::AETE::Parser.)

Reads the data contained in the AETE resource or handle. Example:
     
     $app->read;

=item set_format

(Inherited from Mac::AETE::Parser.)

Sets the output formatter used during by the 'write' subroutine. Example:

     $formatter = Dictionary->new;
     $app->set_format($formatter);
     

=item copy

(Inherited from Mac::AETE::Parser.)

Copies all suites from one Parser object into another. Example:
     
     $aete2 = Parser->new($aete_handle2, $another_name);
     $app->copy($aete2);
     
copies the suites from $aete2 into $aete.

=item merge

(Inherited from Mac::AETE::Parser.)

Merges suites from one Parser object into another. Only the suites that exist in
both objects will be replaced. Example:

     $aete3 = Parser->new($aete_handle2, $another_name);
     $app->merge($aete3);

=item write

(Inherited from Mac::AETE::Parser.)

Prints the contents of the AETE or AEUT resource using the current formatter.

     $app->write;

=back

=head1 INHERITANCE

Inherits from Mac::AETE::Parser.

=head1 AUTHOR

David Schooley <F<dcschooley@mediaone.net>>

=cut


use strict;
use File::Basename;
use File::Spec::Functions qw(catfile);
use Mac::AETE::Parser;
use Mac::AppleEvents::Simple ':all';
use Mac::Errors '$MacError';
use Mac::Files;
use Mac::Memory;
use Mac::Processes;
use Mac::Resources;
use Symbol;

use Carp;

@Mac::AETE::App::ISA = qw(Mac::AETE::Parser);

sub new {
    my ($type, $target) = @_;
    my $self = {};
    my $aete_handle;
    
    my($name, $running, $sign, $pname, $bundle) = &get_app_status_and_launch($target);
    return unless $name;

    $self->{_target}   = $name;
    $self->{ID}        = $sign;
    $self->{BUNDLE_ID} = $bundle;
    $self->{APPNAME}   = $pname;

    if ($running) {
        ($aete_handle, $self->{VERSION}) = get_aete_via_event($target, $sign);
        unless ($aete_handle) {
            $aete_handle = [];
#            carp("The application is not scriptable");
#            return;
        }
    } else {
        my $RF;
        if ($^O eq 'MacOS') {
            $RF = FSpOpenResFile($self->{_target}, fsRdPerm);
        } else {
            $RF = FSOpenResourceFile($self->{_target}, "rsrc", fsRdPerm) ||
                  FSOpenResourceFile($self->{_target}, "data", fsRdPerm);
        }
=pod

        if ( !defined($RF) || $RF == 0) {
            carp("No Resource Fork available for $target");
            return;
        }
        my $temp_handle = Get1Resource('aete', 0);
        if (!defined($temp_handle) || $temp_handle == 0) {
            carp("Application '$self->{_target}' is not scriptable (App.pm)");
            return;
        }
        $aete_handle = new Handle $temp_handle->get;
        CloseResFile($RF);
=cut
        unless ( !defined($RF) || $RF == 0) {
            my $temp_handle = Get1Resource('aete', 0);
            unless (!defined($temp_handle) || $temp_handle == 0) {
                $aete_handle = new Handle $temp_handle->get;
            }
            CloseResFile($RF);
        }
        $aete_handle ||= [];
    }

    my $newself = Mac::AETE::Parser->new($aete_handle, $target, $self);

    @{$newself}{keys %{$self}} = values %{$self};

    return bless $newself, $type;
}

sub get_app_status_and_launch
{
    my ($app_path) = @_;
    my ($name, $path, $suffix, $running, $ok_to_launch, $pname, $launch);
    my ($psn, $psi, $sign, $bundle);

    $running = 0;

#    fileparse_set_fstype("MacOS");
    ($name,$path,$suffix) = fileparse($app_path, "");

    # test for package, works under Mac OS X/Classic too
    my $pkginfo = catfile($app_path, 'Contents', 'PkgInfo');
    if (-d $app_path && -f $pkginfo) {
        my $fh = gensym();
        open $fh, "<" . $pkginfo or croak "Can't open $pkginfo: $!";
        (my($type), $sign) = (<$fh> =~ /^(.{4})(.{4})$/);

        my $infoplist = catfile($app_path, 'Contents', 'Info.plist');
        if (-f $infoplist) {
            my $fh = gensym();
            open $fh, "<" . $infoplist or croak "Can't open $infoplist: $!";
            my $text = join '', <$fh>;
            $text =~ s/\015/\n/g;
            my $i = 0;
            for (split /\n/, $text) {
                if ($i) {
                    m|^\s*<string>([^<\s]+)</string>\s*$|;
                    $bundle = $1;
                    last;
                } elsif (m|^\s*<key>CFBundleIdentifier</key>\s*$|) {
                    $i = 1;
                }
            }
        }

        for $psn (keys %Process) {
            my $psi = $Process{$psn};
            $pname = $psi->processName;
            if ($sign eq '????') {
	        $running = 1, $name = $pname, last
                    if $psi->processAppSpec =~ /^\Q$app_path/;
            } else {
	        $running = 1, $name = $pname, last
                    if $sign eq $psi->processSignature;
       	    }
        }
        $ok_to_launch = !$running;

    } else {
        for $psn (keys %Process) {
            my $psi = $Process{$psn};
            $pname = $psi->processName;
#            print "$pname", "   $name\n";
            $running = 1, last if $pname eq $name;
        }
    }

    if (!$running) {
        if (! -e $app_path) {
            carp("No such file or directory: $app_path");
            return;
        }
        unless (-d $app_path && -f $pkginfo) {
            my $RF;
            if ($^O eq 'MacOS') {
                $RF = FSpOpenResFile($app_path, fsRdPerm);
            } else {
                $RF = FSOpenResourceFile($app_path, "rsrc", fsRdPerm) ||
                      FSOpenResourceFile($app_path, "data", fsRdPerm);
            }

            if (!defined($RF) || $RF == 0) {
                carp("No Resource Fork available for '$app_path': $MacError");
                return;
            }
            my $check_resource =  Get1Resource('scsz', 0);
            if (!defined($check_resource) || $check_resource == 0) {
                $check_resource = Get1Resource('scsz', 128);
            }
            $ok_to_launch = defined($check_resource) && $check_resource;
            CloseResFile($RF); # don't do anything with the resource now!
        }
        if ($ok_to_launch) {
            $launch = new LaunchParam(
                launchControlFlags => eval(launchContinue + launchNoFileFlags + launchDontSwitch),
                launchAppSpec => $app_path
                );
            LaunchApplication $launch;
            $running = 1;
            sleep 10;
        }
    }

    for my $psn (keys %Process) {
        my $psi = $Process{$psn};
        $pname = $psi->processName;
        if (defined $sign && length($sign) && $sign ne '????') {
            $running = 1, $name = $psi->processName,
                last if $sign eq $psi->processSignature;
        } else {
            $running = 1, $sign = $psi->processSignature,
                last if $name eq $psi->processName;
        }
    }
    $name = $app_path if $name !~ /:/;
    ($name, $running, $sign, $pname, $bundle);
}

sub get_aete_via_event
{
    my($target, $sign) = @_;
    if (!$sign) {
        my $info = FSpGetFInfo($target);
        $sign = $info->fdCreator;
    }

    my @add;
    if ($sign eq '????') {
        for my $psn (keys %Process) {
            my $psi = $Process{$psn};
            if ($psi->processAppSpec =~ /^\Q$target/) {
                push @add, typeProcessSerialNumber, pack_psn($psn);
                last;
            }
        }
        die "Can't find process $target\n" unless @add;
    } else {
        push @add, typeApplSignature, $sign;
    }

    my $event = AEBuildAppleEvent('ascr', 'gdte', @add[0, 1], kAutoGenerateReturnID, kAnyTransactionID, "'----':0");
    my $reply = AESend($event, kAEWaitReply);
    my @handles;
    if ($reply) {
        my $result_desc = AEGetParamDesc($reply, keyDirectObject)
            or warn("Cannot get AETE: $MacError") and return;
        if ($result_desc->type eq typeAEList) {
            for (my $i = 1; $i <= AECountItems($result_desc); $i++) {
                my $tmp_desc = AEGetNthDesc($result_desc, $i)
                    or warn("Bad result from GetAETE!") and return;
                my $aete_handle = $tmp_desc->data
                    or warn("Bad result from GetAETE!") and return;
                my $aete = new Handle($aete_handle->get)
                    or warn("Bad result from GetAETE!") and return;
                push @handles, $aete;
            }
       } else {
            my $aete_handle = $result_desc->data
                or warn("Bad result from GetAETE!") and return;
            my $aete = new Handle($aete_handle->get)
                or warn("Bad result from GetAETE!") and return;
            push @handles, $aete;
        }
        AEDisposeDesc $result_desc;
        AEDisposeDesc $reply;
    } else {
        warn("Cannot get AETE: $MacError");
        return;
    }
    AEDisposeDesc $event;

    my $vers;
    $event = AEBuildAppleEvent('core', 'getd', @add[0, 1], kAutoGenerateReturnID, kAnyTransactionID,
        "'----':obj {want:type(prop), form:prop, seld:type(vers), from:'null'()}");
    $reply = AESend($event, kAEWaitReply);
    if ($reply) {
#        print AEPrint($reply), "\n";
        my($result_desc, $type);
        if ($result_desc = AEGetParamDesc($reply, keyDirectObject)) {
            $vers = $result_desc->get;
            if ($result_desc->type eq 'vers') {
                my @l = split(//, unpack("a7", $vers));
                $vers = unpack("x7a@{[ord($l[6])]}", $vers);
            } elsif ($result_desc->type eq 'utxt') {
            	my $char = AECoerceDesc($result_desc, typeChar);
            	$vers = $char->get;
            }
        }

        if ($result_desc) {
            AEDisposeDesc $result_desc;
        }

        AEDisposeDesc $reply;
    }
    AEDisposeDesc $event;

    (\@handles, $vers);
}

1;
