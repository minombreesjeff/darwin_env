;# pwd.pl - keeps track of current working directory in PWD environment var
;#
#
# This library is no longer being maintained, and is included for backward
# compatibility with Perl 4 programs which may require it.
#
# In particular, this should not be used as an example of modern Perl
# programming techniques.
#
# Suggested alternative: Cwd
#
;# $RCSfile: pwd.pl,v $$Revision: 1.4 $$Date: 2003/05/20 22:50:35 $
;#
;# $Log: pwd.pl,v $
;# Revision 1.4  2003/05/20 22:50:35  emoy
;# Update to Perl 5.8.1, including thread support and two level namespace.
;# Bug #: 3258028
;# Reviewed by: Jordan Hubbard
;#
;# Revision 1.3.2.1  2003/05/17 07:04:01  emoy
;# Branch PR3258028 - updating to Perl 5.8.1.  Turning on ithread support and
;# two level namespace.  Append prefix, installprefix, and standard paths to
;# darwin.hints file.  Use perl script to strip DSTROOT from Config.pm and
;# .packlist.
;#
;#
;# Usage:
;#	require "pwd.pl";
;#	&initpwd;
;#	...
;#	&chdir($newdir);

package pwd;

sub main'initpwd {
    if ($ENV{'PWD'}) {
	local($dd,$di) = stat('.');
	local($pd,$pi) = stat($ENV{'PWD'});
	if (!defined $dd or !defined $pd or $di != $pi or $dd != $pd) {
	    chop($ENV{'PWD'} = `pwd`);
	}
    }
    else {
	chop($ENV{'PWD'} = `pwd`);
    }
    if ($ENV{'PWD'} =~ m|(/[^/]+(/[^/]+/[^/]+))(.*)|) {
	local($pd,$pi) = stat($2);
	local($dd,$di) = stat($1);
	if (defined $pd and defined $dd and $di == $pi and $dd == $pd) {
	    $ENV{'PWD'}="$2$3";
	}
    }
}

sub main'chdir {
    local($newdir) = shift;
    $newdir =~ s|/{2,}|/|g;
    if (chdir $newdir) {
	if ($newdir =~ m#^/#) {
	    $ENV{'PWD'} = $newdir;
	}
	else {
	    local(@curdir) = split(m#/#,$ENV{'PWD'});
	    @curdir = '' unless @curdir;
	    foreach $component (split(m#/#, $newdir)) {
		next if $component eq '.';
		pop(@curdir),next if $component eq '..';
		push(@curdir,$component);
	    }
	    $ENV{'PWD'} = join('/',@curdir) || '/';
	}
    }
    else {
	0;
    }
}

1;
