#!/usr/bin/perl
open(IN,"objcdd.h") or die "can't open objcdd.h file";
open(OUT,">temp123.h") or die "can't open temp file";
while(<IN>) {
s/struct SeqInt/struct seqint/;
s/struct Bioseq/struct bioseq/;
s/struct struct_Score/struct score/;
print OUT $_;
}
close(OUT);
close(IN);
unlink("objcdd.h");
rename("temp123.h", "objcdd.h");
