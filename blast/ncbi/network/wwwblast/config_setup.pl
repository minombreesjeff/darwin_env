#!/usr/local/bin/perl

# make_config_files.pl
#
# the purpose of this script is to make it easier to set up the
# Standalone WWW Blast server.
#
# this script accepts as arguments a directory with formatted databases
# and a temporary directory (where it will write output files). for example
#
#   make_config_files.pl $BLASTDB out
#
# It then creates a blast.rc and a psiblast.rc file in this temporary 
# directory.  It also creates blastdb_options.html and 
# psiblastdb_options.html files in this directory.
# The blast.rc and psiblast.rc files can just replace the ones 
# in the blast directory.  You can then cut and paste the 
# <option> entries from the html files into the corresponding place
# in the blast.html, blast_cs.html, xmlblast.html, megablast.html, 
# psiblast.html, and psiblast_cs.html 
# (overwriting the test_aa_db and test_na_db <options>)
# 
# you may want to adjust the default number of processors in the rc files.
#
# finally you will probably want to create a symbolic link from the 
# blast database directory that you passed to this script to the
# db directory that currently contains the test_aa_db and test_na_db
# files.  Assuming you are in the blast directory created by the 
# blast server distribution, the following should work:
# 
#    mv db db.orig
#    ln -s $BLASTDB db
# 
# Author: Joseph Ryan <jfryan@nhgri.nih.gov>
#

use strict;
use vars qw($VERSION);

$VERSION = 0.01;

MAIN: {
    my $blastdb = $ARGV[0] or die "usage: $0 BLASTDB OUTPUT_DIR\n";
    my $output_dir = $ARGV[1] or die "usage:$0 BLASTDB OUTPUT_DIR\n";
    unless (-d $output_dir) {
        mkdir $output_dir, 0755 or die "cannot mkdir $output_dir:$!";
    }
    my ($ra_nts,$ra_aas) = get_dbs($blastdb);
    print_blast_html($output_dir,$ra_nts,$ra_aas);
    print_psiblast_config($output_dir,$ra_aas);
    print_blast_config($output_dir,$ra_nts,$ra_aas);
    print_psiblast_html($output_dir,$ra_aas);
    print "Four files were created in the directory: $output_dir\n";
}

sub print_psiblast_config {
    my $output_dir = shift;
    my $ra_aas     = shift;
    open OUT, ">$output_dir/psiblast.rc" 
         or die "cannot open >$output_dir/psiblast.rc:$!";
    print OUT "NumCpuToUse     4\n\n";
    print OUT "blastp ";
    foreach my $aa (@{$ra_aas}) {
        print OUT "$aa ";
    }
    print OUT "\n";
}

sub print_blast_config {
    my $output_dir = shift;
    my $ra_nts     = shift;
    my $ra_aas     = shift;
    open OUT, ">$output_dir/blast.rc" 
         or die "cannot open >$output_dir/blast.rc:$!";
    print OUT "NumCpuToUse     4\n\n";
    foreach my $p ("blastn", "tblastn", "tblastx") {
        print OUT "$p ";
        foreach my $nt (@{$ra_nts}) {
            print OUT "$nt ";
        }
        print OUT "\n";
    }
    foreach my $p ("blastp", "blastx") {
        print OUT "$p ";
        foreach my $aa (@{$ra_aas}) {
            print OUT "$aa ";
        }
        print OUT "\n";
    }
}

sub print_psiblast_html {
    my $dir    = shift;
    my $ra_aas = shift;
    open OUT, ">$dir/psiblastdb_options.html" 
         or die "cannot open >$dir/psiblastdb_options.html:$!";
    foreach my $aa (@{$ra_aas}) {
        print OUT "    <option>$aa\n";
    }
}

sub print_blast_html {
    my $dir    = shift;
    my $ra_nts = shift; 
    my $ra_aas = shift;
    my %redundants = ();
    open OUT, ">$dir/blastdb_options.html" 
         or die "cannot open $dir/blastdb_options.html:$!";
    my @dbs = sort @{$ra_nts}, @{$ra_aas};
    foreach my $db (@dbs) {
        next if $redundants{$db};
        print OUT "    <option>$db\n";
        $redundants{$db}++;
    }
}

sub get_dbs {
    my $dir = shift;
    my @nts = ();
    my @aas = ();

    opendir DIR, $dir or die "cannot open $dir:$!";
    my @files = grep /\.[np]in$/, readdir DIR;
    foreach my $f (@files) {
        $f =~ m/(.*)\.([pn])in$/;
        push @nts, $1 if ($2 eq 'n');
        push @aas, $1 if ($2 eq 'p');
    }
    return \@nts, \@aas;
}


