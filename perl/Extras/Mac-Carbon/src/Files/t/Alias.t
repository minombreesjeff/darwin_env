#!/usr/bin/perl
use Test::More;
use strict;
use warnings;

BEGIN { plan tests => 17 }

use File::Basename;
use File::Spec::Functions qw(catfile splitdir);
use MacPerl 'MakeFSSpec';
use Mac::Files;

SKIP: {
#	skip "Mac::Files Aliases", 12;

	# 0
	my $file = $INC{'Mac/Files.pm'};
	ok(-f $file,                                                 "file exists");
	ok(my $alias = NewAlias($file),                              "NewAlias");
	ok(-f(my $alias_path = ResolveAlias($alias)),                "ResolveAlias");
	is(basename($alias_path), 'Files.pm',                        "check name");

	# 4
	my $file2 = $INC{'Exporter.pm'};
	ok(UpdateAlias($file2, $alias),                              "UpdateAlias");
	ok(-f($alias_path = ResolveAlias($alias)),                   "ResolveAlias");
	is(basename($alias_path), 'Exporter.pm',                     "check name");

	# 8
	my @dirs = splitdir($alias_path);
	my @path;
	for my $n (0 .. $#dirs) {
		unshift @path, GetAliasInfo($alias, $n);
	}
	TODO: {
		local $TODO = _is_ufs(dirname($alias_path));
		is(catfile(@path), $alias_path,                      "GetAliasInfo");
	}

	# not entirely reliable, perhaps ...
	my $vol;
	if ($^O eq 'MacOS') {
		($vol = $alias_path) =~ s/^([^:]+?).+$/$1/;
	} else {
		if ($alias_path =~ m|^/Volumes/([^/]+)|) {
			$vol = $1;
		} else {
			$vol = "/";
		}
		($vol = MakeFSSpec($vol)) =~ s/^.+://;
	}

	# will fail if file actually IS on a server ... ?
	is(GetAliasInfo($alias, asiZoneName),   '',                  "asiZoneName");
	is(GetAliasInfo($alias, asiServerName), '',                  "asiServerName");
	is(GetAliasInfo($alias, asiVolumeName), $vol,                "asiVolumeName");
	is(GetAliasInfo($alias, asiAliasName),  $path[-1],           "asiAliasName");
	is(GetAliasInfo($alias, asiParentName), $path[-2],           "asiParentName");

	# 14
	ok($alias = NewAliasMinimal($file2),                         "NewAliasMinimal");
	is(ResolveAlias($alias), $alias_path,                        "ResolveAlias");	

    SKIP: {
#	skip "Mac::Files Aliases", 2;

	ok($alias = NewAliasMinimalFromFullPath($alias_path),        "NewAliasMinimalFromFullPath");
	is(ResolveAlias($alias), $alias_path,                        "ResolveAlias");
    }
}

sub _is_ufs {
	my($path) = @_;
	my($nblocks) = (stat($path))[12];
	return $nblocks ? "GetAliasInfo not working for UFS" : "";
}

__END__
